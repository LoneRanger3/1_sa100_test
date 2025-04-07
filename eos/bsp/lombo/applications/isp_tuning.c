/*
 * isp_tuning.c - the main entry for isp_tuning
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

#ifndef ISP_TUNING
#define ISP_TUNING

#include <rtthread.h>
#include <debug.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef ARCH_LOMBO_N7
#ifdef RT_USING_ISP
#include "viss.h"
#endif
#endif

#define ISP_TUNING_DEV_NAME "isp"
rt_device_t  isp_tuning_dev;

static int isp_tuning(int argc, char **argv)
{
	rt_err_t ret = 0;

	LOG_I("isp_tuning start.\n");

#ifdef ARCH_LOMBO_N7
#ifdef RT_USING_ISP

	isp_tuning_dev = rt_device_find(ISP_TUNING_DEV_NAME);
	if (RT_NULL == isp_tuning_dev)
		LOG_E("Request %s device fail.", ISP_TUNING_DEV_NAME);

	if (argc >= 2 && !strncmp(argv[1], "update_all", 10)) {
		LOG_I("isp_tuning update_all.\n");
		ret = rt_device_control(isp_tuning_dev,
			ISP_CMD_UPDATE_CONFIG, (void *)0);
	}

#endif
#endif

	LOG_I("isp_tuning end.\n");

	return 0;
}
MSH_CMD_EXPORT(isp_tuning, "isp tuning");
#endif /* ISP_TUNING */
