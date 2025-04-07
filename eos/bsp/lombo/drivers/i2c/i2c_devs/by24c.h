/*
 * by24c.c - BY24C I2C EEPROM driver code for LomboTech
 * BY24C I2C EEPROM operation interface and macro define
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
#ifndef ___BY24C___H___
#define ___BY24C___H___

/* -------------------------------------------------- */
#define BY24C_TAR_ADDR			0x50
#define BY24C16A_PAGE_SIZE		16
#define BY24C16A_PAGE_SHIFT		4

#define BY24C64A_PAGE_SIZE		32
#define BY24C64A_PAGE_SHIFT		5

#ifdef DEV_IS_BY24C64A
#define BY24C_PAGE_SIZE			BY24C64A_PAGE_SIZE
#define BY24C_PAGE_SHIFT		BY24C64A_PAGE_SHIFT
#else
#define BY24C_PAGE_SIZE			BY24C16A_PAGE_SIZE
#define BY24C_PAGE_SHIFT		BY24C16A_PAGE_SHIFT
#endif

u32 by24c_init(void);
u32 by24c_sw_reset(void);

u32 by24c_byte_write(u8 page_addr, u8 col_addr, u8 write_data);
u32 by24c_page_write(u8 page_addr, u32 page_size, u8 *tx_buf);
u32 by24c_ack_poll(void);

u32 by24c_current_addr_read(u8 *read_data);
u32 by24c_random_read(u8 page_addr, u8 col_addr, u8 *read_data);
u32 by24c_sequence_read(u8 page_addr, u8 col_addr, u32 data_len, u8 *rx_buf);

#endif

