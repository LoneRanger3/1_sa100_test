/*
 * clk_pll.c - operations for pll clock
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
#define DBG_SECTION_NAME	"CLK_PLL"
#include <debug.h>
#include <rtthread.h>
#include "csp.h"
#include "clk.h"
#include "clk_csp.h"
#include "clk_private.h"
#include "board.h"
#include "clk_pll.h"

#define PRE_DIV_MAX	1
#define POST_DIV_MAX	4
#define FAC_N_MIN	10

struct clock_pll clk_pll[MAX_PLL_CNT];

struct pll_fac_dval_table cpu_pll_fac[] = {
	{312000000,  24000000, 13, 1, 1},
	{408000000,  24000000, 17, 1, 1},
	{504000000,  24000000, 21, 1, 1},
	{600000000,  24000000, 25, 1, 1},
	{720000000,  24000000, 30, 1, 1},
	{816000000,  24000000, 34, 1, 1},
	{912000000,  24000000, 38, 1, 1},
	{1008000000, 24000000, 42, 1, 1}
};

struct clock_pll *to_clock_pll(struct clk_hw *hw)
{
	int i;
	if (hw == NULL)
		return NULL;
	for (i = 0; i < MAX_PLL_CNT; i++)
		if (clk_pll[i].hw == hw)
			return &clk_pll[i];
	return NULL;
}

int clock_pll_prepare(struct clk_hw *hw)
{
	struct clock_pll *pll = to_clock_pll(hw);
	u32 val;
	/* enable enm */
	val = readl(pll->reg[PLL_EN]);
	val |= BIT(pll->en_shift[ENM]);
	writel(val, pll->reg[PLL_EN]);
	/* delay 100us */
	udelay(100);

	/* enable enp */
	val |= BIT(pll->en_shift[ENP]);
	writel(val, pll->reg[PLL_EN]);
	/* delay 10us */
	udelay(10);

	/* wait lock status set, usually <= 100us */
	val = 5; /* timout 250~500us */
	while (!(readl(pll->reg[STATUS]) & BIT(pll->en_shift[LOCK]))
			&& val--) {
		LOG_D("check %s status", __clk_get_name(hw));
		udelay(50);
	}

	if (!val) {
		LOG_E("clock %s prepare failed\n", __clk_get_name(hw));
		return -EINVAL;
	}
	return 0;
}

void clock_pll_unprepare(struct clk_hw *hw)
{
	struct clock_pll *pll = to_clock_pll(hw);
	u32 val;

	/* disable enp */
	val = readl(pll->reg[PLL_EN]);
	val &= ~BIT(pll->en_shift[ENP]);
	writel(val, pll->reg[PLL_EN]);

	/* disable enm */
	val &= ~BIT(pll->en_shift[ENM]);
	writel(val, pll->reg[PLL_EN]);

	/* audio pll: disable DIV_ENDIV7 and DIV_ENDIV17 bit */
	if (is_audio_pll_clk(__clk_get_name(hw))) {
		val &= ~BIT(AUDIO_PLL_DIV7_BIT_IDX);
		val &= ~BIT(AUDIO_PLL_DIV17_BIT_IDX);
	}
	writel(val, pll->reg[PLL_EN]);
}

int clock_pll_enable(struct clk_hw *hw)
{
	struct clock_pll *pll = to_clock_pll(hw);
	u32 val;

	clock_pll_prepare(hw);

	/* enable oen */
	val = readl(pll->reg[PLL_EN]);
	val |= BIT(pll->en_shift[OEN]);
	writel(val, pll->reg[PLL_EN]);

	/* audio pll: enable DIV_ENDIV7 and DIV_ENDIV17 bit */
	if (is_audio_pll_clk(__clk_get_name(hw))) {
		val |= BIT(AUDIO_PLL_DIV7_BIT_IDX);
		val |= BIT(AUDIO_PLL_DIV17_BIT_IDX);
	}
	writel(val, pll->reg[PLL_EN]);

	return 0;
}

void clock_pll_disable(struct clk_hw *hw)
{
	struct clock_pll *pll = to_clock_pll(hw);
	u32 val;

	/* disable oen */
	val = readl(pll->reg[PLL_EN]);
	val &= ~BIT(pll->en_shift[OEN]);
	writel(val, pll->reg[PLL_EN]);

	clock_pll_prepare(hw);
}

