/*
 * st658g_protocal.c - ST658G-G11 gps module protocal realization
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

/* st658g-11g use NMEA 0183 protocal */

#define DATA_START_C		'$'
#define DATA_END_C		('\n')

#define DATA_PREFIX		"$"
#define DATA_SUFFIX		"\r\n"		/* end with '<CR><LF>' */

#define DATA_PRE_LEN		1
#define DATA_SUF_LEN		2
#define DATA_TYPE_LEN		5

/* gps data type */

/* the standard NMEA 0183 type */
#define NMEA_DATA_GGA		"GPGGA"		/* satellite position information */
#define NMEA_DATA_GSA		"GPGSA"		/* PRN data */
#define NMEA_DATA_GSV		"GPGSV"		/* Visual satellite information */
#define NMEA_DATA_RMC		"GPRMC"		/* Recommended location information */
#define NMEA_DATA_VTG		"GPVTG"		/* Ground velocity information */
#define NMEA_DATA_GLL		"GPGLL"		/* Geolocation information */
#define NMEA_DATA_ZDA		"GPZDA"		/* UTC and date */

/* st658g type */
#define ST658G_DATA_TXT		"GNTXT"
#define ST658G_DATA_GGA		"GNGGA"
#define ST658G_DATA_GSA		"BDGSA"
#define ST658G_DATA_GSV		"BDGSV"
#define ST658G_DATA_RMC		"GNRMC"
#define ST658G_DATA_ZDA		"GNZDA"

#define DATA_SEP_C		','

#define GGA_MAX_LEN		72
#define GSA_MAX_LEN		65
#define GSV_MAX_LEN		210
#define RMC_MAX_LEN		70
#define VTG_MAX_LEN		34

#define UTC_LEN_10		10	/* for hhmmss.sss*/
#define UTC_LEN_6		6	/* for hhmmss or yymmdd */

#define LAT_LEN			9
#define LONG_LEN		10
#define BIT_LEN			1
#define SPEED_LEN		6
#define GPS_BUFF_MAX		128


static struct gps_parser _parser;
static struct gps_data_t _gps_data;

/* global buffer to save data read from uart */
static char _gps_buffer[GPS_BUFF_MAX] = {0};
static int _cur_index;

static rt_size_t __content_between(const char *source,
			rt_size_t size, u32 from, u32 to, char *dest)
{
	rt_size_t sz;
	u32 from_idx, to_idx, i, count;
	rt_bool_t find_from, find_to;

	RT_ASSERT(to > from);

	from_idx = 0;
	to_idx = 0;
	count = 0;
	find_from = RT_FALSE;
	find_to = RT_FALSE;
	for (i = 0; i < size; i++) {
		if (source[i] == DATA_SEP_C)
			count += 1;

		if ((count == from) && !find_from) {
			from_idx = i;
			find_from = RT_TRUE;
		}

		if (count == to) {
			to_idx = i;
			find_to = RT_TRUE;
			break;
		}
	}

	/* Can't find the content between the range */
	if (!find_to) {
		LOG_W("Can't find -> %s: from: %d, to: %d", source, from, to);
		return 0;
	}

	/* content length between start and end tag */
	sz = to_idx - from_idx - 1;
	if (sz == 0) {
		LOG_I("sz == 0 -> %s: from_idx: %d, to_idx: %d",
			source, from_idx, to_idx);
		return sz;
	}

	rt_memcpy(dest, source + from_idx + 1, sz);
	return sz;
}

static rt_err_t __str_to_uint(const char *str, rt_size_t sz, u32 *result)
{
	u32 res, i, mult, num;
	char c;

	res = 0;
	mult = 1;
	for (i = 0; i < sz; i++) {
		c = str[sz - 1 - i];
		if ((c < '0') || (c > '9')) {
			LOG_W("convert string to number error: %s", str);
			return -RT_ERROR;
		}

		num = c - '0';
		res += (num * mult);
		mult *= 10;
	}

	*result = res;
	return RT_EOK;
}

