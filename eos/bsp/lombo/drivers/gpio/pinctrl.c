/*
 * pinctrl.c - Gpio driver for LomboTech Socs
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
#include <csp.h>

#include "pinctrl_drv.h"
#include "pinctrl_map.h"
#include <spinlock.h>

DEFINE_SPINLOCK(pinctrl_lock);

static struct pinctrl_group *find_group(struct pinctrl *pctrl,
					const char *group_name)
{
	struct pinctrl_dev *dev = (struct pinctrl_dev *)pctrl->dev;
	struct pinctrl_group *group;
	rt_base_t flags;

	/* Find group in module group list */
	spin_lock_irqsave(&pinctrl_lock, flags);
	list_for_each_entry(group, &dev->groups, node) {
		if (!strcmp(group->name, group_name)) {
			spin_unlock_irqrestore(&pinctrl_lock, flags);
			return group;
		}
	}
	spin_unlock_irqrestore(&pinctrl_lock, flags);

	/* Get group from config file */
	group = pinctrl_get_groups_from_map(pctrl, group_name);
	if (group == RT_NULL)
		LOG_E("Failed to get group from map");

	return group;
}

static struct pinctrl *find_pinctrl(struct pinctrl_drv *pctldrv,
					const char *module_name)
{
	struct pinctrl *pctrl;
	rt_base_t flags;

	RT_ASSERT(pctldrv != RT_NULL && module_name != RT_NULL);

	spin_lock_irqsave(&pinctrl_lock, flags);
	list_for_each_entry(pctrl, &pctldrv->pinctrl_list, node) {
		if (!strcmp(pctrl->dev->name, module_name)) {
			spin_unlock_irqrestore(&pinctrl_lock, flags);
			return pctrl;
		}
	}
	spin_unlock_irqrestore(&pinctrl_lock, flags);

	return RT_NULL;
}

static void add_pinctrl(struct pinctrl *pctrl)
{
	struct pinctrl_drv *pctldrv;
	rt_base_t flags;

	RT_ASSERT(pctrl != RT_NULL);
	pctldrv = (struct pinctrl_drv *)pctrl->drv_data;
	RT_ASSERT(pctldrv != RT_NULL);

	spin_lock_irqsave(&pinctrl_lock, flags);
	list_add_tail(&pctrl->node, &pctldrv->pinctrl_list);
	spin_unlock_irqrestore(&pinctrl_lock, flags);
}

static void delete_pinctrl(struct pinctrl *pctrl)
{
	struct pinctrl_drv *pctldrv;
	rt_base_t flags;

	RT_ASSERT(pctrl != RT_NULL);
	pctldrv = (struct pinctrl_drv *)pctrl->drv_data;
	RT_ASSERT(pctldrv != RT_NULL);

	spin_lock_irqsave(&pinctrl_lock, flags);
	list_del(&pctrl->node);
	spin_unlock_irqrestore(&pinctrl_lock, flags);
}

static struct pinctrl *create_pinctrl(struct pinctrl_drv *pctldrv,
				const char *module_name)
{
	struct pinctrl *pctrl = RT_NULL;

	RT_ASSERT(module_name != RT_NULL);

	LOG_D("Create pinctrl for %s", module_name);

	if (strlen(module_name) >= PINCTRL_NAME_MAX) {
		LOG_E("Module name %s is too long, max length is %d",
					module_name, PINCTRL_NAME_MAX);
		goto out_err;
	}

	/* Alloc mem */
	pctrl = rt_zalloc(sizeof(*pctrl));
	if (pctrl == RT_NULL) {
		LOG_E("Failed to alloc mem for pinctrl");
		goto out_err;
	}

	pctrl->dev = rt_zalloc(sizeof(*pctrl->dev));
	if (pctrl->dev == RT_NULL) {
		LOG_E("No mem for dev");
		goto out_err;
	}

	/* copy name */
	rt_strncpy(pctrl->dev->name, module_name, PINCTRL_NAME_MAX);
	pctrl->drv_data = pctldrv;

	INIT_LIST_HEAD(&pctrl->dev->groups);

	return pctrl;

out_err:
	if (pctrl != RT_NULL) {
		if (pctrl->dev != RT_NULL)
			rt_free(pctrl->dev);
		rt_free(pctrl);
	}

	return RT_NULL;
}

