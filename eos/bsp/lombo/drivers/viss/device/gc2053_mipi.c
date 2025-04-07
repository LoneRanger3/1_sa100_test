/*
 * isp_dev_gc2053.c - gc2053 driver code for LomboTech
 *
 * Copyright (C) 2016-2018, LomboTech Co.Ltd.
 * Author: lomboswer <lomboswer@lombotech.com>
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

#define DBG_SECTION_NAME	"GC2053-MIPI"
#define DBG_LEVEL		DBG_INFO

#include <debug.h>
#include "viss.h"
#include "mcsi_dev.h"
#include "viss_i2c.h"
#include "viss_cam_power.h"
#include <div.h>

struct sensor_reg {
	rt_uint8_t reg_add;
	rt_uint8_t reg_value;
};
static u32 cur_expline = -1;
struct video_dev *gc2053_global;
u32 SENSOR_MAX_GAIN = 15.5 * 31;

static u32 cur_again = -1;
static u32 cur_dgain = -1;
#define GC2053_MIRROR
#define DRV_MCSI_GC2053_NAME "gc2053-mipi"
#define DRV_MCSI_GC2053_PWR_NAME "cam-pwr-en"
#define GC2053_MIPI_REG_DELAY_FLAG  (0xff)
#define MAX_AGAIN (15 * 1024 + 512)
#define MAX_DGAIN (12 * 1024)

/* static struct viss_i2c_client *gc2053_mipi_i2c_client = RT_NULL; */

/*
 * 2lane YUV init
 */
 #ifdef GC2053_MIRROR
