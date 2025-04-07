/*
 * pinctrl_debug.c - Gpio driver for LomboTech Socs
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
#define DBG_LEVEL		DBG_LOG
#include <debug.h>

#include <rthw.h>
#include <csp.h>
#include <string.h>
#include <getopt.h>

#include "pinctrl.h"
#include "pinctrl_drv.h"
#include <spinlock.h>

DEFINE_SPINLOCK(pin_debug_lock);

static void pinctrl_show_help(void)
{
	const char *arg_format = "  %-20s %s%s\n";

	rt_kprintf("usage: pinctrl [argument]\n");
	rt_kprintf("arguments:\n");
	rt_kprintf(arg_format, "-a", "Dump all group info", "");
	rt_kprintf(arg_format, "-b", "Dump all gpio banks info", "");
	rt_kprintf(arg_format, "-g", "Dump all gpio info", "");
	rt_kprintf(arg_format, "-e <module:group>", "Enable the group of module", "");
	rt_kprintf(arg_format, "-d <module:group>", "Disable the group of module", "");
	rt_kprintf(arg_format, "-h,-H", "Show help", "");
	rt_kprintf("\nexample: pinctrl -e uart:uart0-gpio\n");
}

static void pinctrl_group_debug(const char *arg, rt_bool_t enable)
{
	const char *group_name;
	struct pinctrl *pctrl;
	char module_name[PINCTRL_NAME_MAX];
	int len;
	rt_err_t ret;

	group_name = strchr(arg, ':');
	if (!group_name) {
		pinctrl_show_help();
		return;
	}

	len = group_name - arg;
	if (len >= PINCTRL_NAME_MAX) {
		rt_kprintf("Module name is too long, max length is %d\n",
				PINCTRL_NAME_MAX);
		return;
	}

	strncpy(module_name, arg, len);
	module_name[len] = '\0';
	group_name++;

	pctrl = pinctrl_get(module_name);
	if (!pctrl) {
		rt_kprintf("pinctrl_get failed, module_name:%s\n", module_name);
		return;
	}

	if (enable)
		ret = pinctrl_enable_group(pctrl, group_name);
	else
		ret = pinctrl_disable_group(pctrl, group_name);
	if (ret != RT_EOK) {
		rt_kprintf("pinctrl %s group failed, group_name:%s\n",
			enable ? "enable" : "disable", group_name);
	}
}

static void pinctrl_dump_group(struct pinctrl *pctrl, struct pinctrl_group *group)
{
	int pin_idx, cfg_idx;
	struct pinctrl_setting *setting;
	enum pincfg_type cfg_type;
	struct pinctrl_pin_desc *pdesc;
	struct pinctrl_drv *pctldrv = (struct pinctrl_drv *)pctrl->drv_data;
	u32 cfg_value;
	int pin_num;
	unsigned long config;
	const char *state, *type;

	rt_kprintf("%5s group:%s\n", "", group->name);
	if (group->state == PIN_GROUP_ENABLE)
		state = "enable";
	else
		state = "disable";
	rt_kprintf("%10s state:%s\n", "", state);

	setting = &group->data;
	for (pin_idx = 0; pin_idx < setting->num_pins; pin_idx++) {
		pin_num = setting->pins[pin_idx];
		pdesc = &pctldrv->pins[pin_num];
		rt_kprintf("%10s [%s]", "", pdesc->name);
		for (cfg_idx = 0; cfg_idx < setting->num_configs; cfg_idx++) {
			config = setting->configs[cfg_idx];
			cfg_type = PINCFG_UNPACK_TYPE(config);
			cfg_value = PINCFG_UNPACK_VALUE(config);
			switch (cfg_type) {
			case PINCFG_TYPE_FUNC:
				type = "func";
				break;
			case PINCFG_TYPE_PUD:
				type = "pud";
				break;
			case PINCFG_TYPE_DRV:
				type = "drv";
				break;
			case PINCFG_TYPE_DAT:
				type = "dat";
				break;
			case PINCFG_TYPE_PUD_REG:
				type = "pud_reg";
				break;
			default:
				type = "Unknown type";
				break;
			}
			rt_kprintf(" %s:%d", type, cfg_value);
		}
		rt_kprintf("\n");
	}
}

static void pinctrl_dump_all(void)
{
	struct pinctrl *pctrl;
	struct pinctrl_dev *dev;
	struct pinctrl_group *group;
	struct pinctrl_pin_desc *pdesc;
	struct pinctrl_drv *pctldrv = get_pinctrl_drv();
	rt_base_t flags;
	int pin_num;

	spin_lock_irqsave(&pin_debug_lock, flags);
	list_for_each_entry(pctrl, &pctldrv->pinctrl_list, node) {
		dev = pctrl->dev;
		rt_kprintf("%s:\n", dev->name);
		list_for_each_entry(group, &dev->groups, node)
			pinctrl_dump_group(pctrl, group);
		rt_kprintf("%5s gpio:", "");
		for (pin_num = 0; pin_num < pctldrv->npins; pin_num++) {
			pdesc = &pctldrv->pins[pin_num];
			if (pdesc->owner == dev)
				rt_kprintf(" %s,", pdesc->name);
		}
		rt_kprintf("\n");
	}
	spin_unlock_irqrestore(&pin_debug_lock, flags);
}

static void pinctrl_dump_banks(void)
{
	struct pinctrl_drv *pctldrv = get_pinctrl_drv();
	struct pin_bank *pb;
	rt_base_t flags;
	char buf[SZ_256];
	int i, off = 0;

	spin_lock_irqsave(&pin_debug_lock, flags);

	rt_kprintf("%-16s%-16s%-16s%-16s%-16s%-16s%-16s%-16s%-16s\n",
		"name", "reg_off", "pin_base", "npins", "eint_nr",
		"eint_func", "eint_type", "eint_mask", "eint_banks_nr");

	for (i = 0; i < pctldrv->nbanks; i++) {
		pb = &pctldrv->pin_banks[i];
		off = 0;
		off += sprintf(buf + off, "%-16s", pb->name);
		off += sprintf(buf + off, "0x%08x      ", (int)pb->pctl_offset);
		off += sprintf(buf + off, "%-16d", (int)pb->pin_base);
		off += sprintf(buf + off, "%-16d", (int)pb->npins);
		off += sprintf(buf + off, "%-16d", (int)pb->eint_nr);
		off += sprintf(buf + off, "%-16d", (int)pb->eint_func);
		off += sprintf(buf + off, "%-16d", (int)pb->eint_type);
		off += sprintf(buf + off, "0x%08x      ", (int)pb->eint_mask);
		off += sprintf(buf + off, "%-16d", (int)pb->eint_banks_nr);
		rt_kprintf("%s\n", buf);
	}

	spin_unlock_irqrestore(&pin_debug_lock, flags);
}

static long pinctrl(int argc, char **argv)
{
	int c;

	while ((c = getopt(argc, argv, "Hhe:d:agb")) != -1) {
		switch (c) {
		case 'e': {
			pinctrl_group_debug(argv[optind - 1], RT_TRUE);
			goto exit;
		}
		case 'd': {
			pinctrl_group_debug(argv[optind - 1], RT_FALSE);
			goto exit;
		}
		case 'g': {
			pinctrl_dump_gpio();
			goto exit;
		}
		case 'a': {
			pinctrl_dump_all();
			goto exit;
		}
		case 'b': {
			pinctrl_dump_banks();
			goto exit;
		}
		case 'H':
		case 'h': {
			goto help;
		}
		case '?':
			if (optind >= argc)
				LOG_E("unknown option '%s'.",  argv[optind - 1]);
			else
				LOG_E("unknown option '%s'.", argv[optind]);
			goto help;
		case ':':
			LOG_E("option '%s' need a parameter.", argv[optind - 1]);
			goto help;
		default:
			LOG_E("Unexcepted parameters, please let me know, option '%c'",
					optopt);
			goto exit;
		}
	}

help:
	pinctrl_show_help();
exit:
	optind = 0;

	return 0;
}

MSH_CMD_EXPORT(pinctrl, gpio debug command);

