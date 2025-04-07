/*
 * tp9930_dvp.c - tp9930 driver code for LomboTech
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

#define DBG_SECTION_NAME	"TP9930-DVP"
#define DBG_LEVEL		DBG_LOG

#include <debug.h>
#include "viss.h"
#include "vic_dev.h"
#include "viss_i2c.h"
#include "viss_cam_power.h"
#include <div.h>

#if defined(ARCH_LOMBO_N7V1_SAR) && defined(RT_USING_VIC_DET_SIGNAL)
#include "system/system_mq.h"
#define ch_num	4
static rt_uint8_t tp9930_in[ch_num];
#endif


struct sensor_reg {
	rt_uint16_t reg_add;
	rt_uint16_t reg_value;
};

#define DRV_VIC_TP9930_NAME "tp9930-dvp"
#ifdef ARCH_LOMBO_N7V1_SAR
#define DRV_VIC_TP9930_PWR_NAME0 "ahd-pwr-en0"
#define DRV_VIC_TP9930_PWR_NAME1 "ahd-pwr-en1"
#endif
#define DRV_VIC_TP9930_PWR_NAME "ahd-pwr-en"
#define TP9930_DVP_REG_DELAY_FLAG  (0xffff)
static const struct sensor_reg tp9930_yuv_1080p_30fps[] = {

	{ 0x40, 0x04 },
	{ 0x3b, 0x20 },
	{ 0x3d, 0xe0 },
	{ 0x3d, 0x60 },
	{ 0x3b, 0x25 },
	{ 0x40, 0x40 },
	{ 0x7a, 0x20 },
	{ 0x3c, 0x20 },
	{ 0x3c, 0x00 },
	{ 0x7a, 0x25 },
	{ 0x40, 0x00 },
	{ 0x44, 0x57 },
	{ 0x43, 0x12 },
	{ 0x45, 0x09 },
	{ 0xf4, 0x80 },
	{ 0x44, 0x17 },
	/* video setting */
	{ 0x40, 0x04 },
	{ 0x02, 0x44 },
	{ 0x07, 0x80 },
	{ 0x0b, 0xc0 },
	{ 0x0c, 0x03 },
	{ 0x0d, 0x72 },
	{ 0x10, 0x00 },
	{ 0x11, 0x40 },
	{ 0x12, 0x40 },
	{ 0x13, 0x00 },
	{ 0x14, 0x00 },
	{ 0x15, 0x01 },
	{ 0x16, 0xf0 },
	{ 0x17, 0x80 },
	{ 0x18, 0x29 },
	{ 0x19, 0x38 },
	{ 0x1a, 0x47 },
	{ 0x1c, 0x08 },
	{ 0x1d, 0x98 },
	{ 0x20, 0x38 },
	{ 0x21, 0x46 },
	{ 0x22, 0x36 },
	{ 0x23, 0x3c },
	{ 0x24, 0x04 },
	{ 0x25, 0xfe },
	{ 0x26, 0x0d },
	{ 0x27, 0x2d },
	{ 0x28, 0x00 },
	{ 0x29, 0x48 },
	{ 0x2a, 0x30 },//{ 0x2a, 0x3c },
	{ 0x2b, 0x60 },
	{ 0x2c, 0x3a },
	{ 0x2d, 0x54 },
	{ 0x2e, 0x40 },
	{ 0x30, 0xa5 },
	{ 0x31, 0x95 },
	{ 0x32, 0xe0 },
	{ 0x33, 0x60 },
	{ 0x35, 0x05 },
	{ 0x36, 0xca },
	{ 0x38, 0x00 },
	{ 0x39, 0x1c },
	{ 0x3a, 0x32 },
	{ 0x3b, 0x26 },
	/* cha nnel ID */
	{ 0x40, 0x00 },
	{ 0x34, 0x10 },
	{ 0x40, 0x01 },
	{ 0x34, 0x11 },
	{ 0x40, 0x02 },
	{ 0x34, 0x10 },
	{ 0x40, 0x03 },
	{ 0x34, 0x11 },
	/* output format */
	{ 0x4f, 0x03 },
	{ 0x50, 0x00 },
	{ 0x52, 0x00 },
	{ 0xf1, 0x04 },
	{ 0xf2, 0x77 },
	{ 0xf3, 0x77 },
	{ 0xf5, 0xF0 },
	{ 0xf6, 0x01 },
	{ 0xf8, 0x45 },
	{ 0xfa, 0x88 },
	{ 0xfb, 0x88 },
	/* output enable */
	/* { 0x4d, 0x07 }, */
	/* { 0x4e, 0x05 }, */

#if 0
	{ 0x40, 0x04 },
	{ 0x3b, 0x20 },
	{ 0x3d, 0xe0 },
	{ 0x3d, 0x60 },
	{ 0x3b, 0x25 },
	{ 0x40, 0x40 },
	{ 0x7a, 0x20 },
	{ 0x3c, 0x20 },
	{ 0x3c, 0x00 },
	{ 0x7a, 0x25 },
	{ 0x40, 0x00 },
	{ 0x44, 0x07 },
	{ 0x43, 0x12 },
	{ 0x45, 0x09 },
	{ 0xf4, 0xa0 },
	{ 0x44, 0x17 },
	/* video setting */
	{ 0x40, 0x04 },
	{ 0x02, 0x44 },
	{ 0x05, 0x00 },
	{ 0x06, 0x32 },
	{ 0x07, 0x80 },
	{ 0x08, 0x00 },
	{ 0x09, 0x24 },
	{ 0x0a, 0x48 },
	{ 0x0b, 0xc0 },
	{ 0x0c, 0x03 },
	{ 0x0d, 0x72 },
	{ 0x10, 0x00 },
	{ 0x11, 0x40 },
	{ 0x12, 0x40 },
	{ 0x13, 0x00 },
	{ 0x14, 0x00 },
	{ 0x15, 0x01 },
	{ 0x16, 0xf0 },
	{ 0x17, 0x80 },
	{ 0x18, 0x29 },
	{ 0x19, 0x38 },
	{ 0x1a, 0x47 },
	{ 0x1c, 0x08 },
	{ 0x1d, 0x98 },
	{ 0x20, 0x38 },
	{ 0x21, 0x46 },
	{ 0x22, 0x36 },
	{ 0x23, 0x3c },
	{ 0x24, 0x04 },
	{ 0x25, 0xfe },
	{ 0x26, 0x0d },
	{ 0x27, 0x2d },
	{ 0x28, 0x00 },
	{ 0x29, 0x48 },
	{ 0x2a, 0x30 },
	{ 0x2b, 0x60 },
	{ 0x2c, 0x3a },
	{ 0x2d, 0x54 },
	{ 0x2e, 0x40 },
	{ 0x30, 0xa5 },
	{ 0x31, 0x95 },
	{ 0x32, 0xe0 },
	{ 0x33, 0x60 },
	{ 0x35, 0x05 },
	{ 0x36, 0xca },
	{ 0x38, 0x00 },
	{ 0x39, 0x1c },
	{ 0x3a, 0x32 },
	{ 0x3b, 0x26 },
	/* cha nnel ID */
	{ 0x40, 0x00 },
	{ 0x34, 0x10 },
	{ 0x40, 0x01 },
	{ 0x34, 0x11 },
	{ 0x40, 0x02 },
	{ 0x34, 0x12 },
	{ 0x40, 0x03 },
	{ 0x34, 0x13 },
	/* output format */
	{ 0x4f, 0x03 },
	{ 0x50, 0x81 },
	{ 0x52, 0xc5 },
	{ 0xf1, 0x04 },
	{ 0xf2, 0x77 },
	{ 0xf3, 0x77 },
	{ 0xf5, 0xF0 },
	{ 0xf6, 0x10 },
	{ 0xf8, 0x54 },
	{ 0xfa, 0x88 },
	{ 0xfb, 0x88 },
	/* output enable */
	/* { 0x4d, 0x07 }, */
	/* { 0x4e, 0x05 }, */
