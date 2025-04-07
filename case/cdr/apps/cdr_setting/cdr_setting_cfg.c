/*
 * cdr_setting_cfg.c - cdr config of settingr code for LomboTech
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
#include "cdr_setting_cfg.h"

cdr_config_t cdr_config;

/**
 * you could extend all field parsing just by configuring "record_table"
 * there is no need modify the function of parsing every time you know guy
 */
#ifdef PARSE_TABLE
static char *record_table[] = {
	"record_duration",
	"record_resolution",
	"gsensor_sensity",
	"mute_enable",
	"watermark_time_enable",
	"watermark_logo_enable",
	"park_monitor",
	"interval_record"
};
#endif

/**
 * record_array_init - initial the array from record
 * @param: lb_obj_t object pointer.
 *
 * This function initial the array from record and each elem must be cJSON_Object
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
static lb_int32 record_array_init(record_t *record, cJSON *para)
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
		printf("[%s,%d,failed]\n", __FILE__, __LINE__);
		ret = -1;
		goto exit;
	}

#ifdef PARSE_TABLE
	/* must make sure object from json and object from program are identical */
	tb_size = sizeof(record_table) / sizeof(char *);
	RT_ASSERT(js_size == tb_size);
#endif

	/**
	 * you could extend all field parsing just by configuring "record_table"
	 * there is no need modify the function of parsing every time you know guy
	 */
	for (i = 0; i < js_size; i++) {
		temp0 = cJSON_GetArrayItem(para, i);
		RT_ASSERT(temp0 != NULL);
		RT_ASSERT(temp0->type == cJSON_Object);

		if (cJSON_GetObjectItem(temp0, "record_duration")) {
			temp1 = cJSON_GetObjectItem(temp0, "record_duration");
			RT_ASSERT(temp1 != NULL);
			record->record_duration_json = temp1;
		} else if (cJSON_GetObjectItem(temp0, "record_resolution")) {
			temp1 = cJSON_GetObjectItem(temp0, "record_resolution");
			RT_ASSERT(temp1 != NULL);
			record->record_resolution_json = temp1;
		} else if (cJSON_GetObjectItem(temp0, "gsensor_sensity")) {
			temp1 = cJSON_GetObjectItem(temp0, "gsensor_sensity");
			RT_ASSERT(temp1 != NULL);
			record->gsensor_sensity_json = temp1;
		} else if (cJSON_GetObjectItem(temp0, "lcd_brightness")) {
			temp1 = cJSON_GetObjectItem(temp0, "lcd_brightness");
			RT_ASSERT(temp1 != NULL);
			record->lcd_brightness_json = temp1;
		}
		else if (cJSON_GetObjectItem(temp0, "mute_enable")) {
			temp1 = cJSON_GetObjectItem(temp0, "mute_enable");
			RT_ASSERT(temp1 != NULL);
			record->mute_enable_json = temp1;
		} else if (cJSON_GetObjectItem(temp0, "warn_tone")) {
			temp1 = cJSON_GetObjectItem(temp0, "warn_tone");
			RT_ASSERT(temp1 != NULL);
			record->warn_tone_json = temp1;
		}else if (cJSON_GetObjectItem(temp0, "watermark_time_enable")) {
			temp1 = cJSON_GetObjectItem(temp0, "watermark_time_enable");
			RT_ASSERT(temp1 != NULL);
			record->watermark_time_enable_json = temp1;
		} else if (cJSON_GetObjectItem(temp0, "watermark_logo_enable")) {
			temp1 = cJSON_GetObjectItem(temp0, "watermark_logo_enable");
			RT_ASSERT(temp1 != NULL);
			record->watermark_logo_enable_json = temp1;
		} else if (cJSON_GetObjectItem(temp0, "park_monitor")) {
			temp1 = cJSON_GetObjectItem(temp0, "park_monitor");
			RT_ASSERT(temp1 != NULL);
			record->park_monitor_json = temp1;
		} else if (cJSON_GetObjectItem(temp0, "interval_record")) {
			temp1 = cJSON_GetObjectItem(temp0, "interval_record");
			RT_ASSERT(temp1 != NULL);
			record->interval_record_json = temp1;
		}else if (cJSON_GetObjectItem(temp0, "language")) {
			temp1 = cJSON_GetObjectItem(temp0, "language");
			RT_ASSERT(temp1 != NULL);
			record->language_json = temp1;
		}
	}

