/*
 * system_setting_dev.c - system dev of setting code for LomboTech
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rtthread.h>
#include <app_manage.h>
#include "cJSON.h"
#include "da380_gsensor.h"

#include <drivers/rtc.h>
#include <time.h>
#include "rtc_csp.h"

#include "lombo_disp.h"



#include "lb_types.h"
#include "lb_common.h"
#include "cdr_set_dev.h"
#include "cdr_setting_ctrl.h"
#include "power_drv.h"
#include "lb_gal_common.h"


static pthread_t format_ope_id;
static pthread_t format_ref_id;
static rt_device_t disp_device;

static dt_t local_dt = {2025, 1, 1, 0, 0, 0}; //2019
static lb_uint8 roller_day_flag;
static char options[128];
const char *day_options;

static lb_int32 format_ope_init(void);
static lb_int32 format_ope_exit(void);
static lb_int32 need_exit;



const char day_str[31][32] = {
	"STR_SETTINGS_DTIME_01",
	"STR_SETTINGS_DTIME_02",
	"STR_SETTINGS_DTIME_03",
	"STR_SETTINGS_DTIME_04",
	"STR_SETTINGS_DTIME_05",
	"STR_SETTINGS_DTIME_06",
	"STR_SETTINGS_DTIME_07",
	"STR_SETTINGS_DTIME_08",
	"STR_SETTINGS_DTIME_09",
	"STR_SETTINGS_DTIME_10",
	"STR_SETTINGS_DTIME_11",
	"STR_SETTINGS_DTIME_12",
	"STR_SETTINGS_DTIME_13",
	"STR_SETTINGS_DTIME_14",
	"STR_SETTINGS_DTIME_15",
	"STR_SETTINGS_DTIME_16",
	"STR_SETTINGS_DTIME_17",
	"STR_SETTINGS_DTIME_18",
	"STR_SETTINGS_DTIME_19",
	"STR_SETTINGS_DTIME_20",
	"STR_SETTINGS_DTIME_21",
	"STR_SETTINGS_DTIME_22",
	"STR_SETTINGS_DTIME_23",
	"STR_SETTINGS_DTIME_24",
	"STR_SETTINGS_DTIME_25",
	"STR_SETTINGS_DTIME_26",
	"STR_SETTINGS_DTIME_27",
	"STR_SETTINGS_DTIME_28",
	"STR_SETTINGS_DTIME_29",
	"STR_SETTINGS_DTIME_30",
	"STR_SETTINGS_DTIME_31",
};



lb_int32 dt_idx_to_ymd(lb_int32 y, lb_int32 m, lb_int32 d)
{
	lb_int32 ret = 0;

	if (y >= 0)
		local_dt.year = y + 2025;//2019
	if (m >= 0)
		local_dt.month = m + 1;
	if (d >= 0)
		local_dt.day = d + 1;

	return ret;
}

lb_int32 dt_idx_to_hms(lb_int32 h, lb_int32 m, lb_int32 s)
{
	lb_int32 ret = 0;

	if (h >= 0)
		local_dt.hour = h;
	if (m >= 0)
		local_dt.minute = m;
	if (s >= 0)
		local_dt.second = 0;

	return ret;
}

lb_uint8 get_leap_month(lb_uint8 *change_falg)
{
	lb_uint8 flag = 0, day_change = 0;

	if (2 == local_dt.month) {
		if (((local_dt.year % 4 == 0) && (local_dt.year % 100 != 0))
			|| (local_dt.year % 400 == 0)) {
			if (29 < local_dt.day) {
				local_dt.day = 1;
				day_change = 1;
			}
			flag = 0;
		} else {
			if (28 < local_dt.day) {
				local_dt.day = 1;
				day_change = 1;
			}
			flag = 1;
		}
	} else if (1 == local_dt.month || 3 == local_dt.month || 5 == local_dt.month
		|| 7 == local_dt.month || 8 == local_dt.month || 10 == local_dt.month
		|| 12 == local_dt.month) {
		flag = 2;
	} else if (4 == local_dt.month || 6 == local_dt.month || 9 == local_dt.month
		|| 11 == local_dt.month) {
		if (30 < local_dt.day) {
			local_dt.day = 1;
			day_change = 1;
		}
		flag = 3;
	}

	*change_falg = day_change;
	return flag;
}

/**
 * update_day_num - update number of days
 *
 * This function use to update number of days per month
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 update_day_num(void)
{
	lb_int32 ret = 0, cur_pos = 0;
	lb_uint8 flag = 0, day_change = 0, option_num = 0, i;
	static lb_roller_selected_t roller_selected;
	void *obj = NULL;

	flag = get_leap_month(&day_change);
	ret = lb_view_get_obj_ext_by_id(206, &obj);
	if (1 == day_change) {
		roller_selected.anim_en = false;
		roller_selected.sel_opt = 0;
		ret = lb_gal_update_roller(obj, LB_ROLLER_UPD_SELECTED,
					0, &roller_selected);
	}
	if (flag != roller_day_flag) {
		roller_day_flag = flag;
		if (0 == roller_day_flag)
			option_num = 29;
		else if (1 == roller_day_flag)
			option_num = 28;
		else if (2 == roller_day_flag)
			option_num = 31;
		else if (3 == roller_day_flag)
			option_num = 30;

		memset(options, 0x00, sizeof(options));
		for (i = 0; i < option_num; i++) {
			day_options = elang_get_utf8_string_josn(day_str[i]);
			memcpy(&options[cur_pos], day_options, strlen(day_options));
			cur_pos += strlen(day_options);
			options[cur_pos] = '\n';
			cur_pos += 1;
		}
		cur_pos -= 1;
		options[cur_pos] = '\0';
		ret = lb_gal_update_roller(obj, LB_ROLLER_UPD_OPTIONS, 0, options);
	}

	return ret;
}



/**
 * get_date_time - set date and time to rtc
 * @param: lb_obj_t object pointer.
 *
 * This function set date and time to rtc
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 set_date_time(void)
{
	lb_int32 ret = 0;
	struct tm *p_tm; /* time variable */
	time_t now;

	printf("write:year:%d, month:%d, day:%d, hour:%d, minute:%d\n",
		local_dt.year, local_dt.month, local_dt.day,
		local_dt.hour, local_dt.minute);

	lb_gal_set_screen_standby_enable(false);
	/* set dt to rtc */
	ret = set_date(local_dt.year, local_dt.month, local_dt.day);
	if (ret != 0) {
		APP_LOG_W("[%s,%d]\n", __FILE__, __LINE__);
		ret = -1;
		goto exit;
	}
	rt_thread_delay(10);
	/* set dt to rtc */
	ret = set_time(local_dt.hour, local_dt.minute, 0);
	if (ret != 0) {
		APP_LOG_W("[%s,%d]\n", __FILE__, __LINE__);
		ret = -1;
		goto exit;
	}
	rt_thread_delay(10);
	lb_gal_set_screen_standby_enable(true);
	now = time(RT_NULL);
	p_tm = localtime(&now);

	printf("read:year:%d, month:%d, day:%d, hour:%d, minute:%d\n",
		(p_tm->tm_year + 1900), (p_tm->tm_mon + 1), p_tm->tm_mday,
		p_tm->tm_hour, p_tm->tm_min);

