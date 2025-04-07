/*
 * div.h - optimized division operations
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

#ifndef _GENERIC_DIV_H
#define _GENERIC_DIV_H

#ifdef ARCH_LOMBO_N7

#include <bitops.h>

/*
 * use the asm version of do_div
 * if you want to use C version of do_div, just comment this line
 */
#define DO_DIV_USE_ASM

/* ---------- do_div merged from arch/arm/include/asm/div64.h ---------- */

#if defined(DO_DIV_USE_ASM) && defined(ARCH_LOMBO_N7)
/*
 * If the divisor happens to be constant, we determine the appropriate
 * inverse at compile time to turn the division into a few inline
 * multiplications instead which is much faster. And yet only if compiling
 * for ARMv4 or higher (we need umull/umlal) and if the gcc version is
 * sufficiently recent to perform proper long long constant propagation.
 * (It is unfortunate that gcc doesn't perform all this internally.)
 */
#define do_div_rem(n, base)						\
({									\
	unsigned int __r, __b = (base);					\
	if (!__builtin_constant_p(__b) || __b == 0) {			\
		/* non-constant divisor (or zero): slow path */		\
		__r = __do_div_asm(n, __b);				\
	} else if ((__b & (__b - 1)) == 0) {				\
		/* Trivial: __b is constant and a power of 2 */		\
		/* gcc does the right thing with this code.  */		\
		__r = n;						\
		__r &= (__b - 1);					\
		n /= __b;						\
	} else {							\
		/* Multiply by inverse of __b: n/b = n*(p/b)/p       */	\
		/* We rely on the fact that most of this code gets   */	\
		/* optimized away at compile time due to constant    */	\
		/* propagation and only a couple inline assembly     */	\
		/* instructions should remain. Better avoid any      */	\
		/* code construct that might prevent that.           */	\
		unsigned long long __res, __x, __t, __m, __n = n;	\
		unsigned int __c, __p, __z = 0;				\
		/* preserve low part of n for reminder computation */	\
		__r = __n;						\
		/* determine number of bits to represent __b */		\
		__p = 1 << __div64_fls(__b);				\
		/* compute __m = ((__p << 64) + __b - 1) / __b */	\
		__m = (~0ULL / __b) * __p;				\
		__m += (((~0ULL % __b + 1) * __p) + __b - 1) / __b;	\
		/* compute __res = __m*(~0ULL/__b*__b-1)/(__p << 64) */	\
		__x = ~0ULL / __b * __b - 1;				\
		__res = (__m & 0xffffffff) * (__x & 0xffffffff);	\
		__res >>= 32;						\
		__res += (__m & 0xffffffff) * (__x >> 32);		\
		__t = __res;						\
		__res += (__x & 0xffffffff) * (__m >> 32);		\
		__t = (__res < __t) ? (1ULL << 32) : 0;		\
		__res = (__res >> 32) + __t;				\
		__res += (__m >> 32) * (__x >> 32);			\
		__res /= __p;						\
		/* Now sanitize and optimize what we've got. */		\
		if (~0ULL % (__b / (__b & -__b)) == 0) {		\
			/* those cases can be simplified with: */	\
			__n /= (__b & -__b);				\
			__m = ~0ULL / (__b / (__b & -__b));		\
			__p = 1;					\
			__c = 1;					\
		} else if (__res != __x / __b) {			\
			/* We can't get away without a correction    */	\
			/* to compensate for bit truncation errors.  */	\
			/* To avoid it we'd need an additional bit   */	\
			/* to represent __m which would overflow it. */	\
			/* Instead we do m=p/b and n/b=(n*m+m)/p.    */	\
			__c = 1;					\
			/* Compute __m = (__p << 64) / __b */		\
			__m = (~0ULL / __b) * __p;			\
			__m += ((~0ULL % __b + 1) * __p) / __b;		\
		} else {						\
			/* Reduce __m/__p, and try to clear bit 31   */	\
			/* of __m when possible otherwise that'll    */	\
			/* need extra overflow handling later.       */	\
			unsigned int __bits = -(__m & -__m);		\
			__bits |= __m >> 32;				\
			__bits = (~__bits) << 1;			\
			/* If __bits == 0 then setting bit 31 is     */	\
			/* unavoidable.  Simply apply the maximum    */	\
			/* possible reduction in that case.          */	\
			/* Otherwise the MSB of __bits indicates the */	\
			/* best reduction we should apply.           */	\
			if (!__bits) {					\
				__p /= (__m & -__m);			\
				__m /= (__m & -__m);			\
			} else {					\
				__p >>= __div64_fls(__bits);		\
				__m >>= __div64_fls(__bits);		\
			}						\
			/* No correction needed. */			\
			__c = 0;					\
		}							\
		/* Now we have a combination of 2 conditions:        */	\
		/* 1) whether or not we need a correction (__c), and */	\
		/* 2) whether or not there might be an overflow in   */	\
		/*    the cross product (__m & ((1<<63) | (1<<31)))  */	\
		/* Select the best insn combination to perform the   */	\
		/* actual __m * __n / (__p << 64) operation.         */	\
		if (!__c) {						\
			asm("umull	%Q0, %R0, %Q1, %Q2\n\t"		\
				"mov	%Q0, #0"			\
				: "=&r" (__res)				\
				: "r" (__m), "r" (__n)			\
				: "cc");				\
		} else if (!(__m & ((1ULL << 63) | (1ULL << 31)))) {	\
			__res = __m;					\
			asm("umlal	%Q0, %R0, %Q1, %Q2\n\t"		\
				"mov	%Q0, #0"			\
				: "+&r" (__res)				\
				: "r" (__m), "r" (__n)			\
				: "cc");				\
		} else {						\
			asm("umull	%Q0, %R0, %Q1, %Q2\n\t"		\
				"cmn	%Q0, %Q1\n\t"			\
				"adcs	%R0, %R0, %R1\n\t"		\
				"adc	%Q0, %3, #0"			\
				: "=&r" (__res)				\
				: "r" (__m), "r" (__n), "r" (__z)	\
				: "cc");				\
		}							\
		if (!(__m & ((1ULL << 63) | (1ULL << 31)))) {		\
			asm("umlal	%R0, %Q0, %R1, %Q2\n\t"		\
				"umlal	%R0, %Q0, %Q1, %R2\n\t"		\
				"mov	%R0, #0\n\t"			\
				"umlal	%Q0, %R0, %R1, %R2"		\
				: "+&r" (__res)				\
				: "r" (__m), "r" (__n)			\
				: "cc");				\
		} else {						\
			asm("umlal	%R0, %Q0, %R2, %Q3\n\t"		\
				"umlal	%R0, %1, %Q2, %R3\n\t"		\
				"mov	%R0, #0\n\t"			\
				"adds	%Q0, %1, %Q0\n\t"		\
				"adc	%R0, %R0, #0\n\t"		\
				"umlal	%Q0, %R0, %R2, %R3"		\
				: "+&r" (__res), "+&r" (__z)		\
				: "r" (__m), "r" (__n)			\
				: "cc");				\
		}							\
		__res /= __p;						\
		/* The reminder can be computed with 32-bit regs     */	\
		/* only, and gcc is good at that.                    */	\
		{							\
			unsigned int __res0 = __res;			\
			unsigned int __b0 = __b;			\
			__r -= __res0 * __b0;				\
		}							\
		/* BUG_ON(__r >= __b || __res * __b + __r != n); */	\
		n = __res;						\
	}								\
	__r;								\
})

