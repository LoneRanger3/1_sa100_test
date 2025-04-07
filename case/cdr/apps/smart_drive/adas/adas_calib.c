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
#include "adas_calib.h"
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
#include "case_config.h"

static void *eplayer;
static void *p_obj[2];

/**
 * load_adas_method - save objects from the adas method view
 *
 * This function save objects from the adas method view
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
static lb_int32 adas_load_method(void *para)
{
	lb_int32 ret = 0;
	lb_int32 i = 0;
	lb_uint32 id = 0;

	for (i = 0; i < 2; i++) {
		id = 200 + i;
		ret = lb_view_get_obj_ext_by_id(id, &p_obj[i]);
		if (0 != ret) {
			printf("%s,%d,failed\n", __FILE__, __LINE__);
			return LB_ERROR_NO_MEM;
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
static lb_int32 adas_hide_method(void *para)
{
	lb_int32 ret = 0;
	lb_int32 i = 0;

	for (i = 0; i < 2; i++)
		ret |= lb_gal_set_obj_hidden(p_obj[i], 1);

	return ret;
}

/**
 * show_method - show the method view
 *
 * This function show the method view
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
static lb_int32 adas_show_method(void *para)
{
	lb_int32 ret = 0;
	lb_int32 i = 0;

	for (i = 0; i < 2; i++)
		ret |= lb_gal_set_obj_hidden(p_obj[i], 0);

	return ret;
}

/**
 * prev_init - initial the mod to start preview
 *
 * This function initial the mod to start preview
 *
 * Returns 0 if called when get success ; otherwise, return -1
 */
