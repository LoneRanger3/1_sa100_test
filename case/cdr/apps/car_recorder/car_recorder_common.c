/*
 * car_recorder_common.c - car recorder module access api
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

#include <pthread.h>
#include "eos.h"
#include "cJSON.h"
#include "car_recorder.h"
#include "car_recorder_common.h"
#include "lb_cfg_file.h"
#include "lb_types.h"
#include "case_common.h"
#include "../home/home.h"

win_para_t front_preview_para;
win_para_t rear_preview_para;

static mod_t g_mod_adas;
static mod_t g_mod_bsd;

static cr_status_t cr_status; /* car recorder status struct */
static cr_jobj_t cr_cfg_jobj; /* car recorder json object struct */

char recorder_rearfilename[64];
char recorder_frontfilename[64];

char f_lock_filename[64];
char r_lock_filename[64];

char picture_rearfilename[64];
char picture_frontfilename[64];

static pano_out_t pano_out;
static int pano_status;

numeral_input_t wm_source;
img_dsc_t *p_img[WATERMARK_SOURCE_NUM];
void *car_file_mgr;
adas_result_callback_t adas_cb;
bsd_result_callback_t bsd_cb;


cdr_comp_thread_manager_t cdr_comp_open_rear;
cdr_comp_thread_manager_t cdr_comp_close_rear;
cdr_comp_thread_manager_t cdr_comp_open_front;
cdr_comp_thread_manager_t cdr_comp_close_front;
cdr_comp_thread_manager_t cdr_comp_start_adas;
cdr_comp_thread_manager_t cdr_comp_stop_adas;
cdr_comp_thread_manager_t cdr_comp_start_bsd;
cdr_comp_thread_manager_t cdr_comp_stop_bsd;

static fix_duration_param_t fix_duration_rear;
static fix_duration_param_t fix_duration_front;
char watermark_filename[WATERMARK_SOURCE_NUM][64] = {
	"V:/image/num0.png.BGRA8888.ez",
	"V:/image/num1.png.BGRA8888.ez",
	"V:/image/num2.png.BGRA8888.ez",
	"V:/image/num3.png.BGRA8888.ez",
	"V:/image/num4.png.BGRA8888.ez",
	"V:/image/num5.png.BGRA8888.ez",
	"V:/image/num6.png.BGRA8888.ez",
	"V:/image/num7.png.BGRA8888.ez",
	"V:/image/num8.png.BGRA8888.ez",
	"V:/image/num9.png.BGRA8888.ez",
	"V:/image/jianhao.png.BGRA8888.ez",
	"V:/image/maohao.png.BGRA8888.ez",
	"V:/image/temple.png.BGRA8888.ez",
	"V:/image/xiegang.png.BGRA8888.ez",
	"V:/image/wm_logo.png.BGRA8888.ez"
};

lb_int32 rec_resolution=0;



/**
 * cr_config_array_init - get config parameter from config array
 * @para: Json array
 *
 * This function use to init config parameter from config array
 *
 */
static void cr_config_array_init(cJSON *para)
{
	lb_int32 i = 0;

	for (i = 0; i < cJSON_GetArraySize(para); i++) {
		cJSON *temp;

		temp = cJSON_GetArrayItem(para, i);
		if (temp && temp->type == cJSON_Object) {
			if (cJSON_GetObjectItem(temp, "record_duration")) {
				cr_cfg_jobj.record_duration = cJSON_GetObjectItem(temp,
						"record_duration");
				cr_status.record_duration =
					cr_cfg_jobj.record_duration->valueint;
				APP_LOG_D("cr_status.record_duration:%d\n",
					cr_status.record_duration);
			} else if (cJSON_GetObjectItem(temp, "record_resolution")) {
				cr_cfg_jobj.record_resolution = cJSON_GetObjectItem(temp,
						"record_resolution");
				cr_status.record_resolution =
					cr_cfg_jobj.record_resolution->valueint;
				APP_LOG_D("cr_status.record_resolution:%d\n",
					cr_status.record_resolution);
			} else if (cJSON_GetObjectItem(temp, "mute_enable")) {
				cr_cfg_jobj.mute_enable = cJSON_GetObjectItem(temp,
						"mute_enable");
				cr_status.mute_enable = cr_cfg_jobj.mute_enable->valueint;
				APP_LOG_D("cr_status.mute_enable:%d\n",
					cr_status.mute_enable);
			} else if (cJSON_GetObjectItem(temp, "watermark_time_enable")) {
				cr_cfg_jobj.watermark_time_enable =
					cJSON_GetObjectItem(temp,
						"watermark_time_enable");
				cr_status.watermark_time_enable =
					cr_cfg_jobj.watermark_time_enable->valueint;
				APP_LOG_D("cr_status.watermark_time_enable:%d\n",
					cr_status.watermark_time_enable);
			} else if (cJSON_GetObjectItem(temp, "watermark_logo_enable")) {
				cr_cfg_jobj.watermark_logo_enable =
					cJSON_GetObjectItem(temp,
						"watermark_logo_enable");
				cr_status.watermark_logo_enable =
					cr_cfg_jobj.watermark_logo_enable->valueint;
				APP_LOG_D("cr_status.watermark_logo_enable:%d\n",
					cr_status.watermark_logo_enable);
			} else if (cJSON_GetObjectItem(temp, "park_monitor")) {
				cr_cfg_jobj.park_monitor = cJSON_GetObjectItem(temp,
						"park_monitor");
				cr_status.park_monitor =
					cr_cfg_jobj.park_monitor->valueint;
				APP_LOG_D("cr_status.park_monitor:%d\n",
					cr_status.park_monitor);
			} else if (cJSON_GetObjectItem(temp, "interval_record")) {
				cr_cfg_jobj.interval_record = cJSON_GetObjectItem(temp,
						"interval_record");
				cr_status.interval_record =
					cr_cfg_jobj.interval_record->valueint;
				APP_LOG_D("cr_status.interval_record:%d\n",
					cr_status.interval_record);
			} else if (cJSON_GetObjectItem(temp, "gsensor_sensity")) {
				cr_cfg_jobj.gsensor_sensity = cJSON_GetObjectItem(temp,
						"gsensor_sensity");
				cr_status.gsensor_sensity =
					cr_cfg_jobj.gsensor_sensity->valueint;
				APP_LOG_D("cr_status.gsensor_sensity:%d\n",
					cr_status.gsensor_sensity);
			} else if (cJSON_GetObjectItem(temp, "rswlevel")) {
				cr_cfg_jobj.bsd_alarm_speed = cJSON_GetObjectItem(temp,
					"rswlevel");
				cr_status.bsd_alarm_speed =
					cr_cfg_jobj.bsd_alarm_speed->valueint;
				APP_LOG_D("cr_status.bsd_alarm_speed:%d\n",
					cr_status.bsd_alarm_speed);
			} else if (cJSON_GetObjectItem(temp, "park_monitor_times")) {
				cr_cfg_jobj.park_monitor_times = cJSON_GetObjectItem(temp,
						"park_monitor_times");
				APP_LOG_D("cr_cfg_jobj.park_monitor_times:%d\n",
					cr_cfg_jobj.park_monitor_times->valueint);
			} else if (cJSON_GetObjectItem(temp, "adas_enable")) {
				cr_cfg_jobj.adas_enable = cJSON_GetObjectItem(temp,
						"adas_enable");
				APP_LOG_D("cr_cfg_jobj.adas_enable:%d\n",
					cr_cfg_jobj.adas_enable->valueint);
				cr_status.adas_enable = cr_cfg_jobj.adas_enable->valueint;
			} else if (cJSON_GetObjectItem(temp, "bsd_enable")) {
				cr_cfg_jobj.bsd_enable = cJSON_GetObjectItem(temp,
						"bsd_enable");
				APP_LOG_D("cr_cfg_jobj.bsd_enable:%d\n",
					cr_cfg_jobj.bsd_enable->valueint);
				cr_status.bsd_enable = cr_cfg_jobj.bsd_enable->valueint;
			} else if (cJSON_GetObjectItem(temp, "pano_enable")) {
				cr_cfg_jobj.pano_enable = cJSON_GetObjectItem(temp,
						"pano_enable");
				APP_LOG_D("cr_cfg_jobj.pano_enable:%d\n",
					cr_cfg_jobj.pano_enable->valueint);
				cr_status.pano_enable = cr_cfg_jobj.pano_enable->valueint;
			} else if (cJSON_GetObjectItem(temp, "front_cropx")) {
				cr_cfg_jobj.front_cropx = cJSON_GetObjectItem(temp,
						"front_cropx");
				APP_LOG_D("cr_cfg_jobj.front_cropx:%d\n",
					cr_cfg_jobj.front_cropx->valueint);
				cr_status.front_cropx = cr_cfg_jobj.front_cropx->valueint;
			} else if (cJSON_GetObjectItem(temp, "rear_cropx")) {
				cr_cfg_jobj.rear_cropx = cJSON_GetObjectItem(temp,
						"rear_cropx");
				APP_LOG_D("cr_cfg_jobj.rear_cropx:%d\n",
					cr_cfg_jobj.rear_cropx->valueint);
				cr_status.rear_cropx = cr_cfg_jobj.rear_cropx->valueint;
			} else if (cJSON_GetObjectItem(temp, "rearmirr_enable")) {
				cr_cfg_jobj.rearmirr_enable = cJSON_GetObjectItem(temp,
						"rearmirr_enable");
				APP_LOG_D("cr_cfg_jobj.rearmirr_enable:%d\n",
					cr_cfg_jobj.rearmirr_enable->valueint);
				cr_status.rearmirr_enable =
					cr_cfg_jobj.rearmirr_enable->valueint;
			}else if (cJSON_GetObjectItem(temp, "lcd_brightness")) {
				cr_cfg_jobj.lcd_brightness = cJSON_GetObjectItem(temp,
						"lcd_brightness");
				APP_LOG_D("cr_cfg_jobj.lcd_brightness:%d\n",
					cr_cfg_jobj.lcd_brightness->valueint);
			}else if (cJSON_GetObjectItem(temp, "warn_tone")) {
				cr_cfg_jobj.warn_tone_enable = cJSON_GetObjectItem(temp,
						"warn_tone");
				APP_LOG_D("cr_cfg_jobj.warn_tone:%d\n",
					cr_cfg_jobj.warn_tone_enable->valueint);
				cr_status.warn_tone_enable =
					cr_cfg_jobj.warn_tone_enable->valueint;
			}

		}
	}
}

/**
 * cr_cfg_init - get config parameter from file
 *
 * This function use to init config parameter from config file
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
lb_int32 cr_cfg_init(void)
{
	lb_int32 ret = 0;

	APP_LOG_D("\n");
#ifdef ARCH_LOMBO_N7_CDR_MMC
	cr_cfg_jobj.cfg_root = lb_open_cfg_file("/mnt/sdcard/etc/cdr_config1.cfg");
#else
	cr_cfg_jobj.cfg_root = lb_open_cfg_file("/mnt/data/cdr_config.cfg");
#endif
	if (cr_cfg_jobj.cfg_root == NULL) {
		cr_cfg_jobj.cfg_root =
			lb_open_cfg_file(ROOTFS_MOUNT_PATH"/etc/cdr_config.cfg");
		if (cr_cfg_jobj.cfg_root == NULL) {
			APP_LOG_E("cr_cfg_jobj.cfg_root is NULL");
			return -1;
		}
	}
	RT_ASSERT(cr_cfg_jobj.cfg_root != NULL)

	cr_cfg_jobj.system_root = cJSON_GetObjectItem(cr_cfg_jobj.cfg_root, "system");
	RT_ASSERT(cr_cfg_jobj.system_root != NULL)

	if (cr_cfg_jobj.system_root && cr_cfg_jobj.system_root->type == cJSON_Array)
		cr_config_array_init(cr_cfg_jobj.system_root);

	cr_cfg_jobj.record_root = cJSON_GetObjectItem(cr_cfg_jobj.cfg_root, "record");
	RT_ASSERT(cr_cfg_jobj.record_root != NULL)

	if (cr_cfg_jobj.record_root && cr_cfg_jobj.record_root->type == cJSON_Array)
		cr_config_array_init(cr_cfg_jobj.record_root);
	cr_cfg_jobj.adas_root = cJSON_GetObjectItem(cr_cfg_jobj.cfg_root, "car_adas");
	RT_ASSERT(cr_cfg_jobj.adas_root != NULL)

	if (cr_cfg_jobj.adas_root && cr_cfg_jobj.adas_root->type == cJSON_Array)
		cr_config_array_init(cr_cfg_jobj.adas_root);

	cr_cfg_jobj.bsd_root = cJSON_GetObjectItem(cr_cfg_jobj.cfg_root, "car_bsd");
	RT_ASSERT(cr_cfg_jobj.bsd_root != NULL)

	if (cr_cfg_jobj.bsd_root && cr_cfg_jobj.bsd_root->type == cJSON_Array)
		cr_config_array_init(cr_cfg_jobj.bsd_root);

	cr_cfg_jobj.pano_root = cJSON_GetObjectItem(cr_cfg_jobj.cfg_root, "car_pano");
	RT_ASSERT(cr_cfg_jobj.pano_root != NULL);

	if (cr_cfg_jobj.pano_root && cr_cfg_jobj.pano_root->type == cJSON_Array)
		cr_config_array_init(cr_cfg_jobj.pano_root);

	return ret;
}

/**
 * cr_cfg_save - save config parameter to file
 *
 * This function use to save config parameter for config file
 *
 * Returns -1 if called when get error ; otherwise, return 1
 */
lb_int32 cr_cfg_save(void)
{
	lb_int32 ret;

	if (cr_cfg_jobj.cfg_root) {
#ifdef ARCH_LOMBO_N7_CDR_MMC
		ret = lb_save_cfg_file("/mnt/sdcard/etc/cdr_config1.cfg",
				cr_cfg_jobj.cfg_root);
#else
		ret = lb_save_cfg_file("/mnt/data/cdr_config.cfg", cr_cfg_jobj.cfg_root);
  #if 1
    if(ret==-1){
		data_regionalization();
		 rt_thread_delay(20);
	    ret = lb_save_cfg_file("/mnt/data/cdr_config.cfg", cr_cfg_jobj.cfg_root);
	}
  #endif
#endif
	} else {
		APP_LOG_E("err cr_cfg_jobj.cfg_root is NULL");
		return -1;
	}

	return ret;
}
/**
 * cr_cfg_save - save config parameter to file
 *
 * This function use to save config parameter for config file
 *
 * Returns -1 if called when get error ; otherwise, return 1
 */
lb_int32 cr_cfg_save_r(void)
{
	lb_int32 ret;

	if (cr_cfg_jobj.cfg_root) {
#ifdef ARCH_LOMBO_N7_CDR_MMC
		ret = lb_save_cfg_file("/mnt/sdcard/etc/cdr_config1.cfg",
				cr_cfg_jobj.cfg_root);
#else
		ret = lb_save_cfg_file("/mnt/data/cdr_config.cfg", cr_cfg_jobj.cfg_root);
  #if 1
  printf("\nret_rec===========================%d\n",ret);
    if(ret==-1){
		data_regionalization();
		 rt_thread_delay(20);
	    ret = lb_save_cfg_file("/mnt/data/cdr_config.cfg", cr_cfg_jobj.cfg_root);
	}else {
		cJSON *cfg = lb_open_cfg_file("/mnt/data/cdr_config.cfg");
		if (cfg == NULL) {
		printf("\nagain_data\n");	
		data_regionalization();
		 rt_thread_delay(20);
	    ret = lb_save_cfg_file("/mnt/data/cdr_config.cfg", cr_cfg_jobj.cfg_root);
		}else{
		 lb_exit_cfg_file(cfg);	
		}
    }
  #endif
#endif
	} else {
		APP_LOG_E("err cr_cfg_jobj.cfg_root is NULL");
		return -1;
	}

	return ret;
}
/**
 * cr_cfg_exit - exit config file
 *
 * This function use to exit config file,if you want save_para,
 * you must call home_cfg_init first
 *
 * Returns  1
 */
