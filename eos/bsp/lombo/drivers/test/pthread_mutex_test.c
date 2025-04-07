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

/* hotplug cpu during test pthread_mutex */
#define TEST_CPU_HOTPLUG

/* test time, in seconds */
#define TEST_SEC	5

#define WAIT_LOCK_NEW(mutex, cond, val)			\
{							\
		pthread_mutex_lock(&(mutex));		\
		pthread_cond_wait(&(cond), &(mutex));	\
		RT_ASSERT((val) == 1);			\
}

#define WAIT_UNLOCK_NEW(mutex, val)			\
{							\
		(val) = 0;				\
		pthread_mutex_unlock(&(mutex));		\
}

#define SIGNAL_WAKEUP_NEW(mutex, cond, val)		\
{							\
		pthread_mutex_lock(&(mutex));		\
		if (!(val)) {				\
			(val) = 1;			\
			pthread_cond_signal(&(cond));	\
			pthread_mutex_unlock(&(mutex));	\
		} else					\
			pthread_mutex_unlock(&(mutex));	\
}

/* index for thread_para.cond[]/mutex[]/val[] */
#define READ	0
#define WRITE	1

struct thread_para {
	pthread_cond_t cond[2];
	pthread_mutex_t mutex[2];
	int val[2];
	int need_exit;
};
static struct thread_para g_para[4];

static char *gbuf[4];
static int size[4];

static void *thread_func0(void *val)
{
	struct thread_para *para = val;
	float a = 123.567;
	float b = 78.90;
	float c = 654.34;
	float d = 0;
	unsigned int julia = 0, idx = 0; /* index */
	time_t t;
	INIT_DUMP_MEM;

	LOG_I("start");

	DUMP_MEMORY();

	srand((unsigned)time(&t));

	while (1 != para->need_exit) {
		LOG_I("loop start...");

		/* aquire read lock */
		LOG_I("before aquire read mutex..");
		WAIT_LOCK_NEW(para->mutex[READ], para->cond[READ], para->val[READ]);
		LOG_I("after aquire read mutex!");

		/* inital to 0, or mainthread set to 0 */
		RT_ASSERT(RT_NULL == gbuf[idx]);

		/* malloc buffer and access */
		size[idx] = (rand() * 0x12345) & (SZ_16M - 1);
		gbuf[idx] = rt_malloc(size[idx]);
		if (RT_NULL != gbuf[idx]) {
			LOG_I("malloc gbuf[%d] success: size 0x%08x, ret 0x%08x",
				(int)idx, size[idx], (int)gbuf[idx]);
			memset(gbuf[idx], 0xaa, size[idx]);
		} else
			LOG_I("malloc gbuf[%d] failed: size 0x%08x", idx, size[idx]);

		rt_thread_delay(1);

		/* float calc test */
		d = float_test_func(a, b, c);
		LOG_FLOAT("a %f, b %f, c %f, d %f", a, b, c, d);

		rt_thread_delay(1);

		julia = float_test_func2();
		LOG_FLOAT("julia is 0x%x", (int)julia);

		/* release read lock */
		WAIT_UNLOCK_NEW(para->mutex[READ], para->val[READ]);

		/* let main thread access the buffer */
		LOG_I("before signal write cond..");
		SIGNAL_WAKEUP_NEW(para->mutex[WRITE], para->cond[WRITE],
			para->val[WRITE]);

		rt_thread_delay(1);
		LOG_I("loop end");
	}

	LOG_I("end");
	return NULL;
}

