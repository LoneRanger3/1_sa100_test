/*
 * isp_dev_sc4353.c - sc4353 driver code for LomboTech
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

#define DBG_SECTION_NAME	"SC4353-MIPI"
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
u32 SENSOR_MAX_GAIN = 15.75 * 31.5;

#define DRV_MCSI_SC4353_NAME "sc4353-mipi"
#define DRV_MCSI_SC4353_PWR_NAME "cam-pwr-en"

#define SC4353_MIPI_REG_DELAY_FLAG  (0xffff)
#define MAX_AGAIN (15 * 1024 + 512)
#define MAX_DGAIN (12 * 1024)

/* static struct viss_i2c_client *sc4353_mipi_i2c_client = RT_NULL; */

/*
 * 2lane YUV init
 */
static const struct sensor_reg sc4353_reg_list[] = {
	/* SC4353   2560*1440  2700*1500   30fps */
	{0x0103, 0x01},
	{0x0100, 0x00},
	{0x36e9, 0xd4},
	{0x36f9, 0xd4},
	{0x301f, 0x21},
	{0x3213, 0x06},
	{0x3301, 0x10},
	{0x3306, 0x60},
	{0x3309, 0x88},
	{0x330a, 0x01},
	{0x330b, 0x08},
	{0x330e, 0x38},
	{0x330f, 0x04},
	{0x3310, 0x20},
	{0x3314, 0x94},
	{0x331f, 0x79},
	{0x3342, 0x01},
	{0x3347, 0x05},
	{0x3364, 0x1d},
	{0x3367, 0x10},
	{0x33b6, 0x07},
	{0x33b7, 0x2f},
	{0x33b8, 0x10},
	{0x33b9, 0x18},
	{0x33ba, 0x70},
	{0x360f, 0x05},
	{0x3614, 0x80},
	{0x3620, 0xa8},
	{0x3622, 0xf6},
	{0x3625, 0x0a},
	{0x3630, 0xc0},
	{0x3631, 0x88},
	{0x3632, 0x18},
	{0x3633, 0x33},
	{0x3636, 0x25},
	{0x3637, 0x70},
	{0x3638, 0x22},
	{0x363a, 0x90},
	{0x363b, 0x09},
	{0x3650, 0x06},
	{0x366e, 0x04},
	{0x3670, 0x0a},
	{0x3671, 0xf6},
	{0x3672, 0xf6},
	{0x3673, 0x16},
	{0x3674, 0xc0},
	{0x3675, 0xc8},
	{0x3676, 0xaf},
	{0x367a, 0x08},
	{0x367b, 0x38},
	{0x367c, 0x38},
	{0x367d, 0x3f},
	{0x3690, 0x33},
	{0x3691, 0x34},
	{0x3692, 0x44},
	{0x369c, 0x38},
	{0x369d, 0x3f},
	{0x36e9, 0x52},
	{0x36ea, 0x65},
	{0x36ed, 0x03},
	{0x36f9, 0x53},
	{0x36fa, 0x65},
	{0x36fd, 0x04},
	{0x3902, 0xc5},
	{0x3904, 0x10},
	{0x3908, 0x41},
	{0x3933, 0x0a},
	{0x3934, 0x0d},
	{0x3940, 0x65},
	{0x3941, 0x18},
	{0x3942, 0x02},
	{0x3943, 0x12},
	{0x395e, 0xa0},
	{0x3960, 0x9d},
	{0x3961, 0x9d},
	{0x3962, 0x89},
	{0x3963, 0x80},
	{0x3966, 0x4e},
	{0x3980, 0x60},
	{0x3981, 0x30},
	{0x3982, 0x15},
	{0x3983, 0x10},
	{0x3984, 0x0d},
	{0x3985, 0x20},
	{0x3986, 0x30},
	{0x3987, 0x60},
	{0x3988, 0x04},
	{0x3989, 0x0c},
	{0x398a, 0x14},
	{0x398b, 0x24},
	{0x398c, 0x50},
	{0x398d, 0x32},
	{0x398e, 0x1e},
	{0x398f, 0x0a},
	{0x3990, 0xc0},
	{0x3991, 0x50},
	{0x3992, 0x22},
	{0x3993, 0x0c},
	{0x3994, 0x10},
	{0x3995, 0x38},
	{0x3996, 0x80},
	{0x3997, 0xff},
	{0x3998, 0x08},
	{0x3999, 0x16},
	{0x399a, 0x28},
	{0x399b, 0x40},
	{0x399c, 0x50},
	{0x399d, 0x28},
	{0x399e, 0x18},
	{0x399f, 0x0c},
	{0x3e01, 0xbb},
	{0x3e02, 0x00},
	{0x3e09, 0x20},
	{0x3e25, 0x03},
	{0x3e26, 0x20},
	{0x5781, 0x04},
	{0x5782, 0x04},
	{0x5783, 0x02},
	{0x5784, 0x02},
	{0x5785, 0x40},
	{0x5786, 0x20},
	{0x5787, 0x18},
	{0x5788, 0x10},
	{0x5789, 0x10},
	{0x57a4, 0xa0},

	{0x3221, 0x66},/*flip & mirror*/
	{0x3928, 0x04},

	{0x36e9, 0x52},
	{0x36f9, 0x53},
	{0x0100, 0x00},
};

