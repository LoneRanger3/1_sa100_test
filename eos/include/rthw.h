/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2006-03-18     Bernard      the first version
 * 2006-04-25     Bernard      add rt_hw_context_switch_interrupt declaration
 * 2006-09-24     Bernard      add rt_hw_context_switch_to declaration
 * 2012-12-29     Bernard      add rt_hw_exception_install declaration
 * 2017-10-17     Hichard      add some micros
 * 2018-11-17     Jesven       add rt_hw_spinlock_t
 *                             add smp support
 */

#ifndef __RT_HW_H__
#define __RT_HW_H__

#include <rtthread.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Some macros define
 */
#ifndef HWREG32
#define HWREG32(x)          (*((volatile rt_uint32_t *)(x)))
#endif
#ifndef HWREG16
#define HWREG16(x)          (*((volatile rt_uint16_t *)(x)))
#endif
#ifndef HWREG8
#define HWREG8(x)           (*((volatile rt_uint8_t *)(x)))
#endif

#ifndef RT_CPU_CACHE_LINE_SZ
#define RT_CPU_CACHE_LINE_SZ	32
#endif

enum RT_HW_CACHE_OPS
{
    RT_HW_CACHE_FLUSH      = 0x01,
    RT_HW_CACHE_INVALIDATE = 0x02,
};

/*
 * CPU interfaces
 */
void rt_hw_cpu_icache_enable(void);
void rt_hw_cpu_icache_disable(void);
rt_base_t rt_hw_cpu_icache_status(void);
void rt_hw_cpu_icache_ops(int ops, void* addr, int size);

void rt_hw_cpu_dcache_enable(void);
void rt_hw_cpu_dcache_disable(void);
rt_base_t rt_hw_cpu_dcache_status(void);
void rt_hw_cpu_dcache_ops(int ops, void* addr, int size);

#ifdef ARCH_LOMBO
void rt_hw_cpu_dcache_flush_all(void);
void rt_hw_cpu_dcache_inval_all(void);
void rt_hw_cpu_icache_inval_all(void);
rt_base_t rt_cpu_mmu_status(void);
#endif

void rt_hw_cpu_reset(void);
void rt_hw_cpu_shutdown(void);

rt_uint8_t *rt_hw_stack_init(void       *entry,
                             void       *parameter,
                             rt_uint8_t *stack_addr,
                             void       *exit);

/*
 * Interrupt handler definition
 */
typedef void (*rt_isr_handler_t)(int vector, void *param);

struct rt_irq_desc
{
    rt_isr_handler_t handler;
    void            *param;

#ifdef RT_USING_INTERRUPT_INFO
    char             name[RT_NAME_MAX];
    rt_uint32_t      counter;
#endif
};

/*
 * Interrupt interfaces
 */
void rt_hw_interrupt_init(void);
void rt_hw_interrupt_mask(int vector);
void rt_hw_interrupt_umask(int vector);
rt_isr_handler_t rt_hw_interrupt_install(int              vector,
                                         rt_isr_handler_t handler,
                                         void            *param,
                                         const char      *name);

#ifdef RT_USING_SMP
rt_base_t rt_hw_local_irq_disable();
void rt_hw_local_irq_enable(rt_base_t level);
int rt_hw_local_irq_is_enabled(void);

#define rt_hw_interrupt_disable rt_cpus_lock
#define rt_hw_interrupt_enable rt_cpus_unlock

#else
rt_base_t rt_hw_interrupt_disable(void);
void rt_hw_interrupt_enable(rt_base_t level);
#endif /*RT_USING_SMP*/

/*
 * Context interfaces
 */
#ifdef RT_USING_SMP
void rt_hw_context_switch(rt_ubase_t from, rt_ubase_t to, struct rt_thread *to_thread);
void rt_hw_context_switch_to(rt_ubase_t to, struct rt_thread *to_thread);
void rt_hw_context_switch_interrupt(void *context, rt_ubase_t from, rt_ubase_t to, struct rt_thread *to_thread);
#else
void rt_hw_context_switch(rt_ubase_t from, rt_ubase_t to);
void rt_hw_context_switch_to(rt_ubase_t to);
void rt_hw_context_switch_interrupt(rt_ubase_t from, rt_ubase_t to);
#endif /*RT_USING_SMP*/

void rt_hw_console_output(const char *str);

void rt_hw_backtrace(rt_uint32_t *fp, rt_ubase_t thread_entry);
void rt_hw_show_memory(rt_uint32_t addr, rt_size_t size);

/*
 * Exception interfaces
 */
void rt_hw_exception_install(rt_err_t (*exception_handle)(void *context));

/*
 * delay interfaces
 */
void rt_hw_us_delay(rt_uint32_t us);

#ifdef RT_USING_SMP
typedef union {
    unsigned long slock;
    struct __arch_tickets {
        unsigned short owner;
        unsigned short next;
    } tickets;
} rt_hw_spinlock_t;

