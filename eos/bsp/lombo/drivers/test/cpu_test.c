/*
 * pthread_mutex_test.c - eos test pthread_mutex functions
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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <rtthread.h>
#include <debug.h>
#include "board.h"

#define TEST_CPU_LOADING
/* #define TEST_CPU_HOTPLUG2 */

/* test time, in seconds */
#define TEST_SEC	(20)

#ifndef CPU_STATS
#undef TEST_CPU_LOADING
#endif

#ifdef TEST_CPU_HOTPLUG2

struct thread_para {
	int need_exit;
};
static struct thread_para g_para[4];

static void *thread_func0(void *val)
{
	struct thread_para *para = val;
	INIT_DUMP_MEM;
	int i;

	LOG_I("start");
	DUMP_MEMORY();

	while (1 != para->need_exit) {
		for (i = 1; i < RT_CPUS_NR; i++) {
			if (CPU_STATUS_ONLINE == rt_cpu_index(i)->status) {
				LOG_I("start power down cpu%d", i);
				cpu_down(i);
				LOG_I("power down cpu%d end", i);
			} else if (CPU_STATUS_OFFLINE == rt_cpu_index(i)->status) {
				LOG_I("start power up cpu%d", i);
				cpu_up(i);
				LOG_I("power up cpu%d end", i);
			}

			rt_thread_delay(10);

			DUMP_MEMORY();
		}
	}

	DUMP_MEMORY();
	LOG_I("end");
	return NULL;
}

static void *thread_func1(void *val)
{
	struct thread_para *para = val;
	INIT_DUMP_MEM;
	int i;

	LOG_I("start");
	DUMP_MEMORY();

	while (1 != para->need_exit) {
		for (i = 1; i < RT_CPUS_NR; i++) {
			if (CPU_STATUS_ONLINE == rt_cpu_index(i)->status) {
				LOG_I("start power down cpu%d", i);
				cpu_down(i);
				LOG_I("power down cpu%d end", i);
			} else if (CPU_STATUS_OFFLINE == rt_cpu_index(i)->status) {
				LOG_I("start power up cpu%d", i);
				cpu_up(i);
				LOG_I("power up cpu%d end", i);
			}

			rt_thread_delay(10);

			DUMP_MEMORY();
		}
	}

	DUMP_MEMORY();
	LOG_I("end");
	return NULL;
}

static void *thread_func2(void *val)
{
	struct thread_para *para = val;
	INIT_DUMP_MEM;
	int i;

	LOG_I("start");
	DUMP_MEMORY();

	while (1 != para->need_exit) {
		for (i = 1; i < RT_CPUS_NR; i++) {
			if (CPU_STATUS_ONLINE == rt_cpu_index(i)->status) {
				LOG_I("start power down cpu%d", i);
				cpu_down(i);
				LOG_I("power down cpu%d end", i);
			} else if (CPU_STATUS_OFFLINE == rt_cpu_index(i)->status) {
				LOG_I("start power up cpu%d", i);
				cpu_up(i);
				LOG_I("power up cpu%d end", i);
			}

			rt_thread_delay(10);

			DUMP_MEMORY();
		}
	}

	DUMP_MEMORY();
	LOG_I("end");
	return NULL;
}

static void *thread_func3(void *val)
{
	struct thread_para *para = val;
	INIT_DUMP_MEM;
	int i;

	LOG_I("start");
	DUMP_MEMORY();

	while (1 != para->need_exit) {
		for (i = 1; i < RT_CPUS_NR; i++) {
			if (CPU_STATUS_ONLINE == rt_cpu_index(i)->status) {
				LOG_I("start power down cpu%d", i);
				cpu_down(i);
				LOG_I("power down cpu%d end", i);
			} else if (CPU_STATUS_OFFLINE == rt_cpu_index(i)->status) {
				LOG_I("start power up cpu%d", i);
				cpu_up(i);
				LOG_I("power up cpu%d end", i);
			}

			rt_thread_delay(10);

			DUMP_MEMORY();
		}
	}

	DUMP_MEMORY();
	LOG_I("end");
	return NULL;
}

#endif /* TEST_CPU_HOTPLUG2 */

