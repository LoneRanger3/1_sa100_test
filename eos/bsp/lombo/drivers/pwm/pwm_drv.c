/*
 * pwm_drv.c - Pwm driver for LomboTech Socs
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

#include "pwm_csp.h"
#include "pwm_drv.h"
#include "pwm_irq.h"

#include "csp.h"
#include "div.h"
#include "bitops.h"

#include "irq_numbers.h"
#include "cfg/config_api.h"

static struct pwm_chip *pwm_chip_drv;
static rt_mutex_t pwm_lock;

static u32 pwm_cfg_get_clk_src(void)
{
	int ret;
	u32 clk_src;

	ret = config_get_u32(PWM_CONFIG_NAME, "clock-src", &clk_src);
	if (ret) {
		LOG_E("config get failed");
		return -1;
	}

	LOG_D("pwm clock src: %d", clk_src);

	return clk_src;
}

static u32 pwm_cfg_get_clk_div(void)
{
	int ret;
	u32 clk_div;

	ret = config_get_u32(PWM_CONFIG_NAME, "clock-div", &clk_div);
	if (ret) {
		LOG_E("config get failed");
		return -1;
	}

	LOG_D("pwm clock div: %d", clk_div);

	return clk_div;
}

struct pwm_chip *get_pwm_chip(void)
{
	return pwm_chip_drv;
}

/**
 * pwm_set_chip_data() - set private chip data for a PWM
 * @pwm: PWM device
 * @data: pointer to chip-specific data
 */
static int pwm_set_chip_data(struct pwm_device *pwm, void *data)
{
	if (!pwm)
		return -EINVAL;

	pwm->chip_data = data;

	return 0;
}

/**
 * pwm_get_chip_data() - get private chip data for a PWM
 * @pwm: PWM device
 */
static void *pwm_get_chip_data(struct pwm_device *pwm)
{
	return pwm ? pwm->chip_data : NULL;
}

static struct pwm_device *pwm_to_device(struct pwm_chip *chip,
					unsigned int pwm)
{
	if (chip)
		return &chip->pwms[pwm];

	return RT_NULL;
}

static struct pwm_device *lombo_pwm_request(struct pwm_chip *chip,
					unsigned int pwm_num,
					const char *module_name)
{
	struct pwm_data *pdata = RT_NULL;
	struct pwm_device *pwm = RT_NULL;

	RT_ASSERT(pwm_num < chip->npwm);

	rt_mutex_take(pwm_lock, RT_WAITING_FOREVER);

	pwm = pwm_to_device(chip, pwm_num);
	if (!pwm) {
		LOG_E("get pwm %d failed", pwm_num);
		goto err;
	}

	if (test_bit(PWMF_REQUESTED, &pwm->flags)) {
		LOG_E("channel has been requested by %s", pwm->owner);
		goto err;
	}

	pdata = rt_zalloc(sizeof(struct pwm_data));
	if (!pdata) {
		LOG_E("alloc pwm_data failed\n");
		goto err;
	}

	pwm_set_chip_data(pwm, pdata);

	set_bit(PWMF_REQUESTED, &pwm->flags);
	pwm->owner = module_name;

	rt_mutex_release(pwm_lock);
	return pwm;
err:
	rt_mutex_release(pwm_lock);
	if (pdata)
		rt_free(pdata);
	return RT_NULL;
}

static void lombo_pwm_free(struct pwm_device *pwm)
{
	struct pwm_data *pdata = pwm_get_chip_data(pwm);

	rt_mutex_take(pwm_lock, RT_WAITING_FOREVER);

	if (!test_and_clear_bit(PWMF_REQUESTED, &pwm->flags)) {
		LOG_E("PWM device already freed\n");
		goto out;
	}

	rt_free(pdata);
	pwm_set_chip_data(pwm, RT_NULL);
	pwm->owner = RT_NULL;

out:
	rt_mutex_release(pwm_lock);
}

static rt_err_t lombo_pwm_enable(struct pwm_device *pwm)
{
	if (!test_and_set_bit(PWMF_ENABLED, &pwm->flags))
		csp_pwm_cnt_endisable(pwm->hwpwm, pwm->chip->reg, RT_TRUE);

	return RT_EOK;
}

static void lombo_pwm_disable(struct pwm_device *pwm)
{
	if (test_and_clear_bit(PWMF_ENABLED, &pwm->flags))
		csp_pwm_cnt_endisable(pwm->hwpwm, pwm->chip->reg, RT_FALSE);
}

static rt_err_t lombo_pwm_set_polarity(struct pwm_device *pwm,
				      enum pwm_polarity polarity)
{
	struct pwm_data *pdata = pwm_get_chip_data(pwm);
	struct pwm_chip *chip = pwm->chip;

	if (test_bit(PWMF_ENABLED, &pwm->flags))
		return -RT_EBUSY;

	pdata->polarity = polarity;
	csp_pwm_control(pwm->hwpwm, PWM_PO_POLARITY_SET,
		chip->reg, (u32)polarity);
	csp_pwm_control(pwm->hwpwm, PWM_PO_EN,
		chip->reg, (u32)1);

	return RT_EOK;
}

/**
 * convert_ns_to_cnt - convert ns to pwm count
 * @clk_rate: pwm's clk rate, that is, pwm count for 1s
 * @ns: nanosecond count
 *
 * return pwm count corresponding to ns
 */
