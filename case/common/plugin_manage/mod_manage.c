/*
 * file mod_manage.c - Common code for LomboTech Socs
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
#include <mod_manage.h>
#include <dlfcn.h>
#include "dlmodule.h"
#include "plugin_manage_common.h"

/**
 * mod_manage_list:mod manage list,store all opened mod
 */
static mod_manage_t *mod_manage_list;

/**
 * lb_mod_open - open a ''*.mod" file
 * @mp: module struct pointer,must be a init var.
 * @mod_path: path of module
 * @mode: open mode ,defined in eos/components/libc/libdl/dlfcn.h
 *
 * This function  use to open a module plugin,and run the init function of module plugin
 *
 * Returns -1 if called when get errors ; otherwise, return other values,the mp struct
 * members will be assigned
 */
lb_int32 lb_mod_open(mod_t *mp, const char *mod_path, lb_uint8 mode)
{
	int result = -1;
	char *mod_name = NULL;
	mod_if_t *(*mod_if)();
	lb_int32(*m_sym)(mod_t *mp);
	mod_manage_t *mod_manage_cur = NULL;
	mod_manage_t *mod_manage_temp = NULL;

	if (!mp) {
		printf("err struct type mod_t var is NULL ,Maybe not be allocted\n");
		return result;
	}
	if (!mod_path) {
		printf("err mod_path is NULL ,Maybe not be allocted\n");
		return result;
	}
	if (mp->mh || mp->mod_name || mp->modx_if) {
		printf("err mod is opened or struct type mod_t var is not init\n");
		return result;
	}

	mp->mh = (lb_int32)(dlopen(mod_path, mode));
	if (!mp->mh) {
		printf("dlopen fail,mod_path:%s is not existed or illegal\n", mod_path);
		return result;
	}
	mod_name = covert_path2basename(mod_path);
	mp->mod_name = malloc(strlen(mod_name) + 1);
	memset(mp->mod_name, 0, strlen(mod_name) + 1);
	strcpy(mp->mod_name, mod_name);
	free(mod_name);

	mod_if = dlsym((struct rt_dlmodule *)(mp->mh), "get_mod_if");
	if (mod_if) {
		mp->modx_if = mod_if();
		if (mp->modx_if)
			m_sym = ((mod_if_t *)mp->modx_if)->init;
		else {
			printf("err get_mod_if return NULL\n");
			goto EXIT;
		}
		if (m_sym)
			result = m_sym(mp);
		else {
			printf("err can't get init function\n");
			goto EXIT;
		}
	} else {
		result = -1;
		printf("err get_app_if fail\n");
		goto EXIT;
	}

	if (mod_manage_list == NULL) {
		mod_manage_list = malloc(sizeof(mod_manage_t));
		memset(mod_manage_list, 0, sizeof(mod_manage_t));
		mod_manage_list->modx = mp;
		mod_manage_list->next = NULL;
		return result;
	} else {
		mod_manage_cur = mod_manage_list;
		while (mod_manage_cur) {
			if (mod_manage_cur->next == NULL) {
				mod_manage_temp = malloc(sizeof(mod_manage_t));
				if (mod_manage_temp == NULL) {
					result = -1;
					printf("mod_manage_temp malloc fail\n");
					goto EXIT;
				}
				memset(mod_manage_temp, 0, sizeof(mod_manage_t));
				mod_manage_temp->modx = mp;
				mod_manage_temp->next = NULL;
				mod_manage_cur->next = mod_manage_temp;
				return result;
			} else
				mod_manage_cur = mod_manage_cur->next;
		}
	}
EXIT:
	if (mp->mod_name) {
		free(mp->mod_name);
		mp->mod_name = NULL;
	}
	if (mp->mh) {
		dlclose((struct rt_dlmodule *)(mp->mh));
		mp->mh = 0;
	}

	return result;
}
RTM_EXPORT(lb_mod_open);

/**
 * lb_mod_close - close a module
 * @mp: module struct pointer,must be opened.
 *
 * This function  use to run the exit function of module,and close a module plugin
 *
 * Returns -1 if called when get errors ; otherwise, return other values,the mp struct
 * members will be zero clearing
 */
lb_int32 lb_mod_close(mod_t *mp)
{
	lb_int32 result = -1;
	lb_int32 found_flag = -1;
	lb_int32(*m_sym)(mod_t *mp);
	mod_manage_t *mod_manage_cur = NULL;
	mod_manage_t *mod_manage_temp = NULL;

	if (mp && mp->mod_name && mp->modx_if)
		m_sym = ((mod_if_t *)mp->modx_if)->exit;
	else {
		printf("err struct mod_t var or mod_name or modx_if is NULL\n");
		return result;
	}

	if (m_sym) {
		result = m_sym(mp);
		mp->modx_if = NULL;
	} else {
		printf("err can't get exit function\n");
		goto EXIT;
	}

	if (mod_manage_list == NULL) {
		result = -1;
		printf("err mod_manage_list is NULL!\n");
		goto EXIT;
	} else if (mp->mh == 0) {
		printf("err mp->mh is zero!\n");
		goto EXIT;
	} else {
		if (mp->mh == mod_manage_list->modx->mh) {
			mod_manage_temp = mod_manage_list->next;
			free(mod_manage_list);
			mod_manage_list = mod_manage_temp;
			found_flag = 1;
		} else {
			mod_manage_temp = mod_manage_list;
			while (mod_manage_temp->next) {
				mod_manage_cur = mod_manage_temp;
				mod_manage_temp = mod_manage_temp->next;
				if (mp->mh == mod_manage_temp->modx->mh) {
					mod_manage_cur->next = mod_manage_temp->next;
					free(mod_manage_temp);
					found_flag = 1;
					break;
				}
			}
		}
		if (found_flag != 1)
			printf("err can't found mod in mod list\n");
	}
EXIT:
	if (mp->mod_name) {
		free(mp->mod_name);
		mp->mod_name = NULL;
	}

	if (mp->mh) {
		dlclose((struct rt_dlmodule *)(mp->mh));
		mp->mh = 0;
	}

	return result;
}
RTM_EXPORT(lb_mod_close);

