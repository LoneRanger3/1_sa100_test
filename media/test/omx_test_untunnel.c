/*
 * omx_core.c - Standard functionality for the openmax core API.
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

#include <oscl.h>
#include <base_component.h>
#include <omx_test.h>

#define TEST_INPUT_BUF_NUM 3
#define TEST_OUTPUT_BUF_NUM 2

#define TEST_BUF_NUM 6

typedef struct untunnel_test {
	OMX_COMPONENTTYPE *source;
	OMX_COMPONENTTYPE *filter;
	OMX_COMPONENTTYPE *sink;
	char *in_filename;
	char *out_filename;
	int port_src_out;
	int port_sink_in;
	int port_filter_in;
	int port_filter_out;
	sem_t sem_event;
	sem_t sem_eos;
	int stat;
	int nbuf_done;
	int nread_buf;
	int nwrite_buf;
	OMX_BUFFERHEADERTYPE *reader_buf[TEST_BUF_NUM];
	OMX_BUFFERHEADERTYPE *writer_buf[TEST_BUF_NUM];
} untunnel_test_t;

untunnel_test_t untunnel_priv;

void __free_buffer(untunnel_test_t *priv)
{
	int i;
	int ret;
	for (i = 0; i < priv->nread_buf; i++) {
		if (priv->reader_buf[i]) {
			ret = OMX_FreeBuffer(priv->source,
					priv->port_src_out, priv->reader_buf[i]);
			if (ret != OMX_ErrorNone)
				OSCL_LOGE("port %d ,freebuffer:%d failed",
					OMX_DEFAULT_INPUT_PORT, i);
		}
		priv->reader_buf[i] = NULL;
	}

	for (i = 0; i < priv->nwrite_buf; i++) {
		if (priv->writer_buf[i]) {
			ret = OMX_FreeBuffer(priv->filter,
					priv->port_filter_out, priv->writer_buf[i]);
			OSCL_LOGE("here");
			if (ret != OMX_ErrorNone)
				OSCL_LOGE("port %d ,freebuffer:%d failed",
					OMX_DEFAULT_OUTPUT_PORT, i);
		}
		priv->writer_buf[i] = NULL;
	}

}

void __untunel_exit(untunnel_test_t *priv)
{
	__free_buffer(priv);
	OMX_FreeHandle(priv->filter);
	OMX_FreeHandle(priv->source);
	OMX_FreeHandle(priv->sink);
	priv->source = NULL;
	priv->sink = NULL;
	priv->filter = NULL;
	sem_destroy(&priv->sem_event);
	sem_destroy(&priv->sem_eos);
}

int __set_port_buf(OMX_COMPONENTTYPE *out_comp, int out_index,
	OMX_COMPONENTTYPE *in_comp, int in_index,
	OMX_BUFFERHEADERTYPE **header, int *count)
{
	int ret = OMX_ErrorNone;
	OMX_PARAM_PORTDEFINITIONTYPE outport, inport;
	int buf_num;
	int i;

	outport.nVersion.nVersion = OMX_VERSION;
	outport.nPortIndex = out_index;
	ret = OMX_GetParameter(out_comp, OMX_IndexParamPortDefinition, &outport);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("Error when getting OMX_PORT_PARAM_TYPE,%x\n", ret);
		return ret;
	}

	inport.nVersion.nVersion = OMX_VERSION;
	inport.nPortIndex = in_index;
	ret = OMX_GetParameter(in_comp, OMX_IndexParamPortDefinition, &inport);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("Error when getting OMX_PORT_PARAM_TYPE,%x\n", ret);
		return ret;
	}
	buf_num = outport.nBufferCountActual;
	if (buf_num < inport.nBufferCountActual)
		buf_num = inport.nBufferCountActual;
	if (buf_num > *count)
		buf_num = *count;

	if (outport.nBufferCountActual != buf_num) {
		outport.nBufferCountActual = buf_num;
		ret = OMX_SetParameter(out_comp,
				OMX_IndexParamPortDefinition, &outport);
		if (ret != OMX_ErrorNone) {
			OSCL_LOGE("Error when setting OMX_PORT_PARAM_TYPE\n");
			return ret;
		}
	}
	if (inport.nBufferCountActual != buf_num) {
		inport.nBufferCountActual = buf_num;
		ret = OMX_SetParameter(in_comp,
				OMX_IndexParamPortDefinition, &inport);
		if (ret != OMX_ErrorNone) {
			OSCL_LOGE("Error when setting OMX_PORT_PARAM_TYPE\n");
			return ret;
		}
	}
	*count = buf_num;

	for (i = 0; i < buf_num; i++) {
		header[i] = 0;
		ret = OMX_AllocateBuffer(out_comp, &header[i], out_index, &untunnel_priv,
				outport.nBufferSize);
		if (ret != OMX_ErrorNone) {
			OSCL_LOGE("Error on AllocateBuffer %p on port %i\n",
				&header[i], out_index);
			return ret;
		}
		header[i]->nInputPortIndex = in_index;
	}
	return ret;
}



/* Callbacks implementation */
OMX_ERRORTYPE untunnel_event_handler(
	OMX_HANDLETYPE hComponent,
	OMX_PTR pAppData,
	OMX_EVENTTYPE eEvent,
	OMX_U32 Data1,
	OMX_U32 Data2,
	OMX_PTR pEventData)
{
	untunnel_test_t *private = (untunnel_test_t *)pAppData;
	char *name;

	name = "filter";
	if (hComponent == private->sink)
		name = "sink";
	if (hComponent == private->source)
		name = "source";
	OSCL_LOGI("I am in untunnel_event_handler\n");
	if (eEvent == OMX_EventCmdComplete) {
		if (Data1 == OMX_CommandStateSet) {
			switch ((int)Data2) {
			case OMX_StateInvalid:
				OSCL_LOGI("StateSet OMX_StateInvalid");
				break;
			case OMX_StateLoaded:
				OSCL_LOGI("StateSet OMX_StateLoaded");
				break;
			case OMX_StateIdle:
				OSCL_LOGI("StateSet OMX_StateIdle");
				break;
			case OMX_StateExecuting:
				OSCL_LOGI("StateSet OMX_StateExecuting");
				break;
			case OMX_StatePause:
				OSCL_LOGI("StateSet OMX_StatePause");
				break;
			case OMX_StateWaitForResources:
				OSCL_LOGI("StateSet OMX_StateWaitForResources");
				break;
			default:
				OSCL_LOGI("StateSet unkown state");
				break;
			}
			sem_post(&private->sem_event);
		} else if (Data1 == OMX_CommandPortEnable) {
			OSCL_LOGI("OMX_EventCmdComplete OMX_CommandPortEnable");
			sem_post(&private->sem_event);
		} else if (Data1 == OMX_CommandPortDisable) {
			OSCL_LOGI("OMX_EventCmdComplete OMX_CommandPortDisable");
			sem_post(&private->sem_event);
		}
	} else if (eEvent == OMX_EventBufferFlag) {
		if ((int)Data2 == OMX_BUFFERFLAG_EOS) {
			OSCL_LOGE("%s BufferFlag OMX_BUFFERFLAG_EOS", name);
			if (hComponent == private->sink) {
				OSCL_LOGE("end of tunnel");
				sem_post(&untunnel_priv.sem_eos);
			}
		} else
			OSCL_LOGI("%s OMX_EventBufferFlag %x", name, Data2);
	} else {
		OSCL_LOGI("parm:%x %x", Data1, Data2);
	}

	return OMX_ErrorNone;
}


