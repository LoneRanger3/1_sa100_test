/*
 * oscl_eos_disp.c - display api used by lombo media framework.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#define DBG_LEVEL		DBG_INFO
#define LOG_TAG			"oscl_display"

#include "oscl_eos_disp.h"

static OMX_S32 open_disp_cnt;
static rt_device_t disp_device;
static pthread_mutex_t *disp_lock;

static disp_pixformat_map_index_t disp_pix_format_map_table[] = {
	{DISP_FORMAT_NV12, OMX_COLOR_FormatYUV420SemiPlanar},
};

static disp_rotmode_map_index_t disp_rot_mode_map_table[] = {
	{LOMBO_DRM_TRANSFORM_COPY, VDISP_ROTATE_NONE},
	{LOMBO_DRM_TRANSFORM_ROT_90, VDISP_ROTATE_90},
	{LOMBO_DRM_TRANSFORM_ROT_180, VDISP_ROTATE_180},
	{LOMBO_DRM_TRANSFORM_ROT_270, VDISP_ROTATE_270},
	{LOMBO_DRM_TRANSFORM_FLIP_H, VDISP_FLIP_H},
	{LOMBO_DRM_TRANSFORM_FLIP_H_ROT_90, VDISP_FLIP_H_ROT_90},
	{LOMBO_DRM_TRANSFORM_FLIP_V, VDISP_FLIP_V},
	{LOMBO_DRM_TRANSFORM_FLIP_V_ROT_90, VDISP_FLIP_V_ROT_90},
};

static void oscl_disp_lock(void)
{
	if (disp_lock)
		pthread_mutex_lock(disp_lock);
}

static void oscl_disp_unlock(void)
{
	if (disp_lock)
		pthread_mutex_unlock(disp_lock);
}

static OMX_S32 omxformat_to_dispformat(OMX_COLOR_FORMATTYPE format)
{
	OMX_S32 i = 0;
	disp_pixformat_map_index_t *pix_map = NULL;

	for (i = 0; i < ARRAY_SIZE(disp_pix_format_map_table); i++) {
		pix_map = &disp_pix_format_map_table[i];
		if (pix_map->eColorFormat == format)
			return pix_map->disp_format;
	}

	return -1;
}

static OMX_S32 omxrot_to_disprot_mode(vdisp_rotate_mode_e mode)
{
	OMX_S32 i = 0;
	disp_rotmode_map_index_t *rot_map = NULL;

	for (i = 0; i < ARRAY_SIZE(disp_rot_mode_map_table); i++) {
		rot_map = &disp_rot_mode_map_table[i];
		if (rot_map->omx_rot_mode == mode)
			return rot_map->disp_rot_mode;
	}

	return -1;
}

void *oscl_open_disp_engine(void)
{
	disp_hdl_t *vdisp = NULL;
	OMX_S32 ret = 0;

	vdisp = oscl_zalloc(sizeof(disp_hdl_t));
	oscl_param_check((NULL != vdisp), NULL, NULL);
	if (open_disp_cnt > 0) {
		vdisp->disp_device = disp_device;
		vdisp->disp_ops =
			(rt_device_disp_ops_t *)(vdisp->disp_device->user_data);
		RT_ASSERT(vdisp->disp_ops != NULL);
		open_disp_cnt++;
		return vdisp;
	}
	vdisp->disp_device = rt_device_find("disp");
	if (vdisp->disp_device != NULL) {
		rt_device_open(vdisp->disp_device, 0);
		vdisp->disp_ops =
			(rt_device_disp_ops_t *)(vdisp->disp_device->user_data);
		RT_ASSERT(vdisp->disp_ops != NULL);
		disp_lock = oscl_zalloc(sizeof(pthread_mutex_t));
		if (NULL == disp_lock)
			goto EXIT;
		ret = pthread_mutex_init(disp_lock, NULL);
		if (0 != ret) {
			OSCL_LOGE("Create mutex lock fail.");
			goto EXIT;
		}
		disp_device = vdisp->disp_device;
		open_disp_cnt++;
	} else {
		goto EXIT;
	}
	return vdisp;

EXIT:
	if (vdisp->disp_device)
		rt_device_close(vdisp->disp_device);
	if (disp_lock) {
		oscl_free(disp_lock);
		disp_lock = NULL;
	}
	open_disp_cnt = 0;
	disp_device = NULL;
	oscl_free(vdisp);
	return NULL;
}

void oscl_close_disp_engine(void *hdl)
{
	disp_hdl_t *vdisp = (disp_hdl_t *)hdl;

	if (vdisp == NULL)
		return;

	OSCL_LOGD("close display engine, cnt:%d", open_disp_cnt);
	open_disp_cnt--;
	if (open_disp_cnt > 0) {
		oscl_free(vdisp);
	} else {
		if (vdisp->disp_device)
			rt_device_close(vdisp->disp_device);
		if (disp_lock) {
			pthread_mutex_destroy(disp_lock);
			oscl_free(disp_lock);
		}
		disp_lock = NULL;
		disp_device = NULL;
		open_disp_cnt = 0;
		oscl_free(vdisp);
		OSCL_LOGI("display engine close complet.");
	}
}

OMX_S32 oscl_open_rot_submodule(void *hdl)
{
	disp_hdl_t *vdisp = (disp_hdl_t *)hdl;
	OMX_S32 ret = 0;

	oscl_param_check((NULL != vdisp), OMX_ErrorInsufficientResources, NULL);
	ret = rt_device_control(vdisp->disp_device, DISP_CMD_ENABLE_HW_ROT, NULL);
	if (ret != DISP_OK) {
		OSCL_LOGE("enable hw rot err");
		return OMX_ErrorInsufficientResources;
	}

	return OMX_ErrorNone;
}

void oscl_close_rot_submodule(void *hdl)
{
	disp_hdl_t *vdisp = (disp_hdl_t *)hdl;
	OMX_S32 ret = 0;

	if (NULL != vdisp)
		return;
	ret = rt_device_control(vdisp->disp_device, DISP_CMD_DISABLE_HW_ROT, NULL);
	if (ret != DISP_OK)
		LOG_E("disable hw rot err");
}

void *oscl_disp_request_layer(void *hdl)
{
	disp_hdl_t *vdisp = (disp_hdl_t *)hdl;
	disp_ctl_t *layer = NULL;
	static OMX_S32 layer_index;

	oscl_param_check((NULL != vdisp), NULL, NULL);
	oscl_disp_lock();
	if (layer_index > 10000)
		layer_index = 0;
	snprintf(vdisp->layer_name, sizeof(vdisp->layer_name),
					"mlayer%ld", layer_index++);
	layer = vdisp->disp_ops->disp_win_request(vdisp->layer_name);
	OSCL_LOGD("Request layer:%s %p", vdisp->layer_name, layer);
	oscl_disp_unlock();

	return layer;
}

OMX_ERRORTYPE oscl_disp_enable_layer(void *hdl, void **layer)
{
	return OMX_ErrorNone;
}

OMX_ERRORTYPE oscl_disp_disable_layer(void *hdl, void **layer)
{
	return OMX_ErrorNone;
}

OMX_ERRORTYPE oscl_disp_release_layer(void *hdl, void **layer)
{
	disp_hdl_t *vdisp = (disp_hdl_t *)hdl;

	oscl_param_check((NULL != vdisp), OMX_ErrorBadParameter, NULL);
	oscl_param_check((NULL != layer), OMX_ErrorBadParameter, NULL);
	oscl_param_check((NULL != *layer), OMX_ErrorBadParameter, NULL);

	oscl_disp_lock();
	OSCL_LOGD("Release layer:%s %p", vdisp->layer_name, *layer);
	vdisp->disp_ops->disp_win_release((disp_ctl_t **)layer);
	oscl_disp_unlock();

	return OMX_ErrorNone;
}

OMX_ERRORTYPE oscl_disp_get_screen_size(void *hdl, disp_size_t *size)
{
	disp_hdl_t *vdisp = (disp_hdl_t *)hdl;
	struct rt_device_graphic_info sinfo;
	disp_io_ctrl_t dic;

	oscl_param_check((NULL != vdisp), OMX_ErrorBadParameter, NULL);
	oscl_param_check((NULL != size), OMX_ErrorBadParameter, NULL);

	oscl_disp_lock();
	rt_memset(&dic, 0x00, sizeof(disp_io_ctrl_t));
	dic.dc_index = 0;
	dic.args = &sinfo;
	rt_device_control(vdisp->disp_device, DISP_CMD_GET_INFO, &dic);
	if ((sinfo.width <= 0) || (sinfo.height <= 0)) {
		OSCL_LOGE("Get screen size error.");
		oscl_disp_unlock();
		return OMX_ErrorInsufficientResources;
	}
	size->width = sinfo.width;
	size->height = sinfo.height;
	oscl_disp_unlock();

	return OMX_ErrorNone;
}

OMX_ERRORTYPE oscl_disp_set_layer_top(void *hdl, void *layer)
{
	disp_hdl_t *vdisp = (disp_hdl_t *)hdl;
	OMX_ERRORTYPE ret = OMX_ErrorNone;

	oscl_param_check((NULL != vdisp), OMX_ErrorBadParameter, NULL);
	oscl_param_check((NULL != layer), OMX_ErrorBadParameter, NULL);

	oscl_disp_lock();
	ret = vdisp->disp_ops->disp_set_win_layer(layer, WIN_LAYER_TOP);
	oscl_disp_unlock();
	oscl_param_check((OMX_ErrorNone == ret), OMX_ErrorResourcesLost, NULL);

	return OMX_ErrorNone;
}

OMX_ERRORTYPE oscl_disp_set_layer_bottom(void *hdl, void *layer)
{
	disp_hdl_t *vdisp = (disp_hdl_t *)hdl;
	OMX_ERRORTYPE ret = OMX_ErrorNone;

	oscl_param_check((NULL != vdisp), OMX_ErrorBadParameter, NULL);
	oscl_param_check((NULL != layer), OMX_ErrorBadParameter, NULL);
	oscl_disp_lock();
	ret = vdisp->disp_ops->disp_set_win_layer(layer, WIN_LAYER_BOTTOM);
	oscl_disp_unlock();
	oscl_param_check((OMX_ErrorNone == ret), OMX_ErrorResourcesLost, NULL);

	return OMX_ErrorNone;
}

OMX_ERRORTYPE oscl_disp_update_layer_data(void *hdl, void *layer,
							layer_data_t *data)
{
	disp_hdl_t *vdisp = (disp_hdl_t *)hdl;
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	OMX_S32 pix_format = 0;
	dc_win_data_t win_data;

	oscl_param_check((NULL != vdisp), OMX_ErrorBadParameter, NULL);
	oscl_param_check((NULL != layer), OMX_ErrorBadParameter, NULL);
	oscl_param_check((NULL != data), OMX_ErrorBadParameter, NULL);

	/* oscl_disp_lock(); */
	memset(&win_data, 0, sizeof(win_data));
	pix_format = omxformat_to_dispformat(data->eColorFormat);
	if ((-1 == pix_format)) {
		/* oscl_disp_unlock(); */
		return OMX_ErrorBadParameter;
	}
	win_data.pixel_format = pix_format;
	win_data.pixel_order = DC_PO_NORMAL;
	win_data.bpp = 32;
	win_data.update_flag = true;

	/* crop picture size */
	win_data.fb_width = data->crop_rect.width;
	win_data.fb_height = data->crop_rect.height;
	win_data.fb_x = data->crop_rect.left;
	win_data.fb_y = data->crop_rect.top;

	/* display positaion in screen*/
	win_data.crtc_x = data->win_rect.left;
	win_data.crtc_y = data->win_rect.top;
	win_data.crtc_width = data->win_rect.width;
	win_data.crtc_height = data->win_rect.height;

	/* source picture size */
	win_data.src_width = data->src_size.width;
	win_data.src_height = data->src_size.height;

	win_data.dma_addr = (dma_addr_t)data->addr[0];
	win_data.chroma_dma_addr = (dma_addr_t)data->addr[1];
	win_data.chroma_x_dma_addr = (dma_addr_t)data->addr[2];
	if (0 == vdisp->dgb_flag) {
		vdisp->dgb_flag = 1;
		OSCL_LOGI("%s >> src[%d %d] crtc[%d %d %d %d]",
			vdisp->layer_name,
			win_data.src_width, win_data.src_height,
			win_data.crtc_x, win_data.crtc_y, win_data.crtc_width,
			win_data.crtc_height);
		OSCL_LOGI("fb[%d %d %d %d] addr[%x %x %x]",
			win_data.fb_x, win_data.fb_y,
			win_data.fb_width, win_data.fb_height,
			win_data.dma_addr, win_data.chroma_dma_addr,
			win_data.chroma_x_dma_addr);
	}
	ret = vdisp->disp_ops->disp_win_update(layer, &win_data);
	/* oscl_disp_unlock(); */
	oscl_param_check((OMX_ErrorNone == ret), OMX_ErrorResourcesLost, NULL);

	return ret;
}

