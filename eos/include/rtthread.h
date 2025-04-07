/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2006-03-18     Bernard      the first version
 * 2006-04-26     Bernard      add semaphore APIs
 * 2006-08-10     Bernard      add version information
 * 2007-01-28     Bernard      rename RT_OBJECT_Class_Static to RT_Object_Class_Static
 * 2007-03-03     Bernard      clean up the definitions to rtdef.h
 * 2010-04-11     yi.qiu       add module feature
 * 2013-06-24     Bernard      add rt_kprintf re-define when not use RT_USING_CONSOLE.
 * 2016-08-09     ArdaFu       add new thread and interrupt hook.
 * 2018-11-22     Jesven       add all cpu's lock and ipi handler
 */

#ifndef __RT_THREAD_H__
#define __RT_THREAD_H__

#include <rtconfig.h>
#include <rtdebug.h>
#include <rtdef.h>
#include <rtservice.h>
#include <rtm.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup KernelObject
 */

/**@{*/

/*
 * kernel object interface
 */
void rt_system_object_init(void);
struct rt_object_information *
rt_object_get_information(enum rt_object_class_type type);
void rt_object_init(struct rt_object         *object,
                    enum rt_object_class_type type,
                    const char               *name);
void rt_object_detach(rt_object_t object);
rt_object_t rt_object_allocate(enum rt_object_class_type type,
                               const char               *name);
void rt_object_delete(rt_object_t object);
rt_bool_t rt_object_is_systemobject(rt_object_t object);
rt_uint8_t rt_object_get_type(rt_object_t object);
rt_object_t rt_object_find(const char *name, rt_uint8_t type);

#ifdef RT_USING_HOOK
void rt_object_attach_sethook(void (*hook)(struct rt_object *object));
void rt_object_detach_sethook(void (*hook)(struct rt_object *object));
void rt_object_trytake_sethook(void (*hook)(struct rt_object *object));
void rt_object_take_sethook(void (*hook)(struct rt_object *object));
void rt_object_put_sethook(void (*hook)(struct rt_object *object));
#endif

/**@}*/

/**
 * @addtogroup Clock
 */

/**@{*/

/*
 * clock & timer interface
 */
void rt_system_tick_init(void);
rt_tick_t rt_tick_get(void);
void rt_tick_set(rt_tick_t tick);
void rt_tick_increase(void);
rt_tick_t  rt_tick_from_millisecond(rt_int32_t ms);

void rt_system_timer_init(void);
void rt_system_timer_thread_init(void);

void rt_timer_init(rt_timer_t  timer,
                   const char *name,
                   void (*timeout)(void *parameter),
                   void       *parameter,
                   rt_tick_t   time,
                   rt_uint8_t  flag);
#ifdef ARCH_LOMBO
#ifdef RT_USING_BIND_TIMER
void rt_timer_init_bind(rt_timer_t  timer,
		const char *name,
		void (*timeout)(void *parameter),
		void       *parameter,
		rt_tick_t   time,
		rt_uint8_t  flag,
		rt_uint8_t      cpu);
rt_timer_t rt_timer_create_bind(const char *name,
			void (*timeout)(void *parameter),
			void       *parameter,
			rt_tick_t   time,
			rt_uint8_t  flag,
			rt_uint8_t      cpu);
#endif
#endif
rt_err_t rt_timer_detach(rt_timer_t timer);
rt_timer_t rt_timer_create(const char *name,
                           void (*timeout)(void *parameter),
                           void       *parameter,
                           rt_tick_t   time,
                           rt_uint8_t  flag);
rt_err_t rt_timer_delete(rt_timer_t timer);
rt_err_t rt_timer_start(rt_timer_t timer);
rt_err_t rt_timer_stop(rt_timer_t timer);
rt_err_t rt_timer_control(rt_timer_t timer, int cmd, void *arg);

rt_tick_t rt_timer_next_timeout_tick(void);
void rt_timer_check(void);

