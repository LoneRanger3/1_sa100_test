/*
 * base_port.c - Functions for common port API.
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
#define LOG_TAG			"base_port"

#include <unistd.h>
#include <oscl.h>
#include <base_component.h>
#define usleep(n) rt_thread_delay(1)

/** The default value for the number of needed buffers for each port. */
#define DEFAULT_NUMBER_BUFFERS_PER_PORT 4
/** The default value for the minimum number of needed buffers for each port. */
#define DEFAULT_MIN_NUMBER_BUFFERS_PER_PORT 2

/* #define PORT_DEBUG_ON */
#ifdef PORT_DEBUG_ON
#undef OSCL_LOGV
#undef OSCL_LOGD
#undef OSCL_LOGI
#undef OSCL_LOGW
#undef OSCL_LOGE
#define OSCL_LOGV(fmt, ...)      dbg_log_port(port, DBG_VERBOSE, fmt, ##__VA_ARGS__)
#define OSCL_LOGD(fmt, ...)      dbg_log_port(port, DBG_LOG, fmt, ##__VA_ARGS__)
#define OSCL_LOGI(fmt, ...)      dbg_log_port(port, DBG_INFO, fmt, ##__VA_ARGS__)
#define OSCL_LOGW(fmt, ...)      dbg_log_port(port, DBG_WARNING, fmt, ##__VA_ARGS__)
#define OSCL_LOGE(fmt, ...)      dbg_log_port(port, DBG_ERROR, fmt, ##__VA_ARGS__)
#endif

static void *__port_malloc(base_port_t *port,
	OMX_U32 size)
{
	char *buffer = NULL;
	OMX_U32 align = 4096;
	if (size == 0 || port == NULL)
		return NULL;
	if (port->port_param.nBufferAlignment > align)
		align = port->port_param.nBufferAlignment;
	if (port->port_param.bBuffersContiguous != OMX_FALSE) {
		if (port->port_param.nBufferAlignment > 0)
			buffer = oscl_malloc_align(size, align);
		else
			buffer = oscl_malloc(size);
	} else {
		if (port->port_param.nBufferAlignment > 0)
			buffer = oscl_malloc_align(size, align);
		else
			buffer = oscl_malloc(size);
	}
	return buffer;
}
static void __port_free(base_port_t *port,
	void *buffer)
{
	if (buffer == NULL || port == NULL)
		return;
	if (port->port_param.bBuffersContiguous != OMX_FALSE) {
		if (port->port_param.nBufferAlignment > 0)
			oscl_free_align(buffer);
		else
			oscl_free(buffer);
	} else {
		if (port->port_param.nBufferAlignment > 0)
			oscl_free_align(buffer);
		else
			oscl_free(buffer);
	}
}
static OMX_ERRORTYPE __add_buffer(base_port_t *port,
	OMX_BUFFERHEADERTYPE **buf_header,
	OMX_PTR app_private,
	OMX_U32 size,
	OMX_U8 *buffer,
	int buffer_state)
{
	unsigned int i;
	lb_omx_component_t *component = (lb_omx_component_t *)port->component;
	int buf_count;
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	OSCL_TRACE("port:%p(%s), buffer:%p-%d", port,
		get_port_name(port), buffer, size);

	buf_count = port->port_param.nBufferCountActual;
	*buf_header = NULL;
	oscl_param_check_exit(!PORT_IS_SUPPLIER(port),
		OMX_ErrorIncorrectStateTransition, NULL);
	oscl_param_check_exit(
		((component->state == OMX_StateLoaded
				|| component->state == OMX_StateIdle)
			|| (component->state == OMX_StateWaitForResources)
			|| !PORT_IS_ENABLED(port)),
		OMX_ErrorIncorrectStateTransition, NULL);
	oscl_param_check_exit((buf_count != 0), OMX_ErrorBadParameter, NULL);
	oscl_param_check_exit((size >= port->port_param.nBufferSize),
		OMX_ErrorBadParameter, NULL);

	if (NULL == port->header) {
		port->header = (OMX_BUFFERHEADERTYPE **) oscl_zalloc(
				buf_count * sizeof(OMX_BUFFERHEADERTYPE *));
		oscl_param_check_exit((port->header != NULL),
			OMX_ErrorInsufficientResources, NULL);
	}
	if (NULL == port->buf_state) {
		port->buf_state = oscl_zalloc(buf_count * sizeof(OMX_U32));
		oscl_param_check_exit((port->buf_state != NULL),
			OMX_ErrorInsufficientResources, NULL);
	}

	for (i = 0; i < buf_count; i++) {
		if (port->buf_state[i] == BUFFER_FREE)
			break;
	}
	oscl_param_check((i < buf_count),
		OMX_ErrorInsufficientResources, NULL);
	port->header[i] = (OMX_BUFFERHEADERTYPE *)oscl_zalloc(
			sizeof(OMX_BUFFERHEADERTYPE));
	oscl_param_check_exit((port->header[i] != NULL),
		OMX_ErrorInsufficientResources, NULL);
	port->header[i]->nVersion.nVersion = OMX_VERSION;
	port->header[i]->pBuffer = buffer;
	port->header[i]->nAllocLen = size;
	port->header[i]->nFilledLen = 0;
	port->header[i]->nOffset = 0;
	port->header[i]->nFlags = 0;
	port->header[i]->pAppPrivate = app_private;
	port->header[i]->nTickCount = 0;
	port->header[i]->nTimeStamp = 0;
	port->header[i]->hMarkTargetComponent = NULL;
	*buf_header = port->header[i];
	if (OMX_DirInput == port->port_param.eDir) {
		port->header[i]->nInputPortIndex = port->port_param.nPortIndex;
		port->header[i]->nOutputPortIndex = -1;
	} else {
		port->header[i]->nInputPortIndex = -1;
		port->header[i]->nOutputPortIndex = port->port_param.nPortIndex;
	}
	port->buf_state[i] |= buffer_state;
	port->buf_state[i] |= HEADER_ALLOCATED;
	port->num_assigned++;
	OSCL_LOGI("buffer count:%d,assigned:%d", buf_count, port->num_assigned);
	if (buf_count == port->num_assigned) {
		port->port_param.bPopulated = OMX_TRUE;
		OSCL_LOGI("buffer count:%d,assigned:%d", buf_count, port->num_assigned);
		sem_post(&port->populate_sem);
	}

EXIT:
	OSCL_TRACE("port:%p, buffer:%p-%d, ret:%x", port, buffer, size, ret);
	return ret;
}


