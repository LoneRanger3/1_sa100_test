/*
 * notification.c - notification module realization
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
#include "notification.h"
#include "bitops.h"
#include "evdev.h"
#include "mousedev.h"

#define KB_EVENT_TO_READ	40

/* the list for store register listener */
static rt_list_t _listener_list = RT_LIST_OBJECT_INIT(_listener_list);
static rt_mutex_t _notif_mutex;

static rt_thread_t _proc_event_thread;
static struct rt_event _nt_event;

static struct evdev_client *_evdev_client;	/* keyboard device client */
static struct mousedev_client *_mousedev_client;	/* mouse device client */

/**
 * client_listen_type - Whether to listen for this event type
 * @listener: the listener
 * @type: event type
 */
static rt_bool_t client_listen_type(struct notif_listener *listener, u32 type)
{
	if (bitmap_subset((unsigned long *)&type,
		(unsigned long *)&listener->msgbit, BITS_PER_LONG))
		return RT_TRUE;

	return RT_FALSE;
}

/**
 * client_handle_event - report event to listener
 * @listener: the listener
 * @type: event type
 * @ev: point to event data
 */
static void client_handle_event(struct notif_listener *listener, u32 type, void *ev)
{
	/* LOG_D("client_handle_event -> type: %d", type); */
	if (client_listen_type(listener, type))
		listener->rec_msg(type, ev);
}

/* register a listener for receive input event message */
rt_err_t notif_register_listener(struct notif_listener *listener)
{
	rt_err_t ret;

	ret = rt_mutex_take(_notif_mutex, RT_WAITING_FOREVER);
	if (ret != RT_EOK) {
		LOG_E("_notif_mutex rt_mutex_take error");
		return ret;
	}

	/* add the listener to list */
	rt_list_insert_after(&_listener_list, &listener->node);
	rt_mutex_release(_notif_mutex);
	return RT_EOK;
}

/* unregister a listener */
void notif_unregister_listener(struct notif_listener *listener)
{
	rt_err_t ret;

	ret = rt_mutex_take(_notif_mutex, RT_WAITING_FOREVER);
	if (ret != RT_EOK) {
		LOG_E("_notif_mutex rt_mutex_take error");
		return;
	}

	/* remove the listener */
	rt_list_remove(&listener->node);
	rt_mutex_release(_notif_mutex);
}

/* set key_event by input_event */
static void set_key_event(struct key_event *key_e,
				struct input_event *e)
{
	RT_ASSERT(e->type == EV_KEY);

	key_e->code = e->code;
	key_e->value = e->value;
	key_e->time = e->time;
}

/* set attribute of touch_event by input_event */
static void set_touch_event(struct touch_event *touch_e,
				struct input_event *e)
{
	RT_ASSERT(e->type == EV_ABS);

	switch (e->code) {
	case ABS_MT_TOUCH_MAJOR:
		touch_e->status = e->value;
		break;
	case ABS_MT_POSITION_X:
		touch_e->x = e->value;
		break;
	case ABS_MT_POSITION_Y:
		touch_e->y = e->value;
		break;
	case ABS_MT_TRACKING_ID:
		touch_e->touch_id = e->value;
		break;
	default:
		LOG_E("Unknown code for EV_ABS type");
	}

	touch_e->time = e->time;
}

/* log event for debug */
void print_event(void *ev, int type)
{
	struct key_event *key_e = RT_NULL;
	struct touch_event *touch_e = RT_NULL;

	if (ev == RT_NULL)
		return;

	if (type == NT_MSG_KEY) {
		key_e = (struct key_event *)ev;
		LOG_D("key_event -> code: %d, value: %d, time: %d",
			key_e->code, key_e->value, key_e->time);
	} else if (type == NT_MSG_TOUCH) {
		touch_e = (struct touch_event *)ev;
		LOG_D("touch_event -> touch_id: %d, status: %d, x: %d, y: %d, time: %d",
			touch_e->touch_id, touch_e->status,
			touch_e->x, touch_e->y, touch_e->time);
	}
}

/**
 * proc_syn_event - process EV_SYN event, report to listener
 * @e: input_event, type must be EV_SYN
 * @need_drop: if true, don't report event at this time
 * @key_e: key event
 * @touch_e: touch event
 */
static void proc_syn_event(struct input_event *e, rt_bool_t *need_drop,
				struct key_event *key_e, struct touch_event *touch_e)
{
	struct notif_listener *listener = RT_NULL;
	void *nt_e = RT_NULL;
	int nt_type = 0;

