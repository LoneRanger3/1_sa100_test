/*
 * pm.c - power management module
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

#include <stdio.h>
#include <stdlib.h>
#include "pm.h"
#include <cache_op.h>
#include <wdog.h>
#include <rtc_csp.h>
#include "platsmp.h"
#include "system/system_mq.h"
#include "power_drv.h"
#include "../drivers/timer/timer.h"

#include "mmu.h"
#include "cp15.h"
#include "gic.h"

static sram_standby_fn sram_standby;
static sram_deepslp_fn sram_deepslp_suspend;

/* for standby synchronization */
struct rt_semaphore sleep_lock;

void disable_all_int(void)
{
	int num = 7, i;
	u32 offset = 0x4;
	u32 addr;

	for (i = 0; i < num; i++) {
		addr = VA_GICD_ICENABLER0 + offset * i;
		WRITEREG32(addr, 0xffffffff);
	}
}

void lb_hw_power_off(void)
{
	rt_base_t flags;
	u32 interval;

	/* clear rtc flag for power off */
	csp_rtc_clear_ddr_pad();

	/* disable all interrupt */
	flags = rt_hw_local_irq_disable();
	disable_all_int();

	/* set timer interrupt enable */
	u32 val = BIT(27);
	WRITEREG32(VA_GICD_ISENABLER0, val);

	/* stop timer */
	csp_vtmr_stop();

	/* interval 160ms */
	interval = (USEC_PER_SEC / RT_TICK_PER_SECOND) * 24 * 16;
	asm volatile("mcr p15, 0, %0, c14, c3, 0" : : "r"(interval));

	/* clear local timer pending */
	WRITEREG32(VA_GICD_ICPENDR0, val);

	/* start timer 160ms */
	csp_vtmr_start();

	WRITEREG32(VA_GPIO_GPIOD_FUNC_R0, 0x2);
	while (1) {
		/* ddr power off */
		csp_rtc_set_pwr_ddr_disable();

		/* cpu and system power off */
		csp_rtc_pwr_en_disable();

		asm volatile("mcr p15, 0, %0, c14, c3, 0" : : "r"(interval));
		asm volatile("dsb\n\t" "wfi\n\t" : : : "memory");
	}
}


void lb_hw_reboot(void)
{
	/* clear rtc flag for power off */
	csp_rtc_clear_ddr_pad();

	/* disable wdog */
	csp_wdog_enable(0);

	/* disable wdog interrupt */
	csp_wdog_irq_enable(0);

	/* set wdog clksrc to 32k */
	csp_set_wdog_clk(0);

	/* set wdog timeout */
	csp_wdog_tmrout_period(0); /* timeout: 0.5s */

	/* reponse mod: gen system reset */
	csp_wdog_response_mod(0);

	/* enable wdog */
	csp_wdog_enable(1);

	/* wait for reset */
	while (1)
		;
}
RTM_EXPORT(lb_hw_reboot);

