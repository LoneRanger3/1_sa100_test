/*
 * system_set_devl.h - system dev of setting code for LomboTech
 * system dev of setting interface and macro define
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

#include "system_setting_cfg.h"

#define FORMAT_OPE_PRIO 28
#define FORMAT_OPE_SIZE 4096
#define FORMAT_REF_PRIO 28
#define FORMAT_REF_SIZE 4096

#define FORMAT_CARD_SUCCEED 20
#define FORMAT_CARD_FAILED 21
#define FORMAT_NO_CARD 8
#define FORMAT_OTHER_ERR 3

typedef struct tag_dt {
	lb_int32 year;
	lb_int32 month;
	lb_int32 day;
	lb_int32 hour;
	lb_int32 minute;
	lb_int32 second;
} dt_t;

typedef enum {
	FMT_CARD_SUCC = 0,
	FMT_CARD_FAIL = -1,
	FMT_NO_CARD = -2,
	FMT_OTHER_ERR = -3,
} fmt_err_t;

extern cdr_config_t cdr_config;

lb_int32 set_version(char *version);
lb_int32 get_version(char *version);
lb_int32 set_date_time(void);
lb_int32 get_date_time(void);
lb_int32 dt_idx_to_ymd(lb_int32 y, lb_int32 m, lb_int32 d);
lb_int32 dt_idx_to_hms(lb_int32 h, lb_int32 m, lb_int32 s);
lb_int32 set_keytone_sw(lb_int8 sw);
lb_int32 get_keytone_sw(lb_int8 *sw);
lb_int32 set_fastboot_sw(lb_int32 sw);
lb_int32 get_fastboot_sw(lb_int32 *sw);
lb_int32 get_rearmirr_sw(lb_int32 *sw);
lb_int32 set_rearmirr_sw(lb_int32 sw);
lb_int32 get_accpower_sw(lb_int32 *sw);
lb_int32 set_accpower_sw(lb_int32 sw);
lb_int32 language_idx_to_string(lb_uint32 idx, void **string);
lb_int32 set_language_string(void *string);
lb_int32 language_string_to_idx(void *string, lb_uint32 *idx);
lb_int32 get_language_string(void **string);
lb_int32 standby_idx_to_value(lb_uint32 idx, lb_uint32 *value);
lb_int32 set_standby_value(lb_uint32 value);
lb_int32 standby_value_to_idx(lb_uint32 value, lb_uint32 *idx);
lb_int32 get_standby_value(lb_uint32 *value);
lb_int32 volume_idx_to_value(lb_uint32 idx, lb_uint32 *value);
lb_int32 set_volume_value(lb_uint32 value);
lb_int32 volume_value_to_idx(lb_uint32 value, lb_uint32 *idx);
lb_int32 get_volume_value(lb_uint32 *value);
lb_int32 backlight_idx_to_value(lb_uint32 idx, lb_uint32 *value);
lb_int32 set_backlight_value(lb_uint32 value);
lb_int32 backlight_value_to_idx(lb_uint32 value, lb_uint32 *idx);
lb_int32 get_backlight_value(lb_uint32 *value);
lb_uint8 get_leap_month(lb_uint8 *change_falg);
lb_int32 update_day_num(void);
lb_int32 set_language_idx(lb_uint32 idx);
lb_int32 get_language_idx(lb_uint32 *idx);
lb_int32 restore_factory_setting(void);
lb_int32 format_sd_card(void);

#endif /* __SYSTEM_SET_DEV_H__ */
