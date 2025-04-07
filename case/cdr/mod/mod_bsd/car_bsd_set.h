/*
 * car_bsd_set.h - file sub_system code for LomboTech
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
#include "libbsd.h"
#include "system/system.h"
#include "input/gps/gps_dev.h"

#ifdef __cplusplus
extern "C"
{
#endif
typedef struct screen_info_ {
	int width;
	int height;
} bsd_screen_info_t;

typedef struct tag_bsd_warn_area {
	int x;
	int y;
} bsd_warn_area_t;

typedef struct _bsd_jobj_ {
	cJSON *para_root;
	cJSON *car_bsd;
	cJSON *bsd_enable;
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
	cJSON *alarm_sensity;
	cJSON *warnarea_enable;
	cJSON *warnarea_lt;
	cJSON *warnarea_rt;
} bsd_jobj_t;

typedef struct _bsd_para_ {
	int bsd_enable;
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
	int alarm_sensity;
	int warnarea_enable;
	bsd_warn_area_t warnarea_lt[4];
	bsd_warn_area_t warnarea_rt[4];
} bsd_para_t;

int bsd_para_init(void);

int bsd_para_exit(void);

int bsd_get_bsd_enable();

int bsd_get_gps_enable();

int bsd_get_time_stamp(HDFrameSetBsd *set_data);

int bsd_get_day_or_night(HDFrameSetBsd *set_data);

int bsd_set_cpuGear(HDFrameSetBsd *set_data);

int bsd_get_vedio_image_info(HDFrameSetBsd *set_data);

int bsd_get_gps_info(HDFrameSetBsd *set_data);

int bsd_get_gsensor_info(HDFrameSetBsd *set_data);

int bsd_get_obd_para(HDFrameSetBsd *set_data);

int bsd_set_warn_sensity(HDFrameSetBsd *set_data);

int bsd_get_run_ped(HDFrameSetBsd *set_data);

int bsd_module_enable(HDIniSetBsd *set_data);

int bsd_set_carpara(HDIniSetBsd *set_data);

int bsd_set_roi_para(HDIniSetBsd *set_data);

int bsd_set_cam_out_para(HDIniSetBsd *set_data);

int bsd_set_cam_in_para(HDIniSetBsd *set_data);

int bsd_set_ped_para(HDIniSetBsd *set_data);

int bsd_set_alarm_sensity(HDIniSetBsd *set_data);

int bsd_set_warn_area(HDIniSetBsd *set_data);

int bsd_get_new_data();

int bsd_create_calibration(void);

int bsd_delete_calibration(void);

void bsd_get_screen_info(bsd_screen_info_t *cdr_screen);

#ifdef __cplusplus
}
#endif

#endif
