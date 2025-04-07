/*
 * oscl_eos_vrec.c - eos video receive api used by lombo media framework.
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

#define DBG_LEVEL		DBG_ERR
#define LOG_TAG			"oscl_vrec"
#include "oscl_eos_vrec.h"
#include "pthread.h"

typedef struct vrec_ctx {
	char name[16];
	char cnt;
	vrec_hdl_t *hdl;
} vrec_ctx_t;

static pthread_mutex_t vctx_lock = {.attr = -1};
static vrec_ctx_t g_vctx[8];

vrec_pixformat_map_index_t vrec_pix_format_map_table[] = {
	{VISS_PIX_FMT_NV12, OMX_COLOR_FormatYUV420SemiPlanar, 2},
};

vrec_image_mode_index_t vrec_image_mode_map_table[] = {
	{0, IMAGE_MODE_1_NORMAL},
	{1, IMAGE_MODE_4_STITCH_CROSS},
	{2, IMAGE_MODE_2_STITCH_H},
};

static OMX_COLOR_FORMATTYPE sensorformat_to_omxformat(OMX_S32 format)
{
	OMX_S32 i = 0;
	vrec_pixformat_map_index_t *pix_map = NULL;

	for (i = 0; i < ARRAY_SIZE(vrec_pix_format_map_table); i++) {
		pix_map = &vrec_pix_format_map_table[i];
		if (pix_map->sensor_format == format)
			return pix_map->omx_format;
	}

	return -1;
}

static image_mode_e buftype_to_mode(OMX_U32 type)
{
	OMX_S32 i = 0;
	vrec_image_mode_index_t *mode_map = NULL;

	for (i = 0; i < ARRAY_SIZE(vrec_image_mode_map_table); i++) {
		mode_map = &vrec_image_mode_map_table[i];
		if (mode_map->buf_type == type)
			return mode_map->mode;
	}

	return -1;
}

vrec_ctx_t *find_vctx_for_name(const char *name)
{
	vrec_ctx_t *ctx = NULL;
	OMX_S32 i = 0;

	pthread_mutex_lock(&vctx_lock);
	for (i = 0; i < ARRAY_SIZE(g_vctx); i++) {
		if (g_vctx[i].hdl && (0 == strcmp(g_vctx[i].name, name))) {
			ctx = &g_vctx[i];
			break;
		}
	}
	pthread_mutex_unlock(&vctx_lock);

	return ctx;
}

vrec_ctx_t *find_vctx_for_handle(vrec_hdl_t *vrec)
{
	vrec_ctx_t *ctx = NULL;
	OMX_S32 i = 0;

	pthread_mutex_lock(&vctx_lock);
	for (i = 0; i < ARRAY_SIZE(g_vctx); i++) {
		if (g_vctx[i].hdl && (g_vctx[i].hdl == vrec)) {
			ctx = &g_vctx[i];
			break;
		}
	}
	pthread_mutex_unlock(&vctx_lock);

	return ctx;
}

OMX_ERRORTYPE vctx_add(const char *name, vrec_hdl_t *hdl)
{
	vrec_ctx_t *ctx = NULL;
	OMX_S32 i = 0;

	oscl_param_check(hdl != NULL, OMX_ErrorBadParameter, NULL);
	oscl_param_check(name != NULL, OMX_ErrorBadParameter, NULL);
	pthread_mutex_lock(&vctx_lock);
	for (i = 0; i < ARRAY_SIZE(g_vctx); i++) {
		if (g_vctx[i].hdl == NULL) {
			ctx = &g_vctx[i];
			break;
		}
	}
	if (ctx) {
		strncpy(ctx->name, name, sizeof(ctx->name));
		ctx->hdl = hdl;
		ctx->cnt++;
	}
	pthread_mutex_unlock(&vctx_lock);
	if (ctx)
		return OMX_ErrorNone;
	else {
		OSCL_LOGE("video handle add fail.");
		return OMX_ErrorInsufficientResources;
	}
}

OMX_ERRORTYPE vctx_del(vrec_ctx_t *ctx)
{
	OMX_S32 i = 0;
	OMX_S32 found = 0;

	oscl_param_check(ctx != NULL, OMX_ErrorBadParameter, NULL);
	pthread_mutex_lock(&vctx_lock);
	for (i = 0; i < ARRAY_SIZE(g_vctx); i++) {
		if (&g_vctx[i] == ctx) {
			found = 1;
			break;
		}
	}
	if (found == 0) {
		OSCL_LOGE("delete video vrec fail.");
		pthread_mutex_unlock(&vctx_lock);
		return OMX_ErrorInsufficientResources;
	}
	ctx->cnt--;
	if (ctx->cnt <= 0) {
		ctx->hdl = NULL;
		memset(ctx->name, 0, sizeof(ctx->name));
	}
	pthread_mutex_unlock(&vctx_lock);
	return OMX_ErrorNone;
}

OMX_ERRORTYPE vctx_vrec_inc(vrec_ctx_t *ctx)
{
	oscl_param_check(ctx != NULL, OMX_ErrorBadParameter, NULL);
	pthread_mutex_lock(&vctx_lock);
	ctx->cnt++;
	pthread_mutex_unlock(&vctx_lock);
	return OMX_ErrorNone;
}

OMX_ERRORTYPE vctx_vrec_dec(vrec_ctx_t *ctx)
{
	oscl_param_check(ctx != NULL, OMX_ErrorBadParameter, NULL);
	pthread_mutex_lock(&vctx_lock);
	if (ctx->cnt > 0)
		ctx->cnt--;
	pthread_mutex_unlock(&vctx_lock);
	return OMX_ErrorNone;
}


void *oscl_open_vrec(const char *name)
{
	vrec_hdl_t *vrec = NULL;
	vrec_ctx_t *ctx = NULL;
	OMX_S32 ret = 0;

	oscl_param_check((NULL != name), NULL, NULL);
	ctx = find_vctx_for_name(name);
	if (ctx) {
		vctx_vrec_inc(ctx);

		return ctx->hdl;
	}
	vrec = oscl_zalloc(sizeof(vrec_hdl_t));
	oscl_param_check((NULL != vrec), NULL, NULL);
	vrec->vrec_device = rt_device_find(name);
	if (NULL == vrec->vrec_device)
		goto EXIT;
	ret = rt_device_open(vrec->vrec_device, 0);
	if (RT_EOK != ret)
		goto EXIT;
	ret = rt_device_control(vrec->vrec_device, VISS_CMD_INIT, 0);
	if (RT_EOK != ret) {
		LOG_E("VISS_CMD_INIT. ret:%d", ret);
		goto EXIT1;
	}
	rt_device_control(vrec->vrec_device, VISS_CMD_GET_WAIT_FRAME_SEM,
			(void *)&vrec->event_hdl);
	strncpy(vrec->name, name, sizeof(vrec->name) - 1);
	vctx_add(name, vrec);

	return vrec;
EXIT1:
	rt_device_close(vrec->vrec_device);
EXIT:
	oscl_free(vrec);

	return NULL;
}

void oscl_close_vrec(void *hdl)
{
	vrec_hdl_t *vrec = (vrec_hdl_t *)hdl;
	struct viss_frame_queue *queue = NULL;
	vrec_ctx_t *ctx = NULL;

	ctx = find_vctx_for_handle(hdl);
	if (ctx == NULL) {
		OSCL_LOGE("close vrec fail. %p", hdl);
		return;
	}
	vctx_del(ctx);
	if (vrec->info.format) {
		oscl_free(vrec->info.format);
		vrec->info.format = NULL;
	}
	if (vrec->frame_q) {
		queue = (struct viss_frame_queue *)vrec->frame_q;
		oscl_free(queue->frame);
		oscl_free(queue);
		vrec->frame_q = NULL;
	}
	if (ctx->cnt <= 0) {
		if (vrec && vrec->vrec_device) {
			rt_sem_release(vrec->event_hdl);
			rt_device_control(vrec->vrec_device, VISS_CMD_EXIT, RT_NULL);
			vrec->event_hdl = NULL;
			rt_device_close(vrec->vrec_device);
			oscl_free(vrec);
		}
	}
}

OMX_ERRORTYPE oscl_open_isp(void *hdl, const char *name)
{
	return OMX_ErrorNone;
}

void oscl_close_isp(void *hdl)
{
	vrec_hdl_t *vrec = (vrec_hdl_t *)hdl;

	if (NULL == vrec)
		return;
}

/* timeout unit 10ms */
OMX_S32 oscl_vrec_wait_frame(void *hdl, OMX_S32 timeout)
{
	vrec_hdl_t *vrec = (vrec_hdl_t *)hdl;
	OMX_S32 ret = 0;

	if (vrec && vrec->event_hdl)
		ret = rt_sem_take(vrec->event_hdl, timeout);
	return ret;
}