static const struct sensor_reg gc2053_reg_list[] = {
/* window_size=1928*1080,mipi 2lane, */
/* mclk=24mhz,mipi_clock=300mhz,frame_rate=30fps,row_time=29.33us pclk =74.25M */
/* pixel_line_total=2200,line_frame_total=1125 */
/*system*/
{0xfe, 0x80},
{0xfe, 0x80},
{0xfe, 0x80},
{0xfe, 0x00},
{0xf2, 0x00}, /* [1]I2C_open_ena [0]pwd_dn */
{0xf3, 0x00}, /* 0f 00[3]Sdata_pad_io [2:0]Ssync_pad_io */
{0xf4, 0x36}, /* [6:4]pll_ldo_set */
{0xf5, 0xc0}, /* [7]soc_mclk_enable [6]pll_ldo_en [5:4]cp_clk_sel [3:0]cp_clk_div */
{0xf6, 0x44}, /* 7:3]wpllclk_div [2:0]refmp_div */
{0xf7, 0x01}, /* [7]refdiv2d5_en [6]refdiv1d5_en[5:4]sca_mode[3]*/
{0xf8, 0x63}, /* refmp_enb[1]div2en[0]pllmp_en */
{0xf9, 0x40},
{0xfc, 0x8e},
{0xfe, 0x00},
{0x87, 0x18}, /* [6]aec_delay_mode */
{0xee, 0x30}, /* [5:4]dwen_sramen */
{0xd0, 0xb7}, /* ramp_en */
{0x03, 0x04},
{0x04, 0x10},
{0x05, 0x04}, /* 05 */
{0x06, 0x4c}, /* 28 [11:0]hb */
{0x07, 0x00},
{0x08, 0x11},
{0x09, 0x00},
{0x0a, 0x02}, /*cisctl row start */
{0x0b, 0x00},
{0x0c, 0x02}, /* cisctl col start */
{0x0d, 0x04},
{0x0e, 0x40},
{0x12, 0xe2}, /* vsync_ahead_mode */
{0x13, 0x16},
{0x19, 0x0a}, /* ad_pipe_num */
{0x21, 0x1c}, /* eqc1fc_eqc2fc_sw */
{0x28, 0x0a}, /* 16 eqc2_c2clpen_sw */
{0x29, 0x24}, /* eq_post_width */
{0x2b, 0x04}, /* c2clpen --eqc2 */
{0x32, 0xd8}, /* 0x32[7]=1,0xd3[7]=1 rsth=vref */
{0x37, 0x03}, /* [3:2]eqc2sel=0 */
{0x39, 0x17}, /* [3:0]rsgl */
{0x43, 0x07},
{0x44, 0x40},
{0x46, 0x0b},
{0x4b, 0x20}, /* rst_tx_width */
{0x4e, 0x08}, /* 12 ramp_t1_width */
{0x55, 0x20}, /* read_tx_width_pp */
{0x66, 0x05}, /* 18 stspd_width_r1 */
{0x67, 0x05}, /* 40 5 stspd_width_r */
{0x77, 0x01},
{0x78, 0x00},
{0x7c, 0x93}, /* [1:0] co1comp */
{0x8c, 0x12}, /* 12 ramp_t1_ref */
{0x8d, 0x92},
{0x90, 0x00},
{0x41, 0x04},
{0x42, 0x65},
{0x9d, 0x10},
{0xce, 0x7c},
{0xd2, 0x41},
{0xd3, 0xdc},
{0xe6, 0x50},

{0xb6, 0xc0},
{0xb0, 0x70},
{0xb1, 0x01},
{0xb2, 0x00},
{0xb3, 0x00},
{0xb4, 0x00},
{0xb8, 0x01},
{0xb9, 0x00},

{0x26, 0x30},
{0xfe, 0x01},
{0x40, 0x23},
{0x55, 0x07},
{0x60, 0x00}, /* 40 */
{0xfe, 0x04},
{0x14, 0x78},
{0x15, 0x78},
{0x16, 0x78},
{0x17, 0x78},
/*window*/
{0xfe, 0x01},
{0x94, 0x01},
{0x95, 0x04},
{0x96, 0x38}, /* [10:0]out_height */
{0x97, 0x07},
{0x98, 0x80}, /* [11:0]out_width */
{0x99, 0x00},
/*ISP*/
{0xfe, 0x01},
{0x01, 0x05},
{0x02, 0x89},
{0x04, 0x01},
{0x07, 0xa6},
{0x08, 0xa9},
{0x09, 0xa8},
{0x0a, 0xa7},
{0x0b, 0xff},
{0x0c, 0xff},
{0x0f, 0x00},
{0x50, 0x1c},
{0x89, 0x03},
{0xfe, 0x04},
{0x28, 0x86},
{0x29, 0x86},
{0x2a, 0x86},
{0x2b, 0x68},
{0x2c, 0x68},
{0x2d, 0x68},
{0x2e, 0x68},
{0x2f, 0x68},
{0x30, 0x4f},
{0x31, 0x68},
{0x32, 0x67},
{0x33, 0x66},
{0x34, 0x66},
{0x35, 0x66},
{0x36, 0x66},
{0x37, 0x66},
{0x38, 0x62},
{0x39, 0x62},
{0x3a, 0x62},
{0x3b, 0x62},
{0x3c, 0x62},
{0x3d, 0x62},
{0x3e, 0x62},
{0x3f, 0x62},
/*DVP & MIPI*/
{0xfe, 0x01},
{0x9a, 0x06}, /* [5]OUT_gate_mode [4]hsync_delay_half_pclk [3]data_delay_half_pclk */
{0xfe, 0x00}, /* [2]vsync_polarity [1]hsync_polarity [0]pclk_out_polarity */
{0x7b, 0x2a}, /* [7:6]updn [5:4]drv_high_data [3:2]drv_low_data [1:0]drv_pclk */
{0x23, 0x2d}, /* [3]rst_rc [2:1]drv_sync [0]pwd_rc */
{0xfe, 0x03},
{0x01, 0x27}, /* 20 27[6:5]clkctr [2]phy-lane1_en [1]phy-lane0_en [0]phy_clk_en */
{0x02, 0x56}, /* [7:6]data1ctr [5:4]data0ctr [3:0]mipi_diff */
{0x03, 0x8e}, /* b2 b6[7]clklane_p2s_sel [6:5]data0hs_ph [4]data0_delay1s */
{0x12, 0x80}, /* [3]clkdelay1s [2]mipi_en [1:0]clkhs_ph */
{0x13, 0x07}, /* LWC */
{0x15, 0x12}, /* [1:0]clk_lane_mode */
{0xfe, 0x00},
{0x3e, 0x91}, /* 40 91[7]lane_ena [6]DVPBUF_ena [5]ULPEna [4]MIPI_ena [3] */
#if 1
{0x17, 0x83},
{0xfe, 0x01},
{0x92, 0x01},
{0x94, 0x04},
{0xfe, 0x00},
#else    /* normal */
{0x17, 0x80},
{0xfe, 0x01},
{0x92, 0x00},
{0x94, 0x01},
{0xfe, 0x00},
#endif
};
#else
static const struct sensor_reg gc2053_reg_list[] = {
	/* window_size=1928*1080,mipi 2lane, */
	/* mclk=24mhz, mipi_clock=300mhz, frame_rate=30fps,
		row_time=29.33us pclk =74.25M */
	/* pixel_line_total=2200,line_frame_total=1125 */
	/* system */
	{0xfe, 0x80},
	{0xfe, 0x80},
	{0xfe, 0x80},
	{0xfe, 0x00},
	{0xf2, 0x00}, /* [1]I2C_open_ena [0]pwd_dn */
	{0xf3, 0x00}, /* 0f//00[3]Sdata_pad_io [2:0]Ssync_pad_io */
	{0xf4, 0x36}, /* [6:4]pll_ldo_set */
	 /* [7]soc_mclk_enable [6]pll_ldo_en
	 [5:4]cp_clk_sel [3:0]cp_clk_div */
	{0xf5, 0xc0},
	{0xf6, 0x44}, /* [7:3]wpllclk_div [2:0]refmp_div */
	/* [7]refdiv2d5_en [6]refdiv1d5_en [5:4]scaler_mode
	[3]refmp_enb [1]div2en [0]pllmp_en */
	{0xf7, 0x01},
	{0xf8, 0x32}, /* 38////38//[7:0]pllmp_div */
	/* 82//[7:3]rpllclk_div [2:1]pllmp_prediv [0]analog_pwc */
	{0xf9, 0x42},
	{0xfc, 0x8e},
	/*cisctl&analog*/
	{0xfe, 0x00},
	{0x87, 0x18}, /* [6]aec_delay_mode */
	{0xee, 0x30}, /* [5:4]dwen_sramen */
	{0xd0, 0xb7}, /* ramp_en */
	{0x03, 0x04},
	{0x04, 0x10},
	{0x05, 0x04}, /* 05 */
	{0x06, 0x4c}, /* 28//[11:0]hb */
	{0x07, 0x00},
	{0x08, 0xfa},
	{0x09, 0x00},
	{0x0a, 0x02}, /* cisctl row start */
	{0x0b, 0x00},
	{0x0c, 0x02}, /* cisctl col start */
	{0x0d, 0x04},
	{0x0e, 0x40},
	{0x0f, 0x07},
	{0x10, 0x88},
	{0x12, 0xe2}, /* vsync_ahead_mode */
	{0x13, 0x16},
	{0x19, 0x0a}, /* ad_pipe_num */
	{0x21, 0x1c}, /* eqc1fc_eqc2fc_sw */
	{0x28, 0x0a}, /* 16//eqc2_c2clpen_sw */
	{0x29, 0x24}, /* eq_post_width */
	{0x2b, 0x04}, /* c2clpen --eqc2 */
	{0x32, 0xf8}, /* [5]txh_en ->avdd28 */
	{0x37, 0x03}, /* [3:2]eqc2sel=0 */
	{0x39, 0x15}, /* 17 //[3:0]rsgl */
	{0x43, 0x07}, /* vclamp */
	{0x44, 0x40}, /* 0e//post_tx_width */
	{0x46, 0x0b}, /* txh¡ª¡ª3.2v */
	{0x4b, 0x20}, /* rst_tx_width */
	{0x4e, 0x08}, /* 12//ramp_t1_width */
	{0x55, 0x20}, /* read_tx_width_pp */
	{0x66, 0x05}, /* 18//stspd_width_r1 */
	{0x67, 0x05}, /* 40//5//stspd_width_r */
	{0x77, 0x01}, /* dacin offset x31 */
	{0x78, 0x00}, /* dacin offset */
	{0x7c, 0x93}, /* [1:0] co1comp */
	{0x8c, 0x12}, /* 12 ramp_t1_ref */
	{0x8d, 0x92},
	{0x90, 0x00},
	{0x41, 0x04},
	{0x42, 0x65},
	{0x9d, 0x10},
	{0xce, 0x7c}, /* 70//78//[4:2]c1isel */
	{0xd2, 0x41}, /* [5:3]c2clamp */
	{0xd3, 0xdc}, /* 0x39[7]=0,0xd3[3]=1 rsgh=vref */
	{0xe6, 0x40}, /* ramps offset */
	/*gain*/
	{0xb6, 0xc0},
	{0xb0, 0x70},
	{0xb1, 0x01},
	{0xb2, 0x00},
	{0xb3, 0x00},
	{0xb4, 0x00},
	{0xb8, 0x01},
	{0xb9, 0x00},
	/*blk*/
	{0x26, 0x30},
	{0xfe, 0x01},
	{0x40, 0x23},
	{0x55, 0x07},
	{0x60, 0x00}, /* [7:0]WB_offset */
	{0xfe, 0x04},
	{0x14, 0x78},
	{0x15, 0x78},
	{0x16, 0x78},
	{0x17, 0x78},
	/*window*/
	{0xfe, 0x01},
	{0x94, 0x01},
	{0x95, 0x04},
	{0x96, 0x38}, /* [10:0]out_height */
	{0x97, 0x07},
	{0x98, 0x80}, /* [11:0]out_width */
	/*ISP*/
	{0xfe, 0x01},
	{0x01, 0x05},
	{0x02, 0x89},
	{0x04, 0x01},
	{0x07, 0xa6},
	{0x08, 0xa9},
	{0x09, 0xa8},
	{0x0a, 0xa7},
	{0x0b, 0xff},
	{0x0c, 0xff},
	{0x0f, 0x00},
	{0x50, 0x1c},
	{0x89, 0x03},
	{0xfe, 0x04},
	{0x28, 0x86},
	{0x29, 0x86},
	{0x2a, 0x86},
	{0x2b, 0x68},
	{0x2c, 0x68},
	{0x2d, 0x68},
	{0x2e, 0x68},
	{0x2f, 0x68},
	{0x30, 0x4f},
	{0x31, 0x68},
	{0x32, 0x67},
	{0x33, 0x66},
	{0x34, 0x66},
	{0x35, 0x66},
	{0x36, 0x66},
	{0x37, 0x66},
	{0x38, 0x62},
	{0x39, 0x62},
	{0x3a, 0x62},
	{0x3b, 0x62},
	{0x3c, 0x62},
	{0x3d, 0x62},
	{0x3e, 0x62},
	{0x3f, 0x62},

	/*DVP & MIPI*/
	{0xfe, 0x01},
	/*
	[5]OUT_gate_mode [4]hsync_delay_half_pclk [3]data_delay_half_pclk
	[2]vsync_polarity [1]hsync_polarity [0]pclk_out_polarity
	*/
	{0x9a, 0x06},
	{0xfe, 0x00},
	 /* [7:6]updn [5:4]drv_high_data [3:2]drv_low_data [1:0]drv_pclk */
	{0x7b, 0x2a},
	{0x23, 0x2d}, /* [3]rst_rc [2:1]drv_sync [0]pwd_rc */
	{0xfe, 0x03},
	 /* 20//27[6:5]clkctr [2]phy-lane1_en [1]phy-lane0_en [0]phy_clk_en */
	{0x01, 0x27},
	/* [7:6]data1ctr [5:4]data0ctr [3:0]mipi_diff */
	{0x02, 0x56},
	 /* b2//b6[7]clklane_p2s_sel [6:5]data0hs_ph
	 [4]data0_delay1s [3]clkdelay1s [2]mipi_en [1:0]clkhs_ph */
	{0x03, 0x8e},
	{0x12, 0x80},
	{0x13, 0x07}, /* LWC */
	{0x15, 0x12}, /* [1:0]clk_lane_mode */
	{0xfe, 0x00},
	/* 40//91[7]lane_ena [6]DVPBUF_ena [5]ULPEna [4]MIPI_ena [3] */
	/* {0x3e, 0x91}, */
};
#endif
struct dev_mode *cur_gc2053_mipi_mode = RT_NULL;
static struct dev_mode gc2053_mipi_mode[] = {
	{
		.index = 0,
		.inp_fmt = VISS_MBUS_FMT_YUYV8_2X8,
		.out_fmt = VISS_PIX_FMT_NV12,
		.colorspace = DEV_COLORSPACE_JPEG,
		.frame_rate = 30000,   /* 30fps */
		.frame_size.width = 1920,
		.frame_size.height = 1080,
		.usr_data = (void *)gc2053_reg_list,
		.usr_data_size = ARRAY_SIZE(gc2053_reg_list),
	},
	{
		.index = 1,
		.inp_fmt = VISS_MBUS_FMT_YUYV8_2X8,
		.out_fmt = VISS_PIX_FMT_NV12,
		.colorspace = DEV_COLORSPACE_JPEG,
		.frame_rate = 30000,   /* 30fps */
		.frame_size.width = 1280,
		.frame_size.height = 720,
		.usr_data = (void *)gc2053_reg_list,
		.usr_data_size = ARRAY_SIZE(gc2053_reg_list),
	},
};

