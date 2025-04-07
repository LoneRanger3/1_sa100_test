/*
 * isp_dev_sc1335t.c - sc1335t driver code for LomboTech
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

#define DBG_SECTION_NAME	"SC1335T-MIPI"
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

#define DRV_MCSI_SC1335T_NAME "sc1335t-mipi"
#define DRV_MCSI_SC1335T_PWR_NAME "cam-pwr-en"

#define SC1335T_MIPI_REG_DELAY_FLAG  (0xffff)
#define MAX_AGAIN (15 * 1024 + 512)
#define MAX_DGAIN (12 * 1024)


/*
 * 2lane YUV init
 */
static const struct sensor_reg sc1335t_reg_list[] = {
	{ 0x0103, 0x01 },
	{ 0x0100, 0x00 },
	{ 0x36e9, 0x80 },
	{ 0x36f9, 0x80 },
	{ 0x301f, 0x12 },
	{ 0x3038, 0x44 },
	{ 0x3200, 0x00 },
	{ 0x3201, 0x00 },
	{ 0x3202, 0x00 },
	{ 0x3203, 0x00 },
	{ 0x3204, 0x05 },
	{ 0x3205, 0x03 },
	{ 0x3206, 0x02 },
	{ 0x3207, 0xd3 },
	{ 0x3208, 0x05 },
	{ 0x3209, 0x00 },
	{ 0x320a, 0x02 },
	{ 0x320b, 0xd0 },
	{ 0x320c, 0x07 },
	{ 0x320d, 0xd0 },
	{ 0x3210, 0x00 },
	{ 0x3211, 0x02 },
	{ 0x3212, 0x00 },
	{ 0x3213, 0x02 },
	{ 0x3253, 0x08 },
	{ 0x3301, 0x05 },
	{ 0x3303, 0x08 },
	{ 0x3304, 0x30 },
	{ 0x3306, 0x3a },
	{ 0x3308, 0x18 },
	{ 0x3309, 0x40 },
	{ 0x330a, 0x00 },
	{ 0x330b, 0xc0 },
	{ 0x330c, 0x10 },
	{ 0x330e, 0x30 },
	{ 0x3316, 0x00 },
	{ 0x331c, 0x01 },
	{ 0x331e, 0x29 },
	{ 0x331f, 0x39 },
	{ 0x3320, 0x05 },
	{ 0x3333, 0x10 },
	{ 0x3364, 0x17 },
	{ 0x3390, 0x04 },
	{ 0x3391, 0x08 },
	{ 0x3392, 0x18 },
	{ 0x3393, 0x07 },
	{ 0x3394, 0x08 },
	{ 0x3395, 0x40 },
	{ 0x3396, 0x48 },
	{ 0x3397, 0x58 },
	{ 0x3398, 0x78 },
	{ 0x3399, 0x06 },
	{ 0x339a, 0x07 },
	{ 0x339b, 0x40 },
	{ 0x339c, 0x40 },
	{ 0x3620, 0xe8 },
	{ 0x3622, 0xe6 },
	{ 0x3630, 0xb0 },
	{ 0x3632, 0x68 },
	{ 0x3633, 0x43 },
	{ 0x3636, 0x77 },
	{ 0x3637, 0x15 },
	{ 0x3638, 0x08 },
	{ 0x363a, 0x1f },
	{ 0x363b, 0xd6 },
	{ 0x363c, 0x04 },
	{ 0x3670, 0x0c },
	{ 0x3677, 0x84 },
	{ 0x3678, 0x88 },
	{ 0x3679, 0x8a },
	{ 0x367e, 0x48 },
	{ 0x367f, 0x58 },
	{ 0x3690, 0x53 },
	{ 0x3691, 0x53 },
	{ 0x3692, 0x53 },
	{ 0x369c, 0x48 },
	{ 0x369d, 0x58 },
	{ 0x36e9, 0x20 },
	{ 0x36ea, 0x31 },
	{ 0x36eb, 0x0d },
	{ 0x36ec, 0x2c },
	{ 0x36f9, 0x20 },
	{ 0x36fa, 0x31 },
	{ 0x36fb, 0x00 },
	{ 0x36fc, 0x10 },
	{ 0x36fd, 0x17 },
	{ 0x3907, 0x00 },
	{ 0x3908, 0x82 },
	{ 0x391f, 0x00 },
	{ 0x3933, 0x44 },
	{ 0x3934, 0x4d },
	{ 0x3940, 0x79 },
	{ 0x3942, 0x04 },
	{ 0x3943, 0x4f },
	{ 0x3e01, 0x5d },
	{ 0x3e02, 0x40 },
	{ 0x3f00, 0x0d },
	{ 0x3f04, 0x03 },
	{ 0x3f05, 0x21 },
	{ 0x450a, 0x71 },
	{ 0x5787, 0x10 },
	{ 0x5788, 0x06 },
	{ 0x578a, 0x10 },
	{ 0x578b, 0x06 },
	{ 0x5790, 0x10 },
	{ 0x5791, 0x10 },
	{ 0x5792, 0x00 },
	{ 0x5793, 0x10 },
	{ 0x5794, 0x10 },
	{ 0x5795, 0x00 },
	{ 0x5799, 0x00 },
	{ 0x57c7, 0x10 },
	{ 0x57c8, 0x06 },
	{ 0x57ca, 0x10 },
	{ 0x57cb, 0x06 },
	{ 0x57d0, 0x10 },
	{ 0x57d1, 0x10 },
	{ 0x57d2, 0x00 },
	{ 0x57d3, 0x10 },
	{ 0x57d4, 0x10 },
	{ 0x57d5, 0x00 },
	{ 0x57d9, 0x00 },
	{ 0x36e9, 0x20 },
	{ 0x36f9, 0x20 },/* pll */
	/* { 0x0100, 0x01 }, */
};

