#ifndef ___LOMBO_USB_H___
#define ___LOMBO_USB_H___

#include "csp.h"

#define VA_USB_USBCMD			(0x00000140 + BASE_USB + VA_USB)

#define VA_USB_DEVICEADDR		VA_USB_PERIODICLISTBASE
#define VA_USB_ENDPOINTLISTADDR		VA_USB_ASYNCLISTADDR

typedef union {
	u32 val;
	struct {
		u32 rs:1;
		u32 rst:1;
		u32 fs1_0:2;
		u32 pse:1;
		u32 ase:1;
		u32 iaa:1;
		u32 lr:1;
		u32 asp0:1;
		u32 asp1:1;
		u32 rsvd0:1;
		u32 aspe:1;
		u32 rsvd1:1;
		u32 sutw:1;
		u32 atdtw:1;
		u32 fs2:1;
		u32 itc:8;
		u32 rsvd2:8;
	} bits;
} reg_usb_usbcmd_t;

typedef union {
	u32 val;
	struct {
		u32 rsvd0:24;
		u32 usbadra:1;
		u32 usbadr:7;
	} bits;
} reg_usb_deviceaddr_t;

typedef union {
	u32 val;
	struct {
		u32 rsvd0:11;
		u32 asybase:21;
	} bits;
} reg_usb_endpointlistaddr_t;

#endif

