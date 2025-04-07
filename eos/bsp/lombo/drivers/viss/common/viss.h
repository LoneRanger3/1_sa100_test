/*
 * Copyright (C) 2016-2018, LomboTech Co.Ltd.
 * Author: lomboswer <lomboswer@lombotech.com>
 *
 * VISS-VIC driver code for LomboTech
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef ___VISS__H___
#define ___VISS__H___

#include <rtthread.h>
#include <pthread.h>
#include <rtdevice.h>
#include <semaphore.h>
#include <rthw.h>
#include "spinlock.h"

#include <drivers/mmcsd_core.h>
#include <drivers/mmcsd_host.h>

#include "csp.h"
#include "irq_numbers.h"
#include "board.h"
#include "cfg/config_api.h"
#include "clk/clk.h"
#include "gpio/pinctrl.h"

#include "vframe_list_manager.h"
#include "viss_mediabus.h"
#include "viss_const.h"
#include "viss_i2c.h"
#include "include.h"

#define ENABLE_TRACE				0

#if (ENABLE_TRACE == 1)

#define PRT_TRACE_BEGIN(fmt, ...)		\
	LOG_D("|B|"fmt, ##__VA_ARGS__)
#define PRT_TRACE_END(fmt, ...)			\
	LOG_D("|E|"fmt, ##__VA_ARGS__)

#else

#define PRT_TRACE_BEGIN(fmt, ...)		do { } while (0)
#define PRT_TRACE_END(fmt, ...)			do { } while (0)

#endif

#define BUG_ON(__BUG_ON_COND)			\
	RT_ASSERT(!(__BUG_ON_COND))

#define CHK_RET_IF_GO(_ret, _lable)		\
	do {					\
		if (0 != _ret) {		\
			_ret = __LINE__;	\
			goto _lable;		\
		}				\
	} while (0)

/*
 * Enumeration of data inputs to the camera subsystem.
 */
enum viss_input {
	VISS_INPUT_PARALLEL_0	= 1,
	VISS_INPUT_PARALLEL_1,
	VISS_INPUT_MIPI_CSI2_0	= 3,
	VISS_INPUT_MIPI_CSI2_1,
	VISS_INPUT_WRITEBACK_A	= 5,
	VISS_INPUT_WRITEBACK_B,
	VISS_INPUT_WRITEBACK_ISP = 5,
};

/*
 * Enumeration of the VISS data bus types.
 */
enum viss_bus_type {
	/* Camera parallel bus */
	VISS_BUS_TYPE_ITU_601 = 1,
	/* Camera parallel bus with embedded synchronization */
	VISS_BUS_TYPE_ITU_656,
	/* Camera MIPI-CSI2 serial bus */
	VISS_BUS_TYPE_MIPI_CSI2,
	/* FIFO link from LCD controller (WriteBack A) */
	VISS_BUS_TYPE_LCD_WRITEBACK_A,
	/* FIFO link from LCD controller (WriteBack B) */
	VISS_BUS_TYPE_LCD_WRITEBACK_B,
	/* FIFO link from FIMC-IS */
	VISS_BUS_TYPE_ISP_WRITEBACK = VISS_BUS_TYPE_LCD_WRITEBACK_B,
};

#define viss_input_is_parallel(x) ((x) == 1 || (x) == 2)
#define viss_input_is_mipi_csi(x) ((x) == 3 || (x) == 4)

enum tvd_state {
	TVD_ST_IDLE,
	TVD_ST_PLUG_IN,
	TVD_ST_LOCKED,
	TVD_ST_SIG_ESTAB,
	TVD_ST_UNLOCKED,
	TVD_ST_SIG_CHANGE,
	TVD_ST_PLUG_OUT,
};

enum viss_dev_stat {
	/* capture node */
	ST_PREV_PEND,
	ST_PREV_ISP_STREAM,
	ST_PREV_SHUT,
	ST_PREV_BUSY,
	ST_PREV_APPLY_CFG,
	ST_PREV_JPEG,
	ST_PREV_LPM,
	ST_PREV_PENDING,
	ST_PREV_RUN,
	ST_PREV_STREAM,
	ST_PREV_SUSPENDED,
	ST_PREV_OFF,
	ST_PREV_IN_USE,
	ST_PREV_CONFIG,
	ST_SENSOR_STREAM,
	ST_LPM,
	/* m2m node */
	ST_M2M_RUN,
	ST_M2M_PEND,
	ST_M2M_SUSPENDING,
	ST_M2M_SUSPENDED,
};