#ifdef RT_USING_HOOK
void rt_timer_enter_sethook(void (*hook)(struct rt_timer *timer));
void rt_timer_exit_sethook(void (*hook)(struct rt_timer *timer));
#endif

/**@}*/

/**
 * @addtogroup Thread
 */

/**@{*/

/*
 * thread interface
 */
rt_err_t rt_thread_init(struct rt_thread *thread,
                        const char       *name,
                        void (*entry)(void *parameter),
                        void             *parameter,
                        void             *stack_start,
                        rt_uint32_t       stack_size,
                        rt_uint8_t        priority,
                        rt_uint32_t       tick);
rt_err_t rt_thread_detach(rt_thread_t thread);
rt_thread_t rt_thread_create(const char *name,
                             void (*entry)(void *parameter),
                             void       *parameter,
                             rt_uint32_t stack_size,
                             rt_uint8_t  priority,
                             rt_uint32_t tick);
rt_thread_t rt_thread_self(void);
rt_thread_t rt_thread_find(char *name);
rt_err_t rt_thread_startup(rt_thread_t thread);
rt_err_t rt_thread_delete(rt_thread_t thread);

rt_err_t rt_thread_yield(void);
rt_err_t rt_thread_delay(rt_tick_t tick);
rt_err_t rt_thread_mdelay(rt_int32_t ms);
rt_err_t rt_thread_control(rt_thread_t thread, int cmd, void *arg);
rt_err_t rt_thread_suspend(rt_thread_t thread);
rt_err_t rt_thread_resume(rt_thread_t thread);
void rt_thread_timeout(void *parameter);

#ifdef RT_USING_SIGNALS
void rt_thread_alloc_sig(rt_thread_t tid);
void rt_thread_free_sig(rt_thread_t tid);
int  rt_thread_kill(rt_thread_t tid, int sig);
#endif

#ifdef RT_USING_HOOK
void rt_thread_suspend_sethook(void (*hook)(rt_thread_t thread));
void rt_thread_resume_sethook (void (*hook)(rt_thread_t thread));
void rt_thread_inited_sethook (void (*hook)(rt_thread_t thread));
#endif

/*
 * idle thread interface
 */
void rt_thread_idle_init(void);
#if defined(RT_USING_HOOK) || defined(RT_USING_IDLE_HOOK)
rt_err_t rt_thread_idle_sethook(void (*hook)(void));
rt_err_t rt_thread_idle_delhook(void (*hook)(void));
#endif
void rt_thread_idle_excute(void);
rt_thread_t rt_thread_idle_gethandler(void);

/*
 * schedule service
 */
void rt_system_scheduler_init(void);
void rt_system_scheduler_start(void);

void rt_schedule(void);
void rt_schedule_insert_thread(struct rt_thread *thread);
void rt_schedule_remove_thread(struct rt_thread *thread);

void rt_enter_critical(void);
void rt_exit_critical(void);
rt_uint16_t rt_critical_level(void);

#ifdef RT_USING_HOOK
void rt_scheduler_sethook(void (*hook)(rt_thread_t from, rt_thread_t to));
#endif

#ifdef RT_USING_SMP
void rt_scheduler_ipi_handler(int vector, void *param);
#endif

/**@}*/

/**
 * @addtogroup Signals
 * @{
 */
#ifdef RT_USING_SIGNALS
void rt_signal_mask(int signo);
void rt_signal_unmask(int signo);
rt_sighandler_t rt_signal_install(int signo, rt_sighandler_t handler);
int rt_signal_wait(const rt_sigset_t *set, rt_siginfo_t *si, rt_int32_t timeout);

int rt_system_signal_init(void);
#endif
/*@}*/

/**
 * @addtogroup MM
 */

/**@{*/

/*
 * memory management interface
 */
#ifdef RT_USING_MEMPOOL
/*
 * memory pool interface
 */
rt_err_t rt_mp_init(struct rt_mempool *mp,
                    const char        *name,
                    void              *start,
                    rt_size_t          size,
                    rt_size_t          block_size);
