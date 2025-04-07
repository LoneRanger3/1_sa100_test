/*
 * mod_player.c - mod player interface code from file explorer
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

#include <app_manage.h>
#include <mod_manage.h>
#include <stdio.h>
#include <stdlib.h>
#include <rtthread.h>
#include "player.h"
#include "mod_media.h"
#include "system/system.h"
#include "lb_gal_common.h"
#include "case_config.h"
#include "omx_mediaplayer.h"
static mod_t g_mod_media;


/* load media module */
int mod_media_load(void)
{
	s32 ret = 0;

	memset(&g_mod_media, 0, sizeof(mod_t));
	ret = lb_mod_open(&g_mod_media, ROOTFS_MOUNT_PATH"/mod/mod_media.mod", 0);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

exit:
	return ret;
}

/* show photo first, once*/
int mod_media_photo_show_start(void)
{
	s32 ret = 0;
	struct media_photo_show_start_param param;
	lb_disp_info_t lb_disp_info;

	if (g_mod_media.mh == 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}
	lb_get_disp_info(&lb_disp_info);
	APP_LOG_W("W:%d H:%d R:%d\n", lb_disp_info.height, lb_disp_info.width,
		lb_disp_info.rot);
	if (lb_disp_info.width == 1280 && lb_disp_info.height == 320) {
		param.win_offset_x = 352;//240
		param.win_offset_y = 0;
		param.win_w = lb_disp_info.width - 2 * param.win_offset_x;
		param.win_h = lb_disp_info.height - param.win_offset_y;
	} else if (lb_disp_info.width == 1920 && lb_disp_info.height == 440) {
		param.win_offset_x = 410;
		param.win_offset_y = 0;
		param.win_w = lb_disp_info.width - 2 * param.win_offset_x;
		param.win_h = lb_disp_info.height - param.win_offset_y;
	} else {
		param.win_offset_x = 0;
		param.win_offset_y = 70;
		param.win_w = lb_disp_info.width - 2 * param.win_offset_x;
		param.win_h = lb_disp_info.height - param.win_offset_y;
	}
	param.way = lb_disp_info.rot;
	param.mode = MEDIA_PHOTO_SHOW_STRETCH;

	ret = lb_mod_ctrl(&g_mod_media, MOD_MEDIA_PHOTO_SHOW_START, 0, &param);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

exit:
	return ret;
}

