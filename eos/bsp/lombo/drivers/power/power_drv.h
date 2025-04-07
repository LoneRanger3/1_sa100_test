/*
 * power_drv.h - head file for power driver
 *
 * Copyright (C) 2016-2019, LomboTech Co.Ltd.
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
#ifndef __POWER_DRV_H__
#define __POWER_DRV_H__

#include "input.h"
#include <pthread.h>
#include "system/system_mq.h"

typedef enum {
	BATTERY_CHARGE = 0,	/* battery charging status */
	BATTERY_LEVEL_1,	/* low power */
	BATTERY_LEVEL_2,
	BATTERY_LEVEL_3,	/* full power */
	BATTERY_LEVEL_4,	/* high power */
} BAT_STATUS_TYPE;
#ifdef ARCH_LOMBO_N7V1_CDR
rt_sem_t gsensor_det_sem;
#endif
void power_key_handle(KEY_STATUS_TYPE type);
void power_connect();
void power_disconnect();
void power_mark_gs_boot();
void gsensor_irq_handle();
void power_mark_gs_boot();
void usb_connect_status_judge();

/* get battery level */
BAT_STATUS_TYPE get_bat_level();
int is_gs_wakeup_boot();
void rt_hw_power_disable_reset(void);
void setup_sio5_wake_en(u32 enable);
int get_acc_sio_val();
void set_acc_line(int able);

#endif /* __POWER_DRV_H__ */
