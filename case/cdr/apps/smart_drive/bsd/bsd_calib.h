/*
 * adas_calib.h - adas calibrate code for LomboTech
 * adas calibrate interface and macro define
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

#ifndef __BSD_CALIB_H__
#define __BSD_CALIB_H__

#include "app_manage.h"
#include <debug.h>
#include "case_config.h"


#define BSD_UP_LINE_ID 200
#define BSD_ME_LINE_ID 201
#define BSD_DN_LINE_ID 202

#define BSD_DIRECT_UP 0x00
#define BSD_DIRECT_DN 0xff
#define BSD_DIRECT_LT 0x55
#define BSD_DIRECT_RT 0xaa

#if 1
#define BSD_BOT_UP_LIMIT 0
#define BSD_BOT_DN_LIMIT 320
#define BSD_TOP_LINE_UP_LIMIT (160 - 160)
#define BSD_TOP_LINE_DN_LIMIT (160 + 160)
#define BSD_MID_LINE_LT_LIMIT (284 - 284)
#define BSD_MID_LINE_RT_LIMIT (284 + 284)
#define BSD_MOVE_LINE_STEP 2
#else
#define BSD_BOT_UP_LIMIT 0
#define BSD_BOT_DN_LIMIT 440
#define BSD_TOP_LINE_UP_LIMIT (220 - 220)
#define BSD_TOP_LINE_DN_LIMIT (220 + 220)
#define BSD_MID_LINE_LT_LIMIT (390 - 390)
#define BSD_MID_LINE_RT_LIMIT (390 + 390)
#define BSD_MOVE_LINE_STEP 2
#endif

#define BSD_MOVE_POINT_STEP 2

lb_int32 bsd_calib_init_funcs(void);
lb_int32 bsd_calib_uninit_funcs(void);
lb_int32 bsd_calib_resp_funcs(void);
lb_int32 bsd_calib_unresp_funcs(void);

#endif /* __BSD_CALIB_H__ */
