/*
 * car_recorder_ctrl.c - car recorder widget reg & unreg implement
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
#include <math.h>
#include <pthread.h>
#include <rtthread.h>
#include "app_manage.h"
#include "mod_manage.h"
#include "car_recorder.h"
#include "car_recorder_common.h"
#include "car_recorder_ctrl.h"
#include "car_recorder.h"
#include "cJSON.h"
#include "lb_common.h"
#include "lb_gal_common.h"

#include "lb_ui.h"

#include "car_recorder_draw.h"

static sb_last_r_t sb_last_r;
static weekday_r weekday;
static apps_t apps;
static status_widget_t status_widget;
static status_property_t status_property;
static int get_object_flag;
static lb_uint8 cmd_hidden_flag;
static int last_alarm_dist_status;
#ifdef REAR_BSD_DEBUG
static int bsd_rect_left_color;
static int bsd_rect_right_color;
#endif
#ifdef LOMBO_GPS
static int gps_ret;
static int last_gps_status = -1;
static struct gps_data_t gps_data;
#endif
static void *line[CR_LINE_MAX];
static lb_point_t last_data_pos[CR_CAR_MAX];
static lb_size_t last_data_size[CR_CAR_MAX];
static float last_distance[CR_CAR_MAX];

static lb_point_t last_data_pos_bsd[CR_CAR_MAX];
static lb_size_t last_data_size_bsd[CR_CAR_MAX];
static float last_distance_bsd[CR_CAR_MAX];

static int alarm_distance_interval = ALARM_INTERVAL_COUNT;
static int alarm_left_bsd_tone_interval = ALARM_INTERVAL_COUNT;
static int alarm_right_bsd_tone_interval = ALARM_INTERVAL_COUNT;
static lb_line_points_t last_line_points[CR_LINE_MAX];

const char str_weekday[7][32] = {
	"STR_SUNDAY",
	"STR_MONDAY",
	"STR_TUESDAY",
	"STR_WEDNESDAY",
	"STR_THURSDAY",
	"STR_FRIDAY",
	"STR_SATURDAY"
};
static int disp_count;
#define WAR_DIST 8
#define WAR_INTERVAL 100
#define UP_STATUS_HIDE 0 //1: 代表隐藏上方状态栏  0:代表不隐藏上方状态栏


static void *cdr_status_refresh(void *parameter);
static void disp_get_object(void);
static void adas_disp_hidden_object(void);
static void adas_disp_show_line(void);
//static void bsd_disp_hidden_object(void);
static void bsd_disp_show_line(void);
lb_int32 data_save_flag=0; //录影界面设置功能时，赋值1

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
 * cdr_statusbar_init - create left & right bar refreash thread
 * @thread_id: thread id
 *
 * This function init statusbar refreash thread attribute,create bar refreash thread
 *
 * Returns 0
 */
lb_int32 cdr_status_thread_init(pthread_t *thread_id)
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
	shed_param.sched_priority = CDR_STATUS_THREAD_PRIORITY;
	ret = pthread_attr_setschedparam(&tmp_attr, &shed_param);
	if (ret != 0) {
		APP_LOG_E("\n set thread priority error: %d!\n", ret);
		goto exit;
	}
	ret = pthread_attr_setstacksize(&tmp_attr, (size_t)CDR_STATUS_STACK_SIZE);
	if (ret != 0) {
		APP_LOG_E("\n set thread stack size error: %d!!\n", ret);
		goto exit;
	}
	ret = pthread_create(thread_id, &tmp_attr, &cdr_status_refresh, NULL);
	if (ret != 0) {
		APP_LOG_E("\n set thread stack size error: %d!!\n", ret);
		goto exit;
	}
exit:
	pthread_attr_destroy(&tmp_attr);

	return ret;
}

/**
 * statusbar_refresh - the thread refresh statusbar view,update time & tfcard & battery
 * view,if status change
 * @parameter: reserved for user custom,no use
 *
 * This function use to refresh statusbar view in thread,update time & tfcard & battery
 * widget view,if status change,when car_recorder app is opened,  update recording status.
 */
static int auto_flag = 0;
static void *cdr_status_refresh(void *parameter)
{
	static lb_int32 r_time;
	static lb_int32 last_r_time = -1;
	struct tm *p_tm; /* time variable */
	time_t now;
	int ret = -1;
	int last_record_flag = 0;
	static char time_buf[TIME_MAX];
	static char rtime_buf[TIME_MAX];
	static char ytd_buf[TXT_MAX];
#ifdef LOMBO_GPS
	int gps_status;
	struct gps_data_t gps_data_temp;
#endif
	lb_view_get_obj_property_by_id(RECORD_ID,
		(void **)(&status_property.record_status));
	lb_view_get_obj_property_by_id(RECORD_TIME,
		(void **)(&status_property.record_time));
	lb_view_get_obj_property_by_id(RECORD_STATUS_BG_ID,
		(void **)(&status_property.record_status_bg));
	lb_view_get_obj_property_by_id(GPS_STATUS_ID,
		(void **)(&status_property.gps_status));
	lb_view_get_obj_property_by_id(RECORD_ONOFF_ID,
		(void **)(&status_property.record_onoff));	
	lb_view_get_obj_property_by_id(LOCK_STATUS_ID,
			(void **)(&status_property.lock_rec_status));	
	lb_view_get_obj_property_by_id(LOCK_ID, (void **)(&status_property.lock));

	lb_view_get_obj_property_by_id(MIC_STATUS_ID, (void **)(&status_property.mic));
	lb_view_get_obj_property_by_id(LIGHT_STATUS_ID, (void **)(&status_property.light));



	lb_view_get_obj_ext_by_id(RECORD_ID, &status_widget.record_status);
	lb_view_get_obj_ext_by_id(RECORD_TIME, &status_widget.record_time);
	lb_view_get_obj_ext_by_id(RECORD_STATUS_BG_ID, &status_widget.record_status_bg);
	lb_view_get_obj_ext_by_id(GPS_STATUS_ID, &status_widget.gps_status);
	lb_view_get_obj_ext_by_id(LOCK_ID, &status_widget.lock);
	lb_view_get_obj_ext_by_id(CMD_ID, &status_widget.cmd_bg);
	lb_view_get_obj_ext_by_id(CONT_TS_ID, &status_widget.cont_ts);
	lb_view_get_obj_ext_by_id(RECORD_ONOFF_ID, &status_widget.record_onoff);
	lb_view_get_obj_ext_by_id(LOCK_STATUS_ID,&status_widget.lock_rec_status);
	lb_view_get_obj_ext_by_id(MIC_STATUS_ID,&status_widget.mic);
	lb_view_get_obj_ext_by_id(LIGHT_STATUS_ID,&status_widget.light);
	lb_view_get_obj_ext_by_id(LIGHT_LAY_ID, &status_widget.light_bg);
	lb_view_get_obj_ext_by_id(UP_LAY_ID, &status_widget.up_bg);


	lb_view_get_obj_property_by_id(UP_WEEK_ID, (void **)(&status_property.up_week));
	lb_view_get_obj_property_by_id(UP_YTD_ID, (void **)(&status_property.up_ytd));
	lb_view_get_obj_property_by_id(UP_TIME_ID, (void **)(&status_property.up_time));
	lb_view_get_obj_property_by_id(UP_TFCARD_ID, (void **)(&status_property.up_tfcard));
	lb_view_get_obj_property_by_id(UP_BATTERY_ID, (void **)(&status_property.up_battery));

	lb_view_get_obj_ext_by_id(UP_WEEK_ID, &status_widget.up_week);
	lb_view_get_obj_ext_by_id(UP_YTD_ID, &status_widget.up_ytd);
	lb_view_get_obj_ext_by_id(UP_TIME_ID, &status_widget.up_time);
	lb_view_get_obj_ext_by_id(UP_TFCARD_ID, &status_widget.up_tfcard);
	lb_view_get_obj_ext_by_id(UP_BATTERY_ID, &status_widget.up_battery);
	
        sb_last_r.last_min=-1;
		sb_last_r.last_hour=-1;
		sb_last_r.last_day=-1;
		sb_last_r.last_mon=-1;
		sb_last_r.last_year=-1;
		sb_last_r.last_week=-1;
		sb_last_r.last_tf_s = -1;
	while (recorder_fd == NULL)
		rt_thread_delay(1);

	while (1) {
            now = time(RT_NULL);
			p_tm = localtime(&now);

		#if 0//根据时间段自动切换屏亮度
			if(p_tm->tm_hour>=6 && p_tm->tm_hour<18) //6:00-17:59 屏亮度默认“中”
			{
				if(auto_flag!=2){
					auto_flag=2;
					imgbtn_light_auto(1);
				}
             
			}else{//屏亮度默认“低”

             if(auto_flag!=1){
					auto_flag=1;
					imgbtn_light_auto(0);
				}
			}
		#endif


		#if 1//保存录影界面设置的参数
		    if(data_save_flag){
				data_save_flag++;
				if(data_save_flag>2){
                  data_save_flag=0;
                  cr_cfg_save_r();
				}
		   }
		#endif

		r_time = get_recorder_time(recorder_fd);
		if (get_recorder_status(recorder_fd) == RECORDER_STATUS_RECORD) {	

			if (get_lock_status() && last_lockstatus == LOCK_OFF) {
				if (get_interval_record_enable() && !get_acc_status()) {
					if (r_time > 30 * get_recorder_duration() - 30) {
						lock_front_file_num = 2;
						lock_rear_file_num = 2;

					} else {
						lock_front_file_num = 1;
						lock_rear_file_num = 1;
					}
				} else {
					if (r_time > get_recorder_duration() - 30) {
						lock_front_file_num = 2;
						lock_rear_file_num = 2;

					} else {
						lock_front_file_num = 1;
						lock_rear_file_num = 1;
					}
				}
				dialog_flag = DIALOG_LOCK_RECORDER_FILE;
				lb_system_mq_send(LB_SYSMSG_DIALOG, &dialog_flag,
					sizeof(int), 0);
			} else if (get_lock_status() == LOCK_OFF && last_lockstatus) {
				lb_view_get_obj_property_by_ext(status_widget.lock,
					(void **)(&status_property.lock));
				lb_gal_update_imgbtn(status_property.lock,
					status_widget.lock,
					LB_IMGBTN_UPD_SRC, get_lock_status(), NULL);
				lb_gal_update_img(status_property.lock_rec_status,
				status_widget.lock_rec_status, LB_IMG_UPD_SRC,
				0, NULL);		
				lock_front_file_num = 0;
			}

			last_lockstatus = get_lock_status();
			if (r_time != last_r_time) {
				if (!pano_get_status() && r_time % 2 == 1) {
					if (lb_gal_get_obj_hidden(
						status_widget.record_status) ==
						false)
						lb_gal_set_obj_hidden(
						status_widget.record_status, true);
				} else if (!pano_get_status() && r_time % 2 == 0) {
					if (lb_gal_get_obj_hidden(
						status_widget.record_time) == true)
						lb_gal_set_obj_hidden(
						status_widget.record_time, false);
					if (lb_gal_get_obj_hidden(
						status_widget.record_status) == true)
						lb_gal_set_obj_hidden(
						status_widget.record_status, false);
					if (lb_gal_get_obj_hidden(
						status_widget.record_status_bg) == true)
						lb_gal_set_obj_hidden(
						status_widget.record_status_bg, false);
				}
				if (!pano_get_status() && status_property.record_time) {
					memset(rtime_buf, 0, TIME_MAX);
					sprintf(rtime_buf, "%02d:%02d", r_time /
						60, r_time % 60);
					lb_gal_update_label(
						status_widget.record_time,
						LB_LABEL_UPD_TXT, rtime_buf);
				}
				last_r_time = r_time;
			}
			
			if (get_watermark_time_enable() == ENABLE_ON)
				watermark_time_fd(recorder_fd, p_tm);
			if (get_watermark_logo_enable() == ENABLE_ON)
				watermark_logo(recorder_fd, 14);
			if (get_av_status()) {
				if (get_watermark_time_enable() == ENABLE_ON)
					watermark_time_rd(recorder_rd, p_tm);
				if (get_watermark_logo_enable() == ENABLE_ON)
					watermark_logo(recorder_rd, 14);
			}
			#if 1
			if(last_record_flag==0){
				lb_gal_update_imgbtn(status_property.record_onoff,
					status_widget.record_onoff,
					LB_IMGBTN_UPD_SRC, 1, NULL);
			 }
			last_record_flag=1;
			#endif
		} else {
			if (lb_gal_get_obj_hidden(status_widget.record_time) == false)
				lb_gal_set_obj_hidden(status_widget.record_time, true);
			if (lb_gal_get_obj_hidden(status_widget.record_status) == false)
				lb_gal_set_obj_hidden(status_widget.record_status, true);
			if (lb_gal_get_obj_hidden(status_widget.record_status_bg)
				== false)
				lb_gal_set_obj_hidden(status_widget.record_status_bg,
					true);
			// if (get_lock_status()) {
			if (1) {
				set_lock_status(0);
				lb_gal_update_imgbtn(status_property.lock,
					status_widget.lock,
					LB_IMGBTN_UPD_SRC, get_lock_status(), NULL);	
				#if 1
				lb_gal_update_img(status_property.lock_rec_status,
				status_widget.lock_rec_status, LB_IMG_UPD_SRC,
				0, NULL);	
				#endif	
				lock_front_file_num = 0;
				lock_rear_file_num = 0;
			}
			r_time = 0;
			#if 1
			if(last_record_flag==1){
				lb_gal_update_imgbtn(status_property.record_onoff,
					status_widget.record_onoff,
					LB_IMGBTN_UPD_SRC, 0, NULL);
			 }
			last_record_flag=0;
			#endif
		}
#ifdef LOMBO_GPS
		gps_status = get_gps_speed(&gps_data_temp);
		if (last_gps_status != gps_status) {
			if (gps_status == -1) {
				lb_view_get_obj_property_by_id(GPS_STATUS_ID,
					(void **)(&status_property.gps_status));
				lb_gal_update_img(status_property.gps_status,
					status_widget.gps_status, LB_IMG_UPD_SRC, 0,
					NULL);
			} else if (gps_status == -2) {
				lb_view_get_obj_property_by_id(GPS_STATUS_ID,
					(void **)(&status_property.gps_status));
				lb_gal_update_img(status_property.gps_status,
				status_widget.gps_status, LB_IMG_UPD_SRC, 1, NULL);
			} else {
				lb_view_get_obj_property_by_id(GPS_STATUS_ID,
					(void **)(&status_property.gps_status));
				lb_gal_update_img(status_property.gps_status,
				status_widget.gps_status, LB_IMG_UPD_SRC, 2, NULL);
			}
			last_gps_status = gps_status;
		}
#endif
		if (!cmd_hidden_flag) {
			if (rt_time_get_msec() - click_time >= 10000)
				cdr_cmd_hidden();
		}
		ret = rt_sem_take(cdr_status_thread_sem, 50);
		if (ret == RT_EOK)
			pthread_exit(NULL);
/////////////系统时间刷新////////////////////
 if (status_widget.up_time) {
	if (p_tm->tm_min != sb_last_r.last_min ||
		p_tm->tm_hour != sb_last_r.last_hour) {
		sb_last_r.last_min = p_tm->tm_min;
		sb_last_r.last_hour = p_tm->tm_hour;
		memset(time_buf, 0, TXT_MAX);
		sprintf(time_buf, "%02d:%02d", p_tm->tm_hour,
			p_tm->tm_min);
		lb_gal_update_label(status_widget.up_time, LB_LABEL_UPD_TXT,
			time_buf);

	}
	if (sb_last_r.last_year != p_tm->tm_year  ||
		sb_last_r.last_mon != p_tm->tm_mon ||
		sb_last_r.last_day != p_tm->tm_mday) {
		memset(ytd_buf, 0, TXT_MAX);

		sprintf(ytd_buf, "%02d-%02d-%02d", p_tm->tm_year +
			1900, p_tm->tm_mon + 1, p_tm->tm_mday);

		lb_gal_update_label(status_widget.up_ytd,
			LB_LABEL_UPD_TXT, ytd_buf);
		sb_last_r.last_year = p_tm->tm_year;
		sb_last_r.last_mon = p_tm->tm_mon;
		sb_last_r.last_day = p_tm->tm_mday;
	}
}
if (status_widget.up_week) {
	weekday.wd_index = getWeekdayByYearday(p_tm->tm_year + 1900,
			p_tm->tm_mon + 1, p_tm->tm_mday);
	if (weekday.wd_index != sb_last_r.last_week) {
		if (status_property.up_week) {
			weekday.wd_str = elang_get_utf8_string_josn(
					str_weekday[weekday.wd_index]);
			lb_gal_update_label(status_widget.up_week,
				LB_LABEL_UPD_TXT, (void *)weekday.wd_str);
			status_property.up_week->str_id =
				elang_get_string_id_josn(
					str_weekday[weekday.wd_index]);
			sb_last_r.last_week = weekday.wd_index;
		}
	}
}
if (status_widget.up_tfcard) {
	if (get_sd_status() != sb_last_r.last_tf_s) {
		sb_last_r.last_tf_s = get_sd_status();
		APP_LOG_D("get_sd_status:%d\n", get_sd_status());
		if (get_sd_status() == SDCARD_PLUGIN)
			lb_gal_update_img(status_property.up_tfcard,
				status_widget.up_tfcard, LB_IMG_UPD_SRC,
				get_sd_status(), NULL);
		else
			lb_gal_update_img(status_property.up_tfcard,
				status_widget.up_tfcard, LB_IMG_UPD_SRC,
				get_sd_status(), NULL);
	}
}

/*if (sb_widget.up_battery) {
	if (get_bat_status() != sb_last_r.last_bat_s) {
		sb_last_r.last_bat_s = get_bat_status();

		APP_LOG_D("bat_status:%d\n", get_bat_status());

		lb_gal_update_img(sb_property.up_battery,
			sb_widget.up_battery, LB_IMG_UPD_SRC,
			get_bat_status(), NULL);
		home_flag.delay_time = 0;
	}


   } */


	}

	return NULL;

}


