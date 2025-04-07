/*
 * lombo_cpufreq.c - Driver of LomboTech CPUfreq
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
#include "config_api.h"
#include "cpufreq.h"
#ifdef LOMBO_CPU_ADPLL
#include "cpu_adpll.h"
#else
#include "clk.h"
#endif

#define DBG_LEVEL		DBG_INFO
#include <debug.h>

#define LOMBO_CPUFREQ_MAX         (1200000) /* KHz */
#define LOMBO_CPUFREQ_MIN          (120000) /* KHz */

/**
 * struct lombo_cpufreq - Runtime control data for cpufreq.
 * @clk_pll: pointer to cpu pll clock.
 * @clk_cpu: pointer to cpu clock.
 * @freq_table: pointer to cpufreq frequency table.
 * @transition_latency: the time of switching two frequencies in nanoseconds.
 * @max_freq: max frequency supported, based on KHz.
 * @min_freq: min frequency supported, based on KHz.
 * @debug: debug control.
 */
struct lombo_cpufreq {
#ifndef LOMBO_CPU_ADPLL
	clk_handle_t clk_pll;
	clk_handle_t clk_cpu;
#endif
	struct cpufreq_frequency_table *freq_table;

	u32 max_freq;
	u32 min_freq;
	u32 debug;
};

static struct lombo_cpufreq l_cpufreq;
struct rt_mutex lombo_cpufreq_mutex;

/**
 * lombo_cpufreq_verify - check if the cpu frequency policy is valid.
 * @policy: pointer to cpufreq policy.
 *
 * Return 0 if valid.
 */
static int lombo_cpufreq_verify(struct cpufreq_policy *policy)
{
	return cpufreq_frequency_table_verify(policy, l_cpufreq.freq_table);
}

/**
 * lombo_cpufreq_get - get cpu frequency from hardware.
 * @cpu: cpu number.
 *
 * Return cpu frequency, based on KHz.
 */
static unsigned int lombo_cpufreq_get(unsigned int cpu)
{
#ifndef LOMBO_CPU_ADPLL
	return clk_get_rate(l_cpufreq.clk_cpu) / 1000;
#else
	return csp_get_cpu_rate() / 1000;
#endif
}

/**
 * lombo_cpufreq_set_rate - set target frequency to hardware.
 * @freq: target frequency.
 *
 * Return 0 if success, 0! error.
 */
static int lombo_cpufreq_set_rate(unsigned int freq)
{
	int err = 0;

#ifndef LOMBO_CPU_ADPLL
	if (clk_set_rate(l_cpufreq.clk_pll, freq * 1000)) {
		LOG_E("set cpu pll to %uKHz failed!\n", freq);
		err = -EINVAL;
		goto out;
	}
#else
	if (csp_set_cpu_rate(freq * 1000)) {
		LOG_E("set cpu pll to %uKHz failed!\n", freq);
		err = -EINVAL;
		goto out;
	}
#endif

out:
	return err;
}

/**
 * lombo_cpufreq_target - set target frequency to cpu.
 * @policy: pointer to cpufreq policy.
 * @target_freq: target frequency to be set, based on KHz.
 * @relation: method for selecting the target frequency.
 *
 * Return 0 if success, !0 error.
 */
static int lombo_cpufreq_target(struct cpufreq_policy *policy,
				unsigned int target_freq, unsigned int relation)
{
	int err = 0;
	unsigned int index;
	unsigned int freq = 0;

	rt_mutex_take(&lombo_cpufreq_mutex, RT_WAITING_FOREVER);

	if (target_freq == policy->cur)
		goto out;

	if (l_cpufreq.debug)
		LOG_I("request frequency: %uKHz", target_freq);

	/* try to look for a valid frequency value from cpu frequency table */
	if (cpufreq_frequency_table_target(policy, l_cpufreq.freq_table,
					target_freq, relation, &index)) {
		LOG_E("try to find %uKHz failed!\n", target_freq);
		err = -EINVAL;
		goto out;
	}

	/* target frequency is the same as current, not set */
	if (l_cpufreq.freq_table[index].frequency == policy->cur)
		goto out;

	/* find target frequency in the cpu frequency table */
	freq = l_cpufreq.freq_table[index].frequency;

	if (l_cpufreq.debug)
		LOG_I("target frequency: %uKHz, index:%u", freq, index);

	/* set cpu frequency to hw */
	if (lombo_cpufreq_set_rate(freq)) {
		LOG_E("set cpu frequency to %uKHz failed!\n", freq);
		err = -EINVAL;
		goto out;
	}

	policy->cur = freq;

	if (l_cpufreq.debug)
		LOG_I("set frequency: %uKHz success", lombo_cpufreq_get(0));

out:
	rt_mutex_release(&lombo_cpufreq_mutex);

	return err;
}

