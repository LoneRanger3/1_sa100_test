/*
 * csp.h - the chip operations file for RTC
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

#ifndef __N7_RTC_CSP_H__
#define __n7_RTC_CSP_H__

/* ALARM MATCH ENABLE ITEM */
#define ALARM_DAY_MATCH_EN	0x0010
#define ALARM_WEEK_MATCH_EN	0x0008
#define ALARM_HOUR_MATCH_EN	0x0004
#define ALARM_MINUTE_MATCH_EN	0x0002
#define ALARM_SECOND_MATCH_EN	0x0001

/* ALARM RANGE */
#define YEAR_MAX		2100
#define YEAR_MIN		1900
#define MONTH_MAX		12
#define MONTH_MIN		1
#define LPYEAR_MON_DAYS		29 /* days of February in leap year */
#define NLPYEAR_MON_DAYS	28 /* days of February in non leap year */
#define LIT_MON_DAYS		30 /* a solar month of 30 days */
#define BIG_MON_DAYS		31 /* a solar month of 31 days */
#define DAYS_MIN		1
#define WEEK_MIN		0 /* 0, sunday; 1~6, monday~satturday */
#define WEEK_MAX		6
#define HOUR_MIN		0
#define HOUR_MAX		23
#define MINUTE_MIN		0
#define MINUTE_MAX		59
#define SECOND_MIN		0
#define SECOND_MAX		59

#define PM_KEY_FIELD		(0xEE18)

typedef enum {
	PM_TYPE_KEY_RELEASE = 0,
	PM_TYPE_KEY_LONG,
	PM_TYPE_KEY_SHORT,
	PM_TYPE_KEY_PRESS,
	PM_TYPE_PW_DISCON,
	PM_TYPE_PW_CON,
	PM_TYPE_GS,
	PM_TYPE_RING_KEY,
	PM_TYPE_RING_KEY_LOW
} RTC_PM_TYPE;

typedef enum {
	TIME_TYPE_SLONG = 0,
	TIME_TYPE_LONG,
	TIME_TYPE_PWR_UP
} RTC_PK_TIME_TYPE;

/*
 * csp_rtc_get_ver - get the version of RTC
 * @param: none
 *
 * return value of the RTC version
 */
u32 csp_rtc_get_ver(void);

/*
 * csp_rtc_set_clk_src - set the CLK source of RTC
 * @clk_src: 1, LFEOSC; 0, RCOSC
 *
 * LFEOSC is recommended because it is high precision
 */
void csp_rtc_set_clk_src(u32 clk_src);

/*
 * csp_rtc_set_clk_rcosc_div - divide the RCOSC
 * @rc_div: frequency division
 *
 * frequency obtained = RCOSC / 32 / (rc_div + 1)
 */
void csp_rtc_set_clk_rcosc_div(u32 rc_div);

/*
 * csp_rtc_get_clk_src_stat - get source of RTC clock
 * @param: none
 *
 * return: 1, LFEOSC; 0, RCOSC
 */
u32 csp_rtc_get_clk_src_stat(void);

/*
 * csp_rtc_ld_enable - RTC LFEOSC detect enable
 * @ld_enable: 1, enable; 0, disable
 *
 */
void csp_rtc_ld_enable(u32 ld_enable);

/*
 * csp_rtc_ld_int_enable - RTC LFEOSC detect interrupt enable
 * @ld_int_enable: 1, interrupt enable; 0, interrupt disable
 *
 */
void csp_rtc_ld_int_enable(u32 ld_int_enable);

/*
 * csp_rtc_get_ld_int_enable - get state of RTC LFEOSC detect interrupt enable
 * @param: none
 *
 * return: 1, interrupt enable; 0, interrupt disable
 */
u32 csp_rtc_get_ld_int_enable(void);

/*
 * csp_rtc_ld_int_clr - clear the state of RTC LFEOSC detect interrupt pending
 * @param: none
 *
 * the bit of interrupt is writing 1 clears pending
 */
void csp_rtc_ld_int_clr(void);

/*
 * csp_rtc_get_ld_int_pending - get state of LFEOSC detect interrupt pending
 * @param: none
 *
 * return: 1, interrupt is pending; 0, interrupt is not pending
 */
