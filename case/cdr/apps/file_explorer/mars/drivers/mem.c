/*
 * mem.c - sub mem system code for LomboTech
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

#include <mars/drivers/mem.h>

#ifdef mars_mem_manage
lb_points_t lb_points[MAX_NUM];

static void add_point(void *addr, int size, int line, const char *func)
{
	int i = 0;
	for (i = 0; i < MAX_NUM; i++) {
		if (0 == lb_points[i].is_empty) {
			lb_points[i].addr = addr;
			lb_points[i].size = size;
			lb_points[i].line = line;
			lb_points[i].is_empty = 1;
			if (NULL != func)
				memcpy(lb_points[i].func, func, strlen(func));
			break;
		}
	}
}

static void del_point(void *addr)
{
	int	i = 0;

	for (i = 0; i < MAX_NUM; i++) {
		if (addr == lb_points[i].addr) {
			lb_points[i].addr = NULL;
			lb_points[i].size = 0;
			lb_points[i].line = 0;
			lb_points[i].is_empty = 0;
			memset(lb_points[i].func, 0, 30);
			break;
		}
	}
}

void mars_mem_dump(void)
{
	int i = 0;

	for (i = 0; i < MAX_NUM; i++) {
		if (0 != lb_points[i].is_empty) {
			printf("\n i : %d", i);
			printf("  line : %d", lb_points[i].line);
			printf(" addr : 0x%x", (unsigned int)lb_points[i].addr);
			printf(" size : %d", lb_points[i].size);
			if (NULL != lb_points[i].func)
				printf(" func : %s", lb_points[i].func);
			printf("  is_empty : %d\n", lb_points[i].is_empty);
		}
	}
}
#endif

void *mars_mem_alloc_ext(int size, const char *func, int lines)
{
	void *ret;

	if (size <= 0)
		return NULL;
	ret = calloc(size, 1);
	if (NULL == ret)
		return NULL;

#ifdef mars_mem_manage
	add_point(ret, size, lines, func);
#endif
	return ret;
}

void *mars_mem_realloc_ext(void *p, int size, const char *func, int lines)
{
	void *ret;

	if (NULL == p)
		return NULL;

	if (size <= 0)
		return NULL;

	ret = realloc(p, size);
	if (NULL == ret)
		return NULL;

#ifdef mars_mem_manage
	del_point(p);
	add_point(ret, size, lines, func);
#endif
	return ret;
}

void mars_mem_free(void *p)
{
	if (NULL == p)
		return;

#ifdef mars_mem_manage
	del_point(p);
#endif
	free(p);
}
