/*
 * vrec_component.c - video receive component.
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

#define DBG_LEVEL		DBG_WARNING
#define LOG_TAG			"vrec_component"

#include <oscl.h>
#include <base_component.h>
#include "vrec_component.h"

#define VREC_DRV_NAME_MAX_LENGTH	(32)
#define DEFAULT_VIDEO_WIDTH		(1920)
#define DEFAULT_VIDEO_HEIGHT		(1080)
#define DEFAULT_BUFFER_NUM		(4)
#define DEFAULT_BUFFER_ALIGN_SIZE	(4096)
#define VREC_ALIGN(x, a)		(((x) + (a) - 1) & ~((a) - 1))

typedef struct vrec_private {
	char  dev_name[VREC_DRV_NAME_MAX_LENGTH]; /* sensor driver name */
	char  isp_name[VREC_DRV_NAME_MAX_LENGTH]; /* isp exif */
	vrec_info_t info;	/* sensor format */
	void *vrec_dev;		/* sendor driver handle */
	vrec_frame_queue_t frame_queue;
	pthread_mutex_t buf_mutex;
	OMX_TICKS ref_time;
	OMX_TICKS cur_time;
} vrec_private_t;

static OMX_S32 vrec_get_pts(OMX_S64 *pts, void *para)
{
	vrec_private_t *source_private = NULL;

	source_private = (vrec_private_t *)para;
	if (source_private)
		source_private->cur_time = oscl_get_msec();
	if (pts)
		*pts = source_private->cur_time;
	else
		return -1;

	return 0;
}

static OMX_ERRORTYPE vrec_dev_init(lb_omx_component_t *component)
{
	vrec_private_t *source_private = NULL;
	vrec_pts_cb_t pts_cb;

	OSCL_LOGI("viss init..");
	source_private = component->component_private;

	pts_cb.vrec_get_pts = vrec_get_pts;
	pts_cb.para = source_private;
	OSCL_LOGI("Set pts source_private:%p", source_private);
	oscl_vrec_set_gpts_cb(source_private->vrec_dev, &pts_cb);
	OSCL_LOGI("Vrec init success..");

	return OMX_ErrorNone;
}