struct dev_mode *cur_sc1335t_mipi_mode = RT_NULL;
static struct dev_mode sc1335t_mipi_mode[] = {
	{
		.index = 0,
		.inp_fmt = VISS_MBUS_FMT_YUYV8_2X8,
		.out_fmt = VISS_PIX_FMT_NV12,
		.colorspace = DEV_COLORSPACE_JPEG,
		.frame_rate = 30000,   /* 30fps */
		.frame_size.width = 1280,
		.frame_size.height = 720,
		.usr_data = (void *)sc1335t_reg_list,
		.usr_data_size = ARRAY_SIZE(sc1335t_reg_list),
	},
};

static rt_err_t __sc1335t_mipi_wake_up(void *hdl)
{
	/* struct video_dev *sc1335t = (struct video_dev *)hdl; */
	return 0;
}

static rt_err_t __sc1335t_mipi_block_write(void *hdl,
						void *data, rt_int32_t size)
{
	struct video_dev *sc1335t = (struct video_dev *)hdl;
	struct sensor_reg *reg = (struct sensor_reg *)data;
	rt_int32_t i = 0;
	rt_err_t ret = 0;
	rt_int32_t delay = 0, temp = 0;

	for (i = 0; i < size; i++) {
		if (SC1335T_MIPI_REG_DELAY_FLAG == reg[i].reg_add) {
			temp = reg[i].reg_value + 9;
			do_div(temp, 10);
			delay = temp * 10;
			LOG_D("delay: %d", delay);
			do_div(delay, 10);
			rt_thread_delay(delay);
			continue;
		}
		ret = viss_i2c_write_reg_16bit(sc1335t->i2c_client,
					reg[i].reg_add, reg[i].reg_value);
		if (RT_EOK != ret) {
			LOG_E("i2c write %x value:%x fail.\n",
				reg[i].reg_add, reg[i].reg_value);
			return ret;
		}
	}

	return RT_EOK;
}

static struct dev_mode *__sc1335t_mipi_cur_mode(void *hdl)
{
	struct video_dev *sc1335t = (struct video_dev *)hdl;
	return sc1335t->cur_mode;
}

static struct dev_mode *__sc1335t_mipi_get_all_mode(void *hdl, rt_int32_t *num)
{
	RT_ASSERT(RT_NULL != num);
	*num = (rt_int32_t)ARRAY_SIZE(sc1335t_mipi_mode);