OMX_ERRORTYPE oscl_vrec_get_info(void *hdl, vrec_info_t *info)
{
	vrec_hdl_t *vrec = (vrec_hdl_t *)hdl;
	struct dev_all_mode all_mode;
	OMX_S32 i = 0;
	OMX_U32 frame_buf_type = 0;

	oscl_param_check(vrec, OMX_ErrorBadParameter, NULL);
	oscl_param_check(info, OMX_ErrorBadParameter, NULL);
	oscl_param_check(vrec->vrec_device, OMX_ErrorBadParameter, NULL);
	rt_device_control(vrec->vrec_device, VISS_CMD_GET_CHANNEL_NUM,
			(void *)&vrec->info.channel_num);
	if (0 == vrec->info.channel_num) {
		OSCL_LOGE("Get channel number error.");
		return OMX_ErrorResourcesLost;
	}
	if (vrec->info.channel_num > 1) {
		rt_device_control(vrec->vrec_device, VISS_CMD_GET_FRAME_BUF_TYPE,
			(void *)&frame_buf_type);
		vrec->info.image_mode = buftype_to_mode(frame_buf_type);
		if (vrec->info.image_mode < 0) {
			OSCL_LOGE("Get sensor information fail.");
			return -1;
		}
	}
	memset(&all_mode, 0, sizeof(all_mode));
	rt_device_control(vrec->vrec_device, VISS_CMD_GET_ALL_MODE,
					(void *)&all_mode);
	oscl_param_check(all_mode.num, OMX_ErrorInsufficientResources, NULL);
	vrec->info.fmt_num = all_mode.num;
	vrec->info.format = oscl_zalloc(sizeof(vrec_format_t) * vrec->info.fmt_num);
	oscl_param_check(vrec->info.format, OMX_ErrorInsufficientResources, NULL);
	for (i = 0; i < all_mode.num; i++) {
		vrec->info.format[i].index = all_mode.mode[i].index;
		vrec->info.format[i].format =
			sensorformat_to_omxformat(all_mode.mode[i].out_fmt);
		vrec->info.format[i].frame_size.nTop = all_mode.mode[i].frame_size.top;
		vrec->info.format[i].frame_size.nLeft = all_mode.mode[i].frame_size.left;
		vrec->info.format[i].frame_size.nWidth =
					all_mode.mode[i].frame_size.width;
		vrec->info.format[i].frame_size.nHeight =
					all_mode.mode[i].frame_size.height;
		vrec->info.format[i].frame_rate = all_mode.mode[i].frame_rate;
	}
	memcpy(info, &vrec->info, sizeof(vrec_info_t));

	return OMX_ErrorNone;
}

