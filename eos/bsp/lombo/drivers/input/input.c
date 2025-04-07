/*
 * input.c - input module realization
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

#include <rtthread.h>
#include <debug.h>
#include "input.h"

static rt_mutex_t input_mutex;
static rt_list_t _dev_list = RT_LIST_OBJECT_INIT(_dev_list);
static rt_list_t _handler_list = RT_LIST_OBJECT_INIT(_handler_list);

static struct rt_event _int_event;
static rt_thread_t _int_thread;
static rt_list_t _int_handler_list = RT_LIST_OBJECT_INIT(_int_handler_list);

/* allocate memory for new input device */
struct input_dev *input_allocate_device(void)
{
	struct input_dev *dev;
	rt_size_t sz = sizeof(struct input_dev);

	dev = rt_malloc(sz);
	if (dev == RT_NULL) {
		LOG_E("input_allocate_device rt_malloc error");
		return dev;
	}

	rt_memset(dev, 0, sz);
	rt_list_init(&dev->node);
	rt_list_init(&dev->h_list);

	return dev;
}

void input_free_device(struct input_dev *dev)
{
	RT_ASSERT(dev != RT_NULL);
	if (dev->vals)
		rt_free(dev->vals);

	rt_free(dev);
}

static const struct input_device_id *input_match_device(struct input_handler *handler,
	struct input_dev *dev)
{
	const struct input_device_id *id;
	for (id = handler->id_table; id->flags || id->driver_info; id++) {
		if (!bitmap_subset(id->evbit, dev->evbit, EV_MAX))
			continue;

		if (!bitmap_subset(id->keybit, dev->keybit, KEY_MAX))
			continue;

		if (!bitmap_subset(id->absbit, dev->absbit, ABS_MAX))
			continue;

		if (!handler->match || handler->match(handler, dev))
			return id;
	}

	return RT_NULL;
}

static rt_err_t input_attach_handler(struct input_handler *handler, struct input_dev *dev)
{
	const struct input_device_id *id;
	rt_err_t ret;

	id = input_match_device(handler, dev);
	if (!id)
		return RT_EINVAL;

	/* LOG_I("input_attach_handler match -> device = %s, handler = %s",
		dev->name, handler->name); */
	ret = handler->connect(handler, dev);

	return ret;
}

static inline int is_event_supported(unsigned int code,
				     unsigned long *bm, unsigned int max)
{
	int ret;

	ret = code <= max && test_bit(code, bm);
	return ret;
}

/* pass events to handler */
static unsigned int input_to_handler(struct input_handle *handle,
			struct input_value *vals, unsigned int count)
{
	struct input_handler *handler;

	handler = handle->handler;
	if (handler->events)
		handler->events(handle, vals, count);
	else if (handler->event) {
		struct input_value *v;
		int i;
		for (i = 0; i < count; i++) {
			v = (struct input_value *)(vals + i);
			handler->event(handle, v->type, v->code, v->value);
		}
	}

	return count;
}

static void input_pass_values(struct input_dev *dev,
			      struct input_value *vals, unsigned int count)
{
	struct input_handle *handle;
	rt_list_for_each_entry(handle, &dev->h_list, d_node)
		input_to_handler(handle, vals, count);
}

static void input_handle_event(struct input_dev *dev,
			       unsigned int type, unsigned int code, int value)
{
	struct input_value *v;

	if (!dev->vals) {
		LOG_W("dev->vals == NULL");
		return;
	}

	if (dev->num_vals >= (dev->max_vals - 1))
		dev->num_vals = 0;

	/* input_value struct for pass to client */
	v = &dev->vals[dev->num_vals];
	dev->num_vals++;
	v->type = type;
	v->code = code;
	v->value = value;

	/* report event */
	if ((type == EV_SYN) &&
		((code == SYN_REPORT) || (code == SYN_MT_REPORT))) {
		input_pass_values(dev, dev->vals, dev->num_vals);
		dev->num_vals = 0;
	}
}

static void input_handle_int(void *param)
{
	struct input_int_handler *handler;
	u32 e, ret;

	while (1) {
		ret = rt_event_recv(&_int_event,
			       INPUT_EVENT_ALL,
			       RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
			       RT_WAITING_FOREVER, &e);
		if (ret != RT_EOK) {
			LOG_E("rt_event_recv error: %d", ret);
			continue;
		}

		rt_list_for_each_entry(handler, &_int_handler_list, node)
			if (handler->type == e)
				handler->int_handle();
	}
}

/* create interrupt event handle thread */
static void create_int_handle_thread()
{
	if (_int_thread) {
		LOG_W("_int_thread should be NULL");
		rt_thread_delete(_int_thread);
	}

	_int_thread = rt_thread_create("input_int_thread",
				input_handle_int, RT_NULL,
				INPUT_INT_THREAD_STACK_SIZE,
				INPUT_THREAD_PRIORITY, 5);
	if (_int_thread != RT_NULL)
		rt_thread_startup(_int_thread);
	else
		LOG_E("rt_thread_create error");
}

/* register device with input core */
rt_err_t input_register_device(struct input_dev *dev)
{
	struct input_handler *handler;
	rt_err_t ret;

	ret = rt_mutex_take(input_mutex, RT_WAITING_FOREVER);
	if (ret != RT_EOK) {
		LOG_E("input_mutex rt_mutex_take error");
		return ret;
	}

	set_bit(EV_SYN, dev->evbit);
	rt_list_insert_after(&_dev_list, &dev->node);
	rt_mutex_release(input_mutex);

	rt_list_for_each_entry(handler, &_handler_list, node)
		input_attach_handler(handler, dev);

	return RT_EOK;
}

