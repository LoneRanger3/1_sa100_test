/*
 * rotate_component.c - code for rotate component.
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

#define DBG_LEVEL		DBG_WARNING
#define LOG_TAG			"rot_component"

#include <oscl.h>
#include <base_component.h>
#include "rotate_component.h"
#include "display/oscl_display.h"
#include <omx_vendor_lb.h>

typedef struct rotate_private {
	void			*disp_hdl;	/* display device handle */
	OMX_CONFIG_ROTATIONTYPE rot_mode; /* rot_mode.nRotation: vdisp_rotate_mode_e */
	rotate_buffer_t		input;
	rotate_buffer_t		output;
	OMX_U32			inbuf_size[3];
	OMX_U32			outbuf_size[3];
} rotate_private_t;

#define DEFAULT_VIDEO_WIDTH		(1280)
#define DEFAULT_VIDEO_HEIGHT		(720)
#define DEFAULT_INBUFFER_NUM		(4)
#define DEFAULT_OUTBUFFER_NUM		(3)
#define DEFAULT_BUFFER_ALIGN_SIZE	(1024)

static OMX_ERRORTYPE get_rotate_rect(vdisp_rotate_mode_e mode,
				     rotate_buffer_t *src,
				     rotate_buffer_t *dst)
{
	OMX_U32 offset_tail_x, offset_tail_y;
	disp_rect_t src_size;

	if (0 != (src->src_size.width % ROT_STRIDE_ALIGN)) {
		OSCL_LOGE("Input video width not support. %d", src->src_size.width);
		return OMX_ErrorBadParameter;
	}

	src_size.left = 0;
	src_size.top = 0;
	src_size.width = src->src_size.width;
	src_size.height = src->src_size.height & (~(ROT_STRIDE_ALIGN - 1));
	OSCL_LOGI("mode:%d src[%d %d %d %d] src_size[%d %d %d %d] disp_size[%d %d %d %d]",
		   mode,
		   src_size.left,
		   src_size.top,
		   src_size.width,
		   src_size.height,
		   src->src_size.left,
		   src->src_size.top,
		   src->src_size.width,
		   src->src_size.height,
		   src->disp_size.left,
		   src->disp_size.top,
		   src->disp_size.width,
		   src->disp_size.height);

	offset_tail_x = src->src_size.width -
		(src->disp_size.width + src->disp_size.left);
	offset_tail_y = 0;
	if (src_size.height > (src->disp_size.height + src->disp_size.top)) {
		offset_tail_y = src_size.height -
			(src->disp_size.height + src->disp_size.top);
	} else if (src_size.height > src->disp_size.height) {
		offset_tail_y = 0;
		src->disp_size.height = src_size.height - src->disp_size.top;
	}

	if (VDISP_ROTATE_90 == mode ||
	    VDISP_FLIP_H_ROT_90 == mode ||
	    VDISP_FLIP_V_ROT_90 == mode) {
		dst->src_size.width = src_size.height;
		dst->src_size.height = src_size.width;
		dst->src_size.left = src_size.left;
		dst->src_size.top = src_size.top;

		dst->disp_size.width = src->disp_size.height;
		dst->disp_size.height = src->disp_size.width;
		dst->disp_size.left = offset_tail_y;
		dst->disp_size.top = src->disp_size.left;
	} else if (VDISP_ROTATE_270 == mode) {
		dst->src_size.width = src_size.height;
		dst->src_size.height = src_size.width;
		dst->src_size.left = src_size.left;
		dst->src_size.top = src_size.top;

		dst->disp_size.width = src_size.height -
				offset_tail_y - src->disp_size.top;
		dst->disp_size.height = src->disp_size.width;
		dst->disp_size.left = src->disp_size.top;
		dst->disp_size.top = offset_tail_x;
	} else if (VDISP_ROTATE_180 == mode) {
		dst->src_size.width = src_size.width;
		dst->src_size.height = src_size.height;
		dst->src_size.left = src_size.left;
		dst->src_size.top = src_size.top;

		dst->disp_size.width = src->disp_size.width;
		dst->disp_size.height = src_size.height -
				offset_tail_y - src->disp_size.top;
		dst->disp_size.left = offset_tail_x;
		dst->disp_size.top = src->disp_size.top;
	} else if (VDISP_ROTATE_NONE == mode) {
		dst->src_size.width = src_size.width;
		dst->src_size.height = src_size.height;
		dst->src_size.left = src_size.left;
		dst->src_size.top = src_size.top;

		dst->disp_size.width = src->disp_size.width;
		dst->disp_size.height = src->disp_size.height;
		dst->disp_size.left = src->disp_size.left;
		dst->disp_size.top = src->disp_size.top;
	} else {
		OSCL_LOGE("%d mode not support.", mode);
		return OMX_ErrorBadParameter;
	}

	src->src_size.height = src_size.height;
	src->src_size.width = src_size.width;
	OSCL_LOGI("rotate(%d)>> dst[%d %d %d %d] disp[%d %d %d %d]",
		   mode,
		   dst->src_size.left, dst->src_size.top,
		   dst->src_size.width, dst->src_size.height,
		   dst->disp_size.left, dst->disp_size.top,
		   dst->disp_size.width, dst->disp_size.height);

	return OMX_ErrorNone;
}

