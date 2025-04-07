/*
 * image.c - image code from file explorer
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
#include "cJSON.h"
#include <app_manage.h>
#include <mod_manage.h>
#include <pthread.h>
#include "lb_common.h"
#include "mars.h"
#include "image.h"
#include "view_stack.h"
#include "view_node.h"
#include "player.h"
#include "mod_media.h"
#include "fileexp_common.h"
#include "thumb_image.h"
#include "thumb_video.h"
#include <time.h>

static lb_int32 img_index;
static void *img_desert;

static lb_int32 thumb_start(void *para)
{
	image_thumb_start(para);
	video_thumb_start(para);

	return 0;
}

static lb_int32 thumb_stop(void *para)
{
	image_thumb_stop(para);
	video_thumb_stop(para);

	return 0;
}

/**
 * image_init - call the image_mod_init
 * @param: lb_obj_t object pointer.
 *
 * This function call the image_mod_init
 *
 * Returns 0
 */
lb_int32 image_init(void *param)
{
	lb_int32 ret = -1;

	static v_node_t node0;
	static v_node_t node1;

	node0.init_op = thumb_stop;
	node0.exit_op = image_mod_exit;
	node0.next = &node1;

	node1.init_op = image_mod_init;
	node1.exit_op = thumb_start;
	node1.next = NULL;

	ret = view_stack_push(&node0);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		return ret;
	}

	if (node0.init_op)
		node0.init_op((void *)0);

	if (node1.init_op)
		node1.init_op((void *)0);

	return 0;
}

/**
 * image_exit - call the nothing
 * @param: lb_obj_t object pointer.
 *
 * This function  call the nothing
 *
 * Returns 0
 */
lb_int32 image_exit(void *param)
{
	lb_int32 ret = 0;

	return ret;
}

/**
 * image_mod_init - initial the mod of image
 * @param: lb_obj_t object pointer.
 *
 * This function initial the mod of image
 *
 * Returns 0
 */
lb_int32 image_mod_init(void *param)
{
	lb_int32 ret = 0;
	char *path = NULL;

	ret = mod_media_photo_show_start();
	if (ret != 0) {
		APP_LOG_W("failed\n");
		return ret;
	}

	ret = mod_media_photo_set_win_level(1);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		return ret;
	}

	ret = mars_get_node_path(img_desert, img_index, &path);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		return ret;
	}

	ret = mod_media_photo_show(path);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		return ret;
	}

	return ret;
}

/**
 * image_mod_init - exit the mod of image
 * @param: lb_obj_t object pointer.
 *
 * This function exit the mod of image
 *
 * Returns 0
 */
lb_int32 image_mod_exit(void *param)
{
	lb_int32 ret = 0;

	ret = mod_media_photo_show_end();
	if (ret != 0) {
		APP_LOG_W("failed\n");
		return 0;
	}

	return ret;
}

/**
 * image_set - set the image list and index
 * @param: lb_obj_t object pointer.
 *
 * This function set the image list and index
 *
 * Returns 0
 */
lb_int32 image_set(void *desert, lb_int32 index)
{
	if (desert != NULL)
		img_desert = desert;

	if (index >= 0)
		img_index = index;

	return 0;
}

/**
 * image_imgbtn_prev - response function for prev imgbtn
 * @param: imgbtn object pointer.
 *
 * This function use to response to prev imgbtn, jump to prev media file
 *
 * Returns 0 if called success ; otherwise, return -1
 */
lb_int32 image_imgbtn_prev(void *param)
{
	lb_int32 ret = -1;
	lb_int32 num = -1;
	char *path = NULL;

	num = mars_get_node_num(img_desert);
	if (num == -1) {
		APP_LOG_W("failed\n");
		return ret;
	}

	if (img_index > 0)
		img_index--;
	else
		img_index = (num - 1);

	ret = mars_get_node_path(img_desert, img_index, &path);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		return 0;
	}

	ret = mod_media_photo_show(path);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		return 0;
	}

	ret = mod_media_photo_set_win_level(1);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		return ret;
	}

	ret = 0;
	return ret;
}

/**
 * image_imgbtn_next - response function for next imgbtn
 * @param: imgbtn object pointer.
 *
 * This function use to response to back imgbtn, jump to next media file
 *
 * Returns 0 if called success ; otherwise, return -1
 */
lb_int32 image_imgbtn_next(void *param)
{
	lb_int32 ret = -1;
	lb_int32 num = -1;
	char *path = NULL;

	num = mars_get_node_num(img_desert);
	if (num == -1) {
		APP_LOG_W("failed\n");
		return ret;
	}

	if (img_index < (num - 1))
		img_index++;
	else
		img_index = 0;

	ret = mars_get_node_path(img_desert, img_index, &path);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		return 0;
	}

	ret = mod_media_photo_show(path);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		return 0;
	}

	ret = mod_media_photo_set_win_level(1);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		return ret;
	}

	ret = 0;
	return ret;
}

/**
 * image_reg_init -reg init function for widgets
 *
 * This function use to register init function
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 image_reg_init(void)
{
	lb_int32	err = 0;

	err |= lb_fmngr_reg_init_func("image_init", image_init);
	err |= lb_fmngr_reg_exit_func("image_exit", image_exit);

	return err;
}


/**
 * image_unreg_init -unreg init function
 *
 * This function use to unregister init function
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 image_unreg_init(void)
{
	lb_int32	err = 0;

	err |= lb_fmngr_unreg_init_func(image_init);
	err |= lb_fmngr_unreg_exit_func(image_exit);

	return err;
}

/**
 * image_reg_resp - reg response function
 *
 * This function use to register response function
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 image_reg_resp(void)
{
	lb_int32 err = 0;

	err |= lb_reg_resp_msg_func(LB_MSG_FILEEXP_IMG_NEXT, image_imgbtn_next);
	err |= lb_reg_resp_msg_func(LB_MSG_FILEEXP_IMG_PREV, image_imgbtn_prev);

	return err;
}

/**
 * image_unreg_resp - unreg response function
 *
 * This function use to unregister response function
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 image_unreg_resp(void)
{
	lb_int32	err = 0;

	err |= lb_unreg_resp_msg_func(image_imgbtn_next);
	err |= lb_unreg_resp_msg_func(image_imgbtn_prev);

	return err;
}
