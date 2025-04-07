/*
 * isp_dev_sc2363.c - sc2363 driver code for LomboTech
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

#define DBG_SECTION_NAME	"SC2363-MIPI"
#define DBG_LEVEL		DBG_ERROR

#include <debug.h>
#include "viss.h"
#include "mcsi_dev.h"
#include "viss_i2c.h"
#include "viss_cam_power.h"
#include <div.h>

struct sensor_reg {
	rt_uint16_t reg_add;
	rt_uint16_t reg_value;
};
static u32 cur_expline = -1;

#define DRV_MCSI_SC2363_NAME "sc2363-mipi"
#define DRV_MCSI_SC2363_PWR_NAME "cam-pwr-en"

#define SC2363_MIPI_REG_DELAY_FLAG  (0xffff)
#define MAX_AGAIN (15 * 1024 + 512)
#define MAX_DGAIN (12 * 1024)

/* static struct viss_i2c_client *sc2363_mipi_i2c_client = RT_NULL; */

/*
 * 2lane YUV init
 */
static const struct sensor_reg sc2363_reg_list[] = {
		/* 0x0103,0x01, */	 /* 74.25 sysclk 297M cntclk */
		{0x0100, 0x00},

		/* close mipi */
		/* 0x3018,0x1f, */
		/* 0x3019,0xff, */
		/* 0x301c,0xb4, */

		{0x320c, 0x0a},
		{0x320d, 0x50},/* a80->a50 */
		{0x3e01, 0x23},
		{0x363c, 0x05},/* 04 */
		{0x3635, 0xa8},/* c0 */
		{0x363b, 0x0d},/* 0d */
		{0x3620, 0x08},
		{0x3622, 0x02},
		{0x3635, 0xc0},
		{0x3908, 0x10},
		{0x3624, 0x08},/* count_clk inv  need debug  flash row in one channel */
		{0x5000, 0x06},/* rts column test */
		{0x3e06, 0x00},
		{0x3e08, 0x03},
		{0x3e09, 0x10},
		{0x3333, 0x10},
		{0x3306, 0x7e},

		/* 0x3e08,0x1f, */
		/* 0x3e09,0x1f, */
		/* 0x3e06,0x03, */
		{0x3902, 0x05},
		/* 0x3909,0x01, */ /* auto blc */
		/* 0x390a,0xf5, */ /* auto blc */

		{0x3213, 0x08},

		/* new auto precharge  330e in 3372
		[7:6] 11: close div_rst 00:open div_rst */
		{0x337f, 0x03},
		{0x3368, 0x04},
		{0x3369, 0x00},
		{0x336a, 0x00},
		{0x336b, 0x00},
		{0x3367, 0x08},
		{0x330e, 0x30},
		{0x3366, 0x7c}, /* div_rst gap */
		{0x3633, 0x42},
		{0x330b, 0xe0},
		{0x3637, 0x57},
		/* adjust the gap betwen first and second
		cunt_en pos edage to even times the clk */
		{0x3302, 0x1f},
		/* adjust the gap betwen first and second
		cunt_en pos edage to even times the clk */
		{0x3309, 0xde},
		/* 0x303f,0x81, */ /* pclk sel pll_sclk_dig_div  20171205*/

		/* leage current */

		{0x3907, 0x00},
		{0x3908, 0x61},
		{0x3902, 0x45},
		{0x3905, 0xb8},
		/* 0x3904,0x06, 10.18 */
		{0x3e01, 0x8c},
		{0x3e02, 0x10},
		{0x3e06, 0x00},
		{0x3038, 0x48},
		{0x3637, 0x5d},
		{0x3e06, 0x00},
		/* 0921 */
		{0x3908, 0x11},
		{0x335e, 0x01},/* ana dithering */
		{0x335f, 0x03},
		{0x337c, 0x04},
		{0x337d, 0x06},
		{0x33a0, 0x05},
		{0x3301, 0x04},
		{0x3633, 0x4f},/* prnu */
		{0x3622, 0x06},/* blksun */
		{0x3630, 0x08},
		{0x3631, 0x84},
		{0x3306, 0x30},
		{0x366e, 0x08},/* ofs auto en [3] */
		{0x366f, 0x22},/* ofs+finegain  real ofs in 0x3687[4:0] */
		{0x3637, 0x59},/* FW to 4.6k //9.22 */
		{0x3320, 0x06},/* New ramp offset timing */
		{0x3321, 0x60},

		/* 0x3321,0x06, */
		{0x3326, 0x00},
		{0x331e, 0x11},
		{0x331f, 0xc1},
		{0x3303, 0x20},
		{0x3309, 0xd0},
		{0x330b, 0xbe},
		{0x3306, 0x36},
		{0x3635, 0xc2},/* TxVDD,HVDD */
		{0x363b, 0x0a},
		{0x3038, 0x88},
		/* 9.22 */
		{0x3638, 0x1f},/* ramp_gen by sc  0x30 */
		{0x3636, 0x25},
		{0x3625, 0x02},
		{0x331b, 0x83},
		{0x3333, 0x30},
		/* 10.18 */
		{0x3635, 0xa0},
		{0x363b, 0x0a},
		{0x363c, 0x05},
		{0x3314, 0x13},/* preprecharge */
		/* 20171101 reduce hvdd pump lighting */
		{0x3038, 0xc8},/* high pump clk,low lighting */
		{0x363b, 0x0b},/* high hvdd ,low lighting */
		/* large current,low ligting  0x38 (option) */
		{0x3632, 0x18},
		/* 20171102 reduce hvdd pump lighting */
		{0x3038, 0xff},/* high pump clk,low lighting */
		{0x3639, 0x09},
		{0x3621, 0x28},
		{0x3211, 0x0c},
		/* 20171106 */
		{0x366f, 0x26},
		/* 20171121 */
		{0x366f, 0x2f},
		{0x3320, 0x01},
		{0x3306, 0x48},
		{0x331e, 0x19},
		{0x331f, 0xc9},

		{0x330b, 0xd3},
		{0x3620, 0x28},

		/* 20171122 */
		{0x3309, 0x60},
		{0x331f, 0x59},
		{0x3308, 0x10},
		{0x3630, 0x0c},

		/* digital ctrl */
		{0x3f00, 0x07},/* bit[2] = 1 */
		{0x3f04, 0x05},
		{0x3f05, 0x04},/* hts / 2 - 0x24 */

		{0x3802, 0x01},
		{0x3235, 0x08},
		{0x3236, 0xc8},/* vts x 2 - 2 */

		/* 20171127 */
		{0x3630, 0x1c},

		/* 20171129 */

		{0x320c, 0x08},/* 2080 hts */
		{0x320d, 0x20},

		{0x320e, 0x04},/* 1250 vts */
		{0x320f, 0xe2},

		/* digital ctrl */
		{0x3f04, 0x03},
		{0x3f05, 0xec},/* hts / 2 - 0x24 */

		{0x3235, 0x09},
		{0x3236, 0xc2},/* vts x 2 - 2 */

		{0x3e01, 0x9c},
		{0x3e02, 0x00},

		{0x3039, 0x54},/* vco 390M */
		{0x303a, 0xb3},/* sysclk  78M */
		{0x303b, 0x06},
		{0x303c, 0x0e},
		{0x3034, 0x01},/* cunt clk 312M */
		{0x3035, 0x9b},

		/* mipi */
		{0x3018, 0x33},/* [7:5] lane_num-1 */
		{0x3031, 0x0a},/* [3:0] bitmode */
		{0x3037, 0x20},/* [6:5] bitsel */
		{0x3001, 0xFE},/* [0] c_y */

		/* lane_dis of lane3~8 */
		/* 0x3018,0x12, */
		/* 0x3019,0xfc, */

		{0x4603, 0x00},/* [0] data_fifo mipi mode */
		{0x4837, 0x19},/* [7:0] pclk period * 2 */
		{0x4827, 0x48},/* [7:0] hs_prepare_time[7:0] */
		{0x33aa, 0x10},/* save power */

		/* 20171208  logical   inter */
		{0x3670, 0x04},/* 0x3631 3670[2] enable  0x3631 in 0x3682 */
		{0x3677, 0x84},/* gain<gain0 */
		{0x3678, 0x88},/* gain0=<gain<gain1 */
		{0x3679, 0x88},/* gain>=gain1 */
		{0x367e, 0x08},/* gain0 {3e08[4:2],3e09[3:1]} */
		{0x367f, 0x28},/* gain1 */

		/* 0x3633 3670[3] enable  0x3633 in 0x3683	   20171227 */
		{0x3670, 0x0c},
		{0x3690, 0x34},/* gain<gain0 */
		{0x3691, 0x11},/* gain0=<gain<gain1 */
		{0x3692, 0x42},/* gain>=gain1 */
		{0x369c, 0x08},/* gain0{3e08[4:2],3e09[3:1]} */
		{0x369d, 0x28},/* gain1 */

		{0x360f, 0x01},/* 0x3622 360f[0] enable  0x3622 in 0x3680 */
		{0x3671, 0xc6},/* gain<gain0 */
		{0x3672, 0x06},/* gain0=<gain<gain1 */
		{0x3673, 0x16},/* gain>=gain1 */
		{0x367a, 0x28},/* gain0{3e08[4:2],3e09[3:1]} */
		{0x367b, 0x3f},/* gain1 */

		/* 20171218 */
		{0x3802, 0x00},

		/* 20171225 BLC power save mode */
		{0x3222, 0x29},
		{0x3901, 0x02},
		{0x3905, 0x98},

		/* 20171227 */
		{0x3e1e, 0x34}, /* digital finegain enable */

		/* 20180113 */
		{0x3314, 0x08},

		/* init */
		{0x3301, 0x06},
		{0x3306, 0x48},
		{0x3632, 0x08},
		{0x3e00, 0x00},
		{0x3e01, 0x4d},
		{0x3e02, 0xe0},
		{0x3e03, 0x03},
		{0x3e06, 0x00},
		{0x3e07, 0x80},
		{0x3e08, 0x00},
		{0x3e09, 0x05},
	#ifdef MIRROR_2363
		{0x3221, 0x66},
	#endif

	#if (SC2363_MIPI_OUTPUT_H == 1088)
		{0x3208, 0x07},/* W-H */
		{0x3209, 0x80},/* W-L 1920 */
		{0x320a, 0x04},/* H-H */
		{0x320b, 0x40},/* H-L 1080:0x38 1088:0x40 */
	#endif
	/* {0x0100,0x01}, */
	/* {0x3008, 0x02}, */
};

