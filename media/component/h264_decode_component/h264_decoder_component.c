#define DBG_LEVEL         DBG_ERR

#include <oscl.h>
#include <base_component.h>
#include "h264_decoder_component.h"
#include "h264_decoder_api.h"

#define DEFAULT_ERR_FRAMECNT	30

static void update_dec_ctx(h264_dec_ctx_t *ctx,
	OMX_PARAM_PORTDEFINITIONTYPE *port_def)
{
	int buffer_size;

	ctx->out_buf_cnt = port_def->nBufferCountActual;
	buffer_size = port_def->format.video.nStride *
		port_def->format.video.nSliceHeight;

	switch (port_def->format.video.eColorFormat) {
	case OMX_COLOR_FormatYUV420SemiPlanar:
		ctx->output_fmt = VDC_YUV420SP;
		ctx->out_planes = 2;
		ctx->out_buf_offset[0] = 0;
		ctx->out_buf_offset[1] =
			(buffer_size + 1024 - 1) & 0xFFFFFC00; /* alinged 1024 */
		break;
	default:
		OSCL_LOGE("color fmt %d not supportted\n",
			port_def->format.video.eColorFormat);
		break;
	}
}

void h264dec_buffer_handle(OMX_HANDLETYPE stand_com,
	OMX_BUFFERHEADERTYPE *inbuffer,
	OMX_BUFFERHEADERTYPE *outbuffer)
{
	int ret = 0;
	lb_omx_component_t *component = NULL;
	h264_dec_ctx_t *ctx = NULL;
	OMX_U8 *tmp = NULL;

	component = get_lb_component(stand_com);
	ctx = (h264_dec_ctx_t *)component->component_private;

	OSCL_LOGD("handle out buffer %p\n", outbuffer->pBuffer);
	if (ctx->is_err_state) {
		inbuffer->nFilledLen = 0;
		return;
	}
	if ((inbuffer->nFlags & OMX_BUFFERFLAG_ENDOFFRAME) == 0) {
		if (ctx->tmp_buf_len) {
			tmp = oscl_zalloc(ctx->tmp_buf_len + inbuffer->nFilledLen);
			if (tmp == NULL) {
				OSCL_LOGE("malloc tmp buf err\n");
				return;
			}
			memcpy(tmp, ctx->tmp_in_buf, ctx->tmp_buf_len);
			memcpy(tmp + ctx->tmp_buf_len,
				inbuffer->pBuffer, inbuffer->nFilledLen);
			oscl_free(ctx->tmp_in_buf);
		} else {
			tmp = oscl_zalloc(inbuffer->nFilledLen);
			if (tmp == NULL) {
				OSCL_LOGE("malloc tmp buf err\n");
				return;
			}
			memcpy(tmp, inbuffer->pBuffer, inbuffer->nFilledLen);
		}
		ctx->tmp_in_buf = tmp;
		ctx->tmp_buf_len += inbuffer->nFilledLen;
		OSCL_LOGI("frame not complete, tmp len %d\n", ctx->tmp_buf_len);
		inbuffer->nFilledLen = 0;
		return;
	}
	h264_enqueue_empty_frame(ctx->decoder,
		(fb_buffer_t *)outbuffer->pOutputPortPrivate);
	if (ctx->tmp_buf_len) {
		tmp = oscl_zalloc(ctx->tmp_buf_len + inbuffer->nFilledLen);
		if (tmp == NULL) {
			OSCL_LOGE("malloc tmp buf err\n");
			return;
		}
		memcpy(tmp, ctx->tmp_in_buf, ctx->tmp_buf_len);
		memcpy(tmp + ctx->tmp_buf_len,
			inbuffer->pBuffer, inbuffer->nFilledLen);
		ctx->tmp_buf_len += inbuffer->nFilledLen;
		OSCL_LOGI("frame complete, len %d\n", ctx->tmp_buf_len);
		oscl_free(ctx->tmp_in_buf);
		ret = h264_dec_packet(ctx->decoder, tmp, ctx->tmp_buf_len);
		oscl_free(tmp);
		ctx->tmp_in_buf = NULL;
		ctx->tmp_buf_len = 0;
	} else {
		OSCL_LOGD("decode packet len %d\n", inbuffer->nFilledLen);
		ret = h264_dec_packet(ctx->decoder,
			inbuffer->pBuffer, inbuffer->nFilledLen);
	}
	if (ret == -H264DEC_PARSE_SLICE_ERR) {
		OSCL_LOGE("decode slice header err\n");
		ctx->is_err_state = OMX_TRUE;
		((component->callbacks.EventHandler))
			(&component->base_comp, component->callback_data,
			OMX_EventError, ret, -1, NULL);
	}

	if (ret == -H264DEC_STREAM_ERR) {
		OSCL_LOGE("stream err frame cnt %d\n", ctx->err_frame_cnt);
		ctx->err_frame_cnt++;
		if (ctx->err_frame_cnt > DEFAULT_ERR_FRAMECNT) {
			ctx->is_err_state = OMX_TRUE;
			ctx->err_frame_cnt = 0;
			((component->callbacks.EventHandler))
			(&component->base_comp,
				component->callback_data, OMX_EventError,
				ret, -1, NULL);
		}
	}
	if (ret == 0)
		ctx->err_frame_cnt = 0;
	inbuffer->nFilledLen = 0;
}

