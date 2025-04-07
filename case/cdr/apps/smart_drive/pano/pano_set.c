/*
 * pano_set.c - pano setting code for LomboTech
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
#include "main_view.h"
#include "pano_set.h"
#include "cJSON.h"
#include "lb_types.h"
#include "lb_common.h"
#include "lb_ui.h"
#include "lb_gal_common.h"
#include "smart_drive_common.h"

double motor_w[MOTOR_TYPE_MAX];
double motor_l[MOTOR_TYPE_MAX];
int motor_d2r[MOTOR_TYPE_MAX];

static lb_int32 motor_width_init(void *param);
static lb_int32 motor_length_init(void *param);
static lb_int32 motor_dist2rear_init(void *param);
static lb_int32 car_imgbtn_init(void *param);
static lb_int32 suv_imgbtn_init(void *param);
static lb_int32 others_imgbtn_init(void *param);

lb_int32 motor_get_set_para(pano_set_t *set)
{
	lb_int32 ret = 0;
	lb_int32 idx = 0;

	if (set) {
		idx = pano_get_type();

		if (motor_w[idx] == 0)
			motor_w[idx] = pano_get_width(idx);
		if (motor_l[idx] == 0)
			motor_l[idx] = pano_get_length(idx);
		if (motor_d2r[idx] == 0)
			motor_d2r[idx] = pano_get_distance(idx);

		set->motor_w = motor_w[idx];
		set->motor_l = motor_l[idx];
		set->motor_d2r = motor_d2r[idx];
	}

	return ret;
}

static lb_int32 motor_reset_data(void)
{
	motor_width_init(NULL);
	motor_length_init(NULL);
	motor_dist2rear_init(NULL);

	return 0;
}

static lb_int32 motor_sel_type(void *param)
{
	void *cur_property = NULL;
	void *btn_property = NULL;
	void *car_property = NULL;
	void *btn_obj = NULL;
	void *car_obj = NULL;

	if (NULL == param) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}

	lb_view_get_obj_property_by_ext(param, &cur_property);
	if (NULL == cur_property) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}

	lb_view_get_obj_property_by_id(221, &car_property);
	lb_view_get_obj_ext_by_id(221, &car_obj);
	if (NULL == car_property || NULL == car_obj) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}

	/* normal */
	lb_view_get_obj_property_by_id(205, &btn_property);
	lb_view_get_obj_ext_by_id(205, &btn_obj);
	if (NULL == btn_property || NULL == btn_obj) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}
	lb_gal_update_imgbtn(btn_property,
		btn_obj, LB_IMGBTN_UPD_SRC, 1, NULL);

	if (btn_property == cur_property) {
		pano_save_type(MOTOR_TYPE_CAR);
		lb_gal_update_img(car_property,
			car_obj, LB_IMG_UPD_SRC, 0, NULL);
		motor_reset_data();
	}
	/* suv */
	lb_view_get_obj_property_by_id(207, &btn_property);
	lb_view_get_obj_ext_by_id(207, &btn_obj);
	if (NULL == btn_property || NULL == btn_obj) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}
	lb_gal_update_imgbtn(btn_property,
		btn_obj, LB_IMGBTN_UPD_SRC, 1, NULL);

	if (btn_property == cur_property) {
		pano_save_type(MOTOR_TYPE_SUV);
		lb_gal_update_img(car_property,
			car_obj, LB_IMG_UPD_SRC, 1, NULL);
		motor_reset_data();
	}
	/* others */
	lb_view_get_obj_property_by_id(209, &btn_property);
	lb_view_get_obj_ext_by_id(209, &btn_obj);
	if (NULL == btn_property || NULL == btn_obj) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}
	lb_gal_update_imgbtn(btn_property,
		btn_obj, LB_IMGBTN_UPD_SRC, 1, NULL);

	if (btn_property == cur_property) {
		pano_save_type(MOTOR_TYPE_OTHERS);
		lb_gal_update_img(car_property,
			car_obj, LB_IMG_UPD_SRC, 2, NULL);
		motor_reset_data();
	}
	lb_gal_update_imgbtn(cur_property,
		param, LB_IMGBTN_UPD_SRC, 0, NULL);

	return SUCCESS;
}

