/*
 * bsd_set.c - setting of bsd
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
#include "libbsd.h"
#include "car_bsd_lib.h"
#include "car_bsd.h"
#include "frame_queue.h"
#include "car_bsd_set.h"
#include "eos.h"
#include <debug.h>

static bsd_para_t bsd_para; /* car bsd parameter struct */
static bsd_jobj_t bsd_jobj; /* car bsd json object struct */

static lb_int32 bsd_area_para_array_init(bsd_para_t *bsd_para, cJSON *para, lb_int8 lt)
{
	lb_int32 ret = 0;
	lb_int32 i = 0;
	lb_int32 js_size = 0;

	cJSON  *temp0 = NULL;
	cJSON  *temp1 = NULL;

	RT_ASSERT(bsd_para != NULL);
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
			RT_ASSERT(temp1 != NULL);
			if (lt == 0)
				bsd_para->warnarea_lt[0].x = temp1->valueint;
			else
				bsd_para->warnarea_rt[0].x = temp1->valueint;
		} else if (cJSON_GetObjectItem(temp0, "lt_up_y")) {
			temp1 = cJSON_GetObjectItem(temp0, "lt_up_y");
			RT_ASSERT(temp1 != NULL);
			if (lt == 0)
				bsd_para->warnarea_lt[0].y = temp1->valueint-60;
			else
				bsd_para->warnarea_rt[0].y = temp1->valueint-60;
		} else if (cJSON_GetObjectItem(temp0, "rt_up_x")) {
			temp1 = cJSON_GetObjectItem(temp0, "rt_up_x");
			RT_ASSERT(temp1 != NULL);
			if (lt == 0)
				bsd_para->warnarea_lt[1].x = temp1->valueint;
			else
				bsd_para->warnarea_rt[1].x = temp1->valueint;
		} else if (cJSON_GetObjectItem(temp0, "rt_up_y")) {
			temp1 = cJSON_GetObjectItem(temp0, "rt_up_y");
			RT_ASSERT(temp1 != NULL);
			if (lt == 0)
				bsd_para->warnarea_lt[1].y = temp1->valueint-60;
			else
				bsd_para->warnarea_rt[1].y = temp1->valueint-60;
		} else if (cJSON_GetObjectItem(temp0, "rt_dn_x")) {
			temp1 = cJSON_GetObjectItem(temp0, "rt_dn_x");
			RT_ASSERT(temp1 != NULL);
			if (lt == 0)
				bsd_para->warnarea_lt[2].x = temp1->valueint;
			else
				bsd_para->warnarea_rt[2].x = temp1->valueint;
		} else if (cJSON_GetObjectItem(temp0, "rt_dn_y")) {
			temp1 = cJSON_GetObjectItem(temp0, "rt_dn_y");
			RT_ASSERT(temp1 != NULL);
			if (lt == 0)
				bsd_para->warnarea_lt[2].y = temp1->valueint-60;
			else
				bsd_para->warnarea_rt[2].y = temp1->valueint-60;
		} else if (cJSON_GetObjectItem(temp0, "lt_dn_x")) {
			temp1 = cJSON_GetObjectItem(temp0, "lt_dn_x");
			RT_ASSERT(temp1 != NULL);
			if (lt == 0)
				bsd_para->warnarea_lt[3].x = temp1->valueint;
			else
				bsd_para->warnarea_rt[3].x = temp1->valueint;
		} else if (cJSON_GetObjectItem(temp0, "lt_dn_y")) {
			temp1 = cJSON_GetObjectItem(temp0, "lt_dn_y");
			RT_ASSERT(temp1 != NULL);
			if (lt == 0)
				bsd_para->warnarea_lt[3].y = temp1->valueint-60;
			else
				bsd_para->warnarea_rt[3].y = temp1->valueint-60;
		}
	}

exit:
	return ret;
}

