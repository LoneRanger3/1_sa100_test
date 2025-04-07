/*
 * clk_test.c
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
#define DBG_LEVEL		0
#define DBG_SECTION_NAME	"CLK_TEST"

#include <clk_debug.h>
#include <rtthread.h>
#include "csp.h"
#include "clk.h"
#include "clk_csp.h"
#include "clk_private.h"


struct UNITY_STORAGE_T {
	const char *file;
	const char *name;
	int ntests;
	int cur_test_line;
	int nfails;
	int line_failed;
};
struct UNITY_STORAGE_T unity;

#define UNITY_TEST_ASSERT(condition, line, format...) \
do { \
	if (!(condition) && (unity.line_failed == 0)) {\
		LOG_E(format); \
		LOG_RAW("%s:%d:%s:failed at %d\n", unity.file, \
			unity.cur_test_line,\
			unity.name, line); \
		unity.line_failed = line; \
	} \
} while (0)

#define TEST_ASSERT(condition, format...) \
do {int line = __LINE__; \
		UNITY_TEST_ASSERT((condition), line, format); \
} while (0)

#define TEST_ASSERT_ON(condition) do { int line = __LINE__; \
	UNITY_TEST_ASSERT((condition), line,\
	" Expression Evaluated To FALSE"); } while (0)

#define TEST_ASSERT_EQUAL(expected, actual) do { int line = __LINE__; \
		UNITY_TEST_ASSERT((expected == actual), line,\
		"expected:%d, actual:%d\n", expected, actual); \
	} while (0)


#define PRT_INFO(...)	LOG_I(__VA_ARGS__)
#define PRT_ERR(...)	LOG_E(__VA_ARGS__)

#define PLL_NAME	CLK_NAME_PERH1_PLL
#define PLL_SRC_NAME	CLK_NAME_OSC24M

#define MODULE_NAME	CLK_NAME_SPI1_CLK
#define MODULE_PARENT	CLK_NAME_PERH0_PLL_DIV2 /* clk parent to test */
#define MODULE_PARENT_RATE	594000000 /* clk parent to test */

#ifdef ARCH_LOMBO_N7V0
#define AUDIO_MOD_NAME	CLK_NAME_I2S_CLK
#endif
#ifdef ARCH_LOMBO_N7V1
#define AUDIO_MOD_NAME	CLK_NAME_I2S0_CLK
#endif

#define GATE_NAME	CLK_NAME_AHB_SDC0_GATE

#ifdef ARCH_LOMBO_N7V0
#define AUDIO_NAME	CLK_NAME_I2S_CLK
#endif
#ifdef ARCH_LOMBO_N7V1
#define AUDIO_NAME	CLK_NAME_I2S0_CLK
#endif

/* rates to test for pll, module and divider clk */
int rates[] = {
	/* [0, 10M] */
	0, 1, 10, 100, 1111, 8021, 16000, 64000, 50000, 250000,
	700000, 1000000, 4000051, 7234567, 8000000, 9500000,
	/* [10M, 100M] */
	10000000, 11000000, 12000000, 13000000, 14000000, 15000000,
	16000000, 17000000, 18000000, 19000000, 24000000, 25000000,
	26000000, 27000000, 28000000, 29000000, 30000000, 31000000,
	32000000, 33000000, 40000000, 50000000, 59000000, 74000000,
	/* [100M, 1000M] */
	130000000, 170000000, 180000000, 190000000, 200000000,
	210000000, 220000000, 230000000, 240000000, 250000000,
	260000000, 270000000, 280000000, 290000000, 300000000,
	310000000, 320000000, 330000000, 340000000, 350000000,
	360000000, 370000000, 380000000, 390000000, 400000000,
	410000000, 600000000, 720000000, 850000000, /* 1008000000 */
};
int rates_cpu[] = {
	/* [0, 10M] */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1,
	/* [10M, 100M] */
	10000000, 11000000, 12000000, 13000000, 14000000, 15000000,
	16000000, 17000000, 18000000, 19000000, 24000000, 25000000,
	26000000, 27000000, 28000000, 29000000, 30000000, 31000000,
	32000000, 33000000, 40000000, 50000000, 59000000, 74000000,
	/* [100M, 1000M] */
	130000000, 170000000, 180000000, 190000000, 200000000,
	210000000, 220000000, 230000000, 240000000, 240000000,
	240000000, 240000000, 240000000, 240000000, 240000000,
	240000000, 240000000, 240000000, 240000000, 240000000,
	360000000, 360000000, 360000000, 360000000, 360000000,
	360000000, 600000000, 720000000, 840000000, /* 1008000000 */
};

