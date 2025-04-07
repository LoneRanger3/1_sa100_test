/*
 * smart_drive_common.c - the Auxiliary Driving System of car
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

#include <finsh.h>
#include <rtthread.h>
#include <dfs_posix.h>
#include <string.h>
#include "smart_drive.h"
#include "smart_drive_common.h"

static car_obj_t car_obj;

static lb_int32 system_para_array_init(car_obj_t *obj, cJSON *para)
{
	lb_int32 ret = 0;
	lb_int32 i = 0;
	lb_int32 js_size = 0;

	cJSON  *temp0 = NULL;
	cJSON  *temp1 = NULL;

	RT_ASSERT(obj != NULL);
	RT_ASSERT(para != NULL);

	js_size = cJSON_GetArraySize(para);
	if (js_size == 0) {
		printf("[%s,%d,failed]\n", __FILE__, __LINE__);
		ret = -1;
		goto exit;
	}

	for (i = 0; i < js_size; i++) {
		temp0 = cJSON_GetArrayItem(para, i);
		RT_ASSERT(temp0 != NULL);
		RT_ASSERT(temp0->type == cJSON_Object);

		if (cJSON_GetObjectItem(temp0, "rearmirr_enable")) {
			temp1 = cJSON_GetObjectItem(temp0, "rearmirr_enable");
			obj->rearmirr_enable = temp1;
		}
	}

exit:
	return ret;
}

int system_get_rearmirr_enable(void)
{
	RT_ASSERT(car_obj.rearmirr_enable != NULL);

	return car_obj.rearmirr_enable->valueint;
}

static lb_int32 adas_para_array_init(car_obj_t *obj, cJSON *para)
{
	lb_int32 ret = 0;
	lb_int32 i = 0;
	lb_int32 js_size = 0;

	cJSON  *temp0 = NULL;
	cJSON  *temp1 = NULL;

	RT_ASSERT(obj != NULL);
	RT_ASSERT(para != NULL);

	js_size = cJSON_GetArraySize(para);
	if (js_size == 0) {
		printf("[%s,%d,failed]\n", __FILE__, __LINE__);
		ret = -1;
		goto exit;
	}

	for (i = 0; i < js_size; i++) {
		temp0 = cJSON_GetArrayItem(para, i);
		RT_ASSERT(temp0 != NULL);
		RT_ASSERT(temp0->type == cJSON_Object);

		if (cJSON_GetObjectItem(temp0, "adas_enable")) {
			temp1 = cJSON_GetObjectItem(temp0, "adas_enable");
			obj->adas_enable = temp1;
		} else if (cJSON_GetObjectItem(temp0, "carpara_enable")) {
			temp1 = cJSON_GetObjectItem(temp0, "carpara_enable");
			obj->carpara_enable = temp1;
		} else if (cJSON_GetObjectItem(temp0, "carpara_width")) {
			temp1 = cJSON_GetObjectItem(temp0, "carpara_width");
			obj->carpara_width = temp1;
		} else if (cJSON_GetObjectItem(temp0, "roipara_enable")) {
			temp1 = cJSON_GetObjectItem(temp0, "roipara_enable");
			obj->roipara_enable = temp1;
		} else if (cJSON_GetObjectItem(temp0, "roipara_uprow")) {
			temp1 = cJSON_GetObjectItem(temp0, "roipara_uprow");
			obj->roipara_uprow = temp1;
		} else if (cJSON_GetObjectItem(temp0, "roipara_dnrow")) {
			temp1 = cJSON_GetObjectItem(temp0, "roipara_dnrow");
			obj->roipara_dnrow = temp1;
		} else if (cJSON_GetObjectItem(temp0, "camoutpara_cam2middle")) {
			temp1 = cJSON_GetObjectItem(temp0, "camoutpara_cam2middle");
			obj->camoutpara_cam2middle = temp1;
		} else if (cJSON_GetObjectItem(temp0, "lanewarndist")) {
			temp1 = cJSON_GetObjectItem(temp0, "lanewarndist");
			obj->lanewarndist = temp1;
		} else if (cJSON_GetObjectItem(temp0, "fcwsensity")) {
			temp1 = cJSON_GetObjectItem(temp0, "fcwsensity");
			obj->fcwsensity = temp1;
		} else if (cJSON_GetObjectItem(temp0, "ldwsensity")) {
			temp1 = cJSON_GetObjectItem(temp0, "ldwsensity");
			obj->ldwsensity = temp1;
		}
	}

exit:
	return ret;
}

static lb_int32 pano_para_array_init(car_obj_t *obj, cJSON *para)
{
	lb_int32 ret = 0;
	lb_int32 i = 0;
	lb_int32 js_size = 0;

	cJSON  *temp0 = NULL;
	cJSON  *temp1 = NULL;

	RT_ASSERT(obj != NULL);
	RT_ASSERT(para != NULL);

	js_size = cJSON_GetArraySize(para);
	if (js_size == 0) {
		printf("[%s,%d,failed]\n", __FILE__, __LINE__);
		ret = -1;
		goto exit;
	}

	for (i = 0; i < js_size; i++) {
		temp0 = cJSON_GetArrayItem(para, i);
		RT_ASSERT(temp0 != NULL);
		RT_ASSERT(temp0->type == cJSON_Object);

		if (cJSON_GetObjectItem(temp0, "pano_enable")) {
			temp1 = cJSON_GetObjectItem(temp0, "pano_enable");
			obj->pano_enable = temp1;
		} else if (cJSON_GetObjectItem(temp0, "pano_type")) {
			temp1 = cJSON_GetObjectItem(temp0, "pano_type");
			obj->pano_type = temp1;
		} else if (cJSON_GetObjectItem(temp0, "car_width")) {
			temp1 = cJSON_GetObjectItem(temp0, "car_width");
			obj->car_width = temp1;
		} else if (cJSON_GetObjectItem(temp0, "car_length")) {
			temp1 = cJSON_GetObjectItem(temp0, "car_length");
			obj->car_length = temp1;
		} else if (cJSON_GetObjectItem(temp0, "car_distance")) {
			temp1 = cJSON_GetObjectItem(temp0, "car_distance");
			obj->car_distance = temp1;
		} else if (cJSON_GetObjectItem(temp0, "suv_width")) {
			temp1 = cJSON_GetObjectItem(temp0, "suv_width");
			obj->suv_width = temp1;
		} else if (cJSON_GetObjectItem(temp0, "suv_length")) {
			temp1 = cJSON_GetObjectItem(temp0, "suv_length");
			obj->suv_length = temp1;
		} else if (cJSON_GetObjectItem(temp0, "suv_distance")) {
			temp1 = cJSON_GetObjectItem(temp0, "suv_distance");
			obj->suv_distance = temp1;
		} else if (cJSON_GetObjectItem(temp0, "others_width")) {
			temp1 = cJSON_GetObjectItem(temp0, "others_width");
			obj->others_width = temp1;
		} else if (cJSON_GetObjectItem(temp0, "others_length")) {
			temp1 = cJSON_GetObjectItem(temp0, "others_length");
			obj->others_length = temp1;
		} else if (cJSON_GetObjectItem(temp0, "others_distance")) {
			temp1 = cJSON_GetObjectItem(temp0, "others_distance");
			obj->others_distance = temp1;
		}
	}

exit:
	return ret;
}

static lb_int32 area_para_array_init(car_obj_t *obj, cJSON *para, lb_int8 lt)
{
	lb_int32 ret = 0;
	lb_int32 i = 0;
	lb_int32 js_size = 0;

	cJSON  *temp0 = NULL;
	cJSON  *temp1 = NULL;

	RT_ASSERT(obj != NULL);
	RT_ASSERT(para != NULL);

	js_size = cJSON_GetArraySize(para);
	if (js_size == 0) {
		printf("[%s,%d,failed]\n", __FILE__, __LINE__);
		ret = -1;
		goto exit;
	}

	for (i = 0; i < js_size; i++) {
		temp0 = cJSON_GetArrayItem(para, i);
		RT_ASSERT(temp0 != NULL);
		RT_ASSERT(temp0->type == cJSON_Object);

		if (cJSON_GetObjectItem(temp0, "lt_up_x")) {
			temp1 = cJSON_GetObjectItem(temp0, "lt_up_x");
			if (lt == 1)
				obj->lt_area.lt_up_x = temp1;
			else
				obj->rt_area.lt_up_x = temp1;
		} else if (cJSON_GetObjectItem(temp0, "lt_up_y")) {
			temp1 = cJSON_GetObjectItem(temp0, "lt_up_y");
			if (lt == 1)
				obj->lt_area.lt_up_y = temp1;
			else
				obj->rt_area.lt_up_y = temp1;
		} else if (cJSON_GetObjectItem(temp0, "rt_up_x")) {
			temp1 = cJSON_GetObjectItem(temp0, "rt_up_x");
			if (lt == 1)
				obj->lt_area.rt_up_x = temp1;
			else
				obj->rt_area.rt_up_x = temp1;
		} else if (cJSON_GetObjectItem(temp0, "rt_up_y")) {
			temp1 = cJSON_GetObjectItem(temp0, "rt_up_y");
			if (lt == 1)
				obj->lt_area.rt_up_y = temp1;
			else
				obj->rt_area.rt_up_y = temp1;
		} else if (cJSON_GetObjectItem(temp0, "rt_dn_x")) {
			temp1 = cJSON_GetObjectItem(temp0, "rt_dn_x");
			if (lt == 1)
				obj->lt_area.rt_dn_x = temp1;
			else
				obj->rt_area.rt_dn_x = temp1;
		} else if (cJSON_GetObjectItem(temp0, "rt_dn_y")) {
			temp1 = cJSON_GetObjectItem(temp0, "rt_dn_y");
			if (lt == 1)
				obj->lt_area.rt_dn_y = temp1;
			else
				obj->rt_area.rt_dn_y = temp1;
		} else if (cJSON_GetObjectItem(temp0, "lt_dn_x")) {
			temp1 = cJSON_GetObjectItem(temp0, "lt_dn_x");
			if (lt == 1)
				obj->lt_area.lt_dn_x = temp1;
			else
				obj->rt_area.lt_dn_x = temp1;
		} else if (cJSON_GetObjectItem(temp0, "lt_dn_y")) {
			temp1 = cJSON_GetObjectItem(temp0, "lt_dn_y");
			if (lt == 1)
				obj->lt_area.lt_dn_y = temp1;
			else
				obj->rt_area.lt_dn_y = temp1;
		}
	}

exit:
	return ret;
}

static lb_int32 bsd_para_array_init(car_obj_t *obj, cJSON *para)
{
	lb_int32 ret = 0;
	lb_int32 i = 0;
	lb_int32 js_size = 0;

	cJSON  *temp0 = NULL;
	cJSON  *temp1 = NULL;

	RT_ASSERT(obj != NULL);
	RT_ASSERT(para != NULL);

	js_size = cJSON_GetArraySize(para);
	if (js_size == 0) {
		printf("[%s,%d,failed]\n", __FILE__, __LINE__);
		ret = -1;
		goto exit;
	}

	for (i = 0; i < js_size; i++) {
		temp0 = cJSON_GetArrayItem(para, i);
		RT_ASSERT(temp0 != NULL);
		RT_ASSERT(temp0->type == cJSON_Object);

		if (cJSON_GetObjectItem(temp0, "bsd_enable")) {
			temp1 = cJSON_GetObjectItem(temp0, "bsd_enable");
			obj->bsd_enable = temp1;
		} else if (cJSON_GetObjectItem(temp0, "warnarea_enable")) {
			temp1 = cJSON_GetObjectItem(temp0, "warnarea_enable");
			obj->warnarea_enable = temp1;
		} else if (cJSON_GetObjectItem(temp0, "roipara_uprow")) {
			temp1 = cJSON_GetObjectItem(temp0, "roipara_uprow");
			obj->ci_area.roipara_uprow = temp1;
		} else if (cJSON_GetObjectItem(temp0, "roipara_dnrow")) {
			temp1 = cJSON_GetObjectItem(temp0, "roipara_dnrow");
			obj->ci_area.roipara_dnrow = temp1;
		} else if (cJSON_GetObjectItem(temp0, "camoutpara_cam2middle")) {
			temp1 = cJSON_GetObjectItem(temp0, "camoutpara_cam2middle");
			obj->ci_area.camoutpara_cam2middle = temp1;
		} else if (cJSON_GetObjectItem(temp0, "rswsensity")) {
			temp1 = cJSON_GetObjectItem(temp0, "rswsensity");
			obj->ci_area.rswsensity = temp1;
		} else if (cJSON_GetObjectItem(temp0, "rswlevel")) {
			temp1 = cJSON_GetObjectItem(temp0, "rswlevel");
			obj->ci_area.rswlevel = temp1;
		} else {
			temp1 = cJSON_GetObjectItem(temp0, "warnarea_lt");
			if (temp1 == NULL)
				continue;

			obj->warnarea_lt = temp1;

			ret = area_para_array_init(obj, temp1, 1);
			RT_ASSERT(ret == 0);

			temp1 = cJSON_GetObjectItem(temp0, "warnarea_rt");
			if (temp1 == NULL)
				continue;

			obj->warnarea_rt = temp1;

			ret = area_para_array_init(obj, temp1, 0);
			RT_ASSERT(ret == 0);
		}
	}

exit:
	return ret;
}

/**
 * cr_cfg_init - get config parameter from file
 *
 * This function use to init config parameter from config file
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
int adas_para_init(void)
{
	int ret = 0;

	/* capture it from customer path first */
	car_obj.para_root = lb_open_cfg_file(CUSTOMER_CONFIG_PATH);
	if (car_obj.para_root == NULL) {
		/* capture it from original path again */
		car_obj.para_root = lb_open_cfg_file(ORIGINAL_CONFIG_PATH);
		/* capture it failed only exit */
		if (car_obj.para_root == NULL) {
			printf("[%s,%d,failed]\n", __FILE__, __LINE__);
			ret = -1;
			goto exit;
		}
	}
	car_obj.system = cJSON_GetObjectItem(car_obj.para_root, "system");
	RT_ASSERT(car_obj.system != NULL);
	RT_ASSERT(car_obj.system->type == cJSON_Array);

	car_obj.car_adas = cJSON_GetObjectItem(car_obj.para_root, "car_adas");
	RT_ASSERT(car_obj.car_adas != NULL);
	RT_ASSERT(car_obj.car_adas->type == cJSON_Array);
	car_obj.car_pano = cJSON_GetObjectItem(car_obj.para_root, "car_pano");
	RT_ASSERT(car_obj.car_pano != NULL);
	RT_ASSERT(car_obj.car_pano->type == cJSON_Array);
	car_obj.car_bsd = cJSON_GetObjectItem(car_obj.para_root, "car_bsd");
	RT_ASSERT(car_obj.car_bsd != NULL);
	RT_ASSERT(car_obj.car_bsd->type == cJSON_Array);
	ret = system_para_array_init(&car_obj, car_obj.system);
	if (ret != 0) {
		printf("[%s,%d,failed]\n", __FILE__, __LINE__);
		ret = -1;
		goto exit;
	}

	ret = adas_para_array_init(&car_obj, car_obj.car_adas);
	if (ret != 0) {
		printf("[%s,%d,failed]\n", __FILE__, __LINE__);
		ret = -1;
		goto exit;
	}

	ret = pano_para_array_init(&car_obj, car_obj.car_pano);
	if (ret != 0) {
		printf("[%s,%d,failed]\n", __FILE__, __LINE__);
		ret = -1;
		goto exit;
	}

	ret = bsd_para_array_init(&car_obj, car_obj.car_bsd);
	if (ret != 0) {
		printf("[%s,%d,failed]\n", __FILE__, __LINE__);
		ret = -1;
		goto exit;
	}

	return ret;
