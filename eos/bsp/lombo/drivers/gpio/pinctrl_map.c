/*
 * pinctrl_map.c - Gpio driver for LomboTech Socs
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
#include <string.h>

#include "pinctrl.h"
#include "pinctrl_map.h"
#include "pinctrl_drv.h"
#include "cfg/config_api.h"
#include <spinlock.h>

DEFINE_SPINLOCK(pin_map_lock);

#ifdef USE_REALLOC

static int add_config(unsigned long **configs,
		      unsigned *num_configs, unsigned long config)
{
	unsigned old_num = *num_configs;
	unsigned new_num = old_num + 1;
	unsigned long *new_configs;

	new_configs = rt_realloc(*configs, sizeof(*new_configs) * new_num);
	if (!new_configs) {
		LOG_E("realloc configs failed");
		return -ENOMEM;
	}

	new_configs[old_num] = config;
	*configs = new_configs;
	*num_configs = new_num;

	return 0;
}

static struct pinctrl_group *config_to_group(struct pinctrl *pctrl,
				const char *group_name, config_gpio_t *cfg)
{
	struct pinctrl_group *group = RT_NULL;
	const gpio_pin_t *pin;
	unsigned long config;
	unsigned long *configs = RT_NULL;
	unsigned num_configs = 0;
	int i, ret;

	if (strlen(group_name) >= PINCTRL_NAME_MAX) {
		LOG_E("group name %s is too long, max length is %d",
					group_name, PINCTRL_NAME_MAX);
		return RT_NULL;
	}

	group = rt_zalloc(sizeof(*group));
	if (!group) {
		LOG_E("alloc mem failed");
		return RT_NULL;
	}

	group->data.num_pins = cfg->npins;
	group->data.pins = rt_zalloc(sizeof(*group->data.pins)
					* group->data.num_pins);
	if (!group->data.pins) {
		LOG_E("alloc mem failed");
		goto free_group;
	}

	for (i = 0; i < group->data.num_pins; i++) {
		pin = &cfg->pins[i];
		group->data.pins[i] = gpio_to_pin(pctrl, pin->port, pin->pin);
		LOG_D("pin: %d", group->data.pins[i]);
	}

	if (cfg->func != PINCTRL_PROP_DEF_VALUE) {
		LOG_D("func: %d", cfg->func);
		config = PINCFG_PACK(PINCFG_TYPE_FUNC, cfg->func);
		ret = add_config(&configs, &num_configs, config);
		if (ret < 0)
			goto exit;
	}

	if (cfg->drv_level != PINCTRL_PROP_DEF_VALUE) {
		LOG_D("drv_level: %d", cfg->drv_level);
		config = PINCFG_PACK(PINCFG_TYPE_DRV, cfg->drv_level);
		ret = add_config(&configs, &num_configs, config);
		if (ret < 0)
			goto exit;
	}

	if (cfg->pull_updown != PINCTRL_PROP_DEF_VALUE) {
		LOG_D("pull_updown: %d", cfg->pull_updown);
		config = PINCFG_PACK(PINCFG_TYPE_PUD, cfg->pull_updown);
		ret = add_config(&configs, &num_configs, config);
		if (ret < 0)
			goto exit;
	}

	if (cfg->pull_resisters != PINCTRL_PROP_DEF_VALUE) {
		LOG_D("pull_resisters: %d", cfg->pull_resisters);
		config = PINCFG_PACK(PINCFG_TYPE_PUD_REG, cfg->pull_resisters);
		ret = add_config(&configs, &num_configs, config);
		if (ret < 0)
			goto exit;
	}

	if (cfg->data != PINCTRL_PROP_DEF_VALUE) {
		LOG_D("data: %d", cfg->data);
		config = PINCFG_PACK(PINCFG_TYPE_DAT, cfg->data);
		ret = add_config(&configs, &num_configs, config);
		if (ret < 0)
			goto exit;
	}

	group->data.configs = configs;
	group->data.num_configs = num_configs;
	group->state = PIN_GROUP_DISABLE;
	strncpy(group->name, group_name, sizeof(group->name));

	return group;

exit:
	if (configs)
		rt_free(configs);

	if (group->data.pins)
		rt_free(group->data.pins);

free_group:
	if (group)
		rt_free(group);

	return RT_NULL;
}

#else

static struct pinctrl_group *config_to_group(struct pinctrl *pctrl,
				const char *group_name, config_gpio_t *cfg)
{
	struct pinctrl_group *group = RT_NULL;
	const gpio_pin_t *pin;
	unsigned long config;
	unsigned long *configs = RT_NULL;
	unsigned num_configs = 0;
	int i, config_index;

	if (strlen(group_name) >= PINCTRL_NAME_MAX) {
		LOG_E("group name %s is too long, max length is %d",
					group_name, PINCTRL_NAME_MAX);
		return RT_NULL;
	}

	group = rt_zalloc(sizeof(*group));
	if (!group) {
		LOG_E("alloc mem failed");
		return RT_NULL;
	}

	group->data.num_pins = cfg->npins;
	group->data.pins = rt_zalloc(sizeof(*group->data.pins)
					* group->data.num_pins);
	if (!group->data.pins) {
		LOG_E("alloc mem failed");
		goto free_group;
	}

	for (i = 0; i < group->data.num_pins; i++) {
		pin = &cfg->pins[i];
		group->data.pins[i] = gpio_to_pin(pctrl, pin->port, pin->pin);
		LOG_D("pin: %d", group->data.pins[i]);
	}

	/* Count config */
	if (cfg->func != PINCTRL_PROP_DEF_VALUE)
		num_configs++;
	if (cfg->drv_level != PINCTRL_PROP_DEF_VALUE)
		num_configs++;
	if (cfg->pull_updown != PINCTRL_PROP_DEF_VALUE)
		num_configs++;
	if (cfg->pull_resisters != PINCTRL_PROP_DEF_VALUE)
		num_configs++;
	if (cfg->data != PINCTRL_PROP_DEF_VALUE)
		num_configs++;

	configs = rt_zalloc(sizeof(config) * num_configs);
	if (!configs) {
		LOG_E("malloc configs failed");
		goto exit;
	}

	/* Add config */
	config_index = 0;
	if (cfg->func != PINCTRL_PROP_DEF_VALUE) {
		LOG_D("func: %d", cfg->func);
		config = PINCFG_PACK(PINCFG_TYPE_FUNC, cfg->func);
		configs[config_index] = config;
		config_index++;
	}

	if (cfg->drv_level != PINCTRL_PROP_DEF_VALUE) {
		LOG_D("drv_level: %d", cfg->drv_level);
		config = PINCFG_PACK(PINCFG_TYPE_DRV, cfg->drv_level);
		configs[config_index] = config;
		config_index++;
	}

	if (cfg->pull_updown != PINCTRL_PROP_DEF_VALUE) {
		LOG_D("pull_updown: %d", cfg->pull_updown);
		config = PINCFG_PACK(PINCFG_TYPE_PUD, cfg->pull_updown);
		configs[config_index] = config;
		config_index++;
	}

	if (cfg->pull_resisters != PINCTRL_PROP_DEF_VALUE) {
		LOG_D("pull_resisters: %d", cfg->pull_resisters);
		config = PINCFG_PACK(PINCFG_TYPE_PUD_REG, cfg->pull_resisters);
		configs[config_index] = config;
		config_index++;
	}

	if (cfg->data != PINCTRL_PROP_DEF_VALUE) {
		LOG_D("data: %d", cfg->data);
		config = PINCFG_PACK(PINCFG_TYPE_DAT, cfg->data);
		configs[config_index] = config;
		config_index++;
	}

	group->data.configs = configs;
	group->data.num_configs = num_configs;
	group->state = PIN_GROUP_DISABLE;
	strncpy(group->name, group_name, sizeof(group->name));

	return group;

