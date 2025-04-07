#define DBG_LEVEL		DBG_ERR

#include <oscl.h>
#include <rtdevice.h>
#include <base_component.h>
#include "audio_core.h"
#include "arender_component.h"
#include "clock_component.h"
#include "clock_port.h"
#include "audio/asoundlib.h"

/* #define SAVE_PCM_FILE */

#define HW_OFFSET_THRESH	400000

typedef struct arender_ctx {
	void		*pcm_dev; /* pcm device */
	OMX_U32		channels; /* audio steam channels */
	OMX_U32		samplefmt; /* bits per sample */
	OMX_U32		samplerate; /* sample rate of audio sream */
	pthread_mutex_t lock;
#ifdef SAVE_PCM_FILE
	FILE *outfile;
#endif
	OMX_TIME_CLOCKSTATE clk_state; /* clk state */
	OMX_S32             xscale;    /* scale of the clk */
	OMX_S64             audio_hw_offset;
} arender_ctx_t;

static OMX_ERRORTYPE audio_render_init(lb_omx_component_t *component)
{
	OMX_S32 ret = 0;
	arender_ctx_t *audio_ctx = NULL;
	pcm_config_t config;

	audio_ctx = component->component_private;
	memset(&config, 0, sizeof(config));
	config.channels = audio_ctx->channels;
	config.bitwidth = audio_ctx->samplefmt;
	config.rate     = audio_ctx->samplerate;
	pthread_mutex_lock(&audio_ctx->lock);
	audio_ctx->pcm_dev = pcm_open(PCM_OUT, &config);
	if (audio_ctx->pcm_dev == NULL) {
		OSCL_LOGE("open pcm err\n");
		ret = OMX_ErrorResourcesLost;
	}
	pthread_mutex_unlock(&audio_ctx->lock);
#ifdef SAVE_PCM_FILE
	audio_ctx->outfile = fopen("aacout.pcm", "wb");
	if (audio_ctx->outfile == NULL)
		OSCL_LOGE("open out file err!\n");
#endif
	OSCL_LOGI("end audio render init!");
	return ret;
}

static void audio_render_exit(lb_omx_component_t *component)
{
	arender_ctx_t *audio_ctx = component->component_private;

	pthread_mutex_lock(&audio_ctx->lock);
	if (audio_ctx->pcm_dev == NULL) {
		pthread_mutex_unlock(&audio_ctx->lock);
		OSCL_LOGI("audio_render_exit pcmdev null\n");
		return;
	}
	pcm_close(audio_ctx->pcm_dev, PCM_OUT);
	audio_ctx->pcm_dev = NULL;
	pthread_mutex_unlock(&audio_ctx->lock);
	OSCL_LOGI("end audio render exit\n");

#ifdef SAVE_PCM_FILE
	fclose(audio_ctx->outfile);
#endif
}

