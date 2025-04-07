/*
 * splitter_component.c - code of splitter component.
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
#include <lb_omx_as_string.h>


#define SPLITTER_MAX_BUF_NUM 16
typedef struct splitter_private {
	pthread_mutex_t lock;
	OMX_S32 outport_use_cb[OMX_PORT_NUMBER_SUPPORTED];
	int use_map[SPLITTER_MAX_BUF_NUM];
} splitter_private_t;

int _splitter_init_buffer(lb_omx_component_t *lb_cmp_hdl)
{
	int i;
	base_port_t *port;
	int ret = 0;
	int buf_count;

	OSCL_TRACE("\n");
	buf_count = SPLITTER_MAX_BUF_NUM;
	for (i = 0; i < lb_cmp_hdl->num_ports; i++) {
		port = &lb_cmp_hdl->port[i];
		if (NULL == port->header) {
			port->header = (OMX_BUFFERHEADERTYPE **) oscl_zalloc(
					buf_count * sizeof(OMX_BUFFERHEADERTYPE *));
			oscl_param_check_exit((port->header != NULL),
				OMX_ErrorInsufficientResources, NULL);
		}
		if (NULL == port->buf_state) {
			port->buf_state = (OMX_U32 *)oscl_zalloc(buf_count * sizeof(u32));
			oscl_param_check_exit((port->buf_state != NULL),
				OMX_ErrorInsufficientResources, NULL);
		}
	}
EXIT:
	OSCL_TRACE("\n");
	return ret;

}


/**
 * splitter_inport_allocate_buf - inport allocate buffer will allcate buffers and return
 * the header. The buffer will share to output port.
 *
 * @port: input port
 * @buf_header:	output buf_header
 * @private: private data used by callbacks
 * @size: buffersize
 *
 * Returns 0 on success, -EERROR otherwise..
 */
OMX_ERRORTYPE splitter_inport_allocate_buf(base_port_t *port,
	OMX_BUFFERHEADERTYPE **buf_header,
	OMX_PTR private,
	OMX_U32 size)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	splitter_private_t *splitter;
	lb_omx_component_t *component;
	int i;

	OSCL_TRACE("\n");
	oscl_param_check((private != NULL && port != NULL && buf_header != NULL),
		OMX_ErrorBadParameter, NULL);
	oscl_param_check(port->port_param.nPortIndex == OMX_DEFAULT_INPUT_PORT,
		OMX_ErrorBadParameter, NULL);
	component = (lb_omx_component_t *)port->component;
	splitter = component->component_private;

	ret = base_port_allocate_buf(port, buf_header, private, size);
	oscl_param_check(ret == OMX_ErrorNone, ret, NULL);

	/* find index in header array, initialsize use_map to 0 */
	for (i = 0; i < port->port_param.nBufferCountActual; i++) {
		if (port->header[i] == *buf_header)
			break;
	}
	if (i == port->port_param.nBufferCountActual) {
		OSCL_LOGE("buffer in splitter is not null! check it!");
		ret = OMX_ErrorUndefined;
		goto EXIT;
	}
	pthread_mutex_lock(&splitter->lock);
	splitter->use_map[i] = 0;
	pthread_mutex_unlock(&splitter->lock);

EXIT:
	OSCL_TRACE("\n");
	return ret;
}

/**
 * splitter_outport_allocate_buf - outport allocate buffer will only allocat a buffer
 * header. If input buffer has allocate buffer or recive buffer before this func,
 * the header will share the buffer in the input port. otherwise, the head will
 * contain a null buffer.
 * Before use a output port, we must call this function to get buffer headers.
 *
 * @port: output port
 * @buf_header:	output buf_header
 * @private: private data used by callbacks
 * @size: buffersize
 *
 * Returns 0 on success, -EERROR otherwise..
 */
