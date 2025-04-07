/*
 * gt913_ts.c - Goodix gt913 driver
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
#include <string.h>
#include "input.h"
#include "gpio/pinctrl.h"
#include "gt913_ts.h"
#include "lombo_disp.h"

#define GT913_TS_MODULE_NAME		"gt913-ts"
#define GT913_I2C_HOST			0

/* I2C address */
#define GT913_I2C_WADDR			(0xBA >> 1)
#define GT913_I2C_RADDR			(0xBB >> 1)

/* register address */
#define GT913_REG_VERSION		0x8140
#define GT913_STATUS_INFO		0x814E
#define GT913_COMMAND_ADDR		0x8040

#define GT913_OUTPUT_MAX		0x8048
#define GT913_TOUCH_NUMBER		0x804C
#define GT913_MODULE_SW_ADDR		0x804D
#define GT913_POINT_COORD_ADDR		0x814F

#define GT913_CNT_DOWN			3
#define GT913_CNT_LONG			40
#define GT913_CNT_HOLD			0
#define GT913_COMMAND_SLEEP		5

#define TOUCH_EVENT_UP			0
#define TOUCH_EVENT_DOWN		1
#define TOUCH_EVENT_MOVE		2
#define TOUCH_EVENT_HOLD		3

#define GT913_MAX_TOUCH_POINT		5
#define GT913_PER_COORD_LEN		8
#define TS_EVENTS_PER_PACKET		5

typedef struct touch_point {
	unsigned char e;
	unsigned int  x;
	unsigned int  y;
	unsigned int  c;
} TOUCH_POINT;

typedef struct touch_panel_points {
	unsigned int point_num;
	TOUCH_POINT  p[5];
} TOUCH_PANEL_POINTS;

typedef struct sw_touch_panel_platform_data {
	unsigned char	enable;
	char		*iic_dev;
	unsigned int	rst_pin;
	unsigned int	int_pin;

	unsigned int	_MAX_POINT;
	unsigned int	_MAX_X;
	unsigned int	_MAX_Y;
	unsigned char	_INT_TRIGGER;
	unsigned char	_X2Y_EN;
	unsigned char	_X_MIRRORING;
	unsigned char	_Y_MIRRORING;
	unsigned char	_DEBUGP;
	unsigned char	_DEBUGE;

	TOUCH_PANEL_POINTS points;

	unsigned int	_DISP_W;
	unsigned int	_DISP_H;
} SW_TOUCH_PANEL_PLATFORM_DATA;

typedef struct {
	u8 pid[5];
	u16 vid;
} TPD_INFO;

typedef struct {
	u8 fresh;
	u16 x;
	u16 y;
	u16 size;
} POINT;

static struct input_dev _ts_dev;
static struct rt_i2c_bus_device *i2c_gt913_dev = RT_NULL;
static struct sw_touch_panel_platform_data *_touch_panel_data = RT_NULL;
static rt_bool_t sleep_cmd = RT_FALSE;

TPD_INFO tpd_info;
POINT point[GT913_MAX_TOUCH_POINT];

/**
 * gt913_i2c_write - write data to gt913 tp register by i2c
 * @addr: register address of gt913 to write
 * @buf: the data to write
 * @len: the length of data
 *
 * return RT_EOK if success, RT_ERROR if failed
 */
rt_err_t gt913_i2c_write(u16 addr, u8 *buf, u32 len)
{
	struct rt_i2c_msg m;
	rt_uint8_t msg_buf[len + 2];

	if (i2c_gt913_dev == RT_NULL) {
		LOG_E("Can't find touch panel I2C device");
		return RT_ERROR;
	}

	msg_buf[0] = (u8)(addr >> 8);
	msg_buf[1] = (u8)addr;
	if (len > 0)
		rt_memcpy(msg_buf + 2, buf, len);

	m.addr  = GT913_I2C_WADDR;
	m.flags = RT_I2C_WR;
	m.buf   = msg_buf;
	m.len   = len + 2;

	if (rt_i2c_transfer(i2c_gt913_dev, &m, 1) != 1) {
		LOG_E("rt_i2c_transfer error");
		return RT_ERROR;
	}

	return RT_EOK;
}

