/*
 * csp.c - the chip operations for RTC
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
#include <time.h>

/*
 * csp_rtc_get_ver - get the version of RTC
 * @param: none
 *
 * return value of the RTC version
 */
u32 csp_rtc_get_ver(void)
{
	reg_rtc_ver_t reg;

	reg.val = READREG32(VA_RTC_VER);
	return reg.val;
}

/*
 * csp_rtc_set_clk_src - set the CLK source of RTC
 * @clk_src: 1, LFEOSC; 0, RCOSC
 *
 * LFEOSC is recommended because it is high precision
 */
void csp_rtc_set_clk_src(u32 clk_src)
{
	reg_rtc_rtc_clk_ctrl_t reg;

	reg.val			= READREG32(VA_RTC_RTC_CLK_CTRL);
	reg.bits.sel		= clk_src;
	/* reg.bits.key_field	= 0xEE18; */
	reg.val |= 0xEE180000;

	WRITEREG32(VA_RTC_RTC_CLK_CTRL, reg.val);
}

/*
 * csp_rtc_set_clk_rcosc_div - divide the RCOSC
 * @rc_div: frequency division
 *
 * frequency obtained = RCOSC / 32 / (rc_div + 1)
 */
void csp_rtc_set_clk_rcosc_div(u32 rc_div)
{
	reg_rtc_rtc_clk_ctrl_t reg;

	reg.val			= READREG32(VA_RTC_RTC_CLK_CTRL);
	reg.bits.rcosc_div	= rc_div;
	/* reg.bits.key_field	= 0xEE18; */
	reg.val |= 0xEE180000;

	WRITEREG32(VA_RTC_RTC_CLK_CTRL, reg.val);
}

/*
 * csp_rtc_get_clk_src_stat - get source of RTC clock
 * @param: none
 *
 * return: 1, LFEOSC; 0, RCOSC
 */
u32 csp_rtc_get_clk_src_stat(void)
{
	reg_rtc_rtc_clk_ctrl_t reg;

	reg.val = READREG32(VA_RTC_RTC_CLK_CTRL);
	return reg.bits.stat;
}

/*
 * csp_rtc_ld_enable - RTC LFEOSC detect enable
 * @ld_enable: 1, enable; 0, disable
 *
 */
void csp_rtc_ld_enable(u32 ld_enable)
{
	reg_rtc_rtc_ld_en_t reg;

	reg.val		= READREG32(VA_RTC_RTC_LD_EN);
	reg.bits.en	= ld_enable;

	WRITEREG32(VA_RTC_RTC_LD_EN, reg.val);
}

/*
 * csp_rtc_ld_int_enable - RTC LFEOSC detect interrupt enable
 * @ld_int_enable: 1, interrupt enable; 0, interrupt disable
 *
 */
void csp_rtc_ld_int_enable(u32 ld_int_enable)
{
	reg_rtc_rtc_ld_int_en_t reg;

	reg.val		= READREG32(VA_RTC_RTC_LD_INT_EN);
	reg.bits.en	= ld_int_enable;

	WRITEREG32(VA_RTC_RTC_LD_INT_EN, reg.val);
}

/*
 * csp_rtc_get_ld_int_enable - get state of RTC LFEOSC detect interrupt enable
 * @param: none
 *
 * return: 1, interrupt enable; 0, interrupt disable
 */
u32 csp_rtc_get_ld_int_enable(void)
{
	reg_rtc_rtc_ld_int_en_t reg;

	reg.val	= READREG32(VA_RTC_RTC_LD_INT_EN);
	return reg.bits.en;
}

/*
 * csp_rtc_ld_int_clr - clear the state of RTC LFEOSC detect interrupt pending
 * @param: none
 *
 * the bit of interrupt is writing 1 clears pending
 */
void csp_rtc_ld_int_clr(void)
{
	reg_rtc_rtc_ld_int_clr_t reg;

	reg.val		= READREG32(VA_RTC_RTC_LD_INT_CLR);
	reg.bits.clr	= 1;

	WRITEREG32(VA_RTC_RTC_LD_INT_CLR, reg.val);
}

/*
 * csp_rtc_get_ld_int_pending - get state of LFEOSC detect interrupt pending
 * @param: none
 *
 * return: 1, interrupt is pending; 0, interrupt is not pending
 */
