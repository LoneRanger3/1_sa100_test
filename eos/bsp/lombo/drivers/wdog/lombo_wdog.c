/*
 * lombo_wdog.c - watchdog driver for LomboTech Socs
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
#include <rtdevice.h>
#include <csp.h>
#include <debug.h>
#include "board.h"
#include "wdog.h"
#include "cfg/config_api.h"
#include <drivers/watchdog.h>

static rt_err_t lombo_wdg_init(rt_watchdog_t *wdt)
{
	u32 value = 0;
	rt_err_t ret = RT_EOK;

	LOG_D("lombo_wdg_init");
	csp_wdog_enable(0);
	csp_clr_wdog_irq_pend();
	csp_wdog_irq_enable(0);

	ret = config_get_u32(WDOG_CONFIG_NAME, "clock-src", &value);
	if (ret) {
		LOG_E("config get [clock-src] failed");
		return -RT_EEMPTY;
	}
	if (CLK_MAX <= value) {
		LOG_E("config [clock-src] argument Invalid");
		return -RT_EINVAL;
	}
	csp_set_wdog_clk(value);

	ret = config_get_u32(WDOG_CONFIG_NAME, "tmrout_period", &value);
	if (ret) {
		LOG_E("config get [tmrout_period] failed");
		return -RT_EEMPTY;
	}
	if (TM_MAX <= value) {
		LOG_E("config [tmrout_period] argument Invalid");
		return -RT_EINVAL;
	}
	csp_wdog_tmrout_period(value);

	csp_wdog_response_mod(WDOG_SYSTEM_RESET_RESP_MOD);
	return RT_EOK;
}

static rt_err_t lombo_wdg_control(rt_watchdog_t *wdt, int cmd, void *arg)
{
	rt_uint32_t timeout = 0;

	LOG_D("lombo_wdg_control: %d", cmd);
	switch (cmd) {
	case RT_DEVICE_CTRL_WDT_SET_TIMEOUT:
		timeout = *((rt_uint32_t *) arg);
		timeout /= 1000;
		if (timeout >= 16)
			csp_wdog_tmrout_period(TM_16S);
		else if (timeout <= 0)
			csp_wdog_tmrout_period(TM_0P5S);
		else
			csp_wdog_tmrout_period(timeout);
		break;
	case RT_DEVICE_CTRL_WDT_KEEPALIVE:
		csp_restart_wdog();
		break;
	case RT_DEVICE_CTRL_WDT_START:
		csp_wdog_enable(1);
		break;
	case RT_DEVICE_CTRL_WDT_STOP:
		csp_wdog_enable(0);
		csp_clr_wdog_irq_pend();
		csp_wdog_irq_enable(0);
		break;
	default:
		return RT_EIO;
	}
	return RT_EOK;
}

static const struct rt_watchdog_ops lombo_wdg_pos = {
	lombo_wdg_init,
	lombo_wdg_control,
};
static rt_watchdog_t lombo_wdg;

int rt_hw_wdg_init(void)
{
	LOG_D("rt_hw_wdg_init");
	lombo_wdg.ops = &lombo_wdg_pos;
	rt_hw_watchdog_register(&lombo_wdg, WDOG_DEV_NAME, 0, RT_NULL);
	return RT_EOK;
}

INIT_BOARD_EXPORT(rt_hw_wdg_init);
