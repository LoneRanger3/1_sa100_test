/*
 * fileexp_common.h - file sub_system common code for LomboTech
 * file sub_system common interface and macro define
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

#ifndef __FILEEXP_COMMON_H__
#define __FILEEXP_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "lb_types.h"
#include "lb_common.h"
#include "cJSON.h"

/* #define FILE_LOCK_OMIT 1 */
/* #define FILE_BIN_OMIT  1 */

typedef enum {
	ALL_MODE = 0x00,
	VIDEO_MODE = 0x01,
	PIC_MODE = 0x02,
	AUDIO_MODE = 0x03,
	USER_DEFINE = 0x04,
} open_mode_t;

typedef struct tag_explore {
	lb_int32 tab_cur;
	void *tab_ext;
	lb_flist_t *sch_list;
	lb_flist_t *all_list;
	lb_flist_t *image_list;
	lb_flist_t *video_list;
	lb_flist_t *audio_list;
	void *text_area;
	void *key_board;
} explore_t;

lb_int32 file_get_list(lb_uint16 tab_cur, void **pproperty);
lb_int32 file_init_funcs(void);
lb_int32 file_uninit_funcs(void);
lb_int32 file_resp_funcs(void);
lb_int32 file_unresp_funcs(void);
lb_int32 file_list_enter(void);
lb_int32 file_list_return(void);
lb_int32 video_list_init(void *param);
lb_int32 video_list_exit(void *param);
lb_int32 image_list_init(void *param);
lb_int32 image_list_exit(void *param);
lb_int32 file_show_tab(void);
lb_int32 file_hide_tab(void);

#define LB_MSG_FILEEXP_IMG_NEXT	(LB_MSG_FILEEXP_BASE|0x80)
#define LB_MSG_FILEEXP_IMG_PREV	(LB_MSG_FILEEXP_BASE|0x81)
#define LB_MSG_FILEEXP_VIDEO_NEXT	(LB_MSG_FILEEXP_BASE|0x82)
#define LB_MSG_FILEEXP_VIDEO_PREV	(LB_MSG_FILEEXP_BASE|0x83)
#define LB_MSG_FILEEXP_VIDEO_PP	(LB_MSG_FILEEXP_BASE|0x84)
#define LB_MSG_FILEEXP_VIDEO_SLIDER	(LB_MSG_FILEEXP_BASE|0x85)
#define LB_MSG_FILEEXP_VIDEO_ERROR	(LB_MSG_FILEEXP_BASE|0x86)

#define LB_MSG_FILEEXP_VIDEO_DRAW_SLIDER	(LB_MSG_FILEEXP_BASE|0x90)
#define LB_MSG_FILEEXP_VIDEO_DRAW_TIME	(LB_MSG_FILEEXP_BASE|0x91)
#define LB_MSG_FILEEXP_IMAGE_DRAW_THUMB	(LB_MSG_FILEEXP_BASE|0x92)
#define LB_MSG_FILEEXP_VIDEO_DRAW_THUMB	(LB_MSG_FILEEXP_BASE|0x93)

#define LB_MSG_FILEEXP_VIDEO_BG_CLICK	 (LB_MSG_FILEEXP_BASE|0xA0)
#define LB_MSG_FILEEXP_IMAGE_BG_CLICK	 (LB_MSG_FILEEXP_BASE|0xA1)

// #define F_LOCK_PATH "VIDEO_F_LOCK"
// #define F_UNLOCK_PATH "VIDEO_F"
// #define R_LOCK_PATH "VIDEO_R_LOCK"
// #define R_UNLOCK_PATH "VIDEO_R"

#define F_LOCK_PATH "FO"
#define F_UNLOCK_PATH "F"
#define R_LOCK_PATH "RO"
#define R_UNLOCK_PATH "R"

#ifdef __cplusplus
}
#endif

#endif /* __FILEEXP_COMMON_H__ */
