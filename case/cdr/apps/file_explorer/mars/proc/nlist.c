/*
 * nlist.c - a lot of list code for LomboTech
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

#include <mars/proc/nlist.h>

char *dir_table[] = {
	// "VIDEO_F",
	// "VIDEO_F_LOCK",
	// "VIDEO_R",
	// "VIDEO_R_LOCK",
	"F",
	"FO",
	"R",
	"RO",
	"VIDEO_TEST"
};

filter_t dir_filter = {
	0xff,
	dir_table
};

char *file_table[] = {
#ifdef MEDIA_TYPE_TS
	"ts",
	"TS",
#else
	"mp4",
	"MP4",
#endif
	"jpg",
	"JPG"
};

filter_t file_filter = {
	0xff,
	file_table
};

static lb_int32 nlist_copy_node(list_node_t *list_node, tree_node_t *tree_node)
{
	lb_int32 ret = 0;

	if (list_node == NULL) {
		printf("[%s,%d]list_node is null\n",
			__func__, __LINE__);
		ret = -1;
		goto exit;
	}

	if (tree_node == NULL) {
		printf("[%s,%d]tree_node is null\n",
			__func__, __LINE__);
		ret = -1;
		goto exit;
	}

	list_node->index++;
	list_node->nd_type = tree_node->nd_type;
	list_node->me_type = tree_node->me_type;
	list_node->lk_type = tree_node->lk_type;
	list_node->path = tree_node->path;
	list_node->name = tree_node->name;
	list_node->thumb = tree_node->thumb;

exit:
	return ret;
}

static lb_int32 nlist_dir_filter(const char *fn)
{
	lb_int32 i = 0;
	lb_int32 ret = 0;

	if (fn == NULL) {
		/* filter */
		ret = 1;
		goto exit;
	}

	/* do not filter */
	if (dir_filter.valid == 0x00) {
		ret = 0;
		goto exit;
	}

	for (i = 0; i < sizeof(dir_table) / sizeof(char *); i++) {
		if (dir_filter.table[i] != NULL) {
			ret = strcmp(fn, dir_filter.table[i]);
			/* do not filter */
			if (ret == 0) {
				ret = 0;
				goto exit;
			}
		}
	}

	/* filter */
	ret = 1;
exit:
	return ret;
}

static lb_int32 nlist_file_filter(const char *fn)
{
	lb_int32 i = 0;
	lb_int32 ret = 0;

	if (fn == NULL) {
		/* filter */
		ret = 1;
		goto exit;
	}

	/* do not filter */
	if (file_filter.valid == 0x00) {
		ret = 0;
		goto exit;
	}

	for (i = 0; i < sizeof(file_table) / sizeof(char *); i++) {
		if (file_filter.table[i] != NULL) {
			ret = strcmp(fn, file_filter.table[i]);
			/* do not filter */
			if (ret == 0) {
				ret = 0;
				goto exit;
			}
		}
	}

	/* filter */
	ret = 1;
exit:
	return ret;
}

static lb_int32 nlist_decide_filter(file_list_t *file_list, tree_node_t *node)
{
	lb_int32 ret = 0;
	char *name = NULL;

	if (file_list == NULL) {
		printf("[%s,%d]file_list is null\n",
			__func__, __LINE__);
		ret = -1;
		goto exit;
	}

	if (node == NULL) {
		printf("[%s,%d]node is null\n",
			__func__, __LINE__);
		ret = -1;
		goto exit;
	}

	if (node->nd_type == NODE_DIR) {
		if (file_list->nd_type == NODE_FILE) {
			ret = 1;
			goto exit;
		}

		name = strrchr(node->path, '/');
		if (name == NULL) {
			ret = 1;
			goto exit;
		}
		name++;

		if (nlist_dir_filter(name) == 1) {
			ret = 1;
			goto exit;
		}
	} else if (node->nd_type == NODE_FILE) {
		if (file_list->nd_type == NODE_DIR) {
			ret = 1;
			goto exit;
		}

		if (file_list->me_type != node->me_type &&
			file_list->me_type != MEDIA_ALL) {
			ret = 1;
			goto exit;
		}

		if (file_list->lk_type != node->lk_type &&
			file_list->lk_type != ALTYPE) {
			ret = 1;
			goto exit;
		}

		name = strrchr(node->path, '.');
		if (name == NULL) {
			ret = 1;
			goto exit;
		}
		name++;

		if (nlist_file_filter(name) == 1) {
			ret = 1;
			goto exit;
		}
	}

exit:
	return ret;
}

