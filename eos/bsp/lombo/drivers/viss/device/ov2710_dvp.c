/*
 * vic_dev_ov2710.c - ov2710 driver code for LomboTech
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

#define DBG_SECTION_NAME	"OV2710-DVP"
#define DBG_LEVEL		DBG_LOG

#include <debug.h>
#include "viss.h"
/* #include "isp_dev.h" */
#include "viss_i2c.h"
#include <div.h>

/**
 * struct sensor_reg - sensor register data
 * @reg_add:	register address
 * @reg_value:	register value
 */
struct sensor_reg {
	rt_uint16_t reg_add;
	rt_uint16_t reg_value;
};
struct dev_mode *ov2710_cur_dvp_mode = RT_NULL;

#define DRV_VIC_0V2710_NAME "ov2710-dvp"
#define OV2710_DVP_REG_DELAY_FLAG  (0xffff)
rt_uint16_t reg_setted;

/* #define COLOR_BAR */
static const struct sensor_reg ov2710_OK_reg_list[] = {
	{0x3103, 0x93},
	{0x3008, 0x82},
	{0x3017, 0x7f},
	{0x3018, 0xfc},
	{0x3706, 0x61},
	{0x3712, 0x0c},
	{0x3630, 0x6d},
	{0x3801, 0xb4},
	{0x3621, 0x04},
	{0x3604, 0x60},
	{0x3603, 0xa7},
	{0x3631, 0x26},
	{0x3600, 0x04},
	{0x3620, 0x37},
	{0x3623, 0x00},
	{0x3702, 0x9e},
	{0x3703, 0x5c},
	{0x3704, 0x40},
	{0x370d, 0x0f},
	{0x3713, 0x9f},
	{0x3714, 0x4c},
	{0x3710, 0x9e},
	{0x3801, 0xc4},
	{0x3605, 0x05},
	{0x3606, 0x3f},
	{0x302d, 0x90},
	{0x370b, 0x40},
	{0x3716, 0x31},
	{0x3707, 0x52},
	{0x380d, 0x74},
	{0x5181, 0x20},
	{0x518f, 0x00},
	{0x4301, 0xff},
	{0x4303, 0x00},
	{0x3a00, 0x78},
	{0x300f, 0x88},
	{0x3011, 0x28},
	{0x3a1a, 0x06},
	{0x3a18, 0x00},
	{0x3a19, 0xe0},
	{0x3a13, 0x50},
	{0x382e, 0x0f},
	{0x381a, 0x1a},
	{0x401d, 0x02},
	{0x5688, 0x03},
	{0x5684, 0x07},
	{0x5685, 0xa0},
	{0x5686, 0x14},
	{0x5687, 0x43},
	{0x3a0f, 0xf0},
	{0x3a10, 0x38},
	{0x3a1b, 0xf8},
	{0x3a1e, 0x30},
	{0x3a11, 0x90},
	{0x3a1f, 0x10},
	{0x3010, 0x10},
	{0x3012, 0x00},/* 0x05 */
	{0x3503, 0x07},
	{0x3502, 0x00},
	{0x3501, 0x2e},
	{0x3500, 0x00},
	{0x350b, 0x0f},
	{0x350a, 0x00},
	{0x3406, 0x01},
	{0x3400, 0x04},
	{0x3401, 0x00},
	{0x3402, 0x04},
	{0x3403, 0x00},
	{0x3404, 0x04},
	{0x3405, 0x00},
	{0x4000, 0x05},
	{0x302c, 0x00},
	{0x5000, 0xdf},
    /*{0x3008,0x02},*/
};

