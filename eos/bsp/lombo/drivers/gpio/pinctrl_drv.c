/*
 * pinctrl_drv.c - Gpio driver for LomboTech Socs
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

#define DBG_SECTION_NAME	"GPIO"
#define DBG_LEVEL		DBG_INFO
#include <debug.h>

#include <rthw.h>

#include "pinctrl_map.h"
#include "pinctrl_drv.h"
#include "pinctrl_platform.h"
#include <spinlock.h>

DEFINE_SPINLOCK(pin_drv_lock);

static struct pinctrl_drv *lombo_pinctrl_drv = RT_NULL;

struct pinctrl_drv *get_pinctrl_drv(void)
{
	return lombo_pinctrl_drv;
}

/* get the special pin bank */
static void pin_to_reg_bank(struct pinctrl_drv *pctldrv,
			    unsigned pin, void **reg, u32 *offset,
			    struct pin_bank **bank)
{
	struct pin_bank *b = pctldrv->pin_banks;

	while ((pin >= b->pin_base) && ((b->pin_base + b->npins - 1) < pin))
		b++;

	*reg = pctldrv->regs_base + b->pctl_offset;
	*offset = pin - b->pin_base;
	if (bank)
		*bank = b;
}

/* pin mux function register write ops*/
int pin_func_setup(struct pinctrl *pctrl, unsigned int pin,
				unsigned long *config, int enable)
{
	struct pinctrl_drv *pctldrv = (struct pinctrl_drv *)pctrl->drv_data;
	const struct pin_bank_type *type;
	struct pin_bank *bank;
	void *reg;
	u32 mask, shift, data, pin_offset, nreg;
	u32 cfg_value;
	rt_base_t flags;

	pin_to_reg_bank(pctldrv, pin - pctldrv->pin_base, &reg,
			&pin_offset, &bank);

	type = bank->type;
	mask = (1 << type->fld_width[PINCFG_TYPE_FUNC]) - 1;
	shift = pin_offset * type->fld_width[PINCFG_TYPE_FUNC];

	/* register is 32-bits width, select config register */
	nreg = shift / REG_BIT_WIDTH;
	shift = shift - nreg * REG_BIT_WIDTH;
	reg = reg + nreg * ADDR_STEP;

	spin_lock_irqsave(&pin_drv_lock, flags);
	data = readl(reg + type->reg_offset[PINCFG_TYPE_FUNC]);
	data &= ~(mask << shift);
	if (enable) {
		cfg_value = PINCFG_UNPACK_VALUE(*config);
		data |= cfg_value << shift;
		LOG_D("setup pin %d reg(0x%x) func = %d shift = %d",
			pin, reg + type->reg_offset[PINCFG_TYPE_FUNC],
			cfg_value, shift);
	}
	writel(data, reg + type->reg_offset[PINCFG_TYPE_FUNC]);
	spin_unlock_irqrestore(&pin_drv_lock, flags);

	return 0;
}

/* pin configuration register read or write ops*/
int pin_rw(struct pinctrl *pctrl, unsigned int pin, unsigned long *config,
				enum pincfg_type cfg_type, int set)
{
	struct pinctrl_drv *pctldrv = (struct pinctrl_drv *)pctrl->drv_data;
	const struct pin_bank_type *type;
	struct pin_bank *bank;
	void *reg;
	rt_base_t flags;
	u32 data, width, pin_offset, mask, shift, nreg;
	u32 cfg_value, cfg_reg;

	pin_to_reg_bank(pctldrv, pin - pctldrv->pin_base, &reg, &pin_offset,
			&bank);

	type = bank->type;
	if (cfg_type >= PINCFG_TYPE_NUM || !type->fld_width[cfg_type]) {
		LOG_E("pin config invalid\n");
		return -EINVAL;
	}

	width = type->fld_width[cfg_type];
	cfg_reg = type->reg_offset[cfg_type];

	spin_lock_irqsave(&pin_drv_lock, flags);