static void bsd_para_array_init(cJSON *para)
{
	lb_int32 i = 0;
	lb_int32 ret = 0;

	for (i = 0; i < cJSON_GetArraySize(para); i++) {
		cJSON *temp;
		temp = cJSON_GetArrayItem(para, i);
		if (temp && temp->type == cJSON_Object) {
			if (cJSON_GetObjectItem(temp, "bsd_enable")) {
				bsd_jobj.bsd_enable =
					cJSON_GetObjectItem(temp, "bsd_enable");
				bsd_para.bsd_enable =
					bsd_jobj.bsd_enable->valueint;
				MOD_LOG_D("bsd_enable %d\n",
					bsd_para.bsd_enable);
			} else if (cJSON_GetObjectItem(temp, "lanewarnDist")) {
				bsd_jobj.lanewarndist =
					cJSON_GetObjectItem(temp, "lanewarndist");
				bsd_para.lanewarndist =
					bsd_jobj.lanewarndist->valueint;
				MOD_LOG_D("lanewarndist %d\n",
					bsd_para.lanewarndist);
			} else if (cJSON_GetObjectItem(temp, "pano_type")) {
				bsd_jobj.car_type = cJSON_GetObjectItem(temp,
					"pano_type");
				bsd_para.car_type = bsd_jobj.car_type->valueint;
				MOD_LOG_D("car_type %d\n", bsd_para.car_type);
			} else if (!bsd_para.car_type && cJSON_GetObjectItem(temp,
			"car_width")) {
				bsd_jobj.car_width = cJSON_GetObjectItem(temp,
					"car_width");
				bsd_para.car_width = bsd_jobj.car_width->valuedouble
					* 100;
				MOD_LOG_D("car_width %d\n", bsd_para.car_width);
			} else if (bsd_para.car_type == 1 && cJSON_GetObjectItem(temp,
			"suv_width")) {
				bsd_jobj.car_width = cJSON_GetObjectItem(temp,
					"suv_width");
				bsd_para.car_width = bsd_jobj.car_width->valuedouble
					* 100;
				MOD_LOG_D("car_width %d\n", bsd_para.car_width);
			}  else if (bsd_para.car_type == 2 && cJSON_GetObjectItem(temp,
			"others_width")) {
				bsd_jobj.car_width = cJSON_GetObjectItem(temp,
					"others_width");
				bsd_para.car_width = bsd_jobj.car_width->valuedouble
					* 100;
				MOD_LOG_D("car_width %d\n", bsd_para.car_width);
			} else if (cJSON_GetObjectItem(temp, "roipara_enable")) {
				bsd_jobj.roipara_enable =
					cJSON_GetObjectItem(temp, "roipara_enable");
				bsd_para.roipara_enable =
					bsd_jobj.roipara_enable->valueint;
				MOD_LOG_D("roipara_enable %d\n",
					bsd_para.roipara_enable);
			} else if (cJSON_GetObjectItem(temp, "roipara_uprow")) {
				bsd_jobj.roipara_uprow =
					cJSON_GetObjectItem(temp, "roipara_uprow");
				bsd_para.roipara_uprow =
					bsd_jobj.roipara_uprow->valueint-60;
				MOD_LOG_D("roipara_uprow %d\n",
					bsd_para.roipara_uprow);
			} else if (cJSON_GetObjectItem(temp, "roipara_dnrow")) {
				bsd_jobj.roipara_dnrow =
					cJSON_GetObjectItem(temp, "roipara_dnrow");
				bsd_para.roipara_dnrow =
					bsd_jobj.roipara_dnrow->valueint-60;
				MOD_LOG_D("roipara_dnrow %d\n",
					bsd_para.roipara_dnrow);
			} else if (cJSON_GetObjectItem(temp,
					"camoutpara_cam2middle")) {
				bsd_jobj.camoutpara_cam2middle =
					cJSON_GetObjectItem(temp,
						"camoutpara_cam2middle");
				bsd_para.camoutpara_cam2middle =
					bsd_jobj.camoutpara_cam2middle->valueint;
				MOD_LOG_D("camoutpara_cam2middle %d\n",
					bsd_para.camoutpara_cam2middle);
			} else if (cJSON_GetObjectItem(temp, "fcwsensity")) {
				bsd_jobj.fcwsensity =
					cJSON_GetObjectItem(temp, "fcwsensity");
				bsd_para.fcwsensity =
					bsd_jobj.fcwsensity->valueint;
				MOD_LOG_D("fcwsensity %d\n",
					bsd_para.fcwsensity);
			} else if (cJSON_GetObjectItem(temp, "ldwsensity")) {
				bsd_jobj.ldwsensity =
					cJSON_GetObjectItem(temp, "ldwsensity");
				bsd_para.ldwsensity =
					bsd_jobj.ldwsensity->valueint;
				MOD_LOG_D("ldwsensity %d\n",
					bsd_para.ldwsensity);
			} else if (cJSON_GetObjectItem(temp, "moduleenable_ingps")) {
				bsd_jobj.gps_enable =
					cJSON_GetObjectItem(temp, "moduleenable_ingps");
				bsd_para.gps_enable =
					bsd_jobj.gps_enable->valueint;
				MOD_LOG_D("gps_enable %d\n", bsd_para.gps_enable);
			} else if (cJSON_GetObjectItem(temp, "caminpara_fps")) {
				bsd_jobj.caminpara_fps =
					cJSON_GetObjectItem(temp, "caminpara_fps");
				bsd_para.caminpara_fps =
					bsd_jobj.caminpara_fps->valueint;
				MOD_LOG_D("caminpara_fps %d\n", bsd_para.caminpara_fps);
			} else if (cJSON_GetObjectItem(temp, "caminpara_imgsize_w")) {
				bsd_jobj.caminpara_imgsize_w =
					cJSON_GetObjectItem(temp, "caminpara_imgsize_w");
				bsd_para.caminpara_imgsize_w =
					bsd_jobj.caminpara_imgsize_w->valueint;
				MOD_LOG_D("caminpara_imgsize_w %d\n",
					bsd_para.caminpara_imgsize_w);
			} else if (cJSON_GetObjectItem(temp, "caminpara_imgsize_h")) {
				bsd_jobj.caminpara_imgsize_h =
					cJSON_GetObjectItem(temp, "caminpara_imgsize_h");
				bsd_para.caminpara_imgsize_h =
					bsd_jobj.caminpara_imgsize_h->valueint;
				MOD_LOG_D("caminpara_imgsize_h %d\n",
					bsd_para.caminpara_imgsize_h);
			} else if (cJSON_GetObjectItem(temp, "caminpara_angh")) {
				bsd_jobj.caminpara_angh =
					cJSON_GetObjectItem(temp, "caminpara_angh");
				bsd_para.caminpara_angh =
					bsd_jobj.caminpara_angh->valueint;
				MOD_LOG_D("caminpara_fps %d\n", bsd_para.caminpara_fps);
			} else if (cJSON_GetObjectItem(temp, "caminpara_angv")) {
				bsd_jobj.caminpara_angv =
					cJSON_GetObjectItem(temp, "caminpara_angv");
				bsd_para.caminpara_angv =
					bsd_jobj.caminpara_angv->valueint;
				MOD_LOG_D("caminpara_fps %d\n", bsd_para.caminpara_fps);
			} else if (cJSON_GetObjectItem(temp, "rswsensity")) {
				bsd_jobj.alarm_sensity =
					cJSON_GetObjectItem(temp, "rswsensity");
				bsd_para.alarm_sensity =
					bsd_jobj.alarm_sensity->valueint;
				MOD_LOG_D("alarm_sensity %d\n", bsd_para.alarm_sensity);
			} else if (cJSON_GetObjectItem(temp, "warnarea_enable")) {
				bsd_jobj.warnarea_enable =
					cJSON_GetObjectItem(temp, "warnarea_enable");
				bsd_para.warnarea_enable =
					bsd_jobj.warnarea_enable->valueint;
				MOD_LOG_D("warnarea_enable %d\n",
					bsd_para.warnarea_enable);
			} else if (cJSON_GetObjectItem(temp, "warnarea_lt")) {
				bsd_jobj.warnarea_lt = cJSON_GetObjectItem(temp,
					"warnarea_lt");

				ret = bsd_area_para_array_init(&bsd_para,
					bsd_jobj.warnarea_lt, 0);
				RT_ASSERT(ret == 0);

				bsd_jobj.warnarea_rt = cJSON_GetObjectItem(temp,
					"warnarea_rt");
				if (bsd_jobj.warnarea_rt == NULL)
					continue;
				ret = bsd_area_para_array_init(&bsd_para,
					bsd_jobj.warnarea_rt, 1);
				RT_ASSERT(ret == 0);
			}
		} else if (temp && temp->type == cJSON_Array)
			bsd_para_array_init(temp);
	}
}

