/*
 * spinor.c_drv - SPI Driver for Nor
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

#include <drivers/spi.h>

#define DBG_SECTION_NAME	"SPINOR"
#define DBG_LEVEL		DBG_INFO
#include <debug.h>

#include "list.h"
#include "board.h"
#include "spinor_drv.h"
#include "cfg/config_api.h"
#include <spi_api.h>

/* Flash opcodes. */
#define	OPCODE_WREN		0x06	/* Write enable */
#define	OPCODE_RDSR		0x05	/* Read status register */
#define	OPCODE_RDSR2		0x35	/* Read status register2 */
#define	OPCODE_WRSR		0x01	/* Write status register */
#define	OPCODE_WRSR2		0x31	/* Write status register2 */
#define	OPCODE_NORM_READ	0x03	/* Read data bytes (low frequency) */
#define	OPCODE_FAST_READ	0x0b	/* Read data bytes (high frequency) */
#define	OPCODE_DUAL_OP_READ	0x3B	/* Dual output fast read */
#define	OPCODE_QUAD_OP_READ	0x6B	/* Quad output fast read */
#define	OPCODE_PP		0x02	/* Page program (up to 256 bytes) */
#define	OPCODE_BE_4K		0x20	/* Erase 4KiB block */
#define	OPCODE_BE_32K		0x52	/* Erase 32KiB block */
#define	OPCODE_SE		0xd8	/* Sector erase (usually 64KiB) */
#define	OPCODE_CHIP_ERASE	0xc7	/* Erase whole flash chip */
#define	OPCODE_RDID		0x9f	/* Read JEDEC ID */
#define	OPCODE_SR_EA		0x44	/* Erase security register(usually 256-byte) */
#define	OPCODE_SR_PP		0x42	/* Program security Registers */
#define	OPCODE_SR_READ		0x48	/* Read security Registers */

/* Used for SST flashes only. */
#define	OPCODE_BP		0x02	/* Byte program */
#define	OPCODE_WRDI		0x04	/* Write disable */
#define	OPCODE_AAI_WP		0xad	/* Auto address increment word program */

/* Used for Macronix flashes only. */
#define	OPCODE_EN4B		0xb7	/* Enter 4-byte mode */
#define	OPCODE_EX4B		0xe9	/* Exit 4-byte mode */

/* Used for Spansion flashes only. */
#define	OPCODE_BRWR		0x17	/* Bank register write */

/* Status Register bits. */
#define WIP_BIT			(0)
#define WEL_BIT			(1)

/*
 * Quad mode enable bit of status register.
 * Different type of flash may have different quad enable bit,
 * and some flash may have not quad enable bit.
 */
#define MACRONIX_QE_BIT		(6)
#define GD_BY_QE_BIT		(1)

/* meaning of other SR_* bits may differ between vendors */
#define	SR_BP0			4	/* Block protect 0 */
#define	SR_BP1			8	/* Block protect 1 */
#define	SR_BP2			0x10	/* Block protect 2 */
#define	SR_SRWD			0x80	/* SR write protect */
#define	SR_LB1			4	/* Lock bit 1 */
#define	SR_LB2			8	/* Lock bit 2 */
#define	SR_LB3			0x10	/* Lock bit 3 */

/* Define max times to check status register before we give up. */
#define	MAX_READY_WAIT_SECOND	120	/* 120s max chip erase */
#define	MAX_CMD_SIZE		5

#define JEDEC_MFR(_jedec_id)	((_jedec_id) >> 16)

#define BUF_READ_SIZE		ESPI_MAX_XFER_SIZE

#define NOR_DEFAULT_SPI_CFG				\
{							\
	.mode = RT_SPI_MODE_0|RT_SPI_MSB|RT_SPI_QSPI,	\
	.data_width = 0,				\
	.max_hz = 50 * 1000 * 1000,			\
}

#define NOR_DEFAULT_QSPI_CFG				\
{							\
	NOR_DEFAULT_SPI_CFG,				\
	.medium_size = 0x800000,			\
	.ddr_mode = 0,					\
	.qspi_dl_width = 4,				\
}

struct flash_info {
	const char	*name;

	/* JEDEC id zero means "no ID" (most older chips); otherwise it has
	 * a high byte of zero plus three data bytes: the manufacturer id,
	 * then a two byte device id.
	 */
	rt_uint32_t	jedec_id;
	rt_uint16_t	ext_id;
	rt_uint16_t	reserved;

	/* The size listed here is what works with OPCODE_SE, which isn't
	 * necessarily called a "sector" by the vendor.
	 */
	rt_uint32_t	sector_size;
	rt_uint16_t	n_sectors;

	rt_uint16_t	page_size;
	rt_uint16_t	addr_width;

	rt_uint16_t	flags;
#define	SECT_4K		0x01		/* OPCODE_BE_4K works uniformly */
#define	DUAL_READ	0x02		/* support DUAL output fast read */
#define	QUAD_READ	0x04		/* support QUAL output fast read */
};

#define INFO(_name, _jedec_id, _ext_id, _sector_size,	\
	_n_sectors, _flags) {				\
		.name = (_name),			\
		.jedec_id = (_jedec_id),		\
		.ext_id = (_ext_id),			\
		.sector_size = (_sector_size),		\
		.n_sectors = (_n_sectors),		\
		.page_size = 256,			\
		.flags = (_flags),			\
	}

/* NOTE: double check command sets and memory organization when you add
 * more flash chips.  This current list focusses on newer chips, which
 * have been converging on command sets which including JEDEC ID.
 */
static const struct flash_info lombo_nor_ids[] = {
	/* GigaDevice */
	INFO("gd25q32",     0xc84016, 0, 64 * 1024,  64, SECT_4K),
	INFO("gd25q64",     0xc84017, 0, 64 * 1024, 128, SECT_4K|DUAL_READ|QUAD_READ),
	INFO("gd25q128",    0xc84018, 0, 64 * 1024, 256, SECT_4K|DUAL_READ|QUAD_READ),
	INFO("gd25q256",    0xc84019, 0, 64 * 1024, 512, SECT_4K|DUAL_READ|QUAD_READ),
	/* Macronix */
	INFO("mx25l6408e",  0xc22017, 0, 64 * 1024, 128, SECT_4K|DUAL_READ),
	INFO("mx25l12836e", 0xc22018, 0, 64 * 1024, 256, SECT_4K|DUAL_READ|QUAD_READ),
	INFO("mx25l25635e", 0xc22019, 0, 64 * 1024, 512, SECT_4K|DUAL_READ|QUAD_READ),
	/* Micron */
	INFO("n25q064a",    0x20ba17, 0, 64 * 1024, 128, SECT_4K|DUAL_READ|QUAD_READ),
	INFO("n25q128a11",  0x20bb18, 0, 64 * 1024, 256, SECT_4K|DUAL_READ|QUAD_READ),
	INFO("n25q128a13",  0x20ba18, 0, 64 * 1024, 256, SECT_4K|DUAL_READ|QUAD_READ),
	INFO("n25q256",     0x20ba19, 0, 64 * 1024, 512, SECT_4K|DUAL_READ|QUAD_READ),
	/* Winbond -- w25x "blocks" are 64K, "sectors" are 4KiB */
	INFO("w25q32",      0xef4016, 0, 64 * 1024,  64, SECT_4K|DUAL_READ|QUAD_READ),
	INFO("w25q64",      0xef4017, 0, 64 * 1024, 128, SECT_4K|DUAL_READ|QUAD_READ),
	INFO("w25q128",     0xef4018, 0, 64 * 1024, 256, SECT_4K|DUAL_READ|QUAD_READ),
	INFO("w25q256",     0xef4019, 0, 64 * 1024, 512, SECT_4K|DUAL_READ|QUAD_READ),
	/* Eon */
	INFO("en25qh128a",  0x1c7018, 0, 64 * 1024, 256, SECT_4K|DUAL_READ|QUAD_READ),
	/* Boya */
	INFO("by25q128as",  0x684018, 0, 64 * 1024, 256, SECT_4K|DUAL_READ|QUAD_READ),
	/* Virtual -- note: must be the last one of this list */
	INFO("virt_nor",    0x000000, 0, 64 * 1024, 256, SECT_4K|DUAL_READ|QUAD_READ),
};