exit:
	if (car_obj.para_root) {
		lb_exit_cfg_file(car_obj.para_root);
		/* para_root will release inside upper interface */
		car_obj.system = NULL;
		/* para_root will release inside upper interface */
		car_obj.para_root = NULL;
		/* car_adas will release inside upper interface */
		car_obj.car_adas = NULL;
		/* car_pano will release inside upper interface */
		car_obj.car_pano = NULL;
		/* car_bsd will release inside upper interface */
		car_obj.car_bsd = NULL;
	}

	return ret;
}

/**
 * cr_cfg_save - save config parameter to file
 *
 * This function use to save config parameter for config file
 *
 * Returns -1 if called when get error ; otherwise, return 1
 */
int adas_para_save(void)
{
	lb_int32 ret = 0;

	RT_ASSERT(car_obj.para_root != NULL);

	/* we save it to json which is coming from customer path */
	ret = lb_save_cfg_file(CUSTOMER_CONFIG_PATH, car_obj.para_root);
	if (ret != 1)
		ret = -1;
	else
		ret = 0;

	return ret;
}

/**
 * cr_cfg_exit - exit config file
 *
 * This function use to exit config file,if you want save_para,
 * you must call home_cfg_init first
 *
 * Returns  1
 */
int adas_para_exit(void)
{
	lb_int32 ret = 0;

	RT_ASSERT(car_obj.para_root != NULL);

	lb_exit_cfg_file(car_obj.para_root);
	/* para_root will release inside upper interface */
	car_obj.para_root = NULL;
	/* car_adas will release inside upper interface */
	car_obj.car_adas = NULL;
	/* car_pano will release inside upper interface */
	car_obj.car_pano = NULL;

	return ret;
}