static const struct sensor_reg ov2710_flicker_reg_list[] = {
	{0x3008, 0x42},
	{0x300e, 0x1a},
	{0x300f, 0x88},
	{0x3010, 0x00},
	{0x3011, 0x18},
	{0x3012, 0x00},
	{0x3016, 0x00},
	{0x3017, 0x7f},
	{0x3018, 0xfc},
	{0x3019, 0x00},
	{0x301a, 0x00},
	{0x301b, 0x00},
	{0x301c, 0x00},
	{0x301d, 0x00},
	{0x301e, 0x00},
	{0x302c, 0x02},
	{0x3100, 0x6c},
	{0x3103, 0x03},
	{0x3104, 0x01},
	{0x3200, 0x40},
	{0x3201, 0x4a},
	{0x3202, 0x54},
	{0x3203, 0x5e},
	{0x3212, 0x10},
	{0x3621, 0x04},
	{0x370d, 0x07},
	{0x3800, 0x01},
	{0x3801, 0xc4},
	{0x3802, 0x00},
	{0x3803, 0x0a},
	{0x3804, 0x07},
	{0x3805, 0x80},
	{0x3806, 0x04},
	{0x3807, 0x38},
	{0x3808, 0x07},
	{0x3809, 0x80},
	{0x380a, 0x04},
	{0x380b, 0x38},
	{0x380c, 0x09},
	{0x380d, 0x74},
	{0x380e, 0x04},
	{0x380f, 0x50},
	{0x3810, 0x10},
	{0x3818, 0x80},
	{0x3500, 0x00},
	{0x3501, 0x33},
	{0x3502, 0x80},
	{0x3503, 0x07},
	{0x350a, 0x00},
	{0x350b, 0x00},
	{0x350c, 0x00},
	{0x350d, 0x00},
	{0x3a00, 0x78},
	{0x3a01, 0x04},
	{0x3a02, 0x02},
	{0x3a03, 0x28},
	{0x3a04, 0x00},
	{0x3a08, 0x14},
	{0x3a09, 0xc0},
	{0x3a0a, 0x11},
	{0x3a0b, 0x40},
	{0x3a0d, 0x04},
	{0x3a0e, 0x03},
	{0x3a0f, 0x40},
	{0x3a10, 0x38},
	{0x3a11, 0x90},
	{0x3a12, 0x00},
	{0x3a13, 0x54},
	{0x3a14, 0x01},
	{0x3a15, 0xf2},
	{0x3a16, 0x00},
	{0x3a17, 0x89},
	{0x3a18, 0x00},
	{0x3a19, 0x7a},
	{0x3a1b, 0x48},
	{0x3a1c, 0x06},
	{0x3a1d, 0x18},
	{0x3a1e, 0x30},
	{0x3a1f, 0x10},
	{0x3a20, 0x20},
	{0x3d00, 0x00},
	{0x3d01, 0x00},
	{0x3d02, 0x00},
	{0x3d03, 0x00},
	{0x3d04, 0x00},
	{0x3d05, 0x00},
	{0x3d06, 0x00},
	{0x3d07, 0x00},
	{0x3d08, 0x00},
	{0x3d09, 0x00},
	{0x3d0a, 0x00},
	{0x3d0b, 0x00},
	{0x3d0c, 0x00},
	{0x3d0d, 0x00},
	{0x3d0e, 0x00},
	{0x3d0f, 0x00},
	{0x3d10, 0x00},
	{0x4000, 0x01},
	{0x4001, 0x00},
	{0x4002, 0x00},
	{0x401d, 0x22},
	{0x4201, 0x00},
	{0x4202, 0x00},
	{0x4700, 0x04},
	{0x4704, 0x00},
	{0x4708, 0x01},
	{0x4709, 0x00},
	{0x4800, 0x04},
	{0x4801, 0x03},
	{0x4803, 0x50},
	{0x4804, 0x8d},
	{0x4805, 0x10},
	{0x4810, 0xff},
	{0x4811, 0xff},
	{0x4812, 0x00},
	{0x4813, 0x00},
	{0x4814, 0x2a},
	{0x4815, 0x00},
	{0x4818, 0x00},
	{0x4819, 0x96},
	{0x481a, 0x00},
	{0x481b, 0x3c},
	{0x481c, 0x01},
	{0x481d, 0x86},
	{0x481e, 0x00},
	{0x481f, 0x3c},
	{0x4820, 0x00},
	{0x4821, 0x56},
	{0x4822, 0x00},
	{0x4823, 0x3c},
	{0x4824, 0x00},
	{0x4825, 0x32},
	{0x4826, 0x00},
	{0x4827, 0x32},
	{0x4828, 0x00},
	{0x4829, 0x64},
	{0x482a, 0x05},
	{0x482b, 0x04},
	{0x482c, 0x00},
	{0x482d, 0x00},
	{0x482e, 0x34},
	{0x482f, 0x00},
	{0x4830, 0x00},
	{0x4831, 0x04},
	{0x4832, 0x00},
	{0x5000, 0x59},
	{0x5001, 0x4e},
	{0x5002, 0xe0},
	{0x5005, 0xdc},
	{0x501f, 0x03},
	{0x503d, 0x00},
	{0x3400, 0x04},
	{0x3401, 0x00},
	{0x3402, 0x04},
	{0x3403, 0x00},
	{0x3404, 0x04},
	{0x3405, 0x00},
	{0x3406, 0x00},
	{0x5180, 0x40},
	{0x5181, 0x20},
	{0x5182, 0x04},
	{0x5183, 0x08},
	{0x518c, 0xf0},
	{0x518d, 0xf0},
	{0x518e, 0xf0},
	{0x518f, 0x00},
	{0x5680, 0x00},
	{0x5681, 0x00},
	{0x5682, 0x00},
	{0x5683, 0x00},
	{0x5684, 0x07},
	{0x5685, 0xa0},
	{0x5686, 0x04},
	{0x5687, 0x43},
	{0x5780, 0x7f},
	{0x5781, 0x20},
	{0x5782, 0x18},
	{0x5783, 0x08},
	{0x5784, 0x04},
	{0x5785, 0x40},
	{0x5786, 0x18},
	{0x5787, 0x08},
	{0x5788, 0x04},
	{0x5789, 0x08},
	{0x578a, 0x20},
	{0x578b, 0x07},
	{0x578c, 0x00},
	{0x5790, 0x00},
	{0x5791, 0x08},
	{0x5792, 0x00},
	{0x5793, 0x18},
	{0x5794, 0x00},
	{0x5795, 0x80},
	{0x5796, 0x01},
	{0x5797, 0x00},
	{0x5800, 0x03},
	{0x5801, 0x0c},
	{0x5802, 0x03},
	{0x5803, 0x06},
	{0x5804, 0x22},
	{0x5805, 0x07},
	{0x5806, 0xc2},
	{0x5807, 0x08},
	{0x5808, 0x03},
	{0x5809, 0x0c},
	{0x580a, 0x03},
	{0x580b, 0x06},
	{0x580c, 0x22},
	{0x580d, 0x07},
	{0x580e, 0xc2},
	{0x580f, 0x08},
	{0x5810, 0x03},
	{0x5811, 0x0c},
	{0x5812, 0x03},
	{0x5813, 0x06},
	{0x5814, 0x22},
	{0x5815, 0x07},
	{0x5816, 0xc2},
	{0x5817, 0x08},
	{0x5818, 0x04},
	{0x5819, 0x80},
	{0x581a, 0x06},
	{0x581b, 0x0c},
	{0x581c, 0x80},
	{0x6000, 0x1f},
	{0x6001, 0x01},
	{0x6002, 0x00},
	{0x6003, 0x76},
	{0x6004, 0x42},
	{0x6005, 0x01},
	{0x6006, 0x00},
	{0x6007, 0x76},
	{0x6008, 0x42},
	{0x6009, 0x01},
	{0x600a, 0x00},
	{0x600b, 0x76},
	{0x600c, 0x42},
	{0x600d, 0x01},
	{0x600e, 0x00},
	{0x600f, 0x76},
	{0x6010, 0x42},
	{0x6011, 0x01},
	{0x6012, 0x00},
	{0x6013, 0x76},
	{0x6014, 0x42},
	{0x302d, 0x90},
	{0x3600, 0x04},
	{0x3601, 0x04},
	{0x3602, 0x04},
	{0x3603, 0xa7},
	{0x3604, 0x60},
	{0x3605, 0x05},
	{0x3606, 0x12},
	{0x3620, 0x07},
	{0x3623, 0x40},
	{0x3630, 0x6b},
	{0x3631, 0x24},
	{0x3a1a, 0x06},
	{0x3702, 0x9e},
	{0x3703, 0x74},
	{0x3704, 0x10},
	{0x3706, 0x61},
	{0x370b, 0x40},
	{0x3710, 0x9e},
	{0x3712, 0x0c},
	{0x3713, 0x8b},
	{0x3714, 0x74},
	{0x3811, 0x06},
	{0x381c, 0x21},
	{0x381d, 0x50},
	{0x381e, 0x01},
	{0x381f, 0x20},
	{0x3820, 0x00},
	{0x3821, 0x00},
	{0x401c, 0x08},
	{0x4301, 0xff},
	{0x4303, 0x00},
	{0x5688, 0x03},
	{0x3500, 0x00},
	{0x3501, 0x4c},
	{0x3502, 0x1c},
	{0x350a, 0x05},
	{0x350b, 0x05},
	/* { 0x3008, 0x02 }, */
};

