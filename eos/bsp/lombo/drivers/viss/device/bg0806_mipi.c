/*
 * isp_dev_bg0806.c - bg0806 driver code for LomboTech
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

#define DBG_SECTION_NAME	"BG0806-MIPI"
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

struct stVrefh {
	u16 index;
	u16 th_low;
	u16 th_high;
	u8 vrefh;
};

struct stVrefh stVrefhTable[] = {
	{5, 263, 267, 0x22},
	{4, 216, 220, 0x1c},
	{3, 176, 180, 0x17},
	{2, 130, 134, 0x12},
	{1, 113, 117, 0x10}
};

static u32 cur_expline = -1;
static u32 cur_again = -1;
static u32 cur_dgain = -1;

#define DRV_MCSI_BG0806_NAME "bg0806-mipi"
#define DRV_MCSI_BG0806_PWR_NAME "cam-pwr-en"

#define BG0806_MIPI_REG_DELAY_FLAG  (0xffff)
#define MAX_AGAIN (15 * 1024 + 512)
#define MAX_DGAIN (12 * 1024)
#define BG0806_RAW12

static u8 vrefh_min_tlb = 0x0c;
static u8 pre_index = 1;

static u8 reg_0x2b = 0x30;
static u8 reg_0x30 = 0x00;
static u8 reg_0x31 = 0xC0;
static u8 reg_0x34 = 0x00;
static u8 reg_0x35 = 0xC0;
static u8 reg_0x4d = 0x00;
static u8 reg_0x4f = 0x09;
static u8 reg_0x61 = 0x04;
static u8 reg_0x67 = 0x01;
static u8 reg_0x68 = 0x90;
static u8 reg_0x82 = 0x0b;
static u8 reg_0x52 = 0x06;
static u8 reg_0xf2 = 0x06;
static u8 reg_0xfb = 0x01;
static u32 cur_reg_idx = -1;
static u32 next_reg_idx = -1;

/* dsc_k blcc_k different with chip pid */
#define BG0806A		0x01
#define BG0806C1	0x07
#define BG0806C2	0x0b
#define BG0806B1	0x03
#define BG0806B2	0x0f
#define DSC_ADDR_BASE	0x0400

/* static struct viss_i2c_client *bg0806_mipi_i2c_client = RT_NULL; */

/*
 * 1lane raw10 init
 */
 #if 1
