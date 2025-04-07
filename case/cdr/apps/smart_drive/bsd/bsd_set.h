/*
 * bsd_set.h - bsd setting code for LomboTech
 * bsd setting interface and macro define
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

#ifndef __BSD_SET_H__
#define __BSD_SET_H__

#include "app_manage.h"
#include <debug.h>

lb_int32 bsd_set_init_funcs(void);
lb_int32 bsd_set_uninit_funcs(void);
lb_int32 bsd_set_resp_funcs(void);
lb_int32 bsd_set_unresp_funcs(void);

#endif /* __BSD_SET_H__ */
