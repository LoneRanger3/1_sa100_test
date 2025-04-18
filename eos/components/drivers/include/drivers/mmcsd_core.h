/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author		Notes
 * 2011-07-25     weety		first version
 */

#ifndef __CORE_H__
#define __CORE_H__

#include <rtthread.h>
#include <drivers/mmcsd_host.h>
#include <drivers/mmcsd_card.h>
#include <drivers/mmcsd_cmd.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef RT_MMCSD_DBG
#define mmcsd_dbg(fmt, ...)  rt_kprintf(fmt, ##__VA_ARGS__)
#else
#define mmcsd_dbg(fmt, ...)
#endif

struct rt_mmcsd_data {
	rt_uint32_t  blksize;
	rt_uint32_t  blks;
	rt_uint32_t  *buf;
	rt_int32_t  err;
	rt_uint32_t  flags;
#define DATA_DIR_WRITE	(1 << 0)
#define DATA_DIR_READ	(1 << 1)
#define DATA_STREAM	(1 << 2)

	unsigned int		bytes_xfered;

	struct rt_mmcsd_cmd	*stop;		/* stop command */
	struct rt_mmcsd_req	*mrq;		/* associated request */

	rt_uint32_t  timeout_ns;
	rt_uint32_t  timeout_clks;
};

struct rt_mmcsd_cmd {
	rt_uint32_t  cmd_code;
	rt_uint32_t  arg;
	rt_uint32_t  resp[4];
	rt_uint32_t  flags;
/*rsponse types 
 *bits:0~3
 */
#define RESP_MASK	(0xF)
#define RESP_NONE	(0)
#define RESP_R1		(1 << 0)
#define RESP_R1B	(2 << 0)
#define RESP_R2		(3 << 0)
#define RESP_R3		(4 << 0)
#define RESP_R4		(5 << 0)
#define RESP_R6		(6 << 0)
#define RESP_R7		(7 << 0)
#define RESP_R5		(8 << 0)	/*SDIO command response type*/
/*command types 
 *bits:4~5
 */
#define CMD_MASK	(3 << 4)		/* command type */
#define CMD_AC		(0 << 4)
#define CMD_ADTC	(1 << 4)
#define CMD_BC		(2 << 4)
#define CMD_BCR		(3 << 4)

#define resp_type(cmd)	((cmd)->flags & RESP_MASK)

/*spi rsponse types 
 *bits:6~8
 */
#define RESP_SPI_MASK	(0x7 << 6)
#define RESP_SPI_R1	(1 << 6)
#define RESP_SPI_R1B	(2 << 6)
#define RESP_SPI_R2	(3 << 6)
#define RESP_SPI_R3	(4 << 6)
#define RESP_SPI_R4	(5 << 6)
#define RESP_SPI_R5	(6 << 6)
#define RESP_SPI_R7	(7 << 6)

#define spi_resp_type(cmd)	((cmd)->flags & RESP_SPI_MASK)
/*
 * These are the command types.
 */
#define cmd_type(cmd)	((cmd)->flags & CMD_MASK)
	
	rt_int32_t  retries;	/* max number of retries */
	rt_int32_t  err;

#ifdef ARCH_LOMBO
	rt_uint32_t cmd_timeout_ms;		/* in milliseconds */
#endif

	struct rt_mmcsd_data *data;
	struct rt_mmcsd_req	*mrq;		/* associated request */
};

struct rt_mmcsd_req {
	struct rt_mmcsd_data  *data;
	struct rt_mmcsd_cmd   *cmd;
	struct rt_mmcsd_cmd   *stop;
};

#ifdef ARCH_LOMBO
	extern rt_int16_t g_sd_speedclass;
#endif

/*the following is response bit*/
#define R1_OUT_OF_RANGE		(1 << 31)	/* er, c */
#define R1_ADDRESS_ERROR	(1 << 30)	/* erx, c */
#define R1_BLOCK_LEN_ERROR	(1 << 29)	/* er, c */
#define R1_ERASE_SEQ_ERROR      (1 << 28)	/* er, c */
#define R1_ERASE_PARAM		(1 << 27)	/* ex, c */
#define R1_WP_VIOLATION		(1 << 26)	/* erx, c */
#define R1_CARD_IS_LOCKED	(1 << 25)	/* sx, a */
#define R1_LOCK_UNLOCK_FAILED	(1 << 24)	/* erx, c */
#define R1_COM_CRC_ERROR	(1 << 23)	/* er, b */
#define R1_ILLEGAL_COMMAND	(1 << 22)	/* er, b */
#define R1_CARD_ECC_FAILED	(1 << 21)	/* ex, c */
#define R1_CC_ERROR		(1 << 20)	/* erx, c */
#define R1_ERROR		(1 << 19)	/* erx, c */
#define R1_UNDERRUN		(1 << 18)	/* ex, c */
#define R1_OVERRUN		(1 << 17)	/* ex, c */
#define R1_CID_CSD_OVERWRITE	(1 << 16)	/* erx, c, CID/CSD overwrite */
#define R1_WP_ERASE_SKIP	(1 << 15)	/* sx, c */
#define R1_CARD_ECC_DISABLED	(1 << 14)	/* sx, a */
#define R1_ERASE_RESET		(1 << 13)	/* sr, c */
#define R1_STATUS(x)            (x & 0xFFFFE000)
#define R1_CURRENT_STATE(x)	((x & 0x00001E00) >> 9)	/* sx, b (4 bits) */
#define R1_READY_FOR_DATA	(1 << 8)	/* sx, a */
#define R1_APP_CMD		(1 << 5)	/* sr, c */


