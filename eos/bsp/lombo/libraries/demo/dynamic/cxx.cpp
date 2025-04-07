/*
 * cxx.cpp - Lombo c++ class code for LomboTech
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

#ifdef __cplusplus
extern "C" {
#endif

#define MOD_NAME	"lib"
#include "common.h"
#include "cxx.h"
#include <rtthread.h>

 A::A() : val(0)
{
	pv  = new int;

	LOG("class A: new int return %p", pv);

	*pv = 19494;

	LOG("class A: after set to 19494, *pv is %d", *pv);
	delete pv;
}

void A::set_val(int value)
{
	LOG("set A: val to %d", value);
	val = value;
}

int A::get_val(void)
{
	LOG("get A: val %d", val);
	return val;
}

int test_cxx_class(void)
{
	int val;
	A a;

	LOG("start");

	a.set_val(80);
	val = a.get_val();

	LOG("end");
	return 0;
}

#ifdef __cplusplus
}
#endif
