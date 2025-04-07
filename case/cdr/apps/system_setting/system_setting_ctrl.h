/*
 * system_setting_ctrl.h - system control of setting code for LomboTech
 * system control of setting interface and macro define
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

#ifndef __SYSTEM_SETTING_CTRL_H__
#define __SYSTEM_SETTING_CTRL_H__

#define LB_MSG_SYSCFG_BRIGHTEN		(LB_MSG_SYSTEM_SETTING_BASE|0x00)
#define LB_MSG_SYSCFG_VOLUME		(LB_MSG_SYSTEM_SETTING_BASE|0x01)
#define LB_MSG_SYSCFG_STANDBY		(LB_MSG_SYSTEM_SETTING_BASE|0x02)
#define LB_MSG_SYSCFG_KEYTONE		(LB_MSG_SYSTEM_SETTING_BASE|0x03)
#define LB_MSG_SYSCFG_LANGUAGE		(LB_MSG_SYSTEM_SETTING_BASE|0x04)
#define LB_MSG_SYSCFG_DTIME		(LB_MSG_SYSTEM_SETTING_BASE|0x05)
#define LB_MSG_SYSCFG_FORMAT		(LB_MSG_SYSTEM_SETTING_BASE|0x06)
#define LB_MSG_SYSCFG_VERSION		(LB_MSG_SYSTEM_SETTING_BASE|0x07)
#define LB_MSG_SYSCFG_FACTORY		(LB_MSG_SYSTEM_SETTING_BASE|0x08)
#define LB_MSG_SYSCFG_FASTBOOT		(LB_MSG_SYSTEM_SETTING_BASE|0x09)
#define LB_MSG_SYSCFG_REARMIR		(LB_MSG_SYSTEM_SETTING_BASE|0x0A)
#define LB_MSG_SYSCFG_ACCPOWER		(LB_MSG_SYSTEM_SETTING_BASE|0x0B)

#define LB_MSG_SEL_YEAR			(LB_MSG_SYSTEM_SETTING_BASE|0x30)
#define LB_MSG_SEL_MONTH		(LB_MSG_SYSTEM_SETTING_BASE|0x31)
#define LB_MSG_SEL_DAY			(LB_MSG_SYSTEM_SETTING_BASE|0x32)
#define LB_MSG_SEL_HOUR			(LB_MSG_SYSTEM_SETTING_BASE|0x33)
#define LB_MSG_SEL_MINTUE		(LB_MSG_SYSTEM_SETTING_BASE|0x34)

lb_int32 init_funcs(void);
lb_int32 uninit_funcs(void);
lb_int32 resp_funcs(void);
lb_int32 unresp_funcs(void);

#endif /* __SYSTEM_SETTING_CTRL_H__ */
