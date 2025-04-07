/*
 * frm_queue.h - frame manager head file
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

#ifndef __FRM_MANAGER_H__
#define __FRM_MANAGER_H__

#include "OMX_Core.h"
#include "base_component.h"

/* frame mamager data sturct */
typedef struct fq_data {
	struct list_head list;
	void *data;
} fq_data_t;

typedef struct fq_manage {
	OMX_U32 magic;
	OMX_U32 number; /* the number of data stored in the data_q list */
	struct list_head data_l; /* data list */
	pthread_mutex_t lock;
} fq_manage_t;

extern fq_manage_t *fq_open();
extern void fq_colse(fq_manage_t *fq);
extern OMX_S32 fq_add(fq_manage_t *fq, fq_data_t *data);
extern fq_data_t *fq_pop(fq_manage_t *fq);
extern OMX_BOOL fq_empty(fq_manage_t *fq);

#endif