/* Crop rotate not support */
OMX_ERRORTYPE oscl_disp_hw_rotate(void *hdl, vdisp_rotate_mode_e mode,
			rotate_buffer_t *src, rotate_buffer_t *dst)
{
	disp_hdl_t *vdisp = (disp_hdl_t *)hdl;
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	OMX_U32 img_w, img_h;
	disp_rot_cfg_t cfgs;
	disp_rect_t src_size;
	OMX_U32 pre_time, cur_time;

	OSCL_LOGD("b>> mode:%d src[%d %d %d %d] disp[%d %d %d %d]",
		mode,
		src->src_size.left, src->src_size.top,
		src->src_size.width, src->src_size.height,
		src->disp_size.top, src->disp_size.left,
		src->disp_size.width, src->disp_size.height);
	src_size.left = 0;
	src_size.top = 0;
	src_size.width = src->src_size.width;
	src_size.height = src->src_size.height;
	memset(&cfgs, 0, sizeof(cfgs));
	cfgs.mode = omxrot_to_disprot_mode(mode);

	/* input para */
	if (DISP_FORMAT_NV12 == omxformat_to_dispformat(src->eColorFormat)) {
		oscl_param_check((0 == (((OMX_U32)src->addr[0]) % ROT_ADDR_ALIGN)),
			OMX_ErrorBadParameter, NULL);
		oscl_param_check((0 == (((OMX_U32)src->addr[1]) % ROT_ADDR_ALIGN)),
			OMX_ErrorBadParameter, NULL);
		img_w = src_size.width;
		img_h = src_size.height;
		ret = omxformat_to_dispformat(src->eColorFormat);
		oscl_param_check((-1 != ret), OMX_ErrorBadParameter, NULL);
		cfgs.infb.format = ret;
		cfgs.infb.addr[0] = (unsigned int)src->addr[0];
		cfgs.infb.width[0] = img_w;
		cfgs.infb.height[0] = img_h;
		cfgs.infb.addr[1] = (unsigned int)src->addr[1];
		cfgs.infb.width[1] = img_w >> 1;
		cfgs.infb.height[1] = img_h >> 1;
	} else {
		OSCL_LOGE("Input format not support.");
		return OMX_ErrorBadParameter;
	}

	/* output para */
	if (DISP_FORMAT_NV12 == omxformat_to_dispformat(dst->eColorFormat)) {
		oscl_param_check((0 == (((OMX_U32)src->addr[0]) % ROT_ADDR_ALIGN)),
			OMX_ErrorBadParameter, NULL);
		oscl_param_check((0 == (((OMX_U32)src->addr[1]) % ROT_ADDR_ALIGN)),
			OMX_ErrorBadParameter, NULL);
		img_w = dst->src_size.width;
		img_h = dst->src_size.height;
		ret = omxformat_to_dispformat(dst->eColorFormat);
		oscl_param_check((-1 != ret), OMX_ErrorBadParameter, NULL);
		cfgs.outfb.format = ret;
		cfgs.outfb.width[0] = img_w;
		cfgs.outfb.height[0] = img_h;
		cfgs.outfb.addr[0] = (unsigned int)dst->addr[0];
		cfgs.outfb.width[1] = img_w >> 1;
		cfgs.outfb.height[1] = img_h >> 1;
		cfgs.outfb.addr[1] = (unsigned int)dst->addr[1];
	} else {
		OSCL_LOGE("Output format not support.");
		return OMX_ErrorBadParameter;
	}
	pre_time = oscl_get_msec();
	/* rotate process */
	ret = vdisp->disp_ops->disp_rot_process(&cfgs);
	if (0 != ret) {
		OSCL_LOGE("Rotate fail.");
		ret = OMX_ErrorInsufficientResources;
	}
	cur_time = oscl_get_msec();
/*	if ((cur_time - pre_time) > 40) {
		OSCL_LOGW("Rotate wast:%d, src[w:%d h:%d]",
			(cur_time - pre_time),
			src->src_size.width, src->src_size.height);
	}*/
	OSCL_LOGD("a>>src[%d %d %d %d] disp[%d %d %d %d]",
		dst->src_size.left, dst->src_size.top,
		dst->src_size.width, dst->src_size.height,
		dst->disp_size.top, dst->disp_size.left,
		dst->disp_size.width, dst->disp_size.height);

	return ret;
}

