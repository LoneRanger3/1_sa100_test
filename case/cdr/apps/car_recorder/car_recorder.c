/*
 * car_recorder.c - car recorder plugin implement
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
#include "car_recorder.h"
#include "car_recorder_common.h"
#include "car_recorder_ctrl.h"
#include "cJSON.h"
#include "lb_types.h"
#include "lb_common.h"
#include "lb_ui.h"
#include "car_recorder_draw.h"

void *recorder_rd; /* rear recoder handle */
void *recorder_fd; /* front recoder handle */
screen_info_t cdr_screen;
int click_time;
pthread_mutex_t cr_mutex; /* car recorder mutex */
pthread_mutex_t cr_front_mutex; /* car front recorder mutex */
pthread_mutex_t cr_rear_mutex; /* car rear recorder mutex */
pthread_mutex_t cr_adas_mutex; /* car recorder adas mutex */
pthread_mutex_t cr_bsd_mutex; /* car recorder bsd mutex */

rt_sem_t pano_sem; /* car pano status sem */
rt_sem_t cdr_status_thread_sem; /* cdr status thread sem */

static lb_view_t *car_recorder_view; /* car recorder view */
static app_t  *car_recorder; /* car recorder app plugin */
static pthread_t cdr_refresh_task_id;
#ifdef BLOCK_LINKER_DRAW
static pthread_t cdr_adas_draw_id;
#endif
static lb_int32 car_recorder_start(app_t  *ap);
static lb_int32 car_recorder_stop(app_t *ap);
static lb_int32 car_recorder_suspend(app_t *ap);
static lb_int32 car_recorder_resume(app_t *ap);
static lb_int32 car_recorder_ctrl(app_t *ap, lb_uint32 cmd, lb_int32 aux0, void *aux1);

static void car_recorder_create(void);
static lb_int32 recorder_exit(void);

/* init app plugin interface struct */
static app_if_t appx = {
	car_recorder_start,
	car_recorder_stop,
	car_recorder_suspend,
	car_recorder_resume,
	car_recorder_ctrl,
};

/**
 * car_recorder_start - car recorder start
 * @ap: APP struct pointer.
 *
 * This function use to init a app,when lb_app_open is called,this function is called
 *
 * Returns 0
 */
static lb_int32 car_recorder_start(app_t  *ap)
{
	car_recorder_create();
	car_recorder = ap;

	return 0;
}

/**
 * car_recorder_suspend - car recorder app suspend
 * @ap: APP struct pointer.
 *
 * This function use to suspend a app to standby status.when lb_app_suspend is called,
 * this function is called
 *
 * Returns 0
 */
static lb_int32 car_recorder_suspend(app_t *ap)
{
	APP_LOG_D("\n");

	return 0;
}

/**
 * car_recorder_resume - car recorder resume
 * @ap: APP struct pointer.
 *
 * This function use to resume a app from standby status.when lb_app_resume is called,
 * this function is called.
 *
 * Returns 0
 */
