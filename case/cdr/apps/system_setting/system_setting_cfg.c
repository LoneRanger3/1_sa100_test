/*
 * system_setting_cfg.c - system config of setting code for LomboTech
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
#include <string.h>

#include "lb_types.h"
#include "lb_common.h"
#include "lb_cfg_file.h"
#include "system_setting_cfg.h"
#include "system/system.h"
#include <app_manage.h>

cdr_config_t cdr_config;

static lb_int32 system_array_init(system_t *syttem, cJSON *para)
{
	lb_int32 ret = 0;
	lb_int32 i = 0;
	lb_int32 js_size = 0;
	cJSON  *temp0 = NULL;
	cJSON  *temp1 = NULL;

	js_size = cJSON_GetArraySize(para);
	if (js_size == 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	for (i = 0; i < js_size; i++) {
		temp0 = cJSON_GetArrayItem(para, i);
		RT_ASSERT(temp0 != NULL);
		RT_ASSERT(temp0->type == cJSON_Object);

		if (cJSON_GetObjectItem(temp0, "lcd_brightness")) {
			temp1 = cJSON_GetObjectItem(temp0, "lcd_brightness");
			syttem->lcd_brightness_json = temp1;
		} else if (cJSON_GetObjectItem(temp0, "volume")) {
			temp1 = cJSON_GetObjectItem(temp0, "volume");
			syttem->volume_json = temp1;
		} else if (cJSON_GetObjectItem(temp0, "lcd_standby_time")) {
			temp1 = cJSON_GetObjectItem(temp0, "lcd_standby_time");
			syttem->lcd_standby_time_json = temp1;
		} else if (cJSON_GetObjectItem(temp0, "keytone_enable")) {
			temp1 = cJSON_GetObjectItem(temp0, "keytone_enable");
			syttem->keytone_enable_json = temp1;
		} else if (cJSON_GetObjectItem(temp0, "fastboot_enable")) {
			temp1 = cJSON_GetObjectItem(temp0, "fastboot_enable");
			syttem->fastboot_enable_json = temp1;
		} else if (cJSON_GetObjectItem(temp0, "rearmirr_enable")) {
			temp1 = cJSON_GetObjectItem(temp0, "rearmirr_enable");
			syttem->rearmirr_enable_json = temp1;
		} else if (cJSON_GetObjectItem(temp0, "accpower_enable")) {
			temp1 = cJSON_GetObjectItem(temp0, "accpower_enable");
			syttem->accpower_enable_json = temp1;
		} else if (cJSON_GetObjectItem(temp0, "language")) {
			temp1 = cJSON_GetObjectItem(temp0, "language");
			syttem->language_json = temp1;
		}
	}

exit:
	return ret;
}

lb_int32 system_set_init(void)
{
	lb_int32 ret = 0;

	/* capture it from customer path first */
	cdr_config.root_json = lb_open_cfg_file(CUSTOMER_CONFIG_PATH);
	if (cdr_config.root_json == NULL) {
		/* capture it from original path again */
		cdr_config.root_json = lb_open_cfg_file(ORIGINAL_CONFIG_PATH);
		/* capture it failed only exit */
		if (cdr_config.root_json == NULL) {
			APP_LOG_W("failed\n");
			ret = -1;
			goto exit;
		}
	}

	cdr_config.system_json = cJSON_GetObjectItem
		(cdr_config.root_json, "system");
	RT_ASSERT(cdr_config.system_json != NULL);
	RT_ASSERT(cdr_config.system_json->type == cJSON_Array);

	ret = system_array_init(&cdr_config.system, cdr_config.system_json);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	cdr_config.version_json = cJSON_GetObjectItem
		(cdr_config.root_json, "version");
	RT_ASSERT(cdr_config.version_json != NULL);
	RT_ASSERT(cdr_config.version_json->string != NULL);

	return ret;
exit:
	if (cdr_config.root_json) {
		lb_exit_cfg_file(cdr_config.root_json);
		/* root_json will release inside upper interface */
		cdr_config.root_json = NULL;
		/* system_json will release inside upper interface */
		cdr_config.system_json = NULL;
		/* version_json will release inside upper interface */
		cdr_config.version_json = NULL;
	}

	return ret;
}

lb_int32 system_set_exit(void)
{
	lb_int32 ret = 0;

	RT_ASSERT(cdr_config.root_json != NULL);

	lb_exit_cfg_file(cdr_config.root_json);
	/* root_json will release inside upper interface */
	cdr_config.root_json = NULL;
	/* system_json will release inside upper interface */
	cdr_config.system_json = NULL;
	/* version_json will release inside upper interface */
	cdr_config.version_json = NULL;

	return ret;
}

lb_int32 system_set_save(void)
{
	lb_int32 ret = 0;

	RT_ASSERT(cdr_config.root_json != NULL);

	/* we save it to json which is coming from customer path */
	ret = lb_save_cfg_file(CUSTOMER_CONFIG_PATH, cdr_config.root_json);
	if (ret != 1)
		ret = -1;
	else
		ret = 0;

	return ret;
}

lb_int32 system_set_default(void)
{
	lb_int32 ret = 0;

	RT_ASSERT(cdr_config.root_json != NULL);

	lb_exit_cfg_file(cdr_config.root_json);
	/* root_json will release inside upper interface */
	cdr_config.root_json = NULL;
	/* system_json will release inside upper interface */
	cdr_config.system_json = NULL;
	/* version_json will release inside upper interface */
	cdr_config.version_json = NULL;

	/* remove it from customer path */
	remove(CUSTOMER_CONFIG_PATH);

	/* capture it from original path */
	cdr_config.root_json = lb_open_cfg_file(ORIGINAL_CONFIG_PATH);
	/* capture it failed only exit */
	if (cdr_config.root_json == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	cdr_config.system_json = cJSON_GetObjectItem
		(cdr_config.root_json, "system");
	RT_ASSERT(cdr_config.system_json != NULL);
	RT_ASSERT(cdr_config.system_json->type == cJSON_Array);

	ret = system_array_init(&cdr_config.system, cdr_config.system_json);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	cdr_config.version_json = cJSON_GetObjectItem
		(cdr_config.root_json, "version");
	RT_ASSERT(cdr_config.version_json != NULL);
	RT_ASSERT(cdr_config.version_json->string != NULL);

	return ret;

exit:
	if (cdr_config.root_json) {
		lb_exit_cfg_file(cdr_config.root_json);
		/* root_json will release inside upper interface */
		cdr_config.root_json = NULL;
		/* system_json will release inside upper interface */
		cdr_config.system_json = NULL;
		/* version_json will release inside upper interface */
		cdr_config.version_json = NULL;
	}

	return ret;
}
