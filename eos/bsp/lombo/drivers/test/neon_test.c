/*
 * neon_test.c - eos test neon functions
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
#include <arm_neon.h>

/* test time, in seconds */
#define TEST_SEC	(20 * 3600)
#define PRINT_GAP_MS	800 /* gap in miliseconds between printf for each thread */

#define  FIR_KERNEL_SIZE	32
#define  FIR_OUTPUT_SIZE	2560
#define  FIR_INPUT_SIZE		(FIR_OUTPUT_SIZE + FIR_KERNEL_SIZE)
#define  FIR_ITERATIONS		10

static const short  fir_kernel[FIR_KERNEL_SIZE] = {
	0x10, 0x20, 0x40, 0x70, 0x8c, 0xa2, 0xce, 0xf0,
	0xe9, 0xce, 0xa2, 0x8c, 070, 0x40, 0x20, 0x10,
	0x10, 0x20, 0x40, 0x70, 0x8c, 0xa2, 0xce, 0xf0,
	0xe9, 0xce, 0xa2, 0x8c, 070, 0x40, 0x20, 0x10
};

static short fir_input_0[FIR_INPUT_SIZE];
static const short *fir_input = fir_input_0 + (FIR_KERNEL_SIZE / 2);
static short fir_output_expected[FIR_OUTPUT_SIZE];

/* this is a FIR filter implemented in C */
static void fir_filter_c(short *output, const short *input,
		const short *kernel, int width, int kernel_size)
{
	int nn, mm, sum = 0, offset = -kernel_size / 2;

	for (nn = 0; nn < width; nn++) {
		sum = 0;
		for (mm = 0; mm < kernel_size; mm++)
			sum += kernel[mm] * input[nn + offset + mm];
		output[nn] = (short)((sum + 0x8000) >> 16);
	}
}

void fir_filter_neon_intrinsics(short *output, const short *input,
			const short *kernel, int width, int kernel_size)
{
	int nn, mm, sum, offset = -kernel_size / 2;
	int16x4_t kernel_vec, input_vec;
	int32x4_t sum_vec;

	for (nn = 0; nn < width; nn++) {
		sum = 0;
		sum_vec = vdupq_n_s32(0);

		for (mm = 0; mm < kernel_size / 4; mm++) {
			kernel_vec = vld1_s16(kernel + mm * 4);
			input_vec = vld1_s16(input + (nn+offset + mm * 4));
			sum_vec = vmlal_s16(sum_vec, kernel_vec, input_vec);
		}

		sum += vgetq_lane_s32(sum_vec, 0);
		sum += vgetq_lane_s32(sum_vec, 1);
		sum += vgetq_lane_s32(sum_vec, 2);
		sum += vgetq_lane_s32(sum_vec, 3);

		if (kernel_size & 3) {
			for (mm = kernel_size - (kernel_size & 3); mm < kernel_size; mm++)
				sum += kernel[mm] * input[nn + offset+mm];
		}

		output[nn] = (short)((sum + 0x8000) >> 16);
	}
}

static void *thread_func1(void *para)
{
	int count, nn, fails = 0, *need_exit = para;
	rt_tick_t start_ms, cur_ms;
	short *fir_output = RT_NULL;

	fir_output = rt_malloc(FIR_OUTPUT_SIZE * sizeof(short));
	if (RT_NULL == fir_output) {
		LOG_FLOAT("alloc fir_output buf failed!");
		return RT_NULL;
	}

	start_ms = rt_time_get_msec();

	while (1 != *need_exit) {
		/* small FIR filter loop - C version */
		count = FIR_ITERATIONS;
		for (; count > 0; count--)
			fir_filter_c(fir_output, fir_input, fir_kernel,
				FIR_OUTPUT_SIZE, FIR_KERNEL_SIZE);

		/* check the result, just in case */
		fails = 0;
		for (nn = 0; nn < FIR_OUTPUT_SIZE; nn++) {
			if (fir_output[nn] != fir_output_expected[nn]) {
				if (++fails < 16)
					LOG_FLOAT("neon[%d] = %d expected %d", nn,
						fir_output[nn], fir_output_expected[nn]);
			}
		}

		if (0 == fails) {
			cur_ms = rt_time_get_msec();
			if (cur_ms - start_ms > PRINT_GAP_MS / 10) {
				LOG_FLOAT("result is ok");
				start_ms = rt_time_get_msec();
			}
		}

		rt_thread_delay(1);
	}

	if (RT_NULL != fir_output)
		rt_free(fir_output);
	return RT_NULL;
}

