/*
 * sizes.h - size macros definations
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

#ifndef __RTT_SIZE_H___
#define __RTT_SIZE_H___

#define SZ_1				0x00000001
#define SZ_2				0x00000002
#define SZ_4				0x00000004
#define SZ_8				0x00000008
#define SZ_16				0x00000010
#define SZ_32				0x00000020
#define SZ_64				0x00000040
#define SZ_128				0x00000080
#define SZ_256				0x00000100
#define SZ_512				0x00000200

#define SZ_1K				0x00000400
#define SZ_2K				0x00000800
#define SZ_4K				0x00001000
#define SZ_8K				0x00002000
#define SZ_16K				0x00004000
#define SZ_32K				0x00008000
#define SZ_64K				0x00010000
#define SZ_128K				0x00020000
#define SZ_256K				0x00040000
#define SZ_512K				0x00080000

#define SZ_1M				0x00100000
#define SZ_2M				0x00200000
#define SZ_4M				0x00400000
#define SZ_8M				0x00800000
#define SZ_16M				0x01000000
#define SZ_32M				0x02000000
#define SZ_64M				0x04000000
#define SZ_128M				0x08000000
#define SZ_256M				0x10000000
#define SZ_512M				0x20000000

#define SZ_1G				0x40000000
#define SZ_2G				0x80000000

#define SIZE_ALIGN(x, a)		(((x) + (a) - 1) & ~((a) - 1))

#define ARRAY_SIZE(x)			(sizeof(x) / sizeof((x)[0]))

#ifndef min
#define min(a, b)			(((a) < (b)) ? (a) : (b))
#endif

#ifndef max
#define max(a, b)			(((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a, b)			(((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b)			(((a) > (b)) ? (a) : (b))
#endif

#ifndef UINT_MAX
#define UINT_MAX			(~0U)
#endif

#ifndef ULONG_MAX
#define ULONG_MAX			(~0UL)
#endif

#ifndef INT_MAX
#define INT_MAX				((int)(~0U >> 1))
#endif

#endif /* __RTT_SIZE_H___ */
