/*
 * rtc_pcf8563.c - pcf8563 driver
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
#include <time.h>
#include <string.h>
#include "gpio/pinctrl.h"

#define RTC_PCF8563_I2C_HOST			2
/* I2C address */
#define RTC_PCF8563_I2C_WADDR			(0xA2 >> 1)
#define RTC_PCF8563_I2C_RADDR			(0xA3 >> 1)

/* register address */
#define RTC_PCF8563_YEAR			0x08 /* 00~99 */
#define RTC_PCF8563_MON				0x07 /* 01~12 */
#define RTC_PCF8563_MDAY			0x05 /* 01~31 */
#define RTC_PCF8563_WDAY			0x06 /* 0~6 */
#define RTC_PCF8563_HOUR			0x04 /* 00~23 */
#define RTC_PCF8563_MIN				0x03 /* 00~59 */
#define RTC_PCF8563_SEC				0x02 /* 00~59 */

static struct rt_i2c_bus_device *i2c_rtc_pcf8563_dev = RT_NULL;

/**
 * rtc_pcf8563_i2c_write - write data to rtc_pcf8563 tp register by i2c
 * @addr: register address of rtc_pcf8563 to write
 * @buf: the data to write
 * @len: the length of data
 *
 * return RT_EOK if success, RT_ERROR if failed
 */
static rt_err_t rtc_pcf8563_i2c_write(u8 reg, u8 data)
{
	struct rt_i2c_msg msgs;
	rt_uint8_t buf[2] = {reg, data};

	if (i2c_rtc_pcf8563_dev == RT_NULL) {
		LOG_E("Can't find pcf8563 I2C device");
		return RT_ERROR;
	}

	msgs.addr  = RTC_PCF8563_I2C_WADDR;
	msgs.flags = RT_I2C_WR;
	msgs.buf   = buf;
	msgs.len   = 2;

	if (rt_i2c_transfer(i2c_rtc_pcf8563_dev, &msgs, 1) != 1) {
		LOG_E("rt_i2c_transfer error");
		return RT_ERROR;
	}

	return RT_EOK;
}

/**
 * rtc_pcf8563_i2c_read - read data from rtc_pcf8563 tp register by i2c
 * @addr: register address of rtc_pcf8563 to read
 * @buf: the data to store
 * @len: the length of data
 *
 * return RT_EOK if success, RT_ERROR if failed
 */
static rt_err_t rtc_pcf8563_i2c_read(u8 reg, u8 *redata)
{
	struct rt_i2c_msg msgs[2];

	if (i2c_rtc_pcf8563_dev == RT_NULL) {
		LOG_E("Can't find pcf8563 I2C device");
		return RT_ERROR;
	}

	msgs[0].addr  = RTC_PCF8563_I2C_WADDR;
	msgs[0].flags = RT_I2C_WR;
	msgs[0].buf   = &reg;
	msgs[0].len   = 1;

	msgs[1].addr  = RTC_PCF8563_I2C_RADDR;
	msgs[1].flags = RT_I2C_RD;
	msgs[1].buf   = redata;
	msgs[1].len   = 1;

	if (rt_i2c_transfer(i2c_rtc_pcf8563_dev, msgs, 2) != 2) {
		LOG_E("rt_i2c_transfer error");
		return RT_ERROR;
	}

	return RT_EOK;
}

/*
 * rtc_pcf8563_get_time - get system time
 * @p_tm: pointer to tm
 *
 */