/**
 * statusbar_exit - exit statusbar refreash thread
 * @thread_id: thread id
 *
 * This function exit statusbar refreash thread
 *
 * Returns 0
 */
lb_int32 cdr_status_thread_exit(void)
{
	rt_sem_release(cdr_status_thread_sem);

	return 0;
}

lb_int32 cdr_cmd_hidden(void)
{
	if (0 == cmd_hidden_flag) {
		lb_view_get_obj_ext_by_id(CMD_ID, &status_widget.cmd_bg);
		lb_gal_set_obj_hidden(status_widget.cmd_bg, true);
		lb_view_get_obj_ext_by_id(LIGHT_LAY_ID, &status_widget.light_bg);
		lb_gal_set_obj_hidden(status_widget.light_bg, true);
      #if UP_STATUS_HIDE
        lb_view_get_obj_ext_by_id(UP_LAY_ID, &status_widget.up_bg);
		lb_gal_set_obj_hidden(status_widget.up_bg, true);
	  #endif	
		cmd_hidden_flag = 1;
	}

	return 0;
}

lb_int32 cdr_cmd_click_en(bool en)
{
	app_t *app_hd = NULL;

	app_hd = lb_app_check("home");
	if (app_hd)
		lb_app_ctrl(app_hd, MSG_FROM_CLICK, en, NULL);
	lb_gal_set_obj_click(status_widget.cont_ts, en);

	return 0;
}

/**
 * mic_imgbtn_init - mic imgbtn widget init function
 * @param: lb_obj_t object pointer.
 *
 * This function use to init mic imgbtn widget property
 *
 * Returns 0
 */
lb_int32 mic_imgbtn_init(void *param)
{
	lb_obj_t        *p_obj = (lb_obj_t *)param;
	lb_imgbtn_t     *p_imgbtn_prop = NULL;

	if (NULL == param) {
		APP_LOG_E("err Invalid parameters!\n");
		return LB_ERROR_NO_MEM;
	}

	p_imgbtn_prop = (lb_imgbtn_t *)p_obj->property;
	strcpy(p_imgbtn_prop->rel_img.p_img_src,
		p_imgbtn_prop->rel_img.src_list[get_mute_enable()]);
	strcpy(p_imgbtn_prop->pr_img.p_img_src,
		p_imgbtn_prop->pr_img.src_list[get_mute_enable()]);

	return 0;
}

/**
 * lock_imgbtn_init - lock imgbtn widget init function
 * @param: lb_obj_t object pointer.
 *
 * This function use to init lock imgbtn widget property
 *
 * Returns 0
 */
lb_int32 lock_imgbtn_init(void *param)
{
	lb_obj_t        *p_obj = (lb_obj_t *)param;
	lb_imgbtn_t     *p_imgbtn_prop = NULL;

	if (NULL == param) {
		APP_LOG_E("err Invalid parameters!\n");
		return LB_ERROR_NO_MEM;
	}

	p_imgbtn_prop = (lb_imgbtn_t *)p_obj->property;
	strcpy(p_imgbtn_prop->rel_img.p_img_src,
		p_imgbtn_prop->rel_img.src_list[get_lock_status()]);
	strcpy(p_imgbtn_prop->pr_img.p_img_src,
		p_imgbtn_prop->pr_img.src_list[get_lock_status()]);

	return 0;

}

/**
 * views_imgbtn_init - views imgbtn widget init function
 * @param: lb_obj_t object pointer.
 *
 * This function use to init views imgbtn widget property
 *
 * Returns 0
 */
lb_int32 views_imgbtn_init(void *param)
{
	lb_obj_t        *p_obj = (lb_obj_t *)param;
	lb_imgbtn_t     *p_imgbtn_prop = NULL;

	if (NULL == param) {
		APP_LOG_E("err Invalid parameters!\n");
		return LB_ERROR_NO_MEM;
	}

	p_imgbtn_prop = (lb_imgbtn_t *)p_obj->property;
	strcpy(p_imgbtn_prop->rel_img.p_img_src,
		p_imgbtn_prop->rel_img.src_list[get_views_status()]);
	strcpy(p_imgbtn_prop->pr_img.p_img_src,
		p_imgbtn_prop->pr_img.src_list[get_views_status()]);

	return 0;
}
#if 1
lb_int32 record_imgbtn_init(void *param)
{
	lb_obj_t        *p_obj = (lb_obj_t *)param;
	lb_imgbtn_t     *p_imgbtn_prop = NULL;
	lb_int8 flag=0;

	if (NULL == param) {
		APP_LOG_E("err Invalid parameters!\n");
		return LB_ERROR_NO_MEM;
	}

	p_imgbtn_prop = (lb_imgbtn_t *)p_obj->property;
	if (get_recorder_status(recorder_fd) == RECORDER_STATUS_RECORD) {
      flag=1;
	}else{
	  flag=0;
	}
	strcpy(p_imgbtn_prop->rel_img.p_img_src,
		p_imgbtn_prop->rel_img.src_list[flag]);
	strcpy(p_imgbtn_prop->pr_img.p_img_src,
		p_imgbtn_prop->pr_img.src_list[flag]);

	return 0;
}
#endif
/**
 * imgbtn_recorder - response function for recorder imgbtn
 * @param: imgbtn object pointer.
 *
 * This function use to response to recorder imgbtn,excute camera command
 *
 * Returns SUCCESS if called when get success ; otherwise, return other custom values
 */
lb_int32 imgbtn_recorder(void *param)
{
	lb_imgbtn_t     *p_imgbtn_prop = NULL;
	lb_int32 ret = -1;
	lb_int32 err = SUCCESS;

	click_time = rt_time_get_msec();
	if (get_sd_status() == SDCARD_NOT_PLUGIN) {
		dialog_flag = DIALOG_NEED_PLUGIN_SDCARD;
		lb_system_mq_send(LB_SYSMSG_DIALOG, &dialog_flag, sizeof(int), 0);
		return err;
	} else if (get_sd_status() == SDCARD_NOT_FORMAT) {
		dialog_flag = DIALOG_PLEASE_FORMAT_SDCARD;
		lb_system_mq_send(LB_SYSMSG_DIALOG, &dialog_flag, sizeof(int), 0);
		return -2;
	}
	if (NULL == param) {
		APP_LOG_E("Invalid parameters!\n");
		return LB_ERROR_NO_MEM;
	}
	err = lb_view_get_obj_property_by_ext(param, (void **)(&p_imgbtn_prop));
	if (err != SUCCESS) {
		APP_LOG_E("err [%d]\n", err);
		return -1;
	}
	if (cdr_comp_open_rear.thread_id) {
		pthread_join(cdr_comp_open_rear.thread_id, NULL);
		cdr_comp_open_rear.thread_id = NULL;
	}

	APP_LOG_D("\n");
	if (cdr_comp_open_front.thread_id) {
		pthread_join(cdr_comp_open_front.thread_id, NULL);
		cdr_comp_open_front.thread_id = NULL;
	}
	if (get_recorder_status(recorder_fd) == RECORDER_STATUS_RECORD) {
#ifdef OPEN_REAR_RECORDER
		pthread_mutex_lock(&cr_rear_mutex);
		if (get_recorder_status(recorder_rd) == RECORDER_STATUS_RECORD)
			recoder_stop(recorder_rd);
		pthread_mutex_unlock(&cr_rear_mutex);
#endif
#ifdef OPEN_FRONT_RECORDER
		pthread_mutex_lock(&cr_front_mutex);
		recoder_stop(recorder_fd);
		pthread_mutex_unlock(&cr_front_mutex);
#endif
       lb_gal_update_imgbtn(p_imgbtn_prop, param, LB_IMGBTN_UPD_SRC, 0,
		NULL);
	} else {
#ifdef OPEN_FRONT_RECORDER
		pthread_mutex_lock(&cr_front_mutex);
		recorder_start_front(recorder_fd);
		pthread_mutex_unlock(&cr_front_mutex);
#endif
#ifdef OPEN_REAR_RECORDER
		if (get_av_status()) {
			while (recorder_rd == NULL)
				oscl_mdelay(10);
			pthread_mutex_lock(&cr_rear_mutex);
			recorder_start_rear(recorder_rd);
			pthread_mutex_unlock(&cr_rear_mutex);
		}
#endif
    lb_gal_update_imgbtn(p_imgbtn_prop, param, LB_IMGBTN_UPD_SRC, 1,
		NULL);
	}

	return ret;
}

/**
 * imgbtn_camera - response function for camera imgbtn
 * @param: imgbtn object pointer.
 *
 * This function use to response to camera imgbtn, excute camera command
 *
 * Returns SUCCESS if called when get success ; otherwise, return other custom values
 */
lb_int32 imgbtn_camera(void *param)
{
	lb_int32 ret = 0;
	struct tm *p_tm; /* time variable */
	time_t now;
      #if 1  //zq watermark_time
	       now = time(RT_NULL);
			p_tm = localtime(&now);
			if (get_watermark_time_enable() == ENABLE_ON)
				watermark_time_fd(recorder_fd, p_tm);
			if (get_watermark_logo_enable() == ENABLE_ON)
				watermark_logo(recorder_fd, 14);
			if (get_av_status()) {
				if (get_watermark_time_enable() == ENABLE_ON)
					watermark_time_rd(recorder_rd, p_tm);
				if (get_watermark_logo_enable() == ENABLE_ON)
					watermark_logo(recorder_rd, 14);
			}
     #endif
	click_time = rt_time_get_msec();
	if (get_sd_status() == SDCARD_NOT_PLUGIN) {
		dialog_flag = DIALOG_NEED_PLUGIN_SDCARD;
		lb_system_mq_send(LB_SYSMSG_DIALOG, &dialog_flag, sizeof(int), 0);
		return -1;
	} else if (get_sd_status() == SDCARD_NOT_FORMAT) {
		dialog_flag = DIALOG_PLEASE_FORMAT_SDCARD;
		lb_system_mq_send(LB_SYSMSG_DIALOG, &dialog_flag, sizeof(int), 0);
		return -2;
	}
	if (cdr_comp_open_rear.thread_id) {
		pthread_join(cdr_comp_open_rear.thread_id, NULL);
		cdr_comp_open_rear.thread_id = NULL;
	}

	if (cdr_comp_open_front.thread_id) {
		pthread_join(cdr_comp_open_front.thread_id, NULL);
		cdr_comp_open_front.thread_id = NULL;
	}
	rm_over_file(PICTURE_PATH, PICTURE_MAX_NUM, ".jpg");
	now = time(RT_NULL);
	p_tm = localtime(&now);
	memset(picture_frontfilename, 0, sizeof(picture_frontfilename));
	sprintf(picture_frontfilename, PICTURE_PATH"%02d%02d%02d%02d%02d%02d_f.jpg",
		p_tm->tm_year + 1900, p_tm->tm_mon + 1, p_tm->tm_mday,
		p_tm->tm_hour, p_tm->tm_min, p_tm->tm_sec);
	memset(picture_rearfilename, 0, sizeof(picture_rearfilename));
	sprintf(picture_rearfilename, PICTURE_PATH"%02d%02d%02d%02d%02d%02d_r.jpg",
		p_tm->tm_year + 1900, p_tm->tm_mon + 1, p_tm->tm_mday,
		p_tm->tm_hour, p_tm->tm_min, p_tm->tm_sec);
	if (NULL == param) {
		APP_LOG_E("Invalid parameters!\n");
		return LB_ERROR_NO_MEM;
	}
	tack_picture(FRONT_RECORDER, p_tm);
	lb_play_warningtone(ROOTFS_MOUNT_PATH"/res/picture_ok.wav");
	if (get_av_status())
		tack_picture(REAR_RECORDER, p_tm);

	return ret;
}

/**
 * imgbtn_lock - response function for lock imgbtn
 * @param: imgbtn object pointer.
 *
 * This function use to response to lock imgbtn, excute lock command
 *
 * Returns SUCCESS if called when get success ; otherwise, return other custom values
 */
lb_int32 imgbtn_lock(void *param)
{
	lb_imgbtn_t     *p_imgbtn_prop = NULL;
	lb_int32 err = SUCCESS;

	click_time = rt_time_get_msec();
	if (get_sd_status() == SDCARD_NOT_PLUGIN) {
		dialog_flag = DIALOG_NEED_PLUGIN_SDCARD;
		lb_system_mq_send(LB_SYSMSG_DIALOG, &dialog_flag, sizeof(int), 0);
		return err;
	} else if (get_sd_status() == SDCARD_NOT_FORMAT) {
		dialog_flag = DIALOG_PLEASE_FORMAT_SDCARD;
		lb_system_mq_send(LB_SYSMSG_DIALOG, &dialog_flag, sizeof(int), 0);
		return -2;
	} else if (get_recorder_status(recorder_fd) != RECORDER_STATUS_RECORD)
		return -3;

	if (NULL == param) {
		APP_LOG_E("Invalid parameters!\n");
		return LB_ERROR_NO_MEM;
	}
	err = lb_view_get_obj_property_by_ext(param, (void **)(&p_imgbtn_prop));
	if (err != SUCCESS) {
		APP_LOG_E("get_obj_property err [%d]\n", err);
		return err;
	}
	if (get_lock_status() == 0) {
		set_lock_status(1);
		last_lockstatus = get_lock_status();
		lock_front_file_num = 1;
		lock_rear_file_num = 1;
		dialog_flag = DIALOG_LOCK_RECORDER_FILE;
		lb_system_mq_send(LB_SYSMSG_DIALOG, &dialog_flag, sizeof(int), 0);
	} else {
		dialog_flag = DIALOG_UNLOCK_RECORDER_FILE;
		lb_system_mq_send(LB_SYSMSG_DIALOG, &dialog_flag, sizeof(int), 0);
		set_lock_status(0);
		last_lockstatus = get_lock_status();
		lock_front_file_num = 0;
		lock_rear_file_num = 0;
	}

	lb_gal_update_imgbtn(p_imgbtn_prop, param, LB_IMGBTN_UPD_SRC, get_lock_status(),
		NULL);
	lb_gal_update_img(status_property.lock_rec_status,
				status_widget.lock_rec_status, LB_IMG_UPD_SRC,
				get_lock_status(), NULL);	

	return err;
}

/**
 * imgbtn_cdr_setting - response function for cdr setting imgbtn
 * @param: imgbtn object pointer.
 *
 * This function use to response to cdr setting imgbtn,open cdr setting app
 *
 * Returns SUCCESS if called when get success ; otherwise, return other custom values
 */
