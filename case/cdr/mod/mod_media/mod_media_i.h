/*
 * mod_media_i.h - module media head file
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

#ifndef __MOD_MEDIA_I_H__
#define __MOD_MEDIA_I_H__

#include "mod_media.h"

#define md_log_line(color_n, fmt, ...) \
			printf("\033[47;"#color_n"m[%s:%d] :"fmt"\033[0m\n",\
			__func__, __LINE__, ##__VA_ARGS__)

#ifdef MEDIA_PRINT_INF_ON
	#define MD_LOG(fmt, ...) md_log_line(34, fmt, ##__VA_ARGS__)
#else
	#define MD_LOG(...)
#endif
#define MD_ERR(fmt, ...) md_log_line(31, fmt, ##__VA_ARGS__)

#endif /* __MOD_MEDIA_I_H__ */