enum viss_datapath {
	VISS_IO_NONE,
	VISS_IO_CAMERA,
	VISS_IO_DMA,
	VISS_IO_LCDFIFO,
	VISS_IO_WRITEBACK,
	VISS_IO_ISP,
};

/*
 * The subdevices' group IDs.
 */
#define GRP_ID_SENSOR		(1 << 8)
#define GRP_ID_MCSI		(1 << 9)
#define GRP_ID_VIC		(1 << 10)
#define GRP_ID_TVD		(1 << 11)
#define GRP_ID_VISS		(1 << 12)
#define GRP_ID_VISS_IS		(1 << 13)
#define GRP_ID_ISP		(1 << 14)
#define GRP_ID_ISP_PREV		(1 << 15)
#define GRP_ID_ISP_CAP		(1 << 16)

#define GRP_CH_ISP_CAP		(1 << 0)
#define GRP_CH_ISP_PREV		(1 << 1)

#define VISS_MAX_PLANES				3
#define VISS_MAX_DMA_CH				4

#define VISS_SD_PAD_SINK			0
#define VISS_SD_PAD_SOURCE_DMA			1
#define VISS_SD_PAD_SOURCE_ISP			2
/*for VIC/MCSI */
#define VISS_SD_PADS_NUM			3
/* for TVD/ISP/ISP-LITE there are no source to isp */
#define VISS_TVD_SD_PADS_NUM			2
#define VISS_REQ_BUFS_MIN			2
#define VISS_MAX_SRCCLKS			2

#define BT601_TIMING_PARA_CNT			6
struct bt601_timing {
	rt_uint32_t	h_off;
	rt_uint32_t	active_w;
	rt_uint32_t	t_off;
	rt_uint32_t	t_active_l;
	rt_uint32_t	b_off;
	rt_uint32_t	b_active_l;
};
/* see also http://vektor.theorem.ca/graphics/ycbcr/ */
enum dev_colorspace {
	/* ITU-R 601 -- broadcast NTSC/PAL */
	DEV_COLORSPACE_SMPTE170M     = 1,

	/* 1125-Line (US) HDTV */
	DEV_COLORSPACE_SMPTE240M     = 2,

	/* HD and modern captures. */
	DEV_COLORSPACE_REC709        = 3,

	/* broken BT878 extents (601, luma range 16-253 instead of 16-235) */
	DEV_COLORSPACE_BT878         = 4,

	/* These should be useful.  Assume 601 extents. */
	DEV_COLORSPACE_470_SYSTEM_M  = 5,
	DEV_COLORSPACE_470_SYSTEM_BG = 6,

	/* I know there will be cameras that send this.  So, this is
	 * unspecified chromaticities and full 0-255 on each of the
	 * Y'CbCr components
	 */
	DEV_COLORSPACE_JPEG          = 7,

	/* For RGB colourspaces, this is probably a good start. */
	DEV_COLORSPACE_SRGB          = 8,
};

/**
 * struct viss_source_info - video source description required for the host
 *			     interface configuration
 *
 * @board_info: pointer to I2C subdevice's board info
 * @clk_frequency: frequency of the clock the host interface provides to sensor
 * @if_type: VISS camera input interface type, MIPI, ITU-R BT.601 etc
 *		for VIC: 0: dc; 1: BT.601; 2: BT.656; 3: BT.1120;
 * @flags: the parallel sensor bus flags defining signals polarity (V4L2_MBUS_*)
 * @i2c_bus_num: i2c control bus id the sensor is attached to
 * @host: index of the host
 */
struct viss_source_info {
	rt_uint32_t  channel_num;
	char        i2c_bus_name[8];
	rt_uint32_t i2c_addr;
	rt_uint32_t i2c_clk_frequency;
	rt_uint32_t mclk_freq;
	rt_uint32_t bus_type;  /* enum viss_bus_type */
	rt_uint32_t sensor_bus_type; /* enum viss_bus_type */
	rt_uint32_t out_path;  /* enum viss_datapath */
	rt_uint32_t if_type;   /* enum viss_if_type */
/* if_mode:
 *	for VISS_IF_TYPE_DC:
 *		mode 0: RAW8
 *		mode 1: RAW10
 *		mode 2: RAW12
 *
 *	for VISS_IF_TYPE_ITU_601:
 *		mode 0: 8-bit HSYNC VSYNC
 *		mode 1: 8-bit HSYNC FIELD
 *		mode 2: 8-bit HSYNC VSYNC FIELD
 *		mode 3: 16-bit HSYNC VSYNC
 *
 *	for VISS_IF_TYPE_ITU_656:
 *		mode 0: 8-bit
 *
 *	for VISS_IF_TYPE_ITU_1120 = 3:
 *		mode 0: 16-bit
 *		mode 1: 8-bit
 *
 *	for VISS_IF_TYPE_MCSI:
 *		mode 0: RAW8
 *		mode 1: RAW10
 *		mode 2: RAW12
 *      VISS_IF_TYPE_TVD:
 *		ignore
 */
	rt_uint32_t if_mode;
	rt_uint32_t flags;
	rt_uint32_t interlaced;

