/*
 * cache_test.c - cache test driver
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
#include <debug.h>
#include "csp.h"

/* for debug: change defination to rt_malloc.., to catch err */
#define ALLOC_UNCA	rt_malloc_unca
#define FREE_UNCA	rt_free_unca

/* memory length in bytes, for cache flush cases */
#define TEST_BYTE_CNT   16

/**
 * test_dcache_global_ops - test dcache global operations
 *
 * return 0 if success, -RT_ERROR if failed
 */
long test_dcache_global_ops(void)
{
	rt_base_t val = 0;

	LOG_I("start");

	/* test rt_hw_cpu_dcache_flush_all */
	LOG_I("start flush all dcache!");
	rt_hw_cpu_dcache_flush_all();

	/* test rt_hw_cpu_dcache_disable */
	LOG_I("start disable dcache!");
	rt_hw_cpu_dcache_disable();

	/* test rt_hw_cpu_dcache_status */
	val = rt_hw_cpu_dcache_status();
	if (1 == val) {
		LOG_E("err: dcache is still enabled!");
		return -RT_ERROR;
	} else if (0 == val)
		LOG_I("ok: dcache is disabled!");

	/* test rt_hw_cpu_dcache_enable */
	LOG_I("start enable dcache!");
	rt_hw_cpu_dcache_enable();

	/* check if dcache enable success */
	val = rt_hw_cpu_dcache_status();
	if (0 == val) {
		LOG_E("err: dcache is still disabled!");
		return -RT_ERROR;
	} else if (1 == val)
		LOG_I("ok: dcache is enabled!");

	LOG_I("end");
	return 0;
}

/**
 * test_icache_global_ops - test icache global operations
 *
 * return 0 if success, -RT_ERROR if failed
 */
long test_icache_global_ops(void)
{
	rt_base_t val = 0;

	LOG_I("start");

	/* test rt_hw_cpu_icache_inval_all */
	LOG_I("start invalidate all icache!");
	rt_hw_cpu_icache_inval_all();

	/* test rt_hw_cpu_icache_disable */
	LOG_I("start disable icache!");
	rt_hw_cpu_icache_disable();

	/* test rt_hw_cpu_icache_status */
	val = rt_hw_cpu_icache_status();
	if (1 == val) {
		LOG_E("err: icache is still enabled!");
		return -RT_ERROR;
	} else if (0 == val)
		LOG_I("ok: icache is disabled!");

	/* test rt_hw_cpu_icache_enable */
	LOG_I("start enable icache!");
	rt_hw_cpu_icache_enable();

	/* check if icache enable success */
	val = rt_hw_cpu_icache_status();
	if (0 == val) {
		LOG_E("err: icache is still disabled!");
		return -RT_ERROR;
	} else if (1 == val)
		LOG_I("ok: icache is enabled!");

	LOG_I("end");
	return 0;
}

/**
 * test_dcache_clean_range - test clean a range of dcache
 *
 * return 0 if success, -RT_ERROR if failed
 */
long test_dcache_clean_range(void)
{
	char *buf, *buf_unca;

	LOG_I("start");

	/* alloc cached memory */
	buf = rt_zalloc(0x10000);
	if (RT_NULL == buf) {
		LOG_E("alloc cached memory failed");
		return -RT_ERROR;
	}
	LOG_I("alloc cached memory success, ret 0x%p", buf);

	/* get uncached addr */
	buf_unca = virt_to_unca(buf);
	LOG_I("get uncached memory addr 0x%p", buf_unca);

	/* initialize the content */
	memset(buf, 0x5a, TEST_BYTE_CNT);
	if (buf[0] == buf_unca[0]) {
		LOG_E("err: cached and uncached value is both %d", buf[0]);
		goto err;
	}
	LOG_I("ok: the cached value %d, uncached value %d", buf[0], buf_unca[0]);

	/* clean the dcache in memory range */
	rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, buf, TEST_BYTE_CNT);
	LOG_I("after clean dcache: cached value %d, uncached %d", buf[0], buf_unca[0]);
	if (0x5a != buf_unca[0]) {
		LOG_E("err: clean dcache failed");
		goto err;
	}
	LOG_I("ok: clean dcache successful");

	/* free memory */
	rt_free(buf);

	LOG_I("end");
	return 0;
err:
	rt_free(buf);
	return -RT_ERROR;
}

/**
 * test_dcache_inval_range - test invalidate a range of dcache
 *
 * return 0 if success, -RT_ERROR if failed
 */
