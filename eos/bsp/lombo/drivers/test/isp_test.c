/*
 * viss_test.c - sensor test code for LomboTech
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

#include <dfs_posix.h>
#include <pthread.h>
#include "viss.h"
#include "lombo_disp.h"

int time_start;
int time_temp;
/* struct vply_test - disp module */
struct vply_test {
	rt_device_t disp_device;
	rt_device_disp_ops_t *disp_ops;
	struct rt_device_graphic_info info;
};

/* struct viss_dev_test - viss module */
struct viss_dev_test {
	rt_device_t dev;
	rt_int32_t exit_flag;
	struct viss_frame_queue frame_queue;
	rt_sem_t wait_frm_sem; /* wait video input */
	rt_sem_t isp_drv_wait_sem; /* wait video input */
	rt_int32_t frm_count; /* use to save video frame */
	rt_int32_t disp_mode;
	disp_ctl_t *layer;
	char win_name[8];
	dc_win_data_t win_data;
	struct vply_test *vply_hdl;
	pthread_t vply_thread_id;
};

struct isp_dev_test {
	struct viss_dev_test *prev[2];
	struct viss_dev_test *cap[2];
	rt_mutex_t lock;
};


#define MAX_USE_CHANNEL_NUM  (1)
#define CACHE_BUFFER_CHANNEL_PER  (8)    /* number of buffers used per channel  */
#define VISS_DMA_ADDR_ALIGN_SIZE   (4*1024)  /* isp dma need 8 or 16 byte align */
#define SCREEN_WIDTH  (800)
#define SCREEN_HEIGHT  (480)
#define FRONT_LAYER_NAME	"front"
#define BACK_LAYER_NAME		"back"
#define FRONT_SCALE_NAME	"fscale"
#define BACK_SCALE_NAME		"bscale"

#define FRONT_LAYER_WIN_INDEX	DPU_DC_WIN_INDEX2
#define BACK_LAYER_WIN_INDEX	DPU_DC_WIN_INDEX3

#define CAM_OUT_FRAME_VGA    (480)  /* 640*480 */
#define CAM_OUT_FRAME_720P   (720)  /* 1280*720 */
#define CAM_OUT_FRAME_1080P  (1080) /* 1920*1080 */

#define VISS_VIDEO_WIN_NAME "viss_video"
#define VISS_ISP_DEV_NAME "isp"
#define VISS_ISP_CAP_DEV_NAME "isp_cap.0"
#define VISS_ISP_PREV_DEV_NAME "isp_prev.0"
#define VISS_MCSI_DEV_NAME "mcsi"

/* front camera use isp, back camera use mcsi */
#define VISS_PREVIEW_BACK_AND_FRONT	(0)
#define VISS_PREVIEW_FRONT		(1)
#define VISS_PREVIEW_BACK		(2)

#define VISS_PREVIEW_MODE	VISS_PREVIEW_FRONT

#define DEBUG_VIDEO_FRAME	(0)
#define PREVIEW_FRAME		(1)
#define CAPTURE_FRAME		(1)
#define CAPTURE_FRAME_NUM	(50)

struct isp_dev_test lombo_isp;

static void __viss_isp_dev_exit(struct viss_dev_test *test_dev, rt_int32_t channel_num);
int frame_cnt_temp1;

static rt_int32_t __calc_frame_size(rt_uint32_t pix_format, struct viss_rect *rect,
	rt_int32_t *size0, rt_int32_t *size1, rt_int32_t *size2)
{
	rt_int32_t size[3] = {0};

	if (VISS_PIX_FMT_NV12 == pix_format) {
		size[0] = rect->width * rect->height;
		size[1] = size[0] >> 1;

		*size0 = size[0];
		*size1 = size[1];
		*size2 = 0;
	}

	return RT_EOK;
}

static struct viss_dev_test *__viss_dev_create(char *name)
{
	struct viss_dev_test *test_dev = RT_NULL;
	rt_err_t ret = 0;

