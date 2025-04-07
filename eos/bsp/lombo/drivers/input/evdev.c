/*
 * evdev.c - event device module realization
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
#include "evdev.h"

static struct evdev _dev;

/* storage the event to buffer of client */
static void __pass_event(struct evdev_client *client,
			 const struct input_event *event)
{
	client->buffer[client->head++] = *event;
	client->head &= client->bufsize - 1;	/* like % operation */

	rt_enter_critical();
	if (unlikely(client->head == client->tail)) {
		/*
		 * This effectively "drops" all unconsumed events, leaving
		 * EV_SYN/SYN_DROPPED plus the newest event in the queue.
		 */
		client->tail = (client->head - 2) & (client->bufsize - 1);

		client->buffer[client->tail].time = event->time;
		client->buffer[client->tail].type = EV_SYN;
		client->buffer[client->tail].code = SYN_DROPPED;
		client->buffer[client->tail].value = 0;

		client->packet_head = client->tail;
	}
	rt_exit_critical();

	/* notify clients to process input events if event code is _xxx_REPORT */
	if (event->type == EV_SYN &&
		(event->code == SYN_REPORT || event->code == SYN_MT_REPORT)) {
		client->packet_head = client->head;
		client->notif_client();
	}
}

/* pass values to client */
static void evdev_pass_values(struct evdev_client *client,
			const struct input_value *vals, unsigned int count,
			rt_tick_t time)
{
	const struct input_value *v;
	struct input_event event;

	event.time = time;
	for (v = vals; v != vals + count; v++) {
		event.type = v->type;
		event.code = v->code;
		event.value = v->value;
		__pass_event(client, &event);
	}
}

static rt_bool_t evdev_match(struct input_handler *handler, struct input_dev *dev)
{
	/* evdev not handle mouse event */
	const u32 count = EV_CNT / BITS_PER_LONG + 1;
	unsigned long ignore_evbit[count];
	rt_memset(ignore_evbit, 0 , sizeof(unsigned long) * count);

	set_bit(EV_REL, ignore_evbit);
	if (bitmap_subset(ignore_evbit, dev->evbit, EV_MAX)) {
		LOG_I("evdev ignore device: %s", dev->name);
		return RT_FALSE;
	}

	return RT_TRUE;
}

/**
 * evdev_events - event device pass events function
 * @handle: input handle
 * @vals: input value array
 * @count: count of values
 */
static void evdev_events(struct input_handle *handle,
				const struct input_value *vals, unsigned int count)
{
	struct evdev_client *client;
	rt_tick_t time = rt_tick_get();	/* current time */

	rt_list_for_each_entry(client, &_dev.client_list, node)
		evdev_pass_values(client, vals, count, time);
}

/**
 * evdev_event - event device pass event function
 * @handle: input handle
 * @type: event type
 * @code: event code
 * @value: event value
 */
static void evdev_event(struct input_handle *handle,
		unsigned int type, unsigned int code, int value)
{
	/* call events function */
	struct input_value vals[] = { { type, code, value } };
	evdev_events(handle, vals, 1);
}

/* callback function for input subsystem when connect handler and device */
static rt_err_t evdev_connect(struct input_handler *handler, struct input_dev *dev)
{
	rt_err_t ret;
	struct input_handle *handle = RT_NULL;

	handle = rt_malloc(sizeof(struct input_handle));
	if (handle == RT_NULL) {
		LOG_E("evdev_connect rt_malloc error");
		return -RT_ENOMEM;
	}

	/* handle connect device and handler */
	handle->dev = dev;
	handle->handler = handler;
	handle->private = &_dev;

	/* register handle to input subsystem */
	ret = input_register_handle(handle);
	if (ret != RT_EOK)
		LOG_E("input_register_handle error -> device: %s, handler: %s",
			dev->name, handler->name);

	return ret;
}

/* callback function for input core when disconnect handler and device */
static void evdev_disconnect(struct input_handle *handle)
{
	/* unregister handle */
	LOG_D("evdev_disconnect -> device: %s, handler: %s",
		handle->dev->name, handle->handler->name);
	input_unregister_handle(handle);
	rt_free(handle);
}

/* set support all event type and key type */
static const struct input_device_id evdev_ids[] = {
	{ .driver_info = 1 },	/* Matches all devices */
	{ },			/* Terminating entry */
};