static OMX_ERRORTYPE vrec_dev_start(lb_omx_component_t *component)
{
	OMX_S32 buffer_num = 0;
	OMX_S32 i = 0;
	OMX_S32 ret = 0;
	OMX_U32 plane_size[3] = {0};
	OMX_BUFFERHEADERTYPE *outbuffer = NULL;
	oscl_message_t *cur_pos = NULL;
	oscl_message_t *message = NULL;
	vrec_private_t *source_private = NULL;
	base_port_t *outport = NULL;

	OSCL_LOGI("viss start..");
	source_private = component->component_private;
	outport = &component->port[SOURCE_OUTPUT_PORT_INDEX];
	buffer_num = oscl_queue_get_num(&outport->buffer_queue);
	oscl_param_check(buffer_num > 0, OMX_ErrorBadParameter, NULL);

	memset(&source_private->frame_queue, 0,
		sizeof(source_private->frame_queue));
	source_private->frame_queue.frame_num = buffer_num;
	source_private->frame_queue.ch_width =
		source_private->info.format->frame_size.nWidth;
	source_private->frame_queue.ch_height =
		source_private->info.format->frame_size.nHeight;
	source_private->frame_queue.image_mode = source_private->info.image_mode;
	source_private->frame_queue.channel_num = source_private->info.channel_num;
	source_private->frame_queue.frame_addr = oscl_zalloc(buffer_num
						* sizeof(vrec_frame_addr_t));
	ret = calc_frame_size(outport->port_param.format.video.eColorFormat,
			source_private->frame_queue.ch_width,
			source_private->frame_queue.ch_height,
			plane_size, 1);
	oscl_param_check((OMX_ErrorNone == ret), ret, NULL);
	source_private->frame_queue.ch_size[0] = plane_size[0];
	source_private->frame_queue.ch_size[1] = plane_size[1];
	source_private->frame_queue.ch_size[2] = plane_size[2];
	OSCL_LOGI("format:%d, w:%d h:%d align:%d",
		outport->port_param.format.video.eColorFormat,
		outport->port_param.format.video.nFrameWidth,
		outport->port_param.format.video.nFrameHeight,
		outport->port_param.nBufferAlignment);

	ret = calc_frame_size(outport->port_param.format.video.eColorFormat,
			outport->port_param.format.video.nFrameWidth,
			outport->port_param.format.video.nFrameHeight,
			plane_size, outport->port_param.nBufferAlignment);
	oscl_param_check((OMX_ErrorNone == ret), ret, NULL);
	OSCL_LOGI("frame size:%d %d %d\n", plane_size[0], plane_size[1], plane_size[2]);
	for (i = 0; i < buffer_num; i++) {
		message = (oscl_message_t *)oscl_queue_query_next(&outport->buffer_queue,
				cur_pos);
		if (message && message->data) {
			outbuffer = (OMX_BUFFERHEADERTYPE *)message->data;
			source_private->frame_queue.frame_addr[i].align =
					outport->port_param.nBufferAlignment;
			source_private->frame_queue.frame_addr[i].frame_index = i;
			if (plane_size[0] > 0) {
				source_private->frame_queue.frame_addr[i].addr[0] =
							outbuffer->pBuffer;
				source_private->frame_queue.frame_addr[i].size[0] =
							plane_size[0];
			}
			if (plane_size[1] > 0) {
				source_private->frame_queue.frame_addr[i].addr[1] =
					(outbuffer->pBuffer + plane_size[0]);
				source_private->frame_queue.frame_addr[i].size[1] =
							plane_size[1];
			}
			if (plane_size[2] > 0) {
				source_private->frame_queue.frame_addr[i].addr[2] =
					(outbuffer->pBuffer + plane_size[0]
						+ plane_size[1]);
				source_private->frame_queue.frame_addr[i].size[2] =
							plane_size[2];
			}
			OSCL_LOGI("index:%d y:%p(%d) cb:%p(%d) cr:%p(%d)", i,
				source_private->frame_queue.frame_addr[i].addr[0],
				plane_size[0],
				source_private->frame_queue.frame_addr[i].addr[1],
				plane_size[1],
				source_private->frame_queue.frame_addr[i].addr[2],
				plane_size[2]);
			cur_pos = message;
		} else {
			OSCL_LOGE("carfull.... buffer error.i:%d\n", i);
			return OMX_ErrorInsufficientResources;
		}
	}

	/* 4. set frame buffer to vic driver */
	oscl_vrec_set_buffer(source_private->vrec_dev, &source_private->frame_queue);

	/* 5. start capture */
	OSCL_LOGI("source_private->channel_num:%d", source_private->info.channel_num);
#if 0
	for (i = 0; i < source_private->info.channel_num; i++) {
		ret = oscl_vrec_capture_on(source_private->vrec_dev, i);
		if (OMX_ErrorNone != ret)
			return OMX_ErrorInsufficientResources;
	}
#else
	ret = oscl_vrec_capture_on(source_private->vrec_dev, 0);
	if (OMX_ErrorNone != ret)
		return OMX_ErrorInsufficientResources;

#endif
	OSCL_LOGI("viss start success...");
	return OMX_ErrorNone;
}


static void vrec_dev_stop(lb_omx_component_t *component)
{
	OMX_S32 i = 0;
	vrec_private_t *source_private = NULL;

	OSCL_LOGI("viss exit...");
	source_private = component->component_private;
	if (NULL == source_private->vrec_dev)
		return;

	for (i = 0; i < source_private->info.channel_num; i++)
		oscl_vrec_capture_off(source_private->vrec_dev, i);

	oscl_vrec_clean_buffer(source_private->vrec_dev);
	if (source_private->frame_queue.frame_addr) {
		oscl_free(source_private->frame_queue.frame_addr);
		source_private->frame_queue.frame_addr = NULL;
	}
	OSCL_LOGI("viss exit success...");
}

