#include <rtthread.h>

#include "dma/dma.h"
#include "dma/dma_common.h"

#define DBG_SECTION_NAME	"DMAT"
#define DBG_LEVEL		DBG_INFO
#include <debug.h>

#define TRANSFER_COUNTS		512
#define TRANSFER_COUNTS_1k	1024
#define TRANSFER_COUNTS_1031B	1031

#define TRANSFER_SG_TEST_COUNTS	1000
#define TRANSFER_CYCLIC_TEST_COUNTS	1000

static int num;
static char *src, *dst;
static char *src_1, *dst_1;
static unsigned char *dma_src, *dma_dst;
static struct dma_channel *chan_sg = RT_NULL;
static struct dma_channel *chan_cyclic = RT_NULL;
static rt_sem_t sem;

static unsigned int pos; /* where should we write in the memory */
static unsigned char w_value; /* what should we write to the memory */

/* in order to create a random value */
static u64 next = 1;

/* create a random value */
static u64 random()
{
	u64 random;

	next += rt_tick_get();
	next = next * 1103515245L + 12345;
	random = next / 65536L;

	return random;
}

/* create a random value in range */
static u32 random2(u32 range)
{
	u32 rand;

	if (0 == range)
		return 0;
	rand = random() % range;
	return rand;
}

static int dma_sg_test(void);

static void dma_callback(void *param)
{
	num++;
	LOG_I("dma test: %d", num);

	/**
	 * adjust whether the number at pos in dst is equal
	 * to the number at pos in src.
	 */
	if (w_value == *(dma_dst + pos) &&
		*(dma_dst + pos) == *(dma_src + pos)) {

		/* sg mode */
		if (chan_sg) {
			if (num > TRANSFER_SG_TEST_COUNTS) {
				LOG_I("terminate sg chan");
				dma_terminate(chan_sg);
				rt_sem_release(sem);
				return;
			} else
				dma_sg_test();
		} else {
			/* cyclic mode */
			if (num > TRANSFER_CYCLIC_TEST_COUNTS) {
				LOG_I("terminate cyclic");
				dma_terminate(chan_cyclic);
				rt_sem_release(sem);
				return;
			}

			pos = random2(TRANSFER_COUNTS);
			if (num % 2 != 0) {
				/**
				 * we need generate the random number for
				 * the second 512B
				 */
				pos += 512;
			}
			w_value = random2(256);
			LOG_D("%s:%d, pos = %d, w_value = %d\n",
						__func__, __LINE__, pos, w_value);
			*(dma_src + pos) = w_value;
		}
	} else {
		if (chan_sg) {
			LOG_E("SG test error, %s:%d\n", __func__, __LINE__);
			dma_terminate(chan_sg);
		} else {
			LOG_E("Cyclic test error, %s:%d\n", __func__, __LINE__);
			dma_terminate(chan_cyclic);
		}
		rt_sem_release(sem);
	}
	return;
}

static int dma_sg_test(void)
{
	struct dma_config config;
	int ret;

	if (!src_1 || !dst_1) {
		LOG_E("alloc coherent fail");
		return 0;
	}

	dma_src = (unsigned char *)unca_to_phys(src_1);
	dma_dst = (unsigned char *)unca_to_phys(dst_1);

	/* 1. request dma channel */
	if (chan_sg == RT_NULL) {
		chan_sg = dma_request_channel("dmatest-rx");
		if (!chan_sg) {
			LOG_E("request dmatest-rx chan_sg fail");
			return 0;
		}

		/* 2. config dma channel */
		config.direction = DMA_MEM_TO_MEM;
		config.src_addr = (u32)dma_src;
		config.dst_addr = (u32)dma_dst;
		config.src_addr_width = DMA_BUSWIDTH_8_BYTES;
		config.dst_addr_width = DMA_BUSWIDTH_8_BYTES;
		config.src_maxburst = 4;
		config.dst_maxburst = 4;
		config.slave_id = MEM_RT_RP;
		config.priority = DMA_CHAN_PRIO_0;
		config.int_type = COMPLETE_INT;
		config.trans_type = MEMORY_TRANS;

		ret = dma_config_channel(chan_sg, &config);
		if (ret < 0) {
			LOG_E("slave config fail %d", ret);
			return 0;
		}
	}

	/**
	 * here we should generate two random numbers, one represents
	 * the place in src memory, the other is the number
	 * that will be set into this place.
	 */
	pos = random2(TRANSFER_COUNTS_1031B);
	w_value = random2(256);
	*(dma_src + pos) = w_value;
	LOG_D("%s: %d, pos=%d, w_value=%d", __func__, __LINE__, pos, w_value);

	/* 3. prepare sg tx descriptor */
	/* 3.1 first tx descriptor */
	ret = dma_prep_sg(chan_sg, (u32)dma_src, (u32)dma_dst, TRANSFER_COUNTS_1031B,
			  DMA_MEM_TO_MEM, dma_callback, RT_NULL);
	if (ret < 0) {
		LOG_E("fail to prepare dma memcpy");
		dma_release_channel(chan_sg);
		return 0;
	}

	/* 4. submit descriptor */
	ret = dma_submit(chan_sg);
	if (ret < 0) {
		LOG_E("sg submit fail %d", ret);
		dma_release_channel(chan_sg);
		return 0;
	}

	/* 5. issue transfer */
	dma_issue_pending(chan_sg);

	/* 6. release channel */
	/* dma_release_channel(chan_sg); */

	return 0;
}

