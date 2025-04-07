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

#ifdef LOMBO_USBH_HID

#define DBG_SECTION_NAME	"HID"
#define DBG_LEVEL		DBG_INFO
#include <debug.h>

#include <rtthread.h>
#include <drivers/usb_host.h>
#include "hid.h"

static struct uclass_driver hid_driver;
static rt_list_t _protocal_list;

static rt_err_t hid_pipe_check(struct uhintf *intf, upipe_t pipe)
{
	struct uinstance *device;
	rt_err_t ret;

	if (intf == RT_NULL || pipe == RT_NULL) {
		LOG_E("the interface is not available");
		return -RT_EIO;
	}

	/* get usb device instance from the interface instance */
	device = intf->device;

	/* check pipe status */
	if (pipe->status == UPIPE_STATUS_OK)
		return RT_EOK;

	if (pipe->status == UPIPE_STATUS_ERROR) {
		LOG_E("pipe status error");
		return -RT_EIO;
	}

	if (pipe->status == UPIPE_STATUS_STALL) {
		/* clear the pipe stall status */
		ret = rt_usbh_clear_feature(device, pipe->ep.bEndpointAddress,
						USB_FEATURE_ENDPOINT_HALT);
		if (ret != RT_EOK)
			return ret;
	}

	rt_thread_delay(50);
	pipe->status = UPIPE_STATUS_OK;

	LOG_I("clean hid in pipe stall");

	return RT_EOK;
}

rt_err_t rt_usbh_hid_set_idle(struct uhintf *intf, int duration, int report_id)
{
	struct urequest setup;
	struct uinstance *device;
	int timeout = USB_TIMEOUT_BASIC;
	rt_err_t ret;

	/* parameter check */
	RT_ASSERT(intf != RT_NULL);
	RT_ASSERT(intf->device != RT_NULL);
	device = intf->device;

	setup.request_type = USB_REQ_TYPE_DIR_OUT | USB_REQ_TYPE_CLASS |
				USB_REQ_TYPE_INTERFACE;
	setup.bRequest = USB_REQ_SET_IDLE;
	setup.wIndex = 0;
	setup.wLength = 0;
	setup.wValue = (duration << 8) | report_id;

	if (rt_usb_hcd_setup_xfer(device->hcd, device->pipe_ep0_out, &setup,
					timeout) != 8)
		return -RT_ERROR;

	if (rt_usb_hcd_pipe_xfer(device->hcd, device->pipe_ep0_in, RT_NULL, 0,
					timeout) != 0)
		return -RT_ERROR;

	/* check in pipes status */
	ret = hid_pipe_check(intf, device->pipe_ep0_in);
	if (ret != RT_EOK) {
		LOG_E("in pipe error");
		return ret;
	}

	return RT_EOK;
}

rt_err_t rt_usbh_hid_get_report(struct uhintf *intf, rt_uint8_t type,
				rt_uint8_t id, rt_uint8_t *buffer, rt_size_t size)
{
	struct urequest setup;
	struct uinstance *device;
	int timeout = USB_TIMEOUT_BASIC;

	/* parameter check */
	RT_ASSERT(intf != RT_NULL);
	RT_ASSERT(intf->device != RT_NULL);
	device = intf->device;

	setup.request_type = USB_REQ_TYPE_DIR_IN | USB_REQ_TYPE_CLASS |
				USB_REQ_TYPE_INTERFACE;
	setup.bRequest = USB_REQ_GET_REPORT;
	setup.wIndex = intf->intf_desc->bInterfaceNumber;
	setup.wLength = size;
	setup.wValue = (type << 8) + id;

	if (rt_usb_hcd_setup_xfer(device->hcd, device->pipe_ep0_out, &setup,
				timeout) != 8)
		return -RT_ERROR;

	if (rt_usb_hcd_pipe_xfer(device->hcd, device->pipe_ep0_in, buffer, size,
					timeout) == size)
		return -RT_ERROR;

	if (rt_usb_hcd_pipe_xfer(device->hcd, device->pipe_ep0_out, RT_NULL, 0,
					timeout) == 0)
		return -RT_ERROR;

	return RT_EOK;
}

