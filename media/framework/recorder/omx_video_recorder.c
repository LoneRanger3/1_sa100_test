/*
 * omx_video_recorder.c - Standard functionality for video stream recorder.
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
#define DBG_LEVEL		DBG_ERR
#include <oscl.h>
#include <base_component.h>
#include "vrender_component.h"
#include "vrec_component.h"
#include "framework_common.h"
#include "lb_recorder.h"
#include "recorder_private.h"
#include "omx_vendor_lb.h"
#include "pano_component.h"
#include "venc_component.h"
#include "av_media_type.h"

#define ENCODE_VIDEO
#define ENCODE_AUDIO
#define ISP_DEV_NAME "isp"
#define ISP_CAP_DEV_NAME "isp_cap.0"
#define ISP_PREV_DEV_NAME "isp_prev.0"

#define __ALIGN(x, a) (((x) + (a) - 1) & ~((a) - 1))

/**
 * _get_config_venc - get default config for video encoder.
 *
 * @venc_info: video encoder handle
 *
 */
static void _get_config_venc(venc_info_t *venc_info)
{
	venc_info->out_width = 1280;
	venc_info->out_height = 720;
	venc_info->bitrate = 4000000;
	venc_info->vout->buf_size = ENCODER_BUF_SIZE;
	venc_info->vout->nbuffer = ENCODER_DEFAULT_BUF_NUM;
	venc_info->play_framerate = 25000; /* 25fps */
	venc_info->interval = 0; /* default disable fast record */
}

/**
 * _get_config_venc - get default config for video sink.
 *
 * @venc_info: video sink handle
 *
 */
static void _get_config_vsink(vsink_info_t *vsink_info)
{
	disp_size_t size;
	disp_hdl_t *disp_dev;

	/* to do: display operation should be done in components */
	if (vsink_info->dev_name == NULL)
		vsink_info->dev_name = oscl_strdup(REC_DISPLAY_DEVICE_NAME);

	disp_dev = oscl_open_disp_engine();
	if (disp_dev) {
		oscl_disp_get_screen_size(disp_dev, &size);
		vsink_info->disp_para.mode = VDISP_WINDOW_USERDEF;
		vsink_info->disp_para.win_rect.width = size.width;
		vsink_info->disp_para.win_rect.height = size.height;
		vsink_info->disp_para.win_rect.left = 0;
		vsink_info->disp_para.win_rect.top = 0;

		/* only use VDISP_WINDOW_USERDEF mode,
		 * if crop rect not set, render component use fb size */
		vsink_info->disp_para.crop_rect.left = 192;
		vsink_info->disp_para.crop_rect.top = 0;
		vsink_info->disp_para.crop_rect.width = 320;
		vsink_info->disp_para.crop_rect.height = 1280;
	}
	oscl_close_disp_engine(disp_dev);

}

/**
 * _get_config_venc - get default config for muxer.
 *
 * @venc_info: video muxer info structure
 *
 */
static void _get_config_mux(mux_info_t *mux_info)
{
	mux_info->fmt = REC_OUTPUT_FORMAT_MP4;
}
static int _get_muxer_type(output_format_t fmt)
{
	int type = AV_MUXER_TYPE_MOV;

	if (fmt == REC_OUTPUT_FORMAT_RAW)
		type = AV_MUXER_TYPE_RAW;
	else if (fmt == REC_OUTPUT_FORMAT_MP4)
		type = AV_MUXER_TYPE_MOV;
	else if (fmt == REC_OUTPUT_FORMAT_MOV)
		type = AV_MUXER_TYPE_MOV;
	else if (fmt == REC_OUTPUT_FORMAT_TS)
		type = AV_MUXER_TYPE_TS;
	else
		type = AV_MUXER_TYPE_MOV;

	return type;
}

static int _vsink_prepare(video_recorder_t *video_rec)
{
	int ret = 0;
	al_comp_info_t *al_comp = &video_rec->vsink_info.al_comp;

	oscl_param_check(video_rec != NULL, -1, NULL);
	oscl_param_check(video_rec->src_vsink == NULL, -1, NULL);

	ret = OMX_SetParameter(al_comp->cmp_hdl, omx_index_vrender_mode,
			&video_rec->vsink_info.disp_para);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	video_rec->src_vsink = vsrc_camera_getport(video_rec->camera, video_rec->rot);
	oscl_param_check_exit(video_rec->src_vsink != NULL, -1, NULL);
	ret = al_untunnel_setup_ports(video_rec->src_vsink, video_rec->vsink_info.vin);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);
	ret = al_component_setstate(&video_rec->vsink_info.al_comp, OMX_StateIdle);

EXIT:
	return ret;

}

static int _vsink_unprepare(video_recorder_t *video_rec)
{
	int ret = 0;

	oscl_param_check(video_rec != NULL, -1, NULL);
	oscl_param_check(video_rec->src_vsink != NULL, 0, NULL);

	vsrc_camera_disable_port(video_rec->camera, video_rec->src_vsink);
	al_untunnel_unset_ports(video_rec->src_vsink, video_rec->vsink_info.vin);
	vsrc_camera_putport(video_rec->camera, video_rec->src_vsink);
	video_rec->src_vsink = NULL;
	al_component_setstate(&video_rec->vsink_info.al_comp, OMX_StateLoaded);

	return ret;

}

vsrc_camera_t *get_enc_cam(video_recorder_t *video_rec)
{
	OMX_ERRORTYPE ret = -1;
	vsrc_camera_t *tmp_cam = NULL;

	oscl_param_check(video_rec != NULL, NULL, NULL);

	if (video_rec->is_isp_cap == 0)
		goto EXIT;

	if (video_rec->rec_para.source_height == REC_VIDEO_SIZE_720P) {
		OSCL_LOGD("when recording 720p, do not use ISP CAPTURE");
		goto EXIT;
	}


	/* get a video source port */
	tmp_cam = cam_manager_get_device(ISP_CAP_DEV_NAME);
	if (video_rec->userdef_sys_para.camera_buf_num) {
		vsrc_camera_set_buffer_num(tmp_cam,
			video_rec->userdef_sys_para.camera_buf_num);
	} else
		vsrc_camera_set_buffer_num(tmp_cam, CAMERA_CAP_BUF_NUM);
	vsrc_camera_set_para(tmp_cam, &video_rec->rec_para);
	ret = vsrc_camera_prepare(tmp_cam);
	if (ret != OMX_ErrorNone) {
		cam_manager_put_device(tmp_cam);
		tmp_cam = NULL;
		OSCL_LOGE("prepare isp cap device failed, use isp preview");
	} else {
		OSCL_LOGI("use isp capture as video source");
	}
EXIT:
	if (tmp_cam == NULL)
		tmp_cam = video_rec->camera;
	return tmp_cam;
}


