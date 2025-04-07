/*
 * i2c_drv.h - I2C host driver code for LomboTech
 * i2c driver structure and interface
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
#ifndef ___I2C___DRV__H___
#define ___I2C___DRV__H___

#include <rtdevice.h>

#define I2C_HS_MASTERCODE_FREQ		400000
#define I2C_LCNT_MIN			6

struct lombo_i2c {
	u32			idx;

	clk_handle_t		clk_gate;
	clk_handle_t		clk_reset;
	clk_handle_t		clk_i2c;
	clk_handle_t		clk_parent;

	u32			baudrate;	/* the baudrate of the bus */

	u32			ic_clkrate;	/* frequence of ic_clk */
	u32			actual_rate;	/* actual baudrate of the bus */
	u32			speed_mode;

	u32			fs_spk_len;	/* FS mode max spike length */
	u32			hs_spk_len;	/* HS mode max spike length */
	/* to adjust the high period of SCL */
	u32			h_adj_cnt;
	/* to adjust the low period of SCL */
	u32			l_adj_cnt;
	/* start address of the msgs sequence */
	struct rt_i2c_msg	*msgs;
	/* number of the msgs sequence */
	u32			msg_num;
	/* the current msg to send cmd */
	u32			msg_send;
	/* the msgs before it have already been read */
	u32			msg_read;
	/* the current byte of the current msg to send cmd */
	u32			msg_byte_send;
	/* he current byte of the current msg to be read */
	u32			msg_byte_read;
	/* count of the bytes need to read */
	u32			msg_pd_rd_cnt;
	int			msg_err;	/* Error number */

	u32			retries;

	struct rt_completion	completion;

	unsigned int		irq;		/* irq number of the I2CC */

	void			*base;		/* base address of the I2CC */

	spinlock_t		lock;

	u32			imask;
	u32			int_status;
	u32			abrtsrc;

	/* driver module device structure */
	struct rt_i2c_bus_device	*i2c_bus_dev;
};

int rt_hw_i2c_init(void);

#endif /* ___I2C___DRV__H___ */
