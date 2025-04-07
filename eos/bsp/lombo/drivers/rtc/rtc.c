/*
 * rtc.c - RTC module realization
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
#include <rtthread.h>
#include <rtdevice.h>
#include <rthw.h>
#include <rtdef.h>
#include <debug.h>
#include <irq_numbers.h>
#include <csp.h>

#include <time.h>
#include <string.h>
#include "rtc.h"
#include "rtc_csp.h"
#include "../board.h"
#ifdef LOMBO_POWER
#include "../power/power_drv.h"
#endif

#define RTC_PCF8563_EXT

#ifdef RTC_PCF8563_EXT
rt_err_t rtc_pcf8563_setup_i2c_device(void);
int rtc_pcf8563_get_time(struct tm *p_tm);
int rtc_pcf8563_set_time(struct tm *p_tm);

static rt_tick_t g_syc_time;
#endif

#define PM_TYPE_NUMBER		6

static struct rt_device rtc;

static RTC_PM_TYPE pm_types[PM_TYPE_NUMBER] = {
		PM_TYPE_KEY_RELEASE, PM_TYPE_KEY_LONG,
		PM_TYPE_KEY_SHORT, PM_TYPE_KEY_PRESS,
		PM_TYPE_PW_DISCON, PM_TYPE_PW_CON
	};

/* hms_valid_judge - judge the time
 * @hour: 0~23
 * @minute: 0~59
 * @second: 0~59
 *
 * return: RT_ERROR, unvalid; RT_EOK, valid
 */
static int hms_valid_judge(u32 hour, u32 minute, u32 second)
{
	int ret = RT_ERROR;

	if ((hour >= HOUR_MIN) && (hour <= HOUR_MAX)) {
		if ((minute >= MINUTE_MIN) && (minute <= MINUTE_MAX)) {
			if ((second >= SECOND_MIN) && (second <= SECOND_MAX))
				ret = RT_EOK;
		}
	}

	return ret;
}

/* ymd_valid_judge - judge the date
 * @year: 1900~2100
 * @month: 1~12
 * @day: range depends on month
 *
 * return: RT_ERROR, unvalid; RT_EOK, valid
 */
static int ymd_valid_judge(u32 year, u32 month, u32 day)
{
	int ret = RT_ERROR;

	if ((year < YEAR_MIN) || (year > YEAR_MAX))
		goto out;
	if ((month < MONTH_MIN) || (month > MONTH_MAX))
		goto out;
	if (((month <= 7) && (month % 2 != 0)) || ((month > 7) &&
							(month % 2 == 0))) {
		if ((day >= DAYS_MIN) && (day <= BIG_MON_DAYS))
			ret = RT_EOK;
		else
			goto out;
	} else {
		if (2 == month) {
			if (((year % 4 == 0) && (year % 100 != 0)) ||
							(year % 400 == 0)) {
				if ((day >= DAYS_MIN) && (day <= LPYEAR_MON_DAYS))
					ret = RT_EOK;
				else
					goto out;
			} else {
				if ((day >= DAYS_MIN) && (day <= NLPYEAR_MON_DAYS))
					ret = RT_EOK;
				else
					goto out;
			}
		} else {
			if ((day >= DAYS_MIN) && (day <= LIT_MON_DAYS))
				ret = RT_EOK;
			else
				goto out;
		}
	}
out:
	return ret;
}

/*
 * rt_hw_rtc_wakeup - set enable status of RTC wakeup for power
 * @wakeup_en: 1, enable; 0, disable
 *
 * RTC wakeup output 1 when alarm is up, only effective when it is power off
 */
void rt_hw_rtc_wakeup(u32 wakeup_en)
{
	csp_rtc_wakeup_enable(wakeup_en);
}

/*
 * rtc_open - for RTOS system
 * @dev: pointer to device(currently unused)
 * @oflag: open flag(currently unused)
 *
 * return RT_EOK all the time
 *
 */
static rt_err_t rtc_open(rt_device_t dev, rt_uint16_t oflag)
{
	return RT_EOK;
}

/*
 * rtc_close - for RTOS system
 * @dev: pointer to device(currently unused)
 *
 * return RT_EOK all the time
 *
 */
