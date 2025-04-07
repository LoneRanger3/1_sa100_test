/*
 * omx_mediaplayer_priv.h - Standard functionality for lombo mediaplayer.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#ifndef __OMX_MEDIAPLAYER_PRIV_H__
#define __OMX_MEDIAPLAYER_PRIV_H__

#define DEFAULT_ASTREAM_BUFSIZE		(4*1024)
#define DEFAULT_ASTREAM_BUFCNT		16
#define DEFAULT_VSTREAM_BUFSIZE		(1024*1024)
#define DEFAULT_VSTREAM_BUFCNT		6
#define DEFAULT_ARAW_BUFSIZE		(8*1024)
#define DEFAULT_ARAW_BUFCNT		16
#define DEFAULT_VRAW_BUFCNT		6
#define DEFAULT_ROTATE_BUFCNT		5

typedef enum player_state {
	OMXMP_STATE_IDLE = 0,
	OMXMP_STATE_INIT,
	OMXMP_STATE_PREPARED,
	OMXMP_STATE_STARTED,
	OMXMP_STATE_PAUSED,
	OMXMP_STATE_STOP,
	OMXMP_STATE_COMPLETED,
	OMXMP_STATE_ERROR,
	OMXMP_STATE_END
} player_state_t;

typedef enum player_msg_type {
	OMXMP_MSG_COMPLETE = 0,
	OMXMP_MSG_PREPARED,
	OMXMP_MSG_SEEK_COMPLETE,
	OMXMP_MSG_ERROR,
	OMXMP_MSG_STREAM_EOS
} player_msg_type_t;

typedef struct mp_callback_ops mp_callback_ops_t;

typedef struct omx_mediaplayer {
	OMX_COMPONENTTYPE *demuxer;
	OMX_COMPONENTTYPE *audiodec;
	OMX_COMPONENTTYPE *videodec;
	OMX_COMPONENTTYPE *arender;
	OMX_COMPONENTTYPE *vrender;
	OMX_COMPONENTTYPE *rotate;
	OMX_COMPONENTTYPE *clocksrc;

	mp_callback_ops_t *callback;

	/* demuxer buffer header */
	OMX_BUFFERHEADERTYPE **outbuf_demux_video, **outbuf_demux_audio;
	/* audio decoder buffer header */
	OMX_BUFFERHEADERTYPE **outbuf_audio_dec;
	/* video decoder buffer header */
	OMX_BUFFERHEADERTYPE **outbuf_video_dec;
	/* rotate output buffer header */
	OMX_BUFFERHEADERTYPE **outbuf_rotate;

	/* audio stream buffer size & count */
	int astream_buf_size, astream_buf_cnt;
	/* video stream buffer size & count */
	int vstream_buf_size, vstream_buf_cnt;
	/* audio raw buffer size & count */
	int araw_buf_size, araw_buf_cnt;
	/* video raw buffer size & count */
	int vraw_buf_size, vraw_buf_cnt;
	/* rotate buffer size & count */
	int rotate_outbuf_size, rotate_outbuf_cnt;

	OMX_U32 bit_rate;
	OMX_U32 channels;	/* Number of channels (e.g. 2 for stereo) */
	OMX_U32 bit_per_sample;	/* Bit per sample */
	OMX_U32 sample_rate;	/* Sampling rate of the source data.
				   Use 0 for variable or unknown sampling rate. */
	OMX_AUDIO_CODINGTYPE audio_codingtype;

	OMX_U32 frame_width;
	OMX_U32 frame_height;
	OMX_U32 frame_stride;
	OMX_U32 frame_sliceheight;
	OMX_U32 rotate_width;
	OMX_U32 rotate_height;
	OMX_U32 rotate_stride;
	OMX_U32 rotate_sliceheight;
	OMX_VIDEO_CODINGTYPE video_codingtype;

	vdisp_para_t disp_para; /* display params */
	OMX_CONFIG_ROTATIONTYPE rot_mode;

	/* for msg queue */
	sem_t msg_sem;
	oscl_queue_t msg_queue;
	OMX_S32 msg_handler_thread_id;
	pthread_t msg_handler_thread;

	sem_t sem_event;
	sem_t sem_eos;
	pthread_mutex_t lock;

	int status;
	int play_rate;
	char *data_url;
	long duration; /* duration of the media stream, in msecs */
	long cur_pos; /* current position of the media stream, in msecs */
	long seek_pos; /* the position to seek */
	OMX_BOOL isloop; /* indicate if it's loop play */
	OMX_BOOL has_audio;
	OMX_BOOL has_video;
	OMX_BOOL is_seeking;
	OMX_BOOL seek_in_pause;
	OMX_BOOL audio_eos;
	OMX_BOOL video_eos;
	/* todo :... */
} omx_mediaplayer_t;

int omxmp_send_msg(omx_mediaplayer_t *player,
	player_msg_type_t cmd, OMX_U32 param1, void *msg_data);

#endif /* __OMX_MEDIAPLAYER_PRIV_H__ */