u32 csp_rtc_get_ld_int_pending(void)
{
	reg_rtc_rtc_ld_int_pending_t reg;

	reg.val = READREG32(VA_RTC_RTC_LD_INT_PENDING);
	return reg.bits.pending;
}

/*
 * csp_rtc_set_ymd - set date(year, month, day)
 * @day: range is depend on month
 * @month: 1~12
 * @year: 1900~2100
 *
 */
void csp_rtc_set_ymd(u32 day, u32 month, u32 year)
{
	reg_rtc_rtc_ymd_t reg;

	RT_ASSERT((month >= MONTH_MIN) && (month <= MONTH_MAX));
	RT_ASSERT((year >= YEAR_MIN) && (year <= YEAR_MAX));
	if ((2 != month) && (4 != month) && (6 != month) && (9 != month) &&
							(11 != month)) {
		RT_ASSERT((day >= DAYS_MIN) && (day <= BIG_MON_DAYS));
	} else {
		if (2 == month) {
			if ((((year % 4) == 0) && ((year % 100) != 0)) ||
							(year % 400 == 0)) {
				RT_ASSERT((day >= DAYS_MIN) &&
					(day <= LPYEAR_MON_DAYS));
			} else {
				RT_ASSERT((day >= DAYS_MIN) &&
					(day <= NLPYEAR_MON_DAYS));
			}
		} else {
			RT_ASSERT((day >= DAYS_MIN) && (day <= LIT_MON_DAYS));
		}
	}

	reg.val = READREG32(VA_RTC_RTC_YMD);
	reg.bits.day	= day;
	reg.bits.month	= month;
	reg.bits.year	= year;

	WRITEREG32(VA_RTC_RTC_YMD, reg.val);
}

/*
 * csp_rtc_get_ymd - get date(year, month, day)
 * @param: none
 *
 * return: date
 */
u32 csp_rtc_get_ymd(void)
{
	reg_rtc_rtc_ymd_t reg;

	reg.val = READREG32(VA_RTC_RTC_YMD);
	return reg.val;
}

/*
 * csp_rtc_set_week - set week
 * @week: range 0~6; 0, sunday; 1~6, monday~saturday
 *
 */
void csp_rtc_set_week(u32 week)
{
	reg_rtc_rtc_week_t reg;

	RT_ASSERT((week >= WEEK_MIN) && (week <= WEEK_MAX));

	reg.val = READREG32(VA_RTC_RTC_WEEK);
	reg.bits.week = week;

	WRITEREG32(VA_RTC_RTC_WEEK, reg.val);
}

/*
 * csp_rtc_get_week - get week
 * @param: none
 *
 * return: week: 0, sunday; 1~6, monday~saturday
 */
u32 csp_rtc_get_week(void)
{
	reg_rtc_rtc_week_t reg;

	reg.val = READREG32(VA_RTC_RTC_WEEK);
	return reg.val;
}

/*
 * csp_rtc_set_hms - set time(second, minute, hour)
 * @second: 0~60
 * @minute: 0~60
 * @hour: 0~23
 *
 */
void csp_rtc_set_hms(u32 second, u32 minute, u32 hour)
{
	reg_rtc_rtc_hms_t reg;

	RT_ASSERT((hour >= HOUR_MIN) && (hour <= HOUR_MAX));
	RT_ASSERT((minute >= MINUTE_MIN) && (minute <= MINUTE_MAX));
	RT_ASSERT((second >= SECOND_MIN) && (second <= SECOND_MAX));

	reg.val = READREG32(VA_RTC_RTC_HMS);
	reg.bits.second = second;
	reg.bits.minute = minute;
	reg.bits.hour = hour;

	WRITEREG32(VA_RTC_RTC_HMS, reg.val);
}

/*
 * csp_rtc_get_hms - get time(second, minute, hour)
 * @param: none
 *
 * retrun: time
 */
u32 csp_rtc_get_hms(void)
{
	reg_rtc_rtc_hms_t reg;

	reg.val = READREG32(VA_RTC_RTC_HMS);
	return reg.val;
}

/*
 * csp_rtc_alarm_int_enable - RTC alarm interrupt enable
 * @alarm_int_enable: 1, interrupt enable; 0, interrupt disable
 *
 */
void csp_rtc_alarm_int_enable(u32 alarm_int_enable)
{
	reg_rtc_rtc_alarm_int_en_t reg;

	reg.val = READREG32(VA_RTC_RTC_ALARM_INT_EN);
	reg.bits.en = alarm_int_enable;

	WRITEREG32(VA_RTC_RTC_ALARM_INT_EN, reg.val);
}

