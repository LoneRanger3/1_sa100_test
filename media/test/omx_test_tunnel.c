/*
 * omx_test_tunnel.c - test tunnel mod of omx component.
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
#define DBG_LEVEL		0

#include <oscl.h>
#include <base_component.h>
#include <omx_test.h>
#include <lb_omx_core.h>

#define TEST_INPUT_BUF_NUM 3
#define TEST_OUTPUT_BUF_NUM 2

#define TEST_STATE_END 0x1
#define TEST_STATE_WANT_PAUSE 0x2
#define TEST_STATE_EXCUTING 0x2

typedef struct tunnel_test {
	OMX_COMPONENTTYPE *source;
	OMX_COMPONENTTYPE *filter;
	OMX_COMPONENTTYPE *sink;
	int port_src_out;
	int port_sink_in;
	int port_filter_in;
	int port_filter_out;
	char *in_filename;
	char *out_filename;

	int evt_thread_id;
	pthread_t evt_thread;
	oscl_queue_t evt_queue;

	sem_t sem_event;
	sem_t sem_eos;

	int stat;
	int nbuf_done;
	OMX_BUFFERHEADERTYPE *source_buf[4];
	OMX_BUFFERHEADERTYPE *sink_buf[4];
} tunnel_test_t;

tunnel_test_t tunnel_priv;

void __tunnel_exit(tunnel_test_t *priv)
{

	if (priv->evt_thread_id != -1) {
		oscl_queue_flush(&priv->evt_queue);
		sem_post(&priv->sem_event);
		pthread_join(priv->evt_thread, NULL);
		priv->evt_thread_id = -1;
	}

	if (priv->source)
		OMX_FreeHandle(priv->source);
	if (priv->sink)
		OMX_FreeHandle(priv->sink);
	if (priv->filter)
		OMX_FreeHandle(priv->filter);
	priv->source = priv->sink = priv->filter = NULL;
	sem_destroy(&priv->sem_event);
	sem_destroy(&priv->sem_eos);
	oscl_queue_deinit(&priv->evt_queue);

	OSCL_LOGE("here");
}



/* Callbacks implementation */
OMX_ERRORTYPE tunnel_event_handler(
	OMX_HANDLETYPE hComponent,
	OMX_PTR pAppData,
	OMX_EVENTTYPE eEvent,
	OMX_U32 Data1,
	OMX_U32 Data2,
	OMX_PTR pEventData)
{
	tunnel_test_t *private = (tunnel_test_t *)pAppData;
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
				sem_post(&tunnel_priv.sem_eos);
			}
		} else
			OSCL_LOGI("%s OMX_EventBufferFlag %x", name, Data2);
	} else {
		OSCL_LOGI("%s parm:%x %x", name, Data1, Data2);
	}

	return OMX_ErrorNone;
}


OMX_CALLBACKTYPE tunnel_callbacks = { .EventHandler = tunnel_event_handler,
			 .EmptyBufferDone = NULL,
			 .FillBufferDone = NULL,
		 };