	test_dev = rt_zalloc(sizeof(struct viss_dev_test));
	if (RT_NULL == test_dev) {
		LOG_E("Malloc fail. %d", sizeof(struct viss_dev_test));
		return RT_NULL;
	}
	test_dev->dev = rt_device_find(name);
	if (RT_NULL == test_dev->dev) {
		LOG_E("Request %s device fail.", name);
		goto EXIT;
	}
	ret = rt_device_open(test_dev->dev, 0);
	if (RT_EOK != ret) {
		LOG_E("Open %s device fail. ret:%d.", name, ret);
		goto EXIT;
	}
	LOG_E("  ");

	return test_dev;

EXIT:
	if (test_dev->dev)
		rt_device_close(test_dev->dev);
	rt_free(test_dev);
	return RT_NULL;
}

static void __viss_dev_destory(struct viss_dev_test **test_dev_p)
{
	struct viss_dev_test *test_dev = *test_dev_p;
	if (test_dev) {
		if (test_dev->dev)
			rt_device_close(test_dev->dev);
		rt_free(test_dev);
		*test_dev_p = RT_NULL;
	}
}

static rt_err_t __viss_dev_init(struct viss_dev_test *test_dev, rt_uint32_t out_fmt,
			rt_uint32_t frame_size, rt_int32_t channel_num)
{
	rt_err_t ret = 0;
	ret = rt_device_control(test_dev->dev, VISS_CMD_INIT,
					(void *)channel_num);
	if (RT_EOK != ret) {
		LOG_E("VISS_CMD_INIT. ret:%d.", ret);
		goto EXIT;
	}
	return RT_EOK;

EXIT:
	return -RT_ERROR;
}

