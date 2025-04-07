/*
 * lombo_thermal.c - Driver of LomboTech Thermal
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
#include "config_api.h"
#include "gpadc_drv.h"
#include "board.h"
#include "cpufreq.h"
#include "lombo_thermal.h"

#include <debug.h>

#define LOMBO_THERMAL_POLLING_MS	(1000)

#if defined(LOMBO_CPUFREQ_COOLING)
struct cpufreq_cooling_state {
	u32 lower;
	u32 upper;
};
#endif

struct thermal_trip {
	int temp;
	char type[16];
};

/**
 * struct lombo_thermal - Runtime control data for thermal.
 * @name: thermal name.
 * @temp: current temperature.
 * @last_temp: last temperature.
 * @emul_temp: emulation temperature.
 * @polling_ms: thermal polling rate.
 * @trip_count: thermal trip point count.
 * @trips: thermal trip point.
 * @last_cooling_trip: last cooling trip point.
 * @last_cooling_idx: last cooling idx.
 * @cooling_state_count: cooling state count.
 * @cooling_state: cooling state point.
 * @debug: debug control.
 * @lombo_thermal_timer: thermal timer.
 * @lombo_thermal_mutex: thermal mutex.
 */
struct lombo_thermal {
	char name[16];
	int temp;
	int last_temp;
	int emul_temp;
	u32 polling_ms;
	u32 trip_count;
	struct thermal_trip *trips;
#if defined(LOMBO_CPUFREQ_COOLING)
	int last_cooling_trip;
	int last_cooling_idx;
	u32 cooling_state_count;
	struct cpufreq_cooling_state *cooling_state;
#endif
	u32 debug;
	struct rt_timer lombo_thermal_timer;
	struct rt_mutex lombo_thermal_mutex;
};

static struct lombo_thermal *l_thermal;
static u32 lombo_thermal_inited;

#if defined(LOMBO_CPUFREQ_COOLING)
struct cpufreq_frequency_table *freq_table;
static u32 freq_table_cnt;

static void cpufreq_table_init(void)
{
	int i;

	freq_table = cpufreq_cpu_get_table(0);
	for (i = 0; (freq_table[i].frequency != CPUFREQ_TABLE_END);)
		i++;

	freq_table_cnt = i;
}

static u32 get_freq_by_idx(u32 idx)
{
	u32 item = freq_table_cnt - 1 - idx;
	u32 freq;

	freq = freq_table[item].frequency;

	if (l_thermal->debug)
		LOG_I("idx=%u, freq=%u", idx, freq);

	return freq;
}

static int get_cooling_trip(struct lombo_thermal *thermal, int temp)
{
	int i = -1, cooling_trip = -1;
	struct thermal_trip *trips = thermal->trips;

	while (temp >= trips[i+1].temp) {
		i++;
		if (i == thermal->trip_count - 1)
			break;
	}

	cooling_trip = i;

	return cooling_trip;
}

static int cpufreq_cooling_target(int start_cooling_idx, int end_cooling_idx)
{
	int i, err = 0;
	u32 max_freq = 0;
	struct cpufreq_policy new_policy;
	struct cpufreq_policy *policy = cpufreq_cpu_get(0);

	err = cpufreq_get_policy(&new_policy, 0);
	if (err) {
		LOG_E("cpufreq_cooling: get policy fail, ret=%d", err);
		goto out;
	}

	if (start_cooling_idx < end_cooling_idx) {
		for (i = start_cooling_idx; i <= end_cooling_idx; i++) {
			max_freq = get_freq_by_idx(i);
			new_policy.max = max_freq;

			err = __cpufreq_set_policy(policy, &new_policy);
			if (err) {
				LOG_E("cpufreq_cooling: set policy fail, ret=%d", err);
				goto out;
			}
		}
	} else if (start_cooling_idx > end_cooling_idx) {
		for (i = start_cooling_idx; i >= end_cooling_idx; i--) {
			max_freq = get_freq_by_idx(i);
			new_policy.max = max_freq;

			err = __cpufreq_set_policy(policy, &new_policy);
			if (err) {
				LOG_E("cpufreq_cooling: set policy fail, ret=%d", err);
				goto out;
			}
		}
	}

out:
	return err;
}
#endif

