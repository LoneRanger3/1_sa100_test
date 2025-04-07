/*
 * usbh_csp.c - Uart driver for LomboTech Socs
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

#include <csp.h>
#include "usbh_csp.h"
#include "usb.h"

void csp_usb_deinit(void)
{
	reg_usb_usbsts_t usb_sts_reg;
	reg_usb_portsc1_t portsc1_reg;
	reg_usb_usbmode_t usb_mode_reg;

	/* clear all interrupt pending */
	usb_sts_reg.val = READREG32(VA_USB_USBSTS);
	WRITEREG32(VA_USB_USBSTS, usb_sts_reg.val);

	/* turn off the port power */
	portsc1_reg.val = READREG32(VA_USB_PORTSC1);
	portsc1_reg.bits.pp = PORT_POWER_OFF;
	WRITEREG32(VA_USB_PORTSC1, portsc1_reg.val);

	/* set idle mode */
	usb_mode_reg.val = READREG32(VA_USB_USBMODE);
	usb_mode_reg.bits.cm = USBMODE_IDLE;
	WRITEREG32(VA_USB_USBMODE, usb_mode_reg.val);
}

void csp_usb_reset_port(void)
{
	reg_usb_portsc1_t portsc1_reg;
	portsc1_reg.val = READREG32(VA_USB_PORTSC1);

	/* reset the port */
	portsc1_reg.val = READREG32(VA_USB_PORTSC1);
	portsc1_reg.bits.pr = PORT_RESET;
	WRITEREG32(VA_USB_PORTSC1, portsc1_reg.val);

	/* wait for completion */
	portsc1_reg.val = READREG32(VA_USB_PORTSC1);
	while (portsc1_reg.bits.pr == PORT_RESET)
		portsc1_reg.val = READREG32(VA_USB_PORTSC1);
}

u32 csp_usb_get_frame_number(void)
{
	reg_usb_frindex_t frame_index_reg;

	frame_index_reg.val = READREG32(VA_USB_FRINDEX);

	return frame_index_reg.bits.frindex;
}

u32 csp_usb_get_frame_index(void)
{
	reg_usb_frindex_t frame_index_reg;

	frame_index_reg.val = READREG32(VA_USB_FRINDEX);

	/* [0:2] uframe [3:N]: frame number */
	return frame_index_reg.bits.frindex / MAX_UFRAME;
}

u32 csp_usb_is_device_connected(void)
{
	reg_usb_portsc1_t portsc1_reg;

	portsc1_reg.val = READREG32(VA_USB_PORTSC1);
	if (portsc1_reg.bits.ccs == PORT_DEV_CONNECT)
		return 1;
	else
		return 0;
}

u32 csp_usb_get_device_speed(void)
{
	reg_usb_portsc1_t portsc1_reg;

	portsc1_reg.val = READREG32(VA_USB_PORTSC1);

	return portsc1_reg.bits.pspd;
}

void csp_usb_set_async_advance(void)
{
	reg_usb_usbcmd_t usb_cmd_reg;

	usb_cmd_reg.val = READREG32(VA_USB_USBCMD);
	usb_cmd_reg.bits.iaa = ASYNC_ADVANCE_ENABLE;

	WRITEREG32(VA_USB_USBCMD, usb_cmd_reg.val);
}

u32 csp_usb_get_int_sts(void)
{
	reg_usb_usbsts_t usb_sts_reg;
	reg_usb_usbintr_t usb_intr_reg;

	usb_sts_reg.val = READREG32(VA_USB_USBSTS);
	usb_intr_reg.val = READREG32(VA_USB_USBINTR);

	return usb_sts_reg.val & usb_intr_reg.val;
}

void csp_usb_clear_usb_sts(u32 valid_sts)
{
	reg_usb_usbsts_t usb_sts_reg;

	usb_sts_reg.val = valid_sts;

	WRITEREG32(VA_USB_USBSTS, usb_sts_reg.val);
}

