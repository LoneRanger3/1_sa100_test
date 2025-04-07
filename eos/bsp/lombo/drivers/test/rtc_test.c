/*
 * rtc_test.c - RTC test driver
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
#include <debug.h>
#include <rtdevice.h>
#include <rthw.h>
#include <rtdef.h>
#include "board.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rtc.h"
#include "rtc_csp.h"

void set_time_test(void)
{
	int set_date_list[][4] = {{2018, 11, 2, RT_EOK},
				{1890, 13, 0, RT_ERROR},
				{1890, 0, 32, RT_ERROR},
				{1890, 1, 1, RT_ERROR},
				{1900, 11, 32, RT_ERROR},
				{1999, 13, 29, RT_ERROR},
				{-1900, -2, -28, RT_ERROR},
				{1900, 2, 29, RT_ERROR},
				{2000, 2, 29, RT_EOK},
				{1908, 2, 28, RT_EOK},
				{2100, 2, 29, RT_ERROR},
				{2004, 2, 29, RT_EOK},
				{2005, 12, 31, RT_EOK},
				{2101, 12, 9, RT_ERROR},
				{2018, 4, 31, RT_ERROR},
				{2018, 1, 31, RT_EOK},
				{2018, 11, 11, RT_EOK},
				{2020, 11, 11, RT_EOK},
				{2037, 12, 31, RT_EOK},
				{2038, 1, 1, RT_EOK},
				{2038, 1, 16, RT_EOK},
				{2038, 1, 17, RT_EOK},
				{2038, 1, 18, RT_EOK},
				{2038, 1, 19, RT_EOK},
				{2039, 11, 11, RT_EOK},
				{2040, 11, 11, RT_EOK},
				{2050, 11, 11, RT_EOK},
				{2080, 11, 11, RT_EOK},
				{1904, 1, 1, RT_EOK},
				{1903, 1, 1, RT_EOK},
				{1902, 1, 1, RT_EOK},
				{1901, 12, 15, RT_EOK},
				{1901, 12, 14, RT_EOK},
				{1901, 12, 13, RT_EOK},
				{1901, 12, 12, RT_EOK},
				{1901, 1, 1, RT_EOK},
				{1900, 1, 1, RT_EOK},
				{1900, 1, 2, RT_EOK},
				{2099, 12, 31, RT_EOK},
				{2100, 12, 30, RT_EOK},
				{2100, 12, 31, RT_EOK},
				{2018, 11, 11, RT_EOK}
				};

	int set_time_list[][4] = {{-1, 8, 59, RT_ERROR},
				{0, 60, 0, RT_ERROR},
				{23, 0, 60, RT_ERROR},
				{24, 50, 59, RT_ERROR},
				{23, 60, 60, RT_ERROR},
				{01, 60, 39, RT_ERROR},
				{0, 0, 0, RT_EOK},
				{23, 0, 0, RT_EOK},
				{23, 59, 59, RT_EOK},
				{0, 59, 59, RT_EOK},
				{1, 1, 1, RT_EOK},
				{2, -2, 0, RT_ERROR}
				};
	int year	= 0;
	int mon		= 0;
	int day		= 0;
	int hour	= 0;
	int min		= 0;
	int sec		= 0;
	int list_num	= 0;
	int i		= 0;
	int ret		= RT_ERROR;
	time_t now;
	struct tm *time_now;

	LOG_I("--set time: 2018-11-11 11:11:11 before test--");
	set_date(2018, 11, 11);
	set_time(11, 11 , 11);
	mdelay(1000);
	now = time(RT_NULL);
	LOG_I("now time:%s", ctime(&now));
	LOG_I("-----start set date test-----");
	list_num = sizeof(set_date_list) / sizeof(set_date_list[0]);
	for (i = 0; i < list_num; i++) {
		LOG_I("the %d times set date test:", i+1);
		year	= set_date_list[i][0];
		mon	= set_date_list[i][1];
		day	= set_date_list[i][2];
		LOG_I("year = %d, month = %d, day = %d", year, mon, day);
		ret = set_date(year, mon, day);
		if (ret == set_date_list[i][3]) {
			mdelay(100);
			now = time(RT_NULL);
			time_now = localtime(&now);

			if ((year == (time_now->tm_year + 1900)) &&
				(mon == (time_now->tm_mon + 1)) &&
				(day == time_now->tm_mday)) {
				LOG_I("now time is: %s", ctime(&now));
				LOG_I("--set date succeed!--");
				LOG_I("-------------------------------\n");
			} else {
				LOG_I("now time is: %s", ctime(&now));
				LOG_I("--set date error!--");
				LOG_I("-------------------------------\n");
			}
		} else {
			LOG_I("ret = %d, set_date_list[%d][3] = %d",
						ret, i, set_date_list[i][3]);
			LOG_E("system test failed!");
		}
		mdelay(1000);
	}

	LOG_I("=====================================");
	LOG_I("------start set time test----------");
	list_num = sizeof(set_time_list) / sizeof(set_time_list[0]);
	for (i = 0; i < list_num; i++) {
		LOG_I("the %d times set time test:", i+1);
		hour	= set_time_list[i][0];
		min	= set_time_list[i][1];
		sec	= set_time_list[i][2];
		ret = set_time(hour, min, sec);
		if (set_time_list[i][3] == ret) {
			mdelay(100);
			now = time(RT_NULL);
			time_now = localtime(&now);

			if ((hour == time_now->tm_hour) && (min == time_now->tm_min)
						&& (sec == time_now->tm_sec)) {
				LOG_I("now time is: %s", ctime(&now));
				LOG_I("--set time succeed!--");
				LOG_I("-------------------------------\n");
			} else {
				LOG_I("now time is: %s", ctime(&now));
				LOG_I("--set time error!--");
				LOG_I("-------------------------------\n");
			}
		} else {
			LOG_I("ret = %d, set_time_list[%d][3] = %d", ret,
							i, set_time_list[i][3]);
			LOG_E("system test failed!");
		}
		mdelay(1000);
	}
}

void set_clk_src_test(void)
{
	int rtc_clk_src = 0;

	rtc_clk_src = csp_rtc_get_clk_src_stat();
	/* CLK is RCOSC */
	if (rtc_clk_src == 0) {
		LOG_I("--the RTC clk src is RCOSC!--");
		LOG_I("--now trun the clk to LFEOSC manually --");
		/* select LFEOSC */
		csp_rtc_set_clk_src(1);
		mdelay(10);
		rtc_clk_src = csp_rtc_get_clk_src_stat();
		if (rtc_clk_src == 1)
			LOG_I("--trun succeed!--");
	}
	/* CLK is LFEOSC */
	if (rtc_clk_src == 1) {
		LOG_I("--the RTC clk src is LFEOSC with power oning!--");
		LOG_I("--now trun to the clk to RCOSC manually--");
		csp_rtc_set_clk_src(0);
		rtc_clk_src = csp_rtc_get_clk_src_stat();
		if (rtc_clk_src == 0)
			LOG_I("trun to RCOSC from LFEOSC succeed!");
		mdelay(10);
		csp_rtc_set_clk_src(1);
		rtc_clk_src = csp_rtc_get_clk_src_stat();
		if (rtc_clk_src == 1)
			LOG_I("trun the clk to LFEOSC and remove LFEOSC.");
		LOG_I("--waiting for LD IRQ--");
		rtc_clk_src = csp_rtc_get_clk_src_stat();
		if (rtc_clk_src == 0)
			LOG_I("IRQ have been runed");
	}
}