static void do_check_thermal(struct lombo_thermal *thermal)
{
	int temp = 0;
#if defined(LOMBO_CPUFREQ_COOLING)
	int target_cooling_trip = 0;
	u32 target_cooling_idx = 0;
	int i = 0;
#endif

	if (thermal->emul_temp != 0) {
		temp = thermal->emul_temp;
	} else {
		temp = temp_sensor_get_val() * 1000;
		thermal->temp = temp;
	}

	if (thermal->debug)
		LOG_I("current temperature: %u", temp);

	/* more than max trip point, need shutdown */
	if (temp >= thermal->trips[thermal->trip_count-1].temp) {
		LOG_E("Temperature: %uC, Shutdown ...", temp / 1000);
		sys_shutdown();
		return;
	}

#if defined(LOMBO_CPUFREQ_COOLING)
	target_cooling_trip = get_cooling_trip(thermal, temp);
	if ((target_cooling_trip < -1)
		|| (target_cooling_trip >= (int)(thermal->trip_count - 1))) {
		LOG_E("target_cooling_trip:%d, invalid", target_cooling_trip);
		return;
	}

	if (thermal->debug)
		LOG_I("target cooling trip:%d", target_cooling_trip);

	if (target_cooling_trip > thermal->last_cooling_trip) {
		target_cooling_idx = thermal->cooling_state[target_cooling_trip].lower;
	} else if (target_cooling_trip < thermal->last_cooling_trip) {
		if (target_cooling_trip == -1)
			target_cooling_idx = 0;
		else
			target_cooling_idx = thermal->cooling_state[target_cooling_trip].upper;
	} else {
		if (target_cooling_trip == -1) {
			target_cooling_idx = 0;
		} else {
			if (temp > thermal->last_temp) {
				i = thermal->last_cooling_idx;
				i++;
				if (i <= thermal->cooling_state[target_cooling_trip].upper)
					target_cooling_idx = i;
				else
					target_cooling_idx = thermal->cooling_state[target_cooling_trip].upper;

			} else if (temp < thermal->last_temp) {
				i = thermal->last_cooling_idx;
				i--;
				if (i >= thermal->cooling_state[target_cooling_trip].lower)
					target_cooling_idx = i;
				else
					target_cooling_idx = thermal->cooling_state[target_cooling_trip].lower;
			} else {
				target_cooling_idx = thermal->last_cooling_idx;
			}
		}
	}

	if (thermal->debug)
		LOG_I("target cooling idx:%d", target_cooling_idx);

	cpufreq_cooling_target(thermal->last_cooling_idx, target_cooling_idx);

	thermal->last_cooling_trip = target_cooling_trip;
	thermal->last_cooling_idx = target_cooling_idx;
#endif

	thermal->last_temp = temp;
}

/**
 * lombo_thermal_timer_func - get current temperature for thermal check
 * @param: pointer to lombo_thermal struct
 */
static void lombo_thermal_timer_func(void *param)
{
	struct lombo_thermal *thermal = (struct lombo_thermal *)param;

	rt_mutex_take(&thermal->lombo_thermal_mutex, RT_WAITING_FOREVER);

	do_check_thermal(thermal);

	rt_mutex_release(&thermal->lombo_thermal_mutex);
}

/**
 * lombo_thermal_init - init thermal driver
 *
 * Return 0 if success, 0! error.
 */