void *h264dec_buffer_manager(void *param)
{
	int ret = 0;
	lb_omx_component_t *component = NULL;
	h264_dec_ctx_t *ctx = NULL;
	base_port_t *inport = NULL;
	base_port_t *outport = NULL;
	OMX_BUFFERHEADERTYPE *outbuffer = NULL;
	OMX_BUFFERHEADERTYPE *inbuffer = NULL;
	fb_buffer_t *out_frame = NULL;

	OSCL_TRACE(" %p\n", param);
	oscl_param_check((param != NULL), NULL, NULL);
	component = get_lb_component(param);
	ctx = (h264_dec_ctx_t *)component->component_private;
	inport = &component->port[BASE_INPUT_PORT];
	outport = &component->port[BASE_OUTPUT_PORT];

	while (component->state == OMX_StateIdle
		|| component->state == OMX_StateExecuting
		|| component->state == OMX_StatePause
		|| component->target_state == OMX_StateIdle) {

		/* flush the ports if they are being flushed */
		pthread_mutex_lock(&component->flush_mutex);
		while (outport->is_flushed || inport->is_flushed) {
			pthread_mutex_unlock(&component->flush_mutex);
			if (outbuffer && outport->is_flushed) {
				outport->return_buffer(outport, outbuffer);
				outbuffer = NULL;
				OSCL_LOGI("retrun outbuffer while flushing port");
			}
			h264_dec_flush(ctx->decoder);
			do {
				if (out_frame)
					outport->recive_buffer(outport,
						(OMX_BUFFERHEADERTYPE *)out_frame->priv);
				OSCL_LOGD("flush out frame %p\n", out_frame);
				out_frame = NULL;
				h264_dequeue_full_frame(ctx->decoder, &out_frame);
			} while (out_frame != NULL);
			if (inbuffer && inport->is_flushed) {
				inport->return_buffer(inport, inbuffer);
				inbuffer = NULL;
				OSCL_LOGI("retrun inbuffer while flushing port");
			}
			sem_post(component->mgmt_flush_sem);
			sem_wait(component->flush_sem);
			OSCL_LOGW("%s flush in buffer manager thread, in %d, out %d\n",
				get_component_name(component), inport->buffer_queue.count,
				outport->buffer_queue.count);
			pthread_mutex_lock(&component->flush_mutex);
		}
		pthread_mutex_unlock(&component->flush_mutex);

		OSCL_LOGD("component->state:%d, sem value:%d", component->state,
			component->buf_mgnt_sem->sem->value);
		if (component->state != OMX_StateExecuting) {
			sem_wait(component->buf_mgnt_sem);
			continue;
		}

		/* get input & output buffer */
		if (inbuffer == NULL)
			inbuffer = oscl_queue_dequeue(&inport->buffer_queue);
		if (outbuffer == NULL)
			outbuffer = oscl_queue_dequeue(&outport->buffer_queue);

		/* if header is not decoded */
		if (!ctx->is_header_decoded) {
			if (inbuffer == NULL) {
				sem_wait(component->buf_mgnt_sem);
				continue;
			}
			if ((inbuffer->nFlags & OMX_BUFFERFLAG_CODECCONFIG) ==
				OMX_BUFFERFLAG_CODECCONFIG)
				OSCL_LOGW("codec config buffer!\n");
			OSCL_LOGW("extra data size %d\n", inbuffer->nFilledLen);
			ret = h264_dec_header(ctx->decoder, inbuffer->pBuffer,
					inbuffer->nFilledLen, &ctx->info);
			if (ret) {
				OSCL_LOGE("decode h264 header error!\n");
				((component->callbacks.EventHandler))
				(&component->base_comp,
					component->callback_data, OMX_EventError,
					ret, -1, NULL);
			}
			if (outport->port_param.format.video.nFrameWidth
				!= ctx->info.width) {
				/* notify port settings changes */
				((component->callbacks.EventHandler))
				(&component->base_comp,
					component->callback_data,
					OMX_EventPortSettingsChanged,
					OMX_DirOutput, 0, &outport->port_param);
			}
			ctx->is_header_decoded = OMX_TRUE;
			/* return the input buffer */
			inbuffer->nFilledLen = 0;
			inport->return_buffer(inport, inbuffer);
			inbuffer = NULL;
			continue;
		}

		if (outbuffer == NULL || inbuffer == NULL) {
			OSCL_LOGD("==========%d\n", component->buf_mgnt_sem->sem->value);
			ret = oscl_sem_timedwait_ms(component->buf_mgnt_sem, 1000);
			if (ret)
				OSCL_LOGD("waiting timeout buffer:%x %x\n",
					inbuffer, outbuffer);
			continue;
		}
		outbuffer->nTimeStamp = inbuffer->nTimeStamp;
		outbuffer->nFlags = inbuffer->nFlags;
		if (inbuffer->nFlags & OMX_BUFFERFLAG_STARTTIME) {
			OSCL_LOGI("input buffer start flag detected\n");
			outbuffer->nFlags = inbuffer->nFlags;
			inbuffer->nFlags &= ~OMX_BUFFERFLAG_STARTTIME;
		}

		/* set mark data to outbuffer if any */
		if (component->mark.hMarkTargetComponent) {
			outbuffer->hMarkTargetComponent
				= component->mark.hMarkTargetComponent;
			outbuffer->pMarkData
				= component->mark.pMarkData;
			component->mark.hMarkTargetComponent = NULL;
			component->mark.pMarkData = NULL;
		}

		if (component->state == OMX_StateExecuting ||
			component->state == OMX_StatePause) {
			if (component->buf_handle && inbuffer->nFilledLen > 0)
				((component->buf_handle))(component, inbuffer, outbuffer);
			else
				inbuffer->nFilledLen = 0;
		}

		/* if inbuffer is mark by this component, then call back */
		if (inbuffer->hMarkTargetComponent != NULL) {
			if ((OMX_COMPONENTTYPE *)inbuffer->hMarkTargetComponent
				== &component->base_comp) {
				/*Clear the mark and generate an event*/
				((component->callbacks.EventHandler))
				(&component->base_comp,
					component->callback_data, OMX_EventMark,
					0, 0, inbuffer->pMarkData);
			} else {
				/* pass the mark*/
				outbuffer->hMarkTargetComponent
					= inbuffer->hMarkTargetComponent;
				outbuffer->pMarkData
					= inbuffer->pMarkData;
			}
			inbuffer->hMarkTargetComponent = NULL;
		}

		/* if we have buffer end flag then notify the apps */
		if ((outbuffer->nFlags & OMX_BUFFERFLAG_EOS) == OMX_BUFFERFLAG_EOS) {
			OSCL_LOGW("Detected EOS flags, filled len=%d\n",
				(int)inbuffer->nFilledLen);
			(*(component->callbacks.EventHandler))
			(&component->base_comp,
				component->callback_data,
				OMX_EventBufferFlag, /* The command was completed */
				inport->port_param.nPortIndex,
				OMX_BUFFERFLAG_EOS,
				NULL);
			inbuffer->nFlags = 0;
			component->eos_flag = OMX_TRUE;
			outbuffer->nFilledLen = 0;
			outbuffer->nTimeStamp = inbuffer->nTimeStamp;
			outport->return_buffer(outport, outbuffer);
		}

		/* return inbuffer if it's consumed */
		if (inbuffer->nFilledLen == 0) {
			inbuffer->nFlags = 0;
			inport->return_buffer(inport, inbuffer);
			inbuffer = NULL;
		}

		if (ctx->tmp_buf_len) {
			OSCL_LOGI("we have an incomplete frame, continue\n");
			continue;
		}

		/* we have handle the outbuffer, set it to null */
		outbuffer = NULL;

		/* get one decoded frame */
		h264_dequeue_full_frame(ctx->decoder, &out_frame);
		if (out_frame && !component->eos_flag) {
			OSCL_LOGD("get frame index %d, frame addr 0x%0x\n",
				out_frame->index, out_frame->phy_addr[0]);
			if (out_frame->buf_flag == H264DEC_DISPLAY) {
				outbuffer = (OMX_BUFFERHEADERTYPE *)out_frame->priv;
				outbuffer->nFilledLen = outbuffer->nAllocLen;
				outport->return_buffer(outport, outbuffer);
				outbuffer = NULL;
			} else
				outport->recive_buffer(outport,
					(OMX_BUFFERHEADERTYPE *)out_frame->priv);
			out_frame = NULL;
		}
	}
	oscl_queue_flush(&inport->buffer_queue);
	oscl_queue_flush(&outport->buffer_queue);
	OSCL_LOGW("exit from buffer_manager:%s\n", rt_thread_self()->name);
	OSCL_LOGW("exit, inport buffer left %d, out buffer left %d\n",
		inport->buffer_queue.count, outport->buffer_queue.count);
	OSCL_TRACE(" %p\n", param);
	return NULL;
}

