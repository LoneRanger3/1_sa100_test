/*
 * sdc_fs_test.c - mmcsd filesystem level test driver
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
#include <dfs_posix.h>
#include <csp.h>

#include <drivers/mmcsd_core.h>

#include "sdc_test.h"

#define DEFAULT_PATH			"/mnt/sdcard/fs_test"
#define DEFAULT_NAME_PRE		"fs_test"

/* consistent with the sdc driver */
#undef SECTOR_SIZE
#define SECTOR_SIZE			(512)

/* file over 2G may seek failed */
#define MAX_FILE_SIZE			(512 * 1024 * 1024UL)

/* max file name length */
#define MAX_FILENAME_LEN		32

/* max number of file can store on the card at one time in aging test */
#define MAX_FILE_COUNT			256

/* max disk used ratio in aging test */
#define AGING_USED_RATIO_MAX		80

/* buffer size in aging test, unit: sector */
#define AGING_SECTOR_OF_BUF		4

/* buffer size in aging test, unit: byte */
#define AGING_BUF_SIZE			(AGING_SECTOR_OF_BUF * SECTOR_SIZE)

/* buffer size in performance test, unit: sector */
#define PERF_SECTOR_OF_BUF		4

/* buffer size in performance test, unit: byte */
#define PERF_BUF_SIZE			(PERF_SECTOR_OF_BUF * SECTOR_SIZE)

typedef int (*test_item_t)();
typedef char (file_name_t)[MAX_FILENAME_LEN];

/* in order to create next file index */
static u32 file_index;

/* get current system time, unix: ms */
static inline u32 current_ms()
{
	return rt_tick_get() * 1000 / RT_TICK_PER_SECOND;
}

/* get next file index */
static u32 next_file_index()
{
	u32 result;

	result = file_index % 1000;
	file_index++;

	return result;
}

/* create filename */
static int next_filename(char *buf)
{
	if (!buf) {
		LOG_E("invalid buffer");
		return -1;
	}

	rt_sprintf(buf, "%s/%s%03u", DEFAULT_PATH,
		DEFAULT_NAME_PRE, next_file_index());

	LOG_D("create filename: %s", buf);

	return 0;
}

/* get disk information, and return used ratio of disk */
static int get_disk_info(char *path)
{
	struct statfs disk_info;
	u32 used_block, used_ratio;
	int err = 0;

	err = statfs(path, &disk_info);
	if (err) {
		LOG_E("failed to get disk infomation");
		goto exit_do_nothing;
	}

	used_block = disk_info.f_blocks - disk_info.f_bfree;
	used_ratio = (used_block * 100) / disk_info.f_blocks;

	LOG_D("block size = %u, total blocks = %u, free blocks = %u, used ratio = %u",
		disk_info.f_bsize, disk_info.f_blocks,
		disk_info.f_bfree, used_ratio);

	return used_ratio;

exit_do_nothing:
	return -1;
}

#if FS_MAKE_FILESYSTEM_TEST
/* make filesystem test */
static int test_mkfs()
{
	int err;

	LOG_I("test make filesystem start...");

	/* make filesystem */
	err = dfs_mkfs("elm", DEFAULT_DEV_NAME);
	if (err) {
		LOG_E("failed to make filesystem");
		goto exit_do_nothing;
	}

	LOG_I("test make filesystem pass...");

	return 0;

exit_do_nothing:
	LOG_E("test make filesystem failed...");

	return -1;
}
#endif

/* make directory test */
static int test_mkdir()
{
	int err;

	LOG_I("test make directory start...");

	/* make filesystem */
	rmdir(DEFAULT_PATH);
	err = mkdir(DEFAULT_PATH, 0);
	if (err) {
		LOG_E("failed to make directory");
		goto exit_do_nothing;
	}

	LOG_I("test make directory pass...");

	return 0;

exit_do_nothing:
	LOG_E("test make directory failed...");

	return -1;
}