void pm_enter(struct rt_pm *pm)
{
	u32 cpu_timestamp[2] = {0};
	int bin_size;

	if (PM_MODE_STANDBY == pm->current_mode) {
		LOG_I("standby -> suspend..");

		bin_size = (int)&suspend_end - (int)&suspend_start;
		if (bin_size >= STANDBY_CODE_SIZE - STANDBY_STACK_SIZE) {
			LOG_E("standby image size(0x%08x) too large!", bin_size);
			return;
		}

		/* copy standby code to sram */
		memcpy((void *)STANDBY_CODE_START, &suspend_start, bin_size);

		/* init sram stack for overflow check */
		memset((void *)(STANDBY_STACK_END - STANDBY_STACK_SIZE),
			'#', STANDBY_STACK_SIZE);

		/*
		 * invalidate icache after copy code
		 * NOTE: donot disble icache here, for the performance consideration
		 */
		rt_hw_cpu_icache_inval_all();

		/* disable dcache */
		rt_hw_cpu_dcache_disable();

		/* clean & invalidate dcache */
		rt_hw_cpu_dcache_flush_all();

		/* make sure sram phys and virt has been mapped to the same */
		sram_standby = (sram_standby_fn)STANDBY_CODE_START;
		sram_standby(pm->current_mode, STANDBY_DDR_PARA, STANDBY_DDR_TIMING);

		LOG_I("standby -> resumed!");
	} else if (PM_MODE_DEEPSLP == pm->current_mode) {
		LOG_I("deepsleep -> suspend..");

		bin_size = (int)&suspend_end - (int)&suspend_start;
		if (bin_size >= SUSPEND_CODE_SIZE - SUSPEND_STACK_SIZE) {
			LOG_E("suspend image size(0x%08x) too large!", bin_size);
			return;
		}

		/* backup cpu timestamp for tick count */
		get_cpu_timestamp(cpu_timestamp);

		/* bakeup cpu/gic/.. status, will be restored in lombo_cpu_resume */
		deepslp_cpu_suspend();

		/* copy standby code to sram */
		memcpy((void *)SUSPEND_CODE_START, &suspend_start, bin_size);

		/* init sram stack for overflow check */
		memset((void *)(SUSPEND_STACK_END - SUSPEND_STACK_SIZE),
			'#', SUSPEND_STACK_SIZE);

		/*
		 * invalidate icache after copy code
		 * NOTE: donot disble icache here, for the performance consideration
		 */
		rt_hw_cpu_icache_inval_all();

		/* disable dcache */
		rt_hw_cpu_dcache_disable();

		/* clean & invalidate dcache */
		rt_hw_cpu_dcache_flush_all();

		/*
		 * set the deep sleep flag in rtc reg region, and save the
		 * resume func addr to rtc regs
		 */
		rtc_set_deep_slp_flag(1);
		rtc_set_deep_slp_exit((u32)virt_to_phys(lombo_deepslp_resume));

		/* make sure sram phys and virt has been mapped to the same */
		sram_deepslp_suspend = (sram_deepslp_fn)SUSPEND_CODE_START;

		/* just suspend, not need ddr_para/timing for resume */
		lombo_cpu_deepslp(pm->current_mode, 0, 0, sram_deepslp_suspend);

		/*
		 * clr the rtc reg that store the resume func addr, and clr the
		 * deep sleep flag in rtc reg region
		 */
		rtc_set_deep_slp_exit(0);
		rtc_set_deep_slp_flag(0);

		/* init uart for print */
		uart_init();

		/* recover cpu timestamp */
		set_cpu_timestamp(cpu_timestamp);

		LOG_I("deepsleep -> resumed!");
	} else if (PM_MODE_SHUTDOWN == pm->current_mode) {
		LOG_I("power off..");

		lb_hw_power_off();
	} else if (PM_MODE_REBOOT == pm->current_mode) {
		LOG_I("reboot..");

		lb_hw_reboot();
	} else {
		LOG_E("pm mode %d!", pm->current_mode);
	}
}

void pm_exit(struct rt_pm *pm)
{
	LOG_I("start");

	if (PM_MODE_STANDBY == pm->current_mode
		|| PM_MODE_SHUTDOWN == pm->current_mode) {
		LOG_I("pm->current_mode %d", (int)pm->current_mode);

		/* now the system exit from sleep */
		rt_sem_release(&sleep_lock);
	}

	LOG_I("end");
}

void pm_frequency_change(struct rt_pm *pm, rt_uint32_t frequency)
{
	LOG_I("frequency %d", (int)frequency);
}

void pm_timer_start(struct rt_pm *pm, rt_uint32_t timeout)
{
	LOG_I("timeout %d", (int)timeout);
}

void pm_timer_stop(struct rt_pm *pm)
{
	LOG_I("");
}

rt_tick_t pm_timer_get_tick(struct rt_pm *pm)
{
	LOG_I("");
	return 0;
}

const struct rt_pm_ops pm_ops = {
	pm_enter,
	pm_exit,
#if PM_RUN_MODE_COUNT > 1
	pm_frequency_change,
#endif
	pm_timer_start,
	pm_timer_stop,
	pm_timer_get_tick
};

void pm_init(void)
{
	rt_sem_init(&sleep_lock, "standby", 0, RT_IPC_FLAG_FIFO);

	rt_system_pm_init(&pm_ops, 0, NULL);

#if defined(ARCH_LOMBO_N7V1_CDR) && defined(__EOS__RELEASE__MP__)
	/* set hw fast boot flag according to img type */
	if (BOOT_FLAG_FAST == boot_flag)
		rtc_store_en_flag_set(1); /* hw enable fast boot */
	else
		rtc_store_en_flag_set(0);
#endif
}

void rt_pm_standby(void)
{
	/* request standby mode: PM_MODE_STANDBY */
	rt_pm_request(PM_MODE_STANDBY);

	/* release current PM_MODE_RUNNING */
	rt_pm_release(PM_MODE_RUNNING);

	/*
	 * now the system enter PM_MODE_STANDBY (standby):
	 *   rt_thread_idle_entry -> rt_pm_enter ->pm_enter
	 */

	LOG_I("");

	/* wait for wakup */
	rt_sem_take(&sleep_lock, RT_WAITING_FOREVER);

	LOG_I("");

	/*
	 * the system wakeup from standby (by power key or other wakup sources):
	 *   rt_thread_idle_entry -> rt_pm_enter ->pm_enter .. -> rt_pm_exit
	 */

	/* request PM_MODE_RUNNING mode */
	rt_pm_request(PM_MODE_RUNNING);

	/* release PM_MODE_STANDBY */
	rt_pm_release(PM_MODE_STANDBY);
}
RTM_EXPORT(rt_pm_standby);

