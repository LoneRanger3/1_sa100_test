/*
 * lb_omx_as_string.h - Standard functionality for lombo omx core.
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

#ifndef __OMX_VENDOR_LB_H__
#define __OMX_VENDOR_LB_H__

#include "OMX_Core.h"

typedef struct omx_stride_info {
	OMX_S32	width; /* linestride */
	OMX_S32	height; /* aligned height */
	void	*priv;
} omx_stride_info_t;

typedef struct omx_size {
	int width;
	int height;
	int stride;
	int slice_height;
} omx_size_t;

typedef struct omx_audio_info {
	OMX_U32 bitrate;
	OMX_U32 samplerate;
	OMX_U32 channels;
	OMX_U32 bitspersample;
	OMX_U8 *extradata;
	OMX_U32 extrasize;
	int codec_id;
} omx_audio_info_t;

typedef struct omx_video_info {
	int width;
	int height;
	OMX_U8 *extradata;
	OMX_U32 extrasize;
	int codec_id;
	int has_video_extra;
} omx_video_info_t;

typedef struct omx_sub_info {
	OMX_U8 *extradata;
	OMX_U32 extrasize;
	int codec_id;
} omx_sub_info_t;

/**
* struct omx_isp_info - structure for the EXIF information
* @exposure_time: exposure time register value
* @shutter_speed: speed of the shutter register value
* @aperture: aperture register value
* @exposure_bias: it calls also EV bias
* @iso_speed: ISO register value
* @flash: status register value of the flash
* @back_score: back light score
* @res: reserved info
* @res[0]: object luminance
* @res[1]: back luminance
* @res[2]: average luminance
* @res[3]: original luminance
* @res[4]: final luminance
*/
typedef struct omx_isp_info {
	OMX_S32	valid;
	OMX_U32 exposure_time;
	OMX_U32 shutter_speed;
	OMX_U32 aperture;
	OMX_U32 brightness;
	OMX_U32 exposure_bias;
	OMX_U32 iso_speed;
	OMX_U32 flash;
	OMX_U32 illu_id;
	OMX_U32 back_score;
	OMX_U32 res[16];
} omx_isp_info_t;

typedef struct omx_extra_data {
	omx_isp_info_t isp_inf;
	void *priv;
} omx_extra_data_t;

#endif /* __OMX_VENDOR_LB_H__ */

