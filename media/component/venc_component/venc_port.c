#define LOG_TAG	"venc_port"

#include "venc_component.h"
#include "venc_port.h"
#include "lombo_encplugin.h"

#define STREAM_BUF_NUM 4
#define STREAM_BUF_SIZE (400*1024)
#define STREAM_BUF_ALIGN (64)

OMX_ERRORTYPE encoder_outport_allocate_buf(base_port_t *port,
	OMX_BUFFERHEADERTYPE **buf_header,
	OMX_PTR app_private,
	OMX_U32 size)
{
	int ret = OMX_ErrorNone;
	OMX_BUFFERHEADERTYPE *tmp_header;

	ret = base_port_allocate_buf(port, buf_header, app_private, size);
	if (ret == OMX_ErrorNone) {
		tmp_header = *buf_header;
		if (tmp_header->pBuffer)
			memset(tmp_header->pBuffer, 0, tmp_header->nAllocLen);
	}

	return ret;
}

OMX_ERRORTYPE encoder_outport_flush_buffer(base_port_t *port)
{
	OMX_BUFFERHEADERTYPE *buf_head = NULL;

	OSCL_TRACE("%p(%s, %d)", port, get_port_name(port), port->port_param.nPortIndex);
	OSCL_LOGD("%s flush buffer:%p, port:%d\n", get_port_name(port), buf_head,
		port->port_param.nPortIndex);
	if (PORT_IS_SUPPLIER(port)) {
		while (oscl_queue_get_num(&port->buffer_queue) != port->num_assigned) {
			OSCL_LOGD("queue num:%d assigned:%d",
				oscl_queue_get_num(&port->buffer_queue),
				port->num_assigned);
			sem_wait(&port->buffer_sem);
		}
	} else {
		do {
			buf_head = oscl_queue_dequeue(&port->buffer_queue);
			OSCL_LOGI("(%s)flush buffer:%p\n", get_port_name(port), buf_head);
			if (buf_head != NULL) {
				port->return_buffer(port, buf_head);
				buf_head->nFilledLen = 0;
				buf_head->nTimeStamp = 0;
			}
		} while (buf_head != NULL);
		OSCL_LOGI(" %p", port);
	}

	return OMX_ErrorNone;
}

OMX_ERRORTYPE encoder_inport_recive_buffer(base_port_t *port,
	OMX_BUFFERHEADERTYPE *buffer)
{
	lb_omx_component_t *component;
	venc_component_private_t *vec_private;

	OSCL_TRACE("%p(%s, %d)", port, get_port_name(port), port->port_param.nPortIndex);
	component = (lb_omx_component_t *)port->component;
	vec_private = (venc_component_private_t *)component->component_private;

	if (buffer->nTimeStamp < vec_private->ref_time) {
		OSCL_LOGI("port:%d drop frame, time:%lld:%lld", buffer->nInputPortIndex,
			  buffer->nTimeStamp, vec_private->ref_time);
		return -1;
	}

	return base_port_recive_buffer(port, buffer);
}


OMX_ERRORTYPE video_encode_port_init(lb_omx_component_t *component,
	base_port_t *base_port,
	OMX_U32 index,
	OMX_U32 dir_type)
{
	OMX_PARAM_PORTDEFINITIONTYPE *pparam;
	OSCL_TRACE(" %p %p %d %d\n", component, base_port, index, dir_type);

	oscl_param_check((component != NULL) && (base_port != NULL),
		OMX_ErrorBadParameter, NULL);

	base_port_init(component, base_port, index, dir_type);

	pparam = &base_port->port_param;
	pparam->eDomain = OMX_PortDomainVideo;
	if (dir_type == OMX_DirOutput) {
		pparam->nBufferCountActual = STREAM_BUF_NUM;
		pparam->nBufferSize = sizeof(venc_packet_t);
		pparam->format.video.eCompressionFormat = OMX_VIDEO_CodingAVC;
		pparam->format.video.xFramerate = 30000;
		base_port->flush_buffer = encoder_outport_flush_buffer;
		base_port->allocate_buffer = encoder_outport_allocate_buf;
	} else
		base_port->recive_buffer = encoder_inport_recive_buffer;
	return OMX_ErrorNone;
}

