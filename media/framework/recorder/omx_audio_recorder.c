/*
 * omx_audio_recorder.c - Standard functionality for audio stream recorder.
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
#define DBG_LEVEL		DBG_ERR
#include <oscl.h>
#include <base_component.h>
#include "vrender_component.h"
#include "vrec_component.h"
#include "framework_common.h"
#include "lb_recorder.h"
#include "recorder_private.h"

#define AUDIO_DEFAULT_RATE		8000
#define AUDIO_DEFAULT_CHANNELS		1
#define AUDIO_DEFAULT_FMT_WIDTH		16
#define AUDIO_DEFAULT_CODINGTYPE	OMX_AUDIO_CodingAAC

#define IS_ASTREAM_HDL_VALID(hdl) ((hdl) > 0 && (hdl) <= AUDIO_STREAM_MAXHDL)
#define _get_index(hdle) (hdle - 1)

audio_stream_t *g_audio_rec;
static void _update_state(audio_stream_t *astream)
{
	int i;

	OSCL_LOGI("astream->cur_state:%d!", astream->cur_state);
	astream->cur_state = REC_STATE_ERR;
	for (i = 0; i < AUDIO_STREAM_MAXHDL; i++) {
		if (astream->state[i] == REC_STATE_RECORD) {
			astream->cur_state = REC_STATE_RECORD;
			break;
		}
		if (astream->state[i] == REC_STATE_PREPARED
			&& astream->cur_state != REC_STATE_RECORD) {
			astream->cur_state = REC_STATE_PREPARED;
		}
		if (astream->state[i] == REC_STATE_INIT
			&& astream->cur_state == REC_STATE_ERR) {
			astream->cur_state = REC_STATE_INIT;
		}
	}
	OSCL_LOGI("astream->cur_state:%d)", astream->cur_state);
}

static void _get_config_audio(audio_stream_t *astream)
{
	astream->channels = AUDIO_DEFAULT_CHANNELS;
	astream->bit_per_sample = AUDIO_DEFAULT_FMT_WIDTH;
	astream->sample_rate = AUDIO_DEFAULT_RATE;
	astream->coding_type = AUDIO_DEFAULT_CODINGTYPE;
	OSCL_LOGI("channels:%d, bit_per_sample:%d, sample_rate:%d, coding_type:%d)",
		astream->channels,
		astream->bit_per_sample,
		astream->sample_rate,
		astream->coding_type);
}

astream_handle _astream_hdle_request(void)
{
	int i;
	astream_handle hdl = 0;
	al_port_info_t *splt_port;
	audio_stream_t *astream = g_audio_rec;

	if (astream) {
		splt_port = astream->aenc_splt.al_comp.port_info;
		for (i = 0; i < AUDIO_STREAM_MAXHDL; i++) {
			if ((astream->hdl_map & (1 << i)) == 0) {
				astream->hdl_map |= (1 << i);
				astream->mux_info[i] = NULL;
				astream->state[i] = REC_STATE_INIT;
				astream->aenc_splt_out[i] = &splt_port[i + 1];
				hdl = i + 1;
				astream->ref++;
				break;
			}
		}
		_update_state(astream);
	}
	return hdl;
}

void _astream_hdle_release(astream_handle hdl)
{
	int index;
	audio_stream_t *astream = g_audio_rec;

	OSCL_TRACE("hdl:%x", hdl);
	if (!IS_ASTREAM_HDL_VALID(hdl) || astream == NULL) {
		OSCL_LOGE("input hdl err!");
		return;
	}
	index = _get_index(hdl);
	if (index < 0) {
		OSCL_LOGE("can not find audio hdl:%x", hdl);
		return;
	}
	if ((astream->hdl_map & (1 << index)) != 0) {
		astream->hdl_map &= ~(1 << index);
		astream->mux_info[index] = NULL;
		astream->aenc_splt_out[index] = NULL;
		astream->state[index] = REC_STATE_ERR;
		astream->ref--;
		_update_state(astream);
	}
	OSCL_TRACE("hdl:%x", hdl);
}

/**
 * astream_set_muxer - stop audio stream muxer.
 *
 * @hdl: audio stream handle
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int astream_set_muxer(astream_handle hdl, mux_info_t *muxer)
{
	int index;
	audio_stream_t *astream = g_audio_rec;

	OSCL_TRACE("hdl:%x", hdl);
	if (!IS_ASTREAM_HDL_VALID(hdl) || astream == NULL) {
		OSCL_LOGE("input hdl err!");
		return -1;
	}
	pthread_mutex_lock(&astream->lock);
	index = _get_index(hdl);
	if (index < 0) {
		OSCL_LOGE("can not find audio hdl:%x", hdl);
		pthread_mutex_unlock(&astream->lock);
		return -1;
	}
	if (muxer == NULL && astream->mux_info[index]) {
		OSCL_LOGI("");
		al_untunnel_unset_ports(astream->aenc_splt_out[index],
					astream->mux_info[index]->ain);
		OSCL_LOGI("");
	}
	astream->mux_info[index] = muxer;
	pthread_mutex_unlock(&astream->lock);
	OSCL_TRACE("hdl:%x", hdl);
	return 0;
}

/**
 * _astream_stop - stop audio stream.
 *
 * @hdl: audio stream handle
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int _astream_stop(astream_handle hdl)
{
	int index;
	int ret = 0;
	audio_stream_t *astream = g_audio_rec;

	OSCL_TRACE("hdl:%x", hdl);
	if (!IS_ASTREAM_HDL_VALID(hdl) || astream == NULL) {
		OSCL_LOGE("input hdl err!");
		return -1;
	}
	index = _get_index(hdl);

	OSCL_LOGI("astream->cur_state:%d!", astream->cur_state);
	oscl_param_check_exit(astream->cur_state == REC_STATE_RECORD, 0, NULL);
	OSCL_LOGI("astream->state[index]:%d!", astream->state[index]);
	oscl_param_check_exit(astream->state[index] == REC_STATE_RECORD, 0, NULL);

	/* enable encoder port of video splitter */
	al_component_sendcom(&astream->aenc_splt.al_comp, OMX_CommandPortDisable,
		astream->aenc_splt_out[index]->index, NULL);
	al_component_setstate(&astream->mux_info[index]->al_comp, OMX_StateIdle);
	OSCL_TRACE("");
	al_untunnel_unset_ports(astream->aenc_splt_out[index],
		astream->mux_info[index]->ain);
	OSCL_TRACE("");

	astream->state[index] = REC_STATE_PREPARED;
	_update_state(astream);
	if (astream->cur_state == REC_STATE_RECORD)
		goto EXIT;
	/* change to idle state for recorder components*/
	al_component_setstate(&astream->aenc_splt.al_comp, OMX_StateIdle);
	al_component_setstate(&astream->aenc_info.al_comp, OMX_StateIdle);
	al_component_setstate(&astream->asrc_info.al_comp, OMX_StateIdle);