static OMX_BOOL clock_handle_frame(lb_omx_component_t *component,
	OMX_BUFFERHEADERTYPE *inbuffer)
{
	base_port_t *clkport = NULL;
	arender_ctx_t *audio_ctx = NULL;
	OMX_HANDLETYPE                        clk_cmp = NULL;
	OMX_BUFFERHEADERTYPE                  *clk_buffer = NULL;
	OMX_TIME_MEDIATIMETYPE                *mediatime;
	OMX_TIME_CONFIG_TIMESTAMPTYPE         client_timestamp;
	OMX_ERRORTYPE                         err;
	OMX_BOOL                              sendframe;

	audio_ctx = component->component_private;
	clkport = &component->port[ARENDER_CLOCK_PORT];
	clk_cmp = clkport->tunneled_component;

	sendframe = OMX_TRUE;

	/* check for any scale change or clock state change
	 * information from the clock component */
	if (sem_get(&clkport->buffer_sem) > 0) {
		OSCL_LOGI("sem value %d\n", sem_get(&clkport->buffer_sem));
		clk_buffer = oscl_queue_dequeue(&clkport->buffer_queue);
		sem_wait(&clkport->buffer_sem);
		OSCL_LOGD("audio wait for sem  end\n");
	}
	if (clk_buffer != NULL) {
		mediatime = (OMX_TIME_MEDIATIMETYPE *)clk_buffer->pBuffer;
		if (mediatime->eUpdateType == OMX_TIME_UpdateScaleChanged) {
			audio_ctx->xscale = mediatime->xScale;
			OSCL_LOGI("got scale changed scale %d\n", audio_ctx->xscale);
		} else if (mediatime->eUpdateType == OMX_TIME_UpdateClockStateChanged) {
			audio_ctx->clk_state = mediatime->eState;
			OSCL_LOGI("clock state change %d\n", audio_ctx->clk_state);
		}
		clkport->return_buffer(clkport, clk_buffer);
		clk_buffer = NULL;
	}

	/* if first timestamp is received, then notify clk component */
	if (inbuffer->nFlags & OMX_BUFFERFLAG_STARTTIME) {
		OSCL_LOGI("audio first timestamp received %lld\n", inbuffer->nTimeStamp);
		client_timestamp.nSize = sizeof(client_timestamp);
		client_timestamp.nPortIndex = clkport->tunnel_ports;
		client_timestamp.nTimestamp =
			inbuffer->nTimeStamp - audio_ctx->audio_hw_offset;
		err = OMX_SetConfig(clk_cmp,
			OMX_IndexConfigTimeClientStartTime, &client_timestamp);
		if (err != OMX_ErrorNone)
			OSCL_LOGE("set client start time error!\n");
		/* wait for state change notification */
		OSCL_LOGI("sem %d (%s, %d)", sem_get(&clkport->buffer_sem),
			get_port_name(clkport), clkport->port_param.nPortIndex);
		oscl_sem_timedwait_ms(&clkport->buffer_sem, 1000);
		OSCL_LOGD("audio wait for sem  end\n");

		clk_buffer = oscl_queue_dequeue(&clkport->buffer_queue);
		if (clk_buffer != NULL) {
			mediatime = (OMX_TIME_MEDIATIMETYPE *)clk_buffer->pBuffer;
			audio_ctx->clk_state = mediatime->eState;
			audio_ctx->xscale    = mediatime->xScale;
			clkport->return_buffer(clkport, clk_buffer);
			clk_buffer = NULL;
			OSCL_LOGI("arender clock state %d\n", audio_ctx->clk_state);
		}
		/* first frame, send it anyway */
		sendframe = OMX_TRUE;
		return sendframe;
	}

	/* do not send the data to sink and return back,
	* if the clock is not running */
	if (audio_ctx->clk_state != OMX_TIME_ClockStateRunning) {
		OSCL_LOGD("clock is not running now, drop this frame!\n");
		inbuffer->nFilledLen = 0;
		sendframe = OMX_FALSE;
		return sendframe;
	}

	/* update mediatime base */
	client_timestamp.nPortIndex = clkport->tunnel_ports;
	client_timestamp.nTimestamp = inbuffer->nTimeStamp - audio_ctx->audio_hw_offset;
	if (inbuffer->nFlags & OMX_BUFFERFLAG_EOS)
		client_timestamp.nTimestamp = 0xffffffff;
	/* OSCL_LOGI("timestamp received %lld\n", inbuffer->nTimeStamp); */
	err = OMX_SetConfig(clk_cmp,
		OMX_IndexConfigTimeCurrentAudioReference,
		&client_timestamp);
	if (err != OMX_ErrorNone)
		OSCL_LOGE("set audio ref time error!\n");

	return sendframe;
}

