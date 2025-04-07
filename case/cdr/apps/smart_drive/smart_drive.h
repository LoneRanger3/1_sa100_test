/*
 * smart_drive.h - smart drive code for LomboTech
 * smart drive interface and macro define
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

#ifndef __SMART_DRIVE_H__
#define __SMART_DRIVE_H__

#include "app_manage.h"
#include <debug.h>

#define SMART_DRIVE_RETURN 0xff00
#define SMART_DRIVE_EXIT 0xff01

//#define PANO
#define BSD
#define ADAS

#if defined(ADAS) && defined(PANO) && defined(BSD)
#define ADAS_PANO_BSD
#elif defined(ADAS) && defined(PANO)
#define ADAS_PANO
#elif defined(ADAS) && defined(BSD)
#define ADAS_BSD
#elif defined(PANO) && defined(BSD)
#define PANO_BSD
#else
#error "Need select two or more of macro ADAS PANO BSD"
#endif

#if defined(ADAS_PANO_BSD) && defined(ADAS_PANO)
#error "Need cancel one of macro ADAS PANO BSD"
#endif

#if defined(ADAS_PANO_BSD) && defined(PANO_BSD)
#error "Need cancel one of macro ADAS PANO BSD"
#endif

#if defined(ADAS_PANO_BSD) && defined(ADAS_BSD)
#error "Need cancel one of macro ADAS PANO BSD"
#endif

#if defined(ADAS_PANO) && defined(PANO_BSD)
#error "Need cancel one of macro ADAS PANO BSD"
#endif

#if defined(ADAS_PANO) && defined(ADAS_BSD)
#error "Need cancel one of macro ADAS PANO BSD"
#endif

#if defined(PANO_BSD) && defined(ADAS_BSD)
#error "Need cancel one of macro ADAS PANO BSD"
#endif

#if defined(ADAS_PANO_BSD)
#undef ADAS_PANO
#undef PANO_BSD
#undef ADAS_BSD
#elif defined(ADAS_PANO)
#undef ADAS_PANO_BSD
#undef PANO_BSD
#undef ADAS_BSD
#elif defined(PANO_BSD)
#undef ADAS_PANO_BSD
#undef ADAS_PANO
#undef ADAS_BSD
#elif defined(ADAS_BSD)
#undef ADAS_PANO_BSD
#undef PANO_BSD
#undef ADAS_PANO
#else
#error "Need select two or more of macro ADAS PANO BSD"
#endif

#endif /* __SMART_DRIVE_H__ */
