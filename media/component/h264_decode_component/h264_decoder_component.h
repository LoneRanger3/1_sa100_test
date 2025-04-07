/*
 * h264_decoder_component.h - h264 decoder component.
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

#ifndef __H264_DECODER_COMPONENT_H__
#define __H264_DECODER_COMPONENT_H__

#include "h264_decoder_api.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct h264_dec_ctx {
	void *decoder; /* h264 decoder handle */
	int out_buf_cnt; /* output buffer count */
	int out_planes; /* output buffer planes */
	int out_buf_offset[3]; /* buffer offset */
	OMX_U8 *tmp_in_buf;
	OMX_U32 tmp_buf_len;
	enum OUTPUT_MODE output_fmt; /* output format */
	frame_info_t info; /* frame info from header decoded */
	OMX_BOOL is_header_decoded; /* if the header is decoded */
	OMX_BOOL is_err_state;
	OMX_U32 err_frame_cnt;
} h264_dec_ctx_t;

extern OMX_ERRORTYPE h264dec_component_deinit(OMX_IN OMX_HANDLETYPE hComponent);
extern OMX_ERRORTYPE h264dec_component_init(lb_omx_component_t *cmp_handle);

extern OMX_ERRORTYPE h264dec_outport_init(lb_omx_component_t *component,
	base_port_t *base_port,
	u32 index,
	u32 dir_type);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __H264_DECODER_COMPONENT_H__ */