/**
 * __read_status_cmd - read nor status without address.
 * @qspi: pointer to qspi device.
 * @cmd: control command to send.
 * @buf: buffer for read.
 * @len: number of bytes to read.
 *
 * Return 0 if success, !0 error.
 */
static int __read_status_cmd(struct rt_qspi_device *qspi, rt_uint8_t cmd,
	rt_uint8_t *buf, rt_uint32_t len)
{
	int err = 0;

	err = lb_qspi_recv(qspi, cmd, 0, 0, 0, 1, buf, len, 8);
	if (err) {
		LOG_E("read status cmd:0x%x ret:%d", cmd, err);
		return err;
	}

	return err;
}

/**
 * __write_status_cmd - write nor status without address.
 * @qspi: pointer to qspi device.
 * @cmd: control command to send.
 * @buf: buffer for write.
 * @len: number of bytes to write.
 *
 * write nor register data without address.
 */
static int __write_status_cmd(struct rt_qspi_device *qspi, rt_uint8_t cmd,
	rt_uint8_t *buf, rt_uint32_t len)
{
	int err = 0;

	err = lb_qspi_send(qspi, cmd, 0, 0, 0, 1, buf, len, 8);
	if (err) {
		LOG_E("write status cmd:0x%x ret:%d", cmd, err);
		return err;
	}

	return err;
}

/**
 * __send_ctrl_cmd - send control command.
 * @qspi: pointer to qspi device.
 * @cmd: control command to send.
 *
 * Return 0 if success, !0 error.
 */
static int __send_ctrl_cmd(struct rt_qspi_device *qspi, rt_uint8_t cmd)
{
	int err = 0;

	err = lb_qspi_send(qspi, cmd, 0, 0, 0, 1, RT_NULL, 0, 8);
	if (err) {
		LOG_E("send control cmd:0x%x ret:%d", cmd, err);
		return err;
	}

	return err;
}

/**
 * lombo_read_sr - read the status register.
 * @qspi: pointer to qspi device.
 *
 * Return the status register value if success, <0 error.
 */
static int lombo_read_sr(struct rt_qspi_device *qspi)
{
	int err = 0;
	rt_uint8_t val = 0;

	err = __read_status_cmd(qspi, OPCODE_RDSR, &val, 1);
	if (err) {
		LOG_E("read status ret:%d", err);
		return err;
	}

	return val;
}

/**
 * lombo_write_sr - write status register 1 byte.
 * @qspi: pointer to qspi device.
 * @val: value to write.
 *
 * Return 0 if success, !0 error.
 */
static int lombo_write_sr(struct rt_qspi_device *qspi, rt_uint8_t val)
{
	int err = 0;

	err = __write_status_cmd(qspi, OPCODE_WRSR, &val, 1);
	if (err) {
		LOG_E("write status ret:%d", err);
		return err;
	}

	return err;
}

/**
 * lombo_wait_till_ready - read status register until ready, or timeout occurs.
 * @flash: pointer to lombo nor.
 * @status_bit: status bit for check.
 * @ready_status: target status, 0 or 1.
 *
 * Return 0 if success, !0 error.
 */
static int lombo_wait_till_ready(struct lombo_nor *flash,
	rt_uint8_t status_bit, rt_uint8_t ready_status)
{
	int sr, err = -RT_ETIMEOUT;
	rt_uint32_t loop = MAX_READY_WAIT_SECOND * RT_TICK_PER_SECOND;

	do {
		sr = lombo_read_sr(flash->qspi);
		if (sr < 0) {
			err = sr;
			break;
		}

		if (((sr >> status_bit) & 0x1) == ready_status)
			return 0;

		rt_thread_delay(1);
	} while (loop-- > 0);

	LOG_W("wait ready timeout");

	return err;
}

/**
 * lombo_wait_till_ready_ontime - read status register ontime until ready,
 * or timeout occurs.
 * @flash: pointer to lombo nor.
 * @status_bit: status bit for check.
 * @ready_status: target status, 0 or 1.
 *
 * Return 0 if success, !0 error.
 */
static int lombo_wait_till_ready_ontime(struct lombo_nor *flash,
	rt_uint8_t status_bit, rt_uint8_t ready_status)
{
	int sr, err = -RT_ETIMEOUT;
	/* retry interval: 10us */
	rt_uint32_t loop = MAX_READY_WAIT_SECOND * 1000 * 1000 / 10;

	do {
		sr = lombo_read_sr(flash->qspi);
		if (sr < 0) {
			err = sr;
			break;
		}

		if (((sr >> status_bit) & 0x1) == ready_status)
			return 0;

		udelay(10);
	} while (loop-- > 0);

	LOG_W("wait ready timeout");

	return err;
}

/**
 * lombo_write_enable - set write enable latch with Write Enable command.
 * @flash: pointer to lombo nor.
 *
 * Return 0 if success, !0 error.
 */
static int lombo_write_enable(struct lombo_nor *flash)
{
	int err = 0;

	err = __send_ctrl_cmd(flash->qspi, OPCODE_WREN);
	if (err) {
		LOG_E("send control cmd ret:%d", err);
		return err;
	}

	err = lombo_read_sr(flash->qspi);
	if (err < 0) {
		LOG_E("read status register ret:%d", err);
		return err;

	}

	if (!(err & (1 << WEL_BIT))) {
		LOG_E("write enable failed");
		return -RT_ERROR;
	}

	return 0;
}

/**
 * lombo_set_quad_mode - enable or disable quad I/O mode.
 * @flash: pointer to flash device.
 * @enable: !0 for enable, 0 for disable.
 *
 * Return 0 if success, !0 error.
 */
static int lombo_set_quad_mode(struct lombo_nor *flash, rt_uint32_t enable)
{
	int err = 0;
	rt_uint8_t val;

	switch (flash->flash_id) {
	/**
	 * These kind of flash (of Macronix),
	 * should set QE bit of status register to enable qual mode read & write.
	 */
	case 0xc22019:
	case 0xc22018: {
		err = lombo_wait_till_ready_ontime(flash, WIP_BIT, 0);
		if (err) {
			LOG_E("wait ready ret:%d", err);
			return err;
		}

		val = lombo_read_sr(flash->qspi);
		if (val < 0) {
			LOG_E("read status register ret:%d", val);
			return val;
		}

		err = lombo_write_enable(flash);
		if (err) {
			LOG_E("write enable ret:%d", err);
			return err;
		}

		val = enable ? (val | (1 << MACRONIX_QE_BIT)) :
			(val & ~(1 << MACRONIX_QE_BIT));
		err = lombo_write_sr(flash->qspi, val);
		if (err) {
			LOG_E("write status register ret:%d", err);
			return err;
		}
	}
		break;
	case 0xc84017:
	case 0xc84018:
	case 0xc84019:
	case 0xef4016:
	case 0xef4017:
	case 0xef4018:
	case 0xef4019:
	case 0x684018: {
		err = lombo_wait_till_ready_ontime(flash, WIP_BIT, 0);
		if (err) {
			LOG_E("wait ready ret:%d", err);
			return err;
		}

		err = __read_status_cmd(flash->qspi, OPCODE_RDSR2, &val, 1);
		if (err) {
			LOG_E("read status ret:%d", err);
			return err;
		}

		err = lombo_write_enable(flash);
		if (err) {
			LOG_E("write enable ret:%d", err);
			return err;
		}

		val = enable ? (val | (1 << GD_BY_QE_BIT)) :
			(val & ~(1 << GD_BY_QE_BIT));
		err = __write_status_cmd(flash->qspi, OPCODE_WRSR2, &val, 1);
		if (err) {
			LOG_E("write status ret:%d", err);
			return err;
		}
	}
		break;

	default:
		break;
	}

	LOG_D("set quad read mode ok");

	return err;
}

/**
 * lombo_set_sr_lock_bit  - Set security register Lock Bit to 1
 * It's set to 1, The security register will become read-only permanently
 * Return 0 if success, !0 error.
 */
int lombo_set_sr_lock_bit(struct lombo_nor *flash)
{
	int err = 0;
	rt_uint8_t val;

	err = lombo_wait_till_ready_ontime(flash, WIP_BIT, 0);
	if (err) {
		LOG_E("wait ready ret:%d", err);
		return err;
	}

	err = __read_status_cmd(flash->qspi, OPCODE_RDSR2, &val, 1);
	if (err) {
		LOG_E("read status ret:%d", err);
		return err;
	}

	err = lombo_write_enable(flash);
	if (err) {
		LOG_E("write enable ret:%d", err);
		return err;
	}

	val = (val | SR_LB1 | SR_LB2 | SR_LB3);
	err = __write_status_cmd(flash->qspi, OPCODE_WRSR2, &val, 1);
	if (err) {
		LOG_E("write status ret:%d", err);
		return err;
	}

	LOG_D("set security register read-only mode ok");

	return err;
}