static rt_err_t __viss_isp_dev_capture_on(struct viss_dev_test *test_dev,
		rt_uint32_t out_fmt, rt_uint32_t frame_size, rt_int32_t channel_num)
{
	rt_err_t ret = 0;
	rt_int32_t i = 0;
	rt_int32_t size[3] = {0};
	struct dev_all_mode all_mode;
	struct dev_mode *use_mode = RT_NULL;
	struct dev_mode *cur_mode = RT_NULL;
	if (RT_NULL == test_dev)
		return -RT_ERROR;
	rt_device_control(test_dev->dev, VISS_CMD_GET_WAIT_FRAME_SEM,
			(void *)&test_dev->wait_frm_sem);
	if (RT_NULL == test_dev->wait_frm_sem) {
		LOG_E("wait_frm_sem is null.");
		goto EXIT;
	}

	rt_device_control(test_dev->dev, ISP_CMD_GET_WAIT_DRV_SEM,
			(void *)&test_dev->isp_drv_wait_sem);
	if (RT_NULL == test_dev->isp_drv_wait_sem) {
		LOG_E("isp_drv_wait_sem is null.");
		goto EXIT;
	}

	/* 2. set mode */
	memset(&all_mode, 0, sizeof(all_mode));
	rt_device_control(test_dev->dev, VISS_CMD_GET_ALL_MODE,
			(void *)&all_mode);
	for (i = 0; i < all_mode.num; i++) {
#if 0
		LOG_E("mode %d: w:%d h:%d outfmt:%#x, inpfmt:%#x, index:%d.", i,
			all_mode.mode[i].frame_size.width,
			all_mode.mode[i].frame_size.height,
			all_mode.mode[i].out_fmt,
			all_mode.mode[i].inp_fmt,
			all_mode.mode[i].index);
#endif
		if ((all_mode.mode[i].out_fmt == out_fmt) &&
			(all_mode.mode[i].frame_size.height == frame_size)) {
			LOG_E("Find dev mode. i:%d.", i);
			use_mode = &all_mode.mode[i];
			break;
		}
	}
	if (i >= all_mode.num) {
		LOG_E("Can't find output format %#x mode.", out_fmt);
		goto EXIT;
	}
	rt_device_control(test_dev->dev, VISS_CMD_SET_MODE,
			(void *)use_mode->index);
	rt_device_control(test_dev->dev, VISS_CMD_GET_CUR_MODE,
			(void *)&cur_mode);

	if (cur_mode != use_mode) {
		LOG_E("Device mode error.");
		goto EXIT;
	}
	/* 3. malloc frame buffer */
	__calc_frame_size(use_mode->out_fmt, &use_mode->frame_size,
		&size[0], &size[1], &size[2]);
	if (size[0] <= 0) {
		LOG_E("Calc frame size error. %d.", size[0]);
		goto EXIT;
	}
	memset(&test_dev->frame_queue, 0, sizeof(test_dev->frame_queue));
	/* only one channel */
	test_dev->frame_queue.all_num = CACHE_BUFFER_CHANNEL_PER * channel_num;
	test_dev->frame_queue.frame = rt_zalloc(test_dev->frame_queue.all_num *
					sizeof(struct viss_buffer));
	for (i = 0; i < test_dev->frame_queue.all_num; i++) {
		test_dev->frame_queue.frame[i].paddr[0].align = VISS_DMA_ADDR_ALIGN_SIZE;
		if (size[0] > 0) {
			test_dev->frame_queue.frame[i].paddr[0].y =
				(rt_uint32_t)rt_malloc_align(size[0],
				VISS_DMA_ADDR_ALIGN_SIZE);
		}
		if (size[1] > 0) {
			test_dev->frame_queue.frame[i].paddr[0].cb =
				(rt_uint32_t)rt_malloc_align(size[1],
				VISS_DMA_ADDR_ALIGN_SIZE);
		}
		if (size[2] > 0) {
			test_dev->frame_queue.frame[i].paddr[0].cr =
				(rt_uint32_t)rt_malloc_align(size[2],
				VISS_DMA_ADDR_ALIGN_SIZE);
		}
		if ((size[0] > 0 && 0 == test_dev->frame_queue.frame[i].paddr[0].y) ||
			(size[1] > 0 &&
				0 == test_dev->frame_queue.frame[i].paddr[0].cb) ||
			(size[2] > 0 &&
				0 == test_dev->frame_queue.frame[i].paddr[0].cr)) {
			LOG_E("Malloc frame buffer fail. i:%d, all_num:%d.",
				i, test_dev->frame_queue.all_num);
			goto EXIT;
		}
#if 0
		LOG_E("malloc>>frame%d y:%#x(%d) cb:%#x(%d) cr:%#x(%d)", i,
			test_dev->frame_queue.frame[i].paddr.y, size[0],
			test_dev->frame_queue.frame[i].paddr.cb, size[1],
			test_dev->frame_queue.frame[i].paddr.cr, size[2]);
#endif
	}
	for (i = 0; i < test_dev->frame_queue.all_num; i++) {
		if (size[0] > 0)
			memset((void *)test_dev->frame_queue.frame[i].paddr[0].y,
				0, size[0]);
		if (size[1] > 0)
			memset((void *)test_dev->frame_queue.frame[i].paddr[0].cb,
				0, size[1]);
		if (size[2] > 0)
			memset((void *)test_dev->frame_queue.frame[i].paddr[0].cr,
				0, size[2]);
	}

	/* 4. set frame buffer to isp driver */
	ret = rt_device_control(test_dev->dev, VISS_CMD_SET_FRAME_QUEUE,
				(void *)&test_dev->frame_queue);
	if (ret < 0) {
		LOG_E("VISS set frame queue failed.");
		goto EXIT;
	}

	/* 5. start capture */
	ret = rt_device_control(test_dev->dev, VISS_CMD_CAPTURE_ON, (void *)0);
	if (ret < 0) {
		LOG_E("VISS start capture failed.");
		goto EXIT;
	}

	return RT_EOK;

EXIT:
	return -RT_ERROR;
}

