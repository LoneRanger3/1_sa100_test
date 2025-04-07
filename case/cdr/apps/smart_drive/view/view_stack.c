/*
 * view_stack.c - view stack code for LomboTech
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

#include <stdio.h>
#include <stdlib.h>
#include <debug.h>
#include <rtthread.h>
#include "view_stack.h"

static view_stack_t *pstack;

lb_int32 view_stack_init(void)
{
	lb_int32 ret = 0;

	/* allocate scan stack structure first */
	pstack = (view_stack_t *)malloc(sizeof(view_stack_t));
	if (pstack == NULL) {
		ret = -1;
		goto exit;
	}

	/* alloc STACK_GROWN_LEN space for stack data area */
	pstack->data = (char *)malloc(STACK_GROWN_LEN);
	if (pstack->data == NULL) {
		ret = -1;
		goto exit;
	}

	/* initialize scan stack structure */
	pstack->total = STACK_GROWN_LEN;
	pstack->used = 0;
	pstack->top = -1;

	return ret;
exit:
	if (pstack) {
		if (pstack->data) {
			free(pstack->data);
			pstack->data = NULL;
		}

		free(pstack);
		pstack = NULL;
	}

	return ret;
}

lb_int32 view_stack_exit(void)
{
	lb_int32 ret = 0;

	if (pstack) {
		if (pstack->data) {
			free(pstack->data);
			pstack->data = NULL;
		}

		free(pstack);
		pstack = NULL;
	}

	printf("[%s,%d]Successed\n", __func__, __LINE__);

	return ret;
}

/*
 * stack_check - Check the node whether exist
 * @pstack: the pointer to the stack
 * @node:   the pointer to the node
 *
 * returns 0 means successed,-1 means failed
 *
 */
static lb_int32 view_stack_check(void *node)
{
	void *data = NULL;
	lb_int32 top = 0;
	entry_t *pentry = NULL;
	void *temp = NULL;

	data = pstack->data;
	top = pstack->top;
	printf("stack_check node:%8p\n", node);

	while (top >= 0) {
		pentry = (entry_t *)(data + top);
		temp = pentry->node;
		printf("stack_check temp:%8p\n", temp);

		if (temp == node)
			return -1;

		top = pentry->prev;
	}

	return 0;

}

/*
 * stack_push - Push the node to the stack
 * @pstack: the pointer to the stack
 * @node:   the pointer to the node
 *
 * returns 0 means successed,-1 means failed
 *
 */
lb_int32 view_stack_push(void *node)
{
	entry_t *pentry;

	if (NULL == pstack)
		return -1;

	if (NULL == node)
		return -1;

	/* check the node whether exist */
	if (0 != view_stack_check(node))
		return -1;

	/* push entry to the end of stack */
	pentry = (entry_t *)(pstack->data + pstack->used);
	pentry->len = sizeof(entry_t);
	pentry->prev = pstack->top;
	pentry->node = node;

	/* adjust stack management information */
	pstack->top = pstack->used;
	pstack->used += pentry->len;

	return 0;
}

/*
 * stack_pop - Pop the node to the stack
 * @pstack: the pointer to the stack
 * @node:   the pointer to the node
 *
 * returns 0 means successed,-1 means failed
 *
 */
lb_int32 view_stack_pop(void **node)
{
	entry_t *pentry;

	if (NULL == pstack) {
		if (node)
			*node = NULL;
		return -1;
	}

	/* stack can't be empty */
	if (0 == pstack->used) {
		if (node)
			*node = NULL;
		return -1;
	}

	/* pop entry from the top of stack */
	pentry = (entry_t *)(pstack->data + pstack->top);
	*node = pentry->node;

	/* adjust stack managemant information */
	pstack->top = pentry->prev;
	pstack->used -= pentry->len;

	return 0;
}