/**
 * lombo_setup_flash - something to do before using the flash.
 * @flash: pointer to flash device.
 *
 * Return 0 if success, !0 error.
 */
static int lombo_setup_flash(struct lombo_nor *flash)
{
	int err = 0;
	rt_uint8_t val;

	switch (flash->flash_id) {
	case 0xef4018: {
		err = lombo_wait_till_ready_ontime(flash, WIP_BIT, 0);
		if (err) {
			LOG_E("wait ready ret:%d", err);
			return err;
		}

		err = __read_status_cmd(flash->qspi, OPCODE_RDSR2, &val, 1);
		if (err) {
			LOG_E("read status ret:%d", err);
			return err;
		}

		err = lombo_write_enable(flash);
		if (err) {
			LOG_E("write enable ret:%d", err);
			return err;
		}

		val &= ~(1 << 6);
		err = __write_status_cmd(flash->qspi, OPCODE_WRSR2, &val, 1);
		if (err) {
			LOG_E("write status ret:%d", err);
			return err;
		}
	}
		break;
	default:
		break;
	}

	return 0;
}

/**
 * lombo_set_4Byte_address - enable or disable 4-byte addressing mode.
 * @flash: pointer to flash device.
 * @enable: !0 for enable, 0 for disable.
 *
 * Return 0 if success, !0 error.
 */
static int lombo_set_4Byte_address(struct lombo_nor *flash, rt_uint32_t enable)
{
	int err = 0;
	rt_uint8_t val;

	err = lombo_wait_till_ready_ontime(flash, WIP_BIT, 0);
	if (err) {
		LOG_E("wait ready ret:%d", err);
		return err;
	}

	err = lombo_write_enable(flash);
	if (err) {
		LOG_E("write enable ret:%d", err);
		return err;
	}

	err = __send_ctrl_cmd(flash->qspi, enable ? OPCODE_EN4B : OPCODE_EX4B);
	if (err) {
		LOG_W("send control cmd ret:%d", err);
		return err;
	}

	switch (flash->flash_id) {
	/**
	 * We get that what we should do to bring these kind of flash (of GigaDevice),
	 * to enter 4-byte address mode is not only to send Enter 4-Byte Address Mode
	 * command, but also to set the EN4B bit in Status Register2 to 1.
	 */
	case 0xc84019: {
		err = lombo_wait_till_ready_ontime(flash, WIP_BIT, 0);
		if (err) {
			LOG_E("wait ready ret:%d", err);
			return err;
		}

		err = __read_status_cmd(flash->qspi, OPCODE_RDSR2, &val, 1);
		if (err) {
			LOG_E("read status ret:%d", err);
			return err;
		}

		err = lombo_write_enable(flash);
		if (err) {
			LOG_E("write enable ret:%d", err);
			return err;
		}

		val = enable ? (val | (1 << 3)) : (val & ~(1 << 3));
		err = __write_status_cmd(flash->qspi, OPCODE_WRSR2, &val, 1);
		if (err) {
			LOG_E("write status ret:%d", err);
			return err;
		}
	}
		break;

	default:
		break;
	}

	return err;
}

#ifndef ARCH_LOMBO_N7V0
/**
 * __read_buf_32 - read chip data into buffer in 32 bits read.
 * @flash: pointer to lombo nor.
 * @addr: nor address for data read.
 * @buf: buffer to store data.
 * @len: number of bytes to read.
 *
 * Return 0 if success, !0 error.
 */
static int __read_buf_32(struct lombo_nor *flash, rt_uint32_t addr,
	rt_uint32_t buf, rt_uint32_t len)
{
	int err = 0;

	if (flash->addr_4byte)
		err = lb_qspi_recv(flash->qspi, OPCODE_NORM_READ, addr, 32, 0, 1,
			(rt_uint32_t *)buf, len, 32);
	else
		err = lb_qspi_recv(flash->qspi, OPCODE_NORM_READ, addr, 24, 0, 1,
			(rt_uint32_t *)buf, len, 32);
	if (err) {
		LOG_E("read from:0x%x to:0x%x len:%d ret:%d", addr, buf, len, err);
		return err;
	}

	return err;
}

/**
 * __read_buf_32_dual - read chip data into buffer in 32 bits read, using 2 data lines.
 * @flash: pointer to lombo nor.
 * @addr: nor address for data read.
 * @buf: buffer to store data.
 * @len: number of bytes to read.
 *
 * Return 0 if success, !0 error.
 */
static int __read_buf_32_dual(struct lombo_nor *flash, rt_uint32_t addr,
	rt_uint32_t buf, rt_uint32_t len)
{
	int err = 0;

	if (flash->addr_4byte)
		err = lb_qspi_recv(flash->qspi, (OPCODE_DUAL_OP_READ+1), addr, 32, 8, 2,
			(rt_uint32_t *)buf, len, 32);
	else
		err = lb_qspi_recv(flash->qspi, OPCODE_DUAL_OP_READ, addr, 24, 8, 2,
			(rt_uint32_t *)buf, len, 32);
	if (err) {
		LOG_E("read from:0x%x to:0x%x len:%d ret:%d", addr, buf, len, err);
		return err;
	}

	return err;
}

/**
 * __read_buf_32_quad - read chip data into buffer in 32 bits read, using 4 data lines.
 * @flash: pointer to lombo nor.
 * @addr: nor address for data read.
 * @buf: buffer to store data.
 * @len: number of bytes to read.
 *
 * Return 0 if success, !0 error.
 */
static int __read_buf_32_quad(struct lombo_nor *flash, rt_uint32_t addr,
	rt_uint32_t buf, rt_uint32_t len)
{
	int err = 0;

	if (flash->addr_4byte)
		err = lb_qspi_recv(flash->qspi, (OPCODE_QUAD_OP_READ+1), addr, 32, 8, 4,
			(rt_uint32_t *)buf, len, 32);
	else
		err = lb_qspi_recv(flash->qspi, OPCODE_QUAD_OP_READ, addr, 24, 8, 4,
			(rt_uint32_t *)buf, len, 32);
	if (err) {
		LOG_E("read from:0x%x to:0x%x len:%d ret:%d", addr, buf, len, err);
		return err;
	}

	return err;
}
#endif

/**
 * __read_buf_8 - read chip data into buffer in 8 bits read.
 * @flash: pointer to lombo nor.
 * @addr: nor address for data read.
 * @buf: buffer to store data.
 * @len: number of bytes to read.
 *
 * Return 0 if success, !0 error.
 */
static int __read_buf_8(struct lombo_nor *flash, rt_uint32_t addr,
	rt_uint32_t buf, rt_uint32_t len)
{
	int err = 0;

	if (flash->addr_4byte)
		err = lb_qspi_recv(flash->qspi, OPCODE_NORM_READ, addr, 32, 0, 1,
			(rt_uint8_t *)buf, len, 8);
	else
		err = lb_qspi_recv(flash->qspi, OPCODE_NORM_READ, addr, 24, 0, 1,
			(rt_uint8_t *)buf, len, 8);
	if (err) {
		LOG_E("read from:0x%x to:0x%x len:%d ret:%d", addr, buf, len, err);
		return err;
	}

	return err;
}

/**
 * __read_buf_8_dual - read chip data into buffer in 8 bits read.
 * @flash: pointer to lombo nor.
 * @addr: nor address for data read.
 * @buf: buffer to store data.
 * @len: number of bytes to read.
 *
 * Return 0 if success, !0 error.
 */
static int __read_buf_8_dual(struct lombo_nor *flash, rt_uint32_t addr,
	rt_uint32_t buf, rt_uint32_t len)
{
	int err = 0;

	if (flash->addr_4byte)
		err = lb_qspi_recv(flash->qspi, (OPCODE_DUAL_OP_READ+1), addr, 32, 8, 2,
			(rt_uint8_t *)buf, len, 8);
	else
		err = lb_qspi_recv(flash->qspi, OPCODE_DUAL_OP_READ, addr, 24, 8, 2,
			(rt_uint8_t *)buf, len, 8);
	if (err) {
		LOG_E("read from:0x%x to:0x%x len:%d ret:%d", addr, buf, len, err);
		return err;
	}

	return err;
}

