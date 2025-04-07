/*
 * lombo_isp.h - lombo isp head file
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
#ifndef LOMBO_ISP_H
#define LOMBO_ISP_H
#include <rtthread.h>

#define		ISP_ASYNC_FLAG		0
#define		ISP_SYNC_FLAG		1

#define		LB_ISP_MSG_SIZE		64
#define		LB_ISP_MSG_MAX_NUM	128


typedef enum _ISP_MSG_TYPE_ {
	LB_ISPMSG_BEGIN = 0xF000,
	LB_ISPMSG_ROOTFS_MOUNT_OK,
	LB_ISPMSG_ROOTFS_MOUNT_FAIL,
	LB_ISPMSG_END,
} ISP_MSG_TYPE;

typedef struct tag_isp_mq {
	rt_mq_t mq;
	rt_mq_t sync_mq;
	rt_sem_t sync_sem;
} lb_isp_mq_t;

typedef struct tag_isp_msg {
	int         type;
	int         len;
	char        data[LB_ISP_MSG_SIZE-8];
} lb_isp_msg_t;

int lb_isp_mq_create(void);
int lb_isp_mq_destroy(void);
int lb_isp_mq_send(int msgtype, void *msg_data, int msg_len, int flag);
int lb_isp_mq_recv(lb_isp_msg_t *isp_msg, int timeout_ms, int flag);


/* isp run main function */
int lombo_isp_run();

#endif
