/*
 * home_common.c - home common status & vlaue api
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "home.h"
#include "home_common.h"
#include "home_ctrl.h"
#include "lb_gal_common.h"
#include "lb_ui.h"
#include "lb_types.h"
#include <rtthread.h>
#include "lombo_disp.h"
#include "audio/asoundlib.h"
#include "wdog/wdog.h"
#include <drivers/watchdog.h>
#include "case_common.h"
#include "lb_recorder.h"


static home_cfg_jobj_t home_cfg_jobj;
static home_status_t home_status;
static rt_device_t disp_device;
/* open wdog in RELEASE__MP mode */
#ifdef __EOS__RELEASE__MP__
static rt_device_t wdog_dev;
#endif

#ifndef ARCH_LOMBO_N7_CDR_MMC
static pthread_t format_ope_id;
static pthread_t format_ref_id;
static lb_int32 need_exit;

static lb_int32 format_ope_init(void);
static lb_int32 format_ope_exit(void);
#endif

/**
 * home_config_array_init - get config parameter from config array
 * @para: Json array
 *
 * This function use to init config parameter from config array
 *
 */
static void home_config_array_init(cJSON *para)
{
	lb_int32 i = 0;

	for (i = 0; i < cJSON_GetArraySize(para); i++) {
		cJSON *temp;

		temp = cJSON_GetArrayItem(para, i);
		if (temp && temp->type == cJSON_Object) {
			if (cJSON_GetObjectItem(temp, "lcd_brightness")) {
				home_cfg_jobj.lcd_brightness = cJSON_GetObjectItem(temp,
						"lcd_brightness");
				APP_LOG_D("home_cfg_jobj.lcd_brightness:%d\n",
					home_cfg_jobj.lcd_brightness->valueint);
			} else if (cJSON_GetObjectItem(temp, "volume")) {
				home_cfg_jobj.volume = cJSON_GetObjectItem(temp,
						"volume");
				APP_LOG_D("home_cfg_jobj.volume:%d\n",
					home_cfg_jobj.volume->valueint);
			} else if (cJSON_GetObjectItem(temp, "lcd_standby_time")) {
				home_cfg_jobj.lcd_standby_time = cJSON_GetObjectItem(temp,
						"lcd_standby_time");
				APP_LOG_D("home_cfg_jobj.lcd_standby_time:%d\n",
					home_cfg_jobj.lcd_standby_time->valueint);
			} else if (cJSON_GetObjectItem(temp, "keytone_enable")) {
				home_cfg_jobj.keytone_enable = cJSON_GetObjectItem(temp,
						"keytone_enable");
				APP_LOG_D("home_cfg_jobj.keytone_enable:%d\n",
					home_cfg_jobj.keytone_enable->valueint);
			} else if (cJSON_GetObjectItem(temp, "fastboot_enable")) {
				home_cfg_jobj.fastboot_enable = cJSON_GetObjectItem(temp,
						"fastboot_enable");
				APP_LOG_D("home_cfg_jobj.fastboot_enable:%d\n",
					home_cfg_jobj.fastboot_enable->valueint);
			} else if (cJSON_GetObjectItem(temp, "language")) {
				home_cfg_jobj.language = cJSON_GetObjectItem(temp,
						"language");
				APP_LOG_D("home_cfg_jobj.language:%d\n",
					home_cfg_jobj.language->valueint);
			} else if (cJSON_GetObjectItem(temp, "interval_record")) {
				home_cfg_jobj.interval_record = cJSON_GetObjectItem(temp,
						"interval_record");
				APP_LOG_D("home_cfg_jobj.interval_record:%d\n",
					home_cfg_jobj.interval_record->valueint);
			}
		}
	}
}