OMX_ERRORTYPE oscl_vrec_set_format(void *hdl, vrec_format_t *format)
{
	vrec_hdl_t *vrec = (vrec_hdl_t *)hdl;
	OMX_S32 ret = 0;

	oscl_param_check(vrec, OMX_ErrorBadParameter, NULL);
	oscl_param_check(format, OMX_ErrorBadParameter, NULL);
	ret = rt_device_control(vrec->vrec_device, VISS_CMD_SET_MODE,
					(void *)format->index);
	oscl_param_check((RT_EOK == ret), OMX_ErrorInsufficientResources, NULL);

	return OMX_ErrorNone;
}

OMX_ERRORTYPE oscl_vrec_get_cur_format(void *hdl, vrec_format_t *format)
{
	vrec_hdl_t *vrec = (vrec_hdl_t *)hdl;
	struct dev_mode *cur_mode = RT_NULL;
	OMX_S32 ret = 0;

	oscl_param_check(vrec, OMX_ErrorBadParameter, NULL);
	oscl_param_check(format, OMX_ErrorBadParameter, NULL);
	ret = rt_device_control(vrec->vrec_device, VISS_CMD_GET_CUR_MODE,
					(void *)&cur_mode);
	oscl_param_check((ret == RT_EOK), OMX_ErrorInsufficientResources, NULL);
	format->index = cur_mode->index;
	format->format = sensorformat_to_omxformat(cur_mode->out_fmt);
	format->frame_size.nTop = cur_mode->frame_size.top;
	format->frame_size.nLeft = cur_mode->frame_size.left;
	format->frame_size.nWidth = cur_mode->frame_size.width;
	format->frame_size.nHeight = cur_mode->frame_size.height;
	format->frame_rate = cur_mode->frame_rate;

	return OMX_ErrorNone;
}

