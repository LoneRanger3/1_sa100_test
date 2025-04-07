/*
 * gpadc_drv.c - GPADC driver module
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

#include "gpadc_drv.h"
#include <debug.h>
#include "clk/clk.h"

/* setup gpadc clock */
void csp_gpadc_clk_cfg(void)
{
	clk_handle_t clk_gate, clk_reset;
	rt_err_t ret;

	clk_gate = clk_get(CLK_NAME_APB_GPADC_GATE);
	if (clk_gate == RT_NULL) {
		LOG_E("clk_get CLK_NAME_APB_GPADC_GATE return NULL");
		return;
	}

	ret = clk_enable(clk_gate);
	if (ret != 0) {
		LOG_E("clk_enable clk_gate error");
		return;
	}

	clk_reset = clk_get(CLK_NAME_APB_GPADC_RESET);
	if (clk_reset == RT_NULL) {
		LOG_E("clk_get CLK_NAME_APB_GPADC_RESET return NULL");
		return;
	}

	ret = clk_enable(clk_reset);
	if (ret != 0) {
		LOG_E("clk_enable clk_reset error");
		return;
	}

#ifdef ARCH_LOMBO_N7V1
	/* add GPADC module clock at n7v1 */
	clk_handle_t gpadc_clk, parent;
	gpadc_clk = clk_get(CLK_NAME_GPADC_CLK);
	if (gpadc_clk == RT_NULL) {
		LOG_E("clk_get CLK_NAME_GPADC_CLK return NULL");
		return;
	}
	clk_disable(gpadc_clk);

	parent = clk_get(CLK_NAME_OSC24M);
	if (parent == RT_NULL) {
		LOG_E("clk_get parrent return NULL");
		return;
	}

	ret = clk_set_parent(gpadc_clk, parent);
	if (ret != 0) {
		LOG_E("clk_set_parent error: %d", ret);
		return;
	}

	ret = clk_enable(gpadc_clk);
	if (ret != 0) {
		LOG_E("clk_enable gpadc_clk error");
		return;
	}
#endif
}

int rt_hw_gpadc_init(void)
{
	u32 val;

	csp_gpadc_clk_cfg();

	/* set debug enable */
	val = READREG32(VA_GPADC_DBG0);
	val |= 0x1;
	WRITEREG32(VA_GPADC_DBG0, val);

	/* set debug1 register 8-15bit to 0xff */
	val = READREG32(VA_GPADC_DBG1);
	val |= 0xff00;
	WRITEREG32(VA_GPADC_DBG1, val);

	csp_gpadc_set_en(RT_TRUE); /* GPADC enable set */
	csp_gpadc_set_sample_rate(GPADC_SRC_512HZ);
	csp_gpadc_sda_en(RT_TRUE);

	return 0;
}

INIT_PREV_EXPORT(rt_hw_gpadc_init);

