/*
 * arecorder_test.c - test audio record omx component.
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
#define DBG_LEVEL		DBG_ERROR /* DBG_INFO */

#include <oscl.h>
#include <base_component.h>
#include <lb_omx_core.h>
#include <omx_test.h>
#include "arec_component.h"

#define TEST_RATE		48000
#define TEST_CHANNELS		2
#define TEST_FMT_WIDTH		16
#define TEST_FILE_NAME		"/audio_out.pcm"

#define DEFAULT_AREC_BUF_CNT		4

typedef struct arecorder_test {
	OMX_COMPONENTTYPE *source;
	OMX_COMPONENTTYPE *sink;

	OMX_PARAM_PORTDEFINITIONTYPE port_para;

	sem_t sem_event;
	sem_t sem_eos;

	OMX_S32 buf_num;
	OMX_BUFFERHEADERTYPE **buffer;
	int stat;
	int nbuf_done;
} arecorder_test_t;

static arecorder_test_t audio_rec_priv;
static OMX_U32 error_value = 0u;

/* Callbacks implementation */
static OMX_ERRORTYPE event_handler(
	OMX_HANDLETYPE hComponent,
	OMX_PTR pAppData,
	OMX_EVENTTYPE eEvent,
	OMX_U32 Data1,
	OMX_U32 Data2,
	OMX_PTR pEventData)
{
	arecorder_test_t *private = (arecorder_test_t *)pAppData;
	char *name;
	OSCL_TRACE("hComponent:%p", hComponent);
	if (hComponent == private->sink)
		name = "sink";
	if (hComponent == private->source)
		name = "source";
	if (eEvent == OMX_EventCmdComplete) {
		if (Data1 == OMX_CommandStateSet) {
			switch ((int)Data2) {
			case OMX_StateInvalid:
				OSCL_LOGI("%s StateSet OMX_StateInvalid", name);
				break;
			case OMX_StateLoaded:
				OSCL_LOGI("%s StateSet OMX_StateLoaded", name);
				break;
			case OMX_StateIdle:
				OSCL_LOGI("%s StateSet OMX_StateIdle", name);
				break;
			case OMX_StateExecuting:
				OSCL_LOGI("%s StateSet OMX_StateExecuting", name);
				break;
			case OMX_StatePause:
				OSCL_LOGI("%s StateSet OMX_StatePause", name);
				break;
			case OMX_StateWaitForResources:
				OSCL_LOGI("%s StateSet WaitForResources", name);
				break;
			default:
				OSCL_LOGE("%s StateSet unkown state", name);
				break;
			}
			sem_post(&private->sem_event);
		} else if (Data1 == OMX_CommandPortEnable) {
			OSCL_LOGI("%s CmdComplete OMX_CommandPortEnable", name);
			sem_post(&private->sem_event);
		} else if (Data1 == OMX_CommandPortDisable) {
			OSCL_LOGI("%s CmdComplete OMX_CommandPortDisable", name);
			sem_post(&private->sem_event);
		}
	} else if (eEvent == OMX_EventBufferFlag) {
		if ((int)Data2 == OMX_BUFFERFLAG_EOS) {
			OSCL_LOGI("%s BufferFlag OMX_BUFFERFLAG_EOS", name);
			if (hComponent == private->sink) {
				OSCL_LOGE("end of tunnel");
				sem_post(&private->sem_eos);
			}
		} else
			OSCL_LOGI("%s OMX_EventBufferFlag %x", name, Data2);
	}  else if (eEvent == OMX_EventError) {
		error_value = Data1;
		OSCL_LOGE("Receive error event. value:%x", error_value);
		sem_post(&private->sem_event);
	} else {
		OSCL_LOGI("%s parm:%x %x", name, Data1, Data2);
	}

	return OMX_ErrorNone;
}

