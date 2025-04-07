/*
 * ee_usbd.c - usb device controller driver code for LomboTech
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

#include <rtthread.h>
#include <rthw.h>

#include "udc.h"
#include "ee_usbd.h"
#include "csp/usbd_const.h"
#include "csp/usbd_csp.h"

#define DBG_SECTION_NAME	"usbd"
#define DBG_LEVEL		DBG_INFO
#include <debug.h>

#include "usbd_mem.c"
#include <spinlock.h>

DEFINE_SPINLOCK(usbd_lock);

void ee_dump_dt(struct hw_td *td)
{
	rt_kprintf("[dt:0x%x]\n next:0x%x\n trans_err:%d\n buffer_err:%d\n",
		td, td->next_td, td->trans_err, td->buffer_err);
	rt_kprintf(" halted:0x%x\n active:%d\n mult:%d\n ioc:%d\n",
		td->halted, td->active, td->mult_override, td->ioc);
	rt_kprintf(" total_bytes:0x%x\n page[0]:0x%x\n",
		td->total_bytes, td->page[0]);
	rt_kprintf(" page[1]:0x%x\n page[2]:0x%x\n page[3]:0x%x\n page[4]:0x%x\n",
		td->page[1], td->page[2], td->page[3], td->page[4]);
}

void ee_dump_qh(struct hw_qh *qh)
{
	rt_kprintf("[qh:0x%x]\n ios:%d\n max:%d\n zero:%d\n mult:%d\n cur:0x%x\n",
		qh, qh->int_on_setup, qh->max_packet_size, qh->zero_len_termination,
		qh->mult, qh->current_td);
	rt_kprintf("[qh->dt]\n next:0x%x\n halt:%d\n active:%d\n",
		qh->td.next_td, qh->td.halted, qh->td.active);
}

static void ee_enable_usb_irq(void)
{
	rt_hw_interrupt_umask(UDC_IRQ_NUM);
}

void ee_disable_usb_irq(void)
{
	rt_hw_interrupt_mask(UDC_IRQ_NUM);
}

void ee_set_address(struct lombo_udc *ee, u8 address)
{
	ee->setaddr = RT_TRUE;
	ee->address = address;
}

void ee_enable_endpoint(struct lombo_udc *ee, u8 ep_num, u8 type,
					u8 dir, u16 max_size)
{
	struct hw_qh *qh;
	u8 ep_phy_num = 2 * ep_num + (dir == USB_DIR_OUT ? 0 : 1);
	struct ee_ep *ep = &ee->ep[ep_phy_num];

	LOG_D("enable ep-%d, dir:%s, type:%d, max size:%d",
		ep_num, dir == USB_DIR_OUT ? "out" : "in", type, max_size);

	/* Initialize queue head */
	qh = ee->ep[ep_phy_num].qh.ptr;
	memset(qh, 0, sizeof(struct hw_qh));
	/* Whether out data equal ep max packet size trigger interrupt */
	qh->int_on_setup = 1;
	qh->max_packet_size = max_size & 0x3ff;
	qh->zero_len_termination = 1;
	qh->td.next_td = LINK_TERMINATE;
	qh->td.active = 0;

	/* Initialize ep */
	ep->type = type;
	ep->num = ep_num;
	ep->dir = dir;
	ep->active = 1;

	csp_usb_config_endpoint(ep_num, type, dir);
}

void ee_disable_endpoint(struct lombo_udc *ee, u8 ep_num, u8 dir)
{
	u8 ep_phy_num = 2 * ep_num + (dir == USB_DIR_OUT ? 0 : 1);
	struct ee_ep *ep = &ee->ep[ep_phy_num];

	ep->active = 0;
}

/* set endpoint stall */
void ee_set_endpoint_stall(u8 ep_num)
{
	csp_usb_set_stall(ep_num);
}

/* clear endpoint stall */
void ee_clear_endpoint_stall(u8 ep_num)
{
	csp_usb_clear_stall(ep_num);
	csp_usb_reset_toggle(ep_num);
}

static void isr_setup_status_complete(struct lombo_udc *ee)
{
	if (ee->setaddr) {
		csp_usb_set_device_add(ee->address);
		ee->setaddr = RT_FALSE;
	}
}

