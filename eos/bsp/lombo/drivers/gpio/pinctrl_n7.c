/*
 * pinctrl_n7.c - Gpio driver for LomboTech Socs
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
#if defined(ARCH_LOMBO_N7)

#define DBG_SECTION_NAME	"GPIO"
#define DBG_LEVEL		DBG_INFO
#include <debug.h>

#include <rthw.h>
#include <fls.h>
#include "irq_numbers.h"
#include "pinctrl_drv.h"
#include "cfg/config_api.h"
#include <spinlock.h>

#define GB_INT_CFG	0x0560
#define GB_INT_MASK	0x0570
#define GB_INT_PEND	0x0574
#define GB_INT_SAMP	0x0578

#define GE_INT_CFG	0x0620
#define GE_INT_MASK	0x0630
#define GE_INT_PEND	0x0634
#define GE_INT_SAMP	0x0638

#define SIO_REG_OFF	(BASE_RTC - BASE_GPIO)
#define SIO_INT_CFG	(SIO_REG_OFF + 0x0860)
#define SIO_INT_MASK	(SIO_REG_OFF + 0x0870)
#define SIO_INT_PEND	(SIO_REG_OFF + 0x0874)
#define SIO_INT_SAMP	(SIO_REG_OFF + 0x0878)

#define GF_INT_CFG	0x0660
#define GF_INT_MASK	0x0670
#define GF_INT_PEND	0x0674
#define GF_INT_SAMP	0x0678

#define GG_INT_CFG	0x06A0
#define GG_INT_MASK	0x06B0
#define GG_INT_PEND	0x06B4
#define GG_INT_SAMP	0x06B8

#define GPIO_B_NAME	"gpb"
#define GPIO_E_NAME	"gpe"
#define GPIO_SIO_NAME	"sio"
#define GPIO_F_NAME	"gpf"
#define GPIO_G_NAME	"gpg"

/**
 * n7 bank type define, contain five registers type.
 * Function configuration: 4-bits for per pin
 * Pull up or pull down configuration: 2-bits for per pin
 * Driver configuration: 3-bits for per pin
 * Data: 1-bit for per pin
 * Resistors configuration: 1-bit for per pin
 */
static const struct pin_bank_type n7_bank_type = {
	.fld_width = {4, 2, 3, 1, 1,},
	.reg_offset = {0x0, 0x10, 0x20, 0x30, 0x34,},
};

#define PIN_BANK(pins, reg, id)		\
	{						\
		.type		= &n7_bank_type,	\
		.pctl_offset	= reg,			\
		.npins		= pins,			\
		.eint_type	= EINT_TYPE_NONE,	\
		.name		= id			\
	}

#define PIN_BANK_GPIO(pins, reg, id, emask, int_offset, eint_banks) \
	{						\
		.type		= &n7_bank_type,	\
		.pctl_offset	= reg,			\
		.npins		= pins,			\
		.eint_type	= EINT_TYPE_GPIO,	\
		.name		= id,			\
		.eint_mask	= emask,		\
		.eint_nr	= int_offset,		\
		.eint_banks_nr	= eint_banks		\
	}

/* n7 pin controller pin banks */
static const struct pin_bank_data n7_pin_banks[] = {
#ifdef ARCH_LOMBO_N7V1
	PIN_BANK(16, 0x020, "gpa"),
#else
	PIN_BANK(12, 0x020, "gpa"),
#endif
	PIN_BANK_GPIO(28, 0x060, "gpb", 0x0FFFFFFF, INT_GPIO0, 1),
	PIN_BANK(23, 0x0A0, "gpc"),
	PIN_BANK(6, 0x0E0, "gpd"),
	PIN_BANK_GPIO(15, 0x120, "gpe", 0x00007FFF, INT_GPIO1, 1),
	PIN_BANK_GPIO(8,  SIO_REG_OFF + 0x820, "sio", 0x000000FF, INT_SIO,   1),
	PIN_BANK_GPIO(10, 0x160, "gpf", 0x000003FF, INT_GPIO2, 2),
	PIN_BANK_GPIO(10, 0x1A0, "gpg", 0x000003FF, INT_GPIO2, 2),
};

