/*
 * cpufreq_test.c - Cpufreq test module driver code for LomboTech
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

#include <debug.h>
#include <string.h>
#include <stdlib.h>
#include "cpufreq.h"

int cpufreq_test_random(int count)
{
	int i, err = 0;
	u32 cur_freq = 0;
	u32 target_freq = 0;
	struct cpufreq_policy *cpu_policy = cpufreq_cpu_get(0);
	struct cpufreq_frequency_table *table = cpufreq_cpu_get_table(0);
	int table_size = 0;
	int idx = 0;

	LOG_I("%s: start", __func__);
	rt_kprintf("available frequencies: ");
	for (i = 0; (table[i].frequency != CPUFREQ_TABLE_END); i++) {
		if (table[i].frequency == CPUFREQ_ENTRY_INVALID)
			continue;
		rt_kprintf("%d ", table[i].frequency);
	}
	rt_kprintf("\n");

	table_size = i;
	if (table_size > CPUFREQ_MAX_TABLE_SIZE) {
		LOG_E("table_size:%d > %d, failed!", table_size, CPUFREQ_MAX_TABLE_SIZE);
		err = __LINE__;
		goto out;
	}

	for (i = 0; i < count; i++) {
		idx = rand() % table_size;
		target_freq = table[idx].frequency;
		err = __cpufreq_driver_target(cpu_policy, target_freq,
				CPUFREQ_RELATION_L);
		if (err) {
			LOG_E("count:%d __cpufreq_driver_target(), failed!", count);
			break;
		}

		cur_freq = __cpufreq_get(0);
		if (cur_freq != target_freq) {
			LOG_E("count:%d cur_freq:%u != target_freq:%u, failed!",
				count, cur_freq, target_freq);
			err = __LINE__;
			break;
		}

		rt_thread_delay(5); /* 50ms */
	}

	LOG_I("%s: end", __func__);
out:
	return err;
}

static void test_cpufreq_help(void){
	rt_kprintf("<help>:\n %s %s",
		"\tlombo_test cpufreq\n",
		"\t\trandom <count>\n");
}

long test_cpufreq(int argc, char **argv)
{
	char *fun_name = NULL;
	int test_count = 0;

	LOG_I("test_cpufreq build time: %s %s", __DATE__, __TIME__);

#if 0
	LOG_I("argc[%d]", argc);
	for (u8 i = 0; i < argc; i++)
		LOG_I("argc[%d][%s]", i, argv[i]);
#endif

	if (argc != 4) {
		test_cpufreq_help();
		return 0;
	}

	fun_name = argv[2];
	test_count = atoi(argv[3]);

	if (fun_name != NULL) {
		if (strcmp(fun_name, "random") == 0) {
			cpufreq_test_random(test_count);
			return 0;
		} else {
			LOG_E("unspported cmd");
		}
	}

	return 0;
}

