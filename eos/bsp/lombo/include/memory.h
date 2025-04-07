/*
 * memory.h - the system memory area definations
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

#ifndef __MEMORY_H___
#define __MEMORY_H___

#include <sizes.h>

#if defined(__EOS__) && !defined(__ASSEMBLY__)
/* dram size from uboot */
extern unsigned long dram_size;
#define LOMBO_DRAM_SIZE		(dram_size)
#endif

#if defined(ARCH_LOMBO_N7V0) || defined(ARCH_LOMBO_N7V1)
/* memory area, move to defconfig */
/* #define LOMBO_DRAM_PBASE	0x40000000 */
/* #define LOMBO_DRAM_VBASE	0xC0000000 */	/* cached start vaddr */

/* sram area */
#define LOMBO_SRAM_PBASE	0x00800000
#define LOMBO_SRAM_SIZE		SZ_32K

/* io area */
#define LOMBO_IO_AHB_PBASE	0x01000000
#define LOMBO_IO_AHB_SIZE	SZ_8M

#define LOMBO_IO_APB_PBASE	0x04000000
#define LOMBO_IO_APB_SIZE	SZ_1M

/* virtual addr */
#ifdef ENABLE_MMU
#define LOMBO_DRAM_VBASE_UNCA	LOMBO_DRAM_PBASE /* uncached start vaddr */
#define LOMBO_SRAM_VBASE	(LOMBO_SRAM_PBASE | VA_OFF_SRAM)
#define LOMBO_IO_AHB_VBASE	(LOMBO_IO_AHB_PBASE | VA_OFF_REG)
#define LOMBO_IO_APB_VBASE	(LOMBO_IO_APB_PBASE | VA_OFF_REG)
#else
#define LOMBO_DRAM_VBASE_UNCA	LOMBO_DRAM_PBASE /* uncached start vaddr */
#define LOMBO_DRAM_VBASE	LOMBO_DRAM_PBASE
#define LOMBO_SRAM_VBASE	LOMBO_SRAM_PBASE
#define LOMBO_IO_AHB_VBASE	LOMBO_IO_AHB_PBASE
#define LOMBO_IO_APB_VBASE	LOMBO_IO_APB_PBASE
#endif

#if defined(__EOS__) && !defined(__ASSEMBLY__)
/* system heap area */
extern int __bss_end;
#define RT_SYS_HEAP_BEGIN	(void *)(&__bss_end)
#ifdef ENABLE_MMU
#define RT_SYS_HEAP_END		(void *)(LOMBO_DRAM_VBASE + LOMBO_DRAM_SIZE)
#else
#define RT_SYS_HEAP_END		(void *)(LOMBO_DRAM_PBASE + LOMBO_DRAM_SIZE)
#endif
#endif /* __EOS__ && !__ASSEMBLY__ */

#elif defined ARCH_LOMBO_N8V0

/* memory area, move to defconfig */
/* #define LOMBO_DRAM_PBASE	0x60000000 */
/* #define LOMBO_DRAM_VBASE	0xC0000000 */	/* cached start vaddr */

#define LOMBO_DRAM_VBASE_UNCA	LOMBO_DRAM_PBASE /* uncached start vaddr */

/* sram area, to check here */
#define LOMBO_SRAM_PBASE	0x01000000
#define LOMBO_SRAM_SIZE		SZ_128K

/* io area */
#define LOMBO_IO_AHB_PBASE	0x51000000
#define LOMBO_IO_AHB_SIZE	SZ_32M

#define LOMBO_IO_APB_PBASE	0x54000000
#define LOMBO_IO_APB_SIZE	SZ_64K

/* virtual addr */
#ifdef ENABLE_MMU
#define LOMBO_SRAM_VBASE	0xF6000000
#define LOMBO_IO_AHB_VBASE	0xF1000000
#define LOMBO_IO_APB_VBASE	0xF4000000
#endif

/*
 * system heap area
 * FIXME: correct the RT_SYS_HEAP_BEGIN
 */
#ifdef ENABLE_MMU
#define RT_SYS_HEAP_END		(void *)(LOMBO_DRAM_VBASE + LOMBO_DRAM_SIZE)
#else
#define RT_SYS_HEAP_END		(void *)(LOMBO_DRAM_PBASE + LOMBO_DRAM_SIZE)
#endif
#define RT_SYS_HEAP_BEGIN	(void *)(RT_SYS_HEAP_END - SZ_64M)

#else /* !ARCH_LOMBO_N7V0 && !ARCH_LOMBO_N8V0 */
#error "please select a valid platform\n"
#endif

