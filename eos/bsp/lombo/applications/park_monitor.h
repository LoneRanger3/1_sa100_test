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

#ifndef __PM_MONITOR_H__
#define __PM_MONITOR_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "pthread.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dfs_posix.h"
#include "lb_types.h"
#include "cJSON.h"
#include "OMX_IVCommon.h"
#include "lb_recorder.h"
#include "vrender_component.h"
#include "system/system.h"
#include "system/system_mq.h"
#include "da380_gsensor.h"
#include "lb_cfg_file.h"
#include "power_drv.h"
#ifdef ARCH_LOMBO_N7V1_TDR
#include "../../../../case/tdr/includes/case_config.h"
#else
#include "../../../../case/cdr/includes/case_config.h"
#endif

#ifdef RT_USING_EGUI
#include "lb_common.h"
#endif

#define PM_FRONT_RECORDER	0
#define PM_REAR_RECORDER	1
#define PM_OPEN_FRONT_RECORDER
#define PM_OPEN_REAR_RECORDER

#define PM_SDCARD_PATH "/mnt/sdcard/"
// #define PM_REAR_LOCK_PATH "/mnt/sdcard/VIDEO_R_LOCK/"
// #define PM_FRONT_LOCK_PATH "/mnt/sdcard/VIDEO_F_LOCK/"
// #define PM_REAR_PATH "/mnt/sdcard/VIDEO_R/"
// #define PM_FRONT_PATH "/mnt/sdcard/VIDEO_F/"
#define PM_REAR_LOCK_PATH "/mnt/sdcard/RO/"
#define PM_FRONT_LOCK_PATH "/mnt/sdcard/FO/"
#define PM_REAR_PATH "/mnt/sdcard/R/"
#define PM_FRONT_PATH "/mnt/sdcard/F/"

#define PM_JIANHAO	10
#define PM_MAOHAO	11
#define PM_TEMPLE	12
#define PM_XIEGANG	13
#define PM_WATERMARK_SOURCE_NUM	15

int pm_car_recorder_finish;

void *pm_recorder_rd; /* rear recoder handle */
void *pm_recorder_fd; /* front recoder handle */

#define PM_CDR_STATUS_THREAD_PRIORITY	24
#define PM_CDR_STATUS_STACK_SIZE		4096
#define PM_DURATION 15

#define PM_HIGH_REC_BITRATE 16000000
#define PM_LOW_REC_BITRATE 6000000

#define IO_FALLOCATE	0xfa
#ifdef ARCH_LOMBO_N7V1_TDR
#define FALLOC_RESERVE		(9*1024*1024)
#else
#define FALLOC_RESERVE		(15*1024*1024)
#endif
#define get_falloc_size(bitrate, time) ((bitrate >> 3) * (time) + FALLOC_RESERVE)
#define falloc_align(n) (((n + (1<<20) - 1) >> 20) << 20)

typedef enum _pm_recorder_status_ {
	PM_RECORDER_STATUS_ERR = 0,
	PM_RECORDER_STATUS_INIT,
	PM_RECORDER_STATUS_PREPARED,
	PM_RECORDER_STATUS_RECORD,
} pm_recorder_status_t;

typedef struct pm_cr_status_ {
	int av_status;
	int sd_status;
} pm_cr_status_t;

typedef struct pm_cr_jobj_ {
	cJSON *cfg_root;
	cJSON *record_root;
	cJSON *park_monitor;
	cJSON *park_monitor_times;
} pm_cr_jobj_t;

typedef struct {
	lb_uint32 cf           : 5;   /* Color format */
	lb_uint32 always_zero  : 3;   /* It the upper bits of the first byte */

	lb_uint32 reserved     : 2;  /* Reserved to be used later */

	lb_uint32 w : 11;             /* Width of the image map */
	lb_uint32 h : 11;             /* Height of the image map */
} pm_img_header_t;

typedef struct {
	pm_img_header_t header;
	lb_uint32 data_size;
	lb_uint8 *data;
} pm_img_dsc_t;

lb_int32 pm_cdr_status_thread_init(pthread_t *thread_id);

int pm_get_av_status(void);
void pm_set_av_status(int value);
int pm_get_sd_status();
void pm_set_sd_status(int value);
void pm_cr_dir_init();
u32 pm_get_disk_info(char *path, u64 *used_mbsize, u64 *free_mbsize);
int pm_get_dir_lockfilesize(char *path, u64 *used_mbsize);
u64 pm_get_lock_mbsize(void);
int pm_car_recorder_exit(void *recorder_hd);
int pm_recorder_start_front(void *recorder_hd);
int pm_recorder_start_rear(void *recorder_hd);
int pm_recoder_stop(void *recorder_hd);
int pm_get_recorder_time(void *recorder_hd);
int pm_get_recorder_status(void *recorder_hd);
int pm_watermark_set_source(void *rec, unsigned int source_num,
	char source_name[][64]);
int pm_watermark_exit_set_source(unsigned int source_num);
int pm_watermark_time(void *rec, struct tm *p_tm);
int pm_watermark_logo(void *rec, int index);
char *pm_get_first_file(char *path);
void *pm_car_recorder_init(char *video_source);
void park_monitor_create(void);
lb_int32 park_monitor_stop(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
