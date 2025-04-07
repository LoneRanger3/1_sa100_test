/*
 * ntree_nd.h - normal tree node code for LomboTech
 * normal tree node interface and macro define
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

#ifndef __NTREE_ND_H__
#define __NTREE_ND_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <mars/mars.h>
#include <mars/core/linkl.h>
#include <mars/core/ohtbl.h>

typedef struct tag_tree_node {
	/* Pointer to the parent node of n tree */
	struct tag_tree_node *par;
	/* Linked list to store the children node of n tree */
	ll_t child_ll;

	/* To store the type of media maybe JPEG/MP4 */
	media_type_t me_type;
	/* To store the type of node */
	node_type_t nd_type;
	/* To store the type of lock */
	lock_type_t lk_type;
	/* Pointer to the path of node of n tree */
	char *path;
	/* Pointer to the name of node of n tree */
	char *name;
	/* Pointer to the thumb of node of n tree */
	char *thumb;

	/* To store the num of total sons in all depth */
	lb_uint32 all_depth_sons;

	/* Active num of sons in one depth */
	lb_uint32 one_depth_sons;
	/* File num of sons in one depth */
	lb_uint32 one_depth_file;
	/* Dir num of sons in one depth  */
	lb_uint32 one_depth_dir;

	/* The flag means whether the node can be loaded */
	time_t    st_mtime;
	/* The flag means whether the node can be loaded */
	lb_int32  st_size;
} tree_node_t;

lb_int32 node_create_dir(ll_t *root_ll,
	tree_node_t *parent, tree_node_t **new_node);
lb_int32 node_create_file(ll_t *root_ll,
	tree_node_t *parent, tree_node_t **new_node);
lb_int32 node_set_parent(tree_node_t *node, tree_node_t *parent);
lb_int32 node_hand(ohtbl_t *htbl, tree_node_t *node);
lb_int32 node_delete(ll_t *root_ll, tree_node_t *node);

#ifdef __cplusplus
}
#endif

#endif /* __NTREE_ND_H__ */

