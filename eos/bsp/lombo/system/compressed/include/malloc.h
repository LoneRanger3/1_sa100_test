/*
 * malloc.c - memory operations for compress kernel
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

#ifndef __COMPRESS_MALLOC_H__
#define __COMPRESS_MALLOC_H__

/* A trivial malloc implementation, adapted from
 *  malloc by Hannu Savolainen 1993 and Matthias Urlichs 1994
 */
unsigned long malloc_ptr;
int malloc_count;

void *malloc(int size)
{
	void *p;

	if (size < 0)
		return NULL;

	if (!malloc_ptr)
		malloc_ptr = free_mem_ptr;

	malloc_ptr = (malloc_ptr + 3) & ~3;     /* Align */
	p = (void *)malloc_ptr;
	malloc_ptr += size;

	if (free_mem_end_ptr && malloc_ptr >= free_mem_end_ptr)
		return NULL;

	malloc_count++;
	return p;
}

void free(void *where)
{
	malloc_count--;
	if (!malloc_count)
		malloc_ptr = free_mem_ptr;
}

#define large_malloc(a) malloc(a)
#define large_free(a) free(a)

#define INIT

#endif /* __COMPRESS_MALLOC_H__ */