#if 0
static void save_video_frame(char *prefix, rotate_buffer_t *buffer, OMX_S32 align)
{
	OMX_S32 fd = 0;
	OMX_U32 size[3] = {0};
	char path[64] = {0};
	static OMX_S32 frm_count;

	if (NULL == buffer) {
		OSCL_LOGE("Input para error.");
		return;
	}
	rt_snprintf(path, sizeof(path), "/%s_%d.yuv", prefix, frm_count);
	fd = open(path, O_WRONLY | O_CREAT);
	if (fd < 0) {
		LOG_E("fail to open file: %s", path);
		return;
	}

	calc_frame_size(buffer->eColorFormat,
		buffer->src_size.width, buffer->src_size.height,
		size, align);
	OSCL_LOGD("Save>> w:%d h:%d(%d %d)",
		buffer->src_size.width, buffer->src_size.height,
		size[0], size[1]);

	if (OMX_COLOR_FormatYUV420SemiPlanar == buffer->eColorFormat) {
		write(fd, (void *)buffer->addr[0], size[0]);
		write(fd, (void *)buffer->addr[1], size[1]);
	}
	close(fd);

	frm_count++;
}
#endif

OMX_ERRORTYPE rotate_set_parameter(OMX_IN OMX_HANDLETYPE hComp,
				   OMX_IN OMX_INDEXTYPE paramIndex,
				   OMX_IN OMX_PTR paramData)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component = NULL;
	rotate_private_t *rot_private = NULL;
	OMX_PARAM_PORTDEFINITIONTYPE *port_def = NULL;
	OMX_VIDEO_PORTDEFINITIONTYPE *video = NULL;
	base_port_t *inport = NULL;
	base_port_t *outport = NULL;

	OSCL_TRACE(" %p, %p , %d\n", hComp, paramData, paramIndex);

	oscl_param_check((hComp != NULL) && (paramData != NULL),
				OMX_ErrorBadParameter, NULL);
	component = get_lb_component(hComp);

	oscl_param_check((component != NULL), OMX_ErrorBadParameter, NULL);
	rot_private = component->component_private;

	switch (paramIndex) {
	case OMX_IndexParamPortDefinition: {
		port_def = (OMX_PARAM_PORTDEFINITIONTYPE *)paramData;

		ret = base_set_parameter(hComp, paramIndex, paramData);
		oscl_param_check((OMX_ErrorNone == ret), ret, NULL);

		if (ROT_INPUT_PORT_INDEX == port_def->nPortIndex) {
			inport = &component->port[ROT_INPUT_PORT_INDEX];
			outport = &component->port[ROT_OUTPUT_PORT_INDEX];
			video = &inport->port_param.format.video;
			ret = calc_frame_size(video->eColorFormat,
					      video->nStride,
					      video->nSliceHeight,
					      rot_private->inbuf_size,
					      inport->port_param.nBufferAlignment);
			oscl_param_check((OMX_ErrorNone == ret), ret, NULL);

			rot_private->input.eColorFormat = video->eColorFormat;

			rot_private->input.src_size.top = 0;
			rot_private->input.src_size.left = 0;
			rot_private->input.src_size.width = video->nStride;
			rot_private->input.src_size.height = video->nSliceHeight;

			/**
			 * default crop rectangle size ie equal
			 * to the original display size
			 */
			rot_private->input.disp_size.top = 0;
			rot_private->input.disp_size.left = 0;
			rot_private->input.disp_size.width = video->nFrameWidth;
			rot_private->input.disp_size.height = video->nFrameHeight;

			inport->port_param.nBufferSize = rot_private->inbuf_size[0] +
							 rot_private->inbuf_size[1] +
							 rot_private->inbuf_size[2];
			inport->port_param.format.video.xFramerate = video->xFramerate;
			OSCL_LOGI("inbuf_size[%d %d %d] src[%d %d %d %d] format:%x",
				rot_private->inbuf_size[0],
				rot_private->inbuf_size[1],
				rot_private->inbuf_size[2],
				video->nFrameWidth,
				video->nFrameHeight,
				video->nStride,
				video->nSliceHeight,
				rot_private->input.eColorFormat);
		}
		break;
	}

	/* this command is set after OMX_IndexParamPortDefinition */
	case OMX_IndexConfigCommonInputCrop: {
#if 0 /* current HW rotate interface not support crop */
		OMX_CONFIG_RECTTYPE *rect = (OMX_CONFIG_RECTTYPE *)paramData;

		rot_private->input.disp_size.top = rect->nTop;
		rot_private->input.disp_size.left = rect->nLeft;
		rot_private->input.disp_size.width = rect->nWidth;
		rot_private->input.disp_size.height = rect->nHeight;
#endif
		break;
	}

	/**
	 * this command is set after
	 * OMX_IndexParamPortDefinition or OMX_IndexConfigCommonInputCrop
	 */
	case OMX_IndexConfigCommonRotate: {
		oscl_param_check((NULL != paramData), OMX_ErrorBadParameter, NULL);
		OMX_CONFIG_ROTATIONTYPE *rot_mode = (OMX_CONFIG_ROTATIONTYPE *)paramData;

		rot_private->rot_mode = *rot_mode;
		ret = get_rotate_rect(rot_private->rot_mode.nRotation,
				      &rot_private->input,
				      &rot_private->output);

		outport = &component->port[ROT_OUTPUT_PORT_INDEX];
		video = &outport->port_param.format.video;

		video->nStride = rot_private->output.src_size.width;
		video->nSliceHeight = rot_private->output.src_size.height;
		video->nFrameWidth = video->nStride;
		video->nFrameHeight = video->nSliceHeight;
		if (rot_private->output.disp_size.width < video->nStride)
			video->nFrameWidth = rot_private->output.disp_size.width;
		if (rot_private->output.disp_size.height < video->nSliceHeight)
			video->nFrameHeight = rot_private->output.disp_size.height;

		ret = calc_frame_size(video->eColorFormat,
				      video->nStride,
				      video->nSliceHeight,
				      rot_private->outbuf_size,
				      outport->port_param.nBufferAlignment);
		rot_private->output.eColorFormat = video->eColorFormat;
		outport->port_param.nBufferSize = rot_private->outbuf_size[0] +
						rot_private->outbuf_size[1] +
						rot_private->outbuf_size[2];
		outport->port_param.nBufferSize += sizeof(omx_extra_data_t);

		OSCL_LOGI("outbuf_size[%d %d %d] src[%d %d %d %d] format:%x",
			rot_private->outbuf_size[0],
			rot_private->outbuf_size[1],
			rot_private->outbuf_size[2],
			video->nFrameWidth,
			video->nFrameHeight,
			video->nStride,
			video->nSliceHeight,
			video->eColorFormat);

		break;
	}
	default:
		OSCL_LOGI("default base set params index %d\n", paramIndex);
		ret = base_set_parameter(hComp, paramIndex, paramData);
		break;
	}

	OSCL_TRACE(" %d\n", ret);

	return ret;
}

