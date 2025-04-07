/*
 * gps_dev.h - gps device module file
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
#include <string.h>
#include <rtdevice.h>
#include "uart.h"
#include "gpio/pinctrl.h"
#include "drivers/serial.h"
#include "gps_dev.h"
#include "gps_protocal.h"

#define GPS_UART_DEVICE			"uart2"
#define GPS_RX_PORT			GPIO_PORT_E
#define GPS_RX_PIN			GPIO_PIN_3
#ifdef ARCH_LOMBO_N7V1_TDR
#define GPS_EN_PORT			GPIO_PORT_D
#define GPS_EN_PIN			GPIO_PIN_1
#else
#define GPS_EN_PORT			GPIO_PORT_B
#define GPS_EN_PIN			GPIO_PIN_5
#endif

static rt_device_t gps_device = RT_NULL;	/* get gps data by uart2 */
static u64 recent_rec_t;			/* the last time data was received */

/* uart receive data callback function */
static rt_err_t uart_gps_input(rt_device_t dev, rt_size_t size)
{
	char buff[128] = {0};
	rt_size_t sz;
	int i;

	/* update the time */
	recent_rec_t = rt_time_get_msec();

	/* read data from device and save to global buffer */
	sz = rt_device_read(dev, 0, buff, size);

#ifdef GPS_PROTOCAL_DEBUG
	for (i = 0; i < sz; i++)
		rt_kprintf("%c", buff[i]);
#endif

	for (i = 0; i < sz; i++)
		protocal_add_data(buff[i]);

	return RT_EOK;
}


/* pull up the rx pin of gps uart */
static rt_err_t gps_pull_up_rx()
{
	struct pinctrl *pctrl;
	rt_err_t ret;
	int pin_num;

	/* gps device use uart2 */
	pctrl = pinctrl_get("uart");
	if (pctrl == RT_NULL) {
		LOG_E("pctrl == RT_NULL");
		return -RT_ERROR;
	}

	/* gps device rx pin */
	pin_num = pinctrl_gpio_request(pctrl, GPS_RX_PORT, GPS_RX_PIN);
	if (pin_num < 0) {
		LOG_E("pin_num < 0");
		return -RT_ERROR;
	}

	/* set pull up mode */
	ret = pinctrl_gpio_set_pud_mode(pctrl, pin_num, PULL_UP);
	if (ret != RT_EOK)
		LOG_E("pinctrl_gpio_set_pud_mode != RT_EOK");

	return ret;
}

/* GPS_EN pin enable */
static rt_err_t gps_enable()
{
	struct pinctrl *pctrl;
	rt_err_t ret;
	int pin_num;

	pctrl = pinctrl_get("gps_en");
	if (pctrl == RT_NULL) {
		LOG_E("pctrl == RT_NULL");
		return -RT_ERROR;
	}

	pin_num = pinctrl_gpio_request(pctrl, GPS_EN_PORT, GPS_EN_PIN);
	if (pin_num < 0) {
		LOG_E("pin_num < 0");
		return -RT_ERROR;
	}

	ret = pinctrl_gpio_direction_output(pctrl, pin_num);
	if (ret != RT_EOK) {
		LOG_E("pinctrl_gpio_direction_output error: %d", ret);
		return ret;
	}

	ret = pinctrl_gpio_set_value(pctrl, pin_num, 1);
	if (ret != RT_EOK) {
		LOG_E("pinctrl_gpio_set_value error: %d", ret);
		return ret;
	}

	return RT_EOK;
}

/* GPS_EN pin disable */
static void gps_disable()
{
	reg_gpio_gpiob_func_r0_t reg;

	/* set GPS_EN pin gpio5 IO DISABLE */
	reg.val = READREG32(VA_GPIO_GPIOB_FUNC_R0);
	reg.bits.gpiob5 = 0;
	WRITEREG32(VA_GPIO_GPIOB_FUNC_R0, reg.val);
}

/*
 * gps_open_device - open gps device for receive data
 *
 * return: RT_EOK, open gps device success; other, failed
 */
rt_err_t gps_open_device()
{
	rt_err_t ret;
	rt_uint16_t oflag;
	struct lombo_uart_port *port;

	if (gps_device) {
		LOG_W("gps device already open");
		return RT_EOK;
	}

	/* pull up gps rx pin */
	ret = gps_pull_up_rx();
	if (ret != RT_EOK)
		return ret;

	/* enable GPS_EN pin */
	ret = gps_enable();
	if (ret != RT_EOK)
		return ret;

	/* Find the device */
	gps_device = rt_device_find(GPS_UART_DEVICE);
	if (gps_device == RT_NULL) {
		LOG_E("%s not found", GPS_UART_DEVICE);
		return -RT_ERROR;
	}

	/* set gps device band rate */
	port = (struct lombo_uart_port *)gps_device->user_data;
	port->serial->config.baud_rate = BAUD_RATE_9600;
	port->fifosize = 128;
	if (port->serial->ops->configure)
		port->serial->ops->configure(port->serial, &port->serial->config);

	/* register received data callback function */
	ret = rt_device_set_rx_indicate(gps_device, uart_gps_input);
	if (ret != RT_EOK) {
		LOG_E("rt_device_set_rx_indicate error: %d", ret);
		return ret;
	}

	/* Open the device */
	oflag = RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX;
	ret = rt_device_open(gps_device, oflag);
	if (ret != RT_EOK)
		LOG_E("Failed to open dev:%s with flag:%d", GPS_UART_DEVICE, oflag);

	/* set the initial value */
	recent_rec_t = rt_time_get_msec();

	LOG_I("gps_open_device finish");
	return ret;
}
RTM_EXPORT(gps_open_device);

/*
 * gps_close_device - close gps device and stop receive data
 *
 * return: RT_EOK, close gps device success; other, failed
 */
rt_err_t gps_close_device()
{
	rt_err_t ret;

	if (gps_device == RT_NULL) {
		LOG_W("gps device already close");
		return RT_EOK;
	}

	/* disable GPS_EN */
	gps_disable();

	ret = rt_device_close(gps_device);
	if (ret != RT_EOK) {
		LOG_E("rt_device_close gps_device error: %d", ret);
		return ret;
	}

	gps_device = RT_NULL;
	LOG_I("gps_close_device finish");
	return RT_EOK;
}
RTM_EXPORT(gps_close_device);

/*
 * gps_get_data - get recently received gps data
 *
 * return: gps data
 */
struct gps_data_t gps_get_data()
{
	struct gps_data_t d;

	d.valid = RT_FALSE;

	if (gps_device == RT_NULL) {
		//LOG_W("the gps device is close, you should open it first");
		return d;
	}

	if (!gps_connect_status())
		return d;

	return protocal_get_gps();
}
RTM_EXPORT(gps_get_data);

/* get gps device connect status */
rt_bool_t gps_connect_status()
{
	u64 current;

	if (gps_device == RT_NULL) {
	//	LOG_W("It will always return FALSE if the gps device not open");
		return RT_FALSE;
	}

	/* No data received for a period of time */
	current = rt_time_get_msec();
	if (current - recent_rec_t > 1500)
		return RT_FALSE;

	return RT_TRUE;
}
RTM_EXPORT(gps_connect_status);



