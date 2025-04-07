/*
 * demuxer.c - Standard functionality for demuxer.
 *
 * Copyright (C) 2016-2019, LomboTech Co.Ltd.
 * Author: lomboswer <lomboswer@lombotech.com>
 */
#define DBG_LEVEL         DBG_WARNING

#include "oscl.h"
#include "av_media_type.h"
#include "demuxer_plugin.h"
#include "iostream_plugin.h"
#include "media_probe.h"
#include "demuxer.h"
#include "demuxer_component.h"

/* #define DBG_SAVE_PACKET */

#define DMX_VIDEO_BUFFER_SIZE		(2*1024*1024)
#define DMX_VIDEO_BUFFER_ALIGN		(1)
#define DMX_AUDIO_BUFFER_SIZE		(128*1024)
#define DMX_AUDIO_BUFFER_ALIGN		(1)
#define DMX_SUBTITLE_BUFFER_SIZE	(128*1024)
#define DMX_SUBTITLE_BUFFER_ALIGN	(1)

#define DMX_VIDEO_ERR_EOS		(1 << 0)
#define DMX_AUDIO_ERR_EOS		(1 << 1)

typedef struct demuxer_private {
	void *parser;
	void *ioctx;
	dmx_plugin_t *plugin;
	iostream_plugin_t *ioplugin;
	int aindex;
	int vindex;

	dmx_mediainfo_t dmx_mi;
	dmx_packet_t packet;
	int pkt_is_valid;
	int pkt_data_ofs;

	uint32_t err_eosflag;

#ifdef DBG_SAVE_PACKET
	int fd;
#endif
} demuxer_private_t;

dmx_plugin_t *mov_demuxer_plugin_info(void);
dmx_plugin_t *ts_demuxer_plugin_info(void);
iostream_plugin_t *iostream_plugin_info(void);

