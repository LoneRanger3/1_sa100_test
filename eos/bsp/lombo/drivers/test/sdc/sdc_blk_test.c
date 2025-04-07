/*
 * sdc_blk_test.c - mmcsd block device level test driver
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

#define DBG_SECTION_NAME		"SD_TEST"
#define DBG_LEVEL			DBG_LOG
#include <debug.h>

#include <rtthread.h>
#include <rtdevice.h>
#include <rthw.h>
#include <csp.h>

#include <drivers/mmcsd_core.h>

#include "sdc_test.h"

/* consistent with the sdc driver */
#undef SECTOR_SIZE
#define SECTOR_SIZE			(512)

/**
 * total block range in test:
 * HALF_OF_MAX_TOTAL_BLK ~ (HALF_OF_MAX_TOTAL_BLK * 2 - 1)
 */
#define HALF_OF_MAX_TOTAL_BLK		(64 * 1024 * 1024 / SECTOR_SIZE)

/**
 * buffer block range in test:
 * HALF_OF_MAX_BUF_BLK ~ (HALF_OF_MAX_BUF_BLK * 2 - 1)
 */
#define HALF_OF_MAX_BUF_BLK		8

/**
 * buffer block range in multi thread test:
 * HALF_OF_MAX_BUF_BLK2 ~ (HALF_OF_MAX_BUF_BLK2 * 2 - 1)
 */
#define HALF_OF_MAX_BUF_BLK2		2

/**
 * thread number range in multi thread test:
 * HALF_OF_MAX_THREAD_NUM ~ (HALF_OF_MAX_THREAD_NUM * 2 - 1)
 */
#define HALF_OF_MAX_THREAD_NUM		3

/**
 * total blk in performance test:
 * PERFORMANCE_TEST_TOTAL_BLK
 */
#define PERFORMANCE_TEST_TOTAL_BLK	((16 * 1024 * 1024) / SECTOR_SIZE)

/**
 * buffer blk in performance test:
 * PERFORMANCE_TEST_BUF_BLK
 */
#define PERFORMANCE_TEST_BUF_BLK	((64 * 1024) / SECTOR_SIZE)

typedef int (*test_item_t)(rt_device_t blk_dev);

/* thread param type in multi thread test */
typedef struct thread_param {
	rt_device_t blk_dev;
	u32 thread_num;

	u32 start_addr;
	u32 buf_blk;
	u32 total_blk;
} thread_param_t;

/* waitting all thread finish event in multi thread test */
static struct rt_event event;

/* get current system time, unix: ms */
static inline u32 current_ms()
{
	return rt_tick_get() * 1000 / RT_TICK_PER_SECOND;
}

/* get max block number of mmcsd block device */
static inline u32 max_blk(rt_device_t blk_dev)
{
	int err = 0;
	struct rt_device_blk_geometry geometry;

	rt_memset(&geometry, 0, sizeof(geometry));
	err = rt_device_control(blk_dev, RT_DEVICE_CTRL_BLK_GETGEOME, &geometry);
	if (err) {
		LOG_W("failed to get block geometry");
		return 0;
	}

	return geometry.sector_count;
}

/* basic read, no data verify */
static int test_basic_read(rt_device_t blk_dev)
{
	u8 *buf;
	u32 addr;
	int err = 0;

	LOG_I("test basic read start...");

	buf = (u8 *)rt_malloc(SECTOR_SIZE);
	if (!buf) {
		LOG_E("failed to malloc");
		return -1;
	}

	addr = random2(max_blk(blk_dev));

	/* read request */
	err = rt_device_read(blk_dev, addr, buf, 1);
	if (1 != err) {
		LOG_E("failed to read");
		goto exit_free_buf;
	}

	rt_free(buf);

	LOG_I("test basic read pass...");

	return 0;

exit_free_buf:
	rt_free(buf);

	LOG_E("test basic read failed...");

	return -1;
}

