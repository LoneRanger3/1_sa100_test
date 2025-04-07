/*
 * dma_common.h - DMA common code for LomboTech
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

#ifndef ___DMA_COMMON___H___
#define ___DMA_COMMON___H___

/**
 * channel device source and destination request define
 */
#define MEM_RT_RP		0
#define SPI0_RX_SRP		1
#define SPI0_TX_DSP		2
#define SPI1_RX_SRP		3
#define SPI1_TX_DSP		4
#define SPI2_RX_SRP		5
#define SPI2_TX_DSP		6
#define I2C0_RX_SRP		7
#define I2C0_TX_DRP		8
#define I2C1_RX_SRP		9
#define I2C1_TX_DRP		10
#define I2C2_RX_SRP		11
#define I2C2_TX_DRP		12
#define I2C3_RX_SRP		13
#define I2C3_TX_DRP		14
#define UART0_RX_SRP		15
#define UART0_TX_DRP		16
#define UART1_RX_SRP		17
#define UART1_TX_DRP		18
#define UART2_RX_SRP		19
#define UART2_TX_DRP		20
#define UART3_RX_SRP		21
#define UART3_TX_DRP		22
#define I2S_RX_SRP		23
#define I2S_TX_DRP		24
#define TON_TX_DRP		26

/*
 * DMA channel priority define, from 0 to 7, 0 is the lowest
 */
#define DMA_CHAN_PRIO_0		0
#define DMA_CHAN_PRIO_1		1
#define DMA_CHAN_PRIO_2		2
#define DMA_CHAN_PRIO_3		3
#define DMA_CHAN_PRIO_4		4
#define DMA_CHAN_PRIO_5		5
#define DMA_CHAN_PRIO_6		6
#define DMA_CHAN_PRIO_7		7

#endif /* ___DMA_COMMON___H___ */
