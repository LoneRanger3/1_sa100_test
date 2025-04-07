/*
 * vic_dev_ov5640.c - ov5640 driver code for LomboTech
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

#define DBG_SECTION_NAME	"OV5640-MIPI"
#define DBG_LEVEL		DBG_LOG

#include <debug.h>
#include "viss.h"
#include "mcsi_dev.h"
#include "viss_i2c.h"
#include <div.h>

struct sensor_reg {
	rt_uint16_t reg_add;
	rt_uint16_t reg_value;
};

#define DRV_MCSI_0V5640_NAME "ov5640-mipi"
#define OV5640_MIPI_REG_DELAY_FLAG  (0xffff)
/*
 * 2lane YUV init
 */
static const struct sensor_reg ov5640_reg_list[] = {
	{ 0x3103, 0x11 }, /* SCCB system control */
	{ 0x3008, 0x82 }, /* software reset */
	{ OV5640_MIPI_REG_DELAY_FLAG, 0x5 }, /* mdelay(5); */
	{ 0x3008, 0x42 }, /* software power down */
	{ 0x3103, 0x03 }, /* SCCB system control */
	{ 0x3017, 0x00 }, /* set Frex, Vsync, Href, PCLK, D[9:6] input */
	{ 0x3018, 0x00 }, /* set d[5:0], GPIO[1:0] input */
	{ 0x3034, 0x18 }, /* MIPI 8-bit mode */
	{ 0x3037, 0x13 }, /* PLL */
	{ 0x3108, 0x01 }, /* system divider */
	{ 0x3630, 0x36 },
	{ 0x3631, 0x0e },
	{ 0x3632, 0xe2 },
	{ 0x3633, 0x12 },
	{ 0x3621, 0xe0 },
	{ 0x3704, 0xa0 },
	{ 0x3703, 0x5a },
	{ 0x3715, 0x78 },
	{ 0x3717, 0x01 },
	{ 0x370b, 0x60 },
	{ 0x3705, 0x1a },
	{ 0x3905, 0x02 },
	{ 0x3906, 0x10 },
	{ 0x3901, 0x0a },
	{ 0x3731, 0x12 },
	{ 0x3600, 0x08 }, /* VCM debug mode */
	{ 0x3601, 0x33 }, /* VCM debug mode */
	{ 0x302d, 0x60 }, /* system control */
	{ 0x3620, 0x52 },
	{ 0x371b, 0x20 },
	{ 0x471c, 0x50 },
	{ 0x3a13, 0x43 }, /* AGC pre-gain, 0x40 = 1x */
	{ 0x3a18, 0x00 }, /* gain ceiling */
	{ 0x3a19, 0xf8 }, /* gain ceiling */
	{ 0x3635, 0x13 },
	{ 0x3636, 0x03 },
	{ 0x3634, 0x40 },
	{ 0x3622, 0x01 },
	/* 50Hz/60Hz */
	{ 0x3c01, 0x34 }, /* 50/ 60Hz */
	{ 0x3c04, 0x28 }, /* threshold for low sum */
	{ 0x3c05, 0x98 }, /* threshold for high sum */
	{ 0x3c06, 0x00 }, /* light meter 1 threshold high */
	{ 0x3c08, 0x00 }, /* light meter 2 threshold high */
	{ 0x3c09, 0x1c }, /* light meter 2 threshold low */
	{ 0x3c0a, 0x9c }, /* sample number high */
	{ 0x3c0b, 0x40 }, /* sample number low */
	/* timing */
	{ 0x3800, 0x00 }, /* HS */
	{ 0x3801, 0x00 }, /* HS */
	{ 0x3802, 0x00 }, /* VS */
	{ 0x3804, 0x0a }, /* HW */
	{ 0x3805, 0x3f }, /* HW */
	{ 0x3810, 0x00 }, /* H offset high */
	{ 0x3811, 0x10 }, /* H offset low */
	{ 0x3812, 0x00 }, /* V offset high */
	{ 0x3708, 0x64 },
	{ 0x3a08, 0x01 }, /* B50 */
	{ 0x4001, 0x02 }, /* BLC start line */
	{ 0x4005, 0x1a }, /* BLC always update */
	{ 0x3000, 0x00 }, /* system reset 0 */
	{ 0x3002, 0x1c }, /* system reset 2 */
	{ 0x3004, 0xff }, /* clock enable 00 */
	{ 0x3006, 0xc3 }, /* clock enable 2 */
	{ 0x300e, 0x45 }, /* MIPI control, 2 lane, MIPI enable */
	{ 0x302e, 0x08 },

	{ 0x4300, 0x30 }, /* YUV 422, YUYV(datasheet wrong£¬is VYUY) */
	{ 0x501f, 0x00 }, /* ISP YUV 422 */

	{ 0x4407, 0x04 }, /* JPEG QS */
	{ 0x440e, 0x00 },

	/* clock lane gating enable 2017-11-28 */
	{ 0x4800, 0x24 },

	{ 0x5000, 0xa7 }, /* ISP control, Lenc on, gamma on,
			   * BPC on, WPC on, CIP on */
	/* AWB */
	{ 0x5180, 0xff },
	{ 0x5181, 0xf2 },
	{ 0x5182, 0x00 },
	{ 0x5183, 0x14 },
	{ 0x5184, 0x25 },
	{ 0x5185, 0x24 },
	{ 0x5186, 0x09 },
	{ 0x5187, 0x09 },
	{ 0x5188, 0x09 },
	{ 0x5189, 0x75 },
	{ 0x518a, 0x54 },
	{ 0x518b, 0xe0 },
	{ 0x518c, 0xb2 },
	{ 0x518d, 0x42 },
	{ 0x518e, 0x3d },
	{ 0x518f, 0x56 },
	{ 0x5190, 0x46 },
	{ 0x5191, 0xf8 },
	{ 0x5192, 0x04 },
	{ 0x5193, 0x70 },
	{ 0x5194, 0xf0 },
	{ 0x5195, 0xf0 },
	{ 0x5196, 0x03 },
	{ 0x5197, 0x01 },
	{ 0x5198, 0x04 },
	{ 0x5199, 0x12 },
	{ 0x519a, 0x04 },
	{ 0x519b, 0x00 },
	{ 0x519c, 0x06 },
	{ 0x519d, 0x82 },
	{ 0x519e, 0x38 },
	/* color matrix */
	{ 0x5381, 0x1e },
	{ 0x5382, 0x5b },
	{ 0x5383, 0x08 },
	{ 0x5384, 0x0a },
	{ 0x5385, 0x7e },
	{ 0x5386, 0x88 },
	{ 0x5387, 0x7c },
	{ 0x5388, 0x6c },
	{ 0x5389, 0x10 },
	{ 0x538a, 0x01 },
	{ 0x538b, 0x98 },
	/* CIP */
	{ 0x5300, 0x08 }, /* sharpen MT th1 */
	{ 0x5301, 0x30 }, /* sharpen MT th2 */
	{ 0x5302, 0x10 }, /* sharpen MT offset 1 */
	{ 0x5303, 0x00 }, /* sharpen MT offset 2 */
	{ 0x5304, 0x08 }, /* DNS threshold 1 */
	{ 0x5305, 0x30 }, /* DNS threshold 2 */
	{ 0x5306, 0x08 }, /* DNS offset 1 */
	{ 0x5307, 0x16 }, /* DNS offset 2 */
	{ 0x5309, 0x08 }, /* sharpen TH th1 */
	{ 0x530a, 0x30 }, /* sharpen TH th2 */
	{ 0x530b, 0x04 }, /* sharpen TH offset 1 */
	{ 0x530c, 0x06 }, /* sharpen Th offset 2 */
	/* gamma */
	{ 0x5480, 0x01 },
	{ 0x5481, 0x08 },
	{ 0x5482, 0x14 },
	{ 0x5483, 0x28 },
	{ 0x5484, 0x51 },
	{ 0x5485, 0x65 },
	{ 0x5486, 0x71 },
	{ 0x5487, 0x7d },
	{ 0x5488, 0x87 },
	{ 0x5489, 0x91 },
	{ 0x548a, 0x9a },
	{ 0x548b, 0xaa },
	{ 0x548c, 0xb8 },
	{ 0x548d, 0xcd },
	{ 0x548e, 0xdd },
	{ 0x548f, 0xea },
	{ 0x5490, 0x1d },
	/* UV adjust */
	{ 0x5580, 0x06 }, /* sat on, contrast on */
	{ 0x5583, 0x40 }, /* sat U */
	{ 0x5584, 0x10 }, /* sat V */
	{ 0x5589, 0x10 }, /* UV adjust th1 */
	{ 0x558a, 0x00 }, /* UV adjust th2[8] */
	{ 0x558b, 0xf8 }, /* UV adjust th2[7:0] */
	{ 0x501d, 0x04 }, /* enable manual offset of contrast */
	/* lens correction */
	{ 0x5800, 0x23 },
	{ 0x5801, 0x14 },
	{ 0x5802, 0x0f },
	{ 0x5803, 0x0f },
	{ 0x5804, 0x12 },
	{ 0x5805, 0x26 },
	{ 0x5806, 0x0c },
	{ 0x5807, 0x08 },
	{ 0x5808, 0x05 },
	{ 0x5809, 0x05 },
	{ 0x580a, 0x08 },
	{ 0x580b, 0x0d },
	{ 0x580c, 0x08 },
	{ 0x580d, 0x03 },
	{ 0x580e, 0x00 },
	{ 0x580f, 0x00 },
	{ 0x5810, 0x03 },
	{ 0x5811, 0x09 },
	{ 0x5812, 0x07 },
	{ 0x5813, 0x03 },
	{ 0x5814, 0x00 },
	{ 0x5815, 0x01 },
	{ 0x5816, 0x03 },
	{ 0x5817, 0x08 },
	{ 0x5818, 0x0d },
	{ 0x5819, 0x08 },
	{ 0x581a, 0x05 },
	{ 0x581b, 0x06 },
	{ 0x581c, 0x08 },
	{ 0x581d, 0x0e },
	{ 0x581e, 0x29 },
	{ 0x581f, 0x17 },
	{ 0x5820, 0x11 },
	{ 0x5821, 0x11 },
	{ 0x5822, 0x15 },
	{ 0x5823, 0x28 },
	{ 0x5824, 0x46 },
	{ 0x5825, 0x26 },
	{ 0x5826, 0x08 },
	{ 0x5827, 0x26 },
	{ 0x5828, 0x64 },
	{ 0x5829, 0x26 },
	{ 0x582a, 0x24 },
	{ 0x582b, 0x22 },
	{ 0x582c, 0x24 },
	{ 0x582d, 0x24 },
	{ 0x582e, 0x06 },
	{ 0x582f, 0x22 },
	{ 0x5830, 0x40 },
	{ 0x5831, 0x42 },
	{ 0x5832, 0x24 },
	{ 0x5833, 0x26 },
	{ 0x5834, 0x24 },
	{ 0x5835, 0x22 },
	{ 0x5836, 0x22 },
	{ 0x5837, 0x26 },
	{ 0x5838, 0x44 },
	{ 0x5839, 0x24 },
	{ 0x583a, 0x26 },
	{ 0x583b, 0x28 },
	{ 0x583c, 0x42 },
	{ 0x583d, 0xce },
	{ 0x5025, 0x00 },
	{ 0x3a0f, 0x30 }, /* stable in high */
	{ 0x3a10, 0x28 }, /* stable in low */
	{ 0x3a1b, 0x30 }, /* stable out high */
	{ 0x3a1e, 0x26 }, /* stable out low */
	{ 0x3a11, 0x60 }, /* fast zone high */
	{ 0x3a1f, 0x14 }, /* fast zone low */

	/* { 0x3008, 0x02 }, */
};