struct dev_mode *cur_sc4353_mipi_mode = RT_NULL;
static struct dev_mode sc4353_mipi_mode[] = {
	{
		.index = 0,
		.inp_fmt = VISS_MBUS_FMT_YUYV8_2X8,/* VISS_MBUS_FMT_SBGGR10_1X10 */
		.out_fmt = VISS_PIX_FMT_NV12,/* VISS_PIX_FMT_SBGGR10 */
		.colorspace = DEV_COLORSPACE_JPEG,
		.frame_rate = 30000,   /* 30fps */
		.frame_size.width = 2560,
		.frame_size.height = 1440,
		.usr_data = (void *)sc4353_reg_list,
		.usr_data_size = ARRAY_SIZE(sc4353_reg_list),
	},
	{
		.index = 1,
		.inp_fmt = VISS_MBUS_FMT_YUYV8_2X8,/* VISS_MBUS_FMT_SBGGR10_1X10 */
		.out_fmt = VISS_PIX_FMT_NV12,/* VISS_PIX_FMT_SBGGR10 */
		.colorspace = DEV_COLORSPACE_JPEG,
		.frame_rate = 30000,   /* 30fps */
		.frame_size.width = 1920,
		.frame_size.height = 1080,
		.usr_data = (void *)sc4353_reg_list,
		.usr_data_size = ARRAY_SIZE(sc4353_reg_list),
	},
	{
		.index = 2,
		.inp_fmt = VISS_MBUS_FMT_YUYV8_2X8,
		.out_fmt = VISS_PIX_FMT_NV12,
		.colorspace = DEV_COLORSPACE_JPEG,
		.frame_rate = 30000,   /* 30fps */
		.frame_size.width = 1280,
		.frame_size.height = 720,
		.usr_data = (void *)sc4353_reg_list,
		.usr_data_size = ARRAY_SIZE(sc4353_reg_list),
	},
};

static rt_err_t __sc4353_mipi_block_write(void *hdl,
						void *data, rt_int32_t size)
{
	struct video_dev *sc4353 = (struct video_dev *)hdl;
	struct sensor_reg *reg = (struct sensor_reg *)data;
	rt_int32_t i = 0;
	rt_err_t ret = 0;
	rt_int32_t delay = 0, temp = 0;

	for (i = 0; i < size; i++) {
		if (SC4353_MIPI_REG_DELAY_FLAG == reg[i].reg_add) {
			temp = reg[i].reg_value + 9;
			do_div(temp, 10);
			delay = temp * 10;
			LOG_D("delay: %d", delay);
			do_div(delay, 10);
			rt_thread_delay(delay);
			continue;
		}
		ret = viss_i2c_write_reg_16bit(sc4353->i2c_client,
					reg[i].reg_add, reg[i].reg_value);
		if (RT_EOK != ret) {
			LOG_E("i2c write %x value:%x fail.\n",
				reg[i].reg_add, reg[i].reg_value);
			return ret;
		}
	}

	return RT_EOK;
}

