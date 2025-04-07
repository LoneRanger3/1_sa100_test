/*
 * pinmux.h - Gpio module head file
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

#ifndef __PINMUX_H__
#define __PINMUX_H__

#include "soc_define.h"
#include "pinctrl_core.h"

/**
 * struct pinmux_ops - pinmux operations, to be implemented by pin controller
 * drivers that support pinmuxing
 * @enable: enable a certain muxing function with a certain pin group. The
 *	driver does not need to figure out whether enabling this function
 *	conflicts some other use of the pins in that group, such collisions
 *	are handled by the pinmux subsystem. The @func_selector selects a
 *	certain function whereas @group_selector selects a certain set of pins
 *	to be used. On simple controllers the latter argument may be ignored
 * @disable: disable a certain muxing selector with a certain pin group
 */
struct pinmux_ops {
	rt_err_t (*enable)(struct pinctrl *pctrl, struct pinctrl_group *group);
	rt_err_t (*disable)(struct pinctrl *pctrl, struct pinctrl_group *group);
};

#endif/* __PINMUX_H__ */