/**
 * bsd_para_init - get config parameter from file
 *
 * This function use to init config parameter from config file
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
int bsd_para_init(void)
{
	int ret = 0;

#ifdef ARCH_LOMBO_N7_CDR_MMC
	bsd_jobj.para_root = lb_open_cfg_file("/mnt/sdcard/etc/cdr_config1.cfg");
#else
	bsd_jobj.para_root = lb_open_cfg_file("/mnt/data/cdr_config.cfg");
#endif
	if (bsd_jobj.para_root == NULL) {
		bsd_jobj.para_root =
			lb_open_cfg_file(ROOTFS_MOUNT_PATH"/etc/cdr_config.cfg");
		if (bsd_jobj.para_root == NULL) {
			MOD_LOG_E("err bsd_jobj.para_root is NULL");
			return -1;
		}
	}
	bsd_jobj.car_bsd =
		cJSON_GetObjectItem(bsd_jobj.para_root, "car_bsd");
	if (bsd_jobj.car_bsd == NULL)
		return -1;

	if (bsd_jobj.car_bsd && bsd_jobj.car_bsd->type == cJSON_Array)
		bsd_para_array_init(bsd_jobj.car_bsd);

	bsd_jobj.car_pano = cJSON_GetObjectItem(bsd_jobj.para_root, "car_pano");
	if (bsd_jobj.car_pano == NULL)
		return -1;

	if (bsd_jobj.car_pano && bsd_jobj.car_pano->type == cJSON_Array)
		bsd_para_array_init(bsd_jobj.car_pano);

	return ret;
}

/**
 * bsd_para_exit - exit config file
 *
 * This function use to exit config file
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
int bsd_para_exit(void)
{
	int ret = -1;
	MOD_LOG_D("bsd_jobj.para_root %p\n", bsd_jobj.para_root);
	if (bsd_jobj.para_root)
		ret = lb_exit_cfg_file(bsd_jobj.para_root);
	else {
		MOD_LOG_E("err bsd_jobj.para_root is NULL");
		return -1;
	}

	return ret;
}

/**
 * bsd_get_bsd_enable - get bsd status
 *
 * This function use to get bsd status
 *
 * Return car recorder status,START or STOP
 */
