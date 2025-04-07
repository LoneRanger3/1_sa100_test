/*
 * i2c_csp.h - I2C host driver code for LomboTech
 * i2c hardware operation interface
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
#ifndef ___I2C___CSP__H___
#define ___I2C___CSP__H___

#include <csp.h>

/******************************************************************************
 * base controller
 *****************************************************************************/
/**
 * csp_i2c_enable - enable/disable the i2c controller
 * @base_addr:	base address for I2CC
 * @enable:	0, disable; 1, enable
 */
void csp_i2c_enable(u32 base_addr, u32 enable);

/******************************************************************************
 * base configure
 *****************************************************************************/

/*
 * operate mode
 */

/**
 * csp_i2c_set_master_mode - enable the I2CC to master mode
 * @base_addr:	base address for I2CC
 */
void csp_i2c_set_master_mode(u32 base_addr);

/**
 * csp_i2c_enable_restart - whether enable restart function
 * @base_addr:	base address for I2CC
 * @enable:	0, disable; 1, enable
 */
void csp_i2c_enable_restart(u32 base_addr, u32 enable);

/**
 * csp_i2c_enable_rx_ffh - set whether to hold SCL LOW when RX_FIFO full
 * @base_addr:	base address for I2CC
 * @enable:	0, not hold SCL LOW when RX_FIFO full;
 *		1, hold SCL LOW when RX_FIFO full
 */
void csp_i2c_enable_rx_ffh(u32 base_addr, u32 enable);

/**
 * csp_i2c_hs_master_code - set the hs master code
 * @base_addr:	base address for I2CC
 * @maddr:	low 3bit of the I2CC master address
 */
void csp_i2c_hs_master_code(u32 base_addr, u32 maddr);

/*
 * Speed mode
 */

/**
 * csp_i2c_set_speed_mode - set the master speed mode
 * @base_addr:	base address for I2CC
 * @speed_mode:	speed mode, can be I2C_SS_MODE, I2C_FS_MODE, I2C_HS_MODE
 */
void csp_i2c_set_speed_mode(u32 base_addr, u32 speed_mode);

#if 0
/**
 * csp_i2c_enable_hs_sclpp - enable/disable HS SCL push-pull
 * @base_addr:	base address for I2CC
 * @enable:	0, disable; 1, enable
 */
void csp_i2c_enable_hs_sclpp(u32 base_addr, u32 enable);
#endif

/*
 * INT_behavior
 */

/**
 * csp_i2c_enable_hs_sclpp - enable/disable HS SCL push-pull
 * @base_addr:	base address for I2CC
 * @enable:	0, disable; 1, enable
 */
void csp_i2c_enable_hs_sclpp(u32 base_addr, u32 enable);

/**
 * csp_i2c_master_stop_det - set STOP_DET behavior
 * @base_addr:	base address for I2CC
 * @bhv:	STOP_DET behavior, can be I2C_STOP_DET_IN_MASTER_ACTIV
 *		or I2C_STOP_DET_IRR_MASTER_ACTIV
 */
void csp_i2c_master_stop_det(u32 base_addr, u32 bhv);

/**
 * csp_i2c_tx_empty_mode - set TX_EMPTY behavior
 * @base_addr:	base address for I2CC
 * @bhv:	TX_EMPTY behavior, can be I2C_TX_EMPTY_IN_CMD_DONE
 *		or I2C_TX_EMPTY_AT_BELOW_TXTH
 */
void csp_i2c_tx_empty_mode(u32 base_addr, u32 bhv);

/******************************************************************************
 * Timing
 *****************************************************************************/
/**
 * csp_i2c_set_ss_baudrate - set the SS baudrate
 * @base_addr:	base address for I2CC
 * @h_cnt:	SCL HIGH duration in ic clock cycle cnt
 * @l_cnt:	SCL LOW duration in ic clock cycle cnt
 */
void csp_i2c_set_ss_baudrate(u32 base_addr, u32 h_cnt, u32 l_cnt);

/**
 * csp_i2c_set_fs_baudrate - set the FS/FS+ baudrate
 * @base_addr:	base address for I2CC
 * @h_cnt:	SCL HIGH duration in ic clock cycle cnt
 * @l_cnt:	SCL LOW duration in ic clock cycle cnt
 */
void csp_i2c_set_fs_baudrate(u32 base_addr, u32 h_cnt, u32 l_cnt);

/**
 * csp_i2c_set_hs_baudrate - set the HS baudrate
 * @base_addr:	base address for I2CC
 * @h_cnt:	SCL HIGH duration in ic clock cycle cnt
 * @l_cnt:	SCL LOW duration in ic clock cycle cnt
 */
void csp_i2c_set_hs_baudrate(u32 base_addr, u32 h_cnt, u32 l_cnt);

/**
 * csp_i2c_cfg_spk_len - set the spk length
 * @base_addr:	base address for I2CC
 * @fs_spk_len:	SS/FS/FS+ spike length in ic clock cycle cnt
 * @hs_spk_len:	HS spike length in ic clock cycle cnt
 */
void csp_i2c_cfg_spk_len(u32 base_addr, u32 fs_spk_len, u32 hs_spk_len);

/**
 * csp_i2c_set_timing - set TX and RX hold time on SDA
 * @base_addr:	base address for I2CC
 * @tx_ht:	TX hold time on SDA in ic clock cycle cnt
 * @rx_ht:	RX hold time on SDA in ic clock cycle cnt
 * @st:		setup time on SDA in ic clock cycle cnt
 */
