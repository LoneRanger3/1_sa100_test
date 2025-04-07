/*
 * pwm_irq.h - Pwm module head file
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

#ifndef __LOMBO_PWM_IRQ_H__
#define __LOMBO_PWM_IRQ_H__

#include "pwm_core.h"

typedef	void (*pwm_irq_handler_t)(void *data);

struct pwm_irq {
	pwm_irq_handler_t handler;
	void *irq_data;
};

struct pwm_irq_data {
	struct pwm_irq *pwm_irqs;
	int irq;
	void *reg;
};

int pwm_irq_init(struct pwm_chip *chip);

#endif/* __LOMBO_PWM_IRQ_H__ */

