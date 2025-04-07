/*
 * dma.h - DMA subsystem code for LomboTech
 * dma subsystem interface and macro define
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

#ifndef __DMA_H___
#define __DMA_H___

#include <stdlib.h>
#include "soc_define.h"

/* dma transfer type */
enum dma_trans_type {
	MEMORY_TRANS = 1,
	SG_TRANS = 2,
	CYCLIC_TRANS = 4,
};

/* interrupt type */
enum dma_int_type {
	HALF_BLOCK_INT,
	BLOCK_INT,
	COMPLETE_INT,
};

/**
 * enum dma_transfer_direction - dma transfer mode and direction indicator
 * @DMA_MEM_TO_MEM: Async/Memcpy mode
 * @DMA_MEM_TO_DEV: Memory to Device
 * @DMA_DEV_TO_MEM: Device to Memory
 * @DMA_DEV_TO_DEV: Device to Device
 */
enum dma_transfer_direction {
	DMA_MEM_TO_MEM,
	DMA_MEM_TO_DEV,
	DMA_DEV_TO_MEM,
	DMA_DEV_TO_DEV,
	DMA_TRANS_NONE,
};

/**
 * enum dma_buswidth - defines bus with of the DMA slave
 * device, source or target buses
 */
enum dma_buswidth {
	DMA_BUSWIDTH_UNDEFINED = 0,
	DMA_BUSWIDTH_1_BYTE = 1,
	DMA_BUSWIDTH_2_BYTES = 2,
	DMA_BUSWIDTH_4_BYTES = 4,
	DMA_BUSWIDTH_8_BYTES = 8,
};

/**
 * struct dma_config - dma channel runtime config
 * @direction: whether the data shall go in or out on this channel
 * @src_addr: the physical address where DMA data should be read (RX)
 * @dst_addr: the physical address where DMA data should be written (TX)
 * @src_addr_width: the width in bytes of the source DMA data shall be read
 * @dst_addr_width: the width in bytes of the source DMA data shall be write
 * @src_maxburst: the maximum number of words (note: words, as in
 * units of the src_addr_width member, not bytes) that can be sent
 * in one burst to the device.
 * @dst_maxburst: same as src_maxburst but for destination target
 * mutatis mutandis.
 * @slave_id: Slave requester id.
 * @priority: channel transfer priority
 * @int_type: interrupt type
 * @trans_type: transfer type
 */
struct dma_config {
	enum dma_transfer_direction direction;
	u32 src_addr;
	u32 dst_addr;
	enum dma_buswidth src_addr_width;
	enum dma_buswidth dst_addr_width;
	u32 src_maxburst;
	u32 dst_maxburst;
	unsigned int slave_id;
	u32 priority;
	enum dma_int_type int_type;
	enum dma_trans_type trans_type;
};

/* transfer callback function */
typedef void (*dma_tx_callback)(void *dma_param);

/**
 * struct dma_tx_descriptor - transaction descriptor
 * @chan: target channel for this operation
 * @callback: routine to call after this operation is complete
 * @callback_param: general parameter to pass to the callback routine
 */
struct dma_tx_descriptor {
	struct dma_channel *chan;
	dma_tx_callback callback;
	void *callback_param;
};

/**
 * struct dma_channel - dma subsystem channel
 * @chan_id: channel id
 * @name: the device who request this channel
 * @list: used to add to global chan list
 * @config: channel config
 * @dma_alloc_chan_res: alloc channel resources before transfer
 * @dma_free_chan_res: release channel resources
 * @dma_config_channel: config a channel operation
 * @dma_prep_sg: prepare a dma operation
 * @dma_prep_cyclic: prepare a cyclic dma operation
 * @dma_submit: submit dma descriptor for transfer operation
 * @dma_issue_pending: start a dma transfer
 * @dma_terminate: shut down channel transfer
 * @request: whether the dma channel requested
 */
struct dma_channel {
	int chan_id;
	char *name;
	struct rt_list_node list;

	rt_err_t (*dma_alloc_chan_res)(struct dma_channel *chan);
	void (*dma_free_chan_res)(struct dma_channel *chan);

	rt_err_t (*dma_config_chan)(struct dma_channel *chan,
				struct dma_config *config);

	rt_err_t (*dma_prep_sg)(struct dma_channel *chan, u32 src_addr,
				u32 dst_addr, u32 sg_len,
				enum dma_transfer_direction dir,
				dma_tx_callback callback, void *callback_param);
	rt_err_t (*dma_prep_cyclic)(struct dma_channel *chan, u32 src_addr,
				    u32 dst_addr, u32 buf_len, u32 period_len,
				    enum dma_transfer_direction dir,
				    dma_tx_callback callback,
				    void *callback_param);
	rt_err_t (*dma_submit)(struct dma_channel *chan);
	void (*dma_issue_pending)(struct dma_channel *chan);
	void (*dma_terminate)(struct dma_channel *chan);

	rt_bool_t request;
};

/* function for dma driver */
void dma_init(void);
rt_err_t dma_add_channel(struct dma_channel *chan);

/* function for other driver or application */
struct dma_channel *dma_request_channel(char *name);
void dma_release_channel(struct dma_channel *chan);
rt_err_t dma_config_channel(struct dma_channel *chan,
			    struct dma_config *config);
rt_err_t dma_prep_sg(struct dma_channel *chan, u32 src_addr,
		     u32 dst_addr, u32 sg_len,
		     enum dma_transfer_direction dir,
		     dma_tx_callback callback, void *callback_param);
rt_err_t dma_prep_cyclic(struct dma_channel *chan, u32 src_addr,
			 u32 dst_addr, u32 buf_len,
			 u32 period_len, enum dma_transfer_direction dir,
			 dma_tx_callback callback, void *callback_param);
rt_err_t dma_submit(struct dma_channel *chan);
void dma_issue_pending(struct dma_channel *chan);
void dma_terminate(struct dma_channel *chan);

#endif /* __DMA_H___ */