/**
 * home_cfg_init - get config parameter from file
 *
 * This function use to init config parameter from config file
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
lb_int32 home_cfg_init(void)
{
	lb_int32 ret = 0;

#ifdef ARCH_LOMBO_N7_CDR_MMC
	home_cfg_jobj.cfg_root =
		lb_open_cfg_file(ROOTFS_MOUNT_PATH"/etc/cdr_config1.cfg");
#else
	home_cfg_jobj.cfg_root = lb_open_cfg_file("/mnt/data/cdr_config.cfg");
#endif
	if (home_cfg_jobj.cfg_root == NULL) {
		home_cfg_jobj.cfg_root =
			lb_open_cfg_file(ROOTFS_MOUNT_PATH"/etc/cdr_config.cfg");
		if (home_cfg_jobj.cfg_root == NULL) {
			APP_LOG_E("home_cfg_jobj.cfg_root is NULL");
			return -1;
		}
	}
	RT_ASSERT(home_cfg_jobj.cfg_root != NULL)

	home_cfg_jobj.system_para = cJSON_GetObjectItem(home_cfg_jobj.cfg_root, "system");
	RT_ASSERT(home_cfg_jobj.system_para  != NULL)

	if (home_cfg_jobj.system_para && home_cfg_jobj.system_para->type == cJSON_Array)
		home_config_array_init(home_cfg_jobj.system_para);

	home_cfg_jobj.record_para = cJSON_GetObjectItem(home_cfg_jobj.cfg_root, "record");
	RT_ASSERT(home_cfg_jobj.record_para  != NULL)

	if (home_cfg_jobj.record_para && home_cfg_jobj.record_para->type == cJSON_Array)
		home_config_array_init(home_cfg_jobj.record_para);

	home_cfg_jobj.version_cfg = cJSON_GetObjectItem(home_cfg_jobj.cfg_root,
			"version");
	RT_ASSERT(home_cfg_jobj.version_cfg  != NULL)

	return ret;
}

/**
 * home_cfg_save - save config parameter to file
 *
 * This function use to save config parameter for config file
 *
 * Returns -1 if called when get error ; otherwise, return 1
 */
lb_int32 home_cfg_save(void)
{
	lb_int32 ret;

#ifdef ARCH_LOMBO_N7_CDR_MMC
	ret = lb_save_cfg_file("/mnt/sdcard/etc/cdr_config1.cfg", home_cfg_jobj.cfg_root);
#else
	ret = lb_save_cfg_file("/mnt/data/cdr_config.cfg", home_cfg_jobj.cfg_root);
#endif

	return ret;
}

/**
 * home_cfg_exit - exit config file
 *
 * This function use to exit config file,if you want  call it,
 * you must call home_cfg_init first
 *
 * Returns -1 if called when get error ; otherwise, return 1
 */
lb_int32 home_cfg_exit(void)
{
	lb_int32 ret = -1;

	if (home_cfg_jobj.cfg_root)
		ret = lb_exit_cfg_file(home_cfg_jobj.cfg_root);

	return ret;
}

/**
 * get_bat_status - get battery status
 *
 * This function use to get battery status
 *
 * Returns  battery status
 */
battery_e get_bat_status(void)
{

	if (get_bat_level() <= 0)
		home_status.bat_s = BATTERY_CHARGING;
	else if (get_bat_level() <= 1)
		home_status.bat_s = BATTERY_LOW;
	else if (get_bat_level() <= 2)
		home_status.bat_s = BATTERY_MID;
	else
		home_status.bat_s = BATTERY_FULL;

	return	home_status.bat_s;
}

/**
 * set_bat_status - set battery status
 * @value: battery status
 *
 * This function use to set battery status
 */
void set_bat_status(lb_int32 value)
{
	home_status.bat_s = value;
}

/**
 * get_tf_status - get TFcard status
 *
 * This function use to get TFcard status
 *
 * Returns  sd status
 */
lb_int32 get_tf_status(void)
{

	if (home_status.tf_s < SD_PLUGOUT || home_status.tf_s >= SD_STATUS_MAX) {
		APP_LOG_E("get_tf_status  error\n");
		home_status.tf_s = SD_PLUGOUT;
	}

	return home_status.tf_s;
}

/**
 * set_tf_status - set TFcard status
 * @value:TFcard status
 *
 * This function use to set TFcard status,
 */
void set_tf_status(lb_int32 value)
{
	if (value < SD_PLUGOUT || value >= SD_STATUS_MAX) {
		APP_LOG_E("set_tf_status error\n");
		return;
	}
	home_status.tf_s = value;
}

lb_int32 get_av_status(void)
{

	return home_status.av_s;
}

/**
 * set_av_status - set av status
 * @value:0: av plugout ; 1:av plugin
 *
 * This function use to set set av status,
 */
