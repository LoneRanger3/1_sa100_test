/*
 * pwm_csp.c - Pwm driver for LomboTech Socs
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

#define DBG_SECTION_NAME	"PWM"
#define DBG_LEVEL		DBG_INFO
#include <debug.h>

#include <csp.h>
#include "pwm_csp.h"
#include "pwm_core.h"
#include "bitops.h"

/* pwm counter en-disable */
void csp_pwm_cnt_endisable(int pwm, void *reg_base, rt_bool_t en)
{
	u32 val, reg_addr = (u32)reg_base + PWM_CE_OFF;

	val = READREG32(reg_addr);
	val = en ? (val | BIT(pwm)) : (val & ~BIT(pwm));
	WRITEREG32(reg_addr, val);
}

/* pwm one pulse enable */
void csp_pwm_ops_endisable(int pwm, void *reg_base, rt_bool_t en)
{
	u32 val, reg_addr = (u32)reg_base + PWM_OPE_OFF;

	val = READREG32(reg_addr);
	val |= en ? (val | BIT(pwm)) : (val & ~BIT(pwm));
	WRITEREG32(reg_addr, val);
}

/*
 * pwm irq operations
 */

/* en-disable a pwm irq */
void csp_pwm_irq_endisable(int irq, void *reg_base, rt_bool_t en)
{
	u32 val, reg_addr = (u32)reg_base + PWM_INT_EN_OFF;

	val = READREG32(reg_addr);
	val = en ? (val | BIT(irq)) : (val & ~BIT(irq));
	WRITEREG32(reg_addr, val);
}

/* clear pending status of singe irq */
void csp_pwm_irq_clr_pend(int irq, void *reg_base)
{
	u32 reg_addr = (u32)reg_base + PWM_CLR_INT_PEND_OFF;

	WRITEREG32(reg_addr, BIT(irq));
}

/* get pending status of singe irq */
int csp_pwm_irq_get_pend(int irq, void *reg_base)
{
	u32 reg_addr = (u32)reg_base + PWM_INT_PEND_OFF;

	return !!(READREG32(reg_addr) & BIT(irq));
}

/* get pending status of singe irq */
int csp_pwm_get_irq_pend_regval(void *reg_base)
{
	u32 reg_addr = (u32)reg_base + PWM_INT_PEND_OFF;

	return READREG32(reg_addr);
}

/*
 * pwm clock operations
 */

void csp_pwm_clk_config(int pwm, void *reg_base, struct n7_pwm_clk_cfg *cfg)
{
	u32 reg_addr = (u32)reg_base + PWM_CLK_CTRL_OFF(pwm);
	reg_pwm_pwm0_clk_ctrl_t rval;

	rval.val = READREG32(reg_addr);
	rval.bits.bypass = (u32)cfg->bypass;
	rval.bits.gat = (u32)cfg->gating;
	rval.bits.src_sel = (u32)cfg->src;
	rval.bits.div = (u32)cfg->div - 1; /* reg value should be div-1 */
	WRITEREG32(reg_addr, rval.val);
}

int csp_pwm_clk_get_rate(int pwm, void *reg_base)
{
	u32 reg_addr = (u32)reg_base + PWM_CLK_CTRL_OFF(pwm);
	reg_pwm_pwm0_clk_ctrl_t rval;
	int div;

	rval.val = READREG32(reg_addr);
	switch (rval.bits.src_sel) {
	case CLKSRC_HFEOSC:
		div = rval.bits.div + 1;
		return 24000000 / div;
	case CLKSRC_NULL:
		LOG_E("clk src is NULL\n");
	default:
		return 0;
	}
}

/* pwm counter store set */
void csp_pwm_set_store(int pwm, void *reg_base, int val)
{
	u32 reg_addr = (u32)reg_base + PWM_CNT_STORE_OFF(pwm);
	reg_pwm_pwm0_cnt_store_t rval;

	if ((val & 0x0000ffff) == 0)
		LOG_W("store val = 0");

	rval.val = 0;
	rval.bits.store = val;
	WRITEREG32(reg_addr, rval.val);
}

/* pwm counter compare set */
void csp_pwm_set_cmp(int pwm, void *reg_base, int val)
{
	u32 reg_addr = (u32)reg_base + PWM_CNT_CMP_OFF(pwm);
	reg_pwm_pwm0_cnt_cmp_t rval;

	if ((val & 0x0000ffff) == 0)
		LOG_W("compare val = 0");

	rval.val = 0;
	rval.bits.cmp = val;
	WRITEREG32(reg_addr, rval.val);
}

/*
 * pwm control reg operations
 */