/**
 * __read_buf_8_quad - read chip data into buffer in 8 bits read.
 * @flash: pointer to lombo nor.
 * @addr: nor address for data read.
 * @buf: buffer to store data.
 * @len: number of bytes to read.
 *
 * Return 0 if success, !0 error.
 */
static int __read_buf_8_quad(struct lombo_nor *flash, rt_uint32_t addr,
	rt_uint32_t buf, rt_uint32_t len)
{
	int err = 0;

	if (flash->addr_4byte)
		err = lb_qspi_recv(flash->qspi, (OPCODE_QUAD_OP_READ+1), addr, 32, 8, 4,
			(rt_uint8_t *)buf, len, 8);
	else
		err = lb_qspi_recv(flash->qspi, OPCODE_QUAD_OP_READ, addr, 24, 8, 4,
			(rt_uint8_t *)buf, len, 8);
	if (err) {
		LOG_E("read from:0x%x to:0x%x len:%d ret:%d", addr, buf, len, err);
		return err;
	}

	return err;
}

/**
 * __read_buf - read chip data into buffer.
 * @flash: pointer to lombo nor.
 * @addr: nor address for data read.
 * @buf: buffer to store data.
 * @len: number of bytes to read.
 *
 * Return 0 if success, !0 error.
 */
static int __read_buf(struct lombo_nor *flash, rt_uint32_t addr,
	rt_uint32_t buf, rt_uint32_t len)
{
	int err = 0;
#ifndef ARCH_LOMBO_N7V0
	rt_uint32_t nor_addr = addr;
	rt_uint32_t dst_buf = buf;
	int div_len = len / 4;
	int mod_len = len & (4 - 1);

	RT_ASSERT(flash->read_buf32);
	RT_ASSERT(flash->read_buf8);

	if (div_len > 0) {
		err = flash->read_buf32(flash, nor_addr, dst_buf, 4 * div_len);
		if (err) {
			LOG_E("read buf32 ret:%d", err);
			goto out;
		}

		nor_addr += 4 * div_len;
		dst_buf  += 4 * div_len;
	}

	if (mod_len != 0) {
		err = flash->read_buf8(flash, nor_addr, dst_buf, mod_len);
		if (err) {
			LOG_E("read buf8 ret:%d", err);
			goto out;
		}
	}
#else
	RT_ASSERT(flash->read_buf8);

	err = flash->read_buf8(flash, addr, buf, len);
	if (err) {
		LOG_E("read buf8 ret:%d", err);
		goto out;
	}
#endif

out:
	return err;
}

/**
 * lombo_nor_read_buf - read chip data into buffer.
 * @flash: pointer to lombo nor.
 * @addr: nor address for data read.
 * @buf: buffer to store data.
 * @len: number of bytes to read.
 *
 * Return 0 if success, !0 error.
 */
static int lombo_nor_read_buf(struct lombo_nor *flash, rt_uint32_t addr,
	rt_uint32_t buf, rt_uint32_t len)
{
	int i, err = 0;
	rt_uint32_t nor_addr = addr;
	rt_uint32_t dst_buf = buf;
	int div_len = len / (BUF_READ_SIZE);
	int mod_len = len & (BUF_READ_SIZE - 1);

	for (i = 0; i < div_len; i++) {
		err = __read_buf(flash, nor_addr, dst_buf, BUF_READ_SIZE);
		if (err) {
			LOG_E("read buf ret:%d", err);
			goto out;
		}

		nor_addr += BUF_READ_SIZE;
		dst_buf  += BUF_READ_SIZE;
	}

	if (mod_len != 0) {
		err = __read_buf(flash, nor_addr, dst_buf, mod_len);
		if (err) {
			LOG_E("read buf ret:%d", err);
			goto out;
		}
	}

out:
	return err;
}

#ifndef ARCH_LOMBO_N7V0
/**
 * __write_buf_32 - write buffer to chip in 32 bits write.
 * @flash: pointer to lombo nor.
 * @addr: nor address for data write.
 * @buf: data buffer.
 * @len: number of bytes to write.
 *
 * Return 0 if success, !0 error.
 */
static int __write_buf_32(struct lombo_nor *flash, rt_uint32_t addr,
	rt_uint32_t buf, rt_uint32_t len)
{
	int err = 0;

	if (flash->addr_4byte)
		err = lb_qspi_send(flash->qspi, OPCODE_PP, addr, 32, 0, 1,
			(rt_uint32_t *)buf, len, 32);
	else
		err = lb_qspi_send(flash->qspi, OPCODE_PP, addr, 24, 0, 1,
			(rt_uint32_t *)buf, len, 32);
	if (err) {
		LOG_E("write from:0x%x to:0x%x len:%d ret:%d", addr, buf, len, err);
		return err;
	}

	return err;
}
#endif

/**
 * __write_buf_8 - write buffer to chip in 8 bits write.
 * @flash: pointer to lombo nor.
 * @addr: nor address for data write.
 * @buf: data buffer.
 * @len: number of bytes to write.
 *
 * Return 0 if success, !0 error.
 */
static int __write_buf_8(struct lombo_nor *flash, rt_uint32_t addr,
	rt_uint32_t buf, rt_uint32_t len)
{
	int err = 0;

	if (flash->addr_4byte)
		err = lb_qspi_send(flash->qspi, OPCODE_PP, addr, 32, 0, 1,
			(rt_uint8_t *)buf, len, 8);
	else
		err = lb_qspi_send(flash->qspi, OPCODE_PP, addr, 24, 0, 1,
			(rt_uint8_t *)buf, len, 8);
	if (err) {
		LOG_E("write from:0x%x to:0x%x len:%d ret:%d", buf, addr, len, err);
		return err;
	}

	return err;
}

/**
 * lombo_nor_write_buf - write buffer to chip.
 * @flash: pointer to lombo nor.
 * @addr: nor address for data write.
 * @buf: data buffer.
 * @len: number of bytes to write.
 *
 * Return 0 if success, !0 error.
 */
static int lombo_nor_write_buf(struct lombo_nor *flash, rt_uint32_t addr,
	rt_uint32_t buf, rt_uint32_t len)
{
	int err = 0;
#ifndef ARCH_LOMBO_N7V0
	rt_uint32_t nor_addr = addr;
	rt_uint32_t src_buf = buf;
	int div_len = len / 4;
	int mod_len = len & (4 - 1);

	if (div_len > 0) {
		err = __write_buf_32(flash, nor_addr, src_buf, 4 * div_len);
		if (err) {
			LOG_E("write buf32 ret:%d", err);
			goto out;
		}

		nor_addr += 4 * div_len;
		src_buf  += 4 * div_len;
	}

	if (mod_len != 0) {
		if (div_len > 0) {
			err = lombo_wait_till_ready_ontime(flash, WIP_BIT, 0);
			if (err) {
				LOG_E("wait ready ret:%d", err);
				goto out;
			}

			err = lombo_write_enable(flash);
			if (err) {
				LOG_E("write enable ret:%d", err);
				goto out;
			}
		}

		err = __write_buf_8(flash, nor_addr, src_buf, mod_len);
		if (err) {
			LOG_E("write buf8 ret:%d", err);
			goto out;
		}
	}
#else
	err = __write_buf_8(flash, addr, buf, len);
	if (err) {
		LOG_E("write buf8 ret:%d", err);
		goto out;
	}
#endif

out:
	return err;

}

/**
 * lombo_erase_chip - erase the whole flash memory.
 * @flash: pointer to lombo nor.
 *
 * Return 0 if success, !0 error.
 */
static int lombo_erase_chip(struct lombo_nor *flash)
{
	int err = 0;
#if 0
	rt_uint32_t capacity;

	capacity = flash->geometry.sector_count * flash->geometry.bytes_per_sector;
	LOG_D("chip erase total:%lldKB", (capacity >> 10));
#endif

	err = lombo_wait_till_ready(flash, WIP_BIT, 0);
	if (err) {
		LOG_E("wait ready ret:%d", err);
		return err;
	}

	err = lombo_write_enable(flash);
	if (err) {
		LOG_E("write enable ret:%d", err);
		return err;
	}

	return __send_ctrl_cmd(flash->qspi, OPCODE_CHIP_ERASE);
}