/**
 * gt913_i2c_read - read data from gt913 tp register by i2c
 * @addr: register address of gt913 to read
 * @buf: the data to store
 * @len: the length of data
 *
 * return RT_EOK if success, RT_ERROR if failed
 */
rt_err_t gt913_i2c_read(u16 addr, u8 *buf, u32 len)
{
	static const int msg_count = 2;
	struct rt_i2c_msg msgs[msg_count];
	u8 reg_h, reg_l;
	u8 reg_addr[2];

	if (i2c_gt913_dev == RT_NULL) {
		LOG_E("Can't find touch panel I2C device");
		return RT_ERROR;
	}

	reg_h = (u8)(addr >> 8);
	reg_l = (u8)addr;
	reg_addr[0] = reg_h;
	reg_addr[1] = reg_l;

	msgs[0].addr  = GT913_I2C_WADDR;
	msgs[0].flags = RT_I2C_WR;
	msgs[0].buf   = reg_addr;
	msgs[0].len   = 2;

	msgs[1].addr  = GT913_I2C_RADDR;
	msgs[1].flags = RT_I2C_RD;
	msgs[1].buf   = buf;
	msgs[1].len   = len;

	if (rt_i2c_transfer(i2c_gt913_dev, msgs, msg_count) != msg_count) {
		LOG_E("rt_i2c_transfer error");
		return RT_ERROR;
	}

	return RT_EOK;
}

/* set gpio pin direction output and level */
static void gt913_set_gpio_output(enum gpio_port port, enum gpio_pin pin, int value)
{
	struct pinctrl *pctrl;
	int pin_num;
	rt_err_t ret;

	RT_ASSERT((value == 0) || (value == 1));

	pctrl = pinctrl_get(GT913_TS_MODULE_NAME);
	if (pctrl == RT_NULL) {
		LOG_E("pinctrl_get return RT_NULL");
		return;
	}

	pin_num = pinctrl_gpio_request(pctrl, port, pin);
	if (pin_num < 0) {
		LOG_E("pin_num < 0");
		return;
	}

	ret = pinctrl_gpio_direction_output(pctrl, pin_num);
	if (ret != RT_EOK) {
		LOG_E("pinctrl_gpio_direction_output != RT_EOK");
		return;
	}

	ret = pinctrl_gpio_set_value(pctrl, pin_num, value);
	if (ret != RT_EOK) {
		LOG_E("pinctrl_gpio_set_value != RT_EOK");
		return;
	}
}

/* set tp INT pin direction output and high level */
static void TCH_INT_H()
{
	gt913_set_gpio_output(GPIO_PORT_E, GPIO_PIN_10, 1);
}

/* set tp INT pin direction output and low level */
static void TCH_INT_L()
{
	gt913_set_gpio_output(GPIO_PORT_E, GPIO_PIN_10, 0);
}

/* set tp RESET pin direction output and high level */
static void TCH_RST_H()
{
#ifdef ARCH_LOMBO_N7V1_TDR
	gt913_set_gpio_output(GPIO_PORT_D, GPIO_PIN_5, 1);
#else
	gt913_set_gpio_output(GPIO_PORT_E, GPIO_PIN_11, 1);
#endif
}

/* set tp RESET pin direction output and low level */
static void TCH_RST_L()
{
#ifdef ARCH_LOMBO_N7V1_TDR
	gt913_set_gpio_output(GPIO_PORT_D, GPIO_PIN_5, 0);
#else
	gt913_set_gpio_output(GPIO_PORT_E, GPIO_PIN_11, 0);
#endif
}

