/*
 * recorder_watermark.c - Standard functionality for take picture.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#ifndef __WATERMARK_H__
#define __WATERMARK_H__

#define MAX_WATERMARK_NUM 4

typedef struct {
	numeral_picture_t numeral_picture;
	unsigned int colorspace;
	unsigned int start_x_pos;
	unsigned int start_y_pos;
	unsigned int end_x_pos;
	unsigned int end_y_pos;
	unsigned int blending_area_index;
} numeral_output_t;

typedef struct {
	pthread_mutex_t lock;
	numeral_input_t bnp; /* input blending pictures struct*/
	numeral_output_t watermark[MAX_WATERMARK_NUM];
} watermark_priv_t;

#endif