OMX_ERRORTYPE base_port_init(lb_omx_component_t *component,
	base_port_t *port,
	OMX_U32 index,
	OMX_U32 dir_type)
{
	OMX_PARAM_PORTDEFINITIONTYPE *pparam;

	oscl_param_check((component != NULL) && (port != NULL),
		OMX_ErrorBadParameter, NULL);
	memset(port, 0, sizeof(base_port_t));
	port->component = component;
	sem_init(&port->populate_sem, 0, 0);
	sem_init(&port->buffer_sem, 0, 0);
	oscl_queue_init(&port->buffer_queue);
	port->log_level = DBG_WARNING;

	OSCL_TRACE(" %p %p %d %d\n", component, port, index, dir_type);

	pparam = &port->port_param;
	pparam->nSize = sizeof(OMX_PARAM_PORTDEFINITIONTYPE);
	pparam->nVersion.nVersion = OMX_VERSION;
	pparam->nPortIndex = index;
	pparam->nBufferCountActual = DEFAULT_NUMBER_BUFFERS_PER_PORT;
	pparam->nBufferCountMin = DEFAULT_MIN_NUMBER_BUFFERS_PER_PORT;
	pparam->bEnabled = OMX_TRUE;
	pparam->bPopulated = OMX_FALSE;
	pparam->eDir = dir_type;

	port->deinit = base_port_deinit;
	port->allocate_buffer = base_port_allocate_buf;
	port->use_buffer = base_port_use_buffer;
	port->free_buffer = base_port_free_buffer;
	port->disable = base_port_disable;
	port->enable = base_port_enable;
	port->recive_buffer = base_port_recive_buffer;
	port->flush_buffer = base_port_flush_buffer;
	port->return_buffer = base_port_return_buffer;
	port->tunnel_request = base_port_tunnel_request;
	port->allocate_tunnel_buffer = base_port_allocate_tunnel_buf;
	port->free_tunnel_buffer = base_port_free_tunnel_buffer;

	OSCL_TRACE(" %p %p %d %d\n", component, port, index, dir_type);
	return OMX_ErrorNone;
}

