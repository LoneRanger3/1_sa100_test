/*
 * mem_test.c - memory management test driver
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
#include <rthw.h>
#include <rtdef.h>
#include <debug.h>
#include <stdlib.h>
#include "csp.h"

#define RAND_ALLOC_SIZE	(SZ_4M + SZ_16K + SZ_1)	/* random allocate size */
#define FIX_BLOCK_CNT   32			/* block cnt for memory pool */
#define FIX_BLOCK_SIZE  4096			/* block size for memory pool */

/* size of the whole memory pool, used only for dynamic memory pool */
#define MEM_POOL_SIZE	((FIX_BLOCK_SIZE + 4) * FIX_BLOCK_CNT)

/**
 * addr_convert_test - test for addr convertion macros
 *
 * return 0 if success, -RT_ERROR if failed
 */
long addr_convert_test(void)
{
	char *addr_ca, *addr_unca, *addr_phys, *tmp;

	LOG_I("start");

	/* alloc memory for test */
	addr_ca = rt_malloc(0x100000);
	if (RT_NULL == addr_ca) {
		LOG_E("alloc cached memory failed");
		return -RT_ERROR;
	}
	LOG_I("alloc cached memory success, ret 0x%p", addr_ca);

	/* test virt_to_unca */
	addr_unca = virt_to_unca(addr_ca);
	LOG_I("uncached addr for 0x%p is 0x%p", addr_ca, addr_unca);

	/* test virt_to_phys */
	addr_phys = (char *)virt_to_phys(addr_ca);
	LOG_I("phsy addr for 0x%p is 0x%p", addr_ca, addr_phys);

	/* test phys_to_virt */
	tmp = phys_to_virt(addr_phys);
	if (addr_ca != tmp) {
		LOG_E("phys_to_virt(0x%p) is 0x%p, not addr_ca(0x%p)",
			addr_phys, tmp, addr_ca);
		return -RT_ERROR;
	}
	LOG_I("phys_to_virt test success");

	/* test phys_to_unca */
	tmp = phys_to_unca(addr_phys);
	if (addr_unca != tmp) {
		LOG_E("phys_to_unca(0x%p) is 0x%p, not addr_unca(0x%p)",
			addr_phys, tmp, addr_unca);
		return -RT_ERROR;
	}
	LOG_I("phys_to_unca test success");

	/* test unca_to_virt */
	tmp = unca_to_virt(addr_unca);
	if (addr_ca != tmp) {
		LOG_E("unca_to_virt(0x%p) is 0x%p, not addr_ca(0x%p)",
			addr_unca, tmp, addr_ca);
		return -RT_ERROR;
	}
	LOG_I("unca_to_virt test success");

	/* test unca_to_phys */
	tmp = (char *)unca_to_phys(addr_unca);
	if (addr_phys != tmp) {
		LOG_E("unca_to_phys(0x%p) is 0x%p, not addr_phys(0x%p)",
			addr_unca, tmp, addr_phys);
		return -RT_ERROR;
	}
	LOG_I("unca_to_phys test success");

	/* free memory */
	rt_free(addr_ca);

	LOG_I("end");
	return 0;
}

/**
 * cached_random_test - test for cached random size memory allocation
 *
 * return 0 if success, -RT_ERROR if failed
 */
long cached_random_test(void)
{
	rt_size_t size = RAND_ALLOC_SIZE;
	char *buf, *buf_unca;

	LOG_I("start");

	/* alloc cached memory */
	buf = rt_zalloc(size);
	if (RT_NULL == buf) {
		LOG_E("alloc 0x%08x cached memory failed", size);
		return -RT_ERROR;
	}
	LOG_I("alloc 0x%08x cached memory success, ret 0x%p, phys addr 0x%p",
		size, buf, virt_to_phys(buf));

	/* get uncached addr */
	buf_unca = virt_to_unca(buf);
	LOG_I("get uncached memory addr 0x%p", buf_unca);

	/* initialize the content */
	/* memset(buf, 0xff, size); */
	buf[0] = 0xAA;
	buf_unca[0] = 0x5F;
	if (buf[0] != buf_unca[0])
		LOG_I("ok: the cached value %d, uncached %d", buf[0], buf_unca[0]);
	else
		LOG_I("err: the cached value %d, uncached %d", buf[0], buf_unca[0]);

	/* free memory */
	rt_free(buf);
	LOG_I("end");
	return 0;
}