static lb_int32 car_recorder_resume(app_t *ap)
{
	APP_LOG_D("\n");

	return 0;
}
static int exit_flag;
static lb_int32 recorder_exit(void)
{
	lb_int32 ret = 0;
	if (exit_flag)
		return 0;
	exit_flag = 1;
	APP_LOG_D("%dms\n", rt_time_get_msec());
	ret = cdr_unreg_resp_sysmsg_func();
	if (ret < 0)
		return -1;
#ifdef LOMBO_GSENSOR
	gsensor_close_monitor();
#endif
	cdr_status_thread_exit();
	APP_LOG_D("\n");
	if (cdr_refresh_task_id) {
		pthread_join(cdr_refresh_task_id, NULL);
		cdr_refresh_task_id = NULL;
	}
	APP_LOG_D("\n");

	if (cdr_comp_open_rear.thread_id) {
		pthread_join(cdr_comp_open_rear.thread_id, NULL);
		cdr_comp_open_rear.thread_id = NULL;
	}
	if (bsd_get_enable() == ENABLE_ON) {
		if (bsd_get_status() == BSD_OPEN) {
			cdr_comp_stop_bsd.com_flag = COMP_STOP_BSD;
			cdr_comp_thread_init(&cdr_comp_stop_bsd);
#if 0
			if (cdr_comp_stop_bsd.thread_id) {
				pthread_join(cdr_comp_stop_bsd.thread_id, NULL);
				cdr_comp_stop_bsd.thread_id = NULL;
			}
#endif
		}
	}
	cdr_comp_close_rear.com_flag = COMP_STOP_REAR_RECORDER;
	cdr_comp_thread_init(&cdr_comp_close_rear);

	APP_LOG_D("\n");
	if (cdr_comp_open_front.thread_id) {
		pthread_join(cdr_comp_open_front.thread_id, NULL);
		cdr_comp_open_front.thread_id = NULL;
	}
	APP_LOG_D("\n");
#if 0
	if (cdr_comp_close_rear.thread_id) {
		pthread_join(cdr_comp_close_rear.thread_id, NULL);
		cdr_comp_close_rear.thread_id = NULL;
	}
#endif
	APP_LOG_D("\n");
	if (adas_get_enable() == ENABLE_ON || bsd_get_enable() == ENABLE_ON) {
		if (adas_get_status() == ADAS_OPEN) {
			cdr_comp_stop_adas.com_flag = COMP_STOP_ADAS;
			cdr_comp_thread_init(&cdr_comp_stop_adas);
#if 0
			if (cdr_comp_stop_adas.thread_id) {
				pthread_join(cdr_comp_stop_adas.thread_id, NULL);
				cdr_comp_stop_adas.thread_id = NULL;
			}
#endif
		}
#if 0
		#ifdef BLOCK_LINKER_DRAW
			cdr_draw_exit(cdr_adas_draw_id);
		#endif
#endif
	}
	cdr_comp_close_front.com_flag = COMP_STOP_FRONT_RECORDER;
	cdr_comp_thread_init(&cdr_comp_close_front);
#if 1
	if (cdr_comp_stop_bsd.thread_id) {
		pthread_join(cdr_comp_stop_bsd.thread_id, NULL);
		cdr_comp_stop_bsd.thread_id = NULL;
	}
	if (adas_get_enable() == ENABLE_ON || bsd_get_enable() == ENABLE_ON) {
#ifdef BLOCK_LINKER_DRAW
		cdr_draw_exit(cdr_adas_draw_id);
#endif
	}
#endif
	if (cdr_comp_close_front.thread_id) {
		pthread_join(cdr_comp_close_front.thread_id, NULL);
		cdr_comp_close_front.thread_id = NULL;
	}
#if 1
	if (cdr_comp_stop_adas.thread_id) {
		pthread_join(cdr_comp_stop_adas.thread_id, NULL);
		cdr_comp_stop_adas.thread_id = NULL;
	}


	if (cdr_comp_close_rear.thread_id) {
		pthread_join(cdr_comp_close_rear.thread_id, NULL);
		cdr_comp_close_rear.thread_id = NULL;
	}
#endif
	cdr_unreg_resp_msg_funcs();
	lb_ui_static_exit(&car_recorder_view);
	cdr_unreg_init_exit_funcs();
	APP_LOG_D("\n");
	if (get_watermark_time_enable() == ENABLE_ON ||
		get_watermark_logo_enable() == ENABLE_ON)
		watermark_exit_set_source(WATERMARK_SOURCE_NUM);
	car_comp_stop_preview();
	if (car_file_mgr != NULL)
		file_mgr_destory(car_file_mgr);

	ret = cr_cfg_exit();
	if (ret < 0)
		APP_LOG_E("err cr_cfg_exit fail\n");

	pthread_mutex_destroy(&cr_mutex);
	pthread_mutex_destroy(&cr_front_mutex);
	pthread_mutex_destroy(&cr_rear_mutex);
	pthread_mutex_destroy(&cr_adas_mutex);
	pthread_mutex_destroy(&cr_bsd_mutex);

	RT_ASSERT(pano_sem);
	rt_sem_delete(pano_sem);
	RT_ASSERT(cdr_status_thread_sem);
	rt_sem_delete(cdr_status_thread_sem);
	APP_LOG_D("%dms\n", rt_time_get_msec());

	return 0;
}


/**
 * car_recorder_ctrl - car recorder app command interface
 * @ap:   APP struct pointer.
 * @cmd:  command,defined in app.
 * @aux0: auxiliary parameter 0.
 * @aux1: auxiliary parameter 1.
 *
 * This function use run the ctrl function of app.when lb_app_ctrl is called,
 * this function is called.
 *
 * Returns 0
 */
