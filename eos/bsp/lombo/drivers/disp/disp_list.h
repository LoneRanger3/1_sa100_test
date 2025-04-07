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
#ifndef __DISP_LIST_H__
#define __DISP_LIST_H__
#include "lombo_dpu.h"

u32 disp_sync_dctx(disp_list_index_t index);
u32 disp_sync_list(disp_list_index_t index);
u32 disp_sync_win_src(disp_list_index_t index);
u32 disp_store_win_para(disp_list_index_t index);
u32 disp_sync_win_para(disp_list_index_t index);
int disp_get_list_index_by_name(disp_list_index_t index,
				const char *name, u8 *disp_idx);
int disp_get_list_index(disp_list_index_t index, disp_ctl_t *dctl, u8 *disp_idx);
int disp_get_list_item_num(disp_list_index_t index);
struct disp_ctl *disp_get_list_item(disp_list_index_t index, int number);
void disp_printf_list_item_name(disp_list_index_t index);
void disp_list_init(disp_list_index_t index);
void disp_list_uninit(disp_list_index_t index);
disp_ctl_t *disp_item_request(disp_list_index_t index, const char *name);
int disp_item_release(disp_list_index_t index, disp_ctl_t *dctl);
int disp_item_config(disp_list_index_t index,
			disp_ctl_t *dctl, dc_win_data_t *win_data);
int disp_list_add_head(disp_list_index_t index, disp_ctl_t *dctl);
int disp_list_add_tail(disp_list_index_t index, disp_ctl_t *dctl);
int disp_list_insert_before(disp_list_index_t index, disp_ctl_t *disp_insert,
				disp_ctl_t *disp_exist);
int disp_list_insert_after(disp_list_index_t index, disp_ctl_t *disp_insert,
				disp_ctl_t *disp_exist);
int disp_list_rm(disp_list_index_t index, disp_ctl_t *dctl);

#endif /* __DISP_LIST_H__ */