/**
 * lombo_nor_security_register_erase - erase Security Register.
 * @flash: pointer to lombo nor.
 *
 * Return 0 if success, !0 error.
 */
int lombo_nor_security_register_erase(struct lombo_nor *flash)
{
	int err = 0;

	err = lombo_wait_till_ready(flash, WIP_BIT, 0);
	if (err) {
		LOG_E("wait ready ret:%d", err);
		return err;
	}

	err = lombo_write_enable(flash);
	if (err) {
		LOG_E("write enable ret:%d", err);
		return err;
	}

	err = lb_qspi_send(flash->qspi, OPCODE_SR_EA, 0, 24, 0, 1, RT_NULL, 0, 8);
	if (err) {
		LOG_E("ret:%d", err);
		return err;
	}

	return 0;
}

/**
 * lombo_erase_sector - erase one sector of flash memory at offset.
 * @flash: pointer to lombo nor.
 * @offset: offset in flash.
 *
 * Return 0 if success, !0 error.
 */
static int lombo_erase_sector(struct lombo_nor *flash, rt_uint32_t addr)
{
	int err = 0;

	LOG_D("sector erase addr:0x%08x len:%d Kbytes", addr,
		(flash->geometry.block_size/1024));

	err = lombo_wait_till_ready(flash, WIP_BIT, 0);
	if (err) {
		LOG_E("wait ready ret:%d", err);
		return err;
	}

	err = lombo_write_enable(flash);
	if (err) {
		LOG_E("write enable ret:%d", err);
		return err;
	}

	if (flash->addr_4byte)
		err = lb_qspi_send(flash->qspi, flash->erase_opcode, addr,
			32, 0, 1, RT_NULL, 0, 8);
	else
		err = lb_qspi_send(flash->qspi, flash->erase_opcode, addr,
			24, 0, 1, RT_NULL, 0, 8);
	if (err) {
		LOG_E("send erase cmd:0x%x ret:%d", flash->erase_opcode, err);
		return err;
	}

	return 0;
}

/* MTD implementation */

/**
 * lombo_nor_security_register_read  - read nor security register
 * @flash: pointer to lombo nor.
 * @buf: buffer to store data.
 * @addr: nor address for data read.
 * @len: number of bytes to read.
 *
 * Return 0 if success, !0 error.
 */
int lombo_nor_security_register_read(struct lombo_nor *flash, rt_uint8_t *buf,
				rt_uint32_t addr, rt_uint32_t len)
{
	int err = 0;

	err = lombo_wait_till_ready(flash, WIP_BIT, 0);
	if (err) {
		LOG_E("wait ready ret:%d", err);
		return err;
	}

	err = lombo_write_enable(flash);
	if (err) {
		LOG_E("write enable ret:%d", err);
		return err;
	}

	err = lb_qspi_recv(flash->qspi, OPCODE_SR_READ, addr, 24, 8, 1,
			buf, len, 8);
	if (err) {
		LOG_E("read nor security register failed\n");
		return err;
	}
	return 0;
}

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
	rt_uint32_t *retlen, void *buf)
{
	int err = 0;

	LOG_D("nor read addr:0x%x len:%d buf:0x%08x", from, len, (rt_uint32_t)buf);

	rt_mutex_take(&flash->lock, RT_WAITING_FOREVER);

	err = lombo_wait_till_ready(flash, WIP_BIT, 0);
	if (err) {
		LOG_E("wait ready ret:%d", err);
		*retlen = 0;
		goto out;
	}

	err = lombo_nor_read_buf(flash, from, (rt_uint32_t)buf, len);
	if (err) {
		LOG_E("read buf ret:%d", err);
		*retlen = 0;
		goto out;
	}

	*retlen = len;

out:
	rt_mutex_release(&flash->lock);

	return err;
}

/**
 * lombo_nor_security_register_write - Program Security Registers.
 * @flash: pointer to lombo nor.
 * @buf: data buffer.
 * @addr: nor address for data write.
 * @len: data len.
 *
 * Return 0 if success, !0 error.
 */
int lombo_nor_security_register_write(struct lombo_nor *flash, rt_uint8_t *buf,
				rt_uint32_t addr, rt_uint32_t len)
{
	int err = 0;

	err = lombo_wait_till_ready(flash, WIP_BIT, 0);
	if (err) {
		LOG_E("wait ready ret:%d", err);
		return err;
	}

	err = lombo_write_enable(flash);
	if (err) {
		LOG_E("write enable ret:%d", err);
		return err;
	}

	err = lb_qspi_send(flash->qspi, OPCODE_SR_PP, addr, 24, 0, 1,
			buf, len, 8);

	if (err) {
		LOG_E("program security registers failed\n");
		return err;
	}

	return 0;
}

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
	rt_uint32_t *retlen, const void *buf)
{
	rt_uint32_t page_offset, page_size;
	int err = 0;

	LOG_D("nor write addr:0x%x len:%d buf:0x%08x", to, len, (rt_uint32_t)buf);

	rt_mutex_take(&flash->lock, RT_WAITING_FOREVER);

	err = lombo_wait_till_ready(flash, WIP_BIT, 0);
	if (err) {
		LOG_E("wait ready ret:%d", err);
		*retlen = 0;
		goto out;
	}

	err = lombo_write_enable(flash);
	if (err) {
		LOG_E("write enable ret:%d", err);
		*retlen = 0;
		goto out;
	}

	page_offset = to & (flash->page_size - 1);
	/* do all the bytes fit onto one page? */
	if ((page_offset + len) <= flash->page_size) {
		err = lombo_nor_write_buf(flash, to, (rt_uint32_t)buf, len);
		if (err) {
			LOG_E("write buf ret:%d", err);
			*retlen = 0;
			goto out;
		}

		*retlen = len;
	} else {
		rt_uint32_t i;

		/* the size of data remaining on the first page */
		page_size = flash->page_size - page_offset;

		err = lombo_nor_write_buf(flash, to, (rt_uint32_t)buf, page_size);
		if (err) {
			LOG_E("write buf ret:%d", err);
			*retlen = 0;
			goto out;
		}

		*retlen = page_size;

		/* write everything in flash->page_size chunks */
		for (i = page_size; i < len; i += page_size) {
			page_size = len - i;
			if (page_size > flash->page_size)
				page_size = flash->page_size;

			err = lombo_wait_till_ready_ontime(flash, WIP_BIT, 0);
			if (err) {
				LOG_E("wait ready ret:%d", err);
				*retlen = 0;
				goto out;
			}

			err = lombo_write_enable(flash);
			if (err) {
				LOG_E("write enable ret:%d", err);
				*retlen = 0;
				goto out;
			}

			err = lombo_nor_write_buf(flash, to + i,
				(rt_uint32_t)buf + i, page_size);
			if (err) {
				LOG_E("write buf ret:%d", err);
				*retlen = 0;
				goto out;
			}

			*retlen += page_size;
		}
	}

out:
	rt_mutex_release(&flash->lock);

	return err;
}

/**
 * lombo_nor_erase - erase an address range on the flash chip.
 * @flash: pointer to lombo nor.
 * @addr: flash address for erase.
 * @len: length for erase.
 *
 * Return 0 if success, !0 error.
 */
int lombo_nor_erase(struct lombo_nor *flash, rt_uint32_t addr, rt_uint32_t len)
{
	int err = 0;
	rt_uint32_t tmp_addr, tmp_len, capacity;

	LOG_D("nor erase addr:0x%x len:%d", addr, len);

	capacity = flash->geometry.sector_count * flash->geometry.bytes_per_sector;

	if ((addr % flash->geometry.block_size) || (len % flash->geometry.block_size)) {
		LOG_E("unsupported addr:0x%0x len:%d", addr, len);
		return -RT_EINVAL;
	}

	if ((addr + len) > capacity) {
		LOG_E("unsupported addr:0x%0x len:%d capacity:%d",
			addr, len, capacity);
		return -RT_EINVAL;
	}

	tmp_addr = addr;
	tmp_len = len;

	rt_mutex_take(&flash->lock, RT_WAITING_FOREVER);

	/* whole-chip erase? */
	if ((0 == tmp_addr) && (tmp_len == capacity)) {
		err = lombo_erase_chip(flash);
		if (err) {
			LOG_E("chip erase ret:%d", err);
			rt_mutex_release(&flash->lock);
			return err;
		}

	/* REVISIT in some cases we could speed up erasing large regions
	 * by using OPCODE_SE instead of OPCODE_BE_4K.  We may have set up
	 * to use "small sector erase", but that's not always optimal. */

	/* "sector"-at-a-time erase */
	} else {
		while (tmp_len) {
			err = lombo_erase_sector(flash, tmp_addr);
			if (err) {
				LOG_E("sector erase ret:%d", err);
				rt_mutex_release(&flash->lock);
				return err;
			}

			tmp_addr += flash->geometry.block_size;
			tmp_len -= flash->geometry.block_size;
		}
	}

	rt_mutex_release(&flash->lock);

	return 0;
}