/*
 * VGA 30fps
 * Input Clock = 24Mhz bit rate 224Mbps
 */
static const struct sensor_reg ov5640_yuv_vga_30fps[] = {
	{ 0x3035, 0x14 }, /* pll */
	{ 0x3036, 0x38 }, /* pll */
	{ 0x3c07, 0x08 }, /* light meter 1 threshold */
	{ 0x3820, 0x41 }, /* ISP flip off, sensor flip off */
	{ 0x3821, 0x07 }, /* ISP mirror on, sensor mirror on */
	/* timing */
	{ 0x3814, 0x31}, /* X inc */
	{ 0x3815, 0x31}, /* Y inc */

	{ 0x3803, 0x04 }, /* VS */
	{ 0x3806, 0x07 }, /* VH */
	{ 0x3807, 0x9b }, /* VH */
	{ 0x3808, 0x02 }, /* DVPHO */
	{ 0x3809, 0x80 }, /* DVPHO */
	{ 0x380a, 0x01 }, /* DVPVO */
	{ 0x380b, 0xe0 }, /* DVPVO */
	{ 0x380c, 0x07 }, /* HTS */
	{ 0x380d, 0x68 }, /* HTS */
	{ 0x380e, 0x03 }, /* VTS */
	{ 0x380f, 0xd8 }, /* VTS */
	{ 0x3813, 0x06 }, /* V offset */

	{ 0x3618, 0x00 },
	{ 0x3612, 0x29 },
	{ 0x3709, 0x52 },
	{ 0x370c, 0x03 },
	{ 0x3a02, 0x03 }, /* 60Hz max exposure */
	{ 0x3a03, 0xd8 }, /* 60Hz max exposure */
	{ 0x3a09, 0x27 }, /* B50 low */
	{ 0x3a0a, 0x00 }, /* B60 high */
	{ 0x3a0b, 0xf6 }, /* B60 low */
	{ 0x3a0e, 0x03 }, /* B50 max */
	{ 0x3a0d, 0x04 }, /* B60 max */
	{ 0x3a14, 0x03 }, /* 50Hz max exposure */
	{ 0x3a15, 0xd8 }, /* 50Hz max exposure */
	{ 0x4004, 0x02 }, /* BLC line number */
	{ 0x4713, 0x03 }, /* JPEG mode 3 */
	{ 0x460b, 0x35 }, /* debug */
	{ 0x460c, 0x22 }, /* VFIFO, PCLK manual */
	{ 0x4837, 0x44 }, /* MIPI global timing */
	{ 0x3824, 0x02 }, /* PCLK divider */
	{ 0x5001, 0xa3 }, /* SDE on, scale on, UV average off, CMX on, AWB on */
};

