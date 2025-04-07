/*
 * cpufreq.c - CPUfreq common
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
#include <stdlib.h>
#include <string.h>
#include "cpufreq.h"

#define DBG_LEVEL		DBG_INFO
#include <debug.h>

static struct cpufreq_policy *g_policy;
static struct cpufreq_driver *g_cpufreq_driver;
static struct cpufreq_frequency_table *g_freq_table;
static u32 cpu_set_freq;

int cpufreq_frequency_table_cpuinfo(struct cpufreq_policy *policy,
		struct cpufreq_frequency_table *table)
{
	unsigned int min_freq = ~0;
	unsigned int max_freq = 0;
	unsigned int i;

	for (i = 0; (table[i].frequency != CPUFREQ_TABLE_END); i++) {
		unsigned int freq = table[i].frequency;

		if (freq == CPUFREQ_ENTRY_INVALID) {
			LOG_D("table entry %u is invalid, skipping", i);
			continue;
		}

		LOG_D("table entry %u: %u kHz, %u index",
				i, freq, table[i].index);
		if (freq < min_freq)
			min_freq = freq;

		if (freq > max_freq)
			max_freq = freq;
	}

	policy->min = policy->cpuinfo.min_freq = min_freq;
	policy->max = policy->cpuinfo.max_freq = max_freq;

	if (policy->min == ~0)
		return -EINVAL;
	else
		return 0;
}

int cpufreq_frequency_table_target(struct cpufreq_policy *policy,
		struct cpufreq_frequency_table *table,
		unsigned int target_freq,
		unsigned int relation,
		unsigned int *index)
{
	struct cpufreq_frequency_table optimal = {
		.index = ~0,
		.frequency = 0,
	};
	struct cpufreq_frequency_table suboptimal = {
		.index = ~0,
		.frequency = 0,
	};
	unsigned int i;

	LOG_D("request for target %u kHz (relation: %u) for cpu %u",
			target_freq, relation, policy->cpu);

	switch (relation) {
	case CPUFREQ_RELATION_H:
		suboptimal.frequency = ~0;
		break;
	case CPUFREQ_RELATION_L:
		optimal.frequency = ~0;
		break;
	}

	for (i = 0; (table[i].frequency != CPUFREQ_TABLE_END); i++) {
		unsigned int freq = table[i].frequency;
		if (freq == CPUFREQ_ENTRY_INVALID)
			continue;
		if ((freq < policy->min) || (freq > policy->max))
			continue;
		switch (relation) {
		case CPUFREQ_RELATION_H:
			if (freq <= target_freq) {
				if (freq >= optimal.frequency) {
					optimal.frequency = freq;
					optimal.index = i;
				}
			} else {
				if (freq <= suboptimal.frequency) {
					suboptimal.frequency = freq;
					suboptimal.index = i;
				}
			}
			break;
		case CPUFREQ_RELATION_L:
			if (freq >= target_freq) {
				if (freq <= optimal.frequency) {
					optimal.frequency = freq;
					optimal.index = i;
				}
			} else {
				if (freq >= suboptimal.frequency) {
					suboptimal.frequency = freq;
					suboptimal.index = i;
				}
			}
			break;
		}
	}
	if (optimal.index > i) {
		if (suboptimal.index > i)
			return -EINVAL;
		*index = suboptimal.index;
	} else
		*index = optimal.index;

	LOG_D("target is %u (%u kHz, %u)", *index, table[*index].frequency,
			table[*index].index);

	return 0;
}

int __cpufreq_driver_target(struct cpufreq_policy *policy,
		unsigned int target_freq,
		unsigned int relation)
{
	int retval = -EINVAL;

	/* Make sure that target_freq is within supported range */
	if (target_freq > policy->max)
		target_freq = policy->max;
	if (target_freq < policy->min)
		target_freq = policy->min;

	LOG_D("target for CPU %u: %u kHz, relation %u", policy->cpu,
		target_freq, relation);

	if (target_freq == policy->cur)
		return 0;

	if (g_cpufreq_driver->target) {
		retval = g_cpufreq_driver->target(policy, target_freq, relation);
		if (retval == 0)
			cpu_set_freq = policy->cur;
	}

	return retval;
}

int cpufreq_frequency_table_verify(struct cpufreq_policy *policy,
		struct cpufreq_frequency_table *table)
{
	unsigned int next_larger = ~0;
	unsigned int i;
	unsigned int count = 0;

	LOG_D("request for verification of policy (%u - %u kHz) for cpu %u",
			policy->min, policy->max, policy->cpu);

	cpufreq_verify_within_limits(policy, policy->cpuinfo.min_freq,
			policy->cpuinfo.max_freq);

	for (i = 0; (table[i].frequency != CPUFREQ_TABLE_END); i++) {
		unsigned int freq = table[i].frequency;
		if (freq == CPUFREQ_ENTRY_INVALID)
			continue;
		if ((freq >= policy->min) && (freq <= policy->max))
			count++;
		else if ((next_larger > freq) && (freq > policy->max))
			next_larger = freq;
	}