int rates_spi[] = {
	/* [0, 10M] */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1,
	/* [10M, 100M] */
	-1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, 24750000,
	24750000, 24750000, 24750000, 24750000, 29700000, 29700000,
	29700000, 29700000, 39600000, 49500000, 49500000, 59400000,
	/* [100M, 1000M] */
	118800000, 148500000, 148500000, 148500000, 198000000,
	198000000, 198000000, 198000000, 198000000, 198000000,
	198000000, 198000000, 198000000, 198000000, 198000000,
	198000000, 198000000, 198000000, 198000000, 198000000,
	198000000, 198000000, 198000000, 198000000, 198000000,
	198000000, 198000000, 198000000, 198000000, /* 1008000000 */
};

/* rates to test for audio clk */
int audio_rates[] = {
	0, 1, 10, 100, 1000, 1000000, 8000000,
	CLK_24571M, CLK_24571M_X2, CLK_24571M_X3,
	CLK_225882M, CLK_225882M_X2, CLK_225882M_X3,
	CLK_24571M_X6, CLK_225882M * 6, 876543210
};

/* rates to test for audio clk */
int audio_rates_expected[] = {
	-1, -1, -1, -1, -1, -1, -1,
	CLK_24571M, CLK_24571M_X2, CLK_24571M_X3,
	CLK_225882M, CLK_225882M_X2, CLK_225882M_X3,
	-1, -1, -1
};


/**
 * fix_rate_clk_test - test fixed-clock(fix rate)
 */
void fix_rate_clk_test(void)
{
	char *clk_name[] = {
		CLK_NAME_OSC24M,
		CLK_NAME_OSC32K,
		CLK_NAME_NULL_CLK
	};
	int rate[] = {CLK_24M, CLK_32K, 0};
	int tmp_rate;
	clk_handle_t clock = 0;
	int i;

	for (i = 0; i < sizeof(rate)/sizeof(rate[0]); i++) {
		/* get clock handle */
		clock = clk_get(clk_name[i]);
		TEST_ASSERT((clock > 0),
			"index:%i, clk:%s", i, clk_name[i] ? clk_name[i] : "NULL");

		/* get clock rate */
		tmp_rate = (int)clk_get_rate(clock);
		TEST_ASSERT(tmp_rate == rate[i],
			"%s: clock %s rate is %d\n", clk_name, tmp_rate);

		/* put clock handle */
		clk_put(clock);
	}
}

/**
 * fix_factor_clk_test - test fix-factor-clock
 */
