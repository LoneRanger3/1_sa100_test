/*
* copyright (c) 2018 All Rights Reserved
*
* This file is part of mp4 muxer.
*
* File   : mp4_muxer.h
* Version: V1.0
* Date   : 2018/11/28 15:34:53
* Author : lszhang
*/
#include "muxer_com.h"
#include "avformat.h"
#include "movenc.h"
#include <oscl.h>

int emp4_deinit(format_muxer *muxer)
{
	AVFormatContext *s = (AVFormatContext *)(muxer->priv);
	int i = 0;

	if (s) {
		mov_free(s);
		for (i = 0; i < s->nb_streams; i++) {
			if (s->streams[i]) {
				if (s->streams[i]->codecpar) {
					free(s->streams[i]->codecpar);
					s->streams[i]->codecpar = NULL;
				}
				free(s->streams[i]);
				s->streams[i] = NULL;

			}

		}
		if (s->priv_data) {
			free(s->priv_data);
			s->priv_data = NULL;
		}
		free(s);
		s = NULL;
	}

	return 0;
}

int emp4_init(format_muxer *muxer)
{
	int ret = 0;
	int i = 0;
	MOVMuxContext *mov = NULL;
	AVFormatContext *s = malloc(sizeof(AVFormatContext));
	if (!s)
		return 0;

	memset(s, 0, sizeof(AVFormatContext));
	muxer->priv = (void *)s;
	mov = malloc(sizeof(MOVMuxContext));
	if (!mov) {
		PRINTF("malloc error!\n");
		goto err;
	}
	memset(mov, 0, sizeof(MOVMuxContext));
	s->priv_data = mov;
	mov->mode = muxer->mode;
	s->nb_streams = muxer->nb_streams;/* audio + video */
	for (i = 0; i < s->nb_streams; i++) {
		AVStream *st = malloc(sizeof(AVStream));
		if (!st) {
			PRINTF("malloc error!\n");
			goto err;
		}
		memset(st, 0, sizeof(AVStream));
		s->streams[i] = st;
		st->codecpar = malloc(sizeof(AV_CodecParameters));
		if (!st->codecpar) {
			PRINTF("malloc error!\n");
			goto err;
		}
		memset(st->codecpar, 0, sizeof(AV_CodecParameters));
		st->codecpar->codec_type = muxer->para[i].codec_type;
		st->codecpar->codec_id = muxer->para[i].codec_id;
		st->codecpar->sample_rate = muxer->para[i].sample_rate;
		st->codecpar->channels = muxer->para[i].channels;
		st->codecpar->channel_layout = muxer->para[i].channel_layout;
		st->codecpar->block_align = muxer->para[i].block_align;
		st->codecpar->frame_size = muxer->para[i].frame_size;
		st->codecpar->extradata = muxer->para[i].extradata;
		st->codecpar->extradata_size = muxer->para[i].extradata_size;
		st->codecpar->width = muxer->para[i].width;
		st->codecpar->height = muxer->para[i].height;
		if (st->codecpar->codec_type == AV_MEDIA_TYPE_VIDEO)
			st->time_base.den = muxer->para[i].sample_rate;
	}
	ret = mov_init(s);
	return ret;
err:
	emp4_deinit(muxer);
	return -1;
}

int emp4_write_header(format_muxer *muxer)
{
	AVFormatContext *s = (AVFormatContext *)(muxer->priv);
	int ret = 0;

	if (muxer->fp)
		s->pb = (AVIOContext *)muxer->fp;
	else
		PRINTF("muxer->fp should not be NULL\n");

	ret = mov_write_header(s);

	return ret;
}

int emp4_write_packet(format_muxer *muxer, packet_t *pin)
{
	AVFormatContext *s = (AVFormatContext *)(muxer->priv);
	int ret = 0;
	int index = pin->stream_index;
	AVPacket packet = { 0 };
	packet.data = pin->data;
	packet.size = pin->size;
	packet.dts = pin->dts;
	packet.pts = pin->pts;
	packet.duration = pin->duration;
	packet.flags = pin->flags;
	/* audio */
	packet.stream_index = index;
	if (s->streams[index]->codecpar->extradata_size !=
		muxer->para[index].extradata_size) {
		s->streams[index]->codecpar->extradata = muxer->para[index].extradata;
		s->streams[index]->codecpar->extradata_size
			= muxer->para[index].extradata_size;
	}
	ret = mov_write_packet(s, &packet);
	/*AVFormatContext *sContext = (AVFormatContext *)parser;
	ret = mp4_read_packet(sContext, &pkt);
	memset(pin, 0, sizeof(packet_t));
	pin->data = (uint16_t*)pkt.data;
	pin->size = pkt.size;*/
	return ret;
}
int emp4_write_trailer(format_muxer *muxer)
{
	AVFormatContext *s = (AVFormatContext *)(muxer->priv);
	MOVMuxContext *mov = s->priv_data;

	if (muxer->user_data.is_valid) {
		mov->free_buff = muxer->user_data.pdata;
		mov->free_data_size = muxer->user_data.size;
	}

	mov_write_trailer(s);

	muxer->user_data.is_valid = 0;
	muxer->user_data.pdata = NULL;
	muxer->user_data.size = 0;

	return 0;
}

int emp4_check_bitstream(format_muxer *muxer, packet_t *pin)
{
	int ret = 1;

	return ret;
}

