/*
 * lb_recorder.c - Standard functionality for recorder.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <oscl.h>
#include <base_component.h>
#include "vrender_component.h"
#include "vrec_component.h"
#include "framework_common.h"
#include "lb_recorder.h"
#include <getopt.h>
#include "recorder_private.h"

/**
 * lb_recorder_creat - get a recorder handle.
 * This function will load all components recorder needs.
 *
 * Returns recorder handle on success, NULL otherwise..
 */
void *lb_recorder_creat(void)
{
	lb_recorder_t *recorder;
	int ret = 0;

	OSCL_TRACE("creat start");
	recorder = oscl_zalloc(sizeof(lb_recorder_t));
	oscl_param_check_exit(recorder != NULL, -1, NULL);

	recorder->video_rec = video_rec_creat();
	oscl_param_check_exit(recorder->video_rec != NULL, -1, NULL);

	recorder->lock = oscl_zalloc(sizeof(pthread_mutex_t));
	if (recorder->lock)
		pthread_mutex_init(recorder->lock, NULL);
EXIT:
	if (ret != 0 && recorder != NULL) {
		lb_recorder_release(recorder);
		recorder = NULL;
	}
	OSCL_TRACE("recorder hdl:%x", recorder);
	return recorder;
}
RTM_EXPORT(lb_recorder_creat);

/**
 * lb_recorder_release - release resource recorder used.
 *
 * @hdl: recorder handle
 *
 * Returns 0 on success, -EERROR otherwise..
 */
void lb_recorder_release(void *hdl)
{
	lb_recorder_t *recorder;
	OSCL_TRACE("hdl:%x", hdl);
	recorder = (lb_recorder_t *)hdl;

	if (hdl == NULL)
		return;
	video_rec_release(recorder->video_rec);
	recorder->video_rec = NULL;
	if (recorder->lock) {
		pthread_mutex_destroy(recorder->lock);
		oscl_free(recorder->lock);
		recorder->lock = NULL;
	}
	oscl_free(recorder);

	OSCL_TRACE("return");
}
RTM_EXPORT(lb_recorder_release);

