/*
* copyright (c) 2018 All Rights Reserved
*
* This file is part of audio.
*
* File   : audio_common.h
* Version: V1.0
* Date   : 2018/8/21 17:16:50
*/

#ifndef	AUDIO_COMMON_H
#define AUDIO_COMMON_H

#include <stdint.h>
#include "av_media_type.h"

/*
 * Rational number (pair of numerator and denominator).
 */
typedef struct AV_Rational {
	int num;/* < Numerator */
	int den;/* < Denominator */
} AV_Rational;

typedef struct AV_CodecParameters {
	/**
	* General type of the encoded data.
	*/
	enum av_media_type codec_type;
	/**
	* Specific type of the encoded data (the codec used).
	*/
	enum av_codec_type   codec_id;
	/**
	* Additional information about the codec (corresponds to the AVI FOURCC).
	*/
	uint32_t         codec_tag;

	/**
	* Extra binary data needed for initializing the decoder, codec-dependent.
	*
	* Must be allocated with av_malloc() and will be freed by
	* avcodec_parameters_free(). The allocated size of extradata must be at
	* least extradata_size + AV_INPUT_BUFFER_PADDING_SIZE, with the padding
	* bytes zeroed.
	*/
	uint8_t *extradata;
	/**
	* Size of the extradata content in bytes.
	*/
	int      extradata_size;

	/**
	* - video: the pixel format, the value corresponds to enum AVPixelFormat.
	* - audio: the sample format, the value corresponds to enum AVSampleFormat.
	*/
	int format;

	/**
	* The average bitrate of the encoded data (in bits per second).
	*/
	int64_t bit_rate;

	/**
	* AV_CODEC_FLAG_*.
	* - encoding: Set by user.
	* - decoding: Set by user.
	*/
	int flags;

	/**
	* The number of bits per sample in the codedwords.
	*
	* This is basically the bitrate per sample. It is mandatory for a bunch of
	* formats to actually decode them. It's the number of bits for one sample in
	* the actual coded bitstream.
	*
	* This could be for example 4 for ADPCM
	* For PCM formats this matches bits_per_raw_sample
	* Can be 0
	*/
	int bits_per_coded_sample;

	/**
	* This is the number of valid bits in each output sample. If the
	* sample format has more bits, the least significant bits are additional
	* padding bits, which are always 0. Use right shifts to reduce the sample
	* to its actual size. For example, audio formats with 24 bit samples will
	* have bits_per_raw_sample set to 24, and format set to AV_SAMPLE_FMT_S32.
	* To get the original sample use "(int32_t)sample >> 8"."
	*
	* For ADPCM this might be 12 or 16 or similar
	* Can be 0
	*/
	int bits_per_raw_sample;

	/**
	* Codec-specific bitstream restrictions that the stream conforms to.
	*/
	int profile;
	int level;

	/**
	* Video only. The dimensions of the video frame in pixels.
	*/
	int width;
	int height;

	/**
	* Video only. The aspect ratio (width / height) which a single pixel
	* should have when displayed.
	*
	* When the aspect ratio is unknown / undefined, the numerator should be
	* set to 0 (the denominator may have any value).
	*/
	AV_Rational sample_aspect_ratio;



	/**
	* Video only. Number of delayed frames.
	*/
	int video_delay;

	/**
	* Audio only. The channel layout bitmask. May be 0 if the channel layout is
	* unknown or unspecified, otherwise the number of bits set must be equal to
	* the channels field.
	*/
	uint64_t channel_layout;
	/**
	* Audio only. The number of audio channels.
	*/
	int      channels;
	/**
	* Audio only. The number of audio samples per second.
	*/
	int      sample_rate;
	/**
	* Audio only. The number of bytes per coded audio frame, required by some
	* formats.
	*
	* Corresponds to nBlockAlign in WAVEFORMATEX.
	*/
	int      block_align;
	/**
	* Audio only. Audio frame size, if known. Required by some formats to be static.
	*/
	int      frame_size;

	/**
	* Audio only. The amount of padding (in samples) inserted by the encoder at
	* the beginning of the audio. I.e. this number of leading decoded samples
	* must be discarded by the caller to get the original audio without leading
	* padding.
	*/
	int initial_padding;
	/**
	* Audio only. The amount of padding (in samples) appended by the encoder to
	* the end of the audio. I.e. this number of decoded samples must be
	* discarded by the caller from the end of the stream to get the original
	* audio without any trailing padding.
	*/
	int trailing_padding;
	/**
	* Audio only. Number of samples to skip after a discontinuity.
	*/
	int seek_preroll;
	/**
	* Frame counter, set by libavcodec.
	*
	* - decoding: total number of frames returned from the decoder so far.
	* - encoding: total number of frames passed to the encoder so far.
	*/
	int frame_number;
} AV_CodecParameters;

typedef struct tag_packet {
	uint8_t *data;
	/* encode  input packet nb_samples */
	int		size;
	int64_t pts;
	int64_t dts;
	int64_t duration;
	int stream_index;
	/**
	* A combination of AV_PKT_FLAG values
	*/
	int flags;
	int buf_size;
	int64_t timestamp;
} packet_t;

typedef struct {
	unsigned char is_valid;
	unsigned char *pdata;
	int size;
} user_data_t;

#endif/*AUDIO_COMMON_H*/

