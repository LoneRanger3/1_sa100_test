/*
 * ohtbl.h - hash table code for LomboTech
 * hash table interface and macro define
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

#ifndef __OHTBL_H__
#define __OHTBL_H__

#include <stdlib.h>
#include <stdint.h>

#define TBLSIZ 16384
#define NHASH (TBLSIZ - 1)
#define MULT1 31
#define MULT2 57

/* Define a structure for open-addressed hash tables. */
typedef struct tag_ohtbl {
	int32_t	 positions;
	void	*vacated;

	int32_t (*h1)(const void *key);
	int32_t (*h2)(const void *key);
	int32_t (*match)(const void *key1, const void *key2);
	void	(*destroy)(void *data);

	int32_t size;
	void	**table;
} ohtbl_t;

/* Initializes the open-addressed hash table specified by htbl. */
int32_t ohtbl_init(ohtbl_t *htbl, int32_t positions,
	int32_t (*h1)(const void *key),
	int32_t (*h2)(const void *key),
	int32_t (*match)(const void *key1, const void *key2),
	void (*destroy)(void *data));

/* Destroys the open-addressed hash table specified by htbl. */
int32_t ohtbl_destroy(ohtbl_t *htbl);

/* Inserts an element into the open-addressed hash table specifiedby htbl. */
int32_t ohtbl_insert(ohtbl_t *htbl, const void *data);

int32_t ohtbl_remove(ohtbl_t *htbl, void **data);

int32_t ohtbl_lookup(const ohtbl_t *htbl, void **data);

int32_t match_str(const void *node1, const void *node2);
int32_t h1_str(const void *node);
int32_t h2_str(const void *node);

#define ohtbl_size(htbl) ((htbl)->size)

#endif/* __OHTBL_H__ */
