/*
 * clk_module.c - operations for composite clock
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
#define DBG_SECTION_NAME	"CLK_M"
#include <clk_debug.h>
#include <rtthread.h>
#include "csp.h"
#include "clk.h"
#include "clk_csp.h"
#include "clk_private.h"

#define MAX_FD_CNT_MOD		32

struct divider2_rate_table {
	u32	rate;
	u32	parent_rate;
	/*
	 * val0 and val1 are the orignal val read from reg bits
	 * NOT the corresponding division value. eg: val0 is 3
	 * while the corresponding division is 8(2^3)
	 */
	int		val0;
	int		val1;
};
struct divider2_rate_table sdc_rate_table[MAX_FD_CNT_MOD] = {
	{240000, 24000000, 10, 10},
	{400000, 24000000, 6 , 10},
	{600000, 24000000, 5 , 8 },
	{800000, 24000000, 5 , 6 },
	{24000000, 24000000, 1 , 1 },

	{30000000, 1188000000, 5 , 8 },
	{40000000, 1188000000, 5 , 6 },
	{48000000, 1188000000, 5 , 5 },
	{50000000, 1188000000, 6 , 4 },
	{60000000, 1188000000, 5 , 4 },
	{66000000, 1188000000, 3 , 6 },
	{74000000, 1188000000, 4 , 4 },
	{80000000, 1188000000, 15, 1 },
	{92000000, 1188000000, 13, 1 },
	{100000000, 1188000000, 3, 4 },
	{108000000, 1188000000, 11, 1 },
	{120000000, 1188000000, 5, 2 },
	{132000000, 1188000000, 9, 1 },
	{150000000, 1188000000, 8, 1 },
	{170000000, 1188000000, 7, 1 },
	{200000000, 1188000000, 6, 1 },
	{240000000, 1188000000, 5, 1 },
	{300000000, 1188000000, 4, 1 },
	{400000000, 1188000000, 3, 1 }
};

struct divider2_rate_table spi_rate_table[MAX_FD_CNT_MOD] = {
	{4000000, 24000000, 1, 6},
	{6000000, 24000000, 1, 4},
	{8000000, 24000000, 1, 3},
	{12000000, 24000000, 1, 2},
	{24000000, 24000000, 1, 1},
	{25000000, 594000000, 6, 4},
	{30000000, 594000000, 5, 4},
	{37500000, 594000000, 4, 4},
	{40000000, 594000000, 3, 5},
	{50000000, 594000000, 3, 4},
	{60000000, 594000000, 5, 2},
	{75000000, 594000000, 2, 4},
	{100000000, 594000000, 3, 2},
	{120000000, 594000000, 1, 5},
	{150000000, 594000000, 1, 4},
	{200000000, 594000000, 1, 3}
};

#define to_clk_divider2(_hw) (_hw)

u32 clk_divider2_recalc_rate(struct clk_hw *hw,
		u32 parent_rate)
{
	clk_divider2_t *divider = to_clk_divider2(hw);
	u32 div[2], val[2];

	val[0] = readl(divider->reg) >> divider->div_shift0;
	val[0] &= (1 << divider->div_width0) - 1;
	div[0] = __get_div(divider->flags, val[0], divider->table);

	val[1] = readl(divider->reg) >> divider->div_shift1;
	val[1] &= (1 << divider->div_width1) - 1;
	div[1] = __get_div(divider->flags, val[1], divider->table);

	if (!(div[0] && div[0])) {
		if (!(divider->flags & CLK_DIVIDER_ALLOW_ZERO))
			LOG_E("%s: divisor zero err!\n",
				__clk_get_name(hw));
		return parent_rate;
	}

	parent_rate = DIV_ROUND_UP(parent_rate, div[0]);
	return DIV_ROUND_UP(parent_rate, div[1]);
}