u32 csp_rtc_get_ld_int_pending(void);

/*
 * csp_rtc_set_ymd - set date(year, month, day)
 * @day: range is depend on month
 * @month: 1~12
 * @year: 1900~2100
 *
 */
void csp_rtc_set_ymd(u32 day, u32 month, u32 year);

/*
 * csp_rtc_get_ymd - get date(year, month, day)
 * @param: none
 *
 * return: date
 */
u32 csp_rtc_get_ymd(void);

/*
 * csp_rtc_set_week - set week
 * @week: range 0~6; 0, sunday; 1~6, monday~saturday
 *
 */
void csp_rtc_set_week(u32 week);

/*
 * csp_rtc_get_week - get week
 * @param: none
 *
 * return: week: 0, sunday; 1~6, monday~saturday
 */
u32 csp_rtc_get_week(void);

/*
 * csp_rtc_set_hms - set time(second, minute, hour)
 * @second: 0~60
 * @minute: 0~60
 * @hour: 0~23
 *
 */
void csp_rtc_set_hms(u32 second, u32 minute, u32 hour);

/*
 * csp_rtc_get_hms - get time(second, minute, hour)
 * @param: none
 *
 * retrun: time
 */
u32 csp_rtc_get_hms(void);

/*
 * csp_rtc_alarm_int_enable - RTC alarm interrupt enable
 * @alarm_int_enable: 1, interrupt enable; 0, interrupt disable
 *
 */
void csp_rtc_alarm_int_enable(u32 alarm_int_enable);

/*
 * csp_rtc_get_alarm_int_enable - get the state of RTC alarm interrupt enable
 * @param: none
 *
 * return: 1, interrupt enable; 0, interrupt disable
 */
u32 csp_rtc_get_alarm_int_enable(void);

/*
 * csp_rtc_alarm_int_clr - clear RTC alarm interrupt pending
 * @param: none
 *
 * the bit of interrupt is writing 1 clears pending
 */
void csp_rtc_alarm_int_clr(void);

/*
 * csp_rtc_get_alarm_int_pending - get the state of RTC alarm interrupt pending
 * @param: none
 *
 * return: 1, interrupt is pending; 0, interrupt is not pending
 */
u32 csp_rtc_get_alarm_int_pending(void);

/*
 * csp_rtc_alarm_match_enable - enable alarm match for counter
 * @alarm_match_enable_item:	0x0010, ALARM_DAY_MATCH_EN;
 *				0x0008, ALARM_WEEK_MATCH_EN;
 *				0x0004, ALARM_HOUR_MATCH_EN;
 *				0x0002, ALARM_MINUTE_MATCH_EN;
 *				0x0001, ALARM_SECOND_MATCH_EN;
 *
 */
void csp_rtc_alarm_match_enable(int alarm_match_enable_item);

/*
 * csp_rtc_alarm_match_disable - disable alarm match
 * @param: none
 *
 * disable all match of alarm
 */
void csp_rtc_alarm_match_disable(void);

/*
 * csp_rtc_alarm_match_get_enable - get alarm match enable stat
 * @param: none
 *
 * return alarm match enable stat
 */
u32 csp_rtc_alarm_match_get_enable(void);

/*
 * csp_rtc_set_alarm_day - set the alarm day for match
 * @day: range depends on month
 *
 */
void csp_rtc_set_alarm_day(u32 day);

/*
 * csp_rtc_get_alarm_day - get the alarm day
 * @param: none
 *
 * return: alarm day
 */
u32 csp_rtc_get_alarm_day(void);

/*
 * csp_rtc_set_alarm_week - set the alarm day for match
 * @week: 0~6
 *
 */
void csp_rtc_set_alarm_week(u32 week);

/*
 * csp_rtc_get_alarm_week - get the alarm week
 * @param: none
 *
 * return: alarm week
 */
u32 csp_rtc_get_alarm_week(void);

/*
 * csp_rtc_set_alarm_hms - set the alarm time(hour, minute, second) for match
 * @hour: 0~23
 * @minute: 0~59
 * @second: 0~59
 *
 */