OMX_ERRORTYPE rotate_get_parameter(OMX_IN OMX_HANDLETYPE hComp,
				   OMX_IN OMX_INDEXTYPE paramIndex,
				   OMX_IN OMX_PTR paramData)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component = NULL;
	rotate_private_t *rot_private = NULL;

	component = get_lb_component(hComp);
	oscl_param_check((component != NULL), OMX_ErrorBadParameter, NULL);
	rot_private = component->component_private;

	switch (paramIndex) {
	case OMX_IndexConfigCommonOutputCrop: {
		OMX_CONFIG_RECTTYPE *rect = (OMX_CONFIG_RECTTYPE *)paramData;

		rect->nLeft = rot_private->output.disp_size.left;
		rect->nTop = rot_private->output.disp_size.top;
		rect->nWidth = rot_private->output.disp_size.width;
		rect->nHeight = rot_private->output.disp_size.height;
		break;
	}
	case omx_index_lombo_video_size: {
		omx_size_t *size = (omx_size_t *)paramData;
		base_port_t *outport = &component->port[ROT_OUTPUT_PORT_INDEX];
		OMX_VIDEO_PORTDEFINITIONTYPE *video = &outport->port_param.format.video;
		size->width = video->nFrameWidth;
		size->height = video->nFrameHeight;
		size->stride = video->nStride;
		size->slice_height = video->nSliceHeight;
		break;
	}
	default:
		ret = base_get_parameter(hComp, paramIndex, paramData);
		break;
	}

	return ret;
}

