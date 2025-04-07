/*
 * piggy.lzo.S - lzo file
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

#include <linkage.h>

	.section .piggydata,#alloc
	.globl	input_data
input_data:
#if defined KERNEL_COMPRESS_LZO
	.incbin	"piggy.lzo"
#elif defined KERNEL_COMPRESS_GZIP
	.incbin	"piggy.gz"
#else
	#warning "Please select kernel compress algorith"
#endif
	.globl	input_data_end
input_data_end:

