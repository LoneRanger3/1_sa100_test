/*
 * q8_protocal.c - q8 gps module protocal realization
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

#include <debug.h>
#include <string.h>
#include "gps_protocal.h"

#define DATA_START_C		'$'
#define DATA_END_C		'~'

#define DATA_PREFIX		"$XZ"
#define DATA_SUFFIX		"~"

#define DATA_PRE_LEN		3
#define DATA_SUF_LEN		1
#define DATA_TYPE_LEN		1
#define DATA_CHECK_LEN		1

#define GPS_DATA_LEN		36
#define	RADAR_DATA_LEN		2
#define VERSION_DATA_LEN	6
#define EDONG_DATA_LEN		1

#define DATA_MIN_LEN		EDONG_DATA_LEN
#define DATA_MAX_LEN		GPS_DATA_LEN
#define GPS_BUFF_MAX		128

/* gps data type */
#define Q8_DATA_GPS		"G"	/* gps data */
#define Q8_DATA_RADAR		"R"	/* radar alarm info */
#define Q8_DATA_VERSION		"V"	/* version info */
#define Q8_DATA_DOG_UPDATE	"D"	/* E-dog upgrade info */
#define Q8_DATA_S		"S"	/* often receive this type but unknow it */

#define Q8_DATA_VALID_TAG	'A'
#define Q8_DATA_INVALID_TAG	'V'

static struct gps_parser _parser;
static struct gps_data_t _gps_data;

/* global buffer to save data read from uart */
static char _gps_buffer[GPS_BUFF_MAX] = {0};
static int _cur_index;

/*
 * parse_gps_data - parse the gps data
 * @gps: the data to parse
 * @sz: the size of gps data
 *
 * return: RT_EOK, parse data success; other, failed
 */
static rt_err_t parse_gps_data(const char *gps, rt_size_t sz)
{
	char status;
	u32 latitude;
	char lat_type;
	u32 longitude;
	char long_type;
	u16 speed;
	u16 angle;
	u8 year, month, day, h, m, s;

	if (sz != GPS_DATA_LEN) {
		LOG_I("gps data length error: %d", sz);
		return -RT_EINVAL;
	}

	status = gps[0];
	if (status != Q8_DATA_VALID_TAG) {
		LOG_I("gps status invalid: %c", status);
		_gps_data.valid = RT_FALSE;
		return RT_EOK;
	}

	_gps_data.valid = RT_TRUE;

	/* latitude data and type */
	latitude = ((u8)gps[1] << 16) | ((u8)gps[2] << 8) | (u8)gps[3];
	lat_type = gps[4];	/* N or S */
	_gps_data.latitude = latitude;
	_gps_data.lat_type = lat_type;

	/* longitude data and type */
	longitude = ((u8)gps[5] << 24) | ((u8)gps[6] << 16) |
			((u8)gps[7] << 8) | (u8)gps[8];
	long_type = gps[9];	/* W or E */
	_gps_data.longitude = longitude;
	_gps_data.long_type = long_type;

	/* the current running speed of car, m/s */
	LOG_I("gps[10] = %d, gps[11] = %d", gps[10], gps[11]);
	speed = ((u8)gps[10] << 8) | (u8)gps[11];

	_gps_data.speed = speed;

	/* direction angle */
	angle = ((u8)gps[12] << 8) | (u8)gps[13];

	/* time */
	year = (u8)gps[17];
	month = (u8)gps[18];
	day = (u8)gps[19];
	h = (u8)gps[20];
	m = (u8)gps[21];
	s = (u8)gps[22];

	_gps_data.year = year;
	_gps_data.month = month;
	_gps_data.day = day;
	_gps_data.h = h;
	_gps_data.m = m;
	_gps_data.s = s;

	/* decode ohter data: to do ... */

#ifdef GPS_PROTOCAL_DEBUG
	LOG_I("===== gps data =====");
	LOG_I("latitude: %d, type: %c", latitude, lat_type);
	LOG_I("longitude: %d, type: %c", longitude, long_type);
	LOG_I("speed: %d, angle: %d", speed, angle);
	LOG_I("%u %u %u - %u:%u:%u", year, month, day, h, m, s);
#endif

	return RT_EOK;
}

