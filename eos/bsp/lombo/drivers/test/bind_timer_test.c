#include <rtthread.h>
#include <rthw.h>

#define DBG_SECTION_NAME        "BIND_TIMERT"
#define DBG_LEVEL               DBG_INFO
#include <debug.h>

static struct rt_thread timer_thread[RT_CPUS_NR];
ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t timer_thread_stack[RT_CPUS_NR][RT_TIMER_THREAD_STACK_SIZE];
#define RT_BIND_TIMER_TEST_PRIO		8
#define RT_BIND_TIMER_TEST_COUNT	10000
static struct rt_timer cpu0_timer;
static struct rt_timer cpu1_timer;
static struct rt_timer cpu2_timer;

static void bind_timer_timeout_0(void *parameter)
{
	rt_timer_t t = (rt_timer_t)parameter;
	rt_tick_t tick;
	static rt_tick_t first;
	static rt_tick_t pre;
	static int flag;
	static int count;

	count++;
	if (count == RT_BIND_TIMER_TEST_COUNT) {
		/* here we need stop this bind timer */
		rt_timer_stop(t);
		rt_timer_detach(t);
		return;
	}

	tick = rt_tick_get();
	pre = first;
	first = tick;
	if (flag == 0) {
		++flag;
	} else if (flag == 1) {
		if (pre + 100 == first)
			LOG_I("global timer, test ok, curr tick: %lu, curr count:%lu",
									tick, count);
		else
			LOG_I("global timer, test error, curr tick: %lu, curr count:%lu",
									tick, count);
	}
}

static void bind_timer_timeout_1(void *parameter)
{
	rt_timer_t t = (rt_timer_t)parameter;
	rt_tick_t tick;
	static rt_tick_t first;
	static rt_tick_t pre;
	static int flag;
	static int count;

	count++;
	if (count == RT_BIND_TIMER_TEST_COUNT) {
		/* here we need stop this bind timer */
		rt_timer_stop(t);
		rt_timer_detach(t);
		return;
	}

	tick = rt_cpu_index(t->bind_cpu)->tick;
	pre = first;
	first = tick;
	if (flag == 0) {
		++flag;
	} else if (flag == 1) {
		if (pre + 120 == first)
			LOG_I("bind timer, test ok, curr tick: %lu, curr count:%lu",
									tick, count);
		else
			LOG_I("bind timer, test error, curr tick: %lu, curr count:%lu",
									tick, count);
	}
}

static void bind_timer_timeout_2(void *parameter)
{
	rt_timer_t t = (rt_timer_t)parameter;
	rt_tick_t tick;
	static rt_tick_t first;
	static rt_tick_t pre;
	static int flag;
	static int count;

	count++;
	if (count == RT_BIND_TIMER_TEST_COUNT) {
		/* here we need stop this bind timer */
		rt_timer_stop(t);
		rt_timer_detach(t);
		return;
	}

	tick = rt_cpu_index(t->bind_cpu)->tick;
	pre = first;
	first = tick;
	if (flag == 0) {
		++flag;
	} else if (flag == 1) {
		if (pre + 50 == first)
			LOG_I("bind timer, test ok, curr tick: %lu, curr count:%lu",
									tick, count);
		else
			LOG_I("bind timer, test error, curr tick: %lu, curr count:%lu",
									tick, count);
	}
}

static void rt_thread_timer_entry(void *parameter)
{
	int i = (int) parameter;

	if (i == 0) {
		/* start a period bind_timer in cpu0 */
		rt_timer_init_bind(&cpu2_timer, "bind_timer2",
		bind_timer_timeout_2, (void *)&cpu2_timer, 50, RT_TIMER_FLAG_PERIODIC, 1);

		/* start this timer */
		rt_timer_start(&cpu2_timer);

	} else {
		/* start a global timer in cpu1 */
		rt_timer_init(&cpu0_timer, "bind_timer0",
		bind_timer_timeout_0, (void *)&cpu0_timer, 100, RT_TIMER_FLAG_PERIODIC);

		/*start this timer */
		rt_timer_start(&cpu0_timer);

		/* start a period bind_timer in cpu1 */
		rt_timer_init_bind(&cpu1_timer, "bind_timer1",
		bind_timer_timeout_1, (void *)&cpu1_timer, 120,
						RT_TIMER_FLAG_PERIODIC, 1);

		/* start this timer */
		rt_timer_start(&cpu1_timer);
	}
}

long bind_timer_test(int argc, char **argv)
{
	int i = 0;

	for (i = 0; i < sizeof(timer_thread) / sizeof(timer_thread[0]); ++i) {

		rt_thread_init(&timer_thread[i],
			"binding-timer",
			rt_thread_timer_entry,
			(void *)i,
			&timer_thread_stack[i][0],
			sizeof(timer_thread_stack[i]),
			RT_BIND_TIMER_TEST_PRIO,
			20);

		rt_thread_control(&timer_thread[i], RT_THREAD_CTRL_BIND_CPU, (void *)i);
		rt_thread_startup(&timer_thread[i]);
	}
	return 0;
}