static rt_err_t rtc_close(rt_device_t dev)
{
	return RT_EOK;
}

/* enable rtc power manager interrupt */
static void enable_rtc_pm_int()
{
	int i;
	for (i = 0; i < PM_TYPE_NUMBER; i++)
		csp_rtc_pm_int_enable(pm_types[i], ENABLE);
}

/* handle rtc power manager interrupt */
static void rtc_pm_int_handle()
{
	/* PWR_KEY short press ISR */
	if (csp_rtc_pm_get_irq(PM_TYPE_KEY_SHORT) == 1) {
		csp_rtc_pm_int_enable(PM_TYPE_KEY_SHORT, DISABLE);
		csp_rtc_pm_int_clr(PM_TYPE_KEY_SHORT);

#ifdef LOMBO_POWER
		power_key_handle(KEY_SHORT_PRESS);
#endif
		csp_rtc_pm_int_enable(PM_TYPE_KEY_SHORT, ENABLE);
	}

	/* PWR_KEY long press ISR */
	if (csp_rtc_pm_get_irq(PM_TYPE_KEY_LONG) == 1) {
		csp_rtc_pm_int_enable(PM_TYPE_KEY_LONG, DISABLE);
		csp_rtc_pm_int_clr(PM_TYPE_KEY_LONG);

#ifdef LOMBO_POWER
		power_key_handle(KEY_LONG_PRESS);
#endif
		csp_rtc_pm_int_enable(PM_TYPE_KEY_LONG, ENABLE);
	}

	/* PWR_KEY press ISR */
	if (csp_rtc_pm_get_irq(PM_TYPE_KEY_PRESS) == 1) {
		csp_rtc_pm_int_enable(PM_TYPE_KEY_PRESS, DISABLE);
		csp_rtc_pm_int_clr(PM_TYPE_KEY_PRESS);

#ifdef LOMBO_POWER
		power_key_handle(KEY_PRESS);
#endif
		csp_rtc_pm_int_enable(PM_TYPE_KEY_PRESS, ENABLE);
	}

	/* PWR_KEY release ISR */
	if (csp_rtc_pm_get_irq(PM_TYPE_KEY_RELEASE) == 1) {
		csp_rtc_pm_int_enable(PM_TYPE_KEY_RELEASE, DISABLE);
		csp_rtc_pm_int_clr(PM_TYPE_KEY_RELEASE);

#ifdef LOMBO_POWER
		power_key_handle(KEY_RELEASE);
#endif
		csp_rtc_pm_int_enable(PM_TYPE_KEY_RELEASE, ENABLE);
	}

	/* usb connect or disconnect */
	if ((csp_rtc_pm_get_irq(PM_TYPE_PW_CON) == 1) &&
		csp_rtc_pm_get_irq(PM_TYPE_PW_DISCON) == 1) {
		/*occur connected and disconnected interrupt at the same time */
		csp_rtc_pm_int_enable(PM_TYPE_PW_CON, DISABLE);
		csp_rtc_pm_int_clr(PM_TYPE_PW_CON);
		csp_rtc_pm_int_enable(PM_TYPE_PW_DISCON, DISABLE);
		csp_rtc_pm_int_clr(PM_TYPE_PW_DISCON);

#ifdef LOMBO_POWER
		/* the actual connection status needs to be determined */
		usb_connect_status_judge();
#endif
		csp_rtc_pm_int_enable(PM_TYPE_PW_CON, ENABLE);
		csp_rtc_pm_int_enable(PM_TYPE_PW_DISCON, ENABLE);
	} else if (csp_rtc_pm_get_irq(PM_TYPE_PW_CON) == 1) {
		/* power connected ISR */
		csp_rtc_pm_int_enable(PM_TYPE_PW_CON, DISABLE);
		csp_rtc_pm_int_clr(PM_TYPE_PW_CON);

#ifdef LOMBO_POWER
		power_connect();
#endif
		csp_rtc_pm_int_enable(PM_TYPE_PW_CON, ENABLE);
	} else if (csp_rtc_pm_get_irq(PM_TYPE_PW_DISCON) == 1) {
		/* power disconnected ISR */
		csp_rtc_pm_int_enable(PM_TYPE_PW_DISCON, DISABLE);
		csp_rtc_pm_int_clr(PM_TYPE_PW_DISCON);

#ifdef LOMBO_POWER
		power_disconnect();
#endif
		csp_rtc_pm_int_enable(PM_TYPE_PW_DISCON, ENABLE);
	}

	/* gsensor ISR */
	if (csp_rtc_pm_get_irq(PM_TYPE_GS) == 1) {
		csp_rtc_pm_int_enable(PM_TYPE_GS, DISABLE);
		csp_rtc_pm_int_clr(PM_TYPE_GS);

#ifdef LOMBO_POWER
		gsensor_irq_handle();
#endif
		csp_rtc_pm_int_enable(PM_TYPE_GS, ENABLE);
	}

	/* ring key ISR */
	if (csp_rtc_pm_get_irq(PM_TYPE_RING_KEY) == 1) {
		csp_rtc_pm_int_enable(PM_TYPE_RING_KEY, DISABLE);
		csp_rtc_pm_int_clr(PM_TYPE_RING_KEY);
		csp_rtc_pm_int_enable(PM_TYPE_RING_KEY, ENABLE);
	}

	/* ring key ISR */
	if (csp_rtc_pm_get_irq(PM_TYPE_RING_KEY_LOW) == 1) {
		csp_rtc_pm_int_enable(PM_TYPE_RING_KEY_LOW, DISABLE);
		csp_rtc_pm_int_clr(PM_TYPE_RING_KEY_LOW);
		csp_rtc_pm_int_enable(PM_TYPE_RING_KEY_LOW, ENABLE);
	}
}