static lb_int32 motor_width_init(void *param)
{
	void *obj = NULL;
	static char width_str[16];
	lb_int32 idx = 0;

	lb_view_get_obj_ext_by_id(260, &obj);
	if (NULL == obj) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	idx = pano_get_type();

	if (motor_w[MOTOR_TYPE_CAR] == 0)
		motor_w[MOTOR_TYPE_CAR] = pano_get_width(MOTOR_TYPE_CAR);
	if (motor_w[MOTOR_TYPE_SUV] == 0)
		motor_w[MOTOR_TYPE_SUV] = pano_get_width(MOTOR_TYPE_SUV);
	if (motor_w[MOTOR_TYPE_OTHERS] == 0)
		motor_w[MOTOR_TYPE_OTHERS] = pano_get_width(MOTOR_TYPE_OTHERS);

	sprintf(width_str, "%4.2fM", motor_w[idx]);
	printf("width_str:%s\n", width_str);

	lb_gal_update_label(obj, LB_LABEL_UPD_TXT, width_str);

	return SUCCESS;
}

static lb_int32 motor_width_exit(void *param)
{
	lb_int32 ret = 0;

	return ret;
}

static lb_int32 motor_length_init(void *param)
{
	void *obj = NULL;
	static char length_str[16];
	lb_int32 idx = 0;

	lb_view_get_obj_ext_by_id(261, &obj);
	if (NULL == obj) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	idx = pano_get_type();

	if (motor_l[MOTOR_TYPE_CAR] == 0)
		motor_l[MOTOR_TYPE_CAR] = pano_get_length(MOTOR_TYPE_CAR);
	if (motor_l[MOTOR_TYPE_SUV] == 0)
		motor_l[MOTOR_TYPE_SUV] = pano_get_length(MOTOR_TYPE_SUV);
	if (motor_l[MOTOR_TYPE_OTHERS] == 0)
		motor_l[MOTOR_TYPE_OTHERS] = pano_get_length(MOTOR_TYPE_OTHERS);

	sprintf(length_str, "%4.2fM", motor_l[idx]);
	printf("length_str:%s\n", length_str);

	lb_gal_update_label(obj, LB_LABEL_UPD_TXT, length_str);

	return SUCCESS;
}

static lb_int32 motor_length_exit(void *param)
{
	lb_int32 ret = 0;

	return ret;
}

static lb_int32 motor_dist2rear_init(void *param)
{
	void *obj = NULL;
	static char str[16];
	lb_int32 idx = 0;

	lb_view_get_obj_ext_by_id(262, &obj);
	if (NULL == obj) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	idx = pano_get_type();

	if (motor_d2r[MOTOR_TYPE_CAR] == 0)
		motor_d2r[MOTOR_TYPE_CAR] = pano_get_distance(MOTOR_TYPE_CAR);
	if (motor_d2r[MOTOR_TYPE_SUV] == 0)
		motor_d2r[MOTOR_TYPE_SUV] = pano_get_distance(MOTOR_TYPE_SUV);
	if (motor_d2r[MOTOR_TYPE_OTHERS] == 0)
		motor_d2r[MOTOR_TYPE_OTHERS] = pano_get_distance(MOTOR_TYPE_OTHERS);

	sprintf(str, "%dCM", motor_d2r[idx]);
	printf("length_str:%s\n", str);

	lb_gal_update_label(obj, LB_LABEL_UPD_TXT, str);

	return SUCCESS;
}

static lb_int32 motor_dist2rear_exit(void *param)
{
	lb_int32 ret = 0;

	return ret;
}

static lb_int32 car_imgbtn_init(void *param)
{
	lb_imgbtn_t *property = NULL;
	lb_obj_t *imgbtn = NULL;
	lb_int32 idx = 0;

	idx = pano_get_type();

	imgbtn = (lb_obj_t *)param;
	if (NULL == imgbtn) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	property = (lb_imgbtn_t *)imgbtn->property;
	if (NULL == property) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	if (idx == MOTOR_TYPE_CAR) {
		strcpy(property->rel_img.p_img_src,
			property->rel_img.src_list[0]);
		strcpy(property->pr_img.p_img_src,
			property->rel_img.src_list[0]);
	} else {
		strcpy(property->rel_img.p_img_src,
			property->rel_img.src_list[1]);
		strcpy(property->pr_img.p_img_src,
			property->rel_img.src_list[1]);
	}

	return SUCCESS;
}