void put_enc_cam(video_recorder_t *video_rec, vsrc_camera_t *enc_cam)
{
	if (enc_cam == video_rec->camera)
		return;

	cam_manager_put_device(enc_cam);
	return;
}


static int _venc_prepare(video_recorder_t *video_rec)
{
	OMX_PARAM_PORTDEFINITIONTYPE port_def;
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	venc_info_t *venc_info;
	mux_info_t *mux_info;
	int height, width;
	OMX_U32 cap_offset[4] = {0};
	int i;
	int nmark;
	wm_data_t wm_data;
	int rate = 0;
	numeral_output_t *mark;
	OMX_U32 nb_streams = 1;
	/*int idr_period = 30;*/

	oscl_param_check(video_rec != NULL, -1, NULL);
	oscl_param_check(video_rec->src_venc == NULL, -1, NULL);

	venc_info = &video_rec->venc_info;
	/* get a video source port */
	video_rec->src_venc = vsrc_camera_getport(video_rec->enc_cam, VDISP_ROTATE_NONE);
	oscl_param_check_exit(video_rec->src_venc != NULL, -1, NULL);

	/* get para from camera, and set it to encoder component */
	OSCL_LOGI("%x, %d", video_rec->src_venc, video_rec->src_venc->index);
	vsrc_camera_getpara_portinfo(video_rec->enc_cam, video_rec->src_venc, &port_def);
	height = port_def.format.video.nFrameHeight;
	width = port_def.format.video.nFrameWidth;
	calc_frame_size(port_def.format.video.eColorFormat, width, height,
			&cap_offset[1], port_def.nBufferAlignment);
	OSCL_LOGI("w-h:%d-%d, offset:%d, %d, %d",
		width, height, cap_offset[0], cap_offset[1], cap_offset[2]);
	ret = OMX_SetParameter(venc_info->al_comp.cmp_hdl,
			omx_index_lombo_capture_plan_offsets, cap_offset);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	ret = OMX_SetParameter(venc_info->al_comp.cmp_hdl,
			omx_index_lombo_venc_rect, &video_rec->rec_para.enc_rect);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	/* set encoder component output port para */
	port_def.nPortIndex = venc_info->vout->index;
	ret = OMX_GetParameter(venc_info->al_comp.cmp_hdl,
			OMX_IndexParamPortDefinition, &port_def);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);
	port_def.format.video.nFrameWidth = venc_info->out_width;
	port_def.format.video.nFrameHeight = venc_info->out_height;
	port_def.format.video.nBitrate = venc_info->bitrate;
	port_def.nBufferCountActual = venc_info->vout->nbuffer;
	ret = OMX_SetParameter(venc_info->al_comp.cmp_hdl,
			OMX_IndexParamPortDefinition, &port_def);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	/* connect video source with video encoder */
	ret = al_untunnel_setup_ports(video_rec->src_venc, video_rec->venc_info.vin);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);
	OSCL_LOGI("venc_info->interval = %d\n", venc_info->interval);
	if (venc_info->interval > 0) {
		rate = venc_info->play_framerate / 1000;
		OSCL_LOGE("rate = %d\n", rate);
		ret = OMX_SetParameter(venc_info->al_comp.cmp_hdl,
					omx_index_lombo_venc_framerate,
					(OMX_PTR)&rate);
		oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);
		ret = OMX_SetParameter(venc_info->al_comp.cmp_hdl,
					omx_index_lombo_process_interval,
					(OMX_PTR)&venc_info->interval);
		oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);
		/*set streams number */
		mux_info = &video_rec->mux_info;
		ret = OMX_SetParameter(mux_info->al_comp.cmp_hdl,
					omx_index_lombo_set_muxer_nb_streams,
					(OMX_PTR)&nb_streams);
		oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);
#if 0
		ret = OMX_SetParameter(venc_info->al_comp.cmp_hdl,
					omx_index_lombo_venc_voprefresh,
					(OMX_PTR)&idr_period);
		oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);
#endif
	} else {
		venc_info->interval = 0;
		ret = OMX_SetParameter(venc_info->al_comp.cmp_hdl,
					omx_index_lombo_process_interval,
					(OMX_PTR)&venc_info->interval);
		oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);
		/*set streams number */
		nb_streams = 2;
		mux_info = &video_rec->mux_info;
		ret = OMX_SetParameter(mux_info->al_comp.cmp_hdl,
					omx_index_lombo_set_muxer_nb_streams,
					(OMX_PTR)&nb_streams);
		oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	}
	/* check output filename is set */
	oscl_param_check_exit(video_rec->mux_info.filename != NULL, ret, NULL);
	ret = OMX_SetParameter(video_rec->mux_info.al_comp.cmp_hdl,
			omx_index_vendor_output_filename,
			video_rec->mux_info.filename);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	ret = OMX_SetParameter(video_rec->mux_info.al_comp.cmp_hdl,
			omx_index_lombo_set_muxer_cache_sw,
			&video_rec->rec_para.muxer_cache_flag);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	int type = _get_muxer_type(video_rec->mux_info.fmt);
	ret = OMX_SetParameter(video_rec->mux_info.al_comp.cmp_hdl,
			omx_index_lombo_set_muxer_format, &type);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

#if 0
	ret = OMX_SetParameter(video_rec->mux_info.al_comp.cmp_hdl,
			omx_index_lombo_venc_bitrate, &venc_info->bitrate);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);
	OSCL_LOGE("");
#endif

	/* connect video splitter with video encoder */
	ret = al_untunnel_setup_ports(video_rec->venc_info.vout,
			video_rec->mux_info.vin);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	al_component_setstate(&video_rec->venc_info.al_comp, OMX_StateIdle);

	nmark = watermark_get_markarray(video_rec->watermark_private, &mark);
	if (nmark == 0 || mark == NULL)
		goto EXIT;
	OSCL_LOGI("set watermark to encoder!");
	for (i = 0; i < nmark; i++) {
		if (mark[i].numeral_picture.picture_size != 0) {
			memset(&wm_data, 0, sizeof(wm_data));
			wm_data.wm_pic.blending_width = mark[i].numeral_picture.width;
			wm_data.wm_pic.blending_height = mark[i].numeral_picture.height;
			wm_data.wm_pic.blending_stride = mark[i].numeral_picture.stride;
			wm_data.wm_pic.blending_x_pos = mark[i].start_x_pos;
			wm_data.wm_pic.blending_y_pos = mark[i].start_y_pos;
			wm_data.wm_pic.vir_addr = mark[i].numeral_picture.data;
			wm_data.wm_pic.phy_addr = mark[i].numeral_picture.phy_addr;
			wm_data.wm_pic.colorspace = mark[i].colorspace;
			wm_data.index = mark[i].blending_area_index;
			OMX_SetParameter(video_rec->venc_info.al_comp.cmp_hdl,
				omx_index_lombo_blending_picture_indexs,
				&wm_data);
		} else
			OMX_SetParameter(video_rec->venc_info.al_comp.cmp_hdl,
				omx_index_lombo_disable_blending_picture,
				(OMX_PTR)i);
	}
	watermark_put_markarray(video_rec->watermark_private, mark);
