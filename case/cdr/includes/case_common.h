/*
 * case_common.h - common struct or marco for case
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

#ifndef __CASE_COMMON_H__
#define __CASE_COMMON_H__



typedef struct _record_obj_ {
	void *record_fhd;
	void *record_rhd;
} record_obj_t;

#if 0
/*static view*/
#define APP_HOME_VIEW				0
#define APP_CAR_RECORDER_VIEW			1
#define APP_FILE_EXPLORER_VIEW			2
#define APP_FILE_EXPLORER_IMAGE_PLAYER_VIEW	3
#define APP_FILE_EXPLORER_VIDEO_PLAYER_VIEW	4
#define APP_CDR_SETTING_VIEW			5
#define APP_CDR_SETTING_LOOP_RECORD_VIEW	6
#define APP_CDR_SETTING_GSENSOR_VIEW		7
#define APP_CDR_SETTING_INTERVAL_RECORD_VIEW	8

#define APP_SYS_SETTING_VIEW			9
#define APP_SYS_SETTING_BRIGHTEN_VIEW		10
#define APP_SYS_SETTING_VOLUME_VIEW		11
#define APP_SYS_SETTING_STANDBY_VIEW		12
#define APP_SYS_SETTING_LANGUAGE_VIEW		13
#define APP_SYS_SETTING_DTIME_VIEW		14
#define APP_SYS_SETTING_FORMAT_VIEW		15
#define APP_SYS_SETTING_VERSION_VIEW		16
#define APP_SYS_SETTING_RESET_VIEW		17
#define APP_SMART_DRIVE_VIEW			18
#define APP_BSD_CALI_METHOD			19
#define APP_BSD_CALI_VIEW0			20
#define APP_BSD_SET_VIEW			21

/*isolate view*/
#define APP_FORMAT_SD_VIEW			50
#define APP_LOCK_RECORDER_FILE_VIEW		51
#define APP_LOWPOWER_SHUTDOWN_VIEW		52
#define APP_PART_MONITOR_TIMES_VIEW		53
#define APP_PLEASE_FORMAT_SDCARD_VIEW		54
#define APP_PLEASE_PLUGIN_AV_VIEW		55
#define APP_PLEASE_PLUGIN_SDCARD_VIEW		56
#define APP_PROGRESS_BAR_VIEW			57
#define APP_QUIT_RECORDER_VIEW			58
#define APP_SDCARD_FORMAT_FAIL_VIEW		59
#define APP_SDCARD_FORMAT_SUCCEED_VIEW		60
#define APP_SDCARD_FULL_VIEW			61
#define APP_SDCARD_LOCK_FULL_VIEW		62
#define APP_SDCARD_PLUGIN_VIEW			63
#define APP_SDCARD_PLUGOUT_VIEW			64
#define APP_SHUTDOWN_VIEW			65
#define APP_UNKNOW_VIEW				66
#define APP_UNLOCK_RECORDER_FILE_VIEW		67
#define APP_WARN_VIEW				68
#define APP_ERROR_VIEW				69
#define APP_LOWER_PERF_FORMAT_SDCARD_VIEW	70
#define APP_START_INTERVAL_REC_VIEW		71
#define APP_STOP_INTERVAL_REC_VIEW		72
#define APP_UPGRADE_FIREWARE_VIEW		73
#define APP_DETELE_DIALOG_VIEW			74
#define APP_LOCK_DIALOG_VIEW			75
#define APP_UNLOCK_DIALOG_VIEW			76
#define APP_BACKDOOR_DIALOG_VIEW		77
#endif


#endif /* __CASE_COMMON_H__ */
