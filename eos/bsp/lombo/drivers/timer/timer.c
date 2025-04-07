/*
 * timer.c - timer module realization
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

#include "timer.h"

/* the CNTV_TVAL reg */
typedef union {
	u32 val;
	struct {
		u32 enable:1;
		u32 imask:1;
		u32 istatus:1;
		u32 rsvd:29;
	} bits;
} cntv_ctl_t;

void csp_vtmr_start(void)
{
	cntv_ctl_t cntv_ctrl;

	cntv_ctrl.val = 0;
	cntv_ctrl.bits.enable = 1;
	cntv_ctrl.bits.imask = 0; /* enable irq to gicd */
	asm volatile("mcr p15, 0, %0, c14, c3, 1" : : "r"(cntv_ctrl.val));
}

void csp_vtmr_stop(void)
{
	cntv_ctl_t cntv_ctrl;

	cntv_ctrl.val = 0;
	cntv_ctrl.bits.enable = 0;
	cntv_ctrl.bits.imask = 0;
	asm volatile("mcr p15, 0, %0, c14, c3, 1" : : "r"(cntv_ctrl.val));
}

void csp_vtmr_set_interval(void)
{
	u32 interval = (USEC_PER_SEC / RT_TICK_PER_SECOND) * 24;

	/* write CNTV_TVAL */
	asm volatile("mcr p15, 0, %0, c14, c3, 0" : : "r"(interval));
}

/**
 * udelay - microsecond delay function
 * @us: the count of microsecond to delay
 *
 * The cpu will be busy waiting the timer irq pending, during which other
 * programe cannot be executed, so be cautious
 */
void udelay(u32 us)
{
	rt_uint64_t cur;

	cur = get_cpu_cntvct();

	/* cnt_per_us: (HFEOSC_RATE / 1000000) = 24 */
	cur += (24 * us);

	while (get_cpu_cntvct() < cur)
		;
}
RTM_EXPORT(udelay);

/**
 * mdelay - millisecond delay function
 * @ms: the count of millisecond to delay
 *
 * The cpu will be busy waiting the timer irq pending, during which other
 * programe cannot be executed, so be cautious
 */
void mdelay(u32 ms)
{
	udelay(ms * 1000);
}
RTM_EXPORT(mdelay);

#ifdef RT_USING_PM
/**
 * rt_hw_tick_suspend - stop the system tick when deepsleep->suspend
 *
 */
void rt_hw_tick_suspend(void)
{
	csp_vtmr_stop();
}

/**
 * rt_hw_tick_resume - resume the system tick when deepsleep->resume
 *
 */
void rt_hw_tick_resume(void)
{
	csp_vtmr_set_interval();

	/* will be done in gic_resume */
	/* rt_hw_interrupt_umask(INT_VIRT_PPI); */

	csp_vtmr_start();
}
#endif

/**
 * rt_hw_tick_stop - stop the system tick timer
 *
 */
void rt_hw_tick_stop(void)
{
	csp_vtmr_stop();

	/* disable irq in gicd */
	rt_hw_interrupt_mask(INT_VIRT_PPI);
}

/**
 * rt_hw_timer_isr - the tick handle
 * @vector: index in isr_table[]
 * @param: parament for handle
 *
 */
void rt_hw_timer_isr(int vector, void *param)
{
	/* set next trigger */
	csp_vtmr_set_interval();

	rt_tick_increase();
}

/**
 * rt_hw_timer_init - timer initialization for system tick
 *
 * return 0 if success, -1 if failed
 */
void rt_hw_tick_init(void)
{
	char name[RT_NAME_MAX];

	csp_vtmr_set_interval();

	rt_sprintf(name, "tick%d", rt_hw_cpu_id());

	rt_hw_interrupt_install(INT_VIRT_PPI,
			rt_hw_timer_isr, NULL, name);

	rt_hw_interrupt_umask(INT_VIRT_PPI);

	csp_vtmr_start();
}