static OMX_ERRORTYPE vrec_release_buffer(lb_omx_component_t *component,
				OMX_BUFFERHEADERTYPE *buffer)
{
	vrec_private_t *source_private = NULL;
	vrec_reqframe_t req_frm;
	OMX_ERRORTYPE ret = OMX_ErrorNone;

	source_private = component->component_private;
	memset(&req_frm, 0, sizeof(req_frm));

	req_frm._private = buffer->pOutputPortPrivate;
	req_frm.channel_id = buffer->nOutputPortIndex;
#ifdef __linux__
	long index = (long)buffer->pOutputPortPrivate;

	req_frm.paddr.frame_index = index;
	req_frm.paddr.addr[0] = source_private->frame_queue.frame_addr[index].addr[0];
	req_frm.paddr.size[0] = source_private->frame_queue.frame_addr[index].size[0];
	req_frm.paddr.addr[1] = source_private->frame_queue.frame_addr[index].addr[1];
	req_frm.paddr.size[1] = source_private->frame_queue.frame_addr[index].size[1];
	req_frm.paddr.addr[2] = source_private->frame_queue.frame_addr[index].addr[2];
	req_frm.paddr.size[2] = source_private->frame_queue.frame_addr[index].size[2];
#endif
	OSCL_LOGD("release frmid:0x%x, private:%p", buffer->nOffset, req_frm._private);
	ret = oscl_vrec_release_frame(source_private->vrec_dev, &req_frm);
	if (OMX_ErrorNone != ret)
		OSCL_LOGW("release frame fail.\n");

	return ret;
}

OMX_ERRORTYPE vrec_fill_this_buffer(OMX_IN OMX_HANDLETYPE hComp,
	OMX_IN OMX_BUFFERHEADERTYPE *header)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component = NULL;
	vrec_private_t *source_private = NULL;

	component = get_lb_component(hComp);
	oscl_param_check(component != NULL, OMX_ErrorBadParameter, NULL);
	source_private = component->component_private;

	OSCL_LOGD("vrec_fill_this_buffer %p pBuffer:%p, state:%d\n",
				header, header->pBuffer, component->state);
	pthread_mutex_lock(&source_private->buf_mutex);
	ret = base_fill_this_buffer(hComp, header);
	if (OMX_ErrorNone != ret) {
		pthread_mutex_unlock(&source_private->buf_mutex);
		return OMX_ErrorResourcesLost;
	}
	if ((component->state == OMX_StateExecuting) ||
		(component->state == OMX_StatePause))
		ret = vrec_release_buffer(component, header);

	OSCL_TRACE("%p %p %d\n", hComp, header, ret);
	pthread_mutex_unlock(&source_private->buf_mutex);

	return ret;
}

void vrec_buffer_handle(OMX_HANDLETYPE stand_com,
	OMX_BUFFERHEADERTYPE *inbuffer,
	OMX_BUFFERHEADERTYPE *outbuffer)
{
	vrec_private_t *source_private = NULL;
	lb_omx_component_t *component = NULL;
	vrec_reqframe_t req_frm;
	OMX_S32 ret = 0;
	base_port_t *outport = NULL;
	OMX_U8 *frame_virtual_addr = NULL;

	component = get_lb_component(stand_com);
	outport = &component->port[SOURCE_OUTPUT_PORT_INDEX];
	source_private = component->component_private;
	if (source_private->vrec_dev == NULL
		|| outbuffer == NULL
		|| outbuffer->nAllocLen == 0
		|| outbuffer->pBuffer == NULL) {
		OSCL_LOGE("param err! vrec_dev:%p, outbuf:%p, nAllocLen:%d, pBuffer:%p!",
			source_private->vrec_dev,
			outbuffer,
			(outbuffer ? outbuffer->nAllocLen : 0),
			(outbuffer ? outbuffer->pBuffer : NULL));
		return;
	}
	outbuffer->nOffset = 0;
	outbuffer->nFlags = 0;
	outbuffer->pOutputPortPrivate = NULL;
	outbuffer->nTimeStamp = -1;
	outbuffer->nFilledLen = 0;
	while (1) {
		if ((outport->is_flushed) || (component->state == OMX_StateIdle)) {
			OSCL_LOGW("vrec quick return, is_flushed:%d, state:%d",
				outport->is_flushed, component->state);
			return;
		}
		/* request frame */
		memset(&req_frm, 0, sizeof(req_frm));
		oscl_vrec_wait_frame(source_private->vrec_dev, 10);/* timeout 100ms */
		req_frm.channel_id = outbuffer->nOutputPortIndex; /* channel id */
		ret = oscl_vrec_request_frame(source_private->vrec_dev, &req_frm);
		if (OMX_ErrorNone != ret) {
			/* LOG_I("request frame fail.\n"); */
			continue;
		}
		break;
	}
#ifdef __linux__
	/* omx output port buffer index is equal to v4l2 dequeue buffer index */
	frame_virtual_addr = (OMX_U8 *)
	source_private->frame_queue.frame_addr[req_frm.paddr.frame_index].addr[0];
	outbuffer->pOutputPortPrivate = (OMX_PTR)req_frm.paddr.frame_index;
#elif defined __EOS__
	frame_virtual_addr = (OMX_U8 *)req_frm.paddr.addr[0];
	outbuffer->pOutputPortPrivate = req_frm._private;
#endif
	OSCL_LOGD("frame_virtual_addr:%p, frame_index:%ld",
		frame_virtual_addr, req_frm.paddr.frame_index);
	if (frame_virtual_addr != outbuffer->pBuffer) {
		OSCL_LOGE("Fatal error, y:0x%p, pBuffer:0x%p",
			frame_virtual_addr, outbuffer->pBuffer);
	}
	source_private->cur_time = req_frm.pts;
	outbuffer->nFilledLen = outport->port_param.nBufferSize;
	if (req_frm.pts > source_private->ref_time)
		outbuffer->nTimeStamp = req_frm.pts - source_private->ref_time;
	else
		outbuffer->nTimeStamp = 0;

	memcpy(outbuffer->pBuffer + outbuffer->nAllocLen
		- sizeof(omx_extra_data_t),
		&req_frm.isp_exif,
		sizeof(omx_extra_data_t));
	/* outbuffer->nOffset = req_frm.frame_id;  */ /* debug frame id */
	OSCL_LOGD("request frmid:0x%x, private:%p", req_frm.frame_id, req_frm._private);
	OSCL_LOGD("Rec>> outbuffer:%p %p", outbuffer, outbuffer->pBuffer);
}