int car_save_adas_enable(int  value)
{
	int ret = 0;

	RT_ASSERT(car_obj.adas_enable != NULL);

	car_obj.adas_enable->valueint = value;
	car_obj.adas_enable->valuedouble = value;

	return ret;
}
int car_get_adas_enable(void)
{
	RT_ASSERT(car_obj.adas_enable != NULL);

	return car_obj.adas_enable->valueint;
}

int car_save_pano_enable(int  value)
{
	int ret = 0;

	RT_ASSERT(car_obj.pano_enable != NULL);

	car_obj.pano_enable->valueint = value;
	car_obj.pano_enable->valuedouble = value;

	return ret;
}

int car_get_pano_enable(void)
{
	RT_ASSERT(car_obj.pano_enable != NULL);

	return car_obj.pano_enable->valueint;
}

int car_save_bsd_enable(int  value)
{
	int ret = 0;

	RT_ASSERT(car_obj.bsd_enable != NULL);

	car_obj.bsd_enable->valueint = value;
	car_obj.bsd_enable->valuedouble = value;

	return ret;
}

int car_get_bsd_enable(void)
{
	RT_ASSERT(car_obj.bsd_enable != NULL);

	return car_obj.bsd_enable->valueint;
}

int adas_save_fcwsensity(int value)
{
	int ret = 0;

	RT_ASSERT(car_obj.fcwsensity != NULL);
	if (!value) {
		car_obj.fcwsensity->valueint = 1;
		car_obj.fcwsensity->valuedouble = 1;
	} else if (value == 1) {
		car_obj.fcwsensity->valueint = 2;
		car_obj.fcwsensity->valuedouble = 2;
	} else {
		car_obj.fcwsensity->valueint = 3;
		car_obj.fcwsensity->valuedouble = 3;
	}

	return ret;
}