/* basic write and read test */
static int test_basic_access()
{
	int fd, err;
	char buf[32], filename[32];
	char *string = "basic access test";
	u32 ret_len, str_len = strlen(string);

	LOG_I("test basic access start...");

	/* create file name */
	rt_memset(filename, 0x00, sizeof(filename));
	err = next_filename(filename);
	if (err) {
		LOG_E("failed to create filename");
		goto exit_do_nothing;
	};

	/* open file */
	fd = open(filename, O_WRONLY | O_CREAT);
	if (fd < 0) {
		LOG_E("failed to open file: %s", filename);
		goto exit_do_nothing;
	}

	/* write file */
	ret_len = write(fd, string, str_len);
	if (ret_len != str_len) {
		LOG_E("failed to write");
		goto exit_do_nothing;
	}

	/* close file */
	close(fd);

	/* reopen file */
	fd = open(filename, O_RDONLY);
	if (fd < 0) {
		LOG_E("failed to open file: %s", filename);
		goto exit_do_nothing;
	}

	/* read file */
	rt_memset(buf, 0x00, sizeof(buf));
	ret_len = read(fd, buf, str_len);
	if (ret_len != str_len) {
		LOG_E("failed to read");
		goto exit_do_nothing;
	}

	/* close file */
	close(fd);

	/* data verify */
	err = strcmp(string, buf);
	if (err) {
		LOG_E("data verify error");
		goto exit_do_nothing;
	}

	LOG_I("test basic access(file: %s) pass...", filename);

	return 0;

exit_do_nothing:
	LOG_E("test basic access(file: %s) failed...", filename);

	return -1;
}

/* sequence write performance test */
static int test_sequence_write_performance()
{
	char filename[MAX_FILENAME_LEN];
	u8 *buf;
	u32 fd, buf_blk, total_blk;
	u32 loop, i, ret_len;
	u32 start_ms, end_ms, ms, kb, perf;

	LOG_I("test sequence write performance start...");

	buf_blk = PERF_SECTOR_OF_BUF;
	total_blk = MAX_FILE_SIZE / SECTOR_SIZE;

	/* get block request loop */
	loop = total_blk / buf_blk;
	if (total_blk % buf_blk)
		LOG_W("total_blk(%u) can not be divisible by buf_blk(%u), loop=%u",
			total_blk, buf_blk, loop);

	buf = (u8 *)rt_malloc(PERF_BUF_SIZE);
	if (!buf) {
		LOG_E("failed to malloc");
		return -1;
	}
	rt_memset(buf, 0xA3, (buf_blk * SECTOR_SIZE));

	LOG_D("test sequence write performance: total_blk:%u, buf_blk:%u, loop:%u",
		total_blk, buf_blk, loop);

	next_filename(filename);
	fd = open(filename, O_WRONLY | O_CREAT);
	if (fd < 0) {
		LOG_E("failed to open file: %s", filename);
		goto exit_free_buf;
	}

	barrier();
	start_ms = current_ms();

	/* write operation */
	for (i = 0; i < loop; i++) {
		ret_len = write(fd, buf, PERF_BUF_SIZE);
		if (ret_len != PERF_BUF_SIZE) {
			LOG_E("failed to write");
			goto exit_free_buf;
		}
	}

	end_ms = current_ms();
	barrier();

	close(fd);
	rt_free(buf);

	/* calculate write performance */
	ms = end_ms - start_ms;
	kb = (loop * buf_blk * SECTOR_SIZE) / 1024;
	perf = (kb * 1000) / ms;

	LOG_D("sequence write byte:%u, consume ms:%u",
		(loop * buf_blk * SECTOR_SIZE), ms);
	LOG_I("sequence write performance: %u KB/s", perf);

	LOG_I("test sequence write performance pass...");

	return 0;

exit_free_buf:
	rt_free(buf);

	LOG_I("test sequence write performance failed...");

	return -1;
}