OMX_ERRORTYPE base_port_deinit(base_port_t *port)
{
	OSCL_TRACE(" %p\n", port);
	if (port->buf_state != NULL || port->header != NULL)
		OSCL_LOGE("%s(%d), buffer not freed, buf_state:%p, header:%p",
			get_port_name(port), port->port_param.nPortIndex,
			port->buf_state, port->header);
	if (oscl_queue_get_num(&port->buffer_queue)) {
		OSCL_LOGE("port%d queue is not empty:%d while deinit",
			  port->port_param.nPortIndex,
			  oscl_queue_get_num(&port->buffer_queue));
	}
	oscl_queue_deinit(&port->buffer_queue);
	sem_destroy(&port->buffer_sem);
	sem_destroy(&port->populate_sem);
	OSCL_TRACE(" %p\n", port);
	return OMX_ErrorNone;
}

OMX_ERRORTYPE base_port_enable(base_port_t *port)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;

	OSCL_TRACE(" %p\n", port);
	oscl_param_check((port != NULL), OMX_ErrorBadParameter, NULL);
	oscl_param_check(!(PORT_IS_ENABLED(port)), OMX_ErrorNone, NULL);

	if (!PORT_IS_SUPPLIER(port)) {
		if (get_component_state(port) != OMX_StateLoaded
			&& get_component_state(port) != OMX_StateWaitForResources
			&& PORT_IS_TUNNELED(port)) {
			OSCL_LOGI("%p waiting for populate_sem\n", port);
			sem_wait(&port->populate_sem);
		}
	} else {
		ret = port->allocate_tunnel_buffer(port);
		oscl_param_check((ret == OMX_ErrorNone), ret, NULL);
	}
	port->port_param.bEnabled = OMX_TRUE;
	port->being_onoff = OMX_FALSE;
	OSCL_TRACE(" %p\n", port);
	return ret;
}


/** @brief Disables the port.
 *
 * This function is called due to a request by the IL client
 *
 * @param port the reference to the port
 *
 */
OMX_ERRORTYPE base_port_disable(base_port_t *port)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;

	OSCL_TRACE(" %p\n", port);
	if (!PORT_IS_ENABLED(port))
		return OMX_ErrorNone;

	port->port_param.bEnabled = OMX_FALSE;
	if (get_component_state(port) != OMX_StateLoaded) {
		if (!PORT_IS_TUNNELED(port))
			oscl_queue_flush(&port->buffer_queue);
		else if (!PORT_IS_SUPPLIER(port))
			sem_wait(&port->populate_sem);
		else {
			/*Since port is being disabled, remove buffers from the queue*/
			oscl_queue_flush(&port->buffer_queue);
			ret = port->free_tunnel_buffer(port);
			if (ret != OMX_ErrorNone)
				OSCL_LOGE("free tunnel buf failed!");
		}
	}

	port->being_onoff = OMX_FALSE;
	OSCL_TRACE(" %p\n", port);
	return ret;
}

OMX_ERRORTYPE base_port_allocate_buf(base_port_t *port,
	OMX_BUFFERHEADERTYPE **buf_header,
	OMX_PTR app_private,
	OMX_U32 size)
{
	int ret = OMX_ErrorNone;
	OMX_U8 *buffer;
	OSCL_TRACE(" %p %p %p %d\n", port, buf_header, app_private, size);

	oscl_param_check((port != NULL), OMX_ErrorBadParameter, NULL);
	buffer = __port_malloc(port, size);
	oscl_param_check((buffer != NULL),
		OMX_ErrorInsufficientResources, NULL);
	ret = __add_buffer(port, buf_header, app_private, size, buffer, BUFFER_ALLOCATED);
	if (ret != OMX_ErrorNone)
		__port_free(port, buffer);

	OSCL_TRACE(" %p %p %p %d\n", port, buf_header, app_private, size);
	return ret;
}

OMX_ERRORTYPE base_port_use_buffer(base_port_t *port,
	OMX_BUFFERHEADERTYPE **buf_header,
	OMX_PTR app_private,
	OMX_U32 size,
	OMX_U8 *buffer)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	ret = __add_buffer(port, buf_header, app_private, size, buffer, BUFFER_ASSIGNED);
	return ret;
}

