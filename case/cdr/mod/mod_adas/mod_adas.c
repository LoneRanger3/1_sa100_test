/*
 * mod_adas.c - module adas for module developer
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

#include "mod_adas.h"
#include "car_adas.h"
#include <stdio.h>
#include <stdlib.h>
pthread_mutex_t adas_mutex; /* adas mutex */

static lb_int32 mod_adas_init(mod_t *mp);
static lb_int32 mod_adas_exit(mod_t *mp);
static lb_int32 mod_adas_read(mod_t *mp, void *mdata, lb_uint32 size, lb_uint32 n);
static lb_int32 mod_adas_write(mod_t *mp, const void *mdata, lb_uint32 size, lb_uint32 n);
static lb_int32 mod_adas_ctrl(mod_t *mp, lb_uint32 cmd, lb_int32 aux0, void *aux1);

/* init module plugin interface struct */

static mod_if_t modx = {
	mod_adas_init,
	mod_adas_exit,
	mod_adas_read,
	mod_adas_write,
	mod_adas_ctrl,
};

/**
 * mod_adas_init - module init
 * @mp: module struct pointer.
 *
 * This function use to init a module,when lb_mod_open is called,this function is called.
 *
 * Returns 0
 */
static lb_int32 mod_adas_init(mod_t *mp)
{
	MOD_LOG_D("\n");
	pthread_mutex_init(&adas_mutex, NULL);

	return 0;
}

/**
 * mod_adas_exit - module exit
 * @mp: module struct pointer.
 *
 * This function use to exit a module,when lb_mod_close is called,
 * this function is called.
 *
 * Returns 0
 */
static lb_int32 mod_adas_exit(mod_t *mp)
{
	MOD_LOG_D("\n");
	pthread_mutex_destroy(&adas_mutex);

	return 0;
}

/**
 * mod_adas_read - module read interface
 * @mp:    module struct pointer.
 * @mdata: store read data.
 * @size:  one block size.
 * @n:     block num.
 *
 * This function use run the read function of module.when lb_mod_read is called,
 * this function is called.
 *
 * Returns 0
 */
static lb_int32 mod_adas_read(mod_t *mp, void *mdata, lb_uint32 size, lb_uint32 n)
{
	return 0;
}

/**
 * mod_adas_write - module write  interface
 * @mp:    module struct pointer.
 * @mdata: store write data.
 * @size:  one block size.
 * @n:     block num.
 *
 * This function use run the write function of module.when lb_mod_write is called,
 * this function is called.
 *
 * Returns 0
 */
static lb_int32 mod_adas_write(mod_t *mp, const void *mdata, lb_uint32 size, lb_uint32 n)
{
	return 0;
}

/**
 * mod_adas_ctrl - module command interface
 * @mp:   module struct pointer.
 * @cmd:  command,defined in mod_adas.h.
 * @aux0: auxiliary parameter 0.
 * @aux1: auxiliary parameter 1.
 *
 * This function use run the ctrl function of module.when lb_mod_ctrl is called,
 * this function is called.
 *
 * Returns 0
 */
static lb_int32 mod_adas_ctrl(mod_t *mp, lb_uint32 cmd, lb_int32 aux0, void *aux1)
{
	int ret = 0;

	switch (cmd) {
	case MOD_ADAS_START:
		if (aux1) {
			pthread_mutex_lock(&adas_mutex);
			car_adas_set_status(adas_status_open);
			adas_create(aux1);
			pthread_mutex_unlock(&adas_mutex);
		} else
			MOD_LOG_E("err para aux1,cmd:%d,aux1 %p\n", cmd, aux1);
		break;
	case MOD_ADAS_STOP:
		if (aux1) {
			pthread_mutex_lock(&adas_mutex);
			adas_delete(aux1);
			car_adas_set_status(adas_status_close);
			pthread_mutex_unlock(&adas_mutex);
		} else
			MOD_LOG_E("err para aux1,cmd:%d,aux1 %p\n", cmd, aux1);
		break;
	case MOD_ADAS_SET_RESULT_CB:
		adas_result_cb = aux1;
		break;
	case MOD_ADAS_SUSPEND:
		pthread_mutex_lock(&adas_mutex);
		car_adas_set_status(adas_status_suspend);
		pthread_mutex_unlock(&adas_mutex);
		break;
	case MOD_ADAS_RESUME:
		pthread_mutex_lock(&adas_mutex);
		car_adas_set_status(adas_status_resume);
		pthread_mutex_unlock(&adas_mutex);
		break;
	case MOD_ADAS_GET_STATUS:
		pthread_mutex_lock(&adas_mutex);
		*(int *)aux1 = car_adas_get_status();
		pthread_mutex_unlock(&adas_mutex);
		break;
	default:
		MOD_LOG_W("err unsupport cmd:%d,aux0 %x\n", cmd, aux0);
		ret = -1;
		break;
	}

	return ret;
}

/**
 * get_mod_if -get module plugin interface
 *
 * This function use to get module plugin interface struct.
 *
 * Returns module plugin interface struct.
 */
mod_if_t *get_mod_if()
{
	return &modx;
}
