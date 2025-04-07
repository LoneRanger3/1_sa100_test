/*
 * input_test.c - input test driver
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
#include "input/notification.h"
#include "evdev.h"

#define TEST_INPUT_CMD_MOUSE_EVENT	"mouse"
#define TEST_INPUT_CMD_REGISTER		"register"
#define TEST_INPUT_CMD_UNREGISTER	"unregister"

#define TEST_INPUT_CMD_NT_INIT		"nt_init"
#define TEST_INPUT_CMD_NT_DEINIT	"nt_deinit"
#define TEST_INPUT_CMD_EVDEV_INIT	"ev_init"
#define TEST_INPUT_CMD_EVDEV_DEINIT	"ev_deinit"

static struct input_dev *test_dev;

/* simulate report mouse event */
static void report_mouse()
{
	if (test_dev) {
		LOG_D("report mouse event");

		signed char data[] = {3, 34, 78, 3, 0, 0};
		input_report_key(test_dev, BTN_LEFT,   data[0] & 0x01);
		input_report_key(test_dev, BTN_RIGHT,  data[0] & 0x02);
		input_report_key(test_dev, BTN_MIDDLE, data[0] & 0x04);
		input_report_key(test_dev, BTN_SIDE,   data[0] & 0x08);
		input_report_key(test_dev, BTN_EXTRA,  data[0] & 0x10);

		input_report_rel(test_dev, REL_X,     data[1]);
		input_report_rel(test_dev, REL_Y,     data[2]);
		input_report_rel(test_dev, REL_WHEEL, data[3]);

		input_sync(test_dev);
	} else
		LOG_D("you should register test device first");
}

/* register test device */
static void register_device()
{
	rt_err_t ret;
	if (test_dev) {
		LOG_D("The test device have already register");
		return;
	}

	test_dev = input_allocate_device();
	if (test_dev == RT_NULL) {
		LOG_D("input_allocate_device error");
		return;
	}

	/* register mouse device */
	test_dev->name = "test_device";

	/* event bit */
	set_bit(EV_KEY, test_dev->evbit);
	set_bit(EV_REL, test_dev->evbit);

	/* key bit */
	set_bit(BTN_LEFT, test_dev->keybit);
	set_bit(BTN_RIGHT, test_dev->keybit);
	set_bit(BTN_MIDDLE, test_dev->keybit);
	set_bit(BTN_SIDE, test_dev->keybit);
	set_bit(BTN_EXTRA, test_dev->keybit);

	/* relative bit */
	set_bit(REL_X, test_dev->relbit);
	set_bit(REL_Y, test_dev->relbit);
	set_bit(REL_WHEEL, test_dev->relbit);

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

	LOG_D("register test device");
	return;

failed:
	input_free_device(test_dev);
	test_dev = RT_NULL;
}

/* unregister test device */
static void unregister_device()
{
	if (test_dev == RT_NULL) {
		LOG_D("test device is NULL, you should register first");
		return;
	}

	input_unregister_device(test_dev);
	input_free_device(test_dev);
	test_dev = RT_NULL;

	LOG_D("unregister test device");
}

static void test_notification_init()
{
	rt_err_t ret;

	LOG_D("test_notification_init");
	ret = notif_init();

	if (ret != 0)
		LOG_D("test_notification_init error");
}

static void test_notification_deinit()
{
	rt_err_t ret;

	LOG_D("test_notification_deinit");
	ret = notif_deinit();

	if (ret != 0)
		LOG_D("test_notification_deinit error");
}

static void test_evdev_init()
{
	rt_err_t ret;

	LOG_D("test_evdev_init");
	ret = evdev_init();

	if (ret != 0)
		LOG_D("test_evdev_init error");
}

static void test_evdev_deinit()
{
	rt_err_t ret;

	LOG_D("test_evdev_deinit");
	ret = evdev_deinit();

	if (ret != 0)
		LOG_D("test_evdev_deinit error");
}

long test_input(int argc, char **argv)
{
	LOG_D("test_input...");
	if (3 == argc) {
		char *cmd;

		cmd = argv[2];
		if (!strcmp(cmd, TEST_INPUT_CMD_MOUSE_EVENT))
			report_mouse();
		else if (!strcmp(cmd, TEST_INPUT_CMD_REGISTER))
			register_device();
		else if (!strcmp(cmd, TEST_INPUT_CMD_UNREGISTER))
			unregister_device();
		else if (!strcmp(cmd, TEST_INPUT_CMD_NT_INIT))
			test_notification_init();
		else if (!strcmp(cmd, TEST_INPUT_CMD_NT_DEINIT))
			test_notification_deinit();
		else if (!strcmp(cmd, TEST_INPUT_CMD_EVDEV_INIT))
			test_evdev_init();
		else if (!strcmp(cmd, TEST_INPUT_CMD_EVDEV_DEINIT))
			test_evdev_deinit();
		else
			LOG_D("invalid cmd\n");
	} else {
		/* to add some test case... */
		LOG_D("test_input success");
	}

	return 0;
}

