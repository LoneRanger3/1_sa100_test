/*
 * aenc_component.c - demo code for aenc component.
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

#include <oscl.h>
#include <base_component.h>
#include "slib.h"
#include "aenc_common.h"
#include "arec_component.h"
#include "avformat.h"

/* Place global headers in extradata instead of every keyframe */
#define AV_CODEC_FLAG_GLOBAL_HEADER   (1 << 22)

void aenc_buffer_handle(OMX_HANDLETYPE hComp,
	OMX_BUFFERHEADERTYPE *inbuf, OMX_BUFFERHEADERTYPE *outbuf)
{
	lb_omx_component_t *component = NULL;
	aenc_private_t *audio_private = NULL;

	component = get_lb_component(hComp);
	audio_private = component->component_private;

	aenc_frame(audio_private, inbuf, outbuf);
	OSCL_LOGD("aenc_frame buffer len %d", outbuf->nFilledLen);
}

/*
 * This is the central function for component processing. It
 * is executed in a separate thread, is synchronized with
 * semaphores at each port, those are released each time a new buffer
 * is available on the given port.
 */
void *aenc_buffer_manager(void *param)
{
	lb_omx_component_t *component;
	base_port_t *inport;
	base_port_t *outport;
	OMX_BUFFERHEADERTYPE *outbuffer = NULL;
	OMX_BUFFERHEADERTYPE *inbuffer = NULL;

	OSCL_TRACE(" %p\n", param);
	oscl_param_check((param != NULL), NULL, NULL);
	component = get_lb_component(param);
	inport = &component->port[BASE_INPUT_PORT];
	outport = &component->port[BASE_OUTPUT_PORT];
	component->dbg_flag = set_debug_state(component->dbg_flag,
			DEBUG_BUF_MGNT_SHT, DEBUG_THREAD_START);
	OSCL_LOGI("%s buffer_manager:%s\n", component->name, rt_thread_self()->name);

	/* checks if the component is in a state able to receive buffers */
	while (component->state == OMX_StateIdle
		|| component->state == OMX_StateExecuting
		|| component->state == OMX_StatePause) {
		/* Wait till the ports are being flushed */
		component->dbg_wdog = 0;
		pthread_mutex_lock(&component->flush_mutex);
		while (inport->is_flushed || outport->is_flushed) {
			pthread_mutex_unlock(&component->flush_mutex);
			OSCL_LOGI("component:%x,flush buffers:%d %d\n",
				component, inport->is_flushed, outport->is_flushed);
			OSCL_LOGI("inport:%d, buffer:%p\n",
				inport->port_param.nPortIndex, inbuffer);
			OSCL_LOGI("outport:%d, buffer:%p\n",
				outport->port_param.nPortIndex, outbuffer);

			if (outbuffer && outport->is_flushed) {
				outport->return_buffer(outport, outbuffer);
				outbuffer = NULL;
			}
			if (inbuffer && inport->is_flushed) {
				inport->return_buffer(inport, inbuffer);
				inbuffer = NULL;
			}

			sem_post(component->mgmt_flush_sem);
			sem_wait(component->flush_sem);
			pthread_mutex_lock(&component->flush_mutex);
		}
		pthread_mutex_unlock(&component->flush_mutex);

		if (component->state != OMX_StateExecuting) {
			OSCL_LOGD("==========%d\n", component->buf_mgnt_sem->sem->value);
			sem_wait(component->buf_mgnt_sem);
			continue;
		}

		if (inbuffer == NULL)
			inbuffer = oscl_queue_dequeue(&inport->buffer_queue);
		if (outbuffer == NULL)
			outbuffer = oscl_queue_dequeue(&outport->buffer_queue);
		if (outbuffer) {
			outbuffer->nFilledLen = 0;
			outbuffer->nFlags = 0;
		}

		if (component->state == OMX_StateExecuting) {
			if (component->buf_handle)
				((component->buf_handle))(component, inbuffer, outbuffer);
			else if (inbuffer)
				inbuffer->nFilledLen = 0;
		}


		base_check_eos(component, outport, outbuffer);
		if (inbuffer == NULL
			&& (outbuffer == NULL || outbuffer->nFilledLen == 0)) {
			OSCL_LOGD("waiting buffer:%x %x\n", inbuffer, outbuffer);
			OSCL_LOGD("==========%d\n", component->buf_mgnt_sem->sem->value);
			sem_wait(component->buf_mgnt_sem);
			while (sem_trywait(component->buf_mgnt_sem) == 0)
				continue;
			continue;
		}
		if (outbuffer) {
			if ((outbuffer->nFilledLen != 0)
				|| (outbuffer->nFlags & OMX_BUFFERFLAG_EOS)) {
				outport->return_buffer(outport, outbuffer);
				outbuffer = NULL;
			}
		}
		if (inbuffer) {
			if (inbuffer->nFilledLen == 0) {
				inport->return_buffer(inport, inbuffer);
				inbuffer = NULL;
			}
		}
	}
	OSCL_LOGW("exit from buffer_manager:%s\n", rt_thread_self()->name);
	OSCL_TRACE(" %p\n", param);
	component->dbg_flag = set_debug_state(component->dbg_flag,
			DEBUG_BUF_MGNT_SHT, DEBUG_THREAD_EXIT);
	pthread_exit(NULL);

	return NULL;
}