EXIT:
	OSCL_TRACE("hdl:%x", hdl);
	return ret;
}

/**
 * _astream_reset - reset audio stream.
 *
 * @hdl: audio stream handle
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int _astream_reset(astream_handle hdl)
{
	int index;
	int ret = 0;
	audio_stream_t *astream = g_audio_rec;

	OSCL_TRACE("hdl:%x", hdl);
	if (!IS_ASTREAM_HDL_VALID(hdl) || astream == NULL) {
		OSCL_LOGE("input hdl err!");
		return -1;
	}
	_astream_stop(hdl);

	index = _get_index(hdl);
	oscl_param_check_exit((astream->state[index] != REC_STATE_INIT), 0, NULL);
	oscl_param_check_exit((astream->state[index] == REC_STATE_PREPARED), 0, NULL);
	astream->state[index] = REC_STATE_INIT;
	_update_state(astream);
	oscl_param_check_exit((astream->cur_state == REC_STATE_INIT), 0, NULL);

	al_untunnel_unset_ports(astream->aenc_info.aout,
		astream->aenc_splt.in);
	al_untunnel_unset_ports(astream->asrc_info.aout,
		astream->aenc_info.ain);
	if (astream->mux_info[index] != NULL) {
		al_untunnel_unset_ports(astream->aenc_splt_out[index],
					astream->mux_info[index]->ain);
		astream->mux_info[index] = NULL;
	}

	/* change to idle state for recorder components*/
	al_component_setstate(&astream->asrc_info.al_comp, OMX_StateLoaded);
	al_component_setstate(&astream->aenc_info.al_comp, OMX_StateLoaded);
	al_component_setstate(&astream->aenc_splt.al_comp, OMX_StateLoaded);