void set_av_status(lb_int32 value)
{

	home_status.av_s = value;
}

lb_int32 get_back_status(void)
{

	return home_status.back_s;
}

/**
 * set_back_status - set back status
 * @value:0: back disable ; 1: av enable
 *
 * This function use to set back status,
 */
void set_back_status(lb_int32 value)
{

	home_status.back_s = value;
}

lb_int32 get_acc_status(void)
{

	return home_status.acc_s;
}

/**
 * set_acc_status - set acc status
 * @value:0: engine stalling ; 1: engine working
 *
 * This function use to set acc status,
 */
void set_acc_status(lb_int32 value)
{

	home_status.acc_s = value;
}


/**
 * get_cfg_volume - get config volume of device
 *
 * This function use to get config volume of device,
 * Returns config volume
 */
volume_e get_cfg_volume(void)
{
	if (home_cfg_jobj.volume) {
		if (home_cfg_jobj.volume->valueint < VOLUME_MAX
			&& home_cfg_jobj.volume->valueint >= VOLUME_OFF)
			return home_cfg_jobj.volume->valueint;
	}

	return VOLUME_MID1;

}

/**
 * set_volume - set device volume
 * @value:volume value,rang 0-100
 *
 * This function use to set device volume
 */
void set_volume(u32 value)
{
	pcm_set_volume(value, PCM_OUT);
}

/**
 * get_cfg_lcd_standby_time - get lcd standby time of config
 *
 * This function use to get lcd standby time of config
 *
 * Returns lcd standby time
 */
lb_int32 get_cfg_lcd_standby_time(void)
{
	if (home_cfg_jobj.lcd_standby_time) {
		if (home_cfg_jobj.lcd_standby_time->valueint < STANDBY_MAX
			&& home_cfg_jobj.lcd_standby_time->valueint >= STANDBY_OFF)
			return home_cfg_jobj.lcd_standby_time->valueint;
	}

	return STANDBY_30;
}

/**
 * get_cfg_keytone_enable - get enable status of keytone
 *
 * This function use to  get enable status of keytone
 * Returns enable status
 */
lb_int32 get_cfg_keytone_enable(void)
{
	if (home_cfg_jobj.keytone_enable) {
		if (home_cfg_jobj.keytone_enable->valueint < ENABLE_MAX
			&& home_cfg_jobj.keytone_enable->valueint >= ENABLE_OFF)
			return home_cfg_jobj.keytone_enable->valueint;
	}

	return ENABLE_ON;
}

/**
 * get_cfg_fastboot_enable - get enable status of fastboot
 *
 * This function use to  get enable status of fastboot
 * Returns enable status
 */
lb_int32 get_cfg_fastboot_enable(void)
{
	if (home_cfg_jobj.fastboot_enable) {
		if (home_cfg_jobj.fastboot_enable->valueint < ENABLE_MAX
			&& home_cfg_jobj.fastboot_enable->valueint >= ENABLE_OFF)
			return home_cfg_jobj.fastboot_enable->valueint;
	}

	return ENABLE_ON;
}

/**
 * get_cfg_language - get config language
 *
 * This function use to get config language style,such as english/chine simple so on.
 *
 * Returns config language style
 */
lb_int32 get_cfg_language(void)
{
	if (home_cfg_jobj.language)
		return home_cfg_jobj.language->valueint;

	return CHINESE_S;
}

/**
 * get_cfg_interval_record_enable - get interval vedio record enable
 *
 * This function use to get interval vedio record enable, use for park monitor.
 *
 * Returns ENABLE_OFF or ENABLE_ON
 */

lb_int32 get_cfg_interval_record_enable(void)
{
	if (home_cfg_jobj.interval_record) {
		if (home_cfg_jobj.interval_record->valueint < ENABLE_MAX
			&& home_cfg_jobj.interval_record->valueint >= ENABLE_OFF)
			return home_cfg_jobj.interval_record->valueint;
	}

	return ENABLE_OFF;
}

/**
 * get_cfg_version - get cfg version
 *
 * This function use to get cfg version
 *
 * Returns cfg version
 */
char *get_cfg_version(void)
{
	return home_cfg_jobj.version_cfg->valuestring;
}