/**
 * test_cpu - test cpu operations such as hotplug..
 *
 * return 0 if success, -RT_ERROR if failed
 */
long test_cpu(int argc, char **argv)
{
#ifdef TEST_CPU_HOTPLUG2
	pthread_t tid[4] = {NULL};
#endif
	rt_err_t ret = -RT_ERROR;
	int start_ms, cur_ms, i = 0;
	INIT_DUMP_MEM;
#ifdef TEST_CPU_LOADING
	int idle_time[RT_CPUS_NR], total_time[RT_CPUS_NR], delta_idle, delta_total;
	int idle_time_last[RT_CPUS_NR], total_time_last[RT_CPUS_NR];
	float idle_rate[RT_CPUS_NR];
#endif

	LOG_I("start");

	DUMP_MEMORY();

#ifdef TEST_CPU_HOTPLUG2
	/* clean global para for each test */
	memset(g_para, 0, sizeof(g_para));

	ret = pthread_create(&tid[0], NULL, (void *)thread_func0, &g_para[0]);
	if (ret != 0) {
		LOG_E("pthread_create failed!");
		goto end;
	}
	ret = pthread_create(&tid[1], NULL, (void *)thread_func1, &g_para[1]);
	if (ret != 0) {
		LOG_E("pthread_create failed!");
		goto end;
	}
	ret = pthread_create(&tid[2], NULL, (void *)thread_func2, &g_para[2]);
	if (ret != 0) {
		LOG_E("pthread_create failed!");
		goto end;
	}
	ret = pthread_create(&tid[3], NULL, (void *)thread_func3, &g_para[3]);
	if (ret != 0) {
		LOG_E("pthread_create failed!");
		goto end;
	}
#endif

#ifdef TEST_CPU_LOADING
	for (i = 0; i < RT_CPUS_NR; i++) {
		idle_time_last[i] = get_cpu_idle_time(i);
		total_time_last[i] = get_cpu_total_time(i);
	}
#endif

	/* continue test until time end */
	start_ms = rt_time_get_msec();
	cur_ms = start_ms;
	while (cur_ms - start_ms < TEST_SEC * 1000) {
		LOG_I("loop start");
		DUMP_MEMORY();

#ifdef TEST_CPU_HOTPLUG2
		for (i = 1; i < RT_CPUS_NR; i++) {
			if (CPU_STATUS_ONLINE == rt_cpu_index(i)->status) {
				LOG_I("start power down cpu%d", i);
				cpu_down(i);
				LOG_I("power down cpu%d end", i);
			} else if (CPU_STATUS_OFFLINE == rt_cpu_index(i)->status) {
				LOG_I("start power up cpu%d", i);
				cpu_up(i);
				LOG_I("power up cpu%d end", i);
			}

			rt_thread_delay(50);
		}
#endif

#ifdef TEST_CPU_LOADING
		for (i = 0; i < RT_CPUS_NR; i++) {
			idle_time[i] = get_cpu_idle_time(i);
			total_time[i] = get_cpu_total_time(i);

			if (total_time[i] - total_time_last[i] > 500) {
				delta_idle = idle_time[i] - idle_time_last[i];
				delta_total = total_time[i] - total_time_last[i];
				idle_rate[i] = (delta_idle * 1.0 / delta_total) * 100;

				printf("cpu%d idle rate %f\n", i, idle_rate[i]);

				idle_time_last[i] = idle_time[i];
				total_time_last[i] = total_time[i];
			}
		}

		rt_thread_delay(50);
#endif

		cur_ms = rt_time_get_msec();
		LOG_I("loop end");
	}

	ret = RT_EOK;

#ifdef TEST_CPU_HOTPLUG2
end:
	for (i = 0; i < 4; i++) {
		g_para[i].need_exit = 1;
		if (NULL != tid[i]) {
			pthread_join(tid[i], NULL);
			tid[i] = NULL;
		}
	}

	/* power up cpu if it's powered down by test */
	for (i = 1; i < RT_CPUS_NR; i++) {
		if (CPU_STATUS_OFFLINE == rt_cpu_index(i)->status)
			cpu_up(i);
	}
#endif

	DUMP_MEMORY();
	LOG_I("end");
	return ret;
}

