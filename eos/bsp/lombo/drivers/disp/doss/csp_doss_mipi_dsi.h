/*
 * csp_doss_mipi_dsi.h - Doss mipi dsi module head file
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

#ifndef __CSP_DOSS_MIPI_DSI_H__
#define __CSP_DOSS_MIPI_DSI_H__

#include "csp_doss_tcon.h"
#include "csp_doss_mipi_dsi_reg.h"

#ifndef ARRAY_CNT
#define ARRAY_CNT(_A) (sizeof(_A) / sizeof((_A)[0]))
#endif /* ARRAY_CNT */
#define CSP_MIPI_DSI_VERSION_NONE	1
#define CSP_MIPI_DSI_VERSION_V10	2

#define TRUE			1
#define FALSE			0

/* Enable/Disable */
#define DSI_DISABLE			(0)
#define DSI_ENABLE			(1)

/* custom define */
#define DSI_REG_NON_BURST_PULSE		MIPI_DSI_VM_CFG_VM_TYPE_0
#define DSI_REG_NON_BURST_EVENT		MIPI_DSI_VM_CFG_VM_TYPE_1
#define DSI_REG_BURST_PULSE		MIPI_DSI_VM_CFG_VM_TYPE_2

#define DSI_REG_DPI_RGB888		MIPI_DSI_DPI_CFG_CC_0
#define DSI_REG_DPI_RGB666		MIPI_DSI_DPI_CFG_CC_1
#define DSI_REG_DPI_RGB565		MIPI_DSI_DPI_CFG_CC_2

#define DSI_REG_DBI_RGB565		MIPI_DSI_DBI_CFG_IN_FMT_8
#define DSI_REG_DBI_RGB666		MIPI_DSI_DBI_CFG_IN_FMT_9
#define DSI_REG_DBI_GB888		MIPI_DSI_DBI_CFG_IN_FMT_B

#define DSI_REG_DBI_LUT_16BPP		MIPI_DSI_DBI_CFG_LUT_SIZE_0
#define DSI_REG_DBI_LUT_18BPP		MIPI_DSI_DBI_CFG_LUT_SIZE_1
#define DSI_REG_DBI_LUT_24BPP		MIPI_DSI_DBI_CFG_LUT_SIZE_2

#define MIPI_DSI_MAX_INST (2)

typedef struct dsi_pll_freq_map {
	u32 freq;
	u32 freq_code;
} dsi_pll_freq_map_t;

typedef enum dsi_dt {
	DSI_DT_VSS		=	0x01,
	DSI_DT_VSE		=	0x11,
	DSI_DT_HSS		=	0x21,
	DSI_DT_HSE		=	0x31,
	DSI_DT_EOTP		=	0x08,
	DSI_DT_CM_OFF		=	0x02,
	DSI_DT_CM_ON		=	0x12,
	DSI_DT_SHUT		=	0x22,
	DSI_DT_TURN_ON		=	0x32,
	DSI_DT_GEN_SW0P		=	0x03,
	DSI_DT_GEN_SW1P		=	0x13,
	DSI_DT_GEN_SW2P		=	0x23,
	DSI_DT_GEN_RD0P		=	0x04,
	DSI_DT_GEN_RD1P		=	0x14,
	DSI_DT_GEN_RD2P		=	0x24,
	DSI_DT_DCS_SW0P		=	0x05,
	DSI_DT_DCS_SW1P		=	0x15,
	DSI_DT_DCS_RD0P		=	0x06,
	DSI_DT_NULL_PKT		=	0x09,
	DSI_DT_BLNK_PKT		=	0x19,
	DSI_DT_GEN_LW		=	0x29,
	DSI_DT_DCS_LW		=	0x39,
	DSI_DT_RGB565		=	0x0E,
	DSI_DT_RGB666		=	0x1E,
	DSI_DT_RGB666_L		=	0x2E,
	DSI_DT_RGB888		=	0x3E,
} dsi_dt_t;