/**
 * uncached_random_test - test for uncached random size memory allocation
 *
 * return 0 if success, -RT_ERROR if failed
 */
long uncached_random_test(void)
{
	rt_size_t size = RAND_ALLOC_SIZE;
	char *buf;

	LOG_I("start");

	/* alloc cached memory */
	buf = rt_malloc_unca(size);
	if (RT_NULL == buf) {
		LOG_E("alloc 0x%08x uncached memory failed", size);
		return -RT_ERROR;
	}
	LOG_I("alloc 0x%08x uncached memory success, ret 0x%p, phys addr 0x%p",
		size, buf, unca_to_phys(buf));

	/* initialize the content */
	memset(buf, 0xff, size);

	/* free memory */
	rt_free_unca(buf);
	LOG_I("end");
	return 0;
}

/**
 * cached_fix_test - test for cached fixed size memory allocation
 *
 * return 0 if success, -RT_ERROR if failed
 */
long cached_fix_test(void)
{
	struct rt_mempool *pool = RT_NULL;
	char *buf;

	LOG_I("start");

	/* create memory pool */
	pool = rt_mp_create("demo", FIX_BLOCK_CNT, FIX_BLOCK_SIZE);
	if (RT_NULL == pool) {
		LOG_E("create memory pool failed!");
		return -RT_ERROR;
	}
	LOG_I("create memory pool success, ret 0x%p", pool);

	/* alloc from memory pool */
	buf = rt_mp_alloc(pool, 0);
	if (RT_NULL == buf) {
		LOG_E("allocate from memory pool failed!");
		rt_mp_delete(pool);
		return -RT_ERROR;
	}
	LOG_I("alloc from memory pool success, ret 0x%p", buf);

	/* access the buf */
	memset(buf, 0x5a, FIX_BLOCK_SIZE);

	/* free the buf */
	rt_mp_free(buf);

	/* delete the memory pool */
	rt_mp_delete(pool);
	LOG_I("end");
	return 0;
}

/**
 * uncached_fix_test - test for uncached fixed size memory allocation
 *
 * return 0 if success, -RT_ERROR if failed
 */
long uncached_fix_test(void)
{
	void *pool_start = RT_NULL;
	char *buf = RT_NULL;
	rt_err_t ret = RT_EOK;
	struct rt_mempool pool;

	LOG_I("start");

	/* create memory pool */
	pool_start = rt_malloc_unca(MEM_POOL_SIZE);
	if (RT_NULL == pool_start) {
		LOG_E("memory allocate failed for mempool!");
		return -RT_ERROR;
	}
	LOG_I("memory allocate for mempool success, ret 0x%p", pool_start);

	/* initialize the memory pool */
	ret = rt_mp_init(&pool, "demo", pool_start, MEM_POOL_SIZE, FIX_BLOCK_SIZE);
	if (RT_EOK != ret) {
		LOG_E("rt_mp_init failed!");
		goto err_free_unca;
	}
	LOG_I("rt_mp_init success for memory pool");

	/* alloc from memory pool */
	buf = rt_mp_alloc(&pool, 0);
	if (RT_NULL == buf) {
		LOG_E("allocate from memory pool failed!");
		goto err_detach;
	}
	LOG_I("allocate from memory pool success, ret 0x%p!", buf);

	/* access the uncached memory block */
	memset(buf, 0x7f, FIX_BLOCK_SIZE);

	/* free the buffer */
	rt_mp_free(buf);

	/* detach the memory pool */
	ret = rt_mp_detach(&pool);
	if (RT_EOK != ret)
		LOG_E("rt_mp_detach failed!");
	else
		LOG_I("rt_mp_detach success!");

	/* free the memory pool buffer */
	rt_free_unca(pool_start);

	LOG_I("end");
	return 0; /* success */
err_detach:
	rt_mp_detach(&pool);
err_free_unca:
	rt_free_unca(pool_start);
	return -RT_ERROR;
}