exit:
	return ret;
}

/**
 * get_date_time - get date and time from rtc
 * @param: lb_obj_t object pointer.
 *
 * This function get date and time from rtc
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 get_date_time(void)
{
	lb_int32 ret = 0;
	struct tm *p_tm; /* time variable */
	time_t now;
	lb_uint8 day_change = 0;

	now = time(RT_NULL);
	p_tm = localtime(&now);

	printf("p_tm->tm_sec:%d, p_tm->tm_min:%d, p_tm->tm_hour:%d\n",
		p_tm->tm_sec, p_tm->tm_min, p_tm->tm_hour);
	printf("p_tm->tm_mday:%d, p_tm->tm_mon:%d, p_tm->tm_year:%d\n",
		p_tm->tm_mday, p_tm->tm_mon, p_tm->tm_year);

	/* get data and time from rtc */
	local_dt.year = (p_tm->tm_year + 1900);
	local_dt.month = (p_tm->tm_mon + 1);
	local_dt.day = p_tm->tm_mday;
	local_dt.hour = p_tm->tm_hour;
	local_dt.minute = p_tm->tm_min;
	local_dt.second = p_tm->tm_sec;

	roller_day_flag = get_leap_month(&day_change);

	return ret;
}


#if 0
lb_int32 loop_record_idx_to_string(lb_int32 idx, void **string)
{
	lb_int32 ret = -1;
	lb_int8 *str = NULL;

	if (idx == 0) {
		str = malloc(strlen("1 minute") + 1);
		strcpy((char *)str, "1 minute");
	} else if (idx == 1) {
		str = malloc(strlen("2 minutes") + 1);
		strcpy((char *)str, "2 minutes");
	}
	*string = str;

	if (cdr_config.record.record_duration_json->valuestring) {
		free(cdr_config.record.record_duration_json->valuestring);
		cdr_config.record.record_duration_json->valuestring = NULL;
	}

	ret = 0;
	return ret;
}

lb_int32 set_loop_record_string(void *string)
{
	lb_int32 ret = -1;

	/* set string to json */
	cdr_config.record.record_duration_json->valuestring = string;
	printf("[%s,%d,string:%s]\n", __FILE__, __LINE__, (char *)string);

	ret = 0;
	return ret;
}

lb_int32 loop_record_string_to_idx(void *string, lb_int32 *idx)
{
	lb_int32 ret = -1;

	if (strcmp(string, "1 minute") == 0)
		*idx = 0;
	else if (strcmp(string, "2 minutes") == 0)
		*idx = 1;

	ret = 0;
	return ret;
}

lb_int32 get_loop_record_string(void **string)
{
	lb_int32 ret = -1;

	/* get string from json */
	*string = cdr_config.record.record_duration_json->valuestring;
	printf("[%s,%d,*string:%s]\n", __FILE__, __LINE__, (char *)*string);

	ret  = 0;
	return ret;
}
#else

/**
 * set_loop_record_idx - set loop record idx to config
 * @param: lb_obj_t object pointer.
 *
 * This function set loop record idx to config
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 set_loop_record_idx(lb_int32 idx)
{
	lb_int32 ret = 0;
	lb_int32 value = 0;

	/* this pointer must be exist when the program runs here */
	RT_ASSERT(cdr_config.record.record_duration_json != NULL);

	/* set xxx to json */
	if (idx == 0) {
		value = 60;
		cdr_config.record.record_duration_json->valueint = value;
		cdr_config.record.record_duration_json->valuedouble = value;
	} else if (idx == 1) {
		value = 120;
		cdr_config.record.record_duration_json->valueint = value;
		cdr_config.record.record_duration_json->valuedouble = value;
	} else {
		printf("[%s,%d]param is need to check\n", __FILE__, __LINE__);
		ret = -1;
	}

	return ret;
}