typedef enum dcs_cmd {
	DCS_ENTER_IDLE_MODE		=	0x39 ,
	DCS_ENTER_INVERT_MODE		=	0x21 ,
	DCS_ENTER_NORMAL_MODE		=	0x13 ,
	DCS_ENTER_PARTIAL_MODE		=	0x12 ,
	DCS_ENTER_SLEEP_MODE		=	0x10 ,
	DCS_EXIT_IDLE_MODE		=	0x38 ,
	DCS_EXIT_INVERT_MODE		=	0x20 ,
	DCS_EXIT_SLEEP_MODE		=	0x11 ,
	DCS_GET_ADDRESS_MODE		=	0x0b ,
	DCS_GET_BLUE_CHANNEL		=	0x08 ,
	DCS_GET_DIAGNOSTIC_RESULT	=	0x0f ,
	DCS_GET_DISPLAY_MODE		=	0x0d ,
	DCS_GET_GREEN_CHANNEL		=	0x07 ,
	DCS_GET_PIXEL_FORMAT		=	0x0c ,
	DCS_GET_POWER_MODE		=	0x0a ,
	DCS_GET_RED_CHANNEL		=	0x06 ,
	DCS_GET_SCANLINE		=	0x45 ,
	DCS_GET_SIGNAL_MODE		=	0x0e ,
	DCS_NOP				=	0x00 ,
	DCS_READ_DDB_CONTINUE		=	0xa8 ,
	DCS_READ_DDB_START		=	0xa1 ,
	DCS_READ_MEMORY_CONTINUE	=	0x3e ,
	DCS_READ_MEMORY_START		=	0x2e ,
	DCS_SET_ADDRESS_MODE		=	0x36 ,
	DCS_SET_COLUMN_ADDRESS		=	0x2a ,
	DCS_SET_DISPLAY_OFF		=	0x28 ,
	DCS_SET_DISPLAY_ON		=	0x29 ,
	DCS_SET_GAMMA_CURVE		=	0x26 ,
	DCS_SET_PAGE_ADDRESS		=	0x2b ,
	DCS_SET_PARTIAL_AREA		=	0x30 ,
	DCS_SET_PIXEL_FORMAT		=	0x3a ,
	DCS_SET_SCROLL_AREA		=	0x33 ,
	DCS_SET_SCROLL_START		=	0x37 ,
	DCS_SET_TEAR_OFF		=	0x34 ,
	DCS_SET_TEAR_ON			=	0x35 ,
	DCS_SET_TEAR_SCANLINE		=	0x44 ,
	DCS_SOFT_RESET			=	0x01 ,
	DCS_WRITE_LUT			=	0x2d ,
	DCS_WRITE_MEMORY_CONTINUE	=	0x3c ,
	DCS_WRITE_MEMORY_START		=	0x2c ,
} dcs_cmd_t;

typedef enum dsi_tx_fmt {
	DSI_RGB888		=	0x0,
	DSI_RGB666		=	0x1,
	DSI_RGB565		=	0x2,
	DSI_RGB666_L		=	0x3,
} dsi_tx_fmt_t;

typedef enum dsi_tx_mode {
	DSI_VIDEO_MODE		=	0x0,
	DSI_CMD_MODE		=	0x1,
} dsi_tx_mode_t;

typedef enum dsi_trans_mode {
	DSI_NON_BURST_PULSE	=	DSI_REG_NON_BURST_PULSE,
	DSI_NON_BURST_EVENT	=	DSI_REG_NON_BURST_EVENT,
	DSI_BURST		=	DSI_REG_BURST_PULSE,
} dsi_trans_mode_t;

typedef struct dsi_video_mode {
	dsi_trans_mode_t	trans_mode;
} dsi_video_mode_t;

typedef struct dsi_command_mode {
	bool			is_te;
} dsi_command_mode_t;

typedef union {
	dsi_video_mode_t	video_mode;
	dsi_command_mode_t	cmd_mode;
} dsi_tx_mode_cfg_t;

typedef struct tcon_dsi_if {
	u32			index;
	bool			is_dual;
	bool			is_bta;
	u32			lane_num;
	dsi_tx_fmt_t		tx_fmt;
	dsi_tx_mode_t		tx_mode;
	dsi_tx_mode_cfg_t	tx_mode_cfg;
	tcon_host_t		*tcon_host;
	bool			is_bitrate_fixed;
	u32			bit_clk;
	u32			dsi_ref_div;
	u32			dsi_ref_freq;
} tcon_mipi_dsi_if_t;

void csp_mipi_dsi_set_version(u32 csp_mipi_dsi_ver);
s32 csp_mipi_dsi_set_register_base(u32 index, void *addr, u32 size);
s32 csp_mipi_dsi_get_register_base(u32 index, unsigned long *addr);
s32 csp_mipi_dsi_init(u32 index, tcon_mipi_dsi_if_t *dsi_if);
s32 csp_mipi_dsi_exit(u32 index, tcon_mipi_dsi_if_t *dsi_if);
s32 csp_mipi_dsi_irq_enable(u32 index, u32 irqnr);
s32 csp_mipi_dsi_irq_disable(u32 index, u32 irqnr);
bool csp_mipi_dsi_query_irq(u32 index, u32 irqnr);
s32 csp_mipi_dsi_irq_clear(u32 index, u32 irqnr);
s32 csp_mipi_dsi_start(u32 index);
s32 csp_mipi_dsi_stop(u32 index);
s32 csp_mipi_dsi_reset(u32 index);
void csp_mipi_dsi_cfg(u32 index, tcon_mipi_dsi_if_t *dsi_if);
void csp_mipi_dsi_dcs_sw_0p(u32 index, u8 vc, u8 cmd);
void csp_mipi_dsi_dcs_sw_1p(u32 index, u8 vc, u8 cmd, u8 para);
void csp_mipi_dsi_dcs_lw(u32 index, u8 vc, u8 *data, u32 lenth);

u64 csp_mipi_dsi_get_ref_clk_rate(u32 index, u8 is_bitrate_fixed,
	u32 dclk_freq);

#endif /*__CSP_DOSS_MIPI_DSI_H__ */
