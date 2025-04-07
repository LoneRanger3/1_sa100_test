/*
 * pinctrl_test.c - Gpio driver for LomboTech Socs
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

#define DBG_SECTION_NAME	"GPIO"
#define DBG_LEVEL		DBG_LOG
#include <debug.h>

#include "gpio/pinctrl.h"

#define TEST_PORT	GPIO_PORT_C
#define TEST_PIN	GPIO_PIN_20

#define TEST_INT_PORT	GPIO_PORT_SIO
#define TEST_INT_PIN	GPIO_PIN_7

#define TEST_MODULE_NAME_ERROR	"testnametestname"
#define TEST_MODULE_NAME0	"module0"
#define TEST_MODULE_NAME1	"module1"

#define TEST_MAP_MODULE		"pinctrl"
#define TEST_MAP_GROUP		"pinctrl-test"

typedef rt_err_t  (*test_func)(void);

static rt_err_t test_pinctrl_name(void)
{
	struct pinctrl *pctrl;

	pctrl = pinctrl_get(TEST_MODULE_NAME_ERROR);
	if (pctrl != RT_NULL) {
		LOG_E("Test module name failed", TEST_MODULE_NAME_ERROR);
		return -RT_ERROR;
	}

	LOG_I("Test pinctrl name pass");

	return RT_EOK;
}

static rt_err_t test_get_pinctrl(void)
{
	struct pinctrl *pctrl0, *pctrl1;

	pctrl0 = pinctrl_get(TEST_MODULE_NAME0);
	if (pctrl0 == RT_NULL) {
		LOG_E("Failed to get pinctrl for %s", TEST_MODULE_NAME0);
		return -RT_ERROR;
	}

	pctrl1 = pinctrl_get(TEST_MODULE_NAME0);
	if (pctrl0 == RT_NULL) {
		LOG_E("Failed to get pinctrl for %s", TEST_MODULE_NAME0);
		return -RT_ERROR;
	}

	if (pctrl0 != pctrl1) {
		LOG_E("Test get pinctrl failed");
		return -RT_ERROR;
	}

	pinctrl_put(pctrl0);

	LOG_I("Test get pinctrl pass");

	return RT_EOK;
}

static rt_err_t test_group_stone_alone(void)
{
	/* enable group */

	/* request the gpio of group */

	return RT_EOK;
}

static rt_err_t test_gpio_stone_alone(void)
{
	struct pinctrl *pctrl0, *pctrl1;
	int pin_num;

	pctrl0 = pinctrl_get(TEST_MODULE_NAME0);
	if (pctrl0 == RT_NULL) {
		LOG_E("Failed to get pinctrl for %s", TEST_MODULE_NAME0);
		return -RT_ERROR;
	}

	pin_num = pinctrl_gpio_request(pctrl0, TEST_PORT, TEST_PIN);
	if (pin_num < 0) {
		LOG_E("Failed to request port %d pin %d", TEST_PORT, TEST_PIN);
		return -RT_ERROR;
	}

	pctrl1 = pinctrl_get(TEST_MODULE_NAME1);
	if (pctrl1 == RT_NULL) {
		LOG_E("Failed to get pinctrl for %s", TEST_MODULE_NAME1);
		return -RT_ERROR;
	}

	pin_num = pinctrl_gpio_request(pctrl1, TEST_PORT, TEST_PIN);
	if (pin_num >= 0) {
		LOG_E("Test stone alone failed");
		return -RT_ERROR;
	}

	pinctrl_put(pctrl0);
	pinctrl_put(pctrl1);

	LOG_I("Test stone alone pass");

	return RT_EOK;
}

static rt_err_t test_gpio_repeat_request(void)
{
	struct pinctrl *pctrl;
	int pin_num0, pin_num1;

	pctrl = pinctrl_get(TEST_MODULE_NAME0);
	if (pctrl == RT_NULL) {
		LOG_E("Failed to get pinctrl for %s", TEST_MODULE_NAME0);
		return -RT_ERROR;
	}

	pin_num0 = pinctrl_gpio_request(pctrl, TEST_PORT, TEST_PIN);
	if (pin_num0 < 0) {
		LOG_E("Failed to request port %d pin %d", TEST_PORT, TEST_PIN);
		return -RT_ERROR;
	}

	pin_num1 = pinctrl_gpio_request(pctrl, TEST_PORT, TEST_PIN);
	if (pin_num1 < 0) {
		LOG_E("Failed to request port %d pin %d", TEST_PORT, TEST_PIN);
		return -RT_ERROR;
	}

	if (pin_num0 != pin_num1) {
		LOG_E("Test stone alone failed");
		return -RT_ERROR;
	}

	pinctrl_put(pctrl);

	LOG_I("Test gpio repeat request pass");

	return RT_EOK;
}

