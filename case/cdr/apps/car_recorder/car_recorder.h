/*
 * car_recorder.h - car recorder head file
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

#ifndef __CAR_RECORDER_H__
#define __CAR_RECORDER_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <pthread.h>
#include "app_manage.h"
#include "mod_manage.h"
#include "mod_adas.h"
#include "mod_bsd.h"
#include "case_config.h"

#define APP_ENTER_BACKGROUND 1
#define APP_SET_ATTR 2
#define CR_GET_RECORDER_STATUS 3
#define RECORDER_EXIT 4
#define CDR_FS_PART_UNMOUNT_PREPARE 5
#define CDR_MUTEX 6
#define CDR_UNMUTEX 7
#define HIGH_REC_BITRATE 16000000
#define LOW_REC_BITRATE 6000000

typedef struct screen_info_ {
	int width;
	int height;
	int front_max_crop_x;
	int front_max_crop_y;
	int rear_max_crop_x;
	int rear_max_crop_y;
} screen_info_t;

extern pthread_mutex_t cr_mutex;
extern pthread_mutex_t cr_front_mutex;
extern pthread_mutex_t cr_rear_mutex;
extern pthread_mutex_t cr_adas_mutex;
extern pthread_mutex_t cr_bsd_mutex; /* car recorder bsd mutex */

extern void *recorder_rd; /* rear recoder handle */
extern void *recorder_fd; /* front recoder handle */
extern screen_info_t cdr_screen;
extern int click_time;

extern void *car_file_mgr;
extern rt_sem_t cdr_status_thread_sem; /* cdr status thread sem */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __CAR_RECORDER_H__ */
