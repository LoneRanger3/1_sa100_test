/*
 * i2c_drv.c - I2C host driver code for LomboTech
 * i2c driver driver code implement
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
#ifndef ___I2C___DRV__C___
#define ___I2C___DRV__C___

#include <rtthread.h>
#include <rtdevice.h>
#include <rthw.h>
#include <irq_numbers.h>
#include <spinlock.h>

#include <csp.h>

#define DBG_SECTION_NAME	"I2C"
#define DBG_LEVEL		DBG_INFO
#include <debug.h>

#include "board.h"
#include "cfg/config_api.h"
#include "clk/clk.h"

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

#ifdef msg
#undef msg
#endif

#if (defined ARCH_LOMBO_N7)
#define I2C_MODULE_CNT                  4
#include "csp-n7/i2c_const.h"
#include "csp-n7/i2c_csp.h"
#else
#error "No supported platform!!!"
#endif

#include "i2c_init.h"
#include "i2c_drv.h"

#include "i2c_init.c"

#define lombo_i2c_chk_abrt(abrtsrc, mask)	\
	((abrtsrc & mask) == mask)

/**
 * lombo_i2c_wait_idle -  wait for the i2c bus to become idle.
 * @i2c: point to i2c host structure
 */
static void lombo_i2c_wait_idle(struct lombo_i2c *i2c);

static int lombo_i2c_batch_cmds(struct lombo_i2c *i2c, u32 entries);

/* dumpd SDC registers for debug */
static void lombo_i2c_dumpregs(struct lombo_i2c *i2c)
{
	u32 offset = 0;

	LOG_D(" =========== REGISTER DUMP ===========");

	for (offset = 0; offset < 0xC; offset += 4) {
		LOG_D(" 0x%x: 0x%08x",
			offset, READREG32((u32)i2c->base + offset));
	}

	for (offset = 0x10; offset < 0x64; offset += 8) {
		LOG_D(" 0x%x: 0x%08x | 0x%x: 0x%08x",
			offset, READREG32((u32)i2c->base + offset),
			offset + 4, READREG32((u32)i2c->base + offset + 4));
	}

	LOG_D(" ===========================================");
}

static void lombo_i2c_show_msg(struct lombo_i2c *i2c, u32 msg_seq)
{
	int i = 0;
	int remain = msg_seq;

	for (i = 0; i < i2c->msg_num; i++) {
		if (remain < i2c->msgs[i].len) {
			LOG_D("0x%08x: msg_seq=%d,msgs[%d].buf[%d] %s 0x%x",
				(u32)i2c->base, msg_seq, i, remain,
				(i2c->msgs[i].flags & RT_I2C_RD) ?
				"READ" : "WRITE",
				i2c->msgs[i].buf[remain]);
			return;
		}
		remain -= i2c->msgs[i].len;
	}

	LOG_E("invalid msg_seq (%d)", msg_seq);

}

/**
 * lombo_i2c_start_msgs - start transfer the messages
 * @i2c: point to i2c host structure
 * @msgs: start address of the messages to transfer
 * @num: number of the messages to transfer
 *
 * return 0 means ok; others means error
 */
static int lombo_i2c_start_msgs(struct lombo_i2c *i2c,
				struct rt_i2c_msg *msgs, u32 num)
{
	unsigned long flags;
	int ret = RT_EOK;

	PRT_TRACE_BEGIN("i2c_idx=%d,msgs=0x%x,num=%d", i2c->idx, msgs, num);

	if ((i2c == NULL) || (msgs == NULL) || (num == 0)) {
		LOG_E("i2c=0x%p,msgs=0x%p,num=%u", i2c, msgs, num);
		return -RT_EINVAL;
	}

#ifdef DEBUG
	LOG_D("0x%08x", (u32)i2c->base);
	{
		int i, j;
		for (i = 0; i < num; i++) {
			LOG_D("0x%08x: msgs[%d].len=%d",
				(u32)i2c->base, i, msgs[i].len);

			for (j = 0; j < msgs[i].len; j++)
				LOG_D("0x%08x: msgs[%d].buf[%d]=0x%02x",
					(u32)i2c->base, i, j, msgs[i].buf[j]);
		}
	}
#endif

	spin_lock_irqsave(&i2c->lock, flags);