static lb_int32 adas_prev_init(void *para)
{
	lb_int32 ret = 0;
	win_para_t win_para;
	rec_param_t rec_para;
#ifdef SYS_MEMERY_128M_FOR_DOUBLE_1080P
	user_def_sys_cfg_t userdef_sys_cfg;
#endif
	eplayer = lb_recorder_creat();
	if (eplayer == NULL) {
		printf("creat recorder err!");
		return -1;
	}
#ifdef SYS_MEMERY_128M_FOR_DOUBLE_1080P
	if ((FRONT_RECORDER_SOURCE_HEIGHT == 1080) &&
			(REAR_RECORDER_SOURCE_HEIGHT == 1080)) {
		userdef_sys_cfg.camera_buf_num = FRONT_CAMERA_BUFFER_NUM;
		lb_recorder_ctrl(eplayer, LB_REC_SET_USER_DEF_SYS_PRAR, &userdef_sys_cfg);
	}
#endif
	lb_recorder_ctrl(eplayer, LB_REC_SET_VIDEO_SOURCE, "vic");

	lb_recorder_ctrl(eplayer, LB_REC_GET_PARA, &rec_para);
	rec_para.source_width = /*FRONT_RECORDER_SOURCE_WIDTH*/1920;
	rec_para.source_height = /*FRONT_RECORDER_SOURCE_HEIGHT*/2160;
	rec_para.width = FRONT_RECORDER_SOURCE_WIDTH;
	rec_para.height = FRONT_RECORDER_SOURCE_HEIGHT;
	rec_para.frame_rate = FRONT_RECORDER_SOURCE_FPS * 1000;
	ret = lb_recorder_ctrl(eplayer, LB_REC_SET_PARA, &rec_para);
	if (ret < 0) {
		APP_LOG_E("LB_REC_SET_PARA failed!\n");
		return -1;
	}

	win_para.mode = VDISP_WINDOW_USERDEF;
	win_para.rect.x = ADAS_CALIB_PREX;
	win_para.rect.y = ADAS_CALIB_PREY;
	win_para.rect.width = ADAS_CALIB_PREW;
	win_para.rect.height = ADAS_CALIB_PREH;
	win_para.crop.x = 0;
	win_para.crop.y = 0;
	win_para.crop.width = 1056;
	win_para.crop.height = 1920;
	lb_recorder_ctrl(eplayer, LB_REC_SET_DISP_PARA, (void *)&win_para);

	lb_recorder_ctrl(eplayer, LB_REC_SET_ROTATE, (void *)VIDEO_ROTATE_270);

	ret = lb_recorder_ctrl(eplayer, LB_REC_PREPARE, 0);
	if (ret < 0) {
		printf("LB_REC_PREPARE failed!");
		return -1;
	}

	ret = lb_recorder_ctrl(eplayer, LB_REC_PREVIEW, 0);
	if (ret < 0) {
		printf("LB_REC_PREPARE failed!");
		return -1;
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
static lb_int32 adas_prev_exit(void *para)
{
	lb_int32 ret = 0;

	if (eplayer == NULL) {
		printf("LB_REC_PREPARE failed!");
		return -1;
	}

	ret = lb_recorder_ctrl(eplayer, LB_REC_STOP_PREVIEW, 0);
	if (ret < 0) {
		printf("LB_REC_PREPARE failed!");
		return -1;
	}

	lb_recorder_release(eplayer);

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
static lb_int32 adas_preview_init(void *param)
{
	lb_int32 ret = 0;
	static v_node_t node0;
	static v_node_t node1;
	static v_node_t node2;

	node0.init_op = adas_prev_init;
	node0.exit_op = adas_prev_exit;
	node0.next = &node1;

	node1.init_op = hide_main_view;
	node1.exit_op = show_main_view;
	node1.next = &node2;

	node2.init_op = adas_hide_method;
	node2.exit_op = adas_show_method;
	node2.next = NULL;

	ret = view_stack_push(&node0);
	if (ret != 0) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return ret;
	}

	if (node0.init_op)
		node0.init_op((void *)0);

	if (node1.init_op)
		node1.init_op((void *)0);

	if (node2.init_op)
		node2.init_op((void *)0);

	return SUCCESS;
}

/**
 * adas_preview_exit - nothing to do
 * @param: lb_obj_t object pointer.
 *
 * This function calls nothing
 *
 * Returns 0
 */
static lb_int32 adas_preview_exit(void *param)
{
	return SUCCESS;
}

lb_int32 adas_top_line_init(void *param)
{
	lb_line_t *pproperty;
	lb_obj_t *left_top_line;

	left_top_line = (lb_obj_t *)param;
	if (NULL == left_top_line) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	pproperty = (lb_line_t *)(left_top_line->property);
	if (NULL == pproperty) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}
	printf("[%s,%d] %d:%d\n", __FILE__, __LINE__, pproperty->comms.y,
		adas_get_roi_para_uprow());
	pproperty->comms.y = adas_get_roi_para_uprow();

	return SUCCESS;
}

lb_int32 adas_bot_line_init(void *param)
{
	lb_line_t *pproperty;
	lb_obj_t *left_bottom_line;

	left_bottom_line = (lb_obj_t *)param;
	if (NULL == left_bottom_line) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	pproperty = (lb_line_t *)(left_bottom_line->property);
	if (NULL == pproperty) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}
	printf("[%s,%d]%d:%d\n", __FILE__, __LINE__, pproperty->comms.y,
		adas_get_roi_para_dnrow());
	pproperty->comms.y = adas_get_roi_para_dnrow();

	return SUCCESS;
}

lb_int32 adas_mid_line_init(void *param)
{
	lb_int32 ret = 0;
	lb_obj_t *middle_obj;
	lb_line_t *top_line;
	lb_line_t *bottom_line;
	lb_line_t *middle_line;

	middle_obj = (lb_obj_t *)param;
	if (NULL == middle_obj) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	middle_line = (lb_line_t *)(middle_obj->property);
	if (NULL == middle_line) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	middle_line->comms.x = adas_get_roi_para_midcolumn();
	middle_line->comms.y = adas_get_roi_para_uprow() + 2;
	printf("[%s,%d]%d %d\n", __FILE__, __LINE__, middle_line->comms.x,
		middle_line->comms.y);

	ret = lb_view_get_obj_property_by_id(ADAS_UP_LINE_ID, (void **)&top_line);
	if (0 != ret) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	ret = lb_view_get_obj_property_by_id(ADAS_DN_LINE_ID, (void **)&bottom_line);
	if (0 != ret) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	middle_line->point_num =
		2;
	middle_line->points[0].x =
		0;
	middle_line->points[0].y =
		0;
	middle_line->points[1].x =
		0;
	middle_line->points[1].y =
		adas_get_roi_para_dnrow() - adas_get_roi_para_uprow() - 4;

	printf("[%s,%d]%d %d\n", __FILE__, __LINE__, middle_line->points[1].x,
		middle_line->points[1].y);

	return SUCCESS;
}

