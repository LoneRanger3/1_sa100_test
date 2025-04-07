#define DBG_LEVEL         DBG_ERR

#include <oscl.h>
#include <base_component.h>
#include <omx_vendor_lb.h>
#include <lb_omx_core.h>
#include <lb_omx_as_string.h>
#include "vrender_component.h"
#include "clock_component.h"
#include "demuxer_component.h"
#include "omx_mediaplayer_priv.h"
#include "omx_mediaplayer.h"
/* #include "osal_mem.h" */

static int omxmp_stream_eos(omx_mediaplayer_t *player)
{
	int ret = OMX_ErrorNone;
	oscl_param_check(player != NULL, -1, "player null\n");

	if (player->status == OMXMP_STATE_COMPLETED) {
		OSCL_LOGE("already in complete state\n");
		return ret;
	}

	/* if it's loop, then seek to start & play from beginning */
	if (player->isloop) {
		ret = omxmp_seek_to(player, 0);
		return ret;
	}

	pthread_mutex_lock(&player->lock);
	/* set clock component to IDLE */
	ret = OMX_SendCommand(player->clocksrc,
			OMX_CommandStateSet, OMX_StateIdle, NULL);
	oscl_param_check_exit(ret == 0, -1, "set clock to idle err!\n");
	sem_wait(&player->sem_event);

	/* set other component to idle */
	if (player->has_audio) {
		ret = OMX_SendCommand(player->arender,
			OMX_CommandStateSet, OMX_StateIdle, NULL);
		oscl_param_check_exit(ret == 0, -1, "set arender to idle err!\n");
		sem_wait(&player->sem_event);
		ret = OMX_SendCommand(player->audiodec,
			OMX_CommandStateSet, OMX_StateIdle, NULL);
		oscl_param_check_exit(ret == 0, -1, "set audiodec to idle err!\n");
		sem_wait(&player->sem_event);
	}
	if (player->has_video) {
		ret = OMX_SendCommand(player->vrender,
			OMX_CommandStateSet, OMX_StateIdle, NULL);
		oscl_param_check_exit(ret == 0, -1, "set vrender to idle err!\n");
		sem_wait(&player->sem_event);
		if (player->rot_mode.nRotation) {
			ret = OMX_SendCommand(player->rotate,
				OMX_CommandStateSet, OMX_StateIdle, NULL);
			oscl_param_check_exit(ret == 0, -1, "set rotate to idle err!\n");
			sem_wait(&player->sem_event);
		}
		ret = OMX_SendCommand(player->videodec,
			OMX_CommandStateSet, OMX_StateIdle, NULL);
		oscl_param_check_exit(ret == 0, -1, "set videodec to idle err!\n");
		sem_wait(&player->sem_event);
	}
	ret = OMX_SendCommand(player->demuxer, OMX_CommandStateSet, OMX_StateIdle, NULL);
	oscl_param_check_exit(ret == 0, -1, "set demuxer to idle err!\n");
	sem_wait(&player->sem_event);
	player->status = OMXMP_STATE_COMPLETED;
	pthread_mutex_unlock(&player->lock);

	/* call back to notify completion */
	ret = omxmp_send_msg(player, OMXMP_MSG_COMPLETE, 0, NULL);
	OSCL_LOGI("=====stream eos end==========\n");
	return ret;
EXIT:
	pthread_mutex_unlock(&player->lock);
	return ret;
}

static OMX_ERRORTYPE omxmp_event_handler(
	OMX_HANDLETYPE hComponent,
	OMX_PTR pAppData,
	OMX_EVENTTYPE eEvent,
	OMX_U32 Data1,
	OMX_U32 Data2,
	OMX_PTR pEventData)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component = NULL;

	oscl_param_check((hComponent != NULL && pAppData != NULL),
		OMX_ErrorBadParameter, NULL);

	omx_mediaplayer_t *player = pAppData;
	component = get_lb_component(hComponent);

	if (eEvent == OMX_EventCmdComplete) {
		OSCL_LOGI("complete cmd :%s, component %s",
			omx_cmd_as_string(Data1), component->name);
		if (Data1 == OMX_CommandStateSet)
			OSCL_LOGI("set state complete:%s, component %s",
				omx_state_as_string(Data2), component->name);
		sem_post(&player->sem_event);
	}
	if (eEvent == OMX_EventError) {
		OSCL_LOGW("command err:%s, %x", omx_cmd_as_string(Data2), Data1);
		if (Data2 == OMX_CommandStateSet ||
			Data2 == OMX_CommandFlush ||
			Data2 == OMX_CommandPortDisable ||
			Data2 == OMX_CommandPortEnable ||
			Data2 == OMX_CommandMarkBuffer)
			sem_post(&player->sem_event);
		else {
			if (hComponent == player->videodec ||
				hComponent == player->demuxer)
				omxmp_send_msg(player, OMXMP_MSG_ERROR,
					OMXMP_ERR_VIDEO_DEC, NULL);
		}
	}
	if (eEvent == OMX_EventBufferFlag) {
		if ((int)Data2 == OMX_BUFFERFLAG_EOS) {
			OSCL_LOGW("buffer eos, cmp %s\n", component->name);
			if (player->arender == hComponent)
				player->audio_eos = OMX_TRUE;
			else if (player->vrender == hComponent) {
				player->video_eos = OMX_TRUE;
				player->audio_eos = OMX_TRUE;
			}
			if (player->audio_eos && player->video_eos)
				omxmp_send_msg(player, OMXMP_MSG_STREAM_EOS,
					0, NULL);
			if (player->has_audio && !player->has_video &&
				player->audio_eos)
				omxmp_send_msg(player, OMXMP_MSG_STREAM_EOS,
					0, NULL);
		}
	}
	if (eEvent == OMX_EventPortSettingsChanged) {
		OSCL_LOGI("port setting changed, cmp %s, index %d\n",
			component->name, Data2);
		/* todo: we should get the new port definiton params here
		 * and init the auido & video decoder component here */
	}
	if (eEvent == OMX_EventPortFormatDetected) {
		OSCL_LOGI("port %d format detected\n", Data2);
		if (Data1 == OMX_IndexParamAudioPortFormat)
			player->has_audio = OMX_TRUE;
		else if (Data1 == OMX_IndexParamVideoPortFormat)
			player->has_video = OMX_TRUE;
		else
			OSCL_LOGW("unkown format %lx\n", Data1);
	}
	if (eEvent == OMX_EventOutputRendered && player->is_seeking) {
		if (player->has_audio && !player->has_video &&
				hComponent == player->arender)
			omxmp_send_msg(player, OMXMP_MSG_SEEK_COMPLETE,
					0, NULL);
		if (player->has_video && hComponent == player->vrender)
			omxmp_send_msg(player, OMXMP_MSG_SEEK_COMPLETE,
					0, NULL);
	}

	return ret;
}

static OMX_ERRORTYPE omxmp_empty_buffer_done(
	OMX_HANDLETYPE hComponent,
	OMX_PTR pAppData,
	OMX_BUFFERHEADERTYPE *pBuffer)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component = NULL;
	OMX_COMPONENTTYPE *dest_cmp = NULL;
	omx_mediaplayer_t *player = pAppData;

	if (pBuffer == NULL || pAppData == NULL) {
		OSCL_LOGE("err: buffer header is null");
		return OMX_ErrorBadParameter;
	}

	component = get_lb_component(hComponent);
	OSCL_LOGD("component %s, empty buffer done>> %p, %d, input:%d output:%d",
		component->name, pBuffer->pBuffer,
		pBuffer->nFlags, pBuffer->nInputPortIndex, pBuffer->nOutputPortIndex);

	if (hComponent == player->audiodec) {
		/* queue the buffer in demuxer audio outport */
		pBuffer->nOutputPortIndex = AUDIO_PORT;
		dest_cmp = player->demuxer;
	} else if (hComponent == player->videodec) {
		/* queue the buffer in demuxer video outport */
		pBuffer->nOutputPortIndex = VIDEO_PORT;
		dest_cmp = player->demuxer;
	} else if (hComponent == player->arender) {
		/* queue the buffer in audio decoder outport */
		pBuffer->nOutputPortIndex = OMX_DEFAULT_OUTPUT_PORT;
		dest_cmp = player->audiodec;
	} else if (hComponent == player->vrender) {
		/* queue the buffer in video decoder outport */
		pBuffer->nOutputPortIndex = OMX_DEFAULT_OUTPUT_PORT;
		if (player->rot_mode.nRotation)
			dest_cmp = player->rotate;
		else
			dest_cmp = player->videodec;
	} else if (hComponent == player->rotate) {
		/* queue the buffer in video decoder outport */
		pBuffer->nOutputPortIndex = OMX_DEFAULT_OUTPUT_PORT;
		dest_cmp = player->videodec;
	} else {
		OSCL_LOGE("error component return %s\n", component->name);
		ret = OMX_ErrorBadParameter;
		return ret;
	}
	if (component->port[pBuffer->nInputPortIndex].is_flushed ||
		(get_lb_component(dest_cmp))->state == OMX_StateIdle ||
		(get_lb_component(dest_cmp))
			->port[pBuffer->nOutputPortIndex].is_flushed) {
		OSCL_LOGD("component %s, flush buffer %p\n",
			component->name, pBuffer->pBuffer);
		return ret;
	}
	ret = OMX_FillThisBuffer(dest_cmp, pBuffer);
	if (ret != OMX_ErrorNone)
		OSCL_LOGE("fill buffer error %s\n", component->name);

	return ret;
}

