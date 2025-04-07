/*
 * untunnel_common.c - Standard functionality for utunnel mode.
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
#define DBG_LEVEL         DBG_ERR

#include <oscl.h>
#include <base_component.h>
#include <framework_common.h>
#include <lb_omx_as_string.h>

typedef struct app_cb_private {
	al_port_info_t *port;
	union {
		al_video_frame_info_t video;
		al_audio_frame_info_t audio;
	} info;
	al_frame_t *cb_frame;
} app_cb_private_t;

#define get_al_comp_state(al_port) \
	(((al_comp_info_t *)(al_port->comp_info))->state)

/* Callbacks implementation */
OMX_ERRORTYPE al_untunnel_event_handler(
	OMX_HANDLETYPE comp_hdl,
	OMX_PTR app_data,
	OMX_EVENTTYPE event,
	OMX_U32 data1,
	OMX_U32 data2,
	OMX_PTR event_data)
{
	al_comp_info_t *al_comp = app_data;
	OSCL_TRACE("comp:%x, app_data:%x, event:%x, data1:%x, data2:%x, event data:%x",
		comp_hdl, app_data, event, data1, data2, event_data);

	oscl_param_check((comp_hdl != NULL && app_data != NULL),
		OMX_ErrorBadParameter, NULL);
	al_comp = (al_comp_info_t *)app_data;

	OSCL_LOGI("%s event:%s", al_comp->name, omx_evt_as_string(event));
	if (event == OMX_EventCmdComplete) {
		OSCL_LOGI("command complete:%s", omx_cmd_as_string(data1));
		if (data1 == OMX_CommandStateSet)
			OSCL_LOGI("set state complete:%s", omx_state_as_string(data2));
		sem_post(al_comp->sem_cmd);
	}
	if (event == OMX_EventError) {
		OSCL_LOGI("command err:%s, %x", omx_cmd_as_string(data2), data1);
		if (data2 == OMX_CommandStateSet ||
			data2 == OMX_CommandFlush ||
			data2 == OMX_CommandPortDisable ||
			data2 == OMX_CommandPortEnable ||
			data2 == OMX_CommandMarkBuffer)
			sem_post(al_comp->sem_cmd);
	}
	if (event == OMX_EventBufferFlag)
		OSCL_LOGI("%s OMX_EventBufferFlag %x", al_comp->name, data2);

	if (event == OMX_Eventneednewoutputfilename && al_comp->priv_data) {
		al_muxer_private_t *p_cb = (al_muxer_private_t *)al_comp->priv_data;
		OSCL_LOGI("OMX_Eventneednewoutputfilename");
		if (p_cb && p_cb->rec_handle && p_cb->fix_duration_param.cb_get_next_file)
			p_cb->fix_duration_param.cb_get_next_file(p_cb->rec_handle,
					event_data);
	}
	if (event == OMX_Eventfileclosed && al_comp->priv_data) {
		OSCL_LOGI("OMX_Eventfileclosed");
		al_muxer_private_t *p_cb = (al_muxer_private_t *)al_comp->priv_data;
		if (p_cb && p_cb->rec_handle && p_cb->fix_duration_param.cb_file_closed)
			p_cb->fix_duration_param.cb_file_closed(p_cb->rec_handle,
				event_data);
	}
	if (event == OMX_Eventgetuserdata && al_comp->priv_data) {
		al_muxer_private_t *p_cb = (al_muxer_private_t *)al_comp->priv_data;
		OSCL_LOGI("OMX_Eventgetuserdata: %p",
			p_cb->fix_duration_param.cb_get_user_data);
		if (p_cb && p_cb->rec_handle && p_cb->fix_duration_param.cb_get_user_data)
			p_cb->fix_duration_param.cb_get_user_data(p_cb->rec_handle,
				event_data, (unsigned int *)data2);
	}

	return OMX_ErrorNone;
}