/*
 * parse_radar_data - parse the radar data
 * @radar: the data to parse
 * @sz: the size of radar data
 *
 * return: RT_EOK, parse data success; other, failed
 */
static rt_err_t parse_radar_data(const char *radar, rt_size_t sz)
{
	/* to do ... */
	return RT_EOK;
}

/*
 * parse_version_data - parse the version data
 * @version: the data to parse
 * @sz: the size of version data
 *
 * return: RT_EOK, parse data success; other, failed
 */
static rt_err_t parse_version_data(const char *version, rt_size_t sz)
{
	u8 firm_v;	/* firmware version */
	u16 data_v;	/* E-dog data version */
	u32 date;	/* data version data */

	if (sz != VERSION_DATA_LEN) {
		LOG_I("gps data length error: %d", sz);
		return -RT_ERROR;
	}

	firm_v = version[0];
	data_v = ((u8)version[1] << 8) | (u8)version[2];
	date = ((u8)version[3] << 16) | ((u8)version[4] << 8) | (u8)version[5];

#ifdef GPS_PROTOCAL_DEBUG
	LOG_I("===== version data =====");
	LOG_I("firm_v: %d, data_v: %d, date: %d", firm_v, data_v, date);
#endif

	return RT_EOK;
}

/*
 * parse_edog_data - parse the E-dog data
 * @d: the data to parse
 * @sz: the size of E-dog data
 *
 * return: RT_EOK, parse data success; other, failed
 */
static rt_err_t parse_edog_data(const char *d, rt_size_t sz)
{
	/* to do ... */
	return RT_EOK;
}

/*
 * q8_parse_data - parse a complete data
 * @data: the data to parse
 * @sz: the size data
 *
 * return: RT_EOK, parse data success; other, failed
 */
static rt_err_t q8_parse_data(const char *data, rt_size_t sz)
{
	/* process a complete data */
	rt_err_t ret;
	u32 other_len = DATA_PRE_LEN + DATA_SUF_LEN + DATA_TYPE_LEN + DATA_CHECK_LEN;
	char pre[DATA_PRE_LEN + 1] = {0};
	char suf[DATA_SUF_LEN + 1] = {0};
	char type[DATA_TYPE_LEN + 1] = {0};
	char content[DATA_MAX_LEN + 1] = {0};

	/* data format: [ start:3 | type:1 | content:1~36 | check:1 | end:1 ] */
	if (sz < (DATA_MIN_LEN + other_len)) {
		LOG_I("size of gps data is too short: %d", sz);
		return -RT_ERROR;
	}

	if (sz > (DATA_MAX_LEN + other_len)) {
		LOG_I("size of gps data is too long: %d", sz);
		return -RT_ERROR;
	}

	/* check the prefix and suffix character */
	rt_memcpy(pre, data, DATA_PRE_LEN);
	rt_memcpy(suf, data + sz - DATA_SUF_LEN, DATA_SUF_LEN);

	if (strcmp(pre, _parser.prefix)) {
		LOG_I("%s: gps data prefix character mismatch: %s", _parser.name, pre);
		return -RT_ERROR;
	}

	if (strcmp(suf, _parser.suffix)) {
		LOG_I("%s: gps data suffix character mismatch: %s", _parser.name, suf);
		return -RT_ERROR;
	}

	/* get the data type and content */
	rt_memcpy(type, data + DATA_PRE_LEN, DATA_TYPE_LEN);
	rt_memcpy(content, data + DATA_PRE_LEN + DATA_TYPE_LEN,
		sz - (DATA_PRE_LEN + DATA_TYPE_LEN + DATA_CHECK_LEN + DATA_SUF_LEN));

	/* parse data */
	if (!strcmp(type, Q8_DATA_GPS)) {
		/* gps data */
		ret = parse_gps_data(content, GPS_DATA_LEN);
	} else if (!strcmp(type, Q8_DATA_RADAR)) {
		/* radar alarm info */
		ret = parse_radar_data(content, RADAR_DATA_LEN);
	} else if (!strcmp(type, Q8_DATA_VERSION)) {
		/* version info */
		ret = parse_version_data(content, VERSION_DATA_LEN);
	} else if (!strcmp(type, Q8_DATA_DOG_UPDATE)) {
		/* E-dog upgrade info */
		ret = parse_edog_data(content, EDONG_DATA_LEN);
	} else if (!strcmp(type, Q8_DATA_S)) {
		/* unknown ths 'S' type,  */
		ret = RT_EOK;
	} else {
		LOG_I("unknown type: %s", type);
		ret = -RT_ERROR;
	}

	/*
	if (ret == RT_EOK) {
		LOG_I("===== parse data =====");
		LOG_I("prefix: %s", pre);
		LOG_I("type: %s", type);
		LOG_I("content: %s", content);
		LOG_I("suffix: %s", suf);
	}
	*/

	return ret;
}

