/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2011-07-25     weety         first version
 */

#include <rtthread.h>
#include <drivers/mmcsd_core.h>
#include <drivers/sd.h>
#include <drivers/mmc.h>
#include <drivers/sdio.h>

#define DBG_TAG               "SDIO"
#ifdef RT_SDIO_DEBUG
#define DBG_LVL               DBG_LOG
#else
#ifdef ARCH_LOMBO
#define DBG_LVL               DBG_INFO
#else
#define DBG_LVL               DBG_INFO
#endif
#endif /* RT_SDIO_DEBUG */
#include <rtdbg.h>

#ifdef ARCH_LOMBO
#include "board.h"
#include "system/system_mq.h"
#endif

#ifndef RT_MMCSD_STACK_SIZE
#define RT_MMCSD_STACK_SIZE 1024
#endif
#ifndef RT_MMCSD_THREAD_PREORITY
#if (RT_THREAD_PRIORITY_MAX == 32)
#define RT_MMCSD_THREAD_PREORITY  0x16
#else
#define RT_MMCSD_THREAD_PREORITY  0x40
#endif
#endif

#ifdef ARCH_LOMBO
rt_int16_t g_sd_plugout;
rt_int16_t g_sd_speedclass = -2;
static struct rt_event mmcsd_mount_event;
#else	/* ARCH_LOMBO */
//static struct rt_semaphore mmcsd_sem;
static struct rt_thread mmcsd_detect_thread;
static rt_uint8_t mmcsd_stack[RT_MMCSD_STACK_SIZE];
static struct rt_mailbox  mmcsd_detect_mb;
static rt_uint32_t mmcsd_detect_mb_pool[4];
#endif
static struct rt_mailbox mmcsd_hotpluge_mb;
static rt_uint32_t mmcsd_hotpluge_mb_pool[4];

void mmcsd_host_lock(struct rt_mmcsd_host *host)
{
    rt_mutex_take(&host->bus_lock, RT_WAITING_FOREVER);
}

void mmcsd_host_unlock(struct rt_mmcsd_host *host)
{
    rt_mutex_release(&host->bus_lock);
}

void mmcsd_req_complete(struct rt_mmcsd_host *host)
{
    rt_sem_release(&host->sem_ack);
}

void mmcsd_send_request(struct rt_mmcsd_host *host, struct rt_mmcsd_req *req)
{
    do {
        req->cmd->retries--;
        req->cmd->err = 0;
        req->cmd->mrq = req;
        if (req->data)
        {   
            req->cmd->data = req->data;
            req->data->err = 0;
            req->data->mrq = req;
            if (req->stop)
            {
                req->data->stop = req->stop;
                req->stop->err = 0;
                req->stop->mrq = req;
            }       
        }
        host->ops->request(host, req);

        rt_sem_take(&host->sem_ack, RT_WAITING_FOREVER);
          
    } while(req->cmd->err && (req->cmd->retries > 0));


}

rt_int32_t mmcsd_send_cmd(struct rt_mmcsd_host *host,
                          struct rt_mmcsd_cmd  *cmd,
                          int                   retries)
{
    struct rt_mmcsd_req req;

    rt_memset(&req, 0, sizeof(struct rt_mmcsd_req));
    rt_memset(cmd->resp, 0, sizeof(cmd->resp));
    cmd->retries = retries;

    req.cmd = cmd;
    cmd->data = RT_NULL;

    mmcsd_send_request(host, &req);

    return cmd->err;
}

rt_int32_t mmcsd_go_idle(struct rt_mmcsd_host *host)
{
    rt_int32_t err;
    struct rt_mmcsd_cmd cmd;

    if (!controller_is_spi(host))
    {
        mmcsd_set_chip_select(host, MMCSD_CS_HIGH);
#ifdef ARCH_LOMBO
		mdelay(1);
#else
        mmcsd_delay_ms(1);
#endif
    }

    rt_memset(&cmd, 0, sizeof(struct rt_mmcsd_cmd));

    cmd.cmd_code = GO_IDLE_STATE;
    cmd.arg = 0;
    cmd.flags = RESP_SPI_R1 | RESP_NONE | CMD_BC;

    err = mmcsd_send_cmd(host, &cmd, 0);

#ifdef ARCH_LOMBO
	mdelay(1);
#else
    mmcsd_delay_ms(1);
#endif

    if (!controller_is_spi(host)) 
    {
        mmcsd_set_chip_select(host, MMCSD_CS_IGNORE);
#ifdef ARCH_LOMBO
		mdelay(1);
#else
        mmcsd_delay_ms(1);
#endif
    }

    return err;
}

rt_int32_t mmcsd_spi_read_ocr(struct rt_mmcsd_host *host,
                              rt_int32_t            high_capacity,
                              rt_uint32_t          *ocr)
{
    struct rt_mmcsd_cmd cmd;
    rt_int32_t err;

    rt_memset(&cmd, 0, sizeof(struct rt_mmcsd_cmd));

    cmd.cmd_code = SPI_READ_OCR;
    cmd.arg = high_capacity ? (1 << 30) : 0;
    cmd.flags = RESP_SPI_R3;

    err = mmcsd_send_cmd(host, &cmd, 0);

    *ocr = cmd.resp[1];

    return err;
}

rt_int32_t mmcsd_all_get_cid(struct rt_mmcsd_host *host, rt_uint32_t *cid)
{
    rt_int32_t err;
    struct rt_mmcsd_cmd cmd;

    rt_memset(&cmd, 0, sizeof(struct rt_mmcsd_cmd));

    cmd.cmd_code = ALL_SEND_CID;
    cmd.arg = 0;
    cmd.flags = RESP_R2 | CMD_BCR;

    err = mmcsd_send_cmd(host, &cmd, 3);
    if (err)
        return err;

    rt_memcpy(cid, cmd.resp, sizeof(rt_uint32_t) * 4);

    return 0;
}

