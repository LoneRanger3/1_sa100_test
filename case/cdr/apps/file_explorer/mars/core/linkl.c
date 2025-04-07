/*
 * linkl.c - double link list code for LomboTech
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

#include <mars/core/linkl.h>

/* defines (pointer of prev and next are hidden in below ) */
#define NODE_META_SIZE (sizeof(ll_node_t *) + sizeof(ll_node_t *))
#define PREV_P_OFFSET(ll_p) (ll_p->n_size)
#define NEXT_P_OFFSET(ll_p) (ll_p->n_size + sizeof(ll_node_t *))

static void mars_node_set_prev(ll_t *ll_p, ll_node_t *act, ll_node_t *prev);
static void mars_node_set_next(ll_t *ll_p, ll_node_t *act, ll_node_t *next);

void ll_init(ll_t *ll_p, lb_uint32 n_size)
{
	ll_p->head = NULL;
	ll_p->tail = NULL;

	if (n_size & 0x3) {
		n_size &= ~0x3;
		n_size += 4;
	}

	ll_p->n_size = n_size;
}

void *ll_ins_head(ll_t *ll_p)
{
	ll_node_t *n_new;

	/* pointer of prev and next are hidden in below */
	n_new = mars_mem_alloc(ll_p->n_size + NODE_META_SIZE);

	if (n_new != NULL) {
		/* No prev. before the new head */
		mars_node_set_prev(ll_p, n_new, NULL);
		/* After new comes the old head */
		mars_node_set_next(ll_p, n_new, ll_p->head);

		if (ll_p->head != NULL)
			mars_node_set_prev(ll_p, ll_p->head, n_new);

		ll_p->head = n_new;
		if (ll_p->tail == NULL)
			ll_p->tail = n_new;
	}

	return n_new;
}

void *ll_ins_prev(ll_t *ll_p, void *n_act)
{
	ll_node_t *n_new;
	ll_node_t *n_prev;

	if (NULL == ll_p || NULL == n_act)
		return NULL;

	if (ll_get_head(ll_p) == n_act)
		n_new = ll_ins_head(ll_p);
	else {
		/* pointer of prev and next are hidden in below */
		n_new = mars_mem_alloc(ll_p->n_size + NODE_META_SIZE);

		n_prev = ll_get_prev(ll_p, n_act);
		mars_node_set_next(ll_p, n_prev, n_new);
		mars_node_set_prev(ll_p, n_new, n_prev);
		mars_node_set_prev(ll_p, n_act, n_new);
		mars_node_set_next(ll_p, n_new, n_act);
	}

	return n_new;
}

void *ll_ins_tail(ll_t *ll_p)
{
	ll_node_t *n_new;

	/* pointer of prev and next are hidden in below */
	n_new = mars_mem_alloc(ll_p->n_size + NODE_META_SIZE);

	if (n_new != NULL) {
		/* No next after the new tail */
		mars_node_set_next(ll_p, n_new, NULL);
		/* The prev. before new is tho old tail */
		mars_node_set_prev(ll_p, n_new, ll_p->tail);
		if (ll_p->tail != NULL)
			mars_node_set_next(ll_p, ll_p->tail, n_new);

		ll_p->tail = n_new;
		if (ll_p->head == NULL)
			ll_p->head = n_new;
	}

	return n_new;
}

void ll_rem(ll_t  *ll_p, void *node_p)
{
	if (ll_get_head(ll_p) == node_p) {
		ll_p->head = ll_get_next(ll_p, node_p);
		if (ll_p->head == NULL)
			ll_p->tail = NULL;
		else
			mars_node_set_prev(ll_p, ll_p->head, NULL);
	} else if (ll_get_tail(ll_p) == node_p) {
		ll_p->tail = ll_get_prev(ll_p, node_p);
		if (ll_p->tail == NULL)
			ll_p->head = NULL;
		else
			mars_node_set_next(ll_p, ll_p->tail, NULL);
	} else {
		ll_node_t *n_prev = ll_get_prev(ll_p, node_p);
		ll_node_t *n_next = ll_get_next(ll_p, node_p);

		mars_node_set_next(ll_p, n_prev, n_next);
		mars_node_set_prev(ll_p, n_next, n_prev);
	}
}

void ll_clear(ll_t *ll_p)
{
	void *i;
	void *i_next;

	i = ll_get_head(ll_p);
	i_next = NULL;

	while (i != NULL) {
		i_next = ll_get_next(ll_p, i);

		ll_rem(ll_p, i);
		mars_mem_free(i);

		i = i_next;
	}
}

void ll_chg_list(ll_t *ll_ori_p, ll_t *ll_new_p, void *node)
{
	ll_rem(ll_ori_p, node);

	mars_node_set_prev(ll_new_p, node, NULL);
	mars_node_set_next(ll_new_p, node, ll_new_p->head);

	if (ll_new_p->head != NULL)
		mars_node_set_prev(ll_new_p, ll_new_p->head, node);

	ll_new_p->head = node;
	if (ll_new_p->tail == NULL)
		ll_new_p->tail = node;
}

void *ll_get_head(ll_t *ll_p)
{
	void *head = NULL;

	if (ll_p != NULL)
		head = ll_p->head;

	return head;
}

void *ll_get_tail(ll_t *ll_p)
{
	void *tail = NULL;

	if (ll_p != NULL)
		tail = ll_p->tail;

	return tail;
}

void *ll_get_next(ll_t *ll_p, void *n_act)
{
	void *next = NULL;

	if (ll_p != NULL) {
		ll_node_t *n_act_d = n_act;
		memcpy(&next, n_act_d + NEXT_P_OFFSET(ll_p), sizeof(void *));
	}

	return next;
}

void *ll_get_prev(ll_t *ll_p, void *n_act)
{
	void *prev = NULL;

	if (ll_p != NULL) {
		ll_node_t *n_act_d = n_act;
		memcpy(&prev, n_act_d + PREV_P_OFFSET(ll_p), sizeof(void *));
	}

	return prev;
}

void ll_move_before(ll_t *ll_p, void *n_act, void *n_after)
{
	if (n_act == n_after)
		return;

	if (n_after == NULL) {
		void *n_before = ll_get_tail(ll_p);
		mars_node_set_next(ll_p, n_before, n_act);
		mars_node_set_prev(ll_p, n_act, n_before);
		mars_node_set_next(ll_p, n_act, NULL);
		ll_p->tail = n_act;
	} else {
		void *n_before = ll_get_prev(ll_p, n_after);
		mars_node_set_next(ll_p, n_before, n_act);
		mars_node_set_prev(ll_p, n_act, n_before);
		mars_node_set_prev(ll_p, n_after, n_act);
		mars_node_set_next(ll_p, n_act, n_after);
	}
}

static void mars_node_set_prev(ll_t *ll_p, ll_node_t *act, ll_node_t *prev)
{
	memcpy(act + PREV_P_OFFSET(ll_p), &prev, sizeof(ll_node_t *));
}

static void mars_node_set_next(ll_t *ll_p, ll_node_t *act, ll_node_t *next)
{
	memcpy(act + NEXT_P_OFFSET(ll_p), &next, sizeof(ll_node_t *));
}
