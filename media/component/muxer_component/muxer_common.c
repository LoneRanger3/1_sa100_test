/*
 * mp4_drv.c - Standard functionality for the media encodec component.
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
#define DBG_LEVEL		DBG_INFO
#define LOG_TAG			"muxer_common"

#include <oscl.h>

#include "muxer_com.h"
#include "avformat.h"
#include "muxer_common.h"
#include "audio_config.h"

int muxer_init(struct format_muxer_st *muxer, int mode)
{
	muxer->mode = mode;
	/*muxer->nb_streams = 2;*/
	if (muxer->para[AUDIO_INDEX].codec_id == (int)AV_CODEC_TYPE_AAC) {
		ga_config_data gadata = { 0 };
		muxer->para[AUDIO_INDEX].extradata =
			malloc(8 + AV_INPUT_BUFFER_PADDING_SIZE);

		if (!muxer->para[AUDIO_INDEX].extradata)
			return -1;

		memset(muxer->para[AUDIO_INDEX].extradata, 0,
			8 + AV_INPUT_BUFFER_PADDING_SIZE);
		gadata.audioObjectType = AACLC;/* AACLC; */
		gadata.samplingFrequency = muxer->para[AUDIO_INDEX].sample_rate;
		gadata.numChannels = muxer->para[AUDIO_INDEX].channels;
		muxer->para[AUDIO_INDEX].extradata_size =
			put_audio_specific_config(muxer->para[AUDIO_INDEX].extradata,
				8, &gadata);
	}

	if (muxer->mode == AV_MUXER_TYPE_RAW) {
		muxer->init = raw_init;
		muxer->write_header = raw_write_header;
		muxer->write_packet = raw_write_packet;
		muxer->write_trailer = raw_write_trailer;
		muxer->deinit = raw_deinit;
		muxer->check_bitstream = raw_check_bitstream;
		muxer->ctrl = raw_ctrl;
	} else if (muxer->mode == AV_MUXER_TYPE_TS) {
		muxer->init = ts_init;
		muxer->write_header = ts_write_head;
		muxer->write_packet = ts_write;
		muxer->write_trailer = ts_write_end;
		muxer->deinit = ts_deinit;
		muxer->check_bitstream = ts_check_bitstream;
		muxer->ctrl = ts_ctrl;
	} else {
		muxer->init = emp4_init;
		muxer->write_header = emp4_write_header;
		muxer->write_packet = emp4_write_packet;
		muxer->write_trailer = emp4_write_trailer;
		muxer->deinit = emp4_deinit;
		muxer->check_bitstream = emp4_check_bitstream;
	}

	return muxer->init(muxer);
}

int muxer_write_header(struct format_muxer_st *muxer)
{
	return muxer->write_header(muxer);
}

int muxer_write_packet(struct format_muxer_st *muxer, packet_t *pin)
{
	if (muxer->para[pin->stream_index].codec_id == AV_CODEC_TYPE_AAC) {
		pin->duration = 1024;
	} else if ((muxer->para[pin->stream_index].codec_id > AV_CODEC_TYPE_INVALID) &&
		(muxer->para[pin->stream_index].codec_id < AV_CODEC_TYPE_FIRST_AUDIO)) {
		pin->duration = VIDEOTIMEFRAME;
	}

	return muxer->write_packet(muxer, pin);
}

int muxer_write_trailer(struct format_muxer_st *muxer)
{
	return muxer->write_trailer(muxer);
}

int muxer_deinit(struct format_muxer_st *muxer)
{
	int i = 0;
	int ret;

	ret = muxer->deinit(muxer);
	for (i = 0; i < muxer->nb_streams; i++) {
		if (muxer->para[i].extradata) {
			oscl_free(muxer->para[i].extradata);
			muxer->para[i].extradata = NULL;
			muxer->para[i].extradata_size = 0;
		}
	}
	if (muxer->filename != NULL) {
		oscl_free(muxer->filename);
		muxer->filename = NULL;
	}

	return ret;
}

int muxer_check_bitstream(format_muxer *muxer, packet_t *pin)
{
	return muxer->check_bitstream(muxer, pin);
}

int muxer_ctrl(format_muxer *muxer, int cmd, void *param)
{
	if (muxer->ctrl == NULL)
		return -1;

	return muxer->ctrl(muxer, cmd, param);
}

