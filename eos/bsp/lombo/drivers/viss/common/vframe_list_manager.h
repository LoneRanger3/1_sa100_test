/*
 * vframe_list_manager.h - video frame manager head file
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

#ifndef __VFRAME_LIST_MANAGER_H__
#define __VFRAME_LIST_MANAGER_H__
#include "spinlock.h"

enum vlist_type {
	VLIST_FREE,
	VLIST_FULL,
};

struct vlist {
	rt_int32_t magic;
	enum vlist_type type;
	rt_int32_t buffer_cnt;
	struct list_head buf_q;
	/* spinlock_t  vflock; */
};

extern rt_int32_t vlist_add(struct vlist *vq, void *buffer);
extern struct vlist *vlist_create(enum vlist_type type, rt_int32_t magic);
extern void vlist_destory(struct vlist *vq);
extern rt_bool_t vlist_empty(struct vlist *vq);
extern rt_int32_t vlist_frmid_to_index(struct vlist *head, rt_int32_t frmid);
extern rt_bool_t vlist_frmid_valid(struct vlist *head, rt_int32_t frmid);
extern rt_int32_t vlist_index_to_frmid(struct vlist *head, rt_int32_t index);
extern void vlist_init(struct vlist *vq);
extern void *vlist_pop(struct vlist *vq);

#endif