OMX_ERRORTYPE h264dec_set_parameter(OMX_IN OMX_HANDLETYPE hComp,
	OMX_IN OMX_INDEXTYPE paramIndex,
	OMX_IN OMX_PTR paramData)
{
	lb_omx_component_t *component = NULL;
	h264_dec_ctx_t *ctx = NULL;
	OMX_PARAM_PORTDEFINITIONTYPE *port_def = NULL;

	oscl_param_check(hComp != NULL, OMX_ErrorBadParameter, NULL);
	component = get_lb_component(hComp);
	ctx = component->component_private;

	if (paramIndex == OMX_IndexParamPortDefinition) {
		port_def = (OMX_PARAM_PORTDEFINITIONTYPE *)paramData;
		if (port_def->nPortIndex == BASE_OUTPUT_PORT)
			update_dec_ctx(ctx, port_def);
	}

	return base_set_parameter(hComp, paramIndex, paramData);
}

OMX_ERRORTYPE h264dec_set_state(OMX_HANDLETYPE hComp,
	OMX_U32 dest_state)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component = NULL;
	h264_dec_ctx_t *ctx = NULL;
	OMX_STATETYPE pre_state;
	dec_open_args_t args;

	OSCL_TRACE(" %p, %x\n", hComp, dest_state);
	oscl_param_check(hComp != NULL, OMX_ErrorBadParameter, NULL);
	component = get_lb_component(hComp);
	ctx = component->component_private;

	if (dest_state == OMX_StateExecuting && component->state == OMX_StateIdle) {
		if (ctx->decoder != NULL) {
			OSCL_LOGW("Device not closed while in StateIdle");
			h264_dec_close(ctx->decoder);
			ctx->decoder = NULL;
		}
		args.ouput_type = ctx->output_fmt;
		args.frame_buf_cnt = ctx->out_buf_cnt;
		ctx->decoder = h264_dec_open(&args);
		if (ctx->decoder == NULL) {
			OSCL_LOGE("open h264 decoder error!\n");
			return OMX_ErrorHardware;
		}
		ctx->is_err_state = OMX_FALSE;
		ctx->err_frame_cnt = 0;
		ctx->tmp_buf_len = 0;
		ctx->tmp_in_buf = NULL;
		OSCL_LOGI("out port queue num %d\n",
			component->port[1].buffer_queue.count);
	}

	pre_state = component->state;
	ret = base_component_set_state(hComp, dest_state);
	if (dest_state == OMX_StateIdle &&
		(pre_state == OMX_StateExecuting || pre_state == OMX_StatePause)) {
		OSCL_LOGI("close h264 decoder\n");
		h264_dec_close(ctx->decoder);
		ctx->decoder = NULL;
		ctx->is_header_decoded = 0;
		component->eos_flag = OMX_FALSE;
		if (ctx->tmp_in_buf) {
			oscl_free(ctx->tmp_in_buf);
			ctx->tmp_in_buf = NULL;
		}
		ctx->tmp_buf_len = 0;
	}
	return ret;
}

