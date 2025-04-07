/*
 * by24c.c - BY24C I2C EEPROM driver code for LomboTech
 * BY24C I2C EEPROM operation
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
#include <csp.h>

#define DBG_SECTION_NAME	"BY24C"
#define DBG_LEVEL		DBG_LOG
#include <debug.h>
#include "board.h"

#define DEV_IS_BY24C16A
#include "by24c.h"

#ifdef msg
#undef msg
#endif

#define ENABLE_TRACE	0

#if (ENABLE_TRACE == 1)
#define PRT_TRACE_BEGIN(fmt, ...)		\
	 LOG_D("|B|"fmt, ##__VA_ARGS__)
#define PRT_TRACE_END(fmt, ...)			\
	 LOG_D("|E|"fmt, ##__VA_ARGS__)
#else

#define PRT_TRACE_BEGIN(fmt, ...)	do { } while (0)
#define PRT_TRACE_END(fmt, ...)	do { } while (0)

#endif

struct rt_i2c_bus_device *i2c_bus_dev = RT_NULL;

u32 by24c_init(void)
{
	char i2c_name[6] = {0};

	PRT_TRACE_BEGIN("");
	rt_sprintf(i2c_name, "%s%d", "i2c", RT_I2C_BY24C_HOST);

	i2c_bus_dev = rt_i2c_bus_device_find(i2c_name);

	if (RT_NULL == i2c_bus_dev) {
		LOG_E("can't find bus dev \"%s\"", i2c_name);
		return -RT_EINVAL;
	}

	PRT_TRACE_END("%s", i2c_name);
	return RT_EOK;
}

u32 by24c_sw_reset(void)
{
	u32 ret = 0;

	return ret;
}

u32 by24c_byte_write(u8 page_addr, u8 col_addr, u8 write_data)
{
	u32 word_addr = 0;
	u8 msgbuf0[2] = {0};
	u8 msgbuf1[1] = {0};
	u32 ret = 0;

	struct rt_i2c_msg msg[2] = {
		{
			BY24C_TAR_ADDR,		/* addr */
			0,			/* flags */
			0,			/* len */
			NULL,			/* buf */
		}, {
			BY24C_TAR_ADDR,		/* addr */
			0,			/* flags */
			1,			/* len */
			NULL,			/* buf */
		},
	};

	PRT_TRACE_BEGIN("page_addr=%d,col_addr=%d,write_data=0x%02x",
			page_addr, col_addr, write_data);

	word_addr = (page_addr << BY24C_PAGE_SHIFT) | col_addr;

	msg[0].buf = msgbuf0;

#ifdef DEV_IS_BY24C64A
	msgbuf0[0] = *((u8 *)(&word_addr) + 1);
	msgbuf0[1] = *((u8 *)(&word_addr));
	msg[0].len = 2;
#else
	msgbuf0[0] = (u8)word_addr;
	msg[0].len = 1;
#endif

	msg[1].buf = msgbuf1;
	msgbuf1[0] = write_data;

	ret = rt_i2c_transfer(i2c_bus_dev, msg, 2);
	if (ret != 2) {
		ret = __LINE__;
		goto out;
	}

	ret = 0;
	by24c_ack_poll();

out:
	if (ret != 0) {
		LOG_E("ret=%d,page_addr=%d,col_addr=%d,write_data=0x%02x",
			ret, page_addr, col_addr, write_data);
	}
	PRT_TRACE_END("");
	return ret;
}

u32 by24c_page_write(u8 page_addr, u32 page_size, u8 *tx_buf)
{
	u32 word_addr = 0;
	u8 msgbuf0[2] = {0};
	u32 ret = 0;

	struct rt_i2c_msg msg[] = {
		{
			BY24C_TAR_ADDR,		/* addr */
			0,			/* flags */
			0,			/* len */
			NULL,			/* buf */
		}, {
			BY24C_TAR_ADDR,		/* addr */
			0,			/* flags */
			0,			/* len */
			NULL,			/* buf */
		},
	};

	PRT_TRACE_BEGIN("page_addr=%d,page_size=%d,tx_buf=0x%08x",
			page_addr, page_size, tx_buf);

	word_addr = page_addr << BY24C_PAGE_SHIFT;

	msg[0].buf = msgbuf0;

#ifdef I2C_DEV_IS_BY24C64A
	msgbuf0[0] = *((u8 *)(&word_addr) + 1);
	msgbuf0[1] = *((u8 *)(&word_addr));
	msg[0].len = 2;
#else
	msgbuf0[0] = (u8)word_addr;
	msg[0].len = 1;
#endif

	msg[1].buf = tx_buf;
	msg[1].len = page_size;

	ret = rt_i2c_transfer(i2c_bus_dev, msg, 2);
	if (ret != 2) {
		ret = __LINE__;
		goto out;
	}

	ret = 0;

	by24c_ack_poll();

out:
	if (ret != 0) {
		LOG_E("ret=%d,page_addr=%d,page_size=%d,tx_buf=0x%x",
			ret, page_addr, page_size, tx_buf);
	}
	PRT_TRACE_END("");
	return ret;
}