#endif
};

static const struct sensor_reg tp9930_yuv_1080p_25fps[] = {

	{ 0x40, 0x04 },
	{ 0x3b, 0x20 },
	{ 0x3d, 0xe0 },
	{ 0x3d, 0x60 },
	{ 0x3b, 0x25 },
	{ 0x40, 0x40 },
	{ 0x7a, 0x20 },
	{ 0x3c, 0x20 },
	{ 0x3c, 0x00 },
	{ 0x7a, 0x25 },
	{ 0x40, 0x00 },
	{ 0x44, 0x57 },
	{ 0x43, 0x12 },
	{ 0x45, 0x09 },
	{ 0xf4, 0x80 },
	{ 0x44, 0x17 },


	//video setting
	{ 0x40, 0x04},
	{ 0x02, 0x44},
	{ 0x07, 0x80},
	{ 0x0b, 0xc0},
	{ 0x0c, 0x03},
	{ 0x0d, 0x73},
	{ 0x10, 0x00},
	{ 0x11, 0x40},
	{ 0x12, 0x40},
	{ 0x13, 0x00},
	{ 0x14, 0x00},
	{ 0x15, 0x01},
	{ 0x16, 0xec},//0xf0
	{ 0x17, 0x80},
	{ 0x18, 0x29},
	{ 0x19, 0x38},
	{ 0x1a, 0x47},
	{ 0x1c, 0x0a},
	{ 0x1d, 0x50},
	{ 0x20, 0x3c},
	{ 0x21, 0x46},
	{ 0x22, 0x36},
	{ 0x23, 0x3c},
	{ 0x24, 0x04},
	{ 0x25, 0xfe},
	{ 0x26, 0x0d},
	{ 0x27, 0x2d},
	{ 0x28, 0x00},
	{ 0x29, 0x48},
	{ 0x2a, 0x30},
	{ 0x2b, 0x60},
	{ 0x2c, 0x3a},
	{ 0x2d, 0x54},
	{ 0x2e, 0x40},
	{ 0x30, 0xa5},
	{ 0x31, 0x86},
	{ 0x32, 0xfb},
	{ 0x33, 0x60},
	{ 0x35, 0x05},
	{ 0x36, 0xca},
	{ 0x38, 0x00},
	{ 0x39, 0x1c},
	{ 0x3a, 0x32},
	{ 0x3b, 0x26},

	//channel ID
	{ 0x40, 0x00},
	{ 0x34, 0x10},
	{ 0x40, 0x01},
	{ 0x34, 0x11},
	{ 0x40, 0x02},
	{ 0x34, 0x10},
	{ 0x40, 0x03},
	{ 0x34, 0x11},
	//output format
	{ 0x4f, 0x03},
	{ 0x50, 0x00},
	{ 0x52, 0x00},
	{ 0xf1, 0x04},
	{ 0xf2, 0x77},
	{ 0xf3, 0x77},
	{ 0xf5, 0xF0},
	{ 0xf6, 0x01},
	{ 0xf8, 0x45},
	{ 0xfa, 0x88},
	{ 0xfb, 0x88},
	//output enable
	//4d:07
	//4e:05

};
#ifdef ARCH_LOMBO_N7V1_SAR
static const struct sensor_reg tp9930_yuv_720p_30fps[] = {

	{ 0x40, 0x04 },
	{ 0x3b, 0x20 },
	{ 0x3d, 0xe0 },
	{ 0x3d, 0x60 },
	{ 0x3b, 0x25 },
	{ 0x40, 0x40 },
	{ 0x7a, 0x20 },
	{ 0x3c, 0x20 },
	{ 0x3c, 0x00 },
	{ 0x7a, 0x25 },
	{ 0x40, 0x00 },
	{ 0x44, 0x17 },
	{ 0x43, 0x12 },
	{ 0x45, 0x09 },
	/* video setting */
	{ 0x40, 0x04 },
	{ 0x02, 0x4e },
	{ 0x05, 0x00 },
	{ 0x06, 0x32 },
	{ 0x07, 0xc0 },
	{ 0x08, 0x00 },
	{ 0x09, 0x24 },
	{ 0x0a, 0x48 },
	{ 0x0b, 0xc0 },
	{ 0x0c, 0x13 },
	{ 0x0d, 0x70 },
	{ 0x0e, 0x00 },
	{ 0x0f, 0x00 },
	{ 0x10, 0x00 },
	{ 0x11, 0x40 },
	{ 0x12, 0x40 },
	{ 0x13, 0x00 },
	{ 0x14, 0x00 },
	{ 0x15, 0x13 },
	{ 0x16, 0x16 },
	{ 0x17, 0x00 },
	{ 0x18, 0x19 },
	{ 0x19, 0xd0 },
	{ 0x1a, 0x25 },
	{ 0x1b, 0x00 },
	{ 0x1c, 0x06 },
	{ 0x1d, 0x72 },
	{ 0x1e, 0x60 },
	{ 0x1f, 0x06 },
	{ 0x20, 0x40 },
	{ 0x21, 0x46 },
	{ 0x22, 0x36 },
	{ 0x23, 0x3c },
	{ 0x24, 0x04 },
	{ 0x25, 0xfe },
	{ 0x26, 0x01 },
	{ 0x27, 0x2d },
	{ 0x28, 0x04 },
	{ 0x29, 0x48 },
	{ 0x2a, 0x30 },
	{ 0x2b, 0x60 },
	{ 0x2c, 0x3a },
	{ 0x2d, 0x5a },
	{ 0x2e, 0x40 },
	{ 0x2f, 0x06 },
	{ 0x30, 0x9d },
	{ 0x31, 0xca },
	{ 0x32, 0x01 },
	{ 0x33, 0xd0 },
	{ 0x35, 0x25 },
	{ 0x36, 0xca },
	{ 0x37, 0x00 },
	{ 0x38, 0x00 },
	{ 0x39, 0x18 },
	{ 0x3a, 0x32 },
	{ 0x3b, 0x26 },
	{ 0x3c, 0x00 },
	{ 0x3d, 0x60 },
	{ 0x3e, 0x00 },
	{ 0x3f, 0x00 },
	/* channel ID */
	{ 0x40, 0x00 },
	{ 0x34, 0x10 },
	{ 0x40, 0x01 },
	{ 0x34, 0x11 },
	{ 0x40, 0x02 },
	{ 0x34, 0x12 },
	{ 0x3d, 0xff },
	{ 0x40, 0x03 },
	{ 0x34, 0x13 },
	{ 0x3d, 0xff },
	/* output format */
	{ 0x4f, 0x03 },
	{ 0x50, 0x81 },
	{ 0x52, 0xc5 },
	{ 0xf1, 0x04 },
	{ 0xf2, 0x77 },
	{ 0xf3, 0x77 },
	{ 0xf4, 0x00 },
	{ 0xf5, 0x0f },
	{ 0xf6, 0x10 },
	{ 0xf8, 0x54 },
	{ 0xfa, 0x99 },
	{ 0xfb, 0x99 },
	{ 0xf4, 0x80 },
	/* output enable */
	/* { 0x4d, 0x07 }, */
	/* { 0x4e, 0x05 }, */
};
/*2 channel 16bit bus-width bt1120*/
static const struct sensor_reg tp9930_yuv_720p_25fps[] = {

	{ 0x40, 0x04 },
	{ 0x3b, 0x20 },
	{ 0x3d, 0xe0 },
	{ 0x3d, 0x60 },
	{ 0x3b, 0x25 },
	{ 0x40, 0x40 },
	{ 0x7a, 0x20 },
	{ 0x3c, 0x20 },
	{ 0x3c, 0x00 },
	{ 0x7a, 0x25 },
	{ 0x40, 0x00 },
	{ 0x44, 0x17 },
	{ 0x43, 0x12 },
	{ 0x45, 0x09 },
	/* video setting */
	{ 0x40, 0x04 },
	{ 0x02, 0x4e },
	{ 0x05, 0x00 },
	{ 0x06, 0x32 },
	{ 0x07, 0xc0 },
	{ 0x08, 0x00 },
	{ 0x09, 0x24 },
	{ 0x0a, 0x48 },
	{ 0x0b, 0xc0 },
	{ 0x0c, 0x13 },
	{ 0x0d, 0x71 },
	{ 0x0e, 0x00 },
	{ 0x0f, 0x00 },
	{ 0x10, 0x00 },
	{ 0x11, 0x40 },
	{ 0x12, 0x40 },
	{ 0x13, 0x00 },
	{ 0x14, 0x00 },
	{ 0x15, 0x13 },
	{ 0x16, 0x16 },
	{ 0x17, 0x00 },
	{ 0x18, 0x19 },
	{ 0x19, 0xd0 },
	{ 0x1a, 0x25 },
	{ 0x1b, 0x00 },
	{ 0x1c, 0x07 },
	{ 0x1d, 0xbc },
	{ 0x1e, 0x60 },
	{ 0x1f, 0x06 },
	{ 0x20, 0x40 },
	{ 0x21, 0x46 },
	{ 0x22, 0x36 },
	{ 0x23, 0x3c },
	{ 0x24, 0x04 },
	{ 0x25, 0xfe },
	{ 0x26, 0x01 },
	{ 0x27, 0x2d },
	{ 0x28, 0x04 },
	{ 0x29, 0x48 },
	{ 0x2a, 0x30 },
	{ 0x2b, 0x60 },
	{ 0x2c, 0x3a },
	{ 0x2d, 0x5a },
	{ 0x2e, 0x40 },
	{ 0x2f, 0x06 },
	{ 0x30, 0x9e },
	{ 0x31, 0x20 },
	{ 0x32, 0x01 },
	{ 0x33, 0x90 },
	{ 0x35, 0x25 },
	{ 0x36, 0xca },
	{ 0x37, 0x00 },
	{ 0x38, 0x00 },
	{ 0x39, 0x18 },
	{ 0x3a, 0x32 },
	{ 0x3b, 0x26 },
	{ 0x3c, 0x00 },
	{ 0x3d, 0x60 },
	{ 0x3e, 0x00 },
	{ 0x3f, 0x00 },
#if 0
	/* 30fps */
	/* only init channel 1 */
	{ 0x40, 0x01 },
	{ 0x02, 0x4e },
	{ 0x05, 0x00 },
	{ 0x06, 0x32 },
	{ 0x07, 0xc0 },
	{ 0x08, 0x00 },
	{ 0x09, 0x24 },
	{ 0x0a, 0x48 },
	{ 0x0b, 0xc0 },
	{ 0x0c, 0x13 },
	{ 0x0d, 0x70 },
	{ 0x0e, 0x00 },
	{ 0x0f, 0x00 },
	{ 0x10, 0x00 },
	{ 0x11, 0x40 },
	{ 0x12, 0x40 },
	{ 0x13, 0x00 },
	{ 0x14, 0x00 },
	{ 0x15, 0x13 },
	{ 0x16, 0x16 },
	{ 0x17, 0x00 },
	{ 0x18, 0x19 },
	{ 0x19, 0xd0 },
	{ 0x1a, 0x25 },
	{ 0x1b, 0x00 },
	{ 0x1c, 0x06 },
	{ 0x1d, 0x72 },
	{ 0x1e, 0x60 },
	{ 0x1f, 0x06 },
	{ 0x20, 0x40 },
	{ 0x21, 0x46 },
	{ 0x22, 0x36 },
	{ 0x23, 0x3c },
	{ 0x24, 0x04 },
	{ 0x25, 0xfe },
	{ 0x26, 0x01 },
	{ 0x27, 0x2d },
	{ 0x28, 0x00 },
	{ 0x29, 0x48 },
	{ 0x2a, 0x30 },
	{ 0x2b, 0x60 },
	{ 0x2c, 0x3a },
	{ 0x2d, 0x5a },
	{ 0x2e, 0x40 },
	{ 0x2f, 0x06 },
	{ 0x30, 0x9d },
	{ 0x31, 0xca },
	{ 0x32, 0x01 },
	{ 0x33, 0xd0 },
	{ 0x35, 0x25 },
	{ 0x36, 0xca },
	{ 0x37, 0x00 },
	{ 0x38, 0x00 },
	{ 0x39, 0x18 },
	{ 0x3a, 0x32 },
	{ 0x3b, 0x26 },
	{ 0x3c, 0x00 },
	{ 0x3d, 0x60 },
	{ 0x3e, 0x00 },
	{ 0x3f, 0x00 },
#endif
	/* channel ID */
	{ 0x40, 0x00 },
	{ 0x34, 0x10 },
	{ 0x40, 0x01 },
	{ 0x34, 0x11 },
	{ 0x40, 0x02 },
	{ 0x34, 0x12 },
	{ 0x3d, 0xff },
	{ 0x40, 0x03 },
	{ 0x34, 0x13 },
	{ 0x3d, 0xff },
	/* output format */
	{ 0x4f, 0x03 },
	{ 0x50, 0x81 },
	{ 0x52, 0xc5 },
	{ 0xf1, 0x04 },
	{ 0xf2, 0x77 },
	{ 0xf3, 0x77 },
	{ 0xf4, 0x00 },
	{ 0xf5, 0x0f },
	{ 0xf6, 0x10 },
	{ 0xf8, 0x54 },
	{ 0xfa, 0x99 },
	{ 0xfb, 0x99 },
	{ 0xf4, 0x80 },
	/* output enable */
	/* { 0x4d, 0x07 }, */
	/* { 0x4e, 0x05 }, */
};
#endif
#if 0
/*2 channel 8bit bus-width bt656*/
static const struct sensor_reg tp9930_yuv_720p_25fps[] = {

	{ 0x40, 0x04 },
	{ 0x3b, 0x20 },
	{ 0x3d, 0xe0 },
	{ 0x3d, 0x60 },
	{ 0x40, 0x40 },
	{ 0x7a, 0x20 },
	{ 0x3c, 0x20 },
	{ 0x3c, 0x00 },
	{ 0x7a, 0x25 },
	{ 0x40, 0x00 },
	{ 0x44, 0x17 },
	{ 0x43, 0x12 },
	{ 0x45, 0x09 },
	/* video setting */
	{ 0x40, 0x04 },
	{ 0x02, 0xce },
	{ 0x05, 0x00 },
	{ 0x06, 0x32 },
	{ 0x07, 0xc0 },
	{ 0x08, 0x00 },
	{ 0x09, 0x24 },
	{ 0x0a, 0x48 },
	{ 0x0b, 0xc0 },
	{ 0x0c, 0x13 },
	{ 0x0d, 0x71 },
	{ 0x0e, 0x00 },
	{ 0x0f, 0x00 },
	{ 0x10, 0x00 },
	{ 0x11, 0x40 },
	{ 0x12, 0x40 },
	{ 0x13, 0x00 },
	{ 0x14, 0x00 },
	{ 0x15, 0x13 },
	{ 0x16, 0x16 },
	{ 0x17, 0x00 },
	{ 0x18, 0x19 },
	{ 0x19, 0xd0 },
	{ 0x1a, 0x25 },
	{ 0x1b, 0x00 },
	{ 0x1c, 0x07 },
	{ 0x1d, 0xbc },
	{ 0x1e, 0x60 },
	{ 0x1f, 0x06 },
	{ 0x20, 0x40 },
	{ 0x21, 0x46 },
	{ 0x22, 0x36 },
	{ 0x23, 0x3c },
	{ 0x24, 0x04 },
	{ 0x25, 0xfe },
	{ 0x26, 0x01 },
	{ 0x27, 0x2d },
	{ 0x28, 0x00 },
	{ 0x29, 0x48 },
	{ 0x2a, 0x30 },
	{ 0x2b, 0x60 },
	{ 0x2c, 0x3a },
	{ 0x2d, 0x5a },
	{ 0x2e, 0x40 },
	{ 0x2f, 0x06 },
	{ 0x30, 0x9e },
	{ 0x31, 0x20 },
	{ 0x32, 0x01 },
	{ 0x33, 0x90 },
	{ 0x35, 0x25 },
	{ 0x36, 0xca },
	{ 0x37, 0x00 },
	{ 0x38, 0x00 },
	{ 0x39, 0x18 },
	{ 0x3a, 0x32 },
	{ 0x3b, 0x26 },
	{ 0x3c, 0x00 },
	{ 0x3d, 0x60 },
	{ 0x3e, 0x00 },
	{ 0x3f, 0x00 },
	/* channel ID */
	{ 0x40, 0x00 },
	{ 0x34, 0x10 },
	{ 0x40, 0x01 },
	{ 0x34, 0x11 },
	{ 0x40, 0x02 },
	{ 0x34, 0x10 },
	{ 0x40, 0x03 },
	{ 0x34, 0x11 },
	/* output format */
	{ 0x4f, 0x03 },
	{ 0x50, 0x00 },
	{ 0x52, 0x00 },
	{ 0xf1, 0x04 },
	{ 0xf2, 0x77 },
	{ 0xf3, 0x77 },
	{ 0xf4, 0x00 },
	{ 0xf5, 0x0f },
	{ 0xf6, 0x10 },
	{ 0xf8, 0x23 },
	{ 0xfa, 0x88 },
	{ 0xfb, 0x88 },
	/* output enable */
	/* { 0x4d, 0x07 }, */
	/* { 0x4e, 0x05 }, */
};
#endif
#ifdef ARCH_LOMBO_N7V1_TDR
/*4 channel 16bit bus-width bt1120*/
static const struct sensor_reg tp9930_yuv_720p_25fps[] = {

	{ 0x40, 0x04 },
	{ 0x3b, 0x20 },
	{ 0x3d, 0xe0 },
	{ 0x3d, 0x60 },
	{ 0x40, 0x40 },
	{ 0x7a, 0x20 },
	{ 0x3c, 0x20 },
	{ 0x3c, 0x00 },
	{ 0x7a, 0x25 },
	{ 0x40, 0x00 },
	{ 0x44, 0x17 },
	{ 0x43, 0x12 },
	{ 0x45, 0x09 },
	/* video setting */
	{ 0x40, 0x04 },
	{ 0x02, 0x4e },
	{ 0x05, 0x00 },
	{ 0x06, 0x32 },
	{ 0x07, 0xc0 },
	{ 0x08, 0x00 },
	{ 0x09, 0x24 },
	{ 0x0a, 0x48 },
	{ 0x0b, 0xc0 },
	{ 0x0c, 0x13 },
	{ 0x0d, 0x71 },
	{ 0x0e, 0x00 },
	{ 0x0f, 0x00 },
	{ 0x10, 0x00 },
	{ 0x11, 0x40 },
	{ 0x12, 0x40 },
	{ 0x13, 0x00 },
	{ 0x14, 0x00 },
	{ 0x15, 0x13 },
	{ 0x16, 0x16 },
	{ 0x17, 0x00 },
	{ 0x18, 0x19 },
	{ 0x19, 0xd0 },
	{ 0x1a, 0x25 },
	{ 0x1b, 0x00 },
	{ 0x1c, 0x07 },
	{ 0x1d, 0xbc },
	{ 0x1e, 0x60 },
	{ 0x1f, 0x06 },
	{ 0x20, 0x40 },
	{ 0x21, 0x46 },
	{ 0x22, 0x36 },
	{ 0x23, 0x3c },
	{ 0x24, 0x04 },
	{ 0x25, 0xfe },
	{ 0x26, 0x01 },
	{ 0x27, 0x2d },
	{ 0x28, 0x00 },
	{ 0x29, 0x48 },
	{ 0x2a, 0x34 },
	{ 0x2b, 0x60 },
	{ 0x2c, 0x3a },
	{ 0x2d, 0x5a },
	{ 0x2e, 0x40 },
	{ 0x2f, 0x06 },
	{ 0x30, 0x9e },
	{ 0x31, 0x20 },
	{ 0x32, 0x01 },
	{ 0x33, 0x90 },
	{ 0x35, 0x25 },
	{ 0x36, 0xca },
	{ 0x37, 0x00 },
	{ 0x38, 0x00 },
	{ 0x39, 0x18 },
	{ 0x3a, 0x32 },
	{ 0x3b, 0x26 },
	{ 0x3c, 0x00 },
	{ 0x3d, 0x60 },
	{ 0x3e, 0x00 },
	{ 0x3f, 0x00 },
	/* channel ID */
	{ 0x40, 0x00 },
	{ 0x34, 0x10 },
	{ 0x40, 0x01 },
	{ 0x34, 0x11 },
	{ 0x40, 0x02 },
	{ 0x34, 0x12 },
	{ 0x40, 0x03 },
	{ 0x34, 0x13 },
	/* output format */
	{ 0x4f, 0x03 },
	{ 0x50, 0xA3 },
	{ 0x52, 0xE7 },
	{ 0xf1, 0x04 },
	{ 0xf2, 0x77 },
	{ 0xf3, 0x77 },
	{ 0xf4, 0x00 },
	{ 0xf5, 0x0f },
	{ 0xf6, 0x10 },
	{ 0xf8, 0x54 },
	{ 0xfa, 0x88 },
	{ 0xfb, 0x88 },
	{ 0xf4, 0x80 },
	/* output enable */
	/* { 0x4d, 0x07 }, */
	/* { 0x4e, 0x05 }, */
};
#endif
static struct dev_mode tp9930_dvp_mode[] = {
#if 0
#if defined(ARCH_LOMBO_N7V1_TDR) || defined(ARCH_LOMBO_N7V1_SAR)
	{
		.index = 0,
		.inp_fmt = VISS_MBUS_FMT_YUYV8_2X8,
		.out_fmt = VISS_PIX_FMT_NV12,
		.colorspace = DEV_COLORSPACE_JPEG,
		/* .frame_rate = 25000,   25fps */
		.frame_size.width = 1280,
		.frame_size.height = 720,
#ifdef ARCH_LOMBO_N7V1_SAR
		.frame_rate = 25000,   /* 25fps */
		.usr_data = (void *)tp9930_yuv_720p_25fps,
		.usr_data_size = ARRAY_SIZE(tp9930_yuv_720p_25fps),
#else
		.frame_rate = 25000,   /* 25fps */
		.usr_data = (void *)tp9930_yuv_720p_25fps,
		.usr_data_size = ARRAY_SIZE(tp9930_yuv_720p_25fps),
#endif
	},
#endif
#endif
#if 1
	{
		.index = 0,
		.inp_fmt = VISS_MBUS_FMT_YUYV8_2X8,//VISS_MBUS_FMT_YUYV8_2X8,
		.out_fmt = VISS_PIX_FMT_NV12,
		.colorspace = DEV_COLORSPACE_JPEG,
		.frame_rate = 30000,
		.frame_size.width = 1920,
		.frame_size.height = 1080,
		.usr_data = (void *)tp9930_yuv_1080p_30fps,
		.usr_data_size = ARRAY_SIZE(tp9930_yuv_1080p_30fps),
	},
#endif
#if 1
	{
		.index = 1,
		.inp_fmt = VISS_MBUS_FMT_YUYV8_2X8,//VISS_MBUS_FMT_YUYV8_2X8,
		.out_fmt = VISS_PIX_FMT_NV12,
		.colorspace = DEV_COLORSPACE_JPEG,
		.frame_rate = 25000,
		.frame_size.width = 1920,
		.frame_size.height = 1080,
		.usr_data = (void *)tp9930_yuv_1080p_25fps,
		.usr_data_size = ARRAY_SIZE(tp9930_yuv_1080p_25fps),
	},
#endif
};