static int __app_empty_this_buffer(al_port_info_t *port,
					OMX_BUFFERHEADERTYPE *buffer)
{
	int ret;
	al_frame_t *frame = NULL;
	int i;
	app_cb_private_t *port_private;

	port_private = port->priv_data;
	for (i = 0; i < port->nbuffer; i++) {
		if (port_private->cb_frame[i].header == NULL) {
			frame = &port_private->cb_frame[i];
			break;
		}
	}
	if (frame == NULL) {
		OSCL_LOGE("can not find frame tobe freed. %x", buffer);
		return -1;
	}

	memset(frame, 0, sizeof(al_frame_t));
	frame->al_data = port->priv_data;
	frame->header = buffer;
	if (port->domain == OMX_PortDomainVideo) {
		frame->type = AL_VIDEO_RAW_FRAME;
		memcpy(&frame->info.video, &port_private->info.video,
			sizeof(al_video_frame_info_t));
		frame->info.video.addr[0] = buffer->pBuffer;
		if (frame->info.video.size[1])
			frame->info.video.addr[1] = frame->info.video.addr[0]
						    + frame->info.video.size[0];
		if (frame->info.video.size[2])
			frame->info.video.addr[2] = frame->info.video.addr[1]
						    + frame->info.video.size[1];
		frame->info.video.time_stamp = buffer->nTimeStamp;
	}
	ret = port->cb.buf_handle(port->cb.app_data, frame);
	return ret;
}

int app_empty_buffer_done(al_frame_t *frame)
{
	int ret;
	app_cb_private_t *port_private;
	al_comp_info_t *al_comp;

	oscl_param_check((frame != NULL && frame->al_data != NULL), -1, NULL);

	port_private = frame->al_data;
	oscl_param_check((port_private->port != NULL && frame->header != NULL),
			-1, NULL);

	al_comp = port_private->port->comp_info;
	/* empty buffer done, ask output port fill this buffer */
	ret = OMX_FillThisBuffer(al_comp->cmp_hdl, frame->header);
	frame->header = NULL;
	return ret;
}

static OMX_BUFFERHEADERTYPE *get_mapped_header(al_port_info_t *port,
	void *conect_port, OMX_BUFFERHEADERTYPE *buffer)
{
	al_port_info_t *al_tunnel_port;
	OMX_BUFFERHEADERTYPE *ret = NULL;
	int i;

	oscl_param_check((port != NULL && conect_port != NULL && buffer != NULL),
		NULL, NULL);
	al_tunnel_port = (al_port_info_t *)conect_port;
	if (al_tunnel_port->header == NULL)
		return buffer;

	for (i = 0; i < port->nbuffer; i++) {
		if (buffer == port->header[i]
			&& (buffer->pBuffer == al_tunnel_port->header[i]->pBuffer)) {
			ret = al_tunnel_port->header[i];
			ret->nFilledLen = buffer->nFilledLen;
			ret->nOffset = buffer->nOffset;
			ret->nTickCount = buffer->nTickCount;
			ret->nTimeStamp = buffer->nTimeStamp;
			ret->nFlags = buffer->nFlags;
			break;
		}
	}
	return ret;
}

int al_untunnel_hold_buffer(al_port_info_t *port, void *pbuffer)
{
	int index, i;
	oscl_param_check(port != NULL, OMX_ErrorBadParameter, NULL);
	oscl_param_check(port->edir == OMX_DirOutput, OMX_ErrorBadParameter, NULL);

	for (index = 0; index < port->nbuffer; index++) {
		if (pbuffer == port->header[index]->pBuffer)
			break;
	}
	oscl_param_check(index < port->nbuffer, OMX_ErrorBadParameter, NULL);

	if (port->hold_map == NULL) {
		port->hold_map = oscl_malloc(sizeof(*port->hold_map) * port->nbuffer);
		oscl_param_check(port->hold_map != NULL,
			OMX_ErrorInsufficientResources, NULL);
		for (i = 0; i < port->nbuffer; i++)
			port->hold_map[i] = -1;
	}

	for (i = 0; i < port->nbuffer; i++) {
		if (port->hold_map[i] == -1) {
			port->hold_map[i] = index;
			port->nbuffer_hold++;
			break;
		} else if (port->hold_map[i] == index) {
			OSCL_LOGE("port:%s already hold buf %d!",
						al_port_name(port), index);
			break;
		}
	}
	oscl_param_check(i < port->nbuffer, OMX_ErrorBadParameter, NULL);
	OSCL_LOGI("port:%s-%d: hold buf=%p idx=%d, num=%d!", al_port_name(port),
		  port->index, pbuffer, index, port->nbuffer_hold);

	return 0;
}