static OMX_ERRORTYPE untunnel_empty_buffer_done(
	OMX_HANDLETYPE hComponent,
	OMX_PTR pAppData,
	OMX_BUFFERHEADERTYPE *pBuffer)
{
	OMX_ERRORTYPE err;
	arecorder_test_t *priv = pAppData;

	if (pBuffer == NULL || pAppData == NULL) {
		OSCL_LOGE("err: buffer header is null");
		return OMX_ErrorBadParameter;
	}
	OSCL_LOGD("empty buffer done>> %p, %d, input:%d output:%d", pBuffer->pBuffer,
		pBuffer->nFlags, pBuffer->nInputPortIndex, pBuffer->nOutputPortIndex);

	pBuffer->nOutputPortIndex = AUDIO_OUTPUT_PORT_INDEX;
	err = OMX_FillThisBuffer(priv->source, pBuffer);
	if (err != OMX_ErrorNone)
		OSCL_LOGE("OMX_FillThisBuffer err: %x", err);

	return err;
}

static OMX_ERRORTYPE untunnel_fill_buffer_done(
	OMX_HANDLETYPE hComponent,
	OMX_PTR pAppData,
	OMX_BUFFERHEADERTYPE *pBuffer)
{
	OMX_ERRORTYPE err;
	arecorder_test_t *priv = pAppData;

	if (pBuffer == NULL || pAppData == NULL) {
		OSCL_LOGE("err: buffer header is null");
		return OMX_ErrorBadParameter;
	}
	OSCL_LOGD("fill buffer done>> %p, %d, input:%d output:%d", pBuffer->pBuffer,
		pBuffer->nFlags, pBuffer->nInputPortIndex, pBuffer->nOutputPortIndex);

	pBuffer->nInputPortIndex = OMX_DEFAULT_INPUT_PORT;
	err = OMX_EmptyThisBuffer(priv->sink, pBuffer);
	if (err != OMX_ErrorNone)
		OSCL_LOGE("OMX_EmptyThisBuffer err: %x", err);

	return err;
}

static OMX_CALLBACKTYPE untunnel_callbacks = {
	.EventHandler = event_handler,
	.EmptyBufferDone = untunnel_empty_buffer_done,
	.FillBufferDone = untunnel_fill_buffer_done,
};

static OMX_ERRORTYPE alloc_buffer(arecorder_test_t *priv, OMX_S32 num, OMX_S32 size)
{
	OMX_S32 i = 0;
	OMX_S32 port_index = AUDIO_OUTPUT_PORT_INDEX;
	OMX_ERRORTYPE ret = OMX_ErrorNone;

	priv->buffer = oscl_zalloc(num * sizeof(OMX_BUFFERHEADERTYPE *));
	if (NULL == priv->buffer)
		return OMX_ErrorBadParameter;
	for (i = 0; i < num; i++) {
		priv->buffer[i] = NULL;
		ret = OMX_AllocateBuffer(priv->source, &priv->buffer[i],
				port_index, priv, size);
		OSCL_LOGI("AllocateBuffer %p on port %i", priv->buffer[i], port_index);
		if (ret != OMX_ErrorNone) {
			OSCL_LOGE("Error on AllocateBuffer %p on port %i\n",
				&priv->buffer[i], port_index);
			break;
		}
	}

	return ret;
}

static void free_buffer(arecorder_test_t *priv)
{
	OMX_S32 i = 0;
	OMX_ERRORTYPE ret = OMX_ErrorNone;

	if (priv->buffer) {
		for (i = 0; i < priv->buf_num; i++) {
			if (priv->buffer[i]) {
				ret = OMX_FreeBuffer(priv->source,
						AUDIO_OUTPUT_PORT_INDEX,
						priv->buffer[i]);
				if (ret != OMX_ErrorNone)
					OSCL_LOGE("port %d ,freebuffer:%d failed",
						AUDIO_OUTPUT_PORT_INDEX, i);
			}
			priv->buffer[i] = NULL;
		}
		oscl_free(priv->buffer);
		priv->buffer = NULL;
	}
}

