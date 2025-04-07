/*
 * adas_calib.c - adas calibrate code for LomboTech
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
#include "lb_gal_common.h"

#include "smart_drive.h"
#include "smart_drive_common.h"
#include "main_view.h"
#include "bsd_calib.h"
#include "view/view_stack.h"
#include "view/view_node.h"

#include <oscl.h>
#include <base_component.h>
#include <omx_test.h>
#include "vrender_component.h"
#include "vrec_component.h"
#include "framework_common.h"
#include "lb_recorder.h"
#include <getopt.h>
#include "recorder_private.h"
#include "system/system.h"
#include "app_manage.h"
#include "case_config.h"
static void *eplayer;
static void *method_obj[2];
static void *view0_obj[3];
static lb_line_t *cali_line_up;
static lb_line_t *cali_line_dn;
static lb_line_t *cali_line_me;
static lb_point_t line_points[2][5];
static lb_int8 focus;
static lb_int8 prev_flag;

/**
 * load_method - save objects from the adas method view
 *
 * This function save objects from the adas method view
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
static lb_int32 bsd_load_method(void *para)
{
	lb_int32 ret = 0;
	lb_int32 i = 0;
	lb_uint32 id = 0;

	for (i = 0; i < 2; i++) {
		id = 200 + i;
		ret = lb_view_get_obj_ext_by_id(id, &method_obj[i]);
		if (0 != ret) {
			APP_LOG_W("failed\n");
			ret = -1;
			return ret;
		}
	}

	return ret;
}

/**
 * hide_method - hide the method view
 *
 * This function hide the main view
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
static lb_int32 bsd_hide_method(void *para)
{
	lb_int32 ret = 0;
	lb_int32 i = 0;

	for (i = 0; i < 2; i++) {
		ret = lb_gal_set_obj_hidden(method_obj[i], 1);
		if (0 != ret) {
			APP_LOG_W("failed\n");
			ret = -1;
			return ret;
		}
	}

	return ret;
}

/**
 * show_method - show the method view
 *
 * This function show the method view
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
static lb_int32 bsd_show_method(void *para)
{
	lb_int32 ret = 0;
	lb_int32 i = 0;

	for (i = 0; i < 2; i++) {
		ret = lb_gal_set_obj_hidden(method_obj[i], 0);
		if (0 != ret) {
			APP_LOG_W("failed\n");
			ret = -1;
			return ret;
		}
	}

	return ret;
}

/**
 * load_method - save objects from the adas method view
 *
 * This function save objects from the adas method view
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
static lb_int32 bsd_load_lines(void *para)
{
	lb_int32 ret = 0;
	lb_int32 i = 0;
	lb_uint32 id = 0;

	for (i = 0; i < 3; i++) {
		id = 200 + i;
		ret = lb_view_get_obj_ext_by_id(id, &view0_obj[i]);
		if (0 != ret) {
			APP_LOG_W("failed\n");
			ret = -1;
			return ret;
		}
	}

	return ret;
}

/**
 * hide_method - hide the method view
 *
 * This function hide the main view
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
static lb_int32 bsd_hide_lines(void *para)
{
	lb_int32 ret = 0;
#if 0
	lb_int32 i = 0;

	for (i = 0; i < 3; i++) {
		ret = lb_gal_set_obj_hidden(view0_obj[i], 1);
		if (0 != ret) {
			APP_LOG_W("failed\n");
			ret = -1;
			return ret;
		}
	}
#endif
	return ret;
}

/**
 * show_method - show the method view
 *
 * This function show the method view
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
static lb_int32 bsd_show_lines(void *para)
{
	lb_int32 ret = 0;
#if 0
	lb_int32 i = 0;

	for (i = 0; i < 3; i++) {
		ret = lb_gal_set_obj_hidden(view0_obj[i], 0);
		if (0 != ret) {
			APP_LOG_W("failed\n");
			ret = -1;
			return ret;
		}
	}
#endif

	return ret;
}


/**
 * prev_init - initial the mod to start preview
 *
 * This function initial the mod to start preview
 *
 * Returns 0 if called when get success ; otherwise, return -1
 */