static OMX_ERRORTYPE omxmp_fill_buffer_done(
	OMX_HANDLETYPE hComponent,
	OMX_PTR pAppData,
	OMX_BUFFERHEADERTYPE *pBuffer)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component = NULL;
	lb_omx_component_t *dst_cmp = NULL;
	omx_mediaplayer_t *player = pAppData;

	if (pBuffer == NULL || pAppData == NULL) {
		OSCL_LOGE("err: buffer header is null");
		return OMX_ErrorBadParameter;
	}

	component = get_lb_component(hComponent);
	OSCL_LOGD("component %s, empty buffer done>> %p, %d, input:%d output:%d",
		component->name, pBuffer->pBuffer,
		pBuffer->nFlags, pBuffer->nInputPortIndex, pBuffer->nOutputPortIndex);

	if (hComponent == player->demuxer) {
		if (pBuffer->nOutputPortIndex == AUDIO_PORT) {
			/* queue the buffer in audiodec inport */
			pBuffer->nInputPortIndex = OMX_DEFAULT_INPUT_PORT;
			dst_cmp = get_lb_component(player->audiodec);
		} else if (pBuffer->nOutputPortIndex == VIDEO_PORT) {
			/* queue the buffer in videodec inport */
			pBuffer->nInputPortIndex = OMX_DEFAULT_INPUT_PORT;
			dst_cmp = get_lb_component(player->videodec);
		}
	} else if (hComponent == player->audiodec) {
		/* queue the buffer in audio render inport */
		pBuffer->nInputPortIndex = OMX_DEFAULT_INPUT_PORT;
		dst_cmp = get_lb_component(player->arender);
	} else if (hComponent == player->videodec) {
		/* queue the buffer in rotate or vrender inport */
		pBuffer->nInputPortIndex = OMX_DEFAULT_INPUT_PORT;
		if (player->rot_mode.nRotation)
			dst_cmp = get_lb_component(player->rotate);
		else
			dst_cmp = get_lb_component(player->vrender);
	} else if (hComponent == player->rotate) {
		/* queue the buffer in video render inport */
		pBuffer->nInputPortIndex = OMX_DEFAULT_INPUT_PORT;
		dst_cmp = get_lb_component(player->vrender);
	} else {
		OSCL_LOGE("error component return %s\n", component->name);
		ret = OMX_ErrorBadParameter;
	}
	if (component->port[pBuffer->nOutputPortIndex].is_flushed ||
		dst_cmp->port[pBuffer->nInputPortIndex].is_flushed ||
		dst_cmp->state == OMX_StateIdle) {
		OSCL_LOGD("component %s, flush buffer %p\n",
			component->name, pBuffer->pBuffer);
		return ret;
	}
	ret = OMX_EmptyThisBuffer(dst_cmp, pBuffer);
	if (ret != OMX_ErrorNone)
		OSCL_LOGE("empty buffer error %s\n", component->name);

	return ret;
}

static OMX_CALLBACKTYPE omxmp_component_callbacks = {
	.EventHandler = omxmp_event_handler,
	.EmptyBufferDone = omxmp_empty_buffer_done,
	.FillBufferDone = omxmp_fill_buffer_done,
};

static int init_outport_buffer(OMX_COMPONENTTYPE *cmphl,
	OMX_BUFFERHEADERTYPE **headers, int size, int count, int portindex)
{
	int ret;
	int i, j;

	for (i = 0; i < count; i++) {
		/* alloc the actual buffer */
		ret = OMX_AllocateBuffer(cmphl, &headers[i],
			portindex, cmphl, size);
		if (ret != OMX_ErrorNone) {
			OSCL_LOGE("omx use buffer %d error\n", i);
			goto error;
		}
	}

	return 0;
error:
	for (j = 0; j < i; j++) {
		if (headers[j])
			OMX_FreeBuffer(cmphl, portindex, headers[j]);
	}

	return -1;
}

static int queue_outport_buffer(OMX_COMPONENTTYPE *cmphl,
	OMX_BUFFERHEADERTYPE **headers, int count)
{
	int ret;
	int i;

	for (i = 0; i < count; i++) {
		ret = OMX_FillThisBuffer(cmphl, headers[i]);
		if (ret != OMX_ErrorNone) {
			OSCL_LOGE("omx fill buffer %d error\n", i);
			return ret;
		}
	}

	return ret;
}

static void deinit_outport_buffer(OMX_COMPONENTTYPE *cmphl,
	OMX_BUFFERHEADERTYPE **headers, int count, int portindex)
{
	int i;

	for (i = 0; i < count; i++) {
		if (headers[i]) {
			OMX_FreeBuffer(cmphl,
				portindex, headers[i]);
		}
	}
}

static int config_audiodec_cmp(omx_mediaplayer_t *player)
{
	int ret = OMX_ErrorNone;
	OMX_PARAM_PORTDEFINITIONTYPE params;
#if 0
	/* set audio coding type */
	memset(&audio_fmt, 0, sizeof(audio_fmt));
	audio_fmt.nPortIndex = 0;
	audio_fmt.eEncoding = player->audio_codingtype;
	ret = OMX_SetParameter(player->audiodec,
			OMX_IndexParamAudioPortFormat, &audio_fmt);
	oscl_param_check(ret == OMX_ErrorNone, -1, "SET port definition err\n");
#endif
	/* set audio pcm params */
	if (player->audio_codingtype == OMX_AUDIO_CodingAAC) {
		OMX_AUDIO_PARAM_AACPROFILETYPE audio_params;
		audio_params.nChannels = player->channels;
		audio_params.nSampleRate = player->sample_rate;
		audio_params.nBitRate = player->bit_rate;
		ret = OMX_SetParameter(player->audiodec,
				OMX_IndexParamAudioAac, &audio_params);
		oscl_param_check(ret == OMX_ErrorNone, -1, "SET audio pcm params err\n");
	} else if (player->audio_codingtype == OMX_AUDIO_CodingPCM) {
		OMX_AUDIO_PARAM_PCMMODETYPE audio_params;
		audio_params.nChannels = player->channels;
		audio_params.nSamplingRate = player->sample_rate;
		ret = OMX_SetParameter(player->audiodec,
				OMX_IndexParamAudioPcm, &audio_params);
		oscl_param_check(ret == OMX_ErrorNone, -1, "SET audio pcm params err\n");
	} else {
		OSCL_LOGE("coding type %d not support yet");
		return -1;
	}

	/* get audio outport definition */
	memset(&params, 0, sizeof(params));
	params.nPortIndex = 1;
	ret = OMX_GetParameter(player->audiodec, OMX_IndexParamPortDefinition, &params);
	oscl_param_check(ret == OMX_ErrorNone, -1, "get port definition err\n");

	/* update audio outport definiton */
	params.format.audio.eEncoding = player->audio_codingtype;
	params.nBufferCountActual = player->araw_buf_cnt;
	params.nBufferSize        = player->araw_buf_size;
	ret = OMX_SetParameter(player->audiodec, OMX_IndexParamPortDefinition, &params);
	oscl_param_check(ret == OMX_ErrorNone, -1, "SET port definition err\n");

	/* alloc audio outport buffer */
	ret = init_outport_buffer(player->audiodec, player->outbuf_audio_dec,
			params.nBufferSize, params.nBufferCountActual, 1);
	oscl_param_check(ret == 0, -1, "alloc audio port buffer err\n");

	return ret;
}

static int config_videodec_cmp(omx_mediaplayer_t *player)
{
	int ret = OMX_ErrorNone;
	OMX_PARAM_PORTDEFINITIONTYPE params;
	int frame_size;

	/* get video outport definition */
	memset(&params, 0, sizeof(params));
	params.nPortIndex = 1;
	ret = OMX_GetParameter(player->videodec, OMX_IndexParamPortDefinition, &params);
	oscl_param_check(ret == OMX_ErrorNone, -1, "get port definition err\n");

	/* update video outport definiton */
	params.format.video.eCompressionFormat = player->video_codingtype;
	params.format.video.eColorFormat = OMX_COLOR_FormatYUV420SemiPlanar;
	params.format.video.nFrameWidth = player->frame_width;
	params.format.video.nFrameHeight = player->frame_height;
	params.format.video.nStride = player->frame_stride;
	params.format.video.nSliceHeight = player->frame_sliceheight;
	params.nBufferCountActual = player->vraw_buf_cnt;
	frame_size = params.format.video.nStride *
			params.format.video.nSliceHeight;
	player->vraw_buf_size = ((frame_size + 1024 - 1) & 0xFFFFFC00)
					+ frame_size / 2; /* plane alinged 1024 */
	params.nBufferSize        = player->vraw_buf_size;
	ret = OMX_SetParameter(player->videodec, OMX_IndexParamPortDefinition, &params);
	oscl_param_check(ret == OMX_ErrorNone, -1, "SET port definition err\n");

	/* alloc video outport buffer */
	ret = init_outport_buffer(player->videodec, player->outbuf_video_dec,
			params.nBufferSize, params.nBufferCountActual, 1);
	oscl_param_check(ret == 0, -1, "alloc audio port buffer err\n");

	return ret;
}

static int config_arender_cmp(omx_mediaplayer_t *player)
{
	int ret = OMX_ErrorNone;
	OMX_AUDIO_PARAM_PCMMODETYPE audio_params;

	/* get audio outport definition */
	memset(&audio_params, 0, sizeof(OMX_AUDIO_PARAM_PCMMODETYPE));
	audio_params.nChannels = player->channels;
	audio_params.nBitPerSample = player->bit_per_sample;
	audio_params.nSamplingRate = player->sample_rate;
	ret = OMX_SetParameter(player->arender, OMX_IndexParamAudioPcm,
			&audio_params);
	oscl_param_check(ret == OMX_ErrorNone, -1, "SET port definition err\n");

	return ret;
}

