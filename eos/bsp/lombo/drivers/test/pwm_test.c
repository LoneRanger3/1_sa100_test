/*
 * pwm_test.c - PWM test driver
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

#define DBG_SECTION_NAME	"PWM"
#define DBG_LEVEL		DBG_LOG
#include <debug.h>

#include <rtthread.h>

#include "soc_define.h"
#include "pwm/pwm.h"
#include "gpio/pinctrl.h"

#define PWM_MODULE_NAME		"pwm-test"
#define PWM_TEST_GROUP_NAME	"pwm0-test"
#define PWM_TEST_GPIO_GROUP	"pwm0-test-gpio"
#define PWM_TEST_CHANNEL	0

#define PWM_TEST_DUTY		(2 * 1000000)
#define PWM_TEST_ADJUST		(3 * 1000000)
#define PWM_TEST_PERIOD		(10 * 1000000)

#define PWM_TEST_PORT	GPIO_PORT_B
#define PWM_TEST_PIN	GPIO_PIN_6

typedef rt_err_t  (*test_func)(void);

rt_err_t test_pwm_standalone(void)
{
	struct pwm_device *pwm0, *pwm1;

	LOG_D("test_pwm_standalone");

	pwm0 = pwm_request(PWM_TEST_CHANNEL, PWM_MODULE_NAME);
	if (!pwm0)
		return -RT_EBUSY;

	/* Return null if request twice */
	pwm1 = pwm_request(PWM_TEST_CHANNEL, PWM_MODULE_NAME);
	if (pwm1)
		return -RT_ERROR;

	pwm_free(pwm0);

	return RT_EOK;
}

rt_err_t test_pwm_reuse(void)
{
	struct pwm_device *pwm;

	LOG_D("test_pwm_reuse");

	pwm = pwm_request(PWM_TEST_CHANNEL, PWM_MODULE_NAME);
	if (!pwm)
		return -RT_EBUSY;

	pwm_free(pwm);

	pwm = pwm_request(PWM_TEST_CHANNEL, PWM_MODULE_NAME);
	if (!pwm)
		return -RT_EBUSY;

	pwm_free(pwm);

	return RT_EOK;
}

rt_err_t test_pwm_request(void)
{
	struct pwm_device *pwm;
	struct pinctrl *pctrl;
	rt_err_t ret;
	int pin_num;

	LOG_D("test_pwm_request");

	pctrl = pinctrl_get("pwm");
	if (!pctrl) {
		LOG_E("get pinctrl failed");
		return -RT_EINTR;
	}

	pin_num = pinctrl_gpio_request(pctrl, PWM_TEST_PORT, PWM_TEST_PIN);
	if (pin_num < 0) {
		LOG_E("port(%d) pin(%d) request failed",
				PWM_TEST_PORT, PWM_TEST_PIN);
		return -RT_EINTR;
	}

	ret = pinctrl_gpio_set_function(pctrl, pin_num, 6);
	if (ret < 0) {
		LOG_E("gpio(%d) set function failed", pin_num);
		return -RT_EINTR;
	}

	pinctrl_put(pctrl);

	pwm = pwm_request(PWM_TEST_CHANNEL, PWM_MODULE_NAME);
	if (!pwm) {
		LOG_E("request pwm failed");
		return -RT_EBUSY;
	}

	ret = pwm_set_polarity_normal(pwm);
	if (ret != RT_EOK) {
		LOG_E("pwm set polarity failed");
		return -RT_EINTR;
	}

	ret = pwm_config(pwm, PWM_TEST_DUTY, PWM_TEST_PERIOD);
	if (ret != RT_EOK) {
		LOG_E("pwm config failed");
		return -RT_EINVAL;
	}

	pwm_enable(pwm);
	if (ret != RT_EOK) {
		LOG_E("pwm enable failed");
		return -RT_EINTR;
	}

	rt_thread_delay(100);

	ret = pwm_config(pwm, PWM_TEST_DUTY + PWM_TEST_ADJUST, PWM_TEST_PERIOD);
	if (ret != RT_EOK) {
		LOG_E("pwm config failed");
		return -RT_EINVAL;
	}

	rt_thread_delay(100);
	pwm_disable(pwm);
	pwm_free(pwm);

	return RT_EOK;
}


rt_err_t test_pwm_cfg_request(void)
{
	struct pwm_device *pwm;
	struct pinctrl *pctrl;
	rt_err_t ret;

	LOG_D("test_pwm_request");

	pctrl = pinctrl_get(PWM_MODULE_NAME);
	if (!pctrl)
		return -RT_EBUSY;

	ret = pinctrl_enable_group(pctrl, PWM_TEST_GPIO_GROUP);
	if (ret != RT_EOK)
		return -RT_EEMPTY;

	pwm = pwm_cfg_request(PWM_MODULE_NAME, PWM_TEST_GROUP_NAME);
	if (!pwm) {
		LOG_E("pwm_cfg_request failed");
		return -RT_ENOSYS;
	}

	pwm_enable(pwm);
	pinctrl_put(pctrl);

	return RT_EOK;
}

static test_func test_funcs[] = {
	test_pwm_standalone,
	test_pwm_reuse,
	test_pwm_request,
	test_pwm_cfg_request,
};

long test_pwm(int argc, char **argv)
{
	rt_err_t ret;
	int i;

	LOG_D("Test pwm start...");

	for (i = 0; i < ARRAY_SIZE(test_funcs); i++) {
		ret = test_funcs[i]();
		if (ret != RT_EOK)
			return -1;
	}

	LOG_D("Test pwm successfully");

	return 0;
}