OMX_ERRORTYPE empty_buffer_done(
	OMX_HANDLETYPE hComponent,
	OMX_PTR pAppData,
	OMX_BUFFERHEADERTYPE *pBuffer)
{
	untunnel_test_t *priv = pAppData;
	OMX_COMPONENTTYPE *fill_component = NULL;
	char *name = "filter";

	if (pBuffer == NULL || pAppData == NULL) {
		OSCL_LOGE("err: buffer header is null");
		return OMX_ErrorBadParameter;
	}

	if (hComponent == priv->filter)
		fill_component = priv->source;
	if (hComponent == priv->sink) {
		fill_component = priv->filter;
		name = "sink";
	}
	OSCL_LOGE("%s:%d,%p %p, %d", name, pBuffer->nFilledLen, pBuffer,
		pBuffer->pBuffer, pBuffer->nFlags);
	if (fill_component != NULL)
		OMX_FillThisBuffer(fill_component, pBuffer);
	return OMX_ErrorNone;
}

OMX_ERRORTYPE fill_buffer_done(
	OMX_HANDLETYPE hComponent,
	OMX_PTR pAppData,
	OMX_BUFFERHEADERTYPE *pBuffer)
{
	untunnel_test_t *priv = pAppData;
	OMX_COMPONENTTYPE *next_component = NULL;
	char *name = "source";

	if (pBuffer == NULL || pAppData == NULL) {
		OSCL_LOGE("err: buffer header is null");
		return OMX_ErrorBadParameter;
	}
	if (hComponent == priv->source)
		next_component = priv->filter;
	if (hComponent == priv->filter) {
		next_component = priv->sink;
		name = "filter";
	}
	if (next_component != NULL) {
		OSCL_LOGI("%s:%d %p %p, %d", name, pBuffer->nFilledLen,
			pBuffer, pBuffer->pBuffer, pBuffer->nFlags);
		OMX_EmptyThisBuffer(next_component, pBuffer);
	} else {
		OSCL_LOGE("%s: next_component is null", name);
	}
	return OMX_ErrorNone;
}

