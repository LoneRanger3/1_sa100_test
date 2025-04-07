/*
 * egalax_exc3111_ts.c - Egalax exc3111 driver
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
#include <rtdevice.h>
#include <rthw.h>
#include <csp.h>

#include <debug.h>
#include "egalax_exc3111_ts.h"
#include "input.h"
#include "gpio/pinctrl.h"

#define EGALAX_TS_MODULE_NAME		"exc3111-ts"

#define EGALAX_TS_INT_PORT		GPIO_PORT_E
#define EGALAX_TS_PIN			GPIO_PIN_12

#define EGALAX_ADDR			0x2a
#define EGALAX_I2C_HOST			3

#define TS_MAX_I2C_LEN			64
#define MAX_SUPPORT_POINT		16

#define REPORTID_MOUSE			0x01
#define REPORTID_VENDOR			0x03
#define REPORTID_MTOUCH			0x06
#define MAX_RESOLUTION			4095

#define TS_EVENTS_PER_PACKET		5

/* mutiple touch info */
struct tag_mt_contacts {
	unsigned char ID;	/* touch ID, for mark mutil-touch*/
	signed char status;	/* touch status */
	unsigned short X;
	unsigned short Y;
};

static struct input_dev _ts_dev;
static struct rt_i2c_bus_device *i2c_ts_dev = RT_NULL;	/* i2c device for touch panel */

static struct tag_mt_contacts p_contact_buf[MAX_SUPPORT_POINT];	/* touch info */

/* to storage data that read from tp by i2c */
static u8 input_report_buf[TS_MAX_I2C_LEN + 2];

/* free the gpio interrupt of tp pin */
static void ts_free_gpio_irq()
{
	struct pinctrl *pctrl;
	int pin_num, ret;

	pctrl = pinctrl_get(EGALAX_TS_MODULE_NAME);
	if (pctrl == RT_NULL) {
		LOG_E("pctrl == RT_NULL");
		return;
	}

	pin_num = pinctrl_gpio_request(pctrl, EGALAX_TS_INT_PORT, EGALAX_TS_PIN);
	if (pin_num < 0) {
		LOG_E("pin_num < 0");
		return;
	}

	ret = pinctrl_gpio_free_irq(pctrl, pin_num);
	if (ret != RT_EOK) {
		LOG_E("pinctrl_gpio_free_irq != RT_EOK");
		return;
	}
}

/* touch panel interrupt handle function */
static void ts_gpio_irq_handler(void *data)
{
	ts_free_gpio_irq();
	input_send_int_event(INPUT_EVENT_TOUCH);
}

/* request gpio interrupt for tp pin */
static void ts_request_gpio_irq()
{
	struct pinctrl *pctrl;
	struct gpio_irq_data irq_data;
	int pin_num, ret;

	pctrl = pinctrl_get(EGALAX_TS_MODULE_NAME);
	if (pctrl == RT_NULL) {
		LOG_E("pctrl == RT_NULL");
		return;
	}

	pin_num = pinctrl_gpio_request(pctrl, EGALAX_TS_INT_PORT, EGALAX_TS_PIN);
	if (pin_num < 0) {
		LOG_E("pin_num < 0");
		return;
	}

	irq_data.handler = ts_gpio_irq_handler;		/* interrupt handle function */
	irq_data.irq_arg = NULL;
	irq_data.clock_src = GPIO_IRQ_HOSC_24MHZ;
	irq_data.clock_src_div = 10;
	irq_data.trig_type = EINT_TRIG_LOW_LEVEL;

	ret = pinctrl_gpio_request_irq(pctrl, pin_num, &irq_data);
	if (ret != RT_EOK) {
		LOG_E("pinctrl_gpio_request_irq != RT_EOK");
		return;
	}
}

/**
 * read_ts_i2c_data - read tp data from i2c
 * @buf: the data to store
 * @len: number of data to read
 *
 * return RT_EOK if success, RT_ERROR if failed
 */
static u32 read_ts_i2c_data(u8 *buf, u32 len)
{
	struct rt_i2c_msg msgs[1];
	u32 ret;

	if (i2c_ts_dev == RT_NULL)
		return RT_ERROR;

	msgs[0].addr  = EGALAX_ADDR;
	msgs[0].flags = RT_I2C_RD;
	msgs[0].buf   = buf;
	msgs[0].len   = len;

	ret = rt_i2c_transfer(i2c_ts_dev, msgs, 1);
	if (ret == 1)
		return RT_EOK;
	else {
		LOG_E("rt_i2c_transfer error");
		return RT_ERROR;
	}
}

