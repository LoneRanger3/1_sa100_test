#include <oscl.h>
#include <base_component.h>
#include "h264_decoder_component.h"
#include "lombo_al_lib.h"

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
		port->buf_state = oscl_zalloc(buf_count * sizeof(u32));
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

OMX_ERRORTYPE h264dec_port_allocate_buf(base_port_t *port,
	OMX_BUFFERHEADERTYPE **buf_header,
	OMX_PTR private,
	OMX_U32 size)
{
	int ret = OMX_ErrorNone;
	OMX_U8 *pBuffer = NULL;
	unsigned long phy_addr = 0;
	h264_dec_ctx_t *ctx = NULL;
	lb_omx_component_t *component = NULL;
	fb_buffer_t *fb = NULL;
	int i;

	oscl_param_check((port != NULL), OMX_ErrorBadParameter, NULL);
	component = (lb_omx_component_t *)port->component;
	ctx = (h264_dec_ctx_t *)component->component_private;

	/* alloc the actual buffer */
	pBuffer = lombo_al_malloc(size, AL_MEM_WB, &phy_addr, __FILE__, __LINE__);
	if (!pBuffer) {
		OSCL_LOGE("alloc vout buffer error\n");
		return OMX_ErrorInsufficientResources;
	}
	ret = __add_buffer(port, buf_header, private, size, pBuffer, BUFFER_ALLOCATED);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("add buffer error\n");
		lombo_al_free(pBuffer, AL_MEM_WB);
		return ret;
	}

	fb = oscl_zalloc(sizeof(fb_buffer_t));
	if (!fb) {
		OSCL_LOGE("malloc out_fb error!\n");
		lombo_al_free(pBuffer, AL_MEM_WB);
		return OMX_ErrorInsufficientResources;
	}
	fb->plane_cnt = ctx->out_planes;
	for (i = 0; i < 3; i++) {
		fb->vir_addr[i] = pBuffer + ctx->out_buf_offset[i];
		fb->phy_addr[i] = phy_addr + ctx->out_buf_offset[i];
	}
	(*buf_header)->pOutputPortPrivate = fb;
	fb->priv = *buf_header;

	return ret;
}

OMX_ERRORTYPE h264dec_port_free_buffer(base_port_t *port,
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
		lombo_al_free(buf_header->pBuffer, AL_MEM_WB);
		buf_header->pBuffer = NULL;
	}
	/* free fb_buffer_t struct */
	if (buf_header->pOutputPortPrivate) {
		oscl_free(buf_header->pOutputPortPrivate);
		buf_header->pOutputPortPrivate = NULL;
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
		sem_post(&port->populate_sem);
	}
	port->port_param.bPopulated = OMX_FALSE;

	OSCL_TRACE("port:%p, buffer:%d", port, port->num_assigned);
	return OMX_ErrorNone;
}

OMX_ERRORTYPE h264dec_outport_init(lb_omx_component_t *component,
	base_port_t *base_port,
	u32 index,
	u32 dir_type)
{
	int ret = OMX_ErrorNone;

	ret = base_port_init(component, base_port, index, dir_type);

	base_port->allocate_buffer = h264dec_port_allocate_buf;
	base_port->free_buffer = h264dec_port_free_buffer;

	return ret;
}

