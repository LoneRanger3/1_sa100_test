/*
 * home_ctrl.c - home widget reg & unreg implement
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
#include <time.h>
#include <rtthread.h>
#include "eos.h"
#include "board.h"
#include "cJSON.h"
#include "app_manage.h"
#include "mod_manage.h"
#include "system/system_mq.h"
#include "home.h"
#include "home_common.h"
#include "home_ctrl.h"
#include "lb_common.h"
#include "lb_gal_common.h"
#include "lb_ui.h"
#include "../car_recorder/car_recorder.h"
#include "../system_setting/system_setting.h"
#include "../cdr_setting/cdr_setting.h"
#include "../file_explorer/file_explorer.h"
#include "../smart_drive/smart_drive.h"

static apps_t apps; /* app set */
static sb_widget_t sb_widget; /* statusbar widget object */
static sb_property_t sb_property; /* statusbar widget property */
static sb_last_s_t sb_last_s; /* statusbar last status */
static cmdbar_widget_t cmdbar_widget;
static cmdbar_property_t cmdbar_property;
static int return_home = 1;
static weekday_t weekday;
static fs_pro_t fs_pro;
home_flag_t home_flag;
static cdr_interval_t cdr_interval;
lb_uint8 left_right_hide_flag=1;
#define POWER_VALUE_CHECK 1//检测电压高低关机
#define DELAY_POWEROFF 1//要不要延时关机


const char str_weekday[7][32] = {
	"STR_SUNDAY",
	"STR_MONDAY",
	"STR_TUESDAY",
	"STR_WEDNESDAY",
	"STR_THURSDAY",
	"STR_FRIDAY",
	"STR_SATURDAY"
};
static lb_int32 home_view_hidden();

static void *statusbar_refresh(void *parameter);

lb_int32 home_view_get_obj_ext(void)
{
	lb_view_get_obj_ext_by_id(LOGO_ID, &sb_widget.logo);
	lb_view_get_obj_ext_by_id(WEEK_ID, &sb_widget.week);
	lb_view_get_obj_ext_by_id(YTD_ID, &sb_widget.ytd);
	lb_view_get_obj_ext_by_id(TIME_ID, &sb_widget.time);
	lb_view_get_obj_ext_by_id(HOME_ID, &sb_widget.home);
	lb_view_get_obj_ext_by_id(BACK_ID, &sb_widget.back);
	lb_view_get_obj_ext_by_id(HOME_LOGO_ID, &sb_widget.home_logo);


	lb_view_get_obj_ext_by_id(SYSTEM_SETTING_ID, &cmdbar_widget.system_setting);
	lb_view_get_obj_ext_by_id(SMART_DRIVE_ID, &cmdbar_widget.smart_drive);
	lb_view_get_obj_ext_by_id(FILE_EXPLORER_ID, &cmdbar_widget.file_explorer);
	lb_view_get_obj_ext_by_id(CDR_ID, &cmdbar_widget.cdr);
	lb_view_get_obj_ext_by_id(CDR_SETTING_ID, &cmdbar_widget.cdr_setting);

	lb_view_get_obj_property_by_id(LOGO_ID, (void **)(&sb_property.logo));
	
	lb_view_get_obj_ext_by_id(LEFT_LAY_ID, &sb_widget.left_lay);
    lb_view_get_obj_ext_by_id(RIGHT_LAY_ID, &sb_widget.right_lay);

	return 0;
}

lb_int32 home_play_boot_music(void)
{
	//lb_play_warningtone(ROOTFS_MOUNT_PATH"/res/boot_music.wav");

	return 0;
}
static int lcd_off_cnt=0;
lb_int32 home_view_hidden_logo(void)
{
	lb_gal_update_img(sb_property.logo, sb_widget.logo, LB_IMG_UPD_SRC, 0xFF, NULL);
	home_flag.lcd_brightness = ENABLE_ON;
	if (lb_gal_get_screen_standby_status() == 1 && !g_upgrade_flag){
	lb_gal_screen_standby_switch();
	lcd_off_cnt=4;
	}
	return 0;
}
lb_int32 left_right_lay_control(lb_int8 tmp)
{
        left_right_hide_flag=tmp;
		lb_gal_set_obj_hidden(sb_widget.left_lay, tmp);
	    lb_gal_set_obj_hidden(sb_widget.right_lay, tmp);


		sb_last_s.last_min=-1;
		sb_last_s.last_hour=-1;
		sb_last_s.last_day=-1;
		sb_last_s.last_mon=-1;
		sb_last_s.last_year=-1;
		sb_last_s.last_week=-1;
		sb_last_s.last_tf_s = -1;
		
	return 0;
}


lb_int32 home_view_show_time_and_status(void)
{
	lb_gal_set_obj_hidden(sb_widget.week, false);
	lb_gal_set_obj_hidden(sb_widget.ytd, false);
	lb_gal_set_obj_hidden(sb_widget.time, false);
	lb_gal_set_obj_hidden(sb_widget.home, false);
	lb_gal_set_obj_hidden(sb_widget.back, false);
	lb_gal_set_obj_hidden(sb_widget.home_logo, false);

	
	/* tfcard and battery status will check all the time,
	 * so must get the ext in the last.
	 */
	lb_view_get_obj_ext_by_id(TFCARD_ID, &sb_widget.tfcard);
	lb_view_get_obj_ext_by_id(BATTERY_ID, &sb_widget.battery);


	//	lb_gal_set_obj_hidden(sb_widget.tfcard, false);
//	lb_gal_set_obj_hidden(sb_widget.battery, false);

	return 0;
}

/* hide home/back/tfcard/battery */
lb_int32 home_view_hide_hbtb(void)
{
	if (sb_widget.home)
		lb_gal_set_obj_hidden(sb_widget.home, true);

	if (sb_widget.back)
		lb_gal_set_obj_hidden(sb_widget.back, true);

	if (sb_widget.tfcard)
		lb_gal_set_obj_hidden(sb_widget.tfcard, true);

	if (sb_widget.battery)
		lb_gal_set_obj_hidden(sb_widget.battery, true);

	if (sb_widget.home_logo)
		lb_gal_set_obj_hidden(sb_widget.home_logo, true);

	return 0;
}

/* show home/back/tfcard/battery */
lb_int32 home_view_show_hbtb(void)
{
	if (sb_widget.home)
		lb_gal_set_obj_hidden(sb_widget.home, false);

	if (sb_widget.back)
		lb_gal_set_obj_hidden(sb_widget.back, false);

	if (sb_widget.tfcard)
		lb_gal_set_obj_hidden(sb_widget.tfcard, false);

	if (sb_widget.battery)
		lb_gal_set_obj_hidden(sb_widget.battery, false);

	if (sb_widget.home_logo)
		lb_gal_set_obj_hidden(sb_widget.home_logo, false);

	return 0;
}

lb_int32 home_view_hidden()
{
	lb_view_get_obj_property_by_id(SYSTEM_SETTING_ID,
		(void **)(&cmdbar_property.system_setting));
	lb_view_get_obj_property_by_id(SMART_DRIVE_ID,
		(void **)(&cmdbar_property.smart_drive));
	lb_view_get_obj_property_by_id(FILE_EXPLORER_ID,
		(void **)(&cmdbar_property.file_explorer));
	lb_view_get_obj_property_by_id(CDR_ID, (void **)(&cmdbar_property.cdr));
	lb_view_get_obj_property_by_id(CDR_SETTING_ID,
		(void **)(&cmdbar_property.cdr_setting));

	lb_gal_update_imgbtn(cmdbar_property.system_setting,
			cmdbar_widget.system_setting,
			LB_IMGBTN_UPD_SRC, 0xFF, NULL);
	lb_gal_update_imgbtn(cmdbar_property.smart_drive,
			cmdbar_widget.smart_drive,
			LB_IMGBTN_UPD_SRC, 0xFF, NULL);
	lb_gal_update_imgbtn(cmdbar_property.file_explorer,
			cmdbar_widget.file_explorer,
			LB_IMGBTN_UPD_SRC, 0xFF, NULL);
	lb_gal_update_imgbtn(cmdbar_property.cdr,
			cmdbar_widget.cdr,
			LB_IMGBTN_UPD_SRC, 0xFF, NULL);
	lb_gal_update_imgbtn(cmdbar_property.cdr_setting,
			cmdbar_widget.cdr_setting,
			LB_IMGBTN_UPD_SRC, 0xFF, NULL);

	return 0;
}
lb_int32 home_view_show()
{
	lb_view_get_obj_property_by_id(SYSTEM_SETTING_ID,
		(void **)(&cmdbar_property.system_setting));
	lb_view_get_obj_property_by_id(SMART_DRIVE_ID,
		(void **)(&cmdbar_property.smart_drive));
	lb_view_get_obj_property_by_id(FILE_EXPLORER_ID,
		(void **)(&cmdbar_property.file_explorer));
	lb_view_get_obj_property_by_id(CDR_ID, (void **)(&cmdbar_property.cdr));
	lb_view_get_obj_property_by_id(CDR_SETTING_ID,
		(void **)(&cmdbar_property.cdr_setting));

	lb_gal_update_imgbtn(cmdbar_property.system_setting,
			cmdbar_widget.system_setting,
			LB_IMGBTN_UPD_SRC, 0, NULL);
	lb_gal_update_imgbtn(cmdbar_property.smart_drive,
			cmdbar_widget.smart_drive,
			LB_IMGBTN_UPD_SRC, 0, NULL);
	lb_gal_update_imgbtn(cmdbar_property.file_explorer,
			cmdbar_widget.file_explorer,
			LB_IMGBTN_UPD_SRC, 0, NULL);
	lb_gal_update_imgbtn(cmdbar_property.cdr,
			cmdbar_widget.cdr,
			LB_IMGBTN_UPD_SRC, 0, NULL);
	lb_gal_update_imgbtn(cmdbar_property.cdr_setting,
			cmdbar_widget.cdr_setting,
			LB_IMGBTN_UPD_SRC, 0, NULL);

	return 0;
}