/**
 * deepslp_cpu_suspend: cpu suspend before deepsleep
 *
 * Save gic, etc.
 */
void deepslp_cpu_suspend(void)
{
	rt_hw_tick_suspend();

	gic_suspend();
}

/**
 * deepslp_cpu_resume1: cpu resume from deepsleep, stage 1
 *
 * Init cache, mmu, tlb, ttbr0/1, ttbcr
 */
void deepslp_cpu_resume1(void)
{
	u32 val = 0;

	/*
	 * initialize cache, mmu, tlb, ttbr0/1, ttbcr
	 */

	/* rt_cpu_mmu_disable(); alreadly done in lombo_cpu_resume */

	/* step1: invalidate cache if needed */
	rt_hw_cpu_icache_disable();
	rt_hw_cpu_icache_inval_all();
	rt_hw_cpu_dcache_disable();
	rt_hw_cpu_dcache_flush_all();

	/*
	 * step2: invalidate entire branch predictor array,
	 * to improve the efficiency of jump instructions
	 */
	n7_inv_branch_pred_cache();

#ifdef ENABLE_ICACHE
	/* step3: enable icache */
	rt_hw_cpu_icache_enable();
#endif

#ifdef ENABLE_MMU
	/* step4: mmu preparation before enable */

	/* initialize every domain entry to b01 (client) */
	rt_hw_set_domain_register(0x55555555);

	/* using TTBR0 for translation, not use TTBR1 */
	n7_set_ttbcr(0);

	/* set TTBR0 register */
	val = (u32)page_table_l1;
	val |= 0x6A;
	rt_cpu_tlb_set((unsigned long *)val);

	/* step5: set ACTLR.SMP bit before mmu & dcache enabled */
	n7_join_smp();

	/* step6: enable mmu */
	rt_cpu_mmu_enable();
#endif /* ENABLE_MMU */

	/* step7: enable branch prediction */
	n7_branch_prediction_enable();

#ifdef ENABLE_DCACHE
	/* step8: enable dcache */
	rt_hw_cpu_dcache_enable();
#endif
}

/**
 * deepslp_cpu_resume2: cpu resume from deepsleep, stage 2
 *
 * Init the vector, gic, tick, etc.
 */
void deepslp_cpu_resume2(void)
{
	rt_hw_vector_init();

	gic_resume();

	rt_hw_tick_resume();
}

void rt_pm_deepsleep(void)
{
	/* request deepsleep mode: PM_MODE_DEEPSLP */
	rt_pm_request(PM_MODE_DEEPSLP);

	/* release current PM_MODE_RUNNING */
	rt_pm_release(PM_MODE_RUNNING);

	/*
	 * now the system enter PM_MODE_DEEPSLP (deepsleep):
	 *   rt_thread_idle_entry -> rt_pm_enter ->pm_enter
	 */

	LOG_I("");

	/* wait for wakup */
	rt_sem_take(&sleep_lock, RT_WAITING_FOREVER);

	LOG_I("");

	/*
	 * the system wakeup from deepsleep (by power key or other wakup sources):
	 *   rt_thread_idle_entry -> rt_pm_enter ->pm_enter .. -> rt_pm_exit
	 */

	/* request PM_MODE_RUNNING mode */
	rt_pm_request(PM_MODE_RUNNING);

	/* release PM_MODE_DEEPSLP */
	rt_pm_release(PM_MODE_DEEPSLP);
}
RTM_EXPORT(rt_pm_deepsleep);

void rt_pm_shutdown(void)
{
	/* request shutdown mode: PM_MODE_SHUTDOWN */
	rt_pm_request(PM_MODE_SHUTDOWN);

	/* release current PM_MODE_RUNNING */
	rt_pm_release(PM_MODE_RUNNING);

	/*
	 * now the system enter PM_MODE_SHUTDOWN (shutdown):
	 *   rt_thread_idle_entry -> rt_pm_enter ->pm_enter
	 */

	LOG_I("");

	/*
	 * wait for wakup
	 *
	 * never waited, because power off during pm_enter,
	 *   and the pm_exit has no chance to be executed
	 */
	rt_sem_take(&sleep_lock, RT_WAITING_FOREVER);

	/* never run here.. */
	RT_ASSERT(0);
}
RTM_EXPORT(rt_pm_shutdown);