/* basic write, no data verify */
static int test_basic_write(rt_device_t blk_dev)
{
	u8 *buf;
	u32 addr;
	int err = 0;

	LOG_I("test basic write start...");

	buf = (u8 *)rt_malloc(SECTOR_SIZE);
	if (!buf) {
		LOG_E("failed to malloc");
		return -1;
	}

	addr = random2(max_blk(blk_dev));

	/* fill buffer with dummy data */
	rt_memset(buf, 0xA3, SECTOR_SIZE);

	/* write request */
	err = rt_device_write(blk_dev, addr, buf, 1);
	if (1 != err) {
		LOG_E("failed to write");
		goto exit_free_buf;
	}

	rt_free(buf);

	LOG_I("test basic write pass...");

	return 0;

exit_free_buf:
	rt_free(buf);

	LOG_I("test basic write failed...");

	return -1;
}

/* single block access, with data verify: write -> read -> data verify */
static int test_single_blk_access(rt_device_t blk_dev)
{
	u8 *buf;
	u32 addr, random;
	int i, err = 0;

	LOG_I("test single block access start...");

	buf = (u8 *)rt_malloc(SECTOR_SIZE);
	if (!buf) {
		LOG_E("failed to malloc");
		return -1;
	}

	/* fill buffer with regular dummy data */
	random = random2(256);
	for (i = 0; i < SECTOR_SIZE; i++)
		buf[i] = ((i + random) & 0xFF);

	addr = random2(max_blk(blk_dev));

	/* single block write */
	err = rt_device_write(blk_dev, addr, buf, 1);
	if (1 != err) {
		LOG_E("failed to write");
		goto exit_free_buf;
	}

	rt_memset(buf, 0x00, SECTOR_SIZE);

	/* single block read */
	err = rt_device_read(blk_dev, addr, buf, 1);
	if (1 != err) {
		LOG_E("failed to read");
		goto exit_free_buf;
	}

	/* data verify */
	for (i = 0; i < SECTOR_SIZE; i++) {
		if (buf[i] != ((i + random) & 0xFF)) {
			LOG_E("data verify error");
			goto exit_free_buf;
		}
	}

	/* free buffer */
	rt_free(buf);

	LOG_I("test single block access pass...");

	return 0;

exit_free_buf:
	rt_free(buf);

	LOG_I("test single block access failed...");

	return -1;
}

/* multi block access, with data verify:
 *	write -> read -> data verify ->
 *	write -> read -> data verify ->
 *	write -> read -> data verify ->
 *	...
 */
static int multi_blk_access(rt_device_t blk_dev, u32 start_addr,
	u32 buf_blk, u32 total_blk)
{
	u8 *buf;
	u32 addr, random, remain_blk, req_blk, cur_ms, last_ms;
	int i, err = 0;

	LOG_D("multi block access start...");

	/* Our cache line is 64byte, keep the buffer 64byte align to avoid problems.*/
	buf = (u8 *)rt_malloc_align(buf_blk * SECTOR_SIZE, 64);
	if (!buf) {
		LOG_E("failed to malloc");
		return -1;
	}

	addr = start_addr;
	remain_blk = total_blk;
	last_ms = current_ms();
	while (remain_blk > 0) {
		/* heart beat */
		cur_ms = current_ms();
		if ((cur_ms - last_ms) >= 5000) {
			LOG_D("multi block access: current second(%u), remain block(%u)",
				cur_ms / 1000, remain_blk);
			last_ms = cur_ms;
		}

		/* get block's number of this request */
		req_blk = ((remain_blk < buf_blk) ? remain_blk : buf_blk);

		/* fill the buffer with regular dummy data */
		random = random2(256);
		for (i = 0; i < (req_blk * SECTOR_SIZE); i++)
			buf[i] = (i / 256 + i + random) & 0xFF;

		/* multi block write */
		err = rt_device_write(blk_dev, addr, buf, req_blk);
		if (req_blk != err) {
			LOG_E("failed to write");
			goto exit_free_buf;
		}

		rt_memset(buf, 0x00, buf_blk * SECTOR_SIZE);

		/* multi block read */
		err = rt_device_read(blk_dev, addr, buf, req_blk);
		if (req_blk != err) {
			LOG_E("failed to read");
			goto exit_free_buf;
		}

		/* data verify */
		for (i = 0; i < (req_blk * SECTOR_SIZE); i++) {
			if (buf[i] != ((i / 256 + i + random) & 0xFF)) {
				LOG_E("data verify error");
				goto exit_free_buf;
			}
		}

		remain_blk -= req_blk;
		start_addr += req_blk;
	}

	rt_free(buf);

	LOG_D("multi block access finish...");

	return 0;

exit_free_buf:
	rt_free(buf);

	LOG_E("multi block access failed...");

	return -1;
}

