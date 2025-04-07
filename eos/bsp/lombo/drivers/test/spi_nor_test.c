/*
 * spi_nor_test.c - spi nor test driver
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
#include <rtdevice.h>
#include <rthw.h>
#include "csp.h"
#ifdef LOMBO_SPI_NOR
#include "spinor_drv.h"
#endif
#ifdef RT_USING_SFUD
#include "spi_flash_sfud.h"
#endif

#define DBG_SECTION_NAME		"NOR_TEST"
#define DBG_LEVEL			DBG_INFO
#include <debug.h>

#define TEST_LOMBO_NOR			(1 << 1)
#define TEST_SFUD_NOR			(1 << 2)

#define NOR_DEV_NAME			"nor0"

#define WRITE_BUF_SIZE			(64 * 1024)
#define READ_BUF_SIZE			(64 * 1024)

/* multi sector test's sector count:
 * MULTI_SECTOR_MAX_HALF ~ (2 * MULTI_SECTOR_MAX_HALF -1)*/
#define MULTI_SECTOR_MAX_HALF		6

typedef int (*test_item_t)();

static u32 next = 1;
static rt_uint8_t w_buf[WRITE_BUF_SIZE], r_buf[READ_BUF_SIZE];

/* create a random value */
static u32 random()
{
	next += rt_tick_get();
	next = next * 1103515245L + 12345;

	return (u32) (next / 65536L);
}

/* create a random value in range */
static u32 random2(u32 range)
{
	if (0 == range)
		return 0;

	next = random() % range;

	return next;
}

/* get current system time, unix: us */
static inline u32 current_us()
{
	return rt_time_get_usec();
}

static int test_chip_erase()
{
	rt_device_t nor_dev;
	struct rt_device_blk_geometry geometry;
	rt_uint32_t erase_addr[2];
	rt_uint32_t sector_remain, sector_start, sector_num, sector_ret, i;
	int err = 0;

	LOG_I("test chip erase start...");

	/* find nor device */
	nor_dev = rt_device_find(NOR_DEV_NAME);
	if (!nor_dev) {
		LOG_E("failed to find nor device: %s", NOR_DEV_NAME);
		goto exit_do_nothing;
	}

	/* get flash geometry */
	err = rt_device_control(nor_dev, RT_DEVICE_CTRL_BLK_GETGEOME, &geometry);
	if (err) {
		LOG_E("failed to get flash geometry");
		goto exit_do_nothing;
	}

	LOG_D("toatal size: %u(Byte), sector count:%u, sector size:%u, block size:%u",
		geometry.sector_count * geometry.bytes_per_sector,
		geometry.sector_count, geometry.bytes_per_sector, geometry.block_size);

	/* chip erase */
	erase_addr[0] = 0;
	erase_addr[1] = geometry.sector_count;
	err = rt_device_control(nor_dev, RT_DEVICE_CTRL_BLK_ERASE, erase_addr);
	if (err) {
		LOG_E("failed to erase chip");
		goto exit_do_nothing;
	}

	/* read and verify data after chip erase, from address 0 to max address */
	sector_remain = geometry.sector_count;
	sector_start = 0;
	while (sector_remain) {
		sector_num = READ_BUF_SIZE / geometry.bytes_per_sector;
		if (sector_num > sector_remain)
			sector_num = sector_remain;

		rt_memset(r_buf, 0x00, READ_BUF_SIZE);
		sector_ret = rt_device_read(nor_dev, sector_start, r_buf, sector_num);

		if (sector_ret != sector_num) {
			LOG_E("return length error");
			goto exit_do_nothing;
		}

		for (i = 0; i < (sector_num * geometry.bytes_per_sector); i++) {
			if (r_buf[i] != 0xFF) {
				LOG_E("data verify error");
				goto exit_do_nothing;
			}
		}

		sector_start += sector_num;
		sector_remain -= sector_num;
	};

	LOG_I("test chip erase pass...");

	return 0;

exit_do_nothing:

	LOG_E("test chip erase failed...");

	return -1;
}

