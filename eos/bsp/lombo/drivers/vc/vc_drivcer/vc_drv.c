#define DBG_SECTION_NAME	"VC_DRV"
#define DBG_LEVEL		DBG_LOG
#include <debug.h>
#include <rtthread.h>
#include <rtdef.h>
#include <csp.h>
#include <rtdevice.h>
#include "vc_core.h"
#include "vc_drv.h"
#include "irq_numbers.h"

#define VC_BASE_ADDR			0x01300000
#define VC_TOP_BASE			0x00000000
#define VC_PRE_BASE			0x00000100
#define VC_ENC_BASE			0x00000200
#define VC_DEC_BASE			0x00000400
#define VC_JPEG_BASE			0x00000500
#define VC_PREPROCESS_SRAM_PORT		55
#define VC_CABAC_SRAM_PORT		61

#define PRCM_BASE_ADDR			0x0400a000
#define AHB_GATE1_REG			0x0204
#define AHB_RST1_REG			0x0224
#define AXI_GAT1_REG			0x0340
#ifdef ARCH_LOMBO_N7V0
#define VC_PLL_ENABLE_REG		0x05a0
#define VC_PLL_STAT_REG			0x05b0
#define VC_PLL_ENABLE_FAC		0x05a4
#define VC_PLL_ENABLE_TUNE0		0x05a8
#define VC_PLL_CTRL_REG			0x09c0
#elif defined ARCH_LOMBO_N7V1
#define VC_TOP_CLOCK_CTRL			0xA0
#define VC_TOP_ADPLL_FAC			0xA4
#define VC_TOP_ADPLL_TUNE0			0xA8
#define VC_TOP_ADPLL_TUNE1			0xAC
#define VC_TOP_ADPLL_STAT			0xB0
#endif

#define ENC_STATUS_ADDR			0xC
#define DEC_STATUS_ADDR			0x10

#define VC_REG_FLAG			0
#define PRCM_REG_FLAG			1
#define VC_TIME_OUT			(5*RT_TICK_PER_SECOND)
#define MAX_VC_REG_RETRY_TIME		5
#define INT_VC				90


#ifdef RT_USING_DEVICE_OPS
static const struct rt_device_ops vc_ops {
	RT_NULL,
	vc_device_open,
	vc_device_close,
	RT_NULL,
	RT_NULL,
	vc_device_ioctl,
};
#endif


typedef struct {
	unsigned int top_reg[MAX_VC_TOP_REG_NUM];
	unsigned int pre_reg[MAX_VC_PRE_REG_NUM];
	unsigned int enc_reg[MAX_VC_ENC_REG_NUM];
	unsigned int dec_reg[MAX_VC_DEC_REG_NUM];
	unsigned int jpeg_reg[MAX_VC_JPEG_REG_NUM];
	unsigned int scale_reg[MAX_SCALE_PARM];
	unsigned int cabac_table[MAX_VC_CABAC_TABLE];
} vc_user_data_t;

typedef struct slot_s {
	unsigned long topptr;
	unsigned long encptr;
	unsigned long decptr;
	unsigned long jpegptr;
	unsigned int vc_status;
	unsigned int slice_mode;
	unsigned int vc_type; /* 1-h264_dec 2-jpeg_dec 3-h264_enc 4-jpeg_enc */
	int pid;
	vc_user_data_t user_data;
} slot_t;

typedef struct {
	struct rt_device vc_device;
	unsigned int vc_base;
	unsigned int prcm_base;
} vc_driver_t;

static struct rt_semaphore vc_sem;
static int vc_open_cnt = -1;
static int vc_occupied = -1;
static int vc_idle_flag = -1;
static int vc_irq_registered = -1;
static int vc_last_status = -1;
static slot_t slot;
static vc_driver_t *vc;
static int vc_enc_stream_full_flag = -1;
static int vc_dec_stream_empty_flag = -1;


static void vc_msleep(int sleep_time)
{
	rt_thread_delay(sleep_time/10);
}


static void *vc_drive_malloc(unsigned int size)
{
	return rt_malloc(size);
}

static void vc_drive_memset(void *ptr, int c, rt_ubase_t n)
{
	rt_memset(ptr, c, n);
}

/*
static void vc_drive_free(void *ptr)
{
	rt_free(ptr);
}
*/

static void vc_drive_memcpy(void *dest, const void *src, rt_ubase_t n)
{
	rt_memcpy(dest, src, n);
}