	/* pull up/down and drive strength*/
	if (cfg_type == PINCFG_TYPE_PUD || cfg_type == PINCFG_TYPE_DRV) {
		shift = pin_offset * 4;
		nreg = shift / REG_BIT_WIDTH;
		shift = shift - nreg * REG_BIT_WIDTH;
		reg = reg + nreg * ADDR_STEP;
	} else
		shift = pin_offset * width;

	mask = (1 << width) - 1;
	data = readl(reg + cfg_reg);

	/* set config */
	if (set) {
		cfg_value = PINCFG_UNPACK_VALUE(*config);
		data &= ~(mask << shift);
		data |= (cfg_value << shift);
		writel(data, reg + cfg_reg);
		LOG_D("set pin %d reg(0x%x) value = 0x%x shift = %d",
			pin, reg + cfg_reg, cfg_value, shift);
	} else {
		data >>= shift;
		data &= mask;
		*config = PINCFG_PACK(cfg_type, data);
	}

	spin_unlock_irqrestore(&pin_drv_lock, flags);

	return RT_EOK;
}

rt_bool_t gpio_is_requested(struct pinctrl *pctrl, int gpio)
{
	struct pinctrl_drv *pctldrv = (struct pinctrl_drv *)pctrl->drv_data;
	struct pinctrl_pin_desc *pindesc;
	struct pinctrl_dev *dev;

	pindesc = &pctldrv->pins[gpio];
	if (pindesc->owner != RT_NULL) {
		dev = (struct pinctrl_dev *)pindesc->owner;
		if (dev == pctrl->dev)
			return RT_TRUE;
	}

	LOG_E("gpio %d hasn't been requested by %s",
				gpio, pctrl->dev->name);

	return RT_FALSE;
}

int gpio_to_pin(struct pinctrl *pctrl, enum gpio_port port, enum gpio_pin pin)
{
	struct pinctrl_drv *pctldrv;
	struct pin_bank *bank;
	int pin_num = 0, bank_index;

	RT_ASSERT(pctrl != RT_NULL);
	RT_ASSERT(port >= 0 && port < GPIO_PORT_NUM);
	RT_ASSERT(pin >= 0 && pin < GPIO_PIN_NUM);
	pctldrv = (struct pinctrl_drv *)pctrl->drv_data;

	/* Check port and pin */
	if (port >= pctldrv->nbanks) {
		LOG_E("Unexpected port");
		return -1;
	}

	bank = &pctldrv->pin_banks[port];
	if (pin >= bank->npins) {
		LOG_E("Unexpected pin");
		return -1;
	}

	for (bank_index = 0; bank_index < port; bank_index++) {
		bank = &pctldrv->pin_banks[bank_index];
		pin_num += bank->npins;
	}
	pin_num += pin;

	return pin_num;
}

static int pin_request(struct pinctrl *pctrl, int pin_num)
{
	struct pinctrl_drv *pctldrv;
	struct pinctrl_pin_desc *pdesc;
	rt_base_t flags;

	RT_ASSERT(pctrl != RT_NULL);
	pctldrv = (struct pinctrl_drv *)pctrl->drv_data;
	RT_ASSERT(pin_num >= 0 && pin_num < pctldrv->npins);

	LOG_D("Request pin %d for %s", pin_num, pctrl->dev->name);

	spin_lock_irqsave(&pin_drv_lock, flags);
	pdesc = &pctldrv->pins[pin_num];
	if (pdesc->owner != RT_NULL) {
		if (pdesc->owner != pctrl->dev) {
			LOG_E("Pin %s is already requested by %s", pdesc->name,
				((struct pinctrl_dev *)pdesc->owner)->name);
			spin_unlock_irqrestore(&pin_drv_lock, flags);
			return -RT_EBUSY;
		} else {
			spin_unlock_irqrestore(&pin_drv_lock, flags);
			return RT_EOK;
		}
	}
	pdesc->owner = pctrl->dev;
	spin_unlock_irqrestore(&pin_drv_lock, flags);

	return RT_EOK;
}