rt_err_t rt_mp_detach(struct rt_mempool *mp);
rt_mp_t rt_mp_create(const char *name,
                     rt_size_t   block_count,
                     rt_size_t   block_size);
rt_err_t rt_mp_delete(rt_mp_t mp);

void *rt_mp_alloc(rt_mp_t mp, rt_int32_t time);
void rt_mp_free(void *block);

#ifdef RT_USING_HOOK
void rt_mp_alloc_sethook(void (*hook)(struct rt_mempool *mp, void *block));
void rt_mp_free_sethook(void (*hook)(struct rt_mempool *mp, void *block));
#endif

#endif

#ifdef RT_USING_HEAP
/*
 * heap memory interface
 */
void rt_system_heap_init(void *begin_addr, void *end_addr);

void *rt_malloc(rt_size_t nbytes);
void rt_free(void *ptr);
void *rt_realloc(void *ptr, rt_size_t nbytes);
void *rt_calloc(rt_size_t count, rt_size_t size);
void *rt_malloc_align(rt_size_t size, rt_size_t align);
void rt_free_align(void *ptr);

void rt_memory_info(rt_uint32_t *total,
                    rt_uint32_t *used,
                    rt_uint32_t *max_used);

#ifdef RT_USING_SLAB
void *rt_page_alloc(rt_size_t npages);
void rt_page_free(void *addr, rt_size_t npages);
#endif

#ifdef RT_USING_HOOK
void rt_malloc_sethook(void (*hook)(void *ptr, rt_size_t size));
void rt_free_sethook(void (*hook)(void *ptr));
#endif

#endif

#ifdef RT_USING_MEMHEAP
/**
 * memory heap object interface
 */
rt_err_t rt_memheap_init(struct rt_memheap *memheap,
                         const char        *name,
                         void              *start_addr,
                         rt_size_t         size);
rt_err_t rt_memheap_detach(struct rt_memheap *heap);
void *rt_memheap_alloc(struct rt_memheap *heap, rt_size_t size);
void *rt_memheap_realloc(struct rt_memheap *heap, void *ptr, rt_size_t newsize);
void rt_memheap_free(void *ptr);
#endif

/**@}*/

/**
 * @addtogroup IPC
 */

/**@{*/

#ifdef RT_USING_SEMAPHORE
/*
 * semaphore interface
 */
rt_err_t rt_sem_init(rt_sem_t    sem,
                     const char *name,
                     rt_uint32_t value,
                     rt_uint8_t  flag);
rt_err_t rt_sem_detach(rt_sem_t sem);
rt_sem_t rt_sem_create(const char *name, rt_uint32_t value, rt_uint8_t flag);
rt_err_t rt_sem_delete(rt_sem_t sem);

rt_err_t rt_sem_take(rt_sem_t sem, rt_int32_t time);
rt_err_t rt_sem_trytake(rt_sem_t sem);
rt_err_t rt_sem_release(rt_sem_t sem);
rt_err_t rt_sem_control(rt_sem_t sem, int cmd, void *arg);
#endif

#ifdef RT_USING_MUTEX
/*
 * mutex interface
 */
rt_err_t rt_mutex_init(rt_mutex_t mutex, const char *name, rt_uint8_t flag);
rt_err_t rt_mutex_detach(rt_mutex_t mutex);
rt_mutex_t rt_mutex_create(const char *name, rt_uint8_t flag);
rt_err_t rt_mutex_delete(rt_mutex_t mutex);

rt_err_t rt_mutex_take(rt_mutex_t mutex, rt_int32_t time);
rt_err_t rt_mutex_release(rt_mutex_t mutex);
rt_err_t rt_mutex_control(rt_mutex_t mutex, int cmd, void *arg);
#endif

#ifdef RT_USING_EVENT
/*
 * event interface
 */
rt_err_t rt_event_init(rt_event_t event, const char *name, rt_uint8_t flag);
rt_err_t rt_event_detach(rt_event_t event);
rt_event_t rt_event_create(const char *name, rt_uint8_t flag);
rt_err_t rt_event_delete(rt_event_t event);