lb_int32 cr_cfg_exit(void)
{
	lb_int32 ret = -1;

	cr_cfg_save();
	if (cr_cfg_jobj.cfg_root) {
		ret = lb_exit_cfg_file(cr_cfg_jobj.cfg_root);
		cr_cfg_jobj.cfg_root = NULL;
	} else {
		APP_LOG_E("err cr_cfg_jobj.cfg_root is NULL");
		return -1;
	}

	return ret;
}
int adas_start(void)
{
	s32 ret = 0;

	APP_LOG_D("\n");
	memset(&g_mod_adas, 0, sizeof(mod_t));
	ret = lb_mod_open(&g_mod_adas, ROOTFS_MOUNT_PATH"/mod/mod_adas.mod", 0);
	if (ret != 0) {
		APP_LOG_E("ret:%d\n", ret);
		return ret;
	}
	adas_cb.get_adas_result = adas_resp;
	ret = lb_mod_ctrl(&g_mod_adas, MOD_ADAS_SET_RESULT_CB, 0, (void *)&adas_cb);
	if (ret != 0) {
		APP_LOG_E("err ret:%d\n", ret);
		return ret;
	}
	ret = lb_mod_ctrl(&g_mod_adas, MOD_ADAS_START, 0, recorder_fd);
	if (ret != 0) {
		APP_LOG_E("err ret:%d\n", ret);
		return ret;
	}
	APP_LOG_D("\n");

	return ret;
}
int adas_stop(void)
{
	s32 ret = 0;

	APP_LOG_D("\n");
	if (g_mod_adas.mh == 0) {
		APP_LOG_E("g_mod_adas NULL\n");
		return -1;
	}
	ret = lb_mod_ctrl(&g_mod_adas, MOD_ADAS_STOP, 0, recorder_fd);
	if (ret != 0) {
		APP_LOG_E("err ret:%d\n", ret);
		return ret;
	}
	ret = lb_mod_close(&g_mod_adas);
	if (ret != 0) {
		APP_LOG_E("err ret:%d\n", ret);
		return ret;
	}
	APP_LOG_D("\n");

	return ret;
}
int adas_get_status(void)
{
	s32 ret = -1;
	int status = -2;

	APP_LOG_D("\n");
	if (g_mod_adas.mh == 0) {
		APP_LOG_W("g_mod_adas NULL\n");
		return -1;
	}
	pthread_mutex_lock(&cr_adas_mutex);
	ret = lb_mod_ctrl(&g_mod_adas, MOD_ADAS_GET_STATUS, 0, &status);
	pthread_mutex_unlock(&cr_adas_mutex);
	if (ret == 0)
		return status;

	APP_LOG_D("\n");

	return ret;
}

int bsd_start(void)
{
	s32 ret = 0;

	APP_LOG_W("\n");
	memset(&g_mod_bsd, 0, sizeof(mod_t));
	ret = lb_mod_open(&g_mod_bsd, ROOTFS_MOUNT_PATH"/mod/mod_bsd.mod", 0);
	if (ret != 0) {
		APP_LOG_E("ret:%d\n", ret);
		return ret;
	}
	APP_LOG_W("\n");
	bsd_cb.get_bsd_result = bsd_resp;
	ret = lb_mod_ctrl(&g_mod_bsd, MOD_BSD_SET_RESULT_CB, 0, (void *)&bsd_cb);
	if (ret != 0) {
		APP_LOG_E("err ret:%d\n", ret);
		return ret;
	}
	APP_LOG_W("\n");
	ret = lb_mod_ctrl(&g_mod_bsd, MOD_BSD_START, 0, recorder_rd);
	if (ret != 0) {
		APP_LOG_E("err ret:%d\n", ret);
		return ret;
	}
	APP_LOG_W("\n");

	return ret;
}
int bsd_stop(void)
{
	s32 ret = 0;

	APP_LOG_D("\n");
	if (g_mod_bsd.mh == 0) {
		APP_LOG_E("g_mod_bsd NULL\n");
		return -1;
	}
	ret = lb_mod_ctrl(&g_mod_bsd, MOD_BSD_STOP, 0, recorder_rd);
	if (ret != 0) {
		APP_LOG_E("err ret:%d\n", ret);
		return ret;
	}
	ret = lb_mod_close(&g_mod_bsd);
	if (ret != 0) {
		APP_LOG_E("err ret:%d\n", ret);
		return ret;
	}
	APP_LOG_D("\n");
  bsd_disp_hidden_object();
	return ret;
}
int bsd_get_status(void)
{
	s32 ret = -1;
	int status = -2;

	APP_LOG_D("\n");
	if (g_mod_bsd.mh == 0) {
		APP_LOG_W("g_mod_bsd NULL\n");
		return -1;
	}
	pthread_mutex_lock(&cr_bsd_mutex);
	ret = lb_mod_ctrl(&g_mod_bsd, MOD_BSD_GET_STATUS, 0, &status);
	pthread_mutex_unlock(&cr_bsd_mutex);
	if (ret == 0)
		return status;

	APP_LOG_D("\n");

	return ret;
}

static void *cdr_start_adas(void *parameter)
{
	APP_LOG_D("\n");
	pthread_mutex_lock(&cr_adas_mutex);
	adas_start();
	pthread_mutex_unlock(&cr_adas_mutex);
	APP_LOG_D("\n");
	return NULL;
}
static void *cdr_stop_adas(void *parameter)
{
	APP_LOG_D("\n");
	pthread_mutex_lock(&cr_adas_mutex);
	adas_stop();
	pthread_mutex_unlock(&cr_adas_mutex);
	APP_LOG_D("\n");

	return NULL;
}
static void *cdr_start_bsd(void *parameter)
{
	APP_LOG_D("\n");
	pthread_mutex_lock(&cr_bsd_mutex);
	bsd_start();
	pthread_mutex_unlock(&cr_bsd_mutex);
	APP_LOG_D("\n");
	return NULL;
}
static void *cdr_stop_bsd(void *parameter)
{
	APP_LOG_D("\n");
	pthread_mutex_lock(&cr_bsd_mutex);
	bsd_stop();
	pthread_mutex_unlock(&cr_bsd_mutex);
	APP_LOG_D("\n");

	return NULL;
}

static void *cdr_comp_start_front_recorder(void *parameter)
{
	int ret;
	cdr_comp_thread_manager_t *cdr_comp = parameter;
	app_t *app_hd = NULL;
	record_obj_t record_obj;

	memset(&record_obj, 0x00, sizeof(record_obj_t));
	app_hd = lb_app_check("home");
	if (app_hd)
		lb_app_ctrl(app_hd, MSG_GET_REC_OBJ, 0, (void *)&record_obj);

	pthread_mutex_lock(&cr_front_mutex);
	if (NULL == record_obj.record_fhd) {
		if (FRONT_PREVIEW_WIDTH == 1920)
			recorder_fd = car_recorder_init("isp_cap.0", cdr_comp);
		else
			recorder_fd = car_recorder_init("isp", cdr_comp);
	} else {
		APP_LOG_D("recorder_fd has been created: %p-%p\n", record_obj.record_fhd,
			recorder_fd);
		if (NULL == recorder_fd)
			recorder_fd = record_obj.record_fhd;
		if (get_interval_record_enable() && !get_acc_status()) {
			cdr_comp->record_mod = RECORDER_TYPE_TIME_LAG;
			cdr_comp->rec_time_lag_para.interval = 1000;
			cdr_comp->rec_time_lag_para.play_framerate = 30000;
		} else	{
			cdr_comp->record_mod = RECORDER_TYPE_NORMAL;
			cdr_comp->rec_time_lag_para.interval = 0;
			cdr_comp->rec_time_lag_para.play_framerate = 30000;
		}
		lb_recorder_ctrl(recorder_fd, LB_REC_SET_MODE,
			&cdr_comp->record_mod);
		lb_recorder_ctrl(recorder_fd, LB_REC_SET_TIME_LAG_PARA,
			&cdr_comp->rec_time_lag_para);
		if (get_views_status() == PIP_VIEW || get_views_status() == FRONT_VIEW)
			_get_config_disp_para(&front_preview_para, FRONT_FULL_SCREEN);
		front_preview_para.crop.x = get_front_cropx();
		lb_recorder_ctrl(recorder_fd, LB_REC_SET_DISP_PARA,
			&front_preview_para);
		if (get_watermark_time_enable() == ENABLE_ON ||
		get_watermark_logo_enable() == ENABLE_ON) {
			pthread_mutex_lock(&cr_mutex);
			watermark_set_source(recorder_fd, WATERMARK_SOURCE_NUM,
				watermark_filename);
			pthread_mutex_unlock(&cr_mutex);
		}

	}

	RT_ASSERT(recorder_fd);
	if (get_views_status() == PIP_VIEW || get_views_status() ==
		FRONT_VIEW) {
		ret = preview_start(recorder_fd);
		if (ret < 0)
			APP_LOG_E("preview_start failed!\n");
	}
#ifdef OPEN_FRONT_RECORDER
	ret = recorder_start_front(recorder_fd);
	if (ret < 0)
		APP_LOG_W("recorder_start_front failed!\n");
#endif
	pthread_mutex_unlock(&cr_front_mutex);
	return NULL;
}
static void *cdr_comp_stop_front_recorder(void *parameter)
{
	int ret;
	app_t *app_hd = NULL;
	record_obj_t record_obj;

	memset(&record_obj, 0x00, sizeof(record_obj_t));
	app_hd = lb_app_check("home");
	if (app_hd)
		lb_app_ctrl(app_hd, MSG_GET_REC_OBJ, 0, (void *)&record_obj);

	APP_LOG_D("\n");
	pthread_mutex_lock(&cr_front_mutex);
#ifdef OPEN_FRONT_RECORDER
	ret = recoder_stop(recorder_fd);
	if (ret < 0)
		APP_LOG_E("recoder_stop err\n");
	APP_LOG_D("\n");
#endif
#if 0
	if (get_views_status() == PIP_VIEW || get_views_status() == FRONT_VIEW) {
		ret = preview_stop(recorder_fd);
		if (ret < 0)
			APP_LOG_E("preview_stop err\n");
	}
#endif
	APP_LOG_D("\n");
	if (record_obj.record_fhd == NULL) {
		ret = car_recorder_exit(recorder_fd);
		if (ret < 0)
			APP_LOG_E("car_recorder_exit err\n");
		recorder_fd = NULL;
	}

	pthread_mutex_unlock(&cr_front_mutex);
	APP_LOG_D("\n");

	return NULL;
}

static void *cdr_comp_start_rear_recorder(void *parameter)
{
	int ret;
	cdr_comp_thread_manager_t *cdr_comp = parameter;
	app_t *app_hd = NULL;
	record_obj_t record_obj;

	memset(&record_obj, 0x00, sizeof(record_obj_t));
	app_hd = lb_app_check("home");
	if (app_hd)
		lb_app_ctrl(app_hd, MSG_GET_REC_OBJ, 0, (void *)&record_obj);

	APP_LOG_D("\n");
	pthread_mutex_lock(&cr_rear_mutex);
	if (record_obj.record_rhd == NULL)
		recorder_rd = car_recorder_init("vic", cdr_comp);
	else {
		APP_LOG_D("recorder_rd has been created: %p-%p\n", record_obj.record_rhd,
			recorder_rd);
		if (NULL == recorder_rd)
			recorder_rd = record_obj.record_rhd;
		if (get_interval_record_enable() && !get_acc_status()) {
			cdr_comp->record_mod = RECORDER_TYPE_TIME_LAG;
			cdr_comp->rec_time_lag_para.interval = 1000;
			cdr_comp->rec_time_lag_para.play_framerate = 30000;
		} else	{
			cdr_comp->record_mod = RECORDER_TYPE_NORMAL;
			cdr_comp->rec_time_lag_para.interval = 0;
			cdr_comp->rec_time_lag_para.play_framerate = 30000;
		}
		lb_recorder_ctrl(recorder_rd, LB_REC_SET_MODE,
			&cdr_comp->record_mod);
		lb_recorder_ctrl(recorder_rd, LB_REC_SET_TIME_LAG_PARA,
			&cdr_comp->rec_time_lag_para);
		if (get_views_status() == PIP_VIEW)
			_get_config_disp_para(&rear_preview_para, SMALL_SCREEN);
		else if (get_views_status() == BACK_VIEW)
			_get_config_disp_para(&rear_preview_para, REAR_FULL_SCREEN);
		else
			_get_config_disp_para(&rear_preview_para, REAR_FULL_SCREEN);
		
		rear_preview_para.crop.x = get_rear_cropx();
		lb_recorder_ctrl(recorder_rd, LB_REC_SET_DISP_PARA,
			&rear_preview_para);
		lb_recorder_ctrl(recorder_rd, LB_REC_SET_ROTATE,
			(void *)VIDEO_ROTATE_270);
		if (get_watermark_time_enable() == ENABLE_ON ||
		get_watermark_logo_enable() == ENABLE_ON) {
			pthread_mutex_lock(&cr_mutex);
			watermark_set_source(recorder_rd, WATERMARK_SOURCE_NUM,
				watermark_filename);
			pthread_mutex_unlock(&cr_mutex);
		}

	}
	RT_ASSERT(recorder_rd);
	if (get_av_status()) {
		APP_LOG_D("\n");
		if (get_views_status() == PIP_VIEW || get_views_status()
			== BACK_VIEW)
			preview_start(recorder_rd);
		if (get_recorder_status(recorder_rd) == RECORDER_STATUS_RECORD) {
			ret = recoder_stop(recorder_rd);
			if (ret < 0)
				APP_LOG_E("recoder_stop err\n");
		}
		ret = recorder_start_rear(recorder_rd);
		if (ret < 0)
			APP_LOG_W("recorder_start_rear failed\n");
	}
	pthread_mutex_unlock(&cr_rear_mutex);
	APP_LOG_D("\n");

	return NULL;
}
static void *cdr_comp_stop_rear_recorder(void *parameter)
{
	int ret;
	app_t *app_hd = NULL;
	record_obj_t record_obj;

	memset(&record_obj, 0x00, sizeof(record_obj_t));
	app_hd = lb_app_check("home");
	if (app_hd)
		lb_app_ctrl(app_hd, MSG_GET_REC_OBJ, 0, (void *)&record_obj);

	APP_LOG_D("\n");
	pthread_mutex_lock(&cr_rear_mutex);
	if (get_back_status() == 1 && pano_get_enable() != ENABLE_OFF && pano_check()
		!= -1) {
		ret = pano_stop(recorder_rd);
		if (ret < 0)
			APP_LOG_E("pano_stop err\n");
		cdr_cmd_click_en(true);
	}
#ifdef OPEN_REAR_RECORDER
	ret = recoder_stop(recorder_rd);
	if (ret < 0)
		APP_LOG_E("recoder_stop err\n");
#endif
#if 0
	if (get_views_status() == PIP_VIEW || get_views_status() == BACK_VIEW) {
		ret = preview_stop(recorder_rd);
		if (ret < 0)
			APP_LOG_E("preview_stop err\n");
	}
#endif
#ifdef PANO_DEBUG
	if (get_views_status() == BACK_VIEW) {
		ret = pano_stop(recorder_rd);
		if (ret < 0)
			APP_LOG_E("pano_stop err\n");
	}
#endif

	APP_LOG_D("\n");
	if (record_obj.record_rhd == NULL) {
		ret = car_recorder_exit(recorder_rd);
		if (ret < 0)
			APP_LOG_E("car_recorder_exit err\n");
		recorder_rd = NULL;
	}

	pthread_mutex_unlock(&cr_rear_mutex);
	APP_LOG_D("\n");

	return NULL;
}
void car_comp_stop_preview(void)
{
	int ret = 0;

	if (get_views_status() == PIP_VIEW || get_views_status() == BACK_VIEW) {
		if (recorder_rd) {
			ret = preview_stop(recorder_rd);
			if (ret < 0)
				APP_LOG_E("preview_stop err\n");
		}
	} else if (get_views_status() == PIP_VIEW || get_views_status() == FRONT_VIEW) {
		if (recorder_fd) {
			ret = preview_stop(recorder_fd);
			if (ret < 0)
				APP_LOG_E("preview_stop err\n");
		}

	}
}
void car_comp_release(void)
{
	int ret = 0;
	ret = car_recorder_exit(recorder_rd);
	if (ret < 0)
		APP_LOG_E("car_recorder_exit err\n");
	recorder_rd = NULL;

	ret = car_recorder_exit(recorder_fd);
	if (ret < 0)
		APP_LOG_E("car_recorder_exit err\n");
	recorder_rd = NULL;
	
}

