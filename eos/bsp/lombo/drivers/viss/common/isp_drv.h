/*
 * isp_drv.h - isp driver head file
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

#ifndef __ISP_DRV_H__
#define __ISP_DRV_H__

#include "isp_dev.h"

/**
 * struct lombo_isp_drv - lombo isp drv.
 * @isp:	rt_device isp
 * @device:	isp device
 * @ops:	rt_device isp ops
 */
struct lombo_isp_drv {
	rt_int32_t dma_ch;
	rt_int32_t open_count;
	rt_mutex_t lock;
	struct rt_device isp;
	struct viss_dev *device;
	struct rt_device_pm_ops ops;
};

#endif /* __ISP_DRV_H__ */