static rt_err_t __parse_utc_aabbcc(const char *data, rt_size_t sz, u32 *a, u32 *b, u32 *c)
{
	rt_err_t ret;

	RT_ASSERT(sz >= UTC_LEN_6);

	ret = __str_to_uint(data, 2, a);
	if (ret != RT_EOK)
		return ret;

	ret = __str_to_uint(data + 2, 2, b);
	if (ret != RT_EOK)
		return ret;

	ret = __str_to_uint(data + 4, 2, c);
	if (ret != RT_EOK)
		return ret;

	return RT_EOK;
}

static rt_err_t parse_utc_hhmmss(const char *data, rt_size_t sz, u32 *h, u32 *m, u32 *s)
{
	/* format: hhmmss.sss or hhmmss */
	return __parse_utc_aabbcc(data, sz, h, m, s);
}

static rt_err_t parse_utc_ddmmyy(const char *data, rt_size_t sz,
						u32 *day, u32 *month, u32 *year)
{
	/* format: ddmmyy */
	RT_ASSERT(sz == UTC_LEN_6);
	return __parse_utc_aabbcc(data, sz, day, month, year);
}

static rt_err_t parse_lat_long(const char *data, rt_size_t sz, u32 *result)
{
	/*
	latitude, ddmm.mmmm
	longitude, dddmm.mmmm
	*/
	char ignore_c = '.';
	u32 res, i, mult, num;
	char c;

	res = 0;
	mult = 1;
	for (i = 0; i < sz; i++) {
		c = data[sz - 1 - i];
		if (c == ignore_c)
			continue;

		if ((c < '0') || (c > '9')) {
			LOG_W("convert string to number error: %s", data);
			return -RT_ERROR;
		}

		num = c - '0';
		res += (num * mult);
		mult *= 10;
	}

	*result = res / 100;
	return RT_EOK;
}

static rt_err_t parse_speed(const char *data, rt_size_t sz, u32 *result)
{
	/* format: x*.xxx, knot, 1knot = 1852m/h, result convert to km/h */
	u32 res, i, mult, num;
	char ignore_c = '.';
	char c;

	/* it contain three decimal */
	res = 0;
	mult = 1;
	for (i = 0; i < sz; i++) {
		c = data[sz - 1 - i];
		if (c == ignore_c)
			continue;

		if ((c < '0') || (c > '9')) {
			LOG_W("convert string to number error: %s", data);
			return -RT_ERROR;
		}

		num = c - '0';
		res += (num * mult);
		mult *= 10;
	}

	/* convert to km/h */
	*result = res * 1852 / 1000 / 1000;
	return RT_EOK;
}