int _al_port_queue_buffers(al_port_info_t *port)
{
	int index;
	int ret = 0;
	OMX_COMPONENTTYPE *omx_hdl;
	int i;

	oscl_param_check(port != NULL, OMX_ErrorBadParameter, NULL);
	oscl_param_check(port->edir == OMX_DirOutput, OMX_ErrorBadParameter, NULL);
	oscl_param_check(port->hold_map != NULL, OMX_ErrorBadParameter, NULL);
	if (port->header == NULL || port->nbuffer_hold == 0) {
		OSCL_LOGI("none buffer to be queued");
		return 0;
	}
	OSCL_LOGW("nhold:%d", port->nbuffer_hold);

	if (port->nbuffer_hold != port->nbuffer)
		OSCL_LOGW("buffer:%d-%d",
			port->nbuffer_hold, port->nbuffer);
	omx_hdl = ((al_comp_info_t *)(port->comp_info))->cmp_hdl;

	for (i = 0; i < port->nbuffer; i++) {
		if (port->hold_map[i] >= 0) {
			index = port->hold_map[i];
			ret = OMX_FillThisBuffer(omx_hdl, port->header[index]);
			if (ret != 0) {
				OSCL_LOGE("queue buffer failed!");
				al_untunnel_hold_buffer(port,
					port->header[index]->pBuffer);
			} else {
				port->hold_map[i] = -1;
				port->nbuffer_hold--;
			}
		}
	}

	if (port->nbuffer_hold)
		OSCL_LOGE("err! hold %d buffers after queue buffer", port->nbuffer_hold);
	if (port->nbuffer_hold < 0)
		port->nbuffer_hold = 0;
	OSCL_LOGW("nhold:%d", port->nbuffer_hold);
	return 0;
}

void al_untunnel_queue_buffers(al_comp_info_t *al)
{
	int i;
	for (i = 0; i < al->num_port; i++) {
		if (al->port_info[i].nbuffer_hold) {
			OSCL_LOGW("%s %d queue buffers", al->name, i);
			_al_port_queue_buffers(&al->port_info[i]);
		}
	}
}

