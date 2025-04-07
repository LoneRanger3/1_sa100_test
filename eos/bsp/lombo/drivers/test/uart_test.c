/*
 * uart_test.c - Uart driver test for LomboTech Socs
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

#define DBG_SECTION_NAME	"UART"
#define DBG_LEVEL	DBG_LOG
#include "debug.h"

#include <rtdevice.h>
#include <string.h>

#define UART_TEST
#define TEST_UART0
/**
 * we don't test the uart1 by default, becuase it was set to a console port.
 * if we want to test this port, we should use the 'release_dev' version.
 */
/* #define TEST_UART1 */
#define TEST_UART2
#define TEST_UART3

#ifdef UART_TEST

struct uart_port {
	char name[RT_NAME_MAX];
	rt_uint16_t oflag;
};

static const char uart_tx_number_data[] = {
	"000000000000000000000000000000000\r\n"
	"111111111111111111111111111111111\r\n"
	"222222222222222222222222222222222\r\n"
	"333333333333333333333333333333333\r\n"
	"444444444444444444444444444444444\r\n"
	"555555555555555555555555555555555\r\n"
	"666666666666666666666666666666666\r\n"
	"777777777777777777777777777777777\r\n"
	"888888888888888888888888888888888\r\n"
	"999999999999999999999999999999999\r\n"
};

static const char uart_tx_char_data[] = {
	"abcdefghijklmnopqrstuvwxyz\r\n"
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ\r\n"
};

static const char uart_tx_clock_data[] = {
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x0d, 0x0a,
	0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
	0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
	0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
	0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x0d, 0x0a,
	0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA,
	0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA,
	0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA,
	0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x0d, 0x0a,
};

static unsigned char uart_tx_mass_data[4096*16];

static struct uart_port uart_port_test[] = {
#ifdef TEST_UART0
	{
		"uart0",
		RT_DEVICE_OFLAG_RDWR
	},
	{
		"uart0",
		RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX
	},
	{
		"uart0",
		RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX | RT_DEVICE_FLAG_INT_TX
	},
	{
		"uart0",
		RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX | RT_DEVICE_FLAG_DMA_TX
	},
#endif
#ifdef TEST_UART1

	{
		"uart1",
		RT_DEVICE_OFLAG_RDWR,
	},
	{
		"uart1",
		RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX | RT_DEVICE_FLAG_INT_TX
	},
	{
		"uart1",
		RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX | RT_DEVICE_FLAG_DMA_TX
	},
#endif
#ifdef TEST_UART2

	{
		"uart2",
		RT_DEVICE_OFLAG_RDWR,
	},
	{
		"uart2",
		RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX | RT_DEVICE_FLAG_INT_TX
	},
	{
		"uart2",
		RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX | RT_DEVICE_FLAG_DMA_TX
	},
#endif
#ifdef TEST_UART3

	{
		"uart3",
		RT_DEVICE_OFLAG_RDWR,
	},
	{
		"uart3",
		RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX | RT_DEVICE_FLAG_INT_TX
	},
	{
		"uart3",
		RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX | RT_DEVICE_FLAG_DMA_TX
	},
#endif
};

rt_err_t uart_tx_test(const char *name, rt_uint16_t oflag)
{
	rt_device_t dev;
	rt_size_t data_size = 0, tx_size = 0;
	rt_err_t ret = RT_EOK;
	int index = 0;

	/* Find the device */
	dev = rt_device_find(name);
	if (dev == RT_NULL) {
		LOG_E("%s not found", name);
		return -RT_ERROR;
	}

	/* Open the device */
	ret = rt_device_open(dev, oflag);
	if (ret != RT_EOK) {
		LOG_E("Failed to open dev:%s with flag:%d", name, oflag);
		return ret;
	}

	/* Test write */
	data_size = sizeof(uart_tx_number_data);
	tx_size = rt_device_write(dev, 0, uart_tx_number_data, data_size);
	if (tx_size != data_size) {
		LOG_E("Tx number test failed, data_size:%d, tx_size:%d",
			data_size, tx_size);
		return -RT_ERROR;
	}

	data_size = sizeof(uart_tx_clock_data);
	tx_size = rt_device_write(dev, 0, uart_tx_clock_data, data_size);
	if (tx_size != data_size) {
		LOG_E("Tx clock test failed, data_size:%d, tx_size:%d",
			data_size, tx_size);
		return -RT_ERROR;
	}

	data_size = sizeof(uart_tx_char_data);
	tx_size = rt_device_write(dev, 0, uart_tx_char_data, data_size);
	if (tx_size != data_size) {
		LOG_E("Tx char test failed, data_size:%d, tx_size:%d",
			data_size, tx_size);
		return -RT_ERROR;
	}

	data_size = sizeof(uart_tx_mass_data);
	memset((void *)uart_tx_mass_data, 0x3B, sizeof(uart_tx_mass_data));
	for (index = 62; index < data_size; index += 62) {
		uart_tx_mass_data[index] = '\r';
		index++;
		uart_tx_mass_data[index] = '\n';
		index++;
	}
	tx_size = rt_device_write(dev, 0, uart_tx_mass_data, data_size);
	if (tx_size != data_size) {
		LOG_E("Tx mass test failed, data_size:%d, tx_size:%d",
			data_size, tx_size);
		return -RT_ERROR;
	}

	/* Close the device */
	ret = rt_device_close(dev);
	if (ret != RT_EOK) {
		LOG_E("Failed to close dev:%s", name);
		return ret;
	}

	return ret;
}

long test_uart(int argc, char **argv)
{
	rt_err_t ret = RT_EOK;
	int index = 0;

	LOG_D("<<< Uart test start >>>");

	for (index  = 0; index < (sizeof(uart_port_test)/sizeof(uart_port_test[0]));
		index++) {
		/* Tx test */
		ret = uart_tx_test(uart_port_test[index].name,
					uart_port_test[index].oflag);
		if (ret != RT_EOK) {
			LOG_E("%s tx test failed, oflag:%d",
				uart_port_test[index].name, uart_port_test[index].oflag);
			return ret;
		}

		/* Rx test */
	}

	LOG_D("<<< Uart test successfully >>>");

	return ret;
}

#endif/* UART_TEST */

