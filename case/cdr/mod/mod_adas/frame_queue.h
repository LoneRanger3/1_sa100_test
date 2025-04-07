/*
 * frame_queue.h - video frame manager head file
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

#ifndef __FRAME_QUEUE_H__
#define __FRAME_QUEUE_H__

enum queue_type {
	QUEUE_FREE,
	QUEUE_FULL,
};

typedef struct tag_queue_frame {
	enum queue_type type;
	rt_int32_t size;
	rt_int32_t *frmid_array;
	rt_int32_t wt;
	rt_int32_t rd;
} queue_frame_t;

extern queue_frame_t *queue_create(rt_int32_t size, enum queue_type type);
extern void queue_destory(queue_frame_t *head);
extern rt_int32_t queue_insert(queue_frame_t *head, rt_int32_t frmid, char *s, int d);
extern rt_int32_t queue_delete(queue_frame_t *head, char *s, int d);

#endif
