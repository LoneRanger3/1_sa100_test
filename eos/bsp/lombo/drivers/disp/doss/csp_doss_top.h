/*
 * csp_doss_top.h - Doss top module head file
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

#ifndef __CSP_DOSS_TOP_H__
#define __CSP_DOSS_TOP_H__

#include "../disp_common.h"
#include "csp_doss_top_reg.h"

#define DOSS_MAX_INST (2)

#define DOSS_PATH_SIZE			(0x020)
#define DOSS_TCON_CTRL_OFF		(0x100)
#define DOSS_TCON_CLK_OFF		(0x104)
#define DOSS_LVDS_CTRL_OFF		(0x140)
#define DOSS_LVDS_CLK_OFF		(0x144)
#define DOSS_MIPI_DSI_CTRL_OFF		(0x180)
#define DOSS_MIPI_DSI_REF_CLK_OFF	(0x184)
#define DOSS_MIPI_DSI_CFG_CLK_OFF	(0x188)
#define DOSS_TVE_CTRL_OFF		(0x400)
#define DOSS_TVE_CLK_OFF		(0x404)

#define DOSS_SRC_DC0		DOSS_ROUTE_SRC_DO0_0
#define DOSS_SRC_DC1		DOSS_ROUTE_SRC_DO0_1
#define DOSS_CLK_ON		(1)
#define DOSS_CLK_OFF		(0)
#define DOSS_CTRL_ON		(1)
#define DOSS_CTRL_OFF		(0)

/* Custom define, DOSS Path Control Register */
typedef union {
	u32 val;
	struct {
	u32 slave:1;            /* Slave Mode Enable
				 * [0x0]: Disable
				 * [0x1]: Enable
				 * Enable when dual channel mode */
	u32 rsvd0:26;           /* - */
	u32 gate:1;             /* Data Path Gating
				 * [0x0]: Data Path Active
				 * [0x1]: Data Path Inactive */
	u32 rsvd1:3;            /* - */
	u32 rst:1;              /* Software Reset
				 * [0x0]: Reset Hold
				 * [0x1]: Reset Release */
	} bits;
} reg_doss_path_ctrl_t;

/* Custom define, DOSS Path Clock Register */
typedef union {
	u32 val;
	struct {
	u32 div0:4;             /* Clock Divider 0
				 * Divider = DIV0 + 1 */
	u32 rsvd0:4;            /* - */
	u32 div1:8;            /* Clock Divider 1
				 * Divider = DIV1 + 1 */
	u32 src:4;            /* Clock Source */
	u32 rsvd1:11;           /* - */
	u32 gate:1;             /* Clock On/Off
				 * [0x0]: Clock Off
				 * [0x1]: Clock On */
	} bits;
} reg_doss_path_clk_t;

typedef enum doss_ctrl_id {
	DOSS_CTRL_TCON		= DOSS_TCON_CTRL_OFF,
	DOSS_CTRL_LVDS		= DOSS_LVDS_CTRL_OFF,
	DOSS_CTRL_MIPI_DSI	= DOSS_MIPI_DSI_CTRL_OFF,
	DOSS_CTRL_TVE		= DOSS_TVE_CTRL_OFF,
} doss_ctrl_id_t;

typedef enum doss_clk_id {
	DOSS_CLK_TCON		= DOSS_TCON_CLK_OFF,
	DOSS_CLK_LVDS		= DOSS_LVDS_CLK_OFF,
	DOSS_CLK_MIPI_DSI_REF	= DOSS_MIPI_DSI_REF_CLK_OFF,
	DOSS_CLK_MIPI_DSI_CFG	= DOSS_MIPI_DSI_CFG_CLK_OFF,
	DOSS_CLK_TVE		= DOSS_TVE_CLK_OFF,
} doss_clk_id_t;

typedef enum doss_clk_src {
	DOSS_CLK_HFEOSC		= 0x00, /* TODO */
	DOSS_CLK_SCLK0		= 0x01, /* TODO */
} doss_clk_src_t;

typedef enum doss_input {
	DOSS_IN_DC0		= DOSS_SRC_DC0,
	DOSS_IN_DC1		= DOSS_SRC_DC1,
} doss_input_t;

void csp_doss_set_register_base(u32 doss_idx, void *addr, u32 size);
void csp_doss_get_register_base(u32 doss_idx, unsigned long *addr);
void csp_doss_init(u32 doss_idx);
void csp_doss_exit(u32 doss_idx);
void csp_doss_clk_enable(u32 doss_idx, u32 path_id, doss_clk_id_t clk_id);
void csp_doss_clk_disable(u32 doss_idx, u32 path_id, doss_clk_id_t clk_id);
void csp_doss_clk_set(u32 doss_idx, u32 path_id, doss_clk_id_t clk_id,
	doss_clk_src_t src, u8 div0, u8 div1);
void csp_doss_path_enable(u32 doss_idx, u32 path_id, doss_ctrl_id_t ctrl_id);
void csp_doss_path_disable(u32 doss_idx, u32 path_id, doss_ctrl_id_t ctrl_id);
void csp_doss_path_init(u32 doss_idx, u32 path_id, doss_ctrl_id_t ctrl_id);
void csp_doss_path_exit(u32 doss_idx, u32 path_id, doss_ctrl_id_t ctrl_id);
void csp_doss_set_input(u32 doss_idx, u32 path_id, doss_input_t input);
void csp_doss_io_set_output(u32 doss_idx, u32 func_id);
void csp_doss_io_set_val(u32 doss_idx, u32 func_id, u32 val);


#endif /* __CSP_DOSS_TOP_H__ */
