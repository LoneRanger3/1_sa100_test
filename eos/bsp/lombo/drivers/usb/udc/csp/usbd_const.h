#ifndef __USBD_CONST__H__
#define __USBD_CONST__H__

#include <bitops.h>

/* CONTROL EP NUMBER */
#define ENDPOINT_CONTROLEP	0

/* Endpoint dir */
#define ENDPOINT_DIR_OUT	0x00

/* USBCMD */
#define USB_RESET_DONE		0
#define USB_SET_RESET		1
#define ZERO_INT_THRESHOLD	0
#define USB_RUN			1
#define USB_STOP		0

/* USBMODE */
#define USBMODE_DEVICE		0x2
#define AUTO_LOWER_POWER_D	0x0

/* OTGSC */
#define OTG_TERMINATION		1

/* PORTSC1 */
#define FORCE_FULL_SPEED	1
#define PTS_UTMI_UTMIP		0x0
#define PTS_ULPI		0x2

#define PTS2_OTHER		0x0

/* DEVICEADDR */
#define ADVANCE_DEV_ADD		1

/* ENDPTCTRL */
#define ENDPT_RX_RUN		0
#define ENDPT_RX_STALL		1
#define ENDPT_RX_SOURCE		0
#define ENDPT_RX_CONTROL	0x0
#define ENDPT_RX_TOGGLE		0
#define ENDPT_RX_RESET_TOGGLE	1
#define ENDPT_RX_ENABLE		1
#define ENDPT_RX_DISABLE	0
#define ENDPT_TX_RUN		0
#define ENDPT_TX_STALL		1
#define ENDPT_TX_SOURCE		0
#define ENDPT_TX_CONTROL	0x0
#define ENDPT_TX_TOGGLE		0
#define ENDPT_TX_RESET_TOGGLE	1
#define ENDPT_TX_ENABLE		1
#define ENDPT_TX_DISABLE	0

/* ENDPTNAK */
#define CLREAR_ALL_NAK		0xFFFFFFFF

/* ENDPTNAKEN */
#define DISABLE_ALL_NAK		0
#define ENABLE_ENDPT_NAK	1

/* USBSTS & USBINTR */
#define USBI_UI			BIT(0)
#define USBI_UEI		BIT(1)
#define USBI_PCI		BIT(2)
#define USBI_URI		BIT(6)
#define USBI_SLI		BIT(8)

/* USBINTR */
#define USB_INT_ENABLE		1
#define USB_ERR_ENABLE		1
#define PORT_CHANGE_ENABLE	1
#define RESET_INT_ENABLE	1
#define SOF_INT_ENABLE		1
#define DCSUSPEND_INT_ENABLE	1
#define NAK_INT_ENABLE		1

/* ENDPTPRIME */
#define ENDPT_NO_PRIME		0

/* ENDPTFLUSH */
#define FLUSH_ALL_ENDPT		0xFFFFFFFF
#define FLUSH_ENDPT_DONE	0

/* EP REGISTER SIZE */
#define EP_ADDR_SETP		0x4

/* PHYCFG */
#define SIDDQ_CLEAR		0

/* USBCTRL */
#define AHB_PORT_DRAM		0x0
#define AHB_PORT_SRAM		0x1

#endif /* __USBD_CONST__H__ */