static lb_int32 suv_imgbtn_init(void *param)
{
	lb_imgbtn_t *property = NULL;
	lb_obj_t *imgbtn = NULL;
	lb_int32 idx = 0;

	idx = pano_get_type();

	imgbtn = (lb_obj_t *)param;
	if (NULL == imgbtn) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	property = (lb_imgbtn_t *)imgbtn->property;
	if (NULL == property) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	if (idx == MOTOR_TYPE_SUV) {
		strcpy(property->rel_img.p_img_src,
			property->rel_img.src_list[0]);
		strcpy(property->pr_img.p_img_src,
			property->rel_img.src_list[0]);
	} else {
		strcpy(property->rel_img.p_img_src,
			property->rel_img.src_list[1]);
		strcpy(property->pr_img.p_img_src,
			property->rel_img.src_list[1]);
	}

	return SUCCESS;
}

static lb_int32 others_imgbtn_init(void *param)
{
	lb_imgbtn_t *property = NULL;
	lb_obj_t *imgbtn = NULL;
	lb_int32 idx = 0;

	idx = pano_get_type();

	imgbtn = (lb_obj_t *)param;
	if (NULL == imgbtn) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	property = (lb_imgbtn_t *)imgbtn->property;
	if (NULL == property) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	if (idx == MOTOR_TYPE_OTHERS) {
		strcpy(property->rel_img.p_img_src,
			property->rel_img.src_list[0]);
		strcpy(property->pr_img.p_img_src,
			property->rel_img.src_list[0]);
	} else {
		strcpy(property->rel_img.p_img_src,
			property->rel_img.src_list[1]);
		strcpy(property->pr_img.p_img_src,
			property->rel_img.src_list[1]);
	}

	return SUCCESS;
}

static lb_int32 motor_width_plus(void *param)
{
	lb_int32 ret = 0;
	static void *p_obj;
	static char width_str[16];
	lb_int32 idx = 0;

	idx = pano_get_type();

	motor_w[idx] += MOTOR_WIDTH_STEP;
	if (motor_w[idx] >= MOTOR_WIDTH_MAX)
		motor_w[idx] = MOTOR_WIDTH_MAX;

	ret = lb_view_get_obj_ext_by_id(260, &p_obj);
	if (0 != ret) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}

	sprintf(width_str, "%4.2fM", motor_w[idx]);
	printf("width_str:%s\n", width_str);

	lb_gal_update_label(p_obj, LB_LABEL_UPD_TXT, width_str);

	pano_save_width(MOTOR_TYPE_CAR,
		motor_w[MOTOR_TYPE_CAR]);
	pano_save_width(MOTOR_TYPE_SUV,
		motor_w[MOTOR_TYPE_SUV]);
	pano_save_width(MOTOR_TYPE_OTHERS,
		motor_w[MOTOR_TYPE_OTHERS]);

	return ret;
}

static lb_int32 motor_width_minus(void *param)
{
	lb_int32 ret = 0;
	static void *p_obj;
	static char width_str[16];
	lb_int32 idx = 0;

	idx = pano_get_type();

	motor_w[idx] -= MOTOR_WIDTH_STEP;
	if (motor_w[idx] <= MOTOR_WIDTH_MIN)
		motor_w[idx] = MOTOR_WIDTH_MIN;

	ret = lb_view_get_obj_ext_by_id(260, &p_obj);
	if (0 != ret) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}

	sprintf(width_str, "%4.2fM", motor_w[idx]);
	printf("width_str:%s\n", width_str);

	lb_gal_update_label(p_obj, LB_LABEL_UPD_TXT, width_str);

	pano_save_width(MOTOR_TYPE_CAR,
		motor_w[MOTOR_TYPE_CAR]);
	pano_save_width(MOTOR_TYPE_SUV,
		motor_w[MOTOR_TYPE_SUV]);
	pano_save_width(MOTOR_TYPE_OTHERS,
		motor_w[MOTOR_TYPE_OTHERS]);

	return ret;
}

