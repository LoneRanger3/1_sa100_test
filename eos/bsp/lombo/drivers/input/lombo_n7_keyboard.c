/*
 * lombo_n7_keyboard.c - Keyboard module realization
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


#include "gpadc_drv.h"
#include <irq_numbers.h>
#include "input.h"
#include <debug.h>
#include "stdlib.h"

#define KEY_SW_5_L				1100
#define KEY_SW_5_H				1700

#define KEY_SW_6_L				1700
#define KEY_SW_6_H				2600

#define KEY_SW_7_L				400
#define KEY_SW_7_H				700

#define KEY_SW_8_L				10
#define KEY_SW_8_H				400

#define KEY_SW_9_L				700
#define KEY_SW_9_H				1100

#define KEY_SW_10_L				0
#define KEY_SW_10_H				10


#define KB_EVENTS_PER_PACKET			2
#define KB_TYPE_NUM				6

#define KB_GPADC_AUX_INDEX			0
#define AUXIN_T_VALUE				4000
#define LONG_KEY_FILTER	8
static int key_count;
static int last_key_value;
struct rt_timer kb_timer;
int timer_tick = 6;

struct kb_key_type {
	u32 low;
	u32 high;
	u32 type;
	u32 state;
};

static struct kb_key_type key_types[KB_TYPE_NUM] = {
	{
		.low	= KEY_SW_5_L,
		.high	= KEY_SW_5_H,
		.type	= KEY_ESC,
		.state  = 0,
	},
	{
		.low	= KEY_SW_6_L,
		.high	= KEY_SW_6_H,
		.type	= KEY_M,
		.state  = 0,
	},
	{
		.low	= KEY_SW_7_L,
		.high	= KEY_SW_7_H,
		.type	= KEY_UP,
		.state  = 0,
	},
	{
		.low	= KEY_SW_8_L,
		.high	= KEY_SW_8_H,
		.type	= KEY_MENU,
		.state  = 0,
	},
	{
		.low	= KEY_SW_9_L,
		.high	= KEY_SW_9_H,
		.type	= KEY_DOWN,
		.state  = 0,
	},
	{
		.low	= KEY_SW_10_L,
		.high	= KEY_SW_10_H,
		.type	= KEY_POWER,
		.state  = 0,
	},
};

static struct input_dev _kb_dev;

/* Keyboard interrupt handle function */
void lombo_keyboard_irq(int vector, void *param)
{
	csp_aux_set_th_en(KB_GPADC_AUX_INDEX, RT_FALSE);

	/* send keyboard message */
	input_send_int_event(INPUT_EVENT_KB);
}

static void kb_handle_int()
{
	int i;
	u32 aux_is, key_val;
	struct kb_key_type t;

	aux_is = csp_aux_get_th_pend(KB_GPADC_AUX_INDEX);
	if (aux_is) {
		key_val = csp_aux_get_sh_data(KB_GPADC_AUX_INDEX);
		/* rt_kprintf("kb_proc_thread key_val1 = %d\n", key_val); */
		rt_timer_stop(&kb_timer);
		rt_timer_control(&kb_timer, RT_TIMER_CTRL_SET_TIME, &timer_tick);
		rt_timer_start(&kb_timer);
		for (i = 0; i < KB_TYPE_NUM; i++) {
			t = key_types[i];
			if ((key_val >= t.low) && (key_val < t.high)) {
				if (key_types[i].state == 0) {
					key_count = 0;
					key_types[i].state = 1;
				} else if (key_types[i].state == 1) {
					key_count++;
					if (key_count == 2) {/* filter error data */
						input_report_key(&_kb_dev, t.type,
								KEY_PRESS);
						input_sync(&_kb_dev);
						last_key_value = t.type;

					}
					if (key_count > LONG_KEY_FILTER) {
						key_types[i].state = 3;
						input_report_key(&_kb_dev, t.type,
								KEY_LONG_PRESS);
						input_sync(&_kb_dev);
						last_key_value = t.type;
					}

				}
			}
		}
	}
	csp_aux_clr_th_pend(KB_GPADC_AUX_INDEX);
	csp_aux_set_th_en(KB_GPADC_AUX_INDEX, RT_TRUE);
}