DEFINE_SPINLOCK(pin_n7_lock);

/* config bank interrupt sample clock */
static inline int n7_gpio_irq_set_samp(struct pin_bank *bank, int div,
					int source)
{
	struct pinctrl_drv *d = (struct pinctrl_drv *)bank->soc_priv;
	void *reg;
	u32 val;

	if (!strcmp(bank->name, GPIO_B_NAME))
		reg = d->regs_base + GB_INT_SAMP;
	else if (!strcmp(bank->name, GPIO_E_NAME))
		reg = d->regs_base + GE_INT_SAMP;
	else if (!strcmp(bank->name, GPIO_SIO_NAME))
		reg = d->regs_base + SIO_INT_SAMP;
	else if (!strcmp(bank->name, GPIO_F_NAME))
		reg = d->regs_base + GF_INT_SAMP;
	else if (!strcmp(bank->name, GPIO_G_NAME))
		reg = d->regs_base + GG_INT_SAMP;
	else {
		LOG_E("%s: not interrupt bank\n", bank->name);
		return -RT_ERROR;
	}

	val = readl(reg);
	val |= 1u << 31;
	val |= div << 4;
	val |= source;

	writel(val, reg);

	return 0;
}

static int n7_gpio_irq_set_type(struct pin_bank *bank,
				u32 hwirq, unsigned int flow_type)
{
	struct pinctrl_drv *d = (struct pinctrl_drv *)bank->soc_priv;
	void *reg;
	u32 nreg, shift, trigger, val;

	/* reg addr */
	if (!strcmp(bank->name, GPIO_B_NAME))
		reg = d->regs_base + GB_INT_CFG;
	else if (!strcmp(bank->name, GPIO_E_NAME))
		reg = d->regs_base + GE_INT_CFG;
	else if (!strcmp(bank->name, GPIO_SIO_NAME))
		reg = d->regs_base + SIO_INT_CFG;
	else if (!strcmp(bank->name, GPIO_F_NAME))
		reg = d->regs_base + GF_INT_CFG;
	else if (!strcmp(bank->name, GPIO_G_NAME))
		reg = d->regs_base + GG_INT_CFG;
	else {
		LOG_E("%s not interrupt bank\n", bank->name);
		return -EINVAL;
	}

	shift = hwirq * 4;
	nreg = shift / REG_BIT_WIDTH;
	shift = shift - nreg * REG_BIT_WIDTH;
	reg = reg + nreg * ADDR_STEP;
	trigger = flow_type;

	val = readl(reg);
	val &= ~(0xf << shift); /* clear the trig bits */
	val |= (trigger << shift); /* set the trig bits */
	writel(val, reg);

	return 0;
}

static inline void n7_gpio_irq_set_mask(struct pin_bank *bank,
				u32 hwirq, rt_bool_t mask)
{
	struct pinctrl_drv *d = (struct pinctrl_drv *)bank->soc_priv;
	void *reg;
	u32 val;

	if (!strcmp(bank->name, GPIO_B_NAME))
		reg = d->regs_base + GB_INT_MASK;
	else if (!strcmp(bank->name, GPIO_E_NAME))
		reg = d->regs_base + GE_INT_MASK;
	else if (!strcmp(bank->name, GPIO_SIO_NAME))
		reg = d->regs_base + SIO_INT_MASK;
	else if (!strcmp(bank->name, GPIO_F_NAME))
		reg = d->regs_base + GF_INT_MASK;
	else if (!strcmp(bank->name, GPIO_G_NAME))
		reg = d->regs_base + GG_INT_MASK;
	else {
		LOG_E("%s not interrupt bank\n", bank->name);
		return;
	}

	val = readl(reg);
	if (mask)
		val |= 1 << (hwirq);
	else
		val &= ~(1 << (hwirq));
	writel(val, reg);
}

