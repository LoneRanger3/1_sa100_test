/*
 * recorder_pano.c - Standard functionality for recorder pano.
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
#include "recorder_private.h"
#include "omx_vendor_lb.h"
#include "pano_component.h"

/**
 * _get_config_pano - get default config for pano.
 *
 * @venc_info: pano info structure
 *
 */
static void _get_config_pano(pano_info_t *pano_info)
{
	pano_info->preview_w = 256;
	pano_info->preview_h = 480;
	pano_info->vout->nbuffer = PANO_DEFAULT_BUF_NUM;
}

int _set_cali_para(pano_info_t *pano_info, pano_cali_para_t *dst, cali_param_t *src)
{
	/* checker board */
	dst->box_rows = src->box_rows;
	dst->box_cols = src->box_cols;
	dst->box_width = src->box_width;
	dst->box_height = src->box_height;
	dst->dist_2_rear = src->dist_2_rear;
	dst->car_width = src->car_width;
	dst->car_length = src->car_length;
	dst->preview_width = pano_info->preview_w;
	dst->preview_height = pano_info->preview_h;
	dst->front_dist = src->front_dist;
	dst->rear_dist = src->rear_dist;
	dst->align = src->align;

	/* calibration imgage */
	dst->use_ext_cali_img = src->use_ext_cali_img; /* only use debug calibration */
	strncpy((char *)dst->cali_img.path, src->ext_cali_img.path,
			sizeof(dst->cali_img.path) - 1);
	dst->cali_img.width = src->ext_cali_img.width;
	dst->cali_img.height = src->ext_cali_img.height;
	if (dst->use_ext_cali_img) {
		if (0 != strcmp(src->ext_cali_img.format, "nv12")) {
			OSCL_LOGE("input calibration image format(%s) not support",
					src->ext_cali_img.format);
			return -1;
		}
		dst->cali_img.format = OMX_COLOR_FormatYUV420SemiPlanar;
	}

	return 0;
}

int _set_init_pano_para(pano_init_para_t *dst, pano_param_t *src)
{
	int i = 0;

	dst->in_gps = src->in_gps;
	dst->in_obd = src->in_obd;
	dst->car_para_en = src->car_para_en;
	dst->car_width = src->car_width;
	dst->data_format = src->data_format;
	dst->use_ext_cutline = src->use_ext_cutline;
	dst->culine = src->culine;

	if (ARRARSIZE(src->warning_line) != ARRARSIZE(dst->warning_line)) {
		OSCL_LOGE("set pano waring line error.");
		return -1;
	}
	for (i = 0; i < ARRARSIZE(src->warning_line); i++) {
		if (src->warning_line[i] <= 0) {
			OSCL_LOGE("warning line value error. [%d] = %d",
						i, src->warning_line[i]);
			return -1;
		}
		dst->warning_line[i] = src->warning_line[i];
	}

	/* car image */
	dst->carb_img.width = src->carboard_img.width;
	dst->carb_img.height = src->carboard_img.height;
	if (0 != strcmp(src->carboard_img.format, "nv12")) {
		OSCL_LOGE("input car board image format(%s) not support",
				src->carboard_img.format);
		return -1;
	}
	dst->carb_img.format = OMX_COLOR_FormatYUV420SemiPlanar;
	strncpy((char *)dst->carb_img.path, src->carboard_img.path,
			sizeof(dst->carb_img.path) - 1);

	return 0;
}

static int _pano_prepare_port(pano_info_t *pano_info)
{
	OMX_PARAM_PORTDEFINITIONTYPE port_def;
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	al_comp_info_t *v_pano = &pano_info->al_comp;

	port_def.nPortIndex = pano_info->vout->index;
	ret = OMX_GetParameter(v_pano->cmp_hdl,
			OMX_IndexParamPortDefinition, &port_def);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	port_def.format.video.nFrameWidth = pano_info->preview_w;
	port_def.format.video.nFrameHeight = pano_info->preview_h;
	port_def.format.video.nStride = pano_info->preview_w;
	port_def.format.video.nSliceHeight = pano_info->preview_h;
	port_def.nBufferCountActual = pano_info->vout->nbuffer;
	ret = OMX_SetParameter(v_pano->cmp_hdl,
			OMX_IndexParamPortDefinition, &port_def);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);


