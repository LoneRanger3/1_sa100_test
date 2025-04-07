/*
 * timer_test.c - timer test driver
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
#include <eos.h>
#include "board.h"

/**
 * test_timer - test for timer
 *
 * return 0 if success, -RT_ERROR if failed
 */
long test_timer(int argc, char **argv)
{
	int delay_ms[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 200, 300,
		400, 500, 600, 700, 800, 900, 1000, 2000, 3000};
	int tick[2], i;
	u32 msec[2];
	u64 nsec[2];

	LOG_I("start");

	LOG_I("before delay 1us!");
	nsec[0] = rt_time_get_nsec();
	msec[0] = rt_time_get_msec();
	udelay(1);
	nsec[1] = rt_time_get_nsec();
	msec[1] = rt_time_get_msec();
	LOG_I("delay 1us end! last %d nsec, %d msec",
		(int)(nsec[1] - nsec[0]),
		(int)(msec[1] - msec[0]));

	LOG_I("before delay 10us!");
	nsec[0] = rt_time_get_nsec();
	msec[0] = rt_time_get_msec();
	udelay(10);
	nsec[1] = rt_time_get_nsec();
	msec[1] = rt_time_get_msec();
	LOG_I("delay 10us end! last %d nsec, %d msec",
		(int)(nsec[1] - nsec[0]),
		(int)(msec[1] - msec[0]));

	LOG_I("before delay 37us!");
	nsec[0] = rt_time_get_nsec();
	msec[0] = rt_time_get_msec();
	udelay(37);
	nsec[1] = rt_time_get_nsec();
	msec[1] = rt_time_get_msec();
	LOG_I("delay 37us end! last %d nsec, %d msec",
		(int)(nsec[1] - nsec[0]),
		(int)(msec[1] - msec[0]));

	LOG_I("before delay 100us!");
	nsec[0] = rt_time_get_nsec();
	msec[0] = rt_time_get_msec();
	udelay(100);
	nsec[1] = rt_time_get_nsec();
	msec[1] = rt_time_get_msec();
	LOG_I("delay 100us end! last %d nsec, %d msec",
		(int)(nsec[1] - nsec[0]),
		(int)(msec[1] - msec[0]));

	LOG_I("before delay 1ms!");
	nsec[0] = rt_time_get_nsec();
	msec[0] = rt_time_get_msec();
	mdelay(1);
	nsec[1] = rt_time_get_nsec();
	msec[1] = rt_time_get_msec();
	LOG_I("delay 1ms end! last %d nsec, %d msec",
		(int)(nsec[1] - nsec[0]),
		(int)(msec[1] - msec[0]));

	LOG_I("before delay 10ms!");
	nsec[0] = rt_time_get_nsec();
	msec[0] = rt_time_get_msec();
	mdelay(10);
	nsec[1] = rt_time_get_nsec();
	msec[1] = rt_time_get_msec();
	LOG_I("delay 10ms end! last %d nsec, %d msec",
		(int)(nsec[1] - nsec[0]),
		(int)(msec[1] - msec[0]));

	LOG_I("before delay 100ms!");
	nsec[0] = rt_time_get_nsec();
	msec[0] = rt_time_get_msec();
	mdelay(100);
	nsec[1] = rt_time_get_nsec();
	msec[1] = rt_time_get_msec();
	LOG_I("delay 100ms end! last %d nsec, %d msec",
		(int)(nsec[1] - nsec[0]),
		(int)(msec[1] - msec[0]));

	LOG_I("before delay 1000ms!");
	nsec[0] = rt_time_get_nsec();
	msec[0] = rt_time_get_msec();
	mdelay(1000);
	nsec[1] = rt_time_get_nsec();
	msec[1] = rt_time_get_msec();
	LOG_I("delay 1000ms end! last %d nsec, %d msec",
		(int)(nsec[1] - nsec[0]),
		(int)(msec[1] - msec[0]));

	/* test rt_thread_delay */
	for (i = 0; i < ARRAY_SIZE(delay_ms); i++) {
		LOG_I("before delay %dms!", delay_ms[i]);

		tick[0] = rt_tick_get();
		msec[0] = rt_time_get_msec();

		rt_thread_delay(msecs_to_tick(delay_ms[i]));

		msec[1] = rt_time_get_msec();
		tick[1] = rt_tick_get();

		LOG_I("delay %dms end! last %d msec, %d tick", delay_ms[i],
			(int)(msec[1] - msec[0]), tick[1] - tick[0]);
	}

	LOG_I("end");
	return 0;
}

