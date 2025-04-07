/*
 * disp_i2c.h - disp i2c head file
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

#ifndef __DISP_I2C_H__
#define __DISP_I2C_H__

struct disp_i2c_client {
	struct rt_i2c_bus_device *i2c_bus;
	rt_uint8_t i2c_addr;
};

struct disp_dbg_register {
	rt_uint32_t size;     /* register size in bytes */
	rt_uint64_t add;
	rt_uint64_t val;
};

rt_err_t disp_i2c_read_reg_8bit(struct disp_i2c_client *client,
	rt_uint8_t reg, rt_uint8_t *value);
rt_err_t disp_i2c_read_buffer_8bit(struct disp_i2c_client *client,
	rt_uint8_t reg, rt_uint8_t *value, rt_size_t size);
rt_err_t disp_i2c_write_reg_8bit(struct disp_i2c_client *client,
	rt_uint8_t reg, rt_uint8_t value);

rt_err_t disp_i2c_read_reg_16bit(struct disp_i2c_client *client,
	rt_uint16_t reg, rt_uint8_t *value);
rt_err_t disp_i2c_write_reg_16bit(struct disp_i2c_client *client,
	rt_uint16_t reg, rt_uint8_t value);

rt_err_t disp_i2c_write_reg_buf(struct disp_i2c_client *client,
	char *buf, int count);
#endif /* __DISP_I2C_H__ */
