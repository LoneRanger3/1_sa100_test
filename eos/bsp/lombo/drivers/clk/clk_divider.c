/*
 * clk_divider.c - operations for divider clock
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

#define DBG_SECTION_NAME	"CLK_DIV"
#include <clk_debug.h>
#include <rtthread.h>
#include "csp.h"
#include "clk.h"

#include "clk_csp.h"
#include "clk_private.h"

/*
 * DOC: basic adjustable divider clock that cannot gate
 *
 * Traits of this clock:
 * prepare - clk_prepare only ensures that parents are prepared
 * enable - clk_enable only ensures that parents are enabled
 * rate - rate is adjustable.  clk->rate = DIV_ROUND_UP(parent->rate / divisor)
 * parent - fixed parent.  No clk_set_parent support
 */

#define to_clk_divider(_hw) (_hw)

#define div_mask(d)	((1 << ((d)->div_width0)) - 1)

static u32 _get_table_maxdiv(const struct clk_div_table *table)
{

	u32 maxdiv = 0;
	const struct clk_div_table *clkt;

	for (clkt = table; clkt->div; clkt++)
		if (clkt->div > maxdiv)
			maxdiv = clkt->div;
	return maxdiv;
}

static u32 _get_table_mindiv(const struct clk_div_table *table)
{
	u32 mindiv = UINT_MAX;
	const struct clk_div_table *clkt;

	for (clkt = table; clkt->div; clkt++)
		if (clkt->div < mindiv)
			mindiv = clkt->div;
	return mindiv;
}

static u32 _get_maxdiv(clk_divider_t *divider)
{
	if (divider->flags & CLK_DIVIDER_ONE_BASED)
		return div_mask(divider);
	if (divider->flags & CLK_DIVIDER_POWER_OF_TWO)
		return 1 << div_mask(divider);
	if (divider->table)
		return _get_table_maxdiv(divider->table);
	return div_mask(divider) + 1;
}

u32 _get_table_div(const struct clk_div_table *table,
					u32 val)
{
	const struct clk_div_table *clkt;

	for (clkt = table; clkt->div; clkt++)
		if (clkt->val == val)
			return clkt->div;
	return 0;
}

/**
 * __get_div - get the division from raw value(directly read from reg)
 * @flags:	the clock divider flags
 * @val:	raw value directly read from reg
 * @table:	the division-val table
 *
 * return >0 when success, otherwise 0
 */
u32 __get_div(u8 flags, u32 val,
			const struct clk_div_table *table)
{
	if (flags & CLK_DIVIDER_ONE_BASED)
		return val;
	if (flags & CLK_DIVIDER_POWER_OF_TWO)
		return 1 << val;
	if (table)
		return _get_table_div(table, val);
	return val + 1;
}

static u32 _get_table_val(const struct clk_div_table *table,
						u32 div)
{
	const struct clk_div_table *clkt;

	for (clkt = table; clkt->div != 0; clkt++)
		if (clkt->div == div)
			return clkt->val;
	return 0;
}

static u32 _get_val(clk_divider_t *divider, u32 div)
{
	if (divider->flags & CLK_DIVIDER_ONE_BASED)
		return div;
	if (divider->flags & CLK_DIVIDER_POWER_OF_TWO) {
		if (divider->table)
			return  _get_table_val(divider->table, div);
		return __rt_ffs(div);
	}
	if (divider->table)
		return  _get_table_val(divider->table, div);
	return div - 1;
}

static u32 clk_divider_recalc_rate(struct clk_hw *hw,
		u32 parent_rate)
{
	clk_divider_t *divider = to_clk_divider(hw);
	u32 div, val;

#ifdef ARCH_LOMBO_N7
	int ret;
	const char *name = __clk_get_name(hw);
	if (is_audio_divider_clk(name) || is_audio_module_clk(name)) {
		ret = audio_clk_recalc_rate(name);
		if (!IS_ERR(ret))
			return ret;
	}
#endif

	val = readl(divider->reg) >> divider->div_shift0;
	val &= div_mask(divider);

	div = __get_div(divider->flags, val, divider->table);
	if (!div) {
		if (!(divider->flags & CLK_DIVIDER_ALLOW_ZERO))
			LOG_E("%s:Zero divisor & DIVIDER_ALLOW_ZERO not set\n",
			__clk_get_name(hw));
		return parent_rate;
	}

	return DIV_ROUND_UP(parent_rate, div);
}