OMX_ERRORTYPE aenc_set_parameter(OMX_IN OMX_HANDLETYPE hComp,
	OMX_IN OMX_INDEXTYPE paramIndex,
	OMX_IN OMX_PTR paramData)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component = NULL;
	aenc_private_t *audio_private = NULL;
	OSCL_TRACE(" %p, %p , %d\n", hComp, paramData, paramIndex);

	oscl_param_check((hComp != NULL) && (paramData != NULL),
		OMX_ErrorBadParameter, NULL);
	component = get_lb_component(hComp);
	oscl_param_check((component != NULL), OMX_ErrorBadParameter, NULL);
	audio_private = component->component_private;

	switch (paramIndex) {
	case omx_index_vendor_input_filename:
		break;
	case OMX_IndexParamAudioPcm: {
		OMX_AUDIO_PARAM_PCMMODETYPE *audio_params;
		audio_params = (OMX_AUDIO_PARAM_PCMMODETYPE *)paramData;
		audio_private->paudio.para->channels = audio_params->nChannels;
		audio_private->paudio.para->bits_per_raw_sample =
			audio_params->nBitPerSample;
		audio_private->paudio.para->sample_rate = audio_params->nSamplingRate;
		audio_private->acode = AV_CODEC_TYPE_PCM_S16LE;
	}
	break;
	case OMX_IndexParamAudioAdpcm: {
		OMX_AUDIO_PARAM_ADPCMTYPE *audio_params;
		audio_params = (OMX_AUDIO_PARAM_ADPCMTYPE *)paramData;
		audio_private->paudio.para->channels = audio_params->nChannels;
		audio_private->paudio.para->bits_per_raw_sample =
			audio_params->nBitsPerSample;
		audio_private->paudio.para->sample_rate = audio_params->nSampleRate;
		audio_private->acode = AV_CODEC_TYPE_ADPCM_IMA_WAV;
	}
	break;
	case OMX_IndexParamAudioAac: {
		OMX_AUDIO_PARAM_AACPROFILETYPE *audio_params;
		audio_params = (OMX_AUDIO_PARAM_AACPROFILETYPE *)paramData;
		audio_private->paudio.para->channels = audio_params->nChannels;
		audio_private->paudio.para->bit_rate = audio_params->nBitRate;
		audio_private->paudio.para->sample_rate = audio_params->nSampleRate;
		if (audio_params->eAACStreamFormat == OMX_AUDIO_AACStreamFormatRAW)
			audio_private->paudio.para->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
		audio_private->acode = AV_CODEC_TYPE_AAC;
	}
	break;

	default:
		ret = base_set_parameter(hComp, paramIndex, paramData);
		break;
	}
	OSCL_LOGI("paramIndex:0x%08x,audio info: ch:%d,fs:%d,bitrate:%lld,code:0x%08x",
		paramIndex,
		audio_private->paudio.para->channels,
		audio_private->paudio.para->sample_rate,
		audio_private->paudio.para->bit_rate,
		audio_private->acode);

	OSCL_TRACE(" %d\n", ret);
	return ret;
}