void __q8_add_to_buffer(const char c)
{
	rt_err_t ret;
	char pre[DATA_PRE_LEN + 1] = {0};

	/* version info data start with some blank char, ignore it */
	if ((_cur_index == 0) && (c != _parser.start_c)) {
		LOG_D("ignore other char when buffer empty: %c", c);
		return;
	}

	/* buffer is full and can't parse it, regard it as other protocal */
	if (_cur_index >= GPS_BUFF_MAX) {
		/* LOG_I("%s: gps data buffer full, reset it", _parser.name); */
		_cur_index = 0;
		_parser.active = RT_FALSE;

		/* reset the valid status */
		_gps_data.valid = RT_FALSE;
		return;
	}

	/* append data to global gps buffer */
	_gps_buffer[_cur_index] = c;
	_cur_index += 1;

	/* at the end of the data, parse it */
	if (_parser.end_c == c) {
		rt_memcpy(pre, _gps_buffer, DATA_PRE_LEN);
		/* the gps buffer is start with special character, it's a complete data */
		if (!strcmp(pre, _parser.prefix)) {
			/* process a complete data */
			ret = q8_parse_data(_gps_buffer, _cur_index);
			if (ret == RT_EOK)
				_parser.active = RT_TRUE;
			else
				_parser.active = RT_FALSE;
		} else {
			/* data mismatch protocal, maybe some other gps module  */
			LOG_W("%s: _gps_buffer start without '%s'",
				_parser.name, _parser.prefix);
			_parser.active = RT_FALSE;
		}

		_cur_index = 0;
	}
}

struct gps_data_t __q8_get_gps_data()
{
	return _gps_data;
}

int q8_protocal_init()
{
	rt_err_t ret;
	int i;

	_parser.name = "q8_protocal";
	_parser.start_c = DATA_START_C;
	_parser.end_c = DATA_END_C;

	_parser.prefix = DATA_PREFIX;
	_parser.pre_len = DATA_PRE_LEN;

	_parser.suffix = DATA_SUFFIX;
	_parser.suf_len = DATA_SUF_LEN;

	_parser.active = RT_FALSE;
	_parser.type_len = DATA_TYPE_LEN;

	char type_arr[][16] = {
		Q8_DATA_GPS, Q8_DATA_RADAR, Q8_DATA_VERSION,
		Q8_DATA_DOG_UPDATE, Q8_DATA_S,
	};
	_parser.type_arr_count = ARRAY_SIZE(type_arr);
	_parser.type_arr = rt_malloc(sizeof(char) *
			DATA_TYPE_LEN * _parser.type_arr_count);
	if (_parser.type_arr == RT_NULL) {
		LOG_E("rt_malloc return RT_NULL");
		return -RT_ENOMEM;
	}

	for (i = 0; i < _parser.type_arr_count; i++)
		_parser.type_arr[i] = type_arr[i];

	rt_list_init(&(_parser.node));
	_parser.add_data = __q8_add_to_buffer;
	_parser.get_gps = __q8_get_gps_data;

	ret = gps_register_parser(&_parser);
	if (ret != RT_EOK) {
		LOG_E("gps_register_parser %s error: %d", _parser.name, ret);
		return ret;
	}

	LOG_I("q8_protocal_init finished");
	return 0;
}

#ifdef ARCH_LOMBO
INIT_DEVICE_EXPORT(q8_protocal_init);
#endif