	rt_uint32_t field_sel;		/* input field selection, enum viss_field_sel*/
	rt_uint32_t vref;	/* input vref signal polarity, enum viss_ref_signal */
	rt_uint32_t href;	/* input href signal polarity, enum viss_ref_signal */
	rt_uint32_t pclk;       /* input data valid of the input clock edge type,
				 * enum viss_clk_active_type*/
	rt_uint32_t mipi_csi_clock;
	rt_uint32_t viss_top_freq;
	rt_uint32_t isp_top_freq;
	rt_uint32_t data_lanes;
	/*
	 *	discrete = 0
	 *	left_and_right = 1
	 *	up_and_down = 2
	 */
	rt_uint32_t frame_buf_type;
	struct bt601_timing bt601_timing;
	rt_uint16_t host_id;
	rt_uint16_t mux_id;
	rt_uint8_t clk_id;
};

/*
 * v4l2_device notification id. This is only for internal use in the kernel.
 * Sensor subdevs should issue N7_VISS_TX_END_NOTIFY notification in single
 * frame capture mode when there is only one VSYNC pulse issued by the sensor
 * at begining of the frame transmission.
 */
#define N7_VISS_TX_END_NOTIFY _IO('e', 0)
#define VISS_MAX_PLANES	3
#define FMT_FLAGS_RAW_BAYER	(1 << 0)
#define FMT_FLAGS_YUV		(1 << 1)
#define FMT_FLAGS_CAM		(1 << 2)
#define FMT_FLAGS_M2M_IN	(1 << 3)
#define FMT_FLAGS_M2M_OUT	(1 << 4)
#define FMT_FLAGS_M2M		(1 << 3 | 1 << 4)
#define FMT_HAS_ALPHA		(1 << 5)
#define FMT_FLAGS_COMPRESSED	(1 << 6)
#define FMT_FLAGS_WRITEBACK	(1 << 7)

/*
#define FMT_FLAGS_M2M_IN	(1 << 3)
#define FMT_FLAGS_M2M_OUT	(1 << 4)
#define FMT_FLAGS_M2M		(FMT_FLAGS_M2M_IN | FMT_FLAGS_M2M_OUT)
*/

/**
 * struct viss_fmt - color format data structure
 * @mbus_code: media bus pixel code, -1 if not applicable
 * @name: format description
 * @fourcc: fourcc code for this format, 0 if not applicable
 * @color: the driver's private color format id
 * @memplanes: number of physically non-contiguous data planes
 * @colplanes: number of physically contiguous data planes
 * @depth: per plane driver's private 'number of bits per pixel'
 * @mdataplanes: bitmask indicating meta data plane(s), (1 << plane_no)
 * @flags: flags indicating which operation mode format applies to
 */
struct viss_fmt {
	enum viss_mbus_pixelcode mbus_code;
	char	*name;
	rt_uint32_t	fourcc;
	rt_uint32_t	color;
	rt_uint16_t	memplanes;
	rt_uint16_t	colplanes;
	rt_uint8_t	depth[VISS_MAX_PLANES];
	rt_uint16_t	mdataplanes;
	rt_uint16_t	flags;
};

enum viss_subdev_index {
	IDX_SENSOR,
	IDX_MCSI,
	IDX_VIC,
	IDX_TVD,
	IDX_IS_ISP,
	IDX_VISS,
	IDX_MAX,
};

struct viss_sclk {
	const char *viss_sclk;
	const char *parrent;
};

struct viss_clk {
	struct viss_sclk sclk[VISS_MAX_SRCCLKS];
	const char *viss_reset;
	const char *viss_gate;
	const char *viss_axi_gate;
};

