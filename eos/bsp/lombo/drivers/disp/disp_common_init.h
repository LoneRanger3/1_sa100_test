/*
 * disp_common_init.h - Lombo disp  common init module common head file
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
#ifndef __DISP_COMMON_INIT_H_
#define __DISP_COMMON_INIT_H_

int disp_common_init(bool is_resume);
int disp_common_uninit(bool is_suspend);
void disp_fun_lock_init(void);
void disp_fun_lock(void);
void disp_fun_unlock(void);
void disp_dc_lock(void);
void disp_dc_unlock(void);
void disp_rot_lock(rot_way_t way);
void disp_rot_unlock(rot_way_t way);
void disp_dit_lock(void);
void disp_dit_unlock(void);
void disp_update_reg_lock(void);
void disp_update_reg_unlock(void);

void set_disp_start_flag(bool flag);
bool is_disp_started(void);
void set_disp_frame_rate(u32 val);
u32 get_disp_frame_rate(void);
dc_context_t *get_disp_dc_ctx(void);
void set_disp_power_sta(disp_power_sta_t sta);
disp_power_sta_t get_disp_power_sta(void);
void set_disp_page_flip_sta(bool sta);
bool get_disp_page_flip_sta(void);
disp_se_t *get_disp_se_dev(void);
rt_mq_t get_disp_mq(void);
rt_event_t get_disp_event(void);
rt_event_t get_vbank_event(void);

int disp_rot_init(void);
int disp_rot_uninit(void);
int disp_dit_init(void);
int disp_dit_uninit(void);

#endif /* __DISP_COMMON_INIT_H_ */