void *vrec_buffer_manager(void *param)
{
	lb_omx_component_t *component = NULL;
	OMX_COMPONENTTYPE *stand_comp = (OMX_COMPONENTTYPE *)param;
	base_port_t *outport = NULL;
	OMX_BUFFERHEADERTYPE *outbuffer = NULL;
	OMX_ERRORTYPE ret;

	OSCL_TRACE(" %p\n", param);
	oscl_param_check((param != NULL), NULL, NULL);
	component = get_lb_component(stand_comp);
	outport = &component->port[SOURCE_OUTPUT_PORT_INDEX];

	while (component->state == OMX_StateIdle
		|| component->state == OMX_StateExecuting
		|| component->state == OMX_StatePause
		|| component->target_state == OMX_StateIdle) {

		/*Wait till the ports are being flushed*/
		pthread_mutex_lock(&component->flush_mutex);
		while (outport->is_flushed) {
			pthread_mutex_unlock(&component->flush_mutex);
			if (outbuffer != NULL) {
				OSCL_LOGI("flushing port, outbuffer:%p", outbuffer);
				outport->return_buffer(outport, outbuffer);
				outbuffer = NULL;
			}
			OSCL_LOGI("%s sem_wait component->flush_sem",
				get_component_name(component));
			sem_post(component->mgmt_flush_sem);
			sem_wait(component->flush_sem);
			pthread_mutex_lock(&component->flush_mutex);
		}
		pthread_mutex_unlock(&component->flush_mutex);

		OSCL_LOGD("component->state:%d", component->state);
		if (component->state != OMX_StateExecuting) {
			sem_wait(component->buf_mgnt_sem);
			continue;
		}

		OSCL_LOGD("comp:%s, queue len:%d outbuf:%p",
			get_component_name(component),
			oscl_queue_get_num(&outport->buffer_queue), outbuffer);
		if (outbuffer == NULL) {
			outbuffer = oscl_queue_dequeue(&outport->buffer_queue);
			OSCL_LOGD("read buf manager:%p\n", outbuffer);
			if (outbuffer) {
				outbuffer->nFilledLen = 0;
				outbuffer->nFlags = 0;
			}
		}
		if (outbuffer == NULL) {
			OSCL_LOGD("sem_wait component->buf_mgnt_sem");
			sem_wait(component->buf_mgnt_sem);
			continue;
		}

		outbuffer->nFlags = 0;
		if (component->mark.hMarkTargetComponent) {
			outbuffer->hMarkTargetComponent
				= component->mark.hMarkTargetComponent;
			outbuffer->pMarkData
				= component->mark.pMarkData;
			component->mark.hMarkTargetComponent = NULL;
			component->mark.pMarkData = NULL;
		}

		OSCL_LOGD("component->state:%x,outbuffer->nAllocLen:%d",
			component->state,
			outbuffer->nAllocLen);
		if (component->state == OMX_StateExecuting) {
			if (component->buf_handle && outbuffer->nAllocLen != 0 &&
				!outbuffer->nFilledLen)
				(*(component->buf_handle))(stand_comp, NULL, outbuffer);
		}
		base_check_mark(component, outbuffer);
		base_check_eos(component, outport, outbuffer);
		if (outbuffer->nFilledLen > 0 ||
			component->target_state == OMX_StateLoaded) {
			OSCL_LOGD("rec outport return_buffer:%p", outbuffer);
			ret = outport->return_buffer(outport, outbuffer);
			if (OMX_ErrorNone != ret)
				vrec_release_buffer(component, outbuffer);
			outbuffer = NULL;
		}
	}
	if ((outbuffer != NULL) && (!PORT_IS_SUPPLIER(outport))) {
		OSCL_LOGI("outport return_buffer before exit");
		outport->return_buffer(outport, outbuffer);
	}
#ifdef __linux__
	OSCL_LOGI("exit from reader_buffer_manager:%d\n", (int)pthread_self());
#elif defined __EOS__
	OSCL_LOGI("exit from reader_buffer_manager:%s\n", pthread_self()->name);
#endif
	OSCL_TRACE(" %p\n", param);

	return NULL;
}

