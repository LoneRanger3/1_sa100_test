/*
 * Copyright (C) 2016-2018, LomboTech Co.Ltd.
 * Author: lomboswer <lomboswer@lombotech.com>
 *
 * Lombo n7 VISS-TOP controller register definitions
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


#include "viss_top_csp.h"
#include <div.h>

typedef struct viss_mclk_cfg {
	u32	freq;
	u32	src;
	u32	div;
} viss_mclk_cfg_t;

viss_mclk_cfg_t g_mclk_freq[] = {
	{ 6000000,	0,	4,},
	{ 8000000,	0,	3,},
	{ 12000000,	0,	2,},
	{ 24000000,	0,	1,},
	{ 27000000,	1,	22,},
	{ 37125000,	1,	16,},
};


reg_viss_t *g_viss = (reg_viss_t *)BASE_VISS;

void csp_viss_top_set_register_base(void *base)
{
	g_viss = (reg_viss_t *)base;
}

/**
 * Select PLL_MUX clock source
 * @src: PLLMUX_CLK_SEL
 *	0: VC_PLL
 *	1: ADPLL
 */
s32 csp_viss_top_pllmux_clock_source(u32 src)
{
	reg_viss_pll_mux_ctrl_t tmpreg;

	PRT_TRACE_BEGIN("src=%d\n", src);
	tmpreg.val = READREG32(&(g_viss->pll_mux_ctrl));
	tmpreg.bits.pllmux_clk_sel = src;
	WRITEREG32(&(g_viss->pll_mux_ctrl), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Setup ADPLL pre-divider value: 1 or 2
 * @div: ADPLL_PRE_DIV
 *	0: pre-divider is 1
 *	1: pre-divider is 2
 */
s32 csp_viss_top_adpll_prediv(u32 div)
{
	reg_viss_pll_mux_ctrl_t tmpreg;

	PRT_TRACE_BEGIN("div=%d\n", div);
	tmpreg.val = READREG32(&(g_viss->pll_mux_ctrl));
	tmpreg.bits.adpll_pre_div = div - 1;
	WRITEREG32(&(g_viss->pll_mux_ctrl), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Setup ADPLL
 * Notes:
 *	a) The frequency should less than 1008MHz
 *	b) Before modify factor dynamically, gating the clock the working module
 */
s32 csp_viss_top_setup_adpll(u32 clk_freq)
{
	u32 clk_step;
	u32 factor;

	reg_viss_adpll_tune0_t tmpreg0;
	reg_viss_adpll_tune1_t tmpreg1;
	reg_viss_adpll_fac_t tmpreg2;
	reg_viss_pll_mux_ctrl_t tmpreg3;

	PRT_TRACE_BEGIN("clk_freq=%d\n", clk_freq);
	tmpreg0.val = 0x139F0000;
	tmpreg1.val = 0x57379120;
	WRITEREG32(&(g_viss->adpll_tune0), tmpreg0.val);
	WRITEREG32(&(g_viss->adpll_tune1), tmpreg1.val);
	tmpreg3.val = READREG32(&(g_viss->pll_mux_ctrl));
	if (tmpreg3.bits.adpll_pre_div)
		clk_step = 12000000;
	else
		clk_step = 24000000;

	/* factor should be greater than 5, and less than 256 */
	factor = clk_freq;
	do_div(factor, clk_step);
	/* factor = clk_freq / clk_step; */
	tmpreg2.val = 0;
	tmpreg2.bits.n = factor;
	WRITEREG32(&(g_viss->adpll_fac), tmpreg2.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Enable ADPLL
 */
s32 csp_viss_top_adpll_enable(void)
{
	reg_viss_adpll_fac_t tmpreg;

	PRT_TRACE_BEGIN("\n");
	tmpreg.val = READREG32(&(g_viss->adpll_fac));
	tmpreg.bits.en = 1;
	WRITEREG32(&(g_viss->adpll_fac), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Disable ADPLL
 */
s32 csp_viss_top_adpll_disable(void)
{
	reg_viss_adpll_fac_t tmpreg;

	PRT_TRACE_BEGIN("\n");
	tmpreg.val = READREG32(&(g_viss->adpll_fac));
	tmpreg.bits.en = 0;
	WRITEREG32(&(g_viss->adpll_fac), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * ADPLL frequency lock status
 */
u32 csp_viss_top_adpll_lock(void)
{
	reg_viss_adpll_stat_t tmpreg;

	PRT_TRACE_BEGIN("\n");
	tmpreg.val = READREG32(&(g_viss->adpll_stat));
	PRT_TRACE_END("tmpreg.bits.flock=%d\n", tmpreg.bits.flock);

	return tmpreg.bits.flock && tmpreg.bits.plock;
}

/**
 * Init VIC module
 */
s32 csp_viss_top_vic_init(u32 clk_freq)
{
	u32 clk_div;

	reg_viss_vic_cfg_t tmpreg;

	PRT_TRACE_BEGIN("clk_freq=%d\n", clk_freq);
#if 0
	if (clk_freq > 297000000)
		clk_div = 1;
	else if (clk_freq > 198000000)
		clk_div = 2;
	else if (clk_freq > 148500000)
		clk_div = 3;
	else if (clk_freq > 118800000)
		clk_div = 4;
	else if (clk_freq > 99000000)
		clk_div = 5;
	else if (clk_freq > 84857000)
		clk_div = 6;
	else if (clk_freq > 74250000)
		clk_div = 7;
	else
		clk_div = 8;
#else
	clk_div = 594000000;
	do_div(clk_div, clk_freq);
	/* clk_div = 594000000 / clk_freq; */
	if (clk_div > 8)
		clk_div = 8;
#endif

	/* setup module clock divider */
	tmpreg.val = 0;
	tmpreg.bits.clk_dir = clk_div - 1;
	WRITEREG32(&(g_viss->vic_cfg), tmpreg.val);

	/* setup module clock gating */
	tmpreg.val = READREG32(&(g_viss->vic_cfg));
	tmpreg.bits.clk_gat = 1;
	WRITEREG32(&(g_viss->vic_cfg), tmpreg.val);

	/* setup AHB clock gating */
	tmpreg.val = READREG32(&(g_viss->vic_cfg));
	tmpreg.bits.hclk_gat = 1;
	WRITEREG32(&(g_viss->vic_cfg), tmpreg.val);

	/* Release module reset */
	tmpreg.val = READREG32(&(g_viss->vic_cfg));
	tmpreg.bits.rst = 1;
	WRITEREG32(&(g_viss->vic_cfg), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Init MCSI module
 */
s32 csp_viss_top_mcsi_init(u32 clk_freq)
{
	u32 clk_div;

	reg_viss_mcsi_cfg_t tmpreg;

	/* clk_src default 0 */

	PRT_TRACE_BEGIN("clk_freq=%d\n", clk_freq);
#if 0
	if (clk_freq > 297000000)
		clk_div = 1;
	else if (clk_freq > 198000000)
		clk_div = 2;
	else if (clk_freq > 148500000)
		clk_div = 3;
	else if (clk_freq > 118800000)
		clk_div = 4;
	else if (clk_freq > 99000000)
		clk_div = 5;
	else if (clk_freq > 84857000)
		clk_div = 6;
	else if (clk_freq > 74250000)
		clk_div = 7;
	else
		clk_div = 8;
#else
	clk_div = 594000000;
	do_div(clk_div, clk_freq);
	/* clk_div = 594000000 / clk_freq; */
	if (clk_div > 8)
		clk_div = 8;
#endif

	/* setup module clock divider */
	tmpreg.val = 0;
	tmpreg.bits.clk_dir = clk_div - 1;
	WRITEREG32(&(g_viss->mcsi_cfg), tmpreg.val);

	/* setup module clock gating */
	tmpreg.val = READREG32(&(g_viss->mcsi_cfg));
	tmpreg.bits.clk_gat = 1;
	WRITEREG32(&(g_viss->mcsi_cfg), tmpreg.val);

	/* setup module config clock gating */
	tmpreg.val = READREG32(&(g_viss->mcsi_cfg));
	tmpreg.bits.cfgclk_gat = 1;
	WRITEREG32(&(g_viss->mcsi_cfg), tmpreg.val);

	/* setup AHB clock gating */
	tmpreg.val = READREG32(&(g_viss->mcsi_cfg));
	tmpreg.bits.hclk_gat = 1;
	WRITEREG32(&(g_viss->mcsi_cfg), tmpreg.val);

	/* Release module reset */
	tmpreg.val = READREG32(&(g_viss->mcsi_cfg));
	tmpreg.bits.rst = 1;
	WRITEREG32(&(g_viss->mcsi_cfg), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

u32 csp_viss_top_mcsi_reset(void)
{
	reg_viss_mcsi_cfg_t tmpreg;

	/* assert module reset */
	tmpreg.val = READREG32(&(g_viss->mcsi_cfg));
	tmpreg.bits.rst = 0;
	WRITEREG32(&(g_viss->mcsi_cfg), tmpreg.val);

	/* disable module clock gating */
	tmpreg.val = READREG32(&(g_viss->mcsi_cfg));
	tmpreg.bits.clk_gat = 0;
	WRITEREG32(&(g_viss->mcsi_cfg), tmpreg.val);

	/* disable AHB clock gating */
	tmpreg.val = READREG32(&(g_viss->mcsi_cfg));
	tmpreg.bits.hclk_gat = 0;
	WRITEREG32(&(g_viss->mcsi_cfg), tmpreg.val);

	/* setup module clock gating */
	tmpreg.val = READREG32(&(g_viss->mcsi_cfg));
	tmpreg.bits.clk_gat = 1;
	WRITEREG32(&(g_viss->mcsi_cfg), tmpreg.val);

	/* setup AHB clock gating */
	tmpreg.val = READREG32(&(g_viss->mcsi_cfg));
	tmpreg.bits.hclk_gat = 1;
	WRITEREG32(&(g_viss->mcsi_cfg), tmpreg.val);

	/* Release module reset */
	tmpreg.val = READREG32(&(g_viss->mcsi_cfg));
	tmpreg.bits.rst = 1;
	WRITEREG32(&(g_viss->mcsi_cfg), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Init TVD module, the typical frequency is 27MHz
 */
s32 csp_viss_top_tvd_init(void)
{
	u32 clk_div;

	reg_viss_tvd_cfg_t tmpreg;

	PRT_TRACE_BEGIN("\n");
	clk_div = 22;

	/* setup module clock divider */
	tmpreg.val = 0;
	tmpreg.bits.clk_dir = clk_div - 1;
	WRITEREG32(&(g_viss->tvd_cfg), tmpreg.val);
#if 1
	/* setup module clock gating */
	tmpreg.val = READREG32(&(g_viss->tvd_cfg));
	tmpreg.bits.clk_gat = 1;
	WRITEREG32(&(g_viss->tvd_cfg), tmpreg.val);
#endif
	/* setup AHB clock gating */
	tmpreg.val = READREG32(&(g_viss->tvd_cfg));
	tmpreg.bits.hclk_gat = 1;
	WRITEREG32(&(g_viss->tvd_cfg), tmpreg.val);

	/* Release module reset */
	tmpreg.val = READREG32(&(g_viss->tvd_cfg));
	tmpreg.bits.rst = 1;
	WRITEREG32(&(g_viss->tvd_cfg), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Init ISP module,
 * The max frequency should be less than 288MHz
 * The min frequency should be more than 99MHz
 */
s32 csp_viss_top_isp_init(u32 clk_freq)
{
	u32 clk_div;
	/* u32 clk_src = 0; */
	/* u32 lock_sta; */

	reg_viss_isp_cfg_t tmpreg;

	PRT_TRACE_BEGIN("clk_freq=%d\n", clk_freq);

	if (clk_freq > 297000000)
		clk_div = 2;
	else if (clk_freq > 198000000)
		clk_div = 2;
	else if (clk_freq > 148500000)
		clk_div = 3;
	else if (clk_freq > 118800000)
		clk_div = 4;
	else if (clk_freq > 99000000)
		clk_div = 5;
	else
		clk_div = 6;
	/* setup module clock divider */
	tmpreg.val = 0;
	tmpreg.bits.clk_dir = clk_div - 1;
	WRITEREG32(&(g_viss->isp_cfg), tmpreg.val);
	/* setup module clock source */
	tmpreg.val = READREG32(&(g_viss->isp_cfg));
	tmpreg.bits.clk_src = 1;
	WRITEREG32(&(g_viss->isp_cfg), tmpreg.val);
	/* setup module clock gating */
	tmpreg.val = READREG32(&(g_viss->isp_cfg));
	tmpreg.bits.clk_gat = 1;
	WRITEREG32(&(g_viss->isp_cfg), tmpreg.val);
	/* setup AHB clock gating */
	tmpreg.val = READREG32(&(g_viss->isp_cfg));
	tmpreg.bits.hclk_gat = 1;
	WRITEREG32(&(g_viss->isp_cfg), tmpreg.val);
	/* Release module reset */
	tmpreg.val = READREG32(&(g_viss->isp_cfg));
	tmpreg.bits.rst = 1;
	WRITEREG32(&(g_viss->isp_cfg), tmpreg.val);

	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Init ISP_Lite module,
 * The max frequency should be less than 288MHz
 * The min frequency should be more than 99MHz
 */
s32 csp_viss_top_isp_lite_init(u32 clk_freq)
{
	u32 clk_div;
	u32 clk_src = 0;
	u32 lock_sta;

	reg_viss_isp_lite_cfg_t tmpreg;

	PRT_TRACE_BEGIN("clk_freq=%d\n", clk_freq);

	if (clk_freq > 288000000) {
		clk_div = 2;
		clk_src = 1;
		/*clock from ADPLL*/
		csp_viss_top_pllmux_clock_source(1);
		csp_viss_top_setup_adpll(576);
		csp_viss_top_adpll_enable();
	} else if (clk_freq > 198000000) {
		clk_div = 2;
		clk_src = 1;
		/*clock from ADPLL*/
		csp_viss_top_pllmux_clock_source(1);
		csp_viss_top_setup_adpll(clk_freq * 2);
		csp_viss_top_adpll_enable();
	} else if (clk_freq > 148500000)
		clk_div = 3;
	else if (clk_freq > 118800000)
		clk_div = 4;
	else if (clk_freq > 99000000)
		clk_div = 5;
	else
		clk_div = 6;
	/*setup module clock divider*/
	tmpreg.val = 0;
	tmpreg.bits.clk_dir = clk_div - 1;
	WRITEREG32(&(g_viss->isp_lite_cfg), tmpreg.val);
	/*setup module clock source*/
	tmpreg.val = READREG32(&(g_viss->isp_lite_cfg));
	tmpreg.bits.clk_gat = clk_src;
	WRITEREG32(&(g_viss->isp_lite_cfg), tmpreg.val);
	/*setup module clock gating*/
	tmpreg.val = READREG32(&(g_viss->isp_lite_cfg));
	tmpreg.bits.clk_gat = 1;
	WRITEREG32(&(g_viss->isp_lite_cfg), tmpreg.val);
	/*setup AHB clock gating*/
	tmpreg.val = READREG32(&(g_viss->isp_lite_cfg));
	tmpreg.bits.hclk_gat = 1;
	WRITEREG32(&(g_viss->isp_lite_cfg), tmpreg.val);
	/*Release module reset */
	tmpreg.val = READREG32(&(g_viss->isp_lite_cfg));
	tmpreg.bits.rst = 1;
	WRITEREG32(&(g_viss->isp_lite_cfg), tmpreg.val);

	if (clk_src) {
		lock_sta = csp_viss_top_adpll_lock();
		/******setup timeout******/
	}
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Init VIC MCLK
 */
s32 csp_viss_top_vic_mclk_init(u32 clk_freq)
{
	/* u32 clk_div; */
	/* u32 clk_src; */
	u32 i;

	reg_viss_vic_cfg_t tmpreg;
	PRT_TRACE_BEGIN("clk_freq=%d\n", clk_freq);
	/* MCLK default is 24M*/
	for (i = 0; i < sizeof(g_mclk_freq) / sizeof(g_mclk_freq[0]); i++) {
		if (clk_freq == g_mclk_freq[i].freq)
			break;
	}

	if (i >= sizeof(g_mclk_freq) / sizeof(g_mclk_freq[0])) {
		PRT_TRACE_END("Not supported mclk frequency (%d) !\n",
			clk_freq);
		return -1;
	}
#if 0
	/* MCLK default is 24M*/
	if (clk_freq == 24000000) {
		clk_div = 1;
		clk_src = 0;
	} else if (clk_freq == 12000000) {
		clk_div = 2;
		clk_src = 0;
	} else if (clk_freq == 8000000) {
		clk_div = 3;
		clk_src = 0;
	} else if (clk_freq == 6000000) {
		clk_div = 4;
		clk_src = 0;
	} else if (clk_freq == 37125000) {
		/* 37.125M may be used to SONY sensors*/
		clk_div = 16;
		clk_src = 1;
	} else {
		clk_div = 1;
		clk_src = 0;
	}
#endif
	/*setup MCLK clock divider*/
	tmpreg.val = READREG32(&(g_viss->vic_cfg));
	tmpreg.bits.mclk_dir = g_mclk_freq[i].div - 1;
	WRITEREG32(&(g_viss->vic_cfg), tmpreg.val);
	/*setup MCLK clock source*/
	tmpreg.val = READREG32(&(g_viss->vic_cfg));
	tmpreg.bits.mclk_src = g_mclk_freq[i].src;
	WRITEREG32(&(g_viss->vic_cfg), tmpreg.val);
	/*setup MCLK clock gating*/
	tmpreg.val = READREG32(&(g_viss->vic_cfg));
	tmpreg.bits.mclk_gat = 1;
	WRITEREG32(&(g_viss->vic_cfg), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

s32 csp_viss_top_vic_mclk_gate(u32 enable)
{
	reg_viss_vic_cfg_t tmpreg;

	PRT_TRACE_BEGIN("enable=%d\n", enable);
	tmpreg.val = READREG32(&(g_viss->vic_cfg));
	tmpreg.bits.mclk_gat = enable;
	WRITEREG32(&(g_viss->vic_cfg), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Init MCSI MCLK
 */
s32 csp_viss_top_mcsi_mclk_init(u32 clk_freq)
{
	/* u32 clk_div; */
	/* u32 clk_src; */
	u32 i;
	reg_viss_mcsi_cfg_t tmpreg;

	PRT_TRACE_BEGIN("clk_freq=%d\n", clk_freq);
#if 0
	/* MCLK default is 24M */
	if (clk_freq == 24000000) {
		clk_div = 1;
		clk_src = 0;
	} else if (clk_freq == 12000000) {
		clk_div = 2;
		clk_src = 0;
	} else if (clk_freq == 8000000) {
		clk_div = 3;
		clk_src = 0;
	} else if (clk_freq == 6000000) {
		clk_div = 4;
		clk_src = 0;
	} else if (clk_freq == 37125000) {
		/* 37.125M may be used to SONY sensors*/
		clk_div = 16;
		clk_src = 1;
	} else {
		clk_div = 1;
		clk_src = 0;
	}
#endif
	/* MCLK default is 24M*/
	for (i = 0; i < sizeof(g_mclk_freq) / sizeof(g_mclk_freq[0]); i++) {
		if (clk_freq == g_mclk_freq[i].freq)
			break;
	}

	if (i >= sizeof(g_mclk_freq) / sizeof(g_mclk_freq[0])) {
		PRT_TRACE_END("Not supported mclk frequency (%d) !\n",
			clk_freq);
		return -1;
	}
	/*setup MCLK clock divider*/
	tmpreg.val = READREG32(&(g_viss->mcsi_cfg));
	tmpreg.bits.mclk_dir = g_mclk_freq[i].div - 1;
	WRITEREG32(&(g_viss->mcsi_cfg), tmpreg.val);

	/*setup MCLK clock source*/
	tmpreg.val = READREG32(&(g_viss->mcsi_cfg));
	tmpreg.bits.mclk_src = g_mclk_freq[i].src;
	WRITEREG32(&(g_viss->mcsi_cfg), tmpreg.val);

	/*setup MCLK clock gating*/
	tmpreg.val = READREG32(&(g_viss->mcsi_cfg));
	tmpreg.bits.mclk_gat = 1;
	WRITEREG32(&(g_viss->mcsi_cfg), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

s32 csp_viss_top_mcsi_mclk_gate(u32 enable)
{
	reg_viss_vic_cfg_t tmpreg;

	PRT_TRACE_BEGIN("enable=%d\n", enable);
	tmpreg.val = READREG32(&(g_viss->mcsi_cfg));
	tmpreg.bits.mclk_gat = enable;
	WRITEREG32(&(g_viss->mcsi_cfg), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Select ISP pixel channel data source
 */
s32 csp_viss_top_isp_data_source(u32 src)
{
	reg_viss_isp_pix_sel_t tmpreg;

	PRT_TRACE_BEGIN("src=%d\n", src);
	tmpreg.val = READREG32(&(g_viss->isp_pix_sel));
	tmpreg.bits.isp_pix_sel = src;
	WRITEREG32(&(g_viss->isp_pix_sel), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Select ISP_LITE pixel channel data source
 */
s32 csp_viss_top_isp_lite_data_source(u32 src)
{
	reg_viss_isp_pix_sel_t tmpreg;

	PRT_TRACE_BEGIN("src=%d\n", src);
	tmpreg.val = READREG32(&(g_viss->isp_pix_sel));
	tmpreg.bits.ispl_pix_sel = src;
	WRITEREG32(&(g_viss->isp_pix_sel), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