EXIT:
	return OMX_ErrorNone;

}

static int _pano_unprepare(video_recorder_t *video_rec)
{
	int ret = 0;

	OSCL_TRACE("start");
	vsrc_camera_disable_port(video_rec->camera, video_rec->splt_pano);

	/* disconnect video source with video splitter */
	OSCL_LOGE("");
	al_untunnel_unset_ports(video_rec->splt_pano,
			video_rec->pano_info.vin);
	vsrc_camera_putport(video_rec->camera, video_rec->splt_pano);

	if (video_rec->prot_info.mode.nRotation == VDISP_ROTATE_NONE)
		al_untunnel_unset_ports(video_rec->pano_info.vout,
				video_rec->psink_info.vin);
	else {
		al_untunnel_unset_ports(video_rec->pano_info.vout,
				video_rec->prot_info.vin);
		al_untunnel_unset_ports(video_rec->prot_info.vout,
				video_rec->psink_info.vin);
		al_component_setstate(&video_rec->prot_info.al_comp, OMX_StateLoaded);
	}
	OSCL_LOGE("");

	/* change to load state for recorder components*/
	al_component_setstate(&video_rec->psink_info.al_comp, OMX_StateLoaded);
	al_component_setstate(&video_rec->pano_info.al_comp, OMX_StateLoaded);

	return ret;
}



static int _pano_prepare(video_recorder_t *video_rec)
{
	int ret = 0;

	OSCL_TRACE("pano prepare start");
	if (video_rec == NULL)
		return ret;
	ret = _pano_prepare_port(&video_rec->pano_info);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);
	video_rec->prot_info.mode.nRotation = video_rec->rot;

	video_rec->splt_pano = vsrc_camera_getport(video_rec->camera, VDISP_ROTATE_NONE);
	oscl_param_check_exit(video_rec->splt_pano != NULL, ret, NULL);

	ret = al_untunnel_setup_ports(video_rec->splt_pano,
				video_rec->pano_info.vin);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	/* connect video splitter with video sink */
	if (video_rec->prot_info.mode.nRotation != VDISP_ROTATE_NONE) {
		ret = al_untunnel_setup_ports(video_rec->pano_info.vout,
				video_rec->prot_info.vin);
		oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

		ret = OMX_SetParameter(video_rec->prot_info.al_comp.cmp_hdl,
				OMX_IndexConfigCommonRotate,
				&video_rec->prot_info.mode);
		if (ret != OMX_ErrorNone) {
			OSCL_LOGE("OMX_IndexConfigCommonRotate fail.");
			video_rec->prot_info.mode.nRotation = VDISP_ROTATE_NONE;
			al_untunnel_unset_ports(video_rec->pano_info.vout,
				video_rec->prot_info.vin);
		}
	}
	if (video_rec->prot_info.mode.nRotation == VDISP_ROTATE_NONE) {
		ret = al_untunnel_setup_ports(video_rec->pano_info.vout,
				video_rec->psink_info.vin);
		oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);
	} else {
		ret = al_untunnel_setup_ports(video_rec->prot_info.vout,
				video_rec->psink_info.vin);
		oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);
	}

	/* change to idle state for recorder components*/
	al_component_setstate(&video_rec->psink_info.al_comp, OMX_StateIdle);
	if (video_rec->prot_info.mode.nRotation != VDISP_ROTATE_NONE)
		al_component_setstate(&video_rec->prot_info.al_comp, OMX_StateIdle);
	al_component_setstate(&video_rec->pano_info.al_comp, OMX_StateIdle);
	video_rec->pstate = REC_STATE_PREPARED;