OMX_ERRORTYPE rotate_set_state(OMX_HANDLETYPE hComp,
			OMX_U32 dest_state)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component = NULL;
	rotate_private_t *rot_private = NULL;
	OMX_STATETYPE pre_state;

	OSCL_TRACE(" %p, %x\n", hComp, dest_state);
	oscl_param_check(hComp != NULL, OMX_ErrorBadParameter, NULL);
	component = get_lb_component(hComp);
	rot_private = component->component_private;

	if (dest_state == OMX_StateExecuting && (component->state == OMX_StateIdle ||
		component->state == OMX_StatePause)) {
		if (rot_private->disp_hdl != NULL) {
			oscl_close_rot_submodule(rot_private->disp_hdl);
			oscl_close_disp_engine(rot_private->disp_hdl);
		}
		rot_private->disp_hdl = oscl_open_disp_engine();
		if (rot_private->disp_hdl == NULL) {
			OSCL_LOGE("open disp engine error!");
			return OMX_ErrorInsufficientResources;
		}
		ret = oscl_open_rot_submodule(rot_private->disp_hdl);
		if (ret != OMX_ErrorNone)
			OSCL_LOGE("open rotate submodule error!");
	}
	pre_state = component->state;
	ret = base_component_set_state(hComp, dest_state);
	if (dest_state == OMX_StateIdle && (pre_state == OMX_StateExecuting ||
		pre_state == OMX_StatePause)) {
		if (rot_private->disp_hdl != NULL) {
			oscl_close_rot_submodule(rot_private->disp_hdl);
			oscl_close_disp_engine(rot_private->disp_hdl);
			rot_private->disp_hdl = NULL;
		}
	}
	return ret;
}