static rt_err_t parse_gga_data(const char *data, rt_size_t sz)
{
	/* format:
	$GPGGA,<1>,<2>,<3>,<4>,<5>,<6>,<7>,<8>,
	<9>,<10>,<11>,<12>,<13>,<14>*<15><CR><LF>
	<1> UTC, hhmmss.sss
	<2> latitude, ddmm.mmmm (if digit is not enough, 0 is added)
	<3> 'N' or 'S'
	<4>longitude, dddmm.mmmm (same as above)
	<5> 'E' or 'W'
	<6> whether the location is effective, 0=invalid, 1=valid
	<7> number of satellites in use, 00~12
	<8> horizontal accuracy, 0.5~99.9
	...
	*/

	char utc[GGA_MAX_LEN] = {0};
	char lat[GGA_MAX_LEN] = {0};
	char lat_t[GGA_MAX_LEN] = {0};
	char lon[GGA_MAX_LEN] = {0};
	char lon_t[GGA_MAX_LEN] = {0};
	char valid[GGA_MAX_LEN] = {0};
	rt_size_t content_sz;

	rt_err_t ret;
	u32 latitude, longitude, h, m, s;

	/* check if the data is valid */
	content_sz = __content_between(data, sz, 6, 7, valid);
	if (content_sz != BIT_LEN) {
		LOG_I("gga: valid data length error: %d", content_sz);
		return -RT_ERROR;
	}

	if (valid[0] != '1') {
		LOG_I("invalid gga data");
		_gps_data.valid = RT_FALSE;
		return RT_EOK;
	}
	_gps_data.valid = RT_TRUE;

	/* utc data */
	content_sz = __content_between(data, sz, 1, 2, utc);
	if (content_sz < UTC_LEN_6) {
		LOG_I("gga: utc data length error: %d", content_sz);
		return -RT_ERROR;
	}

	ret = parse_utc_hhmmss(utc, content_sz, &h, &m, &s);
	if (ret != RT_EOK) {
		LOG_I("gga: parse utc data error: %s", utc);
		return ret;
	}
	_gps_data.h = h;
	_gps_data.m = m;
	_gps_data.s = s;

	/* latitude */
	content_sz = __content_between(data, sz, 2, 3, lat);
	if (content_sz == 0) {
		LOG_I("gga: latitude data length error: %d", content_sz);
		return -RT_ERROR;
	}
	ret = parse_lat_long(lat, content_sz, &latitude);
	if (ret != RT_EOK) {
		LOG_I("gga: parse latitude data error: %s", lat);
		return ret;
	}
	_gps_data.latitude = latitude;

	/* latitude type */
	content_sz = __content_between(data, sz, 3, 4, lat_t);
	if (content_sz != BIT_LEN) {
		LOG_I("gga: latitude type data length error: %d", content_sz);
		return -RT_ERROR;
	}
	_gps_data.lat_type = lat_t[0];

	/* longitude */
	content_sz = __content_between(data, sz, 4, 5, lon);
	if (content_sz == 0) {
		LOG_I("gga: longitude data length error: %d", content_sz);
		return -RT_ERROR;
	}
	ret = parse_lat_long(lon, content_sz, &longitude);
	if (ret != RT_EOK) {
		LOG_I("gga: parse longitude data error: %s", lon);
		return ret;
	}
	_gps_data.longitude = longitude;

	/* longitude type */
	content_sz = __content_between(data, sz, 5, 6, lon_t);
	if (content_sz != BIT_LEN) {
		LOG_I("gga: longitude type data length error: %d", content_sz);
		return -RT_ERROR;
	}
	_gps_data.long_type = lon_t[0];

#ifdef GPS_PROTOCAL_DEBUG
	LOG_I("===== GPGGA data =====");
	LOG_I("%c: %d, %c: %d", _gps_data.lat_type, _gps_data.latitude,
		_gps_data.long_type, _gps_data.longitude);
	LOG_I("hour: %d, min: %d, sec: %d", h, m, s);
#endif

	return RT_EOK;
}

static rt_err_t parse_gsa_data(const char *data, rt_size_t sz)
{
	/* format:
	$GPGSA,<1>,<2>,<3>,<4>,<5>,<6>,<7>,<8>,<9>,<10>,
	<11>,<12>,<13>,<14>,<15>,<16>,<17>*<18><CR><LF>*/

	/* to do: parse the GSA data ... */
	return RT_EOK;
}

static rt_err_t parse_gsv_data(const char *data, rt_size_t sz)
{
	/* format:
	$GPGSV, <1>,<2>,<3>,<4>,<5>,<6>,<7>*<8><CR><LF>
	*/

	/* to do: parse the GSV data ... */
	return RT_EOK;
}

