/*
 * interface.c - dynamic library interface
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

#define MOD_NAME	"lib"
#include "common.h"
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sizes.h>
#include <dfs_posix.h>
#include "slib.h"

#define TEST_FILE	"/text.txt"

int test_cxx_class(void);

void test_posix_file()
{
	char s[] = "this is a test string!", buffer[32];
	int fd, size;

	LOG("start");

	/* create file and write */
	fd = open(TEST_FILE, O_WRONLY | O_CREAT);
	if (fd >= 0) {
		LOG("open %s success, ret fd %d, write %s to it...", TEST_FILE, fd, s);
		size = write(fd, s, strlen(s));
		if (size < 0)
			LOG("write %s failed", TEST_FILE);
		else
			LOG("write %s success, ret %d", TEST_FILE, size);
		close(fd);
	} else {
		LOG("create %s failed", TEST_FILE);
		return;
	}

	/* check write result */
	fd = open(TEST_FILE, O_RDONLY);
	if (fd >= 0) {
		LOG("open %s success, ret fd %d, read from it...", TEST_FILE, fd);
		size = read(fd, buffer, sizeof(buffer));
		if (size < 0)
			LOG("read %s failed", TEST_FILE);
		else
			LOG("read %s success, ret %d", TEST_FILE, size);

		LOG("get buffer data %s", buffer);
		close(fd);
	} else {
		LOG("open %s failed", TEST_FILE);
		return;
	}

	LOG("end");
}

int lib_test_cxx(void)
{
	LOG("start");

	test_cxx_class();
	test_posix_file();

	/* call static lib function in dynamic lib */
	/* slib_test_thread(); */

	LOG("end");
	return 0;
}

int lib_test_add(int a, int b)
{
	int res;

	LOG("start");

	res = a + b;

	LOG("end");
	return res;
}

