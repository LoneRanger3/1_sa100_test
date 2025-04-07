/*
 * cache.c - Cache operations for Lombo Socs
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

#include <rtthread.h>
#include <rthw.h>
#include <debug.h>
#ifdef ARCH_LOMBO_N7
#include "cache_op.h"
#endif

/**
 * rt_hw_cpu_icache_status - check if icache enabled
 *
 * return 1 if enabled, 0 otherwise
 */
rt_base_t rt_hw_cpu_icache_status(void)
{
#if defined(ARCH_LOMBO_N7)
	return (rt_base_t)n7_is_icache_en();
#elif defined(ARCH_LOMBO_N8V0)
	LOG_W("todo###");
	return 0;
#else
	RT_ASSERT(0);
#endif
}

/**
 * rt_hw_cpu_dcache_status - check if dcache enabled
 *
 * return 1 if enabled, 0 otherwise
 */
rt_base_t rt_hw_cpu_dcache_status(void)
{
#if defined(ARCH_LOMBO_N7)
	return (rt_base_t)n7_is_dcache_en();
#elif defined(ARCH_LOMBO_N8V0)
	LOG_W("todo###");
	return 0;
#else
	RT_ASSERT(0);
#endif
}

/**
 * rt_hw_cpu_dcache_flush_all - writeback and invalidate all dcache
 *
 */
void rt_hw_cpu_dcache_flush_all(void)
{
#if defined(ARCH_LOMBO_N7)
	n7_flush_dcache_all();
#elif defined(ARCH_LOMBO_N8V0)
	LOG_W("todo###");
#else
	RT_ASSERT(0);
#endif
}

/**
 * rt_hw_cpu_dcache_inval_all - invalidate all dcache
 *
 */
void rt_hw_cpu_dcache_inval_all(void)
{
#if defined(ARCH_LOMBO_N7)
	n7_inv_dcache_all();
#elif defined(ARCH_LOMBO_N8V0)
	LOG_W("todo###");
#else
	RT_ASSERT(0);
#endif
}

/**
 * rt_hw_cpu_icache_inval_all - invalidate all dcache
 *
 */
void rt_hw_cpu_icache_inval_all(void)
{
#if defined(ARCH_LOMBO_N7)
	n7_inv_icache_all();
#elif defined(ARCH_LOMBO_N8V0)
	LOG_W("todo###");
#else
	RT_ASSERT(0);
#endif
}

#ifdef ARCH_LOMBO_N7
rt_inline rt_uint32_t n7_icache_line_size(void)
{
	rt_uint32_t ctr;

	asm volatile ("mrc p15, 0, %0, c0, c0, 1" : "=r"(ctr));
	return 4 << (ctr & 0xF);
}

void n7_inv_icache_range(void *addr, int size)
{
	rt_uint32_t line_size = n7_icache_line_size();
	rt_uint32_t start_addr = (rt_uint32_t)addr;
	rt_uint32_t end_addr = (rt_uint32_t) addr + size + line_size - 1;

	start_addr &= ~(line_size - 1);
	end_addr &= ~(line_size - 1);
	while (start_addr < end_addr) {
		asm volatile ("mcr p15, 0, %0, c7, c5, 1" : : "r"(start_addr));
		start_addr += line_size;
	}
}
#endif /* ARCH_LOMBO_N7 */

/**
 * rt_hw_cpu_icache_ops - icache operations
 * @ops: operation type
 * @addr: memory vaddr which store the instructions
 * @size: memory size
 *
 */
void rt_hw_cpu_icache_ops(int ops, void *addr, int size)
{
#if defined(ARCH_LOMBO_N7)
	if (ops & RT_HW_CACHE_INVALIDATE) {
		/* n7_inv_icache_all(); */
		n7_inv_icache_range(addr, size);
	}
#elif defined(ARCH_LOMBO_N8V0)
	LOG_W("todo###");
#else
	RT_ASSERT(0);
#endif
}

 /**
 * rt_hw_cpu_dcache_ops - dcache operations
 * @ops: operation type
 * @addr: memory vaddr related to the operations
 * @size: memory size
 *
 */
void rt_hw_cpu_dcache_ops(int ops, void *addr, int size)
{
#if defined(ARCH_LOMBO_N7)
	if ((ops & RT_HW_CACHE_FLUSH) && (ops & RT_HW_CACHE_INVALIDATE))
		n7_flush_dcache_range(addr, addr + size);
	else if (ops & RT_HW_CACHE_FLUSH)
		n7_clean_dcache_range(addr, addr + size);
	else if (ops & RT_HW_CACHE_INVALIDATE)
		n7_inv_dcache_range(addr, addr + size);
	else
		RT_ASSERT(0);
#elif defined(ARCH_LOMBO_N8V0)
	LOG_W("todo###");
#else
	RT_ASSERT(0);
#endif
}
RTM_EXPORT(rt_hw_cpu_dcache_ops);

/**
 * rt_cpu_mmu_status - check if mmu enabled
 *
 * return 1 if enabled, 0 otherwise
 */
rt_base_t rt_cpu_mmu_status(void)
{
#if defined(ARCH_LOMBO_N7)
	return (rt_base_t)n7_is_mmu_en();
#elif defined(ARCH_LOMBO_N8V0)
	LOG_W("todo###");
	return 0;
#else
	RT_ASSERT(0);
#endif
}