lb_int32 cdr_comp_thread_init(cdr_comp_thread_manager_t *cdr_comp)
{
	pthread_attr_t                  tmp_attr;
	struct sched_param              shed_param;
	lb_int32                        ret;

	ret = pthread_attr_init(&tmp_attr);
	if (ret != 0) {
		APP_LOG_E("\n init thread attr error: %d!\n", ret);
		return -1;
	}
	ret = pthread_attr_getschedparam(&tmp_attr, &shed_param);
	if (ret != 0) {
		APP_LOG_E("\n get thread priority error: %d!\n", ret);
		goto exit;
	}
	ret = pthread_attr_setscope(&tmp_attr, PTHREAD_SCOPE_SYSTEM);
	if (ret != 0) {
		APP_LOG_E("\n set thread scope error: %d!\n", ret);
		goto exit;
	}
	shed_param.sched_priority = CDR_COMP_THREAD_PRIORITY;
	ret = pthread_attr_setschedparam(&tmp_attr, &shed_param);
	if (ret != 0) {
		APP_LOG_E("\n set thread priority error: %d!\n", ret);
		goto exit;
	}
	ret = pthread_attr_setstacksize(&tmp_attr, (size_t)CDR_COMP_STACK_SIZE);
	if (ret != 0) {
		APP_LOG_E("\n set thread stack size error: %d!!\n", ret);
		goto exit;
	}
	if (cdr_comp->com_flag == COMP_START_ADAS) {
		ret = pthread_create(&cdr_comp->thread_id, &tmp_attr, &cdr_start_adas,
			cdr_comp);
		if (ret != 0) {
			APP_LOG_E("pthread_create failed!\n");
			goto exit;
		}
	} else if (cdr_comp->com_flag == COMP_STOP_ADAS) {
		ret = pthread_create(&cdr_comp->thread_id, &tmp_attr, &cdr_stop_adas,
			cdr_comp);
		if (ret != 0) {
			APP_LOG_E("pthread_create failed!\n");
			goto exit;
		}
	} else if (cdr_comp->com_flag == COMP_START_BSD) {
		ret = pthread_create(&cdr_comp->thread_id, &tmp_attr, &cdr_start_bsd,
			cdr_comp);
		if (ret != 0) {
			APP_LOG_E("pthread_create failed!\n");
			goto exit;
		}
	} else if (cdr_comp->com_flag == COMP_STOP_BSD) {
		ret = pthread_create(&cdr_comp->thread_id, &tmp_attr, &cdr_stop_bsd,
			cdr_comp);
		if (ret != 0) {
			APP_LOG_E("pthread_create failed!\n");
			goto exit;
		}
	} else if (cdr_comp->com_flag == COMP_START_FRONT_RECORDER) {
		ret = pthread_create(&cdr_comp->thread_id, &tmp_attr,
			&cdr_comp_start_front_recorder, cdr_comp);
		if (ret != 0) {
			APP_LOG_E("pthread_create failed!\n");
			goto exit;
		}
	} else if (cdr_comp->com_flag == COMP_STOP_FRONT_RECORDER) {
		ret = pthread_create(&cdr_comp->thread_id, &tmp_attr,
			&cdr_comp_stop_front_recorder, cdr_comp);
		if (ret != 0) {
			APP_LOG_E("pthread_create failed!\n");
			goto exit;
		}
	} else if (cdr_comp->com_flag == COMP_START_REAR_RECORDER) {
		ret = pthread_create(&cdr_comp->thread_id, &tmp_attr,
			&cdr_comp_start_rear_recorder, cdr_comp);
		if (ret != 0) {
			APP_LOG_E("pthread_create failed!\n");
			goto exit;
		}
	} else if (cdr_comp->com_flag == COMP_STOP_REAR_RECORDER) {
		ret = pthread_create(&cdr_comp->thread_id, &tmp_attr,
			&cdr_comp_stop_rear_recorder, cdr_comp);
		if (ret != 0) {
			APP_LOG_E("pthread_create failed!\n");
			goto exit;
		}
	}
exit:
	pthread_attr_destroy(&tmp_attr);

	return 0;
}

lb_int32 get_backlight_value(void)
{
	if (cr_cfg_jobj.lcd_brightness) {
		if (cr_cfg_jobj.lcd_brightness->valueint >= 0
			&& cr_cfg_jobj.lcd_brightness->valueint <=100)
			return cr_cfg_jobj.lcd_brightness->valueint;
	}

	return 100;
}


lb_int32 backlight_value_to_idx(lb_uint32 value)
{
	lb_int32 ret = 0;

	switch (value) {
	case 15:
		ret = 0;
		break;

	case 70:
		ret = 1;
		break;

	case 100:
		ret = 2;
		break;

	default:
		ret = 0;
		break;
	}

	return ret;
}
static rt_device_t disp_device;
static lb_int32 set_backlight(lb_uint32 value)
{
	lb_int32 ret = 0;
	disp_io_ctrl_t dic;

	/* set value to display */
	disp_device = rt_device_find(DISP_DEVICE_NAME);
	if (disp_device != NULL) {
		rt_device_open(disp_device, 0);

		rt_memset(&dic, 0x00, sizeof(disp_io_ctrl_t));
		dic.args = &value;
		ret = rt_device_control(disp_device, DISP_CMD_SET_BACKLIGHT_VALUE, &dic);
		if (ret != 0) {
			APP_LOG_W("[%s,%d,failed]\n", __FILE__, __LINE__);
			return ret;
		}

		rt_device_close(disp_device);
	}

	return ret;
}
void set_backlight_value(lb_uint32 idx)
{
	lb_int32 value = 0;

	switch (idx) {
	case 0:
		value = 15;
		break;

	case 1:
		value = 70;
		break;

	case 2:
		value = 100;
		break;

	default:
		value = 100;
		break;
	}
    cr_status.lcd_brightness = value;
	cr_cfg_jobj.lcd_brightness->valueint = value;
	cr_cfg_jobj.lcd_brightness->valuedouble = value;
    set_backlight(value);
}
lb_int32 get_recorder_duration(void)
{
	if (cr_cfg_jobj.record_duration) {
		if (cr_cfg_jobj.record_duration->valueint >= DURATION_1MIN
			&& cr_cfg_jobj.record_duration->valueint < DURATION_MAX)
			return cr_cfg_jobj.record_duration->valueint;
	}

	return DURATION_1MIN;
}

lb_int32 get_recorder_resolution(void)
{
	if (cr_cfg_jobj.record_resolution) {
		if (cr_cfg_jobj.record_resolution->valueint >= RESOLUTION_720P
			&& cr_cfg_jobj.record_resolution->valueint < RESOLUTION_MAX)
			return cr_cfg_jobj.record_resolution->valueint;
	}

	return RESOLUTION_1080P;
}

/**
 * get_mute_enable - get mic enable status
 *
 * This function use to get mic status  ,such as ENABLE_ON,ENABLE_OFF
 *
 * Returns mic enable status
 */
enable_e get_mute_enable(void)
{

	if (cr_status.mute_enable < ENABLE_OFF || cr_status.mute_enable >= ENABLE_MAX) {
		APP_LOG_E("get_mic_status error");
		cr_status.mute_enable = ENABLE_OFF;
	}

	return cr_status.mute_enable;
}

enable_e get_warn_tone_enable(void)
{

	if (cr_status.warn_tone_enable < ENABLE_OFF || cr_status.warn_tone_enable >= ENABLE_MAX) {
		APP_LOG_E("get_mic_status error");
		cr_status.warn_tone_enable = ENABLE_OFF;
	}

	return cr_status.warn_tone_enable;
}


/**
 * set_mute_enable - set mic enable status
 * @value: mic status, such as ENABLE_ON,ENABLE_OFF
 *
 * This function use to set mic enable status
 */
void set_mute_enable(enable_e value)
{
	cr_status.mute_enable = value;
	cr_cfg_jobj.mute_enable->valueint = value;
	cr_cfg_jobj.mute_enable->valuedouble = value;

	recoder_enable_mute(recorder_fd, value);
	recoder_enable_mute(recorder_rd, value);
}

/**
 * get_views_status - get display views
 *
 * This function use to get display views ,such as FRONT_VIEWS,BACK_VIEWS,PIC_IN_PIC
 *
 * Returns views of device
 */
views_status_e get_views_status()
{
	if (cr_status.views_s < 0 || cr_status.views_s >= VIEW_MAX) {
		APP_LOG_E("get_views_status error\n");
		cr_status.views_s = FRONT_VIEW;
	}

	return cr_status.views_s;
}


/**
 * set_views_status - set display views
 * @value: display views of device, such as FRONT_VIEWS,BACK_VIEWS,PIC_IN_PIC
 *
 * This function use to set display views
 */
void set_views_status(views_status_e value)
{
	cr_status.views_s = value;
}

/**
 * get_lock_status - get recording video lock status
 *
 * This function use to get recording video lock status ,such as LOCK_ON or LOCK_OFF
 *
 * Returns lock status of device
 */
lock_status_e get_lock_status(void)
{
	if (cr_status.lock_s < 0 || cr_status.lock_s >= LOCK_STATUS_MAX) {
		APP_LOG_E("get_lock_status error\n");
		cr_status.lock_s = LOCK_OFF;
	}

	return cr_status.lock_s;
}

/**
 * set_lock_status - set recording video lock status
 * @value: recording video lock status, such as LOCK_ON or LOCK_OFF
 *
 * This function use to set recording video lock status
 */
void set_lock_status(lock_status_e value)
{
	cr_status.lock_s = value;
}

int get_front_cropx(void)
{
	return cr_status.front_cropx;
}

int set_front_cropx(int value)
{
	cr_status.front_cropx = value;
	if (cr_cfg_jobj.front_cropx) {
		cr_cfg_jobj.front_cropx->valueint = value;
		cr_cfg_jobj.front_cropx->valuedouble = value;
	} else
		return -1;

	return 0;
}

int get_rear_cropx(void)
{
	return cr_status.rear_cropx;
}

int set_rear_cropx(int value)
{
	cr_status.rear_cropx = value;
	if (cr_cfg_jobj.rear_cropx) {
		cr_cfg_jobj.rear_cropx->valueint = value;
		cr_cfg_jobj.rear_cropx->valuedouble = value;
	}

	return 0;
}

/**
 * get_gsensor_sensity - get gsensor sensity
 *
 * This function use to get gsensor sensity
 *
 * Returns sensity of gsensor
 */
int get_gsensor_sensity(void)
{
	return cr_status.gsensor_sensity;
}
gsensor_sensity_e get_gsensor_sensity_map(void)
{
	gsensor_sensity_e ret = MID_SENSITY;

	if (cr_status.gsensor_sensity == 0)
		ret = HIGH_SENSITY;
	else if (cr_status.gsensor_sensity == 2)
		ret = LOW_SENSITY;
	else if (cr_status.gsensor_sensity == 1)
		ret = MID_SENSITY;
	else
		ret = SENSITY_CLOSE;

	return ret;
}

int get_bsd_alarm_speed_map(void)
{
	int ret = 0;

	if (cr_status.bsd_alarm_speed == LEVEL1)
		ret = 0;
	else if (cr_status.bsd_alarm_speed == LEVEL2)
		ret = 20;
	else if (cr_status.bsd_alarm_speed == LEVEL3)
		ret = 50;
	else
		ret = 0;

	return ret;
}

lb_int32 get_park_monitor_enable(void)
{
	if (cr_cfg_jobj.park_monitor) {
		if (cr_cfg_jobj.park_monitor->valueint >= ENABLE_OFF
			&& cr_cfg_jobj.park_monitor->valueint < ENABLE_MAX)
			return cr_cfg_jobj.park_monitor->valueint;
	}

	return ENABLE_OFF;
}

lb_int32 get_interval_record_enable(void)
{
	if (cr_cfg_jobj.interval_record) {
		if (cr_cfg_jobj.interval_record->valueint >= ENABLE_OFF
			&& cr_cfg_jobj.interval_record->valueint < ENABLE_MAX)
			return cr_cfg_jobj.interval_record->valueint;
	}

	return ENABLE_OFF;
}

lb_int32 get_rearmirr_enable(void)
{
	if (cr_cfg_jobj.rearmirr_enable) {
		if (cr_cfg_jobj.rearmirr_enable->valueint >= ENABLE_OFF
			&& cr_cfg_jobj.rearmirr_enable->valueint < ENABLE_MAX)
			return cr_cfg_jobj.rearmirr_enable->valueint;
	}

	return ENABLE_OFF;
}

/**
 * _get_config_disp_para - get disp rect
 * @index: disp index
 * @disp_para: disp rect rang
 *
 * This function get disp rect rang
 */
void _get_config_disp_para(win_para_t *disp_para, int index)
{
	if (index == LEFT_HALF_SCREEN) {
		disp_para->rect.x = 0;
		disp_para->rect.y = 0;

		disp_para->mode = VIDEO_WINDOW_USERDEF;
		disp_para->rect.width = 320;
		disp_para->rect.height = 640;

	} else if (index == RIGHR_HALF_SCREEN) {
		disp_para->rect.x = 0;

		disp_para->rect.y = 640;

		disp_para->mode = VIDEO_WINDOW_USERDEF;
		disp_para->rect.width = 320;
		disp_para->rect.height = 640;
	} else if (index == FRONT_FULL_SCREEN) {
		disp_para->rect.x = 0;
		disp_para->rect.y = 0;
		disp_para->mode = VIDEO_WINDOW_USERDEF;
#ifdef SCREEN_ROT_90
		disp_para->rect.width = cdr_screen.height;
		disp_para->rect.height = cdr_screen.width;
		disp_para->crop.y = 0;
		disp_para->crop.width = cdr_screen.height * FRONT_PREVIEW_WIDTH /
			cdr_screen.width;
		disp_para->crop.height = FRONT_PREVIEW_WIDTH;
#else
		disp_para->rect.width = cdr_screen.width;
		disp_para->rect.height = cdr_screen.height;
		disp_para->crop.y = 0;
		disp_para->crop.width = FRONT_PREVIEW_WIDTH;
		disp_para->crop.height = cdr_screen.height * FRONT_PREVIEW_WIDTH /
			cdr_screen.width;
#endif
	} else if (index == REAR_FULL_SCREEN) {
		disp_para->rect.x = 0;
		disp_para->rect.y = 0;
		disp_para->mode = VIDEO_WINDOW_USERDEF;
#ifdef SCREEN_ROT_90
		disp_para->rect.width = cdr_screen.height;
		disp_para->rect.height = cdr_screen.width;
		disp_para->crop.y = 0;
		disp_para->crop.width = cdr_screen.height * REAR_PREVIEW_WIDTH /
			cdr_screen.width;
		disp_para->crop.height = REAR_PREVIEW_WIDTH;
#else
		disp_para->rect.width = cdr_screen.width;
		disp_para->rect.height = cdr_screen.height;
		disp_para->crop.y = 1080;
		disp_para->crop.width = REAR_PREVIEW_WIDTH;
		disp_para->crop.height = 1080;/*cdr_screen.height * REAR_PREVIEW_WIDTH /
			cdr_screen.width;*/
#endif
	} else if (index == SMALL_SCREEN) {
		disp_para->rect.x = 140;
		disp_para->rect.y = 720;
		disp_para->mode = VIDEO_WINDOW_USERDEF;
		disp_para->rect.width = 180;
		disp_para->rect.height = 320;
	} else if (index == CUSTOM_SCREEN_SIZE) {
		disp_para->rect.x = 319;
		disp_para->rect.y = 1279;
		disp_para->mode = VIDEO_WINDOW_USERDEF;
		disp_para->rect.width = 1;
		disp_para->rect.height = 1;
	} else if (index == PANO_BEFORE_PROC) {
		disp_para->mode = VIDEO_WINDOW_USERDEF;
		disp_para->crop.x = PANO_PREVIEW_CROP_X;
		disp_para->crop.y = PANO_PREVIEW_CROP_Y;
		disp_para->crop.width = PANO_PREVIEW_CROP_W;
		disp_para->crop.height = PANO_PREVIEW_CROP_H;
		disp_para->rect.x = PANO_PREVIEW_WIND_X;
		disp_para->rect.y = PANO_PREVIEW_WIND_Y;
		disp_para->rect.width = PANO_PREVIEW_WIND_W;
		disp_para->rect.height = PANO_PREVIEW_WIND_H;
	} else if (index == PANO_AFTER_PROC) {
		disp_para->mode = VIDEO_WINDOW_USERDEF;
		disp_para->rect.x = PANO_BIRDBIEW_WIND_X;
		disp_para->rect.y = PANO_BIRDBIEW_WIND_Y;
		disp_para->rect.width = PANO_BIRDBIEW_WIND_W;
		disp_para->rect.height = PANO_BIRDBIEW_WIND_H;
	}

}