/**
 * struct viss_dma_offset - pixel offset information for DMA
 * @y_h:	y value horizontal offset
 * @y_v:	y value vertical offset
 * @cb_h:	cb value horizontal offset
 * @cb_v:	cb value vertical offset
 * @cr_h:	cr value horizontal offset
 * @cr_v:	cr value vertical offset
 */
struct viss_dma_offset {
	rt_int32_t	y_h;
	rt_int32_t	y_v;
	rt_int32_t	cb_h;
	rt_int32_t	cb_v;
	rt_int32_t	cr_h;
	rt_int32_t	cr_v;
};

struct viss_events {
	rt_uint32_t p0_overflow;
	rt_uint32_t p1_overflow;
	rt_uint32_t p2_overflow;
};

struct viss_rect {
	rt_int32_t   left;
	rt_int32_t   top;
	rt_int32_t   width;
	rt_int32_t   height;
};

struct viss_addr {
	rt_uint32_t    align;
	rt_uint32_t	y;
	rt_uint32_t	cb;
	rt_uint32_t	cr;
	rt_uint32_t	y_size;
	rt_uint32_t	cb_size;
	rt_uint32_t	cr_size;
#ifdef ARCH_LOMBO_N7V1_SAR
	rt_bool_t       val_flag;
#endif
};

/**
 * struct viss_isp_exif - structure for the EXIF information of VISS
 * @exposure_time: exposure time(s) * 10000
 * @shutter_speed:  exposure time(s) * 10000
 * @aperture: aperture fno2.8 = 280
 * @brightness: LV = [0, 20], 0: star light, 20: sun light
 * @exposure_bias: it calls also EV bias
 * @iso_speed: ISO = gain * 100
 * @flash: status register value of the flash
 * @illu_id: AWB temperature id
 *    0: HOR(2100K)
 *    1: A(2900K)
 *    2: TL84(4000K)
 *    3: CWF(4100K)
 *    4: D50(5000K)
 *    5: D65(6500K)
 *    6: D75(7500K)
 * @back_score: back light score = [0, 100] percent
 * @res: reserved info
 * @res[0]: object luminance
 * @res[1]: back luminance
 * @res[2]: average luminance
 * @res[3]: original target
 * @res[4]: final target
 */
struct viss_isp_exif {
	rt_uint32_t exposure_time;
	rt_uint32_t shutter_speed;
	rt_uint32_t aperture;
	rt_uint32_t brightness;
	rt_uint32_t exposure_bias;
	rt_uint32_t iso_speed;
	rt_uint32_t flash;
	rt_uint32_t illu_id;
	rt_uint32_t back_score;
	rt_uint32_t res[16];
};

/**
 * struct viss_isp_reg_seqdata - isp register sequence
 * @reg_size:	isp register sequence size
 * @reg_add:	register address
 * @reg_value:	register value
 */
struct viss_isp_reg_seqdata {
	rt_uint32_t reg_size;
	rt_uint32_t *reg_add;
	rt_uint32_t *reg_value;
};

/**
 * struct viss_isp_reg_data - isp register data
 * @reg_add:	register address
 * @reg_value:	register value
 */
struct viss_isp_reg_data {
	rt_uint32_t reg_add;
	rt_uint32_t reg_value;
};

/**
 * struct viss_isp_stat_data - isp statistics data
 * @bgm_buf:	isp bayer grid measurement
 * @vsm_buf:	isp video stabilization measurement
 */
struct viss_isp_stat_data {
	void *bgm_buf;
	void *vsm_buf;
	rt_uint32_t bgm_buf_size;
	rt_uint32_t vsm_buf_size;
	rt_uint32_t frame_number;
};

/**
 * struct isp_exp_gain - isp expsure control data
 * @exp:	exp time
 * @gain:	gain
 */
struct isp_exp_gain {
	rt_uint32_t exp;
	rt_uint32_t gain;
};

struct isp_sensor_info {
	char *sensor_name;
	rt_uint32_t pclk;
	rt_uint32_t vts;
	rt_uint32_t hts;
	rt_uint32_t input_width;
	rt_uint32_t input_height;
	rt_uint32_t output_widht;
	rt_uint32_t output_height;
	rt_uint32_t buswidth;
	rt_uint32_t mode;
	rt_uint32_t field_sel;
	rt_uint32_t yuv_seq;
	rt_uint32_t yuv_ds_mode;
	rt_uint32_t bayer_mode;
	rt_uint32_t hpol;
	rt_uint32_t vpol;
	rt_uint32_t edge;
};

