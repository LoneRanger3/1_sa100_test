/*
 * mars.c - file man_system common code for LomboTech
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

#include <mars/mars.h>
#include <mars/core/linkl.h>
#include <mars/core/ntree.h>
#include <mars/core/ntree_nd.h>
#include <mars/proc/mtype.h>
#include <mars/proc/nlist.h>
#include <mars/proc/sort.h>

void *mars;

/**
 * mars_init - Initiallizes the file management module mars
 * @path: file path to manage
 *
 * returns 0 means successed,others mean failed
 *
 */
lb_int32 mars_init(const char *path)
{
	lb_int32 ret = 0;
	tree_t *orit = NULL;

#ifdef QUICK_RESOTRE
	tree_t *newt = NULL;
#endif

	mars_mkdir(MARS_DB_PATH);

#ifndef DATABASE_OMIT
	mars_mkdir(MARS_TB_VIDEO_PATH);
	mars_mkdir(MARS_TB_IMAGE_PATH);
#endif
	printf("[%s,%d],path:%s\n", __func__, __LINE__, path);
	if (stack_init() != 0) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	orit = tree_alloc(path);
	if (orit == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

#ifdef QUICK_RESOTRE
	if (tree_load(orit) == 0) {
		printf("Load the tree successed\n");

		newt = tree_alloc(path);
		if (newt == NULL) {
			printf("[%s,%d]\n", __func__, __LINE__);
			return -1;
		}
		printf("Alloc the new tree successed\n");

		if (tree_update(orit, newt) != 0) {
			printf("[%s,%d]\n", __func__, __LINE__);
			return -1;
		}
		printf("Update the tree successed\n");

		if (tree_statistic(newt) != 0) {
			printf("[%s,%d]\n", __func__, __LINE__);
			return -1;
		}
		printf("Statistic the tree successed\n");

		tree_free(orit);
		printf("Free the old tree successed\n");

		mars = (void *)newt;
		return 0;
	}
#endif

	if (tree_build(orit) == 0) {
		printf("[%s,%d],Build the tree successed\n", __func__, __LINE__);
#ifdef QUICK_RESOTRE
		if (tree_statistic(orit) != 0) {
			printf("[%s,%d]\n", __func__, __LINE__);
			return -1;
		}
		printf("Statistic the tree successed\n");
#endif
		mars = (void *)orit;
	}

	return ret;

exit:
	if (orit) {
		tree_free(orit);
		orit = NULL;
	}

	stack_exit();

	return ret;
}

/**
 * mars_exit - Exit the file management module mars
 *
 * returns 0 means successed
 *
 */
lb_int32 mars_exit(void)
{
	tree_t *tree = NULL;
	lb_int32 ret = 0;

	tree = (tree_t *)mars;

#ifdef QUICK_RESOTRE
	ret = tree_store(tree);
	if (ret == 0) {
		printf("Store the tree successed\n");
		mars = NULL;
	}
#endif

	tree_free(tree);

	stack_exit();

	return ret;
}

/**
 * mars_set_path - Sets the path to the list to be appiled
 * @path: the path to the list
 *
 * returns 0 means successed,-1 means failed
 *
 */
lb_int32 mars_set_path(char *path)
{
	lb_int32 ret = 0;
	tree_t *tree = NULL;
	list_conf_t *list_conf = NULL;

	tree = (tree_t *)mars;
	if (tree == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	list_conf = tree->list_conf;
	if (list_conf == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	if (path == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	strcpy(list_conf->path, path);

exit:
	return ret;
}

/**
 * mars_set_media - Sets the media type to the list to be appiled
 * @me_type: the media type to the list
 *
 * returns 0 means successed,-1 means failed
 *
 */
lb_int32 mars_set_media(media_type_t me_type)
{
	lb_int32 ret = 0;
	tree_t *tree = NULL;
	list_conf_t *list_conf = NULL;

	tree = (tree_t *)mars;
	if (tree == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	list_conf = tree->list_conf;
	if (list_conf == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	list_conf->me_type = me_type;

exit:
	return ret;
}

/**
 * mars_set_node Sets - the node type to the list to be appiled
 * @nd_type: the node type to the list
 *
 * returns 0 means successed,-1 means failed
 *
 */
lb_int32 mars_set_node(node_type_t nd_type)
{
	lb_int32 ret = 0;
	tree_t *tree = NULL;
	list_conf_t *list_conf = NULL;

	tree = (tree_t *)mars;
	if (tree == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	list_conf = tree->list_conf;
	if (list_conf == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	list_conf->nd_type = nd_type;

exit:
	return ret;
}

/**
 * mars_set_query - Sets the query type to the list to be appiled
 * @qu_type: the query type to the list
 *
 * returns 0 means successed,-1 means failed
 *
 */
lb_int32 mars_set_query(query_type_t qu_type)
{
	lb_int32 ret = 0;
	tree_t *tree = NULL;
	list_conf_t *list_conf = NULL;

	tree = (tree_t *)mars;
	if (tree == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	list_conf = tree->list_conf;
	if (list_conf == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	list_conf->qu_type = qu_type;

exit:
	return ret;
}

/**
 * mars_set_sort - Sets the sort type to the list to be appiled
 * @st_type: the sort type to the list
 *
 * returns 0 means successed,-1 means failed
 *
 */
lb_int32 mars_set_sort(sort_type_t st_type)
{
	lb_int32 ret = 0;
	tree_t *tree = NULL;
	list_conf_t *list_conf = NULL;

	tree = (tree_t *)mars;
	if (tree == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	list_conf = tree->list_conf;
	if (list_conf == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	list_conf->st_type = st_type;

exit:
	return ret;
}

/**
 * mars_set_lock - Sets the lock type to the list to be appiled
 * @st_type: the lock type to the list
 *
 * returns 0 means successed,-1 means failed
 *
 */
lb_int32 mars_set_lock(lock_type_t lk_type)
{
	lb_int32 ret = 0;
	tree_t *tree = NULL;
	list_conf_t *list_conf = NULL;

	tree = (tree_t *)mars;
	if (tree == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	list_conf = tree->list_conf;
	if (list_conf == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	list_conf->lk_type = lk_type;

exit:
	return ret;
}

/**
 * mars_alloc_list - Alloc a list of files
 *
 * returns non-null means successed,null means failed
 *
 */
lb_int32 mars_alloc_list(void **desert)
{
	lb_int32 ret = 0;
	tree_t *tree = NULL;
	tree_node_t *tree_node = NULL;
	file_list_t *file_list = NULL;
	list_conf_t *list_conf = NULL;

	tree = (tree_t *)mars;
	if (tree == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	list_conf = (list_conf_t *)tree->list_conf;
	if (list_conf == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	if (desert == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	file_list = (file_list_t *)mars_mem_alloc(sizeof(file_list_t));
	if (file_list == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}
	memset(file_list, 0x00, sizeof(file_list_t));

	tree->file_list = file_list;
	file_list->mars = tree;

	tree_node = tree_get_node(tree, tree->root_node, list_conf->path);
	if (tree_node == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	file_list->cur_node = tree_node;
	file_list->cur_path = tree_node->path;

	if (tree_node->par) {
		file_list->par_node = tree_node->par;
		file_list->par_path = tree_node->par->path;
	} else {
		file_list->par_node = NULL;
		file_list->par_path = NULL;
	}

	file_list->me_type = list_conf->me_type;
	file_list->qu_type = list_conf->qu_type;
	file_list->st_type = list_conf->st_type;
	file_list->nd_type = list_conf->nd_type;
	file_list->lk_type = list_conf->lk_type;

	if (file_list->qu_type == RECURSIVE) {
		ret = nlist_alloc_mlevel(file_list, tree_node);
		if (ret != 0) {
			printf("[[%s,%d]]\n", __func__, __LINE__);
			ret = -1;
			goto exit;
		}
	} else {
		ret = nlist_alloc_olevel(file_list, tree_node);
		if (ret != 0) {
			printf("[[%s,%d]]\n", __func__, __LINE__);
			ret = -1;
			goto exit;
		}
	}

	ret = sort_list(file_list);
	if (ret != 0) {
		printf("[[%s,%d]]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	*desert = file_list;

	return ret;
exit:
	if (file_list) {
		if (file_list->qu_type == RECURSIVE)
			nlist_free_mlevel(file_list);
		else
			nlist_free_olevel(file_list);

		mars_mem_free(file_list);
	}

	return ret;
}

/**
 * mars_free_list - Free a list of files
 * @desert: the list pointer to be freed
 *
 * returns 0 means successed,-1 means failed
 *
 */
lb_int32 mars_free_list(void *desert)
{
	lb_int32 ret = 0;
	file_list_t *file_list;

	file_list = (file_list_t *)desert;
	if (file_list == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	if (file_list->qu_type == RECURSIVE) {
		nlist_free_mlevel(file_list);
		mars_mem_free(file_list);
	} else {
		nlist_free_olevel(file_list);
		mars_mem_free(file_list);
	}

exit:
	return ret;
}

/**
 * mars_get_node_num - Get the total numbers of list items
 * @desert: the list pointer to be get
 *
 * returns -1 means failed,others mean successed
 *
 */
lb_int32 mars_get_node_num(void *desert)
{
	file_list_t *file_list = NULL;

	file_list = (file_list_t *)desert;
	if (file_list == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		return -1;
	}

	return file_list->act_num;
}

/**
 * mars_get_node_lock - Get the lock type of the item
 * @desert: the list pointer to be get
 * @index:  the index of item
 *
 * returns UNKNOWN means failed,others mean successed
 *
 */
lock_type_t mars_get_node_lock(void *desert, lb_uint32 index)
{
	file_list_t *file_list;
	list_node_t *list_node;

	file_list = (file_list_t *)desert;
	if (file_list == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		return ALTYPE;
	}

	list_node = (list_node_t *)file_list->list_node;
	if (list_node == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		return ALTYPE;
	}

	return (list_node + index)->lk_type;
}

/**
 * mars_get_node_media - Get the media type of the item
 * @desert: the list pointer to be get
 * @index:  the index of item
 *
 * returns MEDIA_UNKNOWN means failed,others mean successed
 *
 */
media_type_t mars_get_node_media(void *desert, lb_uint32 index)
{
	file_list_t *file_list;
	list_node_t *list_node;

	file_list = (file_list_t *)desert;
	if (file_list == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		return MEDIA_UNKNOWN;
	}

	list_node = (list_node_t *)file_list->list_node;
	if (list_node == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		return MEDIA_UNKNOWN;
	}

	return (list_node + index)->me_type;
}

/**
 * mars_get_node_type - Get the node type of the item
 * @desert: the list pointer to be get
 * @index:  the index of item
 *
 * returns NODE_UNKNOWN means failed,others mean successed
 *
 */
node_type_t mars_get_node_type(void *desert, lb_uint32 index)
{
	tree_t *tree;
	file_list_t *file_list;
	list_node_t *list_node;

	tree = (tree_t *)mars;
	if (tree == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		return NODE_UNKNOWN;
	}

	file_list = (file_list_t *)desert;
	if (file_list == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		return NODE_UNKNOWN;
	}

	list_node = (list_node_t *)file_list->list_node;
	if (list_node == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		return NODE_UNKNOWN;
	}

	return (list_node + index)->nd_type;
}

/**
 * mars_get_node_path - Get the node path of the item
 * @desert: the list pointer to be get
 * @index:  the index of item
 * @path:   the secondary pointer of the path
 *
 * returns 0 means successed,-1 means failed
 *
 */
lb_int32 mars_get_node_path(void *desert, lb_uint32 index, char **path)
{
	lb_int32 ret = 0;
	file_list_t *file_list = NULL;
	list_node_t *list_node = NULL;

	file_list = (file_list_t *)desert;
	if (file_list == NULL)  {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	list_node = (list_node_t *)file_list->list_node;
	if (list_node == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	if (path == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	if ((list_node + index)->path == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	*path = (list_node + index)->path;

exit:
	return ret;
}

/**
 * mars_get_node_thumb - Get the node thumb of the item
 * @desert: the list pointer to be get
 * @index:  the index of item
 * @thumb:   the secondary pointer of the thumb
 *
 * returns 0 means successed,-1 means failed
 *
 */
lb_int32 mars_get_node_thumb(void *desert, lb_uint32 index, char **thumb)
{
	lb_int32 ret = 0;
	file_list_t *file_list = NULL;
	list_node_t *list_node = NULL;

	file_list = (file_list_t *)desert;
	if (file_list == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	list_node = (list_node_t *)file_list->list_node;
	if (list_node == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	if ((list_node + index)->thumb == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	*thumb = (list_node + index)->thumb;

exit:
	return ret;
}

/**
 * mars_get_node_name  - Get the node name of the item
 * @desert: the list pointer to be get
 * @index:  the index of item
 * @name:   the secondary pointer of the name
 *
 * returns 0 means successed,-1 means failed
 *
 */
lb_int32 mars_get_node_name(void *desert, lb_uint32 index, char **name)
{
	lb_int32 ret = 0;
	file_list_t *file_list = NULL;
	list_node_t *list_node = NULL;

	file_list = (file_list_t *)desert;
	if (file_list == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	list_node = (list_node_t *)file_list->list_node;
	if (list_node == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	if (name == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	if ((list_node + index)->name == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	*name = (list_node + index)->name;

exit:
	return ret;
}

/**
 * mars_get_current_path - Get the current path of the list
 * @desert: the list pointer to be get
 * @path:   the secondary pointer of the path
 *
 * returns 0 means successed,-1 means failed
 *
 */
lb_int32 mars_get_current_path(void *desert, char **path)
{
	lb_int32 ret = 0;
	file_list_t *file_list = NULL;

	file_list = (file_list_t *)desert;
	if (file_list == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	if (path == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	*path = file_list->cur_path;

exit:
	return ret;
}

/**
 * mars_get_parent_path - Get the parent path of the list
 * @desert: the list pointer to be get
 * @path:   the secondary pointer of the path
 *
 * returns 0 means successed,-1 means failed
 *
 */
lb_int32 mars_get_parent_path(void *desert, char **path)
{
	lb_int32 ret = 0;
	file_list_t *file_list = NULL;

	file_list = (file_list_t *)desert;
	if (file_list == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	if (path == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	*path = file_list->par_path;

exit:
	return ret;
}

/**
 * mars_set_node_lock - set the lock type of the item
 * @desert: the list pointer to be set
 * @index:  the index of the item
 * @lock:   the lock type of the item
 *
 * returns 0 means successed,-1 means failed
 *
 */
lb_int32 mars_set_node_lock(void *desert, lb_uint32 index,
	lock_type_t lk_type)
{
	lb_int32 ret = 0;
	tree_t *tree = NULL;
	tree_node_t *tree_node = NULL;
	file_list_t *file_list = NULL;
	list_node_t *list_node = NULL;

	file_list = (file_list_t *)desert;
	if (file_list == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	tree = (tree_t *)file_list->mars;
	if (tree == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	list_node = (list_node_t *)file_list->list_node;
	if (list_node == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	(list_node + index)->lk_type = lk_type;

	tree_node = tree_get_node(tree,
			file_list->cur_node, (list_node + index)->path);
	if (tree_node == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	tree_node->lk_type = lk_type;

exit:
	return ret;
}

/**
 * mars_create_file - Create the file through mars
 * @desert: the list pointer in which create the file
 * @name:   the file name to be created
 * @flags:  the open mode of file
 *
 * returns 0 means successed,-1 means failed
 *
 */
lb_int32 mars_create_file(void *desert, const char *name, const char *flags)
{
	lb_int32 ret = 0;
	tree_t *tree = NULL;
	file_list_t *file_list = NULL;

	file_list = (file_list_t *)desert;
	if (file_list == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	tree = (tree_t *)file_list->mars;
	if (tree == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	if (name == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

exit:
	return ret;
}

/**
 * mars_delete_file - Delete the file through mars
 * @desert: the list pointer in which delete the file
 * @name:   the file name to be deleted
 *
 * returns 0 means successed,-1 means failed
 *
 */
lb_int32 mars_delete_file(void *desert, const char *name)
{
	lb_int32 ret = 0;
	tree_t *tree = NULL;
	tree_node_t *tree_node = NULL;
	file_list_t *file_list = NULL;
	tree_node_t *par_node = NULL;

	file_list = (file_list_t *)desert;
	if (file_list == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	tree = (tree_t *)file_list->mars;
	if (tree == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	par_node = tree_get_parent(tree, tree->root_node, name);
	if (par_node == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	tree_node = tree_get_node(tree, par_node, name);
	if (tree_node == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	if (mars_remove(name) != 0) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	if (nlist_delete_node(file_list, name) != 0) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	if (tree_delete_node(tree, tree_node) != 0) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}
exit:
	return ret;
}

/**
 * mars_rename_file - Rename the file through mars
 * @desert: the list pointer in which Rename the file
 * @name:   the file name to be Renamed
 *
 * returns 0 means successed,-1 means failed
 *
 */
lb_int32 mars_rename_file(void *desert, const char *oldf, const char *newf)
{
	lb_int32 ret = 0;
	tree_t *tree = NULL;
	tree_node_t *old_node = NULL;
	tree_node_t *new_node = NULL;
	file_list_t *file_list = NULL;
	tree_node_t *par_node = NULL;
	void *file_p = NULL;

	file_list = (file_list_t *)desert;
	if (file_list == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	tree = (tree_t *)file_list->mars;
	if (tree == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	if (mars_fopen(&file_p, newf, "r") == 0) {
		if (file_p) {
			mars_fclose(&file_p);
			file_p = NULL;
		}

		par_node = tree_get_parent(tree, tree->root_node, newf);
		if (par_node == NULL) {
			printf("[%s,%d]\n", __func__, __LINE__);
			ret = -1;
			goto exit;
		}

		old_node = tree_get_node(tree, par_node, newf);
		if (old_node == NULL) {
			printf("[%s,%d]\n", __func__, __LINE__);
			ret = -1;
			goto exit;
		}

		if (mars_remove(newf) != 0) {
			printf("[%s,%d]\n", __func__, __LINE__);
			ret = -1;
			goto exit;
		}

		if (tree_delete_node(tree, old_node) != 0) {
			printf("[%s,%d]\n", __func__, __LINE__);
			ret = -1;
			goto exit;
		}

	}

	if (mars_rename(oldf, newf) != 0) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	par_node = tree_get_parent(tree, tree->root_node, oldf);
	if (par_node == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	old_node = tree_get_node(tree, par_node, oldf);
	if (old_node == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	if (nlist_delete_node(file_list, oldf) != 0) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	if (tree_delete_node(tree, old_node) != 0) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	par_node = tree_get_parent(tree, tree->root_node, newf);
	if (par_node == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	if (tree_create_node(tree, &new_node, par_node, newf, NODE_FILE) != 0) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

exit:
	return ret;
}
