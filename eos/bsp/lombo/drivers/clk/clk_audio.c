/*
 * clk_audio.c - operations for audio clock
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
#define DBG_SECTION_NAME	"CLK_AU"

#include <clk_debug.h>
#include <rtthread.h>
#include "csp.h"
#include "clk.h"

#include "clk_csp.h"
#include "clk_private.h"

/* use i2s0 instead of i2s in n7v1 */
#ifdef ARCH_LOMBO_N7V1
#define reg_prcm_i2s_clk_ctrl_t reg_prcm_i2s0_clk_ctrl_t
#define CLK_ID_I2S_CLK CLK_ID_I2S0_CLK
#endif

/*
 * audio type clocks: audio pll relevant clocks, include audio_pll_div0,
 *      audio_pll_div7/17, audio_pll_divm, i2s_clk...
 *
 * for audio type clocks:
 *  1. only "module-clock" can actually update the clock rate
 *  2. the only way to get rate is query the freq-div-table
 *  3. everytime get clock rate(clk_get_rate), should query the hardware,
 *       then query the freq-div-table
 *
 * so these are realized:
 *   (1) clk_ops.set_rate will be bypassed in no "module-clock", such as
 *          audio_pll_div0, audio_pll_div7/17, audio_pll_divm...
 *   (2) clk_ops.set_rate is only valid in "module-clock", such as i2s_clk
 *   (3) clk_ops.round_rate will be bypassed
 *   (4) clk_ops.recalc_rate will everytime query the hardware and then query
 *          the freq-div-table
 *   (5) should set CLK_GET_RATE_NOCACHE when register
 */

int is_audio_pll_clk(const char *name)
{
	return !strcmp(CLK_NAME_AUDIO_PLL_DIV0, name);
}

int is_audio_fix_divider_clk(const char *name)
{
	return !strcmp(CLK_NAME_AUDIO_PLL_DIV7, name) ||
		!strcmp(CLK_NAME_AUDIO_PLL_DIV17, name);
}

int is_audio_divider_clk(const char *name)
{
	return !strcmp(CLK_NAME_AUDIO_PLL_DIVM, name);
}

int is_audio_module_clk(const char *name)
{
	return !strcmp(CLK_NAME_I2S_CLK, name) || !strcmp(CLK_NAME_I2S0_CLK, name);
}

enum {
	RATE_MOD = 0,	/* mod clock(i2s_clk) clock rate index */
	RATE_DIV0,	/* audio_pll_div0 clock rate index */
	RATE_DIV7,	/* audio_pll_div7 clock rate index */
	RATE_DIV17,	/* audio_pll_div17 clock rate index */
	RATE_DIVM,	/* audio_pll_divm clock rate index */
	RATE_CNT	/* clock rate count in table */
};

struct audio_fd_table {
	int prev_div;		/* pll PREV_DIV bit field value */
	int post_div;		/* pll POST_DIV bit field value */
	int fac_n;		/* pll N(factor) bit field value */
	int mod_div;		/* mod DIV bit field value */
	int mod_clksrc;		/* mod SRC_SEL(clksrc sel) bit field value */
	u32 rate[RATE_CNT];	/* clock rate table */
};

/*
 * audio clock div-freq-table
 */
enum {
	I2S_CLKSRC_NULL,
	I2S_CLKSRC_DIVM,
	I2S_CLKSRC_DIV7,
	I2S_CLKSRC_DIV17,
};

const struct audio_fd_table fd_table[] = {
	/*
	 * the table item data arranged as below:
	 * {pll_prev_div, pll_post_div, pll_fac_n, mod_div, mod_clksrc,
	 *	mod_rate, rate_div0, rate_div7, rate_div17, rate_divm},
	 */
	{1, 2, 43, 2, I2S_CLKSRC_DIV7,
		{CLK_24571M, 516000000, CLK_24571M_X3, -1, 172000000} },
	{0, 2, 43, 2, I2S_CLKSRC_DIV7,
		{CLK_24571M_X2, 1032000000, CLK_24571M_X6, -1, 344000000} },
	{1, 0, 43, 0, I2S_CLKSRC_DIV7,
		{CLK_24571M_X3, 516000000, CLK_24571M_X3, -1, 516000000} },
	{1, 0, 32, 0, I2S_CLKSRC_DIV17,
		{CLK_225882M, 384000000, -1, CLK_225882M, 384000000} },
	{0, 0, 32, 0, I2S_CLKSRC_DIV17,
		{CLK_225882M_X2, 768000000, -1, CLK_225882M_X2, 768000000} },
	{1, 2, 96, 0, I2S_CLKSRC_DIV17,
		{CLK_225882M_X3, 1152000000, -1, CLK_225882M_X3, 384000000} }
};