EXIT:
	OSCL_TRACE("hdl:%x", hdl);
	return ret;
}

/**
 * _setup_asrc - set para for audio receiver components
 *
 * @astream: astream
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int _setup_asrc(audio_stream_t *astream)
{
	OMX_AUDIO_PARAM_PCMMODETYPE pcm_type;
	int ret = 0;

	OSCL_TRACE("astream:%x", astream);
	/* setup para for audio receiver components*/
	memset(&pcm_type, 0, sizeof(pcm_type));
	pcm_type.nChannels = astream->channels;
	pcm_type.nBitPerSample = astream->bit_per_sample;
	pcm_type.nSamplingRate = astream->sample_rate;
	ret = OMX_SetParameter(astream->asrc_info.al_comp.cmp_hdl,
			OMX_IndexParamAudioPcm,
			&pcm_type);
	OSCL_TRACE("astream:%x", astream);
	return ret;
}

/**
 * _setup_aenc - set para for audio encoder components
 *
 * @astream: astream
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int _setup_aenc(audio_stream_t *astream, astream_handle hdl)
{
	OMX_AUDIO_PARAM_ADPCMTYPE adpcm_type;
	OMX_AUDIO_PARAM_PCMMODETYPE pcm_type;
	OMX_AUDIO_PARAM_AACPROFILETYPE aac_type;
	int ret = 0;
	int index;
	OSCL_TRACE("hdl:%x", hdl);
	index = _get_index(hdl);

	switch (astream->coding_type) {
	case OMX_AUDIO_CodingADPCM:
		memset(&adpcm_type, 0, sizeof(OMX_AUDIO_PARAM_ADPCMTYPE));
		adpcm_type.nChannels = astream->channels;
		adpcm_type.nBitsPerSample = astream->bit_per_sample;
		adpcm_type.nSampleRate = astream->sample_rate;
		ret = OMX_SetParameter(astream->aenc_info.al_comp.cmp_hdl,
				OMX_IndexParamAudioAdpcm,
				&adpcm_type);
		break;

	case OMX_AUDIO_CodingAAC:
		memset(&aac_type, 0, sizeof(OMX_AUDIO_PARAM_AACPROFILETYPE));
		aac_type.nChannels = astream->channels;
		aac_type.nSampleRate = astream->sample_rate;
		aac_type.nBitRate = 128000;
		if (astream->mux_info[index]->fmt == REC_OUTPUT_FORMAT_AUDIO_AAC ||
		    astream->mux_info[index]->fmt == REC_OUTPUT_FORMAT_TS)
			aac_type.eAACStreamFormat = OMX_AUDIO_AACStreamFormatMP4ADTS;
		else
			aac_type.eAACStreamFormat = OMX_AUDIO_AACStreamFormatRAW;
		OSCL_LOGI("aac setting: channels %d, samplerate:%d, bitrate:%d",
			aac_type.nChannels, aac_type.nSampleRate, aac_type.nBitRate);
		ret = OMX_SetParameter(astream->aenc_info.al_comp.cmp_hdl,
				OMX_IndexParamAudioAac,
				&aac_type);
		break;

	case OMX_AUDIO_CodingPCM:
	default:
		memset(&pcm_type, 0, sizeof(pcm_type));
		pcm_type.nChannels = astream->channels;
		pcm_type.nBitPerSample = astream->bit_per_sample;
		pcm_type.nSamplingRate = astream->sample_rate;
		ret = OMX_SetParameter(astream->aenc_info.al_comp.cmp_hdl,
				OMX_IndexParamAudioPcm,
				&pcm_type);
		break;
	}
	OSCL_TRACE("hdl:%x", hdl);
	return ret;
}

/**
 * _setup_amuxer - set para for audio muxer components
 *
 * @astream: astream
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int _setup_amuxer(audio_stream_t *astream, mux_info_t *muxer)
{
	OMX_AUDIO_PARAM_ADPCMTYPE adpcm_type;
	OMX_AUDIO_PARAM_PCMMODETYPE pcm_type;
	OMX_AUDIO_PARAM_AACPROFILETYPE aac_type;
	int ret = 0;

	OSCL_TRACE("muxer:%x", muxer);
	/* setup para for audio encoder components and muxer */
	switch (astream->coding_type) {
	case OMX_AUDIO_CodingADPCM:
		memset(&adpcm_type, 0, sizeof(OMX_AUDIO_PARAM_ADPCMTYPE));
		adpcm_type.nChannels = astream->channels;
		adpcm_type.nBitsPerSample = astream->bit_per_sample;
		adpcm_type.nSampleRate = astream->sample_rate;
		ret = OMX_SetParameter(muxer->al_comp.cmp_hdl,
				OMX_IndexParamAudioAdpcm,
				&adpcm_type);
		break;

	case OMX_AUDIO_CodingAAC:
		memset(&aac_type, 0, sizeof(OMX_AUDIO_PARAM_AACPROFILETYPE));
		aac_type.nChannels = astream->channels;
		aac_type.nSampleRate = astream->sample_rate;
		aac_type.nBitRate = 128000;

		if (muxer->fmt == REC_OUTPUT_FORMAT_AUDIO_AAC ||
		    muxer->fmt == REC_OUTPUT_FORMAT_TS)
			aac_type.eAACStreamFormat = OMX_AUDIO_AACStreamFormatMP4ADTS;
		else
			aac_type.eAACStreamFormat = OMX_AUDIO_AACStreamFormatRAW;
		ret = OMX_SetParameter(muxer->al_comp.cmp_hdl,
				OMX_IndexParamAudioAac,
				&aac_type);
		break;

	case OMX_AUDIO_CodingPCM:
	default:
		memset(&pcm_type, 0, sizeof(pcm_type));
		pcm_type.nChannels = astream->channels;
		pcm_type.nBitPerSample = astream->bit_per_sample;
		pcm_type.nSamplingRate = astream->sample_rate;
		ret = OMX_SetParameter(muxer->al_comp.cmp_hdl,
				OMX_IndexParamAudioPcm,
				&pcm_type);
		break;
	}
	OSCL_TRACE("ret:%x", ret);
	return ret;
}