static int pin_request_gpio(struct pinctrl *pctrl,
			enum gpio_port port, enum gpio_pin pin)
{
	int pin_num, ret;

	/* To pin number */
	pin_num = gpio_to_pin(pctrl, port, pin);
	if (pin_num == -1)
		return -1;

	/* Request the pin */
	ret = pin_request(pctrl, pin_num);
	if (ret != RT_EOK)
		return -1;

	return pin_num;
}

static rt_err_t pin_free(struct pinctrl *pctrl, int pin_num)
{
	struct pinctrl_drv *pctldrv;
	struct pinctrl_pin_desc *pdesc;
	struct pinctrl_dev *dev;
	rt_base_t flags;

	RT_ASSERT(pctrl != RT_NULL);
	pctldrv = (struct pinctrl_drv *)pctrl->drv_data;
	RT_ASSERT(pin_num >= 0 && pin_num < pctldrv->npins);

	LOG_D("Free pin %d from %s", pin_num, pctrl->dev->name);

	spin_lock_irqsave(&pin_drv_lock, flags);
	pdesc = &pctldrv->pins[pin_num];
	if (pdesc->owner == RT_NULL) {
		LOG_E("Pin %s is already free", pdesc->name);
		spin_unlock_irqrestore(&pin_drv_lock, flags);
		return -RT_ERROR;
	}
	dev = (struct pinctrl_dev *)pdesc->owner;
	if (dev != pctrl->dev) {
		LOG_E("Pin %s is not belong to %s, but %s", pdesc->name,
			pctrl->dev->name, dev->name);
		spin_unlock_irqrestore(&pin_drv_lock, flags);
		return -RT_ERROR;
	}
	pdesc->owner = RT_NULL;
	spin_unlock_irqrestore(&pin_drv_lock, flags);

	return RT_EOK;
}

static rt_err_t func_group_enable(struct pinctrl *pctrl, struct pinctrl_group *group)
{
	struct pinctrl_setting *setting;
	enum pincfg_type cfg_type;
	unsigned long config;
	int ret, pin_idx, cfg_idx;

	if (group->state != PIN_GROUP_DISABLE) {
		LOG_E("group %s has been enabled", group->name);
		return -RT_ERROR;
	}

	/* Try to allocate all pins in this group, one by one */
	setting = &group->data;
	for (pin_idx = 0; pin_idx < setting->num_pins; pin_idx++) {
		ret = pin_request(pctrl, setting->pins[pin_idx]);
		if (ret != RT_EOK) {
			LOG_E("could not request pin %d on device %s\n",
				setting->pins[pin_idx], pctrl->dev->name);
			return -RT_ERROR;
		}
	}

	/* Config the pins of group */
	for (pin_idx = 0; pin_idx < setting->num_pins; pin_idx++) {
		for (cfg_idx = 0; cfg_idx < setting->num_configs; cfg_idx++) {
			config = setting->configs[cfg_idx];
			cfg_type = PINCFG_UNPACK_TYPE(config);
			if (cfg_type == PINCFG_TYPE_FUNC) {
				ret = pin_func_setup(pctrl, setting->pins[pin_idx],
						&config, FUNCTION_ENABLE);

			} else {
				ret = pin_rw(pctrl, setting->pins[pin_idx],
						&config, cfg_type, REG_WRITE);
			}
			if (ret != RT_EOK) {
				LOG_E("config pin failed");
				return ret;
			}
		}
	}
	group->state = PIN_GROUP_ENABLE;

	return RT_EOK;
}