static struct dev_mode ov2710_dvp_mode[] = {
	{
		.index = 0,
		.inp_fmt = VISS_MBUS_FMT_YUYV8_2X8,
		.out_fmt = VISS_PIX_FMT_NV12,
		.colorspace = DEV_COLORSPACE_JPEG,
		.frame_rate = 40000,   /* 40fps */
		.frame_size.width = 1920,
		.frame_size.height = 1080,
		.usr_data = (void *)ov2710_flicker_reg_list,
		.usr_data_size = ARRAY_SIZE(ov2710_flicker_reg_list),
	},
	{
		.index = 1,
		.inp_fmt = VISS_MBUS_FMT_YUYV8_2X8,
		.out_fmt = VISS_PIX_FMT_NV12,
		.colorspace = DEV_COLORSPACE_JPEG,
		.frame_rate = 40000,   /* 40fps */
		.frame_size.width = 1280,
		.frame_size.height = 720,
		.usr_data = (void *)ov2710_flicker_reg_list,
		.usr_data_size = ARRAY_SIZE(ov2710_flicker_reg_list),
	},
};

static rt_err_t __ov2710_dvp_wake_up(void *hdl)
{
	struct video_dev *ov2710 = (struct video_dev *)hdl;

	return viss_i2c_write_reg_16bit(ov2710->i2c_client, 0x3008, 0x02);
}