static rt_err_t __tp9930_block_write(void *hdl, void *data, rt_int32_t size)
{
	struct video_dev *tp9930 = (struct video_dev *)hdl;
	struct sensor_reg *reg = (struct sensor_reg *)data;
	rt_int32_t i = 0;
	rt_err_t ret = 0;
	rt_uint8_t tmp[2] = {0};
	rt_int32_t delay = 0, temp = 0;
	/* TP9930_PLL_Reset */
	ret = viss_i2c_write_reg_8bit(tp9930->i2c_client, 0x40, 0x00);
	if (RT_EOK != ret) {
		LOG_E("reg[i].reg_add : %x\n", reg[i].reg_add);
		return ret;
	}
	/* output disable */
	ret = viss_i2c_write_reg_8bit(tp9930->i2c_client, 0x4d, 0x00);
	if (RT_EOK != ret) {
		LOG_E("reg[i].reg_add : %x\n", reg[i].reg_add);
		return ret;
	}
	ret = viss_i2c_write_reg_8bit(tp9930->i2c_client, 0x4e, 0x00);
	if (RT_EOK != ret) {
		LOG_E("reg[i].reg_add : %x\n", reg[i].reg_add);
		return ret;
	}
	/* PLL reset */
	ret = viss_i2c_read_reg_8bit(tp9930->i2c_client, 0x44, &tmp[0]);
	ret = viss_i2c_write_reg_8bit(tp9930->i2c_client, 0x44, tmp[0] | 0x40);
	if (RT_EOK != ret) {
		LOG_E("reg[i].reg_add : %x\n", 0x40);
		return ret;
	}
	ret = viss_i2c_read_reg_8bit(tp9930->i2c_client, 0xf4, &tmp[1]);
	ret = viss_i2c_write_reg_8bit(tp9930->i2c_client, 0xf4, tmp[1] | 0x80);
	if (RT_EOK != ret) {
		LOG_E("reg[i].reg_add : %x\n", 0xf4);
		return ret;
	}
	rt_thread_delay(10); /* 10ms */
	ret = viss_i2c_write_reg_8bit(tp9930->i2c_client, 0x44, tmp[0]);
	if (RT_EOK != ret) {
		LOG_E("reg[i].reg_add : %x\n", 0x44);
		return ret;
	}

	for (i = 0; i < size; i++) {
		if (TP9930_DVP_REG_DELAY_FLAG == reg[i].reg_add) {
			temp = reg[i].reg_value + 9;
			do_div(temp, 10);
			delay = temp * 10;
			/* LOG_D("delay: %d", delay); */
			do_div(delay, 10);
			rt_thread_delay(delay);
			continue;
		}
		ret = viss_i2c_write_reg_8bit(tp9930->i2c_client,
					reg[i].reg_add, reg[i].reg_value);
		if (RT_EOK != ret) {
			LOG_E("reg[i].reg_add : %x\n", reg[i].reg_add);
			return ret;
		}
	}

	return RT_EOK;
}

