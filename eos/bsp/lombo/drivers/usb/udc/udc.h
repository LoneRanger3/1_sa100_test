/*
 * udc.h - usb device controller head code for LomboTech
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

#ifndef __UDC_H___
#define __UDC_H___

#include <stdlib.h>
#include "soc_define.h"
#include "clk.h"
#include "drivers/usb_common.h"

#define EE_FORCE_SPEED_FULL		0

#if EE_FORCE_SPEED_FULL
#define EP_MAX_PACKET_SIZE		64
#define CTL_EP_MAX_PACKET_SIZE		64
#else
#define EP_MAX_PACKET_SIZE		512
#define CTL_EP_MAX_PACKET_SIZE		64
#endif

#define TOTAL_ENDPOINTS			3
#define EP_MAX				(TOTAL_ENDPOINTS * 2)
#define EP_QUEUE_HEAD_SIZE		64
#define EP_QUEUE_HEAD_TOTAL_SIZE	(EP_QUEUE_HEAD_SIZE * EP_MAX)
#define EP_QUEUE_HEAD_ALIGN		2048
#define EP_TD_ALIGN			32
#define TD_MAX_NUM			5
#define TD_NODE_MAX_NUM			10

#define RX				0
#define TX				1

#define LINK_TERMINATE			1

/* transfer desctriptors */
struct hw_td {
	/* word 0 */
	u32 next_td;
	/* word 1 */
	u32 rsvd0:3;
	volatile u32 trans_err:1;
	u32 rsvd1:1;
	volatile u32 buffer_err:1;
	volatile u32 halted:1;
	volatile u32 active:1;
	u32 rsvd2:2;
	u32 mult_override:2;
	u32 rsvd3:3;
	volatile u32 ioc:1;
	volatile u32 total_bytes:15;
	/* word 2 - 6 */
	u32 page[5];
	u32 used; /* reserved area */
} __packed __aligned(4);

/* queue heads */
struct hw_qh {
	/* word 0: capability/characteristics */
	u32 rsvd0:15;
	volatile u32 int_on_setup:1;
	u32 max_packet_size:11;
	u32 rsvd1:2;
	volatile u32 zero_len_termination:1;
	u32 mult:2;
	/* word 1 */
	u32 current_td;
	/* 2 - 9 */
	struct hw_td td;
	/* 10 - 11 */
	struct urequest setup;
} __packed __aligned(4);

struct td_node {
	struct list_head	td;
	u32			phy_addr;
	struct hw_td		*ptr;
	u32			used;
};

#define EP_STA_ACTIVE		1
#define EP_STA_INACTIVE		0

/* lombo usb endpoint define */
struct ee_ep {
	u8	num;	/* ep number */
	u8	dir;   /* 0x00 = out, 0x80 = in */
	u32	type;    /* ep type */
	u32	maxpacket; /* max packet bytes */
	u8	active; /* 0 = de-active, 1 = active */
	void	*xfer_buff;   /* pointer to transfer buffer */
	u32	xfer_len;   /* number of bytes to transfer */
	u32	xfer_count; /* number of bytes transfered */
	struct {
		struct list_head list;
		struct hw_qh *ptr;
		u32 phy_addr;
	} qh;
	struct list_head	tdl;
};

/* device enum status define */
enum usb_device_states_t {
	DEVICE_STATE_UNATTACHED = 0,
	DEVICE_STATE_POWERED = 1,
	DEVICE_STATE_DEFAULT = 2,
	DEVICE_STATE_ADDRESSED = 3,
	DEVICE_STATE_CONFIGURED = 4,
	DEVICE_STATE_SUSPENDED = 5,
};

/* usb device controller state */
union ee_udc_state {
	u8 d8;
	struct {
		unsigned event:1;
		unsigned state:7;
	} b;
};

#define USB_SPEED_HIGH		0
#define USB_SPEED_FULL		1

/* lombo usb device struct */
struct lombo_udc {
	clk_handle_t		usb_gate;
	clk_handle_t		usb_reset;
	clk_handle_t		phy_reset;
	union ee_udc_state	status;
	u8			speed;
	struct ee_ep		ep[EP_MAX];
	rt_sem_t		isr_sem;

	struct hw_td		*td_pool[TD_MAX_NUM];
	struct td_node		*td_node_pool[TD_NODE_MAX_NUM];
	void			*qh_pool;

	rt_bool_t		setaddr;
	u8			address;
	u8			suspended;
	/* struct ee_ep		*ep0out; */
	/* struct ee_ep		*ep0in; */
	rt_thread_t		isr_thread;
	struct udcd		*udc_device;
};

void udc_event_cb(struct lombo_udc *udc, u8 ep_phy_num, u32 event, void *arg);

#endif /* __UDC_H___ */