int lombo_thermal_init(void)
{
	int i, err = 0;
	struct thermal_trip *trips;
#if defined(LOMBO_CPUFREQ_COOLING)
	struct cpufreq_cooling_state *cooling_state;
	u32 tmp_upper = 0;
	u32 tmp_lower = 0;
#endif
	u32 tmp_temp = 0;
	char tmp_name[32];
	const char *tmp_type = NULL;

	l_thermal = rt_zalloc(sizeof(struct lombo_thermal));
	if (!l_thermal) {
		LOG_E("thermal: malloc failed(%s:%d)", __func__, __LINE__);
		err = RT_ENOMEM;
		goto out;
	}

	rt_sprintf(l_thermal->name, "lombo_thermal");

	/* init mutex */
	err = rt_mutex_init(&l_thermal->lombo_thermal_mutex,
		l_thermal->name, RT_IPC_FLAG_FIFO);
	if (err) {
		LOG_E("init thermal mutex failed, ret=%d", err);
		goto out_free_thermal;
	}

	err = config_get_u32("thermal", "num_trips", &l_thermal->trip_count);
	if (err) {
		LOG_E("thermal: get thermal num_trips failed");
		goto out_free_timer;
	}

	trips = rt_zalloc(sizeof(struct thermal_trip) * l_thermal->trip_count);
	if (!trips) {
		LOG_E("thermal: malloc failed(%s:%d)", __func__, __LINE__);
		err = RT_ENOMEM;
		goto out_free_timer;
	}

	for (i = 0; i < l_thermal->trip_count; i++) {
		sprintf(tmp_name, "trip%d_temp", i);

		err = config_get_u32("thermal", tmp_name, &tmp_temp);
		if (err != 0) {
			LOG_E("unable to get %s", tmp_name);
			goto out_free_trip;
		}
		trips[i].temp = tmp_temp;

		sprintf(tmp_name, "trip%d_type", i);
		err = config_get_string("thermal", tmp_name, &tmp_type);
		if (err != 0) {
			LOG_E("unable to get %s", tmp_name);
			goto out_free_trip;
		}
		sprintf(trips[i].type, tmp_type);
	}

	l_thermal->trips = trips;
	l_thermal->temp = temp_sensor_get_val();
	l_thermal->last_temp = l_thermal->temp;
	l_thermal->polling_ms = LOMBO_THERMAL_POLLING_MS;
	l_thermal->debug = 0;

	if (l_thermal->debug) {
		LOG_I("trip_count=%u", l_thermal->trip_count);
		for (i = 0; i < l_thermal->trip_count; i++) {
			LOG_I("trip[%d]_temp=%d", i, l_thermal->trips[i].temp);
			LOG_I("trip[%d]_type=%s", i, l_thermal->trips[i].type);
		}
		LOG_I("temp=%d", l_thermal->temp);
		LOG_I("last_temp=%d", l_thermal->last_temp);
		LOG_I("emul_temp=%d", l_thermal->emul_temp);
	}

#if defined(LOMBO_CPUFREQ_COOLING)
	l_thermal->cooling_state_count = l_thermal->trip_count - 1;
	cooling_state = rt_zalloc(sizeof(struct cpufreq_cooling_state)
				* l_thermal->cooling_state_count);
	if (!cooling_state) {
		LOG_E("thermal: malloc failed(%s:%d)", __func__, __LINE__);
		err = RT_ENOMEM;
		goto out_free_trip;
	}

	for (i = 0; i < l_thermal->cooling_state_count; i++) {
		sprintf(tmp_name, "trip%d_lower", i);

		err = config_get_u32("cpufreq-cooling", tmp_name, &tmp_lower);
		if (err != 0) {
			LOG_E("unable to get %s", tmp_name);
			goto out_free_trip;
		}
		cooling_state[i].lower = tmp_lower;

		sprintf(tmp_name, "trip%d_upper", i);

		err = config_get_u32("cpufreq-cooling", tmp_name, &tmp_upper);
		if (err != 0) {
			LOG_E("unable to get %s", tmp_name);
			goto out_free_trip;
		}
		cooling_state[i].upper = tmp_upper;
	}

	l_thermal->cooling_state = cooling_state;

	if (l_thermal->debug) {
		LOG_I("cooling_state_count=%u", l_thermal->cooling_state_count);
		for (i = 0; i < l_thermal->cooling_state_count; i++) {
			LOG_I("trip[%d]_lower=%u", i, l_thermal->cooling_state[i].lower);
			LOG_I("trip[%d]_upper=%u", i, l_thermal->cooling_state[i].upper);
		}
	}

	l_thermal->last_cooling_trip = get_cooling_trip(l_thermal, l_thermal->temp);
	l_thermal->last_cooling_idx = 0;

	if (l_thermal->debug)
		LOG_I("last_cooling_trip=%d, last_cooling_idx=%d",
			l_thermal->last_cooling_trip,
			l_thermal->last_cooling_idx);

	cpufreq_table_init();
#endif

	/* init timer for thermal */
	rt_timer_init(&l_thermal->lombo_thermal_timer, l_thermal->name,
		lombo_thermal_timer_func, l_thermal,
		l_thermal->polling_ms / 10,
		RT_TIMER_FLAG_DEACTIVATED
		| RT_TIMER_FLAG_PERIODIC
		| RT_TIMER_FLAG_SOFT_TIMER);

	rt_timer_start(&l_thermal->lombo_thermal_timer);

	lombo_thermal_inited = 1;

	LOG_I("thermal: driver inited");

	return 0;

out_free_trip:
	rt_free(l_thermal->trips);
out_free_timer:
	rt_timer_detach(&l_thermal->lombo_thermal_timer);
out_free_thermal:
	rt_free(l_thermal);
out:
	return err;
}
/* INIT_DEVICE_EXPORT(lombo_thermal_init); */

