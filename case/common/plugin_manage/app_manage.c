/*
 * file app_manage.c - Common code for LomboTech Socs
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
#include <app_manage.h>
#include <dlfcn.h>
#include "dlmodule.h"
#include "plugin_manage_common.h"

/**
 * app_manage_list:app manage list,store all opened app
 */
static app_manage_t *app_manage_list;

/**
 * lb_app_open - open a ''*.app" file
 * @ap: APP struct pointer,must be a init var.
 * @app_path: path of app
 * @mode: open mode ,defined in eos/components/libc/libdl/dlfcn.h
 *
 * This function  use to open a app plugin,and run the start function of app plugin.
 *
 * Returns -1 if called when get errors ; otherwise, return other values,the ap struct
 * members will be assigned
 */
lb_int32 lb_app_open(app_t *ap, char *app_path, lb_int32 mode)
{
	lb_int32 result = -1;
	char *app_name;
	app_if_t *(*app_if)();
	lb_int32(*a_sym)(app_t *ap);
	app_manage_t *app_manage_cur = NULL;
	app_manage_t *app_manage_temp = NULL;

	if (!ap) {
		printf("err struct type ap_t var is NULL ,Maybe not be allocted\n");
		return result;
	}
	if (!app_path) {
		printf("err app_path is NULL ,Maybe not be allocted\n");
		return result;
	}
	if (ap->ah || ap->app_name || ap->appx_if) {
		printf("err app is opened or struct type app_t var is not init\n");
		return result;
	}

	app_name = covert_path2basename(app_path);
	if (lb_app_check(app_name)) {
		free(app_name);
		printf("err app:%s is exsit,can't be opened again!\n", app_name);
		return result;
	}

	ap->ah = (lb_int32)(dlopen(app_path, mode));
	if (!ap->ah) {
		printf("dlopen fail,app_path:%s is not existed or illegal\n", app_path);
		free(app_name);
		return result;
	}
	ap->app_name = malloc(strlen(app_name) + 1);
	memset(ap->app_name, 0, strlen(app_name) + 1);
	strcpy(ap->app_name, app_name);
	free(app_name);

	app_if = dlsym((struct rt_dlmodule *)(ap->ah), "get_app_if");
	if (app_if) {
		ap->appx_if = app_if();
		if (ap->appx_if)
			a_sym = ((app_if_t *)ap->appx_if)->start;
		else {
			printf("err get_app_if return NULL\n");
			goto EXIT;
		}
		if (a_sym)
			result = a_sym(ap);
		else {
			printf("err can't get start function\n");
			goto EXIT;
		}

	} else {
		result = -1;
		printf("err get_app_if fail\n");
		goto EXIT;
	}

	if (app_manage_list == NULL) {
		app_manage_list = malloc(sizeof(app_manage_t));
		memset(app_manage_list, 0, sizeof(app_manage_t));
		app_manage_list->appx = ap;
		app_manage_list->next = NULL;
		return result;
	} else {
		app_manage_cur = app_manage_list;
		while (app_manage_cur) {
			if (NULL == app_manage_cur->next) {
				app_manage_temp = malloc(sizeof(app_manage_t));
				if (app_manage_temp == NULL) {
					result = -1;
					printf("app_manage_temp malloc fail\n");
					goto EXIT;
				}
				memset(app_manage_temp, 0, sizeof(app_manage_t));
				app_manage_temp->appx = ap;
				app_manage_temp->next = NULL;
				app_manage_cur->next = app_manage_temp;
				return result;
			} else
				app_manage_cur = app_manage_cur->next;
		}
	}
EXIT:
	if (ap->app_name) {
		free(ap->app_name);
		ap->app_name = NULL;
	}
	if (ap->ah) {
		dlclose((struct rt_dlmodule *)(ap->ah));
		ap->ah = 0;
	}

	return result;
}
RTM_EXPORT(lb_app_open);

/**
 * lb_app_close - close a app
 * @ap: app struct pointer,must be opened.
 *
 * This function  use to run the stop function of app,and close a app plugin
 *
 * Returns -1 if called when get errors ; otherwise, return other values,the ap struct
 * members will be zero clearing
 */
lb_int32 lb_app_close(app_t *ap)
{
	lb_int32 result = -1;
	lb_int32 found_flag = -1;
	lb_int32(*a_sym)(app_t *ap);
	app_manage_t *app_manage_cur = NULL;
	app_manage_t *app_manage_temp = NULL;

	if (ap && ap->app_name && ap->appx_if)
		a_sym = ((app_if_t *)ap->appx_if)->stop;
	else {
		printf("err struct app_t var or app_name or appx_if is NULL\n");
		return result;
	}

	if (a_sym) {
		result = a_sym(ap);
		ap->appx_if = NULL;
	} else {
		printf("err can't get stop function\n");
		goto EXIT;
	}

	if (app_manage_list == NULL) {
		result = -1;
		printf("err app_manage_list is NULL!\n");
		goto EXIT;
	} else if (ap->ah == 0) {
		printf("err ap->ah is zero!\n");
		goto EXIT;
	} else {
		if (ap->ah == app_manage_list->appx->ah) {
			app_manage_cur = app_manage_list->next;
			free(app_manage_list);
			app_manage_list = app_manage_cur;
			found_flag = 1;
		} else {
			app_manage_temp = app_manage_list;
			while (app_manage_temp->next) {
				app_manage_cur = app_manage_temp;
				app_manage_temp = app_manage_temp->next;
				if (ap->ah == app_manage_temp->appx->ah) {
					app_manage_cur->next = app_manage_temp->next;
					free(app_manage_temp);
					found_flag = 1;
					break;
				}
			}
		}
		if (found_flag != 1)
			printf("err can't found app in app list\n");
	}
EXIT:
	if (ap->app_name) {
		free(ap->app_name);
		ap->app_name = NULL;
	}

	if (ap->ah) {
		dlclose((struct rt_dlmodule *)(ap->ah));
		ap->ah = 0;
	}

	return result;
}
RTM_EXPORT(lb_app_close);

