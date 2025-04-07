/*
 * vic_dev.h - sensor input/output detection driver head file
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

#ifndef __VIC_DET_DEV_H__
#define __VIC_DET_DEV_H__

#include "viss.h"

extern struct vic_det_dev *vic_det_dev_create(void);
extern void vic_det_dev_destory(struct vic_det_dev *vic_det);
extern rt_err_t vic_det_dev_control(struct vic_det_dev *vic_det,
				rt_int32_t cmd, void *para);
extern void vic_det_dev_suspend(struct vic_det_dev *vic_det);
extern void vic_det_dev_resume(struct vic_det_dev *vic_det);
extern struct vic_det_status vic_det_dev_get_sta(void);
#endif