long test_dcache_inval_range(void)
{
	char *buf, *buf_unca;

	LOG_I("start");

	/* alloc cached memory */
	buf = rt_zalloc(0x10000);
	if (RT_NULL == buf) {
		LOG_E("alloc cached memory failed");
		return -RT_ERROR;
	}
	LOG_I("alloc cached memory success, ret 0x%p", buf);

	/* get uncached addr */
	buf_unca = virt_to_unca(buf);
	LOG_I("get uncached memory addr 0x%p", buf_unca);

	/* initialize the content */
	memset(buf, 0x5a, TEST_BYTE_CNT);
	/*
	 * make the dcache "not" dirty, or the following inv operation will be incorrect
	 *
	 * See <DEN0013D_cortex_a_series_PG 4.0.pdf> Page 8-17:
	 * If the cache contains dirty data, it is generally incorrect to invalidate it.
	 */
	rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, buf, TEST_BYTE_CNT);
	LOG_I("initial: the cached value %d, uncached value %d", buf[0], buf_unca[0]);

	/* change the unca value */
	buf_unca[0] = 0xff;
	LOG_I("after change unca val, the cached value %d, uncached value %d",
		buf[0], buf_unca[0]);

	/* invalidate the dcache in memory range */
	rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, buf, TEST_BYTE_CNT);
	LOG_I("after inval dcache: cached value %d, uncached %d", buf[0], buf_unca[0]);
	if (0xff != buf[0]) {
		LOG_E("err: invalidate dcache failed");
		goto err;
	}
	LOG_I("ok: invalidate dcache successful");

	/* free memory */
	rt_free(buf);

	LOG_I("end");
	return 0;
err:
	rt_free(buf);
	return -RT_ERROR;
}

/**
 * test_dcache_clean_inval_range - test clean and invalidate a range of dcache
 *
 * return 0 if success, -RT_ERROR if failed
 */
long test_dcache_clean_inval_range(void)
{
	char *buf, *buf_unca;

	LOG_I("start");

	/* alloc cached memory */
	buf = rt_zalloc(0x10000);
	if (RT_NULL == buf) {
		LOG_E("alloc cached memory failed");
		return -RT_ERROR;
	}
	LOG_I("alloc cached memory success, ret 0x%p", buf);

	/* get uncached addr */
	buf_unca = virt_to_unca(buf);
	LOG_I("get uncached memory addr 0x%p", buf_unca);

	/* initialize the content */
	memset(buf, 0x5a, TEST_BYTE_CNT);
	buf_unca[0] = 0xff;
	LOG_I("initial: the cached value %d, uncached value %d", buf[0], buf_unca[0]);

	/* clean and invalidate the dcache in memory range */
	rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE | RT_HW_CACHE_FLUSH,
		buf, TEST_BYTE_CNT);
	/* donot read buf[0], keep it's "invalidate" state */
	LOG_I("after clean & inval dcache: uncached %d", buf_unca[0]);

	/* check the result of RT_HW_CACHE_FLUSH(clean) */
	if (0x5a != buf_unca[0]) {
		LOG_E("err: clean dcache failed");
		goto err;
	}
	LOG_I("ok: clean dcache successful. now change the unca value to 0xaa");

	/* change the memory content */
	buf_unca[0] = 0xaa;
	LOG_I("after set unca val to 0xaa: cached value %d, uncached %d",
		buf[0], buf_unca[0]);

	if (0xaa != buf[0]) {
		LOG_E("err: invalidate dcache failed");
		goto err;
	}
	LOG_I("ok: invalidate dcache successful");

	/* free memory */
	rt_free(buf);

	LOG_I("end");
	return 0;
err:
	rt_free(buf);
	return -RT_ERROR;
}

/* test file name for uncached cases */
#define TEST_FILE	"testfile"

/* memory length in bytes, for unalign/uncached access cases */
#define BUF_SIZE	1023

/*
 * test for buffer unalign access:
 *   1. uncached buffer random access without 4 bytes align
 *   2. uncached copy to uncached, compare the content
 *   3. uncached copy to cached, compare the content
 *   4. cached copy to uncached, compare the content
 */
