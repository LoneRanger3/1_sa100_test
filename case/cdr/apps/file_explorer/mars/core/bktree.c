/*
 * bktree.c - bk tree code for LomboTech
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

#include <stdlib.h>
#include <string.h>
#include <mars/core/bktree.h>

bk_node_t *root;

static lb_int32 lev_calc(char *col, lb_int32 coll, char *row, lb_int32 rowl)
{
	lb_int32 lt, tp;
	lb_int32 i, j;
	lb_int32 array[128];

	if (__min(coll, rowl) == 0)
		return __max(coll, rowl);

	for (i = 0; i <= rowl; i++)
		array[i] = i;

	for (i = 1; i <= coll; i++) {
		lt = i - 1;
		array[0] = i;
		for (j = 1; j <= rowl; j++) {
			tp = array[j];
			if (row[j - 1] == col[i - 1])
				array[j] = lt;
			else
				array[j] = __min(array[j - 1], __min(array[j], lt)) + 1;
			lt = tp;
		}
	}

	return array[rowl];
}

static lb_int32 bk_create(bk_node_t **bn, list_node_t *ln)
{
	char *suffix;

	*bn = (bk_node_t *)mars_mem_alloc(sizeof(bk_node_t));
	if (*bn == NULL)
		return -1;

	suffix = strrchr(ln->name, '.');
	if (suffix == NULL)
		return -1;

	(*bn)->str = ln->name;
	(*bn)->len = suffix - ln->name;

	(*bn)->sons = 128;
	(*bn)->bn = mars_mem_alloc(sizeof(bk_node_t *) * 128);
	memset((*bn)->bn, 0x00, sizeof(bk_node_t *) * 128);

	(*bn)->ln = ln;

	return 0;
}

static lb_int32 bk_delete(bk_node_t **bn)
{
	if ((*bn)->bn)
		mars_mem_free((*bn)->bn);

	if ((*bn))
		mars_mem_free((*bn));

	return 0;
}

static lb_int32 bk_push(bk_node_t **bn, list_node_t *ln)
{
	lb_int32 dist;
	char *suffix;

	suffix = strrchr((ln)->name, '.');
	if (suffix == NULL)
		return -1;

	dist = lev_calc((*bn)->str, (*bn)->len, (ln)->name, suffix - (ln)->name);

	if ((*bn)->bn[dist] == NULL)
		bk_create(&((*bn)->bn[dist]), ln);
	else
		bk_push(&((*bn)->bn[dist]), ln);

	return 0;
}

static lb_int32 bk_pull(bk_node_t **bn)
{
	lb_int32 i;

	for (i = 0; i < (*bn)->sons; i++) {
		if ((*bn)->bn[i])
			bk_pull(&((*bn)->bn[i]));
	}

	bk_delete(&(*bn));

	return 0;
}

static lb_int32 bk_copy(file_list_t *sch_list, list_node_t *ln)
{
	memcpy(sch_list->temp_node, ln, sizeof(list_node_t));
	sch_list->temp_node++;
	sch_list->act_num++;

	/* The array is less than demand,so need to reallocate the memory */
	if (sch_list->act_num >= sch_list->node_num) {

		list_node_t *new_list_node;
		lb_uint32 new_node_num;

		new_node_num = sch_list->node_num + MIN_FILE_LIST;
		new_list_node = mars_mem_realloc(sch_list->list_node,
				new_node_num * sizeof(list_node_t));
		if (new_list_node == NULL) {
			printf("Reallocate the memory failed\n");
			return -1;
		}

		sch_list->list_node = new_list_node;
		sch_list->node_num = new_node_num;
	}

	if (ln->nd_type == NODE_FILE)
		sch_list->file_num++;
	else
		sch_list->dir_num++;

	return 0;
}

static lb_int32 bk_query(bk_node_t **bn, file_list_t *sch_list, void *str)
{
	lb_int32 dist;
	lb_int32 range;
	lb_int32 min;
	lb_int32 max;
	lb_int32 i;

	dist = lev_calc((*bn)->str, (*bn)->len, str, strlen(str));
	range = 2;

	if (dist <= range)
		bk_copy(sch_list, (*bn)->ln);

	min = __max(1, dist - range);
	max = dist + range;

	for (i = min; i <= max; i++) {
		if ((*bn)->bn[i] != NULL)
			bk_query(&((*bn)->bn[i]), sch_list, str);
	}

	return 0;
}

lb_int32 bk_build(file_list_t *file_list)
{
	lb_int32 i;
	list_node_t *temp_node;

	temp_node = (list_node_t *)file_list->list_node;

	if (file_list->act_num) {
		bk_create(&root, temp_node);
		temp_node++;

		for (i = 1; i < file_list->act_num; i++) {
			bk_push(&root, temp_node);
			temp_node++;
		}
	}

	return 0;
}

lb_int32 bk_destroy(file_list_t *file_list)
{
	bk_pull(&root);

	return 0;
}

lb_int32 bk_open(file_list_t *sch_list, char *str)
{
	sch_list->list_node = mars_mem_alloc(MIN_FILE_LIST * sizeof(list_node_t));
	if (sch_list->list_node == NULL) {
		printf("Allocate the memory failed\n");
		return -1;
	}
	memset(sch_list->list_node, 0x00, MIN_FILE_LIST * sizeof(list_node_t));

	sch_list->node_num = MIN_FILE_LIST;
	sch_list->act_num = 0;
	sch_list->file_num = 0;
	sch_list->dir_num = 0;

	sch_list->temp_node = (list_node_t *)sch_list->list_node;
	bk_query(&root, sch_list, str);

	return 0;
}

lb_int32 bk_close(file_list_t *sch_list)
{
	if (sch_list->list_node) {
		mars_mem_free(sch_list->list_node);
		sch_list->list_node = NULL;
	}

	return 0;
}