	RT_ASSERT(e->value == EV_SYN);

	if (e->code == SYN_REPORT) {
		/* key event */
		nt_type = NT_MSG_KEY;
		nt_e = key_e;

	} else if (e->code == SYN_MT_REPORT) {
		/* touch event */
		nt_type = NT_MSG_TOUCH;
		nt_e = touch_e;
	} else {
		/* otherwise, set the flag to drop the event next time */
		LOG_I("EV_SYN other code: %d", e->code);
		*need_drop = RT_TRUE;
		return;
	}

	if (*need_drop) {
		/* only need to drop once */
		LOG_I("drop the event...");
		*need_drop = RT_FALSE;
		return;
	}

	/* report event to listeners */
	/* print_event(nt_e, nt_type); */
	rt_list_for_each_entry(listener, &_listener_list, node)
			client_handle_event(listener, nt_type, nt_e);
}

/* read evdev event data */
static void read_evdev_event()
{
	/* the buffer to storage event data */
	struct input_event buffer[KB_EVENT_TO_READ];
	struct input_event e;
	struct key_event key_e;
	struct touch_event touch_e;

	int read = 0;	/* number of event have read */
	int total = 0;	/* total event have read */
	int i = 0;

	/* if event code is SYN_DROPPED, an event need to be drop */
	rt_bool_t need_drop = RT_FALSE;

	if (_evdev_client == RT_NULL) {
		LOG_E("read_evdev_event error: _evdev_client == RT_NULL");
		return;
	}

	/* read event data */
	read = evdev_read(_evdev_client, buffer, KB_EVENT_TO_READ);
	while (read) {
		total += read;

		for (i = 0; i < read; i++) {
			e = buffer[i];

			switch (e.type) {
			case EV_KEY:
				set_key_event(&key_e, &e);
				break;
			case EV_ABS:
				set_touch_event(&touch_e, &e);
				break;
			case EV_SYN:
				proc_syn_event(&e, &need_drop, &key_e, &touch_e);
				break;
			default:
				LOG_E("Unknown event type: %d", e.type);
			}
		}

		/* read evdev event util return 0 */
		read = evdev_read(_evdev_client, buffer, KB_EVENT_TO_READ);
	}

	/* LOG_D("read_evdev_event total: %d", total); */
}

static void read_mousedev_event()
{
	const size_t count = 6;
	signed char buf[count];
	size_t read = 0;

	read = mousedev_read(_mousedev_client, buf, count);
	LOG_D("mousedev_read read count: %d", read);

	/* TODO: notify listeners to handle the data */
}

static void notif_evdev_clients()
{
	/* send event to notify thread to process event */
	rt_event_send(&_nt_event, NT_MSG_KEY | NT_MSG_TOUCH);
}

static void notif_mousedev_clients()
{
	/* send event to notify thread to process event */
	rt_event_send(&_nt_event, NT_MSG_MOUSE);
}


/* initialize the global variable client */
static rt_err_t evdev_client_init()
{
	u32 bufsize, size;
	rt_err_t ret;
	struct evdev_client *client;

	/* the client buffer size must be power of 2 */
	bufsize = EV_CLNT_BUFF_SIZE;
	size = sizeof(struct evdev_client) +
		bufsize * sizeof(struct input_event);

	client = rt_malloc(size);
	if (client == RT_NULL) {
		LOG_E("rt_malloc error");
		return -RT_ENOMEM;
	}
	rt_memset(client, 0, size);

	client->bufsize = bufsize;
	client->buffer_mutex = rt_mutex_create("evdev_client_bf_mutex", RT_IPC_FLAG_FIFO);
	client->notif_client = notif_evdev_clients;

	/* open device and add the client to it */
	ret = evdev_open(client);
	if (ret != RT_EOK) {
		LOG_E("evdev_open error");
		ret = -RT_EINVAL;
		goto fail;
	}

	/* global variable */
	if (_evdev_client) {
		ret = evdev_close(_evdev_client);
		if (ret != RT_EOK) {
			LOG_E("_evdev_client not null, evdev_close error");
			ret = -RT_EINVAL;
			goto fail;
		}

		rt_free(_evdev_client);
	}

	_evdev_client = client;
	/* LOG_I("evdev_client_init finished"); */
	return RT_EOK;

fail:
	rt_free(client);
	return ret;
}

