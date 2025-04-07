/*
 * storage.c - store the list to storage code for LomboTech
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

#include <mars/proc/storage.h>
#include <mars/core/ntree.h>

lb_int32 addr_to_index(array_head_t *ahead, void *node)
{
	lb_int32 i = 0;
	array_elem_t *aelem = NULL;

	for (i = 0; i < ahead->anum; i++) {
		aelem = &ahead->aelem[i];
		if (aelem->node == node)
			return i;
	}

	return -1;
}

void *index_to_addr(array_head_t *ahead, lb_int32 index)
{
	array_elem_t *aelem = NULL;

	if (index >= 0) {
		aelem = &ahead->aelem[index];
		return aelem->node;
	}

	return NULL;
}

lb_int32 tree_to_array(tree_node_t *tree_node, array_head_t *ahead, lb_int32 index)
{
	array_elem_t *aelem = NULL;
	const char *name = NULL;

	aelem = &ahead->aelem[index];
	aelem->node = tree_node;
	aelem->index = index;
	aelem->nd_type = tree_node->nd_type;
	aelem->st_mtime = tree_node->st_mtime;
	aelem->st_size = tree_node->st_size;

	if (tree_node->par) {
		aelem->par_node = tree_node->par;
		aelem->par_index = addr_to_index(ahead, tree_node->par);
	} else {
		aelem->par_node = NULL;
		aelem->par_index = -1;
	}

	if (strcmp(tree_node->path, MARS_FD_PATH) != 0) {
		name = strrchr(tree_node->path, '/');
		if (name == NULL)
			name = tree_node->path;
		else
			name++;
	} else
		name = tree_node->path;

	aelem->str_len = strlen(name) + 1;
	aelem->str_data = mars_mem_alloc(aelem->str_len);
	strcpy(aelem->str_data, name);

	return 0;
}

lb_int32 storage_store_open(tree_t *tree)
{
	storage_head_t *shead;

	if (!tree->shead)
		return -1;

	shead = tree->shead;
	printf("%s,%d,shead->path:%s\n", __FILE__, __LINE__, shead->path);

	shead->file = fopen(shead->path, "wb");
	if (!shead->file) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return -1;
	}
	fseek(shead->file, 0, SEEK_SET);

	return 0;
}

lb_int32 storage_store_head(tree_t *tree)
{
	storage_head_t *shead;

	if (!tree->shead)
		return -1;

	shead = tree->shead;

	fwrite(&tree->node_num, 1, 4, shead->file);
	printf("%s,%d,tree->node_num:%d\n", __FILE__, __LINE__, tree->node_num);

	return 0;
}

lb_int32 storage_store_data(tree_t *tree)
{
	storage_head_t *shead;
	array_head_t *ahead;
	lb_int32 i = 0;

	if (!tree->shead || !tree->ahead)
		return -1;

	shead = tree->shead;
	ahead = tree->ahead;

	printf("%s,%d,shead->path:%s\n", __FILE__, __LINE__, shead->path);

	for (i = 0; i < ahead->anum; i++) {
		array_elem_t *aelem;
		aelem = &ahead->aelem[i];

		fwrite(&aelem->index, 1, sizeof(lb_int32), shead->file);
		fwrite(&aelem->par_index, 1, sizeof(lb_int32), shead->file);
		fwrite(&aelem->nd_type, 1, sizeof(lb_int32), shead->file);
		fwrite(&aelem->st_mtime, 1, sizeof(time_t), shead->file);
		fwrite(&aelem->st_size, 1, sizeof(lb_int32), shead->file);

		fwrite(&aelem->str_len, 1, sizeof(lb_int32), shead->file);
		fwrite(aelem->str_data, 1, aelem->str_len, shead->file);
	}

	printf("%s,%d,shead->path:%s\n", __FILE__, __LINE__, shead->path);

	return 0;
}

lb_int32 storage_store_close(tree_t *tree)
{
	storage_head_t *shead;

	if (!tree->shead)
		return -1;

	shead = tree->shead;

	if (shead->file) {
		fclose(shead->file);
		shead->file = NULL;
	}

	return 0;
}

lb_int32 storage_load_open(tree_t *tree)
{
	storage_head_t *shead;

	if (!tree->shead)
		return -1;

	shead = tree->shead;
	printf("%s,%d,shead->path:%s\n", __FILE__, __LINE__, shead->path);

	shead->file = fopen(shead->path, "rb");
	if (!shead->file) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return -1;
	}
	fseek(shead->file, 0, SEEK_SET);

	return 0;
}

lb_int32 storage_load_head(tree_t *tree)
{
	storage_head_t *shead;

	if (!tree->shead) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return -1;
	}
	shead = tree->shead;

	printf("%s,%d,shead->path:%s\n", __FILE__, __LINE__, shead->path);

	fread(&tree->node_num, 1, 4, shead->file);
	printf("%s,%d,node_num:%d\n", __FILE__, __LINE__, tree->node_num);

	return 0;
}

lb_int32 storage_load_data(tree_t *tree)
{
	storage_head_t *shead;
	array_head_t *ahead;
	lb_uint32 i;

	if (!tree->shead || !tree->ahead) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return -1;
	}
	shead = tree->shead;
	ahead = tree->ahead;

	printf("%s,%d,shead->path:%s\n", __FILE__, __LINE__, shead->path);

	for (i = 0; i < ahead->anum; i++) {
		array_elem_t *aelem;
		aelem = &ahead->aelem[i];

		fread(&aelem->index, 1, sizeof(lb_int32), shead->file);
		fread(&aelem->par_index, 1, sizeof(lb_int32), shead->file);
		fread(&aelem->nd_type, 1, sizeof(lb_int32), shead->file);
		fread(&aelem->st_mtime, 1, sizeof(time_t), shead->file);
		fread(&aelem->st_size, 1, sizeof(lb_int32), shead->file);

		fread(&aelem->str_len, 1, sizeof(lb_int32), shead->file);
		aelem->str_data = mars_mem_alloc(aelem->str_len);
		fread(aelem->str_data, 1, aelem->str_len, shead->file);
	}

	printf("%s,%d,shead->path:%s\n", __FILE__, __LINE__, shead->path);

	return 0;
}

lb_int32 storage_load_close(tree_t *tree)
{
	storage_head_t *shead;

	if (!tree->shead)
		return -1;

	shead = tree->shead;

	if (shead->file) {
		fclose(shead->file);
		shead->file = NULL;
	}

	return 0;
}

lb_int32 storage_array_init(tree_t *tree)
{
	array_head_t *ahead;

	ahead = mars_mem_alloc(sizeof(array_head_t));
	if (!ahead) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return -1;
	}

	ahead->anum =  tree->node_num;
	ahead->aelem = mars_mem_alloc(ahead->anum * sizeof(array_elem_t));
	if (!ahead->aelem) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return -1;
	}

	tree->ahead = ahead;

	return 0;
}

lb_int32 storage_array_exit(tree_t *tree)
{
	array_head_t *ahead;
	array_elem_t *aelem;
	lb_int32 i;

	if (!tree->ahead) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return -1;
	}

	ahead = tree->ahead;

	if (!ahead->aelem) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return -1;
	}

	for (i = 0; i < ahead->anum; i++) {
		aelem = &ahead->aelem[i];
		if (aelem->str_data) {
			mars_mem_free(aelem->str_data);
			aelem->str_data = NULL;
		}
	}

	mars_mem_free(ahead->aelem);
	ahead->aelem = NULL;

	mars_mem_free(tree->ahead);
	tree->ahead = NULL;

	return 0;
}

lb_int32 storage_init(void *head)
{
	storage_head_t *shead;
	char db_path[64];
	tree_t *tree = (tree_t *)head;

	shead = mars_mem_alloc(sizeof(storage_head_t));
	if (!shead) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return -1;
	}

	sprintf(db_path, "%s/storage.db", MARS_DB_PATH);

	shead->path = mars_mem_alloc(strlen(db_path) + 1);
	if (!shead->path) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return -1;
	}

	strcpy(shead->path, db_path);
	printf("%s,%d,db_path:%s\n", __FILE__, __LINE__, db_path);

	tree->shead = shead;

	return 0;
}

lb_int32 storage_exit(void *head)
{
	storage_head_t *shead;
	tree_t *tree = (tree_t *)head;

	if (!tree->shead) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return -1;
	}

	shead = tree->shead;

	if (!shead->path) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return -1;
	}

	mars_mem_free(shead->path);
	shead->path = NULL;

	mars_mem_free(tree->shead);
	tree->shead = NULL;

	return 0;
}

lb_int32 storage_store(void *head)
{
	tree_node_t *par_node;
	tree_node_t *chi_node;
	lb_uint32 i = 0;
	tree_t *tree = (tree_t *)head;

	if (storage_array_init(tree) != 0) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return -1;
	}

	if (tree->root_node->nd_type == NODE_DIR)
		stack_push(tree->root_node);

	while (1 != stack_empty()) {
		stack_pop(&par_node);

		chi_node = ll_get_head(&(par_node->child_ll));
		while (chi_node != NULL) {

			tree_to_array(chi_node, tree->ahead, i);
			i++;

			if (chi_node->nd_type == NODE_DIR)
				stack_push(chi_node);

			chi_node = ll_get_next(&(par_node->child_ll), chi_node);
		}
	}
	tree->node_num = i;

	if (storage_store_open(tree) != 0) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return -1;
	}

	if (storage_store_head(tree) != 0) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return -1;
	}

	if (storage_store_data(tree) != 0) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return -1;
	}

	if (storage_store_close(tree) != 0) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return -1;
	}

	if (storage_array_exit(tree) != 0) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return -1;
	}

	return 0;
}

lb_int32 storage_load(void *head)
{
	tree_node_t *par_node;
	array_elem_t *aelem;
	lb_uint32 i = 0;
	lb_int32 ret = 0;
	char path[256];
	tree_t *tree = (tree_t *)head;

	if (storage_load_open(tree) != 0) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return -1;
	}

	if (storage_load_head(tree) != 0) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return -1;
	}

	if (storage_array_init(tree) != 0) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return -1;
	}

	if (storage_load_data(tree) != 0) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return -1;
	}

	if (storage_load_close(tree) != 0) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return -1;
	}

	for (i = 0; i < tree->node_num; i++) {
		aelem = &tree->ahead->aelem[i];
		par_node = (tree_node_t *)index_to_addr(tree->ahead, aelem->par_index);
		if (par_node) {
			strcpy(path, par_node->path);
			strcat(path, "/");
			strcat(path, aelem->str_data);
		} else {
			strcpy(path, MARS_DB_PATH);
			strcat(path, "/");
			strcat(path, aelem->str_data);
			par_node = tree->root_node;
		}

		tree_create_node(tree,
			(tree_node_t **)&aelem->node,
			par_node,
			path,
			aelem->nd_type);

		((tree_node_t *)aelem->node)->st_mtime = aelem->st_mtime;
		((tree_node_t *)aelem->node)->st_size = aelem->st_size;

		ret = ohtbl_insert(&tree->ohtbl, aelem->node);
		if (ret != 0) {
			printf("%s,%d,failed\n", __FILE__, __LINE__);
			return -1;
		}
	}

	if (storage_array_exit(tree) != 0) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return -1;
	}

	return 0;
}
