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
#include <div.h>

struct vply_test {
	rt_device_t disp_device;
	rt_device_disp_ops_t *disp_ops;
	struct rt_device_graphic_info info;
};

struct viss_dev_test {
	rt_device_t  viss_dev;
	rt_uint32_t channel_num;
	rt_uint32_t frame_buf_type;
	rt_int32_t exit_flag;
	struct viss_frame_queue frame_queue;
	rt_sem_t wait_frm_sem; /* wait video input */
	rt_int32_t frm_count; /* use to save video frame */
	rt_int32_t disp_mode;
	disp_ctl_t *layer;
	char win_name[8];
	dc_win_data_t win_data;
	struct vply_test *vply_hdl;
	pthread_t vply_thread_id;
};

#define CACHE_BUFFER_CHANNEL_PER  (8)    /* number of buffers used per channel  */
#define VISS_DMA_ADDR_ALIGN_SIZE   (16)  /* vic dma need 8 or 16 byte align */
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

#define VISS_VIDEO_WIN_NAME	"viss_video"
#define VISS_VIC_DEV_NAME	"vic"
#define VISS_MCSI_DEV_NAME	"mcsi"
#define VISS_TVD_DEV_NAME	"tvd"
#define VISS_ISP_DEV_NAME	"isp"

/* front camera use vic, back camera use mcsi */
#define VISS_PREVIEW_BACK_AND_FRONT	(0)
#define VISS_PREVIEW_FRONT		(1)
#define VISS_PREVIEW_BACK		(2)

#define VISS_PREVIEW_MODE	VISS_PREVIEW_BACK_AND_FRONT

#define DEBUG_VIDEO_FRAME	(0)
/* #define DEBUG_TVD_INTPUT */
/* #define DEBUG_ISP_INPUT */

struct viss_dev_test *vdev[2] = {RT_NULL};

static void __viss_dev_exit(struct viss_dev_test *test_dev);