static void __viss_isp_dev_capture_off(struct viss_dev_test *test_dev,
				rt_int32_t channel_num)
{
	if (RT_NULL == test_dev)
		return;

	/* 1. stop capture */
	rt_device_control(test_dev->dev, VISS_CMD_CAPTURE_OFF, (void *)0);
	LOG_E("...");
	rt_sem_take(test_dev->isp_drv_wait_sem, RT_WAITING_FOREVER);
	LOG_E("...");

}

static void __viss_isp_dev_cleanup_frame_queue(struct viss_dev_test
									*test_dev)
{
	rt_int32_t i = 0;
	rt_device_control(test_dev->dev, VISS_CMD_CLEANUP_FRAME_QUEUE, RT_NULL);
	if (RT_NULL != test_dev->frame_queue.frame) {
		for (i = 0; i < test_dev->frame_queue.all_num; i++) {
		#if 0
			LOG_E("Free frame%d>>y:%#x, cb:%#x, cr:%#x", i,
				test_dev->frame_queue.frame[i].paddr.y,
				test_dev->frame_queue.frame[i].paddr.cb,
				test_dev->frame_queue.frame[i].paddr.cr);
		#endif
			if (0 != test_dev->frame_queue.frame[i].paddr[0].y)
				rt_free_align(
				(void *)test_dev->frame_queue.frame[i].paddr[0].y);
			if (0 != test_dev->frame_queue.frame[i].paddr[0].cb)
				rt_free_align(
				(void *)test_dev->frame_queue.frame[i].paddr[0].cb);
			if (0 != test_dev->frame_queue.frame[i].paddr[0].cr)
				rt_free_align(
				(void *)test_dev->frame_queue.frame[i].paddr[0].cr);
		}
		rt_free(test_dev->frame_queue.frame);
		memset(&test_dev->frame_queue, 0, sizeof(test_dev->frame_queue));
	}

}

static void __viss_isp_dev_exit(struct viss_dev_test *test_dev,
							rt_int32_t channel_num)
{
	rt_int32_t i = 0;
#if 0
	if (RT_NULL == test_dev)
		return;

	/* 1. stop capture */
	rt_device_control(test_dev->dev, VISS_CMD_CAPTURE_OFF, (void *)0);
	LOG_E("...");

	rt_sem_take(test_dev->isp_drv_wait_sem, RT_WAITING_FOREVER);
	LOG_E("...");
#endif

	/* 2. video input device exit */
	rt_device_control(test_dev->dev, VISS_CMD_EXIT, RT_NULL);
	LOG_E("...VISS_CMD_EXIT");
	/* 3. free frame buffer */
	if (RT_NULL != test_dev->frame_queue.frame) {
		for (i = 0; i < test_dev->frame_queue.all_num; i++) {
		#if 0
			LOG_E("Free frame%d>>y:%#x, cb:%#x, cr:%#x", i,
				test_dev->frame_queue.frame[i].paddr.y,
				test_dev->frame_queue.frame[i].paddr.cb,
				test_dev->frame_queue.frame[i].paddr.cr);
		#endif
			if (0 != test_dev->frame_queue.frame[i].paddr[0].y)
				rt_free_align(
				(void *)test_dev->frame_queue.frame[i].paddr[0].y);
			if (0 != test_dev->frame_queue.frame[i].paddr[0].cb)
				rt_free_align(
				(void *)test_dev->frame_queue.frame[i].paddr[0].cb);
			if (0 != test_dev->frame_queue.frame[i].paddr[0].cr)
				rt_free_align(
				(void *)test_dev->frame_queue.frame[i].paddr[0].cr);
		}
		rt_free(test_dev->frame_queue.frame);
		memset(&test_dev->frame_queue, 0, sizeof(test_dev->frame_queue));
	}

	LOG_E("rt_device quit...");
}

static disp_ctl_t *__viss_request_layer(struct viss_dev_test *test_dev)
{
	disp_ctl_t *layer = RT_NULL;

	layer = test_dev->vply_hdl->disp_ops->disp_win_request(test_dev->win_name);
	return layer;
}