static const struct sensor_reg bg0806_reg_list[] = {
	/* MCLK=24M£¬PCLK=75M£¬10bit£¬1928x1088,30fps */
	{0x0200, 0x01},
	{0x0003, 0x27},
	{0x0006, 0x07},
	{0x0007, 0x80},
	{0x0008, 0x04},
	{0x0009, 0x40},
	{0x000e, 0x08},/* 0806_4times_75M_30fps */
	{0x000f, 0xae},/* row time F_W=2222 */
	{0x0013, 0x01},
	/* #ifdef MIRROR_0806 */
	/* {0x0020, 0x0f}, */
	/* #endif */
	/* 0x0020, 0x0001, *//* flip it works but has bug */
	{0x0021, 0x00},
	{0x0022, 0x27},/* vblank // 0x0025 // 0x0027 for 30fps // 0x00fe for 25fps */
	{0x0028, 0x00},
	{0x0029, 0x30},
	{0x002a, 0x00},
	{0x002b, 0x50},/* 0x30 // change PPK 20170623 */
	{0x0030, 0x00},
	{0x0031, 0xc0},/* rstb2rmp1 gap //0x00d0// change PPK 20170623 */
	{0x0034, 0x00},
	{0x0035, 0xc0},/* tx2rmp2 gap //0x00d0// change PPK 20170623 */
	{0x003c, 0x01},
	{0x003d, 0x84},/* rmp1_w@pclk domain */
	{0x003e, 0x05},/* ncp */
	{0x0042, 0x01},

	/* 0x0131, 0x0074, */
#if 0	/*VDDIO_VOLTAGE_3V3 */
	{0x005c, 0x07}, /* lsh_io ctrlbit for 3.3VDDIO */
#else
	{0x005c, 0x00}, /* lsh_io ctrlbit for 1.8VDDIO */
#endif
	{0x0061, 0x04},
	{0x0062, 0x5c},/* rmp2_w@pclk domain */
	{0x0064, 0x00},
	{0x0065, 0x80},/* rmp1_w@rmpclk domain */
	{0x0067, 0x01},
	{0x0068, 0x90},/* rmp2_w@rmpclk domain */
	{0x006c, 0x03},/* pd mipi dphy&dphy ldo */
	{0x007f, 0x03},
	{0x0080, 0x01}, /* dot en disable */
	{0x0081, 0x00},
	{0x0082, 0x0b},
	{0x0084, 0x08},
	{0x0088, 0x05},/* pclk dly */
	{0x008e, 0x00},
	{0x008f, 0x00},
	{0x0090, 0x01},
	{0x0094, 0x01},/* rmp div1 */
	{0x0095, 0x01},/* rmp div4 */
	{0x009e, 0x03},/* 4 times */
	{0x009f, 0x20},/* rmp2gap@rmpclk */
	{0x00b1, 0x7f},
	{0x00b2, 0x02},
	{0x00bc, 0x02},
	{0x00bd, 0x00},
	{0x0120, 0x01},/* blc on 01 direct */
	/* 0x0131, 0x0074, */
	{0x0131, 0x18},
	{0x0132, 0x01},/* k */
	{0x0133, 0x60},
	{0x0139, 0x07},
	{0x0139, 0xff},
	{0x013b, 0x08},
	{0x01a5, 0x07},/* row noise on 07 */
	{0x0160, 0x00},
	{0x0161, 0x30},
	{0x0162, 0x00},
	{0x0163, 0x30},
	{0x0164, 0x00},
	{0x0165, 0x30},
	{0x0166, 0x00},
	{0x0167, 0x30},
	{0x0206, 0x00},
	{0x0207, 0x00},
	{0x0053, 0x00},
	{0x0054, 0x28},/* plm_cnt */
	{0x0055, 0x00},/* pln_cnt */
	{0x00f3, 0x00},
	{0x00f4, 0x7b},/* plm */
	{0x00f5, 0x06},/* pln,371.25M pll */
	{0x006d, 0x03},/* pclk_ctrl, 03 for 1p25,0c for 1p5 */
	{0x006c, 0x00},/* ldo_ctrl,00 */
	{0x008d, 0x3f},
	{0x008e, 0x0c},/* oen_ctrl,0c */
	{0x008f, 0x03},/* io_sel_ctrl,03 */
	{0x00fa, 0xcf},/* ispc,c7 */
	{0x0391, 0x00},/* mipi_ctrl1,(raw10) */
	{0x0392, 0x00},/* mipi_ctrl2,default */
	{0x0393, 0x01},/* mipi_ctrl3,default */
	{0x0398, 0x0a},/* 28(25M),14(50M),0a(100M),08(111.375) */
	{0x0390, 0x06},/* mipi_ctrl0,bit[1],mipi enable */
	{0x001d, 0x01},

};
#else
static const struct sensor_reg bg0806_reg_list[] = {
	{0x0200, 0x01},
	{0x0003, 0x27},
	/* change exposure time */
	{0x000c, 0x00},
	{0x000d, 0xff},
	{0x000e, 0x08},/* 0806_4times_75M_30fps */
	{0x000f, 0xae},/* row time F_W=2222 */
	{0x0013, 0x01},
	{0x0021, 0x00},
	{0x0022, 0x25},/* vblank */
	{0x0028, 0x00},
	{0x0029, 0x30},
	{0x002a, 0x00},
	{0x002b, 0x30},
	{0x0030, 0x00},
	{0x0031, 0xd0},/* rstb2rmp1 gap */
	{0x0034, 0x00},
	{0x0035, 0xd0},/* tx2rmp2 gap */
	{0x003c, 0x01},
	{0x003d, 0x84},/* rmp1_w@pclk domain */
	{0x003e, 0x05},/* ncp */
	{0x0042, 0x01},
	{0x005c, 0x00},/* lsh_io ctrlbit for 1.8VDDIO */
	/* {0x005c, 0x0007}, lsh_io ctrlbit for 3.3VDDIO */
	{0x0061, 0x04},
	{0x0062, 0x5c},/* rmp2_w@pclk domain */
	{0x0064, 0x00},
	{0x0065, 0x80},/* rmp1_w@rmpclk domain */
	{0x0067, 0x01},
	{0x0068, 0x90},/* rmp2_w@rmpclk domain */
	{0x006c, 0x03},/* pd mipi dphy&dphy ldo */
	{0x007f, 0x03},
	{0x0080, 0x01},/* dot en disable */
	{0x0081, 0x00},
	{0x0082, 0x0b},
	{0x0084, 0x08},
	{0x0088, 0x05},/* pclk dly */
	{0x008e, 0x00},
	{0x008f, 0x00},
	{0x0094, 0x01},/* rmp div1 */
	{0x0095, 0x01},/* rmp div4 */
	{0x009e, 0x03},/* 4 times */
	{0x009f, 0x20},/* rmp2gap@rmpclk */
	{0x00b1, 0x1f},/* default is 0x7f, 2019-2-20@dylan */
	{0x00b2, 0x02},
	{0x00bc, 0x02},
	{0x00bd, 0x00},
	{0x0120, 0x01},/* blc on 01 direct */
	{0x0132, 0x01},/* k */
	{0x0133, 0x60},
	{0x0139, 0x07},
	{0x0139, 0xff},
	{0x013b, 0x08},
	{0x01a5, 0x07},/* row noise on 07 */
	{0x0160, 0x00},
	{0x0161, 0x30},
	{0x0162, 0x00},
	{0x0163, 0x30},
	{0x0164, 0x00},
	{0x0165, 0x30},
	{0x0166, 0x00},
	{0x0167, 0x30},
	{0x0206, 0x00},
	{0x0207, 0x00},
	{0x0053, 0x00},
	{0x0054, 0x28},/* plm_cnt */
	{0x0055, 0x00},/* pln_cnt */

#ifdef BG0806_RAW12
	{0x00f3, 0x00},
	{0x00f4, 0x49},/* plm */
	{0x00f5, 0x02},/* pln,445.5M pll */
	{0x006d, 0x0c},/* pclk_ctrl, 03 for 1p25,0c for 1p5 */
	{0x006c, 0x00},/* ldo_ctrl,00 */
	{0x008d, 0x3f},
	{0x008e, 0x0c},/* oen_ctrl,0c */
	{0x008f, 0x03},/* io_sel_ctrl,03 */
	{0x00fa, 0xcf},/* ispc,c7 */
	/* bit[0]=0:raw10, bit[0]=1:raw12) */
	{0x0391, 0x01},
	{0x0392, 0x00},/* mipi_ctrl2,default */
	{0x0393, 0x01},/* mipi_ctrl3,default */
	{0x0398, 0x08},/* 28(25M),14(50M),0a(100M),08(111.375) */
	{0x0390, 0x06},/* mipi_ctrl0,bit[1],mipi enable */
#else
	{0x00f3, 0x00},
	{0x00f4, 0x7b},/* plm */
	{0x00f5, 0x06},/* pln,371.25M pll */
	{0x006d, 0x03},/* pclk_ctrl, 03 for 1p25,0c for 1p5 */
	{0x006c, 0x00},/* ldo_ctrl,00 */
	{0x008d, 0x3f},
	{0x008e, 0x0c},/* oen_ctrl,0c */
	{0x008f, 0x03},/* io_sel_ctrl,03 */
	{0x00fa, 0xcf},/* ispc,c7 */
	/* mipi_ctrl1,(bit[0]=0:raw10, bit[0]=1:raw12) */
	{0x0391, 0x00},
	{0x0392, 0x00},/* mipi_ctrl2,default */
	{0x0393, 0x01},/* mipi_ctrl3,default */
	{0x0398, 0x0a},/* 28(25M),14(50M),0a(100M),08(111.375) */
	{0x0390, 0x06},/* mipi_ctrl0,bit[1],mipi enable */
#endif

#ifdef EXTERNAL_LDO_1V5
	{0x0132, 0x00}, /* k */
	{0x0133, 0xff},
	{0x0206, 0x03},
	{0x0207, 0x01},
	{0x006e, 0x00},
#else
	{0x0132, 0x01}, /* k */
	{0x0133, 0x30},
	{0x0206, 0x04},
	{0x0207, 0xd8},
	{0x006e, 0x01},
#endif
	{0x001d, 0x01},
};
#endif

