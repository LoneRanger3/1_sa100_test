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
#define DBG_LEVEL         DBG_WARNING

#include <oscl.h>
#include <base_component.h>

#define FILE_NAME_MAX_LENTH 512
typedef struct writer_private {
	int fd;
	char filename[FILE_NAME_MAX_LENTH];
} writer_private_t;
#define SINK_INPUT_PORT_INDEX 0
#define SINK_INPUT_DEFAULT_SIZE (16*1024)

void writer_buffer_handle(OMX_HANDLETYPE stand_com,
	OMX_BUFFERHEADERTYPE *inbuffer,
	OMX_BUFFERHEADERTYPE *outbuffer)
{
	writer_private_t *source_private;
	lb_omx_component_t *component;
	int len;

	component = get_lb_component(stand_com);
	source_private = component->component_private;
	if (source_private->fd < 0
		|| inbuffer == NULL
		|| inbuffer->nAllocLen == 0
		|| inbuffer->pBuffer == NULL) {
		OSCL_LOGE("fd :%d, inbuffer:%p, nAllocLen:%d, pBuffer:%p!!",
			source_private->fd, inbuffer,
			inbuffer->nAllocLen, inbuffer->pBuffer);
		OSCL_LOGE("something err, check it!!");
		return;
	}
	len = write(source_private->fd, inbuffer->pBuffer + inbuffer->nOffset,
			inbuffer->nFilledLen);
	OSCL_LOGI("write %d", len);
	inbuffer->nFilledLen -= len;
	inbuffer->nOffset += len;
	if (inbuffer->nFilledLen == 0)
		inbuffer->nOffset = 0;
}

void *writer_buffer_manager(void *param)
{
	lb_omx_component_t *component;
	OMX_COMPONENTTYPE *stand_comp = (OMX_COMPONENTTYPE *)param;
	base_port_t *inport;
	OMX_BUFFERHEADERTYPE *inbuffer = NULL;
	OSCL_TRACE(" %p\n", param);
	oscl_param_check((param != NULL), NULL, NULL);
	component = get_lb_component(stand_comp);
	inport = &component->port[SINK_INPUT_PORT_INDEX];
	OSCL_LOGI("======comp:%s, queue len:%d", get_component_name(component),
		oscl_queue_get_num(&inport->buffer_queue));
	component->dbg_flag = set_debug_state(component->dbg_flag,
			DEBUG_MSG_SHT, DEBUG_THREAD_START);

	while (component->state == OMX_StateIdle
		|| component->state == OMX_StateExecuting
		|| component->state == OMX_StatePause
		|| component->target_state == OMX_StateIdle) {
		component->dbg_wdog = 0;

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
			OSCL_LOGE("sem_wait component->flush_sem");
			sem_wait(component->flush_sem);
			OSCL_LOGE("after sem_wait component->flush_sem");
			OSCL_LOGI("comp:%s, queue len:%d,%p",
				get_component_name(component),
				oscl_queue_get_num(&inport->buffer_queue), inbuffer);
		}
		pthread_mutex_unlock(&component->flush_mutex);

		if (component->state != OMX_StateExecuting) {
			sem_wait(component->buf_mgnt_sem);
			continue;
		}

		if (inbuffer == NULL)
			inbuffer = oscl_queue_dequeue(&inport->buffer_queue);

		if (inbuffer == NULL) {
			OSCL_LOGI("sem_wait component->buf_mgnt_sem");
			sem_wait(component->buf_mgnt_sem);
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

		OSCL_LOGI("comp:%s, state:%d,handle:%x,nFilledLen:%d",
			get_component_name(component),
			component->state,
			component->buf_handle, inbuffer->nFilledLen);
		if (component->state == OMX_StateExecuting) {
			if (component->buf_handle && inbuffer->nFilledLen != 0)
				(*(component->buf_handle))(stand_comp, inbuffer, NULL);
		}
		base_check_mark(component, inbuffer);
		if (inbuffer->nFilledLen == 0)
			base_check_eos(component, inport, inbuffer);
		if (inbuffer->nFilledLen == 0
			|| component->target_state == OMX_StateLoaded) {
			inport->return_buffer(inport, inbuffer);
			inbuffer = NULL;
		}
	}
	if ((inbuffer != NULL) && (!PORT_IS_SUPPLIER(inport))) {
		OSCL_LOGI("inport return_buffer before exit");
		inport->return_buffer(inport, inbuffer);
	}
	OSCL_LOGW("exit from writer_buffer_manager:%s\n", rt_thread_self()->name);
	OSCL_TRACE(" %p\n", param);
	component->dbg_flag = set_debug_state(component->dbg_flag,
			DEBUG_MSG_SHT, DEBUG_THREAD_EXIT);
	pthread_exit(NULL);
	return NULL;
}

