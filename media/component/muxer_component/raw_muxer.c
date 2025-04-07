/*
* copyright (c) 2019 All Rights Reserved
*
* This file is part of raw muxer.
*
* File   : raw_muxer.h
* Version: V1.0
* Date   : 2019/08/20 11:04:53
* Author : gusm
*/
#define DBG_LEVEL		DBG_INFO
#define LOG_TAG			"raw_muxer"

#include <oscl.h>
#include "muxer_com.h"
#include "avformat.h"

typedef struct raw_muxer_context {
	external_stream_writer_t ext_writer;
} raw_muxer_context_t;

int raw_init(format_muxer *muxer)
{
	raw_muxer_context_t *ctx = malloc(sizeof(raw_muxer_context_t));
	if (ctx == NULL)
		return -1;

	memset(ctx, 0, sizeof(raw_muxer_context_t));
	muxer->priv = (void *)ctx;
	return 0;
}

int raw_deinit(format_muxer *muxer)
{
	raw_muxer_context_t *ctx = (raw_muxer_context_t *)(muxer->priv);

	if (ctx != NULL) {
		free(ctx);
		muxer->priv = NULL;
	}

	return 0;
}

int raw_write_header(format_muxer *muxer)
{
	return 0;
}

int raw_write_packet(format_muxer *muxer, packet_t *pin)
{
	raw_muxer_context_t *ctx = (raw_muxer_context_t *)(muxer->priv);
	int type;

	if (ctx->ext_writer.write == NULL) {
		OSCL_LOGW("external write function not set yet");
		return -1;
	}
	if (pin->stream_index == VIDEO_INDEX) {
		type = AV_MEDIA_TYPE_VIDEO;
		if ((pin->flags & AV_PKT_FLAG_KEY) &&
		    (muxer->para[VIDEO_INDEX].extradata)) {
			ctx->ext_writer.write(ctx->ext_writer.app_data,
				type,
				muxer->para[VIDEO_INDEX].extradata,
				muxer->para[VIDEO_INDEX].extradata_size,
				pin->timestamp);
		}
	} else {
		type = AV_MEDIA_TYPE_AUDIO;
	}

	return ctx->ext_writer.write(ctx->ext_writer.app_data, type, pin->data,
		pin->size, pin->timestamp);
}

int raw_write_trailer(format_muxer *muxer)
{
	return 0;
}

int raw_check_bitstream(format_muxer *muxer, packet_t *pin)
{
	return 1;
}

int raw_ctrl(format_muxer *muxer, int cmd, void *param)
{
	raw_muxer_context_t *ctx = (raw_muxer_context_t *)(muxer->priv);
	int ret = 0;

	switch (cmd) {
	case MUXER_CMD_SET_STREAM_CB:
		ctx->ext_writer = *((external_stream_writer_t *)param);
		break;
	default:
		OSCL_LOGW("unknown cmd type %d", cmd);
		break;
	}
	return ret;
}

