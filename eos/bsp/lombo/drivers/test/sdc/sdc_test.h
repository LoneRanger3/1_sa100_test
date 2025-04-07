/*
 * sdc_test.h - head file for sdc test driver
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

#ifndef ___SDC__TEST___H___
#define ___SDC__TEST___H___

#define DEFAULT_DEV_NAME		"sd0"

#define EXT_SDCARD_PATH			"/mnt/sdcard"

#define BLK_ACCESS_AGING_TEST		0

#define FS_MAKE_FILESYSTEM_TEST		0
#define FS_SEQUENCE_AGING_TEST		1
#define FS_RANDOM_AGING_TEST		0

/* create a random value */
u32 random();

/* create a random value in range */
u32 random2(u32 range);

/* sdc block device level test */
int sdc_blk_test();

/* sdc filesystem level test */
int sdc_fs_test();

/* sdc test entry */
long test_sdc(int argc, char **argv);

#endif /* ___SDC__TEST___H___ */
