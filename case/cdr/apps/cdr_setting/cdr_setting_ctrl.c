/*
 * cdr_setting_ctrl.c - cdr control of setting code for LomboTech
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
#include "cJSON.h"
#include "lb_types.h"
#include "lb_common.h"
#include "lb_gal_common.h"

#include "cdr_setting_cfg.h"
#include "cdr_setting_ctrl.h"
#include "cdr_set_dev.h"


lb_obj_t *loop_record_obj;
lb_obj_t *record_reso_obj;
lb_obj_t *gsensor_sens_obj;
lb_obj_t *standby_obj;
lb_obj_t *language_obj;
lb_obj_t *bright_obj;





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




/**
 * loop_record_init - initial the duration obj to show
 * @param: lb_obj_t object pointer.
 *
 * This function initial the duration to show
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 loop_record_init(void *param)
{
	lb_img_t *pproperty;
	lb_int32 ret = 0;
	lb_int32 idx = 0;

	loop_record_obj = (lb_obj_t *)param;
	if (NULL == loop_record_obj) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	pproperty = loop_record_obj->property;
	if (NULL == pproperty) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

#if 0
	ret = get_loop_record_string(&string);
	if (ret != 0)
		return ret;

	ret = loop_record_string_to_idx(string, &idx);
	if (ret != 0)
		return ret;
#else
	ret = get_loop_record_idx(&idx);
	if (ret != 0)
		return ret;
#endif

	pproperty->comms.in_which_btn = idx;

	return SUCCESS;
}

/**
 * loop_record_exit - exit the duration obj maybe you can do sth you need
 * @param: lb_obj_t object pointer.
 *
 * This function  exit the duration obj maybe you can do sth you need
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 loop_record_exit(void *param)
{
	lb_obj_t *lb_obj;
	lb_img_t *pproperty;

	lb_obj = (lb_obj_t *)param;
	if (NULL == lb_obj) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	pproperty = lb_obj->property;
	if (NULL == pproperty) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	return SUCCESS;
}

/**
 * loop_record_resp - modify the cycle duration for recording
 * @param: lb_obj_t object pointer.
 *
 * This function modify thecycle duration for recording
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 loop_record_resp(void *param)
{
	lb_int32 ret = 0;
	lb_int32 idx = 0;
	void *btn = NULL;

	if (NULL == param) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}
	btn = param;

	if (NULL == loop_record_obj) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	ret = lb_gal_list_get_btn_free_num(btn, (lb_uint32 *)&idx);
	if (ret != 0)
		return ret;

	lb_gal_update_img(NULL,
		loop_record_obj->pext,
		LB_IMG_UPD_PAR,
		idx,
		btn);

#if 0
	ret = loop_record_idx_to_string(idx, &string);
	if (ret != 0)
		return ret;

	ret = set_loop_record_string(string);
	if (ret != 0)
		return ret;
#else
	ret = set_loop_record_idx(idx);
	if (ret != 0)
		return ret;
#endif

	return SUCCESS;
}

/**
 * record_reso_init - initial the resolution obj to show
 * @param: lb_obj_t object pointer.
 *
 * This function  initial the resolution obj to show
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 record_reso_init(void *param)
{
	lb_img_t *pproperty;
	lb_int32 ret = 0;
	lb_int32 idx = 0;

	record_reso_obj = (lb_obj_t *)param;
	if (NULL == record_reso_obj) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	pproperty = record_reso_obj->property;
	if (NULL == pproperty) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

#if 0
	ret = get_record_reso_string(&string);
	if (ret != 0)
		return ret;

	ret = record_reso_string_to_idx(string, &idx);
	if (ret != 0)
		return ret;
#else
	ret = get_record_reso_idx(&idx);
	if (ret != 0)
		return ret;
#endif

	pproperty->comms.in_which_btn = idx;

	return SUCCESS;
}

/**
 * record_reso_exit - exit the resolution obj maybe you can do sth you need
 * @param: lb_obj_t object pointer.
 *
 * This function  exit the resolution obj maybe you can do sth you need
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 record_reso_exit(void *param)
{
	lb_obj_t *lb_obj;
	lb_img_t *pproperty;

	lb_obj = (lb_obj_t *)param;
	if (NULL == lb_obj) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	pproperty = lb_obj->property;
	if (NULL == pproperty) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	return SUCCESS;
}

/**
 * record_reso_resp - modify the compress resolution for recording
 * @param: lb_obj_t object pointer.
 *
 * This function modify the compress resolution for recording
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 record_reso_resp(void *param)
{
	lb_int32 ret = 0;
	lb_int32 idx = 0;
	void *btn = NULL;

	if (NULL == param) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}
	btn = param;

	if (NULL == record_reso_obj) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	ret = lb_gal_list_get_btn_free_num(btn, (lb_uint32 *)&idx);
	if (ret != 0)
		return ret;

	lb_gal_update_img(NULL,
		record_reso_obj->pext,
		LB_IMG_UPD_PAR,
		idx,
		btn);

#if 0
	ret = record_reso_idx_to_string(idx, &string);
	if (ret != 0)
		return ret;

	ret = set_record_reso_string(string);
	if (ret != 0)
		return ret;
#else
	ret = set_record_reso_idx(idx);
	if (ret != 0)
		return ret;

#endif

	return SUCCESS;
}

/**
 * gsensor_sensity_init - initial the gsensor sensity obj to show for adas
 * @param: lb_obj_t object pointer.
 *
 * This function  initial the gsensor sensity obj to show for adas
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 gsensor_sensity_init(void *param)
{
	lb_img_t *pproperty;
	lb_int32 ret = 0;
	lb_int32 idx = 0;

	gsensor_sens_obj = (lb_obj_t *)param;
	if (NULL == gsensor_sens_obj) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	pproperty = gsensor_sens_obj->property;
	if (NULL == pproperty) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	ret = get_gsensor_sens_idx(&idx);
	if (ret != 0)
		return ret;

	pproperty->comms.in_which_btn = idx;

	return SUCCESS;
}

/**
 * gsensor_sensity_exit - exit the gsensor sensity obj maybe you can do sth you need
 * @param: lb_obj_t object pointer.
 *
 * This function  initial the gsensor sensity obj maybe you can do sth you need
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 gsensor_sensity_exit(void *param)
{
	lb_obj_t *lb_obj;
	lb_img_t *pproperty;

	lb_obj = (lb_obj_t *)param;
	if (NULL == lb_obj) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	pproperty = lb_obj->property;
	if (NULL == pproperty) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	return SUCCESS;
}

/**
 * gsensor_sensity_resp - modify the gsensor sensity for adas
 * @param: lb_obj_t object pointer.
 *
 * This function modify the gsensor sensity for adas
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 gsensor_sensity_resp(void *param)
{
	lb_int32 ret = 0;
	lb_int32 idx = 0;
	void *btn = NULL;

	if (NULL == param) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	btn = param;

	if (NULL == gsensor_sens_obj) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	ret = lb_gal_list_get_btn_free_num(btn, (lb_uint32 *)&idx);
	if (ret != 0)
		return ret;

	lb_gal_update_img(NULL,
		gsensor_sens_obj->pext,
		LB_IMG_UPD_PAR,
		idx,
		btn);

	ret = set_gsensor_sens_idx(idx);
	if (ret != 0)
		return ret;

	return SUCCESS;
}




lb_int32 lcd_brightness_init(void *param)
{
	lb_img_t *pproperty;
	lb_int32 ret = 0;
	lb_uint32 value = 0;
	lb_uint32 idx = 0;

	bright_obj = (lb_obj_t *)param;
	if (NULL == bright_obj) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	pproperty = bright_obj->property;
	if (NULL == pproperty) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
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

lb_int32 lcd_brightness_exit(void *param)
{
	lb_obj_t *lb_obj;
	lb_img_t *pproperty;

	lb_obj = (lb_obj_t *)param;
	if (NULL == lb_obj) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	pproperty = lb_obj->property;
	if (NULL == pproperty) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
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
lb_int32 lcd_brightness_resp(void *param)
{
	lb_int32 ret = 0;
	lb_uint32 value = 0;
	lb_uint32 idx = 0;
	void *btn = NULL;

	if (NULL == param) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	btn = param;

	if (NULL == bright_obj) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
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
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	pproperty = dt_year->property;
	if (NULL == pproperty) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	pproperty->selected = (p_tm->tm_year + 1900 - 2025); //2019

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
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	pproperty = dt_year->property;
	if (NULL == pproperty) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
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
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	pproperty = dt_year->property;
	if (NULL == pproperty) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
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
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	pproperty = dt_year->property;
	if (NULL == pproperty) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
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
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	pproperty = dt_year->property;
	if (NULL == pproperty) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	pproperty->selected = p_tm->tm_min;

	ret = 0;
	return ret;
}
lb_int32 date_time_init(void *param)
{
	lb_int32 ret = 0;

	ret = get_date_time();
	if (ret != 0) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
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
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		ret = -1;
		goto exit;
	}

	return ret;
exit:
	return ret;
}


static lb_int32 year_resp(void *param)
{
	lb_int32 ret = -1;
	lb_roller_msg_data_t *roller_msg = NULL;

	roller_msg = param;
	if (NULL == param) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	dt_idx_to_ymd(roller_msg->sel_opt_id, -1, -1);
	printf("roller_msg->sel_opt_id===%d",roller_msg->sel_opt_id);
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
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
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
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
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
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
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
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	dt_idx_to_hms(-1, roller_msg->sel_opt_id, -1);

	ret = 0;
	return ret;
}

lb_int32 language_init(void *param)
{
	lb_img_t *pproperty;
	lb_int32 ret = -1;
	lb_uint32 idx = 0;

	language_obj = (lb_obj_t *)param;
	if (NULL == language_obj) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	pproperty = language_obj->property;
	if (NULL == pproperty) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}


	ret = get_language_idx(&idx);
	if (ret != 0)
		return ret;


	pproperty->comms.in_which_btn = idx;

	return SUCCESS;
}


lb_int32 language_exit(void *param)
{
	lb_obj_t *lb_obj;
	lb_img_t *pproperty;

	lb_obj = (lb_obj_t *)param;
	if (NULL == lb_obj) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	pproperty = lb_obj->property;
	if (NULL == pproperty) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
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
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	btn = param;

	if (NULL == language_obj) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	ret = lb_gal_list_get_btn_free_num(btn, &idx);
	if (ret != 0)
		return ret;

	lb_gal_update_img(NULL,
		language_obj->pext,
		LB_IMG_UPD_PAR,
		idx,
		btn);

	ret = set_language_idx(idx);
	if (ret != 0)
		return ret;

	return SUCCESS;
}


/**
 * record_mute_init - initial the mute obj to show
 * @param: lb_obj_t object pointer.
 *
 * This function initial the mute obj to show which is on or off
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 record_mute_init(void *param)
{
	lb_int32 ret = 0;
	lb_int32 sw = 0;
	lb_obj_t *lb_obj = NULL;
	lb_imgbtn_t *pproperty = NULL;

	lb_obj = (lb_obj_t *)param;
	if (NULL == lb_obj) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	pproperty = lb_obj->property;
	if (NULL == pproperty) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	ret = get_record_mute_sw(&sw);
	if (ret != 0)
		return ret;

	strcpy(pproperty->rel_img.p_img_src,
		pproperty->rel_img.src_list[sw]);
	strcpy(pproperty->pr_img.p_img_src,
		pproperty->pr_img.src_list[sw]);

	return SUCCESS;
}

/**
 * record_mute_exit - exit the mute obj
 * @param: lb_obj_t object pointer.
 *
 * This function exit the mute obj
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 record_mute_exit(void *param)
{
	lb_obj_t *lb_obj;
	lb_imgbtn_t *pproperty;

	lb_obj = (lb_obj_t *)param;
	if (NULL == lb_obj) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	pproperty = lb_obj->property;
	if (NULL == pproperty) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	return SUCCESS;
}

/**
 * record_reso_resp - switch mute of recording on or off
 * @param: lb_obj_t object pointer.
 *
 * This function switch mute of recording on or off
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 record_mute_resp(void *param)
{
	lb_int32 sw;
	lb_int32 ret;
	lb_imgbtn_t *property;
	void *obj;

	if (NULL == param) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}

	ret = get_record_mute_sw(&sw);
	if (ret != 0)
		return ret;

	if (sw == 0)
		sw = 1;
	else
		sw = 0;

	ret = set_record_mute_sw(sw);
	if (ret != 0)
		return ret;

	ret = lb_view_get_obj_property_by_id(300, (void *)&property);
	if (0 != ret) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}
	ret = lb_view_get_obj_ext_by_id(300, (void *)&obj);
	if (0 != ret) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}

	lb_gal_update_imgbtn(property, obj, LB_IMGBTN_UPD_SRC, sw, NULL);

	return SUCCESS;
}

/**
 * watermark_sw_init - initial the watermark of time obj to show
 * @param: lb_obj_t object pointer.
 *
 * This function initial the watermark of time obj to show which is on or off
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 watermark_sw_init(void *param)
{
	lb_int32 ret = 0;
	lb_int32 sw = 0;
	lb_obj_t *lb_obj = NULL;
	lb_imgbtn_t *pproperty = NULL;

	lb_obj = (lb_obj_t *)param;
	if (NULL == lb_obj) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	pproperty = lb_obj->property;
	if (NULL == pproperty) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	ret = get_watermark_enable_sw(&sw);
	if (ret != 0)
		return ret;

	strcpy(pproperty->rel_img.p_img_src,
		pproperty->rel_img.src_list[sw]);
	strcpy(pproperty->pr_img.p_img_src,
		pproperty->pr_img.src_list[sw]);

	return SUCCESS;
}

/**
 * watermark_sw_exit - exit the watermark of time obj
 * @param: lb_obj_t object pointer.
 *
 * This function initial the watermark of time obj
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 watermark_sw_exit(void *param)
{
	lb_obj_t *lb_obj;
	lb_imgbtn_t *pproperty;

	lb_obj = (lb_obj_t *)param;
	if (NULL == lb_obj) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	pproperty = lb_obj->property;
	if (NULL == pproperty) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	return SUCCESS;
}

/**
 * watermark_sw_resp - switch watermark of time on or off
 * @param: lb_obj_t object pointer.
 *
 * This function switch watermark of time on or off
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 watermark_sw_resp(void *param)
{
	lb_int32 sw = 0;
	lb_int32 ret = 0;
	void *obj = NULL;
	lb_imgbtn_t *property = NULL;

	if (NULL == param) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}

	ret = get_watermark_enable_sw(&sw);
	if (ret != 0)
		return ret;

	if (sw == 0)
		sw = 1;
	else
		sw = 0;

	ret = set_watermark_enable_sw(sw);
	if (ret != 0)
		return ret;

	ret = lb_view_get_obj_property_by_id(301, (void *)&property);
	if (0 != ret) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}
	ret = lb_view_get_obj_ext_by_id(301, (void *)&obj);
	if (0 != ret) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}

	lb_gal_update_imgbtn(property, obj, LB_IMGBTN_UPD_SRC, sw, NULL);

	return SUCCESS;
}

/**
 * watermark_logo_init - initial the watermark of logo obj to show
 * @param: lb_obj_t object pointer.
 *
 * This function initial the watermark of logo obj to show which is on or off
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 watermark_logo_init(void *param)
{
	lb_int32 ret = 0;
	lb_int32 sw = 0;
	lb_obj_t *lb_obj = NULL;
	lb_imgbtn_t *pproperty = NULL;

	lb_obj = (lb_obj_t *)param;
	if (NULL == lb_obj) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	pproperty = lb_obj->property;
	if (NULL == pproperty) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}


	ret = get_watermark_logo_enable_sw(&sw);
	if (ret != 0)
		return ret;

	strcpy(pproperty->rel_img.p_img_src,
		pproperty->rel_img.src_list[sw]);
	strcpy(pproperty->pr_img.p_img_src,
		pproperty->pr_img.src_list[sw]);

	return SUCCESS;
}

/**
 * watermark_logo_exit - exit the watermark of logo obj
 * @param: lb_obj_t object pointer.
 *
 * This function initial the watermark of logo obj
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 watermark_logo_exit(void *param)
{
	if (NULL == param) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}

	return SUCCESS;
}

/**
 * watermark_sw_resp - switch watermark of logo on or off
 * @param: lb_obj_t object pointer.
 *
 * This function switch watermark of logo on or off
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 watermark_logo_resp(void *param)
{
	lb_int32 sw = 0;
	lb_int32 ret = 0;
	lb_imgbtn_t *property = NULL;
	void *obj = NULL;

	if (NULL == param) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}

	ret = get_watermark_logo_enable_sw(&sw);
	if (ret != 0)
		return ret;

	if (sw == 0)
		sw = 1;
	else
		sw = 0;

	ret = set_watermark_logo_enable_sw(sw);
	if (ret != 0)
		return ret;

	ret = lb_view_get_obj_property_by_id(302, (void *)&property);
	if (0 != ret) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}
	ret = lb_view_get_obj_ext_by_id(302, (void *)&obj);
	if (0 != ret) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}

	lb_gal_update_imgbtn(property, obj, LB_IMGBTN_UPD_SRC, sw, NULL);

	return SUCCESS;
}

/**
 * park_monitor_init - initial the park monitor obj to show
 * @param: lb_obj_t object pointer.
 *
 * This function initial the park monitor to show which is on or off
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 park_monitor_init(void *param)
{
	lb_int32 ret = 0;
	lb_int32 sw = 0;
	lb_obj_t *lb_obj = NULL;
	lb_imgbtn_t *pproperty = NULL;

	lb_obj = (lb_obj_t *)param;
	if (NULL == lb_obj) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	pproperty = lb_obj->property;
	if (NULL == pproperty) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	ret = get_park_monitor_enable_sw(&sw);
	if (ret != 0)
		return ret;

	strcpy(pproperty->rel_img.p_img_src,
		pproperty->rel_img.src_list[sw]);
	strcpy(pproperty->pr_img.p_img_src,
		pproperty->pr_img.src_list[sw]);

	return SUCCESS;
}

/**
 * park_monitor_exit - exit the park monitor obj
 * @param: lb_obj_t object pointer.
 *
 * This function exit the park monitor maybe you can do sth you need
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 park_monitor_exit(void *param)
{
	if (NULL == param) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}

	return SUCCESS;
}

/**
 * park_monitor_resp - switch park monitor on or off
 * @param: lb_obj_t object pointer.
 *
 * This function switch park monitor on or off
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 park_monitor_resp(void *param)
{
	lb_int32 sw;
	lb_int32 ret;
	lb_imgbtn_t *property;
	void *obj;

	if (NULL == param) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}

	ret = get_park_monitor_enable_sw(&sw);
	if (ret != 0)
		return ret;

	if (sw == 0)
		sw = 1;
	else
		sw = 0;

	ret = set_park_monitor_enable_sw(sw);
	if (ret != 0)
		return ret;

	ret = lb_view_get_obj_property_by_id(303, (void *)&property);
	if (0 != ret) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}
	ret = lb_view_get_obj_ext_by_id(303, (void *)&obj);
	if (0 != ret) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}

	lb_gal_update_imgbtn(property, obj, LB_IMGBTN_UPD_SRC, sw, NULL);

	return SUCCESS;
}

/**
 * interval_record_init - initial the interval obj to show
 * @param: lb_obj_t object pointer.
 *
 * This function initial the park monitor to show which is on or off
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 interval_record_init(void *param)
{
	lb_int32 ret = 0;
	lb_int32 sw = 0;
	lb_obj_t *lb_obj = NULL;
	lb_imgbtn_t *pproperty = NULL;

	lb_obj = (lb_obj_t *)param;
	if (NULL == lb_obj) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	pproperty = lb_obj->property;
	if (NULL == pproperty) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	ret = get_interval_enable_sw(&sw);
	if (ret != 0)
		return ret;

	strcpy(pproperty->rel_img.p_img_src,
		pproperty->rel_img.src_list[sw]);
	strcpy(pproperty->pr_img.p_img_src,
		pproperty->pr_img.src_list[sw]);

	return SUCCESS;
}

/**
 * interval_record_exit - exit the interval obj
 * @param: lb_obj_t object pointer.
 *
 * This function exit the interval maybe you can do sth you need
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 interval_record_exit(void *param)
{
	if (NULL == param) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}

	return SUCCESS;
}

/**
 * interval_record_resp - switch interval on or off
 * @param: lb_obj_t object pointer.
 *
 * This function switch interval on or off
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 interval_record_resp(void *param)
{
	lb_int32 sw;
	lb_int32 ret;
	lb_imgbtn_t *property;
	void *obj;

	printf("[%s,%d]\n", __func__, __LINE__);

	if (NULL == param) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}

	printf("[%s,%d]\n", __func__, __LINE__);

	ret = get_interval_enable_sw(&sw);
	if (ret != 0)
		return ret;

	if (sw == 0)
		sw = 1;
	else
		sw = 0;

	printf("[%s,%d]\n", __func__, __LINE__);

	ret = set_interval_enable_sw(sw);
	if (ret != 0)
		return ret;

	printf("[%s,%d]\n", __func__, __LINE__);

	ret = lb_view_get_obj_property_by_id(304, (void *)&property);
	if (0 != ret) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}
	ret = lb_view_get_obj_ext_by_id(304, (void *)&obj);
	if (0 != ret) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}

	lb_gal_update_imgbtn(property, obj, LB_IMGBTN_UPD_SRC, sw, NULL);

	return SUCCESS;
}


lb_int32 warn_tone_init(void *param)
{
	lb_int32 ret = 0;
	lb_int32 sw = 0;
	lb_obj_t *lb_obj = NULL;
	lb_imgbtn_t *pproperty = NULL;

	lb_obj = (lb_obj_t *)param;
	if (NULL == lb_obj) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	pproperty = lb_obj->property;
	if (NULL == pproperty) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	ret = get_warn_tone_sw(&sw);
	if (ret != 0)
		return ret;

	strcpy(pproperty->rel_img.p_img_src,
		pproperty->rel_img.src_list[sw]);
	strcpy(pproperty->pr_img.p_img_src,
		pproperty->pr_img.src_list[sw]);

	return SUCCESS;
}


lb_int32 warn_tone_exit(void *param)
{
	if (NULL == param) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}

	return SUCCESS;
}


lb_int32 warn_tone_resp(void *param)
{
	lb_int32 sw;
	lb_int32 ret;
	lb_imgbtn_t *property;
	void *obj;

	printf("[%s,%d]\n", __func__, __LINE__);

	if (NULL == param) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}

	printf("[%s,%d]\n", __func__, __LINE__);

	ret = get_warn_tone_sw(&sw);
	if (ret != 0)
		return ret;

	if (sw == 0)
		sw = 1;
	else
		sw = 0;

	printf("[%s,%d]\n", __func__, __LINE__);

	ret = set_warn_tone_sw(sw);
	if (ret != 0)
		return ret;

	printf("[%s,%d]\n", __func__, __LINE__);

	ret = lb_view_get_obj_property_by_id(304, (void *)&property);
	if (0 != ret) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}
	ret = lb_view_get_obj_ext_by_id(304, (void *)&obj);
	if (0 != ret) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}

	lb_gal_update_imgbtn(property, obj, LB_IMGBTN_UPD_SRC, sw, NULL);

	return SUCCESS;
}





lb_int32 format_init(void *param)
{
	if (NULL == param) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
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
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}

	return SUCCESS;
}


lb_int32 version_init(void *param)
{
	lb_int32 ret = 0;
	lb_obj_t *label = NULL;
	lb_label_t *property = NULL;

	if (NULL == param) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}

	label = (lb_obj_t *)param;
	if (NULL == label) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}

	property = (lb_label_t *)label->property;
	if (NULL == property) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}

	if (NULL == property->txt) {
		property->txt = malloc(32);
		memset(property->txt, 0x00, 32);
	}

	ret = get_version(property->txt);
	if (ret != 0) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}

	return SUCCESS;
}

lb_int32 version_exit(void *param)
{
	lb_obj_t *label = NULL;
	lb_label_t *property = NULL;

	if (NULL == param) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}

	label = (lb_obj_t *)param;
	if (NULL == label) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}

	property = (lb_label_t *)label->property;
	if (NULL == property) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}

	if (property->txt) {
		free(property->txt);
		property->txt = NULL;
	}

	return SUCCESS;
}


lb_int32 factory_init(void *param)
{
	if (NULL == param) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
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
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}

	return SUCCESS;
}

lb_int32 init_funcs(void)
{
	lb_int32 err = SUCCESS;

	err |= lb_fmngr_reg_init_func("loop_record_init", loop_record_init);
	err |= lb_fmngr_reg_exit_func("loop_record_exit", loop_record_exit);

	err |= lb_fmngr_reg_init_func("record_reso_init", record_reso_init);
	err |= lb_fmngr_reg_exit_func("record_reso_exit", record_reso_exit);

	err |= lb_fmngr_reg_init_func("gsensor_sensity_init", gsensor_sensity_init);
	err |= lb_fmngr_reg_exit_func("gsensor_sensity_exit", gsensor_sensity_exit);

	//err |= lb_fmngr_reg_init_func("record_mute_init", record_mute_init);
	//err |= lb_fmngr_reg_exit_func("record_mute_exit", record_mute_exit);

	//err |= lb_fmngr_reg_init_func("watermark_sw_init", watermark_sw_init);
	//err |= lb_fmngr_reg_exit_func("watermark_sw_exit", watermark_sw_exit);

	err |= lb_fmngr_reg_init_func("watermark_logo_init", watermark_logo_init);
	err |= lb_fmngr_reg_exit_func("watermark_logo_exit", watermark_logo_exit);

	err |= lb_fmngr_reg_init_func("park_monitor_init", park_monitor_init);
	err |= lb_fmngr_reg_exit_func("park_monitor_exit", park_monitor_exit);

	err |= lb_fmngr_reg_init_func("warn_tone_init", warn_tone_init);
	err |= lb_fmngr_reg_exit_func("warn_tone_exit", warn_tone_exit);


	err |= lb_fmngr_reg_init_func("lcd_brightness_init", lcd_brightness_init);
	err |= lb_fmngr_reg_exit_func("lcd_brightness_exit", lcd_brightness_exit);



	err |= lb_fmngr_reg_init_func("language_init", language_init);
	err |= lb_fmngr_reg_exit_func("language_exit", language_exit);



	err |= lb_fmngr_reg_init_func("year_init", year_init);
	err |= lb_fmngr_reg_init_func("month_init", month_init);
	err |= lb_fmngr_reg_init_func("day_init", day_init);
	err |= lb_fmngr_reg_init_func("hour_init", hour_init);
	err |= lb_fmngr_reg_init_func("minute_init", minute_init);
	
	err |= lb_fmngr_reg_init_func("date_time_init", date_time_init);
	err |= lb_fmngr_reg_exit_func("date_time_exit", date_time_exit);

	err |= lb_fmngr_reg_init_func("format_init", format_init);
	err |= lb_fmngr_reg_exit_func("format_exit", format_exit);

	err |= lb_fmngr_reg_init_func("version_init", version_init);
	err |= lb_fmngr_reg_exit_func("version_exit", version_exit);

	err |= lb_fmngr_reg_init_func("factory_init", factory_init);
	err |= lb_fmngr_reg_exit_func("factory_exit", factory_exit);

	return err;
}
lb_int32 uninit_funcs(void)
{
	lb_int32 err = SUCCESS;

	err |= lb_fmngr_unreg_init_func(loop_record_init);
	err |= lb_fmngr_unreg_exit_func(loop_record_exit);

	err |= lb_fmngr_unreg_init_func(record_reso_init);
	err |= lb_fmngr_unreg_exit_func(record_reso_exit);

	err |= lb_fmngr_unreg_init_func(gsensor_sensity_init);
	err |= lb_fmngr_unreg_exit_func(gsensor_sensity_exit);

	//err |= lb_fmngr_unreg_init_func(record_mute_init);
	//err |= lb_fmngr_unreg_exit_func(record_mute_exit);

//	err |= lb_fmngr_unreg_init_func(watermark_sw_init);
//	err |= lb_fmngr_unreg_exit_func(watermark_sw_exit);

	err |= lb_fmngr_unreg_init_func(watermark_logo_init);
	err |= lb_fmngr_unreg_exit_func(watermark_logo_exit);

	err |= lb_fmngr_unreg_init_func(park_monitor_init);
	err |= lb_fmngr_unreg_exit_func(park_monitor_exit);

	err |= lb_fmngr_unreg_init_func(warn_tone_init);
	err |= lb_fmngr_unreg_exit_func(warn_tone_exit);

	
	err |= lb_fmngr_unreg_init_func(lcd_brightness_init);
	err |= lb_fmngr_unreg_exit_func(lcd_brightness_exit);



	err |= lb_fmngr_unreg_init_func(language_init);
	err |= lb_fmngr_unreg_exit_func(language_exit);


	err |= lb_fmngr_unreg_init_func(year_init);
	err |= lb_fmngr_unreg_init_func(month_init);
	err |= lb_fmngr_unreg_init_func(day_init);
	err |= lb_fmngr_unreg_init_func(hour_init);
	err |= lb_fmngr_unreg_init_func(minute_init);
	err |= lb_fmngr_unreg_init_func(date_time_init);
	err |= lb_fmngr_unreg_exit_func(date_time_exit);

	err |= lb_fmngr_unreg_init_func(format_init);
	err |= lb_fmngr_unreg_exit_func(format_exit);

	err |= lb_fmngr_unreg_init_func(version_init);
	err |= lb_fmngr_unreg_exit_func(version_exit);

	err |= lb_fmngr_unreg_init_func(factory_init);
	err |= lb_fmngr_unreg_exit_func(factory_exit);

	return err;
}

lb_int32 resp_funcs(void)
{
	lb_int32 err = SUCCESS;

	err |= lb_reg_resp_msg_func(RECORD_LOOP, loop_record_resp);
	err |= lb_reg_resp_msg_func(RECORD_RESO, record_reso_resp);
	err |= lb_reg_resp_msg_func(RECORD_MUTE, record_mute_resp);
	err |= lb_reg_resp_msg_func(RECORD_WATERMARK, watermark_sw_resp);
	err |= lb_reg_resp_msg_func(RECORD_WATERMARK_LOGO, watermark_logo_resp);
	err |= lb_reg_resp_msg_func(RECORD_PARK_MONITOR, park_monitor_resp);
	err |= lb_reg_resp_msg_func(RECORD_GSENSOR_SENSITY, gsensor_sensity_resp);
	err |= lb_reg_resp_msg_func(RECORD_TONE, warn_tone_resp);
    err |= lb_reg_resp_msg_func(LCD_BRIGHT, lcd_brightness_resp);
    err |= lb_reg_resp_msg_func(LANGUAGE, language_resp);
   

	err |= lb_reg_resp_msg_func(MSG_SEL_YEAR, year_resp);
	err |= lb_reg_resp_msg_func(MSG_SEL_MONTH, month_resp);
	err |= lb_reg_resp_msg_func(MSG_SEL_DAY, day_resp);
	err |= lb_reg_resp_msg_func(MSG_SEL_HOUR, hour_resp);
	err |= lb_reg_resp_msg_func(MSG_SEL_MINTUE, minute_resp);

	return err;
}

lb_int32 unresp_funcs(void)
{
	lb_int32 err = SUCCESS;

	err |= lb_unreg_resp_msg_func(loop_record_resp);
	err |= lb_unreg_resp_msg_func(record_reso_resp);
	err |= lb_unreg_resp_msg_func(record_mute_resp);
	err |= lb_unreg_resp_msg_func(watermark_sw_resp);
	err |= lb_unreg_resp_msg_func(watermark_logo_resp);
	err |= lb_unreg_resp_msg_func(park_monitor_resp);
	err |= lb_unreg_resp_msg_func(gsensor_sensity_resp);
	err |= lb_unreg_resp_msg_func(warn_tone_resp);
	err |= lb_unreg_resp_msg_func(lcd_brightness_resp);
	err |= lb_unreg_resp_msg_func(language_resp);
	

	err |= lb_unreg_resp_msg_func(year_resp);
	err |= lb_unreg_resp_msg_func(month_resp);
	err |= lb_unreg_resp_msg_func(day_resp);
	err |= lb_unreg_resp_msg_func(hour_resp);
	err |= lb_unreg_resp_msg_func(minute_resp);


	return err;
}
