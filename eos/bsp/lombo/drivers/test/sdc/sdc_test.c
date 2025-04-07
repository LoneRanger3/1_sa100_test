/*
 * sdc_test.c - sdc test driver
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

/* create a random value */
u32 random()
{
	return rt_time_get_usec();
}

/* create a random value in range */
u32 random2(u32 range)
{
	return random() % range;
}

/* sdc test entry */
long test_sdc(int argc, char **argv)
{
	long ret = 0;

	if (argc < 3) {
		LOG_W("take parameter: [blk] for block test, [fs] for filesystem test");
		return -1;
	}

	if (!((0 == strcmp(argv[2], "blk")) || (0 == strcmp(argv[2], "fs")))) {
		LOG_W("take parameter: [blk] for block test, [fs] for filesystem test");
		return -1;
	}

	if (0 == strcmp(argv[2], "blk"))
		ret = sdc_blk_test();

	if (0 == strcmp(argv[2], "fs"))
		ret = sdc_fs_test();

	return ret;
}
