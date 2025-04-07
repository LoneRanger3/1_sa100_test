/*
 * usbmouse.c - USB host driver for LomboTech Socs
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

#if defined(LOMBO_USBH_HID) && defined(LOMBO_USBH_HID_MOUSE)

#define DBG_SECTION_NAME	"USBMOUSE"
#define DBG_LEVEL		DBG_LOG
#include <debug.h>

#include <drivers/usb_host.h>
#include "hid.h"
#include "input/input.h"

#define MOUSE_EVENTS_PER_PACKET		9

static struct uprotocal mouse_protocal;

/* log for debug */
void log_mouse_data(rt_uint8_t buffer[4])
{
	LOG_D("BTN_LEFT: %d", buffer[0] & 0x01);
	LOG_D("BTN_RIGHT: %d", buffer[0] & 0x02);
	LOG_D("BTN_MIDDLE: %d", buffer[0] & 0x04);
	LOG_D("BTN_SIDE: %d", buffer[0] & 0x08);
	LOG_D("BTN_EXTRA: %d", buffer[0] & 0x10);

	LOG_D("REL_X: %d", buffer[1]);
	LOG_D("REL_Y: %d", buffer[2]);
	LOG_D("REL_WHEEL: %d", buffer[3]);
}

static rt_err_t lombo_usbh_hid_mouse_callback(void *arg)
{
	struct uhid *hid = (struct uhid *)arg;
	struct input_dev *dev = hid->input;

	RT_ASSERT(arg);
	LOG_I("hid mouse callback: %d %d %d %d", hid->buffer[0],
			hid->buffer[1], hid->buffer[2], hid->buffer[3]);

	/* report input event */
	if (dev) {
		/* log_mouse_data(hid->buffer); */

		input_report_key(dev, BTN_LEFT,   (rt_int8_t)hid->buffer[0] & 0x01);
		input_report_key(dev, BTN_RIGHT,  (rt_int8_t)hid->buffer[0] & 0x02);
		input_report_key(dev, BTN_MIDDLE, (rt_int8_t)hid->buffer[0] & 0x04);
		input_report_key(dev, BTN_SIDE,   (rt_int8_t)hid->buffer[0] & 0x08);
		input_report_key(dev, BTN_EXTRA,  (rt_int8_t)hid->buffer[0] & 0x10);

		input_report_rel(dev, REL_X,     (rt_int8_t)hid->buffer[1]);
		input_report_rel(dev, REL_Y,     (rt_int8_t)hid->buffer[2]);
		input_report_rel(dev, REL_WHEEL, (rt_int8_t)hid->buffer[3]);

		input_sync(dev);
	} else
		LOG_W("hid->input is NULL");

	return RT_EOK;
}

static rt_err_t lombo_usbh_hid_mouse_init(void *arg)
{
	struct uhintf *intf = (struct uhintf *)arg;
	struct uhid *hid;
	struct input_dev *input;
	rt_err_t ret;

	LOG_I("hid mouse init");
	RT_ASSERT(intf != RT_NULL);
	hid = (struct uhid *)intf->user_data;
	RT_ASSERT(hid != RT_NULL);
	input = hid->input;

	/* alloc input dev */
	input = input_allocate_device();
	if (!input) {
		LOG_E("alloc input dev failed");
		return -RT_EIO;
	}
	input->name = "usb_mouse";

	/* event bit */
	set_bit(EV_KEY, input->evbit);
	set_bit(EV_REL, input->evbit);

	/* key bit */
	set_bit(BTN_LEFT, input->keybit);
	set_bit(BTN_RIGHT, input->keybit);
	set_bit(BTN_MIDDLE, input->keybit);
	set_bit(BTN_SIDE, input->keybit);
	set_bit(BTN_EXTRA, input->keybit);

	/* relative bit */
	set_bit(REL_X, input->relbit);
	set_bit(REL_Y, input->relbit);
	set_bit(REL_WHEEL, input->relbit);

	/* set num_vals, max_vals, vals */
	input->num_vals = 0;
	input->max_vals = MOUSE_EVENTS_PER_PACKET + 2;
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
	/* free input device */
	input_free_device(input);
	return ret;
}

static rt_err_t lombo_usbh_hid_mouse_destory(void *arg)
{
	struct uhintf *intf = (struct uhintf *)arg;
	struct uhid *hid;

	LOG_D("hid mouse destroy");
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

uprotocal_t lombo_usbh_hid_protocal_mouse(void)
{
	mouse_protocal.pro_id = USB_HID_MOUSE;
	mouse_protocal.init = lombo_usbh_hid_mouse_init;
	mouse_protocal.callback = lombo_usbh_hid_mouse_callback;
	mouse_protocal.destroy = lombo_usbh_hid_mouse_destory;

	return &mouse_protocal;
}

#endif

