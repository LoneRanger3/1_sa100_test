/*
 * lombo_disp.h - Lombo disp module head file
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
#ifndef __LOMBO_DISP_H__
#define __LOMBO_DISP_H__
#include <rtdevice.h>
#include <stdint.h>
#include "lombo_dpu.h"
#include "lombo_doss.h"
#include "disp_common_init.h"
#include "disp_list.h"
#include "disp_blocklinker.h"
#include "disp_se.h"
#include "disp_window.h"
#include "disp_debug.h"

#define DISP_DEVICE_NAME		"disp"

/*(RT_MAIN_THREAD_PRIORITY / 2)*/
#define DISP_THREAD_PRIORITY	9
#define DISP_THREAD_STACK_SIZE	2048

typedef enum tag_disp_cmd {
	DISP_CMD_GET_INFO = 0,
	DISP_CMD_SET_BACKLIGHT_ON,
	DISP_CMD_SET_BACKLIGHT_OFF,
	DISP_CMD_SET_BACKLIGHT_VALUE,
	DISP_CMD_GET_FRAME_RATE,
	DISP_CMD_SET_DC_BACKCOLOR,

	DISP_CMD_ENABLE_DC_COLOR_ENHANCEMENT,
	DISP_CMD_DISABLE_DC_COLOR_ENHANCEMENT,
	DISP_CMD_SET_DC_BRIGHTNESS,
	DISP_CMD_SET_DC_SATURATION,
	DISP_CMD_SET_DC_CONTRAST,

	DISP_CMD_SET_DC_WIN_COLOR_KEY,
	DISP_CMD_ENABLE_DC_WIN_COLOR_KEY,
	DISP_CMD_DISABLE_DC_WIN_COLOR_KEY,

	DISP_CMD_SET_DC_WIN_ALPHA_MODE,
	DISP_CMD_SET_DC_WIN_ALPHA_VALUE,

	DISP_CMD_ENABLE_DC_WIN,
	DISP_CMD_DISABLE_DC_WIN,

	DISP_CMD_ENABLE_DC_BLOCKLINKER,
	DISP_CMD_DISABLE_DC_BLOCKLINKER,
	DISP_CMD_SET_DC_BLOCKLINKER_ALPHA_MODE,
	DISP_CMD_SET_DC_BLOCKLINKER_ALPHA_VALUE,

	DISP_CMD_ENABLE_DC_HWC,
	DISP_CMD_DISABLE_DC_HWC,

	DISP_CMD_ENABLE_HW_ROT,
	DISP_CMD_DISABLE_HW_ROT,

	DISP_CMD_ENABLE_DIT,
	DISP_CMD_DISABLE_DIT
} disp_cmd_t;

typedef struct dpu_dc_device {
	u32			screen_w;
	u32			screen_h;
	bool			hwc_enable;
	dpu_dc_hwc_size_t	hwc_size_format;
} dpu_dc_device_t;

typedef struct dpu_dc_buf {
	u8	*addr;
	u32	size;
} dpu_dc_buf_t;

typedef struct dpu_se_device {
	dpu_dc_win_index_t	win_index; /* se output windows*/
} dpu_se_device_t;

typedef struct dpu_device {
	dpu_dc_index_t		dc_index;
	dpu_dc_device_t		dpu_dc_dev;
} dpu_device_t;

typedef struct disp_io_ctrl {
	u32 dc_index; /* dc index, must config */
	disp_ctl_t *dctl; /* window handle */
	void *args; /* pointer of parameters */
} disp_io_ctrl_t;

typedef struct rt_device_disp_ops {
	disp_ctl_t* (*disp_win_request)(const char *name);
	int (*disp_win_release)(disp_ctl_t **dctl);
	int (*disp_set_win_layer)(disp_ctl_t *dctl, win_layer_t layer);

	int (*disp_win_update)(disp_ctl_t *dctl, dc_win_data_t *win_data_p);
	int (*disp_bkl_update)(void);
	int (*disp_hwc_update)(int32_t x, int32_t y);

	disp_se_t* (*disp_se_request)(void);
	int (*disp_se_release)(disp_se_t **dse_t);
	int (*disp_se_process)(disp_se_t *dse, dc_win_data_t *se_data,
				u32 out_addr[3], u32 out_format);

	int (*disp_dc_page_flip_start)(void);
	int (*disp_dc_page_flip_stop)(void);

	int (*disp_rot_process)(disp_rot_cfg_t *cfgs);
	int (*disp_dit_process)(disp_dit_cfg_t *cfgs);

	disp_ctl_t* (*disp_bkl_item_request)(const char *name);
	int (*disp_bkl_item_release)(disp_ctl_t **dctl_t);
	int (*disp_bkl_item_config)(disp_ctl_t *dctl, dc_win_data_t *win_data);
	int (*disp_bkl_list_add_head)(disp_ctl_t *dctl);
	int (*disp_bkl_list_add_tail)(disp_ctl_t *dctl);
	int (*disp_bkl_list_insert_before)(disp_ctl_t *bkl_insert, disp_ctl_t *bkl_exist);
	int (*disp_bkl_list_insert_after)(disp_ctl_t *bkl_insert, disp_ctl_t *bkl_exist);
	int (*disp_bkl_list_rm)(disp_ctl_t *dctl);
} rt_device_disp_ops_t;
#define rt_disp_ops(device)          ((struct rt_device_disp_ops *)(device->user_data))

dpu_device_t *get_disp_dpu_dev(void);
rt_tick_t disp_get_tick(void);
#if 0
int disp_init(void);
int disp_uninit(void);
#endif

#endif /* __LOMBO_DISP_H__ */
