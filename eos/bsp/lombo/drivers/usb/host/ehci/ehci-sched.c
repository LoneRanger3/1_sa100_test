/*
 * ehci-sched.c - USB host driver for LomboTech Socs
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

DEFINE_SPINLOCK(ehci_sched_lock);

static void enable_periodic(struct ehci_hcd *ehci)
{
	EHCI_LOG_I("enable period");
	if (ehci->periodic_count++)
		return;

	/* Don't start the schedule until PSS is 0 */
	csp_usb_enable_period_schedule();
}

static void disable_periodic(struct ehci_hcd *ehci)
{
	if (--ehci->periodic_count)
		return;

	/* Don't turn off the schedule until PSS is 1 */
	csp_usb_disable_period_schedule();
}

/*
 * periodic_next_shadow - return "next" pointer on shadow list
 * @periodic: host pointer to qh/itd/sitd
 * @tag: hardware tag for type of this record
 */
static union ehci_shadow *periodic_next_shadow(union ehci_shadow *periodic,
				u32 tag)
{
	switch (tag) {
	case Q_TYPE_QH:
		return &periodic->qh->qh_next;
	case Q_TYPE_FSTN:
		EHCI_LOG_E("TODO");
		/* return &periodic->fstn->fstn_next; */
	case Q_TYPE_ITD:
		return &periodic->itd->itd_next;
	default:
		return &periodic->sitd->sitd_next;
	}
}

static u32 *shadow_next_periodic(union ehci_shadow *periodic, u32 tag)
{
	switch (tag) {
	/* our ehci_shadow.qh is actually software part */
	case Q_TYPE_QH:
		return &periodic->qh->hw->hw_next;
	/* others are hw parts */
	default:
		return periodic->hw_next;
	}
}

/* "first fit" scheduling policy used the first time through,
 * or when the previous schedule slot can't be re-used.
 */
static void qh_schedule(struct ehci_hcd *ehci, struct ehci_qh *qh)
{
	u32 uframe = 0;
	struct ehci_qh_hw *hw = qh->hw;

	/* reset S-frame and (maybe) C-frame masks */
	hw->hw_info2 &= ~(QH_CMASK | QH_SMASK);
	hw->hw_info2 |= qh->period ? (1 << uframe) : QH_SMASK;
}

/* periodic schedule slots have iso tds (normal or split) first, then a
 * sparse tree for active interrupt transfers.
 */
static void qh_link_periodic(struct ehci_hcd *ehci, struct ehci_qh *qh)
{
	u32 i;
	u32 period = qh->period;

	qh_schedule(ehci, qh);

	EHCI_LOG_I("link periodic qh:%x, start:%d, period:%d", qh, qh->start, period);

	/* high bandwidth, or otherwise every microframe */
	if (period == 0)
		period = 1;

	for (i = qh->start; i < ehci->periodic_size; i += period) {
		union ehci_shadow	*prev = &ehci->pshadow[i];
		u32			*hw_p = &ehci->periodic[i];
		union ehci_shadow	here = *prev;
		u32			type = 0;

		/* iso node is head of int node, skip the iso nodes at list head */
		while (here.ptr) {
			type = Q_NEXT_TYPE(*hw_p);
			if (type == Q_TYPE_QH)
				break;
			prev = periodic_next_shadow(prev, type);
			hw_p = shadow_next_periodic(&here, type);
			here = *prev;
		}

		/* sorting each branch by period (slow-->fast)
		 * enables sharing interior tree nodes
		 */
		while (here.ptr && qh != here.qh) {
			if (qh->period > here.qh->period)
				break;
			prev = &here.qh->qh_next;
			hw_p = &here.qh->hw->hw_next;
			here = *prev;
		}
		/* link in this qh, unless some earlier pass did that */
		if (qh != here.qh) {
			qh->qh_next = here;
			if (here.qh)
				qh->hw->hw_next = *hw_p;
			wmb();
			prev->qh = qh;
			*hw_p = QH_NEXT(qh->qh_dma);
			EHCI_LOG_I("hw_p:%x, *hw_p:%x", hw_p, *hw_p);
		}
	}
	qh->qh_state = QH_STATE_LINKED;
	qh->xacterrs = 0;
	qh->exception = 0;
	EHCI_LOG_I("qh->hw->hw_next:%x", qh->hw->hw_next);

	for (i = qh->start; i < ehci->periodic_size; i += period)
		EHCI_LOG_I("periodic[%d]:%x", i, ehci->periodic[i]);

	/* for scan */
	list_add(&qh->intr_node, &ehci->intr_qh_list);

	/* maybe enable periodic schedule processing */
	++ehci->intr_count;
	enable_periodic(ehci);
}

