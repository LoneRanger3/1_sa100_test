/*
 * wdt_test.c - watchdog test driver
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
#define DBG_LEVEL               DBG_LOG
#include <debug.h>
#include "board.h"
#include "wdog/wdog.h"
#include <drivers/watchdog.h>

/* watchdog restart function verify */
rt_err_t wdog_restart_test(void)
{
	rt_device_t dev;
	rt_err_t ret = RT_EOK;
	rt_uint16_t oflag = RT_DEVICE_OFLAG_RDWR;
	int period = 1000; /* time out period: ms */
	int var = 0;

	/* dump_wdog(DUMP_TYPE_READ); */
	LOG_I("Watchdog restart test");

	/* Find the device */
	dev = rt_device_find(WDOG_DEV_NAME);
	if (dev == RT_NULL) {
		LOG_E("%s not found", WDOG_DEV_NAME);
		return -RT_ERROR;
	}

	/* Open the device */
	ret = rt_device_open(dev, oflag);
	if (ret != RT_EOK) {
		LOG_E("Failed to open dev:%s with flag:%d", WDOG_DEV_NAME, oflag);
		return ret;
	}

	/* set wdog time out period*/
	rt_device_control(dev, RT_DEVICE_CTRL_WDT_SET_TIMEOUT, &period);

	/* enable wdog */
	rt_device_control(dev, RT_DEVICE_CTRL_WDT_START, NULL);

	for (var = 0; var < 10; ++var) {
		mdelay(200);
		LOG_D("watchdog wdog_restart[%d]", var);
		rt_device_control(dev, RT_DEVICE_CTRL_WDT_KEEPALIVE, NULL);
	}

	/* disenable wdog */
	rt_device_control(dev, RT_DEVICE_CTRL_WDT_STOP, NULL);

	/* Close the device */
	ret = rt_device_close(dev);
	if (ret != RT_EOK) {
		LOG_E("Failed to close dev:%s", WDOG_DEV_NAME);
		return ret;
	}

	/* dump_wdog(DUMP_TYPE_READ); */
	return ret;
}

/* watchdog reset system verify */
rt_err_t wdog_reset_test(void)
{
	rt_device_t dev;
	rt_err_t ret = RT_EOK;
	rt_uint16_t oflag = RT_DEVICE_OFLAG_RDWR;
	int var = 0;

	/* dump_wdog(DUMP_TYPE_READ); */
	LOG_I("Watchdog reset test");

	/* Find the device */
	dev = rt_device_find(WDOG_DEV_NAME);
	if (dev == RT_NULL) {
		LOG_E("%s not found", WDOG_DEV_NAME);
		return -RT_ERROR;
	}

	/* Open the device */
	ret = rt_device_open(dev, oflag);
	if (ret != RT_EOK) {
		LOG_E("Failed to open dev:%s with flag:%d", WDOG_DEV_NAME, oflag);
		return ret;
	}

	/* enable wdog */
	rt_device_control(dev, RT_DEVICE_CTRL_WDT_START, NULL);

	for (var = 0; var < 10; ++var) {
		mdelay(200);
		LOG_D("watchdog reset wait[%d]", var);
	}

	/* disenable wdog */
	rt_device_control(dev, RT_DEVICE_CTRL_WDT_STOP, NULL);

	/* Close the device */
	ret = rt_device_close(dev);
	if (ret != RT_EOK) {
		LOG_E("Failed to close dev:%s", WDOG_DEV_NAME);
		return ret;
	}

	/* dump_wdog(DUMP_TYPE_READ); */
	return ret;
}

/* test_wdt - test for watchdog */
long test_wdog(int argc, char **argv)
{
	rt_err_t ret = RT_EOK;

	LOG_I("start watchdog test.");
	ret = wdog_restart_test();
	LOG_I("Watchdog restart test finish! >> %d", ret);

	ret = wdog_reset_test();
	LOG_I("Watchdog reset test finish! >> %d", ret);

	LOG_I("end watchdog test.");
	return 0;
}
