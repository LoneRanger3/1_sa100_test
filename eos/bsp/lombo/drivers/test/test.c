/*
 * test.c - lombo module test driver
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
#include <debug.h>
#include "csp.h"
#include "../board.h"

typedef long (*ptestfn)(int argc, char **argv);

struct test_module {
	char	name[32];	/* name of the module */
	ptestfn	func;		/* test entry of the module */
};

/*
 * just see the disassembling code
 */
void test_barrier(void)
{
	int i, j, k;

	/* with barrier, this code will not be omitted by the compiler */
	k = *(int *)0;
	barrier();
	*(int *)0 = k;

	/* same effect with barrier abover */
	j = READREG32(0);
	WRITEREG32(0, j);

	/* without barrier, this code will be omitted by the compiler */
	i = *(int *)0;
	*(int *)0 = i;
}

long test_dummy(int argc, char **argv)
{
	LOG_I("test dummy called");

	test_barrier();
	return 0;
}

const struct test_module modules[] = {
	{"dummy",	test_dummy},
	{"timer",	test_timer},
#ifdef LOMBO_TEST_UART
	{"uart",	test_uart},
#endif
#ifdef LOMBO_TEST_GPIO
	{"gpio",	test_gpio},
#endif
#ifdef LOMBO_TEST_MEMORY
	{"memory",	test_memory},
#endif
#ifdef LOMBO_TEST_CACHE
	{"cache",	test_cache},
#endif
#ifdef LOMBO_TEST_DMA
	{"dma",		test_dma},
#endif
#ifdef LOMBO_TEST_CLK
	{"clk",		test_clk},
#endif
#ifdef LOMBO_TEST_CFG
	{"cfg",		test_config},
#endif
#ifdef LOMBO_TEST_SDC
	{"sdc",		test_sdc},
#endif
#ifdef LOMBO_TEST_SPI_NOR
	{"spinor",	test_spi_nor},
#endif
#ifdef LOMBO_TEST_PWM
	{"pwm",		test_pwm},
#endif
#ifdef LOMBO_TEST_MEMCTRL
	{"memctrl",	test_memctrl},
#endif
#ifdef LOMBO_TEST_PM
	{"pm",		test_pm},
#endif
#ifdef LOMBO_TEST_KEYBOARD
	{"keyboard",	test_keyboard},
#endif
#ifdef LOMBO_TEST_GSENSOR
	{"gsensor",	test_gsensor},
#endif
#ifdef LOMBO_TEST_GPS
	{"gps",	test_gps},
#endif
#ifdef LOMBO_TEST_WDOG
	{"wdog",	test_wdog},
#endif
#ifdef LOMBO_TEST_BINDTIMER
	{"btimer",	bind_timer_test},
#endif
#ifdef LOMBO_TEST_DEADLOCK_MONITOR
	{"dlockm",	deadlock_monitor_test},
#endif
#ifdef LOMBO_TEST_TOUCH_SCREEN
	{"touch",	test_touch_screen},
#endif
#ifdef LOMBO_TEST_INPUT
	{"input",	test_input},
#endif
#ifdef LOMBO_TEST_RTC
	{"rtc",	test_rtc},
#endif
#ifdef LOMBO_TEST_SYS
	{"sys",		test_sys},
#endif
#ifdef LOMBO_TEST_CPU
	{"cpu",		test_cpu},
#endif
#ifdef LOMBO_TEST_PTHREAD
	{"pthread",	test_pthread},
#endif
#ifdef LOMBO_TEST_PTHREAD_MUTEX
	{"pthread_mutex", test_pthread_mutex},
#endif
#ifdef LOMBO_TEST_SPINLOCK
	{"spinlock",	test_spinlock},
#endif
#ifdef LOMBO_TEST_AUDIO
	{"audio",	test_audio},
#endif
#ifdef LOMBO_TEST_I2C
	{"i2c",		test_i2c},
#endif
#ifdef LOMBO_TEST_VISS
	{"viss",	test_viss},
#endif
#ifdef LOMBO_TEST_VFP
	{"vfp",		test_vfp},
#endif
#ifdef LOMBO_TEST_NEON
	{"neon",	test_neon},
#endif
#ifdef LOMBO_TEST_ISP
	{"isp",		test_isp},
#endif
#ifdef LOMBO_TEST_DISP
	{"disp",	test_disp},
#endif
#ifdef LOMBO_TEST_OSAL_MEM
	{"osal_mem",	test_osal_mem},
#endif
#ifdef LOMBO_TEST_CPUFREQ
	{"cpufreq",	test_cpufreq},
#endif
};

void dump_usage(void)
{
	int i;

	LOG_I("Usage: lombo_test mod_name [para1][para2][..]. The mod_name can be:");
	for (i = 0; i < ARRAY_SIZE(modules); i++)
		LOG_I("      %s", modules[i].name);
}

/**
 * lombo_test - the test entrance for lombo modules
 * @argc: count of the arguments
 * @argv: the arguments buffer
 *
 * return 0 if success, -1 if failed
 */
long lombo_test(int argc, char **argv)
{
	char *mod_name = argv[1];
	int i;

	if (argc < 2) {
		LOG_E("invalid arg, argc is %d", argc);
		dump_usage();
		return -1;
	}

	for (i = 0; i < ARRAY_SIZE(modules); i++)
		if (!strcmp(mod_name, modules[i].name))
			return modules[i].func(argc, argv);

	return -1;
}
FINSH_FUNCTION_EXPORT(lombo_test, lombo test module[lombo_test dma/gpio/..]);
MSH_CMD_EXPORT(lombo_test, lombo test module[lombo_test dma/gpio/..]);

