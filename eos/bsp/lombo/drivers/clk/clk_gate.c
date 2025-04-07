/*
 * clk_gate.c - operations for gate clock
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
#define DBG_SECTION_NAME	"CLK_GAT"
#include <debug.h>
#include <rtthread.h>
#include "csp.h"
#include "clk.h"
#include "clk_csp.h"
#include "clk_private.h"

/**
 * DOC: basic gatable clock which can gate and ungate it's ouput
 *
 * Traits of this clock:
 * prepare - clk_(un)prepare only ensures parent is (un)prepared
 * enable - clk_enable and clk_disable are functional & control gating
 * rate - inherits rate from parent.  No clk_set_rate support
 * parent - fixed parent.  No clk_set_parent support
 */

/*
 * It works on following logic:
 *
 * For enabling clock, enable = 1
 *	set2dis = 1	-> clear bit	-> set = 0
 *	set2dis = 0	-> set bit	-> set = 1
 *
 * For disabling clock, enable = 0
 *	set2dis = 1	-> set bit	-> set = 1
 *	set2dis = 0	-> clear bit	-> set = 0
 *
 * So, result is always: enable xor set2dis.
 */
#define is_valid_bit(bit) ((bit <= 31) ? true : false)

static void clk_gate_endisable(struct clk_hw *hw, int enable)
{
	int set = hw->flags & CLK_GATE_SET_TO_DISABLE ? 1 : 0;
	u32 reg;

	set ^= enable;
	reg = readl(hw->reg);
	if (set)
		reg |= BIT(hw->enable_bit);
	else
		reg &= ~BIT(hw->enable_bit);

	writel(reg, hw->reg);
}

static int clk_gate_enable(struct clk_hw *hw)
{
	if (is_valid_bit(hw->enable_bit))
		clk_gate_endisable(hw, 1);

	return 0;
}

static void clk_gate_disable(struct clk_hw *hw)
{
	if (is_valid_bit(hw->enable_bit))
		clk_gate_endisable(hw, 0);
}

static int clk_gate_is_enabled(struct clk_hw *hw)
{
	u32 reg;

	if (is_valid_bit(hw->enable_bit) == false)
		return 1;

	reg = readl(hw->reg);

	/* if a set bit disables this clk, flip it before masking */
	if (hw->flags & CLK_GATE_SET_TO_DISABLE)
		reg ^= BIT(hw->enable_bit);

	reg &= BIT(hw->enable_bit);

	return reg ? 1 : 0;
}

const struct clk_ops clk_gate_ops = {
	.enable = clk_gate_enable,
	.disable = clk_gate_disable,
	.is_enabled = clk_gate_is_enabled,
};

void clk_init_gate(clk_t *clk)
{
	clk->ops = &clk_gate_ops;
	clk->num_parents = 1;
	if (clk->hw->clk_src[0] == CLK_ID_INVALID) {
		clk->hw->flags |= CLK_IS_ROOT;
		clk->num_parents = 0;
	}
}

