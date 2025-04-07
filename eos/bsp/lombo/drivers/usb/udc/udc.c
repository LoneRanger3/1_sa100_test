/*
 * udc.c - usb device controller driver code for LomboTech
 * dma subsystem
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

#define DBG_SECTION_NAME	"udc"
#define DBG_LEVEL		DBG_INFO
#include <debug.h>

#include <rtthread.h>
#include <rthw.h>
#include <drivers/usb_device.h>

#include "udc.h"
#include "ee_usbd.h"

/* global udc struct */
static struct udcd ee_udc_device;
static struct lombo_udc ee_udc;

/* global endpoint pool */
static struct ep_id ee_ep_pool[] = {
	/* ep0 */
	{0x00, USB_EP_ATTR_CONTROL,	USB_DIR_INOUT,	64,  ID_ASSIGNED},
	/* ep1 */
	{0x01, USB_EP_ATTR_BULK,	USB_DIR_OUT,	512, ID_UNASSIGNED},
	/* ep2 */
	{0x02, USB_EP_ATTR_BULK,	USB_DIR_IN,	512, ID_UNASSIGNED},
	/* ep3 */
	{0x03, USB_EP_ATTR_INT,		USB_DIR_OUT,	512, ID_UNASSIGNED},
	/* ep4 */
	{0x04, USB_EP_ATTR_INT,		USB_DIR_IN,	512, ID_UNASSIGNED},
	/* None */
	{0xFF, USB_EP_ATTR_TYPE_MASK,	USB_DIR_MASK,	0,   ID_ASSIGNED},
};

/* set device address */
static rt_err_t ee_udc_set_address(rt_uint8_t address)
{
	LOG_D("set address 0x%x", address);

	ee_set_address(&ee_udc, address);
	ee_udc.status.b.state = DEVICE_STATE_ADDRESSED;

	return RT_EOK;
}

/* select device config */
static rt_err_t ee_udc_set_config(rt_uint8_t config)
{
	LOG_D("set config 0x%x", config);

	ee_udc.status.b.state = DEVICE_STATE_CONFIGURED;

	return RT_EOK;
}

/* set endpoint stall status */
static rt_err_t ee_udc_ep_set_stall(rt_uint8_t address)
{
	u8 ep_num = address & USB_EPNO_MASK;

	LOG_D("set stall 0x%x", ep_num);
	ee_set_endpoint_stall(ep_num);

	return RT_EOK;
}

/* clear endpoint stall status */
static rt_err_t ee_udc_ep_clear_stall(rt_uint8_t address)
{
	u8 ep_num = address & USB_EPNO_MASK;

	LOG_D("clear stall 0x%x", ep_num);
	ee_clear_endpoint_stall(ep_num);

	return RT_EOK;
}

/* enable special endpoint */
static rt_err_t ee_udc_ep_enable(struct uendpoint *ep)
{
	u8 ep_num, ep_dir, ep_type;
	u16 ep_max_pkt;

	RT_ASSERT(ep != RT_NULL);
	RT_ASSERT(ep->ep_desc != RT_NULL);

	LOG_D("enable ep 0x%x", EP_ADDRESS(ep));

	ep_num = EP_ADDRESS(ep) & USB_EP_DESC_NUM_MASK;
	ep_dir = EP_ADDRESS(ep) & USB_DIR_MASK;
	ep_type = EP_ATTRI(ep) & USB_EP_ATTR_TYPE_MASK;
	ep_max_pkt = EP_MAXPACKET(ep);
	ee_enable_endpoint(&ee_udc, ep_num, ep_type, ep_dir, ep_max_pkt);

	return RT_EOK;
}

/* disable special endpoint */
static rt_err_t ee_udc_ep_disable(struct uendpoint *ep)
{
	u8 ep_num, ep_dir;

	RT_ASSERT(ep != RT_NULL);
	RT_ASSERT(ep->ep_desc != RT_NULL);

	LOG_D("disable ep 0x%x", EP_ADDRESS(ep));

	ep_num = EP_ADDRESS(ep) & USB_EP_DESC_NUM_MASK;
	ep_dir = EP_ADDRESS(ep) & USB_DIR_MASK;
	ee_disable_endpoint(&ee_udc, ep_num, ep_dir);

	return RT_EOK;
}

/* Prepare to receive data from host, non-blocking. */
static rt_size_t ee_udc_ep_read_prepare(rt_uint8_t address,
		void *buffer, rt_size_t size)
{
	u8 ep_phy_num = 2 * USB_EP_NUM(address);

	LOG_D("ep-%d-%s read prepare buf:0x%x, size:0x%x", ep_phy_num,
				USB_EP_DIR(address) ? "in" : "out", (u32)buffer, size);

	ee_udc.ep[ep_phy_num].xfer_len = size;
	ee_udc.ep[ep_phy_num].xfer_buff = buffer;

	return ee_prepare_data_transfer(&ee_udc, ep_phy_num, buffer, size);
}