static void __viss_release_layer(struct viss_dev_test *test_dev, disp_ctl_t *layer)
{
	if ((test_dev->vply_hdl) && (RT_NULL != layer))
		test_dev->vply_hdl->disp_ops->disp_win_release(&layer);
}

static rt_err_t __viss_update_data(struct viss_dev_test *test_dev,
						dc_win_data_t *win_data)
{
	rt_err_t ret = 0;

	ret = test_dev->vply_hdl->disp_ops->disp_win_update(test_dev->layer, win_data);
	return ret;
}

static struct vply_test *__disp_isp_dev_create(void)
{
	struct vply_test *vply = RT_NULL;
	disp_io_ctrl_t dic;

	vply = rt_zalloc(sizeof(struct vply_test));
	if (RT_NULL == vply)
		return RT_NULL;

	vply->disp_device = rt_device_find("disp");
	if (vply->disp_device != NULL) {
		#ifndef RT_USING_EGUI
		rt_device_open(vply->disp_device, 0);
		#endif
		vply->disp_ops =
			(rt_device_disp_ops_t *)(vply->disp_device->user_data);
		RT_ASSERT(vply->disp_ops != NULL);
		memset(&dic, 0x00, sizeof(disp_io_ctrl_t));
		dic.dc_index = 0;
		dic.args = &vply->info;
		rt_device_control(vply->disp_device, DISP_CMD_GET_INFO, &dic);
		LOG_I("Screen w:%d h:%d", vply->info.width, vply->info.height);
		if ((0 == vply->info.width) || (0 == vply->info.height)) {
			#ifndef RT_USING_EGUI
			rt_device_close(vply->disp_device);
			#endif
			rt_free(vply);
			return RT_NULL;
		}
	} else {
		LOG_E("disp_config err.");
		rt_free(vply);
		return RT_NULL;
	}

	return vply;
}

static void __disp_dev_destory(struct vply_test *vply)
{
	rt_device_disp_ops_t *disp_ops = NULL;

	LOG_I("disp_unconfig");
	if (vply->disp_device != NULL) {
		disp_ops = (rt_device_disp_ops_t *)(vply->disp_device->user_data);
		RT_ASSERT(disp_ops != NULL);
#ifndef RT_USING_EGUI
		rt_device_close(vply->disp_device);
#endif
		vply->disp_device = RT_NULL;
	}
	rt_free(vply);
}

void __disp_isp_win_init(struct viss_dev_test *test_dev, dc_win_data_t *win_data,
			struct viss_buffer *buf, rt_int32_t mode)
{
	memset(win_data, 0, sizeof(dc_win_data_t));
	win_data->pixel_format = DISP_FORMAT_NV12;
	win_data->pixel_order = DC_PO_NORMAL;
	win_data->bpp = 32;

	if (0 == mode) { /* full screen */
		win_data->crtc_x = 0;
		win_data->crtc_y = 0;
		win_data->crtc_width = test_dev->vply_hdl->info.width;
		win_data->crtc_height = test_dev->vply_hdl->info.height;
	} else {
		win_data->crtc_x = 500;
		win_data->crtc_y = 0;
		win_data->crtc_width = test_dev->vply_hdl->info.width - win_data->crtc_x;
		win_data->crtc_height = (test_dev->vply_hdl->info.height -
					win_data->crtc_y) / 2;
	}
	win_data->fb_x = 0;
	win_data->fb_y = 0;
	win_data->fb_width = buf->src_rect.width;
	win_data->fb_height = buf->src_rect.height;

	win_data->src_width = buf->src_rect.width;
	win_data->src_height = buf->src_rect.height;

	win_data->update_flag = true;

}

#if DEBUG_VIDEO_FRAME
static void __save_video_frame(struct viss_dev_test *test_dev,
						struct viss_reqframe *vfrm)
{
	rt_int32_t fd = 0;
	rt_int32_t size[3] = {0};
	char path[64] = {0};

	if (RT_NULL == vfrm) {
		LOG_E("Input para error.");
		return;
	}

