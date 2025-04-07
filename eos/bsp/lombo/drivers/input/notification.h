/*
 * notification.h - notification module realization
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

#ifndef __NOTIFICATION_H__
#define __NOTIFICATION_H__

#include <rtthread.h>
#include "../board.h"

#define NT_MSG_KEY		BIT(0)		/* keyboard event */
#define NT_MSG_TOUCH		BIT(1)		/* touch screen event */
#define NT_MSG_MOUSE		BIT(2)		/* mouse event */

struct notif_listener {
	struct rt_list_node node;	/* node at listener_list */
	u32 msgbit;			/* bitmap, temporary support 32 events */

	/* receive message callback function */
	void (*rec_msg)(u32 msg_t, void *msg_addr);
};

/* register or unregister listener for notification message */
rt_err_t notif_register_listener(struct notif_listener *listener);
void notif_unregister_listener(struct notif_listener *listener);

/* notification init and deinit function, for test */
int notif_init();
int notif_deinit();


#endif /* __NOTIFICATION_H__ */