static rt_err_t func_group_disable(struct pinctrl *pctrl, struct pinctrl_group *group)
{
	struct pinctrl_setting *setting;
	enum pincfg_type cfg_type;
	unsigned long config;
	int ret, pin_idx, cfg_idx;

	if (group->state != PIN_GROUP_ENABLE) {
		LOG_E("group %s hasn't been enabled", group->name);
		return -RT_ERROR;
	}

	/* Disable the function group */
	setting = &group->data;
	for (pin_idx = 0; pin_idx < setting->num_pins; pin_idx++) {
		for (cfg_idx = 0; cfg_idx < setting->num_configs; cfg_idx++) {
			config = setting->configs[cfg_idx];
			cfg_type = PINCFG_UNPACK_TYPE(config);
			if (cfg_type == PINCFG_TYPE_FUNC) {
				ret = pin_func_setup(pctrl, setting->pins[pin_idx],
						&config, FUNCTION_DISABLE);
				if (ret != RT_EOK) {
					LOG_E("config pin failed");
					return ret;
				}
			}
		}
	}

	/* Try to allocate all pins in this group, one by one */
	for (pin_idx = 0; pin_idx < setting->num_pins; pin_idx++) {
		ret = pin_free(pctrl, setting->pins[pin_idx]);
		if (ret != RT_EOK) {
			LOG_E("could not free pin %d on device %s\n",
				setting->pins[pin_idx], pctrl->dev->name);
			return -RT_ERROR;
		}
	}
	group->state = PIN_GROUP_DISABLE;

	return RT_EOK;
}

/* gpio direction setting function */
static rt_err_t gpio_set_direction(struct pinctrl *pctrl, int pin, rt_bool_t io)
{
	struct pinctrl_drv *pctldrv = (struct pinctrl_drv *)pctrl->drv_data;
	struct pin_bank *bank;
	const struct pin_bank_type *type;
	void *reg;
	rt_base_t flags;
	u32 data, mask, shift, nreg, offset;

	pin_to_reg_bank(pctldrv, pin - pctldrv->pin_base, &reg, &offset,
			&bank);
	type = bank->type;

	reg = pctldrv->regs_base + bank->pctl_offset;
	reg += type->reg_offset[PINCFG_TYPE_FUNC];

	mask = (1 << type->fld_width[PINCFG_TYPE_FUNC]) - 1;
	shift = offset * type->fld_width[PINCFG_TYPE_FUNC];

	/* register is 32-bits width, select config register */
	nreg = shift / REG_BIT_WIDTH;
	shift = shift - nreg * REG_BIT_WIDTH;
	reg = reg + nreg * ADDR_STEP;

	spin_lock_irqsave(&pin_drv_lock, flags);
	data = readl(reg);
	data &= ~(mask << shift);

	/* output */
	if (!io)
		data |= FUN_OUTPUT << shift;
	else
		data |= FUN_INPUT << shift;
	writel(data, reg);
	spin_unlock_irqrestore(&pin_drv_lock, flags);

	return 0;
}

static rt_err_t gpio_direction_input(struct pinctrl *pctrl, int gpio)
{
	if (!gpio_is_requested(pctrl, gpio))
		return -RT_ERROR;

	return gpio_set_direction(pctrl, gpio, RT_TRUE);
}

static rt_err_t gpio_direction_output(struct pinctrl *pctrl, int gpio)
{
	if (!gpio_is_requested(pctrl, gpio))
		return -RT_ERROR;

	return gpio_set_direction(pctrl, gpio, RT_FALSE);
}

static rt_err_t gpio_rw(struct pinctrl *pctrl, int gpio, u32 value,
				int cfg_type, int set)
{
	unsigned long config = 0;
	int ret;

	if (!gpio_is_requested(pctrl, gpio))
		return -RT_ERROR;

	if (set)
		config = PINCFG_PACK(cfg_type, value);

	ret = pin_rw(pctrl, gpio, &config, cfg_type, set);
	if (ret != RT_EOK) {
		LOG_E("Faile to rw of pin %d, cfg_type: %d", gpio, cfg_type);
		return ret;
	}

	if (!set)
		return PINCFG_UNPACK_VALUE(config);

	return RT_EOK;
}

static rt_err_t gpio_set_value(struct pinctrl *pctrl, int gpio, u32 value)
{
	return gpio_rw(pctrl, gpio, value, PINCFG_TYPE_DAT, REG_WRITE);
}

