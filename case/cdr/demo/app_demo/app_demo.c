/*
 * app_demo.c - app demo for app developer
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

#include "app_demo.h"
#include <stdio.h>

static lb_int32 app_demo_start(app_t  *ap);
static lb_int32 app_demo_stop(app_t *ap);
static lb_int32 app_demo_suspend(app_t *ap);
static lb_int32 app_demo_resume(app_t *ap);
static lb_int32 app_demo_ctrl(app_t *ap, lb_uint32 cmd, lb_int32 aux0, void *aux1);

/* init app plugin interface struct */

static app_if_t appx = {
	app_demo_start,
	app_demo_stop,
	app_demo_suspend,
	app_demo_resume,
	app_demo_ctrl,

};

/**
 * app_demo_start -app init
 * @ap: APP struct pointer.
 *
 * This function use to init a app,when lb_app_open is called,this function is called.
 *
 * Returns 0
 */
static lb_int32 app_demo_start(app_t  *ap)
{
	lb_int32 ret = 0;

	return ret;
}

/**
 * app_demo_suspend -app suspend
 * @ap: APP struct pointer.
 *
 * This function use to suspend a app to standby status.when lb_app_suspend is called,
 * this function is called.
 *
 * Returns 0
 */
static lb_int32 app_demo_suspend(app_t *ap)
{
	lb_int32 ret = 0;

	return ret;
}

/**
 * app_demo_resume - app resume
 * @ap: APP struct pointer.
 *
 * This function use to resume a app from standby status.when lb_app_resume is called,
 * this function is called.
 *
 * Returns 0
 */

static lb_int32 app_demo_resume(app_t *ap)
{
	lb_int32 ret = 0;

	return ret;
}

/**
 * app_demo_ctrl -app command interface
 * @ap:   APP struct pointer.
 * @cmd:  command,defined in app_demo.h.
 * @aux0: auxiliary parameter 0.
 * @aux1: auxiliary parameter 1.
 *
 * This function use run the ctrl function of app.when lb_app_ctrl is called,
 * this function is called.
 *
 * Returns 0
 */
static lb_int32 app_demo_ctrl(app_t *ap, lb_uint32 cmd, lb_int32 aux0, void *aux1)
{
	lb_int32 ret = 0;

	switch (cmd) {
	case APP_ENTER_BACKGROUND:
		break;
	case APP_SET_ATTR:
		break;
	default:
		printf("err unsupport cmd:%d,aux0 %x\n", cmd, aux0);
		break;
	}

	return ret;
}

/**
 * app_demo_stop -app exit
 * @ap: APP struct pointer.
 *
 * This function use to exit a app,when lb_app_close is called,this function is called
 *
 * Returns 0
 */
static lb_int32 app_demo_stop(app_t *ap)
{
	lb_int32 ret = 0;

	return ret;
}

/**
 * get_app_if -get app plugin interface
 *
 * This function use to get app plugin interface struct.
 *
 * Returns app plugin interface struct.
 */
app_if_t *get_app_if()
{
	return &appx;

}

/**
 * main -msh entrance
 *
 * This function use to exec in msh env.
 *
 * Returns 0.
 */
lb_int32 main(lb_int32 argc, char *argv[])
{
	printf("\n");

	return 0;
}