u32 ee_prepare_data_transfer(struct lombo_udc *ee, u8 ep_phy_num,
					u8 *buffer, u32 size)
{
	u32 count, tmp_size = size;
	struct td_node *node, *lastnode, *firstnode;
	struct hw_qh *qh = (struct hw_qh *)(ee->ep[ep_phy_num].qh.phy_addr);
	u8 *phy_buf = (u8 *)virt_to_phys((u32)buffer);
	rt_bool_t write = USB_EP_DIR(ee->ep[ep_phy_num].dir) ? RT_TRUE : RT_FALSE;

	LOG_D("data transfer: ep-%d, vir-buf:0x%x, phy-buf:0x%x, size:0x%x",
				ep_phy_num, buffer, phy_buf, size);

	if (size != 0) {
		if (write)
			/*
			 * "flush" to ensure the data has been written to the memory,
			 * not in dcache.
			 * "invalidate" incase the cpu access buf after transfer done
			 */
			rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH | RT_HW_CACHE_INVALIDATE,
					buffer, size);
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
			rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, buffer, size);
	}

	/* Wait ep unprime */
	csp_usb_wait_endpoint_unprimed(ep_phy_num);

	INIT_LIST_HEAD(&ee->ep[ep_phy_num].tdl);

	do {
		count = min(tmp_size, MAX_DTD_BUFFER_SIZE);
		node = alloc_td_node(ee);
		if (node == RT_NULL) {
			LOG_E("alloc td_node fail");
			return 0;
		}

		node->ptr = alloc_td(ee);
		if (node->ptr == RT_NULL) {
			LOG_E("alloc dtd fail");
			rt_free(node);
			return 0;
		}
		node->phy_addr = (u32)unca_to_phys(node->ptr);
		node->ptr->next_td = LINK_TERMINATE;
		node->ptr->total_bytes = count;
		node->ptr->ioc = 0;
		node->ptr->active = 1;
		node->ptr->page[0] = (u32)phy_buf;
		node->ptr->page[1] = ((u32)phy_buf + 0x1000) & 0xfffff000;
		node->ptr->page[2] = ((u32)phy_buf + 0x2000) & 0xfffff000;
		node->ptr->page[3] = ((u32)phy_buf + 0x3000) & 0xfffff000;
		node->ptr->page[4] = ((u32)phy_buf + 0x4000) & 0xfffff000;

		/* updata buffer point and stransfer size */
		phy_buf = (u8 *)((u32)phy_buf + count);
		tmp_size = tmp_size - count;

		if (!list_empty(&ee->ep[ep_phy_num].tdl)) {
			lastnode = list_entry(ee->ep[ep_phy_num].tdl.prev,
						struct td_node, td);
			lastnode->ptr->next_td = node->phy_addr;
		}

		INIT_LIST_HEAD(&node->td);
		list_add_tail(&node->td, &ee->ep[ep_phy_num].tdl);
	} while (tmp_size != 0);

	/* Set last tdt ioc */
	node->ptr->ioc = 1;

	/* Synchronize dtd */
	dsb();

	/* Set ep queue head */
	firstnode = list_entry(ee->ep[ep_phy_num].tdl.next, struct td_node, td);
	qh->td.halted = 0;
	qh->td.active = 0;
	qh->td.next_td = firstnode->phy_addr;

	/* Synchronize before ep prime */
	dsb();

	/*
	ee_dump_qh(qh);
	ee_dump_dt((void *)qh->td.next_td);
	*/

	/* Prime the ep for transmit */
	csp_usb_prime_endpoint(ep_phy_num);

	/* Synchronize after prime */
	dsb();

	return size;
}