lb_int32 home_view_set_home_back_click(bool en)
{
	lb_gal_set_obj_click(sb_widget.home, en);
	lb_gal_set_obj_click(sb_widget.back, en);

	return 0;
}


/**
 * statusbar_init - create left & right bar refreash thread
 * @thread_id: thread id
 *
 * This function init statusbar refreash thread attribute,create bar refreash thread
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 statusbar_init(pthread_t *thread_id)
{
	pthread_attr_t                  tmp_attr;
	struct sched_param              shed_param;
	lb_int32                             ret;

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
	ret = pthread_attr_setdetachstate(&tmp_attr, PTHREAD_CREATE_DETACHED);
	if (ret != 0) {
		APP_LOG_E("\n set thread detach error: %d!\n", ret);
		goto exit;
	}
	shed_param.sched_priority = HEADBAR_THREAD_PRIORITY;
	ret = pthread_attr_setschedparam(&tmp_attr, &shed_param);
	if (ret != 0) {
		APP_LOG_E("\n set thread priority error: %d!\n", ret);
		goto exit;
	}
	ret = pthread_attr_setstacksize(&tmp_attr, (size_t)HEADBAR_THREAD_STACK_SIZE);
	if (ret != 0) {
		APP_LOG_E("\n set thread stack size error: %d!!\n", ret);
		goto exit;
	}
	ret = pthread_create(thread_id, &tmp_attr, &statusbar_refresh, NULL);
	if (ret != 0) {
		APP_LOG_E("\n create thread_func1 failed: %d!\n", ret);
		goto exit;
	}
	pthread_detach(*thread_id);
exit:
	pthread_attr_destroy(&tmp_attr);

	return 0;
}

/**
 * statusbar_exit - exit statusbar refreash thread
 * @thread_id: thread id
 *
 * This function exit statusbar refreash thread
 *
 * Returns 0
 */
lb_int32 statusbar_exit(pthread_t thread_id)
{
	home_flag.thread_exit = 1;

	return 0;
}

/**
 * statusbar_tfcard_init - tfcard img init in statusbar
 * @param: lb_obj_t object pointer.
 *
 * This function use to init tfcard img widget
 *
 * Returns 0
 */
lb_int32 statusbar_tfcard_init(void *param)
{
	return 0;
}

/**
 * statusbar_battery_init - battery img init in statusbar
 * @param: lb_obj_t object pointer.
 *
 * This function use to init battery img widget
 *
 * Returns 0
 */
lb_int32 statusbar_battery_init(void *param)
{
	return 0;
}

/**
 * system_poweroff_init - init system power off
 *
 * This function use to set system power off init
 */
lb_int32 system_poweroff_init(void *param)
{
	APP_LOG_D("\n");

	return 0;
}

/**
 * system_poweroff_exit - interface of system power off
 *
 * This function use to set system power off,if charging and not key poweroff, reboot
 */
lb_int32 system_poweroff_exit(void *param)
{
	APP_LOG_D("\n");
#ifdef USB_POWEROFF_DIALOG
	if (!get_bat_level() && !home_flag.key_poweroff) {
		lb_ui_send_msg(LB_MSG_CDR, NULL, 0, 0);
		return 0;
	}
#endif
	APP_LOG_W("\n");
	
	rt_hw_codec_speak_close();
	
#ifdef __EOS__RELEASE__MP__
	wdog_close();
#endif
	disp_close_device();
#ifdef LOMBO_GPS
	gps_close_device();
#endif
	rt_pm_shutdown();

	return 0;
}

lb_int32 file_explorer_plugout_init(void *param)
{
	APP_LOG_D("\n");

	return 0;
}

lb_int32 file_explorer_plugout_exit(void *param)
{
	APP_LOG_D("\n");

	return 0;
}


static int getWeekdayByYearday(int iY, int iM, int iD)
{
	int iWeekDay = -1;
	if (1 == iM || 2 == iM) {
		iM += 12;
		iY--;
	}
	iWeekDay = (iD + 1 + 2 * iM + 3 * (iM + 1) / 5 + iY + iY / 4 - iY / 100 +
			iY / 400) % 7;

	return iWeekDay;
}

/**
 * statusbar_week_init - statusbar week init
 * @param: lb_obj_t object pointer.
 *
 * This function use to init week label widget
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 statusbar_week_init(void *param)
{
	lb_obj_t        *p_obj = (lb_obj_t *)param;
	lb_label_t      *p_label_prop = NULL;
	struct tm *p_tm; /* time variable */
	time_t now;
	now = time(RT_NULL);
	p_tm = localtime(&now);

	if (NULL == param) {
		APP_LOG_E("err Invalid parameters!\n");
		return LB_ERROR_NO_MEM;
	}
	p_label_prop = (lb_label_t *)p_obj->property;
	weekday.wd_index = getWeekdayByYearday(p_tm->tm_year + 1900, p_tm->tm_mon + 1,
			p_tm->tm_mday);

	p_label_prop->str_id = elang_get_string_id_josn(str_weekday[weekday.wd_index]);
	sb_last_s.last_week = weekday.wd_index;

	return 0;
}

/**
 * statusbar_ytd_init - statusbar year month day init
 * @param: lb_obj_t object pointer.
 *
 * This function use to init ytd label widget
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 statusbar_ytd_init(void *param)
{
	time_t		now;
	struct tm *p_tm;
	lb_obj_t        *p_obj = (lb_obj_t *)param;
	lb_label_t      *p_label_prop = NULL;

	if (NULL == param) {
		APP_LOG_E("err Invalid parameters!\n");
		return LB_ERROR_NO_MEM;
	}
	p_label_prop = (lb_label_t *)p_obj->property;
	p_label_prop->txt = malloc(TXT_MAX);
	now = time(RT_NULL);
	p_tm = localtime(&now);

	if (p_label_prop->txt) {
		memset(p_label_prop->txt, 0x00, TXT_MAX);
		sprintf(p_label_prop->txt, "%02d-%02d-%02d", p_tm->tm_year + 1900,
			p_tm->tm_mon + 1, p_tm->tm_mday);
	}

	return 0;
}

/**
 * statusbar_time_init - statusbar time init
 * @param: lb_obj_t object pointer.
 *
 * This function use to init time label widget
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 statusbar_time_init(void *param)
{
	lb_obj_t        *p_obj = (lb_obj_t *)param;
	lb_label_t      *p_label_prop = NULL;
	time_t		now;
	struct tm *p_tm;

	now = time(RT_NULL);
	p_tm = localtime(&now);

	if (NULL == param) {
		APP_LOG_E("err Invalid parameters!\n");
		return LB_ERROR_NO_MEM;
	}
	p_label_prop = (lb_label_t *)p_obj->property;
	p_label_prop->txt = malloc(TXT_MAX);
	if (p_label_prop->txt) {
		memset(p_label_prop->txt, 0x00, TXT_MAX);
		sprintf(p_label_prop->txt, "%02d:%02d", p_tm->tm_hour,
			p_tm->tm_min);
	}

	return 0;
}
#if defined(SYNC_TIME_FROM_GPS) || defined(SET_ACC_SHUTDOWN_TIME)
static lb_uint8 get_month_day(struct tm *p_tm)
{
	if (1 == (p_tm->tm_mon)) {
		if (((p_tm->tm_year % 4 == 0) && (p_tm->tm_year % 100 != 0))
			|| ((p_tm->tm_year + 1900) % 400 == 0))
			return 29;
		else
			return 28;
	} else if (0 == p_tm->tm_mon || 2 == p_tm->tm_mon || 4 == p_tm->tm_mon
		|| 6 == p_tm->tm_mon || 7 == p_tm->tm_mon || 9 == p_tm->tm_mon
		|| 11 == p_tm->tm_mon)
		return 31;
	else
		return 30;
}
#endif
#ifdef LOMBO_GPS
#ifdef SYNC_TIME_FROM_GPS
static lb_int32 set_date_time(struct tm dt)
{
	lb_int32 ret = 0;
	struct tm *p_tm; /* time variable */
	time_t now;

	APP_LOG_D("write:year:%d, month:%d, day:%d, hour:%d, minute:%d sencond: %d\n",
		dt.tm_year+1900, dt.tm_mon+1, dt.tm_mday,
		dt.tm_hour, dt.tm_min, dt.tm_sec);

	lb_gal_set_screen_standby_enable(false);
	/* set dt to rtc */
	ret = set_date(dt.tm_year+1900, dt.tm_mon+1, dt.tm_mday);
	if (ret != 0) {
		APP_LOG_W("[%s,%d]\n", __FILE__, __LINE__);
		ret = -1;
		goto exit;
	}
	rt_thread_delay(10);
	/* set dt to rtc */
	ret = set_time(dt.tm_hour, dt.tm_min, dt.tm_sec);
	if (ret != 0) {
		APP_LOG_W("[%s,%d]\n", __FILE__, __LINE__);
		ret = -1;
		goto exit;
	}
	rt_thread_delay(10);
	lb_gal_set_screen_standby_enable(true);
	now = time(RT_NULL);
	p_tm = localtime(&now);

	APP_LOG_D("read:year:%d, month:%d, day:%d, hour:%d, minute:%d\n",
		(p_tm->tm_year + 1900), (p_tm->tm_mon + 1), p_tm->tm_mday,
		p_tm->tm_hour, p_tm->tm_min);

