/*
 * file common.c - Common code for LomboTech Socs
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



#ifndef PLUGIN_MANAGE_COMMON_C
#define PLUGIN_MANAGE_COMMON_C

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "lb_types.h"

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
* covert_path2basename - covert path to basename
* @path: path of a file.
*
* This function use to covert file path to basename. such as file path is
* "f:/adb/def.c",so basename is "def"
* Return basename pointer,when you use bsename done,remember to free
* basename that malloced.
*
*/
lb_byte *covert_path2basename(const lb_byte *path)
{
	const lb_byte *first, *end, *ptr;
	lb_byte *name;
	lb_int32 size;
	ptr   = (lb_byte *)path;
	first = ptr;
	end   = path + strlen(path);
	while (*ptr != '\0') {
		if (*ptr == '/')
			first = ptr + 1;
		if (*ptr == '.')
			end = ptr - 1;

		ptr++;
	}

	size = end - first + 1;
	name = malloc(size + 1);
	strncpy(name, first, size);
	name[size] = '\0';

	return name;
}


#endif /*PLUGIN_MANAGE_COMMON_C*/
