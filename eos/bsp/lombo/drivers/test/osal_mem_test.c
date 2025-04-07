/*
 * osal_mem_test.c - memory debug module test
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

#include <debug.h>
#include <string.h>
#include "osal_mem.h"

#define MEM_TEST_RES_OK			0
#define MEM_TEST_RES_FAIL		-1

#define NOT_FOUND			-1
#define MEM_ALLOC_SIZE			20

#define TEST_OSAL_CMD_DUMP		"dump"
#define TEST_OSAL_CMD_ALLOC		"alloc"
#define TEST_OSAL_CMD_MEM_INFO		"info"

static int test_osal_find_ptr()
{
	void *p1, *p2, *p3;
	int index = NOT_FOUND;
	rt_err_t ret;

	int offset = 0;
	/* need to add an offset when testing this function separately */
	if (MEM_BORDER_CHECK)
		offset = -MEM_BORDER_LEN;

	/* find NULL or uninitalized point should return false */
	p1 = RT_NULL;
	ret = osal_find_ptr(p1, &index);
	if (ret != RT_FALSE)
		return __LINE__;

	/*
	ret = osal_find_ptr(p2, &index);
	if (ret != RT_FALSE)
		return __LINE__;
	*/

	/* should return true */
	p1 = rt_malloc(MEM_ALLOC_SIZE);
	index = NOT_FOUND;
	ret = osal_find_ptr(p1 + offset, &index);
	if (ret != RT_TRUE)
		return __LINE__;
	if (index == NOT_FOUND)
		return __LINE__;

	p2 = rt_malloc(MEM_ALLOC_SIZE);
	index = NOT_FOUND;
	ret = osal_find_ptr(p2 + offset, &index);
	if (ret != RT_TRUE)
		return __LINE__;
	if (index == NOT_FOUND)
		return __LINE__;

	/* should return false when point have free */
	rt_free(p1);
	index = NOT_FOUND;
	ret = osal_find_ptr(p1 + offset, &index);
	if (ret != RT_FALSE)
		return __LINE__;
	if (index != NOT_FOUND)
		return __LINE__;

	rt_free(p2);
	index = NOT_FOUND;
	ret = osal_find_ptr(p2 + offset, &index);
	if (ret != RT_FALSE)
		return __LINE__;
	if (index != NOT_FOUND)
		return __LINE__;

	/* find point that isn't alllocated */
	int temp = 666;
	p3 = &temp;
	index = NOT_FOUND;
	ret = osal_find_ptr(p3 + offset, &index);
	if (ret != RT_FALSE)
		return __LINE__;
	if (index != NOT_FOUND)
		return __LINE__;

	return MEM_TEST_RES_OK;
}

static int test_osal_ptr_at_range()
{
	void *p1, *p2;
	int index = NOT_FOUND;
	rt_err_t ret;

	/* find NULL or uninitalized point should return false */
	p1 = RT_NULL;
	ret = osal_ptr_at_range(p1, &index);
	if (ret != RT_FALSE)
		return __LINE__;

	/*
	ret = osal_ptr_at_range(p2, &index);
	if (ret != RT_FALSE)
		return __LINE__;
	*/

	/* should return true */
	p1 = rt_malloc(MEM_ALLOC_SIZE);
	index = NOT_FOUND;
	ret = osal_ptr_at_range(p1, &index);
	if (ret != RT_TRUE)
		return __LINE__;
	if (index == NOT_FOUND)
		return __LINE__;

	/* head boundary test */
	index = NOT_FOUND;
	ret = osal_ptr_at_range(p1 - 1, &index);
	if (ret != RT_FALSE)
		return __LINE__;
	if (index != NOT_FOUND)
		return __LINE__;

	index = NOT_FOUND;
	ret = osal_ptr_at_range(p1 + 1, &index);
	if (ret != RT_TRUE)
		return __LINE__;
	if (index == NOT_FOUND)
		return __LINE__;

	/* tail boundary test */
	index = NOT_FOUND;
	ret = osal_ptr_at_range(p1 + MEM_ALLOC_SIZE, &index);
	if (ret != RT_FALSE)
		return __LINE__;
	if (index != NOT_FOUND)
		return __LINE__;

	index = NOT_FOUND;
	ret = osal_ptr_at_range(p1 + MEM_ALLOC_SIZE + 1, &index);
	if (ret != RT_FALSE)
		return __LINE__;
	if (index != NOT_FOUND)
		return __LINE__;

	index = NOT_FOUND;
	ret = osal_ptr_at_range(p1 + MEM_ALLOC_SIZE - 1, &index);
	if (ret != RT_TRUE)
		return __LINE__;
	if (index == NOT_FOUND)
		return __LINE__;

	/* free point */
	rt_free(p1);
	index = NOT_FOUND;
	ret = osal_ptr_at_range(p1, &index);
	if (ret != RT_FALSE)
		return __LINE__;
	if (index != NOT_FOUND)
		return __LINE__;

	/* find point that isn't alllocated */
	int temp = 666;
	p2 = &temp;
	index = NOT_FOUND;
	ret = osal_ptr_at_range(p2, &index);
	/*
	if (ret != RT_FALSE) {
		LOG_D("p2 address = %x08", (rt_uint32_t)p2);
		return __LINE__;
	}
	if (index != NOT_FOUND)
		return __LINE__;
	*/

	return MEM_TEST_RES_OK;
}