/*
 * rt_rtc_int_isr - lfeosc detect and alarm interrupt handler
 * @vector: index in isr_table[]
 * @param: parament for handle
 *
 */
void rt_rtc_int_isr(int vector, void *param)
{
	u32 ld_int_pending = 0, ld_int_enable = 0;
	u32 alarm_int_pending = 0, alarm_int_enable = 0;
	u32 clk_src_stat = 1;

	ld_int_enable = csp_rtc_get_ld_int_enable();
	ld_int_pending = csp_rtc_get_ld_int_pending();
	alarm_int_enable = csp_rtc_get_alarm_int_enable();
	alarm_int_pending = csp_rtc_get_alarm_int_pending();

	/* LFEOSC detect ISR */
	if ((ld_int_enable & ld_int_pending) == 1) {
		clk_src_stat = csp_rtc_get_clk_src_stat();
		if (CLK_SRC_LFEOSC != clk_src_stat) {
			LOG_E("No external low-frequency crystal oscillator detected");
			while (CLK_SRC_RCOSC != clk_src_stat) {
				clk_src_stat = csp_rtc_get_clk_src_stat();
				csp_rtc_set_clk_src(CLK_SRC_RCOSC);
			}
			/* disable LFEOSC fanout */
			csp_rtc_lfeosc_fanout_enable(DISABLE);
		}
		csp_rtc_ld_int_clr();
	}
	/* alarm ISR */
	if ((alarm_int_enable & alarm_int_pending) == 1) {
		rt_alarm_update(RT_NULL, RT_NULL);
		csp_rtc_alarm_int_clr();
	}

	/* handle rtc power manager interrupt */
	rtc_pm_int_handle();
}
#ifdef ARCH_LOMBO_N7V1_CDR
static void *__gsensor_det_fun(void *param)
{
	pthread_detach(pthread_self());
	while (1) {
		rt_sem_take(gsensor_det_sem, RT_WAITING_FOREVER);
		lb_system_mq_send(LB_SYSMSG_GSENSOR, NULL, 0, 0);
		LOG_E("LB_SYSMSG_GSENSOR");
	}
	return RT_NULL;
}
#endif

/*
 * lombo_rtc_clk_src_set - set the CLK source of RTC
 * @clk_src: 1, LFEOSC; 0, RCOSC
 *
 * LFEOSC is recommended because it is high precision
 */
void lombo_rtc_clk_src_set(u32 clk_src)
{
	u32 clk_src_stat = 0;

	csp_rtc_set_clk_src(clk_src);
	clk_src_stat = csp_rtc_get_clk_src_stat();
	if (clk_src_stat != clk_src)
		LOG_E("cound not set RTC clk source!");
}