static struct input_handler evdev_handler = {
	 .name		= "evdev",
	 .match		= evdev_match,
	 .events	= evdev_events,
	 .event		= evdev_event,
	 .connect	= evdev_connect,
	 .disconnect	= evdev_disconnect,
	 .id_table	= evdev_ids,
};

/**
 * evdev_open - open event device
 * @client: client of event device
 *
 * return RT_EOK if success, other value if failed
 */
rt_err_t evdev_open(struct evdev_client *client)
{
	rt_err_t ret;

	ret = rt_mutex_take(_dev.mutex, RT_WAITING_FOREVER);
	if (ret != RT_EOK) {
		LOG_E("_dev.mutex rt_mutex_take error");
		return ret;
	}

	client->kbd = &_dev;

	/* add the client to client list of device */
	rt_list_insert_after(&_dev.client_list, &client->node);
	rt_mutex_release(_dev.mutex);
	return RT_EOK;
}

/**
 * evdev_close - close event device
 * @client: client of event device
 *
 * return RT_EOK if success, other value if failed
 */
rt_err_t evdev_close(struct evdev_client *client)
{
	rt_err_t ret;

	ret = rt_mutex_take(_dev.mutex, RT_WAITING_FOREVER);
	if (ret != RT_EOK) {
		LOG_E("_dev.mutex rt_mutex_take error");
		return ret;
	}

	/* remove the client from client list of device */
	rt_list_remove(&client->node);
	rt_mutex_release(_dev.mutex);
	return RT_EOK;
}

/**
 * evdev_fetch_next_event -  fetch the next event from client buffer
 * @event: to store fetch event
 * @client: the client to fetch event
 *
 * return 0 if no fetch event
 */
static int evdev_fetch_next_event(struct evdev_client *client,
				  struct input_event *event)
{
	int have_event = 0;
	rt_enter_critical();

	have_event = client->packet_head != client->tail;
	if (have_event) {
		/* update the tail positon */
		*event = client->buffer[client->tail++];
		client->tail &= client->bufsize - 1;
	}

	rt_exit_critical();
	return have_event;
}

/**
 * evdev_read - read event from client
 * @client: client to read
 * @buffer: to store event
 * @count: count of event to read
 *
 * return count of event have read
 */
rt_size_t evdev_read(struct evdev_client *client,
			struct input_event *buffer, rt_size_t count)
{
	struct input_event event;
	rt_size_t read = 0;

	if (count == 0)
		return read;

	/* client buffer have not event */
	if (client->packet_head == client->tail) {
		/* LOG_D("client buffer empty"); */
		return read;
	}

	/* fetch event from client */
	while (read < count &&
		evdev_fetch_next_event(client, &event)) {
		*(buffer + read) = event;
		read++;
	}

	return read;
}

/* event device initialize */
int evdev_init()
{
	rt_err_t ret;

	_dev.mutex = rt_mutex_create("evdev_mutex", RT_IPC_FLAG_FIFO);
	if (_dev.mutex == RT_NULL) {
		LOG_E("evdev_init rt_mutex_create error");
		return -RT_EINVAL;
	}

	/* client_list for store client that opened this device*/
	rt_list_init(&_dev.client_list);

	rt_list_init(&evdev_handler.node);
	rt_list_init(&evdev_handler.h_list);

	/* register the handler to input subsystem */
	ret = input_register_handler(&evdev_handler);
	if (ret != RT_EOK) {
		LOG_E("evdev_init error");
		return ret;
	}

	return 0;
}

int evdev_deinit()
{
	struct evdev_client *client, *temp;
	rt_err_t ret;

	/* unregister evdev handler */
	input_unregister_handler(&evdev_handler);

	/* remove all clients */
	ret = rt_mutex_take(_dev.mutex, RT_WAITING_FOREVER);
	if (ret != RT_EOK) {
		LOG_E("_dev.mutex rt_mutex_take error");
		return ret;
	}

	rt_list_for_each_entry_safe(client, temp, &_dev.client_list, node)
		rt_list_remove(&client->node);
	rt_mutex_release(_dev.mutex);

	rt_mutex_delete(_dev.mutex);
	return RT_EOK;
}

#ifdef ARCH_LOMBO_N7
INIT_COMPONENT_EXPORT(evdev_init);
#endif