/**
 * lb_recorder_ctrl - will send commands to a recorder
 *
 * @hdl: handle of recorder to be controled. recorder handle is get by lb_recorder_creat
 * @cmd: index of command
 * @para: pointer to application allocated structure to be used by the recorder.
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int lb_recorder_ctrl(void *hdl, int cmd, void *para)
{
	lb_recorder_t *recorder = (lb_recorder_t *)hdl;
	int ret = 0;

	OSCL_TRACE("hdl:%x, cmd(%#x):%s, para:%p",
			hdl, cmd, media_cmd_as_string(cmd), para);
	oscl_param_check(hdl != NULL, -1, NULL);
	if (LB_REC_FREE_FRAME != cmd)
		pthread_mutex_lock(recorder->lock);
	switch (cmd) {
	case LB_REC_SET_MODE:
		video_rec_set_mode(recorder->video_rec, (int)(*((int *)para)));
		break;
	case LB_REC_SET_TIME_LAG_PARA:
		video_rec_set_timelag_para(recorder->video_rec,
			(rec_time_lag_para_t *)para);
		break;
	case LB_REC_PREPARE:
		ret = video_rec_prepare(recorder->video_rec);
		oscl_param_check_exit(ret == 0, -1, NULL);
		break;
	case LB_REC_SET_FIX_DURATION_PARA:
		ret = video_rec_set_fix_duration_param(recorder->video_rec, hdl, para);
		break;
	case LB_REC_PREVIEW:
		ret = video_rec_preview(recorder->video_rec);
		break;
	case LB_REC_PAUSE_PREVIEW:
		ret = video_rec_pause_preview(recorder->video_rec);
		break;
	case LB_REC_CONTINUE_PREVIEW:
		ret = video_rec_continue_preview(recorder->video_rec);
		break;
	case LB_REC_STOP_PREVIEW:
		ret = video_rec_stop_preview(recorder->video_rec);
		break;
	case LB_REC_START:
		ret = video_rec_start(recorder->video_rec);
		oscl_param_check_exit(ret == 0, -1, NULL);
		break;
	case LB_REC_STOP:
		ret = video_rec_stop(recorder->video_rec);
		oscl_param_check_exit(ret == 0, -1, NULL);
		break;
	case LB_REC_SET_OUTPUT_FILE:
		ret = video_rec_set_output_file(recorder->video_rec, (char *)para);
		break;
	case LB_REC_SET_OUTPUT_FORMAT:
		ret = video_rec_set_output_format(recorder->video_rec,
						  (output_format_t)para);
		break;
	case LB_REC_SET_ROTATE:
		ret = video_rec_set_rotate(recorder->video_rec, (int)para);
		break;
	case LB_REC_SET_USER_DEF_SYS_PRAR:
		ret = video_rec_set_user_def_sys_cfg(recorder->video_rec, para);
		break;
	case LB_REC_SET_VIDEO_SOURCE:
		ret = video_rec_set_video_source(recorder->video_rec, (char *)para);
		break;
	case LB_REC_SET_SINK:
		ret = video_rec_set_video_sink(recorder->video_rec, (char *)para);
		break;

	case LB_REC_SET_CB_FRAME:
		pthread_mutex_unlock(recorder->lock);
		ret = video_rec_set_frame_cb(recorder->video_rec, para);
		pthread_mutex_lock(recorder->lock);
		break;
	case LB_REC_FREE_FRAME:
		ret = app_empty_buffer_done((al_frame_t *)para);
		break;
	case LB_REC_SET_LAYER_LEVEL:
		ret = video_set_layer_level(recorder->video_rec,
					    (video_layer_level_t)para);
		break;
	case LB_REC_SET_DISP_PARA:
		ret = video_set_disp_para(recorder->video_rec, para);
		break;
	case LB_REC_SET_PARA:
		ret = video_rec_set_para(recorder->video_rec, (rec_param_t *)para);
		break;
	case LB_REC_GET_PARA:
		ret = video_rec_get_para(recorder->video_rec, (rec_param_t *)para);
		break;
	case LB_REC_GET_STATUS:
		ret = video_rec_get_status(recorder->video_rec, para);
		break;
	case LB_REC_GET_TIME:
		ret = video_rec_get_time(recorder->video_rec, (int *)para);
		break;
	case LB_REC_SET_WATERMARK_SOURCE:
		ret = video_rec_wm_set_bnp(recorder->video_rec, para);
		break;
	case LB_REC_SET_WATERMARK_INDEX:
		ret = video_rec_wm_set_mark(recorder->video_rec, para);
		break;
	case LB_REC_PANO_CREAT:
		ret = video_pano_creat(recorder->video_rec);
		break;
	case LB_REC_PANO_RELEASE:
		video_pano_release(recorder->video_rec);
		break;
	case LB_REC_PANO_START:
		ret = video_pano_start(recorder->video_rec);
		break;
	case LB_REC_PANO_STOP:
		ret = video_pano_stop(recorder->video_rec);
		break;
	case LB_REC_PANO_CALI_PROCESS:
		ret = video_pano_cali_process(recorder->video_rec, (cali_contex_t *)para);
		break;
	case LB_REC_PANO_SET_PREVIEW_SIZE:
		ret = video_pano_set_preview_size(recorder->video_rec, (vsize_t *)para);
		break;
	case LB_REC_PANO_SET_DISP_MODE:
		ret = video_pano_set_disp_mode(recorder->video_rec, (win_para_t *)para);
		break;
	case LB_REC_PANO_SET_CALI_PARA:
		ret = video_pano_set_cali_para(recorder->video_rec,
						(cali_param_t *)para);
		break;
	case LB_REC_PANO_SET_INIT_PARA:
		ret = video_pano_set_init_para(recorder->video_rec,
						(pano_param_t *)para);
		break;
	case LB_REC_PANO_SET_CALI_DATA:
		ret = video_pano_set_cali_data(recorder->video_rec,
						(cali_out_data_t *)para);
		break;
	case LB_REC_PANO_GET_CALI_DATA:
		ret = video_pano_get_cali_data(recorder->video_rec,
						(cali_out_data_t *)para);
		break;
	case LB_REC_PANO_SET_LAYER_LEVEL:
		ret = video_pano_set_layer_level(recorder->video_rec,
					    (video_layer_level_t)para);
		break;
	case LB_REC_TAKE_PICTURE:
		pthread_mutex_unlock(recorder->lock);
		ret = video_rec_take_picture(recorder->video_rec, para);
		pthread_mutex_lock(recorder->lock);
		break;
	case LB_REC_SET_AUDIO_MUTE:
		ret = video_rec_set_mute(recorder->video_rec, (int)para);
		break;
	default:
		break;
	}
EXIT:
	if (LB_REC_FREE_FRAME != cmd)
		pthread_mutex_unlock(recorder->lock);
	OSCL_TRACE("hdl:%x, cmd(%#x):%s, ret:%x",
			hdl, cmd, media_cmd_as_string(cmd), ret);
	return ret;
}
RTM_EXPORT(lb_recorder_ctrl);