static int sector_erase_write_read(rt_uint32_t start_sector, rt_uint32_t sector_num,
	rt_uint8_t *w_buf, rt_uint8_t *r_buf)
{
	rt_device_t nor_dev;
	struct rt_device_blk_geometry geometry;
	rt_uint32_t erase_addr[2];
	rt_uint32_t i, random, byte_num, sector_ret;
	int err = 0;

	/* find nor device */
	nor_dev = rt_device_find(NOR_DEV_NAME);
	if (!nor_dev) {
		LOG_E("failed to find nor device: %s", NOR_DEV_NAME);
		goto exit_do_nothing;
	}

	/* get flash geometry */
	err = rt_device_control(nor_dev, RT_DEVICE_CTRL_BLK_GETGEOME, &geometry);
	if (err) {
		LOG_E("failed to get flash geometry");
		goto exit_do_nothing;
	}

	byte_num = sector_num * geometry.bytes_per_sector;

	random = random2(256);

	LOG_D("sector erase write read start: start sector:%u, sector num:%u, random:%u",
		start_sector, sector_num, random);

	/* erase */
	erase_addr[0] = start_sector;
	erase_addr[1] = start_sector + sector_num;
	err = rt_device_control(nor_dev, RT_DEVICE_CTRL_BLK_ERASE, erase_addr);
	if (err) {
		LOG_E("failed to erase nor flash");
		goto exit_do_nothing;
	}

	/* read and verify data after erase */
	rt_memset(r_buf, 0x00, byte_num);
	sector_ret = rt_device_read(nor_dev, start_sector, r_buf, sector_num);

	if (sector_ret != sector_num) {
		LOG_E("return length error");
		goto exit_do_nothing;
	}

	for (i = 0; i < byte_num; i++)
		if (r_buf[i] != 0xFF) {
			LOG_E("data verify error");
			goto exit_do_nothing;
		}

	/* write */
	for (i = 0; i < byte_num; i++)
		w_buf[i] = ((random + i + i / 256) & 0xFF);

	sector_ret = rt_device_write(nor_dev, start_sector, w_buf, sector_num);

	if (sector_ret != sector_num) {
		LOG_E("return length error");
		goto exit_do_nothing;
	}

	/* read and verify data after write */
	rt_memset(r_buf, 0x00, byte_num);
	sector_ret = rt_device_read(nor_dev, start_sector, r_buf, sector_num);

	if (sector_ret != sector_num) {
		LOG_E("return length error");
		goto exit_do_nothing;
	}

	for (i = 0; i < byte_num; i++)
		if (r_buf[i] != ((random + i + i / 256) & 0xFF)) {
			LOG_E("data verify error");
			goto exit_do_nothing;
		}

	LOG_D("sector erase write read pass...");

	return 0;

exit_do_nothing:

	LOG_D("sector erase write read failed...");

	return -1;
}

static int test_sector_erase_write_read()
{
	rt_device_t nor_dev;
	struct rt_device_blk_geometry geometry;
	rt_uint32_t start_sector;
	int err = 0;

	LOG_I("test sector write and read start...");

	/* find nor device */
	nor_dev = rt_device_find(NOR_DEV_NAME);
	if (!nor_dev) {
		LOG_E("failed to find nor device: %s", NOR_DEV_NAME);
		goto exit_do_nothing;
	}

	/* get flash geometry */
	err = rt_device_control(nor_dev, RT_DEVICE_CTRL_BLK_GETGEOME, &geometry);
	if (err) {
		LOG_E("failed to get flash geometry");
		goto exit_do_nothing;
	}

	start_sector = random2(geometry.sector_count);

	/* test single sector erase, write and read*/
	err = sector_erase_write_read(start_sector, 1, w_buf, r_buf);
	if (err)
		goto exit_do_nothing;

	LOG_I("test sector write and read pass...");

	return 0;

exit_do_nothing:

	LOG_E("test sector write and read failed...");

	return -1;
}

static int test_address_range()
{
	rt_device_t nor_dev;
	struct rt_device_blk_geometry geometry;
	int err = 0;

	LOG_I("test address range start...");

	/* find nor device */
	nor_dev = rt_device_find(NOR_DEV_NAME);
	if (!nor_dev) {
		LOG_E("failed to find nor device: %s", NOR_DEV_NAME);
		goto exit_do_nothing;
	}

	/* get flash geometry */
	err = rt_device_control(nor_dev, RT_DEVICE_CTRL_BLK_GETGEOME, &geometry);
	if (err) {
		LOG_E("failed to get flash geometry");
		goto exit_do_nothing;
	}

	/* test the first sector erase, write and read*/
	err = sector_erase_write_read(0, 1, w_buf, r_buf);
	if (err)
		goto exit_do_nothing;

	/* test the last sector erase, write and read*/
	err = sector_erase_write_read((geometry.sector_count - 1), 1, w_buf, r_buf);
	if (err)
		goto exit_do_nothing;

/* TODO: */
#if 0
	/* test out of range sector erase, write and read*/
	err = sector_erase_write_read(geometry.sector_count, 1, w_buf, r_buf);
	if (!err) {
		LOG_E("support out of range operation ???");
		goto exit_do_nothing;
	}
#endif

	LOG_I("test address range pass...");

	return 0;

exit_do_nothing:

	LOG_E("test address range failed...");

	return -1;
}