static lb_int32 bsd_prev_init(void *para)
{
	lb_int32 ret = 0;
	win_para_t win_para;
	rec_param_t rec_para;
#ifdef SYS_MEMERY_128M_FOR_DOUBLE_1080P
	user_def_sys_cfg_t userdef_sys_cfg;
#endif

	memset(&win_para, 0x00, sizeof(win_para));
	memset(&rec_para, 0x00, sizeof(rec_para));

	eplayer = lb_recorder_creat();
	if (eplayer == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}
#ifdef SYS_MEMERY_128M_FOR_DOUBLE_1080P
	if ((FRONT_RECORDER_SOURCE_HEIGHT == 1080) &&
			(REAR_RECORDER_SOURCE_HEIGHT == 1080)) {
		userdef_sys_cfg.camera_buf_num = REAR_CAMERA_BUFFER_NUM;
		lb_recorder_ctrl(eplayer, LB_REC_SET_USER_DEF_SYS_PRAR, &userdef_sys_cfg);
	}
#endif
	lb_recorder_ctrl(eplayer, LB_REC_SET_VIDEO_SOURCE, "vic");

	lb_recorder_ctrl(eplayer, LB_REC_GET_PARA, &rec_para);
	rec_para.source_width = /*REAR_RECORDER_SOURCE_WIDTH*/1920;
	rec_para.source_height = /*REAR_RECORDER_SOURCE_HEIGHT*/2160;
	rec_para.width = REAR_RECORDER_SOURCE_WIDTH;
	rec_para.height = REAR_RECORDER_SOURCE_HEIGHT;
	rec_para.frame_rate = REAR_RECORDER_SOURCE_FPS * 1000;
	ret = lb_recorder_ctrl(eplayer, LB_REC_SET_PARA, &rec_para);
	if (ret < 0) {
		APP_LOG_E("LB_REC_SET_PARA failed!\n");
		ret = -1;
		return ret;
	}

	win_para.mode = /*VIDEO_WINDOW_FULL_SCREEN_VIDEO_RATIO*/VDISP_WINDOW_USERDEF;
	win_para.rect.x = BSD_CALIB_PREX;
	win_para.rect.y = BSD_CALIB_PREY;
	win_para.rect.width = BSD_CALIB_PREW;
	win_para.rect.height = BSD_CALIB_PREH;
	win_para.crop.x = 1080;
	win_para.crop.y = 0;
	win_para.crop.width = 1056;
	win_para.crop.height = 1920;
	lb_recorder_ctrl(eplayer, LB_REC_SET_DISP_PARA, (void *)&win_para);

	lb_recorder_ctrl(eplayer, LB_REC_SET_ROTATE, (void *)VIDEO_ROTATE_270);
	ret = lb_recorder_ctrl(eplayer, LB_REC_PREPARE, 0);
	if (ret < 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}


	ret = lb_recorder_ctrl(eplayer, LB_REC_PREVIEW, 0);
	if (ret < 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	lb_recorder_ctrl(eplayer, LB_REC_SET_LAYER_LEVEL,
		(void *)VIDEO_LAYER_BOTTOM);

	return ret;
}

/**
 * prev_exit - exit the mod to stop preview
 *
 * This function exit the mod to stop preview
 *
 * Returns 0 if called when get success ; otherwise, return -1
 */
static lb_int32 bsd_prev_exit(void *para)
{
	lb_int32 ret = 0;
	APP_LOG_W("\n");
	if (eplayer == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	ret = lb_recorder_ctrl(eplayer, LB_REC_STOP_PREVIEW, 0);
	if (ret < 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	lb_recorder_release(eplayer);
	prev_flag = 0;

	return ret;
}

/**
 * adas_preview_init - call a few of func to start preview
 * @param: lb_obj_t object pointer.
 *
 * This function  call a few of func to start preview
 *
 * Returns 0
 */
static lb_int32 view0_init(void *param)
{
	lb_int32 ret = 0;
	static v_node_t node0;
	static v_node_t node1;
	static v_node_t node2;

	node0.init_op = bsd_prev_init;
	node0.exit_op = bsd_prev_exit;
	node0.next = &node1;

	node1.init_op = hide_main_view;
	node1.exit_op = show_main_view;
	node1.next = &node2;

	node2.init_op = bsd_hide_method;
	node2.exit_op = bsd_show_method;
	node2.next = NULL;

	ret = view_stack_push(&node0);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	if (node0.init_op)
		node0.init_op((void *)0);

	if (node1.init_op)
		node1.init_op((void *)0);

	if (node2.init_op)
		node2.init_op((void *)0);

	return ret;
}

/**
 * adas_preview_exit - nothing to do
 * @param: lb_obj_t object pointer.
 *
 * This function calls nothing
 *
 * Returns 0
 */
static lb_int32 view0_exit(void *param)
{
	lb_int32 ret = 0;

	return ret;
}

/**
 * adas_preview_init - call a few of func to start preview
 * @param: lb_obj_t object pointer.
 *
 * This function  call a few of func to start preview
 *
 * Returns 0
 */
static lb_int32 view1_init(void *param)
{
	lb_int32 ret = 0;
	static v_node_t node0;

	node0.init_op = bsd_hide_lines;
	node0.exit_op = bsd_show_lines;
	node0.next = NULL;

	ret = view_stack_push(&node0);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	if (node0.init_op)
		node0.init_op((void *)0);

	return ret;
}

/**
 * adas_preview_exit - nothing to do
 * @param: lb_obj_t object pointer.
 *
 * This function calls nothing
 *
 * Returns 0
 */
static lb_int32 view1_exit(void *param)
{
	lb_int32 ret = 0;

	return ret;
}

/**
 * adas_preview_init - call a few of func to start preview
 * @param: lb_obj_t object pointer.
 *
 * This function  call a few of func to start preview
 *
 * Returns 0
 */
static lb_int32 bsd_reg_init(void *param)
{
	lb_int32 ret = 0;

	return ret;
}

/**
 * adas_preview_exit - nothing to do
 * @param: lb_obj_t object pointer.
 *
 * This function calls nothing
 *
 * Returns 0
 */
static lb_int32 bsd_reg_exit(void *param)
{
	lb_int32 ret = 0;

	return ret;
}

lb_int32 bsd_lt_reg_init(void *param)
{
	lb_int32 ret = 0;
	lb_line_t *pproperty;
	lb_obj_t *left_line;
	int i = 0;

	left_line = (lb_obj_t *)param;
	if (NULL == left_line) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	pproperty = (lb_line_t *)(left_line->property);
	if (NULL == pproperty) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	RT_ASSERT(pproperty->point_num == 5);

	if ((bsd_get_area_lt_up_x(1) != 0 || bsd_get_area_lt_up_y(1) != 0) &&
		(bsd_get_area_rt_up_x(1) != 0 || bsd_get_area_rt_up_y(1) != 0) &&
		(bsd_get_area_rt_dn_x(1) != 0 || bsd_get_area_rt_dn_y(1) != 0) &&
		(bsd_get_area_lt_dn_x(1) != 0 || bsd_get_area_lt_dn_y(1) != 0)
		) {

		pproperty->points[0].x = pproperty->points[4].x =
			bsd_get_area_lt_up_x(1);
		pproperty->points[0].y = pproperty->points[4].y =
			bsd_get_area_lt_up_y(1);

		pproperty->points[1].x =
			bsd_get_area_rt_up_x(1);
		pproperty->points[1].y =
			bsd_get_area_rt_up_y(1);

		pproperty->points[2].x =
			bsd_get_area_rt_dn_x(1);
		pproperty->points[2].y =
			bsd_get_area_rt_dn_y(1);

		pproperty->points[3].x =
			bsd_get_area_lt_dn_x(1);
		pproperty->points[3].y =
			bsd_get_area_lt_dn_y(1);
	}

	for (i = 0; i < 5; i++) {
		if (pproperty->points) {
			APP_LOG_W("point[%d].x:%d, point[%d].y:%d\n",
				i, pproperty->points[i].x,
				i, pproperty->points[i].y);

			line_points[0][i].x = pproperty->points[i].x;
			line_points[0][i].y = pproperty->points[i].y;
		}
	}

	return ret;
}

lb_int32 bsd_rt_reg_init(void *param)
{
	lb_int32 ret = 0;
	lb_line_t *pproperty;
	lb_obj_t *right_line;
	int i = 0;

	right_line = (lb_obj_t *)param;
	if (NULL == right_line) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	pproperty = (lb_line_t *)(right_line->property);
	if (NULL == pproperty) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	RT_ASSERT(pproperty->point_num == 5);

	if ((bsd_get_area_lt_up_x(0) != 0 || bsd_get_area_lt_up_y(0) != 0) &&
		(bsd_get_area_rt_up_x(0) != 0 || bsd_get_area_rt_up_y(0) != 0) &&
		(bsd_get_area_rt_dn_x(0) != 0 || bsd_get_area_rt_dn_y(0) != 0) &&
		(bsd_get_area_lt_dn_x(0) != 0 || bsd_get_area_lt_dn_y(0) != 0)
		) {

		pproperty->points[0].x = pproperty->points[4].x =
			bsd_get_area_lt_up_x(0);
		pproperty->points[0].y = pproperty->points[4].y =
			bsd_get_area_lt_up_y(0);

		pproperty->points[1].x =
			bsd_get_area_rt_up_x(0);
		pproperty->points[1].y =
			bsd_get_area_rt_up_y(0);

		pproperty->points[2].x =
			bsd_get_area_rt_dn_x(0);
		pproperty->points[2].y =
			bsd_get_area_rt_dn_y(0);

		pproperty->points[3].x =
			bsd_get_area_lt_dn_x(0);
		pproperty->points[3].y =
			bsd_get_area_lt_dn_y(0);
	}

	for (i = 0; i < 5; i++) {
		if (pproperty->points) {
			APP_LOG_W("point[%d].x:%d, point[%d].y:%d\n",
				i, pproperty->points[i].x,
				i, pproperty->points[i].y);

			line_points[1][i].x = pproperty->points[i].x;
			line_points[1][i].y = pproperty->points[i].y;
		}
	}

	return ret;
}

lb_int32 bsd_reg_btn_init(void *param)
{
	lb_int32 ret = 0;
	lb_btn_t *pproperty;
	lb_obj_t *btn;

	btn = (lb_obj_t *)param;
	if (NULL == btn) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	pproperty = (lb_btn_t *)(btn->property);
	if (NULL == pproperty) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	pproperty->comms.x = line_points[focus/4][focus%4].x;
	pproperty->comms.y = line_points[focus/4][focus%4].y;

	pproperty->comms.x -= 5;
	pproperty->comms.y -= 5;

	return ret;
}

lb_int32 bsd_top_line_init(void *param)
{
	lb_int32 ret = 0;
	lb_line_t *pproperty;
	lb_obj_t *left_top_line;

	left_top_line = (lb_obj_t *)param;
	if (NULL == left_top_line) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	pproperty = (lb_line_t *)(left_top_line->property);
	if (NULL == pproperty) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}
	cali_line_up = pproperty;

	pproperty->comms.y = bsd_get_roi_para_uprow();

	return 0;
}

lb_int32 bsd_bot_line_init(void *param)
{
	lb_int32 ret = 0;
	lb_line_t *pproperty;
	lb_obj_t *left_bottom_line;

	left_bottom_line = (lb_obj_t *)param;
	if (NULL == left_bottom_line) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	pproperty = (lb_line_t *)(left_bottom_line->property);
	if (NULL == pproperty) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}
	cali_line_dn = pproperty;

	pproperty->comms.y = bsd_get_roi_para_dnrow();

	return 0;
}

lb_int32 bsd_mid_line_init(void *param)
{
	lb_int32 ret = 0;
	lb_obj_t *middle_obj;
	lb_line_t *top_line;
	lb_line_t *bottom_line;
	lb_line_t *middle_line;

	middle_obj = (lb_obj_t *)param;
	if (NULL == middle_obj) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	middle_line = (lb_line_t *)(middle_obj->property);
	if (NULL == middle_line) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}
	cali_line_me = middle_line;

	middle_line->comms.x = bsd_get_roi_para_midcolumn();
	middle_line->comms.y = bsd_get_roi_para_uprow() + 2;

	ret = lb_view_get_obj_property_by_id(BSD_UP_LINE_ID, (void **)&top_line);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	ret = lb_view_get_obj_property_by_id(BSD_DN_LINE_ID, (void **)&bottom_line);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	middle_line->point_num = 2;
	middle_line->points[0].x = 0;
	middle_line->points[0].y = 0;
	middle_line->points[1].x = 2;
	middle_line->points[1].y =
		bsd_get_roi_para_dnrow() - bsd_get_roi_para_uprow() - 4;

	return 0;
}