/*
* The core invoke this function to read data from controller,
* but the data is already put into the ep buffer when ep_read_prepare is finished.
* So we have nothing to do here and just return the length of data.
*/
static rt_size_t ee_udc_ep_read(rt_uint8_t address, void *buffer)
{
	u8 ep_phy_num = 2 * USB_EP_NUM(address);

	return ee_udc.ep[ep_phy_num].xfer_len;
}

/* endpoint send data */
static rt_size_t ee_udc_ep_write(rt_uint8_t address, void *buffer, rt_size_t size)
{
	u8 ep_phy_num = 2 * USB_EP_NUM(address) + 1;

	LOG_D("ep-%d-%s write buf:0x%x, size:0x%x", ep_phy_num,
				USB_EP_DIR(address) ? "in" : "out", (u32)buffer, size);

	ee_udc.ep[ep_phy_num].xfer_len = size;
	ee_udc.ep[ep_phy_num].xfer_buff = buffer;

	return ee_prepare_data_transfer(&ee_udc, ep_phy_num, buffer, size);
}

/* control endpoint0 send status */
static rt_err_t ee_udc_ep0_send_status(void)
{
	u8 ep_phy_num = 1;

	LOG_D("ep0 send status");

	ee_udc.ep[ep_phy_num].xfer_len = 0;
	ee_udc.ep[ep_phy_num].xfer_buff = RT_NULL;

	return ee_prepare_data_transfer(&ee_udc, ep_phy_num, RT_NULL, 0);
}

/* device suspend */
static rt_err_t ee_udc_suspend(void)
{
	LOG_E("suspend not support now");
	return RT_EOK;
}

/* device wake up */
static rt_err_t ee_udc_wakeup(void)
{
	LOG_E("wakeup not support now");
	return RT_EOK;
}

/* usb device driver resources init */
static rt_err_t ee_udc_init(rt_device_t device)
{
	return ee_usbd_init((struct lombo_udc *)device->user_data);
}

void udc_event_cb(struct lombo_udc *udc, u8 ep_phy_num, u32 event, void *arg)
{
	struct udcd *udc_device = &ee_udc_device;

	LOG_D("event cb phy-ep-%d, evenct:%d", ep_phy_num, event);

	switch (event) {
	case EP_EVENT_SETUP:
		if (ep_phy_num == 0)
			rt_usbd_ep0_setup_handler(udc_device, (struct urequest *)arg);
		break;
	case EP_EVENT_OUT:
		if (ep_phy_num == 0)
			rt_usbd_ep0_out_handler(udc_device, (rt_size_t)arg);
		else
			rt_usbd_ep_out_handler(udc_device, USB_DIR_OUT | ep_phy_num, 0);
		break;
	case EP_EVENT_IN:
		if (ep_phy_num == 0)
			rt_usbd_ep0_in_handler(udc_device);
		else
			rt_usbd_ep_in_handler(udc_device, USB_DIR_IN | ep_phy_num,
				udc->ep[2 * ep_phy_num + 1].xfer_len);
		break;
	case EP_EVENT_RESET:
		rt_usbd_reset_handler(udc_device);
		break;
	default:
		break;
	}
}

static struct udcd_ops ee_udcd_ops = {
	ee_udc_set_address,
	ee_udc_set_config,
	ee_udc_ep_set_stall,
	ee_udc_ep_clear_stall,
	ee_udc_ep_enable,
	ee_udc_ep_disable,
	ee_udc_ep_read_prepare,
	ee_udc_ep_read,
	ee_udc_ep_write,
	ee_udc_ep0_send_status,
	ee_udc_suspend,
	ee_udc_wakeup
};

int lombo_udc_init(void)
{
	struct udcd *udc_device = &ee_udc_device;
	struct lombo_udc *udc = &ee_udc;
	rt_err_t ret;

	LOG_D("lombo_udc_init enter");

	/* Initialize udcd and register usb device */
	rt_memset(udc_device, 0, sizeof(struct udcd));
	udc_device->parent.type = RT_Device_Class_USBDevice;
	udc_device->parent.init = ee_udc_init;
	udc_device->parent.user_data = udc;
	udc_device->ops = &ee_udcd_ops;
	udc_device->ep_pool = ee_ep_pool;
	udc_device->ep0.id = &ee_ep_pool[0];
	udc_device->device_is_hs = EE_FORCE_SPEED_FULL ? RT_FALSE : RT_TRUE;

	ret = rt_device_register(&udc_device->parent, "usbd", 0);
	if (ret) {
		LOG_E("register usb device failed");
		return ret;
	}

	/* Initialize usb core */
	ret = rt_usb_device_init();
	if (ret) {
		LOG_E("usb core init failed");
		goto device_init_failed;
	}

	LOG_I("lombo_udc_init ok");

	return RT_EOK;

device_init_failed:
	rt_device_unregister(&udc_device->parent);

	return ret;
}

INIT_ENV_EXPORT(lombo_udc_init);

