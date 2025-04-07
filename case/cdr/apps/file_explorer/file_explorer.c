/*
 * file_explorer.c - file sub_system code for LomboTech
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
#include "file_explorer.h"
#include "fileexp_common.h"
#include "fileexp_cfg.h"
#include "cJSON.h"
#include "lb_types.h"
#include "lb_common.h"
#include "lb_ui.h"
#include "system/system.h"
#include "view_stack.h"
#include "view_node.h"
#include "thumb_image.h"
#include "thumb_video.h"
#include "image.h"
#include "video.h"
#include "player.h"
#include "mars.h"

static lb_int32 file_explorer_start(app_t  *ap);
static lb_int32 file_explorer_stop(app_t *ap);
static lb_int32 file_explorer_suspend(app_t *ap);
static lb_int32 file_explorer_resume(app_t *ap);
static lb_int32 file_explorer_ctrl(app_t *ap, lb_uint32 cmd, lb_int32 aux0, void *aux1);
static lb_int32 file_explorer_exit(void);

static lb_view_t *file_explorer_view;

static app_if_t appx = {
	file_explorer_start,
	file_explorer_stop,
	file_explorer_suspend,
	file_explorer_resume,
	file_explorer_ctrl,
};

static lb_int32 file_explorer_start(app_t  *ap)
{
	lb_int32 ret = 0;

	/* Initial the file management module */
	ret = mars_init(MARS_FD_PATH);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		return ret;
	}

	/* Load the media module */
	ret = mod_media_load();
	if (ret != 0) {
		APP_LOG_W("failed\n");
		return ret;
	}

	/* Initial the multi view exchange */
	ret = view_stack_init();
	if (ret != 0) {
		APP_LOG_W("failed\n");
		return ret;
	}

	/* Initial the setting of fileexp */
	ret = fileexp_set_init();
	if (ret != 0) {
		APP_LOG_W("failed\n");
		return ret;
	}

	/* Initial the style of fileexp */
	ret = lb_style_init("layout/file_explorer/styles.json");
	if (ret != 0) {
		APP_LOG_W("failed\n");
		return ret;
	}

	ret |= video_reg_init();
	ret |= video_reg_resp();
	ret |= image_reg_init();
	ret |= image_reg_resp();
	ret |= file_init_funcs();
	ret |= file_resp_funcs();
	if (ret != 0) {
		APP_LOG_W("failed\n");
		return ret;
	}

	ret = lb_ui_static_init
		("layout/file_explorer/main_view.json", &file_explorer_view);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		return ret;
	}

	return ret;
}

static lb_int32 file_explorer_suspend(app_t *ap)
{
	return 0;
}

static lb_int32 file_explorer_resume(app_t *ap)
{
	return 0;
}

static lb_int32 file_explorer_ctrl(app_t *ap, lb_uint32 cmd, lb_int32 aux0, void *aux1)
{
	lb_int32 ret = 0xff;
	lb_view_t *parent = NULL;
	v_node_t *node = NULL;

	switch (cmd) {
	case FILE_EXPLORER_RETURN:
		parent = lb_view_get_parent();
		if (parent) {
			if (view_stack_pop((void **)&node) == 0) {
				while (node) {
					if (node && node->exit_op)
						node->exit_op((void *)0);
					node = node->next;
				}
			}
			file_show_tab();
			lb_ui_send_msg(LB_MSG_RETURN_PARENT,
				(void *)&parent, sizeof(void *), 0);
		} else {
			if (file_list_return() != 0)
				ret = 0;
		}

		break;
	case FILE_EXPLORER_EXIT:
	{
		static lb_int32 type;
		void *temp;

		type = LB_MSG_FILEEXP_BASE;
		temp = &type;

		ret = file_explorer_exit();
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


static lb_int32 file_explorer_stop(app_t *ap)
{
	return 0;
}

static lb_int32 file_explorer_exit(void)
{
	lb_int32 ret = 0;
	v_node_t *node = NULL;

	view_stack_pop((void **)&node);
	while (node) {
		while (node) {
			if (node && node->exit_op)
				node->exit_op((void *)0);

			node = node->next;
		}
		view_stack_pop((void **)&node);
	}

	video_thumb_exit((void *)0);
	video_list_exit((void *)0);
	image_thumb_exit((void *)0);
	image_list_exit((void *)0);

	ret = file_unresp_funcs();
	RT_ASSERT(ret == 0);

	ret = image_unreg_resp();
	RT_ASSERT(ret == 0);

	ret = video_unreg_resp();
	RT_ASSERT(ret == 0);

	lb_ui_static_exit(&file_explorer_view);
	file_explorer_view = NULL;

	ret = file_uninit_funcs();
	RT_ASSERT(ret == 0);

	ret = image_unreg_init();
	RT_ASSERT(ret == 0);

	ret = video_unreg_init();
	RT_ASSERT(ret == 0);

	/* Exit the setting of fileexp */
	fileexp_set_exit();

	/* Exit the multi view exchange */
	view_stack_exit();

	/* Unload the media module */
	mod_media_unload();

	/* Exit the file management module */
	mars_exit();

	/* Dump the memory which forgot to free */
	mars_mem_dump();

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