static int test_osal_check_border()
{
#if MEM_BORDER_CHECK
	void *p1;
	rt_bool_t ret;
	char src[] = "abcdefghijklmnopqrstuvwxyz0123456789";

	p1 = rt_malloc(MEM_ALLOC_SIZE);
	ret = osal_check_border(p1, __func__, __LINE__);
	if (ret != RT_FALSE)
		return __LINE__;

	/* memset in the range */
	rt_memset(p1, 6, MEM_ALLOC_SIZE / 2);
	ret = osal_check_border(p1, __func__, __LINE__);
	if (ret != RT_FALSE)
		return __LINE__;

	/* memcpy in the range */
	rt_memcpy(p1, src, MEM_ALLOC_SIZE / 2);
	ret = osal_check_border(p1, __func__, __LINE__);
	if (ret != RT_FALSE)
		return __LINE__;

	/* memset over head boundary */
	rt_free(p1);
	p1 = rt_malloc(MEM_ALLOC_SIZE);
	rt_memset(p1 - MEM_BORDER_LEN / 2, 6, MEM_ALLOC_SIZE / 2);

	ret = osal_check_border(p1, __func__, __LINE__);
	if (ret != RT_TRUE)
		return __LINE__;

	/* memcpy over head boundary */
	rt_free(p1);
	p1 = rt_malloc(MEM_ALLOC_SIZE);
	rt_memcpy(p1 - MEM_BORDER_LEN / 2, src, MEM_ALLOC_SIZE / 2);

	ret = osal_check_border(p1, __func__, __LINE__);
	if (ret != RT_TRUE)
		return __LINE__;

	/* memset over tail boundary */
	rt_free(p1);
	p1 = rt_malloc(MEM_ALLOC_SIZE);
	rt_memset(p1 + MEM_ALLOC_SIZE - MEM_BORDER_LEN / 2, 6, MEM_ALLOC_SIZE / 2);

	ret = osal_check_border(p1, __func__, __LINE__);
	if (ret != RT_TRUE)
		return __LINE__;

	/* memcpy over tail boundary */
	rt_free(p1);
	p1 = rt_malloc(MEM_ALLOC_SIZE);
	rt_memcpy(p1 + MEM_ALLOC_SIZE - MEM_BORDER_LEN / 2, src, MEM_ALLOC_SIZE / 2);

	ret = osal_check_border(p1, __func__, __LINE__);
	if (ret != RT_TRUE)
		return __LINE__;

	/* length of src must larger than MEM_ALLOC_SIZE */
	int l = rt_strlen(src);
	if (l < MEM_ALLOC_SIZE + 2)
		return __LINE__;
	int offset = (l - MEM_ALLOC_SIZE) / 2;

	/* memset over head and tail boundary */
	rt_free(p1);
	p1 = rt_malloc(MEM_ALLOC_SIZE);
	rt_memset(p1 - offset, 6, l);

	ret = osal_check_border(p1, __func__, __LINE__);
	if (ret != RT_TRUE)
		return __LINE__;

	/* memcpy over head and tail boundary */
	rt_free(p1);
	p1 = rt_malloc(MEM_ALLOC_SIZE);
	rt_memcpy(p1 - offset, src, l);

	ret = osal_check_border(p1, __func__, __LINE__);
	if (ret != RT_TRUE)
		return __LINE__;

	/* final free */
	rt_free(p1);
#endif
	return MEM_TEST_RES_OK;
}