static lb_int32 motor_length_plus(void *param)
{
	lb_int32 ret = 0;
	static void *p_obj;
	static char width_str[16];
	lb_int32 idx = 0;

	idx = pano_get_type();

	motor_l[idx] += MOTOR_LENGTH_STEP;
	if (motor_l[idx] >= MOTOR_LENGTH_MAX)
		motor_l[idx] = MOTOR_LENGTH_MAX;

	ret = lb_view_get_obj_ext_by_id(261, &p_obj);
	if (0 != ret) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}

	sprintf(width_str, "%4.2fM", motor_l[idx]);
	printf("width_str:%s\n", width_str);

	lb_gal_update_label(p_obj, LB_LABEL_UPD_TXT, width_str);

	pano_save_length(MOTOR_TYPE_CAR,
		motor_l[MOTOR_TYPE_CAR]);
	pano_save_length(MOTOR_TYPE_SUV,
		motor_l[MOTOR_TYPE_SUV]);
	pano_save_length(MOTOR_TYPE_OTHERS,
		motor_l[MOTOR_TYPE_OTHERS]);

	return ret;
}

static lb_int32 motor_length_minus(void *param)
{
	lb_int32 ret = 0;
	static void *p_obj;
	static char width_str[16];
	lb_int32 idx = 0;

	idx = pano_get_type();

	motor_l[idx] -= MOTOR_LENGTH_STEP;
	if (motor_l[idx] <= MOTOR_LENGTH_MIN)
		motor_l[idx] = MOTOR_LENGTH_MIN;

	ret = lb_view_get_obj_ext_by_id(261, &p_obj);
	if (0 != ret) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}

	sprintf(width_str, "%4.2fM", motor_l[idx]);
	printf("width_str:%s\n", width_str);

	lb_gal_update_label(p_obj, LB_LABEL_UPD_TXT, width_str);

	pano_save_length(MOTOR_TYPE_CAR,
		motor_l[MOTOR_TYPE_CAR]);
	pano_save_length(MOTOR_TYPE_SUV,
		motor_l[MOTOR_TYPE_SUV]);
	pano_save_length(MOTOR_TYPE_OTHERS,
		motor_l[MOTOR_TYPE_OTHERS]);

	return ret;
}

static lb_int32 motor_dist2rear_plus(void *param)
{
	lb_int32 ret = 0;
	static void *p_obj;
	static char str[16];
	lb_int32 idx = 0;

	idx = pano_get_type();

	motor_d2r[idx] += MOTOR_DIST2R_STEP;
	if (motor_d2r[idx] >= MOTOR_DIST2R_MAX)
		motor_d2r[idx] = MOTOR_DIST2R_MAX;

	ret = lb_view_get_obj_ext_by_id(262, &p_obj);
	if (0 != ret) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}

	sprintf(str, "%dCM", motor_d2r[idx]);
	printf("width_str:%s\n", str);

	lb_gal_update_label(p_obj, LB_LABEL_UPD_TXT, str);

	pano_save_distance(MOTOR_TYPE_CAR,
		motor_d2r[MOTOR_TYPE_CAR]);
	pano_save_distance(MOTOR_TYPE_SUV,
		motor_d2r[MOTOR_TYPE_SUV]);
	pano_save_distance(MOTOR_TYPE_OTHERS,
		motor_d2r[MOTOR_TYPE_OTHERS]);

	return ret;
}

