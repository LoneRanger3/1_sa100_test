/*
 * oscl_mem.h - memory api used by lombo media framework.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef __OSCL_MEM_H__
#define __OSCL_MEM_H__

#define oscl_malloc(count)		rt_malloc(count)
#define oscl_zalloc(count)		rt_zalloc(count)
#define oscl_free(p)			rt_free(p)
#define oscl_malloc_align(count, align)	rt_malloc_align(count, align)
#define oscl_free_align(p)		rt_free_align(p)
#define oscl_malloc_unca_align(x, y)	rt_zalloc_unca_align(x, y)
#define oscl_free_unca_align(x)		rt_free_unca_align(x)
#define oscl_unca_to_phys(x)		unca_to_phys(x)
#define oscl_virt_to_phys(x)		virt_to_phys(x)
#define oscl_cache_flush_vir(x, y)				\
do {								\
	if (0 != rt_hw_cpu_dcache_status())			\
		rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, x, y);	\
} while (0)

#define oscl_strdup(str)		rt_strdup(str)
#define oscl_realloc(p, count)		rt_realloc(p, count)

#endif /* __OSCL_MEM_H__ */