void arender_buffer_handle(OMX_HANDLETYPE stand_com,
	OMX_BUFFERHEADERTYPE *inbuffer,
	OMX_BUFFERHEADERTYPE *outbuffer)
{
	arender_ctx_t *audio_ctx = NULL;
	lb_omx_component_t *component = NULL;

	component = get_lb_component(stand_com);
	audio_ctx = component->component_private;
	if (inbuffer == NULL
		|| inbuffer->pBuffer == NULL) {
		OSCL_LOGE("inbuffer:%p, nFilledLen:%d, pBuffer:%p!!",
			inbuffer, inbuffer->nFilledLen, inbuffer->pBuffer);
		return;
	}
	if (audio_ctx->pcm_dev == NULL) {
		if (audio_render_init(component)) {
			OSCL_LOGE("init arender err\n");
			return;
		}
	}
	OSCL_LOGD("fill len %d, offset %d\n", inbuffer->nFilledLen, inbuffer->nOffset);

	if (inbuffer->nFilledLen == 0) {
		OSCL_LOGW("nFilledLen = 0, return!\n");
		return;
	}
#ifdef SAVE_PCM_FILE
	if (inbuffer->nOffset == 0)
		fwrite(inbuffer->pBuffer, 1, inbuffer->nFilledLen, audio_ctx->outfile);
#endif
	pthread_mutex_lock(&audio_ctx->lock);
	pcm_write(audio_ctx->pcm_dev,
			inbuffer->pBuffer, inbuffer->nFilledLen);
	inbuffer->nOffset = inbuffer->nFilledLen;
	pthread_mutex_unlock(&audio_ctx->lock);
}

void *arender_buffer_manager(void *param)
{
	lb_omx_component_t *component = NULL;
	OMX_COMPONENTTYPE *stand_comp = (OMX_COMPONENTTYPE *)param;
	base_port_t *inport = NULL;
	OMX_BUFFERHEADERTYPE *inbuffer = NULL;

	OSCL_TRACE(" %p\n", param);
	oscl_param_check((param != NULL), NULL, NULL);
	component = get_lb_component(stand_comp);
	inport = &component->port[BASE_INPUT_PORT];

	while (component->state == OMX_StateIdle
		|| component->state == OMX_StateExecuting
		|| component->state == OMX_StatePause
		|| component->target_state == OMX_StateIdle) {

		/*Wait till the ports are being flushed*/
		pthread_mutex_lock(&component->flush_mutex);
		while (inport->is_flushed) {
			pthread_mutex_unlock(&component->flush_mutex);
			if (inbuffer != NULL) {
				inport->return_buffer(inport, inbuffer);
				inbuffer = NULL;
				OSCL_LOGI("retrun buffer while flushing port");
			}
			sem_post(component->mgmt_flush_sem);
			sem_wait(component->flush_sem);
			OSCL_LOGW("%s flush in buffer manager thread\n",
				get_component_name(component));
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
			oscl_queue_get_num(&inport->buffer_queue), inbuffer);
		if (inbuffer == NULL) {
			inbuffer = oscl_queue_dequeue(&inport->buffer_queue);
			OSCL_LOGD("read buf manager:%x\n", inbuffer);
		}
		if (inbuffer == NULL) {
			OSCL_LOGD("sem_wait component->buf_mgnt_sem");
			sem_wait(component->buf_mgnt_sem);
			continue;
		}

		if (!clock_handle_frame(component, inbuffer)) {
			inbuffer->nOffset = 0;
			inbuffer->nFilledLen = 0;
			inbuffer->nFlags = 0;
			inport->return_buffer(inport, inbuffer);
			inbuffer = NULL;
			OSCL_LOGD("skip one aduio frame\n");
			continue;
		}

		if (component->mark.hMarkTargetComponent) {
			inbuffer->hMarkTargetComponent
				= component->mark.hMarkTargetComponent;
			inbuffer->pMarkData
				= component->mark.pMarkData;
			component->mark.hMarkTargetComponent = NULL;
			component->mark.pMarkData = NULL;
		}

		OSCL_LOGD("component->state:%x,inbuffer->nFilledLen:%d",
			component->state,
			inbuffer->nFilledLen);
		/* only when we're at executing state, we handle the buffer */
		if (component->state == OMX_StateExecuting) {
			if (component->buf_handle)
				(*(component->buf_handle))(stand_comp, inbuffer, NULL);
		}

		if (inbuffer->hMarkTargetComponent == stand_comp) {
			(*(component->callbacks.EventHandler))
			(stand_comp,
				component->callback_data,
				OMX_EventMark,
				0,
				0,
				inbuffer->pMarkData);
		}
		if (inbuffer->nFlags & OMX_BUFFERFLAG_STARTTIME) {
			inbuffer->nFlags &= (~OMX_BUFFERFLAG_STARTTIME);
			(*(component->callbacks.EventHandler))
			(stand_comp, component->callback_data,
				OMX_EventOutputRendered, 0, 0, NULL);
			OSCL_LOGD("out put render\n");
		}
		/* if we have an eos flag, then notify app from eventHandler */
		if ((inbuffer->nFlags & OMX_BUFFERFLAG_EOS) == OMX_BUFFERFLAG_EOS) {
			OSCL_LOGI("component:%s, flag %d, get a EOS flags in inbuffer",
				get_component_name(component), inbuffer->nFlags);
			audio_render_exit(component);
			(*(component->callbacks.EventHandler))
			(stand_comp,
				component->callback_data,
				OMX_EventBufferFlag,
				inport->port_param.nPortIndex,
				OMX_BUFFERFLAG_EOS,
				NULL);
			component->eos_flag = OMX_TRUE;
		}
		/* if we successfully fill the buffer, then return it */
		if (inbuffer->nFilledLen == inbuffer->nOffset
			|| (inbuffer->nFlags & OMX_BUFFERFLAG_EOS)) {
			OSCL_LOGD("inport return_buffer");
			inbuffer->nOffset = 0;
			inbuffer->nFilledLen = 0;
			inbuffer->nFlags = 0;
			inport->return_buffer(inport, inbuffer);
			inbuffer = NULL;
		}
	}
	if ((inbuffer != NULL) && (!PORT_IS_SUPPLIER(inport))) {
		OSCL_LOGI("outport return_buffer before exit");
		inport->return_buffer(inport, inbuffer);
	}
	oscl_queue_flush(&inport->buffer_queue);
	OSCL_LOGI("exit from arender_buffer_manager:%s\n", rt_thread_self()->name);
	OSCL_LOGW("exit, inport buffer left %d\n",
		inport->buffer_queue.count);
	OSCL_TRACE(" %p\n", param);

	return NULL;

}

