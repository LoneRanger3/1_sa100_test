/*
 * main_view.c - main view code for LomboTech
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
#include "cJSON.h"
#include "lb_types.h"
#include "lb_common.h"
#include "lb_ui.h"
#include "lb_gal_common.h"
#include "smart_drive.h"
#include "smart_drive_common.h"
#include "main_view.h"
#include "pano/pano_calib.h"
#include "adas/adas_calib.h"

static void *p_obj;

lb_int32 adas_enable_sw_init(void *param)
{
	lb_imgbtn_t *pproperty;
	lb_obj_t *adas_enable_sw_imgbtn;

	adas_enable_sw_imgbtn = (lb_obj_t *)param;
	if (NULL == adas_enable_sw_imgbtn) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	pproperty = (lb_imgbtn_t *)adas_enable_sw_imgbtn->property;
	if (NULL == pproperty) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	strcpy(pproperty->rel_img.p_img_src,
		pproperty->rel_img.src_list[car_get_adas_enable()]);
	strcpy(pproperty->pr_img.p_img_src,
		pproperty->rel_img.src_list[car_get_adas_enable()]);

	return SUCCESS;
}

lb_int32 pano_enable_sw_init(void *param)
{
	lb_imgbtn_t *pproperty;
	lb_obj_t *pano_enable_sw_imgbtn;

	pano_enable_sw_imgbtn = (lb_obj_t *)param;
	if (NULL == pano_enable_sw_imgbtn) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	pproperty = (lb_imgbtn_t *)pano_enable_sw_imgbtn->property;
	if (NULL == pproperty) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	strcpy(pproperty->rel_img.p_img_src,
		pproperty->rel_img.src_list[car_get_pano_enable()]);
	strcpy(pproperty->pr_img.p_img_src,
		pproperty->rel_img.src_list[car_get_pano_enable()]);

	return SUCCESS;
}

lb_int32 bsd_enable_sw_init(void *param)
{
	lb_imgbtn_t *pproperty;
	lb_obj_t *bsd_enable_sw_imgbtn;

	bsd_enable_sw_imgbtn = (lb_obj_t *)param;
	if (NULL == bsd_enable_sw_imgbtn) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	pproperty = (lb_imgbtn_t *)bsd_enable_sw_imgbtn->property;
	if (NULL == pproperty) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	strcpy(pproperty->rel_img.p_img_src,
		pproperty->rel_img.src_list[car_get_bsd_enable()]);
	strcpy(pproperty->pr_img.p_img_src,
		pproperty->rel_img.src_list[car_get_bsd_enable()]);

	return SUCCESS;
}


lb_int32 adas_sw_resp(void *param)
{

	lb_int32 ret;
	lb_imgbtn_t *property;
	void *obj;

	if (car_get_adas_enable() == 0)
		car_save_adas_enable(1);
	else if (car_get_adas_enable() == 1)
		car_save_adas_enable(0);

	ret = lb_view_get_obj_property_by_id(208, (void *)&property);
	if (0 != ret) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}
	ret = lb_view_get_obj_ext_by_id(208, (void *)&obj);
	if (0 != ret) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}
	lb_gal_update_imgbtn(property,
		obj, LB_IMGBTN_UPD_SRC, car_get_adas_enable(), NULL);

	return SUCCESS;
}

lb_int32 pano_sw_resp(void *param)
{
	lb_int32 ret;
	lb_imgbtn_t *property;
	void *obj;

	if (car_get_pano_enable() == 0)
		car_save_pano_enable(1);
	else if (car_get_pano_enable() == 1)
		car_save_pano_enable(0);

	ret = lb_view_get_obj_property_by_id(215, (void *)&property);
	if (0 != ret) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}
	ret = lb_view_get_obj_ext_by_id(215, (void *)&obj);
	if (0 != ret) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}

	lb_gal_update_imgbtn(property,
		obj, LB_IMGBTN_UPD_SRC, car_get_pano_enable(), NULL);

	return SUCCESS;
}

lb_int32 bsd_sw_resp(void *param)
{
	lb_int32 ret;
	lb_imgbtn_t *property;
	void *obj;

	if (car_get_bsd_enable() == 0)
		car_save_bsd_enable(1);
	else if (car_get_bsd_enable() == 1)
		car_save_bsd_enable(0);

	ret = lb_view_get_obj_property_by_id(222, (void *)&property);
	if (0 != ret) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}
	ret = lb_view_get_obj_ext_by_id(222, (void *)&obj);
	if (0 != ret) {
		printf("[%s]Invalid parameters!\n", __func__);
		return LB_ERROR_NO_MEM;
	}

	lb_gal_update_imgbtn(property,
		obj, LB_IMGBTN_UPD_SRC, car_get_bsd_enable(), NULL);

	return SUCCESS;
}

/**
 * load_main_view - save objects from the main view
 *
 * This function save objects from the main view
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 load_main_view(void)
{
	lb_int32 ret = 0;
	lb_uint32 id = 0;

	id = 201;
	ret = lb_view_get_obj_ext_by_id(id, &p_obj);
	if (ret != 0) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return ret;
	}

	return ret;
}

/**
 * hide_main_view - hide the main view
 *
 * This function hide the main view
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 hide_main_view(void *para)
{
	lb_int32 ret = 0;

	ret = lb_gal_set_obj_hidden(p_obj, 1);

	return ret;
}

/**
 * show_main_view - show the main view
 *
 * This function show the main view
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 show_main_view(void *para)
{
	lb_int32 ret = 0;

	ret = lb_gal_set_obj_hidden(p_obj, 0);

	return ret;
}

lb_int32 init_funcs(void)
{
	lb_int32 err = SUCCESS;

	err |= lb_fmngr_reg_init_func("adas_enable_sw_init",
			adas_enable_sw_init);
	err |= lb_fmngr_reg_init_func("pano_enable_sw_init",
			pano_enable_sw_init);
	err |= lb_fmngr_reg_init_func("bsd_enable_sw_init",
			bsd_enable_sw_init);

	return err;
}

lb_int32 uninit_funcs(void)
{
	lb_int32 err = SUCCESS;

	err |= lb_fmngr_unreg_init_func(pano_enable_sw_init);
	err |= lb_fmngr_unreg_init_func(adas_enable_sw_init);
	err |= lb_fmngr_unreg_init_func(bsd_enable_sw_init);

	return err;
}

lb_int32 resp_funcs(void)
{
	lb_int32 err = SUCCESS;

	err |= lb_reg_resp_msg_func(ADAS_SW,
			adas_sw_resp);
	err |= lb_reg_resp_msg_func(PANO_SW,
			pano_sw_resp);
	err |= lb_reg_resp_msg_func(BSD_SW,
			bsd_sw_resp);

	return err;
}

lb_int32 unresp_funcs(void)
{
	lb_int32 err = SUCCESS;

	err |= lb_unreg_resp_msg_func(adas_sw_resp);
	err |= lb_unreg_resp_msg_func(pano_sw_resp);
	err |= lb_unreg_resp_msg_func(bsd_sw_resp);

	return err;
}