static int gpio_get_value(struct pinctrl *pctrl, int gpio)
{
	return gpio_rw(pctrl, gpio, 0, PINCFG_TYPE_DAT, REG_READ);
}

static rt_err_t gpio_set_pud_mode(struct pinctrl *pctrl, int gpio,
					enum gpio_pud mode)
{
	return gpio_rw(pctrl, gpio, mode, PINCFG_TYPE_PUD, REG_WRITE);
}

static rt_err_t gpio_set_drv_level(struct pinctrl *pctrl, int gpio,
					enum gpio_drv_level level)
{
	return gpio_rw(pctrl, gpio, level, PINCFG_TYPE_DRV, REG_WRITE);
}

static rt_err_t gpio_set_pud_res(struct pinctrl *pctrl, int gpio,
					enum gpio_pud_res  res)
{
	return gpio_rw(pctrl, gpio, res, PINCFG_TYPE_PUD_REG, REG_WRITE);
}

static rt_err_t gpio_set_function(struct pinctrl *pctrl, int gpio, u32 func)
{
	unsigned long config = 0;
	int ret;

	if (!gpio_is_requested(pctrl, gpio))
		return -RT_ERROR;

	config = PINCFG_PACK(PINCFG_TYPE_FUNC, func);
	ret = pin_func_setup(pctrl, gpio, &config, FUNCTION_ENABLE);
	if (ret != RT_EOK) {
		LOG_E("Failed to set function of pin %d", gpio);
		return ret;
	}

	return RT_EOK;
}

static rt_err_t gpio_request_irq(struct pinctrl *pctrl, int gpio,
				struct gpio_irq_data *irq_data)
{
	struct pinctrl_drv *pctldrv = (struct pinctrl_drv *)pctrl->drv_data;
	struct pin_bank *bank;
	struct pinctrl_irq_desc *irqdesc;
	rt_base_t flags;
	void *reg;
	u32 offset;

	if (!gpio_is_requested(pctrl, gpio))
		return -RT_ERROR;

	pin_to_reg_bank(pctldrv, gpio, &reg, &offset, &bank);
	if (bank->eint_type != EINT_TYPE_GPIO) {
		LOG_E("gpio %d do not support interrupt", gpio);
		return -RT_ERROR;
	}

	spin_lock_irqsave(&pin_drv_lock, flags);
	irqdesc = &bank->irqdesc[offset];
	if (irqdesc->handler != RT_NULL) {
		LOG_E("Failed to request irq of gpio %d", gpio);
		spin_unlock_irqrestore(&pin_drv_lock, flags);
		return -RT_ERROR;
	}
	irqdesc->handler = irq_data->handler;
	irqdesc->data = irq_data->irq_arg;
	pctldrv->chip->irq_ops->irq_set_type(bank, offset, irq_data->trig_type);
	pctldrv->chip->irq_ops->irq_set_samp(bank, irq_data->clock_src_div,
					irq_data->clock_src);
	pctldrv->chip->irq_ops->irq_unmask(bank, offset);
	spin_unlock_irqrestore(&pin_drv_lock, flags);

	return RT_EOK;
}

static rt_err_t gpio_free_irq(struct pinctrl *pctrl, int gpio)
{
	struct pinctrl_drv *pctldrv = (struct pinctrl_drv *)pctrl->drv_data;
	struct pin_bank *bank;
	struct pinctrl_irq_desc *irqdesc;
	rt_base_t flags;
	void *reg;
	u32 offset;

	if (!gpio_is_requested(pctrl, gpio))
		return -RT_ERROR;

	pin_to_reg_bank(pctldrv, gpio, &reg, &offset, &bank);

	spin_lock_irqsave(&pin_drv_lock, flags);
	irqdesc = &bank->irqdesc[offset];
	if (irqdesc->handler == RT_NULL) {
		LOG_E("Failed to free irq of gpio %d", gpio);
		spin_unlock_irqrestore(&pin_drv_lock, flags);
		return -RT_ERROR;
	}
	irqdesc->handler = RT_NULL;
	pctldrv->chip->irq_ops->irq_mask(bank, offset);
	spin_unlock_irqrestore(&pin_drv_lock, flags);

	return RT_EOK;
}