lb_int32 imgbtn_setting_mode(void *param)
{
	int para;
	if (NULL == param) {
		APP_LOG_E("err [%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}

	para = LB_USERDEF_SYSMSG_CDR_SETTING;
	lb_system_mq_send(LB_SYSMSG_USERDEF, &para, sizeof(int *), 0);

	return 0;
}


lb_int32 imgbtn_file_mode(void *param)
{
    int para;
	if (NULL == param) {
		APP_LOG_E("err Invalid parameters!\n");
		return LB_ERROR_NO_MEM;
	}
	if (get_sd_status() == SDCARD_NOT_PLUGIN) {
		dialog_flag = DIALOG_NEED_PLUGIN_SDCARD;
		lb_system_mq_send(LB_SYSMSG_DIALOG, &dialog_flag, sizeof(int), 0);
		return 0;
	}

	para = LB_USERDEF_SYSMSG_PLAYER;
	lb_system_mq_send(LB_SYSMSG_USERDEF, &para, sizeof(int *), 0);

	return 0;
}
lb_int32 imgbtn_light_add(void *param)
{
    int para;
    int value=0;
    int idx=0;
	if (NULL == param) {
		APP_LOG_E("err Invalid parameters!\n");
		return LB_ERROR_NO_MEM;
	}
	value=get_backlight_value();
	idx=backlight_value_to_idx(value);
	idx++;
	if(idx>2)
	idx=2;
	set_backlight_value(idx);

	 lb_gal_update_img(status_property.light,
				status_widget.light, LB_IMG_UPD_SRC,
				idx, NULL);
    data_save_flag=1;
	return 0;
}
lb_int32 imgbtn_light_abate(void *param)
{
    int para;
    int value=0;
    int idx=0;
	if (NULL == param) {
		APP_LOG_E("err Invalid parameters!\n");
		return LB_ERROR_NO_MEM;
	}
	value=get_backlight_value();
	idx=backlight_value_to_idx(value);
	
	if(idx>0)
     idx--;
	set_backlight_value(idx);

	 lb_gal_update_img(status_property.light,
				status_widget.light, LB_IMG_UPD_SRC,
				idx, NULL);
    data_save_flag=1;
	return 0;
}
lb_int32 imgbtn_light_auto(int tmp)
{
	set_backlight_value(tmp);

	 lb_gal_update_img(status_property.light,
				status_widget.light, LB_IMG_UPD_SRC,
				tmp, NULL);

	return 0;
}
/**
 * imgbtn_smart_drive - response function for smart drive imgbtn
 * @param: imgbtn object pointer.
 *
 * This function use to response to adas imgbtn,open smart_drive app
 *
 * Returns SUCCESS if called when get success ; otherwise, return other custom values
 */
lb_int32 imgbtn_smart_drive_mode(void *param)
{
    int para;
	if (NULL == param) {
		APP_LOG_E("err Invalid parameters!\n");
		return LB_ERROR_NO_MEM;
	}

	para = LB_USERDEF_SMART_DRIVE;
	lb_system_mq_send(LB_SYSMSG_USERDEF, &para, sizeof(int *), 0);
	return 0;
}

/**
 * imgbtn_mic - response function for micphone imgbtn
 * @param: imgbtn object pointer.
 *
 * This function use to response to micphone imgbtn,open or close micphone
 * Returns SUCCESS if called when get success ; otherwise, return other custom values
 */
lb_int32 imgbtn_mic(void *param)
{
	lb_imgbtn_t     *p_imgbtn_prop = NULL;
	lb_int32 err = SUCCESS;

	click_time = rt_time_get_msec();
	if (get_sd_status() == SDCARD_NOT_PLUGIN) {
		dialog_flag = DIALOG_NEED_PLUGIN_SDCARD;
		lb_system_mq_send(LB_SYSMSG_DIALOG, &dialog_flag, sizeof(int), 0);
		return err;
	} else if (get_sd_status() == SDCARD_NOT_FORMAT) {
		dialog_flag = DIALOG_PLEASE_FORMAT_SDCARD;
		lb_system_mq_send(LB_SYSMSG_DIALOG, &dialog_flag, sizeof(int), 0);
		return -2;
	}

	if (NULL == param) {
		APP_LOG_E("Invalid parameters!\n");
		return LB_ERROR_NO_MEM;
	}
	err = lb_view_get_obj_property_by_ext(param, (void **)(&p_imgbtn_prop));
	if (err != SUCCESS) {
		APP_LOG_E("get_obj_property err [%d]\n", err);
		return err;
	}
	if (cdr_comp_open_rear.thread_id) {
		pthread_join(cdr_comp_open_rear.thread_id, NULL);
		cdr_comp_open_rear.thread_id = NULL;
	}
	if (cdr_comp_open_front.thread_id) {
		pthread_join(cdr_comp_open_front.thread_id, NULL);
		cdr_comp_open_front.thread_id = NULL;
	}
	if (get_mute_enable() == ENABLE_OFF)
		set_mute_enable(ENABLE_ON);
	else
		set_mute_enable(ENABLE_OFF);
	lb_gal_update_imgbtn(p_imgbtn_prop, param, LB_IMGBTN_UPD_SRC, get_mute_enable(),
		NULL);
    lb_gal_update_img(status_property.mic,
				status_widget.mic, LB_IMG_UPD_SRC,
				get_mute_enable(), NULL);	

	data_save_flag=1;			
	return err;
}

/**
 * imgbtn_views - response function for display views imgbtn
 * @param: imgbtn object pointer.
 *
 * This function use to response to display views imgbtn, switch display views
 * Returns SUCCESS if called when get success ; otherwise, return other custom values
 */
lb_int32 imgbtn_views(void *param)
{
	lb_imgbtn_t     *p_imgbtn_prop = NULL;
	lb_int32 err = SUCCESS;

	click_time = rt_time_get_msec();
	if (NULL == param) {
		APP_LOG_E("Invalid parameters!\n");
		return LB_ERROR_NO_MEM;
	}
	err = lb_view_get_obj_property_by_ext(param, (void **)(&p_imgbtn_prop));
	if (err != SUCCESS) {
		APP_LOG_E("get_obj_property err [%d]\n", err);
		return err;
	}
	if (!get_av_status()) {
		dialog_flag = DIALOG_NEED_PLUGIN_AV;
		lb_system_mq_send(LB_SYSMSG_DIALOG, &dialog_flag, sizeof(int), 0);
		return err;
	}

	if (cdr_comp_open_rear.thread_id) {
		pthread_join(cdr_comp_open_rear.thread_id, NULL);
		cdr_comp_open_rear.thread_id = NULL;
	}

	if (cdr_comp_open_front.thread_id) {
		pthread_join(cdr_comp_open_front.thread_id, NULL);
		cdr_comp_open_front.thread_id = NULL;
	}
	if (get_views_status() == FRONT_VIEW) {
		set_views_status(BACK_VIEW);
#ifdef PANO_DEBUG
		preview_stop(recorder_fd);
		pano_start(recorder_rd);
#else
		recorder_set_views(recorder_fd, recorder_rd, BACK_VIEW);
#endif
		adas_disp_hidden_object();
		if (bsd_get_enable() == ENABLE_ON) {
			if (bsd_get_status() == BSD_OPEN)
				bsd_disp_show_line();
		}
	} else if (get_views_status() == BACK_VIEW) {
#ifdef PANO_DEBUG
		pano_stop(recorder_rd);
#endif
		set_views_status(FRONT_VIEW);
		recorder_set_views(recorder_fd, recorder_rd, FRONT_VIEW);
		bsd_disp_hidden_object();
		if (adas_get_enable() == ENABLE_ON) {
			if (adas_get_status() == ADAS_OPEN)
				adas_disp_show_line();
		}
	}
	lb_gal_update_imgbtn(p_imgbtn_prop, param, LB_IMGBTN_UPD_SRC, get_views_status(),
		NULL);


#if 1
		int hidden = 0;
		hidden = lb_gal_get_obj_hidden(right_war_img);
		if(!hidden){
		lb_gal_set_obj_hidden(right_war_img, true); 
		}
		hidden = lb_gal_get_obj_hidden(left_war_img);
		if(!hidden){
		lb_gal_set_obj_hidden(left_war_img, true);	
		}	
		
#endif
	return err;
}

static lb_int32 dialog_show(void *param)
{
	APP_LOG_D("---%d\n", *(int *)param);

	if (*(int *)param == DIALOG_RECORDER_FILE_FULL)
		lb_view_show_isolate(11);
/*	else if (*(int *)param == DIALOG_LOCKFILE_FULL)
		lb_view_show_isolate(30);//12*/
	else if (*(int *)param == DIALOG_NEED_PLUGIN_SDCARD)
		lb_view_show_isolate(8);
	else if (*(int *)param == DIALOG_PARK_MONITOR_EVENT)
        lb_view_show_isolate(13);
	else if (*(int *)param == DIALOG_NEED_PLUGIN_AV)
		lb_view_show_isolate(19);
	else if (*(int *)param == DIALOG_PLEASE_FORMAT_SDCARD)
		lb_view_show_isolate(24);
	else if (*(int *)param == DIALOG_LOCK_RECORDER_FILE)
		lb_view_show_isolate(22);
	else if (*(int *)param == DIALOG_UNLOCK_RECORDER_FILE)
		lb_view_show_isolate(23);
	APP_LOG_D("\n");

	return 0;
}

static lb_int32 cmd_refresh(void *param)
{
	if (0 == cmd_hidden_flag) {
		lb_gal_set_obj_hidden(status_widget.cmd_bg, true);
		lb_gal_set_obj_hidden(status_widget.light_bg, true);
		#if UP_STATUS_HIDE
		lb_gal_set_obj_hidden(status_widget.up_bg, true);
		#endif
		cmd_hidden_flag = 1;
	} else {
		lb_gal_set_obj_hidden(status_widget.cmd_bg, false);
		lb_gal_set_obj_hidden(status_widget.light_bg, false);
		#if UP_STATUS_HIDE
		lb_gal_set_obj_hidden(status_widget.up_bg, false);
		#endif
		cmd_hidden_flag = 0;
	}
	click_time = rt_time_get_msec();

	return 0;
}

lb_int32 update_views_btn(void)
{
	lb_int32 ret = 0;

	if (status_property.views == NULL) {
		ret = lb_view_get_obj_property_by_id(VIEWS_ID,
				(void **)(&status_property.views));

		if (ret != 0) {
			APP_LOG_E("failed\n");
			ret = -1;
			goto exit;
		}
	}

	if (status_widget.views == NULL) {
		ret = lb_view_get_obj_ext_by_id(VIEWS_ID,
				(void **)&status_widget.views);

		if (ret != 0) {
			APP_LOG_E("failed\n");
			ret = -1;
			goto exit;
		}
	}

	lb_gal_update_imgbtn(status_property.views,
		status_widget.views,
		LB_IMGBTN_UPD_SRC,
		get_views_status(), NULL);

exit:
	return ret;
}

lb_int32 sys_msg_manage(lb_uint32 sys_msg, void *param)
{
	int ret = 0;
	lb_int32 offset;
	if (sys_msg < LB_SYSMSG_ALARM_COLLIDE || sys_msg > LB_SYSMSG_ALARM_BSD_RIGHT)
		APP_LOG_W("msgtype:%x\n", sys_msg);
	switch (sys_msg) {
	case LB_SYSMSG_AV_PLUGIN:
		while (recorder_rd == NULL)
			oscl_mdelay(10);
#ifdef DEFAULT_FRONT_VIEW
		if (get_views_status() == PIP_VIEW || get_views_status() == BACK_VIEW)
			preview_start(recorder_rd);
#else
		set_views_status(BACK_VIEW);
		update_views_btn();
		recorder_set_views(recorder_fd, recorder_rd, BACK_VIEW);
		adas_disp_hidden_object();
#endif
		pthread_mutex_lock(&cr_front_mutex);
		if (get_recorder_status(recorder_fd) == RECORDER_STATUS_RECORD) {
			recoder_stop(recorder_fd);
			recorder_start_front(recorder_fd);
		}
		pthread_mutex_unlock(&cr_front_mutex);
		pthread_mutex_lock(&cr_rear_mutex);
		if (get_recorder_status(recorder_rd) == RECORDER_STATUS_RECORD)
			recoder_stop(recorder_rd);
		recorder_start_rear(recorder_rd);
		pthread_mutex_unlock(&cr_rear_mutex);
		break;
	case LB_SYSMSG_AV_PLUGOUT:
		if (get_views_status() == PIP_VIEW || get_views_status()
			== BACK_VIEW) {
			lb_view_get_obj_property_by_id(VIEWS_ID,
				(void **)(&status_property.views));
			lb_view_get_obj_ext_by_id(VIEWS_ID, &status_widget.views);
			preview_stop(recorder_rd);
			set_views_status(FRONT_VIEW);
			recorder_set_views(recorder_fd, recorder_rd, FRONT_VIEW);
			bsd_disp_hidden_object();
			if (adas_get_enable() == ENABLE_ON) {
				if (adas_get_status() == ADAS_OPEN)
					adas_disp_show_line();
			}
			lb_gal_update_imgbtn(status_property.views,
				status_widget.views, LB_IMGBTN_UPD_SRC,
				get_views_status(), NULL);
		}
		pthread_mutex_lock(&cr_rear_mutex);
		if (get_recorder_status(recorder_rd) == RECORDER_STATUS_RECORD)
			recoder_stop(recorder_rd);
		pthread_mutex_unlock(&cr_rear_mutex);
		pthread_mutex_lock(&cr_front_mutex);
		if (get_recorder_status(recorder_fd) == RECORDER_STATUS_RECORD) {
			recoder_stop(recorder_fd);
			recorder_start_front(recorder_fd);
		}
		pthread_mutex_unlock(&cr_front_mutex);
		break;
	case LB_SYSMSG_BACK_ON:
		if (cdr_comp_open_rear.thread_id) {
			pthread_join(cdr_comp_open_rear.thread_id, NULL);
			cdr_comp_open_rear.thread_id = NULL;
		}
		if (cdr_comp_open_front.thread_id) {
			pthread_join(cdr_comp_open_front.thread_id, NULL);
			cdr_comp_open_front.thread_id = NULL;
		}

		lb_gal_set_screen_standby_enable(false);
		if (lb_gal_get_screen_standby_status() == 0)
			lb_gal_screen_standby_switch();

		APP_LOG_W("Step 0\n");
		if (pano_get_enable() == ENABLE_OFF) {
			if (get_views_status() == FRONT_VIEW) {
				set_views_status(BACK_VIEW);
				update_views_btn();
				recorder_set_views(recorder_fd, recorder_rd, BACK_VIEW);
				adas_disp_hidden_object();
				if (bsd_get_enable() == ENABLE_ON) {
					if (bsd_get_status() == BSD_OPEN)
						bsd_disp_show_line();
				}
			}
			break;
		}

		APP_LOG_W("Step 1\n");
		if (pano_check() != 0) {
			if (get_views_status() == FRONT_VIEW) {
				set_views_status(BACK_VIEW);
				update_views_btn();
				recorder_set_views(recorder_fd, recorder_rd, BACK_VIEW);
				adas_disp_hidden_object();
				if (bsd_get_enable() == ENABLE_ON) {
					if (bsd_get_status() == BSD_OPEN)
						bsd_disp_show_line();
				}
			}
			break;
		}

		APP_LOG_W("Step 2\n");
		if (get_views_status() == FRONT_VIEW) {
			APP_LOG_W("Step 2.0\n");
			if (adas_get_enable() == ENABLE_ON && adas_get_status() ==
				ADAS_OPEN) {
					cdr_comp_stop_adas.com_flag = COMP_STOP_ADAS;
					cdr_comp_thread_init(&cdr_comp_stop_adas);
					if (cdr_comp_stop_adas.thread_id) {
						pthread_join(cdr_comp_stop_adas.thread_id,
							NULL);
						cdr_comp_stop_adas.thread_id = NULL;
					}
					adas_disp_hidden_object();
			}
			preview_stop(recorder_fd);
		} else if (get_views_status() == BACK_VIEW) {
			APP_LOG_W("Step 2.1\n");
			if (bsd_get_enable() == ENABLE_ON && bsd_get_status() ==
				BSD_OPEN) {
				cdr_comp_stop_bsd.com_flag = COMP_STOP_BSD;
				cdr_comp_thread_init(&cdr_comp_stop_bsd);
				if (cdr_comp_stop_bsd.thread_id) {
					pthread_join(cdr_comp_stop_bsd.thread_id, NULL);
					cdr_comp_stop_bsd.thread_id = NULL;
				}
			}
			preview_stop(recorder_rd);
			bsd_disp_hidden_object();
		}

		APP_LOG_D("\n");
		cdr_cmd_hidden();
		cdr_cmd_click_en(false);
		pano_start(recorder_rd);
		break;
	case LB_SYSMSG_BACK_OFF:
		lb_gal_set_screen_standby_enable(true);
#ifdef DEFAULT_FRONT_VIEW
		if (pano_get_enable() == ENABLE_OFF) {
			if (get_views_status() == BACK_VIEW) {
				set_views_status(FRONT_VIEW);
				update_views_btn();
				recorder_set_views(recorder_fd, recorder_rd, FRONT_VIEW);
				bsd_disp_hidden_object();
				if (adas_get_enable() == ENABLE_ON) {
					if (adas_get_status() == ADAS_OPEN)
						adas_disp_show_line();
				}
			}
			break;
		}

		APP_LOG_W("Step 0\n");

		if (pano_check() != 0) {
			if (get_views_status() == BACK_VIEW) {
				set_views_status(FRONT_VIEW);
				update_views_btn();
				recorder_set_views(recorder_fd, recorder_rd, FRONT_VIEW);
				bsd_disp_hidden_object();
				if (adas_get_enable() == ENABLE_ON) {
					if (adas_get_status() == ADAS_OPEN)
						adas_disp_show_line();
				}
			}
			break;
		}
		APP_LOG_W("Step 1\n");

		pano_stop(recorder_rd);
		cdr_cmd_click_en(true);
		APP_LOG_W("Step 2\n");
		if (get_views_status() == FRONT_VIEW) {
			APP_LOG_W("Step 2.0\n");
			_get_config_disp_para(&front_preview_para, FRONT_FULL_SCREEN);
			lb_recorder_ctrl(recorder_fd, LB_REC_SET_DISP_PARA,
				&front_preview_para);
			preview_start(recorder_fd);
			lb_recorder_ctrl(recorder_fd, LB_REC_SET_LAYER_LEVEL,
				(void *)VIDEO_LAYER_BOTTOM);
			bsd_disp_hidden_object();
			if (adas_get_enable() == ENABLE_ON) {
				if (adas_get_status() == ADAS_OPEN)
					adas_disp_show_line();
			}
		} else if (get_views_status() == BACK_VIEW) {
			APP_LOG_W("Step 2.1\n");
			_get_config_disp_para(&rear_preview_para, REAR_FULL_SCREEN);
			lb_recorder_ctrl(recorder_rd, LB_REC_SET_DISP_PARA,
				&rear_preview_para);
			preview_start(recorder_rd);
			lb_recorder_ctrl(recorder_rd, LB_REC_SET_LAYER_LEVEL,
				(void *)VIDEO_LAYER_BOTTOM);
			adas_disp_hidden_object();
			if (bsd_get_enable() == ENABLE_ON) {
				if (bsd_get_status() == BSD_OPEN)
					bsd_disp_show_line();
			}
		}
#else
		if (pano_get_enable() == ENABLE_OFF) {
			if (get_views_status() == FRONT_VIEW) {
				set_views_status(BACK_VIEW);
				update_views_btn();
				recorder_set_views(recorder_fd, recorder_rd, BACK_VIEW);
				adas_disp_hidden_object();
				if (bsd_get_enable() == ENABLE_ON) {
					if (bsd_get_status() == BSD_OPEN)
						bsd_disp_show_line();
				}
			}
			break;
		}

		APP_LOG_W("Step 0\n");

		if (pano_check() != 0) {
			if (get_views_status() == FRONT_VIEW) {
				set_views_status(BACK_VIEW);
				update_views_btn();
				recorder_set_views(recorder_fd, recorder_rd, BACK_VIEW);
				adas_disp_hidden_object();
				if (bsd_get_enable() == ENABLE_ON) {
					if (bsd_get_status() == BSD_OPEN)
						bsd_disp_show_line();
				}
			}
			break;
		}
		APP_LOG_W("Step 1\n");
		pano_stop(recorder_rd);
		cdr_cmd_click_en(true);
		APP_LOG_W("get_views_status() %d\n", get_views_status());
		if (get_views_status() == FRONT_VIEW) {
			APP_LOG_W("Step 2.0\n");
			_get_config_disp_para(&front_preview_para, FRONT_FULL_SCREEN);
			lb_recorder_ctrl(recorder_fd, LB_REC_SET_DISP_PARA,
				&front_preview_para);
			preview_start(recorder_fd);
			lb_recorder_ctrl(recorder_fd, LB_REC_SET_LAYER_LEVEL,
				(void *)VIDEO_LAYER_BOTTOM);
			bsd_disp_hidden_object();
			if (adas_get_enable() == ENABLE_ON) {
				if (adas_get_status() == ADAS_OPEN)
					adas_disp_show_line();
			}
		} else if (get_views_status() == BACK_VIEW) {
			APP_LOG_W("Step 2.1\n");
			_get_config_disp_para(&rear_preview_para, REAR_FULL_SCREEN);
			lb_recorder_ctrl(recorder_rd, LB_REC_SET_DISP_PARA,
				&rear_preview_para);
			preview_start(recorder_rd);
			lb_recorder_ctrl(recorder_rd, LB_REC_SET_LAYER_LEVEL,
				(void *)VIDEO_LAYER_BOTTOM);
			adas_disp_hidden_object();
			if (bsd_get_enable() == ENABLE_ON) {
				if (bsd_get_status() == BSD_OPEN)
					bsd_disp_show_line();
			}
		}
#endif
		break;
	case LB_SYSMSG_ALARM_ACC_CHANGE:
		APP_LOG_E("acc value %d\n", *(int *)param);
		lb_view_get_obj_property_by_id(INTERVAL_REC_ID,
				(void **)(&status_property.interval_rec_status));
		lb_view_get_obj_ext_by_id(INTERVAL_REC_ID,
			&status_widget.interval_rec_status);
		if (*(int *)param == 0) {
			if (get_interval_record_enable()) {
				lb_gal_update_img(status_property.interval_rec_status,
				status_widget.interval_rec_status, LB_IMG_UPD_SRC,
				1, NULL);
				if (cdr_comp_open_rear.thread_id) {
					pthread_join(cdr_comp_open_rear.thread_id, NULL);
					cdr_comp_open_rear.thread_id = NULL;
				}
				cdr_comp_close_rear.com_flag = COMP_STOP_REAR_RECORDER;
				cdr_comp_thread_init(&cdr_comp_close_rear);

				APP_LOG_D("\n");
				if (cdr_comp_open_front.thread_id) {
					pthread_join(cdr_comp_open_front.thread_id, NULL);
					cdr_comp_open_front.thread_id = NULL;
				}
				APP_LOG_D("\n");
				if (cdr_comp_close_rear.thread_id) {
					pthread_join(cdr_comp_close_rear.thread_id, NULL);
					cdr_comp_close_rear.thread_id = NULL;
				}
				APP_LOG_D("\n");
				if (adas_get_enable() == ENABLE_ON) {
					if (adas_get_status() == ADAS_OPEN) {
						cdr_comp_stop_adas.com_flag =
							COMP_STOP_ADAS;
						cdr_comp_thread_init(&cdr_comp_stop_adas);
					if (cdr_comp_stop_adas.thread_id) {
						pthread_join(cdr_comp_stop_adas.thread_id,
							NULL);
						cdr_comp_stop_adas.thread_id = NULL;
					}
						adas_disp_hidden_object();
					}
				}
				cdr_comp_close_front.com_flag = COMP_STOP_FRONT_RECORDER;
				cdr_comp_thread_init(&cdr_comp_close_front);
				if (cdr_comp_close_front.thread_id) {
					pthread_join(cdr_comp_close_front.thread_id,
						NULL);
					cdr_comp_close_front.thread_id = NULL;
				}
				adas_disp_hidden_object();
				bsd_disp_hidden_object();
				cdr_comp_open_front.com_flag = COMP_START_FRONT_RECORDER;
				cdr_comp_thread_init(&cdr_comp_open_front);
				APP_LOG_D("%dms\n", rt_time_get_msec());
				cdr_comp_open_rear.com_flag = COMP_START_REAR_RECORDER;
				cdr_comp_thread_init(&cdr_comp_open_rear);
			}

		} else {
			if (get_interval_record_enable()) {
				lb_gal_update_img(status_property.interval_rec_status,
				status_widget.interval_rec_status, LB_IMG_UPD_SRC,
				0, NULL);
				if (cdr_comp_open_rear.thread_id) {
					pthread_join(cdr_comp_open_rear.thread_id, NULL);
					cdr_comp_open_rear.thread_id = NULL;
				}
				cdr_comp_close_rear.com_flag = COMP_STOP_REAR_RECORDER;
				cdr_comp_thread_init(&cdr_comp_close_rear);

				APP_LOG_D("\n");
				if (cdr_comp_open_front.thread_id) {
					pthread_join(cdr_comp_open_front.thread_id, NULL);
					cdr_comp_open_front.thread_id = NULL;
				}
				APP_LOG_D("\n");
				if (cdr_comp_close_rear.thread_id) {
					pthread_join(cdr_comp_close_rear.thread_id, NULL);
					cdr_comp_close_rear.thread_id = NULL;
				}
				APP_LOG_D("\n");
				if (adas_get_enable() == ENABLE_ON) {
					if (adas_get_status() == ADAS_OPEN) {
						cdr_comp_stop_adas.com_flag =
							COMP_STOP_ADAS;
						cdr_comp_thread_init(&cdr_comp_stop_adas);
					if (cdr_comp_stop_adas.thread_id) {
						pthread_join(cdr_comp_stop_adas.thread_id,
						NULL);
						cdr_comp_stop_adas.thread_id = NULL;
					}
						adas_disp_hidden_object();
					}
				}
				cdr_comp_close_front.com_flag = COMP_STOP_FRONT_RECORDER;
				cdr_comp_thread_init(&cdr_comp_close_front);
				if (cdr_comp_close_front.thread_id) {
					pthread_join(cdr_comp_close_front.thread_id,
						NULL);
					cdr_comp_close_front.thread_id = NULL;
				}
				cdr_comp_open_front.com_flag = COMP_START_FRONT_RECORDER;
				cdr_comp_thread_init(&cdr_comp_open_front);
				APP_LOG_D("%dms\n", rt_time_get_msec());
				cdr_comp_open_rear.com_flag = COMP_START_REAR_RECORDER;
				cdr_comp_thread_init(&cdr_comp_open_rear);
				if (adas_get_enable() == ENABLE_ON) {
					if (adas_get_status() == ADAS_OPEN)
						adas_disp_show_line();
				}
				if (bsd_get_enable() == ENABLE_ON) {
					if (bsd_get_status() == BSD_OPEN)
						bsd_disp_show_line();
				}
			}
		}
		break;
	case LB_SYSMSG_GSENSOR:
		if (get_recorder_status(recorder_fd) == RECORDER_STATUS_RECORD) {
			if (get_lock_status() == 0) {
				set_lock_status(1);
				if(get_warn_tone_enable())
				lb_play_warningtone(ROOTFS_MOUNT_PATH"/res/boot_music.wav");
				APP_LOG_D("\n");
				lb_view_get_obj_property_by_ext(status_widget.lock,
					(void **)(&status_property.lock));
				lb_gal_update_imgbtn(status_property.lock,
					status_widget.lock, LB_IMGBTN_UPD_SRC,
					LOCK_ON, NULL);
				lb_gal_update_img(status_property.lock_rec_status,
				status_widget.lock_rec_status, LB_IMG_UPD_SRC,
				1, NULL);	
				APP_LOG_D("\n");
			} else {
				if (get_interval_record_enable() && !get_acc_status()) {
					if (get_recorder_time(recorder_fd) >
						30 * get_recorder_duration() - 30) {
						lock_front_file_num = 2;
						lock_rear_file_num = 2;

					} else {
						lock_front_file_num = 1;
						lock_rear_file_num = 1;
					}
				} else {
					if (get_recorder_time(recorder_fd) >
						get_recorder_duration() - 30) {
						lock_front_file_num = 2;
						lock_rear_file_num = 2;

					} else {
						lock_front_file_num = 1;
						lock_rear_file_num = 1;
					}
				}
			}
		}
		break;
	case LB_SYSMSG_LOCKFILE_FULL:
		dialog_temp = (void *)param;
		lb_ui_send_msg(LB_MSG_DIALOG, (void *)&dialog_temp, sizeof(void *), 0);
		break;
	case LB_SYSMSG_RECORDER_FILE_FULL:
		dialog_temp = (void *)param;
		lb_ui_send_msg(LB_MSG_DIALOG, (void *)&dialog_temp, sizeof(void *), 0);
		if (get_recorder_status(recorder_fd) == RECORDER_STATUS_RECORD) {
#ifdef OPEN_REAR_RECORDER
			pthread_mutex_lock(&cr_rear_mutex);
			if (get_av_status())
				recoder_stop(recorder_rd);
			pthread_mutex_unlock(&cr_rear_mutex);
#endif
#ifdef OPEN_FRONT_RECORDER
			pthread_mutex_lock(&cr_front_mutex);
			recoder_stop(recorder_fd);
			pthread_mutex_unlock(&cr_front_mutex);
#endif
		}
		break;
	case LB_SYSMSG_DIALOG:
		dialog_temp = (void *)param;
		lb_ui_send_msg(LB_MSG_DIALOG, (void *)&dialog_temp, sizeof(void *), 0);
		if (*(int*)param == DIALOG_PARK_MONITOR_EVENT){
          if(get_warn_tone_enable())
          lb_play_warningtone(ROOTFS_MOUNT_PATH"/res/park.wav");
		}else if (*(int*)param == DIALOG_NEED_PLUGIN_SDCARD){
			if(get_warn_tone_enable())
            lb_play_warningtone(ROOTFS_MOUNT_PATH"/res/sd_err.wav");
		}
		break;
	case LB_SYSMSG_FS_PART_MOUNT_OK:
		APP_LOG_D("\n");
		if (get_recorder_status(recorder_fd) != RECORDER_STATUS_RECORD) {
#ifdef OPEN_FRONT_RECORDER
			pthread_mutex_lock(&cr_front_mutex);
			recorder_start_front(recorder_fd);
			pthread_mutex_unlock(&cr_front_mutex);
#endif
#ifdef OPEN_REAR_RECORDER
			pthread_mutex_lock(&cr_rear_mutex);
			if (get_av_status())
				recorder_start_rear(recorder_rd);
			pthread_mutex_unlock(&cr_rear_mutex);
#endif
		}
		break;
	case LB_SYSMSG_SD_PLUGOUT:
	     lb_view_show_isolate(8);
		APP_LOG_D("%d %d\n", get_recorder_status(recorder_rd),
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
        if(get_warn_tone_enable())
            lb_play_warningtone(ROOTFS_MOUNT_PATH"/res/sd_err.wav");
		break;

	case LB_SYSMSG_SD_INIT_FAIL:
	case LB_SYSMSG_FS_PART_MOUNT_FAIL:
		if(get_warn_tone_enable())
		lb_play_warningtone(ROOTFS_MOUNT_PATH"/res/sd_err.wav");
		break;	
	case LB_SYSMSG_ALARM_DISTANCE:
		// if (lb_get_font_lang_idx() == CHINESE_S || lb_get_font_lang_idx() ==
		// 	CHINESE_T)
		// 	lb_play_warningtone(ROOTFS_MOUNT_PATH"/res/alarm_distance.wav");
		// else
		// 	lb_play_warningtone(
		// 	ROOTFS_MOUNT_PATH"/res/alarm_distance_en.wav");
		break;
	case LB_SYSMSG_ALARM_PRESSURE:
		// if (lb_get_font_lang_idx() == CHINESE_S || lb_get_font_lang_idx() ==
		// 	CHINESE_T)
		// 	lb_play_warningtone(ROOTFS_MOUNT_PATH"/res/alarm_pressure.wav");
		// else
		// 	lb_play_warningtone(
		// 	ROOTFS_MOUNT_PATH"/res/alarm_pressure_en.wav");
		break;
	case LB_SYSMSG_ALARM_LAUNCH:
		// if (lb_get_font_lang_idx() == CHINESE_S || lb_get_font_lang_idx() ==
		// 	CHINESE_T)
		// 	lb_play_warningtone(ROOTFS_MOUNT_PATH"/res/alarm_launch.wav");
		// else
		// 	lb_play_warningtone(ROOTFS_MOUNT_PATH"/res/alarm_launch_en.wav");
		break;
	case LB_SYSMSG_ALARM_COLLIDE:
		// if (lb_get_font_lang_idx() == CHINESE_S || lb_get_font_lang_idx() ==
		// 	CHINESE_T)
		// 	lb_play_warningtone(ROOTFS_MOUNT_PATH"/res/alarm_collide.wav");
		// else
		// 	lb_play_warningtone(ROOTFS_MOUNT_PATH"/res/alarm_collide_en.wav");
		break;
	case LB_SYSMSG_ALARM_BSD_LEFT:
		// if (lb_get_font_lang_idx() == CHINESE_S || lb_get_font_lang_idx() ==
		// 	CHINESE_T)
		// 	//lb_play_warningtone(ROOTFS_MOUNT_PATH"/res/alarm_left.wav");
		// 	lb_play_warningtone(ROOTFS_MOUNT_PATH"/res/BSD_2k.wav");
		// else
		// 	//lb_play_warningtone(ROOTFS_MOUNT_PATH"/res/alarm_left_en.wav");
		// 	lb_play_warningtone(ROOTFS_MOUNT_PATH"/res/BSD_2k.wav");
		break;
	case LB_SYSMSG_ALARM_BSD_RIGHT:
		// if (lb_get_font_lang_idx() == CHINESE_S || lb_get_font_lang_idx() ==
		// 	CHINESE_T)
		// 	//lb_play_warningtone(ROOTFS_MOUNT_PATH"/res/alarm_right.wav");
		// 	lb_play_warningtone(ROOTFS_MOUNT_PATH"/res/BSD_2k.wav");
		// else
		// 	//lb_play_warningtone(ROOTFS_MOUNT_PATH"/res/alarm_right_en.wav");
		// 	lb_play_warningtone(ROOTFS_MOUNT_PATH"/res/BSD_2k.wav");
		break;

	case LB_MSG_CONT_EVENT:
		{
			lb_view_close_isolate();
		lb_cont_event_info_t *p_offset;
		/*offset = *(lb_int32 *)param;*/
		p_offset = (lb_cont_event_info_t *)param;
		offset	= p_offset->y_offset*2;
		/*APP_LOG_W("***************offset: %d***************\n", offset);*/
		if (get_views_status() == FRONT_VIEW) {
			move_cdr_disp_para(recorder_fd, &front_preview_para, offset);
			set_front_cropx(front_preview_para.crop.x);

		} else if (get_views_status() == BACK_VIEW && (pano_get_enable() ==
		ENABLE_OFF  || !get_back_status() || pano_check() == -1)) {
			move_cdr_disp_para(recorder_rd, &rear_preview_para, offset);
			set_rear_cropx(rear_preview_para.crop.x);
		}
		rt_thread_delay(1);
		data_save_flag=1;
		break;
		}
	case LB_SYSMSG_USERDEF:
		if (*(int *)param == LB_USERDEF_SYSMSG_STOP_REC) {
			APP_LOG_W("\n");
			pthread_mutex_lock(&cr_rear_mutex);
			if (get_av_status())
				recoder_stop(recorder_rd);
			pthread_mutex_unlock(&cr_rear_mutex);
			pthread_mutex_lock(&cr_front_mutex);
			recoder_stop(recorder_fd);
			pthread_mutex_unlock(&cr_front_mutex);
		}
		break;
	default:
		break;
	}
	click_time = rt_time_get_msec();

	return ret;


}

/**
 * record_time_init - current time label widget init function
 * @param: lb_obj_t object pointer.
 *
 * This function init time label widget property
 *
 * Returns 0
 */
lb_int32 record_time_init(void *param)
{
	lb_obj_t        *p_obj = (lb_obj_t *)param;
	lb_label_t      *p_label_prop = NULL;

	if (NULL == param) {
		APP_LOG_E("Invalid parameters!\n");
		return LB_ERROR_NO_MEM;
	}
	p_label_prop = (lb_label_t *)p_obj->property;
	p_label_prop->txt = malloc(TIME_MAX);
	if (p_label_prop->txt) {
		memset(p_label_prop->txt, 0x00, TIME_MAX);
		strcpy(p_label_prop->txt, "00:00");
	}

	return 0;

}
lb_int32 record_time_exit(void *param)
{
	APP_LOG_D("\n");
	lb_obj_t        *p_obj = (lb_obj_t *)param;
	lb_label_t      *p_label_prop = NULL;

	if (NULL == param) {
		APP_LOG_E("Invalid parameters!\n");
		return LB_ERROR_NO_MEM;
	}

	p_label_prop = (lb_label_t *)p_obj->property;
	if (p_label_prop->txt) {
		free(p_label_prop->txt);
		p_label_prop->txt = NULL;
	}

	return 0;

}

lb_int32 resolution_init(void *param)
{
	lb_obj_t        *p_obj = (lb_obj_t *)param;
	lb_label_t      *p_label_prop = NULL;

	if (NULL == param) {
		APP_LOG_E("Invalid parameters!\n");
		return LB_ERROR_NO_MEM;
	}
	p_label_prop = (lb_label_t *)p_obj->property;
	p_label_prop->txt = malloc(TXT_MAX);
	if (p_label_prop->txt) {
		memset(p_label_prop->txt, 0x00, TXT_MAX);
		if (get_recorder_resolution() == RESOLUTION_720P)
			strcpy(p_label_prop->txt, "720P");
		else if (get_recorder_resolution() == RESOLUTION_1080P)
			strcpy(p_label_prop->txt, "HD1080P");
		else if (get_recorder_resolution() == RESOLUTION_1296P)
			strcpy(p_label_prop->txt, "1296P");
		else if (get_recorder_resolution() == RESOLUTION_1440P)
			strcpy(p_label_prop->txt, "1440P");
		else if (get_recorder_resolution() == RESOLUTION_2K)
			strcpy(p_label_prop->txt, "2K");
	}

	return 0;

}
lb_int32 resolution_exit(void *param)
{
	APP_LOG_D("\n");
	lb_obj_t        *p_obj = (lb_obj_t *)param;
	lb_label_t      *p_label_prop = NULL;

	if (NULL == param) {
		APP_LOG_E("Invalid parameters!\n");
		return LB_ERROR_NO_MEM;
	}

	p_label_prop = (lb_label_t *)p_obj->property;
	if (p_label_prop->txt) {
		free(p_label_prop->txt);
		p_label_prop->txt = NULL;
	}
	return 0;

}

lb_int32 gps_status_init(void *param)
{
	APP_LOG_D("\n");

	return 0;
}

lb_int32 interval_record_status_init(void *param)
{
	APP_LOG_D("\n");
	lb_obj_t        *p_obj = (lb_obj_t *)param;
	lb_img_t	*p_img_prop = NULL;

	if (NULL == param) {
		APP_LOG_E("err Invalid parameters!\n");
		return LB_ERROR_NO_MEM;
	}

	p_img_prop = (lb_img_t *)p_obj->property;
	if (get_acc_status() || !get_interval_record_enable())
		strcpy(p_img_prop->src, p_img_prop->src_list[0]);
	else
		strcpy(p_img_prop->src, p_img_prop->src_list[1]);

	return 0;
}
#if 1
lb_int32 lock_record_status_init(void *param)
{
	APP_LOG_D("\n");
	lb_obj_t        *p_obj = (lb_obj_t *)param;
	lb_img_t	*p_img_prop = NULL;

	if (NULL == param) {
		APP_LOG_E("err Invalid parameters!\n");
		return LB_ERROR_NO_MEM;
	}

	p_img_prop = (lb_img_t *)p_obj->property;
	if (get_lock_status())
		strcpy(p_img_prop->src, p_img_prop->src_list[1]);
	else
		strcpy(p_img_prop->src, p_img_prop->src_list[0]);

	return 0;
}
#endif

#if 1
lb_int32 mic_status_init(void *param)
{
	APP_LOG_D("\n");
	lb_obj_t        *p_obj = (lb_obj_t *)param;
	lb_img_t	*p_img_prop = NULL;

	if (NULL == param) {
		APP_LOG_E("err Invalid parameters!\n");
		return LB_ERROR_NO_MEM;
	}

	p_img_prop = (lb_img_t *)p_obj->property;
	if (get_mute_enable())
		strcpy(p_img_prop->src, p_img_prop->src_list[1]);
	else
		strcpy(p_img_prop->src, p_img_prop->src_list[0]);

	return 0;
}
#endif

#if 1
lb_int32 light_value_init(void *param)
{
	APP_LOG_D("\n");
	lb_obj_t        *p_obj = (lb_obj_t *)param;
	lb_img_t	*p_img_prop = NULL;
    lb_int32 value = 0;
    lb_int32 idx = 0;
	if (NULL == param) {
		APP_LOG_E("err Invalid parameters!\n");
		return LB_ERROR_NO_MEM;
	}

	p_img_prop = (lb_img_t *)p_obj->property;
    value=get_backlight_value();
	idx=backlight_value_to_idx(value);
	strcpy(p_img_prop->src, p_img_prop->src_list[idx]);
	return 0;
}
#endif


/**
 * statusbar_week_init - statusbar week init
 * @param: lb_obj_t object pointer.
 *
 * This function use to init week label widget
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 week_init(void *param)
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
	sb_last_r.last_week = weekday.wd_index;

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
lb_int32 ytd_init(void *param)
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
lb_int32 time_init(void *param)
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

lb_int32 TFcard_status_init(void *param)
{
	APP_LOG_D("\n");
	lb_obj_t        *p_obj = (lb_obj_t *)param;
	lb_img_t	*p_img_prop = NULL;

	if (NULL == param) {
		APP_LOG_E("err Invalid parameters!\n");
		return LB_ERROR_NO_MEM;
	}

	p_img_prop = (lb_img_t *)p_obj->property;
	if (get_sd_status())
		strcpy(p_img_prop->src, p_img_prop->src_list[1]);
	else
		strcpy(p_img_prop->src, p_img_prop->src_list[0]);

	return 0;
}

/**
 * cdr_reg_init_exit_funcs -reg init and exit function for widgets
 *
 * This function use to register init function for widgets
 *
 * Returns SUCCESS if called when get success ; otherwise, return other custom values
 */
lb_int32 cdr_reg_init_exit_funcs(void)
{
	lb_int32		err = SUCCESS;

	err = lb_fmngr_reg_init_func("record_time_init", record_time_init);
	err |= lb_fmngr_reg_exit_func("record_time_exit", record_time_exit);
	err |= lb_fmngr_reg_init_func("resolution_init", resolution_init);
	err |= lb_fmngr_reg_exit_func("resolution_exit", resolution_exit);
	err |= lb_fmngr_reg_init_func("gps_status_init", gps_status_init);
	err |= lb_fmngr_reg_init_func("interval_record_status_init",
		interval_record_status_init);
	err |= lb_fmngr_reg_init_func("lock_imgbtn_init", lock_imgbtn_init);
	err |= lb_fmngr_reg_init_func("mic_imgbtn_init", mic_imgbtn_init);
	err |= lb_fmngr_reg_init_func("views_imgbtn_init", views_imgbtn_init);
	err |= lb_fmngr_reg_init_func("record_imgbtn_init", record_imgbtn_init);
	err |= lb_fmngr_reg_init_func("lock_record_status_init", lock_record_status_init);
	err |= lb_fmngr_reg_init_func("mic_status_init", mic_status_init);
	err |= lb_fmngr_reg_init_func("light_value_init", light_value_init);


	err |= lb_fmngr_reg_init_func("week_init",week_init);
	err |= lb_fmngr_reg_init_func("ytd_init",ytd_init);
	err |= lb_fmngr_reg_init_func("time_init",time_init);
	err |= lb_fmngr_reg_init_func("TFcard_init",TFcard_status_init);

	return err;
}

/**
 * cdr_unreg_init_exit_funcs - unreg init and exit function for widgets
 *
 * This function use to register response function for widgets
 *
 * Returns SUCCESS if called when get success ; otherwise, return other custom values
 */
lb_int32 cdr_unreg_init_exit_funcs(void)
{
	lb_int32		err = SUCCESS;

	err = lb_fmngr_unreg_init_func(views_imgbtn_init);
	err |= lb_fmngr_unreg_init_func(mic_imgbtn_init);
	err |= lb_fmngr_unreg_init_func(lock_imgbtn_init);
	err |= lb_fmngr_unreg_init_func(resolution_init);
	err |= lb_fmngr_unreg_exit_func(resolution_exit);
	err |= lb_fmngr_unreg_init_func(record_time_init);
	err |= lb_fmngr_unreg_exit_func(record_time_exit);
	err |= lb_fmngr_unreg_init_func(interval_record_status_init);
	err |= lb_fmngr_unreg_init_func(gps_status_init);
	err |= lb_fmngr_unreg_init_func(record_imgbtn_init);
	err |= lb_fmngr_unreg_init_func(lock_record_status_init);
	err |= lb_fmngr_unreg_init_func(mic_status_init);
	err |= lb_fmngr_unreg_init_func(light_value_init);
	err |= lb_fmngr_unreg_init_func(week_init);
	err |= lb_fmngr_unreg_init_func(ytd_init);
	err |= lb_fmngr_unreg_init_func(time_init);
	err |= lb_fmngr_unreg_init_func(TFcard_status_init);


	return err;
}

/**
 * cdr_reg_resp_funcs - unreg init function for widgets
 *
 * This function use to unregister init function for widgets
 *
 * Returns SUCCESS if called when get success ; otherwise, return other custom values
 */
lb_int32 cdr_reg_resp_msg_funcs(void)
{
	lb_int32		err = SUCCESS;

	err = lb_reg_resp_msg_func(LB_MSG_RECORDER, imgbtn_recorder);
	err |= lb_reg_resp_msg_func(LB_MSG_CAMERA, imgbtn_camera);
	err |= lb_reg_resp_msg_func(LB_MSG_LOCK, imgbtn_lock);
	err |= lb_reg_resp_msg_func(LB_MSG_MIC, imgbtn_mic);
	err |= lb_reg_resp_msg_func(LB_MSG_SETTING_MODE, imgbtn_setting_mode);
	err |= lb_reg_resp_msg_func(LB_MSG_VIEW, imgbtn_views);
	err |= lb_reg_resp_msg_func(LB_MSG_DIALOG, dialog_show);
	err |= lb_reg_resp_msg_func(LB_MSG_RECORDER_CMD_REFRESH, cmd_refresh);
	err |= lb_reg_resp_msg_func(LB_MSG_FILE_MODE, imgbtn_file_mode);
	err |= lb_reg_resp_msg_func(LB_MSG_LIGHT_ABATE, imgbtn_light_abate);
	err |= lb_reg_resp_msg_func(LB_MSG_LIGHT_ADD, imgbtn_light_add);
   // err |= lb_reg_resp_msg_func(LB_MSG_SMART_DRIVE_MODE, imgbtn_smart_drive_mode);//bsd_btn
	return err;
}

/**
 * cdr_unreg_resp_funcs - unreg response function for widgets
 *
 * This function use to unregister  response function for widgets
 *
 * Returns SUCCESS if called when get success ; otherwise, return other custom values
 */
lb_int32 cdr_unreg_resp_msg_funcs(void)
{
	lb_int32		err = SUCCESS;

	err = lb_unreg_resp_msg_func(dialog_show);
	err |= lb_unreg_resp_msg_func(imgbtn_views);
	err |= lb_unreg_resp_msg_func(imgbtn_mic);
	err |= lb_unreg_resp_msg_func(imgbtn_setting_mode);
	err |= lb_unreg_resp_msg_func(imgbtn_lock);
	err |= lb_unreg_resp_msg_func(imgbtn_camera);
	err |= lb_unreg_resp_msg_func(imgbtn_recorder);
	err |= lb_unreg_resp_msg_func(cmd_refresh);
	err |= lb_unreg_resp_msg_func(imgbtn_file_mode);
	err |= lb_unreg_resp_msg_func(imgbtn_light_abate);
	err |= lb_unreg_resp_msg_func(imgbtn_light_add);
	// err |= lb_unreg_resp_msg_func(imgbtn_smart_drive_mode);//bsd_btn
	

	return err;
}
lb_int32 cdr_reg_resp_sysmsg_funcs(void)
{
	lb_int32		err = SUCCESS;

	err = lb_reg_resp_sysmsg_func(LB_SYSTEM_MSG, sys_msg_manage);

	return err;
}

lb_int32 cdr_unreg_resp_sysmsg_func(void)
{
	lb_int32		err = SUCCESS;

	err = lb_unreg_resp_sysmsg_func(sys_msg_manage);

	return err;
}

/**
 * adas_disp_lanes_coordinate_trans -Conversion between screen coordinates and video image coordinates
 * in_points :the buffer pointer of image coordinates
 * out_points :the buffer pointer of screen coordinates
 * num_points: number of coordinate needing to convert
 *
 * This function use to Conversion between screen coordinates and video image coordinates
 *
 * return NULL
 */
static void adas_disp_lanes_coordinate_trans(IPOINTC *in_points,
	IPOINTC *out_points,
	int num_points)
{
	int cnt = 0;
	/*int tmp_y = 0;*/

	for (cnt = 0; cnt < num_points; cnt++) {
		out_points[cnt].x =
			in_points[cnt].x * cdr_screen.width / FRONT_PREVIEW_WIDTH;
		out_points[cnt].y =
			in_points[cnt].y * cdr_screen.width / FRONT_PREVIEW_WIDTH;
	}
}

static void disp_get_object(void)
{
	int i = 0;
#ifndef BLOCK_LINKER_DRAW
	for (i = 0; i < CR_CAR_MAX; i++) {
		if (NULL == car_frame_w[i])
			lb_view_get_obj_ext_by_id(BTN_BASE_ID + i, &car_frame_w[i]);

		if (NULL == car_dis_w[i]) {
			lb_btn_t *pproperty = NULL;
			lb_view_get_obj_property_by_ext(car_frame_w[i],
				(void **)&pproperty);
			car_dis_w[i] = pproperty->p_label;
		}
	}
	for (i = 0; i < CR_LINE_MAX; i++) {
		if (NULL == line[i])
			lb_view_get_obj_ext_by_id(LINE_BASE_ID + i, &line[i]);
	}
#else
	if (bsd_get_enable() == ENABLE_ON) {
		for (i = 0; i < CR_CAR_MAX; i++) {
			if (NULL == car_frame_w[i])
				lb_view_get_obj_ext_by_id(BTN_BASE_ID + i,
				&car_frame_w[i]);

			if (NULL == car_dis_w[i]) {
				lb_btn_t *pproperty = NULL;
				lb_view_get_obj_property_by_ext(car_frame_w[i],
					(void **)&pproperty);
				car_dis_w[i] = pproperty->p_label;
			}
		}
		for (i = 0; i < CR_LINE_MAX; i++) {
			if (NULL == line[i])
				lb_view_get_obj_ext_by_id(LINE_BASE_ID + i, &line[i]);
		}
	}
#endif

//#ifndef BLOCK_LINKER_DRAW
#if 1

	if (NULL == left_war_img)
		lb_view_get_obj_ext_by_id(LEFT_WAR_IMG_ID, &left_war_img);

	if (NULL == right_war_img)
		lb_view_get_obj_ext_by_id(RIGHT_WAR_IMG_ID, &right_war_img);
#endif

#ifdef GPS_DEBUG
	if (NULL == gps_debug_w)
		lb_view_get_obj_ext_by_id(GPS_DEBUG_ID, &gps_debug_w);
#endif
}

static void adas_disp_show_line(void)
{
	memset(&last_line_points[0], 0, sizeof(lb_line_points_t));
	memset(&last_line_points[1], 0, sizeof(lb_line_points_t));
#ifndef BLOCK_LINKER_DRAW
	int i = 0;

	for (i = 0; i < CR_LINE_MAX; i++) {
		if (line[i])
			lb_gal_set_obj_hidden(line[i], false);
	}
#endif
}

static void adas_disp_hidden_object(void)
{
	int i = 0;
#ifdef BLOCK_LINKER_DRAW
	car_draw_info_t	tmp_draw_info;

	if (adas_get_enable()) {
		tmp_draw_info.b_draw = BKL_DRAW_RECT;
		tmp_draw_info.buff_rect.x1 = 0;
		tmp_draw_info.buff_rect.y1 = 0;
		tmp_draw_info.buff_rect.x2 = 1;
		tmp_draw_info.buff_rect.y2 = 1;
		for (i = 0; i < ADAS_BKL_NUM_MAX; i++) {
			tmp_draw_info.idx = i;
			cdr_draw_msg_send(tmp_draw_info);
		}
	}
#else
	for (i = 0; i < CR_CAR_MAX; i++) {
		if (car_frame_w[i])
			lb_gal_set_obj_hidden(car_frame_w[i], true);
	}
	for (i = 0; i < CR_LINE_MAX; i++) {
		if (line[i])
			lb_gal_set_obj_hidden(line[i], true);
	}
#endif
#ifdef GPS_DEBUG
	if (gps_debug_w)
		lb_gal_set_obj_hidden(gps_debug_w, true);
#endif
}
static void bsd_disp_show_line(void)
{
#ifdef REAR_BSD_DEBUG
	if (line[1])
		lb_gal_set_obj_hidden(line[1], false);
	if (line[0])
		lb_gal_set_obj_hidden(line[0], false);
#endif
}

void bsd_disp_hidden_object(void)
{
	int i = 0;

#ifdef REAR_BSD_DEBUG
	for (i = 0; i < CR_CAR_MAX; i++) {
		if (car_frame_w[i])
			lb_gal_set_obj_hidden(car_frame_w[i], true);
	}
	if (line[1])
		lb_gal_set_obj_hidden(line[1], true);
	if (line[0])
		lb_gal_set_obj_hidden(line[0], true);
#endif
#ifdef GPS_DEBUG
	if (gps_debug_w)
		lb_gal_set_obj_hidden(gps_debug_w, true);
#endif
#ifdef BLOCK_LINKER_DRAW

	car_draw_info_t	tmp_draw_info;

	if (bsd_get_enable() && last_bsd_car_num) {
		last_bsd_car_num = 0;
		tmp_draw_info.b_draw = BKL_DRAW_BSD_LINE;
		tmp_draw_info.buff_rect.x1 = 0;
		tmp_draw_info.buff_rect.y1 = 0;
		tmp_draw_info.buff_rect.x2 = 1;
		tmp_draw_info.buff_rect.y2 = 1;
		for (i = 0; i < CR_CAR_MAX; i++) {
			tmp_draw_info.idx = i;
			cdr_draw_msg_send(tmp_draw_info);
		}
	}
#else
	if (!lb_gal_get_obj_hidden(left_war_img))
		lb_gal_set_obj_hidden(left_war_img, true);
	if (!lb_gal_get_obj_hidden(right_war_img))
		lb_gal_set_obj_hidden(right_war_img, true);
#endif
}
int line_need_update(lb_line_points_t line, lb_line_points_t last_line, int num_point)
{
	int i;
	int ret = 0;

	for (i = 0; i < num_point; i++) {
		if (abs(line.points[i].x - last_line.points[i].x) > CDR_LINE_UPDATE_LIMIT
			|| abs(line.points[i].y - last_line.points[i].y) >
			CDR_LINE_UPDATE_LIMIT) {
			ret = 1;
			break;
		}
	}

	return ret;
}
int line_save(lb_line_points_t line, lb_line_points_t *last_line)
{
	int i;
	int ret = 0;

	for (i = 0; i < line.num; i++) {
		last_line->points[i].x = line.points[i].x;
		last_line->points[i].y = line.points[i].y;
	}

	return ret;
}
int bsd_line_trans(lb_line_points_t *line_new, IPOINTC *Area, int num_point)
{
	int i;
	int ret = 0;

	for (i = 0; i < num_point-1; i++) {
		line_new->points[i].x = Area[i].x *  cdr_screen.width /
			REAR_PREVIEW_WIDTH;
#ifdef SCREEN_ROT_90
		line_new->points[i].y = (Area[i].y - (cdr_screen.rear_max_crop_x -
		rear_preview_para.crop.x)) * cdr_screen.width / REAR_PREVIEW_WIDTH;
#else
		line_new->points[i].y = (Area[i].y - rear_preview_para.crop.y) *
		cdr_screen.width / REAR_PREVIEW_WIDTH;
#endif
	}
	line_new->points[num_point-1].x = line_new->points[0].x;
	line_new->points[num_point-1].y = line_new->points[0].y;

	return ret;
}
/**
 * adas_cars_alarm_tone -alarm for adas car status
 * p_det_info :the buffer pointer of car's or ped's parameter
 *
 * This function use to alarm for adas car status
 *
 * return NULL
 */
static void adas_alarm_cars_tone(HDFrameGetData *p_det_info)
{

	if (alarm_distance_interval <= ALARM_INTERVAL_COUNT)
		alarm_distance_interval++;
	if (p_det_info->cars.num) {
		/* APP_LOG_D("p_det_info->cars.warmCar.warnStatus:%d\n",
			p_det_info->cars.warmCar.warnStatus); */
		if ((p_det_info->cars.warmCar.warnStatus == ALARM_COLLIDE)
			&& p_det_info->cars.warmCar.warnStatus !=
				last_alarm_dist_status && alarm_distance_interval >
				ALARM_INTERVAL_COUNT) {
			APP_LOG_W("p_det_info->cars.warmCar.warnStatus:%d %d\n",
				p_det_info->cars.warmCar.warnStatus,
				last_alarm_dist_status);
			lb_system_mq_send(LB_SYSMSG_ALARM_COLLIDE,
				&p_det_info->cars.warmCar.warnStatus, sizeof(int), 0);
				alarm_distance_interval = 0;
		} else if ((p_det_info->cars.warmCar.warnStatus == KEEP_DISTANCE)
			&& p_det_info->cars.warmCar.warnStatus !=
				last_alarm_dist_status && alarm_distance_interval >
				ALARM_INTERVAL_COUNT) {
			APP_LOG_W("p_det_info->cars.warmCar.warnStatus:%d %d\n",
				p_det_info->cars.warmCar.warnStatus,
				last_alarm_dist_status);
			lb_system_mq_send(LB_SYSMSG_ALARM_DISTANCE,
				&p_det_info->cars.warmCar.warnStatus, sizeof(int), 0);
				alarm_distance_interval = 0;
		} else if ((p_det_info->cars.warmCar.warnStatus == ALARM_LAUNCH) &&
			p_det_info->cars.warmCar.warnStatus != last_alarm_dist_status) {
			APP_LOG_W("p_det_info->cars.warmCar.warnStatus:%d %d\n",
				p_det_info->cars.warmCar.warnStatus,
				last_alarm_dist_status);
			lb_system_mq_send(LB_SYSMSG_ALARM_LAUNCH,
				&p_det_info->cars.warmCar.warnStatus, sizeof(int), 0);
		}
		last_alarm_dist_status = p_det_info->cars.warmCar.warnStatus;
		/* APP_LOG_D("p_det_info->cars.num:%d\n", p_det_info->cars.num); */


	} else
		last_alarm_dist_status = 0;
}


/**
 * adas_disp_mark_cars -drawing car's or ped's tracing box  in a frame vedio data
 * p_det_info :the buffer pointer of car's or ped's parameter
 *
 * This function use to drawing car's or ped's tracing box  in a frame vedio data
 *
 * return NULL
 */
static void adas_disp_mark_cars(HDFrameGetData *p_det_info)
{
	u8 cnt_cars = 0;
	int x_screen = 0;
	int y_screen = 0;
	int w_screen = 0;
	int h_screen = 0;
	float distance = 0.0;
	int i = 0;

	if (p_det_info->cars.num > CR_CAR_MAX)
		p_det_info->cars.num = CR_CAR_MAX;

#ifdef BLOCK_LINKER_DRAW
	car_draw_info_t	tmp_draw_info;

	/* draw new rectangle for cars */
	if (last_car_num > p_det_info->cars.num) {
		for (i = p_det_info->cars.num; i < last_car_num; i++) {
			tmp_draw_info.b_draw = BKL_DRAW_RECT;
			tmp_draw_info.buff_rect.x1 = 0;
			tmp_draw_info.buff_rect.y1 = 0;
			tmp_draw_info.buff_rect.x2 = 1;
			tmp_draw_info.buff_rect.y2 = 1;
			tmp_draw_info.idx = ADAS_BKL_LINE_NUM_MAX + i;
			cdr_draw_msg_send(tmp_draw_info);
		}
	}
#else
	int hidden = 0;
	static char dist[CR_CAR_MAX][CR_TXT_MAX];
	static lb_point_t data_pos[CR_CAR_MAX];
	static lb_size_t data_size[CR_CAR_MAX];

	/* draw new rectangle for cars */
	if (last_car_num > p_det_info->cars.num) {
		for (i = p_det_info->cars.num; i < last_car_num; i++)
			lb_gal_set_obj_hidden(car_frame_w[i], true);
	}
#endif
	last_car_num = p_det_info->cars.num;

	for (cnt_cars = 0; cnt_cars < p_det_info->cars.num; cnt_cars++) {
		/*x_screen = p_det_info->cars.p[cnt_cars].object.loc.x;*/
		x_screen = p_det_info->cars.p[cnt_cars].object.loc.y -
			(/*cdr_screen.front_max_crop_x - */front_preview_para.crop.x);
		x_screen = x_screen * cdr_screen.width / FRONT_PREVIEW_WIDTH;
		if (x_screen < 0)
			x_screen = 0;
		
#ifdef SCREEN_ROT_90
		y_screen = p_det_info->cars.p[cnt_cars].object.loc.x;
#else
		y_screen = p_det_info->cars.p[cnt_cars].object.loc.y -
		front_preview_para.crop.y;
#endif
		y_screen = cdr_screen.width - y_screen * cdr_screen.width / FRONT_PREVIEW_WIDTH;

		/*w_screen = p_det_info->cars.p[cnt_cars].object.loc.width;*/
		w_screen = p_det_info->cars.p[cnt_cars].object.loc.height;
		w_screen = w_screen * cdr_screen.width / FRONT_PREVIEW_WIDTH;
		if (w_screen < 44)
			w_screen = 44;

		/*h_screen = p_det_info->cars.p[cnt_cars].object.loc.height;*/
		h_screen = p_det_info->cars.p[cnt_cars].object.loc.width;
		h_screen = h_screen * cdr_screen.width / FRONT_PREVIEW_WIDTH;
#if 0
		if (h_screen < 32)
			h_screen = 32;
		else if (h_screen > 220)
			h_screen = 220;
#endif
		if ((y_screen + h_screen) > cdr_screen.width)
			y_screen = cdr_screen.width - h_screen;
		if ((x_screen + w_screen) > cdr_screen.height)
			x_screen = cdr_screen.height - w_screen;
		if ((y_screen + h_screen/2) <= 0) {
#ifdef BLOCK_LINKER_DRAW
			tmp_draw_info.b_draw = BKL_DRAW_RECT;
			tmp_draw_info.buff_rect.x1 = 0;
			tmp_draw_info.buff_rect.y1 = 0;
			tmp_draw_info.buff_rect.x2 = 1;
			tmp_draw_info.buff_rect.y2 = 1;
			tmp_draw_info.idx = ADAS_BKL_LINE_NUM_MAX + cnt_cars;
			cdr_draw_msg_send(tmp_draw_info);
#else
			if (!hidden)
				lb_gal_set_obj_hidden(car_frame_w[cnt_cars], true);
#endif
			continue;
		}

		if (y_screen < 0)
			y_screen = 0;

		distance = p_det_info->cars.p[cnt_cars].object.dist;

#ifdef BLOCK_LINKER_DRAW
		if (distance < 10.0)
			tmp_draw_info.color = 0;
		else
			tmp_draw_info.color = 1;

		if (abs(last_data_pos[cnt_cars].x - x_screen) > CDR_CAR_UPDATE_LIMIT
		|| abs(last_data_pos[cnt_cars].y - y_screen) > CDR_CAR_UPDATE_LIMIT
		|| abs(last_data_size[cnt_cars].w - w_screen) > CDR_CAR_UPDATE_LIMIT
		|| abs(last_data_size[cnt_cars].h - h_screen) > CDR_CAR_UPDATE_LIMIT
		|| fabs(last_distance[cnt_cars] - distance) > 1.0) {
			tmp_draw_info.buff_rect.x1 = x_screen;
			tmp_draw_info.buff_rect.y1 = y_screen - h_screen;
			tmp_draw_info.buff_rect.x2 = tmp_draw_info.buff_rect.x1
						+ w_screen;
			tmp_draw_info.buff_rect.y2 = y_screen;
			tmp_draw_info.idx = ADAS_BKL_LINE_NUM_MAX + cnt_cars;
			tmp_draw_info.meters = (int32_t)distance;
			tmp_draw_info.b_draw = BKL_DRAW_RECT;
			cdr_draw_msg_send(tmp_draw_info);
			last_data_pos[cnt_cars].x = x_screen;
			last_data_pos[cnt_cars].y = y_screen;
			last_data_size[cnt_cars].w = w_screen;
			last_data_size[cnt_cars].h = h_screen;
			last_distance[cnt_cars] = distance;
		}
#else
		RT_ASSERT(car_frame_w[cnt_cars]);
		hidden = lb_gal_get_obj_hidden(car_frame_w[cnt_cars]);
		if (abs(last_data_pos[cnt_cars].x - x_screen) > CDR_CAR_UPDATE_LIMIT ||
			abs(last_data_pos[cnt_cars].y - y_screen) >
			CDR_CAR_UPDATE_LIMIT) {
			data_pos[cnt_cars].x = x_screen;
			data_pos[cnt_cars].y = y_screen;
			lb_gal_update_btn(car_frame_w[cnt_cars], LB_BTN_UPD_POS,
				&data_pos[cnt_cars]);
			last_data_pos[cnt_cars].x = x_screen;
			last_data_pos[cnt_cars].y = y_screen;
		}

		if (abs(last_data_size[cnt_cars].w - w_screen) > CDR_CAR_UPDATE_LIMIT ||
			abs(last_data_size[cnt_cars].h - h_screen) >
			CDR_CAR_UPDATE_LIMIT) {
			data_size[cnt_cars].w = w_screen;
			data_size[cnt_cars].h = h_screen;
			lb_gal_update_btn(car_frame_w[cnt_cars], LB_BTN_UPD_SIZE,
				&data_size[cnt_cars]);
			last_data_size[cnt_cars].w = w_screen;
			last_data_size[cnt_cars].h = h_screen;
		}

		if (last_distance[cnt_cars] - distance > 1.0 ||
			last_distance[cnt_cars] - distance < -1.0) {
			memset(dist[cnt_cars], 0x00, CR_TXT_MAX);
			sprintf(dist[cnt_cars], "%0.0fm", distance);
			lb_gal_update_btn(car_dis_w[cnt_cars], LB_BTN_UPD_TXT,
				dist[cnt_cars]);
			last_distance[cnt_cars] = distance;
		}
		if (hidden == 1)
			lb_gal_set_obj_hidden(car_frame_w[cnt_cars], false);
#endif
	}
}

static void bsd_disp_mark_cars(HDFrameGetData *p_det_info)
{
	u8 cnt_cars = 0;
	int x_screen = 0;
	int y_screen = 0;
	int w_screen = 0;
	int h_screen = 0;
	float distance = 0.0;
	int i = 0;
	int flag = 0;
	if (p_det_info->cars.num > CR_CAR_MAX)
		p_det_info->cars.num = CR_CAR_MAX;

#ifdef BLOCK_LINKER_DRAW
	car_draw_info_t	tmp_draw_info;
	memset(&tmp_draw_info, 0x00, sizeof(car_draw_info_t));
	/* draw new rectangle for cars */
	if (last_bsd_car_num > p_det_info->cars.num) {
		for (i = p_det_info->cars.num; i < last_bsd_car_num; i++) {
			tmp_draw_info.b_draw = BKL_DRAW_BSD_LINE;
			tmp_draw_info.buff_rect.x1 = 0;
			tmp_draw_info.buff_rect.y1 = 0;
			tmp_draw_info.buff_rect.x2 = 1;
			tmp_draw_info.buff_rect.y2 = 1;
			tmp_draw_info.idx = i;
			cdr_draw_msg_send(tmp_draw_info);
		}
	}
#else
	int hidden = 0;
	static char dist[CR_CAR_MAX][CR_TXT_MAX];
	static lb_point_t data_pos[CR_CAR_MAX];
	static lb_size_t data_size[CR_CAR_MAX];

	/* draw new rectangle for cars */
	if (last_bsd_car_num > p_det_info->cars.num) {
		for (i = p_det_info->cars.num; i < last_bsd_car_num; i++)
			lb_gal_set_obj_hidden(car_frame_w_bsd[i], true);
	}
#endif
	last_bsd_car_num = p_det_info->cars.num;
	for (cnt_cars = 0; cnt_cars < p_det_info->cars.num; cnt_cars++) {
		/*x_screen = p_det_info->cars.p[cnt_cars].object.loc.x;*/
		x_screen = p_det_info->cars.p[cnt_cars].object.loc.y -
			(rear_preview_para.crop.x - 1080);
		x_screen = x_screen * cdr_screen.width / FRONT_PREVIEW_WIDTH;
		if ((x_screen < 0) && ((x_screen + cdr_screen.height) <= 0)) {
			tmp_draw_info.b_draw = BKL_DRAW_BSD_LINE;
			tmp_draw_info.buff_rect.x1 = 0;
			tmp_draw_info.buff_rect.y1 = 0;
			tmp_draw_info.buff_rect.x2 = 1;
			tmp_draw_info.buff_rect.y2 = 1;
			tmp_draw_info.idx = cnt_cars;
			cdr_draw_msg_send(tmp_draw_info);
			continue;
		}

		

		y_screen = p_det_info->cars.p[cnt_cars].object.loc.x;
		y_screen = cdr_screen.width - y_screen * cdr_screen.width / FRONT_PREVIEW_WIDTH;

		/*w_screen = p_det_info->cars.p[cnt_cars].object.loc.width;*/
		w_screen = p_det_info->cars.p[cnt_cars].object.loc.height;
		w_screen = w_screen * cdr_screen.width / FRONT_PREVIEW_WIDTH;
		if (x_screen < 0) {
			w_screen = (x_screen + w_screen);
			x_screen = 0;
		}
		if (w_screen < 15) {
			tmp_draw_info.b_draw = BKL_DRAW_BSD_LINE;
			tmp_draw_info.buff_rect.x1 = 0;
			tmp_draw_info.buff_rect.y1 = 0;
			tmp_draw_info.buff_rect.x2 = 1;
			tmp_draw_info.buff_rect.y2 = 1;
			tmp_draw_info.idx = cnt_cars;
			cdr_draw_msg_send(tmp_draw_info);
			continue;
		}


		/*h_screen = p_det_info->cars.p[cnt_cars].object.loc.height;*/
		h_screen = p_det_info->cars.p[cnt_cars].object.loc.width;
		h_screen = h_screen * cdr_screen.width / FRONT_PREVIEW_WIDTH;
		if ((y_screen >= cdr_screen.width) || (x_screen >= cdr_screen.height)) {
			tmp_draw_info.b_draw = BKL_DRAW_BSD_LINE;
			tmp_draw_info.buff_rect.x1 = 0;
			tmp_draw_info.buff_rect.y1 = 0;
			tmp_draw_info.buff_rect.x2 = 1;
			tmp_draw_info.buff_rect.y2 = 1;
			tmp_draw_info.idx = cnt_cars;
			cdr_draw_msg_send(tmp_draw_info);
			continue;
		}
		if ((y_screen + h_screen) > cdr_screen.width)
			h_screen = cdr_screen.width - y_screen;
		if ((x_screen + w_screen) > cdr_screen.height)
			w_screen = cdr_screen.height - x_screen;
		if ((y_screen + h_screen/2) <= 0 || h_screen < 40 || w_screen < 40) {
#ifdef BLOCK_LINKER_DRAW
			tmp_draw_info.b_draw = BKL_DRAW_BSD_LINE;
			tmp_draw_info.buff_rect.x1 = 0;
			tmp_draw_info.buff_rect.y1 = 0;
			tmp_draw_info.buff_rect.x2 = 1;
			tmp_draw_info.buff_rect.y2 = 1;
			tmp_draw_info.idx = cnt_cars;
			cdr_draw_msg_send(tmp_draw_info);
#else
			if (!hidden)
				lb_gal_set_obj_hidden(car_frame_w_bsd[cnt_cars], true);
#endif
			continue;
		}

		if (y_screen < 0)
			y_screen = 0;

		distance = p_det_info->cars.p[cnt_cars].object.dist;

#ifdef BLOCK_LINKER_DRAW
		if (abs(last_data_pos_bsd[cnt_cars].x - x_screen) > CDR_CAR_UPDATE_LIMIT
		|| abs(last_data_pos_bsd[cnt_cars].y - y_screen) > CDR_CAR_UPDATE_LIMIT
		|| abs(last_data_size_bsd[cnt_cars].w - w_screen) > CDR_CAR_UPDATE_LIMIT
		|| abs(last_data_size_bsd[cnt_cars].h - h_screen) > CDR_CAR_UPDATE_LIMIT
		|| fabs(last_distance_bsd[cnt_cars] - distance) > 1.0) {
			tmp_draw_info.buff_rect.x1 = x_screen;
			tmp_draw_info.buff_rect.y1 = y_screen - h_screen;
			tmp_draw_info.buff_rect.x2 = tmp_draw_info.buff_rect.x1
						+ w_screen;
			tmp_draw_info.buff_rect.y2 = y_screen;
			tmp_draw_info.idx = cnt_cars;
			tmp_draw_info.meters = (int32_t)distance;
			tmp_draw_info.b_draw = BKL_DRAW_BSD_LINE;
			tmp_draw_info.en_fill = 0;
			if (distance < 10.0) {
				tmp_draw_info.border_color = 0xFFFF0000;
				tmp_draw_info.meters_color = 0xFFFF0000;
			} else {
				tmp_draw_info.border_color = 0xFF00ff00;
				tmp_draw_info.meters_color = 0xFF00ff00;
			}
			tmp_draw_info.border_width = 2;
			tmp_draw_info.line_points[0].x = 0;
			tmp_draw_info.line_points[0].y = 0;
			tmp_draw_info.line_points[1].x = w_screen;
			tmp_draw_info.line_points[1].y = 0;
			tmp_draw_info.line_points[2].x = w_screen;
			tmp_draw_info.line_points[2].y = h_screen;
			tmp_draw_info.line_points[3].x = 0;
			tmp_draw_info.line_points[3].y = h_screen;
			cdr_draw_msg_send(tmp_draw_info);
			last_data_pos_bsd[cnt_cars].x = x_screen;
			last_data_pos_bsd[cnt_cars].y = y_screen;
			last_data_size_bsd[cnt_cars].w = w_screen;
			last_data_size_bsd[cnt_cars].h = h_screen;
			last_distance_bsd[cnt_cars] = distance;
			/*break;*/
		}
#else
		RT_ASSERT(car_frame_w_bsd[cnt_cars]);
		hidden = lb_gal_get_obj_hidden(car_frame_w_bsd[cnt_cars]);
		if (abs(last_data_pos_bsd[cnt_cars].x - x_screen) > CDR_CAR_UPDATE_LIMIT ||
			abs(last_data_pos_bsd[cnt_cars].y - y_screen) >
			CDR_CAR_UPDATE_LIMIT) {
			data_pos[cnt_cars].x = x_screen;
			data_pos[cnt_cars].y = y_screen;
			lb_gal_update_btn(car_frame_w_bsd[cnt_cars], LB_BTN_UPD_POS,
				&data_pos[cnt_cars]);
			last_data_pos_bsd[cnt_cars].x = x_screen;
			last_data_pos_bsd[cnt_cars].y = y_screen;
		}

		if (abs(last_data_size_bsd[cnt_cars].w - w_screen) > CDR_CAR_UPDATE_LIMIT ||
			abs(last_data_size_bsd[cnt_cars].h - h_screen) >
			CDR_CAR_UPDATE_LIMIT) {
			data_size[cnt_cars].w = w_screen;
			data_size[cnt_cars].h = h_screen;
			lb_gal_update_btn(car_frame_w_bsd[cnt_cars], LB_BTN_UPD_SIZE,
				&data_size[cnt_cars]);
			last_data_size_bsd[cnt_cars].w = w_screen;
			last_data_size_bsd[cnt_cars].h = h_screen;
		}

		if (last_distance_bsd[cnt_cars] - distance > 1.0 ||
			last_distance_bsd[cnt_cars] - distance < -1.0) {
			memset(dist[cnt_cars], 0x00, CR_TXT_MAX);
			sprintf(dist[cnt_cars], "%0.0fm", distance);
			lb_gal_update_btn(car_dis_w_bsd[cnt_cars], LB_BTN_UPD_TXT,
				dist[cnt_cars]);
			last_distance[cnt_cars] = distance;
		}
		if (hidden == 1)
			lb_gal_set_obj_hidden(car_frame_w_bsd[cnt_cars], false);
#endif
	}
}

/**
 * adas_lanes_alarm_tone -alarm for adas lane status
 * p_lanes_info :the buffer pointer of lane line parameter
 *
 * This function use to alarm for adas lane status
 *
 * return NULL
 */

static void adas_alarm_lanes_tone(HDLanes *p_lanes_info)
{
#ifdef LOMBO_GPS
	if (gps_ret != 0 || gps_data.speed > 30) {
#else
	if (1) {
#endif
		if ((p_lanes_info->status == 2 && last_alarm_pressure_status !=
			-2 && p_lanes_info->status != last_alarm_pressure_status) ||
			(p_lanes_info->status == -2 && last_alarm_pressure_status !=
			2 && p_lanes_info->status != last_alarm_pressure_status)) {
			APP_LOG_W("p_lanes_info->status:%d %d\n",
				p_lanes_info->status, last_alarm_pressure_status);
			lb_system_mq_send(LB_SYSMSG_ALARM_PRESSURE,
				&p_lanes_info->status, sizeof(int), 0);
		}
		last_alarm_pressure_status = p_lanes_info->status;
	}
}

#ifdef BLOCK_LINKER_DRAW
/**
 * adas_disp_mark_lanes -drawing lane line  in a frame vedio data
 * p_lanes_info :the buffer pointer of lane line parameter
 *
 * This function use to drawing lane line  in a frame vedio data
 *
 * return NULL
 */
static void adas_disp_mark_lanes(HDLanes *p_lanes_info)
{
	IPOINTC tmp_points_one[16];
	IPOINTC tmp_points_two[16];
	static lb_line_points_t line_points[CR_LINE_MAX];
	int i;
	int left, top, right, bottom;
	int w, h;
	car_draw_info_t	tmp_draw_info;
	lb_rect_t		rect[CR_LINE_MAX];

	adas_disp_lanes_coordinate_trans(p_lanes_info->lt.pOfLane, tmp_points_one,
		LANE_POINT_NUM);
	adas_disp_lanes_coordinate_trans(p_lanes_info->rt.pOfLane, tmp_points_two,
		LANE_POINT_NUM);
	for (i = 0; i < 2; i++) {
		line_points[i].num = 2;
		if (i == 0) {
			line_points[i].points[0].x = tmp_points_one[0].y -
				(/*cdr_screen.front_max_crop_x -*/
				front_preview_para.crop.x) * cdr_screen.width /
				FRONT_PREVIEW_WIDTH;
			line_points[i].points[1].x = tmp_points_one[5].y -
				(/*cdr_screen.front_max_crop_x -*/
				front_preview_para.crop.x) * cdr_screen.width /
				FRONT_PREVIEW_WIDTH;
			line_points[i].points[0].y = cdr_screen.width - tmp_points_one[0].x;
			line_points[i].points[1].y = cdr_screen.width - tmp_points_one[5].x;
		} else if (i == 1) {
			line_points[i].points[0].x = tmp_points_two[0].y -
				(/*cdr_screen.front_max_crop_x -*/
				front_preview_para.crop.x) * cdr_screen.width /
				FRONT_PREVIEW_WIDTH;
			line_points[i].points[1].x = tmp_points_two[5].y -
				(/*cdr_screen.front_max_crop_x -*/
				front_preview_para.crop.x) * cdr_screen.width /
				FRONT_PREVIEW_WIDTH;
			line_points[i].points[0].y = cdr_screen.width - tmp_points_two[0].x;
			line_points[i].points[1].y = cdr_screen.width - tmp_points_two[5].x;
		}

		if (line_points[i].points[0].y < 0)
			line_points[i].points[0].y = 0;
		if (line_points[i].points[1].y < 0)
			line_points[i].points[1].y = 0;

		left = line_points[i].points[0].x > line_points[i].points[1].x ?
			line_points[i].points[1].x : line_points[i].points[0].x;
		top = line_points[i].points[0].y > line_points[i].points[1].y ?
			line_points[i].points[1].y : line_points[i].points[0].y;
		right = line_points[i].points[0].x > line_points[i].points[1].x ?
			line_points[i].points[0].x : line_points[i].points[1].x;
		bottom = line_points[i].points[0].y > line_points[i].points[1].y ?
			line_points[i].points[0].y : line_points[i].points[1].y;
		w = right - left;
		h = bottom - top;

		rect[i].x1 = left;
		if (i == 0)
			rect[i].y1 = top;
		else if (i == 1)
			rect[i].y1 = bottom;
		rect[i].x2 = rect[i].x1 + w;
		if (i == 0)
			rect[i].y2 = rect[i].y1 + h;
		else if (i == 1)
			rect[i].y2 = rect[i].y1 - h;


		if (rect[i].x2 < 0  || rect[i].x1 > cdr_screen.height) {
			rect[i].x1 = 0;
			rect[i].y1 = 0;
			rect[i].x2 = 1;
			rect[i].y2 = 1;
		}
		if (h < 0 || w < 0) {
			APP_LOG_W("err h:%d, w:%d\n", h, w);
			rect[i].x1 = 0;
			rect[i].y1 = 0;
			rect[i].x2 = 1;
			rect[i].y2 = 1;
		} else if (h > ADAS_BKL_LINE_WIDTH_MAX || w > ADAS_BKL_LINE_HEIGHT_MAX) {
			APP_LOG_W("err h:%d, w:%d\n", h, w);
			rect[i].x1 = 0;
			rect[i].y1 = 0;
			rect[i].x2 = 1;
			rect[i].y2 = 1;
		}

	}

	if (line_need_update(line_points[0], last_line_points[0], 2) ||
		line_need_update(line_points[1], last_line_points[1], 2)) {
		tmp_draw_info.line_points[0].x = 0;
		tmp_draw_info.line_points[0].y = rect[1].y1 - rect[1].y2;
		tmp_draw_info.line_points[1].x = rect[1].x2 - rect[1].x1;
		tmp_draw_info.line_points[1].y = 0;
		tmp_draw_info.line_points[2].x = rect[0].x2 - rect[0].x1;
		tmp_draw_info.line_points[2].y = rect[0].y2 - rect[1].y2;
		tmp_draw_info.line_points[3].x = 0;
		tmp_draw_info.line_points[3].y = rect[0].y1 - rect[1].y2;


		tmp_draw_info.buff_rect.x1 = rect[1].x1;
		tmp_draw_info.buff_rect.y1 = rect[1].y2;
		if (rect[1].x1 == rect[1].x2)
			tmp_draw_info.buff_rect.x2 = rect[1].x1 + 1;
		else
			tmp_draw_info.buff_rect.x2 = rect[1].x2;
		if (rect[0].y2 == rect[1].y2)
			tmp_draw_info.buff_rect.y2 = rect[1].y2 + 1;
		else
			tmp_draw_info.buff_rect.y2 = rect[0].y2;

		if (h > ADAS_BKL_LINE_WIDTH_MAX || w > ADAS_BKL_LINE_HEIGHT_MAX) {
			APP_LOG_W("err h:%d, w:%d\n", h, w);
			tmp_draw_info.buff_rect.x1 = 0;
			tmp_draw_info.buff_rect.y1 = 0;
			tmp_draw_info.buff_rect.x2 = 1;
			tmp_draw_info.buff_rect.y2 = 1;
		}

		tmp_draw_info.idx = 0;
		tmp_draw_info.b_draw = BKL_DRAW_LINE;
		tmp_draw_info.en_fill = 1;
		tmp_draw_info.fill_color = 0x4cffff00;
		tmp_draw_info.border_width = 0;
		cdr_draw_msg_send(tmp_draw_info);
		line_save(line_points[0], &last_line_points[0]);
		line_save(line_points[1], &last_line_points[1]);
	}

	return;
}
#else
/**
 * adas_disp_mark_lanes -drawing lane line  in a frame vedio data
 * p_lanes_info :the buffer pointer of lane line parameter
 *
 * This function use to drawing lane line  in a frame vedio data
 *
 * return NULL
 */
static void adas_disp_mark_lanes(HDLanes *p_lanes_info)
{
	IPOINTC tmp_points_one[16];
	IPOINTC tmp_points_two[16];
	static lb_line_points_t line_points[CR_LINE_MAX];
	int i;

	adas_disp_lanes_coordinate_trans(p_lanes_info->lt.pOfLane, tmp_points_one,
		LANE_POINT_NUM);
	adas_disp_lanes_coordinate_trans(p_lanes_info->rt.pOfLane, tmp_points_two,
		LANE_POINT_NUM);

	for (i = 0; i < 2; i++) {
		RT_ASSERT(line[i]);

		line_points[i].num = 2;
		if (i == 0) {
			line_points[i].points[0].x = tmp_points_one[0].x;
			line_points[i].points[1].x = tmp_points_one[5].x;
#ifdef SCREEN_ROT_90
			line_points[i].points[0].y = tmp_points_one[0].y -
				(cdr_screen.front_max_crop_x -
				front_preview_para.crop.x) * cdr_screen.width /
				FRONT_PREVIEW_WIDTH;
			line_points[i].points[1].y = tmp_points_one[5].y -
				(cdr_screen.front_max_crop_x -
				front_preview_para.crop.x) * cdr_screen.width /
				FRONT_PREVIEW_WIDTH;
#else
			line_points[i].points[0].y = tmp_points_one[0].y -
			front_preview_para.crop.y * cdr_screen.width /
				FRONT_PREVIEW_WIDTH;
			line_points[i].points[1].y = tmp_points_one[5].y -
				front_preview_para.crop.y * cdr_screen.width /
				FRONT_PREVIEW_WIDTH;
#endif
		} else if (i == 1) {
			line_points[i].points[0].x = tmp_points_two[0].x;
			line_points[i].points[1].x = tmp_points_two[5].x;
#ifdef SCREEN_ROT_90
			line_points[i].points[0].y = tmp_points_two[0].y -
				(cdr_screen.front_max_crop_x -
				front_preview_para.crop.x) * cdr_screen.width /
				FRONT_PREVIEW_WIDTH;
			line_points[i].points[1].y = tmp_points_two[5].y -
				(cdr_screen.front_max_crop_x -
				front_preview_para.crop.x) * cdr_screen.width /
				FRONT_PREVIEW_WIDTH;
#else
			line_points[i].points[0].y = tmp_points_two[0].y -
			front_preview_para.crop.y * cdr_screen.width /
				FRONT_PREVIEW_WIDTH;
			line_points[i].points[1].y = tmp_points_two[5].y -
				front_preview_para.crop.y * cdr_screen.width /
				FRONT_PREVIEW_WIDTH;
#endif
		}
		if (line_points[i].points[0].y < 0)
			line_points[i].points[0].y = 0;
		if (line_points[i].points[1].y < 0)
			line_points[i].points[1].y = 0;
		if (line_need_update(line_points[i], last_line_points[i], 2) == 1) {
			int hidden;
			hidden = lb_gal_get_obj_hidden(line[i]);
			lb_gal_update_line(line[i], LB_LINE_UPD_POINTS,
				&line_points[i]);

			if (hidden == 1)
				lb_gal_set_obj_hidden(line[i], false);

			line_save(line_points[i], &last_line_points[i]);
		}
	}

	return;
}
#endif

#ifdef REAR_BSD_DEBUG
static void bsd_disp_mark_alarm_area(HDFrameGetBsd *p_det_info)
{
	int hidden;
	static lb_line_points_t line_points_l;
	static lb_line_points_t line_points_r;

	APP_LOG_D("ltWarn:%d, rtWarn:%d enable:%d\n",
				p_det_info->bsd.ltWarn, p_det_info->bsd.rtWarn,
				p_det_info->warnArea.enable);
	RT_ASSERT(line[0]);
	RT_ASSERT(line[1]);

	hidden = lb_gal_get_obj_hidden(line[0]);
	line_points_l.num = 5;
	bsd_line_trans(&line_points_l, p_det_info->warnArea.ltArea, 5);
	if (line_need_update(line_points_l, last_line_points[0], 4)) {
		lb_gal_update_line(line[0], LB_LINE_UPD_POINTS,
			&line_points_l);
		line_save(line_points_l, &last_line_points[0]);
	}
#ifdef LOMBO_GPS
	if (p_det_info->bsd.ltWarn && (gps_ret != 0 ||
		gps_data.speed >= get_bsd_alarm_speed_map())) {
#else
	if (p_det_info->bsd.ltWarn) {
#endif
		bsd_rect_left_color = 0xff0000;
		lb_gal_update_line(line[0], LB_LINE_UPD_COLOR,
		&bsd_rect_left_color);
	} else {
		bsd_rect_left_color = 0x00ff00;
		lb_gal_update_line(line[0], LB_LINE_UPD_COLOR,
		&bsd_rect_left_color);
	}
	if (hidden == 1)
		lb_gal_set_obj_hidden(line[0], false);


	hidden = lb_gal_get_obj_hidden(line[1]);
	line_points_r.num = 5;
	bsd_line_trans(&line_points_r, p_det_info->warnArea.rtArea, 5);
	if (line_need_update(line_points_r, last_line_points[1], 4)) {
		lb_gal_update_line(line[1], LB_LINE_UPD_POINTS,
			&line_points_r);
		line_save(line_points_r, &last_line_points[1]);
	}
#ifdef LOMBO_GPS
	if (p_det_info->bsd.rtWarn && (gps_ret != 0 ||
		gps_data.speed >= get_bsd_alarm_speed_map())) {
#else
	if (p_det_info->bsd.rtWarn) {
#endif
		bsd_rect_right_color = 0xff0000;
		lb_gal_update_line(line[1], LB_LINE_UPD_COLOR,
		&bsd_rect_right_color);
	} else {
		bsd_rect_right_color = 0x00ff00;
		lb_gal_update_line(line[1], LB_LINE_UPD_COLOR,
		&bsd_rect_right_color);
	}
	if (hidden == 1)
		lb_gal_set_obj_hidden(line[1], false);


}
#endif

static void bsd_alarm_disp(HDFrameGetBsd *p_det_info)
{
	disp_count++;
//#ifdef BLOCK_LINKER_DRAW
#if 0

	car_draw_info_t	left_draw_info;
	car_draw_info_t	right_draw_info;
	static	int		left_show, right_show;

	if (p_det_info->bsd.ltWarn) {
		if (disp_count % 5 == 0) {
			left_show = 0;
			memset(&left_draw_info, 0, sizeof(left_draw_info));
			if (get_rearmirr_enable())
				left_draw_info.idx = BSD_BKL_LEFT_WAR_IDX;
			else
				left_draw_info.idx = BSD_BKL_RIGHT_WAR_IDX;
			left_draw_info.b_draw = BKL_DRAW_BSD_LINE;
			left_draw_info.buff_rect.x1 = 0;
			left_draw_info.buff_rect.y1 = 0;
			left_draw_info.buff_rect.x2 = 1;
			left_draw_info.buff_rect.y2 = 1;
			cdr_draw_msg_send(left_draw_info);
		} else {
			if (left_show == 0) {
				left_show = 1;
				memset(&left_draw_info, 0, sizeof(left_draw_info));
				if (get_rearmirr_enable())
					left_draw_info.idx = BSD_BKL_LEFT_WAR_IDX;
				else
					left_draw_info.idx = BSD_BKL_RIGHT_WAR_IDX;
				left_draw_info.b_draw = BKL_DRAW_BSD_LINE;
				cdr_draw_msg_send(left_draw_info);
			}
		}
	} else if (left_show) {
		left_show = 0;
		memset(&left_draw_info, 0, sizeof(left_draw_info));
		if (get_rearmirr_enable())
			left_draw_info.idx = BSD_BKL_LEFT_WAR_IDX;
		else
			left_draw_info.idx = BSD_BKL_RIGHT_WAR_IDX;
		left_draw_info.b_draw = BKL_DRAW_BSD_LINE;
		left_draw_info.buff_rect.x1 = 0;
		left_draw_info.buff_rect.y1 = 0;
		left_draw_info.buff_rect.x2 = 1;
		left_draw_info.buff_rect.y2 = 1;
		cdr_draw_msg_send(left_draw_info);
	}

	if (p_det_info->bsd.rtWarn)  {
		if (disp_count % 5 == 0) {
			right_show = 0;
			memset(&right_draw_info, 0, sizeof(right_draw_info));
			if (get_rearmirr_enable())
				right_draw_info.idx = BSD_BKL_RIGHT_WAR_IDX;
			else
				right_draw_info.idx = BSD_BKL_LEFT_WAR_IDX;
			right_draw_info.b_draw = BKL_DRAW_BSD_LINE;
			right_draw_info.buff_rect.x1 = 0;
			right_draw_info.buff_rect.y1 = 0;
			right_draw_info.buff_rect.x2 = 1;
			right_draw_info.buff_rect.y2 = 1;
			cdr_draw_msg_send(right_draw_info);
		} else {
			if (right_show == 0) {
				right_show = 1;
				memset(&right_draw_info, 0, sizeof(right_draw_info));
				if (get_rearmirr_enable())
					right_draw_info.idx = BSD_BKL_RIGHT_WAR_IDX;
				else
					right_draw_info.idx = BSD_BKL_LEFT_WAR_IDX;
				right_draw_info.b_draw = BKL_DRAW_BSD_LINE;
				cdr_draw_msg_send(right_draw_info);
			}
		}
	} else if (right_show) {
		right_show = 0;
		memset(&right_draw_info, 0, sizeof(right_draw_info));
		if (get_rearmirr_enable())
			right_draw_info.idx = BSD_BKL_RIGHT_WAR_IDX;
		else
			right_draw_info.idx = BSD_BKL_LEFT_WAR_IDX;
		right_draw_info.b_draw = BKL_DRAW_BSD_LINE;
		right_draw_info.buff_rect.x1 = 0;
		right_draw_info.buff_rect.y1 = 0;
		right_draw_info.buff_rect.x2 = 1;
		right_draw_info.buff_rect.y2 = 1;
		cdr_draw_msg_send(right_draw_info);
	}
#else
	int hidden = 0;
	if (get_rearmirr_enable()) {
		if (p_det_info->bsd.ltWarn) {
			hidden = lb_gal_get_obj_hidden(right_war_img);
			if (!hidden) {
				if (disp_count%5 == 0)
					lb_gal_set_obj_hidden(right_war_img, true);
			} else
				lb_gal_set_obj_hidden(right_war_img, false);
		} else {
			hidden = lb_gal_get_obj_hidden(right_war_img);
			if (!hidden)
				lb_gal_set_obj_hidden(right_war_img, true);

		}

		if (p_det_info->bsd.rtWarn) {
			hidden = lb_gal_get_obj_hidden(left_war_img);
			if (!hidden) {
				if (disp_count%10 == 0)
					lb_gal_set_obj_hidden(left_war_img, true);
			} else
				lb_gal_set_obj_hidden(left_war_img, false);
		} else {
			hidden = lb_gal_get_obj_hidden(left_war_img);
			if (!hidden)
				lb_gal_set_obj_hidden(left_war_img, true);
		}
	} else {
		if (p_det_info->bsd.ltWarn) {
			hidden = lb_gal_get_obj_hidden(left_war_img);
			if (!hidden) {
				if (disp_count%10 == 0)
					lb_gal_set_obj_hidden(left_war_img, true);
			} else
				lb_gal_set_obj_hidden(left_war_img, false);
		} else {
			hidden = lb_gal_get_obj_hidden(left_war_img);
			if (!hidden)
				lb_gal_set_obj_hidden(left_war_img, true);

		}

		if (p_det_info->bsd.rtWarn) {
			hidden = lb_gal_get_obj_hidden(right_war_img);
			if (!hidden) {
				if (disp_count%10 == 0)
					lb_gal_set_obj_hidden(right_war_img, true);
			} else
				lb_gal_set_obj_hidden(right_war_img, false);
		} else {
			hidden = lb_gal_get_obj_hidden(right_war_img);
			if (!hidden)
				lb_gal_set_obj_hidden(right_war_img, true);
		}
	}
#endif
}

static void bsd_alarm_tone(HDFrameGetBsd *p_det_info)
{
#ifdef LOMBO_GPS
	if (gps_ret != 0 || gps_data.speed >= get_bsd_alarm_speed_map()) {
#else
	if (1) {
#endif
		if (p_det_info->bsd.ltWarn && p_det_info->bsd.ltWarn !=
			last_alarm_bsd_left_status && alarm_left_bsd_tone_interval >
			ALARM_INTERVAL_COUNT) {
			APP_LOG_W("p_det_info->bsd.ltWarn:%d\n", p_det_info->bsd.ltWarn);
			if (get_rearmirr_enable()) {
				lb_system_mq_send(LB_SYSMSG_ALARM_BSD_RIGHT, NULL, 0, 0);
				alarm_left_bsd_tone_interval = 0;
			} else {
				lb_system_mq_send(LB_SYSMSG_ALARM_BSD_LEFT, NULL, 0, 0);
				alarm_left_bsd_tone_interval = 0;
			}
		}
		if (p_det_info->bsd.rtWarn && p_det_info->bsd.rtWarn !=
			last_alarm_bsd_right_status && alarm_right_bsd_tone_interval >
			ALARM_INTERVAL_COUNT) {
			APP_LOG_W("p_det_info->bsd.rtWarn:%d\n", p_det_info->bsd.rtWarn);
			if (get_rearmirr_enable()) {
				lb_system_mq_send(LB_SYSMSG_ALARM_BSD_LEFT, NULL, 0, 0);
				alarm_right_bsd_tone_interval = 0;
			} else {
				lb_system_mq_send(LB_SYSMSG_ALARM_BSD_RIGHT, NULL, 0, 0);
				alarm_right_bsd_tone_interval = 0;
			}
		}
		last_alarm_bsd_left_status = p_det_info->bsd.ltWarn;
		last_alarm_bsd_right_status = p_det_info->bsd.rtWarn;
	}
}
/**
 * adas_resp -Draw a tracking box in the image according to the calculation results
 * get_data :the strcuct for drawing parameter
 *
 * This function use to Draw a tracking box in the image
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
int adas_resp(HDFrameGetData *get_data)
{
	int ret = -1;

	if (!get_object_flag) {
		disp_get_object();
		get_object_flag = 1;
	}

	if (get_views_status() == FRONT_VIEW) {
#ifdef LOMBO_GPS
		gps_ret = get_gps_speed(&gps_data);
		if (gps_ret == -1)
			gps_data.speed = 60;
#ifdef GPS_DEBUG
		if (gps_debug_w) {
			sprintf(gps_info, "V:%02d A:%d %d %d %d C:%d",
				gps_data.speed,
				last_alarm_dist_status,
				get_data->cars.warmCar.warnStatus,
				last_alarm_pressure_status, get_data->lanes.status,
				gps_ret);
			lb_gal_update_label(gps_debug_w, LB_LABEL_UPD_TXT,
				gps_info);
			if (lb_gal_get_obj_hidden(gps_debug_w) == 1)
				lb_gal_set_obj_hidden(gps_debug_w, false);
		}
#endif
#endif
		adas_alarm_cars_tone(get_data);
		adas_disp_mark_cars(get_data);
		adas_alarm_lanes_tone(&get_data->lanes);
		adas_disp_mark_lanes(&get_data->lanes);
	}

	ret = 0;

	return ret;
}
/**
 * bsd_resp -Draw a tracking box in the image according to the calculation results
 * get_data :the strcuct for drawing parameter
 *
 * This function use to Draw a tracking box in the image
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
int bsd_resp(HDFrameGetBsd *get_data)
{
	int ret = -1;

	if (!get_object_flag) {
		disp_get_object();
		get_object_flag = 1;
	}
	/* draw objects */
	if (get_views_status() == BACK_VIEW) {
#ifdef LOMBO_GPS
		if (disp_count%10 == 0) {
			gps_ret = get_gps_speed(&gps_data);
			if (gps_ret == -1)
				gps_data.speed = 60;
		}
#ifdef GPS_DEBUG
		if (gps_debug_w) {
			sprintf(gps_info, "V:%02d C:%d ltWarn:%d rtWarn:%d",
				gps_data.speed, gps_ret,
				get_data->bsd.ltWarn, get_data->bsd.rtWarn);
			lb_gal_update_label(gps_debug_w, LB_LABEL_UPD_TXT,
				gps_info);
			if (lb_gal_get_obj_hidden(gps_debug_w) == 1)
				lb_gal_set_obj_hidden(gps_debug_w, false);
		}
#endif
#endif
       if (get_warn_tone_enable() == ENABLE_ON){
	     	bsd_alarm_tone(get_data);
       	}
		bsd_alarm_disp(get_data);
		bsd_disp_mark_cars(get_data);
#ifdef REAR_BSD_DEBUG
		//bsd_disp_mark_alarm_area(get_data);
#endif
		if (alarm_left_bsd_tone_interval <= ALARM_INTERVAL_COUNT)
			alarm_left_bsd_tone_interval++;
		if (alarm_right_bsd_tone_interval <= ALARM_INTERVAL_COUNT)
			alarm_right_bsd_tone_interval++;
	}


	return ret;
}

