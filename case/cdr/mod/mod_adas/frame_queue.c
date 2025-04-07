/*
 * frame_queue.c - video frame manager code for LomboTech
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

#include <stdio.h>
#include <debug.h>
#include <string.h>
#include <rtthread.h>
#include "frame_queue.h"

rt_bool_t queue_frmid_valid(queue_frame_t *head, rt_int32_t frmid)
{
	return RT_TRUE;
}

queue_frame_t *queue_create(rt_int32_t size, enum queue_type type)
{
	queue_frame_t *head = RT_NULL;

	head = rt_malloc(sizeof(queue_frame_t));
	RT_ASSERT(head != RT_NULL);
	rt_memset(head, 0, sizeof(queue_frame_t));

	head->frmid_array = rt_malloc(sizeof(rt_uint32_t) * size);
	RT_ASSERT(head->frmid_array != RT_NULL);
	rt_memset(head->frmid_array, 0, sizeof(rt_uint32_t) * size);

	head->size = size;
	head->type = type;
	head->wt = 0;
	head->rd = 0;

	return head;
}

void queue_destory(queue_frame_t *head)
{
	if (head->frmid_array)
		rt_free(head->frmid_array);

	if (head)
		rt_free(head);
}

rt_int32_t queue_insert(queue_frame_t *head,  rt_int32_t frmid, char *s, int d)
{
	RT_ASSERT(head != RT_NULL);

	if (head->rd == (head->wt + 1) % head->size)
		return -RT_ERROR;

	head->frmid_array[head->wt++] = frmid;
	head->wt %= head->size;

	return RT_EOK;
}

rt_int32_t queue_delete(queue_frame_t *head, char *s, int d)
{
	rt_int32_t frmid = 0;

	RT_ASSERT(head != RT_NULL);

	if (head->rd == head->wt)
		return -RT_ERROR;

	frmid = head->frmid_array[head->rd++];
	head->rd %= head->size;

	return frmid;
}
