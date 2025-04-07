/*
 * vrender_component.h - video display component.
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

#ifndef __VRENDER_COMPONENT_H__
#define __VRENDER_COMPONENT_H__

#include "base_component.h"
#include "display/oscl_display.h"

#define SINK_INPUT_PORT_INDEX 0
#define SINK_CLOCK_PORT_INDEX 1

typedef struct vdisp_para {
	vdisp_mode_e mode;
	disp_rect_t win_rect;  /* Window postion and size in the screen */
	disp_rect_t crop_rect; /* Need display picture pos and size in the framebuffer */
	OMX_S32	y_cut_ratio;  /* Use to cut mode. up and down cut ratio */
	OMX_S32 x_cut_ratio;  /* Use to cut mode. left and right cut ratio  */
} vdisp_para_t;

OMX_ERRORTYPE vrender_component_deinit(OMX_IN OMX_HANDLETYPE hComponent);
OMX_ERRORTYPE vrender_component_init(lb_omx_component_t *cmp_handle);

#endif /* __VRENDER_COMPONENT_H__ */