static lb_int32 adas_move_top_line(lb_uint32 line_id, lb_uint8 mv_dir)
{
	lb_int32 ret = 0;
	void *obj;
	lb_line_t *line;
	lb_line_t *bottom_line;
	static lb_point_t point;

	ret = lb_view_get_obj_ext_by_id(line_id, &obj);
	if (0 != ret) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}
	ret = lb_view_get_obj_property_by_id(line_id, (void **)&line);
	if (0 != ret) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	ret = lb_view_get_obj_property_by_id(ADAS_DN_LINE_ID, (void **)&bottom_line);
	if (0 != ret) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	if (mv_dir == ADAS_DIRECT_UP) {
		line->comms.y -= ADAS_MOVE_LINE_STEP;
		if (line->comms.y <= ADAS_TOP_LINE_UP_LIMIT) {
			line->comms.y += ADAS_MOVE_LINE_STEP;
			ret = -1;
			return ret;
		}
	} else if (mv_dir == ADAS_DIRECT_DN) {
		line->comms.y += ADAS_MOVE_LINE_STEP;
		if (line->comms.y >= bottom_line->comms.y) {
			line->comms.y -= ADAS_MOVE_LINE_STEP;
			ret = -1;
			return ret;
		}

		if (line->comms.y >= ADAS_TOP_LINE_DN_LIMIT) {
			line->comms.y -= ADAS_MOVE_LINE_STEP;
			ret = -1;
			return ret;
		}
	}

	point.x = line->comms.x;
	point.y = line->comms.y;
	lb_gal_update_line(obj, LB_LINE_UPD_POS, &point);

	return ret;
}

static lb_int32 adas_move_bot_line(lb_uint32 line_id, lb_uint8 mv_dir)
{
	lb_int32 ret = 0;
	void *obj;
	lb_line_t *line;
	lb_line_t *top_line;
	static lb_point_t point;

	ret = lb_view_get_obj_ext_by_id(line_id, &obj);
	if (0 != ret) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}
	ret = lb_view_get_obj_property_by_id(line_id, (void **)&line);
	if (0 != ret) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	ret = lb_view_get_obj_property_by_id(ADAS_UP_LINE_ID, (void **)&top_line);
	if (0 != ret) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	if (mv_dir == ADAS_DIRECT_UP) {
		line->comms.y -= ADAS_MOVE_LINE_STEP;
		if (line->comms.y <= top_line->comms.y) {
			line->comms.y += ADAS_MOVE_LINE_STEP;
			ret = -1;
			return ret;
		}

		if (line->comms.y <= ADAS_BOT_LINE_UP_LIMIT) {
			line->comms.y -= ADAS_MOVE_LINE_STEP;
			ret = -1;
			return ret;
		}
	} else if (mv_dir == ADAS_DIRECT_DN) {
		line->comms.y += ADAS_MOVE_LINE_STEP;
		if (line->comms.y >= ADAS_BOT_LINE_DN_LIMIT) {
			line->comms.y -= ADAS_MOVE_LINE_STEP;
			ret = -1;
			return ret;
		}
	}

	point.x = line->comms.x;
	point.y = line->comms.y;
	lb_gal_update_line(obj, LB_LINE_UPD_POS, &point);

	return ret;
}

