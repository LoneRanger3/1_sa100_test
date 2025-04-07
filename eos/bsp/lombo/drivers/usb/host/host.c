/*
 * host.c - USB host driver for LomboTech Socs
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

#include <rthw.h>
#include "soc_define.h"

#include "drivers/usb_host.h"
#include "drivers/usb_common.h"
#include "ehci.h"
#include "host.h"

static rt_err_t lombo_reset_port(rt_uint8_t port)
{
	EHCI_LOG_I("reset port:%d", port);

	hcd_port_reset(port);

	return RT_EOK;
}

static int lombo_pipe_xfer(upipe_t pipe, rt_uint8_t token, void *buffer,
			int nbytes, int timeout)
{
	RT_ASSERT(pipe != RT_NULL);
	EHCI_LOG_I("%s buffer:0x%x, len:0x%x, timeout:%d",
			(token == USBH_PID_SETUP) ? "setup" : "data",
			buffer, nbytes, timeout);

	if (token == USBH_PID_SETUP)
		return hcd_control_xfer(pipe, buffer, nbytes);
	else /* USBH_PID_DATA */
		return hcd_data_xfer(pipe, buffer, nbytes);
}

static rt_err_t lombo_open_pipe(upipe_t pipe)
{
	RT_ASSERT(pipe != RT_NULL);
	EHCI_LOG_I("open pipe");

	return hcd_open_pipe(pipe);
}

static rt_err_t lombo_close_pipe(upipe_t pipe)
{
	RT_ASSERT(pipe != RT_NULL);
	EHCI_LOG_I("close pipe");

	return hcd_close_pipe(pipe);
}

static struct uhcd_ops lombo_uhcd_ops = {
	lombo_reset_port,
	lombo_pipe_xfer,
	lombo_open_pipe,
	lombo_close_pipe
};

static rt_err_t lombo_hcd_init(rt_device_t dev)
{
	uhcd_t uhcd = (uhcd_t)dev->user_data;

	return hcd_init(uhcd);
}

int lombo_usbh_init(void)
{
	uhcd_t uhcd;
	rt_err_t ret;

	/* Init and register hcd */
	uhcd = (uhcd_t)rt_zalloc(sizeof(struct uhcd));
	if (!uhcd) {
		LOG_E("alloc mem for uhcd");
		return -RT_ENOMEM;
	}
	uhcd->parent.type = RT_Device_Class_USBHost;
	uhcd->parent.init = lombo_hcd_init;
	uhcd->parent.user_data = uhcd;
	uhcd->ops = &lombo_uhcd_ops;
	uhcd->num_ports = 1;
	ret = rt_device_register((rt_device_t)uhcd, "usbh", 0);
	if (ret != RT_EOK) {
		LOG_E("register usb hcd failed");
		goto free;
	}

	/* Usb host core init */
	ret = lombo_usb_host_init();
	if (ret != RT_EOK) {
		LOG_E("usb host init failed");
		goto unregister;
	}

	LOG_I("usb host init ok");

	return RT_EOK;

unregister:
	rt_device_unregister((rt_device_t)uhcd);
free:
	rt_free(uhcd);

	return ret;
}

INIT_ENV_EXPORT(lombo_usbh_init);

