/*
 * cpufreq_adaptive.c - Governor of LomboTech CPUfreq
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
#include <sizes.h>
#include <stdlib.h>
#include <string.h>
#include "cpufreq.h"

#define DBG_LEVEL				DBG_INFO
#include <debug.h>

#define CPUFREQ_ADAPTIVE_CYCLE			(10)
#define CPUFREQ_ADAPTIVE_SAMPLING_RATE_MS	(50)
#define CPUFREQ_ADAPTIVE_TARGET_LOAD		(90)

/* #define CPUFREQ_ADAPTIVE_DEBUG */

/* set to max freq if the loading exceeds the threshold */
#define CPUFREQ_UP_MAX_LOAD(freq)     \
	(freq < 400000 ? 95 : (freq < 600000 ? 95 : (freq < 800000 ? 90 : 90)))

/* set to next higher freq if the loading exceeds the threshold */
#define CPUFREQ_UP_LOAD(freq)         \
	(freq < 400000 ? 60 : (freq < 600000 ? 70 : (freq < 800000 ? 80 : 80)))

/* set to lower freq if the loading bellow the threshold */
#define CPUFREQ_DOWN_LOAD(freq)       \
	(freq < 400000 ? 30 : (freq < 600000 ? 40 : (freq < 800000 ? 50 : 50)))

struct cpu_load {
	u32 freq;
	u32 loading_max;
	u32 loading[RT_CPUS_NR];
};

struct cpu_load_record {
	struct cpu_load loads[CPUFREQ_ADAPTIVE_CYCLE];
	u32 index;
};

struct cpufreq_adaptive {
	char name[16];
	struct cpufreq_policy *cur_policy;
	struct cpu_load_record *records;
	u32 pre_cpu_total[RT_CPUS_NR];
	u32 pre_cpu_idle[RT_CPUS_NR];
	u32 sampling_rate_ms;
	u32 load_debug;
	struct rt_timer cpufreq_adaptive_timer;
	struct rt_mutex cpufreq_adaptive_mutex;
};

static struct cpufreq_adaptive *governor;
static u32 cpufreq_adaptive_inited;

static u32 freq_down_weight_list[CPUFREQ_ADAPTIVE_CYCLE] = {
	25,
	20,
	15,
	10,
	5,
	5,
	5,
	5,
	5,
	5
};

static int check_freq_change(struct cpufreq_adaptive *governor,
		u32 *target, u32 index)
{
	u32 freq_load = 0;
	u32 max_load = 0;
	struct cpufreq_policy *policy = governor->cur_policy;
	int i, idx_cur, idx_mod;

#ifdef CPUFREQ_ADAPTIVE_DEBUG
	if (governor->load_debug) {
		for (i = 0; i < CPUFREQ_ADAPTIVE_CYCLE; i++)
			LOG_I("loading_max[%d]=%u", i,
				governor->records->loads[i].loading_max);
	}
#endif

	max_load = governor->records->loads[index].loading_max;

	if (governor->load_debug)
		LOG_I("cur_freq=%d, load[max=%d, up_max=%d, up=%d, down=%d]",
			policy->cur, max_load,
			CPUFREQ_UP_MAX_LOAD(policy->cur),
			CPUFREQ_UP_LOAD(policy->cur),
			CPUFREQ_DOWN_LOAD(policy->cur));

	if (max_load > CPUFREQ_UP_MAX_LOAD(policy->cur)) {
		*target = policy->max;
		if (governor->load_debug)
			LOG_I("request max freq: %dKHz", *target);
		return 1;
	}

	if (max_load > CPUFREQ_UP_LOAD(policy->cur)) {
		*target = policy->cur + 1000;
		if (governor->load_debug)
			LOG_I("request next higher freq: %dKHz", *target);
		return 1;
	} else if (max_load >= CPUFREQ_DOWN_LOAD(policy->cur)) {
		*target = policy->cur;
		if (governor->load_debug)
			LOG_I("request cur freq: %dKHz", *target);
		return 0;
	}

#if 0
	freq_load = (policy->cur * max_load) / 100;
	freq_load = (policy->cur * max_load) / CPUFREQ_UP_LOAD(freq_load);
#else
	idx_mod = index % CPUFREQ_ADAPTIVE_CYCLE;

	for (i = 0; i < CPUFREQ_ADAPTIVE_CYCLE; i++) {
		idx_cur = idx_mod - i;
		idx_cur = idx_cur < 0 ? idx_cur + CPUFREQ_ADAPTIVE_CYCLE : idx_cur;
		freq_load += governor->records->loads[idx_cur].loading_max
				* freq_down_weight_list[i];
	}

	freq_load /= 100;

	freq_load = (policy->cur * freq_load) / 100;

	freq_load = freq_load * 100 / CPUFREQ_ADAPTIVE_TARGET_LOAD;
#endif

	/* select target frequency */
	*target = freq_load;

	if (governor->load_debug)
		LOG_I("request lower freq: %dKHz", *target);

	return 1;
}