void alarm_oneshot_callback(rt_alarm_t alarm, time_t timestamp)
{
	LOG_I("----------------------------------------");
	LOG_I("---alarm onshot is happening ---");
	LOG_I("--alarm flag is: 0x%04x--", alarm->flag & 0xFF00);
	LOG_I("--now time is: %s--", ctime(&timestamp));
	LOG_I("----------------------------------------");
}

void alarm_daily_callback(rt_alarm_t alarm, time_t timestamp)
{
	LOG_I("----------------------------------------");
	LOG_I("--alarm daily is happening---");
	LOG_I("--alarm flag is: 0x%04x--", alarm->flag & 0xFF00);
	LOG_I("--now time is: %s--", ctime(&timestamp));
	LOG_I("-----------------------------------------");
}

void alarm_daily_callback2(rt_alarm_t alarm, time_t timestamp)
{
	LOG_I("----------------------------------------");
	LOG_I("--alarm daily2 is happening---");
	LOG_I("--alarm flag is: 0x%04x--", alarm->flag & 0xFF00);
	LOG_I("--now time is: %s--", ctime(&timestamp));
	LOG_I("-----------------------------------------");
}

void alarm_control_callback(rt_alarm_t alarm, time_t timestamp)
{
	int ret = 0;
	struct rt_alarm_setup setup;

	LOG_I("----------------------------------------");
	if ((alarm->flag & 0xF00) == RT_ALARM_MONTHLY) {
		LOG_I("--alarm control(monthly) is happening---");
		LOG_I("--alarm flag is: 0x%04x--", alarm->flag & 0xFF00);
		LOG_I("--now time is: %s--", ctime(&timestamp));

		memset(&setup, 0, sizeof(struct rt_alarm_setup));
		setup.flag = RT_ALARM_DAILY;
		setup.wktime.tm_hour	= 0;
		setup.wktime.tm_min	= 0;
		setup.wktime.tm_sec	= 15;
		ret = rt_alarm_control(alarm, RT_ALARM_CTRL_MODIFY, &setup);
		if (RT_EOK == ret) {
			LOG_I("modify succeed");
			ret = rt_alarm_start(alarm);
			if (RT_EOK == ret)
				LOG_I("alarm start succeed!");
			else
				LOG_I("alarm start failed!");
		} else
			LOG_I("modify failed");
	} else if ((alarm->flag & 0xF00) == RT_ALARM_DAILY) {
		LOG_I("--alarm control(daily) is happening---");
		LOG_I("--alarm flag is: 0x%04x--", alarm->flag & 0xFF00);
		LOG_I("--now time is: %s--", ctime(&timestamp));

		memset(&setup, 0, sizeof(struct rt_alarm_setup));
		setup.flag = RT_ALARM_MONTHLY;
		setup.wktime.tm_mday	= 11;
		setup.wktime.tm_hour	= 0;
		setup.wktime.tm_min	= 0;
		setup.wktime.tm_sec	= 10;
		ret = rt_alarm_control(alarm, RT_ALARM_CTRL_MODIFY, &setup);
		if (RT_EOK == ret) {
			LOG_I("modify succeed");
			ret = rt_alarm_start(alarm);
			if (RT_EOK == ret)
				LOG_I("alarm start succeed!");
			else
				LOG_I("alarm start failed!");
		} else
			LOG_I("modify failed");
	}
	LOG_I("-----------------------------------------");

}