void get_screen_info(screen_info_t *cdr_screen)
{
	int ret;
	rt_device_t disp_dev;
	disp_io_ctrl_t dic;
	struct rt_device_graphic_info info;

	disp_dev = rt_device_find(DISP_DEVICE_NAME);
	/* open the device */
	if (disp_dev != NULL) {
		ret = rt_device_open(disp_dev, 0);
		if (ret != RT_EOK) {
			APP_LOG_E("rt_device_open error\n");
			return;
		}
	} else {
		APP_LOG_E("\n");
		return;
	}

	rt_memset(&dic, 0x00, sizeof(disp_io_ctrl_t));
	dic.dc_index = 0;
	dic.args = &info;
	rt_device_control(disp_dev, DISP_CMD_GET_INFO, &dic);
	APP_LOG_E("width[%d] height[%d]\n", info.width, info.height);
#ifdef SCREEN_ROT_90
	cdr_screen->width = info.height;
	cdr_screen->height = info.width;
	cdr_screen->front_max_crop_x = (FRONT_PREVIEW_HEIGHT / 32) * 32 -
		(cdr_screen->height * FRONT_PREVIEW_WIDTH / cdr_screen->width);
	cdr_screen->rear_max_crop_x = (REAR_PREVIEW_HEIGHT / 32) * 32 -
		(cdr_screen->height * REAR_PREVIEW_WIDTH / cdr_screen->width);
	APP_LOG_E("front x: %d, rear x: %d\n", cdr_screen->front_max_crop_x, cdr_screen->rear_max_crop_x);
#else
	cdr_screen->width = info.width;
	cdr_screen->height = info.height;
	cdr_screen->front_max_crop_y = (FRONT_PREVIEW_HEIGHT / 32) * 32 -
		(cdr_screen->height * FRONT_PREVIEW_WIDTH / cdr_screen->width);
	cdr_screen->rear_max_crop_y = (REAR_PREVIEW_HEIGHT / 32) * 32 -
		(cdr_screen->height * REAR_PREVIEW_WIDTH  / cdr_screen->width);
#endif
	/* close audio device */
	rt_device_close(disp_dev);

	return;
}
char *covert_path2basename(const char *path)
{
	const lb_byte *first, *end, *ptr;
	lb_byte *name;
	lb_int32 size;

	ptr   = (lb_byte *)path;
	first = ptr;
	end   = path + strlen(path);
	while (*ptr != '\0') {
		if (*ptr == '/')
			first = ptr + 1;
		if (*ptr == '.')
			end = ptr - 1;

		ptr++;
	}

	size = end - first + 1;
	name = malloc(size + 1);
	strncpy(name, first, size);
	name[size] = '\0';

	return name;
}

static int get_next_file(char *next_file, int index)
{
		int ret;
		int para;
		u64 used_mbsize = 0;
		u64 free_mbsize = 0;

		pthread_mutex_lock(&cr_mutex);
		ret = _get_disk_info(SDCARD_PATH, &used_mbsize, &free_mbsize);
		if (ret < 0) {
			pthread_mutex_unlock(&cr_mutex);
			return -1;
		}
		APP_LOG_W("used_mb:%lld free_mb:%lld\n", used_mbsize, free_mbsize);
		if (free_mbsize <= REC_RESERVE_SIZE) {
			if (index == FRONT_RECORDER)
				ret = rename_rec(car_file_mgr, FRONT_PATH, next_file);
			else
				ret = rename_rec(car_file_mgr, REAR_PATH, next_file);
			if (ret == 0) {
				APP_LOG_E("rename_rec name Ok\n");
				pthread_mutex_unlock(&cr_mutex);
				return 0;
			}
		}
		pthread_mutex_unlock(&cr_mutex);
		if (index == FRONT_RECORDER)
			ret = fallocate_front_file(car_file_mgr, next_file);
		else
			ret = fallocate_rear_file(car_file_mgr, next_file);
		if (ret < 0) {
			para = LB_USERDEF_SYSMSG_STOP_REC;
			lb_system_mq_send(LB_SYSMSG_USERDEF, &para, sizeof(int *), 0);
			APP_LOG_E("fallocate %s failed!\n", next_file);
			return ret;
		}

		return 0;
}

/**
 * cb_get_next_rear_file - call back for rear file
 * @hdl: recorder handle
 * @next_file: next file name
 *
 * This function called by media ,when cyclic record start ,pass a name to media
 *
 * Returns 0
 */
int cb_get_next_rear_file(void *hdl, char *next_file)
{
	struct tm *p_tm; /* time variable */
	time_t now;
	char tmp_file[64];
	int ret;
	int fd;

	now = time(RT_NULL);
	p_tm = localtime(&now);
#ifdef MEDIA_TYPE_TS
	if (cdr_comp_open_rear.record_mod == RECORDER_TYPE_TIME_LAG)
		sprintf(next_file, REAR_PATH"%02d%02d%02d%02d%02d%02d%d0_r_GAP.ts",
			p_tm->tm_year + 1900, p_tm->tm_mon + 1, p_tm->tm_mday,
			p_tm->tm_hour, p_tm->tm_min, p_tm->tm_sec,
			get_recorder_duration()/60);
	else
		sprintf(next_file, REAR_PATH"%02d%02d%02d%02d%02d%02d%d0_r.ts",
			p_tm->tm_year + 1900, p_tm->tm_mon + 1, p_tm->tm_mday,
			p_tm->tm_hour, p_tm->tm_min, p_tm->tm_sec,
			get_recorder_duration()/60);
#else
	if (cdr_comp_open_rear.record_mod == RECORDER_TYPE_TIME_LAG)
		sprintf(next_file, REAR_PATH"%02d%02d%02d%02d%02d%02d_r_GAP.mp4",
			p_tm->tm_year + 1900, p_tm->tm_mon + 1, p_tm->tm_mday,
			p_tm->tm_hour, p_tm->tm_min, p_tm->tm_sec);
	else
		sprintf(next_file, REAR_PATH"%02d%02d%02d%02d%02d%02d_r.mp4",
			p_tm->tm_year + 1900, p_tm->tm_mon + 1, p_tm->tm_mday,
			p_tm->tm_hour, p_tm->tm_min, p_tm->tm_sec);
#endif
	ret = file_mgr_get_rear_file(car_file_mgr, tmp_file);
	if (ret == 0) {
		pthread_mutex_lock(&cr_mutex);
		ret = rename(tmp_file, next_file);
		pthread_mutex_unlock(&cr_mutex);
		if (ret) {
			APP_LOG_E("rename %s to %s failed.\n", tmp_file, next_file);
			pthread_mutex_lock(&cr_mutex);
			fd = open(next_file, O_RDONLY);
			if (fd > 0)  {
				APP_LOG_W("exsit file %s\n", next_file);
				close(fd);
				pthread_mutex_unlock(&cr_mutex);
				return 0;
			}
			ret = unlink(tmp_file);
			if (ret < 0)
				APP_LOG_E("unlink %s failed!\n", tmp_file);
			pthread_mutex_unlock(&cr_mutex);
			ret = get_next_file(next_file, REAR_RECORDER);
		}
	} else
		ret = get_next_file(next_file, REAR_RECORDER);


	return ret;
}

/**
 * cb_rear_file_closed - call back for rear recorder when one recorder file over in cyclic record mode
 * @hdl: recorder handle
 * @file_name:  file name
 *
 * This function called by media ,when cyclic record over
 *
 * Returns 0
 */
int cb_rear_file_closed(void *hdl, char *file_name)
{
	char *file_name_temp;
	int ret;

	APP_LOG_D("close new file hdl:%p, file %s closed\n", hdl, file_name);
	if (lock_rear_file_num) {
	#if 1//jiasuofenqu
	if (get_sd_status() == SDCARD_PLUGIN){
		ret = file_mgr_judge_lock_memery();
		if (ret)
			APP_LOG_E("rear judge lock file size failed");
	}		
	#endif		
		file_name_temp = covert_path2basename(file_name);
#ifdef MEDIA_TYPE_TS
		sprintf(r_lock_filename, REAR_LOCK_PATH"%s_lock.ts", file_name_temp);
		APP_LOG_D("cb_rear_file_closed:old:%s, lock:%s\n", file_name_temp,
			r_lock_filename);
#else
		sprintf(r_lock_filename, REAR_LOCK_PATH"%s_lock.mp4", file_name_temp);
		APP_LOG_D("cb_rear_file_closed:old:%s, lock:%s\n", file_name_temp,
			r_lock_filename);
#endif
		if (file_name_temp)
			free(file_name_temp);
		pthread_mutex_lock(&cr_mutex);
		ret = rename(file_name, r_lock_filename);
		pthread_mutex_unlock(&cr_mutex);
		if (ret < 0)
			APP_LOG_E("rename %s to %s failed.", file_name, r_lock_filename);

		lock_rear_file_num--;
	}
	file_mgr_wakeup(car_file_mgr, 30);

	return 0;
}

/**
 * cb_get_next_front_file - call back for front file
 * @hdl: recorder handle
 * @next_file: next file name
 *
 * This function called by media ,when cyclic record start ,pass a name to media
 *
 * Returns 0
 */
int cb_get_next_front_file(void *hdl, char *next_file)
{
	struct tm *p_tm; /* time variable */
	time_t now;
	char tmp_file[64];
	int ret;
	int fd;

	now = time(RT_NULL);
	p_tm = localtime(&now);
#ifdef MEDIA_TYPE_TS
	if (cdr_comp_open_front.record_mod == RECORDER_TYPE_TIME_LAG)
		sprintf(next_file, FRONT_PATH"%02d%02d%02d%02d%02d%02d%d%d_f_GAP.ts",
			p_tm->tm_year + 1900, p_tm->tm_mon + 1, p_tm->tm_mday,
			p_tm->tm_hour, p_tm->tm_min, p_tm->tm_sec,
			get_recorder_duration()/60, get_recorder_resolution());
	else
		sprintf(next_file, FRONT_PATH"%02d%02d%02d%02d%02d%02d%d%d_f.ts",
			p_tm->tm_year + 1900, p_tm->tm_mon + 1, p_tm->tm_mday,
			p_tm->tm_hour, p_tm->tm_min, p_tm->tm_sec,
			get_recorder_duration()/60, get_recorder_resolution());
#else
	if (cdr_comp_open_front.record_mod == RECORDER_TYPE_TIME_LAG)
		sprintf(next_file, FRONT_PATH"%02d%02d%02d%02d%02d%02d_f_GAP.mp4",
			p_tm->tm_year + 1900, p_tm->tm_mon + 1, p_tm->tm_mday,
			p_tm->tm_hour, p_tm->tm_min, p_tm->tm_sec);
	else
		sprintf(next_file, FRONT_PATH"%02d%02d%02d%02d%02d%02d_f.mp4",
			p_tm->tm_year + 1900, p_tm->tm_mon + 1, p_tm->tm_mday,
			p_tm->tm_hour, p_tm->tm_min, p_tm->tm_sec);
#endif
	ret = file_mgr_get_front_file(car_file_mgr, tmp_file);
	if (ret == 0) {
		pthread_mutex_lock(&cr_mutex);
		ret = rename(tmp_file, next_file);
		pthread_mutex_unlock(&cr_mutex);
		if (ret) {
			APP_LOG_E("rename %s to %s failed.\n", tmp_file, next_file);
			pthread_mutex_lock(&cr_mutex);
			fd = open(next_file, O_RDONLY);
			if (fd > 0)  {
				APP_LOG_W("exsit file %s\n", next_file);
				close(fd);
				pthread_mutex_unlock(&cr_mutex);
				return 0;
			}
			ret = unlink(tmp_file);
			if (ret < 0)
				APP_LOG_E("unlink %s failed!\n", tmp_file);
			pthread_mutex_unlock(&cr_mutex);
			ret = get_next_file(next_file, FRONT_RECORDER);
		}
	} else
		ret = get_next_file(next_file, FRONT_RECORDER);

	return ret;
}

/**
 * cb_front_file_closed - call back for front recorder when one recorder file over in cyclic record mode
 * @hdl: recorder handle
 * @file_name:  file name
 *
 * This function called by media ,when cyclic record over
 *
 * Returns 0
 */
int cb_front_file_closed(void *hdl, char *file_name)
{
	char *file_name_temp;
	int ret;

	APP_LOG_D("close new file hdl:%p, file %s closed lock_front_file_num:%d\n", hdl,
		file_name, lock_front_file_num);
	if (lock_front_file_num) {
	#if 1//jiasuofenqu
	 printf("\njiasuofenqu>>>>>>>>>>>>>>%d>>>>>>>>>>>\n",get_sd_status());
	if (get_sd_status() == SDCARD_PLUGIN){
		ret = file_mgr_judge_lock_memery();
		if (ret)
			APP_LOG_E("front judge lock file size failed");
	}
	#endif		
		file_name_temp = covert_path2basename(file_name);
#ifdef MEDIA_TYPE_TS
		sprintf(f_lock_filename, FRONT_LOCK_PATH"%s_lock.ts", file_name_temp);
#else
		sprintf(f_lock_filename, FRONT_LOCK_PATH"%s_lock.mp4", file_name_temp);
#endif
		APP_LOG_D("cb_front_file_closed:old:%s, lock:%s\n", file_name_temp,
			f_lock_filename);
		if (file_name_temp)
			free(file_name_temp);
		pthread_mutex_lock(&cr_mutex);
		ret = rename(file_name, f_lock_filename);
		pthread_mutex_unlock(&cr_mutex);
		if (ret < 0)
			APP_LOG_E("rename %s to %s failed.", file_name, r_lock_filename);

		if (get_lock_mbsize() >= LOCK_WAR_SIZE) {
			dialog_flag = DIALOG_LOCKFILE_FULL;
			lb_system_mq_send(LB_SYSMSG_LOCKFILE_FULL, &dialog_flag,
				sizeof(int), 0);
			APP_LOG_W("LB_SYSMSG_LOCKFILE_FULL\n");
		}

		lock_front_file_num--;
		if (lock_front_file_num < 0)
			lock_front_file_num = 0;
		APP_LOG_D("%d\n", lock_front_file_num);
		 if (get_lock_status() && !lock_front_file_num)
		 	set_lock_status(0);
	}
	file_mgr_wakeup(car_file_mgr, 30);

	return 0;
}

/**
 * car_recorder_init - prepara for car recorder
 * @video_source: video source
 * @cdr_comp: cdr component thread manager struct
 *
 * This function use to prepara for car recorder
 *
 * Returns recorder handle
 */
