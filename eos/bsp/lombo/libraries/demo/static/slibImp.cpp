/*
 * slibImp.cpp - Lombo static library slibImp source code for LomboTech
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

#define MOD_NAME	"slib"
#include "common.h"
#include "slibImp.h"
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif

int *pvalue  = NULL;

Slib::Slib()
	: a(0)
{
	pvalue  = new int;
	LOG("new int ret %p", pvalue);

	*pvalue = 29494;
	LOG("after set *pvalue to 29494, *pvalue is %d", *pvalue);

	delete pvalue;
}

void Slib::setA(int value)
{
	std::vector<int> b;

	b.push_back(1024);

	LOG("after push_back, vector b[0] is %d", b[0]);

	a = value;
}

int Slib::getA(void)
{
	return a;
}

void Slib::toString()
{
	LOG("a = %d", a);
}

int test_cxx(void)
{
	Slib lib;

	LOG("start");

	lib.setA(100);
	lib.toString();

	LOG("end");
	return 0;
}

#ifdef __cplusplus
}
#endif