	i2c->msgs = msgs;
	i2c->msg_num = num;
	i2c->msg_send = 0;
	i2c->msg_read = 0;
	i2c->msg_byte_send = 0;
	i2c->msg_byte_read = 0;
	i2c->msg_pd_rd_cnt = 0;

	i2c->msg_err = 0;
	i2c->int_status = 0;
	spin_unlock_irqrestore(&i2c->lock, flags);

	lombo_i2c_wait_idle(i2c);

	spin_lock_irqsave(&i2c->lock, flags);

	/*
	 * clear all int
	 */
	csp_i2c_clr_tx_abrt((u32)i2c->base, I2C_TX_ABTSRC_ALL_MASK);
	csp_i2c_clr_pd((u32)i2c->base, I2C_INT_ALL_MASK);

	/*
	 * transfer configure
	 */
	/* set tar address */
	csp_i2c_set_tar_addr((u32)i2c->base, msgs[0].addr,
				!!(msgs[0].flags & RT_I2C_ADDR_10BIT));
	/* set TX/RX FIFO threshold */
	csp_i2c_set_fifo_thresh((u32)i2c->base, I2C_TX_FIFO_DEPTH / 2,
				I2C_RX_FIFO_DEPTH / 2);
	/* enable_i2c */
	csp_i2c_enable((u32)i2c->base, 1);

	i2c->imask = I2C_INT_ALL_ERR | I2C_INT_TX_EMPTY |
			I2C_INT_RX_FULL | I2C_INT_STOP_DET;

	/*
	 * we push cmds as much as possible to TX_FIFO before
	 * enable any interrupts.
	 */
	ret = lombo_i2c_batch_cmds(i2c, I2C_TX_FIFO_DEPTH);
	if (ret != 0) {
		LOG_E("0x%08x", (u32)i2c->base);
		goto out;
	}

	csp_i2c_clr_pd((u32)i2c->base, I2C_INT_TX_EMPTY | I2C_INT_RX_FULL);
	/* enable TX_EMPTY, RX_FULL, STOP_DET interrupts */
	csp_i2c_enable_ints((u32)i2c->base, i2c->imask);

out:
	spin_unlock_irqrestore(&i2c->lock, flags);
	PRT_TRACE_END("");
	return ret;
}

/**
 * lombo_i2c_send_cmds - send commands to i2c host TX_FIFO with/without STOP
 * @i2c: point to i2c host structure
 * @cmd: read/write command (I2C_CMD_READ/I2C_CMD_WRITE)
 * @bytes: start address of the bytes flow
 * @count: count of the bytes to handle
 * @stop: 0,withou STOP; 1, send STOP when the last byte has been transfered
 */
static void lombo_i2c_send_cmds(struct lombo_i2c *i2c, u32 cmd, u8 *bytes,
				u32 count, u32 stop)
{
	u32 idx = 0;

	PRT_TRACE_BEGIN("i2c=0x%08x,cmd=%d,bytes=0x%08x,count=%d,stop=%d",
			i2c, cmd, bytes, count, stop);

	for (idx = 0; idx < count - 1; idx++) {
		csp_i2c_send_cmd((u32)i2c->base, cmd, bytes[idx],
			I2C_NO_STOP, I2C_NO_RESTART);
	}

	csp_i2c_send_cmd((u32)i2c->base, cmd, bytes[count - 1],
			stop, I2C_NO_RESTART);
	PRT_TRACE_END("");
}

/**
 * lombo_i2c_batch_cmds - send batch commands to TX_FIFO, as much as possible
 * @i2c: point to i2c host structure
 * @entries: available entries of the TX_FIFO
 *
 * return 0 means ok; others means error
 */