/**
 * set_cfg_version - set cfg version
 * @value: cfg version
 *
 * This function use to set cfg version
 */
void set_cfg_version(char *value)
{

	memcpy(home_cfg_jobj.version_cfg->valuestring, value, strlen(value));

	home_cfg_save();
}

/**
 * switch_backlight_status - open or close backlight
 *
 * This function use to switch lcd open or close
 */
void switch_backlight_status(void)
{
	static u8 on_flag;

	APP_LOG_D("switch_backlight_status: sta[%d]\n", on_flag);
	if (!disp_device) {
		disp_device = rt_device_find(DISP_DEVICE_NAME);
		if (disp_device != NULL)
			rt_device_open(disp_device, 0);
	}

	if (on_flag)
		rt_device_control(disp_device, DISP_CMD_SET_BACKLIGHT_ON, NULL);
	else
		rt_device_control(disp_device, DISP_CMD_SET_BACKLIGHT_OFF, NULL);

	on_flag = !on_flag;
}
void set_backlight_status(enable_e status)
{

	APP_LOG_D("set_backlight_status: sta[%d]\n", status);
	if (!disp_device) {
		disp_device = rt_device_find(DISP_DEVICE_NAME);
		if (disp_device != NULL)
			rt_device_open(disp_device, 0);
		else
			return;
	}

	if (status)
		rt_device_control(disp_device, DISP_CMD_SET_BACKLIGHT_ON, NULL);
	else
		rt_device_control(disp_device, DISP_CMD_SET_BACKLIGHT_OFF, NULL);

	return;
}

lb_int32 get_cfg_lcd_brightness(void)
{
	return home_cfg_jobj.lcd_brightness->valueint;

}

/**
 * set_lcd_brightness - set lcd brightness
 * @value:brightness value,rang 0-100
 *
 * This function use to set lcd brightness
 */
void set_lcd_brightness(u32 value)
{
	int ret;
	rt_device_t disp_dev;
	disp_io_ctrl_t dic;

	APP_LOG_D("set_lcd_brightness: value[%d]\n", value);
	if (value < 0 || value > 100) {
		APP_LOG_E("err  value rang %d\n", value);
		return;
	}
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
	dic.args = &value;
	ret = rt_device_control(disp_dev, DISP_CMD_SET_BACKLIGHT_VALUE, &dic);
	if (ret)
		APP_LOG_E("DISP_CMD_SET_BACKLIGHT_VALUE error ret %d\n", ret);
	/* close audio device */
	rt_device_close(disp_dev);
}

