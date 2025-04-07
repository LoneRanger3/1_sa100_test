/*
 * pwm.h - Pwm module head file
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

#ifndef __LOMBO_PWM_H__
#define __LOMBO_PWM_H__

#include <rtdef.h>
#include "pwm_core.h"

struct pwm_device *pwm_request(unsigned int channel,
					const char *module_name);

struct pwm_device *pwm_cfg_request(const char *module_name,
					const char *pwm_group);

void pwm_free(struct pwm_device *pwm);

rt_err_t pwm_config(struct pwm_device *pwm, int duty_ns, int period_ns);

rt_err_t pwm_set_polarity_normal(struct pwm_device *pwm);

rt_err_t pwm_set_polarity_inversed(struct pwm_device *pwm);

rt_err_t pwm_enable(struct pwm_device *pwm);

void pwm_disable(struct pwm_device *pwm);

void pwm_dump(void);

#endif/* __LOMBO_PWM_H__ */