/* sequence read performance test */
static int test_sequence_read_performance()
{
	char filename[MAX_FILENAME_LEN];
	u8 *buf;
	u32 fd, buf_blk, total_blk;
	u32 loop, i, ret_len;
	u32 start_ms, end_ms, ms, kb, perf;

	LOG_I("test sequence read performance start...");

	buf_blk = PERF_SECTOR_OF_BUF;
	total_blk = MAX_FILE_SIZE / SECTOR_SIZE;

	/* get block request loop */
	loop = total_blk / buf_blk;
	if (total_blk % buf_blk)
		LOG_W("total_blk(%u) can not be divisible by buf_blk(%u), loop=%u",
			total_blk, buf_blk, loop);

	buf = (u8 *)rt_malloc(PERF_BUF_SIZE);
	if (!buf) {
		LOG_E("failed to malloc");
		return -1;
	}
	rt_memset(buf, 0xA3, (buf_blk * SECTOR_SIZE));

	LOG_D("test sequence read performance: total_blk:%u, buf_blk:%u, loop:%u",
		total_blk, buf_blk, loop);

	/* we should fill the file first */
	next_filename(filename);
	fd = open(filename, O_WRONLY | O_CREAT);
	if (fd < 0) {
		LOG_E("failed to open file: %s", filename);
		goto exit_free_buf;
	}

	for (i = 0; i < loop; i++) {
		ret_len = write(fd, buf, PERF_BUF_SIZE);
		if (ret_len != PERF_BUF_SIZE) {
			LOG_E("failed to write");
			goto exit_free_buf;
		}
	}

	close(fd);


	/* reopen file and read */
	fd = open(filename, O_RDONLY);
	if (fd < 0) {
		LOG_E("failed to open file: %s", filename);
		goto exit_free_buf;
	}

	barrier();
	start_ms = current_ms();

	for (i = 0; i < loop; i++) {
		ret_len = read(fd, buf, PERF_BUF_SIZE);
		if (ret_len != PERF_BUF_SIZE) {
			LOG_E("failed to read");
			goto exit_free_buf;
		}
	}

	end_ms = current_ms();
	barrier();

	close(fd);
	rt_free(buf);

	/* calculate read performance */
	ms = end_ms - start_ms;
	kb = (loop * buf_blk * SECTOR_SIZE) / 1024;
	perf = (kb  * 1000) / ms;

	LOG_D("sequence read byte:%u, consume ms:%u",
		(loop * buf_blk * SECTOR_SIZE), ms);
	LOG_I("sequence read performance: %u KB/s", perf);

	LOG_I("test sequence read performance pass...");

	return 0;

exit_free_buf:
	rt_free(buf);

	LOG_I("test sequence read performance failed...");

	return -1;
}