OMX_ERRORTYPE vrec_get_parameter(OMX_IN OMX_HANDLETYPE hComp,
	OMX_IN OMX_INDEXTYPE paramIndex,
	OMX_IN OMX_PTR paramData)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component = NULL;
	vrec_private_t *source_private = NULL;

	oscl_param_check((hComp != NULL) && (paramData != NULL),
		OMX_ErrorBadParameter, NULL);
	component = get_lb_component(hComp);
	oscl_param_check((component != NULL), OMX_ErrorBadParameter, NULL);
	source_private = component->component_private;

	switch (paramIndex) {
	case OMX_IndexParamNumAvailableStreams: {
		OMX_PARAM_U32TYPE *para = (OMX_PARAM_U32TYPE *)paramData;
		para->nU32 = source_private->info.channel_num;
		break;
	}
	default:
		ret = base_get_parameter(hComp, paramIndex, paramData);
		break;
	}

	return ret;
}

OMX_ERRORTYPE vrec_set_parameter(OMX_IN OMX_HANDLETYPE hComp,
	OMX_IN OMX_INDEXTYPE paramIndex,
	OMX_IN OMX_PTR paramData)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component = NULL;
	vrec_private_t *source_private = NULL;
	OMX_PARAM_PORTDEFINITIONTYPE *port_def = NULL;
	OMX_VIDEO_PORTDEFINITIONTYPE *video = NULL;
	OMX_U32 plane_size[3] = {0};
#if defined __EOS__
	OMX_S32 i = 0;
	OMX_U32 h_div = 0;
	OMX_U32 v_div = 0;
	vrec_info_t *info;
