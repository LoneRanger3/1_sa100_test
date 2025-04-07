/*
 * temp_sensor_drv.c - GPADC temperature sensor driver module
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

#include "gpadc_drv.h"
#include <debug.h>

#define TEMP_SENSOR_INDEX		0
#define EFUSE_OFFSET_ADDR		(0x01203800 + 0x18)
#define DEFAULT_OFFSET			22

int temp_sensor_get_val(void)
{
	int value = 0, temp_uint = 0;
	float temp = 0.0f;

	/* to do ... : read offset value from register */
	int offset = (READREG32(EFUSE_OFFSET_ADDR) >> 8) & 0x3FF;
	if (offset == 0) {
		LOG_D("read sensor offset value from register failed.");
		offset = DEFAULT_OFFSET;
	}

	value = csp_sensor_get_data(TEMP_SENSOR_INDEX);
	temp = 150 + 0.06459 * (offset - value);
	temp_uint = (int)temp;

	return temp_uint;
}

static void temp_log(void *param)
{
	while (1) {
		int temp = temp_sensor_get_val();
		LOG_D("temp_log -> temp: %d", temp);
		rt_thread_mdelay(2000);
	}
}

/* create thread to monitor temperature for debug */
void create_temp_monitor_thread()
{
	rt_thread_t thread;

	thread = rt_thread_create("temp_monitor_thread", temp_log,
				RT_NULL, 4096,
				INPUT_THREAD_PRIORITY, 5);
	if (thread)
		rt_thread_startup(thread);
}

int rt_hw_temp_sensor_init()
{
	csp_sensor_set_en(RT_TRUE);
	csp_sensor_set_sda_en(RT_TRUE);

	/* create_temp_monitor_thread(); */
	return 0;
}

#ifdef ARCH_LOMBO_N7
INIT_DEVICE_EXPORT(rt_hw_temp_sensor_init);
#endif

