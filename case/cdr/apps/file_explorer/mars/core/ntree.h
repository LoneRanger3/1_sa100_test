/*
 * ntree.h - normal tree code for LomboTech
 * normal tree interface and macro define
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

#ifndef __NTREE_H__
#define __NTREE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <mars/mars.h>
#include <mars/core/ntree_nd.h>
#include <mars/core/stack.h>
#include <mars/core/ohtbl.h>
#include <mars/proc/storage.h>

#define MAX_ALL_DEPTH_NUM 8192
#define MIN_ALL_DEPTH_NUM 1024
#define MAX_ONE_DEPTH_NUM 1024
#define MIN_ONE_DEPTH_NUM 128

typedef struct tag_tree {
	tree_node_t *root_node;
	ll_t root_ll;
	ohtbl_t ohtbl;
	void *file_list;
	void *list_conf;
	lb_uint32 node_num;
	array_head_t *ahead;
	storage_head_t *shead;
} tree_t;

tree_t *tree_alloc(const char *path);
void tree_free(tree_t *tree);
lb_int32 tree_create(tree_t *tree,
	tree_node_t *par,
	const char *path);
lb_int32 tree_delete(tree_t *tree);
tree_node_t *tree_get_node(tree_t *tree,
	tree_node_t *par,
	const char *path);
tree_node_t *tree_get_parent(tree_t *tree,
	tree_node_t *root,
	const char *path);
lb_int32 tree_set_parent(tree_t *tree,
	tree_node_t *node,
	tree_node_t *parent);
tree_node_t *tree_get_child(tree_t *tree,
	tree_node_t *node,
	tree_node_t *child);
lb_int32 tree_create_node(tree_t *tree,
	tree_node_t **node,
	tree_node_t *par_node,
	const char *path,
	node_type_t nd_type);
lb_int32 tree_delete_node(tree_t *tree,
	tree_node_t *node);
lb_int32 tree_build(tree_t *tree);
lb_int32 tree_update(void *orit,
	void *newt);
lb_int32 tree_store(void *head);
lb_int32 tree_load(void *head);
lb_int32 tree_handle(tree_t *tree);

#ifdef __cplusplus
}
#endif

#endif /* __NTREE_H__ */