/* you can show your photos */
int mod_media_photo_show(char *path)
{
	s32 ret = 0;

	if (g_mod_media.mh == 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = lb_mod_ctrl(&g_mod_media, MOD_MEDIA_PHOTO_SHOW_DIRECT, 0, path);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

exit:
	return ret;
}

/* show photo last, once*/
int mod_media_photo_show_end(void)
{
	s32 ret = 0;

	if (g_mod_media.mh == 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = lb_mod_ctrl(&g_mod_media, MOD_MEDIA_PHOTO_SHOW_END, 0, 0);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

exit:
	return ret;
}

/* video set win level */
int mod_media_photo_set_win_level(int aux)
{
	s32 ret = 0;

	if (g_mod_media.mh == 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = lb_mod_ctrl(&g_mod_media, MOD_MEDIA_PHOTO_SET_WIN_LEVEL, aux, 0);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

exit:
	return ret;
}

#if 0
/* get thumb of photo at any time  */
int mod_media_photo_get_thumb(void)
{
	s32 ret = 0;
	struct media_photo_thumb_param temp;

	if (g_mod_media.mh == 0)
		return -1;

	strcpy(temp.file_path, "/mnt/sdcard/out/demo.jpg");
	temp.thumb_w = 320;
	temp.thumb_h = 240;
	temp.thumb_buf = rt_malloc_align(temp.thumb_w * temp.thumb_h * 4, 32);
	temp.way = MEDIA_PHOTO_ROTATION_90;

	ret = lb_mod_ctrl(&g_mod_media, MOD_MEDIA_PHOTO_GET_THUMB, 0, &temp);

	rt_free_align(temp.thumb_buf);

	return ret;
}
#endif

/* get thumb of photo at any time for lvgl
 * @file_path: image path
 * @thumb_w: user need thumb width
 * @thumb_h: user need thumb height
 *
 * success return lvgl image buffer, fail return NULL
 * */
char *mod_media_photo_get_thumb
(char *file_path, int thumb_w, int thumb_h)
{
	s32 ret = 0;
	struct media_photo_thumb_param temp;

	strcpy(temp.file_path, file_path);
	temp.thumb_w = thumb_w;
	temp.thumb_h = thumb_h;
	/* ARGB8888 buffer */
	temp.thumb_buf = rt_malloc_align(temp.thumb_w * temp.thumb_h * 4 + 32, 32);
	temp.way = MEDIA_PHOTO_ROTATION_NONE;

	if (temp.thumb_buf == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	strcpy((char *)temp.thumb_buf, "thumb buffer");
	((u32 *) temp.thumb_buf)[7] = ((5 << 0) | (0 << 5) | (0 << 8)
			| (temp.thumb_w << 10) | (temp.thumb_h << 21));

	temp.thumb_buf = temp.thumb_buf + 32;
	ret = lb_mod_ctrl(&g_mod_media, MOD_MEDIA_PHOTO_GET_THUMB, 0, &temp);
	temp.thumb_buf = temp.thumb_buf - 4;

	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

exit:
	if (ret == -1) {
		if (temp.thumb_buf) {
			rt_free_align((char *)temp.thumb_buf - 28);
			temp.thumb_buf = NULL;
		}

		return NULL;
	}

	return (char *)temp.thumb_buf;
}

/* mars_mem_free thumb of photo buffer
 * @thumb_buf: thumb buffer
 *
 * success return 0, fail return -1
 **/
int mod_media_photo_free_thumb(char *thumb_buf)
{
	s32 ret = 0;

	if (thumb_buf) {
		if (strcmp(thumb_buf - 28, "thumb buffer") != 0) {
			APP_LOG_W("failed\n");
			ret = -1;
			goto exit;
		}

		rt_free_align(thumb_buf - 28);
	}

exit:
	return ret;
}

/* get thumb of video at any time for lvgl
 * @file_path: image path
 * @thumb_w: user need thumb width
 * @thumb_h: user need thumb height
 *
 * success return lvgl image buffer, fail return NULL
 * */
char *mod_media_video_get_thumb
(char *file_path, int thumb_w, int thumb_h)
{
	s32 ret = 0;
	struct media_video_thumb_param temp;

	strcpy(temp.file_path, file_path);
	temp.thumb_w = thumb_w;
	temp.thumb_h = thumb_h;
	/* ARGB8888 buffer */
	temp.thumb_buf = rt_malloc_align(temp.thumb_w * temp.thumb_h * 4 + 32, 32);
	temp.way = MEDIA_VIDEO_ROTATION_NONE;

	if (temp.thumb_buf == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	strcpy((char *)temp.thumb_buf, "thumb buffer");
	((u32 *) temp.thumb_buf)[7] = ((5 << 0) | (0 << 5) | (0 << 8)
			| (temp.thumb_w << 10) | (temp.thumb_h << 21));

	temp.thumb_buf = temp.thumb_buf + 32;
	ret = lb_mod_ctrl(&g_mod_media, MOD_MEDIA_VIDEO_GET_THUMB, 0, &temp);
	temp.thumb_buf = temp.thumb_buf - 4;

	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

exit:
	if (ret == -1) {
		if (temp.thumb_buf) {
			rt_free_align((char *)temp.thumb_buf - 28);
			temp.thumb_buf = NULL;
		}

		return NULL;
	}

	return (char *)temp.thumb_buf;
}

/* mars_mem_free thumb of photo buffer
 * @thumb_buf: thumb buffer
 *
 * success return 0, fail return -1
 **/
int mod_media_video_free_thumb(char *thumb_buf)
{
	s32 ret = 0;

	if (thumb_buf) {
		if (strcmp(thumb_buf - 28, "thumb buffer") != 0) {
			APP_LOG_W("failed\n");
			ret = -1;
			goto exit;
		}

		rt_free_align(thumb_buf - 28);
	}

exit:
	return ret;
}


/* play video first, once*/
int mod_media_video_start(void)
{
	s32 ret = 0;
	struct media_video_start_param param;
	lb_disp_info_t lb_disp_info;

	if (g_mod_media.mh == 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	lb_get_disp_info(&lb_disp_info);
	APP_LOG_W("W:%d H:%d R:%d\n", lb_disp_info.height, lb_disp_info.width,
		lb_disp_info.rot);
	if (lb_disp_info.width == 1280 && lb_disp_info.height == 320) {
		param.win_offset_x = 240;
		param.win_offset_y = 0;
		param.win_w = lb_disp_info.width - 2 * param.win_offset_x;
		param.win_h = lb_disp_info.height - param.win_offset_y;
	} else if (lb_disp_info.width == 1920 && lb_disp_info.height == 440) {
		param.win_offset_x = 410;
		param.win_offset_y = 0;
		param.win_w = lb_disp_info.width - 2 * param.win_offset_x;
		param.win_h = lb_disp_info.height - param.win_offset_y;
	} else {
		param.win_offset_x = 0;
		param.win_offset_y = 70;
		param.win_w = lb_disp_info.width - 2 * param.win_offset_x;
		param.win_h = lb_disp_info.height - param.win_offset_y;
	}
	param.way = lb_disp_info.rot;
	param.mode = OMXMP_WINDOW_USERDEF;
	ret = lb_mod_ctrl(&g_mod_media, MOD_MEDIA_VIDEO_START, 0, &param);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

exit:
	return ret;
}

/* play video step 1 */
int mod_media_video_set_path(char *video_path)
{
	s32 ret = 0;

	if (g_mod_media.mh == 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	if (video_path == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = lb_mod_ctrl(&g_mod_media, MOD_MEDIA_VIDEO_SET_FILE, 0, video_path);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

exit:
	return ret;

}

/* play video step 2 */
int mod_media_video_get_state(void)
{
	s32 ret = 0;

	if (g_mod_media.mh == 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = lb_mod_ctrl(&g_mod_media, MOD_MEDIA_VIDEO_GET_STATE, 0, 0);

exit:
	return ret;
}

/* play video step 2 */
int mod_media_video_play(void)
{
	s32 ret = 0;

	if (g_mod_media.mh == 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = lb_mod_ctrl(&g_mod_media, MOD_MEDIA_VIDEO_PLAY, 0, 0);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

exit:
	return ret;
}

/* stop video step 2 */
int mod_media_video_stop(void)
{
	s32 ret = 0;

	if (g_mod_media.mh == 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = lb_mod_ctrl(&g_mod_media, MOD_MEDIA_VIDEO_STOP, 0, 0);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

exit:
	return ret;
}

/* stop video step 2 */
int mod_media_video_pause(void)
{
	s32 ret = 0;

	if (g_mod_media.mh == 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = lb_mod_ctrl(&g_mod_media, MOD_MEDIA_VIDEO_PAUSE, 0, 0);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

exit:
	return ret;
}

/* video set win level */
int mod_media_video_set_win_level(int aux)
{
	s32 ret = 0;

	if (g_mod_media.mh == 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = lb_mod_ctrl(&g_mod_media, MOD_MEDIA_VIDEO_SET_WIN_LEVEL, aux, 0);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

exit:
	return ret;
}

/* set cur pos */
int mod_media_set_cur_pos(int cur_pos)
{
	s32 ret = 0;

	if (g_mod_media.mh == 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = lb_mod_ctrl(&g_mod_media, MOD_MEDIA_VIDEO_SET_CUR_POS, cur_pos, 0);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

exit:
	return ret;
}

/* get cur pos */
int mod_media_get_cur_pos(void)
{
	s32 ret = 0;

	if (g_mod_media.mh == 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = lb_mod_ctrl(&g_mod_media, MOD_MEDIA_VIDEO_GET_CUR_POS, 0, 0);

exit:
	return ret;
}

/* get end pos */
int mod_media_get_end_pos(void)
{
	s32 ret = 0;

	if (g_mod_media.mh == 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = lb_mod_ctrl(&g_mod_media, MOD_MEDIA_VIDEO_GET_END_POS, 0, 0);

exit:
	return ret;
}

/* play video last, once*/
int mod_media_video_end(void)
{
	s32 ret = 0;

	if (g_mod_media.mh == 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = lb_mod_ctrl(&g_mod_media, MOD_MEDIA_VIDEO_END, 0, 0);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

exit:
	return ret;
}

/* unload media module */
int mod_media_unload(void)
{
	s32 ret = 0;

	if (g_mod_media.mh == 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = lb_mod_close(&g_mod_media);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

exit:
	return ret;
}