/**
 * lombo_cpufreq_init - initialise a policy;
 * @policy:  cpu frequency policy;
 *
 * Return 0 if success, !0 error;
 */
static int lombo_cpufreq_init(struct cpufreq_policy *policy)
{
	int err = 0;

	err = cpufreq_frequency_table_cpuinfo(policy, l_cpufreq.freq_table);
	if (err) {
		LOG_E("init cpufreq table cpuinfo failed\n");
		goto out;
	}

	cpufreq_frequency_table_get_attr(l_cpufreq.freq_table);
	policy->cur = lombo_cpufreq_get(0);

	if (policy->max > l_cpufreq.max_freq)
		policy->max = l_cpufreq.max_freq;

	if (policy->min < l_cpufreq.min_freq)
		policy->min = l_cpufreq.min_freq;

	LOG_D("boot freq: %uKHz", policy->cur);

out:
	return err;
}

static struct cpufreq_driver lombo_cpufreq_driver = {
	.name   = "cpufreq-lombo",
	.init   = lombo_cpufreq_init,
	.verify = lombo_cpufreq_verify,
	.target = lombo_cpufreq_target,
	.get    = lombo_cpufreq_get,
};

/**
 * lombo_cpufreq_get_valid_freq - get a valid frequency from frequency table;
 * target_freq: target frequency to be judge, based on KHz;
 *
 * Return cpu frequency, based on KHz;
 */
static unsigned int lombo_cpufreq_get_valid_freq(unsigned int target_freq)
{
	struct cpufreq_frequency_table *tmp_tbl = l_cpufreq.freq_table;

	while (tmp_tbl->frequency != CPUFREQ_TABLE_END) {
		if ((tmp_tbl + 1)->frequency <= target_freq)
			tmp_tbl++;
		else
			break;
	}

	return tmp_tbl->frequency;
}

/**
 * lombo_cpufreq_range_select - init cpu max/min frequency from config.bin.
 *
 * Return 0 if init cpu max/min success, !0 error.
 */
static int lombo_cpufreq_range_select(void)
{
	int err = 0;

	if (config_get_u32("cpufreq_limit_table", "max_freq", &l_cpufreq.max_freq)) {
		LOG_E("get max freq failed\n");
		goto out_default;
	}

	if (config_get_u32("cpufreq_limit_table", "min_freq", &l_cpufreq.min_freq)) {
		LOG_E("get min freq failed\n");
		goto out_default;
	}

	if (l_cpufreq.min_freq > l_cpufreq.max_freq) {
		LOG_E("min freq(%d) > max freq(%d)\n",
				l_cpufreq.min_freq, l_cpufreq.max_freq);
		goto out_default;
	}

	if (l_cpufreq.max_freq > LOMBO_CPUFREQ_MAX) {
		LOG_E("max freq(%d) > %d\n",
				l_cpufreq.max_freq, LOMBO_CPUFREQ_MAX);
		goto out_default;
	}

	if (l_cpufreq.max_freq < LOMBO_CPUFREQ_MIN) {
		LOG_E("max freq(%d) < %d\n",
				l_cpufreq.max_freq, LOMBO_CPUFREQ_MIN);
		goto out_default;
	}

	if (l_cpufreq.min_freq > LOMBO_CPUFREQ_MAX) {
		LOG_E("min freq(%d) > %d\n",
				l_cpufreq.min_freq, LOMBO_CPUFREQ_MAX);
		goto out_default;
	}

	if (l_cpufreq.min_freq < LOMBO_CPUFREQ_MIN) {
		LOG_E("min freq(%d) < %d\n",
				l_cpufreq.min_freq, LOMBO_CPUFREQ_MIN);
		goto out_default;
	}

	/* get valid max/min frequency */
	l_cpufreq.max_freq = lombo_cpufreq_get_valid_freq(l_cpufreq.max_freq);
	l_cpufreq.min_freq = lombo_cpufreq_get_valid_freq(l_cpufreq.min_freq);

	LOG_D("max: %uKHz, min: %uKHz\n",
				l_cpufreq.max_freq, l_cpufreq.min_freq);

	goto out;

out_default:
	/* use default */
	l_cpufreq.max_freq = LOMBO_CPUFREQ_MAX;
	l_cpufreq.min_freq = LOMBO_CPUFREQ_MIN;
out:
	return err;
}

