/*
 * media_disp.c - display implement
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
#include "media_display.h"

static rt_device_t disp_device;
static disp_ctl_t *wctl[2];
static rt_device_disp_ops_t *disp_ops_p;

static struct media_disp_win_info g_win_inf;

/**
 * media_disp_init - first init display
 *
 * Returns 0 if there is no error; -1, return the error code.
 */
int media_disp_init(void)
{
	int ret;

	disp_device = rt_device_find(DISP_DEVICE_NAME);
	if (disp_device == NULL) {
		MD_ERR("fail to find display");
		return -1;
	}

	rt_device_open(disp_device, 0);

	ret = rt_device_control(disp_device, DISP_CMD_ENABLE_HW_ROT, NULL);
	if (ret != DISP_OK) {
		MD_ERR("enable hw rot err");
		return -1;
	}
#ifdef MY_MEDIA_DEBUG_ON
	ret = rt_device_control(disp_device, DISP_CMD_ENABLE_DC_HWC, NULL);
	if (ret != DISP_OK) {
		MD_ERR("enable hw rot err");
		return -1;
	}
#endif
	disp_ops_p = (rt_device_disp_ops_t *) (disp_device->user_data);
	if (disp_ops_p)
		return 0;
	else
		return -1;
}

/**
 * media_disp_exit - over display
 *
 * Returns 0 if there is no error; -1, return the error code.
 */
int media_disp_exit(void)
{
	int ret;

	if (disp_device) {
		ret = rt_device_control(disp_device, DISP_CMD_DISABLE_HW_ROT, NULL);
		if (ret != DISP_OK)
			MD_ERR("disable hw rot err");
		rt_device_close(disp_device);
		disp_device = NULL;
	}

	return 0;
}

/**
 * media_disp_photo_init - init display for photo show
 *
 * Returns 0 if there is no error; -1, return the error code.
 */
int media_disp_photo_init(void)
{
	if (disp_device) {
		if (disp_ops_p) {
			wctl[0] = disp_ops_p->disp_win_request("media display0");
			wctl[1] = disp_ops_p->disp_win_request("media display1");
		}

		if (wctl[0] == NULL || wctl[1] == NULL) {
			MD_ERR("fail to request win");
			rt_device_close(disp_device);
			disp_device = NULL;
			return -1;
		}
	}

	return 0;
}

/**
 * media_disp_photo_exit - exit display for photo show
 *
 * Returns 0 if there is no error; -1, return the error code.
 */
int media_disp_photo_exit(void)
{
	if (disp_ops_p) {
		disp_ops_p->disp_win_release(&wctl[0]);
		wctl[0] = NULL;
		disp_ops_p->disp_win_release(&wctl[1]);
		wctl[1] = NULL;
	}

	return 0;
}

/**
 * media_disp_set_win_info - set show window information
 * @param: pointer to win_info data pointer.
 *
 * Returns 0 if there is no error; -1, return the error code.
 */
int media_disp_set_win_info(struct media_disp_win_info *inf)
{
	g_win_inf.win_w = inf->win_w;
	g_win_inf.win_h = inf->win_h;
	g_win_inf.screen_offset_x = inf->screen_offset_x;
	g_win_inf.screen_offset_y = inf->screen_offset_y;
	g_win_inf.mode = inf->mode;
	g_win_inf.level = 0;

	return 0;
}

/**
 * media_disp_get_wh - get screen w, h
 * @width: get pointer to width.
 * @height: get pointer to height.
 *
 * Returns 0 if there is no error; -1, return the error code.
 */
int media_disp_get_wh(int *width, int *height)
{
	struct rt_device_graphic_info info;
	disp_io_ctrl_t dic;

	memset(&dic, 0x00, sizeof(disp_io_ctrl_t));
	dic.dc_index = 0;
	dic.args = &info;
	rt_device_control(disp_device, DISP_CMD_GET_INFO, &dic);

	MD_LOG("width[%d] height[%d]", info.width, info.height);

	if (width)
		*width = info.width;
	if (height)
		*height = info.height;

	return 0;
}