/* set tp INT pin direction input */
static void TCH_INT_IN()
{
	struct pinctrl *pctrl;
	int pin_num, ret;

	pctrl = pinctrl_get(GT913_TS_MODULE_NAME);
	if (pctrl == RT_NULL) {
		LOG_E("pctrl == RT_NULL");
		return;
	}

	pin_num = pinctrl_gpio_request(pctrl, GPIO_PORT_E, GPIO_PIN_10);
	if (pin_num < 0) {
		LOG_E("pin_num < 0");
		return;
	}

	ret = pinctrl_gpio_direction_input(pctrl, pin_num);
	if (ret != RT_EOK) {
		LOG_E("pinctrl_gpio_direction_input != RT_EOK");
		return;
	}
}

/* free gpio interrupt for to tp INT pin */
static void gt913_ts_free_gpio_irq()
{
	struct pinctrl *pctrl;
	int pin_num, ret;

	pctrl = pinctrl_get(GT913_TS_MODULE_NAME);
	if (pctrl == RT_NULL) {
		LOG_E("pctrl == RT_NULL");
		return;
	}

	pin_num = pinctrl_gpio_request(pctrl, GPIO_PORT_E, GPIO_PIN_10);
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
static void gt913_ts_irq_handler(void *data)
{
	/* send sleep command will create an interrupt, ignore it */
	if (sleep_cmd) {
		LOG_D("sleep command, ignore this interrupt");
		sleep_cmd = RT_FALSE;
		return;
	}

	gt913_ts_free_gpio_irq();
	input_send_int_event(INPUT_EVENT_TOUCH);
}

/* request gpio interrupt for tp INT pin */
static void gt913_ts_request_gpio_irq()
{
	struct pinctrl *pctrl;
	struct gpio_irq_data irq_data;
	int pin_num, ret;

	pctrl = pinctrl_get(GT913_TS_MODULE_NAME);
	if (pctrl == RT_NULL) {
		LOG_E("pctrl == RT_NULL");
		return;
	}

	pin_num = pinctrl_gpio_request(pctrl, GPIO_PORT_E, GPIO_PIN_10);
	if (pin_num < 0) {
		LOG_E("pin_num < 0");
		return;
	}

	irq_data.handler = gt913_ts_irq_handler;	/* interrupt handle function */
	irq_data.irq_arg = NULL;
	irq_data.clock_src = GPIO_IRQ_HOSC_24MHZ;
	irq_data.clock_src_div = 10;
	irq_data.trig_type = EINT_TRIG_FALLING_EDGE;

	ret = pinctrl_gpio_request_irq(pctrl, pin_num, &irq_data);
	if (ret != RT_EOK) {
		LOG_E("pinctrl_gpio_request_irq != RT_EOK");
		return;
	}
}


/* set touch panel touch point max limit */
static rt_err_t gt913_set_tp_number(u8 number)
{
	rt_err_t ret;
	u8 val, nv;

	RT_ASSERT((number > 0) && (number <= GT913_MAX_TOUCH_POINT));

	/* touch number: bit0-bit3 */
	ret = gt913_i2c_read(GT913_TOUCH_NUMBER, &val, 1);
	if (ret != RT_EOK) {
		LOG_E("read GT913_TOUCH_NUMBER error");
		return ret;
	}

	nv = (val & 0xf0) | (number & 0x0f);
	ret = gt913_i2c_write(GT913_TOUCH_NUMBER, &nv, 1);
	if (ret != RT_EOK) {
		LOG_E("write GT913_TOUCH_NUMBER error");
		return ret;
	}

	return RT_EOK;
}

/* get gt913 touch panel product id and version */
static rt_err_t gt913_read_id(void)
{
	rt_err_t ret;
	u8 buf[6] = {0};

	/* register address from 0x8140 to 0x8145, product id and version */
	ret = gt913_i2c_read(GT913_REG_VERSION, buf, 6);
	if (ret != RT_EOK) {
		LOG_E("get GT913_REG_VERSION error");
		return ret;
	}

	/* Product ID */
	rt_memset(tpd_info.pid, 0, 5);
	rt_memcpy(tpd_info.pid, buf, 4);
	LOG_D("touch panel id: %s", tpd_info.pid);

	/* Firmware version */
	tpd_info.vid = (buf[5] << 8) | buf[4];
	LOG_D("firmware version: %d", tpd_info.vid);

	if (tpd_info.vid == 0)
		return RT_ERROR;

	return RT_EOK;
}

/* handle touch panel event and return number of touch */
static u8 fresh_comm;
static u8 gt913_event_handler(void)
{
	u8 touch_info;
	u8 point_data[GT913_PER_COORD_LEN * GT913_MAX_TOUCH_POINT] = {0};
	u8 tid;
	u8 touch_num = 0;
	u8 buf_status;

	int i;
	rt_err_t ret;

	ret = gt913_i2c_read(GT913_STATUS_INFO, &touch_info, 1);
	if (ret != RT_EOK) {
		LOG_E("gt913_i2c_read GT913_STATUS_INFO error");
		goto exit_work_func;
	}

	/* buffer status */
	buf_status = touch_info & 0x80;
	if (buf_status == 0) {
		//LOG_W("buffer status: %d", buf_status);
		goto exit_work_func;
	}

	/* bit0-bit3: number of touch points */
	touch_num = touch_info & 0x0f;
	if (touch_num > _touch_panel_data->_MAX_POINT) {
		LOG_E("number of touch points larger than MAX_POINT");
		goto exit_work_func;
	}
	/* LOG_D("number of touch point: %d", touch_num); */

	fresh_comm++;
	if (touch_num > 0) {
		ret = gt913_i2c_read(GT913_POINT_COORD_ADDR, point_data,
			GT913_PER_COORD_LEN * touch_num);
		if (ret != RT_EOK) {
			LOG_E("read touch coordinate data error");
			goto exit_work_func;
		}

		for (i = 0; i < touch_num; i++) {
			/* track id */
			int index = GT913_PER_COORD_LEN * i;
			tid = point_data[index] & 0x0F;
			if (tid > _touch_panel_data->_MAX_POINT - 1) {
				LOG_W("track id larger than MAX_POINT");
				continue;
			}

			/* touch point x, y, size */
			point[tid].fresh++;
			point[tid].x = point_data[index + 1] | point_data[index + 2] << 8;
			point[tid].y = point_data[index + 3] | point_data[index + 4] << 8;
			point[tid].size = point_data[index + 5] |
				(point_data[index + 6] << 8);
			if (_touch_panel_data->_X_MIRRORING)
				point[tid].x = _touch_panel_data->_MAX_X - point[tid].x;

			if (_touch_panel_data->_Y_MIRRORING)
				point[tid].y = _touch_panel_data->_MAX_Y - point[tid].y;

			/*
			LOG_D("track id: %d -> x = %d, y = %d, size = %d",
				tid, point[tid].x, point[tid].y, point[tid].size);
			*/
		}
	}

	for (i = 0; i < _touch_panel_data->_MAX_POINT; i++) {
		if (point[i].fresh != fresh_comm) {
			point[i].fresh = fresh_comm;
			point[i].x = 0x7FFF;
			point[i].y = 0x7FFF;
			point[i].size = 0x7FFF;
		}
	}

exit_work_func:
	ret = gt913_i2c_write(GT913_STATUS_INFO, (u8 *)"\0", 1);
	if (ret != RT_EOK)
		LOG_E("write end_cmd to GT913_STATUS_INFO error");

	return touch_num;
}

/* read touch points data and report event */
static void gt913_read_point()
{
	int i, tmp;
	struct sw_touch_panel_platform_data *tpd;

	tpd = _touch_panel_data;
	if (tpd == RT_NULL) {
		LOG_W("gt913_read_point: _touch_panel_data == RT_NULL");
		return;
	}
	tpd->points.point_num = gt913_event_handler();

	for (i = 0; i < tpd->_MAX_POINT; i++) {
		tpd->points.p[i].e = 0xff;

		if (point[i].x > tpd->_MAX_X ||
			point[i].y > tpd->_MAX_Y) {
			if (tpd->points.p[i].c < GT913_CNT_DOWN)
				tpd->points.p[i].e = 0xff;
			else
				tpd->points.p[i].e = TOUCH_EVENT_UP;
			tpd->points.p[i].c = 0;
		} else {
			tpd->points.p[i].c++;

			if (tpd->points.p[i].c == GT913_CNT_DOWN) {
				tpd->points.p[i].x = point[i].x;
				tpd->points.p[i].y = point[i].y;
				tpd->points.p[i].e = TOUCH_EVENT_DOWN;
			} else if (tpd->points.p[i].c > GT913_CNT_DOWN) {
				if (point[i].x != tpd->points.p[i].x ||
					point[i].y != tpd->points.p[i].y) {
					tmp = GT913_CNT_DOWN + GT913_CNT_DOWN;
					if (tpd->points.p[i].c > tmp) {
						tpd->points.p[i].x = point[i].x;
						tpd->points.p[i].y = point[i].y;
						tpd->points.p[i].e = TOUCH_EVENT_MOVE;
						tpd->points.p[i].c = GT913_CNT_DOWN;
					}
				}

				tmp = GT913_CNT_LONG + GT913_CNT_HOLD;
				if (tpd->points.p[i].c == tmp) {
					tpd->points.p[i].x = point[i].x;
					tpd->points.p[i].y = point[i].y;
					tpd->points.p[i].e = TOUCH_EVENT_HOLD;
				}

				tmp = GT913_CNT_LONG + GT913_CNT_HOLD;
				if (tpd->points.p[i].c > tmp)
					tpd->points.p[i].c = GT913_CNT_LONG;
			}
		}

		if (tpd->points.p[i].e != 0xff) {
			if (tpd->points.p[i].e == 3)
				continue;

			/* report event */
			input_report_abs(&_ts_dev,
					ABS_MT_TRACKING_ID,
					i);
			input_report_abs(&_ts_dev,
				ABS_MT_TOUCH_MAJOR,
				tpd->points.p[i].e);
			input_report_abs(&_ts_dev,
				ABS_MT_POSITION_X,
				(tpd->points.p[i].x*tpd->_DISP_W/tpd->_MAX_X)-10);
			input_report_abs(&_ts_dev,
				ABS_MT_POSITION_Y,
				(tpd->points.p[i].y*tpd->_DISP_H/tpd->_MAX_Y)-10);
			input_mt_sync(&_ts_dev);

#if 0
			LOG_E("[id: %d](event: %d) (x: %d, y: %d)", i,
				tpd->points.p[i].e,
				tpd->points.p[i].x,
				tpd->points.p[i].y);
#endif
		}
	}
}

static void power_on_sequence()
{
	TCH_INT_L();
	TCH_RST_L();
	mdelay(50);

	/* TCH_INT_H(); */
	udelay(200);

	TCH_RST_H();
	mdelay(10);

	TCH_INT_L();
	mdelay(100);

	TCH_INT_IN();
}

static void power_on_sequence_ext()
{
	TCH_INT_L();
	TCH_RST_L();
	mdelay(200);

	/* TCH_INT_H(); */
	udelay(200);

	TCH_RST_H();
	mdelay(50);

	TCH_INT_L();
	mdelay(500);

	TCH_INT_IN();
}

static void reset_sequence()
{
	TCH_INT_L();
	TCH_RST_L();
	udelay(100);

	TCH_INT_H();
	udelay(100);

	TCH_RST_H();
	mdelay(5);

	TCH_INT_L();
	mdelay(50);

	TCH_INT_IN();
}

static void gt913_int_sync(s32 ms)
{
	TCH_INT_H();
	mdelay(3);
	TCH_INT_L();
	mdelay(ms);
	TCH_INT_IN();
}

static void gt913_reset_guitar(s32 ms)
{
	TCH_RST_L();
	mdelay(ms);

	TCH_INT_L();

	mdelay(2);
	TCH_RST_H();
	mdelay(6);

	gt913_int_sync(50);
}

/* touch panel wakeup */
int gt913_wakeup()
{
	gt913_reset_guitar(50);
	LOG_D("touch panel wakeup");
	return RT_EOK;
}

/* touch panel sleep */
int gt913_sleep()
{
	rt_err_t ret;
	u8 command = GT913_COMMAND_SLEEP;

	ret = gt913_i2c_write(GT913_COMMAND_ADDR, &command, 1);
	if (ret == RT_EOK)
		sleep_cmd = RT_TRUE;
	else
		LOG_D("gt913_sleep error");

	return ret;
}

rt_err_t gt913_init(void)
{
	rt_err_t ret;
	int i;

	/* gt913 power on sequence */
	power_on_sequence();

	/* get tp product id and version infomation */
	ret = gt913_read_id();
	if (ret != RT_EOK) {
		LOG_E("gt913_read_id error");
		power_on_sequence_ext();
		gt913_read_id();
	}

	/* set touch number */
	gt913_set_tp_number(GT913_MAX_TOUCH_POINT);

	for (i = 0; i < GT913_MAX_TOUCH_POINT; i++) {
		point[i].fresh = 0;
		point[i].x = 0x7FFF;
		point[i].y = 0x7FFF;
		point[i].size = 0x7FFF;
	}
	return ret;
}

/* initialize touch panel i2c device */
static rt_err_t setup_i2c_device()
{
	char i2c_name[6] = {0};
	rt_sprintf(i2c_name, "%s%d", "i2c", GT913_I2C_HOST);

	i2c_gt913_dev = rt_i2c_bus_device_find(i2c_name);
	if (i2c_gt913_dev == RT_NULL) {
		LOG_E("can't find bus dev \"%s\"", i2c_name);
		return RT_ERROR;
	}

	return RT_EOK;
}

static rt_err_t setup_tp_data()
{
#if 0
	disp_io_ctrl_t dic = {0};
	struct rt_device_graphic_info info = {0};
	rt_device_t disp_device;
#endif
	int sz;
	rt_err_t ret;
	u8 buf[4] = {0};
	u16 max_x, max_y;

	sz = sizeof(struct sw_touch_panel_platform_data);
	_touch_panel_data = rt_malloc(sz);
	if (_touch_panel_data == RT_NULL) {
		LOG_E("rt_malloc sw_touch_panel_platform_data error");
		return -RT_ENOMEM;
	}
	rt_memset(_touch_panel_data, 0, sz);

	/* address 0x8048-0x804B, X output max, Y ouput max */
	ret = gt913_i2c_read(GT913_OUTPUT_MAX, buf, 4);
	if (ret != RT_EOK) {
		LOG_E("read X ouput max and Y output max error");
		return ret;
	}

	max_x = (buf[1] << 8) | buf[0];
	max_y = (buf[3] << 8) | buf[2];

	_touch_panel_data->_MAX_POINT = GT913_MAX_TOUCH_POINT;
	_touch_panel_data->_MAX_X = max_x;
	_touch_panel_data->_MAX_Y = max_y;
#if 0
	disp_device = rt_device_find(DISP_DEVICE_NAME);
	if (disp_device != NULL) {
		rt_device_open(disp_device, 0);

		dic.dc_index = 0;
		dic.args = &info;
		rt_device_control(disp_device, DISP_CMD_GET_INFO, &dic);
#if defined(PANEL_WT1096601G01_24_IVO)\
	|| defined(PANEL_ZT1180_2401)\
	|| defined(PANEL_ZT0936BOE)\
	|| defined(PANEL_WT1096602G02_24)
		_touch_panel_data->_DISP_W = info.height;
		_touch_panel_data->_DISP_H = info.width;
#else
		_touch_panel_data->_DISP_W = info.width;
		_touch_panel_data->_DISP_H = info.height;
#endif

		rt_device_close(disp_device);
	} else {
		LOG_E("setup_tp_data ERROR, used default");
		_touch_panel_data->_DISP_W = max_x;
		_touch_panel_data->_DISP_H = max_y;
	}
#else
	_touch_panel_data->_DISP_W = 1280;
	_touch_panel_data->_DISP_H = 320;
#endif
    _touch_panel_data->_X_MIRRORING= 1;
	_touch_panel_data->_Y_MIRRORING = 1;


	return RT_EOK;
}

static void gt913_handle_int()
{
	/* read touch panel data from i2c */
	gt913_read_point();
	gt913_ts_request_gpio_irq();
}

static rt_err_t gt913_register_device()
{
	rt_err_t ret;

	_ts_dev.name = "gt913_device";
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
	if (ret != RT_EOK) {
		LOG_E("input_register_device error");
		return ret;
	}

	return RT_EOK;
}

void gt913_reg_dump()
{
	u8 buf[4] = {0};
	u8 val;
	rt_err_t ret;

	LOG_D("GT913 touch panel dump:");

	/* INT: bit0-bit1 */
	ret = gt913_i2c_read(GT913_MODULE_SW_ADDR, buf, 1);
	if (ret == RT_EOK) {
		val = buf[0] & 0x03;
		LOG_D("INT trigger mode: %d", val);
	}

	/* address 0x8048-0x804B, X output max, Y ouput max */
	ret = gt913_i2c_read(GT913_OUTPUT_MAX, buf, 4);
	if (ret == RT_EOK) {
		u16 max_x = (buf[1] << 8) | buf[0];
		u16 max_y = (buf[3] << 8) | buf[2];
		LOG_D("X output max: %d, Y output max: %d", max_x, max_y);
	}

	/* touch number, bit0-bit3 */
	ret = gt913_i2c_read(0x804C, buf, 1);
	if (ret == RT_EOK) {
		val = buf[0] & 0x0f;
		LOG_D("Touch number: %d", val);
	}
}

static struct input_int_handler ts_int_handler = {
	.type		= INPUT_EVENT_TOUCH,
	.int_handle	= gt913_handle_int,
};

int gt913_ts_init()
{
	rt_err_t ret;
	LOG_I("*************** gt913_ts_init ****************");

	/* find i2c device for touch panel */
	ret = setup_i2c_device();
	if (ret != RT_EOK) {
		LOG_E("setup_i2c_device error");
		return -1;
	}

	/* other i2c read or write operation must after gt913_init function run */
	ret = gt913_init();
	if (ret != RT_EOK) {
		LOG_E("gt913_init error");
		return -1;
	}

	/* create and setup sw_touch_panel_platform_data for global */
	ret = setup_tp_data();
	if (ret != RT_EOK) {
		LOG_E("setup_tp_data error");
		return -1;
	}

	rt_list_init(&ts_int_handler.node);
	ret = input_register_int_handler(&ts_int_handler);
	if (ret != RT_EOK)
		LOG_E("input_register_int_handler ts_int_handler error");

	/* register gt913 device to input core */
	ret = gt913_register_device();
	if (ret != RT_EOK) {
		LOG_E("gt913_register_device error");
		return -1;
	}

	/* log gt913 some register value for debug */
	/* gt913_reg_dump(); */

	/* register gpio interrupt for tp touch event */
	gt913_ts_request_gpio_irq();

	return 0;
}

/* cdr use gt913 touch panel */
#if defined(ARCH_LOMBO_N7V0_CDR) || defined(ARCH_LOMBO_N7V1_CDR)
INIT_APP_EXPORT(gt913_ts_init);
#endif