	return sc1335t_mipi_mode;
}
int reg_writed;
static rt_err_t __sc1335t_mipi_set_mode(void *hdl, rt_int32_t index)
{
	struct video_dev *sc1335t = (struct video_dev *)hdl;
	rt_int32_t num = 0;
	rt_err_t ret = 0;

	num = (rt_int32_t)ARRAY_SIZE(sc1335t_mipi_mode);
	if (index >= num) {
		LOG_E("Input mode index error. num:%d index:%d\n", num, index);
		return -RT_EINVAL;
	}
	if (index >= ARRAY_SIZE(sc1335t_mipi_mode)) {
		LOG_E("Input para error. index:%d\n", index);
		return -RT_EINVAL;
	}
	sc1335t->cur_mode = &sc1335t_mipi_mode[index];
	if (reg_writed)
		return RT_EOK;

	reg_writed = 1;
	ret = __sc1335t_mipi_block_write(hdl, sc1335t->cur_mode->usr_data,
				sc1335t->cur_mode->usr_data_size);
	if (RT_EOK != ret)
		return ret;

	return __sc1335t_mipi_wake_up(hdl);
}

static rt_err_t __sc1335t_set_shutter(struct video_dev *sensor, rt_uint32_t shutter)
{
	struct video_dev *sc1335t = (struct video_dev *)sensor;

	viss_i2c_write_reg_16bit(sc1335t->i2c_client, 0x3e00,
		(shutter >> 12) & 0x0f);
	viss_i2c_write_reg_16bit(sc1335t->i2c_client, 0x3e01,
		(shutter >> 4) & 0xff);
	viss_i2c_write_reg_16bit(sc1335t->i2c_client, 0x3e02,
		(shutter << 4) & 0xf0);

	return RT_EOK;
}

static rt_err_t __sc1335t_set_gain(void *hdl, rt_uint32_t gain)
{
	u32 again_fine, dgain_fine;
	u32 again = 0, dgain = 0;
	struct video_dev *sc1335t = (struct video_dev *)hdl;

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

	viss_i2c_write_reg_16bit(sc1335t->i2c_client, 0x3e09, again_fine);
	viss_i2c_write_reg_16bit(sc1335t->i2c_client, 0x3e08, again);
	viss_i2c_write_reg_16bit(sc1335t->i2c_client, 0x3e07, dgain_fine);
	viss_i2c_write_reg_16bit(sc1335t->i2c_client, 0x3e06, dgain);

	return RT_EOK;
}

static rt_err_t __sc1335t_set_logic(void *hdl, u32 gain)
{
	struct video_dev *sc1335t = (struct video_dev *)hdl;

	if (gain < 2*1024) {
		viss_i2c_write_reg_16bit(sc1335t->i2c_client, 0x3632, 0x18);
		viss_i2c_write_reg_16bit(sc1335t->i2c_client, 0x3631, 0x88);
		viss_i2c_write_reg_16bit(sc1335t->i2c_client, 0x3636, 0x25);
		return;
	}

	if (gain < 4*1024) {
		viss_i2c_write_reg_16bit(sc1335t->i2c_client, 0x3632, 0x18);
		viss_i2c_write_reg_16bit(sc1335t->i2c_client, 0x3631, 0x8e);
		viss_i2c_write_reg_16bit(sc1335t->i2c_client, 0x3636, 0x25);
		return;
	}

	if (gain < 8*1024) {
		viss_i2c_write_reg_16bit(sc1335t->i2c_client, 0x3632, 0x18);
		viss_i2c_write_reg_16bit(sc1335t->i2c_client, 0x3631, 0x80);
		viss_i2c_write_reg_16bit(sc1335t->i2c_client, 0x3636, 0x65);
		return;
	}

	if (gain < 15.75*1024) {
		viss_i2c_write_reg_16bit(sc1335t->i2c_client, 0x3632, 0x18);
		viss_i2c_write_reg_16bit(sc1335t->i2c_client, 0x3631, 0x80);
		viss_i2c_write_reg_16bit(sc1335t->i2c_client, 0x3636, 0x65);
		return;
	}

	viss_i2c_write_reg_16bit(sc1335t->i2c_client, 0x3632, 0xd8);
	viss_i2c_write_reg_16bit(sc1335t->i2c_client, 0x3631, 0x80);
	viss_i2c_write_reg_16bit(sc1335t->i2c_client, 0x3636, 0x65);

	return RT_EOK;
}