struct divider2_rate_table *divder_get_rate_table(clk_divider2_t *dvd)
{
	if (dvd->id == CLK_ID_SDC0_CLK || dvd->id == CLK_ID_SDC1_CLK)
		return sdc_rate_table;
	if (dvd->id == CLK_ID_SPI0_CLK || dvd->id == CLK_ID_SPI1_CLK
		|| dvd->id == CLK_ID_SPI2_CLK)
		return spi_rate_table;
	return NULL;
}

int __get_dval_from_fd_table(struct divider2_rate_table *fd_table, u32 rate,
				u32 parent_rate, int *val)
{
	int i;
	u32 best_rate = 0;

	for (i = 0; (i < MAX_FD_CNT_MOD) && (fd_table[i].rate != 0); i++) {
		if (fd_table[i].parent_rate != parent_rate
			|| fd_table[i].rate > rate)
			continue;
		if (best_rate < fd_table[i].rate) {
			best_rate = fd_table[i].rate;
			val[0] = fd_table[i].val0 - 1;
			val[1] = fd_table[i].val1 - 1;
		}
		if (best_rate == rate)
			break;
	}
	return best_rate ? best_rate : -1;
}

/**
 * __calc_val - get the val(orignal reg bits value) corresponding to the div
 * @flags:	the clock divider flags
 * @div:	the div value
 * @div_width:	reg bits width of divider
 * @table:	the div-val table
 * @exact_match: true indicate the return val must exactly match div.
 *		    so if cannot find the match val, return -EEROR
 *		 if false, just return the minimal val which meet
 *			the condition divider(val) >= div
 *
 * return >=0 when success, otherwise -EEROR
 */
int __calc_val(u8 flags, int div, u32 div_width,
			const struct clk_div_table *table,
			int exact_match)
{
	int val = -EINVAL;

	if (!div) {
		LOG_E("div is 0, to check!\n");
		return 0; /* to make it work temporarily */
	}

	if (flags & CLK_DIVIDER_ONE_BASED)
		val = div;
	else if (flags & CLK_DIVIDER_POWER_OF_TWO) {
		if (exact_match && !is_power_of_2(div))
			return -EINVAL; /* exact value not exist */
		val = __rt_fls(div - 1);
	} else if (table) {
		val = INT_MAX;
		while (table->div) {
			if (table->div == div) {
				val = table->val;
				goto end;
			}
			if (table->div > div && table->val < val)
				val = table->val;
			table++;
		}
		if (exact_match) /* cannot find the exact value */
			return -EINVAL;
	} else
		val = div - 1; /* default case */

end:
	return min(val, ((1 << div_width) - 1));
}

