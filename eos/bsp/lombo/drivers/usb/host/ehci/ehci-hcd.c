/*
 * ehci.c - USB host driver for LomboTech Socs
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

/* Include debug macro */
#include "ehci.h"

#include "board.h"
#include "cache_op.h"
#include "gpio/pinctrl.h"

#include "usbh_csp.h"
#include "drivers/usb_common.h"

#include "ehci-dbg.c"
#include "ehci-mem.c"
#include "ehci-q.c"
#include "ehci-sched.c"

static struct ehci_hcd ehci_hcd_drv;

static inline struct ehci_hcd *get_ehci_hcd(void)
{
	return &ehci_hcd_drv;
}

void hcd_port_reset(int port_num)
{
	csp_usb_reset_port();
}

u8 hcd_get_device_speed(void)
{
	u32 speed;

	speed = csp_usb_get_device_speed();
	switch (speed) {
	case 0:
		return USB_SPEED_FULL;
	case 1:
		return USB_SPEED_LOW;
	case 2:
		return USB_SPEED_HIGH;
	default:
		EHCI_LOG_I("unknown speed");
		return USB_SPEED_HIGH;
	}
}

rt_err_t hcd_open_pipe(upipe_t pipe)
{
	struct ehci_hcd *ehci = get_ehci_hcd();
	struct ehci_qh *qh;

	/* Make a qh for this pipe. Qh won't be recycled until pipe is closed */
	qh = qh_make(ehci, pipe);
	if (!qh)
		return -RT_ERROR;

	switch (usb_pipetype(pipe->info)) {
	case USB_EP_ATTR_CONTROL:
	case USB_EP_ATTR_BULK:
		qh_link_async(ehci, qh);
		break;
	case USB_EP_ATTR_INT:
		qh_link_periodic(ehci, qh);
		break;
	case USB_EP_ATTR_ISOC:
		EHCI_LOG_E("not support ios xfer now");
		return -RT_ERROR;
	default:
		return -RT_ERROR;
	}

	return RT_EOK;
}

rt_err_t hcd_close_pipe(upipe_t pipe)
{
	struct ehci_hcd *ehci = get_ehci_hcd();
	struct ehci_qh *qh = (struct ehci_qh *)pipe->hcpriv;

	switch (usb_pipetype(pipe->info)) {
	case USB_EP_ATTR_CONTROL:
	case USB_EP_ATTR_BULK:
		start_unlink_async(ehci, qh);
		break;
	case USB_EP_ATTR_INT:
		start_unlink_intr(ehci, qh);
		break;
	case USB_EP_ATTR_ISOC:
		EHCI_LOG_E("not support ios xfer now");
		return -RT_ERROR;
	default:
		return -RT_ERROR;
	}

	return RT_EOK;
}

int hcd_control_xfer(upipe_t pipe, void *buffer, int length)
{
	struct ehci_hcd *ehci = get_ehci_hcd();
	struct list_head qtd_list;
	u32 buf_phys = virt_to_phys((u32)buffer);

	INIT_LIST_HEAD(&qtd_list);

	/*
	 * "flush" to ensure the data has been written to the memory,
	 * not in dcache.
	 * "invalidate" incase the cpu access buf after transfer done
	 */
	if (length != 0)
		rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH | RT_HW_CACHE_INVALIDATE,
					buffer, length);

	if (!qh_transaction(ehci, pipe, buf_phys, length, 1, &qtd_list))
		return 0;

	return async_submit(ehci, pipe, &qtd_list, length);
}