u32 csp_usb_get_and_clear_int_status(void)
{
	reg_usb_usbsts_t usb_sts_reg;
	reg_usb_usbintr_t usb_intr_reg;
	u32 reg;

	usb_sts_reg.val = READREG32(VA_USB_USBSTS);
	usb_intr_reg.val = READREG32(VA_USB_USBINTR);
	reg = (usb_sts_reg.val & usb_intr_reg.val);
	WRITEREG32(VA_USB_USBSTS, reg);

	return reg;
}

u32 csp_usb_get_portsc_value(void)
{
	reg_usb_portsc1_t portsc1_reg;

	portsc1_reg.val = READREG32(VA_USB_PORTSC1);

	return portsc1_reg.val;
}

void csp_usb_clear_connect_status_change(void)
{
	reg_usb_portsc1_t portsc1_reg;

	portsc1_reg.val = READREG32(VA_USB_PORTSC1);
	portsc1_reg.bits.csc = PORT_CONNECT_STS_CHANGE;

	WRITEREG32(VA_USB_PORTSC1, portsc1_reg.val);
}

void csp_usb_clear_current_connect_status(void)
{
	reg_usb_portsc1_t portsc1_reg;

	portsc1_reg.val = READREG32(VA_USB_PORTSC1);
	portsc1_reg.bits.ccs = PORT_DEV_CONNECT;

	WRITEREG32(VA_USB_PORTSC1, portsc1_reg.val);
}

void csp_usb_clear_port_enable_change(void)
{
	reg_usb_portsc1_t portsc1_reg;

	portsc1_reg.val = READREG32(VA_USB_PORTSC1);
	portsc1_reg.bits.pec = PORT_ENABLE_CHANGE;

	WRITEREG32(VA_USB_PORTSC1, portsc1_reg.val);
}

void csp_usb_clear_overcurrent_change(void)
{
	reg_usb_portsc1_t portsc1_reg;

	portsc1_reg.val = READREG32(VA_USB_PORTSC1);
	portsc1_reg.bits.occ = PORT_OVERCUR_CHANGE;

	WRITEREG32(VA_USB_PORTSC1, portsc1_reg.val);
}

void csp_usb_clear_force_port_resume(void)
{
	reg_usb_portsc1_t portsc1_reg;

	portsc1_reg.val = READREG32(VA_USB_PORTSC1);
	portsc1_reg.bits.fpr = PORT_FORCE_RESUME;

	WRITEREG32(VA_USB_PORTSC1, portsc1_reg.val);
}

void csp_usb_run_host(void)
{
	reg_usb_usbcmd_t usb_cmd_reg;
	reg_usb_usbsts_t usb_sts_reg;

	usb_cmd_reg.val = READREG32(VA_USB_USBCMD);
	usb_cmd_reg.bits.rs = USB_RUN;
	WRITEREG32(VA_USB_USBCMD, usb_cmd_reg.val);

	usb_sts_reg.val = READREG32(VA_USB_USBSTS);
	while (usb_sts_reg.bits.hch == HOST_HALTED)
		usb_sts_reg.val = READREG32(VA_USB_USBSTS);
}

void csp_usb_stop_host(void)
{
	reg_usb_usbcmd_t usb_cmd_reg;
	reg_usb_usbsts_t usb_sts_reg;

	usb_cmd_reg.val = READREG32(VA_USB_USBCMD);
	usb_cmd_reg.bits.rs = USB_STOP;
	WRITEREG32(VA_USB_USBCMD, usb_cmd_reg.val);

	usb_sts_reg.val = READREG32(VA_USB_USBSTS);
	while (usb_sts_reg.bits.hch == HOST_NO_HALTED)
		usb_sts_reg.val = READREG32(VA_USB_USBSTS);
}

u32 csp_usb_get_halted_status(void)
{
	reg_usb_usbsts_t usb_sts_reg;

	usb_sts_reg.val = READREG32(VA_USB_USBSTS);

	return usb_sts_reg.bits.hch;
}