struct dev_mode *cur_sc2363_mipi_mode = RT_NULL;
static struct dev_mode sc2363_mipi_mode[] = {
	{
		.index = 0,
		.inp_fmt = VISS_MBUS_FMT_YUYV8_2X8,
		.out_fmt = VISS_PIX_FMT_NV12,
		.colorspace = DEV_COLORSPACE_JPEG,
		.frame_rate = 30000,   /* 30fps */
		.frame_size.width = 1920,
		.frame_size.height = 1080,
		.usr_data = (void *)sc2363_reg_list,
		.usr_data_size = ARRAY_SIZE(sc2363_reg_list),
	},
	{
		.index = 1,
		.inp_fmt = VISS_MBUS_FMT_YUYV8_2X8,
		.out_fmt = VISS_PIX_FMT_NV12,
		.colorspace = DEV_COLORSPACE_JPEG,
		.frame_rate = 30000,   /* 30fps */
		.frame_size.width = 1280,
		.frame_size.height = 720,
		.usr_data = (void *)sc2363_reg_list,
		.usr_data_size = ARRAY_SIZE(sc2363_reg_list),
	},
};

static rt_err_t __sc2363_mipi_wake_up(void *hdl)
{
	struct video_dev *sc2363 = (struct video_dev *)hdl;
	return viss_i2c_write_reg_16bit(sc2363->i2c_client, 0x3008, 0x02);
}

