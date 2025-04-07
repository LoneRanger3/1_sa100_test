/*
 * da380_gsensor.h - Gsensor module realization
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

#ifndef __DA380_GSENSOR_H__
#define __DA380_GSENSOR_H__

#include <rtthread.h>
#include <rtdevice.h>
#include <csp.h>

/* gsensor setting parameters */
#define GS_MEASURE_RANGE_2G		1
#define GS_MEASURE_RANGE_4G		2
#define GS_MEASURE_RANGE_8G		3
#define GS_MEASURE_RANGE_16G		4

#define GS_ACC_X			1
#define GS_ACC_Y			2
#define GS_ACC_Z			3

#define GS_MODE_NORMAL			1
#define GS_MODE_LOW_POWER		2
#define GS_MODE_SUSPEND			3

#define GS_MOTION_FLAG_ORIENT		1
#define GS_MOTION_FLAG_S_TAP		2
#define GS_MOTION_FLAG_D_TAP		3
#define GS_MOTION_FLAG_ACTIVE		4
#define GS_MOTION_FLAG_FREEFALL		5

#define GS_AXIS_DISABLE_X		1
#define GS_AXIS_DISABLE_Y		2
#define GS_AXIS_DISABLE_Z		3

#define GS_ORIENT_INT_EN		1
#define GS_S_TAP_INT_EN			2
#define GS_D_TAP_INT_EN			3
#define GS_ACTIVE_INT_EN_Z		4
#define GS_ACTIVE_INT_EN_Y		5
#define GS_ACTIVE_INT_EN_X		6
#define GS_NEW_DATA_INT_EN		7
#define GS_FREEFALL_INT_EN		8

#define GS_INT_ORIENT			1
#define GS_INT_S_TAP			2
#define GS_INT_D_TAP			3
#define GS_INT_ACTIVE			4
#define GS_INT_FREEFALL			5
#define GS_INT_NEW_DATA			6

rt_err_t gsensor_set_measure_range(int type);
rt_err_t gsensor_set_pwr_mode(int type);
rt_err_t gsensor_set_axis_disable(int type, rt_bool_t disable);
rt_err_t gsensor_set_int_en(int type, rt_bool_t en);
rt_err_t gsensor_set_int_map(int type, rt_bool_t mp);
rt_err_t gsensor_get_motion_flag(int type, u8 *flag);
s16 gsensor_get_acc(int type);
rt_err_t gsensor_soft_reset();
void gsensor_clr_int_status();
void test_gsensor_acc_data();
void gsensor_dump();

/* open gsensor interrupt */
void gsensor_open_monitor();

/* close gsensor interrupt */
void gsensor_close_monitor();

/* set gsensor park monitor config */
void gsensor_set_park_monitor_cfg(rt_bool_t open);

/* get gsensor park monitor config */
int gsensor_get_park_monitor_cfg();

#endif /* __DA380_GSENSOR_H__ */

