/*
 * oscl_display.h - display api used by lombo media framework.
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

#ifndef __OSCL_DISPLAY_H__
#define __OSCL_DISPLAY_H__

#ifdef __EOS__
#include "oscl_eos_disp.h"
#elif defined __KERNEL__
#endif

void *oscl_open_disp_engine();
void  oscl_close_disp_engine(void *hdl);
OMX_S32 oscl_open_rot_submodule(void *hdl);
void oscl_close_rot_submodule(void *hdl);
OMX_ERRORTYPE oscl_disp_enable_layer(void *hdl, void **layer);
OMX_ERRORTYPE oscl_disp_disable_layer(void *hdl, void **layer);
OMX_ERRORTYPE oscl_disp_release_layer(void *hdl, void **layer);
void *oscl_disp_request_layer(void *hdl);
OMX_ERRORTYPE oscl_disp_set_layer_top(void *hdl, void *layer);
OMX_ERRORTYPE oscl_disp_set_layer_bottom(void *hdl, void *layer);
OMX_ERRORTYPE oscl_disp_update_layer_data(void *hdl, void *layer,
							layer_data_t *data);
OMX_ERRORTYPE oscl_disp_get_screen_size(void *hdl, disp_size_t *size);
OMX_ERRORTYPE oscl_disp_hw_rotate(void *hdl, vdisp_rotate_mode_e mode,
			rotate_buffer_t *src, rotate_buffer_t *dst);
#endif

