/*
 * home.h - desktop app head file
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

#ifndef __HOME_H__
#define __HOME_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "app_manage.h"
#include "case_config.h"
#include "lb_types.h"
#include "case_common.h"

#define APP_ENTER_BACKGROUND 1
#define APP_SET_ATTR 2
#define GET_SD_STATUS 10
#define GET_AV_STATUS 11
#define GET_BACK_STATUS 12
#define GET_ACC_STATUS 13
#define MSG_GET_REC_OBJ 14

#define MSG_FROM_SYS_SET 0x100
#define MSG_FROM_CDR_SET 0x200
#define MSG_FROM_LOGO_HIDDEN 0x300
#define MSG_FROM_CLICK 0x400



#define MSG_FROM_HIDE_HBTB 0x800
#define MSG_FROM_SHOW_HBTB 0x900

extern int g_upgrade_flag;
extern record_obj_t record_obj;

#ifdef __cplusplus
}
#endif

#endif /* __HOME_H__ */