int rtc_pcf8563_get_time(struct tm *p_tm)
{
	rt_err_t ret;
	u8 val;

	int year, mon, mday;
	int wday;
	int hour, min, sec;

	ret = rtc_pcf8563_i2c_read(RTC_PCF8563_YEAR, &val);
	if (ret != RT_EOK) {
		LOG_E("read rtc_pcf8563 error");
		return RT_ERROR;
	}
	year = ((val>>4)&0xF)*10 + (val&0xF);

	ret = rtc_pcf8563_i2c_read(RTC_PCF8563_MON, &val);
	if (ret != RT_EOK) {
		LOG_E("read rtc_pcf8563 error");
		return RT_ERROR;
	}
	mon = ((val>>4)&0x1)*10 + (val&0xF);

	ret = rtc_pcf8563_i2c_read(RTC_PCF8563_MDAY, &val);
	if (ret != RT_EOK) {
		LOG_E("read rtc_pcf8563 error");
		return RT_ERROR;
	}
	mday = ((val>>4)&0x3)*10 + (val&0xF);

	ret = rtc_pcf8563_i2c_read(RTC_PCF8563_WDAY, &val);
	if (ret != RT_EOK) {
		LOG_E("read rtc_pcf8563 error");
		return RT_ERROR;
	}
	wday = (val&0x7);

	ret = rtc_pcf8563_i2c_read(RTC_PCF8563_HOUR, &val);
	if (ret != RT_EOK) {
		LOG_E("read rtc_pcf8563 error");
		return RT_ERROR;
	}
	hour = ((val>>4)&0x3)*10 + (val&0xF);

	ret = rtc_pcf8563_i2c_read(RTC_PCF8563_MIN, &val);
	if (ret != RT_EOK) {
		LOG_E("read rtc_pcf8563 error");
		return RT_ERROR;
	}
	min = ((val>>4)&0x7)*10 + (val&0xF);

	ret = rtc_pcf8563_i2c_read(RTC_PCF8563_SEC, &val);
	if (ret != RT_EOK) {
		LOG_E("read rtc_pcf8563 error");
		return RT_ERROR;
	}
	sec = ((val>>4)&0x7)*10 + (val&0xF);

	p_tm->tm_year	= year + 2000 - 1900;
	p_tm->tm_mon	= mon - 1;
	p_tm->tm_mday	= mday;
	p_tm->tm_wday	= wday;
	p_tm->tm_hour	= hour;
	p_tm->tm_min	= min;
	p_tm->tm_sec	= sec;
	p_tm->tm_yday	= 0;
	p_tm->tm_isdst	= 0;

	// LOG_W("get time: %d-%d-%d %d:%d:%d (%d)", p_tm->tm_year, p_tm->tm_mon,
	// 		p_tm->tm_mday, p_tm->tm_hour, p_tm->tm_min,
	// 		p_tm->tm_sec, p_tm->tm_wday);

	return RT_EOK;
}

/*
 * rtc_pcf8563_set_time - set system time
 * @p_tm: pointer to tm
 *
 * return: RT_EOK, set succeed; RT_ERROR, set failed
 */
