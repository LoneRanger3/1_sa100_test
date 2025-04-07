/*
 * adas_set.c - setting of adas
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
#include "car_adas_set.h"
#include "car_adas_lib.h"
#include "car_adas.h"
#include "frame_queue.h"
#include "eos.h"
#include <debug.h>

static adas_para_t adas_para; /* car adas parameter struct */
static adas_jobj_t adas_jobj; /* car adas json object struct */

static void adas_para_array_init(cJSON *para)
{
	lb_int32 i = 0;

	for (i = 0; i < cJSON_GetArraySize(para); i++) {
		cJSON *temp;
		temp = cJSON_GetArrayItem(para, i);
		if (temp && temp->type == cJSON_Object) {
			if (cJSON_GetObjectItem(temp, "adas_enable")) {
				adas_jobj.adas_enable =
					cJSON_GetObjectItem(temp, "adas_enable");
				adas_para.adas_enable =
					adas_jobj.adas_enable->valueint;
				MOD_LOG_D("adas_enable %d\n",
					adas_para.adas_enable);
			} else if (cJSON_GetObjectItem(temp, "lanewarndist")) {
				adas_jobj.lanewarndist =
					cJSON_GetObjectItem(temp, "lanewarndist");
				adas_para.lanewarndist =
					adas_jobj.lanewarndist->valueint;
				MOD_LOG_D("lanewarndist %d\n",
					adas_para.lanewarndist);
			} else if (cJSON_GetObjectItem(temp, "pano_type")) {
				adas_jobj.car_type = cJSON_GetObjectItem(temp,
					"pano_type");
				adas_para.car_type = adas_jobj.car_type->valueint;
				MOD_LOG_D("car_type %d\n", adas_para.car_type);
			} else if (!adas_para.car_type && cJSON_GetObjectItem(temp,
			"car_width")) {
				adas_jobj.car_width = cJSON_GetObjectItem(temp,
					"car_width");
				adas_para.car_width = adas_jobj.car_width->valuedouble
					* 100;
				MOD_LOG_D("car_width %d\n", adas_para.car_width);
			} else if (adas_para.car_type == 1 && cJSON_GetObjectItem(temp,
			"suv_width")) {
				adas_jobj.car_width = cJSON_GetObjectItem(temp,
					"suv_width");
				adas_para.car_width = adas_jobj.car_width->valuedouble
					* 100;
				MOD_LOG_D("car_width %d\n", adas_para.car_width);
			}  else if (adas_para.car_type == 2 && cJSON_GetObjectItem(temp,
			"others_width")) {
				adas_jobj.car_width = cJSON_GetObjectItem(temp,
					"others_width");
				adas_para.car_width = adas_jobj.car_width->valuedouble
					* 100;
				MOD_LOG_D("car_width %d\n", adas_para.car_width);
			} else if (cJSON_GetObjectItem(temp, "roipara_enable")) {
				adas_jobj.roipara_enable =
					cJSON_GetObjectItem(temp, "roipara_enable");
				adas_para.roipara_enable =
					adas_jobj.roipara_enable->valueint;
				MOD_LOG_D("roipara_enable %d\n",
					adas_para.roipara_enable);
			} else if (cJSON_GetObjectItem(temp, "roipara_uprow")) {
				adas_jobj.roipara_uprow =
					cJSON_GetObjectItem(temp, "roipara_uprow");
				adas_para.roipara_uprow =
					adas_jobj.roipara_uprow->valueint-60;
				MOD_LOG_D("roipara_uprow %d\n",
					adas_para.roipara_uprow);
			} else if (cJSON_GetObjectItem(temp, "roipara_dnrow")) {
				adas_jobj.roipara_dnrow =
					cJSON_GetObjectItem(temp, "roipara_dnrow");
				adas_para.roipara_dnrow =
					adas_jobj.roipara_dnrow->valueint-60;
				MOD_LOG_D("roipara_dnrow %d\n",
					adas_para.roipara_dnrow);
			} else if (cJSON_GetObjectItem(temp,
					"camoutpara_cam2middle")) {
				adas_jobj.camoutpara_cam2middle =
					cJSON_GetObjectItem(temp,
						"camoutpara_cam2middle");
				adas_para.camoutpara_cam2middle =
					adas_jobj.camoutpara_cam2middle->valueint;
				MOD_LOG_D("camoutpara_cam2middle %d\n",
					adas_para.camoutpara_cam2middle);
			} else if (cJSON_GetObjectItem(temp, "fcwsensity")) {
				adas_jobj.fcwsensity =
					cJSON_GetObjectItem(temp, "fcwsensity");
				adas_para.fcwsensity =
					adas_jobj.fcwsensity->valueint;
				MOD_LOG_D("fcwsensity %d\n",
					adas_para.fcwsensity);
			} else if (cJSON_GetObjectItem(temp, "ldwsensity")) {
				adas_jobj.ldwsensity =
					cJSON_GetObjectItem(temp, "ldwsensity");
				adas_para.ldwsensity =
					adas_jobj.ldwsensity->valueint;
				MOD_LOG_D("ldwsensity %d\n",
					adas_para.ldwsensity);
			} else if (cJSON_GetObjectItem(temp, "moduleenable_ingps")) {
				adas_jobj.gps_enable =
					cJSON_GetObjectItem(temp, "moduleenable_ingps");
				adas_para.gps_enable =
					adas_jobj.gps_enable->valueint;
				MOD_LOG_D("gps_enable %d\n", adas_para.gps_enable);
			} else if (cJSON_GetObjectItem(temp, "caminpara_fps")) {
				adas_jobj.caminpara_fps =
					cJSON_GetObjectItem(temp, "caminpara_fps");
				adas_para.caminpara_fps =
					adas_jobj.caminpara_fps->valueint;
				MOD_LOG_D("caminpara_fps %d\n", adas_para.caminpara_fps);
			} else if (cJSON_GetObjectItem(temp, "caminpara_imgsize_w")) {
				adas_jobj.caminpara_imgsize_w =
					cJSON_GetObjectItem(temp, "caminpara_imgsize_w");
				adas_para.caminpara_imgsize_w =
					adas_jobj.caminpara_imgsize_w->valueint;
				MOD_LOG_D("caminpara_imgsize_w %d\n",
					adas_para.caminpara_imgsize_w);
			} else if (cJSON_GetObjectItem(temp, "caminpara_imgsize_h")) {
				adas_jobj.caminpara_imgsize_h =
					cJSON_GetObjectItem(temp, "caminpara_imgsize_h");
				adas_para.caminpara_imgsize_h =
					adas_jobj.caminpara_imgsize_h->valueint;
				MOD_LOG_D("caminpara_imgsize_h %d\n",
					adas_para.caminpara_imgsize_h);
			} else if (cJSON_GetObjectItem(temp, "caminpara_angh")) {
				adas_jobj.caminpara_angh =
					cJSON_GetObjectItem(temp, "caminpara_angh");
				adas_para.caminpara_angh =
					adas_jobj.caminpara_angh->valueint;
				MOD_LOG_D("adas_para.caminpara_angh %d\n",
					adas_para.caminpara_angh);
			} else if (cJSON_GetObjectItem(temp, "caminpara_angv")) {
				adas_jobj.caminpara_angv =
					cJSON_GetObjectItem(temp, "caminpara_angv");
				adas_para.caminpara_angv =
					adas_jobj.caminpara_angv->valueint;
				MOD_LOG_D("adas_para.caminpara_angv %d\n",
					adas_para.caminpara_angv);
			}
		} else if (temp && temp->type == cJSON_Array)
			adas_para_array_init(temp);
	}
}