static void *thread_func2(void *para)
{
	int count, nn, fails = 0, *need_exit = para;
	rt_tick_t start_ms, cur_ms;
	short *fir_output = RT_NULL;

	fir_output = rt_malloc(FIR_OUTPUT_SIZE * sizeof(short));
	if (RT_NULL == fir_output) {
		LOG_FLOAT("alloc fir_output buf failed!");
		return RT_NULL;
	}

	start_ms = rt_time_get_msec();

	while (1 != *need_exit) {
		/* small FIR filter loop - neon version */
		count = FIR_ITERATIONS;
		for (; count > 0; count--)
			fir_filter_neon_intrinsics(fir_output, fir_input, fir_kernel,
				FIR_OUTPUT_SIZE, FIR_KERNEL_SIZE);

		/* check the result, just in case */
		fails = 0;
		for (nn = 0; nn < FIR_OUTPUT_SIZE; nn++) {
			if (fir_output[nn] != fir_output_expected[nn]) {
				if (++fails < 16)
					LOG_FLOAT("neon[%d] = %d expected %d", nn,
						fir_output[nn], fir_output_expected[nn]);
			}
		}

		if (0 == fails) {
			cur_ms = rt_time_get_msec();
			if (cur_ms - start_ms > PRINT_GAP_MS / 10) {
				LOG_FLOAT("result is ok");
				start_ms = rt_time_get_msec();
			}
		}

		rt_thread_delay(1);
	}

	if (RT_NULL != fir_output)
		rt_free(fir_output);
	return RT_NULL;
}

static void *thread_func3(void *para)
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
	}

	return NULL;
}

static void *thread_func4(void *para)
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
	}

	return NULL;
}

/**
 * test_vectorize_unalign_access: Test vectorize unaligned access
 *
 * If buid with -O2, memset will be vectorized by neon. From trm DDI0406C p106,
 * neon instructions(vld4, vst4..) has very strict alignement condition. So test
 * if the compiler can automatically omit unaligned access
 *
 * return 0 if success, -RT_ERROR if failed
 */