int astream_get_sample_rate(astream_handle hdl)
{
	audio_stream_t *astream = g_audio_rec;

	return astream->sample_rate;
}

int astream_get_sample_width(astream_handle hdl)
{
	audio_stream_t *astream = g_audio_rec;

	return astream->bit_per_sample;
}

int astream_get_channels(astream_handle hdl)
{
	audio_stream_t *astream = g_audio_rec;

	return astream->channels;
}

int astream_get_coding_type(astream_handle hdl)
{
	audio_stream_t *astream = g_audio_rec;

	return astream->coding_type;
}

/**
 * astream_get_cur_time - get audio stream time.
 *
 * @hdl: audio stream handle
 *
 * Returns 0 on success, -EERROR otherwise..
 */
s64 astream_get_time(astream_handle hdl)
{
	int ret = 0;
	audio_stream_t *astream = g_audio_rec;
	OMX_TIME_CONFIG_TIMESTAMPTYPE time;
	s64 cur_time = 0;

	OSCL_TRACE("hdl:%x", hdl);
	oscl_param_check(IS_ASTREAM_HDL_VALID(hdl), -1, NULL);
	oscl_param_check(astream != NULL, -1, NULL);
	oscl_param_check(astream->asrc_info.aout != NULL, -1, NULL);

	time.nPortIndex = astream->asrc_info.aout->index;
	ret = OMX_GetConfig(astream->asrc_info.al_comp.cmp_hdl,
			    omx_index_lombo_config_cur_time, &time);
	if (ret == 0)
		cur_time = time.nTimestamp;

	return cur_time;
}

/**
 * astream_set_sample_rate - astream_set_sample_rate
 *
 * @hdl: recorder handle.
 * @rate: sample rate.
 */
int astream_set_sample_rate(astream_handle hdl, int rate)
{
	audio_stream_t *astream = g_audio_rec;

	OSCL_TRACE("hdl:%x", hdl);
	if (!IS_ASTREAM_HDL_VALID(hdl) || astream == NULL) {
		OSCL_LOGE("input hdl err!");
		return -1;
	}
	astream->sample_rate = rate;
	OSCL_TRACE("hdl:%x", hdl);
	return 0;
}

