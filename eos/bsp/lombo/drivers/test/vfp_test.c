/*
 * vfp_test.c - eos test vfp functions
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

/* test time, in seconds */
#define TEST_SEC	20

void *thread_func1(void *para)
{
	float a = 123.567;
	float b = 78.90;
	float c = 654.34;
	float d = 0;
	int *need_exit = para;
	unsigned int julia = 0;

	while (1 != *need_exit) {
		d = float_test_func(a, b, c);
		LOG_FLOAT("a %f, b %f, c %f, d %f", a, b, c, d);

		rt_thread_delay(1);

		julia = float_test_func2();
		LOG_FLOAT("julia is 0x%x", (int)julia);

		rt_thread_delay(1);
	}

	return NULL;
}

void *thread_func2(void *para)
{
	float a = 123.567;
	float b = 78.90;
	float c = 654.34;
	float d = 0;
	int *need_exit = para;
	unsigned int julia = 0;

	while (1 != *need_exit) {
		julia = float_test_func2();
		LOG_FLOAT("julia is 0x%x", (int)julia);

		rt_thread_delay(1);

		d = float_test_func(a, b, c);
		LOG_FLOAT("a %f, b %f, c %f, d %f", a, b, c, d);

		rt_thread_delay(1);
	}

	return NULL;
}

void *thread_func3(void *para)
{
	float a = 123.567;
	float b = 78.90;
	float c = 654.34;
	float d = 0;
	int *need_exit = para;
	unsigned int julia = 0;

	while (1 != *need_exit) {
		d = float_test_func(a, b, c);
		LOG_FLOAT("a %f, b %f, c %f, d %f", a, b, c, d);

		rt_thread_delay(1);

		julia = float_test_func2();
		LOG_FLOAT("julia is 0x%x", (int)julia);

		rt_thread_delay(1);
	}

	return NULL;
}

void *thread_func4(void *para)
{
	float a = 123.567;
	float b = 78.90;
	float c = 654.34;
	float d = 0;
	int *need_exit = para;
	unsigned int julia = 0;

	while (1 != *need_exit) {
		julia = float_test_func2();
		LOG_FLOAT("julia is 0x%x", (int)julia);

		rt_thread_delay(1);

		d = float_test_func(a, b, c);
		LOG_FLOAT("a %f, b %f, c %f, d %f", a, b, c, d);

		rt_thread_delay(1);
	}

	return NULL;
}

rt_err_t test_float(void)
{
	int need_exit1 = 0, need_exit2 = 0, need_exit3 = 0, need_exit4 = 0;
	rt_thread_t tid1 = NULL, tid2 = NULL, tid3 = NULL, tid4 = NULL;
	int start_ms, cur_ms;
	rt_err_t ret = RT_EOK;

	LOG_FLOAT("start");

	if (pthread_create(&tid1, 0, &thread_func1, &need_exit1) != 0) {
		LOG_FLOAT("create thread_func1 failed!\n");
		goto end;
	}

	if (pthread_create(&tid2, 0, &thread_func2, &need_exit2) != 0) {
		LOG_FLOAT("create thread_func2 failed!\n");
		goto end;
	}

	if (pthread_create(&tid3, 0, &thread_func3, &need_exit3) != 0) {
		LOG_FLOAT("create thread_func3 failed!\n");
		goto end;
	}

	if (pthread_create(&tid4, 0, &thread_func4, &need_exit4) != 0) {
		LOG_FLOAT("create thread_func4 failed!\n");
		goto end;
	}

	/* continue test until time end */
	start_ms = rt_time_get_msec();
	cur_ms = start_ms;
	while (cur_ms - start_ms < TEST_SEC * 1000) {
		float a = 12.34, b = 56.78, c = 90.12;
		float d = 0;

		d = float_test_func(a, b, c);
		LOG_FLOAT("a %f, b %f, c %f, d %f", a, b, c, d);

		rt_thread_delay(5);
		cur_ms = rt_time_get_msec();
	}

end:
	/* let the test thread exit */
	if (NULL != tid1) {
		need_exit1 = 1;
		pthread_join(tid1, NULL);
		tid1 = NULL;
	}
	if (NULL != tid2) {
		need_exit2 = 1;
		pthread_join(tid2, NULL);
		tid2 = NULL;
	}
	if (NULL != tid3) {
		need_exit3 = 1;
		pthread_join(tid3, NULL);
		tid3 = NULL;
	}
	if (NULL != tid4) {
		need_exit4 = 1;
		pthread_join(tid4, NULL);
		tid4 = NULL;
	}

	LOG_FLOAT("end");
	return ret;
}

/**
 * test_vfp - test for vfp operations
 *
 * return 0 if success, -RT_ERROR if failed
 */
long test_vfp(int argc, char **argv)
{
	long ret = 0;

	LOG_I("start");

	ret = test_float();
	if (ret)
		goto end;

	LOG_I("end");
end:
	if (ret)
		LOG_E("failed!");
	return ret;
}

