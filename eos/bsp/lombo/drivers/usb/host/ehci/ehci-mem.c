/*
 * ehci-mem.c - USB host driver for LomboTech Socs
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

static void ehci_qtd_free(struct ehci_qtd *qtd)
{
	if (!qtd->inuse)
		EHCI_LOG_W("qtd is free");
	qtd->inuse = 0;
}

static __inline void ehci_qtd_init(struct ehci_qtd *qtd, u32 dma)
{
	memset(qtd, 0, sizeof(struct ehci_qtd));
	qtd->qtd_dma = dma;
	qtd->hw_token = QTD_STS_HALT;
	qtd->hw_next = EHCI_LIST_END;
	qtd->hw_alt_next = EHCI_LIST_END;
	INIT_LIST_HEAD(&qtd->qtd_list);
}

static struct ehci_qtd *ehci_qtd_alloc(struct ehci_hcd *ehci)
{
	struct ehci_qtd *qtd;
	u32 idx;

	for (idx = 0; idx < HCD_MAX_QTD; idx++) {
		qtd = &ehci->qtd_pool[idx];
		if (!qtd->inuse)
			break;
	}

	if (idx == HCD_MAX_QTD)
		return RT_NULL;

	ehci_qtd_init(qtd, (u32)qtd);
	qtd->inuse = 1;

	return qtd;
}

static struct ehci_qh *qh_alloc(struct ehci_hcd *ehci)
{
	struct ehci_qh *qh;
	u32 i;

	/* Looking for a free QHD */
	for (i = 0; i < HCD_MAX_QHD; i++) {
		qh = &ehci->qh_pool[i];
		if (!qh->inuse)
			break;
	}

	/* Not found */
	if (i == HCD_MAX_QHD)
		return RT_NULL;

	/* Clear memory */
	memset(qh, 0, sizeof(struct ehci_qh));
	qh->inuse = 1;

	return qh;
}


static struct ehci_qh_hw *qh_hw_alloc(struct ehci_hcd *ehci)
{
	struct ehci_qh_hw *hw;
	u32 i;

	/* Looking for a free hw QHD */
	for (i = 0; i < HCD_MAX_QHD; i++) {
		hw = &ehci->qh_hw_pool[i];
		if (!hw->inuse)
			break;
	}

	/* Not found */
	if (i == HCD_MAX_QHD)
		return RT_NULL;

	/* Clear memory */
	memset(hw, 0, sizeof(struct ehci_qh_hw));
	hw->inuse = 1;

	return hw;
}

static void ehci_qh_free(struct ehci_qh *qh)
{
	if (qh->hw)
		qh->hw->inuse = 0;
	qh->inuse = 0;
}

static struct ehci_qh *ehci_qh_alloc(struct ehci_hcd *ehci)
{
	struct ehci_qh *qh;

	qh = qh_alloc(ehci);
	if (!qh)
		return RT_NULL;

	qh->hw = qh_hw_alloc(ehci);
	if (!qh->hw) {
		ehci_qh_free(qh);
		return RT_NULL;
	}

	qh->qh_dma = (u32)qh->hw;
	INIT_LIST_HEAD(&qh->qtd_list);
	rt_completion_init(&qh->completion);

	return qh;
}

#ifdef LOMBO_IOS
static struct ehci_itd *itd_hw_alloc(struct ehci_hcd *ehci)
{
	struct ehci_itd *hw;
	u32 i;

	/* Looking for a free hw QHD */
	for (i = 0; i < HCD_MAX_ITD; i++) {
		hw = &ehci->itd_pool[i];
		if (!hw->inuse)
			break;
	}

	/* Not found */
	if (i == HCD_MAX_ITD)
		return RT_NULL;

	/* Clear memory */
	memset(hw, 0, sizeof(struct ehci_itd));
	hw->inuse = 1;

	return hw;
}

static struct ehci_itd *ehci_itd_alloc(struct ehci_hcd *ehci)
{
	struct ehci_itd *itd;

	itd = itd_hw_alloc(ehci);
	if (!itd)
		return RT_NULL;

	itd->itd_dma = (u32)itd;
	INIT_LIST_HEAD(&itd->itd_list);

	return itd;
}
#endif