OMX_ERRORTYPE splitter_outport_allocate_buf(base_port_t *port,
	OMX_BUFFERHEADERTYPE **buf_header,
	OMX_PTR private,
	OMX_U32 size)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component;
	base_port_t *inport;
	OMX_U8 *buffer = NULL;
	OSCL_TRACE("\n");

	oscl_param_check((private != NULL && port != NULL && buf_header != NULL),
		OMX_ErrorBadParameter, NULL);
	oscl_param_check(port->port_param.nPortIndex != OMX_DEFAULT_INPUT_PORT,
		OMX_ErrorBadParameter, NULL);

	component = (lb_omx_component_t *)port->component;
	inport = &component->port[OMX_DEFAULT_INPUT_PORT];
	if (inport->header && (port->num_assigned < inport->num_assigned)
		&& inport->header[port->num_assigned])
		buffer = inport->header[port->num_assigned]->pBuffer;
	oscl_param_check(buffer != NULL, OMX_ErrorBadParameter, NULL);
	ret = base_port_use_buffer(port, buf_header, private, size, buffer);
	OSCL_TRACE("port:%x(%d) index:%d, buf_header:%x buf_header->pBuffer:%x\n",
		port, port->port_param.nPortIndex, port->num_assigned,
		*buf_header, (*buf_header)->pBuffer);
	OSCL_TRACE("\n");
	return ret;
}

/**
 * splitter_port_use_buffer - only input port can call this func and output port will
 * return OMX_ErrorBadParameter. The buffer will saved in inputport and share to
 * output ports.
 *
 * @port: input port
 * @buf_header:	output buf_header
 * @private: private data used by callbacks
 * @buffer: buffer
 *
 * Returns 0 on success, -EERROR otherwise..
 */
OMX_ERRORTYPE splitter_port_use_buffer(base_port_t *port,
	OMX_BUFFERHEADERTYPE **buf_header,
	OMX_PTR private,
	OMX_U32 size,
	OMX_U8 *buffer)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	splitter_private_t *splitter;
	lb_omx_component_t *component;
	int i;
	OSCL_TRACE("\n");
	oscl_param_check((private != NULL && port != NULL
			&& buf_header != NULL && buffer != NULL),
		OMX_ErrorBadParameter, NULL);
	oscl_param_check(port->port_param.nPortIndex == OMX_DEFAULT_INPUT_PORT,
		OMX_ErrorBadParameter, NULL);

	component = (lb_omx_component_t *)port->component;
	splitter = component->component_private;
	ret = base_port_use_buffer(port, buf_header, private, size, buffer);
	oscl_param_check(ret == OMX_ErrorNone, ret, NULL);

	for (i = 0; i < port->port_param.nBufferCountActual; i++) {
		if (port->header[i] == *buf_header)
			break;
	}
	if (i == port->port_param.nBufferCountActual) {
		OSCL_LOGE("buffer in splitter is not null! check it!");
		ret = OMX_ErrorUndefined;
		goto EXIT;
	}
	pthread_mutex_lock(&splitter->lock);
	splitter->use_map[i] = 0;
	pthread_mutex_unlock(&splitter->lock);
	OSCL_TRACE("port:%x(%d) index:%d, buf_header:%x buf_header->pBuffer:%x\n",
		port, port->port_param.nPortIndex, i,
		*buf_header, (*buf_header)->pBuffer);

EXIT:
	OSCL_TRACE("\n");
	return ret;
}

/**
 * splitter_port_free_buffer - port buffer should freed in outport before input port.
 *
 * @port: port
 * @buf_header:	buf_header to be freed
 *
 * Returns 0 on success, -EERROR otherwise..
 */
OMX_ERRORTYPE splitter_port_free_buffer(base_port_t *port,
	OMX_BUFFERHEADERTYPE *buf_header)
{
	unsigned int i;
	lb_omx_component_t *component;
	splitter_private_t *splitter;
	int mask;
	int ret;

	OSCL_TRACE("\n");
	oscl_param_check(((port != NULL) && (buf_header != NULL)),
		OMX_ErrorBadParameter, NULL);

	component = (lb_omx_component_t *)port->component;
	splitter = component->component_private;


	mask = (1 << port->port_param.nPortIndex);
	pthread_mutex_lock(&splitter->lock);
	for (i = 0; i < port->port_param.nBufferCountActual; i++) {
		if (buf_header == port->header[i])
			break;
	}
	oscl_param_check_exit((i != port->port_param.nBufferCountActual),
		OMX_ErrorBadParameter, NULL);
	splitter->use_map[i] = splitter->use_map[i] & ~mask;
	if (splitter->use_map[i] != 0
		&& port->port_param.nPortIndex == OMX_DEFAULT_INPUT_PORT)
		OSCL_LOGE("free buffer while used(%x), check it!", splitter->use_map);
	ret = base_port_free_buffer(port, buf_header);

EXIT:
	pthread_mutex_unlock(&splitter->lock);
	OSCL_TRACE("\n");
	return ret;
}