static void free_pinctrl(struct pinctrl *pctrl)
{
	struct pinctrl_group *group;
	struct pinctrl_pin_desc *pindesc;
	struct pinctrl_drv *pctldrv;
	struct pinctrl_dev *dev;
	int i, ret;

	RT_ASSERT(pctrl != RT_NULL && pctrl->drv_data != RT_NULL);
	pctldrv = (struct pinctrl_drv *)pctrl->drv_data;

	/* Disable the function group */
	list_for_each_entry(group, &pctrl->dev->groups, node) {
		if (group->state == PIN_GROUP_ENABLE) {
			if (pctldrv->pmxops) {
				ret = pctldrv->pmxops->disable(pctrl, group);
				if (ret != RT_EOK) {
					LOG_E("Failed to disable group %s",
						group->name);
					return;
				}
			}
		}
	}

	/* Free group */
	while (1) {
		group = list_first_entry_or_null(&pctrl->dev->groups,
				struct pinctrl_group, node);
		if (group) {
			rt_free(group->data.configs);
			rt_free(group->data.pins);
			list_del(&group->node);
			rt_free(group);
		} else
			break;
	}

	/* Free gpio */
	for (i = 0; i < pctldrv->npins; i++) {
		pindesc = &pctldrv->pins[i];
		if (pindesc->owner != RT_NULL) {
			dev = (struct pinctrl_dev *)pindesc->owner;
			if (dev == pctrl->dev) {
				if (pctldrv->confops) {
					ret = pctldrv->confops->free_gpio(
						pctrl, pindesc->number);
					if (ret != RT_EOK) {
						LOG_E("Failed to free pin %s",
							pindesc->name);
						return;
					}
				}
			}
		}
	}

	/* Delete from pinctrl list */
	delete_pinctrl(pctrl);

	rt_free(pctrl->dev);
	pctrl->dev = RT_NULL;
	pctrl->drv_data = RT_NULL;
	rt_free(pctrl);
}

struct pinctrl *pinctrl_get(const char *module_name)
{
	struct pinctrl *pctrl;
	struct pinctrl_drv *pctldrv;

	RT_ASSERT(module_name != RT_NULL);

	LOG_D("Get pinctrl for %s", module_name);

	pctldrv = get_pinctrl_drv();
	if (pctldrv == RT_NULL) {
		LOG_E("Failed to get pinctrl drv");
		return RT_NULL;
	}

	/* Find module pinctrl in the list */
	pctrl = find_pinctrl(pctldrv, module_name);
	if (pctrl != RT_NULL) {
		LOG_D("Try to get pinctrl again, module name:%s", module_name);
		return pctrl;
	}

	/* Create pinctrl and return the handle */
	pctrl = create_pinctrl(pctldrv, module_name);
	if (pctrl == RT_NULL) {
		LOG_E("failed to create pinctrl");
		return RT_NULL;
	}

	/* Add to pinctrl list */
	add_pinctrl(pctrl);

	return pctrl;
}

void pinctrl_put(struct pinctrl *pctrl)
{
	RT_ASSERT(pctrl != RT_NULL && pctrl->dev != RT_NULL);

	LOG_D("Free pinctrl for %s", pctrl->dev->name);

	/* Release the handle */
	free_pinctrl(pctrl);
}

rt_err_t pinctrl_enable_group(struct pinctrl *pctrl,
				const char *group_name)
{
	struct pinctrl_drv *pctldrv;
	struct pinctrl_dev *dev;
	struct pinctrl_group *group;

	RT_ASSERT(pctrl != RT_NULL && group_name != RT_NULL);
	pctldrv = (struct pinctrl_drv *)pctrl->drv_data;
	dev = (struct pinctrl_dev *)pctrl->dev;
	RT_ASSERT(dev != RT_NULL);

	LOG_D("enable group %s", group_name);

	group = find_group(pctrl, group_name);
	if (group == RT_NULL) {
		LOG_E("group %s not found", group_name);
		return -RT_EEMPTY;
	}

	if (pctldrv->pmxops)
		return pctldrv->pmxops->enable(pctrl, group);

	return -RT_ENOSYS;
}

