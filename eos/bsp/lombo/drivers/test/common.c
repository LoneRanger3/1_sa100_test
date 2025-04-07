/*
 * common.c - common functions for eos test module
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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <rtthread.h>
#include <debug.h>
#include "board.h"

#if defined(LOMBO_TEST_PTHREAD_MUTEX)	\
	|| defined(LOMBO_TEST_VFP)	\
	|| defined(LOMBO_TEST_NEON)

float multi_func3(float a, float b, float c, float *pa,
			float *pb, float *pc)
{
	float *ptra = pa, *ptrb = pb, *ptrc = pc;

	a *= *ptra;
	b *= *ptrb;
	c *= *ptrc;
	rt_thread_delay(0);
	return (float)(a * b * c);
}

float multi_func2(float a, float b, float c, float *pa,
			float *pb, float *pc)
{
	float *ptra = pa, *ptrb = pb, *ptrc = pc;

	a *= *ptra;
	b *= *ptrb;
	c *= *ptrc;
	rt_thread_delay(0);
	return (float)multi_func3(a, c, b, ptra, ptrb, ptrc);
}

float multi_func(float a, float b, float c, float *pa,
			float *pb, float *pc)
{
	float *ptra = pa, *ptrb = pb, *ptrc = pc;

	a *= *ptra;
	b *= *ptrb;
	c *= *ptrc;
	rt_thread_delay(0);
	return (float)multi_func2(a, c, b, ptra, ptrb, ptrc);
}

float div_func3(float a, float b, float c, float *pa,
			float *pb, float *pc)
{
	a = *pa / b;
	b = *pb / c;
	c = *pc / a;
	rt_thread_delay(0);
	return (float)multi_func(a, c, b, pa, pb, pc);
}

float div_func2(float a, float b, float c, float *pa,
			float *pb, float *pc)
{
	a = *pa / b;
	b = *pb / c;
	c = *pc / a;
	rt_thread_delay(0);
	return (float)div_func3(a, b, c, &a, &b, &c);
}

float div_func(float a, float b, float c, float *pa,
			float *pb, float *pc)
{
	a = *pa / b;
	b = *pb / c;
	c = *pc / a;
	rt_thread_delay(0);
	return (float)div_func2(a, b, c, &a, &b, &c);
}

float float_test_func(float a, float b, float c)
{
	float ta = a, tb = b, tc = c, fm;
	int i = 0;

	for (i = 0; i < 5; i++) {
		fm = multi_func(ta, tb, tc, &tc, &tb, &ta);
		rt_thread_delay(0);
		fm = multi_func(fm, tb, tc, &tc, &fm, &ta);
		rt_thread_delay(0);
		fm = div_func(fm, ta, tc, &ta, &tc, &tb);
		fm *= 1000;
		rt_thread_delay(0);
	}

	return (float)fm;
}

/* -------------------------- julia programe -------------------------- */
#define RGB(r, g, b)		((unsigned int)(((unsigned char)(r) |		\
					((unsigned short)((unsigned char)(g)) << 8))	\
					| (((unsigned int)(unsigned char)(b)) << 16)))
unsigned int float_test_func2()
{
	float m, dx, dy, x, y, x_n, y_n, deltax = 0.009, deltay = 0.999;
	int n, i, j, L = 4;
	int a[8] = { RGB(255, 0, 0), RGB(255, 128, 0), RGB(255, 255, 0), RGB(0, 255, 0),
			RGB(0, 0, 255), RGB(255, 0, 128), RGB(255, 255, 255)};
	unsigned int ret = 0;

	dx = 3.0 / 1440;
	dy = 3.0 / 900;
	for (i = 0; i < 1440; i++) {
		for (j = 0; j < 850; j++) {
			x = -1.5 + i * dx;
			y = -1.5 + j * dy;

			for (n = 0; n <= 1000; n++) {
				x_n = x * x - y * y + deltax;
				y_n = 2 * x * y + deltay;
				m = x_n * x_n;
				if (m > L)
					break;
				x = x_n;
				y = y_n;
			}

			ret += a[(int)(n) % 8];
		}

		if (!(i % 500))
			rt_thread_delay(0);
	}

	return ret;
}
/* -------------------------- julia programe -------------------------- */

#endif /* LOMBO_TEST_PTHREAD_MUTEX || LOMBO_TEST_VFP || LOMBO_TEST_NEON */