static void n7_gpio_irq_unmask(struct pin_bank *bank, u32 hwirq)
{
	n7_gpio_irq_set_mask(bank, hwirq, RT_TRUE);
}

static void n7_gpio_irq_mask(struct pin_bank *bank, u32 hwirq)
{
	n7_gpio_irq_set_mask(bank, hwirq, RT_FALSE);
}

static void n7_gpio_irq_ack(struct pin_bank *bank, u32 hwirq)
{
	struct pinctrl_drv *d = (struct pinctrl_drv *)bank->soc_priv;
	void *reg;

	if (!strcmp(bank->name, GPIO_B_NAME))
		reg = d->regs_base + GB_INT_PEND;
	else if (!strcmp(bank->name, GPIO_E_NAME))
		reg = d->regs_base + GE_INT_PEND;
	else if (!strcmp(bank->name, GPIO_SIO_NAME))
		reg = d->regs_base + SIO_INT_PEND;
	else if (!strcmp(bank->name, GPIO_F_NAME))
		reg = d->regs_base + GF_INT_PEND;
	else if (!strcmp(bank->name, GPIO_G_NAME))
		reg = d->regs_base + GG_INT_PEND;
	else {
		LOG_E("%s not interrupt bank\n", bank->name);
		return;
	}

	writel(1 << (hwirq), reg);
}

static struct irq_ops n7_irq_osp = {
	n7_gpio_irq_ack,
	n7_gpio_irq_mask,
	n7_gpio_irq_unmask,
	n7_gpio_irq_set_type,
	n7_gpio_irq_set_samp,
};

void __do_banks_share_irq(int irqno, struct pinctrl_drv *drv)
{
	struct pinctrl_irq_desc *irqdesc;
	unsigned int pend, mask, bit, i;
	struct pin_bank *bank;
	void *preg, *mreg;
	rt_base_t flags;

	for (i = 0; i < drv->nbanks; i++) {
		bank = &drv->pin_banks[i];
		if (bank->eint_nr != irqno)
			continue;

		if (!strcmp(bank->name, GPIO_F_NAME)) {
			preg = drv->regs_base + GF_INT_PEND;
			mreg = drv->regs_base + GF_INT_MASK;
		} else if (!strcmp(bank->name, GPIO_G_NAME)) {
			preg = drv->regs_base + GG_INT_PEND;
			mreg = drv->regs_base + GG_INT_MASK;
		} else {
			LOG_E("bank %s is not interrupt bank\n", bank->name);
			return;
		}

		spin_lock_irqsave(&pin_n7_lock, flags);

		pend = readl(preg);
		mask = readl(mreg);
		pend = pend & mask & bank->eint_mask;
		while (pend) {
			bit = fls(pend) - 1;
			pend &= ~(1 << bit);

			/* irq handler */
			LOG_D("irq bit = %d", bit);
			irqdesc = &bank->irqdesc[bit];
			if (irqdesc->handler)
				irqdesc->handler(irqdesc->data);
			else
				LOG_W("unpected irq for %s-%x", bank->name, bit);
			drv->chip->irq_ops->irq_ack(bank, bit);
		}

		spin_unlock_irqrestore(&pin_n7_lock, flags);
	}
}