static rt_err_t __sc1335t_exp_ctrl(void *hdl, struct isp_exp_gain *exp_gain)
{
	struct video_dev *sc1335t = (struct video_dev *)hdl;
	rt_err_t ret = 0;

	if ((RT_NULL == hdl) || (RT_NULL == exp_gain)) {
		LOG_E("set sc2363_exp_ctrl fail.");
		return -RT_ERROR;
	}

	LOG_D("**********exp_ctrl: %d, %d.", exp_gain->gain, exp_gain->exp);

	ret = __sc1335t_set_gain(sc1335t, exp_gain->gain * 4);
	if (RT_EOK != ret)
		LOG_E("sc1335t set gain fail.");

	ret = __sc1335t_set_shutter(sc1335t, exp_gain->exp / 8);
	if (RT_EOK != ret)
		LOG_E("sc1335t set shutter fail.");

	viss_i2c_write_reg_16bit(sc1335t->i2c_client, 0x3812, 0x00);

	ret = __sc1335t_set_logic(sc1335t, exp_gain->gain * 4);
	if (RT_EOK != ret)
		LOG_E("sc1335t set logic fail.");
	viss_i2c_write_reg_16bit(sc1335t->i2c_client, 0x3812, 0x30);

	return ret;
}

static rt_err_t __sc1335t_isp_get_sensor_info(void *hdl,
	struct isp_sensor_info *isp_sensor_info)
{
	isp_sensor_info->sensor_name = "sc1335t";
	isp_sensor_info->pclk = 45 * 1000 * 1000;
	isp_sensor_info->vts = 750;
	isp_sensor_info->hts = 2000;
	isp_sensor_info->input_width = 1280;
	isp_sensor_info->input_height = 720;
	isp_sensor_info->output_widht = 1280;
	isp_sensor_info->output_height = 720;
	isp_sensor_info->bayer_mode = ISP_BPAT_BGBGGRGR;

	return RT_EOK;
}
static rt_err_t __sc1335t_mipi_set_register(void *hdl,
					struct viss_dbg_register *reg)
{
	struct video_dev *sc1335t = (struct video_dev *)hdl;
	rt_err_t ret = 0;

	ret = viss_i2c_write_reg_16bit(sc1335t->i2c_client, reg->add, reg->val);

	return ret;
}

static rt_err_t __sc1335t_mipi_get_register(void *hdl,
					struct viss_dbg_register *reg)
{
	struct video_dev *sc1335t = (struct video_dev *)hdl;
	rt_uint8_t value;
	rt_err_t ret = 0;

	ret = viss_i2c_read_reg_16bit(sc1335t->i2c_client, reg->add, &value);
	reg->val = value;

	return ret;
}

static rt_err_t __sc1335t_mipi_set_power(void *hdl, rt_bool_t on)
{
	struct video_dev *sc1335t = (struct video_dev *)hdl;
	/* LOG_D("__sc2363_mipi_set_power"); */

	if ((1 != sc1335t->pwdn_valid) || (1 != sc1335t->rst_valid))
		return RT_EOK;

	if (RT_TRUE == on) {
		/* LOG_D("__sc2363_mipi_set_power 1111"); */
		pinctrl_gpio_set_value(sc1335t->pctrl, sc1335t->pwdn_gpio, 0);
		rt_thread_delay(1); /* 10ms */
		pinctrl_gpio_set_value(sc1335t->pctrl, sc1335t->rst_gpio, 0);
		rt_thread_delay(1); /* 10ms */
		pinctrl_gpio_set_value(sc1335t->pctrl, sc1335t->rst_gpio, 1);
		rt_thread_delay(1); /* 10ms */
		pinctrl_gpio_set_value(sc1335t->pctrl, sc1335t->pwdn_gpio, 1);
	} else {
		/* TO DO */
	}

	return RT_EOK;
}

static rt_err_t __sc1335t_mipi_set_stream(void *hdl, rt_bool_t enable)
{
#if 1
	rt_int32_t ret = 0;
	struct video_dev *sc1335t = (struct video_dev *)hdl;
	if (enable)
		ret = viss_i2c_write_reg_16bit(sc1335t->i2c_client, 0x100, 1);
	else
		ret = viss_i2c_write_reg_16bit(sc1335t->i2c_client, 0x100, 0);
#endif
	return ret;
}