static rt_err_t __gc2053_mipi_wake_up(void *hdl)
{
	/* todo */
	struct video_dev *gc2053 = (struct video_dev *)hdl;
	return 0;
}

static rt_err_t __gc2053_mipi_block_write(void *hdl,
						void *data, rt_int32_t size)
{
	struct video_dev *gc2053 = (struct video_dev *)hdl;
	struct sensor_reg *reg = (struct sensor_reg *)data;
	rt_int32_t i = 0;
	rt_err_t ret = 0;
	rt_int32_t delay = 0, temp = 0;

	for (i = 0; i < size; i++) {
		if (GC2053_MIPI_REG_DELAY_FLAG == reg[i].reg_add) {
			temp = reg[i].reg_value + 9;
			do_div(temp, 10);
			delay = temp * 10;
			LOG_D("delay: %d", delay);
			do_div(delay, 10);
			rt_thread_delay(delay);
			continue;
		}
		ret = viss_i2c_write_reg_8bit(gc2053->i2c_client,
					reg[i].reg_add, reg[i].reg_value);
		if (RT_EOK != ret) {
			LOG_E("i2c write %x value:%x fail.\n",
				reg[i].reg_add, reg[i].reg_value);
			return ret;
		}
	}

	return RT_EOK;
}

static struct dev_mode *__gc2053_mipi_cur_mode(void *hdl)
{
	struct video_dev *gc2053 = (struct video_dev *)hdl;
	return gc2053->cur_mode;
}