static lb_int32 bsd_move_top_line(lb_uint32 line_id, lb_uint8 mv_dir)
{
	lb_int32 ret = 0;
	void *obj;
	lb_line_t *line;
	lb_line_t *bottom_line;
	static lb_point_t point;

	ret = lb_view_get_obj_ext_by_id(line_id, &obj);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}
	ret = lb_view_get_obj_property_by_id(line_id, (void **)&line);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	ret = lb_view_get_obj_property_by_id(BSD_DN_LINE_ID, (void **)&bottom_line);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	if (mv_dir == BSD_DIRECT_UP) {
		line->comms.y -= BSD_MOVE_LINE_STEP;
		if (line->comms.y <= BSD_TOP_LINE_UP_LIMIT) {
			line->comms.y += BSD_MOVE_LINE_STEP;
			ret = -1;
			return ret;
		}
	} else if (mv_dir == BSD_DIRECT_DN) {
		line->comms.y += BSD_MOVE_LINE_STEP;
		if (line->comms.y >= bottom_line->comms.y) {
			line->comms.y -= BSD_MOVE_LINE_STEP;
			ret = -1;
			return ret;
		}

		if (line->comms.y >= BSD_TOP_LINE_DN_LIMIT) {
			line->comms.y -= BSD_MOVE_LINE_STEP;
			ret = -1;
			return ret;
		}
	}

	point.x = line->comms.x;
	point.y = line->comms.y;
	lb_gal_update_line(obj, LB_LINE_UPD_POS, &point);

	return ret;
}

static lb_int32 bsd_move_bot_line(lb_uint32 line_id, lb_uint8 mv_dir)
{
	lb_int32 ret = 0;
	void *obj;
	lb_line_t *line;
	lb_line_t *top_line;
	static lb_point_t point;

	ret = lb_view_get_obj_ext_by_id(line_id, &obj);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}
	ret = lb_view_get_obj_property_by_id(line_id, (void **)&line);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	ret = lb_view_get_obj_property_by_id(BSD_UP_LINE_ID, (void **)&top_line);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	if (mv_dir == BSD_DIRECT_UP) {
		line->comms.y -= BSD_MOVE_LINE_STEP;
		if (line->comms.y <= top_line->comms.y) {
			line->comms.y += BSD_MOVE_LINE_STEP;
			ret = -1;
			return ret;
		}

		if (line->comms.y <= BSD_BOT_UP_LIMIT) {
			line->comms.y -= BSD_MOVE_LINE_STEP;
			ret = -1;
			return ret;
		}
	} else if (mv_dir == BSD_DIRECT_DN) {
		line->comms.y += BSD_MOVE_LINE_STEP;
		if (line->comms.y >= BSD_BOT_DN_LIMIT) {
			line->comms.y -= BSD_MOVE_LINE_STEP;
			ret = -1;
			return ret;
		}
	}

	point.x = line->comms.x;
	point.y = line->comms.y;
	lb_gal_update_line(obj, LB_LINE_UPD_POS, &point);

	return ret;
}