rt_int32_t mmcsd_get_cid(struct rt_mmcsd_host *host, rt_uint32_t *cid)
{
    rt_int32_t err, i;
    struct rt_mmcsd_req req;
    struct rt_mmcsd_cmd cmd;
    struct rt_mmcsd_data data;
    rt_uint32_t *buf = RT_NULL;

    if (!controller_is_spi(host)) 
    {
        if (!host->card)
            return -RT_ERROR;
        rt_memset(&cmd, 0, sizeof(struct rt_mmcsd_cmd));

        cmd.cmd_code = SEND_CID;
        cmd.arg = host->card->rca << 16;
        cmd.flags = RESP_R2 | CMD_AC;
        err = mmcsd_send_cmd(host, &cmd, 3);
        if (err)
            return err;

        rt_memcpy(cid, cmd.resp, sizeof(rt_uint32_t) * 4);

        return 0;
    }

    buf = (rt_uint32_t *)rt_malloc(16);
    if (!buf) 
    {
        LOG_E("allocate memory failed!");

        return -RT_ENOMEM;
    }

    rt_memset(&req, 0, sizeof(struct rt_mmcsd_req));
    rt_memset(&cmd, 0, sizeof(struct rt_mmcsd_cmd));
    rt_memset(&data, 0, sizeof(struct rt_mmcsd_data));

    req.cmd = &cmd;
    req.data = &data;

    cmd.cmd_code = SEND_CID;
    cmd.arg = 0;

    /* NOTE HACK:  the RESP_SPI_R1 is always correct here, but we
     * rely on callers to never use this with "native" calls for reading
     * CSD or CID.  Native versions of those commands use the R2 type,
     * not R1 plus a data block.
     */
    cmd.flags = RESP_SPI_R1 | RESP_R1 | CMD_ADTC;

    data.blksize = 16;
    data.blks = 1;
    data.flags = DATA_DIR_READ;
    data.buf = buf;
    /*
     * The spec states that CSR and CID accesses have a timeout
     * of 64 clock cycles.
     */
    data.timeout_ns = 0;
    data.timeout_clks = 64;

    mmcsd_send_request(host, &req);

    if (cmd.err || data.err)
    {
        rt_free(buf);

        return -RT_ERROR;
    }

    for (i = 0;i < 4;i++)
        cid[i] = buf[i];
    rt_free(buf);

    return 0;
}

rt_int32_t mmcsd_get_csd(struct rt_mmcsd_card *card, rt_uint32_t *csd)
{
    rt_int32_t err, i;
    struct rt_mmcsd_req req;
    struct rt_mmcsd_cmd cmd;
    struct rt_mmcsd_data data;
    rt_uint32_t *buf = RT_NULL;

    if (!controller_is_spi(card->host))
    {
        rt_memset(&cmd, 0, sizeof(struct rt_mmcsd_cmd));

        cmd.cmd_code = SEND_CSD;
        cmd.arg = card->rca << 16;
        cmd.flags = RESP_R2 | CMD_AC;
        err = mmcsd_send_cmd(card->host, &cmd, 3);
        if (err)
            return err;

        rt_memcpy(csd, cmd.resp, sizeof(rt_uint32_t) * 4);

        return 0;
    }

    buf = (rt_uint32_t*)rt_malloc(16);
    if (!buf) 
    {
        LOG_E("allocate memory failed!");

        return -RT_ENOMEM;
    }

    rt_memset(&req, 0, sizeof(struct rt_mmcsd_req));
    rt_memset(&cmd, 0, sizeof(struct rt_mmcsd_cmd));
    rt_memset(&data, 0, sizeof(struct rt_mmcsd_data));

    req.cmd = &cmd;
    req.data = &data;

    cmd.cmd_code = SEND_CSD;
    cmd.arg = 0;

    /* NOTE HACK:  the RESP_SPI_R1 is always correct here, but we
     * rely on callers to never use this with "native" calls for reading
     * CSD or CID.  Native versions of those commands use the R2 type,
     * not R1 plus a data block.
     */
    cmd.flags = RESP_SPI_R1 | RESP_R1 | CMD_ADTC;

    data.blksize = 16;
    data.blks = 1;
    data.flags = DATA_DIR_READ;
    data.buf = buf;

    /*
     * The spec states that CSR and CID accesses have a timeout
     * of 64 clock cycles.
     */
    data.timeout_ns = 0;
    data.timeout_clks = 64;

    mmcsd_send_request(card->host, &req);

    if (cmd.err || data.err)
    {
        rt_free(buf);

        return -RT_ERROR;
    }

    for (i = 0;i < 4;i++)
        csd[i] = buf[i];
    rt_free(buf);

    return 0;
}

static rt_int32_t _mmcsd_select_card(struct rt_mmcsd_host *host,
                                     struct rt_mmcsd_card *card)
{
    rt_int32_t err;
    struct rt_mmcsd_cmd cmd;

    rt_memset(&cmd, 0, sizeof(struct rt_mmcsd_cmd));

    cmd.cmd_code = SELECT_CARD;

    if (card) 
    {
        cmd.arg = card->rca << 16;
        cmd.flags = RESP_R1 | CMD_AC;
    } 
    else 
    {
        cmd.arg = 0;
        cmd.flags = RESP_NONE | CMD_AC;
    }

    err = mmcsd_send_cmd(host, &cmd, 3);
    if (err)
        return err;

    return 0;
}

rt_int32_t mmcsd_select_card(struct rt_mmcsd_card *card)
{
    return _mmcsd_select_card(card->host, card);
}

rt_int32_t mmcsd_deselect_cards(struct rt_mmcsd_card *card)
{
    return _mmcsd_select_card(card->host, RT_NULL);
}

rt_int32_t mmcsd_spi_use_crc(struct rt_mmcsd_host *host, rt_int32_t use_crc)
{
    struct rt_mmcsd_cmd cmd;
    rt_int32_t err;

    rt_memset(&cmd, 0, sizeof(struct rt_mmcsd_cmd));

    cmd.cmd_code = SPI_CRC_ON_OFF;
    cmd.flags = RESP_SPI_R1;
    cmd.arg = use_crc;

    err = mmcsd_send_cmd(host, &cmd, 0);
    if (!err)
        host->spi_use_crc = use_crc;

    return err;
}

rt_inline void mmcsd_set_iocfg(struct rt_mmcsd_host *host)
{
    struct rt_mmcsd_io_cfg *io_cfg = &host->io_cfg;

    mmcsd_dbg("clock %uHz busmode %u powermode %u cs %u Vdd %u "
        "width %u \n",
         io_cfg->clock, io_cfg->bus_mode,
         io_cfg->power_mode, io_cfg->chip_select, io_cfg->vdd,
         io_cfg->bus_width);

    host->ops->set_iocfg(host, io_cfg);
}