void *car_recorder_init(char *video_source, cdr_comp_thread_manager_t *cdr_comp)
{
	int ret = -1;
	void *recorder_hd = NULL;
	rec_param_t rec_para;
#ifdef SYS_MEMERY_128M_FOR_DOUBLE_1080P
	user_def_sys_cfg_t userdef_sys_cfg;
#endif


	recorder_hd = lb_recorder_creat();
	cdr_comp->hd = recorder_hd;
	if (recorder_hd) {
#ifdef SYS_MEMERY_128M_FOR_DOUBLE_1080P
		if ((FRONT_RECORDER_SOURCE_HEIGHT == 1080) &&
			(REAR_RECORDER_SOURCE_HEIGHT == 1080)) {
			if (!strcmp(video_source, "isp") ||
				!strcmp(video_source, "isp_cap.0"))
				userdef_sys_cfg.camera_buf_num = FRONT_CAMERA_BUFFER_NUM;
			else
				userdef_sys_cfg.camera_buf_num = REAR_CAMERA_BUFFER_NUM;
			lb_recorder_ctrl(recorder_hd, LB_REC_SET_USER_DEF_SYS_PRAR,
				&userdef_sys_cfg);
		}
#endif
		APP_LOG_E("video_source: %s\n", video_source);
		if(!strcmp(video_source, "isp") ||
				!strcmp(video_source, "isp_cap.0")) {
			ret = lb_recorder_ctrl(recorder_hd, LB_REC_SET_VIDEO_SOURCE,
					(void *)"vic");
			if (ret < 0) {
				APP_LOG_E("LB_REC_SET_VIDEO_SOURCE failed!");
				return NULL;
			}
		} else {
			ret = lb_recorder_ctrl(recorder_hd, LB_REC_SET_VIDEO_SOURCE,
					(void *)video_source);
			if (ret < 0) {
				APP_LOG_E("LB_REC_SET_VIDEO_SOURCE failed!");
				return NULL;
			}
		}
		if (!strcmp(video_source, "isp")) {
			APP_LOG_W("acc_status:%d\n", get_acc_status());
			if (get_interval_record_enable() && !get_acc_status()) {
				cdr_comp->record_mod = RECORDER_TYPE_TIME_LAG;
				cdr_comp->rec_time_lag_para.interval = 1000;
				cdr_comp->rec_time_lag_para.play_framerate = 30000;
			} else  {
				cdr_comp->record_mod = RECORDER_TYPE_NORMAL;
				cdr_comp->rec_time_lag_para.interval = 0;
				cdr_comp->rec_time_lag_para.play_framerate = 30000;
			}
			lb_recorder_ctrl(recorder_hd, LB_REC_SET_MODE,
				&cdr_comp->record_mod);
			lb_recorder_ctrl(recorder_hd, LB_REC_SET_TIME_LAG_PARA,
				&cdr_comp->rec_time_lag_para);
			memset(&rec_para, 0, sizeof(rec_param_t));
			ret = lb_recorder_ctrl(recorder_hd, LB_REC_GET_PARA, &rec_para);
			if (ret < 0) {
				APP_LOG_E("LB_REC_GET_PARA failed!");
				return NULL;
			}
			rec_para.source_width = /*FRONT_RECORDER_SOURCE_WIDTH*/1920;
			rec_para.source_height = /*FRONT_RECORDER_SOURCE_HEIGHT*/2160;
			if (get_recorder_resolution() == RESOLUTION_1080P) {
				rec_para.width = 1920;
				rec_para.height = 1080;
#if 0
			} else if (get_recorder_resolution() == RESOLUTION_1296P) {
				rec_para.width = 1920;
				rec_para.height = 1296;
			} else if (get_recorder_resolution() == RESOLUTION_1440P) {
				rec_para.width = 1920;
				rec_para.height = 1440;
			} else if (get_recorder_resolution() == RESOLUTION_2K) {
				rec_para.width = 2560;
				rec_para.height = 1440;
#endif
			} else {
				rec_para.width = 1280;
				rec_para.height = 720;
			}
			if (rec_para.height > 720) {
				rec_para.bitrate = HIGH_REC_BITRATE;
				rec_para.enc_rect.x = 0;
				rec_para.enc_rect.y = 0;
				rec_para.enc_rect.width = 1920;
				rec_para.enc_rect.height = 1080;
			} else
				rec_para.bitrate = LOW_REC_BITRATE;
			rec_para.audio_sample_rate = 16000;
#ifdef MEDIA_TYPE_TS
			rec_para.file_fmt = REC_OUTPUT_FORMAT_TS;
#else
			rec_para.file_fmt = REC_OUTPUT_FORMAT_MP4;
#endif
			rec_para.frame_rate = FRONT_RECORDER_SOURCE_FPS * 1000;
			ret = lb_recorder_ctrl(recorder_hd, LB_REC_SET_PARA, &rec_para);
			if (ret < 0) {
				APP_LOG_E("LB_REC_SET_PARA failed!\n");
				return NULL;
			}
			if (get_views_status() == PIP_VIEW || get_views_status() ==
				FRONT_VIEW)
				_get_config_disp_para(&front_preview_para,
				FRONT_FULL_SCREEN);
			front_preview_para.crop.x = get_front_cropx();
			lb_recorder_ctrl(recorder_hd, LB_REC_SET_DISP_PARA,
				&front_preview_para);
#if 1
			lb_recorder_ctrl(recorder_hd, LB_REC_SET_ROTATE,
				(void *)VIDEO_ROTATE_270);
#endif
		} else if (!strcmp(video_source, "isp_cap.0")) {
			APP_LOG_W("acc_status:%d\n", get_acc_status());
			if (get_interval_record_enable() && !get_acc_status()) {
				cdr_comp->record_mod = RECORDER_TYPE_TIME_LAG;
				cdr_comp->rec_time_lag_para.interval = 1000;
				cdr_comp->rec_time_lag_para.play_framerate = 30000;
			} else  {
				cdr_comp->record_mod = RECORDER_TYPE_NORMAL;
				cdr_comp->rec_time_lag_para.interval = 0;
				cdr_comp->rec_time_lag_para.play_framerate = 30000;
			}
			lb_recorder_ctrl(recorder_hd, LB_REC_SET_MODE,
				&cdr_comp->record_mod);
			lb_recorder_ctrl(recorder_hd, LB_REC_SET_TIME_LAG_PARA,
				&cdr_comp->rec_time_lag_para);
			memset(&rec_para, 0, sizeof(rec_param_t));
			ret = lb_recorder_ctrl(recorder_hd, LB_REC_GET_PARA, &rec_para);
			if (ret < 0) {
				APP_LOG_E("LB_REC_GET_PARA failed!");
				return NULL;
			}
			rec_para.source_width = /*FRONT_RECORDER_SOURCE_WIDTH*/1920;
			rec_para.source_height = /*FRONT_RECORDER_SOURCE_HEIGHT*/2160;
			if (get_recorder_resolution() == RESOLUTION_1080P) {
				rec_para.width = 1920;
				rec_para.height = 1080;
#if 0
			} else if (get_recorder_resolution() == RESOLUTION_1296P) {
				rec_para.width = 1920;
				rec_para.height = 1296;
			} else if (get_recorder_resolution() == RESOLUTION_1440P) {
				rec_para.width = 1920;
				rec_para.height = 1440;
			} else if (get_recorder_resolution() == RESOLUTION_2K) {
				rec_para.width = 2560;
				rec_para.height = 1440;
#endif
			} else {
				rec_para.width = 1280;
				rec_para.height = 720;
			}
			if (rec_para.height > 720) {
				rec_para.bitrate = HIGH_REC_BITRATE;
				rec_para.enc_rect.x = 0;
				rec_para.enc_rect.y = 0;
				rec_para.enc_rect.width = 1920;
				rec_para.enc_rect.height = 1080;
			} else
				rec_para.bitrate = LOW_REC_BITRATE;
			rec_para.audio_sample_rate = 16000;
#ifdef MEDIA_TYPE_TS
			rec_para.file_fmt = REC_OUTPUT_FORMAT_TS;
#else
			rec_para.file_fmt = REC_OUTPUT_FORMAT_MP4;
#endif
			rec_para.frame_rate = FRONT_RECORDER_SOURCE_FPS * 1000;
			ret = lb_recorder_ctrl(recorder_hd, LB_REC_SET_PARA, &rec_para);
			if (ret < 0) {
				APP_LOG_E("LB_REC_SET_PARA failed!\n");
				return NULL;
			}
			/*if (get_views_status() == PIP_VIEW || get_views_status() ==
				FRONT_VIEW)*/
			_get_config_disp_para(&front_preview_para, FRONT_FULL_SCREEN);
			front_preview_para.crop.x = get_front_cropx();
			lb_recorder_ctrl(recorder_hd, LB_REC_SET_DISP_PARA,
				&front_preview_para);
#if 1
			lb_recorder_ctrl(recorder_hd, LB_REC_SET_ROTATE,
				(void *)VIDEO_ROTATE_270);
#endif
		} else if (!strcmp(video_source, "vic")) {
			if (get_interval_record_enable() && !get_acc_status()) {
				cdr_comp->record_mod = RECORDER_TYPE_TIME_LAG;
				cdr_comp->rec_time_lag_para.interval = 1000;
				cdr_comp->rec_time_lag_para.play_framerate = 30000;
			} else  {
				cdr_comp->record_mod = RECORDER_TYPE_NORMAL;
				cdr_comp->rec_time_lag_para.interval = 0;
				cdr_comp->rec_time_lag_para.play_framerate = 25000;
			}
			lb_recorder_ctrl(recorder_hd, LB_REC_SET_MODE,
				&cdr_comp->record_mod);
			lb_recorder_ctrl(recorder_hd, LB_REC_SET_TIME_LAG_PARA,
				&cdr_comp->rec_time_lag_para);
			memset(&rec_para, 0, sizeof(rec_param_t));
			ret = lb_recorder_ctrl(recorder_hd, LB_REC_GET_PARA, &rec_para);
			if (ret < 0) {
				APP_LOG_E("LB_REC_GET_PARA failed!\n");
				return NULL;
			}
			rec_para.source_width = /*REAR_RECORDER_SOURCE_WIDTH*/1920;
			rec_para.source_height = /*REAR_RECORDER_SOURCE_HEIGHT*/2160;
			rec_para.width = REAR_RECORDER_SOURCE_WIDTH;
			rec_para.height = REAR_RECORDER_SOURCE_HEIGHT;
			if (rec_para.height > 720)
				rec_para.bitrate = HIGH_REC_BITRATE;
			else
				rec_para.bitrate = LOW_REC_BITRATE;
			rec_para.audio_sample_rate = 16000;
			rec_para.enc_rect.x = 0;
			rec_para.enc_rect.y = 1080;
			rec_para.enc_rect.width = 1920;
			rec_para.enc_rect.height = 1080;
#ifdef MEDIA_TYPE_TS
			rec_para.file_fmt = REC_OUTPUT_FORMAT_TS;
#else
			rec_para.file_fmt = REC_OUTPUT_FORMAT_MP4;
#endif
			rec_para.frame_rate = REAR_RECORDER_SOURCE_FPS * 1000;
			ret = lb_recorder_ctrl(recorder_hd, LB_REC_SET_PARA, &rec_para);
			if (ret < 0) {
				APP_LOG_E("LB_REC_SET_PARA failed!\n");
				return NULL;
			}
			if (get_views_status() == PIP_VIEW)
				_get_config_disp_para(&rear_preview_para, SMALL_SCREEN);
			else if (get_views_status() == BACK_VIEW)
				_get_config_disp_para(&rear_preview_para,
				REAR_FULL_SCREEN);
			else
				_get_config_disp_para(&rear_preview_para,
				REAR_FULL_SCREEN);

			rear_preview_para.crop.x = get_rear_cropx();
			lb_recorder_ctrl(recorder_hd, LB_REC_SET_DISP_PARA,
				&rear_preview_para);
#if 0
			APP_LOG_E("display para rect; x: %d, y: %d, w: %d, h: %d\n",
				rear_preview_para.rect.x, rear_preview_para.rect.y,
				rear_preview_para.rect.width, rear_preview_para.rect.height);
			APP_LOG_E("display para crop; x: %d, y: %d, w: %d, h: %d\n",
				rear_preview_para.crop.x, rear_preview_para.crop.y,
				rear_preview_para.crop.width, rear_preview_para.crop.height);
#endif

			lb_recorder_ctrl(recorder_hd, LB_REC_SET_ROTATE,
				(void *)VIDEO_ROTATE_270);


		}

	} else {
		APP_LOG_E("\n");
		return NULL;
	}
	if (get_watermark_time_enable() == ENABLE_ON ||
		get_watermark_logo_enable() == ENABLE_ON) {
		pthread_mutex_lock(&cr_mutex);
		watermark_set_source(recorder_hd, WATERMARK_SOURCE_NUM,
			watermark_filename);
		pthread_mutex_unlock(&cr_mutex);
	}
	if (/*!strcmp(video_source, "vic") && */get_rearmirr_enable()) {
		ret = lb_recorder_ctrl(recorder_hd, LB_REC_SET_ROTATE,
			(void *)VIDEO_ROTATE_FLIP_V_ROT_90);
		if (ret < 0) {
			APP_LOG_E("LB_REC_SET_ROTATE failed!\n");
			return NULL;
		}
	}
	ret = lb_recorder_ctrl(recorder_hd, LB_REC_PREPARE, 0);
	if (ret < 0) {
		APP_LOG_E("LB_REC_PREPARE failed!\n");
		return NULL;
	}
	return recorder_hd;
}

int car_recorder_exit(void *recorder_hd)
{
	if (recorder_hd) {
		lb_recorder_release(recorder_hd);
		recorder_hd = NULL;
	} else
		APP_LOG_E("err\n");
	return 0;
}

int cr_dir_init()
{
	int ret;
	DIR *fd = NULL;

	pthread_mutex_lock(&cr_mutex);
	fd = opendir(FRONT_PATH);
	if (fd == NULL) {
		ret = mkdir(FRONT_PATH, 0);
		if (ret < 0) {
			pthread_mutex_unlock(&cr_mutex);
			return -1;
		}
	} else
		closedir(fd);
	fd = opendir(FRONT_LOCK_PATH);
	if (fd == NULL) {
		ret = mkdir(FRONT_LOCK_PATH, 0);
		if (ret < 0) {
			pthread_mutex_unlock(&cr_mutex);
			return -1;
		}
	} else
		closedir(fd);
	fd = opendir(REAR_PATH);
	if (fd == NULL) {
		ret = mkdir(REAR_PATH, 0);
		if (ret < 0) {
			pthread_mutex_unlock(&cr_mutex);
			return -1;
		}
	} else
		closedir(fd);
	fd = opendir(REAR_LOCK_PATH);
	if (fd == NULL) {
		ret = mkdir(REAR_LOCK_PATH, 0);
		if (ret < 0) {
			pthread_mutex_unlock(&cr_mutex);
			return -1;
		}
	} else
		closedir(fd);

	fd = opendir(PICTURE_PATH);
	if (fd == NULL) {
		ret = mkdir(PICTURE_PATH, 0);
		if (ret < 0) {
			pthread_mutex_unlock(&cr_mutex);
			return -1;
		}
	} else
		closedir(fd);
	pthread_mutex_unlock(&cr_mutex);

	return 0;
}
int get_sd_status()
{
	app_t *app_hd = NULL;
	int sd_status = -1;
	app_hd = lb_app_check("home");
	if (app_hd)
		lb_app_ctrl(app_hd, GET_SD_STATUS, 0, (void *)&sd_status);

	return sd_status;
}
int get_av_status()
{
	app_t *app_hd = NULL;
	int av_status = -1;
	app_hd = lb_app_check("home");
	if (app_hd)
		lb_app_ctrl(app_hd, GET_AV_STATUS, 0, (void *)&av_status);

	return av_status;
}
int get_back_status()
{
	app_t *app_hd = NULL;
	int av_status = -1;
	app_hd = lb_app_check("home");
	if (app_hd)
		lb_app_ctrl(app_hd, GET_BACK_STATUS, 0, (void *)&av_status);

	return av_status;
}

int get_acc_status()
{
	app_t *app_hd = NULL;
	int acc_status = -1;
	app_hd = lb_app_check("home");
	if (app_hd)
		lb_app_ctrl(app_hd, GET_ACC_STATUS, 0, (void *)&acc_status);

	return acc_status;
}

