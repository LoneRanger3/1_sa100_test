/*
 * test_pm.c - pm test driver
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

#include <board.h>
#include <rtthread.h>
#include <rtdevice.h>

#define TEST_STANDBY
/* #define TEST_SHUTDOWN */

void test_standby(void)
{
	/* NOTE: current mode is PM_MODE_RUNNING, set in rt_system_pm_init */

	LOG_I("start");

	/*
	 * enter standby flow
	 */

	/* request standby mode: PM_MODE_STANDBY */
	LOG_I("before request PM_MODE_STANDBY");
	rt_pm_request(PM_MODE_STANDBY);
	LOG_I("after request PM_MODE_STANDBY");

	/* the system still in PM_MODE_RUNNING */
	rt_thread_mdelay(1);

	/* release current PM_MODE_RUNNING */
	LOG_I("before release PM_MODE_RUNNING");
	rt_pm_release(PM_MODE_RUNNING);
	LOG_I("after release PM_MODE_RUNNING");

	/* the system enter PM_MODE_STANDBY (standby).. */
	rt_thread_mdelay(1);

	/*
	 * recover from standby flow
	 */

	/* recover step1: request PM_MODE_RUNNING mode */
	LOG_I("before request PM_MODE_RUNNING");
	rt_pm_request(PM_MODE_RUNNING);
	LOG_I("after request PM_MODE_RUNNING");

	/* the system enter PM_MODE_RUNNING (normal).. */
	rt_thread_mdelay(1);

	/* recover step2: release PM_MODE_STANDBY */
	LOG_I("before release PM_MODE_STANDBY");
	rt_pm_release(PM_MODE_STANDBY);
	LOG_I("after release PM_MODE_STANDBY");

	/* the system still in PM_MODE_RUNNING */
	rt_thread_mdelay(1);

	LOG_I("end");
}

void test_shutdown(void)
{
	/* NOTE: current mode is PM_MODE_RUNNING, set in rt_system_pm_init */

	LOG_I("start");

	/*
	 * enter shutdwon flow
	 */

	/* request shutdwon mode: PM_MODE_SHUTDOWN */
	LOG_I("before request PM_MODE_SHUTDOWN");
	rt_pm_request(PM_MODE_SHUTDOWN);
	LOG_I("after request PM_MODE_SHUTDOWN");

	/* the system still in PM_MODE_RUNNING */
	rt_thread_mdelay(1);

	/* release current PM_MODE_RUNNING */
	LOG_I("before release PM_MODE_RUNNING");
	rt_pm_release(PM_MODE_RUNNING);
	LOG_I("after release PM_MODE_RUNNING");

	/* the system enter PM_MODE_SHUTDOWN (shutdwon).. */
	rt_thread_mdelay(10);

	LOG_E("shutdown failed, never run here!");
}

/**
 * test_pm - test for power management
 *
 * return 0 if success, -RT_ERROR if failed
 */
long test_pm(int argc, char **argv)
{
	LOG_I("start");

	/* standby test: cpu wfi, ddr self-refresh */
#ifdef TEST_STANDBY
	rt_pm_standby();
/*	test_standby(); */
#endif

	/* shutdown test: cpu and system all power off */
#ifdef TEST_SHUTDOWN
	rt_pm_shutdown();
/*	test_shutdown(); */
#endif

	LOG_I("end");
	return 0;
}