/**
 * astream_set_bit_per_sample - astream_set_bit_per_sample
 *
 * @hdl: recorder handle.
 * @bits: bits per sample.
 */
int astream_set_sample_width(astream_handle hdl, int bits)
{
	audio_stream_t *astream = g_audio_rec;

	OSCL_TRACE("hdl:%x", hdl);
	if (!IS_ASTREAM_HDL_VALID(hdl) || astream == NULL) {
		OSCL_LOGE("input hdl err!");
		return -1;
	}
	astream->bit_per_sample = bits;
	OSCL_TRACE("hdl:%x", hdl);
	return 0;
}

/**
 * astream_set_channels - astream_set_channels
 *
 * @hdl: recorder handle.
 * @channels: number of channel.
 */
int astream_set_channels(astream_handle hdl, int channels)
{
	audio_stream_t *astream = g_audio_rec;

	OSCL_TRACE("hdl:%x", hdl);
	if (!IS_ASTREAM_HDL_VALID(hdl) || astream == NULL) {
		OSCL_LOGE("input hdl err!");
		return -1;
	}
	astream->channels = channels;
	OSCL_TRACE("hdl:%x", hdl);
	return 0;
}

/**
 * astream_set_prio - astream_set_prio
 *
 * @hdl: recorder handle.
 * @prio: prio.
 */
int astream_set_prio(astream_handle hdl, int prio)
{
	audio_stream_t *astream = g_audio_rec;
	OMX_PRIORITYMGMTTYPE priority;

	OSCL_TRACE("hdl:%x", hdl);
	if (!IS_ASTREAM_HDL_VALID(hdl) || astream == NULL) {
		OSCL_LOGE("input hdl err!");
		return -1;
	}
	priority.nVersion.nVersion = OMX_VERSION;
	priority.nGroupPriority = prio;
	OSCL_TRACE("hdl:%x", hdl);
	OSCL_TRACE("astream:%x", astream);
	OSCL_TRACE("astream->asrc_info.al_comp.cmp_hdl:%x",
		   astream->asrc_info.al_comp.cmp_hdl);
	oscl_param_check(astream->asrc_info.al_comp.cmp_hdl, -1, NULL);
	OSCL_TRACE("astream->asrc_info.al_comp.cmp_hdl:%x",
		   astream->asrc_info.al_comp.cmp_hdl);
	OSCL_TRACE("astream->asrc_info.al_comp.cmp_hdl->SetParameter:%x",
		   astream->asrc_info.al_comp.cmp_hdl->SetParameter);
	OMX_SetParameter(astream->asrc_info.al_comp.cmp_hdl,
		OMX_IndexParamPriorityMgmt, &priority);
	OSCL_TRACE("hdl:%x", hdl);
	oscl_param_check(astream->aenc_info.al_comp.cmp_hdl, -1, NULL);
	OMX_SetParameter(astream->aenc_info.al_comp.cmp_hdl,
		OMX_IndexParamPriorityMgmt, &priority);
	OSCL_TRACE("hdl:%x", hdl);
	oscl_param_check(astream->aenc_splt.al_comp.cmp_hdl, -1, NULL);
	OMX_SetParameter(astream->aenc_splt.al_comp.cmp_hdl,
		OMX_IndexParamPriorityMgmt, &priority);
	OSCL_TRACE("hdl:%x", hdl);
	return 0;
}


/**
 * astream_set_coding - astream_set_coding
 *
 * @hdl: recorder handle.
 * @coding: coding type.
 */
int astream_set_coding_type(astream_handle hdl, OMX_AUDIO_CODINGTYPE coding)
{
	audio_stream_t *astream = g_audio_rec;

	OSCL_TRACE("hdl:%x", hdl);
	if (!IS_ASTREAM_HDL_VALID(hdl) || astream == NULL) {
		OSCL_LOGE("input hdl err!");
		return -1;
	}
	astream->coding_type = coding;
	OSCL_TRACE("hdl:%x", hdl);
	return 0;
}

