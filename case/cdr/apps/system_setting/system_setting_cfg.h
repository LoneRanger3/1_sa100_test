/*
 * system_setting_cfg.h - system config of setting code for LomboTech
 * system config of setting interface and macro define
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

#ifndef __SYSTEM_SETTING_CFG_H__
#define __SYSTEM_SETTING_CFG_H__

#include <rtthread.h>
#include "cJSON.h"
#include "system/system.h"

#ifdef ARCH_LOMBO_N7_CDR_MMC
#define CUSTOMER_CONFIG_PATH ROOTFS_MOUNT_PATH"/etc/cdr_config1.cfg"
#define ORIGINAL_CONFIG_PATH ROOTFS_MOUNT_PATH"/etc/cdr_config.cfg"
#else
#define CUSTOMER_CONFIG_PATH ROOTFS_MOUNT_PATH"/mnt/data/cdr_config.cfg"
#define ORIGINAL_CONFIG_PATH ROOTFS_MOUNT_PATH"/etc/cdr_config.cfg"
#endif

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
	cJSON *system_json;
	cJSON *version_json;
	system_t system;
} cdr_config_t;

lb_int32 system_set_init(void);
lb_int32 system_set_exit(void);
lb_int32 system_set_save(void);
lb_int32 system_set_default(void);

#endif /* __SYSTEM_SETTING_CFG_H__ */
