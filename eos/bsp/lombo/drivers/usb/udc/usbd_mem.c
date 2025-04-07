/*
 * usbd_mem.c - usb device controller driver code for LomboTech
 * dma subsystem
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
#include <spinlock.h>

DEFINE_SPINLOCK(usbd_men_lock);

static struct td_node *alloc_td_node(struct lombo_udc *udc)
{
	struct td_node *node;
	rt_base_t level;
	int i;

	/* [0] is likely to be unused */
	for (i = 0; i < TD_NODE_MAX_NUM; i++) {
		node = udc->td_node_pool[i];
		if (!node->used) {
			spin_lock_irqsave(&usbd_men_lock, level);
			node->used = 1;
			spin_unlock_irqrestore(&usbd_men_lock, level);
			return node;
		}
	}

	return RT_NULL;
}

static void free_td_node(struct td_node *node)
{
	rt_base_t level;

	spin_lock_irqsave(&usbd_men_lock, level);
	node->used = 0;
	spin_unlock_irqrestore(&usbd_men_lock, level);
}

static struct hw_td *alloc_td(struct lombo_udc *udc)
{
	struct hw_td *td;
	rt_base_t level;
	int i;

	/* [0] is likely to be unused */
	for (i = 0; i < TD_MAX_NUM; i++) {
		td = udc->td_pool[i];
		if (!td->used) {
			spin_lock_irqsave(&usbd_men_lock, level);
			td->used = 1;
			spin_unlock_irqrestore(&usbd_men_lock, level);
			return td;
		}
	}

	return RT_NULL;
}

static void free_td(struct hw_td *td)
{
	rt_base_t level;

	spin_lock_irqsave(&usbd_men_lock, level);
	td->used = 0;
	spin_unlock_irqrestore(&usbd_men_lock, level);
}

static rt_err_t usbd_mem_init(struct lombo_udc *udc)
{
	struct hw_td *td;
	struct td_node *node;
	int i;

	for (i = 0; i < TD_NODE_MAX_NUM; i++) {
		node = rt_zalloc(sizeof(struct td_node));
		if (node == RT_NULL) {
			LOG_E("alloc td fail");
			goto free_td_node;
		}
		/* node->used = 0; */
		udc->td_node_pool[i] = node;
	}

	for (i = 0; i < TD_MAX_NUM; i++) {
		td = rt_zalloc_unca_align(sizeof(struct hw_td), EP_TD_ALIGN);
		if (td == RT_NULL) {
			LOG_E("alloc td fail");
			goto free_td;
		}
		/* td->used = 0; */
		udc->td_pool[i] = td;
	}

	udc->qh_pool = rt_zalloc_unca_align(EP_QUEUE_HEAD_TOTAL_SIZE,
						EP_QUEUE_HEAD_ALIGN);
	if (udc->qh_pool == RT_NULL) {
		LOG_E("alloc qh failed");
		goto free_td;
	}

	return RT_EOK;

free_td:
	for (i = 0; i < TD_MAX_NUM; i++) {
		if (udc->td_pool[i] != RT_NULL) {
			rt_free_unca_align(udc->td_pool[i]);
			udc->td_pool[i] = RT_NULL;
		}
	}

free_td_node:
	for (i = 0; i < TD_NODE_MAX_NUM; i++) {
		if (udc->td_node_pool[i] != RT_NULL) {
			rt_free(udc->td_node_pool[i]);
			udc->td_node_pool[i] = RT_NULL;
		}
	}

	return -RT_ENOMEM;
}

static void usbd_mem_destroy(struct lombo_udc *udc)
{
	int i;

	for (i = 0; i < TD_MAX_NUM; i++) {
		if (udc->td_pool[i] != RT_NULL) {
			rt_free_unca_align(udc->td_pool[i]);
			udc->td_pool[i] = RT_NULL;
		}
	}

	if (udc->qh_pool != RT_NULL) {
		rt_free_unca_align(udc->qh_pool);
		udc->qh_pool = RT_NULL;
	}

	for (i = 0; i < TD_NODE_MAX_NUM; i++) {
		if (udc->td_node_pool[i] != RT_NULL) {
			rt_free(udc->td_node_pool[i]);
			udc->td_node_pool[i] = RT_NULL;
		}
	}
}

