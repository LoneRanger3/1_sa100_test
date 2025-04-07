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

#define FILE_NAME_MAX_LENTH 512
typedef struct reader_private {
	int fd;
	char filename[FILE_NAME_MAX_LENTH];
} reader_private_t;
#define SOURCE_OUTPUT_PORT_INDEX 0
#define SOURCE_OUTBUF_DEFAULT_SIZE (32*1024)

void reader_buffer_handle(OMX_HANDLETYPE stand_com,
	OMX_BUFFERHEADERTYPE *inbuffer,
	OMX_BUFFERHEADERTYPE *outbuffer)
{
	reader_private_t *source_private;
	lb_omx_component_t *component;

	component = get_lb_component(stand_com);
	source_private = component->component_private;
	if (source_private->fd < 0
		|| outbuffer == NULL
		|| outbuffer->nAllocLen == 0
		|| outbuffer->pBuffer == NULL) {
		OSCL_LOGE("fd :%d, inbuffer:%p, nAllocLen:%d, pBuffer:%p!!",
			source_private->fd,
			outbuffer, outbuffer->nAllocLen, outbuffer->pBuffer);
		OSCL_LOGE("err param!");
		return;
	}
	outbuffer->nOffset = 0;
	outbuffer->nFlags = 0;
	outbuffer->nFilledLen = read(source_private->fd, outbuffer->pBuffer,
			outbuffer->nAllocLen);
	OSCL_LOGI("outbuffer->nFilledLen:%d", outbuffer->nFilledLen);
	if (outbuffer->nFilledLen != outbuffer->nAllocLen) {
		OSCL_LOGE("reach the end of file:%s\n", source_private->filename);
		outbuffer->nFlags |= OMX_BUFFERFLAG_EOS;
	}
}

void *reader_buffer_manager(void *param)
{
	lb_omx_component_t *component;
	OMX_COMPONENTTYPE *stand_comp = (OMX_COMPONENTTYPE *)param;
	base_port_t *outport;
	OMX_BUFFERHEADERTYPE *outbuffer = NULL;
	OSCL_TRACE(" %p\n", param);
	oscl_param_check((param != NULL), NULL, NULL);
	component = get_lb_component(stand_comp);
	outport = &component->port[SOURCE_OUTPUT_PORT_INDEX];
	component->dbg_flag = set_debug_state(component->dbg_flag,
			DEBUG_MSG_SHT, DEBUG_THREAD_START);

	while (component->state == OMX_StateIdle
		|| component->state == OMX_StateExecuting
		|| component->state == OMX_StatePause
		|| component->target_state == OMX_StateIdle) {
		component->dbg_wdog = 0;

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

		if (component->state != OMX_StateExecuting) {
			sem_wait(component->buf_mgnt_sem);
			continue;
		}

		OSCL_LOGI("comp:%s, queue len:%d outbuf:%x",
			get_component_name(component),
			oscl_queue_get_num(&outport->buffer_queue), outbuffer);
		if (outbuffer == NULL) {
			outbuffer = oscl_queue_dequeue(&outport->buffer_queue);
			OSCL_LOGI("read buf manager:%x\n", outbuffer);
			if (outbuffer) {
				outbuffer->nFilledLen = 0;
				outbuffer->nFlags = 0;
			}
		}
		if (outbuffer == NULL) {
			OSCL_LOGI("sem_wait component->buf_mgnt_sem");
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

		OSCL_LOGI("component->state:%x,outbuffer->nAllocLen:%d",
			component->state,
			outbuffer->nAllocLen);
		if (component->state == OMX_StateExecuting) {
			if (component->buf_handle && outbuffer->nAllocLen != 0)
				(*(component->buf_handle))(stand_comp, NULL, outbuffer);
		}
		base_check_mark(component, outbuffer);
		base_check_eos(component, outport, outbuffer);
		if (outbuffer->nFilledLen != 0
			|| component->target_state == OMX_StateLoaded) {
			OSCL_LOGI("outport return_buffer");
			outport->return_buffer(outport, outbuffer);
			outbuffer = NULL;
		}
	}
	if ((outbuffer != NULL) && (!PORT_IS_SUPPLIER(outport))) {
		OSCL_LOGI("outport return_buffer before exit");
		outport->return_buffer(outport, outbuffer);
	}
	OSCL_LOGW("exit from reader_buffer_manager:%s\n", rt_thread_self()->name);
	OSCL_TRACE(" %p\n", param);
	component->dbg_flag = set_debug_state(component->dbg_flag,
			DEBUG_MSG_SHT, DEBUG_THREAD_EXIT);
	pthread_exit(NULL);
	return NULL;
}

OMX_ERRORTYPE file_reader_set_parameter(OMX_IN OMX_HANDLETYPE hComp,
	OMX_IN OMX_INDEXTYPE paramIndex,
	OMX_IN OMX_PTR paramData)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component;
	reader_private_t *private;
	OSCL_TRACE(" %p, %p , %d\n", hComp, paramData, paramIndex);

	oscl_param_check((hComp != NULL) && (paramData != NULL),
		OMX_ErrorBadParameter, NULL);
	component = get_lb_component(hComp);
	oscl_param_check((component != NULL), OMX_ErrorBadParameter, NULL);
	private = component->component_private;

	switch (paramIndex) {
	case omx_index_vendor_input_filename:
		private->filename[0] = '\0';
		private->filename[FILE_NAME_MAX_LENTH - 1] = '\0';
		if (paramData != NULL)
			strncpy(private->filename, paramData, FILE_NAME_MAX_LENTH - 1);
		if (private->fd != -1)
			close(private->fd);
		private->fd = open(private->filename, O_RDONLY);
		break;

	default:
		ret = base_set_parameter(hComp, paramIndex, paramData);
		break;
	}

	OSCL_TRACE(" %d\n", ret);
	return ret;
}


