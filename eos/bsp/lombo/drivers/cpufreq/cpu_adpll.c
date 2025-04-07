/*
 * cpu-adpll.c - Driver of CPU ADPLL
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

#include <csp.h>
#include <debug.h>

static void wait_for_adpll_lock(void)
{
	reg_cpu_ctrl_adpll_stat_t adpll_status;
	u32 reg_val;

	do {
		adpll_status.val = READREG32(VA_CPU_CTRL_ADPLL_STAT);
		reg_val = adpll_status.bits.flock & adpll_status.bits.plock;
	} while (reg_val != 1);
}

int csp_set_cpu_rate(unsigned int freq)
{
	int err = 0;
	reg_cpu_ctrl_adpll_fac_t adpll_fac;
	reg_cpu_ctrl_cpu_clk_sel_t clk_sel;

	clk_sel.val     = READREG32(VA_CPU_CTRL_CPU_CLK_SEL);
	clk_sel.bits.en = 1;

	adpll_fac.val   = READREG32(VA_CPU_CTRL_ADPLL_FAC);

	if ((freq >= 240 * 1000 * 1000) && (freq <= 1200 * 1000 * 1000)) {
		adpll_fac.bits.n = freq / 24000000;
		clk_sel.bits.src_div = 0;
	} else if ((freq >= 120 * 1000 * 1000) && (freq < 240 * 1000 * 1000)) {
		adpll_fac.bits.n     = (freq * 2) / 24000000;
		clk_sel.bits.src_div = 1;
	} else if ((freq >= 60 * 1000 * 1000) && (freq < 120 * 1000 * 1000)) {
		adpll_fac.bits.n     = (freq * 4) / 24000000;
		clk_sel.bits.src_div = 2;
	} else {
		LOG_E("invalid freq: %u\n", freq);
		err = __LINE__;
		goto out;
	}

	WRITEREG32(VA_CPU_CTRL_ADPLL_FAC, adpll_fac.val);

	WRITEREG32(VA_CPU_CTRL_CPU_CLK_SEL, clk_sel.val);

	wait_for_adpll_lock();

out:
	return err;
}

unsigned int csp_get_cpu_rate(void)
{
	unsigned int adpll_rate = 0;
	unsigned int cpu_rate = 0;
	unsigned int src_div = 0;
	reg_cpu_ctrl_adpll_fac_t adpll_fac;
	reg_cpu_ctrl_cpu_clk_sel_t clk_sel;

	clk_sel.val   = READREG32(VA_CPU_CTRL_CPU_CLK_SEL);
	adpll_fac.val = READREG32(VA_CPU_CTRL_ADPLL_FAC);

	adpll_rate = (24 / (adpll_fac.bits.pre_div + 1))
				* adpll_fac.bits.n * 1000 * 1000;

	if (clk_sel.bits.src_div == 0)
		src_div = 1;
	else if (clk_sel.bits.src_div == 1)
		src_div = 2;
	else if ((clk_sel.bits.src_div == 2) || (clk_sel.bits.src_div == 3))
		src_div = 4;

	cpu_rate = adpll_rate / src_div;

	return cpu_rate;
}

