/*
 * mcsi_dev.h - mcsi demcsie driver head file
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

#ifndef __MCSI_DEV_H__
#define __MCSI_DEV_H__

#include "viss.h"

extern struct viss_dev *mcsi_dev_create(rt_int32_t freq);
extern void mcsi_dev_destory(struct viss_dev *mcsi);
extern rt_err_t mcsi_dev_control(struct viss_dev *mcsi,
				rt_int32_t cmd, void *para);
extern void mcsi_dev_suspend(struct viss_dev *mcsi);
extern void mcsi_dev_resume(struct viss_dev *mcsi);
extern void mcsi_dev_reset(struct viss_dev *mcsi, rt_int32_t en);

#endif /* __MCSI_DEV_H__ */

