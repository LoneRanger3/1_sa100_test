#ifndef __MEDIA_EXTRACTOR_H__
#define __MEDIA_EXTRACTOR_H__

typedef struct {
	int codec_id;
	int duration;
	/* for audio */
	int sample_rate;
	int channels;
	int bits_per_sample;
	int frame_size;
	/* for video */
	int width;
	int height;
} media_format_t;

typedef struct {
	void *demuxer;
	char *file_name;
	int stream_nums;
	media_format_t *formats;
	void *video_extradata;
	int video_extra_size;
} media_extractor_t;

void *mextractor_open(char *file);
void mextractor_close(void *handle);
media_format_t *mextractor_get_format(void *handle);
int mextractor_get_thumbnail(void *handle, unsigned char *buffer);

#endif /* __MEDIA_EXTRACTOR_H__ */
