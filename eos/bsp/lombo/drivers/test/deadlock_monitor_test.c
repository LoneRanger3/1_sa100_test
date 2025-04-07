#include <rtthread.h>
#include <rthw.h>
#include <debug.h>
#include "spinlock.h"

static struct rt_thread deadlock_thread[RT_CPUS_NR];
ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t deadlock_thread_stack[RT_CPUS_NR][RT_TIMER_THREAD_STACK_SIZE];
static spinlock_t	lock[RT_CPUS_NR];
static int sg_cpu_deadlock;
static int multi_cpu_deadlock;

#define RT_SOFTLOCKUP_TEST_PRIO		8

static void recursive_func(int cpu)
{
	int i = 65535;

	spin_lock(&lock[cpu]);

	while (--i > 0)
		;

	spin_unlock(&lock[cpu]);

	return;
}

static void rt_deadlock_entry(void *parameter)
{
	int cpu = (int)parameter;
	if (sg_cpu_deadlock == 1) {

		spin_lock(&lock[cpu]);
		if (cpu == 0)
			recursive_func(cpu);
		spin_unlock(&lock[cpu]);

	} else if (multi_cpu_deadlock == 1) {

		/* one of the cpu get the lock first */
		spin_lock(&lock[0]);
		while (1)
			;
		spin_unlock(&lock[0]);

	}
	return;
}

long deadlock_monitor_test(int argc, char **argv)
{
	int i = 0;
	char name[RT_NAME_MAX];
	char *test_mode = argv[2];

	if (argc < 3) {
		LOG_E("Usage: ./lombo_test dlockm [multi/single]\n");
		return 0;
	}
	if (!strcmp(test_mode, "single"))
		sg_cpu_deadlock = 1;
	else if (!strcmp(test_mode, "multi"))
		multi_cpu_deadlock = 1;
	else {
		LOG_E("Usage: ./lombo_test dlockm [multi/single]\n");
		return 0;
	}

	for (i = 0; i < sizeof(deadlock_thread) / sizeof(deadlock_thread[0]); ++i) {

		spin_lock_init(&lock[i]);

		rt_sprintf(name, "dlm-test%d", i);

		rt_thread_init(&deadlock_thread[i],
			name,
			rt_deadlock_entry,
			(void *)i,
			&deadlock_thread_stack[i][0],
			sizeof(deadlock_thread_stack[i]),
			RT_SOFTLOCKUP_TEST_PRIO,
			20);

		rt_thread_control(&deadlock_thread[i],
				RT_THREAD_CTRL_BIND_CPU, (void *)i);
		rt_thread_startup(&deadlock_thread[i]);
	}
	return 0;
}