/*
 * 720p 60fps
 * Input Clock = 24Mhz bit rate 672Mbps
 */
static const struct sensor_reg ov5640_yuv_720p_30fps[] = {
	{ 0x3035, 0x21 }, /* 0x11:60fps, 0x21:30fps */
	{ 0x3036, 0x54 }, /* pll */
	{ 0x3c07, 0x07 }, /* light meter 1 threshold */
	{ 0x3820, 0x41 }, /* ISP flip off, sensor flip off */
	{ 0x3821, 0x07 }, /* ISP mirror on, sensor mirror on */
	/* timing */
	{ 0x3814, 0x31 }, /* X inc */
	{ 0x3815, 0x31 }, /* Y inc */
	{ 0x3803, 0xfa }, /* VS */
	{ 0x3806, 0x06 }, /* VH */
	{ 0x3807, 0xa9 }, /* VH */
	{ 0x3808, 0x05 }, /* DVPHO */
	{ 0x3809, 0x00 }, /* DVPHO */
	{ 0x380a, 0x02 }, /* DVPVO */
	{ 0x380b, 0xd0 }, /* DVPVO */
	{ 0x380c, 0x07 }, /* HTS */
	{ 0x380d, 0x64 }, /* HTS */
	{ 0x380e, 0x02 }, /* VTS */
	{ 0x380f, 0xe4 }, /* VTS */
	{ 0x3813, 0x04 }, /* V offset */
	{ 0x3618, 0x00 },
	{ 0x3612, 0x29 },
	{ 0x3709, 0x52 },
	{ 0x370c, 0x03 },
	/* banding filter */
	{ 0x3a02, 0x02 }, /* 60Hz max exposure */
	{ 0x3a03, 0xe4 }, /* 60Hz max exposure */
	{ 0x3a09, 0xbc }, /* B50 low */
	{ 0x3a0a, 0x01 }, /* B60 high */
	{ 0x3a0b, 0x72 }, /* B60 low */
	{ 0x3a0e, 0x01 }, /* B50 max */
	{ 0x3a0d, 0x02 }, /* B60 max */
	{ 0x3a14, 0x02 }, /* 50Hz max exposure */
	{ 0x3a15, 0xe4 }, /* 50Hz max exposure */
	{ 0x4004, 0x02 }, /* BLC line number */

#if 1	/* 0, 60fps; 1, 30fps */
	{ 0x4713, 0x02 }, /* JPEG mode 2 */
	{ 0x460b, 0x37 }, /* 0x37 */
	{ 0x460c, 0x20 }, /* VFIFO, PCLK auto    0x20 */
	{ 0x4837, 0x16 }, /* MIPI global timing */
#else
	{ 0x4713, 0x00 }, /* JPEG mode */
	{ 0x460b, 0x35 },
	{ 0x460c, 0x22 }, /* VFIFO, PCLK manual */
	{ 0x4837, 0x0a }, /* MIPI global timing */
#endif
	{ 0x3824, 0x01 }, /* PCLK divider */

	{ 0x5001, 0x83 }, /* SDE on, scale off, UV average off,
			   * CMX on, AWB on 0x83 */
};