static struct dev_mode *__sc4353_mipi_cur_mode(void *hdl)
{
	struct video_dev *sc4353 = (struct video_dev *)hdl;
	return sc4353->cur_mode;
}

static struct dev_mode *__sc4353_mipi_get_all_mode(void *hdl, rt_int32_t *num)
{
	RT_ASSERT(RT_NULL != num);
	*num = (rt_int32_t)ARRAY_SIZE(sc4353_mipi_mode);

	return sc4353_mipi_mode;
}
static int reg_writed;
static rt_err_t __sc4353_mipi_set_mode(void *hdl, rt_int32_t index)
{
	struct video_dev *sc4353 = (struct video_dev *)hdl;
	rt_int32_t num = 0;
	rt_err_t ret = 0;

	num = (rt_int32_t)ARRAY_SIZE(sc4353_mipi_mode);
	if (index >= num) {
		LOG_E("Input mode index error. num:%d index:%d\n", num, index);
		return -RT_EINVAL;
	}
	if (index >= ARRAY_SIZE(sc4353_mipi_mode)) {
		LOG_E("Input para error. index:%d\n", index);
		return -RT_EINVAL;
	}
	sc4353->cur_mode = &sc4353_mipi_mode[index];
	if (reg_writed)
		return RT_EOK;

	reg_writed = 1;
	ret = __sc4353_mipi_block_write(hdl, sc4353->cur_mode->usr_data,
				sc4353->cur_mode->usr_data_size);
	if (RT_EOK != ret)
		return ret;

	return 0;
}

static rt_err_t __sc4353_set_shutter(struct video_dev *sensor, rt_uint32_t shutter)
{
	struct video_dev *sc4353 = (struct video_dev *)sensor;

	viss_i2c_write_reg_16bit(sc4353->i2c_client, 0x3e00,
		(shutter >> 12) & 0x0f);
	viss_i2c_write_reg_16bit(sc4353->i2c_client, 0x3e01,
		(shutter >> 4) & 0xff);
	viss_i2c_write_reg_16bit(sc4353->i2c_client, 0x3e02,
		(shutter << 4) & 0xf0);

	return RT_EOK;
}

static rt_err_t __sc4353_set_gain(void *hdl, rt_uint32_t gain)
{
	u32 again_fine, dgain_fine;
	u32 again = 0, dgain = 0;
	struct video_dev *sc4353 = (struct video_dev *)hdl;

	if (gain < 2 * 1024) {
		again = 3;
		again_fine = gain >> 5;
		dgain = 0;
		dgain_fine = (gain * 4 / again_fine);
	} else if (gain <  4 * 1024) {
		again = 7;
		again_fine = gain >> 6;
		dgain = 0;
		dgain_fine = (gain * 2 / again_fine);
	} else if (gain < 8 * 1024) {
		again = 0xf;
		again_fine = gain >> 7;
		dgain = 0;
		dgain_fine = (gain / again_fine);
	} else if (gain <= 15.75 * 1024) {
		again = 0x1f;
		again_fine = gain >> 8;
		dgain = 0;
		dgain_fine = (gain / 2 / again_fine);
	} else if (gain < 31.5 * 1024) {
		again = 0x1f;
		again_fine = 0x3f;
		dgain = 0;
		dgain_fine = gain / 126;
	} else if (gain < 63 * 1024) {
		again = 0x1f;
		again_fine = 0x3f;
		dgain = 1;
		dgain_fine = gain / 252;
	} else if (gain < 126 * 1024) {
		again = 0x1f;
		again_fine = 0x3f;
		dgain = 3;
		dgain_fine = gain / 504;
	} else if (gain < 252 * 1024) {
		again = 0x1f;
		again_fine = 0x3f;
		dgain = 7;
		dgain_fine = gain / 1008;
	} else if (gain <= SENSOR_MAX_GAIN * 1024) {
		again = 0x1f;
		again_fine = 0x3f;
		dgain = 0xF;
		dgain_fine = gain / 2016;
	} else {
		again = 0x1f;
		again_fine = 0x3f;
		dgain = 0xF;
		dgain_fine = 252;
	}

	viss_i2c_write_reg_16bit(sc4353->i2c_client, 0x3e09, again_fine);
	viss_i2c_write_reg_16bit(sc4353->i2c_client, 0x3e08, again);
	viss_i2c_write_reg_16bit(sc4353->i2c_client, 0x3e07, dgain_fine);
	viss_i2c_write_reg_16bit(sc4353->i2c_client, 0x3e06, dgain);

	return RT_EOK;
}

