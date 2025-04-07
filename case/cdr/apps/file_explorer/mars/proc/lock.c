/*
 * lock.c - lock type code for LomboTech
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

#include <mars/proc/lock.h>

lock_type_t suffix_to_ltype(const char *path)
{
	char *temp;

	if (path == NULL) {
		printf("%s,%d\n", __func__, __LINE__);
		return ALTYPE;
	}

	temp = strstr(path, "lock");
	if (temp)
		return LOCKED;
	else
		return UNLOCK;
}