static struct dev_mode ov5640_mipi_mode[] = {
	{
		.index = 0,
		.inp_fmt = VISS_MBUS_FMT_YUYV8_2X8,
		.out_fmt = VISS_PIX_FMT_NV12,
		.colorspace = DEV_COLORSPACE_JPEG,
		.frame_rate = 30000,   /* 30fps */
		.frame_size.width = 640,
		.frame_size.height = 480,
		.usr_data = (void *)ov5640_yuv_vga_30fps,
		.usr_data_size = ARRAY_SIZE(ov5640_yuv_vga_30fps),
	},
	{
		.index = 1,
		.inp_fmt = VISS_MBUS_FMT_YUYV8_2X8,
		.out_fmt = VISS_PIX_FMT_NV12,
		.colorspace = DEV_COLORSPACE_JPEG,
		.frame_rate = 30000,   /* 30fps */
		.frame_size.width = 1280,
		.frame_size.height = 720,
		.usr_data = (void *)ov5640_yuv_720p_30fps,
		.usr_data_size = ARRAY_SIZE(ov5640_yuv_720p_30fps),
	},
};

static rt_err_t __ov5640_mipi_wake_up(void *hdl)
{
	struct video_dev *ov5640 = (struct video_dev *)hdl;

	return viss_i2c_write_reg_16bit(ov5640->i2c_client, 0x3008, 0x02);
}