static rt_err_t parse_rmc_data(const char *data, rt_size_t sz)
{
	/* format:
	$GPRMC,<1>,<2>,<3>,<4>,<5>,<6>,<7>,<8>,<9>,<10>,<11>,<12>*<13><CR><LF>
	<1> UTC, hhmmss.sss
	<2> location status: A=valid, V=invalid
	<3> Latitude, ddmm.mmmm
	<4> Latitude type, N or S
	<5> Longitude, dddmm.mmmm
	<6> Longitude type, E or W
	<7> speed
	<8> ...
	<9> UTC, ddmmyy
	*/
	char utc[RMC_MAX_LEN] = {0};
	char lat[RMC_MAX_LEN] = {0};
	char lat_t[RMC_MAX_LEN] = {0};
	char lon[RMC_MAX_LEN] = {0};
	char lon_t[RMC_MAX_LEN] = {0};
	char status[RMC_MAX_LEN] = {0};
	char speed[RMC_MAX_LEN] = {0};
	rt_size_t content_sz;

	rt_err_t ret;
	u32 latitude, longitude;
	u32 year, month, day, h, m, s, spd;

	/* check if the data is valid */
	content_sz = __content_between(data, sz, 2, 3, status);
	if (content_sz != BIT_LEN) {
		LOG_I("rmc: status data length error: %d", content_sz);
		return -RT_ERROR;
	}

	if (status[0] != 'A') {
		LOG_I("invalid rmc data");
		_gps_data.valid = RT_FALSE;
		return RT_EOK;
	}
	_gps_data.valid = RT_TRUE;

	/* utc hour, min, second data */
	content_sz = __content_between(data, sz, 1, 2, utc);
	if (content_sz < UTC_LEN_6) {
		LOG_I("rmc: utc hhmmss data length error: %d", content_sz);
		return -RT_ERROR;
	}

	ret = parse_utc_hhmmss(utc, content_sz, &h, &m, &s);
	if (ret != RT_EOK) {
		LOG_I("rmc: parse utc hhmmss data error: %s", utc);
		return ret;
	}
	_gps_data.h = h;
	_gps_data.m = m;
	_gps_data.s = s;

	/* latitude */
	content_sz = __content_between(data, sz, 3, 4, lat);
	if (content_sz == 0) {
		LOG_I("rmc: latitude data length error: %d", content_sz);
		return -RT_ERROR;
	}
	ret = parse_lat_long(lat, content_sz, &latitude);
	if (ret != RT_EOK) {
		LOG_I("rmc: parse latitude data error: %s", lat);
		return ret;
	}
	_gps_data.latitude = latitude;

	/* latitude type */
	content_sz = __content_between(data, sz, 4, 5, lat_t);
	if (content_sz != BIT_LEN) {
		LOG_I("rmc: latitude type data length error: %d", content_sz);
		return -RT_ERROR;
	}
	_gps_data.lat_type = lat_t[0];

	/* longitude */
	content_sz = __content_between(data, sz, 5, 6, lon);
	if (content_sz == 0) {
		LOG_I("rmc: longitude data length error: %d", content_sz);
		return -RT_ERROR;
	}
	ret = parse_lat_long(lon, content_sz, &longitude);
	if (ret != RT_EOK) {
		LOG_I("rmc: parse longitude data error: %s", lon);
		return ret;
	}
	_gps_data.longitude = longitude;

	/* longitude type */
	content_sz = __content_between(data, sz, 6, 7, lon_t);
	if (content_sz != BIT_LEN) {
		LOG_I("rmc: longitude type data length error: %d", content_sz);
		return -RT_ERROR;
	}
	_gps_data.long_type = lon_t[0];

	/* speed */
	content_sz = __content_between(data, sz, 7, 8, speed);
	if (content_sz == 0) {
		LOG_I("rmc: speed data length error: %d", content_sz);
		return -RT_ERROR;
	}
	ret = parse_speed(speed, content_sz, &spd);
	if (ret != RT_EOK) {
		LOG_I("rmc: parse speed data error: %s", speed);
		return ret;
	}
	_gps_data.speed = spd;

	/* UTC, day, month, year */
	content_sz = __content_between(data, sz, 9, 10, utc);
	if (content_sz < UTC_LEN_6) {
		LOG_I("rmc: utc ddmmyy data length error: %d", content_sz);
		return -RT_ERROR;
	}

	ret = parse_utc_ddmmyy(utc, content_sz, &day, &month, &year);
	if (ret != RT_EOK) {
		LOG_I("gga: parse utc ddmmyy data error: %s", utc);
		return ret;
	}
	_gps_data.day = day;
	_gps_data.month = month;
	_gps_data.year = year;

#ifdef GPS_PROTOCAL_DEBUG
	LOG_I("===== GPRMC data =====");
	LOG_I("%c: %d, %c: %d", _gps_data.lat_type, _gps_data.latitude,
		_gps_data.long_type, _gps_data.longitude);
	LOG_I("hour: %d, min: %d, sec: %d", h, m, s);
	LOG_I("year: %d, month: %d, day: %d", year, month, day);
	LOG_I("speed: %d", _gps_data.speed);
#endif

	return RT_EOK;
}