rt_err_t hcd_mem_init(struct ehci_hcd *ehci)
{
	void *ptr;
	u32 i;

	/* Periodic List */
	ptr = rt_zalloc_unca_align(sizeof(struct ehci_qh), QH_ALIGN_SIZE);
	if (!ptr) {
		LOG_E("alloc mem failed");
		goto mem_free;
	}
	ehci->int_qhd = (struct ehci_qh *)unca_to_phys((u32)ptr);

	/* Qh pool */
	ptr = rt_zalloc(sizeof(struct ehci_qh) * HCD_MAX_QHD);
	if (!ptr) {
		LOG_E("alloc mem failed");
		goto mem_free;
	}
	ehci->qh_pool = (struct ehci_qh *)ptr;

	/* Qh hw pool */
	ptr = rt_zalloc_unca_align(sizeof(struct ehci_qh_hw) * HCD_MAX_QHD,
				QH_ALIGN_SIZE);
	if (!ptr) {
		LOG_E("alloc mem for qh pool failed");
		goto mem_free;
	}
	ehci->qh_hw_pool = (struct ehci_qh_hw *)unca_to_phys((u32)ptr);

	/* Asynchronous List */
	ehci->async = ehci_qh_alloc(ehci);
	if (!ehci->async) {
		LOG_E("alloc async qh failed");
		goto mem_free;
	}

	/* Qtd pool */
	ptr = rt_zalloc_unca_align(sizeof(struct ehci_qtd) * HCD_MAX_QHD, TD_ALIGN_SIZE);
	if (!ptr) {
		LOG_E("alloc mem failed");
		goto mem_free;
	}
	ehci->qtd_pool = (struct ehci_qtd *)unca_to_phys((u32)ptr);

#ifdef LOMBO_ISO
	/* ITD for high speed ISO transfers
	* itd should be byte alignment (for hw parts) and can't cross 4K,
	* so make it align with 4K temporarily
	*/
	ptr = rt_zalloc_unca_align(sizeof(struct ehci_itd) * HCD_MAX_ITD, 4096);
	if (!ptr) {
		LOG_E("alloc mem failed");
		goto mem_free;
	}
	ehci->itd_pool = (struct ehci_itd *)unca_to_phys((u32)ptr);

	/* SITD for full/low speed split ISO transfers]
	* itd should be byte alignment (for hw parts) and can't cross 4K,
	* so make it align with 4K temporarily
	*/
	ptr = rt_zalloc_unca_align(sizeof(struct ehci_sitd) * HCD_MAX_SITD, 4096);
	if (!ptr) {
		LOG_E("alloc mem failed");
		goto mem_free;
	}
	ehci->sitd_pool = (struct ehci_sitd *)unca_to_phys((u32)ptr);
#endif

	/* Hardware periodic table */
	ptr = rt_zalloc_unca_align(sizeof(void *) * FRAME_LIST_SIZE,
					FRAME_LIST_ALIGNMENT);
	if (!ptr) {
		LOG_E("alloc mem failed");
		goto mem_free;
	}
	ehci->periodic = (u32 *)unca_to_phys(ptr);
	ehci->periodic_dma = (u32)ehci->periodic;

	/* Initialize periodic table */
	for (i = 0; i < ehci->periodic_size; i++)
		ehci->periodic[i] = EHCI_LIST_END;

	/* software shadow of hardware table */
	ehci->pshadow = rt_zalloc(ehci->periodic_size * sizeof(void *));
	if (!ehci->pshadow)
		goto mem_free;

	return RT_EOK;

mem_free:
	if (ehci->int_qhd)
		rt_free_unca_align(phys_to_unca(ehci->int_qhd));
	if (ehci->qh_pool)
		rt_free(ehci->qh_pool);
	if (ehci->qh_hw_pool)
		rt_free_unca_align(phys_to_unca(ehci->qh_hw_pool));
	if (ehci->qtd_pool)
		rt_free_unca_align(phys_to_unca(ehci->qtd_pool));
	if (ehci->periodic)
		rt_free_unca_align(phys_to_unca(ehci->periodic));
#ifdef LOMBO_ISO
	if (ehci->itd_pool)
		rt_free_unca_align(phys_to_unca(ehci->itd_pool));
	if (ehci->sitd_pool)
		rt_free_unca_align(phys_to_unca(ehci->sitd_pool));
#endif
	return -RT_ENOMEM;
}

void hcd_mem_free(struct ehci_hcd *ehci)
{
	if (ehci->int_qhd)
		rt_free_unca_align(phys_to_unca(ehci->int_qhd));
	if (ehci->qh_pool)
		rt_free(ehci->qh_pool);
	if (ehci->qh_hw_pool)
		rt_free_unca_align(phys_to_unca(ehci->qh_hw_pool));
	if (ehci->qtd_pool)
		rt_free_unca_align(phys_to_unca(ehci->qtd_pool));
	if (ehci->periodic)
		rt_free_unca_align(phys_to_unca(ehci->periodic));
#ifdef LOMBO_ISO
	if (ehci->itd_pool)
		rt_free_unca_align(phys_to_unca(ehci->itd_pool));
	if (ehci->sitd_pool)
		rt_free_unca_align(phys_to_unca(ehci->sitd_pool));
#endif

}

