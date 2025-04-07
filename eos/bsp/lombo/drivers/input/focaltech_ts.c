/*
 * focaltech_ts.c - Focal tech touch panel driver
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
#include <debug.h>
#include "input.h"
#include "gpio/pinctrl.h"
#include "focaltech_ts.h"
#include "bitops.h"

#define FOCALTECH_TOUCH_DEBUG		1

#define FT_TS_MODULE_NAME		"focaltech-ts"
#define FT_TS_INT_PORT			GPIO_PORT_E
#define FT_TS_INT_PIN			GPIO_PIN_12
#define FT_TS_RESET_PORT		GPIO_PORT_B
#define FT_TS_RESET_PIN			GPIO_PIN_23

#define FT_I2C_HOST			3
#define TS_EVENTS_PER_PACKET		5

/* I2C address */
#define FT_I2C_ADDR			0x38

/* Focaltech register */
#define WORK_MODE			0x00
#define PROXIMITY_STATUS		0x01
#define POWER_MODE			0xA5
#define FW_VERSION			0xA6
#define FTS_STATE			0xB0
#define MAX_X_HIGH			0x98
#define MAX_X_LOW			0x99
#define MAX_Y_HIGH			0x9A
#define MAX_Y_LOW			0x9B

#define CFG_MAX_TOUCH_POINTS		5
#define FTS_MAX_ID			0x0F
#define FTS_TOUCH_STEP			6
#define FTS_TOUCH_X_H_POS		3
#define FTS_TOUCH_X_L_POS		4
#define FTS_TOUCH_Y_H_POS		5
#define FTS_TOUCH_Y_L_POS		6
#define FTS_TOUCH_XY_POS		7
#define FTS_TOUCH_MISC			8
#define FTS_TOUCH_EVENT_POS		3
#define FTS_TOUCH_ID_POS		5

#define POINT_READ_BUF			(3 + FTS_TOUCH_STEP * CFG_MAX_TOUCH_POINTS)
#define FT_TOUCH_POINT_NUM		2

/* focaltech touch panel power consume mode */
enum FT_POWER_MODE {
	FT_ACTIVE = 0,
	FT_MONITOR,
	FT_HIBERNATE	/* sleep mode */
};

/*touch event info*/
struct ts_event {
	u16	au16_x[CFG_MAX_TOUCH_POINTS];		/* x coordinate */
	u16	au16_y[CFG_MAX_TOUCH_POINTS];		/* y coordinate */

	/* touch event: 0 -- down; 1-- up; 2 -- contact */
	u8	au8_touch_event[CFG_MAX_TOUCH_POINTS];
	u8	au8_finger_id[CFG_MAX_TOUCH_POINTS];	/* touch ID */
	u16	pressure[CFG_MAX_TOUCH_POINTS];
	u16	area[CFG_MAX_TOUCH_POINTS];
	u8	touch_point;
	int	touchs;
	u8	touch_point_num;
};

static struct input_dev _ts_dev;	/* for report touch event */
static struct rt_i2c_bus_device *i2c_ft_dev = RT_NULL;

/**
 * ft_i2c_read - read data from focaltech tp register by i2c
 * @reg: register address of gt913 to read
 * @buf: the data to store
 * @len: the length of data
 *
 * return RT_EOK if success, RT_ERROR if failed
 */
static rt_err_t ft_i2c_read(u8 reg, u8 *buf, u32 len)
{
	struct rt_i2c_msg msgs[2];

	if (i2c_ft_dev == RT_NULL) {
		LOG_E("Can't find focaltech touch panel I2C device");
		return -RT_ERROR;
	}

	msgs[0].addr  = FT_I2C_ADDR;
	msgs[0].flags = RT_I2C_WR;
	msgs[0].buf   = &reg;
	msgs[0].len   = 1;

	msgs[1].addr  = FT_I2C_ADDR;
	msgs[1].flags = RT_I2C_RD;
	msgs[1].buf   = buf;
	msgs[1].len   = len;

	if (rt_i2c_transfer(i2c_ft_dev, msgs, 2) != 2) {
		LOG_E("rt_i2c_transfer error");
		return -RT_ERROR;
	}

	return RT_EOK;
}

