/*
 * ntree.c - normal tree code for LomboTech
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
#include <mars/core/ntree.h>
#include <mars/core/ohtbl.h>
#include <mars/proc/mtype.h>
#include <mars/proc/lock.h>

static lb_int32 update_dir(tree_t *tree, tree_node_t *par,
	tree_node_t *tree_node, const char *path)
{
	time_t st_mtime = 0;
	lb_int32 st_size = 0;

	if (0 != mars_getmtime(path, &st_mtime))
		printf("[%s,%d]\n", __func__, __LINE__);

	if (0 != mars_getsize(path, &st_size))
		printf("[%s,%d]\n", __func__, __LINE__);


	printf("[%s,%d],tree_node->path:%s, ori st_mtime:%lld, new st_mtime:%lld\n",
		__func__, __LINE__, tree_node->path, tree_node->st_mtime, st_mtime);

	if ((st_mtime == tree_node->st_mtime) ||
		(st_size == tree_node->st_size)) {
		printf("[%s,%d],no need to update\n", __func__, __LINE__);
		tree_set_parent(tree, tree_node, par);
		return 0;
	} else {
		printf("[%s,%d],need to update\n", __func__, __LINE__);
		return -1;
	}

}

static lb_int32 update_file(tree_t *tree, tree_node_t *par,
	tree_node_t *tree_node, const char *path)
{
#if 0
	time_t st_mtime = 0;
	lb_uint32 st_size = 0;

	if (0 != mars_getmtime(fildes, &st_mtime))
		printf("[%s,%d]\n", __func__, __LINE__);

	if (0 != mars_getsize(fildes, &st_size))
		printf("[%s,%d]\n", __func__, __LINE__);
#endif

	return 0;
}


tree_t *tree_alloc(const char *path)
{
	tree_t *tree;

	tree = (tree_t *)mars_mem_alloc(sizeof(tree_t));
	if (tree == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		return NULL;
	}
	memset(tree, 0x00, sizeof(tree_t));

	tree->list_conf = (list_conf_t *)mars_mem_alloc(sizeof(list_conf_t));
	if (tree->list_conf == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		return NULL;
	}
	memset(tree->list_conf, 0x00, sizeof(list_conf_t));

#if 0
	if (ohtbl_init(&tree->ohtbl,
			TBLSIZ, h1_str, h2_str, match_str, NULL) != 0) {
		printf("[%s,%d]\n", __func__, __LINE__);
		return NULL;
	}
#endif

	if (tree_create(tree, NULL, path) != 0) {
		printf("[%s,%d]\n", __func__, __LINE__);
		return NULL;
	}

	return tree;
}

void tree_free(tree_t *tree)
{
	if (tree) {

		if (tree_delete(tree) != 0) {
			printf("[%s,%d]\n", __func__, __LINE__);
			return;
		}

#if 0
		if (ohtbl_destroy(&tree->ohtbl) != 0) {
			printf("[%s,%d]\n", __func__, __LINE__);
			return;
		}
#endif
		mars_mem_free(tree->list_conf);
		mars_mem_free(tree);

		return;
	}
}

/**
 * tree_create - Create the tree
 * @tree: the pointer to the tree
 * @par:  the parent of the node
 * @path: the pointer to the path
 *
 * returns 0 means successed
 *
 */
lb_int32 tree_create(tree_t *tree, tree_node_t *par, const char *path)
{
	lb_int32 ret = 0;
	tree_node_t *node = NULL;

	ret = tree_create_node(tree, &node, par, path, NODE_DIR);
	if (ret != 0) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}
	tree->node_num++;
	tree->root_node = node;

	return ret;
exit:
	return ret;
}

/**
 * tree_delete - Delete the tree
 * @tree: the pointer to the tree
 *
 * returns 0 means successed
 *
 */
lb_int32 tree_delete(tree_t *tree)
{
	lb_int32 ret = 0;

	if (tree->root_node) {
		tree_delete_node(tree, tree->root_node);
		tree->root_node = NULL;
	}

	tree->node_num = 0;

	return ret;
}

