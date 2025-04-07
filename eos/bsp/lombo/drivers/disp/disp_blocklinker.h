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
#ifndef __LOMBO_BLOCKLINKER_H__
#define __LOMBO_BLOCKLINKER_H__
#include <list.h>
#include "disp_list.h"

u32 disp_bkl_sync_dctx(void);
int disp_bkl_get_list_index_by_name(const char *name, u8 *disp_idx);
int disp_bkl_get_list_index(disp_ctl_t *dctl, u8 *disp_idx);
void disp_bkl_printf_list_item_name(void);
void disp_bkl_list_init(void);
void disp_bkl_list_uninit(void);
disp_ctl_t *disp_bkl_item_request(const char *name);
int disp_bkl_item_release(disp_ctl_t **dctl_t);
int disp_bkl_item_config(disp_ctl_t *dctl, dc_win_data_t *win_data);
int disp_bkl_list_add_head(disp_ctl_t *dctl);
int disp_bkl_list_add_tail(disp_ctl_t *dctl);
int disp_bkl_list_insert_before(disp_ctl_t *bkl_insert, disp_ctl_t *bkl_exist);
int disp_bkl_list_insert_after(disp_ctl_t *bkl_insert, disp_ctl_t *bkl_exist);
int disp_bkl_list_rm(disp_ctl_t *dctl);

#endif /* __LOMBO_BLOCKLINKER_H__ */
