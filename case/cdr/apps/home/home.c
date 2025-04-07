/*
 * home.c - desktop app use to open or close other apps
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
#include <pthread.h>
#include "home.h"
#include "home_common.h"
#include "home_ctrl.h"
#include "lb_gal_common.h"
#include "lb_types.h"
#include "lb_common.h"
#include "os_common.h"
#include "cJSON.h"
#include "lb_ui.h"
#include "da380_gsensor.h"
#include "lb_recorder.h"
#include "case_common.h"

static pthread_t sb_refresh_task_id;
static pthread_t upgrade_id;
#ifdef UPGRAGE_FIRMWARE
int g_upgrade_flag;
#endif
record_obj_t record_obj;

static lb_int32 home_start(app_t  *ap);
static lb_int32 home_stop(app_t *ap);
static lb_int32 home_suspend(app_t *ap);
static lb_int32 home_resume(app_t *ap);
static lb_int32 home_ctrl(app_t *ap, lb_uint32 cmd, lb_int32 aux0, void *aux1);

static lb_int32 home_create(void);
static lb_int32 home_destroy(void);

/* init app plugin interface struct */
static app_if_t appx = {
	home_start,
	home_stop,
	home_suspend,
	home_resume,
	home_ctrl,
};

#ifdef UPGRAGE_FIRMWARE
static lb_int32 value_init(void)
{
	lb_int32 ret = 0;
	lb_int32 data = 28;

	APP_LOG_W("Initial the value of the bar\n");

	lb_ui_send_msg(LB_MSG_ENTER_ISOLATE, (void *)&data, sizeof(void *), 0);
	rt_thread_delay(20);

	return ret;
}

static lb_int32 value_update(lb_int32 val)
{
	void *bar_obj = NULL;
	void *name_obj = NULL;
	void *perc_obj = NULL;
	char json_str[64];
	static char name_str[32];
	static char perc_str[32];

	if (lb_view_get_obj_ext_by_id(200, (void **)&bar_obj) != 0) {
		rt_thread_delay(20);
		if (lb_view_get_obj_ext_by_id(200, (void **)&bar_obj) != 0)
			return -1;
	}

	if (lb_view_get_obj_ext_by_id(202, (void **)&name_obj) != 0) {
		rt_thread_delay(20);
		if (lb_view_get_obj_ext_by_id(202, (void **)&name_obj) != 0)
			return -1;
	}

	if (lb_view_get_obj_ext_by_id(203, (void **)&perc_obj) != 0) {
		rt_thread_delay(20);
		if (lb_view_get_obj_ext_by_id(203, (void **)&perc_obj) != 0)
			return -1;
	}

	if (val > 0 && val <= 100) {
		lb_gal_set_obj_hidden(bar_obj, 0);
		lb_gal_update_bar(bar_obj, LB_BAR_UPD_CUR_VAL, (void *)&val);

		strcpy(json_str, "STR_UPGRADE_FIRMWARE");
		if (elang_get_utf8_string_josn(json_str)) {
			strcpy(name_str, elang_get_utf8_string_josn(json_str));
			lb_gal_set_obj_hidden(name_obj, 0);
			lb_gal_update_label(name_obj, LB_LABEL_UPD_TXT, name_str);
		}

		sprintf(perc_str, "%d", val);
		strcat(perc_str, "%");

		lb_gal_set_obj_hidden(perc_obj, 0);
		lb_gal_update_label(perc_obj, LB_LABEL_UPD_TXT, perc_str);
	}

	return 0;
}

static void *upgrade_proc(void *parameter)
{
	lb_int32 ret = 0;

	rt_thread_delay(50);

   value_init();
   value_update(ret);
   
	ret = update_firmware_ext(0, NULL);

	if (ret != -1) {
		
		do {
			ret = update_firmware_ext(0, NULL);
			value_update(ret);
			/*
			if (ret == 100)
				goto exit;
			*/
		} while (ret != -1);
	}

	pthread_exit(0);

	return NULL;
}

void upgrade_init(void)
{
	pthread_create(&upgrade_id, NULL, &upgrade_proc, NULL);
}

#endif

/**
 * home_start - home app init
 * @ap: APP struct pointer.
 *
 * This function use to init a app,when lb_app_open is called,this function is called.
 *
 * Returns 0
 */
static lb_int32 home_start(app_t  *ap)
{
	home_create();

	return 0;
}

/**
 * home_suspend - home app suspend
 * @ap: APP struct pointer.
 *
 * This function use to suspend a app to standby status.when lb_app_suspend is called,
 * this function is called.
 *
 * Returns 0
 */
static lb_int32 home_suspend(app_t *ap)
{
	APP_LOG_D("\n");

	return 0;
}

/**
 * home_resume - home app resume
 * @ap: APP struct pointer.
 *
 * This function use to resume a app from standby status.when lb_app_resume is called,
 * this function is called.
 *
 * Returns 0
 */
static lb_int32 home_resume(app_t *ap)
{
	APP_LOG_D("\n");

	return 0;
}

