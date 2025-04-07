#ifndef DEMUXER_MEDIA_H
#define DEMUXER_MEDIA_H

typedef struct {
	int codec_id;
	int bit_rate;
	int sample_rate;
	int channels;
	int bits_per_coded_sample;
	int frame_size;
	int block_align;
	void *extradata;
	unsigned int extradata_size;
} demuxer_audio_t;

typedef struct {
	int codec_id;
	int width;
	int height;
	void *extradata;
	unsigned int extradata_size;
} demuxer_video_t;

typedef struct {
	int duration;
	int audio_streams;
	int video_streams;
	demuxer_audio_t demuxer_audio;
	demuxer_video_t demuxer_video;
} media_info_t;

#endif /* DEMUXER_MEDIA_H */