exit:
	return ret;
}

static lb_int32 get_gps_tm_time(struct tm *tm_time, struct gps_data_t gps_data_temp)
{
	int ret = -1;

	if (gps_data_temp.year < 19 || gps_data_temp.year > 99)
		ret = -1;
	else if (gps_data_temp.month == 0 || gps_data_temp.month > 12)
		ret = -2;
	else if (gps_data_temp.day == 0 || gps_data_temp.day > 31)
		ret = -3;
	else if (gps_data_temp.h > 23)
		ret = -4;
	else if (gps_data_temp.m > 59)
		ret = -5;
	else if (gps_data_temp.s > 59)
		ret = -6;
	else {
		tm_time->tm_sec = gps_data_temp.s;
		tm_time->tm_min = gps_data_temp.m;
		tm_time->tm_hour = gps_data_temp.h;
		tm_time->tm_mday = gps_data_temp.day;
		tm_time->tm_mon = gps_data_temp.month - 1;
		tm_time->tm_year = gps_data_temp.year + 100;
		if (lb_get_font_lang_idx() == JAPANESE)
			tm_time->tm_hour += 9;
		else if (lb_get_font_lang_idx() == RUSSIAN)
			tm_time->tm_hour += 3;
		else if (lb_get_font_lang_idx() == THAI || lb_get_font_lang_idx() ==
			VIETNAMESE)
			tm_time->tm_hour += 7;
		else if (lb_get_font_lang_idx() == CHINESE_S || lb_get_font_lang_idx() ==
			CHINESE_T)
			tm_time->tm_hour += 8;

		if (tm_time->tm_hour >= 24) {
			tm_time->tm_mday += tm_time->tm_hour / 24;
			tm_time->tm_hour = tm_time->tm_hour % 24;
			if (tm_time->tm_mday > get_month_day(tm_time)) {
				tm_time->tm_mon += tm_time->tm_mday /
					get_month_day(tm_time);
				tm_time->tm_mday = tm_time->tm_mday %
					get_month_day(tm_time);
				if (tm_time->tm_mon >= 12) {
					tm_time->tm_year += tm_time->tm_mon / 12;
					tm_time->tm_mon = tm_time->tm_mon % 12;
				}
			}
		}
		return 0;
	}

	return ret;
}
static int  get_gps_data(struct gps_data_t *g)
{
	rt_bool_t status;

	*g = gps_get_data();
	status = gps_connect_status();
	if (!status)
		return -1;
	if (g->valid)
		return 0;
	else
		return -2;
}
#endif
#endif
/**
 * statusbar_refresh - the thread refresh statusbar view,update time & tfcard & battery
 * view,if status change
 * @parameter: reserved for user custom,no use
 *
 * This function use to refresh statusbar view in thread,update time & tfcard & battery
 * widget view,if status change,when car_recorder app is opened,  update recording status.
 */
