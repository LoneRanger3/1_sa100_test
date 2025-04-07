/*
 * clk_debug.c - Debug function for the clock.
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

#define DBG_SECTION_NAME	"CLK_DUBUG"
#include <clk_debug.h>
#include <rtthread.h>
#include "csp.h"
#include "clk.h"
#include "clk_csp.h"
#include "clk_private.h"
#include "clk_pll.h"

static void clk_summary_show_one(clk_t *c, int level)
{
	if (!c)
		return;

	if (get_clk_flag(c) & CLK_GET_RATE_NOCACHE) {
		int parent_rate = 0;
		if (c->parent)
			parent_rate = c->parent->rate;

		if (c->ops->recalc_rate)
			c->rate = c->ops->recalc_rate(c->hw, parent_rate);
		else
			c->rate = parent_rate;

		if (IS_ERR(c->rate))
			c->rate = 0;
	}

	dbg_raw("%*s%-*s %-10d %-11d %-10lu",
		   level * 4 + 1, "",
		   30 - level * 4, __clk_get_name(c->hw),
		   c->enable_count, c->num_parents, c->rate);
	dbg_raw("\n");

}

void clk_summary_show_subtree(clk_t *c,
				     int level)
{
	clk_t *child;

	if (!c)
		return;

	clk_summary_show_one(c, level);

	hlist_for_each_entry(child, &c->children, child_node)
		clk_summary_show_subtree(child, level + 1);
}

int clk_tree(void)
{
	clk_t *c;

	dbg_raw("   clock                        enable_cnt num_parent rate\n");
	dbg_raw("----------------------------------------------------------\n");

	hlist_for_each_entry(c, &clk_root_list, child_node)
		clk_summary_show_subtree(c, 0);

	dbg_raw("----------------------------------------------------------\n");
	hlist_for_each_entry(c, &clk_orphan_list, child_node)
		clk_summary_show_subtree(c, 0);

	return 0;
}

static void clk_dump_one(clk_handle_t clk)
{
	int i;
	clk_t *c = handle_to_clk(clk);
	clk_t *child;
	clk_hw_t *hw;
	if (!c)
		return;
	hw = c->hw;
	if (!hw)
		return;

	dbg_raw("dump clock info for clk %s:\n", __clk_get_name(hw));
	dbg_raw("num_parents: %d\n", c->num_parents);
	if (c->parent)
		dbg_raw("cur parents: %s\n", __clk_get_name(c->parent->hw));
	dbg_raw("optional parents: ");
	for (i = 0; i < c->num_parents; i++)
		dbg_raw(" \"%s\",", __clk_get_name(c->parents[i]->hw));
	dbg_raw("\n");
	if (get_clk_flag(c) & CLK_GET_RATE_NOCACHE) {
		int parent_rate = 0;
		if (c->parent)
			parent_rate = c->parent->rate;

		if (c->ops->recalc_rate)
			c->rate = c->ops->recalc_rate(c->hw, parent_rate);
		else
			c->rate = parent_rate;

		if (IS_ERR(c->rate))
			c->rate = 0;
	}
	dbg_raw("rate: %lu\n", c->rate);
	dbg_raw("enable_count: %d\n", c->enable_count);
	dbg_raw("cur children: ");
	hlist_for_each_entry(child, &c->children, child_node)
		dbg_raw(" \"%s\",", __clk_get_name(child->hw));
	dbg_raw("\n");
	dbg_raw("===hw info====\n");
	if (hw->flags & CLK_TYPE_FIXED_CLOCK) {
		dbg_raw("type: CLK_TYPE_FIXED_CLOCK\n");
		dbg_raw("fixed rate:%d\n", hw->fixed_rate);
	}
	if (hw->flags & CLK_TYPE_FACT_CLOCK) {
		dbg_raw("type: CLK_TYPE_FACT_CLOCK\n");
		dbg_raw("factor:%d\n", hw->mult);
		dbg_raw("div:%d\n", hw->fix_div);
	}
	if (hw->flags & CLK_TYPE_GATE_CLOCK) {
		dbg_raw("type: CLK_TYPE_GATE_CLOCK\n");
		dbg_raw("reg(0x%08x):0x%08x\n", hw->reg, readl(hw->reg));
		dbg_raw("enable_bit:%d\n", hw->enable_bit);
		dbg_raw("enable_val:%x\n",
			(readl(hw->reg))&(BIT(hw->enable_bit)));
	}
	if (hw->flags & CLK_TYPE_DEVIDER_CLOCK) {
		dbg_raw("type: CLK_TYPE_DEVIDER_CLOCK\n");
		dbg_raw("reg(0x%08x):0x%08x\n", hw->reg, readl(hw->reg));
		dbg_raw("div_shift:%d\n", hw->div_shift0);
		dbg_raw("div_width:%x\n", hw->div_width0);
	}
	if (hw->flags & CLK_TYPE_PLL_CLOCK) {
		struct clock_pll *pll;
		dbg_raw("type: CLK_TYPE_PLL_CLOCK\n");
		pll = to_clock_pll(hw);
		if (pll == NULL)
			return;
		dbg_raw("reg[PLL_EN],0x%08x:0x%08x\n",
			pll->reg[PLL_EN], readl(pll->reg[PLL_EN]));
		dbg_raw("reg[FACTOR],0x%08x:0x%08x\n",
			pll->reg[FACTOR], readl(pll->reg[FACTOR]));
		dbg_raw("reg[TUNE0], 0x%08x:0x%08x\n",
			pll->reg[TUNE0], readl(pll->reg[TUNE0]));
		dbg_raw("reg[TEST],  0x%08x:0x%08x\n",
			pll->reg[TEST], readl(pll->reg[TEST]));
		dbg_raw("reg[STATUS],0x%08x:0x%08x\n",
			pll->reg[STATUS], readl(pll->reg[STATUS]));
		dbg_raw("reg[MODE],  0x%08x:0x%08x\n",
			pll->reg[MODE], readl(pll->reg[MODE]));
		dbg_raw("reg[NFAC],  0x%08x:0x%08x\n",
			pll->reg[NFAC], readl(pll->reg[NFAC]));
		dbg_raw("reg[TUNE2], 0x%08x:0x%08x\n",
			pll->reg[TUNE2], readl(pll->reg[TUNE2]));

		dbg_raw("en_shift[ENP], %x\n", pll->en_shift[ENP]);
		dbg_raw("en_shift[ENM], %x\n", pll->en_shift[ENM]);
		dbg_raw("en_shift[OEN], %x\n", pll->en_shift[OEN]);
		dbg_raw("en_shift[LOCK],%x\n", pll->en_shift[LOCK]);

		dbg_raw("fac_shift[PREV],%x\n", pll->fac_shift[PREV]);
		dbg_raw("fac_shift[POST],%x\n", pll->fac_shift[POST]);
		dbg_raw("fac_shift[FAC], %x\n", pll->fac_shift[FAC]);

		dbg_raw("fac_width[PREV],%x\n", pll->fac_width[PREV]);
		dbg_raw("fac_width[POST],%x\n", pll->fac_width[POST]);
		dbg_raw("fac_width[PREV],%x\n", pll->fac_width[FAC]);
	}
	if (hw->flags & CLK_TYPE_MODULE_CLOCK) {
		dbg_raw("type: CLK_TYPE_MODULE_CLOCK\n");
		dbg_raw("reg(%x):%x\n", hw->reg, readl(hw->reg));
		if (hw->enable_bit < 32)
			dbg_raw("enable_bit:%d\n", hw->enable_bit);
		dbg_raw("div_shift0:%d\n", hw->div_shift0);
		dbg_raw("div_width0:%x\n", hw->div_width0);
		if (hw->div_width1) {
			dbg_raw("div_shift1:%d\n", hw->div_shift1);
			dbg_raw("div_width1:%x\n", hw->div_width1);
		}
		dbg_raw("src_shift:%x\n", hw->src_shift);
	}
}

static int clk_dump(int argc, char **argv)
{
	clk_handle_t c;

	if (argc != 3)
		dbg_raw("input err! cmd shoud be \"clk dump clk_name\"");

	c = clk_get((const char *)argv[2]);
	if (c < 0)
		dbg_raw("input err! can not find clk %s\n", argv[2]);

	clk_dump_one(c);
	clk_put(c);
	return 0;
}


void show_clk_debug_help(void)
{
	dbg_raw("clock debug commands:\n");
	dbg_raw("%32s - dump clock tree\n", "clk tree");
	dbg_raw("%32s - dump info of specific clock\n", "clk dump \"name\"");
	dbg_raw("%32s - set parent of specific clock\n",
		"clk sp \"name\" \"parentname\"");
	dbg_raw("%32s - set rate of specific clock\n",
		"clk sr \"name\" rate");
	dbg_raw("%32s - enable specific clock\n", "clk enable \"name\"");
	dbg_raw("%32s - disable specific clock\n", "clk disable \"name\"");
}

long clk_debug(int argc, char **argv)
{
	clk_handle_t clock = 0;
	clk_handle_t p_clock = 0;
	int rate;
	int ret = -1;

	if (argc == 1) {
		show_clk_debug_help();
		return 0;
	}

	/* clk tree */
	if (!strncmp(argv[1], "tree", 4)) {
		clk_tree();
		return 0;
	}
	/* clk dump */
	if (!strncmp(argv[1], "dump", 4)) {
		clk_dump(argc, argv);
		return 0;
	}

	/* clk op name */
	if (argc < 3)
		goto quit;
	clock = clk_get(argv[2]);
	if (clock < 0) {
		LOG_E("get clock err!\n", argv[2]);
		goto quit;
	}

	/* clk enable name */
	if (!strncmp(argv[1], "enable", 6)) {
		clk_enable(clock);
		goto quit;
	}
	/* clk disable name */
	if (!strncmp(argv[1], "disable", 7)) {
		clk_disable(clock);
		goto quit;
	}

	/* clk op name param0 */
	if (argc < 4)
		goto quit;

	/* clk sp name pname */
	if (!strncmp(argv[1], "sp", 2)) {
		p_clock = clk_get(argv[3]);
		if (p_clock < 0) {
			LOG_E("get parent clock err!\n", argv[3]);
			goto quit;
		}
		clk_set_parent(clock, p_clock);
		LOG_I("clk set parent(%s %s) end\n", argv[2], argv[3]);
		goto quit;
	}
	if (!strncmp(argv[1], "sr", 2)) {
		rate = atoi(argv[3]);
		ret = clk_set_rate(clock, rate);
		if (ret)
			LOG_E("setclock %s rate to %d,failed.\n",
				argv[2], rate);
		LOG_I("%s %s %s end\n", argv[1], argv[2], argv[3]);
		goto quit;
	}
	show_clk_debug_help();

quit:
	if (clock)
		clk_put(clock);
	if (p_clock)
		clk_put(p_clock);
	return 0;
}

MSH_CMD_EXPORT(clk_debug, clock debug commands);