OMX_ERRORTYPE oscl_vrec_set_buffer(void *hdl, vrec_frame_queue_t *frame_q)
{
	vrec_hdl_t *vrec = (vrec_hdl_t *)hdl;
	struct viss_frame_queue *queue = NULL;
	OMX_S32 i = 0;
	OMX_U32 y_base_addr = 0;
	OMX_U32 cb_base_addr = 0;
	OMX_U32 cr_base_addr = 0;
	OMX_U32 y_size = 0;
	OMX_U32 cb_size = 0;
	OMX_U32 cr_size = 0;
	OMX_U32 align = 0;

	oscl_param_check(vrec, OMX_ErrorBadParameter, NULL);
	oscl_param_check(frame_q, OMX_ErrorBadParameter, NULL);
	oscl_param_check(frame_q->frame_num, OMX_ErrorBadParameter, NULL);

	if (vrec->frame_q)
		oscl_free(vrec->frame_q);
	queue = oscl_zalloc(sizeof(struct viss_frame_queue));
	oscl_param_check(queue, OMX_ErrorInsufficientResources, NULL);
	queue->frame = oscl_zalloc(frame_q->frame_num * sizeof(struct viss_buffer));
	if (NULL == queue->frame) {
		oscl_free(queue);
		return OMX_ErrorInsufficientResources;
	}

	if (IMAGE_MODE_1_NORMAL == frame_q->image_mode) {
		for (i = 0; i < frame_q->frame_num; i++) {
			align = frame_q->frame_addr[i].align;
			y_size = frame_q->frame_addr[i].size[0];
			cb_size = frame_q->frame_addr[i].size[1];
			cr_size = frame_q->frame_addr[i].size[2];
			y_base_addr = (rt_uint32_t)frame_q->frame_addr[i].addr[0];
			cb_base_addr = (rt_uint32_t)frame_q->frame_addr[i].addr[1];
			cr_base_addr = (rt_uint32_t)frame_q->frame_addr[i].addr[2];

			queue->frame[i].paddr[0].align = align;
			queue->frame[i].paddr[0].y = y_base_addr;
			queue->frame[i].paddr[0].y_size = y_size;
			queue->frame[i].paddr[0].cb = cb_base_addr;
			queue->frame[i].paddr[0].cb_size = cb_size;
			queue->frame[i].paddr[0].cr = cr_base_addr;
			queue->frame[i].paddr[0].cr_size = cr_size;
		}
	} else if (IMAGE_MODE_4_STITCH_CROSS == frame_q->image_mode) {
		for (i = 0; i < frame_q->frame_num; i++) {
			align = frame_q->frame_addr[i].align;
			y_size = frame_q->ch_size[0];
			cb_size = frame_q->ch_size[1];
			cr_size = frame_q->ch_size[2];
			y_base_addr = (rt_uint32_t)frame_q->frame_addr[i].addr[0];
			cb_base_addr = (rt_uint32_t)frame_q->frame_addr[i].addr[1];
			cr_base_addr = (rt_uint32_t)frame_q->frame_addr[i].addr[2];

			queue->frame[i].paddr[0].align = align;
			queue->frame[i].paddr[0].y = y_base_addr;
			queue->frame[i].paddr[0].y_size = y_size;
			queue->frame[i].paddr[0].cb = cb_base_addr;
			queue->frame[i].paddr[0].cb_size = cb_size;
			queue->frame[i].paddr[0].cr = cr_base_addr;
			queue->frame[i].paddr[0].cr_size = cr_size;

			queue->frame[i].paddr[1].align = align;
			queue->frame[i].paddr[1].y = y_base_addr + frame_q->ch_width;
			queue->frame[i].paddr[1].y_size = y_size;
			queue->frame[i].paddr[1].cb = cb_base_addr + frame_q->ch_width;
			queue->frame[i].paddr[1].cb_size = cb_size;
			queue->frame[i].paddr[1].cr = cr_base_addr;
			queue->frame[i].paddr[1].cr_size = cr_size;

			queue->frame[i].paddr[2].align = align;
			queue->frame[i].paddr[2].y = y_base_addr + (y_size * 2);
			queue->frame[i].paddr[2].y_size = y_size;
			queue->frame[i].paddr[2].cb = cb_base_addr + (cb_size * 2);
			queue->frame[i].paddr[2].cb_size = cb_size;
			queue->frame[i].paddr[2].cr = cr_base_addr;
			queue->frame[i].paddr[2].cr_size = cr_size;

			queue->frame[i].paddr[3].align = align;
			queue->frame[i].paddr[3].y = y_base_addr +
				frame_q->ch_width + (y_size * 2);
			queue->frame[i].paddr[3].y_size = y_size;
			queue->frame[i].paddr[3].cb = cb_base_addr +
				frame_q->ch_width + (cb_size * 2);
			queue->frame[i].paddr[3].cb_size = cb_size;
			queue->frame[i].paddr[3].cr = cr_base_addr;
			queue->frame[i].paddr[3].cr_size = cr_size;
		}
	} else if (IMAGE_MODE_2_STITCH_H == frame_q->image_mode) {
		for (i = 0; i < frame_q->frame_num; i++) {
			align = frame_q->frame_addr[i].align;
			y_size = frame_q->ch_size[0];
			cb_size = frame_q->ch_size[1];
			cr_size = frame_q->ch_size[2];
			y_base_addr = (rt_uint32_t)frame_q->frame_addr[i].addr[0];
			cb_base_addr = (rt_uint32_t)frame_q->frame_addr[i].addr[1];
			cr_base_addr = (rt_uint32_t)frame_q->frame_addr[i].addr[2];

			queue->frame[i].paddr[0].align = align;
			queue->frame[i].paddr[0].y = y_base_addr;
			queue->frame[i].paddr[0].y_size = y_size;
			queue->frame[i].paddr[0].cb = cb_base_addr;
			queue->frame[i].paddr[0].cb_size = cb_size;
			queue->frame[i].paddr[0].cr = cr_base_addr;
			queue->frame[i].paddr[0].cr_size = cr_size;

			queue->frame[i].paddr[1].align = align;
			queue->frame[i].paddr[1].y = y_base_addr + y_size;
			queue->frame[i].paddr[1].y_size = y_size;
			queue->frame[i].paddr[1].cb = cb_base_addr + cb_size;
			queue->frame[i].paddr[1].cb_size = cb_size;
			queue->frame[i].paddr[1].cr = cr_base_addr + cr_size;
			queue->frame[i].paddr[1].cr_size = cr_size;

		}
	} else {
		return OMX_ErrorBadParameter;
	}
	queue->all_num = frame_q->frame_num;
	/* 4. set frame buffer to vic driver */
	rt_device_control(vrec->vrec_device, VISS_CMD_SET_FRAME_QUEUE,
				(void *)queue);
	vrec->frame_q = queue;

	return OMX_ErrorNone;
}