/*
 * Control chip select pin on a host.
 */
void mmcsd_set_chip_select(struct rt_mmcsd_host *host, rt_int32_t mode)
{
    host->io_cfg.chip_select = mode;
    mmcsd_set_iocfg(host);
}

/*
 * Sets the host clock to the highest possible frequency that
 * is below "hz".
 */
void mmcsd_set_clock(struct rt_mmcsd_host *host, rt_uint32_t clk)
{
    if (clk < host->freq_min)
    {
        LOG_W("clock too low!");
    }

    host->io_cfg.clock = clk;
    mmcsd_set_iocfg(host);
}

/*
 * Change the bus mode (open drain/push-pull) of a host.
 */
void mmcsd_set_bus_mode(struct rt_mmcsd_host *host, rt_uint32_t mode)
{
    host->io_cfg.bus_mode = mode;
    mmcsd_set_iocfg(host);
}

/*
 * Change data bus width of a host.
 */
void mmcsd_set_bus_width(struct rt_mmcsd_host *host, rt_uint32_t width)
{
    host->io_cfg.bus_width = width;
    mmcsd_set_iocfg(host);
}

void mmcsd_set_data_timeout(struct rt_mmcsd_data       *data,
                            const struct rt_mmcsd_card *card)
{
    rt_uint32_t mult;

    if (card->card_type == CARD_TYPE_SDIO) 
    {
        data->timeout_ns = 1000000000;  /* SDIO card 1s */
        data->timeout_clks = 0;

        return;
    }

    /*
     * SD cards use a 100 multiplier rather than 10
     */
    mult = (card->card_type == CARD_TYPE_SD) ? 100 : 10;

    /*
     * Scale up the multiplier (and therefore the timeout) by
     * the r2w factor for writes.
     */
    if (data->flags & DATA_DIR_WRITE)
        mult <<= card->csd.r2w_factor;

    data->timeout_ns = card->tacc_ns * mult;
    data->timeout_clks = card->tacc_clks * mult;

    /*
     * SD cards also have an upper limit on the timeout.
     */
    if (card->card_type == CARD_TYPE_SD) 
    {
        rt_uint32_t timeout_us, limit_us;

        timeout_us = data->timeout_ns / 1000;
        timeout_us += data->timeout_clks * 1000 /
            (card->host->io_cfg.clock / 1000);

        if (data->flags & DATA_DIR_WRITE)
            /*
             * The limit is really 250 ms, but that is
             * insufficient for some crappy cards.
             */
            limit_us = 300000;
        else
            limit_us = 100000;

        /*
         * SDHC cards always use these fixed values.
         */
        if (timeout_us > limit_us || card->flags & CARD_FLAG_SDHC) 
        {
            data->timeout_ns = limit_us * 1000; /* SDHC card fixed 250ms */
            data->timeout_clks = 0;
        }
    }

    if (controller_is_spi(card->host)) 
    {
        if (data->flags & DATA_DIR_WRITE) 
        {
            if (data->timeout_ns < 1000000000)
                data->timeout_ns = 1000000000;  /* 1s */
        } 
        else 
        {
            if (data->timeout_ns < 100000000)
                data->timeout_ns =  100000000;  /* 100ms */
        }
    }
}

/*
 * Mask off any voltages we don't support and select
 * the lowest voltage
 */
rt_uint32_t mmcsd_select_voltage(struct rt_mmcsd_host *host, rt_uint32_t ocr)
{
    int bit;
    extern int __rt_ffs(int value);

    ocr &= host->valid_ocr;

    bit = __rt_ffs(ocr);
    if (bit) 
    {
        bit -= 1;

        ocr &= 3 << bit;

        host->io_cfg.vdd = bit;
        mmcsd_set_iocfg(host);
    } 
    else 
    {
        LOG_W("host doesn't support card's voltages!");
        ocr = 0;
    }

    return ocr;
}

static void mmcsd_power_up(struct rt_mmcsd_host *host)
{
    int bit = __rt_fls(host->valid_ocr) - 1;

    host->io_cfg.vdd = bit;
    if (controller_is_spi(host))
    {
        host->io_cfg.chip_select = MMCSD_CS_HIGH;
        host->io_cfg.bus_mode = MMCSD_BUSMODE_PUSHPULL;
    } 
    else
    {
        host->io_cfg.chip_select = MMCSD_CS_IGNORE;
        host->io_cfg.bus_mode = MMCSD_BUSMODE_OPENDRAIN;
    }
    host->io_cfg.power_mode = MMCSD_POWER_UP;
    host->io_cfg.bus_width = MMCSD_BUS_WIDTH_1;
    mmcsd_set_iocfg(host);

    /*
     * This delay should be sufficient to allow the power supply
     * to reach the minimum voltage.
     */
    mmcsd_delay_ms(10);

    host->io_cfg.clock = host->freq_min;
    host->io_cfg.power_mode = MMCSD_POWER_ON;
    mmcsd_set_iocfg(host);

    /*
     * This delay must be at least 74 clock sizes, or 1 ms, or the
     * time required to reach a stable voltage.
     */
#ifdef ARCH_LOMBO
	mdelay(1);
#else
    mmcsd_delay_ms(10);
#endif
}

static void mmcsd_power_off(struct rt_mmcsd_host *host)
{
    host->io_cfg.clock = 0;
    host->io_cfg.vdd = 0;
    if (!controller_is_spi(host)) 
    {
        host->io_cfg.bus_mode = MMCSD_BUSMODE_OPENDRAIN;
        host->io_cfg.chip_select = MMCSD_CS_IGNORE;
    }
    host->io_cfg.power_mode = MMCSD_POWER_OFF;
    host->io_cfg.bus_width = MMCSD_BUS_WIDTH_1;
    mmcsd_set_iocfg(host);
}

