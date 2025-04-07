#define DBG_LEVEL		DBG_ERR

#include <base_component.h>
#include "clock_port.h"

static OMX_ERRORTYPE clock_port_flush_buffer(base_port_t *port)
{
	OMX_BUFFERHEADERTYPE *buf_head = NULL;

	OSCL_TRACE("%p(%s, %d)", port, get_port_name(port), port->port_param.nPortIndex);
	if (PORT_IS_SUPPLIER(port)) {
		OSCL_LOGI("%p(%s, %d)", port,
			get_port_name(port), port->port_param.nPortIndex);
		/* while (oscl_queue_get_num(&port->buffer_queue) != port->num_assigned) {
			OSCL_LOGD("(%s)%d queue num:%d assigned:%d",
				get_port_name(port), port->port_param.nPortIndex,
				oscl_queue_get_num(&port->buffer_queue),
				port->num_assigned);
			sem_wait(&port->buffer_sem);
		} */
		OSCL_LOGI("%p(%s, %d)", port,
			get_port_name(port), port->port_param.nPortIndex);
	} else {
		OSCL_LOGI(" %p", port);
		do {
			buf_head = oscl_queue_dequeue(&port->buffer_queue);
			OSCL_LOGI("(%s)flush buffer:%x\n", get_port_name(port), buf_head);
			if (buf_head != NULL)
				port->return_buffer(port, buf_head);
		} while (buf_head != NULL);
		OSCL_LOGI(" %p", port);
	}

	OSCL_LOGI(" %p", port);
	return OMX_ErrorNone;
}


OMX_ERRORTYPE clock_port_init(lb_omx_component_t *component,
	base_port_t *base_port,
	u32 index,
	u32 dir_type)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	clock_port_private_t *private = NULL;

	ret = base_port_init(component, base_port, index, dir_type);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("base port init error!\n");
		return ret;
	}

	private = oscl_zalloc(sizeof(*private));
	if (!private) {
		OSCL_LOGE("alloc clock_port_private_t error!\n");
		return OMX_ErrorInsufficientResources;
	}
	base_port->port_private = private;

	private->timestamp.nSize = sizeof(OMX_TIME_CONFIG_TIMESTAMPTYPE);
	private->timestamp.nPortIndex = index;
	private->timestamp.nTimestamp = 0x00;

	/* set the default port params */
	private->other_param.nSize = sizeof(OMX_OTHER_PARAM_PORTFORMATTYPE);
	private->other_param.nPortIndex = index;
	private->other_param.nIndex = 0;
	private->other_param.eFormat = OMX_OTHER_FormatTime;

	base_port->port_param.eDomain = OMX_PortDomainOther;
	base_port->port_param.format.other.eFormat = OMX_OTHER_FormatTime;
	base_port->port_param.nBufferSize = sizeof(OMX_TIME_MEDIATIMETYPE);
	base_port->port_param.nBufferCountActual = 1;
	base_port->port_param.nBufferCountMin = 1;

	private->mediatime.nSize = sizeof(OMX_TIME_MEDIATIMETYPE);
	private->mediatime.nClientPrivate = 0;
	private->mediatime.nOffset = 0x0;
	private->mediatime.xScale = 1;

	private->mediatime_request.nSize = sizeof(OMX_TIME_CONFIG_MEDIATIMEREQUESTTYPE);
	private->mediatime_request.nPortIndex = index;
	private->mediatime_request.pClientPrivate = NULL;
	private->mediatime_request.nOffset = 0x0;

	base_port->deinit = clock_port_deinit;
	base_port->recive_buffer = clock_port_recive_buffer;
	base_port->flush_buffer = clock_port_flush_buffer;
	return ret;
}

OMX_ERRORTYPE clock_port_deinit(base_port_t *port)
{
	oscl_param_check((port != NULL), OMX_ErrorBadParameter, NULL);
	oscl_free(port->port_private);
	OSCL_LOGI("=====clock_port_deinit=====");
	return base_port_deinit(port);
}

OMX_ERRORTYPE clock_port_recive_buffer(base_port_t *port,
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
	oscl_param_check(((OMX_StateExecuting == get_component_state(port))
			|| (OMX_StatePause == get_component_state(port))
			|| (OMX_StateIdle == get_component_state(port))
			|| (!PORT_IS_ENABLED(port))
			|| (!(port->is_flushed))),
		OMX_ErrorIncorrectStateOperation, "IncorrectState");
	if (!PORT_IS_SUPPLIER(port) && port->is_flushed) {
		OSCL_LOGW("recive buffers while flush port(none supplier)\n");
		OSCL_LOGW("%p(%s, %d)", port,
			get_port_name(port), port->port_param.nPortIndex);
	}

	/* If port is not tunneled then simply return the buffer except paused state */
	if (!PORT_IS_TUNNELED(port) && get_component_state(port) != OMX_StatePause) {
		OSCL_LOGI("not tunneled and not pause state, just return buffer\n");
		port->return_buffer(port, buffer);
		return OMX_ErrorNone;
	}

	/* notify the buffer management thread we have a fresh new buffer to manage */
	OSCL_LOGD("port index %d, name %s\n",
		port->port_param.nPortIndex, get_port_name(port));
	oscl_queue_queue(&port->buffer_queue, buffer);
	if (PORT_IS_TUNNELED(port)) {
		sem_post(&port->buffer_sem);
		OSCL_LOGD("post to notify new buffer!\n");
	}
	sem_post(component->buf_mgnt_sem);
	OSCL_TRACE("%p:%p", port, buffer);
	return OMX_ErrorNone;
}

