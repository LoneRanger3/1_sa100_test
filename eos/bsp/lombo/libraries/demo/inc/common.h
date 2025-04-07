/*
 * common.h - common functions for whole demo
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

#ifndef __DEMO_COMMON_H__
#define __DEMO_COMMON_H__

#include <stdio.h>

#ifndef MOD_NAME
#define MOD_NAME	"NULL"
#endif

/*
 * c++11 require: there should be a space between string and var
 */
#define LOG(fmt, ...)	do {	\
		printf("[%s]%s:%d " fmt, MOD_NAME, __func__, __LINE__, ##__VA_ARGS__); \
		printf("\n");	\
			} while (0)

#endif /* common.h */
