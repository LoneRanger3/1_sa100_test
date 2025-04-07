/*
 * osal_mem.h - head file for memory debug
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

#ifndef __OSAL_MEM_H__
#define __OSAL_MEM_H__

#ifndef EE_GLOBAL_MEM_DEBUG
#include <rtthread.h>
#endif

#ifdef EE_MEM_DEBUG_BORDER_CHECK
#define MEM_BORDER_CHECK	1
#else
#define MEM_BORDER_CHECK	0
#endif

#define MEM_BORDER_LEN		8
#define MEM_BORDER_TAG		"lombotech"

#define rt_malloc(size)			osal_malloc(size, __func__, __LINE__)
#define rt_calloc(nelem, elsize)	osal_calloc(nelem, elsize, __func__, __LINE__)
#define rt_free(ptr)			osal_free(ptr, __func__, __LINE__)

#define rt_malloc_align(size, align)	osal_malloc_align(size, align, __func__, __LINE__)
#define rt_free_align(ptr)		osal_free_align(ptr, __func__, __LINE__)

#define rt_malloc_unca(size)		osal_malloc_unca(size, __func__, __LINE__)
#define rt_free_unca(ptr)		osal_free_unca(ptr, __func__, __LINE__)
#define rt_zalloc(size)			osal_zalloc(size, __func__, __LINE__)
#define rt_zalloc_unca(size)		osal_zalloc_unca(size, __func__, __LINE__)
#define rt_zalloc_unca_align(x, y)	osal_zalloc_unca_align(x, y, __func__, __LINE__)
#define rt_free_unca_align(ptr)		osal_free_unca_align(ptr, __func__, __LINE__)

/* temperary ignore these functions */
/*
#define memset(s, c, n)		osal_memset(s, c, n, __func__, __LINE__)
#define rt_memset(s, c, n)	osal_rt_memset(s, c, n, __func__, __LINE__)

#define memcpy(d, s, l)		osal_memcpy(d, s, l, __func__, __LINE__)
#define rt_memcpy(d, s, l)	osal_rt_memcpy(d, s, l, __func__, __LINE__)
*/

void *osal_malloc(rt_size_t size, const char *func, rt_uint32_t line);
void *osal_calloc(rt_size_t nelem, rt_size_t elsize,
	const char *func, rt_uint32_t line);
void osal_free(void *ptr, const char *func, rt_uint32_t line);

void *osal_malloc_align(rt_size_t size, rt_size_t align,
		 const char *func, rt_uint32_t line);
void osal_free_align(void *ptr, const char *func, rt_uint32_t line);

void *osal_malloc_unca(rt_size_t x, const char *func, rt_uint32_t line);
void osal_free_unca(void *ptr, const char *func, rt_uint32_t line);
void *osal_zalloc(rt_size_t x, const char *func, rt_uint32_t line);
void *osal_zalloc_unca(rt_size_t x, const char *func, rt_uint32_t line);
void *osal_zalloc_unca_align(rt_size_t x, rt_size_t y,
					const char *func, rt_uint32_t line);
void osal_free_unca_align(void *ptr, const char *func, rt_uint32_t line);

void osal_memset(void *s, int c, size_t count,
			const char *func, rt_uint32_t line);
void osal_rt_memset(void *s, int c, rt_ubase_t count,
			const char *func, rt_uint32_t line);

void osal_memcpy(void *dst, const void *src, rt_ubase_t count,
			const char *func, rt_uint32_t line);
void osal_rt_memcpy(void *dst, const void *src, rt_ubase_t count,
			const char *func, rt_uint32_t line);

void osal_dump();

/* declare here for test */
rt_bool_t osal_find_ptr(void *ptr, int *index);
rt_bool_t osal_ptr_at_range(void *ptr, int *index);
rt_bool_t osal_check_border(void *ptr, const char *func, rt_uint32_t line);
rt_bool_t osal_check_over_range(void *ptr, unsigned long count,
					const char *func, rt_uint32_t line);

#endif /* __OSAL_MEM_H__ */