static lb_int32 adas_move_mid_line(lb_uint32 line_id, lb_uint8 mv_dir, lb_uint8 base)
{
	void *obj;
	lb_int32 ret = 0;
	lb_line_t *line;
	static lb_point_t point;
	static lb_size_t size;

	ret = lb_view_get_obj_ext_by_id(line_id, &obj);
	if (0 != ret) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}
	ret = lb_view_get_obj_property_by_id(line_id, (void **)&line);
	if (0 != ret) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	if (base == 0x70) {
		if (mv_dir == ADAS_DIRECT_LT) {
			line->comms.x -= ADAS_MOVE_LINE_STEP;
			if (line->comms.x <= ADAS_MID_LINE_LT_LIMIT) {
				line->comms.x += ADAS_MOVE_LINE_STEP;
				ret = -1;
				return ret;
			}
		} else if (mv_dir == ADAS_DIRECT_RT) {
			line->comms.x += ADAS_MOVE_LINE_STEP;
			if (line->comms.x >= ADAS_MID_LINE_RT_LIMIT) {
				line->comms.x -= ADAS_MOVE_LINE_STEP;
				ret = -1;
				return ret;
			}
		}
	} else if (base == 0x00) {
		if (mv_dir == ADAS_DIRECT_UP) {
			line->comms.y -= ADAS_MOVE_LINE_STEP;
			line->points[1].y += ADAS_MOVE_LINE_STEP;
		} else if (mv_dir == ADAS_DIRECT_DN) {
			line->comms.y += ADAS_MOVE_LINE_STEP;
			line->points[1].y -= ADAS_MOVE_LINE_STEP;
		}
	} else if (base == 0xff) {
		if (mv_dir == ADAS_DIRECT_UP)
			line->points[1].y -= ADAS_MOVE_LINE_STEP;
		else if (mv_dir == ADAS_DIRECT_DN)
			line->points[1].y += ADAS_MOVE_LINE_STEP;
	}

	point.x = line->comms.x;
	point.y = line->comms.y;
	size.w = line->points[1].x - line->points[0].x;
	size.h = line->points[1].y - line->points[0].y;

	lb_gal_update_line(obj, LB_LINE_UPD_SIZE, &size);
	lb_gal_update_line(obj, LB_LINE_UPD_POS, &point);

	return ret;
}

static lb_int32 adas_top_line_up(void *param)
{
	lb_int32 ret = 0;

	ret = adas_move_top_line(ADAS_UP_LINE_ID, ADAS_DIRECT_UP);
	if (0 != ret) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	ret = adas_move_mid_line(ADAS_ME_LINE_ID, ADAS_DIRECT_UP, 0x00);
	if (0 != ret) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	return ret;
}

static lb_int32 adas_top_line_dn(void *param)
{
	lb_int32 ret = 0;

	ret = adas_move_top_line(ADAS_UP_LINE_ID, ADAS_DIRECT_DN);
	if (0 != ret) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	ret = adas_move_mid_line(ADAS_ME_LINE_ID, ADAS_DIRECT_DN, 0x00);
	if (0 != ret) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	return ret;
}

static lb_int32 adas_bot_line_up(void *param)
{
	lb_int32 ret = 0;

	ret = adas_move_bot_line(ADAS_DN_LINE_ID, ADAS_DIRECT_UP);
	if (0 != ret) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	ret = adas_move_mid_line(ADAS_ME_LINE_ID, ADAS_DIRECT_UP, 0xff);
	if (0 != ret) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	return ret;
}

static lb_int32 adas_bot_line_dn(void *param)
{
	lb_int32 ret = 0;

	ret = adas_move_bot_line(ADAS_DN_LINE_ID, ADAS_DIRECT_DN);
	if (0 != ret) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	ret = adas_move_mid_line(ADAS_ME_LINE_ID, ADAS_DIRECT_DN, 0xff);
	if (0 != ret) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	return ret;
}

static lb_int32 adas_mid_line_lt(void *param)
{
	lb_int32 ret = 0;

	adas_move_mid_line(ADAS_ME_LINE_ID, ADAS_DIRECT_LT, 0x70);
	if (0 != ret) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	return ret;
}

