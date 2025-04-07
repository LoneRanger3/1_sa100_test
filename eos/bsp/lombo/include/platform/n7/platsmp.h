/*
 * platsmp.h - smp head file for n7
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

#ifndef __PLAT_SMP_H
#define __PLAT_SMP_H

#define __CPUINIT
#define __init
#define __cpuinit
#define cpu_logical_map(cpu)	cpu

/* cpu index */
#define CPU0		0
#define CPU1		1

/*
 * sev and wfe are ARMv6K extensions.  Uniprocessor ARMv6 may not have the K
 * extensions, so when running on UP, we have to patch these instructions away.
 */
#define ALT_SMP(smp, up)					\
	"9998:	" smp "\n"					\
	"	.pushsection \".alt.smp.init\", \"a\"\n"	\
	"	.long	9998b\n"				\
	"	" up "\n"					\
	"	.popsection\n"

#define SEV		ALT_SMP("sev", "nop")
#define WFE(cond)	ALT_SMP("wfe" cond, "nop")

#ifndef __ASSEMBLY__
static inline void dsb_sev(void)
{
	asm volatile ("dsb\n" SEV);
}

/* csp operations for rtc power management */
void rtc_set_hot_rst_flag(u32 value);
void rtc_set_hot_rst_exit(u32 addr);
void rtc_set_deep_slp_flag(u32 value);
void rtc_set_deep_slp_exit(u32 addr);

/* csp operations for cpu ctrl */
int cpu_is_in_wfi(u32 phys_cpu);
u32 get_cpu_boot_reg(u32 phys_cpu);
u32 get_cpu_core_cnt(void);
void cpu_on(u32 phys_cpu);
void cpu_off(u32 phys_cpu);

void lombo_secondary_startup(void);
void secondary_start_kernel(void);
void __cpuinit secondary_init(unsigned int cpu);
void rt_hw_vector_init(void);

#ifdef HOTPLUG_CPU
void lombo_cpu_die(unsigned int cpu);
int lombo_cpu_kill(unsigned int cpu);
int lombo_cpu_disable(unsigned int cpu);
#endif

#endif /* __ASSEMBLY__ */

#endif /* __CSP_N7_H */