OMX_ERRORTYPE file_reader_set_state(OMX_HANDLETYPE hComp,
	OMX_U32 dest_state)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component;
	reader_private_t *private;

	OSCL_TRACE(" %p, %x\n", hComp, dest_state);
	oscl_param_check(hComp != NULL, OMX_ErrorBadParameter, NULL);
	component = get_lb_component(hComp);
	private = component->component_private;

	if (dest_state == OMX_StateExecuting && component->state == OMX_StateIdle) {
		if (private->fd >= 0) {
			OSCL_LOGW("file not closed while entering StateExecuting");
			close(private->fd);
		}
		private->fd = open(private->filename, O_RDONLY);
		if (private->fd < 0)
			return OMX_ErrorResourcesLost;

	}
	ret = base_component_set_state(hComp, dest_state);
	if (dest_state == OMX_StateIdle) {
		OSCL_LOGW("close file:%d", private->fd);
		if (private->fd >= 0) {
			int ret_fd;
			ret_fd = close(private->fd);
			OSCL_LOGW("close file:%d", ret_fd);
			private->fd = -1;
		}
	}
	return ret;
}

OMX_ERRORTYPE file_reader_component_deinit(OMX_IN OMX_HANDLETYPE hComponent)
{
	OMX_COMPONENTTYPE *base_cmp = (OMX_COMPONENTTYPE *)hComponent;
	lb_omx_component_t *component = NULL;
	reader_private_t *private = NULL;
	int ret;

	OSCL_TRACE("base_cmp_handle:%p\n", hComponent);
	oscl_param_check(hComponent != NULL, OMX_ErrorBadParameter, NULL);
	component = (lb_omx_component_t *)base_cmp->pComponentPrivate;
	private = component->component_private;

	component->port[SOURCE_OUTPUT_PORT_INDEX].deinit(
		&component->port[SOURCE_OUTPUT_PORT_INDEX]);

	if (NULL != private) {
		oscl_free(private);
		component->component_private = NULL;
	}
	ret = base_component_deinit(hComponent);
	return ret;
}


OMX_ERRORTYPE file_reader_component_init(lb_omx_component_t *cmp_handle)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	reader_private_t *omx_reader_private;
	omx_reader_private = oscl_zalloc(sizeof(reader_private_t));
	oscl_param_check_exit((omx_reader_private != NULL),
		OMX_ErrorInsufficientResources, NULL);
	omx_reader_private->fd = -1;
	omx_reader_private->filename[0] = '\0';

	ret = base_component_init(cmp_handle);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	cmp_handle->name = "OMX.LB.SOURCE.FILE";
	cmp_handle->component_private = omx_reader_private;
	cmp_handle->buf_manager = reader_buffer_manager;
	cmp_handle->buf_handle = reader_buffer_handle;
	cmp_handle->base_comp.ComponentDeInit = file_reader_component_deinit;
	cmp_handle->base_comp.SetParameter = file_reader_set_parameter;
	cmp_handle->do_state_set = file_reader_set_state;

	cmp_handle->num_ports = 1;
	ret = base_port_init(cmp_handle, &cmp_handle->port[SOURCE_OUTPUT_PORT_INDEX],
			SOURCE_OUTPUT_PORT_INDEX,
			OMX_DirOutput);
	if (ret != OMX_ErrorNone) {
		base_component_deinit(cmp_handle);
		cmp_handle->port[SOURCE_OUTPUT_PORT_INDEX].deinit(
			&cmp_handle->port[SOURCE_OUTPUT_PORT_INDEX]);
		return ret;
	}
	cmp_handle->port[SOURCE_OUTPUT_PORT_INDEX].port_param.nBufferSize =
		SOURCE_OUTBUF_DEFAULT_SIZE;
	cmp_handle->port[SOURCE_OUTPUT_PORT_INDEX].port_param.eDomain =
		OMX_PortDomainOther;
EXIT:
	if (ret != OMX_ErrorNone) {
		if (omx_reader_private != NULL)
			oscl_free(omx_reader_private);
	}
	OSCL_TRACE("%d\n", ret);
	return ret;
}