static int lombo_i2c_batch_cmds(struct lombo_i2c *i2c, u32 entries)
{
	struct rt_i2c_msg *msg = RT_NULL;
	u32 msg_send = 0;
	u32 msg_byte_send = 0;
	u32 stop = I2C_NO_STOP;
	u32 batch_cnt = 0;
	u32 done = 0;
	u32 cmd = 0;
	int ret = 0;

	if (entries > I2C_TX_FIFO_DEPTH) {
		LOG_E("0x%08x: etries=%d", (u32)i2c->base, entries);
		return -RT_EINVAL;
	}

	PRT_TRACE_BEGIN("i2c=0x%08x,entries=%d", i2c, entries);
	LOG_D("0x%08x: etries=%d", (u32)i2c->base, entries);

	if (i2c->msg_send == i2c->msg_num) {
		LOG_E("0x%08x: all messages have been sent", (u32)i2c->base);
		LOG_E("0x%08x: msg_send=%d,msg_num=%d",
			(u32)i2c->base, i2c->msg_send, i2c->msg_num);
		i2c->imask &= ~I2C_INT_TX_EMPTY;
		csp_i2c_disable_ints((u32)i2c->base, I2C_INT_TX_EMPTY);
		ret = -RT_EIO;
		goto out;
	}

	while (done < entries) {
		msg_send = i2c->msg_send;
		msg_byte_send = i2c->msg_byte_send;

		msg = &i2c->msgs[msg_send];

		batch_cnt = msg->len - msg_byte_send;
		if (batch_cnt > entries - done)
			batch_cnt = entries - done;

		LOG_D("0x%08x: batch_cnt=%d", (u32)i2c->base, batch_cnt);
		if (batch_cnt == 0)
			break;

		/*
		 * It is the last byte of the transfer,
		 * so after this no command need to be sent,
		 * mark STOP and disable TX_EMPTY.
		 */
		if ((msg_send == i2c->msg_num - 1) &&
			(msg_byte_send + batch_cnt == msg->len)) {
			stop = I2C_SEND_STOP;
			i2c->imask &= ~I2C_INT_TX_EMPTY;
			csp_i2c_disable_ints((u32)i2c->base, I2C_INT_TX_EMPTY);
		}

		if (msg->flags & RT_I2C_RD) {
			cmd = I2C_CMD_READ;
			i2c->msg_pd_rd_cnt += batch_cnt;
			LOG_D("msg_pd_rd_cnt=%d", i2c->msg_pd_rd_cnt);
		} else
			cmd = I2C_CMD_WRITE;

		lombo_i2c_send_cmds(i2c, cmd, &msg->buf[msg_byte_send],
				batch_cnt, stop);

		i2c->msg_byte_send += batch_cnt;
		/* current message is all sent, walk to next */
		if (i2c->msg_byte_send == msg->len) {
			i2c->msg_send++;
			i2c->msg_byte_send = 0;
		}
		done += batch_cnt;

		if (stop == I2C_SEND_STOP) {
			if (i2c->msg_send != i2c->msg_num) {
				LOG_E("0x%08x: stop not at transfer end",
					(u32)i2c->base);
				ret = -RT_EIO;
			}
			break;
		}
	}

	if (done > entries) {
		ret = -RT_EINVAL;
		LOG_E("0x%08x: etries=%d", (u32)i2c->base, entries);
	}

out:
	PRT_TRACE_END("");
	return ret;
}

/**
 * lombo_i2c_batch_read - read bytes from TX_FIFO, as much as possible
 * @i2c: point to i2c host structure
 * @entries: available entries of the RX_FIFO
 *
 * return 0 means ok; others means error
 */