rt_err_t rt_usbh_hid_set_report(struct uhintf *intf, rt_uint8_t *buffer,
					rt_size_t size)
{
	struct urequest setup;
	struct uinstance *device;
	int timeout = USB_TIMEOUT_BASIC;

	/* parameter check */
	RT_ASSERT(intf != RT_NULL);
	RT_ASSERT(intf->device != RT_NULL);

	device = intf->device;
	setup.request_type = USB_REQ_TYPE_DIR_OUT | USB_REQ_TYPE_CLASS |
				USB_REQ_TYPE_INTERFACE;
	setup.bRequest = USB_REQ_SET_REPORT;
	setup.wIndex = intf->intf_desc->bInterfaceNumber;
	setup.wLength = size;
	setup.wValue = 0x02 << 8;

	if (rt_usb_hcd_setup_xfer(device->hcd, device->pipe_ep0_out, &setup,
					timeout) != 8)
		return -RT_ERROR;
	if (rt_usb_hcd_pipe_xfer(device->hcd, device->pipe_ep0_out, buffer, size,
					timeout) == size)
		return -RT_ERROR;

	if (rt_usb_hcd_pipe_xfer(device->hcd, device->pipe_ep0_in, RT_NULL, 0,
					timeout) == 0)
		return -RT_ERROR;

	return RT_EOK;
}

rt_err_t rt_usbh_hid_set_protocal(struct uhintf *intf, int protocol)
{
	struct urequest setup;
	struct uinstance *device;
	int timeout = USB_TIMEOUT_BASIC;

	/* parameter check */
	RT_ASSERT(intf != RT_NULL);
	RT_ASSERT(intf->device != RT_NULL);

	device = intf->device;
	setup.request_type = USB_REQ_TYPE_DIR_OUT | USB_REQ_TYPE_CLASS |
				USB_REQ_TYPE_INTERFACE;
	setup.bRequest = USB_REQ_SET_PROTOCOL;
	setup.wIndex = 0;
	setup.wLength = 0;
	setup.wValue = protocol;

	if (rt_usb_hcd_setup_xfer(device->hcd, device->pipe_ep0_out, &setup,
					timeout) != 8)
		return -RT_ERROR;

	if (rt_usb_hcd_pipe_xfer(device->hcd, device->pipe_ep0_in, RT_NULL, 0,
					timeout) != 0)
		return -RT_ERROR;

	return RT_EOK;
}

rt_err_t rt_usbh_hid_get_report_descriptor(struct uhintf *intf,
					rt_uint8_t *buffer, rt_size_t size)
{
	struct urequest setup;
	struct uinstance *device;
	int timeout = USB_TIMEOUT_BASIC;

	/* parameter check */
	RT_ASSERT(intf != RT_NULL);
	RT_ASSERT(intf->device != RT_NULL);

	device = intf->device;

	setup.request_type = USB_REQ_TYPE_DIR_IN | USB_REQ_TYPE_STANDARD |
				USB_REQ_TYPE_INTERFACE;
	setup.bRequest = USB_REQ_GET_DESCRIPTOR;
	setup.wIndex = 0;
	setup.wLength = size;
	setup.wValue = USB_DESC_TYPE_REPORT << 8;

	if (rt_usb_hcd_setup_xfer(device->hcd, device->pipe_ep0_out, &setup,
					timeout) != 8)
		return -RT_ERROR;

	if (rt_usb_hcd_pipe_xfer(device->hcd, device->pipe_ep0_in, buffer, size,
					timeout) != size)
		return -RT_ERROR;

	if (rt_usb_hcd_pipe_xfer(device->hcd, device->pipe_ep0_out, RT_NULL, 0,
					timeout) != 0)
		return -RT_ERROR;

	return RT_EOK;
}

rt_err_t rt_usbh_hid_protocal_register(uprotocal_t protocal)
{
	if (protocal == RT_NULL)
		return -RT_ERROR;

	/* insert class driver into driver list */
	rt_list_insert_after(&_protocal_list, &(protocal->list));

	return RT_EOK;
}