static void *thread_func1(void *val)
{
	struct thread_para *para = val;
	float a = 123.567;
	float b = 78.90;
	float c = 654.34;
	float d = 0;
	unsigned int julia = 0, idx = 1; /* index */
	time_t t;
	INIT_DUMP_MEM;

	LOG_I("start");

	DUMP_MEMORY();

	srand((unsigned)time(&t));

	while (1 != para->need_exit) {
		LOG_I("loop start...");

		/* aquire read lock */
		LOG_I("before aquire read mutex..");
		WAIT_LOCK_NEW(para->mutex[READ], para->cond[READ], para->val[READ]);
		LOG_I("after aquire read mutex!");

		/* inital to 0, or mainthread set to 0 */
		RT_ASSERT(RT_NULL == gbuf[idx]);

		/* malloc buffer and access */
		size[idx] = (rand() * 0x12345) & (SZ_16M - 1);
		gbuf[idx] = rt_malloc(size[idx]);
		if (RT_NULL != gbuf[idx]) {
			LOG_I("malloc gbuf[%d] success: size 0x%08x, ret 0x%08x",
				(int)idx, size[idx], (int)gbuf[idx]);
			memset(gbuf[idx], 0xaa, size[idx]);
		} else
			LOG_I("malloc gbuf[%d] failed: size 0x%08x", idx, size[idx]);

		rt_thread_delay(1);

		/* float calc test */
		d = float_test_func(a, b, c);
		LOG_FLOAT("a %f, b %f, c %f, d %f", a, b, c, d);

		rt_thread_delay(1);

		julia = float_test_func2();
		LOG_FLOAT("julia is 0x%x", (int)julia);

		/* release read lock */
		WAIT_UNLOCK_NEW(para->mutex[READ], para->val[READ]);

		/* let main thread access the buffer */
		LOG_I("before signal write cond..");
		SIGNAL_WAKEUP_NEW(para->mutex[WRITE], para->cond[WRITE],
			para->val[WRITE]);

		rt_thread_delay(1);
		LOG_I("loop end");
	}

	LOG_I("end");
	return NULL;
}

static void *thread_func2(void *val)
{
	struct thread_para *para = val;
	float a = 123.567;
	float b = 78.90;
	float c = 654.34;
	float d = 0;
	unsigned int julia = 0, idx = 2; /* index */
	time_t t;
	INIT_DUMP_MEM;

	LOG_I("start");

	DUMP_MEMORY();

	srand((unsigned)time(&t));

	while (1 != para->need_exit) {
		LOG_I("loop start...");

		/* aquire read lock */
		LOG_I("before aquire read mutex..");
		WAIT_LOCK_NEW(para->mutex[READ], para->cond[READ], para->val[READ]);
		LOG_I("after aquire read mutex!");

		/* inital to 0, or mainthread set to 0 */
		RT_ASSERT(RT_NULL == gbuf[idx]);

		/* malloc buffer and access */
		size[idx] = (rand() * 0x12345) & (SZ_16M - 1);
		gbuf[idx] = rt_malloc(size[idx]);
		if (RT_NULL != gbuf[idx]) {
			LOG_I("malloc gbuf[%d] success: size 0x%08x, ret 0x%08x",
				(int)idx, size[idx], (int)gbuf[idx]);
			memset(gbuf[idx], 0xaa, size[idx]);
		} else
			LOG_I("malloc gbuf[%d] failed: size 0x%08x", idx, size[idx]);

		rt_thread_delay(1);

		/* float calc test */
		d = float_test_func(a, b, c);
		LOG_FLOAT("a %f, b %f, c %f, d %f", a, b, c, d);

		rt_thread_delay(1);

		julia = float_test_func2();
		LOG_FLOAT("julia is 0x%x", (int)julia);

		/* release read lock */
		WAIT_UNLOCK_NEW(para->mutex[READ], para->val[READ]);

		/* let main thread access the buffer */
		LOG_I("before signal write cond..");
		SIGNAL_WAKEUP_NEW(para->mutex[WRITE], para->cond[WRITE],
			para->val[WRITE]);

		rt_thread_delay(1);
		LOG_I("loop end");
	}

	LOG_I("end");
	return NULL;
}

static void *thread_func3(void *val)
{
	struct thread_para *para = val;
	float a = 123.567;
	float b = 78.90;
	float c = 654.34;
	float d = 0;
	unsigned int julia = 0, idx = 3; /* index */
	time_t t;
	INIT_DUMP_MEM;

	LOG_I("start");

	DUMP_MEMORY();

	srand((unsigned)time(&t));

	while (1 != para->need_exit) {
		LOG_I("loop start...");

		/* aquire read lock */
		LOG_I("before aquire read mutex..");
		WAIT_LOCK_NEW(para->mutex[READ], para->cond[READ], para->val[READ]);
		LOG_I("after aquire read mutex!");

		/* inital to 0, or mainthread set to 0 */
		RT_ASSERT(RT_NULL == gbuf[idx]);

		/* malloc buffer and access */
		size[idx] = (rand() * 0x12345) & (SZ_16M - 1);
		gbuf[idx] = rt_malloc(size[idx]);
		if (RT_NULL != gbuf[idx]) {
			LOG_I("malloc gbuf[%d] success: size 0x%08x, ret 0x%08x",
				(int)idx, size[idx], (int)gbuf[idx]);
			memset(gbuf[idx], 0xaa, size[idx]);
		} else
			LOG_I("malloc gbuf[%d] failed: size 0x%08x", idx, size[idx]);

		rt_thread_delay(1);

		/* float calc test */
		d = float_test_func(a, b, c);
		LOG_FLOAT("a %f, b %f, c %f, d %f", a, b, c, d);

		rt_thread_delay(1);

		julia = float_test_func2();
		LOG_FLOAT("julia is 0x%x", (int)julia);

		/* release read lock */
		WAIT_UNLOCK_NEW(para->mutex[READ], para->val[READ]);

		/* let main thread access the buffer */
		LOG_I("before signal write cond..");
		SIGNAL_WAKEUP_NEW(para->mutex[WRITE], para->cond[WRITE],
			para->val[WRITE]);

		rt_thread_delay(1);
		LOG_I("loop end");
	}

	LOG_I("end");
	return NULL;
}