/**
 * media_disp_set_win_level - set window level
 * @level: number.
 *
 * Returns 0 if there is no error; -1, return the error code.
 */
int media_disp_set_win_level(int level)
{
	if (disp_ops_p == NULL)
		return -1;

	g_win_inf.level = level;

	if (g_win_inf.level == 0) {
		if (wctl[0] != NULL)
			disp_ops_p->disp_set_win_layer(wctl[0], WIN_LAYER_TOP);
		if (wctl[1] != NULL)
			disp_ops_p->disp_set_win_layer(wctl[1], WIN_LAYER_TOP);
	} else {
		if (wctl[0] != NULL)
			disp_ops_p->disp_set_win_layer(wctl[0], WIN_LAYER_BOTTOM);
		if (wctl[1] != NULL)
			disp_ops_p->disp_set_win_layer(wctl[1], WIN_LAYER_BOTTOM);
	}
	return 0;
}

/**
 * media_disp_win - show window
 * @param: pointer to win_param data pointer.
 *
 * Returns 0 if there is no error; -1, return the error code.
 */
int media_disp_win(struct media_disp_param *win_param)
{
	dc_win_data_t win_data = {0};
	int screen_w, screen_h, win_w, win_h;

	if (disp_ops_p == NULL || win_param->win_index == -1)
		return -1;

	screen_w = g_win_inf.win_w;
	screen_h = g_win_inf.win_h;
	if (g_win_inf.mode == MEDIA_PHOTO_SHOW_CENTER) {
		if (screen_w > screen_h) {
			if (screen_w * win_param->img_h < screen_h * win_param->img_w) {
				win_h = (screen_w * win_param->img_h) / win_param->img_w;
				win_w = screen_w;
			} else {
				win_w = (win_param->img_w * screen_h) / win_param->img_h;
				win_h = screen_h;
			}
		} else {
			if (screen_w * win_param->img_h > screen_h * win_param->img_w) {
				win_w = (win_param->img_w * screen_h) / win_param->img_h;
				win_h = screen_h;
			} else {
				win_h = (screen_w * win_param->img_h) / win_param->img_w;
				win_w = screen_w;
			}
		}
	} else {
		win_w = g_win_inf.win_w;
		win_h = g_win_inf.win_h;
	}

	win_data.dma_addr = win_param->addr[0];
	win_data.chroma_dma_addr = win_param->addr[1];
	win_data.pixel_format = win_param->pixel_format;
	win_data.pixel_order = DC_PO_NORMAL;
	win_data.bpp = 32;
	win_data.crtc_x = win_param->x_offset + ((screen_w / 2) - (win_w / 2))
			+ g_win_inf.screen_offset_x;
	win_data.crtc_y = win_param->y_offset + ((screen_h / 2) - (win_h / 2))
			+ g_win_inf.screen_offset_y;
	win_data.crtc_width = win_w;
	win_data.crtc_height = win_h;

	MD_LOG("WIN:(%d,%d,%d,%d)", win_data.crtc_x, win_data.crtc_y,
			win_data.crtc_width, win_data.crtc_height);

	win_data.fb_x = 0;
	win_data.fb_y = 0;
	win_data.fb_width = win_param->show_w;
	win_data.fb_height = win_param->show_h;

	win_data.src_width = win_param->img_w;
	win_data.src_height = win_param->img_h;

	win_data.update_flag = true;
	disp_ops_p->disp_win_update(wctl[win_param->win_index], &win_data);

	return 0;
}

/**
 * media_disp_win_left2right - move win left to right
 * @in_win_param: in pointer to disp data pointer.
 * @out_win_param: out pointer to disp data pointer.
 *
 * Returns 0 if there is no error; -1, return the error code.
 */
int media_disp_win_left2right(struct media_disp_param *in_win_param,
		struct media_disp_param *out_win_param)
{
	int i = 0, cnt = 1;