static rt_err_t test_gpio_interface(void)
{
	struct pinctrl *pctrl;
	int pin_num, ret, val;

	pctrl = pinctrl_get(TEST_MODULE_NAME0);
	if (pctrl == RT_NULL) {
		LOG_E("Failed to get pinctrl");
		return -RT_ERROR;
	}

	pin_num = pinctrl_gpio_request(pctrl, TEST_PORT, TEST_PIN);
	if (pin_num < 0) {
		LOG_E("Failed to request gpio");
		return -RT_ERROR;
	}

	ret = pinctrl_gpio_set_drv_level(pctrl, pin_num, DRV_LEVEL_2);
	if (ret != RT_EOK) {
		LOG_E("Can't set drv");
		return -RT_ERROR;
	}

	ret = pinctrl_gpio_set_pud_mode(pctrl, pin_num, PULL_UP);
	if (ret != RT_EOK) {
		LOG_E("Can't set drv");
		return -RT_ERROR;
	}

	ret = pinctrl_gpio_set_pud_res(pctrl, pin_num, 1);
	if (ret != RT_EOK) {
		LOG_E("Can't set pud res");
		return -RT_ERROR;
	}

	ret = pinctrl_gpio_set_drv_level(pctrl, pin_num, DRV_LEVEL_2);
	if (ret != RT_EOK) {
		LOG_E("Can't set drv levle");
		return -RT_ERROR;
	}

	ret = pinctrl_gpio_direction_output(pctrl, pin_num);
	if (ret != RT_EOK) {
		LOG_E("Can't set direction");
		return -RT_ERROR;
	}

	ret = pinctrl_gpio_set_value(pctrl, pin_num, 1);
	if (ret != RT_EOK) {
		LOG_E("Can't set value");
		return -RT_ERROR;
	}

	ret = pinctrl_gpio_direction_input(pctrl, pin_num);
	if (ret != RT_EOK) {
		LOG_E("Can't set direction");
		return -RT_ERROR;
	}

	val = pinctrl_gpio_get_value(pctrl, pin_num);
	if (val < 0) {
		LOG_E("Can't get value");
		return -RT_ERROR;
	}

	/*if (val != 1) {
		LOG_E("Value is unexpected");
		return -RT_ERROR;
	}*/

	ret = pinctrl_gpio_set_function(pctrl, pin_num, FUNC3);
	if (ret != RT_EOK) {
		LOG_E("Can't set function");
		return -RT_ERROR;
	}

	pinctrl_put(pctrl);

	LOG_I("Test gpio interface successfully");

	return RT_EOK;
}

static void gpio_irq_handler(void *data)
{
	LOG_I("I am gpio irq handler");
	LOG_I("You are in interrupt context, don't sleep or block here");
}

