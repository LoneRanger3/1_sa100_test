/*
 * car_adas.h - the Auxiliary Driving System of car head file
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

#ifndef __CAR_ADAS_H__
#define __CAR_ADAS_H__
#include "OMX_IVCommon.h"
#include "lb_recorder.h"
#include "vrender_component.h"
#include "car_adas_set.h"
#include "lb_types.h"
#include "mod_adas.h"

#ifdef __cplusplus
extern "C"
{
#endif
#define ASSERT_ADAS_PARA(EX) RT_ASSERT(EX)

#ifndef CAPTURE_SOURECE
extern const char vedio_y[];
extern const char vedio_uv[];
#endif
extern adas_screen_info_t adas_screen;

#define QUEUE_NUM 1
#define CAPTURE_SOURECE
/* #define SAVE_FRM */

typedef enum {
	init_success = 0,
	init_error_initialed = 1,/* Init processing has been excuted */
	init_error_win_request = 2,/* window request failed */
	init_error_win_request_se = 3,/* window se request failed  */
	init_error_create_sem = 4,/* create semaphore failed */
	init_error_invalid_para = 5,/* invalid parameter */
	init_error_max
} init_state;
typedef enum tag_adas_state {
	adas_status_close = 0,
	adas_status_open = 1,
	adas_status_suspend = 2,
	adas_status_resume = 3,
	adas_status_max
} adas_state_t;

typedef struct tag_mm_frame {
	void *multi_yP;
	void *multi_uvP;
	void *multi_frame;
} mm_frame_t;
typedef struct adas_result_cb_info {
	int type;
	int (*adas_result)(HDFrameGetData *);
} adas_result_cb_t;

adas_result_cb_t *adas_result_cb;

int adas_create(void *recorder_hd);
int adas_delete(void *recorder_hd);
int car_adas_set_status(int adas_status);
int car_adas_get_status(void);

#ifdef __cplusplus
}
#endif

#endif