static rt_err_t __sc2363_mipi_block_write(void *hdl,
						void *data, rt_int32_t size)
{
	struct video_dev *sc2363 = (struct video_dev *)hdl;
	struct sensor_reg *reg = (struct sensor_reg *)data;
	rt_int32_t i = 0;
	rt_err_t ret = 0;
	rt_int32_t delay = 0, temp = 0;

	for (i = 0; i < size; i++) {
		if (SC2363_MIPI_REG_DELAY_FLAG == reg[i].reg_add) {
			temp = reg[i].reg_value + 9;
			do_div(temp, 10);
			delay = temp * 10;
			LOG_D("delay: %d", delay);
			do_div(delay, 10);
			rt_thread_delay(delay);
			continue;
		}
		ret = viss_i2c_write_reg_16bit(sc2363->i2c_client,
					reg[i].reg_add, reg[i].reg_value);
		if (RT_EOK != ret) {
			LOG_E("i2c write %x value:%x fail.\n",
				reg[i].reg_add, reg[i].reg_value);
			return ret;
		}
	}

	return RT_EOK;
}

static struct dev_mode *__sc2363_mipi_cur_mode(void *hdl)
{
	struct video_dev *sc2363 = (struct video_dev *)hdl;
	return sc2363->cur_mode;
}

static struct dev_mode *__sc2363_mipi_get_all_mode(void *hdl, rt_int32_t *num)
{
	RT_ASSERT(RT_NULL != num);
	*num = (rt_int32_t)ARRAY_SIZE(sc2363_mipi_mode);