static struct dev_mode *__tp9930_cur_mode(void *hdl)
{
	struct video_dev *tp9930 = (struct video_dev *)hdl;

	return tp9930->cur_mode;
}

static struct dev_mode *__tp9930_get_all_mode(void *hdl, rt_int32_t *num)
{
	RT_ASSERT(RT_NULL != num);
	*num = (rt_int32_t)ARRAY_SIZE(tp9930_dvp_mode);

	return tp9930_dvp_mode;
}

static rt_err_t __tp9930_set_power(void *hdl, rt_bool_t on)
{
	struct video_dev *tp9930 = (struct video_dev *)hdl;

	if (1 != tp9930->rst_valid)
		return RT_EOK;

	if (RT_TRUE == on) {
		/* LOG_D("tp9930 reset"); */
		pinctrl_gpio_set_value(tp9930->pctrl, tp9930->rst_gpio, 0);
		rt_thread_delay(10); /* 30ms 1*/
		pinctrl_gpio_set_value(tp9930->pctrl, tp9930->rst_gpio, 1);
		rt_thread_delay(10); /* 100ms 6*/
	} else {
		/* TO DO */
	}

	return RT_EOK;
}

static rt_err_t __tp9930_set_mode(void *hdl, rt_int32_t index)
{
	struct video_dev *tp9930 = (struct video_dev *)hdl;
	rt_int32_t num = 0;
	rt_err_t ret = 0;
	/* rt_uint8_t tmp = 0; */

	num = (rt_int32_t)ARRAY_SIZE(tp9930_dvp_mode);
	if (index >= num) {
		LOG_E("Input mode index error. num:%d index:%d\n", num, index);
		return -RT_EINVAL;
	}
	if (index >= ARRAY_SIZE(tp9930_dvp_mode)) {
		LOG_E("Input para error. index:%d\n", index);
		return -RT_EINVAL;
	}

	tp9930->cur_mode = &tp9930_dvp_mode[index];//index值为1
	ret = __tp9930_block_write((void *)tp9930, tp9930->cur_mode->usr_data,
			tp9930->cur_mode->usr_data_size);
#if 0
	rt_thread_delay(300); /* 30ms 1*/
	LOG_E("after init\n");
	ret = viss_i2c_write_reg_8bit(tp9930->i2c_client, 0x40, 0x00);
	ret = viss_i2c_read_reg_8bit(tp9930->i2c_client, 0x01, &tmp);
	LOG_E("add: %x. value: %x", 0x01, tmp);
	ret = viss_i2c_read_reg_8bit(tp9930->i2c_client, 0x02, &tmp);
	LOG_E("add: %x. value: %x", 0x02, tmp);
	ret = viss_i2c_read_reg_8bit(tp9930->i2c_client, 0x03, &tmp);
	LOG_E("add: %x. value: %x", 0x03, tmp);

	ret = viss_i2c_write_reg_8bit(tp9930->i2c_client, 0x40, 0x01);
	ret = viss_i2c_read_reg_8bit(tp9930->i2c_client, 0x01, &tmp);
	LOG_E("add: %x. value: %x", 0x01, tmp);
	ret = viss_i2c_read_reg_8bit(tp9930->i2c_client, 0x02, &tmp);
	LOG_E("add: %x. value: %x", 0x02, tmp);
	ret = viss_i2c_read_reg_8bit(tp9930->i2c_client, 0x03, &tmp);
	LOG_E("add: %x. value: %x", 0x03, tmp);

	ret = viss_i2c_write_reg_8bit(tp9930->i2c_client, 0x40, 0x02);
	ret = viss_i2c_read_reg_8bit(tp9930->i2c_client, 0x01, &tmp);
	LOG_E("add: %x. value: %x", 0x01, tmp);
	ret = viss_i2c_read_reg_8bit(tp9930->i2c_client, 0x02, &tmp);
	LOG_E("add: %x. value: %x", 0x02, tmp);
	ret = viss_i2c_read_reg_8bit(tp9930->i2c_client, 0x03, &tmp);
	LOG_E("add: %x. value: %x", 0x03, tmp);

	ret = viss_i2c_write_reg_8bit(tp9930->i2c_client, 0x40, 0x03);
	ret = viss_i2c_read_reg_8bit(tp9930->i2c_client, 0x01, &tmp);
	LOG_E("add: %x. value: %x", 0x01, tmp);
	ret = viss_i2c_read_reg_8bit(tp9930->i2c_client, 0x02, &tmp);
	LOG_E("add: %x. value: %x", 0x02, tmp);
	ret = viss_i2c_read_reg_8bit(tp9930->i2c_client, 0x03, &tmp);
	LOG_E("add: %x. value: %x", 0x03, tmp);
#endif
	return ret;
}


