/*
 * main_view.h - main view code for LomboTech
 * main view interface and macro define
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

#ifndef __MAIN_VIEW_H__
#define __MAIN_VIEW_H__

#include "app_manage.h"
#include <debug.h>

#define ADAS_SW (LB_MSG_SMART_DRIVE_BASE|0x00)
#define PANO_SW (LB_MSG_SMART_DRIVE_BASE|0x01)
#define BSD_SW (LB_MSG_SMART_DRIVE_BASE|0x02)

#define PANO_CALI_BEGIN (LB_MSG_SMART_DRIVE_BASE|0x03)
#define PANO_SEL_MOTOR_TYPE (LB_MSG_SMART_DRIVE_BASE|0x04)
#define PANO_CAR_W_PLUS (LB_MSG_SMART_DRIVE_BASE|0x05)
#define PANO_CAR_W_MINUS (LB_MSG_SMART_DRIVE_BASE|0x06)
#define PANO_CAR_L_PLUS (LB_MSG_SMART_DRIVE_BASE|0x07)
#define PANO_CAR_L_MINUS (LB_MSG_SMART_DRIVE_BASE|0x08)
#define PANO_CALI_LINE_UP (LB_MSG_SMART_DRIVE_BASE|0x09)
#define PANO_CALI_LINE_DOWN (LB_MSG_SMART_DRIVE_BASE|0x0a)
#define PANO_CALI_SAVE_RES (LB_MSG_SMART_DRIVE_BASE|0x0b)
#define PANO_CAR_DIST2REAR_PLUS (LB_MSG_SMART_DRIVE_BASE|0x30)
#define PANO_CAR_DIST2REAR_MINUS (LB_MSG_SMART_DRIVE_BASE|0x31)

#define ADAS_TIME_LIST_RESP (LB_MSG_SMART_DRIVE_BASE|0x10)
#define ADAS_TYPE_LIST_RESP (LB_MSG_SMART_DRIVE_BASE|0x11)
#define ADAS_CALI_TOP_UP (LB_MSG_SMART_DRIVE_BASE|0x14)
#define ADAS_CALI_TOP_DN (LB_MSG_SMART_DRIVE_BASE|0x15)
#define ADAS_CALI_BOT_UP (LB_MSG_SMART_DRIVE_BASE|0x16)
#define ADAS_CALI_BOT_DN (LB_MSG_SMART_DRIVE_BASE|0x17)
#define ADAS_CALI_MID_LT (LB_MSG_SMART_DRIVE_BASE|0x20)
#define ADAS_CALI_MID_RT (LB_MSG_SMART_DRIVE_BASE|0x21)
#define ADAS_CALI_SAVE_RES (LB_MSG_SMART_DRIVE_BASE|0x18)

#define BSD_CALI_TOP_UP (LB_MSG_SMART_DRIVE_BASE|0x52)
#define BSD_CALI_TOP_DN (LB_MSG_SMART_DRIVE_BASE|0x53)
#define BSD_CALI_BOT_UP (LB_MSG_SMART_DRIVE_BASE|0x54)
#define BSD_CALI_BOT_DN (LB_MSG_SMART_DRIVE_BASE|0x55)
#define BSD_CALI_MID_LT (LB_MSG_SMART_DRIVE_BASE|0x56)
#define BSD_CALI_MID_RT (LB_MSG_SMART_DRIVE_BASE|0x57)
#define BSD_CALI_PREV_PAUSE (LB_MSG_SMART_DRIVE_BASE|0x58)

#define BSD_REGION_POINT_UP (LB_MSG_SMART_DRIVE_BASE|0x60)
#define BSD_REGION_POINT_DN (LB_MSG_SMART_DRIVE_BASE|0x61)
#define BSD_REGION_POINT_LT (LB_MSG_SMART_DRIVE_BASE|0x62)
#define BSD_REGION_POINT_RT (LB_MSG_SMART_DRIVE_BASE|0x63)
#define BSD_REGION_POINT_EE (LB_MSG_SMART_DRIVE_BASE|0x64)
#define BSD_SAVE_RES (LB_MSG_SMART_DRIVE_BASE|0x65)

#define BSD_OPTI_LIST_RESP (LB_MSG_SMART_DRIVE_BASE|0x70)
#define BSD_TYPE_LIST_RESP (LB_MSG_SMART_DRIVE_BASE|0x71)


lb_int32 init_funcs(void);
lb_int32 uninit_funcs(void);
lb_int32 resp_funcs(void);
lb_int32 unresp_funcs(void);
lb_int32 load_main_view(void);
lb_int32 hide_main_view(void *para);
lb_int32 show_main_view(void *para);

#endif /* __MAIN_VIEW_H__ */