rt_err_t rt_event_send(rt_event_t event, rt_uint32_t set);
rt_err_t rt_event_recv(rt_event_t   event,
                       rt_uint32_t  set,
                       rt_uint8_t   opt,
                       rt_int32_t   timeout,
                       rt_uint32_t *recved);
rt_err_t rt_event_control(rt_event_t event, int cmd, void *arg);
#endif

#ifdef RT_USING_MAILBOX
/*
 * mailbox interface
 */
rt_err_t rt_mb_init(rt_mailbox_t mb,
                    const char  *name,
                    void        *msgpool,
                    rt_size_t    size,
                    rt_uint8_t   flag);
rt_err_t rt_mb_detach(rt_mailbox_t mb);
rt_mailbox_t rt_mb_create(const char *name, rt_size_t size, rt_uint8_t flag);
rt_err_t rt_mb_delete(rt_mailbox_t mb);

rt_err_t rt_mb_send(rt_mailbox_t mb, rt_ubase_t value);
rt_err_t rt_mb_send_wait(rt_mailbox_t mb,
                         rt_ubase_t  value,
                         rt_int32_t   timeout);
rt_err_t rt_mb_recv(rt_mailbox_t mb, rt_ubase_t *value, rt_int32_t timeout);
rt_err_t rt_mb_control(rt_mailbox_t mb, int cmd, void *arg);
#endif

#ifdef RT_USING_MESSAGEQUEUE
/*
 * message queue interface
 */
rt_err_t rt_mq_init(rt_mq_t     mq,
                    const char *name,
                    void       *msgpool,
                    rt_size_t   msg_size,
                    rt_size_t   pool_size,
                    rt_uint8_t  flag);
rt_err_t rt_mq_detach(rt_mq_t mq);
rt_mq_t rt_mq_create(const char *name,
                     rt_size_t   msg_size,
                     rt_size_t   max_msgs,
                     rt_uint8_t  flag);
rt_err_t rt_mq_delete(rt_mq_t mq);

rt_err_t rt_mq_send(rt_mq_t mq, void *buffer, rt_size_t size);
rt_err_t rt_mq_urgent(rt_mq_t mq, void *buffer, rt_size_t size);
rt_err_t rt_mq_recv(rt_mq_t    mq,
                    void      *buffer,
                    rt_size_t  size,
                    rt_int32_t timeout);
rt_err_t rt_mq_control(rt_mq_t mq, int cmd, void *arg);
#endif

/**@}*/

#ifdef RT_USING_DEVICE
/**
 * @addtogroup Device
 */

/**@{*/

/*
 * device (I/O) system interface
 */
rt_device_t rt_device_find(const char *name);

rt_err_t rt_device_register(rt_device_t dev,
                            const char *name,
                            rt_uint16_t flags);
rt_err_t rt_device_unregister(rt_device_t dev);

rt_device_t rt_device_create(int type, int attach_size);
void rt_device_destroy(rt_device_t device);

rt_err_t rt_device_init_all(void);

rt_err_t
rt_device_set_rx_indicate(rt_device_t dev,
                          rt_err_t (*rx_ind)(rt_device_t dev, rt_size_t size));
rt_err_t
rt_device_set_tx_complete(rt_device_t dev,
                          rt_err_t (*tx_done)(rt_device_t dev, void *buffer));

rt_err_t  rt_device_init (rt_device_t dev);
rt_err_t  rt_device_open (rt_device_t dev, rt_uint16_t oflag);
rt_err_t  rt_device_close(rt_device_t dev);
rt_size_t rt_device_read (rt_device_t dev,
                          rt_off_t    pos,
                          void       *buffer,
                          rt_size_t   size);
rt_size_t rt_device_write(rt_device_t dev,
                          rt_off_t    pos,
                          const void *buffer,
                          rt_size_t   size);
rt_err_t  rt_device_control(rt_device_t dev, int cmd, void *arg);

/**@}*/
#endif

/*
 * interrupt service
 */

/*
 * rt_interrupt_enter and rt_interrupt_leave only can be called by BSP
 */
