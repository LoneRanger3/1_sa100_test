/*
 * system_mq.h - header file for system message queue porting interface.
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

#ifndef	_CAR_MQ_H_
#define	_CAR_MQ_H_

#include <rtthread.h>
#include <mqueue.h>
#include <semaphore.h>


#define ASYNC_FLAG	0
#define MQ_POSIX

typedef enum _CAR_MQ_TYPE_ {
	LB_CAR_BEGIN = 0xF200,
	LB_CAR_FRONT_BUFFER,
	LB_CAR_REAR_BUFFER,
	LB_CAR_END
} CAR_MQ_TYPE;

typedef struct tag_app_mq {
#ifdef MQ_POSIX
	mqd_t mq;
#else
	rt_mq_t mq;
#endif
} car_mq_t;

typedef struct tag_app_msg {
	int         type;
	int         len;
	int	    buf[4];
	void        *data;
} car_msg_t;

int car_mq_create(int flag);
int car_mq_destroy(int flag);
int car_mq_send(int msgtype, void *msg_data, int msg_len, int flag);
int car_mq_recv(car_msg_t *system_msg, int timeout_ms, int flag);

#endif
