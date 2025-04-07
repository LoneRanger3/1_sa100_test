/*
 * oscl_vrec_i.h - eos video receive base struct used by lombo media framework.
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

#ifndef __OSCL_VREC_I_H__
#define __OSCL_VREC_I_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <omx_vendor_lb.h>

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(_A) (sizeof(_A) / sizeof((_A)[0]))
#endif /* ARRAY_SIZE */

typedef enum image_mode {
	IMAGE_MODE_1_NORMAL,
	IMAGE_MODE_2_STITCH_H,
	IMAGE_MODE_4_STITCH_CROSS,
} image_mode_e;

typedef struct vrec_format {
	OMX_S32 index;
	OMX_U32 frame_rate;
	OMX_COLOR_FORMATTYPE format;
	OMX_CONFIG_RECTTYPE frame_size;
} vrec_format_t;

typedef struct vrec_info {
	OMX_S32 channel_num;
	OMX_S32 fmt_num;
	image_mode_e image_mode;
	vrec_format_t *format;
} vrec_info_t;

typedef struct vrec_frame_addr {
	OMX_U32 align;
	long frame_index;
	OMX_U32 size[3];
	OMX_U8 *addr[3];
} vrec_frame_addr_t;

typedef struct vrec_frame_queue {
	OMX_U32 ch_width;	/* every channel width */
	OMX_U32 ch_height;	/* every channel height */
	OMX_U32 ch_size[3];
	image_mode_e image_mode;
	OMX_S32 channel_num;
	OMX_S32  frame_num;   /* all channel frames number*/
	vrec_frame_addr_t *frame_addr;
} vrec_frame_queue_t;

typedef struct vrec_reqframe {
	OMX_S32 channel_id;
	OMX_U32 frame_id;
	OMX_S64 pts;
	OMX_S32 pts_valid;

	OMX_COLOR_FORMATTYPE pix_format;
	OMX_CONFIG_RECTTYPE src_rect; /* source valid size */
	OMX_CONFIG_RECTTYPE dst_rect; /* source display size, use crop*/
	vrec_frame_addr_t paddr;
	omx_isp_info_t isp_exif;
	void *_private;
} vrec_reqframe_t;

typedef struct vrec_pixformat_map_index {
	OMX_S32 sensor_format;
	OMX_COLOR_FORMATTYPE omx_format;
	OMX_S32 buf_num;
} vrec_pixformat_map_index_t;

typedef struct vrec_image_mode_index {
	OMX_U32 buf_type;
	image_mode_e mode;
} vrec_image_mode_index_t;

typedef struct vrec_pts_cb {
	OMX_S32 (*vrec_get_pts)(OMX_S64 *pts, void *para);
	void *para;
} vrec_pts_cb_t;

#ifdef __cplusplus
}
#endif

#endif

