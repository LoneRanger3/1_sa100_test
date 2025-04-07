/*
 * hid.h - USB module head file
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

#ifndef __HID_H__
#define __HID_H__

#include <rtthread.h>

struct uhid {
	upipe_t pipe_in;
	rt_uint8_t buffer[8];
	uprotocal_t protocal;
	struct input_dev *input;
};

typedef struct uhid uhid_t;

#define USB_REQ_GET_REPORT	0x01
#define USB_REQ_GET_IDLE	0x02
#define USB_REQ_GET_PROTOCOL	0x03
#define USB_REQ_SET_REPORT	0x09
#define USB_REQ_SET_IDLE	0x0a
#define USB_REQ_SET_PROTOCOL	0x0b

#define USB_HID_BOOT	        1
#define USB_HID_KEYBOARD	1
#define USB_HID_MOUSE		2

rt_err_t rt_usbh_hid_set_idle(struct uhintf *intf, int duration, int report_id);
rt_err_t rt_usbh_hid_get_report(struct uhintf *intf, rt_uint8_t type,
				rt_uint8_t id, rt_uint8_t *buffer, rt_size_t size);
rt_err_t rt_usbh_hid_set_report(struct uhintf *intf, rt_uint8_t *buffer,
				rt_size_t size);
rt_err_t rt_usbh_hid_set_protocal(struct uhintf *intf, int protocol);
rt_err_t rt_usbh_hid_get_report_descriptor(struct uhintf *intf,
				rt_uint8_t *buffer, rt_size_t size);
rt_err_t rt_usbh_hid_protocal_register(uprotocal_t protocal);
ucd_t rt_usbh_class_driver_hid(void);

#endif