int adas_get_fcwsensity(void)
{
	RT_ASSERT(car_obj.fcwsensity != NULL);

	return car_obj.fcwsensity->valueint;
}

int adas_save_ldwsensity(int value)
{
	int ret = 0;

	RT_ASSERT(car_obj.ldwsensity != NULL);
	if (!value) {
		car_obj.ldwsensity->valueint = 1;
		car_obj.ldwsensity->valuedouble = 1;
	} else if (value == 1) {
		car_obj.ldwsensity->valueint = 3;
		car_obj.ldwsensity->valuedouble = 3;
	} else {
		car_obj.ldwsensity->valueint = 4;
		car_obj.ldwsensity->valuedouble = 4;
	}

	return ret;
}

int adas_get_ldwsensity(void)
{
	RT_ASSERT(car_obj.ldwsensity != NULL);

	return car_obj.ldwsensity->valueint;
}

int adas_save_carpara(int value)
{
	int ret = 0;

	RT_ASSERT(car_obj.carpara_width != NULL);

	car_obj.carpara_width->valueint = value;
	car_obj.carpara_width->valuedouble = value;

	return ret;
}

int adas_get_carpara(void)
{
	RT_ASSERT(car_obj.carpara_width != NULL);

	return car_obj.carpara_width->valueint;
}