u32 ee_free_data_transfer(struct lombo_udc *ee, u8 ep_phy_num)
{
	u32 actual;
	struct td_node *node, *tmpnode;
	struct list_head *tmplist;

	actual = ee->ep[ep_phy_num].xfer_len;
	tmplist = &ee->ep[ep_phy_num].tdl;
	list_for_each_entry_safe(node, tmpnode, tmplist, td) {
		if (node->ptr->active) {
			LOG_E("ep-%d dtd is still active err", ep_phy_num);
			ee->ep[ep_phy_num].xfer_count = 0;
			return 0;
		}

		actual = actual - node->ptr->total_bytes;
		if (node->ptr->halted) {
			LOG_E("ep-%d halted err", ep_phy_num);
			break;
		} else if (node->ptr->buffer_err) {
			LOG_E("ep-%d data buffer err", ep_phy_num);
			break;
		} else if (node->ptr->trans_err) {
			LOG_E("ep-%d transaction err", ep_phy_num);
			break;
		}

		free_td(node->ptr);
		list_del(&node->td);
		free_td_node(node);
	}

	/* checkout dtd head list */
	if (!list_empty(tmplist))
		LOG_E("ep-%d free dtd err", ep_phy_num);

	ee->ep[ep_phy_num].xfer_count = actual;

	return actual;
}

static void ee_hardware_reset(struct lombo_udc *ee)
{
	/* disable all eps */
	csp_usb_disable_endpoints();

	/* clear all pending interrupts */
	csp_usb_clear_pendings();

	/* set the interrupt threshold control interval to 0 */
	csp_usb_set_threshold();

	/* configure the endpoint list address and make sure it on 64 byte boundary !!! */
	csp_usb_set_list_address(ee->ep[ENDPOINT_CONTROL].qh.phy_addr);

	/* Enalble interrupt : USB interrupt, error, port change, reset, suspend, NAK */
	csp_usb_set_interrupt();

	/* Set default address */
	csp_usb_set_device_add(0);
}

/* init endpoints */
static void ee_init_eps(struct lombo_udc *ee_udc)
{
	int i, j, k;
	struct ee_ep *ep;

	for (i = 0; i < EP_MAX/2; i++) {
		for (j = RX; j <= TX; j++) {
			k = 2 * i + j;
			ep = &ee_udc->ep[k];
			ep->qh.ptr = ee_udc->qh_pool + k * EP_QUEUE_HEAD_SIZE;
			ep->qh.phy_addr = (u32)unca_to_phys(ep->qh.ptr);
			INIT_LIST_HEAD(&ep->qh.list);
			INIT_LIST_HEAD(&ep->tdl);
			LOG_D("ep[%d] ptr: 0x%x phy_addr: 0x%x",
					k, ep->qh.ptr, ep->qh.phy_addr);
		}
	}
}

/* stops all USB activity, flushes & disables all endpts */
static void ee_stop_ep_activity(struct lombo_udc *ee)
{
	struct td_node *node, *tmpnode;
	struct list_head *tmplist;
	rt_base_t level;
	int i;

	/* TODO: flush all endpoints */

	spin_lock_irqsave(&usbd_lock, level);
	for (i = 0; i < EP_MAX; i++) {
		tmplist = &ee->ep[i].tdl;
		list_for_each_entry_safe(node, tmpnode, tmplist, td) {
			free_td(node->ptr);
			list_del_init(&node->td);
			free_td_node(node);
		}
	}
	spin_unlock_irqrestore(&usbd_lock, level);

	ee->suspended = 0;
}

/* Change Data+ pullup status
 * this func is used by usb_gadget_connect/disconnet
 */
static void ee_udc_pullup(int is_on)
{
	if (is_on)
		csp_usb_set_run_stop(1);
	else
		csp_usb_set_run_stop(0);
}

void ee_udc_suspend(void)
{
	ee_udc_pullup(0);
}

void ee_udc_resume(void)
{
	ee_udc_pullup(1);
}

static void isr_reset_handler(struct lombo_udc *ee)
{
	ee_stop_ep_activity(ee);
	ee_hardware_reset(ee);

	/* enable control endpoint */
	ee_enable_endpoint(ee, 0, USB_EP_ATTR_CONTROL, USB_DIR_OUT, 64);
	ee_enable_endpoint(ee, 0, USB_EP_ATTR_CONTROL, USB_DIR_IN, 64);

	udc_event_cb(ee, 0, EP_EVENT_RESET, RT_NULL);
}