/* sequence access with data verify */
static int sequence_access(int fd)
{
	char *write_buf, *read_buf;
	u32 i, j, random, access_cnt, cur_ms, last_ms;
	int ret_len, err = 0;

	LOG_D("sequence access start...");

	write_buf = rt_malloc(AGING_BUF_SIZE);
	if (!write_buf) {
		LOG_E("failed to malloc");
		goto exit_do_nothing;
	}

	read_buf = rt_malloc(AGING_BUF_SIZE);
	if (!read_buf) {
		LOG_E("failed to malloc");
		goto exit_free_buf;
	}

	random = random2(256);
	access_cnt = (u32)(MAX_FILE_SIZE / AGING_BUF_SIZE);

	LOG_D("sequence access, random: %u, total loop: %u", random, access_cnt);

	/* sequence write */
	last_ms = current_ms();
	for (i = 0; i < access_cnt; i++) {
		/* heart beat */
		cur_ms = current_ms();
		if ((cur_ms - last_ms) >= 5000) {
			LOG_D("sequence write: current second(%u), current loop: %u",
				cur_ms / 1000, i);
			last_ms = cur_ms;
		}

		rt_memset(write_buf, 0x00, AGING_BUF_SIZE);

		/* fill write buffer with regular dummy data */
		for (j = 0; j < AGING_BUF_SIZE; j++)
			write_buf[j] = (i + j + j / 256 + random) & 0xFF;

		/* write file */
		ret_len = write(fd, write_buf, AGING_BUF_SIZE);
		if (ret_len != AGING_BUF_SIZE) {
			LOG_E("failed to write");
			goto exit_free_buf;
		}
	}

	/* seek to position 0 */
	err = lseek(fd, 0, SEEK_SET);
	if (-1 == err) {
		LOG_E("lseek error");
		goto exit_free_buf;
	}

	/* sequence read and data verify */
	last_ms = current_ms();
	for (i = 0; i < access_cnt; i++) {
		/* heart beat */
		cur_ms = current_ms();
		if ((cur_ms - last_ms) >= 5000) {
			LOG_D("sequence read: current second(%u), current loop: %u",
				cur_ms / 1000, i);
			last_ms = cur_ms;
		}

		rt_memset(read_buf, 0x00, AGING_BUF_SIZE);

		/* read file */
		ret_len = read(fd, read_buf, AGING_BUF_SIZE);
		if (ret_len != AGING_BUF_SIZE) {
			LOG_E("failed to read");
			goto exit_free_buf;
		}

		/* data verify */
		for (j = 0; j < AGING_BUF_SIZE; j++) {
			if (read_buf[j] != ((i + j + j / 256 + random) & 0xFF)) {
				LOG_E("data verify error");
				goto exit_free_buf;
			}
		}
	}

	if (write_buf)
		rt_free(write_buf);

	if (read_buf)
		rt_free(read_buf);

	LOG_D("sequence access finish...");

	return 0;

exit_free_buf:
	if (write_buf)
		rt_free(write_buf);

	if (read_buf)
		rt_free(read_buf);

exit_do_nothing:
	LOG_E("sequence access failed...");

	return -1;
}

#if FS_SEQUENCE_AGING_TEST
/* sequence access aging test */
static int test_sequence_access_aging()
{
	u32 i, file_cnt, test_cnt;
	int fd, err, used_ratio = 0;
	char *cur_file = RT_NULL;
	file_name_t *file_list;

	LOG_I("aging test sequence access start...");

	/* malloc file list */
	file_list = (file_name_t *)rt_malloc(sizeof(file_name_t) * MAX_FILE_COUNT);
	if (!file_list) {
		LOG_E("failed to malloc");
		goto exit_do_nothing;
	}

	rt_memset(file_list, 0, sizeof(file_list));

	file_cnt = 0;
	test_cnt = 0;

	/* sequence access till reach max used ratio */
	while (1) {
		LOG_I("the %d times of sequence access aging", test_cnt + 1);

		if (file_cnt >= MAX_FILE_COUNT) {
			LOG_E("file count overflow");
			goto exit_free_file_list;
		}

		err = next_filename(file_list[file_cnt]);
		if (err) {
			LOG_E("failed to create filename");
			goto exit_free_file_list;
		}
		cur_file = file_list[file_cnt];

		/* check if reach max used ratio */
		used_ratio = get_disk_info(EXT_SDCARD_PATH);
		if (used_ratio < 0) {
			LOG_E("failed to get disk info");
			goto exit_free_file_list;
		}

		/* break when reach max used ratio */
		if (used_ratio >= AGING_USED_RATIO_MAX) {
			LOG_D("sequence access aging reach max used ratio");
			break;
		}

		/* open -> test -> close */
		fd = open(cur_file, O_RDWR | O_CREAT | O_TRUNC, 0666);
		if (fd < 0) {
			LOG_E("failed to open file: %s", cur_file);
			goto exit_free_file_list;
		}

		err = sequence_access(fd);
		if (err) {
			LOG_E("sequence access failed");
			goto exit_close_file;
		}

		close(fd);

		file_cnt++;
		test_cnt++;
	}

	LOG_D("aging test sequence access reach max used ratio, file_cnt = %d", file_cnt);

	/* now delete 1 file, then create 1 file and test, used ratio keep max ratio */
	while (1) {
		for (i = 0; i < file_cnt; i++) {
			LOG_I("the %d times of sequence access aging", test_cnt + 1);

			/* delete 1 file */
			err = unlink(file_list[i]);
			if (err) {
				LOG_E("unlink failed");
				goto exit_free_file_list;
			}

			/* create file name */
			err = next_filename(file_list[i]);
			if (err) {
				LOG_E("failed to create filename");
				goto exit_free_file_list;
			}
			cur_file = file_list[i];

			/* open -> test -> close */
			fd = open(cur_file, O_RDWR | O_CREAT | O_TRUNC, 0666);
			if (fd < 0) {
				LOG_E("failed to open file: %s", cur_file);
				goto exit_free_file_list;
			}

			err = sequence_access(fd);
			if (err) {
				LOG_E("sequence access failed");
				goto exit_close_file;
			}

			close(fd);

			test_cnt++;
		}
	}

	/* free file list */
	if (file_list)
		rt_free(file_list);

	LOG_I("aging test sequence access pass...");

	return 0;

exit_close_file:
	if (fd >= 0)
		close(fd);

exit_free_file_list:
	if (file_list)
		rt_free(file_list);

exit_do_nothing:
	LOG_E("aging test sequence access failed...");

	return -1;
}
#endif