	if (!count)
		policy->max = next_larger;

	cpufreq_verify_within_limits(policy, policy->cpuinfo.min_freq,
			policy->cpuinfo.max_freq);

	LOG_D("verification lead to (%u - %u kHz) for cpu %u",
			policy->min, policy->max, policy->cpu);

	return 0;
}

void cpufreq_frequency_table_get_attr(struct cpufreq_frequency_table *table)
{
	g_freq_table = table;
}

unsigned int __cpufreq_get(unsigned int cpu)
{
	unsigned int ret_freq = 0;

	if (!g_cpufreq_driver->get)
		return ret_freq;

	ret_freq = g_cpufreq_driver->get(cpu);

	return ret_freq;
}

/**
 * cpufreq_register_driver - register a CPU Frequency driver
 * @driver_data: A struct cpufreq_driver containing the values#
 * submitted by the CPU Frequency driver.
 *
 *   Registers a CPU Frequency driver to this core code. This code
 * returns zero on success, -EBUSY when another driver got here first
 * (and isn't unregistered in the meantime).
 *
 */
int cpufreq_register_driver(struct cpufreq_driver *driver_data)
{
	int err = 0;

	if (!driver_data || !driver_data->verify || !driver_data->init ||
			((!driver_data->setpolicy) && (!driver_data->target)))
		return RT_EINVAL;

	LOG_D("trying to register driver %s", driver_data->name);

	g_cpufreq_driver = driver_data;

	g_policy = rt_zalloc(sizeof(struct cpufreq_policy));
	if (!g_policy) {
		LOG_E("cpufreq: malloc failed(%s:%d)", __func__, __LINE__);
		return RT_ENOMEM;
	}

	g_policy->cpu = 0;
	err = g_cpufreq_driver->init(g_policy);
	if (err) {
		LOG_E("cpufreq: cpufreq_driver->init() failed, ret=%d", err);
		rt_free(g_policy);
		return RT_ERROR;
	}

	cpu_set_freq = g_policy->cur;

	LOG_D("driver %s up and running", driver_data->name);

	return RT_EOK;
}

/**
 * cpufreq_get_policy - get the current cpufreq_policy
 * @policy: struct cpufreq_policy into which the current cpufreq_policy
 *	is written
 *
 * Reads the current cpufreq policy.
 */
int cpufreq_get_policy(struct cpufreq_policy *policy, unsigned int cpu)
{
	struct cpufreq_policy *cpu_policy;

	if (!policy)
		return RT_ERROR;

	cpu_policy = g_policy;
	if (!cpu_policy)
		return RT_ERROR;

	memcpy(policy, cpu_policy, sizeof(struct cpufreq_policy));

	return 0;
}

struct cpufreq_policy *cpufreq_cpu_get(unsigned int cpu)
{
	return g_policy;
}

struct cpufreq_frequency_table *cpufreq_cpu_get_table(unsigned int cpu)
{
	return g_freq_table;
}

/*
 * data   : current policy.
 * policy : policy to be set.
 */
int __cpufreq_set_policy(struct cpufreq_policy *data,
		struct cpufreq_policy *policy)
{
	int ret = 0;

	LOG_D("setting new policy for CPU %u: %u - %u kHz", policy->cpu,
			policy->min, policy->max);

	memcpy(&policy->cpuinfo, &data->cpuinfo,
			sizeof(struct cpufreq_cpuinfo));

	if (policy->min > data->max || policy->max < data->min) {
		ret = RT_ERROR;
		goto error_out;
	}

	/* verify the cpu speed can be set within this limit */
	ret = g_cpufreq_driver->verify(policy);
	if (ret)
		goto error_out;

	data->min = policy->min;
	data->max = policy->max;

	LOG_D("new min and max freqs are %u - %u kHz", data->min, data->max);

	LOG_D("limit event for cpu %u: %u - %u kHz, currently %u kHz, last set to %u kHz",
			policy->cpu, policy->min, policy->max,
			policy->cur, cpu_set_freq);
	if (policy->max < cpu_set_freq)
		ret = __cpufreq_driver_target(policy, policy->max, CPUFREQ_RELATION_H);
	else if (policy->min > cpu_set_freq)
		ret = __cpufreq_driver_target(policy, policy->min, CPUFREQ_RELATION_L);
	else
		ret = __cpufreq_driver_target(policy, cpu_set_freq, CPUFREQ_RELATION_L);

	if (ret == 0)
		data->cur = policy->cur;

error_out:
	return ret;
}

static void cpufreq_help(void){
	rt_kprintf("<help>:\n %s %s %s %s %s %s %s %s %s",
		"\tcpufreq get\n",
		"\t\tscaling_available_frequencies\n",
		"\t\tscaling_cur_freq\n",
		"\t\tscaling_max_freq\n",
		"\t\tscaling_min_freq\n",
		"\t\tcpuinfo_cur_freq\n",
		"\t\tcpuinfo_max_freq\n",
		"\t\tcpuinfo_min_freq\n",
		"\t\tdebug\n");
	rt_kprintf("%s %s %s %s %s",
		"\tcpufreq set\n",
		"\t\tscaling_setspeed <target_freq>\n",
		"\t\tscaling_max_freq <max_freq>\n",
		"\t\tscaling_min_freq <min_freq>\n",
		"\t\tdebug <0 or 1>\n");
	rt_kprintf("%s",
		"\tcpufreq list\n");
}