void record_obj_init(char *video_source, record_obj_t *record_obj)
{
	int ret = -1;
	rec_param_t rec_para;
	void *recorder_hd = NULL;
	rec_time_lag_para_t rec_time_lag;
	int mode = RECORDER_TYPE_NORMAL;

	recorder_hd = lb_recorder_creat();
	if (NULL == recorder_hd) {
		APP_LOG_E("creat %s recorder handle failed!", video_source);
		return;
	}
	if(!strcmp(video_source, "isp") ||
			!strcmp(video_source, "isp_cap.0")) {
		ret = lb_recorder_ctrl(recorder_hd, LB_REC_SET_VIDEO_SOURCE,
				(void *)"vic");
		if (ret < 0) {
			APP_LOG_E("LB_REC_SET_VIDEO_SOURCE failed!");
			goto exit;
		}
	} else {
		ret = lb_recorder_ctrl(recorder_hd, LB_REC_SET_VIDEO_SOURCE,
				(void *)video_source);
		if (ret < 0) {
			APP_LOG_E("LB_REC_SET_VIDEO_SOURCE failed!");
			goto exit;
		}
	}
	
	rec_time_lag.interval = 0;
	rec_time_lag.play_framerate = 30000;
	lb_recorder_ctrl(recorder_hd, LB_REC_SET_MODE,	&mode);
	lb_recorder_ctrl(recorder_hd, LB_REC_SET_TIME_LAG_PARA, &rec_time_lag);

	if (!strcmp(video_source, "isp")) {

		memset(&rec_para, 0, sizeof(rec_param_t));
		ret = lb_recorder_ctrl(recorder_hd, LB_REC_GET_PARA, &rec_para);
		if (ret < 0) {
			APP_LOG_E("LB_REC_GET_PARA failed!");
			goto exit;
		}
		rec_para.source_width = /*FRONT_RECORDER_SOURCE_WIDTH*/1920;
		rec_para.source_height = /*FRONT_RECORDER_SOURCE_HEIGHT*/2160;
		rec_para.width = 1920;
		rec_para.height = 1080;

		rec_para.bitrate = 16000000;
		rec_para.enc_rect.x = 0;
		rec_para.enc_rect.y = 0;
		rec_para.enc_rect.width = 1920;
		rec_para.enc_rect.height = 1080;

		rec_para.audio_sample_rate = 16000;
#ifdef MEDIA_TYPE_TS
		rec_para.file_fmt = REC_OUTPUT_FORMAT_TS;
#else
		rec_para.file_fmt = REC_OUTPUT_FORMAT_MP4;
#endif
		rec_para.frame_rate = FRONT_RECORDER_SOURCE_FPS * 1000;
        rec_para.muxer_cache_flag =RECORDER_CACHE_USE;
		ret = lb_recorder_ctrl(recorder_hd, LB_REC_SET_PARA, &rec_para);
		if (ret < 0) {
			APP_LOG_E("LB_REC_SET_PARA failed!\n");
			goto exit;
		}
		lb_recorder_ctrl(recorder_hd, LB_REC_SET_ROTATE,
			(void *)VIDEO_ROTATE_270);
		ret = lb_recorder_ctrl(recorder_hd, LB_REC_PREPARE, 0);
		if (ret < 0) {
			APP_LOG_E("LB_REC_PREPARE failed!\n");
			goto exit;
		}
		record_obj->record_fhd = recorder_hd;

	} else if (!strcmp(video_source, "vic")) {
		memset(&rec_para, 0, sizeof(rec_param_t));
		ret = lb_recorder_ctrl(recorder_hd, LB_REC_GET_PARA, &rec_para);
		if (ret < 0) {
			APP_LOG_E("LB_REC_GET_PARA failed!\n");
			goto exit;
		}
		rec_para.source_width = 1920;
		rec_para.source_height = 2160;
		rec_para.width = REAR_RECORDER_SOURCE_WIDTH;
		rec_para.height = REAR_RECORDER_SOURCE_HEIGHT;
		rec_para.bitrate = 16000000;
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
		rec_para.muxer_cache_flag =RECORDER_CACHE_USE;

		ret = lb_recorder_ctrl(recorder_hd, LB_REC_SET_PARA, &rec_para);
		if (ret < 0) {
			APP_LOG_E("LB_REC_SET_PARA failed!\n");
			goto exit;
		}
		lb_recorder_ctrl(recorder_hd, LB_REC_SET_ROTATE,
			(void *)VIDEO_ROTATE_270);

		ret = lb_recorder_ctrl(recorder_hd, LB_REC_PREPARE, 0);
		if (ret < 0) {
			APP_LOG_E("LB_REC_PREPARE failed!\n");
			goto exit;
		}
		record_obj->record_rhd = recorder_hd;
	}

	return;

exit:
	lb_recorder_release(recorder_hd);
}
void record_obj_exit(record_obj_t *record_obj)
{
	if (record_obj) {
		if (record_obj->record_fhd)
			lb_recorder_release(record_obj->record_fhd);
		if (record_obj->record_rhd)
			lb_recorder_release(record_obj->record_rhd);
	}
}
record_obj_t get_record_obj(void)
{
	APP_LOG_D("front handle: %p\n", record_obj.record_fhd);
	APP_LOG_D("rear handle: %p\n", record_obj.record_rhd);
	return record_obj;
}


void disp_close_device(void)
{
	if (disp_device != NULL)
		rt_device_close(disp_device);

	return;
}
#ifdef __EOS__RELEASE__MP__
int wdog_open(void)
{
	int ret = 0;
	rt_uint16_t oflag = RT_DEVICE_OFLAG_RDWR;
	int period = 5000; /* time out period: ms */

	/* Find the device */
	wdog_dev = rt_device_find(WDOG_DEV_NAME);
	if (!wdog_dev) {
		APP_LOG_E("%s not found", WDOG_DEV_NAME);
		return -1;
	}

	/* Open the device */
	ret = rt_device_open(wdog_dev, oflag);
	if (ret) {
		APP_LOG_E("Failed to open dev:%s with flag:%d", WDOG_DEV_NAME, oflag);
		return ret;
	}

	/* set wdog time out period*/
	rt_device_control(wdog_dev, RT_DEVICE_CTRL_WDT_SET_TIMEOUT, &period);

	return 0;
}