static int config_vrender_cmp(omx_mediaplayer_t *player)
{
	int ret = OMX_ErrorNone;
	OMX_PARAM_PORTDEFINITIONTYPE params;
	OMX_BOOL avsync;

	/* setup display mode */
	ret = OMX_SetParameter(player->vrender, omx_index_vrender_mode,
			&player->disp_para);
	oscl_param_check(ret == OMX_ErrorNone, ret, NULL);

	/* set rotate */
	ret = OMX_SetParameter(player->vrender,
		OMX_IndexConfigCommonRotate, &player->rot_mode);
	oscl_param_check(ret == OMX_ErrorNone, ret, "set rotation err\n");

	/* get video inport definition */
	memset(&params, 0, sizeof(params));
	params.nPortIndex = 0;
	ret = OMX_GetParameter(player->vrender, OMX_IndexParamPortDefinition, &params);
	oscl_param_check(ret == OMX_ErrorNone, -1, "get port definition err\n");

	/* update video inport definiton */
	params.nBufferAlignment = 1024;
	params.format.video.eColorFormat = OMX_COLOR_FormatYUV420SemiPlanar;
	if (player->rot_mode.nRotation) {
		params.format.video.nFrameWidth = player->rotate_width;
		params.format.video.nFrameHeight = player->rotate_height;
		params.format.video.nStride = player->rotate_stride;
		params.format.video.nSliceHeight = player->rotate_sliceheight;
	} else {
		params.format.video.nFrameWidth = player->frame_width;
		params.format.video.nFrameHeight = player->frame_height;
		params.format.video.nStride = player->frame_stride;
		params.format.video.nSliceHeight = player->frame_sliceheight;
	}

	ret = OMX_SetParameter(player->vrender, OMX_IndexParamPortDefinition, &params);
	oscl_param_check(ret == OMX_ErrorNone, -1, "SET port definition err\n");

	/* set avsync */
	avsync = OMX_TRUE;
	ret = OMX_SetParameter(player->vrender, omx_index_media_avsync, &avsync);
	oscl_param_check(ret == OMX_ErrorNone, -1, "SET avsync err\n");

	return ret;
}

static int config_rotate_cmp(omx_mediaplayer_t *player)
{
	int ret = OMX_ErrorNone;
	OMX_PARAM_PORTDEFINITIONTYPE params;

	/* get rotate inport definition */
	memset(&params, 0, sizeof(params));
	params.nPortIndex = 0;
	ret = OMX_GetParameter(player->rotate, OMX_IndexParamPortDefinition, &params);
	oscl_param_check(ret == OMX_ErrorNone, -1, "get port definition err\n");

	/* update rotate inport definiton */
	params.nBufferAlignment = 1024;
	params.format.video.eColorFormat = OMX_COLOR_FormatYUV420SemiPlanar;
	params.format.video.nFrameWidth = player->frame_width;
	params.format.video.nFrameHeight = player->frame_height;
	params.format.video.nStride = player->frame_stride;
	params.format.video.nSliceHeight = player->frame_sliceheight;
	ret = OMX_SetParameter(player->rotate, OMX_IndexParamPortDefinition, &params);
	oscl_param_check(ret == OMX_ErrorNone, -1, "SET port definition err\n");

	/* get rotate outport definition */
	memset(&params, 0, sizeof(params));
	params.nPortIndex = 1;
	ret = OMX_GetParameter(player->rotate, OMX_IndexParamPortDefinition, &params);
	oscl_param_check(ret == OMX_ErrorNone, -1, "get port definition err\n");

	/* update rotate outport definition */
	params.nBufferAlignment = 1024;
	params.nBufferCountActual = player->rotate_outbuf_cnt;
	ret = OMX_SetParameter(player->rotate, OMX_IndexParamPortDefinition, &params);
	oscl_param_check(ret == OMX_ErrorNone, -1, "SET port definition err\n");

	/* set rotate angle */
	ret = OMX_SetParameter(player->rotate,
		OMX_IndexConfigCommonRotate, &player->rot_mode);
	oscl_param_check(ret == OMX_ErrorNone, ret, "set rotation err\n");

	/* get params after set rotate angle */
	ret = OMX_GetParameter(player->rotate, OMX_IndexParamPortDefinition, &params);
	oscl_param_check(ret == OMX_ErrorNone, -1, "get port definition err\n");
	player->rotate_width = params.format.video.nFrameWidth;
	player->rotate_height = params.format.video.nFrameHeight;
	player->rotate_stride = params.format.video.nStride;
	player->rotate_sliceheight = params.format.video.nSliceHeight;
	OSCL_LOGI("rot:%d %d %d %d", params.format.video.nFrameWidth,
			params.format.video.nFrameHeight,
			params.format.video.nStride,
			params.format.video.nSliceHeight);

	/* alloc rotate outport buffer */
	ret = init_outport_buffer(player->rotate, player->outbuf_rotate,
			params.nBufferSize, params.nBufferCountActual, 1);
	oscl_param_check(ret == 0, -1, "alloc audio port buffer err\n");

	return ret;
}

static int config_demuxer_cmp(omx_mediaplayer_t *player)
{
	int ret = OMX_ErrorNone;
	OMX_PARAM_PORTDEFINITIONTYPE params;

	/* get audio outport definition */
	memset(&params, 0, sizeof(params));
	params.nPortIndex = 0;
	ret = OMX_GetParameter(player->demuxer, OMX_IndexParamPortDefinition, &params);
	oscl_param_check(ret == OMX_ErrorNone, -1, "get port definition err\n");

	/* update audio outport definition */
	params.nBufferCountActual = player->astream_buf_cnt;
	params.nBufferSize        = player->astream_buf_size;
	ret = OMX_SetParameter(player->demuxer, OMX_IndexParamPortDefinition, &params);
	oscl_param_check(ret == OMX_ErrorNone, -1, "SET port definition err\n");

	/* alloc audio outport buffer */
	ret = init_outport_buffer(player->demuxer, player->outbuf_demux_audio,
			params.nBufferSize, params.nBufferCountActual, AUDIO_PORT);
	oscl_param_check(ret == 0, -1, "alloc audio port buffer err\n");

	/* get video outport definition */
	memset(&params, 0, sizeof(params));
	params.nPortIndex = 1;
	ret = OMX_GetParameter(player->demuxer, OMX_IndexParamPortDefinition, &params);
	oscl_param_check(ret == OMX_ErrorNone, -1, "get port definition err\n");

	/* update video outport definition */
	params.nBufferCountActual = player->vstream_buf_cnt;
	params.nBufferSize        = player->vstream_buf_size;
	ret = OMX_SetParameter(player->demuxer, OMX_IndexParamPortDefinition, &params);
	oscl_param_check(ret == OMX_ErrorNone, -1, "SET port definition err\n");

	/* alloc video outport buffer */
	ret = init_outport_buffer(player->demuxer, player->outbuf_demux_video,
			params.nBufferSize, params.nBufferCountActual, VIDEO_PORT);
	oscl_param_check(ret == 0, -1, "alloc video port buffer err\n");

	return ret;
}

static int config_clock_cmp(omx_mediaplayer_t *player)
{
	int ret = OMX_ErrorNone;
	OMX_TIME_CONFIG_ACTIVEREFCLOCKTYPE  refclk;

	/* set ref clk */
	refclk.nSize = sizeof(refclk);
	refclk.eClock = OMX_TIME_RefClockAudio;
	ret = OMX_SetConfig(player->clocksrc, OMX_IndexConfigTimeActiveRefClock, &refclk);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("set activ ref clock error!\n");
		return -1;
	}

	/* setup clk port tunnel */
	ret = OMX_SetupTunnel(player->clocksrc, CLOCK_PORT_VIDEO, player->vrender, 1);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("setup clock port tunnel error!\n");
		return -1;
	}
	ret = OMX_SetupTunnel(player->clocksrc, CLOCK_PORT_AUDIO, player->arender, 1);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("setup clock port tunnel error!\n");
		return -1;
	}
	/* disable clk port that we don't use */
	ret = OMX_SendCommand(player->clocksrc,
		OMX_CommandPortDisable, CLOCK_PORT_DEMUXER, NULL);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("disable clk demuxer port error!\n");
		return -1;
	}
	sem_wait(&player->sem_event);

	return 0;
}

static int get_mediainfo(omx_mediaplayer_t *player)
{
	int ret = OMX_ErrorNone;
	OMX_PARAM_PORTDEFINITIONTYPE params;

	/* get audio outport params */
	memset(&params, 0, sizeof(params));
	params.nPortIndex = 0;
	ret = OMX_GetParameter(player->demuxer, OMX_IndexParamPortDefinition, &params);
	oscl_param_check(ret == OMX_ErrorNone, -1, "get port definition err\n");
	player->audio_codingtype = params.format.audio.eEncoding;
	OSCL_LOGI("audio coding type %d\n", player->audio_codingtype);
	if (player->audio_codingtype == OMX_AUDIO_CodingAAC) {
		OMX_AUDIO_PARAM_AACPROFILETYPE audio_params;
		ret = OMX_GetParameter(player->demuxer,
				OMX_IndexParamAudioAac, &audio_params);
		oscl_param_check(ret == OMX_ErrorNone, -1, "get audio aac params err\n");
		player->channels = audio_params.nChannels;
		player->bit_rate = audio_params.nBitRate;
		player->sample_rate = audio_params.nSampleRate;
		player->bit_per_sample = 16;
	} else if (player->audio_codingtype == OMX_AUDIO_CodingPCM) {
		OMX_AUDIO_PARAM_PCMMODETYPE audio_params;
		ret = OMX_GetParameter(player->demuxer,
				OMX_IndexParamAudioPcm, &audio_params);
		oscl_param_check(ret == OMX_ErrorNone, -1, "get audio aac params err\n");
		player->channels = audio_params.nChannels;
		player->sample_rate = audio_params.nSamplingRate;
		player->bit_rate = audio_params.nBitPerSample * player->channels *
			player->sample_rate;
		player->bit_per_sample = 16;
	} else {
		OSCL_LOGE("coding type %d not support yet", player->audio_codingtype);
		player->has_audio = OMX_FALSE;
	}

	OSCL_LOGI("channels %d, bit per sample %d, sample rate %d, bitrate %d\n",
		player->channels, player->bit_per_sample,
		player->sample_rate, player->bit_rate);

	/* get video outport definition */
	memset(&params, 0, sizeof(params));
	params.nPortIndex = 1;
	ret = OMX_GetParameter(player->demuxer, OMX_IndexParamPortDefinition, &params);
	oscl_param_check(ret == OMX_ErrorNone, -1, "get port definition err\n");
	player->video_codingtype = params.format.video.eCompressionFormat;
	player->frame_width = params.format.video.nFrameWidth;
	player->frame_height = params.format.video.nFrameHeight;
	player->frame_stride = (player->frame_width + 15) & 0xFFFFFFF0;
	/* add one more mblock to avoid hw bug */
	player->frame_sliceheight = (player->frame_height + 31) & 0xFFFFFFF0;
	OSCL_LOGI("video codingtype %d, width %d, height %d, stride %d, sliceheight %d\n",
		player->video_codingtype, player->frame_width, player->frame_height,
		player->frame_stride, player->frame_sliceheight);

	/* get duration */
	ret = OMX_GetParameter(player->demuxer,
		omx_index_media_duration, &player->duration);
	oscl_param_check(ret == OMX_ErrorNone, -1, "get duration err\n");
	OSCL_LOGI("media duration %ld\n", player->duration);

	if (!player->has_audio && !player->has_video)
		ret = -1;

	return ret;
}