static void vc_write(u32 val, u32 offset, u32 reg,
		u32 flag_base) /* flag_base:0 - vc reg base;1 - prcm reg base */
{
	void *write_add;

	if (flag_base == VC_REG_FLAG) {
		write_add = (void *)(vc->vc_base + offset + reg);
		WRITEREG32(write_add, val);
	} else if (flag_base == PRCM_REG_FLAG) {
		write_add = (void *)(vc->prcm_base + offset + reg);
		WRITEREG32(write_add, val);
	} else {
		LOG_E("vc : write error flag_base\n");
	}
}

static unsigned int vc_read(u32 offset, u32 reg,
		u32 flag_base) /* flag_base:0 - vc reg base;1 - prcm reg base */
{
	unsigned int data;

	if (flag_base == VC_REG_FLAG) {
		data = READREG32((void *)(vc->vc_base + offset + reg));
		return data;
	} else if (flag_base == PRCM_REG_FLAG) {
		data = READREG32((void *)(vc->prcm_base + offset + reg));
		return data;
	} else {
		LOG_E("vc : write error flag_base\n");
		return 0;
	}
}


static int slot_reset()
{
	int ii;

	slot.slice_mode = 0;
	slot.pid = -1;
	slot.topptr = 0;
	slot.encptr = 0;
	slot.decptr = 0;
	slot.jpegptr = 0;
	slot.vc_type = 0;
	slot.vc_status = 0;

	for (ii = 0; ii < MAX_VC_TOP_REG_NUM; ii++)
		slot.user_data.top_reg[ii] = 0;
	for (ii = 0; ii < MAX_VC_PRE_REG_NUM; ii++)
		slot.user_data.pre_reg[ii] = 0;
	for (ii = 0; ii < MAX_VC_ENC_REG_NUM; ii++)
		slot.user_data.enc_reg[ii] = 0;
	for (ii = 0; ii < MAX_VC_JPEG_REG_NUM; ii++)
		slot.user_data.jpeg_reg[ii] = 0;
	for (ii = 0; ii < MAX_SCALE_PARM; ii++)
		slot.user_data.scale_reg[ii] = 0;
	for (ii = 0; ii < MAX_VC_CABAC_TABLE; ii++)
		slot.user_data.cabac_table[ii] = 0;

	return 0;
}


static inline void disable_vc_irq()
{
	unsigned int v, c;

	c = MAX_VC_REG_RETRY_TIME;
	v = vc_read(VC_TOP_BASE, 0x4, VC_REG_FLAG);
	v &= ~(1<<8);
	vc_write(v, VC_TOP_BASE, 0x4, VC_REG_FLAG);

	while ((vc_read(VC_TOP_BASE, 0x4, VC_REG_FLAG)) & (0x1<<8) && c-- > 0) {
		LOG_E("vc : can not disable irq\n");
		vc_write(v, VC_TOP_BASE, 0x4, VC_REG_FLAG);
	}
}

static void enable_vc_all_clock_on()
{
	unsigned int v;

	v = vc_read(VC_TOP_BASE, 0x4, VC_REG_FLAG);
	v |= (1<<30);
	vc_write(v, VC_TOP_BASE, 0x4, VC_REG_FLAG);
}

static void disable_vc_all_clock_on()
{
	unsigned int v;

	v = vc_read(VC_TOP_BASE, 0x4, VC_REG_FLAG);
	v &= ~(1<<30);
	vc_write(v, VC_TOP_BASE, 0x4, VC_REG_FLAG);
}

static void vc_waitforidle()
{
	int ctr = 0, v_en = 0, v_de = 0;

	v_en = vc_read(VC_TOP_BASE, 0xC, VC_REG_FLAG);
	v_de = vc_read(VC_TOP_BASE, 0x10, VC_REG_FLAG);

	if (((v_en & 0x1) == 0x1) || (((v_de & 0x1) == 0x1))) {
		LOG_E("vc : vc is on air\n");
		do {
			vc_msleep(200);
			ctr++;
			if (ctr == 100) {
				LOG_E("vc : vc always busy\n");
				break;
			}
		} while ((((vc_read(VC_TOP_BASE, 0xC, VC_REG_FLAG)) & 0x1)
			== 0x1) || ((((vc_read(VC_TOP_BASE, 0x10,
				VC_REG_FLAG)) & 0x1) == 0x1)));
	}
}