static lb_int32 car_recorder_ctrl(app_t *ap, lb_uint32 cmd, lb_int32 aux0, void *aux1)
{
	switch (cmd) {
	case APP_ENTER_BACKGROUND:
		break;
	case APP_SET_ATTR:
		break;
	case CR_GET_RECORDER_STATUS:
		if (recorder_fd)
			*(int *)aux1 = get_recorder_status(recorder_fd);
		else
			*(int *)aux1 = -1;
		break;
	case CDR_FS_PART_UNMOUNT_PREPARE:
		APP_LOG_D("get_recorder_status(recorder_fd) %d\n",
			get_recorder_status(recorder_fd));
#ifdef OPEN_REAR_RECORDER
		pthread_mutex_lock(&cr_rear_mutex);
		if (get_recorder_status(recorder_rd) == RECORDER_STATUS_RECORD)
			recoder_stop(recorder_rd);
		pthread_mutex_unlock(&cr_rear_mutex);
#endif
#ifdef OPEN_FRONT_RECORDER
		pthread_mutex_lock(&cr_front_mutex);
		if (get_recorder_status(recorder_fd) == RECORDER_STATUS_RECORD)
			recoder_stop(recorder_fd);
		pthread_mutex_unlock(&cr_front_mutex);
#endif
		APP_LOG_D("\n");
		break;
	case RECORDER_EXIT: {
		static lb_int32 type;
		void *temp;
		lb_int32 ret = 0;

		type = LB_MSG_CAR_RECORDER_BASE;
		temp = &type;

		ret = recorder_exit();
		if (ret < 0)
			return -1;
		if (aux0 == 0xfe02)
			car_comp_release();
		lb_ui_send_msg(LB_MSG_HOME_BASE, &temp, sizeof(void *), 0);
	}
		break;
	case CDR_MUTEX:
		pthread_mutex_lock(&cr_mutex);
		APP_LOG_D("\n");
		break;
	case CDR_UNMUTEX:
		pthread_mutex_unlock(&cr_mutex);
		APP_LOG_D("\n");
		break;
	default:
		APP_LOG_W("err unsupport cmd:%d,aux0 %x\n", cmd, aux0);
		break;
	}

	return 0;
}

/**
 * car_recorder_stop - car recorder app exit
 * @ap: APP struct pointer.
 *
 * This function use to exit a app,when lb_app_close is called,this function is called.
 *
 * Returns 0
 */
static lb_int32 car_recorder_stop(app_t *ap)
{
	return 0;
}

/**
 * car_recorder_create - car recorder app create
 *
 * This function use to create app view and app module init
 *
 * Returns 0 if called when get success ; otherwise, return other values defined bu user
 */