EXIT:
	return ret;

}

static void _set_prio(video_recorder_t *video_rec)
{
	OMX_PRIORITYMGMTTYPE priority;

	astream_set_prio(video_rec->audio, LB_RECORDER_AUDIO_PRIO);
	priority.nVersion.nVersion = OMX_VERSION;
	priority.nGroupPriority = LB_RECORDER_MUXER_PRIO;
	OMX_SetParameter(video_rec->mux_info.al_comp.cmp_hdl,
		OMX_IndexParamPriorityMgmt, &priority);

	priority.nGroupPriority = LB_RECORDER_VIDEO_PRIO;
	OMX_SetParameter(video_rec->venc_info.al_comp.cmp_hdl,
		OMX_IndexParamPriorityMgmt, &priority);

	priority.nGroupPriority = LB_RECORDER_SINK_PRIO;
	OMX_SetParameter(video_rec->vsink_info.al_comp.cmp_hdl,
		OMX_IndexParamPriorityMgmt, &priority);

}


int video_rec_prepare(video_recorder_t *video_rec)
{
	int ret = 0;
	OMX_S32 frame_rate;
	rec_param_t rec_para;

	OSCL_TRACE("prepare start");
	oscl_param_check_exit(video_rec != 0, -1, NULL);

	memcpy(&rec_para, &video_rec->rec_para, sizeof(rec_param_t));

	/* if device is isp and app want to encoding 1080P pixels, use isp_cap device */
	if (video_rec->is_isp_cap && rec_para.source_height != REC_VIDEO_SIZE_720P) {
		/* when use isp_cap, isp_preview use default settings */
		OSCL_LOGD("when use isp_cap, isp_preview use default settings");
		frame_rate = rec_para.frame_rate;
		memset(&rec_para, 0, sizeof(rec_param_t));
		rec_para.frame_rate = frame_rate;
	}
	vsrc_camera_set_para(video_rec->camera, &rec_para);

	/* setup para for recorder components*/
	vsrc_camera_prepare(video_rec->camera);
	_set_prio(video_rec);
	video_rec->state = REC_STATE_PREPARED;
EXIT:
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("video_rec_prepare error! ret:%x", ret);
		video_rec->state = REC_STATE_ERR;
	}
	OSCL_TRACE("prepare end! ret:%x", ret);
	OSCL_TRACE("video_rec->state:%x", video_rec->state);
	return ret;
}

/**
 * video_pause_preview - pause preview.
 *
 * @video_rec: video recorder handle
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int video_rec_pause_preview(video_recorder_t *video_rec)
{
	int ret = 0;

	if (video_rec->src_vsink &&
		video_rec->vsink_info.al_comp.state == OMX_StateExecuting) {
		ret = al_component_setstate(
			&video_rec->vsink_info.al_comp, OMX_StatePause);
	}

	return ret;
}

/**
 * video_continue_preview - continue preview.
 *
 * @video_rec: video recorder handle
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int video_rec_continue_preview(video_recorder_t *video_rec)
{
	int ret = 0;

	if (video_rec->src_vsink &&
		video_rec->vsink_info.al_comp.state == OMX_StatePause) {
		ret = al_component_setstate(
			&video_rec->vsink_info.al_comp, OMX_StateExecuting);
	}

	return ret;
}
/**
 * video_rec_preview - start preview.
 *
 * @video_rec: video recorder handle
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int video_rec_preview(video_recorder_t *video_rec)
{
	int ret = 0;

	OSCL_TRACE("start");
	/* current state is preview, return with no err */
	if (video_rec->src_vsink) {
		OSCL_LOGW("preview is enabled, do nothing");
		goto EXIT;
	}
	vsrc_camera_prepare(video_rec->camera);
	ret = _vsink_prepare(video_rec);
	oscl_param_check_exit(ret == 0, ret, NULL);

	al_component_setstate(&video_rec->vsink_info.al_comp, OMX_StateExecuting);
	ret = vsrc_camera_enable_port(video_rec->camera, video_rec->src_vsink);

	video_rec->prev_en = 1;
EXIT:
	OSCL_TRACE("end! ret:%x", ret);
	OSCL_TRACE("video_rec->state:%x", video_rec->state);
	return ret;
}

/**
 * video_rec_stop_preview - stop preview.
 *
 * @video_rec: video recorder handle
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int video_rec_stop_preview(video_recorder_t *video_rec)
{
	int ret = 0;

	OSCL_TRACE("start");

	/* preview is already stopped, return failed */
	if (video_rec->src_vsink == NULL) {
		OSCL_LOGW("preview is stop, do nothing");
		goto EXIT;
	}

	/* change to idle state for recorder components*/
	OSCL_LOGW("===");
	al_component_setstate(&video_rec->vsink_info.al_comp, OMX_StateIdle);
	_vsink_unprepare(video_rec);

	video_rec->prev_en = 0;
EXIT:
	OSCL_TRACE("end! ret:%x", ret);
	OSCL_TRACE("video_rec->state:%x", video_rec->state);
	return ret;
}

/**
 * video_rec_set_fix_duration_param - set fix duration param.
 *
 * @video_rec: video recorder handle
 *
 * Returns 0 on success, -EERROR otherwise..
 */


int video_rec_set_fix_duration_param(video_recorder_t *video_rec,
	void *hdl, fix_duration_param_t *param)
{
	al_muxer_private_t *muxer_private;

	oscl_param_check((video_rec != NULL), -1, NULL);
	oscl_param_check((param != NULL), -1, NULL);

	muxer_private = video_rec->mux_info.al_comp.priv_data;
	if (video_rec->mux_info.al_comp.priv_data == NULL) {
		muxer_private = oscl_zalloc(sizeof(al_muxer_private_t));
		video_rec->mux_info.al_comp.priv_data = muxer_private;
	}
	oscl_param_check((muxer_private != NULL), -1, NULL);
	memcpy(&muxer_private->fix_duration_param, param, sizeof(fix_duration_param_t));
	muxer_private->rec_handle = hdl;

	OMX_SetParameter(video_rec->mux_info.al_comp.cmp_hdl,
		omx_index_vendor_set_max_duration,
		&(muxer_private->fix_duration_param.file_duration));

	return 0;
}