static rt_err_t __sc4353_set_logic(void *hdl, u32 gain)
{
	struct video_dev *sc4353 = (struct video_dev *)hdl;

	if (gain < 2*1024) {
		viss_i2c_write_reg_16bit(sc4353->i2c_client, 0x3632, 0x18);
		viss_i2c_write_reg_16bit(sc4353->i2c_client, 0x3631, 0x88);
		viss_i2c_write_reg_16bit(sc4353->i2c_client, 0x3636, 0x25);
		return;
	}

	if (gain < 4*1024) {
		viss_i2c_write_reg_16bit(sc4353->i2c_client, 0x3632, 0x18);
		viss_i2c_write_reg_16bit(sc4353->i2c_client, 0x3631, 0x8e);
		viss_i2c_write_reg_16bit(sc4353->i2c_client, 0x3636, 0x25);
		return;
	}

	if (gain < 8*1024) {
		viss_i2c_write_reg_16bit(sc4353->i2c_client, 0x3632, 0x18);
		viss_i2c_write_reg_16bit(sc4353->i2c_client, 0x3631, 0x80);
		viss_i2c_write_reg_16bit(sc4353->i2c_client, 0x3636, 0x65);
		return;
	}

	if (gain < 15.75*1024) {
		viss_i2c_write_reg_16bit(sc4353->i2c_client, 0x3632, 0x18);
		viss_i2c_write_reg_16bit(sc4353->i2c_client, 0x3631, 0x80);
		viss_i2c_write_reg_16bit(sc4353->i2c_client, 0x3636, 0x65);
		return;
	}

	viss_i2c_write_reg_16bit(sc4353->i2c_client, 0x3632, 0xd8);
	viss_i2c_write_reg_16bit(sc4353->i2c_client, 0x3631, 0x80);
	viss_i2c_write_reg_16bit(sc4353->i2c_client, 0x3636, 0x65);

	return RT_EOK;
}

static rt_err_t __sc4353_exp_ctrl(void *hdl, struct isp_exp_gain *exp_gain)
{
	struct video_dev *sc4353 = (struct video_dev *)hdl;
	rt_err_t ret = 0;

	if ((RT_NULL == hdl) || (RT_NULL == exp_gain)) {
		LOG_E("set sc2363_exp_ctrl fail.");
		return -RT_ERROR;
	}

	LOG_D("**********exp_ctrl: %d, %d.", exp_gain->gain, exp_gain->exp);

	ret = __sc4353_set_gain(sc4353, exp_gain->gain * 4);
	if (RT_EOK != ret)
		LOG_E("sc4353 set gain fail.");

	ret = __sc4353_set_shutter(sc4353, exp_gain->exp / 8);
	if (RT_EOK != ret)
		LOG_E("sc4353 set shutter fail.");

	viss_i2c_write_reg_16bit(sc4353->i2c_client, 0x3812, 0x00);

	ret = __sc4353_set_logic(sc4353, exp_gain->gain * 4);
	if (RT_EOK != ret)
		LOG_E("sc4353 set logic fail.");
	viss_i2c_write_reg_16bit(sc4353->i2c_client, 0x3812, 0x30);

	return ret;
}
static rt_err_t __sc4353_isp_get_sensor_info(void *hdl,
	struct isp_sensor_info *isp_sensor_info)
{
	isp_sensor_info->sensor_name = "sc4353";
	isp_sensor_info->pclk = 121.5 * 1000 * 1000;
	isp_sensor_info->vts = 1500;
	isp_sensor_info->hts = 2700;
	isp_sensor_info->input_width = 2560;
	isp_sensor_info->input_height = 1440;
	isp_sensor_info->output_widht = 2560;
	isp_sensor_info->output_height = 1440;
	isp_sensor_info->bayer_mode = ISP_BPAT_BGBGGRGR;