/* get the best div0 and div1, and return the corresponding rate */
long clk_divider2_get_best_rate(struct clk_hw *hw, u32 rate,
				u32 *prate, int *dval)
{
	clk_divider2_t *dvd;
	char *name;
	u32 tmp, best_rate = 0;
	int max_val[2] = {0, 0}; /* correspond to best_rate */
	int mval[2] = {0, 0}; /* correspond to best_rate */
	int i, j, div[2];
	struct divider2_rate_table *fd_table;

	if (hw == NULL)
		return -EINVAL;
	name = (char *)__clk_get_name(hw);
	dvd = to_clk_divider2(hw);
	if (!prate && !rate) {
		/*
		 * the case that clk src is null_clk, or clk src's factor
		 * is 0(fpga debug?), is normal case. so return success
		 * here. this also void the below finding fd table operation
		 * which will be failed
		 */
		return 0;
	} else if (!*prate || !rate) {
		/*
		 * these cases will never occur:
		 * 1. !parent_rate && rate: round_rate will set rate to 0
		 * 2. parent_rate && !rate: round_rate will return -EINVAL,
		 *    which will abort clk_set_rate(will not run to here)
		 */
		LOG_E("clk %s parent_rate(%d) or rate(%d) is 0",
			__clk_get_name(hw), (int)*prate, (int)rate);
		return -EINVAL;
	}

	/* get the matched item in freq-div table */
	fd_table = divder_get_rate_table(dvd);
	if (fd_table) {
		best_rate = __get_dval_from_fd_table(
			fd_table, rate, *prate, mval);
		if (dval) {
			dval[0] = mval[0];
			dval[1] = mval[1];
		}
		return best_rate;
	}

	/* if cannot find matched item in table, calculate manually */
	if ((hw->flags & CLK_SET_RATE_PARENT)) {
		LOG_E("CLK_SET_RATE_PARENT set for clock(%s)\n", name);
		return rate;
	}

	/* calc the max val0 and val1 */
	tmp = DIV_ROUND_UP(*prate, rate);
	max_val[0] = __calc_val(dvd->flags, tmp, dvd->div_width0,
			dvd->table, false);
	max_val[1] = __calc_val(dvd->flags, tmp, dvd->div_width1,
			dvd->table, false);

	/* try all [val0, val1] case, to catch the best rate */
	for (i = 0; i <= max_val[0]; i++) {
		div[0] = __get_div(dvd->flags, i, dvd->table);
		if (!div[0]) /* i is invalid */
			continue;

		for (j = 0; j <= max_val[1]; j++) {
			div[1] = __get_div(dvd->flags, j, dvd->table);
			if (!div[1])
				continue;

			/* calc the rate for cur [val0, val1] */
			tmp = *prate;
			tmp = DIV_ROUND_UP(tmp, div[0]);
			tmp = DIV_ROUND_UP(tmp, div[1]);
			/*
			 * donot consider CLK_DIVIDER_ROUND_CLOSEST
			 * flag right now
			 */
			if (tmp > rate)
				continue;
			if (tmp == rate) {
				best_rate = tmp;
				mval[0] = i;
				mval[1] = j;
				goto exit;
			}
			if (tmp > best_rate) {
				best_rate = tmp;
				mval[0] = i;
				mval[1] = j;
			}
		}
	}

exit:
	/*
	 * rate to set is too small(less than *prate/max_divs), so we let
	 * round_rate failed, which will abort clk_set_rate
	 */
	if (!best_rate) {
		LOG_I("clk %s rate to set(%d) too small, *prate %d\n",
			name, (int)rate, (int)*prate);
		return -EINVAL;
	}
	if (dval) {
		dval[0] = mval[0];
		dval[1] = mval[1];
		LOG_D("clk %s prate:%d, rate:%d, val[0]:%d, val[1]:%d\n",
			name, (int)*prate, (int)rate, dval[0], dval[1]);
	}
	return best_rate;
}

long clk_divider2_round_rate(struct clk_hw *hw, u32 rate,
				u32 *prate)
{
	return clk_divider2_get_best_rate(hw, rate, prate, NULL);
}

int clk_divider2_set_rate(struct clk_hw *hw, u32 rate,
				u32 parent_rate)
{
	clk_divider2_t *divider = to_clk_divider2(hw);
	int reg_val, val[2];
	long new_rate;
	u32 best_parent = parent_rate;

	new_rate = clk_divider2_get_best_rate(hw, rate, &best_parent, val);
	if (new_rate <= 0) {
		LOG_E("clk_divider2_set_rate %d failed clock(%s),val:%d %d\n",
			(int)rate, __clk_get_name(hw), val[0], val[1]);
		return -EINVAL;
	}

	reg_val = readl(divider->reg);
	reg_val &= (~(((1 << divider->div_width0) - 1) <<
			divider->div_shift0));
	reg_val |= (val[0] << divider->div_shift0);
	reg_val &= (~(((1 << divider->div_width1) - 1) <<
			divider->div_shift1));
	reg_val |= (val[1] << divider->div_shift1);

	writel(reg_val, divider->reg);
	return 0;
}

const struct clk_ops clk_divider2_ops = {
	.recalc_rate = clk_divider2_recalc_rate,
	.round_rate = clk_divider2_round_rate,
	.set_rate = clk_divider2_set_rate,
};