#ifdef ARCH_LOMBO
void mmcsd_init_erase(struct rt_mmcsd_card *card)
{
	rt_uint32_t sz;

	if (is_power_of_2(card->erase_size))
		card->erase_shift = ffs(card->erase_size) - 1;
	else
		card->erase_shift = 0;

	/*
	 * It is possible to erase an arbitrarily large area of an SD or MMC
	 * card.  That is not desirable because it can take a long time
	 * (minutes) potentially delaying more important I/O, and also the
	 * timeout calculations become increasingly hugely over-estimated.
	 * Consequently, 'pref_erase' is defined as a guide to limit erases
	 * to that size and alignment.
	 *
	 * For SD cards that define Allocation Unit size, limit erases to one
	 * Allocation Unit at a time.  For MMC cards that define High Capacity
	 * Erase Size, whether it is switched on or not, limit to that size.
	 * Otherwise just have a stab at a good value.  For modern cards it
	 * will end up being 4MiB.  Note that if the value is too small, it
	 * can end up taking longer to erase.
	 */
	if ((CARD_TYPE_SD == card->card_type) && card->ssr.au) {
		card->pref_erase = card->ssr.au;
		card->erase_shift = ffs(card->ssr.au) - 1;
	} else if (card->ext_csd.hc_erase_size) {
		card->pref_erase = card->ext_csd.hc_erase_size;
	} else {
		sz = card->card_capacity >> 10;		/* in MiB */
		if (sz < 128)
			card->pref_erase = 512 * 1024 / 512;
		else if (sz < 512)
			card->pref_erase = 1024 * 1024 / 512;
		else if (sz < 1024)
			card->pref_erase = 2 * 1024 * 1024 / 512;
		else
			card->pref_erase = 4 * 1024 * 1024 / 512;
		if (card->pref_erase < card->erase_size)
			card->pref_erase = card->erase_size;
		else {
			sz = card->pref_erase % card->erase_size;
			if (sz)
				card->pref_erase += card->erase_size - sz;
		}
	}
}

static rt_uint32_t mmcsd_mmc_erase_timeout(struct rt_mmcsd_card *card,
	rt_uint32_t arg, rt_uint32_t qty)
{
	rt_uint32_t erase_timeout;

	if (arg == MMC_DISCARD_ARG || (arg == MMC_TRIM_ARG && card->ext_csd.rev >= 6)) {
		erase_timeout = card->ext_csd.trim_timeout;
	} else if (card->ext_csd.erase_group_def & 1) {
		/* High Capacity Erase Group Size uses HC timeouts */
		if (arg == MMC_TRIM_ARG)
			erase_timeout = card->ext_csd.trim_timeout;
		else
			erase_timeout = card->ext_csd.hc_erase_timeout;
	} else {
		/* CSD Erase Group Size uses write timeout */
		rt_uint32_t mult = (10 << card->csd.r2w_factor);
		rt_uint32_t timeout_clks = card->csd.taac * mult;
		rt_uint32_t timeout_us;

		/* Avoid overflow: e.g. tacc_ns=80000000 mult=1280 */
		if (card->csd.nsac < 1000000)
			timeout_us = (card->csd.nsac * mult) / 1000;
		else
			timeout_us = (card->csd.nsac / 1000) * mult;

		/*
		 * ios.clock is only a target.  The real clock rate might be
		 * less but not that much less, so fudge it by multiplying by 2.
		 */
		timeout_clks <<= 1;
		timeout_us += (timeout_clks * 1000) / (card->host->io_cfg.clock / 1000);

		erase_timeout = timeout_us / 1000;

		/*
		 * Theoretically, the calculation could underflow so round up
		 * to 1ms in that case.
		 */
		if (!erase_timeout)
			erase_timeout = 1;
	}

	/* Multiplier for secure operations */
	if (arg & MMC_SECURE_ARGS) {
		if (arg == MMC_SECURE_ERASE_ARG)
			erase_timeout *= card->ext_csd.sec_erase_mult;
		else
			erase_timeout *= card->ext_csd.sec_trim_mult;
	}

	erase_timeout *= qty;

	/*
	 * Ensure at least a 1 second timeout for SPI as per
	 * 'mmc_set_data_timeout()'
	 */
	if (controller_is_spi(card->host) && erase_timeout < 1000)
		erase_timeout = 1000;

	return erase_timeout;
}

static rt_uint32_t mmcsd_sd_erase_timeout(struct rt_mmcsd_card *card,
	rt_uint32_t arg, rt_uint32_t qty)
{
	rt_uint32_t erase_timeout;

	if (card->ssr.erase_timeout) {
		/* Erase timeout specified in SD Status Register (SSR) */
		erase_timeout = card->ssr.erase_timeout * qty + card->ssr.erase_offset;
	} else {
		/*
		 * Erase timeout not specified in SD Status Register (SSR) so
		 * use 250ms per write block.
		 */
		erase_timeout = 250 * qty;
	}

	/* Must not be less than 1 second */
	if (erase_timeout < 1000)
		erase_timeout = 1000;

	return erase_timeout;
}

static rt_uint32_t mmcsd_erase_timeout(struct rt_mmcsd_card *card,
	rt_uint32_t arg, rt_uint32_t qty)
{
	if (CARD_TYPE_SD == card->card_type)
		return mmcsd_sd_erase_timeout(card, arg, qty);
	else
		return mmcsd_mmc_erase_timeout(card, arg, qty);
}

