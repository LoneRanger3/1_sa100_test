/*
 * gsensor_test.c - G-sensor test driver
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
#include "input/da380_gsensor.h"

#define TEST_GSENSOR_SLEEP		"sleep"
#define TEST_GSENSOR_ADD		"add"
#define TEST_GSENSOR_REMOVE		"remove"
#define TEST_GSENSOR_OPEN		"open"
#define TEST_GSENSOR_CLOSE		"close"
#define TEST_GSENSOR_PARK_OPEN		"park_open"
#define TEST_GSENSOR_PARK_CLOSE		"park_close"
#define TEST_GSENSOR_DUMP		"dump"

static rt_thread_t gs_thread = RT_NULL;

static void gs_proc_thread(void *param)
{
	 while (1) {
		test_gsensor_acc_data();
		rt_thread_mdelay(2);
	 }
}

static void test_gsensor_add()
{
	if (gs_thread == RT_NULL) {
		gs_thread = rt_thread_create("gsensor_thread",
			gs_proc_thread, RT_NULL, 2048, 20, 5);
		if (gs_thread != RT_NULL) {
			rt_thread_startup(gs_thread);
			rt_kprintf("start a thread to monitor gsensor\n");
		} else
			LOG_E("rt_thread_create error");
	} else
		rt_kprintf("Gsensor monitor thread already added\n");
}

static void test_gsensor_remove()
{
	if (gs_thread == RT_NULL)
		rt_kprintf("You should input \"add\" command first\n");
	else {
		rt_thread_delete(gs_thread);
		gs_thread = RT_NULL;
		rt_kprintf("Remove the gsensor monitor thread\n");
	}
}

static void test_gsensor_open()
{
	rt_kprintf("gsensor_open_monitor, now shake the device and see log.\n");
	gsensor_open_monitor();
}

static void test_gsensor_close()
{
	rt_kprintf("test_gsensor_close...\n");
	gsensor_close_monitor();
}

static void test_gsensor_park_open()
{
	int val;

	rt_kprintf("test_gsensor_park_open\n");
	gsensor_set_park_monitor_cfg(RT_TRUE);

	val = gsensor_get_park_monitor_cfg();
	if (val != 1)
		LOG_E("gsensor_get_park_monitor_cfg return value: %d, != 1", val);
}

static void test_gsensor_park_close()
{
	int val;

	rt_kprintf("test_gsensor_park_close\n");
	gsensor_set_park_monitor_cfg(RT_FALSE);

	val = gsensor_get_park_monitor_cfg();
	if (val != 0)
		LOG_E("gsensor_get_park_monitor_cfg return value: %d, != 0", val);
}

static void test_gsensor_dump()
{
	gsensor_dump();
}

static int first = 1;
static void test_gsensor_sleep()
{
	LOG_D("try to sleep for test gsensor wakeup");

	/* copy from pm_test, make device sleep for gsensor wakeup test */
	LOG_D("start");

	if (!first) { /* the first test after boot? */
		LOG_I("before request PM_SLEEP_MODE_DEFAULT");
		rt_pm_request(PM_SLEEP_MODE_DEFAULT);
		LOG_I("after request PM_SLEEP_MODE_DEFAULT");

		/* to see the system entering PM_SLEEP_MODE_DEFAULT */
		rt_thread_mdelay(1);

		LOG_I("before release PM_SLEEP_MODE_DEFAULT");
		rt_pm_release(PM_SLEEP_MODE_DEFAULT);
		LOG_I("after release PM_SLEEP_MODE_DEFAULT");

		return;
	} else
		first = 0;

	LOG_D("before release PM_RUN_MODE_DEFAULT");

	/*
	 * PM_RUN_MODE_DEFAULT, PM_SLEEP_MODE_DEFAULT, PM_MODE_MAX were requested
	 * by rt_system_pm_init, which prevent the system entering low power mode,
	 * so we release them to let rt_pm_enter enter low power state
	 */
	rt_pm_release(PM_RUN_MODE_DEFAULT);

	LOG_D("after release PM_RUN_MODE_DEFAULT");

	/* to see the system entering PM_SLEEP_MODE_DEFAULT */
	rt_thread_mdelay(1);

	LOG_D("before release PM_SLEEP_MODE_DEFAULT");

	rt_pm_release(PM_SLEEP_MODE_DEFAULT);

	LOG_D("after release PM_SLEEP_MODE_DEFAULT");

	/* to see the system entering PM_MODE_MAX */
	rt_thread_mdelay(1);

	LOG_D("before release PM_MODE_MAX");

	rt_pm_release(PM_MODE_MAX);

	LOG_D("after release PM_MODE_MAX");

	/* now the system in which mode? none */
	rt_thread_mdelay(1);

	LOG_D("end");
}

long test_gsensor(int argc, char **argv)
{
	LOG_D("test_gsensor...");
	if (3 == argc) {
		/* gsensor test cmd */
		char *cmd;

		cmd = argv[2];
		if (!strcmp(cmd, TEST_GSENSOR_SLEEP))
			test_gsensor_sleep();
		else if (!strcmp(cmd, TEST_GSENSOR_ADD))
			test_gsensor_add();
		else if (!strcmp(cmd, TEST_GSENSOR_REMOVE))
			test_gsensor_remove();
		else if (!strcmp(cmd, TEST_GSENSOR_OPEN))
			test_gsensor_open();
		else if (!strcmp(cmd, TEST_GSENSOR_CLOSE))
			test_gsensor_close();
		else if (!strcmp(cmd, TEST_GSENSOR_PARK_OPEN))
			test_gsensor_park_open();
		else if (!strcmp(cmd, TEST_GSENSOR_PARK_CLOSE))
			test_gsensor_park_close();
		else if (!strcmp(cmd, TEST_GSENSOR_DUMP))
			test_gsensor_dump();
		else
			LOG_D("invalid cmd\n");
	} else {
		/* test case function */
		LOG_D("test_gsensor success");
	}

	return 0;
}