int adas_save_roi_para(int upRow, int dnRow, int midcolumn)
{
	int ret = 0;

	RT_ASSERT(car_obj.roipara_uprow != NULL);
	RT_ASSERT(car_obj.roipara_dnrow != NULL);
	RT_ASSERT(car_obj.camoutpara_cam2middle != NULL);

	car_obj.roipara_uprow->valueint = upRow;
	car_obj.roipara_uprow->valuedouble = upRow;

	car_obj.roipara_dnrow->valueint = dnRow;
	car_obj.roipara_dnrow->valuedouble = dnRow;

	car_obj.camoutpara_cam2middle->valueint = midcolumn;
	car_obj.camoutpara_cam2middle->valuedouble = midcolumn;

	printf("upRow:%d,dnRow:%d midcolumn:%d\n", upRow, dnRow, midcolumn);
	return ret;
}

int adas_get_roi_para_uprow(void)
{
	RT_ASSERT(car_obj.roipara_uprow != NULL);

	return car_obj.roipara_uprow->valueint;
}

int adas_get_roi_para_dnrow(void)
{
	RT_ASSERT(car_obj.roipara_dnrow != NULL);

	return car_obj.roipara_dnrow->valueint;
}

int adas_get_roi_para_midcolumn(void)
{
	RT_ASSERT(car_obj.camoutpara_cam2middle != NULL);

	return car_obj.camoutpara_cam2middle->valueint;
}

int pano_get_type(void)
{
	RT_ASSERT(car_obj.pano_type != NULL);
	return car_obj.pano_type->valuedouble;
}

int pano_save_type(int type)
{
	int ret = 0;

	RT_ASSERT(car_obj.pano_type != NULL);
	car_obj.pano_type->valuedouble = type;

	return ret;
}