int bsd_get_bsd_enable()
{
	return bsd_para.bsd_enable;
}

/**
 * bsd_get_gps_enable - get gps status
 *
 * This function use to get gps status
 *
 * Return car gps enable status,ENABLE or DISABLE
 */
int bsd_get_gps_enable()
{
	return bsd_para.gps_enable;
}

/**
 * bsd_get_time_stamp - get time stamp
 *
 * This function use to get time stamp
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
int bsd_get_time_stamp(HDFrameSetBsd *set_data)
{
	int ret = -1;

	set_data->timeStamp = 0;

	ret = 0;
	return ret;
}

/**
 * bsd_get_day_or_night - get day or night
 *
 * This function use to get day or night
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
int bsd_get_day_or_night(HDFrameSetBsd *set_data)
{
	int ret = -1;

	/*
	 * +1 means day
	 * -1 means night
	 */
	set_data->isDay = 0;

	ret = 0;
	return ret;
}

/**
* bsd_set_cpuGear - set cpu load level
*
* This function use to set cpu load level
*
* Returns -1 if called when get error ; otherwise, return 0
*/
int bsd_set_cpuGear(HDFrameSetBsd *set_data)
{
	int ret = -1;

	/*
	 * -1 means low
	 * 0 means middle
	 * 1 means high
	 */
	set_data->cpuGear = 0;

	ret = 0;
	return ret;
}