/*
 * csp_rtc_get_alarm_int_enable - get the state of RTC alarm interrupt enable
 * @param: none
 *
 * return: 1, interrupt enable; 0, interrupt disable
 */
u32 csp_rtc_get_alarm_int_enable(void)
{
	reg_rtc_rtc_alarm_int_en_t reg;

	reg.val = READREG32(VA_RTC_RTC_ALARM_INT_EN);
	return reg.bits.en;
}

/*
 * csp_rtc_alarm_int_clr - clear RTC alarm interrupt pending
 * @param: none
 *
 * the bit of interrupt is writing 1 clears pending
 */
void csp_rtc_alarm_int_clr(void)
{
	reg_rtc_rtc_alarm_int_clr_t reg;

	reg.val = READREG32(VA_RTC_RTC_ALARM_INT_CLR);
	reg.bits.clr = 1;

	WRITEREG32(VA_RTC_RTC_ALARM_INT_CLR, reg.val);
}

/*
 * csp_rtc_get_alarm_int_pending - get the state of RTC alarm interrupt pending
 * @param: none
 *
 * return: 1, interrupt is pending; 0, interrupt is not pending
 */
u32 csp_rtc_get_alarm_int_pending(void)
{
	reg_rtc_rtc_alarm_int_pending_t reg;

	reg.val = READREG32(VA_RTC_RTC_ALARM_INT_PENDING);
	return reg.bits.pending;
}

/*
 * csp_rtc_alarm_match_enable - enable alarm match for counter
 * @alarm_match_enable_item:	0x0010, ALARM_DAY_MATCH_EN;
 *				0x0008, ALARM_WEEK_MATCH_EN;
 *				0x0004, ALARM_HOUR_MATCH_EN;
 *				0x0002, ALARM_MINUTE_MATCH_EN;
 *				0x0001, ALARM_SECOND_MATCH_EN;
 *
 */
void csp_rtc_alarm_match_enable(int alarm_match_enable_item)
{
	reg_rtc_rtc_alarm_match_en_t reg;

	reg.val = 0;
	if (alarm_match_enable_item & ALARM_SECOND_MATCH_EN)
		reg.bits.sec_alarm_match_en = 1;
	if (alarm_match_enable_item & ALARM_MINUTE_MATCH_EN)
		reg.bits.min_alarm_match_en = 1;
	if (alarm_match_enable_item & ALARM_HOUR_MATCH_EN)
		reg.bits.hour_alarm_match_en = 1;
	if (alarm_match_enable_item & ALARM_WEEK_MATCH_EN)
		reg.bits.week_alarm_match_en = 1;
	if (alarm_match_enable_item & ALARM_DAY_MATCH_EN)
		reg.bits.day_alarm_match_en = 1;

	WRITEREG32(VA_RTC_RTC_ALARM_MATCH_EN, reg.val);
}

/*
 * csp_rtc_alarm_match_get_enable - get alarm match enable stat
 * @param: none
 *
 * return alarm match enable stat
 */
u32 csp_rtc_alarm_match_get_enable(void)
{
	reg_rtc_rtc_alarm_match_en_t reg;

	reg.val = READREG32(VA_RTC_RTC_ALARM_MATCH_EN);
	return reg.val;
}

/*
 * csp_rtc_alarm_match_disable - disable alarm match
 * @param: none
 *
 * disable all match of alarm
 */
void csp_rtc_alarm_match_disable(void)
{
	reg_rtc_rtc_alarm_match_en_t reg;

	reg.val = 0;
	WRITEREG32(VA_RTC_RTC_ALARM_MATCH_EN, reg.val);
}

/*
 * csp_rtc_set_alarm_day - set the alarm day for match
 * @day: range depends on month
 *
 */
void csp_rtc_set_alarm_day(u32 day)
{
	reg_rtc_rtc_alarm_day_match_t reg;

	RT_ASSERT((day >= DAYS_MIN) && (day <= BIG_MON_DAYS));

	reg.val = READREG32(VA_RTC_RTC_ALARM_DAY_MATCH);
	reg.bits.day_match = day;

	WRITEREG32(VA_RTC_RTC_ALARM_DAY_MATCH, reg.val);
}

/*
 * csp_rtc_get_alarm_day - get the alarm day
 * @param: none
 *
 * return: alarm day
 */
