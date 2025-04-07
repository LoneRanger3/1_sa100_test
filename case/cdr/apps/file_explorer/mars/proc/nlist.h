/*
 * nlist.h - a lot of list code for LomboTech
 * a lot of list interface and macro define
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

#ifndef __NLIST_H__
#define __NLIST_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <mars/mars.h>
#include <mars/core/ntree_nd.h>
#include <mars/core/stack.h>
#include <mars/core/linkl.h>
#include <mars/drivers/filesys.h>
#include <mars/drivers/mem.h>

#define FREE_NUM 2048

typedef struct tag_filter {
	char valid;
	char **table;
} filter_t;

lb_int32 nlist_insert_node(file_list_t *file_list, const char *path);
lb_int32 nlist_delete_node(file_list_t *file_list, const char *path);
lb_int32 nlist_alloc_olevel(file_list_t *file_list, tree_node_t *node);
lb_int32 nlist_free_olevel(file_list_t *file_list);
lb_int32 nlist_alloc_mlevel(file_list_t *file_list, tree_node_t *node);
lb_int32 nlist_free_mlevel(file_list_t *file_list);

#ifdef __cplusplus
}
#endif

#endif /* __NLIST_H__ */