/**
 * bsd_get_vedio_image_info - get vedio image info
 *
 * This function use to set cpu load level
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
int bsd_get_vedio_image_info(HDFrameSetBsd *set_data)
{
	int ret = -1;
	if (bsd_para.caminpara_imgsize_w)
		set_data->imgYuv[MAJOR_CAM].size.width = bsd_para.caminpara_imgsize_w;
	else
		set_data->imgYuv[MAJOR_CAM].size.width = REAR_PREVIEW_WIDTH;
	if (bsd_para.caminpara_imgsize_h)
		set_data->imgYuv[MAJOR_CAM].size.height = bsd_para.caminpara_imgsize_h;
	else
		set_data->imgYuv[MAJOR_CAM].size.height = REAR_PREVIEW_HEIGHT;

	ret = 0;

	return ret;
}

/**
 * bsd_get_gps_info - get gps info
 *
 * This function use to get gps info
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
int bsd_get_gps_info(HDFrameSetBsd *set_data)
{
	int ret = -1;

	if (!bsd_get_gps_enable()) {
		set_data->gps.enable = 0;
		set_data->gps.speed = 60;
	} else {
#ifdef LOMBO_GPS
		struct gps_data_t g;
		rt_bool_t status;
		set_data->gps.enable = 1;
		set_data->gps.speed = 60;
		g = gps_get_data();
		status = gps_connect_status();
		/* MOD_LOG_D("GPS device connect status: %d\n", status); */
		if (!status) {
			/* MOD_LOG_D("GPS device not connected!\n"); */
			ret = 0;
			return ret;
		}
		if (g.valid) {
			MOD_LOG_D("long_type: %c, speed: %d\n", g.long_type, g.speed);
			set_data->gps.enable = 1;
			set_data->gps.speed = g.speed;
		} else {
			MOD_LOG_D("gps data invalid\n");
			set_data->gps.enable = 0;
			set_data->gps.speed = 0;
		}
#else
	set_data->gps.enable = 0;
	set_data->gps.speed = 60;
#endif
	}
	ret = 0;
	return ret;
}

/**
 * bsd_get_gsensor_info - get gsensor info
 *
 * This function use to get gsensor info
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
int bsd_get_gsensor_info(HDFrameSetBsd *set_data)
{
	int ret = -1;

	set_data->gsensor.acct = 0;
	set_data->gsensor.speed = 0;
	set_data->gsensor.stop = 0;

	ret = 0;
	return ret;
}

/**
 * bsd_get_obd_para - get obd para
 *
 * This function use to get gsensor para
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
int bsd_get_obd_para(HDFrameSetBsd *set_data)
{
	int ret = -1;

	memset(&(set_data->obd), 0x00, sizeof(HDObdBsd));

	ret = 0;
	return ret;
}

/**
* bsd_set_warn_sensity - set warn sensity
*
* This function use to set warn sensity
*
* Returns -1 if called when get error ; otherwise, return 0
*/
int bsd_set_warn_sensity(HDFrameSetBsd *set_data)
{
	int ret = -1;

	/*
	 * car_sensity:collision distance early warning sensitivity
	 * 0 - warnning before 4s
	 * 1 - warnning before 3s
	 * 2 - warnning before 2s
	 * lane_sensiry:sensitivity of lane deviation warning
	 * 0 - warnning before 2s
	 * 1 - warnning before 2.5s
	 * 2 - warnning before 3s
	 */
	memset(&(set_data->warnSensity), 0x00, sizeof(HDWarnSensityBsd));
	set_data->warnSensity.fcwSensity = bsd_para.fcwsensity;
	set_data->warnSensity.ldwSensity = bsd_para.ldwsensity;

	ret = 0;
	return ret;
}

/**
 * bsd_get_run_ped - get run ped
 *
 * This function use to get run ped
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
int bsd_get_run_ped(HDFrameSetBsd *set_data)
{
	int ret = -1;

	set_data->runPed = 0;

	ret = 0;
	return ret;
}

/**
 * bsd_module_enable - enbale some modules
 *
 * This function use to enbale some modules
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
int bsd_module_enable(HDIniSetBsd *set_data)
{
	int ret = -1;

	set_data->moduleEnable.inGps = bsd_get_gps_enable();
	set_data->moduleEnable.inGsensor = 0;
	set_data->moduleEnable.inObd = 0;
	set_data->moduleEnable.inCamOutPara = 1;

	ret = 0;
	return ret;
}

/**
 * bsd_set_carpara - set car para
 *
 * This function use to set car para
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
int bsd_set_carpara(HDIniSetBsd *set_data)
{
	int ret = 0;
#ifdef CAPTURE_SOURECE
	set_data->carPara.enable = 1;
	if (set_data->carPara.enable) {
		set_data->carPara.width = bsd_para.car_width;
	} else {
		set_data->carPara.enable = 1;
		set_data->carPara.width = 180;
	}
#else
	set_data->carPara.enable = 1;
	set_data->carPara.width = 180;
#endif
	return ret;
}

/**
 * bsd_set_roi_para - set roi para
 *
 * This function use to set roi para
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
int bsd_set_roi_para(HDIniSetBsd *set_data)
{
	int ret = -1;

#ifdef CAPTURE_SOURECE
	ASSERT_BSD_PARA(((bsd_para.roipara_enable == 1) ||
			(bsd_para.roipara_enable == 0)));
	ASSERT_BSD_PARA(bsd_para.roipara_uprow >= 0);
	ASSERT_BSD_PARA(bsd_para.roipara_dnrow >= 0);

	set_data->roiPara.enable = bsd_para.roipara_enable;
	if (set_data->roiPara.enable) {
		set_data->roiPara.upRow = bsd_para.roipara_uprow *
			REAR_PREVIEW_HEIGHT / BSD_CALIB_PREW;/* sky_line_y; */
		set_data->roiPara.dnRow = bsd_para.roipara_dnrow *
			REAR_PREVIEW_HEIGHT / BSD_CALIB_PREW;/* hood_line_y */
	} else {
		set_data->roiPara.enable = 1;
		set_data->roiPara.upRow = 360;/*sky_line_y;*/
		set_data->roiPara.dnRow = 680;/*hood_line_y;*/
	}
