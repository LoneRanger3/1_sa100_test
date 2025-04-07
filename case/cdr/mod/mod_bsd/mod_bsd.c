/*
 * mod_bsd.c - module bsd for module developer
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

#include "mod_bsd.h"
#include "car_bsd.h"
#include <stdio.h>
#include <stdlib.h>
pthread_mutex_t bsd_mutex; /* bsd mutex */

static lb_int32 mod_bsd_init(mod_t *mp);
static lb_int32 mod_bsd_exit(mod_t *mp);
static lb_int32 mod_bsd_read(mod_t *mp, void *mdata, lb_uint32 size, lb_uint32 n);
static lb_int32 mod_bsd_write(mod_t *mp, const void *mdata, lb_uint32 size, lb_uint32 n);
static lb_int32 mod_bsd_ctrl(mod_t *mp, lb_uint32 cmd, lb_int32 aux0, void *aux1);

/* init module plugin interface struct */

static mod_if_t modx = {
	mod_bsd_init,
	mod_bsd_exit,
	mod_bsd_read,
	mod_bsd_write,
	mod_bsd_ctrl,
};

/**
 * mod_bsd_init - module init
 * @mp: module struct pointer.
 *
 * This function use to init a module,when lb_mod_open is called,this function is called.
 *
 * Returns 0
 */
static lb_int32 mod_bsd_init(mod_t *mp)
{
	MOD_LOG_D("\n");
	pthread_mutex_init(&bsd_mutex, NULL);

	return 0;
}

/**
 * mod_bsd_exit - module exit
 * @mp: module struct pointer.
 *
 * This function use to exit a module,when lb_mod_close is called,
 * this function is called.
 *
 * Returns 0
 */
static lb_int32 mod_bsd_exit(mod_t *mp)
{
	MOD_LOG_D("\n");
	pthread_mutex_destroy(&bsd_mutex);

	return 0;
}

/**
 * mod_bsd_read - module read interface
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
static lb_int32 mod_bsd_read(mod_t *mp, void *mdata, lb_uint32 size, lb_uint32 n)
{
	return 0;
}

/**
 * mod_bsd_write - module write  interface
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
static lb_int32 mod_bsd_write(mod_t *mp, const void *mdata, lb_uint32 size, lb_uint32 n)
{
	return 0;
}

/**
 * mod_bsd_ctrl - module command interface
 * @mp:   module struct pointer.
 * @cmd:  command,defined in mod_bsd.h.
 * @aux0: auxiliary parameter 0.
 * @aux1: auxiliary parameter 1.
 *
 * This function use run the ctrl function of module.when lb_mod_ctrl is called,
 * this function is called.
 *
 * Returns 0
 */
static lb_int32 mod_bsd_ctrl(mod_t *mp, lb_uint32 cmd, lb_int32 aux0, void *aux1)
{
	int ret = 0;

	switch (cmd) {
	case MOD_BSD_START:
		if (aux1) {
			pthread_mutex_lock(&bsd_mutex);
			car_bsd_set_status(bsd_status_open);
			bsd_create(aux1);
			pthread_mutex_unlock(&bsd_mutex);
		} else
			MOD_LOG_E("err para aux1,cmd:%d,aux1 %p\n", cmd, aux1);
		break;
	case MOD_BSD_STOP:
		if (aux1) {
			pthread_mutex_lock(&bsd_mutex);
			bsd_delete(aux1);
			car_bsd_set_status(bsd_status_close);
			pthread_mutex_unlock(&bsd_mutex);
		} else
			MOD_LOG_E("err para aux1,cmd:%d,aux1 %p\n", cmd, aux1);
		break;
	case MOD_BSD_SET_RESULT_CB:
		bsd_result_cb = aux1;
		break;
	case MOD_BSD_SUSPEND:
		pthread_mutex_lock(&bsd_mutex);
		car_bsd_set_status(bsd_status_suspend);
		pthread_mutex_unlock(&bsd_mutex);
		break;
	case MOD_BSD_RESUME:
		pthread_mutex_lock(&bsd_mutex);
		car_bsd_set_status(bsd_status_resume);
		pthread_mutex_unlock(&bsd_mutex);
		break;
	case MOD_BSD_GET_STATUS:
		pthread_mutex_lock(&bsd_mutex);
		*(int *)aux1 = car_bsd_get_status();
		pthread_mutex_unlock(&bsd_mutex);
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
