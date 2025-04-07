/*
 * wdt.h - head file for watchdog module
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

#ifndef __LOMBO_WDOG_H__
#define __LOMBO_WDOG_H__

#include <soc_define.h>
#include "irq_numbers.h"

/* clock control */
#define WDOG_CLK_SRC_SEL_POS	(0)
#define WDOG_CLK_SRC_SEL_WIDTH	(1)

/* clock source */
typedef enum clock {
	CLK_RTC = (WDOG_WDOG_CLK_CTRL_SEL_0),
	CLK_32K = (WDOG_WDOG_CLK_CTRL_SEL_1),
	CLK_MAX
} clock_src;

/* response mode */
#define WDOG_RESP_MOD_POS	(1)
#define WDOG_RESP_MOD_WIDTH	(1)
#define WDOG_SYSTEM_RESET_RESP_MOD	(WDOG_WDOG_CTRL_RMOD_0)
#define WDOG_INTERRUPT_RESP_MOD	(WDOG_WDOG_CTRL_RMOD_1)

/* wodg enable */
#define WDOG_EN_POS	(0)
#define WDOG_EN_WIDTH	(1)

/* timeout period */
#define WDOG_TMROUT_PERIOD_POS	(0)
#define WDOG_TMROUT_PERIOD_WIDTH	(4)

/* timeout period enum */
typedef enum period {
	TM_0P5S	= (WDOG_WDOG_TMRPERIOD_TMRPERIOD_0),
	TM_1S	= (WDOG_WDOG_TMRPERIOD_TMRPERIOD_1),
	TM_2S	= (WDOG_WDOG_TMRPERIOD_TMRPERIOD_2),
	TM_3S	= (WDOG_WDOG_TMRPERIOD_TMRPERIOD_3),
	TM_4S	= (WDOG_WDOG_TMRPERIOD_TMRPERIOD_4),
	TM_5S	= (WDOG_WDOG_TMRPERIOD_TMRPERIOD_5),
	TM_6S	= (WDOG_WDOG_TMRPERIOD_TMRPERIOD_6),
	TM_7S	= (WDOG_WDOG_TMRPERIOD_TMRPERIOD_7),
	TM_8S	= (WDOG_WDOG_TMRPERIOD_TMRPERIOD_8),
	TM_9S	= (WDOG_WDOG_TMRPERIOD_TMRPERIOD_9),
	TM_10S	= (WDOG_WDOG_TMRPERIOD_TMRPERIOD_A),
	TM_11S	= (WDOG_WDOG_TMRPERIOD_TMRPERIOD_B),
	TM_12S	= (WDOG_WDOG_TMRPERIOD_TMRPERIOD_C),
	TM_13S	= (WDOG_WDOG_TMRPERIOD_TMRPERIOD_D),
	TM_14S	= (WDOG_WDOG_TMRPERIOD_TMRPERIOD_E),
	TM_16S	= (WDOG_WDOG_TMRPERIOD_TMRPERIOD_F),
	TM_MAX
} tmrout_period;

/* current value */
#define WDOG_CUR_VAL_MASK	(0xFFFFFFFF)

/* restart */
#define WDOG_RESTART_POS	(0)
#define WDOG_RESTART_WIDTH	(16)
#define WDOG_RESTART_KEY	(0xEE18)

/* wdog interrupt */
#define WDOG_INT_EN_POS	(0)
#define WDOG_INT_EN_WIDTH	(1)
#define WDOG_INT_CLR_PENDING_POS	(0)
#define WDOG_INT_CLR_PENDING_WIDTH	(1)
#define WDOG_INT_PENDING_POS	(0)
#define WDOG_INT_PENDING_WIDTH	(1)

#define WDOG_CONFIG_NAME	"wdog"
#define WDOG_DEV_NAME	"wdog"

int wdog_init_cfg(void);
int wdog_init(clock_src src, tmrout_period time);
void wdog_deinit(void);
void wdog_reset(void);
int wdog_enable(int enable);
int wdog_get_en_state(void);
void wdog_restart(void);
int wdog_get_count(void);

int csp_wdog_enable(int enadle);
int csp_get_wdog_en_state(int *state);
int csp_clr_wdog_irq_pend(void);
int csp_get_wdog_irq_pend(int *state);
int csp_wdog_irq_enable(int enadle);
int csp_get_wdog_irq_en_state(int *state);
int csp_wdog_tmrout_period(int period);
int csp_wdog_response_mod(int mode);
int csp_set_wdog_clk(int clk);
int csp_restart_wdog(void);
int csp_get_wdog_cnt_cur(int *count);
int csp_wdog_su_enable(int enable);

#endif /* __LOMBO_WDOG_H__ */