void fix_factor_clk_test(void)
{
	clk_handle_t clock = 0;
	clk_handle_t ahb_clock = 0;
	clk_handle_t ahb_pclock = 0;
	int rate, ori_rate, minrate, maxrate;
	int ret;

	/* test init, get minrate, maxrate */
	ahb_clock = clk_get(CLK_NAME_AHB_CLK);
	TEST_ASSERT_ON(ahb_clock > 0);
	ahb_pclock = clk_get_parent(ahb_clock);
	TEST_ASSERT_ON(ahb_pclock > 0);
	rate = (int)clk_get_rate(ahb_pclock);
	TEST_ASSERT_ON(!IS_ERR(rate));
	minrate = rate/4/2;
	maxrate = rate/2;
	clk_put(ahb_clock);
	clk_put(ahb_pclock);

	/* get clock handle */
	clock = clk_get(CLK_NAME_APB_CLK);
	TEST_ASSERT_ON(clock > 0);

	/* get clock rate */
	ori_rate = (int)clk_get_rate(clock);
	TEST_ASSERT_ON((ori_rate >= minrate) && (ori_rate <= maxrate));

	/* set clock rate */
	ret = clk_set_rate(clock, minrate - 1);
	TEST_ASSERT_ON(ret != 0);

	/* check if set rate success */
	ret = clk_set_rate(clock, minrate);
	TEST_ASSERT_ON(ret == 0);
	rate = (int)clk_get_rate(clock);
	TEST_ASSERT_EQUAL(minrate, rate);

	ret = clk_set_rate(clock, minrate + 1);
	TEST_ASSERT_ON(ret == 0);
	rate = (int)clk_get_rate(clock);
	TEST_ASSERT_EQUAL(minrate, rate);

	/* check if set rate success */
	ret = clk_set_rate(clock, maxrate - 1);
	TEST_ASSERT_ON(ret == 0);
	rate = (int)clk_get_rate(clock);
	TEST_ASSERT_EQUAL(maxrate/2, rate);

#if 0
	/* set clock rate */
	ret = clk_set_rate(clock, maxrate + 1);
	TEST_ASSERT_ON(ret == 0);
	rate = (int)clk_get_rate(clock);
	TEST_ASSERT_EQUAL(maxrate, rate);

	/* check if set rate success */
	ret = clk_set_rate(clock, maxrate);
	TEST_ASSERT_ON(ret == 0);
	rate = (int)clk_get_rate(clock);
	TEST_ASSERT_EQUAL(maxrate, rate);
#endif
	ret = clk_set_rate(clock, ori_rate);
	TEST_ASSERT_ON(ret == 0);
	/* put clock handle */
	clk_put(clock);

}

/**
 * gate_clk_test - test gate-clock
 */
void gate_clk_test(void)
{
	clk_handle_t clock = 0;
	char clk_name[32] = GATE_NAME;
	int ret;

	/* get clock handle */
	clock = clk_get(clk_name);
	TEST_ASSERT_ON(clock > 0);

	/* enable clock */
	ret = clk_enable(clock);
	TEST_ASSERT((ret == 0), "clock %s prepare enable failed\n", clk_name);

	/* recover the environment */
	clk_disable(clock);

	/* put clock handle */
	clk_put(clock);
}

/**
 * divider_clk_test - test divider-clk
 */
void divider_clk_test(void)
{
	clk_handle_t clock = 0;
	clk_handle_t pclock = 0;
	int i, rate, ori_rate;
	int ret;
	int prate;

	/* get clock handle */
	clock = clk_get(CLK_NAME_CPU_AXI_CLK);
	TEST_ASSERT_ON(clock > 0);
	pclock = clk_get_parent(clock);
	TEST_ASSERT_ON(pclock > 0);
	prate = (int)clk_get_rate(pclock);
	TEST_ASSERT_ON(!IS_ERR(prate));

	/* get clock rate */
	ori_rate = (int)clk_get_rate(clock);
	TEST_ASSERT_ON(!IS_ERR(ori_rate));

	for (i = 0; i < ARRAY_SIZE(rates); i++) {
		ret = clk_set_rate(clock, rates[i]);
		if (rates[i] < prate/4) {
			TEST_ASSERT_ON(ret < 0);
			continue;
		}
		TEST_ASSERT_ON(ret == 0);
		rate = (int)clk_get_rate(clock);
		TEST_ASSERT_ON(!IS_ERR(rate));
		if (rates[i] < prate/3)
			TEST_ASSERT_EQUAL(prate/4, rate);
		else if (rates[i] < prate/2)
			TEST_ASSERT_EQUAL(prate/3, rate);
		else if (rates[i] < prate)
			TEST_ASSERT_EQUAL(prate/2, rate);
		else
			TEST_ASSERT_EQUAL(prate, rate);
	}

	/* recover the environment */
	clk_set_rate(clock, ori_rate);

	/* put clock handle */
	clk_put(clock);
}

