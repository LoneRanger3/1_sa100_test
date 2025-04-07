/*
 * pwm_core.h - Pwm module head file
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

#ifndef __LOMBO_PWM_CORE_H__
#define __LOMBO_PWM_CORE_H__

#define MAX_PWMS 10

#define PWM_CONFIG_NAME		"pwm"
#define PWM_CONFIG_NUM		4


struct pwm_chip;
struct pwm_irq_ops;

/**
 * enum pwm_polarity - polarity of a PWM signal
 * @PWM_POLARITY_NORMAL: a high signal for the duration of the duty-
 * cycle, followed by a low signal for the remainder of the pulse
 * period
 * @PWM_POLARITY_INVERSED: a low signal for the duration of the duty-
 * cycle, followed by a high signal for the remainder of the pulse
 * period
 */
enum pwm_polarity {
	PWM_POLARITY_NORMAL,
	PWM_POLARITY_INVERSED,
};

enum pwm_flag {
	PWMF_REQUESTED = 1 << 0,
	PWMF_ENABLED = 1 << 1,
};

/**
 * struct pwm_ops - Abstract of a PWM channel
 */
struct pwm_device {
	const char		*owner;		/* Owner of this channel*/
	unsigned long		flags;		/* Pwm status(requested, enable) */
	unsigned int		hwpwm;		/* Hardware number */
	struct pwm_chip		*chip;		/* PWM chip this channel belong to */
	void			*chip_data;	/* Private data of chip */
};

/**
 * struct pwm_ops - PWM controller operations
 * @request: optional hook for requesting a PWM
 * @free: optional hook for freeing a PWM
 * @config: configure duty cycles and period length for this PWM
 * @set_polarity: configure the polarity of this PWM
 * @enable: enable PWM output toggling
 * @disable: disable PWM output toggling
 */
struct pwm_ops {
	struct pwm_device *	(*request)(struct pwm_chip *chip,
					   unsigned int pwm_num,
					   const char *label);	/* Request pwm channel */
	void			(*free)(struct pwm_device *pwm); /* Free pwm channel */
	rt_err_t		(*config)(struct pwm_device *pwm,
					  int duty_ns, int period_ns); /* Conifg pwm */
	rt_err_t		(*set_polarity)(struct pwm_device *pwm,
					  enum pwm_polarity polarity);
	rt_err_t		(*enable)(struct pwm_device *pwm);
						/* Enable pwm channel */
	void			(*disable)(struct pwm_device *pwm);
						/* Disable pwm channel */
};

/**
 * struct pwm_chip - abstract a PWM controller
 */
struct pwm_chip {
	const struct pwm_ops	*pwmops;	/* Low-level operations */
	void			*reg;		/* Membase */
	int			irq;		/* Global irq number */
	unsigned int		npwm;		/* Total pwm number */
	struct pwm_device	*pwms;		/* Array of PWM devices */
	struct pwm_irq_ops	*irqops;	/* Low-level irq operations */
};

/**
 * struct pwm_data - per-pwm private data, for chip operations
 * @polarity: pwm's polarity. p or n output polarity? to realize...
 * @duty_ns: pwm's duty in ns
 * @period_ns: pwm's period in ns
 */
struct pwm_data {
	enum pwm_polarity polarity;
	int duty_ns;
	int period_ns;
};

struct pwm_irq_ops {
	void	(*irq_mask)(struct pwm_chip *chip, int irq);
	void	(*irq_unmask)(struct pwm_chip *chip, int irq);
	int	(*irq_set_type)(struct pwm_chip *chip, unsigned int flow_type);
};

#endif/* __LOMBO_PWM_CORE_H__ */