/**
 * get_loop_record_idx - get loop record idx from config
 * @param: lb_obj_t object pointer.
 *
 * This function get loop record idx from config
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 get_loop_record_idx(lb_int32 *idx)
{
	lb_int32 ret = 0;
	lb_int32 value = 0;

	/* this pointer must be exist when the program runs here */
	RT_ASSERT(cdr_config.record.record_duration_json != NULL);
	/* get xxx from json */
	value = cdr_config.record.record_duration_json->valueint;

	if (idx == NULL) {
		printf("[%s,%d]param is need to check\n", __FILE__, __LINE__);
		ret = -1;
		return ret;
	}

	if (value == 60)
		*idx = 0;
	else if (value == 120)
		*idx = 1;
	else
		*idx = -1;

	if (*idx != 0 && *idx != 1) {
		printf("[%s,%d]param is need to check\n", __FILE__, __LINE__);
		ret = -1;
		return ret;
	}

	return ret;
}
#endif

#if 0
lb_int32 record_reso_idx_to_string(lb_int32 idx, void **string)
{
	lb_int32 ret = -1;
	lb_int8 *str = NULL;

	if (idx == 0) {
		str = malloc(strlen("720 p") + 1);
		strcpy((char *)str, "720 p");
	} else if (idx == 1) {
		str = malloc(strlen("1080 p") + 1);
		strcpy((char *)str, "1080 p");
	}
	*string = str;

	if (cdr_config.record.record_resolution_json->valuestring) {
		free(cdr_config.record.record_resolution_json->valuestring);
		cdr_config.record.record_resolution_json->valuestring = NULL;
	}

	ret = 0;
	return ret;
}

lb_int32 set_record_reso_string(void *string)
{
	lb_int32 ret = -1;

	/* set string to json */
	cdr_config.record.record_resolution_json->valuestring = string;
	printf("[%s,%d,string:%s]\n", __FILE__, __LINE__, (char *)string);

	ret = 0;
	return ret;
}

lb_int32 record_reso_string_to_idx(void *string, lb_int32 *idx)
{
	lb_int32 ret = -1;

	if (strcmp(string, "720 p") == 0)
		*idx = 0;
	else if (strcmp(string, "1080 p") == 0)
		*idx = 1;

	ret = 0;
	return ret;
}

lb_int32 get_record_reso_string(void **string)
{
	lb_int32 ret = -1;

	/* get string from json */
	*string = cdr_config.record.record_resolution_json->valuestring;
	printf("[%s,%d,*string:%s]\n", __FILE__, __LINE__, (char *)*string);

	ret  = 0;
	return ret;
}
#else
/**
 * set_record_reso_idx - set resolution idx to config
 * @param: lb_obj_t object pointer.
 *
 * This function set resolution idx to config
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 set_record_reso_idx(lb_int32 idx)
{
	lb_int32 ret = 0;

	/* this pointer must be exist when the program runs here */
	RT_ASSERT(cdr_config.record.record_resolution_json != NULL);

	/* set xxx to json */
	cdr_config.record.record_resolution_json->valueint = idx;
	cdr_config.record.record_resolution_json->valuedouble = idx;

	return ret;
}

/**
 * get_record_reso_idx - get resolution idx from config
 * @param: lb_obj_t object pointer.
 *
 * This function get resolution idx from config
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 get_record_reso_idx(lb_int32 *idx)
{
	lb_int32 ret = 0;

	/* this pointer must be exist when the program runs here */
	RT_ASSERT(cdr_config.record.record_resolution_json != NULL);

	if (idx == NULL) {
		printf("[%s,%d]param is need to check\n", __FILE__, __LINE__);
		ret = -1;
		return ret;
	}

	/* get xxx from json */
	*idx = cdr_config.record.record_resolution_json->valueint;

	return ret;
}
#endif




lb_int32 backlight_idx_to_value(lb_uint32 idx, lb_uint32 *value)
{
	lb_int32 ret = 0;

	switch (idx) {
	case 0:
		*value = 100;//50
		break;

	case 1:
		*value = 70;//70
		break;

	case 2:
		*value = 15;//80
		break;

	default:
		*value = 100;
		break;
	}

	return ret;
}

static lb_int32 set_backlight(lb_uint32 value)
{
	lb_int32 ret = 0;
	disp_io_ctrl_t dic;

	/* set value to display */
	disp_device = rt_device_find(DISP_DEVICE_NAME);
	if (disp_device != NULL) {
		rt_device_open(disp_device, 0);

		rt_memset(&dic, 0x00, sizeof(disp_io_ctrl_t));
		dic.args = &value;
		ret = rt_device_control(disp_device, DISP_CMD_SET_BACKLIGHT_VALUE, &dic);
		if (ret != 0) {
			APP_LOG_W("[%s,%d,failed]\n", __FILE__, __LINE__);
			return ret;
		}

		rt_device_close(disp_device);
	}

	return ret;
}
/**
 * set_backlight_value - set backlight to display and config
 * @param: lb_obj_t object pointer.
 *
 * This function set backlight to display and config
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 set_backlight_value(lb_uint32 value)
{
	lb_int32 ret = 0;
	lb_uint32 level = value;

	set_backlight(level);

	RT_ASSERT(cdr_config.record.lcd_brightness_json != NULL);

	/* set value to json */
	cdr_config.record.lcd_brightness_json->valueint = value;
	cdr_config.record.lcd_brightness_json->valuedouble = value;

	return ret;
}
lb_int32 backlight_value_to_idx(lb_uint32 value, lb_uint32 *idx)
{
	lb_int32 ret = 0;

	switch (value) {
	case 15:
		*idx = 0;
		break;

	case 70:
		*idx = 1;
		break;

	case 100:
		*idx = 2;
		break;

	default:
		*idx = 0;
		break;
	}

	return ret;
}

