/*
 * isp_dma_dev.h - isp device capture driver head file
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

#ifndef __ISP_DMA_DEV_H__
#define __ISP_DMA_DEV_H__

#include "viss.h"

extern struct rt_event isp_event;
extern struct lombo_viss lombo_viss_dev;
extern void __isp_dma_ch_isr_handle(rt_int32_t group_id);
extern rt_err_t __isp_dma_dev_init(struct viss_dev *isp_dma);
extern void __isp_dma_dev_exit(struct viss_dev *isp_dma);
extern void __isp_dma_dev_stop_hw(struct viss_dev *isp_prev);
extern void __isp_dma_dev_start_streaming(rt_int32_t dma_ch);
extern struct viss_dev *isp_dma_dev_create(rt_uint32_t group_id);
extern void isp_dma_dev_destory(struct viss_dev *isp);
extern rt_err_t isp_dma_dev_control(struct viss_dev *isp,
				rt_int32_t cmd, void *para);
extern void isp_dma_dev_suspend(struct viss_dev *isp);
extern void isp_dma_dev_resume(struct viss_dev *isp);

#endif /* __ISP_DMA_DEV_H__ */

