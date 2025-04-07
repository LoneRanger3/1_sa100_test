/*
 * view_node.h - vide node code for LomboTech
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

#ifndef __VIEW_NODE_H__
#define __VIEW_NODE_H__

#include "app_manage.h"
#include <debug.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef lb_int32(*init_ope)(void *para);
typedef lb_int32(*exit_ope)(void *para);

typedef	struct tag_v_node {
	init_ope init_op;
	exit_ope exit_op;
	struct tag_v_node *next;
} v_node_t;

#ifdef __cplusplus
}
#endif

#endif /* __VIEW_NODE_H__ */
