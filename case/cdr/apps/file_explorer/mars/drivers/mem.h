/*
 * mem.h - sub mem system code for LomboTech
 * sub men system interface and macro define
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

#ifndef __MEM_H__
#define __MEM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <mars/mars.h>

#define MAX_NUM   2048
#define mars_mem_manage
#define mars_mem_alloc(s) mars_mem_alloc_ext(s, __func__, __LINE__)
#define mars_mem_realloc(s, t) mars_mem_realloc_ext(s, t, __func__, __LINE__)

typedef struct lb_tag_points {
	void	*addr;
	int	size;
	int	line;
	char	func[30];
	int	is_empty;
} lb_points_t;

void *mars_mem_alloc_ext(int size, const char *func, int lines);
void *mars_mem_realloc_ext(void *p, int size, const char *func, int lines);
void mars_mem_dump(void);
void mars_mem_free(void *p);

#ifdef __cplusplus
}
#endif

#endif /* __MEM_H__ */

