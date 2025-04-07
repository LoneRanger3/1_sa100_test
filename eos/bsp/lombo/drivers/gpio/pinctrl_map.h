/*
 * pinctrl_map.h - Gpio module head file
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

#ifndef __PINCTRL_MAP_H__
#define __PINCTRL_MAP_H__

#include "pinctrl_core.h"

#define PINCTRL_PROP_DEF_VALUE		0xFF

#define MAP_NAME			"pinctrl"
#define MAP_ENABLE_GROUP_NAME		"enable-group"
#define MAP_ENABLE_GROUP_SAPARATOR	":"
#define MAP_ENABLE_GROUP_MAX		32

struct pinctrl_group *pinctrl_get_groups_from_map(struct pinctrl *pctrl,
					const char *group_name);

void pinctrl_map_init(void);

#endif/* __PINCTRL_MAP_H__ */