u32 csp_rtc_get_alarm_day(void)
{
	reg_rtc_rtc_alarm_day_match_t reg;

	reg.val = READREG32(VA_RTC_RTC_ALARM_DAY_MATCH);
	return reg.bits.day_match;
}

/*
 * csp_rtc_set_alarm_week - set the alarm day for match
 * @week: 0~6
 *
 */
void csp_rtc_set_alarm_week(u32 week)
{
	reg_rtc_rtc_alarm_week_match_t reg;

	RT_ASSERT((week >= WEEK_MIN) && (week <= WEEK_MAX));

	reg.val = READREG32(VA_RTC_RTC_ALARM_WEEK_MATCH);
	reg.bits.week_match = week;

	WRITEREG32(VA_RTC_RTC_ALARM_WEEK_MATCH, reg.val);
}

/*
 * csp_rtc_get_alarm_week - get the alarm week
 * @param: none
 *
 * return: alarm week
 */
u32 csp_rtc_get_alarm_week(void)
{
	reg_rtc_rtc_alarm_week_match_t reg;

	reg.val = READREG32(VA_RTC_RTC_ALARM_WEEK_MATCH);
	return reg.bits.week_match;
}

/*
 * csp_rtc_set_alarm_hms - set the alarm time(hour, minute, second) for match
 * @hour: 0~23
 * @minute: 0~59
 * @second: 0~59
 *
 */
void csp_rtc_set_alarm_hms(u32 hour, u32 minute, u32 second)
{
	reg_rtc_rtc_alarm_hms_match_t reg;

	RT_ASSERT((hour >= HOUR_MIN) && (hour <= HOUR_MAX));
	RT_ASSERT((minute >= MINUTE_MIN) && (minute <= MINUTE_MAX));
	RT_ASSERT((second >= SECOND_MIN) && (second <= SECOND_MAX));

	reg.val = READREG32(VA_RTC_RTC_ALARM_HMS_MATCH);
	reg.bits.second_match = second;
	reg.bits.minute_match = minute;
	reg.bits.hour_match = hour;

	WRITEREG32(VA_RTC_RTC_ALARM_HMS_MATCH, reg.val);
}

/*
 * csp_rtc_get_alarm_hms - get the alarm time(hour, minute, second)
 * @param: none
 *
 * return: alarm time(hour, minute, second)
 */
u32 csp_rtc_get_alarm_hms(void)
{
	reg_rtc_rtc_alarm_hms_match_t reg;

	reg.val = READREG32(VA_RTC_RTC_ALARM_HMS_MATCH);
	return reg.val;
}

/*
 * csp_rtc_wakeup_enable - enable RTC wakeup for power
 * @wakeup_enable: 1, enable; 0, disable
 *
 */
void csp_rtc_wakeup_enable(u32 wakeup_enable)
{
	reg_rtc_rtc_wakeup_en_t reg;

	reg.val = READREG32(VA_RTC_RTC_WAKEUP_EN);
	reg.bits.wakeup_en = wakeup_enable;

	WRITEREG32(VA_RTC_RTC_WAKEUP_EN, reg.val);
}

/*
 * csp_rtc_get_wakeup_enable - get the enable status of RTC wakeup for power
 * @wakeup_enable: 1, enable; 0, disable
 *
 */
u32 csp_rtc_get_wakeup_enable(void)
{
	reg_rtc_rtc_wakeup_en_t reg;

	reg.val = READREG32(VA_RTC_RTC_WAKEUP_EN);
	return reg.bits.wakeup_en;
}

/*
 * csp_rtc_lfeosc_fanout_enable - enable LFEOSC fanout
 * @lfeosc_out_enable: 1, enable; 0, disable
 *
 */
void csp_rtc_lfeosc_fanout_enable(u32 lfeosc_out_enable)
{
	u32 addr = 0;

#ifdef ARCH_LOMBO_N7V0
	reg_rtc_rtc_lfeosc_fanout_en_t reg;
	addr = VA_RTC_RTC_LFEOSC_FANOUT_EN;
#elif defined ARCH_LOMBO_N7V1
	reg_rtc_rtc_lfeosc_fanout_cfg_t reg;
	addr = VA_RTC_RTC_LFEOSC_FANOUT_CFG;
#endif

	reg.val = READREG32(addr);
	reg.bits.lfeosc_fanout_en = lfeosc_out_enable;

	WRITEREG32(addr, reg.val);
}

