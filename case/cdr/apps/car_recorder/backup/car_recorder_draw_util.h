/*
 * car_recorder_draw_util.c - draw rectangle and line functions.
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

#ifndef _CAR_RECORDER_DRAW_UTIL_H_
#define _CAR_RECORDER_DRAW_UTIL_H_
#include "car_recorder.h"
#if (FRONT_PREVIEW_WIDTH == 1920)
#define ADAS_BKL_RECT_WIDTH_MAX		275
#define ADAS_BKL_RECT_HEIGHT_MAX	275
#define ADAS_BKL_RECT_SIZE_MAX		(275 * 275 * 4)

#define ADAS_BKL_LINE_WIDTH_MAX		275
#define ADAS_BKL_LINE_HEIGHT_MAX	1920
#define ADAS_BKL_LINE_SIZE_MAX		(275 * 1920 * 4)
#else
#define ADAS_BKL_RECT_WIDTH_MAX		200
#define ADAS_BKL_RECT_HEIGHT_MAX	200
#define ADAS_BKL_RECT_SIZE_MAX		(200 * 200 * 4)

#define ADAS_BKL_LINE_WIDTH_MAX		200
#define ADAS_BKL_LINE_HEIGHT_MAX	800
#define ADAS_BKL_LINE_SIZE_MAX		(200 * 800 * 4)
#endif

#if (REAR_PREVIEW_WIDTH == 1920)
#define BSD_BKL_LINE_WIDTH_MAX		440
#define BSD_BKL_LINE_HEIGHT_MAX		187
#define BSD_BKL_LINE_START_POS		87

#define BSD_BKL_LINE_SIZE_MAX		(187 * 440 * 4)
#else
#define BSD_BKL_LINE_WIDTH_MAX		320
#define BSD_BKL_LINE_HEIGHT_MAX		136
#define BSD_BKL_LINE_START_POS		60

#define BSD_BKL_LINE_SIZE_MAX		(136 * 320 * 4)
#endif

typedef struct tag_line_draw_point_t {
	int	x;
	int	y;
} line_point;

int car_recorder_adas_draw_rect(unsigned char *p_buff, int len, char b_color);
int car_recorder_adas_draw_line(unsigned char *buff,
				line_point start,
				line_point end,
				unsigned char dir);
int car_recorder_adas_draw_str(unsigned char *p_buff,
					line_point start,
					line_point end,
					char *p_str);
void car_recorder_draw_line(uint8_t *buff, line_point point1,
				line_point point2 , uint32_t color);
int car_recorder_draw_polygon(unsigned char *buff,
			line_point *points, uint32_t color);

#endif