static int mmcsd_do_erase(struct rt_mmcsd_card *card, rt_uint32_t from,
	rt_uint32_t to, rt_uint32_t arg)
{
	struct rt_mmcsd_cmd cmd = {0};
	rt_uint32_t qty = 0;
	rt_uint64_t timeout;
	int err;

	/*
	 * qty is used to calculate the erase timeout which depends on how many
	 * erase groups (or allocation units in SD terminology) are affected.
	 * We count erasing part of an erase group as one erase group.
	 * For SD, the allocation units are always a power of 2.  For MMC, the
	 * erase group size is almost certainly also power of 2, but it does not
	 * seem to insist on that in the JEDEC standard, so we fall back to
	 * division in that case.  SD may not specify an allocation unit size,
	 * in which case the timeout is based on the number of write blocks.
	 *
	 * Note that the timeout for secure trim 2 will only be correct if the
	 * number of erase groups specified is the same as the total of all
	 * preceding secure trim 1 commands.  Since the power may have been
	 * lost since the secure trim 1 commands occurred, it is generally
	 * impossible to calculate the secure trim 2 timeout correctly.
	 */
	if (card->erase_shift)
		qty += ((to >> card->erase_shift) - (from >> card->erase_shift)) + 1;
	else if (CARD_TYPE_SD == card->card_type)
		qty += to - from + 1;
	else
		qty += ((to / card->erase_size) - (from / card->erase_size)) + 1;

	if (CARD_TYPE_SD == card->card_type)
		cmd.cmd_code = SD_ERASE_WR_BLK_START;
	else
		cmd.cmd_code = ERASE_GROUP_START;
	cmd.arg = from;
	cmd.flags = RESP_SPI_R1 | RESP_R1 | CMD_AC;
	err = mmcsd_send_cmd(card->host, &cmd, 0);
	if (err) {
		LOG_E("mmcsd_erase: group start error %d, status %#x", err, cmd.resp[0]);
		err = -RT_EIO;
		goto out;
	}

	memset(&cmd, 0, sizeof(struct rt_mmcsd_cmd));
	if (CARD_TYPE_SD == card->card_type)
		cmd.cmd_code = SD_ERASE_WR_BLK_END;
	else
		cmd.cmd_code = ERASE_GROUP_END;
	cmd.arg = to;
	cmd.flags = RESP_SPI_R1 | RESP_R1 | CMD_AC;
	err = mmcsd_send_cmd(card->host, &cmd, 0);
	if (err) {
		LOG_E("mmcsd_erase: group end error %d, status %#x", err, cmd.resp[0]);
		err = -RT_EIO;
		goto out;
	}

	memset(&cmd, 0, sizeof(struct rt_mmcsd_cmd));
	cmd.cmd_code = ERASE;
	cmd.arg = arg;
	cmd.flags = RESP_SPI_R1B | RESP_R1B | CMD_AC;
	cmd.cmd_timeout_ms = mmcsd_erase_timeout(card, arg, qty);
	err = mmcsd_send_cmd(card->host, &cmd, 0);
	if (err) {
		LOG_E("mmcsd_erase: erase error %d, status %#x", err, cmd.resp[0]);
		err = -RT_EIO;
		goto out;
	}

	if (controller_is_spi(card->host))
		goto out;

	/* 10 minute timeout */
	timeout = rt_time_get_usec() + (10 * 60 * 1000 * 1000UL);
	do {
		memset(&cmd, 0, sizeof(struct rt_mmcsd_cmd));
		cmd.cmd_code = SEND_STATUS;
		cmd.arg = card->rca << 16;
		cmd.flags = RESP_R1 | CMD_AC;
		/* Do not retry else we can't see errors */
		err = mmcsd_send_cmd(card->host, &cmd, 0);
		if (err || (cmd.resp[0] & 0xFDF92000)) {
			LOG_E("mmcsd_erase: error %d requesting status %#x",
				err, cmd.resp[0]);
			err = -RT_EIO;
			goto out;
		}

		/* Timeout if the device never becomes ready for data and
		 * never leaves the program state.
		 */
		if (rt_time_get_usec() > timeout) {
			LOG_E("mmcsd_erase: Card stuck in programming state!");
			err =  -RT_EIO;
			goto out;
		}
	} while (!(cmd.resp[0] & R1_READY_FOR_DATA) ||
		(R1_CURRENT_STATE(cmd.resp[0]) == 7));

out:
	return err;
}

static int __mmcsd_erase(struct rt_mmcsd_card *card, rt_uint32_t from,
			 rt_uint32_t nr, rt_uint32_t arg)
{
	rt_uint32_t rem, to = from + nr;

	if (!(card->host->flags & MMCSD_SUP_ERASE) ||
	    !(card->csd.card_cmd_class & CCC_ERASE))
		return -RT_ERROR;

	if (!card->erase_size)
		return -RT_ERROR;

	if ((CARD_TYPE_SD == card->card_type) && (arg != MMC_ERASE_ARG))
		return -RT_ERROR;

	if ((arg & MMC_SECURE_ARGS) &&
	    !(card->ext_csd.sec_feature_support & EXT_CSD_SEC_ER_EN))
		return -RT_ERROR;

	if ((arg & MMC_TRIM_ARGS) &&
	    !(card->ext_csd.sec_feature_support & EXT_CSD_SEC_GB_CL_EN))
		return -RT_ERROR;

	if (arg == MMC_SECURE_ERASE_ARG) {
		if (from % card->erase_size || nr % card->erase_size)
			return -RT_EINVAL;
	}

	if (arg == MMC_ERASE_ARG) {
		rem = from % card->erase_size;
		if (rem) {
			rem = card->erase_size - rem;
			from += rem;
			if (nr > rem)
				nr -= rem;
			else
				return 0;
		}
		rem = nr % card->erase_size;
		if (rem)
			nr -= rem;
	}

	if (nr == 0)
		return 0;

	to = from + nr;

	if (to <= from)
		return -RT_EINVAL;

	/* 'from' and 'to' are inclusive */
	to -= 1;

	return mmcsd_do_erase(card, from, to, arg);
}

int mmcsd_erase(struct rt_mmcsd_card *card, rt_uint32_t from, rt_uint32_t nr,
		rt_uint32_t arg)
{
	int ret = 0;
	rt_uint32_t to, tmp_from, tmp_nr, max_nr, remain_nr;

	max_nr = MIN(mmcsd_calc_max_discard(card), UINT_MAX >> 9);
	max_nr -= (max_nr % card->pref_erase);
	if (unlikely(!max_nr)) {
		LOG_W("max request sectors is 0");
		return -RT_ERROR;
	}

	tmp_from = from;
	to = from + nr - 1;
	while (1) {
		if (tmp_from > to)
			break;

		remain_nr = to - tmp_from + 1;
		tmp_nr = MIN(remain_nr, max_nr);
		if (tmp_nr < remain_nr)
			tmp_nr -= tmp_from % card->pref_erase;

		ret = __mmcsd_erase(card, tmp_from, tmp_nr, arg);
		if (ret) {
			LOG_E("mmcsd_erase: from %d nr %d failed", tmp_from, tmp_nr);
			break;
		}

		tmp_from += tmp_nr;
	}

	return ret;
}

int mmcsd_can_erase(struct rt_mmcsd_card *card)
{
	if ((card->host->flags & MMCSD_SUP_ERASE) &&
	    (card->csd.card_cmd_class & CCC_ERASE) && card->erase_size)
		return 1;
	return 0;
}

