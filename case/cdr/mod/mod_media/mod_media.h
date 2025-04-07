/*
 * mod_media.h - module media head file
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

#ifndef __MOD_MEDIA_H__
#define __MOD_MEDIA_H__

/*         #define MY_MEDIA_DEBUG_ON */
/*         #define MEDIA_PRINT_INF_ON */

enum media_photo_rotation {
	MEDIA_PHOTO_ROTATION_NONE,
	MEDIA_PHOTO_ROTATION_90,/* LOMBO_DRM_TRANSFORM_ROT_90 */
	MEDIA_PHOTO_ROTATION_180,/* LOMBO_DRM_TRANSFORM_ROT_180 */
	MEDIA_PHOTO_ROTATION_270,/* LOMBO_DRM_TRANSFORM_ROT_270 */
	MEDIA_PHOTO_ROTATION_MAX,
};

enum media_photo_show_mode {
	MEDIA_PHOTO_SHOW_CENTER,
	MEDIA_PHOTO_SHOW_STRETCH,
	MEDIA_PHOTO_SHOW_MODE_MAX,
};

enum media_video_rotation {
	MEDIA_VIDEO_ROTATION_NONE,
	MEDIA_VIDEO_ROTATION_90,
	MEDIA_VIDEO_ROTATION_180,
	MEDIA_VIDEO_ROTATION_270,
	MEDIA_VIDEO_ROTATION_MAX,
};

enum media_video_show_mode {
	MEDIA_VIDEO_SHOW_CENTER = 1,/* OMXMP_WINDOW_FULL_SCREEN_VIDEO_RATIO */
	MEDIA_VIDEO_SHOW_STRETCH = 2,/* OMXMP_WINDOW_FULL_SCREEN_SCREEN_RATIO */
	MEDIA_VIDEO_SHOW_MODE_MAX,
};

enum media_video_state {
	MEDIA_VIDEO_STATE_IDLE = 0,
	MEDIA_VIDEO_STATE_INIT,
	MEDIA_VIDEO_STATE_PREPARED,
	MEDIA_VIDEO_STATE_STARTED,/* video play */
	MEDIA_VIDEO_STATE_PAUSED,/* video pause */
	MEDIA_VIDEO_STATE_STOP,/* video stop */
	MEDIA_VIDEO_STATE_COMPLETED,/* video stop */
	MEDIA_VIDEO_STATE_ERROR,
	MEDIA_VIDEO_STATE_END
};

enum {
	MOD_MEDIA_PHOTO_SHOW_START = 1,
	MOD_MEDIA_PHOTO_SHOW_END,
	MOD_MEDIA_PHOTO_SHOW_NEXT,/* aux1 = photo of path */
	MOD_MEDIA_PHOTO_SHOW_PREV,/* aux1 = photo of path */
	MOD_MEDIA_PHOTO_SHOW_DIRECT,/* aux1 = photo of path */
	MOD_MEDIA_PHOTO_SET_WIN_LEVEL,/* set aux0 = 0 top; 1 bottom */
	MOD_MEDIA_PHOTO_SHOW_SAVE_JPG,
	MOD_MEDIA_PHOTO_SHOW_SAVE_ROT,

	MOD_MEDIA_PHOTO_SLID_READY = 100,
	MOD_MEDIA_PHOTO_SLID_OVER,
	MOD_MEDIA_PHOTO_SLID_DOWN,
	MOD_MEDIA_PHOTO_SLID_MOVE,
	MOD_MEDIA_PHOTO_SLID_UP,

	/*
	 * set aux1 = struct media_photo_thumb_param
	 * and get thumb_buf for argb buf
	 */
	MOD_MEDIA_PHOTO_GET_THUMB,

	MOD_MEDIA_VIDEO_START = 200,
	MOD_MEDIA_VIDEO_END,
	MOD_MEDIA_VIDEO_SET_FILE,/* set aux1 = video of path */
	MOD_MEDIA_VIDEO_PLAY,
	MOD_MEDIA_VIDEO_PAUSE,
	MOD_MEDIA_VIDEO_STOP,
	MOD_MEDIA_VIDEO_SET_CUR_POS,/* set aux0 = seconds */
	MOD_MEDIA_VIDEO_GET_END_POS,/* return seconds */
	MOD_MEDIA_VIDEO_GET_CUR_POS,/* return seconds */
	MOD_MEDIA_VIDEO_GET_STATE,/* return = enum media_video_state */
	MOD_MEDIA_VIDEO_SET_WIN_LEVEL,/* set aux0 = 0 top; 1 bottom */

	/*
	 * set aux1 = struct media_video_thumb_param
	 * and get thumb_buf for argb buf
	 */
	MOD_MEDIA_VIDEO_GET_THUMB,

	MOD_MEDIA_MAX
};

struct media_photo_show_start_param {
	enum media_photo_rotation way;
	enum media_photo_show_mode mode;
	int win_w;
	int win_h;
	int win_offset_x;
	int win_offset_y;
};

struct media_photo_thumb_param {
	char file_path[256];
	enum media_photo_rotation way;

	/*
	 * user rt_malloc_align(thumb_w*thumb_h*4, 32); and rt_free_align(thumb_buf);
	 * out format = ARGB8888
	 */
	unsigned char *thumb_buf;
	int thumb_w;
	int thumb_h;
};

struct media_video_start_param {
	enum media_video_rotation way;
	enum media_video_show_mode mode;
	int win_w;
	int win_h;
	int win_offset_x;
	int win_offset_y;
};

struct media_video_thumb_param {
	char file_path[256];
	enum media_video_rotation way;

	/*
	 * user rt_malloc_align(thumb_w*thumb_h*4, 32); and rt_free_align(thumb_buf);
	 * out format = ARGB8888
	 */
	unsigned char *thumb_buf;
	int thumb_w;
	int thumb_h;
};

#endif /* __MOD_MEDIA_H__ */
