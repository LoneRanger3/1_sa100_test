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

#define DBG_SECTION_NAME	"OV5640-DVP"
#define DBG_LEVEL		DBG_LOG

#include <debug.h>
#include "viss.h"
#include "vic_dev.h"
#include "viss_i2c.h"
#include <div.h>

struct sensor_reg {
	rt_uint16_t reg_add;
	rt_uint16_t reg_value;
};

#define DRV_VIC_0V5640_NAME "ov5640-dvp"
#define OV5640_DVP_REG_DELAY_FLAG  (0xffff)

/* #define COLOR_BAR */

struct dev_mode *cur_dvp_mode = RT_NULL;

/*
 * 15fps VGA YUV output
 * 24MHz input clock, 24MHz PCLK
 */
static const struct sensor_reg ov5640_reg_list[] = {
	{ 0x3103, 0x11 }, /* system clock from pad, bit[1] */
	{ 0x3008, 0x82 }, /* software reset, bit[7] */
	{ OV5640_DVP_REG_DELAY_FLAG, 0x5 }, /*	 mdelay(5); */
	{ 0x3008, 0x42 }, /* software power down, bit[6] */
	{ OV5640_DVP_REG_DELAY_FLAG, 0x5 },
	{ 0x3103, 0x03 }, /* system clock from PLL, bit[1] */
	{ 0x3017, 0xff }, /* FREX, Vsync, HREF, PCLK, D[9:6] output enable */
	{ 0x3018, 0xff }, /* D[5:0], GPIO[1:0] output enable */
	{ 0x3034, 0x18 }, /* MIPI 8-bit */

	/* PLL root divider, bit[4], PLL pre-divider, bit[3:0],
	 * default is 0x13 */
	{ 0x3037, 0x13 },

	/* PCLK root divider, bit[5:4], SCLK2x root divider, bit[3:2] */
	{ 0x3108, 0x01 },
	/*
	 * SCLK root divider, bit[1:0]
	 */
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
	{ 0x3600, 0x08 }, /* VCM control */
	{ 0x3601, 0x33 }, /* VCM control */
	{ 0x302d, 0x60 }, /* system control */
	{ 0x3620, 0x52 },
	{ 0x371b, 0x20 },
	{ 0x471c, 0x50 },
	{ 0x3a13, 0x43 }, /* pre-gain = 1.047x */
	{ 0x3a18, 0x00 }, /* gain ceiling */
	{ 0x3a19, 0xf8 }, /* gain ceiling = 15.5x */
	{ 0x3635, 0x13 },
	{ 0x3636, 0x03 },
	{ 0x3634, 0x40 },
	{ 0x3622, 0x01 },

	/* 50/60Hz detection */
	{ 0x3c01, 0x34 }, /* Band auto, bit[7] */
	{ 0x3c04, 0x28 }, /* threshold low sum */
	{ 0x3c05, 0x98 }, /* threshold high sum */
	{ 0x3c06, 0x00 }, /* light meter 1 threshold[15:8] */
	{ 0x3c07, 0x08 }, /* light meter 1 threshold[7:0] */
	{ 0x3c08, 0x00 }, /* light meter 2 threshold[15:8] */
	{ 0x3c09, 0x1c }, /* light meter 2 threshold[7:0] */
	{ 0x3c0a, 0x9c }, /* sample number[15:8] */
	{ 0x3c0b, 0x40 }, /* sample number[7:0] */
	{ 0x3810, 0x00 }, /* Timing Hoffset[11:8] */
	{ 0x3811, 0x10 }, /* Timing Hoffset[7:0] */
	{ 0x3812, 0x00 }, /* Timing Voffset[10:8] */
	{ 0x3708, 0x64 },
	{ 0x4001, 0x02 }, /* BLC start from line 2 */
	{ 0x4005, 0x1a }, /* BLC always update */
	{ 0x3000, 0x00 }, /* enable blocks */
	{ 0x3004, 0xff }, /* enable clocks */
	{ 0x300e, 0x58 }, /* MIPI power down, DVP enable */
	{ 0x302e, 0x00 },
	{ 0x4300, 0x30 }, /* YUV 422, YUYV 2018-3-19  YUYV */
	{ 0x501f, 0x00 }, /* YUV 422 */
	{ 0x440e, 0x00 },
	{ 0x5000, 0xa7 }, /* Lenc on, raw gamma on, BPC on, WPC on, CIP on */

	/* AEC target */
	{ 0x3a0f, 0x30 }, /* stable range in high */
	{ 0x3a10, 0x28 }, /* stable range in low */
	{ 0x3a1b, 0x30 }, /* stable range out high */
	{ 0x3a1e, 0x26 }, /* stable range out low */
	{ 0x3a11, 0x60 }, /* fast zone high */
	{ 0x3a1f, 0x14 }, /* fast zone low */

	/* Lens correction for ? */
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
	{ 0x583d, 0xce }, /* lenc BR offset */

	/* AWB */
	{ 0x5180, 0xff }, /* AWB B block */
	{ 0x5181, 0xf2 }, /* AWB control */
	{ 0x5182, 0x00 }, /* [7:4] max local counter, [3:0] max fast counter */
	{ 0x5183, 0x14 }, /* AWB advanced */
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
	{ 0x5191, 0xf8 }, /* AWB top limit */
	{ 0x5192, 0x04 }, /* AWB bottom limit */
	{ 0x5193, 0x70 }, /* red limit */
	{ 0x5194, 0xf0 }, /* green limit */
	{ 0x5195, 0xf0 }, /* blue limit */
	{ 0x5196, 0x03 }, /* AWB control */
	{ 0x5197, 0x01 }, /* local limit */
	{ 0x5198, 0x04 },
	{ 0x5199, 0x12 },
	{ 0x519a, 0x04 },
	{ 0x519b, 0x00 },
	{ 0x519c, 0x06 },
	{ 0x519d, 0x82 },
	{ 0x519e, 0x38 }, /* AWB control */

	/* Gamma */
	{ 0x5480, 0x01 }, /* Gamma bias plus on, bit[0] */
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

	/* color matrix */
	{ 0x5381, 0x1e }, /* CMX1 for Y */
	{ 0x5382, 0x5b }, /* CMX2 for Y */
	{ 0x5383, 0x08 }, /* CMX3 for Y */
	{ 0x5384, 0x0a }, /* CMX4 for U */
	{ 0x5385, 0x7e }, /* CMX5 for U */
	{ 0x5386, 0x88 }, /* CMX6 for U */
	{ 0x5387, 0x7c }, /* CMX7 for V */
	{ 0x5388, 0x6c }, /* CMX8 for V */
	{ 0x5389, 0x10 }, /* CMX9 for V */
	{ 0x538a, 0x01 }, /* sign[9] */
	{ 0x538b, 0x98 }, /* sign[8:1] */

	/* UV adjsut */
	{ 0x5580, 0x06 }, /* saturation on, bit[1] */
	{ 0x5583, 0x40 },
	{ 0x5584, 0x10 },
	{ 0x5589, 0x10 },
	{ 0x558a, 0x00 },
	{ 0x558b, 0xf8 },
	{ 0x501d, 0x40 }, /* enable manual offset of contrast */

	/* CIP */
	{ 0x5300, 0x08 }, /* CIP sharpen MT threshold 1 */
	{ 0x5301, 0x30 }, /* CIP sharpen MT threshold 2 */
	{ 0x5302, 0x10 }, /* CIP sharpen MT offset 1 */
	{ 0x5303, 0x00 }, /* CIP sharpen MT offset 2 */
	{ 0x5304, 0x08 }, /* CIP DNS threshold 1 */
	{ 0x5305, 0x30 }, /* CIP DNS threshold 2 */
	{ 0x5306, 0x08 }, /* CIP DNS offset 1 */
	{ 0x5307, 0x16 }, /* CIP DNS offset 2 */
	{ 0x5309, 0x08 }, /* CIP sharpen TH threshold 1 */
	{ 0x530a, 0x30 }, /* CIP sharpen TH threshold 2 */
	{ 0x530b, 0x04 }, /* CIP sharpen TH offset 1 */
	{ 0x530c, 0x06 }, /* CIP sharpen TH offset 2 */
	{ 0x5025, 0x00 },
#ifdef COLOR_BAR
	{ 0x503d, 0x80 }, /* color bar */
#endif
};