#define delay_poweroff_cnt  2*60 //2*5 //2*60
static void *statusbar_refresh(void *parameter)
{
	struct tm *p_tm; /* time variable */
	static struct tm p_tm_shutdown; /* interval record time */
	void *ret_home = NULL;
	int shutdown_count = 0; /* delay count for shutdown */
	time_t now;
	int para;
	static int delay_cnt=0;
	static int low_high_cnt=0;
	static char time_buf[TXT_MAX];
	static char ytd_buf[TXT_MAX];
#ifdef LOMBO_GPS
#ifdef SYNC_TIME_FROM_GPS
	int	ret;
	static struct tm gps_time;
	static struct gps_data_t gps_data;
#endif
#endif
	lb_view_get_obj_property_by_id(WEEK_ID, (void **)(&sb_property.week));
	lb_view_get_obj_property_by_id(YTD_ID, (void **)(&sb_property.ytd));
	lb_view_get_obj_property_by_id(TIME_ID, (void **)(&sb_property.time));
	lb_view_get_obj_property_by_id(TFCARD_ID, (void **)(&sb_property.tfcard));
	lb_view_get_obj_property_by_id(BATTERY_ID, (void **)(&sb_property.battery));
	
	sb_last_s.last_min = -1;
	sb_last_s.last_bat_s = -1;
	sb_last_s.last_tf_s = -1;
	sb_last_s.last_recording_s = -1;
	sb_last_s.last_acc_s = 1;
	cdr_interval.acc_closebk_duration_s = -1;
	cdr_interval.acc_shutdown_duration_h = -1;

	left_right_lay_control(1);



	para = LB_USERDEF_SYSMSG_ENTER_CDR;
	lb_system_mq_send(LB_SYSMSG_USERDEF, &para, sizeof(int *), 0);
#ifdef __EOS__RELEASE__MP__
	wdog_start();
#endif
	while (1) {	
		now = time(RT_NULL);
		p_tm = localtime(&now);
 if(left_right_hide_flag==0){	

		if (sb_widget.time) {
			if (p_tm->tm_min != sb_last_s.last_min ||
				p_tm->tm_hour != sb_last_s.last_hour) {
				sb_last_s.last_min = p_tm->tm_min;
				sb_last_s.last_hour = p_tm->tm_hour;
				memset(time_buf, 0, TXT_MAX);
				sprintf(time_buf, "%02d:%02d", p_tm->tm_hour,
					p_tm->tm_min);
				lb_gal_update_label(sb_widget.time, LB_LABEL_UPD_TXT,
					time_buf);

			}
			if (sb_last_s.last_year != p_tm->tm_year  ||
				sb_last_s.last_mon != p_tm->tm_mon ||
				sb_last_s.last_day != p_tm->tm_mday) {
				memset(ytd_buf, 0, TXT_MAX);

				sprintf(ytd_buf, "%02d-%02d-%02d", p_tm->tm_year +
					1900, p_tm->tm_mon + 1, p_tm->tm_mday);

				lb_gal_update_label(sb_widget.ytd,
					LB_LABEL_UPD_TXT, ytd_buf);
				sb_last_s.last_year = p_tm->tm_year;
				sb_last_s.last_mon = p_tm->tm_mon;
				sb_last_s.last_day = p_tm->tm_mday;
			}
		}
		if (sb_widget.week) {
			weekday.wd_index = getWeekdayByYearday(p_tm->tm_year + 1900,
					p_tm->tm_mon + 1, p_tm->tm_mday);
			if (weekday.wd_index != sb_last_s.last_week) {
				if (sb_property.week) {
					weekday.wd_str = elang_get_utf8_string_josn(
							str_weekday[weekday.wd_index]);
					lb_gal_update_label(sb_widget.week,
						LB_LABEL_UPD_TXT, (void *)weekday.wd_str);
					sb_property.week->str_id =
						elang_get_string_id_josn(
							str_weekday[weekday.wd_index]);
					sb_last_s.last_week = weekday.wd_index;
				}
			}
		}
		if (sb_widget.tfcard) {
			if (get_tf_status() != sb_last_s.last_tf_s) {
				sb_last_s.last_tf_s = get_tf_status();
				APP_LOG_D("get_tf_status:%d\n", get_tf_status());
				if (get_tf_status() == SD_PLUGIN)
					lb_gal_update_img(sb_property.tfcard,
						sb_widget.tfcard, LB_IMG_UPD_SRC,
						get_tf_status(), NULL);
				else
					lb_gal_update_img(sb_property.tfcard,
						sb_widget.tfcard, LB_IMG_UPD_SRC,
						get_tf_status(), NULL);
			}
		}

	/*	if (sb_widget.battery) {
			if (get_bat_status() != sb_last_s.last_bat_s) {
				sb_last_s.last_bat_s = get_bat_status();

				APP_LOG_D("bat_status:%d\n", get_bat_status());

				lb_gal_update_img(sb_property.battery,
					sb_widget.battery, LB_IMG_UPD_SRC,
					get_bat_status(), NULL);
				home_flag.delay_time = 0;
			}


		}*/
 	}else{		
////////////////////////////////////////////////////////////////////////////////////

// if (sb_widget.up_time) {
// 	if (p_tm->tm_min != sb_last_s.last_min ||
// 		p_tm->tm_hour != sb_last_s.last_hour) {
// 		sb_last_s.last_min = p_tm->tm_min;
// 		sb_last_s.last_hour = p_tm->tm_hour;
// 		memset(time_buf, 0, TXT_MAX);
// 		sprintf(time_buf, "%02d:%02d", p_tm->tm_hour,
// 			p_tm->tm_min);
// 		lb_gal_update_label(sb_widget.up_time, LB_LABEL_UPD_TXT,
// 			time_buf);

// 	}
// 	if (sb_last_s.last_year != p_tm->tm_year  ||
// 		sb_last_s.last_mon != p_tm->tm_mon ||
// 		sb_last_s.last_day != p_tm->tm_mday) {
// 		memset(ytd_buf, 0, TXT_MAX);

// 		sprintf(ytd_buf, "%02d-%02d-%02d", p_tm->tm_year +
// 			1900, p_tm->tm_mon + 1, p_tm->tm_mday);

// 		lb_gal_update_label(sb_widget.up_ytd,
// 			LB_LABEL_UPD_TXT, ytd_buf);
// 		sb_last_s.last_year = p_tm->tm_year;
// 		sb_last_s.last_mon = p_tm->tm_mon;
// 		sb_last_s.last_day = p_tm->tm_mday;
// 	}
// }
// if (sb_widget.up_week) {
// 	weekday.wd_index = getWeekdayByYearday(p_tm->tm_year + 1900,
// 			p_tm->tm_mon + 1, p_tm->tm_mday);
// 	if (weekday.wd_index != sb_last_s.last_week) {
// 		if (sb_property.up_week) {
// 			weekday.wd_str = elang_get_utf8_string_josn(
// 					str_weekday[weekday.wd_index]);
// 			lb_gal_update_label(sb_widget.up_week,
// 				LB_LABEL_UPD_TXT, (void *)weekday.wd_str);
// 			sb_property.up_week->str_id =
// 				elang_get_string_id_josn(
// 					str_weekday[weekday.wd_index]);
// 			sb_last_s.last_week = weekday.wd_index;
// 		}
// 	}
// }
// if (sb_widget.up_tfcard) {
// 	if (get_tf_status() != sb_last_s.last_tf_s) {
// 		sb_last_s.last_tf_s = get_tf_status();
// 		APP_LOG_D("get_tf_status:%d\n", get_tf_status());
// 		if (get_tf_status() == SD_PLUGIN)
// 			lb_gal_update_img(sb_property.up_tfcard,
// 				sb_widget.up_tfcard, LB_IMG_UPD_SRC,
// 				get_tf_status(), NULL);
// 		else
// 			lb_gal_update_img(sb_property.up_tfcard,
// 				sb_widget.up_tfcard, LB_IMG_UPD_SRC,
// 				get_tf_status(), NULL);
// 	}
// }

// /*if (sb_widget.up_battery) {
// 	if (get_bat_status() != sb_last_s.last_bat_s) {
// 		sb_last_s.last_bat_s = get_bat_status();

// 		APP_LOG_D("bat_status:%d\n", get_bat_status());

// 		lb_gal_update_img(sb_property.up_battery,
// 			sb_widget.up_battery, LB_IMG_UPD_SRC,
// 			get_bat_status(), NULL);
// 		home_flag.delay_time = 0;
// 	}


//    } */
 }
///////////////////////////////////////////////////////////////////////////////////////
#if 1 //不接ACC，按键开机，防止提示语显示过早
     if(get_acc_sio_val()==0 && delay_cnt==0){
       shutdown_count = delay_poweroff_cnt;
	 }
		if(delay_cnt<8)	 { //不接ACC开机，第一次延时处理断ACC处理动作
			delay_cnt++;
		}
#endif


#if 1  //开机logo影藏后延时亮屏
	if(lcd_off_cnt>0){
		lcd_off_cnt--;
	}
	if(lcd_off_cnt==1){
		if (lb_gal_get_screen_standby_status() == 0)
			lb_gal_screen_standby_switch();
	}
#endif
		if (get_acc_sio_val() != sb_last_s.last_acc_s && get_bat_status() ==
			BATTERY_CHARGING && delay_cnt==8) {

			sb_last_s.last_acc_s = get_acc_sio_val();
			// if (lb_gal_get_screen_standby_status() == 0)
			// 	lb_gal_screen_standby_switch();
			if (get_tf_status() == SD_PLUGIN)
				lb_system_mq_send(LB_SYSMSG_ALARM_ACC_CHANGE,
					&sb_last_s.last_acc_s, sizeof(int *), 0);
			#if POWER_VALUE_CHECK  //检测电压高低关机		
			if (!sb_last_s.last_acc_s && _get_bat_level()<4 &&_get_bat_level()>1) {
			#else
			if (!sb_last_s.last_acc_s) {
			#endif
				// cdr_interval.acc_closebk_duration_s = 9;
				// lb_gal_set_screen_standby_time(
				// 	cdr_interval.acc_closebk_duration_s);
				// lb_gal_set_screen_standby_enable(true);

    	#if DELAY_POWEROFF //提示“系统将在60S后自动关机”
				home_flag.dialog_value = DIALOG_ACC_OFF;
				home_flag.dialog_temp = &home_flag.dialog_value;
				lb_ui_send_msg(LB_MSG_HOME_DIALOG,
					(void *)&home_flag.dialog_temp,
					sizeof(void *), 0);
		#else
				shutdown_count=1;	
		#endif		
		        if(!shutdown_count)	
				shutdown_count = delay_poweroff_cnt;
#ifdef SET_ACC_SHUTDOWN_TIME
				cdr_interval.acc_shutdown_duration_h = 24;
				memcpy(&p_tm_shutdown, p_tm, sizeof(struct tm));
				p_tm_shutdown.tm_hour +=
					cdr_interval.acc_shutdown_duration_h;
				if (p_tm_shutdown.tm_hour >= 24) {
					p_tm_shutdown.tm_mday += p_tm_shutdown.tm_hour/24;
					p_tm_shutdown.tm_hour =
						p_tm_shutdown.tm_hour % 24;
					if (p_tm_shutdown.tm_mday >
						get_month_day(&p_tm_shutdown)) {
						p_tm_shutdown.tm_mday =
							p_tm_shutdown.tm_mday %
							get_month_day(&p_tm_shutdown);
						p_tm_shutdown.tm_mon++;
					if (p_tm_shutdown.tm_mon >= 12) {
						p_tm_shutdown.tm_mon = 0;
						p_tm_shutdown.tm_year++;
					}
					}
				}
#endif
			} else {
				  lb_view_close_isolate();
				cdr_interval.acc_closebk_duration_s = -1;
				cdr_interval.acc_shutdown_duration_h = -1;
				shutdown_count = 0;
				lb_gal_set_screen_standby_time(
					get_cfg_lcd_standby_time());
				lb_gal_set_screen_standby_enable(true);
				if (get_cfg_interval_record_enable() && get_tf_status() ==
					SD_PLUGIN) {
					home_flag.dialog_value = STOP_INTERVAL_REC;
					home_flag.dialog_temp = &home_flag.dialog_value;
					lb_ui_send_msg(LB_MSG_HOME_DIALOG,
						(void *)&home_flag.dialog_temp,
						sizeof(void *), 0);
				}
			}
		}
		#if POWER_VALUE_CHECK //检测电压高低关机
			if ((_get_bat_level()==4 || _get_bat_level()==1)  && !shutdown_count) {
				if(++low_high_cnt>=4){	
                  shutdown_count=5;
				}
			}else if((_get_bat_level()<4 &&_get_bat_level()>1)  && low_high_cnt>0){
                 low_high_cnt=0;
				 shutdown_count=0;

				}
		#endif
		
		if (!get_cfg_interval_record_enable() || get_tf_status() != SD_PLUGIN || _get_bat_level()==1|| _get_bat_level()==4) {
			if (shutdown_count > 0) {
				shutdown_count--;
				if (!shutdown_count) {
					home_flag.key_poweroff = 1;
					return_home = LB_MSG_POWEROFF;
					APP_LOG_W("\n");
					ret_home = &return_home;
					lb_ui_send_msg(LB_MSG_HOME, &ret_home,
						sizeof(void *), 0);
				}
			}
		}
		if (cdr_interval.acc_shutdown_duration_h > 0) {
			if (p_tm_shutdown.tm_hour == p_tm->tm_hour &&
				p_tm_shutdown.tm_mday == p_tm->tm_mday &&
				p_tm_shutdown.tm_mon == p_tm->tm_mon &&
				p_tm_shutdown.tm_year == p_tm->tm_year &&
				p_tm_shutdown.tm_min == p_tm->tm_min &&
				p_tm_shutdown.tm_sec == p_tm->tm_sec) {
				home_flag.key_poweroff = 1;
				return_home = LB_MSG_POWEROFF;
				ret_home = &return_home;
				APP_LOG_W("\n");
				lb_ui_send_msg(LB_MSG_HOME, &ret_home, sizeof(void *), 0);
			}
		}
#ifdef LOMBO_GPS
#ifdef SYNC_TIME_FROM_GPS
		if (!home_flag.set_gps_time && !get_gps_data(&gps_data)) {
			ret = get_gps_tm_time(&gps_time, gps_data);
			APP_LOG_W("%d %d %d %d %d %d\n", gps_data.year, gps_data.month,
				gps_data.day, gps_data.h, gps_data.m, gps_data.s);
			if (!ret) {
				if (mktime(&gps_time) > now + 60 ||
					mktime(&gps_time) < now - 60) {
					set_date_time(gps_time);
					APP_LOG_W("gps time %lld now:%lld\n",
							mktime(&gps_time), now);
				}
				home_flag.set_gps_time = 1;
			} else
				APP_LOG_E("get gps time error ret:%d\n", ret);
		}
#endif
#endif
#ifdef __EOS__RELEASE__MP__
		wdog_keepalive();
#endif
		rt_thread_delay(50);
		if (home_flag.thread_exit)
			pthread_exit(NULL);
	}

	return NULL;

}

/**
 * imgbtn_back - response function for back imgbtn
 * @param: imgbtn object pointer.
 *
 * This function use to response to back imgbtn, return to last view
 *
 * Returns SUCCESS if called when get success ; otherwise, return other custom values
 */
lb_int32 imgbtn_back(void *param)
{
	lb_int32 ret = -1;

	if (NULL == param) {
		APP_LOG_E("err [%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}

	ret = lb_view_close_isolate();
	if (0 == ret)
		return 0;
	ret = 0;
	// if (apps.car_recorder.ah) {
	// 	int car_recorder_status;
	// 	ret = lb_app_ctrl(&apps.car_recorder, CR_GET_RECORDER_STATUS, 0,
	// 			&car_recorder_status);
	// 	APP_LOG_D("ret:%d %d\n", ret, car_recorder_status);
	// 	if (car_recorder_status == 3) {
	// 		home_flag.dialog_value = QUIT_RECORDER;
	// 		home_flag.dialog_temp = &home_flag.dialog_value;
	// 		lb_ui_send_msg(LB_MSG_HOME_DIALOG, (void *)&home_flag.dialog_temp,
	// 			sizeof(void *), 0);
	// 		ret = 0;
	// 	} else
	// 		ret = lb_app_ctrl(&apps.car_recorder, RECORDER_EXIT, 0, 0);
	// }
	if (apps.smart_drive.ah) {
		ret = lb_app_ctrl(&apps.smart_drive, SMART_DRIVE_RETURN, 0, 0);
		if (ret == 0){
			ret = lb_app_ctrl(&apps.smart_drive, SMART_DRIVE_EXIT, 0, 0);
            left_right_lay_control(1);
		    lb_ui_send_msg(LB_MSG_CDR, NULL, 0, 0);
			}
	}
	if (apps.cdr_setting.ah) {
		ret = lb_app_ctrl(&apps.cdr_setting, CDR_SETTING_RETURN, 0, 0);
		if (ret == 0){
			ret = lb_app_ctrl(&apps.cdr_setting, CDR_SETTING_EXIT, 0, 0);
			left_right_lay_control(1);
		    lb_ui_send_msg(LB_MSG_CDR, NULL, 0, 0);
			}
	}
	if (apps.system_setting.ah) {
		ret = lb_app_ctrl(&apps.system_setting, SYSTEM_SETTING_RETURN, 0, 0);
		if (ret == 0){
			ret = lb_app_ctrl(&apps.system_setting, SYSTEM_SETTING_EXIT, 0,0);
			left_right_lay_control(1);
		    lb_ui_send_msg(LB_MSG_CDR, NULL, 0, 0);
			}
	}
	if (apps.file_explorer.ah) {
		ret = lb_app_ctrl(&apps.file_explorer, FILE_EXPLORER_RETURN, 0, 0);
		if (ret == 0){
			ret = lb_app_ctrl(&apps.file_explorer, FILE_EXPLORER_EXIT, 0, 0);
			left_right_lay_control(1);
		    lb_ui_send_msg(LB_MSG_CDR, NULL, 0, 0);
			}
	}

	if (ret < 0)
		APP_LOG_W("ret:%d\n", ret);

	return ret;
}

/**
 * imgbtn_home - response function for home imgbtn
 * @param: imgbtn object pointer.
 *
 * This function use to response to home imgbtn, close other app,back to home app
 *
 * Returns SUCCESS if called when get success ; otherwise, return other custom values
 */
lb_int32 imgbtn_home(void *param)
{
	lb_int32 ret = -1;
	int msg_temp = 0;

	if (param)
		msg_temp = *(int *)param;

	ret = lb_view_close_isolate();
	if (!(apps.car_recorder.ah || apps.smart_drive.ah || apps.cdr_setting.ah
			|| apps.system_setting.ah || apps.file_explorer.ah)) {
		if (0 == ret) {
			if (msg_temp == LB_USERDEF_SYSMSG_RETURN_HOME)
				lb_system_mq_syncsem_release();
			return 0;
		}
	}
	ret = 0;

	if (apps.car_recorder.ah) {
		if (NULL == param || msg_temp == LB_MSG_LOWPOWEROFF || msg_temp ==
			LB_MSG_POWEROFF || msg_temp == LB_USERDEF_SYSMSG_RETURN_HOME) {
			lb_app_ctrl(&apps.car_recorder, RECORDER_EXIT, 0xfe02, 0);
			/* lb_set_tone_flag(get_cfg_keytone_enable()); */
		} else {
			int car_recorder_status;
			ret = lb_app_ctrl(&apps.car_recorder, CR_GET_RECORDER_STATUS, 0,
					&car_recorder_status);
			APP_LOG_D("ret:%d %d\n", ret, car_recorder_status);
			if (car_recorder_status == 3) {
			 #if 0
				home_flag.dialog_value = QUIT_RECORDER;
				home_flag.dialog_temp = &home_flag.dialog_value;
				lb_ui_send_msg(LB_MSG_HOME_DIALOG,
					(void *)&home_flag.dialog_temp,
					sizeof(void *), 0);
				return ret;
			  #else
				ret = lb_app_ctrl(&apps.car_recorder, RECORDER_EXIT, 0,0);
			  #endif
			  
			} else
				ret = lb_app_ctrl(&apps.car_recorder, RECORDER_EXIT, 0,
				0);
		}
	}else if (msg_temp == LB_MSG_POWEROFF ||msg_temp==LB_USERDEF_SYSMSG_RETURN_HOME) {
		APP_LOG_W("close machine\n");
		record_obj_exit(&record_obj);
	}
	if (apps.smart_drive.ah)
		ret = lb_app_ctrl(&apps.smart_drive, SMART_DRIVE_EXIT, 0, 0);

	if (apps.cdr_setting.ah)
		ret = lb_app_ctrl(&apps.cdr_setting, CDR_SETTING_EXIT, 0, 0);

	if (apps.system_setting.ah)
		ret = lb_app_ctrl(&apps.system_setting, SYSTEM_SETTING_EXIT, 0, 0);

	if (apps.file_explorer.ah)
		ret = lb_app_ctrl(&apps.file_explorer, FILE_EXPLORER_EXIT, 0, 0);

	if (ret < 0) {
		APP_LOG_W("ret:%d\n", ret);
		if (msg_temp == LB_USERDEF_SYSMSG_RETURN_HOME)
			lb_system_mq_syncsem_release();
		if (!home_flag.key_poweroff)
			return ret;
	}
	if (NULL != param) {
		if (msg_temp == LB_MSG_LOWPOWEROFF)
			lb_ui_send_msg(LB_MSG_LOWPOWEROFF, NULL, 0, 0);
		else if (msg_temp == LB_MSG_POWEROFF)
			lb_ui_send_msg(LB_MSG_POWEROFF, NULL, 0, 0);
		else if (msg_temp == LB_MSG_CDR){
			left_right_lay_control(1);
			lb_ui_send_msg(LB_MSG_CDR, NULL, 0, 0);
			}
		else if (msg_temp == LB_MSG_FILE_EXPLORE){
			left_right_lay_control(0);
			lb_ui_send_msg(LB_MSG_FILE_EXPLORE, NULL, 0, 0);
			}
		else if (msg_temp == LB_MSG_CDR_SETTING){
			left_right_lay_control(0);
			lb_ui_send_msg(LB_MSG_CDR_SETTING, NULL, 0, 0);
			}
		else if (msg_temp == LB_MSG_SYSTEM_SETTING)	{
			left_right_lay_control(0);
			lb_ui_send_msg(LB_MSG_SYSTEM_SETTING, NULL, 0, 0);
			}
		else if (msg_temp == LB_MSG_SMART_DRIVE)	{
			left_right_lay_control(0);
			lb_ui_send_msg(LB_MSG_SMART_DRIVE, NULL, 0, 0);
			}
		
		else if (msg_temp == LB_USERDEF_SYSMSG_RETURN_HOME)
			lb_system_mq_syncsem_release();
		
		else{
        left_right_lay_control(1);
		lb_ui_send_msg(LB_MSG_CDR, NULL, 0, 0);
		}
	}

	return ret;
}

/**
 * imgbtn_cdr - response function for cdr imgbtn
 * @param: imgbtn object pointer.
 *
 * This function use to response to cdr imgbtn, open car recorder app
 *
 * Returns SUCCESS if called when get success ; otherwise, return other custom values
 */
lb_int32 imgbtn_cdr(void *param)
{

	lb_int32 ret = -1;
	int time;

	time = rt_time_get_msec();
	APP_LOG_D("time %d %d\n", time, __LINE__);
	if (apps.car_recorder.ah == 0) {
		/* lb_set_tone_flag(0); */
		home_view_hidden();
		ret = lb_app_open(&apps.car_recorder,
				ROOTFS_MOUNT_PATH"/apps/car_recorder.app", 0);
		if (ret < 0)
			APP_LOG_E("err ret:%d\n", ret);

	}
	time = rt_time_get_msec();
	APP_LOG_D("time %d %d\n", time, __LINE__);
	if (get_cfg_interval_record_enable() && get_tf_status() == SD_PLUGIN &&
		!get_acc_status()) {
		lb_gal_set_screen_standby_time(cdr_interval.acc_closebk_duration_s);
		lb_gal_set_screen_standby_enable(true);
	}

	return ret;


}
void *quit_recorder(void)
{
	void	*temp = NULL;

	lb_ui_send_msg(LB_MSG_HOME, &temp, sizeof(void *), 0);

	return NULL;
}
lb_int32 quit_recorder_init(void *param)
{
	lb_view_set_require_func(lb_view_get_parent(),
		(void *)quit_recorder);
	lb_view_set_require_param(lb_view_get_parent(),
		(void *)NULL);

	return 0;
}

lb_int32 quit_recorder_exit(void *param)
{

	APP_LOG_D("\n");

	return 0;
}

static lb_int32 home_dialog_show(void *param)
{
	APP_LOG_D("-----%d\n", *(int *)param);
	if (*(int *)param == QUIT_RECORDER)
		lb_view_show_isolate(16);
	else if (*(int *)param == PLEASE_PLUGIN_SDCARD)
		lb_view_show_isolate(8);
	else if (*(int *)param == START_INTERVAL_REC)
		lb_view_show_isolate(26);
	else if (*(int *)param == STOP_INTERVAL_REC)
		lb_view_show_isolate(27);
	else if (*(int *)param == DIALOG_ACC_OFF)
		lb_view_show_isolate(5);
	APP_LOG_D("-----\n");

	return 0;
}

static lb_int32 exit_current_app(void *param)
{
	lb_int32	ret = -1;
	lb_int32	type;

	if (NULL == param) {
		APP_LOG_E("err Invalid parameters!\n");
		return LB_ERROR_INVALID_PARAM;
	}

	type = *(lb_int32 *)param;
	APP_LOG_D("type = 0x%x\n", type);
	if (type == LB_MSG_CAR_RECORDER_BASE) {
		ret = lb_app_close(&apps.car_recorder);
		/* if (0 == ret)
			lb_set_tone_flag(get_cfg_keytone_enable()); */
	} else if (type == LB_MSG_SYSTEM_SETTING_BASE) {
		ret = lb_app_close(&apps.system_setting);
		if (0 == ret) {
			home_cfg_exit();
			home_cfg_init();
		}
	} else if (type == LB_MSG_CDR_SETTING_BASE) {
		ret = lb_app_close(&apps.cdr_setting);
		if (0 == ret) {
			home_cfg_exit();
			home_cfg_init();
		}
	} else if (type == LB_MSG_FILEEXP_BASE)
		ret = lb_app_close(&apps.file_explorer);
	else if (type == LB_MSG_SMART_DRIVE_BASE)
		ret = lb_app_close(&apps.smart_drive);

	if (ret < 0)
		APP_LOG_E("err ret:%d\n", ret);
	else
	//	home_view_show();

	APP_LOG_D("\n");

	return 0;
}

lb_int32 sys_msg_manage(lb_uint32 sys_msg, void *param)
{
	int ret = 0;
	lb_key_msg_data_t *sysmsg_addr;
	void *ret_home = NULL;

	if (sys_msg < LB_SYSMSG_ALARM_COLLIDE || sys_msg > LB_SYSMSG_ALARM_BSD_RIGHT)
		APP_LOG_W("msgtype:%x\n", sys_msg);
	switch (sys_msg) {
	case LB_SYSMSG_FS_PART_MOUNT_OK:
		if (fs_pro.fs_unformat_flag == 1) {
			set_tf_status(SD_PLUGIN_NOT_FORMAT);
			if (!apps.car_recorder.ah)
				lb_ui_send_msg(LB_MSG_SDCARD_MOUNT_FAIL, NULL, 0, 0);
		} else {
			set_tf_status(SD_PLUGIN);
			/* lb_ui_send_msg(LB_MSG_SDCARD_PLUGIN, NULL, 0, 0); */
		}
		break;
	case LB_SYSMSG_SD_PLUGOUT:
		set_tf_status(SD_PLUGOUT);
		/* lb_ui_send_msg(LB_MSG_SDCARD_PLUGOUT, NULL, 0, 0); */
		break;
	case LB_SYSMSG_POWER_LOW:
		return_home = LB_MSG_LOWPOWEROFF;
		ret_home = &return_home;
		lb_ui_send_msg(LB_MSG_HOME, &ret_home, sizeof(void *), 0);
		break;
	case LB_SYSMSG_SD_INIT_FAIL:
		lb_ui_send_msg(LB_MSG_SDCARD_INIT_FAIL, NULL, 0, 0);
		break;
	case LB_SYSMSG_FS_PART_MOUNT_FAIL:
		if (!strcmp("spinor", boot_get_boot_type()))
			set_tf_status(SD_PLUGIN_NOT_FORMAT);

		lb_ui_send_msg(LB_MSG_SDCARD_MOUNT_FAIL, (int *)&home_flag.delay_time,
			sizeof(int *), 0);
		break;
	case LB_SYSMSG_POWER_OFF:
		APP_LOG_W("LB_SYSMSG_POWER_OFF\n");
		return_home = LB_MSG_POWEROFF;
		ret_home = &return_home;
		lb_ui_send_msg(LB_MSG_HOME, &ret_home, sizeof(void *), 0);
		break;
	case LB_SYSMSG_KEY:
		sysmsg_addr = (lb_key_msg_data_t *)param;
		if (sysmsg_addr->code == POWER_KEY && sysmsg_addr->value == 3) {
			APP_LOG_W("LB_MSG_POWEROFF\n");
			home_flag.key_poweroff = 1;
			return_home = LB_MSG_POWEROFF;
			ret_home = &return_home;
			lb_ui_send_msg(LB_MSG_HOME, &ret_home, sizeof(void *), 0);
		}
		break;
	case LB_SYSMSG_USERDEF:
		if (*(int *)param == LB_USERDEF_SYSMSG_ENTER_CDR)
			lb_ui_send_msg(LB_MSG_CDR, (int *)&return_home, sizeof(int *), 0);
		else if (*(int *)param == LB_USERDEF_SYSMSG_RETURN_HOME) {
			return_home = LB_USERDEF_SYSMSG_RETURN_HOME;
			ret_home = &return_home;
			lb_ui_send_msg(LB_MSG_HOME, &ret_home, sizeof(void *), 0);
		 }else if (*(int *)param == LB_USERDEF_SYSMSG_PLAYER) {			
		    return_home = LB_MSG_FILE_EXPLORE;			
			ret_home = &return_home;			
			lb_ui_send_msg(LB_MSG_HOME, &ret_home, sizeof(void *), 0);		
		} else if (*(int *)param == LB_USERDEF_SYSMSG_CDR_SETTING) {			
		    return_home = LB_MSG_CDR_SETTING;			
			ret_home = &return_home;			
			lb_ui_send_msg(LB_MSG_HOME, &ret_home, sizeof(void *), 0);		
		} else if (*(int *)param == LB_USERDEF_SYSMSG_SYSTEM_SETTING) {			
		    return_home = LB_MSG_SYSTEM_SETTING;			
			ret_home = &return_home;			
			lb_ui_send_msg(LB_MSG_HOME, &ret_home, sizeof(void *), 0);		
		} else if (*(int *)param == LB_USERDEF_SYSMSG_CDR) {			
		    return_home = LB_MSG_CDR;			
			ret_home = &return_home;			
			lb_ui_send_msg(LB_MSG_HOME, &ret_home, sizeof(void *), 0);		
		}else if (*(int *)param == LB_USERDEF_SMART_DRIVE) {			
	        return_home = LB_MSG_SMART_DRIVE;			
		    ret_home = &return_home;			
		    lb_ui_send_msg(LB_MSG_HOME, &ret_home, sizeof(void *), 0);		
		}
		
		break;
	case LB_SYSMSG_DIALOG:
		home_flag.dialog_temp = (void *)param;
		lb_ui_send_msg(LB_MSG_HOME_DIALOG, (void *)&home_flag.dialog_temp,
			sizeof(void *), 0);
		break;
	case LB_SYSMSG_FS_PART_UNMOUNT_PREPARE:
		fs_pro.fs_unmount_pre_flag = 1;
		if (apps.file_explorer.ah)
			lb_ui_send_msg(LB_MSG_HOME, &ret_home, sizeof(void *), 0);
		while (apps.file_explorer.ah)
			rt_thread_delay(10);
		memcpy(&fs_pro.fs_unpre_event, param, sizeof(rt_event_t));
		fs_pro.fs_unpre = &fs_pro.fs_unpre_event;
		lb_ui_send_msg(LB_MSG_SDCARD_PRE_UNMOUNT, &fs_pro.fs_unpre,
			sizeof(rt_event_t *), 0);
		break;
	case LB_SYSMSG_AV_PLUGIN:
		set_av_status(1);
		break;
	case LB_SYSMSG_AV_PLUGOUT:
		set_av_status(0);
		break;
	case LB_SYSMSG_BACK_ON:
		if (home_flag.click_en == ENABLE_OFF)
			break;
		set_back_status(1);
		if (!apps.car_recorder.ah) {
			return_home = LB_MSG_CDR;
			ret_home = &return_home;
			lb_ui_send_msg(LB_MSG_HOME, &ret_home, sizeof(void *), 0);
		}
		break;
	case LB_SYSMSG_BACK_OFF:
		set_back_status(0);
		break;
	case LB_SYSMSG_SD_MOUNT_INV_VOL:
		if (!strcmp("spinor", boot_get_boot_type()))
			fs_pro.fs_unformat_flag = 1;

		break;
	case LB_SYSMSG_SD_PLUGIN:
		fs_pro.fs_unformat_flag = 0;
		break;
	case LB_SYSMSG_FS_PART_UNMOUNT:
		fs_pro.fs_unformat_flag = 0;
		fs_pro.fs_unmount_pre_flag = 0;
		break;
	case LB_SYSMSG_USB_POWER_DISCONNECT:
		APP_LOG_W("time: %d\n", rt_time_get_msec());
		home_flag.delay_time = 0;
		if (home_flag.lcd_brightness != ENABLE_OFF) {
#ifndef USB_POWEROFF_DIALOG
			set_backlight_status(ENABLE_OFF);
#endif
			home_flag.lcd_brightness = ENABLE_OFF;
			APP_LOG_W("get_bat_status %d\n", get_bat_status());
			if (get_bat_status() > 1) {
				return_home = LB_MSG_POWEROFF;
				ret_home = &return_home;
				lb_ui_send_msg(LB_MSG_HOME, &ret_home, sizeof(void *), 0);
			} else if (get_bat_status() == 1) {
				return_home = LB_MSG_LOWPOWEROFF;
				ret_home = &return_home;
				lb_ui_send_msg(LB_MSG_HOME, &ret_home, sizeof(void *), 0);
			 }
		}
		break;
	case LB_SYSMSG_USB_POWER_CONNECT:
		home_flag.delay_time = 0;
		if (home_flag.lcd_brightness != ENABLE_ON) {
#ifndef USB_POWEROFF_DIALOG
			set_backlight_status(ENABLE_ON);
#endif
			home_flag.lcd_brightness = ENABLE_ON;
		}
		break;
	case LB_SYSMSG_ALARM_ACC_CHANGE:
		APP_LOG_E("acc value %d\n", *(int *)param);
		if (*(int *)param == 0) {
			set_acc_status(ENABLE_OFF);
			if (get_cfg_interval_record_enable()) {
				home_flag.dialog_value = START_INTERVAL_REC;
				home_flag.dialog_temp = &home_flag.dialog_value;
				lb_ui_send_msg(LB_MSG_HOME_DIALOG,
					(void *)&home_flag.dialog_temp,
					sizeof(void *), 0);
				if (!apps.car_recorder.ah) {
					sleep(2);
					return_home = LB_MSG_CDR;
					ret_home = &return_home;
					lb_ui_send_msg(LB_MSG_HOME, &ret_home,
						sizeof(void *), 0);
				}
			}
		} else
			set_acc_status(ENABLE_ON);
		break;
	case LB_SYSMSG_SD_LOW_PERF:
		lb_ui_send_msg(LB_MSG_SDCARD_LOW_PERF, NULL, 0, 0);
		break;
	default:
		break;
	}

	return ret;
}

lb_int32 imgbtn_file_explorer(void *param)
{
	lb_int32 ret = -1;

	if (NULL == param) {
		APP_LOG_E("err Invalid parameters!\n");
		return LB_ERROR_NO_MEM;
	}
	if (!get_tf_status()) {
		home_flag.dialog_value = PLEASE_PLUGIN_SDCARD;
		lb_system_mq_send(LB_SYSMSG_DIALOG, &home_flag.dialog_value, sizeof(int),
			0);
		return 0;
	}
	home_view_hidden();
	if (apps.file_explorer.ah == 0) {
		ret = lb_app_open(&apps.file_explorer,
				ROOTFS_MOUNT_PATH"/apps/file_explorer.app", 0);
		if (ret < 0)
			APP_LOG_E("err ret:%d\n", ret);
	}

	return ret;
}

/**
 * imgbtn_smart_drive - response function for smart drive imgbtn
 * @param: imgbtn object pointer.
 *
 * This function use to response to adas imgbtn,open smart_drive app
 *
 * Returns SUCCESS if called when get success ; otherwise, return other custom values
 */
lb_int32 imgbtn_smart_drive(void *param)
{
	lb_int32 ret = -1;

	if (NULL == param) {
		APP_LOG_E("err Invalid parameters!\n");
		return LB_ERROR_NO_MEM;
	}

	home_view_hidden();
	if (apps.smart_drive.ah == 0) {
		ret = lb_app_open(&apps.smart_drive,
				ROOTFS_MOUNT_PATH"/apps/smart_drive.app", 0);
		if (ret < 0)
			APP_LOG_E("err ret:%d\n", ret);
	}

	return ret;
}

/**
 * imgbtn_cdr_setting - response function for cdr setting imgbtn
 * @param: imgbtn object pointer.
 *
 * This function use to response to cdr setting imgbtn,open cdr setting app
 *
 * Returns SUCCESS if called when get success ; otherwise, return other custom values
 */
lb_int32 imgbtn_cdr_setting(void *param)
{
	lb_int32 ret = -1;

	if (NULL == param) {
		APP_LOG_E("err [%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}

	home_view_hidden();
	if (apps.cdr_setting.ah == 0) {
		ret = lb_app_open(&apps.cdr_setting,
				ROOTFS_MOUNT_PATH"/apps/cdr_setting.app", 0);
		if (ret < 0)
			APP_LOG_E("err ret:%d\n", ret);
	}

	return ret;
}
lb_int32 power_off(void *param)
{
#ifdef USB_POWEROFF_DIALOG
	lb_view_show_isolate(5);
#else
	if (home_flag.key_poweroff){
		// lb_view_show_isolate(5);
		if (lb_gal_get_screen_standby_status() == 0)
			lb_gal_screen_standby_switch();
		lb_view_show_isolate(29);
		}
	else if (!get_bat_level())
		lb_ui_send_msg(LB_MSG_CDR, NULL, 0, 0);
	else {
		APP_LOG_W("\n");
#ifdef __EOS__RELEASE__MP__
		wdog_close();
#endif
		disp_close_device();
#ifdef LOMBO_GPS
		gps_close_device();
#endif
		rt_pm_shutdown();
	}
#endif

	return 0;
}

lb_int32 lower_poweroff(void *param)
{
	if (!get_bat_level())
		lb_ui_send_msg(LB_MSG_CDR, NULL, 0, 0);
	else {
#ifdef USB_POWEROFF_DIALOG
		lb_view_show_isolate(4);
#else
		APP_LOG_W("\n");
#ifdef __EOS__RELEASE__MP__
		wdog_close();
#endif
		disp_close_device();
#ifdef LOMBO_GPS
		gps_close_device();
#endif
		rt_pm_shutdown();
#endif
	}

	return 0;
}

lb_int32 sdcard_plugin(void *param)
{

	lb_view_show_isolate(9);

	return 0;
}
lb_int32 sdcard_init_fail(void *param)
{
	lb_view_show_isolate(14);

	return 0;
}

lb_int32 sdcard_mount_fail(void *param)
{
	lb_view_show_isolate(15);

	return 0;
}

lb_int32 sdcard_lower_pref(void *param)
{
	lb_view_show_isolate(25);

	return 0;
}

lb_int32 sdcard_plugout(void *param)
{
	lb_view_show_isolate(10);

	return 0;
}
lb_int32 sdcard_prepare_unmount(void *param)
{
	if (apps.car_recorder.ah) {
		lb_app_ctrl(&apps.car_recorder, CDR_FS_PART_UNMOUNT_PREPARE, 0, 0);
		lb_app_ctrl(&apps.car_recorder, CDR_MUTEX, 0, 0);
		if (*(rt_event_t *)param == NULL)
			APP_LOG_E("sdcard_prepare_unmount para null\n");
		else {
			rt_event_send(*(rt_event_t *)param, 1);
			while (fs_pro.fs_unmount_pre_flag)
				rt_thread_delay(5);
		}
		lb_app_ctrl(&apps.car_recorder, CDR_UNMUTEX, 0, 0);
	} else
		rt_event_send(*(rt_event_t *)param, 1);

	return 0;
}

/**
 * imgbtn_system_setting - response function for system setting imgbtn
 * @param: imgbtn object pointer.
 *
 * This function use to response to system setting imgbtn,open system setting app
 *
 * Returns SUCCESS if called when get success ; otherwise, return other custom values
 */
lb_int32 imgbtn_system_setting(void *param)
{
	lb_int32 ret = -1;

	if (NULL == param) {
		APP_LOG_E("err Invalid parameters!\n");
		return LB_ERROR_NO_MEM;
	}

	home_view_hidden();
	if (apps.system_setting.ah == 0) {
		ret = lb_app_open(&apps.system_setting,
				ROOTFS_MOUNT_PATH"/apps/system_setting.app", 0);
		if (ret < 0)
			APP_LOG_E("err ret:%d\n", ret);
	}

	return ret;
}

lb_int32 home_stop_recorder(void)
{
	if (apps.car_recorder.ah)
		lb_app_ctrl(&apps.car_recorder, CDR_FS_PART_UNMOUNT_PREPARE, 0, 0);

	return 0;
}

/**
 * home_reg_init_exit_funcs - reg init and exit function for widgets
 *
 * This function use to register init exit function for widgets
 *
 * Returns SUCCESS if called when get success ; otherwise, return other custom values
 */
lb_int32 home_reg_init_exit_funcs(void)
{
	lb_int32		err = SUCCESS;

	err = lb_fmngr_reg_init_func("leftbar_TFcard_init",
			statusbar_tfcard_init);
	err |= lb_fmngr_reg_init_func("leftbar_battery_init",
			statusbar_battery_init);
	err |= lb_fmngr_reg_init_func("rightbar_week_init",
			statusbar_week_init);
	err |= lb_fmngr_reg_init_func("rightbar_ytd_init",
			statusbar_ytd_init);
	err |= lb_fmngr_reg_init_func("rightbar_time_init",
			statusbar_time_init);




	
	err |= lb_fmngr_reg_init_func("system_poweroff_init",
			system_poweroff_init);
	err |= lb_fmngr_reg_exit_func("system_poweroff_exit",
			system_poweroff_exit);
	err |= lb_fmngr_reg_init_func("quit_recorder_init", quit_recorder_init);
	err |= lb_fmngr_reg_exit_func("quit_recorder_exit", quit_recorder_exit);
	err |= lb_fmngr_reg_init_func("format_sdcard_init", format_sdcard_init);
	err |= lb_fmngr_reg_exit_func("format_sdcard_exit", format_sdcard_exit);
	
	err |= lb_fmngr_reg_init_func("lock_format_sdcard_init", lock_format_sdcard_init);
	err |= lb_fmngr_reg_exit_func("lock_format_sdcard_exit", lock_format_sdcard_exit);

	return err;
}

/**
 * home_reg_resp_funcs - reg response function for widgets
 *
 * This function use to register response function for widgets
 *
 * Returns SUCCESS if called when get success ; otherwise, return other custom values
 */
lb_int32 home_reg_resp_funcs(void)
{
	lb_int32		err = SUCCESS;

	err = lb_reg_resp_msg_func(LB_MSG_HOME, imgbtn_home);
	err |= lb_reg_resp_msg_func(LB_MSG_BACK, imgbtn_back);
	err |= lb_reg_resp_msg_func(LB_MSG_SYSTEM_SETTING, imgbtn_system_setting);
	err |= lb_reg_resp_msg_func(LB_MSG_SMART_DRIVE, imgbtn_smart_drive);
	err |= lb_reg_resp_msg_func(LB_MSG_FILE_EXPLORE, imgbtn_file_explorer);
	err |= lb_reg_resp_msg_func(LB_MSG_CDR, imgbtn_cdr);
	err |= lb_reg_resp_msg_func(LB_MSG_CDR_SETTING, imgbtn_cdr_setting);
	err |= lb_reg_resp_msg_func(LB_MSG_POWEROFF, power_off);
	err |= lb_reg_resp_msg_func(LB_MSG_LOWPOWEROFF, lower_poweroff);
	err |= lb_reg_resp_msg_func(LB_MSG_SDCARD_PLUGIN, sdcard_plugin);
	err |= lb_reg_resp_msg_func(LB_MSG_SDCARD_PLUGOUT, sdcard_plugout);
	err |= lb_reg_resp_msg_func(LB_MSG_SDCARD_INIT_FAIL, sdcard_init_fail);
	err |= lb_reg_resp_msg_func(LB_MSG_SDCARD_MOUNT_FAIL, sdcard_mount_fail);
	err |= lb_reg_resp_msg_func(LB_MSG_SDCARD_PRE_UNMOUNT, sdcard_prepare_unmount);
	err |= lb_reg_resp_msg_func(LB_MSG_SDCARD_LOW_PERF, sdcard_lower_pref);
	err |= lb_reg_resp_msg_func(LB_MSG_HOME_DIALOG, home_dialog_show);
	err |= lb_reg_resp_msg_func(LB_MSG_HOME_BASE, exit_current_app);

	return err;
}

lb_int32 home_reg_resp_sys_funcs(void)
{
	lb_int32		err = SUCCESS;

#ifdef UPGRAGE_FIRMWARE
	if (g_upgrade_flag == 0)
		err = lb_reg_resp_sysmsg_func(LB_SYSTEM_MSG, sys_msg_manage);
#else
	err = lb_reg_resp_sysmsg_func(LB_SYSTEM_MSG, sys_msg_manage);
#endif

	return err;
}

/**
 * home_unreg_init_funcs - unreg init function for widgets
 *
 * This function use to unregister init function for widgets
 *
 * Returns SUCCESS if called when get success ; otherwise, return other custom values
 */
lb_int32 home_unreg_init_exit_funcs(void)
{
	lb_int32	err = SUCCESS;

	err = lb_fmngr_unreg_init_func(statusbar_time_init);
	err |= lb_fmngr_unreg_init_func(statusbar_ytd_init);
	err |= lb_fmngr_unreg_init_func(statusbar_week_init);
	err |= lb_fmngr_unreg_init_func(statusbar_battery_init);
	err |= lb_fmngr_unreg_init_func(statusbar_tfcard_init);
	err |= lb_fmngr_unreg_init_func(system_poweroff_init);
	err |= lb_fmngr_unreg_exit_func(system_poweroff_exit);
	err |= lb_fmngr_unreg_init_func(quit_recorder_init);
	err |= lb_fmngr_unreg_exit_func(quit_recorder_exit);
	err |= lb_fmngr_unreg_init_func(format_sdcard_init);
	err |= lb_fmngr_unreg_exit_func(format_sdcard_exit);

	err |= lb_fmngr_unreg_init_func(lock_format_sdcard_init);
	err |= lb_fmngr_unreg_exit_func(lock_format_sdcard_exit);

	return err;
}

/**
 * home_unreg_resp_funcs - unreg response function for widgets
 *
 * This function use to unregister response function for widgets
 *
 * Returns SUCCESS if called when get success ; otherwise, return other custom values
 */
lb_int32 home_unreg_resp_funcs(void)
{
	lb_int32	err = SUCCESS;

	err = lb_unreg_resp_msg_func(home_dialog_show);
	err |= lb_unreg_resp_msg_func(sdcard_lower_pref);
	err |= lb_unreg_resp_msg_func(sdcard_prepare_unmount);
	err |= lb_unreg_resp_msg_func(sdcard_mount_fail);
	err |= lb_unreg_resp_msg_func(sdcard_init_fail);
	err |= lb_unreg_resp_msg_func(sdcard_plugout);
	err |= lb_unreg_resp_msg_func(sdcard_plugin);
	err |= lb_unreg_resp_msg_func(lower_poweroff);
	err |= lb_unreg_resp_msg_func(power_off);
	err |= lb_unreg_resp_msg_func(imgbtn_cdr_setting);
	err |= lb_unreg_resp_msg_func(imgbtn_cdr);
	err |= lb_unreg_resp_msg_func(imgbtn_file_explorer);
	err |= lb_unreg_resp_msg_func(imgbtn_smart_drive);
	err |= lb_unreg_resp_msg_func(imgbtn_system_setting);
	err |= lb_unreg_resp_msg_func(imgbtn_back);
	err |= lb_unreg_resp_msg_func(imgbtn_home);
	err |= lb_unreg_resp_msg_func(exit_current_app);

	return err;
}

lb_int32 home_unreg_resp_sys_funcs(void)
{
	lb_int32	err = SUCCESS;

	err = lb_unreg_resp_sysmsg_func(sys_msg_manage);

	return err;
}
