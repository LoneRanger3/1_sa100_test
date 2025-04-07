/*
 * ehci-q.c - USB host driver for LomboTech Socs
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

DEFINE_SPINLOCK(ehci_q_lock);

static __inline void qh_update(struct ehci_hcd *ehci, struct ehci_qh *qh)
{
	struct ehci_qh_hw *hw = qh->hw;
	struct ehci_qtd *qtd;

	qtd = list_entry(qh->qtd_list.next, struct ehci_qtd, qtd_list);
	hw->hw_qtd_next = qtd->qtd_dma;
	hw->hw_alt_next = EHCI_LIST_END;

	/* Except for control endpoints, we make hardware maintain data
	 * toggle (like OHCI) */
	hw->hw_token &= QTD_TOGGLE | QTD_STS_PING;
}

static void qh_refresh(struct ehci_hcd *ehci, struct ehci_qh *qh)
{
	if (list_empty(&qh->qtd_list))
		EHCI_LOG_E("qtd list is empty?");

	/* Update the QH overlay here. */
	qh_update(ehci, qh);
	EHCI_LOG_QH("qh fresh", qh);
}

static void enable_async(struct ehci_hcd *ehci)
{
	if (ehci->async_count++)
		return;

	/* Don't start the schedule until ASS is 0 */
	csp_usb_enable_async_schedule();
}

static void disable_async(struct ehci_hcd *ehci)
{
	if (--ehci->async_count)
		return;

	/* The async schedule and unlink lists are supposed to be empty */
	if (ehci->async->qh_next.qh || !list_empty(&ehci->async_unlink))
		EHCI_LOG_W("lists are supposed to be empty");

	/* Don't turn off the schedule until ASS is 1 */
	csp_usb_disable_async_schedule();
}

static void qh_link_async(struct ehci_hcd *ehci, struct ehci_qh *qh)
{
	struct ehci_qh *head;

	EHCI_LOG_I("link async qh:%x", qh);

	/* splice right after start */
	head = ehci->async;
	qh->qh_next = head->qh_next;
	qh->hw->hw_next = head->hw->hw_next;
	wmb();

	head->qh_next.qh = qh;
	head->hw->hw_next = QH_NEXT(qh->qh_dma);

	qh->qh_state = QH_STATE_LINKED;
	qh->xacterrs = 0;
	qh->exception = 0;
	/* qtd completions reported later by interrupt */

	EHCI_LOG_QH_LIST("qh link async", ehci->async);
	enable_async(ehci);
}

static void start_iaa_cycle(struct ehci_hcd *ehci)
{
	/* Make sure the unlinks are all visible to the hardware */
	wmb();

	/* DoorBell Handshake: Queue Head will only be free in async_advance_isr */
	csp_usb_set_async_advance();
}

static void single_unlink_async(struct ehci_hcd *ehci, struct ehci_qh *qh)
{
	struct ehci_qh *prev;
	rt_base_t flags;

	/* Unlink it from the schedule */
	prev = ehci->async;
	while (prev->qh_next.qh != qh)
		prev = prev->qh_next.qh;

	prev->hw->hw_next = qh->hw->hw_next;
	prev->qh_next = qh->qh_next;

	/* Add to the end of the list of QHs waiting for the next IAAD */
	qh->qh_state = QH_STATE_UNLINK_WAIT;
	spin_lock_irqsave(&ehci_q_lock, flags);
	list_add_tail(&qh->unlink_node, &ehci->async_unlink);
	spin_unlock_irqrestore(&ehci_q_lock, flags);
}

static void start_unlink_async(struct ehci_hcd *ehci, struct ehci_qh *qh)
{
	/* If the QH isn't linked then there's nothing we can do. */
	if (qh->qh_state != QH_STATE_LINKED)
		return;

	single_unlink_async(ehci, qh);
	start_iaa_cycle(ehci);
}