static int test_multi_sector_erase_write_read()
{
	rt_device_t nor_dev;
	struct rt_device_blk_geometry geometry;
	rt_uint32_t start_sector, sector_num, byte_num;
	rt_uint8_t *w_buf, *r_buf;
	int err = 0;

	LOG_I("test multi sector write and read start...");

	/* find nor device */
	nor_dev = rt_device_find(NOR_DEV_NAME);
	if (!nor_dev) {
		LOG_E("failed to find nor device: %s", NOR_DEV_NAME);
		goto exit_do_nothing;
	}

	/* get flash geometry */
	err = rt_device_control(nor_dev, RT_DEVICE_CTRL_BLK_GETGEOME, &geometry);
	if (err) {
		LOG_E("failed to get flash geometry");
		goto exit_do_nothing;
	}

	start_sector = random2(geometry.sector_count);
	sector_num = random2(MULTI_SECTOR_MAX_HALF);
	sector_num += MULTI_SECTOR_MAX_HALF;

	byte_num = sector_num * geometry.bytes_per_sector;

	/* check if test area out of range */
	if ((start_sector + sector_num) >= geometry.sector_count)
		start_sector = geometry.sector_count - sector_num - 1;

	/* malloc buffer */
	w_buf = (rt_uint8_t *)rt_malloc(byte_num);
	if (!w_buf) {
		LOG_E("failed to malloc");
		goto exit_do_nothing;
	}

	r_buf = (rt_uint8_t *)rt_malloc(byte_num);
	if (!r_buf) {
		LOG_E("failed to malloc");
		goto exit_free_buf;
	}

	/* test multi sector erase, write and read*/
	err = sector_erase_write_read(start_sector, sector_num, w_buf, r_buf);
	if (err)
		goto exit_free_buf;

	/* free buffer */
	if (w_buf)
		rt_free(w_buf);

	if (r_buf)
		rt_free(r_buf);

	LOG_I("test multi sector write and read pass...");

	return 0;

exit_free_buf:
	if (w_buf)
		rt_free(w_buf);

	if (r_buf)
		rt_free(r_buf);

exit_do_nothing:

	LOG_E("test multi sector write and read failed...");

	return -1;
}

static int test_chip_erase_performance()
{
	rt_device_t nor_dev;
	struct rt_device_blk_geometry geometry;
	rt_uint32_t erase_addr[2];
	rt_uint32_t flash_size, sector_remain, sector_start, sector_num, sector_ret, i;
	rt_uint32_t start_us, us;
	int err = 0;

	LOG_I("test chip erase performance start...");

	/* find nor device */
	nor_dev = rt_device_find(NOR_DEV_NAME);
	if (!nor_dev) {
		LOG_E("failed to find nor device: %s", NOR_DEV_NAME);
		goto exit_do_nothing;
	}

	/* get flash geometry */
	err = rt_device_control(nor_dev, RT_DEVICE_CTRL_BLK_GETGEOME, &geometry);
	if (err) {
		LOG_E("failed to get flash geometry");
		goto exit_do_nothing;
	}

	flash_size = geometry.sector_count * geometry.bytes_per_sector;
	LOG_D("toatal size: %u(Byte), sector count:%u, sector size:%u, block size:%u",
		flash_size, geometry.sector_count,
		geometry.bytes_per_sector, geometry.block_size);

	/* chip erase */
	erase_addr[0] = 0;
	erase_addr[1] = geometry.sector_count;

	start_us = current_us();

	err = rt_device_control(nor_dev, RT_DEVICE_CTRL_BLK_ERASE, erase_addr);
	if (err) {
		LOG_E("failed to erase chip");
		goto exit_do_nothing;
	}

	us = current_us() - start_us;

	LOG_I("chip erase performance(%u KB/s): total byte: %u, consume us: %u",
		(flash_size * 1000) / us, flash_size, us);

	/* read and verify data after chip erase, from address 0 to max address */
	sector_remain = geometry.sector_count;
	sector_start = 0;
	while (sector_remain) {
		sector_num = READ_BUF_SIZE / geometry.bytes_per_sector;
		if (sector_num > sector_remain)
			sector_num = sector_remain;

		rt_memset(r_buf, 0x00, READ_BUF_SIZE);
		sector_ret = rt_device_read(nor_dev, sector_start, r_buf, sector_num);

		if (sector_ret != sector_num) {
			LOG_E("return length error");
			goto exit_do_nothing;
		}

		for (i = 0; i < (sector_num * geometry.bytes_per_sector); i++) {
			if (r_buf[i] != 0xFF) {
				LOG_E("data verify error");
				goto exit_do_nothing;
			}
		}

		sector_start += sector_num;
		sector_remain -= sector_num;
	};

	LOG_I("test chip erase performance pass...");

	return 0;

exit_do_nothing:

	LOG_E("test chip erase performance failed...");

	return -1;
}

