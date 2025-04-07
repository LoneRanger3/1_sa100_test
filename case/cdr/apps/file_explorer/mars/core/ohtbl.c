/*
 * ohtbl.c - hash table code for LomboTech
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
#include <mars/core/ohtbl.h>
#include <mars/core/ntree.h>
#include <mars/core/ntree_nd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Reserve a sentinel memory address for vacated elements. */
static char vacated;


int32_t ohtbl_init(ohtbl_t *htbl, int32_t positions,
	int32_t (*h1)(const void *key),
	int32_t (*h2)(const void *key),
	int32_t (*match)(const void *key1, const void *key2),
	void (*destroy)(void *data))
{
	int32_t i;

	htbl->table = (void **)mars_mem_alloc(positions * sizeof(void *));
	/* Allocate space for the hash table. */
	if (htbl->table == NULL)
		return -1;

	/* Initialize each position. */
	htbl->positions = positions;
	for (i = 0; i < htbl->positions; i++)
		htbl->table[i] = NULL;

	/* Set the vacated member to the sentinel memory address reserved for this. */
	htbl->vacated = &vacated;

	/* Encapsulate the functions. */
	htbl->h1 = h1;
	htbl->h2 = h2;
	htbl->match = match;
	htbl->destroy = destroy;

	/* Initialize the number of elements in the table. */
	htbl->size = 0;
	return 0;
}


int32_t ohtbl_destroy(ohtbl_t *htbl)
{
	int i;
	if (htbl->destroy != NULL) {
		/* Call a user-defined function to free dynamically allocated data. */
		for (i = 0; i < htbl->positions; i++) {
			if (htbl->table[i] != NULL && htbl->table[i] != htbl->vacated)
				htbl->destroy(htbl->table[i]);
		}
	}

	/* Free the storage allocated for the hash table. */
	mars_mem_free(htbl->table);

	/* No operations are allowed now, but clear the structure as a precaution. */
	memset(htbl, 0, sizeof(ohtbl_t));

	return 0;
}


int32_t ohtbl_insert(ohtbl_t *htbl, const void *data)
{
	void	*temp;
	int		position, i;

	/* Do not exceed the number of positions in the table. */
	if (htbl->size == htbl->positions) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return -1;
	}

	/* Do nothing if the data is already in the table. */
	temp = (void *)data;
	if (ohtbl_lookup(htbl, &temp) == 0) {
		printf("Do nothing if the data is already in the table.\n");
		return 1;
	}

	/* Use double hashing to hash the key. */
	for (i = 0; i < htbl->positions; i++) {
		position = (htbl->h1(data) +
				(i * htbl->h2(data))) % htbl->positions;
		if (htbl->table[position] == NULL ||
			htbl->table[position] == htbl->vacated) {
			/* Insert the data into the table. */
			htbl->table[position] = (void *)data;
			htbl->size++;
			return 0;
		}
	}

	/* Return that the hash functions were selected incorrectly. */
	printf("Return that the hash functions were selected incorrectly\n");

	return -1;
}


int32_t ohtbl_remove(ohtbl_t *htbl, void **data)
{
	int position, i;

	/* Use double hashing to hash the key. */
	for (i = 0; i < htbl->positions; i++) {
		position = (htbl->h1(*data) +
				(i * htbl->h2(*data))) % htbl->positions;
		if (htbl->table[position] == NULL) {
			/* Return that the data was not found. */
			return -1;
		} else if (htbl->table[position] == htbl->vacated) {
			/* Search beyond vacated positions. */
			continue;
		} else if (htbl->match(htbl->table[position], *data)) {
			/* Pass back the data from the table. */
			*data = htbl->table[position];
			htbl->table[position] = htbl->vacated;
			htbl->size--;
			return 0;
		}
	}

	/* Return that the data was not found. */
	return -1;
}


int32_t ohtbl_lookup(const ohtbl_t *htbl, void **data)
{
	int position, i;

	/* Use double hashing to hash the key. */
	for (i = 0; i < htbl->positions; i++) {
		position = (htbl->h1(*data) +
				(i * htbl->h2(*data))) % htbl->positions;

		if (htbl->table[position] == NULL) {
			/* Return that the data was not found. */
			return -1;
		} else if (htbl->match(htbl->table[position], *data)) {
			/* Pass back the data from the table. */
			/* printf("Pass back the data from the table.\n"); */
			*data = htbl->table[position];
			return 0;
		}
	}

	/* Return that the data was not found. */
	return -1;
}

int32_t match_str(const void *node1, const void *node2)
{
	tree_node_t *tree_node1;
	tree_node_t *tree_node2;

	tree_node1 = (tree_node_t *)node1;
	tree_node2 = (tree_node_t *)node2;

	return strcmp(tree_node1->path, tree_node2->path) ? 0 : 1;
}


int32_t h1_str(const void *node)
{
	uint32_t h = 1315423911;
	tree_node_t *tree_node;
	char *str;

	tree_node = (tree_node_t *)node;
	str = tree_node->path;

	while (*str)
		h ^= ((h << 5) + (*str++) + (h >> 2));

	return h % NHASH;
}

int32_t h2_str(const void *node)
{
	uint32_t h = 1315423911;
	tree_node_t *tree_node;
	char *str;

	tree_node = (tree_node_t *)node;
	str = tree_node->path;

	while (*str)
		h ^= ((h << 5) + (*str++) + (h >> 2));

	return h % NHASH;
}
