/*
 * gps_protocal.h - gps protocal head file
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

#ifndef __GPS_PROTOCAL_H__
#define __GPS_PROTOCAL_H__

#include <rtthread.h>
#include "gps_dev.h"

/* #define GPS_PROTOCAL_DEBUG */

struct gps_parser {
	char		*name;
	char		start_c;
	char		end_c;

	char		*prefix;
	u32		pre_len;
	char		*suffix;
	u32		suf_len;

	rt_bool_t	active;

	struct rt_list_node	node;
	void (*add_data)(const char c);
	struct gps_data_t (*get_gps)();

	u32		type_len;
	u32		type_arr_count;
	char		**type_arr;
};

rt_err_t gps_register_parser(struct gps_parser *parser);
rt_err_t gps_unregister_parser(struct gps_parser *parser);

void protocal_add_data(const char c);
struct gps_data_t protocal_get_gps();

#endif /* __GPS_PROTOCAL_H__ */