/**
 * adas_para_init - get config parameter from file
 *
 * This function use to init config parameter from config file
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
int adas_para_init(void)
{
	int ret = 0;

#ifdef ARCH_LOMBO_N7_CDR_MMC
	adas_jobj.para_root = lb_open_cfg_file("/mnt/sdcard/etc/cdr_config1.cfg");
#else
	adas_jobj.para_root = lb_open_cfg_file("/mnt/data/cdr_config.cfg");
#endif
	if (adas_jobj.para_root == NULL) {
		adas_jobj.para_root =
			lb_open_cfg_file(ROOTFS_MOUNT_PATH"/etc/cdr_config.cfg");
		if (adas_jobj.para_root == NULL) {
			MOD_LOG_E("err adas_jobj.para_root is NULL");
			return -1;
		}
	}
	adas_jobj.car_adas =
		cJSON_GetObjectItem(adas_jobj.para_root, "car_adas");
	if (adas_jobj.car_adas == NULL)
		return -1;

	if (adas_jobj.car_adas && adas_jobj.car_adas->type == cJSON_Array)
		adas_para_array_init(adas_jobj.car_adas);

	adas_jobj.car_pano = cJSON_GetObjectItem(adas_jobj.para_root, "car_pano");
	if (adas_jobj.car_pano == NULL)
		return -1;

	if (adas_jobj.car_pano && adas_jobj.car_pano->type == cJSON_Array)
		adas_para_array_init(adas_jobj.car_pano);

	return ret;
}

/**
 * adas_para_exit - exit config file
 *
 * This function use to exit config file
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
int adas_para_exit(void)
{
	int ret = -1;

	MOD_LOG_D("adas_jobj.para_root %p\n", adas_jobj.para_root);
	if (adas_jobj.para_root)
		ret = lb_exit_cfg_file(adas_jobj.para_root);
	else {
		MOD_LOG_E("err adas_jobj.para_root is NULL");
		return -1;
	}

	return ret;
}

/**
 * adas_get_enable - get adas enable status
 *
 * This function use to get adas status
 *
 * Return car recorder status,START or STOP
 */