lb_int32 get_backlight_value(lb_uint32 *value)
{
	lb_int32 ret = 0;

	RT_ASSERT(cdr_config.record.lcd_brightness_json != NULL);

	/* get value from json */
	*value = cdr_config.record.lcd_brightness_json->valueint;

	return ret;
}

/**
 * set_gsensor_sens_idx - set gsensor sensity idx to config
 * @param: lb_obj_t object pointer.
 *
 * This function set gsensor sensity idx to config
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 set_gsensor_sens_idx(lb_int32 idx)
{
	lb_int32 ret = 0;
	gsensor_sensity_e sens = SENSITY_MAX;

	/* this pointer must be exist when the program runs here */
	RT_ASSERT(cdr_config.record.gsensor_sensity_json != NULL);

	/* set xxx to json */
	if (idx != 0 && idx != 1 && idx != 2 && idx != 3) {
		printf("[%s,%d]param is need to check\n", __FILE__, __LINE__);
		ret = -1;
		return ret;
	}

	cdr_config.record.gsensor_sensity_json->valueint = idx;
	cdr_config.record.gsensor_sensity_json->valuedouble = idx;

	if (idx == 0)
		sens = HIGH_SENSITY;
	else if (idx == 2)
		sens = LOW_SENSITY;
	else if (idx == 1)
		sens = MID_SENSITY;
	else
		sens = SENSITY_CLOSE;
#ifdef LOMBO_GSENSOR
	if (sens != SENSITY_CLOSE)
		gsensor_set_measure_range(sens);
#endif

	return ret;
}

/**
 * get_gsensor_sens_idx - get gsensor sensity idx from config
 * @param: lb_obj_t object pointer.
 *
 * This function get gsensor sensity idx from config
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 get_gsensor_sens_idx(lb_int32 *idx)
{
	lb_int32 ret = 0;

	/* this pointer must be exist when the program runs here */
	RT_ASSERT(cdr_config.record.gsensor_sensity_json != NULL);

	if (idx == NULL) {
		printf("[%s,%d]param is need to check\n", __FILE__, __LINE__);
		ret = -1;
		return ret;
	}

	/* get string from json */
	*idx = cdr_config.record.gsensor_sensity_json->valueint;

	if (*idx != 0 && *idx != 1 && *idx != 2 && *idx != 3) {
		printf("[%s,%d]param is need to check\n", __FILE__, __LINE__);
		ret = -1;
		return ret;
	}

	return ret;
}

lb_int32 set_record_mute_sw(lb_int32 idx)
{
	lb_int32 ret = 0;

	/* this pointer must be exist when the program runs here */
	RT_ASSERT(cdr_config.record.mute_enable_json != NULL);

	/* set xxx to json */
	if (idx == 0) {
		cdr_config.record.mute_enable_json->valueint = idx;
		cdr_config.record.mute_enable_json->valuedouble = idx;
	} else if (idx == 1) {
		cdr_config.record.mute_enable_json->valueint = idx;
		cdr_config.record.mute_enable_json->valuedouble = idx;
	} else {
		printf("[%s,%d]param is need to check\n", __FILE__, __LINE__);
		ret = -1;
	}

	return ret;
}

lb_int32 get_record_mute_sw(lb_int32 *idx)
{
	lb_int32 ret = 0;

	/* this pointer must be exist when the program runs here */
	RT_ASSERT(cdr_config.record.mute_enable_json != NULL);

	if (idx == NULL) {
		printf("[%s,%d]param is need to check\n", __FILE__, __LINE__);
		ret = -1;
		return ret;
	}

	/* get switch from json */
	*idx = cdr_config.record.mute_enable_json->valueint;

	if (*idx != 0 && *idx != 1) {
		printf("[%s,%d]param is need to check\n", __FILE__, __LINE__);
		ret = -1;
		return ret;
	}

	return ret;
}

/**
 * set_watermark_enable_sw - set watermark time switch to config
 * @param: lb_obj_t object pointer.
 *
 * This function set watermark time switch to config
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 set_watermark_enable_sw(lb_int32 idx)
{
	lb_int32 ret = 0;

	/* this pointer must be exist when the program runs here */
	RT_ASSERT(cdr_config.record.watermark_time_enable_json != NULL);

	/* set xxx to json */
	if (idx == 0) {
		cdr_config.record.watermark_time_enable_json->valueint = idx;
		cdr_config.record.watermark_time_enable_json->valuedouble = idx;
	} else if (idx == 1) {
		cdr_config.record.watermark_time_enable_json->valueint = idx;
		cdr_config.record.watermark_time_enable_json->valuedouble = idx;
	} else {
		printf("[%s,%d]param is need to check\n", __FILE__, __LINE__);
		ret = -1;
	}

	return ret;
}

