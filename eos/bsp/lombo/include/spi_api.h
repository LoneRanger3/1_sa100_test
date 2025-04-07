/*
 * spi_api.h - Generic definitions for LomboTech SPI API
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

#ifndef ___SPI__API___H___
#define ___SPI__API___H___

#include <drivers/spi.h>

#ifdef ARCH_LOMBO_N7

#define ESPI_MAX_XFER_SIZE			(32 * 1024)

#else

#error "please select a valid platform\n"

#endif

/**
 * lb_qspi_transfer_message - transfer one spi message.
 * @device: pointer to qspi device.
 * @message: pointer to ez qspi message.
 *
 * Return 0 if success, !0 error.
 */
rt_err_t lb_qspi_transfer_message(struct rt_qspi_device *qdevice,
	struct rt_qspi_message *message);

/**
 * lb_qspi_recv - send instruction and address, and then receive some data.
 * @device: pointer to qspi device.
 * @inst: instruction to send.
 * @addr: address to send.
 * @addr_bit_len: addr length, unit: bit.
 * @wait_cycle: wait cycle.
 * @data_lines: number of lines using in data stage, same meaning with frame format.
 * @data_buf: data buffer.
 * @data_length: data length.
 * @data_width: data width, can be 8, 16 or 32.
 *
 * Return 0 if success, !0 error.
 */
rt_err_t lb_qspi_recv(struct rt_qspi_device *qdevice,
	rt_uint32_t inst,
	rt_uint32_t addr, rt_uint32_t addr_bit_len,
	rt_uint32_t wait_cycle, rt_uint32_t data_lines,
	void *data_buf, rt_size_t data_length, rt_uint32_t data_width);

/**
 * lb_qspi_send - send instruction and address, and maybe some data.
 * @device: pointer to qspi device.
 * @inst: instruction to send.
 * @addr: address to send.
 * @addr_bit_len: addr length, unit: bit.
 * @wait_cycle: wait cycle.
 * @data_lines: number of lines using in data stage, same meaning with frame format.
 * @data_buf: data buffer.
 * @data_length: data length.
 * @data_width: data width, can be 8, 16 or 32.
 *
 * Return 0 if success, !0 error.
 */
rt_err_t lb_qspi_send(struct rt_qspi_device *qdevice,
	rt_uint32_t inst,
	rt_uint32_t addr, rt_uint32_t addr_bit_len,
	rt_uint32_t wait_cycle, rt_uint32_t data_lines,
	const void *data_buf, rt_size_t data_length, rt_uint32_t data_width);

#endif /* ___SPI__API___H___ */