OMX_ERRORTYPE h264dec_component_deinit(OMX_IN OMX_HANDLETYPE hComponent)
{
	OMX_COMPONENTTYPE *base_cmp = (OMX_COMPONENTTYPE *)hComponent;
	lb_omx_component_t *component = NULL;
	h264_dec_ctx_t *ctx = NULL;
	OMX_ERRORTYPE ret = OMX_ErrorNone;

	OSCL_TRACE("base_cmp_handle:%p\n", hComponent);
	oscl_param_check(hComponent != NULL, OMX_ErrorBadParameter, NULL);
	component = (lb_omx_component_t *)base_cmp->pComponentPrivate;
	ctx = (h264_dec_ctx_t *)component->component_private;

	base_port_deinit(&component->port[OMX_DEFAULT_INPUT_PORT]);
	base_port_deinit(&component->port[OMX_DEFAULT_OUTPUT_PORT]);
	oscl_free(ctx);
	ret = base_component_deinit(hComponent);

	return ret;
}

OMX_ERRORTYPE h264dec_component_init(lb_omx_component_t *cmp_handle)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	h264_dec_ctx_t *ctx = NULL;

	ctx = oscl_zalloc(sizeof(h264_dec_ctx_t));
	if (!ctx) {
		OSCL_LOGE("malloc h264_dec_ctx_t error!\n");
		return OMX_ErrorInsufficientResources;
	}

	/* init component */
	ret = base_component_init(cmp_handle);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("base_component_init error!\n");
		goto error1;
	}
	cmp_handle->name = "OMX.LB.VIDEO.H264DEC";
	cmp_handle->component_private = ctx;
	cmp_handle->buf_manager = h264dec_buffer_manager;
	cmp_handle->buf_handle = h264dec_buffer_handle;
	cmp_handle->base_comp.ComponentDeInit = h264dec_component_deinit;
	cmp_handle->base_comp.SetParameter = h264dec_set_parameter;
	cmp_handle->do_state_set = h264dec_set_state;
	cmp_handle->num_ports = 2;

	/* init input & output port */
	ret = base_port_init(cmp_handle, &cmp_handle->port[OMX_DEFAULT_INPUT_PORT],
			OMX_DEFAULT_INPUT_PORT,
			OMX_DirInput);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE(" input base_port_init error!\n");
		goto error2;
	}
	ret = h264dec_outport_init(cmp_handle, &cmp_handle->port[OMX_DEFAULT_OUTPUT_PORT],
			OMX_DEFAULT_OUTPUT_PORT,
			OMX_DirOutput);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("output base_port_init error!\n");
		goto error3;
	}
	cmp_handle->port[OMX_DEFAULT_INPUT_PORT].port_param.eDomain = OMX_PortDomainVideo;
	cmp_handle->port[OMX_DEFAULT_OUTPUT_PORT].port_param.eDomain =
								OMX_PortDomainVideo;

	return ret;

error3:
	ret = base_port_deinit(&cmp_handle->port[OMX_DEFAULT_INPUT_PORT]);
error2:
	ret = base_component_deinit(&cmp_handle->base_comp);
error1:
	if (ctx != NULL)
		oscl_free(ctx);
	OSCL_TRACE("%d arec:%p\n", ret, cmp_handle);

	return ret;
}