	return RT_EOK;
}
static rt_err_t __sc4353_mipi_set_register(void *hdl,
					struct viss_dbg_register *reg)
{
	struct video_dev *sc4353 = (struct video_dev *)hdl;
	rt_err_t ret = 0;

	ret = viss_i2c_write_reg_16bit(sc4353->i2c_client, reg->add, reg->val);

	return ret;
}

static rt_err_t __sc4353_mipi_get_register(void *hdl,
					struct viss_dbg_register *reg)
{
	struct video_dev *sc4353 = (struct video_dev *)hdl;
	rt_uint8_t value;
	rt_err_t ret = 0;

	ret = viss_i2c_read_reg_16bit(sc4353->i2c_client, reg->add, &value);
	reg->val = value;

	return ret;
}

static rt_err_t __sc4353_mipi_set_power(void *hdl, rt_bool_t on)
{
	struct video_dev *sc4353 = (struct video_dev *)hdl;
	/* LOG_D("__sc4353_mipi_set_power"); */

	if ((1 != sc4353->pwdn_valid) || (1 != sc4353->rst_valid))
		return RT_EOK;

	if (RT_TRUE == on) {
		/* LOG_D("__sc4353_mipi_set_power 1111"); */
		pinctrl_gpio_set_value(sc4353->pctrl, sc4353->pwdn_gpio, 0);
		rt_thread_delay(1); /* 10ms */
		pinctrl_gpio_set_value(sc4353->pctrl, sc4353->rst_gpio, 0);
		rt_thread_delay(1); /* 10ms */
		pinctrl_gpio_set_value(sc4353->pctrl, sc4353->rst_gpio, 1);
		rt_thread_delay(1); /* 10ms */
		pinctrl_gpio_set_value(sc4353->pctrl, sc4353->pwdn_gpio, 1);
	} else {
		/* LOG_D("__sc4353_mipi_set_power 222 "); */
		/* TO DO */
	}
	/* LOG_D("__sc4353_mipi_set_power 3333"); */

	return RT_EOK;
}

static rt_err_t __sc4353_mipi_set_stream(void *hdl, rt_bool_t enable)
{
#if 1
	rt_int32_t ret = 0;
	struct video_dev *sc4353 = (struct video_dev *)hdl;
	if (enable)
		ret = viss_i2c_write_reg_16bit(sc4353->i2c_client, 0x100, 1);
	else
		ret = viss_i2c_write_reg_16bit(sc4353->i2c_client, 0x100, 0);
#endif
	return ret;
}

rt_err_t __sc4353_mipi_ioctl(void *hdl, rt_int32_t cmd, void *para)
{
	rt_err_t ret = 0;
	struct isp_exp_gain *sc4353_exp = RT_NULL;
	struct isp_sensor_info *sc4353_isp_sensor_info = RT_NULL;

	switch (cmd) {
	case ISP_GET_SENSOR_INFO:
		sc4353_isp_sensor_info = (struct isp_sensor_info *)para;
		__sc4353_isp_get_sensor_info(hdl, sc4353_isp_sensor_info);
		break;
	case ISP_SET_EXP_GAIN:
		sc4353_exp = (struct isp_exp_gain *)para;
		__sc4353_exp_ctrl(hdl, sc4353_exp);
		/* LOG_FLOAT("ISP_SET_EXP_GAIN: exp: %d, gain: %f.",
			ov2710_exp->exp, ov2710_exp->gain); */
		break;
	default:
		return -RT_ERROR;
	}
	return ret;
}

