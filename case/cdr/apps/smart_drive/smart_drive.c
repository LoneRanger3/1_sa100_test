/*
 * smart_drive.c - smart drive code for LomboTech
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
#include "cJSON.h"
#include "lb_types.h"
#include "lb_common.h"
#include "lb_ui.h"
#include "smart_drive.h"
#include "smart_drive_common.h"
#include "main_view.h"
#include "adas/adas_set.h"
#include "adas/adas_calib.h"
#include "pano/pano_set.h"
#include "pano/pano_calib.h"
#include "bsd/bsd_set.h"
#include "bsd/bsd_calib.h"
#include "view/view_node.h"
#include "view/view_stack.h"

static lb_int32 smart_drive_start(app_t  *ap);
static lb_int32 smart_drive_stop(app_t *ap);
static lb_int32 smart_drive_suspend(app_t *ap);
static lb_int32 smart_drive_resume(app_t *ap);
static lb_int32 smart_drive_ctrl(app_t *ap, lb_uint32 cmd, lb_int32 aux0, void *aux1);
static lb_int32 smart_drive_exit(void);

static lb_view_t *smart_drive_view;

static app_if_t appx = {
	smart_drive_start,
	smart_drive_stop,
	smart_drive_suspend,
	smart_drive_resume,
	smart_drive_ctrl,
};

static lb_int32 smart_drive_start(app_t  *ap)
{
	lb_int32 ret = 0;

	ret = view_stack_init();
	if (ret != 0) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return ret;
	}
	ret = adas_para_init();
	if (ret != 0) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return ret;
	}

	ret |= init_funcs();
	ret |= resp_funcs();
	if (ret != 0) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return ret;
	}

	ret |= adas_calib_init_funcs();
	ret |= adas_calib_resp_funcs();
	ret |= adas_set_init_funcs();
	ret |= adas_set_resp_funcs();
	if (ret != 0) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return ret;
	}

	ret |= pano_calib_init_funcs();
	ret |= pano_calib_resp_funcs();
	ret |= pano_set_init_funcs();
	ret |= pano_set_resp_funcs();
	if (ret != 0) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return ret;
	}

	ret |= bsd_calib_init_funcs();
	ret |= bsd_calib_resp_funcs();
	ret |= bsd_set_init_funcs();
	ret |= bsd_set_resp_funcs();
	if (ret != 0) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return ret;
	}

	ret = lb_style_init("layout/smart_drive/styles.json");
	if (ret != 0) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return ret;
	}

#if defined(ADAS_PANO_BSD)
	ret = lb_ui_static_init("layout/smart_drive/main_view_adas_pano_bsd.json",
			&smart_drive_view);
#elif defined(ADAS_PANO)
	ret = lb_ui_static_init("layout/smart_drive/main_view_adas_pano.json",
			&smart_drive_view);
#elif defined(PANO_BSD)
	ret = lb_ui_static_init("layout/smart_drive/main_view_pano_bsd.json",
			&smart_drive_view);
#elif defined(ADAS_BSD)
	//ret = lb_ui_static_init("layout/smart_drive/main_view_adas_bsd.json",
	//		&smart_drive_view);
	ret = lb_ui_static_init("layout/smart_drive/main_view_bsd.json",
			&smart_drive_view);
#else
	ret = lb_ui_static_init("layout/smart_drive/main_view_adas_pano_bsd.json",
			&smart_drive_view);
#endif

	if (ret != 0) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return ret;
	}

	load_main_view();

	return ret;
}

static lb_int32 smart_drive_suspend(app_t *ap)
{
	return 0;
}

static lb_int32 smart_drive_resume(app_t *ap)
{
	return 0;
}

static lb_int32 smart_drive_ctrl(app_t *ap, lb_uint32 cmd, lb_int32 aux0, void *aux1)
{
	lb_int32 ret = 0xff;
	lb_view_t *parent = NULL;
	v_node_t *node = NULL;

	switch (cmd) {
	case SMART_DRIVE_RETURN:

		parent = lb_view_get_parent();
		if (parent != NULL) {
			printf("internal return\n");
			lb_ui_send_msg(LB_MSG_RETURN_PARENT,
				(void *)&parent, sizeof(void *), 0);
			ret = 0xff;
		} else {
			printf("external return\n");
			ret = 0;
		}

		view_stack_pop((void **)&node);
		while (node) {
			if (node && node->exit_op)
				node->exit_op((void *)0);

			node = node->next;
		}

		break;
	case SMART_DRIVE_EXIT: {
		static lb_int32 type;
		void *temp;

		type = LB_MSG_SMART_DRIVE_BASE;
		temp = &type;

		ret = smart_drive_exit();
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


static lb_int32 smart_drive_stop(app_t *ap)
{
	return 0;
}

static lb_int32 smart_drive_exit(void)
{
	lb_int32 ret = 0;
	v_node_t *node = NULL;

	ret = adas_set_unresp_funcs();
	RT_ASSERT(ret == 0);

	ret = adas_calib_unresp_funcs();
	RT_ASSERT(ret == 0);

	ret = pano_set_unresp_funcs();
	RT_ASSERT(ret == 0);

	ret = pano_cailb_unresp_funcs();
	RT_ASSERT(ret == 0);

	ret = bsd_set_unresp_funcs();
	RT_ASSERT(ret == 0);

	ret = bsd_calib_unresp_funcs();
	RT_ASSERT(ret == 0);

	ret = unresp_funcs();
	RT_ASSERT(ret == 0);

	lb_ui_static_exit(&smart_drive_view);
	smart_drive_view = NULL;

	ret = adas_set_uninit_funcs();
	RT_ASSERT(ret == 0);

	ret = adas_calib_uninit_funcs();
	RT_ASSERT(ret == 0);

	ret = pano_set_uninit_funcs();
	RT_ASSERT(ret == 0);

	ret = pano_calib_uninit_funcs();
	RT_ASSERT(ret == 0);

	ret = bsd_set_uninit_funcs();
	RT_ASSERT(ret == 0);

	ret = bsd_calib_uninit_funcs();
	RT_ASSERT(ret == 0);

	ret = uninit_funcs();
	RT_ASSERT(ret == 0);

	view_stack_pop((void **)&node);
	while (node) {
		while (node) {
			if (node && node->exit_op)
				node->exit_op((void *)0);

			node = node->next;
		}
		view_stack_pop((void **)&node);
	}

	ret = adas_para_save();
	RT_ASSERT(ret == 0);

	ret = adas_para_exit();
	RT_ASSERT(ret == 0);

	view_stack_exit();

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