int hcd_data_xfer(upipe_t pipe, void *buffer, int length)
{
	struct ehci_hcd *ehci = get_ehci_hcd();
	struct list_head qtd_list;
	rt_bool_t write = usb_pipeout(pipe->info) ? RT_TRUE : RT_FALSE;
	u32 buf_phys = virt_to_phys((u32)buffer);
	int count = 0;

	INIT_LIST_HEAD(&qtd_list);

	if (length != 0) {
		if (write)
			/*
			 * "flush" to ensure the data has been written to the memory,
			 * not in dcache.
			 * "invalidate" incase the cpu access buf after transfer done
			 */
			rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH | RT_HW_CACHE_INVALIDATE,
					buffer, length);
		else
			/*
			 * the buf may be "dirty" by user, so clean it to
			 * let the following invalidate operation(after data
			 * transfer over) effective.
			 * the buf can NOT be accessed by cpu, before dma transfer done.
			 * otherwise the invalidate operation will be incorrect
			 *
			 * See <DEN0013D_cortex_a_series_PG 4.0.pdf> Page 8-17:
			 * If the cache contains dirty data, it is generally incorrect to
			 * invalidate it.
			 */
			rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, buffer, length);
	}

	switch (usb_pipetype(pipe->info)) {
	case USB_EP_ATTR_CONTROL:
	case USB_EP_ATTR_BULK:
		if (!qh_transaction(ehci, pipe, buf_phys, length, 0, &qtd_list))
			return 0;
		count = async_submit(ehci, pipe, &qtd_list, length);
		break;
	case USB_EP_ATTR_INT:
		if (!qh_transaction(ehci, pipe, buf_phys, length, 0, &qtd_list))
			return 0;
		intr_submit(ehci, pipe, &qtd_list, length);
		break;
	case USB_EP_ATTR_ISOC:
		EHCI_LOG_E("not support ios xfer now");
		return 0;
	default:
		return 0;
	}

	/* Invalidate the cache, let cpu read from memory */
	if (!write && length != 0) {
		rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, buffer, length);
		EHCI_LOG_BUF("xfer read:", buffer, length);
	}

	return count;
}

/*
 * Interrupt On Completion has occurred, however have no clues on which QueueHead
 * happened. Also IOC TD may be advanced already, So free all TD which is not active
 */
static void async_schedule_isr(struct ehci_hcd *ehci)
{
	EHCI_LOG_I("asynchronous interrupt");

	scan_async(ehci);
}

static void period_schedule_isr(struct ehci_hcd *ehci)
{
	EHCI_LOG_I("period sched interrupt");

	if (ehci->intr_count > 0)
		scan_intr(ehci);
}

static void ehci_host_reset(void)
{
	/* stop host */
	if (csp_usb_get_halted_status())
		csp_usb_stop_host();

	/* reset host */
	csp_usb_reset_host();

	/* Program the controller to be the USB host controller,
	this can only be done after Reset */
	csp_usb_set_host_mode();
}

static void usb_error_isr(void)
{
	EHCI_LOG_I("usb error interrupt");
}

static void port_status_change_isr(struct ehci_hcd *hcd)
{
	u32 is_connected, speed;

	EHCI_LOG_I("port status change isr");

	/* Is device Attached */
	is_connected = csp_usb_is_device_connected();
	if (is_connected) {
		EHCI_LOG_I("device attached");
		speed = csp_usb_get_device_speed();
		if (speed == PORT_LOW_SPEED)
			rt_usbh_root_hub_connect_handler(hcd->uhcd, 1, RT_FALSE);
		else
			rt_usbh_root_hub_connect_handler(hcd->uhcd, 1, RT_TRUE);
	} else {
		EHCI_LOG_I("device not attached");
		rt_usbh_root_hub_disconnect_handler(hcd->uhcd, 1);
	}
}

static void async_advance_isr(struct ehci_hcd *ehci)
{
	struct ehci_qh *qh, *tmp;

	EHCI_LOG_I("Async Advance interrupt");

	if (list_empty(&ehci->async_unlink))
		return;

	list_for_each_entry_safe(qh, tmp, &ehci->async_unlink, unlink_node) {
		/* qtds in qh hasn't been releaseed by qh_completions
		When transfer errors happened */
		if (!list_empty(&qh->qtd_list))
			free_qtd_list(qh);
		ehci_qh_free(qh);
		list_del(&qh->unlink_node);
		disable_async(ehci);
	}
}

static void hcd_irq_handler(int vector, void *arg)
{
	u32 intr, port_sc;
	struct ehci_hcd *ehci = (struct ehci_hcd *)arg;

	RT_ASSERT(ehci != RT_NULL);

	/* Get and acknowledge Interrrupt */
	intr = csp_usb_get_and_clear_int_status();
	if (intr == 0)
		return;

	/* Port change detect */
	if (intr & EHC_USBSTS_PCD) {
		port_sc = csp_usb_get_portsc_value();
		EHCI_LOG_I("port change detect:0x%x", port_sc);

		/* Connected status change */
		if (port_sc & EHC_PORTSC_CSC) {
			/* Check connection */
			port_status_change_isr(ehci);
			csp_usb_clear_connect_status_change();
		} else if (port_sc & EHC_PORTSC_CCS) {
			EHCI_LOG_I("CCS interrupt");
		}

		/* Port enabled change */
		if (port_sc & EHC_PORTSC_PEC) {
			EHCI_LOG_I("port enabled change");
			csp_usb_clear_port_enable_change();
		}
		/* Over-current change */
		if (port_sc & EHC_PORTSC_OCC) {
			EHCI_LOG_I("over-current change");
			csp_usb_clear_overcurrent_change();
		}
		/* Force port resume */
		if (port_sc & EHC_PORTSC_FPR) {
			EHCI_LOG_I("Force port resume");
			csp_usb_clear_force_port_resume();
		}
	}

	/* USB host asynchronous interrupt */
	if (intr & EHC_USBSTS_UAI)
		async_schedule_isr(ehci);

	/* USB host periodic interrupt */
	if (intr & EHC_USBSTS_UPI)
		period_schedule_isr(ehci);

	/* USB error interrupt */
	if (intr & EHC_USBSTS_UEI)
		usb_error_isr();

	/* Interrupt on Async Advance */
	if (intr & EHC_USBSTS_IAA)
		async_advance_isr(ehci);
}

