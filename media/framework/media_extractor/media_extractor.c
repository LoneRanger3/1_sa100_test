#define DBG_LEVEL         DBG_INFO

#include <oscl.h>
#include "media_extractor.h"
#include "demuxer_media.h"
#include "h264_decoder_api.h"
#include "demuxer_component.h"

void *mextractor_open(char *file)
{
	media_info_t media_info;
	media_extractor_t *handle = NULL;

	handle = oscl_zalloc(sizeof(*handle));
	if (!handle) {
		OSCL_LOGE("malloc extractor err!\n");
		return NULL;
	}

	media_info.duration = 1; /* skip over parsing duration */

	handle->demuxer = demuxer_open(file, &media_info);
	if (handle->demuxer == NULL) {
		OSCL_LOGE("open demuxer err!\n");
		oscl_free(handle);
		return NULL;
	}
	handle->formats = oscl_zalloc(sizeof(media_format_t));
	if (!handle->formats) {
		OSCL_LOGE("malloc handle->formats err!\n");
		oscl_free(handle);
		return NULL;
	}
	if (media_info.audio_streams) {
		handle->formats->bits_per_sample =
			media_info.demuxer_audio.bits_per_coded_sample;
		handle->formats->channels = media_info.demuxer_audio.channels;
		handle->formats->sample_rate = media_info.demuxer_audio.sample_rate;
	}
	if (media_info.video_streams) {
		handle->formats->width = media_info.demuxer_video.width;
		handle->formats->height = media_info.demuxer_video.height;
		if (media_info.demuxer_video.extradata) {
			handle->video_extradata =
				oscl_zalloc(media_info.demuxer_video.extradata_size);
			handle->video_extra_size =
				media_info.demuxer_video.extradata_size;
			memcpy(handle->video_extradata,
				media_info.demuxer_video.extradata,
				media_info.demuxer_video.extradata_size);
		}
	}
	if (media_info.video_streams <= 0)
		return NULL;
	return handle;
}
RTM_EXPORT(mextractor_open);

void mextractor_close(void *handle)
{
	media_extractor_t *extractor;
	if (handle == NULL) {
		OSCL_LOGE("handle null\n");
		return;
	}

	extractor = (media_extractor_t *)handle;
	demuxer_dispose(&extractor->demuxer);
	oscl_free(extractor->formats);
	if (extractor->video_extradata)
		oscl_free(extractor->video_extradata);
	oscl_free(extractor);
}
RTM_EXPORT(mextractor_close);

media_format_t *mextractor_get_format(void *handle)
{
	media_extractor_t *extractor;
	if (handle == NULL) {
		OSCL_LOGE("handle null\n");
		return NULL;
	}
	extractor = (media_extractor_t *)handle;
	return extractor->formats;
}
RTM_EXPORT(mextractor_get_format);

/* buffer must be allocated before call this func */
int mextractor_get_thumbnail(void *handle, unsigned char *buffer)
{
	int ret;
	media_extractor_t *extractor;
	void *decoder = NULL;
	frame_info_t info;
	dec_open_args_t args;
	packet_t packet;
	if (handle == NULL || buffer == NULL) {
		OSCL_LOGE("params err\n");
		return -1;
	}

	extractor = (media_extractor_t *)handle;
	args.frame_buf_cnt = 1;
	args.ouput_type = VDC_YUV420SP;
	decoder = h264_dec_open(&args);
	if (!decoder) {
		OSCL_LOGE("open decoder err!");
		return -1;
	}

	memset(&packet, 0, sizeof(packet));
	while (1) {
		int port_index;
		port_index = demuxer_frame(extractor->demuxer, &packet);
		if (port_index == VIDEO_PORT)
			break;
		else
			oscl_mdelay(10);
	}
	OSCL_LOGI("packet size %d\n", packet.size);
	packet.data = oscl_zalloc(packet.size);
	if (packet.data == NULL) {
		OSCL_LOGE("malloc packet data err\n");
		return -1;
	}
	demuxer_read(extractor->demuxer, &packet);

	ret = h264_dec_header(decoder, extractor->video_extradata,
				extractor->video_extra_size, &info);

	ret = h264_dec_keyframe(decoder, packet.data, packet.size, buffer);
	oscl_free(packet.data);
	h264_dec_close(decoder);
	return ret;
}
RTM_EXPORT(mextractor_get_thumbnail);