	return sc2363_mipi_mode;
}
int reg_writed;
static rt_err_t __sc2363_mipi_set_mode(void *hdl, rt_int32_t index)
{
	struct video_dev *sc2363 = (struct video_dev *)hdl;
	rt_int32_t num = 0;
	rt_err_t ret = 0;

	num = (rt_int32_t)ARRAY_SIZE(sc2363_mipi_mode);
	if (index >= num) {
		LOG_E("Input mode index error. num:%d index:%d\n", num, index);
		return -RT_EINVAL;
	}
	if (index >= ARRAY_SIZE(sc2363_mipi_mode)) {
		LOG_E("Input para error. index:%d\n", index);
		return -RT_EINVAL;
	}
	sc2363->cur_mode = &sc2363_mipi_mode[index];
	if (reg_writed)
		return RT_EOK;

	reg_writed = 1;
	ret = __sc2363_mipi_block_write(hdl, sc2363->cur_mode->usr_data,
				sc2363->cur_mode->usr_data_size);
	if (RT_EOK != ret)
		return ret;

	return __sc2363_mipi_wake_up(hdl);
}

static u32 convertgain2cfg(u32 val)
{
	u32 course = 1;
	u32 finegain;

	while (val >= 2048) {
		course *= 2;
		val /= 2;
	}
	finegain = 0x10 + val / 128;
	return ((course-1) << 8) + finegain;
}

static void calc_gain(u32 gain, u32 *_again, u32 *_dgain)
{
	u32 dgain, again;

	if (gain < 1024)
		gain = 1024;
	if (gain > 127 * 1024)
		gain = 127 * 1024;

	dgain = gain * 1024 / MAX_AGAIN;
	if (dgain < 1024)
		dgain = 1024;
	if (dgain > MAX_DGAIN)
		dgain = MAX_DGAIN;

	again = gain * 1024 / dgain;

	if (again < 1024)
		again = 1024;
	if (again > MAX_AGAIN)
		again = MAX_AGAIN;

	*_again = convertgain2cfg(again);
	*_dgain = convertgain2cfg(dgain);
}

static void set_shutter(struct video_dev *sc2363, u32 texp)
{
	viss_i2c_write_reg_16bit(sc2363->i2c_client, 0x3e00,
		(texp >> 12) & 0xff);
	viss_i2c_write_reg_16bit(sc2363->i2c_client, 0x3e01,
		(texp >> 4) & 0xff);
	viss_i2c_write_reg_16bit(sc2363->i2c_client, 0x3e02,
		(texp << 4) & 0xf0);
}

static void set_again(struct video_dev *sc2363, u32 again)
{
	viss_i2c_write_reg_16bit(sc2363->i2c_client, 0x3e08,
			((again >> 6) & 0x1c) | 0x03);
	viss_i2c_write_reg_16bit(sc2363->i2c_client, 0x3e09, again & 0xff);
}

static void set_dgain(struct video_dev *sc2363, u32 dgain, u32 gain)
{
	viss_i2c_write_reg_16bit(sc2363->i2c_client, 0x3e08,
		(gain >> 14) & 0xff);
	viss_i2c_write_reg_16bit(sc2363->i2c_client, 0x3e09,
		(gain>>6) & 0xff);

	if (cur_expline > 160)
		viss_i2c_write_reg_16bit(sc2363->i2c_client, 0x3314, 0x04);
	if (cur_expline < 80)
		viss_i2c_write_reg_16bit(sc2363->i2c_client, 0x3314, 0x14);

	gain >>= 10;
	if (gain < 2) {
		viss_i2c_write_reg_16bit(sc2363->i2c_client, 0x3301, 0x06);
		viss_i2c_write_reg_16bit(sc2363->i2c_client, 0x3606, 0x48);
		viss_i2c_write_reg_16bit(sc2363->i2c_client, 0x3632, 0x08);
		return;
	}

	if (gain < 4) {
		viss_i2c_write_reg_16bit(sc2363->i2c_client, 0x3301, 0x14);
		viss_i2c_write_reg_16bit(sc2363->i2c_client, 0x3606, 0x48);
		viss_i2c_write_reg_16bit(sc2363->i2c_client, 0x3632, 0x08);
		/* viss_i2c_write_reg_16bit(sc2363->i2c_client, 0x3314,0x02); */
		return;
	}

	if (gain < 8) {
		viss_i2c_write_reg_16bit(sc2363->i2c_client, 0x3301, 0x18);
		viss_i2c_write_reg_16bit(sc2363->i2c_client, 0x3606, 0x48);
		viss_i2c_write_reg_16bit(sc2363->i2c_client, 0x3632, 0x08);
		/* viss_i2c_write_reg_16bit(sc2363->i2c_client, 0x3314,0x02); */
		return;
	}

	if (gain < 15) {
		viss_i2c_write_reg_16bit(sc2363->i2c_client, 0x3301, 0x13);
		viss_i2c_write_reg_16bit(sc2363->i2c_client, 0x3606, 0x48);
		viss_i2c_write_reg_16bit(sc2363->i2c_client, 0x3632, 0x08);
		/* viss_i2c_write_reg_16bit(sc2363->i2c_client, 0x3314,0x02); */
		return;
	}

	if (gain < 31) {
		viss_i2c_write_reg_16bit(sc2363->i2c_client, 0x3301, 0xa1);
		viss_i2c_write_reg_16bit(sc2363->i2c_client, 0x3606, 0x88);
		viss_i2c_write_reg_16bit(sc2363->i2c_client, 0x3632, 0x48);
		/* viss_i2c_write_reg_16bit(sc2363->i2c_client, 0x3314,0x02); */
		return;
	}

	viss_i2c_write_reg_16bit(sc2363->i2c_client, 0x3301, 0xa1);
	viss_i2c_write_reg_16bit(sc2363->i2c_client, 0x3606, 0x78);
	viss_i2c_write_reg_16bit(sc2363->i2c_client, 0x3632, 0x78);
	/* viss_i2c_write_reg_16bit(sc2363->i2c_client, 0x3314,0x02); */
	return;
}

