/*
 * view_stack.h - stack code for LomboTech
 * view stack interface and macro define
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

#ifndef __VIEW_STACK_H__
#define __VIEW_STACK_H__

#include "app_manage.h"
#include <debug.h>

#ifdef __cplusplus
extern "C" {
#endif

/* the length of scan stack grown one time */
#define STACK_GROWN_LEN	512

typedef	struct tag_entry {
	/* total stack entry size */
	lb_uint32 len;
	/* prev stack entry offset within stack data areas */
	lb_int32 prev;
	/* it should be void * later */
	void *node;
} entry_t;

typedef	struct tag_view_stack {
	/* size of total stack data area */
	lb_uint32 total;
	/* size of useful stack data area */
	lb_uint32 used;
	/* offset of top of stack */
	lb_int32 top;
	/* data area to store stack entry, size will be gown if necessary */
	char *data;
} view_stack_t;

lb_int32 view_stack_init();
lb_int32 view_stack_exit();
lb_int32 view_stack_push(void *node);
lb_int32 view_stack_pop(void **node);

#ifdef __cplusplus
}
#endif

#endif /* __VIEW_STACK_H__ */