/**
 * Releases buffers under processing..
 * This function must be implemented in the derived classes, for the
 * specific processing
 *
 * @port: port
 *
 * Returns 0 on success, -EERROR otherwise..
 */
OMX_ERRORTYPE spliter_outport_flush_buffer(base_port_t *port)
{
	OMX_ERRORTYPE ret;
	int i;
	lb_omx_component_t *component;
	splitter_private_t *splitter;
	base_port_t *inport;
	int isbusy;

	OSCL_TRACE("\n");
	ret = base_port_flush_buffer(port);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);
	oscl_param_check_exit(port->header != NULL, ret, NULL);
	for (i = 0; i < port->port_param.nBufferCountActual; i++) {
		if (port->header[i])
			port->header[i]->pBuffer = NULL;
	}
	component = (lb_omx_component_t *)port->component;
	splitter = component->component_private;
	inport = &component->port[OMX_DEFAULT_INPUT_PORT];
	pthread_mutex_lock(&splitter->lock);
	for (i = 0; i < port->port_param.nBufferCountActual; i++) {
		isbusy = splitter->use_map[i] & (1 << port->port_param.nPortIndex);
		if (isbusy == 0)
			continue;
		OSCL_LOGE("buf[%d]:%x is busy after flush port!!", i);
		splitter->use_map[i] &= ~(1 << port->port_param.nPortIndex);
		if (inport->header[i] == NULL || inport->header[i]->pBuffer == NULL)
			continue;
		if (isbusy && splitter->use_map[i] == 0) {
			inport->header[i]->nFilledLen = 0;
			inport->header[i]->nFlags = 0;
			OSCL_LOGE("return_buffer:%x:%x\n",
				inport->return_buffer, inport->header[i]);
			inport->return_buffer(inport, inport->header[i]);
		}
	}
	pthread_mutex_unlock(&splitter->lock);

EXIT:
	OSCL_TRACE("\n");
	return ret;
}

/**
 * Get index of header array in input port.  If can not find, save it to array
 *
 * @component: component
 * @buffer: buffer header
 *
 * Returns index on success, -EERROR otherwise..
 */
int _get_buffer_index(lb_omx_component_t *component, OMX_BUFFERHEADERTYPE *buffer)
{
	int buf_index = -1;
	base_port_t *port;
	int null_buffer = -1;
	int buf_count;

	OSCL_TRACE("\n");
	port = &component->port[OMX_DEFAULT_INPUT_PORT];
	if (port->header == NULL)
		_splitter_init_buffer(component);
	oscl_param_check(port->header != NULL, -1, NULL);

	/* find in input port buffer array first */
	buf_count = port->port_param.nBufferCountActual;
	for (buf_index = 0; buf_index < buf_count; buf_index++) {
		if (null_buffer == -1 && port->header[buf_index] == NULL)
			null_buffer = buf_index;
		if (port->header[buf_index] == buffer) {
			OSCL_LOGI("header[%d]:%x,flag:%x",
				buf_index, port->header[buf_index], buffer->nFlags);
			goto EXIT;
		}
	}

	/*not find in splitter, save it to splitter buffer array if there has a null */
	buf_index = -1;
	if (null_buffer != -1) {
		buf_index = null_buffer;
		port->header[buf_index] = buffer;
		port->num_assigned++;
	}
	OSCL_LOGE("buf_index:%x, header:%x, null:%d",
		buf_index, port->header[buf_index], null_buffer);
EXIT:
	OSCL_TRACE("\n");
	return buf_index;
}

/**
 * splitter_share_buffer - share buffers to all enabled outport.
 *
 * @component: component
 * @inbuffer: buf_header to be shared
 *
 * Returns: counts of outports who share this buffer
 */