/* random write performance test */
static int test_random_write_performance()
{
	char filename[MAX_FILENAME_LEN];
	u8 *buf;
	u32 fd, buf_blk, total_blk, addr, cur_ms, last_ms;
	u32 loop, i, ret_len;
	u32 start_ms, end_ms, ms, kb, perf;

	LOG_I("test random write performance start...");

	buf_blk = PERF_SECTOR_OF_BUF;
	total_blk = MAX_FILE_SIZE / SECTOR_SIZE;

	/* get block request loop */
	loop = total_blk / buf_blk;
	if (total_blk % buf_blk)
		LOG_W("total_blk(%u) can not be divisible by buf_blk(%u), loop=%u",
			total_blk, buf_blk, loop);

	buf = (u8 *)rt_malloc(PERF_BUF_SIZE);
	if (!buf) {
		LOG_E("failed to malloc");
		return -1;
	}
	rt_memset(buf, 0xA3, (buf_blk * SECTOR_SIZE));

	LOG_D("test random write performance: total_blk:%u, buf_blk:%u, loop:%u",
		total_blk, buf_blk, loop);

	/* we should fill the file first */
	next_filename(filename);
	fd = open(filename, O_WRONLY | O_CREAT);
	if (fd < 0) {
		LOG_E("failed to open file: %s", filename);
		goto exit_free_buf;
	}

	rt_memset(buf, 0xA3, PERF_BUF_SIZE);
	last_ms = current_ms();
	for (i = 0; i < loop; i++) {
		/* heart beat */
		cur_ms = current_ms();
		if ((cur_ms - last_ms) >= 5000) {
			LOG_D("random write: current second(%u), current loop: %u",
				cur_ms / 1000, i);
			last_ms = cur_ms;
		}

		ret_len = write(fd, buf, PERF_BUF_SIZE);
		if (ret_len != PERF_BUF_SIZE) {
			LOG_E("failed to write");
			goto exit_free_buf;
		}
	}

	close(fd);

	/* reopen file and random write*/
	fd = open(filename, O_WRONLY | O_CREAT);
	if (fd < 0) {
		LOG_E("failed to open file: %s", filename);
		goto exit_free_buf;
	}

	barrier();
	start_ms = current_ms();

	/* write operation */
	for (i = 0; i < loop; i++) {
		addr = random2(MAX_FILE_SIZE - PERF_BUF_SIZE);
		addr &= ~0x03;
		lseek(fd, addr, SEEK_SET);

		ret_len = write(fd, buf, PERF_BUF_SIZE);
		if (ret_len != PERF_BUF_SIZE) {
			LOG_E("failed to write");
			goto exit_free_buf;
		}
	}

	end_ms = current_ms();
	barrier();

	close(fd);
	rt_free(buf);

	/* calculate write performance */
	ms = end_ms - start_ms;
	kb = (loop * buf_blk * SECTOR_SIZE) / 1024;
	perf = (kb  * 1000) / ms;

	LOG_D("random write byte:%u, consume ms:%u",
		(loop * buf_blk * SECTOR_SIZE), ms);
	LOG_I("random write performance: %u KB/s", perf);

	LOG_I("test random write performance pass...");

	return 0;

exit_free_buf:
	rt_free(buf);

	LOG_I("test random write performance failed...");

	return -1;
}