/*
 * VGA 30fps
 * YUV VGA 30fps, night mode 5fps
 * Input Clock = 24Mhz, PCLK = 56MHz
 */
static const struct sensor_reg ov5640_yuv_vga_30fps[] = {
#ifdef CONFIG_ARCH_LOMBO_N7V0_FPGA
	{ 0x3035, 0x41 }, /* PLL, default is 0x11 */
#else
	{ 0x3035, 0x11 }, /* PLL, default is 0x11 */
#endif
	{ 0x3036, 0x4A }, /* PLL, default is 0x46 */
	{ 0x3c07, 0x08 }, /* light meter 1 threshold [7:0] */
	{ 0x3820, 0x41 }, /* Sensor flip off, ISP flip on */
	{ 0x3821, 0x07 }, /* Sensor mirror on, ISP mirror on, H binning on */
	{ 0x3814, 0x31 }, /* X INC */
	{ 0x3815, 0x31 }, /* Y INC */
	{ 0x3800, 0x00 }, /* HS */
	{ 0x3801, 0x00 }, /* HS */
	{ 0x3802, 0x00 }, /* VS */
	{ 0x3803, 0x04 }, /* VS */
	{ 0x3804, 0x0a }, /* HW (HE) */
	{ 0x3805, 0x3f }, /* HW (HE) */
	{ 0x3806, 0x07 }, /* VH (VE) */
	{ 0x3807, 0x9b }, /* VH (VE) */
#if 1
	{ 0x3808, 0x02 }, /* DVPHO */
	{ 0x3809, 0x80 }, /* DVPHO */
	{ 0x380a, 0x01 }, /* DVPVO */
	{ 0x380b, 0xe0 }, /* DVPVO */
#else
	{ 0x3808, 0x00 }, /* DVPHO */
	{ 0x3809, 0xa0 }, /* DVPHO */
	{ 0x380a, 0x00 }, /* DVPVO */
	{ 0x380b, 0x78 }, /* DVPVO */
#endif
	{ 0x380c, 0x07 }, /* HTS */
	{ 0x380d, 0x68 }, /* HTS */
	{ 0x380e, 0x03 }, /* VTS */
	{ 0x380f, 0xd8 }, /* VTS */
	{ 0x3813, 0x06 }, /* Timing Voffset */

	/* bit[7:6]: output drive capability[00: 1x   01: 2x  10: 3x  11: 4x] */
	{ 0x302c, 0x02 },

	{ 0x3618, 0x00 },
	{ 0x3612, 0x29 },
	{ 0x3709, 0x52 },
	{ 0x370c, 0x03 },
	{ 0x3a02, 0x17 }, /* 60Hz max exposure, night mode 5fps */
	{ 0x3a03, 0x10 }, /* 60Hz max exposure */
	/*
	 * banding filters are calculated automatically in camera driver
	 */
#if 1
	{ 0x3a08, 0x01 }, /* B50 step */
	{ 0x3a09, 0x86 }, /* B50 step */
	{ 0x3a0a, 0x01 }, /* B60 step */
	{ 0x3a0b, 0x45 }, /* B60 step */
	{ 0x3a0e, 0x02 }, /* 50Hz max band */
	{ 0x3a0d, 0x03 }, /* 60Hz max band */
#endif
	{ 0x3a14, 0x17 }, /* 50Hz max exposure, night mode 5fps */
	{ 0x3a15, 0x10 }, /* 50Hz max exposure */
	{ 0x4004, 0x02 }, /* BLC 2 lines */
	{ 0x3002, 0x1c }, /* reset JFIFO, SFIFO, JPEG */
	{ 0x3006, 0xc3 }, /* disable clock of JPEG2x, JPEG */
	{ 0x4713, 0x03 }, /* JPEG mode 3 */
	{ 0x4407, 0x04 }, /* Quantization scale */
	{ 0x460b, 0x35 },
	{ 0x460c, 0x22 },
	{ 0x4837, 0x22 }, /* DVP CLK divider */
	{ 0x3824, 0x02 }, /* DVP CLK divider, default is 0x2 */
	/* SDE on, scale on, UV average off, color matrix on, AWB on */
	{ 0x5001, 0xa3 },
	{ 0x3503, 0x00 }, /* AEC/ AGC on */
#ifdef COLOR_BAR
	{ 0x503d, 0x80 }, /* color bar */
#endif

};