static rt_err_t __tp9930_set_register(void *hdl, struct viss_dbg_register *reg)
{
	return RT_EOK;
}

static rt_err_t __tp9930_get_register(void *hdl, struct viss_dbg_register *reg)
{
	return RT_EOK;
}
static rt_err_t __tp9930_set_stream(void *hdl, rt_bool_t enable)
{
	struct video_dev *tp9930 = (struct video_dev *)hdl;
	rt_err_t ret = 0;
	rt_uint8_t tmp[2] = {0};
	/* rt_uint8_t temp = 0; */
	/* rt_int32_t num = 0; */

	if (enable) {
		viss_i2c_write_reg_8bit(tp9930->i2c_client, 0x4d, 0x07);
		viss_i2c_write_reg_8bit(tp9930->i2c_client, 0x4e, 0x05);
		LOG_E("__tp9930_set_stream\n");
		#if 1
		if (tp9930->cur_mode->frame_size.height == 1080) {

			/* PLL reset */
			ret = viss_i2c_read_reg_8bit(tp9930->i2c_client, 0x44, &tmp[0]);
			ret = viss_i2c_write_reg_8bit(tp9930->i2c_client, 0x44, tmp[0] | 0x40);
			if (RT_EOK != ret) {
				LOG_E("reg[i].reg_add : %x\n", 0x40);
				return ret;
			}
			ret = viss_i2c_read_reg_8bit(tp9930->i2c_client, 0xf4, &tmp[1]);
			ret = viss_i2c_write_reg_8bit(tp9930->i2c_client, 0xf4, tmp[1] | 0x80);
			if (RT_EOK != ret) {
				LOG_E("reg[i].reg_add : %x\n", 0xf4);
				return ret;
			}
			rt_thread_delay(10); /* 10ms */
			ret = viss_i2c_write_reg_8bit(tp9930->i2c_client, 0x44, tmp[0]);
			if (RT_EOK != ret) {
				LOG_E("reg[i].reg_add : %x\n", 0x44);
				return ret;
			}

		}
		#endif

#if 0
		ret = viss_i2c_write_reg_8bit(tp9930->i2c_client, 0x40, 0x03);
		rt_thread_delay(10); /* 30ms 1*/
		ret = viss_i2c_write_reg_8bit(tp9930->i2c_client, 0x40, 0x00);
		    for(num =0 ;num <= 0xff;num ++)
		    {
		        ret = viss_i2c_read_reg_8bit(tp9930->i2c_client, num, &temp);
		        LOG_E("add: 0x%x. value: 0x%x", num, temp);
		    }
#endif
	} else {
		viss_i2c_write_reg_8bit(tp9930->i2c_client, 0x4e, 0x00);
		viss_i2c_write_reg_8bit(tp9930->i2c_client, 0x4d, 0x00);
	}
	return RT_EOK;
}