OMX_ERRORTYPE base_port_free_buffer(base_port_t *port,
	OMX_BUFFERHEADERTYPE *buf_header)
{
	unsigned int i;
	int buf_count;

	oscl_param_check((port != NULL) && (buf_header != NULL),
		OMX_ErrorBadParameter, NULL);
	oscl_param_check(!(PORT_IS_SUPPLIER(port) && PORT_IS_TUNNELED(port)),
		OMX_ErrorIncorrectStateTransition, NULL);
	OSCL_LOGD("port:%p header:%p buf_state:%p", port, port->header, port->buf_state);
	oscl_param_check((port->header != NULL) && (port->buf_state != NULL),
		OMX_ErrorIncorrectStateTransition, NULL);

	buf_count = port->port_param.nBufferCountActual;
	OSCL_TRACE("buf_count:%d, buffer:%p", buf_count, buf_header);
	for (i = 0; i < buf_count; i++) {
		if (port->header[i] == buf_header)
			break;
	}
	oscl_param_check((i < buf_count),
		OMX_ErrorBadParameter, NULL);
	port->num_assigned--;
	if (port->buf_state[i] & BUFFER_ALLOCATED) {
		__port_free(port, buf_header->pBuffer);
		buf_header->pBuffer = NULL;
	}
	if (port->buf_state[i] & HEADER_ALLOCATED) {
		oscl_free(buf_header);
		port->header[i] = NULL;
	}
	port->buf_state[i] = BUFFER_FREE;

	if (port->num_assigned == 0) {
		if (NULL != port->header) {
			oscl_free(port->header);
			port->header = NULL;
		}
		if (NULL != port->buf_state) {
			oscl_free(port->buf_state);
			port->buf_state = NULL;
		}
		if (oscl_queue_get_num(&port->buffer_queue)) {
			OSCL_LOGE("port%d queue is not empty:%d while free",
				  port->port_param.nPortIndex,
				  oscl_queue_get_num(&port->buffer_queue));
			oscl_queue_flush(&port->buffer_queue);
		}
		sem_post(&port->populate_sem);
	}
	port->port_param.bPopulated = OMX_FALSE;

	OSCL_TRACE("port:%p, buffer:%d", port, port->num_assigned);
	return OMX_ErrorNone;
}

OMX_ERRORTYPE base_port_tunnel_request(base_port_t *port,
	OMX_HANDLETYPE tunneld_comp,
	OMX_U32 tunneled_index,
	OMX_TUNNELSETUPTYPE *tunnel)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	OMX_PARAM_PORTDEFINITIONTYPE port_def;
	OMX_PARAM_BUFFERSUPPLIERTYPE supplier;

	OSCL_TRACE("port:%p, tunnel:%p-%d", port, tunneld_comp, tunneled_index);
	oscl_param_check((port != NULL), OMX_ErrorBadParameter, NULL);
	oscl_param_check((port->tunnel_flags == 0),
		OMX_ErrorPortsNotCompatible, NULL);

	port->tunneled_component = 0;
	port->tunnel_ports = 0;
	port->tunnel_flags = 0;

	/* cancel previous tunnel */
	if (tunnel == NULL || tunneld_comp == 0)
		return OMX_ErrorNone;

	/* Get Port Definition of the Tunnelled Component*/
	port_def.nPortIndex = tunneled_index;
	port_def.nVersion.nVersion = OMX_VERSION;
	ret = OMX_GetParameter(tunneld_comp,
			OMX_IndexParamPortDefinition, &port_def);
	oscl_param_check((ret == OMX_ErrorNone),
		OMX_ErrorPortsNotCompatible, NULL);
	oscl_param_check((port_def.eDomain == port->port_param.eDomain),
		OMX_ErrorPortsNotCompatible, NULL);

	if (port->port_param.eDir == OMX_DirOutput) {
		tunnel->eSupplier = OMX_BufferSupplyOutput;
		port->tunneled_component = tunneld_comp;
		port->tunnel_ports = tunneled_index;
		port->num_tunnel_buffer = port_def.nBufferCountActual;
		port->tunnel_flags |= TUNNEL_IS_SUPPLIER;
		port->tunnel_flags |= TUNNEL_ESTABLISHED;
	} else {
		/* Get Buffer Supplier type of the Tunneled Component*/
		supplier.nPortIndex = tunneled_index;
		supplier.nVersion.nVersion = OMX_VERSION;
		supplier.eBufferSupplier = OMX_BufferSupplyOutput;
		ret = OMX_GetParameter(tunneld_comp,
				OMX_IndexParamCompBufferSupplier, &supplier);
		oscl_param_check((ret == OMX_ErrorNone),
			OMX_ErrorPortsNotCompatible, NULL);
		if ((tunnel->nTunnelFlags & OMX_PORTTUNNELFLAG_READONLY)
			|| (tunnel->eSupplier == OMX_BufferSupplyInput)
			|| (tunnel->eSupplier == OMX_BufferSupplyUnspecified))
			supplier.eBufferSupplier = OMX_BufferSupplyInput;
		OSCL_LOGI("eBufferSupplier:%x", supplier.eBufferSupplier);

		/* Set Buffer Supplier type of Tunnelled Component after negotiation*/
		supplier.nPortIndex = tunneled_index;
		ret = OMX_SetParameter(tunneld_comp,
				OMX_IndexParamCompBufferSupplier, &supplier);
		oscl_param_check((ret == OMX_ErrorNone),
			OMX_ErrorPortsNotCompatible, NULL);
		port->tunneled_component = tunneld_comp;
		port->tunnel_ports = tunneled_index;
		port->num_tunnel_buffer = port_def.nBufferCountActual;
		if (supplier.eBufferSupplier == OMX_BufferSupplyInput) {
			tunnel->eSupplier = OMX_BufferSupplyInput;
			port->tunnel_flags |= TUNNEL_IS_SUPPLIER;
		}
		port->tunnel_flags |= TUNNEL_ESTABLISHED;
	}

	OSCL_TRACE("port:%p, tunnel:%p-%d sucess",
		port, tunneld_comp, tunneled_index);
	return OMX_ErrorNone;
}