/*
 * lombo_rtc_get_clk_src_stat - get source of RTC clock
 * @param: none
 *
 * return: 1, LFEOSC; 0, RCOSC
 */
u32 lombo_rtc_get_clk_src_stat(void)
{
	u32 clk_src_stat = 0;

	clk_src_stat = csp_rtc_get_clk_src_stat();
	return clk_src_stat;
}

/*
 * rt_rtc_clk_src_init - init the RTC CLK source, select LFEOSC
 * @param: none
 *
 * return: RT_EOK
 */
int rt_hw_rtc_clk_src_init(void)
{
	u32 clk_src_stat = 0;
#ifdef ARCH_LOMBO_N7V1_CDR
	rt_err_t ret = 0;
	pthread_t gsensor_det_tid;
#endif

	clk_src_stat = csp_rtc_get_clk_src_stat();
	if (CLK_SRC_LFEOSC != clk_src_stat)
		csp_rtc_set_clk_src(CLK_SRC_LFEOSC);

	/* clear int pending before enable int */
	csp_rtc_ld_int_clr();
	csp_rtc_alarm_int_clr();
	/* enable LFEOSC detect */
	csp_rtc_ld_enable(ENABLE);

#ifdef LOMBO_POWER
	u32 gs_pend;
	u32 rk_pend;

	/* get gsensor interrupt pending status */
	gs_pend = csp_rtc_get_pm_int_pending(PM_TYPE_GS);
	if (gs_pend) {
		power_mark_gs_boot();
		csp_rtc_pm_int_clr(PM_TYPE_GS);
	}

	/* clear the ring key pending if the device was wake up by ring key */
	rk_pend = csp_rtc_get_pm_int_pending(PM_TYPE_RING_KEY);
	if (rk_pend)
		csp_rtc_pm_int_clr(PM_TYPE_RING_KEY);

	/* enable rtc power int */
	enable_rtc_pm_int();
#endif
#ifdef ARCH_LOMBO_N7V1_CDR
	gsensor_det_sem = rt_sem_create("gsensor_det_sem", 0,
			RT_IPC_FLAG_FIFO);
	ret = pthread_create(&gsensor_det_tid, RT_NULL, __gsensor_det_fun, NULL);
	if (ret  < 0) {
		LOG_E("av_det_fun create fail.");
		if (gsensor_det_sem) {
			rt_sem_delete(gsensor_det_sem);
			gsensor_det_sem = RT_NULL;
		}
	}
#endif
	rt_hw_interrupt_install(INT_RTC, rt_rtc_int_isr, RT_NULL, "rtc");
	rt_hw_interrupt_umask(INT_RTC);
	/* enable LFEOSC detect interrupt */
	csp_rtc_ld_int_enable(ENABLE);

	return RT_EOK;
}

/*
 * rt_hw_rtc_get_time - get system time
 * @p_tm: pointer to tm
 *
 */
static void rt_hw_rtc_get_time(struct tm *p_tm)
{
	u32 have_retried = 0;
	reg_rtc_rtc_ymd_t	reg_ymd;
	reg_rtc_rtc_week_t	reg_week;
	reg_rtc_rtc_hms_t	reg_hms;

retry_get_time:
	reg_ymd.val	= csp_rtc_get_ymd();
	reg_week.val	= csp_rtc_get_week();
	reg_hms.val	= csp_rtc_get_hms();

	if ((0 == reg_hms.bits.second) && (have_retried == 0)) {
		have_retried = 1;
		goto retry_get_time;
	}

	p_tm->tm_year	= reg_ymd.bits.year - 1900;
	p_tm->tm_mon	= reg_ymd.bits.month - 1;
	p_tm->tm_mday	= reg_ymd.bits.day;
	p_tm->tm_wday	= reg_week.bits.week;
	p_tm->tm_hour	= reg_hms.bits.hour;
	p_tm->tm_min	= reg_hms.bits.minute;
	p_tm->tm_sec	= reg_hms.bits.second;
	p_tm->tm_yday	= 0;
	p_tm->tm_isdst	= 0;
}