static void vc_clk_enable()
{
	unsigned int regval = 0;
	int retry_cnt = 10, times_retry = 0;

	/* AHB BUS CLOCK GATING */
	regval = vc_read(AHB_GATE1_REG, 0, PRCM_REG_FLAG);
	regval = regval | (1 << 12);
	vc_write(regval, AHB_GATE1_REG, 0, PRCM_REG_FLAG);

	/* AHB BUS Reset/VC Reset */
	regval = vc_read(AHB_RST1_REG, 0, PRCM_REG_FLAG);
	regval = regval & 0xffffefff;
	vc_write(regval, AHB_RST1_REG, 0, PRCM_REG_FLAG);
	vc_msleep(10);
	regval = regval | (1 << 12);
	vc_write(regval, AHB_RST1_REG, 0, PRCM_REG_FLAG);

	/* AXI BUS CLOCK GATING */
	regval = vc_read(AXI_GAT1_REG, 0, PRCM_REG_FLAG);
	regval = regval | (1 << 1);
	vc_write(regval, AXI_GAT1_REG, 0, PRCM_REG_FLAG);

#ifdef ARCH_LOMBO_N7V0
	/* set vc_pll clk */
	regval = ((25 & 0xff) << 8) | ((0 & 0x3) << 4) | ((1 & 0x3) << 0);
	vc_write(regval, VC_PLL_ENABLE_FAC, 0, PRCM_REG_FLAG);
	regval = 0x20B1D8;
	vc_write(regval, VC_PLL_ENABLE_TUNE0, 0, PRCM_REG_FLAG);

	/* enable pll: */
	/* enable ENM */
	regval = vc_read(VC_PLL_ENABLE_REG, 0, PRCM_REG_FLAG);
	regval = regval | (1 << 1);
	vc_write(regval, VC_PLL_ENABLE_REG, 0, PRCM_REG_FLAG);
	vc_msleep(10);
	/* enable ENP */
	regval = vc_read(VC_PLL_ENABLE_REG, 0, PRCM_REG_FLAG);
	regval = regval | (1 << 0);
	vc_write(regval, VC_PLL_ENABLE_REG, 0, PRCM_REG_FLAG);
	vc_msleep(10);

	while (1) {
		regval = vc_read(VC_PLL_STAT_REG, 0, PRCM_REG_FLAG);
		if ((regval & 0x7) == 7)
			break;
		else {
			vc_msleep(10);
			retry_cnt--;
			if (retry_cnt <= 0) {
				LOG_E("read vc pll reg stat error , break!");
				break;
			}
			LOG_E("read vc pll reg stat error, try again\n");
		}
	}

	/* enable OGAT */
	regval = vc_read(VC_PLL_ENABLE_REG, 0, PRCM_REG_FLAG);
	regval = regval | (1 << 2);
	vc_write(regval, VC_PLL_ENABLE_REG, 0, PRCM_REG_FLAG);
	/* VC clock select VC_PLL */
	regval = vc_read(VC_PLL_CTRL_REG, 0, PRCM_REG_FLAG);
	regval = regval | (1ul << 31) | 1;
	vc_write(regval, VC_PLL_CTRL_REG, 0, PRCM_REG_FLAG);
#elif defined ARCH_LOMBO_N7V1
	regval = 0x5 << 28 | 0x7 << 24 | 0x3 << 20 | 0x7 << 16 | 0x4 << 13 | 0x1 << 12 |
		0x1 << 8 | 0x2 << 4;
	vc_write(regval, VC_TOP_BASE, VC_TOP_ADPLL_TUNE1, VC_REG_FLAG);
	vc_msleep(10);

	regval = 0x1 << 8 | 0x19;
	vc_write(regval, VC_TOP_BASE, VC_TOP_ADPLL_FAC, VC_REG_FLAG);
	vc_msleep(10);

	regval = 0x1ul << 31 | 0x1 << 8 | 0x19;
	vc_write(regval, VC_TOP_BASE, VC_TOP_ADPLL_FAC, VC_REG_FLAG);

query_lock_status:
	vc_msleep(10);
	while (1) {
		regval = vc_read(VC_TOP_BASE, VC_TOP_ADPLL_STAT, VC_REG_FLAG);
		if ((regval & 0x80008000) == 0x80008000)
			break;
		else {
			vc_msleep(10);
			retry_cnt--;
			if (retry_cnt <= 0 && times_retry == 0) {
				/* disable ADPLL */
				vc_write(0x0, VC_TOP_BASE, VC_TOP_ADPLL_FAC,
							VC_REG_FLAG);
				/* set ADPLL n = 17 */
				regval = 0x1 << 8 | 0x11;
				vc_write(regval, VC_TOP_BASE, VC_TOP_ADPLL_FAC,
							VC_REG_FLAG);
				vc_msleep(10);

				regval = 0x1ul << 31 | 0x1 << 8 | 0x11;
				vc_write(regval, VC_TOP_BASE, VC_TOP_ADPLL_FAC,
							VC_REG_FLAG);
				vc_msleep(10);

				/* retry ADPLL set */
				regval = 0x1 << 8 | 0x19;
				vc_write(regval, VC_TOP_BASE, VC_TOP_ADPLL_FAC,
							VC_REG_FLAG);
				vc_msleep(10);

				regval = 0x1ul << 31 | 0x1 << 8 | 0x19;
				vc_write(regval, VC_TOP_BASE, VC_TOP_ADPLL_FAC,
							VC_REG_FLAG);

				retry_cnt = 10;
				times_retry = 1;
				LOG_E("read VC_TOP_ADPLL_STAT error, try again\n");
				goto query_lock_status;
			} else if (retry_cnt <= 0 && times_retry == 1) {
				retry_cnt = 10;
				LOG_E("read vc pll reg stat fail\n");
			}
		}
	}

	regval = 0x1ul << 31 | 0x1;
	vc_write(regval, VC_TOP_BASE, VC_TOP_CLOCK_CTRL, VC_REG_FLAG);

#endif
}