static int test_check_border_by_calloc()
{
#if MEM_BORDER_CHECK
	void *p1;
	rt_bool_t ret;
	char src[] = "abcdefghijklmnopqrstuvwxyz0123456789";

	p1 = rt_calloc(1, MEM_ALLOC_SIZE);
	ret = osal_check_border(p1, __func__, __LINE__);
	if (ret != RT_FALSE)
		return __LINE__;

	/* memset in the range */
	rt_memset(p1, 6, MEM_ALLOC_SIZE / 2);
	ret = osal_check_border(p1, __func__, __LINE__);
	if (ret != RT_FALSE)
		return __LINE__;

	/* memcpy in the range */
	rt_memcpy(p1, src, MEM_ALLOC_SIZE / 2);
	ret = osal_check_border(p1, __func__, __LINE__);
	if (ret != RT_FALSE)
		return __LINE__;

	/* memset over head boundary */
	rt_free(p1);
	p1 = rt_calloc(1, MEM_ALLOC_SIZE);
	rt_memset(p1 - MEM_BORDER_LEN / 2, 6, MEM_ALLOC_SIZE / 2);

	ret = osal_check_border(p1, __func__, __LINE__);
	if (ret != RT_TRUE)
		return __LINE__;

	/* memcpy over head boundary */
	rt_free(p1);
	p1 = rt_calloc(1, MEM_ALLOC_SIZE);
	rt_memcpy(p1 - MEM_BORDER_LEN / 2, src, MEM_ALLOC_SIZE / 2);

	ret = osal_check_border(p1, __func__, __LINE__);
	if (ret != RT_TRUE)
		return __LINE__;

	/* memset over tail boundary */
	rt_free(p1);
	p1 = rt_calloc(1, MEM_ALLOC_SIZE);
	rt_memset(p1 + MEM_ALLOC_SIZE - MEM_BORDER_LEN / 2, 6, MEM_ALLOC_SIZE / 2);

	ret = osal_check_border(p1, __func__, __LINE__);
	if (ret != RT_TRUE)
		return __LINE__;

	/* memcpy over tail boundary */
	rt_free(p1);
	p1 = rt_calloc(1, MEM_ALLOC_SIZE);
	rt_memcpy(p1 + MEM_ALLOC_SIZE - MEM_BORDER_LEN / 2, src, MEM_ALLOC_SIZE / 2);

	ret = osal_check_border(p1, __func__, __LINE__);
	if (ret != RT_TRUE)
		return __LINE__;

	/* length of src must larger than MEM_ALLOC_SIZE */
	int l = rt_strlen(src);
	if (l < MEM_ALLOC_SIZE + 2)
		return __LINE__;
	int offset = (l - MEM_ALLOC_SIZE) / 2;

	/* memset over head and tail boundary */
	rt_free(p1);
	p1 = rt_calloc(1, MEM_ALLOC_SIZE);
	rt_memset(p1 - offset, 6, l);

	ret = osal_check_border(p1, __func__, __LINE__);
	if (ret != RT_TRUE)
		return __LINE__;

	/* memcpy over head and tail boundary */
	rt_free(p1);
	p1 = rt_calloc(1, MEM_ALLOC_SIZE);
	rt_memcpy(p1 - offset, src, l);

	ret = osal_check_border(p1, __func__, __LINE__);
	if (ret != RT_TRUE)
		return __LINE__;

	/* final free */
	rt_free(p1);
#endif
	return MEM_TEST_RES_OK;
}

