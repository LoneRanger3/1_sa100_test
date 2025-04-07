/*
 * omx_mediaplayer.h - Standard functionality for lombo mediaplayer.
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

#ifndef __OMX_MEDIAPLAYER_H__
#define __OMX_MEDIAPLAYER_H__

typedef enum OMXMP_WIN_LAYER {
	OMXMP_LAYER_TOP = 0,
	OMXMP_LAYER_BOTTOM,
	OMXMP_LAYER_NONE = 0x7fff
} omxmp_win_layer_t;

typedef enum omxmp_rotate {
	OMXMP_ROTATE_NONE	= 0,
	OMXMP_ROTATE_90		= 1,
	OMXMP_ROTATE_180	= 2,
	OMXMP_ROTATE_270	= 3,
} omxmp_rotate_e;

typedef enum OMXMP_DISP_MODE {
	/* Display in the window at the original size of the video,
	 * can't overflow the window */
	OMXMP_WINDOW_ORIGINAL,
	/* Scale to full screen by video ratio, the video show normal */
	OMXMP_WINDOW_FULL_SCREEN_VIDEO_RATIO,
	/* Scale to full screen by screen ratio, the video may be distorted */
	OMXMP_WINDOW_FULL_SCREEN_SCREEN_RATIO,
	/* Forced to display at 4:3 ratio, the video may be distorted */
	OMXMP_WINDOW_4R3MODE,
	/* Forced to display at 16:9 ratio, the video may be distorted */
	OMXMP_WINDOW_16R9MODE,
	/* Used to cut off the black side of the video */
	OMXMP_WINDOW_CUTEDGE,
	/* User defined mode */
	OMXMP_WINDOW_USERDEF,
} omx_disp_mode_t;

typedef struct omxmp_win {
	int   left;
	int   top;
	int   width;
	int   height;
} omxmp_win_t;

typedef enum omxmp_error {
	OMXMP_ERR_UNKNOW = 0,
	OMXMP_ERR_IO,
	OMXMP_ERR_UNSUPPORTED,
	OMXMP_ERR_TIMEOUT,
	OMXMP_ERR_FUNC,
	OMXMP_ERR_VIDEO_DEC,
	OMXMP_ERR_AUDIO_DEC,
} omxmp_error_t;

typedef struct mp_callback_ops {
	void (*on_completion)(void *handle);
	void (*on_error)(void *handle, int omx_err);
	void (*on_prepared)(void *handle);
	void (*on_seek_complete)(void *handle);
	void (*on_video_size_changed)(void *handle, int width, int height);
	void (*on_timed_text)(void *handle, char *text);
} mp_callback_ops_t;

void *omxmp_create(mp_callback_ops_t *cb_ops);

void omxmp_release(void *handle);

int omxmp_set_data_source(void *handle, const char *url);

int omxmp_prepare(void *handle);

int omxmp_start(void *handle);

int omxmp_pause(void *handle);

int omxmp_stop(void *handle);

int omxmp_reset(void *handle);

int omxmp_seek_to(void *handle, long msec);

int omxmp_get_state(void *handle);

/* return 0 if false, return 1 if true */
int omxmp_is_playing(void *handle);

/* current position of meida stream, unit: ms */
long omxmp_get_current_position(void *handle);

/* duration of meida stream, unit: ms */
long omxmp_get_duration(void *handle);

/* setloop, 0: not loop 1: loop */
void omxmp_set_loop(void *handle, int loop);

int omxmp_set_playback_rate(void *handle, int rate);

void omxmp_set_playback_volume(void *handle, int rate);

int omxmp_set_scaling_mode(void *handle, int mode);

int omxmp_set_window(void *handle, omxmp_win_t *win);

int omxmp_set_rotation(void *handle, int rot_mode);

/* 0: layer top, 1: layer bottom */
int omxmp_set_win_layer(void *handle, int layer);

#endif /* __OMX_MEDIAPLAYER_H__ */