struct viss_buffer {
	struct list_head list;
	rt_int32_t channel_id;   /* output channel index */
	rt_uint32_t frame_id;
	rt_int32_t frame_rate;

	rt_int64_t pts;
	rt_int32_t pts_valid;

	rt_int32_t pix_format;     /* videodev2.h */
	rt_int32_t color_space;

	struct viss_rect src_rect; /* source valid size */
	struct viss_rect dst_rect; /* source display size, use crop*/
	struct viss_addr paddr[VISS_MAX_DMA_CH]; /* video frame buffer address */
};

struct viss_frame_queue {
	rt_int32_t  all_num;   /* all channel frames number*/
	struct viss_buffer *frame;
};

struct viss_reqframe {
	rt_int32_t channel_id;
	struct viss_buffer *buf;
};

struct dev_mode {
	rt_int32_t index;
	rt_uint32_t inp_fmt; /* viss-mediabus.h enum viss_mbus_pixelcode */
	rt_uint32_t out_fmt; /* videodev2.h */
	rt_int32_t colorspace;

	struct viss_rect frame_size;
	rt_uint32_t frame_rate;

	rt_uint32_t usr_data_size;
	void *usr_data;
};

struct dev_all_mode {
	struct dev_mode *mode;
	rt_int32_t num;
};

struct viss_pts_callback {
	rt_int32_t (*get_pts)(rt_int64_t *pts, void *para);
	void *para;
};

struct video_dev {
	const char *name;
	rt_uint32_t group_id;
	struct dev_mode *cur_mode;
	struct viss_source_info info;

	struct viss_i2c_client *i2c_client;
	struct pinctrl *pctrl;
	rt_int32_t  pwdn_valid;
	rt_int32_t  pwdn_gpio;
	rt_uint32_t pwdn_val[7]; /*port, pin, function, drv_level, pud, pud_res, data*/

	rt_int32_t  rst_valid;
	rt_int32_t  rst_gpio;
	rt_uint32_t rst_val[7];/*port, pin, function, drv_level, pud, pud_res, data*/

	rt_int32_t  mclk_valid;
	rt_int32_t  mclk_gpio;
	rt_uint32_t mclk_val[7];/*port, pin, function, drv_level, pud, pud_res, data*/

	void *user_data;

	rt_err_t (*prepare)(void *hdl);
	rt_err_t (*init)(void *hdl);
	void (*exit)(void *hdl);

	rt_err_t (*s_mode)(void *hdl, rt_int32_t index); /* select use mode */
	struct dev_mode *(*g_cur_mode)(void *hdl); /* get current mode */
	struct dev_mode *(*g_all_mode)(void *hdl, rt_int32_t *num);
	rt_err_t (*s_power)(void *hdl, rt_bool_t on);     /* power control */
	rt_err_t (*s_stream)(void *hdl, rt_bool_t enable);
	rt_err_t (*g_info)(void *hdl, struct viss_source_info *info);
	rt_err_t (*ioctl)(void *hdl, rt_int32_t cmd, void *para);
	rt_err_t (*s_register)(void *hdl, struct viss_dbg_register *reg);
	rt_err_t (*g_register)(void *hdl, struct viss_dbg_register *reg);
};

#if defined(ARCH_LOMBO_N7V1_TDR) && defined(ARCH_LOMBO_N7V1_CDR)
struct vic_det_dev {
	struct rt_timer back_timer_1;
	struct gpio_irq_data back_in_det_gpio_irq_1;
	rt_uint32_t back_in_det_gpio_1;
	rt_uint32_t back_in_det_pctrl_1[2];/*port, pin*/
	rt_int32_t  back_status_1;

	struct rt_timer back_timer_2;
	struct gpio_irq_data back_in_det_gpio_irq_2;
	rt_uint32_t back_in_det_gpio_2;
	rt_uint32_t back_in_det_pctrl_2[2];/*port, pin*/
	rt_int32_t  back_status_2;

	struct rt_timer back_timer_3;
	struct gpio_irq_data back_in_det_gpio_irq_3;
	rt_uint32_t back_in_det_gpio_3;
	rt_uint32_t back_in_det_pctrl_3[2];/*port, pin*/
	rt_int32_t  back_status_3;

	struct rt_timer back_timer_4;
	struct gpio_irq_data back_in_det_gpio_irq_4;
	rt_uint32_t back_in_det_gpio_4;
	rt_uint32_t back_in_det_pctrl_4[2];/*port, pin*/
	rt_int32_t  back_status_4;

