/*
 * car_bsd.h - the Auxiliary Driving System of car head file
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

#ifndef __CAR_BSD_H__
#define __CAR_BSD_H__
#include "OMX_IVCommon.h"
#include "lb_recorder.h"
#include "vrender_component.h"
#include "car_bsd_set.h"
#include "lb_types.h"
#include "mod_bsd.h"

#ifdef __cplusplus
extern "C"
{
#endif
#define ASSERT_BSD_PARA(EX) RT_ASSERT(EX)

#ifndef CAPTURE_SOURECE
extern const char vedio_y[];
extern const char vedio_uv[];
#endif
extern bsd_screen_info_t bsd_screen;

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
typedef enum tag_bsd_state {
	bsd_status_close = 0,
	bsd_status_open = 1,
	bsd_status_suspend = 2,
	bsd_status_resume = 3,
	bsd_status_max
} bsd_state_t;

typedef struct tag_mm_frame {
	void *multi_yP;
	void *multi_uvP;
	void *multi_frame;
} mm_frame_t;
typedef struct bsd_result_cb_info {
	int type;
	int (*bsd_result)(HDFrameGetBsd *);
} bsd_result_cb_t;

bsd_result_cb_t *bsd_result_cb;

int bsd_create(void *recorder_hd);
int bsd_delete(void *recorder_hd);
int car_bsd_set_status(int bsd_status);
int car_bsd_get_status(void);

#ifdef __cplusplus
}
#endif

#endif