	for (i = 0; i < g_win_inf.win_w; i += cnt * cnt, cnt++) {
		in_win_param->x_offset = i - g_win_inf.win_w;
		in_win_param->y_offset = 0;
		media_disp_win(in_win_param);
		out_win_param->x_offset = i;
		out_win_param->y_offset = 0;
		media_disp_win(out_win_param);
	}
	in_win_param->x_offset = 0;
	in_win_param->y_offset = 0;
	media_disp_win(in_win_param);
	out_win_param->x_offset = -10000;
	out_win_param->y_offset = 0;
	media_disp_win(out_win_param);

	return 0;
}

/**
 * media_disp_win_right2left - move win right to left
 * @in_win_param: in pointer to disp data pointer.
 * @out_win_param: out pointer to disp data pointer.
 *
 * Returns 0 if there is no error; -1, return the error code.
 */
int media_disp_win_right2left(struct media_disp_param *in_win_param,
		struct media_disp_param *out_win_param)
{
	int i = 0, cnt = 1;

	for (i = g_win_inf.win_w; i > 0; i -= cnt * cnt, cnt++) {
		in_win_param->x_offset = i;
		in_win_param->y_offset = 0;
		media_disp_win(in_win_param);
		out_win_param->x_offset = i - g_win_inf.win_w;
		out_win_param->y_offset = 0;
		media_disp_win(out_win_param);
	}
	in_win_param->x_offset = 0;
	in_win_param->y_offset = 0;
	media_disp_win(in_win_param);
	out_win_param->x_offset = -10000;
	out_win_param->y_offset = 0;
	media_disp_win(out_win_param);
	return 0;
}

/**
 * media_disp_win_show - show win
 * @in_win_param: in pointer to disp data pointer.
 * @out_win_param: out pointer to disp data pointer.
 *
 * Returns 0 if there is no error; -1, return the error code.
 */
int media_disp_win_show(struct media_disp_param *in_win_param,
		struct media_disp_param *out_win_param)
{
	in_win_param->x_offset = 0;
	in_win_param->y_offset = 0;
	media_disp_win(in_win_param);
	out_win_param->x_offset = -10000;
	out_win_param->y_offset = 0;
	media_disp_win(out_win_param);

	return 0;
}

/**
 * media_disp_win_hide - hid win
 * @in_win_param: in pointer to disp data pointer.
 * @out_win_param: out pointer to disp data pointer.
 *
 * Returns 0 if there is no error; -1, return the error code.
 */
int media_disp_win_hide(struct media_disp_param *out_win_param)
{
	out_win_param->x_offset = -10000;
	out_win_param->y_offset = 0;
	media_disp_win(out_win_param);

	return 0;
}

/**
 * media_disp_rot - rot yuv
 * @in_img_w: input image w.
 * @in_img_h: input image h.
 * @in_format: input image format.
 * @in_addr: pointer input image buffer.
 * @out_img_w: get output image w.
 * @out_img_h: get output image h.
 * @out_format: output image format.
 * @out_addr: pointer output image buffer.
 * @rot_mode: rot mode.
 *
 * Returns 0 if there is no error; -1, return the error code.
 */