int omxmp_send_msg(omx_mediaplayer_t *player,
	player_msg_type_t cmd, OMX_U32 param1, void *msg_data)
{
	int ret = 0;
	oscl_message_t message;

	message.comp = player;
	message.msg_type = cmd;
	message.para1 = param1;
	message.data = msg_data;
	ret = oscl_message_put(&player->msg_queue, &message);
	if (ret)
		return OMX_ErrorInsufficientResources;
	sem_post(&player->msg_sem);

	return ret;
}

void *omxmp_message_thread(void *param)
{
	oscl_message_t message;
	int ret;
	omx_mediaplayer_t *player = param;

	while (1) {
		if (player->status == OMXMP_STATE_END) {
			OSCL_LOGI("exit from msg thread!\n");
			break;
		}
		/* wait for a msg */
		sem_wait(&player->msg_sem);
		/* dequeue a msg */
		ret = oscl_message_get(&player->msg_queue, &message);
		if (ret) {
			OSCL_LOGW("get msg failed!\n");
			continue;
		}

		/* hanle the msg */
		switch (message.msg_type) {
		case OMXMP_MSG_COMPLETE:
			player->audio_eos = OMX_FALSE;
			player->video_eos = OMX_FALSE;
			if (player->callback && player->callback->on_completion)
				player->callback->on_completion(player);
			break;
		case OMXMP_MSG_PREPARED:
			if (player->callback && player->callback->on_prepared)
				player->callback->on_prepared(player);
			break;
		case OMXMP_MSG_SEEK_COMPLETE:
			if (player->seek_in_pause) {
				omxmp_pause(player);
				player->seek_in_pause = OMX_FALSE;
			}
			pthread_mutex_lock(&player->lock);
			player->is_seeking = OMX_FALSE;
			pthread_mutex_unlock(&player->lock);
			if (player->callback && player->callback->on_seek_complete)
				player->callback->on_seek_complete(player);
			break;
		case OMXMP_MSG_ERROR:
			pthread_mutex_lock(&player->lock);
			player->status = OMXMP_STATE_ERROR;
			pthread_mutex_unlock(&player->lock);
			if (player->callback && player->callback->on_error)
				player->callback->on_error(player, message.para1);
			break;
		case OMXMP_MSG_STREAM_EOS:
			omxmp_stream_eos(player);
			break;
		default:
			OSCL_LOGE("msg type %d not supported!\n", message.msg_type);
			break;
		}
	}

	OSCL_LOGD("omxmp message thread exit.");
	return NULL;
}


void *omxmp_create(mp_callback_ops_t *cb_ops)
{
	omx_mediaplayer_t *player = NULL;
	int ret = OMX_ErrorNone;
	pthread_attr_t msg_thread_attr;
	struct sched_param shed_param = {0};

	oscl_param_check(cb_ops != NULL, NULL, "call back ops null\n");

	player = oscl_zalloc(sizeof(*player));
	if (player == NULL) {
		OSCL_LOGE("alloc omx_mediaplayer_t error!\n");
		return NULL;
	}

	pthread_mutex_init(&player->lock, NULL);
	pthread_mutex_lock(&player->lock);

	player->callback = cb_ops;
	player->play_rate = 1; /* default rate is normal */

	/* get handle of all component */
	ret = OMX_GetHandle((void **)&player->demuxer,
		"OMX.LB.SOURCE.DEMUXER", player, &omxmp_component_callbacks);
	oscl_param_check_exit((player->demuxer != NULL && ret == OMX_ErrorNone),
		ret, "get demuxer handle error!\n");
	ret = OMX_GetHandle((void **)&player->audiodec,
		"OMX.LB.SOURCE.ADEC", player, &omxmp_component_callbacks);
	oscl_param_check_exit((player->audiodec != NULL && ret == OMX_ErrorNone),
		ret, "get audio decoder handle error!\n");
	ret = OMX_GetHandle((void **)&player->videodec,
		"OMX.LB.VIDEO.H264DEC", player, &omxmp_component_callbacks);
	oscl_param_check_exit((player->videodec != NULL && ret == OMX_ErrorNone),
		ret, "get h264 decoder handle error!\n");
	ret = OMX_GetHandle((void **)&player->arender,
		"OMX.LB.SINK.ARENDER", player, &omxmp_component_callbacks);
	oscl_param_check_exit((player->arender != NULL && ret == OMX_ErrorNone),
		ret, "get arender handle error!\n");
	ret = OMX_GetHandle((void **)&player->vrender,
		"OMX.LB.SINK.VRENDER", player, &omxmp_component_callbacks);
	oscl_param_check_exit((player->vrender != NULL && ret == OMX_ErrorNone),
		ret, "get vrender handle error!\n");
	ret = OMX_GetHandle((void **)&player->rotate,
		"OMX.LB.VIDEO.ROT", player, &omxmp_component_callbacks);
	oscl_param_check_exit((player->rotate != NULL && ret == OMX_ErrorNone),
		ret, "get roate cmp handle error!\n");
	ret = OMX_GetHandle((void **)&player->clocksrc,
		"OMX.LB.SOURCE.CLOCK", player, &omxmp_component_callbacks);
	oscl_param_check_exit((player->clocksrc != NULL && ret == OMX_ErrorNone),
		ret, "get clock handle error!\n");

	sem_init(&player->sem_event, 0, 0);
	sem_init(&player->sem_eos, 0, 0);

	/* set the default buffer size & count */
	player->astream_buf_size    = DEFAULT_ASTREAM_BUFSIZE;
	player->astream_buf_cnt     = DEFAULT_ASTREAM_BUFCNT;
	player->araw_buf_size       = DEFAULT_ARAW_BUFSIZE;
	player->araw_buf_cnt        = DEFAULT_ARAW_BUFCNT;
	player->vstream_buf_size    = DEFAULT_VSTREAM_BUFSIZE;
	player->vstream_buf_cnt     = DEFAULT_VSTREAM_BUFCNT;
	player->vraw_buf_cnt        = DEFAULT_VRAW_BUFCNT;
	player->rotate_outbuf_cnt   = DEFAULT_ROTATE_BUFCNT;

	/* alloc buffer header for output port */
	player->outbuf_audio_dec = oscl_zalloc(player->araw_buf_cnt *
					sizeof(OMX_BUFFERHEADERTYPE *));
	oscl_param_check_exit(player->outbuf_audio_dec != NULL,
		ret, "alloc outbuf header error!\n");
	player->outbuf_video_dec = oscl_zalloc(player->vraw_buf_cnt *
					sizeof(OMX_BUFFERHEADERTYPE *));
	oscl_param_check_exit(player->outbuf_video_dec != NULL,
		ret, "alloc outbuf header error!\n");
	player->outbuf_demux_audio = oscl_zalloc(player->astream_buf_cnt *
					sizeof(OMX_BUFFERHEADERTYPE *));
	oscl_param_check_exit(player->outbuf_demux_audio != NULL,
		ret, "alloc outbuf header error!\n");
	player->outbuf_demux_video = oscl_zalloc(player->vstream_buf_cnt *
					sizeof(OMX_BUFFERHEADERTYPE *));
	oscl_param_check_exit(player->outbuf_demux_video != NULL,
		ret, "alloc outbuf header error!\n");
	player->outbuf_rotate = oscl_zalloc(player->rotate_outbuf_cnt *
					sizeof(OMX_BUFFERHEADERTYPE *));
	oscl_param_check_exit(player->outbuf_rotate != NULL,
		ret, "alloc outbuf header error!\n");

	/* config clock component */
	ret = config_clock_cmp(player);
	oscl_param_check_exit(ret == 0, -1, "config clock cmp err!\n");

	/* create the msg thread */
	oscl_queue_init(&player->msg_queue);
	sem_init(&player->msg_sem, 0, 0);
	pthread_attr_init(&msg_thread_attr);
	pthread_attr_setstacksize(&msg_thread_attr, 0x2000);
	shed_param.sched_priority = 15;
	pthread_attr_setschedparam(&msg_thread_attr, &shed_param);
	player->msg_handler_thread_id = pthread_create(
			&player->msg_handler_thread, &msg_thread_attr,
			omxmp_message_thread, player);

	/* change state to idle */
	player->status = OMXMP_STATE_IDLE;
	pthread_mutex_unlock(&player->lock);

EXIT:
	if (ret != OMX_ErrorNone) {
		pthread_mutex_unlock(&player->lock);
		omxmp_release(player);
		player = NULL;
	}
	return player;
}
RTM_EXPORT(omxmp_create);