OMX_ERRORTYPE oscl_vrec_clean_buffer(void *hdl)
{
	vrec_hdl_t *vrec = (vrec_hdl_t *)hdl;
	OMX_S32 ret = 0;

	oscl_param_check(hdl, OMX_ErrorBadParameter, NULL);
	ret = rt_device_control(vrec->vrec_device, VISS_CMD_CLEANUP_FRAME_QUEUE, NULL);
	if (RT_EOK != ret)
		return OMX_ErrorInsufficientResources;

	return OMX_ErrorNone;
}

OMX_ERRORTYPE oscl_vrec_set_gpts_cb(void *hdl, vrec_pts_cb_t *callback)
{
	vrec_hdl_t *vrec = (vrec_hdl_t *)hdl;
	struct viss_pts_callback viss_pts_cb;
	OMX_S32 ret = 0;

	oscl_param_check(hdl, OMX_ErrorBadParameter, NULL);
	rt_memcpy(&viss_pts_cb, callback, sizeof(viss_pts_cb));
	ret = rt_device_control(vrec->vrec_device,
			VISS_CMD_SET_PTS_CB, (void *)&viss_pts_cb);
	if (RT_EOK != ret)
		return OMX_ErrorInsufficientResources;

	return OMX_ErrorNone;
}

OMX_ERRORTYPE oscl_vrec_request_frame(void *hdl, vrec_reqframe_t *vrec_frame)
{
	vrec_hdl_t *vrec = (vrec_hdl_t *)hdl;
	OMX_S32 ret = 0;
	struct viss_reqframe req_frm;

	oscl_param_check(hdl, OMX_ErrorBadParameter, NULL);
	oscl_param_check(vrec_frame, OMX_ErrorBadParameter, NULL);
	req_frm.channel_id = vrec_frame->channel_id;
	ret = rt_device_control(vrec->vrec_device, VISS_CMD_REQUEST_FRAME,
								(void *)&req_frm);
	if (RT_EOK != ret)
		return OMX_ErrorInsufficientResources;
	vrec_frame->frame_id = req_frm.buf->frame_id;
	vrec_frame->pts = req_frm.buf->pts;
	vrec_frame->pts_valid = req_frm.buf->pts_valid;
	vrec_frame->pix_format = sensorformat_to_omxformat(req_frm.buf->pix_format);

	vrec_frame->paddr.addr[0] = (OMX_U8 *)req_frm.buf->paddr[0].y;
	vrec_frame->paddr.size[0] = req_frm.buf->paddr[0].y_size;
	vrec_frame->paddr.addr[1] = (OMX_U8 *)req_frm.buf->paddr[0].cb;
	vrec_frame->paddr.size[1] = req_frm.buf->paddr[0].cb_size;
	vrec_frame->paddr.addr[2] = (OMX_U8 *)req_frm.buf->paddr[0].cr;
	vrec_frame->paddr.size[2] = req_frm.buf->paddr[0].cr_size;

	vrec_frame->src_rect.nTop = req_frm.buf->src_rect.top;
	vrec_frame->src_rect.nLeft = req_frm.buf->src_rect.left;
	vrec_frame->src_rect.nWidth = req_frm.buf->src_rect.width;
	vrec_frame->src_rect.nHeight = req_frm.buf->src_rect.height;
	vrec_frame->dst_rect.nTop = req_frm.buf->dst_rect.top;
	vrec_frame->dst_rect.nLeft = req_frm.buf->dst_rect.left;
	vrec_frame->dst_rect.nWidth = req_frm.buf->dst_rect.width;
	vrec_frame->dst_rect.nHeight = req_frm.buf->dst_rect.height;

	vrec_frame->_private = req_frm.buf;
	return OMX_ErrorNone;
}

