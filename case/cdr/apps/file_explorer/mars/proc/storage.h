/*
 * storage.h - store the list to storage code for LomboTech
 * store the list to storage interface and macro define
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

#ifndef __STORAGE_H__
#define __STORAGE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <mars/mars.h>

typedef struct tag_array_elem {
	/* -1 means illegal, others means index in array */
	lb_int32 index;
	/* -1 means no parent, others means index in array */
	lb_int32 par_index;
	/* 1 means dir, 2 means file, others means unknown */
	lb_int32 nd_type;

	/* The flag means whether the node can be loaded */
	time_t st_mtime;
	/* The flag means whether the node can be loaded */
	lb_int32 st_size;

	void *node;
	void *par_node;
	lb_int32 str_len;
	char *str_data;
} array_elem_t;

typedef struct tag_array_head {
	/* Number of array */
	lb_uint32 anum;
	/* Array of elements */
	array_elem_t *aelem;
} array_head_t;

typedef struct tag_storage_head {
	FILE *file;
	char *path;
} storage_head_t;

lb_int32 storage_init(void *tree);
lb_int32 storage_exit(void *tree);
lb_int32 storage_store(void *tree);
lb_int32 storage_load(void *tree);

#endif /* __STORAGE_H__ */