/**
 * test_pthread_mutex - test for pthread_mutex operations
 *
 * return 0 if success, -RT_ERROR if failed
 */
long test_pthread_mutex(int argc, char **argv)
{
	pthread_t tid[4] = {NULL};
	rt_err_t ret = -RT_ERROR;
	int start_ms, cur_ms, i = 0;
	INIT_DUMP_MEM;

	LOG_I("start");

	DUMP_MEMORY();

	for (i = 0; i < sizeof(g_para) / sizeof(g_para[0]); i++) {
		pthread_mutex_init(&g_para[i].mutex[READ], NULL);
		pthread_mutex_init(&g_para[i].mutex[WRITE], NULL);
		pthread_cond_init(&g_para[i].cond[READ], NULL);
		pthread_cond_init(&g_para[i].cond[WRITE], NULL);
		g_para[i].val[READ] = 0;
		g_para[i].val[WRITE] = 0;
		g_para[i].need_exit = 0;
	}

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
		LOG_I("loop start..");

		/* let the threads go */
		for (i = 0; i < 4; i++)
			SIGNAL_WAKEUP_NEW(g_para[i].mutex[READ],
				g_para[i].cond[READ],
				g_para[i].val[READ]);

		for (i = 0; i < 4; i++) {
			/* wait for write mutex from threads */
			LOG_I("before aquire write mutex%d..", i);
			WAIT_LOCK_NEW(g_para[i].mutex[WRITE],
				g_para[i].cond[WRITE],
				g_para[i].val[WRITE]);
			LOG_I("=============%d%d%d%d%d=============", i, i, i, i, i);

			/* buf allocated in sub-threads */
			RT_ASSERT(RT_NULL != gbuf[i]);

			/* access buf */
			memset(gbuf[i], 0x5a, size[i]);
			rt_free(gbuf[i]);
			gbuf[i] = NULL;

			/* release write mutex */
			WAIT_UNLOCK_NEW(g_para[i].mutex[WRITE], g_para[i].val[WRITE]);
		}

#ifdef TEST_CPU_HOTPLUG
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
		}
#endif
		rt_thread_delay(1);
		cur_ms = rt_time_get_msec();
		LOG_I("loop end");
	}

	ret = RT_EOK;
end:
	LOG_I("");

	for (i = 0; i < 4; i++) {
		g_para[i].need_exit = 1;
		if (NULL != tid[i]) {
			/* let sub-thread exit, they may be waiting in WAIT_LOCK_NEW.. */
			SIGNAL_WAKEUP_NEW(g_para[i].mutex[READ],
				g_para[i].cond[READ], g_para[i].val[READ]);
			pthread_join(tid[i], NULL);
			tid[i] = NULL;

			/* release resources */
			if (RT_NULL != gbuf[i]) {
				rt_free(gbuf[i]);
				gbuf[i] = NULL;
			}
			if (0 != pthread_cond_destroy(&g_para[i].cond[READ]))
				LOG_E("destroy read cond failed! i %d", i);
			if (0 != pthread_cond_destroy(&g_para[i].cond[WRITE]))
				LOG_E("destroy write cond failed! i %d", i);
			if (0 != pthread_mutex_destroy(&g_para[i].mutex[READ]))
				LOG_E("destroy read mutex failed! i %d", i);
			if (0 != pthread_mutex_destroy(&g_para[i].mutex[WRITE]))
				LOG_E("destroy write mutex failed! i %d", i);
		}
	}

	DUMP_MEMORY();
	LOG_I("end");
	return ret;
}