int media_disp_rot(int in_img_w, int in_img_h, u32 in_format, u32 *in_addr, int out_img_w,
		int out_img_h, u32 out_format, u32 *out_addr, rot_mode_type_t rot_mode)
{
	disp_rot_cfg_t cfgs = { 0 };
	u32 out_size;
	char *buf_data;
	char *rot_data;

	if (disp_ops_p == NULL)
		return -1;

	buf_data = (char *) in_addr[0];

	if (DISP_FORMAT_BGRA8888 == out_format)
		out_size = out_img_w * out_img_h * 3;
	else
		out_size = out_img_w * out_img_h * 3 / 2;

	rot_data = rt_malloc_align(out_size, 32);
	if (rot_data == NULL)
		return -1;
	out_addr[0] = (u32) rot_data;

	cfgs.mode = rot_mode;

	cfgs.infb.format = in_format;
	cfgs.outfb.format = out_format;

	cfgs.infb.addr[0] = (unsigned int) buf_data;
	cfgs.infb.width[0] = (in_img_w / 32 * 32);
	cfgs.infb.height[0] = (in_img_h / 32 * 32);
	cfgs.infb.addr[1] = (unsigned int) (buf_data + in_img_w * in_img_h);
	cfgs.infb.width[1] = (in_img_w / 32 * 32) / 2;
	cfgs.infb.height[1] = (in_img_h / 32 * 32) / 2;

	cfgs.outfb.addr[0] = (unsigned int) rot_data;
	cfgs.outfb.width[0] = (out_img_w / 32 * 32);
	cfgs.outfb.height[0] = (out_img_h / 32 * 32);

	cfgs.outfb.addr[1] = (unsigned int) (rot_data
			+ (out_img_w / 32 * 32) * (out_img_h / 32 * 32));
	cfgs.outfb.width[1] = (out_img_w / 32 * 32) / 2;
	cfgs.outfb.height[1] = (out_img_h / 32 * 32) / 2;

	disp_ops_p->disp_rot_process(&cfgs);

	return 0;
}

/**
 * media_disp_rot_free - free rot
 * @out_addr: point to buffer.
 *
 * Returns 0 if there is no error; -1, return the error code.
 */
void media_disp_rot_free(u32 *out_addr)
{
	if (out_addr[0])
		rt_free_align((void *) out_addr[0]);
}

/**
 * media_disp_rotation_jpg - rotation jpg yuv
 * @param: pointer to jpeg data pointer.
 * @rot_mode: rot mode.
 *
 * Returns 0 if there is no error; -1, return the error code.
 */
