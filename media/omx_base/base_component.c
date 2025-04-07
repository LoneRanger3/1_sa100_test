/*
 * base_component.c - Standard functionality for the common component.
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
#define DBG_LEVEL         DBG_ERR

#include <oscl.h>
#include <base_component.h>
#include "lb_omx_as_string.h"
#include <clock_time.h>

/* #define COMPONENT_DEBUG_ON */
#ifdef COMPONENT_DEBUG_ON
#undef LOG_D
#undef LOG_I
#undef LOG_W
#undef LOG_E
#define LOG_D(fmt, ...)      dbg_log_comp(cmp_hdl, DBG_LOG, fmt, ##__VA_ARGS__)
#define LOG_I(fmt, ...)      dbg_log_comp(cmp_hdl, DBG_INFO, fmt, ##__VA_ARGS__)
#define LOG_W(fmt, ...)      dbg_log_comp(cmp_hdl, DBG_WARNING, fmt, ##__VA_ARGS__)
#define LOG_E(fmt, ...)      dbg_log_comp(cmp_hdl, DBG_ERROR, fmt, ##__VA_ARGS__)
#define set_debug_hdl() \
	OMX_HANDLETYPE cmp_hdl = (component != NULL) ? (&component->base_comp) : NULL;
#else
#define set_debug_hdl()
#endif
static OMX_ERRORTYPE __flush_ports(lb_omx_component_t *component,
	u32 index)
{
	int num_ports = index;
	int start_ports = index;
	int end_ports = index;
	base_port_t *port;
	int i;
	int ret = OMX_ErrorNotImplemented;

	set_debug_hdl();
	oscl_param_check((component != NULL),	OMX_ErrorBadParameter, NULL);
	num_ports = component->num_ports;
	oscl_param_check(((index == -1) || (index < num_ports)),
		OMX_ErrorBadPortIndex, NULL);

	if (-1 == index) {
		start_ports = 0;
		end_ports = num_ports - 1;
	}
	pthread_mutex_lock(&component->flush_mutex);
	for (i = start_ports; i <= end_ports; i++) {
		component->port[i].is_flushed = OMX_TRUE;
	}
	pthread_mutex_unlock(&component->flush_mutex);

	if (component->buf_mgnt_thread_id != -1 && component->mgmt_flush_sem) {
		sem_post(component->buf_mgnt_sem);
		if (oscl_sem_timedwait_ms(component->mgmt_flush_sem, 3000) != 0)
			OSCL_LOGE("flush wait manager timeout!");
	}

	for (i = start_ports; i <= end_ports; i++) {
		port = &component->port[i];
		OSCL_LOGI(" %s %d:header:%x", get_port_name(port),
			  port->port_param.nPortIndex, port->header);
		if (port->flush_buffer)
			ret = port->flush_buffer(port);
		component->port[i].is_flushed = OMX_FALSE;
	}
	sem_post(component->flush_sem);

	return ret;
}

static OMX_ERRORTYPE __set_ports(lb_omx_component_t *component,
	u32 index)
{
	int num_ports = index;
	int start_ports = index;
	int end_ports = index;
	base_port_t *port;
	int i, j;

	set_debug_hdl();
	oscl_param_check((component != NULL),	OMX_ErrorBadParameter, NULL);
	num_ports = component->num_ports;
	oscl_param_check(((index == -1) || (index < num_ports)),
		OMX_ErrorBadPortIndex, NULL);

	if (-1 == index) {
		start_ports = 0;
		end_ports = num_ports - 1;
	}
	for (i = start_ports; i <= end_ports; i++) {
		port = &component->port[i];
		if (port->header == NULL)
			port->header = oscl_zalloc(port->port_param.nBufferCountActual
					* sizeof(OMX_BUFFERHEADERTYPE *));
		if (port->buf_state == NULL) {
			port->buf_state = oscl_zalloc(port->port_param.nBufferCountActual
					* sizeof(u32));
			for (j = 0; j < port->port_param.nBufferCountActual; j++)
				port->buf_state[j] = BUFFER_FREE;
		}
	}
	return OMX_ErrorNone;
}

static OMX_ERRORTYPE __ports_onoff(lb_omx_component_t *component,
	u32 index, int is_on)
{
	int num_ports = index;
	int start_ports = index;
	int end_ports = index;
	base_port_t *port;
	int i;
	OMX_ERRORTYPE ret = OMX_ErrorNone;

	set_debug_hdl();
	oscl_param_check((component != NULL),	OMX_ErrorBadParameter, NULL);
	num_ports = component->num_ports;
	oscl_param_check(((index == -1) || (index < num_ports)),
			OMX_ErrorBadPortIndex, NULL);

	if (-1 == index) {
		start_ports = 0;
		end_ports = num_ports - 1;
	}
	for (i = start_ports; i <= end_ports; i++) {
		port = &component->port[i];
		if (is_on) {
			ret = port->enable(port);
			if (component->state == OMX_StateExecuting
					&& PORT_IS_SUPPLIER(port))
				sem_post(component->buf_mgnt_sem);
		} else {
			ret = port->disable(port);
		}
	}
	return ret;
}


/************************************************************************/
/*               COMPONENT INTERFACE                                    */
/************************************************************************/

OMX_ERRORTYPE base_get_version(OMX_IN OMX_HANDLETYPE cmp_hdl,
	OMX_STRING name,
	OMX_VERSIONTYPE *cmp_ver,
	OMX_VERSIONTYPE *spec_ver,
	OMX_UUIDTYPE *uuid)
{
	OMX_ERRORTYPE ret = OMX_ErrorNotImplemented;
	OSCL_TRACE("%p, %p, %p\n", cmp_hdl, name, cmp_ver);

	oscl_param_check((cmp_ver != NULL) && (spec_ver != NULL),
		OMX_ErrorBadParameter, NULL);
	oscl_param_check(cmp_hdl != NULL, OMX_ErrorInvalidComponent, NULL);

	spec_ver->nVersion = OMX_VERSION;
	cmp_ver->nVersion = BASE_COMPONENT_VERSION;
	OSCL_TRACE(" %d\n", ret);
	return ret;
}

