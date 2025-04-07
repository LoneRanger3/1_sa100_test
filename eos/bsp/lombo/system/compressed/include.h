/*
 * include.h - compressed module head file
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

#ifndef __COMPRESS_INCLUDE_H
#define __COMPRESS_INCLUDE_H

/* if need print msg during decompressing */
/* #define DEBUG */

/*
 * the uart index used for decompressing debug
 */
#ifdef ARCH_LOMBO_N7V1_CDR
#define USE_UART0
#elif defined(ARCH_LOMBO_N7V1_EVB)
#define USE_UART1
#else
#define USE_UART1
#endif
#define CONFIG_MMU	/* if need enable mmu during decompressing */

/* defined for compile */
#define size_t		u32
#define STATIC		static
#define INIT
#define STATIC_RW_DATA	/* non-static please */

/* the entry point for kernel */
#define zreladdr	0x40008000

#ifndef __ASSEMBLY__

#include <csp.h>

extern unsigned long malloc_ptr;
extern int malloc_count;
extern unsigned char *output_data;
extern unsigned long free_mem_ptr;
extern unsigned long free_mem_end_ptr;

extern char input_data[];
extern char input_data_end[];

void free(void *where);
void *malloc(int size);

void *memcpy(void *__dest, __const void *__src, size_t __n);
void *memmove(void *__dest, __const void *__src, size_t count);
size_t strlen(const char *s);
int memcmp(const void *cs, const void *ct, size_t count);
int strcmp(const char *cs, const char *ct);
void *memchr(const void *s, int c, size_t count);
char *strchr(const char *s, int c);
void *memset(void *s, int c, size_t count);
void __memzero(void *s, size_t count);

/* Not needed, but used in some headers pulled in by decompressors */
extern char *strstr(const char *s1, const char *s2);
extern void putc(int c);
extern void error(char *x);
extern void lombo_early_uart_config();

extern int do_decompress(u8 *input, int len, u8 *output, void (*error)(char *x));

#else

/*
 * uart reg definations
 *
 * note: the va and pa is the same during decompressing stage
 */
#define PA_UART0	0x04003000 /* BASE_UART0 */
#define PA_UART1	0x04003800 /* BASE_UART1 */
#define VA_UART0	PA_UART0
#define VA_UART1	PA_UART1

#define UART_SHIFT	2
#define UART_RX		4	/* In:  Receive buffer */
#define UART_TX		3	/* Out: Transmit buffer */
#define UART_LSR	8	/* In:  Line Status Register */
#define UART_LSR_TEMT	0x40	/* Transmitter empty */
#define UART_LSR_THRE	0x20	/* Transmit-hold-register empty */
#define UART_MSR_DSR	0x20	/* Data Set Ready */
#define UART_MSR_CTS	0x10	/* Clear to Send */

.macro	addruart, rp, rv, tmp
#if defined(USE_UART0)
	ldr	\rp, = PA_UART0
	ldr	\rv, = VA_UART0
#elif defined(USE_UART1)
	ldr	\rp, = PA_UART1
	ldr	\rv, = VA_UART1
#endif
.endm

.macro	senduart, rd, rx
	strb	\rd, [\rx, #UART_TX << UART_SHIFT]
.endm

.macro	busyuart, rd, rx
1002:
	ldrb	\rd, [\rx, #UART_LSR << UART_SHIFT]
	and	\rd, \rd, #UART_LSR_TEMT | UART_LSR_THRE
	teq	\rd, #UART_LSR_TEMT | UART_LSR_THRE
	bne	1002b
.endm

.macro  waituart, rd, rx
#ifdef FLOW_CONTROL
1001:	ldrb	\rd, [\rx, #UART_MSR << UART_SHIFT]
	tst	\rd, #UART_MSR_CTS
	beq	1001b
#endif
.endm

#endif /* __ASSEMBLY__ */

#endif /* __COMPRESS_INCLUDE_H */