static rt_err_t __ov5640_mipi_block_write(void *hdl,
						void *data, rt_int32_t size)
{
	struct video_dev *ov5640 = (struct video_dev *)hdl;
	struct sensor_reg *reg = (struct sensor_reg *)data;
	rt_int32_t i = 0;
	rt_err_t ret = 0;
	rt_int32_t delay = 0, temp = 0;

	for (i = 0; i < size; i++) {
		if (OV5640_MIPI_REG_DELAY_FLAG == reg[i].reg_add) {
			temp = reg[i].reg_value + 9;
			do_div(temp, 10);
			delay = temp * 10;
			LOG_D("delay: %d", delay);
			do_div(delay, 10);
			rt_thread_delay(delay);
			continue;
		}
		ret = viss_i2c_write_reg_16bit(ov5640->i2c_client,
					reg[i].reg_add, reg[i].reg_value);
		if (RT_EOK != ret) {
			LOG_E("i2c write %x value:%x fail.\n",
				reg[i].reg_add, reg[i].reg_value);
			return ret;
		}
	}

	return RT_EOK;
}

static struct dev_mode *__ov5640_mipi_cur_mode(void *hdl)
{
	struct video_dev *ov5640 = (struct video_dev *)hdl;

	return ov5640->cur_mode;
}

static struct dev_mode *__ov5640_mipi_get_all_mode(void *hdl, rt_int32_t *num)
{
	RT_ASSERT(RT_NULL != num);
	*num = (rt_int32_t)ARRAY_SIZE(ov5640_mipi_mode);

	return ov5640_mipi_mode;
}