int rtc_pcf8563_set_time(struct tm *p_tm)
{
	rt_err_t ret;
	u8 val;

	int year, mon, mday;
	int wday;
	int hour, min, sec;

	year	= p_tm->tm_year + 1900 - 2000;
	mon	= p_tm->tm_mon + 1;
	mday	= p_tm->tm_mday;
	wday	= p_tm->tm_wday;
	hour	= p_tm->tm_hour;
	min	= p_tm->tm_min;
	sec	= p_tm->tm_sec;

	LOG_W("set time: %d-%d-%d %d:%d:%d (%d)", p_tm->tm_year, p_tm->tm_mon,
			p_tm->tm_mday, p_tm->tm_hour, p_tm->tm_min,
			p_tm->tm_sec, p_tm->tm_wday);

	val = ((((year/10)&0xF)<<4) | ((year%10)&0xF));
	ret = rtc_pcf8563_i2c_write(RTC_PCF8563_YEAR, val);
	if (ret != RT_EOK) {
		LOG_E("read rtc_pcf8563 error");
		return RT_ERROR;
	}

	val = ((((mon/10)&0x1)<<4) | ((mon%10)&0xF));
	ret = rtc_pcf8563_i2c_write(RTC_PCF8563_MON, val);
	if (ret != RT_EOK) {
		LOG_E("read rtc_pcf8563 error");
		return RT_ERROR;
	}

	val = ((((mday/10)&0x3)<<4) | ((mday%10)&0xF));
	ret = rtc_pcf8563_i2c_write(RTC_PCF8563_MDAY, val);
	if (ret != RT_EOK) {
		LOG_E("read rtc_pcf8563 error");
		return RT_ERROR;
	}

	val = ((wday%10)&0x7);
	ret = rtc_pcf8563_i2c_write(RTC_PCF8563_WDAY, val);
	if (ret != RT_EOK) {
		LOG_E("read rtc_pcf8563 error");
		return RT_ERROR;
	}

	val = ((((hour/10)&0x3)<<4) | ((hour%10)&0xF));
	ret = rtc_pcf8563_i2c_write(RTC_PCF8563_HOUR, val);
	if (ret != RT_EOK) {
		LOG_E("read rtc_pcf8563 error");
		return RT_ERROR;
	}

	val = ((((min/10)&0x7)<<4) | ((min%10)&0xF));
	ret = rtc_pcf8563_i2c_write(RTC_PCF8563_MIN, val);
	if (ret != RT_EOK) {
		LOG_E("read rtc_pcf8563 error");
		return RT_ERROR;
	}

	val = ((((sec/10)&0x7)<<4) | ((sec%10)&0xF));
	ret = rtc_pcf8563_i2c_write(RTC_PCF8563_SEC, val);
	if (ret != RT_EOK) {
		LOG_E("read rtc_pcf8563 error");
		return RT_ERROR;
	}

	return RT_EOK;
}

/* initialize pcf8563 i2c device */
rt_err_t rtc_pcf8563_setup_i2c_device(void)
{
	char i2c_name[6] = {0};

	rt_sprintf(i2c_name, "%s%d", "i2c", RTC_PCF8563_I2C_HOST);

	i2c_rtc_pcf8563_dev = rt_i2c_bus_device_find(i2c_name);
	if (i2c_rtc_pcf8563_dev == RT_NULL) {
		LOG_E("can't find bus dev \"%s\"", i2c_name);
		return RT_ERROR;
	}

	return RT_EOK;
}

#if 0
static void rtc_pcf8563_init(int argc, char **argv)
{
	rtc_pcf8563_setup_i2c_device();
}

static void rtc_pcf8563_get(int argc, char **argv)
{
	struct tm p_tm;

	rtc_pcf8563_get_time(&p_tm);
}

static void rtc_pcf8563_set(int argc, char **argv)
{
	struct tm p_tm;

	p_tm.tm_year	= 2020 - 1900;
	p_tm.tm_mon	= 1 - 1;
	p_tm.tm_mday	= 1;
	p_tm.tm_wday	= 3;
	p_tm.tm_hour	= 11;
	p_tm.tm_min	= 30;
	p_tm.tm_sec	= 40;
	p_tm.tm_yday	= 0;
	p_tm.tm_isdst	= 0;

	rtc_pcf8563_set_time(&p_tm);
}
#ifdef RT_USING_FINSH
#include <finsh.h>
FINSH_FUNCTION_EXPORT(rtc_pcf8563_init, FINSH rtc_pcf8563_init);
MSH_CMD_EXPORT(rtc_pcf8563_init, MSH rtc_pcf8563_init);

FINSH_FUNCTION_EXPORT(rtc_pcf8563_get, FINSH rtc_pcf8563_get);
MSH_CMD_EXPORT(rtc_pcf8563_get, MSH rtc_pcf8563_get);

FINSH_FUNCTION_EXPORT(rtc_pcf8563_set, FINSH rtc_pcf8563_set);
MSH_CMD_EXPORT(rtc_pcf8563_set, MSH rtc_pcf8563_set);
#endif
#endif