static rt_err_t __sc2363_set_gain(struct video_dev *sensor, rt_uint32_t gain)
{
	u32 again, dgain;

	calc_gain(gain * 4, &again, &dgain);
	set_again(sensor, again);
	set_dgain(sensor, dgain, gain * 4);

	return RT_EOK;
}

static rt_err_t __sc2363_set_shutter(struct video_dev *sensor, rt_uint32_t shutter)
{
	cur_expline  = shutter;
	set_shutter(sensor, shutter);

	return RT_EOK;
}

static rt_err_t __sc2363_exp_ctrl(void *hdl, struct isp_exp_gain *exp_gain)
{
	struct video_dev *sc2363 = (struct video_dev *)hdl;
	rt_err_t ret = 0;

	if ((RT_NULL == hdl) || (RT_NULL == exp_gain)) {
		LOG_E("set sc2363_exp_ctrl fail.");
		return -RT_ERROR;
	}
	/* LOG_D("**********exp_ctrl: %d, %d.", exp_gain->gain, exp_gain->exp); */
	viss_i2c_write_reg_16bit(sc2363->i2c_client, 0x3812, 0x00);

	ret = __sc2363_set_shutter(sc2363, exp_gain->exp / 8);
	if (RT_EOK != ret)
		LOG_E("sc2363 set shutter fail.");

	ret = __sc2363_set_gain(sc2363, exp_gain->gain);
	if (RT_EOK != ret)
		LOG_E("sc2363 set gain fail.");
	viss_i2c_write_reg_16bit(sc2363->i2c_client, 0x3812, 0x30);

	return ret;
}
static rt_err_t __sc2363_isp_get_sensor_info(void *hdl,
	struct isp_sensor_info *isp_sensor_info)
{
	isp_sensor_info->sensor_name = "sc2363";
	isp_sensor_info->pclk = 78 * 1000 * 1000;
	isp_sensor_info->vts = 1250;
	isp_sensor_info->hts = 2080;
	isp_sensor_info->input_width = 1920;
	isp_sensor_info->input_height = 1080;
	isp_sensor_info->output_widht = 1920;
	isp_sensor_info->output_height = 1080;
	isp_sensor_info->bayer_mode = ISP_BPAT_BGBGGRGR;

	return RT_EOK;
}
static rt_err_t __sc2363_mipi_set_register(void *hdl,
					struct viss_dbg_register *reg)
{
	struct video_dev *sc2363 = (struct video_dev *)hdl;
	rt_err_t ret = 0;

	ret = viss_i2c_write_reg_16bit(sc2363->i2c_client, reg->add, reg->val);

	return ret;
}

static rt_err_t __sc2363_mipi_get_register(void *hdl,
					struct viss_dbg_register *reg)
{
	struct video_dev *sc2363 = (struct video_dev *)hdl;
	rt_uint8_t value;
	rt_err_t ret = 0;

	ret = viss_i2c_read_reg_16bit(sc2363->i2c_client, reg->add, &value);
	reg->val = value;

	return ret;
}

