/*
 * bktree.h - bk tree code for LomboTech
 * bk tree interface and macro define
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

#ifndef __BKTREE_H__
#define __BKTREE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <mars/mars.h>

typedef struct tag_bk_node {
	char *str;
	lb_int32 len;
	lb_int32 sons;
	struct tag_bk_node **bn;
	struct tag_list_node *ln;
} bk_node_t;

#define __min(a, b)			(((a) < (b)) ? (a) : (b))
#define __max(a, b)			(((a) > (b)) ? (a) : (b))

#ifdef __cplusplus
}
#endif

#endif /* __BKTREE_H__ */