static rt_int32_t __calc_frame_size(rt_uint32_t pix_format, struct viss_rect *rect,
	rt_int32_t *size0, rt_int32_t *size1, rt_int32_t *size2)
{
	rt_int32_t size[3] = {0};

	if (VISS_PIX_FMT_NV12 == pix_format) {
		size[0] = rect->width * rect->height;
		/* ALIGN(rect->width, 64) * ALIGN(rect->height, 64); */
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
	test_dev->viss_dev = rt_device_find(name);
	if (RT_NULL == test_dev->viss_dev) {
		LOG_E("Request %s device fail.", name);
		goto EXIT;
	}
	ret = rt_device_open(test_dev->viss_dev, 0);
	if (RT_EOK != ret) {
		LOG_E("Open %s device fail. ret:%d", name, ret);
		goto EXIT;
	}
	return test_dev;

EXIT:
	if (test_dev->viss_dev)
		rt_device_close(test_dev->viss_dev);
	rt_free(test_dev);
	return RT_NULL;
}

static void __viss_dev_destory(struct viss_dev_test *test_dev)
{
	if (test_dev) {
		if (test_dev->viss_dev)
			rt_device_close(test_dev->viss_dev);
		rt_free(test_dev);
	}
}

static rt_err_t __viss_dev_init(struct viss_dev_test *test_dev, rt_uint32_t out_fmt,
			rt_uint32_t frame_size)
{
	rt_err_t ret = 0;
	rt_int32_t i = 0, j = 0;
	rt_int32_t channel_num = 0;
	rt_int32_t size[3] = {0};
	struct dev_all_mode all_mode;
	struct dev_mode *use_mode = RT_NULL;
	struct dev_mode *cur_mode = RT_NULL;
	rt_int32_t buf_size = 0;
	rt_int32_t y_size = 0;
	rt_int32_t cb_size = 0;
	rt_int32_t cr_size = 0;
	rt_int32_t buff_add0 = 0, buff_add1 = 0;
	test_dev->frame_buf_type = 0;

	ret = rt_device_control(test_dev->viss_dev, VISS_CMD_INIT, 0);
	if (RT_EOK != ret) {
		LOG_E("VISS_CMD_INIT. ret:%d", ret);
		goto EXIT;
	}

	rt_device_control(test_dev->viss_dev, VISS_CMD_GET_CHANNEL_NUM,
			(void *)&test_dev->channel_num);

	if (test_dev->channel_num > 1)
		rt_device_control(test_dev->viss_dev, VISS_CMD_GET_FRAME_BUF_TYPE,
			(void *)&test_dev->frame_buf_type);

	rt_device_control(test_dev->viss_dev, VISS_CMD_GET_WAIT_FRAME_SEM,
			(void *)&test_dev->wait_frm_sem);
	if ((RT_NULL == test_dev->wait_frm_sem) || (0 == test_dev->channel_num)) {
		LOG_E("Get driver information fail.\n");
		goto EXIT;
	}
	channel_num = test_dev->channel_num;

	/* 2. set mode */
	if (TVD_AUTO_DETECT_MODE != frame_size) {
		memset(&all_mode, 0, sizeof(all_mode));
		rt_device_control(test_dev->viss_dev, VISS_CMD_GET_ALL_MODE,
					(void *)&all_mode);
		for (i = 0; i < all_mode.num; i++) {
			LOG_D("mode %d: w:%d h:%d outfmt:%#x, inpfmt:%#x, index:%d", i,
				all_mode.mode[i].frame_size.width,
				all_mode.mode[i].frame_size.height,
				all_mode.mode[i].out_fmt,
				all_mode.mode[i].inp_fmt,
				all_mode.mode[i].index);
			if ((all_mode.mode[i].out_fmt == out_fmt) &&
				(all_mode.mode[i].frame_size.height == frame_size)) {
				LOG_D("Find dev mode. i:%d", i);
				use_mode = &all_mode.mode[i];
				break;
			}
		}
		if (i >= all_mode.num) {
			LOG_E("Can't find output format %#x mode.", out_fmt);
			goto EXIT;
		}
#if 0
		use_mode->frame_size.left = 320;
		use_mode->frame_size.top = 0;
		use_mode->frame_size.height = 720;
		use_mode->frame_size.width = 320;

		use_mode->frame_size.left = 320;
		use_mode->frame_size.top = 0;
		use_mode->frame_size.height = 720;
		use_mode->frame_size.width = 320;
#endif
		rt_device_control(test_dev->viss_dev, VISS_CMD_SET_MODE,
					(void *)use_mode->index);
		rt_device_control(test_dev->viss_dev, VISS_CMD_GET_CUR_MODE,
					(void *)&cur_mode);
		if (cur_mode != use_mode) {
			LOG_E("Device mode error.");
			goto EXIT;
		}
	} else {
		ret = rt_device_control(test_dev->viss_dev, VISS_CMD_SET_MODE,
						(void *)TVD_AUTO_DETECT_MODE);
		if (RT_EOK != ret) {
			LOG_E("TVD format detect error.");
			goto EXIT;
		}
		rt_device_control(test_dev->viss_dev, VISS_CMD_GET_CUR_MODE,
						(void *)&cur_mode);
		if (RT_NULL == cur_mode) {
			LOG_E("Get sensor mode fail.");
			goto EXIT;
		}
	}
	LOG_E("Current %d mode size>> w:%d h:%d",
		cur_mode->index, cur_mode->frame_size.width, cur_mode->frame_size.height);
	if (0 == test_dev->frame_buf_type) {
		/* 3. malloc frame buffer */
		__calc_frame_size(cur_mode->out_fmt, &cur_mode->frame_size,
			&size[0], &size[1], &size[2]);
		if (size[0] <= 0) {
			LOG_E("Calc frame size error. %d", size[0]);
			goto EXIT;
		}
	}
	memset(&test_dev->frame_queue, 0, sizeof(test_dev->frame_queue));
	/* test_dev->frame_queue.all_num = CACHE_BUFFER_CHANNEL_PER * channel_num; */
	test_dev->frame_queue.all_num = CACHE_BUFFER_CHANNEL_PER;
	test_dev->frame_queue.frame = rt_zalloc(test_dev->frame_queue.all_num *
					sizeof(struct viss_buffer));
	if (0 == test_dev->frame_buf_type) {
		for (i = 0; i < test_dev->frame_queue.all_num; i++) {
			for (j = 0; j < channel_num; j++) {
				test_dev->frame_queue.frame[i].paddr[j].align =
								VISS_DMA_ADDR_ALIGN_SIZE;
				if (size[0] > 0) {
					test_dev->frame_queue.frame[i].paddr[j].y =
						(rt_uint32_t)rt_malloc_align(size[0],
						VISS_DMA_ADDR_ALIGN_SIZE);
					test_dev->frame_queue.frame[i].paddr[j].y_size =
										size[0];
				}
				if (size[1] > 0) {
					test_dev->frame_queue.frame[i].paddr[j].cb =
						(rt_uint32_t)rt_malloc_align(size[1],
						VISS_DMA_ADDR_ALIGN_SIZE);
					test_dev->frame_queue.frame[i].paddr[j].cb_size =
										size[1];
				}
				if (size[2] > 0) {
					test_dev->frame_queue.frame[i].paddr[j].cr =
						(rt_uint32_t)rt_malloc_align(size[2],
						VISS_DMA_ADDR_ALIGN_SIZE);
					test_dev->frame_queue.frame[i].paddr[j].cr_size =
										size[2];
				}
				if ((size[0] > 0 &&
				(0 == test_dev->frame_queue.frame[i].paddr[j].y))
				|| (size[1] > 0 &&
				(0 == test_dev->frame_queue.frame[i].paddr[j].cb))
				|| (size[2] > 0 &&
				(0 == test_dev->frame_queue.frame[i].paddr[j].cr))) {
					LOG_E("Malloc frame buff fail. i:%d, all_num:%d",
						i, test_dev->frame_queue.all_num);
					goto EXIT;
				}
				LOG_D("malloc>>frame%d y:%#x(%d) cb:%#x(%d) cr:%#x(%d)",
				i, test_dev->frame_queue.frame[i].paddr[j].y, size[0],
				test_dev->frame_queue.frame[i].paddr[j].cb, size[1],
				test_dev->frame_queue.frame[i].paddr[j].cr, size[2]);
			}
		}
		for (i = 0; i < test_dev->frame_queue.all_num; i++) {
			for (j = 0; j < channel_num; j++) {
				if (size[0] > 0)
					memset((void *)test_dev->
					frame_queue.frame[i].paddr[j].y, 0, size[0]);
				if (size[1] > 0)
					memset((void *)test_dev->
					frame_queue.frame[i].paddr[j].cb , 0, size[1]);
				if (size[2] > 0)
					memset((void *)test_dev->
					frame_queue.frame[i].paddr[j].cr , 0, size[2]);
			}
		}
	} else if (1 == test_dev->frame_buf_type) {
		y_size = cur_mode->frame_size.height * cur_mode->frame_size.width;
		cb_size = (cur_mode->frame_size.height * cur_mode->frame_size.width) / 2;
		for (i = 0; i < test_dev->frame_queue.all_num; i++) {
			buff_add0 = 0;
			buff_add0 = (rt_uint32_t)rt_malloc_align(y_size * channel_num,
							VISS_DMA_ADDR_ALIGN_SIZE);

			buff_add1 = 0;
			buff_add1 = (rt_uint32_t)rt_malloc_align(cb_size * channel_num,
							VISS_DMA_ADDR_ALIGN_SIZE);
			for (j = 0; j < channel_num; j++) {
				test_dev->frame_queue.frame[i].paddr[j].align =
								VISS_DMA_ADDR_ALIGN_SIZE;
				if (0 == j) {
					test_dev->frame_queue.frame[i].paddr[j].y =
									buff_add0;

					test_dev->frame_queue.frame[i].paddr[j].y_size =
									y_size;

					test_dev->frame_queue.frame[i].paddr[j].cb =
									buff_add1;
					test_dev->frame_queue.frame[i].paddr[j].cb_size =
									cb_size;

					test_dev->frame_queue.frame[i].paddr[j].cr_size =
									cr_size;

				} else if (1 == j) {
					test_dev->frame_queue.frame[i].paddr[j].y =
						buff_add0 + cur_mode->frame_size.width;
					test_dev->frame_queue.frame[i].paddr[j].y_size =
									y_size;

					test_dev->frame_queue.frame[i].paddr[j].cb =
						buff_add1 + cur_mode->frame_size.width;
					test_dev->frame_queue.frame[i].paddr[j].cb_size =
									cb_size;

					test_dev->frame_queue.frame[i].paddr[j].cr_size =
									cr_size;

				} else if (2 == j) {
					test_dev->frame_queue.frame[i].paddr[j].y =
							buff_add0 + (y_size * 2);
					test_dev->frame_queue.frame[i].paddr[j].y_size =
									y_size;

					test_dev->frame_queue.frame[i].paddr[j].cb =
							buff_add1 + (cb_size * 2);
					test_dev->frame_queue.frame[i].paddr[j].cb_size =
									cb_size;

					test_dev->frame_queue.frame[i].paddr[j].cr_size =
									cr_size;

				} else if (3 == j) {
					test_dev->frame_queue.frame[i].paddr[j].y =
							buff_add0 + (y_size * 2) +
							cur_mode->frame_size.width;
					test_dev->frame_queue.frame[i].paddr[j].y_size =
									y_size;

					test_dev->frame_queue.frame[i].paddr[j].cb =
							buff_add1 + (cb_size * 2) +
							cur_mode->frame_size.width;
					test_dev->frame_queue.frame[i].paddr[j].cb_size =
									cb_size;

					test_dev->frame_queue.frame[i].paddr[j].cr_size =
									cr_size;

				}
			}
		}
		for (i = 0; i < test_dev->frame_queue.all_num; i++) {
			for (j = 0; j < channel_num; j++) {
				if (test_dev->frame_queue.frame[i].paddr[j].y_size > 0)
					memset((void *)test_dev->
						frame_queue.frame[i].paddr[j].y, 0,
					test_dev->frame_queue.frame[i].paddr[j].y_size);
				if (test_dev->frame_queue.frame[i].paddr[j].cb_size > 0)
					memset((void *)test_dev->
					frame_queue.frame[i].paddr[j].cb, 0,
					test_dev->frame_queue.frame[i].paddr[j].cb_size);
				if (test_dev->frame_queue.frame[i].paddr[j].cr_size > 0)
					memset((void *)test_dev->
					frame_queue.frame[i].paddr[j].cr, 0,
					test_dev->frame_queue.frame[i].paddr[j].cr_size);
			}
		}

	} else {
		y_size = cur_mode->frame_size.height * cur_mode->frame_size.width;
		cb_size = (cur_mode->frame_size.height * cur_mode->frame_size.width) / 2;
		for (i = 0; i < test_dev->frame_queue.all_num; i++) {
			buff_add0 = 0;
			buff_add0 = (rt_uint32_t)rt_malloc_align(y_size * channel_num,
							VISS_DMA_ADDR_ALIGN_SIZE);

			buff_add1 = 0;
			buff_add1 = (rt_uint32_t)rt_malloc_align(cb_size * channel_num,
							VISS_DMA_ADDR_ALIGN_SIZE);
			for (j = 0; j < channel_num; j++) {
				test_dev->frame_queue.frame[i].paddr[j].align =
								VISS_DMA_ADDR_ALIGN_SIZE;
				if (0 == j) {
					test_dev->frame_queue.frame[i].paddr[j].y =
									buff_add0;

					test_dev->frame_queue.frame[i].paddr[j].y_size =
									y_size;

					test_dev->frame_queue.frame[i].paddr[j].cb =
									buff_add1;
					test_dev->frame_queue.frame[i].paddr[j].cb_size =
									cb_size;

					test_dev->frame_queue.frame[i].paddr[j].cr_size =
									cr_size;

				} else if (1 == j) {
					test_dev->frame_queue.frame[i].paddr[j].y =
							buff_add0 + (y_size * 1);
					test_dev->frame_queue.frame[i].paddr[j].y_size =
									y_size;

					test_dev->frame_queue.frame[i].paddr[j].cb =
							buff_add1 + (cb_size * 1);
					test_dev->frame_queue.frame[i].paddr[j].cb_size =
									cb_size;

					test_dev->frame_queue.frame[i].paddr[j].cr_size =
									cr_size;

				} else if (2 == j) {
					test_dev->frame_queue.frame[i].paddr[j].y =
							buff_add0 + (y_size * 2);
					test_dev->frame_queue.frame[i].paddr[j].y_size =
									y_size;

					test_dev->frame_queue.frame[i].paddr[j].cb =
							buff_add1 + (cb_size * 2);
					test_dev->frame_queue.frame[i].paddr[j].cb_size =
									cb_size;

					test_dev->frame_queue.frame[i].paddr[j].cr_size =
									cr_size;

				} else if (3 == j) {
					test_dev->frame_queue.frame[i].paddr[j].y =
							buff_add0 + (y_size * 3);
					test_dev->frame_queue.frame[i].paddr[j].y_size =
									y_size;

					test_dev->frame_queue.frame[i].paddr[j].cb =
							buff_add1 + (cb_size * 3);
					test_dev->frame_queue.frame[i].paddr[j].cb_size =
									cb_size;

					test_dev->frame_queue.frame[i].paddr[j].cr_size =
									cr_size;

				}
			}
		}
		for (i = 0; i < test_dev->frame_queue.all_num; i++) {
			for (j = 0; j < channel_num; j++) {
				if (test_dev->frame_queue.frame[i].paddr[j].y_size > 0)
					memset((void *)test_dev->
						frame_queue.frame[i].paddr[j].y, 0,
					test_dev->frame_queue.frame[i].paddr[j].y_size);
				if (test_dev->frame_queue.frame[i].paddr[j].cb_size > 0)
					memset((void *)test_dev->
						frame_queue.frame[i].paddr[j].cb, 0,
					test_dev->frame_queue.frame[i].paddr[j].cb_size);
				if (test_dev->frame_queue.frame[i].paddr[j].cr_size > 0)
					memset((void *)test_dev->
						frame_queue.frame[i].paddr[j].cr, 0,
					test_dev->frame_queue.frame[i].paddr[j].cr_size);
			}
		}

		}
	/* 4. set frame buffer to vic driver */
	rt_device_control(test_dev->viss_dev, VISS_CMD_SET_FRAME_QUEUE,
				(void *)&test_dev->frame_queue);
	/* 5. start capture */
	ret = rt_device_control(test_dev->viss_dev, VISS_CMD_CAPTURE_ON, (void *)0);
	if (ret < 0)
		goto EXIT;
	return RT_EOK;

EXIT:
	return -RT_ERROR;
}

static void __viss_dev_exit(struct viss_dev_test *test_dev)
{
	rt_int32_t i = 0, j = 0;

	if (RT_NULL == test_dev)
		return;

	/* 1. stop capture */
	rt_device_control(test_dev->viss_dev, VISS_CMD_CAPTURE_OFF, (void *)0);

	/* 2. video input device exit */
	rt_device_control(test_dev->viss_dev, VISS_CMD_EXIT, RT_NULL);

	/* 3. free frame buffer */
	if (RT_NULL != test_dev->frame_queue.frame) {
		for (i = 0; i < test_dev->frame_queue.all_num; i++) {
			for (j = 0; j < test_dev->channel_num; j++) {
				LOG_D("Free frame%d>>y:%#x, cb:%#x, cr:%#x", i,
					test_dev->frame_queue.frame[i].paddr[j].y,
					test_dev->frame_queue.frame[i].paddr[j].cb,
					test_dev->frame_queue.frame[i].paddr[j].cr);
				if (0 != test_dev->frame_queue.frame[i].paddr[j].y)
					rt_free_align(
				(void *)test_dev->frame_queue.frame[i].paddr[j].y);
				if (0 != test_dev->frame_queue.frame[i].paddr[j].cb)
					rt_free_align(
				(void *)test_dev->frame_queue.frame[i].paddr[j].cb);
				if (0 != test_dev->frame_queue.frame[i].paddr[j].cr)
					rt_free_align(
				(void *)test_dev->frame_queue.frame[i].paddr[j].cr);
			}
		}
		rt_free(test_dev->frame_queue.frame);
		memset(&test_dev->frame_queue, 0, sizeof(test_dev->frame_queue));
	}

	LOG_D("rt_device quit...");
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

static struct vply_test *__disp_dev_create(void)
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
		rt_memset(&dic, 0x00, sizeof(disp_io_ctrl_t));
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
		LOG_E("disp_config err");
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
}

void __disp_win_init(struct viss_dev_test *test_dev, dc_win_data_t *win_data,
			struct viss_buffer *buf, rt_int32_t mode)
{
	memset(win_data, 0, sizeof(dc_win_data_t));
	win_data->pixel_format = DISP_FORMAT_NV12;
	win_data->pixel_order = DC_PO_NORMAL;
	win_data->bpp = 32;

	if (0 == mode) { /* full screen */
		win_data->crtc_x = 0;
		win_data->crtc_y = 0; /* 15 */
		win_data->crtc_width = test_dev->vply_hdl->info.width; /* 800 */
		win_data->crtc_height = test_dev->vply_hdl->info.height; /* 450 */
	} else {
		win_data->crtc_x = 500;
		win_data->crtc_y = 0; /* 36 */
		win_data->crtc_width = test_dev->vply_hdl->info.width -
					win_data->crtc_x; /* 300 */
		/* win_data->crtc_height = (test_dev->vply_hdl->info.height -
					win_data->crtc_y) / 2; */ /* 168 */
		win_data->crtc_height = (test_dev->vply_hdl->info.height -
					win_data->crtc_y);
		do_div(win_data->crtc_height, 2);/* 168 */
	}
	win_data->fb_x = 0;
	win_data->fb_y = 0;

	if (test_dev->channel_num > 1) {
		if (test_dev->frame_buf_type == 2) {
			win_data->fb_width = buf->src_rect.width * 1;
			win_data->fb_height = buf->src_rect.height * 2;

			win_data->src_width = buf->src_rect.width * 1;
			win_data->src_height = buf->src_rect.height * 2;
		} else if (test_dev->frame_buf_type == 1) {
			win_data->fb_width = buf->src_rect.width * 2;
			win_data->fb_height = buf->src_rect.height * 2;

			win_data->src_width = buf->src_rect.width * 2;
			win_data->src_height = buf->src_rect.height * 2;
		}
	} else {
		win_data->fb_width = buf->src_rect.width;
		win_data->fb_height = buf->src_rect.height;

		win_data->src_width = buf->src_rect.width;
		win_data->src_height = buf->src_rect.height;
	}
	win_data->update_flag = true;
}
#if DEBUG_VIDEO_FRAME
static void __save_video_frame(struct viss_dev_test *test_dev,
						struct viss_reqframe *vfrm)
{
	rt_int32_t fd = 0;
	rt_int32_t size[3] = {0}, i = 0;
	char path[64] = {0};

	if (RT_NULL == vfrm) {
		LOG_E("Input para error.");
		return;
	}
	rt_snprintf(path, sizeof(path), "/mnt/sdcard/%s_%d.yuv",
			test_dev->win_name, test_dev->frm_count);
	fd = open(path, O_WRONLY | O_CREAT);
	if (fd < 0) {
		LOG_E("fail to open file: %s", path);
		return;
	}


	if (VISS_PIX_FMT_NV12 == vfrm->buf->pix_format) {
		/*
		write(fd, (void *)vfrm->buf->paddr[i].y, 1280 * 720 * 3 *2);
		*/
		write(fd, (void *)vfrm->buf->paddr[i].y, vfrm->buf->paddr[i].y_size *
						test_dev->channel_num);
		write(fd, (void *)vfrm->buf->paddr[i].cb , vfrm->buf->paddr[i].cb_size *
						test_dev->channel_num);
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
	rt_int32_t first_frame = 0, i = 0;
	rt_err_t ret = 0;
	memset(&prv_req_frm_b, 0, sizeof(prv_req_frm_b));
	memset(&prv_req_frm, 0, sizeof(prv_req_frm));
	memset(&req_frm, 0, sizeof(req_frm));
	while (1) {
		/* request frame */
		memset(&req_frm, 0, sizeof(req_frm));
		rt_sem_take(test_dev->wait_frm_sem, 50); /* timeout 100ms */
		if (test_dev->exit_flag) {
			LOG_I("video playback thread exit.");
			break;
		}
		req_frm.channel_id = 0; /* request channel 0 */
		ret = rt_device_control(test_dev->viss_dev, VISS_CMD_REQUEST_FRAME,
					(void *)&req_frm);
		if (RT_EOK != ret) {
			/* LOG_I("request frame fail.\n"); */
			continue;
		}
		/* init display layer */
		frame_count++;
		if (0 == first_frame) {
			first_frame = 1;
			LOG_D("Request win layer.");
			test_dev->layer = __viss_request_layer(test_dev);
			if (RT_NULL == test_dev->layer) {
				LOG_E("Request win fail.");
				break;
			}
			__disp_win_init(test_dev, &test_dev->win_data,
					req_frm.buf, test_dev->disp_mode);
		}
#if DEBUG_VIDEO_FRAME
	__save_video_frame(test_dev, &req_frm);

#endif
		/* send video frame to disaply */
		test_dev->win_data.dma_addr = (dma_addr_t)req_frm.buf->paddr[i].y;
		test_dev->win_data.chroma_dma_addr =
						(dma_addr_t)req_frm.buf->paddr[i].cb;
		test_dev->win_data.chroma_x_dma_addr =
						(dma_addr_t)req_frm.buf->paddr[i].cr;
		if (((req_frm.buf->paddr[i].y >> 28) & 0xc) &&
			((req_frm.buf->paddr[i].cb >> 28) & 0xc)) {
			__viss_update_data(test_dev, &test_dev->win_data);
		} else {
			LOG_E("Fatal error: %x %x %x", req_frm.buf->paddr[i].y,
				req_frm.buf->paddr[i].cb, req_frm.buf->paddr[i].cr);
		}
		/* LOG_D("Request frame id:%x", req_frm.buf->frame_id); */
		prv_req_frm_b = prv_req_frm;
		prv_req_frm = req_frm;
		if (frame_count < 2)
			continue;
		else
			frame_count = 2;
		/* release frame */
		rt_device_control(test_dev->viss_dev, VISS_CMD_RELEASE_FRAME,
							(void *)&prv_req_frm_b);
		if (frame_count > 10)
			break;
	}

	pthread_exit(RT_NULL);
	return RT_NULL;
}

rt_err_t camera_display(void)
{
	rt_err_t ret = 0;
	struct vply_test *vply = RT_NULL;
	vply = __disp_dev_create();
	if (RT_NULL == vply) {
		LOG_E("Open dev fail.");
		goto EXIT;
	}
	if (VISS_PREVIEW_BACK_AND_FRONT == VISS_PREVIEW_MODE) {
#ifdef DEBUG_ISP_INPUT
		vdev[0] = __viss_dev_create(VISS_ISP_DEV_NAME);
#else
		vdev[0] = __viss_dev_create(VISS_VIC_DEV_NAME);
#endif
#ifdef DEBUG_TVD_INTPUT
		vdev[1] = __viss_dev_create(VISS_TVD_DEV_NAME);
#else
		vdev[1] = __viss_dev_create(VISS_MCSI_DEV_NAME);
#endif
		if (RT_NULL != vdev[0]) {
			vdev[0]->vply_hdl = vply;
			strncpy(vdev[0]->win_name, FRONT_LAYER_NAME,
						sizeof(vdev[0]->win_name) - 1);
			vdev[0]->disp_mode = 0;
		}
		if (RT_NULL != vdev[1]) {
			vdev[1]->vply_hdl = vply;
			strncpy(vdev[1]->win_name, BACK_LAYER_NAME,
						sizeof(vdev[1]->win_name) - 1);
			vdev[1]->disp_mode = 1;
		}
	} else if (VISS_PREVIEW_FRONT == VISS_PREVIEW_MODE) {
#ifdef DEBUG_ISP_INPUT
		vdev[0] = __viss_dev_create(VISS_ISP_DEV_NAME);
#else
		vdev[0] = __viss_dev_create(VISS_VIC_DEV_NAME);
#endif
		if (RT_NULL == vdev[0]) {
			LOG_E("Open viss dev fail. %p", vdev[0]);
			goto EXIT;
		}
		vdev[0]->vply_hdl = vply;
		strncpy(vdev[0]->win_name, FRONT_LAYER_NAME,
					sizeof(vdev[0]->win_name) - 1);
		vdev[0]->disp_mode = 0;
	} else if (VISS_PREVIEW_BACK == VISS_PREVIEW_MODE) {
#ifdef DEBUG_TVD_INTPUT
		vdev[1] = __viss_dev_create(VISS_TVD_DEV_NAME);
#else
		vdev[1] = __viss_dev_create(VISS_MCSI_DEV_NAME);
#endif
		if (RT_NULL == vdev[1]) {
			LOG_E("Open viss dev fail. %p", vdev[1]);
			goto EXIT;
		}
		vdev[1]->vply_hdl = vply;
		strncpy(vdev[1]->win_name, BACK_LAYER_NAME,
					sizeof(vdev[1]->win_name) - 1);
		vdev[1]->disp_mode = 1;
	}
	if (vdev[0]) {
		ret = __viss_dev_init(vdev[0], VISS_PIX_FMT_NV12,
				CAM_OUT_FRAME_720P /* CAM_OUT_FRAME_1080P */);
		if (RT_EOK == ret) {
			pthread_create(&vdev[0]->vply_thread_id,
						RT_NULL,
						__vply_disp_thread,
						vdev[0]);
		}
	}
	if (vdev[1]) {
#ifdef DEBUG_TVD_INTPUT
		ret = __viss_dev_init(vdev[1], VISS_PIX_FMT_NV12, TVD_AUTO_DETECT_MODE);
#else
		ret = __viss_dev_init(vdev[1], VISS_PIX_FMT_NV12, CAM_OUT_FRAME_720P);
#endif
		if (RT_EOK == ret) {
			pthread_create(&vdev[1]->vply_thread_id,
						RT_NULL,
						__vply_disp_thread,
						vdev[1]);
		}
	}
	if (vdev[0])
		pthread_join(vdev[0]->vply_thread_id, RT_NULL);
	if (vdev[1])
		pthread_join(vdev[1]->vply_thread_id, RT_NULL);

EXIT:
	if (vdev[0]) {
		__viss_dev_exit(vdev[0]);
		__viss_release_layer(vdev[0], vdev[0]->layer);
		__viss_dev_destory(vdev[0]);
		vdev[0] = RT_NULL;
	}
	if (vdev[1]) {
		__viss_dev_exit(vdev[1]);
		__viss_release_layer(vdev[1], vdev[1]->layer);
		__viss_dev_destory(vdev[1]);
		vdev[1] = RT_NULL;
	}
#if 1
	if (vply) {
		__disp_dev_destory(vply);
		rt_free(vply);
		vply = RT_NULL;
	}
#endif
	return -RT_ERROR;
}

void print_sys_mem_info(void)
{
	rt_uint32_t total = 0;
	rt_uint32_t used = 0;
	rt_uint32_t max_used = 0;

	rt_memory_info(&total, &used, &max_used);
	rt_kprintf("total memory: %d\n", total);
	rt_kprintf("used memory : %d\n", used);
	rt_kprintf("maximum allocated memory: %d\n", max_used);
}

long test_viss(int argc, char **argv)
{
#if 1
	print_sys_mem_info();
	camera_display();/* diplay 60s */
	rt_thread_delay(50); /* 500ms */
	print_sys_mem_info();
#endif
	return RT_EOK;
}