int get_pll_expect(int rate)
{
	int step_6[2];
	int step_8[2];
	int step_12[2];
	int step_24[2];
	int rem = 24;
	int rate_mhz = rate/1000000;
	int tmp_rate = -1;
	step_6[0] = 6*10;
	step_6[1] = 6*50;
	step_8[0] = 8*10;
	step_8[1] = 8*50;
	step_12[0] = 12*10;
	step_12[1] = 12*50;
	step_24[0] = 24*10;
	step_24[1] = 24*50;
	if (rate_mhz >= step_6[0] && rate_mhz <= step_6[1]) {
		rem = rate_mhz % 6;
		tmp_rate = rate_mhz/6*6000000;
	}
	if (rate_mhz >= step_12[0] && rate_mhz <= step_12[1]
		&& (rate_mhz % 12 < rem)) {
		rem = rate % 12;
		tmp_rate = rate_mhz/12*12000000;
	}
	if (rate_mhz >= step_8[0] && rate_mhz <= step_8[1]
		&& (rate_mhz % 8 < rem)) {
		rem = rate%8;
		tmp_rate = rate_mhz/8*8000000;
	}
	if (rate_mhz >= step_24[0] && rate_mhz <= step_24[1]
		&& (rate_mhz % 24 < rem)) {
		rem = rate % 24;
		tmp_rate = rate_mhz/24*24000000;
	}
	return tmp_rate;
}


/**
 * pll_clk_test - test pll-clk
 */
void pll_clk_test(void)
{
	char clk_name[32] = PLL_NAME;
	clk_handle_t clock = 0;
	clk_handle_t clock_p = 0;
	clk_handle_t clock_tmp = 0;
	int i, rate, ori_rate;
	int min_rate = 60000000, max_rate = 1200000000;
	int ret;

	/* get clock handle */
	clock = clk_get(clk_name);
	TEST_ASSERT_ON(clock > 0);

	/* get parent clock */
	clock_p = clk_get_parent(clock);
	TEST_ASSERT_ON(clock_p > 0);
	/* get parent clock */
	clock_tmp = clk_get(CLK_NAME_OSC24M);
	TEST_ASSERT_EQUAL(clock_p, clock_tmp);
	clk_put(clock_p);
	clk_put(clock_tmp);

	/* prepare enable clock */
	ret = clk_enable(clock);
	TEST_ASSERT_ON(ret == 0);

	/* get clock rate */
	ori_rate = (int)clk_get_rate(clock);
	TEST_ASSERT_ON(!IS_ERR(ori_rate));

	/* set clock rate */
	for (i = 0; i < ARRAY_SIZE(rates); i++) {
		ret = clk_set_rate(clock, rates[i]);
		if (rates[i] < min_rate) {
			TEST_ASSERT_ON(ret < 0);
			continue;
		}
		TEST_ASSERT_ON(ret == 0);
		rate = (int)clk_get_rate(clock);
		TEST_ASSERT_ON(!IS_ERR(rate));
		if (rates[i] >= max_rate) {
			TEST_ASSERT_EQUAL(max_rate, rate);
			continue;
		}
		TEST_ASSERT_EQUAL(get_pll_expect(rate), rate);
	}

	/* recover the environment */
	clk_set_rate(clock, ori_rate);
	clk_disable(clock);
	/* put clock handle */
	clk_put(clock);

}


/**
 * pll_clk_test - test pll-clk
 */