static void vc_clk_disable()
{
	unsigned int regval = 0;

	vc_waitforidle();
#ifdef ARCH_LOMBO_N7V0
	vc_write(regval, VC_PLL_ENABLE_REG, 0, PRCM_REG_FLAG);
#elif defined ARCH_LOMBO_N7V1
	vc_write(0x0, VC_TOP_BASE, VC_TOP_ADPLL_FAC, VC_REG_FLAG);
#endif

	/* AXI BUS CLOCK GATING */
	regval = vc_read(AXI_GAT1_REG, 0, PRCM_REG_FLAG);
	regval = regval & 0xFFFFFFFD;
	vc_write(regval, AXI_GAT1_REG, 0, PRCM_REG_FLAG);

	/* AHB BUS CLOCK GATING */
	regval = vc_read(AHB_GATE1_REG, 0, PRCM_REG_FLAG);
	regval = regval & 0xFFFFEFFF;
	vc_write(regval, AHB_GATE1_REG, 0, PRCM_REG_FLAG);
}

static void vc_drv_updatereg()
{
	int i;

	if (slot.vc_type == ENC_H264) {
		for (i = 0; i < MAX_VC_ENC_REG_NUM; i++) {
			slot.user_data.enc_reg[i] = (unsigned int)(vc_read(
				VC_ENC_BASE, i*4, VC_REG_FLAG));
			/* PRT_ERR("enc end : %d -- %x\n"
				,i,slot[id].user_data.enc_reg[i]); */
		}
	}
	if (slot.vc_type == DEC_H264) {
		for (i = 0; i < MAX_VC_DEC_REG_NUM; i++)
			slot.user_data.dec_reg[i] = (unsigned int)(vc_read(
				VC_DEC_BASE, i*4, VC_REG_FLAG));
	}
	if ((slot.vc_type == DEC_JPEG) || (slot.vc_type == ENC_JPEG)) {
		for (i = 0; i < MAX_VC_JPEG_REG_NUM; i++)
			slot.user_data.jpeg_reg[i] = (unsigned int)(vc_read(
				 VC_JPEG_BASE, i*4, VC_REG_FLAG));
	}
}

static void vc_isr(int vector, void *param)
{
	unsigned int s;

	rt_hw_interrupt_mask(INT_VC);
	vc_enc_stream_full_flag = 0;
	vc_dec_stream_empty_flag = 0;

	if ((slot.vc_type == DEC_H264) || (slot.vc_type == DEC_JPEG)) {
		s = vc_read(VC_TOP_BASE, 0x10, VC_REG_FLAG);
		if (s == 0x201)
			vc_dec_stream_empty_flag = 1;
		else
			vc_write(s, VC_TOP_BASE, 0x10, VC_REG_FLAG);
		disable_vc_irq();
		if (slot.vc_type == DEC_H264)
			disable_vc_all_clock_on();
	} else {
		s = vc_read(VC_TOP_BASE, 0x0C, VC_REG_FLAG);
		if (s == 0x101)
			vc_enc_stream_full_flag = 1;
		else
			vc_write(s, VC_TOP_BASE, 0x0C, VC_REG_FLAG);
		disable_vc_irq();
		if (slot.vc_type == ENC_H264)
			disable_vc_all_clock_on();
	}

	slot.vc_status = s;

	vc_drv_updatereg();

	/* when bistream empty or jpeg slice ready, and not frame ready. */
	if ((slot.vc_type == DEC_H264) || (slot.vc_type == DEC_JPEG)) {
		if (((s & (0x1<<9)) || (s & (0x1<<4))) && !(s & (0x1<<8))) {
			slot.slice_mode = 1;
		} else {
			slot.slice_mode = 0;
			vc_occupied = 0;
		}
	} else {
		/* PRT_ERR("s : %x\n", s); */
		if ((s & (0x1<<8))) {
			slot.slice_mode = 1;
		} else {
			slot.slice_mode = 0;
			vc_occupied = 0;
		}
	}

	if ((slot.vc_type == DEC_H264) || (slot.vc_type == DEC_JPEG)) {
		if (slot.vc_status & 0x8FFFF100)
			vc_last_status = -1;
	}

	vc_idle_flag = 1;
	rt_sem_release(&vc_sem);
}