int mmcsd_can_trim(struct rt_mmcsd_card *card)
{
	if (card->ext_csd.sec_feature_support & EXT_CSD_SEC_GB_CL_EN)
		return 1;
	return 0;
}

int mmcsd_can_discard(struct rt_mmcsd_card *card)
{
#if 0
	/*
	 * As there's no way to detect the discard support bit at v4.5
	 * use the s/w feature support filed.
	 */
	if (card->ext_csd.feature_support & MMC_DISCARD_FEATURE)
		return 1;
#endif
	return 0;
}

int mmcsd_can_sanitize(struct rt_mmcsd_card *card)
{
	if (!mmcsd_can_trim(card) && !mmcsd_can_erase(card))
		return 0;
	if (card->ext_csd.sec_feature_support & EXT_CSD_SEC_SANITIZE)
		return 1;
	return 0;
}

int mmcsd_can_secure_erase_trim(struct rt_mmcsd_card *card)
{
	if (card->ext_csd.sec_feature_support & EXT_CSD_SEC_ER_EN)
		return 1;
	return 0;
}

int mmsdc_erase_group_aligned(struct rt_mmcsd_card *card, rt_uint32_t from,
			      rt_uint32_t nr)
{
	if (!card->erase_size)
		return 0;
	if (from % card->erase_size || nr % card->erase_size)
		return 0;
	return 1;
}

static rt_uint32_t mmcsd_do_calc_max_discard(struct rt_mmcsd_card *card, rt_uint32_t arg)
{
	struct rt_mmcsd_host *host = card->host;
	rt_uint32_t max_discard, x, y, qty = 0, max_qty, timeout;
	rt_uint32_t last_timeout = 0;

	if (card->erase_shift)
		max_qty = UINT_MAX >> card->erase_shift;
	else if (CARD_TYPE_SD == card->card_type)
		max_qty = UINT_MAX;
	else
		max_qty = UINT_MAX / card->erase_size;

	/* Find the largest qty with an OK timeout */
	do {
		y = 0;
		for (x = 1; x && x <= max_qty && max_qty - x >= qty; x <<= 1) {
			timeout = mmcsd_erase_timeout(card, arg, qty + x);
			if (timeout > host->max_discard_to)
				break;
			if (timeout < last_timeout)
				break;
			last_timeout = timeout;
			y = x;
		}
		qty += y;
	} while (y);

	if (!qty)
		return 0;

	if (qty == 1)
		return 1;

	/* Convert qty to sectors */
	if (card->erase_shift)
		max_discard = --qty << card->erase_shift;
	else if (CARD_TYPE_SD == card->card_type)
		max_discard = qty;
	else
		max_discard = --qty * card->erase_size;

	return max_discard;
}

rt_uint32_t mmcsd_calc_max_discard(struct rt_mmcsd_card *card)
{
	struct rt_mmcsd_host *host = card->host;
	rt_uint32_t max_discard, max_trim;

	if (!host->max_discard_to)
		return UINT_MAX;

	/*
	 * Without erase_group_def set, MMC erase timeout depends on clock
	 * frequence which can change.  In that case, the best choice is
	 * just the preferred erase size.
	 */
	if ((CARD_TYPE_MMC == card->card_type) && !(card->ext_csd.erase_group_def & 1))
		return card->pref_erase;

	max_discard = mmcsd_do_calc_max_discard(card, MMC_ERASE_ARG);
	if (mmcsd_can_trim(card)) {
		max_trim = mmcsd_do_calc_max_discard(card, MMC_TRIM_ARG);
		if (max_trim < max_discard)
			max_discard = max_trim;
	} else if (max_discard < card->erase_size) {
		max_discard = 0;
	}

	LOG_D("calculated max. discard sectors %u for timeout %u ms",
		max_discard, host->max_discard_to);

	return max_discard;
}
#endif

int mmcsd_wait_cd_changed(rt_int32_t timeout)
{
    struct rt_mmcsd_host *host;
    if (rt_mb_recv(&mmcsd_hotpluge_mb, (rt_ubase_t *)&host, timeout) == RT_EOK)
    {
        if(host->card == RT_NULL)
        {
            return MMCSD_HOST_UNPLUGED;
        }
        else
        {
            return MMCSD_HOST_PLUGED;
        }
    }
    return -RT_ETIMEOUT;
}
RTM_EXPORT(mmcsd_wait_cd_changed);

#ifndef ARCH_LOMBO
void mmcsd_change(struct rt_mmcsd_host *host)
{
    rt_mb_send(&mmcsd_detect_mb, (rt_ubase_t)host);
}

void mmcsd_detect(void *param)
{
    struct rt_mmcsd_host *host;
    rt_uint32_t  ocr;
    rt_int32_t  err;

    while (1) 
    {
        if (rt_mb_recv(&mmcsd_detect_mb, (rt_ubase_t *)&host, RT_WAITING_FOREVER) == RT_EOK)
        {
            if (host->card == RT_NULL)
            {
                mmcsd_host_lock(host);
                mmcsd_power_up(host);
                mmcsd_go_idle(host);

                mmcsd_send_if_cond(host, host->valid_ocr);

                err = sdio_io_send_op_cond(host, 0, &ocr);
                if (!err)
                {
                    if (init_sdio(host, ocr))
                        mmcsd_power_off(host);
                    mmcsd_host_unlock(host);
                    continue;
                }

                /*
                 * detect SD card
                 */
                err = mmcsd_send_app_op_cond(host, 0, &ocr);
                if (!err) 
                {
                    if (init_sd(host, ocr))
                        mmcsd_power_off(host);
                    mmcsd_host_unlock(host);
                    rt_mb_send(&mmcsd_hotpluge_mb, (rt_uint32_t)host);
                    continue;
                }
                
                /*
                 * detect mmc card
                 */
                err = mmc_send_op_cond(host, 0, &ocr);
                if (!err) 
                {
                    if (init_mmc(host, ocr))
                        mmcsd_power_off(host);
                    mmcsd_host_unlock(host);
                    rt_mb_send(&mmcsd_hotpluge_mb, (rt_uint32_t)host);
                    continue;
                }
                mmcsd_host_unlock(host);
            }
            else
            {
            	/* card removed */
            	mmcsd_host_lock(host);
            	if (host->card->sdio_function_num != 0)
            	{
            		LOG_W("unsupport sdio card plug out!");
            	}
            	else
            	{
            		rt_mmcsd_blk_remove(host->card);
            		rt_free(host->card);

            		host->card = RT_NULL;
            	}
            	mmcsd_host_unlock(host);
            	rt_mb_send(&mmcsd_hotpluge_mb, (rt_uint32_t)host);
            }
        }
    }
}