/* fill a qtd, returning how much of the buffer we were able to queue up */
static int qtd_fill(struct ehci_hcd *ehci, struct ehci_qtd *qtd, u32 buf,
		  size_t len, int token, int maxpacket)
{
	int i, count;
	u32 addr = buf;

	/* one buffer entry per 4K ... first might be short or unaligned */
	qtd->hw_buf[0] = addr;
	count = 0x1000 - (buf & 0x0fff);	/* rest of that page */
	if (likely(len < count))		/* ... iff needed */
		count = len;
	else {
		buf +=  0x1000;
		buf &= ~0x0fff;

		/* per-qtd limit: from 16K to 20K (best alignment) */
		for (i = 1; count < len && i < 5; i++) {
			addr = buf;
			qtd->hw_buf[i] = addr;
			buf += 0x1000;
			if ((count + 0x1000) < len)
				count += 0x1000;
			else
				count = len;
		}

		/* short packets may only terminate transfers */
		if (count != len)
			count -= (count % maxpacket);
	}
	qtd->hw_token = (count << 16) | token;
	qtd->length = count;

	return count;
}

/*
 * reverse of qh_urb_transaction:  free a list of TDs.
 * used for cleanup after errors, before HC sees an URB's TDs.
 */
static void qtd_list_free(struct list_head *qtd_list)
{
	struct list_head *entry, *temp;
	struct ehci_qtd	*qtd;

	list_for_each_safe(entry, temp, qtd_list) {
		qtd = list_entry(entry, struct ehci_qtd, qtd_list);
		list_del(&qtd->qtd_list);
		ehci_qtd_free(qtd);
	}
}

static __inline void qh_wait_complete(struct ehci_qh *qh)
{
	EHCI_LOG_ASYN("qh wait");
	rt_completion_wait(&(qh->completion), RT_WAITING_FOREVER);
}

/*
 * DataToggle parameter only has meaning for control transfer,
 * for other transfer use 0 for these paras
 */
static struct list_head *qh_transaction(struct ehci_hcd *ehci, struct upipe *pipe,
					u32 buf, u32 len, u32 is_setup,
					struct list_head *head)
{
	struct ehci_qtd	*qtd, *qtd_prev;
	u32 token, this_qtd_len, maxpacket, is_input;

	qtd = ehci_qtd_alloc(ehci);
	if (!qtd) {
		EHCI_LOG_E("alloc qtd failed");
		return RT_NULL;
	}
	list_add_tail(&qtd->qtd_list, head);

	token = QTD_STS_ACTIVE;
	/* for split transactions, SplitXState initialized to zero */
	token |= (EHCI_TUNE_CERR << 10);

	is_input = usb_pipein(pipe->info);
	if (is_setup) {
		/* SETUP pid */
		this_qtd_len = qtd_fill(ehci, qtd, buf, len,
				token | (2 /* "setup" */ << 8), 8);
	} else {
		if (is_input)
			token |= (1 /* "in" */ << 8);
		maxpacket = USB_EP_MAX_PKT(pipe->ep.wMaxPacketSize);

		if (usb_pipecontrol(pipe->info))
			token |= QTD_TOGGLE;

		/*
		 * buffer gets wrapped in one or more qtds;
		 * last one may be "short" (including zero len)
		 * and may serve as a control status ack
		 */
		for (;;) {
			this_qtd_len = qtd_fill(ehci, qtd, buf, len, token,
					maxpacket);
			len -= this_qtd_len;
			buf += this_qtd_len;

			/*
			 * short reads advance to a "magic" dummy instead of the next
			 * qtd ... that forces the queue to stop, for manual cleanup.
			 * (this will usually be overridden later.)
			 */

			/* qh makes control packets use qtd toggle; maybe switch it */
			if ((maxpacket & (this_qtd_len + (maxpacket - 1))) == 0)
				token ^= QTD_TOGGLE;

			if (likely(len == 0))
				break;

			qtd_prev = qtd;
			qtd = ehci_qtd_alloc(ehci);
			if (!qtd)
				goto cleanup;
			qtd_prev->hw_next = qtd->qtd_dma;
			list_add_tail(&qtd->qtd_list, head);
		}
	}

	/* by default, enable interrupt on urb completion */
	qtd->hw_token |= QTD_IOC;

	EHCI_LOG_QTD_LIST("qh transation", head);
	return head;

cleanup:
	qtd_list_free(head);

	return RT_NULL;
}

