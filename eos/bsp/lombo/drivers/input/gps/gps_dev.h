/*
 * gps_dev.h - gps device head file
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

#ifndef __GPS_DEV_H__
#define __GPS_DEV_H__

#include <rtthread.h>

struct gps_data_t {
	rt_bool_t	valid;		/* this data valid or not */
	u32		latitude;	/* divide by 100000 to get actual value */
	char		lat_type;	/* N or S */
	u32		longitude;	/* divide by 100000 to get actual value */
	char		long_type;	/* W or E */
	u32		speed;		/* km/h */

	u8		year;
	u8		month;
	u8		day;
	u8		h;		/* hour */
	u8		m;		/* minute */
	u8		s;		/* second */
};

/* open the gps device and start receive location data */
rt_err_t gps_open_device();

/* close the gps device and stop receive location data */
rt_err_t gps_close_device();

/* get recently received gps data */
struct gps_data_t gps_get_data();

/* get gps device connect status */
rt_bool_t gps_connect_status();

#endif /* __GPS_DEV_H__ */