/**
 * lombo_cpufreq_initcall - init cpufreq driver
 *
 * Return 0 if success, 0! error.
 */
static int lombo_cpufreq_initcall(void)
{
	struct cpufreq_frequency_table *freq_table;
	int err = 0, cnt, ret_cnt, i;
	u32 cpufreq_table[CPUFREQ_MAX_TABLE_SIZE] = {0};
#ifndef LOMBO_CPU_ADPLL
	const char *clk_cpu = NULL;
	const char *clk_pll = NULL;
#endif

	cnt = config_count_elems("cpus", "cpufreq_table");
	if (cnt == -1) {
		LOG_E("cpufreq: get cpufreq table count failed, ret:%d", cnt);
		err = RT_ERROR;
		goto out;
	}

	if (cnt > CPUFREQ_MAX_TABLE_SIZE) {
		LOG_E("cpufreq: cpufreq table count:%d > max count:%d",
				cnt, CPUFREQ_MAX_TABLE_SIZE);
		err = RT_ERROR;
		goto out;
	}

	ret_cnt = config_get_u32_array("cpus", "cpufreq_table", cpufreq_table, cnt);
	if (ret_cnt != cnt) {
		LOG_E("cpufreq: ret_cnt:%d != cnt:%d", ret_cnt, cnt);
		err = RT_ERROR;
		goto out;
	}

	freq_table = rt_zalloc(sizeof(*freq_table) * (cnt + 1));
	if (!freq_table) {
		LOG_E("cpufreq: malloc failed(%s:%d)", __func__, __LINE__);
		err = RT_ENOMEM;
		goto out;
	}

	for (i = 0; i < cnt; i++) {
		freq_table[i].index = i;
		freq_table[i].frequency = cpufreq_table[i];
	}
	freq_table[i].index = i;
	freq_table[i].frequency = CPUFREQ_TABLE_END;
	l_cpufreq.freq_table = freq_table;

	err = lombo_cpufreq_range_select();
	if (err)
		goto out_kfree;

#ifndef LOMBO_CPU_ADPLL
	/* get cpu module clk */
	err = config_get_string("cpus", "cpu_clk", &clk_cpu);
	if (err != 0) {
		LOG_E("unable to get CPU clock");
		goto out_kfree;
	}

	l_cpufreq.clk_cpu = clk_get(clk_cpu);
	if (l_cpufreq.clk_cpu < 0) {
		LOG_E("unable to get CPU clock");
		err = __LINE__;
		goto out_kfree;
	}

	/* get pll cpu clk */
	err = config_get_string("cpus", "cpu_pll", &clk_pll);
	if (err != 0) {
		LOG_E("unable to get PLL CPU clock");
		goto out_clk_cpu_put;
	}

	l_cpufreq.clk_pll = clk_get(clk_pll);
	if (l_cpufreq.clk_pll < 0) {
		LOG_E("unable to get PLL CPU clock");
		err = __LINE__;
		goto out_clk_cpu_put;
	}

	/* set cpu parent clk */
	err = clk_set_parent(l_cpufreq.clk_cpu, l_cpufreq.clk_pll);
	if (err) {
		LOG_E("set cpu parent clk failed");
		goto out_clk_pll_put;
	}

	/* enable cpu clk */
	err = clk_enable(l_cpufreq.clk_cpu);
	if (err) {
		LOG_E("enable cpu clock failed");
		goto out_clk_pll_put;
	}
#endif

	l_cpufreq.debug = 0;

	/* init mutex */
	err = rt_mutex_init(&lombo_cpufreq_mutex, "cpufreq", RT_IPC_FLAG_FIFO);
	if (err) {
		LOG_E("%s init cpufreq mutex failed");
		goto out_clk_pll_put;
	}

	lombo_cpufreq_driver.private_data = &l_cpufreq.debug,
	/* register cpufreq driver to kernel */
	err = cpufreq_register_driver(&lombo_cpufreq_driver);
	if (err) {
		LOG_E("register cpufreq driver failed\n");
		goto out_clk_pll_put;
	}

	LOG_D("cpufreq: driver inited");

	return 0;

out_clk_pll_put:
#ifndef LOMBO_CPU_ADPLL
	clk_put(l_cpufreq.clk_pll);
out_clk_cpu_put:
	clk_put(l_cpufreq.clk_cpu);
#endif
out_kfree:
	rt_free(freq_table);
out:
	return err;
}
INIT_DEVICE_EXPORT(lombo_cpufreq_initcall);