static rt_err_t test_gpio_interrupt(void)
{
	struct pinctrl *pctrl;
	int pin_num, ret;
	struct gpio_irq_data irq_data;

	pctrl = pinctrl_get(TEST_MODULE_NAME0);
	if (pctrl == RT_NULL) {
		LOG_E("Failed to get pinctrl");
		return -RT_ERROR;
	}

	pin_num = pinctrl_gpio_request(pctrl, TEST_PORT, TEST_PIN);
	if (pin_num < 0) {
		LOG_E("Failed to request gpio");
		return -RT_ERROR;
	}

	irq_data.handler = gpio_irq_handler;
	irq_data.irq_arg = NULL;
	irq_data.clock_src = GPIO_IRQ_HOSC_24MHZ;
	irq_data.clock_src_div = 10;
	irq_data.trig_type = EINT_TRIG_HIGH_LEVEL;
	ret = pinctrl_gpio_request_irq(pctrl, pin_num, &irq_data);
	if (RT_EOK == ret) {
		LOG_E("Port: %d, pin: %d should not support interrupt",
				TEST_PORT, TEST_PIN);
		return -RT_ERROR;
	}

	pinctrl_gpio_free(pctrl, pin_num);

	pin_num = pinctrl_gpio_request(pctrl, TEST_INT_PORT, TEST_INT_PIN);
	if (pin_num < 0) {
		LOG_E("Failed to request gpio");
		return -RT_ERROR;
	}

	ret = pinctrl_gpio_set_pud_mode(pctrl, pin_num, PULL_UP);
	if (ret != RT_EOK) {
		LOG_E("Can't set drv");
		return -RT_ERROR;
	}

	ret = pinctrl_gpio_direction_input(pctrl, pin_num);
	if (ret != RT_EOK) {
		LOG_E("Can't set direction");
		return -RT_ERROR;
	}

	irq_data.handler = gpio_irq_handler;
	irq_data.irq_arg = RT_NULL;
	irq_data.clock_src = GPIO_IRQ_HOSC_24MHZ;
	irq_data.clock_src_div = 10;
	irq_data.trig_type = EINT_TRIG_RISING_EDGE;
	ret = pinctrl_gpio_request_irq(pctrl, pin_num, &irq_data);
	if (ret != RT_EOK) {
		LOG_E("Failed to request irq");
		return -RT_ERROR;
	}

	ret = pinctrl_gpio_free_irq(pctrl, pin_num);
	if (ret != RT_EOK) {
		LOG_E("Failed to request irq");
		return -RT_ERROR;
	}

	pinctrl_put(pctrl);

	LOG_I("Test gpio interrupt successfully");

	return RT_EOK;
}

static rt_err_t test_gpio_config(void)
{
	struct pinctrl *pctrl;
	rt_err_t ret;

	LOG_I("test gpio config");

	pctrl = pinctrl_get(TEST_MAP_MODULE);
	if (!pctrl)
		return -RT_ENOSYS;

	ret = pinctrl_enable_group(pctrl, TEST_MAP_GROUP);
	if (ret)
		return -RT_ENOSYS;

	ret = pinctrl_disable_group(pctrl, TEST_MAP_GROUP);
	if (ret)
		return -RT_ENOSYS;

	pinctrl_put(pctrl);

	LOG_I("test gpio config successfully");

	return RT_EOK;
}

static rt_err_t test_gpio_mem_leak(void)
{
	struct pinctrl *pctrl;
	u32 total, used_before, used_after, max_used;
	rt_err_t ret;

	LOG_I("test gpio mem");

	/* Check pinctrl_get and pinctrl_put */
	rt_memory_info(&total, &used_before, &max_used);

	pctrl = pinctrl_get(TEST_MAP_MODULE);
	if (!pctrl)
		return -RT_ENOSYS;
	pinctrl_put(pctrl);

	rt_memory_info(&total, &used_after, &max_used);
	if (used_before != used_after) {
		LOG_E("There is a memory leak maybe, used before:%x, used after:%x",
					used_before, used_after);
		return -RT_ERROR;
	}

	/* Check pinctrl_enable_group and pinctrl_disable_group */
	rt_memory_info(&total, &used_before, &max_used);

	pctrl = pinctrl_get(TEST_MAP_MODULE);
	if (!pctrl)
		return -RT_ENOSYS;

	ret = pinctrl_enable_group(pctrl, TEST_MAP_GROUP);
	if (ret)
		return -RT_ENOSYS;

	ret = pinctrl_disable_group(pctrl, TEST_MAP_GROUP);
	if (ret)
		return -RT_ENOSYS;

	pinctrl_put(pctrl);

	rt_memory_info(&total, &used_after, &max_used);
	if (used_before != used_after) {
		LOG_E("There is a memory leak maybe, used before:%x, used after:%x",
					used_before, used_after);
		return -RT_ERROR;
	}

	LOG_I("test gpio mem successfully");

	return RT_EOK;
}

static test_func gpio_test_funcs[] = {
	test_pinctrl_name,
	test_get_pinctrl,
	test_gpio_interface,
	test_group_stone_alone,
	test_gpio_stone_alone,
	test_gpio_repeat_request,
	test_gpio_interrupt,
	test_gpio_config,
	test_gpio_mem_leak
};

long test_gpio(int argc, char **argv)
{
	rt_err_t ret;
	int i;

	LOG_I("Test gpio start...");

	for (i = 0; i < ARRAY_SIZE(gpio_test_funcs); i++) {
		ret = gpio_test_funcs[i]();
		if (ret != RT_EOK)
			return -1;
	}

	LOG_I("Test gpio successfully");

	return 0;
}

