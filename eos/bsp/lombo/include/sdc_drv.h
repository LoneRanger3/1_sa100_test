/*
 * sdc_drv.h - Generic definitions of LomboTech SDC Driver
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

#ifndef ___SDC__DRV___H___
#define ___SDC__DRV___H___

#include "spinlock.h"

#define ENABLE_TRACE			0

#if (ENABLE_TRACE == 1)

#define PRT_TRACE_BEGIN(fmt, ...)	LOG_D("|B|"fmt, ##__VA_ARGS__)
#define PRT_TRACE_END(fmt, ...)		LOG_D("|E|"fmt, ##__VA_ARGS__)

#else

#define PRT_TRACE_BEGIN(fmt, ...)	do { } while (0)
#define PRT_TRACE_END(fmt, ...)		do { } while (0)

#endif

#define BUG_ON(__BUG_ON_COND)		RT_ASSERT(!(__BUG_ON_COND))

#ifdef ARCH_LOMBO_N7

/* card init clock rate: 120K */
#define LBSDC_INIT_CLOCK_RATE		(120000)

/**
 * if clock rate <= 12M: select OSC 24MHZ as clock source
 * if clock rate >  12M: select PLL 1.2G as clock source
 */
#define LBSDC_CLK_SRC_SWITCH_RATE	(12000000)

#define CLOCK_PARENT(clock)		\
	(((clock)			\
	> LBSDC_CLK_SRC_SWITCH_RATE)	\
	? CLK_NAME_PERH0_PLL_VCO	\
	: CLK_NAME_OSC24M)

#else

#error "please select a valid platform\n"

#endif

#define LBSDC_WAIT_UCF_TIMEOUT		(2)

#define LBSDC_MAX_BLK_SIZE		(512)
#define LBSDC_MAX_BLOCK_CNT		(4096)
#define LBSDC_MAX_DATA_SIZE		(LBSDC_MAX_BLK_SIZE * LBSDC_MAX_BLOCK_CNT)
#define LBSDC_IDMA_DESC_TABLE_SIZE	(4096)
#define LBSDC_MAX_DESC_CNT		(LBSDC_IDMA_DESC_TABLE_SIZE / IDMA_DESC_SIZE)

#define LBSDC_IDMA_ADDR_MASK		(0x3)

/**
 * Runtime Flags
 * @LBSDC_USE_IDMA: Host is prefer to use iDMA
 * @LBSDC_REQ_USE_DMA: Use DMA for this req
 * @LBSDC_DEVICE_DEAD: Device unresponsive
 * @LBSDC_AUTO_CMD12: Auto CMD12 support
 * @LBSDC_SDIO_IRQ_ENABLED: SDIO irq enabled
 * @LBSDC_LP_DISABLED: low power mode disabled
 * @LBSDC_SDIO_MODE: SDIO host for SDIO card
 * @LBSDC_CAP_UHS: Host support UHS
 */
#define LBSDC_USE_IDMA			(1UL << 0)
#define LBSDC_REQ_USE_DMA		(1UL << 2)
#define LBSDC_DEVICE_DEAD		(1UL << 3)
#define LBSDC_AUTO_CMD12		(1UL << 6)
#define LBSDC_SDIO_IRQ_ENABLED		(1UL << 9)
#define LBSDC_LP_DISABLED		(1UL << 10)
#define LBSDC_SDIO_MODE			(1UL << 16)
#define LBSDC_CAP_UHS			(1UL << 17)

/**
 * Event to communicate with bottom half thread
 * @LBSDC_EVENT_FINISH_REQ: notify the bottom half thread to finish request
 */
#define LBSDC_EVENT_FINISH_REQ		(1UL << 0)

/**
 * Card detect debounce
 * @LBSDC_CD_INT_DEBO_MS: interrupt mode debounce time, in ms
 * @LBSDC_CD_INT_DEBO_TICK: interrupt mode debounce time, in tick
 * @LBSDC_CD_POLL_DEBO_MS: polling mode debounce time, in ms
 * @LBSDC_CD_POLL_PERIOD_MS: polling period, in ms
 * @LBSDC_CD_POLL_CNT: polling count for debounce
 * @LBSDC_CD_POLL_PERIOD_TICK: polling period, in tick
 */
#define LBSDC_CD_INT_DEBO_MS		(800)
#define LBSDC_CD_INT_DEBO_TICK		(LBSDC_CD_INT_DEBO_MS*RT_TICK_PER_SECOND/1000)
#define LBSDC_CD_POLL_DEBO_MS		(480)
#define LBSDC_CD_POLL_PERIOD_MS		(60)
#define LBSDC_CD_POLL_CNT		(LBSDC_CD_POLL_DEBO_MS/LBSDC_CD_POLL_PERIOD_MS)
#define LBSDC_CD_POLL_PERIOD_TICK	(LBSDC_CD_POLL_PERIOD_MS*RT_TICK_PER_SECOND/1000)

