/*
 * fileexp_cfg.c - system config of setting code for LomboTech
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
#include <rtthread.h>
#include <app_manage.h>
#include <mod_manage.h>
#include "lb_types.h"
#include "lb_common.h"
#include "lb_cfg_file.h"
#include "lb_gal_common.h"
#include "fileexp_cfg.h"

fxp_config_t fxp_config;

lb_int32 close_standby(void)
{
	bool enable = 0;

	lb_gal_set_screen_standby_enable(enable);


	return 0;

}

lb_int32 open_standby(void)
{
	bool enable = 1;

	lb_gal_set_screen_standby_enable(enable);

	return 0;

}

lb_int32 close_keytone(void)
{
	lb_int32 keytone = 0;

	if (fxp_config.system.keytone_enable_json) {
		keytone = fxp_config.system.keytone_enable_json->valueint;
		if (keytone == 1)
			lb_set_tone_flag(0);
	}

	return 0;

}

lb_int32 open_keytone(void)
{
	lb_int32 keytone = 0;

	if (fxp_config.system.keytone_enable_json) {
		keytone = fxp_config.system.keytone_enable_json->valueint;
		if (keytone == 1)
			lb_set_tone_flag(1);
	}

	return 0;
}

/**
 * you could extend all field parsing just by configuring "system_table"
 * there is no need modify the function of parsing every time you know guy
 */
#ifdef PARSE_TABLE
static char *system_table[] = {
	"keytone_enable",
	"lcd_standby_time"
};
#endif

/**
 * record_array_init - initial the array from system
 * @param: lb_obj_t object pointer.
 *
 * This function initial the array from record and each elem must be cJSON_Object
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
static lb_int32 system_array_init(system_t *system, cJSON *para)
{
	lb_int32 ret = 0;
	lb_int32 i = 0;
	lb_int32 js_size = 0;
#ifdef PARSE_TABLE
	lb_int32 tb_size = 0;
#endif
	cJSON  *temp0 = NULL;
	cJSON  *temp1 = NULL;

	js_size = cJSON_GetArraySize(para);
	if (js_size == 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

#ifdef PARSE_TABLE
	/* must make sure object from json and object from program are identical */
	tb_size = sizeof(system_table) / sizeof(char *);
	RT_ASSERT(js_size == tb_size);
#endif

	/**
	 * you could extend all field parsing just by configuring "system_table"
	 * there is no need modify the function of parsing every time you know guy
	 */
	for (i = 0; i < js_size; i++) {
		temp0 = cJSON_GetArrayItem(para, i);
		RT_ASSERT(temp0 != NULL);
		RT_ASSERT(temp0->type == cJSON_Object);

		if (cJSON_GetObjectItem(temp0, "keytone_enable")) {
			temp1 = cJSON_GetObjectItem(temp0, "keytone_enable");
			system->keytone_enable_json = temp1;
		}

		if (cJSON_GetObjectItem(temp0, "lcd_standby_time")) {
			temp1 = cJSON_GetObjectItem(temp0, "lcd_standby_time");
			system->lcd_standby_time_json = temp1;
		}
	}

exit:
	return ret;
}

/**
 * record_set_init - initial the setting for system
 * @param: lb_obj_t object pointer.
 *
 * This function initial the setting for system
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 fileexp_set_init(void)
{
	lb_int32 ret = 0;

	/* capture it from customer path first */
	fxp_config.root_json = lb_open_cfg_file(CUSTOMER_CONFIG_PATH);
	if (fxp_config.root_json == NULL) {
		/* capture it from original path again */
		fxp_config.root_json = lb_open_cfg_file(ORIGINAL_CONFIG_PATH);
		/* capture it failed only exit */
		if (fxp_config.root_json == NULL) {
			APP_LOG_W("failed\n");
			ret = -1;
			goto exit;
		}
	}

	fxp_config.system_json = cJSON_GetObjectItem(fxp_config.root_json, "system");
	RT_ASSERT(fxp_config.system_json != NULL);
	RT_ASSERT(fxp_config.system_json->type == cJSON_Array);

	ret = system_array_init(&fxp_config.system, fxp_config.system_json);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	return ret;
exit:
	if (fxp_config.root_json) {
		lb_exit_cfg_file(fxp_config.root_json);
		/* root_json will release inside upper interface */
		fxp_config.root_json = NULL;
		/* record_json will release inside upper interface */
		fxp_config.system_json = NULL;
	}

	return ret;
}

/**
 * fileexp_set_exit - exit the setting for system
 * @param: lb_obj_t object pointer.
 *
 * This function exit the setting for system to remove json from memory
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 fileexp_set_exit(void)
{
	lb_int32 ret = 0;

	RT_ASSERT(fxp_config.root_json != NULL);

	lb_exit_cfg_file(fxp_config.root_json);
	/* root_json will release inside upper interface */
	fxp_config.root_json = NULL;
	/* record_json will release inside upper interface */
	fxp_config.system_json = NULL;

	return ret;
}