u32 by24c_ack_poll(void)
{
	u8 msgbuf0[1] = {0};
	int timeout = 100;
	u32 ret = 0;

	struct rt_i2c_msg msg[] = {
		{
			BY24C_TAR_ADDR,		/* addr */
			RT_I2C_RD,		/* flags */
			1,			/* len */
			NULL,			/* buf */
		},
	};

	PRT_TRACE_BEGIN("");

	msg[0].buf = msgbuf0;

	while (timeout > 0) {
		ret = rt_i2c_transfer(i2c_bus_dev, msg, 1);
		if (ret == 1) {
			ret = 0;
			break;
		}
		udelay(200);
		timeout--;
	}

	PRT_TRACE_END("");
	return ret;
}

u32 by24c_random_read(u8 page_addr, u8 col_addr, u8 *read_data)
{
	u32 word_addr = 0;
	u8 msgbuf0[2] = {0};
	u8 msgbuf1[1] = {0};
	u32 ret = 0;

	struct rt_i2c_msg msg[2] = {
		{
			BY24C_TAR_ADDR,		/* addr */
			0,			/* flags */
			0,			/* len */
			NULL,			/* buf */
		}, {
			BY24C_TAR_ADDR,		/* addr */
			RT_I2C_RD,		/* flags */
			1,			/* len */
			NULL,			/* buf */
		},
	};

	PRT_TRACE_BEGIN("page_addr=%d,col_addr=%d,read_data=0x%08x",
		page_addr, col_addr, read_data);

	word_addr = (page_addr << BY24C_PAGE_SHIFT) | col_addr;

	msg[0].buf = msgbuf0;

#ifdef DEV_IS_BY24C64A
	msgbuf0[0] = *((u8 *)(&word_addr) + 1);
	msgbuf0[0] = *((u8 *)(&word_addr));
	msg[0].len = 2;
#else
	msgbuf0[0] = (u8)word_addr;
	msg[0].len = 1;
#endif

	msg[1].buf = msgbuf1;

	ret = rt_i2c_transfer(i2c_bus_dev, msg, 2);
	if (ret != 2) {
		ret = __LINE__;
		goto out;
	}

	ret = 0;

	*read_data = msgbuf1[0];

out:
	if (ret != 0) {
		LOG_E("ret=%d,page_addr=%d,col_addr=%d,*read_data=%02x",
			ret, page_addr, col_addr, *read_data);
	}
	PRT_TRACE_END("*read_data=%d", *read_data);
	return ret;
}

u32 by24c_sequence_read(u8 page_addr, u8 col_addr, u32 data_len, u8 *rx_buf)
{
	u32 word_addr = 0;
	u8 msgbuf0[2] = {0};
	u32 ret = 0;

	struct rt_i2c_msg msg[] = {
		{
			BY24C_TAR_ADDR,		/* addr */
			0,			/* flags */
			0,			/* len */
			NULL,			/* buf */
		}, {
			BY24C_TAR_ADDR,		/* addr */
			RT_I2C_RD,		/* flags */
			0,			/* len */
			NULL,			/* buf */
		},
	};

	PRT_TRACE_BEGIN("page_addr=%d,col_addr=%d,data_len=%d,rx_buf=0x%08x",
			page_addr, col_addr, data_len, rx_buf);

	word_addr = (page_addr << BY24C_PAGE_SHIFT) | col_addr;

	msg[0].buf = msgbuf0;

#ifdef I2C_DEV_IS_BY24C64A
	msgbuf0[0] = *((u8 *)(&word_addr) + 1);
	msgbuf0[1] = *((u8 *)(&word_addr));
	msg[0].len = 2;
#else
	msgbuf0[0] = (u8)word_addr;
	msg[0].len = 1;
#endif

	msg[1].buf = rx_buf;
	msg[1].len = data_len;

	ret = rt_i2c_transfer(i2c_bus_dev, msg, 2);
	if (ret != 2) {
		ret = __LINE__;
		goto out;
	}

	ret = 0;

out:
	if (ret != 0) {
		LOG_E("ret=%d,page_addr=%d,col_addr=%d,data_len=%d,rx_buf=0x%08x",
			ret, page_addr, col_addr, data_len, rx_buf);
	}
	PRT_TRACE_END("");
	return ret;
}