static void mute_record(arecorder_test_t *priv, OMX_BOOL bmute)
{
	int ret = OMX_ErrorNone;
	OMX_AUDIO_CONFIG_MUTETYPE mute;

	memset(&mute, 0, sizeof(mute));
	mute.bMute = bmute;
	ret = OMX_SetConfig(priv->source, OMX_IndexConfigAudioMute, &mute);
	if (ret)
		OSCL_LOGE("set mute config err\n");
}

static int untunnel_config_sink_component(arecorder_test_t *priv)
{
	int ret = OMX_ErrorNone;
	OMX_PARAM_PORTDEFINITIONTYPE port_def;

	memset(&port_def, 0, sizeof(port_def));
	port_def.nBufferCountActual = priv->buf_num;
	port_def.bBuffersContiguous = 1;
	port_def.eDomain = OMX_PortDomainOther;
	port_def.nBufferSize = AUDIO_OUTBUF_DEFAULT_SIZE;
	ret = OMX_SetParameter(priv->sink, OMX_IndexParamPortDefinition,
			&port_def);
	if (ret) {
		OSCL_LOGE("set port params error!");
		return ret;
	}

	ret = OMX_SetParameter(priv->sink, omx_index_vendor_output_filename,
			TEST_FILE_NAME);
	if (ret) {
		OSCL_LOGE("set output filename params error!");
		return ret;
	}

	return ret;
}

static int untunnel_config_source_component(arecorder_test_t *priv)
{
	int ret = OMX_ErrorNone;
	OMX_AUDIO_PARAM_PCMMODETYPE audio_params;
	OMX_PARAM_PORTDEFINITIONTYPE port_def;

	memset(&priv->port_para, 0, sizeof(priv->port_para));
	priv->buf_num = DEFAULT_AREC_BUF_CNT;
	priv->port_para.nBufferCountActual = priv->buf_num;
	priv->port_para.bBuffersContiguous = 1;
	priv->port_para.eDomain = OMX_PortDomainAudio;
	priv->port_para.format.audio.eEncoding = OMX_AUDIO_CodingPCM;
	priv->port_para.nBufferSize = AUDIO_OUTBUF_DEFAULT_SIZE;
	ret = OMX_SetParameter(priv->source, OMX_IndexParamPortDefinition,
			&priv->port_para);
	if (ret) {
		OSCL_LOGE("set port params error!");
		return ret;
	}

	memset(&audio_params, 0, sizeof(OMX_AUDIO_PARAM_PCMMODETYPE));
	audio_params.nChannels = TEST_CHANNELS;
	audio_params.nBitPerSample = TEST_FMT_WIDTH;
	audio_params.nSamplingRate = TEST_RATE;
	ret = OMX_SetParameter(priv->source, OMX_IndexParamAudioPcm,
			&audio_params);
	if (ret) {
		OSCL_LOGE("set audio record params error!");
		return ret;
	}

	port_def.nVersion.nVersion = OMX_VERSION;
	port_def.nPortIndex = AUDIO_OUTPUT_PORT_INDEX;
	ret = OMX_GetParameter(priv->source, OMX_IndexParamPortDefinition, &port_def);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("Error when getting OMX_PORT_PARAM_TYPE,%x\n", ret);
		return ret;
	}
	if ((priv->port_para.nBufferSize != port_def.nBufferSize) ||
		(priv->port_para.nBufferCountActual != port_def.nBufferCountActual)) {
		OSCL_LOGE("Error when getting OMX_PORT_PARAM_TYPE");
		return ret;
	}

	ret = alloc_buffer(priv, port_def.nBufferCountActual, port_def.nBufferSize);

	return ret;
}



static void untunel_exit(arecorder_test_t *priv)
{
	OMX_FreeHandle(priv->source);
	priv->source = NULL;
	OMX_FreeHandle(priv->sink);
	priv->sink = NULL;
	sem_destroy(&priv->sem_event);
	sem_destroy(&priv->sem_eos);
	OSCL_LOGD("here");
}