void csp_rtc_set_alarm_hms(u32 hour, u32 minute, u32 second);

/*
 * csp_rtc_get_alarm_hms - get the alarm time(hour, minute, second)
 * @param: none
 *
 * return: alarm time(hour, minute, second)
 */
u32 csp_rtc_get_alarm_hms(void);

/*
 * csp_rtc_wakeup_enable - enable RTC wakeup for power
 * @wakeup_enable: 1, enable; 0, disable
 *
 */
void csp_rtc_wakeup_enable(u32 wakeup_enable);

/*
 * csp_rtc_get_wakeup_enable - get the enable status of RTC wakeup for power
 * @wakeup_enable: 1, enable; 0, disable
 *
 */
u32 csp_rtc_get_wakeup_enable(void);

/*
 * csp_rtc_lfeosc_fanout_enable - enable LFEOSC fanout
 * @lfeosc_out_enable: 1, enable; 0, disable
 *
 */
void csp_rtc_lfeosc_fanout_enable(u32 lfeosc_out_enable);

/*
 * csp_rtc_su_enable - enable RTC su
 * @lfeosc_out_enable: 1, enable; 0, disable
 *
 */
void csp_rtc_su_enable(u32 su_enable);

/*
 * csp_rtc_set_speed - set speed of clock
 * @speed: speed up second rate
 *
 */
void csp_rtc_set_speed(u32 speed);

/*
 * csp_rtc_pm_int_enable - one of power manager type interrupt enable
 * @type: RTC power manager type
 * @kl_int_enable: 1, interrupt enable; 0, interrupt disable
 *
 */
void csp_rtc_pm_int_enable(RTC_PM_TYPE type, u32 enable);

/*
 * csp_rtc_get_pm_int_enable - get state of one of power manager type interrupt
 * @type: RTC power manager type
 *
 * return: 1, interrupt enable; 0, interrupt disable
 */
u32 csp_rtc_get_pm_int_enable(RTC_PM_TYPE type);

/*
 * csp_rtc_pm_int_clr - clear the state of one of power manager type interrupt
 * @type: RTC power manager type
 *
 * the bit of interrupt is writing 1 clears pending
 */
void csp_rtc_pm_int_clr(RTC_PM_TYPE type);

/*
 * csp_rtc_get_pm_int_pending - get pending of one of power manager type interrupt
 * @type: RTC power manager type
 *
 * return: 1, interrupt is pending; 0, interrupt is not pending
 */
u32 csp_rtc_get_pm_int_pending(RTC_PM_TYPE type);

/*
 * csp_rtc_pm_get_irq - get interrupt status of one of power manager type
 * @type: RTC power manager type
 *
 * return: 1, interrupt hapened; 0, interrupt not happen
 */
u32 csp_rtc_pm_get_irq(RTC_PM_TYPE type);

/*
 * csp_rtc_pwr_en_disable - PWR_EN PIN software disable
 * @param: none
 *
 */
void csp_rtc_pwr_en_disable();

/*
 * csp_rtc_set_pwr_ddr_disable - PWR_DDR_EN PIN software disable
 * @param: none
 *
 */
void csp_rtc_set_pwr_ddr_disable();

void csp_rtc_pm_gs_enable(u32 enable);

/*
 * csp_rtc_clear_ddr_pad - clear ddr pad hold
 *
 * The booster will enter deepsleep resume flow if ddr pad holded, so
 * need to clear it before shutdown
 */
void csp_rtc_clear_ddr_pad();

/*
 * csp_rtc_get_pwr_con - get the state of power connection
 * @param: none
 *
 * return: 1, power connected; 0, power disconnected
 */
u32 csp_rtc_get_pwr_con();

/*
 * csp_rtc_set_key_time - set the counter time of power key action
 * @type: key time type
 * @ms: millisecond
 *
 * return: 1, power connected; 0, power disconnected
 */
void csp_rtc_set_key_time(RTC_PK_TIME_TYPE type, u32 ms);

/* get power state machine switch */
u32 csp_rtc_get_pe2_swi();

/* set power state machine switch */
void csp_rtc_set_pe2_swi(u32 s);

#endif
