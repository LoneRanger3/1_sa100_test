/*
 * pinctrl_drv.h - Gpio module head file
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

#ifndef __PINCTRL_DRV_H__
#define __PINCTRL_DRV_H__

#include "pinctrl_core.h"
#include "pinmux.h"
#include "pinctrl_gpio.h"

#define PINCTRL_CONFIG_NAME	"pinctrl"

struct pinctrl_drv {
	const char *name;
	void *regs_base;
	struct pin_bank *pin_banks;
	u32 nbanks;
	struct pinctrl_pin_desc *pins;
	u32 pin_base;
	u32 npins;

	struct list_head pinctrl_list; /* List of pin controller
					handles (struct pinctrl) */
	const struct chip_pin_ctrl *chip;

	struct pinconf_ops *confops;
	struct pinmux_ops *pmxops;
};

/**
 * struct irq_chip - hardware interrupt chip descriptor
 * @irq_ack:		start of a new interrupt
 * @irq_mask:		mask an interrupt source
 * @irq_unmask:		unmask an interrupt source
 * @irq_set_type:	set the flow type (IRQ_TYPE_LEVEL/etc.) of an IRQ
 */
struct irq_ops {
	void	(*irq_ack)(struct pin_bank *bank, u32 hwirq);
	void	(*irq_mask)(struct pin_bank *bank, u32 hwirq);
	void	(*irq_unmask)(struct pin_bank *bank, u32 hwirq);
	int	(*irq_set_type)(struct pin_bank *bank,
					u32 hwirq, unsigned int flow_type);
	int	(*irq_set_samp)(struct pin_bank *bank, int div, int source);
};

/**
 * struct platform_pin_ctrl: represent a pin controller.
 * @pin_banks: list of pin banks included in this controller.
 * @nbanks: number of pin banks.
 * @eint_gpio_init: callback to setup the external gpio interrupts controller.
 * @eint_wkup_init: callback to setup the external wakeup interrupts controller.
 */
struct chip_pin_ctrl {
	const struct pin_bank_data *pin_banks;
	u32 nbanks;
	int (*eint_gpio_init)(struct pinctrl_drv *);
	struct irq_ops *irq_ops;
};

struct pinctrl_drv *get_pinctrl_drv(void);

int pin_func_setup(struct pinctrl *pctrl, unsigned int pin,
				unsigned long *config, int enable);

int pin_rw(struct pinctrl *pctrl, unsigned int pin, unsigned long *config,
				enum pincfg_type cfg_type, int set);

int gpio_to_pin(struct pinctrl *pctrl, enum gpio_port port, enum gpio_pin pin);

#endif/* __PINCTRL_DRV_H__ */

