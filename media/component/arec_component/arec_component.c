/*
 * arec_component.c - audio receive component.
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

#define DBG_LEVEL		DBG_ERROR
#include <rtdevice.h>
#include <drivers/audio.h>

#include <oscl.h>
#include <base_component.h>
#include "arec_component.h"
#include "audio/asoundlib.h"

/* #define SAVE_PCM_FILE */

#define DEFAULT_RAMPUP_TIME	1000

typedef struct arec_ctx {
	void		*pcm_dev;
	OMX_U32		channels;
	OMX_U32		samplefmt;
	OMX_U32		samplerate;
	OMX_S32		rampup_data_size;
	pthread_mutex_t lock;
#ifdef SAVE_PCM_FILE
	FILE *outfile;
#endif
} arec_ctx_t;

static OMX_ERRORTYPE audio_receive_init(lb_omx_component_t *component)
{
	pcm_config_t config;
	OMX_S32 ret = 0;
	arec_ctx_t *audio_ctx = NULL;

	audio_ctx = component->component_private;

	audio_ctx->rampup_data_size = audio_ctx->channels * audio_ctx->samplefmt
			* audio_ctx->samplerate * DEFAULT_RAMPUP_TIME / 8 / 1000;

	memset(&config, 0, sizeof(config));
	config.channels = audio_ctx->channels;
	config.bitwidth = audio_ctx->samplefmt;
	config.rate     = audio_ctx->samplerate;
	pthread_mutex_lock(&audio_ctx->lock);
	audio_ctx->pcm_dev = pcm_open(PCM_IN, &config);
	if (audio_ctx->pcm_dev == NULL) {
		OSCL_LOGE("open pcm err\n");
		ret = OMX_ErrorResourcesLost;
	}
	pthread_mutex_unlock(&audio_ctx->lock);
#ifdef SAVE_PCM_FILE
	audio_ctx->outfile = fopen("mnt/sdcard/recout.pcm", "wb");
	if (audio_ctx->outfile == NULL)
		OSCL_LOGE("open out file err!\n");
#endif
	OSCL_LOGI("audio receive init success!");
	return ret;
}

static void audio_receive_exit(lb_omx_component_t *component)
{
	arec_ctx_t *audio_ctx = component->component_private;

	if (audio_ctx->pcm_dev == NULL)
		return;

	pthread_mutex_lock(&audio_ctx->lock);
	pcm_close(audio_ctx->pcm_dev, PCM_IN);
	audio_ctx->pcm_dev = NULL;
	pthread_mutex_unlock(&audio_ctx->lock);
#ifdef SAVE_PCM_FILE
	fclose(audio_ctx->outfile);
#endif

}

void arec_buffer_handle(OMX_HANDLETYPE stand_com,
	OMX_BUFFERHEADERTYPE *inbuffer,
	OMX_BUFFERHEADERTYPE *outbuffer)
{
	arec_ctx_t *audio_ctx = NULL;
	lb_omx_component_t *component = NULL;
	OMX_S32 ret = 0;

	component = get_lb_component(stand_com);
	audio_ctx = component->component_private;
	if (audio_ctx->pcm_dev == NULL
		|| outbuffer == NULL
		|| outbuffer->nAllocLen == 0
		|| outbuffer->pBuffer == NULL) {
		OSCL_LOGE("fd :%d, inbuffer:%p, nAllocLen:%d, pBuffer:%p!!",
			audio_ctx->pcm_dev,
			outbuffer, outbuffer->nAllocLen, outbuffer->pBuffer);
		OSCL_LOGE("err param!");
		return;
	}

	outbuffer->nOffset = 0;
	outbuffer->nFlags = 0;

	pthread_mutex_lock(&audio_ctx->lock);
	ret = pcm_read(audio_ctx->pcm_dev,
		outbuffer->pBuffer, outbuffer->nAllocLen);
	pthread_mutex_unlock(&audio_ctx->lock);
	if (ret <= 0) {
		OSCL_LOGI("audio read data error ret %d", ret);
		outbuffer->nFilledLen = 0;
		return;
	}
	outbuffer->nFilledLen = ret;
	if (audio_ctx->rampup_data_size > 0) {
		memset(outbuffer->pBuffer, 0, outbuffer->nAllocLen);
		audio_ctx->rampup_data_size -= outbuffer->nFilledLen;
	}
#ifdef SAVE_PCM_FILE
	fwrite(outbuffer->pBuffer, 1, outbuffer->nFilledLen, audio_ctx->outfile);
#endif

	OSCL_LOGD("read audio buffer len %d", ret);
}