/* caller must hold ehci->lock */
static void periodic_unlink(struct ehci_hcd *ehci, unsigned frame, void *ptr)
{
	union ehci_shadow	*prev_p = &ehci->pshadow[frame];
	u32			*hw_p = &ehci->periodic[frame];
	union ehci_shadow	here = *prev_p;

	/* find predecessor of "ptr"; hw and shadow lists are in sync */
	while (here.ptr && here.ptr != ptr) {
		prev_p = periodic_next_shadow(prev_p, Q_NEXT_TYPE(*hw_p));
		hw_p = shadow_next_periodic(&here, Q_NEXT_TYPE(*hw_p));
		here = *prev_p;
	}
	/* an interrupt entry (at list end) could have been shared */
	if (!here.ptr)
		return;

	/* update shadow and hardware lists ... the old "next" pointers
	 * from ptr may still be in use, the caller updates them.
	 */
	*prev_p = *periodic_next_shadow(&here, Q_NEXT_TYPE(*hw_p));

	if (*shadow_next_periodic(&here, Q_NEXT_TYPE(*hw_p)) != EHCI_LIST_END)
		*hw_p = *shadow_next_periodic(&here, Q_NEXT_TYPE(*hw_p));
}

static void qh_unlink_periodic(struct ehci_hcd *ehci, struct ehci_qh *qh)
{
	u32 i;
	u32 period;

	EHCI_LOG_I("unlink period qh:%x", qh);

	/*
	 * If qh is for a low/full-speed device, simply unlinking it
	 * could interfere with an ongoing split transaction.  To unlink
	 * it safely would require setting the QH_INACTIVATE bit and
	 * waiting at least one frame, as described in EHCI 4.12.2.5.
	 *
	 * We won't bother with any of this.  Instead, we assume that the
	 * only reason for unlinking an interrupt QH while the current URB
	 * is still active is to dequeue all the URBs (flush the whole
	 * endpoint queue).
	 *
	 * If rebalancing the periodic schedule is ever implemented, this
	 * approach will no longer be valid.
	 */

	/* high bandwidth, or otherwise part of every microframe */
	period = qh->period;
	if (period == 0)
		period = 1;

	for (i = qh->start; i < ehci->periodic_size; i += period)
		periodic_unlink(ehci, i, qh);

	/* qh->qh_next still "live" to HC */
	qh->qh_state = QH_STATE_UNLINK;
	qh->qh_next.ptr = NULL;

	list_del(&qh->intr_node);
}

static void end_unlink_intr(struct ehci_hcd *ehci, struct ehci_qh *qh)
{
	struct ehci_qh_hw *hw = qh->hw;

	qh->qh_state = QH_STATE_IDLE;
	hw->hw_next = EHCI_LIST_END;

	if (!list_empty(&qh->qtd_list))
		qh_completions(ehci, qh);

	/* maybe turn off periodic schedule */
	--ehci->intr_count;
	disable_periodic(ehci);
}

static void start_unlink_intr(struct ehci_hcd *ehci, struct ehci_qh *qh)
{
	/* If the QH isn't linked then there's nothing we can do. */
	if (qh->qh_state != QH_STATE_LINKED)
		return;

	qh_unlink_periodic(ehci, qh);

	/* Make sure the unlinks are visible */
	wmb();

	end_unlink_intr(ehci, qh);

	ehci_qh_free(qh);
}

static void intr_submit(struct ehci_hcd *ehci, struct upipe *pipe,
				struct list_head *qtd_list, int length)
{
	struct ehci_qh *qh = (struct ehci_qh *)pipe->hcpriv;

	qh_append_tds(qh, qtd_list, length);
	qh_refresh(ehci, qh);
}

static void scan_intr(struct ehci_hcd *ehci)
{
	struct ehci_qh *qh, *qh_next;
	struct upipe *pipe;
	int temp;

	list_for_each_entry_safe(qh, qh_next, &ehci->intr_qh_list, intr_node) {
		EHCI_LOG_QH("scan intr qh", qh);
		/* clean any finished work for this qh */
		if (!list_empty(&qh->qtd_list)) {
			temp = qh_completions(ehci, qh);
			if (likely(temp)) {
				EHCI_LOG_I("qh[%x] is empty", qh);
				pipe = qh->pipe;
				if (pipe->callback && pipe->status == UPIPE_STATUS_OK)
					pipe->callback(pipe);
			}
		}
	}
}