static void vc_do_reset()
{
	unsigned int regval = 0;
	int retry_cnt = 10, times_retry = 0;

	regval = vc_read(AHB_RST1_REG, 0, PRCM_REG_FLAG);
	regval = regval & 0xffffefff;
	vc_write(regval, AHB_RST1_REG, 0, PRCM_REG_FLAG);
	vc_msleep(10);
	regval = regval | (1 << 12);
	vc_write(regval, AHB_RST1_REG, 0, PRCM_REG_FLAG);

#ifdef ARCH_LOMBO_N7V1
	regval = 0x5 << 28 | 0x7 << 24 | 0x3 << 20 | 0x7 << 16 | 0x4 << 13 | 0x1 << 12 |
		0x1 << 8 | 0x2 << 4;
	vc_write(regval, VC_TOP_BASE, VC_TOP_ADPLL_TUNE1, VC_REG_FLAG);
	vc_msleep(10);

	regval = 0x1 << 8 | 0x19;
	vc_write(regval, VC_TOP_BASE, VC_TOP_ADPLL_FAC, VC_REG_FLAG);
	vc_msleep(10);

	regval = 0x1ul << 31 | 0x1 << 8 | 0x19;
	vc_write(regval, VC_TOP_BASE, VC_TOP_ADPLL_FAC, VC_REG_FLAG);

query_lock_status:
	vc_msleep(10);
	while (1) {
		regval = vc_read(VC_TOP_BASE, VC_TOP_ADPLL_STAT, VC_REG_FLAG);
		if ((regval & 0x80008000) == 0x80008000)
			break;
		else {
			vc_msleep(10);
			retry_cnt--;
			if (retry_cnt <= 0 && times_retry == 0) {
				/* disable ADPLL */
				vc_write(0x0, VC_TOP_BASE, VC_TOP_ADPLL_FAC,
							VC_REG_FLAG);
				/* set ADPLL n = 17 */
				regval = 0x1 << 8 | 0x11;
				vc_write(regval, VC_TOP_BASE, VC_TOP_ADPLL_FAC,
							VC_REG_FLAG);
				vc_msleep(10);

				regval = 0x1ul << 31 | 0x1 << 8 | 0x11;
				vc_write(regval, VC_TOP_BASE, VC_TOP_ADPLL_FAC,
							VC_REG_FLAG);
				vc_msleep(10);

				/* retry ADPLL set */
				regval = 0x1 << 8 | 0x19;
				vc_write(regval, VC_TOP_BASE, VC_TOP_ADPLL_FAC,
							VC_REG_FLAG);
				vc_msleep(10);

				regval = 0x1ul << 31 | 0x1 << 8 | 0x19;
				vc_write(regval, VC_TOP_BASE, VC_TOP_ADPLL_FAC,
							VC_REG_FLAG);

				retry_cnt = 10;
				times_retry = 1;
				LOG_E("read VC_TOP_ADPLL_STAT error, try again\n");
				goto query_lock_status;
			} else if (retry_cnt <= 0 && times_retry == 1) {
				retry_cnt = 10;
				LOG_E("read vc pll reg stat fail\n");
			}
		}
	}

	regval = 0x1ul << 31 | 0x1;
	vc_write(regval, VC_TOP_BASE, VC_TOP_CLOCK_CTRL, VC_REG_FLAG);
#endif

	return;
}

static void vc_reset()
{
	vc_do_reset();
}

static void vc_drv_writereg(unsigned int regno,
			int offset, unsigned int value)
{
	vc_write(value, offset, regno*4, VC_REG_FLAG);
}

static void flush_dec_h264()
{
	int ii = 0;
	for (ii = 0; ii < MAX_VC_TOP_REG_NUM; ii++) {
		if (ii == 4)
			;
		else
			vc_drv_writereg(ii, VC_TOP_BASE,
				slot.user_data.top_reg[ii]);
	}
	for (ii = 0; ii < MAX_VC_DEC_REG_NUM; ii++)
		vc_drv_writereg(ii, VC_DEC_BASE,
			slot.user_data.dec_reg[ii]);

	if (vc_dec_stream_empty_flag != 1) {
		for (ii = 0; ii < MAX_VC_H264_DEC_CABAC_TABLE; ii++)
			vc_drv_writereg(VC_CABAC_SRAM_PORT, VC_DEC_BASE,
				slot.user_data.cabac_table[ii]);
	}
}