#endif
	vrec_format_t use_format;
	vrec_format_t cur_format;

	OSCL_TRACE(" %p, %p , %d\n", hComp, paramData, paramIndex);
	oscl_param_check((hComp != NULL) && (paramData != NULL),
		OMX_ErrorBadParameter, NULL);
	component = get_lb_component(hComp);
	oscl_param_check((component != NULL), OMX_ErrorBadParameter, NULL);
	source_private = component->component_private;

	switch (paramIndex) {
	case OMX_IndexParamVideoInit:
		memset(source_private->dev_name, 0, sizeof(source_private->dev_name));
		strncpy(source_private->dev_name, paramData,
				sizeof(source_private->dev_name) - 1);
		OSCL_LOGI("open vrec :%s ", source_private->dev_name);
		source_private->vrec_dev = oscl_open_vrec(source_private->dev_name);
		if (NULL == source_private->vrec_dev) {
			OSCL_LOGE("Open %s device fail.", source_private->dev_name);
			return OMX_ErrorInsufficientResources;
		}
		ret = oscl_vrec_get_info(source_private->vrec_dev, &source_private->info);
		oscl_param_check((OMX_ErrorNone == ret),
					OMX_ErrorInsufficientResources, NULL);
		break;
	case OMX_IndexParamOtherInit:
		memset(source_private->isp_name, 0, sizeof(source_private->isp_name));
		strncpy(source_private->isp_name, paramData,
				sizeof(source_private->isp_name) - 1);
		ret = oscl_open_isp(source_private->vrec_dev, source_private->isp_name);
		if (OMX_ErrorNone != ret) {
			OSCL_LOGE("Open %s device fail.", source_private->isp_name);
			return OMX_ErrorInsufficientResources;
		}
		break;
	case OMX_IndexParamPortDefinition:
		port_def = (OMX_PARAM_PORTDEFINITIONTYPE *)paramData;
		memset(&use_format, 0, sizeof(vrec_format_t));
		memset(&cur_format, 0, sizeof(vrec_format_t));
#ifdef __linux__
		use_format.frame_size.nWidth = port_def->format.video.nFrameWidth;
		use_format.frame_size.nHeight = port_def->format.video.nFrameHeight;
		use_format.format = port_def->format.video.eColorFormat;
		use_format.frame_rate = port_def->format.video.xFramerate;
#elif defined __EOS__
		info = &source_private->info;
		video = &port_def->format.video;
		for (i = 0; i < source_private->info.fmt_num; i++) {
			if (video->xFramerate <= 0)
				video->xFramerate = info->format[i].frame_rate;
			OSCL_LOGI("i:%d image_mode:%d frame[%dX%d] src[%dX%d]",
				i, info->image_mode,
				video->nFrameWidth,
				video->nFrameHeight,
				info->format[i].frame_size.nWidth,
				info->format[i].frame_size.nHeight);
			if (IMAGE_MODE_1_NORMAL == info->image_mode) {
				if ((video->eColorFormat == info->format[i].format) &&
					(video->nFrameHeight ==
						info->format[i].frame_size.nHeight) &&
					(video->nFrameWidth ==
						info->format[i].frame_size.nWidth) &&
					(video->xFramerate ==
						info->format[i].frame_rate)) {
					memcpy(&use_format,
						&info->format[i], sizeof(vrec_format_t));
					break;
				}
			} else if ((IMAGE_MODE_2_STITCH_H == info->image_mode) ||
				(IMAGE_MODE_4_STITCH_CROSS == info->image_mode)) {
				h_div = 0;
				v_div = 0;
				if ((video->nFrameWidth %
					info->format[i].frame_size.nWidth) == 0) {
					h_div = video->nFrameWidth /
						info->format[i].frame_size.nWidth;
				}
				if ((video->nFrameHeight %
					info->format[i].frame_size.nHeight) == 0) {
					v_div = video->nFrameHeight /
						info->format[i].frame_size.nHeight;
				}
				if ((h_div * v_div) != info->channel_num) {
					OSCL_LOGE("Check sensor informat error.");
					OSCL_LOGE("Media[%dx%d] sensor[i:%d %dx%d]",
						video->nFrameWidth, video->nFrameHeight,
						i, info->format[i].frame_size.nWidth,
						info->format[i].frame_size.nHeight);
					continue;
				}
				if ((video->eColorFormat == info->format[i].format) &&
					(video->xFramerate ==
						info->format[i].frame_rate)) {
					memcpy(&use_format,
						&info->format[i], sizeof(vrec_format_t));
					break;
				}
			}
		}
		if (i >= source_private->info.fmt_num) {
			OSCL_LOGE("Sensor unsupport this format. %d %d %d",
				port_def->format.video.eColorFormat,
				port_def->format.video.nFrameWidth,
				port_def->format.video.nFrameHeight);
			return OMX_ErrorInsufficientResources;
		}
#endif
		ret = oscl_vrec_set_format(source_private->vrec_dev, &use_format);
		if (OMX_ErrorNone != ret)
			return ret;

		ret = oscl_vrec_get_cur_format(source_private->vrec_dev, &cur_format);
		if (OMX_ErrorNone != ret)
			return ret;

		if ((use_format.format != cur_format.format) ||
		    (use_format.frame_size.nWidth != cur_format.frame_size.nWidth) ||
		    (use_format.frame_size.nHeight != cur_format.frame_size.nHeight)) {
			OSCL_LOGE("Format set error.\n");
			return OMX_ErrorInsufficientResources;
		}
		port_def->format.video.xFramerate = cur_format.frame_rate;
		ret = calc_frame_size(port_def->format.video.eColorFormat,
				port_def->format.video.nFrameWidth,
				port_def->format.video.nFrameHeight,
				plane_size, port_def->nBufferAlignment);
		if (OMX_ErrorNone != ret)
			return ret;
		port_def->nBufferSize = plane_size[0] + plane_size[1] + plane_size[2];
		port_def->nBufferSize += sizeof(omx_extra_data_t);
#ifdef CONFIG_ARCH_LOMBO_N7V0
		port_def->nBufferSize += 64 * 1024;
#endif
		ret = base_set_parameter(hComp, paramIndex, paramData);
		break;
	default:
		ret = base_set_parameter(hComp, paramIndex, paramData);
		break;
	}
	OSCL_TRACE(" %d\n", ret);

	return ret;
}