/**
 * video_rec_stop_preview - stop preview.
 *
 * @video_rec: video recorder handle
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int video_rec_set_frame_cb(video_recorder_t *video_rec, app_frame_cb_t *cb)
{
	int ret = 0;

	OSCL_TRACE("start");
	if (video_rec->src_vcb && video_rec->src_vcb->cb.buf_handle != NULL) {
		vsrc_camera_disable_port(video_rec->camera, video_rec->src_vcb);
		al_untunnel_unset_cb(video_rec->src_vcb);
		video_rec->src_vcb->cb.buf_handle = NULL;
		video_rec->src_vcb->cb.app_data = NULL;
		vsrc_camera_putport(video_rec->camera, video_rec->src_vcb);
		video_rec->src_vcb = NULL;
	}
	if (cb->buf_handle) {
		video_rec->src_vcb = vsrc_camera_getport(video_rec->camera,
							    VDISP_ROTATE_NONE);
		oscl_param_check_exit(video_rec->src_vcb, -1, NULL);
		al_untunnel_setup_cb(video_rec->src_vcb, cb);
		ret = vsrc_camera_enable_port(video_rec->camera, video_rec->src_vcb);
	}

EXIT:
	return ret;
}

/**
 * video_rec_stop_preview - stop preview.
 *
 * @video_rec: video recorder handle
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int video_rec_reset(video_recorder_t *video_rec)
{
	int ret = 0;

	OSCL_TRACE("start");
	/* current state is record, stop recorder first */
	if (video_rec->state == REC_STATE_RECORD)
		video_rec_stop(video_rec);
	if (video_rec->prev_en)
		video_rec_stop_preview(video_rec);

#ifdef ENCODE_AUDIO
	if (video_rec->audio_en) {
		astream_set_muxer(video_rec->audio, NULL);
		astream_reset(video_rec->audio);
	}
#endif

	/* change to load state for recorder components*/
	al_component_setstate(&video_rec->mux_info.al_comp, OMX_StateLoaded);
	al_component_setstate(&video_rec->venc_info.al_comp, OMX_StateLoaded);

	video_rec->state = REC_STATE_INIT;
	return ret;
}

void _avsync_before_start(video_recorder_t *video_rec)
{
	int nretry = 500;
	OMX_TIME_CONFIG_TIMESTAMPTYPE start_time;

	if (video_rec->audio_en && video_rec->video_en) {
		while (nretry--) {
			video_rec->aref_time = astream_get_time(video_rec->audio);
			video_rec->vref_time = vsrc_camera_get_time(video_rec->enc_cam);
			if (video_rec->aref_time >= 0 && video_rec->vref_time >= 0)
				break;
			if (nretry%10 == 0)
				OSCL_LOGE("get reftime fail, retry!");
			oscl_mdelay(20);
		}
	}
	OSCL_LOGI("get reftime:%lld, %lld", video_rec->aref_time, video_rec->vref_time);
	if (video_rec->aref_time < 0)
		video_rec->aref_time = 0;
	if (video_rec->vref_time < 0)
		video_rec->vref_time = 0;
	start_time.nPortIndex = video_rec->mux_info.ain->index;
	start_time.nTimestamp = video_rec->aref_time;
	OMX_SetConfig(video_rec->mux_info.al_comp.cmp_hdl,
		omx_index_lombo_config_cur_time, &start_time);

	start_time.nPortIndex = video_rec->venc_info.vin->index;
	start_time.nTimestamp = video_rec->vref_time;
	OMX_SetConfig(video_rec->venc_info.al_comp.cmp_hdl,
		omx_index_lombo_config_cur_time, &start_time);
}

/**
 * video_rec_start - start recording.
 *
 * @video_rec: video recorder handle
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int video_rec_start(video_recorder_t *video_rec)
{
	int ret = 0;

	OSCL_TRACE("start");
	OSCL_LOGI("video_rec->state:%d", video_rec->state);

#ifdef ENCODE_VIDEO
	oscl_param_check(video_rec->src_venc == NULL, -1, NULL);
	video_rec->enc_cam = get_enc_cam(video_rec);
	oscl_param_check_exit(video_rec->enc_cam != NULL, ret, NULL);

	ret = _venc_prepare(video_rec);
	oscl_param_check_exit(ret == 0, ret, NULL);
#endif
	if (video_rec->type ==  RECORDER_TYPE_TIME_LAG) {
		/*astream_release(video_rec->audio);*/
		video_rec->audio_en = 0;
	}
#ifdef ENCODE_AUDIO
	if (video_rec->audio_en) {
		astream_set_muxer(video_rec->audio, &video_rec->mux_info);
		ret = astream_prepare(video_rec->audio);
		oscl_param_check_exit(ret == 0, -1, NULL);
	}
#endif

	/* change to idle state for recorder components.
	 * muxer must set to idle after video/audio encoder prepared,
	 * becase of set port definition must be in load state */
	al_component_setstate(&video_rec->mux_info.al_comp, OMX_StateIdle);

	/* excute encoder and mux compnent */
	al_component_setstate(&video_rec->mux_info.al_comp, OMX_StateExecuting);
#ifdef ENCODE_VIDEO
	al_component_setstate(&video_rec->venc_info.al_comp, OMX_StateExecuting);
	/* enable encoder port of video splitter */
	ret = vsrc_camera_enable_port(video_rec->enc_cam, video_rec->src_venc);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);
#endif

#ifdef ENCODE_AUDIO
	if (video_rec->audio_en) {
		ret = astream_start(video_rec->audio);
		oscl_param_check_exit(ret == 0, -1, NULL);
	}
#endif
	_avsync_before_start(video_rec);

	video_rec->state = REC_STATE_RECORD;
EXIT:
	if (ret != OMX_ErrorNone)
		video_rec->state = REC_STATE_ERR;
	OSCL_TRACE("video_rec->state:%x", video_rec->state);
	OSCL_TRACE("end! ret:%x", ret);
	return ret;
}