void csp_usb_reset_host(void)
{
	reg_usb_usbcmd_t usb_cmd_reg;

	usb_cmd_reg.val = READREG32(VA_USB_USBCMD);
	usb_cmd_reg.bits.rst = USB_SET_RESET;
	WRITEREG32(VA_USB_USBCMD, usb_cmd_reg.val);

	/* Wait for completion */
	usb_cmd_reg.val = READREG32(VA_USB_USBCMD);
	while (usb_cmd_reg.bits.rst == USB_SET_RESET)
		usb_cmd_reg.val = READREG32(VA_USB_USBCMD);
}

void csp_usb_set_host_mode(void)
{
	reg_usb_usbmode_t usb_mode_reg;

	usb_mode_reg.val = READREG32(VA_USB_USBMODE);
	usb_mode_reg.bits.cm = USBMODE_HOST;
	usb_mode_reg.bits.vbps = VBUS_POWER_ENABLE;
	usb_mode_reg.bits.alp = AUTO_LOWER_POWER_D;
	WRITEREG32(VA_USB_USBMODE, usb_mode_reg.val);
}

void csp_usb_disable_all_interrupt(void)
{
	WRITEREG32(VA_USB_USBINTR, 0);
}

void csp_usb_clear_all_status(void)
{
	reg_usb_usbsts_t usb_sts_reg;

	usb_sts_reg.val = CLEAR_ALL_INT_STS;
	WRITEREG32(VA_USB_USBSTS, usb_sts_reg.val);
}

void csp_usb_set_interrutp(void)
{
	reg_usb_usbintr_t usb_intr_reg;

	/* enable interrupt:async/periodic/port change/usb err/async advance */
	usb_intr_reg.val = READREG32(VA_USB_USBINTR);
	usb_intr_reg.bits.uaie = ASYNC_INT_ENABLE;
	usb_intr_reg.bits.upie = PERIODIC_INT_ENABLE;
	usb_intr_reg.bits.pce  = PORT_CHANGE_ENABLE;
	usb_intr_reg.bits.uee  = USB_ERR_ENABLE;
	usb_intr_reg.bits.aae  = ASYNC_ADVANCE_ENABLE;
	usb_intr_reg.bits.see  = 1;
	usb_intr_reg.bits.ue  = USB_INT_ENABLE;
	WRITEREG32(VA_USB_USBINTR, usb_intr_reg.val);
}

/* set AHB port to sdram or sram */
void csp_usb_select_ahb_port(u8 value)
{
	reg_usb_usbctrl_t usb_ctrl_reg;

	usb_ctrl_reg.val = READREG32(VA_USB_USBCTRL);
	usb_ctrl_reg.bits.mhport_sel = value;
	WRITEREG32(VA_USB_USBCTRL, usb_ctrl_reg.val);
}

void csp_usb_set_async_list_addr(u32 addr)
{
	reg_usb_asynclistaddr_t async_list_addr_reg;

	async_list_addr_reg.val = addr;
	WRITEREG32(VA_USB_ASYNCLISTADDR, async_list_addr_reg.val);
}

void csp_usb_set_period_frame_addr(u32 addr)
{
	reg_usb_periodiclistbase_t period_frame_base_reg;
	period_frame_base_reg.val = addr;
	WRITEREG32(VA_USB_PERIODICLISTBASE, period_frame_base_reg.val);
}

void csp_usb_set_frame_list_size(u32 bits)
{
	reg_usb_usbcmd_t usb_cmd_reg;

	/* frame list size determined by bit 15/3/2 */
	usb_cmd_reg.val = READREG32(VA_USB_USBCMD);
	usb_cmd_reg.bits.fs2 = bits % FRAME_LIST_DIV;
	usb_cmd_reg.bits.fs1_0 = bits / FRAME_LIST_DIV;
	WRITEREG32(VA_USB_USBCMD, usb_cmd_reg.val);
}

void csp_usb_set_port_power_on(void)
{
	reg_usb_portsc1_t portsc1_reg;

	portsc1_reg.val = READREG32(VA_USB_PORTSC1);
	portsc1_reg.bits.pp = PORT_POWER_ON;
	WRITEREG32(VA_USB_PORTSC1, portsc1_reg.val);
}

