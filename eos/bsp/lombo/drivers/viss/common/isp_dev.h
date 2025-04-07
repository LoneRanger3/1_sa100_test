/*
 * isp_dev.h - isp device driver head file
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

#ifndef __ISP_DEV_H__
#define __ISP_DEV_H__

#include "viss.h"

#define ISP_CTRL			0x400
#define ISP_INT_EN			0x5bc
#define ISP_RIS				0x5c0
#define ISP_ICR				0x5c8

#define TNR_CTRL			0x1100
#define ISP_TNR_INPUT_MEM_Y_ADD		0x1138
#define ISP_TNR_INPUT_MEM_CB_ADD	0x113C
#define ISP_TNR_INPUT_MEM_CR_ADD	0x1140
#define ISP_TNR_OUTPUT_MEM_Y_ADD	0x1144
#define ISP_TNR_OUTPUT_MEM_CB_ADD	0x1148
#define ISP_TNR_OUTPUT_MEM_CR_ADD	0x114C

#define ISP_DMA_CTRL			0x1400
#define ISP_DMA_INIT			0x1404
#define ISP_DMA_MAIN_Y_BASE_ADD		0x1408
#define ISP_DMA_MAINPATH_Y_SIZE		0x140c
#define ISP_DMA_MAIN_CB_BASE_ADD	0x141c
#define ISP_DMA_MAINPATH_CB_SIZE	0x1420
#define ISP_DMA_MAIN_CR_BASE_ADD	0x142c
#define ISP_DMA_MAINPATH_CR_SIZE	0x1430
#define ISP_DMA_SUB_Y_BASE_ADD		0x143c
#define ISP_DMA_SUBPATH_Y_SIZE		0x1440
#define ISP_DMA_SUB_CB_BASE_ADD		0x1450
#define ISP_DMA_SUBPATH_CB_SIZE		0x1454
#define ISP_DMA_SUB_CR_BASE_ADD		0x1460
#define ISP_DMA_SUBPATH_CR_SIZE		0x1464

#define ISP_DMA_IRQ_STATUS		0x14FC
#define ISP_DMA_IRQ_STATUS_MASK		0x1500
#define ISP_DMA_IRQ_CLR			0x1504
#define ISP_DMA_IRQ_EN			0x1508
#define ISP_DMA_STATUS			0x150C
#define ISP_DMA_STATUS_CLR		0x1510

#define ISP_BGM_STAT_MEM_ADD		0x2f88
#define ISP_ISM_STAT_MEM_ADD		0x2f24

extern struct rt_event isp_event;
extern struct isp_event_data event_data;

rt_sem_t isp_drv_wait_sem; /* wait video input */
rt_sem_t isp_cap_wait_prev_sem; /* wait isp preview channel start */
rt_sem_t isp_wait_reset_sem; /* wait isp reset finish */
rt_sem_t isp_wait_restart_sem; /* wait isp restart finish */
rt_sem_t isp_wait_wdg_sem; /* wait isp irq feed dog */
rt_sem_t isp_wait_exit_sem; /* wait isp exit */
rt_sem_t isp_wait_lib_start; /* wait isp lib start finish*/
rt_sem_t isp_wait_lib_stop; /* wait isp lib stop finish*/

extern rt_mq_t isp_irq_mq;

extern rt_uint32_t isp_reg_readl(rt_uint32_t add);
extern void isp_reg_writel(rt_uint32_t add, rt_uint32_t val);
extern void isp_dma_sp_buf_addr(struct viss_buffer *buffer);
extern void isp_dma_mp_buf_addr(struct viss_buffer *buffer);
extern rt_err_t isp_reset_hw(u32 val);

extern rt_err_t __isp_dev_init(struct viss_dev *isp);
extern void __isp_dev_exit(struct viss_dev *isp);
extern void __isp_dev_stop_hw(struct viss_dev *isp);

extern struct viss_dev *isp_dev_create(rt_int32_t freq);
extern void isp_dev_destory(struct viss_dev *isp);
extern rt_err_t isp_dev_control(struct viss_dev *isp,
				rt_int32_t cmd, void *para);
extern void isp_dev_suspend(struct viss_dev *isp);
extern void isp_dev_resume(struct viss_dev *isp);

#endif /* __ISP_DEV_H__ */