/* free the global variable client */
static rt_err_t evdev_client_deinit()
{
	rt_err_t ret;

	if (_evdev_client) {
		/* remove the client from device */
		ret = evdev_close(_evdev_client);
		if (ret != RT_EOK) {
			LOG_E("evdev_client_deinit error");
			return -RT_EINVAL;
		}

		rt_free(_evdev_client);
		_evdev_client = RT_NULL;
	}

	return RT_EOK;
}

static rt_err_t mousedev_client_init()
{
	rt_err_t ret;
	u32 size;
	struct mousedev_client *client;

	size = sizeof(struct mousedev_client);
	client = rt_malloc(size);
	if (client == RT_NULL) {
		LOG_E("rt_malloc error");
		return -RT_ENOMEM;
	}
	rt_memset(client, 0, size);

	client->notif_client = notif_mousedev_clients;
	client->packet_mutex = rt_mutex_create("msc_packet_mutex", RT_IPC_FLAG_FIFO);

	ret = mousedev_open(client);
	if (ret != RT_EOK) {
		LOG_E("mousedev_open error");
		ret = -RT_EINVAL;
		goto fail;
	}

	if (_mousedev_client) {
		ret = mousedev_close(_mousedev_client);
		if (ret != RT_EOK) {
			LOG_E("_mousedev_client not null, mousedev_close error");
			ret = -RT_EINVAL;
			goto fail;
		}

		rt_free(_mousedev_client);
	}

	_mousedev_client = client;
	/* LOG_I("mousedev_client_init finished"); */
	return RT_EOK;

fail:
	rt_free(client);
	return ret;

}

static rt_err_t mousedev_client_deinit()
{
	rt_err_t ret;

	if (_mousedev_client) {
		/* remove the client from device */
		ret = mousedev_close(_mousedev_client);
		if (ret != RT_EOK) {
			LOG_E("mousedev_client_deinit error");
			return -RT_EINVAL;
		}

		rt_free(_mousedev_client);
		_mousedev_client = RT_NULL;
	}

	return RT_EOK;
}

/* event thread to process report input event */
static void proc_event_thread(void *param)
{
	u32 e, ret;
	while (1) {
		ret = rt_event_recv(&_nt_event,
				NT_MSG_KEY | NT_MSG_TOUCH | NT_MSG_MOUSE,
				RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
				RT_WAITING_FOREVER, &e);
		if (ret != RT_EOK) {
			LOG_E("rt_event_recv error: %d", ret);
			continue;
		}

		switch (e) {
		case NT_MSG_KEY | NT_MSG_TOUCH:
			/* to read event and process */
			read_evdev_event();
			break;
		case NT_MSG_MOUSE:
			read_mousedev_event();
			break;
		default:
			LOG_W("Unknown event type: %d", e);
		}
	}
}

/* create an event process thread */
static void create_event_proc_thread()
{
	if (_proc_event_thread) {
		LOG_W("_proc_event_thread should be NULL");
		rt_thread_delete(_proc_event_thread);
	}

	_proc_event_thread = rt_thread_create("input_event_thread",
				proc_event_thread, RT_NULL,
				INPUT_EVENT_PROC_THREAD_STACK_SIZE,
				INPUT_THREAD_PRIORITY, 5);
	if (_proc_event_thread != RT_NULL)
		rt_thread_startup(_proc_event_thread);
	else
		LOG_E("rt_thread_create error");
}


int notif_deinit()
{
	struct notif_listener *listener, *temp;

	/* detach rt_event */
	rt_event_detach(&_nt_event);

	if (_proc_event_thread) {
		/* delete the thread */
		rt_thread_delete(_proc_event_thread);
		_proc_event_thread = RT_NULL;
	}

	/* deinit client */
	evdev_client_deinit();
	mousedev_client_deinit();

	/* remove all listeners of list */
	rt_list_for_each_entry_safe(listener, temp, &_listener_list, node)
		notif_unregister_listener(listener);

	return 0;
}

/* notification initialize, open devices and receive event */
int notif_init()
{
	/* initialize event and thread */
	rt_event_init(&_nt_event, "notif_event", RT_IPC_FLAG_FIFO);
	create_event_proc_thread();

	_notif_mutex = rt_mutex_create("_notif_mutex", RT_IPC_FLAG_FIFO);
	if (_notif_mutex == RT_NULL) {
		LOG_E("_notif_mutex rt_mutex_create error");
		return -RT_ERROR;
	}

	evdev_client_init();
	mousedev_client_init();
	return 0;
}

#ifdef ARCH_LOMBO_N7
/* notification module must initialize after tsdev, evdev ... */
INIT_ENV_EXPORT(notif_init);
#endif