int test_unalign_access(void)
{
	char *buf1 = NULL, *buf2 = NULL, *buf3 = NULL;
	int tmp = 0, ret = -RT_ERROR;

	LOG_I("start");

	/* alloc buffer */
	buf1 = ALLOC_UNCA(BUF_SIZE);
	if (!buf1) {
		LOG_I("alloc(uncached) %d bytes failed!", BUF_SIZE);
		goto end;
	}
	LOG_I("rt_malloc_unca %d bytes success, ret 0x%08x!", BUF_SIZE, buf1);

	buf2 = ALLOC_UNCA(BUF_SIZE);
	if (!buf2) {
		LOG_I("rt_malloc_unca %d bytes failed!", BUF_SIZE);
		goto end;
	}
	LOG_I("alloc(uncached) %d bytes success, ret 0x%08x!", BUF_SIZE, buf2);

	buf3 = rt_malloc(BUF_SIZE);
	if (!buf3) {
		LOG_I("rt_malloc %d bytes failed!", BUF_SIZE);
		goto end;
	}
	LOG_I("rt_malloc %d bytes success, ret 0x%08x!", BUF_SIZE, buf3);

	/* uncached memory, unalign access */
	tmp = *(buf1 + 8);
	LOG_I("");
	tmp = *(buf1 + 4);
	LOG_I("");
	tmp = *(buf1 + 6);
	LOG_I("");
	tmp = *(buf1 + 2);
	LOG_I("");
	tmp = *(buf1 + 5);
	LOG_I("");
	tmp = *(buf1 + 1);
	LOG_I("");

	/* initialize memory content */
	memset(buf1, 0x55, BUF_SIZE);
	LOG_I("");
	memset(buf2, 0xaa, BUF_SIZE);
	LOG_I("");
	memset(buf3, 0x77, BUF_SIZE);
	LOG_I("");

	/* uncached copy to uncached */
	memcpy(buf1, buf2, BUF_SIZE - 3);
	for (tmp = 0; tmp < BUF_SIZE - 3; tmp++) {
		if (buf1[tmp] != 0xaa) {
			LOG_E("uncached copy to uncached, data err!");
			LOG_E("expect 0x%x while actual get 0x%x", 0xaa, buf1[tmp]);
			goto end;
		}
	}

	/* cached copy to uncached */
	memcpy(buf1, buf3, BUF_SIZE - 2);
	for (tmp = 0; tmp < BUF_SIZE - 2; tmp++) {
		if (buf1[tmp] != 0x77) {
			LOG_E("cached copy to uncached, data err!");
			LOG_E("expect 0x%x while actual get 0x%x", 0x77, buf1[tmp]);
			goto end;
		}
	}

	/* uncached copy to cached */
	memcpy(buf3, buf2, BUF_SIZE - 1);
	for (tmp = 0; tmp < BUF_SIZE - 1; tmp++) {
		if (buf3[tmp] != 0xaa) {
			LOG_E("uncached copy to cached, data err!");
			LOG_E("expect 0x%x while actual get 0x%x", 0xaa, buf3[tmp]);
			goto end;
		}
	}

	/* uncached copy to uncached, random access */
	memcpy(buf1 + 8, buf2, 11);
	LOG_I("");
	memcpy(buf1 + 1, buf2, 9);
	LOG_I("");
	memcpy(buf1 + 4, buf2, 7);
	LOG_I("");
	memcpy(buf1 + 5, buf2, 5);
	LOG_I("");
	memcpy(buf1 + 6, buf2, 3);
	LOG_I("");
	memcpy(buf1 + 2, buf2, 1);
	LOG_I("");

	/* uncached copy to cached, random access */
	memcpy(buf3 + 8, buf2, 11);
	LOG_I("");
	memcpy(buf3 + 1, buf2, 9);
	LOG_I("");
	memcpy(buf3 + 4, buf2, 7);
	LOG_I("");
	memcpy(buf3 + 5, buf2, 5);
	LOG_I("");
	memcpy(buf3 + 6, buf2, 3);
	LOG_I("");
	memcpy(buf3 + 2, buf2, 1);
	LOG_I("");

	/* cached copy to uncached, random access */
	memcpy(buf1 + 8, buf3, 11);
	LOG_I("");
	memcpy(buf1 + 1, buf3, 9);
	LOG_I("");
	memcpy(buf1 + 4, buf3, 7);
	LOG_I("");
	memcpy(buf1 + 5, buf3, 5);
	LOG_I("");
	memcpy(buf1 + 6, buf3, 3);
	LOG_I("");
	memcpy(buf1 + 2, buf3, 1);
	LOG_I("");

	ret = 0; /* success */
end:
	if (buf1) {
		FREE_UNCA(buf1);
		buf1 = NULL;
	}
	if (buf2) {
		FREE_UNCA(buf2);
		buf2 = NULL;
	}
	if (buf3) {
		rt_free(buf3);
		buf3 = NULL;
	}

	LOG_I("test %s", (0 == ret ? "success" : "failed"));
	return ret;
}