static rt_err_t __ov5640_mipi_set_mode(void *hdl, rt_int32_t index)
{
	struct video_dev *ov5640 = (struct video_dev *)hdl;
	rt_int32_t num = 0;
	rt_err_t ret = 0;

	num = (rt_int32_t)ARRAY_SIZE(ov5640_mipi_mode);
	if (index >= num) {
		LOG_E("Input mode index error. num:%d index:%d\n", num, index);
		return -RT_EINVAL;
	}
	if (index >= ARRAY_SIZE(ov5640_mipi_mode)) {
		LOG_E("Input para error. index:%d\n", index);
		return -RT_EINVAL;
	}
	ov5640->cur_mode = &ov5640_mipi_mode[index];
	ret = __ov5640_mipi_block_write(hdl, ov5640->cur_mode->usr_data,
				ov5640->cur_mode->usr_data_size);
	if (RT_EOK != ret)
		return ret;

	return __ov5640_mipi_wake_up(hdl);
}

static rt_err_t __ov5640_mipi_set_register(void *hdl,
					struct viss_dbg_register *reg)
{
	return RT_EOK;
}

static rt_err_t __ov5640_mipi_get_register(void *hdl,
					struct viss_dbg_register *reg)
{
	return RT_EOK;
}

static rt_err_t __ov5640_mipi_set_power(void *hdl, rt_bool_t on)
{
	struct video_dev *ov5640 = (struct video_dev *)hdl;

	if ((1 != ov5640->pwdn_valid) || (1 != ov5640->rst_valid))
		return RT_EOK;

	if (RT_TRUE == on) {
		pinctrl_gpio_set_value(ov5640->pctrl, ov5640->rst_gpio, 0);
		rt_thread_delay(1); /* 10ms */
		pinctrl_gpio_set_value(ov5640->pctrl, ov5640->pwdn_gpio, 1);
		rt_thread_delay(1); /* 10ms */
		pinctrl_gpio_set_value(ov5640->pctrl, ov5640->pwdn_gpio, 0);
		rt_thread_delay(1); /* 10ms */
		pinctrl_gpio_set_value(ov5640->pctrl, ov5640->rst_gpio, 1);
		rt_thread_delay(1); /* 10ms */
	} else {
		/* TO DO */
	}

	return RT_EOK;
}

static rt_err_t __ov5640_mipi_set_stream(void *hdl, rt_bool_t enable)
{
	return RT_EOK;
}

rt_err_t __ov5640_mipi_ioctl(void *hdl, rt_int32_t cmd, void *para)
{
	rt_err_t ret = 0;

	return ret;
}

static rt_err_t __ov5640_mipi_get_info(void *hdl, struct viss_source_info *info)
{
	struct video_dev *ov5640 = (struct video_dev *)hdl;

	if ((RT_NULL == hdl) || (RT_NULL == info)) {
		LOG_E("Get ov5640 information fail.");
		return -RT_ERROR;
	}
	rt_memcpy(info, &ov5640->info, sizeof(struct viss_source_info));

	return RT_EOK;
}

static rt_err_t __ov5640_mipi_parser_config(void *hdl)
{
	struct video_dev *ov5640 = (struct video_dev *)hdl;
	struct viss_source_info info;
	rt_uint32_t tmp_data = 0;
	const char *status;
	const char *i2c_bus_name;
	const char *module = DRV_MCSI_0V5640_NAME;
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
		LOG_W("Ov5640 mipi disable.");
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
			info.data_lanes = tmp_data - 1;
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
	rt_memcpy(&ov5640->info, &info, sizeof(struct viss_source_info));
	LOG_D("ov5640->info.if_type:%d", ov5640->info.if_type);
	/* power pin */
	ov5640->pwdn_valid = 1;
	ret = config_get_u32_array(module, "mcsi-pwdn",
				ov5640->pwdn_val, ARRAY_SIZE(ov5640->pwdn_val));
	if (ret != ARRAY_SIZE(ov5640->pwdn_val)) {
		LOG_E("mcsi: power pin config error. ret:%d", ret);
		ov5640->pwdn_valid = 0;
	}

	/* reset pin */
	ov5640->rst_valid = 1;
	ret = config_get_u32_array(module, "mcsi-rst",
				ov5640->rst_val, ARRAY_SIZE(ov5640->rst_val));
	if (ret != ARRAY_SIZE(ov5640->rst_val)) {
		LOG_E("mcsi: reset pin config error. ret:%d", ret);
		ov5640->rst_valid = 0;
	}

	/* mclk pin */
	ov5640->mclk_valid = 1;
	ret = config_get_u32_array(module, "mcsi-mclk",
				ov5640->mclk_val, ARRAY_SIZE(ov5640->mclk_val));
	if (ret != ARRAY_SIZE(ov5640->mclk_val)) {
		LOG_E("mcsi: mclk pin config error. ret:%d", ret);
		ov5640->mclk_valid = 0;
	}

	return RT_EOK;
}