static int lombo_i2c_batch_read(struct lombo_i2c *i2c, u32 entries)
{
	struct rt_i2c_msg *msg = RT_NULL;
	u32 msg_read = 0;
	u32 msg_byte_read = 0;
	u32 batch_cnt = 0;
	u32 done = 0;
	u32 idx = 0;
	u8 *buf = RT_NULL;
	int ret = 0;

	if ((entries > i2c->msg_pd_rd_cnt) ||
		(entries > I2C_RX_FIFO_DEPTH)) {
		LOG_E("0x%08x: etries=%d,msg_pd_rd_cnt=%d",
			(u32)i2c->base, entries, i2c->msg_pd_rd_cnt);
		return -RT_EINVAL;
	}

	PRT_TRACE_BEGIN("i2c=0x%08x,entries=%d", i2c, entries);

	LOG_D("0x%08x: etries=%d,msg_pd_rd_cnt=%d",
		(u32)i2c->base, entries, i2c->msg_pd_rd_cnt);

	if (i2c->msg_read == i2c->msg_num) {
		LOG_E("0x%08x: all messages have been read", (u32)i2c->base);
		LOG_E("0x%08x: msg_send=%d,msg_num=%d",
			(u32)i2c->base, i2c->msg_send, i2c->msg_num);
		i2c->imask &= ~I2C_INT_RX_FULL;
		csp_i2c_disable_ints((u32)i2c->base, I2C_INT_RX_FULL);
		ret = -RT_EIO;
		goto out;
	}

	while (done < entries) {
		msg_read = i2c->msg_read;
		msg_byte_read = i2c->msg_byte_read;

		msg = &i2c->msgs[msg_read];

		/* skip the write message */
		if ((msg->flags & RT_I2C_RD) == 0) {
			if (msg_byte_read != 0) {
				LOG_E("read bytes in write message!");
				ret = -EINVAL;
				goto out;
			}

			i2c->msg_read++;
			continue;
		}

		batch_cnt = msg->len - msg_byte_read;
		if (batch_cnt > entries - done)
			batch_cnt = entries - done;

		buf = &msg->buf[msg_byte_read];
		for (idx = 0; idx < batch_cnt; idx++)
			buf[idx] = csp_i2c_read_byte((u32)i2c->base);

		i2c->msg_byte_read += batch_cnt;
		/* current message is completed, walk to next */
		if (i2c->msg_byte_read == msg->len) {
			i2c->msg_byte_read = 0;
			i2c->msg_read++;
		}
		done += batch_cnt;

		/* all data have been read  */
		if (i2c->msg_read == i2c->msg_num)
			break;
	}

	if (done != entries)
		ret = -RT_EIO;

	/* update the pending read count */
	i2c->msg_pd_rd_cnt -= done;

out:
	if (ret != 0)
		LOG_E("0x%08x: etries=%d,done=%d",
			(u32)i2c->base, entries, done);

	PRT_TRACE_END("");
	return ret;
}

/**
 * lombo_i2c_master_complete
 * @i2c: point to i2c host structure
 * @ret: return code, 0 means ok; others means error
 *
 * complete the message and wake up the caller, using the given return code,
 * or zero to mean ok.
*/
static inline void lombo_i2c_master_complete(struct lombo_i2c *i2c, int ret)
{
	PRT_TRACE_BEGIN("i2c=0x%08x,ret=%d", i2c, ret);
	LOG_D("0x%08x: master_complete %d", (u32)i2c->base, ret);

	i2c->msgs = RT_NULL;
	i2c->msg_num = 0;
	i2c->msg_err = ret;

	rt_completion_done(&i2c->completion);
	PRT_TRACE_END("");
}