int splitter_share_buffer(lb_omx_component_t *component,
	OMX_BUFFERHEADERTYPE *inbuffer)
{
	splitter_private_t *splitter;
	int buf_index;
	int port_index;
	int ret;
	int count = 0;
	base_port_t *port;
	OMX_BUFFERHEADERTYPE *outbuffer = NULL;
	OSCL_TRACE("%x %x\n", inbuffer, inbuffer->pBuffer);

	splitter = component->component_private;

	/* get buffer index in array. index is the same in splitter and all outports */
	buf_index = _get_buffer_index(component, inbuffer);
	oscl_param_check_exit(buf_index != -1, 0, NULL);

	pthread_mutex_lock(&splitter->lock);

	/* to prevent input port return buffer while share it to outputs */
	splitter->use_map[buf_index] |= (1 << OMX_DEFAULT_INPUT_PORT);

	/* if outport is enabled and has malloc header for this buffer, share the buf */
	for (port_index = OMX_DEFAULT_OUTPUT_PORT; port_index < component->num_ports;
		port_index++) {
		port = &component->port[port_index];
		if (!PORT_IS_ENABLED(port))
			continue;
		OSCL_TRACE("port_index:%d\n", port_index);
		if ((port->header == NULL) || (port->header[buf_index] == NULL)) {
			OSCL_LOGE("err! outport buffer header uninitialised");
			continue;
		}
		outbuffer = port->header[buf_index];
		if (outbuffer->pBuffer == NULL)
			outbuffer->pBuffer = inbuffer->pBuffer;

		if (outbuffer->pBuffer != inbuffer->pBuffer) {
			OSCL_LOGE("bufindex:%d, outbuf(%x %x) != inbuffer(%x %x)!",
				buf_index, outbuffer, outbuffer->pBuffer,
				inbuffer, inbuffer->pBuffer);
			continue;
		}

		outbuffer->nFilledLen = inbuffer->nFilledLen;
		outbuffer->nOffset = inbuffer->nOffset;
		outbuffer->nFlags = inbuffer->nFlags;
		outbuffer->nTickCount = inbuffer->nTickCount;
		outbuffer->nTimeStamp = inbuffer->nTimeStamp;
		outbuffer->hMarkTargetComponent = inbuffer->hMarkTargetComponent;
		splitter->use_map[buf_index] |= (1 << port_index);
		count++;

		pthread_mutex_unlock(&splitter->lock);
		ret = port->return_buffer(port, port->header[buf_index]);
		pthread_mutex_lock(&splitter->lock);

		if (ret != 0) {
			splitter->use_map[buf_index] &= ~(1 << port_index);
			count--;
		}
	}

EXIT:
	splitter->use_map[buf_index] &= ~(1 << OMX_DEFAULT_INPUT_PORT);
	if (splitter->use_map[buf_index] == 0 && count != 0) {
		OSCL_LOGI("splitter share to %d ports", count);
		OSCL_LOGI("all outport return it to splitter while sharing");
		count = 0;
	}

	ret = count;
	pthread_mutex_unlock(&splitter->lock);
	OSCL_TRACE("%x %x\n", inbuffer, inbuffer->pBuffer);
	return ret;
}