/**
 * ft_i2c_write - write data to focaltech tp register by i2c
 * @reg: register address of focaltech to write
 * @buf: the data to write
 * @len: the length of data
 *
 * return RT_EOK if success, RT_ERROR if failed
 */
static rt_err_t ft_i2c_write(u8 reg, u8 *buf, u32 len)
{
	struct rt_i2c_msg m;
	u8 msg_buf[len + 1];

	msg_buf[0] = reg;
	if (len > 0)
		rt_memcpy(msg_buf + 1, buf, len);

	m.addr  = FT_I2C_ADDR;
	m.flags = RT_I2C_WR;
	m.buf   = msg_buf;
	m.len   = len + 1;

	if (rt_i2c_transfer(i2c_ft_dev, &m, 1) != 1) {
		LOG_E("rt_i2c_transfer error");
		return -RT_ERROR;
	}

	return RT_EOK;
}

/* initialize touch panel i2c device */
static rt_err_t setup_i2c_device()
{
	char i2c_name[6] = {0};
	rt_sprintf(i2c_name, "%s%d", "i2c", FT_I2C_HOST);

	i2c_ft_dev = rt_i2c_bus_device_find(i2c_name);
	if (i2c_ft_dev == RT_NULL) {
		LOG_E("can't find bus dev \"%s\"", i2c_name);
		return -RT_ERROR;
	}

	return RT_EOK;
}