OMX_ERRORTYPE base_send_command(OMX_IN OMX_HANDLETYPE cmp_hdl,
	OMX_IN OMX_COMMANDTYPE cmd,
	OMX_IN OMX_U32 param1,
	OMX_IN OMX_PTR cmd_data)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component;
	oscl_message_t message;
	OMX_STATETYPE state;
	int num_ports;
	int i;
	OSCL_TRACE("%p, %x, %x , %p\n", cmp_hdl, cmd, param1, cmd_data);

	oscl_param_check(cmp_hdl != NULL, OMX_ErrorBadParameter, NULL);
	component = get_lb_component(cmp_hdl);
	oscl_param_check(component != NULL, OMX_ErrorBadParameter, NULL);
	oscl_param_check(component->state != OMX_StateInvalid,
		OMX_ErrorInvalidState, NULL);

	state = component->state;
	num_ports = component->num_ports;
	message.comp = component;
	message.msg_type = cmd;
	message.para1 = param1;
	message.data = cmd_data;
	OSCL_LOGD("%s message:%p msg_type:%x header:%p",
		component->name,
		&message, message.msg_type,
		component->port[0].header);
	switch (cmd) {
	case OMX_CommandStateSet:
		component->target_state = param1;
		if ((param1 == OMX_StateIdle) && (state == OMX_StateLoaded))
			ret = __set_ports(component, -1);
		break;
	case OMX_CommandFlush:
		if ((param1 >= component->num_ports) && param1 != OMX_ALL)
			return OMX_ErrorBadPortIndex;
		break;
	case OMX_CommandPortDisable:
	case OMX_CommandPortEnable:
		if ((param1 >= component->num_ports) && param1 != OMX_ALL)
			return OMX_ErrorBadPortIndex;
		if (param1 == OMX_ALL) {
			for (i = 0; i < component->num_ports; i++)
				component->port[i].being_onoff = OMX_TRUE;
		} else {
			component->port[param1].being_onoff = OMX_TRUE;
		}
		break;
	case OMX_CommandMarkBuffer:
		oscl_param_check(
			(state == OMX_StateExecuting || state == OMX_StatePause),
			OMX_ErrorIncorrectStateOperation,
			NULL);
		oscl_param_check(((param1 == OMX_ALL) || (param1 < num_ports)),
			OMX_ErrorBadPortIndex, NULL);
		break;
	default:
		ret = OMX_ErrorUnsupportedIndex;
		break;
	}

	if (ret == OMX_ErrorNone) {
		ret = oscl_message_put(&component->msg_queue, &message);
		if (ret)
			return OMX_ErrorInsufficientResources;
		OSCL_LOGD("==========%p %d\n", component, component->msg_sem->sem->value);
		sem_post(component->msg_sem);
	}

	OSCL_TRACE(" %d\n", ret);

	return ret;
}

OMX_ERRORTYPE base_get_parameter(OMX_IN OMX_HANDLETYPE cmp_hdl,
	OMX_IN OMX_INDEXTYPE index,
	OMX_INOUT OMX_PTR param_data)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	base_port_t *port;
	lb_omx_component_t *component;
	OMX_PARAM_PORTDEFINITIONTYPE *port_def;
	OMX_PARAM_BUFFERSUPPLIERTYPE *supplier;
	OSCL_TRACE(" %p, %x, %p\n", cmp_hdl, index, param_data);

	oscl_param_check((cmp_hdl != NULL) && (param_data != NULL),
		OMX_ErrorBadParameter, NULL);
	component = get_lb_component(cmp_hdl);
	oscl_param_check((component != NULL), OMX_ErrorBadParameter, NULL);

	switch (index) {
	case omx_index_vendor_get_port_number:
		*(int *)param_data = component->num_ports;
		break;

	case OMX_IndexParamPortDefinition:
		port_def = (OMX_PARAM_PORTDEFINITIONTYPE *) param_data;
		oscl_param_check((port_def->nPortIndex < component->num_ports),
			OMX_ErrorBadPortIndex, NULL);
		memcpy(port_def, &component->port[port_def->nPortIndex].port_param,
			sizeof(OMX_PARAM_PORTDEFINITIONTYPE));
		break;
	case OMX_IndexParamCompBufferSupplier:
		supplier = (OMX_PARAM_BUFFERSUPPLIERTYPE *)param_data;
		oscl_param_check((supplier->nPortIndex < component->num_ports),
			OMX_ErrorBadPortIndex, NULL);
		port = &component->port[supplier->nPortIndex];

		if (port->port_param.eDir == OMX_DirInput) {
			if (PORT_IS_SUPPLIER(port))
				supplier->eBufferSupplier = OMX_BufferSupplyInput;
			else if (PORT_IS_TUNNELED(port))
				supplier->eBufferSupplier = OMX_BufferSupplyOutput;
			else
				supplier->eBufferSupplier = OMX_BufferSupplyUnspecified;
		} else {
			if (PORT_IS_SUPPLIER(port))
				supplier->eBufferSupplier = OMX_BufferSupplyOutput;
			else if (PORT_IS_TUNNELED(port))
				supplier->eBufferSupplier = OMX_BufferSupplyInput;
			else
				supplier->eBufferSupplier = OMX_BufferSupplyUnspecified;
		}
		break;
	default:
		ret = OMX_ErrorUnsupportedIndex;
		break;
	}

	OSCL_TRACE(" %x\n", ret);
	return ret;
}

OMX_ERRORTYPE __set_port_def(lb_omx_component_t *component,
	OMX_PTR param_data)
{
	int ret = OMX_ErrorNone;
	OMX_PARAM_PORTDEFINITIONTYPE *port_param;
	OMX_PARAM_PORTDEFINITIONTYPE *port_def;
	OMX_U32 nbuffer_old = 0;
	base_port_t *port;

	set_debug_hdl();
	port_def = (OMX_PARAM_PORTDEFINITIONTYPE *)param_data;
	oscl_param_check(port_def->nPortIndex < component->num_ports,
		OMX_ErrorBadParameter, NULL);
	port = &component->port[port_def->nPortIndex];
	port_param = &port->port_param;
	oscl_param_check(port_def->nBufferCountActual >= port_param->nBufferCountMin,
		OMX_ErrorBadParameter, NULL);
	if (component->state != OMX_StateLoaded
		&& component->state != OMX_StateWaitForResources)
		return OMX_ErrorIncorrectStateOperation;

	nbuffer_old = port_param->nBufferCountActual;
	port_param->nBufferCountActual = port_def->nBufferCountActual;
	port_param->nBufferSize = port_def->nBufferSize;
	port_param->bBuffersContiguous = port_def->bBuffersContiguous;
	port_param->nBufferAlignment = port_def->nBufferAlignment;

	switch (port_def->eDomain) {
	case OMX_PortDomainAudio:
		memcpy(&port_param->format.audio, &port_def->format.audio,
			sizeof(OMX_AUDIO_PORTDEFINITIONTYPE));
		break;

	case OMX_PortDomainVideo:
		memcpy(&port_param->format.video, &port_def->format.video,
			sizeof(OMX_VIDEO_PORTDEFINITIONTYPE));
		break;
	case OMX_PortDomainImage:
		memcpy(&port_param->format.image, &port_def->format.image,
			sizeof(OMX_IMAGE_PORTDEFINITIONTYPE));
		break;
	case OMX_PortDomainOther:
		memcpy(&port_param->format.other, &port_def->format.other,
			sizeof(OMX_OTHER_PORTDEFINITIONTYPE));
		break;
	default:
		ret = OMX_ErrorBadParameter;
		break;
	}

	if (port_param->nBufferCountActual > nbuffer_old) {
		if (port->num_assigned)
			OSCL_LOGE("set port buffer num while assigned != 0");
		if (port->header) {
			oscl_free(port->header);
			port->header = NULL;
		}
		if (port->buf_state) {
			oscl_free(port->buf_state);
			port->buf_state = NULL;
		}
	}
	return ret;
}