rt_err_t pinctrl_disable_group(struct pinctrl *pctrl,
				const char *group_name)
{
	struct pinctrl_drv *pctldrv;
	struct pinctrl_dev *dev;
	struct pinctrl_group *group;

	RT_ASSERT(pctrl != RT_NULL && group_name != RT_NULL);
	pctldrv = (struct pinctrl_drv *)pctrl->drv_data;
	dev = (struct pinctrl_dev *)pctrl->dev;
	RT_ASSERT(dev != RT_NULL);

	LOG_D("disable group %s", group_name);

	group = find_group(pctrl, group_name);
	if (group == RT_NULL) {
		LOG_E("group %s not found", group_name);
		return -RT_EEMPTY;
	}

	if (pctldrv->pmxops)
		return pctldrv->pmxops->disable(pctrl, group);

	return -RT_ENOSYS;
}

int pinctrl_gpio_request(struct pinctrl *pctrl,
			enum gpio_port port, enum gpio_pin pin)
{
	struct pinctrl_drv *pctldrv;

	RT_ASSERT(pctrl != RT_NULL);
	pctldrv = (struct pinctrl_drv *)pctrl->drv_data;
	RT_ASSERT(pctldrv != RT_NULL);

	if (pctldrv->confops)
		return pctldrv->confops->request_gpio(pctrl, port, pin);

	return -RT_ENOSYS;
}

rt_err_t pinctrl_gpio_free(struct pinctrl *pctrl, int gpio)
{
	struct pinctrl_drv *pctldrv;

	RT_ASSERT(pctrl != RT_NULL);
	pctldrv = (struct pinctrl_drv *)pctrl->drv_data;
	RT_ASSERT(pctldrv != RT_NULL);

	if (pctldrv->confops)
		return pctldrv->confops->free_gpio(pctrl, gpio);

	return -RT_ENOSYS;
}

rt_err_t pinctrl_gpio_direction_input(struct pinctrl *pctrl, int gpio)
{
	struct pinctrl_drv *pctldrv;

	RT_ASSERT(pctrl != RT_NULL);
	pctldrv = (struct pinctrl_drv *)pctrl->drv_data;
	RT_ASSERT(pctldrv != RT_NULL);

	if (pctldrv->confops)
		return pctldrv->confops->direction_input(pctrl, gpio);

	return -RT_ENOSYS;
}

rt_err_t pinctrl_gpio_direction_output(struct pinctrl *pctrl, int gpio)
{
	struct pinctrl_drv *pctldrv;

	RT_ASSERT(pctrl != RT_NULL);
	pctldrv = (struct pinctrl_drv *)pctrl->drv_data;
	RT_ASSERT(pctldrv != RT_NULL);

	if (pctldrv->confops)
		return pctldrv->confops->direction_output(pctrl, gpio);

	return -RT_ENOSYS;
}

rt_err_t pinctrl_gpio_set_value(struct pinctrl *pctrl, int gpio, u32 value)
{
	struct pinctrl_drv *pctldrv;

	RT_ASSERT(pctrl != RT_NULL);
	pctldrv = (struct pinctrl_drv *)pctrl->drv_data;
	RT_ASSERT(pctldrv != RT_NULL);

	if (pctldrv->confops)
		return pctldrv->confops->set_value(pctrl, gpio, value);

	return -RT_ENOSYS;
}

int pinctrl_gpio_get_value(struct pinctrl *pctrl, int gpio)
{
	struct pinctrl_drv *pctldrv;

	RT_ASSERT(pctrl != RT_NULL);
	pctldrv = (struct pinctrl_drv *)pctrl->drv_data;
	RT_ASSERT(pctldrv != RT_NULL);

	if (pctldrv->confops)
		return pctldrv->confops->get_value(pctrl, gpio);

	return -RT_ENOSYS;
}

rt_err_t pinctrl_gpio_set_pud_mode(struct pinctrl *pctrl, int gpio,
					enum gpio_pud mode)
{
	struct pinctrl_drv *pctldrv;