/**
 * astream_set_mute - astream_set_mute
 *
 * @hdl: recorder handle.
 * @coding: coding type.
 */
int astream_set_mute(astream_handle hdl, int mute_on)
{
	audio_stream_t *astream = g_audio_rec;
	OMX_AUDIO_CONFIG_MUTETYPE mute;
	int ret;

	OSCL_TRACE("hdl:%x", hdl);
	if (!IS_ASTREAM_HDL_VALID(hdl) || astream == NULL) {
		OSCL_LOGE("input hdl err!");
		return -1;
	}
	memset(&mute, 0, sizeof(OMX_AUDIO_CONFIG_MUTETYPE));
	if (mute_on)
		mute.bMute = OMX_TRUE;
	mute.nPortIndex = g_audio_rec->asrc_info.aout->index;
	ret = OMX_SetConfig(g_audio_rec->asrc_info.al_comp.cmp_hdl,
			    OMX_IndexConfigAudioMute, &mute);
	OSCL_TRACE("hdl:%x", hdl);
	return ret;
}


/**
 * astream_prepare - astream preapra, setup audio component and set state to idle.
 *
 * @astream_handle: astream handle
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int astream_prepare(astream_handle hdl)
{
	OMX_PARAM_PORTDEFINITIONTYPE omx_port;
	int ret = 0;
	int index;
	audio_stream_t *astream = g_audio_rec;
	mux_info_t *muxer;
	OSCL_TRACE("hdl:%x", hdl);

	if (!IS_ASTREAM_HDL_VALID(hdl)) {
		OSCL_LOGE("input hdl invalid:%x!", hdl);
		return -1;
	}
	index = _get_index(hdl);
	muxer = astream->mux_info[index];
	oscl_param_check((muxer != NULL), -1, NULL);
	oscl_param_check((astream->cur_state != REC_STATE_ERR), -1, NULL);
	oscl_param_check(((astream->state[index] == REC_STATE_INIT)
		|| (astream->state[index] == REC_STATE_PREPARED)), -1, NULL);

	pthread_mutex_lock(&astream->lock);

	/* audio source and encoder should only set once */
	if (astream->cur_state == REC_STATE_INIT) {
		/* set para to audio component */
		ret = _setup_asrc(astream);
		oscl_param_check_exit((ret == 0), ret, NULL);
		ret = _setup_aenc(astream, hdl);
		oscl_param_check_exit((ret == 0), ret, NULL);

		/* setup splitter port definition */
		omx_port.nPortIndex = astream->aenc_info.aout->index;
		omx_port.nVersion.nVersion = OMX_VERSION;
		ret = OMX_GetParameter(astream->aenc_info.al_comp.cmp_hdl,
				OMX_IndexParamPortDefinition, &omx_port);
		oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);
		_splt_prepare(&astream->aenc_splt, &omx_port);

		/* connect audio source with audio encoder */
		OSCL_TRACE("");
		ret = al_untunnel_setup_ports(astream->asrc_info.aout,
				astream->aenc_info.ain);
		oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

		/* connect audio splitter with audio encoder */
		OSCL_TRACE("");
		ret = al_untunnel_setup_ports(astream->aenc_info.aout,
				astream->aenc_splt.in);
		OSCL_TRACE("");
		oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

		/* change to idle state for audio components*/
		al_component_setstate(&astream->aenc_splt.al_comp, OMX_StateIdle);
		al_component_setstate(&astream->aenc_info.al_comp, OMX_StateIdle);
		al_component_setstate(&astream->asrc_info.al_comp, OMX_StateIdle);
	}

	ret = _setup_amuxer(astream, muxer);
	oscl_param_check_exit((ret == 0), ret, NULL);

	/* connect audio splitter with audio muxer */
	OSCL_LOGI("out %d %x, in:%x", index, astream->aenc_splt_out[index], muxer->ain);
	if (muxer->ain->state == AL_PORT_STATE_INIT) {
		ret = al_untunnel_setup_ports(astream->aenc_splt_out[index], muxer->ain);
		oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);
	}

	astream->state[index] = REC_STATE_PREPARED;
