/*
 * spinor_drv.h - SPI Driver for Nor
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

#ifndef ___SPINOR__DRV___H___
#define ___SPINOR__DRV___H___

#include <drivers/spi.h>

#define RT_DEVICE_CTRL_WRITE_BLK	0x14
#define RT_DEVICE_CTRL_OTG_ERASE	0x15
#define RT_DEVICE_CTRL_OTG_WRITE	0x16
#define RT_DEVICE_CTRL_OTG_READ		0x17
#define RT_DEVICE_CTRL_OTG_SETT_LOCK	0x18

#define OTG_LEN 256


/**
 * struct lombo_nor - runtime info holder for SPI nor.
 * @dev: device node.
 * @qspi: pointer to the qspi device.
 * @lock: mutex lock for sync.
 * @geometry: block device geometry structure.
 * @flash_id: flash id.
 * @page_size: max write bytes for one program.
 * @addr_4byte: flash is in 4-byte addressing.
 * @erase_opcode: erase code, 4K erase or 64K erase.
 * @read_buf32: interface for read flash data (32 bits read).
 * @read_buf8: interface for read flash data (8 bits read).
 */
struct lombo_nor {
	struct rt_device		dev;
	struct rt_qspi_device		*qspi;
	struct rt_mutex			lock;

	struct rt_device_blk_geometry	geometry;

	rt_uint32_t			flash_id;
	rt_uint16_t			page_size;
	rt_uint16_t			addr_4byte;
	rt_uint8_t			erase_opcode;

#ifndef ARCH_LOMBO_N7V0
	int (*read_buf32)(struct lombo_nor *flash, rt_uint32_t addr, rt_uint32_t buf,
		rt_uint32_t len);
#endif
	int (*read_buf8)(struct lombo_nor *flash, rt_uint32_t addr, rt_uint32_t buf,
		rt_uint32_t len);
};

struct rt_device_otg_read {
	rt_uint32_t pos;
	rt_uint32_t size;
	rt_uint8_t buffer[OTG_LEN];

};

struct rt_device_otg_write {
	rt_uint32_t pos;
	rt_uint32_t size;
	rt_uint8_t buffer[OTG_LEN];

};

struct rt_device_blk_write {
	rt_off_t pos;
	const void *buffer;
	rt_size_t size;
};

/**
 * lombo_nor_erase - erase an address range on the flash chip.
 * @flash: pointer to lombo nor.
 * @addr: flash address for erase.
 * @len: length for erase.
 *
 * Return 0 if success, !0 error.
 */
int lombo_nor_erase(struct lombo_nor *flash, rt_uint32_t addr, rt_uint32_t len);

/**
 * lombo_nor_read - read an address range from the flash chip.
 * @flash: pointer to lombo nor.
 * @from: flash address from read.
 * @len: length for read.
 * @retlen: actual length for read.
 * @buf: buffer for read.
 *
 * Return 0 if success, !0 error.
 */
int lombo_nor_read(struct lombo_nor *flash, rt_uint32_t from, rt_uint32_t len,
	rt_uint32_t *retlen, void *buf);

/**
 * lombo_nor_write - write an address range to the flash chip.
 * @flash: pointer to lombo nor.
 * @to: flash address to write.
 * @len: length for read.
 * @retlen: actual length for write.
 * @buf: buffer for write.
 *
 * Return 0 if success, !0 error.
 */
int lombo_nor_write(struct lombo_nor *flash, rt_uint32_t to, rt_uint32_t len,
	rt_uint32_t *retlen, const void *buf);

/**
 * lombo_nor_init - init nor device.
 * @flash_name: pointer to the nor device's name.
 *
 * Return 0 if success, 0! error.
 */
int lombo_nor_init(const char *flash_name);

/**
 * lombo_nor_get_device_priv - get nor device's private data.
 * @flash_name: pointer to the nor device's name.
 *
 * Return pointer of nor device's private data.
 */
struct lombo_nor *lombo_nor_get_device_priv(const char *flash_name);

#endif /* ___SPINOR__DRV___H___ */