/* multi block access test */
static int test_multi_blk_access(rt_device_t blk_dev)
{
	u32 buf_blk, total_blk, start_addr;

	LOG_I("test multi block access start...");

	/* buffer size, unit: block */
	buf_blk = random2(HALF_OF_MAX_BUF_BLK);
	buf_blk += HALF_OF_MAX_BUF_BLK;

	/* total size, unit: block */
	total_blk = random2(HALF_OF_MAX_TOTAL_BLK);
	total_blk += HALF_OF_MAX_TOTAL_BLK;

	/* start address, unit: block */
	start_addr = random2(max_blk(blk_dev) - total_blk);

	LOG_I("test multi block access: buffer block = %u, total block = %u, addr = %u",
		buf_blk, total_blk, start_addr);

	/* multi block access test */
	if (multi_blk_access(blk_dev, start_addr, buf_blk, total_blk))
		goto exit_do_nothing;

	LOG_I("test multi block access pass...");

	return 0;

exit_do_nothing:
	LOG_E("test multi block access failed...");

	return -1;
}

/* thread function in multi thread test */
static void multi_thread_access_t(void *thread_param)
{
	u32 thread_num, start_addr, buf_blk, total_blk;
	rt_device_t blk_dev;
	thread_param_t *param;

	/* get and parse thread param */
	param = (thread_param_t *)thread_param;

	blk_dev = param->blk_dev;
	thread_num = param->thread_num;
	start_addr = param->start_addr;
	buf_blk = param->buf_blk;
	total_blk = param->total_blk;

	LOG_D("thread sdmta%02u start...", thread_num);

	/* multi block access test */
	if (multi_blk_access(blk_dev, start_addr, buf_blk, total_blk)) {
		LOG_E("thread sdmta%02u try multi block access failed", thread_num);
		return;
	}

	/* send thread finish event */
	rt_event_send(&event, (1 << thread_num));

	LOG_D("thread sdmta%02u finish...", thread_num);
}

/* multi thread access test */
static int test_multi_thread_access(rt_device_t blk_dev)
{
	u32 i, thread_num, part_blk;
	rt_uint32_t event_flag, event_recv;
	rt_thread_t tid;
	thread_param_t param[2 * HALF_OF_MAX_THREAD_NUM];
	char thread_name[8];

	LOG_I("test multi thread access start...");

	/* init event */
	rt_event_init(&event, "sd_bt", RT_IPC_FLAG_FIFO);

	/* total thread number */
	thread_num = random2(HALF_OF_MAX_THREAD_NUM);
	thread_num += HALF_OF_MAX_THREAD_NUM;

	/* divide mmcsd partition to thread_num part */
	part_blk = max_blk(blk_dev) / thread_num;

	LOG_D("part_blk:%u", part_blk);

	/* create thread */
	for (i = 0; i < thread_num; i++) {
		rt_memset(thread_name, 0, sizeof(thread_name));
		rt_sprintf(thread_name, "sdmta%02u", i);

		param[i].blk_dev = blk_dev;
		param[i].thread_num = i;
		param[i].buf_blk = HALF_OF_MAX_BUF_BLK2 + random2(HALF_OF_MAX_BUF_BLK2);
		param[i].total_blk = HALF_OF_MAX_TOTAL_BLK
			+ random2(HALF_OF_MAX_TOTAL_BLK);
		if (param[i].total_blk >= part_blk) {
			param[i].total_blk = part_blk;
			param[i].start_addr = part_blk * param[i].thread_num;
		} else {
			param[i].start_addr = part_blk * param[i].thread_num +
				random2(part_blk - param[i].total_blk);
		}

		LOG_D("thread: num:%u, buf_blk:%u, total_blk:%u, start_addr:%u",
			param[i].thread_num, param[i].buf_blk,
			param[i].total_blk, param[i].start_addr);

		/* create one thread */
		tid = rt_thread_create(thread_name, multi_thread_access_t,
			(void *)(&param[i]), 2048, 20, 20);

		if (!tid) {
			LOG_E("failed to create thread %s", thread_name);
			break;
		}

		/* start up one thread */
		rt_thread_startup(tid);
	}

	event_flag = (1 << i) - 1;

	/* waitting for all thread finish */
	LOG_I("test multi thread access: waitting for event");
	rt_event_recv(&event, event_flag, RT_EVENT_FLAG_AND,
		RT_WAITING_FOREVER, &event_recv);

	if (i != thread_num) {
		LOG_E("failed to create some thread");
		goto exit_detach_event;
	}

	/* detach event */
	rt_event_detach(&event);

	LOG_I("test multi thread access pass...");

	return 0;

exit_detach_event:
	rt_event_detach(&event);

	LOG_E("test multi thread access failed...");

	return -1;
}

