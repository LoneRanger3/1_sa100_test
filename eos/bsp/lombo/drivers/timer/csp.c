/*
 * csp.c - the chip operations for timer
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

#include "timer.h"

#define REG_GAP(gtmr_no)		((gtmr_no) << 5) /* 0x20 * gtmr_no */
#define VA_GTIMER_INT_PENDING(n)	(VA_GTIMER_GTMR0_INT_PENDING + REG_GAP(n))
#define VA_GTIMER_CLR_INT_PENDING(n)	(VA_GTIMER_GTMR0_CLR_INT_PENDING + REG_GAP(n))
#define VA_GTIMER_CLK_CTRL(n)		(VA_GTIMER_GTMR0_CLK_CTRL + REG_GAP(n))
#define VA_GTIMER_STORE_VAL(n)		(VA_GTIMER_GTMR0_STORE_VAL + REG_GAP(n))
#define VA_GTIMER_CTRL(n)		(VA_GTIMER_GTMR0_CTRL + REG_GAP(n))
#define VA_GTIMER_INT_EN(n)		(VA_GTIMER_GTMR0_INT_EN + REG_GAP(n))

u32 csp_gtmr_get_int_pend(int gtmr_no)
{
	reg_gtimer_gtmr0_int_pending_t reg;

	RT_ASSERT(gtmr_no < GTMR_TOTAL);

	reg.val = READREG32(VA_GTIMER_INT_PENDING(gtmr_no));
	return reg.val;
}

void csp_gtmr_clr_int_pend(int gtmr_no)
{
	reg_gtimer_gtmr0_clr_int_pending_t reg;

	RT_ASSERT(gtmr_no < GTMR_TOTAL);

	reg.val = 0;
	reg.bits.clr = 1;
	WRITEREG32(VA_GTIMER_CLR_INT_PENDING(gtmr_no), reg.val);
}

void csp_gtmr_set_clk(int gtmr_no, u32 src, u32 div)
{
	reg_gtimer_gtmr0_clk_ctrl_t reg;

	RT_ASSERT(gtmr_no < GTMR_TOTAL);

	reg.val = READREG32(VA_GTIMER_CLK_CTRL(gtmr_no));
	reg.bits.clk_sel = src;
	reg.bits.clk_div = div;
	WRITEREG32(VA_GTIMER_CLK_CTRL(gtmr_no), reg.val);
}

void csp_gtmr_set_store(int gtmr_no, u32 val)
{
	reg_gtimer_gtmr0_store_val_t reg;

	RT_ASSERT(gtmr_no < GTMR_TOTAL);

	reg.bits.store_val = val;
	WRITEREG32(VA_GTIMER_STORE_VAL(gtmr_no), reg.val);
}

void csp_gtmr_set_cnt_mode(int gtmr_no, u32 dire, u32 mode)
{
	reg_gtimer_gtmr0_ctrl_t reg;

	RT_ASSERT(gtmr_no < GTMR_TOTAL);

	reg.val = READREG32(VA_GTIMER_CTRL(gtmr_no));
	reg.bits.direction = dire;
	reg.bits.mode = mode;
	WRITEREG32(VA_GTIMER_CTRL(gtmr_no), reg.val);
}

void csp_gtmr_enable(int gtmr_no)
{
	reg_gtimer_gtmr0_ctrl_t reg;

	RT_ASSERT(gtmr_no < GTMR_TOTAL);

	reg.val = READREG32(VA_GTIMER_CTRL(gtmr_no));
	/* wait gtimer ready to enable */
	while (reg.bits.nrdy)
		reg.val = READREG32(VA_GTIMER_CTRL(gtmr_no));

	reg.bits.enable = 1;
	WRITEREG32(VA_GTIMER_CTRL(gtmr_no), reg.val);
}

void csp_gtmr_disable(int gtmr_no)
{
	reg_gtimer_gtmr0_ctrl_t reg;

	RT_ASSERT(gtmr_no < GTMR_TOTAL);

	reg.val = READREG32(VA_GTIMER_CTRL(gtmr_no));
	reg.bits.enable = 0;
	WRITEREG32(VA_GTIMER_CTRL(gtmr_no), reg.val);
}

u32 csp_gtmr_get_ctrl(int gtmr_no)
{
	reg_gtimer_gtmr0_ctrl_t reg;

	RT_ASSERT(gtmr_no < GTMR_TOTAL);

	reg.val = READREG32(VA_GTIMER_CTRL(gtmr_no));
	return reg.val;
}

void csp_gtmr_irq_enable(int gtmr_no)
{
	reg_gtimer_gtmr0_int_en_t reg;

	RT_ASSERT(gtmr_no < GTMR_TOTAL);

	reg.val = READREG32(VA_GTIMER_INT_EN(gtmr_no));
	reg.bits.enable = 1; /* enable gtimer irq */
	WRITEREG32(VA_GTIMER_INT_EN(gtmr_no), reg.val);
}

void csp_gtmr_irq_disable(int gtmr_no)
{
	reg_gtimer_gtmr0_int_en_t reg;

	RT_ASSERT(gtmr_no < GTMR_TOTAL);

	reg.val = READREG32(VA_GTIMER_INT_EN(gtmr_no));
	reg.bits.enable = 0; /* disable gtimer irq */
	WRITEREG32(VA_GTIMER_INT_EN(gtmr_no), reg.val);
}