/*
 * csp_rtc_su_enable - enable RTC su
 * @lfeosc_out_enable: 1, enable; 0, disable
 *
 */
void csp_rtc_su_enable(u32 su_enable)
{
	reg_rtc_rtc_su_en_t reg;

	reg.val = READREG32(VA_RTC_RTC_SU_EN);
	reg.bits.en = su_enable;

	WRITEREG32(VA_RTC_RTC_SU_EN, reg.val);
}

/*
 * csp_rtc_set_speed - set speed of clock
 * @speed: speed up second rate
 *
 */
void csp_rtc_set_speed(u32 speed)
{
	reg_rtc_rtc_su_en_t reg;

	reg.val = READREG32(VA_RTC_RTC_SU_EN);
	reg.bits.rate = speed;

	WRITEREG32(VA_RTC_RTC_SU_EN, reg.val);
}

#define REG_KEY_FILED		0xee18

/* config gsensor for wakeup */
void csp_rtc_pm_gs_enable(u32 enable)
{
	/* reg_rtc_pm_cfg_t cfg_reg; */
	reg_rtc_pm_int_en_t en_reg;
	reg_rtc_sio_func_r0_t sf_reg;
	u32 cfg_reg;

	RT_ASSERT((enable == 0) || (enable == 1));

	/* gsensor wake detect enable, and interrupt level trigger config */
	cfg_reg = READREG32(VA_RTC_PM_CFG);
	cfg_reg |= (REG_KEY_FILED << 16);
	if (enable)
		cfg_reg |= BIT(4);
	else
		cfg_reg &= ~BIT(4);
	/* high level trigger, correspond to the gsensor INT pin level */
	cfg_reg |= BIT(0);
	WRITEREG32(VA_RTC_PM_CFG, cfg_reg);

	/* gsensor interrupt detect interrupt enable */
	en_reg.val = READREG32(VA_RTC_PM_INT_EN);
	en_reg.bits.gs_int = enable;
	WRITEREG32(VA_RTC_PM_INT_EN, en_reg.val);

	/* SIO3: gs_wake */
	sf_reg.val = READREG32(VA_RTC_SIO_FUNC_R0);
	/* n7v0 default 0x3, n7v1 default 0x0 */
	if (enable)
		sf_reg.bits.sio3 = 0x3;
	else
		sf_reg.bits.sio3 = 0x0;
	WRITEREG32(VA_RTC_SIO_FUNC_R0, sf_reg.val);
}

static u32 g_kr_enable, g_kr_low_enable;
void csp_rtc_pm_kr_enable(u32 enable)
{
	/* pin sio5 for ring key wake */
	reg_rtc_pm_int_en_t en_reg;
	reg_rtc_sio_func_r0_t sf_reg;
	u32 cfg_reg;
	rt_bool_t high_trig;

	RT_ASSERT((enable == 0) || (enable == 1));

	g_kr_enable = enable;

	/* RING_KEY detect enable, and interrupt level trigger config */
	cfg_reg = READREG32(VA_RTC_PM_CFG);
	cfg_reg |= (REG_KEY_FILED << 16);
	if (enable)
		cfg_reg |= BIT(6);
	else
		cfg_reg &= ~BIT(6);

	/* RING_KEY level trigger config, high level trigger */
	high_trig = RT_TRUE;	/* acc use high level wake up */
	if (high_trig)
		cfg_reg |= BIT(2);
	else
		cfg_reg &= ~BIT(2);
	WRITEREG32(VA_RTC_PM_CFG, cfg_reg);

	/* RING_KEY interrupt enable */
	en_reg.val = READREG32(VA_RTC_PM_INT_EN);
	en_reg.bits.ring_key = enable;
	WRITEREG32(VA_RTC_PM_INT_EN, en_reg.val);

	/* set sio5 function: 00 IO disable, 01 input, 10 output, 11 RING_KEY_WAKE */
	sf_reg.val = READREG32(VA_RTC_SIO_FUNC_R0);
	if (enable)
		sf_reg.bits.sio5 = 0x3;
	WRITEREG32(VA_RTC_SIO_FUNC_R0, sf_reg.val);
}