static lb_int32 bsd_move_mid_line(lb_uint32 line_id, lb_uint8 mv_dir, lb_uint8 base)
{
	void *obj;
	lb_int32 ret = 0;
	lb_line_t *line;
	static lb_point_t point;
	static lb_size_t size;

	ret = lb_view_get_obj_ext_by_id(line_id, &obj);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}
	ret = lb_view_get_obj_property_by_id(line_id, (void **)&line);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	if (base == 0x70) {
		if (mv_dir == BSD_DIRECT_LT) {
			line->comms.x -= BSD_MOVE_LINE_STEP;
			if (line->comms.x <= BSD_MID_LINE_LT_LIMIT) {
				line->comms.x += BSD_MOVE_LINE_STEP;
				ret = -1;
				return ret;
			}
		} else if (mv_dir == BSD_DIRECT_RT) {
			line->comms.x += BSD_MOVE_LINE_STEP;
			if (line->comms.x >= BSD_MID_LINE_RT_LIMIT) {
				line->comms.x -= BSD_MOVE_LINE_STEP;
				ret = -1;
				return ret;
			}
		}
	} else if (base == 0x00) {
		if (mv_dir == BSD_DIRECT_UP) {
			line->comms.y -= BSD_MOVE_LINE_STEP;
			line->points[1].y += BSD_MOVE_LINE_STEP;
		} else if (mv_dir == BSD_DIRECT_DN) {
			line->comms.y += BSD_MOVE_LINE_STEP;
			line->points[1].y -= BSD_MOVE_LINE_STEP;
		}
	} else if (base == 0xff) {
		if (mv_dir == BSD_DIRECT_UP)
			line->points[1].y -= BSD_MOVE_LINE_STEP;
		else if (mv_dir == BSD_DIRECT_DN)
			line->points[1].y += BSD_MOVE_LINE_STEP;
	}

	point.x = line->comms.x;
	point.y = line->comms.y;
	size.w = line->points[1].x - line->points[0].x;
	size.h = line->points[1].y - line->points[0].y;

	lb_gal_update_line(obj, LB_LINE_UPD_SIZE, &size);
	lb_gal_update_line(obj, LB_LINE_UPD_POS, &point);

	return ret;
}

static lb_int32 bsd_reg_up_limit(void)
{
	lb_int32 ret = 0;
	lb_int32 i = 0;
	lb_int32 j = 0;
	lb_point_t lp[2][5];
	float slope_03;
	float slope_12;
	float slope_47;
	float slope_56;
	float x1;
	float y1;
	float x2;
	float y2;

	RT_ASSERT(focus >= 0 || focus <= 8);
	RT_ASSERT(line_points != NULL);

	for (i = 0; i < 2; i++) {
		for (j = 0; j < 5; j++)
			lp[i][j] = line_points[i][j];
	}

	if ((lp[focus/4][focus%4].y - BSD_MOVE_POINT_STEP) >= 0)
		lp[focus/4][focus%4].y -= BSD_MOVE_POINT_STEP;

	x1 = (float)lp[0][0].x;
	y1 = (float)lp[0][0].y;
	x2 = (float)lp[0][3].x;
	y2 = (float)lp[0][3].y;
	slope_03 = (y2 - y1) / (x2 - x1);

	x1 = (float)lp[0][1].x;
	y1 = (float)lp[0][1].y;
	x2 = (float)lp[0][2].x;
	y2 = (float)lp[0][2].y;
	slope_12 = (y2 - y1) / (x2 - x1);
	if (slope_03 < slope_12) {
		printf("slope_03:%f, slope_12:%f\n", slope_03, slope_12);
		ret = -1;
		return ret;
	}

	x1 = (float)lp[1][0].x;
	y1 = (float)lp[1][0].y;
	x2 = (float)lp[1][3].x;
	y2 = (float)lp[1][3].y;
	slope_47 = (y2 - y1) / (x2 - x1);

	x1 = (float)lp[1][1].x;
	y1 = (float)lp[1][1].y;
	x2 = (float)lp[1][2].x;
	y2 = (float)lp[1][2].y;
	slope_56 = (y2 - y1) / (x2 - x1);
	if (slope_47 < slope_56) {
		printf("slope_47:%f, slope_56:%f\n", slope_47, slope_56);
		ret = -1;
		return ret;
	}

	if (lp[focus/4][0].y < cali_line_up->comms.y) {
		printf("out of limit point 0\n");
		ret = -1;
		return ret;
	}

	if (lp[focus/4][1].y < cali_line_up->comms.y) {
		printf("out of limit point 1\n");
		ret = -1;
		return ret;
	}

	return ret;
}

static lb_int32 bsd_reg_point_up(void *param)
{
	lb_int32 ret = 0;
	lb_int32 i = 0;
	void *pobj_line[2];
	void *pobj_btn = NULL;
	static lb_point_t point;
	static lb_line_points_t l_point[2];

	if (bsd_reg_up_limit() != 0) {
		APP_LOG_W("out of limit\n");
		ret = -1;
		return ret;
	}

	ret = lb_view_get_obj_ext_by_id(200, &pobj_line[0]);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	ret = lb_view_get_obj_ext_by_id(201, &pobj_line[1]);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	ret = lb_view_get_obj_ext_by_id(202, &pobj_btn);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	RT_ASSERT(focus >= 0 || focus <= 8);
	RT_ASSERT(line_points != NULL);

	switch (focus) {
	case 0:
	case 4:
		if ((line_points[focus/4][4].y - BSD_MOVE_POINT_STEP)
			>= 0) {
			line_points[focus/4][4].y -= BSD_MOVE_POINT_STEP;
			line_points[focus/4][0].y -= BSD_MOVE_POINT_STEP;

			line_points[focus/4][1].y -= BSD_MOVE_POINT_STEP;
		}

		break;
	case 1:
	case 5:
		if ((line_points[focus/4][focus%4].y - BSD_MOVE_POINT_STEP)
			>= 0) {
			line_points[focus/4][focus%4].y -=
				BSD_MOVE_POINT_STEP;

			line_points[focus/4][4].y -=
				BSD_MOVE_POINT_STEP;
			line_points[focus/4][0].y -=
				BSD_MOVE_POINT_STEP;
		}

		break;
	case 2:
	case 3:
	case 6:
	case 7:
		if ((line_points[focus/4][focus%4].y - BSD_MOVE_POINT_STEP)
			>= 0)
			line_points[focus/4][focus%4].y -=
			BSD_MOVE_POINT_STEP;

		break;

	default:
		break;
	}

	point.x = line_points[focus/4][focus%4].x;
	point.y = line_points[focus/4][focus%4].y;
	point.x -= 5;
	point.y -= 5;

	l_point[0].num = 5;
	l_point[1].num = 5;
	for (i = 0; i < 5; i++) {
		l_point[0].points[i].x = line_points[0][i].x;
		l_point[0].points[i].y = line_points[0][i].y;
		l_point[1].points[i].x = line_points[1][i].x;
		l_point[1].points[i].y = line_points[1][i].y;
	}

	RT_ASSERT(pobj_btn != NULL);
	RT_ASSERT(pobj_line[focus / 4] != NULL);

	lb_gal_update_btn(pobj_btn,
		LB_BTN_UPD_POS,
		&point);

	lb_gal_update_line(pobj_line[focus / 4],
		LB_LINE_UPD_POINTS,
		&l_point[focus / 4]);

	return ret;
}