exit:
	if (configs)
		rt_free(configs);

	if (group->data.pins)
		rt_free(group->data.pins);

free_group:
	if (group)
		rt_free(group);

	return RT_NULL;
}


#endif

static void add_group(struct pinctrl *pctrl, struct pinctrl_group *group)
{
	rt_base_t flags;

	spin_lock_irqsave(&pin_map_lock, flags);
	list_add_tail(&group->node, &pctrl->dev->groups);
	spin_unlock_irqrestore(&pin_map_lock, flags);
}

struct pinctrl_group *pinctrl_get_groups_from_map(struct pinctrl *pctrl,
					const char *group_name)
{
	struct pinctrl_group *group;
	config_gpio_t cfg;
	int ret;

	LOG_D("get group %s from map", group_name);

	ret = config_get_gpio(pctrl->dev->name, group_name, &cfg);
	if (ret)
		return RT_NULL;

	group = config_to_group(pctrl, group_name, &cfg);
	if (!group) {
		LOG_E("config to group failed");
		return RT_NULL;
	}

	add_group(pctrl, group);

	return group;
}

static void pinctrl_map_enable(void)
{
	struct pinctrl *pctrl;
	const char *strings[MAP_ENABLE_GROUP_MAX];
	char strtmp[32], *module, *group;
	int count, i;
	rt_err_t ret;

	count = config_get_string_array(MAP_NAME, MAP_ENABLE_GROUP_NAME,
					strings, MAP_ENABLE_GROUP_MAX);

	for (i = 0; i < count; i++) {
		/* Enable group format is "module:group" */
		strcpy(strtmp, strings[i]);
		module = strtok(strtmp, MAP_ENABLE_GROUP_SAPARATOR);
		group = strtok(RT_NULL, MAP_ENABLE_GROUP_SAPARATOR);

		LOG_D("enable %s", strings[i]);

		/* Get pinctrl for module and enable group */
		pctrl = pinctrl_get(module);
		if (pctrl == RT_NULL) {
			LOG_E("Failed to get pinctrl for %s", module);
			continue;
		}

		ret = pinctrl_enable_group(pctrl, group);
		if (ret) {
			LOG_E("Enable group %s failed", group);
			continue;
		}
	}
}

void pinctrl_map_init(void)
{
	pinctrl_map_enable();
}

