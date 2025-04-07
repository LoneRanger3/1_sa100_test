/*
 * car recorder widget reg & unreg api head file
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

#ifndef __CAR_RECORDER_CTRL_H__
#define __CAR_RECORDER_CTRL_H__

#include "pthread.h"
#include "libadas.h"
#include "libbsd.h"
#include "lb_types.h"
#include "lb_common.h"



typedef struct apps_t_ {
	app_t home;
	app_t car_recorder;
	app_t file_explorer;
	app_t smart_drive;
	app_t cdr_setting;
	app_t system_setting;
} apps_t;


/* car recorder status widget  struct */
typedef struct status_widget {
	void	*record_time;
	void	*record_status;
	void	*record_status_bg;
	void	*gps_status;
	void	*interval_rec_status;
	void	*lock_rec_status;
	void	*lock;
	void	*views;
	void	*record_onoff;
	void	*cmd_bg;
	void	*cont_ts;
	void	*mic;
	void	*light;
	void	*light_bg;
	void	*up_bg;
	void	*up_week;
	void	*up_ytd;
	void	*up_time;
	void	*up_tfcard;
	void	*up_battery;
} status_widget_t;

/* car recorder status property  struct */
typedef struct status_property {
	lb_label_t	*record_time;
	lb_img_t	*record_status; /* cdr recording status widget */
	lb_img_t	*record_status_bg;
	lb_img_t	*gps_status;
	lb_img_t	*interval_rec_status;
	lb_img_t	*lock_rec_status;
	lb_img_t	*light;
	lb_img_t	*mic;
	lb_imgbtn_t	*lock;
	lb_imgbtn_t	*views;
	lb_imgbtn_t	*record_onoff;
	lb_label_t  *up_week;
	lb_label_t  *up_ytd;
	lb_label_t  *up_time;
	lb_label_t  *up_tfcard;
	lb_label_t  *up_battery;
} status_property_t;


typedef struct sb_last_r_ {
	lb_int32 last_min;
	lb_int32 last_hour;
	lb_int32 last_year;
	lb_int32 last_mon;
	lb_int32 last_day;
	lb_int32 last_week;
	lb_int32 last_bat_s;
	lb_int32 last_tf_s;
	lb_int32 last_recording_s; /* cdr last recording status widget */
	lb_int32 last_acc_s; /* use for cdr interval record */
} sb_last_r_t;

typedef struct _weekday_ {
	const char *wd_str;
	int wd_index;
} weekday_r;

/* call back for get adas result  struct */
typedef struct adas_result_callback_info {
	int type; /* for reserved */
	int (*get_adas_result)(HDFrameGetData *);
} adas_result_callback_t;

/* call back for get bsd result  struct */
typedef struct bsd_result_callback_info {
	int type; /* for reserved */
	int (*get_bsd_result)(HDFrameGetBsd *);
} bsd_result_callback_t;

#define CDR_STATUS_THREAD_PRIORITY	24
#define CDR_COMP_THREAD_PRIORITY	24

#define CDR_STATUS_STACK_SIZE		4096
#define CDR_COMP_STACK_SIZE		4096

#define	CHINESE_S	1
#define	CHINESE_T	2

typedef enum _comp_cmd_e_ {
	COMP_START_ADAS,
	COMP_STOP_ADAS,
	COMP_START_BSD,
	COMP_STOP_BSD,
	COMP_START_FRONT_RECORDER,
	COMP_STOP_FRONT_RECORDER,
	COMP_START_REAR_RECORDER,
	COMP_STOP_REAR_RECORDER,
	COMP_MAX
} comp_cmd_e;

typedef enum _alarm_status_e_ {
	LEFT_LANE = -1,
	CURRENT_LANE = 0,
	RIGHT_LANE,
	CURRENT_LANE_NEAREST,
	ALARM_COLLIDE,
	ALARM_LAUNCH,
	CARE_FRONT_CAR,
	KEEP_DISTANCE,
	CARE_COLLIDE,
} alarm_status_e;

