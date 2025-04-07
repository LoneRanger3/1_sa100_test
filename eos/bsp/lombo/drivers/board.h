/*
 * board.h - board operations head file
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

#ifndef __BOARD_H___
#define __BOARD_H___

#include <debug.h>
#include <rthw.h>
#include <csp.h>
#include <drivers/pm.h>

void rt_hw_mmu_init(void);
#ifdef ENABLE_MMU
void rt_hw_mmu_setup_l1(rt_uint32_t va_start, rt_uint32_t pa_start,
			rt_uint32_t size, int prog);
void rt_hw_mmu_setup_l2(rt_uint32_t va_start, rt_uint32_t pa_start,
			rt_uint32_t size, int prog);
void rt_hw_dump_page_table(rt_uint32_t va_start, rt_uint32_t size);
#else
static inline void rt_hw_mmu_setup_l1(rt_uint32_t va_start,
		rt_uint32_t pa_start, rt_uint32_t size, int prog)
{
}
static inline void rt_hw_mmu_setup_l2(rt_uint32_t va_start,
		rt_uint32_t pa_start, rt_uint32_t size, int prog)
{
}
static inline void rt_hw_dump_page_table(rt_uint32_t va_start, rt_uint32_t size)
{
}
#endif

#ifdef RT_USING_SMP
void secondary_cpu_init(void);
#endif
void rt_hw_board_init(void);

/*
 * timer operations
 */
void rt_hw_tick_init(void);
void rt_hw_tick_stop(void);
#ifdef RT_USING_PM
void rt_hw_tick_suspend(void);
void rt_hw_tick_resume(void);
#endif

/*
 * uart operations
 */
int rt_hw_uart_init(void);

/*
 * gpio operations
 */
int rt_hw_gpio_init(void);

/*
 * clk operations
 */
int clk_init(void);

/*
 * config operations
 */
void config_init(void);

/*
 * input operation
 */
int rt_hw_input_init(void);

/*
 * pm operations
 */
#ifdef RT_USING_PM
void pm_init(void);
#endif /* RT_USING_PM */

#ifdef RT_USING_SMP
void rt_hw_ipi_handler_install(int ipi_vector, rt_isr_handler_t ipi_isr_handler);
#endif

#define INIT_DUMP_MEM	rt_uint32_t mem_total, mem_used, mem_max_used
#define DUMP_MEMORY()	do { rt_memory_info(&mem_total, &mem_used, &mem_max_used);	\
				LOG_E("total memory %d, use %d, max_used %d",		\
					mem_total, mem_used, mem_max_used); } while (0)

#ifdef LOMBO_TEST
/*
 * test driver operations
 */
long test_memory(int argc, char **argv);
long test_cache(int argc, char **argv);
long test_timer(int argc, char **argv);
long test_uart(int argc, char **argv);
long test_gpio(int argc, char **argv);
long test_dma(int argc, char **argv);
long test_clk(int argc, char **argv);
long test_sdc(int argc, char **argv);
long test_spi_nor(int argc, char **argv);
long test_config(int argc, char **argv);
long test_pwm(int argc, char **argv);
long test_wdog(int argc, char **argv);
long bind_timer_test(int argc, char **argv);
long deadlock_monitor_test(int argc, char **argv);
long test_pm(int argc, char **argv);
long test_memctrl(int argc, char **argv);
long test_keyboard(int argc, char **argv);
long test_gsensor(int argc, char **argv);
long test_touch_screen(int argc, char **argv);
long test_input(int argc, char **argv);
long test_gps(int argc, char **argv);
long test_osal_mem(int argc, char **argv);
long test_rtc(int argc, char **argv);
long test_sys(int argc, char **argv);
long test_pthread(int argc, char **argv);
long test_pthread_mutex(int argc, char **argv);
long test_spinlock(int argc, char **argv);
long test_audio(int argc, char **argv);
long test_i2c(int argc, char **argv);
long test_viss(int argc, char **argv);
long test_vfp(int argc, char **argv);
#if defined(LOMBO_TEST_PTHREAD_MUTEX)	\
	|| defined(LOMBO_TEST_VFP)	\
	|| defined(LOMBO_TEST_NEON)
float float_test_func(float a, float b, float c); /* shared by neon and vfp */
unsigned int float_test_func2();
#else
static inline float float_test_func(float a, float b, float c)
{
	return 0;
}
static inline unsigned int float_test_func2()
{
	return 0;
}
#endif
long test_neon(int argc, char **argv);
long test_isp(int argc, char **argv);
long test_disp(int argc, char **argv);
long test_cpu(int argc, char **argv);
long test_cpufreq(int argc, char **argv);
#endif /* LOMBO_TEST */

#endif