void alarm_weekly_callback(rt_alarm_t alarm, time_t timestamp)
{
	LOG_I("----------------------------------------");
	LOG_I("--alarm weekly is happening--");
	LOG_I("--alarm flag is: 0x%04x--", alarm->flag & 0xFF00);
	LOG_I("--now time is: %s--", ctime(&timestamp));
	LOG_I("----------------------------------------");
	/*
	LOG_I("--now set time:THR 23:59:54--");

	set_date(2018, 11, 1);
	set_time(9, 59, 54);
	*/
}

void alarm_weekly_callback2(rt_alarm_t alarm, time_t timestamp)
{
	LOG_I("----------------------------------------");
	LOG_I("--alarm weekly2 is happening--");
	LOG_I("--alarm flag is: 0x%04x--", alarm->flag & 0xFF00);
	LOG_I("--now time is: %s--", ctime(&timestamp));
	LOG_I("----------------------------------------");
	/*
	LOG_I("--now set time:THR 23:59:54--");

	set_date(2018, 11, 1);
	set_time(9, 59, 54);
	*/
}

void alarm_monthly_callback(rt_alarm_t alarm, time_t timestamp)
{
	LOG_I("----------------------------------------");
	LOG_I("--alarm monthly is happening--");
	LOG_I("--alarm flag is: 0x%04x--", alarm->flag & 0xFF00);
	LOG_I("--now time is: %s--", ctime(&timestamp));
	LOG_I("----------------------------------------");
	/*
	LOG_I("--now set time:25th 9:59:58--");
	srand(alarm_time);
	while (num == 0)
	{
		num = rand() % 10;
	}

	set_date(2018, 10, 26);
	set_time(9, 59, 58);
	*/
}

void alarm_monthly_callback2(rt_alarm_t alarm, time_t timestamp)
{
	LOG_I("----------------------------------------");
	LOG_I("--alarm monthly2 is happening--");
	LOG_I("--alarm flag is: 0x%04x--", alarm->flag & 0xFF00);
	LOG_I("--now time is: %s--", ctime(&timestamp));
	LOG_I("----------------------------------------");
	/*
	LOG_I("--now set time:25th 9:59:58--");
	srand(alarm_time);
	while (num == 0)
	{
		num = rand() % 10;
	}

	set_date(2018, 10, 26);
	set_time(9, 59, 58);
	*/
}

void alarm_yearly_callback2(rt_alarm_t alarm, time_t timestamp)
{
	LOG_I("----------------------------------------");
	LOG_I("--alarm yearly2 is happening--");
	LOG_I("--alarm flag is: 0x%04x--", alarm->flag & 0xFF00);
	LOG_I("--now time is: %s--", ctime(&timestamp));
	LOG_I("----------------------------------------");
}

void alarm_yearly_callback(rt_alarm_t alarm, time_t timestamp)
{
	time_t now;

	LOG_I("----------------------------------------");
	LOG_I("--alarm yearly is happening--");
	LOG_I("--alarm flag is: 0x%04x--", alarm->flag & 0xFF00);
	LOG_I("--now time is: %s--", ctime(&timestamp));
	LOG_I("----------------------------------------");
	LOG_I("--now set time:11-10 23:59:50--");

	set_date(2018, 11, 10);
	mdelay(1000);
	set_time(23, 59, 50);
	mdelay(100);
	now = time(RT_NULL);
	LOG_I("now time is: %s", ctime(&now));
	LOG_I("----------------------------------------");
}

void alarm_oneshot_test(rt_alarm_callback_t callback, char *cmd)
{
	struct rt_alarm_setup setup;
	rt_alarm_t alarm = RT_NULL;
	int ret = RT_ERROR;

	setup.flag = RT_ALARM_ONESHOT;
	setup.wktime.tm_year	= 2018 - 1900;
	setup.wktime.tm_mon	= 11 - 1;
	setup.wktime.tm_mday	= 10;
	setup.wktime.tm_hour	= 23;
	setup.wktime.tm_min	= 59;
	setup.wktime.tm_sec	= 50;
	mktime(&setup.wktime);

	alarm = rt_alarm_create(callback, &setup);
	if (RT_NULL != alarm)
		LOG_I("--alarm create succeed!--");
	else
		LOG_I("--alarm create failed--");
	if (strcmp(cmd, "start") == 0) {
		ret = rt_alarm_start(alarm);
		if (RT_EOK == ret)
			LOG_I("--alarm start succeed!--");
		else
			LOG_I("--alarm start failed!--");
	}
	if (strcmp(cmd, "stop") == 0) {
		ret = rt_alarm_stop(alarm);
		if (RT_EOK == ret)
			LOG_I("--alarm stop succeed!--");
		else
			LOG_I("--alarm stop failed!--");
	}
}