#define R1_SPI_IDLE		(1 << 0)
#define R1_SPI_ERASE_RESET	(1 << 1)
#define R1_SPI_ILLEGAL_COMMAND	(1 << 2)
#define R1_SPI_COM_CRC		(1 << 3)
#define R1_SPI_ERASE_SEQ	(1 << 4)
#define R1_SPI_ADDRESS		(1 << 5)
#define R1_SPI_PARAMETER	(1 << 6)
/* R1 bit 7 is always zero */
#define R2_SPI_CARD_LOCKED	(1 << 8)
#define R2_SPI_WP_ERASE_SKIP	(1 << 9)	/* or lock/unlock fail */
#define R2_SPI_LOCK_UNLOCK_FAIL	R2_SPI_WP_ERASE_SKIP
#define R2_SPI_ERROR		(1 << 10)
#define R2_SPI_CC_ERROR		(1 << 11)
#define R2_SPI_CARD_ECC_ERROR	(1 << 12)
#define R2_SPI_WP_VIOLATION	(1 << 13)
#define R2_SPI_ERASE_PARAM	(1 << 14)
#define R2_SPI_OUT_OF_RANGE	(1 << 15)	/* or CSD overwrite */
#define R2_SPI_CSD_OVERWRITE	R2_SPI_OUT_OF_RANGE

#define CARD_BUSY	0x80000000	/* Card Power up status bit */

/* R5 response bits */
#define R5_COM_CRC_ERROR	(1 << 15)
#define R5_ILLEGAL_COMMAND	(1 << 14)
#define R5_ERROR			(1 << 11)
#define R5_FUNCTION_NUMBER	(1 << 9)
#define R5_OUT_OF_RANGE		(1 << 8)
#define R5_STATUS(x)		(x & 0xCB00)
#define R5_IO_CURRENT_STATE(x)	((x & 0x3000) >> 12)

#ifdef ARCH_LOMBO
#define MMC_ERASE_ARG		0x00000000
#define MMC_SECURE_ERASE_ARG	0x80000000
#define MMC_TRIM_ARG		0x00000001
#define MMC_DISCARD_ARG		0x00000003
#define MMC_SECURE_TRIM1_ARG	0x80000001
#define MMC_SECURE_TRIM2_ARG	0x80008000

#define MMC_SECURE_ARGS		0x80000000
#define MMC_TRIM_ARGS		0x00008001

/*
 * Card Command Classes (CCC)
 */
#define CCC_BASIC		(1<<0)	/* (0) Basic protocol functions */
					/* (CMD0,1,2,3,4,7,9,10,12,13,15) */
					/* (and for SPI, CMD58,59) */
#define CCC_STREAM_READ		(1<<1)	/* (1) Stream read commands */
					/* (CMD11) */
#define CCC_BLOCK_READ		(1<<2)	/* (2) Block read commands */
					/* (CMD16,17,18) */
#define CCC_STREAM_WRITE	(1<<3)	/* (3) Stream write commands */
					/* (CMD20) */
#define CCC_BLOCK_WRITE		(1<<4)	/* (4) Block write commands */
					/* (CMD16,24,25,26,27) */
#define CCC_ERASE		(1<<5)	/* (5) Ability to erase blocks */
					/* (CMD32,33,34,35,36,37,38,39) */
#define CCC_WRITE_PROT		(1<<6)	/* (6) Able to write protect blocks */
					/* (CMD28,29,30) */
#define CCC_LOCK_CARD		(1<<7)	/* (7) Able to lock down card */
					/* (CMD16,CMD42) */
#define CCC_APP_SPEC		(1<<8)	/* (8) Application specific */
					/* (CMD55,56,57,ACMD*) */
#define CCC_IO_MODE		(1<<9)	/* (9) I/O mode */
					/* (CMD5,39,40,52,53) */
#define CCC_SWITCH		(1<<10)	/* (10) High speed switch */
					/* (CMD6,34,35,36,37,50) */
					/* (11) Reserved */
					/* (CMD?) */
#endif

/**
 * fls - find last (most-significant) bit set
 * @x: the word to search
 *
 * This is defined the same way as ffs.
 * Note fls(0) = 0, fls(1) = 1, fls(0x80000000) = 32.
 */

