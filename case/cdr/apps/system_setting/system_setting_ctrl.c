/*
 * system_setting_ctrl.c - system control of setting code for LomboTech
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
#include <time.h>
#include <rtthread.h>
#include "app_manage.h"
#include "mod_manage.h"
#include "cJSON.h"
#include "lb_types.h"
#include "lb_common.h"
#include "lb_gal_common.h"
#include "system_setting_cfg.h"
#include "system_setting_ctrl.h"
#include "system_set_dev.h"

lb_obj_t *bright_obj;
lb_obj_t *volume_obj;
lb_obj_t *standby_obj;
lb_obj_t *keytone_obj;
lb_obj_t *language_obj;
lb_obj_t *date_time_obj;
lb_obj_t *format_obj;
lb_obj_t *version_obj;
lb_obj_t *factory_obj;

const char string[31][32] = {
	"STR_SETTINGS_DTIME_01",
	"STR_SETTINGS_DTIME_02",
	"STR_SETTINGS_DTIME_03",
	"STR_SETTINGS_DTIME_04",
	"STR_SETTINGS_DTIME_05",
	"STR_SETTINGS_DTIME_06",
	"STR_SETTINGS_DTIME_07",
	"STR_SETTINGS_DTIME_08",
	"STR_SETTINGS_DTIME_09",
	"STR_SETTINGS_DTIME_10",
	"STR_SETTINGS_DTIME_11",
	"STR_SETTINGS_DTIME_12",
	"STR_SETTINGS_DTIME_13",
	"STR_SETTINGS_DTIME_14",
	"STR_SETTINGS_DTIME_15",
	"STR_SETTINGS_DTIME_16",
	"STR_SETTINGS_DTIME_17",
	"STR_SETTINGS_DTIME_18",
	"STR_SETTINGS_DTIME_19",
	"STR_SETTINGS_DTIME_20",
	"STR_SETTINGS_DTIME_21",
	"STR_SETTINGS_DTIME_22",
	"STR_SETTINGS_DTIME_23",
	"STR_SETTINGS_DTIME_24",
	"STR_SETTINGS_DTIME_25",
	"STR_SETTINGS_DTIME_26",
	"STR_SETTINGS_DTIME_27",
	"STR_SETTINGS_DTIME_28",
	"STR_SETTINGS_DTIME_29",
	"STR_SETTINGS_DTIME_30",
	"STR_SETTINGS_DTIME_31",
};

lb_int32 brighten_init(void *param)
{
	lb_img_t *pproperty;
	lb_int32 ret = -1;
	lb_uint32 value = 0;
	lb_uint32 idx = 0;

	bright_obj = (lb_obj_t *)param;
	if (NULL == bright_obj) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	pproperty = bright_obj->property;
	if (NULL == pproperty) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	ret = get_backlight_value(&value);
	if (ret != 0)
		return ret;

	ret = backlight_value_to_idx(value, &idx);
	if (ret != 0)
		return ret;

	pproperty->comms.in_which_btn = idx;

	return SUCCESS;
}

lb_int32 brighten_exit(void *param)
{
	lb_obj_t *lb_obj;
	lb_img_t *pproperty;

	lb_obj = (lb_obj_t *)param;
	if (NULL == lb_obj) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	pproperty = lb_obj->property;
	if (NULL == pproperty) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	return SUCCESS;
}

/**
 * brighten_resp - modify the brigntness via response
 * @param: lb_obj_t object pointer.
 *
 * This function modify the brigntness via response
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 brighten_resp(void *param)
{
	lb_int32 ret = -1;
	lb_uint32 value = 0;
	lb_uint32 idx = 0;
	void *btn = NULL;

	if (NULL == param) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	btn = param;

	if (NULL == bright_obj) {
		APP_LOG_W("failed\n");
		return FAIL;
	}
	ret = lb_gal_list_get_btn_free_num(btn, &idx);
	if (ret != 0)
		return ret;

	lb_gal_update_img(NULL,
		bright_obj->pext,
		LB_IMG_UPD_PAR,
		idx,
		btn);

	ret = backlight_idx_to_value(idx, &value);
	if (ret != 0)
		return ret;

	ret = set_backlight_value(value);
	if (ret != 0)
		return ret;

	return SUCCESS;
}

lb_int32 volume_init(void *param)
{
	lb_img_t *pproperty;
	lb_int32 ret = -1;
	lb_uint32 value = 0;
	lb_uint32 idx = 0;

	volume_obj = (lb_obj_t *)param;
	if (NULL == volume_obj) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	pproperty = volume_obj->property;
	if (NULL == pproperty) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	ret = get_volume_value(&value);
	if (ret != 0)
		return ret;

	ret = volume_value_to_idx(value, &idx);
	if (ret != 0)
		return ret;

	pproperty->comms.in_which_btn = idx;

	return SUCCESS;
}

lb_int32 volume_exit(void *param)
{
	lb_obj_t *lb_obj;
	lb_img_t *pproperty;

	lb_obj = (lb_obj_t *)param;
	if (NULL == lb_obj) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	pproperty = lb_obj->property;
	if (NULL == pproperty) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	return SUCCESS;
}

/**
 * volume_resp - modify the volume via response
 * @param: lb_obj_t object pointer.
 *
 * This function modify the volume via response
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 volume_resp(void *param)
{
	lb_int32 ret = -1;
	lb_uint32 value = 0;
	lb_uint32 idx = 0;
	void *btn = NULL;

	if (NULL == param) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	btn = param;

	if (NULL == volume_obj) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	ret = lb_gal_list_get_btn_free_num(btn, &idx);
	if (ret != 0)
		return ret;

	lb_gal_update_img(NULL,
		volume_obj->pext,
		LB_IMG_UPD_PAR,
		idx,
		btn);

	ret = volume_idx_to_value(idx, &value);
	if (ret != 0)
		return ret;

	ret = set_volume_value(value);
	if (ret != 0)
		return ret;

	return SUCCESS;
}

lb_int32 standby_init(void *param)
{
	lb_img_t *pproperty;
	lb_int32 ret = -1;
	lb_uint32 value = 0;
	lb_uint32 idx = 0;

	standby_obj = (lb_obj_t *)param;
	if (NULL == standby_obj) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	pproperty = standby_obj->property;
	if (NULL == pproperty) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	ret = get_standby_value(&value);
	if (ret != 0)
		return ret;

	ret = standby_value_to_idx(value, &idx);
	if (ret != 0)
		return ret;

	pproperty->comms.in_which_btn = idx;

	return SUCCESS;
}

lb_int32 standby_exit(void *param)
{
	lb_obj_t *lb_obj;
	lb_img_t *pproperty;

	lb_obj = (lb_obj_t *)param;
	if (NULL == lb_obj) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	pproperty = lb_obj->property;
	if (NULL == pproperty) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	return SUCCESS;
}

/**
 * volume_resp - modify the standby time via response
 * @param: lb_obj_t object pointer.
 *
 * This function modify the standby time via response
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 standby_resp(void *param)
{
	lb_int32 ret = -1;
	lb_uint32 value = 0;
	lb_uint32 idx = 0;
	void *btn = NULL;

	if (NULL == param) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	btn = param;

	if (NULL == standby_obj) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	ret = lb_gal_list_get_btn_free_num(btn, &idx);
	if (ret != 0)
		return ret;

	lb_gal_update_img(NULL,
		standby_obj->pext,
		LB_IMG_UPD_PAR,
		idx,
		btn);

	ret = standby_idx_to_value(idx, &value);
	if (ret != 0)
		return ret;

	ret = set_standby_value(value);
	if (ret != 0)
		return ret;

	return SUCCESS;
}

lb_int32 keytone_init(void *param)
{
	lb_int8 sw;
	lb_int32 ret = -1;
	lb_obj_t *lb_obj;
	lb_imgbtn_t *pproperty;

	lb_obj = (lb_obj_t *)param;
	if (NULL == lb_obj) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	pproperty = lb_obj->property;
	if (NULL == pproperty) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	ret = get_keytone_sw(&sw);
	if (ret != 0)
		return ret;

	if (sw == 0) {
		strcpy(pproperty->rel_img.p_img_src,
			pproperty->rel_img.src_list[0]);
		strcpy(pproperty->pr_img.p_img_src,
			pproperty->pr_img.src_list[0]);
	} else {
		strcpy(pproperty->rel_img.p_img_src,
			pproperty->rel_img.src_list[1]);
		strcpy(pproperty->pr_img.p_img_src,
			pproperty->pr_img.src_list[1]);
	}

	return SUCCESS;
}

lb_int32 keytone_exit(void *param)
{
	if (NULL == param) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	return SUCCESS;
}

/**
 * keytone_resp - switch keytone to on or off via response
 * @param: lb_obj_t object pointer.
 *
 * This function switch keytone to on or off via response
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 keytone_resp(void *param)
{
	lb_int8 sw;
	lb_int32 ret;
	lb_imgbtn_t *property;
	void *obj;

	if (NULL == param) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	get_keytone_sw(&sw);

	if (sw == 0)
		sw = 1;
	else
		sw = 0;

	set_keytone_sw(sw);

	ret = lb_view_get_obj_property_by_id(211, (void *)&property);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		return FAIL;
	}
	ret = lb_view_get_obj_ext_by_id(211, (void *)&obj);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	lb_gal_update_imgbtn(property, obj, LB_IMGBTN_UPD_SRC, sw, NULL);

	return SUCCESS;
}

lb_int32 language_init(void *param)
{
	lb_img_t *pproperty;
	lb_int32 ret = -1;
	lb_uint32 idx = 0;

	language_obj = (lb_obj_t *)param;
	if (NULL == language_obj) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	pproperty = language_obj->property;
	if (NULL == pproperty) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

#if 0
	ret = get_language_string((void *)&string);
	if (ret != 0)
		return ret;

	ret = language_string_to_idx((void *)string, &idx);
	if (ret != 0)
		return ret;
#else
	ret = get_language_idx(&idx);
	if (ret != 0)
		return ret;
#endif

	pproperty->comms.in_which_btn = idx;

	return SUCCESS;
}

lb_int32 language_exit(void *param)
{
	lb_obj_t *lb_obj;
	lb_img_t *pproperty;

	lb_obj = (lb_obj_t *)param;
	if (NULL == lb_obj) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	pproperty = lb_obj->property;
	if (NULL == pproperty) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	return SUCCESS;
}

/**
 * language_resp - select language via response
 * @param: lb_obj_t object pointer.
 *
 * This function select language via response
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 language_resp(void *param)
{
	lb_int32 ret = -1;
	lb_uint32 idx = 0;
	void *btn = NULL;

	if (NULL == param) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	btn = param;

	if (NULL == language_obj) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	ret = lb_gal_list_get_btn_free_num(btn, &idx);
	if (ret != 0)
		return ret;

	lb_gal_update_img(NULL,
		language_obj->pext,
		LB_IMG_UPD_PAR,
		idx,
		btn);

#if 0
	ret = language_idx_to_string(idx, (void *)&string);
	if (ret != 0)
		return ret;

	ret = set_language_string(string);
	if (ret != 0)
		return ret;
#else
	ret = set_language_idx(idx);
	if (ret != 0)
		return ret;
#endif

	return SUCCESS;
}

lb_int32 fastboot_init(void *param)
{
	lb_int32 sw;
	lb_obj_t *lb_obj;
	lb_imgbtn_t *pproperty;

	lb_obj = (lb_obj_t *)param;
	if (NULL == lb_obj) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	pproperty = lb_obj->property;
	if (NULL == pproperty) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	get_fastboot_sw(&sw);

	if (sw == 0) {
		strcpy(pproperty->rel_img.p_img_src,
		pproperty->rel_img.src_list[0]);
		strcpy(pproperty->pr_img.p_img_src,
		pproperty->pr_img.src_list[0]);
	} else {
		strcpy(pproperty->rel_img.p_img_src,
		pproperty->rel_img.src_list[1]);
		strcpy(pproperty->pr_img.p_img_src,
		pproperty->pr_img.src_list[1]);
	}

	return SUCCESS;
}

lb_int32 fastboot_exit(void *param)
{
	if (NULL == param) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	return SUCCESS;
}

/**
 * keytone_resp - switch keytone to on or off via response
 * @param: lb_obj_t object pointer.
 *
 * This function switch keytone to on or off via response
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 fastboot_resp(void *param)
{
	lb_int32 sw;
	lb_int32 ret;
	lb_imgbtn_t *property;
	void *obj;

	if (NULL == param) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	get_fastboot_sw(&sw);

	if (sw == 0)
		sw = 1;
	else
		sw = 0;

	set_fastboot_sw(sw);

	ret = lb_view_get_obj_property_by_id(231, (void *)&property);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		return FAIL;
	}
	ret = lb_view_get_obj_ext_by_id(231, (void *)&obj);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	lb_gal_update_imgbtn(property, obj, LB_IMGBTN_UPD_SRC, sw, NULL);

	return SUCCESS;
}

lb_int32 rearmirr_init(void *param)
{
	lb_int32 sw;
	lb_obj_t *lb_obj;
	lb_imgbtn_t *pproperty;

	lb_obj = (lb_obj_t *)param;
	if (NULL == lb_obj) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	pproperty = lb_obj->property;
	if (NULL == pproperty) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	get_rearmirr_sw(&sw);

	if (sw == 0) {
		strcpy(pproperty->rel_img.p_img_src,
		pproperty->rel_img.src_list[0]);
		strcpy(pproperty->pr_img.p_img_src,
		pproperty->pr_img.src_list[0]);
	} else {
		strcpy(pproperty->rel_img.p_img_src,
		pproperty->rel_img.src_list[1]);
		strcpy(pproperty->pr_img.p_img_src,
		pproperty->pr_img.src_list[1]);
	}

	return SUCCESS;
}

lb_int32 rearmirr_exit(void *param)
{
	if (NULL == param) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	return SUCCESS;
}

/**
 * keytone_resp - switch keytone to on or off via response
 * @param: lb_obj_t object pointer.
 *
 * This function switch keytone to on or off via response
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 rearmirr_resp(void *param)
{
	lb_int32 sw;
	lb_int32 ret;
	lb_imgbtn_t *property;
	void *obj;

	if (NULL == param) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	get_rearmirr_sw(&sw);

	if (sw == 0)
		sw = 1;
	else
		sw = 0;

	set_rearmirr_sw(sw);

	ret = lb_view_get_obj_property_by_id(233, (void *)&property);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		return FAIL;
	}
	ret = lb_view_get_obj_ext_by_id(233, (void *)&obj);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	lb_gal_update_imgbtn(property, obj, LB_IMGBTN_UPD_SRC, sw, NULL);

	return SUCCESS;
}

lb_int32 accpower_init(void *param)
{
	lb_int32 sw;
	lb_obj_t *lb_obj;
	lb_imgbtn_t *pproperty;

	lb_obj = (lb_obj_t *)param;
	if (NULL == lb_obj) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	pproperty = lb_obj->property;
	if (NULL == pproperty) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	get_accpower_sw(&sw);

	if (sw == 0) {
		strcpy(pproperty->rel_img.p_img_src,
		pproperty->rel_img.src_list[0]);
		strcpy(pproperty->pr_img.p_img_src,
		pproperty->pr_img.src_list[0]);
	} else {
		strcpy(pproperty->rel_img.p_img_src,
		pproperty->rel_img.src_list[1]);
		strcpy(pproperty->pr_img.p_img_src,
		pproperty->pr_img.src_list[1]);
	}

	return SUCCESS;
}

lb_int32 accpower_exit(void *param)
{
	if (NULL == param) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	return SUCCESS;
}

/**
 * keytone_resp - switch keytone to on or off via response
 * @param: lb_obj_t object pointer.
 *
 * This function switch keytone to on or off via response
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 accpower_resp(void *param)
{
	lb_int32 sw;
	lb_int32 ret;
	lb_imgbtn_t *property;
	void *obj;

	if (NULL == param) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	get_accpower_sw(&sw);

	if (sw == 0)
		sw = 1;
	else
		sw = 0;

	set_accpower_sw(sw);

	ret = lb_view_get_obj_property_by_id(235, (void *)&property);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		return FAIL;
	}
	ret = lb_view_get_obj_ext_by_id(235, (void *)&obj);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	lb_gal_update_imgbtn(property, obj, LB_IMGBTN_UPD_SRC, sw, NULL);

	return SUCCESS;
}

static lb_int32 year_init(void *param)
{
	lb_int32 ret = -1;
	lb_obj_t *dt_year = NULL;
	lb_roller_t *pproperty = NULL;
	struct tm *p_tm; /* time variable */
	time_t now;

	now = time(RT_NULL);
	p_tm = localtime(&now);

	dt_year = (lb_obj_t *)param;
	if (NULL == dt_year) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	pproperty = dt_year->property;
	if (NULL == pproperty) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	pproperty->selected = (p_tm->tm_year + 1900 - 2025);//2019

	ret = 0;
	return ret;
}