double pano_get_width(int motor_type)
{
	if (0 == motor_type) {
		RT_ASSERT(car_obj.car_width != NULL);
		return car_obj.car_width->valuedouble;
	} else if (1 == motor_type) {
		RT_ASSERT(car_obj.suv_width != NULL);
		return car_obj.suv_width->valuedouble;
	} else {
		RT_ASSERT(car_obj.others_width != NULL);
		return car_obj.others_width->valuedouble;
	}
}

int pano_save_width(int motor_type, double width)
{
	int ret = 0;

	if (0 == motor_type) {
		RT_ASSERT(car_obj.car_width != NULL);
		car_obj.car_width->valuedouble = width;
	} else if (1 == motor_type) {
		RT_ASSERT(car_obj.suv_width != NULL);
		car_obj.suv_width->valuedouble = width;
	} else {
		RT_ASSERT(car_obj.others_width != NULL);
		car_obj.others_width->valuedouble = width;
	}

	return ret;
}

double pano_get_length(int motor_type)
{
	if (0 == motor_type) {
		RT_ASSERT(car_obj.car_length != NULL);
		return car_obj.car_length->valuedouble;
	} else if (1 == motor_type) {
		RT_ASSERT(car_obj.suv_length != NULL);
		return car_obj.suv_length->valuedouble;
	} else {
		RT_ASSERT(car_obj.others_length != NULL);
		return car_obj.others_length->valuedouble;
	}
}

int pano_save_length(int motor_type, double length)
{
	int ret = 0;

	if (0 == motor_type) {
		RT_ASSERT(car_obj.car_length != NULL);
		car_obj.car_length->valuedouble = length;
	} else if (1 == motor_type) {
		RT_ASSERT(car_obj.suv_length != NULL);
		car_obj.suv_length->valuedouble = length;
	} else {
		RT_ASSERT(car_obj.others_length != NULL);
		car_obj.others_length->valuedouble = length;
	}

	return ret;
}

int pano_get_distance(int motor_type)
{
	if (0 == motor_type) {
		RT_ASSERT(car_obj.car_distance != NULL);
		return car_obj.car_distance->valuedouble;
	} else if (1 == motor_type) {
		RT_ASSERT(car_obj.suv_distance != NULL);
		return car_obj.suv_distance->valuedouble;
	} else {
		RT_ASSERT(car_obj.others_distance != NULL);
		return car_obj.others_distance->valuedouble;
	}
}

int pano_save_distance(int motor_type, int distance)
{
	int ret = 0;

	if (0 == motor_type) {
		RT_ASSERT(car_obj.car_distance != NULL);
		car_obj.car_distance->valuedouble = distance;
	} else if (1 == motor_type) {
		RT_ASSERT(car_obj.suv_distance != NULL);
		car_obj.suv_distance->valuedouble = distance;
	} else {
		RT_ASSERT(car_obj.others_distance != NULL);
		car_obj.others_distance->valuedouble = distance;
	}

	return ret;
}

int bsd_save_roi_para(int upRow, int dnRow, int midcolumn)
{
	int ret = 0;

	RT_ASSERT(car_obj.ci_area.roipara_uprow != NULL);
	RT_ASSERT(car_obj.ci_area.roipara_dnrow != NULL);
	RT_ASSERT(car_obj.ci_area.camoutpara_cam2middle != NULL);

	car_obj.ci_area.roipara_uprow->valueint = upRow;
	car_obj.ci_area.roipara_uprow->valuedouble = upRow;

	car_obj.ci_area.roipara_dnrow->valueint = dnRow;
	car_obj.ci_area.roipara_dnrow->valuedouble = dnRow;

	car_obj.ci_area.camoutpara_cam2middle->valueint = midcolumn;
	car_obj.ci_area.camoutpara_cam2middle->valuedouble = midcolumn;

	printf("upRow:%d,dnRow:%d midcolumn:%d\n", upRow, dnRow, midcolumn);
	return ret;
}

int bsd_get_roi_para_uprow(void)
{
	RT_ASSERT(car_obj.ci_area.roipara_uprow != NULL);

	return car_obj.ci_area.roipara_uprow->valueint;
}

int bsd_get_roi_para_dnrow(void)
{
	RT_ASSERT(car_obj.ci_area.roipara_dnrow != NULL);

	return car_obj.ci_area.roipara_dnrow->valueint;
}

int bsd_get_roi_para_midcolumn(void)
{
	RT_ASSERT(car_obj.ci_area.camoutpara_cam2middle != NULL);

	return car_obj.ci_area.camoutpara_cam2middle->valueint;
}