static const struct sensor_reg ov5640_yuv_vga_15fps[] = {
#ifndef CONFIG_ARCH_LOMBO_N7V0_FPGA
	{ 0x3008, 0x82 },
	{ 0x3103, 0x03 },
	{ 0x3017, 0xff },
	{ 0x3018, 0xff },
	{ 0x3108, 0x01 },
	{ 0x3037, 0x13 },
	{ 0x3630, 0x2e },
	{ 0x3632, 0xe2 },
	{ 0x3633, 0x23 },
	{ 0x3634, 0x44 },
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
	{ 0x3600, 0x08 },
	{ 0x3601, 0x33 },
	{ 0x471c, 0x50 },
	{ 0x3820, 0x41 },
	{ 0x3821, 0x07 },
	{ 0x3814, 0x31 },
	{ 0x3815, 0x31 },
	{ 0x3800, 0x00 },
	{ 0x3801, 0x00 },
	{ 0x3802, 0x00 },
	{ 0x3803, 0x04 },
	{ 0x3804, 0x0a },
	{ 0x3805, 0x3f },
	{ 0x3806, 0x07 },
	{ 0x3807, 0x9b },

	/* set output size */
	{ 0x3808, 0x02 }, /* DVPHO */
	{ 0x3809, 0x80 }, /* DVPHO */
	{ 0x380a, 0x01 }, /* DVPVO */
	{ 0x380b, 0xe0 }, /* DVPVO */

	{ 0x380c, 0x07 },
	{ 0x380d, 0x68 },
	{ 0x380e, 0x03 },
	{ 0x380f, 0xd8 },
	{ 0x3810, 0x00 },
	{ 0x3811, 0x10 },
	{ 0x3812, 0x00 },
	{ 0x3813, 0x06 },
	{ 0x3618, 0x00 },
	{ 0x3612, 0x49 },
	{ 0x3708, 0x62 },
	{ 0x3709, 0x52 },
	{ 0x370c, 0x03 },
	{ 0x3a02, 0x03 },
	{ 0x3a03, 0xd8 },
	{ 0x3a08, 0x01 },
	{ 0x3a09, 0x27 },
	{ 0x3a0a, 0x00 },
	{ 0x3a0b, 0xf6 },
	{ 0x3a0e, 0x03 },
	{ 0x3a0d, 0x04 },
	{ 0x3a14, 0x03 },
	{ 0x3a15, 0xd8 },
	{ 0x4001, 0x02 },
	{ 0x4004, 0x02 },
	{ 0x3002, 0x1c },
	{ 0x3006, 0xc3 },
	{ 0x4300, 0x30 },
	{ 0x501f, 0x00 },
	{ 0x4713, 0x03 },
	{ 0x3035, 0x11 },
	{ 0x3036, 0x3c }, /* default 0x46 */
	{ 0x4407, 0x04 },
	{ 0x460b, 0x35 },
	{ 0x460c, 0x22 },
	{ 0x3824, 0x02 },
	{ 0x5000, 0xa7 },
	{ 0x5001, 0xa3 },
	{ 0x5000, 0xa7 },
	{ 0x3622, 0x01 },
	{ 0x3635, 0x1c },
	{ 0x3634, 0x40 },
	{ 0x3c01, 0x34 },
	{ 0x3c00, 0x00 },
	{ 0x3c04, 0x28 },
	{ 0x3c05, 0x98 },
	{ 0x3c06, 0x00 },
	{ 0x3c07, 0x08 },
	{ 0x3c08, 0x00 },
	{ 0x3c09, 0x1c },
	{ 0x300c, 0x22 },
	{ 0x3c0a, 0x9c },
	{ 0x3c0b, 0x40 },
	{ 0x5180, 0xff },
	{ 0x5181, 0xf2 },
	{ 0x5182, 0x00 },
	{ 0x5183, 0x94 },
	{ 0x5184, 0x25 },
	{ 0x5185, 0x24 },
	{ 0x5186, 0x06 },
	{ 0x5187, 0x08 },
	{ 0x5188, 0x08 },
	{ 0x5189, 0x78 },
	{ 0x518a, 0x54 },
	{ 0x518b, 0xb2 },
	{ 0x518c, 0xb2 },
	{ 0x518d, 0x44 },
	{ 0x518e, 0x3d },
	{ 0x518f, 0x58 },
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
	{ 0x5381, 0x1c },
	{ 0x5382, 0x5a },
	{ 0x5383, 0x06 },
	{ 0x5384, 0x20 },
	{ 0x5385, 0x80 },
	{ 0x5386, 0xa0 },
	{ 0x5387, 0xa2 },
	{ 0x5388, 0xa0 },
	{ 0x5389, 0x02 },
	{ 0x538a, 0x01 },
	{ 0x538b, 0x98 },
	{ 0x5300, 0x08 },
	{ 0x5301, 0x30 },
	{ 0x5302, 0x10 },
	{ 0x5303, 0x00 },
	{ 0x5304, 0x08 },
	{ 0x5305, 0x30 },
	{ 0x5306, 0x08 },
	{ 0x5307, 0x16 },
	{ 0x5309, 0x08 },
	{ 0x530a, 0x30 },
	{ 0x530b, 0x04 },
	{ 0x530c, 0x06 },
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
	{ 0x5580, 0x02 },
	{ 0x5583, 0x40 },
	{ 0x5584, 0x10 },
	{ 0x5589, 0x10 },
	{ 0x558a, 0x00 },
	{ 0x558b, 0xf8 },
	{ 0x5800, 0x23 },
	{ 0x5801, 0x15 },
	{ 0x5802, 0x10 },
	{ 0x5803, 0x10 },
	{ 0x5804, 0x15 },
	{ 0x5805, 0x23 },
	{ 0x5806, 0x0c },
	{ 0x5807, 0x08 },
	{ 0x5808, 0x05 },
	{ 0x5809, 0x05 },
	{ 0x580a, 0x08 },
	{ 0x580b, 0x0c },
	{ 0x580c, 0x07 },
	{ 0x580d, 0x03 },
	{ 0x580e, 0x00 },
	{ 0x580f, 0x00 },
	{ 0x5810, 0x03 },
	{ 0x5811, 0x07 },
	{ 0x5812, 0x07 },
	{ 0x5813, 0x03 },
	{ 0x5814, 0x00 },
	{ 0x5815, 0x00 },
	{ 0x5816, 0x03 },
	{ 0x5817, 0x07 },
	{ 0x5818, 0x0b },
	{ 0x5819, 0x08 },
	{ 0x581a, 0x05 },
	{ 0x581b, 0x05 },
	{ 0x581c, 0x07 },
	{ 0x581d, 0x0b },
	{ 0x581e, 0x2a },
	{ 0x581f, 0x16 },
	{ 0x5820, 0x11 },
	{ 0x5821, 0x11 },
	{ 0x5822, 0x15 },
	{ 0x5823, 0x29 },
	{ 0x5824, 0xbf },
	{ 0x5825, 0xaf },
	{ 0x5826, 0x9f },
	{ 0x5827, 0xaf },
	{ 0x5828, 0xdf },
	{ 0x5829, 0x6f },
	{ 0x582a, 0x8e },
	{ 0x582b, 0xab },
	{ 0x582c, 0x9e },
	{ 0x582d, 0x7f },
	{ 0x582e, 0x4f },
	{ 0x582f, 0x89 },
	{ 0x5830, 0x86 },
	{ 0x5831, 0x98 },
	{ 0x5832, 0x6f },
	{ 0x5833, 0x4f },
	{ 0x5834, 0x6e },
	{ 0x5835, 0x7b },
	{ 0x5836, 0x7e },
	{ 0x5837, 0x6f },
	{ 0x5838, 0xde },
	{ 0x5839, 0xbf },
	{ 0x583a, 0x9f },
	{ 0x583b, 0xbf },
	{ 0x583c, 0xec },
	{ 0x5025, 0x00 },
	{ 0x3a0f, 0x30 },
	{ 0x3a10, 0x28 },
	{ 0x3a1b, 0x30 },
	{ 0x3a1e, 0x26 },
	{ 0x3a11, 0x60 },
	{ 0x3a1f, 0x14 },
	{ 0x3a18, 0x00 },
	{ 0x3a19, 0xf8 },
	{ 0x3035, 0x41 },

#ifdef COLOR_BAR
	{ 0x503d, 0x80 },
#endif
#endif
};