EXIT:
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("video_rec_prepare error! ret:%x", ret);
		astream->state[index] = REC_STATE_INIT;
		al_component_setstate(&astream->aenc_splt.al_comp, OMX_StateLoaded);
		al_component_setstate(&astream->aenc_info.al_comp, OMX_StateLoaded);
		al_component_setstate(&astream->asrc_info.al_comp, OMX_StateLoaded);
		OSCL_LOGE("");
		al_untunnel_unset_ports(astream->aenc_splt_out[index], muxer->ain);
		OSCL_LOGE("");
		al_untunnel_unset_ports(astream->aenc_info.aout,
			astream->aenc_splt.in);
		OSCL_LOGE("");
		al_untunnel_unset_ports(astream->asrc_info.aout,
			astream->aenc_info.ain);
		OSCL_LOGE("");
	}
	_update_state(astream);
	pthread_mutex_unlock(&astream->lock);
	OSCL_TRACE("hdl:%x astream->cur_state:%d", hdl, astream->cur_state);
	return ret;
}

void _astream_init(void)
{
	g_audio_rec = oscl_zalloc(sizeof(audio_stream_t));
	if (g_audio_rec == NULL)
		return;
	pthread_mutex_init(&g_audio_rec->lock, NULL);
}

/**
 * audio_stream_creat - get a astream handle.
 * This function will load all components audio stream needs.
 *
 * Returns audio stream handle on success, 0 otherwise..
 */
astream_handle astream_creat(void)
{
	audio_stream_t *astream;
	astream_handle hdl = 0;
	int index;
	int ret = 0;

	if (g_audio_rec == NULL)
		_astream_init();
	if (g_audio_rec == NULL)
		return 0;

	astream = g_audio_rec;
	pthread_mutex_lock(&astream->lock);
	hdl = _astream_hdle_request();
	if (hdl == 0 || astream->ref > 1) {
		OSCL_LOGE("hdl:%d, ref:%d", hdl, astream->ref);
		goto EXIT;
	}
	OSCL_TRACE("hdl:%x", hdl);

	/* initialize audio config */
	_get_config_audio(astream);

	/* init audio source component */
	ret = al_component_init(&astream->asrc_info.al_comp, "OMX.LB.SOURCE.AREC",
			&al_untunnel_common_callbacks);
	oscl_param_check_exit(ret == 0, ret, NULL);
	index = al_get_port_index(&astream->asrc_info.al_comp, OMX_DirOutput,
			OMX_PortDomainAudio);
	oscl_param_check_exit(index >= 0, -1, NULL);
	astream->asrc_info.aout = &astream->asrc_info.al_comp.port_info[index];

	/* init audio enc component */
	ret = al_component_init(&astream->aenc_info.al_comp, "OMX.LB.ENCODER.AUDIO",
			&al_untunnel_common_callbacks);
	oscl_param_check_exit(ret == 0, ret, NULL);
	index = al_get_port_index(&astream->aenc_info.al_comp, OMX_DirInput,
			OMX_PortDomainAudio);
	oscl_param_check_exit(index >= 0, -1, NULL);
	astream->aenc_info.ain = &astream->aenc_info.al_comp.port_info[index];
	index = al_get_port_index(&astream->aenc_info.al_comp, OMX_DirOutput,
			OMX_PortDomainAudio);
	oscl_param_check_exit(index >= 0, -1, NULL);
	astream->aenc_info.aout = &astream->aenc_info.al_comp.port_info[index];
	OSCL_LOGD("aenc buffer number:%d", astream->aenc_info.aout->nbuffer);

	/* init audio encoder splitter component */
	ret = al_component_init(&astream->aenc_splt.al_comp,
			"OMX.LB.SPLITTER.BASE",
			&al_untunnel_common_callbacks);
	oscl_param_check_exit(ret == 0, ret, NULL);
	astream->aenc_splt.in = &astream->aenc_splt.al_comp.port_info[0];

	index = _get_index(hdl);
	astream->aenc_splt_out[index] = &astream->aenc_splt.al_comp.port_info[index + 1];

EXIT:
	pthread_mutex_unlock(&astream->lock);
	if (ret != 0) {
		OSCL_LOGE("astream creat fail:%x", ret);
		astream_release(hdl);
		hdl = 0;
	}
	OSCL_TRACE("hdl:%x astream->cur_state:%d", hdl, astream->cur_state);
	return hdl;
}

