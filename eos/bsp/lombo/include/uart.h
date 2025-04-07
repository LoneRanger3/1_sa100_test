/*
 * uart.h - Uart module head file
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

#ifndef __LOMBO_UART_H__
#define __LOMBO_UART_H__

#include <rtdevice.h>
#include "circ_buf.h"
#include <spinlock.h>

#if defined(ARCH_LOMBO_N7V0) || defined(ARCH_LOMBO_N7V1)
#define LOMBO_UART0_BASE	(BASE_UART0 + VA_UART)
#define LOMBO_UART1_BASE	(BASE_UART1 + VA_UART)
#define LOMBO_UART2_BASE	(BASE_UART2 + VA_UART)
#define LOMBO_UART3_BASE	(BASE_UART3 + VA_UART)
#elif defined(ARCH_LOMBO_N8V0)
#define LOMBO_UART0_BASE	(0x5400C000)
#define BASE_UART0		BASE_UART
#endif

#define UART_HW_FIFO_SIZE	64

struct lombo_uart_clk {
	const char *gate;
	const char *reset;
	const char *self;
	const char *parrent;
};

struct lombo_uart_port {
	char name[RT_NAME_MAX];
	void *membase;
	void *phy_addr;
	rt_uint32_t irqno;
	rt_uint32_t mode;
	struct lombo_uart_clk clk;
	u32 fifosize;
	u32 dma_tx_id;
	u32 dma_rx_id;
	struct dma_channel *dmachn;
	struct rt_completion dma_tx_completion;
	struct rt_serial_device *serial;
};

void early_uart_output(const char *str);
void lombo_uart_flush(void *user_data);

#define UART_XMIT_SIZE	(32 * 1024)

#define uart_circ_empty(circ)		((circ)->head == (circ)->tail)
#define uart_circ_clear(circ)		((circ)->head = (circ)->tail = 0)

#define uart_circ_chars_pending(circ)	\
	(CIRC_CNT((circ)->head, (circ)->tail, UART_XMIT_SIZE))

#define uart_circ_chars_free(circ)	\
	(CIRC_SPACE((circ)->head, (circ)->tail, UART_XMIT_SIZE))

/* #define UART_TMP_DEBUG */

#endif/* __LOMBO_UART_H__ */