static long cpufreq(int argc, char **argv)
{
	unsigned int i = 0;
	unsigned int target_freq = 0;
	unsigned int max_freq = 0;
	unsigned int min_freq = 0;
	struct cpufreq_frequency_table *table = g_freq_table;
	struct cpufreq_policy new_policy;
	int err = 0;

	if (argc < 2)
		goto help;

	if ((strcmp(argv[1], "get") != 0)
		&& (strcmp(argv[1], "set") != 0)
		&& (strcmp(argv[1], "list") != 0))
		goto help;

	if (strcmp(argv[1], "get") == 0) {
		if (strcmp(argv[2], "scaling_cur_freq") == 0) {
			rt_kprintf("%u\n", g_policy->cur);
		} else if (strcmp(argv[2], "scaling_min_freq") == 0) {
			rt_kprintf("%u\n", g_policy->min);
		} else if (strcmp(argv[2], "scaling_max_freq") == 0) {
			rt_kprintf("%u\n", g_policy->max);
		} else if (strcmp(argv[2], "cpuinfo_cur_freq") == 0) {
			rt_kprintf("%u\n", __cpufreq_get(g_policy->cpu));
		} else if (strcmp(argv[2], "cpuinfo_min_freq") == 0) {
			rt_kprintf("%u\n", g_policy->cpuinfo.min_freq);
		} else if (strcmp(argv[2], "cpuinfo_max_freq") == 0) {
			rt_kprintf("%u\n", g_policy->cpuinfo.max_freq);
		} else if (strcmp(argv[2], "scaling_available_frequencies") == 0) {
			for (i = 0; (table[i].frequency != CPUFREQ_TABLE_END); i++) {
				if (table[i].frequency == CPUFREQ_ENTRY_INVALID)
					continue;
				rt_kprintf("%d ", table[i].frequency);
			}

			rt_kprintf("\n");
		} else if (strcmp(argv[2], "debug") == 0) {
			rt_kprintf("%u\n", *(u32 *)g_cpufreq_driver->private_data);
		}
	} else if (strcmp(argv[1], "set") == 0) {
		if (strcmp(argv[2], "scaling_setspeed") == 0) {
			if (argc < 4)
				goto help;
			target_freq = atoi(argv[3]);
			err = __cpufreq_driver_target(g_policy, target_freq,
						CPUFREQ_RELATION_L);
		} else if (strcmp(argv[2], "scaling_max_freq") == 0) {
			if (argc < 4)
				goto help;

			max_freq = atoi(argv[3]);

			err = cpufreq_get_policy(&new_policy, g_policy->cpu);
			if (err)
				goto out;

			new_policy.max = max_freq;

			err = __cpufreq_set_policy(g_policy, &new_policy);
			if (err)
				goto out;
		} else if (strcmp(argv[2], "scaling_min_freq") == 0) {
			if (argc < 4)
				goto help;

			min_freq = atoi(argv[3]);

			err = cpufreq_get_policy(&new_policy, g_policy->cpu);
			if (err)
				goto out;

			new_policy.min = min_freq;

			err = __cpufreq_set_policy(g_policy, &new_policy);
			if (err)
				goto out;
		} else if (strcmp(argv[2], "debug") == 0) {
			if (argc < 4)
				goto help;

			*(u32 *)g_cpufreq_driver->private_data = atoi(argv[3]);
		}
	} else if (strcmp(argv[1], "list") == 0) {
		rt_kprintf("scaling_cur_freq: %u\n", g_policy->cur);
		rt_kprintf("scaling_min_freq: %u\n", g_policy->min);
		rt_kprintf("scaling_max_freq: %u\n", g_policy->max);
		rt_kprintf("cpuinfo_cur_freq: %u\n", __cpufreq_get(g_policy->cpu));
		rt_kprintf("cpuinfo_min_freq: %u\n", g_policy->cpuinfo.min_freq);
		rt_kprintf("cpuinfo_max_freq: %u\n", g_policy->cpuinfo.max_freq);
		rt_kprintf("scaling_available_frequencies: ");
		for (i = 0; (table[i].frequency != CPUFREQ_TABLE_END); i++) {
			if (table[i].frequency == CPUFREQ_ENTRY_INVALID)
				continue;
			rt_kprintf("%d ", table[i].frequency);
		}
		rt_kprintf("\n");
		rt_kprintf("debug: %u\n", *(u32 *)g_cpufreq_driver->private_data);
	}

	return 0;

help:
	cpufreq_help();
out:
	return -1;
}

MSH_CMD_EXPORT(cpufreq, cpufreq ops commands);