static int dma_cyclic_test(void)
{
	struct dma_config config;
	int ret;

	if (!src || !dst) {
		LOG_E("dma test: alloc coherent fail");
		return 0;
	}

	dma_src = (unsigned char *)unca_to_phys(src);
	dma_dst = (unsigned char *)unca_to_phys(dst);

	/* 1. request dma channel */
	if (chan_cyclic == RT_NULL) {
		chan_cyclic = dma_request_channel("dmatest-rx");
		if (!chan_cyclic) {
			LOG_E("request dmatest-rx chan_cyclic fail");
			return 0;
		}

		/* 2. config dma channel */
		config.direction = DMA_MEM_TO_MEM;
		config.src_addr = (u32)dma_src;
		config.dst_addr = (u32)dma_dst;
		config.src_addr_width = DMA_BUSWIDTH_8_BYTES;
		config.dst_addr_width = DMA_BUSWIDTH_8_BYTES;
		config.src_maxburst = 4;
		config.dst_maxburst = 4;
		config.slave_id = MEM_RT_RP;
		config.priority = DMA_CHAN_PRIO_0;
		config.int_type = BLOCK_INT;
		config.trans_type = CYCLIC_TRANS;

		ret = dma_config_channel(chan_cyclic, &config);
		if (ret < 0) {
			LOG_E("slave config fail %d", ret);
			return 0;
		}
	}

	/**
	 * here we should generate two random numbers, one represents
	 * the place in src memory, the other is the number
	 * that will be set into this place.
	 */
	pos = random2(TRANSFER_COUNTS);
	w_value = random2(256);
	*(dma_src + pos) = w_value;
	LOG_D("%s: %d, pos=%d, w_value=%d", __func__, __LINE__, pos, w_value);

	/* 3. prepare cyclic tx descriptor */
	ret = dma_prep_cyclic(chan_cyclic, (u32)dma_src, (u32)dma_dst, TRANSFER_COUNTS_1k,
			      TRANSFER_COUNTS, DMA_MEM_TO_MEM, dma_callback,
			      RT_NULL);
	if (ret < 0) {
		LOG_E("fail to prepare cyclic dma memcpy");
		dma_release_channel(chan_cyclic);
		return 0;
	}

	/* 4. submit descriptor */
	ret = dma_submit(chan_cyclic);
	if (ret < 0) {
		LOG_E("sg submit fail %d", ret);
		dma_release_channel(chan_cyclic);
		return 0;
	}

	/* 5. issue transfer */
	dma_issue_pending(chan_cyclic);

	/* 6. release channel */
	/* dma_release_channel(chan_cyclic); */

	return 0;
}

int test_dma(int argc, char **argv)
{
	num = 0;
	char *test_mode = argv[2];
	LOG_I("dma test enter");

	src = rt_malloc_unca(TRANSFER_COUNTS_1k);
	dst = rt_malloc_unca(TRANSFER_COUNTS_1k);

	src_1 = rt_malloc_unca(TRANSFER_COUNTS_1031B);
	dst_1 = rt_malloc_unca(TRANSFER_COUNTS_1031B);

	if (argc < 3) {
		LOG_E("invalid arg, argc is %d", argc);
		return 0;
	}

	/* semaphore for bottom irq */
	sem = rt_sem_create("dma-test", 0, RT_IPC_FLAG_FIFO);
	if (!sem) {
		LOG_E("sem create failed");
		return 0;
	}

	if (!strcmp(test_mode, "sg")) {
		if (chan_cyclic) {
			dma_release_channel(chan_cyclic);
			chan_cyclic = RT_NULL;
		}
		dma_sg_test();
	} else if (!strcmp(test_mode, "cyclic")) {
		if (chan_sg) {
			dma_release_channel(chan_sg);
			chan_sg = RT_NULL;
		}
		num = 0;
		dma_cyclic_test();
	} else
		LOG_E("unsupport argv: %s", test_mode);

	rt_sem_take(sem, RT_WAITING_FOREVER);

	if (chan_cyclic) {
		dma_release_channel(chan_cyclic);
		chan_cyclic = RT_NULL;
	}

	if (chan_sg) {
		dma_release_channel(chan_sg);
		chan_sg = RT_NULL;
	}

	rt_free_unca(src);
	rt_free_unca(dst);

	rt_free_unca(src_1);
	rt_free_unca(dst_1);

	return 0;
}
