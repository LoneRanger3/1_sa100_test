/*
 * player.h -  mod media interface code from file explorer
 * mod media interface code interface and macro define
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

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <app_manage.h>
#include <mod_manage.h>
#include <stdio.h>
#include <stdlib.h>
#include <rtthread.h>

typedef struct tag_mod_screen {
	int width;
	int height;
} mod_screen_t;

int mod_media_load(void);
int mod_media_photo_show_start(void);
int mod_media_photo_show(char *path);
int mod_media_photo_show_end(void);
int mod_media_photo_set_win_level(int aux);
#if 0
int mod_media_photo_get_thumb(void);
#endif
char *mod_media_photo_get_thumb
(char *file_path, int thumb_w, int thumb_h);
int mod_media_photo_free_thumb(char *thumb_buf);
char *mod_media_video_get_thumb
(char *file_path, int thumb_w, int thumb_h);
int mod_media_video_free_thumb(char *thumb_buf);
int mod_media_video_start(void);
int mod_media_video_set_path(char *video_path);
int mod_media_video_get_state(void);
int mod_media_video_play(void);
int mod_media_video_stop(void);
int mod_media_video_pause(void);
int mod_media_video_set_win_level(int aux);
int mod_media_set_cur_pos(int cur_pos);
int mod_media_get_cur_pos(void);
int mod_media_get_end_pos(void);
int mod_media_video_end(void);
int mod_media_unload(void);

#endif /* __PLAYER_H__ */