static int test_sector_erase_write_read_performance()
{
	rt_device_t nor_dev;
	struct rt_device_blk_geometry geometry;
	rt_uint32_t erase_addr[2];
	rt_uint32_t random, i, sector_start, sector_ret;
	rt_uint32_t start_us, us;
	int err = 0;

	LOG_I("test sector write and read performance start...");

	/* find nor device */
	nor_dev = rt_device_find(NOR_DEV_NAME);
	if (!nor_dev) {
		LOG_E("failed to find nor device: %s", NOR_DEV_NAME);
		goto exit_do_nothing;
	}

	/* get flash geometry */
	err = rt_device_control(nor_dev, RT_DEVICE_CTRL_BLK_GETGEOME, &geometry);
	if (err) {
		LOG_E("failed to get flash geometry");
		goto exit_do_nothing;
	}

	sector_start = random2(geometry.sector_count);
	random = random2(256);

	/* erase */
	erase_addr[0] = sector_start;
	erase_addr[1] = sector_start + 1;

	start_us = current_us();

	err = rt_device_control(nor_dev, RT_DEVICE_CTRL_BLK_ERASE, erase_addr);
	if (err) {
		LOG_E("failed to erase nor flash");
		goto exit_do_nothing;
	}

	us = current_us() - start_us;

	LOG_I("sector erase performance(%u KB/s): total byte: %u, consume us: %u",
		(geometry.bytes_per_sector * 1000) / us,
		geometry.bytes_per_sector, us);

	/* read and verify data after erase */
	rt_memset(r_buf, 0x00, geometry.bytes_per_sector);
	sector_ret = rt_device_read(nor_dev, sector_start, r_buf, 1);

	if (sector_ret != 1) {
		LOG_E("return length error");
		goto exit_do_nothing;
	}

	for (i = 0; i < geometry.bytes_per_sector; i++)
		if (r_buf[i] != 0xFF) {
			LOG_E("data verify error");
			goto exit_do_nothing;
		}

	/* write */
	for (i = 0; i < geometry.bytes_per_sector; i++)
		w_buf[i] = ((random + i + i / 256) & 0xFF);

	start_us = current_us();

	sector_ret = rt_device_write(nor_dev, sector_start, w_buf, 1);

	us = current_us() - start_us;

	if (sector_ret != 1) {
		LOG_E("return length error");
		goto exit_do_nothing;
	}

	LOG_I("sector write performance(%u KB/s): total byte: %u, consume us: %u",
		(geometry.bytes_per_sector * 1000) / us,
		geometry.bytes_per_sector, us);

	/* read and verify data after write */
	rt_memset(r_buf, 0x00, geometry.bytes_per_sector);

	start_us = current_us();

	sector_ret = rt_device_read(nor_dev, sector_start, r_buf, 1);

	us = current_us() - start_us;

	if (sector_ret != 1) {
		LOG_E("return length error");
		goto exit_do_nothing;
	}

	for (i = 0; i < geometry.bytes_per_sector; i++)
		if (r_buf[i] != ((random + i + i / 256) & 0xFF)) {
			LOG_E("data verify error");
			goto exit_do_nothing;
		}

	LOG_I("sector read performance(%u KB/s): total byte: %u, consume us: %u",
		(geometry.bytes_per_sector * 1000) / us,
		geometry.bytes_per_sector, us);

	LOG_I("test sector write and read performance pass...");

	return 0;

exit_do_nothing:

	LOG_E("test sector write and read performance failed...");

	return -1;
}

