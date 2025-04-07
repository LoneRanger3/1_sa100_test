/*
 * csp.h - register operation head file for LomboTech Socs
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

#ifndef __CSP_H
#define __CSP_H

#include "soc_define.h"

#if defined(ARCH_LOMBO_N7V0)

#include "n7/v0/csp-n7v0.h"

#define VA_OFF_REG	VA_OFF_N7V0
#define VA_OFF_SRAM	0x10000000

#elif defined(ARCH_LOMBO_N7V1)

#include "n7/v1/csp-n7v1.h"

#define VA_OFF_REG	VA_OFF_N7V1
#define VA_OFF_SRAM	0x10000000

#elif defined(ARCH_LOMBO_N8V0)

#include "v0/csp-n8v0.h"

#else
#error "please select a valid platform\n"
#endif

#endif /* __CSP_H */