void omxmp_release(void *handle)
{
	omx_mediaplayer_t *player = NULL;

	if (handle == NULL) {
		OSCL_LOGE("handle null\n");
		return;
	}

	player = (omx_mediaplayer_t *)handle;

	if (player->status == OMXMP_STATE_PREPARED ||
		player->status == OMXMP_STATE_PAUSED ||
		player->status == OMXMP_STATE_STARTED ||
		player->status == OMXMP_STATE_COMPLETED) {
		OSCL_LOGW("call at a running state, so stop first\n");
		omxmp_stop(player);
	}

	pthread_mutex_lock(&player->lock);
	/* quit the msg thread */
	player->status = OMXMP_STATE_END;
	if (player->msg_handler_thread_id == 0) {
		oscl_queue_flush(&player->msg_queue);
		sem_post(&player->msg_sem);
		pthread_join(player->msg_handler_thread, NULL);
		player->msg_handler_thread_id = -1;
	}
	/* free all the comnent handle */
	OMX_FreeHandle(player->demuxer);
	player->demuxer = NULL;
	OMX_FreeHandle(player->audiodec);
	player->audiodec = NULL;
	OMX_FreeHandle(player->videodec);
	player->videodec = NULL;
	OMX_FreeHandle(player->arender);
	player->arender = NULL;
	OMX_FreeHandle(player->vrender);
	player->vrender = NULL;
	OMX_FreeHandle(player->rotate);
	player->rotate = NULL;
	OMX_FreeHandle(player->clocksrc);
	player->clocksrc = NULL;

	/* destroy sem */
	sem_destroy(&player->sem_event);
	sem_destroy(&player->sem_eos);
	sem_destroy(&player->msg_sem);

	/* free the url */
	if (player->data_url) {
		oscl_free(player->data_url);
		player->data_url = NULL;
	}

	/* free all out buf header */
	if (player->outbuf_audio_dec) {
		oscl_free(player->outbuf_audio_dec);
		player->outbuf_audio_dec = NULL;
	}
	if (player->outbuf_video_dec) {
		oscl_free(player->outbuf_video_dec);
		player->outbuf_video_dec = NULL;
	}
	if (player->outbuf_demux_audio) {
		oscl_free(player->outbuf_demux_audio);
		player->outbuf_demux_audio = NULL;
	}
	if (player->outbuf_demux_video) {
		oscl_free(player->outbuf_demux_video);
		player->outbuf_demux_video = NULL;
	}
	if (player->outbuf_rotate) {
		oscl_free(player->outbuf_rotate);
		player->outbuf_rotate = NULL;
	}

	oscl_queue_deinit(&player->msg_queue);

	pthread_mutex_unlock(&player->lock);
	pthread_mutex_destroy(&player->lock);
	/* free the player */
	oscl_free(player);

	/* for memory observation */
	/* osal_dump(); */
	OSCL_LOGI("====== player release =======\n");
}
RTM_EXPORT(omxmp_release);

int omxmp_set_data_source(void *handle, const char *url)
{
	int ret = OMX_ErrorNone;
	omx_mediaplayer_t *player = NULL;

	oscl_param_check(handle != NULL, -1, "handle null");

	player = (omx_mediaplayer_t *)handle;
	if (player->status != OMXMP_STATE_IDLE) {
		OSCL_LOGE("error state %d\n", player->status);
		return -1;
	}

	pthread_mutex_lock(&player->lock);
	if (player->data_url) {
		oscl_free(player->data_url);
		player->data_url = NULL;
	}
	player->data_url = oscl_strdup(url);
	ret = OMX_SetParameter(player->demuxer,
		omx_index_vendor_input_filename, player->data_url);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("set input file error!\n");
		pthread_mutex_unlock(&player->lock);
		omxmp_send_msg(player, OMXMP_MSG_ERROR, OMXMP_ERR_FUNC, NULL);
		return ret;
	}
	player->status = OMXMP_STATE_INIT;
	pthread_mutex_unlock(&player->lock);
	OSCL_LOGI("set data url %s\n", player->data_url);
	return 0;
}
RTM_EXPORT(omxmp_set_data_source);

int omxmp_prepare(void *handle)
{
	int ret = 0;
	omx_mediaplayer_t *player = NULL;

	oscl_param_check(handle != NULL, -1, "handle null\n");

	player = (omx_mediaplayer_t *)handle;

	if (player->status != OMXMP_STATE_INIT &&
		player->status != OMXMP_STATE_STOP) {
		OSCL_LOGE("error state %d\n", player->status);
		return -1;
	}

	pthread_mutex_lock(&player->lock);
	/* config demuxer, alloc buffer */
	ret = config_demuxer_cmp(player);
	oscl_param_check_exit(ret == 0, -1, "config demuxer err!\n");

	/* set demxer to idle */
	ret = OMX_SendCommand(player->demuxer, OMX_CommandStateSet, OMX_StateIdle, NULL);
	oscl_param_check_exit(ret == 0, -1, "set demuxer to idle err!\n");
	sem_wait(&player->sem_event);

	/* set demuxer to execute to get media info */
	ret = OMX_SendCommand(player->demuxer,
		OMX_CommandStateSet, OMX_StateExecuting, NULL);
	oscl_param_check_exit(ret == 0, -1, "set demuxer to executing err!\n");
	sem_wait(&player->sem_event);

	/* get mediainfo from demuxer */
	ret = get_mediainfo(player);
	oscl_param_check_exit(ret == 0, -1, "get_mediainfo err!\n");

	/* clock component set to idle, tunnel port in clk cmp act as supllier,
	 * so set to idle prior to the client */
	ret = OMX_SendCommand(player->clocksrc, OMX_CommandStateSet, OMX_StateIdle, NULL);
	oscl_param_check_exit(ret == 0, -1, "set clock to idle err!\n");

	/* config audio component & set to idle */
	if (player->has_audio) {
		ret = config_audiodec_cmp(player);
		oscl_param_check_exit(ret == 0, -1, "config audiodec err!\n");
		ret = config_arender_cmp(player);
		oscl_param_check_exit(ret == 0, -1, "config arender err!\n");
		ret = OMX_SendCommand(player->audiodec,
			OMX_CommandStateSet, OMX_StateIdle, NULL);
		oscl_param_check_exit(ret == 0, -1, "set audiodec to idle err!\n");
		ret = OMX_SendCommand(player->arender,
			OMX_CommandStateSet, OMX_StateIdle, NULL);
		oscl_param_check_exit(ret == 0, -1, "set arender to idle err!\n");

		sem_wait(&player->sem_event);
		sem_wait(&player->sem_event);
	}
	/* config video component & set to idle */
	if (player->has_video) {
		ret = config_videodec_cmp(player);
		oscl_param_check_exit(ret == 0, -1, "config videodec err!\n");
		if (player->rot_mode.nRotation) {
			ret = config_rotate_cmp(player);
			oscl_param_check_exit(ret == 0, -1, "config rotate err!\n");
		}
		ret = config_vrender_cmp(player);
		oscl_param_check_exit(ret == 0, -1, "config vrender err!\n");

		ret = OMX_SendCommand(player->videodec,
			OMX_CommandStateSet, OMX_StateIdle, NULL);
		oscl_param_check_exit(ret == 0, -1, "set videodec to idle err!\n");
		if (player->rot_mode.nRotation) {
			ret = OMX_SendCommand(player->rotate,
				OMX_CommandStateSet, OMX_StateIdle, NULL);
			oscl_param_check_exit(ret == 0, -1, "set rotate to idle err!\n");
			sem_wait(&player->sem_event);
		}
		ret = OMX_SendCommand(player->vrender,
			OMX_CommandStateSet, OMX_StateIdle, NULL);
		oscl_param_check_exit(ret == 0, -1, "set vrender to idle err!\n");
		sem_wait(&player->sem_event);
		sem_wait(&player->sem_event);
	}
	sem_wait(&player->sem_event);

	/* change state to prepared */
	player->status = OMXMP_STATE_PREPARED;
	pthread_mutex_unlock(&player->lock);

	ret = omxmp_send_msg(player, OMXMP_MSG_PREPARED, 0, NULL);
	return ret;
EXIT:
	player->status = OMXMP_STATE_ERROR;
	pthread_mutex_unlock(&player->lock);
	omxmp_send_msg(player, OMXMP_MSG_ERROR, OMXMP_ERR_FUNC, NULL);
	return ret;
}
RTM_EXPORT(omxmp_prepare);