OMX_ERRORTYPE al_untunnel_empty_buffer_done(
	OMX_HANDLETYPE comp_hdl,
	OMX_PTR app_data,
	OMX_BUFFERHEADERTYPE *buffer)
{
	al_port_info_t *al_port, *tunnel_port;
	al_comp_info_t *al_comp, *tunnel_comp;
	OMX_BUFFERHEADERTYPE *outport_buffer;
	int ret = 0;

	OSCL_TRACE("comp_hdl:%x, app_data:%x, buffer:%x", comp_hdl, app_data, buffer);

	oscl_param_check((buffer != NULL && app_data != NULL),
		OMX_ErrorBadParameter, NULL);
	al_comp = (al_comp_info_t *)app_data;
	OSCL_TRACE("comp:%s, buffer:%x", al_comp->name, buffer);

	oscl_param_check((buffer->nInputPortIndex < al_comp->num_port),
		OMX_ErrorBadParameter, NULL);
	al_port = &al_comp->port_info[buffer->nInputPortIndex];
	oscl_param_check((al_port->state == AL_PORT_STATE_UNTUN_SETUP),
			 OMX_ErrorBadParameter, NULL);
	oscl_param_check((al_port->tunnel_hdl != NULL), OMX_ErrorInvalidState, NULL);

	/* tunneled component will not call this callback */
	if (al_port->state != AL_PORT_STATE_UNTUN_SETUP) {
		OSCL_LOGE("err: port is tunneled!");
		return OMX_ErrorBadParameter;
	}

	tunnel_port = al_port->tunnel_port;
	tunnel_comp = tunnel_port->comp_info;
	OSCL_LOGD("%s-->%s:pbuffer=%p",
		al_comp->name,
		tunnel_comp->name,
		buffer->pBuffer);
	pthread_mutex_lock(&tunnel_comp->state_lock);
	/* client(out port) hold this buffer while not executing in untunnel mode */
	if (get_al_comp_state(tunnel_port) != OMX_StateExecuting &&
		get_al_comp_state(tunnel_port) != OMX_StatePause &&
		(tunnel_port->is_shared_buffer == 0)) {
		OSCL_LOGW("%s-->%s state %d, hold buffer",
			al_comp->name,
			tunnel_comp->name,
			get_al_comp_state(tunnel_port));
		al_untunnel_hold_buffer(tunnel_port, buffer->pBuffer);
		ret = OMX_ErrorNone;
		pthread_mutex_unlock(&tunnel_comp->state_lock);
		goto EXIT;
	} else {
		if (tunnel_port->nbuffer_hold > 0) {
			OSCL_LOGW("%s(%d): hold %d buffer while in executing state!!",
				tunnel_comp->name,
				tunnel_port->index,
				tunnel_port->nbuffer_hold);
			al_untunnel_queue_buffers(tunnel_comp);
		}
	}
	pthread_mutex_unlock(&tunnel_comp->state_lock);
	/* get output port buffer header*/
	outport_buffer = get_mapped_header(al_port, al_port->tunnel_port, buffer);
	oscl_param_check(outport_buffer != NULL, OMX_ErrorBadParameter, NULL);

	/* empty buffer done, ask output port fill this buffer */
	ret = OMX_FillThisBuffer(al_port->tunnel_hdl, outport_buffer);
	if (ret != 0) {
		OSCL_LOGW("fill buffer failed, hold it!");
		if (tunnel_port->is_shared_buffer == 0) {
			OSCL_LOGE("====%s-->%s, hold buffer",
				al_comp->name,
				tunnel_comp->name);
			al_untunnel_hold_buffer(tunnel_port, buffer->pBuffer);
			ret = OMX_ErrorNone;
		}
	}
EXIT:
	OSCL_TRACE("%x", ret);
	return ret;
}