static lb_int32 bsd_reg_dn_limit(void)
{
	lb_int32 ret = 0;
	lb_int32 i = 0;
	lb_int32 j = 0;
	lb_point_t lp[2][5];
	float slope_03;
	float slope_12;
	float slope_47;
	float slope_56;
	float x1;
	float y1;
	float x2;
	float y2;

	RT_ASSERT(focus >= 0 || focus <= 8);
	RT_ASSERT(line_points != NULL);

	for (i = 0; i < 2; i++) {
		for (j = 0; j < 5; j++)
			lp[i][j] = line_points[i][j];
	}

	if ((lp[focus/4][focus%4].y + BSD_MOVE_POINT_STEP) <= BSD_CALIB_PREW)
		lp[focus/4][focus%4].y += BSD_MOVE_POINT_STEP;

	x1 = (float)lp[0][0].x;
	y1 = (float)lp[0][0].y;
	x2 = (float)lp[0][3].x;
	y2 = (float)lp[0][3].y;
	slope_03 = (y2 - y1) / (x2 - x1);

	x1 = (float)lp[0][1].x;
	y1 = (float)lp[0][1].y;
	x2 = (float)lp[0][2].x;
	y2 = (float)lp[0][2].y;
	slope_12 = (y2 - y1) / (x2 - x1);
	if (slope_03 < slope_12) {
		printf("slope_03:%f, slope_12:%f\n", slope_03, slope_12);
		ret = -1;
		return ret;
	}

	x1 = (float)lp[1][0].x;
	y1 = (float)lp[1][0].y;
	x2 = (float)lp[1][3].x;
	y2 = (float)lp[1][3].y;
	slope_47 = (y2 - y1) / (x2 - x1);

	x1 = (float)lp[1][1].x;
	y1 = (float)lp[1][1].y;
	x2 = (float)lp[1][2].x;
	y2 = (float)lp[1][2].y;
	slope_56 = (y2 - y1) / (x2 - x1);
	if (slope_47 < slope_56) {
		printf("slope_47:%f, slope_56:%f\n", slope_47, slope_56);
		ret = -1;
		return ret;
	}

	return ret;
}



static lb_int32 bsd_reg_point_dn(void *param)
{
	lb_int32 ret = 0;
	lb_int32 i = 0;
	void *pobj_line[2];
	void *pobj_btn = NULL;
	static lb_point_t point;
	static lb_line_points_t l_point[2];

	if (bsd_reg_dn_limit() != 0) {
		APP_LOG_W("out of limit\n");
		ret = -1;
		return ret;
	}

	ret = lb_view_get_obj_ext_by_id(200, &pobj_line[0]);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	ret = lb_view_get_obj_ext_by_id(201, &pobj_line[1]);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	ret = lb_view_get_obj_ext_by_id(202, &pobj_btn);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	RT_ASSERT(focus >= 0 || focus <= 8);
	RT_ASSERT(line_points != NULL);

	switch (focus) {
	case 0:
	case 4:
		if ((line_points[focus/4][4].y + BSD_MOVE_POINT_STEP)
			<= BSD_CALIB_PREW) {
			line_points[focus/4][4].y += BSD_MOVE_POINT_STEP;
			line_points[focus/4][0].y += BSD_MOVE_POINT_STEP;

			line_points[focus/4][1].y += BSD_MOVE_POINT_STEP;
		}


		break;
	case 1:
	case 5:
		if ((line_points[focus/4][focus%4].y + BSD_MOVE_POINT_STEP)
			<= BSD_CALIB_PREW) {
			line_points[focus/4][focus%4].y +=
				BSD_MOVE_POINT_STEP;
			line_points[focus/4][4].y +=
				BSD_MOVE_POINT_STEP;
			line_points[focus/4][0].y +=
				BSD_MOVE_POINT_STEP;
		}
		break;
	case 2:
	case 3:
	case 6:
	case 7:
		if ((line_points[focus/4][focus%4].y + BSD_MOVE_POINT_STEP)
			<= BSD_CALIB_PREW)
			line_points[focus/4][focus%4].y +=
				BSD_MOVE_POINT_STEP;

		break;

	default:
		break;
	}

	point.x = line_points[focus/4][focus%4].x;
	point.y = line_points[focus/4][focus%4].y;
	point.x -= 5;
	point.y -= 5;

	l_point[0].num = 5;
	l_point[1].num = 5;
	for (i = 0; i < 5; i++) {
		l_point[0].points[i].x = line_points[0][i].x;
		l_point[0].points[i].y = line_points[0][i].y;
		l_point[1].points[i].x = line_points[1][i].x;
		l_point[1].points[i].y = line_points[1][i].y;
	}

	RT_ASSERT(pobj_btn != NULL);
	RT_ASSERT(pobj_line[focus / 4] != NULL);

	lb_gal_update_btn(pobj_btn,
		LB_BTN_UPD_POS,
		&point);

	lb_gal_update_line(pobj_line[focus / 4],
		LB_LINE_UPD_POINTS,
		&l_point[focus / 4]);

	return ret;
}

static lb_int32 bsd_reg_lt_limit(void)
{
	lb_int32 ret = 0;
	lb_int32 i = 0;
	lb_int32 j = 0;
	lb_point_t lp[2][5];
	float slope_03;
	float slope_12;
	float slope_47;
	float slope_56;
	float x1;
	float y1;
	float x2;
	float y2;

	RT_ASSERT(focus >= 0 || focus <= 8);
	RT_ASSERT(line_points != NULL);

	for (i = 0; i < 2; i++) {
		for (j = 0; j < 5; j++)
			lp[i][j] = line_points[i][j];
	}

	if ((lp[focus/4][focus%4].x - BSD_MOVE_POINT_STEP) >= 0)
		lp[focus/4][focus%4].x -= BSD_MOVE_POINT_STEP;

	x1 = (float)lp[0][0].x;
	y1 = (float)lp[0][0].y;
	x2 = (float)lp[0][3].x;
	y2 = (float)lp[0][3].y;
	slope_03 = (y2 - y1) / (x2 - x1);

	x1 = (float)lp[0][1].x;
	y1 = (float)lp[0][1].y;
	x2 = (float)lp[0][2].x;
	y2 = (float)lp[0][2].y;
	slope_12 = (y2 - y1) / (x2 - x1);
	if (slope_03 < slope_12) {
		printf("slope_03:%f, slope_12:%f\n", slope_03, slope_12);
		ret = -1;
		return ret;
	}

	x1 = (float)lp[1][0].x;
	y1 = (float)lp[1][0].y;
	x2 = (float)lp[1][3].x;
	y2 = (float)lp[1][3].y;
	slope_47 = (y2 - y1) / (x2 - x1);

	x1 = (float)lp[1][1].x;
	y1 = (float)lp[1][1].y;
	x2 = (float)lp[1][2].x;
	y2 = (float)lp[1][2].y;
	slope_56 = (y2 - y1) / (x2 - x1);
	if (slope_47 < slope_56) {
		printf("slope_47:%f, slope_56:%f\n", slope_47, slope_56);
		ret = -1;
		return ret;
	}

	return ret;
}