static u32 pipe_make(u8 ep_num, u8 type, u8 dir)
{
	switch (type) {
	case USB_EP_ATTR_CONTROL:
		if (dir == USB_DIR_IN)
			return usb_rcvctrlpipe(0, ep_num);
		else
			return usb_sndctrlpipe(0, ep_num);
	case USB_EP_ATTR_ISOC:
		if (dir == USB_DIR_IN)
			return usb_rcvisocpipe(0, ep_num);
		else
			return usb_sndisocpipe(0, ep_num);
	case USB_EP_ATTR_BULK:
		if (dir == USB_DIR_IN)
			return usb_rcvbulkpipe(0, ep_num);
		else
			return usb_sndbulkpipe(0, ep_num);
	case USB_EP_ATTR_INT:
		if (dir == USB_DIR_IN)
			return usb_rcvintpipe(0, ep_num);
		else
			return usb_sndintpipe(0, ep_num);
	default:
		EHCI_LOG_E("unknown endpoint type");
		return 0;
	}
}

static struct ehci_qh *qh_make(struct ehci_hcd *ehci, struct upipe *pipe)
{
	struct ehci_qh *qh;
	struct ehci_qh_hw *hw;
	struct uendpoint_descriptor *ep = &pipe->ep;
	u32 ep_num, type, dir, maxp, speed, port, info1 = 0, info2 = 0;
	u32 interval;

	qh = ehci_qh_alloc(ehci);
	if (!qh) {
		EHCI_LOG_E("alloc qh failed");
		return RT_NULL;
	}

	ep_num = USB_EP_NUM(ep->bEndpointAddress);
	type = USB_EP_ATTR(ep->bmAttributes);
	dir = USB_EP_DIR(ep->bEndpointAddress);
	maxp = USB_EP_MAX_PKT(ep->wMaxPacketSize);
	speed = pipe->inst->speed;
	port = pipe->inst->port;
	EHCI_LOG_I("device addr:%d, speed:%d, ep num:%d, type:%d, dir:0x%x, max:%d",
		pipe->inst->address, speed, ep_num, type, dir, maxp);

	/*
	 * init endpoint/device data for this QH
	 */
	info1 |= ep_num << 8;
	info1 |= pipe->inst->address << 0;

	/* 1024 byte maxpacket is a hardware ceiling.  High bandwidth
	 * acts like up to 3KB, but is built from smaller packets.
	 */
	if (maxp > 1024) {
		EHCI_LOG_E("bogus qh maxpacket %d\n", maxp);
		goto done;
	}

	/* Compute interrupt scheduling parameters just once, and save.
	 * - allowing for high bandwidth, how many nsec/uframe are used?
	 * - split transactions need a second CSPLIT uframe; same question
	 * - splits also need a schedule gap (for full/low speed I/O)
	 * - qh has a polling interval
	 *
	 * For control/bulk requests, the HC or TT(Transaction Translators) handles these.
	 */
	if (type == USB_EP_ATTR_INT) {
		qh->start = 0;
		/* For high and super device, interval is in 1~16, uint: 128us
		For */
		if (speed == USB_SPEED_HIGH || speed == USB_SPEED_SUPER)
			interval = 1 << (ep->bInterval - 1);
		else
			interval = ep->bInterval;
		EHCI_LOG_I("interval:%d", interval);

		if (speed == USB_SPEED_HIGH) {
			qh->period = interval >> 3;
			if (qh->period == 0 && interval != 1) {
				/* NOTE interval 2 or 4 uframes could work.
				 * But interval 1 scheduling is simpler, and
				 * includes high bandwidth.
				 */
				interval = 1;
			} else if (qh->period > ehci->periodic_size) {
				qh->period = ehci->periodic_size;
				interval = qh->period << 3;
			}
		} else {
			qh->period = interval;
			if (qh->period > ehci->periodic_size) {
				qh->period = ehci->periodic_size;
				interval = qh->period;
			}
		}
	}