void cpu_clk_test(void)
{
	char clk_name[32] = CLK_NAME_CPU_PLL;
	clk_handle_t clock = 0;
	clk_handle_t clock_p = 0;
	clk_handle_t clock_tmp = 0;
	int i, rate, ori_rate;
	int min_rate = 240000000, max_rate = 1008000000;
	int ret;

	/* get clock handle */
	clock = clk_get(clk_name);
	TEST_ASSERT_ON(clock > 0);

	/* get parent clock */
	clock_p = clk_get_parent(clock);
	TEST_ASSERT_ON(clock_p > 0);
	/* get parent clock */
	clock_tmp = clk_get(CLK_NAME_OSC24M);
	TEST_ASSERT_EQUAL(clock_p, clock_tmp);
	clk_put(clock_p);
	clk_put(clock_tmp);

	/* prepare enable clock */
	ret = clk_enable(clock);
	TEST_ASSERT_ON(ret == 0);

	/* get clock rate */
	ori_rate = (int)clk_get_rate(clock);
	TEST_ASSERT_ON(!IS_ERR(ori_rate));

	/* set clock rate */
	for (i = 0; i < ARRAY_SIZE(rates); i++) {
		ret = clk_set_rate(clock, rates[i]);
		if (rates[i] < min_rate) {
			TEST_ASSERT_ON(ret < 0);
			continue;
		}
		TEST_ASSERT_ON(ret == 0);
		rate = (int)clk_get_rate(clock);
		TEST_ASSERT_ON(!IS_ERR(rate));
		if (rates[i] >= max_rate) {
			TEST_ASSERT_EQUAL(max_rate, rate);
			continue;
		}
		TEST_ASSERT_EQUAL(rates_cpu[i], rate);
	}

	/* recover the environment */
	clk_set_rate(clock, ori_rate);
	clk_disable(clock);
	/* put clock handle */
	clk_put(clock);

}



/**
 * module_clk_test - test module-clk
 */
void module_clk_test(void)
{
	char clk_name[32] = MODULE_NAME;
	char clk_p_name[32] = MODULE_PARENT;
	clk_handle_t clock = 0;
	clk_handle_t clock_tmp = 0;
	clk_handle_t clock_p_org = 0;
	clk_handle_t clock_p_test = 0; /* handle for clk_p_name */
	int i, rate;
	int ret;

	/* get clock handle */
	clock = clk_get(clk_name);
	TEST_ASSERT_ON(clock > 0);

	/* get parent(to test) clock handle */
	clock_p_test = clk_get(clk_p_name);
	TEST_ASSERT_ON(clock_p_test > 0);

	/*
	 * test set clock parent
	 */
	/* get original parent clock */
	clock_p_org = clk_get_parent(clock);
	TEST_ASSERT_ON(clock_p_org > 0);

	/* set clock parent */
	ret = clk_set_parent(clock, clock_p_test);
	TEST_ASSERT_ON(ret == 0);

	/* check if set parent success */
	clock_tmp = clk_get_parent(clock);
	TEST_ASSERT_EQUAL(clock_p_test, clock_tmp);
	ret = clk_set_rate(clock_p_test, MODULE_PARENT_RATE);
	TEST_ASSERT_ON(ret == 0);

	/*
	 * test set rate
	 */
	/* prepare enable clock */
	ret = clk_enable(clock);
	TEST_ASSERT_ON(ret == 0);

	/* get clock rate */
	rate = (int)clk_get_rate(clock);
	TEST_ASSERT_ON(!IS_ERR(rate));

	/* set clock rate */
	for (i = 0; i < ARRAY_SIZE(rates); i++) {
		ret = clk_set_rate(clock, rates[i]);
		if (rates_spi[i] < 0) {
			TEST_ASSERT(ret < 0, "set r:%d ret %d", rates[i], ret);
			continue;
		}
		TEST_ASSERT_ON(ret == 0);
		rate = (int)clk_get_rate(clock);
		TEST_ASSERT_ON(!IS_ERR(rate));
		TEST_ASSERT_EQUAL(rates_spi[i], rate);
	}

	/* recover the environment */
	clk_set_rate(clock, rate);
	clk_disable(clock);

	/* recover clock parent */
	clk_set_parent(clock, clock_p_org);

	/* put clock handle */
	clk_put(clock);
	clk_put(clock_p_test);
}