/* free the gpio interrupt of tp pin */
static void ts_free_gpio_irq()
{
	struct pinctrl *pctrl;
	int pin_num, ret;

	pctrl = pinctrl_get(FT_TS_MODULE_NAME);
	if (pctrl == RT_NULL) {
		LOG_E("pctrl == RT_NULL");
		return;
	}

	pin_num = pinctrl_gpio_request(pctrl, FT_TS_INT_PORT, FT_TS_INT_PIN);
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

/* request gpio interrupt for tp INT pin */
static rt_err_t ts_request_gpio_irq()
{
	struct pinctrl *pctrl;
	struct gpio_irq_data irq_data;
	int pin_num, ret;

	pctrl = pinctrl_get(FT_TS_MODULE_NAME);
	if (pctrl == RT_NULL) {
		LOG_E("pctrl == RT_NULL");
		return -RT_ERROR;
	}

	pin_num = pinctrl_gpio_request(pctrl, FT_TS_INT_PORT, FT_TS_INT_PIN);
	if (pin_num < 0) {
		LOG_E("pin_num < 0");
		return -RT_ERROR;
	}

	irq_data.handler = ts_gpio_irq_handler;		/* interrupt handle function */
	irq_data.irq_arg = NULL;
	irq_data.clock_src = GPIO_IRQ_HOSC_24MHZ;
	irq_data.clock_src_div = 10;
	irq_data.trig_type = EINT_TRIG_LOW_LEVEL;

	ret = pinctrl_gpio_request_irq(pctrl, pin_num, &irq_data);
	if (ret != RT_EOK) {
		LOG_E("pinctrl_gpio_request_irq != RT_EOK");
		return ret;
	}

	return ret;
}

/* set gpio pin direction output and level */
static rt_err_t ft_set_gpio_output(enum gpio_port port, enum gpio_pin pin, int value)
{
	struct pinctrl *pctrl;
	int pin_num;
	rt_err_t ret;

	RT_ASSERT((value == 0) || (value == 1));

	pctrl = pinctrl_get(FT_TS_MODULE_NAME);
	if (pctrl == RT_NULL) {
		LOG_E("pinctrl_get return RT_NULL");
		return -RT_ERROR;
	}

	pin_num = pinctrl_gpio_request(pctrl, port, pin);
	if (pin_num < 0) {
		LOG_E("pin_num < 0");
		return -RT_ERROR;
	}

	ret = pinctrl_gpio_direction_output(pctrl, pin_num);
	if (ret != RT_EOK) {
		LOG_E("pinctrl_gpio_direction_output != RT_EOK");
		return ret;
	}

	ret = pinctrl_gpio_set_value(pctrl, pin_num, value);
	if (ret != RT_EOK) {
		LOG_E("pinctrl_gpio_set_value != RT_EOK");
		return ret;
	}

	return RT_EOK;
}

/* set tp RESET pin direction output and high level */
static void FT_RESET_H()
{
	ft_set_gpio_output(FT_TS_RESET_PORT, FT_TS_RESET_PIN, 1);
}

/* set tp RESET pin direction output and low level */
static void FT_RESET_L()
{
	ft_set_gpio_output(FT_TS_RESET_PORT, FT_TS_RESET_PIN, 0);
}

/* initialize touch panel gpio */
static rt_err_t init_ts_gpio()
{
	struct pinctrl *pctrl;
	int pin_num, ret;

	pctrl = pinctrl_get(FT_TS_MODULE_NAME);
	if (pctrl == RT_NULL) {
		LOG_E("pctrl == RT_NULL");
		return -RT_ERROR;
	}

	pin_num = pinctrl_gpio_request(pctrl, FT_TS_INT_PORT, FT_TS_INT_PIN);
	if (pin_num < 0) {
		LOG_E("pin_num < 0");
		return -RT_ERROR;
	}

	/* set pull up mode */
	ret = pinctrl_gpio_set_pud_mode(pctrl, pin_num, PULL_UP);
	if (ret != RT_EOK) {
		LOG_E("pinctrl_gpio_set_pud_mode != RT_EOK");
		return ret;
	}

	ret = pinctrl_gpio_direction_output(pctrl, pin_num);
	if (ret != RT_EOK) {
		LOG_E("pinctrl_gpio_direction_output != RT_EOK");
		return ret;
	}

	ret = pinctrl_gpio_direction_input(pctrl, pin_num);
	if (ret != RT_EOK) {
		LOG_E("pinctrl_gpio_direction_input != RT_EOK");
		return ret;
	}

	ret = ts_request_gpio_irq();	/* gpio interrupt */
	return ret;
}

/* read touch data by i2c and process it */
static rt_err_t ft_read_touch_data(struct ts_event *data)
{
	u8 buf[POINT_READ_BUF] = {0}; /* 0xFF */
	int ret = -1;
	int i = 0;
	u8 pointid = FTS_MAX_ID;

	ret = ft_i2c_read(buf[0], buf, POINT_READ_BUF);
	if (ret != RT_EOK) {
		LOG_E("ft_i2c_read error: %d", ret);
		return ret;
	}

	memset(data, 0, sizeof(struct ts_event));
	data->touch_point = 0;
	data->touch_point_num = buf[FT_TOUCH_POINT_NUM] & 0x0F;

	for (i = 0; i < CFG_MAX_TOUCH_POINTS; i++) {
		pointid = (buf[FTS_TOUCH_ID_POS + FTS_TOUCH_STEP * i]) >> 4;
		if (pointid >= FTS_MAX_ID)
			break;
		else
			data->touch_point++;
		data->au16_x[i] =
		    (s16) (buf[FTS_TOUCH_X_H_POS + FTS_TOUCH_STEP * i] & 0x0F) <<
		    8 | (s16) buf[FTS_TOUCH_X_L_POS + FTS_TOUCH_STEP * i];
		data->au16_y[i] =
		    (s16) (buf[FTS_TOUCH_Y_H_POS + FTS_TOUCH_STEP * i] & 0x0F) <<
		    8 | (s16) buf[FTS_TOUCH_Y_L_POS + FTS_TOUCH_STEP * i];
		data->au8_touch_event[i] =
		    buf[FTS_TOUCH_EVENT_POS + FTS_TOUCH_STEP * i] >> 6;
		data->au8_finger_id[i] =
		    (buf[FTS_TOUCH_ID_POS + FTS_TOUCH_STEP * i]) >> 4;

		/* cannot constant value */
		data->pressure[i] =
			(buf[FTS_TOUCH_XY_POS + FTS_TOUCH_STEP * i]);
		data->area[i] =
			(buf[FTS_TOUCH_MISC + FTS_TOUCH_STEP * i]) >> 4;

		if ((data->au8_touch_event[i] == 0 || data->au8_touch_event[i] == 2) &&
			(data->touch_point_num == 0))
			return 1;
	}

	return 0;
}

/* report touch event */
static int ft_report_value(struct ts_event *data)
{
	int i = 0;
	int up_point = 0;
	int touchs = 0;

	for (i = 0; i < data->touch_point; i++) {
		if (data->au8_touch_event[i] == 0 || data->au8_touch_event[i] == 2) {
			/* swap x and y when touch down */
			int tmp = data->au16_x[i];
			data->au16_x[i] = data->au16_y[i];
			data->au16_y[i] = tmp;

			/* report touch down event */
			input_report_abs(&_ts_dev,
				ABS_MT_TRACKING_ID,
				data->au8_finger_id[i]);
			input_report_abs(&_ts_dev,
				ABS_MT_TOUCH_MAJOR, 1);
			input_report_abs(&_ts_dev,
				ABS_MT_POSITION_X,
				data->au16_x[i]);
			input_report_abs(&_ts_dev,
				ABS_MT_POSITION_Y,
				data->au16_y[i]);
			input_mt_sync(&_ts_dev);

			touchs |= BIT(data->au8_finger_id[i]);
			data->touchs |= BIT(data->au8_finger_id[i]);
#if FOCALTECH_TOUCH_DEBUG
			LOG_D("tp down (id = %d, x = %d, y = %d, pres = %d, area = %d)",
				data->au8_finger_id[i], data->au16_x[i],
				data->au16_y[i], data->pressure[i], data->area[i]);
#endif
		} else {
#if FOCALTECH_TOUCH_DEBUG
			LOG_D("tp up (id = %d, x = %d, y = %d, pres = %d, area = %d)",
				data->au8_finger_id[i], data->au16_x[i],
				data->au16_y[i], data->pressure[i], data->area[i]);
#endif
			/* report touch release event */
			input_report_abs(&_ts_dev,
				ABS_MT_TRACKING_ID,
				data->au8_finger_id[i]);
			input_report_abs(&_ts_dev,
				ABS_MT_TOUCH_MAJOR, 0);
			input_report_abs(&_ts_dev,
				ABS_MT_POSITION_X,
				data->au16_x[i]);
			input_report_abs(&_ts_dev,
				ABS_MT_POSITION_Y,
				data->au16_y[i]);
			input_mt_sync(&_ts_dev);

			up_point++;
			data->touchs &= ~BIT(data->au8_finger_id[i]);
		}
	}

	for (i = 0; i < CFG_MAX_TOUCH_POINTS; i++) {
		if (BIT(i) & (data->touchs ^ touchs)) {
#if FOCALTECH_TOUCH_DEBUG
			LOG_D("tp normal_up id = %d", i);
#endif
			data->touchs &= ~BIT(i);
		}
	}
	data->touchs = touchs;

	/*  release all touches in final */
	if (data->touch_point_num == 0) {
		data->touchs = 0;
#if FOCALTECH_TOUCH_DEBUG
		LOG_D("release all touches in final: touch_point_num == 0");
#endif
	}

	return 0;
}

/* process data about tp ic2 */
static void ft_handle_int()
{
	struct ts_event pevent;
	rt_err_t ret;

	/* read touch panel data from i2c */
	ret = ft_read_touch_data(&pevent);
	if (ret == RT_EOK) {
		ret = ft_report_value(&pevent);
		if (ret != RT_EOK)
			LOG_E("ft_report_value error: %d", ret);
	}

#if FOCALTECH_TOUCH_DEBUG
	LOG_D("ts_event -> touch_point: %d, touchs: %d, touch_point_num: %d",
		pevent.touch_point, pevent.touchs, pevent.touch_point_num);

	int i;
	for (i = 0; i < pevent.touch_point; i++) {
		LOG_D("touch -> (id = %d, x = %d, y = %d, pres = %d, area=%d)",
			pevent.au8_finger_id[i], pevent.au16_x[i],
			pevent.au16_y[i], pevent.pressure[i], pevent.area[i]);
	}
#endif

	ts_request_gpio_irq();
}

/* print some register value for debug */
void focaltech_dump()
{
	rt_err_t ret;
	u8 val;

	ret = ft_i2c_read(WORK_MODE, &val, 1);
	if (ret != RT_EOK)
		return;
	LOG_D("WORK_MODE = %x", val);

	ret = ft_i2c_read(PROXIMITY_STATUS, &val, 1);
	if (ret != RT_EOK)
		return;
	LOG_D("PROXIMITY_STATUS = %x", val);

	ret = ft_i2c_read(FW_VERSION, &val, 1);
	if (ret != RT_EOK)
		return;
	LOG_D("FW_VERSION = %x", val);

	ret = ft_i2c_read(FTS_STATE, &val, 1);
	if (ret != RT_EOK)
		return;
	LOG_D("FTS_STATE = %x", val);
}

static rt_err_t ft_register_device()
{
	rt_err_t ret;

	_ts_dev.name = "focaltech_device";
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
		return -RT_ENOMEM;
	}

	ret = input_register_device(&_ts_dev);
	if (ret != RT_EOK)
		LOG_E("input_register_device error");

	return ret;
}