void rt_hw_spin_lock(rt_hw_spinlock_t *lock);
void rt_hw_spin_unlock(rt_hw_spinlock_t *lock);
int __raw_spin_trylock(rt_hw_spinlock_t *lock);
int rt_hw_spin_trylock(rt_hw_spinlock_t *lock);

int rt_hw_cpu_id(void);

extern rt_hw_spinlock_t _cpus_lock;
extern rt_hw_spinlock_t _rt_critical_lock;

#define __RT_HW_SPIN_LOCK_INITIALIZER(lockname) {0}

#define __RT_HW_SPIN_LOCK_UNLOCKED(lockname) \
 (struct rt_hw_spinlock ) __RT_HW_SPIN_LOCK_INITIALIZER(lockname)

#define RT_DEFINE_SPINLOCK(x)  struct rt_hw_spinlock x = __RT_HW_SPIN_LOCK_UNLOCKED(x)

/**
 *  ipi function
 */
void rt_hw_ipi_send(int ipi_vector, unsigned int cpu_mask);

/**
 * boot secondary cpu
 */
void rt_hw_secondary_cpu_up(void);

/**
 * secondary cpu idle function
 */
void rt_hw_secondary_cpu_idle_exec(void);

#ifdef ARCH_LOMBO
#define rt_interrupt_nest rt_cpu_self()->irq_nest

void rt_hw_cpu_idle_exec(void);

#ifdef HOTPLUG_CPU
void cpu_die(void);
int cpu_up(int cpu);
void cpu_down(int cpu);
int enable_nonboot_cpus(void);
int disable_nonboot_cpus(void);
#endif

#ifdef ARCH_LOMBO_N7
/*
 * core sync functions.
 * during the sync procedure, the irq and scheduler was disabled for all cores
 *
 * so the procedure was like this:
 *      core-a                        core-b
 *  ------------------------   ------------------------
 *  core_sync_1_start
 *     ..(wait 2_start over)         ...
 *     ..(wait 2_start over)     core_sync_2_start
 *     ..(do)                        ..(do)
 *     ...                           ...
 *     ...                       core_sync_2_end
 *     ...                           ..(wait 1_end start)
 *  core_sync_1_end                  ..(wait 1_end start)
 *     ..(wait 2_end over)           ..(do)
 *     ..(wait 2_end over)           ..(do)
 *     ..(do)                        ...
 *
 * in n7 platform, when one core write dma regs, the other core should
 * be hung (do nothing).
 */
enum {
	SYNC_PHASE_1 = 1,	/* initial */
	SYNC_PHASE_2 = 2,
	SYNC_PHASE_3 = 3,
	SYNC_PHASE_4 = 4,
};

u32 core_sync_1_start(int sgi_no);
void core_sync_1_end(u32 flags);
u32 core_sync_2_start(void);
void core_sync_2_end(u32 flags);
#endif
void get_cpu_timestamp(u32 val[]);
void set_cpu_timestamp(u32 val[]);
void uart_init(void);
#ifdef CPU_STATS

int get_cpu_idle_time(int cpu);
int get_cpu_total_time(int cpu);
float get_cpu_loading(int cpu);

#ifdef SCHED_LOCK_DEBUG
int get_cpu_sched_lock_time(int cpu);
float get_cpu_sched_lock_loading(int cpu);
#endif
#ifdef SPIN_LOCK_DEBUG
int get_cpu_spin_lock_time(int cpu);
float get_cpu_spin_lock_loading(int cpu);
#endif

#ifdef THREAD_STATS

int get_thread_busy_time(struct rt_thread *pth);
float get_thread_loading(struct rt_thread *pth);

#ifdef SCHED_LOCK_DEBUG
int get_thread_sched_lock_time(struct rt_thread *pth);
float get_thread_sched_lock_loading(struct rt_thread *pth);
#endif
#ifdef SPIN_LOCK_DEBUG
int get_thread_spin_lock_time(struct rt_thread *pth);
float get_thread_spin_lock_loading(struct rt_thread *pth);
#endif

#endif /* THREAD_STATS */

#endif /* CPU_STATS */

#define for_each_cpu(cpu)						\
	for ((cpu) = 0; (cpu) < RT_CPUS_NR; (cpu)++)

#define for_each_online_cpu(cpu)					\
	for ((cpu) = 0; (cpu) < RT_CPUS_NR; (cpu)++)			\
		if (CPU_STATUS_ONLINE != rt_cpu_index(cpu)->status)	\
			;						\
		else

/* check if cpu online */
#define is_cpu_online(cpu)						\
	(CPU_STATUS_ONLINE == rt_cpu_index(cpu)->status)

/*
 * check if scheduler started, the cpu can be online but scheduler not started
 *   only after scheduler started can the cpu ack interrupts
 */
#define is_cpu_sched_started(cpu)					\
	(RT_NULL != rt_cpu_index(cpu)->current_thread)

#endif /* ARCH_LOMBO */

#else
#define rt_hw_cpu_id()		0
#endif

#ifdef __cplusplus
}
#endif

#endif