#ifdef RT_USING_VFP
#define do_div(n, base)							\
	do {								\
		n = (n) * 1.0 / base;					\
	} while (0)
#else
#define do_div		do_div_rem
#endif /* RT_USING_VFP */

/*
 * This is used to ensure the compiler did actually allocate the register we
 * asked it for some inline assembly sequences.  Apparently we can't trust
 * the compiler from one version to another so a bit of paranoia won't hurt.
 * This string is meant to be concatenated with the inline asm string and
 * will cause compilation to stop on mismatch.
 * (for details, see gcc PR 15089)
 */
#define __asmeq(x, y)  ".ifnc " x "," y " ; .err ; .endif\n\t"

#ifdef __ARMEB__ /* big endian */
#define __xh "r0"
#define __xl "r1"
#else
#define __xl "r0"
#define __xh "r1"
#endif

extern void __do_div64(void);

#define __do_div_asm(n, base)					\
({								\
	register unsigned int __base      asm("r4") = base;	\
	register unsigned long long __n   asm("r0") = n;	\
	register unsigned long long __res asm("r2");		\
	register unsigned int __rem       asm(__xh);		\
	asm(__asmeq("%0", __xh)					\
		__asmeq("%1", "r2")				\
		__asmeq("%2", "r0")				\
		__asmeq("%3", "r4")				\
		"bl	__do_div64"				\
		: "=r" (__rem), "=r" (__res)			\
		: "r" (__n), "r" (__base)			\
		: "ip", "lr", "cc");				\
	n = __res;						\
	__rem;							\
})

