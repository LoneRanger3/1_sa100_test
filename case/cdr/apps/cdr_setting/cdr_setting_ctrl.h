/*
 * cdr_setting_ctrl.h - cdr control of setting code for LomboTech
 * cdr control of setting interface and macro define
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

#ifndef __CDR_SETTING_CTRL_H__
#define __CDR_SETTING_CTRL_H__

#define RECORD_LOOP			(LB_MSG_CDR_SETTING_BASE|0x00)
#define RECORD_RESO			(LB_MSG_CDR_SETTING_BASE|0x01)
#define RECORD_MUTE			(LB_MSG_CDR_SETTING_BASE|0x02)
#define RECORD_WATERMARK		(LB_MSG_CDR_SETTING_BASE|0x03)
#define RECORD_WATERMARK_LOGO		(LB_MSG_CDR_SETTING_BASE|0x04)
#define RECORD_PARK_MONITOR		(LB_MSG_CDR_SETTING_BASE|0x05)
#define RECORD_GSENSOR_SENSITY	(LB_MSG_CDR_SETTING_BASE|0x06)
#define RECORD_TONE 		(LB_MSG_CDR_SETTING_BASE|0x07)
#define LCD_BRIGHT		    (LB_MSG_CDR_SETTING_BASE|0x12)
#define LANGUAGE		    (LB_MSG_CDR_SETTING_BASE|0x13)
#define WARN_TONE		    (LB_MSG_CDR_SETTING_BASE|0x22)

#define MSG_SEL_YEAR		 (LB_MSG_CDR_SETTING_BASE|0x30)
#define MSG_SEL_MONTH		 (LB_MSG_CDR_SETTING_BASE|0x31)
#define MSG_SEL_DAY		     (LB_MSG_CDR_SETTING_BASE|0x32)
#define MSG_SEL_HOUR		 (LB_MSG_CDR_SETTING_BASE|0x33)
#define MSG_SEL_MINTUE		 (LB_MSG_CDR_SETTING_BASE|0x34)




lb_int32 init_funcs(void);
lb_int32 uninit_funcs(void);
lb_int32 resp_funcs(void);
lb_int32 unresp_funcs(void);

#endif /* __CDR_SETTING_CTRL_H__ */