OMX_ERRORTYPE vrec_get_config(OMX_IN OMX_HANDLETYPE hcomp,
		OMX_IN OMX_INDEXTYPE cfg_index,
		OMX_IN OMX_PTR cfg_data)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component = NULL;
	vrec_private_t *source_private = NULL;
	OMX_TIME_CONFIG_TIMESTAMPTYPE *time;

	OSCL_TRACE(" %p, %x , %p\n", hcomp, cfg_index, cfg_data);
	oscl_param_check((hcomp != NULL) && (cfg_data != NULL),
		OMX_ErrorBadParameter, NULL);
	component = get_lb_component(hcomp);
	oscl_param_check((component != NULL), OMX_ErrorBadParameter, NULL);
	source_private = component->component_private;

	switch (cfg_index) {
	case omx_index_lombo_config_cur_time:
		time = cfg_data;
		time->nTimestamp = -1;
		if (source_private->cur_time >= 0)
			time->nTimestamp = source_private->cur_time
						- source_private->ref_time;
		break;
	default:
		ret = base_get_config(hcomp, cfg_index, cfg_data);
		break;
	}
	OSCL_TRACE(" %d\n", ret);

	return ret;
}

OMX_ERRORTYPE vrec_set_state(OMX_HANDLETYPE hComp,
	OMX_U32 dest_state)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component = NULL;
	vrec_private_t *source_private = NULL;
	OMX_STATETYPE pre_state;

	OSCL_TRACE(" %p, %x\n", hComp, dest_state);
	oscl_param_check(hComp != NULL, OMX_ErrorBadParameter, NULL);
	component = get_lb_component(hComp);
	source_private = component->component_private;

	if (dest_state == OMX_StateIdle && component->state == OMX_StateLoaded) {
		ret = vrec_dev_init(component);
		oscl_param_check((OMX_ErrorNone == ret),
					OMX_ErrorInsufficientResources, NULL);
	}
	if (dest_state == OMX_StateExecuting && component->state == OMX_StateIdle) {
		ret = vrec_dev_start(component); /* all channel start capture */
		if (OMX_ErrorNone != ret)
			goto EXIT;
		OSCL_LOGI("vrec dev start success.");
	}
	pre_state = component->state;
	ret = base_component_set_state(hComp, dest_state);
	if (dest_state == OMX_StateIdle && pre_state == OMX_StateExecuting) {
		source_private->cur_time = -1;
		OSCL_LOGI("vrec dev exit\n");
		vrec_dev_stop(component);
		if (source_private->vrec_dev) {
			oscl_close_isp(source_private->vrec_dev);
			oscl_close_vrec(source_private->vrec_dev);
			source_private->vrec_dev = NULL;
		}
	}
	return ret;

EXIT:
	source_private->cur_time = -1;
	return OMX_ErrorResourcesLost;
}

