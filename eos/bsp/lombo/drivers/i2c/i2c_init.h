/*
 * i2c_init.c - I2C host driver code for LomboTech
 * i2c hardware init operation
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
#ifndef ___I2C___INIT__H___
#define ___I2C___INIT__H___

struct lombo_i2c;

/* 初始化I2C控制器 */
u32 i2c_init(struct lombo_i2c *i2c);

/* I2CC退出 */
void i2c_deinit(struct lombo_i2c *i2c);

#endif /* ___I2C___INIT__H___ */