int test_uncached_file_access(void)
{
	FILE *fp = NULL;
	char *buf = NULL;
	int tmp = 0, ret = -RT_ERROR;

	LOG_I("start");

	fp = fopen(TEST_FILE, "rb");
	if (fp == NULL) {
		LOG_I("open file %s error", TEST_FILE);
		return ret;
	}

	buf = ALLOC_UNCA(BUF_SIZE);
	if (!buf) {
		LOG_E("alloc(uncached) %d bytes failed!", BUF_SIZE);
		goto end;
	}
	LOG_I("alloc(uncached) %d bytes success, ret 0x%08x!", BUF_SIZE, buf);

	tmp = fread(buf, 1, BUF_SIZE, fp);
	if (tmp != BUF_SIZE) {
		LOG_E("fread %d bytes failed, ret(bytes) 0x%08x!", BUF_SIZE, tmp);
		goto end;
	}
	LOG_I("fread %d bytes success, ret 0x%08x!", BUF_SIZE, tmp);

	ret = 0; /* success */
end:
	if (buf) {
		FREE_UNCA(buf);
		buf = NULL;
	}
	if (NULL != fp) {
		fclose(fp);
		fp = NULL;
	}

	LOG_I("test %s", (0 == ret ? "success" : "failed"));
	return ret;
}

/* flush dcache and check if the neighbouring area was destroyed */
#define TEST_SEC	3
#define MEM_GAP		19
#define MEM_BLOCK_LEN	67
#define BUF1_ORG	0x55
#define BUF1_NEW	0x77
#define BUF2_ORG	0xaa
#define BUF2_NEW	0xee
#define BUF1_SIZE	511
#define BUF2_SIZE	513
long test_cached_flush_access(void)
{
	char buf1[BUF1_SIZE], buf2[BUF2_SIZE];
	int i = 0, len_mdf = 0, tmp = 0, ret = -RT_ERROR;
	int start_ms, cur_ms;

	LOG_I("start");

	memset(buf1, BUF1_ORG, sizeof(buf1));
	memset(buf2, BUF2_ORG, sizeof(buf2));

	/* continue until time end */
	start_ms = rt_time_get_msec();
	cur_ms = start_ms;
	while (cur_ms - start_ms < TEST_SEC * 1000) {
		/* change the buf data, with gap */
		for (tmp = 0; tmp < sizeof(buf1); tmp += (MEM_GAP + MEM_BLOCK_LEN)) {
			len_mdf = MEM_BLOCK_LEN;
			if (tmp + MEM_BLOCK_LEN > sizeof(buf1)) { /* exced the buf end */
				len_mdf = sizeof(buf1) - tmp;
				if (len_mdf <= 0) /* no space left for modify */
					break;
			}
			memset(buf1 + tmp, BUF1_NEW, len_mdf);
			rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE | RT_HW_CACHE_FLUSH,
				buf1 + tmp, len_mdf);
		}

		for (tmp = 0; tmp < sizeof(buf2); tmp += (MEM_GAP + MEM_BLOCK_LEN)) {
			len_mdf = MEM_BLOCK_LEN;
			if (tmp + MEM_BLOCK_LEN > sizeof(buf2)) { /* exced the buf end */
				len_mdf = sizeof(buf2) - tmp;
				if (len_mdf <= 0) /* no space left for modify */
					break;
			}
			memset(buf2 + tmp, BUF2_NEW, len_mdf);
			rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE | RT_HW_CACHE_FLUSH,
				buf2 + tmp, len_mdf);
		}

		/* check data for buf1 */
		tmp = 0;
		do {
			/* check the data modified */
			for (i = tmp; i < tmp + MEM_BLOCK_LEN; i++) {
				if (i >= sizeof(buf1))
					goto continue1;

				if (buf1[i] != BUF1_NEW) {
					LOG_E("buf1[%d] is 0x%x, expect 0x%x",
						i, buf1[i], BUF1_NEW);
					goto end;
				}
			}
			/* check the data not been modified */
			for (i = tmp + MEM_BLOCK_LEN;
					i < tmp + MEM_BLOCK_LEN + MEM_GAP; i++) {
				if (i >= sizeof(buf1))
					goto continue1;

				if (buf1[i] != BUF1_ORG) {
					LOG_E("buf1[%d] is 0x%x, expect 0x%x",
						i, buf1[i], BUF1_ORG);
					goto end;
				}
			}

			/* update the compare pos */
			tmp += MEM_BLOCK_LEN + MEM_GAP;
		} while (tmp < sizeof(buf1));