static rt_err_t gpio_set_irq_trig_type(struct pinctrl *pctrl, int gpio,
					struct gpio_irq_data *irq_data)
{
	struct pinctrl_drv *pctldrv = (struct pinctrl_drv *)pctrl->drv_data;
	struct pin_bank *bank;
	struct pinctrl_irq_desc *irqdesc;
	rt_base_t flags;
	void *reg;
	u32 offset;

	if (!gpio_is_requested(pctrl, gpio))
		return -RT_ERROR;

	pin_to_reg_bank(pctldrv, gpio, &reg, &offset, &bank);

	spin_lock_irqsave(&pin_drv_lock, flags);
	irqdesc = &bank->irqdesc[offset];
	if (irqdesc->handler == RT_NULL) {
		LOG_E("Failed to change irq of gpio %d", gpio);
		spin_unlock_irqrestore(&pin_drv_lock, flags);
		return -RT_ERROR;
	}
	pctldrv->chip->irq_ops->irq_set_type(bank, offset, irq_data->trig_type);
	pctldrv->chip->irq_ops->irq_unmask(bank, offset);
	spin_unlock_irqrestore(&pin_drv_lock, flags);

	return RT_EOK;
}

static rt_err_t gpio_irq_disable(struct pinctrl *pctrl, int gpio)
{
	struct pinctrl_drv *pctldrv = (struct pinctrl_drv *)pctrl->drv_data;
	struct pin_bank *bank;
	struct pinctrl_irq_desc *irqdesc;
	rt_base_t flags;
	void *reg;
	u32 offset;

	if (!gpio_is_requested(pctrl, gpio))
		return -RT_ERROR;

	pin_to_reg_bank(pctldrv, gpio, &reg, &offset, &bank);

	spin_lock_irqsave(&pin_drv_lock, flags);
	irqdesc = &bank->irqdesc[offset];
	if (irqdesc->handler == RT_NULL) {
		LOG_E("Failed to change irq of gpio %d", gpio);
		spin_unlock_irqrestore(&pin_drv_lock, flags);
		return -RT_ERROR;
	}
	pctldrv->chip->irq_ops->irq_mask(bank, offset);
	spin_unlock_irqrestore(&pin_drv_lock, flags);

	return RT_EOK;
}

static struct pinconf_ops lombo_pinconf_ops = {
	pin_request_gpio,
	pin_free,
	gpio_direction_input,
	gpio_direction_output,
	gpio_set_value,
	gpio_get_value,
	gpio_set_pud_mode,
	gpio_set_pud_res,
	gpio_set_drv_level,
	gpio_set_function,
	gpio_request_irq,
	gpio_free_irq,
	gpio_set_irq_trig_type,
	gpio_irq_disable
};

static struct pinmux_ops lombo_pinmux_ops = {
	func_group_enable,
	func_group_disable
};