/**
 * lombo_i2c_stop - stop i2c transfer, using the given return code
 * @i2c: point to i2c host structure
 * @ret: return code, 0 means ok; others means error
*/
static inline void lombo_i2c_stop(struct lombo_i2c *i2c, int ret)
{
	PRT_TRACE_BEGIN("i2c=0x%08x,ret=%d", i2c, ret);
	LOG_D("0x%08x: STOP", (u32)i2c->base);

	if (ret != 0) {
		i2c->abrtsrc = csp_i2c_get_abrt((u32)i2c->base);

		LOG_E("0x%08x: baudrate=%d,ic_clkrate=%d",
			(u32)i2c->base, i2c->baudrate, i2c->ic_clkrate);

		if (i2c->msgs != RT_NULL) {
			u32 rx_flush_cnt = 0;

			LOG_E("0x%08x: device address 0x%02x",
				(u32)i2c->base, i2c->msgs[0].addr);

			rx_flush_cnt =
				(i2c->abrtsrc & I2C_TX_FLUSH_CNT_MASK) >>
				I2C_TX_FLUSH_CNT_SHIFT;
			if (csp_i2c_chk_pd((u32)i2c->base, I2C_INT_TX_ABRT)) {
				LOG_E("0x%08x: rx_flush_cnt %d",
					(u32)i2c->base, rx_flush_cnt);
				lombo_i2c_show_msg(i2c, rx_flush_cnt);
			}
		}

		if (lombo_i2c_chk_abrt(i2c->abrtsrc, I2C_ABT_MST_ARB_LOST)) {
			/* deal with arbitration loss */
			LOG_E("0x%08x: deal with arbitration loss",
				(u32)i2c->base);
		}

		if (lombo_i2c_chk_abrt(i2c->abrtsrc, I2C_ABT_MST_DIS))
			LOG_E("0x%08x: master disabled", (u32)i2c->base);

		if (lombo_i2c_chk_abrt(i2c->abrtsrc, I2C_ABT_10B_RD_NORSTRT))
			LOG_E("0x%08x: restart is disbled with 10bit address",
				(u32)i2c->base);

		if (lombo_i2c_chk_abrt(i2c->abrtsrc, I2C_ABT_SB_NORSTRT))
			LOG_E("0x%08x: send START when restart is disbled",
				(u32)i2c->base);

		if (lombo_i2c_chk_abrt(i2c->abrtsrc, I2C_ABT_HS_NORSTRT))
			LOG_E("0x%08x: in HS mode when restart is disbled",
				(u32)i2c->base);

		if (lombo_i2c_chk_abrt(i2c->abrtsrc, I2C_ABT_SB_ACKDET))
			LOG_E("0x%08x: START byte is acknowledged",
				(u32)i2c->base);

		if (lombo_i2c_chk_abrt(i2c->abrtsrc, I2C_ABT_HS_ACKDET))
			LOG_E("0x%08x: HS master code is acknowledged",
				(u32)i2c->base);

		if (lombo_i2c_chk_abrt(i2c->abrtsrc, I2C_ABT_GCALL_READ))
			LOG_E("0x%08x: treated General Call as read",
				(u32)i2c->base);

		if (lombo_i2c_chk_abrt(i2c->abrtsrc, I2C_ABT_GCALL_NAK))
			LOG_E("0x%08x: sent General Call but no ask",
				(u32)i2c->base);

		if (lombo_i2c_chk_abrt(i2c->abrtsrc, I2C_ABT_TXDATA_NAK))
			LOG_E("0x%08x: no ask for the sent data",
				(u32)i2c->base);

		if (lombo_i2c_chk_abrt(i2c->abrtsrc, I2C_ABT_10ADDR2_NAK))
			LOG_E("0x%08x: no ask for the 2nd address byte",
				(u32)i2c->base);

		if (lombo_i2c_chk_abrt(i2c->abrtsrc, I2C_ABT_10ADDR1_NAK))
			LOG_E("0x%08x: no ask for the 1nd byte of 10bit addr",
				(u32)i2c->base);

		if (lombo_i2c_chk_abrt(i2c->abrtsrc, I2C_ABT_7B_ADDR_NAK))
			LOG_E("0x%08x: no ask for the 7bit address",
				(u32)i2c->base);

		lombo_i2c_dumpregs(i2c);
	}

	csp_i2c_disable_ints((u32)i2c->base, I2C_INT_ALL_MASK);
	csp_i2c_clr_pd((u32)i2c->base, I2C_INT_ALL_MASK);
	csp_i2c_enable((u32)i2c->base, 0);
	lombo_i2c_master_complete(i2c, ret);
	PRT_TRACE_END("");
}