int wdog_start(void)
{
	int ret = -1;

	if (wdog_dev) {
		rt_device_control(wdog_dev, RT_DEVICE_CTRL_WDT_START, NULL);
		ret = 0;
	}

	return ret;
}

int wdog_keepalive(void)
{
	int ret = -1;

	if (wdog_dev) {
		rt_device_control(wdog_dev, RT_DEVICE_CTRL_WDT_KEEPALIVE, NULL);
		ret = 0;
	}

	return ret;
}

int wdog_close(void)
{
	int ret = -1;

	if (wdog_dev) {
		ret = rt_device_close(wdog_dev);
		if (ret != RT_EOK) {
			APP_LOG_E("Failed to close dev:%s", WDOG_DEV_NAME);
			return ret;
		}
	}

	return ret;
}
#endif
#ifndef ARCH_LOMBO_N7_CDR_MMC
static lb_int32 format_prepare(void)
{
	lb_int32 ret = 0;

	ret = sdcard_umount();

	lb_system_mq_send(LB_SYSMSG_FS_PART_UNMOUNT,
		"/mnt/sdcard",
		strlen("/mnt/sdcard"),
		ASYNC_FLAG);

	APP_LOG_D("ret:%d\n", ret);

	return ret;
}

static lb_int32 format_finalize(void)
{
	lb_int32 ret = 0;

	ret = sdcard_mount();

	if (ret != 0)
		lb_system_mq_send(LB_SYSMSG_FS_PART_MOUNT_FAIL,
			"/mnt/sdcard",
			strlen("/mnt/sdcard"),
			ASYNC_FLAG);
	else
		lb_system_mq_send(LB_SYSMSG_FS_PART_MOUNT_OK,
			"/mnt/sdcard",
			strlen("/mnt/sdcard"),
			ASYNC_FLAG);

	APP_LOG_D("ret:%d\n", ret);

	return ret;
}

static lb_int32 format_get_stat(void)
{
	app_t *app_hd = NULL;
	lb_int32 ret = 0;
	lb_int32 stat = -1;

	app_hd = lb_app_check("home");
	if (app_hd)
		lb_app_ctrl(app_hd, GET_SD_STATUS, 0, (void *)&stat);

	if (stat == SDCARD_PLUGIN || stat == SDCARD_NOT_FORMAT)
		ret = 0;
	else
		ret = -1;

	return ret;
}

static void *format_ref_proc(void *parameter)
{
	lb_int32 ret = 0;
	lb_int32 data = 17;
	void *bar = NULL;
	static lb_int32 val;

	APP_LOG_D("Enter format\n");

	lb_ui_send_msg(LB_MSG_ENTER_ISOLATE, (void *)&data, sizeof(void *), 0);
	rt_thread_delay(20);

	ret = lb_view_get_obj_ext_by_id(200, (void **)&bar);
	if (0 != ret) {
		APP_LOG_E("failed ret:%d\n", ret);
		goto exit;
	}

	val = 0;
	lb_gal_update_bar(bar, LB_BAR_UPD_CUR_VAL, (void *)&val);
	rt_thread_delay(20);

	while (need_exit != 1) {
		if (val < 100) {
			lb_gal_update_bar(bar, LB_BAR_UPD_CUR_VAL, (void *)&val);
			val += 1;
		}
		rt_thread_delay(50);
	}

	val = 100;
	lb_gal_update_bar(bar, LB_BAR_UPD_CUR_VAL, (void *)&val);
	rt_thread_delay(20);

exit:
	APP_LOG_D("format QUIT\n");
	lb_ui_send_msg(LB_MSG_RETURN_PARENT, (void *)&data, sizeof(void *), 0);
	rt_thread_delay(20);

	pthread_exit(0);

	return NULL;
}