OMX_ERRORTYPE al_untunnel_fill_buffer_done(
	OMX_HANDLETYPE comp_hdl,
	OMX_PTR app_data,
	OMX_BUFFERHEADERTYPE *buffer)
{
	al_port_info_t *al_port, *tunnel_port;
	al_comp_info_t *al_comp;
	OMX_BUFFERHEADERTYPE *inport_buffer;
	int ret = 0;

	OSCL_TRACE("comp_hdl:%x, app_data:%x, buffer:%x", comp_hdl, app_data, buffer);

	/* get app layer component info and port info */
	oscl_param_check((buffer != NULL && app_data != NULL),
		OMX_ErrorBadParameter, NULL);
	al_comp = (al_comp_info_t *)app_data;
	oscl_param_check((buffer->nOutputPortIndex < al_comp->num_port),
		OMX_ErrorBadParameter, NULL);
	OSCL_TRACE("comp:%s, buffer:%x", al_comp->name, buffer);
	al_port = &al_comp->port_info[buffer->nOutputPortIndex];

	/* tunneled component will not call this callback */
	if (al_port->state != AL_PORT_STATE_UNTUN_SETUP) {
		OSCL_LOGE("err: port is tunneled!");
		return OMX_ErrorBadParameter;
	}

	/* if callback is set, send buffer to app */
	if (al_port->cb.buf_handle != NULL)
		return __app_empty_this_buffer(al_port, buffer);

	tunnel_port = al_port->tunnel_port;
	/* client hold this buffer while not executing in untunnel mode */
	pthread_mutex_lock(&al_comp->state_lock);
	if (get_al_comp_state(tunnel_port) != OMX_StateExecuting &&
		get_al_comp_state(tunnel_port) != OMX_StatePause) {
		if (al_port->is_shared_buffer == 0) {
			OSCL_LOGI("%s-->%s state %d, hold buffer",
				al_comp->name,
				((al_comp_info_t *)((tunnel_port)->comp_info))->name,
				get_al_comp_state(tunnel_port));
			al_untunnel_hold_buffer(al_port, buffer->pBuffer);
			ret = OMX_ErrorNone;
		} else {
			ret = OMX_ErrorIncorrectStateOperation;
		}
		pthread_mutex_unlock(&al_comp->state_lock);
		goto EXIT;
	}
	if (al_port->nbuffer_hold > 0 &&
		(al_comp->state == OMX_StateExecuting ||
		al_comp->state == OMX_StatePause)) {
		OSCL_LOGW("%s(%d): hold %d buffer while in executing state!!",
			al_comp->name,
			al_port->index,
			al_port->nbuffer_hold);
		al_untunnel_queue_buffers(al_comp);
	}
	pthread_mutex_unlock(&al_comp->state_lock);

	/* get input port buffer header*/
	inport_buffer = get_mapped_header(al_port, al_port->tunnel_port, buffer);
	oscl_param_check(inport_buffer != NULL, OMX_ErrorBadParameter, al_comp->name);
	if (inport_buffer->nFilledLen == 0)
		OSCL_LOGI("component:%s(%d) inbuf:%x,%d ",
			((al_comp_info_t *)(al_port->comp_info))->name,
			al_port->index, inport_buffer, inport_buffer->nFilledLen);

	/* fill buffer done, ask input port empty this buffer */
	ret = OMX_EmptyThisBuffer(al_port->tunnel_hdl, inport_buffer);
	if (ret != 0) {
		if (al_port->is_shared_buffer == 0) {
			OSCL_LOGE("empty buffer failed, hold it! %s", al_comp->name);
			al_untunnel_hold_buffer(al_port, buffer->pBuffer);
			ret = OMX_ErrorNone;
		}
	}
EXIT:
	OSCL_TRACE("%x", ret);
	return ret;
}

OMX_CALLBACKTYPE al_untunnel_common_callbacks = {
	.EventHandler = al_untunnel_event_handler,
	.EmptyBufferDone = al_untunnel_empty_buffer_done,
	.FillBufferDone = al_untunnel_fill_buffer_done,
};