void rt_interrupt_enter(void);
void rt_interrupt_leave(void);

#ifdef RT_USING_SMP

/*
 * smp cpus lock service
 */

rt_base_t rt_cpus_lock(void);
void rt_cpus_unlock(rt_base_t level);

struct rt_cpu *rt_cpu_self(void);
struct rt_cpu *rt_cpu_index(int index);

#endif

/*
 * the number of nested interrupts.
 */
rt_uint8_t rt_interrupt_get_nest(void);

#ifdef RT_USING_HOOK
void rt_interrupt_enter_sethook(void (*hook)(void));
void rt_interrupt_leave_sethook(void (*hook)(void));
#endif

#ifdef RT_USING_COMPONENTS_INIT
void rt_components_init(void);
void rt_components_board_init(void);
#endif

/**
 * @addtogroup KernelService
 */

/**@{*/

/*
 * general kernel service
 */
#ifndef RT_USING_CONSOLE
#define rt_kprintf(...)
#define rt_kputs(str)
#else
void rt_kprintf(const char *fmt, ...);
void rt_kputs(const char *str);
#endif
rt_int32_t rt_vsprintf(char *dest, const char *format, va_list arg_ptr);
rt_int32_t rt_vsnprintf(char *buf, rt_size_t size, const char *fmt, va_list args);
rt_int32_t rt_sprintf(char *buf, const char *format, ...);
rt_int32_t rt_snprintf(char *buf, rt_size_t size, const char *format, ...);

#if defined(RT_USING_DEVICE) && defined(RT_USING_CONSOLE)
rt_device_t rt_console_set_device(const char *name);
rt_device_t rt_console_get_device(void);
#endif

rt_err_t rt_get_errno(void);
void rt_set_errno(rt_err_t no);
int *_rt_errno(void);
#if !defined(RT_USING_NEWLIB) && !defined(_WIN32)
#ifndef errno
#define errno    *_rt_errno()
#endif
#endif

int __rt_ffs(int value);

void *rt_memset(void *src, int c, rt_ubase_t n);
void *rt_memcpy(void *dest, const void *src, rt_ubase_t n);

rt_int32_t rt_strncmp(const char *cs, const char *ct, rt_ubase_t count);
rt_int32_t rt_strcmp(const char *cs, const char *ct);
rt_size_t rt_strlen(const char *src);
rt_size_t rt_strnlen(const char *s, rt_ubase_t maxlen);
char *rt_strdup(const char *s);
#if defined(__CC_ARM) || defined(__CLANG_ARM)
/* leak strdup interface */
char* strdup(const char* str);
#endif

char *rt_strstr(const char *str1, const char *str2);
rt_int32_t rt_sscanf(const char *buf, const char *fmt, ...);
char *rt_strncpy(char *dest, const char *src, rt_ubase_t n);
void *rt_memmove(void *dest, const void *src, rt_ubase_t n);
rt_int32_t rt_memcmp(const void *cs, const void *ct, rt_ubase_t count);
rt_uint32_t rt_strcasecmp(const char *a, const char *b);

void rt_show_version(void);

#ifdef RT_DEBUG
extern void (*rt_assert_hook)(const char *ex, const char *func, rt_size_t line);
void rt_assert_set_hook(void (*hook)(const char *ex, const char *func, rt_size_t line));

void rt_assert_handler(const char *ex, const char *func, rt_size_t line);
#endif /* RT_DEBUG */

#ifdef RT_USING_FINSH
#include <finsh_api.h>
#endif

/**@}*/

#ifdef ARCH_LOMBO
#include <memory.h>
#include <rthw.h>

void *rt_malloc_unca(rt_size_t x);
void rt_free_unca(void *x);

/* rt_zalloc and rt_free should be used together */
void *rt_zalloc(rt_size_t x);

/* rt_zalloc_unca and rt_free_unca should be used together */
void *rt_zalloc_unca(rt_size_t x);