static rt_err_t __ov5640_mipi_prepare(void *hdl)
{
	return __ov5640_mipi_parser_config(hdl);
}

static rt_err_t __ov5640_mipi_init(void *hdl)
{
	rt_int32_t ret = 0;
	rt_uint8_t tmp[2] = {0};
	rt_uint16_t id = 0;
	struct video_dev *ov5640 = (struct video_dev *)hdl;

	RT_ASSERT(RT_NULL != ov5640);
	ov5640->pctrl = pinctrl_get(DRV_MCSI_0V5640_NAME);
	if (RT_NULL == ov5640->pctrl)
		return -RT_ERROR;
	if (1 == ov5640->pwdn_valid) {
		ov5640->pwdn_gpio = pinctrl_gpio_request(ov5640->pctrl,
					ov5640->pwdn_val[0], ov5640->pwdn_val[1]);
		if (ov5640->pwdn_gpio >= 0) {
			LOG_D("%d %d %d %d %d", ov5640->pwdn_val[2],
				ov5640->pwdn_val[3], ov5640->pwdn_val[4],
				ov5640->pwdn_val[5], ov5640->pwdn_val[6]);
			pinctrl_gpio_set_function(ov5640->pctrl, ov5640->pwdn_gpio,
						ov5640->pwdn_val[2]);
			pinctrl_gpio_set_drv_level(ov5640->pctrl, ov5640->pwdn_gpio,
						ov5640->pwdn_val[3]);
			pinctrl_gpio_set_pud_mode(ov5640->pctrl, ov5640->pwdn_gpio,
						ov5640->pwdn_val[4]);
			pinctrl_gpio_set_pud_res(ov5640->pctrl, ov5640->pwdn_gpio,
						ov5640->pwdn_val[5]);
			pinctrl_gpio_set_value(ov5640->pctrl, ov5640->pwdn_gpio,
						ov5640->pwdn_val[6]);
		} else
			ov5640->pwdn_valid = 0;
	}
	if (1 == ov5640->rst_valid) {
		ov5640->rst_gpio = pinctrl_gpio_request(ov5640->pctrl,
					ov5640->rst_val[0], ov5640->rst_val[1]);
		if (ov5640->rst_gpio >= 0) {
			LOG_D("%d %d %d %d %d", ov5640->rst_val[2],
				ov5640->rst_val[3], ov5640->rst_val[4],
				ov5640->rst_val[5], ov5640->rst_val[6]);
			pinctrl_gpio_set_function(ov5640->pctrl, ov5640->rst_gpio,
						ov5640->rst_val[2]);
			pinctrl_gpio_set_drv_level(ov5640->pctrl, ov5640->rst_gpio,
						ov5640->rst_val[3]);
			pinctrl_gpio_set_pud_mode(ov5640->pctrl, ov5640->rst_gpio,
						ov5640->rst_val[4]);
			pinctrl_gpio_set_pud_res(ov5640->pctrl, ov5640->rst_gpio,
						ov5640->rst_val[5]);
			pinctrl_gpio_set_value(ov5640->pctrl, ov5640->rst_gpio,
						ov5640->rst_val[6]);
		} else
			ov5640->rst_valid = 0;
	}
	if (1 == ov5640->mclk_valid) {
		ov5640->mclk_gpio = pinctrl_gpio_request(ov5640->pctrl,
					ov5640->mclk_val[0], ov5640->mclk_val[1]);
		if (ov5640->mclk_gpio >= 0) {
			pinctrl_gpio_set_function(ov5640->pctrl, ov5640->mclk_gpio,
						ov5640->mclk_val[2]);
			pinctrl_gpio_set_drv_level(ov5640->pctrl, ov5640->mclk_gpio,
						ov5640->mclk_val[3]);
			pinctrl_gpio_set_pud_mode(ov5640->pctrl, ov5640->mclk_gpio,
						ov5640->mclk_val[4]);
			pinctrl_gpio_set_pud_res(ov5640->pctrl, ov5640->mclk_gpio,
						ov5640->mclk_val[5]);
			pinctrl_gpio_set_value(ov5640->pctrl, ov5640->mclk_gpio,
						ov5640->mclk_val[6]);
		} else
			ov5640->mclk_valid = 0;
	}
	__ov5640_mipi_set_power(ov5640, RT_TRUE);

	ov5640->i2c_client = rt_zalloc(sizeof(struct viss_i2c_client));
	if (RT_NULL == ov5640->i2c_client)
		return -RT_ENOMEM;
	ov5640->i2c_client->i2c_bus = rt_i2c_bus_device_find(ov5640->info.i2c_bus_name);
	if (RT_NULL == ov5640->i2c_client->i2c_bus) {
		LOG_E("can't find bus dev \"%s\"", ov5640->info.i2c_bus_name);
		goto exit;
	}
	ov5640->i2c_client->i2c_addr = ov5640->info.i2c_addr;
	ret = viss_i2c_read_reg_16bit(ov5640->i2c_client, 0x300a, &tmp[0]);
	LOG_D("ret: %d. 0x300a: %x", ret, tmp[0]);
	ret = viss_i2c_read_reg_16bit(ov5640->i2c_client, 0x300b, &tmp[1]);
	LOG_D("ret: %d. 0x300b: %x", ret, tmp[1]);
	id = (tmp[0] << 8) | tmp[1];
	if (id != 0x5640) {
		LOG_E("ID wrong! (0x%x)\n", id);
		goto exit;
	}

	__ov5640_mipi_block_write((void *)ov5640,
		(void *)ov5640_reg_list, ARRAY_SIZE(ov5640_reg_list));

	return RT_EOK;

exit:
	if (ov5640->pctrl) {
		if (ov5640->rst_valid)
			pinctrl_gpio_free(ov5640->pctrl, ov5640->rst_gpio);
		if (ov5640->pwdn_valid)
			pinctrl_gpio_free(ov5640->pctrl, ov5640->pwdn_gpio);
		if (ov5640->mclk_valid)
			pinctrl_gpio_free(ov5640->pctrl, ov5640->mclk_gpio);
		pinctrl_put(ov5640->pctrl);
		ov5640->pctrl = RT_NULL;
	}
	if (ov5640->i2c_client) {
		rt_free(ov5640->i2c_client);
		ov5640->i2c_client = RT_NULL;
	}

	return -RT_ERROR;
}