int test_vectorize_unalign_access(void)
{
	static char buf1[32 + 8];
	static char buf2[64 + 8];
	static char buf3[128 + 8];
	static char buf4[256 + 8];

	LOG_I("start");

	memset(buf1 + 0, 0x12, 32);
	LOG_I("");
	memset(buf1 + 1, 0x23, 32);
	LOG_I("");
	memset(buf1 + 2, 0x34, 32);
	LOG_I("");
	memset(buf1 + 3, 0x45, 32);
	LOG_I("");
	memset(buf1 + 4, 0x56, 32);
	LOG_I("");
	memset(buf1 + 5, 0x67, 32);
	LOG_I("");
	memset(buf1 + 6, 0x78, 32);
	LOG_I("");
	memset(buf1 + 7, 0x89, 32);
	LOG_I("");

	memset(buf2 + 0, 0x12, 64);
	LOG_I("");
	memset(buf2 + 1, 0x23, 64);
	LOG_I("");
	memset(buf2 + 2, 0x34, 64);
	LOG_I("");
	memset(buf2 + 3, 0x45, 64);
	LOG_I("");
	memset(buf2 + 4, 0x56, 64);
	LOG_I("");
	memset(buf2 + 5, 0x67, 64);
	LOG_I("");
	memset(buf2 + 6, 0x78, 64);
	LOG_I("");
	memset(buf2 + 7, 0x89, 64);
	LOG_I("");

	memset(buf3 + 0, 0x12, 128);
	LOG_I("");
	memset(buf3 + 1, 0x23, 128);
	LOG_I("");
	memset(buf3 + 2, 0x34, 128);
	LOG_I("");
	memset(buf3 + 3, 0x45, 128);
	LOG_I("");
	memset(buf3 + 4, 0x56, 128);
	LOG_I("");
	memset(buf3 + 5, 0x67, 128);
	LOG_I("");
	memset(buf3 + 6, 0x78, 128);
	LOG_I("");
	memset(buf3 + 7, 0x89, 128);
	LOG_I("");

	memset(buf4 + 0, 0x12, 256);
	LOG_I("");
	memset(buf4 + 1, 0x23, 256);
	LOG_I("");
	memset(buf4 + 2, 0x34, 256);
	LOG_I("");
	memset(buf4 + 3, 0x45, 256);
	LOG_I("");
	memset(buf4 + 4, 0x56, 256);
	LOG_I("");
	memset(buf4 + 5, 0x67, 256);
	LOG_I("");
	memset(buf4 + 6, 0x78, 256);
	LOG_I("");
	memset(buf4 + 7, 0x89, 256);
	LOG_I("");

	LOG_I("end");
	return 0;
}

/**
 * test_neon - test for neon operations
 *
 * return 0 if success, -RT_ERROR if failed
 */
long test_neon(int argc, char **argv)
{
	rt_thread_t tid1 = NULL, tid2 = NULL, tid3 = NULL, tid4 = NULL;
	int need_exit1 = 0, need_exit2 = 0, need_exit3 = 0, need_exit4 = 0;
	int nn = 0, count = 0, fails = 0;
	int start_ms, cur_ms, tmp_ms;
	short *fir_output = RT_NULL;
	rt_err_t ret = RT_EOK;

	LOG_FLOAT("start");

	if (test_vectorize_unalign_access()) {
		LOG_FLOAT("test vectorize unalign access failed!");
		goto end;
	}

	fir_output = rt_malloc(FIR_OUTPUT_SIZE * sizeof(short));
	if (RT_NULL == fir_output) {
		LOG_FLOAT("alloc fir_output buf failed!");
		return RT_NULL;
	}

	/* setup FIR input - whatever */
	for (nn = 0; nn < FIR_INPUT_SIZE; nn++)
		fir_input_0[nn] = (5 * nn) & 255;

	/* calc the expected output - for compare */
	fir_filter_c(fir_output_expected, fir_input, fir_kernel,
		FIR_OUTPUT_SIZE, FIR_KERNEL_SIZE);

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
	tmp_ms = start_ms;
	while (cur_ms - start_ms < TEST_SEC * 1000) {
		/* small FIR filter loop - neon version */
		count = FIR_ITERATIONS;
		for (; count > 0; count--)
			fir_filter_neon_intrinsics(fir_output, fir_input, fir_kernel,
				FIR_OUTPUT_SIZE, FIR_KERNEL_SIZE);

		/* check the result, just in case */
		fails = 0;
		for (nn = 0; nn < FIR_OUTPUT_SIZE; nn++) {
			if (fir_output[nn] != fir_output_expected[nn]) {
				if (++fails < 16)
					LOG_FLOAT("neon[%d] = %d expected %d", nn,
						fir_output[nn], fir_output_expected[nn]);
			}
		}

		if (0 == fails) {
			if (cur_ms - tmp_ms > PRINT_GAP_MS) {
				LOG_FLOAT("result is ok");
				tmp_ms = rt_time_get_msec();
			}
		}

		rt_thread_delay(1);
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
	if (RT_NULL != fir_output)
		rt_free(fir_output);

	LOG_FLOAT("end");
	return ret;
}

