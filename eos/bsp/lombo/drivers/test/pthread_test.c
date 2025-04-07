/*
 * pthread_test.c - eos test pthread functions
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

#include <pthread.h>
#include <rtthread.h>
#include <debug.h>
#include "board.h"

/* test mempoll module */
#define TEST_MEMPOOL

/*
 * the PTHREAD_INIT_STATUS should either be PTHREAD_CREATE_JOINABLE,
 * or PTHREAD_CREATE_DETACHED, not both
 *
 * PTHREAD_CREATE_JOINABLE: call pthread_join to wait child thread exit
 * PTHREAD_CREATE_DETACHED: call pthread_attr_setdetachstate to set thread
 *   state to DETACHED status before pthread_create
 *
 */
#define PTHREAD_INIT_STATUS	PTHREAD_CREATE_JOINABLE
/* #define PTHREAD_INIT_STATUS	PTHREAD_CREATE_DETACHED */

/* call pthread_detach after pthread_create */
#define HAVE_PTHREAD_DETACH

/* call pthread_exit in the end of child thread */
#define HAVE_PTHREAD_EXIT

/*
 * test time, in seconds
 */
#define TEST_SEC_SINGLE	10	/* time for one test */
#define TEST_SEC_TOTAL	50	/* time for total looping test */

int need_exit[4] = {0};

#ifdef TEST_MEMPOOL
rt_mp_t g_mp;
#endif

static void *thread_func1(void *para)
{
	INIT_DUMP_MEM;
#ifdef TEST_MEMPOOL
	void *buf = RT_NULL;
#endif

	while (1 != need_exit[0]) {
		LOG_I("");
#ifdef TEST_MEMPOOL
		if (RT_NULL != g_mp) {
			buf = rt_mp_alloc(g_mp, 5);
			/* LOG_W("alloc buf ret 0x%08x", (int)buf); */
		}
#endif
		rt_thread_delay(1);
		LOG_I("");
#ifdef TEST_MEMPOOL
		if (RT_NULL != buf) {
			/* LOG_W("free buf 0x%08x", (int)buf); */
			rt_mp_free(buf);
			buf = RT_NULL;
		}
#endif
	}

	DUMP_MEMORY();

#ifdef HAVE_PTHREAD_EXIT
	pthread_exit(0);
#endif
	return NULL;
}

static void *thread_func2(void *para)
{
	INIT_DUMP_MEM;
#ifdef TEST_MEMPOOL
	void *buf = RT_NULL;
#endif

	while (1 != need_exit[1]) {
		LOG_I("");
#ifdef TEST_MEMPOOL
		if (RT_NULL != g_mp) {
			buf = rt_mp_alloc(g_mp, 5);
			/* LOG_W("alloc buf ret 0x%08x", (int)buf); */
		}
#endif
		rt_thread_delay(1);
		LOG_I("");
#ifdef TEST_MEMPOOL
		if (RT_NULL != buf) {
			/* LOG_W("free buf 0x%08x", (int)buf); */
			rt_mp_free(buf);
			buf = RT_NULL;
		}
#endif
	}

	DUMP_MEMORY();

#ifdef HAVE_PTHREAD_EXIT
	pthread_exit(0);
#endif
	return NULL;
}

static void *thread_func3(void *para)
{
	INIT_DUMP_MEM;
#ifdef TEST_MEMPOOL
	void *buf = RT_NULL;
#endif

	while (1 != need_exit[2]) {
		LOG_I("");
#ifdef TEST_MEMPOOL
		if (RT_NULL != g_mp) {
			buf = rt_mp_alloc(g_mp, 5);
			/* LOG_W("alloc buf ret 0x%08x", (int)buf); */
		}
#endif
		rt_thread_delay(1);
		LOG_I("");
#ifdef TEST_MEMPOOL
		if (RT_NULL != buf) {
			/* LOG_W("free buf 0x%08x", (int)buf); */
			rt_mp_free(buf);
			buf = RT_NULL;
		}
#endif
	}

	DUMP_MEMORY();

#ifdef HAVE_PTHREAD_EXIT
	pthread_exit(0);
#endif
	return NULL;
}

static void *thread_func4(void *para)
{
	INIT_DUMP_MEM;
#ifdef TEST_MEMPOOL
	void *buf = RT_NULL;
#endif

	while (1 != need_exit[3]) {
		LOG_I("");
#ifdef TEST_MEMPOOL
		if (RT_NULL != g_mp) {
			buf = rt_mp_alloc(g_mp, 5);
			/* LOG_W("alloc buf ret 0x%08x", (int)buf); */
		}
#endif
		rt_thread_delay(1);
		LOG_I("");
#ifdef TEST_MEMPOOL
		if (RT_NULL != buf) {
			/* LOG_W("free buf 0x%08x", (int)buf); */
			rt_mp_free(buf);
			buf = RT_NULL;
		}
#endif
	}

	DUMP_MEMORY();

#ifdef HAVE_PTHREAD_EXIT
	pthread_exit(0);
#endif
	return NULL;
}

