/*
 * ee_usbd.h - usb device controller head code for LomboTech
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

#ifndef __EE_USBD_H___
#define __EE_USBD_H___

#include <stdlib.h>
#include <drivers/usb_device.h>

#include "soc_define.h"
#include "irq_numbers.h"

#define UDC_IRQ_NUM		INT_USB20

#define ENDPOINT_CONTROL	0

/* Max data length of a transfer description */
#define MAX_DTD_BUFFER_SIZE	(4096 * 5)

/* udc irq service thread */
#define USB_THREAD_PRIO			6
#define USB_THREAD_STACK_SIZE		2048
#define USB_THREAD_TICK			50

static inline int usb_endpoint_phy_num(const struct ee_ep *ep)
{
	return ep->num + (USB_EP_DIR(ep->dir) ? 16 : 0);
}

/* USB EP callback events */
#define EP_EVENT_SETUP	1
#define EP_EVENT_OUT	2
#define EP_EVENT_IN	3
#define EP_EVENT_RESET	4

void ee_set_address(struct lombo_udc *ee, u8 address);
void ee_set_endpoint_stall(u8 ep_num);
void ee_clear_endpoint_stall(u8 ep_num);
void ee_enable_endpoint(struct lombo_udc *ee, u8 ep_num,
			u8 type, u8 dir, u16 size);
void ee_disable_endpoint(struct lombo_udc *ee, u8 ep_num, u8 dir);
u32 ee_prepare_data_transfer(struct lombo_udc *ee, u8 ep_phy_num,
			u8 *buffer, u32 size);
u32 ee_free_data_transfer(struct lombo_udc *ee, u8 ep_phy_num);
int ee_usbd_init(struct lombo_udc *ee);

#endif /* __EE_USBD_H___ */
