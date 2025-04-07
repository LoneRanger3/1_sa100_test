/*
 * oscl_display_i.h - display common struct used by lombo media framework.
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

#ifndef __OSCL_DISP_I_H__
#define __OSCL_DISP_I_H__

#include "vdisp.h"

#define ROT_STRIDE_ALIGN	(32)
#define ROT_ADDR_ALIGN		(32)

typedef struct disp_rotmode_map_index {
	OMX_S32 disp_rot_mode;
	vdisp_rotate_mode_e omx_rot_mode;
} disp_rotmode_map_index_t;

typedef struct disp_pixformat_map_index {
	OMX_S32 disp_format;
	OMX_COLOR_FORMATTYPE eColorFormat;
} disp_pixformat_map_index_t;

typedef struct disp_fb_map_index {
	void *virtual_addr;
	OMX_U32 fb_id;
} disp_fb_map_index_t;

typedef struct disp_rect {
	OMX_S32   left;
	OMX_S32   top;
	OMX_S32   width;
	OMX_S32   height;
} disp_rect_t;

typedef struct disp_size {
	OMX_S32   width;
	OMX_S32   height;
} disp_size_t;

typedef struct layer_data {
	disp_size_t src_size;
	disp_rect_t win_rect;  /* display window */
	disp_rect_t crop_rect; /* need to display source image */
	OMX_COLOR_FORMATTYPE eColorFormat;
	OMX_U32 size[3];
	OMX_U8 *addr[3];
} layer_data_t;

typedef struct rotate_buffer {
	OMX_S32 index;
	OMX_COLOR_FORMATTYPE eColorFormat;
	disp_rect_t src_size;	/* aligned original image */
	disp_rect_t disp_size;	/* effective display area */
	OMX_U32	size[3];
	OMX_U8 *addr[3];
} rotate_buffer_t;
#endif

