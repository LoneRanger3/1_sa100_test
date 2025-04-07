/*
 * disp_test.h - Disp test module head file
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
#include <rtthread.h>
#include <list.h>
#include <string.h>
#include <ctype.h>

#define DBG_SECTION_NAME		"DISP_TEST"
#define DBG_LEVEL			DBG_INFO
#include <debug.h>

#include <dfs_posix.h>
#include "lombo_disp.h"

#define DISP_TEST_THREAD_PRIORITY	(RT_MAIN_THREAD_PRIORITY - 1)
#define DISP_TEST_THREAD_STACK_SIZE	4096

#define SWITCH_WIN_TEST		0
#define SCREENSHOT_TEST		1
#define SCREEN_ROT_TEST		1
#define DISP_EXIT_TEST		0
#define DOUBLE_SE_TEST		1
#if DOUBLE_SE_TEST
#define OFFLINE_SE_TEST		0
#define OFFLINE_SE_IMAGE_IS_YUV	1
#endif
#define POWER_TEST		0 /* suspend and resume test */
#define PAGE_FLIP_TEST		0

#define BACKLIGHT_TEST		1 /* open and close backlight test */
#define BACKLIGHT_PWM_TEST	1 /* backlight value test */

#define ROT_FORMAT_3_PLANAR	0

#define DC_BACKCOLOR_TEST	1
#define DC_COLOR_ENHANCE_ENABLE	1
#define DC_BKL_SHOW_HIDE_ENABLE	1 /* show or hide BLOCKLINKER */
#define DC_HWC_SHOW_HIDE_ENABLE	1 /* show or hide hwc */
#define DC_BKL_ALPHA_TEST	1 /* BLOCKLINKER alpha test  */

#define WIN_COLOR_KEY_TEST	1
#define WIN_ALPHA_TEST		1
#define WIN_SHOW_HIDE_TEST	1

#define DISP_TEST_TIME		10 /* seconds */
#define SE_BUF_NUM		5

#define VIDEO_W			352
#define VIDEO_H			288
#define VIDEO_SIZE		(VIDEO_W * VIDEO_H * 3 / 2)
#define VIDEO_FMT		DISP_FORMAT_YUV420

#define BKL_COLOR_TEST_NUM	(DC_BL_IDX_5 + 1)

#define WIN0_TEST_ENABLE	1
#if WIN0_TEST_ENABLE
#define DYNAMIC_SE		0
#endif

#ifdef ARCH_LOMBO_N7V1_CDR
#define CDR_START_Y		200
#endif

#define SRC_FILE_PATH		"mnt/sdcard/disp_src/"

#define SCREENSHOT_FILE_NAME	"mnt/sdcard/screenshot"

#define VIDEO_NAME		"ok.yuv"
#define SE1_IMAGE_NAME		"viss_nv12.yuv"
#define SUBTITLE_NAME		"subtitle.bmp"
#define SYS_BAR_NAME		"sys_bar.bmp"
#define SE1_BMP_IMAGE_NAME	"screenshot.bmp"

#define ROT_IMAGE1_NAME		"rot/yuv420_800x480.yuv"
#define ROT_IMAGE2_NAME		"rot/nv12_640x480.yuv"

rt_device_t disp_device;
disp_ctl_t *wctl[DC_WIN_NUM + 1];
bool disp_test_start_flag;

#if SCREENSHOT_TEST
extern int disp_screenshot(char *buf_addr, u32 size);
#endif

int disp_ioctl_test(u32 cnt);
int disp_ioctl_cfg(char *cmd);
void disp_se0_test(rt_device_disp_ops_t *disp_ops_p);
void disp_se1_test(rt_device_disp_ops_t *disp_ops_p);
int disp_dit_test(rt_device_disp_ops_t *disp_ops_p);
int disp_rot_test(rt_device_disp_ops_t *disp_ops_p);

void backlight_pwm_set_test(u32 value);
void backcolor_set_test(u32 bk_color);