OMX_ERRORTYPE aenc_set_state(OMX_HANDLETYPE hComp,
	OMX_U32 dest_state)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component;
	aenc_private_t *audio_private;
	OMX_STATETYPE pre_state;

	OSCL_TRACE(" %p, %x\n", hComp, dest_state);
	oscl_param_check(hComp != NULL, OMX_ErrorBadParameter, NULL);
	component = get_lb_component(hComp);
	audio_private = component->component_private;

	if (dest_state == OMX_StateExecuting && component->state == OMX_StateIdle) {
		ret = aenc_open(audio_private);
		OSCL_LOGW("while entering StateExecuting:%d", ret);
		if (ret < 0) {
			OSCL_LOGE("aenc open fail!!!!:%d", ret);
			return OMX_ErrorUndefined;
		}
	}

	pre_state = component->state;
	ret = base_component_set_state(hComp, dest_state);

	if (dest_state == OMX_StateIdle &&
		(pre_state == OMX_StateExecuting || pre_state == OMX_StatePause)) {
		aenc_close(audio_private);
		OSCL_LOGW("close ");
	}

	return ret;
}

/*
 * arec_recive_buffer the entry point for sending buffers to the port
 *
 * This function can be called by the EmptyThisBuffer or FillThisBuffer. It depends on
 * the nature of the port, that can be an input or output port.
 */
#if 0
OMX_ERRORTYPE arec_recive_buffer(base_port_t *port,
	OMX_BUFFERHEADERTYPE *buffer)
{
	OMX_U32 port_idx;
	lb_omx_component_t *component = (lb_omx_component_t *)port->component;
	aenc_private_t *audio_private = (aenc_private_t *)component->component_private;

	OSCL_TRACE("%p:%p", port, buffer);
	port_idx = (port->port_param.eDir == OMX_DirInput) ? buffer->nInputPortIndex :
		buffer->nOutputPortIndex;
	oscl_param_check((port_idx == port->port_param.nPortIndex),
		OMX_ErrorBadPortIndex, NULL);
	oscl_param_check((OMX_StateInvalid != get_component_state(port)),
		OMX_ErrorInvalidState, NULL);
	oscl_param_check(((OMX_StateExecuting == get_component_state(port))
			|| (OMX_StatePause == get_component_state(port))
			|| (OMX_StateIdle == get_component_state(port))
			|| (!PORT_IS_ENABLED(port))
			|| (!(port->is_flushed))),
		OMX_ErrorIncorrectStateOperation, "IncorrectState");
	if (!PORT_IS_SUPPLIER(port) && port->is_flushed) {
		OSCL_LOGW("recive buffers while flush port(none supplier)\n");
		OSCL_LOGW("%p(%s, %d)", port,
			get_port_name(port), port->port_param.nPortIndex);
	}
	if (buffer) {
		while (buffer->nFilledLen) {
			if (buffer->nFilledLen && buffer->pBuffer) {
				int ret = writePcmData(buffer->pBuffer + buffer->nOffset,
						buffer->nFilledLen,
						audio_private->pcm_manager);
				if (ret > 0)
					buffer->nFilledLen -= ret;
			}
			if (buffer->nFlags & OMX_BUFFERFLAG_EOS) {
				OSCL_LOGE("adv_buf get the end of stream\n");
				audio_private->pcm_manager->nFlags |= OMX_BUFFERFLAG_EOS;
			}
		}
		port->return_buffer(port, buffer);
		buffer = NULL;
	}

#if 0
	/* notify the buffer management thread we have a fresh new buffer to manage*/
	oscl_queue_queue(&port->buffer_queue, buffer);
	if (PORT_IS_SUPPLIER(port) && (port->is_flushed))
		sem_post(&port->buffer_sem);
	sem_post(component->buf_mgnt_sem);
#endif
	OSCL_TRACE("%p:%p", port, buffer);

	return OMX_ErrorNone;
}
#endif