	/* open YUV file. */
	rt_snprintf(path, sizeof(path), "/mnt/sdcard/%s_%d.yuv",
			test_dev->win_name, test_dev->frm_count);
	fd = open(path, O_WRONLY | O_CREAT);
	if (fd < 0) {
		LOG_E("fail to open file: %s", path);
		return;
	}

	/* calculate frame Y and CB size. */
	__calc_frame_size(vfrm->buf->pix_format, &vfrm->buf->src_rect,
		&size[0], &size[1], &size[2]);
	if (VISS_PIX_FMT_NV12 == vfrm->buf->pix_format) {
		write(fd, (void *)vfrm->buf->paddr[0].y, size[0]);
		write(fd, (void *)vfrm->buf->paddr[0].cb, size[1]);
	}
	close(fd);
	test_dev->frm_count++;
}
#endif

static void *__vply_disp_thread(void *param)
{
	struct viss_dev_test *test_dev = (struct viss_dev_test *)param;
	struct viss_reqframe prv_req_frm_b;
	struct viss_reqframe prv_req_frm;
	struct viss_reqframe req_frm;
	rt_int32_t frame_count = 0;
	rt_int32_t first_frame = 0;
	rt_err_t ret = 0;
	/* rt_tick_t tick1 = 0; */

	memset(&prv_req_frm_b, 0, sizeof(prv_req_frm_b));
	memset(&prv_req_frm, 0, sizeof(prv_req_frm));
	memset(&req_frm, 0, sizeof(req_frm));
	/* tick1 = rt_tick_get(); */
	while (1) {
		frame_cnt_temp1++;
		/* request frame */
		memset(&req_frm, 0, sizeof(req_frm));
		rt_sem_take(test_dev->wait_frm_sem, 50); /* timeout 100ms */
		if (test_dev->exit_flag) {
			LOG_I("video playback thread exit.");
			break;
		}
		req_frm.channel_id = 0; /* request channel 0 */
		ret = rt_device_control(test_dev->dev, VISS_CMD_REQUEST_FRAME,
					(void *)&req_frm);
		if (RT_EOK != ret) {
			/* LOG_I("request frame fail."); */
			continue;
		}
		time_temp = rt_time_get_msec();
		/* LOG_E("time_temp: %d.", time_temp); */

		/* init display layer */
		frame_count++;
		if (0 == first_frame) {
			first_frame = 1;
			LOG_E("Request win layer.");
			test_dev->layer = __viss_request_layer(test_dev);
			if (RT_NULL == test_dev->layer) {
				LOG_E("Request win fail.\n");
				break;
			}
			__disp_isp_win_init(test_dev, &test_dev->win_data,
					req_frm.buf, test_dev->disp_mode);
		}
#if DEBUG_VIDEO_FRAME
		__save_video_frame(test_dev, &req_frm);
#endif
		/* send video frame to disaply */
		test_dev->win_data.dma_addr =
			(dma_addr_t)req_frm.buf->paddr[0].y;
		test_dev->win_data.chroma_dma_addr =
			(dma_addr_t)req_frm.buf->paddr[0].cb;
		test_dev->win_data.chroma_x_dma_addr =
			(dma_addr_t)req_frm.buf->paddr[0].cr;
		__viss_update_data(test_dev, &test_dev->win_data);
		/* LOG_E("Request frame id:%x", req_frm.buf->frame_id); */
		prv_req_frm_b = prv_req_frm;
		prv_req_frm = req_frm;
		if (frame_count < 2)
			continue;
		else
			frame_count = 2;

		/* release frame */
		rt_device_control(test_dev->dev, VISS_CMD_RELEASE_FRAME,
			(void *)&prv_req_frm_b);
		if(frame_cnt_temp1 % 20 == 0)
			list_mem();
		if(frame_cnt_temp1 % 100 == 0)
			break;
	}

	pthread_exit(RT_NULL);
	return RT_NULL;
}

