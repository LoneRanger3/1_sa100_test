/*
 * adas_set.c - adas setting code for LomboTech
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
#include <debug.h>
#include <rtthread.h>
#include "smart_drive.h"
#include "smart_drive_common.h"
#include "main_view.h"
#include "cJSON.h"
#include "lb_types.h"
#include "lb_common.h"
#include "lb_ui.h"
#include "lb_gal_common.h"


static lb_obj_t *time_sel;
static lb_int32 cur_list_type;
static lb_int32 cur_fcw_index;
static lb_int32 cur_ldw_index;

static lb_int32 time_sel_init(void *param)
{
	lb_img_t *pproperty;

	time_sel = (lb_obj_t *)param;
	if (NULL == time_sel) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	pproperty = time_sel->property;
	if (NULL == pproperty) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	cur_list_type = 0;
	if (cur_list_type == 0) {
		if (adas_get_fcwsensity() == 1)
			pproperty->comms.in_which_btn = 0;
		else if (adas_get_fcwsensity() == 2)
			pproperty->comms.in_which_btn = 1;
		else
			pproperty->comms.in_which_btn = 2;
	} else {
		if (adas_get_ldwsensity() == 1)
			pproperty->comms.in_which_btn = 0;
		else if (adas_get_ldwsensity() == 3)
			pproperty->comms.in_which_btn = 1;
		else
			pproperty->comms.in_which_btn = 2;
	}

	if (adas_get_fcwsensity() == 1)
		cur_fcw_index = 0;
	else if (adas_get_fcwsensity() == 2)
		cur_fcw_index = 1;
	else
		cur_fcw_index = 2;

	if (adas_get_ldwsensity() == 1)
		cur_ldw_index  = 0;
	else if (adas_get_ldwsensity() == 3)
		cur_ldw_index  = 1;
	else
		cur_ldw_index  = 2;

	return SUCCESS;
}

static lb_int32 time_sel_exit(void *param)
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

static lb_int32 time_list_resp(void *param)
{
	void *btn = NULL;
	lb_uint32 idx = 0;
	lb_int32 ret = -1;

	if (NULL == param) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	btn = param;

	if (NULL == time_sel) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	ret = lb_gal_list_get_btn_free_num(btn, &idx);
	if (ret != 0)
		return ret;

	lb_gal_update_img(NULL,
		time_sel->pext,
		LB_IMG_UPD_PAR,
		idx,
		btn);

	if (cur_list_type == 0) {
		cur_fcw_index = idx;
		adas_save_fcwsensity(idx);
	} else {
		cur_ldw_index = idx;
		adas_save_ldwsensity(idx);
	}

	return SUCCESS;
}

static lb_int32 update_time_parent(lb_uint32 idx)
{
	lb_int32 ret = 0;
	lb_list_t *list = NULL;

	ret = lb_view_get_obj_property_by_id(208, (void **)&list);
	if (0 != ret) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	if (NULL == time_sel || NULL == time_sel->pext) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	if ((0 > idx) || (list->options_num < idx)) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	lb_gal_update_img(NULL,
		time_sel->pext,
		LB_IMG_UPD_PAR,
		idx,
		list->options[idx]);

	return ret;
}

static lb_int32 change_time_list(lb_uint32 idx)
{
	lb_int32 ret = 0;
	void *obj0 = NULL;
	void *obj1 = NULL;
	void *obj2 = NULL;
	const char *result_str = NULL;
	char json_str[64];
	static char data_str0[64];
	static char data_str1[64];
	static char data_str2[64];

	lb_view_get_obj_ext_by_id(209, &obj0);
	if (NULL == obj0) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	lb_view_get_obj_ext_by_id(210, &obj1);
	if (NULL == obj1) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	lb_view_get_obj_ext_by_id(211, &obj2);
	if (NULL == obj2) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	if (idx == 0) {
		strcpy(json_str, "STR_SMART_DRIVE_ADAS_SETTING_HEAD_ALARM_TIME_1");
		result_str = elang_get_utf8_string_josn(json_str);
		if (result_str)
			strcpy(data_str0, result_str);

		strcpy(json_str, "STR_SMART_DRIVE_ADAS_SETTING_HEAD_ALARM_TIME_2");
		result_str = elang_get_utf8_string_josn(json_str);
		if (result_str)
			strcpy(data_str1, result_str);

		strcpy(json_str, "STR_SMART_DRIVE_ADAS_SETTING_HEAD_ALARM_TIME_3");
		result_str = elang_get_utf8_string_josn(json_str);
		if (result_str)
			strcpy(data_str2, result_str);

		update_time_parent(cur_fcw_index);

	} else if (idx == 1) {
		strcpy(json_str, "STR_SMART_DRIVE_ADAS_SETTING_LANE_ALARM_TIME_1");
		result_str = elang_get_utf8_string_josn(json_str);
		if (result_str)
			strcpy(data_str0, result_str);

		strcpy(json_str, "STR_SMART_DRIVE_ADAS_SETTING_LANE_ALARM_TIME_2");
		result_str = elang_get_utf8_string_josn(json_str);
		if (result_str)
			strcpy(data_str1, result_str);

		strcpy(json_str, "STR_SMART_DRIVE_ADAS_SETTING_LANE_ALARM_TIME_3");
		result_str = elang_get_utf8_string_josn(json_str);
		if (result_str)
			strcpy(data_str2, result_str);

		update_time_parent(cur_ldw_index);
	}

	lb_gal_update_label(obj0, LB_LABEL_UPD_TXT, data_str0);
	lb_gal_update_label(obj1, LB_LABEL_UPD_TXT, data_str1);
	lb_gal_update_label(obj2, LB_LABEL_UPD_TXT, data_str2);

	return ret;
}

static lb_int32 type_list_resp(void *param)
{
	void *obj;
	lb_int32 ret;
	lb_label_t *label;
	void *obj0;
	void *obj1;
	static lb_al_style_t al_style0;
	static lb_al_style_t al_style1;
	void *btn = NULL;
	lb_uint32 idx = 0;

	if (NULL == param) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	btn = param;

	ret = lb_view_get_obj_ext_by_id(207, &obj);
	if (0 != ret) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}
	ret = lb_gal_list_get_btn_free_num(btn, &idx);
	if (ret != 0)
		return ret;

	if (idx == 0) {
		ret = lb_view_get_obj_property_by_id(203, (void **)&label);
		if (0 != ret) {
			printf("[%s]Invalid parameters!\n", __func__);
			return LB_ERROR_NO_MEM;
		}
	} else if (idx == 1) {
		ret = lb_view_get_obj_property_by_id(205, (void **)&label);
		if (0 != ret) {
			printf("[%s]Invalid parameters!\n", __func__);
			return LB_ERROR_NO_MEM;
		}
	}

	if (label->txt)
		lb_gal_update_label(obj, LB_LABEL_UPD_TXT, label->txt);

	ret = lb_view_get_obj_ext_by_id(203, &obj0);
	ret = lb_view_get_obj_ext_by_id(205, &obj1);

	lb_gal_label_get_style(obj0, &al_style0);
	lb_gal_label_get_style(obj1, &al_style1);
	cur_list_type = idx;
	if (idx == 0) {
		al_style0.text.color = 0x0be4be;
		al_style1.text.color = 0xf0f0f0;
	} else if (idx == 1) {
		al_style0.text.color = 0xf0f0f0;
		al_style1.text.color = 0x0be4be;
	}

	lb_gal_update_label(obj0, LB_LABEL_UPD_STYLE, &al_style0);
	lb_gal_update_label(obj1, LB_LABEL_UPD_STYLE, &al_style1);

	change_time_list(idx);

	return SUCCESS;
}

lb_int32 adas_set_init_funcs(void)
{
	lb_int32 err = SUCCESS;

	err |= lb_fmngr_reg_init_func("time_sel_init",
			time_sel_init);
	err |= lb_fmngr_reg_exit_func("time_sel_exit",
			time_sel_exit);

	return err;
}

lb_int32 adas_set_uninit_funcs(void)
{
	lb_int32 err = SUCCESS;

	err |= lb_fmngr_unreg_init_func(time_sel_init);
	err |= lb_fmngr_unreg_exit_func(time_sel_exit);

	return err;
}

lb_int32 adas_set_resp_funcs(void)
{
	lb_int32 err = SUCCESS;

	err |= lb_reg_resp_msg_func(ADAS_TIME_LIST_RESP,
			time_list_resp);
	err |= lb_reg_resp_msg_func(ADAS_TYPE_LIST_RESP,
			type_list_resp);

	return err;
}

lb_int32 adas_set_unresp_funcs(void)
{
	lb_int32 err = SUCCESS;

	err |= lb_unreg_resp_msg_func(time_list_resp);
	err |= lb_unreg_resp_msg_func(type_list_resp);

	return err;
}