/**
 * lombo_i2c_irq
 *
 * top level IRQ servicing routine
*/
static void lombo_i2c_irq(int irqno, void *dev_id)
{
	struct lombo_i2c *i2c = dev_id;
	u32 int_status = 0;
	u32 flags = 0;
	int ret = 0;

	PRT_TRACE_BEGIN("irqno=%d,dev_id=0x%08x", irqno, dev_id);
/*	LOG_D("0x%08x", (u32)i2c->base); */
	spin_lock_irqsave(&i2c->lock, flags);

	if (csp_i2c_chk_err((u32)i2c->base) != 0) {
		LOG_E("0x%08x: transfer error!", (u32)i2c->base);
		lombo_i2c_stop(i2c, -RT_EIO);
		goto out;
	}

	int_status = csp_i2c_get_int_pd((u32)i2c->base);
	if (!int_status) {
		LOG_E("0x%08x: no i2c interrupt pendings", (u32)i2c->base);
		goto out;
	}

	/* STOP_DET, finish the transfer */
	if ((int_status & I2C_INT_STOP_DET) != 0) {
		/*
		 * clear the pending bits that to be handled
		 */
		csp_i2c_clr_pd((u32)i2c->base,
				I2C_INT_RX_FULL | I2C_INT_TX_EMPTY |
				I2C_INT_STOP_DET);
		csp_i2c_disable_ints((u32)i2c->base,
				I2C_INT_RX_FULL | I2C_INT_TX_EMPTY |
				I2C_INT_STOP_DET);

		/* read the data from RX_FIFO, if any */
		if (csp_i2c_chk_status((u32)i2c->base, I2C_STATUS_RFNE) == 1) {
			u32 rd_entries = 0;

			rd_entries = csp_i2c_get_rx_fl((u32)i2c->base);
			ret = lombo_i2c_batch_read(i2c, rd_entries);
			if (ret != 0) {
				LOG_E("0x%08x", (u32)i2c->base);
				lombo_i2c_stop(i2c, -RT_EIO);
				goto out;
			}

			/* after read, the pending read count should be 0 */
			if (i2c->msg_pd_rd_cnt != 0) {
				LOG_E("0x%08x: i2c->msg_pd_rd_cnt=%d",
					(u32)i2c->base, i2c->msg_pd_rd_cnt);
				lombo_i2c_stop(i2c, -RT_EIO);
				goto out;
			}
		}
		lombo_i2c_stop(i2c, 0);
		goto out;
	}

	/*
	 * When TX_EMPTY interrupt is enable (means that there are commands
	 * to be sent), and TX_EMPTY is raised, than we continue to send the
	 * rest of the commands.
	 */
	if (((i2c->imask & I2C_INT_TX_EMPTY) != 0) &&
		((int_status & I2C_INT_TX_EMPTY) != 0)) {
		/*
		 * disable TX_EMPTY interrupt before sending the commands
		 * to avoid the interrupt raise unexecptedly.
		 */
		csp_i2c_disable_ints((u32)i2c->base, I2C_INT_TX_EMPTY);
		ret = lombo_i2c_batch_cmds(i2c, I2C_TX_FIFO_DEPTH / 2);
		if (ret != 0) {
			LOG_E("0x%08x", (u32)i2c->base);
			lombo_i2c_stop(i2c, -RT_EIO);
			goto out;
		}
		csp_i2c_clr_pd((u32)i2c->base, I2C_INT_TX_EMPTY);
		/* if there are commands to be sent, enable the TX_EMPTY int */
		if ((i2c->imask & I2C_INT_TX_EMPTY) != 0)
			csp_i2c_enable_ints((u32)i2c->base, I2C_INT_TX_EMPTY);
	}

	/*
	 * RX_FULL pending raised means the RX_FIFO has valid data more than
	 * the RX_FIFO threshold (I2C_RX_FIFO_DEPTH / 2 - 1)
	 */
	if (((i2c->imask & I2C_INT_RX_FULL) != 0) &&
		((int_status & I2C_INT_RX_FULL) != 0)) {
		csp_i2c_disable_ints((u32)i2c->base, I2C_INT_RX_FULL);
		ret = lombo_i2c_batch_read(i2c, I2C_RX_FIFO_DEPTH / 2);
		if (ret != 0) {
			LOG_E("0x%08x", (u32)i2c->base);
			lombo_i2c_stop(i2c, -RT_EIO);
			goto out;
		}
		csp_i2c_clr_pd((u32)i2c->base, I2C_INT_RX_FULL);
		/* if there are data to be read, enable the RX_FULL int */
		if ((i2c->imask & I2C_INT_RX_FULL) != 0)
			csp_i2c_enable_ints((u32)i2c->base, I2C_INT_RX_FULL);
	}

 out:
/*	LOG_D("end %p", (u32)i2c->base); */
	spin_unlock_irqrestore(&i2c->lock, flags);
	PRT_TRACE_END("");
}