static void* camera_isp_display(void *param)
{
	while (1) {
		rt_err_t ret = 0;
		struct vply_test *vply = RT_NULL;
		frame_cnt_temp1 = 0;
		LOG_E("camera_isp_display start.");
		vply = __disp_isp_dev_create();
		if (RT_NULL == vply) {
			LOG_E("Open dev fail.");
			break;
		}
		rt_mutex_take(lombo_isp.lock, RT_WAITING_FOREVER);

		lombo_isp.prev[0] = __viss_dev_create(VISS_ISP_PREV_DEV_NAME);
		if (RT_NULL == lombo_isp.prev[0]) {
			LOG_E("Open isp prev dev fail. %p", lombo_isp.prev[0]);
			rt_mutex_release(lombo_isp.lock);
		}

		lombo_isp.prev[0]->vply_hdl = vply;
		strncpy(lombo_isp.prev[0]->win_name, FRONT_LAYER_NAME,
			sizeof(lombo_isp.prev[0]->win_name) - 1);
		lombo_isp.prev[0]->disp_mode = 0;

		ret = __viss_dev_init(lombo_isp.prev[0], VISS_PIX_FMT_NV12,
			CAM_OUT_FRAME_720P, MAX_USE_CHANNEL_NUM);
		if (RT_EOK != ret) {
			LOG_E("init isp prev dev fail");
			rt_mutex_release(lombo_isp.lock);
		}
		while (1) {
			ret = __viss_isp_dev_capture_on(lombo_isp.prev[0],
				VISS_PIX_FMT_NV12, CAM_OUT_FRAME_720P,
				MAX_USE_CHANNEL_NUM);
			rt_mutex_release(lombo_isp.lock);

			pthread_create(&lombo_isp.prev[0]->vply_thread_id,
				RT_NULL, __vply_disp_thread,
				lombo_isp.prev[0]);

			pthread_join(lombo_isp.prev[0]->vply_thread_id, RT_NULL);

			LOG_E(" EXIT ");
			rt_mutex_take(lombo_isp.lock, RT_WAITING_FOREVER);
			__viss_isp_dev_capture_off(lombo_isp.prev[0],
				MAX_USE_CHANNEL_NUM);
			__viss_isp_dev_cleanup_frame_queue(lombo_isp.prev[0]);
			__viss_release_layer(lombo_isp.prev[0],
				lombo_isp.prev[0]->layer);
			rt_thread_delay(50);
		}
		__viss_isp_dev_exit(lombo_isp.prev[0], MAX_USE_CHANNEL_NUM);
		__viss_dev_destory(&lombo_isp.prev[0]);
		rt_mutex_release(lombo_isp.lock);

		if (vply) {
			__disp_dev_destory(vply);
			vply = RT_NULL;
		}
		LOG_E("camera_isp_display end.");
		rt_thread_delay(50); /* 500ms */
	}

	return RT_NULL;
}