#if 0
static void *thread_func1(void *para)
{
	int *need_exit = para;
	char *buf = RT_NULL;
	u32 size = 0;
	time_t t;

	LOG_I("start");

	srand((unsigned)time(&t));

	while (1 != *need_exit) {
		size = (rand() * 0x12345) & (SZ_32M - 1);

		buf = rt_malloc(size);
		if (RT_NULL == buf) {
			LOG_E("alloc %d failed!", (int)size);
			goto end;
		}
		LOG_I("alloc %d success! ret 0x%08x", (int)size, (int)buf);

		rt_thread_delay(1);
	}

end:
	LOG_I("end");
	return NULL;
}

/**
 * malloc_access_stess_test - test for malloc and access
 *
 * return 0 if success, -RT_ERROR if failed
 */
long malloc_access_stess_test(void)
{
	rt_tick_t tick_start, tick_cur, period;
	int need_exit1 = 0, need_exit2 = 0, need_exit3 = 0, need_exit4 = 0;
	rt_thread_t tid1 = NULL, tid2 = NULL, tid3 = NULL, tid4 = NULL;
	rt_err_t ret = RT_EOK;

	LOG_I("start");

	if (pthread_create(&tid1, 0, &thread_func1, &need_exit1) != 0) {
		LOG_VFP("create thread_func1 failed!\n");
		goto end;
	}

	if (pthread_create(&tid2, 0, &thread_func2, &need_exit2) != 0) {
		LOG_VFP("create thread_func2 failed!\n");
		goto end;
	}

	if (pthread_create(&tid3, 0, &thread_func3, &need_exit3) != 0) {
		LOG_VFP("create thread_func3 failed!\n");
		goto end;
	}

	if (pthread_create(&tid4, 0, &thread_func4, &need_exit4) != 0) {
		LOG_VFP("create thread_func4 failed!\n");
		goto end;
	}

	/* continue test until time end */
	tick_start = rt_tick_get();
	tick_cur = tick_start;
	period = rt_tick_from_millisecond(TEST_SEC * 1000);
	while (tick_cur - tick_start < period) {
		float a = 12.34, b = 56.78, c = 90.12;
		float d = 0;

		d = float_test_func(a, b, c);
		LOG_VFP("a %f, b %f, c %f, d %f", a, b, c, d);

		rt_thread_delay(5);
		tick_cur = rt_tick_get();
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

	LOG_I("end");
	return ret;
}
#endif

/**
 * test_memory - test for memory management
 *
 * return 0 if success, -RT_ERROR if failed
 */
long test_memory(int argc, char **argv)
{
	long ret = 0;

	LOG_I("start");

	/* test addr convert macros */
	ret = addr_convert_test();
	if (ret) {
		LOG_E("addr_convert_test failed!");
		return ret;
	}
	LOG_I("addr_convert_test success!");

	/* test alloc cached memory with random size */
	ret = cached_random_test();
	if (ret) {
		LOG_E("cached_random_test failed!");
		return ret;
	}
	LOG_I("cached_random_test success!");

	/* test alloc uncached memory with random size */
	ret = uncached_random_test();
	if (ret) {
		LOG_E("uncached_random_test failed!");
		return ret;
	}
	LOG_I("uncached_random_test success!");

	/* test alloc cached memory with fixed size */
	ret = cached_fix_test();
	if (ret) {
		LOG_E("cached_fix_test failed!");
		return ret;
	}
	LOG_I("cached_fix_test success!");

	/* test alloc uncached memory with fixed size */
	ret = uncached_fix_test();
	if (ret) {
		LOG_E("uncached_fix_test failed!");
		return ret;
	}
	LOG_I("uncached_fix_test success!");

#if 0
	/* test malloc exceed heapsize */
	ret = malloc_access_stess_test();
	if (ret) {
		LOG_E("malloc_access_stess_test failed!");
		return ret;
	}
	LOG_I("malloc_access_stess_test success!");
#endif
	LOG_I("end");
	return 0;
}

