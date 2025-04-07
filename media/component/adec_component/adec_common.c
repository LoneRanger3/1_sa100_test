/*
 * adec_component.c - demo code for adec component.
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
#include <oscl.h>
#include <base_component.h>
#include "adec_common.h"
#include "adec_lib.h"
#include "avformat.h"

/* #define DECRAWDATA */
#ifdef DECRAWDATA
static int pcmfd;
#define PCMNAME "audio.pcm"
#endif

#if 0
int aacdec_init(struct tag_audio_dec *pdec)
{
	return 0;
}
int aacdec_frame(struct tag_audio_dec *pdec, packet_t *pin, packet_t *pout)
{
	return 0;
}
int aacdec_close(struct tag_audio_dec *pdec)
{
	return 0;
}
int aacdec_flush(struct tag_audio_dec *pdec)
{
	return 0;
}
#endif

int adec_frame(adec_private_t *audio_private,
	OMX_BUFFERHEADERTYPE *inbuf, OMX_BUFFERHEADERTYPE *outbuf)
{
	int ret = 0;
	packet_t packet_in;
	packet_t packet_out;

	if (inbuf == NULL || inbuf->pBuffer == NULL)
		return 0;
	if (outbuf == NULL || outbuf->pBuffer == NULL)
		return 0;
	outbuf->nFilledLen = 0;
	packet_out.data = outbuf->pBuffer + outbuf->nOffset;
	packet_in.data = inbuf->pBuffer;
	packet_in.size = inbuf->nFilledLen;

	ret =  audio_private->paudio.decode(&audio_private->paudio,
			&packet_in, &packet_out);
	/* OSCL_LOGE("dec size:%d,ret:%d,frame:%lld",
	packet_out.size, ret, audio_private->nFrames); */
	if (inbuf->nFlags & OMX_BUFFERFLAG_EOS)  {
		OSCL_LOGE("adec_buf_handle get the end of stream\n");
		outbuf->nFlags |= OMX_BUFFERFLAG_EOS;
	}
	outbuf->nOffset = 0;
	outbuf->nFilledLen = packet_out.size;
	outbuf->nTimeStamp = inbuf->nTimeStamp;
	audio_private->nFrames += audio_private->paudio.para->frame_number;

#ifdef DECRAWDATA
	if (pcmfd > 0) {
		int len = write(pcmfd, packet_out.data, packet_out.size);
		if (len != packet_out.size)
			OSCL_LOGE("write err:ret:%d,len:%d\n", ret, len);
	}
#endif

	return ret;
}

int adec_open(adec_private_t *audio_private)
{
	switch (audio_private->paudio.para->codec_id) {
	case AV_CODEC_TYPE_AAC:
		AAC_DecOpen(&(audio_private->paudio));
		break;
	case AV_CODEC_TYPE_PCM_ALAW:
	case AV_CODEC_TYPE_PCM_MULAW:
	case AV_CODEC_TYPE_PCM_F64LE:
	case AV_CODEC_TYPE_PCM_F32LE:
	case AV_CODEC_TYPE_PCM_S16LE:
	case AV_CODEC_TYPE_PCM_U8:
	case AV_CODEC_TYPE_ADPCM_IMA_WAV:
		WAV_DecOpen(&(audio_private->paudio));
		break;
	default:
		OSCL_LOGE("acode:%d,error!!!!", audio_private->paudio.para->codec_id);
		return -1;
	}

	OSCL_LOGI("acode:%d,!!!!", audio_private->paudio.para->codec_id);

	if (audio_private->paudio.para->extradata) {
		oscl_free(audio_private->paudio.para->extradata);
		audio_private->paudio.para->extradata = NULL;
		audio_private->paudio.para->extradata_size = 0;
	}

#ifdef DECRAWDATA
#ifdef __KERNEL__
	remove(PCMNAME);
#else
	rm(PCMNAME);
#endif
	pcmfd = open(PCMNAME, O_WRONLY | O_CREAT);
	OSCL_LOGW("====open pcm file:%d", pcmfd);
#endif

	return  audio_private->paudio.init(&(audio_private->paudio));
}

int adec_close(adec_private_t *audio_private)
{
#ifdef DECRAWDATA
	if (pcmfd > 0) {
		close(pcmfd);
		OSCL_LOGE("close pcm file :%d\n", pcmfd);
		pcmfd = 0;
	}
#endif
	return audio_private->paudio.close(&(audio_private->paudio));
}

int adec_flush(adec_private_t *audio_private)
{
	return audio_private->paudio.flush(&(audio_private->paudio));
}

int adec_deinit(adec_private_t *audio_private)
{
	if (audio_private) {
		if (audio_private->paudio.para) {
			if (audio_private->paudio.para->extradata) {
				oscl_free(audio_private->paudio.para->extradata);
				audio_private->paudio.para->extradata = NULL;
				audio_private->paudio.para->extradata_size = 0;
			}
			oscl_free(audio_private->paudio.para);
			audio_private->paudio.para = NULL;
		}

		oscl_free(audio_private);
	}

	return 0;
}

adec_private_t *adec_init(void)
{
	adec_private_t *audio_private = NULL;

	audio_private = oscl_zalloc(sizeof(adec_private_t));
	if (!audio_private) {
		OSCL_LOGE("malloc arec_ctx_t error!\n");
		return NULL;
	}
	audio_private->paudio.para = oscl_zalloc(sizeof(AV_CodecParameters));
	if (!audio_private->paudio.para) {
		OSCL_LOGE("malloc paudio.para error!\n");
		oscl_free(audio_private);
		return NULL;
	}

	return audio_private;
}