static void __ov5640_mipi_exit(void *hdl)
{
	struct video_dev *ov5640 = (struct video_dev *)hdl;

	__ov5640_mipi_set_power(ov5640, RT_FALSE);
	if (ov5640->pctrl) {
		if (ov5640->rst_valid)
			pinctrl_gpio_free(ov5640->pctrl, ov5640->rst_gpio);
		if (ov5640->pwdn_valid)
			pinctrl_gpio_free(ov5640->pctrl, ov5640->pwdn_gpio);
		if (ov5640->mclk_valid)
			pinctrl_gpio_free(ov5640->pctrl, ov5640->mclk_gpio);
		pinctrl_put(ov5640->pctrl);
		ov5640->pctrl = RT_NULL;
	}
	if (ov5640->i2c_client) {
		rt_free(ov5640->i2c_client);
		ov5640->i2c_client = RT_NULL;
	}
}

struct video_dev ov5640_mipi = {
	.name = DRV_MCSI_0V5640_NAME,
	.group_id = GRP_ID_MCSI,
	.prepare = __ov5640_mipi_prepare,
	.init = __ov5640_mipi_init,
	.exit = __ov5640_mipi_exit,
	.s_mode = __ov5640_mipi_set_mode,
	.g_cur_mode = __ov5640_mipi_cur_mode,
	.g_all_mode = __ov5640_mipi_get_all_mode,
	.s_power = __ov5640_mipi_set_power,
	.s_stream = __ov5640_mipi_set_stream,
	.g_info = __ov5640_mipi_get_info,
	.ioctl = __ov5640_mipi_ioctl,
	.s_register = __ov5640_mipi_set_register,
	.g_register = __ov5640_mipi_get_register,
};