OMX_ERRORTYPE __set_buffer_supplier(lb_omx_component_t *component,
	OMX_PTR param_data)
{
	OMX_PARAM_BUFFERSUPPLIERTYPE *buf_supplier;
	base_port_t *port;

	set_debug_hdl();
	buf_supplier = (OMX_PARAM_BUFFERSUPPLIERTYPE *)param_data;
	OSCL_TRACE(" %p, %d\n", component, buf_supplier->nPortIndex);

	port = &component->port[buf_supplier->nPortIndex];
	oscl_param_check(buf_supplier->nPortIndex < component->num_ports,
		OMX_ErrorBadParameter, NULL);
	if (component->state != OMX_StateLoaded
		&& component->state != OMX_StateWaitForResources
		&& PORT_IS_ENABLED(port))
		return OMX_ErrorIncorrectStateOperation;

	if (buf_supplier->eBufferSupplier == OMX_BufferSupplyUnspecified)
		return OMX_ErrorNone;

	if ((PORT_IS_TUNNELED(port)) == 0)
		return OMX_ErrorNone;

	port->tunnel_flags &= ~TUNNEL_IS_SUPPLIER;
	if ((buf_supplier->eBufferSupplier == OMX_BufferSupplyInput)
		&& (port->port_param.eDir == OMX_DirInput))
		port->tunnel_flags |= TUNNEL_IS_SUPPLIER;

	if ((buf_supplier->eBufferSupplier == OMX_BufferSupplyOutput)
		&& (port->port_param.eDir == OMX_DirOutput))
		port->tunnel_flags |= TUNNEL_IS_SUPPLIER;
	return OMX_ErrorNone;
}


OMX_ERRORTYPE base_set_parameter(OMX_IN OMX_HANDLETYPE cmp_hdl,
	OMX_IN OMX_INDEXTYPE index,
	OMX_IN OMX_PTR param_data)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component;
	OMX_PRIORITYMGMTTYPE *priority = param_data;
	struct sched_param shed_param = {0};
	OSCL_TRACE(" %p, %p , %d\n", cmp_hdl, param_data, index);

	oscl_param_check((cmp_hdl != NULL) && (param_data != NULL),
		OMX_ErrorBadParameter, NULL);
	component = get_lb_component(cmp_hdl);
	oscl_param_check((component != NULL), OMX_ErrorBadParameter, NULL);

	switch (index) {
	case OMX_IndexParamPortDefinition:
		ret = __set_port_def(component, param_data);
		break;

	case OMX_IndexParamCompBufferSupplier:
		__set_buffer_supplier(component, param_data);
		break;

	case OMX_IndexParamPriorityMgmt:
		shed_param.sched_priority = priority->nGroupPriority;
		pthread_attr_setschedparam(&component->buf_thread_attr, &shed_param);
		break;

	default:
		ret = OMX_ErrorUnsupportedIndex;
		break;
	}

	OSCL_TRACE(" %d\n", ret);
	return ret;
}

OMX_ERRORTYPE base_get_config(OMX_IN OMX_HANDLETYPE cmp_hdl,
	OMX_IN OMX_INDEXTYPE index,
	OMX_INOUT OMX_PTR data)
{
	OMX_ERRORTYPE ret = OMX_ErrorNotImplemented;
	OSCL_TRACE(" %p, %x, %p\n", cmp_hdl, index, data);
	OSCL_TRACE(" %d\n", ret);
	return ret;
}

OMX_ERRORTYPE base_set_config(OMX_IN OMX_HANDLETYPE cmp_hdl,
	OMX_IN OMX_INDEXTYPE index,
	OMX_IN OMX_PTR data)
{
	OMX_ERRORTYPE ret = OMX_ErrorNotImplemented;
	OSCL_TRACE(" %p, %x, %p\n", cmp_hdl, index, data);
	OSCL_TRACE(" %d\n", ret);
	return ret;
}

OMX_ERRORTYPE base_get_extension_index(OMX_IN OMX_HANDLETYPE cmp_hdl,
	OMX_IN OMX_STRING name,
	OMX_OUT OMX_INDEXTYPE * type)
{
	OMX_ERRORTYPE ret = OMX_ErrorNotImplemented;
	return ret;
}

OMX_ERRORTYPE base_get_state(OMX_IN OMX_HANDLETYPE cmp_hdl,
	OMX_OUT OMX_STATETYPE *state)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component = NULL;
	OSCL_TRACE(" %p, %p\n", cmp_hdl, state);

	oscl_param_check((cmp_hdl != NULL) && (state != NULL),
		OMX_ErrorBadParameter, NULL);
	component = get_lb_component(cmp_hdl);
	oscl_param_check(component != NULL, OMX_ErrorBadParameter, NULL);
	*state = component->state;
	OSCL_TRACE(" ret:%d, state:%d\n", ret, *state);
	return ret;
}

OMX_ERRORTYPE base_tunnel_request(OMX_IN OMX_HANDLETYPE cmp_hdl,
	OMX_IN OMX_U32 port,
	OMX_IN OMX_HANDLETYPE peer_cmp_hdl,
	OMX_IN OMX_U32 peer_port,
	OMX_INOUT OMX_TUNNELSETUPTYPE * tunnel_setup)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component = NULL;
	base_port_t *base_port;
	OSCL_TRACE(" %p, %d\n", cmp_hdl, port);

	oscl_param_check((cmp_hdl != NULL), OMX_ErrorBadParameter, NULL);
	component = get_lb_component(cmp_hdl);
	oscl_param_check(component != NULL, OMX_ErrorBadParameter, NULL);
	oscl_param_check((port < component->num_ports),
		OMX_ErrorBadPortIndex, NULL);
	base_port = &component->port[port];
	ret = base_port->tunnel_request(base_port,
			peer_cmp_hdl, peer_port, tunnel_setup);

	OSCL_TRACE(" %p, %d, ret:%x\n", cmp_hdl, port, ret);
	return ret;
}