#define AUDIO_PLL_FACTOR_REG	(0x0400a524 + VA_PRCM)
#define AUDIO_I2S_CLKCTRL_REG	(0x0400abe0 + VA_PRCM)

/**
 * clk_name_to_rate_index - get clock's rate index in audio_fd_table.rate[]
 * @name: clock name
 *
 * return the index if success, -EINVAL if failed
 */
int clk_name_to_rate_index(const char *name)
{
	if (!strcmp(CLK_NAME_I2S_CLK, name) ||
		!strcmp(CLK_NAME_I2S0_CLK, name))
		return RATE_MOD;
	else if (!strcmp(CLK_NAME_AUDIO_PLL_DIV7, name))
		return RATE_DIV7;
	else if (!strcmp(CLK_NAME_AUDIO_PLL_DIV17, name))
		return RATE_DIV17;
	else if (!strcmp(CLK_NAME_AUDIO_PLL_DIVM, name))
		return RATE_DIVM;
	else if (!strcmp(CLK_NAME_AUDIO_PLL_DIV0, name))
		return RATE_DIV0;
	else
		LOG_W("clock name %s is not audio clock\n", name);

	return -EINVAL;
}

/**
 * fd_table_match_for_recalc - find the item in fd_table, whose div
 *     and factor matched with para. for recalc rate
 * @apf: audio pll factor reg value
 * @icc: i2s clock ctrl reg value
 *
 * return the matched item if success, NULL if failed
 */
#define ARRAY_SIZE(x)	(sizeof(x) / sizeof((x)[0]))
struct audio_fd_table *
fd_table_match_for_recalc(reg_prcm_audio_pll_fac_t *apf,
			reg_prcm_i2s_clk_ctrl_t *icc)
{
	int i;
	LOG_D("%d %d %d %d %d\n", apf->bits.pre_div,
	apf->bits.post_div, apf->bits.n, icc->bits.div, icc->bits.src_sel);
	for (i = 0; i < ARRAY_SIZE(fd_table); i++) {
		LOG_D("%d %d %d %d %d\n", fd_table[i].prev_div,
			fd_table[i].post_div, fd_table[i].fac_n,
			fd_table[i].mod_div, fd_table[i].mod_clksrc);
		if (apf->bits.pre_div == fd_table[i].prev_div
			&& apf->bits.post_div == fd_table[i].post_div
			&& apf->bits.n == fd_table[i].fac_n
			&& icc->bits.div == fd_table[i].mod_div
			&& icc->bits.src_sel == fd_table[i].mod_clksrc)
			return (struct audio_fd_table *)&fd_table[i];
	}

	return NULL;
}

/**
 * audio_clk_recalc_rate - get clock rate from hw
 * @name: clk name
 *
 * query the hw for div and factor, and then query the freq-div-table,
 * get the matched rate
 *
 * return the matched rate if success, 0 if failed
 */
u32 audio_clk_recalc_rate(const char *name)
{
	struct audio_fd_table *pft = NULL;
	reg_prcm_audio_pll_fac_t apf;
	reg_prcm_i2s_clk_ctrl_t icc;
	int index;

	apf.val = readl(AUDIO_PLL_FACTOR_REG);
	icc.val = readl(AUDIO_I2S_CLKCTRL_REG);

	/* get matched item in fd_table */
	pft = fd_table_match_for_recalc(&apf, &icc);
	if (pft) {
		index = clk_name_to_rate_index(name);
		if (index >= 0 && index < RATE_CNT)
			return pft->rate[index];
		else
			LOG_W("invalid index!");
	}

	LOG_E("cannot find matched rate for audio clk %s", name);
	return -1;
}

/**
 * fd_table_match_for_set - find the item in fd_table, whose rate
 *     matched with para. for set rate
 * @rate: clock rate
 * @index: index in audio_fd_table.rate[]
 *
 * return the matched item if success, NULL if failed
 */
struct audio_fd_table *
fd_table_match_for_set(u32 rate, int index)
{
	int i;

	if (index < 0 || index >= RATE_CNT) {
		LOG_W("warning: index = %d", index);
		return NULL;
	}

	for (i = 0; i < ARRAY_SIZE(fd_table); i++) {
		if (fd_table[i].rate[index] == rate)
			return (struct audio_fd_table *)&fd_table[i];
	}

	LOG_W("cannot find matched rate(%d) for audio clk", rate);
	return NULL;
}

