/*
 * system_setting.c - setting of system code for LomboTech
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
#include <debug.h>
#include <rtthread.h>
#include "system_setting.h"
#include "system_setting_cfg.h"
#include "system_setting_ctrl.h"
#include "cJSON.h"
#include "lb_types.h"
#include "lb_common.h"
#include "lb_ui.h"

static lb_int32 system_setting_start(app_t  *ap);
static lb_int32 system_setting_stop(app_t *ap);
static lb_int32 system_setting_suspend(app_t *ap);
static lb_int32 system_setting_resume(app_t *ap);
static lb_int32 system_setting_ctrl(app_t *ap, lb_uint32 cmd, lb_int32 aux0, void *aux1);
static lb_int32	system_setting_exit(void);

static lb_view_t *system_setting_view;

static app_if_t appx = {
	system_setting_start,
	system_setting_stop,
	system_setting_suspend,
	system_setting_resume,
	system_setting_ctrl,
};

static lb_int32 system_setting_start(app_t  *ap)
{
	lb_int32 ret = 0;

	init_funcs();
	resp_funcs();

	ret |= system_set_init();
	ret |= lb_style_init("layout/system_setting/styles.json");
	ret |= lb_ui_static_init("layout/system_setting/system_setting.json",
			&system_setting_view);
	if (ret != 0) {
		APP_LOG_W("err lb ui init failed!\n");
		return ret;
	}

	return ret;
}

static lb_int32 system_setting_suspend(app_t *ap)
{
	return 0;
}

static lb_int32 system_setting_resume(app_t *ap)
{
	return 0;
}

static lb_int32	system_setting_exit(void)
{
	lb_int32 ret = 0;

	ret = unresp_funcs();
	RT_ASSERT(ret == 0);

	lb_ui_static_exit(&system_setting_view);
	system_setting_view = NULL;

	ret = uninit_funcs();
	RT_ASSERT(ret == 0);

	ret = system_set_save();
	RT_ASSERT(ret == 0);

	ret = system_set_exit();
	RT_ASSERT(ret == 0);

	return ret;
}
static lb_int32 system_setting_ctrl(app_t *ap, lb_uint32 cmd, lb_int32 aux0, void *aux1)
{
	lb_int32 ret = 0xff;
	lb_view_t *parent = NULL;

	switch (cmd) {
	case SYSTEM_SETTING_RETURN:
		parent = lb_view_get_parent();
		if (parent != NULL) {
			APP_LOG_D("internal return\n");
			lb_ui_send_msg(LB_MSG_RETURN_PARENT,
				(void *)&parent, sizeof(void *), 0);
			ret = 0xff;
		} else {
			APP_LOG_D("external return\n");
			ret = 0;
		}
		break;
	case SYSTEM_SETTING_EXIT:
	{
		static lb_int32 type;
		void *temp;

		type = LB_MSG_SYSTEM_SETTING_BASE;
		temp = &type;

		ret = system_setting_exit();
		if (ret < 0)
			return -1;
		ret |= lb_ui_send_msg(LB_MSG_HOME_BASE, &temp, sizeof(lb_int32), 0);
	}
		break;
	default:
		ret = 0xff;
		break;
	}
	return ret;
}

static lb_int32 system_setting_stop(app_t *ap)
{
	return 0;
}

app_if_t *get_app_if()
{
	return &appx;
}

lb_int32 main(lb_int32 argc, char **argv)
{
	return 0;
}