int al_untunnel_setup_ports(al_port_info_t *out_port, al_port_info_t *in_port)
{
	int ret;
	int i;
	al_comp_info_t *al_out;
	al_comp_info_t *al_in;
	OMX_PARAM_PORTDEFINITIONTYPE port_def;
	OMX_BUFFERHEADERTYPE *outport_buf;

	oscl_param_check((out_port != NULL && in_port != NULL),
		OMX_ErrorBadParameter, NULL);
	oscl_param_check((out_port->domain == in_port->domain),
		OMX_ErrorBadParameter, NULL);

	OSCL_LOGI("peer ports:%s(%d)--%s(%d)",
		((al_comp_info_t *)out_port->comp_info)->name, out_port->index,
		((al_comp_info_t *)in_port->comp_info)->name, in_port->index);
	out_port->tunnel_hdl = ((al_comp_info_t *)in_port->comp_info)->cmp_hdl;
	out_port->tunnel_port = in_port;
	in_port->tunnel_hdl = ((al_comp_info_t *)out_port->comp_info)->cmp_hdl;
	in_port->tunnel_port = out_port;

	/* set output config to input component: get output config */
	port_def.nPortIndex = out_port->index;
	port_def.nVersion.nVersion = OMX_VERSION;
	ret = OMX_GetParameter(((al_comp_info_t *)out_port->comp_info)->cmp_hdl,
			OMX_IndexParamPortDefinition, &port_def);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);
	OSCL_LOGI("port_def %d, %d", port_def.nBufferCountActual, port_def.nBufferSize);
	OSCL_LOGI("out_port %d, %d", out_port->nbuffer, out_port->buf_size);
	out_port->nbuffer = port_def.nBufferCountActual;
	out_port->buf_size = port_def.nBufferSize;

	if (port_def.eDomain == OMX_PortDomainVideo)
		OSCL_LOGI("eColorFormat:%d, w-h(%d, %d), (%d, %d)",
			port_def.format.video.eColorFormat,
			port_def.format.video.nFrameWidth,
			port_def.format.video.nFrameHeight,
			port_def.format.video.nStride,
			port_def.format.video.nSliceHeight);

	/* set output config to input component: set input config */
	port_def.nPortIndex = in_port->index;
	OSCL_LOGI("port_def %d, %d", port_def.nBufferCountActual, port_def.nBufferSize);
	ret = OMX_SetParameter(((al_comp_info_t *)in_port->comp_info)->cmp_hdl,
			OMX_IndexParamPortDefinition, &port_def);
	OSCL_LOGI("port_def %d, %d", port_def.nBufferCountActual, port_def.nBufferSize);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);
	in_port->nbuffer = port_def.nBufferCountActual;
	in_port->buf_size = port_def.nBufferSize;

	/* require buffer : init header array */
	if (out_port->header != NULL) {
		OSCL_LOGE("err! header is not null while set up ports!");
		oscl_free(out_port->header);
	}
	out_port->header = oscl_zalloc(
			out_port->nbuffer * sizeof(OMX_BUFFERHEADERTYPE *));
	oscl_param_check_exit(NULL != out_port->header,
		OMX_ErrorInsufficientResources, NULL);

	in_port->nbuffer = out_port->nbuffer;
	if (in_port->header != NULL) {
		OSCL_LOGE("err! header is not null while set up ports!");
		oscl_free(in_port->header);
	}
	in_port->header = oscl_zalloc(in_port->nbuffer * sizeof(OMX_BUFFERHEADERTYPE *));
	oscl_param_check_exit(NULL != in_port->header,
		OMX_ErrorInsufficientResources, NULL);

	al_out = out_port->comp_info;
	al_in = in_port->comp_info;
	for (i = 0; i < out_port->nbuffer; i++) {
		/* require buffer : allocate buffer in output port */
		out_port->header[i] = NULL;
		ret = OMX_AllocateBuffer(al_out->cmp_hdl, &out_port->header[i],
				out_port->index, al_out, out_port->buf_size);
		OSCL_LOGI("=====AllocBuffer %d(p%d), %x(%x)", i, out_port->index,
			out_port->header[i],
			out_port->header[i]->pBuffer);
		if (ret != OMX_ErrorNone) {
			out_port->nbuffer = i;
			OSCL_LOGE("get Buffer %d on port %d fail, size:%d",
				i, out_port->index, out_port->buf_size);
			break;
		}
		outport_buf = out_port->header[i];
		outport_buf->nInputPortIndex = in_port->index;

		/* require buffer : use buffer in input port */
		in_port->header[i] = NULL;
		ret = OMX_UseBuffer(al_in->cmp_hdl, &in_port->header[i], in_port->index,
				al_in, in_port->buf_size, outport_buf->pBuffer);
		OSCL_LOGI("=====OMX_UseBuffer %d(p%d), %d, %x(%x)", i, in_port->index,
			in_port->buf_size, in_port->header[i],
			in_port->header[i]->pBuffer);
		if (ret != OMX_ErrorNone) {
			in_port->nbuffer = i;
			OSCL_LOGE("OMX_UseBuffer %d on port %d fail", i, in_port->index);
			break;
		}
		in_port->header[i]->nOutputPortIndex = out_port->index;
		in_port->header[i]->pOutputPortPrivate = outport_buf->pOutputPortPrivate;
		outport_buf->pInputPortPrivate = in_port->header[i]->pInputPortPrivate;

	}

	for (i = 0; (i < out_port->nbuffer) && (out_port->header[i]); i++) {
		ret = OMX_FillThisBuffer(al_out->cmp_hdl, out_port->header[i]);
		OSCL_LOGI("OMX_FillThisBuffer %d on port %d", i, out_port->index);
		if (ret != OMX_ErrorNone) {
			OSCL_LOGE("fill Buffer %d on port %d fail", i, out_port->index);
			break;
		}
	}
	out_port->state = AL_PORT_STATE_UNTUN_SETUP;
	in_port->state = AL_PORT_STATE_UNTUN_SETUP;

