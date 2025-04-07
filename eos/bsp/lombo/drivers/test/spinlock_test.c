/*
 * spinlock_test.c - eos test spinlock functions
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
#include <spinlock.h>
#include "board.h"

/* test time, in seconds */
#define TEST_SEC	100

/* test spin_lock_irqsave/.. or spin_lock/.. */
/* #define	TEST_SPINLOCK_IRQ */

#ifdef TEST_SPINLOCK_IRQ
#define LOCK()		spin_lock_irqsave(&g_lock, level);
#define UNLOCK()	spin_unlock_irqrestore(&g_lock, level);
#else
#define LOCK()		spin_lock(&g_lock);
#define UNLOCK()	spin_unlock(&g_lock);
#endif

struct thread_para {
	int need_exit;
};
static struct thread_para g_para[4];

DEFINE_SPINLOCK(g_lock);

static void *thread_func0(void *val)
{
	struct thread_para *para = val;
#ifdef TEST_SPINLOCK_IRQ
	unsigned long level;
#endif
	time_t t;
	INIT_DUMP_MEM;

	LOG_I("start");

	DUMP_MEMORY();

	srand((unsigned)time(&t));

	while (1 != para->need_exit) {
		LOG_I("loop start...");

		LOG_I("before aquire spinlock..");

		LOCK();

		LOG_I("after aquire spinlock!");

		mdelay(100);

		LOG_I("before release spinlock..");

		UNLOCK();

		LOG_I("after release spinlock!");

		rt_thread_delay(1);

		LOG_I("loop end");
	}

	LOG_I("end");
	return NULL;
}

static void *thread_func1(void *val)
{
	struct thread_para *para = val;
#ifdef TEST_SPINLOCK_IRQ
	unsigned long level;
#endif
	time_t t;
	INIT_DUMP_MEM;

	LOG_I("start");

	DUMP_MEMORY();

	srand((unsigned)time(&t));

	while (1 != para->need_exit) {
		LOG_I("loop start...");

		LOG_I("before aquire spinlock..");

		LOCK();

		LOG_I("after aquire spinlock!");

		mdelay(100);

		LOG_I("before release spinlock..");

		UNLOCK();

		LOG_I("after release spinlock!");

		rt_thread_delay(1);

		LOG_I("loop end");
	}

	LOG_I("end");
	return NULL;
}

static void *thread_func2(void *val)
{
	struct thread_para *para = val;
#ifdef TEST_SPINLOCK_IRQ
	unsigned long level;
#endif
	time_t t;
	INIT_DUMP_MEM;

	LOG_I("start");

	DUMP_MEMORY();

	srand((unsigned)time(&t));

	while (1 != para->need_exit) {
		LOG_I("loop start...");

		LOG_I("before aquire spinlock..");

		LOCK();

		LOG_I("after aquire spinlock!");

		mdelay(100);

		LOG_I("before release spinlock..");

		UNLOCK();

		LOG_I("after release spinlock!");

		rt_thread_delay(1);

		LOG_I("loop end");
	}

	LOG_I("end");
	return NULL;
}

static void *thread_func3(void *val)
{
	struct thread_para *para = val;
#ifdef TEST_SPINLOCK_IRQ
	unsigned long level;
#endif
	time_t t;
	INIT_DUMP_MEM;

	LOG_I("start");

	DUMP_MEMORY();

	srand((unsigned)time(&t));

	while (1 != para->need_exit) {
		LOG_I("loop start...");

		LOG_I("before aquire spinlock..");

		LOCK();

		LOG_I("after aquire spinlock!");

		mdelay(100);

		LOG_I("before release spinlock..");

		UNLOCK();

		LOG_I("after release spinlock!");

		rt_thread_delay(1);

		LOG_I("loop end");
	}

	LOG_I("end");
	return NULL;
}

/**
 * test_spinlock - test for spinlock operations
 *
 * return 0 if success, -RT_ERROR if failed
 */
long test_spinlock(int argc, char **argv)
{
	pthread_t tid[4] = {NULL};
	rt_err_t ret = -RT_ERROR;
	int start_ms, cur_ms, i = 0;
#ifdef TEST_SPINLOCK_IRQ
	unsigned long level;
#endif
	INIT_DUMP_MEM;

	LOG_I("start");

	DUMP_MEMORY();

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

	/* continue test until time end */
	start_ms = rt_time_get_msec();
	cur_ms = start_ms;
	while (cur_ms - start_ms < TEST_SEC * 1000) {
		LOG_I("before aquire spinlock..");

		LOCK();

		LOG_I("after aquire spinlock!");

		mdelay(100);

		LOG_I("before release spinlock..");

		UNLOCK();

		LOG_I("after release spinlock!");

		rt_thread_delay(1);
		cur_ms = rt_time_get_msec();
	}

	ret = RT_EOK;
end:
	LOG_I("");
	for (i = 0; i < 4; i++) {
		g_para[i].need_exit = 1;
		if (NULL != tid[i]) {
			pthread_join(tid[i], NULL);
			tid[i] = NULL;
		}
	}

	DUMP_MEMORY();
	LOG_I("end");
	return ret;
}