static void* camera_isp_cap(void *param)
{
	while (1) {
		LOG_E("camera_isp_cap start.");
		rt_int32_t ret = 0;
		struct viss_reqframe prv_req_frm_b;
		struct viss_reqframe prv_req_frm;
		struct viss_reqframe req_frm;
		rt_int32_t frame_count = 0, num = 0;
		rt_int32_t frame_cnt_temp = 0;

		memset(&prv_req_frm_b, 0, sizeof(prv_req_frm_b));
		memset(&prv_req_frm, 0, sizeof(prv_req_frm));
		memset(&req_frm, 0, sizeof(req_frm));
		rt_mutex_take(lombo_isp.lock, RT_WAITING_FOREVER);

		lombo_isp.cap[0] = __viss_dev_create(VISS_ISP_CAP_DEV_NAME);
		if (RT_NULL == lombo_isp.cap[0]) {
			LOG_E("Open isp cap dev fail. %p", lombo_isp.cap[0]);
			rt_mutex_release(lombo_isp.lock);
			break;
		}
		ret = __viss_dev_init(lombo_isp.cap[0], VISS_PIX_FMT_NV12,
			CAM_OUT_FRAME_1080P, MAX_USE_CHANNEL_NUM);
		while (1) {
			ret = __viss_isp_dev_capture_on(lombo_isp.cap[0],
				VISS_PIX_FMT_NV12, CAM_OUT_FRAME_1080P,
				MAX_USE_CHANNEL_NUM);
			if (RT_EOK != ret) {
				LOG_E("init isp cap dev fail");
				rt_mutex_release(lombo_isp.lock);
			}
			num++;
			LOG_E("num:%d.", num);
			rt_mutex_release(lombo_isp.lock);
			while (frame_cnt_temp < CAPTURE_FRAME_NUM) {
				memset(&req_frm, 0, sizeof(req_frm));
				rt_sem_take(lombo_isp.cap[0]->wait_frm_sem,
					RT_WAITING_FOREVER);
				req_frm.channel_id = 0; /* request channel 0 */
				ret = rt_device_control(lombo_isp.cap[0]->dev,
					VISS_CMD_REQUEST_FRAME, (void *)&req_frm);
				if (ret < 0) {
					LOG_E("VISS_CMD_REQUEST_FRAME failed.");
					continue;
				}
				frame_count++;
				frame_cnt_temp++;
				prv_req_frm_b = prv_req_frm;
				prv_req_frm = req_frm;
				if (frame_count < 2)
					continue;
				else
					frame_count = 2;
				/* release frame */
				rt_device_control(lombo_isp.cap[0]->dev,
					VISS_CMD_RELEASE_FRAME,
					(void *)&prv_req_frm_b);
			}
			rt_mutex_take(lombo_isp.lock, RT_WAITING_FOREVER);
			__viss_isp_dev_capture_off(lombo_isp.cap[0],
				MAX_USE_CHANNEL_NUM);
			__viss_isp_dev_cleanup_frame_queue(lombo_isp.cap[0]);
			frame_cnt_temp = 0;
			frame_count = 0;
			LOG_E("camera_isp_cap end.");
		}
		__viss_isp_dev_exit(lombo_isp.cap[0], MAX_USE_CHANNEL_NUM);
		__viss_dev_destory(&lombo_isp.cap[0]);
		rt_mutex_release(lombo_isp.lock);
		LOG_E("camera_isp_cap end.");
		rt_thread_delay(50); /* 500ms */
	}
	return RT_NULL;
}

void isp_print_sys_mem_info(void)
{
	rt_uint32_t total = 0;
	rt_uint32_t used = 0;
	rt_uint32_t max_used = 0;

	rt_memory_info(&total, &used, &max_used);
	rt_kprintf("total memory: %d.\n", total);
	rt_kprintf("used memory : %d.\n", used);
	rt_kprintf("maximum allocated memory: %d\n", max_used);
}

long test_isp(int argc, char **argv)
{
	rt_int32_t ret;
	pthread_t tid1, tid2;
	time_start = time_temp = frame_cnt_temp1 = 0;
	isp_print_sys_mem_info();
	time_start = rt_time_get_msec();
	LOG_E("time_start: %d.", time_start);
	lombo_isp.lock = rt_mutex_create("isp_test", RT_IPC_FLAG_FIFO);

#if CAPTURE_FRAME
	ret = pthread_create(&tid2, RT_NULL, camera_isp_cap, RT_NULL);
	if (ret  < 0)
		LOG_E("pthread_create fail.");

	rt_thread_delay(50); /* 500ms */
#endif

#if PREVIEW_FRAME
	ret = pthread_create(&tid1, RT_NULL, camera_isp_display, RT_NULL);
	if (ret  < 0)
		LOG_E("pthread_create fail.");
	rt_thread_delay(100); /* 1000ms */
#endif

#if CAPTURE_FRAME
	pthread_join(tid2, RT_NULL);
#endif

#if PREVIEW_FRAME
	pthread_join(tid1, RT_NULL);
#endif

	isp_print_sys_mem_info();
	if (RT_NULL != lombo_isp.lock)
		rt_mutex_delete(lombo_isp.lock);

	return RT_EOK;
}