static int test_multi_sector_erase_write_read_performance()
{
	rt_device_t nor_dev;
	struct rt_device_blk_geometry geometry;
	rt_uint32_t erase_addr[2];
	rt_uint32_t random, i, sector_start, sector_num, byte_num, sector_ret;
	rt_uint32_t start_us, us;
	rt_uint8_t *w_buf, *r_buf;
	int err = 0;

	LOG_I("test multi sector write and read performance start...");

	/* find nor device */
	nor_dev = rt_device_find(NOR_DEV_NAME);
	if (!nor_dev) {
		LOG_E("failed to find nor device: %s", NOR_DEV_NAME);
		goto exit_do_nothing;
	}

	/* get flash geometry */
	err = rt_device_control(nor_dev, RT_DEVICE_CTRL_BLK_GETGEOME, &geometry);
	if (err) {
		LOG_E("failed to get flash geometry");
		goto exit_do_nothing;
	}

	sector_start = random2(geometry.sector_count);

	sector_num = random2(MULTI_SECTOR_MAX_HALF);
	sector_num += MULTI_SECTOR_MAX_HALF;

	byte_num = sector_num * geometry.bytes_per_sector;


	/* check if test area out of range */
	if ((sector_start + sector_num) >= geometry.sector_count)
		sector_start = geometry.sector_count - sector_num - 1;

	random = random2(256);

	/* erase */
	erase_addr[0] = sector_start;
	erase_addr[1] = sector_start + sector_num;

	start_us = current_us();

	err = rt_device_control(nor_dev, RT_DEVICE_CTRL_BLK_ERASE, erase_addr);
	if (err) {
		LOG_E("failed to erase nor flash");
		goto exit_do_nothing;
	}

	us = current_us() - start_us;

	LOG_I("multi sector erase performance(%u KB/s): total byte: %u, consume us: %u",
		(byte_num  * 1000) / us, byte_num, us);

	/* malloc buffer */
	w_buf = (rt_uint8_t *)rt_malloc(byte_num);
	if (!w_buf) {
		LOG_E("failed to malloc");
		goto exit_do_nothing;
	}

	r_buf = (rt_uint8_t *)rt_malloc(byte_num);
	if (!r_buf) {
		LOG_E("failed to malloc");
		goto exit_free_buf;
	}

	/* read and verify data after erase */
	rt_memset(r_buf, 0x00, byte_num);
	sector_ret = rt_device_read(nor_dev, sector_start, r_buf, sector_num);

	if (sector_ret != sector_num) {
		LOG_E("return length error");
		goto exit_free_buf;
	}

	for (i = 0; i < byte_num; i++)
		if (r_buf[i] != 0xFF) {
			LOG_E("data verify error");
			goto exit_free_buf;
		}

	/* write */
	for (i = 0; i < byte_num; i++)
		w_buf[i] = ((random + i + i / 256) & 0xFF);

	start_us = current_us();

	sector_ret = rt_device_write(nor_dev, sector_start, w_buf, sector_num);

	us = current_us() - start_us;

	if (sector_ret != sector_num) {
		LOG_E("return length error");
		goto exit_free_buf;
	}

	LOG_I("multi sector write performance(%u KB/s): total byte: %u, consume us: %u",
		(byte_num * 1000) / us, byte_num, us);

	/* read and verify data after write */
	rt_memset(r_buf, 0x00, byte_num);

	start_us = current_us();

	sector_ret = rt_device_read(nor_dev, sector_start, r_buf, sector_num);

	us = current_us() - start_us;

	if (sector_ret != sector_num) {
		LOG_E("return length error");
		goto exit_free_buf;
	}

	for (i = 0; i < byte_num; i++)
		if (r_buf[i] != ((random + i + i / 256) & 0xFF)) {
			LOG_E("data verify error");
			goto exit_free_buf;
		}

	LOG_I("multi sector read performance(%u KB/s): total byte: %u, consume us: %u",
		(byte_num * 1000) / us, byte_num, us);

	/* free buffer */
	if (w_buf)
		rt_free(w_buf);

	if (r_buf)
		rt_free(r_buf);

	LOG_I("test multi sector write and read performance pass...");

	return 0;

exit_free_buf:
	if (w_buf)
		rt_free(w_buf);

	if (r_buf)
		rt_free(r_buf);

exit_do_nothing:

	LOG_E("test multi sector write and read performance failed...");

	return -1;
}

static test_item_t test_item[] = {
	test_chip_erase,
	test_sector_erase_write_read,
	test_address_range,
	test_multi_sector_erase_write_read,
	test_chip_erase_performance,
	test_sector_erase_write_read_performance,
	test_multi_sector_erase_write_read_performance,
};