static rt_err_t __sc4353_mipi_get_info(void *hdl,
			struct viss_source_info *info)
{
	struct video_dev *sc4353 = (struct video_dev *)hdl;
	/* LOG_D("__sc4353_mipi_get_info in"); */

	if ((RT_NULL == hdl) || (RT_NULL == info)) {
		LOG_E("Get sc4353 information fail.");
		return -RT_ERROR;
	}
	rt_memcpy(info, &sc4353->info, sizeof(struct viss_source_info));
	/* LOG_D("__sc4353_mipi_get_info out"); */

	return RT_EOK;
}

static rt_err_t __sc4353_mipi_parser_config(void *hdl)
{
	struct video_dev *sc4353 = (struct video_dev *)hdl;
	struct viss_source_info info;
	rt_uint32_t tmp_data = 0;
	const char *status;
	const char *i2c_bus_name;
	const char *module = DRV_MCSI_SC4353_NAME;
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
		LOG_W("sc4353 mipi disable.");
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
	rt_memcpy(&sc4353->info, &info, sizeof(struct viss_source_info));
	/* LOG_D("sc4353->info.if_type:%d, lane_num: %d.",
		sc4353->info.if_type, sc4353->info.data_lanes); */

	/* power pin */
	sc4353->pwdn_valid = 1;
	ret = config_get_u32_array(module, "mcsi-pwdn",
				sc4353->pwdn_val, ARRAY_SIZE(sc4353->pwdn_val));
	if (ret != ARRAY_SIZE(sc4353->pwdn_val)) {
		LOG_E("mcsi: power pin config error. ret:%d", ret);
		sc4353->pwdn_valid = 0;
	}
	/* LOG_D("sc4353 config power"); */

	/* reset pin */
	sc4353->rst_valid = 1;
	ret = config_get_u32_array(module, "mcsi-rst",
				sc4353->rst_val, ARRAY_SIZE(sc4353->rst_val));
	if (ret != ARRAY_SIZE(sc4353->rst_val)) {
		LOG_E("mcsi: reset pin config error. ret:%d", ret);
		sc4353->rst_valid = 0;
	}
	/* LOG_D("sc4353 config reset"); */

	/* mclk pin */
	sc4353->mclk_valid = 1;
	ret = config_get_u32_array(module, "mcsi-mclk",
				sc4353->mclk_val, ARRAY_SIZE(sc4353->mclk_val));
	if (ret != ARRAY_SIZE(sc4353->mclk_val)) {
		LOG_E("mcsi: mclk pin config error. ret:%d", ret);
		sc4353->mclk_valid = 0;
	}
/* #endif */
	/* LOG_D("sc4353 config finish"); */

	return RT_EOK;
}

static rt_err_t __sc4353_mipi_prepare(void *hdl)
{
	return __sc4353_mipi_parser_config(hdl);
}