/**
 * get_watermark_enable_sw - get watermark time switch from config
 * @param: lb_obj_t object pointer.
 *
 * This function get watermark time switch from config
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 get_watermark_enable_sw(lb_int32 *idx)
{
	lb_int32 ret = 0;

	/* this pointer must be exist when the program runs here */
	RT_ASSERT(cdr_config.record.watermark_time_enable_json != NULL);

	if (idx == NULL) {
		printf("[%s,%d]param is need to check\n", __FILE__, __LINE__);
		ret = -1;
		return ret;
	}

	/* get switch from json */
	*idx = cdr_config.record.watermark_time_enable_json->valueint;

	if (*idx != 0 && *idx != 1) {
		printf("[%s,%d]param is need to check\n", __FILE__, __LINE__);
		ret = -1;
		return ret;
	}

	return ret;
}

/**
 * set_watermark_logo_enable_sw - set watermark logo switch to config
 * @param: lb_obj_t object pointer.
 *
 * This function set watermark logo switch to config
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 set_watermark_logo_enable_sw(lb_int32 idx)
{
	lb_int32 ret = 0;

	/* this pointer must be exist when the program runs here */
	RT_ASSERT(cdr_config.record.watermark_logo_enable_json != NULL);

	/* set xxx to json */
	if (idx == 0) {
		cdr_config.record.watermark_logo_enable_json->valueint = idx;
		cdr_config.record.watermark_logo_enable_json->valuedouble = idx;
	} else if (idx == 1) {
		cdr_config.record.watermark_logo_enable_json->valueint = idx;
		cdr_config.record.watermark_logo_enable_json->valuedouble = idx;
	} else {
		printf("[%s,%d]param is need to check\n", __FILE__, __LINE__);
		ret = -1;
	}

	return ret;
}

/**
 * get_watermark_logo_enable_sw - get watermark logo switch from config
 * @param: lb_obj_t object pointer.
 *
 * This function get watermark logo switch from config
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 get_watermark_logo_enable_sw(lb_int32 *idx)
{
	lb_int32 ret = 0;

	/* this pointer must be exist when the program runs here */
	RT_ASSERT(cdr_config.record.watermark_logo_enable_json != NULL);

	if (idx == NULL) {
		printf("[%s,%d]param is need to check\n", __FILE__, __LINE__);
		ret = -1;
		return ret;
	}

	/* get switch from json */
	*idx = cdr_config.record.watermark_logo_enable_json->valueint;

	if (*idx != 0 && *idx != 1) {
		printf("[%s,%d]param is need to check\n", __FILE__, __LINE__);
		ret = -1;
		return ret;
	}

	return ret;
}

/**
 * set_park_monitor_enable_sw - set park monitor switch to config
 * @param: lb_obj_t object pointer.
 *
 * This function set park monitor switch to config
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 set_park_monitor_enable_sw(lb_int32 idx)
{
	lb_int32 ret = 0;

	/* this pointer must be exist when the program runs here */
	RT_ASSERT(cdr_config.record.park_monitor_json != NULL);

	/* set xxx to json */
	if (idx == 0) {
		cdr_config.record.park_monitor_json->valueint = idx;
		cdr_config.record.park_monitor_json->valuedouble = idx;
#ifdef LOMBO_GSENSOR
		gsensor_set_park_monitor_cfg(RT_FALSE);
#endif
	} else if (idx == 1) {
		cdr_config.record.park_monitor_json->valueint = idx;
		cdr_config.record.park_monitor_json->valuedouble = idx;

		/* this pointer must be exist when the program runs here */
		RT_ASSERT(cdr_config.record.gsensor_sensity_json != NULL);
#ifdef LOMBO_GSENSOR
		gsensor_set_park_monitor_cfg(RT_TRUE);
#endif
	} else {
		printf("[%s,%d]param is need to check\n", __FILE__, __LINE__);
		ret = -1;
	}

	return ret;
}

/**
 * get_park_monitor_enable_sw - get park monitor switch from config
 * @param: lb_obj_t object pointer.
 *
 * This function get park monitor switch from config
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 get_park_monitor_enable_sw(lb_int32 *idx)
{
	lb_int32 ret = 0;

	/* this pointer must be exist when the program runs here */
	RT_ASSERT(cdr_config.record.park_monitor_json != NULL);

	if (idx == NULL) {
		printf("[%s,%d]param is need to check\n", __FILE__, __LINE__);
		ret = -1;
		return ret;
	}

	/* get switch from json */
	*idx = cdr_config.record.park_monitor_json->valueint;

	if (*idx != 0 && *idx != 1) {
		printf("[%s,%d]param is need to check\n", __FILE__, __LINE__);
		ret = -1;
		return ret;
	}

	return ret;
}

/**
 * set_interval_enable_sw - set interval switch to config
 * @param: lb_obj_t object pointer.
 *
 * This function set interval record switch to config
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 set_interval_enable_sw(lb_int32 idx)
{
	lb_int32 ret = 0;

	/* this pointer must be exist when the program runs here */
	RT_ASSERT(cdr_config.record.interval_record_json != NULL);

	/* set xxx to json */
	if (idx == 0) {
		cdr_config.record.interval_record_json->valueint = idx;
		cdr_config.record.interval_record_json->valuedouble = idx;
	} else if (idx == 1) {
		cdr_config.record.interval_record_json->valueint = idx;
		cdr_config.record.interval_record_json->valuedouble = idx;
	} else {
		printf("[%s,%d]param is need to check\n", __FILE__, __LINE__);
		ret = -1;
	}

	return ret;
}