static lb_int32 nlist_alloc_child(file_list_t *file_list, tree_node_t *node)
{
	lb_int32 ret = 0;
	tree_node_t *i = NULL;
	tree_node_t *i_next = NULL;

	i = ll_get_head(&(node->child_ll));
	while (i != NULL) {
		if (file_list->act_num >= MAX_FILE_LIST) {
			printf("[%s,%d]exceed max act_num of array\n",
				__func__, __LINE__);
			ret = 0;
			goto exit;
		}

		if (nlist_decide_filter(file_list, i) == 0) {
			nlist_copy_node(file_list->temp_node, i);
			file_list->temp_node++;
			file_list->act_num++;

			if (file_list->act_num >= file_list->node_num) {
				list_node_t *new_list_node;
				lb_uint32 old_node_num;
				lb_uint32 new_node_num;

				old_node_num = file_list->node_num;
				new_node_num = old_node_num + MIN_FILE_LIST;
				new_list_node = mars_mem_alloc(new_node_num
						* sizeof(list_node_t));

				if (new_list_node == NULL) {
					printf("[%s,%d]realloc nodes failed for array\n",
						__func__, __LINE__);
					ret = -1;
					goto exit;
				}
				memset(new_list_node, 0x00,
					new_node_num * sizeof(list_node_t));
				memcpy(new_list_node, file_list->list_node,
					old_node_num * sizeof(list_node_t));

				mars_mem_free(file_list->list_node);

				file_list->list_node = new_list_node;
				file_list->node_num = new_node_num;

				file_list->temp_node = new_list_node;
				file_list->temp_node += old_node_num;
			}

			if (i->nd_type == NODE_FILE)
				file_list->file_num++;
			else
				file_list->dir_num++;
		}

		/* Get the next node before delete this */
		i_next = ll_get_next(&(node->child_ll), i);

		/* Call the recursive get to the child too */
		ret = nlist_alloc_child(file_list, i);
		if (ret != 0) {
			printf("[%s,%d]nlist alloc child failed\n",
				__func__, __LINE__);
			ret = -1;
			goto exit;
		}

		/* Set i to the next node */
		i = i_next;
	}

exit:
	return ret;
}

lb_int32 nlist_insert_node(file_list_t *file_list, const char *path)
{
	lb_int32 i = 0;
	lb_int32 j = 0;
	list_node_t *temp_node;
	list_node_t *curr_node;
	list_node_t *next_node;

	temp_node = (list_node_t *)file_list->list_node;

	for (i = 0; i < file_list->act_num; i++) {
		if (strcmp(temp_node->path, path) == 0)
			break;
		temp_node++;
	}

	if (i == file_list->act_num - 1)
		return 0;

	temp_node = (list_node_t *)file_list->list_node;

	for (j = i; j < file_list->act_num - 1; j++) {
		curr_node = &temp_node[j];
		next_node = &temp_node[j + 1];
		memcpy(curr_node, next_node, sizeof(list_node_t));
	}

	file_list->act_num++;

	return 0;
}

lb_int32 nlist_delete_node(file_list_t *file_list, const char *path)
{
	lb_int32 i = 0;
	lb_int32 j = 0;
	list_node_t *temp_node;
	list_node_t *curr_node;
	list_node_t *next_node;

	temp_node = (list_node_t *)file_list->list_node;

	for (i = 0; i < file_list->act_num; i++) {
		if (strcmp(temp_node->path, path) == 0)
			break;
		temp_node++;
	}


	if (i == file_list->act_num)
		return 0;


	temp_node = (list_node_t *)file_list->list_node;

	for (j = i; j < file_list->act_num - 1; j++) {
		curr_node = &temp_node[j];
		next_node = &temp_node[j + 1];
		memcpy(curr_node, next_node, sizeof(list_node_t));
	}


	memset(&temp_node[j], 0x00, sizeof(list_node_t));

	file_list->act_num--;

	return 0;
}

