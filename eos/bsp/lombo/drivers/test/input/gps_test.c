/*
 * gps_test.c - gps test driver
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
#include <rtthread.h>
#include <debug.h>
#include <string.h>
#include "input/gps/gps_dev.h"

#define TEST_GPS_CMD_OPEN		"open"
#define TEST_GPS_CMD_CLOSE		"close"
#define TEST_GPS_CMD_GET		"get"

static void test_gps_open()
{
	LOG_D("test_gps_open");
	gps_open_device();
}

static void test_gps_close()
{
	LOG_D("test_gps_close");
	gps_close_device();
}

static void test_gps_get_data()
{
	struct gps_data_t g;
	rt_bool_t status;

	LOG_D("test_gps_get_data");
	g = gps_get_data();
	status = gps_connect_status();

	LOG_D("GPS device connect status: %d", status);
	if (!status) {
		LOG_D("GPS device not connected!");
		return;
	}

	if (g.valid) {
		LOG_D("lat: %d, lat_type: %c, long: %d, long_type: %c, speed: %d",
			g.latitude, g.lat_type,
			g.longitude, g.long_type, g.speed);
	} else
		LOG_D("gps data invalid");
}

long test_gps(int argc, char **argv)
{
	LOG_D("test_gps...");
	if (3 == argc) {
		char *cmd;

		cmd = argv[2];
		if (!strcmp(cmd, TEST_GPS_CMD_OPEN))
			test_gps_open();
		else if (!strcmp(cmd, TEST_GPS_CMD_CLOSE))
			test_gps_close();
		else if (!strcmp(cmd, TEST_GPS_CMD_GET))
			test_gps_get_data();
		else
			LOG_D("invalid cmd\n");
	} else {
		/* to add some test case... */
		LOG_D("test_gps success");
	}

	return 0;
}