/* write performance test */
static int test_write_performance(rt_device_t blk_dev)
{
	u8 *buf;
	u32 addr, buf_blk, total_blk;
	u32 loop, i;
	u32 start_ms, end_ms, ms, kb, perf;
	int err = 0;

	LOG_I("test write performance start...");

	buf_blk = PERFORMANCE_TEST_BUF_BLK;
	total_blk = PERFORMANCE_TEST_TOTAL_BLK;

	/* get block request loop */
	loop = total_blk / buf_blk;
	if (total_blk % buf_blk)
		LOG_W("total_blk(%u) can not be divisible by buf_blk(%u), loop=%u",
			total_blk, buf_blk, loop);

	/* start address, unit: block */
	addr = random2(max_blk(blk_dev) - total_blk);

	buf = (u8 *)rt_malloc(buf_blk * SECTOR_SIZE);
	if (!buf) {
		LOG_E("failed to malloc");
		return -1;
	}

	/* fill buffer with dummy data */
	rt_memset(buf, 0xA3, (buf_blk * SECTOR_SIZE));

	LOG_D("test write performance: addr:%u, total_blk:%u, buf_blk:%u, loop:%u",
		addr, total_blk, buf_blk, loop);

	barrier();
	start_ms = current_ms();

	/* write operation */
	for (i = 0; i < loop; i++) {
		err = rt_device_write(blk_dev, addr, buf, buf_blk);
		if (buf_blk != err) {
			LOG_E("failed to write");
			goto exit_free_buf;
		}

		addr += buf_blk;
	}

	end_ms = current_ms();
	barrier();

	rt_free(buf);

	/* calculate write performance */
	ms = end_ms - start_ms;
	kb = (loop * buf_blk * SECTOR_SIZE) / 1024;
	perf = (kb  * 1000) / ms;

	LOG_D("write byte:%u, consume ms:%u", (loop * buf_blk * SECTOR_SIZE), ms);
	LOG_I("write performance: %u KB/s", perf);

	LOG_I("test write performance pass...");

	return 0;

exit_free_buf:
	rt_free(buf);

	LOG_I("test write performance failed...");

	return -1;
}

