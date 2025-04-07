/*
 * memctrl_test.c - memctrl test driver
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
#define DBG_SECTION_NAME	"MEMCTRL"
#define DBG_LEVEL		DBG_LOG
#include <debug.h>

#include <rtthread.h>
#include <rthw.h>

#include "dma/dma.h"
#include "dma/dma_common.h"
#include "memctrl/memctrl_debug.h"

typedef rt_err_t  (*test_func)(void);

#define TRANSFER_LENGTH		102400
#define TRANSFER_COUNT		100

static struct dma_channel *dma_chan_sg = RT_NULL;
static char *mem_src = RT_NULL, *mem_dst = RT_NULL;
static rt_bool_t dma_count;
static struct debug_param input;

static rt_err_t test_memctrl_kick_dma(void);

static void test_memctrl_dma_callback(void *param)
{
	if (++dma_count > TRANSFER_COUNT) {
		dma_terminate(dma_chan_sg);
		dma_release_channel(dma_chan_sg);
		dma_chan_sg = RT_NULL;
		rt_free(mem_src);
		rt_free(mem_dst);
	} else
		test_memctrl_kick_dma();
}

static rt_err_t test_memctrl_kick_dma(void)
{
	struct dma_config config;
	u32 dma_src, dma_dst;
	rt_err_t ret;

	/* Virtual addr to physic addr */
	dma_src = (u32)virt_to_phys(mem_src);
	dma_dst = (u32)virt_to_phys(mem_dst);

	if (dma_chan_sg == RT_NULL) {
		dma_chan_sg = dma_request_channel("memctrl-dma");
		if (!dma_chan_sg) {
			LOG_E("request dmatest-rx dma_chan_sg fail");
			return 0;
		}

		/* 2. config dma channel */
		config.direction = DMA_MEM_TO_MEM;
		config.src_addr = dma_src;
		config.dst_addr = dma_dst;
		config.src_addr_width = DMA_BUSWIDTH_1_BYTE;
		config.dst_addr_width = DMA_BUSWIDTH_1_BYTE;
		config.src_maxburst = 4;
		config.dst_maxburst = 4;
		config.slave_id = MEM_RT_RP;
		config.priority = DMA_CHAN_PRIO_0;
		config.int_type = COMPLETE_INT;
		config.trans_type = MEMORY_TRANS;

		ret = dma_config_channel(dma_chan_sg, &config);
		if (ret < 0) {
			LOG_E("slave config fail %d", ret);
			return 0;
		}
	}

	/* 3. prepare sg tx descriptor */
	ret = dma_prep_sg(dma_chan_sg, dma_src, dma_dst, TRANSFER_LENGTH,
			  DMA_MEM_TO_MEM, test_memctrl_dma_callback, RT_NULL);
	if (ret < 0) {
		LOG_E("fail to prepare dma memcpy");
		dma_release_channel(dma_chan_sg);
		return 0;
	}

	/* 4. submit descriptor */
	ret = dma_submit(dma_chan_sg);
	if (ret < 0) {
		LOG_E("sg submit fail %d", ret);
		dma_release_channel(dma_chan_sg);
		return 0;
	}

	/* 5. issue transfer */
	dma_issue_pending(dma_chan_sg);

	return RT_EOK;
}

static rt_err_t test_memctrl_dma(void)
{
	rt_err_t ret;

	mem_src = rt_malloc(TRANSFER_LENGTH);
	if (!mem_src) {
		LOG_E("alloc mem failed");
		return -RT_ENOMEM;
	}
	mem_dst = rt_malloc(TRANSFER_LENGTH);
	if (!mem_dst) {
		LOG_E("alloc mem failed");
		rt_free(mem_src);
		return -RT_ENOMEM;
	}

	dma_count = 0;
	ret = test_memctrl_kick_dma();
	if (ret != RT_EOK) {
		rt_free(mem_src);
		rt_free(mem_dst);
		return ret;
	}

	input.count = 2;
	input.flaw_unit = 1;
	input.period = 1;
	input.time = 3;
	input.size_unit = 0;
	input.sample_unit = 1;
	input.op = OP_CONSOLE;
	input.background = RT_TRUE;
	input.running = RT_TRUE;
	memctrl_do(&input);

	return RT_EOK;
}

static test_func test_funcs[] = {
	test_memctrl_dma
};

long test_memctrl(int argc, char **argv)
{
	rt_err_t ret;
	int i;

	LOG_D("Test memctrl start...");

	for (i = 0; i < ARRAY_SIZE(test_funcs); i++) {
		ret = test_funcs[i]();
		if (ret != RT_EOK)
			return -1;
	}

	LOG_D("Test memctrl successfully");

	return 0;
}

