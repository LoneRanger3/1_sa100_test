/*
 * spinlock.h - spinlock operations
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
#include <csp.h>

/*
 * preempt operations
 */
#ifdef RT_USING_SMP
#define preempt_enable()						\
do {									\
	rt_base_t lock;							\
	rt_thread_t self;						\
									\
	lock = rt_hw_local_irq_disable();				\
	self = rt_cpu_self()->current_thread;				\
	if (RT_NULL != self) {						\
		barrier();						\
		self->scheduler_lock_nest += (-1);			\
		barrier();						\
	}								\
	rt_hw_local_irq_enable(lock);					\
	/* preempt_check_resched(); */					\
} while (0)

#define preempt_disable()						\
do {									\
	rt_base_t lock;							\
	rt_thread_t self;						\
									\
	lock = rt_hw_local_irq_disable();				\
	self = rt_cpu_self()->current_thread;				\
	if (RT_NULL != self) {						\
		self->scheduler_lock_nest += (1);			\
		barrier();						\
	}								\
	rt_hw_local_irq_enable(lock);					\
} while (0)

#else /* !RT_USING_SMP */

extern rt_int16_t rt_scheduler_lock_nest;
#define add_preempt_count(val)						\
do {									\
	rt_base_t lock;							\
									\
	lock = rt_hw_interrupt_disable();				\
	rt_scheduler_lock_nest += (val);				\
	rt_hw_interrupt_enable(lock);					\
} while (0)

#define inc_preempt_count()	add_preempt_count(1)
#define dec_preempt_count()	add_preempt_count(-1)

#define sched_preempt_enable_no_resched()				\
do {									\
	barrier();							\
	dec_preempt_count();						\
} while (0)

#define preempt_enable_no_resched()	sched_preempt_enable_no_resched()

#define preempt_enable()						\
do {									\
	if (RT_NULL != rt_thread_self()) {				\
		preempt_enable_no_resched();				\
		barrier();						\
		/* preempt_check_resched(); */				\
	}								\
} while (0)

#define preempt_disable()						\
do {									\
	if (RT_NULL != rt_thread_self()) {				\
		inc_preempt_count();					\
		barrier();						\
	}								\
} while (0)
#endif /* RT_USING_SMP */

/*
 * spinlock operations compatible with linux
 */
#ifdef RT_USING_SMP

typedef rt_hw_spinlock_t spinlock_t;

/* initial unlock status */
#define __ARCH_SPIN_LOCK_UNLOCKED		0

/* global spinlock defination: "DEFINE_SPINLOCK(x);" */
#define DEFINE_SPINLOCK(x)						\
	spinlock_t x = { .slock = __ARCH_SPIN_LOCK_UNLOCKED }

/* spinlock initialize to unlocked state */
#define spin_lock_init(_lock)						\
do {									\
	(_lock)->slock = __ARCH_SPIN_LOCK_UNLOCKED;			\
} while (0)

#define spin_lock_irqsave(lock, flags)					\
	do {								\
		flags = (unsigned long)rt_hw_local_irq_disable();	\
		preempt_disable();					\
		rt_hw_spin_lock(lock);					\
	} while (0)

#define spin_unlock_irqrestore(lock, flags)				\
	do {								\
		rt_hw_spin_unlock(lock);				\
		preempt_enable();					\
		rt_hw_local_irq_enable((rt_base_t)flags);		\
		/* preempt_enable(); move to above.. */			\
	} while (0)

#define spin_trylock_irqsave(lock, flags)				\
({									\
	flags = (unsigned long)rt_hw_local_irq_disable();		\
	__raw_spin_trylock(lock) ?					\
	1 : ({ rt_hw_local_irq_enable((rt_base_t)flags); 0; });		\
})

#define spin_lock(lock)							\
	do {								\
		preempt_disable();					\
		rt_hw_spin_lock(lock);					\
	} while (0)

#define spin_unlock(lock)						\
	do {								\
		rt_hw_spin_unlock(lock);				\
		preempt_enable();					\
	} while (0)

#else /* !RT_USING_SMP */

typedef rt_int32_t spinlock_t;

#define DEFINE_SPINLOCK(x)

#define spin_lock_init(_lock)

#define spin_lock_irqsave(lock, flags)					\
	do {								\
		flags = (unsigned long)rt_hw_interrupt_disable();	\
		preempt_disable();					\
	} while (0)

#define spin_unlock_irqrestore(lock, flags)				\
	do {								\
		preempt_enable();					\
		rt_hw_interrupt_enable((rt_base_t)flags);		\
	} while (0)

#define spin_lock(lock)							\
	do {								\
		preempt_disable();					\
	} while (0)

#define spin_unlock(lock)						\
	do {								\
		preempt_enable();					\
	} while (0)

#endif /* RT_USING_SMP */