void csp_rtc_pm_kr_enable_low(u32 enable)
{
	/* pin sio5 for ring key wake */
	reg_rtc_pm_int_en_t en_reg;
	reg_rtc_sio_func_r0_t sf_reg;
	u32 cfg_reg;
	rt_bool_t high_trig;

	RT_ASSERT((enable == 0) || (enable == 1));

	g_kr_low_enable = enable;

	/* RING_KEY detect enable, and interrupt level trigger config */
	cfg_reg = READREG32(VA_RTC_PM_CFG);
	cfg_reg |= (REG_KEY_FILED << 16);
	if (enable)
		cfg_reg |= BIT(6);
	else
		cfg_reg &= ~BIT(6);

	/* RING_KEY level trigger config, high level trigger */
	high_trig = RT_FALSE;	/* acc use low level wake up */
	if (high_trig)
		cfg_reg |= BIT(2);
	else
		cfg_reg &= ~BIT(2);
	WRITEREG32(VA_RTC_PM_CFG, cfg_reg);

	/* RING_KEY interrupt enable */
	en_reg.val = READREG32(VA_RTC_PM_INT_EN);
	en_reg.bits.ring_key = enable;
	WRITEREG32(VA_RTC_PM_INT_EN, en_reg.val);

	/* set sio5 function: 00 IO disable, 01 input, 10 output, 11 RING_KEY_WAKE */
	sf_reg.val = READREG32(VA_RTC_SIO_FUNC_R0);
	if (enable)
		sf_reg.bits.sio5 = 0x3;
	WRITEREG32(VA_RTC_SIO_FUNC_R0, sf_reg.val);
}

/*
 * csp_rtc_pm_int_enable - one of power manager type interrupt enable
 * @type: RTC power manager type
 * @kl_int_enable: 1, interrupt enable; 0, interrupt disable
 *
 */
void csp_rtc_pm_int_enable(RTC_PM_TYPE type, u32 enable)
{
	reg_rtc_pm_int_en_t reg;

	reg.val = READREG32(VA_RTC_PM_INT_EN);
	switch (type) {
	case PM_TYPE_KEY_RELEASE:
		reg.bits.key_release = enable;
		break;
	case PM_TYPE_KEY_LONG:
		reg.bits.key_long = enable;
		break;
	case PM_TYPE_KEY_SHORT:
		reg.bits.key_short = enable;
		break;
	case PM_TYPE_KEY_PRESS:
		reg.bits.key_press = enable;
		break;
	case PM_TYPE_PW_DISCON:
	case PM_TYPE_PW_CON:
		reg.bits.pwr_en = enable;
		break;
	case PM_TYPE_GS:
		csp_rtc_pm_gs_enable(enable);
		return;
	case PM_TYPE_RING_KEY:
		csp_rtc_pm_kr_enable(enable);
		return;
	case PM_TYPE_RING_KEY_LOW:
		csp_rtc_pm_kr_enable_low(enable);
		return;
	}

	WRITEREG32(VA_RTC_PM_INT_EN, reg.val);
}

/*
 * csp_rtc_get_pm_int_enable - get state of one of power manager type interrupt
 * @type: RTC power manager type
 *
 * return: 1, interrupt enable; 0, interrupt disable
 */
u32 csp_rtc_get_pm_int_enable(RTC_PM_TYPE type)
{
	reg_rtc_pm_int_en_t reg;
	u32 res = 0;

	reg.val = READREG32(VA_RTC_PM_INT_EN);
	switch (type) {
	case PM_TYPE_KEY_RELEASE:
		res = reg.bits.key_release;
		break;
	case PM_TYPE_KEY_LONG:
		res = reg.bits.key_long;
		break;
	case PM_TYPE_KEY_SHORT:
		res = reg.bits.key_short;
		break;
	case PM_TYPE_KEY_PRESS:
		res = reg.bits.key_press;
		break;
	case PM_TYPE_PW_DISCON:
	case PM_TYPE_PW_CON:
		res = reg.bits.pwr_en;
		break;
	case PM_TYPE_GS:
		res = reg.bits.gs_int;
		break;
	case PM_TYPE_RING_KEY:
		res = g_kr_enable;/* reg.bits.ring_key; */
		break;
	case PM_TYPE_RING_KEY_LOW:
		res = g_kr_low_enable;/* reg.bits.ring_key; */
		break;
	}

	return res;
}

/*
 * csp_rtc_pm_int_clr - clear the state of one of power manager type interrupt
 * @type: RTC power manager type
 *
 * the bit of interrupt is writing 1 clears pending
 */