rt_err_t __sc1335t_mipi_ioctl(void *hdl, rt_int32_t cmd, void *para)
{
	rt_err_t ret = 0;
	struct isp_exp_gain *sc1335t_exp = RT_NULL;
	struct isp_sensor_info *sc1335t_isp_sensor_info = RT_NULL;

	switch (cmd) {
	case ISP_GET_SENSOR_INFO:
		sc1335t_isp_sensor_info = (struct isp_sensor_info *)para;
		__sc1335t_isp_get_sensor_info(hdl, sc1335t_isp_sensor_info);
		break;
	case ISP_SET_EXP_GAIN:
		sc1335t_exp = (struct isp_exp_gain *)para;
		__sc1335t_exp_ctrl(hdl, sc1335t_exp);
		/* LOG_FLOAT("ISP_SET_EXP_GAIN: exp: %d, gain: %f.",
			sc1335t_exp->exp, sc1335t_exp->gain); */
		break;
	default:
		return -RT_ERROR;
	}
	return ret;
}

static rt_err_t __sc1335t_mipi_get_info(void *hdl,
			struct viss_source_info *info)
{
	struct video_dev *sc1335t = (struct video_dev *)hdl;
	/* LOG_D("__sc1335t_mipi_get_info in"); */

	if ((RT_NULL == hdl) || (RT_NULL == info)) {
		LOG_E("Get sc1335t information fail.");
		return -RT_ERROR;
	}
	rt_memcpy(info, &sc1335t->info, sizeof(struct viss_source_info));
	/* LOG_D("__sc2363_mipi_get_info out"); */

	return RT_EOK;
}

static rt_err_t __sc1335t_mipi_parser_config(void *hdl)
{
	struct video_dev *sc1335t = (struct video_dev *)hdl;
	struct viss_source_info info;
	rt_uint32_t tmp_data = 0;
	const char *status;
	const char *i2c_bus_name;
	const char *module = DRV_MCSI_SC1335T_NAME;
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
		LOG_W("sc1335t mipi disable.");
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
	rt_memcpy(&sc1335t->info, &info, sizeof(struct viss_source_info));
	/* LOG_D("sc1335t->info.if_type:%d, lane_num: %d.",
		sc1335t->info.if_type, sc1335t->info.data_lanes); */

	/* power pin */
	sc1335t->pwdn_valid = 1;
	ret = config_get_u32_array(module, "mcsi-pwdn",
				sc1335t->pwdn_val, ARRAY_SIZE(sc1335t->pwdn_val));
	if (ret != ARRAY_SIZE(sc1335t->pwdn_val)) {
		LOG_E("mcsi: power pin config error. ret:%d", ret);
		sc1335t->pwdn_valid = 0;
	}
	/* LOG_D("sc1335t config power"); */

	/* reset pin */
	sc1335t->rst_valid = 1;
	ret = config_get_u32_array(module, "mcsi-rst",
				sc1335t->rst_val, ARRAY_SIZE(sc1335t->rst_val));
	if (ret != ARRAY_SIZE(sc1335t->rst_val)) {
		LOG_E("mcsi: reset pin config error. ret:%d", ret);
		sc1335t->rst_valid = 0;
	}
	/* LOG_D("sc1335t config reset"); */

	/* mclk pin */
	sc1335t->mclk_valid = 1;
	ret = config_get_u32_array(module, "mcsi-mclk",
				sc1335t->mclk_val, ARRAY_SIZE(sc1335t->mclk_val));
	if (ret != ARRAY_SIZE(sc1335t->mclk_val)) {
		LOG_E("mcsi: mclk pin config error. ret:%d", ret);
		sc1335t->mclk_valid = 0;
	}
/* #endif */
	/* LOG_D("sc1335t config finish"); */

	return RT_EOK;
}

static rt_err_t __sc1335t_mipi_prepare(void *hdl)
{
	return __sc1335t_mipi_parser_config(hdl);
}