/*
 * sram area for deep sleep
 *
 * the STANDBY_CODE_START/DEEPSLP_RESUME_START are parsed by .../deepsleep/..Makefile
 * so the defination must begin with "#define", and the comment(if exist) can only be
 * in the end of line(not in middle).
 *
 * The right defination style:
 *   "#define STANDBY_CODE_START 0xF0804000"
 *   "#define STANDBY_CODE_START 0xF0804000 *comment*"
 *
 * The wrong defination style:
 *   "#define STANDBY_CODE_START .*comment*. 0xF0804000"
 *   "  #define STANDBY_CODE_START 0xF0804000"
 */
#if defined(ARCH_LOMBO_N7)
/* standby */
#define STANDBY_CODE_START	0x00800000 /* LOMBO_SRAM_PBASE */
#define STANDBY_CODE_SIZE	SZ_16K
#define STANDBY_STACK_SIZE	SZ_1K
#define STANDBY_STACK_END	(STANDBY_CODE_START + STANDBY_CODE_SIZE)

#define STANDBY_DDR_PARA	(0x00804C00 - 0x400) /* ddr para: 512bytes */
#define STANDBY_DDR_TIMING	(0x00804C00 - 0x200) /* ddr timing: 512bytes */

/* deepsleep -> suspend */
#define SUSPEND_CODE_START	0x00800000 /* LOMBO_SRAM_PBASE */
#define SUSPEND_CODE_SIZE	SZ_16K
#define SUSPEND_STACK_SIZE	SZ_1K
#define SUSPEND_STACK_END	(SUSPEND_CODE_START + SUSPEND_CODE_SIZE)

/* deepsleep -> resume */
#define RESUME_CODE_START	0x00808000 /* LOMBO_BRAM_PBASE: 64k */
#define RESUME_CODE_SIZE	0x00001800 /* resume code size 6k */

#define RESUME_DDR_PARA		(0x00808000 - 0x400)
#define RESUME_DDR_TIMING	(0x00808000 - 0x200)

#elif defined(ARCH_LOMBO_N8)

#define RESUME_CODE_SIZE	0x00001400 /* resume code size 5k */

#else
/* for the other cases, to do## */
#endif

/*
 * memory space convertions
 * NOTE: these macros are valid only for ddr memory space
 */
#ifdef ENABLE_MMU
#ifdef __ASSEMBLY__ /* used for asm environment */
#define virt_to_phys(x)		((x) - LOMBO_DRAM_VBASE + LOMBO_DRAM_PBASE)
#define phys_to_virt(x)		((x) - LOMBO_DRAM_PBASE + LOMBO_DRAM_VBASE)
#define virt_to_unca(x)		((x) - LOMBO_DRAM_VBASE + LOMBO_DRAM_VBASE_UNCA)
#define unca_to_virt(x)		((x) - LOMBO_DRAM_VBASE_UNCA + LOMBO_DRAM_VBASE)
#define phys_to_unca(x)		((x) - LOMBO_DRAM_PBASE + LOMBO_DRAM_VBASE_UNCA)
#define unca_to_phys(x)		((x) - LOMBO_DRAM_VBASE_UNCA + LOMBO_DRAM_PBASE)
#else
#define virt_to_phys(x)		((unsigned long)(x) - LOMBO_DRAM_VBASE + LOMBO_DRAM_PBASE)
#define phys_to_virt(x)		(void *)((unsigned long)(x) - LOMBO_DRAM_PBASE + LOMBO_DRAM_VBASE)
#define virt_to_unca(x)		(void *)((unsigned long)(x) - LOMBO_DRAM_VBASE + LOMBO_DRAM_VBASE_UNCA)
#define unca_to_virt(x)		(void *)((unsigned long)(x) - LOMBO_DRAM_VBASE_UNCA + LOMBO_DRAM_VBASE)
#define phys_to_unca(x)		(void *)((unsigned long)(x) - LOMBO_DRAM_PBASE + LOMBO_DRAM_VBASE_UNCA)
#define unca_to_phys(x)		((unsigned long)(x) - LOMBO_DRAM_VBASE_UNCA + LOMBO_DRAM_PBASE)
#endif

#else
#define virt_to_phys(x)		(x)
#define phys_to_virt(x)		(x)
#define virt_to_unca(x)		(x)
#define unca_to_virt(x)		(x)
#define phys_to_unca(x)		(x)
#define unca_to_phys(x)		(x)
#endif

#endif