const u8 Tab_sensor_dsc[768] = {
	/* 20170206 update dsc	external ldo */
	0x0a, 0x5c, 0x09, 0x4a, 0x08, 0x07, 0x06, 0xee, 0x06, 0x23,
	0x05, 0x7e, 0x05, 0x21, 0x05, 0x01, 0x05, 0xcc, 0x06, 0xf5,
	0x08, 0x0b, 0x09, 0x0e, 0x0a, 0x0e, 0x0b, 0x34, 0x0c, 0x42,
	0x0e, 0x9b, 0x09, 0xe0, 0x08, 0x71, 0x06, 0xce, 0x05, 0xd7,
	0x05, 0x19, 0x04, 0x9c, 0x04, 0x54, 0x04, 0x4d, 0x04, 0xd2,
	0x05, 0xf8, 0x07, 0x04, 0x07, 0xe3, 0x08, 0xe2, 0x09, 0xfe,
	0x0b, 0x69, 0x0d, 0xfa, 0x09, 0x62, 0x07, 0x83, 0x05, 0xd8,
	0x04, 0xe6, 0x04, 0x44, 0x03, 0xe5, 0x03, 0x82, 0x03, 0x88,
	0x04, 0x07, 0x05, 0x09, 0x05, 0xfe, 0x06, 0xc5, 0x07, 0xbf,
	0x08, 0xe2, 0x0a, 0x68, 0x0d, 0x59, 0x08, 0xf7, 0x06, 0xea,
	0x05, 0x43, 0x04, 0x55, 0x03, 0xbf, 0x03, 0x43, 0x03, 0x09,
	0x02, 0xf7, 0x03, 0x83, 0x04, 0x75, 0x05, 0x5c, 0x06, 0x09,
	0x06, 0xf5, 0x08, 0x1c, 0x09, 0xa6, 0x0c, 0xc6, 0x08, 0x6a,
	0x06, 0x5e, 0x04, 0xa9, 0x03, 0xc7, 0x03, 0x36, 0x02, 0xd1,
	0x02, 0x99, 0x02, 0x91, 0x02, 0xea, 0x03, 0xdb, 0x04, 0xad,
	0x05, 0x4f, 0x06, 0x48, 0x07, 0x62, 0x08, 0xf4, 0x0b, 0x71,
	0x07, 0xbd, 0x05, 0xd0, 0x04, 0x2a, 0x03, 0x43, 0x02, 0xa1,
	0x02, 0x4d, 0x02, 0x08, 0x01, 0xf8, 0x02, 0x58, 0x03, 0x3f,
	0x03, 0xec, 0x04, 0xa1, 0x05, 0x87, 0x06, 0xa5, 0x08, 0x31,
	0x0a, 0xc0, 0x07, 0x5a, 0x05, 0x54, 0x03, 0xa0, 0x02, 0xca,
	0x02, 0x4a, 0x01, 0xd9, 0x01, 0x9c, 0x01, 0x98, 0x01, 0xf0,
	0x02, 0xba, 0x03, 0x70, 0x04, 0x02, 0x04, 0xe5, 0x06, 0x0b,
	0x07, 0x84, 0x0a, 0x2f, 0x06, 0xdf, 0x04, 0xe4, 0x03, 0x3f,
	0x02, 0x6d, 0x01, 0xe9, 0x01, 0x84, 0x01, 0x47, 0x01, 0x3f,
	0x01, 0x8e, 0x02, 0x31, 0x02, 0xea, 0x03, 0x78, 0x04, 0x5e,
	0x05, 0x68, 0x06, 0xea, 0x09, 0x27, 0x06, 0x82, 0x04, 0x81,
	0x02, 0xe8, 0x02, 0x20, 0x01, 0x9c, 0x01, 0x2c, 0x00, 0xfa,
	0x00, 0xea, 0x01, 0x37, 0x01, 0xdc, 0x02, 0x76, 0x02, 0xf6,
	0x03, 0xd5, 0x04, 0xd7, 0x06, 0x49, 0x08, 0xb0, 0x06, 0x4b,
	0x04, 0x42, 0x02, 0xaf, 0x01, 0xde, 0x01, 0x4f, 0x00, 0xe7,
	0x00, 0xa2, 0x00, 0x92, 0x00, 0xb6, 0x01, 0x5a, 0x01, 0xe4,
	0x02, 0x7e, 0x03, 0x3b, 0x04, 0x31, 0x05, 0x88, 0x07, 0xb8,
	0x06, 0x25, 0x04, 0x0d, 0x02, 0x7a, 0x01, 0xa1, 0x01, 0x1b,
	0x00, 0xab, 0x00, 0x5e, 0x00, 0x4e, 0x00, 0x68, 0x00, 0xf2,
	0x01, 0x62, 0x01, 0xec, 0x02, 0x9e, 0x03, 0x8f, 0x04, 0xe3,
	0x07, 0x01, 0x06, 0x0e, 0x04, 0x0d, 0x02, 0x66, 0x01, 0x94,
	0x01, 0x10, 0x00, 0x99, 0x00, 0x44, 0x00, 0x21, 0x00, 0x34,
	0x00, 0x97, 0x01, 0x13, 0x01, 0x94, 0x02, 0x49, 0x03, 0x40,
	0x04, 0x75, 0x06, 0x92, 0x06, 0x54, 0x04, 0x31, 0x02, 0x96,
	0x01, 0xc5, 0x01, 0x18, 0x00, 0x99, 0x00, 0x39, 0x00, 0x21,
	0x00, 0x21, 0x00, 0x65, 0x00, 0xd6, 0x01, 0x5f, 0x02, 0x0c,
	0x02, 0xfa, 0x04, 0x31, 0x06, 0x5b, 0x06, 0x5c, 0x04, 0x41,
	0x02, 0x9e, 0x01, 0xc5, 0x01, 0x18, 0x00, 0x99, 0x00, 0x39,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x59, 0x00, 0xc6, 0x01, 0x4b,
	0x01, 0xf9, 0x02, 0xf3, 0x04, 0x1d, 0x06, 0x37, 0x06, 0x0e,
	0x04, 0x1a, 0x02, 0x92, 0x01, 0xbd, 0x01, 0x18, 0x00, 0xa1,
	0x00, 0x4a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x59, 0x00, 0xce,
	0x01, 0x4b, 0x02, 0x14, 0x02, 0xfa, 0x04, 0x25, 0x06, 0x3f,
	0x05, 0x82, 0x03, 0xf2, 0x02, 0x87, 0x01, 0xa0, 0x01, 0x28,
	0x00, 0xaa, 0x00, 0x62, 0x00, 0x31, 0x00, 0x3c, 0x00, 0x89,
	0x00, 0xfe, 0x01, 0x7b, 0x02, 0x3a, 0x03, 0x26, 0x04, 0x69,
	0x06, 0x82, 0x05, 0xb3, 0x04, 0x02, 0x02, 0x97, 0x01, 0xd2,
	0x01, 0x43, 0x00, 0xdb, 0x00, 0x99, 0x00, 0x5d, 0x00, 0x75,
	0x00, 0xd6, 0x01, 0x4b, 0x01, 0xc7, 0x02, 0x71, 0x03, 0x73,
	0x04, 0xc7, 0x06, 0xe2, 0x05, 0xcf, 0x04, 0x3c, 0x02, 0xe4,
	0x02, 0x1f, 0x01, 0x90, 0x01, 0x30, 0x00, 0xe6, 0x00, 0xcb,
	0x00, 0xd6, 0x01, 0x2b, 0x01, 0xb5, 0x02, 0x25, 0x02, 0xf3,
	0x03, 0xe4, 0x05, 0x42, 0x07, 0xb2, 0x05, 0xe0, 0x04, 0x83,
	0x03, 0x22, 0x02, 0x67, 0x01, 0xed, 0x01, 0x7d, 0x01, 0x3f,
	0x01, 0x14, 0x01, 0x27, 0x01, 0x93, 0x02, 0x10, 0x02, 0x8d,
	0x03, 0x4c, 0x04, 0x3f, 0x05, 0x9c, 0x07, 0xc1, 0x06, 0x1d,
	0x04, 0xce, 0x03, 0x7d, 0x02, 0xb5, 0x02, 0x42, 0x01, 0xd1,
	0x01, 0x85, 0x01, 0x74, 0x01, 0x88, 0x01, 0xe8, 0x02, 0x71,
	0x02, 0xe1, 0x03, 0xc0, 0x04, 0xb6, 0x06, 0x04, 0x08, 0x7f,
	0x06, 0x40, 0x05, 0x10, 0x03, 0xe0, 0x03, 0x1b, 0x02, 0xab,
	0x02, 0x3a, 0x01, 0xe4, 0x01, 0xd4, 0x01, 0xdc, 0x02, 0x50,
	0x02, 0xce, 0x03, 0x60, 0x04, 0x29, 0x05, 0x32, 0x06, 0x88,
	0x09, 0x16, 0x06, 0x63, 0x05, 0x8c, 0x04, 0x66, 0x03, 0xbc,
	0x03, 0x1b, 0x02, 0xc2, 0x02, 0x79, 0x02, 0x41, 0x02, 0x69,
	0x02, 0xde, 0x03, 0x52, 0x03, 0xdf, 0x04, 0xc9, 0x05, 0xc8,
	0x06, 0xfd, 0x09, 0x46, 0x06, 0x88, 0x05, 0xf8, 0x05, 0x2a,
	0x04, 0x42, 0x03, 0xb0, 0x03, 0x4b, 0x02, 0xf3, 0x02, 0xcf,
	0x02, 0xc5, 0x03, 0x46, 0x03, 0xdf, 0x04, 0x6d, 0x05, 0x46,
	0x06, 0x54, 0x07, 0x69, 0x09, 0xe9, 0x06, 0x9d, 0x05, 0xe7,
	0x06, 0x75, 0x04, 0x68, 0x04, 0x10, 0x03, 0x5c, 0x03, 0x0f,
	0x02, 0xeb, 0x02, 0xf6, 0x03, 0x7d, 0x04, 0x0e, 0x04, 0xa7,
	0x05, 0xc7, 0x06, 0x72, 0x07, 0x84, 0x0a, 0x0d
};