static rt_err_t __tp9930_ioctl(void *hdl, rt_int32_t cmd, void *para)
{
	rt_err_t ret = 0;
#ifdef ARCH_LOMBO_N7V1_SAR
	rt_int32_t index = (rt_int32_t)para;
	rt_uint8_t tmp = 0;
	struct video_dev *tp9930 = (struct video_dev *)hdl;

	if (RT_NULL == hdl) {
		LOG_E("__tp9930_ioctl fail.");
		return -RT_ERROR;
	}
	switch (index) {
	case 0:
		viss_i2c_write_reg_8bit(tp9930->i2c_client, 0x40, 0x00);
		viss_i2c_read_reg_8bit(tp9930->i2c_client, 0x01, &tmp);
		if ((tmp & (0x1 << 5)) && (tmp & (0x1 << 6))) {
#ifdef RT_USING_VIC_DET_SIGNAL
			if (tp9930_in[0] != 1) {
				tp9930_in[0] = 1;
				lb_system_mq_send(LB_SYSMSG_AV_PLUGIN, NULL, 0, 0);
				LOG_E("__tp9930_ioctl ch0 LB_SYSMSG_AV_PLUGIN.");
			}
#endif
			ret = 0;
		} else {
#ifdef RT_USING_VIC_DET_SIGNAL
			if (tp9930_in[0] != 0) {
				tp9930_in[0] = 0;
				lb_system_mq_send(LB_SYSMSG_AV_PLUGOUT, NULL, 0, 0);
				LOG_E("__tp9930_ioctl ch0 LB_SYSMSG_AV_PLUGOUT.");
			}
#endif
			ret = 1;
		}

		/* LOG_E("tmp:%02x\n", tmp); */

		break;
	case 1:
		viss_i2c_write_reg_8bit(tp9930->i2c_client, 0x40, 0x01);
		viss_i2c_read_reg_8bit(tp9930->i2c_client, 0x01, &tmp);
		if ((tmp & (0x1 << 5)) && (tmp & (0x1 << 6))) {
#ifdef RT_USING_VIC_DET_SIGNAL
			if (tp9930_in[1] != 1) {
				tp9930_in[1] = 1;
				lb_system_mq_send(LB_SYSMSG_DMS_PLUGIN, NULL, 0, 0);
				LOG_E("__tp9930_ioctl ch1 LB_SYSMSG_DMS_PLUGIN.");
			}
#endif
			ret = 0;
		} else {
#ifdef RT_USING_VIC_DET_SIGNAL
			if (tp9930_in[1] != 0) {
				tp9930_in[1] = 0;
				lb_system_mq_send(LB_SYSMSG_DMS_PLUGOUT, NULL, 0, 0);
				LOG_E("__tp9930_ioctl ch1 LB_SYSMSG_DMS_PLUGOUT.");
			}
#endif
			ret = 1;
		}
		/* LOG_E("tmp:%02x\n", tmp); */

		break;
	case 2:
		viss_i2c_write_reg_8bit(tp9930->i2c_client, 0x40, 0x02);
		viss_i2c_read_reg_8bit(tp9930->i2c_client, 0x01, &tmp);
		if ((tmp & (0x1 << 5)) && (tmp & (0x1 << 6)))
			ret = 0;
		else
			ret = 1;
		/* LOG_E("tmp:%02x\n", tmp); */

		break;
	case 3:
		viss_i2c_write_reg_8bit(tp9930->i2c_client, 0x40, 0x03);
		viss_i2c_read_reg_8bit(tp9930->i2c_client, 0x01, &tmp);
		if ((tmp & (0x1 << 5)) && (tmp & (0x1 << 6)))
			ret = 0;
		else
			ret = 1;
		/* LOG_E("tmp:%02x\n", tmp); */

		break;

	default:
		return -EINVAL;

	}
#endif
	return ret;
}