static struct dev_mode *__gc2053_mipi_get_all_mode(void *hdl, rt_int32_t *num)
{
	RT_ASSERT(RT_NULL != num);
	*num = (rt_int32_t)ARRAY_SIZE(gc2053_mipi_mode);

	return gc2053_mipi_mode;
}
int reg_writed;
static rt_err_t __gc2053_mipi_set_mode(void *hdl, rt_int32_t index)
{
	struct video_dev *gc2053 = (struct video_dev *)hdl;
	rt_int32_t num = 0;
	rt_err_t ret = 0;

	num = (rt_int32_t)ARRAY_SIZE(gc2053_mipi_mode);
	if (index >= num) {
		LOG_E("Input mode index error. num:%d index:%d\n", num, index);
		return -RT_EINVAL;
	}
	if (index >= ARRAY_SIZE(gc2053_mipi_mode)) {
		LOG_E("Input para error. index:%d\n", index);
		return -RT_EINVAL;
	}
	gc2053->cur_mode = &gc2053_mipi_mode[index];
	if (reg_writed)
		return RT_EOK;

	reg_writed = 1;
	ret = __gc2053_mipi_block_write(hdl, gc2053->cur_mode->usr_data,
				gc2053->cur_mode->usr_data_size);
	if (RT_EOK != ret)
		return ret;

	return __gc2053_mipi_wake_up(hdl);
}

u32 regValTable[29][4] = {
	{0x00, 0x00, 0x01, 0x00},
	{0x00, 0x10, 0x01, 0x0c},
	{0x00, 0x20, 0x01, 0x1b},
	{0x00, 0x30, 0x01, 0x2c},
	{0x00, 0x40, 0x01, 0x3f},
	{0x00, 0x50, 0x02, 0x16},
	{0x00, 0x60, 0x02, 0x35},
	{0x00, 0x70, 0x03, 0x16},
	{0x00, 0x80, 0x04, 0x02},
	{0x00, 0x90, 0x04, 0x31},
	{0x00, 0xa0, 0x05, 0x32},
	{0x00, 0xb0, 0x06, 0x35},
	{0x00, 0xc0, 0x08, 0x04},
	{0x00, 0x5a, 0x09, 0x19},
	{0x00, 0x83, 0x0b, 0x0f},
	{0x00, 0x93, 0x0d, 0x12},
	{0x00, 0x84, 0x10, 0x00},
	{0x00, 0x94, 0x12, 0x3a},
	{0x01, 0x2c, 0x1a, 0x02},
	{0x01, 0x3c, 0x1b, 0x20},
	{0x00, 0x8c, 0x20, 0x0f},
	{0x00, 0x9c, 0x26, 0x07},
	{0x02, 0x64, 0x36, 0x21},
	{0x02, 0x74, 0x37, 0x3a},
	{0x00, 0xc6, 0x3d, 0x02},
	{0x00, 0xdc, 0x3f, 0x3f},
	{0x02, 0x85, 0x3f, 0x3f},
	{0x02, 0x95, 0x3f, 0x3f},
	{0x00, 0xce, 0x3f, 0x3f},
};