static rt_err_t __sc2363_mipi_set_power(void *hdl, rt_bool_t on)
{
	struct video_dev *sc2363 = (struct video_dev *)hdl;
	/* LOG_D("__sc2363_mipi_set_power"); */

	if ((1 != sc2363->pwdn_valid) || (1 != sc2363->rst_valid))
		return RT_EOK;

	if (RT_TRUE == on) {
		/* LOG_D("__sc2363_mipi_set_power 1111"); */
		pinctrl_gpio_set_value(sc2363->pctrl, sc2363->pwdn_gpio, 0);
		rt_thread_delay(1); /* 10ms */
		pinctrl_gpio_set_value(sc2363->pctrl, sc2363->rst_gpio, 0);
		rt_thread_delay(1); /* 10ms */
		pinctrl_gpio_set_value(sc2363->pctrl, sc2363->rst_gpio, 1);
		rt_thread_delay(1); /* 10ms */
		pinctrl_gpio_set_value(sc2363->pctrl, sc2363->pwdn_gpio, 1);
	} else {
		/* LOG_D("__sc2363_mipi_set_power 222 "); */
		/* TO DO */
	}
	/* LOG_D("__sc2363_mipi_set_power 3333"); */

	return RT_EOK;
}

static rt_err_t __sc2363_mipi_set_stream(void *hdl, rt_bool_t enable)
{
#if 1
	rt_int32_t ret = 0;
	struct video_dev *sc2363 = (struct video_dev *)hdl;
	if (enable)
		ret = viss_i2c_write_reg_16bit(sc2363->i2c_client, 0x100, 1);
	else
		ret = viss_i2c_write_reg_16bit(sc2363->i2c_client, 0x100, 0);
#endif
	return ret;
}

rt_err_t __sc2363_mipi_ioctl(void *hdl, rt_int32_t cmd, void *para)
{
	rt_err_t ret = 0;
	struct isp_exp_gain *sc2363_exp = RT_NULL;
	struct isp_sensor_info *sc2363_isp_sensor_info = RT_NULL;

	switch (cmd) {
	case ISP_GET_SENSOR_INFO:
		sc2363_isp_sensor_info = (struct isp_sensor_info *)para;
		__sc2363_isp_get_sensor_info(hdl, sc2363_isp_sensor_info);
		break;
	case ISP_SET_EXP_GAIN:
		sc2363_exp = (struct isp_exp_gain *)para;
		__sc2363_exp_ctrl(hdl, sc2363_exp);
		/* LOG_FLOAT("ISP_SET_EXP_GAIN: exp: %d, gain: %f.",
			ov2710_exp->exp, ov2710_exp->gain); */
		break;
	default:
		return -RT_ERROR;
	}
	return ret;
}

static rt_err_t __sc2363_mipi_get_info(void *hdl,
			struct viss_source_info *info)
{
	struct video_dev *sc2363 = (struct video_dev *)hdl;
	/* LOG_D("__sc2363_mipi_get_info in"); */

	if ((RT_NULL == hdl) || (RT_NULL == info)) {
		LOG_E("Get sc2363 information fail.");
		return -RT_ERROR;
	}
	rt_memcpy(info, &sc2363->info, sizeof(struct viss_source_info));
	/* LOG_D("__sc2363_mipi_get_info out"); */

	return RT_EOK;
}

static rt_err_t __sc2363_mipi_parser_config(void *hdl)
{
	struct video_dev *sc2363 = (struct video_dev *)hdl;
	struct viss_source_info info;
	rt_uint32_t tmp_data = 0;
	const char *status;
	const char *i2c_bus_name;
	const char *module = DRV_MCSI_SC2363_NAME;
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
		LOG_W("sc2363 mipi disable.");
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
	rt_memcpy(&sc2363->info, &info, sizeof(struct viss_source_info));
	/* LOG_D("sc2363->info.if_type:%d, lane_num: %d.",
		sc2363->info.if_type, sc2363->info.data_lanes); */

	/* power pin */
	sc2363->pwdn_valid = 1;
	ret = config_get_u32_array(module, "mcsi-pwdn",
				sc2363->pwdn_val, ARRAY_SIZE(sc2363->pwdn_val));
	if (ret != ARRAY_SIZE(sc2363->pwdn_val)) {
		LOG_E("mcsi: power pin config error. ret:%d", ret);
		sc2363->pwdn_valid = 0;
	}
	/* LOG_D("sc2363 config power"); */

	/* reset pin */
	sc2363->rst_valid = 1;
	ret = config_get_u32_array(module, "mcsi-rst",
				sc2363->rst_val, ARRAY_SIZE(sc2363->rst_val));
	if (ret != ARRAY_SIZE(sc2363->rst_val)) {
		LOG_E("mcsi: reset pin config error. ret:%d", ret);
		sc2363->rst_valid = 0;
	}
	/* LOG_D("sc2363 config reset"); */

	/* mclk pin */
	sc2363->mclk_valid = 1;
	ret = config_get_u32_array(module, "mcsi-mclk",
				sc2363->mclk_val, ARRAY_SIZE(sc2363->mclk_val));
	if (ret != ARRAY_SIZE(sc2363->mclk_val)) {
		LOG_E("mcsi: mclk pin config error. ret:%d", ret);
		sc2363->mclk_valid = 0;
	}
/* #endif */
	/* LOG_D("sc2363 config finish"); */

	return RT_EOK;
}