static void car_recorder_create(void)
{
	lb_int32 ret = 0;
	exit_flag = 0;
	pthread_mutex_init(&cr_mutex, NULL);
	pthread_mutex_init(&cr_front_mutex, NULL);
	pthread_mutex_init(&cr_rear_mutex, NULL);
	pthread_mutex_init(&cr_adas_mutex, NULL);
	pthread_mutex_init(&cr_bsd_mutex, NULL);
	get_screen_info(&cdr_screen);
	if (!cdr_screen.width || !cdr_screen.height) {
		APP_LOG_E("get_screen_info error\n");
		return;
	}
	APP_LOG_W("cdr_screen.width: %d %d\n", cdr_screen.width, cdr_screen.height);
	pano_sem = rt_sem_create("pano_sem", 1, RT_IPC_FLAG_FIFO);
	RT_ASSERT(pano_sem);
	cdr_status_thread_sem = rt_sem_create("status_th_quit_sem", 0, RT_IPC_FLAG_FIFO);
	RT_ASSERT(cdr_status_thread_sem);
	ret = cr_cfg_init();
	if (ret < 0) {
		APP_LOG_E("err cr_cfg_get_para fail\n");
		return;
	}
	if (car_file_mgr == NULL) {
		if (get_sd_status() == SDCARD_PLUGIN)
			cr_dir_init();
		if (REAR_RECORDER_SOURCE_HEIGHT > 720)
			car_file_mgr = file_mgr_create(HIGH_REC_BITRATE, HIGH_REC_BITRATE,
			get_recorder_duration());
		else
			car_file_mgr = file_mgr_create(HIGH_REC_BITRATE, LOW_REC_BITRATE,
			get_recorder_duration());
	}
	if (car_file_mgr)
		file_mgr_wakeup(car_file_mgr, 0);
#ifdef DEFAULT_FRONT_VIEW
	set_views_status(FRONT_VIEW);
#else
	if (get_av_status() != 0)
		set_views_status(BACK_VIEW);
	else
		set_views_status(FRONT_VIEW);
#endif
	cdr_reg_init_exit_funcs();
	cdr_reg_resp_msg_funcs();
	cdr_reg_resp_sysmsg_funcs();

	ret = lb_style_init("layout/car_recorder/styles.json");
	ret |= lb_ui_static_init("layout/car_recorder/car_recorder.json",
			&car_recorder_view);
#ifdef BLOCK_LINKER_DRAW
	if (adas_get_enable() == ENABLE_ON || bsd_get_enable() == ENABLE_ON)
		cdr_draw_init(&cdr_adas_draw_id);
	if (bsd_get_enable() == ENABLE_ON) {
		ret = lb_ui_static_add("layout/car_recorder/car_ai.json",
				&car_recorder_view);
		if (ret != 0)
			APP_LOG_E("err lb ui init failed ret:%d!\n", ret);
	}
#else
	if (adas_get_enable() == ENABLE_ON || bsd_get_enable() == ENABLE_ON) {
		ret = lb_ui_static_add("layout/car_recorder/car_ai.json",
				&car_recorder_view);
		if (ret != 0)
			APP_LOG_E("err lb ui init failed ret:%d!\n", ret);
	}
#endif
	APP_LOG_D("get_sd_status:%d  %d %dms\n", get_sd_status(),
		get_av_status(), rt_time_get_msec());
	cdr_comp_open_front.com_flag = COMP_START_FRONT_RECORDER;
	cdr_comp_thread_init(&cdr_comp_open_front);
	APP_LOG_D("%dms\n", rt_time_get_msec());
	cdr_comp_open_rear.com_flag = COMP_START_REAR_RECORDER;
	cdr_comp_thread_init(&cdr_comp_open_rear);
	cdr_status_thread_init(&cdr_refresh_task_id);
#ifdef LOMBO_GSENSOR
	if (get_park_monitor_enable() == ENABLE_ON)
		gsensor_set_park_monitor_cfg(RT_TRUE);
	else
		gsensor_set_park_monitor_cfg(RT_FALSE);

	APP_LOG_D("gsensor_get_park_monitor_cfg:%d sensity:%d\n",
		gsensor_get_park_monitor_cfg(), get_gsensor_sensity());
	if (get_gsensor_sensity_map()) {
		gsensor_set_measure_range(get_gsensor_sensity_map());
		gsensor_open_monitor();
	}
#endif
	if (get_sd_status() == SDCARD_NOT_PLUGIN) {
		dialog_flag = DIALOG_NEED_PLUGIN_SDCARD;
		lb_system_mq_send(LB_SYSMSG_DIALOG, &dialog_flag, sizeof(int), 0);
	} else if (get_sd_status() == SDCARD_NOT_FORMAT) {
		dialog_flag = DIALOG_PLEASE_FORMAT_SDCARD;
		lb_system_mq_send(LB_SYSMSG_DIALOG, &dialog_flag, sizeof(int), 0);
	} else if (get_cfg_park_monitor_times()) {
		 park_monitor_times_clear();
		dialog_flag = DIALOG_PARK_MONITOR_EVENT;
		lb_system_mq_send(LB_SYSMSG_DIALOG, &dialog_flag, sizeof(int), 0);
	}
	APP_LOG_D("get_cfg_park_monitor_times:%d\n", get_cfg_park_monitor_times());

	if (get_back_status() != 0)
		lb_system_mq_send(LB_SYSMSG_BACK_ON, NULL, 0, 0);
	click_time = rt_time_get_msec();
	return;
}

/**
 * get_app_if - get app plugin interface
 *
 * This function use to get app plugin interface struct
 *
 * Returns app plugin interface struct.
 */
app_if_t *get_app_if()
{
	return &appx;
}

/**
 * main
 *
 * This function use to exec in msh env
 *
 * Returns 0.
 */
lb_int32 main(lb_int32 argc, char **argv)
{
	car_recorder_create();

	return 0;
}