OMX_ERRORTYPE aenc_component_deinit(OMX_IN OMX_HANDLETYPE hComp)
{
	OMX_COMPONENTTYPE *base_cmp = (OMX_COMPONENTTYPE *)hComp;
	lb_omx_component_t *component = NULL;
	aenc_private_t *audio_private = NULL;
	int ret;

	OSCL_TRACE("base_cmp_handle:%p\n", hComp);
	oscl_param_check(hComp != NULL, OMX_ErrorBadParameter, NULL);
	component = (lb_omx_component_t *)base_cmp->pComponentPrivate;
	audio_private = (aenc_private_t *)component->component_private;

	component->port[OMX_DEFAULT_INPUT_PORT].deinit(
		&component->port[OMX_DEFAULT_INPUT_PORT]);
	component->port[OMX_DEFAULT_OUTPUT_PORT].deinit(
		&component->port[OMX_DEFAULT_OUTPUT_PORT]);

	if (audio_private) {
		aenc_deinit(audio_private);
		component->component_private = NULL;
	}
	ret = base_component_deinit(hComp);

	return ret;
}

OMX_ERRORTYPE aenc_component_init(lb_omx_component_t *cmp_handle)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	aenc_private_t *audio_private = NULL;
	struct sched_param shed_param = {0};

	audio_private = aenc_init();
	if (!audio_private) {
		OSCL_LOGE("audio_private error!\n");
		return OMX_ErrorInsufficientResources;
	}

	OSCL_TRACE("aenc_component_init:%p\n", cmp_handle);
	ret = base_component_init(cmp_handle);
	if (ret != OMX_ErrorNone)
		return ret;

	/* pthread prrority must high to other */
	pthread_attr_getschedparam(&cmp_handle->buf_thread_attr, &shed_param);
	OSCL_LOGI("priority:%d", shed_param.sched_priority);
	shed_param.sched_priority++;
	pthread_attr_setschedparam(&cmp_handle->buf_thread_attr, &shed_param);
	pthread_attr_setstacksize(&cmp_handle->buf_thread_attr, 0x4000);

	/* port init */
	cmp_handle->num_ports = 2;
	ret = base_port_init(cmp_handle, &cmp_handle->port[OMX_DEFAULT_INPUT_PORT],
			OMX_DEFAULT_INPUT_PORT,
			OMX_DirInput);
	if (ret != OMX_ErrorNone) {
		base_component_deinit(cmp_handle);
		return ret;
	}
	ret = base_port_init(cmp_handle, &cmp_handle->port[OMX_DEFAULT_OUTPUT_PORT],
			OMX_DEFAULT_OUTPUT_PORT,
			OMX_DirOutput);
	if (ret != OMX_ErrorNone) {
		base_component_deinit(cmp_handle);
		cmp_handle->port[OMX_DEFAULT_INPUT_PORT].deinit(
			&cmp_handle->port[OMX_DEFAULT_INPUT_PORT]);
		return ret;
	}
	cmp_handle->port[OMX_DEFAULT_INPUT_PORT].port_param.nBufferSize =
		AUDIO_OUTBUF_DEFAULT_SIZE;
	cmp_handle->port[OMX_DEFAULT_INPUT_PORT].port_param.eDomain =
		OMX_PortDomainAudio;

	cmp_handle->port[OMX_DEFAULT_OUTPUT_PORT].port_param.nBufferSize =
		OMX_DEFAULT_BUF_SIZE;
	cmp_handle->port[OMX_DEFAULT_OUTPUT_PORT].port_param.eDomain =
		OMX_PortDomainAudio;
	cmp_handle->port[OMX_DEFAULT_OUTPUT_PORT].port_param.nBufferCountActual = 8;

	/*cmp_handle->port[OMX_DEFAULT_INPUT_PORT].recive_buffer = arec_recive_buffer;*/

	cmp_handle->name = "OMX.LB.SOURCE.AENC";
	cmp_handle->component_private = audio_private;
	cmp_handle->base_comp.ComponentDeInit = aenc_component_deinit;
	cmp_handle->buf_manager = aenc_buffer_manager;
	cmp_handle->buf_handle = aenc_buffer_handle;
	cmp_handle->base_comp.SetParameter = aenc_set_parameter;
	cmp_handle->do_state_set = aenc_set_state;
	OSCL_TRACE("aenc_component_init:%d\n", ret);

	return ret;
}
