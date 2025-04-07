/*
 * interface.c - static library interface defination
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
#include "slib.h"
#include "pthread.h"

int test_cxx(void);

void *test_thread(void *param)
{
	int i;

	for (i = 5; i > 0; i--)
		LOG("test thread index: %d", i);

	pthread_exit(0);
	return NULL;
}

void slib_test_thread(void)
{
	pthread_t tid;

	LOG("start!");

	if (pthread_create(&tid, NULL, test_thread, NULL) != 0) {
		LOG("create test_thread failed");
		return;
	}

	pthread_join(tid, NULL);

	LOG("end!");
}

int slib_test_cxx(void)
{
	LOG("start!");

	test_cxx();

	LOG("end!");
	return 0;
}

int slib_test_add(int a, int b)
{
	int res;

	LOG("start!");

	res = a + b;

	LOG("end!");
	return res;
}

