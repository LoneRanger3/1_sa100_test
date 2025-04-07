/*
 * mod_adas.h - module adas head file
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

#ifndef __MOD_ADAS_H__
#define __MOD_ADAS_H__

#include "mod_manage.h"
enum {
	MOD_ADAS_START = 1,
	MOD_ADAS_STOP,
	MOD_ADAS_SUSPEND,
	MOD_ADAS_RESUME,
	MOD_ADAS_SET_RESULT_CB,
	MOD_ADAS_GET_STATUS,
	MOD_ADAS_MAX
};
#endif /* __MOD_ADAS_H__ */