/**
 * video_rec_stop - stop recording.
 *
 * @video_rec: video recorder handle
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int video_rec_stop(video_recorder_t *video_rec)
{
	int ret = 0;

	oscl_param_check_exit((video_rec != NULL), 0, NULL);

	OSCL_TRACE("start video_rec->state:%x", video_rec->state);
	oscl_param_check_exit((video_rec->state == REC_STATE_RECORD), 0, NULL);

	vsrc_camera_disable_port(video_rec->enc_cam, video_rec->src_venc);
#ifdef ENCODE_AUDIO
	if (video_rec->audio_en)
		ret = astream_stop(video_rec->audio);
#endif

#ifdef ENCODE_VIDEO
	al_component_setstate(&video_rec->venc_info.al_comp, OMX_StateIdle);
#endif
	/* stop encoder and mux compnent */
	al_component_setstate(&video_rec->mux_info.al_comp, OMX_StateIdle);

#ifdef ENCODE_VIDEO
	/* disconnect video splitter with video encoder */
	al_untunnel_unset_ports(video_rec->src_venc, video_rec->venc_info.vin);
	/* disconnect video splitter with video encoder */
	al_untunnel_unset_ports(video_rec->venc_info.vout, video_rec->mux_info.vin);
	vsrc_camera_putport(video_rec->enc_cam, video_rec->src_venc);
	al_component_setstate(&video_rec->venc_info.al_comp, OMX_StateLoaded);
	video_rec->src_venc = NULL;
#endif
	/* stop encoder and mux compnent */
	al_component_setstate(&video_rec->mux_info.al_comp, OMX_StateLoaded);
	watermark_set_mark(video_rec->watermark_private, NULL);
	put_enc_cam(video_rec, video_rec->enc_cam);
	video_rec->enc_cam = NULL;

	video_rec->state = REC_STATE_PREPARED;
EXIT:
	if (ret != OMX_ErrorNone)
		video_rec->state = REC_STATE_ERR;
	OSCL_TRACE("video_rec->state:%x", video_rec->state);
	OSCL_TRACE("end! ret:%x", ret);
	return ret;
}
/* level>> 0: top 1: bottom */
int video_set_layer_level(video_recorder_t *video_rec, video_layer_level_t level)
{
	int ret = 0;
	al_comp_info_t *al_comp = NULL;

	oscl_param_check((NULL != video_rec), -1, NULL);
	al_comp = &video_rec->vsink_info.al_comp;
	OSCL_LOGI("Set video layer level:%x", level);

	if (VIDEO_LAYER_TOP == level)
		ret = OMX_SetParameter(al_comp->cmp_hdl, omx_index_vrender_win_top,
					NULL);
	else
		ret = OMX_SetParameter(al_comp->cmp_hdl, omx_index_vrender_win_bottom,
					NULL);
	if (ret != OMX_ErrorNone)
		video_rec->state = REC_STATE_ERR;

	return ret;
}

int video_set_disp_para(video_recorder_t *video_rec, win_para_t *disp_para)
{
	int ret = 0;
	al_comp_info_t *al_comp = NULL;
	vdisp_mode_e mode = VDISP_WINDOW_FULL_SCREEN_VIDEO_RATIO;

	OSCL_TRACE("%x %x", video_rec, disp_para);
	oscl_param_check((NULL != video_rec) && (NULL != disp_para), -1, NULL);
	al_comp = &video_rec->vsink_info.al_comp;

	if (disp_para->mode > VIDEO_WINDOW_USERDEF) {
		OSCL_LOGE("Display mode set error.");
		return OMX_ErrorBadParameter;
	}
	if (disp_para->mode == VIDEO_WINDOW_ORIGINAL)
		mode = VDISP_WINDOW_ORIGINAL;
	else if (disp_para->mode == VIDEO_WINDOW_FULL_SCREEN_VIDEO_RATIO)
		mode = VDISP_WINDOW_FULL_SCREEN_VIDEO_RATIO;
	else if (disp_para->mode == VIDEO_WINDOW_FULL_SCREEN_SCREEN_RATIO)
		mode = VDISP_WINDOW_FULL_SCREEN_SCREEN_RATIO;
	else if (disp_para->mode == VIDEO_WINDOW_4R3MODE)
		mode = VDISP_WINDOW_4R3MODE;
	else if (disp_para->mode == VIDEO_WINDOW_16R9MODE)
		mode = VDISP_WINDOW_16R9MODE;
	else if (disp_para->mode == VIDEO_WINDOW_CUTEDGE)
		mode = VDISP_WINDOW_CUTEDGE;
	else if (disp_para->mode == VIDEO_WINDOW_USERDEF)
		mode = VDISP_WINDOW_USERDEF;
	else
		mode = VDISP_WINDOW_FULL_SCREEN_VIDEO_RATIO;
	memset(&video_rec->vsink_info.disp_para, 0, sizeof(vdisp_para_t));
	video_rec->vsink_info.disp_para.mode = mode;
	video_rec->vsink_info.disp_para.win_rect.top = disp_para->rect.y;
	video_rec->vsink_info.disp_para.win_rect.left = disp_para->rect.x;
	video_rec->vsink_info.disp_para.win_rect.width = disp_para->rect.width;
	video_rec->vsink_info.disp_para.win_rect.height = disp_para->rect.height;

	if ((disp_para->crop.width > 0) && (disp_para->crop.height > 0)) {
		video_rec->vsink_info.disp_para.crop_rect.top = disp_para->crop.y;
		video_rec->vsink_info.disp_para.crop_rect.left = disp_para->crop.x;
		video_rec->vsink_info.disp_para.crop_rect.width = disp_para->crop.width;
		video_rec->vsink_info.disp_para.crop_rect.height = disp_para->crop.height;
	}
	ret = OMX_SetParameter(al_comp->cmp_hdl, omx_index_vrender_mode,
				&video_rec->vsink_info.disp_para);
	return ret;
}



