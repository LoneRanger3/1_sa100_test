/*
 * vdisp_api.h - Standard functionality for lombo media.
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

#ifndef __VDISP_H__
#define __VDISP_H__

typedef enum vdisp_rotate_mode {
	VDISP_ROTATE_NONE	= 0,
	VDISP_ROTATE_90		= 1,
	VDISP_ROTATE_180	= 2,
	VDISP_ROTATE_270	= 3,
	VDISP_FLIP_H		= 4,
	VDISP_FLIP_H_ROT_90	= 5,
	VDISP_FLIP_V		= 6,
	VDISP_FLIP_V_ROT_90	= 7,
} vdisp_rotate_mode_e;

typedef enum vdisp_mode {
	/* Display in the window at the original size of the video,
	 * can't overflow the window */
	VDISP_WINDOW_ORIGINAL,
	/* Scale to full screen by video ratio, the video show normal */
	VDISP_WINDOW_FULL_SCREEN_VIDEO_RATIO,
	/* Scale to full screen by screen ratio, the video may be distorted */
	VDISP_WINDOW_FULL_SCREEN_SCREEN_RATIO,
	/* Forced to display at 4:3 ratio, the video may be distorted */
	VDISP_WINDOW_4R3MODE,
	/* Forced to display at 16:9 ratio, the video may be distorted */
	VDISP_WINDOW_16R9MODE,
	/* Used to cut off the black side of the video */
	VDISP_WINDOW_CUTEDGE,
	/* User defined mode */
	VDISP_WINDOW_USERDEF,
} vdisp_mode_e;

#endif /* __VDISP_H__ */