/*
static void print_mt_contact(struct tag_mt_contacts *con)
{
	LOG_D("tag_mt_contacts-> ID: %d, Status: %d, X: %d, Y: %d",
		con->ID, con->status, con->X, con->Y);
}
*/

/* process touch info, copy from linux egalax driver */
#define MAX_POINT_PER_PACKET	5
#define POINT_STRUCT_SIZE	10
static int total_pts_cnt, recv_pts_cnt;

/**
 * process_report - process the data that read from i2c, and report the event
 * @buf: data to be processed
 */
static void process_report(u8 *buf)
{
	u8 i, index = 0, cnt_down = 0, cnt_up = 0, shift = 0;
	u8 status = 0;
	u16 contact_id = 0, x = 0, y = 0;

	if (total_pts_cnt <= 0) {
		if (buf[1] == 0 || buf[1] > MAX_SUPPORT_POINT) {
			LOG_E("NumsofContacts mismatch, skip packet");
			return;
		}

		total_pts_cnt = buf[1];
		recv_pts_cnt = 0;
	} else if (buf[1] > 0) {
		total_pts_cnt = recv_pts_cnt = 0;
		LOG_E("NumsofContacts mismatch, skip packet");
		return;
	}

	while (index < MAX_POINT_PER_PACKET) {
		shift = index * POINT_STRUCT_SIZE + 2;
		status = buf[shift] & 0x01;
		contact_id = buf[shift + 1];
		x = ((buf[shift + 3] << 8) + buf[shift + 2]);
		y = ((buf[shift + 5] << 8) + buf[shift + 4]);

		if (contact_id >= MAX_SUPPORT_POINT) {
			total_pts_cnt = recv_pts_cnt = 0;
			LOG_E("Get error ContactID");
			return;
		}

		p_contact_buf[recv_pts_cnt].ID = contact_id;
		p_contact_buf[recv_pts_cnt].status = status;
		p_contact_buf[recv_pts_cnt].X = x;
		p_contact_buf[recv_pts_cnt].Y = y;

		recv_pts_cnt++;
		index++;

		/* Recv all points, send input report */
		if (recv_pts_cnt == total_pts_cnt) {
			for (i = 0; i < recv_pts_cnt; i++) {
				/* todo, input event report */
				/* print_mt_contact(&(p_contact_buf[i])); */
				input_report_abs(&_ts_dev,
					ABS_MT_TRACKING_ID,
					p_contact_buf[i].ID);
				input_report_abs(&_ts_dev,
					ABS_MT_TOUCH_MAJOR,
					p_contact_buf[i].status);
				input_report_abs(&_ts_dev,
					ABS_MT_POSITION_X,
					p_contact_buf[i].X);
				input_report_abs(&_ts_dev,
					ABS_MT_POSITION_Y,
					p_contact_buf[i].Y);
				input_mt_sync(&_ts_dev);

				if (p_contact_buf[i].status)
					cnt_down++;
				else
					cnt_up++;
			}

			total_pts_cnt = recv_pts_cnt = 0;
			return;
		}
	}
}

static void egalax_i2c_measure()
{
	u32 ret;

	if (i2c_ts_dev == NULL) {
		LOG_E("i2c_ts_dev == NULL");
		return;
	}

	ret = read_ts_i2c_data(input_report_buf, TS_MAX_I2C_LEN + 2);
	if (ret != RT_EOK) {
		LOG_E("read_ts_i2c_data != RT_EOK");
		return;
	}

	ret = 0;
	switch (input_report_buf[2]) {
	case REPORTID_MTOUCH:
		process_report(input_report_buf + 2);
		break;
	case REPORTID_VENDOR:
		/* todo something */
		break;
	default:
		break;
	}
}

/* process data about tp ic2 */
static void egalax_handle_int()
{
	/* read touch panel data from i2c and measure */
	egalax_i2c_measure();
	ts_request_gpio_irq();
}

/**
 * write_ts_i2c_data - write data to tp by i2c
 * @buf: the data to write
 * @len: number of data to write
 *
 * return RT_EOK if success, RT_ERROR if failed
 */
static u32 write_ts_i2c_data(u8 *buf, u32 len)
{
	struct rt_i2c_msg msgs;

	if (i2c_ts_dev == RT_NULL)
		return RT_ERROR;

	msgs.addr  = EGALAX_ADDR;
	msgs.flags = RT_I2C_WR;
	msgs.buf   = buf;
	msgs.len   = len;

	if (rt_i2c_transfer(i2c_ts_dev, &msgs, 1) == 1)
		return RT_EOK;

	return RT_ERROR;
}

