/*
 * eos.h - eos common head file
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

#ifndef __EOS_H__
#define __EOS_H__

#include <sizes.h>

/*
 * definations for c and asm
 */

/*
 * pthread default prio and stack size, moved from pthread_attr.c
 */
#define DEFAULT_STACK_SIZE	2048
#define DEFAULT_PRIORITY	(RT_THREAD_PRIORITY_MAX / 2 + RT_THREAD_PRIORITY_MAX / 4)
#define PTHREAD_DEFAULT_STACK_SIZE	DEFAULT_STACK_SIZE
#define PTHREAD_DEFAULT_PRIO		DEFAULT_PRIORITY
#define PTHREAD_DEFAULT_TICK_CNT	5 /* see pthread_create func */

/*
 * thread priority definations
 */
#define SDC_BOTTOM_HALF_THREAD_PRIO		8
#define IDLE_THREAD_PRIO			(RT_THREAD_PRIORITY_MAX - 1)

/*
 * daemon thread prio must be high enough, to let defunct thread be freed in time
 *
 * when cpu is dying, its daemon thread should be scheduled as soon as possible (to
 * let cpu_die be called), so change its prio to DAEMON_THREAD_DYING_PRIO before cpu down
 *
 * the dying cpu's daemon thread will be detached before cpu down, and reinitialized with
 * prio DAEMON_THREAD_PRIO when powered up
 */
#define DAEMON_THREAD_PRIO		10
#define DAEMON_THREAD_DYING_PRIO	2

/* guard thread prio must high enough to print in time */
#define GUARD_THREAD_PRIO		PTHREAD_DEFAULT_PRIO

#ifdef ARCH_LOMBO
#ifdef RT_USING_DEADLOCK_MONITOR
/* deadlock monitor thread prio */
#define LOCKUP_MONITOR_THREAD_PRIO		3
/* deadlock monitor stack size */
#define LOCKUP_MONITOR_THREAD_STACK_SIZE	4096
/* deadlock monitor tick cnt */
#define LOCKUP_MONITOR_THREAD_TICK_CNT		PTHREAD_DEFAULT_TICK_CNT
#endif
#endif

/*
 * thread stack size definations
 */
#define DEMO_THREAD_STACK_SIZE			2048
#define SDC_BOTTOM_HALF_THREAD_STACK_SIZE	2048
#define DAEMON_THREAD_STACK_SIZE		4096
#define MPSTAT_STACK_SIZE			0x4000 /* high enough incase overflow */
#define CPU_STATS_STACK_SIZE			PTHREAD_DEFAULT_STACK_SIZE
#define GUARD_STACK_SIZE			PTHREAD_DEFAULT_STACK_SIZE

#define INPUT_EVENT_PROC_THREAD_STACK_SIZE	2048
#define INPUT_INT_THREAD_STACK_SIZE		2048
#define INPUT_THREAD_PRIORITY			20

#define DMA_THREAD_STACK_SIZE			4096

#ifdef FUNC_PROFILE
#define PROFILE_SAVE_THREAD_STACK_SIZE		4096
#define PROFILE_THREAD_PRIORITY			10
#endif

#ifdef LOMBO_SAVE_LOG
#define SAVE_LOG_THREAD_STACK_SIZE		4096
#define SAVE_LOG_THREAD_PRIORITY		10
#endif

/*
 * thread tick cnt(time slice) definations.
 *  Unit: (1000 / RT_TICK_PER_SECOND) ms
 */
#define DAEMON_THREAD_TICK_CNT			10	/* 100ms */
#define GUARD_THREAD_TICK_CNT			PTHREAD_DEFAULT_TICK_CNT

/*
 * time const definations
 */
#define MSEC_PER_SEC				1000L
#define USEC_PER_SEC				1000000L
#define NSEC_PER_SEC				1000000000L
#define msecs_to_tick(ms)			((ms) / (1000 / RT_TICK_PER_SECOND))

/*
 * other definations
 */
#ifndef NULL
#define NULL			(0)
#endif

#ifndef BIT
#define BIT(nr)			(1UL << (nr))
#endif

#ifdef ARCH_LOMBO_N7
#define Mode_USR		0x10
#define Mode_FIQ		0x11
#define Mode_IRQ		0x12
#define Mode_SVC		0x13
#define Mode_ABT		0x17
#define Mode_UND		0x1B
#define Mode_SYS		0x1F