/**
 * lombo_nor_dev_init - init nor device.
 * @dev: device node.
 *
 * Return 0 if success, !0 error.
 */
static rt_err_t lombo_nor_dev_init(rt_device_t dev)
{
	return RT_EOK;
}

/**
 * lombo_nor_dev_open - open nor device.
 * @dev: device node.
 * @oflag: open flag.
 *
 * Return 0 if success, !0 error.
 */
static rt_err_t lombo_nor_dev_open(rt_device_t dev, rt_uint16_t oflag)
{
	return RT_EOK;
}

/**
 * lombo_nor_dev_close - close nor device.
 * @dev: device node.
 *
 * Return 0 if success, !0 error.
 */
static rt_err_t lombo_nor_dev_close(rt_device_t dev)
{
	return RT_EOK;
}

/**
 * lombo_nor_dev_read - read nor device.
 * @dev: device node.
 * @pos: read position, unit: byte.
 * @buffer: data buffer.
 * @size: read data size, unit: byte.
 *
 * Return actual length for read.
 */
static rt_size_t lombo_nor_dev_read(rt_device_t dev, rt_off_t pos,
	void *buffer, rt_size_t size)
{
	struct lombo_nor *flash = (struct lombo_nor *)dev->user_data;
	rt_uint32_t phy_pos = pos * flash->geometry.bytes_per_sector;
	rt_uint32_t phy_size = size * flash->geometry.bytes_per_sector;
	rt_uint32_t length = 0;

	if (lombo_nor_read(flash, phy_pos, phy_size, &length, buffer))
		return 0;
	else
		return size;
}

/**
 * lombo_nor_dev_write - write nor device.
 * @dev: device node.
 * @pos: write position, unit: byte.
 * @buffer: data buffer.
 * @size: write data size, unit: byte.
 *
 * Return actual length for write.
 */
static rt_size_t lombo_nor_dev_write(rt_device_t dev, rt_off_t pos,
	const void *buffer, rt_size_t size)
{
	struct lombo_nor *flash = (struct lombo_nor *)dev->user_data;
	rt_uint32_t phy_pos = pos * flash->geometry.bytes_per_sector;
	rt_uint32_t phy_size = size * flash->geometry.bytes_per_sector;
	rt_uint32_t length = 0;

	if (lombo_nor_erase(flash, phy_pos, phy_size))
		return 0;

	if (lombo_nor_write(flash, phy_pos, phy_size, &length, buffer))
		return 0;
	else
		return size;
}

/**
 * lombo_nor_dev_control - control nor device.
 * @dev: device node.
 * @cmd: control command.
 * @args: control command arguments.
 *
 * Return 0 if success, !0 error.
 */
static rt_err_t lombo_nor_dev_control(rt_device_t dev, int cmd, void *args)
{
	int err = 0;
	struct lombo_nor *flash;
	struct rt_device_blk_geometry *geometry;
	rt_uint32_t *addrs, start_addr, end_addr, phy_start_addr, phy_size;
	struct rt_device_blk_write *w_info;
	struct rt_device_otg_read *read_info;
	struct rt_device_otg_write *write_info;
	rt_uint32_t phy_pos;
	rt_uint32_t length;

	RT_ASSERT(dev != RT_NULL);

	flash = (struct lombo_nor *)dev->user_data;
	if (!flash) {
		LOG_E("may be this is not an lombo nor device");
		return -RT_EINVAL;
	}

	switch (cmd) {
	case RT_DEVICE_CTRL_WRITE_BLK:
		w_info = (struct rt_device_blk_write *)args;
		if (RT_NULL == w_info) {
			LOG_E("cmd:%u, unsupported argument:NULL", cmd);
			err = -RT_EINVAL;
			break;
		}

		phy_pos = w_info->pos * flash->geometry.bytes_per_sector;
		phy_size = w_info->size * flash->geometry.bytes_per_sector;
		lombo_nor_write(flash, phy_pos, phy_size, &length, w_info->buffer);

		err = length;
		break;

	case RT_DEVICE_CTRL_BLK_GETGEOME:
		geometry = (struct rt_device_blk_geometry *)args;

		if (RT_NULL == geometry) {
			LOG_E("cmd:%u, unsupported argument:NULL", cmd);
			err = -RT_EINVAL;
			break;
		}

		geometry->bytes_per_sector = flash->geometry.bytes_per_sector;
		geometry->sector_count = flash->geometry.sector_count;
		geometry->block_size = flash->geometry.block_size;
		err = 0;

		break;

	case RT_DEVICE_CTRL_BLK_ERASE:
		addrs = (rt_uint32_t *)args;
		if (!addrs) {
			LOG_E("cmd:%u, unsupported argument:NULL", cmd);
			err = -RT_EINVAL;
			break;
		}

		start_addr = addrs[0];
		end_addr = addrs[1];

		if (start_addr > end_addr) {
			LOG_E("cmd:%u, start addr(0x%x) greater than end addr(0x%x)",
				cmd, start_addr, end_addr);
			err = -RT_EINVAL;
			break;
		}

		if (end_addr == start_addr)
			end_addr++;

		phy_start_addr = start_addr * flash->geometry.bytes_per_sector;
		phy_size = (end_addr - start_addr) * flash->geometry.bytes_per_sector;

		err = lombo_nor_erase(flash, phy_start_addr, phy_size);
		break;

	case RT_DEVICE_CTRL_OTG_ERASE:
		err = lombo_nor_security_register_erase(flash);
		break;

	case RT_DEVICE_CTRL_OTG_READ:
		read_info = (struct rt_device_otg_read *)args;
		err = lombo_nor_security_register_read(flash, read_info->buffer,
							read_info->pos, read_info->size);
		break;

	case RT_DEVICE_CTRL_OTG_WRITE:
		write_info = (struct rt_device_otg_write *)args;
		err = lombo_nor_security_register_write(flash, write_info->buffer,
						write_info->pos, write_info->size);
		break;

	case RT_DEVICE_CTRL_OTG_SETT_LOCK:
		err = lombo_nor_security_register_erase(flash);

	default:
		LOG_W("unknow command");
		break;
	}

	return err;
}

#ifdef RT_USING_DEVICE_OPS
static const struct rt_device_ops lombo_nor_dev_ops = {
	lombo_nor_dev_init,
	lombo_nor_dev_open,
	lombo_nor_dev_close,
	lombo_nor_dev_read,
	lombo_nor_dev_write,
	lombo_nor_dev_control
};
#endif

/**
 * lombo_jedec_probe - probe flash by read jedec id.
 * @spi: pointer to the spi device.
 *
 * Return the pointer to flash info if success, otherwise ERR_PTR.
 */
static const struct flash_info *lombo_jedec_probe(struct rt_qspi_device *qspi)
{
	int			tmp;
	rt_uint8_t		code = OPCODE_RDID;
	rt_uint8_t		id[5];
	rt_uint32_t		jedec;
	rt_uint16_t		ext_jedec;
	const struct flash_info	*info;

	/*
	 * JEDEC also defines an optional "extended device information"
	 * string for after vendor-specific data, after the three bytes
	 * we use here.  Supporting some chips might require using it.
	 */
	tmp = __read_status_cmd(qspi, code, id, 5);
	if (tmp < 0) {
		LOG_E("read status ret:%d", tmp);
		return RT_NULL;
	}

	jedec = id[0];
	jedec = jedec << 8;
	jedec |= id[1];
	jedec = jedec << 8;
	jedec |= id[2];

	ext_jedec = id[3] << 8 | id[4];

	for (tmp = 0; tmp < ARRAY_SIZE(lombo_nor_ids) - 1; tmp++) {
		info = &lombo_nor_ids[tmp];
		if (info->jedec_id == jedec) {
			if (info->ext_id != 0 && info->ext_id != ext_jedec)
				continue;

			return &lombo_nor_ids[tmp];
		}
	}

	LOG_E("unrecognized JEDEC id 0x%06x", jedec);

	if (0xFFFFFF != (jedec & 0xFFFFFF))
		return &lombo_nor_ids[ARRAY_SIZE(lombo_nor_ids) - 1];
	else
		return RT_NULL;
}

