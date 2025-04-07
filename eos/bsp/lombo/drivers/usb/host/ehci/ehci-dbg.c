/*
 * ehci-dbg.c - Uart driver for LomboTech Socs
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

static char *pid_code(int pid)
{
	switch (pid) {
	case 0:
		return "OUT";
	case 1:
		return "IN";
	case 2:
		return "SETUP";
	default:
		return "unknown";
	}
}

static void __attribute__((unused)) dbg_qtd(const char *label, struct ehci_qtd *qtd)
{
	rt_kprintf("%s<qtd:%x, dma:%x>\n", label, qtd, qtd->qtd_dma);
	rt_kprintf("next_qtd:0x%x alter_next_qtd:0x%x ping_state:0x%x", qtd->hw_next,
			qtd->hw_alt_next, qtd->hw_token & 1);
	rt_kprintf(" split_xstate:%d missed_uframe:%d trans_err:%d\n",
		(qtd->hw_token >> 1) & 1, (qtd->hw_token >> 2) & 1,
		(qtd->hw_token >> 3) & 1);
	rt_kprintf("babble:0x%x buffer_err:%d halted:%d active:%d",
		(qtd->hw_token >> 4) & 1, (qtd->hw_token >> 5) & 1,
		(qtd->hw_token >> 6) & 1, (qtd->hw_token >> 7) & 1);
	rt_kprintf(" pid_code:%s err_counter:%d current_page:%d ioc:%d\n",
		pid_code((qtd->hw_token >> 8) & 3), (qtd->hw_token >> 10) & 3,
		(qtd->hw_token >> 12) & 0x07, (qtd->hw_token >> 15) & 1);
	rt_kprintf("bytes_to_transfer:0x%x data_toggle:%d\n",
		(qtd->hw_token >> 16) & 0x7fff, (qtd->hw_token >> 31) & 1);
	rt_kprintf("buffer[0]:0x%x\nbuffer[1]:0x%x\n",
		qtd->hw_buf[0], qtd->hw_buf[1]);
	rt_kprintf("buffer[2]:0x%x\nbuffer[3]:0x%x\n",
		qtd->hw_buf[2], qtd->hw_buf[3]);
	rt_kprintf("buffer[4]:0x%x\n", qtd->hw_buf[4]);
}

static void __attribute__((unused)) dbg_qtd_list(const char *label,
				struct list_head *qtd_list)
{
	struct ehci_qtd *qtd;

	rt_kprintf(label);
	rt_kprintf("\n");

	if (list_empty(qtd_list)) {
		rt_kprintf("qtd list is empty\n");
		return;
	}

	list_for_each_entry(qtd, qtd_list, qtd_list) {
		dbg_qtd("", qtd);
	}
}

static char *qh_type(int type)
{
	switch (type) {
	case 0:
		return "iTD";
	case 1:
		return "QH";
	case 2:
		return "siTD";
	case 3:
		return "FSTN";
	default:
		return "unknown";
	}
}

static char *qh_speed(int speed)
{
	switch (speed) {
	case 0:
		return "Full";
	case 1:
		return "Low";
	case 2:
		return "High";
	default:
		return "unknown";
	}
}

static void __attribute__((unused)) dbg_qh(const char *label, struct ehci_qh *qh)
{
	struct ehci_qh_hw *hw = qh->hw;

	rt_kprintf("\n");
	rt_kprintf("%s<qh:%x, dma:%x>\n", label, qh, qh->qh_dma);
	rt_kprintf("next:0x%x type:%s terminate:%d dev_addr:%d", QTD_NEXT(hw->hw_next),
		qh_type((hw->hw_next >> 1) & 3), hw->hw_next & 1, hw->hw_info1 & 0x7f);
	rt_kprintf(" inactive_on_next_trans:%d ep_num:%d\n",
		(hw->hw_info1 >> 7) & 1, (hw->hw_info1 >> 8) & 0x0f);
	rt_kprintf("ep_speed:%s data_toggle_control:%d",
		qh_speed((hw->hw_info1 >> 12) & 0x03), (hw->hw_info1 >> 14) & 1);
	rt_kprintf(" head_reclamation_flag:%d maxpkg:%d\n",
		(hw->hw_info1 >> 15) & 1, (hw->hw_info1 >> 16) & 0x7ff);
	rt_kprintf("control_endpoint_flag:%d nak_count_reload:%d",
		(hw->hw_info1 >> 27) & 1, (hw->hw_info1 >> 27) & 1);
	rt_kprintf(" uframe_smask:%d uframe_cmask:%d\n",
		(hw->hw_info2 >> 0) & 0xff, (hw->hw_info2 >> 8) & 0xff);
	rt_kprintf("hub_address:%d port_num:%d mult:%d current_qtd:0x%x\n",
		(hw->hw_info2 >> 16) & 0x7f, (hw->hw_info2 >> 23) & 0x7f,
		(hw->hw_info2 >> 30) & 3, hw->hw_current);

	dbg_qtd("overlay", (struct ehci_qtd *)&hw->hw_qtd_next);
}

static void __attribute__((unused)) dbg_qh_list(const char *label, struct ehci_qh *head)
{
	struct ehci_qh *qh;

	qh = head;
	dbg_qh(label, qh);
	while (qh->qh_next.qh) {
		qh = qh->qh_next.qh;
		dbg_qh(label, qh);
	}
}

