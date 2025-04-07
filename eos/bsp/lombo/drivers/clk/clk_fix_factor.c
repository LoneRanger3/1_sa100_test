/*
 * clk_fix_factor.c - operations for fixed factor clock
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
#define DBG_SECTION_NAME	"CLK_FAC"

#include <clk_debug.h>
#include <rtthread.h>
#include "csp.h"
#include "clk.h"

#include "clk_csp.h"
#include "clk_private.h"

static u32 clk_factor_recalc_rate(struct clk_hw *hw,
		u32 parent_rate)
{
	unsigned long long int rate;

#ifdef ARCH_LOMBO_N7
	const char *name = __clk_get_name(hw);
	if (is_audio_fix_divider_clk(name)) {
		LOG_D("%s(%d): clk(%s) is audio clk\n",
			__func__, __LINE__, name);
		rate = audio_clk_recalc_rate(name);
		if (!IS_ERR(rate))
			return rate;
	}
#endif

	rate = parent_rate;
	rate = rate * hw->mult;
	rate = rate/hw->fix_div;
	return (u32)rate;
}

static long clk_factor_round_rate(struct clk_hw *hw, u32 rate,
				u32 *prate)
{
	if (hw == NULL)
		return -EINVAL;
#ifdef ARCH_LOMBO_N7
	if (is_audio_fix_divider_clk(__clk_get_name(hw))) {
		LOG_E("%s(%d) err: audio clk(%s) cannot call clk_set_rate\n",
			__func__, __LINE__, __clk_get_name(hw));
		return -EINVAL; /* abort clk_set_rate */
	}
#endif

	if (hw->flags & CLK_SET_RATE_PARENT) {
		u32 best_parent;
		clk_t *clk;

		clk = get_clk_from_id(hw->id);
		best_parent = (rate / hw->mult) * hw->fix_div;
		best_parent = __clk_round_rate(__clk_get_parent(clk),
				best_parent);
		if (IS_ERR(best_parent))
			return best_parent;
		*prate = best_parent;
	}

	return (*prate / hw->fix_div) * hw->mult;
}

static int clk_factor_set_rate(struct clk_hw *hw, u32 rate,
				u32 parent_rate)
{
	return 0;
}

struct clk_ops clk_fixed_factor_ops = {
	.recalc_rate = clk_factor_recalc_rate,
	.round_rate = clk_factor_round_rate,
	.set_rate = clk_factor_set_rate,
};

void clk_init_fixed_factor(clk_t *clk)
{
	clk->ops = &clk_fixed_factor_ops;
	clk->num_parents = 1;

	clk->hw->flags |= CLK_SET_RATE_PARENT;
#ifdef ARCH_LOMBO_N7
	if (is_audio_fix_divider_clk(clk->hw->name))
		clk->hw->flags |= CLK_GET_RATE_NOCACHE;
#endif
}