/**
 * set_rate_hw - set audio clock's div and factor in reg
 * @ptbl: the fd_table item, which contain the value to be set
 */
void set_rate_hw(struct clk_hw *hw, struct audio_fd_table *ptbl)
{
	reg_prcm_audio_pll_fac_t apf;
	reg_prcm_i2s_clk_ctrl_t icc;
	clk_t *i2s;

	apf.val = readl(AUDIO_PLL_FACTOR_REG);
	icc.val = readl(AUDIO_I2S_CLKCTRL_REG);

	apf.bits.pre_div = ptbl->prev_div;
	apf.bits.post_div = ptbl->post_div;
	apf.bits.n = ptbl->fac_n;
	icc.bits.div = ptbl->mod_div;
	if (icc.bits.src_sel != ptbl->mod_clksrc) {
		/*
		 * for i2s_clk, we should call clk_set_parent to select the
		 * correct clk src before set rate(the rate is corresponding to
		 * the clk src).
		 *
		 * if donot warn here, clk_get_parent(i2s_clk) may return the
		 * old(cached, wrong) parent handle.
		 */
		LOG_W("i2s clk src reg field(%d) wrong", icc.bits.src_sel);
		icc.bits.src_sel = ptbl->mod_clksrc;

		i2s = get_clk_from_id(CLK_ID_I2S_CLK);
		if (i2s) {
			if (ptbl->mod_clksrc < i2s->num_parents)
				i2s->parent = i2s->parents[ptbl->mod_clksrc];
		}
	}

	writel(apf.val, AUDIO_PLL_FACTOR_REG);
	writel(icc.val, AUDIO_I2S_CLKCTRL_REG);
}

/**
 * audio_clk_set_rate - set audio module clock rate
 * @rate: rate to be set
 * @name: clk name
 *
 * query the freq-div-table for the rate, if rate is valid,
 * then set the hw div and factor
 *
 * return 0 if success, <0 if failed
 */
int audio_clk_set_rate(struct clk_hw *hw, u32 rate, const char *name)
{
	struct audio_fd_table *ptbl = NULL;
	int index;

	if (!is_audio_module_clk(name))
		LOG_W("clock is not audio clock!");

	/* get clock rate's index in audio_fd_table.rate[] */
	index = clk_name_to_rate_index(name);
	if (index < 0 || index >= RATE_CNT) {
		LOG_E("clock name %s invalid\n", name);
		return -EINVAL;
	}

	/* get the matched table item */
	ptbl = fd_table_match_for_set(rate, index);
	/* must be in table, because the rate was passed by round_rate */
	if (!ptbl) {
		LOG_W("%s(%d) err, not find match rate", __func__, __LINE__);
		return -EINVAL;
	}

	set_rate_hw(hw, ptbl);
	return 0;
}

/**
 * audio_clk_round_rate - round audio module clock rate
 * @rate: rate to be rounded
 * @name: clk name
 *
 * query the freq-div-table for the rate, if rate is valid,
 * then return the rate. otherwise return <0, which will
 * abort the clk_set_rate
 */
long audio_clk_round_rate(char *name, u32 rate, u32 *prate)
{
	struct audio_fd_table *ptbl = NULL;
	int index;

	if (!is_audio_module_clk(name))
		LOG_W("clk is not audio clock!\n");

	/* get clock rate's index in audio_fd_table.rate[] */
	index = clk_name_to_rate_index(name);
	if (index < 0 || index >= RATE_CNT) {
		LOG_E("clock name %s invalid\n", name);
		return -EINVAL;
	}

	/* get the matched table item */
	ptbl = fd_table_match_for_set(rate, index);
	if (ptbl)
		return rate;

	/* cannot find matched item */
	LOG_W("rate %d invalid for clock %s", (int)rate, name);
	return -EINVAL;
}

void init_clk_i2s()
{
	reg_prcm_audio_pll_fac_t apf;
	reg_prcm_i2s_clk_ctrl_t icc;
	int i;
	apf.val = readl(AUDIO_PLL_FACTOR_REG);
	icc.val = readl(AUDIO_I2S_CLKCTRL_REG);

	for (i = 0; i < ARRAY_SIZE(fd_table); i++) {
		if (apf.bits.pre_div == fd_table[i].prev_div
		&& apf.bits.post_div == fd_table[i].post_div
		&& apf.bits.n == fd_table[i].fac_n) {
			icc.bits.div = fd_table[i].mod_div;
			icc.bits.src_sel = fd_table[i].mod_clksrc;
			writel(icc.val, AUDIO_I2S_CLKCTRL_REG);
			return;
		}
	}
}