/* 1280x720, 30fps
 * input clock 24Mhz, PCLK 42Mhz */
static const struct sensor_reg ov5640_yuv_720p[] = {
	{ 0x3035, 0x21 }, /* PLL */
	{ 0x3036, 0x54 }, /* PLL */
	{ 0x3c07, 0x07 }, /* lightmeter 1 threshold[7:0] */
	{ 0x3820, 0x41 }, /* flip */
	{ 0x3821, 0x07 }, /* mirror */
	{ 0x3814, 0x31 }, /* timing X inc */
	{ 0x3815, 0x31 }, /* timing Y inc */
	{ 0x3800, 0x00 }, /* HS */
	{ 0x3801, 0x00 }, /* HS */
	{ 0x3802, 0x00 }, /* VS */
	{ 0x3803, 0xfa }, /* VS */
	{ 0x3804, 0x0a }, /* HW (HE) */
	{ 0x3805, 0x3f }, /* HW (HE) */
	{ 0x3806, 0x06 }, /* VH (VE) */
	{ 0x3807, 0xa9 }, /* VH (VE) */
	{ 0x3808, 0x05 }, /* DVPHO */
	{ 0x3809, 0x00 }, /* DVPHO */
	{ 0x380a, 0x02 }, /* DVPVO */
	{ 0x380b, 0xd0 }, /* DVPVO */
	{ 0x380c, 0x07 }, /* HTS */
	{ 0x380d, 0x64 }, /* HTS */
	{ 0x380e, 0x02 }, /* VTS */
	{ 0x380f, 0xe4 }, /* VTS */
	{ 0x3813, 0x04 }, /* timing V offset */
	{ 0x3618, 0x00 },
	{ 0x3612, 0x29 },
	{ 0x3709, 0x52 },
	{ 0x370c, 0x03 },
	{ 0x3a02, 0x02 }, /* 60Hz max exposure */
	{ 0x3a03, 0xe0 }, /* 60Hz max exposure */
	/* banding filters are calculated automatically in camera driver */
	{ 0x3a08, 0x00 }, /* B50 step */
	{ 0x3a09, 0xDD }, /* B50 step */
	{ 0x3a0a, 0x00 }, /* B60 step */
	{ 0x3a0b, 0xB8 }, /* B60 step */
	{ 0x3a0e, 0x03 }, /* 50Hz max band */
	{ 0x3a0d, 0x04 }, /* 60Hz max band */

	{ 0x3a14, 0x02 }, /* 50Hz max exposure */
	{ 0x3a15, 0xe0 }, /* 50Hz max exposure */
	{ 0x4004, 0x02 }, /* BLC line number */
	{ 0x3002, 0x1c }, /* reset JFIFO, SFIFO, JPG */
	{ 0x3006, 0xc3 }, /* disable clock of JPEG2x, JPEG */
	{ 0x4713, 0x03 }, /* JPEG mode 3 */
	{ 0x4407, 0x04 }, /* Quantization scale */
	{ 0x460b, 0x37 },
	{ 0x460c, 0x20 },
	{ 0x4837, 0x16 }, /* MIPI global timing */
	{ 0x3824, 0x02 }, /* PCLK manual divider */
	{ 0x5001, 0x83 }, /* SDE on, CMX on, AWB on */
	{ 0x3503, 0x00 }, /* AEC/AGC on */
#ifdef COLOR_BAR
	{ 0x503d, 0x80 }, /* color bar */
#endif

};