/**
 * astream_release - release resource recorder used.
 *
 * @hdl: recorder handle.
 */
void astream_release(astream_handle hdl)
{
	audio_stream_t *astream = g_audio_rec;

	OSCL_TRACE("hdl:%x", hdl);
	if (astream == NULL)
		return;
	if (!IS_ASTREAM_HDL_VALID(hdl)) {
		OSCL_TRACE("hdl:%x astream->cur_state:%d", hdl, astream->cur_state);
		return;
	}

	/* only deinit astream when all audio handle released */
	pthread_mutex_lock(&astream->lock);
	_astream_reset(hdl);
	_astream_hdle_release(hdl);
	if (astream->ref > 0) {
		OSCL_LOGI("cur state:%d", astream->cur_state);
		pthread_mutex_unlock(&astream->lock);
		return;
	}
	al_component_deinit(&astream->aenc_splt.al_comp);
	al_component_deinit(&astream->aenc_info.al_comp);
	al_component_deinit(&astream->asrc_info.al_comp);
	pthread_mutex_unlock(&astream->lock);

	OSCL_TRACE("hdl:%x astream->cur_state:%d", hdl, astream->cur_state);
	pthread_mutex_destroy(&astream->lock);
	if (astream->hdl_map == 0) {
		oscl_free(g_audio_rec);
		g_audio_rec = NULL;
	}
}

/**
 * astream_reset - reset audio stream.
 *
 * @hdl: audio stream handle
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int astream_reset(astream_handle hdl)
{
	audio_stream_t *astream = g_audio_rec;
	int ret;
	OSCL_TRACE("hdl:%x", hdl);
	oscl_param_check(IS_ASTREAM_HDL_VALID(hdl), -1, NULL);
	oscl_param_check(astream != NULL, -1, NULL);

	pthread_mutex_lock(&astream->lock);
	ret = _astream_reset(hdl);
	pthread_mutex_unlock(&astream->lock);
	OSCL_TRACE("hdl:%x astream->cur_state:%d", hdl, astream->cur_state);
	return ret;
}

/**
 * astream_start - start audio stream.
 *
 * @hdl: audio stream handle
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int astream_start(astream_handle hdl)
{
	int index;
	int ret = 0;
	audio_stream_t *astream = g_audio_rec;

	OSCL_TRACE("hdl:%x", hdl);
	oscl_param_check(IS_ASTREAM_HDL_VALID(hdl), -1, NULL);
	oscl_param_check(astream != NULL, -1, NULL);

	pthread_mutex_lock(&astream->lock);
	index = _get_index(hdl);
	oscl_param_check_exit(astream->state[index] == REC_STATE_PREPARED, 0, NULL);

	/* enable encoder port of video splitter */
	al_component_sendcom(&astream->aenc_splt.al_comp, OMX_CommandPortEnable,
		astream->aenc_splt_out[index]->index, NULL);

	astream->state[index] = REC_STATE_RECORD;
	if (astream->cur_state == REC_STATE_RECORD)
		goto EXIT;

	/* change to executing state for recorder components*/
	al_component_setstate(&astream->aenc_splt.al_comp, OMX_StateExecuting);
	al_component_setstate(&astream->aenc_info.al_comp, OMX_StateExecuting);
	al_component_setstate(&astream->asrc_info.al_comp, OMX_StateExecuting);
	_update_state(astream);

EXIT:
	OSCL_TRACE("hdl:%x astream->cur_state:%d", hdl, astream->cur_state);
	pthread_mutex_unlock(&astream->lock);
	return ret;
}

/**
 * astream_stop - stop audio stream.
 *
 * @hdl: audio stream handle
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int astream_stop(astream_handle hdl)
{
	audio_stream_t *astream = g_audio_rec;
	int ret;
	OSCL_TRACE("hdl:%x", hdl);
	oscl_param_check(IS_ASTREAM_HDL_VALID(hdl), -1, NULL);
	oscl_param_check(astream != NULL, -1, NULL);

	pthread_mutex_lock(&astream->lock);
	ret = _astream_stop(hdl);
	pthread_mutex_unlock(&astream->lock);
	OSCL_TRACE("hdl:%x astream->cur_state:%d", hdl, astream->cur_state);
	return ret;
}