static void flush_dec_jpeg()
{
	int ii = 0;

	for (ii = 0; ii < MAX_VC_TOP_REG_NUM; ii++) {
		if (ii == 4)
			;
		else
			vc_drv_writereg(ii, VC_TOP_BASE,
				slot.user_data.top_reg[ii]);
	}
	for (ii = 0; ii < MAX_VC_JPEG_REG_NUM; ii++)
		vc_drv_writereg(ii, VC_JPEG_BASE,
			slot.user_data.jpeg_reg[ii]);
	if (vc_dec_stream_empty_flag != 1) {
		for (ii = 0; ii < MAX_VC_JPEG_DEC_CABAC_TABLE; ii++)
			vc_drv_writereg(VC_CABAC_SRAM_PORT, VC_JPEG_BASE,
				slot.user_data.cabac_table[ii]);
	}
}

static void flush_enc_h264()
{
	int ii = 0;

	for (ii = 0; ii < MAX_VC_TOP_REG_NUM; ii++) {
		if (ii == 3)
			;
		else
			vc_drv_writereg(ii, VC_TOP_BASE,
				slot.user_data.top_reg[ii]);
	}
	for (ii = 0; ii < MAX_VC_PRE_REG_NUM; ii++) {
		vc_drv_writereg(ii, VC_PRE_BASE,
			slot.user_data.pre_reg[ii]);
	}
	if (vc_enc_stream_full_flag != 1) {
		for (ii = 0; ii < MAX_SCALE_PARM; ii++) {
			vc_drv_writereg(VC_PREPROCESS_SRAM_PORT, VC_PRE_BASE,
				slot.user_data.scale_reg[ii]);
		}
	}
	for (ii = 0; ii < MAX_VC_ENC_REG_NUM; ii++) {
		vc_drv_writereg(ii, VC_ENC_BASE,
			slot.user_data.enc_reg[ii]);

	}
	if (vc_enc_stream_full_flag != 1) {
		for (ii = 0; ii < MAX_VC_H264_ENC_CABAC_TABLE; ii++) {
			vc_drv_writereg(VC_CABAC_SRAM_PORT, VC_ENC_BASE,
				slot.user_data.cabac_table[ii]);
		}
	}
}

static void flush_enc_jpeg()
{
	int ii = 0;

	for (ii = 0; ii < MAX_VC_TOP_REG_NUM; ii++) {
		if (ii == 3)
			;
		else
			vc_drv_writereg(ii, VC_TOP_BASE,
				slot.user_data.top_reg[ii]);
	}
	for (ii = 0; ii < MAX_VC_PRE_REG_NUM; ii++)
		vc_drv_writereg(ii, VC_PRE_BASE,
			slot.user_data.pre_reg[ii]);
	if (vc_enc_stream_full_flag != 1) {
		for (ii = 0; ii < MAX_SCALE_PARM; ii++)
			vc_drv_writereg(VC_PREPROCESS_SRAM_PORT, VC_PRE_BASE,
				slot.user_data.scale_reg[ii]);
	}
	for (ii = 0; ii < MAX_VC_JPEG_REG_NUM; ii++)
		vc_drv_writereg(ii, VC_JPEG_BASE,
			slot.user_data.jpeg_reg[ii]);

	if (vc_enc_stream_full_flag != 1) {
		for (ii = 0; ii < MAX_VC_JPEG_ENC_CABAC_TABLE; ii++)
			vc_drv_writereg(VC_CABAC_SRAM_PORT, VC_JPEG_BASE,
				slot.user_data.cabac_table[ii]);
	}
}

static void vc_drv_flushreg()
{
	if (slot.vc_type == DEC_H264)
		flush_dec_h264();
	else if (slot.vc_type == DEC_JPEG)
		flush_dec_jpeg();
	else if (slot.vc_type == ENC_H264)
		flush_enc_h264();
	else if (slot.vc_type == ENC_JPEG)
		flush_enc_jpeg();
	else
		LOG_E("--- error vc type ---\n");
}