static rt_err_t __sc4353_mipi_init(void *hdl)
{
	rt_int32_t ret = 0;
	rt_uint8_t tmp[2] = {0};
	rt_uint16_t id = 0;
	struct video_dev *sc4353 = (struct video_dev *)hdl;
	char *module = DRV_MCSI_SC4353_PWR_NAME;
	/* LOG_D("__sc4353_mipi_init in."); */

#if 1
	RT_ASSERT(RT_NULL != sc4353);
	sc4353->pctrl = pinctrl_get(DRV_MCSI_SC4353_NAME);
	if (RT_NULL == sc4353->pctrl)
		return -RT_ERROR;
	#ifdef ARCH_LOMBO_N7V1_CDR
	/* Init gpio */
	/* cam_power_enable(); */
	camera_ldo_set(vol_1p85v);
	cam_power_set(module);
	#endif
	if (1 == sc4353->pwdn_valid) {
		sc4353->pwdn_gpio = pinctrl_gpio_request(sc4353->pctrl,
					sc4353->pwdn_val[0], sc4353->pwdn_val[1]);
		if (sc4353->pwdn_gpio >= 0) {
			/* LOG_D("%d %d %d %d %d", sc4353->pwdn_val[2],
				sc4353->pwdn_val[3], sc4353->pwdn_val[4],
				sc4353->pwdn_val[5], sc4353->pwdn_val[6]); */
			pinctrl_gpio_set_function(sc4353->pctrl, sc4353->pwdn_gpio,
						sc4353->pwdn_val[2]);
			pinctrl_gpio_set_drv_level(sc4353->pctrl, sc4353->pwdn_gpio,
						sc4353->pwdn_val[3]);
			pinctrl_gpio_set_pud_mode(sc4353->pctrl, sc4353->pwdn_gpio,
						sc4353->pwdn_val[4]);
			pinctrl_gpio_set_pud_res(sc4353->pctrl, sc4353->pwdn_gpio,
						sc4353->pwdn_val[5]);
			pinctrl_gpio_set_value(sc4353->pctrl, sc4353->pwdn_gpio,
						sc4353->pwdn_val[6]);
		} else
			sc4353->pwdn_valid = 0;
	}

	if (1 == sc4353->rst_valid) {
		sc4353->rst_gpio = pinctrl_gpio_request(sc4353->pctrl,
					sc4353->rst_val[0], sc4353->rst_val[1]);
		if (sc4353->rst_gpio >= 0) {
			/* LOG_D("%d %d %d %d %d", sc4353->rst_val[2],
				sc4353->rst_val[3], sc4353->rst_val[4],
				sc4353->rst_val[5], sc4353->rst_val[6]); */
			pinctrl_gpio_set_function(sc4353->pctrl, sc4353->rst_gpio,
						sc4353->rst_val[2]);
			pinctrl_gpio_set_drv_level(sc4353->pctrl, sc4353->rst_gpio,
						sc4353->rst_val[3]);
			pinctrl_gpio_set_pud_mode(sc4353->pctrl, sc4353->rst_gpio,
						sc4353->rst_val[4]);
			pinctrl_gpio_set_pud_res(sc4353->pctrl, sc4353->rst_gpio,
						sc4353->rst_val[5]);
			pinctrl_gpio_set_value(sc4353->pctrl, sc4353->rst_gpio,
						sc4353->rst_val[6]);
		} else
			sc4353->rst_valid = 0;
	}

	if (1 == sc4353->mclk_valid) {
		sc4353->mclk_gpio = pinctrl_gpio_request(sc4353->pctrl,
					sc4353->mclk_val[0], sc4353->mclk_val[1]);
		if (sc4353->mclk_gpio >= 0) {
			pinctrl_gpio_set_function(sc4353->pctrl, sc4353->mclk_gpio,
						sc4353->mclk_val[2]);
			pinctrl_gpio_set_drv_level(sc4353->pctrl, sc4353->mclk_gpio,
						sc4353->mclk_val[3]);
			pinctrl_gpio_set_pud_mode(sc4353->pctrl, sc4353->mclk_gpio,
						sc4353->mclk_val[4]);
			pinctrl_gpio_set_pud_res(sc4353->pctrl, sc4353->mclk_gpio,
						sc4353->mclk_val[5]);
			pinctrl_gpio_set_value(sc4353->pctrl, sc4353->mclk_gpio,
						sc4353->mclk_val[6]);
		} else
			sc4353->mclk_valid = 0;
	}
	__sc4353_mipi_set_power(sc4353, RT_TRUE);
#endif

	sc4353->i2c_client = rt_zalloc(sizeof(struct viss_i2c_client));
	if (RT_NULL == sc4353->i2c_client)
		return -RT_ENOMEM;
	sc4353->i2c_client->i2c_bus = rt_i2c_bus_device_find(sc4353->info.i2c_bus_name);
	if (RT_NULL == sc4353->i2c_client->i2c_bus) {
		LOG_E("can't find bus dev \"%s\"", sc4353->info.i2c_bus_name);
		goto exit;
	}

	sc4353->i2c_client->i2c_addr = sc4353->info.i2c_addr;
	ret = viss_i2c_read_reg_16bit(sc4353->i2c_client, 0x3107, &tmp[0]);
	LOG_D("add: %x, ret: %d. 0x3107: %x", sc4353->i2c_client->i2c_addr, ret, tmp[0]);
	ret = viss_i2c_read_reg_16bit(sc4353->i2c_client, 0x3108, &tmp[1]);
	LOG_D("add: %x, ret: %d. 0x3108: %x", sc4353->i2c_client->i2c_addr, ret, tmp[1]);

	id = (tmp[0] << 8) | tmp[1];
	if (id != 0xcd01) {
		LOG_E("ID wrong! (0x%x)\n", id);
		goto exit;
	}

	/* __sc4353_mipi_block_write((void *)sc4353,
		(void *)sc4353_reg_list, ARRAY_SIZE(sc4353_reg_list)); */

	return RT_EOK;
exit:
	if (sc4353->pctrl) {
		if (sc4353->rst_valid)
			pinctrl_gpio_free(sc4353->pctrl, sc4353->rst_gpio);
		if (sc4353->pwdn_valid)
			pinctrl_gpio_free(sc4353->pctrl, sc4353->pwdn_gpio);
		if (sc4353->mclk_valid)
			pinctrl_gpio_free(sc4353->pctrl, sc4353->mclk_gpio);
		pinctrl_put(sc4353->pctrl);
		sc4353->pctrl = RT_NULL;
	}
	if (sc4353->i2c_client) {
		rt_free(sc4353->i2c_client);
		sc4353->i2c_client = RT_NULL;
	}
	#ifdef ARCH_LOMBO_N7V1_CDR
	/* cam_power_disable(); */
	cam_power_exit(module);
	camera_ldo_exit();
	#endif
	return -RT_ERROR;
}