u32 Analog_Multiple[29] = {
	1024,
	1184,
	1424,
	1632,
	2032,
	2352,
	2832,
	3248,
	4160,
	4800,
	5776,
	6640,
	8064,
	9296,
	11552,
	13312,
	16432,
	18912,
	22528,
	25936,
	31840,
	36656,
	45600,
	52512,
	64768,
	82880,
	88000,
	107904,
	113168,
};

static void gc2053_calc_gain(u32 gain, u32 *_again, u32 *_dgain)
{
	int i;
	u32 Decimal;
	u32 reg0, reg1, reg2, Analog_Index;
	u32 dgain, again, dcggain = 0;

	if (gain < 1024)
		gain = 1024;
	if (gain > 127 * 1024)
		gain = 127*1024;
	LOG_D("gain = %d ", gain);

	Analog_Index = 0;
	while (Analog_Index < 29) {
		if (gain < Analog_Multiple[Analog_Index])
			break;
		else
			Analog_Index++;
	}
	dgain = gain * 1000 / Analog_Multiple[Analog_Index - 1];
	Decimal = (dgain * 64) / 1000;

	*_dgain = (Decimal << 2) & 0xfffc;
	*_again = Analog_Index - 1;
}


static void gc2053_set_shutter(void *hdl, u32 texp)
{
	u8 wval, rval;
	struct video_dev *gc2053 = (struct video_dev *)hdl;

	if (cur_expline == texp)
		return;
	cur_expline  = texp;

	viss_i2c_write_reg_8bit(gc2053->i2c_client,
		0xfe, 00);
	wval = (u8)(texp >> 8);
	viss_i2c_write_reg_8bit(gc2053->i2c_client,
		0x03, wval);
	wval = (u8)texp;
	viss_i2c_write_reg_8bit(gc2053->i2c_client,
		0x04, wval);
}
static void gc2053_set_again(void *hdl, u32 again)
{
	struct video_dev *gc2053 = (struct video_dev *)hdl;
	if (cur_again == again)
		return;

	cur_again  = again;
	viss_i2c_write_reg_8bit(gc2053->i2c_client,
		0xb4, regValTable[again][0]);
	viss_i2c_write_reg_8bit(gc2053->i2c_client,
		0xb3, regValTable[again][1]);
	viss_i2c_write_reg_8bit(gc2053->i2c_client,
		0xb8, regValTable[again][2]);
	viss_i2c_write_reg_8bit(gc2053->i2c_client,
		0xb9, regValTable[again][3]);
	return;
}

static void gc2053_set_dgain(void *hdl, u32 dgain)
{
	struct video_dev *gc2053 = (struct video_dev *)hdl;
	if (cur_dgain == dgain)
		return;
	cur_dgain  = dgain;

	viss_i2c_write_reg_8bit(gc2053->i2c_client,
		0xb1, (dgain >> 8) & 0xff);
	viss_i2c_write_reg_8bit(gc2053->i2c_client,
		0xb2, (dgain >> 0) & 0xff);
}

rt_err_t __gc2053_exp_ctrl(void *hdl, struct isp_exp_gain *exp_gain)
{
	struct video_dev *gc2053 = (struct video_dev *)hdl;
	rt_err_t ret = 0;
	u32 gain;
	u32 again, dgain;

	if ((RT_NULL == hdl) || (RT_NULL == exp_gain)) {
		LOG_E("set gc2053_exp_ctrl fail.");
		return -RT_ERROR;
	}

	LOG_D("**********exp_ctrl: %d, %d.", exp_gain->gain, exp_gain->exp);

	gain = (exp_gain->gain * 4);
	gc2053_calc_gain(gain, &again, &dgain);
	gc2053_set_shutter(gc2053, exp_gain->exp / 16);
	gc2053_set_again(gc2053, again);
	gc2053_set_dgain(gc2053, dgain);

	return ret;
}
static rt_err_t __gc2053_isp_get_sensor_info(void *hdl,
	struct isp_sensor_info *isp_sensor_info)
{
	isp_sensor_info->sensor_name = "gc2053";
	isp_sensor_info->pclk = 74250 * 1000;
	isp_sensor_info->vts = 1125;
	isp_sensor_info->hts = 2200;
	isp_sensor_info->input_width = 1920;
	isp_sensor_info->input_height = 1080;
	isp_sensor_info->output_widht = 1920;
	isp_sensor_info->output_height = 1080;
	isp_sensor_info->bayer_mode = ISP_BPAT_RGRGGBGB;

	return RT_EOK;
}
static rt_err_t __gc2053_mipi_set_register(void *hdl,
					struct viss_dbg_register *reg)
{
	return RT_EOK;
}

static rt_err_t __gc2053_mipi_get_register(void *hdl,
					struct viss_dbg_register *reg)
{
/*
	rt_int32_t i = 0;
	rt_uint8_t reg_value;

	LOG_D("gc2053_reg_list>>");
	for (i = 0; i < ARRAY_SIZE(gc2053_reg_list); i++) {
		viss_i2c_read_reg_16bit(gc2053_mipi_i2c_client,
				gc2053_reg_list[i].reg_add, &reg_value);
		rt_kprintf("%x: %x\n", gc2053_reg_list[i].reg_add, reg_value);
	}
*/
	return RT_EOK;
}

