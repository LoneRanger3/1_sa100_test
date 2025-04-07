/*
 * pwm_core.c - Pwm driver for LomboTech Socs
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
#define DBG_LEVEL		DBG_INFO
#include <debug.h>

#include <rthw.h>
#include <rtdevice.h>
#include <csp.h>

#include "pwm.h"
#include "pwm_drv.h"
#include "cfg/config_api.h"

/**
 * pwm_config() - change a PWM device configuration
 * @pwm: PWM device
 * @duty_ns: "on" time (in nanoseconds)
 * @period_ns: duration (in nanoseconds) of one cycle
 */
rt_err_t pwm_config(struct pwm_device *pwm, int duty_ns, int period_ns)
{
	RT_ASSERT(pwm != RT_NULL);
	RT_ASSERT(pwm->chip != RT_NULL && pwm->chip->pwmops != RT_NULL);
	RT_ASSERT(duty_ns >= 0 && period_ns > 0 && duty_ns <= period_ns);

	return pwm->chip->pwmops->config(pwm, duty_ns, period_ns);
}

/**
 * pwm_set_polarity() - configure the polarity of a PWM signal
 * @pwm: PWM device
 * @polarity: new polarity of the PWM signal
 *
 * Note that the polarity cannot be configured while the PWM device is enabled
 */
static rt_err_t pwm_set_polarity(struct pwm_device *pwm,
					enum pwm_polarity polarity)
{
	RT_ASSERT(pwm != RT_NULL);
	RT_ASSERT(pwm->chip != RT_NULL && pwm->chip->pwmops != RT_NULL);

	return pwm->chip->pwmops->set_polarity(pwm, polarity);
}

rt_err_t pwm_set_polarity_normal(struct pwm_device *pwm)
{
	return pwm_set_polarity(pwm, PWM_POLARITY_NORMAL);
}

rt_err_t pwm_set_polarity_inversed(struct pwm_device *pwm)
{
	return pwm_set_polarity(pwm, PWM_POLARITY_INVERSED);
}

/**
 * pwm_request() - request a PWM device
 * @channel: global PWM device index
 * @label: PWM device label
 *
 * This function is deprecated, use pwm_get() instead.
 */
struct pwm_device *pwm_request(unsigned int channel,
					const char *module_name)
{
	struct pwm_device *dev = RT_NULL;
	struct pwm_chip *chip;

	chip = get_pwm_chip();
	RT_ASSERT(chip != RT_NULL && chip->pwmops != RT_NULL);

	if (chip->pwmops->request) {
		dev = chip->pwmops->request(chip, channel, module_name);
		if (!dev) {
			LOG_E("request channel %d failed", channel);
			return RT_NULL;
		}
	}

	return dev;
}

struct pwm_device *pwm_cfg_request(const char *module_name,
					const char *pwm_group)
{
	int count;
	u32 cfg_data[PWM_CONFIG_NUM];
	rt_err_t ret;
	struct pwm_device *pwm = RT_NULL;

	count = config_get_u32_array(module_name, pwm_group,
					cfg_data, PWM_CONFIG_NUM);
	if (count != PWM_CONFIG_NUM) {
		LOG_E("config get failed");
		return RT_NULL;
	}

	pwm = pwm_request(cfg_data[0], module_name);
	if (!pwm)
		return RT_NULL;

	ret = pwm_set_polarity(pwm, cfg_data[1]);
	if (ret != RT_EOK)
		return RT_NULL;

	ret = pwm_config(pwm, (int)cfg_data[2], (int)cfg_data[3]);
	if (ret != RT_EOK)
		return RT_NULL;

	return pwm;
}

/**
 * pwm_put() - release a PWM device
 * @pwm: PWM device
 */
void pwm_free(struct pwm_device *pwm)
{
	RT_ASSERT(pwm != RT_NULL);
	RT_ASSERT(pwm->chip != RT_NULL && pwm->chip->pwmops != RT_NULL);

	if (pwm->chip->pwmops->free)
		pwm->chip->pwmops->free(pwm);
}

/**
 * pwm_enable() - start a PWM output toggling
 * @pwm: PWM device
 */
rt_err_t pwm_enable(struct pwm_device *pwm)
{
	RT_ASSERT(pwm != RT_NULL);
	RT_ASSERT(pwm->chip != RT_NULL && pwm->chip->pwmops != RT_NULL);

	return pwm->chip->pwmops->enable(pwm);
}

/**
 * pwm_disable() - stop a PWM output toggling
 * @pwm: PWM device
 */
void pwm_disable(struct pwm_device *pwm)
{
	RT_ASSERT(pwm != RT_NULL);
	RT_ASSERT(pwm->chip != RT_NULL && pwm->chip->pwmops != RT_NULL);

	return pwm->chip->pwmops->disable(pwm);
}

void pwm_dump(void)
{
	/* dump_pwm(DUMP_TYPE_READ); */
}

