/*
 * rtl8189.c - realtek rtl8189ftv wireless driver
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

#include <rtthread.h>
#include "rtl8189.h"

#define DBG_SECTION_NAME	"rtl8189"
#define DBG_LEVEL		DBG_INFO
#include <debug.h>

/* Customer function to control HW specific wlan gpios */
void Set_WLAN_Power_On(void)
{
	LOG_I("enter");
}

/* Customer function to control HW specific wlan gpios */
void Set_WLAN_Power_Off(void)
{
	LOG_I("enter");
}

int rtl8189_init(void)
{
	int ret;

	/* param: 0 - station, 1 - softap */
	ret = rtl_wlan_drv_init(1);
	LOG_D("init return %d", ret);

	return ret;
}
INIT_APP_EXPORT(rtl8189_init);