static void __sc4353_mipi_exit(void *hdl)
{
	struct video_dev *sc4353 = (struct video_dev *)hdl;
	char *module = DRV_MCSI_SC4353_PWR_NAME;

	__sc4353_mipi_set_power(sc4353, RT_FALSE);
	if (sc4353->pctrl) {
		if (sc4353->rst_valid)
			pinctrl_gpio_free(sc4353->pctrl, sc4353->rst_gpio);
		if (sc4353->pwdn_valid)
			pinctrl_gpio_free(sc4353->pctrl, sc4353->pwdn_gpio);
		if (sc4353->mclk_valid)
			pinctrl_gpio_free(sc4353->pctrl, sc4353->mclk_gpio);
		pinctrl_put(sc4353->pctrl);
		sc4353->pctrl = RT_NULL;
	}
	if (sc4353->i2c_client) {
		rt_free(sc4353->i2c_client);
		sc4353->i2c_client = RT_NULL;
	}
	reg_writed = 0;
	#ifdef ARCH_LOMBO_N7V1_CDR
	cam_power_exit(module);
	camera_ldo_exit();
	/* cam_power_disable(); */
	#endif
}

struct video_dev sc4353_mipi = {
	.name = DRV_MCSI_SC4353_NAME,
	.group_id = GRP_ID_MCSI,
	.prepare = __sc4353_mipi_prepare,
	.init = __sc4353_mipi_init,
	.exit = __sc4353_mipi_exit,
	.s_mode = __sc4353_mipi_set_mode,
	.g_cur_mode = __sc4353_mipi_cur_mode,
	.g_all_mode = __sc4353_mipi_get_all_mode,
	.s_power = __sc4353_mipi_set_power,
	.s_stream = __sc4353_mipi_set_stream,
	.g_info = __sc4353_mipi_get_info,
	.ioctl = __sc4353_mipi_ioctl,
	.s_register = __sc4353_mipi_set_register,
	.g_register = __sc4353_mipi_get_register,
};

