/*
 * disp_se.h - Lombo disp se module head file
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
#ifndef __DISP_SE_H__
#define __DISP_SE_H__

#include "disp_list.h"

disp_se_t *disp_get_win_se(disp_ctl_t *dctl);
int disp_win_request_se(disp_ctl_t *dctl);
int disp_win_release_se(disp_ctl_t *dctl, bool is_release);
int disp_win_se_release_do(disp_se_t *dse);

disp_se_t *disp_se_request(void);
int disp_se_release(disp_se_t **dse_t);
int disp_se_process(disp_se_t *dse, dc_win_data_t *se_data,
			u32 out_addr[3], u32 out_format);

#endif /* __DISP_SE_H__ */