int bsd_save_rswsensity(int value)
{
	int ret = 0;

	RT_ASSERT(car_obj.ci_area.rswsensity != NULL);

	if (!value) {
		car_obj.ci_area.rswsensity->valueint = 1;
		car_obj.ci_area.rswsensity->valuedouble = 1;
	} else if (value == 1) {
		car_obj.ci_area.rswsensity->valueint = 2;
		car_obj.ci_area.rswsensity->valuedouble = 2;
	} else if (value == 2) {
		car_obj.ci_area.rswsensity->valueint = 3;
		car_obj.ci_area.rswsensity->valuedouble = 3;
	} else if (value == 3) {
		car_obj.ci_area.rswsensity->valueint = 4;
		car_obj.ci_area.rswsensity->valuedouble = 4;
	} else {
		car_obj.ci_area.rswsensity->valueint = 5;
		car_obj.ci_area.rswsensity->valuedouble = 5;
	}

	return ret;
}

int bsd_get_rswsensity(void)
{
	RT_ASSERT(car_obj.ci_area.rswsensity != NULL);

	return car_obj.ci_area.rswsensity->valueint;
}

int bsd_save_rswlevel(int value)
{
	int ret = 0;

	RT_ASSERT(car_obj.ci_area.rswlevel != NULL);

	if (!value) {
		car_obj.ci_area.rswlevel->valueint = 1;
		car_obj.ci_area.rswlevel->valuedouble = 1;
	} else if (value == 1) {
		car_obj.ci_area.rswlevel->valueint = 2;
		car_obj.ci_area.rswlevel->valuedouble = 2;
	} else if (value == 2) {
		car_obj.ci_area.rswlevel->valueint = 3;
		car_obj.ci_area.rswlevel->valuedouble = 3;
	} else if (value == 3) {
		car_obj.ci_area.rswlevel->valueint = 4;
		car_obj.ci_area.rswlevel->valuedouble = 4;
	} else {
		car_obj.ci_area.rswlevel->valueint = 5;
		car_obj.ci_area.rswlevel->valuedouble = 5;
	}

	return ret;
}

int bsd_get_rswlevel(void)
{
	RT_ASSERT(car_obj.ci_area.rswlevel != NULL);

	return car_obj.ci_area.rswlevel->valueint;
}

int bsd_save_area_lt_up(int x, int y, int lt)
{
	if (lt == 1) {
		RT_ASSERT(car_obj.lt_area.lt_up_x);
		RT_ASSERT(car_obj.lt_area.lt_up_y);

		car_obj.lt_area.lt_up_x->valueint = x;
		car_obj.lt_area.lt_up_x->valuedouble = x;

		car_obj.lt_area.lt_up_y->valueint = y;
		car_obj.lt_area.lt_up_y->valuedouble = y;
	} else {
		RT_ASSERT(car_obj.rt_area.lt_up_x);
		RT_ASSERT(car_obj.rt_area.lt_up_y);

		car_obj.rt_area.lt_up_x->valueint = x;
		car_obj.rt_area.lt_up_x->valuedouble = x;

		car_obj.rt_area.lt_up_y->valueint = y;
		car_obj.rt_area.lt_up_y->valuedouble = y;
	}

	return 0;
}

int bsd_save_area_rt_up(int x, int y, int lt)
{
	if (lt == 1) {
		RT_ASSERT(car_obj.lt_area.rt_up_x);
		RT_ASSERT(car_obj.lt_area.rt_up_y);

		car_obj.lt_area.rt_up_x->valueint = x;
		car_obj.lt_area.rt_up_x->valuedouble = x;

		car_obj.lt_area.rt_up_y->valueint = y;
		car_obj.lt_area.rt_up_y->valuedouble = y;
	} else {
		RT_ASSERT(car_obj.rt_area.rt_up_x);
		RT_ASSERT(car_obj.rt_area.rt_up_y);

		car_obj.rt_area.rt_up_x->valueint = x;
		car_obj.rt_area.rt_up_x->valuedouble = x;

		car_obj.rt_area.rt_up_y->valueint = y;
		car_obj.rt_area.rt_up_y->valuedouble = y;
	}

	return 0;
}

