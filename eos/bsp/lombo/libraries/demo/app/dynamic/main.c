/*
 * main.c - Lombo dynamic library demo code for LomboTech
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

#define MOD_NAME	"app_lib"
#include "common.h"
#include <dlfcn.h>

/* Path of dynamic library, here saved in Sdcard root*/
#define APP_PATH    "/libdynamic.so"

/* Function pointer, must be the same in the library */
typedef int (*add_func_t)(int, int);
typedef void (*lib_func_t)(void);

int main(int argc, char **argv)
{
	lib_func_t lib_function;
	add_func_t add_function;
	void *handle;

	LOG("start");

	/* open .so */
	handle = dlopen(APP_PATH, RTLD_LAZY);
	if (!handle) {
		LOG("dlopen %s failed!", APP_PATH);
		return -1;
	}
	LOG("dlopen %s success, ret handle %p!", APP_PATH, handle);

	/* find symbol lib_test_cxx */
	lib_function = (lib_func_t)dlsym(handle, "lib_test_cxx");
	if (!lib_function) {
		LOG("find symbol lib_test_cxx failed!");
		goto end;
	}
	LOG("find symbol lib_test_cxx success, ret handle %p!", lib_function);

	lib_function();

	/* find symbol lib_test_add */
	add_function = (add_func_t)dlsym(handle, "lib_test_add");
	if (!add_function) {
		LOG("find symbol lib_test_add failed!");
		goto end;
	}
	LOG("find symbol lib_test_add success, ret handle %p!", add_function);

	LOG("add_function(3, 4) result is:%d", add_function(3, 4));

end:
	if (handle) {
		dlclose(handle);
		handle = NULL;
	}

	LOG("end");
	return 0;
}
