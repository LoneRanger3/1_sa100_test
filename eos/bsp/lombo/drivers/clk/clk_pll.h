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

#ifndef __CLK_PLL_H__
#define __CLK_PLL_H__
#include "board.h"

#ifdef ARCH_LOMBO_N7V0
#define MAX_PLL_CNT		8
#endif
#ifdef ARCH_LOMBO_N7V1
#define MAX_PLL_CNT		10
#endif

/* max freq-div count for pll */
#define MAX_FD_CNT_PLL		32

/* bit index in audio pll enable reg for DIV_ENDIV7 */
#define AUDIO_PLL_DIV7_BIT_IDX	9
/* bit index in audio pll enable reg for DIV_ENDIV17 */
#define AUDIO_PLL_DIV17_BIT_IDX	8

#define MASK(width)	((1 << (width)) - 1)

/* get factor, prev/post div from reg value */
#define GET(val, shift, width)	(((val) >> (shift)) & MASK(width))
#define FAC_N(val)	GET(val, pll->fac_shift[FAC], pll->fac_width[FAC])
#define PREV_DIV(val) (1 + GET(val, pll->fac_shift[PREV], pll->fac_width[PREV]))
#define POST_DIV(val) (1 + GET(val, pll->fac_shift[POST], pll->fac_width[POST]))

#define CLR_BITS(reg, shift, width)	((reg) & ~(MASK(width) << shift))
#define FORM_BITS(val, shift, width)	((val & MASK(width)) << shift)
#define SET(reg, val, shift, width)	(CLR_BITS(reg, shift, width) |	\
						FORM_BITS(val, shift, width))
/* set factor, prev/post div from reg value */
#define SET_FAC_N(reg, val)	SET(reg, val, pll->fac_shift[FAC],	\
					pll->fac_width[FAC])
#define SET_PREV_DIV(reg, val)	SET(reg, val, pll->fac_shift[PREV],	\
					pll->fac_width[PREV])
#define SET_POST_DIV(reg, val)	SET(reg, val, pll->fac_shift[POST],	\
					pll->fac_width[POST])

/* reg index in clock_pll->reg[] */
enum {
	PLL_EN = 0, FACTOR, TUNE0, TEST,
	STATUS, MODE, NFAC, TUNE1, TUNE2,
	MAX_PLL_REGS = TUNE2 + 1,
};

/* element index in clock_pll->en_shift[] */
enum { ENM = 0, ENP, LOCK, OEN };

/* element index in clock_pll->fac_shift[] or fac_width[] */
enum { PREV = 0, POST, FAC };

/* pll clock's freq-factor-div table */
struct pll_fac_dval_table {
	u32		rate;		/* pll clock rate in HZ */
	u32		parent_rate;	/* parent clock rate in HZ */
	int		factor;		/* pll factor N */
	int		prev_val;	/* PREV_DIV = prev_val + 1 */
	int		post_val;	/* PREV_DIV = prev_val + 1 */
};

/* pll clock struct */
struct clock_pll {
	struct clk_hw	*hw;
	void		*reg[MAX_PLL_REGS]; /* regs' address list for pll */
	u32		reg_cnt;	/* actual reg cnt, <= MAX_PLL_REGS */
	u32		en_shift[4];	/* bit index for enm enp lock oen */
	u32		fac_shift[3];	/* bit index for prevdiv postdiv n */
	u32		fac_width[3];	/* bit width for prevdiv postdiv n */
	u8		flags[2];	/* divider flags for prevdiv postdiv */
	const struct clk_div_table *table[2]; /* prevdiv and postdiv table */

	/*
	 * the pll freq and factor/div table
	 */
	int		fd_cnt;		/* element cnt of table */
	struct pll_fac_dval_table fd_table[MAX_FD_CNT_PLL];
};

struct clock_pll *to_clock_pll(struct clk_hw *hw);

#endif