int adas_get_enable()
{
	return adas_para.adas_enable;
}

/**
 * adas_get_gps_enable - get gps status
 *
 * This function use to get gps status
 *
 * Return car gps enable status,ENABLE or DISABLE
 */
int adas_get_gps_enable()
{
	return adas_para.gps_enable;
}

/**
 * adas_get_time_stamp - get time stamp
 *
 * This function use to get time stamp
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
int adas_get_time_stamp(HDFrameSetData *set_data)
{
	int ret = -1;

	set_data->timeStamp = 0;
	ret = 0;

	return ret;
}

/**
 * adas_get_day_or_night - get day or night
 *
 * This function use to get day or night
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
int adas_get_day_or_night(HDFrameSetData *set_data)
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
* adas_set_cpugear - set cpu load level
*
* This function use to set cpu load level
*
* Returns -1 if called when get error ; otherwise, return 0
*/
int adas_set_cpugear(HDFrameSetData *set_data)
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
 * adas_get_vedio_image_info - get vedio image info
 *
 * This function use to set cpu load level
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
int adas_get_vedio_image_info(HDFrameSetData *set_data)
{
	int ret = -1;
	if (adas_para.caminpara_imgsize_w)
		set_data->imgYuv[MAJOR_CAM].size.width = adas_para.caminpara_imgsize_w;
	else
		set_data->imgYuv[MAJOR_CAM].size.width = FRONT_PREVIEW_WIDTH;
	if (adas_para.caminpara_imgsize_h)
		set_data->imgYuv[MAJOR_CAM].size.height = adas_para.caminpara_imgsize_h;
	else
		set_data->imgYuv[MAJOR_CAM].size.height = FRONT_PREVIEW_HEIGHT;

	ret = 0;

	return ret;
}

/**
 * adas_get_gps_info - get gps info
 *
 * This function use to get gps info
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
int adas_get_gps_info(HDFrameSetData *set_data)
{
	int ret = -1;

	if (!adas_get_gps_enable()) {
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
 * adas_get_gsensor_info - get gsensor info
 *
 * This function use to get gsensor info
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
int adas_get_gsensor_info(HDFrameSetData *set_data)
{
	int ret = -1;

	set_data->gsensor.acct = 0;
	set_data->gsensor.speed = 0;
	set_data->gsensor.stop = 0;

	ret = 0;

	return ret;
}

/**
 * adas_get_obd_para - get obd para
 *
 * This function use to get gsensor para
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
int adas_get_obd_para(HDFrameSetData *set_data)
{
	int ret = -1;

	memset(&(set_data->obd), 0x00, sizeof(HDObd));

	ret = 0;

	return ret;
}

/**
* adas_set_warn_sensity - set warn sensity
*
* This function use to set warn sensity
*
* Returns -1 if called when get error ; otherwise, return 0
*/
int adas_set_warn_sensity(HDFrameSetData *set_data)
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
	memset(&(set_data->warnSensity), 0x00, sizeof(HDWarnSensity));
	set_data->warnSensity.fcwSensity = adas_para.fcwsensity;
	set_data->warnSensity.ldwSensity = adas_para.ldwsensity;

	ret = 0;

	return ret;
}