static rt_err_t __gc2053_mipi_set_power(void *hdl, rt_bool_t on)
{
	struct video_dev *gc2053 = (struct video_dev *)hdl;

	if ((1 != gc2053->pwdn_valid) || (1 != gc2053->rst_valid))
		return RT_EOK;

	if (RT_TRUE == on) {
		pinctrl_gpio_set_value(gc2053->pctrl, gc2053->rst_gpio, 0);
		rt_thread_delay(1); /* 10ms */
		pinctrl_gpio_set_value(gc2053->pctrl, gc2053->pwdn_gpio, 0);
		rt_thread_delay(1); /* 10ms */
		pinctrl_gpio_set_value(gc2053->pctrl, gc2053->pwdn_gpio, 1);
		rt_thread_delay(1); /* 10ms */
		pinctrl_gpio_set_value(gc2053->pctrl, gc2053->rst_gpio, 1);
	} else {
		/* LOG_D("__gc2053_mipi_set_power 222 "); */
		/* TO DO */
	}
	/* LOG_D("__gc2053_mipi_set_power 3333"); */

	return RT_EOK;
}

static rt_err_t __gc2053_mipi_set_stream(void *hdl, rt_bool_t enable)
{
	rt_int32_t ret = 0;

	struct video_dev *gc2053 = (struct video_dev *)hdl;
	if (enable)
		ret = viss_i2c_write_reg_8bit(gc2053->i2c_client, 0x3e, 0x91);
	else {
		ret = viss_i2c_write_reg_8bit(gc2053->i2c_client, 0x3e, 0x00);
		rt_thread_delay(5); /* 50ms */
	}
	return ret;
}

rt_err_t __gc2053_mipi_ioctl(void *hdl, rt_int32_t cmd, void *para)
{
	rt_err_t ret = 0;
	struct isp_exp_gain *gc2053_exp = RT_NULL;
	struct isp_sensor_info *gc2053_isp_sensor_info = RT_NULL;

	switch (cmd) {
	case ISP_GET_SENSOR_INFO:
		gc2053_isp_sensor_info = (struct isp_sensor_info *)para;
		__gc2053_isp_get_sensor_info(hdl, gc2053_isp_sensor_info);
		break;
	case ISP_SET_EXP_GAIN:
		gc2053_exp = (struct isp_exp_gain *)para;
		__gc2053_exp_ctrl(hdl, gc2053_exp);
		/* LOG_FLOAT("ISP_SET_EXP_GAIN: exp: %d, gain: %f.",
			ov2710_exp->exp, ov2710_exp->gain); */
		break;
	default:
		return -RT_ERROR;
	}
	return ret;
}

static rt_err_t __gc2053_mipi_get_info(void *hdl,
			struct viss_source_info *info)
{
	struct video_dev *gc2053 = (struct video_dev *)hdl;
	/* LOG_D("__gc2053_mipi_get_info in"); */

	if ((RT_NULL == hdl) || (RT_NULL == info)) {
		LOG_E("Get gc2053 information fail.");
		return -RT_ERROR;
	}
	rt_memcpy(info, &gc2053->info, sizeof(struct viss_source_info));
	/* LOG_D("__gc2053_mipi_get_info out"); */

	return RT_EOK;
}

static rt_err_t __gc2053_mipi_parser_config(void *hdl)
{
	struct video_dev *gc2053 = (struct video_dev *)hdl;
	struct viss_source_info info;
	rt_uint32_t tmp_data = 0;
	const char *status;
	const char *i2c_bus_name;
	const char *module = DRV_MCSI_GC2053_NAME;
	rt_err_t ret = 0;
	rt_int32_t  use_enabel = 0;

	rt_memset(&info, 0, sizeof(info));
	ret = config_get_string(module, "status", &status);
	if (-1 == ret) {
		LOG_E("Get status fail.");
	} else {
		if (0 == strcmp(status, "okay"))
			use_enabel = 1;
	}
	if (0 == use_enabel) {
		LOG_W("gc2053 mipi disable.");
		return -RT_ERROR;
	}
	ret = config_get_string(module, "i2c-bus", &i2c_bus_name);
	if (-1 == ret) {
		LOG_E("Get i2c_bus fail.");
		return -RT_ERROR;
	}
	strncpy(info.i2c_bus_name, i2c_bus_name, sizeof(info.i2c_bus_name) - 1);
	LOG_D("info.i2c_bus_name:%s\n", info.i2c_bus_name);

	ret = config_get_u32(module, "if-type", &info.if_type);
	if (-1 == ret)
		LOG_E("Get if-type fail.");

	LOG_D("info.if_type:%d", info.if_type);
	ret = config_get_u32(module, "out-path", &tmp_data);
	if (-1 == ret) {
		LOG_E("Get out-path fail.");
	} else {
		if (1 == tmp_data)
			info.out_path = VISS_IO_ISP;
		else
			info.out_path = VISS_IO_DMA;
	}
	ret = config_get_u32(module, "data-lanes", &tmp_data);

	if (-1 == ret) {
		LOG_E("Get data-lanes fail.");
		return -RT_ERROR;
	} else {
		if (1 <= tmp_data)
			info.data_lanes = tmp_data;
		else {
			LOG_E("data-lanes config error.");
			return -RT_ERROR;
		}
	}
	ret = config_get_u32(module, "if-mode", &info.if_mode);
	if (-1 == ret)
		info.if_mode = -1;
	ret = config_get_u32(module, "mipi-csi-freq", &info.mipi_csi_clock);
	if (-1 == ret) {
		LOG_E("Get mipi-csi-freq fail.");
		return -RT_ERROR;
	}

	ret = config_get_u32(module, "viss_top_freq", &info.viss_top_freq);
	if (-1 == ret) {
		LOG_E("Get viss_top_freq fail.");
		return -RT_ERROR;
	}

	if (info.out_path == VISS_IO_ISP) {
		ret = config_get_u32(module, "isp_top_freq", &info.isp_top_freq);
		if (-1 == ret) {
			LOG_E("Get isp_top_freq fail.");
			return -RT_ERROR;
		}
	}

	info.channel_num = 1;
	ret = config_get_u32(module, "reg", &info.i2c_addr);
	if (-1 == ret)
		LOG_E("Get i2c_addr fail.");
	ret = config_get_u32(module, "mclk-freq", &info.mclk_freq);
	if (-1 == ret)
		LOG_E("Get mclk_freq fail.");
	rt_memcpy(&gc2053->info, &info, sizeof(struct viss_source_info));
	/* LOG_D("gc2053->info.if_type:%d, lane_num: %d.",
		gc2053->info.if_type, gc2053->info.data_lanes); */

	/* power pin */
	gc2053->pwdn_valid = 1;
	ret = config_get_u32_array(module, "mcsi-pwdn",
				gc2053->pwdn_val, ARRAY_SIZE(gc2053->pwdn_val));
	if (ret != ARRAY_SIZE(gc2053->pwdn_val)) {
		LOG_E("mcsi: power pin config error. ret:%d", ret);
		gc2053->pwdn_valid = 0;
	}
	/* LOG_D("gc2053 config power"); */

	/* reset pin */
	gc2053->rst_valid = 1;
	ret = config_get_u32_array(module, "mcsi-rst",
				gc2053->rst_val, ARRAY_SIZE(gc2053->rst_val));
	if (ret != ARRAY_SIZE(gc2053->rst_val)) {
		LOG_E("mcsi: reset pin config error. ret:%d", ret);
		gc2053->rst_valid = 0;
	}
	/* LOG_D("gc2053 config reset"); */

	/* mclk pin */
	gc2053->mclk_valid = 1;
	ret = config_get_u32_array(module, "mcsi-mclk",
				gc2053->mclk_val, ARRAY_SIZE(gc2053->mclk_val));
	if (ret != ARRAY_SIZE(gc2053->mclk_val)) {
		LOG_E("mcsi: mclk pin config error. ret:%d", ret);
		gc2053->mclk_valid = 0;
	}
	/* #endif */
	/* LOG_D("gc2053 config finish"); */

	return RT_EOK;
}

