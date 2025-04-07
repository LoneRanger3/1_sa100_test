/*
 * cpufreq.h - Standard functionality for the cpufreq API.
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

#ifndef _CPUFREQ_H
#define _CPUFREQ_H

#define CPUFREQ_NAME_LEN 16
#define CPUFREQ_MAX_TABLE_SIZE (16)

#define CPUFREQ_RELATION_L 0  /* lowest frequency at or above target */
#define CPUFREQ_RELATION_H 1  /* highest frequency below or at target */

#define CPUFREQ_ENTRY_INVALID ~0
#define CPUFREQ_TABLE_END     ~1

struct cpufreq_frequency_table {
	unsigned int	index;     /* any */
	unsigned int	frequency; /* kHz - doesn't need to be in ascending
				    * order */
};

struct cpufreq_cpuinfo {
	unsigned int		max_freq;
	unsigned int		min_freq;
};

struct cpufreq_policy {
	unsigned int		cpu;    /* cpu nr of CPU managing this policy */
	struct cpufreq_cpuinfo	cpuinfo;/* see above */

	unsigned int		min;    /* in kHz */
	unsigned int		max;    /* in kHz */
	unsigned int		cur;    /* in kHz, only needed if cpufreq
					 * governors are used */
};

struct cpufreq_driver {
	char			name[CPUFREQ_NAME_LEN];
	/* needed by all drivers */
	int	(*init)		(struct cpufreq_policy *policy);
	int	(*verify)	(struct cpufreq_policy *policy);

	/* define one out of two */
	int	(*setpolicy)	(struct cpufreq_policy *policy);
	int	(*target)	(struct cpufreq_policy *policy,
			unsigned int target_freq,
			unsigned int relation);

	/* should be defined, if possible */
	unsigned int	(*get)	(unsigned int cpu);

	/* optional */
	unsigned int (*getavg)	(struct cpufreq_policy *policy,
			unsigned int cpu);
	int	(*bios_limit)	(int cpu, unsigned int *limit);

	int	(*exit)		(struct cpufreq_policy *policy);
	int	(*suspend)	(struct cpufreq_policy *policy);
	int	(*resume)	(struct cpufreq_policy *policy);
	struct freq_attr	**attr;
	void			*private_data;
};

static inline void cpufreq_verify_within_limits(struct cpufreq_policy *policy,
	unsigned int min, unsigned int max)
{
	if (policy->min < min)
		policy->min = min;
	if (policy->max < min)
		policy->max = min;
	if (policy->min > max)
		policy->min = max;
	if (policy->max > max)
		policy->max = max;
	if (policy->min > policy->max)
		policy->min = policy->max;
	return;
}

int cpufreq_frequency_table_cpuinfo(struct cpufreq_policy *policy,
		struct cpufreq_frequency_table *table);
int cpufreq_frequency_table_target(struct cpufreq_policy *policy,
		struct cpufreq_frequency_table *table,
		unsigned int target_freq,
		unsigned int relation,
		unsigned int *index);
int __cpufreq_driver_target(struct cpufreq_policy *policy,
		unsigned int target_freq,
		unsigned int relation);
int cpufreq_register_driver(struct cpufreq_driver *driver_data);
int cpufreq_frequency_table_verify(struct cpufreq_policy *policy,
		struct cpufreq_frequency_table *table);
void cpufreq_frequency_table_get_attr(struct cpufreq_frequency_table *table);
struct cpufreq_policy *cpufreq_cpu_get(unsigned int cpu);
struct cpufreq_frequency_table *cpufreq_cpu_get_table(unsigned int cpu);
unsigned int __cpufreq_get(unsigned int cpu);
int cpufreq_get_policy(struct cpufreq_policy *policy, unsigned int cpu);
int __cpufreq_set_policy(struct cpufreq_policy *data,
		struct cpufreq_policy *policy);
#ifdef CPUFREQ_GOVERNOR_ADAPTIVE
int cpufreq_adaptive_init(void);
#endif
#endif /* _CPUFREQ_H */