static lb_int32 format_ref_init(void)
{
	pthread_attr_t tmp_attr;
	struct sched_param shed_param;
	lb_int32 ret = 0;

	if ((pthread_t)format_ref_id) {
		ret = -1;
		APP_LOG_E("failed ret:%d\n", ret);
		return -1;
	}

	need_exit = 0;

	ret = pthread_attr_init(&tmp_attr);
	if (ret != 0) {
		ret = -1;
		APP_LOG_E("failed ret:%d\n", ret);
		return -1;
	}

	ret = pthread_attr_setscope(&tmp_attr, PTHREAD_SCOPE_SYSTEM);
	if (ret != 0) {
		ret = -1;
		APP_LOG_E("failed ret:%d\n", ret);
		goto exit;
	}

	ret = pthread_attr_getschedparam(&tmp_attr, &shed_param);
	if (ret != 0) {
		ret = -1;
		APP_LOG_E("failed ret:%d\n", ret);
		goto exit;
	}

	shed_param.sched_priority = FORMAT_REF_PRIO;
	ret = pthread_attr_setschedparam(&tmp_attr, &shed_param);
	if (ret != 0) {
		ret = -1;
		APP_LOG_E("failed ret:%d\n", ret);
		goto exit;
	}

	ret = pthread_attr_setstacksize(&tmp_attr, (size_t)FORMAT_REF_SIZE);
	if (ret != 0) {
		ret = -1;
		APP_LOG_E("failed ret:%d\n", ret);
		goto exit;
	}

	ret = pthread_create(&format_ref_id, &tmp_attr, &format_ref_proc, NULL);
	if (ret != 0) {
		ret = -1;
		APP_LOG_E("failed ret:%d\n", ret);
		goto exit;
	}

	return ret;

exit:
	pthread_attr_destroy(&tmp_attr);

	return ret;
}

static lb_int32 format_set_obj_click(lb_uint8 en)
{
	app_t *app_hd = NULL;

	app_hd = lb_app_check("home");
	if (app_hd) {
		if (en == 0)
			lb_app_ctrl(app_hd, 0x400, 0, NULL);
		else
			lb_app_ctrl(app_hd, 0x400, 1, NULL);
	}

	return 0;
}

static lb_int32 format_ref_exit(void)
{
	lb_int32 ret = 0;

	if ((pthread_t)format_ref_id) {
		need_exit = 1;
		pthread_join(format_ref_id, NULL);
		format_ref_id = (pthread_t)NULL;
	}

	return ret;
}

static void *format_ope_proc(void *parameter)
{
	lb_int32 ret = 0;
	lb_int32 data = 0;

	APP_LOG_D("Enter\n");

	format_set_obj_click(0);
	rt_thread_delay(20);
	ret = format_get_stat();
	if (ret != 0) {
		APP_LOG_E("failed ret:%d\n", ret);
		ret = FMT_NO_CARD;
		goto exit;
	}

	rt_thread_delay(20);
	ret = format_ref_init();
	if (ret != 0) {
		APP_LOG_E("failed ret:%d\n", ret);
		ret = FMT_OTHER_ERR;
		goto exit;
	}

	rt_thread_delay(20);
	format_prepare();

	rt_thread_delay(20);
	ret = dfs_mkfs("elm", "sd0");
	if (ret != 0) {
		APP_LOG_E("failed ret:%d\n", ret);
		ret = FMT_CARD_FAIL;
		goto exit;
	}
	APP_LOG_D("End\n");

exit:
	if (ret == FMT_CARD_SUCC) {
		data = FORMAT_CARD_SUCCEED;
		format_ref_exit();
		format_ope_exit();

		rt_thread_delay(50);
		lb_ui_send_msg(LB_MSG_ENTER_ISOLATE,
			(void *)&data, sizeof(void *), 0);

		rt_thread_delay(50);
		format_finalize();
	} else if (ret == FMT_CARD_FAIL) {
		data = FORMAT_CARD_FAILED;
		format_ref_exit();
		format_ope_exit();

		rt_thread_delay(50);
		lb_ui_send_msg(LB_MSG_ENTER_ISOLATE,
			(void *)&data, sizeof(void *), 0);
	} else if (ret == FMT_NO_CARD) {
		data = FORMAT_NO_CARD;
		format_ope_exit();

		rt_thread_delay(50);
		lb_ui_send_msg(LB_MSG_ENTER_ISOLATE,
			(void *)&data, sizeof(void *), 0);
	} else if (ret == FMT_OTHER_ERR) {
		data = FORMAT_OTHER_ERR;
		format_ope_exit();

		rt_thread_delay(50);
		lb_ui_send_msg(LB_MSG_ENTER_ISOLATE,
			(void *)&data, sizeof(void *), 0);
	}

	format_set_obj_click(1);
	pthread_exit(0);

	return NULL;
}

