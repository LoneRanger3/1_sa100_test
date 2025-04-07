/*
 * vic_drv.h - vic driver head file
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

#ifndef __VIC_DRV_H__
#define __VIC_DRV_H__

#include "vic_dev.h"

struct lombo_vic_drv {
	rt_mutex_t lock;
	struct rt_device vic;
	struct viss_dev *device;
	struct rt_device_pm_ops ops;
};

#endif /* __VIC_DRV_H__ */