static vc_status_t vc_query_status(int vc_type, unsigned int vc_status)
{

	if ((vc_type == DEC_H264) || (vc_type == DEC_JPEG)) {
		switch (vc_status) {
		case (0x1 << 4):
			return VC_DEC_STATUS_SLICE_FININSH_H264;
		case (0x3 << 4):
			return VC_DEC_STATUS_FININSH_H264;
		case (0x1 << 6):
			return VC_DEC_STATUS_FININSH_JPEG;
		case (0x3 << 6):
			return VC_DEC_STATUS_MBLINE_FININSH_JPEG;
		case (0x1 << 8):
			LOG_E("vc dec status error\n");
			return VC_DEC_STATUS_ERROR;
		case ((0x1 << 9) | 1):
			return VC_DEC_STATUS_EMPTY;
		case (0x1ul << 31):
			LOG_E("vc dec status time out\n");
			return VC_DEC_STATUS_TIMEOUT;
		default:
			LOG_E("vc dec status unkown error1 %x\n", vc_status);
			return VC_DEC_STATUS_ERROR;
		}
	} else {
		switch (vc_status) {
		case (0x1 << 4):
			return VC_ENC_STATUS_FINISH_H264;
		case (0x1 << 5):
			return VC_ENC_STATUS_FINISH_JPEG;
		case (0x1 << 6):
			return VC_ENC_STATUS_PREVIEW_FINISH;
		case ((0x1 << 8) | 1):
			return VC_ENC_STATUS_STREAM_FULL;
		case (0x1ul << 31):
			LOG_E("vc enc status time out\n");
			return VC_ENC_STATUS_TIMEOUT;
		default:
			LOG_E("vc enc status unkown error %x\n", vc_status);
			return VC_ENC_STATUS_ERROR;
		}
	}
}


static rt_err_t vc_device_open(rt_device_t dev, rt_uint16_t oflag)
{
	vc_open_cnt++;
	if (vc_open_cnt > 1) {
		LOG_E("vc_driver can not be open twice\n");
		return -1;
	}

	vc_clk_enable();
	disable_vc_irq();

	rt_sem_init(&vc_sem, "vc_sem", 0, RT_IPC_FLAG_FIFO);
	return 0;
}

static rt_err_t vc_device_close(rt_device_t dev)
{
	vc_clk_disable();
	vc_open_cnt--;
	slot_reset();
	vc_occupied = 0;
	rt_sem_detach(&vc_sem);
	return 0;
}