static lb_int32 bsd_reg_point_lt(void *param)
{
	lb_int32 ret = 0;
	lb_int32 i = 0;
	void *pobj_line[2];
	void *pobj_btn = NULL;
	static lb_point_t point;
	static lb_line_points_t l_point[2];

	if (bsd_reg_lt_limit() != 0) {
		APP_LOG_W("out of limit\n");
		ret = -1;
		return ret;
	}

	ret = lb_view_get_obj_ext_by_id(200, &pobj_line[0]);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	ret = lb_view_get_obj_ext_by_id(201, &pobj_line[1]);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	ret = lb_view_get_obj_ext_by_id(202, &pobj_btn);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	RT_ASSERT(focus >= 0 || focus <= 8);
	RT_ASSERT(line_points != NULL);

	switch (focus) {
	case 0:
		if ((line_points[focus/4][4].x - BSD_MOVE_POINT_STEP)
				>= 0) {
			line_points[focus/4][4].x -= BSD_MOVE_POINT_STEP;
			line_points[focus/4][0].x -= BSD_MOVE_POINT_STEP;
		}

		break;
	case 4:
		if ((line_points[focus/4][4].x - BSD_MOVE_POINT_STEP)
				>= BSD_CALIB_PREH / 2) {
			line_points[focus/4][4].x -= BSD_MOVE_POINT_STEP;
			line_points[focus/4][0].x -= BSD_MOVE_POINT_STEP;
		}

		break;
	case 1:
	case 2:
	case 3:
		if ((line_points[focus/4][focus%4].x - BSD_MOVE_POINT_STEP)
				>= 0)
			line_points[focus/4][focus%4].x -=
				BSD_MOVE_POINT_STEP;

		break;

	case 5:
	case 6:
	case 7:
		if ((line_points[focus/4][focus%4].x - BSD_MOVE_POINT_STEP)
				>= BSD_CALIB_PREH / 2)
			line_points[focus/4][focus%4].x -=
				BSD_MOVE_POINT_STEP;

		break;

	default:
		break;
	}

	point.x = line_points[focus/4][focus%4].x;
	point.y = line_points[focus/4][focus%4].y;
	point.x -= 5;
	point.y -= 5;

	l_point[0].num = 5;
	l_point[1].num = 5;
	for (i = 0; i < 5; i++) {
		l_point[0].points[i].x = line_points[0][i].x;
		l_point[0].points[i].y = line_points[0][i].y;
		l_point[1].points[i].x = line_points[1][i].x;
		l_point[1].points[i].y = line_points[1][i].y;
	}

	RT_ASSERT(pobj_btn != NULL);
	RT_ASSERT(pobj_line[focus / 4] != NULL);

	lb_gal_update_btn(pobj_btn,
		LB_BTN_UPD_POS,
		&point);

	lb_gal_update_line(pobj_line[focus / 4],
		LB_LINE_UPD_POINTS,
		&l_point[focus / 4]);

	return ret;
}

static lb_int32 bsd_reg_rt_limit(void)
{
	lb_int32 ret = 0;
	lb_int32 i = 0;
	lb_int32 j = 0;
	lb_point_t lp[2][5];
	float slope_03;
	float slope_12;
	float slope_47;
	float slope_56;
	float x1;
	float y1;
	float x2;
	float y2;

	RT_ASSERT(focus >= 0 || focus <= 8);
	RT_ASSERT(line_points != NULL);

	for (i = 0; i < 2; i++) {
		for (j = 0; j < 5; j++)
			lp[i][j] = line_points[i][j];
	}

	if ((lp[focus/4][focus%4].x + BSD_MOVE_POINT_STEP) <= BSD_CALIB_PREH)
		lp[focus/4][focus%4].x += BSD_MOVE_POINT_STEP;

	x1 = (float)lp[0][0].x;
	y1 = (float)lp[0][0].y;
	x2 = (float)lp[0][3].x;
	y2 = (float)lp[0][3].y;
	slope_03 = (y2 - y1) / (x2 - x1);

	x1 = (float)lp[0][1].x;
	y1 = (float)lp[0][1].y;
	x2 = (float)lp[0][2].x;
	y2 = (float)lp[0][2].y;
	slope_12 = (y2 - y1) / (x2 - x1);
	if (slope_03 < slope_12) {
		printf("slope_03:%f, slope_12:%f\n", slope_03, slope_12);
		ret = -1;
		return ret;
	}

	x1 = (float)lp[1][0].x;
	y1 = (float)lp[1][0].y;
	x2 = (float)lp[1][3].x;
	y2 = (float)lp[1][3].y;
	slope_47 = (y2 - y1) / (x2 - x1);

	x1 = (float)lp[1][1].x;
	y1 = (float)lp[1][1].y;
	x2 = (float)lp[1][2].x;
	y2 = (float)lp[1][2].y;
	slope_56 = (y2 - y1) / (x2 - x1);
	if (slope_47 < slope_56) {
		printf("slope_47:%f, slope_56:%f\n", slope_47, slope_56);
		ret = -1;
		return ret;
	}

	return ret;
}