	struct pinctrl *pctrl;
	rt_int32_t  av_status;
};

struct vic_det_status {
	rt_int32_t  av_status;
	rt_int32_t  dms_status;
	rt_int32_t  back_status;
};
#else
#if defined(ARCH_LOMBO_N7V1_SAR) || defined(ARCH_LOMBO_N7V1_CDR)
struct vic_det_dev {
	spinlock_t av_det_spinlock;
	spinlock_t dms_det_spinlock;
	spinlock_t back_det_spinlock;
	struct rt_timer av_timer;
	struct rt_timer dms_timer;
	struct rt_timer back_timer;
	struct gpio_irq_data av_in_det_gpio_irq;
	struct gpio_irq_data av_dms_det_gpio_irq;
	struct gpio_irq_data back_in_det_gpio_irq;
#ifndef RT_USING_VIC_DET_SIGNAL
	rt_uint32_t av_in_det_gpio;
	rt_uint32_t av_in_det_pctrl[2];/*port, pin*/
	rt_uint32_t av_dms_det_gpio;
	rt_uint32_t av_dms_det_pctrl[2];/*port, pin*/
#endif
	rt_uint32_t back_in_det_gpio;
	rt_uint32_t back_in_det_pctrl[2];/*port, pin*/
	rt_int32_t  av_status;
	rt_int32_t  dms_status;
	rt_int32_t  back_status;
	struct pinctrl *pctrl;
};

struct vic_det_status {
	rt_int32_t  av_status;
	rt_int32_t  dms_status;
	rt_int32_t  back_status;
};

enum vic_det {
	AV_IN_DET,
	AV_DMS_DET,
	BACK_IN_DET,
};
#endif
#endif

struct viss_dev {
	rt_uint32_t group_id;
	void	*link_dev;  /* use isp link vic or mcsi */
	spinlock_t spinlock;
	pthread_mutex_t lock;
	rt_uint32_t irqno;
	rt_uint32_t irqno_isp;
	rt_uint32_t channel_num;          /* video input channel number */
	rt_int32_t  state[VISS_MAX_DMA_CH];  /* channel status */
	rt_uint32_t isr_count;
	rt_bool_t buf_flag;
	rt_bool_t get_buf_flag;
	rt_sem_t    irq_sem[VISS_MAX_DMA_CH];
	struct viss_buffer *prv_frame_b[VISS_MAX_DMA_CH];
	struct viss_buffer *prv_frame[VISS_MAX_DMA_CH];
	struct viss_buffer *prv_frame_temporary[VISS_MAX_DMA_CH];
	rt_uint32_t list_frame_num[VISS_MAX_DMA_CH];
	enum viss_if_type if_type;  /* input interface type */
	struct viss_buffer *frame[VISS_MAX_DMA_CH];
	struct vlist *free[VISS_MAX_DMA_CH];
	struct vlist *full[VISS_MAX_DMA_CH];
	struct viss_pts_callback pts_cb;
	rt_sem_t wait_frm_sem;
	rt_sem_t mcsi_timeout_sem;
	rt_sem_t vic_det_sem;
	rt_sem_t isp_drv_wait_sem;
	rt_uint32_t ref_count;
	rt_uint32_t create_count;
	rt_uint32_t init_count;
	rt_uint32_t stream_count;

	pthread_t isp_tid;
	pthread_t wdt_tid;
	pthread_t det_tid;
	pthread_t isp_exit_tid;
	rt_uint32_t mq_full_cnt;
	struct pinctrl *pctrl;
	struct video_dev *sensor;
	struct viss_source_info sensor_info;
	struct viss_isp_exif exif;
	rt_uint32_t awb_stat[20];

	/* pts need adequate system time */
	rt_err_t (*streamon)(struct viss_dev *dev, rt_bool_t isp_output, rt_int32_t ch);
	rt_err_t (*streamoff)(struct viss_dev *dev, rt_int32_t ch);

	char *tnr_mem_y;
	char *tnr_mem_cb;
	char *tnr_mem_cr;
	char *bgm_stat_mem;
	char *ism_stat_mem;
	rt_uint32_t isp_irq_status;
	rt_uint32_t isp_prev_dma_irq_status;
	rt_uint32_t isp_cap_dma_irq_status;
	rt_uint32_t isp_restart_en_flag;
	rt_uint32_t isp_timeout_en_flag;
};