static void hcd_enable_irq(void)
{
	rt_hw_interrupt_umask(USBH_IRQ_NUM);
}

static void hcd_disable_irq(void)
{
	rt_hw_interrupt_mask(USBH_IRQ_NUM);
}

static rt_err_t hcd_clock_init(struct ehci_hcd *hcd)
{
	u32 result, p1, p2;
	rt_err_t ret;

	result = lombo_func2(&p1, &p2);
	if (result) {
		EHCI_LOG_E("get func2 failed");
		p1 = 1;
	}

	/* 1.1 get reset and gate clock  */
	hcd->phy_reset = clk_get(CLK_NAME_USB_PHY_RESET);
	hcd->usb_gate = clk_get(CLK_NAME_AHB_USB_GATE);
	hcd->usb_reset = clk_get(CLK_NAME_AHB_USB_RESET);
	if ((hcd->usb_gate < 0) || (hcd->usb_reset < 0) || (hcd->phy_reset < 0)) {
		EHCI_LOG_E("get usb gate/reset clk failed");
		ret = -RT_EINVAL;
		goto get_clk_failed;
	}

	/* 1.2 reset usb phy and usb, usb clock off */
	clk_disable(hcd->phy_reset);

	/* wait phy pll drop totally */
	if (p1 == 2)
		rt_thread_mdelay(5);

	clk_disable(hcd->usb_reset);
	clk_disable(hcd->usb_gate);

	/* 1.3 usb clock on */
	ret = clk_enable(hcd->usb_gate);
	if (ret) {
		EHCI_LOG_E("enable usb gate failed");
		goto enable_usb_gate_failed;
	}

	/* 1.4 release phy reset */
	ret = clk_enable(hcd->phy_reset);
	if (ret) {
		EHCI_LOG_E("enable usb phy reset failed");
		goto enable_phy_reset_failed;
	}

	/* Clear siddq before using phy */
	if (p1 == 2)
		csp_usb_clear_siddq();

	/* Delay for phy clk output stablely */
	rt_thread_mdelay(5);

	/* 1.5 release usb reset */
	ret = clk_enable(hcd->usb_reset);
	if (ret) {
		EHCI_LOG_E("enable usb reset failed");
		goto enable_usb_reset_failed;
	}

	return RT_EOK;

enable_usb_reset_failed:
	clk_disable(hcd->phy_reset);

enable_phy_reset_failed:
	clk_disable(hcd->usb_gate);

enable_usb_gate_failed:
get_clk_failed:
	if (hcd->phy_reset >= 0)
		clk_put(hcd->phy_reset);
	if (hcd->usb_gate >= 0)
		clk_put(hcd->usb_gate);
	if (hcd->usb_reset >= 0)
		clk_put(hcd->usb_reset);
	EHCI_LOG_E("set clk fail.");
	return ret;
}

static void hcd_clock_deinit(struct ehci_hcd *hcd)
{
	clk_disable(hcd->usb_reset);
	clk_put(hcd->phy_reset);
	clk_disable(hcd->usb_gate);
	clk_put(hcd->usb_gate);
	clk_disable(hcd->phy_reset);
	clk_put(hcd->phy_reset);
}