OMX_ERRORTYPE file_writer_set_parameter(OMX_IN OMX_HANDLETYPE hdl,
	OMX_IN OMX_INDEXTYPE omx_index,
	OMX_IN OMX_PTR data)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component;
	writer_private_t *private;
	OMX_PARAM_PORTDEFINITIONTYPE *port;

	OSCL_TRACE(" %p, %p , %d\n", hdl, data, omx_index);

	oscl_param_check((hdl != NULL) && (data != NULL),
		OMX_ErrorBadParameter, NULL);
	component = get_lb_component(hdl);
	oscl_param_check((component != NULL), OMX_ErrorBadParameter, NULL);
	private = component->component_private;

	switch (omx_index) {
	case omx_index_vendor_output_filename:
		private->filename[0] = '\0';
		private->filename[FILE_NAME_MAX_LENTH - 1] = '\0';
		if (data != NULL)
			strncpy(private->filename, data, FILE_NAME_MAX_LENTH - 1);
		if (private->fd != -1) {
			OSCL_LOGW("file not closed while set param");
			close(private->fd);
		}
		private->fd = open(private->filename, O_WRONLY | O_CREAT);
		OSCL_LOGW("====open file:%d", private->fd);
		break;

	case OMX_IndexParamPortDefinition:
		port = (OMX_PARAM_PORTDEFINITIONTYPE *)data;
		component->port[port->nPortIndex].port_param.eDomain = port->eDomain;
	default:
		ret = base_set_parameter(hdl, omx_index, data);
		break;
	}

	OSCL_TRACE(" %d\n", ret);
	return ret;
}

OMX_ERRORTYPE file_writer_set_state(OMX_HANDLETYPE hdl,
	OMX_U32 dest_state)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component;
	writer_private_t *private;

	OSCL_TRACE(" %p, %x\n", hdl, dest_state);
	oscl_param_check(hdl != NULL, OMX_ErrorBadParameter, NULL);
	component = get_lb_component(hdl);
	private = component->component_private;

	OSCL_LOGW("file_writer_set_state dest_state:%d", dest_state);
	if (dest_state == OMX_StateExecuting && component->state == OMX_StateIdle) {
		if (private->fd >= 0) {
			OSCL_LOGW("file not closed while entering StateExecuting");
			close(private->fd);
		}
		private->fd = open(private->filename, O_WRONLY | O_CREAT);
		OSCL_LOGW("====open file:%d", private->fd);
		if (private->fd < 0)
			return OMX_ErrorResourcesLost;
	}
	OSCL_LOGW("file_writer_set_state dest_state:%d", dest_state);
	ret = base_component_set_state(hdl, dest_state);
	OSCL_LOGW("file_writer_set_state dest_state:%d", dest_state);
	if (dest_state == OMX_StateIdle) {
		OSCL_LOGW("close file:%d", private->fd);
		if (private->fd >= 0) {
			OSCL_LOGW("closing file:%d", private->fd);
			close(private->fd);
			private->fd = -1;
		}
	}
	return ret;
}


OMX_ERRORTYPE file_writer_component_deinit(OMX_IN OMX_HANDLETYPE hdl)
{
	OMX_COMPONENTTYPE *base_cmp = (OMX_COMPONENTTYPE *)hdl;
	lb_omx_component_t *component = NULL;
	writer_private_t *private = NULL;

	OSCL_TRACE("base_cmp_handle:%p\n", hdl);
	oscl_param_check(hdl != NULL, OMX_ErrorBadParameter, NULL);
	component = (lb_omx_component_t *)base_cmp->pComponentPrivate;
	private = component->component_private;

	component->port[SINK_INPUT_PORT_INDEX].deinit(
		&component->port[SINK_INPUT_PORT_INDEX]);
	if (NULL != private) {
		oscl_free(private);
		component->component_private = NULL;
	}
	base_component_deinit(hdl);
	return OMX_ErrorNone;
}


OMX_ERRORTYPE file_writer_component_init(lb_omx_component_t *cmp_handle)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	writer_private_t *omx_writer_private;
	omx_writer_private = oscl_zalloc(sizeof(writer_private_t));
	oscl_param_check_exit((omx_writer_private != NULL),
		OMX_ErrorInsufficientResources, NULL);
	omx_writer_private->fd = -1;
	omx_writer_private->filename[0] = '\0';

	ret = base_component_init(cmp_handle);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	cmp_handle->name = "OMX.LB.SINK.FILE";
	cmp_handle->component_private = omx_writer_private;
	cmp_handle->buf_manager = writer_buffer_manager;
	cmp_handle->buf_handle = writer_buffer_handle;
	cmp_handle->base_comp.ComponentDeInit = file_writer_component_deinit;
	cmp_handle->base_comp.SetParameter = file_writer_set_parameter;
	cmp_handle->do_state_set = file_writer_set_state;

	cmp_handle->num_ports = 1;
	ret = base_port_init(cmp_handle, &cmp_handle->port[SINK_INPUT_PORT_INDEX],
			SINK_INPUT_PORT_INDEX,
			OMX_DirInput);
	if (ret != OMX_ErrorNone) {
		base_component_deinit(cmp_handle);
		cmp_handle->port[SINK_INPUT_PORT_INDEX].deinit(
			&cmp_handle->port[SINK_INPUT_PORT_INDEX]);
		return ret;
	}
	cmp_handle->port[SINK_INPUT_PORT_INDEX].port_param.nBufferSize =
		SINK_INPUT_DEFAULT_SIZE;
	cmp_handle->port[SINK_INPUT_PORT_INDEX].port_param.eDomain =
		OMX_PortDomainOther;
EXIT:
	if (ret != OMX_ErrorNone) {
		if (omx_writer_private != NULL)
			oscl_free(omx_writer_private);
	}
	OSCL_TRACE("%d\n", ret);
	return ret;
}

