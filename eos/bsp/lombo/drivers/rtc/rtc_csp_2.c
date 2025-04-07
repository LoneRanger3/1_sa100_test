/*
 * rtc_csp_2.c - the chip operations for RTC
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
#include "rtc_csp.h"
#include <debug.h>

/* N7V1 chip csp operation */

#define VA_RTC_PM_STAT                       (0x00000480 + BASE_RTC + VA_RTC)
#define VA_RTC_PM_PKT                        (0x00000484 + BASE_RTC + VA_RTC)
#define VA_RTC_PM_PE2                        (0x00000488 + BASE_RTC + VA_RTC)

/* Power Manage Status Register */
typedef union {
	u32 val;
	struct {
	u32 pwr_con:1;         /* */
	u32 rsvd0:31;          /* */
	} bits;
} reg_rtc_pm_stat_t;

typedef union {
	u32 val;
	struct {
	u32 pwr_up:9;                  /* */
	u32 key_long:9;                /* */
	u32 key_slong:10;              /* */
	u32 rsvd0:4;                   /* */
	} bits;
} reg_rtc_pm_pkt_t;

typedef union {
	u32 val;
	struct {
	u32 swi:1;                     /* */
	u32 rsvd0:15;                  /* */
	u32 key_filed:16;              /* */
	} bits;
} reg_rtc_pm_pe2_t;

/*
 * csp_rtc_get_pwr_con - get the state of power connection
 * @param: none
 *
 * return: 1, power connected; 0, power disconnected
 */
u32 csp_rtc_get_pwr_con()
{
	reg_rtc_pm_stat_t reg;

	reg.val = READREG32(VA_RTC_PM_STAT);
	return reg.bits.pwr_con;
}

/*
 * csp_rtc_set_key_time - set the counter time of power key action
 * @type: key time type
 * @ms: millisecond
 *
 * return: 1, power connected; 0, power disconnected
 */
void csp_rtc_set_key_time(RTC_PK_TIME_TYPE type, u32 ms)
{
	reg_rtc_pm_pkt_t reg;
	u32 t;

	reg.val = READREG32(VA_RTC_PM_PKT);
	t = ms / 10;	/* 10ms/per count */

	switch (type) {
	case TIME_TYPE_SLONG:
		reg.bits.key_slong = t;
		break;
	case TIME_TYPE_LONG:
		reg.bits.key_long = t;
		break;
	case TIME_TYPE_PWR_UP:
		reg.bits.pwr_up = t;
		break;
	default:
		LOG_W("unknow RTC_PK_TIME_TYPE: %d", type);
		return;
	}

	WRITEREG32(VA_RTC_PM_PKT, reg.val);
}

/* get power state machine switch */
u32 csp_rtc_get_pe2_swi()
{
	reg_rtc_pm_pe2_t reg;

	reg.val = READREG32(VA_RTC_PM_PE2);
	return reg.bits.swi;
}

/* set power state machine switch */
void csp_rtc_set_pe2_swi(u32 s)
{
	reg_rtc_pm_pe2_t reg;

	RT_ASSERT((s == 0) || (s == 1));
	reg.val = READREG32(VA_RTC_PM_PE2);

	reg.bits.swi = s;
	reg.val |= 0xEE180000;

	WRITEREG32(VA_RTC_PM_PE2, reg.val);
}
