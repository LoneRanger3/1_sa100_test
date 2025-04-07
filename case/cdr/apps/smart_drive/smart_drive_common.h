/*
 * smart_drive_common.h - smart drive common code for LomboTech
 * smart drive interface common and macro define
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

#ifndef __SMART_DRIVE_COMMON_H_
#define __SMART_DRIVE_COMMON_H_

#include "cJSON.h"
#include "lb_cfg_file.h"
#include "lb_types.h"
#include "system/system.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef ARCH_LOMBO_N7_CDR_MMC
#define CUSTOMER_CONFIG_PATH ROOTFS_MOUNT_PATH"/etc/cdr_config1.cfg"
#define ORIGINAL_CONFIG_PATH ROOTFS_MOUNT_PATH"/etc/cdr_config.cfg"
#else
#define CUSTOMER_CONFIG_PATH ROOTFS_MOUNT_PATH"/mnt/data/cdr_config.cfg"
#define ORIGINAL_CONFIG_PATH ROOTFS_MOUNT_PATH"/etc/cdr_config.cfg"
#endif

typedef struct tag_bsd_warn_area {
	cJSON *lt_up_x;
	cJSON *lt_up_y;
	cJSON *rt_up_x;
	cJSON *rt_up_y;
	cJSON *rt_dn_x;
	cJSON *rt_dn_y;
	cJSON *lt_dn_x;
	cJSON *lt_dn_y;
} bsd_warn_area_t;

typedef struct tag_bsd_cali_area {
	cJSON *roipara_uprow;
	cJSON *roipara_dnrow;
	cJSON *camoutpara_cam2middle;
	cJSON *rswsensity;
	cJSON *rswlevel;
} bsd_cali_area_t;

typedef struct tag_car_obj {
	cJSON *para_root;

	cJSON *system;
	cJSON *rearmirr_enable;
	cJSON *car_adas;
	cJSON *adas_enable;
	cJSON *carpara_enable;
	cJSON *carpara_width;
	cJSON *roipara_enable;
	cJSON *roipara_uprow;
	cJSON *roipara_dnrow;
	cJSON *camoutpara_cam2middle;
	cJSON *lanewarndist;
	cJSON *fcwsensity;
	cJSON *ldwsensity;

	cJSON *car_pano;
	cJSON *pano_enable;
	cJSON *pano_type;
	cJSON *car_width;
	cJSON *car_length;
	cJSON *car_distance;
	cJSON *suv_width;
	cJSON *suv_length;
	cJSON *suv_distance;
	cJSON *others_width;
	cJSON *others_length;
	cJSON *others_distance;

	cJSON *car_bsd;
	cJSON *bsd_enable;
	cJSON *warnarea_enable;
	cJSON *warnarea_lt;
	cJSON *warnarea_rt;

	bsd_warn_area_t lt_area;
	bsd_warn_area_t rt_area;
	bsd_cali_area_t ci_area;
} car_obj_t;

int system_get_rearmirr_enable(void);

int car_save_adas_enable(int  value);

int car_get_adas_enable();

int car_save_pano_enable(int  value);

int car_get_pano_enable(void);

int car_save_bsd_enable(int  value);

int car_get_bsd_enable();

int adas_para_init(void);

int adas_para_save(void);

int adas_para_exit(void);

int adas_save_fcwsensity(int value);

int adas_get_fcwsensity();

int adas_save_ldwsensity(int value);

int adas_get_ldwsensity();

int adas_save_carpara(int  value);

int adas_get_carpara();

int adas_save_roi_para(int upRow, int dnRow, int midcolumn);

int adas_get_roi_para_uprow();

int adas_get_roi_para_dnrow();

int adas_get_roi_para_midcolumn();

int pano_get_type(void);

int pano_save_type(int type);

double pano_get_width(int motor_type);

int pano_save_width(int motor_type, double width);

double pano_get_length(int motor_type);

int pano_save_length(int motor_type, double length);

int pano_get_distance(int motor_type);

int pano_save_distance(int motor_type, int distance);

int bsd_save_roi_para(int upRow, int dnRow, int midcolumn);

int bsd_get_roi_para_uprow(void);

int bsd_get_roi_para_dnrow(void);

int bsd_get_roi_para_midcolumn(void);

int bsd_save_rswsensity(int value);

int bsd_get_rswsensity(void);

int bsd_save_rswlevel(int value);

int bsd_get_rswlevel(void);

int bsd_save_area_lt_up(int x, int y, int lt);

int bsd_save_area_rt_up(int x, int y, int lt);

int bsd_save_area_rt_dn(int x, int y, int lt);

int bsd_save_area_lt_dn(int x, int y, int lt);

int bsd_get_area_lt_up_x(int lt);

int bsd_get_area_lt_up_y(int lt);

int bsd_get_area_rt_up_x(int lt);

int bsd_get_area_rt_up_y(int lt);

int bsd_get_area_rt_dn_x(int lt);

int bsd_get_area_rt_dn_y(int lt);

int bsd_get_area_lt_dn_x(int lt);

int bsd_get_area_lt_dn_y(int lt);

#ifdef __cplusplus
}
#endif

#endif /* __SMART_DRIVE_COMMON_H_ */