/**
 * get_interval_enable_sw - get park monitor switch from config
 * @param: lb_obj_t object pointer.
 *
 * This function get park monitor switch from config
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 get_interval_enable_sw(lb_int32 *idx)
{
	lb_int32 ret = 0;

	/* this pointer must be exist when the program runs here */
	RT_ASSERT(cdr_config.record.interval_record_json != NULL);

	if (idx == NULL) {
		printf("[%s,%d]param is need to check\n", __FILE__, __LINE__);
		ret = -1;
		return ret;
	}

	/* get switch from json */
	*idx = cdr_config.record.interval_record_json->valueint;

	if (*idx != 0 && *idx != 1) {
		printf("[%s,%d]param is need to check\n", __FILE__, __LINE__);
		ret = -1;
		return ret;
	}

	return ret;
}


lb_int32 standby_idx_to_value(lb_uint32 idx, lb_uint32 *value)
{
	lb_int32 ret = 0;

	switch (idx) {
	case 0:
		*value = 10;
		break;

	case 1:
		*value = 30;
		break;

	case 2:
		*value = 60;
		break;

	case 3:
		*value = 0;
		break;

	default:
		*value = 10;
		break;
	}

	return ret;
}

lb_int32 set_standby_value(lb_uint32 value)
{
	lb_int32 ret = 0;

	RT_ASSERT(cdr_config.record.lcd_standby_time_json != NULL);

	/* set value to json */
	cdr_config.record.lcd_standby_time_json->valueint = value;
	cdr_config.record.lcd_standby_time_json->valuedouble = value;

	lb_gal_set_screen_standby_time(value);

	return ret;
}

lb_int32 standby_value_to_idx(lb_uint32 value, lb_uint32 *idx)
{
	lb_int32 ret = 0;

	switch (value) {
	case 10:
		*idx = 0;
		break;

	case 30:
		*idx = 1;
		break;

	case 60:
		*idx = 2;
		break;

	case 0:
		*idx = 3;
		break;

	default:
		*idx = 0;
		break;
	}

	return ret;
}

lb_int32 get_standby_value(lb_uint32 *value)
{
	lb_int32 ret = 0;

	RT_ASSERT(cdr_config.record.lcd_standby_time_json != NULL);

	/* get value from json */
	*value = cdr_config.record.lcd_standby_time_json->valueint;

	return ret;
}
lb_int32 set_warn_tone_sw(lb_int32 sw)
{
	lb_int32 ret = 0;

	RT_ASSERT(cdr_config.record.warn_tone_json != NULL);

	/* set switch to json */
	cdr_config.record.warn_tone_json->valueint = sw;
	cdr_config.record.warn_tone_json->valuedouble = sw;
if(sw){
	lb_play_warningtone(ROOTFS_MOUNT_PATH"/res/boot_music.wav");
}
	return ret;
}

lb_int32 get_warn_tone_sw(lb_int32 *sw)
{
	lb_int32 ret = 0;

	RT_ASSERT(cdr_config.record.warn_tone_json != NULL);

	/* get switch from json */
	*sw = cdr_config.record.warn_tone_json->valueint;

	return ret;
}


lb_int32 set_language_idx(lb_uint32 idx)
{
	lb_int32 ret = 0;

	lb_gal_set_language(idx);
	lb_gal_refre_language(lb_view_get_parent());
	lb_gal_refre_language(lb_view_get_static_head());
	/* set xxx to json */
	cdr_config.record.language_json->valueint = idx;
	cdr_config.record.language_json->valuedouble = idx;

	ret = 0;
	return ret;
}


lb_int32 get_language_idx(lb_uint32 *idx)
{
	lb_int32 ret = 0;

	RT_ASSERT(cdr_config.record.language_json != NULL);

	/* get xxx from json */
	*idx = cdr_config.record.language_json->valueint;

	return ret;
}

/**
 * restore_factory_setting - resotre factory setting
 * @param: lb_obj_t object pointer.
 *
 * This function resotre factory setting
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 restore_factory_setting(void)
{
	lb_int32 ret = 0;
	lb_uint32 idx = 0, value = 0;
	lb_int8 sw = 0;
	lb_imgbtn_t *property;
	void *obj;

	ret = system_set_default();
	if (ret != 0)
		printf("[%s,%d,default failed]\n", __FILE__, __LINE__);

	ret = get_language_idx(&idx);
	ret |= lb_gal_set_language(idx);
	ret |= lb_gal_refre_language(lb_view_get_parent());
	ret |= lb_gal_refre_language(lb_view_get_static_head());
	if (ret != 0)
		printf("[%s,%d,refre language failed]\n", __FILE__, __LINE__);

	ret = get_backlight_value(&value);
	ret |= set_backlight(value);
	if (ret != 0)
		printf("[%s,%d,set backlight failed]\n", __FILE__, __LINE__);

	//ret = get_standby_value(&value);
	//ret |= lb_gal_set_screen_standby_time(value);
	//if (ret != 0)
	//	printf("[%s,%d,set screen standby failed]\n", __FILE__, __LINE__);

	//ret = get_keytone_sw(&sw);
	//lb_set_tone_flag(sw);
	//ret |= get_volume_value(&value);
	//ret |= set_volume(value);
	if (ret != 0)
		printf("[%s,%d,set volume failed]\n", __FILE__, __LINE__);

    ret = get_warn_tone_sw(&sw);
	ret |= lb_view_get_obj_property_by_id(304, (void *)&property);
	ret |= lb_view_get_obj_ext_by_id(304, (void *)&obj);
	ret |= lb_gal_update_imgbtn(property, obj, LB_IMGBTN_UPD_SRC, sw, NULL);
	if (ret != 0)
		printf("[%s,%d,update imgbtn failed]\n", __FILE__, __LINE__);

    ret = get_park_monitor_enable_sw(&sw);
    ret |= lb_view_get_obj_property_by_id(303, (void *)&property);
	ret |= lb_view_get_obj_ext_by_id(303, (void *)&obj);
	ret |= lb_gal_update_imgbtn(property, obj, LB_IMGBTN_UPD_SRC, sw, NULL);
	if (ret != 0)
		printf("[%s,%d,update imgbtn failed]\n", __FILE__, __LINE__);


	return ret;
}

static lb_int32 format_prepare(void)
{
	lb_int32 ret = 0;

	ret = sdcard_umount();

	lb_system_mq_send(LB_SYSMSG_FS_PART_UNMOUNT,
		"/mnt/sdcard",
		strlen("/mnt/sdcard"),
		ASYNC_FLAG);

	return ret;
}

static lb_int32 format_finalize(void)
{
	lb_int32 ret = 0;

	ret = sdcard_mount();

	if (ret != 0)
		lb_system_mq_send(LB_SYSMSG_FS_PART_MOUNT_FAIL,
			"/mnt/sdcard",
			strlen("/mnt/sdcard"),
			ASYNC_FLAG);
	else
		lb_system_mq_send(LB_SYSMSG_FS_PART_MOUNT_OK,
			"/mnt/sdcard",
			strlen("/mnt/sdcard"),
			ASYNC_FLAG);

	return ret;
}

static lb_int32 format_get_stat(void)
{
	app_t *app_hd = NULL;
	lb_int32 ret = 0;
	lb_int32 stat = -1;

	app_hd = lb_app_check("home");
	if (app_hd)
		lb_app_ctrl(app_hd, 10/* GET_SD_STATUS */, 0, (void *)&stat);

	if (stat == 1 || stat == 2)
		ret = 0;
	else
		ret = -1;

	return ret;
}