#ifdef LOMBO_ISO

static inline void itd_sched_init(struct ehci_hcd *ehci,
				struct ehci_iso_sched *iso_sched,
				struct ehci_iso_stream *stream,
				struct upipe *pipe)
{
	unsigned i;
	dma_addr_t dma = pipe->transfer_dma;

	/* how many uframes are needed for these transfers */
	iso_sched->span = pipe->number_of_packets * stream->interval;

	/* figure out per-uframe itd fields that we'll need later
	 * when we fit new itds into the schedule.
	 */
	for (i = 0; i < pipe->number_of_packets; i++) {
		struct ehci_iso_packet	*uframe = &iso_sched->packet[i];
		unsigned		length;
		dma_addr_t		buf;
		u32			trans;

		length = pipe->iso_frame_desc[i].length;
		buf = dma + pipe->iso_frame_desc[i].offset;

		trans = EHCI_ISOC_ACTIVE;
		trans |= buf & 0x0fff;

		if (unlikely((i + 1) == pipe->number_of_packets))
			trans |= EHCI_ITD_IOC;
		trans |= length << 16;
		uframe->transaction = trans;

		/* might need to cross a buffer page within a uframe */
		uframe->bufp = (buf & ~(u64)0x0fff);
		buf += length;
		if (unlikely((uframe->bufp != (buf & ~(u64)0x0fff))))
			uframe->cross = 1;
	}
}

static struct ehci_iso_sched *iso_sched_alloc(unsigned packets)
{
	struct ehci_iso_sched *iso_sched;
	int size = sizeof(*iso_sched);

	size += packets * sizeof(struct ehci_iso_packet);
	iso_sched = rt_zalloc(size);
	if (likely(iso_sched != NULL))
		INIT_LIST_HEAD(&iso_sched->td_list);

	return iso_sched;
}

static void iso_sched_free(struct ehci_iso_stream *stream,
				struct ehci_iso_sched *iso_sched)
{
	if (!iso_sched)
		return;

	list_splice(&iso_sched->td_list, &stream->free_list);
	rt_free(iso_sched);
}

static int itd_urb_transaction(struct ehci_hcd *ehci,
			struct ehci_iso_stream *stream, struct upipe *pipe)
{
	struct ehci_itd		*itd;
	dma_addr_t		itd_dma;
	int			i;
	unsigned		num_itds;
	struct ehci_iso_sched	*sched;
	rt_base_t		flags;

	sched = iso_sched_alloc(1/*urb->number_of_packets*/);
	if (unlikely(sched == NULL))
		return -ENOMEM;

	itd_sched_init(ehci, sched, stream, pipe);

	if (pipe->interval < 8)
		num_itds = 1 + (sched->span + 7) / 8;
	else
		num_itds = pipe->number_of_packets;

	/* allocate/init ITDs */
	spin_lock_irqsave(&ehci_sched_lock, flags);
	for (i = 0; i < num_itds; i++) {

		/*
		 * Use iTDs from the free list, but not iTDs that may
		 * still be in use by the hardware.
		 */
		if (likely(!list_empty(&stream->free_list))) {
			itd = list_first_entry(&stream->free_list,
					struct ehci_itd, itd_list);
			if (itd->frame == ehci->now_frame)
				goto alloc_itd;
			list_del(&itd->itd_list);
			itd_dma = itd->itd_dma;
		} else {
 alloc_itd:
			spin_unlock_irqrestore(&ehci_sched_lock, flags);
			itd = ehci_itd_alloc(ehci);
			spin_lock_irqsave(&ehci_sched_lock, flags);
			if (!itd) {
				iso_sched_free(stream, sched);
				spin_unlock_irqrestore(&ehci_sched_lock, flags);
				return -ENOMEM;
			}
		}

		memset(itd, 0, sizeof(*itd));
		itd->frame = 9999;		/* an invalid value */
		list_add(&itd->itd_list, &sched->td_list);
	}
	spin_unlock_irqrestore(&ehci_sched_lock, flags);