/* our own fls implementation to make sure constant propagation is fine */
#define __div64_fls(bits)					\
({								\
	unsigned int __left = (bits), __nr = 0;			\
	if (__left & 0xffff0000)				\
		__nr += 16, __left >>= 16;			\
	if (__left & 0x0000ff00)				\
		__nr +=  8, __left >>=  8;			\
	if (__left & 0x000000f0)				\
		__nr +=  4, __left >>=  4;			\
	if (__left & 0x0000000c)				\
		__nr +=  2, __left >>=  2;			\
	if (__left & 0x00000002)				\
		__nr +=  1;					\
	__nr;							\
})

#endif /* DO_DIV_USE_ASM */
/* ---------- do_div merged from arch/arm/include/asm/div64.h ---------- */

/*
 * 64bit division summary:
 *  1. u64 / u32: use do_div or div_u64
 *  2. u64 / u64: use div64_u64
 *  3. s64 / s32: use div_s64
 *  4. s64 / s64: use div64_s64
 */

#if BITS_PER_LONG == 64

#define div64_long(x, y)	div64_s64((x), (y))
#define div64_ul(x, y)		div64_u64((x), (y))

/* do_div is used in case that u64 / u32 */
#ifndef do_div
#define do_div(n, base) ({					\
	uint32_t __base = (base);				\
	uint32_t __rem;						\
	__rem = ((uint64_t)(n)) % __base;			\
	(n) = ((uint64_t)(n)) / __base;				\
	__rem;							\
})
#endif /* !defined do_div */

/**
 * div_u64_rem - unsigned 64bit divide with 32bit divisor with remainder
 *
 * This is commonly provided by 32bit archs to provide an optimized 64bit
 * divide.
 */
static inline u64 div_u64_rem(u64 dividend, u32 divisor, u32 *remainder)
{
	*remainder = dividend % divisor;
	return dividend / divisor;
}

/**
 * div_s64_rem - signed 64bit divide with 32bit divisor with remainder
 */
static inline s64 div_s64_rem(s64 dividend, s32 divisor, s32 *remainder)
{
	*remainder = dividend % divisor;
	return dividend / divisor;
}

/**
 * div64_u64 - unsigned 64bit divide with 64bit divisor
 */
static inline u64 div64_u64(u64 dividend, u64 divisor)
{
	return dividend / divisor;
}

/**
 * div64_s64 - signed 64bit divide with 64bit divisor
 */
static inline s64 div64_s64(s64 dividend, s64 divisor)
{
	return dividend / divisor;
}

#elif BITS_PER_LONG == 32

#define div64_long(x, y)	div_s64((x), (y))
#define div64_ul(x, y)		div_u64((x), (y))

extern uint32_t __div64_32(uint64_t *dividend, uint32_t divisor);

/* do_div is used in case that u64 / u32 */
#ifndef do_div
#define do_div(n, base) ({					\
	uint32_t __base = (base);				\
	uint32_t __rem;						\
	(void)(((typeof((n)) *)0) == ((uint64_t *)0));	\
	if (likely(((n) >> 32) == 0)) {			\
		__rem = (uint32_t)(n) % __base;		\
		(n) = (uint32_t)(n) / __base;		\
	} else							\
		__rem = __div64_32(&(n), __base);		\
	__rem;							\
})
#endif /* !defined do_div */

static inline u64 div_u64_rem(u64 dividend, u32 divisor, u32 *remainder)
{
	*remainder = do_div_rem(dividend, divisor);
	return dividend;
}

extern s64 div_s64_rem(s64 dividend, s32 divisor, s32 *remainder);
extern u64 div64_u64(u64 dividend, u64 divisor);
extern s64 div64_s64(s64 dividend, s64 divisor);

#else /* BITS_PER_LONG == ?? */

# error do_div() does not yet support the C64

#endif /* BITS_PER_LONG */

/**
 * div_u64 - unsigned 64bit divide with 32bit divisor
 *
 * This is the most common 64bit divide and should be used if possible,
 * as many 32bit archs can optimize this variant better than a full 64bit
 * divide.
 */
static inline u64 div_u64(u64 dividend, u32 divisor)
{
	u32 remainder;
	return div_u64_rem(dividend, divisor, &remainder);
}

/**
 * div_s64 - signed 64bit divide with 32bit divisor
 */
static inline s64 div_s64(s64 dividend, s32 divisor)
{
	s32 remainder;
	return div_s64_rem(dividend, divisor, &remainder);
}

#else /* ARCH_LOMBO_N8.. */

#define do_div(n, base)		(n = (n) / (base))

#define do_div_rem(n, base)						\
({									\
	unsigned int __r = (n), __b = (base);				\
	n /= __b;							\
	__r %= __b;							\
	__r;								\
})

#endif /* ARCH_LOMBO_N7 */

#endif /* _GENERIC_DIV_H */