static lb_int32 month_init(void *param)
{
	lb_int32 ret = -1;
	lb_obj_t *dt_year = NULL;
	lb_roller_t *pproperty = NULL;
	struct tm *p_tm; /* time variable */
	time_t now;

	now = time(RT_NULL);
	p_tm = localtime(&now);

	dt_year = (lb_obj_t *)param;
	if (NULL == dt_year) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	pproperty = dt_year->property;
	if (NULL == pproperty) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	pproperty->selected = (p_tm->tm_mon + 1 - 1);

	ret = 0;
	return ret;
}

static lb_int32 day_init(void *param)
{
	lb_int32 ret = -1;
	lb_obj_t *dt_year = NULL;
	lb_roller_t *pproperty = NULL;
	struct tm *p_tm; /* time variable */
	time_t now;
	lb_uint8 flag = 0, day_change = 0, option_num = 0;
	lb_uint8 i = 0;
	lb_uint32 string_id[31];

	now = time(RT_NULL);
	p_tm = localtime(&now);

	dt_year = (lb_obj_t *)param;
	if (NULL == dt_year) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	pproperty = dt_year->property;
	if (NULL == pproperty) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	flag = get_leap_month(&day_change);
	if (0 == flag)
		option_num = 29;
	else if (1 == flag)
		option_num = 28;
	else if (2 == flag)
		option_num = 31;
	else if (3 == flag)
		option_num = 30;

	for (i = 0; i < option_num; i++)
		string_id[i] = elang_get_string_id_josn(string[i]);

	lb_gal_roller_set_options(pproperty, string_id, option_num);
	pproperty->selected = (p_tm->tm_mday - 1);

	ret = 0;
	return ret;
}

