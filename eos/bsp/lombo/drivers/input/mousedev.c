/*
 * mousedev.c - mouse device module realization
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

#include <debug.h>
#include "mousedev.h"

#ifndef CONFIG_INPUT_MOUSEDEV_SCREEN_X
#define CONFIG_INPUT_MOUSEDEV_SCREEN_X	1024
#endif
#ifndef CONFIG_INPUT_MOUSEDEV_SCREEN_Y
#define CONFIG_INPUT_MOUSEDEV_SCREEN_Y	768
#endif

/* Horizontal screen resolution */
static int xres = CONFIG_INPUT_MOUSEDEV_SCREEN_X;

/* "Vertical screen resolution */
static int yres = CONFIG_INPUT_MOUSEDEV_SCREEN_Y;

struct mousedev_hw_data {
	int dx, dy, dz;
	int x, y;

	/* not support touch pad, this attribute unused */
	int abs_event;
	unsigned long buttons;
};

struct mousedev {
	rt_mutex_t mutex;
	struct rt_list_node client_list;	/* mousedev_client list */
	struct mousedev_hw_data packet;
};

static struct mousedev _mousedev;

/* print for debug */
void print_hw_data(struct mousedev_hw_data *data)
{
	LOG_I("mousedev_hw_data -> dx: %d, dy: %d, dz; %d, x: %d, y: %d, buttons: %d",
		data->dx, data->dy, data->dz, data->x, data->y, data->buttons);
}

/* print motion for debug */
void print_motion(struct mousedev_motion *motion)
{
	LOG_I("mousedev_motion -> dx: %d, dy: %d, dz; %d, buttons: %d",
		motion->dx, motion->dy, motion->dz, motion->buttons);
}

/* process mouse device relative event */
static void mousedev_rel_event(struct mousedev *mousedev,
					unsigned int code, int value)
{
	switch (code) {
	case REL_X:
		mousedev->packet.dx += value;
		break;

	case REL_Y:
		mousedev->packet.dy -= value;
		break;

	case REL_WHEEL:
		mousedev->packet.dz -= value;
		break;
	}
}

/* process mouse device key event */
static void mousedev_key_event(struct mousedev *mousedev,
				unsigned int code, int value)
{
	int index;

	switch (code) {

	case BTN_TOUCH:
	case BTN_0:
	case BTN_LEFT:
		index = 0;
		break;

	case BTN_STYLUS:
	case BTN_1:
	case BTN_RIGHT:
		index = 1;
		break;

	case BTN_2:
	case BTN_FORWARD:
	case BTN_STYLUS2:
	case BTN_MIDDLE:
		index = 2;
		break;

	case BTN_3:
	case BTN_BACK:
	case BTN_SIDE:
		index = 3;
		break;

	case BTN_4:
	case BTN_EXTRA:
		index = 4;
		break;

	default:
		return;
	}

	if (value)
		set_bit(index, &mousedev->packet.buttons);
	else
		test_and_clear_bit(index, &mousedev->packet.buttons);
}

/* notify clients of mouse device to process event data */
static void mousedev_notify_readers(struct mousedev *mousedev,
				    struct mousedev_hw_data *packet)
{
	struct mousedev_client *client;
	struct mousedev_motion *p;
	unsigned int new_head;

	rt_list_for_each_entry(client, &_mousedev.client_list, node) {
		/* protect packets of client */
		rt_enter_critical();

		p = &client->packets[client->head];
		if (client->ready && p->buttons != mousedev->packet.buttons) {
			new_head = (client->head + 1) % PACKET_QUEUE_LEN;
			if (new_head != client->tail) {
				client->head = new_head;
				p = &client->packets[client->head];
				memset(p, 0, sizeof(struct mousedev_motion));
			}
		}

		client->pos_x += packet->dx;
		client->pos_x = client->pos_x < 0 ?
			0 : (client->pos_x >= xres ? xres : client->pos_x);
		client->pos_y += packet->dy;
		client->pos_y = client->pos_y < 0 ?
			0 : (client->pos_y >= yres ? yres : client->pos_y);

		p->dx += packet->dx;
		p->dy += packet->dy;
		p->dz += packet->dz;
		p->buttons = mousedev->packet.buttons;

		if (p->dx || p->dy || p->dz ||
		    (p->buttons != client->last_buttons))
			client->ready = 1;

		rt_exit_critical();

		/* notify the client handle event */
		if (client->ready) {
			LOG_D("notif_client -> pos_x: %d, pos_y: %d",
				client->pos_x, client->pos_y);
			client->notif_client();
		}
	}
}

