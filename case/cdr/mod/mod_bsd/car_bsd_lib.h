/*
 * car_bsd_lib.h - the Auxiliary Driving System of car lib head file
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

#ifndef _CAR_BSD_LIB_H_
#define _CAR_BSD_LIB_H_
#include "mod_bsd.h"

#ifdef __cplusplus
extern "C"
{
#endif
typedef void (*car_bsd_callback)(void);

extern HDFrameSetBsd frame_set_data;
extern HDFrameGetBsd frame_get_data;
extern HDIniSetBsd init_set_data;

extern rt_sem_t sem_bsd_data;
extern rt_sem_t sem_bsd_proc;
extern rt_sem_t sem_bsd_mutex;
extern int get_data_proc_thread_exit;
extern int set_data_proc_thread_exit;

int bsd_lib_init(HDIniSetBsd *p_iniSetData);
int bsd_lib_exit(void);
int bsd_sem_init(void);
int bsd_sem_exit(void);

#ifdef __cplusplus
};
#endif

#endif