int omx_test_arec_untunnel(void)
{
	int i = 0;
	int ret = OMX_ErrorNone;
	arecorder_test_t *priv = &audio_rec_priv;

	OSCL_LOGD("here");
	memset(priv, 0, sizeof(arecorder_test_t));

	/* 1. get component handle */
	OMX_GetHandle((void **)&priv->source,
		"OMX.LB.SOURCE.AREC", priv, &untunnel_callbacks);
	OMX_GetHandle((void **)&priv->sink,
		"OMX.LB.SINK.FILE", priv, &untunnel_callbacks);
	OSCL_LOGD("here");
	if (priv->source == NULL || priv->sink == NULL) {
		OSCL_LOGE("get handle failed! %x, %x, %x", priv->source,
			priv->sink);
		OSCL_LOGE("here");
		return -1;
	}
	OSCL_LOGD("here");

	sem_init(&priv->sem_event, 0, 0);
	sem_init(&priv->sem_eos, 0, 0);

	/* 2. config source & sink component */
	ret = untunnel_config_source_component(priv);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);
	ret = untunnel_config_sink_component(priv);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	/* 3. set component stat to idle */
	ret = OMX_SendCommand(priv->source, OMX_CommandStateSet, OMX_StateIdle, NULL);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	sem_wait(&priv->sem_event);
	ret = OMX_SendCommand(priv->sink, OMX_CommandStateSet, OMX_StateIdle, NULL);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	sem_wait(&priv->sem_event);

	/* for mute record test */
	mute_record(priv, OMX_TRUE);

	/* 4. send buffer to source component queue */
	for (i = 0; i < priv->buf_num; i++) {
		ret = OMX_FillThisBuffer(priv->source, priv->buffer[i]);
		OSCL_LOGI("OMX_FillThisBuffer %p on port %i", priv->buffer[i],
			priv->buffer[i]->nOutputPortIndex);
		oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	}

	/* 5. set component stat to executing */
	ret = OMX_SendCommand(priv->source, OMX_CommandStateSet,
			OMX_StateExecuting, NULL);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	sem_wait(&priv->sem_event);
	ret = OMX_SendCommand(priv->sink, OMX_CommandStateSet, OMX_StateExecuting, NULL);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	sem_wait(&priv->sem_event);
#if 0
	while (1) {
		sem_wait(&priv->sem_event);
		OSCL_LOGI("error_value:%d", error_value);
		if (error_value != OMX_ErrorNone)
			break;
	}
	sem_wait(&priv->sem_eos);
#else
	rt_thread_delay(3500); /* display 15s */
	OSCL_LOGI("quit test...");
#endif

	/* 6. set component stat to idle */
	ret = OMX_SendCommand(priv->source, OMX_CommandStateSet, OMX_StateIdle, NULL);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	sem_wait(&priv->sem_event);
	ret = OMX_SendCommand(priv->sink, OMX_CommandStateSet, OMX_StateIdle, NULL);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	sem_wait(&priv->sem_event);

	/* 7. free buffer */
	free_buffer(priv);

	/* 8. set component stat to loaded */
	ret = OMX_SendCommand(priv->source, OMX_CommandStateSet, OMX_StateLoaded, NULL);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	sem_wait(&priv->sem_event);
	ret = OMX_SendCommand(priv->sink, OMX_CommandStateSet, OMX_StateLoaded, NULL);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	sem_wait(&priv->sem_event);

EXIT:
	untunel_exit(priv);
	return ret;
}

static omx_component_tbl_t omx_arec_component = {
	"OMX.LB.SOURCE.AREC",
	1,
	arec_component_init, NULL, NULL,
	{
		"source.areceiver",
		NULL
	}
};

static omx_component_tbl_t omx_writer_component = {
	"OMX.LB.SINK.FILE",
	1,
	file_writer_component_init, NULL, NULL,
	{
		"sink.filewriter",
		NULL
	}
};

int arec_component_test(int argc, char **argv)
{
	/* init omx and regster components */
	OMX_Init();
	omx_component_register(&omx_writer_component);
	omx_component_register(&omx_arec_component);

	/* test untunnel audio receiver component */
	omx_test_arec_untunnel();

	return 0;
}

MSH_CMD_EXPORT(arec_component_test, "arec_component_test");