OMX_ERRORTYPE base_use_buffer(OMX_IN OMX_HANDLETYPE cmp_hdl,
	OMX_BUFFERHEADERTYPE **header,
	OMX_U32 port_index,
	OMX_PTR data,
	OMX_U32 bytes,
	OMX_U8 *buffer)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	OMX_COMPONENTTYPE *base_cmp = (OMX_COMPONENTTYPE *)cmp_hdl;
	lb_omx_component_t *component = NULL;
	base_port_t *port;
	OSCL_TRACE(" %p %x %d %p %d %p\n", cmp_hdl, header,
		port_index, data, bytes, buffer);

	oscl_param_check(base_cmp != NULL, OMX_ErrorBadParameter, NULL);
	component = get_lb_component(cmp_hdl);
	oscl_param_check(component != NULL, OMX_ErrorBadParameter, NULL);
	oscl_param_check(port_index < component->num_ports, OMX_ErrorBadParameter, NULL);
	port = &component->port[port_index];
	ret = port->use_buffer(port, header, data, bytes, buffer);

	OSCL_TRACE("%p %x %d\n", cmp_hdl, header, ret);
	return ret;
}


OMX_ERRORTYPE base_allocate_buffer(OMX_IN OMX_HANDLETYPE cmp_hdl,
	OMX_INOUT OMX_BUFFERHEADERTYPE **header,
	OMX_IN OMX_U32 port_index,
	OMX_IN OMX_PTR data,
	OMX_IN OMX_U32 bytes)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component;
	base_port_t *port;
	OSCL_TRACE(" %p %x %p %d\n", cmp_hdl, header, data, bytes);

	oscl_param_check(cmp_hdl != NULL, OMX_ErrorBadParameter, NULL);
	component = get_lb_component(cmp_hdl);
	oscl_param_check(component != NULL, OMX_ErrorBadParameter, NULL);
	port = &component->port[port_index];
	ret = port->allocate_buffer(port, header, data, bytes);

	OSCL_TRACE("%p %x %d\n", cmp_hdl, header, ret);
	return ret;
}


OMX_ERRORTYPE base_free_buffer(OMX_IN OMX_HANDLETYPE cmp_hdl,
	OMX_IN OMX_U32 port_index,
	OMX_IN OMX_BUFFERHEADERTYPE *buffer)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component;
	base_port_t *port;
	OSCL_TRACE(" %p %x %p\n", cmp_hdl, port_index, buffer);

	oscl_param_check(cmp_hdl != NULL, OMX_ErrorBadParameter, NULL);
	component = get_lb_component(cmp_hdl);
	oscl_param_check(component != NULL, OMX_ErrorBadParameter, NULL);
	port = &component->port[port_index];
	ret = port->free_buffer(port, buffer);

	OSCL_TRACE("%p %x %x\n", cmp_hdl, port_index, ret);
	return ret;
}


OMX_ERRORTYPE base_empty_this_buffer(OMX_IN OMX_HANDLETYPE cmp_hdl,
	OMX_IN OMX_BUFFERHEADERTYPE *header)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component;
	base_port_t *port;
	OSCL_TRACE(" %p %p\n", cmp_hdl, header);

	oscl_param_check((cmp_hdl != NULL) && (header != NULL),
		OMX_ErrorBadParameter, NULL);
	component = get_lb_component(cmp_hdl);
	oscl_param_check(component != NULL, OMX_ErrorBadParameter, NULL);
	oscl_param_check(header->nInputPortIndex < component->num_ports,
		OMX_ErrorBadParameter,
		NULL);
	port = &component->port[header->nInputPortIndex];
	oscl_param_check(port->port_param.eDir == OMX_DirInput,
		OMX_ErrorBadParameter, NULL);

	oscl_param_check((OMX_StateExecuting == get_component_state(port) ||
		OMX_StatePause == get_component_state(port)),
		OMX_ErrorIncorrectStateOperation, "IncorrectState");
	oscl_param_check((PORT_IS_ENABLED(port)),
		OMX_ErrorIncorrectStateOperation, "IncorrectState");
	oscl_param_check((!(port->is_flushed)),
		OMX_ErrorIncorrectStateOperation, "IncorrectState");

	ret = port->recive_buffer(port, header);
	OSCL_TRACE("%p %x %d\n", cmp_hdl, header, ret);
	return ret;
}

OMX_ERRORTYPE base_fill_this_buffer(OMX_IN OMX_HANDLETYPE cmp_hdl,
	OMX_IN OMX_BUFFERHEADERTYPE *header)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component;
	base_port_t *port;
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
	ret = port->recive_buffer(port, header);
	OSCL_LOGI("%s(%p) %x %d\n", component->name, cmp_hdl, header, ret);
	return ret;
}

OMX_ERRORTYPE base_set_callbacks(OMX_IN OMX_HANDLETYPE cmp_hdl,
	OMX_CALLBACKTYPE *callbacks,
	OMX_PTR data)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component;
	base_port_t *port;
	int i;
	OSCL_TRACE(" %p, %p , %p\n", cmp_hdl, callbacks, data);

	oscl_param_check(cmp_hdl != NULL, OMX_ErrorBadParameter, NULL);
	component = get_lb_component(cmp_hdl);
	oscl_param_check(component != NULL, OMX_ErrorBadParameter, NULL);
	component->callbacks = *callbacks;
	component->callback_data = data;
	for (i = 0; i < component->num_ports; i++) {
		port = &component->port[i];
		if (port->port_param.eDir == OMX_DirInput)
			port->buf_done_callback = callbacks->EmptyBufferDone;
		else
			port->buf_done_callback = callbacks->FillBufferDone;
	}

	OSCL_TRACE(" %d\n", ret);
	return ret;
}