void *arec_buffer_manager(void *param)
{
	lb_omx_component_t *component = NULL;
	OMX_COMPONENTTYPE *stand_comp = (OMX_COMPONENTTYPE *)param;
	base_port_t *outport = NULL;
	OMX_BUFFERHEADERTYPE *outbuffer = NULL;

	OSCL_TRACE(" %p\n", param);
	oscl_param_check((param != NULL), NULL, NULL);
	component = get_lb_component(stand_comp);
	outport = &component->port[AUDIO_OUTPUT_PORT_INDEX];

	while (component->state == OMX_StateIdle
		|| component->state == OMX_StateExecuting
		|| component->state == OMX_StatePause
		|| component->target_state == OMX_StateIdle) {

		while (sem_trywait(component->buf_mgnt_sem) == 0)
			continue;
		/*Wait till the ports are being flushed*/
		pthread_mutex_lock(&component->flush_mutex);
		while (outport->is_flushed) {
			pthread_mutex_unlock(&component->flush_mutex);
			if (outbuffer != NULL) {
				outport->return_buffer(outport, outbuffer);
				outbuffer = NULL;
				OSCL_LOGI("retrun buffer while flushing port");
			}
			OSCL_LOGI("%s sem_wait component->flush_sem",
				get_component_name(component));
			sem_post(component->mgmt_flush_sem);
			sem_wait(component->flush_sem);
			pthread_mutex_lock(&component->flush_mutex);
		}
		pthread_mutex_unlock(&component->flush_mutex);

		OSCL_LOGD("component->state:%d, sem value:%d", component->state,
			component->buf_mgnt_sem->sem->value);
		if (component->state != OMX_StateExecuting) {
			sem_wait(component->buf_mgnt_sem);
			continue;
		}

		OSCL_LOGD("comp:%s, queue len:%d outbuf:%x",
			get_component_name(component),
			oscl_queue_get_num(&outport->buffer_queue), outbuffer);
		if (outbuffer == NULL) {
			outbuffer = oscl_queue_dequeue(&outport->buffer_queue);
			OSCL_LOGD("read buf manager:%x\n", outbuffer);
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
		/* only when we're at executing state, we handle the buffer */
		if (component->state == OMX_StateExecuting) {
			if (component->buf_handle && outbuffer->nAllocLen != 0)
				(*(component->buf_handle))(stand_comp, NULL, outbuffer);
		}
		base_check_mark(component, outbuffer);
		/* if we have an eos flag, then notify app from eventHandler */
		base_check_eos(component, outport, outbuffer);
		/* if we successfully fill the buffer, then return it */
		if (outbuffer->nFilledLen != 0
			|| component->target_state == OMX_StateLoaded) {
			OSCL_LOGI("outport return_buffer");
			outport->return_buffer(outport, outbuffer);
			outbuffer = NULL;
		}
		outbuffer = NULL;
	}
	if ((outbuffer != NULL) && (!PORT_IS_SUPPLIER(outport))) {
		OSCL_LOGI("outport return_buffer before exit");
		outport->return_buffer(outport, outbuffer);
	}
	OSCL_LOGI("exit from reader_buffer_manager:%s\n", rt_thread_self()->name);
	OSCL_TRACE(" %p\n", param);
	pthread_exit(NULL);

	return NULL;

}

OMX_ERRORTYPE arec_set_config(OMX_IN OMX_HANDLETYPE cmp_hdl,
	OMX_IN OMX_INDEXTYPE index,
	OMX_IN OMX_PTR data)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	OMX_AUDIO_CONFIG_MUTETYPE *mute = NULL;

	mute = (OMX_AUDIO_CONFIG_MUTETYPE *)data;
	switch (index) {
	case OMX_IndexConfigAudioMute:
		if (mute->bMute == OMX_TRUE)
			pcm_set_mute(1, PCM_IN);
		else
			pcm_set_mute(0, PCM_IN);
		break;
	default:
		OSCL_LOGE("config index %d not support yet\n");
		break;
	}

	return ret;
}

OMX_ERRORTYPE arec_set_parameter(OMX_IN OMX_HANDLETYPE hComp,
		OMX_IN OMX_INDEXTYPE paramIndex,
		OMX_IN OMX_PTR paramData)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component = NULL;
	arec_ctx_t *audio_ctx = NULL;
	OMX_AUDIO_PARAM_PCMMODETYPE *audio_params = NULL;

	audio_params = (OMX_AUDIO_PARAM_PCMMODETYPE *)paramData;
	OSCL_TRACE(" %p, %p , %d\n", hComp, paramData, paramIndex);
	oscl_param_check((hComp != NULL) && (paramData != NULL),
		OMX_ErrorBadParameter, NULL);
	component = get_lb_component(hComp);
	oscl_param_check((component != NULL), OMX_ErrorBadParameter, NULL);
	audio_ctx = component->component_private;

	switch (paramIndex) {
	case OMX_IndexParamAudioPcm:
		audio_ctx->channels = audio_params->nChannels;
		audio_ctx->samplefmt = audio_params->nBitPerSample;
		audio_ctx->samplerate = audio_params->nSamplingRate;
		break;
	default:
		OSCL_LOGI("default base set params index %d\n", paramIndex);
		ret = base_set_parameter(hComp, paramIndex, paramData);
		break;
	}
	OSCL_TRACE(" %d\n", ret);

	return ret;
}