static lb_int32 bsd_reg_point_rt(void *param)
{
	lb_int32 ret = 0;
	lb_int32 i = 0;
	void *pobj_line[2];
	void *pobj_btn = NULL;
	static lb_point_t point;
	static lb_line_points_t l_point[2];

	if (bsd_reg_rt_limit() != 0) {
		APP_LOG_W("out of limit\n");
		ret = -1;
		return ret;
	}

	ret = lb_view_get_obj_ext_by_id(200, &pobj_line[0]);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	ret = lb_view_get_obj_ext_by_id(201, &pobj_line[1]);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	ret = lb_view_get_obj_ext_by_id(202, &pobj_btn);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	RT_ASSERT(focus >= 0 || focus <= 8);
	RT_ASSERT(line_points != NULL);

	switch (focus) {
	case 0:
		if ((line_points[focus/4][4].x + BSD_MOVE_POINT_STEP) <=
			BSD_CALIB_PREH / 2) {
			line_points[focus/4][4].x += BSD_MOVE_POINT_STEP;
			line_points[focus/4][0].x += BSD_MOVE_POINT_STEP;
		}

		break;
	case 4:
		if ((line_points[focus/4][4].x + BSD_MOVE_POINT_STEP) <=
			BSD_CALIB_PREH) {
			line_points[focus/4][4].x += BSD_MOVE_POINT_STEP;
			line_points[focus/4][0].x += BSD_MOVE_POINT_STEP;
		}

		break;
	case 1:
	case 2:
	case 3:
		if ((line_points[focus/4][focus%4].x + BSD_MOVE_POINT_STEP) <=
			BSD_CALIB_PREH / 2)
			line_points[focus/4][focus%4].x +=
				BSD_MOVE_POINT_STEP;

		break;

	case 5:
	case 6:
	case 7:
		if ((line_points[focus/4][focus%4].x + BSD_MOVE_POINT_STEP) <=
			BSD_CALIB_PREH)
			line_points[focus/4][focus%4].x +=
				BSD_MOVE_POINT_STEP;

		break;

	default:
		break;
	}

	point.x = line_points[focus/4][focus%4].x;
	point.y = line_points[focus/4][focus%4].y;
	point.x -= 5;
	point.y -= 5;

	l_point[0].num = 5;
	l_point[1].num = 5;
	for (i = 0; i < 5; i++) {
		l_point[0].points[i].x = line_points[0][i].x;
		l_point[0].points[i].y = line_points[0][i].y;
		l_point[1].points[i].x = line_points[1][i].x;
		l_point[1].points[i].y = line_points[1][i].y;
	}

	RT_ASSERT(pobj_btn != NULL);
	RT_ASSERT(pobj_line[focus / 4] != NULL);

	lb_gal_update_btn(pobj_btn,
		LB_BTN_UPD_POS,
		&point);

	lb_gal_update_line(pobj_line[focus / 4],
		LB_LINE_UPD_POINTS,
		&l_point[focus / 4]);

	return ret;
}

static lb_int32 bsd_reg_point_ee(void *param)
{
	lb_int32 ret = 0;
	void *pobj = NULL;
	static lb_point_t point;

	ret = lb_view_get_obj_ext_by_id(202, &pobj);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	focus = (focus + 1) % 8;

	point.x = line_points[focus/4][focus%4].x;
	point.y = line_points[focus/4][focus%4].y;

	point.x -= 5;
	point.y -= 5;

	lb_gal_update_btn(pobj, LB_BTN_UPD_POS, &point);

	return ret;
}

static lb_int32 bsd_top_line_up(void *param)
{
	lb_int32 ret = 0;

	ret = bsd_move_top_line(BSD_UP_LINE_ID, BSD_DIRECT_UP);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	ret = bsd_move_mid_line(BSD_ME_LINE_ID, BSD_DIRECT_UP, 0x00);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	return ret;
}

static lb_int32 bsd_top_line_dn(void *param)
{
	lb_int32 ret = 0;

	ret = bsd_move_top_line(BSD_UP_LINE_ID, BSD_DIRECT_DN);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	ret = bsd_move_mid_line(BSD_ME_LINE_ID, BSD_DIRECT_DN, 0x00);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	return ret;
}

static lb_int32 bsd_bot_line_up(void *param)
{
	lb_int32 ret = 0;

	ret = bsd_move_bot_line(BSD_DN_LINE_ID, BSD_DIRECT_UP);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	ret = bsd_move_mid_line(BSD_ME_LINE_ID, BSD_DIRECT_UP, 0xff);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	return ret;
}

static lb_int32 bsd_bot_line_dn(void *param)
{
	lb_int32 ret = 0;

	ret = bsd_move_bot_line(BSD_DN_LINE_ID, BSD_DIRECT_DN);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	ret = bsd_move_mid_line(BSD_ME_LINE_ID, BSD_DIRECT_DN, 0xff);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	return ret;
}

static lb_int32 bsd_mid_line_lt(void *param)
{
	lb_int32 ret = 0;

	bsd_move_mid_line(BSD_ME_LINE_ID, BSD_DIRECT_LT, 0x70);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	return ret;
}

static lb_int32 bsd_mid_line_rt(void *param)
{
	lb_int32 ret = 0;

	bsd_move_mid_line(BSD_ME_LINE_ID, BSD_DIRECT_RT, 0x70);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	return ret;
}

static lb_int32 bsd_prev_btn(void *param)
{
	lb_int32 ret = 0;
	char json_str[64];
	const char *result_str;
	void *obj = NULL;

	lb_view_get_obj_ext_by_id(220, &obj);
	if (NULL == obj) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	if (!prev_flag) {
		ret = lb_recorder_ctrl(eplayer, LB_REC_PAUSE_PREVIEW, 0);
		if (ret < 0) {
			APP_LOG_W("failed!");
			return -1;
		}
		strcpy(json_str, "STR_SMART_DRIVE_BSD_CALIBRATE_CONTINUE");
		result_str = elang_get_utf8_string_josn(json_str);
		lb_gal_update_label(obj, LB_LABEL_UPD_TXT, (void *)result_str);
		prev_flag = 1;
	} else if (prev_flag) {
		ret = lb_recorder_ctrl(eplayer, LB_REC_CONTINUE_PREVIEW, 0);
		if (ret < 0) {
			APP_LOG_W("failed!");
			return -1;
		}
		strcpy(json_str, "STR_SMART_DRIVE_BSD_CALIBRATE_PAUSE");
		result_str = elang_get_utf8_string_josn(json_str);
		lb_gal_update_label(obj, LB_LABEL_UPD_TXT, (void *)result_str);
		prev_flag = 0;
	} else {
		APP_LOG_W("failed!");
		return -1;
	}

	return ret;
}

/**
 * save_succeed - show the result(label) means succeed
 *
 * This function show the result(label) means succeed
 *
 * Returns 0 if called when get success ; otherwise, return -1
 */
static lb_int32 bsd_save_succeed(void)
{
	lb_int32 ret = 0;
	void *obj = NULL;
	char json_str[64];
	const char *result_str;

	lb_view_get_obj_ext_by_id(216, &obj);
	if (NULL == obj) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	strcpy(json_str, "STR_SMART_DRIVE_SAVE_SUCCEED");
	result_str = elang_get_utf8_string_josn(json_str);

	lb_gal_update_label(obj, LB_LABEL_UPD_TXT, (void *)result_str);

	return ret;
}

/**
 * save_failed - show the result(label) means failed
 *
 * This function show the result(label) means failed
 *
 * Returns 0 if called when get success ; otherwise, return -1
 */
static lb_int32 bsd_save_failed(void)
{
	lb_int32 ret = 0;
	void *obj = NULL;
	char json_str[64];
	const char *result_str;

	lb_view_get_obj_ext_by_id(216, &obj);
	if (NULL == obj) {
		APP_LOG_W("failed\n");
		ret = -1;
		return ret;
	}

	strcpy(json_str, "STR_SMART_DRIVE_SAVE_FAILED");
	result_str = elang_get_utf8_string_josn(json_str);

	lb_gal_update_label(obj, LB_LABEL_UPD_TXT, (void *)result_str);

	return ret;
}