#ifdef LOMBO_SPI_NOR
static int lombo_nor_device_init()
{
	int err = 0;
	rt_device_t nor_dev;

	LOG_I("test nor init start...");

	/* try to find nor device */
	nor_dev = rt_device_find(NOR_DEV_NAME);

	/* check if nor device has been init */
	if (nor_dev)
		LOG_W("nor device has been init");
	else {
		/* init nor device */
		err = lombo_nor_init(NOR_DEV_NAME);
		if (err) {
			LOG_E("failed to init nor");
			goto exit_do_nothing;
		}
	}

	LOG_I("test nor init pass...");

	return 0;

exit_do_nothing:

	LOG_E("test nor init failed...");

	return -1;
}
#endif /* LOMBO_SPI_NOR */

#ifdef RT_USING_SFUD
static int sfud_nor_device_init()
{
	int err = 0;
	rt_device_t nor_dev;

	LOG_I("test nor init start...");

	/* try to find nor device */
	nor_dev = rt_device_find(NOR_DEV_NAME);

	/* check if nor device has been init */
	if (nor_dev)
		LOG_W("nor device has been init");
	else {
		/* init nor device */
		err = !lombo_sfud_flash_probe(NOR_DEV_NAME);
		if (err) {
			LOG_E("failed to probe sfud flash");
			goto exit_do_nothing;
		}
	}

	LOG_I("test nor init pass...");

	return 0;

exit_do_nothing:

	LOG_E("test nor init failed...");

	return -1;
}
#endif

static void spi_nor_test_thread(void *param)
{
	int i, err = 0;
	rt_device_t nor_dev;
	rt_uint32_t test_target = (rt_uint32_t)param;

	LOG_I("spi nor test start...");

	/* check if flash has been init */
	nor_dev = rt_device_find(NOR_DEV_NAME);
	if (nor_dev)
		LOG_W("spi device has been register");
	else {
		/* init flash */
#ifdef LOMBO_SPI_NOR
		if (TEST_LOMBO_NOR == test_target)
			lombo_nor_device_init();
#endif
#ifdef RT_USING_SFUD
		if (TEST_SFUD_NOR == test_target)
			sfud_nor_device_init();
#endif

		nor_dev = rt_device_find(NOR_DEV_NAME);
		if (!nor_dev) {
			LOG_E("failed to init flash");
			goto exit_err;
		}
	}

	/* init and open nor device */
	err = rt_device_init(nor_dev);
	if (err) {
		LOG_E("failed to init nor device");
		goto exit_err;
	}

	err = rt_device_open(nor_dev, RT_DEVICE_OFLAG_RDWR);
	if (err) {
		LOG_E("failed to open nor device");
		goto exit_err;
	}

	/* check if every test item pass */
	for (i = 0; i < (sizeof(test_item) / sizeof(test_item[0])); i++) {
		err = test_item[i]();
		if (err)
			goto exit_err;
	}

	/* close nor device */
	rt_device_close(nor_dev);

	LOG_I("spi nor test pass !!!");

	return;

exit_err:
	if (nor_dev)
		rt_device_close(nor_dev);

	LOG_E("spi nor test failed !!!");
}

long test_spi_nor(int argc, char **argv)
{
	rt_thread_t tid;
	rt_uint32_t test_target, support = 0;

	if (argc < 3) {
		LOG_W("take parameter: [eznor] for lombo nor driver test, %s",
			"[sfud] for sfud nor driver test");
		return -1;
	}

	if (0 == strcmp(argv[2], "eznor"))
		test_target = TEST_LOMBO_NOR;
	else if (0 == strcmp(argv[2], "sfud"))
		test_target = TEST_SFUD_NOR;
	else {
		LOG_W("take parameter: [eznor] for lombo nor driver test, %s",
			"[sfud] for sfud nor driver test");
		return -1;
	}

#ifdef LOMBO_SPI_NOR
	support |= TEST_LOMBO_NOR;
#endif

#ifdef RT_USING_SFUD
	support |= TEST_SFUD_NOR;
#endif

	if (!(test_target & support)) {
		LOG_E("unsupport %s test", argv[2]);
		return -1;
	}

	/* create spi nor test thread */
	tid = rt_thread_create("nor_test", spi_nor_test_thread, (void *)test_target,
		2048, 20, 20);

	if (!tid) {
		LOG_E("fail to create spi nor test thread");
		return -RT_ENOMEM;
	}

	/* start spi nor test thread */
	rt_thread_startup(tid);

	return 0;
}