OMX_ERRORTYPE base_component_deinit(OMX_IN OMX_HANDLETYPE cmp_hdl)
{
	OMX_COMPONENTTYPE *base_cmp = (OMX_COMPONENTTYPE *)cmp_hdl;
	lb_omx_component_t *component;

	OSCL_TRACE("base_cmp_handle:%p\n", cmp_hdl);
	oscl_param_check(cmp_hdl != NULL, OMX_ErrorBadParameter, NULL);
	component = (lb_omx_component_t *)base_cmp->pComponentPrivate;
	component->state = OMX_StateInvalid;
	component->callbacks.EmptyBufferDone = NULL;
	component->callbacks.FillBufferDone = NULL;
	component->callbacks.EventHandler = NULL;
	component->dbg_flag = set_debug_state(component->dbg_flag,
			DEBUG_BUF_MGNT_SHT, DEBUG_THREAD_STOP);
	if (component->msg_handler_thread_id == 0) {
		OSCL_LOGD("oscl_queue_flush");
		oscl_queue_flush(&component->msg_queue);
		OSCL_LOGD("oscl_queue_flush sem_post");
		sem_post(component->msg_sem);
		pthread_join(component->msg_handler_thread, NULL);
		component->msg_handler_thread_id = -1;
		component->msg_handler_thread = NULL;
	}
	if (component->msg_sem) {
		sem_destroy(component->msg_sem);
		oscl_free(component->msg_sem);
		component->msg_sem = NULL;
	}
	if (component->buf_mgnt_sem) {
		sem_destroy(component->buf_mgnt_sem);
		oscl_free(component->buf_mgnt_sem);
		component->buf_mgnt_sem = NULL;
	}

	if (component->flush_sem) {
		sem_destroy(component->flush_sem);
		oscl_free(component->flush_sem);
		component->flush_sem = NULL;
	}
	if (component->mgmt_flush_sem) {
		sem_destroy(component->mgmt_flush_sem);
		oscl_free(component->mgmt_flush_sem);
		component->mgmt_flush_sem = NULL;
	}
	oscl_queue_deinit(&component->msg_queue);
	pthread_mutex_destroy(&component->flush_mutex);
	OSCL_TRACE("base_cmp_handle:%p\n", cmp_hdl);
	return OMX_ErrorNone;
}

OMX_ERRORTYPE
base_use_EGL_image(OMX_IN OMX_HANDLETYPE cmp_hdl,
	OMX_INOUT OMX_BUFFERHEADERTYPE **header,
	OMX_IN OMX_U32 port,
	OMX_IN OMX_PTR data,
	OMX_IN void *egl)
{
	return OMX_ErrorNotImplemented;
}

OMX_ERRORTYPE
base_role_enum(OMX_IN OMX_HANDLETYPE comp,
	OMX_U8 *role,
	OMX_U32 index)
{
	return OMX_ErrorNotImplemented;
}


/** This is the central function for component processing. It
 * is executed in a separate thread, is synchronized with
 * semaphores at each port, those are released each time a new buffer
 * is available on the given port.
 */
void *base_component_buffer_manager(void *param)
{
	lb_omx_component_t *component;
	base_port_t *inport;
	base_port_t *outport;
	OMX_BUFFERHEADERTYPE *outbuffer = NULL;
	OMX_BUFFERHEADERTYPE *inbuffer = NULL;
	OMX_ERRORTYPE ret;
	OMX_HANDLETYPE cmp_hdl = param;

	OSCL_TRACE(" %p\n", param);
	oscl_param_check((param != NULL), NULL, NULL);
	component = get_lb_component(cmp_hdl);
	inport = &component->port[BASE_INPUT_PORT];
	outport = &component->port[BASE_OUTPUT_PORT];
	component->dbg_flag = set_debug_state(component->dbg_flag,
			DEBUG_BUF_MGNT_SHT, DEBUG_THREAD_START);

	/* checks if the component is in a state able to receive buffers */
	while (component->state == OMX_StateIdle
		|| component->state == OMX_StateExecuting
		|| component->state == OMX_StatePause) {
		/*Wait till the ports are being flushed*/
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
				outport->recive_buffer(outport, outbuffer);
				outbuffer = NULL;
			}
			if (inbuffer && inport->is_flushed) {
				ret = inport->return_buffer(inport, inbuffer);
				if (OMX_ErrorNone != ret)
					OSCL_LOGE("component (%s) state error.",
						get_port_name(inport));
				inbuffer = NULL;
			}

			sem_post(component->mgmt_flush_sem);
			OSCL_LOGI("==%s %d\n", __FILE__, __LINE__);
			if (oscl_sem_timedwait_ms(component->flush_sem, 15000))
				OSCL_LOGE("wait sem timeout!");
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
		if (outbuffer == NULL || inbuffer == NULL) {
			OSCL_LOGD("waiting buffer:%x %x\n", inbuffer, outbuffer);
			OSCL_LOGD("==========%d\n", component->buf_mgnt_sem->sem->value);
			sem_wait(component->buf_mgnt_sem);
			continue;
		}
		if (component->mark.hMarkTargetComponent) {
			outbuffer->hMarkTargetComponent
				= component->mark.hMarkTargetComponent;
			outbuffer->pMarkData
				= component->mark.pMarkData;
			component->mark.hMarkTargetComponent = NULL;
			component->mark.pMarkData = NULL;
		}

		outbuffer->nFilledLen = 0;
		outbuffer->nFlags = inbuffer->nFlags;
		if (component->state == OMX_StateExecuting) {
			if (component->buf_handle && inbuffer->nFilledLen > 0)
				((component->buf_handle))(component, inbuffer, outbuffer);
			else
				inbuffer->nFilledLen = 0;
		}
		if (inbuffer->hMarkTargetComponent != NULL) {
			if ((OMX_COMPONENTTYPE *)inbuffer->hMarkTargetComponent
				== &component->base_comp) {
				/*Clear the mark and generate an event*/
				((component->callbacks.EventHandler))
				(&component->base_comp,
					component->callback_data, OMX_EventMark,
					0, 0, inbuffer->pMarkData);
			} else {
				/* pass the mark*/
				outbuffer->hMarkTargetComponent
					= inbuffer->hMarkTargetComponent;
				outbuffer->pMarkData
					= inbuffer->pMarkData;
			}
			inbuffer->hMarkTargetComponent = NULL;
		}

		base_check_eos(component, outport, outbuffer);

		if ((outbuffer->nFilledLen != 0)
			|| (outbuffer->nFlags & OMX_BUFFERFLAG_EOS)) {
			outport->return_buffer(outport, outbuffer);
			outbuffer = NULL;
		}
		if (inbuffer->nFilledLen == 0) {
			inport->return_buffer(inport, inbuffer);
			inbuffer = NULL;
		}
	}
	OSCL_LOGW("exit from buffer_manager:%s\n", rt_thread_self()->name);
	OSCL_TRACE(" %p\n", param);
	component->dbg_flag = set_debug_state(component->dbg_flag,
			DEBUG_BUF_MGNT_SHT, DEBUG_THREAD_EXIT);
	return NULL;
}