/**
 * csp_pwm_control - pwm control reg operations
 * @pwm: pwm channel index
 * @cmd: operation command
 * @reg_base: pwm reg's start vaddr
 * @para: para for the command
 *
 */
void csp_pwm_control(int pwm, int cmd, void *reg_base, int para)
{
	u32 reg_addr = (u32)reg_base + PWM_CTRL_OFF(pwm);
	reg_pwm_pwm0_ctrl_t rval;
	enum n7_pwm_pol pol_set = POLARITY_LOW; /* default */
	enum pwm_polarity pol;

	/* convert system pwm_polarity to hw polarity value */
	if (PWM_NO_POLARITY_SET == cmd || PWM_PO_POLARITY_SET == cmd) {
		pol = (enum pwm_polarity)para;
		if (PWM_POLARITY_NORMAL == pol)
			pol_set = POLARITY_LOW;
		else if (PWM_POLARITY_INVERSED == pol)
			pol_set = POLARITY_HIGH;
		else {
			LOG_E("pwm polarity para err\n");
			return;
		}
	}

	rval.val = READREG32(reg_addr);

	switch (cmd) {
	case PWM_PI_CAPTURE_EN:
		rval.bits.p_ie = (u32)para;
		break;
	case PWM_PI_CAPTURE_RVS:
		rval.bits.p_rev = (u32)para;
		break;
	case PWM_FE_DETECT_EN:
		rval.bits.fen = (u32)para;
		break;
	case PWM_RE_DETECT_EN:
		rval.bits.ren = (u32)para;
		break;
	case PWM_NO_POLARITY_SET:
		rval.bits.n_pol = (u32)pol_set;
		break;
	case PWM_NO_EN:
		rval.bits.n_oe = (u32)para;
		break;
	case PWM_PO_POLARITY_SET:
		rval.bits.p_pol = (u32)pol_set;
		break;
	case PWM_PO_EN:
		rval.bits.p_oe = (u32)para;
		break;
	case PWM_MODE_SET:
		rval.bits.mod = (u32)para;
		break;
	case PWM_DEADTIME_SET:
		rval.bits.dt = (u32)para;
		break;
	case PWM_DEADTIME_EN:
		rval.bits.dt_en = (u32)para;
		break;
	default:
		LOG_E("cmd err\n");
		break;
	}

	WRITEREG32(reg_addr, rval.val);
}

/*
 * pwm counter capture reg operations
 */

/* get pwm falling edge capture value(F_CAP) */
int csp_pwm_get_f_cap(int pwm, void *reg_base)
{
	u32 reg_addr = (u32)reg_base + PWM_CNT_CAP_OFF(pwm);
	reg_pwm_pwm0_cnt_cap_t rval;

	rval.val = READREG32(reg_addr);
	return rval.bits.f_cap;
}

/* get pwm rising edge capture value(R_CAP) */
int csp_pwm_get_r_cap(int pwm, void *reg_base)
{
	u32 reg_addr = (u32)reg_base + PWM_CNT_CAP_OFF(pwm);
	reg_pwm_pwm0_cnt_cap_t rval;

	rval.val = READREG32(reg_addr);
	return rval.bits.r_cap;
}

/*
 * pwm input status reg operations
 */

/* get pwm falling edge detect status(FFLAG) */
int csp_pwm_get_fflag(int pwm, void *reg_base)
{
	u32 reg_addr = (u32)reg_base + PWM_IN_STS_OFF(pwm);
	reg_pwm_pwm0_in_sts_t rval;

	rval.val = READREG32(reg_addr);
	return rval.bits.fflag;
}

/* get pwm rising edge detect status(RFLAG) */
int csp_pwm_get_rflag(int pwm, void *reg_base)
{
	u32 reg_addr = (u32)reg_base + PWM_IN_STS_OFF(pwm);
	reg_pwm_pwm0_in_sts_t rval;

	rval.val = READREG32(reg_addr);
	return rval.bits.rflag;
}

/* get pwm over falling edge detect status(OFFLAG) */
int csp_pwm_get_offlag(int pwm, void *reg_base)
{
	u32 reg_addr = (u32)reg_base + PWM_IN_STS_OFF(pwm);
	reg_pwm_pwm0_in_sts_t rval;

	rval.val = READREG32(reg_addr);
	return rval.bits.offlag;
}

/* get pwm over rising edge detect status(ORFLAG) */
int csp_pwm_get_orflag(int pwm, void *reg_base)
{
	u32 reg_addr = (u32)reg_base + PWM_IN_STS_OFF(pwm);
	reg_pwm_pwm0_in_sts_t rval;

	rval.val = READREG32(reg_addr);
	return rval.bits.orflag;
}

