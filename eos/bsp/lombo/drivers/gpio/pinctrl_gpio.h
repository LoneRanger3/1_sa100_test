/*
 * pinctrl_gpio.h - Gpio module head file
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

#ifndef __PINCTRL_GPIO_H__
#define __PINCTRL_GPIO_H__

#include <rtconfig.h>
#include "pinctrl_core.h"

/*
 * ------------------------------------------------------------------------------
 * GPIO port
 * ------------------------------------------------------------------------------
 */
enum gpio_port {
	GPIO_PORT_A = 0,
	GPIO_PORT_B,
	GPIO_PORT_C,
	GPIO_PORT_D,
	GPIO_PORT_E,
	GPIO_PORT_SIO,
	GPIO_PORT_F,
	GPIO_PORT_G,
	GPIO_PORT_NUM,
};

/*
 * ------------------------------------------------------------------------------
 * GPIO pin
 * ------------------------------------------------------------------------------
 */
enum gpio_pin {
	GPIO_PIN_0 = 0,
	GPIO_PIN_1,
	GPIO_PIN_2,
	GPIO_PIN_3,
	GPIO_PIN_4,
	GPIO_PIN_5,
	GPIO_PIN_6,
	GPIO_PIN_7,
	GPIO_PIN_8,
	GPIO_PIN_9,
	GPIO_PIN_10,
	GPIO_PIN_11,
	GPIO_PIN_12,
	GPIO_PIN_13,
	GPIO_PIN_14,
	GPIO_PIN_15,
	GPIO_PIN_16,
	GPIO_PIN_17,
	GPIO_PIN_18,
	GPIO_PIN_19,
	GPIO_PIN_20,
	GPIO_PIN_21,
	GPIO_PIN_22,
	GPIO_PIN_23,
	GPIO_PIN_24,
	GPIO_PIN_25,
	GPIO_PIN_26,
	GPIO_PIN_27,
	GPIO_PIN_NUM,
};

/*
 * ------------------------------------------------------------------------------
 * Driver Level
 * ------------------------------------------------------------------------------
 */
enum gpio_drv_level {
	DRV_LEVEL_0 = 0,
	DRV_LEVEL_1,
	DRV_LEVEL_2,
	DRV_LEVEL_3,
	DRV_LEVEL_4,
	DRV_LEVEL_5,
	DRV_LEVEL_6,
	DRV_LEVEL_7,
	DRV_LEVEL_MAX
};

/*
 * ------------------------------------------------------------------------------
 * Pull-Up or Pull-Down
 * ------------------------------------------------------------------------------
 */
enum gpio_pud {
	PUD_DISABLE = 0,
	PULL_UP,
	PULL_DOWN,
	PUD_MAX
};

/*
 * ------------------------------------------------------------------------------
 * Pull Resistor val
 * ------------------------------------------------------------------------------
 */
enum gpio_pud_res {
	PUD_RES_100KO = 0,
	PUD_RES_20KO,
	PUD_RES_MAX
};

/*
 * ------------------------------------------------------------------------------
 * Multi-function
 * ------------------------------------------------------------------------------
 */
enum gpio_function {
	FUNC0 = 0,
	FUNC1,
	FUNC2,
	FUNC3,
	FUNC4,
	FUNC5,
	FUNC6,
	FUNC7,
	FUN_MAX
};

/*
 * ------------------------------------------------------------------------------
 * Interrupt type
 * ------------------------------------------------------------------------------
 */
enum gpio_irq_type {
	POSITIVE = 0,
	NEGATIVE,
	HIGH,
	LOW,
	DOUBLE,
};

/*
 * ------------------------------------------------------------------------------
 * GPIO eint clock src
 * ------------------------------------------------------------------------------
 */
enum gpio_eint_clock_src {
	GPIO_IRQ_LOSC_32KHZ = 0,
	GPIO_IRQ_HOSC_24MHZ,
};

/**
 * enum eint_trig_type - bank external interrupt trig types
 * @EINT_TRIG_LOW_LEVEL: eint trigged by low level signals
 * @EINT_TRIG_HIGH_LEVEL: eint trigged by high level signals
 * @EINT_TRIG_RISING_EDGE: eint trigged by rising edge signals
 * @EINT_TRIG_FALLING_EDGE: eint trigged by falling edge signals
 * @EINT_TRIG_BOTH_EDGE: eint trigged by both rising and falling
 * These bits' arrangement are agree with the spec. For example,
 * EINT_TRIG_HIGH_LEVEL's value is 1, which indicate triggered by high level in
 * GPIO_External_Interrupt_Trigger_Configuration registers
 */
enum gpio_eint_trig_type {
	EINT_TRIG_LOW_LEVEL,
	EINT_TRIG_HIGH_LEVEL,
	EINT_TRIG_RISING_EDGE,
	EINT_TRIG_FALLING_EDGE,
	EINT_TRIG_BOTH_EDGE
};

/**
 * struct gpio_irq_data - irq data
 */
struct gpio_irq_data {
	enum gpio_eint_trig_type trig_type;
	enum gpio_eint_clock_src clock_src;
	int clock_src_div;
	irq_handler_t handler;
	void *irq_arg;
};

/**
 * struct gpio_ops - abstract a GPIO controller
 *
 * A gpio_ops can help platforms abstract various sources of GPIOs so
 * they can all be accessed through a common programing interface.
 * Example sources would be SOC controllers, FPGAs, multifunction
 * chips, dedicated GPIO expanders, and so on.
 */
struct pinconf_ops {
	int		(*request_gpio)(struct pinctrl *pctrl,
				enum gpio_port port, enum gpio_pin pin);
	rt_err_t	(*free_gpio)(struct pinctrl *pctrl, int gpio);
	rt_err_t	(*direction_input)(struct pinctrl *pctrl, int gpio);
	rt_err_t	(*direction_output)(struct pinctrl *pctrl, int gpio);
	rt_err_t	(*set_value)(struct pinctrl *pctrl,
				int gpio, u32 value);
	int		(*get_value)(struct pinctrl *pctrl,
				int gpio);
	rt_err_t	(*set_pud_mode)(struct pinctrl *pctrl,
				int gpio, enum gpio_pud mode);
	rt_err_t	(*set_pud_res)(struct pinctrl *pctrl,
				int gpio, enum gpio_pud_res reg);
	rt_err_t	(*set_drv_level)(struct pinctrl *pctrl,
				int gpio, enum gpio_drv_level level);
	rt_err_t	(*set_function)(struct pinctrl *pctrl,
				int gpio, u32 func);
	rt_err_t	(*request_irq)(struct pinctrl *pctrl,
				int gpio, struct gpio_irq_data *irq_data);
	rt_err_t	(*free_irq)(struct pinctrl *pctrl,
				int gpio);
	rt_err_t	(*set_irq_trig_type)(struct pinctrl *pctrl,
				int gpio, struct gpio_irq_data *irq_data);
	rt_err_t	(*irq_disable)(struct pinctrl *pctrl, int gpio);
};

#endif/* __PINCTRL_GPIO_H__ */