int omxmp_start(void *handle)
{
	int ret = 0;
	omx_mediaplayer_t *player = NULL;
	OMX_TIME_CONFIG_CLOCKSTATETYPE      clk_state;
	OMX_TIME_CONFIG_ACTIVEREFCLOCKTYPE  refclk;

	oscl_param_check(handle != NULL, -1, "handle null\n");

	player = (omx_mediaplayer_t *)handle;

	pthread_mutex_lock(&player->lock);

	if (player->status != OMXMP_STATE_PREPARED &&
		player->status != OMXMP_STATE_PAUSED &&
		player->status != OMXMP_STATE_COMPLETED) {
		pthread_mutex_unlock(&player->lock);
		OSCL_LOGE("error state %d\n", player->status);
		return -1;
	}

	player->audio_eos = OMX_FALSE;
	player->video_eos = OMX_FALSE;
	player->is_seeking = OMX_FALSE;
	player->seek_pos  = 0;

	/* set ref clk */
	refclk.nSize = sizeof(refclk);
	if (player->has_audio)
		refclk.eClock = OMX_TIME_RefClockAudio;
	else
		refclk.eClock = OMX_TIME_RefClockNone;
	ret = OMX_SetConfig(player->clocksrc, OMX_IndexConfigTimeActiveRefClock, &refclk);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("set activ ref clock error!\n");
		return -1;
	}
	if (player->status == OMXMP_STATE_PREPARED ||
		player->status == OMXMP_STATE_COMPLETED ||
		player->status == OMXMP_STATE_PAUSED) {
		/* set clock to executing */
		ret = OMX_SendCommand(player->clocksrc,
				OMX_CommandStateSet, OMX_StateExecuting, NULL);
		oscl_param_check_exit(ret == 0, -1, "set clock to execute err!\n");
		sem_wait(&player->sem_event);
	}
	if (player->status == OMXMP_STATE_PREPARED ||
		player->status == OMXMP_STATE_COMPLETED) {
		/* set clock state to ClockStateWaitingForStartTime */
		clk_state.nSize = sizeof(clk_state);
		clk_state.nWaitMask = 0;
		if (player->has_audio)
			clk_state.nWaitMask |= OMX_CLOCKPORT0;
		if (player->has_video)
			clk_state.nWaitMask |= OMX_CLOCKPORT1;
		clk_state.eState = OMX_TIME_ClockStateWaitingForStartTime;
		ret = OMX_SetConfig(player->clocksrc,
			OMX_IndexConfigTimeClockState, &clk_state);
		oscl_param_check_exit(ret == 0, -1, "set clock to state err!\n");
	}

	/* set component to executing */
	if (player->has_audio) {
		/* queue all the outport buffer */
		if (player->status != OMXMP_STATE_PAUSED) {
			ret = queue_outport_buffer(player->audiodec,
				player->outbuf_audio_dec, player->araw_buf_cnt);
			oscl_param_check(ret == 0, -1, "queue audio port buffer err\n");
		}
		ret = OMX_SendCommand(player->audiodec,
			OMX_CommandStateSet, OMX_StateExecuting, NULL);
		oscl_param_check_exit(ret == 0, -1, "set audiodec to execute err!\n");
		ret = OMX_SendCommand(player->arender,
			OMX_CommandStateSet, OMX_StateExecuting, NULL);
		oscl_param_check_exit(ret == 0, -1, "set arender to execute err!\n");
		sem_wait(&player->sem_event);
		sem_wait(&player->sem_event);
	}
	if (player->has_video) {
		/* queue all the outport buffer */
		if (player->status != OMXMP_STATE_PAUSED) {
			ret = queue_outport_buffer(player->videodec,
				player->outbuf_video_dec, player->vraw_buf_cnt);
			oscl_param_check(ret == 0, -1, "queue video port buffer err\n");
			if (player->rot_mode.nRotation) {
				ret = queue_outport_buffer(player->rotate,
					player->outbuf_rotate, player->rotate_outbuf_cnt);
				oscl_param_check(ret == 0, -1,
					"queue rotate outport buffer err\n");
			}
		}
		ret = OMX_SendCommand(player->videodec,
			OMX_CommandStateSet, OMX_StateExecuting, NULL);
		oscl_param_check_exit(ret == 0, -1, "set videodec to execute err!\n");
		if (player->rot_mode.nRotation) {
			ret = OMX_SendCommand(player->rotate,
				OMX_CommandStateSet, OMX_StateExecuting, NULL);
			oscl_param_check_exit(ret == 0, -1,
				"set rotate to execute err!\n");
			sem_wait(&player->sem_event);
		}
		ret = OMX_SendCommand(player->vrender,
			OMX_CommandStateSet, OMX_StateExecuting, NULL);
		oscl_param_check_exit(ret == 0, -1, "set vrender to execute err!\n");
		sem_wait(&player->sem_event);
		sem_wait(&player->sem_event);
	}
	if (player->status == OMXMP_STATE_PAUSED ||
		player->status == OMXMP_STATE_COMPLETED){
		ret = OMX_SendCommand(player->demuxer,
			OMX_CommandStateSet, OMX_StateExecuting, NULL);
		oscl_param_check_exit(ret == 0, -1, "set demuxer to executing err!\n");
		sem_wait(&player->sem_event);
	}
	/* queue buffer in demuxer if just prepared */
	if (player->status == OMXMP_STATE_PREPARED ||
		player->status == OMXMP_STATE_COMPLETED) {
		ret = queue_outport_buffer(player->demuxer,
			player->outbuf_demux_audio, player->astream_buf_cnt);
		oscl_param_check_exit(ret == 0, -1, "queue audio buffer err!\n");
		queue_outport_buffer(player->demuxer,
			player->outbuf_demux_video, player->vstream_buf_cnt);
		oscl_param_check_exit(ret == 0, -1, "queue video buffer err!\n");
	}
	OSCL_LOGW("-------start end--------\n");
	player->status = OMXMP_STATE_STARTED;

	pthread_mutex_unlock(&player->lock);

	return ret;
EXIT:
	player->status = OMXMP_STATE_ERROR;
	pthread_mutex_unlock(&player->lock);
	omxmp_send_msg(player, OMXMP_MSG_ERROR, OMXMP_ERR_FUNC, NULL);
	return ret;
}
RTM_EXPORT(omxmp_start);

int omxmp_pause(void *handle)
{
	int ret = 0;
	omx_mediaplayer_t *player = NULL;

	oscl_param_check(handle != NULL, -1, "handle null\n");

	player = (omx_mediaplayer_t *)handle;

	if (player->audio_eos || player->video_eos) {
		OSCL_LOGE("have eos message, don't pause now!\n");
		return -1;
	}

	pthread_mutex_lock(&player->lock);

	if (player->status != OMXMP_STATE_STARTED) {
		pthread_mutex_unlock(&player->lock);
		OSCL_LOGE("error state %d\n", player->status);
		return -1;
	}

	/* set clock to pause */
	ret = OMX_SendCommand(player->clocksrc,
				OMX_CommandStateSet, OMX_StatePause, NULL);
	oscl_param_check_exit(ret == 0, -1, "set clk to pause err!\n");
	sem_wait(&player->sem_event);

	/* set other component to pause */
	ret = OMX_SendCommand(player->demuxer, OMX_CommandStateSet, OMX_StatePause, NULL);
	oscl_param_check_exit(ret == 0, -1, "set demuxer to pause err!\n");
	if (player->has_audio) {
		ret = OMX_SendCommand(player->audiodec,
			OMX_CommandStateSet, OMX_StatePause, NULL);
		oscl_param_check_exit(ret == 0, -1, "set audiodec to pause err!\n");
		ret = OMX_SendCommand(player->arender,
			OMX_CommandStateSet, OMX_StatePause, NULL);
		oscl_param_check_exit(ret == 0, -1, "set arender to pause err!\n");
		sem_wait(&player->sem_event);
		sem_wait(&player->sem_event);
	}
	if (player->has_video) {
		ret = OMX_SendCommand(player->videodec,
			OMX_CommandStateSet, OMX_StatePause, NULL);
		oscl_param_check_exit(ret == 0, -1, "set videodec to pause err!\n");
		if (player->rot_mode.nRotation) {
			ret = OMX_SendCommand(player->rotate,
				OMX_CommandStateSet, OMX_StatePause, NULL);
			oscl_param_check_exit(ret == 0, -1, "set rotate to pause err!\n");
			sem_wait(&player->sem_event);
		}
		ret = OMX_SendCommand(player->vrender,
			OMX_CommandStateSet, OMX_StatePause, NULL);
		oscl_param_check_exit(ret == 0, -1, "set vrender to pause err!\n");

		sem_wait(&player->sem_event);
		sem_wait(&player->sem_event);
	}
	sem_wait(&player->sem_event);
	player->status = OMXMP_STATE_PAUSED;
	OSCL_LOGW("------------pause end-------\n");
	pthread_mutex_unlock(&player->lock);

	return ret;
EXIT:
	player->status = OMXMP_STATE_ERROR;
	pthread_mutex_unlock(&player->lock);
	omxmp_send_msg(player, OMXMP_MSG_ERROR, OMXMP_ERR_FUNC, NULL);
	return ret;
}
RTM_EXPORT(omxmp_pause);