static rt_err_t __sc2363_mipi_prepare(void *hdl)
{
	return __sc2363_mipi_parser_config(hdl);
}

static rt_err_t __sc2363_mipi_init(void *hdl)
{
	rt_int32_t ret = 0;
	rt_uint8_t tmp[2] = {0};
	rt_uint16_t id = 0;
	struct video_dev *sc2363 = (struct video_dev *)hdl;
	char *module = DRV_MCSI_SC2363_PWR_NAME;
	/* LOG_D("__sc2363_mipi_init in."); */

#if 1
	RT_ASSERT(RT_NULL != sc2363);
	sc2363->pctrl = pinctrl_get(DRV_MCSI_SC2363_NAME);
	if (RT_NULL == sc2363->pctrl)
		return -RT_ERROR;
	#ifdef ARCH_LOMBO_N7V1_CDR
	/* Init gpio */
	/* cam_power_enable(); */
	camera_ldo_set(vol_1p85v);
	cam_power_set(module);
	#endif
	if (1 == sc2363->pwdn_valid) {
		sc2363->pwdn_gpio = pinctrl_gpio_request(sc2363->pctrl,
					sc2363->pwdn_val[0], sc2363->pwdn_val[1]);
		if (sc2363->pwdn_gpio >= 0) {
			/* LOG_D("%d %d %d %d %d", sc2363->pwdn_val[2],
				sc2363->pwdn_val[3], sc2363->pwdn_val[4],
				sc2363->pwdn_val[5], sc2363->pwdn_val[6]); */
			pinctrl_gpio_set_function(sc2363->pctrl, sc2363->pwdn_gpio,
						sc2363->pwdn_val[2]);
			pinctrl_gpio_set_drv_level(sc2363->pctrl, sc2363->pwdn_gpio,
						sc2363->pwdn_val[3]);
			pinctrl_gpio_set_pud_mode(sc2363->pctrl, sc2363->pwdn_gpio,
						sc2363->pwdn_val[4]);
			pinctrl_gpio_set_pud_res(sc2363->pctrl, sc2363->pwdn_gpio,
						sc2363->pwdn_val[5]);
			pinctrl_gpio_set_value(sc2363->pctrl, sc2363->pwdn_gpio,
						sc2363->pwdn_val[6]);
		} else
			sc2363->pwdn_valid = 0;
	}

	if (1 == sc2363->rst_valid) {
		sc2363->rst_gpio = pinctrl_gpio_request(sc2363->pctrl,
					sc2363->rst_val[0], sc2363->rst_val[1]);
		if (sc2363->rst_gpio >= 0) {
			/* LOG_D("%d %d %d %d %d", sc2363->rst_val[2],
				sc2363->rst_val[3], sc2363->rst_val[4],
				sc2363->rst_val[5], sc2363->rst_val[6]); */
			pinctrl_gpio_set_function(sc2363->pctrl, sc2363->rst_gpio,
						sc2363->rst_val[2]);
			pinctrl_gpio_set_drv_level(sc2363->pctrl, sc2363->rst_gpio,
						sc2363->rst_val[3]);
			pinctrl_gpio_set_pud_mode(sc2363->pctrl, sc2363->rst_gpio,
						sc2363->rst_val[4]);
			pinctrl_gpio_set_pud_res(sc2363->pctrl, sc2363->rst_gpio,
						sc2363->rst_val[5]);
			pinctrl_gpio_set_value(sc2363->pctrl, sc2363->rst_gpio,
						sc2363->rst_val[6]);
		} else
			sc2363->rst_valid = 0;
	}

	if (1 == sc2363->mclk_valid) {
		sc2363->mclk_gpio = pinctrl_gpio_request(sc2363->pctrl,
					sc2363->mclk_val[0], sc2363->mclk_val[1]);
		if (sc2363->mclk_gpio >= 0) {
			pinctrl_gpio_set_function(sc2363->pctrl, sc2363->mclk_gpio,
						sc2363->mclk_val[2]);
			pinctrl_gpio_set_drv_level(sc2363->pctrl, sc2363->mclk_gpio,
						sc2363->mclk_val[3]);
			pinctrl_gpio_set_pud_mode(sc2363->pctrl, sc2363->mclk_gpio,
						sc2363->mclk_val[4]);
			pinctrl_gpio_set_pud_res(sc2363->pctrl, sc2363->mclk_gpio,
						sc2363->mclk_val[5]);
			pinctrl_gpio_set_value(sc2363->pctrl, sc2363->mclk_gpio,
						sc2363->mclk_val[6]);
		} else
			sc2363->mclk_valid = 0;
	}
	__sc2363_mipi_set_power(sc2363, RT_TRUE);
#endif

	sc2363->i2c_client = rt_zalloc(sizeof(struct viss_i2c_client));
	if (RT_NULL == sc2363->i2c_client)
		return -RT_ENOMEM;
	sc2363->i2c_client->i2c_bus = rt_i2c_bus_device_find(sc2363->info.i2c_bus_name);
	if (RT_NULL == sc2363->i2c_client->i2c_bus) {
		LOG_E("can't find bus dev \"%s\"", sc2363->info.i2c_bus_name);
		goto exit;
	}

	sc2363->i2c_client->i2c_addr = sc2363->info.i2c_addr;
	ret = viss_i2c_read_reg_16bit(sc2363->i2c_client, 0x3107, &tmp[0]);
	LOG_D("add: %x, ret: %d. 0x3107: %x", sc2363->i2c_client->i2c_addr, ret, tmp[0]);
	ret = viss_i2c_read_reg_16bit(sc2363->i2c_client, 0x3108, &tmp[1]);
	LOG_D("add: %x, ret: %d. 0x3108: %x", sc2363->i2c_client->i2c_addr, ret, tmp[1]);

	id = (tmp[0] << 8) | tmp[1];
	if (id != 0x2232) {
		LOG_E("ID wrong! (0x%x)\n", id);
		goto exit;
	}

	/* __sc2363_mipi_block_write((void *)sc2363,
		(void *)sc2363_reg_list, ARRAY_SIZE(sc2363_reg_list)); */

	return RT_EOK;
exit:
	if (sc2363->pctrl) {
		if (sc2363->rst_valid)
			pinctrl_gpio_free(sc2363->pctrl, sc2363->rst_gpio);
		if (sc2363->pwdn_valid)
			pinctrl_gpio_free(sc2363->pctrl, sc2363->pwdn_gpio);
		if (sc2363->mclk_valid)
			pinctrl_gpio_free(sc2363->pctrl, sc2363->mclk_gpio);
		pinctrl_put(sc2363->pctrl);
		sc2363->pctrl = RT_NULL;
	}
	if (sc2363->i2c_client) {
		rt_free(sc2363->i2c_client);
		sc2363->i2c_client = RT_NULL;
	}
	#ifdef ARCH_LOMBO_N7V1_CDR
	/* cam_power_disable(); */
	cam_power_exit(module);
	camera_ldo_exit();
	#endif
	return -RT_ERROR;
}