u32 clock_pll_recalc_rate(struct clk_hw *hw, u32 parent_rate)
{
	struct clock_pll *pll = to_clock_pll(hw);
	u32 val, n, prev_div, post_div;
	u32 ret = 0;

/*
	if (is_audio_pll_clk(name)) {
		LOG_D("clk(%s) is audio clk\n", name);
		return audio_clk_recalc_rate(name);
	}
*/
	val = readl(pll->reg[FACTOR]);
	n = FAC_N(val);
	prev_div = PREV_DIV(val);
	post_div = POST_DIV(val);
	if (!n)
		LOG_E("%s %d error\n", __func__, __LINE__);

	ret = parent_rate / prev_div;
	ret = ret * n;
	if (hw->id != CLK_ID_PERH0_PLL_VCO && hw->id != CLK_ID_AUDIO_PLL_DIV0)
		ret = ret / post_div;
	return ret;
}

struct pll_fac_dval_table *__get_rate_from_fd_table(struct clock_pll *pll,
			u32 rate, u32 parent_rate)
{
	int i;
	struct pll_fac_dval_table *tmp = NULL;
	u32 best_rate = 0;

	for (i = 0; i < pll->fd_cnt; i++) {
		if (pll->fd_table[i].parent_rate != parent_rate
			|| pll->fd_table[i].rate > rate)
			continue;
		if (pll->fd_table[i].rate == rate) {
			tmp = &pll->fd_table[i];
			break;
		}
		if (best_rate < pll->fd_table[i].rate) {
			best_rate = pll->fd_table[i].rate;
			tmp = &pll->fd_table[i];
		}
	}
	return tmp;
}

/* get the max factor value, according to pll's recommended freq-div table */
int __get_fac_max(struct clk_hw *hw, int pre_div)
{
	int ret = 50;
	if (!(pre_div > 0 && pre_div <= PRE_DIV_MAX))
		LOG_E("err! clk:%s check pre_div(%d) faile!", pre_div);

	if ((hw->id == CLK_ID_SDRAM_PLL)
			|| (hw->id == CLK_ID_PERH0_PLL_VCO)) {
		ret = 73;
		if (pre_div == 2)
			ret = 146;
	} else if (hw->id == CLK_ID_PERH2_PLL) {
		ret = 66;
		if (pre_div == 2)
			ret = 132;
	} else {
		if (pre_div == 2)
			ret = 100;
	}
	return ret;
}
int _pll_round_up(struct clk_hw *hw, u32 rate, u32 *prate,
	struct pll_fac_dval_table *pfdt)
{
	struct clock_pll *pll = to_clock_pll(hw);
	int pre_max, post_max, fac_max;
	int i, j;
	int fac_tmp;
	int tmp_rate;
	int best_rate = -1;
	struct pll_fac_dval_table *tmp_table;

	if (pll->fd_cnt != 0) {
		tmp_table = __get_rate_from_fd_table(pll, rate, *prate);
		if (pfdt && tmp_table)
			memcpy(pfdt, tmp_table,
				sizeof(struct pll_fac_dval_table));
		if (tmp_table)
			return (long)tmp_table->rate;
	}

	/* get the max prev_val, post_val and factor */
	pre_max = BIT(pll->fac_width[PREV]);
	pre_max = min(pre_max, PRE_DIV_MAX);
	post_max = BIT(pll->fac_width[POST]);
	post_max = min(post_max, POST_DIV_MAX);
	if ((hw->id == CLK_ID_AUDIO_PLL_DIV0)
			|| (hw->id == CLK_ID_PERH0_PLL_VCO)) {
			post_max = 1;
	}
	for (i = 1; i <= pre_max; i++) {
		for (j = 1; j <= post_max; j++) {
			fac_max = __get_fac_max(hw, i);
			fac_tmp = (rate)/((*prate)/i/j);
			tmp_rate = ((*prate)/i/j)*fac_tmp;
			if (fac_tmp < FAC_N_MIN || fac_tmp > fac_max
				|| tmp_rate < best_rate) {
				continue;
			}
			best_rate = tmp_rate;
			if (pfdt) {
				pfdt->parent_rate = *prate;
				pfdt->rate = best_rate;
				pfdt->prev_val = i - 1;
				pfdt->post_val = j - 1;
				pfdt->factor = fac_tmp;
			}
			if (best_rate == rate)
				break;
		}
	}
	return best_rate;
}


long clock_pll_round_rate(struct clk_hw *hw, u32 rate, u32 *prate)
{
	const char *name = __clk_get_name(hw);
	long best_rate;

	/* pll rate should be always >0 */
	if (!rate) {
		LOG_I("pll clk %s rate to set is 0", name);
		return -EINVAL;
	}

	if (is_audio_pll_clk(name)) {
		LOG_E("audio clk(%s) cannot call clk_set_rate\n", name);
		return -EINVAL; /* abort clk_set_rate */
	}

	/* get the matched item in freq-div table */
	best_rate = _pll_round_up(hw, rate, prate, NULL);

	return best_rate;
}