void rotate_buf_handle(OMX_HANDLETYPE hComp,
		       OMX_BUFFERHEADERTYPE *inbuf,
		       OMX_BUFFERHEADERTYPE *outbuf)
{
	lb_omx_component_t *component = NULL;
	rotate_private_t *rot_private = NULL;
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	OMX_S32 video_size = 0;

	component = get_lb_component(hComp);
	rot_private = component->component_private;

	/* rot buffer transfer */
	rot_private->input.size[0] = rot_private->inbuf_size[0];
	rot_private->input.size[1] = rot_private->inbuf_size[1];
	rot_private->input.size[2] = rot_private->inbuf_size[2];
	rot_private->output.size[0] = rot_private->outbuf_size[0];
	rot_private->output.size[1] = rot_private->outbuf_size[1];
	rot_private->output.size[2] = rot_private->outbuf_size[2];

	OSCL_LOGD("input offset:%d %d %d, output offset:%d %d %d",
			rot_private->input.size[0],
			rot_private->input.size[1],
			rot_private->input.size[2],
			rot_private->output.size[0],
			rot_private->output.size[1],
			rot_private->output.size[2]);
	rot_private->input.addr[0] = inbuf->pBuffer;
	rot_private->input.addr[1] = inbuf->pBuffer +
				rot_private->inbuf_size[0];
	rot_private->input.addr[2] = inbuf->pBuffer +
				rot_private->inbuf_size[0] +
				rot_private->inbuf_size[1];

	rot_private->output.addr[0] = outbuf->pBuffer;
	rot_private->output.addr[1] = outbuf->pBuffer +
				rot_private->outbuf_size[0];
	rot_private->output.addr[2] = outbuf->pBuffer +
				rot_private->outbuf_size[0] +
				rot_private->outbuf_size[1];

#if 0
{

	rotate_buffer_t tmp_input;
	OMX_VIDEO_PORTDEFINITIONTYPE *video = NULL;
	base_port_t *inport = NULL;

	inport = &component->port[ROT_INPUT_PORT_INDEX];
	video = &inport->port_param.format.video;
	tmp_input.addr[0] = rot_private->input.addr[0];
	tmp_input.addr[1] = rot_private->input.addr[1];
	tmp_input.addr[2] = rot_private->input.addr[2];
	tmp_input.eColorFormat = rot_private->input.eColorFormat;
	tmp_input.src_size.left = 0;
	tmp_input.src_size.top = 0;
	tmp_input.src_size.width = video->nStride;
	tmp_input.src_size.height = video->nSliceHeight;

	save_video_frame("before", &tmp_input, 1);
}
#endif
	OSCL_LOGD("Rot>> inbuf:%p %p, outbuf:%p %p",
						     inbuf,
						     inbuf->pBuffer,
						     outbuf,
						     outbuf->pBuffer);

	ret = oscl_disp_hw_rotate(rot_private->disp_hdl,
				  rot_private->rot_mode.nRotation,
				  &rot_private->input,
				  &rot_private->output);
	if (OMX_ErrorNone == ret) {
		outbuf->nFilledLen = rot_private->outbuf_size[0] +
				     rot_private->outbuf_size[1] +
				     rot_private->outbuf_size[2];
	} else {
		outbuf->nFilledLen = 0;
	}
	video_size =  rot_private->inbuf_size[0] +
			 rot_private->inbuf_size[1] +
			 rot_private->inbuf_size[2];
	if ((video_size + sizeof(omx_extra_data_t)) <= inbuf->nAllocLen) {
		memcpy(outbuf->pBuffer + outbuf->nAllocLen - sizeof(omx_extra_data_t),
			inbuf->pBuffer + inbuf->nAllocLen - sizeof(omx_extra_data_t),
			sizeof(omx_extra_data_t));
	} else {
		OSCL_LOGD("No extra data, video_size:%d, inbuf->nAllocLen:%d, struct:%d",
			video_size, inbuf->nAllocLen, (int)sizeof(omx_extra_data_t));
	}

	outbuf->nTimeStamp = inbuf->nTimeStamp;
	inbuf->nFilledLen = 0;
	/* save_video_frame("after", &rot_private->output, 1); */
}

OMX_ERRORTYPE rotate_component_deinit(OMX_IN OMX_HANDLETYPE hComp)
{
	lb_omx_component_t *component;
	rotate_private_t *rot_private = NULL;

	oscl_param_check(hComp != NULL, OMX_ErrorBadParameter, NULL);

	OSCL_TRACE("base_cmp_handle:%p\n", hComp);
	oscl_param_check(hComp != NULL, OMX_ErrorBadParameter, NULL);
	component = get_lb_component(hComp);
	rot_private = component->component_private;

	base_port_deinit(&component->port[ROT_INPUT_PORT_INDEX]);
	base_port_deinit(&component->port[ROT_OUTPUT_PORT_INDEX]);

	oscl_free(rot_private);

	base_component_deinit(hComp);

	return OMX_ErrorNone;
}