/**
 * lb_app_suspend - use for standby to suspend a app plugin
 * @ap: app struct pointer,must be opened.
 *
 * This function use to run the suspend function of app
 *
 * Returns -1 if called when get errors ; otherwise, return other valus.
 */
lb_int32 lb_app_suspend(app_t *ap)
{
	lb_int32 result = -1;
	lb_int32(*a_sym)(app_t *ap);

	if (ap && ap->app_name && ap->appx_if)
		a_sym = ((app_if_t *)ap->appx_if)->suspend;
	else {
		printf("err struct app_t var or app_name or appx_if is NULL\n");
		return result;
	}

	if (a_sym)
		result = a_sym(ap);
	else
		printf("err can't get suspend function\n");

	return result;
}
RTM_EXPORT(lb_app_suspend);

/**
 * lb_app_suspend - use for standby to resume a app plugin
 * @ap: app struct pointer,must be opened.
 *
 * This function use to run the resume function of app
 *
 * Returns -1 if called when get errors ; otherwise, return other valus.
 */
lb_int32 lb_app_resume(app_t *ap)
{
	lb_int32 result = -1;
	lb_int32(*a_sym)(app_t *ap);

	if (ap && ap->app_name && ap->appx_if)
		a_sym = ((app_if_t *)ap->appx_if)->resume;
	else {
		printf("err struct app_t var or app_name or appx_if is NULL\n");
		return result;
	}

	if (a_sym)
		result = a_sym(ap);
	else
		printf("err can't get resume function\n");

	return result;
}
RTM_EXPORT(lb_app_resume);

/**
 * lb_app_ctrl - send or request control commond to a app
 * @mp: module struct pointer,must be opened.
 * @cmd: command,defined in app plugin
 * @aux0: auxiliary parameters 0 ,accord to control command
 * @aux1: auxiliary parameters 1 accord to control command,if needed,must be allocted and
 * used to store data request from a app or send to a app
 *
 * This function use to run the ctrl function of app
 *
 * Returns -1 if called when get errors ; otherwise, return other values
 */
lb_int32 lb_app_ctrl(app_t *ap, lb_uint32 cmd, lb_int32 aux0, void *aux1)
{
	lb_int32 result = -1;
	lb_int32(*a_sym)(app_t *ap, lb_uint32 cmd, lb_int32 aux0, void *aux1);

	if (ap && ap->app_name && ap->appx_if)
		a_sym = ((app_if_t *)ap->appx_if)->ctrl;
	else {
		printf("err struct app_t var or app_name or appx_if is NULL\n");
		return result;
	}

	if (a_sym)
		result = a_sym(ap, cmd, aux0, aux1);
	else
		printf("err can't get ctrl function\n");

	return result;
}
RTM_EXPORT(lb_app_ctrl);

/**
 * lb_app_dump - dump all app plugin that is opened
 */
void lb_app_dump()
{
	lb_int32 result = -1;
	app_manage_t *app_manage_cur = app_manage_list;

	while (app_manage_cur) {
		printf("%s\n", app_manage_cur->appx->app_name);
		app_manage_cur = app_manage_cur->next;
		result = 0;
	}
	if (result == -1)
		printf("err no app  opened\n");
}
RTM_EXPORT(lb_app_dump);

/**
 * lb_app_check - check app is opened or not
 * @app_name: app name,basename of app path
 *
 * This function use to check app is opened or not
 *
 * Returns NULL if app_name of app is not opened ; otherwise, return struct app app_t
 * pointer vars of app_name,same app_name of app can't be opened for twice.
 */
app_t *lb_app_check(char *app_name)
{
	lb_int32 result = 0;
	app_manage_t *app_manage_cur = app_manage_list;

	while (app_manage_cur) {
		if (!strcmp(app_name, app_manage_cur->appx->app_name)) {
			result = 1;
			break;
		}
		app_manage_cur = app_manage_cur->next;
	}
	if (result == 0)
		return NULL;
	else
		return	app_manage_cur->appx;
}
RTM_EXPORT(lb_app_check);

/**
 * lb_app_get_if - get interface of a app
 * @ap: app struct pointer,must be opened.
 *
 * This function use to get interface of a app
 *
 * Returns NULL if called when get errors ; otherwise, return struct app_if_t pointer
 */
app_if_t *lb_app_get_if(app_t *ap)
{
	if (ap && ap->app_name && ap->appx_if)
		return ap->appx_if;
	else {
		printf("err struct app_t var or app_name or appx_if is NULL\n");

		return NULL;
	}
}
RTM_EXPORT(lb_app_get_if);