static int test_osal_check_over_range()
{
	void *p1;
	rt_bool_t ret;

	p1 = rt_malloc(MEM_ALLOC_SIZE);
	ret = osal_check_over_range(p1, MEM_ALLOC_SIZE / 2, __func__, __LINE__);
	if (ret != RT_FALSE)
		return __LINE__;

	/* boundary test */
	ret = osal_check_over_range(p1, MEM_ALLOC_SIZE, __func__, __LINE__);
	if (ret != RT_FALSE)
		return __LINE__;

	ret = osal_check_over_range(p1, MEM_ALLOC_SIZE - 1, __func__, __LINE__);
	if (ret != RT_FALSE)
		return __LINE__;

	ret = osal_check_over_range(p1, MEM_ALLOC_SIZE + 1, __func__, __LINE__);
	if (ret != RT_TRUE)
		return __LINE__;

	/* center test */
	ret = osal_check_over_range(p1 + MEM_ALLOC_SIZE / 2, 1, __func__, __LINE__);
	if (ret != RT_FALSE)
		return __LINE__;

	ret = osal_check_over_range(p1 + MEM_ALLOC_SIZE / 2,
			MEM_ALLOC_SIZE / 2 + 6, __func__, __LINE__);
	if (ret != RT_TRUE)
		return __LINE__;

	rt_free(p1);
	return MEM_TEST_RES_OK;
}

static int test_align_mem()
{
	void *p;
	rt_bool_t ret;
	int index;
	LOG_D("===== test_align_mem =====");

	p = rt_malloc_align(MEM_ALLOC_SIZE, 32);
	if (p)
		LOG_D("rt_malloc_align p = %08x", (rt_uint32_t)p);
	else
		return __LINE__;

#ifndef EE_GLOBAL_MEM_DEBUG
	osal_dump();
#endif

	index = NOT_FOUND;
	ret = osal_find_ptr(p, &index);

	if (ret != RT_TRUE)
		return __LINE__;
	if (index == NOT_FOUND)
		return __LINE__;

	rt_free_align(p);
	ret = osal_find_ptr(p, &index);
	if (ret != RT_FALSE)
		return __LINE__;

#ifndef EE_GLOBAL_MEM_DEBUG
		osal_dump();
#endif

	return MEM_TEST_RES_OK;
}

static void __dump_unca_case()
{
#ifndef EE_GLOBAL_MEM_DEBUG
	void *p1 = rt_malloc_unca(11);
	void *p2 = rt_zalloc(22);
	void *p3 = rt_zalloc_unca(33);
	void *p4 = rt_zalloc_unca_align(44, 32);
	void *p5 = rt_malloc_align(55, 32);

	LOG_D("__dump_unca_case");
	LOG_D("alloc case:");
	osal_dump();

	rt_free_unca(p1);
	rt_free(p2);
	rt_free_unca(p3);
	rt_free_unca_align(p4);
	rt_free_align(p5);
	LOG_D("free case:");
	osal_dump();
#endif
}

static int test_unca_alloc()
{
	rt_bool_t ret;
	int index;
	rt_size_t sz = 33;
	LOG_D("===== test_unca_alloc =====");

	/* test rt_malloc_unca and rt_free_unca */
	void *p1 = rt_malloc_unca(sz);
	LOG_D("rt_malloc_unca p1 = %08x", (rt_uint32_t)p1);

	index = NOT_FOUND;
	ret = osal_find_ptr(p1, &index);
	if (ret != RT_TRUE)
		return __LINE__;
	if (index == NOT_FOUND)
		return __LINE__;

	rt_free_unca(p1);
	ret = osal_find_ptr(p1, &index);
	if (ret != RT_FALSE)
		return __LINE__;

	/* test rt_zalloc and rt_free */
	void *p2 = rt_zalloc(sz);
	index = NOT_FOUND;
	int offset = 0;
	if (MEM_BORDER_CHECK)
		offset = -MEM_BORDER_LEN;

	ret = osal_find_ptr(p2 + offset, &index);
	if (ret != RT_TRUE)
		return __LINE__;
	if (index == NOT_FOUND)
		return __LINE__;

	rt_free(p2);
	ret = osal_find_ptr(p2, &index);
	if (ret != RT_FALSE)
		return __LINE__;

	/* test rt_zalloc_unca and rt_free_unca */
	void *p3 = rt_zalloc_unca(sz);
	index = NOT_FOUND;
	ret = osal_find_ptr(p3, &index);
	if (ret != RT_TRUE)
		return __LINE__;
	if (index == NOT_FOUND)
		return __LINE__;

	rt_free_unca(p3);
	ret = osal_find_ptr(p3, &index);
	if (ret != RT_FALSE)
		return __LINE__;

	/* test rt_zalloc_unca_align and rt_free_unca_align */
	void *p4 = rt_zalloc_unca_align(sz, 32);
	index = NOT_FOUND;
	ret = osal_find_ptr(p4, &index);
	if (ret != RT_TRUE)
		return __LINE__;
	if (index == NOT_FOUND)
		return __LINE__;

	rt_free_unca_align(p4);
	ret = osal_find_ptr(p4, &index);
	if (ret != RT_FALSE)
		return __LINE__;

	__dump_unca_case();
	return MEM_TEST_RES_OK;
}