/**
 * lombo_i2c_wait_idle
 *
 * wait for the i2c bus to become idle.
*/
static void lombo_i2c_wait_idle(struct lombo_i2c *i2c)
{
	int timeout = 200;	/* 200ms */
	int spins;

	PRT_TRACE_BEGIN("i2c=0x%08x", i2c);

	/* ensure the stop has been through the bus */

	LOG_D("0x%08x: waiting for bus idle", (u32)i2c->base);

	/*
	 * Most of the time, the bus is already idle within a few usec of the
	 * end of a transaction.  However, really slow i2c devices can stretch
	 * the clock, delaying STOP generation.
	 *
	 * On slower SoCs this typically happens within a very small number of
	 * instructions so busy wait briefly to avoid scheduling overhead.
	 */
	spins = 3;
	while (--spins &&
		(csp_i2c_chk_status((u32)i2c->base, I2C_STATUS_ACTIVITY) != 0))
			rt_thread_yield();

	/*
	 * If we do get an appreciable delay as a compromise between idle
	 * detection latency for the normal, fast case, and system load in the
	 * slow device case, use an exponential back off in the polling loop,
	 * up to 1/10th of the total timeout, then continue to poll at a
	 * constant rate up to the timeout.
	 */
	while (csp_i2c_chk_status((u32)i2c->base, I2C_STATUS_ACTIVITY) != 0) {
		if (timeout < 0) {
			LOG_E("0x%08x: timeout waiting for bus idle",
				(u32)i2c->base);
			break;
		}
		timeout--;
		rt_thread_delay(1);
	}
	PRT_TRACE_END("");
}

/* lombo_i2c_doxfer - starts an i2c transfer
 * @i2c: point to i2c host structure
 * @msgs: start address of the messages to transfer
 * @num: number of the messages to transfer
 *
 * return message count when success; return error code when failed
 *   TODO: return 0 when address NAK
 */
static rt_size_t lombo_i2c_doxfer(struct lombo_i2c *i2c,
			      struct rt_i2c_msg *msgs, int num)
{
	int ret;

	PRT_TRACE_BEGIN("i2c=0x%08x,msgs=0x%08x,num=%d", i2c, msgs, num);

	rt_completion_init(&i2c->completion);

	ret = lombo_i2c_start_msgs(i2c, msgs, num);
	if (ret != 0) {
		LOG_E("0x%08x: cannot get bus (error %d)", (u32)i2c->base, ret);
		ret = -EAGAIN;
		goto out;
	}

	ret = rt_completion_wait(&i2c->completion, RT_TICK_PER_SECOND * 5);
	if (ret != RT_EOK) {
		LOG_E("0x%08x: timeout", (u32)i2c->base);
		lombo_i2c_stop(i2c, -RT_EIO);
		ret = -RT_ETIMEOUT;
		goto out;
	}

	if (i2c->msg_err != 0) {
		LOG_E("i2c->msg_err=%d", i2c->msg_err);
		ret = i2c->msg_err;
	} else
		ret = num;

	/*
	 * TODO: return 0 when address NAK,
	 *   so that we can distinguish whether the device is busy or
	 *   errors occur.
	 */

out:
	PRT_TRACE_END("");
	return ret;
}

/**
 * lombo_i2c_xfer
 *
 * @i2c: point to i2c host structure
 * @msgs: start address of the messages to transfer
 * @num: number of the messages to transfer
 *
 * first port of call from the i2c bus code when an message needs
 * transferring across the i2c bus.
 *
 * return message count when success; return error code when failed
 *   TODO: return 0 when address NAK
 */
static rt_size_t lombo_i2c_xfer(struct rt_i2c_bus_device *bus,
				struct rt_i2c_msg msgs[], rt_uint32_t num)
{
	struct lombo_i2c *i2c = (struct lombo_i2c *)bus->priv;
	int retry = 0;
	int ret = 0;

	if ((bus == RT_NULL) || (msgs == RT_NULL)) {
		LOG_E("bus=0x%08x,msgs=0x%08x,num=%d", bus, msgs, num);
		return -RT_EINVAL;
	}

	PRT_TRACE_BEGIN("bus=0x%08x,msgs=0x%08x,num=%d", bus, msgs, num);

	for (retry = 0; retry < i2c->retries; retry++) {
		ret = lombo_i2c_doxfer(i2c, msgs, num);
		if (ret != -EAGAIN)
			goto out;

		LOG_D("0x%08x: Retrying transmission (%d)",
			(u32)i2c->base, retry);

		udelay(100);
	}

	ret = -RT_EIO;

out:
	if (ret < 0) {
		LOG_E("ret=%d,i2c->idx=%d,msgs=0x%08x,num=%d",
			ret, i2c->idx, msgs, num);
	}
	PRT_TRACE_END("");
	return ret;
}

static const struct rt_i2c_bus_device_ops lombo_i2c_ops = {
	.master_xfer = lombo_i2c_xfer,
};