static void do_check_cpu(struct cpufreq_adaptive *governor)
{
	u32 cur_index = governor->records->index;
	u32 freq_target = 0;
	u32 max_load = 0;
	int i;

	++governor->records->index;
	governor->records->loads[cur_index].freq = governor->cur_policy->cur;

	for (i = 0; i < RT_CPUS_NR; i++) {
		u32 cur_cpu_total, cur_cpu_idle;
		u32 pre_cpu_total, pre_cpu_idle;
		u32 total_time, idle_time;
		u32 load = 0;

		pre_cpu_total = governor->pre_cpu_total[i];
		pre_cpu_idle = governor->pre_cpu_idle[i];

		cur_cpu_total = get_cpu_total_time(i);
		cur_cpu_idle = get_cpu_idle_time(i);

		governor->pre_cpu_total[i] = cur_cpu_total;
		governor->pre_cpu_idle[i] = cur_cpu_idle;

		total_time = cur_cpu_total - pre_cpu_total;
		idle_time = cur_cpu_idle - pre_cpu_idle;

		if (total_time && (total_time > idle_time))
			load = 100 * (total_time - idle_time) / total_time;
		else
			load = 0;

		governor->records->loads[cur_index].loading[i] = load;

		if (governor->load_debug)
			LOG_I("cpu%d:%u,%u", i,
				governor->records->loads[cur_index].freq,
				governor->records->loads[cur_index].loading[i]);
	}

	for (i = 0; i < RT_CPUS_NR; i++)
		max_load = max(max_load, governor->records->loads[cur_index].loading[i]);

	governor->records->loads[cur_index].loading_max = max_load;

	if (check_freq_change(governor, &freq_target, cur_index))
		__cpufreq_driver_target(governor->cur_policy,
				freq_target, CPUFREQ_RELATION_L);

	if (governor->records->index == CPUFREQ_ADAPTIVE_CYCLE)
		governor->records->index = 0;
}

/**
 * cpufreq_adaptive_timer_func - calculate cpu loading and set cpu frequency
 * @param: pointer to cpufreq adaptive struct
 */
static void cpufreq_adaptive_timer_func(void *param)
{
	struct cpufreq_adaptive *governor = (struct cpufreq_adaptive *)param;

	rt_mutex_take(&governor->cpufreq_adaptive_mutex, RT_WAITING_FOREVER);

	do_check_cpu(governor);

	rt_mutex_release(&governor->cpufreq_adaptive_mutex);
}

/**
 * cpufreq_adaptive_init - init adaptive governor
 *
 * return 0 if success, !0 error
 */
int cpufreq_adaptive_init(void)
{
	int i, err = 0;

	/* malloc cpufreq_adaptive struct */
	governor = (struct cpufreq_adaptive *)rt_zalloc(sizeof(struct cpufreq_adaptive));
	if (!governor) {
		LOG_E("failed to malloc for cpufreq adaptive");
		err = __LINE__;
		goto out;
	}

	rt_sprintf(governor->name, "adaptive");

	/* init mutex */
	err = rt_mutex_init(&governor->cpufreq_adaptive_mutex,
		governor->name, RT_IPC_FLAG_FIFO);
	if (err) {
		LOG_E("init cpufreq adaptive  mutex failed, ret=%d", err);
		err = __LINE__;
		goto out_free;
	}

	governor->sampling_rate_ms = CPUFREQ_ADAPTIVE_SAMPLING_RATE_MS;

	/* init timer for calculate cpu loading */
	rt_timer_init(&governor->cpufreq_adaptive_timer, governor->name,
		cpufreq_adaptive_timer_func, governor,
		governor->sampling_rate_ms / 10,
		RT_TIMER_FLAG_DEACTIVATED
		| RT_TIMER_FLAG_PERIODIC
		| RT_TIMER_FLAG_SOFT_TIMER);

	governor->cur_policy = cpufreq_cpu_get(0);
	if (!governor->cur_policy) {
		LOG_E("can not get current cpufreq policy");
		err = __LINE__;
		goto out_free_timer;
	}

	/* malloc struct cpu_load_record struct */
	governor->records = (struct cpu_load_record *)rt_zalloc(sizeof(struct cpu_load_record));
	if (!governor->records) {
		LOG_E("failed to malloc for cpu_load_record");
		err = __LINE__;
		goto out_free_timer;
	}

	governor->records->index = 0;

	for (i = 0; i < RT_CPUS_NR; i++) {
		governor->pre_cpu_total[i] = get_cpu_total_time(i);
		governor->pre_cpu_idle[i] = get_cpu_idle_time(i);
	}

	rt_timer_start(&governor->cpufreq_adaptive_timer);

	cpufreq_adaptive_inited = 1;

	LOG_I("%s: done", __func__);

	return 0;

out_free_timer:
	rt_timer_detach(&governor->cpufreq_adaptive_timer);
out_free:
	if (governor)
		rt_free(governor);
out:
	return err;
}
/* INIT_APP_EXPORT(cpufreq_adaptive_init); */

