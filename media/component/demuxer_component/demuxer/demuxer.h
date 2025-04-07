/*
 * demuxer.h - Standard functionality for demuxer.
 *
 * Copyright (C) 2016-2019, LomboTech Co.Ltd.
 * Author: lomboswer <lomboswer@lombotech.com>
 */

#ifndef __DEMUXER_H__
#define __DEMUXER_H__

#include "audio_common.h"
#include "../demuxer_media.h"

void *demuxer_open(char *name, media_info_t *media_info);
int demuxer_dispose(void **handle);
int demuxer_frame(void *handle, packet_t *pkt);
int demuxer_read(void *handle, packet_t *pkt);
int demuxer_seek(void *handle, int64_t timestamp);

#endif /* __DEMUXER_H__ */

