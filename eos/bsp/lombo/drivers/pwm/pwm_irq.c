/*
 * pwm_irq.c - Pwm driver for LomboTech Socs
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

#include <fls.h>
#include <rthw.h>

#include "pwm_csp.h"
#include "pwm_irq.h"

static void pwm_irq_mask(struct pwm_chip *chip, int irq)
{
	RT_ASSERT(chip != RT_NULL);

	csp_pwm_irq_endisable(irq, chip->reg, RT_TRUE);
}

static void pwm_irq_unmask(struct pwm_chip *chip, int irq)
{
	RT_ASSERT(chip != RT_NULL);

	csp_pwm_irq_endisable(irq, chip->reg, RT_FALSE);
}

static int pwm_irq_set_type(struct pwm_chip *chip,
					unsigned int flow_type)
{
	LOG_W("no operation\n");

	return 0;
}

static struct pwm_irq_ops lombo_pwm_irq_ops = {
	.irq_mask	= pwm_irq_mask,
	.irq_unmask	= pwm_irq_unmask,
	.irq_set_type	= pwm_irq_set_type,
};

void pwm_irq_handler(int vector, void *param)
{
	struct pwm_irq_data *pdata = (struct pwm_irq_data *)param;
	struct pwm_irq *pwmirq;
	u32 pend, bit;

	pend = csp_pwm_get_irq_pend_regval(pdata->reg);
	while (pend) {
		/* Check pending */
		bit = fls(pend) - 1;
		pend &= ~(1 << bit);

		/* Handle the irq, call user callback */
		pwmirq = &pdata->pwm_irqs[bit];
		if (!pwmirq->handler)
			pwmirq->handler(param);

		/* Clear pending */
		csp_pwm_irq_clr_pend(bit, pdata->reg);
	}
}

int pwm_irq_init(struct pwm_chip *chip)
{
	struct pwm_irq_data *pdata;

	pdata = rt_zalloc(sizeof(*pdata));
	if (!pdata) {
		LOG_E("zalloc failed\n");
		return -RT_ENOMEM;
	}

	chip->irqops = &lombo_pwm_irq_ops;
	pdata->irq = chip->irq;
	pdata->reg = chip->reg;
	pdata->pwm_irqs = rt_zalloc(sizeof(struct pwm_irq) * chip->npwm);
	if (NULL == pdata->pwm_irqs) {
		LOG_E("zalloc for pwm_irqs failed\n");
		rt_free(pdata);
		return -ENOMEM;
	}

	/* Register pwm interrupt */
	rt_hw_interrupt_install(pdata->irq, pwm_irq_handler,
				pdata, "pwm_irq");

	rt_hw_interrupt_umask(pdata->irq);

	return RT_EOK;
}

