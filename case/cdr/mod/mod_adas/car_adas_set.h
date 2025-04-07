/*
 * car_adas_set.h - file sub_system code for LomboTech
 * file sub_system interface and macro define
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

#ifndef	__CAR_ADAS_SET_H__
#define	__CAR_ADAS_SET_H__

#include "cJSON.h"
#include "case_config.h"
#include "lb_cfg_file.h"
#include "libadas.h"
#include "system/system.h"
#include "input/gps/gps_dev.h"

#ifdef __cplusplus
extern "C"
{
#endif
typedef struct screen_info_ {
	int width;
	int height;
} adas_screen_info_t;

typedef struct _adas_jobj_ {
	cJSON *para_root;
	cJSON *car_adas;
	cJSON *adas_enable;
	cJSON *car_type;
	cJSON *car_width;
	cJSON *roipara_enable;
	cJSON *roipara_uprow;
	cJSON *roipara_dnrow;
	cJSON *camoutpara_cam2middle;
	cJSON *lanewarndist;
	cJSON *fcwsensity;
	cJSON *ldwsensity;
	cJSON *car_pano;
	cJSON *pano_enable;
	cJSON *gps_enable;
	cJSON *caminpara_fps;
	cJSON *caminpara_imgsize_w;
	cJSON *caminpara_imgsize_h;
	cJSON *caminpara_angh;
	cJSON *caminpara_angv;
} adas_jobj_t;

typedef struct _adas_para_ {
	int adas_enable;
	int car_type;
	int car_width;
	int roipara_enable;
	int roipara_uprow;
	int roipara_dnrow;
	int camoutpara_cam2middle;
	int lanewarndist;
	int fcwsensity;
	int ldwsensity;
	int pano_enable;
	int gps_enable;
	int caminpara_fps;
	int caminpara_imgsize_w;
	int caminpara_imgsize_h;
	int caminpara_angh;
	int caminpara_angv;
} adas_para_t;
int adas_para_init(void);

int adas_para_exit(void);

int adas_get_enable();

int adas_get_gps_enable();

int adas_get_time_stamp(HDFrameSetData *set_data);

int adas_get_day_or_night(HDFrameSetData *set_data);

int adas_set_cpugear(HDFrameSetData *set_data);

int adas_get_vedio_image_info(HDFrameSetData *set_data);

int adas_get_gps_info(HDFrameSetData *set_data);

int adas_get_gsensor_info(HDFrameSetData *set_data);

int adas_get_obd_para(HDFrameSetData *set_data);

int adas_set_warn_sensity(HDFrameSetData *set_data);

int adas_get_run_ped(HDFrameSetData *set_data);

int adas_module_enable(HDIniSetData *set_data);

int adas_set_carpara(HDIniSetData *set_data);

int adas_set_roi_para(HDIniSetData *set_data);

int adas_set_cam_out_para(HDIniSetData *set_data);

int adas_set_cam_in_para(HDIniSetData *set_data);

int adas_set_ped_para(HDIniSetData *set_data);

int adas_get_new_data();

int adas_create_calibration(void);

int adas_delete_calibration(void);

void adas_get_screen_info(adas_screen_info_t *cdr_screen);

#ifdef __cplusplus
}
#endif

#endif