/* rt_zalloc_unca_align and rt_free_unca_align should be used together */
void *rt_zalloc_unca_align(rt_size_t x, rt_size_t y);
void rt_free_unca_align(void *x);

/* dump all context, include threads' stack, timers, devices, ipc objects, etc. */
void dump_all(int in_excpt);
void dumpmem(u32 start_va, u32 size);
void clrmem(u32 start_va, u32 size);

#if defined(RT_DEBUG) && defined(DUMP_EXCP_STACK)
void dump_stack(void); /* dump current thread stack */
void deadlock_dump_stack(void); /* dump current thread stack when deadlock occured */
void dump_stack_thread(struct rt_thread *pth); /* dump a certain thread stack */
long dump_stack_all(void); /* dump all threads stack */
void lombo_assert_hook(const char *ex, const char *func, rt_size_t line);
#else
static inline void dump_stack(void)
{
}
static inline void dump_stack_thread(struct rt_thread *pth)
{
}
static inline long dump_stack_all(void)
{
	return 0;
}
#endif /* RT_DEBUG && DUMP_EXCP_STACK */

#if defined(ARCH_LOMBO_N7)
/*
 * get_cpu_cntvct - get the cpu virt counter value(only for E81 now)
 *   1. there is a gloable 64bit counter in N7Vx system(not inside percpu), named
 *        CPU_Time_Stamp, and its value is from osc24m, and it is 0 when system power off
 *   2. each cpu has a phys_counter and a virt_counter, and the phys_counter_value is from
 *        CPU_Time_Stamp, their values are almost equal
 *   3. virt_counter_value = phys_counter_value - CNTVOFF, and the CNTVOFF value
 *        is set to 0 in booster
 */
static inline u64 get_cpu_cntvct(void)
{
	u64 cval;

	asm volatile("isb\n\t");

	/* read virt_counter_value */
	asm volatile("mrrc p15, 1, %Q0, %R0, c14" : "=r" (cval));
	return cval;
}

u64 rt_time_get_nsec(void);
u64 rt_time_get_usec(void);
int rt_time_get_msec(void);
#else
static inline u64 rt_time_get_nsec(void)
{
	return 0;
}
static inline u64 rt_time_get_usec(void)
{
	return 0;
}
static inline int rt_time_get_msec(void)
{
	return 0;
}
#endif /* ARCH_LOMBO_N7 */

#ifdef CPU_STATS

#ifdef THREAD_STATS
/*
 * thread iteration cmd
 */
#define THREAD_STATIST_INIT	0 /* start statist thread loading with a period */
#define THREAD_STATIST_CALC	1 /* calc thread loading at the end of period */
#define THREAD_STATIST_DUMP	2 /* calc and dump loading at the end of period */
void __threads_iterate(int cmd, void *para);

#endif /* THREAD_STATS */

/* the cpu stats thread incharge of cpu and thread loading statistics */
void cpu_stats_init(void);

#endif /* CPU_STATS */

#ifdef RT_USING_MEMTRACE
int memcheck(void);
int memtrace(int argc, char **argv);
#endif /* RT_USING_MEMTRACE */

#ifdef EE_GLOBAL_MEM_DEBUG
/* for global memory debug */
#include "osal_mem.h"
#endif

/* save log buffer to sdcard */
void force_save_log_buf();

#if defined(RT_USING_HOOK) && defined(RT_DEBUG)
void rt_object_hook_init(void);
#endif

void rt_thread_daemon_init(void);
void rt_thread_daemon_init_single(int cpu);

void rt_thread_guard_init(void);

#ifdef ARCH_LOMBO
#ifdef RT_USING_DEADLOCK_MONITOR
void rt_thread_deadlock_monitor_init(void);
void rt_thread_deadlock_monitor_init_single(int cpu);
void rt_release_deadlock_mbtimer(int cpu);
void rt_detach_deadlock_monitor_thread(int cpu);
#endif
#endif

extern rt_uint32_t rt_thread_ready_priority_group;

