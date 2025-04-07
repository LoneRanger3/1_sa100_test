/*
 * usbkbd.c - USB host driver for LomboTech Socs
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

#if defined(LOMBO_USBH_HID) && defined(LOMBO_USBH_HID_KEYBOARD)

#define DBG_SECTION_NAME	"USBKBD"
#define DBG_LEVEL		DBG_INFO
#include <debug.h>

#include <rtthread.h>
#include <drivers/usb_host.h>
#include "hid.h"
#include "input/input.h"

#define USBKB_EVENTS_PER_PACKET	2


static const unsigned char usb_kbd_keycode[256] = {
	  0,   0,   0,   0,  30,  48,  46,  32,  18,  33,  34,  35,  23,  36,  37,  38,
	 50,  49,  24,  25,  16,  19,  31,  20,  22,  47,  17,  45,  21,  44,   2,   3,
	  4,   5,   6,   7,   8,   9,  10,  11,  28,   1,  14,  15,  57,  12,  13,  26,
	 27,  43,  43,  39,  40,  41,  51,  52,  53,  58,  59,  60,  61,  62,  63,  64,
	 65,  66,  67,  68,  87,  88,  99,  70, 119, 110, 102, 104, 111, 107, 109, 106,
	105, 108, 103,  69,  98,  55,  74,  78,  96,  79,  80,  81,  75,  76,  77,  71,
	 72,  73,  82,  83,  86, 127, 116, 117, 183, 184, 185, 186, 187, 188, 189, 190,
	191, 192, 193, 194, 134, 138, 130, 132, 128, 129, 131, 137, 133, 135, 136, 113,
	115, 114,   0,   0,   0, 121,   0,  89,  93, 124,  92,  94,  95,   0,   0,   0,
	122, 123,  90,  91,  85,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	 29,  42,  56, 125,  97,  54, 100, 126, 164, 166, 165, 163, 161, 115, 114, 113,
	150, 158, 159, 128, 136, 177, 178, 176, 142, 152, 173, 140
};

static struct uprotocal kbd_protocal;

static rt_err_t lombo_usbh_hid_kbd_callback(void *arg)
{
	struct uhid *hid = (struct uhid *)arg;
	struct input_dev *dev = hid->input;

	RT_ASSERT(hid != RT_NULL);
	LOG_I("keyboard down: %x, %x, %x, %x, %x, %x, %x, %x",
				hid->buffer[0], hid->buffer[1], hid->buffer[2],
				hid->buffer[3], hid->buffer[4], hid->buffer[5],
				hid->buffer[6], hid->buffer[7]);
	LOG_I("keycode:%d", usb_kbd_keycode[hid->buffer[2]]);

	if (dev) {
		input_report_key(dev, usb_kbd_keycode[hid->buffer[2]], 1);
		input_sync(dev);
	} else
		LOG_W("hid->input is NULL");

	return RT_EOK;
}

static rt_err_t lombo_usbh_hid_kbd_init(void *arg)
{
	struct uhintf *intf = (struct uhintf *)arg;
	struct uhid *hid;
	struct input_dev *input;
	rt_err_t ret;
	int i, keycode;

	LOG_I("hid keyboard init");
	RT_ASSERT(intf != RT_NULL);
	hid = (struct uhid *)intf->user_data;
	RT_ASSERT(hid != RT_NULL);

	/* alloc input dev */
	input = input_allocate_device();
	if (!input) {
		LOG_E("alloc input dev failed");
		return -RT_EIO;
	}
	input->name = "usb_keyborad";

	/* event bit */
	set_bit(EV_KEY, input->evbit);

	/* key bit */
	for (i = 0; i < 256; i++) {
		keycode = usb_kbd_keycode[i];
		set_bit(keycode, input->keybit);
	}

	/* set num_vals, max_vals, vals */
	input->num_vals = 0;
	input->max_vals = USBKB_EVENTS_PER_PACKET + 2;
	input->vals = rt_malloc(sizeof(struct input_value) * input->max_vals);
	if (input->vals == RT_NULL) {
		LOG_E("rt_malloc for input_dev vals error");
		ret = -RT_ENOMEM;
		goto failed;
	}

	/* register input devic */
	ret = input_register_device(input);
	if (ret) {
		LOG_E("register input dev failed");
		goto failed;
	}

	hid->input = input;

	/* set idle */
	rt_usbh_hid_set_idle(intf, 0, 0);

	return RT_EOK;

failed:
	/*  free input device */
	input_free_device(input);
	return ret;
}

static rt_err_t lombo_usbh_hid_kbd_destory(void *arg)
{
	struct uhintf *intf = (struct uhintf *)arg;
	struct uhid *hid;

	LOG_I("hid keyboard destroy");
	RT_ASSERT(intf != RT_NULL);
	hid = (struct uhid *)intf->user_data;
	RT_ASSERT(hid != RT_NULL);

	if (hid->input) {
		input_unregister_device(hid->input);
		/* free input device */
		input_free_device(hid->input);
		hid->input = RT_NULL;
	}

	return RT_EOK;
}

uprotocal_t lombo_usbh_hid_protocal_kbd(void)
{
	kbd_protocal.pro_id = USB_HID_KEYBOARD;
	kbd_protocal.init = lombo_usbh_hid_kbd_init;
	kbd_protocal.callback = lombo_usbh_hid_kbd_callback;
	kbd_protocal.destroy = lombo_usbh_hid_kbd_destory;

	return &kbd_protocal;
}

#endif