static rt_err_t __gc2053_mipi_prepare(void *hdl)
{
	return __gc2053_mipi_parser_config(hdl);
}

static rt_err_t __gc2053_mipi_init(void *hdl)
{
	rt_int32_t ret = 0;
	rt_uint8_t tmp[2] = {0};
	rt_uint16_t id = 0;
	struct video_dev *gc2053 = (struct video_dev *)hdl;
	gc2053_global = gc2053;
	char *module = DRV_MCSI_GC2053_PWR_NAME;
#if 1
	RT_ASSERT(RT_NULL != gc2053);
	gc2053->pctrl = pinctrl_get(DRV_MCSI_GC2053_NAME);
	if (RT_NULL == gc2053->pctrl)
		return -RT_ERROR;
	#ifdef ARCH_LOMBO_N7V1_CDR
	/* Init gpio */
	/* cam_power_enable(); */
	camera_ldo_set(vol_1p85v);
	cam_power_set(module);
	#endif
	if (1 == gc2053->pwdn_valid) {
		gc2053->pwdn_gpio = pinctrl_gpio_request(gc2053->pctrl,
					gc2053->pwdn_val[0], gc2053->pwdn_val[1]);
		if (gc2053->pwdn_gpio >= 0) {
			/* LOG_D("%d %d %d %d %d", gc2053->pwdn_val[2],
				gc2053->pwdn_val[3], gc2053->pwdn_val[4],
				gc2053->pwdn_val[5], gc2053->pwdn_val[6]); */
			pinctrl_gpio_set_function(gc2053->pctrl, gc2053->pwdn_gpio,
						gc2053->pwdn_val[2]);
			pinctrl_gpio_set_drv_level(gc2053->pctrl, gc2053->pwdn_gpio,
						gc2053->pwdn_val[3]);
			pinctrl_gpio_set_pud_mode(gc2053->pctrl, gc2053->pwdn_gpio,
						gc2053->pwdn_val[4]);
			pinctrl_gpio_set_pud_res(gc2053->pctrl, gc2053->pwdn_gpio,
						gc2053->pwdn_val[5]);
			pinctrl_gpio_set_value(gc2053->pctrl, gc2053->pwdn_gpio,
						gc2053->pwdn_val[6]);
		} else
			gc2053->pwdn_valid = 0;
	}

	if (1 == gc2053->rst_valid) {
		gc2053->rst_gpio = pinctrl_gpio_request(gc2053->pctrl,
					gc2053->rst_val[0], gc2053->rst_val[1]);
		if (gc2053->rst_gpio >= 0) {
			/* LOG_D("%d %d %d %d %d", gc2053->rst_val[2],
				gc2053->rst_val[3], gc2053->rst_val[4],
				gc2053->rst_val[5], gc2053->rst_val[6]); */
			pinctrl_gpio_set_function(gc2053->pctrl, gc2053->rst_gpio,
						gc2053->rst_val[2]);
			pinctrl_gpio_set_drv_level(gc2053->pctrl, gc2053->rst_gpio,
						gc2053->rst_val[3]);
			pinctrl_gpio_set_pud_mode(gc2053->pctrl, gc2053->rst_gpio,
						gc2053->rst_val[4]);
			pinctrl_gpio_set_pud_res(gc2053->pctrl, gc2053->rst_gpio,
						gc2053->rst_val[5]);
			pinctrl_gpio_set_value(gc2053->pctrl, gc2053->rst_gpio,
						gc2053->rst_val[6]);
		} else
			gc2053->rst_valid = 0;
	}

	if (1 == gc2053->mclk_valid) {
		gc2053->mclk_gpio = pinctrl_gpio_request(gc2053->pctrl,
					gc2053->mclk_val[0], gc2053->mclk_val[1]);
		if (gc2053->mclk_gpio >= 0) {
			pinctrl_gpio_set_function(gc2053->pctrl, gc2053->mclk_gpio,
						gc2053->mclk_val[2]);
			pinctrl_gpio_set_drv_level(gc2053->pctrl, gc2053->mclk_gpio,
						gc2053->mclk_val[3]);
			pinctrl_gpio_set_pud_mode(gc2053->pctrl, gc2053->mclk_gpio,
						gc2053->mclk_val[4]);
			pinctrl_gpio_set_pud_res(gc2053->pctrl, gc2053->mclk_gpio,
						gc2053->mclk_val[5]);
			pinctrl_gpio_set_value(gc2053->pctrl, gc2053->mclk_gpio,
						gc2053->mclk_val[6]);
		} else
			gc2053->mclk_valid = 0;
	}
	__gc2053_mipi_set_power(gc2053, RT_TRUE);
#endif
	rt_thread_delay(1); /* 10ms */

	gc2053->i2c_client = rt_zalloc(sizeof(struct viss_i2c_client));
	if (RT_NULL == gc2053->i2c_client)
		return -RT_ENOMEM;
	gc2053->i2c_client->i2c_bus = rt_i2c_bus_device_find(gc2053->info.i2c_bus_name);
	if (RT_NULL == gc2053->i2c_client->i2c_bus) {
		LOG_E("can't find bus dev \"%s\"", gc2053->info.i2c_bus_name);
		goto exit;
	}

	gc2053->i2c_client->i2c_addr = gc2053->info.i2c_addr;
	ret = viss_i2c_read_reg_8bit(gc2053->i2c_client, 0xf0, &tmp[0]);
	LOG_D("add: %x, ret: %d. 0x3107: %x", gc2053->i2c_client->i2c_addr, ret, tmp[0]);
	ret = viss_i2c_read_reg_8bit(gc2053->i2c_client, 0xf1, &tmp[1]);
	LOG_D("add: %x, ret: %d. 0x3108: %x", gc2053->i2c_client->i2c_addr, ret, tmp[1]);

	id = (tmp[0] << 8) | tmp[1];
	if (id != 0x2053) {
		LOG_E("ID wrong! (0x%x)\n", id);
		goto exit;
	}

	/* __gc2053_mipi_block_write((void *)gc2053,
		(void *)gc2053_reg_list, ARRAY_SIZE(gc2053_reg_list)); */

	return RT_EOK;
exit:
	if (gc2053->pctrl) {
		if (gc2053->rst_valid)
			pinctrl_gpio_free(gc2053->pctrl, gc2053->rst_gpio);
		if (gc2053->pwdn_valid)
			pinctrl_gpio_free(gc2053->pctrl, gc2053->pwdn_gpio);
		if (gc2053->mclk_valid)
			pinctrl_gpio_free(gc2053->pctrl, gc2053->mclk_gpio);
		pinctrl_put(gc2053->pctrl);
		gc2053->pctrl = RT_NULL;
	}
	if (gc2053->i2c_client) {
		rt_free(gc2053->i2c_client);
		gc2053->i2c_client = RT_NULL;
	}
	#ifdef ARCH_LOMBO_N7V1_CDR
	/* cam_power_disable(); */
	cam_power_exit(module);
	camera_ldo_exit();
	#endif
	return -RT_ERROR;
}

