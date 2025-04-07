/*
 * thumb_image.h - thumb image head file
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

#ifndef __THUMB_IMAGE_H__
#define __THUMB_IMAGE_H__

#include "lb_types.h"
#include "lb_common.h"
#include <pthread.h>
#include <semaphore.h>
#include "fileexp_cfg.h"

extern fxp_config_t fileexp_config;

#define THUMBNAIL_PRIO 28
#define THUMBNAIL_SIZE 4096

typedef struct tag_thumb_img {
	lb_int32 index;
	void *desert;
	pthread_t id;
	lb_int32 exit;
	sem_t in_sem;
	sem_t out_sem;
	char **path;
	char **thumb;
	char **data;
	char **image;
	char **db_data;
	char **db_image;
	char *type;
} thumb_img_t;

typedef struct {
	lb_uint32 cf           : 5;   /* Color format */
	lb_uint32 always_zero  : 3;   /* It the upper bits of the first byte */

	lb_uint32 reserved     : 2;  /* Reserved to be used later */

	lb_uint32 w : 11;             /* Width of the image map */
	lb_uint32 h : 11;             /* Height of the image map */
} thumb_img_header_t;

typedef struct {
	thumb_img_header_t header;
	lb_uint32 data_size;
	const lb_uint8 *data;
} thumb_img_dsc_t;

lb_int32 image_thumb_init(void *para);
lb_int32 image_thumb_exit(void *para);
lb_int32 image_thumb_start(void *para);
lb_int32 image_thumb_stop(void *para);
lb_int32 image_thumb_reg_resp(void);
lb_int32 image_thumb_unreg_resp(void);
lb_int32 image_thumb_set(void *desert);

#endif /* __THUMB_IMAGE_H__ */