OMX_ERRORTYPE vrec_component_deinit(OMX_IN OMX_HANDLETYPE hComponent)
{
	OMX_COMPONENTTYPE *base_cmp = (OMX_COMPONENTTYPE *)hComponent;
	lb_omx_component_t *component = NULL;
	vrec_private_t *source_private = NULL;
	OMX_ERRORTYPE ret = OMX_ErrorNone;

	OSCL_TRACE("base_cmp_handle:%p\n", hComponent);
	oscl_param_check(hComponent != NULL, OMX_ErrorBadParameter, NULL);
	component = (lb_omx_component_t *)base_cmp->pComponentPrivate;
	source_private = component->component_private;

	if (source_private->vrec_dev) {
		oscl_close_isp(source_private->vrec_dev);
		oscl_close_vrec(source_private->vrec_dev);
		source_private->vrec_dev = NULL;
	}
	pthread_mutex_destroy(&source_private->buf_mutex);
	component->port[SOURCE_OUTPUT_PORT_INDEX].deinit(
		&component->port[SOURCE_OUTPUT_PORT_INDEX]);
	oscl_free(source_private);
	ret = base_component_deinit(hComponent);

	return ret;
}

OMX_ERRORTYPE vrec_component_init(lb_omx_component_t *cmp_handle)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	vrec_private_t *source_private = NULL;
	OMX_PARAM_PORTDEFINITIONTYPE *port_param;
	OMX_U32 plane_size[3] = {0};
	source_private = oscl_zalloc(sizeof(vrec_private_t));
	oscl_param_check_exit((source_private != NULL),
		OMX_ErrorInsufficientResources, NULL);
	pthread_mutex_init(&source_private->buf_mutex, NULL);
	source_private->ref_time = 0;
	source_private->cur_time = -1;

	ret = base_component_init(cmp_handle);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	cmp_handle->name = "OMX.LB.SOURCE.VREC";
	cmp_handle->component_private = source_private;
	cmp_handle->buf_manager = vrec_buffer_manager;
	cmp_handle->buf_handle = vrec_buffer_handle;
	cmp_handle->base_comp.ComponentDeInit = vrec_component_deinit;
	cmp_handle->base_comp.SetParameter = vrec_set_parameter;
	cmp_handle->base_comp.GetParameter = vrec_get_parameter;
	cmp_handle->base_comp.GetConfig = vrec_get_config;
	cmp_handle->base_comp.FillThisBuffer = vrec_fill_this_buffer;
	cmp_handle->do_state_set = vrec_set_state;
	cmp_handle->num_ports = 1;
	ret = base_port_init(cmp_handle, &cmp_handle->port[SOURCE_OUTPUT_PORT_INDEX],
			SOURCE_OUTPUT_PORT_INDEX,
			OMX_DirOutput);
	if (ret != OMX_ErrorNone) {
		base_component_deinit(cmp_handle);
		cmp_handle->port[SOURCE_OUTPUT_PORT_INDEX].deinit(
			&cmp_handle->port[SOURCE_OUTPUT_PORT_INDEX]);
		return ret;
	}

	port_param = &cmp_handle->port[SOURCE_OUTPUT_PORT_INDEX].port_param;
	port_param->eDomain = OMX_PortDomainVideo;
	port_param->nBufferAlignment = DEFAULT_BUFFER_ALIGN_SIZE;
	port_param->nBufferCountActual = DEFAULT_BUFFER_NUM;
	port_param->bBuffersContiguous = 1;

	port_param->format.video.eColorFormat = OMX_COLOR_FormatYUV420SemiPlanar;
	port_param->format.video.nFrameWidth = DEFAULT_VIDEO_WIDTH;
	port_param->format.video.nFrameHeight = DEFAULT_VIDEO_HEIGHT;
	ret = calc_frame_size(port_param->format.video.eColorFormat,
			port_param->format.video.nFrameWidth,
			port_param->format.video.nFrameHeight,
			plane_size, port_param->nBufferAlignment);
	if (OMX_ErrorNone != ret)
		goto EXIT;
	port_param->nBufferSize = plane_size[0] + plane_size[1] + plane_size[2];
#ifdef CONFIG_ARCH_LOMBO_N7V0
	port_def->nBufferSize += 64 * 1024;
#endif

EXIT:
	if (ret != OMX_ErrorNone) {
		if (source_private != NULL)
			oscl_free(source_private);
	}
	OSCL_TRACE("%d vrec:%p\n", ret, cmp_handle);

	return ret;
}