void *base_component_message_thread(void *param)
{
	lb_omx_component_t *component = param;
	oscl_message_t message;
	int ret;

	set_debug_hdl();
	OSCL_TRACE(" %p\n", param);
	oscl_param_check(param != NULL, NULL, NULL);
	component = get_lb_component(param);
	component->dbg_flag = set_debug_state(component->dbg_flag,
			DEBUG_MSG_SHT, DEBUG_THREAD_START);

	while (1) {
		/* Wait for an incoming message */
		component->dbg_wdog = 0;
		sem_wait(component->msg_sem);

		/*Destructor has been called. So exit from the loop*/
		if (component->state == OMX_StateInvalid) {
			OSCL_LOGI("exit from the msg handle\n");
			break;
		}
		/* Dequeue it */
		ret = oscl_message_get(&component->msg_queue, &message);
		if (ret) {
			OSCL_LOGE("get message failed\n");
			continue;
		}
		/* Process it by calling component's message handler method */
		if (component->msg_handle)
			component->msg_handle(param, &message);
	}
	OSCL_LOGD("base message thread exit.");
	component->dbg_flag = set_debug_state(component->dbg_flag,
			DEBUG_MSG_SHT, DEBUG_THREAD_EXIT);
	return NULL;
}

OMX_ERRORTYPE base_component_message_handle(OMX_HANDLETYPE cmp_hdl,
	oscl_message_t *message)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	OMX_EVENTTYPE event = OMX_EventMax;
	OMX_U32 event_data1 = OMX_ErrorNone;
	OMX_U32 event_data2 = 0;
	lb_omx_component_t *component;

	OSCL_TRACE("%p, %x\n", cmp_hdl, message);
	oscl_param_check(cmp_hdl != NULL, OMX_ErrorBadParameter, NULL);
	component = get_lb_component(cmp_hdl);
	OSCL_LOGD("%s message->msg_type:%s", component->name,
		omx_cmd_as_string(message->msg_type));
	switch (message->msg_type) {
	case OMX_CommandStateSet:
		/* Do the actual state change */
		OSCL_LOGD("OMX_CommandStateSet %p %d",
			component->do_state_set, message->para1);
		if (component->do_state_set) {
			ret = component->do_state_set(cmp_hdl, message->para1);
			event = OMX_EventCmdComplete;
			event_data1 = OMX_CommandStateSet;
			event_data2 = message->para1;
		}
		break;

	case OMX_CommandFlush:
		ret = __flush_ports(component, message->para1);
		break;

	case OMX_CommandPortDisable:
		if (component->state != OMX_StateLoaded)
			ret = __flush_ports(component, message->para1);
		ret = __ports_onoff(component, message->para1, 0);
		break;

	case OMX_CommandPortEnable:
		ret = __ports_onoff(component, message->para1, 1);
		break;
	case OMX_CommandMarkBuffer:
		component->mark.hMarkTargetComponent = ((OMX_MARKTYPE *)
				message->data)->hMarkTargetComponent;
		component->mark.pMarkData = ((OMX_MARKTYPE *)message->data)->pMarkData;
		break;
	default:
		break;
	}

	if (ret == OMX_ErrorNone) {
		event = OMX_EventCmdComplete;
		event_data1 = message->msg_type;
		event_data2 = message->para1;
	} else {
		event = OMX_EventError;
		event_data1 = ret;
		event_data2 = message->msg_type;
	}
	(component->callbacks.EventHandler)(&component->base_comp,
		component->callback_data,
		event,
		event_data1,
		event_data2,
		NULL);
	OSCL_TRACE(" %p,%x,%x\n", component, message, ret);
	return ret;
}