/*
 * The reverse of DIV_ROUND_UP: The maximum number which
 * divided by m is r
 */
#define MULT_ROUND_UP(r, m) ((r) * (m) + (m) - 1)

static int _is_valid_table_div(const struct clk_div_table *table,
							 u32 div)
{
	const struct clk_div_table *clkt;

	for (clkt = table; clkt->div; clkt++)
		if (clkt->div == div)
			return true;
	return false;
}

static int _is_valid_div(clk_divider_t *divider, u32 div)
{
	if (divider->flags & CLK_DIVIDER_POWER_OF_TWO)
		return is_power_of_2(div);
	if (divider->table)
		return _is_valid_table_div(divider->table, div);
	return true;
}

static int _round_up_table(const struct clk_div_table *table, int div)
{
	const struct clk_div_table *clkt;
	int up = INT_MAX;

	for (clkt = table; clkt->div; clkt++) {
		if (clkt->div == div)
			return clkt->div;
		else if (clkt->div < div)
			continue;

		if ((clkt->div - div) < (up - div))
			up = clkt->div;
	}

	return up;
}

static int _round_down_table(const struct clk_div_table *table, int div)
{
	const struct clk_div_table *clkt;
	int down = _get_table_mindiv(table);

	for (clkt = table; clkt->div; clkt++) {
		if (clkt->div == div)
			return clkt->div;
		else if (clkt->div > div)
			continue;

		if ((div - clkt->div) < (div - down))
			down = clkt->div;
	}

	return down;
}

static int _div_round_up(clk_divider_t *divider,
		u32 parent_rate, u32 rate)
{
	int div = DIV_ROUND_UP(parent_rate, rate);

	if (divider->flags & CLK_DIVIDER_POWER_OF_TWO)
		div = __roundup_pow_of_two(div);
	if (divider->table)
		div = _round_up_table(divider->table, div);

	return div;
}

static int _div_round_closest(clk_divider_t *divider,
		u32 parent_rate, u32 rate)
{
	int up, down, div;

	up = down = div = DIV_ROUND_CLOSEST(parent_rate, rate);

	if (divider->flags & CLK_DIVIDER_POWER_OF_TWO) {
		up = __roundup_pow_of_two(div);
		down = __rounddown_pow_of_two(div);
	} else if (divider->table) {
		up = _round_up_table(divider->table, div);
		down = _round_down_table(divider->table, div);
	}

	return (up - div) <= (div - down) ? up : down;
}

static int _div_round(clk_divider_t *divider, u32 parent_rate,
		u32 rate)
{
	if (divider->flags & CLK_DIVIDER_ROUND_CLOSEST)
		return _div_round_closest(divider, parent_rate, rate);

	return _div_round_up(divider, parent_rate, rate);
}

static int _is_best_div(clk_divider_t *divider,
		u32 rate, u32 now, u32 best)
{
	if (divider->flags & CLK_DIVIDER_ROUND_CLOSEST)
		return abs(rate - now) < abs(rate - best);

	return now <= rate && now > best;
}

static int clk_divider_bestdiv(struct clk_hw *hw, u32 rate,
		u32 *best_parent_rate)
{
	clk_divider_t *divider;
	clk_t *clk;
	int i, bestdiv = 0;
	u32 parent_rate, now, maxdiv, best = 0;

	if (hw == NULL)
		return -1;

	divider = to_clk_divider(hw);
	clk = get_clk_from_id(hw->id);
	if (!rate)
		rate = 1;

	maxdiv = _get_maxdiv(divider);

	if (!(hw->flags & CLK_SET_RATE_PARENT)) {
		parent_rate = *best_parent_rate;
		bestdiv = _div_round(divider, parent_rate, rate);
		bestdiv = bestdiv == 0 ? 1 : bestdiv;
		bestdiv = bestdiv > maxdiv ? maxdiv : bestdiv;
		return bestdiv;
	}

	/*
	 * The maximum divider we can use without overflowing
	 * u32 in rate * i below
	 */
	maxdiv = min(ULONG_MAX / rate, maxdiv);

	for (i = 1; i <= maxdiv; i++) {
		if (!_is_valid_div(divider, i))
			continue;
		parent_rate = __clk_round_rate(clk,
				MULT_ROUND_UP(rate, i));
		now = DIV_ROUND_UP(parent_rate, i);
		if (_is_best_div(divider, rate, now, best)) {
			bestdiv = i;
			best = now;
			*best_parent_rate = parent_rate;
		}
	}

	if (!bestdiv) {
		bestdiv = _get_maxdiv(divider);
		*best_parent_rate = __clk_round_rate(__clk_get_parent(clk), 1);
	}

	return bestdiv;
}