OMX_ERRORTYPE arec_set_state(OMX_HANDLETYPE hComp,
	OMX_U32 dest_state)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component = NULL;
	arec_ctx_t *audio_ctx = NULL;
	OMX_STATETYPE pre_state;

	OSCL_TRACE(" %p, %x\n", hComp, dest_state);
	oscl_param_check(hComp != NULL, OMX_ErrorBadParameter, NULL);
	component = get_lb_component(hComp);
	audio_ctx = component->component_private;

	if (dest_state == OMX_StateExecuting && component->state == OMX_StateIdle) {
		if (audio_ctx->pcm_dev != NULL) {
			OSCL_LOGW("Device not closed while entering StateIdle");
			audio_receive_exit(component);
		}
		ret = audio_receive_init(component);
		if (OMX_ErrorNone != ret)
			return ret;
	}

	pre_state = component->state;
	ret = base_component_set_state(hComp, dest_state);
	if (dest_state == OMX_StateIdle && pre_state == OMX_StateExecuting) {
		OSCL_LOGI("audio receiver exit\n");
		audio_receive_exit(component);
	}
	return ret;

}

OMX_ERRORTYPE arec_component_deinit(OMX_IN OMX_HANDLETYPE hComponent)
{
	OMX_COMPONENTTYPE *base_cmp = (OMX_COMPONENTTYPE *)hComponent;
	lb_omx_component_t *component = NULL;
	arec_ctx_t *audio_ctx = NULL;
	OMX_ERRORTYPE ret = OMX_ErrorNone;

	OSCL_TRACE("base_cmp_handle:%p\n", hComponent);
	oscl_param_check(hComponent != NULL, OMX_ErrorBadParameter, NULL);
	component = (lb_omx_component_t *)base_cmp->pComponentPrivate;
	audio_ctx = (arec_ctx_t *)component->component_private;

	component->port[AUDIO_OUTPUT_PORT_INDEX].deinit(
		&component->port[AUDIO_OUTPUT_PORT_INDEX]);
	pthread_mutex_destroy(&audio_ctx->lock);
	oscl_free(audio_ctx);
	ret = base_component_deinit(hComponent);

	return ret;
}

OMX_ERRORTYPE arec_component_init(lb_omx_component_t *cmp_handle)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	arec_ctx_t *audio_ctx = NULL;

	audio_ctx = oscl_zalloc(sizeof(arec_ctx_t));
	if (!audio_ctx) {
		OSCL_LOGE("malloc arec_ctx_t error!\n");
		return OMX_ErrorInsufficientResources;
	}
	pthread_mutex_init(&audio_ctx->lock, NULL);

	ret = base_component_init(cmp_handle);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("base_component_init error!\n");
		goto error1;
	}
	cmp_handle->name = "OMX.LB.SOURCE.AREC";
	cmp_handle->component_private = audio_ctx;
	cmp_handle->buf_manager = arec_buffer_manager;
	cmp_handle->buf_handle = arec_buffer_handle;
	cmp_handle->base_comp.ComponentDeInit = arec_component_deinit;
	cmp_handle->base_comp.SetParameter = arec_set_parameter;
	cmp_handle->base_comp.SetConfig = arec_set_config;
	cmp_handle->do_state_set = arec_set_state;
	cmp_handle->num_ports = 1;
	ret = base_port_init(cmp_handle, &cmp_handle->port[AUDIO_OUTPUT_PORT_INDEX],
			AUDIO_OUTPUT_PORT_INDEX,
			OMX_DirOutput);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("base_port_init error!\n");
		goto error2;
	}
	cmp_handle->port[AUDIO_OUTPUT_PORT_INDEX].port_param.nBufferSize =
		AUDIO_OUTBUF_DEFAULT_SIZE;
	cmp_handle->port[AUDIO_OUTPUT_PORT_INDEX].port_param.eDomain =
		OMX_PortDomainAudio;
	cmp_handle->port[AUDIO_OUTPUT_PORT_INDEX].port_param.nBufferCountActual =
		8;

	return ret;

error2:
	base_component_deinit(cmp_handle);
error1:
	if (audio_ctx != NULL)
			oscl_free(audio_ctx);
	OSCL_TRACE("%d arec:%p\n", ret, cmp_handle);

	return ret;

}