continue1:
		/* check data for buf2 */
		tmp = 0;
		do {
			/* check the data modified */
			for (i = tmp; i < tmp + MEM_BLOCK_LEN; i++) {
				if (i >= sizeof(buf2))
					goto continue2;

				if (buf2[i] != BUF2_NEW) {
					LOG_E("buf2[%d] is 0x%x, expect 0x%x",
						i, buf2[i], BUF2_NEW);
					goto end;
				}
			}
			/* check the data not been modified */
			for (i = tmp + MEM_BLOCK_LEN;
					i < tmp + MEM_BLOCK_LEN + MEM_GAP; i++) {
				if (i >= sizeof(buf2))
					goto continue2;

				if (buf2[i] != BUF2_ORG) {
					LOG_E("buf2[%d] is 0x%x, expect 0x%x",
						i, buf2[i], BUF2_ORG);
					goto end;
				}
			}

			/* update the compare pos */
			tmp += MEM_BLOCK_LEN + MEM_GAP;
		} while (tmp < sizeof(buf2));

continue2:
		/* let me show */
		if ((cur_ms - start_ms) >= (TEST_SEC * 1000) - 5)
			LOG_I("hello, test good");

		rt_thread_delay(1);
		cur_ms = rt_time_get_msec();
	}

	ret = 0; /* success */
end:
	LOG_I("test %s", (0 == ret ? "success" : "failed"));
	return ret;
}

/**
 * test_mmu_operations - test mmu operations
 *
 * return 0 if success, -RT_ERROR if failed
 */
#ifdef ARCH_LOMBO_N7
#include <cp15.h>
#endif
long test_mmu_operations(void)
{
	rt_base_t val = 0;

	LOG_I("start");

	/*
	 * the mmu functions rt_cpu_mmu_enable/.. is not ready for n8,
	 * so omit it for compile pass
	 */
#ifdef ARCH_LOMBO_N7
	/* test rt_cpu_mmu_status */
	val = rt_cpu_mmu_status();
	if (0 == val)
		LOG_I("mmu is disabled!");
	else if (1 == val)
		LOG_I("mmu is enabled!");
#endif

	LOG_I("end");
	return 0;
}

/**
 * test_cache - test for cache operations
 *
 * return 0 if success, -RT_ERROR if failed
 */
long test_cache(int argc, char **argv)
{
	long ret = 0;

	LOG_I("start");

	/* test mmu operations */
	ret = test_mmu_operations();
	if (ret) {
		LOG_E("test_mmu_operations failed!");
		return ret;
	}
	LOG_I("test_mmu_operations success!");

	/* test dcache global operations */
	ret = test_dcache_global_ops();
	if (ret) {
		LOG_E("test_dcache_global_ops failed!");
		return ret;
	}
	LOG_I("test_dcache_global_ops success!");

	/* test icache global operations */
	ret = test_icache_global_ops();
	if (ret) {
		LOG_E("test_icache_global_ops failed!");
		return ret;
	}
	LOG_I("test_icache_global_ops success!");

	/* test dcache clean range */
	ret = test_dcache_clean_range();
	if (ret) {
		LOG_E("test_dcache_clean_range failed!");
		return ret;
	}
	LOG_I("test_dcache_clean_range success!");

	/* test dcache invalidate range */
	ret = test_dcache_inval_range();
	if (ret) {
		LOG_E("test_dcache_inval_range failed!");
		return ret;
	}
	LOG_I("test_dcache_inval_range success!");

	/* test dcache clean&invalidate range */
	ret = test_dcache_clean_inval_range();
	if (ret) {
		LOG_E("test_dcache_clean_inval_range failed!");
		return ret;
	}
	LOG_I("test_dcache_clean_inval_range success!");

	ret = test_unalign_access();
	if (ret) {
		LOG_E("test_unalign_access failed!");
		return ret;
	}
	LOG_I("test_unalign_access success!");

	ret = test_cached_flush_access();
	if (ret) {
		LOG_E("test_cached_flush_access failed!");
		return ret;
	}
	LOG_I("test_cached_flush_access success!");

	ret = test_uncached_file_access();
	if (ret) {
		LOG_E("test_uncached_file_access failed!");
		return ret;
	}
	LOG_I("test_uncached_file_access success!");

	LOG_I("end");
	return 0;
}