#else
	set_data->roiPara.enable = 1;
	set_data->roiPara.upRow = 360;/*sky_line_y;*/
	set_data->roiPara.dnRow = 680;/*hood_line_y;*/
#endif
	ret = 0;
	return ret;
}

/**
 * bsd_set_cam_out_para - set cam out para
 *
 * This function use to set cam out para
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
int bsd_set_cam_out_para(HDIniSetBsd *set_data)
{
	int ret = -1;
	int cali_width = BSD_CALIB_PREH;

	set_data->cameraPara[MAJOR_CAM].camOutPara.enable = 1;
	set_data->cameraPara[MAJOR_CAM].camOutPara.cam2ground = 130;

#ifdef CAPTURE_SOURECE
	set_data->cameraPara[MAJOR_CAM].camOutPara.cam2middle =
		(bsd_para.camoutpara_cam2middle - cali_width / 2) * REAR_PREVIEW_WIDTH /
		cali_width;
	if (set_data->cameraPara[MAJOR_CAM].camOutPara.cam2middle > 100)
		set_data->cameraPara[MAJOR_CAM].camOutPara.cam2middle = 100;
	else if (set_data->cameraPara[MAJOR_CAM].camOutPara.cam2middle < -100)
		set_data->cameraPara[MAJOR_CAM].camOutPara.cam2middle = -100;
#else
	set_data->cameraPara[MAJOR_CAM].camOutPara.cam2middle = 0;
#endif
	set_data->cameraPara[MAJOR_CAM].camOutPara.cam2head = 150;
	set_data->cameraPara[MAJOR_CAM].camOutPara.laneCrossPoint.x =
		bsd_para.camoutpara_cam2middle *
		REAR_PREVIEW_WIDTH / cali_width;
	set_data->cameraPara[MAJOR_CAM].camOutPara.laneCrossPoint.y =
		bsd_para.roipara_uprow * REAR_PREVIEW_HEIGHT / BSD_CALIB_PREW;

	ret = 0;
	return ret;
}

/**
 * bsd_set_cam_in_para - set cam in para
 *
 * This function use to set cam in para
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
int bsd_set_cam_in_para(HDIniSetBsd *set_data)
{
	int ret = -1;

	if (bsd_para.caminpara_fps)
		set_data->cameraPara[MAJOR_CAM].camInPara.fps = bsd_para.caminpara_fps;
	else
		set_data->cameraPara[MAJOR_CAM].camInPara.fps = 30;
	if (bsd_para.caminpara_imgsize_w)
		set_data->cameraPara[MAJOR_CAM].camInPara.imgSize.width =
		bsd_para.caminpara_imgsize_w;
	else
		set_data->cameraPara[MAJOR_CAM].camInPara.imgSize.width =
		REAR_PREVIEW_WIDTH;
	if (bsd_para.caminpara_imgsize_h)
		set_data->cameraPara[MAJOR_CAM].camInPara.imgSize.height =
		bsd_para.caminpara_imgsize_h;
	else
		set_data->cameraPara[MAJOR_CAM].camInPara.imgSize.height =
		REAR_PREVIEW_HEIGHT;
	if (bsd_para.caminpara_angh)
		set_data->cameraPara[MAJOR_CAM].camInPara.viewAng.angH =
		bsd_para.caminpara_angh;
	else
		set_data->cameraPara[MAJOR_CAM].camInPara.viewAng.angH = 130;
	if (bsd_para.caminpara_angv)
		set_data->cameraPara[MAJOR_CAM].camInPara.viewAng.angV =
		bsd_para.caminpara_angv;
	else
		set_data->cameraPara[MAJOR_CAM].camInPara.viewAng.angV = 70;

	ret = 0;

	return ret;
}

/**
 * bsd_set_ped_para - set ped para
 *
 * This function use to set ped para
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
int bsd_set_ped_para(HDIniSetBsd *set_data)
{
	int ret = -1;

	set_data->laneWarnDist = bsd_para.lanewarndist;

	ret = 0;
	return ret;
}
static int alarm_sensity_map(void)
{
	int ret;

	if (bsd_para.alarm_sensity == 1)
		ret = -100;
	else if (bsd_para.alarm_sensity == 2)
		ret = -20;
	else if (bsd_para.alarm_sensity == 3)
		ret = 10;
	else
		ret = -20;
	MOD_LOG_E("ret:%d\n", ret);

	return ret;
}

int bsd_set_alarm_sensity(HDIniSetBsd *set_data)
{
	int ret = -1;

	set_data->bsdSensity = alarm_sensity_map();

	ret = 0;
	return ret;
}

/**
 * bsd_set_ped_para - set ped para
 *
 * This function use to set ped para
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
int bsd_set_warn_area(HDIniSetBsd *set_data)
{
	int ret = -1;
	int i = 0;

	set_data->warnArea.enable = bsd_para.warnarea_enable;
	MOD_LOG_E("set_data->warnArea.enable:%d\n", set_data->warnArea.enable);
	for (i = 0; i < 4; i++) {
		set_data->warnArea.ltArea[i].x = bsd_para.warnarea_lt[i].x *
			REAR_PREVIEW_HEIGHT / BSD_CALIB_PREW;
		set_data->warnArea.ltArea[i].y = bsd_para.warnarea_lt[i].y *
			REAR_PREVIEW_HEIGHT / BSD_CALIB_PREW;
		set_data->warnArea.rtArea[i].x = bsd_para.warnarea_rt[i].x *
			REAR_PREVIEW_HEIGHT / BSD_CALIB_PREW;
		set_data->warnArea.rtArea[i].y = bsd_para.warnarea_rt[i].y *
			REAR_PREVIEW_HEIGHT / BSD_CALIB_PREW;
		MOD_LOG_E("[%d]:x:%d y:%d\n", i, set_data->warnArea.ltArea[i].x,
			set_data->warnArea.ltArea[i].y);
		MOD_LOG_E("[%d]:x:%d y:%d\n", i, set_data->warnArea.rtArea[i].x,
		set_data->warnArea.rtArea[i].y);
	}
	ret = 0;

	return ret;
}

/**
 * bsd_get_new_data - get new data
 *
 * This function use to get new data
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
int bsd_get_new_data()
{
	int ret = -1;

	ret = 0;
	return ret;
}

int bsd_create_calibration(void)
{
	int ret = -1;

	ret = 0;

	return ret;
}

int bsd_delete_calibration(void)
{
	int ret = -1;

	ret = 0;

	return ret;
}

void bsd_get_screen_info(bsd_screen_info_t *cdr_screen)
{
	int ret;
	rt_device_t disp_dev;
	disp_io_ctrl_t dic;
	struct rt_device_graphic_info info;

	disp_dev = rt_device_find(DISP_DEVICE_NAME);
	/* open the device */
	if (disp_dev != NULL) {
		ret = rt_device_open(disp_dev, 0);
		if (ret != RT_EOK) {
			MOD_LOG_E("rt_device_open error\n");
			return;
		}
	} else {
		MOD_LOG_E("\n");
		return;
	}
	rt_memset(&dic, 0x00, sizeof(disp_io_ctrl_t));
	dic.dc_index = 0;
	dic.args = &info;
	rt_device_control(disp_dev, DISP_CMD_GET_INFO, &dic);
	MOD_LOG_W("width[%d] height[%d]", info.width, info.height);
#ifdef SCREEN_ROT_90
	cdr_screen->width = info.height;
	cdr_screen->height = info.width;
#else
	cdr_screen->width = info.width;
	cdr_screen->height = info.height;
#endif
	/* close audio device */
	rt_device_close(disp_dev);
}


