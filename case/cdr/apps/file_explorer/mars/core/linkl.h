/*
 * linkl.h - double link list code for LomboTech
 * double link list interface and macro define
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

#ifndef __LINKL_H__
#define __LINKL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <mars/mars.h>

typedef lb_uint8 ll_node_t;

typedef struct tag_ll {
	lb_uint32 n_size;
	ll_node_t *head;
	ll_node_t *tail;
} ll_t;

void ll_init(ll_t *ll_p, lb_uint32 node_size);

void *ll_ins_head(ll_t *ll_p);

void *ll_ins_prev(ll_t *ll_p, void *n_act);

void *ll_ins_tail(ll_t *ll_p);

void ll_rem(ll_t  *ll_p, void *node_p);

void ll_clear(ll_t *ll_p);

void ll_chg_list(ll_t *ll_ori_p, ll_t *ll_new_p, void *node);

void *ll_get_head(ll_t *ll_p);

void *ll_get_tail(ll_t *ll_p);

void *ll_get_next(ll_t *ll_p, void *n_act);

void *ll_get_prev(ll_t *ll_p, void *n_act);

void ll_move_before(ll_t *ll_p, void *n_act, void *n_after);

#ifdef __cplusplus
}
#endif

#endif /* __LINKL_H__ */