static int pinctrl_init(void)
{
	struct pinctrl_pin_desc *pindesc, *pdesc;
	struct pinctrl_drv *pctldrv;
	struct pin_bank *bank;
	const struct chip_pin_ctrl *chip = RT_NULL;
	const struct pin_bank_data *bank_data;
	int i, pin, ret;
	char *pin_names;
	u32 p1, p2;

	lombo_pinctrl_drv = rt_zalloc(sizeof(*lombo_pinctrl_drv));
	if (lombo_pinctrl_drv == RT_NULL) {
		LOG_E("Failed to zalloc for lombo_pinctrl_drv");
		return -RT_ENOMEM;
	}
	pctldrv = lombo_pinctrl_drv;
	pctldrv->name = "lombo-pinctrl-drv";
	pctldrv->regs_base = (void *)(BASE_GPIO + VA_GPIO);
	pctldrv->confops = &lombo_pinconf_ops;
	pctldrv->pmxops = &lombo_pinmux_ops;

	/* Init pinctrl list */
	INIT_LIST_HEAD(&pctldrv->pinctrl_list);

#if defined(ARCH_LOMBO_N7)
	chip = n7_pin_ctrl;
#elif defined(ARCH_LOMBO_N8V0)
	chip = n8_pin_ctrl;
#endif

	/* Init pin and bank */
	pctldrv->chip = chip;
	pctldrv->nbanks = chip->nbanks;
	RT_ASSERT(!lombo_func2(&p1, &p2));
	if (1 == p1)
		pctldrv->nbanks -= 2;
	pctldrv->pin_banks = rt_zalloc(sizeof(*pctldrv->pin_banks)
				* pctldrv->nbanks);
	if (pctldrv->pin_banks == RT_NULL) {
		LOG_E("Failed to zalloc for pin banks");
		return -RT_ENOMEM;
	}

	pctldrv->npins = 0;
	bank_data = chip->pin_banks;
	bank = pctldrv->pin_banks;
	for (i = 0; i < pctldrv->nbanks; ++i, ++bank_data, ++bank) {
		bank->type = bank_data->type;
		bank->pctl_offset = bank_data->pctl_offset;
		bank->npins = bank_data->npins;
		bank->eint_func = bank_data->eint_func;
		bank->eint_type = bank_data->eint_type;
		bank->eint_mask = bank_data->eint_mask;
		bank->eint_nr = bank_data->eint_nr;
		bank->eint_banks_nr = bank_data->eint_banks_nr;
		bank->name = bank_data->name;
		/* spin_lock_init(&bank->slock); */
		bank->pin_base = pctldrv->npins;
		bank->soc_priv = pctldrv;
		pctldrv->npins += bank->npins;
	}

	/* Init pin desc */
	pctldrv->pins = rt_zalloc(sizeof(*pindesc) * pctldrv->npins);
	if (pctldrv->pins == RT_NULL) {
		LOG_E("failed to alloc pin desc mem");
		return -RT_ENOMEM;
	}

	/* pin number */
	pindesc = pctldrv->pins;
	for (pin = 0, pdesc = pindesc; pin < pctldrv->npins; pin++, pdesc++)
		pdesc->number = pin + pctldrv->pin_base;

	/* pin name */
	pin_names = rt_zalloc(sizeof(char) * PIN_NAME_LENGTH * pctldrv->npins);
	if (!pin_names) {
		LOG_E("failed to alloc pin name mem");
		return -ENOMEM;
	}

	for (i = 0; i < pctldrv->nbanks; i++) {
		bank = &pctldrv->pin_banks[i];
		for (pin = 0; pin < bank->npins; pin++) {
			rt_sprintf(pin_names, "%s-%d", bank->name, pin);
			pdesc = pindesc + bank->pin_base + pin;
			pdesc->name = pin_names;
			pin_names += PIN_NAME_LENGTH;
		}
	}

	/* Init irq */
	ret = chip->eint_gpio_init(pctldrv);
	if (ret != RT_EOK) {
		LOG_E("Failed to init irq");
		return ret;
	}

	pinctrl_map_init();

	return RT_EOK;
}

int rt_hw_gpio_init(void)
{
	int ret;

#ifdef ARCH_LOMBO_N7V1_TDR
	u32 val;
	/* clear sio 7 int */
	val = READREG32(VA_RTC_SIO_INT_MASK0);
	val &= (~(1<<7));
	WRITEREG32(VA_RTC_SIO_INT_MASK0, val);

	val = READREG32(VA_RTC_SIO_INT_PEND0);
	val &= (~(1<<7));
	WRITEREG32(VA_RTC_SIO_INT_PEND0, val);
#endif
	ret = pinctrl_init();
	if (ret != RT_EOK) {
		LOG_E("Failed to initialize pinctrl");
		return -RT_ERROR;
	}

	return RT_EOK;
}