struct dev_mode *cur_bg0806_mipi_mode = RT_NULL;
static struct dev_mode bg0806_mipi_mode[] = {
	{
		.index = 0,
		.inp_fmt = VISS_MBUS_FMT_YUYV8_2X8,
		.out_fmt = VISS_PIX_FMT_NV12,
		.colorspace = DEV_COLORSPACE_JPEG,
		.frame_rate = 30000,   /* 30fps */
		.frame_size.width = 1920,
		.frame_size.height = 1080,
		.usr_data = (void *)bg0806_reg_list,
		.usr_data_size = ARRAY_SIZE(bg0806_reg_list),
	},
	{
		.index = 1,
		.inp_fmt = VISS_MBUS_FMT_YUYV8_2X8,
		.out_fmt = VISS_PIX_FMT_NV12,
		.colorspace = DEV_COLORSPACE_JPEG,
		.frame_rate = 30000,   /* 30fps */
		.frame_size.width = 1280,
		.frame_size.height = 720,
		.usr_data = (void *)bg0806_reg_list,
		.usr_data_size = ARRAY_SIZE(bg0806_reg_list),
	},
};

static rt_err_t __bg0806_mipi_wake_up(void *hdl)
{
	struct video_dev *bg0806 = (struct video_dev *)hdl;
	return 0;
}
rt_int32_t register_finished;

static struct dev_mode *__bg0806_mipi_cur_mode(void *hdl)
{
	struct video_dev *bg0806 = (struct video_dev *)hdl;
	return bg0806->cur_mode;
}

static struct dev_mode *__bg0806_mipi_get_all_mode(void *hdl, rt_int32_t *num)
{
	RT_ASSERT(RT_NULL != num);
	*num = (rt_int32_t)ARRAY_SIZE(bg0806_mipi_mode);

	return bg0806_mipi_mode;
}
int reg_writed;
static rt_err_t __bg0806_mipi_set_mode(void *hdl, rt_int32_t index)
{
	struct video_dev *bg0806 = (struct video_dev *)hdl;
	rt_int32_t num = 0;
	rt_err_t ret = 0;

	num = (rt_int32_t)ARRAY_SIZE(bg0806_mipi_mode);
	if (index >= num) {
		LOG_E("Input mode index error. num:%d index:%d\n", num, index);
		return -RT_EINVAL;
	}
	if (index >= ARRAY_SIZE(bg0806_mipi_mode)) {
		LOG_E("Input para error. index:%d\n", index);
		return -RT_EINVAL;
	}
	bg0806->cur_mode = &bg0806_mipi_mode[index];
	if (reg_writed)
		return RT_EOK;

	reg_writed = 1;

	/*
	ret = __bg0806_mipi_block_write(hdl, bg0806->cur_mode->usr_data,
				bg0806->cur_mode->usr_data_size);
	*/
	if (RT_EOK != ret)
		return ret;

	return __bg0806_mipi_wake_up(hdl);
}