static void lombo_gpio_isr(int irqno, void *param)
{
	struct pin_bank *bank = (struct pin_bank *)(param);
	struct pinctrl_drv *d = (struct pinctrl_drv *)bank->soc_priv;
	struct pinctrl_irq_desc *irqdesc;
	void *preg, *mreg;
	unsigned int pend, mask, bit;
	rt_base_t flags;

	if (bank->eint_banks_nr > 1) {
		__do_banks_share_irq(irqno, d);
		return;
	}

	/* reg addr */
	if (!strcmp(bank->name, GPIO_B_NAME)) {
		preg = d->regs_base + GB_INT_PEND;
		mreg = d->regs_base + GB_INT_MASK;
	} else if (!strcmp(bank->name, GPIO_E_NAME)) {
		preg = d->regs_base + GE_INT_PEND;
		mreg = d->regs_base + GE_INT_MASK;
	} else if (!strcmp(bank->name, GPIO_SIO_NAME)) {
		preg = d->regs_base + SIO_INT_PEND;
		mreg = d->regs_base + SIO_INT_MASK;
	} else {
		LOG_E("%s not interrupt bank\n", bank->name);
		return;
	}

	spin_lock_irqsave(&pin_n7_lock, flags);

	pend = readl(preg);
	mask = readl(mreg);
	pend = pend & mask & bank->eint_mask;

	while (pend) {
		bit = fls(pend) - 1;
		pend &= ~(1 << bit);

		/* irq handler */
		LOG_D("irq bit = %d", bit);
		irqdesc = &bank->irqdesc[bit];
		if (irqdesc->handler)
			irqdesc->handler(irqdesc->data);
		else
			LOG_W("unpected irq for %s-%x", bank->name, bit);
		d->chip->irq_ops->irq_ack(bank, bit);
	}

	spin_unlock_irqrestore(&pin_n7_lock, flags);
}

static u32 pinctrl_cfg_get_clk_src(void)
{
	int ret;
	u32 clk_src;

	ret = config_get_u32(PINCTRL_CONFIG_NAME, "clock-src", &clk_src);
	if (ret) {
		LOG_E("config get failed");
		return -1;
	}

	LOG_D("pinctrl clock src: %d", clk_src);

	return clk_src;
}

static u32 pinctrl_cfg_get_clk_div(void)
{
	int ret;
	u32 clk_div;

	ret = config_get_u32(PINCTRL_CONFIG_NAME, "clock-div", &clk_div);
	if (ret) {
		LOG_E("config get failed");
		return -1;
	}

	LOG_D("pinctrl clock div: %d", clk_div);

	return clk_div;
}

static int pinctrl_irq_init(struct pinctrl_drv *pctldrv)
{
	struct pin_bank *bank;
	int n, eint_pin_num;
	u32 clk_src, clk_div;

	for (n = 0; n < pctldrv->nbanks; n++) {
		bank = &pctldrv->pin_banks[n];
		if (bank->eint_type != EINT_TYPE_GPIO)
			continue;

		/* Get sample time property from dts */
		clk_src = pinctrl_cfg_get_clk_src();
		clk_div = pinctrl_cfg_get_clk_div();
		if ((clk_src != -1) && (clk_div != -1)) {
			LOG_D("sample clock src:%d, div:%d", clk_src, clk_div);
			n7_gpio_irq_set_samp(bank, clk_div, clk_src);

		}

		/* Init bank irq desc */
		eint_pin_num = fls(bank->eint_mask);
		bank->irqdesc = rt_zalloc(sizeof(*bank->irqdesc) * eint_pin_num);
		if (bank->irqdesc == RT_NULL) {
			LOG_E("No mem for bank %s irq desc", bank->name);
			return -RT_ENOMEM;
		}

		/* TODO: mask all bank bits that can generate irq */

		/*
		 * Register irq
		 * NOTE: if multi bank share one irq, ok, only the last one registerd
		 */
		rt_hw_interrupt_install(bank->eint_nr, lombo_gpio_isr,
					bank, (char *)bank->name);

		/* Enable gpio irq */
		rt_hw_interrupt_umask(bank->eint_nr);
	}

	return RT_EOK;
}

const struct chip_pin_ctrl n7_pin_ctrl[] = {
	{
		.pin_banks	= n7_pin_banks,
		.nbanks		= ARRAY_SIZE(n7_pin_banks),
		.eint_gpio_init	= pinctrl_irq_init,
		.irq_ops	= &n7_irq_osp,
	},
};

#endif