OMX_ERRORTYPE base_port_allocate_tunnel_buf(base_port_t *port)
{
	unsigned int i;
	OMX_U8 *buffer = NULL;
	int ret = OMX_ErrorNone;
	OMX_PARAM_PORTDEFINITIONTYPE port_def;
	OMX_U32 nbuffer;
	OMX_U32 nretry = 0, buf_size;

	OSCL_TRACE(" %p", port);
	oscl_param_check(PORT_IS_ENABLED(port) && PORT_IS_TUNNELED(port),
		OMX_ErrorBadPortIndex, NULL);

	/*Get nBufferSize of the peer port and allocate which one is bigger*/
	buf_size = port->port_param.nBufferSize;
	port_def.nVersion.nVersion = OMX_VERSION;
	port_def.nPortIndex = port->tunnel_ports;
	ret = OMX_GetParameter(port->tunneled_component,
			OMX_IndexParamPortDefinition, &port_def);
	if (ret != OMX_ErrorNone)
		return OMX_ErrorPortsNotCompatible;
	if (buf_size < port_def.nBufferSize) {
		buf_size = port_def.nBufferSize;
		port->port_param.nBufferSize = buf_size;
	}

	/* set the number of buffer needed getting the max nBufferCountActual
	 * of the two components. On the one with the minor nBufferCountActual a
	 * setParam should be called to normalize the value, if possible.
	 */
	nbuffer = port->port_param.nBufferCountActual;
	if (port_def.nBufferCountActual < nbuffer) {
		port_def.nBufferCountActual = nbuffer;
		ret = OMX_SetParameter(port->tunneled_component,
				OMX_IndexParamPortDefinition, &port_def);
		if (ret != OMX_ErrorNone)
			return OMX_ErrorPortsNotCompatible;
	}
	if (nbuffer < port_def.nBufferCountActual) {
		nbuffer = port_def.nBufferCountActual;
		port->port_param.nBufferCountActual = nbuffer;
	}

	if (port->port_param.nBufferCountActual == 0) {
		port->port_param.bPopulated = OMX_TRUE;
		OSCL_LOGW("Allocated nothing");
		return OMX_ErrorNone;
	}
	if (NULL == port->header) {
		port->header = (OMX_BUFFERHEADERTYPE **) oscl_zalloc(
				nbuffer * sizeof(OMX_BUFFERHEADERTYPE *));
		if (port->header == NULL)
			return OMX_ErrorInsufficientResources;
	}
	if (NULL == port->buf_state) {
		port->buf_state = oscl_zalloc(nbuffer * sizeof(OMX_U32));
		if (port->buf_state == NULL)
			return OMX_ErrorInsufficientResources;
	}
	for (i = 0; i < nbuffer; i++) {
		if (port->buf_state[i] != BUFFER_FREE)
			OSCL_LOGE("find a unfree buffer while tunnel allocate buf!");
		buffer = __port_malloc(port, buf_size);
		if (buffer == NULL)
			return OMX_ErrorInsufficientResources;
		/*Retry more than once, if the tunneled component is
		not in Loaded->Idle State. tobedo: wait for sem*/
		while (nretry++ < TUNNEL_USE_BUFFER_RETRY) {
			ret = OMX_UseBuffer(port->tunneled_component, &port->header[i],
					port->tunnel_ports, NULL, buf_size, buffer);
			if ((ret == OMX_ErrorIncorrectStateTransition)) {
				OSCL_LOGW("Waiting for next try %i\n", (int)nretry);
				usleep(TUNNEL_USE_BUFFER_RETRY_USLEEP_TIME);
				continue;
			}
			if (ret != OMX_ErrorNone) {
				OSCL_LOGE("OMX_UseBuffer faile:%x\n", ret);
				__port_free(port, buffer);
				return ret;
			}
			if (port->port_param.eDir == OMX_DirInput) {
				port->header[i]->nInputPortIndex
					= port->port_param.nPortIndex;
				port->header[i]->nOutputPortIndex = port->tunnel_ports;
			} else {
				port->header[i]->nInputPortIndex = port->tunnel_ports;
				port->header[i]->nOutputPortIndex
					= port->port_param.nPortIndex;
			}
			break;
		}
		port->buf_state[i] = BUFFER_ALLOCATED;
		port->num_assigned++;
		oscl_queue_queue(&port->buffer_queue, port->header[i]);
		sem_post(&port->buffer_sem);
	}
	if (nbuffer == port->num_assigned)
		port->port_param.bPopulated = OMX_TRUE;
	OSCL_TRACE(" %p, nbuf:%d, size:%d, assigned:%d", port, nbuffer, buf_size,
		port->num_assigned);
	return OMX_ErrorNone;
}