rt_inline rt_uint32_t __rt_fls(rt_uint32_t val)
{
	rt_uint32_t  bit = 32;

	if (!val)
		return 0;
	if (!(val & 0xffff0000u)) 
	{
		val <<= 16;
		bit -= 16;
	}
	if (!(val & 0xff000000u)) 
	{
		val <<= 8;
		bit -= 8;
	}
	if (!(val & 0xf0000000u)) 
	{
		val <<= 4;
		bit -= 4;
	}
	if (!(val & 0xc0000000u)) 
	{
		val <<= 2;
		bit -= 2;
	}
	if (!(val & 0x80000000u)) 
	{
		bit -= 1;
	}

	return bit;
}

#define MMCSD_HOST_PLUGED       0
#define MMCSD_HOST_UNPLUGED     1

#ifdef ARCH_LOMBO
#define MMCSD_PART_MOUNT_OK	(1UL << 0)
#define MMCSD_PART_MOUNT_FAIL	(1UL << 1)
#define MMCSD_PART_UMOUNT	(1UL << 2)

#define CARD_REINIT_WHEN_CHARGE	(1)	/* reinit when charge if card has power down */
#define CARD_PERF_MONITOR	(1)	/* performance monitor */
#endif

int mmcsd_wait_cd_changed(rt_int32_t timeout);
void mmcsd_host_lock(struct rt_mmcsd_host *host);
void mmcsd_host_unlock(struct rt_mmcsd_host *host);
void mmcsd_req_complete(struct rt_mmcsd_host *host);
void mmcsd_send_request(struct rt_mmcsd_host *host, struct rt_mmcsd_req *req);
rt_int32_t mmcsd_send_cmd(struct rt_mmcsd_host *host, struct rt_mmcsd_cmd *cmd, int retries);
rt_int32_t mmcsd_go_idle(struct rt_mmcsd_host *host);
rt_int32_t mmcsd_spi_read_ocr(struct rt_mmcsd_host *host, rt_int32_t high_capacity, rt_uint32_t *ocr);
rt_int32_t mmcsd_all_get_cid(struct rt_mmcsd_host *host, rt_uint32_t *cid);
rt_int32_t mmcsd_get_cid(struct rt_mmcsd_host *host, rt_uint32_t *cid);
rt_int32_t mmcsd_get_csd(struct rt_mmcsd_card *card, rt_uint32_t *csd);
rt_int32_t mmcsd_select_card(struct rt_mmcsd_card *card);
rt_int32_t mmcsd_deselect_cards(struct rt_mmcsd_card *host);
rt_int32_t mmcsd_spi_use_crc(struct rt_mmcsd_host *host, rt_int32_t use_crc);
void mmcsd_set_chip_select(struct rt_mmcsd_host *host, rt_int32_t mode);
void mmcsd_set_clock(struct rt_mmcsd_host *host, rt_uint32_t clk);
void mmcsd_set_bus_mode(struct rt_mmcsd_host *host, rt_uint32_t mode);
void mmcsd_set_bus_width(struct rt_mmcsd_host *host, rt_uint32_t width);
void mmcsd_set_data_timeout(struct rt_mmcsd_data *data, const struct rt_mmcsd_card *card);
rt_uint32_t mmcsd_select_voltage(struct rt_mmcsd_host *host, rt_uint32_t ocr);
#ifndef ARCH_LOMBO
void mmcsd_change(struct rt_mmcsd_host *host);
void mmcsd_detect(void *param);
struct rt_mmcsd_host *mmcsd_alloc_host(void);
#else
void mmcsd_init_erase(struct rt_mmcsd_card *card);
int mmcsd_erase(struct rt_mmcsd_card *card, rt_uint32_t from, rt_uint32_t nr,
		rt_uint32_t arg);
int mmcsd_can_erase(struct rt_mmcsd_card *card);
int mmcsd_can_trim(struct rt_mmcsd_card *card);
int mmcsd_can_discard(struct rt_mmcsd_card *card);
int mmcsd_can_sanitize(struct rt_mmcsd_card *card);
int mmcsd_can_secure_erase_trim(struct rt_mmcsd_card *card);
int mmsdc_erase_group_aligned(struct rt_mmcsd_card *card, rt_uint32_t from,
			      rt_uint32_t nr);
rt_uint32_t mmcsd_calc_max_discard(struct rt_mmcsd_card *card);

int mmcsd_wait_partition_mount(rt_int32_t timeout);
void mmcsd_send_partition_mount_event(rt_uint32_t event);
void mmcsd_change(struct rt_mmcsd_host *host, rt_uint32_t status);
#if CARD_REINIT_WHEN_CHARGE
void mmcsd_change_virt(struct rt_mmcsd_host *host);
#endif
struct rt_mmcsd_host *mmcsd_alloc_host(rt_uint32_t index);
#endif
void mmcsd_free_host(struct rt_mmcsd_host *host);
int rt_mmcsd_core_init(void);

int rt_mmcsd_blk_init(void);
rt_int32_t rt_mmcsd_blk_probe(struct rt_mmcsd_card *card);
void rt_mmcsd_blk_remove(struct rt_mmcsd_card *card);


#ifdef __cplusplus
}
#endif

#endif