OMX_ERRORTYPE rotate_component_init(lb_omx_component_t *cmp_handle)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	OMX_COMPONENTTYPE *base_cmp = &(cmp_handle->base_comp);
	rotate_private_t *rot_private = NULL;
	OMX_PARAM_PORTDEFINITIONTYPE *port_param = NULL;
	OMX_U32 plane_size[3] = {0};

	rot_private = oscl_zalloc(sizeof(rotate_private_t));
	oscl_param_check_exit((rot_private != NULL),
		OMX_ErrorInsufficientResources, NULL);
	rot_private->rot_mode.nRotation = -1;

	ret = base_component_init(cmp_handle);
	if (ret != OMX_ErrorNone) {
		oscl_free(rot_private);
		return ret;
	}
	cmp_handle->name = "OMX.LB.VIDEO.ROT";
	cmp_handle->component_private = rot_private;
	cmp_handle->base_comp.ComponentDeInit = rotate_component_deinit;
	cmp_handle->buf_handle = rotate_buf_handle;
	cmp_handle->do_state_set = rotate_set_state;
	cmp_handle->num_ports = 2;

	/* input port */
	ret = base_port_init(cmp_handle,
			     &cmp_handle->port[ROT_INPUT_PORT_INDEX],
			     ROT_INPUT_PORT_INDEX,
			     OMX_DirInput);
	if (OMX_ErrorNone != ret)
		goto EXIT;

	port_param = &cmp_handle->port[ROT_INPUT_PORT_INDEX].port_param;
	port_param->eDomain = OMX_PortDomainVideo;
	port_param->nBufferAlignment = DEFAULT_BUFFER_ALIGN_SIZE;
	port_param->nBufferCountActual = DEFAULT_INBUFFER_NUM;
	port_param->bBuffersContiguous = 1;

	port_param->format.video.eColorFormat = OMX_COLOR_FormatYUV420SemiPlanar;
	port_param->format.video.nFrameWidth = DEFAULT_VIDEO_WIDTH;
	port_param->format.video.nFrameHeight = DEFAULT_VIDEO_HEIGHT;
	ret = calc_frame_size(port_param->format.video.eColorFormat,
			      port_param->format.video.nFrameWidth,
			      port_param->format.video.nFrameHeight,
			      plane_size,
			      port_param->nBufferAlignment);
	port_param->nBufferSize = plane_size[0] + plane_size[1] + plane_size[2];

	/* output port */
	ret = base_port_init(cmp_handle, &cmp_handle->port[ROT_OUTPUT_PORT_INDEX],
			ROT_OUTPUT_PORT_INDEX,
			OMX_DirOutput);
	if (ret != OMX_ErrorNone)
		goto EXIT1;
	port_param = &cmp_handle->port[ROT_OUTPUT_PORT_INDEX].port_param;
	port_param->eDomain = OMX_PortDomainVideo;
	port_param->nBufferAlignment = DEFAULT_BUFFER_ALIGN_SIZE;
	port_param->nBufferCountActual = DEFAULT_OUTBUFFER_NUM;
	port_param->bBuffersContiguous = 1;

	port_param->format.video.eColorFormat = OMX_COLOR_FormatYUV420SemiPlanar;
	port_param->format.video.nFrameWidth = DEFAULT_VIDEO_HEIGHT;
	port_param->format.video.nFrameHeight = DEFAULT_VIDEO_WIDTH;

	ret = calc_frame_size(port_param->format.video.eColorFormat,
			      port_param->format.video.nFrameWidth,
			      port_param->format.video.nFrameHeight,
			      plane_size,
			      port_param->nBufferAlignment);
	port_param->nBufferSize = plane_size[0] + plane_size[1] + plane_size[2];

	base_cmp->SetParameter = rotate_set_parameter;
	base_cmp->GetParameter = rotate_get_parameter;


	OSCL_TRACE("rotate_component_init:%d\n", ret);

	return ret;

EXIT1:
	base_port_deinit(&cmp_handle->port[ROT_INPUT_PORT_INDEX]);
EXIT:
	base_component_deinit(cmp_handle);
	oscl_free(rot_private);

	return OMX_ErrorInsufficientResources;
}