void lombo_thermal_exit(void)
{
	rt_timer_stop(&l_thermal->lombo_thermal_timer);

	rt_timer_detach(&l_thermal->lombo_thermal_timer);

#if defined(LOMBO_CPUFREQ_COOLING)
	if (l_thermal->cooling_state)
		rt_free(l_thermal->cooling_state);
#endif

	if (l_thermal->trips)
		rt_free(l_thermal->trips);

	rt_mutex_detach(&l_thermal->lombo_thermal_mutex);

	if (l_thermal)
		rt_free(l_thermal);

	lombo_thermal_inited = 0;

	LOG_I("%s: done", __func__);
}

static void thermal_help(void){
	rt_kprintf("<help>:\n %s %s %s %s %s",
		"\tthermal get\n",
		"\t\tpolling_ms\n",
		"\t\ttemp\n",
		"\t\temul_temp\n",
		"\t\tdebug\n");
	rt_kprintf("%s %s %s %s",
		"\tthermal set\n",
		"\t\tpolling_ms <polling rate ms > 10>\n",
		"\t\temul_temp <emulation temperature, 0 ~ 150000>\n",
		"\t\tdebug <0 or 1>\n");
	rt_kprintf("%s", "\tthermal enable\n");
	rt_kprintf("%s", "\tthermal disable\n");
	rt_kprintf("%s", "\tthermal trips\n");
}

