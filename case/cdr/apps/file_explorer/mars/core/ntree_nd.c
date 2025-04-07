/*
 * ntree_nd.h - normal tree node code for LomboTech
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

#include <mars/core/ntree.h>
#include <mars/core/ntree_nd.h>

/**
 * node_create_dir - Create the node that is dir
 * @parent:   the parent of the node
 * @new_node: pointer to the creating node
 *
 * returns 0 means successed
 *
 */
lb_int32 node_create_dir(ll_t *root_ll,
	tree_node_t *parent, tree_node_t **new_node)
{
	if (parent == NULL) {
		ll_init(root_ll, sizeof(tree_node_t));
		*new_node = ll_ins_head(root_ll);

		(*new_node)->par = NULL; /* The root node has no parent */
		ll_init(&((*new_node)->child_ll), sizeof(tree_node_t));
	} else {
		*new_node = ll_ins_head(&(parent)->child_ll);

		(*new_node)->par = parent; /* Set the parent */
		ll_init(&((*new_node)->child_ll), sizeof(tree_node_t));
	}

	return 0;
}

/**
 * node_create_file - Create the node that is file
 * @parent:   the parent of the node
 * @new_node: pointer to the creating node
 *
 * returns 0 means successed
 *
 */
lb_int32 node_create_file(ll_t *root_ll,
	tree_node_t *parent, tree_node_t **new_node)
{
	if (parent == NULL) /* The node must have a parent */
		return -1;

	*new_node = ll_ins_tail(&(parent)->child_ll);
	if (*new_node == NULL) {
		printf("Create node failed\n");
		return -1;
	}

	(*new_node)->par = parent; /* Set the parent */
	ll_init(&((*new_node)->child_ll), sizeof(tree_node_t));

	return 0;
}

/**
 * node_set_parent - Set the parent of the node
 * @parent:   the parent of the node
 * @node: pointer to the setting node
 *
 * returns 0 means successed
 *
 */
lb_int32 node_set_parent(tree_node_t *node, tree_node_t *parent)
{
	if (node == NULL) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return -1;
	}

	if (node->par == NULL) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return -1;
	}

	if (parent == NULL) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return -1;
	}

	ll_chg_list(&node->par->child_ll, &parent->child_ll, node);
	node->par = parent;

	return 0;
}

/**
 * node_create_file - Delete the node that is file or dir
 * @node: pointer to the deleting node
 *
 * returns 0 means successed
 *
 */
lb_int32 node_hand(ohtbl_t *ohtbl, tree_node_t *node)
{
	/* Recursively delete the children */
	tree_node_t *i;
	tree_node_t *i_next;

	if (node == NULL) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return -1;
	}

	if (ohtbl_insert(ohtbl, node) != 0) {
		printf("%s,%d\n", __FILE__, __LINE__);
		return -1;
	}

	i = ll_get_head(&(node->child_ll));
	while (i != NULL) {
		/* Get the next node before statistics this */
		i_next = ll_get_next(&(node->child_ll), i);

		/* Call the recursive statistics to the child too */
		node->all_depth_sons += node_hand(ohtbl, i);

		/* Set i to the next node */
		i = i_next;

		node->all_depth_sons++;
	}

	return node->all_depth_sons;
}

/**
 * node_create_file - Delete the node that is file or dir
 * @node: pointer to the deleting node
 *
 * returns 0 means successed
 *
 */
lb_int32 node_delete(ll_t *root_ll, tree_node_t *node)
{
	/* Recursively delete the children */
	tree_node_t *i;
	tree_node_t *i_next;
	i = ll_get_head(&(node->child_ll));
	while (i != NULL) {
		/* Get the next node before delete this */
		i_next = ll_get_next(&(node->child_ll), i);

		/* Call the recursive del to the child too */
		node_delete(root_ll, i);

		/* Set i to the next node */
		i = i_next;
	}

	/* Remove the node from parent's children list */
	tree_node_t *par = node->par;
	if (par == NULL) { /* It is a screen */
		ll_rem(root_ll, node);
	} else {
		ll_rem(&(par->child_ll), node);
	}

	if (node->path) {
		mars_mem_free(node->path); /* Free the path of node */
		node->path = NULL;
	}

	if (node->thumb) {
		mars_mem_free(node->thumb); /* Free the thumb of node */
		node->thumb = NULL;
	}

	mars_mem_free(node); /* Free the node itself */

	return 0;
}