/* unregister previously registered device */
void input_unregister_device(struct input_dev *dev)
{
	struct input_handle *handle, *temp;
	rt_err_t ret;

	LOG_D("input_unregister_device: %s", dev->name);

	ret = rt_mutex_take(input_mutex, RT_WAITING_FOREVER);
	if (ret != RT_EOK) {
		LOG_E("input_mutex rt_mutex_take error");
		return;
	}

	rt_list_for_each_entry_safe(handle, temp, &dev->h_list, d_node)
		handle->handler->disconnect(handle);

	rt_list_remove(&dev->node);
	rt_mutex_release(input_mutex);
}

/**
 * input_register_handler - register a new input handler
 * @handler: handler to be registered
 *
 * This function registers a new input handler (interface) for input
 * devices in the system and attaches it to all input devices that
 * are compatible with the handler.
 */
rt_err_t input_register_handler(struct input_handler *handler)
{
	struct input_dev *dev;
	rt_err_t ret;

	ret = rt_mutex_take(input_mutex, RT_WAITING_FOREVER);
	if (ret != RT_EOK) {
		LOG_E("input_mutex rt_mutex_take error");
		return ret;
	}

	rt_list_insert_after(&_handler_list, &handler->node);
	rt_mutex_release(input_mutex);

	rt_list_for_each_entry(dev, &_dev_list, node)
		input_attach_handler(handler, dev);

	return RT_EOK;
}

/**
 * input_unregister_handler - unregisters an input handler
 * @handler: handler to be unregistered
 *
 * This function disconnects a handler from its input devices and
 * removes it from lists of known handlers.
 */
void input_unregister_handler(struct input_handler *handler)
{
	struct input_handle *handle, *temp;
	rt_err_t ret;

	LOG_D("input_unregister_handler: %s", handler->name);

	ret = rt_mutex_take(input_mutex, RT_WAITING_FOREVER);
	if (ret != RT_EOK) {
		LOG_E("input_mutex rt_mutex_take error");
		return;
	}

	rt_list_for_each_entry_safe(handle, temp, &handler->h_list, h_node)
		handler->disconnect(handle);

	rt_list_remove(&handler->node);
	rt_mutex_release(input_mutex);
}

/**
 * input_register_handle - register a new input handle
 * @handle: handle to register
 *
 * This function is supposed to be called from handler's
 * connect() method.
 */
rt_err_t input_register_handle(struct input_handle *handle)
{
	struct input_handler *handler = handle->handler;
	struct input_dev *dev = handle->dev;

	rt_list_insert_after(&dev->h_list, &handle->d_node);
	rt_list_insert_after(&handler->h_list, &handle->h_node);

	return RT_EOK;
}

/**
 * input_unregister_handle - unregister an input handle
 * @handle: handle to unregister
 *
 * This function removes input handle from device's
 * and handler's lists.
 *
 * This function is supposed to be called from handler's
 * disconnect() method.
 */
void input_unregister_handle(struct input_handle *handle)
{
	rt_list_remove(&handle->d_node);
	rt_list_remove(&handle->h_node);
}

void input_send_int_event(u32 type)
{
	rt_event_send(&_int_event, type);
}

rt_err_t input_register_int_handler(struct input_int_handler *handler)
{
	rt_err_t ret;

	if (handler == RT_NULL)
		return -RT_EINVAL;

	ret = rt_mutex_take(input_mutex, RT_WAITING_FOREVER);
	if (ret != RT_EOK) {
		LOG_E("input_mutex rt_mutex_take error = %d", ret);
		return ret;
	}

	rt_list_insert_after(&_int_handler_list, &handler->node);
	rt_mutex_release(input_mutex);

	return RT_EOK;
}

void input_unregister_int_handler(struct input_int_handler *handler)
{
	rt_err_t ret;

	if (handler == RT_NULL)
		return;

	ret = rt_mutex_take(input_mutex, RT_WAITING_FOREVER);
	if (ret != RT_EOK) {
		LOG_E("input_mutex rt_mutex_take error = %d", ret);
		return;
	}

	rt_list_remove(&handler->node);
	rt_mutex_release(input_mutex);
}

/**
 * input_event() - report new input event
 * @dev: device that generated the event
 * @type: type of the event
 * @code: event code
 * @value: value of the event
 */
void input_event(struct input_dev *dev, unsigned int type, unsigned int code, int value)
{
	if (is_event_supported(type, dev->evbit, EV_MAX))
		input_handle_event(dev, type, code, value);
}

void input_log_event(struct input_event *i_e)
{
	LOG_D("input_log_event -> time: %d, type: %d, code: %d, value: %d",
		i_e->time, i_e->type, i_e->code, i_e->value);
}

int input_core_init()
{
	/* mutex for protect _dev_list and _handler_list */
	input_mutex = rt_mutex_create("input_mutex", RT_IPC_FLAG_FIFO);

	/* initialize event for interrupt message */
	rt_event_init(&_int_event, "input_int_event", RT_IPC_FLAG_FIFO);

	/* create a thread for handle all input event interrupt */
	create_int_handle_thread();

	return 0;
}

#ifdef ARCH_LOMBO_N7
INIT_PREV_EXPORT(input_core_init);
#endif

