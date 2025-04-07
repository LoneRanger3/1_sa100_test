/*
 * oscl_queue.h - queue api used by lombo media framework.
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

#ifndef __OSCL_QUEUE_H__
#define __OSCL_QUEUE_H__

#include "oscl.h"

#define MAX_MESSAGE_CNT 1000

typedef struct oscl_message {
	void *comp;
	int msg_type;
	int para1;
	void *data;
	struct oscl_message *next;
} oscl_message_t;

typedef struct oscl_queue {
	oscl_message_t *head;
	oscl_message_t *tail;
	int count;
	pthread_mutex_t mutex;
} oscl_queue_t;

int oscl_queue_init(oscl_queue_t *queue);
void oscl_queue_deinit(oscl_queue_t *queue);
void oscl_queue_flush(oscl_queue_t *queue);
int oscl_message_put(oscl_queue_t *queue, oscl_message_t *msg_in);
int oscl_message_get(oscl_queue_t *queue, oscl_message_t *msg_out);
int oscl_message_count(oscl_queue_t *queue);
void *oscl_message_query_next(oscl_queue_t *queue, oscl_message_t *cur_pos);
int oscl_queue_queue(oscl_queue_t *queue, void *data);
void *oscl_queue_dequeue(oscl_queue_t *queue);
void oscl_queue_list(oscl_queue_t *queue);

#define oscl_queue_get_num(queue) oscl_message_count(queue)
#define oscl_queue_query_next(queue, cur_pos) oscl_message_query_next(queue, cur_pos)

#endif /* __OSCL_QUEUE_H__ */