static void __gc2053_mipi_exit(void *hdl)
{
	struct video_dev *gc2053 = (struct video_dev *)hdl;
	char *module = DRV_MCSI_GC2053_PWR_NAME;

	__gc2053_mipi_set_power(gc2053, RT_FALSE);
	if (gc2053->pctrl) {
		if (gc2053->rst_valid)
			pinctrl_gpio_free(gc2053->pctrl, gc2053->rst_gpio);
		if (gc2053->pwdn_valid)
			pinctrl_gpio_free(gc2053->pctrl, gc2053->pwdn_gpio);
		if (gc2053->mclk_valid)
			pinctrl_gpio_free(gc2053->pctrl, gc2053->mclk_gpio);
		pinctrl_put(gc2053->pctrl);
		gc2053->pctrl = RT_NULL;
	}
	if (gc2053->i2c_client) {
		rt_free(gc2053->i2c_client);
		gc2053->i2c_client = RT_NULL;
	}
	reg_writed = 0;
	#ifdef ARCH_LOMBO_N7V1_CDR
	cam_power_exit(module);
	camera_ldo_exit();
	/* cam_power_disable(); */
	#endif
}

struct video_dev gc2053_mipi = {
	.name = DRV_MCSI_GC2053_NAME,
	.group_id = GRP_ID_MCSI,
	.prepare = __gc2053_mipi_prepare,
	.init = __gc2053_mipi_init,
	.exit = __gc2053_mipi_exit,
	.s_mode = __gc2053_mipi_set_mode,
	.g_cur_mode = __gc2053_mipi_cur_mode,
	.g_all_mode = __gc2053_mipi_get_all_mode,
	.s_power = __gc2053_mipi_set_power,
	.s_stream = __gc2053_mipi_set_stream,
	.g_info = __gc2053_mipi_get_info,
	.ioctl = __gc2053_mipi_ioctl,
	.s_register = __gc2053_mipi_set_register,
	.g_register = __gc2053_mipi_get_register,
};