OMX_ERRORTYPE oscl_vrec_release_frame(void *hdl, vrec_reqframe_t *vrec_frame)
{
	vrec_hdl_t *vrec = (vrec_hdl_t *)hdl;
	OMX_S32 ret = 0;
	struct viss_reqframe req_frm;

	oscl_param_check(vrec_frame, OMX_ErrorBadParameter, NULL);
	memset(&req_frm, 0, sizeof(req_frm));
	req_frm.channel_id = vrec_frame->channel_id;
	req_frm.buf = (struct viss_buffer *)vrec_frame->_private;
	ret = rt_device_control(vrec->vrec_device, VISS_CMD_RELEASE_FRAME,
								(void *)&req_frm);
	oscl_param_check((RT_EOK == ret), OMX_ErrorInsufficientResources, NULL);

	return OMX_ErrorNone;
}

OMX_ERRORTYPE oscl_vrec_capture_on(void *hdl, OMX_S32 ch)
{
	vrec_hdl_t *vrec = (vrec_hdl_t *)hdl;
	OMX_S32 ret = 0;

	OSCL_LOGD("oscl_vrec_capture_on ch:%d", ch);
	ret = rt_device_control(vrec->vrec_device, VISS_CMD_CAPTURE_ON, (void *)ch);
	oscl_param_check((RT_EOK == ret), OMX_ErrorInsufficientResources, NULL);

	return OMX_ErrorNone;
}

OMX_ERRORTYPE oscl_vrec_capture_off(void *hdl, OMX_S32 ch)
{
	vrec_hdl_t *vrec = (vrec_hdl_t *)hdl;
	rt_sem_t isp_drv_wait_sem = NULL;
	OMX_S32 ret = 0;

	OSCL_LOGD("oscl_vrec_capture_off ch:%d", ch);
	ret = rt_device_control(vrec->vrec_device, VISS_CMD_CAPTURE_OFF, (void *)ch);
	if (RT_EOK != ret)
		return OMX_ErrorInsufficientResources;
	if (strncmp("isp", vrec->name, 3) == 0) {
		isp_drv_wait_sem = NULL;
		rt_device_control(vrec->vrec_device,
			ISP_CMD_GET_WAIT_DRV_SEM,
			(void *)&isp_drv_wait_sem);
		OSCL_LOGW("======wait for isp_drv_wait_sem====");
		if (NULL == isp_drv_wait_sem)
			OSCL_LOGE("isp_drv_wait_sem is null");
		else {
			while (rt_sem_take(isp_drv_wait_sem, 100))
				OSCL_LOGE("wait for isp sem timeout!");
		}
	}

	return OMX_ErrorNone;
}