/**
 * lb_mod_read - read data from a module
 * @mp: module struct pointer,must be opened.
 * @mdata: must be allocted and used to store data read out
 * @size: number size of one block use to read data
 * @n: number block use to read data
 *
 * This function use to run the read function of module,and read n*size of data from
 * a module store in mdata
 *
 * Returns -1 if called when get errors ; otherwise, return real read out data size
 */
lb_int32 lb_mod_read(mod_t *mp, void *mdata, lb_uint32 size, lb_uint32 n)
{
	lb_int32 result = -1;
	lb_int32(*m_sym)(mod_t *mp, void *mdata, lb_uint32 size, lb_uint32 n);

	if (mp && mp->mod_name && mp->modx_if)
		m_sym = ((mod_if_t *)mp->modx_if)->read;
	else {
		printf("err struct mod_t var or mod_name or modx_if is NULL\n");
		return result;
	}

	if (m_sym && mdata)
		result = m_sym(mp, mdata, size, n);
	else
		printf("err can't get read function or read buffer is NULL\n");

	return result;
}
RTM_EXPORT(lb_mod_read);

/**
 * lb_mod_write - write data to a module
 * @mp: module struct pointer,must be opened.
 * @mdata: must be allocted and used to store data to write
 * @size: number size of one block use to write
 * @n: number block use to write data
 *
 * This function use to run the write function of module,and write n*size of data to
 * a module store in mdata
 *
 * Returns -1 if called when get errors ; otherwise, return real write data size
 */
lb_int32 lb_mod_write(mod_t *mp, const void *mdata, lb_uint32 size, lb_uint32 n)
{
	lb_int32 result = -1;
	lb_int32(*m_sym)(mod_t *mp, const void *mdata, lb_uint32 size, lb_uint32 n);

	if (mp && mp->mod_name && mp->modx_if)
		m_sym = ((mod_if_t *)mp->modx_if)->write;
	else {
		printf("err struct mod_t var or mod_name or modx_if is NULL\n");
		return result;
	}

	if (m_sym && mdata)
		result = m_sym(mp, mdata, size, n);
	else
		printf("err can't get write function or write buffer is NULL\n");

	return result;
}
RTM_EXPORT(lb_mod_write);

/**
 * lb_mod_ctrl - send or request cmd to a module
 * @mp: module struct pointer,must be opened.
 * @cmd: command,defined in module
 * @aux0: auxiliary parameters 0 ,accord to control command
 * @aux1: auxiliary parameters 1 ,accord to control command,if needed,must be
 * allocted and used to store data request from a module or send to a module
 *
 * This function use to run the ctrl function of module,and write n*size of data to
 * a module store in mdata
 *
 * Returns -1 if called when get errors ; otherwise, return other values
 */
lb_int32 lb_mod_ctrl(mod_t *mp, lb_uint32 cmd, lb_int32 aux0, void *aux1)
{
	lb_int32 result = -1;

	int (*m_sym)(mod_t *mp, lb_uint32 cmd, lb_int32 aux0, void *aux1);

	if (mp && mp->mod_name && mp->modx_if)
		m_sym = ((mod_if_t *)mp->modx_if)->ctrl;
	else {
		printf("err struct mod_t var or mod_name or modx_if is NULL\n");
		return result;
	}

	if (m_sym)
		result = m_sym(mp, cmd, aux0, aux1);
	else
		printf("err can't get ctrl function\n");

	return result;
}
RTM_EXPORT(lb_mod_ctrl);

/**
 * lb_mod_dump - dump all modules that is opened
 */
void lb_mod_dump()
{
	lb_int32 result = -1;
	mod_manage_t *mod_manage_cur = mod_manage_list;

	while (mod_manage_cur) {
		printf("%s\n", mod_manage_cur->modx->mod_name);
		mod_manage_cur = mod_manage_cur->next;
		result = 0;
	}
	if (result == -1)
		printf("err no mod opened\n");
}
RTM_EXPORT(lb_mod_dump);

/**
 * lb_mod_get_if - get interface of a module
 * @mp: module struct pointer,must be opened.
 *
 * This function use to get interface of a module
 * Returns NULL if called when get errors ; otherwise, return struct mod_if_t pointer
 */
mod_if_t *lb_mod_get_if(mod_t *mp)
{
	if (mp && mp->mod_name && mp->modx_if)
		return mp->modx_if;
	else {
		printf("err struct mod_t var or mod_name or modx_if is NULL\n");

		return NULL;
	}
}
RTM_EXPORT(lb_mod_get_if);
