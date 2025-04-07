/*
 * usbh_csp.h - USB module head file
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


#ifndef __USBH_CSP_H__
#define __USBH_CSP_H__

#include "soc_define.h"

/* USBCMD */
#define  ASYNC_ADVANCE_ENABLE     1
#define  USB_RUN                  1
#define  USB_STOP                 0
#define  HOST_HALTED              1
#define  HOST_NO_HALTED           0
#define  USB_SET_RESET            1
#define  ASYNC_SCHEDULE_ENABLE    1
#define  ASYNC_SCHEDULE_DISABLE   0
#define  PERIOD_SCHEDULE_ENABLE   1
#define  PERIOD_SCHEDULE_DISABLE  0
#define  FRAME_LIST_DIV           4

/* USBSTS */
#define  CLEAR_ALL_STS            0xFFFFFFFF
#define  PERIODIC_STATUS_ENABLE   1
#define  PERIODIC_STATUS_DISABLE  0
#define  ASYNC_STATUS_ENABLE      1
#define  ASYNC_STATUS_DISABLE     0

/* USBINT */
#define  CLEAR_ALL_INT_STS        0xFFF3FF40
#define  PERIODIC_INT_ENABLE      1
#define  PERIODIC_INT_DISABLE     0
#define  ASYNC_INT_ENABLE         1
#define  ASYNC_INT_DISABLE        0
#define  PORT_CHANGE_ENABLE       1
#define  PORT_CHANGE_DISABLE      0
#define  USB_ERR_ENABLE           1
#define  USB_ERR_DISABLE          0
#define  USB_INT_ENABLE           1
#define  USB_INT_DISABLE          0
#define  ROLLOVER_INT_ENABLE      1
#define  ROLLOVER_INT_DISABLE     0
#define  ASYNC_ADVANCE_ENABLE     1
#define  ASYNC_ADVANCE_DISABLE    0

/* FRINDEX */
#define  MAX_UFRAME               8

/* PORTSC1 */
#define  PORT_POWER_ON            1
#define  PORT_POWER_OFF           0
#define  PORT_ENABLE              1
#define  PORT_DISABLE             0
#define  PORT_RESET               1
#define  PORT_NO_RESET            0
#define  PORT_DEV_CONNECT         1
#define  PORT_DEV_NO_CONNECT      0
#define  PORT_CONNECT_STS_CHANGE  1
#define  PORT_ENABLE_CHANGE       1
#define  PORT_OVERCUR_CHANGE      1
#define  PORT_FORCE_RESUME        1
#define  PORT_NOT_INDICATOR       0
#define  PORT_AMBER_INDICATOR     1
#define  PORT_GREEN_INDICATOR     2
#define  PTS_UTMI_UTMIP          0x0
#define  PTS_ULPI_DDR            0x1
#define  PTS_ULPI                0x2
#define  PTS_PHY1_IC             0x3
#define  PTS_HSIC_UTMI           0x0
#define  PTS2_OTHER              0x0
#define  PTS2_HSIC_UTMI          0x1

enum port_speed {
	PORT_FULL_SPEED,
	PORT_LOW_SPEED,
	PORT_HIGH_SPEED,
	PORT_NOT_CONNECTED
};

/* USBMODE */
#define  USBMODE_IDLE            0x0
#define  USBMODE_DEVICE          0x2
#define  USBMODE_HOST            0x3
#define  VBUS_POWER_ENABLE       1
#define  VBUS_POWER_DISABLE      0
#define  AUTO_LOWER_POWER_D      0x0
#define  SOFT_CONNECT_ENABLE     1

/* SBUSCFG */
#define  NO_BURST_ALIGN          0
#define  SINGLE_BURST            0

/* BURSTSIZE */
#define  BURST_SIZE_64           64

/* AHB port select */
#define AHB_PORT_SDRAM		0
#define AHB_PORT_SRAM		1

void csp_usb_deinit(void);
void csp_usb_reset_port(void);
u32 csp_usb_get_frame_number(void);
u32 csp_usb_get_frame_index(void);
u32 csp_usb_is_device_connected(void);
u32 csp_usb_get_device_speed(void);
void csp_usb_set_async_advance(void);
u32 csp_usb_get_int_sts(void);
void csp_usb_clear_usb_sts(u32 valid_sts);
u32 csp_usb_get_and_clear_int_status(void);
u32 csp_usb_get_portsc_value(void);
void csp_usb_clear_connect_status_change(void);
void csp_usb_clear_current_connect_status(void);
void csp_usb_clear_port_enable_change(void);
void csp_usb_clear_overcurrent_change(void);
void csp_usb_clear_force_port_resume(void);
void csp_usb_run_host(void);
void csp_usb_stop_host(void);
u32 csp_usb_get_halted_status(void);
void csp_usb_reset_host(void);
void csp_usb_set_host_mode(void);
void csp_usb_disable_all_interrupt(void);
void csp_usb_clear_all_status(void);
void csp_usb_set_interrutp(void);
void csp_usb_select_ahb_port(u8 value);
void csp_usb_set_async_list_addr(u32 addr);
void csp_usb_set_period_frame_addr(u32 addr);
void csp_usb_set_frame_list_size(u32 bits);
void csp_usb_set_port_power_on(void);
void csp_usb_set_port_power_off(void);
void csp_usb_set_amber_port_indicator(void);
void csp_usb_enable_period_schedule(void);
void csp_usb_disable_period_schedule(void);
void csp_usb_enable_async_schedule(void);
void csp_usb_disable_async_schedule(void);
void csp_usb_ulpi_transceiver(void);
void csp_usb_utmi_transceiver(void);
void csp_usb_host_soft_connect(void);
void csp_usb_bus_cfg(u32 tx_burst, u32 rx_burst);
void csp_usb_set_threshold(u32 thredhold);
void csp_usb_clear_siddq(void);
void csp_usb_set_siddq(void);

#endif /* __USBH_CSP_H__ */
