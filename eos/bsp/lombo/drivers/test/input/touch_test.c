/*
 * touch_test.c - Touch screen test driver
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

#ifdef ARCH_LOMBO_N7V0_CDR
#include "gt913_ts.h"
#endif

#ifdef ARCH_LOMBO_N7V0_EVB
#include "egalax_exc3111_ts.h"
#endif

#ifdef ARCH_LOMBO_N7V1_EVB
#include "focaltech_ts.h"
#endif

#define TEST_TS_CMD_WAKEUP	"wakeup"
#define TEST_TS_CMD_SLEEP	"sleep"
#define TEST_TS_CMD_ADD		"add"
#define TEST_TS_CMD_REMOVE	"remove"
#define TEST_TS_CMD_REGISTER	"register"
#define TEST_TS_CMD_UNREGISTER	"unregister"
#define TEST_TS_CMD_EVENT	"event"

static struct notif_listener *test_listener;
static struct input_dev *test_dev;

void ts_rec_msg(u32 msg_t, void *msg_addr)
{
	switch (msg_t) {
	case NT_MSG_TOUCH:
	{
		struct touch_event *e;
		e = (struct touch_event *)msg_addr;
		LOG_D("test touch event -> touch_id: %d, status: %d, x: %d, y: %d",
			e->touch_id, e->status, e->x, e->y);
	}
		break;

	default:
		break;
	}
}

static void test_wakeup_cmd()
{
	rt_err_t ret;
	int(*wakeup_func)();

	wakeup_func = RT_NULL;
#ifdef ARCH_LOMBO_N7V0_CDR
	wakeup_func = gt913_wakeup;
#endif

#ifdef ARCH_LOMBO_N7V0_EVB
	wakeup_func = ts_send_wakeup_cmd;
#endif

#ifdef ARCH_LOMBO_N7V1_EVB
	wakeup_func = ft_wakeup;
#endif

	if (wakeup_func == RT_NULL) {
		LOG_D("tp wakeup function is NULL");
		return;
	}

	ret = wakeup_func();
	if (ret == RT_EOK)
		LOG_D("screen wakeup, now you can touch and see the info log");
	else
		LOG_D("test_wakeup_cmd error");
}

static void test_sleep_cmd()
{
	rt_err_t ret;
	int(*sleep_func)();

	sleep_func = RT_NULL;
#ifdef ARCH_LOMBO_N7V0_CDR
	sleep_func = gt913_sleep;
#endif

#ifdef ARCH_LOMBO_N7V0_EVB
	sleep_func = ts_send_sleep_cmd;
#endif

#ifdef ARCH_LOMBO_N7V1_EVB
	sleep_func = ft_sleep;
#endif

	if (sleep_func == RT_NULL) {
		LOG_D("tp sleep function is NULL");
		return;
	}

	ret = sleep_func();
	if (ret == RT_EOK)
		LOG_D("screen sleep, now you can't see any log when touch the tp");
	else
		LOG_D("test_sleep_cmd error");
}

static void test_add_cmd()
{
	if (test_listener)
		LOG_D("You have already add the touch listener");
	else {
		rt_err_t ret;

		test_listener = rt_malloc(sizeof(struct notif_listener));
		if (test_listener == RT_NULL) {
			LOG_D("rt_malloc notif_listener error");
			return;
		}

		test_listener->msgbit = NT_MSG_TOUCH;
		test_listener->rec_msg = ts_rec_msg;
		rt_list_init(&(test_listener->node));

		ret = notif_register_listener(test_listener);
		if (ret == RT_EOK)
			LOG_D("Add touch event listener success, ",
				"now you can touch and see log");
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

/* register touch screen device to input subsystem */
static void test_tsdev_register()
{
	rt_err_t ret;
	if (test_dev) {
		LOG_D("The test touch screen device have already register");
		return;
	}

	test_dev = input_allocate_device();
	if (test_dev == RT_NULL) {
		LOG_D("input_allocate_device error");
		return;
	}

	test_dev->name = "test_ts";
	set_bit(EV_ABS, test_dev->evbit);

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

	LOG_D("register test touch screen device");
	return;

failed:
	input_free_device(test_dev);
	test_dev = RT_NULL;
}

/* unregister touch screen device from input subsystem */
static void test_tsdev_unregister()
{
	if (test_dev == RT_NULL) {
		LOG_D("test touch screen is NULL, you should register first");
		return;
	}

	input_unregister_device(test_dev);
	input_free_device(test_dev);
	test_dev = RT_NULL;

	LOG_D("unregister test touch screen device");
}

/* simulate report touch screen event */
static void test_tsdev_event()
{
	if (test_dev) {
		LOG_D("report_touch");
		int i = 0;
		for (i = 0; i < 8; i++) {
			input_report_abs(test_dev, ABS_MT_TRACKING_ID, 3);
			input_report_abs(test_dev, ABS_MT_TOUCH_MAJOR, 0);
			input_report_abs(test_dev, ABS_MT_POSITION_X, 666);
			input_report_abs(test_dev, ABS_MT_POSITION_Y, 888);
			input_mt_sync(test_dev);

			rt_thread_mdelay(50);
		}
	} else
		LOG_D("you should register touch screen device first");
}


long test_touch_screen(int argc, char **argv)
{
	LOG_D("test_touch_screen...");
	if (3 == argc) {
		/* touch screen test cmd */
		char *cmd;

		cmd = argv[2];
		if (!strcmp(cmd, TEST_TS_CMD_WAKEUP))
			test_wakeup_cmd();
		else if (!strcmp(cmd, TEST_TS_CMD_SLEEP))
			test_sleep_cmd();
		else if (!strcmp(cmd, TEST_TS_CMD_ADD))
			test_add_cmd();
		else if (!strcmp(cmd, TEST_TS_CMD_REMOVE))
			test_remove_cmd();
		else if (!strcmp(cmd, TEST_TS_CMD_REGISTER))
			test_tsdev_register();
		else if (!strcmp(cmd, TEST_TS_CMD_UNREGISTER))
			test_tsdev_unregister();
		else if (!strcmp(cmd, TEST_TS_CMD_EVENT))
			test_tsdev_event();
		else
			LOG_D("invalid cmd\n");
	} else {
		/* to add some test case... */
		LOG_D("test_touch_screen success");
	}

	return 0;
}