static rt_err_t __ov2710_block_write(void *hdl, void *data, rt_int32_t size)
{
	struct video_dev *ov2710 = (struct video_dev *)hdl;
	struct sensor_reg *reg = (struct sensor_reg *)data;
	rt_int32_t i = 0;
	rt_err_t ret = 0;
	rt_int32_t delay = 0, temp = 0;

	for (i = 0; i < size; i++) {
		if (OV2710_DVP_REG_DELAY_FLAG == reg[i].reg_add) {
			temp = reg[i].reg_value + 9;
			do_div(temp, 10);
			delay = temp * 10;
			LOG_D("delay: %d", delay);
			do_div(delay, 10);
			rt_thread_delay(delay);
			continue;
		}
		ret = viss_i2c_write_reg_16bit(ov2710->i2c_client,
					reg[i].reg_add, reg[i].reg_value);
		if (RT_EOK != ret) {
			LOG_E("ov2710 block write error.");
			return ret;
		}
	}

	return RT_EOK;
}

static struct dev_mode *__ov2710_cur_mode(void *hdl)
{
	struct video_dev *ov2710 = (struct video_dev *)hdl;

	return ov2710->cur_mode;
}

static struct dev_mode *__ov2710_get_all_mode(void *hdl, rt_int32_t *num)
{
	RT_ASSERT(RT_NULL != num);
	*num = (rt_int32_t)ARRAY_SIZE(ov2710_dvp_mode);

	return ov2710_dvp_mode;
}

static rt_err_t __ov2710_set_mode(void *hdl, rt_int32_t index)
{
	struct video_dev *ov2710 = (struct video_dev *)hdl;
	rt_err_t ret = 0;
	/* LOG_D("ov2710 set mode."); */

	ov2710->cur_mode = &ov2710_dvp_mode[index];
	rt_int32_t num = 0;
	num = (rt_int32_t)ARRAY_SIZE(ov2710_dvp_mode);
	if (index >= num) {
		LOG_E("Input mode index error. num:%d index:%d\n", num, index);
		return -RT_EINVAL;
	}
	if (index >= ARRAY_SIZE(ov2710_dvp_mode)) {
		LOG_E("Input para error. index:%d\n", index);
		return -RT_EINVAL;
	}

	ov2710_cur_dvp_mode = &ov2710_dvp_mode[index];
	if (reg_setted) {
		LOG_D("ov2710 block has been writed.");
		return ret;
	}

	reg_setted = 1;
	/* LOG_D("ov2710 set block write."); */
	ret = __ov2710_block_write((void *)ov2710, ov2710_cur_dvp_mode->usr_data,
		ov2710_cur_dvp_mode->usr_data_size);
	if (RT_EOK != ret)
		return ret;

	return __ov2710_dvp_wake_up((void *)ov2710);
}

static rt_err_t __ov2710_set_gain(struct video_dev *sensor, float gain)
{
	rt_err_t ret = 0;

	rt_uint8_t  gain_multi, digi_gain, final_gain;

	if (gain < 2.0)
		gain_multi = 0;
	else if (gain < 4.0) {
		gain_multi = 0x10;
		gain /= 2.0;
	} else if (gain < 8.0) {
		gain_multi = 0x20 + 0x10;
		gain /= 4.0;
	} else if (gain < 16.0) {
		gain_multi = 0x40 + 0x20 + 0x10;
		gain /= 8.0;
	} else {
		gain_multi = 0x80 + 0x40 + 0x20 + 0x10;
		gain /= 16.0;
	}

	gain  = 16 * (gain - 1.0) + 0.5;

	digi_gain = (rt_uint8_t)gain;
	if (digi_gain > 0x0F)
		digi_gain = 0x0F;

	final_gain = gain_multi | digi_gain;

	ret = viss_i2c_write_reg_16bit(sensor->i2c_client, 0x350a, 0);
	ret = viss_i2c_write_reg_16bit(sensor->i2c_client, 0x350b, final_gain);

	return ret;
}