struct rt_mmcsd_host *mmcsd_alloc_host(void)
{
    struct rt_mmcsd_host *host;

    host = rt_malloc(sizeof(struct rt_mmcsd_host));
    if (!host) 
    {
        LOG_E("alloc host failed");

        return RT_NULL;
    }

    rt_memset(host, 0, sizeof(struct rt_mmcsd_host));

    host->max_seg_size = 65535;
    host->max_dma_segs = 1;
    host->max_blk_size = 512;
    host->max_blk_count = 4096;

    rt_mutex_init(&host->bus_lock, "sd_bus_lock", RT_IPC_FLAG_FIFO);
    rt_sem_init(&host->sem_ack, "sd_ack", 0, RT_IPC_FLAG_FIFO);

    return host;
}

void mmcsd_free_host(struct rt_mmcsd_host *host)
{
    rt_mutex_detach(&host->bus_lock);
    rt_sem_detach(&host->sem_ack);
    rt_free(host);
}
#else
int mmcsd_wait_partition_mount(rt_int32_t timeout)
{
	rt_uint32_t event;

	if (!rt_event_recv(&mmcsd_mount_event,
		MMCSD_PART_MOUNT_OK | MMCSD_PART_MOUNT_FAIL | MMCSD_PART_UMOUNT,
		RT_EVENT_FLAG_OR, timeout, &event)) {
		rt_event_control(&mmcsd_mount_event, RT_IPC_CMD_RESET, RT_NULL);

		if ((MMCSD_PART_MOUNT_OK == event) || (MMCSD_PART_MOUNT_FAIL == event) ||
			(MMCSD_PART_UMOUNT == event))
			return event;

		return -RT_ERROR;
	}

	return -RT_ETIMEOUT;
}

void mmcsd_send_partition_mount_event(rt_uint32_t event)
{
	rt_event_send(&mmcsd_mount_event, event);
}

void mmcsd_change(struct rt_mmcsd_host *host, rt_uint32_t status)
{
	rt_uint32_t event = status ? MMCSD_CARD_PLUG_IN : MMCSD_CARD_PLUG_OUT;
	int msg_type = status ? LB_SYSMSG_SD_PLUGIN : LB_SYSMSG_SD_PLUGOUT;

	LOG_I("mmcsd host-%u: card plug %s", host->host_index, status ? "in" : "out");

	host->mmcsd_plug_status = status;
	host->mmcsd_plug_count++;
	rt_event_send(host->mmcsd_plug_event, event);
	lb_system_mq_send(msg_type, &(host->host_index), sizeof(host->host_index),
		ASYNC_FLAG);
}

#if CARD_REINIT_WHEN_CHARGE
void mmcsd_change_virt(struct rt_mmcsd_host *host)
{
	LOG_W("mmcsd host-%u: card plug in(virtual)", host->host_index);
	host->mmcsd_plug_count++;
	rt_event_send(host->mmcsd_plug_event, MMCSD_CARD_PLUG_IN);
}
#endif

static void mmcsd_detect(void *param)
{
	int msg_type;
	rt_uint32_t err, ocr, plug_event, plug_count = 0;
	struct rt_mmcsd_host *host = (struct rt_mmcsd_host *)param;

	while (1) {
		rt_event_recv(host->mmcsd_plug_event,
			MMCSD_CARD_PLUG_IN | MMCSD_CARD_PLUG_OUT,
			RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
			RT_WAITING_FOREVER, &plug_event);

		LOG_I("mmcsd host-%u: card plug event(%u) received",
			host->host_index, plug_event);

card_init:
		if (host->mmcsd_plug_status) {
			if (host->card) {
				LOG_I("mmcsd host-%u: last card hasn't been removed",
					host->host_index);
				if (plug_count != host->mmcsd_plug_count) {
					LOG_I("mmcsd host-%u: card had once been removed",
						host->host_index);
					goto card_remove;
				}
				continue;
			}

			LOG_I("mmcsd host-%u: card init start", host->host_index);
			plug_count = host->mmcsd_plug_count;

			mmcsd_host_lock(host);
			mmcsd_power_up(host);
			mmcsd_go_idle(host);

			mmcsd_send_if_cond(host, host->valid_ocr);

			if (!sdio_io_send_op_cond(host, 0, &ocr)) {
				err = init_sdio(host, ocr);
				if (err)
					mmcsd_power_off(host);
				mmcsd_host_unlock(host);

				LOG_I("mmcsd host-%u: card init %s", host->host_index,
					err ? "failed" : "ok");
			} else if (!mmcsd_send_app_op_cond(host, 0, &ocr)) {
				err = init_sd(host, ocr);
				if (err)
					mmcsd_power_off(host);
				mmcsd_host_unlock(host);

				LOG_I("mmcsd host-%u: card  init %s", host->host_index,
					err ? "failed" : "ok");
				if(!err){  //排除坏卡的消息发送导不断重启
                 msg_type = err ? LB_SYSMSG_SD_INIT_FAIL :
					LB_SYSMSG_SD_INIT_OK;
				lb_system_mq_send(msg_type, &(host->host_index),
					sizeof(host->host_index), ASYNC_FLAG);
				rt_mb_send(&mmcsd_hotpluge_mb, (rt_uint32_t)host);
			 }	
						
			} else if (!mmc_send_op_cond(host, 0, &ocr)) {
				err = init_mmc(host, ocr);
				if (err)
					mmcsd_power_off(host);
				mmcsd_host_unlock(host);

				LOG_I("mmcsd host-%u: card init %s", host->host_index,
					err ? "failed" : "ok");
				msg_type = err ? LB_SYSMSG_SD_INIT_FAIL :
					LB_SYSMSG_SD_INIT_OK;
				lb_system_mq_send(msg_type, &(host->host_index),
					sizeof(host->host_index), ASYNC_FLAG);
				rt_mb_send(&mmcsd_hotpluge_mb, (rt_uint32_t)host);
			} else {
				mmcsd_host_unlock(host);

				LOG_I("mmcsd host-%u: card init failed",
					host->host_index);
			}

			if (host->card) {
				if (!host->mmcsd_plug_status) {
					LOG_I("mmcsd host-%u: card has been removed",
						host->host_index);
					goto card_remove;
				}
				if (plug_count != host->mmcsd_plug_count) {
					LOG_I("mmcsd host-%u: card had once been removed",
						host->host_index);
					goto card_remove;
				}
			}

			LOG_I("mmcsd host-%u: card init end", host->host_index);
			continue;
		}

card_remove:
		if (!host->card) {
			LOG_I("mmcsd host-%u: there is not a card", host->host_index);
			continue;
		}

		if (host->card->sdio_function_num) {
			LOG_W("mmcsd host-%u: unsupport sdio card plug out",
				host->host_index);
			continue;
		}

		g_sd_plugout = 1;
		LOG_I("mmcsd host-%u: card remove preparing", host->host_index);
		rt_event_control(host->mmcsd_plug_event, RT_IPC_CMD_RESET, RT_NULL);
		lb_system_mq_send(LB_SYSMSG_FS_PART_UNMOUNT_PREPARE,
			&(host->mmcsd_plug_event), sizeof(rt_event_t), ASYNC_FLAG);

		rt_event_recv(host->mmcsd_plug_event, MMCSD_READY_TO_UMOUNT,
			RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
			RT_WAITING_FOREVER, &plug_event);

		LOG_I("mmcsd host-%u: card remove start", host->host_index);

		mmcsd_host_lock(host);
		rt_mmcsd_blk_remove(host->card);
		rt_free(host->card);
		host->card = RT_NULL;
		mmcsd_host_unlock(host);
		rt_mb_send(&mmcsd_hotpluge_mb, (rt_uint32_t)host);

		LOG_I("mmcsd host-%u: card remove ok", host->host_index);

		/* sd card's speed class resumes to initialed value */
		if (0 == host->host_index)
			g_sd_speedclass = -2;

		rt_event_control(host->mmcsd_plug_event, RT_IPC_CMD_RESET, RT_NULL);
		if (host->mmcsd_plug_status) {
			LOG_I("mmcsd host-%u: new card inserted", host->host_index);
			goto card_init;
		}
	}
}