static rt_err_t parse_vtg_data(const char *data, rt_size_t sz)
{
	/* format:
	$GPVTG,<1>,T,<2>,M,<3>,N,<4>,K,<5>*hh
	...
	<4> speed, km/h, format: 0000.0~1851.8
	<5> mode: A=self-location, D=different, E=estimate, N=invalid
	...
	*/
#if 0
	char speed[VTG_MAX_LEN] = {0};

	rt_err_t ret;
	rt_size_t content_sz;
	u32 i, m_idx, spd;
	rt_bool_t find;

	if ((sz > VTG_MAX_LEN) || (sz <= 0)) {
		LOG_I("vtg data length error: %d", sz);
		return -RT_ERROR;
	}

	/* find the mode index, behind 'K' 2 characters */
	find = RT_FALSE;
	for (i = 0; i < sz; i++) {
		if (data[i] == 'K') {
			m_idx = i + 2;
			if (m_idx < VTG_MAX_LEN) {
				find = RT_TRUE;
				break;
			}
		}
	}

	if (!find) {
		LOG_I("vgt: can't find mode information");
		return -RT_ERROR;
	}

	if (data[m_idx] == 'N') {
		LOG_I("invalid vgt data");
		return RT_EOK;
	}

	/* speed data */
	content_sz = __content_between(data, sz, 7, 8, speed);
	if (content_sz != SPEED_LEN) {
		LOG_I("vgt: speed data length error: %d", content_sz);
		return -RT_ERROR;
	}

	ret = parse_speed(speed, content_sz, &spd);
	if (ret != RT_EOK) {
		LOG_I("vgt: parse speed data error: %s", speed);
		return ret;
	}
	_gps_data.speed = spd;

#ifdef GPS_PROTOCAL_DEBUG
	LOG_I("===== GPVTG data =====");
	LOG_I("speed: %d", spd);
#endif

#endif /* 0, ignore vtg data temporarily */
	return RT_EOK;
}

static rt_err_t parse_gll_data(const char *data, rt_size_t sz)
{
	return RT_EOK;
}

static rt_err_t parse_zda_data(const char *data, rt_size_t sz)
{
	return RT_EOK;
}

static rt_err_t parse_txt_data(const char *data, rt_size_t sz)
{
	return RT_EOK;
}

static rt_err_t st658g_parse_data(const char *data, rt_size_t sz)
{
	rt_err_t ret;
	char type[DATA_TYPE_LEN + 1] = {0};

	/* get the data type */
	rt_memcpy(type, data + DATA_PRE_LEN, DATA_TYPE_LEN);

	/* parse data */
	if (!strcmp(type, NMEA_DATA_GGA) || !strcmp(type, ST658G_DATA_GGA)) {
		/* GGA data */
		ret = parse_gga_data(data, sz);
	} else if (!strcmp(type, NMEA_DATA_GSA) || !strcmp(type, ST658G_DATA_GSA)) {
		/* GSA data */
		ret = parse_gsa_data(data, sz);
	} else if (!strcmp(type, NMEA_DATA_GSV) || !strcmp(type, ST658G_DATA_GSV)) {
		/* GSV data */
		ret = parse_gsv_data(data, sz);
	} else if (!strcmp(type, NMEA_DATA_RMC) || !strcmp(type, ST658G_DATA_RMC)) {
		/* RMC data */
		ret = parse_rmc_data(data, sz);
	} else if (!strcmp(type, NMEA_DATA_VTG)) {
		/* VTG data */
		ret = parse_vtg_data(data, sz);
	} else if (!strcmp(type, NMEA_DATA_GLL)) {
		/* GLL data */
		ret = parse_gll_data(data, sz);
	} else if (!strcmp(type, NMEA_DATA_ZDA) || !strcmp(type, ST658G_DATA_ZDA)) {
		/* ZDA data */
		ret = parse_zda_data(data, sz);
	} else if (!strcmp(type, ST658G_DATA_TXT)) {
		/* TXT data */
		ret = parse_txt_data(data, sz);
	} else {
		LOG_I("unknown type: %s", type);
		ret = -RT_ERROR;
	}

	return ret;
}

