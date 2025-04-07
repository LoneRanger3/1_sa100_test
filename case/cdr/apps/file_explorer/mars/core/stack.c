/*
 * stack.c - stack code for LomboTech
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

#include <mars/core/stack.h>

static lb_stack_t *pstack;

lb_int32 stack_init(void)
{
	lb_int32 ret = 0;

	/* allocate scan stack structure first */
	pstack = (lb_stack_t *)mars_mem_alloc(sizeof(lb_stack_t));
	if (NULL == pstack) {
		ret = -1;
		goto exit;
	}

	/* preallocate STACK_GROWN_LEN space for stack data area,
	 * if the scan stack space not enough, will grown scan stack data space.
	 */
	pstack->data = (char *)mars_mem_alloc(STACK_GROWN_LEN);
	if (NULL == pstack->data) {
		ret = -1;
		goto exit;
	}

	/* initialize scan stack structure */
	pstack->alloc =	STACK_GROWN_LEN;
	pstack->used = 0;
	pstack->top = 0;

	return ret;
exit:
	if (pstack) {
		if (pstack->data) {
			mars_mem_free(pstack->data);
			pstack->data = NULL;
		}

		mars_mem_free(pstack);
		pstack = NULL;
	}
	return ret;
}

lb_int32 stack_exit(void)
{
	lb_int32 ret = 0;

	if (pstack) {
		if (pstack->data) {
			mars_mem_free(pstack->data);
			pstack->data = NULL;
		}

		mars_mem_free(pstack);
		pstack = NULL;
	}

	return ret;
}

/*
 * stack_push - Push the node to the stack
 * @pstack: the pointer to the stack
 * @node:   the pointer to the node
 *
 * returns 0 means successed,-1 means failed
 *
 */
lb_int32 stack_push(tree_node_t *node)
{
	stack_entry_t *pentry;
	lb_uint32 len;

	if (NULL == pstack) {
		printf("%s,%d\n", __FILE__, __LINE__);
		return -1;
	}

	if (NULL == node->path) {
		printf("%s,%d\n", __FILE__, __LINE__);
		return -1;
	}

	/* calc the pushed entry needed space, should align by 64bit(8byte) */
	len = sizeof(lb_uint32) + sizeof(lb_uint32) + sizeof(tree_node_t *) + 1;
	len = (((len) + ((sizeof(lb_uint64)) - 1)) & ~((sizeof(lb_uint64)) - 1));

	/* check stack mars_mem_free space enough or not */
	while (len > (pstack->alloc - pstack->used)) {

		char *ndata;
		lb_uint32 nlen = pstack->alloc + STACK_GROWN_LEN;
		ndata = mars_mem_realloc(pstack->data, nlen);
		if (NULL == ndata)
			return -1;
		pstack->data = ndata;
		pstack->alloc += STACK_GROWN_LEN;
	}

	/* push directory to the end of stack */
	pentry = (stack_entry_t *)(pstack->data + pstack->used);
	pentry->len	= len;
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
lb_int32 stack_pop(tree_node_t **node)
{
	stack_entry_t *pentry;

	if (NULL == pstack) {
		printf("%s,%d\n", __FILE__, __LINE__);
		return -1;
	}

	/* stack can't be empty */
	if (0 == pstack->used) {
		printf("%s,%d\n", __FILE__, __LINE__);
		return -1;
	}

	/* pop the top entry */
	pentry = (stack_entry_t *)(pstack->data + pstack->top);
	*node = pentry->node;

	/* adjust stack managemant information */
	pstack->top = pentry->prev;
	pstack->used -= pentry->len;

	return 0;
}

lb_int32 stack_empty(void)
{
	if (NULL == pstack) {
		printf("%s,%d\n", __FILE__, __LINE__);
		return -1;
	}

	if (0 == pstack->used)
		return 1;

	return 0;
}
