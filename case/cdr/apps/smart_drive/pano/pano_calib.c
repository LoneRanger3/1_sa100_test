/*
 * pano_calib.c - pano calibrate code for LomboTech
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
#include "pano_calib.h"
#include "main_view.h"
#include "pano_set.h"
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

#include "lombo_disp.h"
#include "disp_list.h"
#include "system/system.h"

static void *recorder;
static void *p_obj[2];
static lb_int32 result;

cali_contex_t cutl_ctx;
lb_int32 cutl_use;
lb_int32 cutl_dnthr;
lb_int32 cutl_upthr;
lb_int32 cutl_val;

/**
 * load_pano_method - save objects from the pano method view
 *
 * This function save objects from the pano method view
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
static lb_int32 load_pano_method(void *para)
{
	lb_int32 ret = -1;
	lb_int32 i = 0;
	lb_uint32 id = 0;

	for (i = 0; i < 2; i++) {
		id = 200 + i;
		ret = lb_view_get_obj_ext_by_id(id, &p_obj[i]);
		if (0 != ret) {
			APP_LOG_W("failed\n");
			return LB_ERROR_NO_MEM;
		}
	}

	ret = 0;
	return ret;
}

/**
 * hide_method - hide the method view
 *
 * This function hide the main view
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
static lb_int32 hide_method(void *para)
{
	lb_int32 ret = -1;
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
static lb_int32 show_method(void *para)
{
	lb_int32 ret = -1;
	lb_int32 i = 0;

	for (i = 0; i < 2; i++)
		ret |= lb_gal_set_obj_hidden(p_obj[i], 0);

	return ret;
}

static lb_int32 prev_disp(win_para_t *win_para)
{
	lb_int32 ret = 0;

	if (win_para == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	win_para->mode = VDISP_WINDOW_USERDEF;
	win_para->rect.x = PANO_PREVIEW_WIND_X;
	win_para->rect.y = PANO_PREVIEW_WIND_Y;
	win_para->rect.width = PANO_PREVIEW_WIND_W;
	win_para->rect.height = PANO_PREVIEW_WIND_H;

exit:
	return ret;
}

/**
 * prev_init - initial the mod to start preview
 *
 * This function initial the mod to start preview
 *
 * Returns 0 if called when get success ; otherwise, return -1
 */
static lb_int32 prev_init(void *para)
{
	lb_int32 ret = 0;
	win_para_t win_para;
	rec_param_t rec_para;
#ifdef SYS_MEMERY_128M_FOR_DOUBLE_1080P
	user_def_sys_cfg_t userdef_sys_cfg;
#endif


	memset(&win_para, 0x00, sizeof(win_para));
	memset(&rec_para, 0x00, sizeof(rec_para));

	ret = prev_disp(&win_para);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	recorder = lb_recorder_creat();
	if (recorder == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}
#ifdef SYS_MEMERY_128M_FOR_DOUBLE_1080P
	if ((FRONT_RECORDER_SOURCE_HEIGHT == 1080) &&
			(REAR_RECORDER_SOURCE_HEIGHT == 1080)) {
		userdef_sys_cfg.camera_buf_num = REAR_CAMERA_BUFFER_NUM;
		lb_recorder_ctrl(recorder, LB_REC_SET_USER_DEF_SYS_PRAR,
			&userdef_sys_cfg);
	}
#endif
	ret = lb_recorder_ctrl(recorder, LB_REC_SET_VIDEO_SOURCE, "vic");
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	lb_recorder_ctrl(recorder, LB_REC_GET_PARA, &rec_para);
	rec_para.source_width = REAR_RECORDER_SOURCE_WIDTH;
	rec_para.source_height = REAR_RECORDER_SOURCE_HEIGHT;
	rec_para.width = REAR_RECORDER_SOURCE_WIDTH;
	rec_para.height = REAR_RECORDER_SOURCE_HEIGHT;
	rec_para.frame_rate = REAR_RECORDER_SOURCE_FPS * 1000;
	ret = lb_recorder_ctrl(recorder, LB_REC_SET_PARA, &rec_para);
	if (ret < 0) {
		APP_LOG_E("LB_REC_SET_PARA failed!\n");
		ret = -1;
		goto exit;
	}

	ret = lb_recorder_ctrl(recorder, LB_REC_SET_ROTATE, (void *)1);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = lb_recorder_ctrl(recorder, LB_REC_SET_DISP_PARA, (void *)&win_para);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}
	if (system_get_rearmirr_enable()) {
		ret = lb_recorder_ctrl(recorder, LB_REC_SET_ROTATE,
		(void *)VIDEO_ROTATE_FLIP_H_ROT_90);
		if (ret != 0) {
			APP_LOG_W("failed\n");
			ret = -1;
			goto exit;
		}
	}
	ret = lb_recorder_ctrl(recorder, LB_REC_PREPARE, 0);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = lb_recorder_ctrl(recorder, LB_REC_PREVIEW, 0);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	return ret;

