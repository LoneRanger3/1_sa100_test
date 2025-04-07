/*
 * aenc_common.h - Standard functionality for the audio encodec component.
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

#ifndef __AENC_COMMON_H__
#define __AENC_COMMON_H__

#include "OMX_Core.h"
#include "OMX_Component.h"
#include "oscl.h"
#include "pcm_stream.h"
#include "decode_com.h"

typedef struct aenc_private {
	int64_t nFrames;
	int acode;
	pcm_buf_manager *pcm_manager;
	audio_dec_t paudio;
	unsigned char pcm_buf[16 * 1024];
} aenc_private_t;

#ifndef __KERNEL__
void rm(const char *filename);
#endif

aenc_private_t *aenc_init(void);
int aenc_deinit(aenc_private_t *audio_private);
int aenc_frame(aenc_private_t *audio_private,
	OMX_BUFFERHEADERTYPE *inbuf, OMX_BUFFERHEADERTYPE *outbuf);
int aenc_open(aenc_private_t *audio_private);
int aenc_close(aenc_private_t *audio_private);

#endif /* __AENC_COMMON_H__ */