OMX_ERRORTYPE arender_set_parameter(OMX_IN OMX_HANDLETYPE hComp,
	OMX_IN OMX_INDEXTYPE paramIndex,
	OMX_IN OMX_PTR paramData)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component = NULL;
	arender_ctx_t *audio_ctx = NULL;
	OMX_AUDIO_PARAM_PCMMODETYPE *audio_params = NULL;
	OMX_S64 offset;

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
		offset = REPLAY_MP_BLOCK_SIZE * REPLAY_MP_COUNT * 1000 /
			(2 * audio_ctx->samplerate *
			audio_ctx->samplefmt / 8 / 1000);
		/* use 2/3 of the hw_offset */
		audio_ctx->audio_hw_offset = offset * 2 / 3;
		if (audio_ctx->audio_hw_offset > HW_OFFSET_THRESH)
			audio_ctx->audio_hw_offset = HW_OFFSET_THRESH;
		OSCL_LOGI("audio_hw_offset %lld\n", audio_ctx->audio_hw_offset);
		break;
	default:
		OSCL_LOGI("default base set params index %d\n", paramIndex);
		ret = base_set_parameter(hComp, paramIndex, paramData);
		break;
	}
	OSCL_TRACE(" %d\n", ret);

	return ret;
}

OMX_ERRORTYPE arender_set_state(OMX_HANDLETYPE hComp,
	OMX_U32 dest_state)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component = NULL;
	arender_ctx_t *audio_ctx = NULL;
	OMX_STATETYPE pre_state;

	OSCL_TRACE(" %p, %x\n", hComp, dest_state);
	oscl_param_check(hComp != NULL, OMX_ErrorBadParameter, NULL);
	component = get_lb_component(hComp);
	audio_ctx = component->component_private;

	if (dest_state == OMX_StateExecuting &&
		(component->state == OMX_StateIdle ||
		component->state == OMX_StatePause)) {
		if (audio_ctx->pcm_dev != NULL) {
			OSCL_LOGW("Device not closed while entering StateIdle");
			audio_render_exit(component);
		}
		ret = audio_render_init(component);
		if (OMX_ErrorNone != ret)
			return ret;
	}

	pre_state = component->state;
	ret = base_component_set_state(hComp, dest_state);
	if (dest_state == OMX_StatePause && pre_state == OMX_StateExecuting) {
		OSCL_LOGI("audio receiver exit\n");
		audio_render_exit(component);
	}
	if (dest_state == OMX_StateIdle &&
		(pre_state == OMX_StateExecuting || pre_state == OMX_StatePause)) {
		OSCL_LOGD("audio render exit\n");
		audio_render_exit(component);
	}

	return ret;

}