static void *format_ref_proc(void *parameter)
{
	lb_int32 ret = 0;
	lb_int32 data = 17;
	void *bar = NULL;
	static lb_int32 val;

	lb_ui_send_msg(LB_MSG_ENTER_ISOLATE, (void *)&data, sizeof(void *), 0);
	rt_thread_delay(20);

	ret = lb_view_get_obj_ext_by_id(200, (void **)&bar);
	if (0 != ret) {
		printf("%s,%d,failed\n", __func__, __LINE__);
		goto exit;
	}

	val = 0;
	lb_gal_update_bar(bar, LB_BAR_UPD_CUR_VAL, (void *)&val);
	rt_thread_delay(20);

	while (need_exit != 1) {
		if (val < 100) {
			lb_gal_update_bar(bar, LB_BAR_UPD_CUR_VAL, (void *)&val);
			val += 1;
		}
		rt_thread_delay(50);
	}

	val = 100;
	lb_gal_update_bar(bar, LB_BAR_UPD_CUR_VAL, (void *)&val);
	rt_thread_delay(20);

exit:
	lb_ui_send_msg(LB_MSG_RETURN_PARENT, (void *)&data, sizeof(void *), 0);
	rt_thread_delay(20);

	pthread_exit(0);
	return NULL;
}

static lb_int32 format_ref_init(void)
{
	pthread_attr_t tmp_attr;
	struct sched_param shed_param;
	lb_int32 ret = 0;

	if ((pthread_t)format_ref_id) {
		ret = -1;
		printf("%s,%d,failed\n", __func__, __LINE__);
		goto exit;
	}

	need_exit = 0;

	ret = pthread_attr_init(&tmp_attr);
	if (ret != 0) {
		ret = -1;
		printf("%s,%d,failed\n", __func__, __LINE__);
		goto exit;
	}

	ret = pthread_attr_setscope(&tmp_attr, PTHREAD_SCOPE_SYSTEM);
	if (ret != 0) {
		ret = -1;
		printf("%s,%d,failed\n", __func__, __LINE__);
		goto exit;
	}

	ret = pthread_attr_getschedparam(&tmp_attr, &shed_param);
	if (ret != 0) {
		ret = -1;
		printf("%s,%d,failed\n", __func__, __LINE__);
		goto exit;
	}

	shed_param.sched_priority = FORMAT_REF_PRIO;
	ret = pthread_attr_setschedparam(&tmp_attr, &shed_param);
	if (ret != 0) {
		ret = -1;
		printf("%s,%d,failed\n", __func__, __LINE__);
		goto exit;
	}

	ret = pthread_attr_setstacksize(&tmp_attr, (size_t)FORMAT_REF_SIZE);
	if (ret != 0) {
		ret = -1;
		printf("%s,%d,failed\n", __func__, __LINE__);
		goto exit;
	}

	ret = pthread_create(&format_ref_id, &tmp_attr, &format_ref_proc, NULL);
	if (ret != 0) {
		ret = -1;
		printf("%s,%d,failed\n", __func__, __LINE__);
		goto exit;
	}

	return ret;

exit:
	pthread_attr_destroy(&tmp_attr);

	return ret;
}

static lb_int32 format_set_obj_click(lb_uint8 en)
{
	app_t *app_hd = NULL;

	app_hd = lb_app_check("home");
	if (app_hd) {
		if (en == 0)
			lb_app_ctrl(app_hd, 0x400, 0, NULL);
		else
			lb_app_ctrl(app_hd, 0x400, 1, NULL);
	}

	return 0;
}

static lb_int32 format_ref_exit(void)
{
	lb_int32 ret = 0;

	if ((pthread_t)format_ref_id) {
		need_exit = 1;
		pthread_join(format_ref_id, NULL);
		format_ref_id = (pthread_t)NULL;
	}

	return ret;
}