OMX_ERRORTYPE base_component_set_state(OMX_HANDLETYPE cmp_hdl,
	OMX_U32 dest_state)
{
	base_port_t *port;
	OMX_U32 i;
	OMX_ERRORTYPE ret = OMX_ErrorIncorrectStateTransition;
	lb_omx_component_t *component;

	OSCL_TRACE(" %p, %x\n", cmp_hdl, dest_state);
	oscl_param_check(cmp_hdl != NULL, OMX_ErrorBadParameter, NULL);
	component = get_lb_component(cmp_hdl);
	OSCL_TRACE(" %s, set state:%s->%s\n",
		get_component_name(component),
		omx_state_as_string(component->state),
		omx_state_as_string(dest_state));
	oscl_param_check_exit((component->state != OMX_StateInvalid),
		OMX_ErrorInvalidState, NULL);
	oscl_param_check_exit((component->state != OMX_StateWaitForResources),
		OMX_ErrorInvalidState, NULL);
	oscl_param_check_exit((dest_state != OMX_StateWaitForResources),
		OMX_ErrorInvalidState, NULL);
	oscl_param_check_exit((dest_state != component->state),
		OMX_ErrorSameState, NULL);

	if (dest_state == OMX_StateLoaded && component->state == OMX_StateIdle) {
		OSCL_LOGD("idle->load 1");
		component->state = OMX_StateLoaded;
		if (component->buf_mgnt_thread_id == 0) {
			sem_post(component->buf_mgnt_sem);
			pthread_join(component->buffer_mgmt_thread, NULL);
			component->buf_mgnt_thread_id = -1;
			component->buffer_mgmt_thread = NULL;
		}
		OSCL_LOGD("idle->load 2, port num:%d", component->num_ports);
		for (i = 0; i < component->num_ports; i++) {
			port = &component->port[i];
			if (PORT_IS_SUPPLIER(port)) {
				OSCL_LOGD("idle->load 2.1");
				while (oscl_queue_dequeue(&port->buffer_queue))
					continue;
				OSCL_LOGD("idle->load 2.2");
				ret = port->free_tunnel_buffer(port);
				oscl_param_check_exit((ret == OMX_ErrorNone),
					ret, NULL);
			} else if (PORT_IS_ENABLED(port)
				   && port->port_param.bPopulated == OMX_TRUE
				   && port->header != NULL){
				OSCL_LOGD("%s, tun:%d header:%p populated:%d",
					   component->name, PORT_IS_TUNNELED(port),
					   port->header, port->port_param.bPopulated);
				if (oscl_sem_timedwait_ms(&port->populate_sem, 5000))
					OSCL_LOGE("wait sem timeout!");
				OSCL_LOGD("idle->load 2.4");
			}
			port->port_param.bPopulated = OMX_FALSE;
			OSCL_LOGD("idle->load 2.5");
			if (port->header != NULL) {
				oscl_free(port->header);
				port->header = NULL;
			}
			OSCL_LOGD("idle->load 2.6");
			if (port->buf_state != NULL) {
				oscl_free(port->buf_state);
				port->buf_state = NULL;
			}
			OSCL_LOGD("idle->load 2.7");
		}
		OSCL_LOGD("idle->load 3");
		ret = OMX_ErrorNone;
	}

	if (dest_state == OMX_StateIdle) {
		switch (component->state) {
		case OMX_StateLoaded:
			/* for all ports */
			for (i = 0; i < component->num_ports; i++) {
				port = &component->port[i];
				if (!PORT_IS_ENABLED(port))
					continue;
				if (PORT_IS_SUPPLIER(port)) {
					ret = port->allocate_tunnel_buffer(port);
					if (ret != OMX_ErrorNone) {
						OSCL_LOGE("allocate Buffer Error");
						return ret;
					}
					continue;
				}
				OSCL_LOGI("%s wait for buf.\n", get_port_name(port));
				if (PORT_IS_TUNNELED(port)
					&& (port->port_param.nBufferCountActual > 0))
					sem_wait(&port->populate_sem);
				OSCL_LOGI("get buffers.\n");
			}
			component->state = OMX_StateIdle;
			/** starting buffer management thread */
			component->buf_mgnt_thread_id = pthread_create(
					&component->buffer_mgmt_thread,
					&component->buf_thread_attr,
					component->buf_manager,
					cmp_hdl);
			OSCL_LOGI("component name:%s buf thread name:%s", component->name,
				component->buffer_mgmt_thread->name);
			if (component->buf_mgnt_thread_id < 0) {
				component->buffer_mgmt_thread = NULL;
				OSCL_LOGE("starting buffer management thread failed\n");
				return OMX_ErrorUndefined;
			}
			ret = OMX_ErrorNone;
			break;
		case OMX_StateExecuting:
		case OMX_StatePause:
			OSCL_LOGI("==%s %d\n", __FILE__, __LINE__);
			__flush_ports(component, -1);
			component->state = OMX_StateIdle;
			sem_post(component->buf_mgnt_sem);
			ret = OMX_ErrorNone;
			break;
		default:
			OSCL_LOGE("state transition not allowed\n");
			ret = OMX_ErrorIncorrectStateTransition;
			break;
		}
	}

	if (dest_state == OMX_StatePause) {
		if (component->state == OMX_StateIdle
			|| component->state == OMX_StateExecuting) {
			component->state = OMX_StatePause;
			ret = OMX_ErrorNone;
		}
	}

	if (dest_state == OMX_StateExecuting) {
		if (component->state == OMX_StateIdle
			|| component->state == OMX_StatePause) {
			component->state = OMX_StateExecuting;
			ret = OMX_ErrorNone;
		}
		sem_post(component->buf_mgnt_sem);
	}

	if (dest_state == OMX_StateInvalid) {
		OSCL_LOGW("set to invalid state");
		component->state = OMX_StateInvalid;
		if (component->buf_mgnt_thread_id == 0) {
			sem_post(component->buf_mgnt_sem);
			pthread_join(component->buffer_mgmt_thread, NULL);
			component->buf_mgnt_thread_id = -1;
			component->buffer_mgmt_thread = NULL;
		}
		ret = OMX_ErrorInvalidState;
	}
EXIT:
	OSCL_TRACE("comp:%s, state:%s, ret:%x\n", get_component_name(component),
		omx_state_as_string(component->state), ret);
	return ret;
}


