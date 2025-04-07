/*
 * h_file_demo.h - Generic definitions for LomboTech SoCs
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

#ifndef __H_FILE_DEMO_H
#define __H_FILE_DEMO_H

/* comments for these macros */
#define LOMBO_IO_AHB_PBASE	0x01000000
#define LOMBO_IO_AHB_VBASE	0xf1000000
#define LOMBO_IO_AHB_SIZE	SZ_8M

struct clk_type {
	const char		*name;	/* some short comments here */
	const struct my_clk_ops *ops;	/* some long comments here, some long
					 * comments here */
	unsigned long		rate;	/* some short comments here */
	/*
	 * some long comments here, some long comments here, some long
	 * comments here, some long comments here
	 */
	struct my_list_head	children;
	struct my_list_node	child_node;
};

extern int global_flag;
extern struct demo_struct global_struct;

void lombo_restart(char mode, const char *cmd);
int clk_register(struct clk_type *clk);

#endif /* __H_FILE_DEMO_H */