	RT_ASSERT(pctrl != RT_NULL);
	pctldrv = (struct pinctrl_drv *)pctrl->drv_data;
	RT_ASSERT(pctldrv != RT_NULL);

	if (pctldrv->confops)
		return pctldrv->confops->set_pud_mode(pctrl, gpio, mode);

	return -RT_ENOSYS;
}

rt_err_t pinctrl_gpio_set_pud_res(struct pinctrl *pctrl, int gpio,
					enum gpio_pud_res reg)
{
	struct pinctrl_drv *pctldrv;

	RT_ASSERT(pctrl != RT_NULL);
	pctldrv = (struct pinctrl_drv *)pctrl->drv_data;
	RT_ASSERT(pctldrv != RT_NULL);

	if (pctldrv->confops)
		return pctldrv->confops->set_pud_res(pctrl, gpio, reg);

	return -RT_ENOSYS;
}

rt_err_t pinctrl_gpio_set_drv_level(struct pinctrl *pctrl, int gpio,
					enum gpio_drv_level level)
{
	struct pinctrl_drv *pctldrv;

	RT_ASSERT(pctrl != RT_NULL);
	pctldrv = (struct pinctrl_drv *)pctrl->drv_data;
	RT_ASSERT(pctldrv != RT_NULL);

	if (pctldrv->confops)
		return pctldrv->confops->set_drv_level(pctrl, gpio, level);

	return -RT_ENOSYS;
}

rt_err_t pinctrl_gpio_set_function(struct pinctrl *pctrl, int gpio,
					u32 func)
{
	struct pinctrl_drv *pctldrv;

	RT_ASSERT(pctrl != RT_NULL);
	pctldrv = (struct pinctrl_drv *)pctrl->drv_data;
	RT_ASSERT(pctldrv != RT_NULL);

	if (pctldrv->confops)
		return pctldrv->confops->set_function(pctrl, gpio, func);

	return -RT_ENOSYS;
}

rt_err_t pinctrl_gpio_request_irq(struct pinctrl *pctrl, int gpio,
					struct gpio_irq_data *irq_data)
{
	struct pinctrl_drv *pctldrv;

	RT_ASSERT(pctrl != RT_NULL);
	pctldrv = (struct pinctrl_drv *)pctrl->drv_data;
	RT_ASSERT(pctldrv != RT_NULL);

	if (pctldrv->confops)
		return pctldrv->confops->request_irq(pctrl, gpio, irq_data);

	return -RT_ENOSYS;
}

rt_err_t pinctrl_gpio_free_irq(struct pinctrl *pctrl, int gpio)
{
	struct pinctrl_drv *pctldrv;

	RT_ASSERT(pctrl != RT_NULL);
	pctldrv = (struct pinctrl_drv *)pctrl->drv_data;
	RT_ASSERT(pctldrv != RT_NULL);

	if (pctldrv->confops)
		return pctldrv->confops->free_irq(pctrl, gpio);

	return -RT_ENOSYS;
}

rt_err_t pinctrl_gpio_set_irq_trig_type(struct pinctrl *pctrl, int gpio,
					struct gpio_irq_data *irq_data)
{
	struct pinctrl_drv *pctldrv;

	RT_ASSERT(pctrl != RT_NULL);
	pctldrv = (struct pinctrl_drv *)pctrl->drv_data;
	RT_ASSERT(pctldrv != RT_NULL);

	if (pctldrv->confops)
		return pctldrv->confops->set_irq_trig_type(pctrl, gpio, irq_data);

	return -RT_ENOSYS;
}

rt_err_t pinctrl_gpio_irq_disable(struct pinctrl *pctrl, int gpio)
{
	struct pinctrl_drv *pctldrv;

	RT_ASSERT(pctrl != RT_NULL);
	pctldrv = (struct pinctrl_drv *)pctrl->drv_data;
	RT_ASSERT(pctldrv != RT_NULL);

	if (pctldrv->confops)
		return pctldrv->confops->irq_disable(pctrl, gpio);

	return -RT_ENOSYS;
}

void pinctrl_dump_gpio(void)
{
#ifdef ARCH_LOMBO_N7V0
	dump_gpio(DUMP_TYPE_READ);
#endif
}