OMX_ERRORTYPE splitter_fill_this_buffer(OMX_IN OMX_HANDLETYPE cmp_hdl,
	OMX_IN OMX_BUFFERHEADERTYPE *header)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component;
	base_port_t *port;
	int port_index;
	int buf_index;
	splitter_private_t *splitter;
	base_port_t *inport;
	OSCL_TRACE(" %p %p\n", cmp_hdl, header);

	oscl_param_check((cmp_hdl != NULL) && (header != NULL),
		OMX_ErrorBadParameter, NULL);

	component = get_lb_component(cmp_hdl);
	oscl_param_check(component != NULL, OMX_ErrorBadParameter, NULL);
	oscl_param_check(header->nOutputPortIndex < component->num_ports,
		OMX_ErrorBadParameter, NULL);

	port = &component->port[header->nOutputPortIndex];
	oscl_param_check(port->port_param.eDir == OMX_DirOutput,
		OMX_ErrorBadParameter, NULL);

	splitter = component->component_private;
	port_index = header->nOutputPortIndex;
	inport = &component->port[OMX_DEFAULT_INPUT_PORT];
	pthread_mutex_lock(&splitter->lock);
	/* get buffer index in array. */
	for (buf_index = 0;
		buf_index < port->port_param.nBufferCountActual; buf_index++) {
		if (header  == port->header[buf_index]) {
			OSCL_TRACE("%d return buffer(%x)\n", port_index, buf_index);
			break;
		}
	}
	if (buf_index == port->port_param.nBufferCountActual) {
		OSCL_LOGE("buffer(%x) not find in :%d", header, port_index);
		header->nFilledLen = 0;
		header->nFlags = 0;
		ret = -1;
		goto EXIT;
	}
	/* outbuffer is in queue and usemap is unset, means app first call
	 * fill this buffer function. just skip it.*/
	if (((splitter->use_map[buf_index]) & (1 << port_index)) == 0) {
		OSCL_LOGI("buffer(%d-%x) state err, check it", port_index, buf_index);
		goto EXIT;
	}
	splitter->use_map[buf_index] &= ~(1 << port_index);

	/* double check */
	if (inport->header[buf_index] == NULL
		|| inport->header[buf_index]->pBuffer != header->pBuffer){
		OSCL_LOGE("buffer(%x) not find in inport", buf_index);
		ret = -1;
		goto EXIT;
	}
	if (splitter->use_map[buf_index] == 0) {
		inport->header[buf_index]->nFilledLen = 0;
		inport->header[buf_index]->nFlags = 0;
		OSCL_TRACE("return_buffer:%x\n", inport->return_buffer);
		ret = inport->return_buffer(inport, inport->header[buf_index]);
		if (ret != 0) {
			OSCL_LOGE("return buffer failed!");
			inport->recive_buffer(inport, inport->header[buf_index]);
		}
	}

EXIT:
	pthread_mutex_unlock(&splitter->lock);
	OSCL_TRACE("\n");
	return ret;
}

/**
 * splitter_buffer_manager - buffer manager thread function.
 *
 * @param: OMX_COMPONENTTYPE component handle
 *
 * Returns 0 on success, -EERROR otherwise..
 */
void *splitter_buffer_manager(void *param)
{
	lb_omx_component_t *component;
	base_port_t *inport;
	OMX_BUFFERHEADERTYPE *inbuffer = NULL;
	int i;
	int share_count;
	int ret;

	OSCL_TRACE(" %p\n", param);
	oscl_param_check((param != NULL), NULL, NULL);
	component = get_lb_component(param);
	inport = &component->port[OMX_DEFAULT_INPUT_PORT];
	component->dbg_flag = set_debug_state(component->dbg_flag,
			DEBUG_BUF_MGNT_SHT, DEBUG_THREAD_START);

	/* checks if the component is in a state able to receive buffers */
	while (component->state == OMX_StateIdle
		|| component->state == OMX_StateExecuting
		|| component->state == OMX_StatePause) {
		/*Wait till the ports are being flushed*/
		component->dbg_wdog = 0;
		while (sem_trywait(component->buf_mgnt_sem) == 0)
			continue;

		pthread_mutex_lock(&component->flush_mutex);

		for (i = 0; i < component->num_ports; i++) {
			while (component->port[i].is_flushed) {
				pthread_mutex_unlock(&component->flush_mutex);
				OSCL_LOGI("comp(%s:%d) flush buffers\n",
					get_component_name(component), i);
				if (i == OMX_DEFAULT_INPUT_PORT && inbuffer) {
					component->port[i].return_buffer(
						&component->port[i], inbuffer);
					inbuffer = NULL;
				}
				sem_post(component->mgmt_flush_sem);
				sem_wait(component->flush_sem);
				pthread_mutex_lock(&component->flush_mutex);
			}
		}
		pthread_mutex_unlock(&component->flush_mutex);

		if (component->state != OMX_StateExecuting) {
			OSCL_LOGI("comp(%s) waiting for excute:%s, buf num:%d\n",
				get_component_name(component),
				omx_state_as_string(component->state),
				component->port[OMX_DEFAULT_INPUT_PORT].num_assigned);
			sem_wait(component->buf_mgnt_sem);
			continue;
		}

		if (inbuffer == NULL)
			inbuffer = oscl_queue_dequeue(&inport->buffer_queue);
		if (inbuffer == NULL) {
			OSCL_LOGI("sem_wait buf_mgnt_sem\n");
			sem_wait(component->buf_mgnt_sem);
			OSCL_LOGI("after sem_wait buf_mgnt_sem\n");
			continue;
		}
		share_count = splitter_share_buffer(component, inbuffer);
		if (share_count == 0) {
			OSCL_LOGD("output is not ready , wait. %d\n",
				component->buf_mgnt_sem->sem->value);
			ret = inport->return_buffer(inport, inbuffer);
			if (ret != 0)
				continue;
		}
		inbuffer = NULL;

	}
	OSCL_LOGW("exit from buffer_manager:%s\n", rt_thread_self()->name);
	OSCL_TRACE(" %p\n", param);
	component->dbg_flag = set_debug_state(component->dbg_flag,
			DEBUG_BUF_MGNT_SHT, DEBUG_THREAD_EXIT);
	pthread_exit(NULL);
	OSCL_TRACE("\n");
	return NULL;
}