/* random read performance test */
static int test_random_read_performance()
{
	char filename[MAX_FILENAME_LEN];
	u8 *buf;
	u32 fd, buf_blk, total_blk, addr, cur_ms, last_ms;
	u32 loop, i, ret_len;
	u32 start_ms, end_ms, ms, kb, perf;

	LOG_I("test random read performance start...");

	buf_blk = PERF_SECTOR_OF_BUF;
	total_blk = MAX_FILE_SIZE / SECTOR_SIZE;

	/* get block request loop */
	loop = total_blk / buf_blk;
	if (total_blk % buf_blk)
		LOG_W("total_blk(%u) can not be divisible by buf_blk(%u), loop=%u",
			total_blk, buf_blk, loop);

	buf = (u8 *)rt_malloc(PERF_BUF_SIZE);
	if (!buf) {
		LOG_E("failed to malloc");
		return -1;
	}
	rt_memset(buf, 0xA3, (buf_blk * SECTOR_SIZE));

	LOG_D("test random read performance: total_blk:%u, buf_blk:%u, loop:%u",
		total_blk, buf_blk, loop);

	/* we should fill the file first */
	next_filename(filename);
	fd = open(filename, O_WRONLY | O_CREAT);
	if (fd < 0) {
		LOG_E("failed to open file: %s", filename);
		goto exit_free_buf;
	}

	rt_memset(buf, 0xA3, PERF_BUF_SIZE);
	last_ms = current_ms();
	for (i = 0; i < loop; i++) {
		/* heart beat */
		cur_ms = current_ms();
		if ((cur_ms - last_ms) >= 5000) {
			LOG_D("random read: current second(%u), current loop: %u",
				cur_ms / 1000, i);
			last_ms = cur_ms;
		}

		ret_len = write(fd, buf, PERF_BUF_SIZE);
		if (ret_len != PERF_BUF_SIZE) {
			LOG_E("failed to write");
			goto exit_free_buf;
		}
	}

	close(fd);

	/* reopen file and random read*/
	fd = open(filename, O_RDONLY | O_CREAT);
	if (fd < 0) {
		LOG_E("failed to open file: %s", filename);
		goto exit_free_buf;
	}

	barrier();
	start_ms = current_ms();

	/* read operation */
	for (i = 0; i < loop; i++) {
		addr = random2(MAX_FILE_SIZE - PERF_BUF_SIZE);
		addr &= ~0x03;
		lseek(fd, addr, SEEK_SET);

		ret_len = read(fd, buf, PERF_BUF_SIZE);
		if (ret_len != PERF_BUF_SIZE) {
			LOG_E("failed to read");
			goto exit_free_buf;
		}
	}

	end_ms = current_ms();
	barrier();

	close(fd);
	rt_free(buf);

	/* calculate read performance */
	ms = end_ms - start_ms;
	kb = (loop * buf_blk * SECTOR_SIZE) / 1024;
	perf = (kb  * 1000) / ms;

	LOG_D("random read byte:%u, consume ms:%u",
		(loop * buf_blk * SECTOR_SIZE), ms);
	LOG_I("random read performance: %u KB/s", perf);
	LOG_I("test random read performance pass...");

	return 0;

exit_free_buf:
	rt_free(buf);

	LOG_I("test random read performance failed...");

	return -1;
}

