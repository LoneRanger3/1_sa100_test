/*
 * pano_set.h - pano setting code for LomboTech
 * pano setting interface and macro define
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

#ifndef __PANO_SET_H__
#define __PANO_SET_H__

#include "app_manage.h"
#include <debug.h>

#define MOTOR_WIDTH_MAX 3.00
#define MOTOR_WIDTH_MIN 1.50
#define MOTOR_WIDTH_STEP 0.01
#define MOTOR_LENGTH_MAX 6.00
#define MOTOR_LENGTH_MIN 3.00
#define MOTOR_LENGTH_STEP 0.01
#define MOTOR_DIST2R_MAX 300
#define MOTOR_DIST2R_MIN 0
#define MOTOR_DIST2R_STEP 1

typedef struct tag_pano_set {
	float motor_w;
	float motor_l;
	int motor_d2r;
} pano_set_t;

typedef enum tag_motor_type {
	MOTOR_TYPE_CAR,
	MOTOR_TYPE_SUV,
	MOTOR_TYPE_OTHERS,
	MOTOR_TYPE_MAX
} motor_type_e;

lb_int32 pano_set_init_funcs(void);
lb_int32 pano_set_uninit_funcs(void);
lb_int32 pano_set_resp_funcs(void);
lb_int32 pano_set_unresp_funcs(void);
lb_int32 motor_get_set_para(pano_set_t *set);

#endif /* __PANO_SET_H__ */