void csp_i2c_set_timing(u32 base_addr, u32 tx_ht, u32 rx_ht, u32 st);

/******************************************************************************
 * interrput enable/pending/clear
 *****************************************************************************/

/**
 * csp_i2c_enable_ints - enable the specified interrputs
 * @base_addr:	base address for I2CC
 * @int_mask:	int enable bits to be set
 */
void csp_i2c_enable_ints(u32 base_addr, u32 int_mask);

/**
 * csp_i2c_disable_ints - enable the specified interrputs
 * @base_addr:	base address for I2CC
 * @int_mask:	int enable bits to be set
 */
void csp_i2c_disable_ints(u32 base_addr, u32 int_mask);

/* get int pending status */
u32 csp_i2c_get_int_pd(u32 base_addr);

/**
 * csp_i2c_clr_pd - clear int pending status
 * @base_addr:	base address for I2CC
 * @pd_mask:	int pending bits status to be clear
 */
void csp_i2c_clr_pd(u32 base_addr, u32 pd_mask);

/**
 * csp_i2c_chk_err - check whether there are error pendings occur
 * @base_addr:	base address for I2CC
 *
 * return 0 when not error, otherwise 1
 */
u32 csp_i2c_chk_err(u32 base_addr);

/**
 * csp_i2c_chk_pd - check whether all the expected int pending have been raise
 * @base_addr:	base address for I2CC
 * @pd_mask:	int pending bits to be check
 *
 * return 1 when all the expected pending bits are set, otherwise 0
 */
u32 csp_i2c_chk_pd(u32 base_addr, u32 pd_mask);

/******************************************************************************
 * status
 *****************************************************************************/

/**
 * csp_i2c_chk_status - check whether all the expected status have been present
 * @base_addr:	base address for I2CC
 * @status:	expected status
 *
 * return 1 when all the expected status bits are set, otherwise 0
 */
u32 csp_i2c_chk_status(u32 base_addr, u32 status);

/******************************************************************************
 * abort source
 *****************************************************************************/
/**
 * csp_i2c_get_abrt - get the abort sources
 * @base_addr:	base address for I2CC
 *
 * return bit masks of abort sources
 */
u32 csp_i2c_get_abrt(u32 base_addr);

/**
 * csp_i2c_clr_tx_abrt - clear the abort source
 * @base_addr:	base address for I2CC
 * @abrtsrc:	abort source to be clear
 */
void csp_i2c_clr_tx_abrt(u32 base_addr, u32 abrtsrc);

/******************************************************************************
 * transfer configure
 *****************************************************************************/

/**
 * csp_i2c_set_fifo_thresh - set TX & RX FIFO threshold
 * @base_addr:	base address for I2CC
 * @tx_th:	TX_FIFO threshold
 * @rx_th:	RX_FIFO threshold
 */
void csp_i2c_set_fifo_thresh(u32 base_addr, u32 tx_th, u32 rx_th);

/**
 * csp_i2c_set_tar_addr - set the target address
 * @base_addr:	base address for I2CC
 * @addr:	target address (slave address to access)
 * @use_10bit:	use 10bit address
 */
void csp_i2c_set_tar_addr(u32 base_addr, u16 addr, u32 use_10bit);

/******************************************************************************
 * transfer function
 *****************************************************************************/

/**
 * csp_i2c_send_cmd - send read/write cmd to TX_FIFO
 * @base_addr:	base address for I2CC
 * @cmd:	transfer direction, I2C_CMD_WRITE or I2C_CMD_READ
 * @data:	byte to transfer if I2C_CMD_WRITE, or ignored
 * @stop:	whether to send STOP after this byte
 * @restart:	whether to send RESTART before this byte
 */
void csp_i2c_send_cmd(u32 base_addr, u32 cmd, u32 data,
			u32 stop, u32 restart);

/**
 * csp_i2c_get_rx_fl - get RX_FIFO fill level
 * @base_addr:	base address for I2CC
 *
 * return RX_FIFO fill level
 */

u32 csp_i2c_get_rx_fl(u32 base_addr);

/**
 * csp_i2c_read_byte - read one byte from RX_FIFO
 * @base_addr:	base address for I2CC
 *
 * return value of the read byte
 */
u8 csp_i2c_read_byte(u32 base_addr);

/**
 * csp_i2c_cfg_dma_rx - set dma rx threshold and rx length
 * @base_addr:	base address for I2CC
 * @rx_th:	dma rx threshold
 * @rx_len:	dma rx length
 */
void csp_i2c_cfg_dma_rx(u32 base_addr, u32 rx_th, u32 rx_len);

/**
 * csp_i2c_cfg_dma_tx - set dma tx threshold and tx length
 * @base_addr:	base address for I2CC
 * @tx_th:	dma tx threshold
 * @tx_len:	dma tx length
 */
void csp_i2c_cfg_dma_tx(u32 base_addr, u32 tx_th, u32 tx_len);

/**
 * csp_i2c_enable_dma_rx - whether to enable DMA receive
 * @base_addr:	base address for I2CC
 * @enable:	0, disable; 1, enable
 */
void csp_i2c_enable_dma_rx(u32 base_addr, u32 enable);

/**
 * csp_i2c_write_data - write one byte from master to slave
 * @base_addr:	base address for I2CC
 * @enable:	0, disable; 1, enable
 */
void csp_i2c_enable_dma_tx(u32 base_addr, u32 enable);

#endif /* ___I2C___CSP__H___ */