/* read performance test */
static int test_read_performance(rt_device_t blk_dev)
{
	u8 *buf;
	u32 addr, buf_blk, total_blk;
	u32 loop, i;
	u32 start_ms, end_ms, ms, kb, perf;
	int err = 0;

	LOG_I("test read performance start...");

	buf_blk = PERFORMANCE_TEST_BUF_BLK;
	total_blk = PERFORMANCE_TEST_TOTAL_BLK;

	/* get block request loop */
	loop = total_blk / buf_blk;
	if (total_blk % buf_blk)
		LOG_W("total_blk(%u) can not be divisible by buf_blk(%u), loop=%u",
			total_blk, buf_blk, loop);

	/* start address, unit: block */
	addr = random2(max_blk(blk_dev) - total_blk);

	buf = (u8 *)rt_malloc(buf_blk * SECTOR_SIZE);
	if (!buf) {
		LOG_E("failed to malloc");
		return -1;
	}

	/* fill buffer with dummy data */
	rt_memset(buf, 0xA3, (buf_blk * SECTOR_SIZE));

	LOG_D("test read performance: addr:%u, total_blk:%u, buf_blk:%u, loop:%u",
		addr, total_blk, buf_blk, loop);

	barrier();
	start_ms = current_ms();

	/* write operation */
	for (i = 0; i < loop; i++) {
		err = rt_device_read(blk_dev, addr, buf, buf_blk);
		if (buf_blk != err) {
			LOG_E("failed to write");
			goto exit_free_buf;
		}

		addr += buf_blk;
	}

	end_ms = current_ms();
	barrier();

	rt_free(buf);

	/* calculate write performance */
	ms = end_ms - start_ms;
	kb = (loop * buf_blk * SECTOR_SIZE) / 1024;
	perf = (kb  * 1000) / ms;

	LOG_D("read byte:%u, consume ms:%u", (loop * buf_blk * SECTOR_SIZE), ms);
	LOG_I("read performance: %u KB/s", perf);

	LOG_I("test read performance pass...");

	return 0;

exit_free_buf:
	rt_free(buf);

	LOG_I("test read performance failed...");

	return -1;
}

#if BLK_ACCESS_AGING_TEST
/* multi block access aging test */
static int test_multi_blk_access_aging(rt_device_t blk_dev)
{
	u32 buf_blk, count = 1;

	LOG_I("test multi block access aging start, current second(%u)...",
		current_ms() / 1000);

	while (1) {
		/* buffer size, unit: block */
		buf_blk = random2(HALF_OF_MAX_BUF_BLK);
		buf_blk += HALF_OF_MAX_BUF_BLK;

		LOG_I("multi block aging: count(%u), current second(%u), buf_blk(%u)",
			count, current_ms() / 1000, buf_blk);

		/* multi block access test */
		if (multi_blk_access(blk_dev, 0, buf_blk, max_blk(blk_dev)))
			goto exit_do_nothing;

		count++;
	}

	LOG_I("test multi block access aging pass...");

	return 0;

exit_do_nothing:
	LOG_E("test multi block access aging failed: count(%u), current second(%u)...",
		count, current_ms() / 1000);

	return -1;
}
#endif

/* test item */
static test_item_t test_item[] = {
	test_basic_read,
	test_basic_write,
	test_single_blk_access,
	test_multi_blk_access,
	test_multi_thread_access,
	test_write_performance,
	test_read_performance,
#if BLK_ACCESS_AGING_TEST
	test_multi_blk_access_aging,
#endif
};

/* sd block test thread */
static void sd_blk_test_thread(void *param)
{
	struct rt_device *dev;
	char *dev_name;
	int i, err = 0;

	dev_name = (char *)param;
	LOG_I("sd block access test start, dev(%s)...", dev_name);

	/* find mmcsd device */
	dev = rt_device_find(dev_name);
	if (!dev) {
		LOG_E("failed to find device %s", dev_name);
		goto exit_do_nothing;
	}

	/* usually the filesystem will open the "sd0" device */
	if (!(dev->open_flag & RT_DEVICE_OFLAG_OPEN)) {
		err = rt_device_open(dev, RT_DEVICE_FLAG_RDWR);
		if (err) {
			LOG_E("failed to open block device %s", dev_name);
			return;
		}
	}

	/* check if every test item pass */
	for (i = 0; i < (sizeof(test_item) / sizeof(test_item[0])); i++) {
		err = (test_item[i])(dev);
		if (err)
			goto exit_do_nothing;
	}

	LOG_I("sd block access test pass !!!");

	return;

exit_do_nothing:
	LOG_I("sd block access test failed !!!");
}

/* to create sdc block test thread */
int sdc_blk_test()
{
	rt_thread_t tid;

	/* create sd block test thread */
	tid = rt_thread_create("sd_bt", sd_blk_test_thread,
		DEFAULT_DEV_NAME, 8192, 20, 20);

	if (!tid) {
		LOG_E("failed to create sd block test thread");
		return -RT_ENOMEM;
	}

	/* start sd block test thread */
	rt_thread_startup(tid);

	return 0;
}