static rt_err_t __sc1335t_mipi_init(void *hdl)
{
	rt_int32_t ret = 0;
	rt_uint8_t tmp[2] = {0};
	rt_uint16_t id = 0;
	struct video_dev *sc1335t = (struct video_dev *)hdl;
	char *module = DRV_MCSI_SC1335T_PWR_NAME;
	/* LOG_D("__sc1335t_mipi_init in."); */

#if 1
	RT_ASSERT(RT_NULL != sc1335t);
	sc1335t->pctrl = pinctrl_get(DRV_MCSI_SC1335T_NAME);
	if (RT_NULL == sc1335t->pctrl)
		return -RT_ERROR;
	#ifdef ARCH_LOMBO_N7V1_CDR
	/* Init gpio */
	/* cam_power_enable(); */
	camera_ldo_set(vol_1p85v);
	cam_power_set(module);
	#endif
	if (1 == sc1335t->pwdn_valid) {
		sc1335t->pwdn_gpio = pinctrl_gpio_request(sc1335t->pctrl,
					sc1335t->pwdn_val[0], sc1335t->pwdn_val[1]);
		if (sc1335t->pwdn_gpio >= 0) {
			pinctrl_gpio_set_function(sc1335t->pctrl, sc1335t->pwdn_gpio,
						sc1335t->pwdn_val[2]);
			pinctrl_gpio_set_drv_level(sc1335t->pctrl, sc1335t->pwdn_gpio,
						sc1335t->pwdn_val[3]);
			pinctrl_gpio_set_pud_mode(sc1335t->pctrl, sc1335t->pwdn_gpio,
						sc1335t->pwdn_val[4]);
			pinctrl_gpio_set_pud_res(sc1335t->pctrl, sc1335t->pwdn_gpio,
						sc1335t->pwdn_val[5]);
			pinctrl_gpio_set_value(sc1335t->pctrl, sc1335t->pwdn_gpio,
						sc1335t->pwdn_val[6]);
		} else
			sc1335t->pwdn_valid = 0;
	}

	if (1 == sc1335t->rst_valid) {
		sc1335t->rst_gpio = pinctrl_gpio_request(sc1335t->pctrl,
					sc1335t->rst_val[0], sc1335t->rst_val[1]);
		if (sc1335t->rst_gpio >= 0) {
			pinctrl_gpio_set_function(sc1335t->pctrl, sc1335t->rst_gpio,
						sc1335t->rst_val[2]);
			pinctrl_gpio_set_drv_level(sc1335t->pctrl, sc1335t->rst_gpio,
						sc1335t->rst_val[3]);
			pinctrl_gpio_set_pud_mode(sc1335t->pctrl, sc1335t->rst_gpio,
						sc1335t->rst_val[4]);
			pinctrl_gpio_set_pud_res(sc1335t->pctrl, sc1335t->rst_gpio,
						sc1335t->rst_val[5]);
			pinctrl_gpio_set_value(sc1335t->pctrl, sc1335t->rst_gpio,
						sc1335t->rst_val[6]);
		} else
			sc1335t->rst_valid = 0;
	}

	if (1 == sc1335t->mclk_valid) {
		sc1335t->mclk_gpio = pinctrl_gpio_request(sc1335t->pctrl,
					sc1335t->mclk_val[0], sc1335t->mclk_val[1]);
		if (sc1335t->mclk_gpio >= 0) {
			pinctrl_gpio_set_function(sc1335t->pctrl, sc1335t->mclk_gpio,
						sc1335t->mclk_val[2]);
			pinctrl_gpio_set_drv_level(sc1335t->pctrl, sc1335t->mclk_gpio,
						sc1335t->mclk_val[3]);
			pinctrl_gpio_set_pud_mode(sc1335t->pctrl, sc1335t->mclk_gpio,
						sc1335t->mclk_val[4]);
			pinctrl_gpio_set_pud_res(sc1335t->pctrl, sc1335t->mclk_gpio,
						sc1335t->mclk_val[5]);
			pinctrl_gpio_set_value(sc1335t->pctrl, sc1335t->mclk_gpio,
						sc1335t->mclk_val[6]);
		} else
			sc1335t->mclk_valid = 0;
	}
	__sc1335t_mipi_set_power(sc1335t, RT_TRUE);
#endif

	sc1335t->i2c_client = rt_zalloc(sizeof(struct viss_i2c_client));
	if (RT_NULL == sc1335t->i2c_client)
		return -RT_ENOMEM;
	sc1335t->i2c_client->i2c_bus = rt_i2c_bus_device_find(sc1335t->info.i2c_bus_name);
	if (RT_NULL == sc1335t->i2c_client->i2c_bus) {
		LOG_E("can't find bus dev \"%s\"", sc1335t->info.i2c_bus_name);
		goto exit;
	}

	sc1335t->i2c_client->i2c_addr = sc1335t->info.i2c_addr;
	ret = viss_i2c_read_reg_16bit(sc1335t->i2c_client, 0x3107, &tmp[0]);
	LOG_D("add: %x, ret: %d. 0x3107: %x", sc1335t->i2c_client->i2c_addr, ret, tmp[0]);
	ret = viss_i2c_read_reg_16bit(sc1335t->i2c_client, 0x3108, &tmp[1]);
	LOG_D("add: %x, ret: %d. 0x3108: %x", sc1335t->i2c_client->i2c_addr, ret, tmp[1]);

	id = (tmp[0] << 8) | tmp[1];
	if (id != 0xca13) {
		LOG_E("ID wrong! (0x%x)\n", id);
		goto exit;
	}

	return RT_EOK;
exit:
	if (sc1335t->pctrl) {
		if (sc1335t->rst_valid)
			pinctrl_gpio_free(sc1335t->pctrl, sc1335t->rst_gpio);
		if (sc1335t->pwdn_valid)
			pinctrl_gpio_free(sc1335t->pctrl, sc1335t->pwdn_gpio);
		if (sc1335t->mclk_valid)
			pinctrl_gpio_free(sc1335t->pctrl, sc1335t->mclk_gpio);
		pinctrl_put(sc1335t->pctrl);
		sc1335t->pctrl = RT_NULL;
	}
	if (sc1335t->i2c_client) {
		rt_free(sc1335t->i2c_client);
		sc1335t->i2c_client = RT_NULL;
	}
	#ifdef ARCH_LOMBO_N7V1_CDR
	/* cam_power_disable(); */
	cam_power_exit(module);
	camera_ldo_exit();
	#endif
	return -RT_ERROR;
}

