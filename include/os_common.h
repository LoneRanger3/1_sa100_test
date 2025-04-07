/*
 * os_common.h - system common interface head file
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

#ifndef __OS_COMMON_H__
#define __OS_COMMON_H__

#include <rtthread.h>
#include <rtdevice.h>


extern rt_int16_t g_sd_plugout;

extern rt_uint8_t os_getc(void);
extern rt_int32_t os_sinput_init(void);
extern void os_sinput_uninit(void);
extern rt_uint32_t os_time_get(void);
extern void os_delayms(rt_uint32_t ms);

extern int update_firmware(int argc, char **argv);
extern int get_update_state(void);

#endif