static u8 clk_composite_get_parent(struct clk_hw *hw)
{
	int num = __clk_get_num_parents(hw);

	if (num > 1)
		return clk_mux_ops.get_parent(hw);
	return 0;
}

static int clk_composite_set_parent(struct clk_hw *hw, u8 index)
{
	int num = __clk_get_num_parents(hw);

	if (num > 1)
		return clk_mux_ops.set_parent(hw, index);
	return -EINVAL;
}

static u32 clk_composite_recalc_rate(struct clk_hw *hw, u32 parent_rate)
{
	u32 (*recalc_rate)(struct clk_hw *, u32);

	recalc_rate = NULL;
	if ((hw->div_width0 != 0) && (hw->div_width1 == 0))
		recalc_rate = clk_divider_ops.recalc_rate;
	if ((hw->div_width0 != 0) && (hw->div_width1 != 0))
		recalc_rate = clk_divider2_ops.recalc_rate;

	if (recalc_rate)
		return recalc_rate(hw, parent_rate);

	return parent_rate;
}

static long clk_composite_round_rate(struct clk_hw *hw,
			u32 rate, u32 *prate)
{
	long (*round_rate)(struct clk_hw *, u32, u32 *);
	round_rate = NULL;

	if ((hw->div_width0 != 0) && (hw->div_width1 == 0))
		round_rate = clk_divider_ops.round_rate;
	if ((hw->div_width0 != 0) && (hw->div_width1 != 0))
		round_rate = clk_divider2_ops.round_rate;

	if (round_rate)
		return round_rate(hw, rate, prate);

	return -EINVAL;
}

static int clk_composite_set_rate(struct clk_hw *hw, u32 rate,
			       u32 parent_rate)
{
	int (*set_rate)(struct clk_hw *, u32, u32);

	set_rate = NULL;
	if ((hw->div_width0 != 0) && (hw->div_width1 == 0))
		set_rate = clk_divider_ops.set_rate;
	if ((hw->div_width0 != 0) && (hw->div_width1 != 0))
		set_rate = clk_divider2_ops.set_rate;

	if (set_rate)
		return set_rate(hw, rate, parent_rate);

	return -EINVAL;
}

static int clk_composite_is_enabled(struct clk_hw *hw)
{
	return clk_gate_ops.is_enabled(hw);
}

static int clk_composite_enable(struct clk_hw *hw)
{
	return clk_gate_ops.enable(hw);
}

static void clk_composite_disable(struct clk_hw *hw)
{
	clk_gate_ops.disable(hw);
}

struct clk_ops clk_module_ops = {
		.enable = clk_composite_enable,
		.disable = clk_composite_disable,
		.is_enabled = clk_composite_is_enabled,
		.recalc_rate = clk_composite_recalc_rate,
		.round_rate = clk_composite_round_rate,
		.set_rate = clk_composite_set_rate,
		.get_parent = clk_composite_get_parent,
		.set_parent = clk_composite_set_parent,
};

struct clk_div_table power_of_two_div_table[] = {
	{0, 1},
	{1, 2},
	{2, 4},
	{3, 0}
};

void clk_init_module(clk_t *clk)
{
	int i;
	clk->ops = &clk_module_ops;

	clk->num_parents = CLK_MAX_PARENTS;
	/* setup mux */
	for (i = 0; i < CLK_MAX_PARENTS; i++) {
		if (clk->hw->clk_src[i] == CLK_ID_INVALID) {
			clk->num_parents = i;
			break;
		}
	}
	for (i = 1; i < 31; i++) {
		if (clk->num_parents < BIT(i)) {
			clk->hw->src_width = i;
			break;
		}
	}

	/* for sdram_clk and cpu_clk */
	if (clk->hw->flags & CLK_DIVIDER_POWER_OF_TWO)
		clk->hw->table = power_of_two_div_table;
}