static void isr_tr_complete_handler(struct lombo_udc *ee)
{
	u8 i, n;
	struct ee_ep *ep;
	u32 complete_sts, complete_sts_tmp, setup_value;

	complete_sts = cps_usb_get_complete_sts();
	csp_usb_clear_complete(complete_sts);

	for (i = 0; i < EP_MAX; i++) {
		ep = &ee->ep[i];

		/* Check whether endpoint is active */
		if (!ep->active)
			continue;

		/* Check which ep has been completed */
		n = usb_endpoint_phy_num(ep);
		complete_sts_tmp = complete_sts & BIT(n);
		if (complete_sts_tmp) {
			LOG_D("ep-%d-%s completed", ep->num,
					USB_EP_DIR(ep->dir) ? "in" : "out");
			ee_free_data_transfer(ee, i);
			if (ep->type == USB_EP_ATTR_CONTROL && USB_EP_DIR(ep->dir)) {
				if (ep->xfer_len == 0)
					isr_setup_status_complete(ee);
			}

			/* Invalidate the cache, let the cpu read from dram */
			if (!USB_EP_DIR(ep->dir) && ep->xfer_len != 0)
				rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE,
						ep->xfer_buff, ep->xfer_len);

			udc_event_cb(ee, ep->num,
				USB_EP_DIR(ep->dir) ? EP_EVENT_IN : EP_EVENT_OUT,
				RT_NULL);
		}

		/* Only handle setup packet below */
		setup_value = csp_usb_get_setup_sts();
		if (i == 0 && setup_value & BIT(0)) {
			csp_usb_clear_setup();
			/* Invalidate the cache, let the cpu read from dram */
			rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE,
						&ep->qh.ptr->setup, ep->xfer_len);
			udc_event_cb(ee, 0, EP_EVENT_SETUP, &ep->qh.ptr->setup);
		}
	}
}

static void ee_device_hardware_init(struct lombo_udc *ee)
{
	u32 result, p1, p2;

	result = lombo_func2(&p1, &p2);
	if (result) {
		LOG_W("get func2 failed");
		p1 = 1;
	}

	/* usb ahb interface select - dram */
	csp_usb_select_ahb_port(AHB_PORT_DRAM);

	/* deassert phy siddq reset */
	csp_usb_clear_siddq();

	/* UTMI PHY */
	csp_usb_utmi_transceiver();

#ifdef ARCH_LOMBO_N7
	if (p1 == 1)
		csp_usb_force_cfg();
#endif

	/* reset usb controller */
	csp_usb_reset_controller();

	/* set usb controller for usb device */
	csp_usb_device_mode();

	/* csp_usb_full_speed(); */

	ee_hardware_reset(ee);

	/* enable control endpoint */
	ee_enable_endpoint(ee, 0, USB_EP_ATTR_CONTROL, USB_DIR_OUT, 64);
	ee_enable_endpoint(ee, 0, USB_EP_ATTR_CONTROL, USB_DIR_IN, 64);
}

static void ee_show_mem_used(void)
{
#ifdef UDC_MEM_TEST
	u32 total, used, max_used;
	rt_memory_info(&total, &used, &max_used);
	LOG_I("mem used:%x", used);
#endif
}

static void ee_irq_handler(int vector, void *arg)
{
	struct lombo_udc *udc = (struct lombo_udc *)arg;
	u32 intr;

	RT_ASSERT(udc != RT_NULL);

	intr = csp_usb_get_and_clear_int_active();
	if (intr == 0)
		return;

	/* Detect usb reset */
	if (intr & USBI_URI) {
		LOG_D("usbd irq reset");
		isr_reset_handler(udc);
	}

	/* Detect resume signaling or speed change or VBUS deasserted */
	if (intr & USBI_PCI) {
		LOG_D("usbd irq port change");
		if (udc->suspended) {
			LOG_D("udc resume");
			/* ee_udc_resume(); */
			udc->suspended = 0;
		}
	}

	/* Transaction with ioc bit set in td or setup packet or short packet */
	if (intr & USBI_UI) {
		LOG_D("usbd irq interrupt");
		isr_tr_complete_handler(udc);
	}

	/* Enter suspend state from active state */
	if (intr & USBI_SLI) {
		LOG_D("usbd irq suspend");
		udc->suspended = 1;
		/* ee_udc_suspend(); */
		ee_show_mem_used();
	}

	/* Transaction results in an error condition */
	if (intr & USBI_UEI)
		LOG_E("usbd irq transfer error");
}

