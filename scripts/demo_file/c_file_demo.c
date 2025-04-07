/*
 * c_file_demo.c - Common code for LomboTech Socs
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

#include <rtthread.h>
#include <csp.h>
#include <debug.h>

#include "c_file_demo.h"

#define MACRO_DEMO_1	5	/* comments for MACRO_DEMO_1 */
#define MACRO_DEMO_2	89	/* comments for MACRO_DEMO_2 */

struct demo_struct {
	u32	val;		/* comments for val */
	char	*string1;	/* comments for string1 */
	char	*string2;	/* comments for string2 */
};

static int global_flag;			/* comments for global_flag */
struct demo_struct global_struct;	/* comments for global_struct */

/**
 * lombo_restart - restart the system hardware
 * @mode: the mode for restart
 * @cmd: user command indicate how to restart
 *
 * should not be called in irq context
 *
 * return 0 if success, otherwise -1
 */
int lombo_restart(char mode, const char *cmd)
{
	...
}

/**
 * clk_register - register clk to the system
 * @clk: handle of the clock to be registered
 *
 * some comments here, some comments here, some comments
 * here, some comments here
 *
 * return 0 if success, otherwise -1
 */
int clk_register(struct clk_type *clk)
{
	...
}