	/* temporarily store schedule info in hcpriv */
	pipe->hcpriv = sched;
	pipe->error_count = 0;

	return 0;
}

static inline void itd_init(struct ehci_hcd *ehci, struct ehci_iso_stream *stream,
				struct ehci_itd *itd)
{
	int i;

	/* it's been recently zeroed */
	itd->hw_next = EHCI_LIST_END;
	itd->hw_bufp[0] = stream->buf0;
	itd->hw_bufp[1] = stream->buf1;
	itd->hw_bufp[2] = stream->buf2;

	for (i = 0; i < 8; i++)
		itd->index[i] = -1;

	/* All other fields are filled when scheduling */
}

static inline void itd_patch(struct ehci_hcd	*ehci, struct ehci_itd	*itd,
				struct ehci_iso_sched *iso_sched, unsigned index,
				u16 uframe)
{
	struct ehci_iso_packet *uf = &iso_sched->packet[index];
	unsigned pg = itd->pg;

	uframe &= 0x07;
	itd->index[uframe] = index; /* uframe to packet */

	itd->hw_transaction[uframe] = uf->transaction;
	itd->hw_transaction[uframe] |= cpu_to_hc32(pg << 12);
	itd->hw_bufp[pg] |= cpu_to_hc32(uf->bufp & ~(u32)0);
	itd->hw_bufp_hi[pg] |= cpu_to_hc32((u32)(uf->bufp >> 32));

	/* iso_frame_desc[].offset must be strictly increasing */
	if (unlikely(uf->cross)) {
		u64 bufp = uf->bufp + 4096;

		itd->pg = ++pg;
		itd->hw_bufp[pg] |= cpu_to_hc32(bufp & ~(u32)0);
		itd->hw_bufp_hi[pg] |= cpu_to_hc32((u32)(bufp >> 32));
	}
}

static inline void itd_link(struct ehci_hcd *ehci, unsigned frame,
				struct ehci_itd *itd)
{
	union ehci_shadow *prev = &ehci->pshadow[frame];
	u32 *hw_p = &ehci->periodic[frame];
	union ehci_shadow here = *prev;
	u32 type = 0;

	/* skip any iso nodes which might belong to previous microframes */
	while (here.ptr) {
		type = Q_NEXT_TYPE(*hw_p);
		if (type == cpu_to_hc32(Q_TYPE_QH))
			break;
		prev = periodic_next_shadow(prev, type);
		hw_p = shadow_next_periodic(&here, type);
		here = *prev;
	}

	itd->itd_next = here;
	itd->hw_next = *hw_p;
	prev->itd = itd;
	itd->frame = frame;
	wmb();
	*hw_p = cpu_to_hc32(itd->itd_dma | Q_TYPE_ITD);
}

/* fit urb's itds into the selected schedule slot; activate as needed */
static void itd_link_urb(struct ehci_hcd *ehci, struct upipe *pipe, unsigned mod,
				struct ehci_iso_stream *stream)
{
	int packet;
	unsigned next_uframe, uframe, frame;
	struct ehci_iso_sched *iso_sched = pipe->hcpriv;
	struct ehci_itd	*itd;

	next_uframe = stream->next_uframe & (mod - 1);

	/* fill iTDs uframe by uframe */
	for (packet = 0, itd = NULL; packet < pipe->number_of_packets;) {
		if (itd == NULL) {
			/* ASSERT: no itds for this endpoint in this uframe */
			itd = list_entry(iso_sched->td_list.next,
					struct ehci_itd, itd_list);
			list_move_tail(&itd->itd_list, &stream->td_list);
			itd->stream = stream;
			itd->pipe = pipe;
			itd_init(ehci, stream, itd);
		}

		uframe = next_uframe & 0x07;
		frame = next_uframe >> 3;

		itd_patch(ehci, itd, iso_sched, packet, uframe);

		next_uframe += stream->interval;
		next_uframe &= mod - 1;
		packet++;

		/* link completed itds into the schedule */
		if (((next_uframe >> 3) != frame)
				|| packet == pipe->number_of_packets) {
			itd_link(ehci, frame & (ehci->periodic_size - 1), itd);
			itd = NULL;
		}
	}
	stream->next_uframe = next_uframe;

	/* don't need that schedule data any more */
	iso_sched_free(stream, iso_sched);
	pipe->hcpriv = stream;

	++ehci->isoc_count;
	enable_periodic(ehci);
}
#endif

