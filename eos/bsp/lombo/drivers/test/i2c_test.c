/*
 * i2c_test.c - I2C driver test code for LomboTech Socs
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

#define DBG_SECTION_NAME	"I2C"
#define DBG_LEVEL	DBG_LOG
#include "debug.h"

#include <rtdevice.h>
#include <string.h>

#include "board.h"
#include "i2c/i2c_devs/by24c.h"

long test_i2c(int argc, char **argv)
{
	u32 *src_buf = NULL;
	u32 *dst_buf = NULL;
	u8 page_addr = 0;
	u8 col_addr = 0;
	u8 test_byte = 0;
	u8 read_byte = 0;
	u32 i;
	u32 ret = 0;

	ret = by24c_init();
	if (ret != 0) {
		ret = __LINE__;
		goto out;
	}

	src_buf = rt_malloc(BY24C_PAGE_SIZE);
	if (src_buf == NULL) {
		ret = __LINE__;
		goto out;
	}

	dst_buf = rt_malloc(BY24C_PAGE_SIZE);
	if (src_buf == NULL) {
		ret = __LINE__;
		goto out;
	}

	page_addr = 0x01;
	col_addr = 0x00;
	test_byte = 0xe7;

	/*
	 * --------------------------------------------------------------------------
	 * data inital
	 * --------------------------------------------------------------------------
	 */
	rt_memset(dst_buf, 0, BY24C_PAGE_SIZE);

	for (i = 0; i < BY24C_PAGE_SIZE / 4; i++)
		src_buf[i] = 0x5ac39600 + i;

	ret = by24c_byte_write(page_addr, col_addr, test_byte);
	if (ret != 0) {
		ret = __LINE__;
		goto out;
	}

	ret = by24c_random_read(page_addr, col_addr, &read_byte);
	if (ret != 0) {
		ret = __LINE__;
		goto out;
	}

	ret = by24c_page_write(page_addr, BY24C_PAGE_SIZE, (u8 *)src_buf);
	if (ret != 0) {
		ret = __LINE__;
		goto out;
	}

	ret = by24c_sequence_read(page_addr, col_addr, BY24C_PAGE_SIZE, (u8 *)dst_buf);
	if (ret != 0) {
		ret = __LINE__;
		goto out;
	}

	ret = rt_memcmp(src_buf, dst_buf, BY24C_PAGE_SIZE);
	if (ret != 0) {
		ret = __LINE__;
		goto out;
	}
	LOG_I("---- i2c pattern ok ----\n");

out:
	if (dst_buf != NULL)
		rt_free(dst_buf);
	if (src_buf != NULL)
		rt_free(src_buf);

	LOG_D("---- i2c pattern end ----\n");
	if (ret != 0) {
		LOG_E("i2c pattern failed ret=%d\n", ret);
		ret = -RT_EINVAL;
	}
	return ret;
}