static void __sc2363_mipi_exit(void *hdl)
{
	struct video_dev *sc2363 = (struct video_dev *)hdl;
	char *module = DRV_MCSI_SC2363_PWR_NAME;

	__sc2363_mipi_set_power(sc2363, RT_FALSE);
	if (sc2363->pctrl) {
		if (sc2363->rst_valid)
			pinctrl_gpio_free(sc2363->pctrl, sc2363->rst_gpio);
		if (sc2363->pwdn_valid)
			pinctrl_gpio_free(sc2363->pctrl, sc2363->pwdn_gpio);
		if (sc2363->mclk_valid)
			pinctrl_gpio_free(sc2363->pctrl, sc2363->mclk_gpio);
		pinctrl_put(sc2363->pctrl);
		sc2363->pctrl = RT_NULL;
	}
	if (sc2363->i2c_client) {
		rt_free(sc2363->i2c_client);
		sc2363->i2c_client = RT_NULL;
	}
	reg_writed = 0;
	#ifdef ARCH_LOMBO_N7V1_CDR
	cam_power_exit(module);
	camera_ldo_exit();
	/* cam_power_disable(); */
	#endif
}

struct video_dev sc2363_mipi = {
	.name = DRV_MCSI_SC2363_NAME,
	.group_id = GRP_ID_MCSI,
	.prepare = __sc2363_mipi_prepare,
	.init = __sc2363_mipi_init,
	.exit = __sc2363_mipi_exit,
	.s_mode = __sc2363_mipi_set_mode,
	.g_cur_mode = __sc2363_mipi_cur_mode,
	.g_all_mode = __sc2363_mipi_get_all_mode,
	.s_power = __sc2363_mipi_set_power,
	.s_stream = __sc2363_mipi_set_stream,
	.g_info = __sc2363_mipi_get_info,
	.ioctl = __sc2363_mipi_ioctl,
	.s_register = __sc2363_mipi_set_register,
	.g_register = __sc2363_mipi_get_register,
};