int media_disp_rotation_jpg(jpeg_param_t *param, rot_mode_type_t rot_mode)
{
	disp_rot_cfg_t cfgs = { 0 };
	u32 out_size;
	u32 in_addr[3], out_addr[3];
	int in_img_w, in_img_h, out_img_w, out_img_h, temp;
#if 0
	if (0) {
		FILE *f;
		f = fopen("/mnt/sdcard/out.bin", "wb");
		fwrite(param->output_data, 1,
			param->output_w * param->output_h * 3 / 2, f);
		fclose(f);
		f = fopen("/mnt/sdcard/out.bin", "rb");
		fread(param->output_data, 1,
			param->output_w * param->output_h * 3 / 2, f);
		fclose(f);
	}
	if (0) {
		int f;
		f = open("/mnt/sdcard/out.bin", O_RDWR | O_CREAT);
		write(f, param->output_data, param->output_w * param->output_h * 3 / 2);
		close(f);
		f = open("/mnt/sdcard/out.bin", O_RDWR);
		read(f, param->output_data, param->output_w * param->output_h * 3 / 2);
		close(f);
	}
	if (0) {
		int f;
		f = open("/mnt/sdcard/out.bin", O_RDWR);
		jpeg_decode_end(param);
		param->output_data = rt_malloc_align(param->output_w *
							param->output_h * 3 / 2, 32);
		read(f, param->output_data, param->output_w * param->output_h * 3 / 2);
		close(f);
	}
#endif
	if (disp_ops_p == NULL || param == NULL) {
		MD_ERR("media_disp_rotation_jpg disp_ops_p == NULL");
		return -1;
	}

	MD_LOG("media_disp_rotation_jpg,%d", rot_mode);

	in_img_w = param->output_w;
	in_img_h = param->output_h;
	if (LOMBO_DRM_TRANSFORM_ROT_90 == rot_mode
			|| LOMBO_DRM_TRANSFORM_ROT_270 == rot_mode) {
		out_img_w = (in_img_h / 32 * 32);
		out_img_h = (in_img_w / 32 * 32);
		in_img_w = (in_img_w / 32 * 32);
		in_img_h = (in_img_h / 32 * 32);
	} else {
		out_img_w = (in_img_w / 32 * 32);
		out_img_h = (in_img_h / 32 * 32);
		in_img_w = (in_img_w / 32 * 32);
		in_img_h = (in_img_h / 32 * 32);
	}

	in_addr[0] = (u32) param->output_data;
	in_addr[1] = (u32) param->output_data + param->offset_chroma;
	in_addr[2] = 0;

	out_size = out_img_w * out_img_h * 3 / 2;
	out_addr[0] = (u32) rt_malloc_align(out_size, 32);
	if (out_addr[0] == 0) {
		MD_ERR("media_disp_rotation_jpg malloc fail, %ud", out_size);
		return -1;
	}
	out_addr[1] = out_addr[0] + out_img_w * out_img_h;
	out_addr[2] = 0;

	cfgs.mode = rot_mode;
	cfgs.rot_way = HW_ROT;

	cfgs.infb.format = DISP_FORMAT_NV12;
	cfgs.outfb.format = DISP_FORMAT_NV12;

	cfgs.infb.addr[0] = in_addr[0];
	cfgs.infb.width[0] = in_img_w;
	cfgs.infb.height[0] = in_img_h;
	cfgs.infb.addr[1] = in_addr[1];
	cfgs.infb.width[1] = in_img_w / 2;
	cfgs.infb.height[1] = in_img_h / 2;

	cfgs.outfb.addr[0] = out_addr[0];
	cfgs.outfb.width[0] = out_img_w;
	cfgs.outfb.height[0] = out_img_h;
	cfgs.outfb.addr[1] = out_addr[1];
	cfgs.outfb.width[1] = out_img_w / 2;
	cfgs.outfb.height[1] = out_img_h / 2;

	disp_ops_p->disp_rot_process(&cfgs);

	if (param->jpeg_file)
		jpeg_decode_end(param);
	else
		rt_free_align((void *) param->output_data);

	param->jpeg_file = 0;
	param->output_data = (unsigned char *) out_addr[0];
	param->offset_luma = 0;
	param->offset_chroma = out_img_w * out_img_h;
	param->size_luma = out_img_w * out_img_h;
	param->size_chroma = out_img_w * out_img_h / 2;

	param->output_w = out_img_w;
	param->output_h = out_img_h;

	temp = param->real_w;
	param->real_w = out_img_w > param->real_h ? param->real_h : out_img_w;
	param->real_h = out_img_h > temp ? temp : out_img_h;
#if 0
	rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH,
				(void *)out_addr[0], out_size);
	rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE,
				(void *)out_addr[0], out_size);
#endif

	return 0;
}

/**
 * media_disp_rotation_jpg_free - free rotation jpg yuv
 * @param: pointer to jpeg data pointer.
 *
 */
void media_disp_rotation_jpg_free(jpeg_param_t *param)
{
	if (param->output_data) {
		rt_free_align((void *) param->output_data);
		param->output_data = NULL;
	}
}

/**
 * media_disp_scale_nv12_to_argb - scale nv12 and change to argb
 * @in_addr: point to input buffer.
 * @in_img_w: input image w.
 * @in_img_h: input image h.
 * @in_real_w: input real w.
 * @in_real_h: input real h.
 * @out_addr: point to output buffer.
 * @out_img_w: output image w.
 * @out_img_h: output image h.
 *
 * Returns 0 if there is no error; -1, return the error code.
 */