void csp_usb_set_port_power_off(void)
{
	reg_usb_portsc1_t portsc1_reg;

	portsc1_reg.val = READREG32(VA_USB_PORTSC1);
	portsc1_reg.bits.pp = PORT_POWER_OFF;
	WRITEREG32(VA_USB_PORTSC1, portsc1_reg.val);
}

void csp_usb_set_amber_port_indicator(void)
{
	reg_usb_portsc1_t portsc1_reg;

	portsc1_reg.val = READREG32(VA_USB_PORTSC1);
	portsc1_reg.bits.pic = PORT_AMBER_INDICATOR;
	WRITEREG32(VA_USB_PORTSC1, portsc1_reg.val);
}

void csp_usb_enable_period_schedule(void)
{
	reg_usb_usbcmd_t usb_cmd_reg;
	reg_usb_usbsts_t usb_sts_reg;

	usb_sts_reg.val = READREG32(VA_USB_USBSTS);
	if (usb_sts_reg.bits.ps == PERIODIC_STATUS_DISABLE) {
		/* enable schedule */
		usb_cmd_reg.val = READREG32(VA_USB_USBCMD);
		usb_cmd_reg.bits.pse = PERIOD_SCHEDULE_ENABLE;
		WRITEREG32(VA_USB_USBCMD, usb_cmd_reg.val);
		/* wait for starting */
		usb_sts_reg.val = READREG32(VA_USB_USBSTS);
		while (usb_sts_reg.bits.ps == PERIODIC_STATUS_DISABLE)
			usb_sts_reg.val = READREG32(VA_USB_USBSTS);
	}
}

void csp_usb_disable_period_schedule(void)
{
	reg_usb_usbcmd_t usb_cmd_reg;
	reg_usb_usbsts_t usb_sts_reg;

	usb_sts_reg.val = READREG32(VA_USB_USBSTS);
	if (usb_sts_reg.bits.ps == PERIODIC_STATUS_ENABLE) {
		/* disable schedule */
		usb_cmd_reg.val = READREG32(VA_USB_USBCMD);
		usb_cmd_reg.bits.pse = PERIOD_SCHEDULE_DISABLE;
		WRITEREG32(VA_USB_USBCMD, usb_cmd_reg.val);
		/* wait for stopping */
		usb_sts_reg.val = READREG32(VA_USB_USBSTS);
		while (usb_sts_reg.bits.ps == PERIODIC_STATUS_ENABLE)
			usb_sts_reg.val = READREG32(VA_USB_USBSTS);
	}
}

void csp_usb_enable_async_schedule(void)
{
	reg_usb_usbcmd_t usb_cmd_reg;
	reg_usb_usbsts_t usb_sts_reg;

	usb_sts_reg.val = READREG32(VA_USB_USBSTS);
	if (usb_sts_reg.bits.as == ASYNC_STATUS_DISABLE) {
		/* Enable schedule */
		usb_cmd_reg.val = READREG32(VA_USB_USBCMD);
		usb_cmd_reg.bits.ase = ASYNC_SCHEDULE_ENABLE;
		WRITEREG32(VA_USB_USBCMD, usb_cmd_reg.val);
		/* Modifications to the Asynchronous Schedule Enable bit are
		not necessarily immediate. Rather the new value of the bit will
		only be taken into consideration the next time the host controller
		needs to use the value of the ASYNCLISTADDR register to get the
		next queue head, poll the Asynchronous Schedule Status bit to
		determine when the asynchronous schedule has made the desired
		transition */
		usb_sts_reg.val = READREG32(VA_USB_USBSTS);
		while (usb_sts_reg.bits.as == ASYNC_STATUS_DISABLE)
			usb_sts_reg.val = READREG32(VA_USB_USBSTS);
	}
}

