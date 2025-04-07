/*
 * car_mq.c - car message queue interface for lombo platform
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
#include <string.h>
#include <stdlib.h>
#include "car_mq.h"
#include "mod_bsd.h"
#include <clock_time.h>
#define     CAR_MQ_SIZE         64
#define     CAR_MQ_MAX_NUM      1

static car_mq_t carmq;

/**
 * car_mq_create - create car message queue
 *
 * This function use create car message queue, create a async and
 * a sync message queue.
 *
 * Returns 0 if succes,
 * returns -1 if create "car_mq" message queue failed,
 * returns -2 if create "car_sync_mq" message queue failed.
 */
int car_mq_create(int flag)
{
#ifdef MQ_POSIX
	struct mq_attr      mqstat;
	int                 flags = O_CREAT | O_RDWR | O_NONBLOCK;

	memset(&mqstat, 0, sizeof(struct mq_attr));
	mqstat.mq_maxmsg    = CAR_MQ_MAX_NUM;
	mqstat.mq_msgsize   = CAR_MQ_SIZE;
	mqstat.mq_flags     = flags;
	if (flag == ASYNC_FLAG) {
		if (carmq.mq == NULL) {
			carmq.mq = mq_open("car_mq", flags, 0666, &mqstat);
			if (carmq.mq == NULL) {
				MOD_LOG_E("Message queue open failed!\n");
				return -1;
			}
		}
	} else
		MOD_LOG_E("Invalid parameter!\n");
#else
	carmq.mq = rt_mq_create("car_mq", CAR_MQ_SIZE, CAR_MQ_MAX_NUM, RT_IPC_FLAG_FIFO);
	if (carmq.mq == RT_NULL) {
		MOD_LOG_E("Message queue open failed!\n");
		return -1;
	}
#endif

	return 0;
}

/**
 * car_mq_destroy - destroy car message queue
 *
 * This function use to destroy a async and a sync message queue.
 *
 * Returns 0 if succes,returns -1 if message queue is not exist.
 */
int car_mq_destroy(int flag)
{
	if (flag == ASYNC_FLAG) {
		if (NULL == carmq.mq) {
			MOD_LOG_E("Invalid parameter!\n");
			return -1;
		}
#ifdef MQ_POSIX
		mq_unlink("car_mq");
		mq_close(carmq.mq);
#else
		rt_mq_delete(carmq.mq);
#endif
	}

	return 0;
}

/**
 * car_mq_send - send a car message by message queue
 * @msgtype:   car message type,see in car_mq.h.
 * @msg_data:  message data pointer.
 * @msg_len: message data length.
 * @flag: 0:async flag,default value; 1: sync flag.
 *
 * This function use to send a car message by message queue,
 * if flag is 0,send a car message by "car_mq" queue,
 * if flag is 0,send a car message by "car_sync_mq" queue
 *
 * Returns 0 if succes,
 * returns -1 if message queue is not exist,
 * returns -2 if message length is too long,
 * returns -3 if message buffer create failed,
 * returns -4 if message send failed
 */
int car_mq_send(int msgtype, void *msg_data, int msg_len, int flag)
{
	char            *p_msg_buf = NULL;
	int             err = 0;
	unsigned int    msg_act_len = 0;

	if (flag == ASYNC_FLAG) {
		if (NULL == carmq.mq) {
			MOD_LOG_E("Invalid parameter!\n");
			return -1;
		}
	} else {
		MOD_LOG_E("Invalid parameter!\n");
		return -5;
	}
	msg_act_len = sizeof(int) + sizeof(int) + msg_len;
	if (msg_act_len > CAR_MQ_SIZE) {
		MOD_LOG_E("Msg is too long!\n");
		return -2;
	}
	p_msg_buf = (char *)malloc(msg_act_len);
	if (NULL == p_msg_buf) {
		MOD_LOG_E("No enough memory!\n");
		return -3;
	}

	memcpy(p_msg_buf, &msgtype, sizeof(int));
	memcpy(p_msg_buf + sizeof(int), &msg_len, sizeof(int));
	if (msg_len)
		memcpy(p_msg_buf + sizeof(int) + sizeof(int), msg_data, msg_len);
	if (flag == ASYNC_FLAG) {
#ifdef MQ_POSIX
		err = mq_send(carmq.mq, p_msg_buf, msg_act_len, 0);
#else
		err = rt_mq_send(carmq.mq, p_msg_buf, msg_act_len);
#endif
		if (err == -1) {
			free(p_msg_buf);
			return -4;
		}
	}
	free(p_msg_buf);

	return 0;
}

/**
 * car_mq_recv - receive a car message from message queue
 * @system_msg: message data struct,defined in car_mq.h
 * @timeout_ms: wait out time,ms unit.
 * @flag: 0,async flag,default value; 1,sync flag.
 *
 * This function use to receive a car message from message queue,
 * if flag is 0,receive a car message from "car_mq" queue,
 * if flag is 1,receive a car message from "car_sync_mq" queue
 *
 * Returns 0 if succes,
 * returns -1 if message queue is not exist or system_msg struct is NULL,
 * returns -2 if receive a car message failed
 */
int car_mq_recv(car_msg_t *system_msg, int timeout_ms, int flag)
{
	int		err = 0;
	char	recv_buf[CAR_MQ_SIZE] = {0};
	struct	timespec tmo;

	if (NULL == system_msg) {
		MOD_LOG_E("Invalid parameter!\n");
		return -1;
	}
	if (timeout_ms == 0) {
		if (flag == ASYNC_FLAG)
#ifdef MQ_POSIX
			err = mq_receive(carmq.mq, recv_buf, CAR_MQ_SIZE, 0);
#else
			err = rt_mq_recv(carmq.mq, recv_buf, CAR_MQ_SIZE, timeout_ms);
#endif
	} else {
		clock_gettime(CLOCK_REALTIME, &tmo);
		tmo.tv_nsec += (timeout_ms % 1000) * 1000 * 1000;
		tmo.tv_sec  += timeout_ms / 1000;
		if (flag == ASYNC_FLAG) {
#ifdef MQ_POSIX

			err = mq_timedreceive(carmq.mq, recv_buf, CAR_MQ_SIZE, 0, &tmo);
#else
			err = rt_mq_recv(carmq.mq, recv_buf, CAR_MQ_SIZE, timeout_ms);
#endif

			}
	}
	if (err < 0)
		return -2;

	memcpy(&(system_msg->type), &recv_buf[0], sizeof(int));
	memcpy(&(system_msg->len), &recv_buf[4], sizeof(int));
	if (system_msg->len <= 4) {
		memset(system_msg->buf, 0, system_msg->len);
		memcpy(system_msg->buf, &recv_buf[8], system_msg->len);
	} else if (system_msg->len) {
		system_msg->data = malloc(system_msg->len + 1);
		memset(system_msg->data, 0, system_msg->len + 1);
		memcpy(system_msg->data, &recv_buf[8], system_msg->len);
	}

	return 0;
}