static rt_err_t __ov2710_set_shutter(struct video_dev *sensor, rt_uint32_t shutter)
{
	rt_err_t ret = 0;

	ret = viss_i2c_write_reg_16bit(sensor->i2c_client, 0x3500,
		(rt_uint8_t)((shutter & 0xf0000) >> 16));
	ret = viss_i2c_write_reg_16bit(sensor->i2c_client, 0x3501,
		(rt_uint8_t)((shutter & 0x0ff00) >>  8));
	ret = viss_i2c_write_reg_16bit(sensor->i2c_client, 0x3502,
		(rt_uint8_t)(shutter & 0x000ff));

	return ret;
}

static rt_err_t __ov2710_exp_ctrl(void *hdl, struct isp_exp_gain *exp_gain)
{
	struct video_dev *ov2710 = (struct video_dev *)hdl;
	rt_err_t ret = 0;

	if ((RT_NULL == hdl) || (RT_NULL == exp_gain)) {
		LOG_E("set ov2710_exp_ctrl fail.");
		return -RT_ERROR;
	}
	/* LOG_FLOAT("**********exp_ctrl: %d, %d.", exp_gain->gain, exp_gain->exp); */

	ret = viss_i2c_write_reg_16bit(ov2710->i2c_client, 0x3212, 0x00);

	ret = __ov2710_set_gain(ov2710, (float)(exp_gain->gain/256));
	if (RT_EOK != ret)
		LOG_E("ov2710 set gain fail.");

	ret = __ov2710_set_shutter(ov2710, exp_gain->exp);
	if (RT_EOK != ret)
		LOG_E("ov2710 set shutter fail.");

	ret = viss_i2c_write_reg_16bit(ov2710->i2c_client, 0x3212, 0x10);
	ret = viss_i2c_write_reg_16bit(ov2710->i2c_client, 0x3212, 0xA0);

	return ret;
}

static rt_err_t __ov2710_isp_get_sensor_info(void *hdl,
		struct isp_sensor_info *isp_sensor_info)
{
	isp_sensor_info->sensor_name = "ov2710";
	isp_sensor_info->pclk = 63750000;
	isp_sensor_info->vts = 1101;
	isp_sensor_info->hts = 2420;
	isp_sensor_info->input_width = 1920;
	isp_sensor_info->input_height = 1080;
	isp_sensor_info->output_widht = 1920;
	isp_sensor_info->output_height = 1080;
	isp_sensor_info->bayer_mode = ISP_BPAT_BGBGGRGR;
	return RT_EOK;
}

static rt_err_t __ov2710_set_register(void *hdl, struct viss_dbg_register *reg)
{
	struct video_dev *ov2710 = (struct video_dev *)hdl;
	rt_err_t ret = 0;

	/* LOG_D("ov2710_set_register 0x%llx: 0x%llx.", reg->add, reg->val); */
	ret = viss_i2c_write_reg_16bit(ov2710->i2c_client, (rt_uint16_t)reg->add,
				(rt_uint8_t)reg->val);
	return ret;
}

static rt_err_t __ov2710_get_register(void *hdl, struct viss_dbg_register *reg)
{
	struct video_dev *ov2710 = (struct video_dev *)hdl;
	rt_uint8_t value;
	rt_err_t ret = 0;

	ret = viss_i2c_read_reg_16bit(ov2710->i2c_client, reg->add, &value);
	reg->val = value;

	return ret;
}

static rt_err_t __ov2710_set_power(void *hdl, rt_bool_t on)
{
	struct video_dev *ov2710 = (struct video_dev *)hdl;

	if ((1 != ov2710->pwdn_valid) || (1 != ov2710->rst_valid))
		return RT_EOK;

	if (RT_TRUE == on) {
		pinctrl_gpio_set_value(ov2710->pctrl, ov2710->rst_gpio, 0);
		rt_thread_delay(1); /* 10ms */
		pinctrl_gpio_set_value(ov2710->pctrl, ov2710->pwdn_gpio, 1);
		rt_thread_delay(1); /* 10ms */
		pinctrl_gpio_set_value(ov2710->pctrl, ov2710->pwdn_gpio, 0);
		rt_thread_delay(1); /* 10ms */
		pinctrl_gpio_set_value(ov2710->pctrl, ov2710->rst_gpio, 1);
		rt_thread_delay(1); /* 10ms */
	} else {
		/* TO DO */
	}
	return RT_EOK;
}