#define LB_MSG_RECORDER			(LB_MSG_CAR_RECORDER_BASE | 0x21)
#define LB_MSG_CAMERA			(LB_MSG_CAR_RECORDER_BASE | 0x22)
#define LB_MSG_LOCK			(LB_MSG_CAR_RECORDER_BASE | 0x23)
#define LB_MSG_MIC			(LB_MSG_CAR_RECORDER_BASE | 0x24)
#define LB_MSG_VIEW			(LB_MSG_CAR_RECORDER_BASE | 0x25)
#define LB_MSG_DIALOG			(LB_MSG_CAR_RECORDER_BASE | 0x26)
#define LB_MSG_CDR_LOCK_STATUS_UPDATE	(LB_MSG_CAR_RECORDER_BASE | 0x28)
#define LB_MSG_RECORDER_TIME_UPDATE	(LB_MSG_CAR_RECORDER_BASE | 0x29)
#define LB_MSG_RECORDER_CMD_REFRESH	(LB_MSG_CAR_RECORDER_BASE | 0x30)
#define LB_MSG_SETTING_MODE	            (LB_MSG_CAR_RECORDER_BASE | 0x10)
#define LB_MSG_FILE_MODE	        (LB_MSG_CAR_RECORDER_BASE | 0x27)
#define LB_MSG_SMART_DRIVE_MODE	        (LB_MSG_CAR_RECORDER_BASE | 0x28)
#define LB_MSG_LIGHT_ABATE	        (LB_MSG_CAR_RECORDER_BASE | 0x31)
#define LB_MSG_LIGHT_ADD	        (LB_MSG_CAR_RECORDER_BASE | 0x32)



#define CMD_ID	206
#define CONT_TS_ID	208

#define LOCK_ID		203
#define RECORD_ONOFF_ID	 201
#define VIEWS_ID	205
#define INTERVAL_REC_ID	217
#define RECORD_STATUS_BG_ID		213
#define RECORD_ID			214
#define RECORD_TIME			215
#define GPS_STATUS_ID			216
#define TIME_MAX			32
#define TXT_MAX			16
#define BTN_BASE_ID		220
#define LINE_BASE_ID		292

#define LOCK_STATUS_ID		19
#define MIC_STATUS_ID		18
#define LIGHT_STATUS_ID		22
#define LIGHT_LAY_ID        23
#define UP_LAY_ID           24


#define CR_CAR_MAX 8
#define CR_LINE_MAX 2
#define ALARM_INTERVAL_COUNT	200 //100

#define CR_TXT_MAX 32
#define DEFAULT_SLIDER_WIDTH 100
#define DEFAULT_SLIDER_HIGH 30

#define CDR_CAR_UPDATE_LIMIT 5
#define CDR_LINE_UPDATE_LIMIT 10
#define MSG_FROM_CLICK 0x400


#define UP_WEEK_ID				27
#define UP_YTD_ID				28
#define UP_TIME_ID				29
#define UP_TFCARD_ID			25
#define UP_BATTERY_ID			26

#define LEFT_WAR_IMG_ID			228
#define RIGHT_WAR_IMG_ID		229
void *car_frame_w[CR_CAR_MAX];
void *car_dis_w[CR_CAR_MAX];

void *car_frame_w_bsd[CR_CAR_MAX];
void *car_dis_w_bsd[CR_CAR_MAX];

void *left_war_img;
void *right_war_img;

int last_car_num;
int last_alarm_pressure_status;
int last_alarm_bsd_left_status;
int last_alarm_bsd_right_status;
int last_bsd_car_num;
#ifdef GPS_DEBUG
void *gps_debug_w;
lb_label_t *gps_debug_p;
char gps_info[CR_TXT_MAX];
#define GPS_DEBUG_ID	268
#endif
int adas_resp(HDFrameGetData *get_data);
int bsd_resp(HDFrameGetBsd *get_data);
void bsd_disp_hidden_object(void);
lb_int32 cdr_cmd_hidden(void);
lb_int32 cdr_cmd_click_en(bool en);
lb_int32 cdr_reg_init_exit_funcs(void);
lb_int32 cdr_unreg_init_exit_funcs(void);
lb_int32 cdr_reg_resp_msg_funcs(void);
lb_int32 cdr_unreg_resp_msg_funcs(void);
lb_int32 cdr_reg_resp_sysmsg_funcs(void);
lb_int32 cdr_unreg_resp_sysmsg_func(void);
lb_int32 cdr_status_thread_init(pthread_t *thread_id);
lb_int32 cdr_status_thread_exit(void);
void _get_config_disp_para(win_para_t *disp_para, int index);

#endif /* __CAR_RECORDER_CTRL_H__ */