EXIT:
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("video_rec_prepare error! ret:%x", ret);
		_pano_unprepare(video_rec);
		video_rec->state = REC_STATE_ERR;
	}
	OSCL_TRACE("prepare end! ret:%x", ret);
	OSCL_TRACE("video pano->state:%x", video_rec->state);
	return ret;
}

/**
 * video_pano_start - start preview.
 *
 * @video_rec: video recorder handle
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int video_pano_start(video_recorder_t *video_rec)
{
	int ret = 0;

	ret = _pano_prepare(video_rec);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);

	/* current state is not prepared, return failed */
	oscl_param_check_exit((video_rec->pstate == REC_STATE_PREPARED), -1, NULL);

	/* change to idle state for recorder components*/
	al_component_setstate(&video_rec->psink_info.al_comp, OMX_StateExecuting);
	if (video_rec->prot_info.mode.nRotation != VDISP_ROTATE_NONE)
		al_component_setstate(&video_rec->prot_info.al_comp, OMX_StateExecuting);
	al_component_setstate(&video_rec->pano_info.al_comp, OMX_StateExecuting);
	vsrc_camera_enable_port(video_rec->camera, video_rec->splt_pano);

	video_rec->pstate = REC_STATE_PREVIEW;
EXIT:
	if (ret != OMX_ErrorNone)
		video_rec->pstate = REC_STATE_ERR;
	OSCL_TRACE("end! ret:%x", ret);
	OSCL_TRACE("video pano->state:%x", video_rec->state);
	return ret;
}

/**
 * video_rec_stop_preview - stop preview.
 *
 * @video_rec: video recorder handle
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int video_pano_stop(video_recorder_t *video_rec)
{
	int ret = 0;

	OSCL_TRACE("start");
	/* current state is not REC_STATE_PREVIEW, return failed */
	oscl_param_check_exit((video_rec->pstate == REC_STATE_PREVIEW), -1, NULL);

	/* change to idle state for recorder components*/
	OSCL_LOGW("===");
	al_component_setstate(&video_rec->psink_info.al_comp, OMX_StateIdle);
	if (video_rec->prot_info.mode.nRotation != VDISP_ROTATE_NONE)
		al_component_setstate(&video_rec->prot_info.al_comp, OMX_StateIdle);
	OSCL_LOGW("===");
	al_component_setstate(&video_rec->pano_info.al_comp, OMX_StateIdle);
	_pano_unprepare(video_rec);
	video_rec->pstate = REC_STATE_PREPARED;
EXIT:
	if (ret != OMX_ErrorNone)
		video_rec->pstate = REC_STATE_ERR;

	return ret;
}

int video_pano_cali_process(video_recorder_t *video_rec, cali_contex_t *ctx)
{
	int ret = 0;
	pano_cali_contex_t cali_ctx;

	ret = OMX_SetParameter(video_rec->pano_info.al_comp.cmp_hdl,
				omx_index_pano_cb_process, &cali_ctx);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);
	if (ctx) {
		ctx->cutline_dnthr = cali_ctx.cutline_dnthr;
		ctx->cutline_upthr = cali_ctx.cutline_upthr;
		ctx->cutline = cali_ctx.cutline;
		ctx->car_rect.x = cali_ctx.car_rect.left;
		ctx->car_rect.y = cali_ctx.car_rect.top;
		ctx->car_rect.width = cali_ctx.car_rect.width;
		ctx->car_rect.height = cali_ctx.car_rect.height;
	}
EXIT:
	return ret;
}

