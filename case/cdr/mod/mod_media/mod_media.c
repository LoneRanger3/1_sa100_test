/*
 * mod_media.c - module media for module developer
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

#include <stdio.h>
#include <stdlib.h>
#include "mod_manage.h"
#include "mod_media_i.h"
#include "media_photo.h"
#include "media_video.h"
#include "media_display.h"

#if 0
#define MD_CTR	MD_ERR
#else
#define MD_CTR(...)
#endif

static struct rt_mutex g_media_lock;

/**
 * mod_media_init - module init
 * @mp: module struct pointer.
 *
 * This function use to init a module,when lb_mod_open is called,this function is called.
 *
 * Returns 0
 */
static lb_int32 mod_media_init(mod_t *mp)
{
	MD_CTR("mod_media_init");

	rt_mutex_init(&g_media_lock, "media_lock", RT_IPC_FLAG_FIFO);

	return media_disp_init();
}

static void mod_media_lock(void)
{
	rt_mutex_take(&g_media_lock, RT_WAITING_FOREVER);
}

static void mod_media_unlock(void)
{
	rt_mutex_release(&g_media_lock);
}

/**
 * mod_media_exit - module exit
 * @mp: module struct pointer.
 *
 * This function use to exit a module,when lb_mod_close is called,
 * this function is called.
 *
 * Returns 0
 */
static lb_int32 mod_media_exit(mod_t *mp)
{
	MD_CTR("mod_media_exit");

	rt_mutex_detach(&g_media_lock);

	return media_disp_exit();
}

/**
 * mod_media_read - module read interface
 * @mp:    module struct pointer.
 * @mdata: store read data.
 * @size:  one block size.
 * @n:     block num.
 *
 * This function use run the read function of module.when lb_mod_read is called,
 * this function is called.
 *
 * Returns 0
 */
static lb_int32 mod_media_read(mod_t *mp, void *mdata, lb_uint32 size, lb_uint32 n)
{
	MD_CTR("mod_media_read: num:%d mdata:%p", size * n, mdata);

	return 0;
}

/**
 * mod_media_write - module write  interface
 * @mp:    module struct pointer.
 * @mdata: store write data.
 * @size:  one block size.
 * @n:     block num.
 *
 * This function use run the write function of module.when lb_mod_write is called,
 * this function is called.
 *
 * Returns 0
 */
static lb_int32 mod_media_write(mod_t *mp, const void *mdata, lb_uint32 size, lb_uint32 n)
{
	MD_CTR("mod_media_write: num:%d", size * n);

	return 0;
}

/**
 * mod_media_ctrl - module command interface
 * @mp:   module struct pointer.
 * @cmd:  command,defined in mod_media.h.
 * @aux0: auxiliary parameter 0.
 * @aux1: auxiliary parameter 1.
 *
 * This function use run the ctrl function of module.when lb_mod_ctrl is called,
 * this function is called.
 *
 * Returns 0
 */
