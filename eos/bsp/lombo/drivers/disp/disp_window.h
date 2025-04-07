/*
 * disp_windows.h - Lombo disp window module head file
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
#ifndef __LOMBO_WINDOWS_H__
#define __LOMBO_WINDOWS_H__
#include "disp_list.h"

typedef enum tag_win_layer {
	WIN_LAYER_TOP		= 0,
	WIN_LAYER_BOTTOM	= 1,
} win_layer_t;

u32 disp_win_sync_dctx(void);
u32 disp_win_sync_list(void);
u32 disp_win_sync_win_src(void);

int disp_win_get_list_index_by_name(const char *name, u8 *disp_idx);
int disp_win_get_list_index(disp_ctl_t *dctl, u8 *disp_idx);
int disp_win_get_list_num(void);
struct disp_ctl *disp_win_get_list_item(int number);
void disp_win_printf_list_item_name(void);

void disp_win_list_init(void);
void disp_win_list_uninit(void);

#if 0
int disp_win_item_config(disp_ctl_t *dctl, dc_win_data_t *win_data);
int disp_win_list_add_head(disp_ctl_t *dctl);
int disp_win_list_add_tail(disp_ctl_t *dctl);
int disp_win_list_insert_before(disp_ctl_t *win_insert, disp_ctl_t *win_exist);
int disp_win_list_insert_after(disp_ctl_t *win_insert, disp_ctl_t *win_exist);
int disp_win_list_rm(disp_ctl_t *dctl);
#endif

disp_ctl_t *disp_win_request(const char *name);
disp_ctl_t *disp_win_request_with_se(const char *name);
int disp_win_release(disp_ctl_t **dctl_t);
void disp_win_para_update(void);
int disp_set_win_layer(disp_ctl_t *dctl, win_layer_t layer);

bool disp_win_check_para(dc_win_data_t *win_data);

#endif /* __LOMBO_WINDOWS_H__ */