static rt_err_t __ov2710_set_stream(void *hdl, rt_bool_t enable)
{
	struct video_dev *ov2710 = (struct video_dev *)hdl;

	LOG_D("__ov2710_set_stream %d.", enable);
	if (enable)
		viss_i2c_write_reg_16bit(ov2710->i2c_client, 0x3008, 0x02);
	else
		viss_i2c_write_reg_16bit(ov2710->i2c_client, 0x3008, 0x42);
	return RT_EOK;
}

rt_err_t __ov2710_ioctl(void *hdl, rt_int32_t cmd, void *para)
{
	rt_err_t ret = 0;
	struct isp_exp_gain *ov2710_exp = RT_NULL;
	struct isp_sensor_info *ov2710_isp_sensor_info = RT_NULL;

	switch (cmd) {
	case ISP_GET_SENSOR_INFO:
		LOG_D(" ");
		ov2710_isp_sensor_info = (struct isp_sensor_info *)para;
		__ov2710_isp_get_sensor_info(hdl, ov2710_isp_sensor_info);
		break;
	case ISP_SET_EXP_GAIN:
		ov2710_exp = (struct isp_exp_gain *)para;
		__ov2710_exp_ctrl(hdl, ov2710_exp);
		/* LOG_FLOAT("ISP_SET_EXP_GAIN: exp: %d, gain: %f.",
			ov2710_exp->exp, ov2710_exp->gain); */
		break;
	default:
		return -RT_ERROR;
	}

	return ret;
}

static rt_err_t __ov2710_get_info(void *hdl, struct viss_source_info *info)
{
	struct video_dev *ov2710 = (struct video_dev *)hdl;

	if ((RT_NULL == hdl) || (RT_NULL == info)) {
		LOG_E("Get ov2710 information fail.");
		return -RT_ERROR;
	}
	rt_memcpy(info, &ov2710->info, sizeof(struct viss_source_info));

	return RT_EOK;
}

