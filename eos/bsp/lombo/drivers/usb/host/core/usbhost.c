/*
 * usbhost.c - USB host driver for LomboTech Socs
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

#define DBG_SECTION_NAME	"USBH"
#define DBG_LEVEL		DBG_INFO
#include <debug.h>

#include "drivers/usb_host.h"
#include "drivers/usb_common.h"
#include "host.h"

#if defined(LOMBO_USBH_HID)
#include <hid.h>
#endif

#define USB_HOST_CONTROLLER_NAME      "usbh"

rt_err_t lombo_usb_host_init(void)
{
	ucd_t drv;
	rt_device_t uhc;
	rt_err_t ret;
#if defined(LOMBO_USBH_HID_MOUSE) || defined(LOMBO_USBH_HID_KEYBOARD)
	uprotocal_t protocal;
#endif
	uhc = rt_device_find(USB_HOST_CONTROLLER_NAME);
	if (uhc == RT_NULL) {
		LOG_E("can't find usb host controller %s\n", USB_HOST_CONTROLLER_NAME);
		return -RT_ERROR;
	}

	/* initialize usb hub */
	rt_usbh_hub_init((uhcd_t)uhc);

	/* initialize class driver */
	rt_usbh_class_driver_init();

#ifdef RT_USBH_MSTORAGE
	/* register mass storage class driver */
	drv = rt_usbh_class_driver_storage();
	rt_usbh_class_driver_register(drv);
#endif

#ifdef LOMBO_USBH_HID

	drv = rt_usbh_class_driver_hid();
	rt_usbh_class_driver_register(drv);

#ifdef LOMBO_USBH_HID_MOUSE
	protocal = lombo_usbh_hid_protocal_mouse();
	rt_usbh_hid_protocal_register(protocal);
#endif

#ifdef LOMBO_USBH_HID_KEYBOARD
	protocal = lombo_usbh_hid_protocal_kbd();
	rt_usbh_hid_protocal_register(protocal);
#endif

#endif /* LOMBO_USBH_HID */

	/* register hub class driver */
	drv = rt_usbh_class_driver_hub();
	rt_usbh_class_driver_register(drv);

	/* initialize usb host controller */
	ret = rt_device_init(uhc);

	return ret;
}