static void __sc1335t_mipi_exit(void *hdl)
{
	struct video_dev *sc1335t = (struct video_dev *)hdl;
	char *module = DRV_MCSI_SC1335T_PWR_NAME;

	__sc1335t_mipi_set_power(sc1335t, RT_FALSE);
	if (sc1335t->pctrl) {
		if (sc1335t->rst_valid)
			pinctrl_gpio_free(sc1335t->pctrl, sc1335t->rst_gpio);
		if (sc1335t->pwdn_valid)
			pinctrl_gpio_free(sc1335t->pctrl, sc1335t->pwdn_gpio);
		if (sc1335t->mclk_valid)
			pinctrl_gpio_free(sc1335t->pctrl, sc1335t->mclk_gpio);
		pinctrl_put(sc1335t->pctrl);
		sc1335t->pctrl = RT_NULL;
	}
	if (sc1335t->i2c_client) {
		rt_free(sc1335t->i2c_client);
		sc1335t->i2c_client = RT_NULL;
	}
	reg_writed = 0;
	#ifdef ARCH_LOMBO_N7V1_CDR
	cam_power_exit(module);
	camera_ldo_exit();
	/* cam_power_disable(); */
	#endif
}

struct video_dev sc1335t_mipi = {
	.name = DRV_MCSI_SC1335T_NAME,
	.group_id = GRP_ID_MCSI,
	.prepare = __sc1335t_mipi_prepare,
	.init = __sc1335t_mipi_init,
	.exit = __sc1335t_mipi_exit,
	.s_mode = __sc1335t_mipi_set_mode,
	.g_cur_mode = __sc1335t_mipi_cur_mode,
	.g_all_mode = __sc1335t_mipi_get_all_mode,
	.s_power = __sc1335t_mipi_set_power,
	.s_stream = __sc1335t_mipi_set_stream,
	.g_info = __sc1335t_mipi_get_info,
	.ioctl = __sc1335t_mipi_ioctl,
	.s_register = __sc1335t_mipi_set_register,
	.g_register = __sc1335t_mipi_get_register,
};

