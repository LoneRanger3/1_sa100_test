/*
 * dpu_clk.h - Dpu clk module head file
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

#ifndef __DPU_CLK_H__
#define __DPU_CLK_H__
#include "csp_dpu_top.h"

#define DPU_MIN_DIV		1
#define DPU_MAX_DIV		DPU_CLK_DIV_MAX

typedef struct tag_lombo_dup_clk {
	bool enable; /* 0 is disable, 1 is enable */
	u32 freq; /* the clk freq to set if the freq can be setted */
	const char *self; /* self clk name */
	const char *parent;/* parent clk name */
} lombo_dup_clk_t;

typedef enum tag_dpu_clk_src {
	DPU_SCLK0,
	DPU_SCLK1,
	DPU_SCLK2,
	DPU_SCLK_MAX
} dpu_clk_src_t;

enum {
	DPU_APDLL0,
	DPU_APDLL1
};

typedef struct tag_dpu_mod_clk {
	u32 parent; /* module parent clk, sclk0 or sclk1  */
	u32 div; /* divider, [DPU_MIN_DIV, DPU_MAX_DIV] */
	u32 freq;
} dpu_mod_clk_t;

typedef struct tag_dpu_sclk {
	bool enable; /* clk is valid when enable is true */
	u32 freq; /*clk frequent*/
	const char *parent; /* parent clk name */
	const char *self; /* self clk name */
} dpu_sclk_t;

typedef struct tag_dpu_clk_cfg {
	bool is_initial;
	bool is_sclk_select_adpll[DPU_ADPLL_NUM];
	bool is_need_adpll[DPU_ADPLL_NUM];
	struct dpu_adpll_config dac[DPU_ADPLL_NUM]; /* SCLK0 and SCLK1*/
	dpu_mod_clk_t mod_clk[DPU_MOD_SUB_MAX];
	dpu_sclk_t sclk[DPU_SCLK_MAX];
	u32 internal_sclk_freq[DPU_SCLK_MAX - 1];
} dpu_clk_cfg_t;

int dpu_clk_init(void);
int dpu_clk_uninit(void);
int dpu_prepare_clk(dpu_mod mod);
int dpu_unprepare_clk(dpu_mod mod);
float get_dpu_mod_clk_freq(dpu_mod mod);

#endif