/**
 * lombo_nor_get_read_if - get proper read data interface.
 * @flash: pointer to lombo nor.
 * @lines: max lines number of flash to transfer data.
 * @flags: flash info flags.
 *
 * Return 0 if success, 0! error.
 */
static int lombo_nor_get_read_if(struct lombo_nor *flash,
	rt_uint8_t lines, rt_uint16_t flags)
{
	RT_ASSERT(RT_NULL != flash);

	if ((4 == lines) && ((flags & QUAD_READ) || (flags & DUAL_READ))) {
		if (flags & QUAD_READ) {
#ifndef ARCH_LOMBO_N7V0
			flash->read_buf32 = __read_buf_32_quad;
#endif
			flash->read_buf8 = __read_buf_8_quad;
		} else {
#ifndef ARCH_LOMBO_N7V0
			flash->read_buf32 = __read_buf_32_dual;
#endif
			flash->read_buf8 = __read_buf_8_dual;
		}
	} else if ((2 == lines) && (flags & DUAL_READ)) {
#ifndef ARCH_LOMBO_N7V0
		flash->read_buf32 = __read_buf_32_dual;
#endif
		flash->read_buf8 = __read_buf_8_dual;
	} else {
#ifndef ARCH_LOMBO_N7V0
		flash->read_buf32 = __read_buf_32;
#endif
		flash->read_buf8 = __read_buf_8;
	}

	/* quad read mode is supported, set quad mode enable */
	if (flash->read_buf8 == __read_buf_8_quad)
		return lombo_set_quad_mode(flash, 1);

	return 0;
}

/**
 * lombo_nor_parse_config - parse nor config.
 * @flash_name: pointer to the nor device's name.
 * @qcfg: pointer to qspi configuration struct.
 *
 * Return 0 if success, 0! error.
 */
static int lombo_nor_parse_config(const char *flash_name,
	struct rt_qspi_configuration *qcfg)
{
	int err = 0;
	rt_uint32_t item;

	RT_ASSERT(RT_NULL != flash_name);
	RT_ASSERT(RT_NULL != qcfg);

	/* try to get msb-lsb */
	err = config_get_u32(flash_name, "msb-lsb", &item);
	if (err) {
		LOG_E("%s: failed to get msb-lsb item", flash_name);
		return -RT_EIO;
	}
	if (item)
		qcfg->parent.mode |= RT_SPI_MSB;
	else
		qcfg->parent.mode &= ~RT_SPI_MSB;

	/* try to get cpol-cpha */
	err = config_get_u32(flash_name, "cpol-cpha", &item);
	if (err) {
		LOG_E("%s: failed to get cpol-cpha item", flash_name);
		return -RT_EIO;
	}
	if (!((RT_SPI_MODE_0 == item) || (RT_SPI_MODE_1 == item) ||
		(RT_SPI_MODE_2 == item) || (RT_SPI_MODE_3 == item))) {
		LOG_E("%s: illegal cpol-cpha %u", flash_name, item);
		return -RT_ERROR;
	}
	qcfg->parent.mode &= ~(RT_SPI_CPHA | RT_SPI_CPOL);
	qcfg->parent.mode |= item;

	/* try to get max-lines */
	err = config_get_u32(flash_name, "max-lines", &item);
	if (err) {
		LOG_E("%s: failed to get max-lines item", flash_name);
		return -RT_EIO;
	}
	if (!((1 == item) || (2 == item) || (4 == item))) {
		LOG_E("%s: illegal max-lines %u", flash_name, item);
		return -RT_ERROR;
	}
	qcfg->qspi_dl_width = item;

	/* try to get max-baud-rate */
	err = config_get_u32(flash_name, "max-baud-rate", &item);
	if (err) {
		LOG_E("%s: failed to get max-baud-rate item", flash_name);
		return -RT_EIO;
	}
	qcfg->parent.max_hz = item;

	return 0;
}

/**
 * lombo_nor_init - init nor device.
 * @flash_name: pointer to the nor device's name.
 *
 * Return 0 if success, 0! error.
 */
int lombo_nor_init(const char *flash_name)
{
	int err = 0;
	rt_uint32_t chip_select, capacity;
	const char *bus_name, *dev_name;
	const struct flash_info *info;
	struct lombo_nor *flash;
	struct rt_qspi_device *qspi;
	struct rt_qspi_configuration qcfg = NOR_DEFAULT_QSPI_CFG;

	RT_ASSERT(NULL != flash_name);

	/* get spi bus name, spi dev name, and chip select index */
	err = config_get_string(flash_name, "bus_name", &bus_name);
	if (err) {
		LOG_E("%s: failed to get bus name", flash_name);
		return -RT_EIO;
	}

	err = config_get_string(flash_name, "dev_name", &dev_name);
	if (err) {
		LOG_E("%s: failed to get dev name", flash_name);
		return -RT_EIO;
	}

	err = config_get_u32(flash_name, "chip-select", &chip_select);
	if (err) {
		LOG_E("%s: failed to get chip select", flash_name);
		return -RT_EIO;
	}

	/* alloc qspi device and attach device to spi bus */
	qspi = (struct rt_qspi_device *)rt_zalloc(sizeof(struct rt_qspi_device));
	if (!qspi) {
		LOG_E("%s: failed to malloc qspi device", flash_name);
		return -RT_ENOMEM;
	}

	err = rt_spi_bus_attach_device(&(qspi->parent), dev_name, bus_name,
		(void *)chip_select);
	if (err) {
		LOG_E("%s: failed to attach %s to %s", flash_name, dev_name, bus_name);
		rt_free(qspi);
		return -RT_ENOSYS;
	}

	/* parse nor configuration and config qspi */
	err = lombo_nor_parse_config(flash_name, &qcfg);
	if (err) {
		LOG_E("%s: failed to parse config", flash_name);
		goto out_unregister_qspi;
	}

	err = rt_qspi_configure(qspi, &qcfg);
	if (err) {
		LOG_E("%s: failed to config qspi device %s", flash_name, dev_name);
		goto out_unregister_qspi;
	}

	/* probe flash by read jedec id */
	info = lombo_jedec_probe(qspi);
	if (!info) {
		LOG_E("%s: failed to probe jedec id", flash_name);
		err = -RT_ERROR;
		goto out_unregister_qspi;
	}

	LOG_D("%s: found spi nor flash: %s", flash_name, info->name);

	/* malloc lombo_nor struct */
	flash = (struct lombo_nor *)rt_zalloc(sizeof(struct lombo_nor));
	if (!flash) {
		LOG_E("%s: failed to malloc lombo_nor struct", flash_name);
		err = -RT_ENOMEM;
		goto out_unregister_qspi;
	}

	/* init mutex */
	err = rt_mutex_init(&flash->lock, flash_name, RT_IPC_FLAG_FIFO);
	if (err) {
		LOG_E("%s: failed to init mutex", flash_name);
		goto out_free_flash;
	}

	flash->qspi = qspi;
	qspi->parent.user_data = flash;

	/* get flash parameter  */
	flash->flash_id = info->jedec_id;
	flash->page_size = info->page_size;
	flash->geometry.sector_count = info->n_sectors;
	flash->geometry.bytes_per_sector = info->sector_size;

	/* prefer "small sector" erase if possible */
	if (info->flags & SECT_4K) {
		flash->erase_opcode = OPCODE_BE_4K;
		flash->geometry.block_size = 4096;

		flash->geometry.sector_count *= (info->sector_size / 4096);
		flash->geometry.bytes_per_sector = flash->geometry.block_size;
	} else {
		flash->erase_opcode = OPCODE_SE;
		flash->geometry.block_size = info->sector_size;
	}

	/* something to do before using the flash */
	err = lombo_setup_flash(flash);
	if (err) {
		LOG_E("%s: failed to setup flash %s", flash_name);
		goto out_detach_mutex;
	}

	/* get read interface and enter quad mode if necessarily */
	err = lombo_nor_get_read_if(flash, qcfg.qspi_dl_width, info->flags);
	if (err) {
		LOG_E("%s: failed to get read interface", flash_name);
		goto out_detach_mutex;
	}

	/* calculate capacity and enter 4-byte address mode if necessarily */
	capacity = flash->geometry.sector_count * flash->geometry.bytes_per_sector;
	if (capacity > 0x1000000) {
		err = lombo_set_4Byte_address(flash, 1);
		if (err) {
			LOG_E("%s: failed to set 4-byte address mode", flash_name);
			goto out_detach_mutex;
		} else {
			LOG_D("%s: set 4-byte address mode ok", flash);
			flash->addr_4byte = 1;
		}
	} else {
		LOG_D("%s: use 3-byte address mode default", flash);
		flash->addr_4byte = 0;
	}

	/* repeat config spi for medium size */
	qcfg.medium_size = capacity;
	err = rt_qspi_configure(qspi, &qcfg);
	if (err) {
		LOG_E("%s: failed to config qspi device %s", flash_name, dev_name);
		err = -RT_EBUSY;
		goto out_detach_mutex;
	}

	LOG_D("%s: name:%s id:0x%06x (%d Kbytes)",
		flash_name, info->name, flash->flash_id, capacity >> 10);

#ifdef RT_USING_DEVICE_OPS
	flash->dev.ops = lombo_nor_dev_ops;
#else
	flash->dev.init = lombo_nor_dev_init;
	flash->dev.open = lombo_nor_dev_open;
	flash->dev.close = lombo_nor_dev_close;
	flash->dev.read = lombo_nor_dev_read;
	flash->dev.write = lombo_nor_dev_write;
	flash->dev.control = lombo_nor_dev_control;
#endif
	flash->dev.type = RT_Device_Class_Block;
	flash->dev.user_data = flash;

	/* register nor device */
	err = rt_device_register(&flash->dev, flash_name,
		RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_STANDALONE);
	if (err) {
		LOG_E("%s: failed to register as device", flash_name);
		err = -RT_EBUSY;
		goto out_detach_mutex;
	}

	return RT_EOK;

out_detach_mutex:
	rt_mutex_detach(&flash->lock);

out_free_flash:
	if (RT_NULL != flash)
		rt_free(flash);

out_unregister_qspi:
	rt_device_unregister(&(qspi->parent.parent));
	rt_free(qspi);

	return err;
}

