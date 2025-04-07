/*
 * car_adas_lib.h - the Auxiliary Driving System of car lib head file
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

#ifndef _CAR_ADAS_LIB_H_
#define _CAR_ADAS_LIB_H_
#include "mod_adas.h"
#include "car_adas_set.h"

#ifdef __cplusplus
extern "C"
{
#endif
typedef void (*car_adas_callback)(void);

extern HDFrameSetData frame_set_data;
extern HDFrameGetData frame_get_data;
extern HDIniSetData init_set_data;

extern rt_sem_t sem_adas_data;
extern rt_sem_t sem_adas_proc;
extern rt_sem_t sem_adas_mutex;
extern int get_data_proc_thread_exit;
extern int set_data_proc_thread_exit;

int adas_lib_init(HDIniSetData *p_iniSetData);
int adas_lib_exit(void);
int adas_sem_init(void);
int adas_sem_exit(void);

#ifdef __cplusplus
};
#endif

#endif