struct isp_adjust {
	rt_uint32_t cmd;
	rt_uint32_t val;
};

enum viss_cmd {
	/* common */
	VISS_CMD_INIT,
	VISS_CMD_EXIT,

	VISS_CMD_GET_CHANNEL_NUM, /* sensor use channel number */
	VISS_CMD_GET_ALL_MODE,  /* get all suported modes  */
	VISS_CMD_SET_MODE,      /* select mode, para=mode index */
	VISS_CMD_GET_CUR_MODE,
	VISS_CMD_SET_FRAME_QUEUE,
	VISS_CMD_CAPTURE_ON,   /* start capture, para=channel id */
	VISS_CMD_CAPTURE_OFF,  /* stop capture, para=channel id */
	VISS_CMD_REQUEST_FRAME, /* request one video frame, para=struct viss_reqframe*/
	VISS_CMD_RELEASE_FRAME, /* release one video frame, para=struct viss_reqframe*/
	VISS_CMD_SET_PTS_CB,
	VISS_CMD_GET_WAIT_FRAME_SEM,

	VISS_CMD_SET_BRIGHT,    /* set video bright */
	VISS_CMD_GET_BRIGHT,    /* get video bright */
	VISS_CMD_SET_SATURATION, /* set video staturation */
	VISS_CMD_GET_SATURATION, /* get video staturation */
	VISS_CMD_SET_CONTRAST,   /* set video contrast */
	VISS_CMD_GET_CONTRAST,   /* get video staturation */

	/* debug */
	VISS_CMD_DEBUG_GET_REG,
	VISS_CMD_DEBUG_SET_REG,

	/* isp */
	VISS_CMD_ISP_SET_SENSOR_REG,	/* set sensor register */
	VISS_CMD_ISP_GET_SENSOR_REG, /* get sensor register */
	VISS_CMD_ISP_SET_SEQREG,	/* set isp register sequence*/
	VISS_CMD_ISP_GET_SEQREG,	/* get isp register sequence*/
	VISS_CMD_ISP_SET_REG,	/* set isp register*/
	VISS_CMD_ISP_GET_REG,	/* get isp register*/
	VISS_CMD_ISP_GET_STAT,	/* get isp statistics*/
	VISS_CMD_ISP_EN,
	VISS_CMD_ISP_RESET,
	VISS_CMD_ISP_RESTART,
	VISS_CMD_ISP_FINISH,
	ISP_CMD_EXP_CTRL,
	ISP_CMD_GET_SENSOR_INFO,
	ISP_CMD_GET_WAIT_DRV_SEM,
	ISP_CMD_UPDATE_CONFIG,
	ISP_CMD_ADJUST_PARAM,
	/* common */
	VISS_CMD_SET_SENSOR_STANDBY,
	VISS_CMD_CLEANUP_FRAME_QUEUE,
	VISS_CMD_GET_FRAME_BUF_TYPE,
};

enum isp_cmd {
	ISP_GET_SENSOR_INFO,
	ISP_SET_EXP_GAIN,
};

#define ISP_ADJUST_CMD_MANUAL_CONTRAST		(0)
#define ISP_ADJUST_CMD_MANUAL_SATURATION	(1)
#define ISP_ADJUST_CMD_MANUAL_SHARPNESS		(2)
#define ISP_ADJUST_CMD_MANUAL_EXPOSURE		(3)
#define ISP_ADJUST_CMD_MANUAL_BRIGHTNESS	(4)
#define ISP_ADJUST_CMD_MANUAL_HUE		(5)
#define ISP_ADJUST_CMD_WIN_POS_X		(6)
#define ISP_ADJUST_CMD_WIN_POS_Y		(7)
#define ISP_ADJUST_CMD_WIN_WIDTH		(8)
#define ISP_ADJUST_CMD_WIN_HEIGHT		(9)
#define ISP_ADJUST_CMD_EXP_METERING		(10)
#define ISP_ADJUST_CMD_EXP_BIAS			(11)
#define ISP_ADJUST_CMD_AWB_MODE			(12)
#define ISP_ADJUST_CMD_WDR_MODE			(13)
#define ISP_ADJUST_CMD_FLICKER_MODE		(14)

enum isp_event_type {
	ISP_EVENT_OPEN			= (1 << 0),
	ISP_EVENT_CLOSE			= (1 << 1),
	ISP_EVENT_ISR			= (1 << 2),
	ISP_EVENT_STREAMON		= (1 << 3),
	ISP_EVENT_STREAMOFF		= (1 << 4),
	ISP_EVENT_S_CTRL		= (1 << 5),
	ISP_EVENT_S_FMT			= (1 << 6),
	ISP_EVENT_ADJUST		= (1 << 7),