/**
 * video_rec_set_output_file - set output file.
 *
 * @video_rec: video recorder handle
 * @filename: output filename
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int video_rec_set_output_file(video_recorder_t *video_rec, char *filename)
{
	int ret = 0;

	OSCL_TRACE("start");
	OSCL_TRACE("filename:%s", filename);

	if (video_rec->mux_info.filename != NULL)
		oscl_free(video_rec->mux_info.filename);
	OSCL_TRACE("");
	video_rec->mux_info.filename = oscl_strdup(filename);
	OSCL_TRACE("");
	oscl_param_check_exit(video_rec->mux_info.filename != NULL,
		OMX_ErrorInsufficientResources, NULL);

EXIT:
	OSCL_TRACE("end! ret:%x", ret);
	return ret;
}

/**
 * video_rec_set_output_format - set output format.
 *
 * @video_rec: video recorder handle
 * @fmt: output format
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int video_rec_set_output_format(video_recorder_t *video_rec, output_format_t fmt)
{
	OSCL_TRACE("start");

	oscl_param_check((video_rec != NULL), -1, NULL);
	oscl_param_check((video_rec->state == REC_STATE_INIT), -1, NULL);

	if (fmt == REC_OUTPUT_FORMAT_MP4)
		video_rec->mux_info.fmt = REC_OUTPUT_FORMAT_MP4;
	if (fmt == REC_OUTPUT_FORMAT_MOV)
		video_rec->mux_info.fmt = REC_OUTPUT_FORMAT_MOV;
	if (fmt == REC_OUTPUT_FORMAT_TS)
		video_rec->mux_info.fmt = REC_OUTPUT_FORMAT_TS;

	OSCL_TRACE("end!");
	return 0;
}

/**
 * video_rec_set_rotate - set preview rotate mode.
 *
 * @video_rec: video recorder handle
 * @fmt: output format
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int video_rec_set_rotate(video_recorder_t *video_rec, int rotation)
{
	OSCL_TRACE("start");

	oscl_param_check((video_rec != NULL), -1, NULL);
	oscl_param_check((video_rec->state == REC_STATE_INIT), -1, NULL);
	video_rec->rot = rotation;
	OSCL_TRACE("end!");
	return 0;
}
/**
 * video_rec_set_user_def_sys_cfg - set system config of user define .
 *
 * @video_rec: video recorder handle
 * @fmt: output format
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int video_rec_set_user_def_sys_cfg(video_recorder_t *video_rec, void *p)
{
	user_def_sys_cfg_t *puser_sys_cfg = (user_def_sys_cfg_t *)p;
	OSCL_TRACE("start");

	oscl_param_check((video_rec != NULL), -1, NULL);
	oscl_param_check((video_rec->state == REC_STATE_INIT), -1, NULL);
	video_rec->userdef_sys_para.camera_buf_num = puser_sys_cfg->camera_buf_num;
	OSCL_TRACE("end!");
	return 0;
}

/**
 * video_rec_set_video_sink - set display device.
 *
 * @video_rec: video recorder handle
 * @dev: display device
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int video_rec_set_video_sink(video_recorder_t *video_rec, char *dev)
{
	OSCL_TRACE("start");

	oscl_param_check((video_rec != NULL && dev != NULL), -1, NULL);
	oscl_param_check((video_rec->state == REC_STATE_INIT), -1, NULL);
	if (video_rec->vsink_info.dev_name) {
		OSCL_LOGW("switch video sink:%s to %s",
			video_rec->vsink_info.dev_name, dev);
		oscl_free(video_rec->vsink_info.dev_name);
	}
	video_rec->vsink_info.dev_name = oscl_strdup(dev);
	oscl_param_check(video_rec->vsink_info.dev_name != NULL,
		OMX_ErrorInsufficientResources, NULL);
	OSCL_TRACE("end!");
	return 0;
}

/**
 * video_rec_set_video_source - set source device of recorder.
 *
 * @video_rec: video recorder handle
 * @src_name: device name
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int video_rec_set_video_source(video_recorder_t *video_rec, char *src_name)
{
	int ret = -1;
	oscl_param_check(((video_rec != NULL) && (src_name != NULL)), -1, NULL);
	oscl_param_check((video_rec->state == REC_STATE_INIT), -1, NULL);

	video_rec->is_isp_cap = 0;
	if (strcmp(src_name, ISP_DEV_NAME) == 0) {
		video_rec->is_isp_cap = 1;
		src_name = ISP_PREV_DEV_NAME;
	}
	if (video_rec->camera)
		cam_manager_put_device(video_rec->camera);

	video_rec->camera = cam_manager_get_device(src_name);
	if (video_rec->userdef_sys_para.camera_buf_num) {
		vsrc_camera_set_buffer_num(video_rec->camera,
			video_rec->userdef_sys_para.camera_buf_num);
	} else
		vsrc_camera_set_buffer_num(video_rec->camera, CAMERA_CAP_BUF_NUM);
	if (video_rec->camera != NULL)
		ret = 0;
	return ret;
}

int video_rec_set_timelag_para(video_recorder_t *video_rec,
	rec_time_lag_para_t *para)
{

	oscl_param_check((video_rec != NULL), -1, NULL);
	oscl_param_check((para != NULL), -1, NULL);

	/*if (para->interval)*/
	video_rec->venc_info.interval = para->interval;
	/*if (para->play_framerate)*/
	video_rec->venc_info.play_framerate = para->play_framerate;

	return OMX_ErrorNone;
}
int video_rec_set_mode(video_recorder_t *video_rec, int mode)
{
	oscl_param_check((video_rec != NULL), -1, NULL);
	oscl_param_check((mode < 2), -1, NULL);

	video_rec->type = mode;

	return OMX_ErrorNone;
}