static rt_err_t __tp9930_get_info(void *hdl, struct viss_source_info *info)
{
	struct video_dev *tp9930 = (struct video_dev *)hdl;

	if ((RT_NULL == hdl) || (RT_NULL == info)) {
		LOG_E("Get tp9930 information fail.");
		return -RT_ERROR;
	}
	rt_memcpy(info, &tp9930->info, sizeof(struct viss_source_info));

	return RT_EOK;
}

static rt_err_t __tp9930_parser_config(void *hdl)
{
	struct video_dev *tp9930 = (struct video_dev *)hdl;
	struct viss_source_info info;
	rt_uint32_t tmp_data = 0;
	const char *status;
	const char *i2c_bus_name;
	const char *module = DRV_VIC_TP9930_NAME;
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
		LOG_W("tp9930 dvp disable.");
		return -RT_ERROR;
	}
	ret = config_get_string(module, "i2c-bus", &i2c_bus_name);
	if (-1 == ret) {
		LOG_E("Get i2c_bus fail.");
		return -RT_ERROR;
	}
	strncpy(info.i2c_bus_name, i2c_bus_name, sizeof(info.i2c_bus_name) - 1);
	/* LOG_D("info.i2c_bus_name:%s\n", info.i2c_bus_name); */

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

	info.frame_buf_type = 0;
	if (((VISS_IF_TYPE_ITU_656 == info.if_type) ||
		(VISS_IF_TYPE_ITU_1120 == info.if_type)) && (info.channel_num > 1)) {
		ret = config_get_u32(module, "frame-buf-type", &info.frame_buf_type);
		if (-1 == ret)
			LOG_E("Get frame_buf_type fail.");
	}

	ret = config_get_u32(module, "reg", &info.i2c_addr);
	if (-1 == ret)
		LOG_E("Get i2c_addr fail.");
	ret = config_get_u32(module, "hsync-active", &info.href);
	if (-1 == ret)
		LOG_E("Get href fail.");
	ret = config_get_u32(module, "vsync-active", &info.vref);
	if (-1 == ret)
		LOG_E("Get vref fail.");
	ret = config_get_u32(module, "interlaced", &info.interlaced);
	if (-1 == ret)
		LOG_E("Get vref fail.");
	ret = config_get_u32(module, "pclk-sample", &info.pclk);
	if (-1 == ret)
		LOG_E("Get pclk fail.");
	ret = config_get_u32(module, "field-sel", &info.field_sel);
	if (-1 == ret)
		LOG_E("Get field_sel fail.");
#if 1
	ret = config_get_u32(module, "mclk-freq", &info.mclk_freq);
	if (-1 == ret)
		LOG_E("Get mclk_freq fail.");
#endif
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
	rt_memcpy(&tp9930->info, &info, sizeof(struct viss_source_info));

	/* reset pin */
	tp9930->rst_valid = 1;
	ret = config_get_u32_array(module, "dvp-rst",
				tp9930->rst_val, ARRAY_SIZE(tp9930->rst_val));
	if (ret != ARRAY_SIZE(tp9930->rst_val)) {
		LOG_E("vic: reset pin config error. ret:%d", ret);
		tp9930->rst_valid = 0;
	}
#if 1
	/* mclk pin */
	tp9930->mclk_valid = 1;
	ret = config_get_u32_array(module, "dvp-mclk",
				tp9930->mclk_val, ARRAY_SIZE(tp9930->mclk_val));
	if (ret != ARRAY_SIZE(tp9930->mclk_val)) {
		LOG_E("vic: mclk pin config error. ret:%d", ret);
		tp9930->mclk_valid = 0;
	}
#endif
	return RT_EOK;
}

static rt_err_t __tp9930_prepare(void *hdl)
{
	return __tp9930_parser_config(hdl);
}