int omxmp_stop(void *handle)
{
	int ret = 0;
	omx_mediaplayer_t *player = NULL;

	oscl_param_check(handle != NULL, -1, "handle null\n");

	player = (omx_mediaplayer_t *)handle;

	if (player->status != OMXMP_STATE_STARTED &&
		player->status != OMXMP_STATE_PREPARED &&
		player->status != OMXMP_STATE_PAUSED &&
		player->status != OMXMP_STATE_COMPLETED &&
		player->status != OMXMP_STATE_ERROR) {
		OSCL_LOGW("error state %d\n", player->status);
		return -1;
	}

	pthread_mutex_lock(&player->lock);
	if (player->status == OMXMP_STATE_COMPLETED) {
		OSCL_LOGW("already idle, goto set loaded\n");
		goto set_to_loaded;
	}
	/* set other component to idle */
	if (player->has_audio) {
		ret = OMX_SendCommand(player->arender,
			OMX_CommandStateSet, OMX_StateIdle, NULL);
		oscl_param_check_exit(ret == 0, -1, "set arender to idle err!\n");
		sem_wait(&player->sem_event);
		ret = OMX_SendCommand(player->audiodec,
			OMX_CommandStateSet, OMX_StateIdle, NULL);
		oscl_param_check_exit(ret == 0, -1, "set audiodec to idle err!\n");
		sem_wait(&player->sem_event);
	}
	if (player->has_video) {
		ret = OMX_SendCommand(player->vrender,
			OMX_CommandStateSet, OMX_StateIdle, NULL);
		oscl_param_check_exit(ret == 0, -1, "set vrender to idle err!\n");
		sem_wait(&player->sem_event);
		if (player->rot_mode.nRotation) {
			ret = OMX_SendCommand(player->rotate,
				OMX_CommandStateSet, OMX_StateIdle, NULL);
			oscl_param_check_exit(ret == 0, -1, "set rotate to idle err!\n");
			sem_wait(&player->sem_event);
		}
		ret = OMX_SendCommand(player->videodec,
			OMX_CommandStateSet, OMX_StateIdle, NULL);
		oscl_param_check_exit(ret == 0, -1, "set videodec to idle err!\n");
		sem_wait(&player->sem_event);
	}
	ret = OMX_SendCommand(player->demuxer, OMX_CommandStateSet, OMX_StateIdle, NULL);
	oscl_param_check_exit(ret == 0, -1, "set demuxer to idle err!\n");
	/* set clk to idle, as supplier, set it to idle before
	 * the other client */
	ret = OMX_SendCommand(player->clocksrc,
			OMX_CommandStateSet, OMX_StateIdle, NULL);
	oscl_param_check_exit(ret == 0, -1, "set clock to idle err!\n");
	sem_wait(&player->sem_event);
	sem_wait(&player->sem_event);

set_to_loaded:
	deinit_outport_buffer(player->demuxer, player->outbuf_demux_audio,
			player->astream_buf_cnt, AUDIO_PORT);
	deinit_outport_buffer(player->demuxer, player->outbuf_demux_video,
		player->vstream_buf_cnt, VIDEO_PORT);
	ret = OMX_SendCommand(player->demuxer,
		OMX_CommandStateSet, OMX_StateLoaded, NULL);
	oscl_param_check_exit(ret == 0, -1, "set demuxer to loaded err!\n");
	sem_wait(&player->sem_event);
	/* set clk to loaded */
	ret = OMX_SendCommand(player->clocksrc,
			OMX_CommandStateSet, OMX_StateLoaded, NULL);
	oscl_param_check_exit(ret == 0, -1, "set clock to idle err!\n");
	sem_wait(&player->sem_event);
	/* set audio component to loaded & free the buffer */
	if (player->has_audio) {
		deinit_outport_buffer(player->audiodec, player->outbuf_audio_dec,
			player->araw_buf_cnt, OMX_DEFAULT_OUTPUT_PORT);
		ret = OMX_SendCommand(player->audiodec,
			OMX_CommandStateSet, OMX_StateLoaded, NULL);
		oscl_param_check_exit(ret == 0, -1, "set audiodec to loaded err!\n");
		sem_wait(&player->sem_event);
		ret = OMX_SendCommand(player->arender,
			OMX_CommandStateSet, OMX_StateLoaded, NULL);
		oscl_param_check_exit(ret == 0, -1, "set arender to loaded err!\n");
		sem_wait(&player->sem_event);

	}
	/* set video component to loaded & free the buffer */
	if (player->has_video) {
		deinit_outport_buffer(player->videodec, player->outbuf_video_dec,
			player->vraw_buf_cnt, OMX_DEFAULT_OUTPUT_PORT);
		ret = OMX_SendCommand(player->videodec,
			OMX_CommandStateSet, OMX_StateLoaded, NULL);
		oscl_param_check_exit(ret == 0, -1, "set videodec to loaded err!\n");
		sem_wait(&player->sem_event);
		if (player->rot_mode.nRotation) {
			deinit_outport_buffer(player->rotate, player->outbuf_rotate,
				player->rotate_outbuf_cnt, OMX_DEFAULT_OUTPUT_PORT);
			ret = OMX_SendCommand(player->rotate,
				OMX_CommandStateSet, OMX_StateLoaded, NULL);
			oscl_param_check_exit(ret == 0, -1,
				"set rotate to loaded err!\n");
			sem_wait(&player->sem_event);
		}
		ret = OMX_SendCommand(player->vrender,
			OMX_CommandStateSet, OMX_StateLoaded, NULL);
		oscl_param_check_exit(ret == 0, -1, "set vrender to loaded err!\n");
		sem_wait(&player->sem_event);

	}

	player->status = OMXMP_STATE_STOP;

	pthread_mutex_unlock(&player->lock);
#if 0
	rt_uint32_t mem_total, mem_used, mem_max_used;
	rt_memory_info(&mem_total, &mem_used, &mem_max_used);
	LOG_W("total memory %d, use %d, max_used %d",
				mem_total, mem_used, mem_max_used);
#endif
	return ret;
EXIT:
	player->status = OMXMP_STATE_ERROR;
	pthread_mutex_unlock(&player->lock);
	omxmp_send_msg(player, OMXMP_MSG_ERROR, OMXMP_ERR_FUNC, NULL);
	return ret;
}
RTM_EXPORT(omxmp_stop);

int omxmp_reset(void *handle)
{
	int ret = 0;
	omx_mediaplayer_t *player = NULL;

	oscl_param_check(handle != NULL, -1, "handle null\n");

	player = (omx_mediaplayer_t *)handle;

	if (player->status == OMXMP_STATE_STARTED ||
		player->status == OMXMP_STATE_PAUSED ||
		player->status == OMXMP_STATE_PREPARED ||
		player->status == OMXMP_STATE_COMPLETED ||
		player->status == OMXMP_STATE_ERROR) {
		OSCL_LOGI("call reset not in stop state, stop first\n");
		ret = omxmp_stop(handle);
	}

	pthread_mutex_lock(&player->lock);
	if (player->data_url) {
		oscl_free(player->data_url);
		player->data_url = NULL;
	}
	player->has_audio = OMX_FALSE;
	player->has_video = OMX_FALSE;
	player->isloop = OMX_FALSE;
	player->status = OMXMP_STATE_IDLE;
	pthread_mutex_unlock(&player->lock);

	return ret;
}
RTM_EXPORT(omxmp_reset);

int omxmp_seek_to(void *handle, long msec)
{
	int ret = 0;
	omx_mediaplayer_t *player = NULL;
	OMX_TIME_CONFIG_TIMESTAMPTYPE       timestamp;
	OMX_TIME_CONFIG_CLOCKSTATETYPE      clk_state;

	oscl_param_check(handle != NULL, -1, "handle null\n");

	player = (omx_mediaplayer_t *)handle;
	if (msec < 0 || msec > player->duration) {
		OSCL_LOGE("invalid seek position %ld\n", msec);
		return -1;
	}
	if (player->is_seeking) {
		OSCL_LOGE("seeking is not complete!\n");
		return -1;
	}
	if (player->audio_eos || player->video_eos) {
		OSCL_LOGE("have eos message, don't seek now!\n");
		return -1;
	}

	pthread_mutex_lock(&player->lock);

	if (player->status != OMXMP_STATE_PAUSED &&
		player->status != OMXMP_STATE_STARTED) {
		pthread_mutex_unlock(&player->lock);
		OSCL_LOGW("error state %d\n", player->status);
		return -1;
	}

	if (player->status == OMXMP_STATE_PAUSED) {
		omxmp_start(player);
		player->seek_in_pause = OMX_TRUE;
		OSCL_LOGD("seek in pause state\n");
	}

	player->is_seeking = OMX_TRUE;
	player->seek_pos = msec;

	timestamp.nSize = sizeof(timestamp);
	timestamp.nPortIndex = 0;
	timestamp.nTimestamp = msec;

	/* pause the demuxer component */
	ret = OMX_SendCommand(player->demuxer, OMX_CommandStateSet, OMX_StatePause, NULL);
	oscl_param_check_exit(ret == 0, -1, "set demuxer to pause err!\n");
	sem_wait(&player->sem_event);

	/* pause the audio component */
	if (player->has_audio) {
		ret = OMX_SendCommand(player->arender,
			OMX_CommandStateSet, OMX_StatePause, NULL);
		oscl_param_check_exit(ret == 0, -1, "pause arender err!\n");
		sem_wait(&player->sem_event);
	}

	/* stop the clock */
	clk_state.nSize = sizeof(clk_state);
	clk_state.eState = OMX_TIME_ClockStateStopped;
	ret = OMX_SetConfig(player->clocksrc,
			OMX_IndexConfigTimeClockState, &clk_state);
	oscl_param_check_exit(ret == 0, -1, "set clock stop err!\n");

	/* set clock to wait for start */
	clk_state.nSize = sizeof(clk_state);
	clk_state.nWaitMask = 0;
	if (player->has_audio)
		clk_state.nWaitMask |= OMX_CLOCKPORT0;
	if (player->has_video)
		clk_state.nWaitMask |= OMX_CLOCKPORT1;
	clk_state.eState = OMX_TIME_ClockStateWaitingForStartTime;
	ret = OMX_SetConfig(player->clocksrc,
			OMX_IndexConfigTimeClockState, &clk_state);
	oscl_param_check_exit(ret == 0, -1, "set clock to wait for start err!\n");

	/* set the seek position to demuxer */
	ret = OMX_SetParameter(player->demuxer, OMX_IndexConfigTimePosition, &timestamp);
	oscl_param_check_exit(ret == 0, -1, "set demuxer to seek pos err!\n");

	/* flush ports */
	if (player->has_audio) {
		ret = OMX_SendCommand(player->audiodec, OMX_CommandFlush, 0, NULL);
		oscl_param_check_exit(ret == 0, -1, "flush audiodec inport err!\n");
		sem_wait(&player->sem_event);
	}
	if (player->has_video) {
		ret = OMX_SendCommand(player->videodec, OMX_CommandFlush, 0, NULL);
		oscl_param_check_exit(ret == 0, -1, "flush videodec  inport err!\n");
		sem_wait(&player->sem_event);
	}
	ret = OMX_SendCommand(player->demuxer, OMX_CommandFlush, -1, NULL);
	oscl_param_check_exit(ret == 0, -1, "flush demuxer  inport err!\n");
	sem_wait(&player->sem_event);

	/* start the demuxer component & queue buffer */
	ret = OMX_SendCommand(player->demuxer,
		OMX_CommandStateSet, OMX_StateExecuting, NULL);
	oscl_param_check_exit(ret == 0, -1, "set demuxer to pause err!\n");
	sem_wait(&player->sem_event);

	if (player->has_audio) {
		ret = OMX_SendCommand(player->arender,
			OMX_CommandStateSet, OMX_StateExecuting, NULL);
		oscl_param_check_exit(ret == 0, -1, "start arender err!\n");
		sem_wait(&player->sem_event);
	}
	ret = queue_outport_buffer(player->demuxer,
		player->outbuf_demux_audio, player->astream_buf_cnt);
	oscl_param_check_exit(ret == 0, -1, "queue audio buffer err!\n");
	ret = queue_outport_buffer(player->demuxer,
		player->outbuf_demux_video, player->vstream_buf_cnt);
	oscl_param_check_exit(ret == 0, -1, "queue video buffer err!\n");

	pthread_mutex_unlock(&player->lock);

	return ret;
EXIT:
	player->status = OMXMP_STATE_ERROR;
	pthread_mutex_unlock(&player->lock);
	omxmp_send_msg(player, OMXMP_MSG_ERROR, OMXMP_ERR_FUNC, NULL);
	return ret;
}
RTM_EXPORT(omxmp_seek_to);