	ISP_EVENT_UPDATE_ISP_CFG	= (1 << 16),
};

struct isp_event_data {
	/*word 0*/
	rt_uint32_t main_path_width;
	/*word 1*/
	rt_uint32_t main_path_height;
	/*word 2*/
	rt_uint32_t sub_path_width;
	/*word 3*/
	rt_uint32_t sub_path_height;
	/*word 4*/
	rt_uint32_t isp_adjust_cmd;
	/*word 5*/
	rt_uint32_t isp_adjust_val;
	/*word 6*/
	rt_uint32_t nop6;
	/*word 7*/
	rt_uint32_t nop7;
	/*word 8*/
	rt_uint32_t nop8;
	/*word 9*/
	rt_uint32_t nop9;
	/*word 10*/
	rt_uint32_t nop10;
	/*word 11*/
	rt_uint32_t nop11;
	/*word 12*/
	rt_uint32_t nop12;
	/*word 13*/
	rt_uint32_t nop13;
	/*word 14*/
	rt_uint32_t nop14;
	/*word 15*/
	rt_uint32_t nop15;
};

#define ISP_SET_CMD_FLAG		(1 << 31)
#define ISP_SET_CMD_EXPOSURE_TIME	(ISP_SET_CMD_FLAG | 0x0001)
#define ISP_SET_CMD_SHUTTER_SPEED	(ISP_SET_CMD_FLAG | 0x0002)
#define ISP_SET_CMD_APERTURE		(ISP_SET_CMD_FLAG | 0x0003)
#define ISP_SET_CMD_BRIGHTNESS		(ISP_SET_CMD_FLAG | 0x0004)
#define ISP_SET_CMD_EXPOSURE_BIAS	(ISP_SET_CMD_FLAG | 0x0005)
#define ISP_SET_CMD_ISO_SPEED		(ISP_SET_CMD_FLAG | 0x0006)
#define ISP_SET_CMD_FLASH		(ISP_SET_CMD_FLAG | 0x0007)
#define ISP_SET_CMD_ILLU_ID		(ISP_SET_CMD_FLAG | 0x0008)
#define ISP_SET_CMD_BACK_SCORE		(ISP_SET_CMD_FLAG | 0x0009)
#define ISP_SET_CMD_OBJ_LUM		(ISP_SET_CMD_FLAG | 0x000A)
#define ISP_SET_CMD_BACK_LUM		(ISP_SET_CMD_FLAG | 0x000B)
#define ISP_SET_CMD_AVG_LUM		(ISP_SET_CMD_FLAG | 0x000C)
#define ISP_SET_CMD_ORG_TARGET		(ISP_SET_CMD_FLAG | 0x000D)
#define ISP_SET_CMD_FINAL_TARGET	(ISP_SET_CMD_FLAG | 0x000E)
#define ISP_SET_CMD_AWB_STAT		(ISP_SET_CMD_FLAG | 0x000F)

struct isp_usb_info {
	char sensor_name[32];
	int width;
	int height;
	int json_len;
	int res[4096];
};

struct isp_usb_cmd {
	u8 cmd;			/* command 0xf6 */
	u8 flag;		/* modules */
	u32 addr;		/* address */
	u32 len;		/* len */
	u8 res[6];
} __packed;

#define ISP_USB_CMD_GET_IMAGE		(0x01)
#define ISP_USB_CMD_GET_JSON		(0x02)
#define ISP_USB_CMD_GET_SENSOR_INFO	(0x03)
#define ISP_USB_CMD_GET_SENSOR_REG	(0x04)

#define ISP_USB_CMD_SET_SENSOR_REG	(0x40)
#define ISP_USB_CMD_SET_JSON		(0x80)
#define ISP_USB_CMD_SET_RELOAD		(0x81)
#define ISP_USB_CMD_SET_JSON_RELOAD	(0x82)

#define ISP_BPAT_RGRGGBGB	0x00000001
#define ISP_BPAT_GRGRBGBG	0x00000002
#define ISP_BPAT_GBGBRGRG	0x00000004
#define ISP_BPAT_BGBGGRGR	0x00000008

#define TVD_AUTO_DETECT_MODE	(0xffff)

#endif /* ___VISS__H___ */