static const struct sensor_reg ov5640_yuv_1080p[] = {
	{ 0x3820, 0x40 },
	{ 0x3821, 0x06 },
	/* pll and clock setting */
	{ 0x3034, 0x18 },
#ifdef CONFIG_ARCH_LOMBO_N7V0_FPGA
	{ 0x3035, 0x41 }, /* 0x11:30fps 0x21:15fps 0x41:7.5fps */
#else
	{ 0x3035, 0x11 }, /* 0x11:30fps 0x21:15fps 0x41:7.5fps */
#endif
	{ 0x3036, 0x54 },
	{ 0x3037, 0x13 },
	{ 0x3108, 0x01 },
	{ 0x3824, 0x01 },
	/* delay 5ms */
	{ OV5640_DVP_REG_DELAY_FLAG, 0x5 },
	/* timing */
	/* 1920x1080 */
	{ 0x3808, 0x07 }, /* H size MSB */
	{ 0x3809, 0x80 }, /* H size LSB */
	{ 0x380a, 0x04 }, /* V size MSB */
	{ 0x380b, 0x38 }, /* V size LSB */
	{ 0x380c, 0x09 }, /* HTS MSB */
	{ 0x380d, 0xc4 }, /* HTS LSB */
	{ 0x380e, 0x04 }, /* VTS MSB */
	{ 0x380f, 0x60 }, /* VTS LSB */

	/* banding step */
	{ 0x3a08, 0x01 }, /* 50HZ step MSB */
	{ 0x3a09, 0x50 }, /* 50HZ step LSB */
	{ 0x3a0a, 0x01 }, /* 60HZ step MSB */
	{ 0x3a0b, 0x18 }, /* 60HZ step LSB */
	{ 0x3a0e, 0x03 }, /* 50HZ step max */
	{ 0x3a0d, 0x04 }, /* 60HZ step max */

	{ 0x3503, 0x00 }, /* AEC enable */
	{ 0x350c, 0x00 },
	{ 0x350d, 0x00 },
	{ 0x3c07, 0x07 }, /* light meter 1 thereshold */
	{ 0x3814, 0x11 }, /* horizton subsample */
	{ 0x3815, 0x11 }, /* vertical subsample */
	{ 0x3800, 0x01 }, /* x address start high byte */
	{ 0x3801, 0x50 }, /* x address start low byte */
	{ 0x3802, 0x01 }, /* y address start high byte */
	{ 0x3803, 0xb2 }, /* y address start low byte */
	{ 0x3804, 0x08 }, /* x address end high byte */
	{ 0x3805, 0xef }, /* x address end low byte */
	{ 0x3806, 0x05 }, /* y address end high byte */
	{ 0x3807, 0xf1 }, /* y address end low byte */
	{ 0x3810, 0x00 }, /* isp hortizontal offset high byte */
	{ 0x3811, 0x10 }, /* isp hortizontal offset low byte */
	{ 0x3812, 0x00 }, /* isp vertical offset high byte */
	{ 0x3813, 0x04 }, /* isp vertical offset low byte */

	{ 0x4002, 0x45 }, /* BLC related */
	{ 0x4005, 0x18 }, /* BLC related */

	{ 0x3618, 0x04 },
	{ 0x3612, 0x2b },
	{ 0x3709, 0x12 },
	{ 0x370c, 0x00 },
	{ 0x3a02, 0x04 }, /* 60HZ max exposure limit MSB */
	{ 0x3a03, 0x60 }, /* 60HZ max exposure limit LSB */
	{ 0x3a14, 0x04 }, /* 50HZ max exposure limit MSB */
	{ 0x3a15, 0x60 }, /* 50HZ max exposure limit LSB */

	{ 0x4004, 0x06 }, /* BLC line number */
	{ 0x3002, 0x1c }, /* reset JFIFO SFIFO JPG */
	{ 0x3006, 0xc3 }, /* enable xx clock */
	{ 0x460b, 0x37 }, /* debug mode */
	{ 0x460c, 0x20 }, /* PCLK Manuale */
	{ 0x4837, 0x16 }, /* PCLK period */
	{ 0x5001, 0x83 }, /* ISP effect */

	{ 0x302c, 0x42 }, /* bit[7:6]: output drive capability
			   * 00: 1x, 01: 2x, 10: 3x, 11: 4x */
	{ 0x3a18, 0x00 },
	{ 0x3a19, 0x80 },

#ifdef COLOR_BAR
	{ 0x503d, 0x80 }, /* color bar */
#endif
};