void csp_rtc_pm_int_clr(RTC_PM_TYPE type)
{
	reg_rtc_pm_pend_clr_t reg;

	reg.val = READREG32(VA_RTC_PM_PEND_CLR);
	switch (type) {
	case PM_TYPE_KEY_RELEASE:
		reg.bits.key_release = 1;
		break;
	case PM_TYPE_KEY_LONG:
		reg.bits.key_long = 1;
		break;
	case PM_TYPE_KEY_SHORT:
		reg.bits.key_short = 1;
		break;
	case PM_TYPE_KEY_PRESS:
		reg.bits.key_press = 1;
		break;
	case PM_TYPE_PW_DISCON:
		reg.bits.pw_discon = 1;
		break;
	case PM_TYPE_PW_CON:
		reg.bits.pw_con = 1;
		break;
	case PM_TYPE_GS:
		reg.bits.gs_int = 1;
		break;
	case PM_TYPE_RING_KEY:
		reg.bits.ring_key = 1;
		break;
	case PM_TYPE_RING_KEY_LOW:
		reg.bits.ring_key = 1;
		break;
	}

	WRITEREG32(VA_RTC_PM_PEND_CLR, reg.val);
}

/*
 * csp_rtc_get_pm_int_pending - get pending of one of power manager type interrupt
 * @type: RTC power manager type
 *
 * return: 1, interrupt is pending; 0, interrupt is not pending
 */
u32 csp_rtc_get_pm_int_pending(RTC_PM_TYPE type)
{
	reg_rtc_pm_pend_t reg;
	u32 res = 0;

	reg.val = READREG32(VA_RTC_PM_PEND);
	switch (type) {
	case PM_TYPE_KEY_RELEASE:
		res = reg.bits.key_release;
		break;
	case PM_TYPE_KEY_LONG:
		res = reg.bits.key_long;
		break;
	case PM_TYPE_KEY_SHORT:
		res = reg.bits.key_short;
		break;
	case PM_TYPE_KEY_PRESS:
		res = reg.bits.key_press;
		break;
	case PM_TYPE_PW_DISCON:
		res = reg.bits.pw_discon;
		break;
	case PM_TYPE_PW_CON:
		res = reg.bits.pw_con;
		break;
	case PM_TYPE_GS:
		res = reg.bits.gs_int;
		break;
	case PM_TYPE_RING_KEY:
		res = reg.bits.ring_key;
		break;
	case PM_TYPE_RING_KEY_LOW:
		res = reg.bits.ring_key;
		break;
	}

	return res;
}

/*
 * csp_rtc_pm_get_irq - get interrupt status of one of power manager type
 * @type: RTC power manager type
 *
 * return: 1, interrupt hapened; 0, interrupt not happen
 */
u32 csp_rtc_pm_get_irq(RTC_PM_TYPE type)
{
	u32 enable, pending;

	enable = csp_rtc_get_pm_int_enable(type);
	pending = csp_rtc_get_pm_int_pending(type);
	return (enable & pending) == 1;
}

/*
 * csp_rtc_pwr_en_disable - PWR_EN PIN software disable
 * @param: none
 *
 */
void csp_rtc_pwr_en_disable()
{
	/* reg_rtc_pm_pe_t reg; */
	u32 reg;

	reg = READREG32(VA_RTC_PM_PE);
	reg |= (PM_KEY_FIELD << 16);
	reg &= ~BIT(0);

	WRITEREG32(VA_RTC_PM_PE, reg);
}

/*
 * csp_rtc_set_pwr_ddr_disable - PWR_DDR_EN PIN software disable
 * @param: none
 *
 */
void csp_rtc_set_pwr_ddr_disable()
{
	/* reg_rtc_pm_pe_t reg; */
	u32 reg;

	reg = READREG32(VA_RTC_PM_PE);
	reg |= (PM_KEY_FIELD << 16);
	reg &= ~BIT(1);

	WRITEREG32(VA_RTC_PM_PE, reg);
}

/*
 * csp_rtc_clear_ddr_pad - clear ddr pad hold
 *
 * The booster will enter deepsleep resume flow if ddr pad holded, so
 * need to clear it before shutdown
 */
void csp_rtc_clear_ddr_pad()
{
	reg_rtc_pad_hold_t reg;

	/* clear pad hold, or the booster will enter deepsleep resume flow */
	reg.val = READREG32(VA_RTC_PAD_HOLD);
	reg.bits.ddr_pad = 0;

	WRITEREG32(VA_RTC_PAD_HOLD, reg.val);
}

