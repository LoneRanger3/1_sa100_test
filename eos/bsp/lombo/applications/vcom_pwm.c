/*
 * vcom_pwm.c - vcom adjust source file
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

#ifndef VCOM_PWM_C
#define VCOM_PWM_C

#include <rtthread.h>
#include "stdlib.h"
#include "stdio.h"
#include <debug.h>
#include "pwm/pwm.h"
#include "gpio/pinctrl.h"
#include <dfs_posix.h>

static struct pwm_device *pwm;
static struct pinctrl *pctrl;

static void vcom_adjust_pwm0(int duty)
{
	rt_err_t ret;
	int pin_num;
	int period = 168000;

	if (duty < 0 || duty > 50)
		return;

	duty *= 3340;
	duty += 1000;

	LOG_E("VCOM ADJUST: %d, %d", duty, period);

	if (!pwm) {
		pctrl = pinctrl_get("pwm");
		if (!pctrl) {
			LOG_E("get pinctrl failed");
			return;
		}

		pin_num = pinctrl_gpio_request(pctrl, GPIO_PORT_E, GPIO_PIN_0);
		if (pin_num < 0) {
			LOG_E("port(%d) pin(%d) request failed",
					GPIO_PORT_E, GPIO_PIN_0);
			return;
		}

		ret = pinctrl_gpio_set_function(pctrl, pin_num, 5);
		if (ret < 0) {
			LOG_E("gpio(%d) set function failed", pin_num);
			return;
		}

		pinctrl_put(pctrl);

		pwm = pwm_request(0, "pwm-0");
		if (!pwm) {
			LOG_E("request pwm failed");
			return;
		}

		ret = pwm_set_polarity_normal(pwm);
		if (ret != RT_EOK) {
			LOG_E("pwm set polarity failed");
			return;
		}

		ret = pwm_config(pwm, duty, period);
		if (ret != RT_EOK) {
			LOG_E("pwm config failed");
			return;
		}

		pwm_enable(pwm);
		if (ret != RT_EOK) {
			LOG_E("pwm enable failed");
			return;
		}
	} else {
		ret = pwm_config(pwm, duty, period);
		if (ret != RT_EOK) {
			LOG_E("pwm config failed");
			return;
		}
	}
}

void vcom_adjust_start(void)
{
	int duty;
	int file;
	char str[10] = {0};

	file = open("/mnt/data/pwm.bin", O_RDONLY);
	if (file != -1) {
		read(file, str, 4);
		close(file);
		duty = atoi((const char *)str);
		if (duty >= 0 && duty <= 50)
			vcom_adjust_pwm0(duty);
	}
}
RTM_EXPORT(vcom_adjust_start);

int vcom_get_duty(void)
{
	int duty;
	int file;
	char str[10] = {0};

	file = open("/mnt/data/pwm.bin", O_RDONLY);
	if (file != -1) {
		read(file, str, 4);
		close(file);
		duty = atoi((const char *)str);
		if (duty >= 0 && duty <= 50)
			return duty;
	}

	return 40;
}
RTM_EXPORT(vcom_get_duty);

void vcom_adjust_save(int duty)
{
	int file;
	char str[10] = {0};

	if (duty >= 0 && duty <= 50) {
		file = open("/mnt/data/pwm.bin", O_WRONLY|O_CREAT);
		if (file != -1) {
			sprintf(str, "%d", duty);
			write(file, str, 4);
			close(file);
			vcom_adjust_pwm0(duty);
		} else {
			LOG_E("vcom_adjust_save failed");
		}
	} else
		LOG_E("vcom_adjust_save failed, %d", duty);
}
RTM_EXPORT(vcom_adjust_save);


static void vcom_adjust_pwm0_cmd(int duty, int period)
{
	rt_err_t ret;
	int pin_num;

	if (duty == -1)
		return;

	LOG_E("VCOM ADJUST: %d", duty);



	if (!pwm) {
		pctrl = pinctrl_get("pwm");
		if (!pctrl) {
			LOG_E("get pinctrl failed");
			return;
		}

		pin_num = pinctrl_gpio_request(pctrl, GPIO_PORT_E, GPIO_PIN_0);
		if (pin_num < 0) {
			LOG_E("port(%d) pin(%d) request failed",
					GPIO_PORT_E, GPIO_PIN_0);
			return;
		}

		ret = pinctrl_gpio_set_function(pctrl, pin_num, 5);
		if (ret < 0) {
			LOG_E("gpio(%d) set function failed", pin_num);
			return;
		}

		pinctrl_put(pctrl);

		pwm = pwm_request(0, "pwm-0");
		if (!pwm) {
			LOG_E("request pwm failed");
			return;
		}

		ret = pwm_set_polarity_normal(pwm);
		if (ret != RT_EOK) {
			LOG_E("pwm set polarity failed");
			return;
		}

		ret = pwm_config(pwm, duty, period);
		if (ret != RT_EOK) {
			LOG_E("pwm config failed");
			return;
		}

		pwm_enable(pwm);
		if (ret != RT_EOK) {
			LOG_E("pwm enable failed");
			return;
		}
	} else {
		ret = pwm_config(pwm, duty, period);
		if (ret != RT_EOK) {
			LOG_E("pwm config failed");
			return;
		}
	}
}

static void vcom_adjust(int argc, char **argv)
{
	int duty, period;

	if (argc != 3)
		return;

	duty = atoi((const char *)argv[1]);
	period = atoi((const char *)argv[2]);

	vcom_adjust_pwm0_cmd(duty, period);
}

#ifdef RT_USING_FINSH
#include <finsh.h>
FINSH_FUNCTION_EXPORT(vcom_adjust, FINSH vcom_adjust);
MSH_CMD_EXPORT(vcom_adjust, MSH vcom_adjust);
#endif

#endif