exit:
	return ret;
}

/**
 * record_set_init - initial the setting for record
 * @param: lb_obj_t object pointer.
 *
 * This function initial the setting for record
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 record_set_init(void)
{
	lb_int32 ret = 0;

	/* capture it from customer path first */
	cdr_config.root_json = lb_open_cfg_file(CUSTOMER_CONFIG_PATH);
	if (cdr_config.root_json == NULL) {
		/* capture it from original path again */
		cdr_config.root_json = lb_open_cfg_file(ORIGINAL_CONFIG_PATH);
		/* capture it failed only exit */
		if (cdr_config.root_json == NULL) {
			printf("[%s,%d,failed]\n", __FILE__, __LINE__);
			ret = -1;
			goto exit;
		}
	}

	cdr_config.record_json = cJSON_GetObjectItem(cdr_config.root_json, "record");
	RT_ASSERT(cdr_config.record_json != NULL);
	RT_ASSERT(cdr_config.record_json->type == cJSON_Array);

	ret = record_array_init(&cdr_config.record, cdr_config.record_json);
	if (ret != 0) {
		printf("[%s,%d,failed]\n", __FILE__, __LINE__);
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
		/* record_json will release inside upper interface */
		cdr_config.record_json = NULL;
	}

	return ret;
}

/**
 * record_set_exit - exit the setting for record
 * @param: lb_obj_t object pointer.
 *
 * This function exit the setting for record to remove json from memory
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 record_set_exit(void)
{
	lb_int32 ret = 0;

	RT_ASSERT(cdr_config.root_json != NULL);

	lb_exit_cfg_file(cdr_config.root_json);
	/* root_json will release inside upper interface */
	cdr_config.root_json = NULL;
	/* record_json will release inside upper interface */
	cdr_config.record_json = NULL;

	return ret;
}

/**
 * record_set_save - save the setting for record
 * @param: lb_obj_t object pointer.
 *
 * This function save the setting for record to json
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 record_set_save(void)
{
	lb_int32 ret = 0;

	RT_ASSERT(cdr_config.root_json != NULL);

	/* we save it to json which is coming from customer path */
	ret = lb_save_cfg_file(CUSTOMER_CONFIG_PATH, cdr_config.root_json);
	printf("\nret_set===============================%d\n",ret);
  #if 1
    if(ret==-1){
		data_regionalization();
		 rt_thread_delay(20);
	    ret = lb_save_cfg_file(CUSTOMER_CONFIG_PATH, cdr_config.root_json);
	}else {
		cJSON *cfg = lb_open_cfg_file(CUSTOMER_CONFIG_PATH);
		if (cfg == NULL) {
		printf("\nagain_data\n");	
		data_regionalization();
		 rt_thread_delay(20);
	    ret = lb_save_cfg_file(CUSTOMER_CONFIG_PATH, cdr_config.root_json);
		}else{
		 lb_exit_cfg_file(cfg);	
		}
    }
  #endif
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
		printf("failed\n");
		ret = -1;
		goto exit;
	}

	cdr_config.record_json = cJSON_GetObjectItem
		(cdr_config.root_json, "record");
	RT_ASSERT(cdr_config.record_json != NULL);
	RT_ASSERT(cdr_config.record_json->type == cJSON_Array);

	ret = record_array_init(&cdr_config.record, cdr_config.record_json);
	if (ret != 0) {
		printf("failed\n");
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