#if FS_RANDOM_AGING_TEST
/* random access with data verify */
static int random_access(int fd)
{
	char *write_buf, *read_buf;
	u32 i, j, random, addr, access_cnt, cur_ms, last_ms;
	int ret_len, err = 0;

	LOG_D("random access start...");

	write_buf = rt_malloc(AGING_BUF_SIZE);
	if (!write_buf) {
		LOG_E("failed to malloc");
		goto exit_do_nothing;
	}

	read_buf = rt_malloc(AGING_BUF_SIZE);
	if (!read_buf) {
		LOG_E("failed to malloc");
		goto exit_free_buf;
	}

	access_cnt = (u32)(MAX_FILE_SIZE / AGING_BUF_SIZE);

	/* we should fill the file first */
	rt_memset(write_buf, 0xA3, AGING_BUF_SIZE);
	last_ms = current_ms();
	for (i = 0; i < access_cnt; i++) {
		/* heart beat */
		cur_ms = current_ms();
		if ((cur_ms - last_ms) >= 5000) {
			LOG_D("random write: current second(%u), current loop: %u",
				cur_ms / 1000, i);
			last_ms = cur_ms;
		}

		ret_len = write(fd, write_buf, AGING_BUF_SIZE);
		if (ret_len != AGING_BUF_SIZE) {
			LOG_E("failed to write");
			goto exit_free_buf;
		}
	}

	last_ms = current_ms();
	for (i = 0; i < access_cnt; i++) {
		/* heart beat */
		cur_ms = current_ms();
		if ((cur_ms - last_ms) >= 5000) {
			LOG_D("random access: current second(%u), current loop: %u",
				cur_ms / 1000, i);
			last_ms = cur_ms;
		}

		random = random2(256);
		addr = random2(MAX_FILE_SIZE - AGING_BUF_SIZE);
		addr &= ~0x03;

		/* LOG_D("random access: random:%u, addr:%u", random, addr); */

		/* seek to random addr */
		err = lseek(fd, addr, SEEK_SET);
		if (addr != err) {
			LOG_E("lseek error");
			goto exit_free_buf;
		}

		rt_memset(write_buf, 0x00, AGING_BUF_SIZE);

		/* fill write buffer with regular dummy data */
		for (j = 0; j < AGING_BUF_SIZE; j++)
			write_buf[j] = (i + j + j / 256 + random) & 0xFF;

		/* write file */
		ret_len = write(fd, write_buf, AGING_BUF_SIZE);
		if (ret_len != AGING_BUF_SIZE) {
			LOG_E("failed to write");
			goto exit_free_buf;
		}

		/* seek to write start position */
		err = lseek(fd, -AGING_BUF_SIZE, SEEK_CUR);
		if (-1 == err) {
			LOG_E("lseek error");
			goto exit_free_buf;
		}

		rt_memset(read_buf, 0x00, AGING_BUF_SIZE);

		/* read file */
		ret_len = read(fd, read_buf, AGING_BUF_SIZE);
		if (ret_len != AGING_BUF_SIZE) {
			LOG_E("failed to read");
			goto exit_free_buf;
		}

		/* data verify */
		for (j = 0; j < AGING_BUF_SIZE; j++) {
			if (read_buf[j] != ((i + j + j / 256 + random) & 0xFF)) {
				LOG_E("data verify error, addr:%u", addr);
				goto exit_free_buf;
			}
		}
	}

	if (write_buf)
		rt_free(write_buf);

	if (read_buf)
		rt_free(read_buf);

	LOG_D("random access finish...");

	return 0;

exit_free_buf:
	if (write_buf)
		rt_free(write_buf);

	if (read_buf)
		rt_free(read_buf);

exit_do_nothing:
	LOG_E("random access failed...");

	return -1;
}

