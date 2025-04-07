/*
 * car_recorder_common.h - car recorder module head file
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

#ifndef __CAR_RECORDER_COMMON_H__
#define __CAR_RECORDER_COMMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eos.h"
#include "da380_gsensor.h"
#include "input/gps/gps_dev.h"
#include "dfs_posix.h"
#include "cJSON.h"
#include "system/system.h"
#include "system/system_mq.h"
#include "OMX_IVCommon.h"
#include "lb_recorder.h"
#include "vrender_component.h"
#include "lb_types.h"
#include "lb_common.h"
#include "lb_cfg_file.h"
#include "car_recorder_ctrl.h"

#define FRONT_RECORDER	0
#define REAR_RECORDER	1

#define SDCARD_PATH "/mnt/sdcard/"
#define REAR_PATH "/mnt/sdcard/R/"
#define REAR_LOCK_PATH "/mnt/sdcard/RO/"
#define FRONT_PATH "/mnt/sdcard/F/"
#define FRONT_LOCK_PATH "/mnt/sdcard/FO/"
#define PICTURE_PATH "/mnt/sdcard/PHOTO/"
// #define REAR_PATH "/mnt/sdcard/VIDEO_R/"
// #define REAR_LOCK_PATH "/mnt/sdcard/VIDEO_R_LOCK/"
// #define FRONT_PATH "/mnt/sdcard/VIDEO_F/"
// #define FRONT_LOCK_PATH "/mnt/sdcard/VIDEO_F_LOCK/"
// #define PICTURE_PATH "/mnt/sdcard/PICTURE/"
#define PICTURE_MAX_NUM 500
#define JIANHAO	10
#define MAOHAO	11
#define TEMPLE	12
#define XIEGANG	13
#define WATERMARK_SOURCE_NUM	15

#define SCREEN_W 320
#define SCREEN_H 1280

#define PANO_BIRDBIEW_SOURCE_W 320
#define PANO_BIRDBIEW_SOURCE_H 320

#define PANO_BIRDBIEW_WIND_X 0
#define PANO_BIRDBIEW_WIND_Y 70
#define PANO_BIRDBIEW_WIND_W 320
#define PANO_BIRDBIEW_WIND_H 320

#define PANO_PREVIEW_SOURCE_W 1280
#define PANO_PREVIEW_SOURCE_H 720

#define PANO_PREVIEW_WIND_X 0
#define PANO_PREVIEW_WIND_Y (PANO_BIRDBIEW_WIND_Y + PANO_BIRDBIEW_WIND_H)
#define PANO_PREVIEW_WIND_W SCREEN_W
#define PANO_PREVIEW_WIND_H (SCREEN_H - PANO_BIRDBIEW_WIND_Y * 2 - PANO_BIRDBIEW_WIND_H)

#define PANO_PREVIEW_CROP_X 0
#define PANO_PREVIEW_CROP_Y 0
#define PANO_PREVIEW_CROP_W ((SCREEN_W * SCREEN_H) / PANO_PREVIEW_WIND_H)
#define PANO_PREVIEW_CROP_H SCREEN_H

#define PANO_CARIMG_W 54
#define PANO_CARIMG_H 142

#define NORMAL_FILE_RATIO 6
#define LOCK_FILE_RATIO 4 //jiasuofenqu

#define PANO_OUT_BIN_PATH "mnt/data/cali_out.bin"

#define ORIGIN_CAR
#define ROTATE_CAR
#define SCALER_CAR
#define BIRD_VIEW_PATH "V:/image/02_cdr_bird_view.png.BGRA8888.ez"

extern char picture_rearfilename[64];
extern char picture_frontfilename[64];
extern win_para_t front_preview_para;
extern win_para_t rear_preview_para;
int lock_front_file_num;
int lock_rear_file_num;
int last_lockstatus;
int dialog_flag;
void *dialog_temp;
#define OPEN_FRONT_RECORDER
#define OPEN_REAR_RECORDER

#define GET_SD_STATUS 10
#define GET_AV_STATUS 11
#define GET_BACK_STATUS 12
#define GET_ACC_STATUS 13





#define MSG_FROM_HIDE_HBTB 0x800
#define MSG_FROM_SHOW_HBTB 0x900

extern rt_sem_t pano_sem; /* car pano status mutex */