OMX_CALLBACKTYPE untunnel_callbacks = { .EventHandler = untunnel_event_handler,
			 .EmptyBufferDone = empty_buffer_done,
			 .FillBufferDone = fill_buffer_done,
		 };


int openmax_test_untunel(void)
{
	int ret = OMX_ErrorNone;
	int i;

	ret = OMX_GetHandle((void **)&untunnel_priv.filter,
			"OMX.LB.SPLITTER.BASE", &untunnel_priv, &untunnel_callbacks);
	ret = OMX_GetHandle((void **)&untunnel_priv.source,
			"OMX.LB.SOURCE.FILE", &untunnel_priv, &untunnel_callbacks);
	ret = OMX_GetHandle((void **)&untunnel_priv.sink,
			"OMX.LB.SINK.FILE", &untunnel_priv, &untunnel_callbacks);
	if (untunnel_priv.source == NULL
		|| untunnel_priv.sink == NULL
		|| untunnel_priv.filter == NULL) {
		OSCL_LOGE("get handle failed! %x, %x, %x",
			untunnel_priv.source,
			untunnel_priv.sink,
			untunnel_priv.filter);
		goto EXIT;
	}


	untunnel_priv.in_filename = TEST_IN_FILE;
	untunnel_priv.out_filename = TEST_OUT_FILE;
	untunnel_priv.stat = 0;

	sem_init(&untunnel_priv.sem_event, 0, 0);
	sem_init(&untunnel_priv.sem_eos, 0, 0);
	untunnel_priv.nbuf_done = 0;
	rt_kprintf("%s %d\n", __FILE__, __LINE__);

	untunnel_priv.port_src_out = get_port_index(untunnel_priv.source,
			OMX_DirOutput, OMX_PortDomainOther, 0);
	untunnel_priv.port_sink_in = get_port_index(untunnel_priv.sink,
			OMX_DirInput, OMX_PortDomainOther, 0);
	untunnel_priv.port_filter_in = get_port_index(untunnel_priv.filter,
			OMX_DirInput, OMX_PortDomainOther, 0);
	untunnel_priv.port_filter_out = get_port_index(untunnel_priv.filter,
			OMX_DirOutput, OMX_PortDomainOther, 0);
	if (untunnel_priv.port_src_out < 0 || untunnel_priv.port_sink_in < 0
		|| untunnel_priv.port_filter_in < 0
		|| untunnel_priv.port_filter_out < 0) {
		goto EXIT;
	}
	ret = OMX_SendCommand(untunnel_priv.filter,
			OMX_CommandPortEnable, untunnel_priv.port_filter_out, NULL);


	untunnel_priv.nread_buf = TEST_BUF_NUM;
	ret = __set_port_buf(untunnel_priv.source, untunnel_priv.port_src_out,
			untunnel_priv.filter, untunnel_priv.port_filter_in,
			untunnel_priv.reader_buf, &untunnel_priv.nread_buf);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("Error when __set_port_buf\n");
		ret = -1;
		goto EXIT;
	}

	untunnel_priv.nwrite_buf = TEST_BUF_NUM;
	ret = __set_port_buf(untunnel_priv.filter, untunnel_priv.port_filter_out,
			untunnel_priv.sink, untunnel_priv.port_sink_in,
			untunnel_priv.writer_buf, &untunnel_priv.nwrite_buf);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("Error when __set_port_buf\n");
		ret = -1;
		goto EXIT;
	}
	OSCL_LOGE("here");

	ret = OMX_SetParameter(untunnel_priv.source, omx_index_vendor_input_filename,
			untunnel_priv.in_filename);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);
	ret = OMX_SetParameter(untunnel_priv.sink, omx_index_vendor_output_filename,
			untunnel_priv.out_filename);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	OSCL_LOGE("here");

	ret = OMX_SendCommand(untunnel_priv.source,
			OMX_CommandStateSet, OMX_StateIdle, NULL);
	oscl_param_check_exit((ret == OMX_ErrorNone), -1, NULL);

	ret = OMX_SendCommand(untunnel_priv.filter,
			OMX_CommandStateSet, OMX_StateIdle, NULL);
	oscl_param_check_exit((ret == OMX_ErrorNone), -1, NULL);

	ret = OMX_SendCommand(untunnel_priv.sink,
			OMX_CommandStateSet, OMX_StateIdle, NULL);
	oscl_param_check_exit((ret == OMX_ErrorNone), -1, NULL);
	OSCL_LOGE("here");

	sem_wait(&untunnel_priv.sem_event);
	sem_wait(&untunnel_priv.sem_event);
	sem_wait(&untunnel_priv.sem_event);
	OSCL_LOGE("here");

	ret = OMX_SendCommand(untunnel_priv.source, OMX_CommandStateSet,
			OMX_StateExecuting, NULL);
	oscl_param_check_exit((ret == OMX_ErrorNone), -1, NULL);

	ret = OMX_SendCommand(untunnel_priv.filter, OMX_CommandStateSet,
			OMX_StateExecuting, NULL);
	oscl_param_check_exit((ret == OMX_ErrorNone), -1, NULL);

	ret = OMX_SendCommand(untunnel_priv.sink, OMX_CommandStateSet,
			OMX_StateExecuting, NULL);
	oscl_param_check_exit((ret == OMX_ErrorNone), -1, NULL);
	sem_wait(&untunnel_priv.sem_event);
	sem_wait(&untunnel_priv.sem_event);
	sem_wait(&untunnel_priv.sem_event);

	for (i = 0; i < untunnel_priv.nwrite_buf; i++) {
		ret = OMX_FillThisBuffer(untunnel_priv.filter,
				untunnel_priv.writer_buf[i]);
		OSCL_LOGE("OMX_FillThisBuffer %p on port %i\n",
			untunnel_priv.writer_buf[i],
			untunnel_priv.writer_buf[i]->nOutputPortIndex);
		oscl_param_check_exit((ret == OMX_ErrorNone), -1, NULL);
	}

	for (i = 0; i < untunnel_priv.nread_buf; i++) {
		ret = OMX_FillThisBuffer(untunnel_priv.source,
				untunnel_priv.reader_buf[i]);
		OSCL_LOGE("OMX_FillThisBuffer %p on port %i\n",
			untunnel_priv.reader_buf[i],
			untunnel_priv.reader_buf[i]->nOutputPortIndex);
		oscl_param_check_exit((ret == OMX_ErrorNone), -1, NULL);
	}


	sem_wait(&untunnel_priv.sem_eos);
	OSCL_LOGE("here");

	OSCL_LOGE("%d", (sem_getvalue(&untunnel_priv.sem_event, &i), i));
	while (sem_trywait(&untunnel_priv.sem_event) == 0)
		continue;
	OSCL_LOGE("here");
	OMX_SendCommand(untunnel_priv.sink,
		OMX_CommandStateSet, OMX_StateIdle, NULL);
	OMX_SendCommand(untunnel_priv.filter,
		OMX_CommandStateSet, OMX_StateIdle, NULL);
	OMX_SendCommand(untunnel_priv.source,
		OMX_CommandStateSet, OMX_StateIdle, NULL);
	OSCL_LOGE("here");
	sem_wait(&untunnel_priv.sem_event);
	sem_wait(&untunnel_priv.sem_event);
	sem_wait(&untunnel_priv.sem_event);

	__free_buffer(&untunnel_priv);

	OMX_SendCommand(untunnel_priv.sink,
		OMX_CommandStateSet, OMX_StateLoaded, NULL);
	OMX_SendCommand(untunnel_priv.filter,
		OMX_CommandStateSet, OMX_StateLoaded, NULL);
	OMX_SendCommand(untunnel_priv.source,
		OMX_CommandStateSet, OMX_StateLoaded, NULL);
	sem_wait(&untunnel_priv.sem_event);
	sem_wait(&untunnel_priv.sem_event);
	sem_wait(&untunnel_priv.sem_event);
	OSCL_LOGE("here");

EXIT:
	__untunel_exit(&untunnel_priv);
	return ret;
}