void alarm_daily_test(rt_alarm_callback_t callback, char *cmd)
{
	struct rt_alarm_setup setup;
	rt_alarm_t alarm = RT_NULL;
	int ret = RT_ERROR;

	memset(&setup, 0, sizeof(struct rt_alarm_setup));
	setup.flag = RT_ALARM_DAILY;
	setup.wktime.tm_hour	= 23;
	setup.wktime.tm_min	= 59;
	setup.wktime.tm_sec	= 55;

	if (strcmp(cmd, "start") == 0) {
		alarm = rt_alarm_create(callback, &setup);
		if (RT_NULL == alarm)
			LOG_I("--alarm create failed--");
		else
			LOG_I("--alarm create succeed!--");
		ret = rt_alarm_start(alarm);
		if (RT_EOK == ret)
			LOG_I("--alarm start succeed!--");
		else
			LOG_I("--alarm start failed!--");
	}
}

void alarm_weekly_test(rt_alarm_callback_t callback, char *cmd)
{
	struct rt_alarm_setup setup;
	rt_alarm_t alarm = RT_NULL;
	int ret = RT_ERROR;

	memset(&setup, 0, sizeof(struct rt_alarm_setup));
	setup.flag = RT_ALARM_WEEKLY;
	setup.wktime.tm_wday	= 0;
	setup.wktime.tm_hour	= 0;
	setup.wktime.tm_min	= 0;
	setup.wktime.tm_sec	= 0;

	if (strcmp(cmd, "start") == 0) {
		alarm = rt_alarm_create(callback, &setup);
		if (RT_NULL == alarm)
			LOG_I("--alarm create failed--");
		else
			LOG_I("--alarm create succeed!--");
		ret = rt_alarm_start(alarm);
		if (RT_EOK == ret)
			LOG_I("--alarm start succeed!--");
		else
			LOG_I("--alarm start failed!--");
	}
	if (strcmp(cmd, "stop") == 0) {
		ret = rt_alarm_stop(alarm);
		if (RT_EOK == ret)
			LOG_I("--alarm stop succeed!--");
		else
			LOG_I("--alarm stop failed!--");
	}
}

void alarm_monthly_test(rt_alarm_callback_t callback, char *cmd)
{
	struct rt_alarm_setup setup;
	rt_alarm_t alarm = RT_NULL;
	int ret = RT_ERROR;

	memset(&setup, 0, sizeof(struct rt_alarm_setup));
	setup.flag = RT_ALARM_MONTHLY;
	setup.wktime.tm_mday	= 11;
	setup.wktime.tm_hour	= 0;
	setup.wktime.tm_min	= 0;
	setup.wktime.tm_sec	= 5;

	if (strcmp(cmd, "start") == 0) {
		alarm = rt_alarm_create(callback, &setup);
		if (alarm ==  RT_NULL)
			LOG_I("alarm create failed!");
		else
			LOG_I("alarm create succeed!");
		ret = rt_alarm_start(alarm);
		if (RT_EOK == ret)
			LOG_I("alarm start succeed!");
		else
			LOG_I("alarm start failed!");
	}
	if (strcmp(cmd, "stop") == 0) {
		ret = rt_alarm_stop(alarm);
		if (RT_EOK == ret)
			LOG_I("alarm stop succeed!");
		else
			LOG_I("alarm stop failed!");
	}
}

void alarm_control_test(rt_alarm_callback_t callback, char *cmd)
{
	struct rt_alarm_setup setup;
	rt_alarm_t alarm = RT_NULL;
	int ret = RT_ERROR;

	memset(&setup, 0, sizeof(struct rt_alarm_setup));
	setup.flag = RT_ALARM_MONTHLY;
	setup.wktime.tm_mday	= 11;
	setup.wktime.tm_hour	= 0;
	setup.wktime.tm_min	= 0;
	setup.wktime.tm_sec	= 10;

	if (strcmp(cmd, "start") == 0) {
		alarm = rt_alarm_create(callback, &setup);
		if (alarm ==  RT_NULL)
			LOG_I("alarm create failed!");
		else
			LOG_I("alarm create succeed!");
		ret = rt_alarm_start(alarm);
		if (RT_EOK == ret)
			LOG_I("alarm start succeed!");
		else
			LOG_I("alarm start failed!");
	}
	if (strcmp(cmd, "stop") == 0) {
		ret = rt_alarm_stop(alarm);
		if (RT_EOK == ret)
			LOG_I("alarm stop succeed!");
		else
			LOG_I("alarm stop failed!");
	}
}