static lb_int32 hour_init(void *param)
{
	lb_int32 ret = -1;
	lb_obj_t *dt_year = NULL;
	lb_roller_t *pproperty = NULL;
	struct tm *p_tm; /* time variable */
	time_t now;

	now = time(RT_NULL);
	p_tm = localtime(&now);

	dt_year = (lb_obj_t *)param;
	if (NULL == dt_year) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	pproperty = dt_year->property;
	if (NULL == pproperty) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	pproperty->selected = p_tm->tm_hour;

	ret = 0;
	return ret;
}

static lb_int32 minute_init(void *param)
{
	lb_int32 ret = -1;
	lb_obj_t *dt_year = NULL;
	lb_roller_t *pproperty = NULL;
	struct tm *p_tm; /* time variable */
	time_t now;

	now = time(RT_NULL);
	p_tm = localtime(&now);

	dt_year = (lb_obj_t *)param;
	if (NULL == dt_year) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	pproperty = dt_year->property;
	if (NULL == pproperty) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	pproperty->selected = p_tm->tm_min;

	ret = 0;
	return ret;
}

static lb_int32 year_resp(void *param)
{
	lb_int32 ret = -1;
	lb_roller_msg_data_t *roller_msg = NULL;

	roller_msg = param;
	if (NULL == param) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	dt_idx_to_ymd(roller_msg->sel_opt_id, -1, -1);
	update_day_num();

	ret = 0;
	return ret;
}

