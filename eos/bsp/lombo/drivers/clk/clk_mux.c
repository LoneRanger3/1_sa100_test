/*
 * clk_mux.c - operations for mux clock
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
#include <debug.h>
#include <rtthread.h>
#include "csp.h"
#include "clk.h"
#include "clk_csp.h"
#include "clk_private.h"

/*
 * DOC: basic adjustable multiplexer clock that cannot gate
 *
 * Traits of this clock:
 * prepare - clk_prepare only ensures that parents are prepared
 * enable - clk_enable only ensures that parents are enabled
 * rate - rate is only affected by parent switching.  No clk_set_rate support
 * parent - parent is adjustable through clk_set_parent
 */

#define to_clk_mux(_hw) (_hw)

u8 clk_mux_get_parent(struct clk_hw *hw)
{
	clk_mux_t *mux = to_clk_mux(hw);
	int num_parents = __clk_get_num_parents(hw);
	u32 val;

	/*
	 * FIXME need a mux-specific flag to determine if val is bitwise or numeric
	 * e.g. sys_clkin_ck's clksel field is 3 bits wide, but ranges from 0x1
	 * to 0x7 (index starts at one)
	 * OTOH, pmd_trace_clk_mux_ck uses a separate bit for each clock, so
	 * val = 0x4 really means "bit 2, index starts at bit 0"
	 */
	val = readl(mux->reg) >> mux->src_shift;
	val &= ((1<<mux->src_width) - 1);

	if (val >= num_parents) {
		LOG_E("%s %d: err index:%d\n",
			__func__, __LINE__, readl(mux->reg));
		return 0;
	}

	return val;
}

int clk_mux_set_parent(struct clk_hw *hw, u8 index)
{
	clk_mux_t *mux = to_clk_mux(hw);
	u32 val;

	val = readl(mux->reg);
	val &= ~(((1<<mux->src_width) - 1) << mux->src_shift);
	val |= index << mux->src_shift;
	writel(val, mux->reg);

	return 0;
}

const struct clk_ops clk_mux_ops = {
	.get_parent = clk_mux_get_parent,
	.set_parent = clk_mux_set_parent,
};