/**
 * ft_set_power_mode - set touch panel power consume mode
 * @mode: power consume mode
 *
 * return RT_EOK if success, RT_ERROR if failed
 */
static rt_err_t ft_set_power_mode(enum FT_POWER_MODE mode)
{
	u8 val;
	rt_err_t ret;

	switch (mode) {
	case FT_ACTIVE:
		val = 0x00;
		break;
	case FT_MONITOR:
		val = 0x01;
		break;
	case FT_HIBERNATE:
		val = 0x03;
		break;
	default:
		LOG_E("ft_set_power_mode param error: %d", mode);
		return -RT_EINVAL;
	}

	ret = ft_i2c_write(POWER_MODE, &val, 1);
	if (ret != RT_EOK)
		LOG_E("ft_set_power_mode error: %d", ret);

	return ret;
}

/* wake up touch panel */
int ft_wakeup()
{
	/* wake up the tp by reset the pin */
	FT_RESET_L();
	mdelay(1);
	FT_RESET_H();
	mdelay(300);

	return RT_EOK;
}

/* make touch panle sleep */
int ft_sleep()
{
	rt_err_t ret;
	ret = ft_set_power_mode(FT_HIBERNATE);
	return ret;
}

static struct input_int_handler ts_int_handler = {
	.type		= INPUT_EVENT_TOUCH,
	.int_handle	= ft_handle_int,
};

int focaltech_ts_init()
{
	rt_err_t ret;

	ret = setup_i2c_device();
	if (ret != RT_EOK) {
		LOG_E("setup_i2c_device error: %d", ret);
		return -1;
	}

	ret = init_ts_gpio();
	if (ret != RT_EOK) {
		LOG_E("init_ts_gpio error: %d", ret);
		return -1;
	}

	rt_list_init(&ts_int_handler.node);
	ret = input_register_int_handler(&ts_int_handler);
	if (ret != RT_EOK)
		LOG_E("input_register_int_handler ts_int_handler error");

	ret = ft_register_device();
	if (ret != RT_EOK) {
		LOG_E("register_ts_device error: %d", ret);
		return -1;
	}

	focaltech_dump();
	LOG_I("focaltech_ts_init finished");
	return 0;
}

/* n7v1 evb use focaltech touch panel */
#ifdef ARCH_LOMBO_N7V1_EVB
INIT_DEVICE_EXPORT(focaltech_ts_init);
#endif
