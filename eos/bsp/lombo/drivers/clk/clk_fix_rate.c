/*
 * clk_fix_rate.c - operations for fixed rate clock
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
#define DBG_SECTION_NAME	"CLK_FIX"
#include <debug.h>
#include <rtthread.h>
#include "csp.h"
#include "clk.h"

#include "clk_csp.h"
#include "clk_private.h"

/*
 * DOC: basic fixed-rate clock that cannot gate
 *
 * Traits of this clock:
 * rate - rate is always a fixed value.  No clk_set_rate support
 * parent - root clock.  No clk_set_parent support
 */
static u32 clk_fixed_rate_recalc_rate(struct clk_hw *hw,
		u32 parent_rate)
{
	return hw->fixed_rate;
}

const struct clk_ops clk_fixed_rate_ops = {
	.recalc_rate = clk_fixed_rate_recalc_rate,
};

void clk_init_fixed_rate(clk_t *clk)
{
	clk->ops = &clk_fixed_rate_ops;
	clk->rate = clk->hw->fixed_rate;
	clk->new_rate = clk->rate;
	clk->hw->flags |= CLK_IS_ROOT;
}

