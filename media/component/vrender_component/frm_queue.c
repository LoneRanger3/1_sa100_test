/*
 * frm_queue.c - frame manager code for LomboTech
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

#define DBG_SECTION_NAME	"FQ"
#define DBG_LEVEL		DBG_LOG

#include <pthread.h>
#include <list.h>
#include "frm_queue.h"

#define FQ_MAGIC (0x20190226)

fq_manage_t *fq_open(void)
{
	fq_manage_t *fq = NULL;

	fq = oscl_zalloc(sizeof(fq_manage_t));
	if (NULL == fq)
		return NULL;
	memset(fq, 0, sizeof(fq_manage_t));
	INIT_LIST_HEAD(&fq->data_l);
	pthread_mutex_init(&fq->lock, NULL);
	fq->magic = FQ_MAGIC;

	return fq;
}

void fq_colse(fq_manage_t *fq)
{
	if (fq) {
		pthread_mutex_destroy(&fq->lock);
		oscl_free(fq);
	}
}

OMX_BOOL fq_empty(fq_manage_t *fq)
{
	OMX_BOOL ret = OMX_FALSE;

	if (NULL == fq)
		return OMX_FALSE;
	pthread_mutex_lock(&fq->lock);
	ret = list_empty(&fq->data_l);
	pthread_mutex_unlock(&fq->lock);

	return ret ? OMX_TRUE : OMX_FALSE;
}

OMX_S32 fq_add(fq_manage_t *fq, fq_data_t *data)
{
	OMX_S32 ret = OMX_ErrorNone;

	pthread_mutex_lock(&fq->lock);
	if ((RT_NULL == fq) || (RT_NULL == data)) {
		ret = OMX_ErrorBadParameter;
		goto EXIT;
	}
	list_add_tail(&data->list, &fq->data_l);
	fq->number++;

EXIT:
	pthread_mutex_unlock(&fq->lock);
	return ret;
}

fq_data_t *fq_pop(fq_manage_t *fq)
{
	fq_data_t *data = NULL;

	if (NULL == fq)
		return NULL;
	if (OMX_TRUE == fq_empty(fq))
		return NULL;
	pthread_mutex_lock(&fq->lock);
	data = list_entry(fq->data_l.next, fq_data_t, list);
	if (data) {
		list_del(&data->list);
		fq->number--;
	}
	pthread_mutex_unlock(&fq->lock);

	return data;
}