static void __bg0806_calc_gain(void *hdl, u32 gain, u32 *_again, u32 *_dgain)
{
	struct video_dev *bg0806 = (struct video_dev *)hdl;
	u32 total_gain, act_again;
	u8 vrefh, temp;
	u16 dgain;
	u32 dark_ave;
	u16 vrefh_old;
	u16 rmp_gain;
	u8 i, rdRegDat;

	viss_i2c_read_reg_16bit(bg0806->i2c_client, 0x00b1, &rdRegDat);
	vrefh_old = rdRegDat;
	rmp_gain = 128 / (vrefh_old + 1);

	dark_ave = 0;
	viss_i2c_read_reg_16bit(bg0806->i2c_client, 0x012b, &rdRegDat);
	dark_ave = rdRegDat;
	dark_ave <<= 8;
	viss_i2c_read_reg_16bit(bg0806->i2c_client, 0x012c, &rdRegDat);
	dark_ave |= rdRegDat;

	if (dark_ave > 0xfff)
		dark_ave = 0;

	dark_ave = dark_ave / rmp_gain;

	if (dark_ave < stVrefhTable[4].th_low) {
		pre_index = 0;
		vrefh_min_tlb = 0x0c;
	} else {
		i = 0;
		while (i < 5) {
			if (pre_index < stVrefhTable[i].index
				&& dark_ave > stVrefhTable[i].th_high) {
				pre_index = stVrefhTable[i].index;
				vrefh_min_tlb = stVrefhTable[i].vrefh;
				break;
			}
			i++;
		}

		i = 0;
		for (i = 0; i < 5; i++) {
			if (pre_index >=  stVrefhTable[i].index
				&& dark_ave < stVrefhTable[i].th_low) {
				pre_index = stVrefhTable[i - 1].index;
				vrefh_min_tlb = stVrefhTable[i - 1].vrefh;
			}
		}
	}

#if 0
	total_gain = gain;
	if (total_gain >= (128 << 10))
		vrefh = 0;
	else
		vrefh = (128 << 10) / total_gain - 1;

	if (vrefh > 0x7f)
		vrefh = 0x7f;
	else if (vrefh < vrefh_min_tlb)
		vrefh = vrefh_min_tlb;
	dgain = (total_gain * (vrefh + 1) * 512) / (128 * 1024);
	if (dgain < 0x200)
		dgain = 0x200;
	if (dgain > 0xfff)
		dgain = 0xfff;
#else
	total_gain = gain;
	vrefh = ((128 << 10) / total_gain) - 1;
	if (vrefh < vrefh_min_tlb)
		vrefh = vrefh_min_tlb;
	if (vrefh > 0x7f)
		vrefh = 0x7f;

	act_again = (128 << 10) / (vrefh + 1);
	dgain = (total_gain << 9) / act_again;

	if (dgain < 0x0200)
		dgain = 0x0200;
	if (dgain > 0x0fff)
		dgain = 0x0fff;
#endif


	if (vrefh == vrefh_min_tlb || vrefh <= 0x0f) {
		/* reg_0x2b = 0x10; */
		reg_0x30 = 0x01;
		reg_0x31 = 0xb0;
		reg_0x34 = 0x01;
		reg_0x35 = 0xb0;
		reg_0x4d = 0x03;
		reg_0x4f = 0x0c;
		reg_0x61 = 0x02;
		reg_0x67 = 0x00;
		reg_0x68 = 0x80;
		next_reg_idx = 1;
	} else if ((vrefh > vrefh_min_tlb) && (vrefh <= 0x7f)) {
		/* reg_0x2b = 0x30; */
		reg_0x30 = 0x00;
		reg_0x31 = 0xc0;
		reg_0x34 = 0x00;
		reg_0x35 = 0xc0;
		reg_0x4d = 0x00;
		reg_0x4f = 0x09;
		reg_0x61 = 0x04;
		reg_0x67 = 0x01;
		reg_0x68 = 0x90;
		next_reg_idx = 0;
	}


#if 0
	if (vrefh == vrefh_min_tlb) {
		if (gVblank > 0x106) {
			reg_0x82 = 0x04;
			reg_0x52 = 0x07;
			reg_0xf2 = 0x07;
			reg_0xfb = 0x02;
		} else {
			reg_0x82 = 0x08;
			reg_0x52 = 0x06;
			reg_0xf2 = 0x06;
			reg_0xfb = 0x01;
		}
	} else {
		reg_0x82 = 0x0b;
		reg_0x52 = 0x06;
		reg_0xf2 = 0x06;
		reg_0xfb = 0x01;
	}
#endif

	*_again = vrefh;
	*_dgain = dgain;
}

static rt_err_t __bg0806_set_again(void *hdl, rt_uint32_t again)
{
	struct video_dev *bg0806 = (struct video_dev *)hdl;

	if (cur_again == again)
		return RT_EOK;
	cur_again = again;

	viss_i2c_write_reg_16bit(bg0806->i2c_client, 0x00b1, again);
	return RT_EOK;
}

static rt_err_t __bg0806_set_dgain(void *hdl, rt_uint32_t dgain)
{
	struct video_dev *bg0806 = (struct video_dev *)hdl;

	if (cur_dgain == dgain)
		return RT_EOK;
	cur_dgain = dgain;

	viss_i2c_write_reg_16bit(bg0806->i2c_client, 0x00bc, dgain >> 8);
	viss_i2c_write_reg_16bit(bg0806->i2c_client, 0x00bd, dgain & 0xff);
	return RT_EOK;
}

static rt_err_t __bg0806_set_shutter(void *hdl, rt_uint32_t shutter)
{
	struct video_dev *bg0806 = (struct video_dev *)hdl;

	if (cur_expline == shutter)
		return RT_EOK;
	cur_expline  = shutter;

	viss_i2c_write_reg_16bit(bg0806->i2c_client, 0x000c, shutter >> 8);
	viss_i2c_write_reg_16bit(bg0806->i2c_client, 0x000d, shutter & 0xff);

	return RT_EOK;
}

static rt_err_t __bg0806_exp_ctrl(void *hdl, struct isp_exp_gain *exp_gain)
{
	struct video_dev *bg0806 = (struct video_dev *)hdl;
	u32 again, dgain;
	rt_err_t ret = 0;

	if ((RT_NULL == hdl) || (RT_NULL == exp_gain)) {
		LOG_E("set bg0806_exp_ctrl fail.");
		return -RT_ERROR;
	}
	LOG_D("**********exp_ctrl: %d, %d.", exp_gain->gain, exp_gain->exp);

	if (register_finished == 0) {
		LOG_E("register not finished.\n");
		return RT_EOK;
	}

	__bg0806_calc_gain(bg0806, exp_gain->gain * 4, &again, &dgain);

	if (cur_reg_idx != next_reg_idx) {
		cur_reg_idx = next_reg_idx;
		viss_i2c_write_reg_16bit(bg0806->i2c_client, 0x0030, reg_0x30);
		viss_i2c_write_reg_16bit(bg0806->i2c_client, 0x0031, reg_0x31);
		viss_i2c_write_reg_16bit(bg0806->i2c_client, 0x0034, reg_0x34);
		viss_i2c_write_reg_16bit(bg0806->i2c_client, 0x0035, reg_0x35);
		viss_i2c_write_reg_16bit(bg0806->i2c_client, 0x004d, reg_0x4d);
		viss_i2c_write_reg_16bit(bg0806->i2c_client, 0x004f, reg_0x4f);
		viss_i2c_write_reg_16bit(bg0806->i2c_client, 0x0061, reg_0x61);

		/*viss_i2c_write_reg_16bit(bg0806->i2c_client, 0x002b, reg_0x2b); */
		viss_i2c_write_reg_16bit(bg0806->i2c_client, 0x0067, reg_0x67);
		viss_i2c_write_reg_16bit(bg0806->i2c_client, 0x0068, reg_0x68);
	}
#if 1
	viss_i2c_write_reg_16bit(bg0806->i2c_client, 0x0082, reg_0x82);
	viss_i2c_write_reg_16bit(bg0806->i2c_client, 0x0052, reg_0x52);
	viss_i2c_write_reg_16bit(bg0806->i2c_client, 0x00f2, reg_0xf2);
	viss_i2c_write_reg_16bit(bg0806->i2c_client, 0x00fb, reg_0xfb);
#endif
	__bg0806_set_again(bg0806, again);
	__bg0806_set_dgain(bg0806, dgain);
	__bg0806_set_shutter(bg0806, exp_gain->exp / 16);

	viss_i2c_write_reg_16bit(bg0806->i2c_client, 0x001d, 0x02);

	return ret;
}
static rt_err_t __bg0806_isp_get_sensor_info(void *hdl,
	struct isp_sensor_info *isp_sensor_info)
{
	isp_sensor_info->sensor_name = "bg0806";
	isp_sensor_info->pclk = 75 * 1000 * 1000;
	isp_sensor_info->vts = 1125;
	isp_sensor_info->hts = 2222;
	isp_sensor_info->input_width = 1920;
	isp_sensor_info->input_height = 1080;
	isp_sensor_info->output_widht = 1920;
	isp_sensor_info->output_height = 1080;
	isp_sensor_info->bayer_mode = ISP_BPAT_GRGRBGBG;

