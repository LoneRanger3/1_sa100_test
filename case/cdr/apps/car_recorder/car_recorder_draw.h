/*
 * car_recorder_draw.c - car recorder draw graphics.
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

#ifndef _CAR_RECORDER_DRAW_H_
#define _CAR_RECORDER_DRAW_H_

#include "lb_gal_common.h"
#include <pthread.h>
#include "car_recorder_draw_util.h"
#include "car_recorder.h"

#define ADAS_BKL_LINE_NUM_MAX		1

#define ADAS_BKL_NUM_MAX		9

#define BSD_BKL_NUM_MAX			2
#define BSD_BKL_LEFT_WAR_IDX		9
#define BSD_BKL_RIGHT_WAR_IDX		10

typedef enum draw_cmd {
	BKL_SHOW,
	BKL_HIDE,
	BKL_DRAW_RECT,
	BKL_DRAW_LINE,
	BKL_DRAW_BSD_LINE,
	BKL_RELESE
} car_draw_cmd_e;

typedef struct tag_car_draw_info {
	uint8_t		b_draw;
	uint8_t		line_dir;
	uint8_t		en_fill;
	uint8_t		idx;
	lb_rect_t	buff_rect;
	line_point	line_points[4];
	unsigned int	meters_color;
	unsigned int	color;
	int32_t		meters;
	unsigned int	border_width;
	unsigned int	border_color;
	unsigned int	fill_color;
} car_draw_info_t;


int cdr_draw_init(pthread_t *p_tid);
int cdr_draw_exit(pthread_t tid);
int cdr_draw_msg_send(car_draw_info_t adas_draw_info);
#endif