EXIT:
	return ret;
}

void _reset_port(al_port_info_t *port)
{
	int i;
	int ret;
	OMX_COMPONENTTYPE *omx_comp;

	if (port == NULL || (port->comp_info == NULL) || (port->header == NULL))
		return;

	port->tunnel_hdl = NULL;
	port->tunnel_port = NULL;
	port->nbuffer_hold = 0;
	port->state = AL_PORT_STATE_INIT;
	omx_comp = ((al_comp_info_t *)(port->comp_info))->cmp_hdl;
	OSCL_LOGI("%s OMX_FreeBuffer on p%d",
		((al_comp_info_t *)(port->comp_info))->name, port->index);
	for (i = 0; i < port->nbuffer; i++) {
		OSCL_LOGI("%s OMX_FreeBuffer %d on p%d:%x,%x",
			((al_comp_info_t *)(port->comp_info))->name,
			i, port->index, port->header[i],
			(port->header[i]) ? port->header[i]->pBuffer : NULL);
		ret = OMX_FreeBuffer(omx_comp, port->index, port->header[i]);
		if (ret != OMX_ErrorNone)
			OSCL_LOGE("%s OMX_FreeBuffer %d on p%d fail",
				((al_comp_info_t *)(port->comp_info))->name,
				i, port->index);
		if (port->hold_map != NULL)
			port->hold_map[i] = -1;
	}
	oscl_free(port->header);
	if (port->priv_data) {
		OSCL_LOGE("private data not freed while reset port, check it!!!");
		oscl_free(port->priv_data);
	}
	port->priv_data = NULL;
	port->header = NULL;
}

void al_untunnel_unset_ports(al_port_info_t *out_port, al_port_info_t *in_port)
{
	int nbuf_hold = 0;
	int nbuf = 0;
	int nretry = 10;
	al_comp_info_t *al_out = NULL;
	al_comp_info_t *al_in = NULL;

	if (out_port && in_port && (!out_port->is_shared_buffer)) {
		al_in = in_port->comp_info;
		al_out = out_port->comp_info;
	}
	if (al_in && al_out) {
		OSCL_LOGI("outport:%s-%d!", al_out->name, out_port->index);
		OSCL_LOGI("in_port:%s-%d!", al_in->name, in_port->index);
		do {
			nbuf_hold = out_port->nbuffer_hold + in_port->nbuffer_hold;
			nbuf = out_port->nbuffer;
			oscl_mdelay(10);
		} while (nbuf_hold != nbuf && nretry--);
	}
	if (nretry <= 0) {
		OSCL_LOGW("%s:%s nhold(%d:%d/%d) / nbuf(%d), flag:%d!",
			al_out->name, al_in->name,
			nbuf_hold, out_port->nbuffer_hold,
			in_port->nbuffer_hold, nbuf, out_port->is_shared_buffer);
	}
	_reset_port(out_port);
	_reset_port(in_port);
}