	switch (speed) {
	case USB_SPEED_LOW:
		info1 |= QH_LOW_SPEED;
		/* FALL THROUGH */
	case USB_SPEED_FULL:
		/* EPS 0 means "full" */
		if (type != USB_EP_ATTR_INT)
			info1 |= (EHCI_TUNE_RL_TT << 28);
		if (type == USB_EP_ATTR_CONTROL) {
			info1 |= QH_CONTROL_EP;		/* for TT */
			info1 |= QH_TOGGLE_CTL;		/* toggle from qtd */
		}
		info1 |= maxp << 16;

		info2 |= (EHCI_TUNE_MULT_TT << 30);
		/* port number in the queue head is 0..N-1
		 * but eos hub port number is 1..N.
		 */
		info2 |= (port - 1) << 23;
		/* set the address of the TT; for TDI's integrated
		 * root hub tt, leave it zeroed.
		 * root hub for now!
		 */
		info2 |= 0 << 16;
		break;
	case USB_SPEED_HIGH:		/* no TT involved */
		info1 |= QH_HIGH_SPEED;
		if (type == USB_EP_ATTR_CONTROL) {
			info1 |= (EHCI_TUNE_RL_HS << 28);
			info1 |= 64 << 16;	/* usb2 fixed maxpacket */
			info1 |= QH_TOGGLE_CTL;	/* toggle from qtd */
			info2 |= (EHCI_TUNE_MULT_HS << 30);
		} else if (type == USB_EP_ATTR_BULK) {
			info1 |= (EHCI_TUNE_RL_HS << 28);
			/* The USB spec says that high speed bulk endpoints
			 * always use 512 byte maxpacket.  But some device
			 * vendors decided to ignore that, and MSFT is happy
			 * to help them do so.  So now people expect to use
			 * such nonconformant devices with Linux too; sigh.
			 */
			info1 |= maxp << 16;
			info2 |= EHCI_TUNE_MULT_HS << 30;
		} else { /* USB_EP_ATTR_INT */
			info1 |= maxp << 16;
			info2 |= 1 << 30;
		}
		break;
	default:
		EHCI_LOG_E("bogus speed %d\n", speed);
		goto done;
	}

	pipe->info = pipe_make(ep_num, type, dir);
	pipe->hcpriv = qh;
	qh->pipe = pipe;

	/* init as live */
	qh->qh_state = QH_STATE_IDLE;
	hw = qh->hw;
	hw->hw_next = EHCI_LIST_END;
	hw->hw_info1 = info1;
	hw->hw_info2 = info2;

	EHCI_LOG_QH("make qh", qh);

	return qh;
done:
	ehci_qh_free(qh);

	return NULL;
}

/*
 * For control/bulk/interrupt, return QH with these TDs appended.
 * Allocates and initializes the QH if necessary.
 * Returns null if it can't allocate a QH it needs to.
 * If the QH has TDs (urbs) already, that's great.
 */
static __inline void qh_append_tds(struct ehci_qh *qh,
					struct list_head *qtd_list, u32 length)
{
	list_splice_tail(qtd_list, &qh->qtd_list);
	qh->xfer_length = length;
}

static int async_submit(struct ehci_hcd *ehci, struct upipe *pipe,
				struct list_head *qtd_list, int length)
{
	struct ehci_qh *qh = (struct ehci_qh *)pipe->hcpriv;

	EHCI_LOG_QH("async submit", qh);

	/* add qtds to qh's qtd_list*/
	qh_append_tds(qh, qtd_list, length);

	/* clear halt and/or toggle */
	qh_refresh(ehci, qh);

	qh_wait_complete(qh);

	return length - qh->xfer_length;
}

