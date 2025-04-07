/*
 * pm_asm.S - power management asm module
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
#include <platsmp.h>
#include <memory.h>

	.globl	suspend_start
	.globl	suspend_end

	.text

suspend_start:
#ifdef ARCH_LOMBO_N7
/*	.incbin	"standby.bin" */
/*	.incbin	"suspend.bin" */
#endif
suspend_end:
	.align	2