static rt_err_t vc_device_ioctl(rt_device_t dev, int cmd, void *arg)
{
	int s;
	struct reg_p *reg;

	switch (cmd) {
	case VC_S_TOPREG_PARAMS:
		reg = (struct reg_p *)arg;
		if (vc_occupied == 1) {
			if ((unsigned long)reg->register_p != slot.topptr) {
				LOG_E("vc : VC_RUN --- UNKNOWN_ERROR\n");
				return VC_STATUS_UNKOWNERROR;
			}
		}
		slot.topptr = (unsigned long)reg->register_p;
		vc_drive_memcpy(slot.user_data.top_reg, reg->register_p,
			reg->size);

		return 0;

	case VC_S_PREREG_PARAMS:
		reg = (struct reg_p *)arg;
		vc_drive_memcpy(slot.user_data.pre_reg, reg->register_p,
			reg->size);
		break;

	case VC_S_DECREG_PARAMS:
		reg = (struct reg_p *)arg;
		slot.decptr = (unsigned long)reg->register_p;
		vc_drive_memcpy(slot.user_data.dec_reg, reg->register_p,
			reg->size);
		break;

	case VC_S_ENCREG_PARAMS:
		reg = (struct reg_p *)arg;
		slot.encptr = (unsigned long)reg->register_p;
		vc_drive_memcpy(slot.user_data.enc_reg, reg->register_p,
			reg->size);
		break;

	case VC_S_JPEGREG_PARAMS:
		reg = (struct reg_p *)arg;
		slot.jpegptr = (unsigned long)reg->register_p;
		vc_drive_memcpy(slot.user_data.jpeg_reg, reg->register_p,
			reg->size);
		break;

	case VC_S_ENC_DEC_TABLE:
		reg = (struct reg_p *)arg;
		vc_drive_memcpy(slot.user_data.cabac_table, reg->register_p,
			reg->size);
		break;

	case VC_S_SCALE_PARAMS:
		reg = (struct reg_p *)arg;
		vc_drive_memcpy(slot.user_data.scale_reg, reg->register_p,
			reg->size);
		break;

	case VC_RUN:
		/* LOG_E("vc driver run\n"); */
		if (vc_idle_flag == 0) {
			LOG_E("vc is not idle\n");
			return VC_STATUS_UNKOWNERROR;
		}

		if (vc_last_status == -1) {
			vc_last_status = 0;
			vc_reset();
		}

		slot.vc_type = *((int *)arg);
		vc_drv_flushreg();
		vc_occupied = 1;
		vc_idle_flag = 0;

		s = 0;

		if (vc_enc_stream_full_flag == 1)
			s |= 0x101;
		else if (vc_dec_stream_empty_flag == 1)
			s |= 0x201;
		else
			s |= 1;

		if ((vc_enc_stream_full_flag != 1) && (vc_dec_stream_empty_flag != 1))
			rt_hw_interrupt_umask(INT_VC);

		if ((slot.vc_type == DEC_H264) || (slot.vc_type == DEC_JPEG)) {
			if (slot.vc_type == DEC_H264)
				enable_vc_all_clock_on();
			vc_write(s, VC_TOP_BASE, 0x10, VC_REG_FLAG);
		} else {
			if (slot.vc_type == ENC_H264)
				enable_vc_all_clock_on();
			vc_write(s, VC_TOP_BASE, 0xC, VC_REG_FLAG);
		}

		if ((vc_enc_stream_full_flag == 1) || (vc_dec_stream_empty_flag == 1))
			rt_hw_interrupt_umask(INT_VC);

		return 0;

	case VC_QUERY:
		if (vc_irq_registered) {
			vc_status_t s;
			rt_err_t sem_ret = RT_EOK;

			sem_ret = rt_sem_take(&vc_sem, VC_TIME_OUT);

			if (sem_ret != RT_EOK) {
				LOG_E("QUERY: wait timeout, or error\n");
				vc_reset();
				s = VC_STATUS_DEAD;
			} else {
				/* normal case */
				if (slot.vc_type == DEC_H264)
					vc_drive_memcpy((void *)slot.decptr,
						&(slot.user_data.dec_reg[0]),
						MAX_VC_DEC_REG_NUM * 4);
				else if (slot.vc_type == ENC_H264)
					vc_drive_memcpy((void *)slot.encptr,
						&(slot.user_data.enc_reg[0]),
						MAX_VC_ENC_REG_NUM * 4);
				else if ((slot.vc_type == ENC_JPEG)
					|| (slot.vc_type == DEC_JPEG))
					vc_drive_memcpy((void *)slot.jpegptr,
						&(slot.user_data.jpeg_reg[0]),
						MAX_VC_JPEG_REG_NUM * 4);
				else
					LOG_E("vc : vc type error\n");


				s = vc_query_status(slot.vc_type,
					slot.vc_status);
			}

			if ((slot.slice_mode == 0)
					||  (s == VC_STATUS_DEAD)) {
				slot_reset();
			}
			return s;
		} else {
			LOG_E("vc : should not be here\n");
			return -1;
		}
		break;
	default:
		LOG_E("vc : no such cmd 0x%x\n", cmd);
		return -1;
	}
	/* vde_reset_in_playing(); */
	return 0;
}

/*
	int vc_open()
	{
		return rt_device_open(&(vc->vc_device), 0);
	}

	int vc_close()
	{
		return rt_device_close(&(vc->vc_device));
	}

	int vc_ioctl(int cmd, void *arg)
	{
		return rt_device_control(&(vc->vc_device), cmd, arg);
	}
*/


static int vc_driver_prob()
{
	vc_driver_t *pvc = NULL;

	LOG_E("vc prob\n");
	pvc = (vc_driver_t *)vc_drive_malloc(sizeof(vc_driver_t));
	if (pvc == NULL) {
		LOG_E("malloc vc buffer error\n");
		return -1;
	}
	vc_drive_memset(pvc, 0, sizeof(struct rt_device));

	pvc->vc_base = VC_BASE_ADDR | VA_AC;
	pvc->prcm_base = PRCM_BASE_ADDR | VA_AC;

	rt_hw_interrupt_install(INT_VC, vc_isr, NULL, "vc_isr");

	vc_irq_registered = 1;
	vc_last_status = 0;
	vc_idle_flag = 1;
	vc_open_cnt = 0;

#ifdef RT_USING_DEVICE_OPS
	pvc->vc_device.ops = vc_ops;
#else
	pvc->vc_device.init = RT_NULL;
	pvc->vc_device.open = vc_device_open;
	pvc->vc_device.close = vc_device_close;
	pvc->vc_device.read = RT_NULL;
	pvc->vc_device.write = RT_NULL;
	pvc->vc_device.control = vc_device_ioctl;
#endif
	rt_device_register(&(pvc->vc_device), "vc_driver", RT_DEVICE_FLAG_RDWR);
	vc = pvc;

	return 0;
}
INIT_DEVICE_EXPORT(vc_driver_prob);

/*
static int vc_driver_remove()
{
	rt_device_unregister(&(vc->vc_device));
	if (vc != NULL) {
		vc_drive_free(vc);
		vc = NULL;
	}
	return 0;
}
*/