static int lombo_i2c_should_disable(u32 i2c_idx)
{
	char i2c_node[6] = {0};
	const char *status;
	int ret;

	rt_snprintf(i2c_node, 6, "i2c%d", i2c_idx);

	ret = config_get_string(i2c_node, "status", &status);
	if (ret != 0)
		return 1;

	if (strcmp(status, "disabled") == 0)
		return 1;

	return 0;
}

/**
 * lombo_i2c_init - init the i2c host with the specified index
 * @i2c_idx: index of the i2c host
 */
static int lombo_i2c_init(u32 i2c_idx)
{
	int ret = 0;
	struct lombo_i2c *i2c = RT_NULL;
	struct rt_i2c_bus_device *i2c_bus_dev = RT_NULL;
	char i2c_dev_name[6] = {0};

	if (i2c_idx >= I2C_MODULE_CNT) {
		LOG_E("i2c_idx=%d", i2c_idx);
		return -RT_EINVAL;
	}

	PRT_TRACE_BEGIN("i2c_idx=%d", i2c_idx);
	if (lombo_i2c_should_disable(i2c_idx)) {
		LOG_D("i2c%d: \"status\" is disabled", i2c_idx);
		goto out;
	}

	i2c_bus_dev = (struct rt_i2c_bus_device *)
			rt_malloc(sizeof(struct rt_i2c_bus_device));
	if (i2c_bus_dev == RT_NULL) {
		ret = -RT_ENOMEM;
		goto err_free;
	}
	rt_memset(i2c_bus_dev, 0, sizeof(struct rt_i2c_bus_device));
	i2c_bus_dev->ops = &lombo_i2c_ops;

	rt_sprintf(i2c_dev_name, "%s%d", "i2c", i2c_idx);

	/* priv struct init */
	i2c = (struct lombo_i2c *)rt_malloc(sizeof(struct lombo_i2c));
	if (i2c == RT_NULL) {
		ret = -RT_ENOMEM;
		goto err_free;
	}
	rt_memset(i2c, 0, sizeof(struct lombo_i2c));

	i2c->idx = i2c_idx;
	i2c->i2c_bus_dev = i2c_bus_dev;
	i2c->retries = 3;
	i2c_bus_dev->priv = i2c;

	ret = i2c_init(i2c);
	if (ret != 0)
		return -RT_EINVAL;

	spin_lock_init(&i2c->lock);
	rt_hw_interrupt_install(i2c->irq, lombo_i2c_irq,
				(void *)i2c, i2c_dev_name);

#ifdef ARCH_LOMBO_N8V0
	switch (i2c_idx) {
	case 0:
		rt_hw_interrupt_umask(INT_I2C0);
		break;
	case 1:
		rt_hw_interrupt_umask(INT_I2C1);
		break;
	case 2:
		rt_hw_interrupt_umask(INT_I2C2);
		break;
	case 3:
		rt_hw_interrupt_umask(INT_I2C3);
		break;
	default:
		ret = -RT_EINVAL;
		goto err_init;
	}
#else
	rt_hw_interrupt_umask(i2c->irq);
#endif

	ret = rt_i2c_bus_device_register(i2c_bus_dev, i2c_dev_name);
	if (ret != RT_EOK) {
		LOG_E("rt_i2c_bus_device_register failed, ret=%d", ret);
		ret = -RT_ENOMEM;
		goto err_init;
	}

	LOG_D("end");

out:
	PRT_TRACE_END("");
	return ret;

err_init:
	if (i2c != RT_NULL)
		i2c_deinit(i2c);

err_free:
	if (i2c != RT_NULL)
		rt_free(i2c);

	if (i2c_bus_dev != RT_NULL)
		rt_free(i2c);

	goto out;
}

/**
 * rt_hw_i2c_init - init all the i2c host that we need to use
 */
int rt_hw_i2c_init(void)
{
	int idx;
	int ret = RT_EOK;

	for (idx = 0; idx < I2C_MODULE_CNT; idx++) {
		ret = lombo_i2c_init(idx);
		if (ret != 0)
			return -EINVAL;
	}

	return RT_EOK;
}

INIT_PREV_EXPORT(rt_hw_i2c_init);

#endif /* ___I2C___DRV__C___ */
