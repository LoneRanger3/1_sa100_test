/*
 * media_disp.h - display implement
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

#ifndef __MEDIA_DISPLAY_H__
#define __MEDIA_DISPLAY_H__

#include "lombo_disp.h"
#include "eos.h"
#include "jpeg_dec_interface.h"
#include "mod_media_i.h"

struct media_disp_win_info {
	int win_w;
	int win_h;
	int screen_offset_x;
	int screen_offset_y;
	int level;
	enum media_photo_show_mode mode;
};

struct media_disp_param {
	int win_index;
	unsigned int pixel_format;
	int img_w;
	int img_h;
	int show_w;
	int show_h;
	int x_offset;
	int y_offset;
	u32 addr[3];
};

extern int media_disp_init(void);

extern int media_disp_exit(void);

extern int media_disp_photo_init(void);

extern int media_disp_photo_exit(void);

extern int media_disp_set_win_info(struct media_disp_win_info *inf);

extern int media_disp_get_wh(int *width, int *height);

extern int media_disp_set_win_level(int level);

extern int media_disp_win(struct media_disp_param *win_param);

extern int media_disp_win_left2right(struct media_disp_param *in_win_param,
		struct media_disp_param *out_win_param);

extern int media_disp_win_right2left(struct media_disp_param *in_win_param,
		struct media_disp_param *out_win_param);

extern int media_disp_win_show(struct media_disp_param *in_win_param,
		struct media_disp_param *out_win_param);

extern int media_disp_win_hide(struct media_disp_param *out_win_param);

extern int media_disp_rot(int in_img_w, int in_img_h, u32 in_format, u32 *in_addr,
		int out_img_w, int out_img_h, u32 out_format, u32 *out_addr,
		rot_mode_type_t rot_mode);

extern void media_disp_rot_free(u32 *out_addr);

extern int media_disp_rotation_jpg(jpeg_param_t *param, rot_mode_type_t rot_mode);

extern void media_disp_rotation_jpg_free(jpeg_param_t *param);

extern int media_disp_scale_nv12_to_argb(u8 *in_addr, int in_img_w, int in_img_h,
		int in_real_w, int in_real_h, u8 *out_addr, int out_img_w, int out_img_h);

#endif /* __MEDIA_DISPLAY_H__ */
