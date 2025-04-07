/*
 * pinctrl.h - Gpio module head file
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

#ifndef __PINCTRL_H__
#define __PINCTRL_H__

#include "pinctrl_core.h"
#include "pinctrl_gpio.h"

struct pinctrl *pinctrl_get(const char *module_name);
void pinctrl_put(struct pinctrl *pctrl);

/* Group operations */
rt_err_t pinctrl_enable_group(struct pinctrl *pctrl,
				const char *group_name);
rt_err_t pinctrl_disable_group(struct pinctrl *pctrl,
				const char *group_name);

/* GPIO operations */
int pinctrl_gpio_request(struct pinctrl *pctrl,
			enum gpio_port port, enum gpio_pin pin);

rt_err_t pinctrl_gpio_free(struct pinctrl *pctrl, int gpio);

rt_err_t pinctrl_gpio_direction_input(struct pinctrl *pctrl, int gpio);

rt_err_t pinctrl_gpio_direction_output(struct pinctrl *pctrl, int gpio);

rt_err_t pinctrl_gpio_set_value(struct pinctrl *pctrl, int gpio, u32 value);

int pinctrl_gpio_get_value(struct pinctrl *pctrl, int gpio);

rt_err_t pinctrl_gpio_set_pud_mode(struct pinctrl *pctrl, int gpio,
					enum gpio_pud mode);

rt_err_t pinctrl_gpio_set_pud_res(struct pinctrl *pctrl, int gpio,
					enum gpio_pud_res res);

rt_err_t pinctrl_gpio_set_drv_level(struct pinctrl *pctrl, int gpio,
					enum gpio_drv_level level);

rt_err_t pinctrl_gpio_set_function(struct pinctrl *pctrl, int gpio,
					u32 func);

rt_err_t pinctrl_gpio_request_irq(struct pinctrl *pctrl, int gpio,
					struct gpio_irq_data *irq_data);

rt_err_t pinctrl_gpio_free_irq(struct pinctrl *pctrl, int gpio);


rt_err_t pinctrl_gpio_set_irq_trig_type(struct pinctrl *pctrl, int gpio,
					struct gpio_irq_data *irq_data);


rt_err_t pinctrl_gpio_irq_disable(struct pinctrl *pctrl, int gpio);

void pinctrl_dump_gpio(void);

int rt_hw_gpio_init(void);

#endif/* __PINCTRL_H__ */

