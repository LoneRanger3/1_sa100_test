/*
 * oscl_queue.c - queue api used by lombo media framework.
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
#define DBG_LEVEL		DBG_WARNING
#include <oscl.h>

int oscl_queue_init(oscl_queue_t *queue)
{
	int ret;

	ret = pthread_mutex_init(&queue->mutex, NULL);
	if (ret != 0)
		return -1;

	queue->head = (oscl_message_t *)oscl_malloc(sizeof(oscl_message_t));
	if (!(queue->head))
		return -1;

	memset(queue->head, 0, sizeof(oscl_message_t));
	queue->tail = queue->head;

	queue->count = 0;
	return 0;
}

void oscl_queue_deinit(oscl_queue_t *queue)
{
	oscl_message_t *tmp;

	pthread_mutex_lock(&queue->mutex);
	OSCL_TRACE("queue:%x, head:%x, count:%d\n",
		queue, queue->head, queue->count);

	while (queue->head) {
		tmp = queue->head;
		queue->head = tmp->next;
		oscl_free(tmp);
	}
	queue->count = 0;

	pthread_mutex_unlock(&queue->mutex);
	pthread_mutex_destroy(&queue->mutex);
}

void oscl_queue_flush(oscl_queue_t *queue)
{
	oscl_message_t *tmp;

	pthread_mutex_lock(&queue->mutex);
	OSCL_TRACE("queue:%x, head:%x, count:%d\n",
		queue, queue->head, queue->count);

	while (queue->head) {
		tmp = queue->head;
		queue->head = tmp->next;
		oscl_free(tmp);
	}
	queue->count = 0;
	queue->tail = NULL;
	queue->head = NULL;

	queue->head = oscl_malloc(sizeof(oscl_message_t));
	if (!(queue->head)) {
		pthread_mutex_unlock(&queue->mutex);
		return;
	}
	memset(queue->head, 0, sizeof(oscl_message_t));
	queue->tail = queue->head;

	pthread_mutex_unlock(&queue->mutex);
}

int oscl_message_put(oscl_queue_t *queue, oscl_message_t *msg_in)
{
	oscl_message_t *new_msg;
	int ret = 0;
	OSCL_TRACE("queue:%x, msg_in:%x, count:%d\n",
		queue, msg_in, queue->count);

	pthread_mutex_lock(&queue->mutex);
	oscl_param_check_exit(queue->count <= MAX_MESSAGE_CNT, -1, NULL);
	oscl_param_check_exit(queue->tail != NULL, -1, NULL);

	new_msg = oscl_malloc(sizeof(oscl_message_t));
	oscl_param_check_exit(new_msg != NULL, -1, NULL);
	memcpy(new_msg, msg_in, sizeof(oscl_message_t));

	new_msg->next = NULL;
	queue->tail->next = new_msg;
	queue->tail = new_msg;

	queue->count++;
EXIT:
	OSCL_TRACE("put message count %d\n", queue->count);
	pthread_mutex_unlock(&queue->mutex);
	return ret;
}

int oscl_message_get(oscl_queue_t *queue, oscl_message_t *msg_out)
{
	oscl_message_t *tmp;
	int ret = 0;

	pthread_mutex_lock(&queue->mutex);
	if (queue->count <= 0) {
		OSCL_LOGI("queue->count <= 0");
		ret = -1;
		goto EXIT;
	}

	tmp = queue->head;
	memcpy(msg_out, tmp->next, sizeof(oscl_message_t));
	queue->head = tmp->next;
	oscl_free(tmp);
	queue->count--;
EXIT:
	pthread_mutex_unlock(&queue->mutex);
	return ret;
}

int oscl_message_count(oscl_queue_t *queue)
{
	int count;

	pthread_mutex_lock(&queue->mutex);
	count = queue->count;
	pthread_mutex_unlock(&queue->mutex);

	return count;
}

void *oscl_message_query_next(oscl_queue_t *queue, oscl_message_t *cur_pos)
{
	oscl_message_t *tmp;

	pthread_mutex_lock(&queue->mutex);
	oscl_param_check(queue->count > 0, NULL, NULL);
	if (NULL == cur_pos)
		tmp = queue->head;
	else
		tmp = cur_pos;
	pthread_mutex_unlock(&queue->mutex);

	return tmp->next;
}

int oscl_queue_queue(oscl_queue_t *queue, void *data)
{
	oscl_message_t message;
	message.data = data;
	return oscl_message_put(queue, &message);
}

void *oscl_queue_dequeue(oscl_queue_t *queue)
{
	int ret = 0;
	void *data = NULL;
	oscl_message_t message;
	ret = oscl_message_get(queue, &message);
	if (ret == 0)
		data = message.data;
	return data;
}

void oscl_queue_list(oscl_queue_t *queue)
{
	oscl_message_t *tmp;
	int count;

	pthread_mutex_lock(&queue->mutex);
	OSCL_PRINT("%d buffers in queue %p:", oscl_queue_get_num(queue), queue);
	tmp = queue->head;
	count = queue->count;
	while ((count--) && (tmp != NULL) && (tmp->next != NULL)) {
		OSCL_PRINT("%p, ", tmp->next->data);
		tmp = tmp->next;
	}
	OSCL_PRINT("\n");

	pthread_mutex_unlock(&queue->mutex);
	return;
}


