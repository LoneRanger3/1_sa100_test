/*
 * mousedev.h - head file for mouse input handler
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

#ifndef __MOUSEDEV_H__
#define __MOUSEDEV_H__

#include "input.h"

struct mousedev_motion {
	int dx, dy, dz;
	unsigned long buttons;
};

#define PACKET_QUEUE_LEN	16
struct mousedev_client {
	u32 head;
	u32 tail;

	rt_mutex_t packet_mutex;	/* protects packets */
	struct mousedev_motion packets[PACKET_QUEUE_LEN];

	struct mousedev *msd;
	struct rt_list_node node;	/* node at client_list of mousedev */

	int pos_x;
	int pos_y;
	signed char ps2[6];
	u8 ready, buffer, bufsiz;
	u32 last_buttons;

	/* call when receive event */
	void (*notif_client)();
};

/* mouse device open and close function */
rt_err_t mousedev_open(struct mousedev_client *client);
rt_err_t mousedev_close(struct mousedev_client *client);
rt_size_t mousedev_read(struct mousedev_client *client,
				void *buf, rt_size_t count);


#endif /* __MOUSEDEV_H__ */