tree_node_t *tree_get_node(tree_t *tree, tree_node_t *par, const char *path)
{
#if 0
	tree_node_t *node;
	tree_node_t *temp;
#endif
	/* Recursively find the children */
	tree_node_t *i;
	tree_node_t *i_next;
	tree_node_t *n;
#if 0
	node = (tree_node_t *)mars_mem_alloc(sizeof(tree_node_t));
	if (node == NULL) {
		printf("[%s,%d],failed\n", __func__, __LINE__);
		return NULL;
	}

	temp = node;

	node->path = mars_mem_alloc(strlen(path) + 1);
	if (node == NULL) {
		printf("[%s,%d],failed\n", __func__, __LINE__);
		return NULL;
	}

	strcpy(node->path, path);

	if (ohtbl_lookup(&tree->ohtbl, (void **)&temp) == 0) {
		mars_mem_free(node->path);
		mars_mem_free(node);

		return temp;
	} else {
		printf("[%s,%d],node->path:%s not found\n", __func__, __LINE__,
			node->path);
		mars_mem_free(node->path);
		mars_mem_free(node);
	}
#endif

	if (0 == strcmp(par->path, path))
		return par;

	i = ll_get_head(&(par->child_ll));

	while (i != NULL) {

		/* Get the next node before find this */
		i_next = ll_get_next(&(par->child_ll), i);

		/* Call the recursive get to the child too */
		n = tree_get_node(tree, i, path);

		/* Find the node that need */
		if (n)
			return n;

		/* Set i to the next node */
		i = i_next;
	}

	return NULL;
}

tree_node_t *tree_get_parent(tree_t *tree, tree_node_t *root, const char *path)
{
	char *last_path = NULL;
	lb_uint8 par_len = 0;
	char par_path[256];
	tree_node_t *par_node = NULL;

	last_path = strrchr(path, '/');
	if (last_path == NULL)
		return NULL;

	par_len = last_path - path;
	memcpy(par_path, path, par_len);
	par_path[par_len] = '\0';

	par_node = tree_get_node(tree, root, par_path);
	if (par_node == NULL)
		return NULL;

	return par_node;
}

lb_int32 tree_set_parent(tree_t *tree, tree_node_t *node, tree_node_t *parent)
{
	if (node) {
		node_set_parent(node, parent);
		return 0;
	}

	return -1;
}

tree_node_t *tree_get_child(tree_t *tree, tree_node_t *node, tree_node_t *child)
{
	if (node) {
		if (child == NULL)
			return ll_get_head(&node->child_ll);
		else
			return ll_get_next(&node->child_ll, child);
	}

	return NULL;
}

char *tree_get_node_name(char *path)
{
	char *name = NULL;

	if (path == NULL) {
		printf("[%s,%d],failed\n", __func__, __LINE__);
		goto exit;
	}

	name = strrchr(path, '/');
	if (name)
		name++;

	return name;

exit:
	return NULL;
}

char *tree_get_node_thumb(char *name, media_type_t me_type)
{
	lb_int32 len = 0;
	char *thumb = NULL;
	char temp_path[256];
	char temp_name[256];
	char *suffix = NULL;

	if (name == NULL) {
		printf("[%s,%d],failed\n", __func__, __LINE__);
		goto exit;
	}

	suffix = strrchr(name, '.');
	if (suffix == NULL)
		goto exit;

	len = suffix - name;
	if (len) {
		memcpy(temp_name, name, len);
		temp_name[len] = '\0';
	}
	strcat(temp_name, ".db");

	if (me_type == MEDIA_PIC)
		strcpy(temp_path, MARS_TB_IMAGE_PATH);
	else if (me_type == MEDIA_VIDEO)
		strcpy(temp_path, MARS_TB_VIDEO_PATH);
	else
		strcpy(temp_path, MARS_TB_OTHER_PATH);

	strcat(temp_path, temp_name);

	thumb = mars_mem_alloc(strlen(temp_path) + 1);
	if (thumb  == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		goto exit;
	}
	strcpy(thumb, temp_path);

	return thumb;

exit:
	return NULL;
}

