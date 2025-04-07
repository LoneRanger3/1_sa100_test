/*
 * cdr_setting_cfg.h - cdr config of setting code for LomboTech
 * cdr config of setting interface and macro define
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

#ifndef __CDR_SETTING_CFG_H__
#define __CDR_SETTING_CFG_H__

#include "cJSON.h"
#include "da380_gsensor.h"
#include "system/system.h"

#ifdef ARCH_LOMBO_N7_CDR_MMC
#define CUSTOMER_CONFIG_PATH ROOTFS_MOUNT_PATH"/etc/cdr_config1.cfg"
#define ORIGINAL_CONFIG_PATH ROOTFS_MOUNT_PATH"/etc/cdr_config.cfg"
#else
#define CUSTOMER_CONFIG_PATH ROOTFS_MOUNT_PATH"/mnt/data/cdr_config.cfg"
#define ORIGINAL_CONFIG_PATH ROOTFS_MOUNT_PATH"/etc/cdr_config.cfg"
#endif

/* #define PARSE_TABLE */

typedef struct tag_record {
	cJSON *record_duration_json;
	cJSON *record_resolution_json;
	cJSON *gsensor_sensity_json;
	cJSON *mute_enable_json;
	cJSON *watermark_time_enable_json;
	cJSON *watermark_logo_enable_json;
	cJSON *park_monitor_json;
	cJSON *interval_record_json;
	cJSON *lcd_standby_time_json;
	cJSON *warn_tone_json;
	cJSON *language_json;
	cJSON *lcd_brightness_json;
} record_t;
typedef struct tag_system {
	cJSON *lcd_brightness_json;
	cJSON *volume_json;
	cJSON *lcd_standby_time_json;
	cJSON *keytone_enable_json;
	cJSON *fastboot_enable_json;
	cJSON *language_json;
	cJSON *rearmirr_enable_json;
	cJSON *accpower_enable_json;
} system_t;

typedef struct tag_cdr_config {
	cJSON *root_json;
	cJSON *record_json;
	cJSON *system_json;
	record_t record;
	system_t system;
	cJSON *version_json;
} cdr_config_t;

lb_int32 record_set_init(void);
lb_int32 record_set_exit(void);
lb_int32 record_set_save(void);
lb_int32 system_set_default(void);

#endif /* __CDR_SETTING_CFG_H__ */