static void rt_usbh_hid_callback(void *context)
{
	upipe_t pipe;
	struct uhid *hid;
	int timeout = USB_TIMEOUT_LONG;
	struct uhintf *intf;

	/* parameter check */
	RT_ASSERT(context != RT_NULL);
	pipe = (upipe_t)context;
	RT_ASSERT(pipe->user_data != RT_NULL);
	intf = (struct uhintf *)pipe->user_data;
	hid = (struct uhid *)intf->user_data;

	/* invoke protocal callback function */
	hid->protocal->callback((void *)hid);

	/* parameter check */
	RT_ASSERT(intf->device->hcd != RT_NULL);

	rt_usb_hcd_pipe_xfer(intf->device->hcd, hid->pipe_in,
			hid->buffer, hid->pipe_in->ep.wMaxPacketSize, timeout);
}

static uprotocal_t rt_usbh_hid_protocal_find(int pro_id)
{
	struct rt_list_node *node;

	/* try to find protocal object */
	for (node = _protocal_list.next; node != &_protocal_list; node = node->next) {
		uprotocal_t protocal = (uprotocal_t)rt_list_entry(node, struct uprotocal,
					list);
		if (protocal->pro_id == pro_id)
			return protocal;
	}

	/* not found */
	return RT_NULL;
}

static rt_err_t rt_usbh_hid_enable(void *arg)
{
	int i = 0, pro_id;
	uprotocal_t protocal;
	struct uhid *hid;
	struct uhintf *intf = (struct uhintf *)arg;
	int timeout = USB_TIMEOUT_BASIC;
	struct uinstance *device;

	/* parameter check */
	if (intf == RT_NULL) {
		LOG_E("the interface is not available");
		return -RT_EIO;
	}

	pro_id = intf->intf_desc->bInterfaceProtocol;
	LOG_I("HID device enable, protocal id %d", pro_id);

	protocal = rt_usbh_hid_protocal_find(pro_id);
	if (protocal == RT_NULL) {
		LOG_E("can't find hid protocal %d", pro_id);
		intf->user_data = RT_NULL;
		return -RT_ERROR;
	}

	hid = rt_malloc(sizeof(struct uhid));
	RT_ASSERT(hid != RT_NULL);

	/* initilize the data structure */
	rt_memset(hid, 0, sizeof(struct uhid));
	intf->user_data = (void *)hid;
	hid->protocal = protocal;
	device = intf->device;

	for (i = 0; i < intf->intf_desc->bNumEndpoints; i++) {
		uep_desc_t ep_desc;

		/* get endpoint descriptor */
		rt_usbh_get_endpoint_descriptor(intf->intf_desc, i, &ep_desc);
		if (ep_desc == RT_NULL) {
			LOG_E("rt_usbh_get_endpoint_descriptor error");
			return -RT_ERROR;
		}

		if (USB_EP_ATTR(ep_desc->bmAttributes) != USB_EP_ATTR_INT)
			continue;

		if (!(ep_desc->bEndpointAddress & USB_DIR_IN))
			continue;

		hid->pipe_in = rt_usb_instance_find_pipe(device,
						ep_desc->bEndpointAddress);
		if (hid->pipe_in == RT_NULL) {
			LOG_E("pipe in not found");
			return -RT_ERROR;
		}
		rt_usb_pipe_add_callback(hid->pipe_in, rt_usbh_hid_callback);
		hid->pipe_in->user_data = intf;
	}

	/* initialize hid protocal */
	if (hid->protocal->init)
		hid->protocal->init((void *)intf);
	else
		LOG_E("hid->protocal->init is NULL");

	rt_usb_hcd_pipe_xfer(device->hcd, hid->pipe_in,
		hid->buffer, hid->pipe_in->ep.wMaxPacketSize, timeout);

	return RT_EOK;
}

static rt_err_t rt_usbh_hid_disable(void *arg)
{
	struct uhid *hid;
	uprotocal_t protocal;
	struct uhintf *intf = (struct uhintf *)arg;

	RT_ASSERT(intf != RT_NULL);
	LOG_I("hid diasable");

	hid = (struct uhid *)intf->user_data;
	if (hid != RT_NULL) {
		protocal = hid->protocal;
		if (protocal->destroy)
			protocal->destroy(intf);
		rt_free(hid);
	}

	return RT_EOK;
}

ucd_t rt_usbh_class_driver_hid(void)
{
	rt_list_init(&_protocal_list);

	hid_driver.class_code = USB_CLASS_HID;
	hid_driver.enable = rt_usbh_hid_enable;
	hid_driver.disable = rt_usbh_hid_disable;

	return &hid_driver;
}

#endif