void test_example()
{
	/* this test will make system crash */
	void *p;
	char src[] = "abcdefghijklmnopqrstuvwxyz";
	int sz = 66;
	int write_count = 10;

	p = rt_malloc(sz);

	LOG_D("rt_memset: over head boundary to set data");
	rt_memset(p - 2, 1, write_count);

	LOG_D("rt_memset: over tail boundary to set data");
	rt_memset(p + sz - write_count / 2, 1, write_count);

	LOG_D("rt_memcpy: over head boundary to copy data");
	rt_memcpy(p - 2, src, write_count);

	LOG_D("rt_memcpy: over tail boundary to copy data");
	rt_memcpy(p + sz - write_count / 2, src, write_count);

	/* osal_dump(); */

	LOG_D("check boundary value before free");
	rt_free(p);

	/* osal_dump(); */

	/* free again will throw Unexception */
	rt_free(p);
}

void __test_osal_mem()
{
	int ret;
	LOG_D("__test_osal_mem:");
	LOG_D("MEM_BORDER_CHECK = %d", MEM_BORDER_CHECK);

	ret = test_osal_find_ptr();
	if (ret != MEM_TEST_RES_OK) {
		LOG_D("test_osal_find_ptr error: %d", ret);
		return;
	}

	ret = test_osal_ptr_at_range();
	if (ret != MEM_TEST_RES_OK) {
		LOG_D("test_osal_ptr_at_range error: %d", ret);
		return;
	}

	ret = test_osal_check_border();
	if (ret != MEM_TEST_RES_OK) {
		LOG_D("test_osal_check_border error: %d", ret);
		return;
	}

	ret = test_check_border_by_calloc();
	if (ret != MEM_TEST_RES_OK) {
		LOG_D("test_check_border_for_calloc error: %d", ret);
		return;
	}

	ret = test_osal_check_over_range();
	if (ret != MEM_TEST_RES_OK) {
		LOG_D("test_osal_check_over_range error: %d", ret);
		return;
	}

	ret = test_align_mem();
	if (ret != MEM_TEST_RES_OK) {
		LOG_D("test_align_mem error: %d", ret);
		return;
	}

	ret = test_unca_alloc();
	if (ret != MEM_TEST_RES_OK) {
		LOG_D("test_unca_alloc error: %d", ret);
		return;
	}

	LOG_D("test_osal_mem success");

	/*
	LOG_D("----- try crash test -----");
	test_example();
	*/

}

/* temp command to log memory debug info */
static void test_osal_dump()
{
	LOG_D("test_osal_dump:");
	osal_dump();
}

static void osal_mem_info()
{
	rt_uint32_t total, used, max;
	rt_memory_info(&total, &used, &max);
	rt_kprintf("memory info: total = %d, used = %d, max = %d\n", total, used, max);
}

static void test_osal_alloc()
{
	void *p1, *p2;
	rt_size_t sz = 33;

	LOG_D("test_osal_alloc");

	p1 = rt_malloc(sz);
	if (p1 == RT_NULL)
		LOG_D("rt_malloc error");

	p2 = rt_malloc_align(sz, 32);
	if (p2 == RT_NULL)
		LOG_D("rt_malloc_align error");

	if (p1)
		rt_free(p1);

	if (p2)
		rt_free_align(p2);
}

long test_osal_mem(int argc, char **argv)
{
	LOG_D("test_osal_mem...");
	if (3 == argc) {
		char *cmd;

		cmd = argv[2];
		if (!strcmp(cmd, TEST_OSAL_CMD_DUMP))
			test_osal_dump();
		else if (!strcmp(cmd, TEST_OSAL_CMD_ALLOC))
			test_osal_alloc();
		else if (!strcmp(cmd, TEST_OSAL_CMD_MEM_INFO))
			osal_mem_info();
		else
			LOG_D("invalid cmd\n");
	} else
		__test_osal_mem();

	return 0;
}