void *demuxer_open(char *name, media_info_t *mediainfo)
{
	demuxer_private_t *priv = NULL;
	dmx_buffer_info_t bufinfo;
	char *suffix;

	if (name == NULL || mediainfo == NULL) {
		OSCL_LOGE("invalid parameter! name=%p, mediainfo=%p", name, mediainfo);
		return NULL;
	}

	priv = oscl_zalloc(sizeof(demuxer_private_t));
	if (priv == NULL) {
		OSCL_LOGE("alloc demuxer priv error!");
		return NULL;
	}

	suffix = media_probe(name);
	if (suffix == NULL) {
		OSCL_LOGE("probe error, file not support!");
		goto ERR_EXIT;
	}

	if (!strcmp(suffix, "mov")) {
		OSCL_LOGI("probe result: mov");
		priv->plugin = mov_demuxer_plugin_info();
	} else if (!strcmp(suffix, "ts")) {
		OSCL_LOGI("probe result: ts");
		priv->plugin = ts_demuxer_plugin_info();
	} else {
		OSCL_LOGI("file %s not support yet", suffix);
		priv->plugin = NULL;
	}
	if (priv->plugin == NULL) {
		OSCL_LOGE("demuxer plugin is NULL!");
		goto ERR_EXIT;
	}

	priv->ioplugin = iostream_plugin_info();
	if (priv->ioplugin == NULL) {
		OSCL_LOGE("iostream plugin is NULL!");
		goto ERR_EXIT;
	}

	priv->ioctx = priv->ioplugin->create(name, STREAM_TYPE_FILE);
	if (priv->ioctx == NULL) {
		OSCL_LOGE("create iostream error!");
		goto ERR_EXIT;
	}
	bufinfo.video_buf_size = DMX_VIDEO_BUFFER_SIZE;
	bufinfo.video_buf_align = DMX_VIDEO_BUFFER_ALIGN;
	bufinfo.audio_buf_size = DMX_AUDIO_BUFFER_SIZE;
	bufinfo.audio_buf_align = DMX_AUDIO_BUFFER_ALIGN;
	bufinfo.sub_buf_size = DMX_SUBTITLE_BUFFER_SIZE;
	bufinfo.sub_buf_align = DMX_SUBTITLE_BUFFER_ALIGN;

	priv->dmx_mi.duration = mediainfo->duration;
	priv->parser = priv->plugin->read_header(priv->ioctx, &bufinfo, &priv->dmx_mi);
	if (priv->parser == NULL) {
		OSCL_LOGE("read header error!");
		goto ERR_EXIT;
	}
	if (priv->dmx_mi.astream_num > 0) {
		dmx_ainfo_t *ainfo = &priv->dmx_mi.ainfo[0];
		priv->aindex = priv->dmx_mi.ainfo[0].stream_index;
		mediainfo->demuxer_audio.codec_id = ainfo->codec_id;
		mediainfo->demuxer_audio.channels = ainfo->channels;
		mediainfo->demuxer_audio.sample_rate = ainfo->sample_rate;
		mediainfo->demuxer_audio.bit_rate = ainfo->bit_rate;
		mediainfo->demuxer_audio.frame_size = ainfo->frame_size;
		mediainfo->demuxer_audio.block_align = ainfo->block_align;
		mediainfo->demuxer_audio.bits_per_coded_sample =
			ainfo->bits_per_coded_sample;
		mediainfo->demuxer_audio.extradata = ainfo->extradata;
		mediainfo->demuxer_audio.extradata_size = ainfo->extradata_size;
		mediainfo->audio_streams = priv->dmx_mi.astream_num;
		OSCL_LOGI("audio codec %d, idx %d, ch %d, sr %d, bit %d, stream num %d",
			ainfo->codec_id, priv->aindex, ainfo->channels,
			ainfo->sample_rate, ainfo->bits_per_coded_sample,
			priv->dmx_mi.astream_num);
		OSCL_LOGI("audio codec extradata=%p, extrasize=%d",
			ainfo->extradata, ainfo->extradata_size);
	} else {
		OSCL_LOGW("no audio stream");
		priv->aindex = -1;
	}
	if (priv->dmx_mi.vstream_num > 0) {
		dmx_vinfo_t *vinfo = &priv->dmx_mi.vinfo[0];
		priv->vindex = priv->dmx_mi.vinfo[0].stream_index;
		mediainfo->demuxer_video.codec_id = vinfo->codec_id;
		mediainfo->demuxer_video.width = vinfo->width;
		mediainfo->demuxer_video.height = vinfo->height;
		mediainfo->demuxer_video.extradata = vinfo->extradata;
		mediainfo->demuxer_video.extradata_size = vinfo->extradata_size;
		mediainfo->video_streams = priv->dmx_mi.vstream_num;
		OSCL_LOGI("video codec %d, idx %d, size %dx%d, stream num %d",
			vinfo->codec_id, priv->vindex, vinfo->width, vinfo->height,
			priv->dmx_mi.vstream_num);
		OSCL_LOGI("video codec extradata=%p, extrasize=%d",
			vinfo->extradata, vinfo->extradata_size);
	} else {
		OSCL_LOGW("no video stream");
		priv->vindex = -1;
	}
	if (priv->vindex == -1 && priv->aindex == -1)
		goto ERR_EXIT;

	mediainfo->duration = priv->dmx_mi.duration / 1000;
	OSCL_LOGI("duration %d ms", mediainfo->duration);

#ifdef DBG_SAVE_PACKET
	priv->fd = open("/mnt/sdcard/packets.dat", O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (priv->fd < 0)
		OSCL_LOGE("open file error(%s)!", strerror(errno));
#endif

	return priv;

ERR_EXIT:
	if (priv->ioctx != NULL)
		priv->ioplugin->destroy(priv->ioctx);
	if (priv != NULL)
		oscl_free(priv);
	return NULL;
}

int demuxer_dispose(void **handle)
{
	demuxer_private_t *priv = (demuxer_private_t *)(*handle);

	if (priv == NULL) {
		OSCL_LOGE("invalid parameter!");
		return -1;
	}

#ifdef DBG_SAVE_PACKET
	if (priv->fd >= 0)
		close(priv->fd);
#endif
	priv->plugin->read_close(priv->parser);
	if (priv->ioctx != NULL)
		priv->ioplugin->destroy(priv->ioctx);
	oscl_free(priv);
	priv = NULL;
	*handle = NULL;

	return 0;
}

int demuxer_frame(void *handle, packet_t *pkt)
{
	demuxer_private_t *priv = (demuxer_private_t *)handle;
	int ret = -1;

	if (priv == NULL) {
		OSCL_LOGE("handle = NULL!");
		return -1;
	}
	if ((priv->aindex < 0) && (priv->vindex < 0)) {
		OSCL_LOGE("no audio and video data!");
		return -1;
	}
	if (priv->pkt_is_valid) {
		priv->plugin->return_packet(priv->parser, &priv->packet);
		priv->pkt_is_valid = 0;
		priv->pkt_data_ofs = 0;
	}

	while (priv->pkt_is_valid == 0) {
		memset(&priv->packet, 0, sizeof(priv->packet));
		ret = priv->plugin->read_packet(priv->parser, &priv->packet);
		if (ret < 0) {
			OSCL_LOGI("reach eos");
			if (!(priv->err_eosflag & DMX_AUDIO_ERR_EOS)) {
				priv->err_eosflag |= DMX_AUDIO_ERR_EOS;
				priv->packet.stream_index = priv->aindex;
				priv->packet.flags |= AV_PACKET_FLAG_EOS;
			} else {
				priv->err_eosflag |= DMX_VIDEO_ERR_EOS;
				priv->packet.stream_index = priv->vindex;
				priv->packet.flags |= AV_PACKET_FLAG_EOS;
			}
			break;
		}
		if (priv->packet.stream_index == priv->aindex ||
			priv->packet.stream_index == priv->vindex) {
			priv->pkt_is_valid = 1;
			priv->pkt_data_ofs = 0;
		} else {
			OSCL_LOGE("unsupport stream index %d!",
				priv->packet.stream_index);
			priv->plugin->return_packet(priv->parser, &priv->packet);
		}

#ifdef DBG_SAVE_PACKET
		if (priv->packet.stream_index == priv->vindex && priv->fd >= 0) {
			ret = write(priv->fd, priv->packet.vir_addr,
				priv->packet.data_size);
			if (ret != priv->packet.data_size) {
				OSCL_LOGE("write %d bytes error(%s)!",
					priv->packet.data_size, strerror(errno));
			}
		}
#endif
	}

	pkt->size = priv->packet.data_size;
	pkt->stream_index = priv->packet.stream_index;
	pkt->pts = priv->packet.timestamp;
	if (priv->packet.flags & AV_PACKET_FLAG_EOS) {
		OSCL_LOGI("%d stream detect eos", pkt->stream_index);
		pkt->flags |= OMX_BUFFERFLAG_EOS;
	}
	if (priv->packet.flags & AV_PACKET_FLAG_RESET) {
		OSCL_LOGI("%d stream detect starttime", pkt->stream_index);
		pkt->flags |= OMX_BUFFERFLAG_STARTTIME;
	}
	if (priv->packet.stream_index == priv->aindex) {
		OSCL_LOGD("audio packet size=%d, ts=%lld",
			priv->packet.data_size, priv->packet.timestamp);
		ret = AUDIO_PORT;
	} else if (priv->packet.stream_index == priv->vindex) {
		OSCL_LOGD("video packet size=%d, ts=%lld",
			priv->packet.data_size, priv->packet.timestamp);
		ret = VIDEO_PORT;
	} else {
		OSCL_LOGE("unsupport stream index %d!",
			priv->packet.stream_index);
		ret = -1;
	}

	return ret;
}

int demuxer_read(void *handle, packet_t *pkt)
{
	demuxer_private_t *priv = (demuxer_private_t *)handle;
	int cpsz;

	if (priv == NULL) {
		OSCL_LOGE("handle = NULL!");
		return -1;
	}

	if (priv->err_eosflag) {
		OSCL_LOGI("err_eosflag %d!", priv->err_eosflag);
		return 0;
	}

	if (priv->pkt_is_valid == 0) {
		OSCL_LOGW("demuxer_frame first!");
		return 0;
	}

	if (priv->packet.data_size <= priv->pkt_data_ofs) {
		OSCL_LOGW("no data left in packet, need demuxer_frame first!");
		return 0;
	}

	if (pkt->size > priv->packet.data_size - priv->pkt_data_ofs)
		cpsz = priv->packet.data_size - priv->pkt_data_ofs;
	else
		cpsz = pkt->size;

	memcpy(pkt->data, priv->packet.vir_addr + priv->pkt_data_ofs, cpsz);
	priv->pkt_data_ofs += cpsz;
	if (priv->pkt_data_ofs >= priv->packet.data_size) {
		priv->pkt_is_valid = 0;
		priv->pkt_data_ofs = 0;
		priv->plugin->return_packet(priv->parser, &priv->packet);
	}
	OSCL_LOGD("read size %d", cpsz);

	return 0;
}

int demuxer_seek(void *handle, int64_t timestamp)
{
	demuxer_private_t *priv = (demuxer_private_t *)handle;
	int ret;

	if (priv == NULL) {
		OSCL_LOGE("handle = NULL!");
		return -1;
	}

	if (timestamp < 0)
		timestamp = 0;
	timestamp *= 1000;

	if (priv->pkt_is_valid) {
		priv->pkt_is_valid = 0;
		priv->pkt_data_ofs = 0;
		priv->plugin->return_packet(priv->parser, &priv->packet);
	}

	OSCL_LOGI("seek time:%lld", timestamp);
	ret = priv->plugin->read_seek(priv->parser, timestamp);
	if (ret < 0) {
		OSCL_LOGE("read_seek error, ret=%d!", ret);
		return ret;
	}

	return 0;
}