static long thermal(int argc, char **argv)
{
	struct cpufreq_policy *policy;
	rt_tick_t timer_tick;
	int i = 0;

	if (argc < 2)
		goto help;

	if ((strcmp(argv[1], "get") != 0)
		&& (strcmp(argv[1], "set") != 0)
		&& (strcmp(argv[1], "enable") != 0)
		&& (strcmp(argv[1], "disable") != 0)
		&& (strcmp(argv[1], "trips") != 0))
		goto help;

	if (strcmp(argv[1], "get") == 0) {
		if (argc < 3)
			goto help;

		if (lombo_thermal_inited == 0) {
			rt_kprintf("thermal is not enabled\n");
			goto help;
		}

		if (strcmp(argv[2], "polling_ms") == 0)
			rt_kprintf("%u\n", l_thermal->polling_ms);
		else if (strcmp(argv[2], "temp") == 0)
			rt_kprintf("%d\n", temp_sensor_get_val() * 1000);
		else if (strcmp(argv[2], "emul_temp") == 0)
			rt_kprintf("%d\n", l_thermal->emul_temp);
		else if (strcmp(argv[2], "debug") == 0)
			rt_kprintf("%u\n", l_thermal->debug);
	} else if (strcmp(argv[1], "set") == 0) {
		if (argc < 4)
			goto help;

		if (lombo_thermal_inited == 0) {
			rt_kprintf("thermal is not enabled\n");
			goto help;
		}

		if (strcmp(argv[2], "polling_ms") == 0) {
			l_thermal->polling_ms = atoi(argv[3]);
			timer_tick = l_thermal->polling_ms * RT_TICK_PER_SECOND / 1000;

			rt_timer_stop(&l_thermal->lombo_thermal_timer);
			rt_timer_control(&l_thermal->lombo_thermal_timer,
					RT_TIMER_CTRL_SET_TIME, &timer_tick);
			rt_timer_start(&l_thermal->lombo_thermal_timer);
		} else if (strcmp(argv[2], "emul_temp") == 0) {
			if (atoi(argv[3]) > 150000)
				goto help;

			rt_timer_stop(&l_thermal->lombo_thermal_timer);

			l_thermal->emul_temp = atoi(argv[3]);
			rt_mutex_take(&l_thermal->lombo_thermal_mutex,
						RT_WAITING_FOREVER);
			do_check_thermal(l_thermal);
			rt_mutex_release(&l_thermal->lombo_thermal_mutex);

			rt_timer_start(&l_thermal->lombo_thermal_timer);
		} else if (strcmp(argv[2], "debug") == 0) {
			l_thermal->debug = atoi(argv[3]);
		}
	} else if (strcmp(argv[1], "enable") == 0) {
		if (lombo_thermal_inited)
			rt_kprintf("already enabled\n");
		else
			lombo_thermal_init();
	} else if (strcmp(argv[1], "disable") == 0) {
		if (!lombo_thermal_inited) {
			rt_kprintf("already disabled\n");
		} else {
			lombo_thermal_exit();

			policy = cpufreq_cpu_get(0);
			__cpufreq_driver_target(policy,
				policy->max, CPUFREQ_RELATION_L);
		}
	} else if (strcmp(argv[1], "trips") == 0) {
		if (lombo_thermal_inited) {
			rt_kprintf("trip \ttemp \ttype\n");
			for (i = 0; i < l_thermal->trip_count; i++)
				rt_kprintf("%d \t%d \t%s\n", i,
					l_thermal->trips[i].temp,
					l_thermal->trips[i].type);
		} else {
			rt_kprintf("thermal is not enabled\n");
			goto help;
		}
	}

	return 0;

help:
	thermal_help();
	return -1;
}

MSH_CMD_EXPORT(thermal, thermal ops commands);

#if defined(LOMBO_CPUFREQ_COOLING)
static void cpufreq_cooling_help(void){
	rt_kprintf("<help>:\n %s %s",
		"\tcpufreq_cooling get\n",
		"\t\temul_temp\n");
	rt_kprintf("%s", "\tcpufreq_cooling trips\n");
}

static long cpufreq_cooling(int argc, char **argv)
{
	u32 lower = 0;
	u32 upper = 0;
	int i = 0;

	if (argc < 2)
		goto help;

	if ((strcmp(argv[1], "get") != 0) && (strcmp(argv[1], "trips") != 0))
		goto help;

	if (strcmp(argv[1], "get") == 0) {
		if (argc < 3)
			goto help;

		if (strcmp(argv[2], "polling_ms") == 0)
			rt_kprintf("%u\n", l_thermal->polling_ms);
	} else if (strcmp(argv[1], "trips") == 0) {
		if (lombo_thermal_inited) {
			rt_kprintf("trip \ttemp \t\tlower \t\tupper\n");
			for (i = 0; i < l_thermal->cooling_state_count; i++) {
				lower = l_thermal->cooling_state[i].lower;
				upper = l_thermal->cooling_state[i].upper;
				rt_kprintf("%d \t%d \t\t%u[%u] \t%u[%u]\n", i,
					l_thermal->trips[i].temp,
					lower, get_freq_by_idx(lower),
					upper, get_freq_by_idx(upper));
			}
		} else {
			rt_kprintf("thermal is not enabled\n");
			goto help;
		}
	}

	return 0;

help:
	cpufreq_cooling_help();
	return -1;
}

MSH_CMD_EXPORT(cpufreq_cooling, cpufreq cooling ops commands);
#endif