static lb_int32 bsd_save_result(void *param)
{
	lb_int32 ret = 0;

	RT_ASSERT(cali_line_up);
	RT_ASSERT(cali_line_dn);
	RT_ASSERT(cali_line_me);

	ret |= bsd_save_roi_para(cali_line_up->comms.y,
		cali_line_dn->comms.y, cali_line_me->comms.x);

	ret |= bsd_save_area_lt_up(line_points[0][0].x,
		line_points[0][0].y, 1);
	ret |= bsd_save_area_rt_up(line_points[0][1].x,
		line_points[0][1].y, 1);
	ret |= bsd_save_area_rt_dn(line_points[0][2].x,
		line_points[0][2].y, 1);
	ret |= bsd_save_area_lt_dn(line_points[0][3].x,
		line_points[0][3].y, 1);

	ret |= bsd_save_area_lt_up(line_points[1][0].x,
		line_points[1][0].y, 0);
	ret |= bsd_save_area_rt_up(line_points[1][1].x,
		line_points[1][1].y, 0);
	ret |= bsd_save_area_rt_dn(line_points[1][2].x,
		line_points[1][2].y, 0);
	ret |= bsd_save_area_lt_dn(line_points[1][3].x,
		line_points[1][3].y, 0);

	if (ret == 0)
		bsd_save_succeed();
	else
		bsd_save_failed();

	return ret;
}

lb_int32 bsd_calib_init_funcs(void)
{
	lb_int32 err = 0;

	err |= lb_fmngr_reg_init_func("bsd_lt_reg_init",
			bsd_lt_reg_init);
	err |= lb_fmngr_reg_init_func("bsd_rt_reg_init",
			bsd_rt_reg_init);
	err |= lb_fmngr_reg_init_func("bsd_reg_btn_init",
			bsd_reg_btn_init);

	err |= lb_fmngr_reg_init_func("bsd_load_method",
			bsd_load_method);
	err |= lb_fmngr_reg_init_func("bsd_load_lines",
			bsd_load_lines);

	err |= lb_fmngr_reg_init_func("view0_init",
			view0_init);
	err |= lb_fmngr_reg_exit_func("view0_exit",
			view0_exit);

	err |= lb_fmngr_reg_init_func("view1_init",
			view1_init);
	err |= lb_fmngr_reg_exit_func("view1_exit",
			view1_exit);

	err |= lb_fmngr_reg_init_func("bsd_region_init",
			bsd_reg_init);
	err |= lb_fmngr_reg_exit_func("bsd_region_exit",
			bsd_reg_exit);

	err |= lb_fmngr_reg_init_func("bsd_top_line_init",
			bsd_top_line_init);
	err |= lb_fmngr_reg_init_func("bsd_bot_line_init",
			bsd_bot_line_init);
	err |= lb_fmngr_reg_init_func("bsd_mid_line_init",
			bsd_mid_line_init);

	return err;
}

lb_int32 bsd_calib_uninit_funcs(void)
{
	lb_int32 err = 0;

	err |= lb_fmngr_unreg_init_func(bsd_top_line_init);
	err |= lb_fmngr_unreg_init_func(bsd_bot_line_init);
	err |= lb_fmngr_unreg_init_func(bsd_mid_line_init);

	err |= lb_fmngr_unreg_init_func(view0_init);
	err |= lb_fmngr_unreg_exit_func(view0_exit);

	err |= lb_fmngr_unreg_init_func(view1_init);
	err |= lb_fmngr_unreg_exit_func(view1_exit);

	err |= lb_fmngr_unreg_init_func(bsd_reg_init);
	err |= lb_fmngr_unreg_exit_func(bsd_reg_exit);

	err |= lb_fmngr_unreg_init_func(bsd_load_method);
	err |= lb_fmngr_unreg_init_func(bsd_load_lines);

	err |= lb_fmngr_unreg_init_func(bsd_lt_reg_init);
	err |= lb_fmngr_unreg_init_func(bsd_rt_reg_init);
	err |= lb_fmngr_unreg_init_func(bsd_reg_btn_init);

	return err;
}

lb_int32 bsd_calib_resp_funcs(void)
{
	lb_int32 err = 0;

	err |= lb_reg_resp_msg_func(BSD_CALI_TOP_UP,
			bsd_top_line_up);
	err |= lb_reg_resp_msg_func(BSD_CALI_TOP_DN,
			bsd_top_line_dn);
	err |= lb_reg_resp_msg_func(BSD_CALI_BOT_UP,
			bsd_bot_line_up);
	err |= lb_reg_resp_msg_func(BSD_CALI_BOT_DN,
			bsd_bot_line_dn);
	err |= lb_reg_resp_msg_func(BSD_CALI_MID_LT,
			bsd_mid_line_lt);
	err |= lb_reg_resp_msg_func(BSD_CALI_MID_RT,
			bsd_mid_line_rt);
	err |= lb_reg_resp_msg_func(BSD_CALI_PREV_PAUSE,
			bsd_prev_btn);

	err |= lb_reg_resp_msg_func(BSD_REGION_POINT_UP,
			bsd_reg_point_up);
	err |= lb_reg_resp_msg_func(BSD_REGION_POINT_DN,
			bsd_reg_point_dn);
	err |= lb_reg_resp_msg_func(BSD_REGION_POINT_LT,
			bsd_reg_point_lt);
	err |= lb_reg_resp_msg_func(BSD_REGION_POINT_RT,
			bsd_reg_point_rt);
	err |= lb_reg_resp_msg_func(BSD_REGION_POINT_EE,
			bsd_reg_point_ee);

	err |= lb_reg_resp_msg_func(BSD_SAVE_RES,
			bsd_save_result);

	return err;
}

lb_int32 bsd_calib_unresp_funcs(void)
{
	lb_int32 err = 0;

	err |= lb_unreg_resp_msg_func(bsd_top_line_up);
	err |= lb_unreg_resp_msg_func(bsd_top_line_dn);
	err |= lb_unreg_resp_msg_func(bsd_bot_line_up);
	err |= lb_unreg_resp_msg_func(bsd_bot_line_dn);
	err |= lb_unreg_resp_msg_func(bsd_mid_line_lt);
	err |= lb_unreg_resp_msg_func(bsd_mid_line_rt);
	err |= lb_unreg_resp_msg_func(bsd_prev_btn);

	err |= lb_unreg_resp_msg_func(bsd_reg_point_up);
	err |= lb_unreg_resp_msg_func(bsd_reg_point_dn);
	err |= lb_unreg_resp_msg_func(bsd_reg_point_lt);
	err |= lb_unreg_resp_msg_func(bsd_reg_point_rt);
	err |= lb_unreg_resp_msg_func(bsd_reg_point_ee);

	err |= lb_unreg_resp_msg_func(bsd_save_result);

	return err;
}