struct rt_mmcsd_host *mmcsd_alloc_host(rt_uint32_t index)
{
	struct rt_mmcsd_host *host;
	char name[16] = {0};

	host = rt_malloc(sizeof(struct rt_mmcsd_host));
	if (!host) {
		LOG_E("mmcsd host-%u: failed to malloc host", index);
		return RT_NULL;
	}

	rt_memset(host, 0, sizeof(struct rt_mmcsd_host));

	host->max_seg_size = 65535;
	host->max_dma_segs = 1;
	host->max_blk_size = 512;
	host->max_blk_count = 4096;

	rt_mutex_init(&host->bus_lock, "sd_bus_lock", RT_IPC_FLAG_FIFO);
	rt_sem_init(&host->sem_ack, "sd_ack", 0, RT_IPC_FLAG_FIFO);

	host->host_index = index;

	rt_memset(name, 0, sizeof(name));
	rt_sprintf(name, "mmcsd%u_plug", index);
	host->mmcsd_plug_event = rt_event_create(name, RT_IPC_FLAG_FIFO);
	RT_ASSERT(host->mmcsd_plug_event);

	rt_memset(name, 0, sizeof(name));
	rt_sprintf(name, "mmcsd%u_detect", index);
	host->mmcsd_detect_thread = rt_thread_create(name, mmcsd_detect,
		host, RT_MMCSD_STACK_SIZE, RT_MMCSD_THREAD_PREORITY, 20);
	RT_ASSERT(host->mmcsd_detect_thread);

	rt_thread_startup(host->mmcsd_detect_thread);

	return host;
}

void mmcsd_free_host(struct rt_mmcsd_host *host)
{
	rt_thread_delete(host->mmcsd_detect_thread);
	rt_event_delete(host->mmcsd_plug_event);

	rt_mutex_detach(&host->bus_lock);
	rt_sem_detach(&host->sem_ack);
	rt_free(host);
}
#endif

int rt_mmcsd_core_init(void)
{
#ifdef ARCH_LOMBO
	rt_err_t ret;

	ret = rt_event_init(&mmcsd_mount_event, "mmcsd_mount", RT_IPC_FLAG_FIFO);
	RT_ASSERT(RT_EOK == ret);

	ret = rt_mb_init(&mmcsd_hotpluge_mb, "mmcsdhotplugmb",
		&mmcsd_hotpluge_mb_pool[0],
		sizeof(mmcsd_hotpluge_mb_pool) / sizeof(mmcsd_hotpluge_mb_pool[0]),
		RT_IPC_FLAG_FIFO);
	RT_ASSERT(RT_EOK == ret);
#else
    rt_err_t ret;

    /* initialize detect SD cart thread */
    /* initialize mailbox and create detect SD card thread */
    ret = rt_mb_init(&mmcsd_detect_mb, "mmcsdmb",
        &mmcsd_detect_mb_pool[0], sizeof(mmcsd_detect_mb_pool) / sizeof(mmcsd_detect_mb_pool[0]),
        RT_IPC_FLAG_FIFO);
    RT_ASSERT(ret == RT_EOK);

   ret = rt_mb_init(&mmcsd_hotpluge_mb, "mmcsdhotplugmb",
        &mmcsd_hotpluge_mb_pool[0], sizeof(mmcsd_hotpluge_mb_pool) / sizeof(mmcsd_hotpluge_mb_pool[0]),
        RT_IPC_FLAG_FIFO);
    RT_ASSERT(ret == RT_EOK);
     ret = rt_thread_init(&mmcsd_detect_thread, "mmcsd_detect", mmcsd_detect, RT_NULL, 
                 &mmcsd_stack[0], RT_MMCSD_STACK_SIZE, RT_MMCSD_THREAD_PREORITY, 20);
	 printf("\nret==============%d\n",ret);
    if (ret == RT_EOK) 
    {
        rt_thread_startup(&mmcsd_detect_thread);
    }
#endif

    rt_sdio_init();

	return 0;
}
INIT_PREV_EXPORT(rt_mmcsd_core_init);
