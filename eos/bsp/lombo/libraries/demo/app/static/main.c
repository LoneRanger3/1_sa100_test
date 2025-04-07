/*
 * main.c - Lombo static library demo code for LomboTech
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

#define MOD_NAME	"app_slib"
#include "common.h"
#include "slib.h"

int main(int argc, char **argv)
{
	int val;

	LOG("start");

	LOG("call slib_test_thread..");

	slib_test_thread();

	val = slib_test_add(3, 5);

	LOG("slib_test_add(3, 5) ret %d", val);

	LOG("end");
	return 0;
}