void cpufreq_adaptive_exit(void)
{
	rt_timer_stop(&governor->cpufreq_adaptive_timer);

	rt_timer_detach(&governor->cpufreq_adaptive_timer);

	if (governor->records)
		rt_free(governor->records);

	rt_mutex_detach(&governor->cpufreq_adaptive_mutex);

	if (governor)
		rt_free(governor);

	cpufreq_adaptive_inited = 0;

	LOG_I("%s: done", __func__);
}

static void adaptive_help(void){
	rt_kprintf("<help>:\n %s %s %s %s",
		"\tadaptive get\n",
		"\t\tsampling_rate_ms\n",
		"\t\tdebug\n",
		"\t\tinit\n");
	rt_kprintf("%s %s %s",
		"\tadaptive set\n",
		"\t\tsampling_rate_ms <sampling_rate > 10>\n",
		"\t\tdebug <1 or 0>\n");
	rt_kprintf("%s",
		"\tadaptive enable\n");
	rt_kprintf("%s",
		"\tadaptive disable\n");
}

static long adaptive(int argc, char **argv)
{
	struct cpufreq_policy *policy;
	rt_tick_t timer_tick;

	if (argc < 2)
		goto help;

	if ((strcmp(argv[1], "get") != 0)
		&& (strcmp(argv[1], "set") != 0)
		&& (strcmp(argv[1], "enable") != 0)
		&& (strcmp(argv[1], "disable") != 0))
		goto help;

	if (strcmp(argv[1], "get") == 0) {
		if (argc < 3)
			goto help;

		if (strcmp(argv[2], "debug") == 0)
			rt_kprintf("%u\n", governor->load_debug);
		else if (strcmp(argv[2], "init") == 0)
			rt_kprintf("%u\n", cpufreq_adaptive_inited);
		else if (strcmp(argv[2], "sampling_rate_ms") == 0)
			rt_kprintf("%u\n", governor->sampling_rate_ms);
	} else if (strcmp(argv[1], "set") == 0) {
		if (argc < 4)
			goto help;

		if (strcmp(argv[2], "debug") == 0) {
			governor->load_debug = atoi(argv[3]);
		} else if (strcmp(argv[2], "sampling_rate_ms") == 0) {
			if (cpufreq_adaptive_inited) {
				governor->sampling_rate_ms = atoi(argv[3]);
				timer_tick = governor->sampling_rate_ms
						* RT_TICK_PER_SECOND / 1000;

				rt_timer_stop(&governor->cpufreq_adaptive_timer);
				rt_timer_control(&governor->cpufreq_adaptive_timer,
						RT_TIMER_CTRL_SET_TIME, &timer_tick);
				rt_timer_start(&governor->cpufreq_adaptive_timer);
			} else {
				LOG_E("cpufreq adaptive governor is not inited");
				return -1;
			}
		}
	} else if (strcmp(argv[1], "enable") == 0) {
		if (cpufreq_adaptive_inited)
			rt_kprintf("already enabled\n");
		else
			cpufreq_adaptive_init();
	} else if (strcmp(argv[1], "disable") == 0) {
		if (!cpufreq_adaptive_inited) {
			rt_kprintf("already disabled\n");
		} else {
			cpufreq_adaptive_exit();

			policy = cpufreq_cpu_get(0);
			__cpufreq_driver_target(policy,
				policy->max, CPUFREQ_RELATION_L);
		}
	}

	return 0;

help:
	adaptive_help();
	return -1;
}

MSH_CMD_EXPORT(adaptive, cpufreq adaptive governor ops commands);