static lb_int32 month_resp(void *param)
{
	lb_int32 ret = -1;
	lb_roller_msg_data_t *roller_msg = NULL;

	roller_msg = param;
	if (NULL == param) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	dt_idx_to_ymd(-1, roller_msg->sel_opt_id, -1);
	update_day_num();

	ret = 0;
	return ret;
}

static lb_int32 day_resp(void *param)
{
	lb_int32 ret = -1;
	lb_roller_msg_data_t *roller_msg = NULL;

	roller_msg = param;
	if (NULL == param) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	dt_idx_to_ymd(-1, -1, roller_msg->sel_opt_id);

	ret = 0;
	return ret;
}

static lb_int32 hour_resp(void *param)
{
	lb_int32 ret = -1;
	lb_roller_msg_data_t *roller_msg = NULL;

	roller_msg = param;
	if (NULL == param) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	dt_idx_to_hms(roller_msg->sel_opt_id, -1, -1);

	ret = 0;
	return ret;
}

static lb_int32 minute_resp(void *param)
{
	lb_int32 ret = -1;
	lb_roller_msg_data_t *roller_msg = NULL;

	roller_msg = param;
	if (NULL == param) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	dt_idx_to_hms(-1, roller_msg->sel_opt_id, -1);

	ret = 0;
	return ret;
}