static long clk_divider_round_rate(struct clk_hw *hw, u32 rate,
				u32 *prate)
{
	int div;

#ifdef ARCH_LOMBO_N7
	int ret;
	const char *name = __clk_get_name(hw);
	if (is_audio_divider_clk(name)) {
		LOG_E("%s(%d) err: audio clk(%s) cannot call clk_set_rate\n",
			__func__, __LINE__, name);
		return -EINVAL; /* abort clk_set_rate */
	}
	if (is_audio_module_clk(name)) {
		LOG_W("%s(%d): audio clk(%s)\n", __func__, __LINE__, name);
		return audio_clk_round_rate((char *)name, rate, prate);
	}
#endif

	div = clk_divider_bestdiv(hw, rate, prate);

#ifdef ARCH_LOMBO_N7
	ret = DIV_ROUND_UP(*prate, div);
	if (ret > rate) { /* rate too small to be set */
		LOG_W("err: clk(%s) rate(%d) too small(p:%d, div:%d)!",
			name, (int)rate, *prate, div);
		return -EINVAL; /* abort clk_set_rate */
	}
	return ret;
#else
	return DIV_ROUND_UP(*prate, div);
#endif
}

static int clk_divider_set_rate(struct clk_hw *hw, u32 rate,
				u32 parent_rate)
{
	clk_divider_t *divider = to_clk_divider(hw);
	u32 div, value;
	u32 val;
	if (rate <= 0)
		return -EINVAL;

#ifdef ARCH_LOMBO_N7
	const char *name = __clk_get_name(hw);
	if (is_audio_divider_clk(name)) {
		LOG_E("%s(%d) err: audio clk(%s) should not run here\n",
			__func__, __LINE__, name);
		/*
		 * clk_set_rate will be aborted by clk_divider_round_rate,
		 * so it should not be running here. BTW, we return ok
		 */
		return 0;
	}
	if (is_audio_module_clk(name)) {
		LOG_D("%s(%d): audio clk(%s)\n", __func__, __LINE__, name);
		return audio_clk_set_rate(hw, rate, name);
	}
#endif

	div = DIV_ROUND_UP(parent_rate, rate);

	if (!_is_valid_div(divider, div))
		return -EINVAL;

	value = _get_val(divider, div);

	if (value > div_mask(divider))
		value = div_mask(divider);

	val = readl(divider->reg);
	val &= ~(div_mask(divider) << divider->div_shift0);
	val |= value << divider->div_shift0;
	writel(val, divider->reg);

	return 0;
}

const struct clk_ops clk_divider_ops = {
	.recalc_rate = clk_divider_recalc_rate,
	.round_rate = clk_divider_round_rate,
	.set_rate = clk_divider_set_rate,
};

#ifdef ARCH_LOMBO_N7
struct clk_div_table power_of_two_div[] = {
	{0, 1},
	{1, 2},
	{2, 4},
	{3, 0}
};
#endif

void clk_init_divider(clk_t *clk)
{
	clk->ops = &clk_divider_ops;
	clk->num_parents = 1;

#ifdef ARCH_LOMBO_N7
	if (clk->hw->flags & CLK_DIVIDER_POWER_OF_TWO) {
		clk->hw->table = power_of_two_div;
		clk->hw->flags &= CLK_DIVIDER_POWER_OF_TWO;
	}
#endif
}