OMX_ERRORTYPE base_port_free_tunnel_buffer(base_port_t *port)
{
	unsigned int i;
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	OMX_U32 nretry = 0;
	OMX_U8 *pbuffer;
	OSCL_TRACE(" %p", port);
	oscl_param_check(PORT_IS_ENABLED(port) && PORT_IS_TUNNELED(port),
		OMX_ErrorBadPortIndex, NULL);

	for (i = 0; i < port->port_param.nBufferCountActual; i++) {
		if (port->header[i])
			pbuffer = port->header[i]->pBuffer;
		else
			pbuffer = NULL;
		if (((port->buf_state[i] & BUFFER_ALLOCATED) == 0)
			|| ((port->buf_state[i] & HEADER_ALLOCATED) != 0))
			OSCL_LOGE("buf state %x err, check it!!", port->buf_state[i]);
		/*Retry more than once, if the tunneled component is
		not in Idle->Loaded State. tobedo: wait for sem*/
		while (nretry++ < TUNNEL_USE_BUFFER_RETRY) {
			ret = OMX_FreeBuffer(port->tunneled_component,
					port->tunnel_ports, port->header[i]);
			if (ret == OMX_ErrorIncorrectStateTransition) {
				usleep(TUNNEL_USE_BUFFER_RETRY_USLEEP_TIME);
				continue;
			}
			if (ret != OMX_ErrorNone) {
				OSCL_LOGE("free buffer %p(%d) failed!!",
					port->header[i], i);
				return ret;
			}
			port->header[i] = NULL;
			break;
		}
		if ((port->buf_state[i] & BUFFER_ALLOCATED) && pbuffer)
			__port_free(port, pbuffer);
		port->buf_state[i] = BUFFER_FREE;
	}
	port->port_param.bPopulated = OMX_FALSE;
	port->num_assigned = 0;
	OSCL_TRACE(" %p", port);
	return OMX_ErrorNone;
}


/** @brief Releases buffers under processing.
 * This function must be implemented in the derived classes, for the
 * specific processing
 */