static rt_err_t __ov2710_parser_config(void *hdl)
{
	struct video_dev *ov2710 = (struct video_dev *)hdl;
	struct viss_source_info info;
	rt_uint32_t tmp_data = 0;
	const char *status;
	const char *i2c_bus_name;
	const char *module = DRV_VIC_0V2710_NAME;
	rt_err_t ret = 0;
	rt_int32_t  use_enabel = 0;

	ov2710->pwdn_valid = 0;
	ov2710->rst_valid = 0;
	ov2710->mclk_valid = 0;
	rt_memset(&info, 0, sizeof(info));
	ret = config_get_string(module, "status", &status);
	if (-1 == ret) {
		LOG_E("Get status fail.");
	} else {
		if (0 == strcmp(status, "okay"))
			use_enabel = 1;
	}
	if (0 == use_enabel) {
		LOG_W("Ov2710 dvp disable.");
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
	ret = config_get_u32(module, "if-mode", &info.if_mode);
	if (-1 == ret)
		LOG_E("Get if-mode fail.");
	ret = config_get_u32(module, "out-path", &tmp_data);
	if (-1 == ret) {
		LOG_E("Get out-path fail.");
	} else {
		if (1 == tmp_data)
			info.out_path = VISS_IO_ISP;
		else
			info.out_path = VISS_IO_DMA;
	}
	ret = config_get_u32(module, "channel-num", &info.channel_num);
	if (-1 == ret)
		LOG_E("Get channel_num fail.");
	ret = config_get_u32(module, "reg", &info.i2c_addr);
	if (-1 == ret)
		LOG_E("Get i2c_addr fail.");
	ret = config_get_u32(module, "hsync-active", &info.href);
	if (-1 == ret)
		LOG_E("Get href fail.");
	ret = config_get_u32(module, "vsync-active", &info.vref);

	if (-1 == ret)
		LOG_E("Get vref fail.");
	ret = config_get_u32(module, "pclk-sample", &info.pclk);
	if (-1 == ret)
		LOG_E("Get pclk fail.");
	ret = config_get_u32(module, "field-sel", &info.field_sel);
	if (-1 == ret)
		LOG_E("Get field_sel fail.");
	ret = config_get_u32(module, "mclk-freq", &info.mclk_freq);
	if (-1 == ret)
		LOG_E("Get mclk_freq fail.");
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
	rt_memcpy(&ov2710->info, &info, sizeof(struct viss_source_info));

	/* power pin */
	ov2710->pwdn_valid = 1;
	ret = config_get_u32_array(module, "dvp-pwdn",
				ov2710->pwdn_val, ARRAY_SIZE(ov2710->pwdn_val));
	if (ret != ARRAY_SIZE(ov2710->pwdn_val)) {
		LOG_E("vic: power pin config error. ret:%d", ret);
		ov2710->pwdn_valid = 0;
	}

	/* reset pin */
	ov2710->rst_valid = 1;
	ret = config_get_u32_array(module, "dvp-rst",
				ov2710->rst_val, ARRAY_SIZE(ov2710->rst_val));
	if (ret != ARRAY_SIZE(ov2710->rst_val)) {
		LOG_E("vic: reset pin config error. ret:%d", ret);
		ov2710->rst_valid = 0;
	}

	/* mclk pin */
	ov2710->mclk_valid = 1;
	ret = config_get_u32_array(module, "dvp-mclk",
				ov2710->mclk_val, ARRAY_SIZE(ov2710->mclk_val));
	if (ret != ARRAY_SIZE(ov2710->mclk_val)) {
		LOG_E("vic: mclk pin config error. ret:%d", ret);
		ov2710->mclk_valid = 0;
	}

	return RT_EOK;
}

static rt_err_t __ov2710_prepare(void *hdl)
{
	return __ov2710_parser_config(hdl);
}

static rt_err_t __ov2710_init(void *hdl)
{
	rt_int32_t ret = 0;
	rt_uint8_t tmp[2] = {0};
	rt_uint16_t id = 0;
	struct video_dev *ov2710 = (struct video_dev *)hdl;
	reg_setted = 0;
	/* LOG_D("__ov2710_init"); */

	RT_ASSERT(RT_NULL != ov2710);
	ov2710->pctrl = pinctrl_get(DRV_VIC_0V2710_NAME);
	if (RT_NULL == ov2710->pctrl)
		return -RT_ERROR;
	if (1 == ov2710->pwdn_valid) {
		ov2710->pwdn_gpio = pinctrl_gpio_request(ov2710->pctrl,
					ov2710->pwdn_val[0], ov2710->pwdn_val[1]);
		if (ov2710->pwdn_gpio >= 0) {
			pinctrl_gpio_set_function(ov2710->pctrl, ov2710->pwdn_gpio,
						ov2710->pwdn_val[2]);
			pinctrl_gpio_set_drv_level(ov2710->pctrl, ov2710->pwdn_gpio,
						ov2710->pwdn_val[3]);
			pinctrl_gpio_set_pud_mode(ov2710->pctrl, ov2710->pwdn_gpio,
						ov2710->pwdn_val[4]);
			pinctrl_gpio_set_pud_res(ov2710->pctrl, ov2710->pwdn_gpio,
						ov2710->pwdn_val[5]);
			pinctrl_gpio_set_value(ov2710->pctrl, ov2710->pwdn_gpio,
						ov2710->pwdn_val[6]);
		} else
			ov2710->pwdn_valid = 0;
	}
	if (1 == ov2710->rst_valid) {
		ov2710->rst_gpio = pinctrl_gpio_request(ov2710->pctrl,
					ov2710->rst_val[0], ov2710->rst_val[1]);
		if (ov2710->rst_gpio >= 0) {
			pinctrl_gpio_set_function(ov2710->pctrl, ov2710->rst_gpio,
						ov2710->rst_val[2]);
			pinctrl_gpio_set_drv_level(ov2710->pctrl, ov2710->rst_gpio,
						ov2710->rst_val[3]);
			pinctrl_gpio_set_pud_mode(ov2710->pctrl, ov2710->rst_gpio,
						ov2710->rst_val[4]);
			pinctrl_gpio_set_pud_res(ov2710->pctrl, ov2710->rst_gpio,
						ov2710->rst_val[5]);
			pinctrl_gpio_set_value(ov2710->pctrl, ov2710->rst_gpio,
						ov2710->rst_val[6]);
		} else
			ov2710->rst_valid = 0;
	}
	if (1 == ov2710->mclk_valid) {
		ov2710->mclk_gpio = pinctrl_gpio_request(ov2710->pctrl,
					ov2710->mclk_val[0], ov2710->mclk_val[1]);
		if (ov2710->mclk_gpio >= 0) {
			pinctrl_gpio_set_function(ov2710->pctrl, ov2710->mclk_gpio,
						ov2710->mclk_val[2]);
			pinctrl_gpio_set_drv_level(ov2710->pctrl, ov2710->mclk_gpio,
						ov2710->mclk_val[3]);
			pinctrl_gpio_set_pud_mode(ov2710->pctrl, ov2710->mclk_gpio,
						ov2710->mclk_val[4]);
			pinctrl_gpio_set_pud_res(ov2710->pctrl, ov2710->mclk_gpio,
						ov2710->mclk_val[5]);
			pinctrl_gpio_set_value(ov2710->pctrl, ov2710->mclk_gpio,
						ov2710->mclk_val[6]);
		} else
			ov2710->mclk_valid = 0;
	}
	__ov2710_set_power((void *)ov2710, RT_TRUE);

	ov2710->i2c_client = rt_zalloc(sizeof(struct viss_i2c_client));
	if (RT_NULL == ov2710->i2c_client)
		return -RT_ENOMEM;
	ov2710->i2c_client->i2c_bus = rt_i2c_bus_device_find(ov2710->info.i2c_bus_name);
	if (RT_NULL == ov2710->i2c_client->i2c_bus) {
		LOG_E("can't find bus dev \"%s\"", ov2710->info.i2c_bus_name);
		goto exit;
	}
	ov2710->i2c_client->i2c_addr = ov2710->info.i2c_addr;
	ret = viss_i2c_read_reg_16bit(ov2710->i2c_client, 0x300a, &tmp[0]);
	LOG_D("i2c_addr: %x, ret: %d. 0x300a: %x",
		ov2710->i2c_client->i2c_addr, ret, tmp[0]);
	ret = viss_i2c_read_reg_16bit(ov2710->i2c_client, 0x300b, &tmp[1]);
	LOG_D("i2c_addr: %x, ret: %d. 0x300b: %x",
		ov2710->i2c_client->i2c_addr, ret, tmp[1]);
	id = (tmp[0] << 8) | tmp[1];
	if (id != 0x2710) {
		LOG_E("ID wrong! (0x%x)\n", id);
		goto exit;
	}

	return RT_EOK;

exit:
	if (ov2710->pctrl) {
		if (ov2710->rst_valid)
			pinctrl_gpio_free(ov2710->pctrl, ov2710->rst_gpio);
		if (ov2710->pwdn_valid)
			pinctrl_gpio_free(ov2710->pctrl, ov2710->pwdn_gpio);
		if (ov2710->mclk_valid)
			pinctrl_gpio_free(ov2710->pctrl, ov2710->mclk_gpio);
		pinctrl_put(ov2710->pctrl);
		ov2710->pctrl = RT_NULL;
	}
	if (ov2710->i2c_client) {
		rt_free(ov2710->i2c_client);
		ov2710->i2c_client = RT_NULL;
	}

	return -RT_ERROR;
}

static void __ov2710_exit(void *hdl)
{
	struct video_dev *ov2710 = (struct video_dev *)hdl;
	reg_setted = 0;
	LOG_D("__ov2710_exit");

	__ov2710_set_power(ov2710, RT_FALSE);
	if (ov2710->pctrl) {
		if (ov2710->rst_valid)
			pinctrl_gpio_free(ov2710->pctrl, ov2710->rst_gpio);
		if (ov2710->pwdn_valid)
			pinctrl_gpio_free(ov2710->pctrl, ov2710->pwdn_gpio);
		if (ov2710->mclk_valid)
			pinctrl_gpio_free(ov2710->pctrl, ov2710->mclk_gpio);
		pinctrl_put(ov2710->pctrl);
		ov2710->pctrl = RT_NULL;
	}
	if (ov2710->i2c_client) {
		rt_free(ov2710->i2c_client);
		ov2710->i2c_client = RT_NULL;
	}
}

struct video_dev ov2710_dvp = {
	.name = DRV_VIC_0V2710_NAME,
	.group_id = GRP_ID_VIC,
	.prepare = __ov2710_prepare,
	.init = __ov2710_init,
	.exit = __ov2710_exit,
	.s_mode = __ov2710_set_mode,
	.g_cur_mode = __ov2710_cur_mode,
	.g_all_mode = __ov2710_get_all_mode,
	.s_power = __ov2710_set_power,
	.s_stream = __ov2710_set_stream,
	.g_info = __ov2710_get_info,
	.ioctl = __ov2710_ioctl,
	.s_register = __ov2710_set_register,
	.g_register = __ov2710_get_register,
};

