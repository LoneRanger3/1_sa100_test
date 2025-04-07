/*
 * clk_private.h
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

#ifndef __CLK_PRIVATE_H___
#define __CLK_PRIVATE_H___

#define true	1
#define false       0
#define MAX_ERRNO 300
#define IS_ERR(x) ((u32)(x) >= (u32)-MAX_ERRNO)

#define CLK_MAX_NOTIFIER 4
#define CLK_MAX_PARENTS 6

struct clk_div_table {
	u32	val;
	u32	div;
};

typedef struct clk_hw {
	const char *name;
	u32 flags;
	u32 reg;
	u32 fixed_rate;
	u8 id;
	u8 mult;
	u8 fix_div;
	u8 src_shift;
	u8 src_width;
	u8 enable_bit;
	u8 div_shift0;
	u8 div_width0;
	u8 div_shift1;
	u8 div_width1;
	u8 clk_src[CLK_MAX_PARENTS];
	struct clk_div_table *table;
} clk_hw_t;

typedef struct _clk {
	struct clk_hw		*hw;
	const struct clk_ops	*ops;
	struct _clk		*parent;
	struct _clk		*parents[CLK_MAX_PARENTS];
	u8			num_parents;
	u32			rate;
	u32			new_rate;
	u32			enable_count;
	struct hlist_head	children;
	struct hlist_node	child_node;
	u32			notifier_count;
	notifier_fn_t		*notifier_call;
} clk_t;

struct clk_ops {
	int	(*enable)(struct clk_hw *);
	void	(*disable)(struct clk_hw *);
	int	(*is_enabled)(struct clk_hw *);
	long	(*round_rate)(struct clk_hw *, u32, u32 *);
	u8	(*get_parent) (struct clk_hw *);
	int	(*set_parent) (struct clk_hw *, u8);
	u32	(*recalc_rate)(struct clk_hw *, u32);
	int	(*set_rate)(struct clk_hw *, u32, u32);
	void	(*init)(struct clk_hw *hw);
};

typedef struct clk_hw clk_divider_t;
typedef struct clk_hw clk_divider2_t;
typedef struct clk_hw clk_mux_t;

#define get_clk_from_id(id) (&g_clk_src[id])
#define get_clk_flag(clk) (clk->hw->flags)

extern const struct clk_ops clk_gate_ops;
extern const struct clk_ops clk_mux_ops;
extern const struct clk_ops clk_divider_ops;
extern struct hlist_head clk_root_list;
extern struct hlist_head clk_orphan_list;
extern clk_hw_t clk_hw_all[CLK_NUM];
extern clk_t g_clk_src[CLK_NUM];

/**
 * fls - find last (most-significant) bit set
 * @x: the word to search
 *
 * This is defined the same way as ffs.
 * Note fls(0) = 0, fls(1) = 1, fls(0x80000000) = 32.
 */
rt_inline rt_uint32_t __rt_fls(rt_uint32_t val)
{
	rt_uint32_t  bit = 32;

	if (!val)
		return 0;
	if (!(val & 0xffff0000u)) {
		val <<= 16;
		bit -= 16;
	}
	if (!(val & 0xff000000u)) {
		val <<= 8;
		bit -= 8;
	}
	if (!(val & 0xf0000000u)) {
		val <<= 4;
		bit -= 4;
	}
	if (!(val & 0xc0000000u)) {
		val <<= 2;
		bit -= 2;
	}
	if (!(val & 0x80000000u))
		bit -= 1;

	return bit;
}

/*
 * round up to nearest power of two
 */
static inline u32 __roundup_pow_of_two(u32 n)
{
	return 1UL << __rt_fls(n - 1);
}

/*
 * round down to nearest power of two
 */
static inline u32 __rounddown_pow_of_two(u32 n)
{
	return 1UL << (__rt_fls(n) - 1);
}
#define abs(x) ((x < 0) ? -x : x)

#define __clk_get_name(hw) (!hw->name ? "NULL" : hw->name)

static inline int __clk_get_num_parents(struct clk_hw *hw)
{
	clk_t *clk = get_clk_from_id(hw->id);
	return clk->num_parents;
}

u32 __get_div(u8 flags, u32 val,
			const struct clk_div_table *table);
int is_audio_fix_divider_clk(const char *name);
int is_audio_divider_clk(const char *name);
int is_audio_module_clk(const char *name);
int is_audio_pll_clk(const char *name);
u32 audio_clk_recalc_rate(const char *name);
long audio_clk_round_rate(char *name, u32 rate, u32 *prate);
int audio_clk_set_rate(struct clk_hw *hw, u32 rate, const char *name);
void init_clk_i2s();

u32 __clk_round_rate(clk_t *clk, u32 rate);
clk_t *__clk_get_parent(clk_t *clk);
extern u32 _get_table_div(const struct clk_div_table *table, u32 val);
extern void clk_init_fixed_rate(clk_t *clk);
void clk_init_fixed_factor(clk_t *clk);
void clk_init_gate(clk_t *clk);
void clk_init_divider(clk_t *clk);
void clk_init_pll(clk_t *clk);
void clk_init_module(clk_t *clk);
clk_t *handle_to_clk(clk_handle_t handle);
clk_handle_t clk_to_handle(clk_t *clk);

#endif
