/*
 * ehci.h - USB module head file
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

#ifndef __EHCI_H__
#define __EHCI_H__

#define DBG_SECTION_NAME	"EHCI"
#define DBG_LEVEL		DBG_LOG
#include <debug.h>

#include <rtdevice.h>

#include "soc_define.h"
#include "irq_numbers.h"
#include "clk.h"

#include "drivers/usb_host.h"
#include <spinlock.h>

#define DBG_ERR		(1 << 0)
#define DBG_WRN		(1 << 1)
#define DBG_INF		(1 << 2)
#define DBG_PIPE	(1 << 3)
#define DBG_QH		(1 << 4)
#define DBG_ASYN	(1 << 5)
#define DBG_SHED	(1 << 6)
#define DBG_BUF		(1 << 7)

#define DBG_FLAG (DBG_ERR | DBG_WRN/* | DBG_INF | DBG_QH | DBG_ASYN | DBG_BUF*/)

#define EHCI_LOG_E(fmt, ...)				\
do {							\
	if (DBG_FLAG & DBG_ERR)				\
		LOG_E(fmt, ##__VA_ARGS__);		\
} while (0)

#define EHCI_LOG_W(fmt, ...)				\
do {							\
	if (DBG_FLAG & DBG_WRN)				\
		LOG_W(fmt, ##__VA_ARGS__);		\
} while (0)


#define EHCI_LOG_I(fmt, ...)				\
do {							\
	if (DBG_FLAG & DBG_INF)				\
		LOG_I(fmt, ##__VA_ARGS__);		\
} while (0)

#define EHCI_LOG_QH(label, QH)				\
do {							\
	if (DBG_FLAG & DBG_QH)				\
		dbg_qh(label, QH);			\
} while (0)

#define EHCI_LOG_QTD_LIST(label, list)			\
do {							\
	if (DBG_FLAG & DBG_QH)				\
		dbg_qtd_list(label, list);		\
} while (0)

#define EHCI_LOG_QH_LIST(label, list)			\
do {							\
	if (DBG_FLAG & DBG_QH)				\
		dbg_qh_list(label, list);		\
} while (0)

#define EHCI_LOG_ASYN(fmt, ...)				\
do {							\
	if (DBG_FLAG & DBG_ASYN)			\
		LOG_I(fmt, ##__VA_ARGS__);		\
} while (0)

#define EHCI_LOG_SHED(fmt, ...)				\
do {							\
	if (DBG_FLAG & DBG_SHED)			\
		LOG_I(fmt, ##__VA_ARGS__);		\
} while (0)

#define EHCI_LOG_BUF(str, buf, size)				\
	do {							\
		if (DBG_FLAG & DBG_BUF) {			\
			char *b = (char *)buf;			\
			int i;					\
			rt_kprintf("%s\n", str);		\
			for (i = 0; i < size; i++) {		\
				if (i % 32 == 0 && i != 0)	\
					rt_kprintf("\n");	\
				rt_kprintf("%x ", b[i]);	\
			}					\
			rt_kprintf("\n");			\
		}						\
	} while (0)

typedef u32 dma_addr_t;

#define cpu_to_hc32(flag)	(flag)

#define USBH_IRQ_NUM		INT_USB20

#define HCD_ENDPOINT_MAXPACKET_XFER_LEN		0xFFEEFFEE

/* max endpoint number  */
#define HCD_MAX_ENDPOINT	8

#define HCD_EP_MAX_PKT_SIZE	1024

/* time of a reset period (ms) */
#define PORT_RESET_PERIOD_MS	100

/* EHCI configuration */
#define HCD_MAX_QHD		HCD_MAX_ENDPOINT
#define	HCD_MAX_QTD		(HCD_MAX_ENDPOINT+3)
#define	HCD_MAX_HS_ITD		4
#define HCD_MAX_SITD		16
#define HCD_MAX_PIPE		HCD_MAX_ENDPOINT
#define HCD_MAX_ITD		HCD_MAX_ENDPOINT

/*
* relationship of periodic frame length and bit:
* (0:1024) - (1:512) - (2:256) - (3:128) - (4:64) - (5:32) - (6:16) - (7:8)
*/
#define	FRAME_LIST_SIZE_BITS	5
#define FRAME_LIST_SIZE		(1024 >> FRAME_LIST_SIZE_BITS)
#define FRAME_LIST_ALIGNMENT	4096

/*
 * Some drivers think it's safe to schedule isochronous transfers more than
 * 256 ms into the future (partly as a result of an old bug in the scheduling
 * code).  In an attempt to avoid trouble, we will use a minimum scheduling
 * length of 512 frames instead of 256.
 */
#define	EHCI_TUNE_FLS		1	/* (medium) 512-frame schedule */

/* Bit field definition for register UsbStatus */
#define EHC_USBSTS_UEI	0x00000002UL	/* USB Error Interrupt */
#define EHC_USBSTS_PCD	0x00000004UL	/* Port Change Detect */
#define	EHC_USBSTS_IAA	0x00000020UL	/* Interrupt on Async Advance */
#define EHC_USBSTS_UAI	0x00040000UL	/* USB Asynchronous Interrupt - EHCI derivation */
#define EHC_USBSTS_UPI	0x00080000UL	/* USB Period Interrupt - EHCI derivation */

#define STS_PPCE_MASK	(0xff<<16)	/* Per-Port change event 1-16 */
#define STS_ASS		(1<<15)		/* Async Schedule Status */
#define STS_PSS		(1<<14)		/* Periodic Schedule Status */
#define STS_RECL	(1<<13)		/* Reclamation */
#define STS_HALT	(1<<12)		/* Not running (any reason) */
/* some bits reserved */
/* these STS_* flags are also intr_enable bits (USBINTR) */
#define STS_IAA		(1<<5)		/* Interrupted on async advance */
#define STS_FATAL	(1<<4)		/* such as some PCI access errors */
#define STS_FLR		(1<<3)		/* frame list rolled over */
#define STS_PCD		(1<<2)		/* port change detect */
#define STS_ERR		(1<<1)		/* "error" completion (overflow, ...) */
#define STS_INT		(1<<0)		/* "normal" completion (short, ...) */

/* Bit field definition for register PortSC */
#define EHC_PORTSC_CCS	0x00000001UL	/* Current Connect Status */
#define EHC_PORTSC_CSC	0x00000002UL	/* Connect Status Change */
#define EHC_PORTSC_PEC	0x00000008UL	/* Port Enabled/Disabled Change */
#define EHC_PORTSC_OCC	0x00000020UL	/* Over-current Change */
#define EHC_PORTSC_FPR	0x00000040UL	/* Force Port Resume */

/* Definitions for Frame List Element Pointer */
#define QTD_MAX_XFER_LENGTH	0x5000
#define LINK_TERMINATE		0x01
#define SPLIT_MAX_LEN_UFRAME	188

/* Each element in the list is a 4K page aligned
physical memory address. The lower 12 bits in each pointer are reserved (except for the
first one), as each memory pointer must reference the start of a 4K page. The field
C_Page specifies the current active pointer. When the transfer element descriptor is
fetched, the starting buffer address is selected using C_Page (similar to an array index
to select an array element).  If a transaction spans a 4K buffer boundary, the host
controller must detect the page-span boundary in the data stream, increment C_Page
and advance to the next buffer pointer in the list, and conclude the transaction via the
new buffer pointer. */
#define BUFFER_PAGE_SIZE		0x1000

#define TD_ALIGN_SIZE		32
#define QH_ALIGN_SIZE		32

/*
 * For various legacy reasons, Linux has a small cookie that's paired with
 * a struct usb_device to identify an endpoint queue.  Queue characteristics
 * are defined by the endpoint's descriptor.  This cookie is called a "pipe",
 * an unsigned int encoded as:
 *
 *  - direction:	bit 7		(0 = Host-to-Device [Out],
 *					 1 = Device-to-Host [In] ...
 *					like endpoint bEndpointAddress)
 *  - device address:	bits 8-14       ... bit positions known to uhci-hcd
 *  - endpoint:		bits 15-18      ... bit positions known to uhci-hcd
 *  - pipe type:	bits 30-31	(00 = isochronous, 01 = interrupt,
 *					 10 = control, 11 = bulk)
 *
 * Given the device address and endpoint descriptor, pipes are redundant.
 */

/* NOTE:  these are not the standard USB_ENDPOINT_XFER_* values!! */
/* (yet ... they're the values used by usbfs) */

#define usb_pipein(pipe)	((pipe) & USB_DIR_IN)
#define usb_pipeout(pipe)	(!usb_pipein(pipe))

#define usb_pipedevice(pipe)	(((pipe) >> 8) & 0x7f)
#define usb_pipeendpoint(pipe)	(((pipe) >> 15) & 0xf)

#define usb_pipetype(pipe)	(((pipe) >> 30) & 3)
#define usb_pipeisoc(pipe)	(usb_pipetype((pipe)) == USB_EP_ATTR_ISOC)
#define usb_pipeint(pipe)	(usb_pipetype((pipe)) == USB_EP_ATTR_INT)
#define usb_pipecontrol(pipe)	(usb_pipetype((pipe)) == USB_EP_ATTR_CONTROL)
#define usb_pipebulk(pipe)	(usb_pipetype((pipe)) == USB_EP_ATTR_BULK)

static inline u32 __create_pipe(u32 devnum, u32 endpoint)
{
	return (devnum << 8) | (endpoint << 15);
}

/* Create various pipes... */
#define usb_sndctrlpipe(dev, endpoint)	\
	((USB_EP_ATTR_CONTROL << 30) | __create_pipe(dev, endpoint))
#define usb_rcvctrlpipe(dev, endpoint)	\
	((USB_EP_ATTR_CONTROL << 30) | __create_pipe(dev, endpoint) | USB_DIR_IN)
#define usb_sndisocpipe(dev, endpoint)	\
	((USB_EP_ATTR_ISOC << 30) | __create_pipe(dev, endpoint))
#define usb_rcvisocpipe(dev, endpoint)	\
	((USB_EP_ATTR_ISOC << 30) | __create_pipe(dev, endpoint) | USB_DIR_IN)
#define usb_sndbulkpipe(dev, endpoint)	\
	((USB_EP_ATTR_BULK << 30) | __create_pipe(dev, endpoint))
#define usb_rcvbulkpipe(dev, endpoint)	\
	((USB_EP_ATTR_BULK << 30) | __create_pipe(dev, endpoint) | USB_DIR_IN)
#define usb_sndintpipe(dev, endpoint)	\
	((USB_EP_ATTR_INT << 30) | __create_pipe(dev, endpoint))
#define usb_rcvintpipe(dev, endpoint)	\
	((USB_EP_ATTR_INT << 30) | __create_pipe(dev, endpoint) | USB_DIR_IN)

#define	QTD_NEXT(dma)	((u32)dma & ~0x1f)

/*
 * EHCI Specification 0.95 Section 3.5
 * QTD: describe data transfer components (buffer, direction, ...)
 * See Fig 3-6 "Queue Element Transfer Descriptor Block Diagram".
 *
 * These are associated only with "QH" (Queue Head) structures,
 * used with control, bulk, and interrupt transfers.
 */
struct ehci_qtd {
	/* first part defined by EHCI spec */
	u32			hw_next;	/* see EHCI 3.5.1 */
	u32			hw_alt_next;    /* see EHCI 3.5.2 */
	u32			hw_token;       /* see EHCI 3.5.3 */
#define	QTD_TOGGLE	(1u << 31)	/* data toggle */
#define	QTD_LENGTH(tok)	(((tok)>>16) & 0x7fff)
#define	QTD_IOC		(1 << 15)	/* interrupt on complete */
#define	QTD_CERR(tok)	(((tok)>>10) & 0x3)
#define	QTD_PID(tok)	(((tok)>>8) & 0x3)
#define	QTD_STS_ACTIVE	(1 << 7)	/* HC may execute this */
#define	QTD_STS_HALT	(1 << 6)	/* halted on error */
#define	QTD_STS_DBE	(1 << 5)	/* data buffer error (in HC) */
#define	QTD_STS_BABBLE	(1 << 4)	/* device was babbling (qtd halted) */
#define	QTD_STS_XACT	(1 << 3)	/* device gave illegal response */
#define	QTD_STS_MMF	(1 << 2)	/* incomplete split transaction */
#define	QTD_STS_STS	(1 << 1)	/* split transaction state */
#define	QTD_STS_PING	(1 << 0)	/* issue PING? */

#define ACTIVE_BIT	QTD_STS_ACTIVE
#define HALT_BIT	QTD_STS_HALT
#define STATUS_BIT	QTD_STS_STS

	u32			hw_buf[5];        /* see EHCI 3.5.4 */

	/* the rest is HCD-private */
	u32 inuse;
	u32			qtd_dma;		/* qtd address */
	struct list_head	qtd_list;		/* sw qtd list */
	size_t			length;			/* length of buffer */
} __aligned(32);

#define IS_SHORT_READ(token) (QTD_LENGTH(token) != 0 && QTD_PID(token) == 1)

/* type tag from {qh,itd,sitd,fstn}->hw_next */
#define Q_NEXT_TYPE(dma)	((dma) & (3 << 1))

/*
 * Now the following defines are not converted using the
 * cpu_to_le32() macro anymore, since we have to support
 * "dynamic" switching between be and le support, so that the driver
 * can be used on one system with SoC EHCI controller using big-endian
 * descriptors as well as a normal little-endian PCI EHCI controller.
 */
/* values for that type tag */
#define Q_TYPE_ITD	(0 << 1)
#define Q_TYPE_QH	(1 << 1)
#define Q_TYPE_SITD	(2 << 1)
#define Q_TYPE_FSTN	(3 << 1)

/* next async queue entry, or pointer to interrupt/periodic QH */
#define QH_NEXT(dma)	((((u32)dma & ~0x01f) | Q_TYPE_QH))

/* for periodic/async schedules and qtd lists, mark end of list */
#define EHCI_LIST_END	1 /* "null pointer" to hw */

/* magic numbers that can affect system performance */
#define	EHCI_TUNE_CERR		3	/* 0-3 qtd retries; 0 == don't stop */
#define	EHCI_TUNE_RL_HS		4	/* nak throttle; see 4.9 */
#define	EHCI_TUNE_RL_TT		0
#define	EHCI_TUNE_MULT_HS	1	/* 1-3 transactions/uframe; 4.10.3 */
#define	EHCI_TUNE_MULT_TT	1

/* first part defined by EHCI spec */
struct ehci_qh_hw {
	u32			hw_next;	/* see EHCI 3.6.1 */
/*----------------------------------------------------------------
|  28| 27|		         16|15|   14|  12|	    8|  7|	        0 |
------------------------------------------------------------------
| RL | C | Maximum Packet Lenght | H | dtc | EP | EndPt | I | Device Address |
------------------------------------------------------------------*/
	u32			hw_info1;       /* see EHCI 3.6.2 */
#define	QH_CONTROL_EP	(1 << 27)	/* FS/LS control endpoint */
#define	QH_HEAD		(1 << 15)	/* Head of async reclamation list */
#define	QH_TOGGLE_CTL	(1 << 14)	/* Data toggle control */
#define	QH_HIGH_SPEED	(2 << 12)	/* Endpoint speed */
#define	QH_LOW_SPEED	(1 << 12)
#define	QH_FULL_SPEED	(0 << 12)
#define	QH_INACTIVATE	(1 << 7)	/* Inactivate on next transaction */
	u32			hw_info2;        /* see EHCI 3.6.2 */
#define	QH_SMASK	0x000000ff
#define	QH_CMASK	0x0000ff00
#define	QH_HUBADDR	0x007f0000
#define	QH_HUBPORT	0x3f800000
#define	QH_MULT		0xc0000000
	u32			hw_current;	/* qtd list - see EHCI 3.6.4 */

	/* qtd overlay (hardware parts of a struct ehci_qtd) */
	u32			hw_qtd_next;
	u32			hw_alt_next;
	u32			hw_token;
	u32			hw_buf[5];
	u32			hw_buf_hi[5];

	u32 inuse;
} __aligned(32);

/*
 * Entries in periodic shadow table are pointers to one of four kinds
 * of data structure.  That's dictated by the hardware; a type tag is
 * encoded in the low bits of the hardware's periodic schedule.  Use
 * Q_NEXT_TYPE to get the tag.
 *
 * For entries in the async schedule, the type tag always says "qh".
 */
union ehci_shadow {
	struct ehci_qh		*qh;		/* Q_TYPE_QH */
	struct ehci_itd		*itd;		/* Q_TYPE_ITD */
	struct ehci_sitd	*sitd;		/* Q_TYPE_SITD */
	/* struct ehci_fstn	*fstn; */	/* Q_TYPE_FSTN */
	u32			*hw_next;	/* (all types) */
	void			*ptr;
};

/* Queue head */
struct ehci_qh {
	struct ehci_qh_hw	*hw;		/* Must come first */

	/* the rest is HCD-private */
	u32			qh_dma;
	struct list_head	qtd_list;	/* sw qtd list */
	struct list_head	intr_node;	/* list of intr QHs */
	union ehci_shadow	qh_next;	/* ptr to qh; or periodic */

	struct list_head	unlink_node;

	u8			qh_state;
#define	QH_STATE_LINKED		1		/* HC sees this */
#define	QH_STATE_UNLINK		2		/* HC may still see this */
#define	QH_STATE_IDLE		3		/* HC doesn't see this */
#define	QH_STATE_UNLINK_WAIT	4		/* LINKED and on unlink q */
#define	QH_STATE_COMPLETING	5		/* don't touch token.HALT */

	u8			xacterrs;	/* XactErr retry counter */
#define	QH_XACTERR_MAX		32		/* XactErr retry limit */

	u16			period;		/* polling interval */
	u16			start;		/* where polling starts */
#define NO_FRAME		((u16)~0)	/* pick new start */


	u32			exception:1;	/* got a fault, or an unlink
							   was requested */
	struct rt_completion	completion;
	u32			xfer_length;
	u32			inuse;
	struct upipe		*pipe;
} __aligned(32);

/* description of one iso transaction (up to 3 KB data if highspeed) */
struct ehci_iso_packet {
	/* These will be copied to iTD when scheduling */
	u64			bufp;		/* itd->hw_bufp{,_hi}[pg] |= */
	u32			transaction;	/* itd->hw_transaction[i] |= */
	u8			cross;		/* buf crosses pages */
	/* for full speed OUT splits */
	u32			buf1;
};

/* temporary schedule data for packets from iso urbs (both speeds)
 * each packet is one logical usb transaction to the device (not TT),
 * beginning at stream->next_uframe
 */
struct ehci_iso_sched {
	struct list_head	td_list;
	unsigned		span;
	struct ehci_iso_packet	packet[0];
};

/*
 * ehci_iso_stream - groups all (s)itds for this endpoint.
 * acts like a qh would, if EHCI had them for ISO.
 */
struct ehci_iso_stream {
	/* first field matches ehci_hq, but is NULL */
	struct ehci_qh_hw	*hw;

	/* u8			bEndpointAddress; */
	u8			highspeed;
	struct list_head	td_list;	/* queued itds/sitds */
	struct list_head	free_list;	/* list of unused itds/sitds */
	/* struct usb_device	*udev; */
	/* struct usb_host_endpoint *ep; */

	/* output of (re)scheduling */
	int			next_uframe;
	u32			splits;

	/* the rest is derived from the endpoint descriptor,
	 * trusting urb->interval == f(epdesc->bInterval) and
	 * including the extra info for hw_bufp[0..2]
	 */
	u8			usecs, c_usecs;
	u16			interval;
	u16			tt_usecs;
	u16			maxp;
	u16			raw_mask;
	unsigned		bandwidth;

	/* This is used to initialize iTD's hw_bufp fields */
	u32			buf0;
	u32			buf1;
	u32			buf2;

	/* this is used to initialize sITD's tt info */
	u32			address;
};

/* isochronous(high-speed) transfer descriptor */
struct ehci_itd {
	/* first part defined by EHCI spec */
	u32			hw_next;	   /* see EHCI 3.3.1 */
	u32			hw_transaction[8]; /* see EHCI 3.3.2 */
#define EHCI_ISOC_ACTIVE        (1u<<31)        /* activate transfer this slot */
#define EHCI_ISOC_BUF_ERR       (1<<30)        /* Data buffer error */
#define EHCI_ISOC_BABBLE        (1<<29)        /* babble detected */
#define EHCI_ISOC_XACTERR       (1<<28)        /* XactErr - transaction error */
#define	EHCI_ITD_LENGTH(tok)	(((tok)>>16) & 0x0fff)
#define	EHCI_ITD_IOC		(1 << 15)	/* interrupt on complete */

#define ITD_ACTIVE(ehci)	EHCI_ISOC_ACTIVE

	u32			hw_bufp[7];	/* see EHCI 3.3.3 */
	u32			hw_bufp_hi[7]; /* Appendix B */

	/* the rest is HCD-private */
	u32			itd_dma;	/* for this itd */
	union ehci_shadow	itd_next;	/* ptr to periodic q entry */

	struct ehci_iso_stream	*stream;	/* endpoint's queue */
	struct list_head	itd_list;	/* list of stream's itds */

	/* any/all hw_transactions here may be used by that urb */
	u32			frame;		/* where scheduled */
	u32			pg;
	u32			index[8];	/* in urb->iso_frame_desc */

	struct upipe		*pipe;
	int			inuse;
} __aligned(32);

/* split transaction isochronous transfer descriptor */
struct ehci_sitd {
	/* first part defined by EHCI spec */
	u32			hw_next;
/* uses bit field macros above - see EHCI 0.95 Table 3-8 */
	u32			hw_fullspeed_ep;	/* EHCI table 3-9 */
	u32			hw_uframe;		/* EHCI table 3-10 */
	u32			hw_results;		/* EHCI table 3-11 */
#define	SITD_IOC	(1u << 31)	/* interrupt on completion */
#define	SITD_PAGE	(1 << 30)	/* buffer 0/1 */
#define	SITD_LENGTH(x)	(0x3ff & ((x)>>16))
#define	SITD_STS_ACTIVE	(1 << 7)	/* HC may execute this */
#define	SITD_STS_ERR	(1 << 6)	/* error from TT */
#define	SITD_STS_DBE	(1 << 5)	/* data buffer error (in HC) */
#define	SITD_STS_BABBLE	(1 << 4)	/* device was babbling */
#define	SITD_STS_XACT	(1 << 3)	/* illegal IN response */
#define	SITD_STS_MMF	(1 << 2)	/* incomplete split transaction */
#define	SITD_STS_STS	(1 << 1)	/* split transaction state */

#define SITD_ACTIVE(ehci)	SITD_STS_ACTIVE

	u32			hw_buf[2];		/* EHCI table 3-12 */
	u32			hw_backpointer;		/* EHCI table 3-13 */
	u32			hw_buf_hi[2];		/* Appendix B */

	/* the rest is HCD-private */
	u32			sitd_dma;
	union ehci_shadow	sitd_next;	/* ptr to periodic q entry */

	struct urb		*urb;
	struct list_head	sitd_list;	/* list of stream's sitds */
	u32			frame;
	u32			index;
} __aligned(32);

/* Type of a transfer description */
enum td_type {
	ITD_TYPE = 0,
	QHD_TYPE,
	SITD_TYPE,
	FSTN_TYPE
};

struct ehci_hcd {
	/* clock resource */
	clk_handle_t		phy_reset;
	clk_handle_t		usb_gate;
	clk_handle_t		usb_reset;

	/* pin resource */
	struct pinctrl		*pctrl;

	/* asynchronic schedule */
	struct ehci_qh		*async;		/* Serve as H-Queue Head in Async
						Schedule, should be physical address */
	u32			async_count;
	struct list_head	async_unlink;

	/* periodic schedule support */
#define	DEFAULT_I_TDPS		1024		/* some HCs can do less */
	u32			periodic_size;
	u32			*periodic;	/* hw periodic table */
	u32			periodic_dma;
	struct list_head	intr_qh_list;

	union ehci_shadow	*pshadow;	/* mirror hw periodic table */
	unsigned		now_frame;	/* frame from HC hardware */
	u32			intr_count;	/* intr activity count */
	u32			isoc_count;	/* isoc activity count */
	u32			periodic_count;

	struct ehci_qh		*int_qhd;	/* Serve as Static 1ms Interrupt Heads,
						should be physical address */
	struct ehci_qh		*qh_pool;
	struct ehci_qh_hw	*qh_hw_pool; 	/* Queue Head, should be
						physical address  */
	struct ehci_qtd		*qtd_pool; 	/* Queue Transfer Descriptor (Queue
						Element), should be physical address */
	struct ehci_itd		*itd_pool;
	struct ehci_sitd	*sitd_pool;

	int			inuse;

	/* usb host conctroller device */
	uhcd_t			uhcd;
};

rt_err_t hcd_init(uhcd_t uhcd);
rt_err_t hcd_deinit(void);

void hcd_port_reset(int port_num);
rt_err_t hcd_open_pipe(upipe_t upipe);
rt_err_t hcd_close_pipe(upipe_t upipe);

int hcd_control_xfer(upipe_t pipe, void *buffer, int length);
int hcd_data_xfer(upipe_t pipe, void *buffer, int length);

#endif /* __EHCI_H__ */