long test_proc_main(int argc, char **argv)
{
	rt_thread_t tid1 = NULL, tid2 = NULL, tid3 = NULL, tid4 = NULL;
	int start_ms, cur_ms;
	rt_err_t ret = RT_EOK;
	pthread_attr_t tmp_attr;
	int tmp;
	INIT_DUMP_MEM;

	LOG_I("start");

	DUMP_MEMORY();

	memset(need_exit, 0, sizeof(need_exit));

	tmp = pthread_attr_init(&tmp_attr);
	if (tmp != 0) {
		LOG_I("pthread_attr_init ret %d!\n", tmp);
		goto end;
	}

#if PTHREAD_INIT_STATUS == PTHREAD_CREATE_DETACHED
	tmp = pthread_attr_setdetachstate(&tmp_attr, PTHREAD_CREATE_DETACHED);
	if (tmp != 0) {
		LOG_I("pthread_attr_setdetachstate ret %d\n", tmp);
		goto end;
	}
#endif

	if (pthread_create(&tid1, &tmp_attr, &thread_func1, NULL) != 0) {
		LOG_I("create thread_func1 failed!\n");
		goto end;
	}
	DUMP_MEMORY();

#ifdef HAVE_PTHREAD_DETACH
	pthread_detach(tid1);
	DUMP_MEMORY();
#endif

	if (pthread_create(&tid2, &tmp_attr, &thread_func2, NULL) != 0) {
		LOG_I("create thread_func2 failed!\n");
		goto end;
	}
	DUMP_MEMORY();

#ifdef HAVE_PTHREAD_DETACH
	pthread_detach(tid2);
	DUMP_MEMORY();
#endif

	if (pthread_create(&tid3, &tmp_attr, &thread_func3, NULL) != 0) {
		LOG_I("create thread_func3 failed!\n");
		goto end;
	}
	DUMP_MEMORY();

#ifdef HAVE_PTHREAD_DETACH
	pthread_detach(tid3);
	DUMP_MEMORY();
#endif

	if (pthread_create(&tid4, &tmp_attr, &thread_func4, NULL) != 0) {
		LOG_I("create thread_func4 failed!\n");
		goto end;
	}
	DUMP_MEMORY();

#ifdef HAVE_PTHREAD_DETACH
	pthread_detach(tid4);
	DUMP_MEMORY();
#endif

#ifdef TEST_MEMPOOL
	g_mp = rt_mp_create("mp_test", 2, SZ_128);
	if (RT_NULL == g_mp) {
		LOG_I("rt_mp_create failed!\n");
		goto end;
	}
#endif

	/* continue until time end */
	start_ms = rt_time_get_msec();
	cur_ms = start_ms;
	while (cur_ms - start_ms < TEST_SEC_SINGLE * 1000) {
		LOG_I("");
		rt_thread_delay(5);
		LOG_I("");
		cur_ms = rt_time_get_msec();
		LOG_I("");
	}

end:
	/* let the test thread exit */
	need_exit[0] = 1;
	need_exit[1] = 1;
	need_exit[2] = 1;
	need_exit[3] = 1;

	/* wait the thread exit if it was not detached */
#if (PTHREAD_INIT_STATUS == PTHREAD_CREATE_JOINABLE) && !defined(HAVE_PTHREAD_DETACH)
	if (NULL != tid1) {
		pthread_join(tid1, NULL);
		tid1 = NULL;
	}
	DUMP_MEMORY();

	if (NULL != tid2) {
		pthread_join(tid2, NULL);
		tid2 = NULL;
	}
	DUMP_MEMORY();

	if (NULL != tid3) {
		pthread_join(tid3, NULL);
		tid3 = NULL;
	}
	DUMP_MEMORY();

	if (NULL != tid4) {
		pthread_join(tid4, NULL);
		tid4 = NULL;
	}
	DUMP_MEMORY();
#endif

#ifdef TEST_MEMPOOL
	if (RT_NULL != g_mp) {
		rt_mp_delete(g_mp);
		g_mp = RT_NULL;
	}
#endif

	LOG_I("end");
	return ret;
}

/**
 * test_pthread - test for pthread operations
 *
 * return 0 if success, -RT_ERROR if failed
 */
long test_pthread(int argc, char **argv)
{
	int start_ms, cur_ms;
	rt_err_t ret = RT_EOK;
	INIT_DUMP_MEM;

	LOG_I("start");

	DUMP_MEMORY();

	list_thread();

	/* continue until time end */
	start_ms = rt_time_get_msec();
	cur_ms = start_ms;
	while (cur_ms - start_ms < TEST_SEC_TOTAL * 1000) {
		LOG_I("");

		ret = test_proc_main(argc, argv);
		if (RT_EOK != ret) {
			LOG_E("test_proc_main failed, ret %d", ret);
			goto end;
		}

		cur_ms = rt_time_get_msec();
		LOG_I("");
	}

end:
	DUMP_MEMORY();

	list_thread();

	LOG_I("end");
	return ret;
}