/*
 * rt_hw_rtc_set_time - set system time
 * @p_tm: pointer to tm
 *
 * return: RT_EOK, set succeed; RT_ERROR, set failed
 */
static int rt_hw_rtc_set_time(struct tm *p_tm)
{
	u32 year = 0, month = 0, day = 0;
	u32 week = 0, hour = 0, minute = 0, second = 0;
	int ret = RT_ERROR;
	struct tm ti;

	year	= p_tm->tm_year + 1900;
	month	= p_tm->tm_mon + 1;
	day	= p_tm->tm_mday;
	week	= p_tm->tm_wday;
	hour	= p_tm->tm_hour;
	minute	= p_tm->tm_min;
	second	= p_tm->tm_sec;

	ret = ymd_valid_judge(year, month, day);
	if (RT_EOK != ret)
		goto out;
	ret = hms_valid_judge(hour, minute, second);
	if (RT_EOK != ret)
		goto out;

	do {
		csp_rtc_set_ymd(day, month, year);
		csp_rtc_set_week(week);
		csp_rtc_set_hms(second, minute, hour);
		rt_hw_rtc_get_time(&ti);
	} while ((ti.tm_sec != second) || (ti.tm_mday != day));
out:
	return ret;
}

/*
 * rt_hw_alarm_set - set the time and date of alarm to device
 * @wkalarm: pointer to wkalarm
 *
 * function: set the alarm of most recent to device
 */
static int rt_hw_alarm_set(struct rt_rtc_wkalarm *wkalarm)
{
	u32 hour = 0, minute = 0, second = 0;
	u32 alarm_match_en = 0;
	int ret = RT_ERROR;

	hour	= wkalarm->tm_hour;
	minute	= wkalarm->tm_min;
	second	= wkalarm->tm_sec;

	ret = hms_valid_judge(hour, minute, second);
	if (RT_EOK != ret)
		goto out;

	if (hour < 24 && hour >= 0)
		alarm_match_en |= ALARM_HOUR_MATCH_EN;
	if (minute < 60 && minute >= 0)
		alarm_match_en |= ALARM_MINUTE_MATCH_EN;
	if (second < 60 && second >= 0)
		alarm_match_en |= ALARM_SECOND_MATCH_EN;
	/* enable alarm match */
	csp_rtc_alarm_match_enable(alarm_match_en);
	csp_rtc_set_alarm_hms(hour, minute, second);
	/* enable alarm interrupt */
	csp_rtc_alarm_int_enable(wkalarm->enable);
	/* enable RTC wakeup */
	rt_hw_rtc_wakeup(wkalarm->enable);
out:
	return ret;
}

/*
 * rt_hw_alarm_get - get the time and date of alarm form device
 * @wkalarm: pointer to wkalarm
 *
 */
static void rt_hw_alarm_get(struct rt_rtc_wkalarm *wkalarm)
{
	int hour = -1, minute = -1, second = -1;
	u32 alarm_match_en_stat = 0;
	reg_rtc_rtc_alarm_hms_match_t reg_alm_hms;

	alarm_match_en_stat = csp_rtc_alarm_match_get_enable();
	reg_alm_hms.val = csp_rtc_get_alarm_hms();
	if (alarm_match_en_stat & ALARM_HOUR_MATCH_EN)
		hour = reg_alm_hms.bits.hour_match;
	if (alarm_match_en_stat & ALARM_MINUTE_MATCH_EN)
		minute = reg_alm_hms.bits.minute_match;
	if (alarm_match_en_stat & ALARM_SECOND_MATCH_EN)
		second = reg_alm_hms.bits.second_match;

	wkalarm->tm_hour	= hour;
	wkalarm->tm_min		= minute;
	wkalarm->tm_sec		= second;
}

/*
 * lombo_rtc_lfeosc_fanout - fanout LFEOSC(32768Hz) for peripheral
 * @lfeosc_out_en: 1, enable; 0, disable
 *
 */