#define I_Bit			0x80 /* when I bit is set, IRQ is disabled */
#define F_Bit			0x40 /* when F bit is set, FIQ is disabled */
#endif

/*
 * definations only for c
 */
#ifndef __ASSEMBLY__

/* ------------------- compiler option definations ------------------ */

#define likely(x)		__builtin_expect(!!(x), 1)
#define unlikely(x)		__builtin_expect(!!(x), 0)

#define __iomem
#define __force

#ifndef __aligned
#define __aligned(x)		__attribute_((aligned(x)))
#endif

#ifndef __packed
#define __packed		__attribute__((packed))
#endif

/*
 * Prevent the compiler from merging or refetching accesses.  The compiler
 * is also forbidden from reordering successive instances of ACCESS_ONCE(),
 * but only when the compiler is aware of some particular ordering.  One way
 * to make the compiler aware of ordering is to put the two invocations of
 * ACCESS_ONCE() in different C statements.
 *
 * This macro does absolutely -nothing- to prevent the CPU from reordering,
 * merging, or refetching absolutely anything at any time.  Its main intended
 * use is to mediate communication between process-level code and irq/NMI
 * handlers, all running on the same CPU.
 */
#define ACCESS_ONCE(x)		(*(volatile typeof(x) *)&(x))

/* ------------------- compiler option definations ------------------ */

/* ------------------------ type definations ------------------------ */

typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
typedef long long s64;
typedef int s32;
typedef short s16;
typedef char s8;

/* ------------------------ type definations ------------------------ */

/* ------------------- barrier related definations ------------------ */

/*
 * barrier will lead an extra load instruction at the end of WRITEREG32, which will
 * be fatal err in some cases. so remove it temporarily, but need to find why.
 *   -- has been fixed in newer toolchain(7-2017-q4-major), so recover it
 */
#define barrier()		({ asm volatile("" : : : "memory"); })

#define cpu_relax()		barrier()

#ifdef ARCH_LOMBO_N7
#define nop()			({ asm volatile("mov\tr0,r0\t@ nop\n\t"); })

#define sev()			({ asm volatile("sev" : : : "memory"); })
#define wfe()			({ asm volatile("wfe" : : : "memory"); })
#define wfi()			({ asm volatile("wfi" : : : "memory"); })

#define isb(option)		({ asm volatile("isb " #option : : : "memory"); })
#define dsb(option)		({ asm volatile("dsb " #option : : : "memory"); })
#define dmb(option)		({ asm volatile("dmb " #option : : : "memory"); })

static inline void outer_sync(void) { }

#define mb()			do { dsb(); outer_sync(); } while (0)
#define rmb()			dsb()
#define wmb()			do { dsb(st); outer_sync(); } while (0)
#else
#define mb()			barrier()
#define rmb()			barrier()
#define wmb()			barrier()
#endif /* ARCH_LOMBO_N7 */

#ifndef RT_USING_SMP
#define smp_mb()			barrier()
#define smp_rmb()			barrier()
#define smp_wmb()			barrier()
#else
#define smp_mb()			dmb(ish)
#define smp_rmb()			smp_mb()
#define smp_wmb()			dmb(ishst)
#endif

#define read_barrier_depends()		do { } while (0)
#define smp_read_barrier_depends()	do { } while (0)

#define set_mb(var, value)		do { var = value; smp_mb(); } while (0)

#define __iormb()			rmb()
#define __iowmb()			wmb()

/* ------------------- barrier related definations ------------------ */

/* ------------------------ register operation ---------------------- */

#ifdef ARCH_LOMBO_N7
static inline u8 __raw_readb(const volatile void __iomem *addr)
{
	u8 val;
	asm volatile("ldrb %1, %0"
		     : "+Qo" (*(volatile u8 __force *)addr),
		       "=r" (val));
	return val;
}

static inline u32 __raw_readl(const volatile void __iomem *addr)
{
	u32 val;
	asm volatile("ldr %1, %0"
		     : "+Qo" (*(volatile u32 __force *)addr),
		       "=r" (val));
	return val;
}

static inline void __raw_writeb(u8 val, volatile void __iomem *addr)
{
	asm volatile("strb %1, %0"
		     : "+Qo" (*(volatile u8 __force *)addr)
		     : "r" (val));
}