exit:
	if (recorder) {
		lb_recorder_release(recorder);
		recorder = NULL;
	}

	return ret;
}

/**
 * prev_exit - exit the mod to stop preview
 *
 * This function exit the mod to stop preview
 *
 * Returns 0 if called when get success ; otherwise, return -1
 */
static lb_int32 prev_exit(void *para)
{
	lb_int32 ret = 0;

	if (recorder) {
		lb_recorder_ctrl(recorder, LB_REC_STOP_PREVIEW, 0);
		lb_recorder_release(recorder);
		recorder = NULL;
	}

	return ret;
}

static lb_int32 pano_cali(cali_param_t *cali_para)
{
	lb_int32 ret = 0;
	pano_set_t set;

	memset(&set, 0x00, sizeof(pano_set_t));

	if (cali_para == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = motor_get_set_para(&set);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	cali_para->box_rows = 5;
	cali_para->box_cols = 11;
	cali_para->box_width = 20;
	cali_para->box_height = 20;
	cali_para->dist_2_rear = set.motor_d2r;
	cali_para->car_width = (lb_int32)(set.motor_w * 100);
	cali_para->car_length = (lb_int32)(set.motor_l * 100);
	cali_para->front_dist = 50;
	cali_para->rear_dist = 200;
	cali_para->align = 0;
	cali_para->use_ext_cali_img = 0;
	cali_para->ext_cali_img.width = 1280;
	cali_para->ext_cali_img.height = 720;
	strncpy(cali_para->ext_cali_img.format, "nv12",
		sizeof(cali_para->ext_cali_img.format) - 1);
	strncpy(cali_para->ext_cali_img.path, ROOTFS_MOUNT_PATH"/res/cali_yuv.bin",
		sizeof(cali_para->ext_cali_img.path) - 1);

exit:
	return ret;
}

static lb_int32 pano_dest(win_para_t *win_para)
{
	lb_int32 ret = 0;

	if (win_para == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	win_para->mode = VDISP_WINDOW_USERDEF;
	win_para->rect.x = PANO_BIRDBIEW_DEST_X;
	win_para->rect.y = PANO_BIRDBIEW_DEST_Y;
	win_para->rect.width = PANO_BIRDBIEW_DEST_W;
	win_para->rect.height = PANO_BIRDBIEW_DEST_H;

exit:
	return ret;
}

static lb_int32 pano_source(vsize_t *prev_size)
{
	lb_int32 ret = 0;

	if (prev_size == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	prev_size->width = PANO_BIRDBIEW_SOURCE_W;
	prev_size->height = PANO_BIRDBIEW_SOURCE_H;

exit:
	return ret;
}

/**
 * pano_init - initial the mod to start panoramic algorithm
 *
 * This function initial the mod to start panoramic algorithm
 *
 * Returns 0 if called when get success ; otherwise, return -1
 */
static lb_int32 pano_init(void *para)
{
	cali_param_t cali_para;
	win_para_t win_para;
	vsize_t prev_size;
	lb_int32 ret = 0;

	memset(&cali_para, 0, sizeof(cali_para));
	memset(&win_para, 0, sizeof(win_para));
	memset(&prev_size, 0, sizeof(vsize_t));

	cutl_use = 0;
	cutl_val = 0;
	cutl_dnthr = 0;
	cutl_upthr = 0;

	ret = pano_cali(&cali_para);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = pano_dest(&win_para);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = pano_source(&prev_size);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = lb_recorder_ctrl(recorder, LB_REC_PANO_CREAT,
		NULL);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = lb_recorder_ctrl(recorder, LB_REC_PANO_SET_CALI_PARA,
		(void *)&cali_para);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = lb_recorder_ctrl(recorder, LB_REC_PANO_SET_DISP_MODE,
		(void *)&win_para);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = lb_recorder_ctrl(recorder, LB_REC_PANO_SET_PREVIEW_SIZE,
		(void *)&prev_size);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = lb_recorder_ctrl(recorder, LB_REC_PANO_START,
		NULL);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = lb_recorder_ctrl(recorder, LB_REC_PANO_SET_LAYER_LEVEL,
		(void *)VIDEO_LAYER_BOTTOM);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = lb_recorder_ctrl(recorder, LB_REC_PANO_CALI_PROCESS,
		&cutl_ctx);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	cutl_use = 0;
	cutl_val = (PANO_BIRDBIEW_DEST_W * cutl_ctx.cutline) /
		PANO_BIRDBIEW_SOURCE_H;
	cutl_dnthr = (PANO_BIRDBIEW_DEST_W * cutl_ctx.cutline_dnthr) /
		PANO_BIRDBIEW_SOURCE_H;
	cutl_upthr = (PANO_BIRDBIEW_DEST_W * cutl_ctx.cutline_upthr) /
		PANO_BIRDBIEW_SOURCE_H;

exit:
	return ret;
}

/**
 * pano_exit - exit the mod to stop panoramic algorithm
 *
 * This function exit the mod to stop panoramic algorithm
 *
 * Returns 0 if called when get success ; otherwise, return -1
 */
static lb_int32 pano_exit(void *para)
{
	lb_int32 ret = 0;
	cali_out_data_t out_data;

	memset(&out_data, 0, sizeof(cali_out_data_t));

	lb_recorder_ctrl(recorder, LB_REC_PANO_GET_CALI_DATA, &out_data);
	if (out_data.data) {
		free(out_data.data);
		out_data.data = NULL;
	}

	lb_recorder_ctrl(recorder, LB_REC_PANO_STOP, NULL);
	lb_recorder_ctrl(recorder, LB_REC_PANO_RELEASE, NULL);

	/* temporary solution to solve key tone distortion when exit from pano */
	rt_thread_delay(60);

	return ret;
}

/**
 * pano_preview_init - call a few of func to start preview
 * @param: lb_obj_t object pointer.
 *
 * This function  call a few of func to start preview
 *
 * Returns 0
 */
static lb_int32 pano_preview_init(void *param)
{
	lb_int32 ret = -1;
	static v_node_t node0;
	static v_node_t node1;
	static v_node_t node2;

	node0.init_op = prev_init;
	node0.exit_op = prev_exit;
	node0.next = &node1;

	node1.init_op = hide_main_view;
	node1.exit_op = show_main_view;
	node1.next = &node2;

	node2.init_op = hide_method;
	node2.exit_op = show_method;
	node2.next = NULL;

	ret = view_stack_push(&node0);
	if (ret != 0) {
		APP_LOG_W("failed\n");
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
 * pano_preview_exit - nothing to do
 * @param: lb_obj_t object pointer.
 *
 * This function calls nothing
 *
 * Returns 0
 */
static lb_int32 pano_preview_exit(void *param)
{
	return 0;
}

/**
 * pano_view_init - call a few of func to start panoramic view
 * @param: lb_obj_t object pointer.
 *
 * This function call a few of func to start panoramic view
 *
 * Returns 0
 */
static lb_int32 pano_view_init(void *param)
{
	lb_int32 ret = -1;
	static v_node_t node;

	node.init_op = pano_init;
	node.exit_op = pano_exit;
	node.next = NULL;

	ret = view_stack_push(&node);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		return ret;
	}

	if (node.init_op) {
		ret = node.init_op((void *)0);
		if (ret != 0)
			result = -1;
		else
			result = 0;
		node.init_op = NULL;
	}

	return SUCCESS;
}

/**
 * pano_view_exit - nothing to do
 * @param: lb_obj_t object pointer.
 *
 * This function calls nothing
 *
 * Returns 0
 */
static lb_int32 pano_view_exit(void *param)
{
	return 0;
}

static lb_int32 explain_init(void *param)
{
	void *obj = NULL;
	char json_str[64];
	const char *result_str;

	if (result == 0)
		return SUCCESS;

	lb_view_get_obj_ext_by_id(214, &obj);
	if (NULL == obj) {
		APP_LOG_W("failed\n");
		return LB_ERROR_NO_MEM;
	}

	strcpy(json_str, "STR_SMART_DRIVE_PANO_CALIBRATE_EXPLAIN_FAILED");
	result_str = elang_get_utf8_string_josn(json_str);

	lb_gal_update_label(obj, LB_LABEL_UPD_TXT, (void *)result_str);


	return SUCCESS;
}

static lb_int32 explain_exit(void *param)
{
	return 0;
}

static lb_int32 result_init(void *param)
{
	void *obj = NULL;
	char json_str[64];
	const char *result_str;

	if (result == 0)
		return SUCCESS;

	lb_view_get_obj_ext_by_id(216, &obj);
	if (NULL == obj) {
		APP_LOG_W("failed\n");
		return LB_ERROR_NO_MEM;
	}

	strcpy(json_str, "STR_SMART_DRIVE_PANO_CALIBRATE_RESULT_FAILED");
	result_str = elang_get_utf8_string_josn(json_str);

	lb_gal_update_label(obj, LB_LABEL_UPD_TXT, (void *)result_str);

	return SUCCESS;
}

static lb_int32 result_exit(void *param)
{
	return 0;
}

static lb_int32 move_side(lb_uint32 line_id, lb_uint8 mv_dir)
{
	lb_int32 ret = -1;
	void *obj;
	lb_line_t *line;
	static lb_point_t point;

	ret = lb_view_get_obj_ext_by_id(line_id, &obj);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		return LB_ERROR_NO_MEM;
	}
	ret = lb_view_get_obj_property_by_id(line_id, (void **)&line);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		return LB_ERROR_NO_MEM;
	}

	/* up - 0x00 */
	if (mv_dir == 0x00) {
		line->comms.y -= 1;
		if (line->comms.y <= cutl_upthr)
			line->comms.y = cutl_upthr;
		/* down - 0xff */
	} else if (mv_dir == 0xff) {
		line->comms.y += 1;
		if (line->comms.y >= cutl_dnthr)
			line->comms.y = cutl_dnthr;
	}

	point.x = line->comms.x;
	point.y = line->comms.y;
	lb_gal_update_line(obj, LB_LINE_UPD_POS, &point);

	cutl_ctx.cutline = line->comms.y * PANO_BIRDBIEW_SOURCE_H / PANO_BIRDBIEW_DEST_W;
	cutl_use = 1;

	ret = 0;
	return ret;
}

static lb_int32 line_init(void *param)
{
	lb_int32 ret = 0;
	void *obj = NULL;
	lb_line_t *line = NULL;
	static lb_point_t point;

	ret = lb_view_get_obj_ext_by_id(200, &obj);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		return LB_ERROR_NO_MEM;
	}
	ret = lb_view_get_obj_property_by_id(200, (void **)&line);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		return LB_ERROR_NO_MEM;
	}

	if (cutl_val != 0) {
		lb_gal_set_obj_hidden(obj, 0);

		/* line->comms.y */
		line->comms.y = cutl_val;
		point.x = line->comms.x;
		point.y = line->comms.y;
		lb_gal_update_line(obj, LB_LINE_UPD_POS, &point);

		return ret;
	}

	lb_gal_set_obj_hidden(obj, 1);

	return ret;
}