void alarm_daily_test2(rt_alarm_callback_t callback, char *cmd)
{
	struct rt_alarm_setup setup;
	rt_alarm_t alarm = RT_NULL;
	int ret = RT_ERROR;

	memset(&setup, 0, sizeof(struct rt_alarm_setup));
	setup.flag = RT_ALARM_DAILY;
	setup.wktime.tm_hour	= 0;
	setup.wktime.tm_min	= 0;
	setup.wktime.tm_sec	= 20;

	if (strcmp(cmd, "start") == 0) {
		alarm = rt_alarm_create(callback, &setup);
		if (alarm ==  RT_NULL)
			LOG_I("alarm create failed");
		else
			LOG_I("alarm create succeed!");
		ret = rt_alarm_start(alarm);
		if (RT_EOK == ret)
			LOG_I("alarm start succeed!");
		else
			LOG_I("alarm start failed!");
	}
	if (strcmp(cmd, "stop") == 0) {
		ret = rt_alarm_stop(alarm);
		if (RT_EOK == ret)
			LOG_I("alarm stop succeed!");
		else
			LOG_I("alarm stop failed!");
	}
}

void alarm_weekly_test2(rt_alarm_callback_t callback, char *cmd)
{
	struct rt_alarm_setup setup;
	rt_alarm_t alarm = RT_NULL;
	int ret = RT_ERROR;

	memset(&setup, 0, sizeof(struct rt_alarm_setup));
	setup.flag = RT_ALARM_WEEKLY;
	setup.wktime.tm_wday	= 0;
	setup.wktime.tm_hour	= 0;
	setup.wktime.tm_min	= 0;
	setup.wktime.tm_sec	= 25;

	if (strcmp(cmd, "start") == 0) {
		alarm = rt_alarm_create(callback, &setup);
		if (alarm ==  RT_NULL)
			LOG_I("alarm create failed");
		else
			LOG_I("alarm create succeed!");
		ret = rt_alarm_start(alarm);
		if (RT_EOK == ret)
			LOG_I("alarm start succeed!");
		else
			LOG_I("alarm start failed!");
	}
	if (strcmp(cmd, "stop") == 0) {
		ret = rt_alarm_stop(alarm);
		if (RT_EOK == ret)
			LOG_I("alarm stop succeed!");
		else
			LOG_I("alarm stop failed!");
	}
}

void alarm_monthly_test2(rt_alarm_callback_t callback, char *cmd)
{
	struct rt_alarm_setup setup;
	rt_alarm_t alarm = RT_NULL;
	int ret = RT_ERROR;

	memset(&setup, 0, sizeof(struct rt_alarm_setup));
	setup.flag = RT_ALARM_MONTHLY;
	setup.wktime.tm_mday	= 11;
	setup.wktime.tm_hour	= 0;
	setup.wktime.tm_min	= 0;
	setup.wktime.tm_sec	= 30;

	if (strcmp(cmd, "start") == 0) {
		alarm = rt_alarm_create(callback, &setup);
		if (alarm ==  RT_NULL)
			LOG_I("alarm create failed!");
		else
			LOG_I("alarm create succeed!");
		ret = rt_alarm_start(alarm);
		if (RT_EOK == ret)
			LOG_I("alarm start succeed!");
		else
			LOG_I("alarm start failed!");
	}
	if (strcmp(cmd, "stop") == 0) {
		ret = rt_alarm_stop(alarm);
		if (RT_EOK == ret)
			LOG_I("alarm stop succeed!");
		else
			LOG_I("alarm stop failed!");
	}
}

void alarm_yearly_test2(rt_alarm_callback_t callback, char *cmd)
{
	struct rt_alarm_setup setup;
	rt_alarm_t alarm = RT_NULL;
	int ret = RT_ERROR;

	memset(&setup, 0, sizeof(struct rt_alarm_setup));
	setup.flag = RT_ALARM_YAERLY;
	setup.wktime.tm_mon	= 11 - 1;
	setup.wktime.tm_mday	= 11;
	setup.wktime.tm_hour	= 0;
	setup.wktime.tm_min	= 0;
	setup.wktime.tm_sec	= 35;
	mktime(&setup.wktime);

	if (strcmp(cmd, "start") == 0) {
		alarm = rt_alarm_create(callback, &setup);
		if (alarm ==  RT_NULL)
			LOG_I("alarm create failed");
		else
			LOG_I("alarm create succeed!");
		ret = rt_alarm_start(alarm);
		if (RT_EOK == ret)
			LOG_I("alarm start succeed!");
		else
			LOG_I("alarm start failed!");
	}
	if (strcmp(cmd, "stop") == 0) {
		ret = rt_alarm_stop(alarm);
		if (RT_EOK == ret)
			LOG_I("alarm stop succeed!");
		else
			LOG_I("alarm stop failed!");
	}
}

