/*
 * lb_omx_core.h - Standard functionality for lombo omx core.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef LB_OMX_CORE_H
#define LB_OMX_CORE_H
#include "OMX_Core.h"
#include "base_component.h"

#define OMX_COMP_MAX_INST 25
#define OMX_MAX_ROLES 20
#define MAX_TABLE_SIZE 50

typedef struct _omx_component_tbl {
	const char *name;
	char	reg_flag;
	OMX_ERRORTYPE(*fn_init)(lb_omx_component_t *);
	void	*so_lib_handle;
	char	*so_lib_name;
	const char *roles[OMX_MAX_ROLES];
} omx_component_tbl_t;

void omx_component_register(omx_component_tbl_t *omx_component_tbl);
OMX_HANDLETYPE *omx_active_handle_get(void);
void omx_active_handle_put(void);

/*component initial func*/
extern OMX_ERRORTYPE vrec_component_init(lb_omx_component_t *cmp_handle);
extern OMX_ERRORTYPE vrender_component_init(lb_omx_component_t *cmp_handle);
extern OMX_ERRORTYPE video_encoder_component_init(lb_omx_component_t *cmp_handle);
extern OMX_ERRORTYPE splitter_component_init(lb_omx_component_t *lb_cmp_hdl);
extern OMX_ERRORTYPE file_writer_component_init(lb_omx_component_t *lb_cmp_hdl);
extern OMX_ERRORTYPE muxer_component_init(lb_omx_component_t *cmp_handle);
extern OMX_ERRORTYPE arec_component_init(lb_omx_component_t *cmp_handle);
extern OMX_ERRORTYPE aenc_component_init(lb_omx_component_t *cmp_handle);
extern OMX_ERRORTYPE rotate_component_init(lb_omx_component_t *cmp_handle);
extern OMX_ERRORTYPE demuxer_component_init(lb_omx_component_t *cmp_handle);
extern OMX_ERRORTYPE adec_component_init(lb_omx_component_t *cmp_handle);
extern OMX_ERRORTYPE arender_component_init(lb_omx_component_t *cmp_handle);
extern OMX_ERRORTYPE h264dec_component_init(lb_omx_component_t *cmp_handle);
extern OMX_ERRORTYPE clock_component_init(lb_omx_component_t *cmp_handle);
extern OMX_ERRORTYPE pano_component_init(lb_omx_component_t *cmp_handle);
#endif