int media_disp_scale_nv12_to_argb(u8 *in_addr, int in_img_w, int in_img_h,
		int in_real_w, int in_real_h, u8 *out_addr, int out_img_w, int out_img_h)
{
	int ret;
	dc_win_data_t win_data;
	disp_se_t *dse = NULL;
	u32 addr[3] = {0};

	if (disp_ops_p == NULL || out_addr == NULL) {
		MD_ERR("media_disp_scale_nv12_to_argb disp_ops_p == NULL");
		return -1;
	}

	dse = disp_ops_p->disp_se_request();
	if (NULL == dse) {
		MD_ERR("disp_se_request err");
		return -1;
	}

	MD_LOG("media_disp_scale_jpg: (%d, %d)", out_img_w, out_img_h);

	win_data.dma_addr = (u32)in_addr;
	win_data.chroma_dma_addr = (u32)(in_addr + in_img_w*in_img_h);
	win_data.pixel_format = DISP_FORMAT_NV12;
	win_data.pixel_order = DC_PO_NORMAL;
	win_data.bpp = 32;

	win_data.crtc_x = 0;
	win_data.crtc_y = 0;
	win_data.crtc_width = out_img_w;
	win_data.crtc_height = out_img_h;

	win_data.fb_x = 0;
	win_data.fb_y = 0;
	win_data.fb_width = in_img_w > in_real_w ? in_real_w : in_img_w;
	win_data.fb_height = in_img_h > in_real_h ? in_real_h : in_img_h;

	win_data.src_width = in_img_w;
	win_data.src_height = in_img_h;

	win_data.update_flag = true;

	addr[0] = (u32)out_addr;
	addr[1] = 0;
	addr[2] = 0;
	ret = disp_ops_p->disp_se_process(dse, &win_data,
						addr, DISP_FORMAT_ARGB8888);

	disp_ops_p->disp_se_release(&dse);

	return ret;
}

#if 0
#define SRC_FILE_PATH "/mnt/sdcard/"
#define ROT_IMAGE2_NAME "in.bin"

#define LOG_I MD_LOG
#define LOG_E MD_LOG

#define ROT_FILE_LEN			128

#define ROT_TRANSFORM_COPY		"-mode-copy"
#define ROT_TRANSFORM_ROT_90		"-mode-rot90"
#define ROT_TRANSFORM_ROT_180		"-mode-rot180"
#define ROT_TRANSFORM_ROT_270		"-mode-rot270"

#define ROT_TRANSFORM_FLIP_H		"-mode-flip-h"
#define ROT_TRANSFORM_FLIP_H_ROT_90	"-mode-flip-h-rot90"
#define ROT_TRANSFORM_FLIP_V		"-mode-flip-v"
#define ROT_TRANSFORM_FLIP_V_ROT_90	"-mode-flip-v-rot90"

static char *get_rot_mode_symbol(u32 mode)
{
	char *str = NULL;

	switch (mode) {

	case LOMBO_DRM_TRANSFORM_COPY:
		str = ROT_TRANSFORM_COPY;
		break;
	case LOMBO_DRM_TRANSFORM_ROT_90:
		str = ROT_TRANSFORM_ROT_90;
		break;
	case LOMBO_DRM_TRANSFORM_ROT_180:
		str = ROT_TRANSFORM_ROT_180;
		break;
	case LOMBO_DRM_TRANSFORM_ROT_270:
		str = ROT_TRANSFORM_ROT_270;
		break;

	case LOMBO_DRM_TRANSFORM_FLIP_H:
		str = ROT_TRANSFORM_FLIP_H;
		break;
	case LOMBO_DRM_TRANSFORM_FLIP_H_ROT_90:
		str = ROT_TRANSFORM_FLIP_H_ROT_90;
		break;
	case LOMBO_DRM_TRANSFORM_FLIP_V:
		str = ROT_TRANSFORM_FLIP_V;
		break;
	case LOMBO_DRM_TRANSFORM_FLIP_V_ROT_90:
		str = ROT_TRANSFORM_FLIP_V_ROT_90;
		break;

	default:
		LOG_E("illegal rot mode");
		break;
	}

	return str;
}