#ifdef __cplusplus
extern "C" {
#endif


/* Micphone status  */
typedef enum _enable_e_ {
	ENABLE_OFF,
	ENABLE_ON,
	ENABLE_MAX
} enable_e;
/* views status  */
typedef enum _views_status_e_ {
	PIP_VIEW,
	FRONT_VIEW,
	BACK_VIEW,
	PANO_VIEW,
	VIEW_MAX
} views_status_e;

typedef enum _screen_rect_e_ {
	LEFT_HALF_SCREEN,
	RIGHR_HALF_SCREEN,
	FRONT_FULL_SCREEN,
	REAR_FULL_SCREEN,
	SMALL_SCREEN,
	PANO_BEFORE_PROC,
	PANO_AFTER_PROC,
	CUSTOM_SCREEN_SIZE,
	FULL_SCREEN_1080P,
} screen_rect_e;

/* record status  */
typedef enum _record_status_e_ {
	STOP,
	PAUSE,
	RECORDING,
	RECORD_MAX
} record_status_e;

typedef enum _recorder_status_ {
	RECORDER_STATUS_ERR = 0,
	RECORDER_STATUS_INIT,
	RECORDER_STATUS_PREPARED,
	RECORDER_STATUS_RECORD,
} recorder_status_t;

/* adas status  */
typedef enum _record_duration_e_ {
	DURATION_1MIN = 60,
	DURATION_2MIN = 120,
	DURATION_MAX
} record_duration_e;

typedef enum _record_resolution_e_ {
	RESOLUTION_720P,
	RESOLUTION_1080P,
	RESOLUTION_1296P,
	RESOLUTION_1440P,
	RESOLUTION_2K,
	RESOLUTION_MAX
} record_resolution_e;

typedef enum _gsensor_sensity_e_ {
	SENSITY_CLOSE = 0,
	HIGH_SENSITY = 1,
	MID_SENSITY = 3,
	LOW_SENSITY = 4,
	SENSITY_MAX
} gsensor_sensity_e;

typedef enum _level_e_ {
	LEVEL0 = 0,
	LEVEL1 = 1,
	LEVEL2 = 2,
	LEVEL3 = 3,
	LEVEL4 = 4,
	LEVEL5 = 5,
	LEVEL_MAX
} level_e;

typedef enum _adas_status_e_ {
	ADAS_CLOSE,
	ADAS_OPEN,
	ADAS_MAX
} adas_status_e;

typedef enum _bsd_status_e_ {
	BSD_CLOSE,
	BSD_OPEN,
	BSD_MAX
} bsd_status_e;

/* lock status  */
typedef enum _lock_status_e_ {
	LOCK_OFF,
	LOCK_ON,
	LOCK_STATUS_MAX
} lock_status_e;

/* dialog enum  */
typedef enum _dialog_e_ {
	DIALOG_RECORDER_FILE_FULL,
	DIALOG_LOCKFILE_FULL,
	DIALOG_NEED_PLUGIN_SDCARD,
	DIALOG_PARK_MONITOR_EVENT,
	DIALOG_NEED_PLUGIN_AV,
	DIALOG_PLEASE_FORMAT_SDCARD,
	DIALOG_LOCK_RECORDER_FILE,
	DIALOG_UNLOCK_RECORDER_FILE,
	DIALOG_MAX
} dialog_e;
typedef enum _sd_status_e_ {
	SDCARD_NOT_PLUGIN,
	SDCARD_PLUGIN,
	SDCARD_NOT_FORMAT,
	SDCARD_MAX
} sd_status_e;

typedef enum _record_fps_mode_e_ {
	REC_FPS_NORMAL,
	REC_FPS_INTERVAl,
	REC_FPS_MAX
} record_fps_mode_e;

typedef struct cr_status_ {
	record_status_e recording_s;
	record_duration_e  record_duration;
	record_resolution_e record_resolution;
	enable_e mute_enable;
	enable_e warn_tone_enable;
	enable_e watermark_time_enable;
	enable_e watermark_logo_enable;
	enable_e park_monitor;
	enable_e interval_record;
	int gsensor_sensity;
	int bsd_alarm_speed;
	views_status_e views_s;
	lock_status_e lock_s;
	int av_status;
	int back_status;
	enable_e adas_enable;
	enable_e bsd_enable;
	enable_e pano_enable;
	int front_cropx;
	int rear_cropx;
	int rearmirr_enable;
	int lcd_brightness;
} cr_status_t;

typedef struct cr_jobj_ {
	cJSON *cfg_root;
	cJSON *system_root;
	cJSON *record_root;
	cJSON *adas_root;
	cJSON *bsd_root;
	cJSON *pano_root;
	cJSON *record_duration;
	cJSON *record_resolution;
	cJSON *mute_enable;
	cJSON *watermark_time_enable;
	cJSON *watermark_logo_enable;
	cJSON *park_monitor;
	cJSON *interval_record;
	cJSON *park_monitor_times;
	cJSON *gsensor_sensity;
	cJSON *bsd_alarm_speed;
	cJSON *adas_enable;
	cJSON *bsd_enable;
	cJSON *pano_enable;
	cJSON *front_cropx;
	cJSON *rear_cropx;
	cJSON *rearmirr_enable;
	cJSON *lcd_brightness;
	cJSON *warn_tone_enable;
} cr_jobj_t;
typedef struct cdr_comp_thread_manager {
	pthread_t thread_id; /* recorder file manager thread id */
	int com_flag;
	int record_mod;
	rec_time_lag_para_t rec_time_lag_para;
	void *hd;
} cdr_comp_thread_manager_t;

extern cdr_comp_thread_manager_t cdr_comp_open_rear;
extern cdr_comp_thread_manager_t cdr_comp_close_rear;
extern cdr_comp_thread_manager_t cdr_comp_open_front;
extern cdr_comp_thread_manager_t cdr_comp_close_front;
extern cdr_comp_thread_manager_t cdr_comp_start_adas;
extern cdr_comp_thread_manager_t cdr_comp_stop_adas;
extern cdr_comp_thread_manager_t cdr_comp_start_bsd;
extern cdr_comp_thread_manager_t cdr_comp_stop_bsd;

typedef struct {
	lb_uint32 cf           : 5;   /* Color format */
	lb_uint32 always_zero  : 3;   /* It the upper bits of the first byte */

	lb_uint32 reserved     : 2;  /* Reserved to be used later */

	lb_uint32 w : 11;             /* Width of the image map */
	lb_uint32 h : 11;             /* Height of the image map */
} img_header_t;

typedef struct {
	img_header_t header;
	lb_uint32 data_size;
	lb_uint8 *data;
} img_dsc_t;

typedef struct pano_set_ {
	float motor_w;
	float motor_l;
	int motor_d2r;
} pano_set_t;

typedef struct pano_out_ {
	lb_int32 use_ext_cutline;
	cali_contex_t cali_ctx;
	cali_out_data_t cali_out;
	pano_set_t set;
	dc_win_data_t win_data;
	disp_ctl_t *disp_ctl;
	rt_device_t disp_device;
	rt_device_disp_ops_t *disp_ops;
	img_dsc_t *car_img;
	disp_rot_cfg_t rot_cfg;
	lb_uint8 *rot_buf;
	lb_uint8 *sca_buf;
	lb_uint8 *ori_buf;
} pano_out_t;

lb_int32 cr_cfg_init(void);

lb_int32 cr_cfg_save(void);
lb_int32 cr_cfg_save_r(void);

lb_int32 cr_cfg_exit(void);

void car_comp_release(void);

lb_int32 cdr_comp_thread_init(cdr_comp_thread_manager_t *cdr_comp);

lb_int32 get_recorder_duration(void);

lb_int32 get_recorder_resolution(void);

lb_int32 get_watermark_time_enable(void);

lb_int32 get_watermark_logo_enable(void);

lb_int32 get_park_monitor_enable(void);

lb_int32 get_interval_record_enable(void);

lb_int32 get_rearmirr_enable(void);

lb_int32 get_cfg_park_monitor_times(void);

void park_monitor_times_clear(void);

enable_e get_mute_enable(void);

void set_mute_enable(enable_e value);

enable_e get_mute_enable(void);

void set_mute_enable(enable_e value);

views_status_e get_views_status(void);

void set_views_status(views_status_e value);

lock_status_e get_lock_status(void);

void set_lock_status(lock_status_e value);

int get_av_status(void);

int get_back_status(void);

int set_back_status(int value);

int get_acc_status(void);

int get_front_cropx(void);

int set_front_cropx(int value);

int get_rear_cropx(void);

int set_rear_cropx(int value);

int get_sd_status();

int get_gsensor_sensity(void);

gsensor_sensity_e get_gsensor_sensity_map(void);

int get_bsd_alarm_speed_map(void);

int recoder_enable_mute(void *recorder_hd, int mute_flag);

int cr_dir_init();

u32 _get_disk_info(char *path, u64 *used_mbsize, u64 *free_mbsize);

int get_dir_lockfilesize(char *path, u64 *used_mbsize);

u64 get_lock_mbsize(void);

int rm_over_file(char *path, int max_num, char *suffix_name);

void get_screen_info(screen_info_t *cdr_screen);

void *car_recorder_init(char *video_source, cdr_comp_thread_manager_t *cdr_comp);

int car_recorder_exit(void *recorder_hd);

int preview_start(void *recorder_hd);

int preview_stop(void *recorder_hd);

int recorder_start_front(void *recorder_hd);

int recorder_start_rear(void *recorder_hd);

int recoder_stop(void *recorder_hd);

int get_recorder_time(void *recorder_hd);

int get_recorder_status(void *recorder_hd);

int tack_picture(int recoder_flag, struct tm *p_tm);

int watermark_set_source(void *rec, unsigned int source_num, char source_name[][64]);

int watermark_exit_set_source(unsigned int source_num);

int watermark_time_fd(void *rec, struct tm *p_tm);
int watermark_time_rd(void *rec, struct tm *p_tm);


int watermark_logo(void *rec, int index);

int pano_get_enable(void);

int adas_get_enable();

int bsd_get_enable();

int pano_start(void *recorder_hd);

int pano_stop(void *recorder_hd);

int pano_check(void);

int recorder_set_views(void *recorder_fd, void *recorder_rd, views_status_e value);

int _get_file_exist_indir_(char *path);

int move_cdr_disp_para(void *recorder_hd, win_para_t *cdr_disp_para, int num);

int adas_get_status(void);

int pano_get_status(void);

int bsd_get_status(void);

int rename_rec(void *hdl, char *path, char *next_file);

void *file_mgr_create(int f_bitrate, int r_bitrate, int time);

int file_mgr_resize(void *hdl, int f_bitrate, int r_bitrate, int time);

void file_mgr_wakeup(void *hdl, int time);

int file_mgr_get_rear_file(void *hdl, char *file);

int file_mgr_get_front_file(void *hdl, char *file);

int file_mgr_front_wait(void *hdl, int timeout_ms);

int file_mgr_rear_wait(void *hdl, int timeout_ms);
#if 1//jiasuofenqu
int file_mgr_judge_lock_memery(void);
#endif
int fallocate_front_file(void *hdl, char *file);

int fallocate_rear_file(void *hdl, char *file);

int truncate_file(void *hdl, char *file, int recoder_flag);

void file_mgr_destory(void *hdl);
lb_int32 get_backlight_value(void);
lb_int32 backlight_value_to_idx(lb_uint32 value);
void set_backlight_value(lb_uint32 idx);

#ifdef LOMBO_GPS
int  get_gps_speed(struct gps_data_t *g);
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __CAR_RECORDER_COMMON_H__ */