	return RT_EOK;
}
static rt_err_t __bg0806_mipi_set_register(void *hdl,
					struct viss_dbg_register *reg)
{
	struct video_dev *bg0806 = (struct video_dev *)hdl;
	rt_err_t ret = 0;

	ret = viss_i2c_write_reg_16bit(bg0806->i2c_client, reg->add, reg->val);

	return RT_EOK;
}

static rt_err_t __bg0806_mipi_get_register(void *hdl,
					struct viss_dbg_register *reg)
{
	struct video_dev *bg0806 = (struct video_dev *)hdl;
	rt_uint8_t value;
	rt_err_t ret = 0;

	ret = viss_i2c_read_reg_16bit(bg0806->i2c_client, reg->add, &value);
	reg->val = value;

	return ret;
}


static rt_err_t __bg0806_mipi_block_write(void *hdl,
						void *data, rt_int32_t size)
{
	struct video_dev *bg0806 = (struct video_dev *)hdl;
	struct sensor_reg *reg = (struct sensor_reg *)data;
	rt_int32_t i = 0;
	rt_err_t ret = 0;
	rt_int32_t delay = 0, temp = 0;
	rt_uint8_t temp_reg_val = 0;
	struct isp_exp_gain exp_gain_temp;

	u16 dsc_addr;
	u8 pid;

	viss_i2c_read_reg_16bit(bg0806->i2c_client, 0x0045, &pid);
	pid = pid & 0x3f;

	LOG_D("bg0806 pid = %x\n", pid);

	for (i = 0; i < size; i++) {
		ret = viss_i2c_write_reg_16bit(bg0806->i2c_client,
					reg[i].reg_add, reg[i].reg_value);
		if (RT_EOK != ret) {
			LOG_E("i2c write %x value:%x fail.\n",
				reg[i].reg_add, reg[i].reg_value);
			return ret;
		}
	}

	switch (pid) {
	case BG0806A:
	    viss_i2c_write_reg_16bit(bg0806->i2c_client, 0x0206, 0x02);
	    viss_i2c_write_reg_16bit(bg0806->i2c_client, 0x0207, 0xc8);
	    viss_i2c_write_reg_16bit(bg0806->i2c_client, 0x0132, 0x01);
	    viss_i2c_write_reg_16bit(bg0806->i2c_client, 0x0133, 0x38);
	    break;
	case BG0806C1:
	case BG0806C2:
	    viss_i2c_write_reg_16bit(bg0806->i2c_client, 0x0206, 0x02);
	    viss_i2c_write_reg_16bit(bg0806->i2c_client, 0x0207, 0xaa);
	    viss_i2c_write_reg_16bit(bg0806->i2c_client, 0x0132, 0x01);
	    viss_i2c_write_reg_16bit(bg0806->i2c_client, 0x0133, 0x56);
	    break;
	case BG0806B1:
	case BG0806B2:
	default:
	    viss_i2c_write_reg_16bit(bg0806->i2c_client, 0x0206, 0x02);
	    viss_i2c_write_reg_16bit(bg0806->i2c_client, 0x0207, 0xde);
	    viss_i2c_write_reg_16bit(bg0806->i2c_client, 0x0132, 0x01);
	    viss_i2c_write_reg_16bit(bg0806->i2c_client, 0x0133, 0x22);
	    break;
	}

	dsc_addr = DSC_ADDR_BASE;

	for (i = 0; i < 768; i++)
		viss_i2c_write_reg_16bit(bg0806->i2c_client,
		dsc_addr + i, Tab_sensor_dsc[i]);


	viss_i2c_write_reg_16bit(bg0806->i2c_client, 0x001d, 0x01);
	exp_gain_temp.exp = 300;
	exp_gain_temp.gain = 300;
	LOG_D("__bg0806_mipi_block_write exp init.");
	register_finished = 1;

	__bg0806_exp_ctrl(hdl, &exp_gain_temp);
	viss_i2c_write_reg_16bit(bg0806->i2c_client, 0x001d, 0x02);

	return RT_EOK;
}

static rt_err_t __bg0806_mipi_set_power(void *hdl, rt_bool_t on)
{
	struct video_dev *bg0806 = (struct video_dev *)hdl;
	/* LOG_D("__bg0806_mipi_set_power"); */

	if ((1 != bg0806->pwdn_valid) || (1 != bg0806->rst_valid))
		return RT_EOK;

	if (RT_TRUE == on) {
		/* LOG_D("__bg0806_mipi_set_power 1111"); */
		pinctrl_gpio_set_value(bg0806->pctrl, bg0806->rst_gpio, 0);
		rt_thread_delay(1); /* 10ms */
		pinctrl_gpio_set_value(bg0806->pctrl, bg0806->pwdn_gpio, 1);
		rt_thread_delay(1); /* 10ms */
		pinctrl_gpio_set_value(bg0806->pctrl, bg0806->pwdn_gpio, 0);
		rt_thread_delay(1); /* 10ms */
		pinctrl_gpio_set_value(bg0806->pctrl, bg0806->rst_gpio, 1);
		rt_thread_delay(1); /* 10ms */
	} else {
		/* LOG_D("__bg0806_mipi_set_power 222 "); */
		/* TO DO */
	}
	/* LOG_D("__bg0806_mipi_set_power 3333"); */

	return RT_EOK;
}