/* Init sensor config and check chip id */
static rt_err_t __tp9930_init(void *hdl)
{
	rt_int32_t ret = 0;
	rt_uint8_t tmp[2] = {0};
	rt_uint16_t id = 0;
	struct video_dev *tp9930 = (struct video_dev *)hdl;
#ifdef ARCH_LOMBO_N7V1_SAR
	char *module_0 = DRV_VIC_TP9930_PWR_NAME0;
	char *module_1 = DRV_VIC_TP9930_PWR_NAME1;
#endif

	RT_ASSERT(RT_NULL != tp9930);
	tp9930->pctrl = pinctrl_get(DRV_VIC_TP9930_NAME);
	if (RT_NULL == tp9930->pctrl)
		return -RT_ERROR;


	/* Init gpio */
	/* cam_power_enable(); */
	camera_ldo_set(vol_1p85v);
#ifdef ARCH_LOMBO_N7V1_SAR
	cam_power_set(module_0);
	cam_power_set(module_1);
#endif
	if (1 == tp9930->rst_valid) {
		tp9930->rst_gpio = pinctrl_gpio_request(tp9930->pctrl,
					tp9930->rst_val[0], tp9930->rst_val[1]);
		if (tp9930->rst_gpio >= 0) {
			pinctrl_gpio_set_function(tp9930->pctrl, tp9930->rst_gpio,
						tp9930->rst_val[2]);
			pinctrl_gpio_set_drv_level(tp9930->pctrl, tp9930->rst_gpio,
						tp9930->rst_val[3]);
			pinctrl_gpio_set_pud_mode(tp9930->pctrl, tp9930->rst_gpio,
						tp9930->rst_val[4]);
			pinctrl_gpio_set_pud_res(tp9930->pctrl, tp9930->rst_gpio,
						tp9930->rst_val[5]);
			pinctrl_gpio_set_value(tp9930->pctrl, tp9930->rst_gpio,
						tp9930->rst_val[6]);
		} else
			tp9930->rst_valid = 0;
	}
	if (1 == tp9930->mclk_valid) {
		tp9930->mclk_gpio = pinctrl_gpio_request(tp9930->pctrl,
					tp9930->mclk_val[0], tp9930->mclk_val[1]);
		if (tp9930->mclk_gpio >= 0) {
			pinctrl_gpio_set_function(tp9930->pctrl, tp9930->mclk_gpio,
						tp9930->mclk_val[2]);
			pinctrl_gpio_set_drv_level(tp9930->pctrl, tp9930->mclk_gpio,
						tp9930->mclk_val[3]);
			pinctrl_gpio_set_pud_mode(tp9930->pctrl, tp9930->mclk_gpio,
						tp9930->mclk_val[4]);
			pinctrl_gpio_set_pud_res(tp9930->pctrl, tp9930->mclk_gpio,
						tp9930->mclk_val[5]);
			pinctrl_gpio_set_value(tp9930->pctrl, tp9930->mclk_gpio,
						tp9930->mclk_val[6]);
		} else
			tp9930->mclk_valid = 0;
	}
	__tp9930_set_power(tp9930, RT_TRUE);

	/* check chip id */
	tp9930->i2c_client = rt_zalloc(sizeof(struct viss_i2c_client));
	if (RT_NULL == tp9930->i2c_client)
		return -RT_ENOMEM;
	tp9930->i2c_client->i2c_bus = rt_i2c_bus_device_find(tp9930->info.i2c_bus_name);
	if (RT_NULL == tp9930->i2c_client->i2c_bus) {
		LOG_E("can't find bus dev \"%s\"", tp9930->info.i2c_bus_name);
		goto exit;
	}
	tp9930->i2c_client->i2c_addr = tp9930->info.i2c_addr;
	ret = viss_i2c_read_reg_8bit(tp9930->i2c_client, 0xFE, &tmp[0]);
	LOG_D("ret: %d. 0xfd: %x", ret, tmp[0]);
	ret = viss_i2c_read_reg_8bit(tp9930->i2c_client, 0xFF, &tmp[1]);
	LOG_D("ret: %d. 0xfe: %x", ret, tmp[1]);
	id = (tmp[1] << 8) | tmp[0];
	if (id != 0x3228) {
		LOG_E("ID wrong! (0x%x)\n", id);
		goto exit;
	}
	return RT_EOK;

exit:
	if (tp9930->pctrl) {
		if (tp9930->rst_valid)
			pinctrl_gpio_free(tp9930->pctrl, tp9930->rst_gpio);
		if (tp9930->pwdn_valid)
			pinctrl_gpio_free(tp9930->pctrl, tp9930->pwdn_gpio);
		if (tp9930->mclk_valid)
			pinctrl_gpio_free(tp9930->pctrl, tp9930->mclk_gpio);
		pinctrl_put(tp9930->pctrl);
		tp9930->pctrl = RT_NULL;
	}
	if (tp9930->i2c_client) {
		rt_free(tp9930->i2c_client);
		tp9930->i2c_client = RT_NULL;
	}
#ifdef ARCH_LOMBO_N7V1_SAR
	cam_power_exit(module_0);
	cam_power_exit(module_1);
#endif
	camera_ldo_exit();
	/* cam_power_disable(); */

	return -RT_ERROR;
}

static void __tp9930_exit(void *hdl)
{
	struct video_dev *tp9930 = (struct video_dev *)hdl;
#ifdef ARCH_LOMBO_N7V1_SAR
	char *module_0 = DRV_VIC_TP9930_PWR_NAME0;
	char *module_1 = DRV_VIC_TP9930_PWR_NAME1;
#endif

	__tp9930_set_power(tp9930, RT_FALSE);
	if (tp9930->pctrl) {
		if (tp9930->rst_valid)
			pinctrl_gpio_free(tp9930->pctrl, tp9930->rst_gpio);
		if (tp9930->pwdn_valid)
			pinctrl_gpio_free(tp9930->pctrl, tp9930->pwdn_gpio);
		if (tp9930->mclk_valid)
			pinctrl_gpio_free(tp9930->pctrl, tp9930->mclk_gpio);
		pinctrl_put(tp9930->pctrl);
		tp9930->pctrl = RT_NULL;
	}
	if (tp9930->i2c_client) {
		rt_free(tp9930->i2c_client);
		tp9930->i2c_client = RT_NULL;
	}
#ifdef ARCH_LOMBO_N7V1_SAR
	cam_power_exit(module_0);
	cam_power_exit(module_1);
#endif
	camera_ldo_exit();
	/* cam_power_disable(); */
}

struct video_dev tp9930_dvp = {
	.name = DRV_VIC_TP9930_NAME,
	.group_id = GRP_ID_VIC,
	.prepare = __tp9930_prepare,
	.init = __tp9930_init,
	.exit = __tp9930_exit,
	.s_mode = __tp9930_set_mode,
	.g_cur_mode = __tp9930_cur_mode,
	.g_all_mode = __tp9930_get_all_mode,
	.s_power = __tp9930_set_power,
	.s_stream = __tp9930_set_stream,
	.g_info = __tp9930_get_info,
	.ioctl = __tp9930_ioctl,
	.s_register = __tp9930_set_register,
	.g_register = __tp9930_get_register,
};