lb_int32 date_time_init(void *param)
{
	lb_int32 ret = 0;

	ret = get_date_time();
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

exit:
	return ret;
}

lb_int32 date_time_exit(void *param)
{
	lb_int32 ret = 0;

	ret = set_date_time();
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	return ret;
exit:
	return ret;
}

/**
 * date_time_resp - modify the date and time via rolloer
 * @param: lb_obj_t object pointer.
 *
 * This function modify the date and time via rolloer
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 date_time_resp(void *param)
{
	if (NULL == param) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	return SUCCESS;
}

lb_int32 format_init(void *param)
{
	if (NULL == param) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	lb_view_set_require_func(lb_view_get_parent(),
		(void *)format_sd_card);
	lb_view_set_require_param(lb_view_get_parent(),
		(void *)NULL);

	return SUCCESS;
}

lb_int32 format_exit(void *param)
{
	if (NULL == param) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	return SUCCESS;
}

/**
 * date_time_resp - format the card through interface
 * @param: lb_obj_t object pointer.
 *
 * This function format the card through interface
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 format_resp(void *param)
{
	if (NULL == param) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	return SUCCESS;
}

lb_int32 version_init(void *param)
{
	lb_int32 ret = 0;
	lb_obj_t *label = NULL;
	lb_label_t *property = NULL;

	if (NULL == param) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	label = (lb_obj_t *)param;
	if (NULL == label) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	property = (lb_label_t *)label->property;
	if (NULL == property) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	if (NULL == property->txt) {
		property->txt = malloc(32);
		memset(property->txt, 0x00, 32);
	}

	ret = get_version(property->txt);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	return SUCCESS;
}

lb_int32 version_exit(void *param)
{
	lb_obj_t *label = NULL;
	lb_label_t *property = NULL;

	if (NULL == param) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	label = (lb_obj_t *)param;
	if (NULL == label) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	property = (lb_label_t *)label->property;
	if (NULL == property) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	if (property->txt) {
		free(property->txt);
		property->txt = NULL;
	}

	return SUCCESS;
}

lb_int32 version_resp(void *param)
{
	return SUCCESS;
}

lb_int32 factory_init(void *param)
{
	if (NULL == param) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	lb_view_set_require_func(lb_view_get_parent(),
		(void *)restore_factory_setting);
	lb_view_set_require_param(lb_view_get_parent(),
		(void *)NULL);

	return SUCCESS;
}

lb_int32 factory_exit(void *param)
{
	if (NULL == param) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	return SUCCESS;
}

/**
 * version_resp - restore the setting to factory state
 * @param: lb_obj_t object pointer.
 *
 * This function restore the setting to factory state
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 factory_resp(void *param)
{
	if (NULL == param) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	return SUCCESS;
}

lb_int32 init_funcs(void)
{
	lb_int32 err = SUCCESS;

	err |= lb_fmngr_reg_init_func("brighten_init", brighten_init);
	err |= lb_fmngr_reg_exit_func("brighten_exit", brighten_exit);

	err |= lb_fmngr_reg_init_func("volume_init", volume_init);
	err |= lb_fmngr_reg_exit_func("volume_exit", volume_exit);

	err |= lb_fmngr_reg_init_func("standby_init", standby_init);
	err |= lb_fmngr_reg_exit_func("standby_exit", standby_exit);

	err |= lb_fmngr_reg_init_func("keytone_init", keytone_init);
	err |= lb_fmngr_reg_exit_func("keytone_exit", keytone_exit);

	err |= lb_fmngr_reg_init_func("language_init", language_init);
	err |= lb_fmngr_reg_exit_func("language_exit", language_exit);

	err |= lb_fmngr_reg_init_func("date_time_init", date_time_init);
	err |= lb_fmngr_reg_exit_func("date_time_exit", date_time_exit);

	err |= lb_fmngr_reg_init_func("format_init", format_init);
	err |= lb_fmngr_reg_exit_func("format_exit", format_exit);

	err |= lb_fmngr_reg_init_func("version_init", version_init);
	err |= lb_fmngr_reg_exit_func("version_exit", version_exit);

	err |= lb_fmngr_reg_init_func("factory_init", factory_init);
	err |= lb_fmngr_reg_exit_func("factory_exit", factory_exit);

	err |= lb_fmngr_reg_init_func("fastboot_init", fastboot_init);
	err |= lb_fmngr_reg_exit_func("fastboot_exit", fastboot_exit);

	err |= lb_fmngr_reg_init_func("rearmirr_init", rearmirr_init);
	err |= lb_fmngr_reg_exit_func("rearmirr_exit", rearmirr_exit);

	err |= lb_fmngr_reg_init_func("accpower_init", accpower_init);
	err |= lb_fmngr_reg_exit_func("accpower_exit", accpower_exit);

	err |= lb_fmngr_reg_init_func("date_time_init", date_time_init);
	err |= lb_fmngr_reg_exit_func("date_time_exit", date_time_exit);

	err |= lb_fmngr_reg_init_func("year_init", year_init);
	err |= lb_fmngr_reg_init_func("month_init", month_init);
	err |= lb_fmngr_reg_init_func("day_init", day_init);
	err |= lb_fmngr_reg_init_func("hour_init", hour_init);
	err |= lb_fmngr_reg_init_func("minute_init", minute_init);

	return err;
}

lb_int32 uninit_funcs(void)
{
	lb_int32 err = SUCCESS;

	err |= lb_fmngr_unreg_init_func(brighten_init);
	err |= lb_fmngr_unreg_exit_func(brighten_exit);

	err |= lb_fmngr_unreg_init_func(volume_init);
	err |= lb_fmngr_unreg_exit_func(volume_exit);

	err |= lb_fmngr_unreg_init_func(standby_init);
	err |= lb_fmngr_unreg_exit_func(standby_exit);

	err |= lb_fmngr_unreg_init_func(keytone_init);
	err |= lb_fmngr_unreg_exit_func(keytone_exit);

	err |= lb_fmngr_unreg_init_func(language_init);
	err |= lb_fmngr_unreg_exit_func(language_exit);

	err |= lb_fmngr_unreg_init_func(date_time_init);
	err |= lb_fmngr_unreg_exit_func(date_time_exit);

	err |= lb_fmngr_unreg_init_func(format_init);
	err |= lb_fmngr_unreg_exit_func(format_exit);

	err |= lb_fmngr_unreg_init_func(version_init);
	err |= lb_fmngr_unreg_exit_func(version_exit);

	err |= lb_fmngr_unreg_init_func(factory_init);
	err |= lb_fmngr_unreg_exit_func(factory_exit);

	err |= lb_fmngr_unreg_init_func(fastboot_init);
	err |= lb_fmngr_unreg_exit_func(fastboot_exit);

	err |= lb_fmngr_unreg_init_func(rearmirr_init);
	err |= lb_fmngr_unreg_exit_func(rearmirr_exit);

	err |= lb_fmngr_unreg_init_func(accpower_init);
	err |= lb_fmngr_unreg_exit_func(accpower_exit);

	err |= lb_fmngr_unreg_init_func(date_time_init);
	err |= lb_fmngr_unreg_exit_func(date_time_exit);

	err |= lb_fmngr_unreg_init_func(year_init);
	err |= lb_fmngr_unreg_init_func(month_init);
	err |= lb_fmngr_unreg_init_func(day_init);
	err |= lb_fmngr_unreg_init_func(hour_init);
	err |= lb_fmngr_unreg_init_func(minute_init);

	return err;
}

lb_int32 resp_funcs(void)
{
	lb_int32 err = SUCCESS;

	err |= lb_reg_resp_msg_func(LB_MSG_SYSCFG_BRIGHTEN, brighten_resp);
	err |= lb_reg_resp_msg_func(LB_MSG_SYSCFG_VOLUME, volume_resp);
	err |= lb_reg_resp_msg_func(LB_MSG_SYSCFG_STANDBY, standby_resp);
	err |= lb_reg_resp_msg_func(LB_MSG_SYSCFG_KEYTONE, keytone_resp);
	err |= lb_reg_resp_msg_func(LB_MSG_SYSCFG_LANGUAGE, language_resp);
	err |= lb_reg_resp_msg_func(LB_MSG_SYSCFG_DTIME, date_time_resp);
	err |= lb_reg_resp_msg_func(LB_MSG_SYSCFG_FORMAT, format_resp);
	err |= lb_reg_resp_msg_func(LB_MSG_SYSCFG_VERSION, version_resp);
	err |= lb_reg_resp_msg_func(LB_MSG_SYSCFG_FACTORY, factory_resp);
	err |= lb_reg_resp_msg_func(LB_MSG_SYSCFG_FASTBOOT, fastboot_resp);
	err |= lb_reg_resp_msg_func(LB_MSG_SYSCFG_REARMIR, rearmirr_resp);
	err |= lb_reg_resp_msg_func(LB_MSG_SYSCFG_ACCPOWER, accpower_resp);

	err |= lb_reg_resp_msg_func(LB_MSG_SEL_YEAR, year_resp);
	err |= lb_reg_resp_msg_func(LB_MSG_SEL_MONTH, month_resp);
	err |= lb_reg_resp_msg_func(LB_MSG_SEL_DAY, day_resp);
	err |= lb_reg_resp_msg_func(LB_MSG_SEL_HOUR, hour_resp);
	err |= lb_reg_resp_msg_func(LB_MSG_SEL_MINTUE, minute_resp);

	return err;
}

lb_int32 unresp_funcs(void)
{
	lb_int32 err = SUCCESS;

	err |= lb_unreg_resp_msg_func(brighten_resp);
	err |= lb_unreg_resp_msg_func(volume_resp);
	err |= lb_unreg_resp_msg_func(standby_resp);
	err |= lb_unreg_resp_msg_func(keytone_resp);
	err |= lb_unreg_resp_msg_func(language_resp);
	err |= lb_unreg_resp_msg_func(date_time_resp);
	err |= lb_unreg_resp_msg_func(format_resp);
	err |= lb_unreg_resp_msg_func(version_resp);
	err |= lb_unreg_resp_msg_func(factory_resp);
	err |= lb_unreg_resp_msg_func(fastboot_resp);
	err |= lb_unreg_resp_msg_func(rearmirr_resp);
	err |= lb_unreg_resp_msg_func(accpower_resp);

	err |= lb_unreg_resp_msg_func(year_resp);
	err |= lb_unreg_resp_msg_func(month_resp);
	err |= lb_unreg_resp_msg_func(day_resp);
	err |= lb_unreg_resp_msg_func(hour_resp);
	err |= lb_unreg_resp_msg_func(minute_resp);

	return err;
}