static lb_int32 line_exit(void *param)
{
	return 0;
}

static lb_int32 line_up(void *param)
{
	lb_int32 ret = -1;

	ret = move_side(200, 0x00);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		return LB_ERROR_NO_MEM;
	}

	ret  = 0;
	return ret;
}

static lb_int32 line_down(void *param)
{
	lb_int32 ret = -1;

	ret = move_side(200, 0xff);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		return LB_ERROR_NO_MEM;
	}

	ret  = 0;
	return ret;
}

static lb_int32 save_data(lb_int32 size, lb_uint8 *data, FILE *file)
{
	lb_int32 ret = 0;
	lb_int32 len = 0;

	if (file == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	if (data && size) {
		len = fwrite(data, 1, size, file);
		if (len != size) {
			APP_LOG_W("failed\n");
			ret = -1;
			goto exit;
		}
	}

exit:
	return ret;
}

static lb_int32 save_bin(void)
{
	lb_int32 ret = 0;
	cali_out_data_t out;
	pano_set_t set;
	FILE *file = NULL;

	memset(&out, 0x00, sizeof(cali_out_data_t));
	memset(&set, 0x00, sizeof(pano_set_t));

	if (recorder == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = lb_recorder_ctrl(recorder, LB_REC_PANO_GET_CALI_DATA, &out);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = motor_get_set_para(&set);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	file = fopen(PANO_OUT_BIN_PATH, "wb");
	if (file == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}
	fseek(file, 0, SEEK_SET);

	ret = save_data(4, (lb_uint8 *)&cutl_use, file);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = save_data(sizeof(cali_contex_t), (lb_uint8 *)&cutl_ctx, file);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = save_data(sizeof(pano_set_t), (lb_uint8 *)&set, file);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = save_data(4, (lb_uint8 *)&out.data_size, file);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = save_data(out.data_size, (lb_uint8 *)out.data, file);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	if (file) {
		fclose(file);
		file = NULL;
	}

	return ret;

exit:
	if (file) {
		fclose(file);
		file = NULL;
	}

	return ret;
}

/**
 * save_succeed - show the result(label) means successed
 *
 * This function show the result(label) means successed
 *
 * Returns 0 if called when get success ; otherwise, return -1
 */
static lb_int32 save_succeed(void)
{
	lb_int32 ret = -1;
	void *obj = NULL;
	char json_str[64];
	const char *result_str;

	lb_view_get_obj_ext_by_id(216, &obj);
	if (NULL == obj) {
		APP_LOG_W("failed\n");
		return LB_ERROR_NO_MEM;
	}

	strcpy(json_str, "STR_SMART_DRIVE_SAVE_SUCCEED");
	result_str = elang_get_utf8_string_josn(json_str);

	lb_gal_update_label(obj, LB_LABEL_UPD_TXT, (void *)result_str);

	/* there are somthing wrong with dialgo */

	ret = 0;
	return ret;
}

/**
 * save_failed - show the result(label) means failed
 *
 * This function show the result(label) means failed
 *
 * Returns 0 if called when get success ; otherwise, return -1
 */
static lb_int32 save_failed(void)
{
	lb_int32 ret = -1;
	void *obj = NULL;
	char json_str[64];
	const char *result_str;

	lb_view_get_obj_ext_by_id(216, &obj);
	if (NULL == obj) {
		APP_LOG_W("failed\n");
		return LB_ERROR_NO_MEM;
	}

	strcpy(json_str, "STR_SMART_DRIVE_SAVE_FAILED");
	result_str = elang_get_utf8_string_josn(json_str);

	lb_gal_update_label(obj, LB_LABEL_UPD_TXT, (void *)result_str);

	/* there are somthing wrong with dialgo */

	ret = 0;
	return ret;
}

static lb_int32 save_result(void *param)
{
	lb_int32 ret = -1;

	if (result == 0) {
		ret = save_bin();
		if (ret == 0)
			save_succeed();
		else
			save_failed();
	}

	return ret;
}

lb_int32 pano_calib_init_funcs(void)
{
	lb_int32 err = SUCCESS;

	err |= lb_fmngr_reg_init_func("pano_preview_init",
			pano_preview_init);
	err |= lb_fmngr_reg_exit_func("pano_preview_exit",
			pano_preview_exit);

	err |= lb_fmngr_reg_init_func("pano_view_init",
			pano_view_init);
	err |= lb_fmngr_reg_exit_func("pano_view_exit",
			pano_view_exit);

	err |= lb_fmngr_reg_init_func("explain_init",
			explain_init);
	err |= lb_fmngr_reg_exit_func("explain_exit",
			explain_exit);

	err |= lb_fmngr_reg_init_func("result_init",
			result_init);
	err |= lb_fmngr_reg_exit_func("result_exit",
			result_exit);

	err |= lb_fmngr_reg_init_func("line_init",
			line_init);
	err |= lb_fmngr_reg_exit_func("line_exit",
			line_exit);

	err |= lb_fmngr_reg_init_func("load_pano_method",
			load_pano_method);

	return err;
}
lb_int32 pano_calib_uninit_funcs(void)
{
	lb_int32 err = SUCCESS;

	err |= lb_fmngr_unreg_init_func(pano_preview_init);
	err |= lb_fmngr_unreg_exit_func(pano_preview_exit);

	err |= lb_fmngr_unreg_init_func(pano_view_init);
	err |= lb_fmngr_unreg_exit_func(pano_view_exit);

	err |= lb_fmngr_unreg_init_func(explain_init);
	err |= lb_fmngr_unreg_exit_func(explain_exit);

	err |= lb_fmngr_unreg_init_func(result_init);
	err |= lb_fmngr_unreg_exit_func(result_exit);

	err |= lb_fmngr_unreg_init_func(line_init);
	err |= lb_fmngr_unreg_exit_func(line_exit);

	err |= lb_fmngr_unreg_init_func(load_pano_method);

	return err;
}

lb_int32 pano_calib_resp_funcs(void)
{
	lb_int32 err = SUCCESS;

	err |= lb_reg_resp_msg_func(PANO_CALI_LINE_UP,
			line_up);
	err |= lb_reg_resp_msg_func(PANO_CALI_LINE_DOWN,
			line_down);

	err |= lb_reg_resp_msg_func(PANO_CALI_SAVE_RES,
			save_result);

	return err;
}

lb_int32 pano_cailb_unresp_funcs(void)
{
	lb_int32 err = SUCCESS;

	err |= lb_unreg_resp_msg_func(line_up);
	err |= lb_unreg_resp_msg_func(line_down);

	err |= lb_unreg_resp_msg_func(save_result);

	return err;
}