void lombo_rtc_lfeosc_fanout(u32 lfeosc_out_en)
{
	u32 clk_src_stat = 0;

	if (ENABLE == lfeosc_out_en) {
		clk_src_stat = csp_rtc_get_clk_src_stat();
		if (CLK_SRC_LFEOSC == clk_src_stat)
			csp_rtc_lfeosc_fanout_enable(ENABLE);
		else {
			LOG_E("No external low-frequency crystal oscillator detected");
			csp_rtc_lfeosc_fanout_enable(DISABLE);
		}
	} else
		csp_rtc_lfeosc_fanout_enable(lfeosc_out_en);
}

/*
 * rt_hw_rtc_control - RTC control
 * @dev: device
 * @cmd: cntrol cmd
 * @args: argument
 *
 */
static rt_err_t rt_hw_rtc_control(rt_device_t dev, int cmd, void *args)
{
	struct tm tm_now, *p_tm = RT_NULL;
	struct rt_rtc_wkalarm get_wkalarm, *p_wkalarm = RT_NULL;
	time_t *timestamp = RT_NULL;
	int ret = RT_ERROR;

	RT_ASSERT(dev != RT_NULL);

	switch (cmd) {
	case RT_DEVICE_CTRL_RTC_GET_TIME:
		memset(&tm_now, 0, sizeof(struct tm));
#ifdef RTC_PCF8563_EXT
        rtc_pcf8563_get_time(&tm_now);
	
#else
       rt_hw_rtc_get_time(&tm_now);
#endif
		*((time_t *)args) = mktime(&tm_now);
		ret = RT_EOK;
		break;
	case RT_DEVICE_CTRL_RTC_SET_TIME:
		timestamp = (time_t *)args;
		p_tm = localtime(timestamp);
#ifdef RTC_PCF8563_EXT
		ret = rtc_pcf8563_set_time(p_tm);
#else
		ret = rt_hw_rtc_set_time(p_tm);
#endif
		break;
	case RT_DEVICE_CTRL_RTC_SET_ALARM:
		p_wkalarm = (struct rt_rtc_wkalarm *)args;
		ret = rt_hw_alarm_set(p_wkalarm);
		break;
	case RT_DEVICE_CTRL_RTC_GET_ALARM:
		memset(&get_wkalarm, 0, sizeof(struct rt_rtc_wkalarm));
		rt_hw_alarm_get(&get_wkalarm);
		*((struct rt_rtc_wkalarm *)args) = get_wkalarm;
		ret = RT_EOK;
		break;
	default:
		goto out;
	}
out:
	return ret;
}

/*
 * rt_hw_rtc_init - RTC init
 * @param: none
 * @return: none
 *
 */
int lombo_rtc_init(void)
{
	struct tm rtc_tm;
	rtc.type = RT_Device_Class_RTC;

#ifdef RTC_PCF8563_EXT
	rtc_pcf8563_setup_i2c_device();
	g_syc_time = rt_tick_get();
#endif

	rt_hw_rtc_clk_src_init();
	/* register rtc device */
	rtc.init	= RT_NULL;
	rtc.open	= rtc_open;
	rtc.close	= rtc_close;
	rtc.read	= RT_NULL;
	rtc.write	= RT_NULL;
	rtc.control	= rt_hw_rtc_control;

	/* no private */
	rtc.user_data = RT_NULL;

	memset(&rtc_tm, 0, sizeof(struct tm));
#ifdef RTC_PCF8563_EXT
	rtc_pcf8563_get_time(&rtc_tm);
#else
    rt_hw_rtc_get_time(&rtc_tm);
#endif
	
	if (rtc_tm.tm_year < 125) {
		rtc_tm.tm_year	= 125;
		rtc_tm.tm_mon	= 0;
		rtc_tm.tm_mday	= 1;
		rtc_tm.tm_hour	= 0;
		rtc_tm.tm_min	= 0;
		rtc_tm.tm_sec	= 0;

#ifdef RTC_PCF8563_EXT
		rtc_pcf8563_set_time(&rtc_tm);
#else
		rt_hw_rtc_set_time(&rtc_tm);
#endif
		LOG_W("Initializing RTC to: %s\n", asctime(&rtc_tm));
	}

	rt_device_register(&rtc, "rtc", RT_DEVICE_FLAG_RDWR);

	return RT_EOK;
}

INIT_COMPONENT_EXPORT(lombo_rtc_init);
INIT_COMPONENT_EXPORT(rt_alarm_system_init);