int get_dir_lockfilesize(char *path, u64 *used_mbsize)
{
	struct stat statbuf;
	struct dirent *ptr;
	DIR *dir = NULL;
	char filename[512];
	u64 used_size = 0;

	pthread_mutex_lock(&cr_mutex);
	dir = opendir(path);
	if (dir == NULL) {
		APP_LOG_E("Open dir error...\n");
		pthread_mutex_unlock(&cr_mutex);
		return -1;
	}
	while ((ptr = readdir(dir)) != NULL)  {
		if (ptr->d_type == 1) {
#ifdef MEDIA_TYPE_TS
			if (strstr(ptr->d_name, "_lock.ts") != NULL) {
#else
			if (strstr(ptr->d_name, "_lock.mp4") != NULL) {
#endif
				sprintf(filename, "%s/%s", path, ptr->d_name);
				stat(filename, &statbuf);
				used_size += ((u64)statbuf.st_size);
			}
		}
	}
	closedir(dir);
	pthread_mutex_unlock(&cr_mutex);
	*used_mbsize = used_size >> 20;

	return 0;
}

u64 get_lock_mbsize(void)
{
	u64 use_rearlock_mbsize;
	u64 use_frontlock_mbsize;
	int ret = 0;

	ret = get_dir_lockfilesize(REAR_LOCK_PATH, &use_rearlock_mbsize);
	if (ret < 0)
		return 0;
	APP_LOG_D("use_rearlock_mbsize:%lld\n", use_rearlock_mbsize);
	ret = get_dir_lockfilesize(FRONT_LOCK_PATH, &use_frontlock_mbsize);
	if (ret < 0)
		return 0;
	APP_LOG_D("use_frontlock_mbsize:%lld\n", use_frontlock_mbsize);

	return use_rearlock_mbsize + use_frontlock_mbsize;
}

int rm_over_file(char *path, int max_num, char *suffix_name)
{
	DIR *dir = NULL;
	int i = 0;
	struct dirent *ptr = NULL;
	char first_filename[32];
	char sec_filename[32];
	char temp_name[64];
	int ret;

	pthread_mutex_lock(&cr_mutex);
	dir = opendir(path);
	if (dir == NULL) {
		APP_LOG_E("Open dir error...\n");
		pthread_mutex_unlock(&cr_mutex);
		return -1;
	}
	first_filename[0] = 0;
	sec_filename[0] = 0;
	while ((ptr = readdir(dir)) != NULL) {
		if (ptr->d_type == 1 && strstr(ptr->d_name, suffix_name) != NULL) {
			i++;
			if (!strlen(first_filename)
				|| strncmp(first_filename, ptr->d_name, 14) >= 0) {
				strcpy(sec_filename, first_filename);
				strcpy(first_filename, ptr->d_name);
				continue;
			}
			if (!strlen(sec_filename)
				|| strncmp(sec_filename, ptr->d_name, 14) >= 0)
				strcpy(sec_filename, ptr->d_name);
		}
	}
	closedir(dir);
	if (i >= max_num) {
		if (first_filename[0]) {
			temp_name[0] = 0;
			sprintf(temp_name, "%s%s", path, first_filename);
			APP_LOG_D("temp_name:%s\n", temp_name);
			ret = unlink(temp_name);
			if (ret < 0)
				APP_LOG_E("unlink %s failed!", temp_name);
		}
		if (sec_filename[0]) {
			temp_name[0] = 0;
			sprintf(temp_name, "%s%s", path, sec_filename);
			APP_LOG_D("temp_name:%s\n", temp_name);
			ret = unlink(temp_name);
			if (ret < 0)
				APP_LOG_E("unlink %s failed!", temp_name);
		}
	}
	APP_LOG_D("file_num:%d\n", i);
	pthread_mutex_unlock(&cr_mutex);
	return i;

}

int preview_start(void *recorder_hd)
{
	int ret;

	if (recorder_hd) {
		APP_LOG_D("\n");
		ret = lb_recorder_ctrl(recorder_hd, LB_REC_PREVIEW, 0);
		if (ret < 0) {
			APP_LOG_E("LB_REC_PREVIEW failed!");
			return -1;
		}
		ret = lb_recorder_ctrl(recorder_hd, LB_REC_SET_LAYER_LEVEL,
				(void *)VIDEO_LAYER_BOTTOM);
		if (ret < 0) {
			APP_LOG_E("LB_REC_SET_LAYER_LEVEL failed!");
			return -1;
		}
		if (recorder_hd == recorder_fd) {
			if (adas_get_enable() == ENABLE_ON) {
				APP_LOG_D("\n");
				if ((!get_interval_record_enable() || get_acc_status())
					&& (adas_get_status() == ADAS_CLOSE
					|| adas_get_status() == -1)) {
					cdr_comp_start_adas.com_flag = COMP_START_ADAS;
					cdr_comp_thread_init(&cdr_comp_start_adas);
					pthread_join(cdr_comp_start_adas.thread_id, NULL);
				}
			}
		}
		if (recorder_hd == recorder_rd) {
			if (bsd_get_enable() == ENABLE_ON) {
				APP_LOG_D("\n");
				if ((!get_interval_record_enable() || get_acc_status())
					&& (bsd_get_status() == BSD_CLOSE
					|| bsd_get_status() == -1)) {
					cdr_comp_start_bsd.com_flag = COMP_START_BSD;
					cdr_comp_thread_init(&cdr_comp_start_bsd);
					pthread_join(cdr_comp_start_bsd.thread_id, NULL);
				}
			}
		}
	} else
		APP_LOG_E("recorder_hd NULL\n");

	return 0;
}

int preview_stop(void *recorder_hd)
{
	if (recorder_hd) {
		if (recorder_hd == recorder_fd) {
			if (adas_get_enable() == ENABLE_ON && adas_get_status() ==
				ADAS_OPEN) {
				if (bsd_get_enable() == ENABLE_ON ||
				(get_interval_record_enable() && !get_acc_status())) {
					cdr_comp_stop_adas.com_flag = COMP_STOP_ADAS;
					cdr_comp_thread_init(&cdr_comp_stop_adas);
					if (cdr_comp_stop_adas.thread_id) {
						pthread_join(cdr_comp_stop_adas.thread_id,
							NULL);
						cdr_comp_stop_adas.thread_id = NULL;
					}
				}
			}
		}
		if (recorder_hd == recorder_rd) {
			if (bsd_get_enable() == ENABLE_ON && bsd_get_status() ==
				BSD_OPEN) {
				if (adas_get_enable() == ENABLE_ON ||
				(get_interval_record_enable() && !get_acc_status())) {
					cdr_comp_stop_bsd.com_flag = COMP_STOP_BSD;
					cdr_comp_thread_init(&cdr_comp_stop_bsd);
					if (cdr_comp_stop_bsd.thread_id) {
						pthread_join(cdr_comp_stop_bsd.thread_id,
							NULL);
						cdr_comp_stop_bsd.thread_id = NULL;
					}
				}
			}
		}
		lb_recorder_ctrl(recorder_hd, LB_REC_STOP_PREVIEW, 0);
	} else
		APP_LOG_E("recorder_hd NULL\n");

	return 0;
}

int recorder_start_front(void *recorder_hd)
{
	int ret = -1;
	struct tm *p_tm; /* time variable */
	time_t now;
	u64 used_mbsize;
	u64 free_mbsize;

	if (!recorder_hd)
		return -1;

	if (get_sd_status() == SDCARD_NOT_PLUGIN) {
		APP_LOG_W("sd not is plugin\n");
		return -1;
	} else if (get_sd_status() == SDCARD_NOT_FORMAT) {
		dialog_flag = DIALOG_PLEASE_FORMAT_SDCARD;
		lb_system_mq_send(LB_SYSMSG_DIALOG, &dialog_flag, sizeof(int), 0);
		return -2;
	}
	ret = cr_dir_init();
	if (ret < 0)
		return -1;
	pthread_mutex_lock(&cr_mutex);
	ret = _get_disk_info(SDCARD_PATH, &used_mbsize, &free_mbsize);
	pthread_mutex_unlock(&cr_mutex);
	if (ret < 0) {
		APP_LOG_E("_get_disk_info failed!\n");
		rt_thread_mdelay(10);
		return -1;
	} else if (free_mbsize <= REC_RESERVE_SIZE) {
		if (!_get_file_exist_indir_(FRONT_PATH) &&
			!_get_file_exist_indir_(REAR_PATH)) {
			dialog_flag = DIALOG_RECORDER_FILE_FULL;
			lb_system_mq_send(LB_SYSMSG_RECORDER_FILE_FULL, &dialog_flag,
				sizeof(int), 0);
			APP_LOG_W("LB_SYSMSG_RECORDER_FILE_FULL %lld\n", free_mbsize);
			return -1;
		}
	}

	if (get_lock_mbsize() >= LOCK_WAR_SIZE) {
		dialog_flag = DIALOG_LOCKFILE_FULL;
		lb_system_mq_send(LB_SYSMSG_LOCKFILE_FULL, &dialog_flag, sizeof(int), 0);
		APP_LOG_W("LB_SYSMSG_LOCKFILE_FULL\n");
	}

	now = time(RT_NULL);
	p_tm = localtime(&now);

	fix_duration_front.file_duration = get_recorder_duration();
	fix_duration_front.cb_get_next_file = cb_get_next_front_file;
	fix_duration_front.cb_file_closed = cb_front_file_closed;
	memset(recorder_frontfilename, 0x00, 64);

	file_mgr_front_wait(car_file_mgr, 400);
	ret = cb_get_next_front_file(recorder_hd, recorder_frontfilename);
	if (ret < 0) {
		APP_LOG_W("ret:%d\n", ret);
		return ret;
	}
	file_mgr_wakeup(car_file_mgr, 10);
	if (recorder_hd) {
		ret = lb_recorder_ctrl(recorder_hd, LB_REC_SET_FIX_DURATION_PARA,
				(void *)(&fix_duration_front));
		if (ret < 0) {
			APP_LOG_E("LB_REC_SET_FIX_DURATION_PARA failed!\n");
			return -1;
		}
		ret = lb_recorder_ctrl(recorder_hd, LB_REC_SET_OUTPUT_FILE,
				(void *)recorder_frontfilename);
		if (ret < 0) {
			APP_LOG_E("LB_REC_SET_OUTPUT_FILE failed!\n");
			return -1;
		}
#if 1  
	rec_resolution=get_recorder_resolution();
#endif
		if (get_watermark_time_enable() == ENABLE_ON)
			watermark_time_fd(recorder_hd, p_tm);
		if (get_watermark_logo_enable() == ENABLE_ON)
			watermark_logo(recorder_hd, 14);

		recoder_enable_mute(recorder_fd, get_mute_enable());
		ret = lb_recorder_ctrl(recorder_hd, LB_REC_START, 0);
		if (ret < 0) {
			APP_LOG_E("LB_REC_START failed!\n");
			return -1;
		}
	}
	APP_LOG_D("\n");
	lock_front_file_num = 0;

	return ret;
}
int recorder_start_rear(void *recorder_hd)
{
	int ret = -1;
	struct tm *p_tm; /* time variable */
	time_t now;
	u64 used_mbsize;
	u64 free_mbsize;
	if (!recorder_hd)
		return -1;

	if (get_sd_status() == SDCARD_NOT_PLUGIN) {
		APP_LOG_W("sd not is plugin\n");
		return -1;
	} else if (get_sd_status() == SDCARD_NOT_FORMAT) {
		dialog_flag = DIALOG_PLEASE_FORMAT_SDCARD;
		lb_system_mq_send(LB_SYSMSG_DIALOG, &dialog_flag, sizeof(int), 0);
		return -2;
	}
	ret = cr_dir_init();
	if (ret < 0)
		return -1;
	pthread_mutex_lock(&cr_mutex);
	ret = _get_disk_info(SDCARD_PATH, &used_mbsize, &free_mbsize);
	pthread_mutex_unlock(&cr_mutex);
	if (ret < 0) {
		APP_LOG_E("_get_disk_info failed!\n");
		rt_thread_mdelay(10);
		return -1;
	} else if (free_mbsize <= REC_RESERVE_SIZE) {
		if (!_get_file_exist_indir_(FRONT_PATH) &&
			!_get_file_exist_indir_(REAR_PATH)) {
			dialog_flag = DIALOG_RECORDER_FILE_FULL;
			lb_system_mq_send(LB_SYSMSG_RECORDER_FILE_FULL, &dialog_flag,
				sizeof(int), 0);
			APP_LOG_W("LB_SYSMSG_RECORDER_FILE_FULL %lld\n", free_mbsize);
			return -1;
		}
	}
	now = time(RT_NULL);
	p_tm = localtime(&now);

	fix_duration_rear.file_duration = get_recorder_duration();
	fix_duration_rear.cb_get_next_file = cb_get_next_rear_file;
	fix_duration_rear.cb_file_closed = cb_rear_file_closed;
	memset(recorder_rearfilename, 0x00, 64);
	file_mgr_rear_wait(car_file_mgr, 400);
	ret = cb_get_next_rear_file(recorder_hd, recorder_rearfilename);
	if (ret < 0) {
		APP_LOG_W("ret:%d\n", ret);
		return ret;
	}
	file_mgr_wakeup(car_file_mgr, 10);
	APP_LOG_D("recorder_rearfilename:%s %d\n", recorder_rearfilename,
		fix_duration_rear.file_duration);
	if (recorder_hd) {
		ret = lb_recorder_ctrl(recorder_hd, LB_REC_SET_FIX_DURATION_PARA,
				(void *)(&fix_duration_rear));
		if (ret < 0) {
			APP_LOG_E("LB_REC_SET_FIX_DURATION_PARA failed!\n");
			return -1;
		}
		ret = lb_recorder_ctrl(recorder_hd, LB_REC_SET_OUTPUT_FILE,
				(void *)recorder_rearfilename);
		if (ret < 0) {
			APP_LOG_E("LB_REC_SET_OUTPUT_FILE failed!\n");
			return -1;
		}

		if (get_watermark_time_enable() == ENABLE_ON)
			watermark_time_rd(recorder_hd, p_tm);

		if (get_watermark_logo_enable() == ENABLE_ON)
			watermark_logo(recorder_hd, 14);

		recoder_enable_mute(recorder_rd, get_mute_enable());
		ret = lb_recorder_ctrl(recorder_hd, LB_REC_START, 0);
		if (ret < 0) {
			APP_LOG_E("LB_REC_START failed!");
			return -1;
		}
	} else
		APP_LOG_E("recorder_hd NULL\n");
	lock_rear_file_num = 0;

	return ret;
}


/**
 * recoder_stop - stop record
 * @recorder_hd: recorder handle,init by recoder_start
 *
 * This function use to stop record video & release recorder handle
 */
int recoder_stop(void *recorder_hd)
{
	int ret;

	if (recorder_hd) {
		if (get_recorder_status(recorder_hd) == RECORDER_STATUS_RECORD) {
			ret = lb_recorder_ctrl(recorder_hd, LB_REC_STOP, 0);
			if (ret < 0) {
				APP_LOG_E("LB_REC_STOP failed!\n");
				return -1;
			}
		}
	} else
		APP_LOG_E("recorder_hd err\n");

	return 0;
}

int recoder_enable_mute(void *recorder_hd, int mute_flag)
{
	int ret = -1;
	if (recorder_hd) {
		if (mute_flag)
			ret = lb_recorder_ctrl(recorder_hd, LB_REC_SET_AUDIO_MUTE,
					(void *)1);
		else
			ret = lb_recorder_ctrl(recorder_hd, LB_REC_SET_AUDIO_MUTE,
					(void *)0);
		if (ret < 0)
			APP_LOG_E("LB_REC_SET_AUDIO_MUTE failed!\n");
	} else
		APP_LOG_E("recorder_hd err\n");

	return ret;
}

int recorder_set_views(void *recorder_fd, void *recorder_rd, views_status_e value)
{
	int ret = -1;

	APP_LOG_D("value:%d\n", value);
	if (value == FRONT_VIEW) {
		_get_config_disp_para(&front_preview_para, FRONT_FULL_SCREEN);
		if (recorder_rd && get_av_status())
			preview_stop(recorder_rd);
		if (recorder_fd) {
			lb_recorder_ctrl(recorder_fd, LB_REC_SET_DISP_PARA,
				&front_preview_para);
			preview_start(recorder_fd);
			ret = lb_recorder_ctrl(recorder_fd, LB_REC_SET_LAYER_LEVEL,
					(void *)VIDEO_LAYER_BOTTOM);
			if (ret < 0) {
				APP_LOG_E("LB_REC_SET_LAYER_LEVEL failed!\n");
				return -1;
			}
		}

	} else if (value == BACK_VIEW) {
		_get_config_disp_para(&rear_preview_para, REAR_FULL_SCREEN);
		if (recorder_fd)
			preview_stop(recorder_fd);
		if (recorder_rd && get_av_status()) {
			lb_recorder_ctrl(recorder_rd, LB_REC_SET_DISP_PARA,
				&rear_preview_para);
			preview_start(recorder_rd);
			ret = lb_recorder_ctrl(recorder_rd, LB_REC_SET_LAYER_LEVEL,
					(void *)VIDEO_LAYER_BOTTOM);
			if (ret < 0) {
				APP_LOG_E("LB_REC_SET_LAYER_LEVEL failed!\n");
				return -1;
			}
		}

	}

	return 0;
}

int get_recorder_time(void *recorder_hd)
{
	int time = 0;
	if (recorder_hd)
		lb_recorder_ctrl(recorder_hd, LB_REC_GET_TIME, (void *)&time);
	else
		APP_LOG_E("recorder_hd err\n");

	return time;
}
int get_recorder_status(void *recorder_hd)
{
	int status = 0;

	if (recorder_hd)
		lb_recorder_ctrl(recorder_hd, LB_REC_GET_STATUS, (void *)&status);
	else
		APP_LOG_E("recorder_hd err\n");

	return status;
}

int tack_picture(int recoder_flag, struct tm *p_tm)
{
	int ret = -1;

	if (recoder_flag == FRONT_RECORDER) {

		if (recorder_fd) {
			ret = lb_recorder_ctrl(recorder_fd, LB_REC_TAKE_PICTURE,
					(void *)&picture_frontfilename);
			if (ret < 0)
				APP_LOG_E("LB_REC_TAKE_PICTURE err\n");
		} else
			APP_LOG_E("recorder_hd err\n");
	} else if (recoder_flag == REAR_RECORDER) {

		if (recorder_rd) {
			ret = lb_recorder_ctrl(recorder_rd, LB_REC_TAKE_PICTURE,
					(void *)&picture_rearfilename);
			if (ret < 0)
				APP_LOG_E("LB_REC_TAKE_PICTURE err\n");
		} else
			APP_LOG_E("recorder_hd err\n");
	} else {
		APP_LOG_E("err param\n");
		return ret;
	}
	return ret;
}
lb_int32 get_cfg_park_monitor_times(void)
{
	if (cr_cfg_jobj.park_monitor_times)
		return cr_cfg_jobj.park_monitor_times->valueint;

	return 0;
}

void park_monitor_times_clear(void)
{
	if (cr_cfg_jobj.park_monitor_times) {
		cr_cfg_jobj.park_monitor_times->valueint = 0;
		cr_cfg_jobj.park_monitor_times->valuedouble = 0;
	}
	cr_cfg_save();

	return;
}


lb_int32 get_watermark_logo_enable(void)
{
	if (cr_cfg_jobj.watermark_logo_enable) {
		if (cr_cfg_jobj.watermark_logo_enable->valueint >= ENABLE_OFF
			&& cr_cfg_jobj.watermark_logo_enable->valueint < ENABLE_MAX)
			return cr_cfg_jobj.watermark_logo_enable->valueint;
	}

	return ENABLE_OFF;
}

lb_int32 get_watermark_time_enable(void)
{
	if (cr_cfg_jobj.watermark_time_enable) {
		if (cr_cfg_jobj.watermark_time_enable->valueint >= ENABLE_OFF
			&& cr_cfg_jobj.watermark_time_enable->valueint < ENABLE_MAX)
			return cr_cfg_jobj.watermark_time_enable->valueint;
	}

	return ENABLE_OFF;
}

int watermark_set_source(void *rec, unsigned int source_num, char source_name[][64])
{
	int i;
	int ret = -1;
	if (wm_source.input_picture_num == 0) {
		wm_source.colorspace = OMX_COLOR_Format32bitBGRA8888;
		wm_source.input_picture_num = source_num;
		for (i = 0; i < wm_source.input_picture_num; i++) {
			p_img[i] = (img_dsc_t *)eimage_create_img_buf(source_name[i]);
			if (p_img[i]) {
				wm_source.numeral_picture[i].width = p_img[i]->header.w;
				wm_source.numeral_picture[i].height = p_img[i]->header.h;
				wm_source.numeral_picture[i].picture_size =
					p_img[i]->header.w * p_img[i]->header.h * 4;
				wm_source.numeral_picture[i].stride = p_img[i]->header.w;
				wm_source.numeral_picture[i].data = p_img[i]->data;
			} else {
				APP_LOG_E("open wm file error\n");
				return ret;
			}
		}
	}
	ret = lb_recorder_ctrl(rec, LB_REC_SET_WATERMARK_SOURCE, &wm_source);

	return ret;
}
int watermark_exit_set_source(unsigned int source_num)
{
	int i;
	int ret = -1;

	for (i = 0; i < source_num; i++) {
		if (p_img[i]) {
			eimage_destory_img_buf((void *)p_img[i]);
			p_img[i] = NULL;
		} else
			return ret;
	}
	ret = 0;

	return ret;
}
int move_cdr_disp_para(void *recorder_hd, win_para_t *cdr_disp_para, int num)
{
	int ret;

	cdr_disp_para->rect.x = 0;
	cdr_disp_para->rect.y = 0;
#ifdef SCREEN_ROT_90
	cdr_disp_para->rect.width = cdr_screen.height;
	cdr_disp_para->rect.height = cdr_screen.width;
	cdr_disp_para->crop.x = cdr_disp_para->crop.x + num;
	if (recorder_fd == recorder_hd) {
		if (cdr_disp_para->crop.x <= 0)
			cdr_disp_para->crop.x = 0;
		else if (cdr_disp_para->crop.x >= cdr_screen.rear_max_crop_x)
			cdr_disp_para->crop.x = cdr_screen.rear_max_crop_x;
		cdr_disp_para->crop.width =  cdr_screen.height * REAR_PREVIEW_WIDTH /
			cdr_screen.width;
		cdr_disp_para->crop.height = REAR_PREVIEW_WIDTH;
	} else {
		if (cdr_disp_para->crop.x <= 1080)
			cdr_disp_para->crop.x = 1080;
		else if (cdr_disp_para->crop.x >= (cdr_screen.front_max_crop_x + 1080))
			cdr_disp_para->crop.x = cdr_screen.front_max_crop_x  + 1080;
		cdr_disp_para->crop.width =  cdr_screen.height * FRONT_PREVIEW_WIDTH /
			cdr_screen.width;
		cdr_disp_para->crop.height = FRONT_PREVIEW_WIDTH;
	}
#else
	cdr_disp_para->rect.width = cdr_screen.width;
	cdr_disp_para->rect.height = cdr_screen.height;
	cdr_disp_para->crop.y = cdr_disp_para->crop.y + num;
	APP_LOG_D("cdr_disp_para.crop.x %d\n", cdr_disp_para->crop.x);
	if (recorder_rd == recorder_hd) {
		if (cdr_disp_para->crop.y <= 0)
			cdr_disp_para->crop.y = 0;
		else if (cdr_disp_para->crop.y >= cdr_screen.rear_max_crop_y)
			cdr_disp_para->crop.y = cdr_screen.rear_max_crop_y;
		cdr_disp_para->crop.width =  REAR_PREVIEW_WIDTH;
		cdr_disp_para->crop.height = cdr_screen.height * REAR_PREVIEW_WIDTH /
			cdr_screen.width;
	} else {
		if (cdr_disp_para->crop.y <= 0)
			cdr_disp_para->crop.y = 0;
		else if (cdr_disp_para->crop.y >= cdr_screen.front_max_crop_y)
			cdr_disp_para->crop.y = cdr_screen.front_max_crop_y;
		cdr_disp_para->crop.width =  FRONT_PREVIEW_WIDTH;
		cdr_disp_para->crop.height = cdr_screen.height * FRONT_PREVIEW_WIDTH /
			cdr_screen.width;
	}
#endif
	cdr_disp_para->mode = VIDEO_WINDOW_USERDEF;

	ret = lb_recorder_ctrl(recorder_hd, LB_REC_SET_DISP_PARA, cdr_disp_para);

	return 0;
}

int watermark_time_fd(void *rec, struct tm *p_tm)
{
	numeral_picture_index_t watermark;
	numeral_picture_index_t *wm;
	int ret;
	watermark.total_index_num = 19;
	watermark.start_x_pos = 48;//55
	if(rec_resolution==0){  //720 
	watermark.start_y_pos = 720-64*2;//20
	
}else{
      watermark.start_y_pos = 1080-64*2;//20
		}	
	
	watermark.blending_area_index = 0;

	watermark.index_array[0] = ((p_tm->tm_year + 1900) / 1000) % 10;
	watermark.index_array[1] = ((p_tm->tm_year + 1900) / 100) % 10;
	watermark.index_array[2] = ((p_tm->tm_year + 1900) / 10) % 10;
	watermark.index_array[3] = (p_tm->tm_year + 1900) % 10;
	watermark.index_array[4] = XIEGANG;
	watermark.index_array[5] = ((p_tm->tm_mon + 1) / 10) % 10;
	watermark.index_array[6] = (p_tm->tm_mon + 1) % 10;
	watermark.index_array[7] = XIEGANG;
	watermark.index_array[8] = (p_tm->tm_mday / 10) % 10;
	watermark.index_array[9] = p_tm->tm_mday % 10;
	watermark.index_array[10] = TEMPLE;
	watermark.index_array[11] = (p_tm->tm_hour / 10) % 10;
	watermark.index_array[12] = p_tm->tm_hour % 10;
	watermark.index_array[13] = MAOHAO;
	watermark.index_array[14] = (p_tm->tm_min / 10) % 10;
	watermark.index_array[15] = p_tm->tm_min % 10;
	watermark.index_array[16] = MAOHAO;
	watermark.index_array[17] = (p_tm->tm_sec / 10) % 10;
	watermark.index_array[18] = p_tm->tm_sec % 10;

	wm = &watermark;
	ret = lb_recorder_ctrl(rec, LB_REC_SET_WATERMARK_INDEX, wm);
	return ret;
}

int watermark_time_rd(void *rec, struct tm *p_tm)
{
	numeral_picture_index_t watermark;
	numeral_picture_index_t *wm;
	int ret;
	watermark.total_index_num = 19;
	watermark.start_x_pos = 48;//55
	watermark.start_y_pos = REAR_RECORDER_SOURCE_HEIGHT-64*2;//20
	watermark.blending_area_index = 0;

	watermark.index_array[0] = ((p_tm->tm_year + 1900) / 1000) % 10;
	watermark.index_array[1] = ((p_tm->tm_year + 1900) / 100) % 10;
	watermark.index_array[2] = ((p_tm->tm_year + 1900) / 10) % 10;
	watermark.index_array[3] = (p_tm->tm_year + 1900) % 10;
	watermark.index_array[4] = XIEGANG;
	watermark.index_array[5] = ((p_tm->tm_mon + 1) / 10) % 10;
	watermark.index_array[6] = (p_tm->tm_mon + 1) % 10;
	watermark.index_array[7] = XIEGANG;
	watermark.index_array[8] = (p_tm->tm_mday / 10) % 10;
	watermark.index_array[9] = p_tm->tm_mday % 10;
	watermark.index_array[10] = TEMPLE;
	watermark.index_array[11] = (p_tm->tm_hour / 10) % 10;
	watermark.index_array[12] = p_tm->tm_hour % 10;
	watermark.index_array[13] = MAOHAO;
	watermark.index_array[14] = (p_tm->tm_min / 10) % 10;
	watermark.index_array[15] = p_tm->tm_min % 10;
	watermark.index_array[16] = MAOHAO;
	watermark.index_array[17] = (p_tm->tm_sec / 10) % 10;
	watermark.index_array[18] = p_tm->tm_sec % 10;

	wm = &watermark;
	ret = lb_recorder_ctrl(rec, LB_REC_SET_WATERMARK_INDEX, wm);
	return ret;
}



int watermark_logo(void *rec, int index)
{
	numeral_picture_index_t watermark;
	numeral_picture_index_t *wm;
	int ret;

	watermark.total_index_num = 1;
	watermark.start_x_pos = 20;
	watermark.start_y_pos = 20;
	watermark.blending_area_index = 1;

	watermark.index_array[0] = index;

	wm = &watermark;
	if (index >= WATERMARK_SOURCE_NUM)
		wm = NULL;
	ret = lb_recorder_ctrl(rec, LB_REC_SET_WATERMARK_INDEX, wm);

	return ret;
}
int pano_get_enable(void)
{
	return cr_status.pano_enable;
}
int adas_get_enable()
{
	return cr_status.adas_enable;
}

int pano_get_status(void)
{
	return pano_status;
}

int bsd_get_enable()
{
	return cr_status.bsd_enable;
}
static lb_int32 load_data(lb_int32 size, lb_uint8 *data, FILE *file)
{
	lb_int32 ret = 0;
	lb_int32 len = 0;

	if (file == NULL) {
		APP_LOG_E("failed\n");
		ret = -1;
		goto exit;
	}

	if (data && size) {
		len = fread(data, 1, size, file);
		if (len != size) {
			APP_LOG_E("failed\n");
			ret = -1;
			goto exit;
		}
	}

exit:
	return ret;
}

static lb_int32 load_bin(pano_out_t *pano_out)
{
	lb_int32 ret = 0;
	FILE *file = NULL;

	file = fopen(PANO_OUT_BIN_PATH, "rb");
	if (file == NULL) {
		APP_LOG_E("failed\n");
		ret = -1;
		goto exit;
	}
	fseek(file, 0, SEEK_SET);

	ret = load_data(sizeof(lb_int32), (lb_uint8 *)&pano_out->use_ext_cutline, file);
	if (ret != 0) {
		APP_LOG_E("failed\n");
		ret = -1;
		goto exit;
	}

	ret = load_data(sizeof(cali_contex_t), (lb_uint8 *)&pano_out->cali_ctx, file);
	if (ret != 0) {
		APP_LOG_E("failed\n");
		ret = -1;
		goto exit;
	}

	ret = load_data(sizeof(pano_set_t), (lb_uint8 *)&pano_out->set, file);
	if (ret != 0) {
		APP_LOG_E("failed\n");
		ret = -1;
		goto exit;
	}
	APP_LOG_D("***load_bin************************\n*");
	APP_LOG_D("***use_ext_cutline:%d\n", pano_out->use_ext_cutline);
	APP_LOG_D("***cali_ctx.cutline:%d\n", pano_out->cali_ctx.cutline);
	APP_LOG_D("***cali_ctx.cutline_dnthr:%d\n", pano_out->cali_ctx.cutline_dnthr);
	APP_LOG_D("***cali_ctx.cutline_upthr:%d\n", pano_out->cali_ctx.cutline_upthr);
	APP_LOG_D("***set.motor_w:%d\n", (lb_int32)(pano_out->set.motor_w * 100));
	APP_LOG_D("***set.motor_l:%d\n", (lb_int32)(pano_out->set.motor_l * 100));
	APP_LOG_D("***set.motor_d2r:%d\n", pano_out->set.motor_d2r);
	APP_LOG_D("***load_bin************************\n");

	ret = load_data(4, (lb_uint8 *)&pano_out->cali_out.data_size, file);
	if (ret != 0) {
		APP_LOG_E("failed\n");
		ret = -1;
		goto exit;
	}

	pano_out->cali_out.data = malloc(pano_out->cali_out.data_size);
	if (pano_out->cali_out.data == NULL) {
		APP_LOG_E("failed\n");
		ret = -1;
		goto exit;
	}

	ret = load_data(pano_out->cali_out.data_size,
			(lb_uint8 *)pano_out->cali_out.data, file);
	if (ret != 0) {
		APP_LOG_E("failed\n");
		ret = -1;
		goto exit;
	}

	if (file) {
		fclose(file);
		file = NULL;
	}

	return ret;

exit:
	if (file) {
		fclose(file);
		file = NULL;
	}

	if (pano_out->cali_out.data) {
		free(pano_out->cali_out.data);
		pano_out->cali_out.data = NULL;
	}

	return ret;
}

static lb_int32 load_para(pano_param_t *init_para)
{
	lb_int32 ret = 0;

	init_para->in_gps = 0;
	init_para->in_obd = 0;
	init_para->car_para_en = 1;
	init_para->car_width = pano_out.set.motor_w;
	init_para->data_format = NULL;
	init_para->carboard_img.width = PANO_CARIMG_W;
	init_para->carboard_img.height = PANO_CARIMG_H;
	init_para->warning_line[0] = 30;
	init_para->warning_line[1] = 80;
	init_para->warning_line[2] = 150;

	strncpy(init_para->carboard_img.format, "nv12",
		sizeof(init_para->carboard_img.format) - 1);

#if 0
	strncpy(init_para->carboard_img.path, ROOTFS_MOUNT_PATH"/res/car_yuv.bin",
		sizeof(init_para->carboard_img.path) - 1);
#else
	memset(init_para->carboard_img.path, 0x00,
		sizeof(init_para->carboard_img.path));
#endif
	init_para->use_ext_cutline = pano_out.use_ext_cutline;
	init_para->culine = pano_out.cali_ctx.cutline;

	return ret;
}

static lb_int32 open_lyr(pano_out_t *pano_out)
{
	lb_int32 ret = 0;

	RT_ASSERT(pano_out != NULL);

	pano_out->disp_device = rt_device_find("disp");
	RT_ASSERT(pano_out->disp_device != NULL);
	rt_device_open(pano_out->disp_device, 0);

	pano_out->disp_ops = (rt_device_disp_ops_t *)(pano_out->disp_device->user_data);
	RT_ASSERT(pano_out->disp_ops != NULL);

	pano_out->disp_ctl = pano_out->disp_ops->disp_win_request("car_lyr");
	RT_ASSERT(pano_out->disp_ctl != NULL);

	return ret;
}

static lb_int32 close_lyr(pano_out_t *pano_out)
{
	lb_int32 ret = 0;

	RT_ASSERT(pano_out != NULL);

	pano_out->disp_ops = (rt_device_disp_ops_t *)(pano_out->disp_device->user_data);
	RT_ASSERT(pano_out->disp_ops != RT_NULL);

	RT_ASSERT(pano_out->disp_ctl != NULL);
	pano_out->disp_ops->disp_win_release(&pano_out->disp_ctl);

	RT_ASSERT(pano_out->disp_device != NULL);
	rt_device_close(pano_out->disp_device);

	return ret;
}

static void config_lyr(pano_out_t *pano_out, lb_int32 width, lb_int32 height)
{
	RT_ASSERT(pano_out != NULL);

	rt_memset(&pano_out->win_data, 0, sizeof(dc_win_data_t));

	pano_out->win_data.pixel_format = DISP_FORMAT_ARGB8888;
	pano_out->win_data.pixel_order = DC_PO_NORMAL;
	pano_out->win_data.bpp = 32;

	RT_ASSERT(pano_out != NULL);
	pano_out->win_data.crtc_x =
		PANO_BIRDBIEW_WIND_W - pano_out->cali_ctx.car_rect.y - width;
	pano_out->win_data.crtc_y =
		pano_out->cali_ctx.car_rect.x + PANO_BIRDBIEW_WIND_Y;
	pano_out->win_data.crtc_width =
		width;
	pano_out->win_data.crtc_height =
		height;

	pano_out->win_data.fb_x = 0;
	pano_out->win_data.fb_y = 0;
	pano_out->win_data.fb_width = pano_out->win_data.crtc_width;
	pano_out->win_data.fb_height = pano_out->win_data.crtc_height;

	pano_out->win_data.src_width = pano_out->win_data.crtc_width;
	pano_out->win_data.src_height = pano_out->win_data.crtc_height;

	pano_out->win_data.update_flag = true;
}

static void update_lyr(pano_out_t *pano_out)
{
	RT_ASSERT(pano_out != NULL);

#if defined(SCALER_CAR)
	pano_out->win_data.dma_addr = (dma_addr_t)pano_out->sca_buf;
#elif defined(ROTATE_CAR)
	pano_out->win_data.dma_addr = (dma_addr_t)pano_out->rot_buf;
#elif defined(ORIGIN_CAR)
	pano_out->win_data.dma_addr = (dma_addr_t)pano_out->ori_buf;
#endif
	pano_out->win_data.chroma_dma_addr = (dma_addr_t)0;
	pano_out->win_data.chroma_x_dma_addr = (dma_addr_t)0;

	RT_ASSERT(pano_out->disp_ops != NULL);
	pano_out->disp_ops->disp_win_update(pano_out->disp_ctl, &pano_out->win_data);
}

static lb_int32 open_img(pano_out_t *pano_out)
{
	lb_int32 ret = 0;

	RT_ASSERT(pano_out != NULL);

	pano_out->car_img = (img_dsc_t *)eimage_create_img_buf(BIRD_VIEW_PATH);
	RT_ASSERT(pano_out->car_img != 0);

	return ret;
}

static lb_int32 close_img(pano_out_t *pano_out)
{
	lb_int32 ret = 0;

	RT_ASSERT(pano_out != NULL);

	RT_ASSERT(pano_out->car_img != 0);
	eimage_destory_img_buf((void *)pano_out->car_img);

	return ret;
}

#ifdef SCALER_CAR
static lb_int32 sca_img(pano_out_t *pano_out,
	lb_int8 *psrc, lb_uint32 src_w, lb_uint32 src_h,
	lb_int8 *pdst, lb_uint32 dst_w, lb_uint32 dst_h)
{
	lb_int32 ret = 0;
	dc_win_data_t se_data;
	lb_uint32 addr[3];
	disp_se_t *se = NULL;

	RT_ASSERT(pano_out != NULL);

	rt_memset(&se_data, 0, sizeof(dc_win_data_t));

	se_data.pixel_format = DISP_FORMAT_ARGB8888;
	se_data.pixel_order = DC_PO_NORMAL;
	se_data.bpp = 32;

	se_data.crtc_x = 0;
	se_data.crtc_y = 0;
	se_data.crtc_width = dst_w;
	se_data.crtc_height = dst_h;

	se_data.fb_x = 0;
	se_data.fb_y = 0;
	se_data.fb_width = src_w;
	se_data.fb_height = src_h;

	se_data.src_width = src_w;
	se_data.src_height = src_h;

	se_data.dma_addr = (dma_addr_t)psrc;
	se_data.chroma_dma_addr = (dma_addr_t)0;
	se_data.chroma_x_dma_addr = (dma_addr_t)0;

	se = pano_out->disp_ops->disp_se_request();
	RT_ASSERT(se != NULL);

	addr[0] = (lb_uint32)pdst;
	addr[1] = 0;
	addr[2] = 0;

	ret = pano_out->disp_ops->disp_se_process(se,
		&se_data, addr, DISP_FORMAT_ARGB8888);
	RT_ASSERT(ret == 0);

	ret = pano_out->disp_ops->disp_se_release(&se);
	RT_ASSERT(ret == 0);

	return ret;
}
#endif

#ifdef ROTATE_CAR
static lb_int32 rot_img(pano_out_t *pano_out,
	lb_int8 *psrc, lb_uint32 src_w, lb_uint32 src_h,
	lb_int8 *pdst, lb_uint32 dst_w, lb_uint32 dst_h)
{
	lb_int32 ret = 0;

	RT_ASSERT(pano_out != NULL);

	memset(&pano_out->rot_cfg, 0x00, sizeof(disp_rot_cfg_t));

	pano_out->rot_cfg.mode = LOMBO_DRM_TRANSFORM_ROT_90;
	pano_out->rot_cfg.infb.format = DISP_FORMAT_ARGB8888;
	pano_out->rot_cfg.outfb.format = DISP_FORMAT_ARGB8888;

	pano_out->rot_cfg.infb.addr[0] = (unsigned int)psrc;
	pano_out->rot_cfg.infb.width[0] = src_w;
	pano_out->rot_cfg.infb.height[0] = src_h;
	pano_out->rot_cfg.outfb.addr[0] = (unsigned int)pdst;
	pano_out->rot_cfg.outfb.width[0] = dst_w;
	pano_out->rot_cfg.outfb.height[0] = dst_h;
	pano_out->rot_cfg.rot_way = SW_ROT;

	RT_ASSERT(pano_out->disp_ops != NULL);
	pano_out->disp_ops->disp_rot_process(&pano_out->rot_cfg);

	return ret;
}
#endif

static lb_int32 show_hbtb(pano_out_t *pano_out)
{
	lb_int32 ret = 0;
	app_t *app_hd = NULL;
	app_hd = lb_app_check("home");
	if (app_hd)
		lb_app_ctrl(app_hd, MSG_FROM_SHOW_HBTB, 0, NULL);

	return ret;
}

static lb_int32 hide_hbtb(pano_out_t *pano_out)
{
	lb_int32 ret = 0;

	app_t *app_hd = NULL;
	app_hd = lb_app_check("home");
	if (app_hd)
		lb_app_ctrl(app_hd, MSG_FROM_HIDE_HBTB, 0, NULL);

	return ret;
}

static lb_int32 show_car(pano_out_t *pano_out)
{
	lb_int32 ret = 0;

	lb_uint32 ori_w = 0;
	lb_uint32 ori_h = 0;
	lb_uint32 rot_w = 0;
	lb_uint32 rot_h = 0;
	lb_uint32 sca_w = 0;
	lb_uint32 sca_h = 0;

	RT_ASSERT(pano_out != NULL);

	ret = open_img(pano_out);
	RT_ASSERT(ret == 0);

	RT_ASSERT(pano_out->car_img->data != NULL);
	pano_out->ori_buf = pano_out->car_img->data;
	pano_out->rot_buf = pano_out->car_img->data;
	pano_out->sca_buf = pano_out->car_img->data;

	RT_ASSERT(pano_out->car_img->header.w != 0);
	RT_ASSERT(pano_out->car_img->header.h != 0);
	sca_w = rot_w = ori_w = pano_out->car_img->header.w;
	sca_h = rot_h = ori_h = pano_out->car_img->header.h;

	ret = open_lyr(pano_out);
	RT_ASSERT(ret == 0);

#ifdef ROTATE_CAR
	rot_w = ori_h;
	rot_h = ori_w;

	pano_out->rot_buf = rt_malloc_align(rot_w * rot_h * 4, 32);
	RT_ASSERT(pano_out->rot_buf != NULL);
	memset(pano_out->rot_buf, 0x00, rot_w * rot_h * 4);

	ret = rot_img(pano_out,
		(lb_int8 *)pano_out->car_img->data, ori_w, ori_h,
		(lb_int8 *)pano_out->rot_buf, rot_w, rot_h);
	RT_ASSERT(ret == 0);
#endif

#ifdef SCALER_CAR
	sca_w = (pano_out->cali_ctx.car_rect.height >> 2) << 2;
	sca_h = (pano_out->cali_ctx.car_rect.width >> 0) << 0;

	pano_out->sca_buf = rt_malloc_align(sca_w * sca_h * 4, 32);
	RT_ASSERT(pano_out->sca_buf != NULL);
	memset(pano_out->sca_buf, 0x00, sca_w * sca_h * 4);

	ret = sca_img(pano_out,
		(lb_int8 *)pano_out->rot_buf, rot_w, rot_h,
		(lb_int8 *)pano_out->sca_buf, sca_w, sca_h);
	RT_ASSERT(ret == 0);
#endif


#if defined(SCALER_CAR)
	config_lyr(pano_out, sca_w, sca_h);
#elif defined(ROTATE_CAR)
	config_lyr(pano_out, rot_w, rot_h);
#elif defined(ORIGIN_CAR)
	config_lyr(pano_out, ori_w, ori_h);
#endif

	update_lyr(pano_out);

	return ret;
}

static lb_int32 hide_car(pano_out_t *pano_out)
{
	lb_int32 ret = 0;
	lb_uint32 ori_w = 0;
	lb_uint32 ori_h = 0;
	lb_uint32 rot_w = 0;
	lb_uint32 rot_h = 0;
	lb_uint32 sca_w = 0;
	lb_uint32 sca_h = 0;

	RT_ASSERT(pano_out != NULL);
	RT_ASSERT(pano_out->car_img->header.w != 0);
	RT_ASSERT(pano_out->car_img->header.h != 0);

	sca_w = rot_w = ori_w = pano_out->car_img->header.w;
	sca_h = rot_h = ori_h = pano_out->car_img->header.h;

	ret = close_lyr(pano_out);
	RT_ASSERT(ret == 0);

	ret = close_img(pano_out);
	RT_ASSERT(ret == 0);

#ifdef ROTATE_CAR
	rot_w = ori_h;
	rot_h = ori_w;

	if (pano_out->rot_buf) {
		memset(pano_out->rot_buf, 0x00, rot_w * rot_h * 4);
		rt_free_align(pano_out->rot_buf);
		pano_out->rot_buf = NULL;
	}
#endif

#ifdef SCALER_CAR
	sca_w = (pano_out->cali_ctx.car_rect.height >> 2) << 2;
	sca_h = (pano_out->cali_ctx.car_rect.width >> 0) << 0;

	if (pano_out->sca_buf) {
		memset(pano_out->sca_buf, 0x00, sca_w * sca_h * 4);
		rt_free_align(pano_out->sca_buf);
		pano_out->sca_buf = NULL;
	}
#endif
	return ret;
}

static lb_int32 pano_load(pano_out_t *pano_out)
{
	lb_int32 ret = 0;

	ret = load_bin(pano_out);
	if (ret != 0) {
		APP_LOG_E("failed\n");
		ret = -1;
		goto exit;
	}

exit:
	return ret;
}

static lb_int32 pano_back(pano_out_t *pano_out)
{
	lb_int32 ret = 0;

	if (pano_out == NULL) {
		APP_LOG_E("failed\n");
		ret = -1;
		goto exit;
	}

	if (pano_out->cali_out.data_size == 0) {
		APP_LOG_E("failed\n");
		ret = -1;
		goto exit;
	}

	if (pano_out->cali_out.data == NULL) {
		APP_LOG_E("failed\n");
		ret = -1;
		goto exit;
	}

	free(pano_out->cali_out.data);
	pano_out->cali_out.data = NULL;
	pano_out->cali_out.data_size = 0;

exit:
	return ret;
}

static lb_int32 prev_init(void *hdl)
{
	lb_int32 ret = 0;
	win_para_t disp_para;

	memset(&disp_para, 0x00, sizeof(win_para_t));

	if (hdl == NULL) {
		APP_LOG_E("failed\n");
		ret = -1;
		goto exit;
	}

	_get_config_disp_para(&disp_para, PANO_BEFORE_PROC);

	ret = lb_recorder_ctrl(hdl, LB_REC_SET_DISP_PARA,
			(void *)&disp_para);
	if (ret != 0) {
		APP_LOG_E("failed\n");
		ret = -1;
		goto exit;
	}

	ret = lb_recorder_ctrl(hdl, LB_REC_PREVIEW, 0);
	if (ret != 0) {
		APP_LOG_E("failed\n");
		ret = -1;
		goto exit;
	}

	ret = lb_recorder_ctrl(hdl, LB_REC_SET_LAYER_LEVEL,
			(void *)VIDEO_LAYER_BOTTOM);
	if (ret != 0) {
		APP_LOG_E("failed\n");
		ret = -1;
		goto exit;
	}

exit:
	return ret;
}

static lb_int32 prev_exit(void *hdl)
{
	lb_int32 ret = 0;

	if (hdl == NULL) {
		APP_LOG_E("failed\n");
		ret = -1;
		goto exit;
	}

	ret = lb_recorder_ctrl(hdl, LB_REC_STOP_PREVIEW, 0);
	if (ret != 0) {
		APP_LOG_E("failed\n");
		ret = -1;
	}

exit:
	return ret;
}

static lb_int32 pano_init(void *hdl)
{
	lb_int32 ret = 0;
	pano_param_t init_para;
	win_para_t win_para;
	vsize_t prev_size;

	memset(&init_para, 0x00, sizeof(pano_param_t));
	memset(&win_para, 0x00, sizeof(win_para_t));
	memset(&prev_size, 0x00, sizeof(vsize_t));

	if (hdl == NULL) {
		APP_LOG_E("failed\n");
		ret = -1;
		goto exit;
	}

	ret = lb_recorder_ctrl(hdl, LB_REC_PANO_CREAT, NULL);
	if (ret != 0) {
		APP_LOG_E("failed\n");
		ret = -1;
		goto exit;
	}

	ret = load_para(&init_para);
	if (ret != 0) {
		APP_LOG_E("failed\n");
		ret = -1;
		goto exit;
	}

	ret = lb_recorder_ctrl(hdl, LB_REC_PANO_SET_INIT_PARA,
			(void *)&init_para);
	if (ret != 0) {
		APP_LOG_E("failed\n");
		ret = -1;
		goto exit;
	}

	_get_config_disp_para(&win_para, PANO_AFTER_PROC);

	ret = lb_recorder_ctrl(hdl, LB_REC_PANO_SET_DISP_MODE,
			(void *)&win_para);
	if (ret != 0) {
		APP_LOG_E("failed\n");
		ret = -1;
		goto exit;
	}

	prev_size.width = PANO_BIRDBIEW_SOURCE_W;
	prev_size.height = PANO_BIRDBIEW_SOURCE_H;

	ret = lb_recorder_ctrl(hdl, LB_REC_PANO_SET_PREVIEW_SIZE,
			(void *)&prev_size);
	if (ret != 0) {
		APP_LOG_E("failed\n");
		ret = -1;
		goto exit;
	}

	ret = lb_recorder_ctrl(hdl, LB_REC_PANO_SET_CALI_DATA,
			&pano_out.cali_out);
	if (ret != 0) {
		APP_LOG_E("failed\n");
		ret = -1;
		goto exit;
	}

	ret = lb_recorder_ctrl(hdl, LB_REC_PANO_START, NULL);
	if (ret != 0) {
		APP_LOG_E("failed\n");
		ret = -1;
		goto exit;
	}

	ret = lb_recorder_ctrl(hdl, LB_REC_PANO_SET_LAYER_LEVEL,
			(void *)VIDEO_LAYER_BOTTOM);
	if (ret != 0) {
		APP_LOG_E("failed\n");
		ret = -1;
		goto exit;
	}

exit:
	return ret;
}

static lb_int32 pano_exit(void *hdl)
{
	lb_int32 ret = 0;

	if (hdl == NULL) {
		APP_LOG_E("failed\n");
		ret = -1;
		goto exit;
	}

	ret = lb_recorder_ctrl(hdl, LB_REC_PANO_STOP, NULL);
	if (ret != 0) {
		APP_LOG_E("failed\n");
		ret = -1;
	}

	ret = lb_recorder_ctrl(hdl, LB_REC_PANO_RELEASE, NULL);
	if (ret != 0) {
		APP_LOG_E("failed\n");
		ret = -1;
	}

exit:
	return ret;
}

int pano_start(void *hdl)
{
	lb_int32 ret = 0;

	pano_status = 1;
	ret = prev_init(hdl);
	RT_ASSERT(ret == 0);

	ret = pano_load(&pano_out);
	RT_ASSERT(ret == 0);

	ret = hide_hbtb(&pano_out);
	RT_ASSERT(ret == 0);

	ret = show_car(&pano_out);
	RT_ASSERT(ret == 0);

	ret = pano_init(hdl);
	RT_ASSERT(ret == 0);

	return ret;
}

int pano_stop(void *hdl)
{
	lb_int32 ret = 0;

	ret = hide_car(&pano_out);
	RT_ASSERT(ret == 0);

	ret = show_hbtb(&pano_out);
	RT_ASSERT(ret == 0);

	ret = pano_exit(hdl);
	RT_ASSERT(ret == 0);

	ret = pano_back(&pano_out);
	RT_ASSERT(ret == 0);

	ret = prev_exit(hdl);
	RT_ASSERT(ret == 0);

	pano_status = 0;
	return ret;
}

int pano_check(void)
{
	FILE *file = NULL;
	int ret = 0;

	file = fopen(PANO_OUT_BIN_PATH, "rb");
	if (file == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

exit:
	if (file) {
		fclose(file);
		file = NULL;
	}

	return ret;
}
#ifdef LOMBO_GPS
int  get_gps_speed(struct gps_data_t *g)
{
	rt_bool_t status;
	int ret = -1;

	*g = gps_get_data();
	status = gps_connect_status();
	if (!status)
		ret = -1;
	else if (g->valid)
		ret = 0;
	else
		ret = -2;

	return ret;
}
#endif