/**
 * video_rec_set_para - set recording para.
 *
 * @video_rec: video recorder handle
 * @fmt: output format
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int video_rec_set_para(video_recorder_t *video_rec, rec_param_t *rec_para)
{
	OSCL_TRACE("start");
	OMX_AUDIO_CODINGTYPE coding_type;
	win_rect_t *area;

	oscl_param_check((video_rec != NULL), -1, NULL);
	oscl_param_check((video_rec->state == REC_STATE_INIT), -1, NULL);

	memcpy(&video_rec->rec_para, rec_para, sizeof(rec_param_t));

	area = &video_rec->rec_para.enc_rect;
#if 0	
	if (((area->x % 16) != 0) || ((area->y % 16) != 0) ||
		((area->width % 16) != 0) || ((area->height % 16) != 0)) {
		area->x = __ALIGN(area->x - 15, 16);
		area->y = __ALIGN(area->y - 15, 16);
		area->width = __ALIGN(area->width, 16);
		area->height = __ALIGN(area->height, 16);
		OSCL_LOGE("warning! enc rec x/y/width/height is not 16X");
	}
#endif
	video_rec->rot = rec_para->rotate;

	if (video_rec->video_en) {
		if (rec_para->height)
			video_rec->venc_info.out_height = rec_para->height;
		if (rec_para->width)
			video_rec->venc_info.out_width = rec_para->width;
		if (rec_para->bitrate)
			video_rec->venc_info.bitrate = rec_para->bitrate;
		if (rec_para->file_fmt)
			video_rec_set_output_format(video_rec, rec_para->file_fmt);
#if 0
		if (rec_para->interval)
			video_rec->venc_info.interval = rec_para->interval;
		if (rec_para->play_framerate)
			video_rec->venc_info.play_framerate = rec_para->play_framerate;
#endif
	}
	if (video_rec->audio_en) {
		if (rec_para->audio_channels)
			astream_set_channels(video_rec->audio, rec_para->audio_channels);
		if (rec_para->audio_sample_width)
			astream_set_sample_width(video_rec->audio,
						rec_para->audio_sample_width);
		if (rec_para->audio_sample_rate)
			astream_set_sample_rate(video_rec->audio,
						rec_para->audio_sample_rate);

		switch (rec_para->aenc_format) {
		case AUDIO_ENCODER_AAC:
			coding_type = OMX_AUDIO_CodingAAC;
			break;
		case AUDIO_ENCODER_ADPCM:
			coding_type = OMX_AUDIO_CodingADPCM;
			break;
		case AUDIO_ENCODER_PCM:
			coding_type = OMX_AUDIO_CodingPCM;
			break;
		default:
			coding_type = OMX_AUDIO_CodingUnused;
			break;
		}
		if (coding_type != OMX_AUDIO_CodingUnused)
			astream_set_coding_type(video_rec->audio, coding_type);
	}

	OSCL_TRACE("end!");
	return 0;
}

/**
 * video_rec_get_para - set current para of recorder.
 *
 * @video_rec: video recorder handle
 * @fmt: output format
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int video_rec_get_para(video_recorder_t *video_rec, rec_param_t *rec_para)
{
	OSCL_TRACE("start");
	OMX_AUDIO_CODINGTYPE coding_type;

	oscl_param_check((video_rec != NULL), -1, NULL);
	oscl_param_check((rec_para != NULL), -1, NULL);

	memset(rec_para, 0x00, sizeof(rec_param_t));
	vsrc_camera_get_para(video_rec->camera, rec_para);
	rec_para->rotate = video_rec->rot;

	rec_para->height = video_rec->venc_info.out_height;
	rec_para->width = video_rec->venc_info.out_width;
	rec_para->bitrate = video_rec->venc_info.bitrate;
	rec_para->file_fmt = video_rec->mux_info.fmt;

	rec_para->audio_sample_rate = astream_get_sample_rate(video_rec->audio);
	rec_para->audio_sample_width = astream_get_sample_width(video_rec->audio);
	rec_para->audio_channels = astream_get_channels(video_rec->audio);
	rec_para->muxer_cache_flag = RECORDER_CACHE_NOT_USE;
	coding_type = astream_get_coding_type(video_rec->audio);

	switch (coding_type) {
	case OMX_AUDIO_CodingAAC:
		rec_para->aenc_format = AUDIO_ENCODER_AAC;
		break;
	case OMX_AUDIO_CodingADPCM:
		rec_para->aenc_format = AUDIO_ENCODER_ADPCM;
		break;
	case OMX_AUDIO_CodingPCM:
		rec_para->aenc_format = AUDIO_ENCODER_PCM;
		break;
	default:
		rec_para->aenc_format = AUDIO_ENCODER_NULL;
		break;
	}

	OSCL_TRACE("end!");
	return 0;
}


/**
 * video_rec_get_time - get current recoder time.
 *
 * @video_rec: video recorder handle
 * @time: output time in second. If recorder is not in recording state, *time will be 0;
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int video_rec_get_time(video_recorder_t *video_rec, int *time)
{
	int ret = 0;
	OMX_TIME_CONFIG_TIMESTAMPTYPE cur_time;

	oscl_param_check((video_rec != NULL), -1, NULL);
	oscl_param_check((time != NULL), -1, NULL);

	*time = 0;
	cur_time.nPortIndex = video_rec->mux_info.vin->index;
	ret = OMX_GetConfig(video_rec->mux_info.al_comp.cmp_hdl,
			    omx_index_lombo_config_cur_time, &cur_time);
	if (ret == 0) {
		if (video_rec->type == RECORDER_TYPE_TIME_LAG) {
			if (!video_rec->venc_info.play_framerate) {
				OSCL_LOGE("play framerate is zero,it.s fault");
				return -1;
			}
			*time = (cur_time.nTimestamp *
				(video_rec->venc_info.play_framerate / 1000)) / 1000;
		} else
			*time = cur_time.nTimestamp/1000;
	}

	return 0;
}

/**
 * video_rec_get_stat - set current status of recorder.
 *
 * @video_rec: video recorder handle
 * @fmt: output format
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int video_rec_get_status(video_recorder_t *video_rec, int *status)
{
	oscl_param_check((video_rec != NULL), -1, NULL);
	oscl_param_check((status != NULL), -1, NULL);

	switch (video_rec->state) {
	case REC_STATE_INIT:
		*status = RECORDER_STATE_INIT;
		break;
	case REC_STATE_PREPARED:
		*status = RECORDER_STATE_PREPARED;
		break;
	case REC_STATE_RECORD:
		*status = RECORDER_STATE_RECORD;
		break;
	default:
		*status = RECORDER_STATE_ERR;
		break;
	}
	return 0;
}

/**
 * video_rec_set_mute - enable/disable mute.
 *
 * @video_rec: video recorder handle
 * @mute: 1 for enable and 0 for disable mute
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int video_rec_set_mute(video_recorder_t *video_rec, int mute)
{
	int ret;
	OSCL_TRACE("start");

	oscl_param_check((video_rec != NULL), -1, NULL);
	ret = astream_set_mute(video_rec->audio, mute);
	OSCL_TRACE("end!");

	return ret;
}

/**
 * video_rec_wm_set_bnp - set watermark source pic.
 *
 * @video_rec: video recorder handle
 * @bnp: source picture struct
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int video_rec_wm_set_bnp(video_recorder_t *video_rec, numeral_input_t *bnp)
{
	int ret;
	oscl_param_check((video_rec != NULL), -1, NULL);
	oscl_param_check((bnp != NULL), -1, NULL);

	ret = watermark_set_bnp(video_rec->watermark_private, bnp);
	return ret;
}

/**
 * video_rec_wm_set_mark - set watermark watermark pic index.
 *
 * @video_rec: video recorder handle
 * @mk: watermark
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int video_rec_wm_set_mark(video_recorder_t *video_rec, numeral_picture_index_t *mk)
{
	int ret = 0;
	numeral_output_t *watermark;
	int index;
	wm_data_t wm_data;
	oscl_param_check((video_rec != NULL), -1, NULL);

	watermark = watermark_set_mark(video_rec->watermark_private, mk);
	/* if not encoding, just save in watermark and exit. watermark will set to
	 * encoder befor ecoder change to state executing*/
	if (video_rec->venc_info.al_comp.state != OMX_StateExecuting)
		goto EXIT;

	/* set to encoder */
	index = MAX_WATERMARK_NUM;
	if (mk == NULL)
		goto EXIT;
	else if (mk->total_index_num == 0 || watermark == NULL)
		index = mk->blending_area_index;
	if (index != MAX_WATERMARK_NUM)
		ret = OMX_SetParameter(video_rec->venc_info.al_comp.cmp_hdl,
				omx_index_lombo_disable_blending_picture, (OMX_PTR)index);
	else {
		memset(&wm_data, 0, sizeof(wm_data));
		wm_data.wm_pic.blending_width = watermark->numeral_picture.width;
		wm_data.wm_pic.blending_height = watermark->numeral_picture.height;
		wm_data.wm_pic.blending_stride = watermark->numeral_picture.stride;
		wm_data.wm_pic.blending_x_pos = watermark->start_x_pos;
		wm_data.wm_pic.blending_y_pos = watermark->start_y_pos;
		wm_data.wm_pic.vir_addr = watermark->numeral_picture.data;
		wm_data.wm_pic.phy_addr = watermark->numeral_picture.phy_addr;
		wm_data.wm_pic.colorspace = watermark->colorspace;
		wm_data.index = watermark->blending_area_index;
		ret = OMX_SetParameter(video_rec->venc_info.al_comp.cmp_hdl,
				omx_index_lombo_blending_picture_indexs,
				&wm_data);
	}