int video_pano_set_disp_mode(video_recorder_t *video_rec,
			win_para_t *win)
{
	int ret = 0;
	vdisp_para_t disp_para;

	disp_para.mode = win->mode;
	disp_para.win_rect.left = win->rect.x;
	disp_para.win_rect.top = win->rect.y;
	disp_para.win_rect.width = win->rect.width;
	disp_para.win_rect.height = win->rect.height;
	ret = OMX_SetParameter(video_rec->psink_info.al_comp.cmp_hdl,
			omx_index_vrender_mode,
			&disp_para);
	oscl_param_check_exit(ret == OMX_ErrorNone, -1, NULL);
EXIT:
	return ret;
}

int video_pano_set_preview_size(video_recorder_t *video_rec, vsize_t *size)
{
	video_rec->pano_info.preview_w = size->width;
	video_rec->pano_info.preview_h = size->height;

	return 0;
}

/**
* video_pano_set_init_para - set pano init param
*
* @video_rec: video recorder handle
* @para: init pano para
*
* Returns 0 on success, -EERROR otherwise..
*/
int video_pano_set_init_para(video_recorder_t *video_rec,
			pano_param_t *para)
{
	int ret = 0;
	pano_init_para_t init_para;

	ret = _set_init_pano_para(&init_para, para);
	oscl_param_check_exit(ret == 0, -1, NULL);
	ret = OMX_SetParameter(video_rec->pano_info.al_comp.cmp_hdl,
				omx_index_pano_init_para, &init_para);
	oscl_param_check_exit(ret == OMX_ErrorNone, -1, NULL);
EXIT:
	return ret;
}

/**
* video_pano_set_cali_data - set pano calibration out data
*
* @video_rec: video recorder handle
* @para: calibration param
*
* Returns 0 on success, -EERROR otherwise..
*/
int video_pano_set_cali_para(video_recorder_t *video_rec,
			cali_param_t *para)
{
	int ret = 0;
	pano_cali_para_t cali_para;

	ret = _set_cali_para(&video_rec->pano_info, &cali_para, para);
	oscl_param_check_exit(ret == 0, -1, NULL);
	cali_para.preview_height = video_rec->pano_info.preview_h;
	cali_para.preview_width = video_rec->pano_info.preview_w;
	ret = OMX_SetParameter(video_rec->pano_info.al_comp.cmp_hdl,
				omx_index_pano_cb_para, &cali_para);
	oscl_param_check_exit(ret == OMX_ErrorNone, -1, NULL);
EXIT:
	return ret;
}

/**
* video_pano_set_cali_data - set pano calibration out data
*
* @video_rec: video recorder handle
* @out: calibration out data
*
* Returns 0 on success, -EERROR otherwise..
*/
int video_pano_set_cali_data(video_recorder_t *video_rec,
			cali_out_data_t *out)
{
	int ret = 0;
	pano_cali_out_data_t cali_out;

	oscl_param_check_exit(out != NULL, -1, NULL);

	cali_out.data_size = out->data_size;
	cali_out.data = out->data;
	ret = OMX_SetParameter(video_rec->pano_info.al_comp.cmp_hdl,
				omx_index_pano_cali_out_data,
				&cali_out);
	oscl_param_check_exit(ret == OMX_ErrorNone, -1, NULL);
EXIT:
	return ret;
}

/**
* video_pano_get_cali_data - get pano calibration out data
*
* @video_rec: video recorder handle
* @out: calibration out data
*
* Returns 0 on success, -EERROR otherwise..
*/
int video_pano_get_cali_data(video_recorder_t *video_rec,
			cali_out_data_t *out)
{
	int ret = 0;
	pano_cali_out_data_t cali_out;

	memset(&cali_out, 0, sizeof(cali_out));
	oscl_param_check_exit(out != NULL, -1, NULL);
	ret = OMX_GetParameter(video_rec->pano_info.al_comp.cmp_hdl,
				omx_index_pano_cali_out_data,
				&cali_out);
	oscl_param_check_exit(ret == OMX_ErrorNone, -1, NULL);
	out->data_size = cali_out.data_size;
	out->data = cali_out.data; /* careful! app need to free */
EXIT:
	return ret;
}