#if 0
/*
 * YUV 2592x1944, 3.75fps
 * input clock 24Mhz, PCLK 42Mhz
 */
static const struct sensor_reg ov5640_yuv_5m[] = {
#ifdef CONFIG_ARCH_LOMBO_N7V0_FPGA
	{ 0x3035, 0x41 }, /* PLL */
	{ 0x3036, 0x69 }, /* PLL */
	{ 0x3c07, 0x07 }, /* lightm eter 1 threshold[7:0] */
	{ 0x3820, 0x40 }, /* flip */
	{ 0x3821, 0x06 }, /* mirror */
	{ 0x3814, 0x11 }, /* timing X inc */
	{ 0x3815, 0x11 }, /* timing Y inc */
	{ 0x3800, 0x00 }, /* HS */
	{ 0x3801, 0x00 }, /* HS */
	{ 0x3802, 0x00 }, /* VS */
	{ 0x3803, 0x00 }, /* VS */
	{ 0x3804, 0x0a }, /* HW (HE) */
	{ 0x3805, 0x3f }, /* HW (HE) */
	{ 0x3806, 0x07 }, /* VH (VE) */
	{ 0x3807, 0x9f }, /* VH (VE) */
	{ 0x3808, 0x0a }, /* DVPHO */
	{ 0x3809, 0x20 }, /* DVPHO */
	{ 0x380a, 0x07 }, /* DVPVO */
	{ 0x380b, 0x98 }, /* DVPVO */
	{ 0x380c, 0x0b }, /* HTS */
	{ 0x380d, 0x1c }, /* HTS */
	{ 0x380e, 0x07 }, /* VTS */
	{ 0x380f, 0xb0 }, /* VTS */
	{ 0x3813, 0x04 }, /* timing V offset */
	{ 0x3618, 0x04 },
	{ 0x3612, 0x2b },
	{ 0x3709, 0x12 },
	{ 0x370c, 0x00 },
	/* banding filters are calculated automatically in camera driver */
#if 0
	{ 0x3a02, 0x07 }, /* 60Hz max exposure */
	{ 0x3a03, 0xae }, /* 60Hz max exposure */
	{ 0x3a08, 0x01 }, /* B50 step */
	{ 0x3a09, 0x27 }, /* B50 step */
	{ 0x3a0a, 0x00 }, /* B60 step */
	{ 0x3a0b, 0xf6 }, /* B60 step */
	{ 0x3a0e, 0x06 }, /* 50Hz max band */
	{ 0x3a0d, 0x08 }, /* 60Hz max band */
	{ 0x3a14, 0x07 }, /* 50Hz max exposure */
	{ 0x3a15, 0xae }, /* 50Hz max exposure */
#else
	{ 0x3a02, 0x07 }, /* 60Hz max exposure */
	{ 0x3a03, 0xb0 }, /* 60Hz max exposure */
	{ 0x3a09, 0x27 }, /* B50 low */
	{ 0x3a0a, 0x00 }, /* B60 high */
	{ 0x3a0b, 0xf6 }, /* B60 low */
	{ 0x3a0e, 0x06 }, /* B50 max */
	{ 0x3a0d, 0x08 }, /* B60 max */
	{ 0x3a14, 0x07 }, /* 50Hz max exposure */
	{ 0x3a15, 0xb0 }, /* 50Hz max exposure */
#endif
	{ 0x4004, 0x06 }, /* BLC line number */
	{ 0x3002, 0x1c }, /* reset JFIFO, SFIFO, JPG */
	{ 0x3006, 0xc3 }, /* disable clock of JPEG2x, JPEG */
	{ 0x4713, 0x02 }, /* JPEG mode 3 */
	{ 0x4407, 0x0c }, /* Quantization sacle */
	{ 0x460b, 0x37 },
	{ 0x460c, 0x20 },
	{ 0x4837, 0x2c }, /* MIPI global timing */
	{ 0x3824, 0x01 }, /* PCLK manual divider */
	{ 0x5001, 0x83 }, /* SDE on, CMX on, AWB on */
	{ 0x3503, 0x00 }, /* defualt is 0x3 */

	{ 0x302c, 0x42 }, /* bit[7:6]: output drive capability
			   * 00: 1x, 01: 2x, 10: 3x, 11: 4x */
#else
	/* capture 5Mega 7.5fps */
	/* pll and clock setting */
	{ 0x3820, 0x40 },
	{ 0x3821, 0x06 },
	{ 0x3034, 0x18 },
	{ 0x3035, 0x41 },
	{ 0x3036, 0x54 },
	{ 0x3037, 0x13 },
	{ 0x3108, 0x01 },
	{ 0x3824, 0x01 },
	/* timing */
	/* 2592*1936 */
	{ 0x3808, 0x0a }, /* H size MSB */
	{ 0x3809, 0x20 }, /* H size LSB */
	{ 0x380a, 0x07 }, /* V size MSB */
	{ 0x380b, 0x90 }, /* V size LSB */
	{ 0x380c, 0x0b }, /* HTS MSB */
	{ 0x380d, 0x1c }, /* HTS LSB */
	{ 0x380e, 0x07 }, /* VTS MSB */
	{ 0x380f, 0xb0 }, /* LSB */

	/* banding step */
	{ 0x3a08, 0x00 }, /* 50HZ step MSB */
	{ 0x3a09, 0x93 }, /* 50HZ step LSB */
	{ 0x3a0a, 0x00 }, /* 60HZ step MSB */
	{ 0x3a0b, 0x7b }, /* 60HZ step LSB */
	{ 0x3a0e, 0x0d }, /* 50HZ step max */
	{ 0x3a0d, 0x10 }, /* 60HZ step max */

	{ 0x3503, 0x07 }, /* AEC disable */
	{ 0x350c, 0x00 },
	{ 0x350d, 0x00 },
	{ 0x3c07, 0x07 }, /* light meter 1 thereshold */

	{ 0x3814, 0x11 }, /* horizton subsample */
	{ 0x3815, 0x11 }, /* vertical subsample */
	{ 0x3800, 0x00 }, /* x address start high byte */
	{ 0x3801, 0x00 }, /* x address start low byte */
	{ 0x3802, 0x00 }, /* y address start high byte */
	{ 0x3803, 0x00 }, /* y address start low byte */
	{ 0x3804, 0x0a }, /* x address end high byte */
	{ 0x3805, 0x3f }, /* x address end low byte */
	{ 0x3806, 0x07 }, /* y address end high byte */
	{ 0x3807, 0x9f }, /* y address end low byte */
	{ 0x3810, 0x00 }, /* isp hortizontal offset high byte */
	{ 0x3811, 0x10 }, /* isp hortizontal offset low byte */
	{ 0x3812, 0x00 }, /* isp vertical offset high byte */
	{ 0x3813, 0x04 }, /* isp vertical offset low byte */

	{ 0x4002, 0xc5 }, /* BLC related */
	{ 0x4005, 0x1a }, /* BLC related */

	{ 0x3618, 0x04 },
	{ 0x3612, 0x2b },
	{ 0x3709, 0x12 },
	{ 0x370c, 0x00 },
	{ 0x3a02, 0x07 }, /* 60HZ max exposure limit MSB */
	{ 0x3a03, 0xb0 }, /* 60HZ max exposure limit LSB */
	{ 0x3a14, 0x07 }, /* 50HZ max exposure limit MSB */
	{ 0x3a15, 0xb0 }, /* 50HZ max exposure limit LSB */
	{ 0x4004, 0x06 }, /* BLC line number */
	{ 0x4837, 0x2c }, /* PCLK period */
	{ 0x5001, 0xa3 }, /* ISP effect */

	{ 0x302c, 0x42 }, /* bit[7:6]: output drive capability
			   * 00: 1x, 01: 2x, 10: 3x, 11: 4x */
#endif
};
#endif

