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

#ifndef __OSCL_SEMAPHORE_H__
#define __OSCL_SEMAPHORE_H__
#include <clock_time.h>
#include <div.h>

#define oscl_sem_close(sem) sem_close(sem)
#define oscl_sem_destroy(sem) sem_destroy(sem)
#define oscl_sem_getvalue(sem, sval) sem_getvalue(sem, sval)
#define oscl_sem_init(sem, pshared, value) sem_init(sem, pshared, value)
#define oscl_sem_open(...) sem_open(__VA_ARGS__)
#define oscl_sem_post(sem) sem_post(sem)
#define oscl_sem_timedwait(sem, abs_timeout) sem_timedwait(sem, abs_timeout)
#define oscl_sem_trywait(sem) sem_trywait(sem)
#define oscl_sem_unlink(name) sem_unlink(name)
#define oscl_sem_wait(sem) sem_wait(sem)

static inline int sem_get(sem_t *sem)
{
	return sem->sem->value;
}

static inline int oscl_sem_timedwait_ms(sem_t *sem, int time_ms)
{
	struct timespec time;
	int ret;
	u64 total_nsec;

	clock_gettime(CLOCK_REALTIME, &time);
	total_nsec = (u64)time.tv_nsec + (u64)time_ms*1000000L;
	time.tv_nsec = do_div_rem(total_nsec, 1000000000L);
	time.tv_sec += total_nsec;
	ret = sem_timedwait(sem, &time);
	return ret;
}

static inline int _oscl_sem_wait_debug(sem_t *sem, char *file, int line)
{
	while (oscl_sem_timedwait_ms(sem, 1000))
		OSCL_LOGE("wait sem timeout, %s %d", file, line);
	return 0;
}
#define oscl_sem_wait_debug(sem) _oscl_sem_wait_debug(sem, __FILE__, __LINE__)

#endif /* __OSCL_SEMAPHORE_H__ */