/**
 * splitter_component_deinit - component deinit function.
 *
 * @cmp_hdl: OMX_COMPONENTTYPE component handle
 *
 * Returns 0 on success, -EERROR otherwise..
 */
OMX_ERRORTYPE splitter_component_deinit(OMX_IN OMX_HANDLETYPE cmp_hdl)
{
	OMX_COMPONENTTYPE *base_cmp = (OMX_COMPONENTTYPE *)cmp_hdl;
	lb_omx_component_t *component;
	splitter_private_t *private;
	int ret;
	int i;

	OSCL_TRACE("base_cmp_handle:%p\n", cmp_hdl);
	oscl_param_check(cmp_hdl != NULL, OMX_ErrorBadParameter, NULL);
	component = (lb_omx_component_t *)base_cmp->pComponentPrivate;
	private = component->component_private;

	component->port[OMX_DEFAULT_INPUT_PORT].deinit(
		&component->port[OMX_DEFAULT_INPUT_PORT]);
	for (i = 1; i < component->num_ports; i++) {
		if (component->port[i].header)
			oscl_free(component->port[i].header);
		if (component->port[i].buf_state)
			oscl_free(component->port[i].buf_state);
		component->port[i].header = NULL;
		component->port[i].buf_state = NULL;
		component->port[i].deinit(&component->port[i]);
	}
	ret = base_component_deinit(cmp_hdl);
	if (private != NULL) {
		component->component_private = NULL;
		pthread_mutex_destroy(&private->lock);
		oscl_free(private);
	}
	OSCL_TRACE("\n");
	return ret;
}


/**
 * splitter_set_parameter - set component param.
 *
 * @cmp_hdl: OMX_COMPONENTTYPE component handle
 * @index: param index
 * @param_data: param data
 *
 * Returns 0 on success, -EERROR otherwise..
 */
OMX_ERRORTYPE splitter_set_parameter(OMX_IN OMX_HANDLETYPE cmp_hdl,
	OMX_IN OMX_INDEXTYPE index,
	OMX_IN OMX_PTR param_data)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component = NULL;
	OMX_PARAM_PORTDEFINITIONTYPE *port_def;
	int enabled, populate;
	int i;
	OSCL_TRACE(" %p, %p , %d\n", cmp_hdl, param_data, index);

	oscl_param_check((cmp_hdl != NULL) && (param_data != NULL),
		OMX_ErrorBadParameter, NULL);
	component = get_lb_component(cmp_hdl);
	oscl_param_check((component != NULL), OMX_ErrorBadParameter, NULL);

	if (index == OMX_IndexParamPortDefinition) {
		port_def = (OMX_PARAM_PORTDEFINITIONTYPE *)param_data;
		if (port_def->nPortIndex != OMX_DEFAULT_INPUT_PORT) {
			ret = OMX_ErrorBadParameter;
			goto EXIT;
		}
		if (port_def->nBufferCountActual > SPLITTER_MAX_BUF_NUM) {
			OSCL_LOGE("nBufferCountActual > SPLITTER_MAX_BUF_NUM");
			port_def->nBufferCountActual = SPLITTER_MAX_BUF_NUM;
		}
		for (i = 0; i < component->num_ports; i++) {
			port_def = &component->port[i].port_param;
			enabled = port_def->bEnabled;
			populate = port_def->bPopulated;
			memcpy(port_def, param_data, sizeof(*port_def));
			port_def->nPortIndex = i;
			if (i == OMX_DEFAULT_INPUT_PORT)
				port_def->eDir = OMX_DirInput;
			else
				port_def->eDir = OMX_DirOutput;
			port_def->bEnabled = enabled;
			port_def->bPopulated = populate;
		}
		goto EXIT;
	}
	ret = base_set_parameter(cmp_hdl, index, param_data);

