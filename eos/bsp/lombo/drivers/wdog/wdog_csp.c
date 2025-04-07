/*
 * wdog_csp.c - the chip operations for watchdog
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

#include <csp.h>
#include <debug.h>
#include "wdog.h"

/* wdog enable */
int csp_wdog_enable(int enadle)
{
	reg_wdog_wdog_ctrl_t reg;

	reg.val = READREG32(VA_WDOG_WDOG_CTRL);
	reg.bits.en = enadle;
	WRITEREG32(VA_WDOG_WDOG_CTRL, reg.val);

	return RT_EOK;
}

/* get wdog enable status */
int csp_get_wdog_en_state(int *state)
{
	reg_wdog_wdog_ctrl_t reg;

	reg.val = READREG32(VA_WDOG_WDOG_CTRL);
	*state = reg.bits.en;

	return RT_EOK;
}

/* clear wdog interrupt pending */
int csp_clr_wdog_irq_pend(void)
{
	reg_wdog_wdog_clr_int_pending_t reg;

	reg.val = READREG32(VA_WDOG_WDOG_CLR_INT_PENDING);
	reg.bits.clr = 1;
	WRITEREG32(VA_WDOG_WDOG_CLR_INT_PENDING, reg.val);

	return RT_EOK;
}

/* get wdog interrupt pending */
int csp_get_wdog_irq_pend(int *state)
{
	reg_wdog_wdog_int_pending_t reg;

	reg.val = READREG32(VA_WDOG_WDOG_INT_PENDING);
	*state = reg.bits.pending;

	return RT_EOK;
}

/* enable wdog interrupt */
int csp_wdog_irq_enable(int enadle)
{
	reg_wdog_wdog_int_en_t reg;

	reg.val = READREG32(VA_WDOG_WDOG_INT_EN);
	reg.bits.enable = enadle;
	WRITEREG32(VA_WDOG_WDOG_INT_EN, reg.val);

	return RT_EOK;
}

/* get wdog interrupt enable status */
int csp_get_wdog_irq_en_state(int *state)
{
	reg_wdog_wdog_int_en_t reg;

	reg.val = READREG32(VA_WDOG_WDOG_INT_EN);
	*state = reg.bits.enable;

	return RT_EOK;
}

/* set wdog timeout period */
int csp_wdog_tmrout_period(int period)
{
	reg_wdog_wdog_tmrperiod_t reg;

	reg.val = READREG32(VA_WDOG_WDOG_TMRPERIOD);
	reg.bits.tmrperiod = period;
	WRITEREG32(VA_WDOG_WDOG_TMRPERIOD, reg.val);

	return RT_EOK;
}

/* set wdog response mode */
int csp_wdog_response_mod(int mode)
{
	reg_wdog_wdog_ctrl_t reg;

	reg.val = READREG32(VA_WDOG_WDOG_CTRL);
	reg.bits.rmod = mode;
	WRITEREG32(VA_WDOG_WDOG_CTRL, reg.val);

	return RT_EOK;
}

/* set wdog clock source */
int csp_set_wdog_clk(int clk)
{
	reg_wdog_wdog_clk_ctrl_t reg;

	reg.val = READREG32(VA_WDOG_WDOG_CLK_CTRL);
	reg.bits.sel = clk;
	WRITEREG32(VA_WDOG_WDOG_CLK_CTRL, reg.val);

	return RT_EOK;
}

/* do a wdog restart */
int csp_restart_wdog(void)
{
	reg_wdog_wdog_restart_t reg;

	reg.val = READREG32(VA_WDOG_WDOG_RESTART);
	reg.bits.restart = WDOG_RESTART_KEY;
	WRITEREG32(VA_WDOG_WDOG_RESTART, reg.val);

	return RT_EOK;
}

/* get wdog counter current value */
int csp_get_wdog_cnt_cur(int *count)
{
	int reg_val = 0;
	reg_wdog_wdog_cnt_cur_val_t reg;

	reg.val = READREG32(VA_WDOG_WDOG_CNT_CUR_VAL);
	reg_val &= WDOG_CUR_VAL_MASK;
	*count = reg.val;

	return RT_EOK;
}

/* enable wdog speed up */
int csp_wdog_su_enable(int enable)
{
	reg_wdog_wdog_ctrl_t reg;

	reg.val = READREG32(VA_WDOG_WDOG_CTRL);
	reg.bits.su = enable;
	WRITEREG32(VA_WDOG_WDOG_CTRL, reg.val);

	return RT_EOK;
}
