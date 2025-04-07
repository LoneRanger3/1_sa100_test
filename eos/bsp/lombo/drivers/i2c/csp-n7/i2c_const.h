/*
 * i2c_const.h - I2C host driver code for LomboTech
 * i2c host driver macro define
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
#ifndef ___I2C___CONST__H___
#define ___I2C___CONST__H___

/******************************************************************************
 * feature
 *****************************************************************************/
#define I2C_TX_FIFO_DEPTH		32
#define I2C_RX_FIFO_DEPTH		32

/******************************************************************************
 * Control Register
 *****************************************************************************/
/*
 * speed mode (SPEED)
 */
#define I2C_SS_MODE			1
#define I2C_FS_MODE			2
#define I2C_HS_MODE			3

/*
 * stop detect (STOP_DET)
 */
#define I2C_STOP_DET_IRR_MASTER_ACTIV	0
#define I2C_STOP_DET_IN_MASTER_ACTIV	1

/*
 * tx empty (TX_EMPTY)
 */
#define I2C_TX_EMPTY_AT_BELOW_TXTH	0
#define I2C_TX_EMPTY_IN_CMD_DONE	1

/******************************************************************************
 * Data Buffer and Command Register
 *****************************************************************************/
/*
 * transfer direction (CMD)
 */
#define I2C_CMD_WRITE			0
#define I2C_CMD_READ			1

/*
 * stop (STOP)
 */
#define I2C_NO_STOP			0
#define I2C_SEND_STOP			1

/*
 * restart (RESTART)
 */
#define I2C_NO_RESTART			0
#define I2C_SEND_RESTART		1

/******************************************************************************
 * Target Address Register
 *****************************************************************************/
/*
 * target address format (MASTER_10BITADDR)
 */
#define I2C_TAR_ADDR_7BIT		0
#define I2C_TAR_ADDR_10BIT		1

/******************************************************************************
 * STATUS Register
 *****************************************************************************/
#define I2C_STATUS_TX_DMA_FINISH		(1UL << 29)
#define I2C_STATUS_RX_DMA_FINISH		(1UL << 28)
#define I2C_STATUS_SLV_RX_DATA_LOST		(1UL << 26)
#define I2C_STATUS_SLV_DIS_WHILE_BUSY		(1UL << 25)
#define I2C_STATUS_IC_EN			(1UL << 24)
#define I2C_STATUS_SDA_STK_NOT_RECOV		(1UL << 11)
#define I2C_STATUS_SLV_HOLD_RX_FIFO_FULL	(1UL << 10)
#define I2C_STATUS_SLV_HOLD_TX_FIFO_EMP		(1UL << 9)
#define I2C_STATUS_MST_HOLD_RX_FIFO_FULL	(1UL << 8)
#define I2C_STATUS_MST_HOLD_TX_FIFO_EMP		(1UL << 7)
#define I2C_STATUS_SLV_ACTIVITY			(1UL << 6)
#define I2C_STATUS_MST_ACTIVITY			(1UL << 5)
#define I2C_STATUS_RFF				(1UL << 4)
#define I2C_STATUS_RFNE				(1UL << 3)
#define I2C_STATUS_TFE				(1UL << 2)
#define I2C_STATUS_TFNE				(1UL << 1)
#define I2C_STATUS_ACTIVITY			(1UL << 0)

/******************************************************************************
 * interrupt  (Interrupt Enable/Pending/Clear Register)
 *****************************************************************************/
#define I2C_INT_SCL_STK_AT_LOW			(1UL << 14)
#define I2C_INT_MST_ON_HOLD			(1UL << 13)
#define I2C_INT_RESTART_DET			(1UL << 12)
#define I2C_INT_GEN_CALL			(1UL << 11)
#define I2C_INT_START_DET			(1UL << 10)
#define I2C_INT_STOP_DET			(1UL << 9)
#define I2C_INT_ACTIVITY			(1UL << 8)
#define I2C_INT_TX_ABRT				(1UL << 6)

#ifdef I2C_ENABLE_SLAVE
#define I2C_INT_RX_DONE				(1UL << 7)
#define I2C_INT_RD_REQ				(1UL << 5)
#endif

#define I2C_INT_TX_EMPTY			(1UL << 4)
#define I2C_INT_TX_OVER				(1UL << 3)
#define I2C_INT_RX_FULL				(1UL << 2)
#define I2C_INT_RX_OVER				(1UL << 1)
#define I2C_INT_RX_UNDER			(1UL << 0)

#define I2C_INT_ALL_MASK			(0xFFFFFFFF)

#define I2C_INT_ALL_ERR		(I2C_INT_TX_ABRT | I2C_INT_TX_OVER | \
				I2C_INT_RX_OVER | I2C_INT_RX_UNDER)

/******************************************************************************
 * Transmit Abort Source Register
 *****************************************************************************/
#define I2C_TX_FLUSH_CNT_SHIFT			(23UL)
#define I2C_TX_FLUSH_CNT_MASK			(((1UL << 9) - 1) << 23)

#define I2C_ABT_DEVID_WRITE			(1UL << 20)
#define I2C_ABT_DEVID_ADDR_NAK			(1UL << 19)
#define I2C_ABT_DEVID_NAK			(1UL << 18)
#define I2C_ABT_SDA_STK_AT_LOW			(1UL << 17)
#define I2C_ABT_USER_ABRT			(1UL << 16)

#ifdef I2C_ENABLE_SLAVE
#define I2C_ABT_SLVRD_INTX			(1UL << 15)
#define I2C_ABT_SLV_ARBLOST			(1UL << 14)
#define I2C_ABT_SLV_FL_TX_FIFO			(1UL << 13)
#endif

#define I2C_ABT_MST_ARB_LOST			(1UL << 12)
#define I2C_ABT_MST_DIS				(1UL << 11)
#define I2C_ABT_10B_RD_NORSTRT			(1UL << 10)
#define I2C_ABT_SB_NORSTRT			(1UL << 9)
#define I2C_ABT_HS_NORSTRT			(1UL << 8)
#define I2C_ABT_SB_ACKDET			(1UL << 7)
#define I2C_ABT_HS_ACKDET			(1UL << 6)
#define I2C_ABT_GCALL_READ			(1UL << 5)
#define I2C_ABT_GCALL_NAK			(1UL << 4)
#define I2C_ABT_TXDATA_NAK			(1UL << 3)
#define I2C_ABT_10ADDR2_NAK			(1UL << 2)
#define I2C_ABT_10ADDR1_NAK			(1UL << 1)
#define I2C_ABT_7B_ADDR_NAK			(1UL << 0)

#define I2C_TX_ABTSRC_ALL_MASK			(0xFFFFFFFF)

#endif /* ___I2C___CONST__H___ */