int clock_pll_set_rate(struct clk_hw *hw, u32 rate, u32 parent_rate)
{
	struct clock_pll *pll = to_clock_pll(hw);
	const char *name = __clk_get_name(hw);
	struct pll_fac_dval_table fdt;
	u32 reg_val;
	u32 prate = parent_rate;

	if (is_audio_pll_clk(name)) {
		LOG_E("audio clk(%s) should not run here\n", name);
		/*
		 * clk_set_rate will be aborted by clock_pll_round_rate,
		 * so it should not be running here. BTW, we return ok
		 */
		return 0;
	}

	rate = _pll_round_up(hw, rate, &prate, &fdt);
	if (rate <= 0) {
		LOG_E("pll_round_rate failed, clock(%s)\n", name);
		return -EINVAL;
	}

	reg_val = readl(pll->reg[FACTOR]);
	reg_val = SET_FAC_N(reg_val, fdt.factor);
	reg_val = SET_PREV_DIV(reg_val, fdt.prev_val);
	if ((hw->id != CLK_ID_AUDIO_PLL_DIV0)
			&& (hw->id != CLK_ID_PERH0_PLL_VCO)) {
		reg_val = SET_POST_DIV(reg_val, fdt.post_val);
	}
	writel(reg_val, pll->reg[FACTOR]);

	return 0;
}

const struct clk_ops clk_pll_ops = {
	.enable = clock_pll_enable,
	.disable = clock_pll_disable,
	.recalc_rate = clock_pll_recalc_rate,
	.round_rate = clock_pll_round_rate,
	.set_rate = clock_pll_set_rate,
};

int __init_pll_fd_table(struct clock_pll *pll)
{
	int i;
	int size;

	pll->fd_cnt = 0;
	if (pll->hw->id == CLK_ID_CPU_PLL) {
		pll->fd_cnt = ARRAY_SIZE(cpu_pll_fac);
		if (pll->fd_cnt > MAX_FD_CNT_PLL)
			pll->fd_cnt = MAX_FD_CNT_PLL;
		size = pll->fd_cnt*sizeof(struct pll_fac_dval_table);
		memcpy(pll->fd_table, cpu_pll_fac, size);
	}

	for (i = 0; i < pll->fd_cnt; i++) {
		pll->fd_table[i].prev_val -= 1;
		pll->fd_table[i].post_val -= 1;
		if (pll->fd_table[i].prev_val < 0
			|| pll->fd_table[i].post_val < 0) {
			pll->fd_cnt = i;
			break;
		}
	}
	return 0;
}

/**
 * setup_pll_regs - set all pll reg's vaddr
 * @pll_clk: the pll clock's struct
 *
 */
void setup_pll_regs(struct clock_pll *pclk)
{
	int i;
	struct clk_hw *hw = pclk->hw;
	void *reg_start = (void *)hw->reg;

	if (!strcmp("cpu_pll", pclk->hw->name) ||
		!strcmp("ax_pll", pclk->hw->name)) {
		pclk->reg[PLL_EN] = reg_start;
		pclk->reg[FACTOR] = reg_start + 0x8;
		pclk->reg[TUNE0] = reg_start + 0xc;
		pclk->reg[TEST] = reg_start + 0x14;
		pclk->reg[STATUS] = reg_start + 0x18;
		pclk->reg[MODE] = reg_start + 0x1c;
		pclk->reg[NFAC] = reg_start + 0x20;
		pclk->reg[TUNE2] = reg_start + 0x2c;
		return;
	}

	for (i = 0; i < MAX_PLL_REGS; i++) /* omit ENABLE reg */
		pclk->reg[i] = reg_start + (i << 2);
}

void clk_init_pll(clk_t *clk)
{
	int i;
	struct clock_pll *pll = NULL;

	clk->num_parents = 1;
	if (is_audio_pll_clk(__clk_get_name(clk->hw)))
		clk->hw->flags |= CLK_GET_RATE_NOCACHE;
	clk->ops = &clk_pll_ops;

	for (i = 0; i < MAX_PLL_CNT; i++) {
		if (clk_pll[i].hw == NULL) {
			pll = &clk_pll[i];
			pll->hw = clk->hw;
			break;
		}
	}
	if (pll == NULL) {
		LOG_E("%s %d err.\n", __func__, __LINE__);
		return;
	}

	setup_pll_regs(pll);
	pll->en_shift[ENP] = 0;
	pll->en_shift[ENM] = 1;
	pll->en_shift[OEN] = 2;
	pll->en_shift[LOCK] = 0;

	pll->fac_shift[PREV] = 0;
	pll->fac_shift[POST] = 4;
	pll->fac_shift[FAC] = 8;

	pll->fac_width[PREV] = 2;
	pll->fac_width[POST] = 2;
	pll->fac_width[FAC] = 8;

	if (readl(pll->reg[PLL_EN]) & BIT(pll->en_shift[OEN]))
		clk->enable_count = 1;
	__init_pll_fd_table(pll);
}

