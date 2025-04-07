/*
 * gps_protocal.c - gps protocal realization
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
#include "gps_protocal.h"

static rt_mutex_t _gps_mutex;
static rt_list_t _parser_list = RT_LIST_OBJECT_INIT(_parser_list);

rt_err_t gps_register_parser(struct gps_parser *parser)
{
	rt_err_t ret;

	if (parser == RT_NULL)
		return -RT_EINVAL;

	ret = rt_mutex_take(_gps_mutex, RT_WAITING_FOREVER);
	if (ret != RT_EOK) {
		LOG_E("_gps_mutex rt_mutex_take error = %d", ret);
		return ret;
	}

	rt_list_insert_after(&_parser_list, &parser->node);
	rt_mutex_release(_gps_mutex);

	return RT_EOK;
}

rt_err_t gps_unregister_parser(struct gps_parser *parser)
{
	rt_err_t ret;

	if (parser == RT_NULL)
		return -RT_EINVAL;

	ret = rt_mutex_take(_gps_mutex, RT_WAITING_FOREVER);
	if (ret != RT_EOK) {
		LOG_E("_gps_mutex rt_mutex_take error = %d", ret);
		return ret;
	}

	rt_list_remove(&parser->node);
	rt_mutex_release(_gps_mutex);

	return RT_EOK;
}

void protocal_add_data(const char c)
{
	struct gps_parser *parser;
	rt_list_for_each_entry(parser, &_parser_list, node)
		parser->add_data(c);
}

struct gps_data_t protocal_get_gps()
{
	struct gps_parser *parser;
	struct gps_data_t d;

	rt_list_for_each_entry(parser, &_parser_list, node)
		if (parser->active)
			return parser->get_gps();

	/* no parser can process data */
	LOG_W("unknwon gps protocal data.");
	d.valid = RT_FALSE;
	return d;
}

int gps_protocal_init()
{
	/* mutex for protect _parser_list */
	_gps_mutex = rt_mutex_create("gps_mutex", RT_IPC_FLAG_FIFO);
	if (_gps_mutex == RT_NULL) {
		LOG_E("rt_mutex_create gps_mutex return RT_NULL");
		return -RT_EINVAL;
	}

	LOG_I("===== gps_protocal_init finished ======");
	return 0;
}

#ifdef ARCH_LOMBO
INIT_PREV_EXPORT(gps_protocal_init);
#endif