#ifdef RT_USING_SMP
int is_idle_thread(rt_thread_t tid);
void rt_kprintf_buf(char *buf, rt_size_t off);
void rt_kprintf_raw(const char *fmt, ...);
rt_uint8_t disable_async_print(void);
void enable_async_print(rt_uint8_t en);

extern rt_uint8_t console_closed;
void enable_console(void);
void disable_console(void);

void console_write(const char *buf, rt_size_t length);
#else
static inline int is_idle_thread(rt_thread_t tid)
{
	return 0;
}
static inline void rt_kprintf_buf(char *buf, rt_size_t off)
{
	return;
}

extern rt_int16_t rt_scheduler_lock_nest;
#endif

/**
 * rt_thread_need_sched - check if any thread in ready queuem (need schedule)
 *
 * for performance consideration, this func called without _cpus_lock.
 *
 * usually the procedure is like:
 *     if (rt_thread_need_sched())
 *           rt_schedule();
 * the schedule condition will be re-checked inside the above rt_schedule (with
 * _cpus_lock aquired), so it's ok not to aquire _cpus_lock in rt_thread_need_sched.
 *
 * return 1 if need schedule, 0 if not
 */
static inline int rt_thread_need_sched(void)
{
#ifdef RT_USING_SMP
	return (rt_thread_ready_priority_group != 0
			|| rt_cpu_self()->priority_group != 0);
#else
	return (rt_thread_ready_priority_group != 0);
#endif
}

static inline int in_interrupt_context()
{
	return !!rt_interrupt_get_nest();
}

/**
 * rt_thread_can_sched - check if cur thread can be scheduded out immediately
 *
 * for performance consideration, this func called without _cpus_lock.
 *
 * usually the procedure is like:
 *     if (rt_thread_can_sched())
 *           rt_schedule();
 * the schedule condition will be re-checked inside the above rt_schedule (with
 * _cpus_lock aquired), so it's ok not to aquire _cpus_lock in rt_thread_can_sched.
 *
 * The difference betwween rt_thread_need_sched and this func is:
 *     the rt_schedule() in rt_thread_need_sched may not switch to another thread
 *  immediately, for example, in irq context or with _cpus_lock acuqired.
 *     While the rt_schedule() in rt_thread_can_sched will switch to another thread
 *  immediately.
 *
 * return 1 if can schedule, 0 if not
 */
static inline int rt_thread_can_sched(void)
{
	rt_thread_t cur_thread = rt_thread_self();

	/* not in irq context, scheduler unlocked, and ready group not null */
	if (!in_interrupt_context() && RT_NULL != cur_thread) {
#ifdef RT_USING_SMP
		/* scheduler unlocked */
		if (0 == cur_thread->scheduler_lock_nest
			/* && 0 == cur_thread->cpus_lock_nest */)
			return (rt_thread_ready_priority_group != 0
				|| rt_cpu_self()->priority_group != 0);
#else
		/* scheduler unlocked */
		if (0 == rt_scheduler_lock_nest)
			return (rt_thread_ready_priority_group != 0);
#endif
	}

	return 0;
}

#ifdef RT_USING_PM
long sys_standby(void);
long sys_shutdown(void);
void rt_pm_standby(void);
void rt_pm_deepsleep(void);
void deepslp_cpu_suspend(void);
void deepslp_cpu_resume1(void);
void deepslp_cpu_resume2(void);
void rt_pm_shutdown(void);
void lb_hw_power_off(void);
void lb_hw_reboot(void);
#else
static inline long sys_standby(void) { return 0; }
static inline long sys_shutdown(void){ return 0; }
static inline void rt_pm_standby(void) {}
static inline void rt_pm_shutdown(void) {}
static inline void rt_pm_deepsleep(void) {}
static inline void deepslp_cpu_suspend(void) {}
static inline void deepslp_cpu_resume1(void) {}
static inline void deepslp_cpu_resume2(void) {}
static inline void lb_hw_power_off(void) {}
static inline void lb_hw_reboot(void) {}
#endif /* RT_USING_PM */

#endif /* ARCH_LOMBO */

#ifdef __cplusplus
}
#endif

#endif
