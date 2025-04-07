/*
 * home_common.h - home common api head file
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

#ifndef __HOME_COMMON_H__
#define __HOME_COMMON_H__

#include <pthread.h>
#include "eos.h"
#include "dfs_posix.h"
#include "boot_param.h"
#include "system/system.h"
#include "power_drv.h"
#include "lb_types.h"
#include "cJSON.h"
#include "lb_cfg_file.h"
#include "lb_common.h"
#include "case_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FORMAT_OPE_PRIO 28
#define FORMAT_OPE_SIZE 4096
#define FORMAT_REF_PRIO 28
#define FORMAT_REF_SIZE 4096

#define FORMAT_CARD_SUCCEED 20
#define FORMAT_CARD_FAILED 21
#define FORMAT_NO_CARD 8
#define FORMAT_OTHER_ERR 3

typedef enum {
	FMT_CARD_SUCC = 0,
	FMT_CARD_FAIL = -1,
	FMT_NO_CARD = -2,
	FMT_OTHER_ERR = -3,
} fmt_err_t;

typedef enum _sdcard_status_e_ {
	SDCARD_NOT_PLUGIN,
	SDCARD_PLUGIN,
	SDCARD_NOT_FORMAT,
	SDCARD_MAX
} sdcard_status_e;

typedef enum _enable_e_ {
	ENABLE_OFF,
	ENABLE_ON,
	ENABLE_MAX
} enable_e;

/* volume value  */
typedef enum _volume_e_ {
	VOLUME_OFF = 0,
	VOLUME_LOW = 70,
	VOLUME_MID0 = 80,
	VOLUME_MID1 = 90,
	VOLUME_HIG = 100,
	VOLUME_MAX
} volume_e;

/* battery value  */
typedef enum _battery_e_ {
	BATTERY_CHARGING = 0,	/* battery charging status */
	BATTERY_LOW,	/* low power */
	BATTERY_MID,
	BATTERY_FULL,	/* full power */
	BATTERY_MAX
} battery_e;

typedef enum _lcd_standby_time_e_ {
	STANDBY_OFF,
	STANDBY_10 = 10,
	STANDBY_30 = 30,
	STANDBY_60 = 60,
	STANDBY_MAX
} lcd_standby_time_e;

typedef enum _sd_status_e_ {
	SD_PLUGOUT,
	SD_PLUGIN,
	SD_PLUGIN_NOT_FORMAT,
	SD_STATUS_MAX
} sd_status_e;

typedef enum _language_e_ {
	ENGLISH,
	CHINESE_S,
	CHINESE_T,
	JAPANESE,
	RUSSIAN,
	THAI,
	VIETNAMESE,
	LANGUAGE_MAX
} language_e;

typedef struct _home_status_ {
	lb_int32 bat_s;
	lb_int32 tf_s;
	lb_int32 av_s;
	lb_int32 back_s;
	lb_int32 acc_s;
} home_status_t;

/* home config json object struct */
typedef struct _home_cfg_jobj_ {
	cJSON *cfg_root;
	cJSON *system_para;
	cJSON *record_para;
	cJSON *lcd_brightness;
	cJSON *volume;
	cJSON *lcd_standby_time;
	cJSON *keytone_enable;
	cJSON *fastboot_enable;
	cJSON *language;
	cJSON *interval_record;
	cJSON *version_cfg;
} home_cfg_jobj_t;

lb_int32 home_cfg_init(void);

lb_int32 home_cfg_save(void);

lb_int32 home_cfg_exit(void);

battery_e get_bat_status(void);

void set_bat_status(lb_int32 value);

lb_int32 get_tf_status(void);

void set_tf_status(lb_int32 value);

lb_int32 get_av_status(void);

void set_av_status(lb_int32 value);

lb_int32 get_back_status(void);

void set_back_status(lb_int32 value);

int get_acc_status(void);

void set_acc_status(lb_int32 value);

volume_e get_cfg_volume(void);

void set_volume(u32 value);

lb_int32 get_cfg_lcd_standby_time(void);

lb_int32 get_cfg_keytone_enable(void);

lb_int32 get_cfg_fastboot_enable(void);

lb_int32 get_cfg_language(void);

lb_int32 get_cfg_interval_record_enable(void);

char *get_cfg_version(void);

void set_cfg_version(char *value);

void switch_backlight_status(void);

void set_backlight_status(enable_e status);

lb_int32 get_cfg_lcd_brightness(void);

void set_lcd_brightness(u32 value);

void disp_close_device(void);

#ifdef __EOS__RELEASE__MP__
int wdog_open(void);

int wdog_start(void);

int wdog_keepalive(void);

int wdog_close(void);
#endif

lb_int32 format_sdcard_init(void *param);
lb_int32 format_sdcard_exit(void *param);
lb_int32 lock_format_sdcard_init(void *param);
lb_int32 lock_format_sdcard_exit(void *param);

void record_obj_init(char *video_source, record_obj_t *record_obj);
void record_obj_exit(record_obj_t *record_obj);
record_obj_t get_record_obj(void);

#ifdef __cplusplus
}
#endif

#endif /* __HOME_COMMON_H__ */