static struct dev_mode ov5640_dvp_mode[] = {
	{
		.index = 0,
		.inp_fmt = VISS_MBUS_FMT_YUYV8_2X8,
		.out_fmt = VISS_PIX_FMT_NV12,
		.colorspace = DEV_COLORSPACE_JPEG,
		.frame_rate = 40000,   /* 40fps */
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
		.frame_rate = 15000,   /* 15fps */
		.frame_size.width = 640,
		.frame_size.height = 480,
		.usr_data = (void *)ov5640_yuv_vga_15fps,
		.usr_data_size = ARRAY_SIZE(ov5640_yuv_vga_15fps),
	},
	{
		.index = 2,
		.inp_fmt = VISS_MBUS_FMT_YUYV8_2X8,
		.out_fmt = VISS_PIX_FMT_NV12,
		.colorspace = DEV_COLORSPACE_JPEG,
		.frame_rate = 30000,   /* 30fps */
		.frame_size.width = 1280,
		.frame_size.height = 720,
		.usr_data = (void *)ov5640_yuv_720p,
		.usr_data_size = ARRAY_SIZE(ov5640_yuv_720p),
	},
	{
		.index = 3,
		.inp_fmt = VISS_MBUS_FMT_YUYV8_2X8,
		.out_fmt = VISS_PIX_FMT_NV12,
		.colorspace = DEV_COLORSPACE_JPEG,
		.frame_rate = 30000,   /* 30fps */
		.frame_size.width = 1920,
		.frame_size.height = 1080,
		.usr_data = (void *)ov5640_yuv_1080p,
		.usr_data_size = ARRAY_SIZE(ov5640_yuv_1080p),
	},
};

static rt_err_t __ov5640_dvp_wake_up(void *hdl)
{
	struct video_dev *ov5640 = (struct video_dev *)hdl;

	return viss_i2c_write_reg_16bit(ov5640->i2c_client, 0x3008, 0x02);
}

static rt_err_t __ov5640_block_write(void *hdl, void *data, rt_int32_t size)
{
	struct video_dev *ov5640 = (struct video_dev *)hdl;
	struct sensor_reg *reg = (struct sensor_reg *)data;
	rt_int32_t i = 0;
	rt_err_t ret = 0;
	rt_int32_t delay = 0, temp = 0;

	for (i = 0; i < size; i++) {
		if (OV5640_DVP_REG_DELAY_FLAG == reg[i].reg_add) {
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
			LOG_E("\n");
			return ret;
		}
	}

	return RT_EOK;
}

static struct dev_mode *__ov5640_cur_mode(void *hdl)
{
	struct video_dev *ov5640 = (struct video_dev *)hdl;

	return ov5640->cur_mode;
}

static struct dev_mode *__ov5640_get_all_mode(void *hdl, rt_int32_t *num)
{
	RT_ASSERT(RT_NULL != num);
	*num = (rt_int32_t)ARRAY_SIZE(ov5640_dvp_mode);

	return ov5640_dvp_mode;
}