#ifdef USBD_THREAD
static void ee_usbd_isr_service(void *param)
{
	struct lombo_udc *udc = (struct lombo_udc *)param;

	RT_ASSERT(udc != RT_NULL)

	while (1) {
		rt_sem_take(udc->isr_sem, RT_WAITING_FOREVER);
		isr_tr_complete_handler(udc);
	}
}
#endif

int ee_usbd_init(struct lombo_udc *udc)
{
	int ret;

	memset(udc, 0, sizeof(struct lombo_udc));

	/* 1.1 get reset and gate clock  */
	udc->phy_reset = clk_get(CLK_NAME_USB_PHY_RESET);
	udc->usb_gate = clk_get(CLK_NAME_AHB_USB_GATE);
	udc->usb_reset = clk_get(CLK_NAME_AHB_USB_RESET);
	if ((udc->usb_gate < 0) || (udc->usb_reset < 0) || (udc->phy_reset < 0)) {
		LOG_E("get usb gate/reset clk failed");
		ret = -RT_EINVAL;
		goto get_clk_failed;
	}

	/* 1.2 enable usb gate */
	ret = clk_enable(udc->usb_gate);
	if (ret) {
		LOG_E("enable usb gate failed");
		goto enable_usb_gate_failed;
	}

	/* 1.3 enable phy reset */
	ret = clk_enable(udc->phy_reset);
	if (ret) {
		LOG_E("enable usb phy reset failed");
		goto enable_phy_reset_failed;
	}

	/* Delay for phy clk output stablely */
	rt_thread_mdelay(5);

	/* 1.4 enable usb reset */
	ret = clk_enable(udc->usb_reset);
	if (ret) {
		LOG_E("enable usb reset failed");
		goto enable_usb_reset_failed;
	}

	/* 2. prealloc memory */
	ret = usbd_mem_init(udc);
	if (ret) {
		LOG_E("mem init failed");
		ret = -RT_ENOMEM;
		goto alloc_mem_failed;
	}

	/* 3. init endpoints */
	ee_init_eps(udc);

	/* 4. Usb device controller init */
	ee_device_hardware_init(udc);

	udc->setaddr = RT_FALSE;
	udc->address = 0;
	udc->status.b.state = DEVICE_STATE_UNATTACHED;
	udc->status.b.event = 0;

#ifdef USBD_THREAD
	udc->isr_sem = rt_sem_create("usbdsem", 0, RT_IPC_FLAG_FIFO);
	if (!udc->isr_sem) {
		LOG_E("create sem failed");
		ret = -RT_ENOSYS;
		goto sem_creat_failed;
	}

	udc->isr_thread = rt_thread_create("usbdsrv", ee_usbd_isr_service, (void *)udc,
				USB_THREAD_STACK_SIZE, USB_THREAD_PRIO, USB_THREAD_TICK);
	if (udc->isr_thread != RT_NULL)
		rt_thread_startup(udc->isr_thread);
	else {
		LOG_E("usbd interrupt service init failed");
		ret = -RT_ENOSYS;
		goto create_thread_failed;
	}
#endif

	/* Install and enalbe usb irq */
	rt_hw_interrupt_install(UDC_IRQ_NUM, ee_irq_handler, (void *)udc, "usbd");
	ee_enable_usb_irq();

	/* Set usb controll run */
	csp_usb_set_run_stop(1);

	/* Check whether initialization is ok */
	ret = csp_usb_check_init();
	if (ret) {
		LOG_E("usb init failed");
		goto usb_init_failed;
	}

	return RT_EOK;

usb_init_failed:
#ifdef USBD_THREAD
	rt_thread_delete(udc->isr_thread);

create_thread_failed:
	rt_sem_delete(udc->isr_sem);

sem_creat_failed:
#endif
	usbd_mem_destroy(udc);

alloc_mem_failed:
	clk_disable(udc->usb_reset);

enable_usb_reset_failed:
	clk_disable(udc->usb_gate);

enable_usb_gate_failed:
	clk_disable(udc->phy_reset);

enable_phy_reset_failed:
get_clk_failed:
	if (udc->phy_reset >= 0)
		clk_put(udc->phy_reset);
	if (udc->usb_gate >= 0)
		clk_put(udc->usb_gate);
	if (udc->usb_reset >= 0)
		clk_put(udc->usb_reset);

	return ret;
}