/**
 * mousedev_event - mopuse device pass event function
 * @handle: input handle
 * @type: event type
 * @code: event code
 * @value: event value
 */
static void mousedev_event(struct input_handle *handle,
			   unsigned int type, unsigned int code, int value)
{
	struct mousedev *mousedev = handle->private;

	switch (type) {
	case EV_REL:
		mousedev_rel_event(mousedev, code, value);
		break;

	case EV_KEY:
		mousedev_key_event(mousedev, code, value);
		break;

	case EV_SYN:
		if (code == SYN_REPORT) {
			/* report event */
			mousedev_notify_readers(mousedev, &mousedev->packet);

			/* reset value */
			mousedev->packet.dx = 0;
			mousedev->packet.dy = 0;
			mousedev->packet.dz = 0;
		}
		break;
	}
}

/* callback function for input subsystem when connect handler and device */
static rt_err_t mousedev_connect(struct input_handler *handler, struct input_dev *dev)
{
	rt_err_t ret;
	struct input_handle *handle;

	handle = rt_malloc(sizeof(struct input_handle));
	if (handle == RT_NULL) {
		LOG_E("mousedev_connect rt_malloc error");
		return -RT_ENOMEM;
	}

	handle->dev = dev;
	handle->handler = handler;
	handle->private = &_mousedev;

	/* register handle to input subsystem */
	ret = input_register_handle(handle);
	if (ret != RT_EOK)
		LOG_E("input_register_handle error -> device: %s, handler: %s",
			dev->name, handler->name);

	return ret;
}

/* callback function for input core when disconnect handler and device */
static void mousedev_disconnect(struct input_handle *handle)
{
	/* unregister handle */
	LOG_I("mousedev_disconnect -> device: %s, handler: %s",
		handle->dev->name, handle->handler->name);
	input_unregister_handle(handle);
	rt_free(handle);
}

/* set support mouse device event type and key type */
static const struct input_device_id mousedev_ids[] = {
	{
		.flags = INPUT_DEVICE_ID_MATCH_EVBIT |
				INPUT_DEVICE_ID_MATCH_KEYBIT |
				INPUT_DEVICE_ID_MATCH_RELBIT,
		.evbit = { BIT_MASK(EV_KEY) | BIT_MASK(EV_REL) },
		.keybit = { [BIT_WORD(BTN_LEFT)] = BIT_MASK(BTN_LEFT) },
		.relbit = { BIT_MASK(REL_X) | BIT_MASK(REL_Y) },
	},
	{
		.flags = INPUT_DEVICE_ID_MATCH_EVBIT |
				INPUT_DEVICE_ID_MATCH_RELBIT,
		.evbit = { BIT_MASK(EV_KEY) | BIT_MASK(EV_REL) },
		.relbit = { BIT_MASK(REL_WHEEL) },
	},
	{ },	/* Terminating entry */
};

static struct input_handler mousedev_handler = {
	 .name		= "mousedev",
	 .event		= mousedev_event,
	 .connect	= mousedev_connect,
	 .disconnect	= mousedev_disconnect,
	 .id_table	= mousedev_ids,
};

/**
 * mousedev_open - open mouse device
 * @client: client of mouse device
 *
 * return RT_EOK if success, other value if failed
 */
rt_err_t mousedev_open(struct mousedev_client *client)
{
	rt_err_t ret;

	ret = rt_mutex_take(_mousedev.mutex, RT_WAITING_FOREVER);
	if (ret != RT_EOK) {
		LOG_E("_mousedev.mutex rt_mutex_take error");
		return ret;
	}

	client->msd = &_mousedev;

	/* initialized the cursor at the center of screen */
	client->pos_x = xres / 2;
	client->pos_y = yres / 2;

	/* add the client to client list of device */
	rt_list_insert_after(&_mousedev.client_list, &client->node);
	rt_mutex_release(_mousedev.mutex);
	return RT_EOK;
}

