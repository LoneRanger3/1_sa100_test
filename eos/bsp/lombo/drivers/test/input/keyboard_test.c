/*
 * keyboard_test.c - Keyboard test driver
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
#include "input/input.h"
#include "notification.h"
#include "evdev.h"

#define TEST_KB_CMD_ADD		"add"
#define TEST_KB_CMD_REMOVE	"remove"
#define TEST_KB_CMD_REGISTER	"register"
#define TEST_KB_CMD_UNREGISTER	"unregister"
#define TEST_KB_CMD_EVENT	"event"

static struct notif_listener *test_listener;
static struct input_dev *test_dev;

void kb_rec_msg(u32 msg_t, void *msg_addr)
{
	switch (msg_t) {
	case NT_MSG_KEY:
	{
		struct key_event *e;
		e = (struct key_event *)msg_addr;
		LOG_D("test key event -> code: %d, value: %d", e->code, e->value);
	}
		break;

	default:
		break;
	}
}

static void test_add_cmd()
{
	if (test_listener)
		LOG_D("You have already add the keyboard listener");
	else {
		rt_err_t ret;

		test_listener = rt_malloc(sizeof(struct notif_listener));
		if (test_listener == RT_NULL) {
			LOG_D("rt_malloc notif_listener error");
			return;
		}

		test_listener->msgbit = NT_MSG_KEY;
		test_listener->rec_msg = kb_rec_msg;
		rt_list_init(&(test_listener->node));

		ret = notif_register_listener(test_listener);
		if (ret == RT_EOK)
			LOG_D("Add key event listener success, ",
				"now you can click and see log");
		else
			LOG_D("add cmd: notif_register_listener error");
	}
}

static void test_remove_cmd()
{
	if (test_listener) {
		notif_unregister_listener(test_listener);
		LOG_D("Remove touch event listener success, now you can't see any log");

		rt_free(test_listener);
		test_listener = RT_NULL;
	} else
		LOG_D("You should input \"add\" command before \"remove\"");
}

/* register keyboard device to input subsystem */
static void test_kbdev_register()
{
	rt_err_t ret;
	if (test_dev) {
		LOG_D("The test keyboard device have already register");
		return;
	}

	test_dev = input_allocate_device();
	if (test_dev == RT_NULL) {
		LOG_D("input_allocate_device error");
		return;
	}

	test_dev->name = "test_keyboard";

	set_bit(EV_KEY, test_dev->evbit);
	set_bit(KEY_5, test_dev->keybit);
	set_bit(KEY_6, test_dev->keybit);
	set_bit(KEY_7, test_dev->keybit);
	set_bit(KEY_8, test_dev->keybit);
	set_bit(KEY_9, test_dev->keybit);

	test_dev->num_vals = 0;
	test_dev->max_vals = 16;
	test_dev->vals = rt_malloc(sizeof(struct input_value) * test_dev->max_vals);
	if (test_dev->vals == RT_NULL) {
		LOG_D("rt_malloc for input_dev vals error");
		goto failed;
	}

	ret = input_register_device(test_dev);
	if (ret != RT_EOK) {
		LOG_D("input_register_device error");
		goto failed;
	}

	LOG_D("register test keyboard device");
	return;

failed:
	input_free_device(test_dev);
	test_dev = RT_NULL;
}

/* unregister keyborad device from input subsystem */
static void test_kbdev_unregister()
{
	if (test_dev == RT_NULL) {
		LOG_D("test keyboard is NULL, you should register first");
		return;
	}

	input_unregister_device(test_dev);
	input_free_device(test_dev);
	test_dev = RT_NULL;

	LOG_D("unregister test keyboard device");
}

/* simulate report keyboard event */
static void test_kbdev_event()
{
	if (test_dev) {
		LOG_D("report keyboard event");
		int i = 0;
		for (i = 0; i < 5; i++) {
			input_report_key(test_dev, KEY_9, 11);
			input_sync(test_dev);
		}
	} else
		LOG_D("you should register keyboard device first");
}

long test_keyboard(int argc, char **argv)
{
	LOG_D("test_keyboard...");
	if (3 == argc) {
		/* keyboard test cmd */
		char *cmd;

		cmd = argv[2];
		if (!strcmp(cmd, TEST_KB_CMD_ADD))
			test_add_cmd();
		else if (!strcmp(cmd, TEST_KB_CMD_REMOVE))
			test_remove_cmd();
		else if (!strcmp(cmd, TEST_KB_CMD_REGISTER))
			test_kbdev_register();
		else if (!strcmp(cmd, TEST_KB_CMD_UNREGISTER))
			test_kbdev_unregister();
		else if (!strcmp(cmd, TEST_KB_CMD_EVENT))
			test_kbdev_event();
		else
			LOG_D("invalid cmd\n");
	} else {
		/* test case function */
		LOG_D("test_keyboard success");
	}

	return 0;
}
