/*
 * home_ctrl.h - home widget reg & unreg implement head file
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

#ifndef __HOME_CTRL_H__
#define __HOME_CTRL_H__

#include <pthread.h>
#include <rtthread.h>
#include "input/gps/gps_dev.h"
#include "lb_types.h"
#include "lb_common.h"

/* home status bar widget object struct */
typedef struct sb_widget_ {
	void	*week;
	void	*ytd;
	void	*time;
	void	*tfcard;
	void	*battery;
	void	*home;
	void	*back;
	void	*home_logo;
	void	*logo;
	void	*recording_s; /* cdr recording status property */
	void	*left_lay;
	void	*right_lay;

} sb_widget_t;

/* home status bar widget object property struct */
typedef struct sb_property_ {
	lb_label_t	*week;
	lb_label_t	*ytd;
	lb_label_t	*time;
	lb_img_t	*tfcard;
	lb_img_t	*battery;
	lb_img_t	*recording_s; /* cdr recording status widget */
	lb_img_t	*home_logo;
	lb_img_t	*logo;
} sb_property_t;

typedef struct sb_last_s_ {
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
} sb_last_s_t;

typedef struct cmdbar_property_ {
	lb_imgbtn_t	*system_setting;
	lb_imgbtn_t	*smart_drive;
	lb_imgbtn_t	*file_explorer;
	lb_imgbtn_t	*cdr;
	lb_imgbtn_t	*cdr_setting;
} cmdbar_property_t;

typedef struct cmdbar_widget_ {
	void	*system_setting;
	void	*smart_drive;
	void	*file_explorer;
	void	*cdr;
	void	*cdr_setting;
} cmdbar_widget_t;

typedef struct _weekday_ {
	const char *wd_str;
	int wd_index;
} weekday_t;

typedef struct _home_flag_ {
	int thread_exit;
	int delay_time;
	int lcd_brightness;
	int dialog_value;
	int key_poweroff;
	int set_gps_time;
	int click_en;
	void *dialog_temp;
} home_flag_t;

typedef struct _fs_pro_ {
	int fs_unformat_flag;
	/* fs umount prepare flag 1: umount prepare 0: umount ok */
	int fs_unmount_pre_flag;
	rt_event_t fs_unpre_event;
	void *fs_unpre;
} fs_pro_t;

typedef struct apps_t_ {
	app_t home;
	app_t car_recorder;
	app_t file_explorer;
	app_t smart_drive;
	app_t cdr_setting;
	app_t system_setting;
} apps_t;
/* dialog enum  */
typedef enum _dialog_e_ {
	QUIT_RECORDER = 0xC100,
	PLEASE_PLUGIN_SDCARD = 0xC101,
	START_INTERVAL_REC = 0xC102,
	STOP_INTERVAL_REC = 0xC103,
	DIALOG_ACC_OFF = 0xC104,
	DIALOG_MAX
} dialog_e;

typedef struct  _cdr_interval_t_ {
	lb_int32 acc_shutdown_duration_h;
	lb_int32 acc_closebk_duration_s;
} cdr_interval_t;
#define HEADBAR_THREAD_PRIORITY		24
#define HEADBAR_THREAD_STACK_SIZE	4096

#define POWER_KEY			116

#define LB_MSG_HOME			(LB_MSG_HOME_BASE | 0x01)
#define LB_MSG_BACK			(LB_MSG_HOME_BASE | 0x02)
#define LB_MSG_SYSTEM_SETTING		(LB_MSG_HOME_BASE | 0x03)
#define LB_MSG_SMART_DRIVE		(LB_MSG_HOME_BASE | 0x04)
#define LB_MSG_FILE_EXPLORE		(LB_MSG_HOME_BASE | 0x05)
#define LB_MSG_CDR			(LB_MSG_HOME_BASE | 0x06)
#define LB_MSG_CDR_SETTING		(LB_MSG_HOME_BASE | 0x07)
#define LB_MSG_POWEROFF			(LB_MSG_HOME_BASE | 0x08)
#define LB_MSG_LOWPOWEROFF		(LB_MSG_HOME_BASE | 0x09)
#define LB_MSG_REFRESH_CARD		(LB_MSG_HOME_BASE | 0x0A)
#define LB_MSG_REFRESH_BATTERY		(LB_MSG_HOME_BASE | 0x0B)
#define LB_MSG_SDCARD_PLUGIN		(LB_MSG_HOME_BASE | 0x0C)
#define LB_MSG_SDCARD_PLUGOUT		(LB_MSG_HOME_BASE | 0x0D)
#define LB_MSG_SDCARD_INIT_FAIL		(LB_MSG_HOME_BASE | 0x0E)
#define LB_MSG_SDCARD_MOUNT_FAIL	(LB_MSG_HOME_BASE | 0x0F)
#define LB_MSG_HOME_DIALOG		(LB_MSG_HOME_BASE | 0x10)
#define LB_MSG_SDCARD_PRE_UNMOUNT	(LB_MSG_HOME_BASE | 0x11)
#define LB_MSG_SDCARD_LOW_PERF		(LB_MSG_HOME_BASE | 0x12)

#define WEEK_ID				121
#define YTD_ID				122
#define TIME_ID				123
#define TFCARD_ID			111
#define BATTERY_ID			112
#define HOME_LOGO_ID			113
#define LOGO_ID				124

#define HOME_ID			101
#define BACK_ID			102

#define LEFT_LAY_ID      110 
#define RIGHT_LAY_ID	 120 

#define LEFT_UP_LAY_ID   130 
#define RIGHT_UP_LAY_ID	 140 


#define SYSTEM_SETTING_ID	103
#define SMART_DRIVE_ID		104
#define FILE_EXPLORER_ID	105
#define CDR_ID			106
#define CDR_SETTING_ID		107

#define TXT_MAX 32

extern home_flag_t home_flag;

lb_int32 home_view_get_obj_ext(void);
lb_int32 home_play_boot_music(void);
lb_int32 home_view_hidden_logo(void);
lb_int32 home_view_show_time_and_status(void);
lb_int32 home_view_set_home_back_click(bool en);
lb_int32 home_reg_init_exit_funcs(void);
lb_int32 home_unreg_init_exit_funcs(void);
lb_int32 home_reg_resp_funcs(void);
lb_int32 home_unreg_resp_funcs(void);
lb_int32 home_reg_resp_sys_funcs(void);
lb_int32 home_unreg_resp_sys_funcs(void);
lb_int32 statusbar_init(pthread_t *thread_id);
lb_int32 statusbar_exit(pthread_t thread_id);
lb_int32 home_stop_recorder(void);
lb_int32 home_view_hide_hbtb(void);
lb_int32 home_view_show_hbtb(void);
lb_int32 left_right_lay_control(lb_int8 tmp);




#endif /* __HOME_CTRL_H__ */