static rt_err_t __ov5640_set_mode(void *hdl, rt_int32_t index)
{
	struct video_dev *ov5640 = (struct video_dev *)hdl;
	rt_int32_t num = 0;
	rt_err_t ret = 0;

	num = (rt_int32_t)ARRAY_SIZE(ov5640_dvp_mode);
	if (index >= num) {
		LOG_E("Input mode index error. num:%d index:%d\n", num, index);
		return -RT_EINVAL;
	}
	if (index >= ARRAY_SIZE(ov5640_dvp_mode)) {
		LOG_E("Input para error. index:%d\n", index);
		return -RT_EINVAL;
	}
	LOG_D("set_mode index:%d", index);
	ov5640->cur_mode = &ov5640_dvp_mode[index];
	ret = __ov5640_block_write((void *)ov5640, ov5640->cur_mode->usr_data,
			ov5640->cur_mode->usr_data_size);
	if (RT_EOK != ret)
		return ret;

	return __ov5640_dvp_wake_up((void *)ov5640);
}


static rt_err_t __ov5640_set_register(void *hdl, struct viss_dbg_register *reg)
{
	return RT_EOK;
}

static rt_err_t __ov5640_get_register(void *hdl, struct viss_dbg_register *reg)
{
	return RT_EOK;
}

static rt_err_t __ov5640_set_power(void *hdl, rt_bool_t on)
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

static rt_err_t __ov5640_set_stream(void *hdl, rt_bool_t enable)
{
	return RT_EOK;
}


static rt_err_t __ov5640_ioctl(void *hdl, rt_int32_t cmd, void *para)
{
	rt_err_t ret = 0;

	return ret;
}

static rt_err_t __ov5640_get_info(void *hdl, struct viss_source_info *info)
{
	struct video_dev *ov5640 = (struct video_dev *)hdl;

	if ((RT_NULL == hdl) || (RT_NULL == info)) {
		LOG_E("Get ov5640 information fail.");
		return -RT_ERROR;
	}
	rt_memcpy(info, &ov5640->info, sizeof(struct viss_source_info));

	return RT_EOK;
}

static rt_err_t __ov5640_parser_config(void *hdl)
{
	struct video_dev *ov5640 = (struct video_dev *)hdl;
	struct viss_source_info info;
	rt_uint32_t tmp_data = 0;
	const char *status;
	const char *i2c_bus_name;
	const char *module = DRV_VIC_0V5640_NAME;
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
		LOG_W("Ov5640 dvp disable.");
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
	rt_memcpy(&ov5640->info, &info, sizeof(struct viss_source_info));

	/* power pin */
	ov5640->pwdn_valid = 1;
	ret = config_get_u32_array(module, "dvp-pwdn",
				ov5640->pwdn_val, ARRAY_SIZE(ov5640->pwdn_val));
	if (ret != ARRAY_SIZE(ov5640->pwdn_val)) {
		LOG_E("vic: power pin config error. ret:%d", ret);
		ov5640->pwdn_valid = 0;
	}

	/* reset pin */
	ov5640->rst_valid = 1;
	ret = config_get_u32_array(module, "dvp-rst",
				ov5640->rst_val, ARRAY_SIZE(ov5640->rst_val));
	if (ret != ARRAY_SIZE(ov5640->rst_val)) {
		LOG_E("vic: reset pin config error. ret:%d", ret);
		ov5640->rst_valid = 0;
	}

	/* mclk pin */
	ov5640->mclk_valid = 1;
	ret = config_get_u32_array(module, "dvp-mclk",
				ov5640->mclk_val, ARRAY_SIZE(ov5640->mclk_val));
	if (ret != ARRAY_SIZE(ov5640->mclk_val)) {
		LOG_E("vic: mclk pin config error. ret:%d", ret);
		ov5640->mclk_valid = 0;
	}

	return RT_EOK;
}

static rt_err_t __ov5640_prepare(void *hdl)
{
	return __ov5640_parser_config(hdl);
}

static rt_err_t __ov5640_init(void *hdl)
{
	rt_int32_t ret = 0;
	rt_uint8_t tmp[2] = {0};
	rt_uint16_t id = 0;
	struct video_dev *ov5640 = (struct video_dev *)hdl;

	RT_ASSERT(RT_NULL != ov5640);
	ov5640->pctrl = pinctrl_get(DRV_VIC_0V5640_NAME);
	if (RT_NULL == ov5640->pctrl)
		return -RT_ERROR;
	if (1 == ov5640->pwdn_valid) {
		ov5640->pwdn_gpio = pinctrl_gpio_request(ov5640->pctrl,
					ov5640->pwdn_val[0], ov5640->pwdn_val[1]);
		if (ov5640->pwdn_gpio >= 0) {
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
	__ov5640_set_power(ov5640, RT_TRUE);

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
	__ov5640_block_write((void *)ov5640, (void *)ov5640_reg_list,
			ARRAY_SIZE(ov5640_reg_list));
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

static void __ov5640_exit(void *hdl)
{
	struct video_dev *ov5640 = (struct video_dev *)hdl;

	__ov5640_set_power(ov5640, RT_FALSE);
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

struct video_dev ov5640_dvp = {
	.name = DRV_VIC_0V5640_NAME,
	.group_id = GRP_ID_VIC,
	.prepare = __ov5640_prepare,
	.init = __ov5640_init,
	.exit = __ov5640_exit,
	.s_mode = __ov5640_set_mode,
	.g_cur_mode = __ov5640_cur_mode,
	.g_all_mode = __ov5640_get_all_mode,
	.s_power = __ov5640_set_power,
	.s_stream = __ov5640_set_stream,
	.g_info = __ov5640_get_info,
	.ioctl = __ov5640_ioctl,
	.s_register = __ov5640_set_register,
	.g_register = __ov5640_get_register,
};