int openmax_test_tunnel(void)
{
	int ret = OMX_ErrorNone;
	OSCL_LOGE("here");

	memset(&tunnel_priv, 0, sizeof(tunnel_test_t));
	tunnel_priv.evt_thread_id = -1;
	OMX_GetHandle((void **)&tunnel_priv.source,
		"OMX.LB.SOURCE.FILE", &tunnel_priv, &tunnel_callbacks);
	OMX_GetHandle((void **)&tunnel_priv.sink,
		"OMX.LB.SINK.FILE", &tunnel_priv, &tunnel_callbacks);
	OMX_GetHandle((void **)&tunnel_priv.filter,
		"OMX.LB.FILTER", &tunnel_priv, &tunnel_callbacks);
	OSCL_LOGE("here");
	if (tunnel_priv.source == NULL || tunnel_priv.sink == NULL
		|| tunnel_priv.filter == NULL) {
		OSCL_LOGE("get handle failed! %x, %x, %x", tunnel_priv.source,
			tunnel_priv.sink, tunnel_priv.filter);
		OSCL_LOGE("here");
		goto EXIT;
	}
	tunnel_priv.in_filename = TEST_IN_FILE;
	tunnel_priv.out_filename = TEST_OUT_FILE;
	OSCL_LOGE("here");


	tunnel_priv.stat = 0;
	sem_init(&tunnel_priv.sem_event, 0, 0);
	sem_init(&tunnel_priv.sem_eos, 0, 0);
	oscl_queue_init(&tunnel_priv.evt_queue);

	tunnel_priv.port_src_out = get_port_index(tunnel_priv.source, OMX_DirOutput,
			OMX_PortDomainOther, 0);
	tunnel_priv.port_sink_in = get_port_index(tunnel_priv.sink, OMX_DirInput,
			OMX_PortDomainOther, 0);
	tunnel_priv.port_filter_in = get_port_index(tunnel_priv.filter, OMX_DirInput,
			OMX_PortDomainOther, 0);
	tunnel_priv.port_filter_out = get_port_index(tunnel_priv.filter,
			OMX_DirOutput, OMX_PortDomainOther, 0);
	if (tunnel_priv.port_src_out < 0 || tunnel_priv.port_sink_in < 0
		|| tunnel_priv.port_filter_in < 0 || tunnel_priv.port_filter_out < 0) {
		goto EXIT;
	}
	ret = OMX_SetParameter(tunnel_priv.source, omx_index_vendor_input_filename,
			tunnel_priv.in_filename);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);
	ret = OMX_SetParameter(tunnel_priv.sink, omx_index_vendor_output_filename,
			tunnel_priv.out_filename);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);


	ret = OMX_SetupTunnel(tunnel_priv.source, tunnel_priv.port_src_out,
			tunnel_priv.filter, tunnel_priv.port_filter_in);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);

	ret = OMX_SetupTunnel(tunnel_priv.filter, tunnel_priv.port_filter_out,
			tunnel_priv.sink, tunnel_priv.port_sink_in);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);


	ret = OMX_SendCommand(tunnel_priv.source, OMX_CommandStateSet, OMX_StateIdle,
			NULL);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	ret = OMX_SendCommand(tunnel_priv.sink, OMX_CommandStateSet, OMX_StateIdle,
			NULL);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	ret = OMX_SendCommand(tunnel_priv.filter, OMX_CommandStateSet, OMX_StateIdle,
			NULL);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	sem_wait(&tunnel_priv.sem_event);
	sem_wait(&tunnel_priv.sem_event);
	sem_wait(&tunnel_priv.sem_event);

	ret = OMX_SendCommand(tunnel_priv.source, OMX_CommandStateSet,
			OMX_StateExecuting, NULL);
	ret = OMX_SendCommand(tunnel_priv.sink, OMX_CommandStateSet,
			OMX_StateExecuting, NULL);
	ret = OMX_SendCommand(tunnel_priv.filter, OMX_CommandStateSet,
			OMX_StateExecuting, NULL);
	sem_wait(&tunnel_priv.sem_event);
	sem_wait(&tunnel_priv.sem_event);
	sem_wait(&tunnel_priv.sem_event);
	OSCL_LOGE("here");

	sem_wait(&tunnel_priv.sem_eos);
	OSCL_LOGE("here");

	ret = OMX_SendCommand(tunnel_priv.source, OMX_CommandStateSet, OMX_StateIdle,
			NULL);
	ret = OMX_SendCommand(tunnel_priv.sink, OMX_CommandStateSet, OMX_StateIdle,
			NULL);
	ret = OMX_SendCommand(tunnel_priv.filter, OMX_CommandStateSet, OMX_StateIdle,
			NULL);
	sem_wait(&tunnel_priv.sem_event);
	sem_wait(&tunnel_priv.sem_event);
	sem_wait(&tunnel_priv.sem_event);
	OSCL_LOGE("here");

EXIT:
	if (tunnel_priv.source)
		ret = OMX_SendCommand(tunnel_priv.source, OMX_CommandStateSet,
				OMX_StateLoaded, NULL);
	if (tunnel_priv.sink)
		ret = OMX_SendCommand(tunnel_priv.sink, OMX_CommandStateSet,
				OMX_StateLoaded, NULL);
	if (tunnel_priv.filter)
		ret = OMX_SendCommand(tunnel_priv.filter, OMX_CommandStateSet,
				OMX_StateLoaded, NULL);
	if (tunnel_priv.source)
		sem_wait(&tunnel_priv.sem_event);
	if (tunnel_priv.sink)
		sem_wait(&tunnel_priv.sem_event);
	if (tunnel_priv.filter)
		sem_wait(&tunnel_priv.sem_event);
	__tunnel_exit(&tunnel_priv);
	OSCL_LOGE("here");
	return ret;
}