static lb_int32 mod_media_ctrl(mod_t *mp, lb_uint32 cmd, lb_int32 aux0, void *aux1)
{
	lb_int32 ret = -1;

	mod_media_lock();

	switch (cmd) {
	case MOD_MEDIA_PHOTO_SHOW_START:
	{
		MD_CTR("MOD_MEDIA_PHOTO_SHOW_START, aux0:%x, aux1:(%d,%d,[%d,%d,%d,%d])" ,
			aux0,
			((struct media_photo_show_start_param *) aux1)->way,
			((struct media_photo_show_start_param *) aux1)->mode,
			((struct media_photo_show_start_param *) aux1)->win_w,
			((struct media_photo_show_start_param *) aux1)->win_h,
			((struct media_photo_show_start_param *) aux1)->win_offset_x,
			((struct media_photo_show_start_param *) aux1)->win_offset_y);
#ifdef MY_MEDIA_DEBUG_ON
		struct media_photo_show_start_param temp;

		temp.way = MEDIA_PHOTO_ROTATION_90;
		temp.mode = MEDIA_PHOTO_SHOW_STRETCH;
		temp.win_w = 400;
		temp.win_h = 100;
		temp.win_offset_x = 100;
		temp.win_offset_y = 50;

		ret = media_photo_show_start(&temp);
#else
		ret = media_photo_show_start(
				(struct media_photo_show_start_param *) aux1);
#endif
	}
		break;
	case MOD_MEDIA_PHOTO_SHOW_NEXT:
		MD_CTR("MOD_MEDIA_PHOTO_SHOW_NEXT, aux0: %x, aux1: %s", aux0,
				(char *) aux1);
		ret = media_photo_show((char *) aux1, MEDIA_PHOTO_SHOW_RIGHT_LEFT);
		break;
	case MOD_MEDIA_PHOTO_SHOW_PREV:
		MD_CTR("MOD_MEDIA_PHOTO_SHOW_PREV, aux0: %x, aux1: %s", aux0,
				(char *) aux1);
		ret = media_photo_show((char *) aux1, MEDIA_PHOTO_SHOW_LEFT_RIGHT);
		break;
	case MOD_MEDIA_PHOTO_SHOW_DIRECT:
		MD_CTR("MOD_MEDIA_PHOTO_SHOW_DIRECT, aux0: %x, aux1: %s", aux0,
				(char *) aux1);
		ret = media_photo_show((char *) aux1, MEDIA_PHOTO_SHOW_DIRECT);
		break;
	case MOD_MEDIA_PHOTO_SET_WIN_LEVEL:
		MD_CTR("MOD_MEDIA_PHOTO_SET_WIN_LEVEL, aux0: %x, aux1: %p", aux0, aux1);
		ret = media_photo_set_win_level(aux0);
		break;
	case MOD_MEDIA_PHOTO_SHOW_END:
		MD_CTR("MOD_MEDIA_PHOTO_SHOW_END, aux0: %x, aux1: %p", aux0, aux1);
		ret = media_photo_show_end();
		break;
	case MOD_MEDIA_PHOTO_SHOW_SAVE_JPG:
		MD_CTR("MOD_MEDIA_PHOTO_SHOW_SAVE_JPG, aux0: %x, aux1: %p", aux0, aux1);
		ret = media_photo_save_buffer();
		break;
	case MOD_MEDIA_PHOTO_SHOW_SAVE_ROT:
		MD_CTR("MOD_MEDIA_PHOTO_SHOW_SAVE_ROT, aux0: %x, aux1: %p", aux0, aux1);
		ret = media_photo_save_thumb();
		break;
	case MOD_MEDIA_PHOTO_GET_THUMB:
	{
		MD_CTR("MOD_MEDIA_PHOTO_GET_THUMB, aux1:(%d,%p,[%d,%d], %s)",
			((struct media_photo_thumb_param *) aux1)->way,
			((struct media_photo_thumb_param *) aux1)->thumb_buf,
			((struct media_photo_thumb_param *) aux1)->thumb_w,
			((struct media_photo_thumb_param *) aux1)->thumb_h,
			((struct media_photo_thumb_param *) aux1)->file_path);
#ifdef MY_MEDIA_DEBUG_ON
		struct media_photo_thumb_param temp;

		strcpy(temp.file_path, (char *)aux1);
		temp.thumb_w = 320;
		temp.thumb_h = 240;
		temp.thumb_buf = rt_malloc_align(temp.thumb_w*temp.thumb_h*4, 32);
		temp.way = MEDIA_PHOTO_ROTATION_90;
		ret = media_photo_get_thumb(&temp);
		rt_free_align(temp.thumb_buf);
#else
		ret = media_photo_get_thumb((struct media_photo_thumb_param *) aux1);
#endif
	}
		break;
	case MOD_MEDIA_VIDEO_START:
	{
		MD_CTR("MOD_MEDIA_VIDEO_START, aux0:%x, aux1:(%d,%d,[%d,%d,%d,%d])" ,
			aux0,
			((struct media_video_start_param *) aux1)->way,
			((struct media_video_start_param *) aux1)->mode,
			((struct media_video_start_param *) aux1)->win_w,
			((struct media_video_start_param *) aux1)->win_h,
			((struct media_video_start_param *) aux1)->win_offset_x,
			((struct media_video_start_param *) aux1)->win_offset_y);
#ifdef MY_MEDIA_DEBUG_ON
		struct media_video_start_param temp;

		temp.way = MEDIA_VIDEO_ROTATION_90;
		temp.mode = MEDIA_VIDEO_SHOW_STRETCH;
		temp.win_w = 400;
		temp.win_h = 100;
		temp.win_offset_x = 100;
		temp.win_offset_y = 50;

		ret = media_video_start(&temp);
#else
		ret = media_video_start((struct media_video_start_param *) aux1);
#endif
	}
		break;
	case MOD_MEDIA_VIDEO_SET_FILE:
		MD_CTR("MOD_MEDIA_VIDEO_SET_FILE, aux0:%x, aux1:%s", aux0, (char *)aux1);
		ret = media_video_set_file((char *) aux1);
		break;
	case MOD_MEDIA_VIDEO_PLAY:
		MD_CTR("MOD_MEDIA_VIDEO_PLAY, aux0: %x, aux1: %p", aux0, aux1);
		ret = media_video_play();
		break;
	case MOD_MEDIA_VIDEO_PAUSE:
		MD_CTR("MOD_MEDIA_VIDEO_PAUSE, aux0: %x, aux1: %p", aux0, aux1);
		ret = media_video_pause();
		break;
	case MOD_MEDIA_VIDEO_STOP:
		MD_CTR("MOD_MEDIA_VIDEO_STOP, aux0: %x, aux1: %p", aux0, aux1);
		ret = media_video_stop();
		break;
	case MOD_MEDIA_VIDEO_SET_CUR_POS:
		MD_CTR("MOD_MEDIA_VIDEO_SET_CUR_POS, aux0: %d second", aux0);
		ret = media_video_set_cur_pos(aux0);
		break;
	case MOD_MEDIA_VIDEO_GET_END_POS:
		ret = media_video_get_end_pos();
		MD_CTR("MOD_MEDIA_VIDEO_GET_END_POS, ret: %d second", ret);
		break;
	case MOD_MEDIA_VIDEO_GET_CUR_POS:
		ret = media_video_get_cur_pos();
		MD_CTR("MOD_MEDIA_VIDEO_GET_CUR_POS, ret: %d second", ret);
		break;
	case MOD_MEDIA_VIDEO_GET_STATE:
		ret = media_video_get_state();
		MD_CTR("MOD_MEDIA_VIDEO_GET_STATE, ret: %d", ret);
		break;
	case MOD_MEDIA_VIDEO_SET_WIN_LEVEL:
		MD_CTR("MOD_MEDIA_VIDEO_SET_WIN_LEVEL, aux0: %x, aux1: %p", aux0, aux1);
		ret = media_video_set_win_level(aux0);
		break;
	case MOD_MEDIA_VIDEO_GET_THUMB:
		MD_CTR("MOD_MEDIA_VIDEO_GET_THUMB, aux1:(%d,%p,[%d,%d], %s)",
			((struct media_video_thumb_param *) aux1)->way,
			((struct media_video_thumb_param *) aux1)->thumb_buf,
			((struct media_video_thumb_param *) aux1)->thumb_w,
			((struct media_video_thumb_param *) aux1)->thumb_h,
			((struct media_video_thumb_param *) aux1)->file_path);
#ifdef MY_MEDIA_DEBUG_ON
		struct media_video_thumb_param temp;

		strcpy(temp.file_path, (char *)aux1);
		temp.thumb_w = 320;
		temp.thumb_h = 240;
		temp.thumb_buf = rt_malloc_align(temp.thumb_w*temp.thumb_h*4, 32);
		temp.way = MEDIA_PHOTO_ROTATION_90;
		ret = media_video_get_thumb(&temp);
		rt_free_align(temp.thumb_buf);
#else
		ret = media_video_get_thumb((struct media_video_thumb_param *) aux1);
#endif
		break;
	case MOD_MEDIA_VIDEO_END:
		MD_CTR("MOD_MEDIA_VIDEO_END, aux0: %x, aux1: %p", aux0, aux1);
		ret = media_video_end();
		break;
	default:
		MD_ERR("err unsupport cmd:%ud, aux0: %x, aux1: %p", cmd, aux0, aux1);
		break;
	}

	mod_media_unlock();

	return ret;
}

/* init module plugin interface struct */
static mod_if_t modx = { mod_media_init, mod_media_exit, mod_media_read, mod_media_write,
		mod_media_ctrl, };

/**
 * get_mod_if -get module plugin interface
 *
 * This function use to get module plugin interface struct.
 *
 * Returns module plugin interface struct.
 */
mod_if_t *get_mod_if()
{
	return &modx;
}