static lb_int32 adas_mid_line_rt(void *param)
{
	lb_int32 ret = 0;

	adas_move_mid_line(ADAS_ME_LINE_ID, ADAS_DIRECT_RT, 0x70);
	if (0 != ret) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
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
static lb_int32 adas_save_succeed(void)
{
	lb_int32 ret = 0;
	void *obj = NULL;
	char json_str[64];
	const char *result_str;

	lb_view_get_obj_ext_by_id(216, &obj);
	if (NULL == obj) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	strcpy(json_str, "STR_SMART_DRIVE_SAVE_SUCCEED");
	result_str = elang_get_utf8_string_josn(json_str);

	lb_gal_update_label(obj, LB_LABEL_UPD_TXT, (void *)result_str);

	/* there are somthing wrong with dialgo */

	return ret;
}

/**
 * save_failed - show the result(label) means failed
 *
 * This function show the result(label) means failed
 *
 * Returns 0 if called when get success ; otherwise, return -1
 */
static lb_int32 adas_save_failed(void)
{
	lb_int32 ret = 0;
	void *obj = NULL;
	char json_str[64];
	const char *result_str;

	lb_view_get_obj_ext_by_id(216, &obj);
	if (NULL == obj) {
		printf("[%s,%d]Invalid parameters!\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	strcpy(json_str, "STR_SMART_DRIVE_SAVE_FAILED");
	result_str = elang_get_utf8_string_josn(json_str);

	lb_gal_update_label(obj, LB_LABEL_UPD_TXT, (void *)result_str);

	/* there are somthing wrong with dialgo */

	return ret;
}

static lb_int32 adas_save_result(void *param)
{
	lb_int32 ret = 0;
	lb_line_t *line_up;
	lb_line_t *line_down;
	lb_line_t *line_middle;

	/* Save the result of calibration to json */
	printf("save_result\n");

	ret = lb_view_get_obj_property_by_id(ADAS_UP_LINE_ID, (void **)&line_up);
	if (0 != ret) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	ret = lb_view_get_obj_property_by_id(ADAS_DN_LINE_ID, (void **)&line_down);
	if (0 != ret) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}
	ret = lb_view_get_obj_property_by_id(ADAS_ME_LINE_ID, (void **)&line_middle);
	if (0 != ret) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return LB_ERROR_NO_MEM;
	}

	ret = adas_save_roi_para(line_up->comms.y,
		line_down->comms.y, line_middle->comms.x);

	if (ret == 0)
		adas_save_succeed();
	else
		adas_save_failed();

	return ret;
}

lb_int32 adas_calib_init_funcs(void)
{
	lb_int32 err = SUCCESS;

	err |= lb_fmngr_reg_init_func("adas_preview_init",
			adas_preview_init);
	err |= lb_fmngr_reg_exit_func("adas_preview_exit",
			adas_preview_exit);

	err |= lb_fmngr_reg_init_func("adas_load_method",
			adas_load_method);

	err |= lb_fmngr_reg_init_func("adas_top_line_init",
			adas_top_line_init);
	err |= lb_fmngr_reg_init_func("adas_bot_line_init",
			adas_bot_line_init);
	err |= lb_fmngr_reg_init_func("adas_mid_line_init",
			adas_mid_line_init);

	return err;
}

lb_int32 adas_calib_uninit_funcs(void)
{
	lb_int32 err = SUCCESS;

	err |= lb_fmngr_unreg_init_func(adas_top_line_init);
	err |= lb_fmngr_unreg_init_func(adas_bot_line_init);
	err |= lb_fmngr_unreg_init_func(adas_mid_line_init);

	err |= lb_fmngr_unreg_init_func(adas_preview_init);
	err |= lb_fmngr_unreg_exit_func(adas_preview_exit);

	err |= lb_fmngr_unreg_init_func(adas_load_method);

	return err;
}

lb_int32 adas_calib_resp_funcs(void)
{
	lb_int32 err = SUCCESS;

	err |= lb_reg_resp_msg_func(ADAS_CALI_TOP_UP,
			adas_top_line_up);
	err |= lb_reg_resp_msg_func(ADAS_CALI_TOP_DN,
			adas_top_line_dn);
	err |= lb_reg_resp_msg_func(ADAS_CALI_BOT_UP,
			adas_bot_line_up);
	err |= lb_reg_resp_msg_func(ADAS_CALI_BOT_DN,
			adas_bot_line_dn);
	err |= lb_reg_resp_msg_func(ADAS_CALI_MID_LT,
			adas_mid_line_lt);
	err |= lb_reg_resp_msg_func(ADAS_CALI_MID_RT,
			adas_mid_line_rt);

	err |= lb_reg_resp_msg_func(ADAS_CALI_SAVE_RES,
			adas_save_result);

	return SUCCESS;
}

lb_int32 adas_calib_unresp_funcs(void)
{
	lb_int32 err = SUCCESS;

	err |= lb_unreg_resp_msg_func(adas_top_line_up);
	err |= lb_unreg_resp_msg_func(adas_top_line_dn);
	err |= lb_unreg_resp_msg_func(adas_bot_line_up);
	err |= lb_unreg_resp_msg_func(adas_bot_line_dn);
	err |= lb_unreg_resp_msg_func(adas_mid_line_lt);
	err |= lb_unreg_resp_msg_func(adas_mid_line_rt);

	err |= lb_unreg_resp_msg_func(adas_save_result);

	return SUCCESS;
}