EXIT:
	OSCL_TRACE(" %d\n", ret);
	return ret;
}

/**
 * splitter_component_init - component init func.
 *
 * @lb_cmp_hdl: output lb component handle
 *
 * Returns 0 on success, -EERROR otherwise..
 */
OMX_ERRORTYPE splitter_component_init(lb_omx_component_t *lb_cmp_hdl)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	int i;
	splitter_private_t *private;

	OSCL_TRACE("splitter_component_init:%p\n", lb_cmp_hdl);
	private = (splitter_private_t *)oscl_zalloc(sizeof(splitter_private_t));
	oscl_param_check(private != NULL, OMX_ErrorInsufficientResources, NULL);
	pthread_mutex_init(&private->lock, NULL);

	ret = base_component_init(lb_cmp_hdl);
	lb_cmp_hdl->base_comp.SetParameter = splitter_set_parameter;
	lb_cmp_hdl->buf_manager = splitter_buffer_manager;
	lb_cmp_hdl->base_comp.ComponentDeInit = splitter_component_deinit;
	lb_cmp_hdl->base_comp.FillThisBuffer = splitter_fill_this_buffer;
	lb_cmp_hdl->component_private = private;
	lb_cmp_hdl->name = "splitter";

	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	lb_cmp_hdl->num_ports = OMX_PORT_NUMBER_SUPPORTED;
	ret = base_port_init(lb_cmp_hdl, &lb_cmp_hdl->port[OMX_DEFAULT_INPUT_PORT],
			OMX_DEFAULT_INPUT_PORT,
			OMX_DirInput);
	if (ret != OMX_ErrorNone) {
		base_component_deinit(lb_cmp_hdl);
		return ret;
	}
	lb_cmp_hdl->port[OMX_DEFAULT_INPUT_PORT].allocate_buffer =
		splitter_inport_allocate_buf;
	lb_cmp_hdl->port[OMX_DEFAULT_INPUT_PORT].use_buffer =
		splitter_port_use_buffer;
	lb_cmp_hdl->port[OMX_DEFAULT_INPUT_PORT].free_buffer =
		splitter_port_free_buffer;
	lb_cmp_hdl->port[OMX_DEFAULT_INPUT_PORT].port_param.nBufferSize =
		OMX_DEFAULT_BUF_SIZE;
	lb_cmp_hdl->port[OMX_DEFAULT_INPUT_PORT].port_param.eDomain =
		OMX_PortDomainOther;

	for (i = OMX_DEFAULT_OUTPUT_PORT; i < lb_cmp_hdl->num_ports; i++) {
		ret = base_port_init(lb_cmp_hdl, &lb_cmp_hdl->port[i],
				i, OMX_DirOutput);
		lb_cmp_hdl->port[i].allocate_buffer = splitter_outport_allocate_buf;
		lb_cmp_hdl->port[i].use_buffer = splitter_port_use_buffer;
		lb_cmp_hdl->port[i].free_buffer = splitter_port_free_buffer;

		lb_cmp_hdl->port[i].port_param.nBufferSize = OMX_DEFAULT_BUF_SIZE;
		lb_cmp_hdl->port[i].port_param.eDomain = OMX_PortDomainOther;
		lb_cmp_hdl->port[i].port_param.bEnabled = OMX_FALSE;
	}

	lb_cmp_hdl->base_comp.ComponentDeInit = splitter_component_deinit;
	lb_cmp_hdl->buf_manager = splitter_buffer_manager;
EXIT:
	if (ret != OMX_ErrorNone) {
		if (private != NULL) {
			pthread_mutex_destroy(&private->lock);
			oscl_free(private);
			lb_cmp_hdl->component_private = NULL;
		}
	}
	OSCL_TRACE("splitter_component_init:%d\n", ret);
	OSCL_TRACE("\n");
	return ret;
}