static unsigned int convert_ns_to_cnt(int clk_rate, int ns)
{
	u64 tmp = (u64)ns * clk_rate;

	do_div(tmp, NSEC_PER_SEC);

	return (unsigned int)tmp;
}

static rt_err_t lombo_pwm_config(struct pwm_device *pwm,
				int duty_ns, int period_ns)
{
	struct pwm_data *pdata = pwm_get_chip_data(pwm);
	struct pwm_chip *chip = pwm->chip;
	struct n7_pwm_clk_cfg cfg;
	int clk_rate, duty_cnt, period_cnt;

	cfg.bypass = 0;
	cfg.gating = 1;
	cfg.src = pwm_cfg_get_clk_src();
	cfg.div = pwm_cfg_get_clk_div();
	csp_pwm_clk_config((int)pwm->hwpwm, chip->reg, &cfg);
	LOG_D("clock src: %d, clk div: %d", cfg.src, cfg.div);

	/* Attention:
	* Clock ctrl register has to be written twice, because for the first time written,
	* there is no clock and div config is invalid.
	* Only when clock gate has been opened, div config is valid.
	*/
	barrier();
	csp_pwm_clk_config((int)pwm->hwpwm, chip->reg, &cfg);

	/* get clock rate, and calca duty count and period count */
	clk_rate = csp_pwm_clk_get_rate(pwm->hwpwm, chip->reg);
	duty_cnt = convert_ns_to_cnt(clk_rate, duty_ns);
	period_cnt = convert_ns_to_cnt(clk_rate, period_ns);

	LOG_D("pwm %d, clk rate %d", (int)pwm->hwpwm, clk_rate);
	LOG_D("duty_ns %d, perid_ns %d, duty_cnt %d, perid_cnt %d",
		duty_ns, period_ns, duty_cnt, period_cnt);

	/*
	 * if duty_cnt/period_cnt exceed 0xffff, the reg cnt field will
	 * overflow, so we should decrease clk rate or input duty_ns/peroid_ns,
	 * to meet the case
	 */
	if (!period_cnt
		|| (duty_cnt & 0xffff0000)
		|| (period_cnt & 0xffff0000)) {
		LOG_E("pwm %d, clk rate %d, duty_cnt(0x%08x)\n\t"
			"or period_cnt(0x%08x) invlaid, please re-config\n\t"
			"clk, or change the input duty_ns/peroid_ns para",
			(int)pwm->hwpwm, clk_rate, duty_cnt, period_cnt);
		return -RT_EINVAL;
	}

	csp_pwm_set_store(pwm->hwpwm, chip->reg, period_cnt);
	csp_pwm_set_cmp(pwm->hwpwm, chip->reg, duty_cnt);

	pdata->duty_ns = duty_ns;
	pdata->period_ns = period_ns;

	return RT_EOK;
}

static const struct pwm_ops lombo_pwm_ops = {
	.request = lombo_pwm_request,
	.free = lombo_pwm_free,
	.config = lombo_pwm_config,
	.set_polarity = lombo_pwm_set_polarity,
	.enable = lombo_pwm_enable,
	.disable = lombo_pwm_disable
};

static int rt_hw_pwm_init(void)
{
	struct pwm_chip *chip;
	struct pwm_device *pwm;
	int i, ret = RT_EOK;
	u32 value;

	/* Alloc mem for chip */
	chip = rt_zalloc(sizeof(*chip));
	if (!chip) {
		LOG_E("failed to alloc memory");
		return -RT_ENOMEM;
	}

	/* Pwm base */
	ret = config_get_u32(PWM_CONFIG_NAME, "reg", &value);
	if (ret) {
		LOG_E("config get failed");
		goto free_chip;
	}
	chip->reg = (void *)(value + VA_PWM);

	/* Pwm channel number */
	ret = config_get_u32(PWM_CONFIG_NAME, "channel", &value);
	if (ret) {
		LOG_E("config get failed");
		goto free_chip;
	}
	chip->npwm = value;

	chip->irq = INT_PWM;
	chip->pwmops = &lombo_pwm_ops;
	chip->pwms = rt_zalloc(chip->npwm * sizeof(*pwm));
	if (!chip->pwms) {
		ret = -RT_ENOMEM;
		goto free_chip;
	}

	/* Initialize all pwm channel */
	for (i = 0; i < chip->npwm; i++) {
		pwm = &chip->pwms[i];
		pwm->chip = chip;
		pwm->hwpwm = i;
	}

	ret = pwm_irq_init(chip);
	if (ret != RT_EOK) {
		LOG_E("pwm irq init failed");
		goto free_pwm;
	}

	/* Initialize pwm mutex */
	pwm_lock = rt_mutex_create("pwm", RT_IPC_FLAG_FIFO);
	if (!pwm_lock) {
		LOG_E("creat pwm mutex failed");
		ret = -RT_EINVAL;
		goto free_pwm;
	}

	pwm_chip_drv = chip;

	LOG_D("pwm init ok");

	return RT_EOK;

free_pwm:
	rt_free(chip->pwms);

free_chip:
	rt_free(chip);

	return ret;
}

INIT_DEVICE_EXPORT(rt_hw_pwm_init);

