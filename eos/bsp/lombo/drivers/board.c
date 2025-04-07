/*
 * board.c - board initialization code
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

#include <debug.h>
#include <rtthread.h>
#include <mmu.h>
#include "csp.h"
#include "memory.h"
#include "board.h"
#include "timer.h"
#include "boot_param.h"

unsigned long dram_size;

/*
 *  1 - fast boot. booster load kernel directly(without leaver), only can boot from nor
 *  0 - normal boot
 */
u32 boot_flag;

void temp_set_boot_para(void); /* temply */

/**
 * get dram size, should be called before rt_hw_mmu_init
 */
void rt_hw_get_dram_size(void)
{
	dram_size = boot_get_dram_size();

	/*
	 * Note: only support <1G dram, otherwise RT_SYS_HEAP_END will overflow
	 *  The max virtual memory addr is 0xFFFFF000
	 */
	if (dram_size + (u32)LOMBO_DRAM_VBASE < dram_size) {
		LOG_I("dram size(0x%08x) overflow!", dram_size);
		dram_size = UINT_MAX - LOMBO_DRAM_VBASE;
		dram_size &= ~(PAGE_SIZE - 1); /* max waste: 4K */
	}
}

/**
 * get current boot flag
 */
void rt_hw_get_boot_flag(void)
{
	boot_flag = boot_get_boot_flag();

	if (boot_flag)
		boot_print_booster_version();

	LOG_I("boot flag(%d): %s", (int)boot_flag, (boot_flag ? "fast" : "normal"));

#if 0 /* #ifdef __EOS__RELEASE__MP__ */
	if (boot_flag)
		temp_set_boot_para();
#endif
}

/**
 * rt_system_early_init: System init before mmu eanbled
 */
void rt_system_early_init(void)
{
	rt_hw_get_dram_size();

	rt_hw_get_boot_flag();

#ifdef RT_USING_HEAP
	/* init heap memory system */
	rt_system_heap_init(RT_SYS_HEAP_BEGIN, RT_SYS_HEAP_END);
#endif
}

/**
 * This function will init lombo board
 */
void rt_hw_board_init(void)
{
#if defined(ARCH_ARM)
	/* initialize mmu, move to _reset entry */
	/* rt_hw_mmu_init(); */
#endif

#if defined(RT_USING_HOOK) && defined(RT_DEBUG)
	rt_object_hook_init();
#endif

	/* initialize hardware interrupt */
	rt_hw_interrupt_init();
#ifdef ARCH_LOMBO_N7
	config_init();
#endif
	/* initialize timer0 */
	rt_hw_tick_init();

	/* move to rt_system_early_init, to let allcator work before mmu enabled */
	/* rt_system_heap_init(RT_SYS_HEAP_BEGIN, RT_SYS_HEAP_END); */

#if defined(ARCH_LOMBO_N7V0) || defined(ARCH_LOMBO_N7V1)
	/* initialize pll etc */
	/* rt_hw_clock_init(); */
	clk_init();
#endif

#ifdef ARCH_LOMBO_N7
	rt_hw_gpio_init();
#endif

#ifdef ARCH_LOMBO_N7
	/* initialize uart */
	rt_hw_uart_init();
#endif

#ifdef RT_USING_CONSOLE
	rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif

	/* dump boot para */
	/* boot_para_dump_all(); */
	lombo_func3();

#ifdef RT_USING_COMPONENTS_INIT
	rt_components_board_init();
#endif

#ifdef RT_USING_PM
	pm_init();
#endif

#if defined(RT_DEBUG) && defined(DUMP_EXCP_STACK)
	rt_assert_set_hook(lombo_assert_hook);
#endif

#ifdef RT_USING_SMP
	rt_hw_ipi_handler_install(RT_SCHEDULE_IPI, rt_scheduler_ipi_handler);
	rt_hw_ipi_handler_install(RT_IPI_INV_TLB, rt_scheduler_ipi_handler);
	rt_hw_ipi_handler_install(RT_IPI_INV_ICACHE, rt_scheduler_ipi_handler);
	rt_hw_ipi_handler_install(RT_IPI_WAIT, rt_scheduler_ipi_handler);
#ifdef RT_USING_BIND_TIMER
	rt_hw_ipi_handler_install(RT_IPI_ENABLE_LOCAL_INT, rt_scheduler_ipi_handler);
	rt_hw_ipi_handler_install(RT_IPI_DISABLE_LOCAL_INT, rt_scheduler_ipi_handler);
#endif
#ifdef RT_USING_DEADLOCK_MONITOR
	rt_hw_ipi_handler_install(RT_IPI_DUMP_CURTHREAD_STACK, rt_scheduler_ipi_handler);
#endif
#endif
}