/**
 * enum card_detect_mode - card detect mode
 * @LBSDC_CD_NONE: no card detect
 * @LBSDC_CD_INTERRUPT: card detect pin support interrupt
 * @LBSDC_CD_POLLING: card detect pin do not support interrupt
 */
enum card_detect_mode {
	LBSDC_CD_NONE      = 0,
	LBSDC_CD_INTERRUPT = 1,
	LBSDC_CD_POLLING   = 2,
};

/**
 * union idma_des0 - idma descriptor desc0
 */
union idma_des0 {
	u32 val;
	struct {
		u32	resv0:1;
		u32	dic:1;
		u32	ld:1;
		u32	fs:1;
		u32	ch:1;
		u32	er:1;
		u32	resv1:24;
		u32	ces:1;
		u32	own:1;
	} bits;
};

/**
 * union idma_des2 - idma descriptor desc2
 */
union idma_des2 {
	u32 val;
	struct {
		u32	bs1:16;
		u32	bs2:16;
	} bits;
};

/**
 * struct idma_des4 - idma descriptor desc4
 */
struct idma_des4 {
	u32		bap1;
};

/**
 * struct idma_des5 - idma descriptor desc5
 */
struct idma_des5 {
	u32		bap1;
};

/**
 * union idma_des6 - idma descriptor desc6
 */
union idma_des6 {
	u32		bap2;
	u32		nda;
};

/**
 * union idma_des7 - idma descriptor desc7
 */
union idma_des7 {
	u32		bap2;
	u32		nda;
};

/**
 * struct idma_desc - idma descriptor
 */
struct idma_desc {
	union idma_des0		des0;
	u32			des1;	/* rsvd */
	union idma_des2		des2;
	u32			des3;	/* rsvd */
	struct idma_des4	des4;
	struct idma_des5	des5;	/* rsvd */
	union idma_des6		des6;
	union idma_des7		des7;	/* rsvd */
};

/**
 * struct lombo_sdc - Runtime info holder for SDC driver.
 * @base: pointer to virtual address of SDC Controller
 * @name: module name
 * @index: module index
 * @flags: runtime flags
 * @clk_gate: pointer to ahb gate clock name
 * @clk_reset: pointer to ahb reset clock name
 * @clk_self: pointer to sdc module clock name
 * @clock: current clock rate, in Hz
 * @data_early: indicate that DTF before CMD_DONE or not
 * @last_cmd: command code of last command
 * @req: current request
 * @cmd: current command
 * @data: current data
 * @blocks: remaining blocks in pio mode
 * @send_auto_stop: indicate that send auto stop command or not
 * @irq_no: irq number
 * @int_mask: interrupt mask
 * @int_status: interrupt status
 * @idma_desc: pointer to idma descriptor table
 * @idma_table_sz: idma descriptor table size
 * @pin_ctrl: pointer to struct pinctrl
 * @cd_pin: card detect pin number
 * @cd_status: last card detect status
 * @cd_mode: card detect mode
 * @cd_debo_cnt: card detect debounce count
 * @cd_debo_timer: card detect debounce timer
 * @lock: mutex lock
 * @bottom_half_thread: pointer to bottom half thread
 * @bottom_half_event: event to communicate with bottom half thread
 * @timeout_timer: request timeout timer
 * @host: pointer to mmc host struct
 */
struct lombo_sdc {
	void			*base;
	char			name[8];
	u32			index;
	u32			flags;

	const char		*clk_gate;
	const char		*clk_reset;
	const char		*clk_self;
	u32			clock;

	u32			data_early;
	u32			last_cmd;

	struct rt_mmcsd_req	*req;
	struct rt_mmcsd_cmd	*cmd;
	struct rt_mmcsd_data	*data;

	u32			blocks;

	u32			send_auto_stop;

	u32			irq_no;
	u32			int_mask;
	u32			int_status;

	struct idma_desc	*idma_desc;
	u32			idma_table_sz;

	struct pinctrl		*pin_ctrl;
	int			cd_pin;
	int			cd_status;
	u32			cd_mode;
	u32			cd_debo_cnt;
	struct rt_timer		cd_debo_timer;

	spinlock_t		lock;
	rt_thread_t		bottom_half_thread;
	struct rt_event		bottom_half_event;
	struct rt_timer		timeout_timer;

	struct rt_mmcsd_host	*host;
};

#endif /* ___SDC__DRV___H___ */