rt_err_t hcd_hardware_init(struct ehci_hcd *ehci)
{
	u32 result, p1, p2;
	rt_err_t ret;

	result = lombo_func2(&p1, &p2);
	if (result) {
		EHCI_LOG_E("get func2 failed");
		p1 = 1;
	}

	/* shutdown phy analogy */
	if (p1 == 2)
		csp_usb_set_siddq();

	/* Init clock */
	ret = hcd_clock_init(ehci);
	if (ret) {
		EHCI_LOG_E("hcd clock init failed");
		return ret;
	}

	/* Usb master access sdram master */
	csp_usb_select_ahb_port(AHB_PORT_SDRAM);

	/* burst size */
	csp_usb_bus_cfg(0x4, 0x4);

	/* Select utmi phy on ic environment */
	csp_usb_utmi_transceiver();

	/* Clear siddq before using phy */
	if (p1 == 1)
		csp_usb_clear_siddq();

	ehci_host_reset();

	/* Interrupts init: enable necessary interrupt source: Async Advance,
	System error, Port Change, USB error, USB Int */
	csp_usb_disable_all_interrupt();
	csp_usb_clear_all_status();
	csp_usb_set_interrutp();

	return RT_EOK;
}

void hcd_hardware_deinit(struct ehci_hcd *ehci)
{
	csp_usb_deinit();
	hcd_clock_deinit(ehci);
}

rt_err_t hcd_init(uhcd_t uhcd)
{
	struct ehci_hcd *ehci = get_ehci_hcd();
	struct ehci_qh_hw *hw;
	rt_err_t ret;

	/* save core hcd */
	ehci->uhcd = uhcd;

	ret = hcd_hardware_init(ehci);
	if (ret)
		return ret;

	/*
	 * hw default: 1K periodic list heads, one per frame.
	 * periodic_size can shrink by USBCMD update if hcc_params allows.
	 */
	ehci->periodic_size = FRAME_LIST_SIZE;
	INIT_LIST_HEAD(&ehci->intr_qh_list);

	ret = hcd_mem_init(ehci);
	if (ret) {
		EHCI_LOG_E("hcd mem init failed");
		goto hw_deinit;
	}

	/* enable power IC */
	ehci->pctrl = pinctrl_get("usb");
	if (!ehci->pctrl) {
		EHCI_LOG_E("pinctrl get failed");
		goto free_mem;
	}
#ifdef ARCH_LOMBO_N7V1_EVB
	ret = pinctrl_enable_group(ehci->pctrl, "host-pwric");
#else
	ret = pinctrl_enable_group(ehci->pctrl, "host-pwric-ddr2");
#endif
	if (ret) {
		EHCI_LOG_E("enable power ic failed");
		goto free_pinctrl;
	}
	/*
	 * dedicate a qh for the async ring head, since we couldn't unlink
	 * a 'real' qh without stopping the async schedule [4.8].  use it
	 * as the 'reclamation list head' too.
	 * its dummy is used in hw_alt_next of many tds, to prevent the qh
	 * from automatically advancing to the next td after short reads.
	 */
	ehci->async->qh_next.qh = RT_NULL;
	hw = ehci->async->hw;
	hw->hw_next = QH_NEXT(ehci->async->qh_dma);
	hw->hw_info1 = QH_HEAD;
	hw->hw_token = QTD_STS_HALT;
	hw->hw_qtd_next = EHCI_LIST_END;
	ehci->async->qh_state = QH_STATE_LINKED;
	hw->hw_alt_next = EHCI_LIST_END;
	INIT_LIST_HEAD(&ehci->async_unlink);
	csp_usb_set_async_list_addr(ehci->async->qh_dma);
	EHCI_LOG_ASYN("hcd async qh:%x, dma:%x", ehci->async, ehci->async->qh_dma);

	/* periodic table */
	csp_usb_set_period_frame_addr(ehci->periodic_dma);
	csp_usb_set_frame_list_size(FRAME_LIST_SIZE_BITS);

	/* Power on the port */
	csp_usb_set_port_power_on();
	csp_usb_set_amber_port_indicator();

	/* Run the host */
	csp_usb_run_host();

	/* Install and enalbe usbh irq */
	rt_hw_interrupt_install(USBH_IRQ_NUM, hcd_irq_handler, ehci, "usbh");
	hcd_enable_irq();

	/* Available for port connect event */
	csp_usb_host_soft_connect();

	EHCI_LOG_I("hcd init OK");

	return RT_EOK;

free_pinctrl:
	pinctrl_put(ehci->pctrl);
free_mem:
	hcd_mem_free(ehci);
hw_deinit:
	hcd_hardware_deinit(ehci);

	return -RT_ERROR;
}

rt_err_t hcd_deinit(void)
{
	struct ehci_hcd *ehci = get_ehci_hcd();

	EHCI_LOG_I("hcd deinit");

	hcd_disable_irq();
	hcd_hardware_deinit(ehci);
	hcd_mem_free(ehci);
	pinctrl_put(ehci->pctrl);

	return RT_EOK;
}