static int qh_completions(struct ehci_hcd *ehci, struct ehci_qh *qh)
{
	struct list_head *entry, *tmp;
	struct ehci_qtd *qtd, *last = RT_NULL;
	struct ehci_qh_hw *hw = qh->hw;
	u32 token = 0, stopped;
	u8 state;

	EHCI_LOG_I("qh:%x", qh);
	state = qh->qh_state;
	stopped = (state == QH_STATE_IDLE);

	list_for_each_safe(entry, tmp, &qh->qtd_list) {
		qtd = list_entry(entry, struct ehci_qtd, qtd_list);
		EHCI_LOG_I("qtd:%x", qtd);

		/* hardware copies qtd out of qh overlay */
		rmb();
		token = qtd->hw_token;

		/* always clean up qtds the hc de-activated */
 retry_xacterr:
		if ((token & QTD_STS_ACTIVE) == 0) {
			/* Report Data Buffer Error: non-fatal but useful */
			if (token & QTD_STS_DBE)
				EHCI_LOG_E("data buffer err");

			/* on STALL, error, and short reads this urb must
			 * complete and all its qtds must be recycled.
			 */
			if ((token & QTD_STS_HALT) != 0) {
				EHCI_LOG_I("qtd halted");
				/* retry transaction errors until we
				 * reach the software xacterr limit
				 */
				if ((token & QTD_STS_XACT) && QTD_CERR(token) == 0 &&
						++qh->xacterrs < QH_XACTERR_MAX) {
					EHCI_LOG_I("detected XactErr len %d/%d retry %d",
			qtd->length - QTD_LENGTH(token), qtd->length, qh->xacterrs);
					/* reset the token in the qtd and the
					 * qh overlay (which still contains
					 * the qtd) so that we pick up from
					 * where we left off
					 */
					token &= ~QTD_STS_HALT;
					token |= QTD_STS_ACTIVE | (EHCI_TUNE_CERR << 10);
					qtd->hw_token = token;
					wmb();
					hw->hw_token = token;
					goto retry_xacterr;
				}
				stopped = 1;
				qh->pipe->status = UPIPE_STATUS_STALL;
			/* magic dummy for some short reads; qh won't advance.
			 * that silicon quirk can kick in with this dummy too.
			 *
			 * other short reads won't stop the queue, including
			 * control transfers (status stage handles that) or
			 * most other single-qtd reads ... the queue stops if
			 * URB_SHORT_NOT_OK was set so the driver submitting
			 * the urbs could clean it up.
			 */
			} else if (IS_SHORT_READ(token)
					&& !(qtd->hw_alt_next & EHCI_LIST_END)) {
				EHCI_LOG_W("short read!!!");
				stopped = 1;
			}

			if (stopped == 0)
				qh->xfer_length -= qtd->length - QTD_LENGTH(token);

		/* stop scanning when we reach qtds the hc is using */
		} else if (likely(!stopped)) {
			EHCI_LOG_I("stopped == 0");
			break;
		}

		EHCI_LOG_I("stoped:%d", stopped);

		/* if we're removing something not at the queue head,
		 * patch the hardware queue pointer.
		 */
		if (qtd->qtd_list.prev != &qh->qtd_list) {
			last = list_entry(qtd->qtd_list.prev,
					struct ehci_qtd, qtd_list);
			EHCI_LOG_I("last:%x", last);
			last->hw_next = qtd->hw_next;
		}

		/* remove qtd; it's recycled after possible urb completion */
		list_del(&qtd->qtd_list);
		ehci_qtd_free(qtd);
		EHCI_LOG_I("free qtd:%x", qtd);

		/* reinit the xacterr counter for the next qtd */
		qh->xacterrs = 0;
	}

	if (stopped != 0 || hw->hw_qtd_next == EHCI_LIST_END)
		qh->exception = 1;

	return qh->exception;
}

static void free_qtd_list(struct ehci_qh *qh)
{
	struct list_head *entry, *tmp;
	struct ehci_qtd *qtd, *last = RT_NULL;

	list_for_each_safe(entry, tmp, &qh->qtd_list) {
		qtd = list_entry(entry, struct ehci_qtd, qtd_list);
		/* if we're removing something not at the queue head,
		 * patch the hardware queue pointer.
		 */
		if (qtd->qtd_list.prev != &qh->qtd_list) {
			last = list_entry(qtd->qtd_list.prev,
					struct ehci_qtd, qtd_list);
			last->hw_next = qtd->hw_next;
		}

		/* remove qtd; it's recycled after possible urb completion */
		list_del(&qtd->qtd_list);
		ehci_qtd_free(qtd);
		EHCI_LOG_I("free qtd:%x", qtd);
	}
}

static void scan_async(struct ehci_hcd *ehci)
{
	struct ehci_qh *qh, *qh_next;
	int temp;

	qh_next = ehci->async->qh_next.qh;
	while (qh_next) {
		qh = qh_next;
		qh_next = qh->qh_next.qh;
		EHCI_LOG_QH("scan async qh", qh);

		/* clean any finished work for this qh */
		if (!list_empty(&qh->qtd_list)) {
			temp = qh_completions(ehci, qh);
			if (likely(temp)) {
				EHCI_LOG_I("qh[%x] is empty", qh);
				rt_completion_done(&qh->completion);
			}
		}
	}
}