/**
 * mousedev_close - close mouse device
 * @client: client of mouse device
 *
 * return RT_EOK if success, other value if failed
 */
rt_err_t mousedev_close(struct mousedev_client *client)
{
	rt_err_t ret;
	LOG_I("mousedev_close");

	ret = rt_mutex_take(_mousedev.mutex, RT_WAITING_FOREVER);
	if (ret != RT_EOK) {
		LOG_E("_mousedev.mutex rt_mutex_take error");
		return ret;
	}

	/* remove the client from client list of device */
	rt_list_remove(&client->node);
	rt_mutex_release(_mousedev.mutex);
	return RT_EOK;
}

/* mouse device initialize */
int mousedev_init()
{
	rt_err_t ret;

	_mousedev.mutex = rt_mutex_create("mousedev_mutex", RT_IPC_FLAG_FIFO);
	if (_mousedev.mutex == RT_NULL) {
		LOG_E("evdev_init rt_mutex_create error");
		return -RT_EINVAL;
	}

	/* client_list for store client that opened this device*/
	rt_list_init(&_mousedev.client_list);

	rt_list_init(&mousedev_handler.node);
	rt_list_init(&mousedev_handler.h_list);

	/* register the handler to input subsystem */
	ret = input_register_handler(&mousedev_handler);
	if (ret != RT_EOK) {
		LOG_E("mousedev_init error");
		return ret;
	}

	return RT_EOK;
}

int mousedev_deinit()
{
	struct mousedev_client *client, *temp;
	rt_err_t ret;

	/* unregister evdev handler */
	input_unregister_handler(&mousedev_handler);

	/* remove all clients */
	ret = rt_mutex_take(_mousedev.mutex, RT_WAITING_FOREVER);
	if (ret != RT_EOK) {
		LOG_E("_mousedev.mutex rt_mutex_take error");
		return ret;
	}

	/* remove all clients */
	rt_list_for_each_entry_safe(client, temp, &_mousedev.client_list, node)
		rt_list_remove(&client->node);
	rt_mutex_release(_mousedev.mutex);

	rt_mutex_delete(_mousedev.mutex);
	return RT_EOK;
}

static inline int mousedev_limit_delta(int delta, int limit)
{
	return delta > limit ? limit : (delta < -limit ? -limit : delta);
}

static void mousedev_packet(struct mousedev_client *client,
			    signed char *ps2_data)
{
	struct mousedev_motion *p = &client->packets[client->tail];

	/* copy from linux mousedev.c ... */
	ps2_data[0] = 0x08 |
		((p->dx < 0) << 4) | ((p->dy < 0) << 5) | (p->buttons & 0x07);
	ps2_data[1] = mousedev_limit_delta(p->dx, 127);
	ps2_data[2] = mousedev_limit_delta(p->dy, 127);
	p->dx -= ps2_data[1];
	p->dy -= ps2_data[2];


	/* ps2 */
	ps2_data[0] |= ((p->buttons & 0x10) >> 3) | ((p->buttons & 0x08) >> 1);
	p->dz = 0;
	client->bufsiz = 3;

	if (!p->dx && !p->dy && !p->dz) {
		if (client->tail == client->head) {
			client->ready = 0;
			client->last_buttons = p->buttons;
		} else
			client->tail = (client->tail + 1) % PACKET_QUEUE_LEN;
	}
}

/**
 * mousedev_read - read event from client
 * @client: client to read
 * @buffer: to store data
 * @count: count of data to read
 *
 * return count of data have read
 */
rt_size_t mousedev_read(struct mousedev_client *client,
				void *buf, rt_size_t count)
{
	signed char data[sizeof(client->ps2)];

	if (!client->ready && !client->buffer)
		return 0;

	/* protect packets of client */
	rt_enter_critical();

	if (!client->buffer && client->ready) {
		mousedev_packet(client, client->ps2);
		client->buffer = client->bufsiz;
	}

	if (count > client->buffer)
		count = client->buffer;

	rt_memcpy(data, client->ps2 + client->bufsiz - client->buffer, count);
	client->buffer -= count;

	rt_exit_critical();

	/* pass to function caller */
	rt_memcpy(buf, data, count);

	return count;
}

#ifdef ARCH_LOMBO_N7
INIT_COMPONENT_EXPORT(mousedev_init);
#endif