static void vkb_handle_int(int argc, char **argv)
{
	int i;
	u32 key_val = 0xFFFFFFFF;
	struct kb_key_type t;
	char value[5] = {0}, key;
	int state;
	static int vkb_key_count;

	if (argc != 2)
		return;

	if (argv[1][0] == '@') {
		if (argv[1][1] == '3')
			state = 1;
		else if (argv[1][1] == '4')
			state = 0;
		else {
			vkb_key_count = 0;
			last_key_value = 0;
			return;
		}
		value[0] = argv[1][2];
		value[1] = argv[1][3];
		value[2] = argv[1][4];
		value[3] = argv[1][5];
		value[4] = '\0';
		key = atoi((const char *)value);
		for (i = 0; i < KB_TYPE_NUM; i++) {
			if ((key == 'z' || key == 'Z') && i == 0)
				key_val = key_types[i].low;
			else if ((key == 'x' || key == 'X') && i == 1)
				key_val = key_types[i].low;
			else if ((key == 'c' || key == 'C') && i == 2)
				key_val = key_types[i].low;
			else if ((key == 'v' || key == 'V') && i == 3)
				key_val = key_types[i].low;
			else if ((key == 'b' || key == 'B') && i == 4)
				key_val = key_types[i].low;
			else if ((key == 'n' || key == 'N') && i == 5)
				key_val = key_types[i].low;
			else if ((key == 'm' || key == 'M') && i == 6)
				key_val = key_types[i].low;
		}
	} else
		return;

	if (key_val == 0xFFFFFFFF)
		return;

	rt_kprintf("kb_proc_thread key_val1 = %d\n", key_val);
	for (i = 0; i < KB_TYPE_NUM; i++) {
		t = key_types[i];
		if ((key_val >= t.low) && (key_val < t.high)) {
			if (state == 1) {
				if (vkb_key_count == 0) {
					input_report_key(&_kb_dev, t.type,
							KEY_PRESS);
					input_sync(&_kb_dev);
					last_key_value = t.type;
				} else {
					input_report_key(&_kb_dev, t.type,
							KEY_LONG_PRESS);
					input_sync(&_kb_dev);
					last_key_value = t.type;

				}
				vkb_key_count++;
			} else {
				input_report_key(&_kb_dev, last_key_value, KEY_RELEASE);
				input_sync(&_kb_dev);
				vkb_key_count = 0;
				last_key_value = 0;
			}
		}
	}
}

static void kb_time_handler(void *data)
{
	int i;

	if (last_key_value) {
		key_count = 0;
		for (i = 0; i < KB_TYPE_NUM; i++)
			key_types[i].state = 0;
		input_report_key(&_kb_dev, last_key_value, KEY_RELEASE);
		input_sync(&_kb_dev);
		last_key_value = 0;
	}
}

static void register_kb_device()
{
	rt_err_t ret;

	_kb_dev.name = "n7_keyboard";
	rt_list_init(&(_kb_dev.node));
	rt_list_init(&(_kb_dev.h_list));

	/* set the device generate event and key type */
	set_bit(EV_KEY, _kb_dev.evbit);
	set_bit(KEY_ESC, _kb_dev.keybit);
	set_bit(KEY_M, _kb_dev.keybit);
	set_bit(KEY_UP, _kb_dev.keybit);
	set_bit(KEY_MENU, _kb_dev.keybit);
	set_bit(KEY_DOWN, _kb_dev.keybit);
	set_bit(KEY_POWER, _kb_dev.keybit);

	_kb_dev.num_vals = 0;
	_kb_dev.max_vals = KB_EVENTS_PER_PACKET + 2;
	_kb_dev.vals = rt_malloc(sizeof(struct input_value) * _kb_dev.max_vals);
	if (_kb_dev.vals == RT_NULL) {
		LOG_E("rt_malloc for input_dev vals error");
		return;
	}
	rt_timer_init(&kb_timer, "kb", kb_time_handler, NULL, timer_tick,
		RT_TIMER_FLAG_DEACTIVATED | RT_TIMER_FLAG_PERIODIC |
		RT_TIMER_FLAG_HARD_TIMER);
	ret = input_register_device(&_kb_dev);
	if (ret != RT_EOK)
		LOG_E("input_register_device error");
	rt_timer_start(&kb_timer);

}

static void kb_gpadc_cfg()
{
	csp_aux_set_en(KB_GPADC_AUX_INDEX, RT_TRUE);
	/* set AUXIN threshold interrupt enable */
	csp_aux_set_th_en(KB_GPADC_AUX_INDEX, RT_TRUE);
	/* set AUXIN threshold value */
	csp_aux_set_th_data(KB_GPADC_AUX_INDEX, AUXIN_T_VALUE);

	rt_hw_interrupt_install(INT_GPADC, lombo_keyboard_irq,
				RT_NULL, "gpadc_irq");
	rt_hw_interrupt_umask(INT_GPADC);
}

static struct input_int_handler kb_int_handler = {
	.type		= INPUT_EVENT_KB,
	.int_handle	= kb_handle_int,
};

int n7_keyboard_init()
{
	rt_err_t ret;

	register_kb_device();	/* register keyboard device to input core */

	kb_gpadc_cfg();	/* use gpadc auxin for keyboard */

	rt_list_init(&kb_int_handler.node);
	ret = input_register_int_handler(&kb_int_handler);
	if (ret != RT_EOK)
		LOG_E("input_register_int_handler kb_int_handler error");

	LOG_I("n7_keyboard_init finished");
	return 0;
}

#ifdef RT_USING_FINSH
#include <finsh.h>
FINSH_FUNCTION_EXPORT(vkb_handle_int, FINSH vmouse and vkey input);
MSH_CMD_EXPORT(vkb_handle_int, MSH vmouse and vkey input);
#endif
INIT_COMPONENT_EXPORT(n7_keyboard_init);
