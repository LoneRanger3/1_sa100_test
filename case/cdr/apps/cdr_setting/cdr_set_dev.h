/*
 * system_setting_dev.h - system dev of setting code for LomboTech
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

#ifndef __SYSTEM_SET_DEV_H__
#define __SYSTEM_SET_DEV_H__

#include "cdr_setting_cfg.h"

extern cdr_config_t cdr_config;
#define FORMAT_OPE_PRIO 28
#define FORMAT_OPE_SIZE 4096
#define FORMAT_REF_PRIO 28
#define FORMAT_REF_SIZE 4096

#define FORMAT_CARD_SUCCEED 20
#define FORMAT_CARD_FAILED 21
#define FORMAT_NO_CARD 8
#define FORMAT_OTHER_ERR 3

typedef enum {
	FMT_CARD_SUCC = 0,
	FMT_CARD_FAIL = -1,
	FMT_NO_CARD = -2,
	FMT_OTHER_ERR = -3,
} fmt_err_t;

typedef enum _gsensor_sensity_e_ {
	SENSITY_CLOSE = 0,
	HIGH_SENSITY = 1,
	MID_SENSITY = 3,
	LOW_SENSITY = 4,
	SENSITY_MAX
} gsensor_sensity_e;

typedef struct tag_dt {
	lb_int32 year;
	lb_int32 month;
	lb_int32 day;
	lb_int32 hour;
	lb_int32 minute;
	lb_int32 second;
} dt_t;

lb_int32 loop_record_idx_to_string(lb_int32 idx, void **string);
lb_int32 set_loop_record_string(void *string);
lb_int32 loop_record_string_to_idx(void *string, lb_int32 *idx);
lb_int32 get_loop_record_string(void **string);
lb_int32 record_reso_idx_to_string(lb_int32 idx, void **string);
lb_int32 set_record_reso_string(void *string);
lb_int32 record_reso_string_to_idx(void *string, lb_int32 *idx);
lb_int32 get_record_reso_string(void **string);
lb_int32 set_record_mute_sw(lb_int32 sw);
lb_int32 get_record_mute_sw(lb_int32 *sw);
lb_int32 set_watermark_enable_sw(lb_int32 sw);
lb_int32 get_watermark_enable_sw(lb_int32 *sw);
lb_int32 set_watermark_logo_enable_sw(lb_int32 sw);
lb_int32 get_watermark_logo_enable_sw(lb_int32 *sw);
lb_int32 set_park_monitor_enable_sw(lb_int32 sw);
lb_int32 get_park_monitor_enable_sw(lb_int32 *sw);
lb_int32 set_interval_enable_sw(lb_int32 sw);
lb_int32 get_interval_enable_sw(lb_int32 *sw);

lb_int32 set_loop_record_idx(lb_int32 idx);
lb_int32 get_loop_record_idx(lb_int32 *idx);
lb_int32 set_record_reso_idx(lb_int32 idx);
lb_int32 get_record_reso_idx(lb_int32 *idx);
lb_int32 set_gsensor_sens_idx(lb_int32 idx);
lb_int32 get_gsensor_sens_idx(lb_int32 *idx);
lb_int32 format_sd_card(void);
lb_int32 restore_factory_setting(void);
lb_int32 backlight_idx_to_value(lb_uint32 idx, lb_uint32 *value);
lb_int32 set_backlight_value(lb_uint32 value);
lb_int32 backlight_value_to_idx(lb_uint32 value, lb_uint32 *idx);
lb_int32 get_backlight_value(lb_uint32 *value);



#endif /* __SYSTEM_SET_DEV_H__ */