/**
 * adas_get_run_ped - get run ped
 *
 * This function use to get run ped
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
int adas_get_run_ped(HDFrameSetData *set_data)
{
	int ret = -1;

	set_data->runPed = 0;

	ret = 0;

	return ret;
}

/**
 * adas_module_enable - enbale some modules
 *
 * This function use to enbale some modules
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
int adas_module_enable(HDIniSetData *set_data)
{
	int ret = -1;

	set_data->moduleEnable.inGps = adas_get_gps_enable();
	set_data->moduleEnable.inGsensor = 0;
	set_data->moduleEnable.inObd = 0;
	set_data->moduleEnable.inCamOutPara = 1;

	ret = 0;

	return ret;
}

/**
 * adas_set_carpara - set car para
 *
 * This function use to set car para
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
int adas_set_carpara(HDIniSetData *set_data)
{
	int ret = 0;

#ifdef CAPTURE_SOURECE
	set_data->carPara.enable = 1;
	if (set_data->carPara.enable) {
		set_data->carPara.width = adas_para.car_width;
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
 * adas_set_roi_para - set roi para
 *
 * This function use to set roi para
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
int adas_set_roi_para(HDIniSetData *set_data)
{
	int ret = -1;

#ifdef CAPTURE_SOURECE
	ASSERT_ADAS_PARA(((adas_para.roipara_enable == 1) ||
			(adas_para.roipara_enable == 0)));
	ASSERT_ADAS_PARA(adas_para.roipara_uprow >= 0);
	ASSERT_ADAS_PARA(adas_para.roipara_dnrow >= 0);

	set_data->roiPara.enable = adas_para.roipara_enable;
	if (set_data->roiPara.enable) {
		set_data->roiPara.upRow = adas_para.roipara_uprow *
			FRONT_PREVIEW_HEIGHT / ADAS_CALIB_PREW;/* sky_line_y; */
		set_data->roiPara.dnRow = adas_para.roipara_dnrow *
			FRONT_PREVIEW_HEIGHT / ADAS_CALIB_PREW;/* hood_line_y */
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
 * adas_set_cam_out_para - set cam out para
 *
 * This function use to set cam out para
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
int adas_set_cam_out_para(HDIniSetData *set_data)
{
	int ret = -1;
	int cali_width = ADAS_CALIB_PREH;

	set_data->cameraPara[MAJOR_CAM].camOutPara.enable = 1;
	set_data->cameraPara[MAJOR_CAM].camOutPara.cam2ground = 130;
#ifdef CAPTURE_SOURECE
	set_data->cameraPara[MAJOR_CAM].camOutPara.cam2middle =
		(adas_para.camoutpara_cam2middle - cali_width / 2) *
		FRONT_PREVIEW_WIDTH / cali_width;
	if (set_data->cameraPara[MAJOR_CAM].camOutPara.cam2middle > 100)
		set_data->cameraPara[MAJOR_CAM].camOutPara.cam2middle = 100;
	else if (set_data->cameraPara[MAJOR_CAM].camOutPara.cam2middle < -100)
		set_data->cameraPara[MAJOR_CAM].camOutPara.cam2middle = -100;
#else
	set_data->cameraPara[MAJOR_CAM].camOutPara.cam2middle = 0;
#endif
	set_data->cameraPara[MAJOR_CAM].camOutPara.cam2head = 150;
	set_data->cameraPara[MAJOR_CAM].camOutPara.laneCrossPoint.x =
		adas_para.camoutpara_cam2middle *
		FRONT_PREVIEW_WIDTH / cali_width;
	set_data->cameraPara[MAJOR_CAM].camOutPara.laneCrossPoint.y =
		adas_para.roipara_uprow * FRONT_PREVIEW_HEIGHT / ADAS_CALIB_PREW;

	ret = 0;

	return ret;
}

/**
 * adas_set_cam_in_para - set cam in para
 *
 * This function use to set cam in para
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
int adas_set_cam_in_para(HDIniSetData *set_data)
{
	int ret = -1;

	if (adas_para.caminpara_fps)
		set_data->cameraPara[MAJOR_CAM].camInPara.fps = adas_para.caminpara_fps;
	else
		set_data->cameraPara[MAJOR_CAM].camInPara.fps = 30;
	if (adas_para.caminpara_imgsize_w)
		set_data->cameraPara[MAJOR_CAM].camInPara.imgSize.width =
		adas_para.caminpara_imgsize_w;
	else
		set_data->cameraPara[MAJOR_CAM].camInPara.imgSize.width =
		FRONT_PREVIEW_WIDTH;
	if (adas_para.caminpara_imgsize_h)
		set_data->cameraPara[MAJOR_CAM].camInPara.imgSize.height =
		adas_para.caminpara_imgsize_h;
	else
		set_data->cameraPara[MAJOR_CAM].camInPara.imgSize.height =
		FRONT_PREVIEW_HEIGHT;
	if (adas_para.caminpara_angh)
		set_data->cameraPara[MAJOR_CAM].camInPara.viewAng.angH =
		adas_para.caminpara_angh;
	else
		set_data->cameraPara[MAJOR_CAM].camInPara.viewAng.angH = 130;
	if (adas_para.caminpara_angv)
		set_data->cameraPara[MAJOR_CAM].camInPara.viewAng.angV =
		adas_para.caminpara_angv;
	else
		set_data->cameraPara[MAJOR_CAM].camInPara.viewAng.angV = 70;

	ret = 0;

	return ret;
}

/**
 * adas_set_ped_para - set ped para
 *
 * This function use to set ped para
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
int adas_set_ped_para(HDIniSetData *set_data)
{
	int ret = -1;

	set_data->laneWarnDist = adas_para.lanewarndist;

	ret = 0;

	return ret;
}

/**
 * adas_get_new_data - get new data
 *
 * This function use to get new data
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
int adas_get_new_data()
{
	int ret = -1;

	ret = 0;

	return ret;
}

int adas_create_calibration(void)
{
	int ret = -1;

	ret = 0;

	return ret;
}

int adas_delete_calibration(void)
{
	int ret = -1;

	ret = 0;

	return ret;
}

void adas_get_screen_info(adas_screen_info_t *cdr_screen)
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


