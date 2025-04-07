/*
 * debug.h - debug macros definations
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

#ifndef __INC_DEBUG_H___
#define __INC_DEBUG_H___

/*
 * redefine these macros in your module if needed
 *   DBG_SECTION_NAME: the module name, default is DBG
 *   DBG_LEVEL: debug level, default is DBG_WARNING, logs can be
        printed out only when log's level >= DBG_LEVEL
 */
/* #define DBG_SECTION_NAME	"MOD"	 */
/* #define DBG_LEVEL		DBG_INFO */

#ifndef DBG_LEVEL
#if defined __EOS__DEBUG__
#define DBG_LEVEL		DBG_INFO
#elif defined __EOS__RELEASE__
#define DBG_LEVEL		DBG_WARNING /* only print warning and err */
#else
#define DBG_LEVEL		DBG_LOG
#endif
#endif /* DBG_LEVEL */

#define DBG_ENABLE		/* enable debug macro */
#define	DBG_COLOR		/* enable log color */
#include <rtdbg.h>		/* LOG_D definations */

/* only printf can support %f, so define LOG_FLOAT */
#define LOG_FLOAT(fmt, ...)	do {	\
			printf("[%s:%d] "fmt"\n", __func__, __LINE__, ##__VA_ARGS__); \
			} while (0)

#endif /* __INC_DEBUG_H___ */