OMX_ERRORTYPE base_component_init(lb_omx_component_t *cmp_handle)
{
	lb_omx_component_t *component = (lb_omx_component_t *)cmp_handle;
	OMX_COMPONENTTYPE *base_cmp;
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	pthread_attr_t msg_thread_attr;
	struct sched_param shed_param = {0};

	set_debug_hdl();
	OSCL_TRACE("base_cmp_handle:%p\n", cmp_handle);
	oscl_param_check(cmp_handle != NULL, OMX_ErrorBadParameter, NULL);
	memset(cmp_handle, 0, sizeof(lb_omx_component_t));

	base_cmp = &(component->base_comp);
	component->log_level = DBG_WARNING;
	component->state = OMX_StateLoaded;
	component->target_state = OMX_StateLoaded;
	component->buf_mgnt_thread_id = -1;
	component->msg_handler_thread_id = -1;
	component->msg_sem = (sem_t *)oscl_zalloc(sizeof(sem_t));
	oscl_param_check_exit(component->msg_sem != NULL,
		OMX_ErrorInsufficientResources, NULL);
	sem_init(component->msg_sem, 0, 0);

	component->buf_mgnt_sem = (sem_t *)oscl_zalloc(sizeof(sem_t));
	oscl_param_check_exit(component->buf_mgnt_sem != NULL,
		OMX_ErrorInsufficientResources,
		NULL);
	sem_init(component->buf_mgnt_sem, 0, 0);

	component->flush_sem = (sem_t *)oscl_zalloc(sizeof(sem_t));
	oscl_param_check_exit(component->flush_sem != NULL,
		OMX_ErrorInsufficientResources, NULL);
	sem_init(component->flush_sem, 0, 0);

	component->mgmt_flush_sem = (sem_t *)oscl_zalloc(sizeof(sem_t));
	oscl_param_check_exit(component->mgmt_flush_sem != NULL,
		OMX_ErrorInsufficientResources, NULL);
	sem_init(component->mgmt_flush_sem, 0, 0);

	oscl_queue_init(&component->msg_queue);
	pthread_mutex_init(&component->flush_mutex, NULL);

	memset(base_cmp, 0, sizeof(OMX_COMPONENTTYPE));
	base_cmp->nSize = sizeof(OMX_COMPONENTTYPE);
	base_cmp->nVersion.nVersion = OMX_VERSION;
	base_cmp->pApplicationPrivate = 0;
	base_cmp->pComponentPrivate = component;

	base_cmp->AllocateBuffer = base_allocate_buffer;
	base_cmp->FreeBuffer = base_free_buffer;
	base_cmp->GetParameter = base_get_parameter;
	base_cmp->SetParameter = base_set_parameter;
	base_cmp->SendCommand = base_send_command;
	base_cmp->FillThisBuffer = base_fill_this_buffer;
	base_cmp->EmptyThisBuffer = base_empty_this_buffer;
	base_cmp->GetState = base_get_state;
	base_cmp->GetComponentVersion = base_get_version;
	base_cmp->GetConfig = base_get_config;
	base_cmp->SetConfig = base_set_config;
	base_cmp->GetExtensionIndex = base_get_extension_index;
	base_cmp->ComponentTunnelRequest = base_tunnel_request;
	base_cmp->UseBuffer = base_use_buffer;
	base_cmp->SetCallbacks = base_set_callbacks;
	base_cmp->UseEGLImage = base_use_EGL_image;
	base_cmp->ComponentRoleEnum = base_role_enum;
	base_cmp->ComponentDeInit = base_component_deinit;

	cmp_handle->name = "common component";
	cmp_handle->buf_manager = base_component_buffer_manager;
	cmp_handle->msg_handle = base_component_message_handle;
	cmp_handle->do_state_set = base_component_set_state;
	pthread_attr_init(&component->buf_thread_attr);
	pthread_attr_setstacksize(&component->buf_thread_attr, 0x1000);

	pthread_attr_init(&msg_thread_attr);
	pthread_attr_setstacksize(&msg_thread_attr, 0x1000);
	shed_param.sched_priority = 15;
	pthread_attr_setschedparam(&msg_thread_attr, &shed_param);
	cmp_handle->msg_handler_thread_id = pthread_create(
			&cmp_handle->msg_handler_thread, &msg_thread_attr,
			base_component_message_thread, base_cmp);
	OSCL_LOGI("component name:%s thread name:%s %d", cmp_handle->name,
		cmp_handle->msg_handler_thread->name, shed_param.sched_priority);
	if (cmp_handle->msg_handler_thread_id < 0)
		return OMX_ErrorInsufficientResources;

	return OMX_ErrorNone;

EXIT:
	if (component->msg_sem) {
		sem_destroy(component->msg_sem);
		oscl_free(component->msg_sem);
		component->msg_sem = NULL;
	}
	if (component->buf_mgnt_sem) {
		sem_destroy(component->buf_mgnt_sem);
		oscl_free(component->buf_mgnt_sem);
		component->buf_mgnt_sem = NULL;
	}

	if (component->flush_sem) {
		sem_destroy(component->flush_sem);
		oscl_free(component->flush_sem);
		component->flush_sem = NULL;
	}
	if (component->mgmt_flush_sem) {
		sem_destroy(component->mgmt_flush_sem);
		oscl_free(component->mgmt_flush_sem);
		component->mgmt_flush_sem = NULL;
	}
	OSCL_TRACE("base_cmp_handle:%d\n", ret);
	return ret;
}


void base_show_component(OMX_HANDLETYPE pHandle)
{
	lb_omx_component_t *component = NULL;
	base_port_t *port = NULL;
	int tmp;
	int i, j;

	/* param check*/
	component = get_lb_component(pHandle);

	OSCL_PRINT("=============%s(%p)=============\n",
		get_component_name(component), pHandle);
	OSCL_PRINT("state:%x, target state:%x, debug flag:%x, msg queue:%d\n",
		component->state, component->target_state, component->dbg_flag,
		oscl_queue_get_num(&component->msg_queue));
	if (component->msg_handler_thread) {
		sem_getvalue(component->msg_sem, &tmp);
		OSCL_PRINT("message thread id:%d, name:%s\n",
			component->msg_handler_thread_id,
			component->msg_handler_thread->name);
		OSCL_PRINT("message sem:%d, name:%s\n",
			tmp, component->msg_sem->sem->parent.parent.name);
	}
	if (component->buffer_mgmt_thread) {
		sem_getvalue(component->buf_mgnt_sem, &tmp);
		OSCL_PRINT("buffer manager thread id:%d, name:%s, sem:%d\n",
			component->buf_mgnt_thread_id,
			component->buffer_mgmt_thread->name, tmp);
		OSCL_PRINT("buffer manager sem:%d, name:%s\n",
			tmp, component->buf_mgnt_sem->sem->parent.parent.name);
		sem_getvalue(component->flush_sem, &tmp);
		OSCL_PRINT("flush_sem sem:%d, name:%s\n",
			tmp, component->flush_sem->sem->parent.parent.name);
	}
	OSCL_PRINT("eos flag:%x\n", component->eos_flag);
	OSCL_PRINT("====number of ports:%d====\n", component->num_ports);
	for (i = 0; i < component->num_ports; i++) {
		port = &component->port[i];
		OSCL_PRINT("**** info of port %d ****\n", i);
		OSCL_PRINT("tunnel flags:%x, comp:%x, port:%d\n", port->tunnel_flags,
			port->tunneled_component, port->tunnel_ports);
		OSCL_PRINT("enabled:%d being onoff:%x, is flushed:%d\n",
			port->port_param.bEnabled, port->being_onoff, port->is_flushed);
		sem_getvalue(&port->populate_sem, &tmp);
		OSCL_PRINT("populate_sem:%d\n", tmp);
		sem_getvalue(&port->buffer_sem, &tmp);
		OSCL_PRINT("buffer_sem:%d\n", tmp);
		oscl_queue_list(&port->buffer_queue);
		OSCL_PRINT("buffer assigned:%d\n", (port->num_assigned));
		for (j = 0; j < port->num_assigned; j++) {
			OSCL_PRINT("buffer[%d], ", j);
			if (port->buf_state)
				OSCL_PRINT("state:%x ", port->buf_state[j]);
			if (port->header && port->header[j]) {
				OSCL_PRINT("buf:%x(%x), flag:%x, ports:%d-%d ",
					port->header[j], port->header[j]->pBuffer,
					port->header[j]->nFlags,
					port->header[j]->nInputPortIndex,
					port->header[j]->nOutputPortIndex);
				OSCL_PRINT("size:%d, fill:%d, offset:%d, flag:%x ",
					port->header[j]->nAllocLen,
					port->header[j]->nFilledLen,
					port->header[j]->nOffset,
					port->header[j]->nFlags);
				OSCL_PRINT("tick:%d, time:%x",
					port->header[j]->nTickCount,
					port->header[j]->nTimeStamp);
			}
			OSCL_PRINT("\n");
			OSCL_PRINT("************************\n");
		}
		OSCL_PRINT("\n");
	}
	OSCL_PRINT("==================================\n\n\n");
}