int omxmp_get_state(void *handle)
{
	omx_mediaplayer_t *player = NULL;

	oscl_param_check(handle != NULL, -1, "handle null\n");

	player = (omx_mediaplayer_t *)handle;

	return player->status;
}
RTM_EXPORT(omxmp_get_state);

int omxmp_is_playing(void *handle)
{
	omx_mediaplayer_t *player = NULL;

	oscl_param_check(handle != NULL, -1, "handle null\n");

	player = (omx_mediaplayer_t *)handle;

	if (player->status == OMXMP_STATE_STARTED)
		return 1;
	else
		return 0;
}
RTM_EXPORT(omxmp_is_playing);

long omxmp_get_current_position(void *handle)
{
	int ret = 0;
	omx_mediaplayer_t *player = NULL;
	OMX_TIME_CONFIG_TIMESTAMPTYPE timestamp;

	oscl_param_check(handle != NULL, -1, "handle null\n");

	player = (omx_mediaplayer_t *)handle;

	if (player->status != OMXMP_STATE_STARTED &&
		player->status != OMXMP_STATE_PAUSED) {
		OSCL_LOGE("error state %d\n", player->status);
		return -1;
	}

	if (player->is_seeking)
		return player->seek_pos;

	timestamp.nPortIndex = CLOCK_PORT_AUDIO;
	timestamp.nSize = sizeof(timestamp);
	ret = OMX_GetConfig(player->clocksrc,
		OMX_IndexConfigTimeCurrentMediaTime, &timestamp);
	oscl_param_check(ret == 0, -1, "get cur mediatime err!\n");

	return timestamp.nTimestamp / 1000;
}
RTM_EXPORT(omxmp_get_current_position);

long omxmp_get_duration(void *handle)
{
	omx_mediaplayer_t *player = NULL;
	oscl_param_check(handle != NULL, -1, "handle null\n");
	player = (omx_mediaplayer_t *)handle;
	return player->duration;
}
RTM_EXPORT(omxmp_get_duration);

void omxmp_set_loop(void *handle, int loop)
{
	omx_mediaplayer_t *player = NULL;

	if (handle == NULL) {
		OSCL_LOGE("handle null\n");
		return;
	}

	player = (omx_mediaplayer_t *)handle;

	pthread_mutex_lock(&player->lock);
	if (loop)
		player->isloop = OMX_TRUE;
	else
		player->isloop = OMX_FALSE;
	pthread_mutex_unlock(&player->lock);
}
RTM_EXPORT(omxmp_set_loop);

int omxmp_set_playback_rate(void *handle, int rate)
{
	int ret = 0;
	omx_mediaplayer_t *player = NULL;
	OMX_TIME_CONFIG_SCALETYPE config_scale;

	oscl_param_check(handle != NULL, -1, "handle null\n");

	player = (omx_mediaplayer_t *)handle;

	pthread_mutex_lock(&player->lock);
	player->play_rate = rate;
	config_scale.nSize = sizeof(config_scale);
	config_scale.xScale = rate << 16;
	ret = OMX_SetConfig(player->clocksrc, OMX_IndexConfigTimeScale, &config_scale);
	oscl_param_check_exit(ret == 0, -1, "set clock scale err!\n");

	pthread_mutex_unlock(&player->lock);
	return ret;
EXIT:
	pthread_mutex_unlock(&player->lock);
	return ret;
}
RTM_EXPORT(omxmp_set_playback_rate);

void omxmp_set_playback_volume(void *handle, int rate)
{

}
RTM_EXPORT(omxmp_set_playback_volume);

int omxmp_set_scaling_mode(void *handle, int mode)
{
	int ret = 0;
	omx_mediaplayer_t *player = NULL;

	oscl_param_check(handle != NULL, -1, "handle null\n");

	player = (omx_mediaplayer_t *)handle;

	pthread_mutex_lock(&player->lock);
	switch (mode) {
	case OMXMP_WINDOW_ORIGINAL:
		player->disp_para.mode = VDISP_WINDOW_ORIGINAL;
		break;
	case OMXMP_WINDOW_FULL_SCREEN_VIDEO_RATIO:
		player->disp_para.mode = VDISP_WINDOW_FULL_SCREEN_VIDEO_RATIO;
		break;
	case OMXMP_WINDOW_FULL_SCREEN_SCREEN_RATIO:
		player->disp_para.mode = VDISP_WINDOW_FULL_SCREEN_SCREEN_RATIO;
		break;
	case OMXMP_WINDOW_4R3MODE:
		player->disp_para.mode = VDISP_WINDOW_4R3MODE;
		break;
	case OMXMP_WINDOW_16R9MODE:
		player->disp_para.mode = VDISP_WINDOW_16R9MODE;
		break;
	case OMXMP_WINDOW_USERDEF:
		player->disp_para.mode = VDISP_WINDOW_USERDEF;
		break;
	case OMXMP_WINDOW_CUTEDGE:
		player->disp_para.mode = VDISP_WINDOW_CUTEDGE;
	default:
		OSCL_LOGE("error scaling mode %d\n", mode);
		player->disp_para.mode = VDISP_WINDOW_ORIGINAL;
		break;
	}

	/* setup display mode */
	ret = OMX_SetParameter(player->vrender, omx_index_vrender_mode,
			&player->disp_para);
	oscl_param_check_exit(ret == OMX_ErrorNone, -1, "set display mode err\n");

	pthread_mutex_unlock(&player->lock);
	return ret;
EXIT:
	player->status = OMXMP_STATE_ERROR;
	pthread_mutex_unlock(&player->lock);
	return ret;
}
RTM_EXPORT(omxmp_set_scaling_mode);

int omxmp_set_window(void *handle, omxmp_win_t *win)
{
	int ret = 0;
	omx_mediaplayer_t *player = NULL;

	oscl_param_check(handle != NULL, -1, "handle null\n");

	player = (omx_mediaplayer_t *)handle;

	pthread_mutex_lock(&player->lock);
	player->disp_para.win_rect.left = win->left;
	player->disp_para.win_rect.top  = win->top;
	player->disp_para.win_rect.width = win->width;
	player->disp_para.win_rect.height = win->height;
	/* setup display window */
	ret = OMX_SetParameter(player->vrender, omx_index_vrender_mode,
			&player->disp_para);
	oscl_param_check_exit(ret == OMX_ErrorNone, -1, "set display window err\n");

	pthread_mutex_unlock(&player->lock);
	return ret;
EXIT:
	player->status = OMXMP_STATE_ERROR;
	pthread_mutex_unlock(&player->lock);
	return ret;
}
RTM_EXPORT(omxmp_set_window);

int omxmp_set_rotation(void *handle, int rot_mode)
{
	int ret = 0;
	omx_mediaplayer_t *player = NULL;

	oscl_param_check(handle != NULL, -1, "handle null\n");

	player = (omx_mediaplayer_t *)handle;

	pthread_mutex_lock(&player->lock);
	switch (rot_mode) {
	case OMXMP_ROTATE_NONE:
		player->rot_mode.nRotation = VDISP_ROTATE_NONE;
		break;
	case OMXMP_ROTATE_90:
		player->rot_mode.nRotation = VDISP_ROTATE_90;
		break;
	case OMXMP_ROTATE_180:
		player->rot_mode.nRotation = VDISP_ROTATE_180;
		break;
	case OMXMP_ROTATE_270:
		player->rot_mode.nRotation = VDISP_ROTATE_270;
		break;
	default:
		OSCL_LOGE("mode not supported, set to default\n");
		player->rot_mode.nRotation = VDISP_ROTATE_NONE;
		break;
	}
	pthread_mutex_unlock(&player->lock);
	return ret;
}
RTM_EXPORT(omxmp_set_rotation);

/* 0: layer top, 1: layer bottom */
int omxmp_set_win_layer(void *handle, int layer)
{
	int ret = 0;
	omx_mediaplayer_t *player = NULL;

	oscl_param_check(handle != NULL, -1, "handle null\n");

	player = (omx_mediaplayer_t *)handle;

	pthread_mutex_lock(&player->lock);
	switch (layer) {
	case OMXMP_LAYER_TOP:
		ret = OMX_SetParameter(player->vrender,
			omx_index_vrender_win_top, NULL);
		break;
	case OMXMP_LAYER_BOTTOM:
		ret = OMX_SetParameter(player->vrender,
			omx_index_vrender_win_bottom, NULL);
		break;
	default:
		OSCL_LOGW("error layer %d, set to bottom as default!\n", layer);
		ret = OMX_SetParameter(player->vrender,
			omx_index_vrender_win_bottom, NULL);
		break;
	}

	pthread_mutex_unlock(&player->lock);
	return ret;
}
RTM_EXPORT(omxmp_set_win_layer);