static void *format_ope_proc(void *parameter)
{
	lb_int32 ret = 0;
	lb_int32 data = 0;

	format_set_obj_click(0);
	rt_thread_delay(20);
	ret = format_get_stat();
	if (ret != 0) {
		printf("[%s,%d]failed\n", __FILE__, __LINE__);
		ret = FMT_NO_CARD;
		goto exit;
	}

	rt_thread_delay(20);
	ret = format_ref_init();
	if (ret != 0) {
		printf("[%s,%d]failed\n", __FILE__, __LINE__);
		ret = FMT_OTHER_ERR;
		goto exit;
	}

	rt_thread_delay(20);
	format_prepare();

	rt_thread_delay(20);
	ret = dfs_mkfs("elm", "sd0");
	if (ret != 0) {
		printf("[%s,%d]failed\n", __FILE__, __LINE__);
		ret = FMT_CARD_FAIL;
		goto exit;
	}

exit:
	if (ret == FMT_CARD_SUCC) {
		data = FORMAT_CARD_SUCCEED;
		format_ref_exit();
		format_ope_exit();

		rt_thread_delay(50);
		lb_ui_send_msg(LB_MSG_ENTER_ISOLATE,
			(void *)&data, sizeof(void *), 0);

		rt_thread_delay(50);
		format_finalize();
	} else if (ret == FMT_CARD_FAIL) {
		data = FORMAT_CARD_FAILED;
		format_ref_exit();
		format_ope_exit();

		rt_thread_delay(50);
		lb_ui_send_msg(LB_MSG_ENTER_ISOLATE,
			(void *)&data, sizeof(void *), 0);
	} else if (ret == FMT_NO_CARD) {
		data = FORMAT_NO_CARD;
		format_ope_exit();

		rt_thread_delay(50);
		lb_ui_send_msg(LB_MSG_ENTER_ISOLATE,
			(void *)&data, sizeof(void *), 0);
	} else if (ret == FMT_OTHER_ERR) {
		data = FORMAT_OTHER_ERR;
		format_ope_exit();

		rt_thread_delay(50);
		lb_ui_send_msg(LB_MSG_ENTER_ISOLATE,
			(void *)&data, sizeof(void *), 0);
	}

	format_set_obj_click(1);
	pthread_exit(0);
	return NULL;
}

static lb_int32 format_ope_init(void)
{
	pthread_attr_t tmp_attr;
	struct sched_param shed_param;
	lb_int32 ret = 0;

	if ((pthread_t)format_ope_id) {
		ret = -1;
		printf("%s,%d,failed\n", __func__, __LINE__);
		goto exit;
	}

	ret = pthread_attr_init(&tmp_attr);
	if (ret != 0) {
		ret = -1;
		printf("%s,%d,failed\n", __func__, __LINE__);
		goto exit;
	}

	ret = pthread_attr_setscope(&tmp_attr, PTHREAD_SCOPE_SYSTEM);
	if (ret != 0) {
		ret = -1;
		printf("%s,%d,failed\n", __func__, __LINE__);
		goto exit;
	}

	ret = pthread_attr_setdetachstate(&tmp_attr, PTHREAD_CREATE_DETACHED);
	if (ret != 0) {
		printf("%s,%d,failed\n", __func__, __LINE__);
		goto exit;
	}

	ret = pthread_attr_getschedparam(&tmp_attr, &shed_param);
	if (ret != 0) {
		ret = -1;
		printf("%s,%d,failed\n", __func__, __LINE__);
		goto exit;
	}

	shed_param.sched_priority = FORMAT_OPE_PRIO;
	ret = pthread_attr_setschedparam(&tmp_attr, &shed_param);
	if (ret != 0) {
		ret = -1;
		printf("%s,%d,failed\n", __func__, __LINE__);
		goto exit;
	}

	ret = pthread_attr_setstacksize(&tmp_attr, (size_t)FORMAT_OPE_SIZE);
	if (ret != 0) {
		ret = -1;
		printf("%s,%d,failed\n", __func__, __LINE__);
		goto exit;
	}

	ret = pthread_create(&format_ope_id, &tmp_attr, &format_ope_proc, NULL);
	if (ret != 0) {
		ret = -1;
		printf("%s,%d,failed\n", __func__, __LINE__);
		goto exit;
	}

exit:
	pthread_attr_destroy(&tmp_attr);

	return ret;
}

static lb_int32 format_ope_exit(void)
{
	lb_int32 ret = 0;

	if (format_ope_id)
		format_ope_id = (pthread_t)NULL;

	return ret;
}

/**
 * sd_card_format - format sd card
 * @param: lb_obj_t object pointer.
 *
 * This function format sd card
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 format_sd_card(void)
{
	lb_int32 ret = 0;

	if (!strcmp("spinor", boot_get_boot_type())) {
		ret = format_ope_init();
		if (ret != 0) {
			printf("[%s,%d]failed\n", __FILE__, __LINE__);
			ret = -1;
		}
		format_ope_exit();
		return ret;
	}

	return ret;
}
lb_int32 set_version(char *version)
{
	lb_int32 ret = 0;

	return ret;
}

lb_int32 get_version(char *version)
{
	lb_int32 ret = 0;

	RT_ASSERT(version != NULL);
	RT_ASSERT(cdr_config.version_json != NULL);
	RT_ASSERT(cdr_config.version_json->valuestring != NULL);

	/* get version from json */
	strcpy(version, cdr_config.version_json->valuestring);

	return ret;
}