/* wake touch screen up */
int ts_send_wakeup_cmd()
{
	u8 cmdbuf[4];
	u32 ret;

	cmdbuf[0] = 0xA7;
	cmdbuf[1] = 0x00;
	cmdbuf[2] = 0x00;
	cmdbuf[3] = 0x08;

	ret = write_ts_i2c_data(cmdbuf, 4);
	if (ret != RT_EOK)
		LOG_E("write_ts_i2c_data != RT_EOK");

	return ret;
}

/* make touch screen sleep */
int ts_send_sleep_cmd()
{
	u8 cmdbuf[4];
	u32 ret;

	cmdbuf[0] = 0xA7;
	cmdbuf[1] = 0x00;
	cmdbuf[2] = 0x01;
	cmdbuf[3] = 0x08;

	ret = write_ts_i2c_data(cmdbuf, 4);
	if (ret != RT_EOK)
		LOG_E("write_ts_i2c_data != RT_EOK");

	return ret;
}

/* initialize touch panel i2c device */
static void init_ts_i2c()
{
	char i2c_name[6] = {0};
	rt_sprintf(i2c_name, "%s%d", "i2c", EGALAX_I2C_HOST);

	i2c_ts_dev = rt_i2c_bus_device_find(i2c_name);
	if (i2c_ts_dev == RT_NULL)
		LOG_E("can't find bus dev \"%s\"", i2c_name);
}

/* initialize touch panel gpio */
static void init_ts_gpio()
{
	struct pinctrl *pctrl;
	int pin_num, ret;

	pctrl = pinctrl_get(EGALAX_TS_MODULE_NAME);
	if (pctrl == RT_NULL) {
		LOG_E("pctrl == RT_NULL");
		return;
	}

	pin_num = pinctrl_gpio_request(pctrl, EGALAX_TS_INT_PORT, EGALAX_TS_PIN);
	if (pin_num < 0) {
		LOG_E("pin_num < 0");
		return;
	}

	/* set pull up mode */
	ret = pinctrl_gpio_set_pud_mode(pctrl, pin_num, PULL_UP);
	if (ret != RT_EOK) {
		LOG_E("pinctrl_gpio_set_pud_mode != RT_EOK");
		return;
	}

	ret = pinctrl_gpio_direction_output(pctrl, pin_num);
	if (ret != RT_EOK) {
		LOG_E("pinctrl_gpio_direction_output != RT_EOK");
		return;
	}

	ret = pinctrl_gpio_direction_input(pctrl, pin_num);
	if (ret != RT_EOK) {
		LOG_E("pinctrl_gpio_direction_input != RT_EOK");
		return;
	}

	ts_request_gpio_irq();	/* gpio interrupt */
}

static void register_ts_device()
{
	rt_err_t ret;

	_ts_dev.name = "egalax_device";
	rt_list_init(&(_ts_dev.node));
	rt_list_init(&(_ts_dev.h_list));

	/* set the device generate event and key type */
	set_bit(EV_ABS, _ts_dev.evbit);
	set_bit(ABS_MT_TOUCH_MAJOR, _ts_dev.absbit);
	set_bit(ABS_MT_POSITION_X, _ts_dev.absbit);
	set_bit(ABS_MT_POSITION_Y, _ts_dev.absbit);
	set_bit(ABS_MT_TRACKING_ID, _ts_dev.absbit);

	_ts_dev.num_vals = 0;
	_ts_dev.max_vals = TS_EVENTS_PER_PACKET + 2;
	_ts_dev.vals = rt_malloc(sizeof(struct input_value) * _ts_dev.max_vals);
	if (_ts_dev.vals == RT_NULL) {
		LOG_E("rt_malloc for input_dev vals error");
		return;
	}

	ret = input_register_device(&_ts_dev);
	if (ret != RT_EOK)
		LOG_E("input_register_device error");
}

static struct input_int_handler ts_int_handler = {
	.type		= INPUT_EVENT_TOUCH,
	.int_handle	= egalax_handle_int,
};

/* initialize egalax touch panel driver */
int egalax_ts_init()
{
	init_ts_i2c();	/*init i2c for touch screen transfer */
	if (i2c_ts_dev) {
		init_ts_gpio();		/* setup gpio for touch interrupt */
		register_ts_device();	/* register touch screen device to input core */

		rt_list_init(&ts_int_handler.node);
		input_register_int_handler(&ts_int_handler);

		LOG_I("egalax_ts_init finished");
	} else
		LOG_E("egalax_ts_init error: i2c_ts_dev == NULL");

	return 0;
}

/* evb use egalax touch panel */
#ifdef ARCH_LOMBO_N7V0_EVB
INIT_DEVICE_EXPORT(egalax_ts_init);
#endif

