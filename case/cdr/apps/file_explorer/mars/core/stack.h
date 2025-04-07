/*
 * stack.h - stack code for LomboTech
 * stack interface and macro define
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

#ifndef __STACK_H__
#define __STACK_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <mars/mars.h>
#include <mars/core/ntree_nd.h>

/* the length of scan stack grown one time */
#define STACK_GROWN_LEN	4096

typedef	struct tag_stack_entry {
	/* total stack entry size */
	lb_uint32 len;
	/* prev stack entry offset within stack data areas */
	lb_uint32 prev;
	/* it should be void * later */
	tree_node_t *node;
} stack_entry_t;

typedef	struct tag_stack {
	/* size of allocated stack data area */
	lb_uint32 alloc;
	/* size of useful stack data area */
	lb_uint32 used;
	/* offet of top scan stack */
	lb_uint32 top;
	/* data area to store stack entry, size will be gown if necessary */
	char *data;
} lb_stack_t;

lb_int32 stack_init(void);
lb_int32 stack_exit(void);
lb_int32 stack_push(tree_node_t *node);
lb_int32 stack_pop(tree_node_t **node);
lb_int32 stack_empty(void);

#ifdef __cplusplus
}
#endif

#endif /* __STACK_H__ */