int media_disp_rot_test(void)
{
	u32 write_size, read_size, size, mod;
	int ret = DISP_OK, fd_i, fd_o;
	char *buf_data;
	u32 img_w, img_h;
	char *rot_data;
	char *mode_symbol;
	disp_rot_cfg_t cfgs;
	char file_name[ROT_FILE_LEN];

	LOG_I("disp_rot_test start");
	RT_ASSERT(disp_ops_p != NULL);

	rt_memset(file_name, 0x00, ROT_FILE_LEN);

	img_w = 1024;
	img_h = 688;
	rt_sprintf(file_name, "%s%s", SRC_FILE_PATH, ROT_IMAGE2_NAME);
	fd_i = open(file_name, O_RDWR);

	if (fd_i <= 0) {
		LOG_E("open file[%s] err", file_name);
		return DISP_ERROR;
	}

	size = img_w * img_h * 3 / 2;
	RT_ASSERT(size > 0);

	buf_data = rt_malloc_align(size, 32);
	RT_ASSERT(buf_data != NULL);

	read_size = read(fd_i, buf_data, size);
	if (read_size != size)
		LOG_E("read err size[%d] write_size[%d]", size, read_size);

	img_h = 672;
	size = img_w * img_h * 3 / 2;
	rot_data = rt_malloc_align(size, 32);
	RT_ASSERT(rot_data != NULL);

	write_size = 0;

	LOG_I("size[%d] img_w[%d] img_h[%d]", size, img_w, img_h);
	rt_memset(&cfgs, 0x00, sizeof(disp_rot_cfg_t));

	for (mod = LOMBO_DRM_TRANSFORM_ROT_90; mod < LOMBO_DRM_TRANSFORM_ROT_90 + 1;
										mod++) {
		mode_symbol = get_rot_mode_symbol(mod);
		if (NULL == mode_symbol)
			break;

		rt_memset(file_name, 0x00, ROT_FILE_LEN);

		rt_sprintf(file_name, "%s%s%s", SRC_FILE_PATH,
				ROT_IMAGE2_NAME, mode_symbol);

		LOG_I("mode[%d] file_name[%s]", mod, file_name);
		fd_o = open(file_name, O_RDWR | O_CREAT);
		if (fd_o <= 0) {
			LOG_E("open file[%s] err", file_name);
			ret = DISP_ERROR;
			break;
		}
		cfgs.mode = mod;

		cfgs.infb.format = DISP_FORMAT_NV12;
		cfgs.outfb.format = DISP_FORMAT_NV12;

		cfgs.infb.addr[0] = (unsigned int)buf_data;
		cfgs.infb.width[0] = img_w;
		cfgs.infb.height[0] = img_h;

		cfgs.outfb.addr[0] = (unsigned int)rot_data;
		if (cfgs.mode % 2 == 0) {
			cfgs.outfb.width[0] = img_w;
			cfgs.outfb.height[0] = img_h;
		} else { /* w and h is exchange */
			cfgs.outfb.width[0] = img_h;
			cfgs.outfb.height[0] = img_w;
		}

		cfgs.infb.addr[1] = (unsigned int)(buf_data + img_w * 688);
		cfgs.infb.width[1] = img_w / 2;
		cfgs.infb.height[1] = img_h / 2;

		cfgs.outfb.addr[1] = (unsigned int)(rot_data + img_w * img_h);
		if (cfgs.mode % 2 == 0) {
			cfgs.outfb.width[1] = img_w / 2;
			cfgs.outfb.height[1] = img_h / 2;
		} else { /* w and h is exchange */
			cfgs.outfb.width[1] = img_h / 2;
			cfgs.outfb.height[1] = img_w / 2;
		}

		disp_ops_p->disp_rot_process(&cfgs);

		lseek(fd_o, 0, SEEK_SET);
		write_size = write(fd_o, rot_data, size);
		if (write_size != size)
			LOG_E("write err size[%d] write_size[%d]", size, write_size);

		if (fd_o > 0)
			close(fd_o);
	}
	if (fd_i > 0)
		close(fd_i);

	rt_free_align(buf_data);
	rt_free_align(rot_data);

	LOG_I("disp_rot_test end");
	return ret;
}
#endif