/* level>> 0: top 1: bottom */
int video_pano_set_layer_level(video_recorder_t *video_rec,
			video_layer_level_t level)
{
	int ret = 0;
	al_comp_info_t *al_comp = NULL;

	oscl_param_check((NULL != video_rec), -1, NULL);
	al_comp = &video_rec->psink_info.al_comp;
	OSCL_LOGI("Set video layer level:%x", level);

	if (VIDEO_LAYER_TOP == level)
		ret = OMX_SetParameter(al_comp->cmp_hdl, omx_index_vrender_win_top,
					NULL);
	else
		ret = OMX_SetParameter(al_comp->cmp_hdl, omx_index_vrender_win_bottom,
					NULL);
	if (ret != OMX_ErrorNone)
		video_rec->pstate = REC_STATE_ERR;

	return ret;
}

int video_pano_creat(video_recorder_t *video_rec)
{
	int index = 0;
	int ret = 0;

	if (video_rec == NULL || video_rec->pstate == REC_STATE_INIT)
		return -1;
	/* init panoramic component */
	ret = al_component_init(&video_rec->pano_info.al_comp, "OMX.LB.VIDEO.PANO",
			&al_untunnel_common_callbacks);
	oscl_param_check_exit(ret == 0, ret, NULL);
	index = al_get_port_index(&video_rec->pano_info.al_comp, OMX_DirInput,
				OMX_PortDomainVideo);
	oscl_param_check_exit(index >= 0, -1, NULL);
	video_rec->pano_info.vin = &video_rec->pano_info.al_comp.port_info[index];
	index = al_get_port_index(&video_rec->pano_info.al_comp, OMX_DirOutput,
			OMX_PortDomainVideo);
	oscl_param_check_exit(index >= 0, -1, NULL);
	video_rec->pano_info.vout = &video_rec->pano_info.al_comp.port_info[index];

	/* init pano video rotate component */
	ret = al_component_init(&video_rec->prot_info.al_comp, "OMX.LB.VIDEO.ROT",
			&al_untunnel_common_callbacks);
	oscl_param_check_exit(ret == 0, ret, NULL);
	index = al_get_port_index(&video_rec->prot_info.al_comp, OMX_DirInput,
			OMX_PortDomainVideo);
	oscl_param_check_exit(index >= 0, -1, NULL);
	video_rec->prot_info.vin = &video_rec->prot_info.al_comp.port_info[index];
	index = al_get_port_index(&video_rec->prot_info.al_comp, OMX_DirOutput,
			OMX_PortDomainVideo);
	oscl_param_check_exit(index >= 0, -1, NULL);
	video_rec->prot_info.vout = &video_rec->prot_info.al_comp.port_info[index];

	/* init pano video sink component */
	ret = al_component_init(&video_rec->psink_info.al_comp, "OMX.LB.SINK.VRENDER",
			&al_untunnel_common_callbacks);
	oscl_param_check_exit(ret == 0, ret, NULL);
	index = al_get_port_index(&video_rec->psink_info.al_comp, OMX_DirInput,
			OMX_PortDomainVideo);
	oscl_param_check_exit(index >= 0, -1, NULL);
	video_rec->psink_info.vin = &video_rec->psink_info.al_comp.port_info[index];

	_get_config_pano(&video_rec->pano_info);
	video_rec->prot_info.mode.nRotation = VDISP_ROTATE_90;

	video_rec->pstate = REC_STATE_INIT;

EXIT:
	if (ret != 0)
		video_pano_release(video_rec);
	return ret;
}

void video_pano_release(video_recorder_t *video_rec)
{
	if (video_rec == NULL)
		return;
	al_component_deinit(&video_rec->pano_info.al_comp);
	OSCL_LOGE("");
	al_component_deinit(&video_rec->prot_info.al_comp);
	OSCL_LOGE("");
	al_component_deinit(&video_rec->psink_info.al_comp);
	OSCL_LOGE("");
}