/**
 * home_ctrl - home app command interface
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
static lb_int32 home_ctrl(app_t *ap, lb_uint32 cmd, lb_int32 aux0, void *aux1)
{
	switch (cmd) {
	case APP_ENTER_BACKGROUND:
		break;
	case APP_SET_ATTR:
		break;
	case MSG_FROM_SYS_SET:
		APP_LOG_D("Got it\n");
		break;
	case MSG_FROM_CDR_SET:
		APP_LOG_D("Got it\n");
		break;
	case GET_SD_STATUS:
		*(int *)aux1 = get_tf_status();
		break;
	case GET_AV_STATUS:
		*(int *)aux1 = get_av_status();
		break;
	case GET_BACK_STATUS:
		*(int *)aux1 = get_back_status();
		break;
	case GET_ACC_STATUS:
		*(int *)aux1 = get_acc_status();
		break;
	case MSG_FROM_LOGO_HIDDEN:
		set_backlight_status(ENABLE_ON);
		if (0 == get_cfg_fastboot_enable()) {
			home_play_boot_music();
			/* sleep is the time of show logo */
			sleep(1);
		}
		APP_LOG_E("���ؿ���logo\n");
		home_view_hidden_logo();
		home_view_show_time_and_status();
		break;	 
	case MSG_FROM_CLICK:
		if (aux0 == 0) {
			home_flag.click_en = ENABLE_OFF;
			home_view_set_home_back_click(false);
		} else {
			home_view_set_home_back_click(true);
			home_flag.click_en = ENABLE_ON;
		}
		break;
	case MSG_FROM_HIDE_HBTB:
		home_view_hide_hbtb();
		break;
	case MSG_FROM_SHOW_HBTB:
		home_view_show_hbtb();
		break;
	case MSG_GET_REC_OBJ:
		*(record_obj_t *)aux1 = get_record_obj();
		break;
	default:
		APP_LOG_E("err unsupport cmd:%d,aux0 %x\n", cmd, aux0);
		break;
	}

	return 0;
}

/**
 * home_stop - home app exit
 * @ap: APP struct pointer.
 *
 * This function use to exit a app,when lb_app_close is called,this function is called.
 * Returns 0
 */
static lb_int32 home_stop(app_t *ap)
{
	home_destroy();

	return 0;
}

/**
 * home_create - home app create
 *
 * This function use to create app view and app module init.
 * Returns 0 if called when get success ; otherwise, return other values defined bu user
 */
static lb_int32 home_create(void)
{
	lb_int32 ret = 0;

#ifdef UPGRAGE_FIRMWARE
	if (get_update_state_ext() != 0)
		g_upgrade_flag = 0;
	else
		g_upgrade_flag = 1;
#endif
#ifdef LOMBO_GSENSOR
	gsensor_close_monitor();
#endif
#if 1
	record_obj_init("vic", &record_obj);
	record_obj_init("isp", &record_obj);
#endif

	ret = home_cfg_init();
	if (ret < 0) {
		APP_LOG_E("err cfg_get_para fail\n");
		return ret;
	}
	home_reg_init_exit_funcs();
	home_reg_resp_funcs();
	home_reg_resp_sys_funcs();
	lb_set_font_lang_idx(get_cfg_language());
	ret = lb_ui_load("layout/common/root.json");
	if (ret != 0) {
		APP_LOG_E("err lb ui init failed!\n");
		return ret;
	}

	home_view_get_obj_ext();
	APP_LOG_D("in home_create\n");
	set_lcd_brightness(get_cfg_lcd_brightness());
	set_volume(get_cfg_volume());
	lb_set_tone_flag(get_cfg_keytone_enable());
	lb_gal_set_screen_standby_time(get_cfg_lcd_standby_time());
#ifdef LOMBO_GPS
	gps_open_device();
#endif
#ifdef __EOS__RELEASE__MP__
	wdog_open();
#endif
	set_acc_status(get_acc_sio_val());

#ifdef UPGRAGE_FIRMWARE
	if (g_upgrade_flag == 0)
		statusbar_init(&sb_refresh_task_id);
	else
		upgrade_init();
#else
	statusbar_init(&sb_refresh_task_id);
#endif

	rtc_store_en_flag_set(get_cfg_fastboot_enable());

	APP_LOG_D("hb_refresh_task_id:%d\n", (int)sb_refresh_task_id);
	home_flag.click_en = ENABLE_ON;

	return ret;
}

/**
 * home_destroy - home app destroy
 *
 * This function use to destroy app view and app module exit.
 *
 * Returns 0
 */
static lb_int32 home_destroy(void)
{
	lb_int32 ret = 0;

	statusbar_exit(sb_refresh_task_id);
#if 1
	record_obj_exit(&record_obj);
#endif
	record_obj.record_fhd = NULL;
	record_obj.record_rhd = NULL;
	home_unreg_resp_funcs();
	home_unreg_resp_sys_funcs();
	home_unreg_init_exit_funcs();
	
	ret = home_cfg_exit();
	if (ret < 0)
		APP_LOG_E("err home_cfg_exit fail\n");

	return 0;
}

/**
 * get_app_if -get app plugin interface
 *
 * This function use to get app plugin interface struct.
 *
 * Returns app plugin interface struct.
 */
app_if_t *get_app_if()
{
	return &appx;
}

/**
 * main -msh entrance
 *
 * This function use to exec in msh env.
 * Returns 0.
 */
lb_int32 main(lb_int32 argc, char **argv)
{
	home_create();

	return 0;
}