OMX_ERRORTYPE base_port_flush_buffer(base_port_t *port)
{
	OMX_BUFFERHEADERTYPE *buf_head = NULL;

	OSCL_TRACE("%p(%s, %d)", port, get_port_name(port), port->port_param.nPortIndex);
	if (PORT_IS_SUPPLIER(port)) {
		OSCL_LOGW("%p(%s, %d)", port,
			get_port_name(port), port->port_param.nPortIndex);
		while (oscl_queue_get_num(&port->buffer_queue) != port->num_assigned) {
			OSCL_LOGD("(%s)%d queue num:%d assigned:%d",
				get_port_name(port), port->port_param.nPortIndex,
				oscl_queue_get_num(&port->buffer_queue),
				port->num_assigned);
			sem_wait(&port->buffer_sem);
		}
		OSCL_LOGW("%p(%s, %d)", port,
			get_port_name(port), port->port_param.nPortIndex);
	} else {
		OSCL_LOGI(" %p", port);
		do {
			buf_head = oscl_queue_dequeue(&port->buffer_queue);
			OSCL_LOGI("(%s)flush buffer:%p\n", get_port_name(port), buf_head);
			if (buf_head != NULL)
				port->return_buffer(port, buf_head);
		} while (buf_head != NULL);
		OSCL_LOGI(" %p", port);
	}

	OSCL_LOGI(" %p", port);
	return OMX_ErrorNone;
}




/** @brief the entry point for sending buffers to the port
 *
 * This function can be called by the EmptyThisBuffer or FillThisBuffer. It depends on
 * the nature of the port, that can be an input or output port.
 */
OMX_ERRORTYPE base_port_recive_buffer(base_port_t *port,
	OMX_BUFFERHEADERTYPE *buffer)
{
	OMX_U32 port_idx;
	lb_omx_component_t *component = (lb_omx_component_t *)port->component;

	OSCL_TRACE("%p:%p", port, buffer);
	port_idx = (port->port_param.eDir == OMX_DirInput) ? buffer->nInputPortIndex :
		buffer->nOutputPortIndex;
	oscl_param_check((port_idx == port->port_param.nPortIndex),
		OMX_ErrorBadPortIndex, NULL);
	oscl_param_check((OMX_StateInvalid != get_component_state(port)),
		OMX_ErrorInvalidState, NULL);
	if (!PORT_IS_SUPPLIER(port) && port->is_flushed) {
		OSCL_LOGW("recive buffers while flush port(none supplier)\n");
		OSCL_LOGW("%p(%s, %d)", port,
			get_port_name(port), port->port_param.nPortIndex);
		return OMX_ErrorInvalidState;
	}

	/* notify the buffer management thread we have a fresh new buffer to manage*/
	oscl_queue_queue(&port->buffer_queue, buffer);
	if (PORT_IS_SUPPLIER(port) && (port->is_flushed))
		sem_post(&port->buffer_sem);
	sem_post(component->buf_mgnt_sem);
	OSCL_TRACE("%p:%p", port, buffer);
	return OMX_ErrorNone;
}

/**
 * Returns Input/Output Buffer to the IL client or Tunneled Component
 */
OMX_ERRORTYPE base_port_return_buffer(base_port_t *port,
	OMX_BUFFERHEADERTYPE *buffer)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	OMX_ERRORTYPE(*return_buf_to_tunneled)(OMX_HANDLETYPE, OMX_BUFFERHEADERTYPE *);
	lb_omx_component_t *component = (lb_omx_component_t *)port->component;

	OSCL_TRACE("%p:%p", port, buffer);
	if (!PORT_IS_TUNNELED(port)) {
		OSCL_LOGD("%s port %d is not tunneled",
			get_port_name(port),
			port->port_param.nPortIndex);
		ret = (*(port->buf_done_callback))(
				&component->base_comp,
				component->callback_data,
				buffer);
		return ret;
	}
	if (PORT_IS_SUPPLIER(port) && port->is_flushed) {
		OSCL_LOGW("return buffers while flush port(supplier)\n");
		OSCL_LOGW("%p(%s, %d)", port,
			get_port_name(port), port->port_param.nPortIndex);
		oscl_queue_queue(&port->buffer_queue, buffer);
		sem_post(&port->buffer_sem);
		return ret;
	}
	if (port->port_param.eDir == OMX_DirInput) {
		return_buf_to_tunneled = ((OMX_COMPONENTTYPE *)(
					port->tunneled_component))->FillThisBuffer;
	} else {
		return_buf_to_tunneled = ((OMX_COMPONENTTYPE *)(
					port->tunneled_component))->EmptyThisBuffer;
	}
	ret = return_buf_to_tunneled(port->tunneled_component, buffer);
	if (/*PORT_IS_SUPPLIER(port) && */ret != OMX_ErrorNone) {
		oscl_queue_queue(&port->buffer_queue, buffer);
		sem_post(&port->buffer_sem);
	}
	OSCL_TRACE("%s-%d:%p:ret:%d", get_port_name(port),
		port->port_param.nPortIndex, buffer, ret);
	return ret;
}