static rt_err_t __bg0806_mipi_set_stream(void *hdl, rt_bool_t enable)
{
	rt_int32_t ret = 0;
	struct video_dev *bg0806 = (struct video_dev *)hdl;

	if (enable) {
		__bg0806_mipi_set_power(bg0806, RT_TRUE);

		ret = __bg0806_mipi_block_write(hdl, bg0806->cur_mode->usr_data,
					bg0806->cur_mode->usr_data_size);
	}

	return ret;
}

rt_err_t __bg0806_mipi_ioctl(void *hdl, rt_int32_t cmd, void *para)
{
	rt_err_t ret = 0;
	struct isp_exp_gain *bg0806_exp = RT_NULL;
	struct isp_sensor_info *bg0806_isp_sensor_info = RT_NULL;

	switch (cmd) {
	case ISP_GET_SENSOR_INFO:
		bg0806_isp_sensor_info = (struct isp_sensor_info *)para;
		__bg0806_isp_get_sensor_info(hdl, bg0806_isp_sensor_info);
		break;
	case ISP_SET_EXP_GAIN:
		bg0806_exp = (struct isp_exp_gain *)para;
		__bg0806_exp_ctrl(hdl, bg0806_exp);
		/* LOG_FLOAT("ISP_SET_EXP_GAIN: exp: %d, gain: %f.",
			ov2710_exp->exp, ov2710_exp->gain); */
		break;
	default:
		return -RT_ERROR;
	}
	return ret;
}

static rt_err_t __bg0806_mipi_get_info(void *hdl,
			struct viss_source_info *info)
{
	struct video_dev *bg0806 = (struct video_dev *)hdl;
	/* LOG_D("__bg0806_mipi_get_info in"); */

	if ((RT_NULL == hdl) || (RT_NULL == info)) {
		LOG_E("Get bg0806 information fail.");
		return -RT_ERROR;
	}
	rt_memcpy(info, &bg0806->info, sizeof(struct viss_source_info));
	/* LOG_D("__bg0806_mipi_get_info out"); */

	return RT_EOK;
}

static rt_err_t __bg0806_mipi_parser_config(void *hdl)
{
	struct video_dev *bg0806 = (struct video_dev *)hdl;
	struct viss_source_info info;
	rt_uint32_t tmp_data = 0;
	const char *status;
	const char *i2c_bus_name;
	const char *module = DRV_MCSI_BG0806_NAME;
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
		LOG_W("bg0806 mipi disable.");
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
	rt_memcpy(&bg0806->info, &info, sizeof(struct viss_source_info));
	/* LOG_D("bg0806->info.if_type:%d, lane_num: %d.",
		bg0806->info.if_type, bg0806->info.data_lanes); */

	/* power pin */
	bg0806->pwdn_valid = 1;
	ret = config_get_u32_array(module, "mcsi-pwdn",
				bg0806->pwdn_val, ARRAY_SIZE(bg0806->pwdn_val));
	if (ret != ARRAY_SIZE(bg0806->pwdn_val)) {
		LOG_E("mcsi: power pin config error. ret:%d", ret);
		bg0806->pwdn_valid = 0;
	}
	/* LOG_D("bg0806 config power"); */

	/* reset pin */
	bg0806->rst_valid = 1;
	ret = config_get_u32_array(module, "mcsi-rst",
				bg0806->rst_val, ARRAY_SIZE(bg0806->rst_val));
	if (ret != ARRAY_SIZE(bg0806->rst_val)) {
		LOG_E("mcsi: reset pin config error. ret:%d", ret);
		bg0806->rst_valid = 0;
	}
	/* LOG_D("bg0806 config reset"); */

	/* mclk pin */
	bg0806->mclk_valid = 1;
	ret = config_get_u32_array(module, "mcsi-mclk",
				bg0806->mclk_val, ARRAY_SIZE(bg0806->mclk_val));
	if (ret != ARRAY_SIZE(bg0806->mclk_val)) {
		LOG_E("mcsi: mclk pin config error. ret:%d", ret);
		bg0806->mclk_valid = 0;
	}
/* #endif */
	/* LOG_D("bg0806 config finish"); */

	return RT_EOK;
}

static rt_err_t __bg0806_mipi_prepare(void *hdl)
{
	return __bg0806_mipi_parser_config(hdl);
}