void csp_usb_disable_async_schedule(void)
{
	reg_usb_usbcmd_t usb_cmd_reg;
	reg_usb_usbsts_t usb_sts_reg;

	usb_sts_reg.val = READREG32(VA_USB_USBSTS);
	if (usb_sts_reg.bits.as == ASYNC_STATUS_ENABLE) {
		/* Disable schedule */
		usb_cmd_reg.val = READREG32(VA_USB_USBCMD);
		usb_cmd_reg.bits.ase = ASYNC_SCHEDULE_DISABLE;
		WRITEREG32(VA_USB_USBCMD, usb_cmd_reg.val);
		/* Modifications to the Asynchronous Schedule Enable bit are
		not necessarily immediate. Rather the new value of the bit will
		only be taken into consideration the next time the host controller
		needs to use the value of the ASYNCLISTADDR register to get the
		next queue head, poll the Asynchronous Schedule Status bit to
		determine when the asynchronous schedule has made the desired transition*/
		usb_sts_reg.val = READREG32(VA_USB_USBSTS);
		while (usb_sts_reg.bits.as == ASYNC_STATUS_ENABLE)
			usb_sts_reg.val = READREG32(VA_USB_USBSTS);
	}
}

void csp_usb_ulpi_transceiver(void)
{
	reg_usb_portsc1_t portsc1_reg;

	portsc1_reg.val = READREG32(VA_USB_PORTSC1);
	portsc1_reg.bits.pts2 = PTS2_OTHER;
	portsc1_reg.bits.pts = PTS_ULPI;
	WRITEREG32(VA_USB_PORTSC1, portsc1_reg.val);
}

void csp_usb_utmi_transceiver(void)
{
	reg_usb_portsc1_t portsc1_reg;

	portsc1_reg.val = READREG32(VA_USB_PORTSC1);
	portsc1_reg.bits.pts2 = PTS2_OTHER;
	portsc1_reg.bits.pts = PTS_UTMI_UTMIP;
	WRITEREG32(VA_USB_PORTSC1, portsc1_reg.val);
}

void csp_usb_host_soft_connect(void)
{
	reg_usb_usbmode_t usb_mode_reg;

	usb_mode_reg.val = READREG32(VA_USB_USBMODE);
	usb_mode_reg.bits.sf_con = SOFT_CONNECT_ENABLE;
	WRITEREG32(VA_USB_USBMODE, usb_mode_reg.val);
}

void csp_usb_bus_cfg(u32 tx_burst, u32 rx_burst)
{
	reg_usb_sbuscfg_t sbus_cfg_reg;
	reg_usb_burstsize_t burst_size_reg;

	sbus_cfg_reg.val = READREG32(VA_USB_SBUSCFG);
	burst_size_reg.val = READREG32(VA_USB_BURSTSIZE);

	/* config amba burst alignment for writing as no burst align */
	sbus_cfg_reg.bits.bawr = NO_BURST_ALIGN;
	/* amba burst alignment for reading */
	/* amba burst cfg as similar to unspecified length, always single burst */
	sbus_cfg_reg.bits.ahbbrst = SINGLE_BURST;
	WRITEREG32(VA_USB_SBUSCFG, sbus_cfg_reg.val);

	/* TX BURST set */
	burst_size_reg.bits.txpburst = tx_burst;
	/* RX BURST set */
	burst_size_reg.bits.txpburst = rx_burst;
	WRITEREG32(VA_USB_BURSTSIZE, burst_size_reg.val);

	/* Don't configure RX BURST */
}

void csp_usb_set_threshold(u32 thredhold)
{
	reg_usb_usbcmd_t usb_cmd_reg;

	/* set the interrupt threshold control interval*/
	usb_cmd_reg.val = READREG32(VA_USB_USBCMD);
	usb_cmd_reg.bits.itc = thredhold;
	WRITEREG32(VA_USB_USBCMD, usb_cmd_reg.val);
}

void csp_usb_clear_siddq(void)
{
	reg_usb_phycfg_t usb_phy_reg;

	usb_phy_reg.val = READREG32(VA_USB_PHYCFG);
	usb_phy_reg.bits.siddq = 0;
	WRITEREG32(VA_USB_PHYCFG, usb_phy_reg.val);
}

void csp_usb_set_siddq(void)
{
	reg_usb_phycfg_t usb_phy_reg;

	usb_phy_reg.val = READREG32(VA_USB_PHYCFG);
	usb_phy_reg.bits.siddq = 1;
	WRITEREG32(VA_USB_PHYCFG, usb_phy_reg.val);
}