/**
 * audio_clk_test - test audio clk, such as i2s_clk, audio_pll_divX
 */
void audio_clk_test(void)
{
	char clk_name[32] = AUDIO_MOD_NAME;
	char *pname[2] = {CLK_NAME_AUDIO_PLL_DIV7, CLK_NAME_AUDIO_PLL_DIV17};
	clk_handle_t clock = 0;
	clk_handle_t clock_p, clock_tmp;
	int i, rate;
	int pindex;
	int ret;

	/* get clock handle */
	clock = clk_get(clk_name);
	TEST_ASSERT_ON(clock > 0);

	for (pindex = 0; pindex < ARRAY_SIZE(pname); pindex++) {
		/* get parent clock */
		clock_p = clk_get(CLK_NAME_AUDIO_PLL_DIV7);
		TEST_ASSERT_ON(clock_p > 0);

		/* set clock parent */
		ret = clk_set_parent(clock, clock_p);
		TEST_ASSERT_ON(ret == 0);

		/* check if set parent success */
		clock_tmp = clk_get_parent(clock);
		TEST_ASSERT_EQUAL(clock_p, clock_tmp);
		clk_put(clock_tmp);
		clk_put(clock_p);

		/* prepare enable clock */
		ret = clk_enable(clock);
		TEST_ASSERT_ON(ret == 0);

		/* set clock rate */
		for (i = 0; i < ARRAY_SIZE(audio_rates); i++) {
			ret = clk_set_rate(clock, audio_rates[i]);
			if (audio_rates_expected[i] < 0) {
				TEST_ASSERT(ret < 0, "set r:%d ret %d",
					audio_rates[i], ret);
				continue;
			}
			TEST_ASSERT_ON(ret == 0);

			rate = (int)clk_get_rate(clock);
			TEST_ASSERT_ON(!IS_ERR(rate));
			TEST_ASSERT_EQUAL(audio_rates_expected[i], rate);
		}

		clk_disable(clock);
	}

	/* put clock handle */
	clk_put(clock);
}

typedef void (*test_func)(void);

#define clk_test_run(func, func_line) \
do { \
	unity.name = #func; \
	unity.cur_test_line = func_line; \
	unity.ntests++; \
	unity.line_failed = 0; \
	func(); \
	if (!unity.line_failed) \
		LOG_RAW("%s:%d:%s:pass\n", unity.file, \
			unity.cur_test_line, unity.name); \
	else \
		unity.nfails++; \
} while (0)\


/**
 * clk_test - clk drv test function
 */
long test_clk(int argc, char **argv)
{
	if (argc != 2)
		return -1;
	unity.file = __FILE__;
	unity.name = NULL;
	unity.cur_test_line = 0;
	unity.ntests = 0;
	unity.nfails = 0;
	unity.line_failed = 0;

	clk_init();

	clk_test_run(fix_rate_clk_test, __LINE__);
	clk_test_run(fix_factor_clk_test, __LINE__);
	clk_test_run(gate_clk_test, __LINE__);
	clk_test_run(divider_clk_test, __LINE__);
	clk_test_run(pll_clk_test, __LINE__);
	clk_test_run(module_clk_test, __LINE__);
	clk_test_run(audio_clk_test, __LINE__);
	clk_test_run(cpu_clk_test, __LINE__);

	LOG_RAW("================");
	LOG_RAW("number of tests:%d ---- ", unity.ntests);
	LOG_RAW("nfails:%d\n", unity.nfails);
	if (unity.nfails == 0U)
		LOG_RAW("=======ok=========");
	else
		LOG_RAW("=======fail=========");
	return 0;
}