static lb_int32 motor_dist2rear_minus(void *param)
{
	lb_int32 ret = 0;
	static void *p_obj;
	static char str[16];
	lb_int32 idx = 0;

	idx = pano_get_type();

	motor_d2r[idx] -= MOTOR_DIST2R_STEP;
	if (motor_d2r[idx] <= MOTOR_DIST2R_MIN)
		motor_d2r[idx] = MOTOR_DIST2R_MIN;

	ret = lb_view_get_obj_ext_by_id(262, &p_obj);
	if (0 != ret) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}

	sprintf(str, "%dCM", motor_d2r[idx]);
	printf("width_str:%s\n", str);

	lb_gal_update_label(p_obj, LB_LABEL_UPD_TXT, str);

	pano_save_distance(MOTOR_TYPE_CAR,
		motor_d2r[MOTOR_TYPE_CAR]);
	pano_save_distance(MOTOR_TYPE_SUV,
		motor_d2r[MOTOR_TYPE_SUV]);
	pano_save_distance(MOTOR_TYPE_OTHERS,
		motor_d2r[MOTOR_TYPE_OTHERS]);

	return ret;
}

lb_int32 pano_set_init_funcs(void)
{
	lb_int32 err = SUCCESS;

	err |= lb_fmngr_reg_init_func("motor_width_init",
			motor_width_init);
	err |= lb_fmngr_reg_exit_func("motor_width_exit",
			motor_width_exit);

	err |= lb_fmngr_reg_init_func("motor_length_init",
			motor_length_init);
	err |= lb_fmngr_reg_exit_func("motor_length_exit",
			motor_length_exit);

	err |= lb_fmngr_reg_init_func("motor_dist2rear_init",
			motor_dist2rear_init);
	err |= lb_fmngr_reg_exit_func("motor_dist2rear_exit",
			motor_dist2rear_exit);

	err |= lb_fmngr_reg_init_func("car_imgbtn_init",
			car_imgbtn_init);
	err |= lb_fmngr_reg_init_func("suv_imgbtn_init",
			suv_imgbtn_init);
	err |= lb_fmngr_reg_init_func("others_imgbtn_init",
			others_imgbtn_init);

	return err;
}

lb_int32 pano_set_uninit_funcs(void)
{
	lb_int32 err = SUCCESS;

	err |= lb_fmngr_unreg_init_func(car_imgbtn_init);
	err |= lb_fmngr_unreg_init_func(suv_imgbtn_init);
	err |= lb_fmngr_unreg_init_func(others_imgbtn_init);

	err |= lb_fmngr_unreg_init_func(motor_width_init);
	err |= lb_fmngr_unreg_exit_func(motor_width_exit);

	err |= lb_fmngr_unreg_init_func(motor_length_init);
	err |= lb_fmngr_unreg_exit_func(motor_length_exit);

	err |= lb_fmngr_unreg_init_func(motor_dist2rear_init);
	err |= lb_fmngr_unreg_exit_func(motor_dist2rear_exit);

	return err;
}

lb_int32 pano_set_resp_funcs(void)
{
	lb_int32 err = SUCCESS;

	err |= lb_reg_resp_msg_func(PANO_SEL_MOTOR_TYPE,
			motor_sel_type);

	err |= lb_reg_resp_msg_func(PANO_CAR_W_PLUS,
			motor_width_plus);
	err |= lb_reg_resp_msg_func(PANO_CAR_W_MINUS,
			motor_width_minus);

	err |= lb_reg_resp_msg_func(PANO_CAR_L_PLUS,
			motor_length_plus);
	err |= lb_reg_resp_msg_func(PANO_CAR_L_MINUS,
			motor_length_minus);

	err |= lb_reg_resp_msg_func(PANO_CAR_DIST2REAR_PLUS,
			motor_dist2rear_plus);
	err |= lb_reg_resp_msg_func(PANO_CAR_DIST2REAR_MINUS,
			motor_dist2rear_minus);

	return err;
}

lb_int32 pano_set_unresp_funcs(void)
{
	lb_int32 err = SUCCESS;

	err |= lb_unreg_resp_msg_func(motor_sel_type);

	err |= lb_unreg_resp_msg_func(motor_width_plus);
	err |= lb_unreg_resp_msg_func(motor_width_minus);

	err |= lb_unreg_resp_msg_func(motor_length_plus);
	err |= lb_unreg_resp_msg_func(motor_length_minus);

	err |= lb_unreg_resp_msg_func(motor_dist2rear_plus);
	err |= lb_unreg_resp_msg_func(motor_dist2rear_minus);

	return err;
}
