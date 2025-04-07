/*
 * evdev.h - head file for event input handler
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

#ifndef __EVDEV_H__
#define __EVDEV_H__

#include "input.h"

/* the client buffer size must be power of 2 */
#define EV_CLNT_BUFF_SIZE		32

/* key event */
struct key_event {
	u32 code;	/* key event code */
	s32 value;
	rt_tick_t time;
};

/* touch screen event */
struct touch_event {
	u8 touch_id;		/* touch id, for mark multi-touch */
	s8 status;		/* touch status, 1: press on, 0: lift up */
	u16 x;			/* x value of coordinate */
	u16 y;			/* y value of coordinate */
	rt_tick_t time;
};

/* event device */
struct evdev {
	struct input_handle handle;
	rt_mutex_t mutex;
	struct rt_list_node client_list;	/* evdev_client list */
};


/* client of event device */
struct evdev_client {
	u32 head;
	u32 tail;

	/* position of the first element of next packet */
	u32 packet_head;
	rt_mutex_t buffer_mutex;

	u32 bufsize;	/* number of evnet in buffer */
	struct evdev *kbd;
	struct rt_list_node node;	/* node at client_list of evdev*/

	/* call when receive event */
	void (*notif_client)();

	/* must at the end of this struct */
	struct input_event buffer[];
};

/* event device open and close function */
rt_err_t evdev_open(struct evdev_client *client);
rt_err_t evdev_close(struct evdev_client *client);
rt_size_t evdev_read(struct evdev_client *client,
			struct input_event *buffer, rt_size_t count);

int evdev_init();
int evdev_deinit();

#endif /* __EVDEV_H__ */
