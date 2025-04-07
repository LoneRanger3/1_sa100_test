/*
 * disp_rot_test.c - Disp rot test module driver code for LomboTech
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
#include "disp_test.h"

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

int disp_rot_test(rt_device_disp_ops_t *disp_ops_p)
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

	ret = rt_device_control(disp_device, DISP_CMD_ENABLE_HW_ROT, NULL);
	if (ret != DISP_OK) {
		LOG_E("enable hw rot err");
		return ret;
	}

	rt_memset(file_name, 0x00, ROT_FILE_LEN);
#if ROT_FORMAT_3_PLANAR
	img_w = 800;
	img_h = 480;
	rt_sprintf(file_name, "%s%s", SRC_FILE_PATH, ROT_IMAGE1_NAME);
	fd_i = open(file_name, O_RDWR);
#else
	img_w = 640;
	img_h = 480;
	rt_sprintf(file_name, "%s%s", SRC_FILE_PATH, ROT_IMAGE2_NAME);
	fd_i = open(file_name, O_RDWR);
#endif
	if (fd_i <= 0) {
		LOG_E("open file[%s] err", file_name);
		return DISP_ERROR;
	}

	size = img_w * img_h * 3 / 2;
	RT_ASSERT(size > 0);

	buf_data = rt_malloc_align(size, 8);
	RT_ASSERT(buf_data != NULL);

	rot_data = rt_malloc_align(size, 8);
	RT_ASSERT(rot_data != NULL);

	read_size = read(fd_i, buf_data, size);
	if (read_size != size)
		LOG_E("read err size[%d] write_size[%d]", size, read_size);

	write_size = 0;

	LOG_I("size[%d] img_w[%d] img_h[%d]", size, img_w, img_h);
	rt_memset(&cfgs, 0x00, sizeof(disp_rot_cfg_t));

	for (mod = LOMBO_DRM_TRANSFORM_COPY; mod < LOMBO_DRM_TRANSFORM_MAX; mod++) {
		mode_symbol = get_rot_mode_symbol(mod);
		if (NULL == mode_symbol)
			break;

		rt_memset(file_name, 0x00, ROT_FILE_LEN);
#if ROT_FORMAT_3_PLANAR
		rt_sprintf(file_name, "%s%s%s", SRC_FILE_PATH,
				ROT_IMAGE1_NAME, mode_symbol);
#else
		rt_sprintf(file_name, "%s%s%s", SRC_FILE_PATH,
				ROT_IMAGE2_NAME, mode_symbol);
#endif
		LOG_I("mode[%d] file_name[%s]", mod, file_name);
		fd_o = open(file_name, O_RDWR | O_CREAT);
		if (fd_o <= 0) {
			LOG_E("open file[%s] err", file_name);
			ret = DISP_ERROR;
			break;
		}
		cfgs.mode = mod;

#if ROT_FORMAT_3_PLANAR
		cfgs.infb.format = DISP_FORMAT_YUV420;
		cfgs.outfb.format = DISP_FORMAT_YUV420;

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

		cfgs.infb.addr[1] = (unsigned int)(buf_data + img_w * img_h);
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

		cfgs.infb.addr[2] = (unsigned int)(buf_data + img_w * img_h * 5 / 4);
		cfgs.infb.width[2] = img_w / 2;
		cfgs.infb.height[2] = img_h / 2;

		cfgs.outfb.addr[2] = (unsigned int)(rot_data + img_w * img_h * 5 / 4);
		if (cfgs.mode % 2 == 0) {
			cfgs.outfb.width[2] = img_w / 2;
			cfgs.outfb.height[2] = img_h / 2;
		} else { /* w and h is exchange */
			cfgs.outfb.width[2] = img_h / 2;
			cfgs.outfb.height[2] = img_w / 2;
		}
#else
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

		cfgs.infb.addr[1] = (unsigned int)(buf_data + img_w * img_h);
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
#endif

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

	ret = rt_device_control(disp_device, DISP_CMD_DISABLE_HW_ROT, NULL);
	if (ret != DISP_OK) {
		LOG_E("disable hw rot err");
		return ret;
	}

	LOG_I("disp_rot_test end");
	return ret;
}