int al_untunnel_setup_cb(al_port_info_t *port, app_frame_cb_t *cb)
{
	int ret = 0;
	int i;
	al_comp_info_t *al_comp;
	app_cb_private_t *cb_private;
	OMX_PARAM_PORTDEFINITIONTYPE port_def;

	oscl_param_check((port != NULL), OMX_ErrorBadParameter, NULL);
	OSCL_LOGI("out_port:%x(%x)", port, port->comp_info);
	port->tunnel_hdl = NULL;
	port->tunnel_port = NULL;
	port->state = AL_PORT_STATE_UNTUN_SETUP;
	/* require buffer : init header array */
	if (port->header != NULL) {
		OSCL_LOGE("err! header is not null while set up ports!");
		oscl_free(port->header);
	}
	port->header = oscl_zalloc(port->nbuffer * sizeof(OMX_BUFFERHEADERTYPE *));
	oscl_param_check_exit(NULL != port->header,
			OMX_ErrorInsufficientResources, NULL);

	cb_private = port->priv_data;
	if (cb_private && cb_private->cb_frame)
		oscl_free(cb_private->cb_frame);
	if (cb_private)
		oscl_free(cb_private);
	port->priv_data = oscl_zalloc(sizeof(app_cb_private_t));
	oscl_param_check_exit(NULL != port->priv_data,
			OMX_ErrorInsufficientResources, NULL);
	cb_private = port->priv_data;
	cb_private->cb_frame = oscl_zalloc(port->nbuffer * sizeof(al_frame_t));
	oscl_param_check_exit(NULL != cb_private->cb_frame,
			OMX_ErrorInsufficientResources, NULL);

	al_comp = port->comp_info;
	/* require buffer : allocate buffer in output port */
	for (i = 0; i < port->nbuffer; i++) {
		OSCL_LOGI("=i:%d==", i);
		port->header[i] = NULL;
		ret = OMX_AllocateBuffer(al_comp->cmp_hdl, &port->header[i],
				port->index, al_comp, port->buf_size);
		OSCL_LOGI("=====AllocBuffer %d(p%d), %x(%x)", i, port->index,
				port->header[i], port->header[i]->pBuffer);
		if (ret != OMX_ErrorNone) {
			port->nbuffer = i;
			OSCL_LOGE("get Buffer %d on port %d fail, size:%d",
				i, port->index, port->buf_size);
			break;
		}
		port->header[i]->nInputPortIndex = -1;
	}

	for (i = 0; (i < port->nbuffer) && (port->header[i]); i++) {
		ret = OMX_FillThisBuffer(al_comp->cmp_hdl, port->header[i]);
		OSCL_LOGI("OMX_FillThisBuffer %d on port %d", i, port->index);
		if (ret != OMX_ErrorNone) {
			OSCL_LOGE("fill Buffer %d on port %d fail", i, port->index);
			break;
		}
	}
	/* save output config to private */
	port_def.nPortIndex = port->index;
	port_def.nVersion.nVersion = OMX_VERSION;
	ret = OMX_GetParameter(((al_comp_info_t *)port->comp_info)->cmp_hdl,
			OMX_IndexParamPortDefinition, &port_def);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);
	cb_private->port = port;
	OSCL_LOGI("port_def.eDomain:%d", port_def.eDomain);
	if (port_def.eDomain == OMX_PortDomainVideo) {
		cb_private->info.video.height = port_def.format.video.nFrameHeight;
		cb_private->info.video.width = port_def.format.video.nFrameWidth;
		cb_private->info.video.color_fmt = port_def.format.video.eColorFormat;
		cb_private->info.video.planar = 2;
		calc_frame_size(cb_private->info.video.color_fmt,
			cb_private->info.video.width,
			cb_private->info.video.height,
			(OMX_U32 *)cb_private->info.video.size,
			port_def.nBufferAlignment);
	}

	memset(&port->cb, 0, sizeof(app_frame_cb_t));
	if (cb)
		memcpy(&port->cb, cb, sizeof(app_frame_cb_t));


EXIT:
	if (ret != 0)
		_reset_port(port);
	return ret;
}

void al_untunnel_unset_cb(al_port_info_t *out_port)
{
	app_cb_private_t *cb_private;
	int i;
	cb_private = out_port->priv_data;
	if (cb_private == NULL || cb_private->cb_frame == NULL)
		return;

	for (i = 0; i < out_port->nbuffer; i++) {
		if (cb_private->cb_frame[i].header != NULL) {
			OSCL_LOGW("framebuffer[%d] not freed", i);
			oscl_mdelay(10);
			i--;
		}
	}
	if (cb_private && cb_private->cb_frame)
		oscl_free(cb_private->cb_frame);
	if (cb_private)
		oscl_free(cb_private);
	out_port->priv_data = NULL;
	_reset_port(out_port);
}