OMX_ERRORTYPE arender_component_deinit(OMX_IN OMX_HANDLETYPE hComponent)
{
	OMX_COMPONENTTYPE *base_cmp = (OMX_COMPONENTTYPE *)hComponent;
	lb_omx_component_t *component = NULL;
	arender_ctx_t *audio_ctx = NULL;
	OMX_ERRORTYPE ret = OMX_ErrorNone;

	OSCL_TRACE("base_cmp_handle:%p\n", hComponent);
	oscl_param_check(hComponent != NULL, OMX_ErrorBadParameter, NULL);
	component = (lb_omx_component_t *)base_cmp->pComponentPrivate;
	audio_ctx = (arender_ctx_t *)component->component_private;

	component->port[BASE_INPUT_PORT].deinit(
		&component->port[BASE_INPUT_PORT]);
	clock_port_deinit(&component->port[ARENDER_CLOCK_PORT]);
	pthread_mutex_destroy(&audio_ctx->lock);
	oscl_free(audio_ctx);
	ret = base_component_deinit(hComponent);

	return ret;
}

OMX_ERRORTYPE arender_component_init(lb_omx_component_t *cmp_handle)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	arender_ctx_t *audio_ctx = NULL;

	audio_ctx = oscl_zalloc(sizeof(arender_ctx_t));
	if (!audio_ctx) {
		OSCL_LOGE("malloc arender_ctx_t error!\n");
		return OMX_ErrorInsufficientResources;
	}
	pthread_mutex_init(&audio_ctx->lock, NULL);

	ret = base_component_init(cmp_handle);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("base_component_init error!\n");
		goto error1;
	}
	cmp_handle->name = "OMX.LB.SINK.ARENDER";
	cmp_handle->component_private = audio_ctx;
	cmp_handle->buf_manager = arender_buffer_manager;
	cmp_handle->buf_handle = arender_buffer_handle;
	cmp_handle->base_comp.ComponentDeInit = arender_component_deinit;
	cmp_handle->base_comp.SetParameter = arender_set_parameter;
	cmp_handle->do_state_set = arender_set_state;
	cmp_handle->num_ports = 2;
	ret = base_port_init(cmp_handle, &cmp_handle->port[BASE_INPUT_PORT],
			BASE_INPUT_PORT,
			OMX_DirInput);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("base_port_init error!\n");
		goto error2;
	}
	ret = clock_port_init(cmp_handle, &cmp_handle->port[ARENDER_CLOCK_PORT],
		ARENDER_CLOCK_PORT, OMX_DirInput);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("clock_port_init error!\n");
		goto error3;
	}

	return ret;

error3:
	base_port_deinit(&cmp_handle->port[BASE_INPUT_PORT]);
error2:
	base_component_deinit(cmp_handle);
error1:
	if (audio_ctx != NULL)
		oscl_free(audio_ctx);
	OSCL_TRACE("%d arec:%p\n", ret, cmp_handle);

	return ret;
}

