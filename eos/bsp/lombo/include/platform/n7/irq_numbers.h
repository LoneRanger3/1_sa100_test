/*
 * Copyright (c) 2012, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#if !defined(__IRQ_NUMBERS_H__)
#define __IRQ_NUMBERS_H__

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

//! @brief i.MX6 interrupt numbers.
//!
//! This enumeration lists the numbers for all of the interrupts available on the i.MX6 series.
//! Use these numbers when specifying an interrupt to the GIC.
//!
//! The first 16 interrupts are special in that they are reserved for software interrupts generated
//! by the SWI instruction.
enum _imx_interrupts
{
	SW_INTERRUPT_0 = 0, //!< Software interrupt 0.
	SW_INTERRUPT_1 = 1, //!< Software interrupt 1.
	SW_INTERRUPT_2 = 2, //!< Software interrupt 2.
	SW_INTERRUPT_3 = 3, //!< Software interrupt 3.
	SW_INTERRUPT_4 = 4, //!< Software interrupt 4.
	SW_INTERRUPT_5 = 5, //!< Software interrupt 5.
	SW_INTERRUPT_6 = 6, //!< Software interrupt 6.
	SW_INTERRUPT_7 = 7, //!< Software interrupt 7.
	SW_INTERRUPT_8 = 8, //!< Software interrupt 8.
	SW_INTERRUPT_9 = 9, //!< Software interrupt 9.
	SW_INTERRUPT_10 = 10,   //!< Software interrupt 10.
	SW_INTERRUPT_11 = 11,   //!< Software interrupt 11.
	SW_INTERRUPT_12 = 12,   //!< Software interrupt 12.
	SW_INTERRUPT_13 = 13,   //!< Software interrupt 13.
	SW_INTERRUPT_14 = 14,   //!< Software interrupt 14.
	SW_INTERRUPT_15 = 15,   //!< Software interrupt 15.
	RSVD_INTERRUPT_16 = 16, //!< Reserved.
	RSVD_INTERRUPT_17 = 17, //!< Reserved.
	RSVD_INTERRUPT_18 = 18, //!< Reserved.
	RSVD_INTERRUPT_19 = 19, //!< Reserved.
	RSVD_INTERRUPT_20 = 20, //!< Reserved.
	RSVD_INTERRUPT_21 = 21, //!< Reserved.
	RSVD_INTERRUPT_22 = 22, //!< Reserved.
	RSVD_INTERRUPT_23 = 23, //!< Reserved.
	RSVD_INTERRUPT_24 = 24, //!< Reserved.
	RSVD_INTERRUPT_25 = 25, //!< Reserved.
	INT_HYP_PPI = 26, //!< Reserved.
	INT_VIRT_PPI = 27, //!< Reserved.
	RSVD_INTERRUPT_28 = 28, //!< Reserved.
	INT_PHYS_SECURE_PPI = 29, //!< Reserved.
	INT_PHYS_NONSECURE_PPI = 30, //!< Reserved.
	RSVD_INTERRUPT_31 = 31, //!< Reserved.

	/* lomboswer */
	INT_RSVD_32	= 32,
	INT_WDOG	= 33,
	INT_RTC		= 34,
	INT_GTIMER0	= 35,
	INT_GTIMER1	= 36,
	INT_GTIMER2	= 37,
	INT_GTIMER3	= 38,
	INT_RSVD_39	= 39,
	INT_RSVD_40	= 40,
	INT_PWM		= 41,
	INT_DMA		= 42,
	INT_PTIMER0	= 43,
	INT_PTIMER1	= 44,
	INT_RSVD_45	= 45,
	INT_RSVD_46	= 46,
	INT_I2C0	= 47,
	INT_I2C1	= 48,
	INT_I2C2	= 49,
	INT_I2C3	= 50,
	INT_RSVD_51	= 51,
	INT_RSVD_52	= 52,
	INT_SPI0	= 53,
	INT_SPI1	= 54,
	INT_SPI2	= 55,
	INT_RSVD_56	= 56,
	INT_RSVD_57	= 57,
	INT_RSVD_58	= 58,
	INT_UART0	= 59,
	INT_UART1	= 60,
	INT_UART2	= 61,
	INT_UART3	= 62,
	INT_RSVD_63	= 63,
	INT_RSVD_64	= 64,
	INT_GPADC	= 65,
	INT_RSVD_66	= 66,
	INT_USB20	= 67,
	INT_RSVD_68	= 68,
	INT_RSVD_69	= 69,
	INT_RSVD_70	= 70,
	INT_RSVD_71	= 71,
	INT_RSVD_72	= 72,
	INT_SIO		= 73,
	INT_GPIO0	= 74,
	INT_GPIO1	= 75,
	INT_GPIO2	= 76,
	INT_RSVD_77	= 77,
	INT_RSVD_78	= 78,
	INT_RSVD_79	= 79,
	INT_SDC0	= 80,
	INT_SDC1	= 81,
	INT_RSVD_82	= 82,
	INT_RSVD_83	= 83,
	INT_SDRAM	= 84,
	INT_I2S		= 85,
	INT_RSVD_86	= 86,
	INT_RSVD_87	= 87,
	INT_AC		= 88,
	INT_RSVD_89	= 89,
	INT_VC		= 90,
	INT_RSVD_91	= 91,
	INT_TCON	= 92,
	INT_RSVD_93	= 93,
	INT_RSVD_94	= 94,
	INT_RSVD_95	= 95,
	INT_RSVD_96	= 96,
	INT_MIPI_DSI	= 97,
	INT_RSVD_98	= 98,
	INT_TVE		= 99,
	INT_RSVD_100	= 100,
	INT_RSVD_101	= 101,
	INT_VIC		= 102,
	INT_MIPI_CSI	= 103,
	INT_AX_ENU	= 104,
	INT_AX_CVU	= 105,
	INT_ISP0	= 106,
	INT_ISP_MI0	= 107,
	INT_ISP1	= 108,
	INT_ISP_MI1	= 109,
	INT_RSVD_110	= 110,
	INT_RSVD_111	= 111,
	INT_TVD		= 112,
	INT_RSVD_113	= 113,
	INT_RSVD_114	= 114,
	INT_RSVD_115	= 115,
	INT_DC0		= 116,
	INT_DC1		= 117,
	INT_SE0		= 118,
	INT_SE1		= 119,
	INT_DIT		= 120,
	INT_RSVD_121	= 121,
	INT_ROT		= 122,
	INT_RSVD_123	= 123,
	INT_AXIERR	= 124,
	INT_nPMUIRQ0	= 125,
	INT_COMMRX0	= 126,
	INT_COMMTX0	= 127,
	INT_nPMUIRQ1	= 128,
	INT_COMMRX1	= 129,
	INT_COMMTX1	= 130,
	INT_IR_RX	= 143,
	IMX_INTERRUPT_COUNT = 160   //!< Total number of interrupts.
};

#ifdef ARCH_LOMBO
#define IRQ_PRIO_CRITICAL	0
#define IRQ_PRIO_HIGH		1
#define IRQ_PRIO_NORMAL		2
#define IRQ_PRIO_LOW		3
#endif

#endif // __IRQ_NUMBERS_H__
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