EXIT:
	return ret;
}

/**
 * video_rec_creat - get a video_rec handle.
 * This function will load all components video_rec needs.
 *
 * Returns video_rec handle on success, NULL otherwise..
 */
void *video_rec_creat(void)
{
	video_recorder_t *video_rec;
	int index;
	int ret;

	OSCL_TRACE("creat start");
	video_rec = oscl_zalloc(sizeof(video_recorder_t));
	oscl_param_check_exit(video_rec != NULL, -1, NULL);
	/*video_rec->type = type;*/
	video_rec->video_en = 1;
	video_rec->audio_en = 1;

	/* init video sink component */
	ret = al_component_init(&video_rec->vsink_info.al_comp, "OMX.LB.SINK.VRENDER",
			&al_untunnel_common_callbacks);
	oscl_param_check_exit(ret == 0, ret, NULL);
	index = al_get_port_index(&video_rec->vsink_info.al_comp, OMX_DirInput,
			OMX_PortDomainVideo);
	oscl_param_check_exit(index >= 0, -1, NULL);
	video_rec->vsink_info.vin = &video_rec->vsink_info.al_comp.port_info[index];

	/* init video encoder component */
	ret = al_component_init(&video_rec->venc_info.al_comp,
			"OMX.LB.VIDEO.ENCODECOMPONENT",
			&al_untunnel_common_callbacks);
	oscl_param_check_exit(ret == 0, ret, NULL);
	index = al_get_port_index(&video_rec->venc_info.al_comp, OMX_DirInput,
			OMX_PortDomainVideo);
	oscl_param_check_exit(index >= 0, -1, NULL);
	video_rec->venc_info.vin = &video_rec->venc_info.al_comp.port_info[index];
	index = al_get_port_index(&video_rec->venc_info.al_comp, OMX_DirOutput,
			OMX_PortDomainVideo);
	oscl_param_check_exit(index >= 0, -1, NULL);
	video_rec->venc_info.vout = &video_rec->venc_info.al_comp.port_info[index];

	/* init video mux component */
	ret = al_component_init(&video_rec->mux_info.al_comp, "OMX.LB.SINK.MUXER",
			&al_untunnel_common_callbacks);
	oscl_param_check_exit(ret == 0, ret, NULL);

	video_rec->mux_info.vin = &video_rec->mux_info.al_comp.port_info[0];
	index = al_get_port_index(&video_rec->mux_info.al_comp, OMX_DirInput,
			OMX_PortDomainVideo);
	if (index >= 0)
		video_rec->mux_info.vin = &video_rec->mux_info.al_comp.port_info[index];
	index = al_get_port_index(&video_rec->mux_info.al_comp, OMX_DirInput,
			OMX_PortDomainAudio);
	if (index >= 0)
		video_rec->mux_info.ain = &video_rec->mux_info.al_comp.port_info[index];

	/* git initial config for components */
	video_rec->rot = VDISP_ROTATE_90;
	_get_config_venc(&video_rec->venc_info);
	_get_config_vsink(&video_rec->vsink_info);
	_get_config_mux(&video_rec->mux_info);
	take_pic_init(video_rec);
	video_rec->watermark_private = watermark_init();

#ifdef ENCODE_AUDIO
	if (video_rec->audio_en) {
		video_rec->audio = astream_creat();
		oscl_param_check_exit(video_rec->audio != 0, -1, NULL);
	}
#endif
	video_rec->state = REC_STATE_INIT;
EXIT:
	if (ret != 0) {
		video_rec_release(video_rec);
		video_rec = NULL;
	}
	OSCL_TRACE("recorder hdl:%x", video_rec);
	return video_rec;
}

/**
 * video_rec_release - release resource recorder used.
 *
 * @hdl: recorder handle
 *
 * Returns 0 on success, -EERROR otherwise..
 */
void video_rec_release(void *hdl)
{
	video_recorder_t *video_rec;
	OSCL_TRACE("hdl:%x", hdl);
	video_rec = (video_recorder_t *)hdl;

	if (video_rec == NULL)
		return;
	OSCL_LOGI("");
	take_pic_deinit(video_rec);
	video_rec_reset(hdl);
	cam_manager_put_device(video_rec->camera);

#ifdef ENCODE_AUDIO
	if (video_rec->audio_en) {
		astream_release(video_rec->audio);
		OSCL_LOGI("");
		video_rec->audio = 0;
		OSCL_LOGI("");
	}
#endif

	al_component_deinit(&video_rec->venc_info.al_comp);
	al_component_deinit(&video_rec->vsink_info.al_comp);
	al_component_deinit(&video_rec->mux_info.al_comp);

	if (video_rec->mux_info.filename != NULL)
		oscl_free(video_rec->mux_info.filename);
	if (video_rec->vsink_info.dev_name)
		oscl_free(video_rec->vsink_info.dev_name);

	watermark_deinit(video_rec->watermark_private);
	memset(video_rec, 0, sizeof(video_recorder_t));
	oscl_free(video_rec);
	OSCL_TRACE("exit");
}