void alarm_yearly_test(rt_alarm_callback_t callback, char *cmd)
{
	struct rt_alarm_setup setup;
	rt_alarm_t alarm = RT_NULL;
	int ret = RT_ERROR;

	memset(&setup, 0, sizeof(struct rt_alarm_setup));
	setup.flag = RT_ALARM_YAERLY;
	setup.wktime.tm_mon	= 11 - 1;
	setup.wktime.tm_mday	= 11;
	setup.wktime.tm_hour	= 0;
	setup.wktime.tm_min	= 0;
	setup.wktime.tm_sec	= 40;
	mktime(&setup.wktime);

	if (strcmp(cmd, "start") == 0) {
		alarm = rt_alarm_create(callback, &setup);
		if (alarm ==  RT_NULL)
			LOG_I("alarm create failed");
		else
			LOG_I("alarm create succeed!");
		ret = rt_alarm_start(alarm);
		if (RT_EOK == ret)
			LOG_I("alarm start succeed!");
		else
			LOG_I("alarm start failed!");
	}
	if (strcmp(cmd, "stop") == 0) {
		ret = rt_alarm_stop(alarm);
		if (RT_EOK == ret)
			LOG_I("alarm stop succeed!");
		else
			LOG_I("alarm stop failed!");
	}
}

void get_time(void)
{
	time_t now;

	now = time(RT_NULL);
	LOG_I("now time is: %s", ctime(&now));
}

void alarm_test_callback(rt_alarm_t alarm, time_t timestamp)
{
	u32 alarm_mode = 0;

	LOG_I("----------------------------------------");
	LOG_I("--alarm test is happening---");
	alarm_mode = alarm->flag & 0xFF00;
	switch (alarm_mode) {
	case RT_ALARM_ONESHOT:
		LOG_I("--alarm flag is: ONESHOT ---");
		break;
	case RT_ALARM_DAILY:
		LOG_I("--alarm flag is: DAILY ---");
		break;
	case RT_ALARM_WEEKLY:
		LOG_I("--alarm flag is: WEEKLY --");
		break;
	case RT_ALARM_MONTHLY:
		LOG_I("--alarm flag is: MONTHLY --");
		break;
	case RT_ALARM_YAERLY:
		LOG_I("--alarm flag is: YAERLY --");
		break;
	default:
		LOG_I("--alarm flag error---");
		break;
	}
	LOG_I("--now time is: %s--", ctime(&timestamp));
	LOG_I("-----------------------------------------");
}

void alarm_test(rt_alarm_callback_t callback, struct rt_alarm_setup *setup)
{
	rt_alarm_t alarm = RT_NULL;
	int ret = RT_ERROR;

	alarm = rt_alarm_create(callback, setup);
	if (alarm ==  RT_NULL)
		LOG_I("--alarm create failed--");
	else
		LOG_I("--alarm create succeed!--");
	ret = rt_alarm_start(alarm);
	if (RT_EOK == ret)
		LOG_I("--alarm start succeed!--");
	else
		LOG_I("--alarm start failed!--");
}

