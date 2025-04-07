/*
 * timer.h - head file for timer module
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

#include <rtthread.h>
#include <rthw.h>
#include <debug.h>
#include <irq_numbers.h>
#include <csp.h>

#include "../board.h"

/* gtimer count in lombo platform.. */
#define GTMR_TOTAL		4

/* clk src, division, and rate */
#define CLK_SRC_HFEOSC		0
#define CLK_DIV1		0
#define HFEOSC_RATE		24000000

/* timer counter direction */
#define DOWN_CNT		0
#define UP_CNT			1

/* timer counter load mode */
#define MANUAL_MODE		0
#define AUTO_MODE		1

/* chip operations */
void csp_gtmr_irq_enable(int gtmr_no);
void csp_gtmr_irq_disable(int gtmr_no);
u32 csp_gtmr_get_ctrl(int gtmr_no);
void csp_gtmr_enable(int gtmr_no);
void csp_gtmr_disable(int gtmr_no);
void csp_gtmr_set_cnt_mode(int gtmr_no, u32 dire, u32 mode);
void csp_gtmr_set_store(int gtmr_no, u32 val);
void csp_gtmr_set_clk(int gtmr_no, u32 src, u32 div);
void csp_gtmr_clr_int_pend(int gtmr_no);
u32 csp_gtmr_get_int_pend(int gtmr_no);
void csp_vtmr_set_interval(void);
void csp_vtmr_start(void);
void csp_vtmr_stop(void);