static inline void __raw_writel(u32 val, volatile void __iomem *addr)
{
	asm volatile("str %1, %0"
		     : "+Qo" (*(volatile u32 __force *)addr)
		     : "r" (val));
}
#else
#define __raw_readb(c)		({ u32 __v = *(const volatile u8 *)(c); __v; })
#define __raw_readl(c)		({ u32 __v = *(const volatile u32 *)(c); __v; })
#define __raw_writeb(v, c)	({ *(volatile u8 *)(c) = (v); })
#define __raw_writel(v, c)	({ *(volatile u8 *)(c) = (v); })
#endif /* ARCH_LOMBO_N7 */

#define readb_relaxed(c)	({ u8  __r = __raw_readb((volatile void *)(c)); __r; })
#define readl_relaxed(c)	({ u32 __r = __raw_readl((volatile void *)(c)); __r; })
#define writeb_relaxed(v, c)	__raw_writeb((u8)(v), (volatile void *)(c))
#define writel_relaxed(v, c)	__raw_writel((u32)(v), (volatile void *)(c))

#ifndef readb
#define readb(c)		({ u8  __v = readb_relaxed(c); __iormb(); __v; })
#endif

#ifndef readl
#define readl(c)		({ u32 __v = readl_relaxed(c); __iormb(); __v; })
#endif

#ifndef writeb
#define writeb(v, c)		({ __iowmb(); writeb_relaxed(v, c); })
#endif

#ifndef writel
#define writel(v, c)		({ __iowmb(); writel_relaxed(v, c); })
#endif

/* ------------------------ register operation ---------------------- */

/* ------------------- common funciton definations ------------------ */

static inline u32 is_power_of_2(u32 n)
{
	return (n != 0 && ((n & (n - 1)) == 0));
}

/**
 * ffs - find first bit set
 * @x: the word to search
 *
 * This is defined the same way as
 * the libc and compiler builtin ffs routines, therefore
 * differs in spirit from the above ffz (man ffs).
 */
static inline int ffs(int x)
{
	int r = 1;

	if (!x)
		return 0;
	if (!(x & 0xffff)) {
		x >>= 16;
		r += 16;
	}
	if (!(x & 0xff)) {
		x >>= 8;
		r += 8;
	}
	if (!(x & 0xf)) {
		x >>= 4;
		r += 4;
	}
	if (!(x & 3)) {
		x >>= 2;
		r += 2;
	}
	if (!(x & 1)) {
		x >>= 1;
		r += 1;
	}
	return r;
}

#define DIV_ROUND_UP(n, d) (((n) + (d) - 1) / (d))
/*
 * Divide positive or negative dividend by positive divisor and round
 * to closest integer. Result is undefined for negative divisors and
 * for negative dividends if the divisor variable type is unsigned.
 */
#define DIV_ROUND_CLOSEST(x, divisor)(			\
{							\
	typeof(x) __x = x;				\
	typeof(divisor) __d = divisor;			\
	(((typeof(x))-1) > 0 ||				\
	 ((typeof(divisor))-1) > 0 || (__x) > 0) ?	\
		(((__x) + ((__d) / 2)) / (__d)) :	\
		(((__x) - ((__d) / 2)) / (__d));	\
}							\
)

/*
 *  BOOT_FLAG_FAST: booster load kernel directly(without leaver), only can boot from nor
 *  BOOT_FLAG_NORMAL: normal boot
 */
#define BOOT_FLAG_NORMAL	0
#define BOOT_FLAG_FAST		1

extern u32 boot_flag;

void udelay(u32 us);
void mdelay(u32 ms);

long list_thread(void);
long list_sem(void);
long list_timer(void);
long list_mutex(void);
long list_mailbox(void);
long list_msgqueue(void);
long list_event(void);
long list_mempool(void);
long list_device(void);
int list_module(void);
long list_memheap(void);

/*
 * If store_en bit is 1, crom get the storage type firstly by reading from reg,
 * not by scanning. Only if the storage type read is wrong (cannot read write?), then
 * the crom try scanning media.
 */
int rtc_store_en_flag_get(void);
void rtc_store_en_flag_set(int enable);
void rtc_pol_flag_set(void);

/* ------------------- common funciton definations ------------------ */

#ifdef __cplusplus
extern "C" {
#endif

u32 lombo_func1(void);
u32 lombo_func2(u32 *p1, u32 *p2);
u32 lombo_func3();
s32 pdef_rd(u32 rd_value);

#ifdef __cplusplus
}
#endif


#endif /* __ASSEMBLY__ */

#endif /* __EOS_H__ */