long test_rtc(int argc, char **argv)
{
	time_t timestamp;
	char obj[20] = {0}, cmd[10] = {0};
	char time_data[12] = {0};
	char *t = RT_NULL;
	u32 year = 0, mon = 0, day = 0;
	u32 hour = 0, min = 0, sec = 0;
	struct rt_alarm_setup alarm_setup;
	int ret = RT_ERROR;

	if (RT_NULL != argv[2])
		sprintf(obj, "%s", argv[2]);

	if (strcmp(obj, "clk") == 0) {
		LOG_I("===rtc test start===");
		LOG_I("==rtc clk src test start===");
		set_clk_src_test();
		LOG_I("==rtc clk src test stop===");
	}

	if (strcmp(obj, "set_time") == 0) {
		if (argv[3] == RT_NULL) {
			LOG_E("no time input");
			return RT_ERROR;
		}
		sprintf(time_data, "%s", argv[3]);
		t = strtok(time_data, ",");
		if (RT_NULL != t) {
			hour = atoi(t);
			t = strtok(RT_NULL, ",");
			if (RT_NULL != t) {
				min = atoi(t);
				t = strtok(RT_NULL, ",");
				if (RT_NULL != t)
					sec = atoi(t);
				else
					LOG_E("no sec data");
			} else
				LOG_E("no min data");
		} else
			LOG_E("no hour data");
		ret = set_time(hour, min, sec);
		if (ret == RT_ERROR)
			LOG_E("set time failed");
		mdelay(100);
		get_time();
	}

	if (strcmp(obj, "set_date") == 0) {
		if (argv[3] == RT_NULL) {
			LOG_E("no time input");
			return RT_ERROR;
		}
		sprintf(time_data, "%s", argv[3]);
		t = strtok(time_data, ",");
		if (RT_NULL != t) {
			year = atoi(t);
			t = strtok(RT_NULL, ",");
			if (RT_NULL != t) {
				mon = atoi(t);
				t = strtok(RT_NULL, ",");
				if (RT_NULL != t) {
					day = atoi(t);
				} else {
					LOG_E("no day date");
					return RT_ERROR;
				}
			} else {
				LOG_E("no mon data");
				return RT_ERROR;
			}
		} else {
			LOG_E("no year data");
			return RT_ERROR;
		}
		ret = set_date(year, mon, day);
		if (ret == RT_ERROR)
			LOG_E("set date failed");
		mdelay(100);
		get_time();
	}

	if (strcmp(obj, "get_time") == 0)
		get_time();

	if (strcmp(obj, "alarm") == 0) {
		memset(&alarm_setup, 0, sizeof(struct rt_alarm_setup));
		if (argv[4] == RT_NULL) {
			LOG_E("no input");
			return RT_ERROR;
		}
		sprintf(time_data, "%s", argv[4]);
		if (strcmp(argv[3], "oneshot") == 0) {
			alarm_setup.flag = RT_ALARM_ONESHOT;
			t = strtok(time_data, ",");
			if (RT_NULL != t) {
				alarm_setup.wktime.tm_year = atoi(t) - 1900;
				t = strtok(RT_NULL, ",");
				if (RT_NULL != t) {
					alarm_setup.wktime.tm_mon = atoi(t) - 1;
					t = strtok(RT_NULL, ",");
					if (RT_NULL != t)
						alarm_setup.wktime.tm_mday = atoi(t);
					else {
						LOG_E("no day");
						return RT_ERROR;
					}
				} else {
					LOG_E("no month");
					return RT_ERROR;
				}
			} else {
				LOG_E("no year");
				return RT_ERROR;
			}
			t = strtok(RT_NULL, ",");
			if (t != RT_NULL) {
				alarm_setup.wktime.tm_hour = atoi(t);
				t = strtok(RT_NULL, ",");
				if (t != RT_NULL) {
					alarm_setup.wktime.tm_min = atoi(t);
					t = strtok(RT_NULL, ",");
					if (t != RT_NULL)
						alarm_setup.wktime.tm_sec = atoi(t);
					else {
						LOG_E("no sec");
						return RT_ERROR;
					}
				} else {
					LOG_E("no min");
					return RT_ERROR;
				}
			} else {
				LOG_E("no hour");
				return RT_ERROR;
			}
		} else if (strcmp(argv[3], "daily") == 0) {
			alarm_setup.flag = RT_ALARM_DAILY;
			t = strtok(time_data, ",");
			if (t != RT_NULL) {
				alarm_setup.wktime.tm_hour = atoi(t);
				t = strtok(RT_NULL, ",");
				if (t != RT_NULL) {
					alarm_setup.wktime.tm_min = atoi(t);
					t = strtok(RT_NULL, ",");
					if (t != RT_NULL)
						alarm_setup.wktime.tm_sec = atoi(t);
					else {
						LOG_E("no sec");
						return RT_ERROR;
					}
				} else {
					LOG_E("no min");
					return RT_ERROR;
				}
			} else {
				LOG_E("no hour");
				return RT_ERROR;
			}
		} else if (strcmp(argv[3], "weekly") == 0) {
			alarm_setup.flag = RT_ALARM_WEEKLY;
			t = strtok(time_data, ",");
			if (RT_NULL != t)
				alarm_setup.wktime.tm_wday = atoi(t);
			else {
				LOG_E("no week");
				return RT_ERROR;
			}
			t = strtok(RT_NULL, ",");
			if (t != RT_NULL) {
				alarm_setup.wktime.tm_hour = atoi(t);
				t = strtok(RT_NULL, ",");
				if (t != RT_NULL) {
					alarm_setup.wktime.tm_min = atoi(t);
					t = strtok(RT_NULL, ",");
					if (t != RT_NULL)
						alarm_setup.wktime.tm_sec = atoi(t);
					else {
						LOG_E("no sec");
						return RT_ERROR;
					}
				} else {
					LOG_E("no min");
					return RT_ERROR;
				}
			} else {
				LOG_E("no hour");
				return RT_ERROR;
			}
		} else if (strcmp(argv[3], "monthly") == 0) {
			alarm_setup.flag = RT_ALARM_MONTHLY;
			t = strtok(time_data, ",");
			if (RT_NULL != t)
				alarm_setup.wktime.tm_mday = atoi(t);
			else {
				LOG_E("no day");
				return RT_ERROR;
			}
			t = strtok(RT_NULL, ",");
			if (t != RT_NULL) {
				alarm_setup.wktime.tm_hour = atoi(t);
				t = strtok(RT_NULL, ",");
				if (t != RT_NULL) {
					alarm_setup.wktime.tm_min = atoi(t);
					t = strtok(RT_NULL, ",");
					if (t != RT_NULL)
						alarm_setup.wktime.tm_sec = atoi(t);
					else {
						LOG_E("no sec");
						return RT_ERROR;
					}
				} else {
					LOG_E("no min");
					return RT_ERROR;
				}
			} else {
				LOG_E("no hour");
				return RT_ERROR;
			}
		} else if (strcmp(argv[3], "yaerly") == 0) {
			alarm_setup.flag = RT_ALARM_YAERLY;
			t = strtok(time_data, ",");
			if (RT_NULL != t) {
				alarm_setup.wktime.tm_mon = atoi(t) - 1;
				t = strtok(RT_NULL, ",");
				if (RT_NULL != t)
					alarm_setup.wktime.tm_mday = atoi(t);
				else {
					LOG_E("no day");
					return RT_ERROR;
				}
			} else {
				LOG_E("no month");
				return RT_ERROR;
			}
			t = strtok(RT_NULL, ",");
			if (t != RT_NULL) {
				alarm_setup.wktime.tm_hour = atoi(t);
				t = strtok(RT_NULL, ",");
				if (t != RT_NULL) {
					alarm_setup.wktime.tm_min = atoi(t);
					t = strtok(RT_NULL, ",");
					if (t != RT_NULL)
						alarm_setup.wktime.tm_sec = atoi(t);
					else {
						LOG_E("no sec");
						return RT_ERROR;
					}
				} else {
					LOG_E("no min");
					return RT_ERROR;
				}
			} else {
				LOG_E("no hour");
				return RT_ERROR;
			}
		}
		alarm_test(alarm_test_callback, &alarm_setup);
	}


	if (strcmp(obj, "time") == 0) {
		LOG_I("==rtc set time test start===");
		set_time_test();
		LOG_I("==rtc set time test stop===");
	}
	if (strcmp(obj, "fanout") == 0) {
		if (RT_NULL != argv[3])
			sprintf(cmd, "%s", argv[3]);
		LOG_I("fanout 32k CLK");
		if (strcmp(cmd, "en") == 0) {
			LOG_I("enable lfeosc fanout");
			lombo_rtc_lfeosc_fanout(1);
		}
		if (strcmp(cmd, "dis") == 0) {
			LOG_I("disable lfeosc fanout");
			lombo_rtc_lfeosc_fanout(0);
		}
	}

	if ((strcmp(obj, "alarm_oneshot") == 0) || (strcmp(obj, "all") == 0)) {
		LOG_I("===set time before alarm===");
		LOG_I("==set time to device:2018-11-10 23:59:35==");
		set_date(2018, 11, 10);
		set_time(23, 59, 35);
		mdelay(1000);
		LOG_I("===set time end===");
		LOG_I("==now time is:===");
		timestamp = time(RT_NULL);
		LOG_I("%s", ctime(&timestamp));
		LOG_I("==== test alarm once====");
		alarm_oneshot_test(alarm_oneshot_callback, "start");
		LOG_I("===delay 1s===");
		mdelay(1000);
		LOG_I("===get now time===");
		timestamp = time(RT_NULL);
		LOG_I("%s", ctime(&timestamp));
		LOG_I("====test alarm oneshot end!===");
	}

	if ((strcmp(obj, "alarm_control") == 0) || (strcmp(obj, "all") == 0)) {
		LOG_I("=== test alarm control===");
		alarm_control_test(alarm_control_callback, "start");
		LOG_I("===test alarm control end!===");
	}

	if ((strcmp(obj, "alarm_weekly") == 0) || (strcmp(obj, "all") == 0)) {
		LOG_I("=== test alarm weekly===");
		alarm_weekly_test(alarm_weekly_callback, "start");
		LOG_I("===test alarm weekly end!===");
	}

	if ((strcmp(obj, "alarm_weekly2") == 0) || (strcmp(obj, "all") == 0)) {
		LOG_I("=== test alarm weekly2===");
		alarm_weekly_test2(alarm_weekly_callback2, "start");
		LOG_I("===test alarm weekly2 end!===");
	}

	if ((strcmp(obj, "alarm_daily") == 0) || (strcmp(obj, "all") == 0)) {
		LOG_I("====test alarm daily===");
		alarm_daily_test(alarm_daily_callback, "start");
		LOG_I("===test alarm daily end!===");
	}

	if ((strcmp(obj, "alarm_daily2") == 0) || (strcmp(obj, "all") == 0)) {
		LOG_I("====test alarm daily2===");
		alarm_daily_test2(alarm_daily_callback2, "start");
		LOG_I("===test alarm daily2 end!===");
	}

	if ((strcmp(obj, "alarm_monthly") == 0) || (strcmp(obj, "all") == 0)) {
		LOG_I("=== test alarm monthly===");
		alarm_monthly_test(alarm_monthly_callback, "start");
		LOG_I("===test alarm monthly end!===");
	}

	if ((strcmp(obj, "alarm_monthly2") == 0) || (strcmp(obj, "all") == 0)) {
		LOG_I("=== test alarm monthly2===");
		alarm_monthly_test2(alarm_monthly_callback2, "start");
		LOG_I("===test alarm monthly2 end!===");
	}

	if ((strcmp(obj, "alarm_yearly") == 0) || (strcmp(obj, "all") == 0)) {
		LOG_I("=== test alarm yearly===");
		alarm_yearly_test(alarm_yearly_callback, "start");
		LOG_I("===test alarm yearly end!===");
	}

	if ((strcmp(obj, "alarm_yearly2") == 0) || (strcmp(obj, "all") == 0)) {
		LOG_I("=== test alarm yearly2===");
		alarm_yearly_test2(alarm_yearly_callback2, "start");
		LOG_I("===test alarm yearly2 end!===");
	}

	return RT_EOK;
}
