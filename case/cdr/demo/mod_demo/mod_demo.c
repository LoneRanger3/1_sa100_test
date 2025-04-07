/*
 * mod_demo.c - module demo for module developer
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

#include "mod_demo.h"
#include <stdio.h>
#include <stdlib.h>

static lb_int32 mod_demo_init(mod_t *mp);
static lb_int32 mod_demo_exit(mod_t *mp);
static lb_int32 mod_demo_read(mod_t *mp, void *mdata, lb_uint32 size, lb_uint32 n);
static lb_int32 mod_demo_write(mod_t *mp, const void *mdata, lb_uint32 size, lb_uint32 n);
static lb_int32 mod_demo_ctrl(mod_t *mp, lb_uint32 cmd, lb_int32 aux0, void *aux1);

/* init module plugin interface struct */

static mod_if_t modx = {
	mod_demo_init,
	mod_demo_exit,
	mod_demo_read,
	mod_demo_write,
	mod_demo_ctrl,
};

/**
 * mod_demo_init - module init
 * @mp: module struct pointer.
 *
 * This function use to init a module,when lb_mod_open is called,this function is called.
 *
 * Returns 0
 */
static lb_int32 mod_demo_init(mod_t *mp)
{
	printf("\n");

	return 0;
}

/**
 * mod_demo_exit - module exit
 * @mp: module struct pointer.
 *
 * This function use to exit a module,when lb_mod_close is called,
 * this function is called.
 *
 * Returns 0
 */
static lb_int32 mod_demo_exit(mod_t *mp)
{
	printf("\n");

	return 0;
}

/**
 * mod_demo_read - module read interface
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
static lb_int32 mod_demo_read(mod_t *mp, void *mdata, lb_uint32 size, lb_uint32 n)
{
	memset(mdata, 0x55, size * n);
	printf("num:%d mdata[0]:%x\n", size * n, *(lb_uint8 *)mdata);

	return 0;
}

/**
 * mod_demo_write - module write  interface
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
static lb_int32 mod_demo_write(mod_t *mp, const void *mdata, lb_uint32 size, lb_uint32 n)
{
	lb_uint8 *mbuffer;

	printf("num:%d\n", size * n);

	mbuffer = malloc(size * n);
	if (mbuffer) {
		memcpy(mbuffer, mdata, size * n);
		printf("num:%d mbuffer[0]:%x\n", size * n, mbuffer[0]);
		free(mbuffer);
	}

	return 0;
}

/**
 * mod_demo_ctrl - module command interface
 * @mp:   module struct pointer.
 * @cmd:  command,defined in mod_demo.h.
 * @aux0: auxiliary parameter 0.
 * @aux1: auxiliary parameter 1.
 *
 * This function use run the ctrl function of module.when lb_mod_ctrl is called,
 * this function is called.
 *
 * Returns 0
 */
static lb_int32 mod_demo_ctrl(mod_t *mp, lb_uint32 cmd, lb_int32 aux0, void *aux1)
{
	lb_int32 param = 5;
	char *buffer;
	lb_uint8 *data;

	switch (cmd) {
	case MOD_DEMO_START:
		break;
	case MOD_DEMO_STOP:
		break;
	case MOD_DEMO_SET_PARM:
		param = aux0;
		break;
	case MOD_DEMO_GET_PARM:
		param = 0x44;
		*(lb_int32 *)aux1 = param;
		break;
	case MOD_DEMO_DUMP:
		data = malloc(128);
		memset(data, 0x11, 128);
		memcpy(aux1, data, 128);
		free(data);
		break;
	case MOD_DEMO_GET_VERSION:
		buffer = malloc(16);
		strcpy(buffer, "V101");
		strcpy(aux1, buffer);
		free(buffer);
		break;
	default:
		printf("err unsupport cmd:%d,aux0 %x\n", cmd, aux0);
		break;
	}

	return 0;
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