int bsd_save_area_rt_dn(int x, int y, int lt)
{
	if (lt == 1) {
		RT_ASSERT(car_obj.lt_area.rt_dn_x);
		RT_ASSERT(car_obj.lt_area.rt_dn_y);

		car_obj.lt_area.rt_dn_x->valueint = x;
		car_obj.lt_area.rt_dn_x->valuedouble = x;

		car_obj.lt_area.rt_dn_y->valueint = y;
		car_obj.lt_area.rt_dn_y->valuedouble = y;
	} else {
		RT_ASSERT(car_obj.rt_area.rt_dn_x);
		RT_ASSERT(car_obj.rt_area.rt_dn_y);

		car_obj.rt_area.rt_dn_x->valueint = x;
		car_obj.rt_area.rt_dn_x->valuedouble = x;

		car_obj.rt_area.rt_dn_y->valueint = y;
		car_obj.rt_area.rt_dn_y->valuedouble = y;
	}

	return 0;
}

int bsd_save_area_lt_dn(int x, int y, int lt)
{
	if (lt == 1) {
		RT_ASSERT(car_obj.lt_area.lt_dn_x);
		RT_ASSERT(car_obj.lt_area.lt_dn_y);

		car_obj.lt_area.lt_dn_x->valueint = x;
		car_obj.lt_area.lt_dn_x->valuedouble = x;

		car_obj.lt_area.lt_dn_y->valueint = y;
		car_obj.lt_area.lt_dn_y->valuedouble = y;
	} else {
		RT_ASSERT(car_obj.rt_area.lt_dn_x);
		RT_ASSERT(car_obj.rt_area.lt_dn_y);

		car_obj.rt_area.lt_dn_x->valueint = x;
		car_obj.rt_area.lt_dn_x->valuedouble = x;

		car_obj.rt_area.lt_dn_y->valueint = y;
		car_obj.rt_area.lt_dn_y->valuedouble = y;
	}

	return 0;
}

int bsd_get_area_lt_up_x(int lt)
{
	if (lt == 1) {
		RT_ASSERT(car_obj.lt_area.lt_up_x);
		return car_obj.lt_area.lt_up_x->valueint;
	} else {
		RT_ASSERT(car_obj.rt_area.lt_up_x);
		return car_obj.rt_area.lt_up_x->valueint;
	}
}

int bsd_get_area_lt_up_y(int lt)
{
	if (lt == 1) {
		RT_ASSERT(car_obj.lt_area.lt_up_y);
		return car_obj.lt_area.lt_up_y->valueint;
	} else {
		RT_ASSERT(car_obj.rt_area.lt_up_y);
		return car_obj.rt_area.lt_up_y->valueint;
	}
}

int bsd_get_area_rt_up_x(int lt)
{
	if (lt == 1) {
		RT_ASSERT(car_obj.lt_area.rt_up_x);
		return car_obj.lt_area.rt_up_x->valueint;
	} else {
		RT_ASSERT(car_obj.rt_area.rt_up_x);
		return car_obj.rt_area.rt_up_x->valueint;
	}
}

int bsd_get_area_rt_up_y(int lt)
{
	if (lt == 1) {
		RT_ASSERT(car_obj.lt_area.rt_up_y);
		return car_obj.lt_area.rt_up_y->valueint;
	} else {
		RT_ASSERT(car_obj.rt_area.rt_up_y);
		return car_obj.rt_area.rt_up_y->valueint;
	}
}

int bsd_get_area_rt_dn_x(int lt)
{
	if (lt == 1) {
		RT_ASSERT(car_obj.lt_area.rt_dn_x);
		return car_obj.lt_area.rt_dn_x->valueint;

	} else {
		RT_ASSERT(car_obj.rt_area.rt_dn_x);
		return car_obj.rt_area.rt_dn_x->valueint;
	}

}

int bsd_get_area_rt_dn_y(int lt)
{
	if (lt == 1) {
		RT_ASSERT(car_obj.lt_area.rt_dn_y);
		return car_obj.lt_area.rt_dn_y->valueint;
	} else {
		RT_ASSERT(car_obj.rt_area.rt_dn_y);
		return car_obj.rt_area.rt_dn_y->valueint;
	}
}

int bsd_get_area_lt_dn_x(int lt)
{
	if (lt == 1) {
		RT_ASSERT(car_obj.lt_area.lt_dn_x);
		return car_obj.lt_area.lt_dn_x->valueint;
	} else {
		RT_ASSERT(car_obj.rt_area.lt_dn_x);
		return car_obj.rt_area.lt_dn_x->valueint;
	}
}

int bsd_get_area_lt_dn_y(int lt)
{
	if (lt == 1) {
		RT_ASSERT(car_obj.lt_area.lt_dn_y);
		return car_obj.lt_area.lt_dn_y->valueint;
	} else {
		RT_ASSERT(car_obj.rt_area.lt_dn_y);
		return car_obj.rt_area.lt_dn_y->valueint;
	}
}