static lb_int32 format_ope_init(void)
{
	pthread_attr_t tmp_attr;
	struct sched_param shed_param;
	lb_int32 ret = 0;

	if ((pthread_t)format_ope_id) {
		ret = -1;
		APP_LOG_E("failed ret:%d\n", ret);
		return -1;
	}

	ret = pthread_attr_init(&tmp_attr);
	if (ret != 0) {
		ret = -1;
		APP_LOG_E("failed ret:%d\n", ret);
		return -1;
	}

	ret = pthread_attr_setscope(&tmp_attr, PTHREAD_SCOPE_SYSTEM);
	if (ret != 0) {
		ret = -1;
		APP_LOG_E("failed ret:%d\n", ret);
		goto exit;
	}

	ret = pthread_attr_setdetachstate(&tmp_attr, PTHREAD_CREATE_DETACHED);
	if (ret != 0) {
		APP_LOG_E("failed ret:%d\n", ret);
		goto exit;
	}

	ret = pthread_attr_getschedparam(&tmp_attr, &shed_param);
	if (ret != 0) {
		ret = -1;
		APP_LOG_E("failed ret:%d\n", ret);
		goto exit;
	}

	shed_param.sched_priority = FORMAT_OPE_PRIO;
	ret = pthread_attr_setschedparam(&tmp_attr, &shed_param);
	if (ret != 0) {
		ret = -1;
		APP_LOG_E("failed ret:%d\n", ret);
		goto exit;
	}

	ret = pthread_attr_setstacksize(&tmp_attr, (size_t)FORMAT_OPE_SIZE);
	if (ret != 0) {
		ret = -1;
		APP_LOG_E("failed ret:%d\n", ret);
		goto exit;
	}

	ret = pthread_create(&format_ope_id, &tmp_attr, &format_ope_proc, NULL);
	if (ret != 0) {
		ret = -1;
		APP_LOG_E("failed ret:%d\n", ret);
		goto exit;
	}

exit:
	pthread_attr_destroy(&tmp_attr);

	return ret;
}

static lb_int32 format_ope_exit(void)
{
	lb_int32 ret = 0;

	if (format_ope_id)
		format_ope_id = (pthread_t)NULL;

	return ret;
}
#endif

/**
 * sd_card_format - format sd card
 * @param: lb_obj_t object pointer.
 *
 * This function format sd card
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 format_sdcard(void *param)
{
	lb_int32 ret = 0;
	APP_LOG_D("\n");
	home_stop_recorder();
#ifndef ARCH_LOMBO_N7_CDR_MMC
	ret = format_ope_init();
	if (ret != 0) {
		APP_LOG_E("failed ret:%d\n", ret);
		ret = -1;
		goto exit;
	}

exit:
	format_ope_exit();

	return ret;
#endif

	return ret;
}
lb_int32 format_sdcard_exit(void *param)
{
	APP_LOG_D("\n");

	return 0;
}
lb_int32 format_sdcard_init(void *param)
{
	APP_LOG_D("\n");
	lb_view_set_require_func(lb_view_get_parent(), (void *)format_sdcard);
	lb_view_set_require_param(lb_view_get_parent(), (void *)NULL);

	return 0;
}
lb_int32 lock_format_sdcard_exit(void *param)
{
	APP_LOG_D("\n");

	return 0;
}
lb_int32 lock_format_sdcard_init(void *param)
{
	APP_LOG_D("\n");
	lb_view_set_require_func(lb_view_get_parent(), (void *)format_sdcard);
	lb_view_set_require_param(lb_view_get_parent(), (void *)NULL);

	return 0;
}

