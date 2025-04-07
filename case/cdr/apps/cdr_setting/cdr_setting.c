/*
 * cdr_setting.c - setting of cdr code for LomboTech
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
#include "cdr_setting.h"
#include "cJSON.h"
#include "lb_types.h"
#include "lb_common.h"
#include "lb_ui.h"
#include "cdr_setting_ctrl.h"
#include "cdr_setting_cfg.h"

static lb_int32 cdr_setting_start(app_t  *ap);
static lb_int32 cdr_setting_stop(app_t *ap);
static lb_int32 cdr_setting_suspend(app_t *ap);
static lb_int32 cdr_setting_resume(app_t *ap);
static lb_int32 cdr_setting_ctrl(app_t *ap, lb_uint32 cmd, lb_int32 aux0, void *aux1);
static lb_int32 cdr_setting_exit(void);

static lb_view_t *cdr_setting_view;

static app_if_t appx = {
	cdr_setting_start,
	cdr_setting_stop,
	cdr_setting_suspend,
	cdr_setting_resume,
	cdr_setting_ctrl,
};

/**
 * cdr_setting_start - first func for this app
 * @param: lb_obj_t object pointer.
 *
 * This function means when you load the app this func is executed first
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
static lb_int32 cdr_setting_start(app_t  *ap)
{
	lb_int32 ret = 0;

	ret = init_funcs();
	RT_ASSERT(ret == 0);
	ret = resp_funcs();
	RT_ASSERT(ret == 0);

	ret = record_set_init();
	if (ret != 0) {
		printf("[%s,%d,failed]\n", __FILE__, __LINE__);
		ret = -1;
		goto exit;
	}

	ret = lb_style_init("layout/cdr_setting/styles.json");
	if (ret != 0) {
		printf("[%s,%d,failed]\n", __FILE__, __LINE__);
		ret = -1;
		goto exit;
	}

	ret = lb_ui_static_init("layout/cdr_setting/cdr_setting.json",
			&cdr_setting_view);
	if (ret != 0) {
		printf("[%s,%d,failed]\n", __FILE__, __LINE__);
		ret = -1;
		goto exit;
	}

	return ret;

exit:
	lb_ui_static_exit(&cdr_setting_view);
	cdr_setting_view = NULL;

	record_set_save();
	record_set_exit();

	unresp_funcs();
	uninit_funcs();

	return ret;
}

static lb_int32 cdr_setting_suspend(app_t *ap)
{
	return 0;
}

static lb_int32 cdr_setting_resume(app_t *ap)
{
	return 0;
}

/**
 * cdr_setting_ctrl - provide some user-defined interface for other app
 * @param: lb_obj_t object pointer.
 *
 * This function provide some user-defined interface for other app
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
static lb_int32 cdr_setting_ctrl(app_t *ap, lb_uint32 cmd, lb_int32 aux0, void *aux1)
{
	lb_int32 ret = 0xff;
	lb_view_t *parent = NULL;

	switch (cmd) {
	case CDR_SETTING_RETURN:
		parent = lb_view_get_parent();
		if (parent != NULL) {
			/* exchange internal view in app */
			printf("internal return\n");
			lb_ui_send_msg(LB_MSG_RETURN_PARENT,
				(void *)&parent, sizeof(void *), 0);
			ret = 0xff;
		} else {
			/* exchange external view on app */
			printf("external return\n");
			ret = 0;
		}
		break;
	case CDR_SETTING_EXIT:
	{
		static lb_int32 type;
		void *temp;

		type = LB_MSG_CDR_SETTING_BASE;
		temp = &type;

		ret = cdr_setting_exit();
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

/**
 * cdr_setting_stop - final func for this app
 * @param: lb_obj_t object pointer.
 *
 * This function means when you load the app this func is executed final
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
static lb_int32 cdr_setting_stop(app_t *ap)
{
	return 0;
}

static lb_int32 cdr_setting_exit(void)
{
	lb_int32 ret = 0;

	ret = unresp_funcs();
	RT_ASSERT(ret == 0);

	lb_ui_static_exit(&cdr_setting_view);
	cdr_setting_view = NULL;

	ret = uninit_funcs();
	RT_ASSERT(ret == 0);

	ret = record_set_save();
	RT_ASSERT(ret == 0);
	ret = record_set_exit();
	RT_ASSERT(ret == 0);

	return ret;
}


app_if_t *get_app_if()
{
	return &appx;
}

lb_int32 main(lb_int32 argc, char **argv)
{
	return 0;
}