/* random access aging test */
static int test_random_access_aging()
{
	u32 i, file_cnt, test_cnt;
	int fd, err, used_ratio = 0;
	char *cur_file = RT_NULL;
	file_name_t *file_list;

	LOG_I("aging test random access start...");

	/* malloc file list */
	file_list = (file_name_t *)rt_malloc(sizeof(file_name_t) * MAX_FILE_COUNT);
	if (!file_list) {
		LOG_E("failed to malloc");
		goto exit_do_nothing;
	}

	rt_memset(file_list, 0, sizeof(file_list));

	file_cnt = 0;
	test_cnt = 0;

	/* random access till reach max used ratio */
	while (1) {
		LOG_I("the %d times of random access aging", test_cnt + 1);

		if (file_cnt >= MAX_FILE_COUNT) {
			LOG_E("file count overflow");
			goto exit_free_file_list;
		}

		err = next_filename(file_list[file_cnt]);
		if (err) {
			LOG_E("failed to create filename");
			goto exit_free_file_list;
		}
		cur_file = file_list[file_cnt];

		/* check if reach max used ratio */
		used_ratio = get_disk_info(EXT_SDCARD_PATH);
		if (used_ratio < 0) {
			LOG_E("failed to get disk info");
			goto exit_free_file_list;
		}

		/* break when reach max used ratio */
		if (used_ratio >= AGING_USED_RATIO_MAX) {
			LOG_D("random access aging reach max used ratio");
			break;
		}

		/* open -> test -> close */
		fd = open(cur_file, O_RDWR | O_CREAT | O_TRUNC, 0666);
		if (fd < 0) {
			LOG_E("failed to open file: %s", cur_file);
			goto exit_free_file_list;
		}

		err = random_access(fd);
		if (err) {
			LOG_E("random access failed");
			goto exit_close_file;
		}

		close(fd);

		file_cnt++;
		test_cnt++;
	}

	LOG_D("aging test random access reach max used ratio, file_cnt = %d", file_cnt);

	/* now delete 1 file, then create 1 file and test, used ratio keep max ratio */
	while (1) {
		for (i = 0; i < file_cnt; i++) {
			LOG_I("the %d times of random access aging", test_cnt + 1);

			/* delete 1 file */
			err = unlink(file_list[i]);
			if (err) {
				LOG_E("unlink failed");
				goto exit_free_file_list;
			}

			/* create file name */
			err = next_filename(file_list[i]);
			if (err) {
				LOG_E("failed to create filename");
				goto exit_free_file_list;
			}
			cur_file = file_list[i];

			/* open -> test -> close */
			fd = open(cur_file, O_RDWR | O_CREAT | O_TRUNC, 0666);
			if (fd < 0) {
				LOG_E("failed to open file: %s", cur_file);
				goto exit_free_file_list;
			}

			err = random_access(fd);
			if (err) {
				LOG_E("random access failed");
				goto exit_close_file;
			}

			close(fd);

			test_cnt++;
		}
	}

	/* free file list */
	if (file_list)
		rt_free(file_list);

	LOG_I("aging test random access pass...");

	return 0;

exit_close_file:
	if (fd >= 0)
		close(fd);

exit_free_file_list:
	if (file_list)
		rt_free(file_list);

exit_do_nothing:
	LOG_E("aging test random access failed...");

	return -1;
}
#endif

static test_item_t test_item[] = {
#if FS_MAKE_FILESYSTEM_TEST
	test_mkfs,
#endif
	test_mkdir,
	test_basic_access,
	test_sequence_write_performance,
	test_sequence_read_performance,
	test_random_write_performance,
	test_random_read_performance,
#if FS_SEQUENCE_AGING_TEST
	test_sequence_access_aging,
#endif
#if FS_RANDOM_AGING_TEST
	test_random_access_aging,
#endif
};

/* sd filesystem test thread */
static void sd_fs_test_thread(void *param)
{
	int i, err = 0;

	LOG_I("sd filesystem level test start...");

	/* check if every test item pass */
	for (i = 0; i < (sizeof(test_item) / sizeof(test_item[0])); i++) {
		err = test_item[i]();
		if (err)
			goto exit_do_nothing;
	}

	LOG_I("sd filesystem level test pass !!!");

	return;

exit_do_nothing:

	LOG_I("sd filesystem level test failed !!!");
}

/* to create sdc filesystem test thread */
int sdc_fs_test()
{
	rt_thread_t tid;

	/* create sd block test thread */
	tid = rt_thread_create("sd_ft", sd_fs_test_thread, RT_NULL, 2048, 20, 20);

	if (!tid) {
		LOG_E("failed to create sd filesystem test thread");
		return -RT_ENOMEM;
	}

	/* start sd block test thread */
	rt_thread_startup(tid);

	return 0;
}