void __st658g_add_to_buffer(const char c)
{
	rt_err_t ret;
	char pre[DATA_PRE_LEN + 1] = {0};
	char suf[DATA_SUF_LEN + 1] = {0};

	/* ignore char when the buffer empty and the char is not start char */
	if ((_cur_index == 0) && (c != _parser.start_c)) {
		LOG_D("ignore other char when buffer empty: %c", c);
		return;
	}

	/* buffer is full and can't parse it, regard it as other protocal */
	if (_cur_index >= GPS_BUFF_MAX) {
		/* LOG_I("%s: gps data buffer full, reset it", _parser.name); */
		_cur_index = 0;
		_parser.active = RT_FALSE;
		return;
	}

	/* append data to global gps buffer */
	_gps_buffer[_cur_index] = c;
	_cur_index += 1;

	/* at the end of the data, parse it */
	if (_parser.end_c == c) {
		/* check prefix and suffix content */
		rt_memcpy(pre, _gps_buffer, DATA_PRE_LEN);
		rt_memcpy(suf, _gps_buffer + _cur_index - DATA_SUF_LEN, DATA_SUF_LEN);

		/* the gps buffer is start with special character, it's a complete data */
		if (!strcmp(pre, _parser.prefix) && !strcmp(suf, _parser.suffix)) {
			/* process a complete data */
			ret = st658g_parse_data(_gps_buffer, _cur_index);
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

struct gps_data_t __st658g_get_gps_data()
{
	return _gps_data;
}

#define TEST_CONTENT_MAX	120
void __test_parse()
{
	rt_err_t ret;
	char utc[TEST_CONTENT_MAX] = "091422.043";
	char lat[TEST_CONTENT_MAX] = "2222.0242";
	char lon[TEST_CONTENT_MAX] = "11335.5045";

	u32 latitude, longitude, h, m, s;

	char rmc[TEST_CONTENT_MAX] =
		"$GNRMC,004613.000,A,2221.7696,N,11332.6236,E,16.446,26.43,221019,,,A*45";
	char gga[TEST_CONTENT_MAX] =
		"$GNGGA,004614.000,2221.7734,N,11332.6257,E,1,12,0.79,6.0,M,-3.4,M,,*64";

	LOG_I("===== test data parse =====");

	ret = parse_utc_hhmmss(utc, 10, &h, &m, &s);
	if (ret != RT_EOK) {
		LOG_W("parse_utc_hhmmss error");
		return;
	}

	ret = parse_lat_long(lat, 9, &latitude);
	if (ret != RT_EOK) {
		LOG_W("parse_lat_long latitude error");
		return;
	}

	ret = parse_lat_long(lon, 10, &longitude);
	if (ret != RT_EOK) {
		LOG_W("parse_lat_long longitude error");
		return;
	}

	ret = parse_rmc_data(rmc, 70);
	if (ret != RT_EOK) {
		LOG_W("parse_rmc_data error");
		return;
	}

	ret = parse_gga_data(gga, 70);
	if (ret != RT_EOK) {
		LOG_W("parse_gga_data error");
		return;
	}

	LOG_I("__test_parse result:");
	LOG_I("hour: %d, min: %d, sec: %d", h, m, s);
	LOG_I("latitude: %d, longitude: %d", latitude, longitude);
}

int st658g_protocal_init()
{
	rt_err_t ret;
	int i;

#ifdef GPS_PROTOCAL_DEBUG
	__test_parse();
#endif

	_parser.name = "st658g_protocal";
	_parser.start_c = DATA_START_C;
	_parser.end_c = DATA_END_C;

	_parser.prefix = DATA_PREFIX;
	_parser.pre_len = DATA_PRE_LEN;

	_parser.suffix = DATA_SUFFIX;
	_parser.suf_len = DATA_SUF_LEN;

	_parser.active = RT_FALSE;
	_parser.type_len = DATA_TYPE_LEN;

	char type_arr[][16] = {
		NMEA_DATA_GGA, NMEA_DATA_GSA, NMEA_DATA_GSV, NMEA_DATA_RMC,
		NMEA_DATA_VTG, NMEA_DATA_GLL, NMEA_DATA_ZDA, ST658G_DATA_TXT,
		ST658G_DATA_GGA, ST658G_DATA_GSA, ST658G_DATA_GSV, ST658G_DATA_RMC,
		ST658G_DATA_ZDA,
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
	_parser.add_data = __st658g_add_to_buffer;
	_parser.get_gps = __st658g_get_gps_data;

	ret = gps_register_parser(&_parser);
	if (ret != RT_EOK) {
		LOG_E("gps_register_parser %s error: %d", _parser.name, ret);
		return ret;
	}

	LOG_I("st658g_protocal_init finished");
	return 0;
}

#ifdef ARCH_LOMBO
INIT_DEVICE_EXPORT(st658g_protocal_init);
#endif