lb_int32 nlist_alloc_olevel(file_list_t *file_list, tree_node_t *node)
{
	lb_int32 ret = 0;
	tree_node_t *i = NULL;
	tree_node_t *i_next = NULL;
	list_node_t *temp_node = NULL;

	if (file_list == NULL) {
		printf("[%s,%d]file_list is null\n",
			__func__, __LINE__);
		ret = -1;
		goto exit;
	}

	if (node == NULL) {
		printf("[%s,%d]node is null\n",
			__func__, __LINE__);
		ret = -1;
		goto exit;
	}

	file_list->list_node = mars_mem_alloc(MIN_FILE_LIST * sizeof(list_node_t));
	if (file_list->list_node == NULL) {
		printf("[%s,%d]alloc nodes failed for array\n",
			__func__, __LINE__);
		ret = -1;
		goto exit;
	}
	memset(file_list->list_node, 0x00, MIN_FILE_LIST * sizeof(list_node_t));
	file_list->node_num = MIN_FILE_LIST;

	file_list->act_num = 0;
	file_list->file_num = 0;
	file_list->dir_num = 0;

	temp_node = (list_node_t *)file_list->list_node;

	i = ll_get_head(&(node->child_ll));
	while (NULL != i) {
		if (file_list->act_num >= MAX_FILE_LIST) {
			printf("[%s,%d]exceed max act_num of array\n",
				__func__, __LINE__);
			ret = 0;
			goto exit;
		}
		if (nlist_decide_filter(file_list, i) == 0) {
			nlist_copy_node(temp_node, i);
			temp_node++;
			file_list->act_num++;
			if (file_list->act_num >= file_list->node_num) {
				list_node_t *new_list_node;
				lb_uint32 old_node_num;
				lb_uint32 new_node_num;

				old_node_num = file_list->node_num;
				new_node_num = old_node_num + MIN_FILE_LIST;
				new_list_node =	mars_mem_alloc(new_node_num
						* sizeof(list_node_t));

				if (new_list_node == NULL) {
					printf("[%s,%d]realloc nodes failed for array\n",
						__func__, __LINE__);
					ret = -1;
					goto exit;
				}
				memset(new_list_node, 0x00,
					new_node_num * sizeof(list_node_t));
				memcpy(new_list_node, file_list->list_node,
					old_node_num * sizeof(list_node_t));

				mars_mem_free(file_list->list_node);

				file_list->list_node = new_list_node;
				file_list->node_num = new_node_num;

				temp_node = new_list_node;
				temp_node += old_node_num;
			}

			if (i->nd_type == NODE_FILE)
				file_list->file_num++;
			else
				file_list->dir_num++;
		}

		/* Get the next node before delete this */
		i_next = ll_get_next(&(node->child_ll), i);

		/* Set i to the next node */
		i = i_next;
	}

exit:
	if (ret == -1) {
		if (file_list) {
			if (file_list->list_node) {
				mars_mem_free(file_list->list_node);
				file_list->list_node = NULL;
			}

			file_list->act_num = 0;
			file_list->file_num = 0;
			file_list->dir_num = 0;
		}
	}

	return ret;
}

lb_int32 nlist_free_olevel(file_list_t *file_list)
{
	lb_int32 ret = 0;

	if (file_list == NULL) {
		printf("[%s,%d]file_list is null\n",
			__func__, __LINE__);
		ret = -1;
		goto exit;
	}

	if (file_list->list_node) {
		mars_mem_free(file_list->list_node);
		file_list->list_node = NULL;
	}

	file_list->act_num = 0;
	file_list->file_num = 0;
	file_list->dir_num = 0;

exit:
	return ret;
}

lb_int32 nlist_alloc_mlevel(file_list_t *file_list, tree_node_t *node)
{
	lb_int32 ret = 0;

	if (file_list == NULL) {
		printf("[%s,%d]file_list is null\n",
			__func__, __LINE__);
		ret = -1;
		goto exit;
	}

	if (node == NULL) {
		printf("[%s,%d]node is null\n",
			__func__, __LINE__);
		ret = -1;
		goto exit;
	}

	file_list->list_node = mars_mem_alloc(MIN_FILE_LIST * sizeof(list_node_t));
	if (file_list->list_node == NULL) {
		printf("[%s,%d]alloc nodes failed for array\n",
			__func__, __LINE__);
		ret = -1;
		goto exit;
	}
	memset(file_list->list_node, 0x00, MIN_FILE_LIST * sizeof(list_node_t));
	file_list->node_num = MIN_FILE_LIST;


	file_list->act_num = 0;
	file_list->file_num = 0;
	file_list->dir_num = 0;

	file_list->temp_node = (list_node_t *)file_list->list_node;
	ret = nlist_alloc_child(file_list, node);
	if (ret != 0) {
		printf("[%s,%d]nlist alloc child failed\n",
			__func__, __LINE__);
		ret = -1;
		goto exit;
	}

exit:
	if (ret == -1) {
		if (file_list) {
			if (file_list->list_node) {
				mars_mem_free(file_list->list_node);
				file_list->list_node = NULL;
			}

			file_list->act_num = 0;
			file_list->file_num = 0;
			file_list->dir_num = 0;
		}
	}

	return ret;
}

lb_int32 nlist_free_mlevel(file_list_t *file_list)
{
	lb_int32 ret = 0;

	if (file_list == NULL) {
		printf("[%s,%d]file_list is null\n",
			__func__, __LINE__);
		ret = -1;
		goto exit;
	}

	if (file_list->list_node) {
		mars_mem_free(file_list->list_node);
		file_list->list_node = NULL;
	}

	file_list->act_num = 0;
	file_list->file_num = 0;
	file_list->dir_num = 0;
	file_list->temp_node = NULL;

exit:
	return ret;
}