static rt_err_t __bg0806_mipi_init(void *hdl)
{
	rt_int32_t ret = 0;
	rt_uint8_t tmp[2] = {0};
	rt_uint16_t id = 0;
	struct video_dev *bg0806 = (struct video_dev *)hdl;
	char *module = DRV_MCSI_BG0806_PWR_NAME;
	/* LOG_D("__bg0806_mipi_init in."); */

#if 1
	RT_ASSERT(RT_NULL != bg0806);
	bg0806->pctrl = pinctrl_get(DRV_MCSI_BG0806_NAME);
	if (RT_NULL == bg0806->pctrl)
		return -RT_ERROR;
	#ifdef ARCH_LOMBO_N7V1_CDR
	/* Init gpio */
	/* cam_power_enable(); */
	camera_ldo_set(vol_1p85v);
	cam_power_set(module);
	#endif
	if (1 == bg0806->pwdn_valid) {
		bg0806->pwdn_gpio = pinctrl_gpio_request(bg0806->pctrl,
					bg0806->pwdn_val[0], bg0806->pwdn_val[1]);
		if (bg0806->pwdn_gpio >= 0) {
			/* LOG_D("%d %d %d %d %d", bg0806->pwdn_val[2],
				bg0806->pwdn_val[3], bg0806->pwdn_val[4],
				bg0806->pwdn_val[5], bg0806->pwdn_val[6]); */
			pinctrl_gpio_set_function(bg0806->pctrl, bg0806->pwdn_gpio,
						bg0806->pwdn_val[2]);
			pinctrl_gpio_set_drv_level(bg0806->pctrl, bg0806->pwdn_gpio,
						bg0806->pwdn_val[3]);
			pinctrl_gpio_set_pud_mode(bg0806->pctrl, bg0806->pwdn_gpio,
						bg0806->pwdn_val[4]);
			pinctrl_gpio_set_pud_res(bg0806->pctrl, bg0806->pwdn_gpio,
						bg0806->pwdn_val[5]);
			pinctrl_gpio_set_value(bg0806->pctrl, bg0806->pwdn_gpio,
						bg0806->pwdn_val[6]);
		} else
			bg0806->pwdn_valid = 0;
	}

	if (1 == bg0806->rst_valid) {
		bg0806->rst_gpio = pinctrl_gpio_request(bg0806->pctrl,
					bg0806->rst_val[0], bg0806->rst_val[1]);
		if (bg0806->rst_gpio >= 0) {
			/* LOG_D("%d %d %d %d %d", bg0806->rst_val[2],
				bg0806->rst_val[3], bg0806->rst_val[4],
				bg0806->rst_val[5], bg0806->rst_val[6]); */
			pinctrl_gpio_set_function(bg0806->pctrl, bg0806->rst_gpio,
						bg0806->rst_val[2]);
			pinctrl_gpio_set_drv_level(bg0806->pctrl, bg0806->rst_gpio,
						bg0806->rst_val[3]);
			pinctrl_gpio_set_pud_mode(bg0806->pctrl, bg0806->rst_gpio,
						bg0806->rst_val[4]);
			pinctrl_gpio_set_pud_res(bg0806->pctrl, bg0806->rst_gpio,
						bg0806->rst_val[5]);
			pinctrl_gpio_set_value(bg0806->pctrl, bg0806->rst_gpio,
						bg0806->rst_val[6]);
		} else
			bg0806->rst_valid = 0;
	}

	if (1 == bg0806->mclk_valid) {
		bg0806->mclk_gpio = pinctrl_gpio_request(bg0806->pctrl,
					bg0806->mclk_val[0], bg0806->mclk_val[1]);
		if (bg0806->mclk_gpio >= 0) {
			pinctrl_gpio_set_function(bg0806->pctrl, bg0806->mclk_gpio,
						bg0806->mclk_val[2]);
			pinctrl_gpio_set_drv_level(bg0806->pctrl, bg0806->mclk_gpio,
						bg0806->mclk_val[3]);
			pinctrl_gpio_set_pud_mode(bg0806->pctrl, bg0806->mclk_gpio,
						bg0806->mclk_val[4]);
			pinctrl_gpio_set_pud_res(bg0806->pctrl, bg0806->mclk_gpio,
						bg0806->mclk_val[5]);
			pinctrl_gpio_set_value(bg0806->pctrl, bg0806->mclk_gpio,
						bg0806->mclk_val[6]);
		} else
			bg0806->mclk_valid = 0;
	}
	__bg0806_mipi_set_power(bg0806, RT_TRUE);
#endif

	bg0806->i2c_client = rt_zalloc(sizeof(struct viss_i2c_client));
	if (RT_NULL == bg0806->i2c_client)
		return -RT_ENOMEM;
	bg0806->i2c_client->i2c_bus = rt_i2c_bus_device_find(bg0806->info.i2c_bus_name);
	if (RT_NULL == bg0806->i2c_client->i2c_bus) {
		LOG_E("can't find bus dev \"%s\"", bg0806->info.i2c_bus_name);
		goto exit;
	}
	rt_thread_delay(10); /* 10ms */

	bg0806->i2c_client->i2c_addr = bg0806->info.i2c_addr;
	ret = viss_i2c_read_reg_16bit(bg0806->i2c_client, 0x0000, &tmp[0]);
	LOG_E("add: %x, ret: %d. 0x0000: %x", bg0806->i2c_client->i2c_addr, ret, tmp[0]);
	ret = viss_i2c_read_reg_16bit(bg0806->i2c_client, 0x0001, &tmp[1]);
	LOG_E("add: %x, ret: %d. 0x0001: %x", bg0806->i2c_client->i2c_addr, ret, tmp[1]);

	pre_index = 0;
	register_finished = 0;

	id = (tmp[0] << 8) | tmp[1];
	if (id != 0x0806) {
		LOG_E("ID wrong! (0x%x)\n", id);
		goto exit;
	}

	/* __bg0806_mipi_block_write((void *)bg0806,
		(void *)bg0806_reg_list, ARRAY_SIZE(bg0806_reg_list)); */

	return RT_EOK;
exit:
	if (bg0806->pctrl) {
		if (bg0806->rst_valid)
			pinctrl_gpio_free(bg0806->pctrl, bg0806->rst_gpio);
		if (bg0806->pwdn_valid)
			pinctrl_gpio_free(bg0806->pctrl, bg0806->pwdn_gpio);
		if (bg0806->mclk_valid)
			pinctrl_gpio_free(bg0806->pctrl, bg0806->mclk_gpio);
		pinctrl_put(bg0806->pctrl);
		bg0806->pctrl = RT_NULL;
	}
	if (bg0806->i2c_client) {
		rt_free(bg0806->i2c_client);
		bg0806->i2c_client = RT_NULL;
	}
	#ifdef ARCH_LOMBO_N7V1_CDR
	/* cam_power_disable(); */
	cam_power_exit(module);
	camera_ldo_exit();
	#endif
	return -RT_ERROR;
}

static void __bg0806_mipi_exit(void *hdl)
{
	struct video_dev *bg0806 = (struct video_dev *)hdl;
	char *module = DRV_MCSI_BG0806_PWR_NAME;

	__bg0806_mipi_set_power(bg0806, RT_FALSE);
	if (bg0806->pctrl) {
		if (bg0806->rst_valid)
			pinctrl_gpio_free(bg0806->pctrl, bg0806->rst_gpio);
		if (bg0806->pwdn_valid)
			pinctrl_gpio_free(bg0806->pctrl, bg0806->pwdn_gpio);
		if (bg0806->mclk_valid)
			pinctrl_gpio_free(bg0806->pctrl, bg0806->mclk_gpio);
		pinctrl_put(bg0806->pctrl);
		bg0806->pctrl = RT_NULL;
	}
	if (bg0806->i2c_client) {
		rt_free(bg0806->i2c_client);
		bg0806->i2c_client = RT_NULL;
	}
	reg_writed = 0;
	#ifdef ARCH_LOMBO_N7V1_CDR
	cam_power_exit(module);
	camera_ldo_exit();
	/* cam_power_disable(); */
	#endif
}

struct video_dev bg0806_mipi = {
	.name = DRV_MCSI_BG0806_NAME,
	.group_id = GRP_ID_MCSI,
	.prepare = __bg0806_mipi_prepare,
	.init = __bg0806_mipi_init,
	.exit = __bg0806_mipi_exit,
	.s_mode = __bg0806_mipi_set_mode,
	.g_cur_mode = __bg0806_mipi_cur_mode,
	.g_all_mode = __bg0806_mipi_get_all_mode,
	.s_power = __bg0806_mipi_set_power,
	.s_stream = __bg0806_mipi_set_stream,
	.g_info = __bg0806_mipi_get_info,
	.ioctl = __bg0806_mipi_ioctl,
	.s_register = __bg0806_mipi_set_register,
	.g_register = __bg0806_mipi_get_register,
};