void rt_pm_reboot(void)
{
	int ret = LB_USERDEF_SYSMSG_RETURN_HOME;

	lb_system_mq_send(LB_SYSMSG_USERDEF, &ret, sizeof(int), 0);
	ret = lb_system_mq_syncsem_take(800);
	rt_kprintf("syncsem ret:%d %d\n", ret, __LINE__);
	/* request reboot mode: PM_MODE_REBOOT */
	rt_pm_request(PM_MODE_REBOOT);

	/* release current PM_MODE_RUNNING */
	rt_pm_release(PM_MODE_RUNNING);

	/*
	 * now the system enter PM_MODE_REBOOT (reboot):
	 *   rt_thread_idle_entry -> rt_pm_enter ->pm_enter
	 */

	LOG_I("");

	/*
	 * wait for wakup
	 *
	 * never waited, because power off during pm_enter,
	 *   and the pm_exit has no chance to be executed
	 */
	rt_sem_take(&sleep_lock, RT_WAITING_FOREVER);

	/* never run here.. */
	RT_ASSERT(0);
}
RTM_EXPORT(rt_pm_reboot);

long reboot(int argc, char **argv)
{
	if ((argc >= 2) && !rt_strcmp(argv[1], "pol")) {
		LOG_I("to enter pol");
		rtc_pol_flag_set();
	}
	rt_pm_reboot();
	return 0;
}
FINSH_FUNCTION_EXPORT(reboot, system reboot);
MSH_CMD_EXPORT(reboot, system reboot);

long reset(int argc, char **argv)
{
	if ((argc >= 2) && !rt_strcmp(argv[1], "pol")) {
		LOG_I("to enter pol");
		rtc_pol_flag_set();
	}
	rt_pm_reboot();
	return 0;
}
FINSH_FUNCTION_EXPORT(reset, reboot system);
MSH_CMD_EXPORT(reset, reboot system);

#ifdef ARCH_LOMBO_N7V1
long sys_shutdown(void)
{
	int ret = LB_USERDEF_SYSMSG_RETURN_HOME;

	rt_hw_power_disable_reset();
	lb_system_mq_send(LB_SYSMSG_USERDEF, &ret, sizeof(int), 0);
	ret = lb_system_mq_syncsem_take(800);
	rt_kprintf("syncsem ret:%d %d\n", ret, __LINE__);
	rt_pm_shutdown();

	return 0;
}
FINSH_FUNCTION_EXPORT(sys_shutdown, system power off);
MSH_CMD_EXPORT(sys_shutdown, system power off);

long cpu(int argc, char **argv)
{
	int cpu_id;

	if (3 != argc) {
		LOG_E("para err, argc %d", argc);
		return -RT_ERROR;
	}

	cpu_id = atoi(argv[2]);

	if (!rt_strcmp(argv[1], "on")) {
		LOG_I("power on cpu%d", cpu_id);

		cpu_up(cpu_id);
	} else if (!rt_strcmp(argv[1], "off")) {
		LOG_I("power off cpu%d", cpu_id);

		cpu_down(cpu_id);
	} else {
		LOG_E("argv[1] %s invalid", argv[1]);
		return -RT_ERROR;
	}

	return 0;
}
FINSH_FUNCTION_EXPORT(cpu, enable or disable a cpu);
MSH_CMD_EXPORT(cpu, enable or disable a cpu);
#endif /* ARCH_LOMBO_N7V1 */

long sys_standby(void)
{
	int ret = LB_USERDEF_SYSMSG_RETURN_HOME;

	rt_hw_power_disable_reset();
	lb_system_mq_send(LB_SYSMSG_USERDEF, &ret, sizeof(int), 0);
	ret = lb_system_mq_syncsem_take(800);
	rt_kprintf("syncsem ret:%d %d\n", ret, __LINE__);
	rt_pm_standby();

	return 0;
}
FINSH_FUNCTION_EXPORT(sys_standby, system enter standby);
MSH_CMD_EXPORT(sys_standby, system enter standby);

long deepsleep(void)
{
	int ret = LB_USERDEF_SYSMSG_RETURN_HOME;

	rt_hw_power_disable_reset();
	lb_system_mq_send(LB_SYSMSG_USERDEF, &ret, sizeof(int), 0);
	ret = lb_system_mq_syncsem_take(800);
	rt_kprintf("syncsem ret:%d %d\n", ret, __LINE__);
	rt_pm_deepsleep();

	return 0;
}
FINSH_FUNCTION_EXPORT(deepsleep, system enter deepsleep);
MSH_CMD_EXPORT(deepsleep, system enter deepsleep);

