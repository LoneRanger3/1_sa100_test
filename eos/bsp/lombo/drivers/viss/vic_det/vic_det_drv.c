/*
 * vic_det_drv.c - sensor input/output detection
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

#define DBG_LEVEL		DBG_ERROR

#include <debug.h>
#include "vic_det_dev.h"
#include "vic_det_drv.h"
#include "media_dev.h"

static struct lombo_vic_det_drv vic_det_drv;

static rt_err_t lombo_vic_det_open(rt_device_t device, rt_uint16_t oflag)
{
	struct lombo_vic_det_drv *vdrv = RT_NULL;
	struct vic_det_dev *vic_det = RT_NULL;

	vdrv = (struct lombo_vic_det_drv *)device->user_data;
	RT_ASSERT(RT_NULL != vdrv);

	vdrv->lock = rt_mutex_create("vic_det", RT_IPC_FLAG_FIFO);
	if (RT_NULL == vdrv->lock)
		return -RT_ERROR;

	vic_det = viss_det_create_links();
	if (RT_NULL == vic_det) {
		LOG_E("lombo_vic_det_open fail.");
		rt_mutex_delete(vdrv->lock);
		vdrv->lock = RT_NULL;
		return -RT_ERROR;
	}
	vdrv->device = vic_det;
	return RT_EOK;
}

static rt_err_t lombo_vic_det_close(rt_device_t device)
{
	struct lombo_vic_det_drv *vdrv = RT_NULL;

	vdrv = (struct lombo_vic_det_drv *)device->user_data;
	RT_ASSERT(RT_NULL != vdrv);
	if (RT_NULL != vdrv->device) {
		viss_det_destory_links(vdrv->device);
		vdrv->device = RT_NULL;
	}
	if (RT_NULL != vdrv->lock) {
		rt_mutex_delete(vdrv->lock);
		vdrv->lock = RT_NULL;
	}
	LOG_D("lombo_vic_det_close");

	return RT_EOK;
}

static rt_err_t lombo_vic_det_control(rt_device_t device, int cmd, void *args)
{
	struct lombo_vic_det_drv *vdrv = RT_NULL;
	rt_err_t ret = 0;

	vdrv = (struct lombo_vic_det_drv *)device->user_data;
	RT_ASSERT(RT_NULL != vdrv);

	if (RT_NULL == vdrv->lock)
		return -RT_EINVAL;
	rt_mutex_take(vdrv->lock, RT_WAITING_FOREVER);
	if (RT_NULL != vdrv->device)
		ret = vic_det_dev_control(vdrv->device, cmd, args);
	rt_mutex_release(vdrv->lock);

	return ret;
}

void lombo_vic_det_suspend(const struct rt_device *device)
{
	struct lombo_vic_det_drv *vdrv = RT_NULL;

	vdrv = (struct lombo_vic_det_drv *)device->user_data;
	RT_ASSERT(RT_NULL != vdrv);
	LOG_D("lombo vic_det suspend");

	if (vdrv->lock)
		rt_mutex_take(vdrv->lock, RT_WAITING_FOREVER);
	if (RT_NULL != vdrv->device)
		vic_det_dev_suspend(vdrv->device);
	if (vdrv->lock)
		rt_mutex_release(vdrv->lock);
}

void lombo_vic_det_resume(const struct rt_device *device)
{
	struct lombo_vic_det_drv *vdrv = RT_NULL;

	vdrv = (struct lombo_vic_det_drv *)device->user_data;
	RT_ASSERT(RT_NULL != vdrv);
	LOG_D("lombo vic_det resume");

	if (vdrv->lock)
		rt_mutex_take(vdrv->lock, RT_WAITING_FOREVER);
	if (RT_NULL != vdrv->device)
		vic_det_dev_resume(vdrv->device);
	if (vdrv->lock)
		rt_mutex_release(vdrv->lock);
}

static int rt_hw_vic_det_init(void)
{
	rt_memset(&vic_det_drv, 0, sizeof(vic_det_drv));

	/* register vic_det device */
	vic_det_drv.vic_det.type = RT_Device_Class_Miscellaneous;
	vic_det_drv.vic_det.init = RT_NULL;
	vic_det_drv.vic_det.open = lombo_vic_det_open;
	vic_det_drv.vic_det.close = lombo_vic_det_close;
	vic_det_drv.vic_det.control = lombo_vic_det_control;
	vic_det_drv.vic_det.read = RT_NULL;
	vic_det_drv.vic_det.write = RT_NULL;

	vic_det_drv.ops.suspend = lombo_vic_det_suspend;
	vic_det_drv.ops.resume = lombo_vic_det_resume;

	vic_det_drv.vic_det.user_data = (void *)&vic_det_drv;

	/* register graphic device driver */
	rt_device_register(&vic_det_drv.vic_det, "vic_det", RT_DEVICE_FLAG_RDWR |
							RT_DEVICE_FLAG_STANDALONE);
#ifdef RT_USING_PM
	rt_pm_register_device(&vic_det_drv.vic_det, &vic_det_drv.ops);
#endif
	return RT_EOK;
}

#ifdef ARCH_LOMBO_N7V1_CDR
INIT_DEVICE_EXPORT(rt_hw_vic_det_init);
#endif