lb_int32 tree_create_node(tree_t *tree,
	tree_node_t **node,
	tree_node_t *par_node,
	const char *path,
	node_type_t nd_type)
{
	lb_int32 ret = 0;

	if (tree == NULL) {
		printf("[%s,%d],failed\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	if (node == NULL) {
		printf("[%s,%d],failed\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	if (nd_type == NODE_DIR) {
		ret = node_create_dir(&tree->root_ll, par_node, node);
		if (ret != 0) {
			printf("[%s,%d],failed\n", __func__, __LINE__);
			ret = -1;
			goto exit;
		}
	} else if (nd_type == NODE_FILE) {
		ret = node_create_file(&tree->root_ll, par_node, node);
		if (ret != 0) {
			printf("[%s,%d],failed\n", __func__, __LINE__);
			ret = -1;
			goto exit;
		}
	}

	(*node)->nd_type = nd_type;
	(*node)->path = mars_mem_alloc(strlen(path) + 1);
	if ((*node)->path == NULL) {
		printf("[%s,%d],failed\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}
	strcpy((*node)->path, path);

	(*node)->me_type = suffix_to_mtype(path);
	(*node)->lk_type = suffix_to_ltype(path);

	(*node)->name = tree_get_node_name((*node)->path);
	if ((*node)->name)
		(*node)->thumb = tree_get_node_thumb((*node)->name, (*node)->me_type);

	(*node)->all_depth_sons = 0;

	(*node)->one_depth_sons = 0;
	(*node)->one_depth_file = 0;
	(*node)->one_depth_dir = 0;

	return ret;
exit:
	if (node) {
		if (*node) {
			node_delete(&tree->root_ll, *node);
			*node = NULL;
		}
	}

	return ret;
}

lb_int32 tree_delete_node(tree_t *tree,
	tree_node_t *node)
{
	lb_int32 ret = 0;

	if (tree == NULL) {
		printf("[%s,%d],failed\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	if (node == NULL) {
		printf("[%s,%d],failed\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	node_delete(&tree->root_ll, node);

	return ret;
exit:
	return ret;
}

lb_int32 tree_handle(tree_t *tree)
{
	lb_int32 ret = 0;

	if (tree == NULL) {
		printf("[%s,%d],failed\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	if (tree->root_node == NULL) {
		printf("[%s,%d],failed\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	if (node_hand(&tree->ohtbl, tree->root_node) != 0) {
		printf("[%s,%d],failed\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

exit:
	return ret;
}

lb_int32 tree_build(tree_t *tree)
{
	lb_int32 ret = 0;
	mars_dir dir = NULL;
	mars_dirent dirent = NULL;
	tree_node_t *par_node = NULL;
	tree_node_t *chi_node = NULL;
	char path[256];

	stack_push(tree->root_node);

	while (1 != stack_empty()) {
		stack_pop(&par_node);

		if (-1 == mars_opendir(&dir, par_node->path)) {
			printf("[%s,%d],failed\n",
				__func__, __LINE__);
			ret = -1;
			goto err0;
		}

		while (-1 != mars_readdir(&dir, &dirent)) {
			if (dirent->d_name[0] == '.')
				continue;

			strcpy(path, par_node->path);
			strcat(path, "/");
			strcat(path, dirent->d_name);

			if (1 == mars_isdir(&dirent)) {
				ret = tree_create_node(tree,
						&chi_node,
						par_node,
						path,
						NODE_DIR);

				if (ret != 0) {
					printf("[%s,%d],failed\n",
						__func__, __LINE__);
					ret = -1;
					goto err0;
				}

				if (1 == mars_validdir(path))
					stack_push(chi_node);

				mars_getmtime(path, &chi_node->st_mtime);
				mars_getsize(path, &chi_node->st_size);

				par_node->one_depth_dir++;
				par_node->one_depth_sons++;

			} else if (0 == mars_isdir(&dirent)) {
				ret = tree_create_node(tree,
						&chi_node,
						par_node,
						path,
						NODE_FILE);

				if (ret != 0) {
					printf("[%s,%d],failed\n",
						__func__, __LINE__);
					ret = -1;
					goto err0;
				}

				par_node->one_depth_file++;
				par_node->one_depth_sons++;
			}

			if (par_node->one_depth_sons >= MAX_ONE_DEPTH_NUM) {
				printf("[%s,%d],exceed max one depth\n",
					__func__, __LINE__);
				goto exit;
			}
		}

exit:
		if (dir) {
			mars_closedir(&dir);
			dir = NULL;
		}

		continue;

err0:
		if (dir) {
			mars_closedir(&dir);
			dir = NULL;
		}

		if (par_node) {
			tree_delete_node(tree, par_node);
			par_node = NULL;
		}

		if (chi_node) {
			tree_delete_node(tree, chi_node);
			chi_node = NULL;
		}
	}

	return ret;
}

lb_int32 tree_update(void *orit, void *newt)
{
	mars_dir dir;
	mars_dirent dirent;
	char path[256];
	tree_node_t *par_node;
	tree_node_t *chi_node;
	tree_node_t *key_node;
	tree_t *ori_tree;
	tree_t *new_tree;

	if (orit == NULL || newt == NULL) {
		printf("[%s,%d],failed\n", __func__, __LINE__);
		return -1;
	}
	ori_tree = (tree_t *)orit;
	new_tree = (tree_t *)newt;

	stack_push(new_tree->root_node);

	while (1 != stack_empty()) {
		stack_pop(&par_node);

		if (-1 == mars_opendir(&dir, par_node->path))
			return -1;

		while (-1 != mars_readdir(&dir, &dirent)) {
			if (dirent->d_name[0] == '.')
				continue;

			strcpy(path, par_node->path);
			strcat(path, "/");
			strcat(path, dirent->d_name);

			key_node = tree_get_node(ori_tree,
					ori_tree->root_node, path);

			if (key_node && (NODE_DIR == key_node->nd_type)) {
				if (update_dir(new_tree,
						par_node, key_node, path) == 0)
					continue;
			} else if (key_node && (NODE_FILE == key_node->nd_type)) {
				if (update_file(new_tree,
						par_node, key_node, path) == 0)
					continue;
			}

			if (1 == mars_isdir(&dirent)) {
				new_tree->node_num++;
				tree_create_node(new_tree,
					&chi_node,
					par_node,
					path,
					NODE_DIR);
				if (1 == mars_validdir(path))
					stack_push(chi_node);
			} else if (0 == mars_isdir(&dirent)) {
				new_tree->node_num++;
				tree_create_node(new_tree,
					&chi_node,
					par_node,
					path,
					NODE_FILE);
			}
		}
		mars_closedir(&dir);
	}

	return 0;
}

lb_int32 tree_store(void *tree)
{
	if (storage_init(tree) != 0) {
		printf("[%s,%d],failed\n", __func__, __LINE__);
		return -1;
	}

	if (storage_store(tree) != 0) {
		printf("[%s,%d],failed\n", __func__, __LINE__);
		return -1;
	}

	if (storage_exit(tree) != 0) {
		printf("[%s,%d],failed\n", __func__, __LINE__);
		return -1;
	}

	return 0;
}

lb_int32 tree_load(void *tree)
{
	if (storage_init(tree) != 0) {
		printf("[%s,%d],failed\n", __func__, __LINE__);
		return -1;
	}

	if (storage_load(tree) != 0) {
		printf("[%s,%d],failed\n", __func__, __LINE__);
		return -1;
	}

	if (storage_exit(tree) != 0) {
		printf("[%s,%d],failed\n", __func__, __LINE__);
		return -1;
	}

	return 0;
}