/**
 * lombo_nor_get_device_priv - get nor device's private data.
 * @flash_name: pointer to the nor device's name.
 *
 * Return pointer of nor device's private data.
 */
struct lombo_nor *lombo_nor_get_device_priv(const char *flash_name)
{
	struct lombo_nor *flash;
	struct rt_device *rt_dev;

	rt_dev = rt_device_find(flash_name);
	if (RT_NULL == rt_dev) {
		LOG_E("flash %s may hasn't been register", flash_name);
		return RT_NULL;
	}

	flash = (struct lombo_nor *)rt_dev->user_data;

	return flash;
}


#if 1
#define WRITE_BUF_SIZE			(10)
#define READ_BUF_SIZE			(10)
#define NOR_DEV_NAME			"nor0"

static u32 next = 1;

/* create a random value */
static u32 random()
{
	next += rt_tick_get();
	next = next * 1103515245L + 12345;

	return (u32) (next / 65536L);
}

/* create a random value in range */
static u32 random2(u32 range)
{
	if (0 == range)
		return 0;

	next = random() % range;

	return next;
}

//static int nor_data_sample(int argc, char *argv[])
int nor_data_sample(void)
{
	rt_device_t nor_dev;
	rt_uint32_t i, random;
	struct rt_device_otg_read *read_info;
	struct rt_device_otg_write *write_info;
	int err = 0;
	char register_code[10]={'X','D','S','M','2','0','2','1','Y','Z'};
	/* check if flash has been init */
	nor_dev = rt_device_find(NOR_DEV_NAME);
	if (nor_dev) {
		LOG_W("spi device has been register");
	} else {
		/* init nor device */
		err = lombo_nor_init(NOR_DEV_NAME);
		if (err) {
			LOG_E("failed to init nor");
			return RT_ERROR;
		}
		nor_dev = rt_device_find(NOR_DEV_NAME);
		if (!nor_dev) {
			rt_kprintf("find %s failed!\n", NOR_DEV_NAME);
			return RT_ERROR;
		}
	}

	err = rt_device_open(nor_dev, RT_DEVICE_OFLAG_RDWR);
	if (err) {
		LOG_E("failed to open nor device");
		return RT_ERROR;
	}
		read_info = (struct rt_device_otg_read *)rt_zalloc(sizeof(struct rt_device_otg_read));
		if (!read_info) {
			LOG_E("failed to malloc read_info");
			return -RT_ENOMEM;
		}

		// read_info->pos = 0x00;
		// read_info->size = READ_BUF_SIZE;
		// rt_memset(read_info->buffer, 0x0, READ_BUF_SIZE);


		// write_info = (struct rt_device_otg_write *)rt_zalloc(sizeof(struct rt_device_otg_write));
		// if (!write_info) {
		// 	LOG_E("failed to malloc write_info");
		// 	err = -RT_ENOMEM;
		// 	goto out_unregister_read_info;
		// }

		//  write_info->pos = 0x001000;
		//  write_info->size = WRITE_BUF_SIZE;
        // *(write_info->buffer)='X';
        // *(write_info->buffer+1)='D';
        // *(write_info->buffer+2)='S';
	    // *(write_info->buffer+3)='M';
	    // *(write_info->buffer+4)='2';
	    // *(write_info->buffer+5)='0';
	    // *(write_info->buffer+6)='2';
	    // *(write_info->buffer+7)='1';
	    // *(write_info->buffer+8)='Y';
	    // *(write_info->buffer+9)='Z';

		//  err = rt_device_control(nor_dev, RT_DEVICE_CTRL_OTG_ERASE, RT_NULL);
		//   if (err) {
		//   	LOG_E("failed to erase nor flash");
		//  	err = -RT_ERROR;
		//   	goto out_unregister_write_info;
		//   }

		// err = rt_device_control(nor_dev, RT_DEVICE_CTRL_OTG_READ, read_info);
		// if (err) {
		// 	LOG_E("failed to write nor flash");
		// 	err = -RT_ERROR;
		// 	goto out_unregister_write_info;
		// }
		// for (i = 0; i < READ_BUF_SIZE; i++) {
		// 	if((*(read_info->buffer +i)) != 0xFF) {
		// 		LOG_E("data1 verify error");
		// 		err = -RT_ERROR;
		// 		goto out_unregister_write_info;
		// 	}
		// }

		//  err = rt_device_control(nor_dev, RT_DEVICE_CTRL_OTG_WRITE, write_info);
		//   if (err) {
		//   	LOG_E("failed to write nor flash");
		//   	err = -RT_ERROR;
		//   	goto out_unregister_write_info;
		//   }

		 read_info->size = WRITE_BUF_SIZE;
		 rt_memset(read_info->buffer, 0x0, WRITE_BUF_SIZE);
		 read_info->pos = 0x001000;

		 err = rt_device_control(nor_dev, RT_DEVICE_CTRL_OTG_READ, read_info);
		 if (err) {
		 	LOG_E("failed to read nor flash");
		 	err = RT_ERROR;
			goto out_unregister_write_info;
		 }
		 for (i = 0; i < WRITE_BUF_SIZE; i++) {	 
			// printf("\nread_buffer=======%c\n",*(read_info->buffer +i));
             if(register_code[i] !=(*(read_info->buffer +i)) ){
               err = RT_ERROR;
			  goto out_unregister_read_info;
			 }
		 }




		//  for (i = 0; i < WRITE_BUF_SIZE; i++) {	 
		//  	if((*(read_info->buffer +i)) != (*(write_info->buffer +i))) {
		//  		LOG_E("data verify error");
		//  		err = RT_ERROR;
		//  		goto out_unregister_write_info;
		//  	}
		//  }

	rt_device_close(nor_dev);
	rt_free(read_info);
	rt_free(write_info);
	rt_kprintf("security Register test succeed\n");
	return err;

out_unregister_write_info:
	rt_free(write_info);
out_unregister_read_info:
	rt_free(read_info);
	rt_device_close(nor_dev);
	rt_kprintf("security Register test failed\n");
	return err;
}
/* ������msh �����б���*/
//MSH_CMD_EXPORT(nor_data_sample, spi device sample);

#endif