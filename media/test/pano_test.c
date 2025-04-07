/*
 * pano_test.c - test pano mod of omx component.
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
#define DBG_LEVEL		DBG_INFO

#include <oscl.h>
#include <base_component.h>
#include <omx_vendor_lb.h>
#include "vrender_component.h"
#include "vrec_component.h"
#include "pano_component.h"

#define INPUT_BUF_NUM		8
#define OUTPUT_BUF_NUM		8

#define CAM_SIZE_VGA	(480)  /* 640*480 */
#define CAM_SIZE_720P	(720)  /* 1280*720 */
#define CAM_SIZE_1080P	(1080) /* 1920*1080 */

#define INPUT_DEVICE_NAME	"vic"
#define ISP_PREVIEW_DEV_NAME	"isp"
#define ISP_CAPTURE_DEV_NAME	"isp_cap.0"

typedef enum cam_out_size {
	CAM_OUTPUT_SIZE_VGA,
	CAM_OUTPUT_SIZE_720P,
	CAM_OUTPUT_SIZE_1080P,
} cam_out_size_e;

typedef enum preview_mode {
	PREVIEW_MOD_FRONT_AND_BACK,
	PREVIEW_MOD_FRONT,
	PREVIEW_MOD_BACK,
} preview_mode_e;

typedef enum stream_id {
	FRONT_STREAM_ID,
	BACK_STREAM_ID,
	ISP_PREV_STREAM_ID,
	ISP_CAP_STREAM_ID
} stream_id_e;

typedef struct port_info {
	void *comp_info;	/* al component*/
	int index;
	OMX_DIRTYPE edir;
	OMX_PORTDOMAINTYPE domain;

	int nbuffer;
	int buf_size;
	OMX_BUFFERHEADERTYPE **header;
	void *priv_data;
} port_info_t;

typedef struct pano_test {
	stream_id_e id;
	OMX_COMPONENTTYPE *source;	/* video receive component */

	OMX_COMPONENTTYPE *pano;	/* video panoation component */
	OMX_COMPONENTTYPE *rot;
	OMX_COMPONENTTYPE *sink;	/* pano out picture preview */

	OMX_S32 plane_size[3];		/* video size */
	OMX_PARAM_PORTDEFINITIONTYPE source_para;
	OMX_PARAM_PORTDEFINITIONTYPE pano_in_para;
	OMX_PARAM_PORTDEFINITIONTYPE pano_out_para;
	OMX_PARAM_PORTDEFINITIONTYPE sink_para;

	OMX_CONFIG_ROTATIONTYPE rot_mode;
	cam_out_size_e cam_size;	/* sensor out video size */
	preview_mode_e pre_mode;	/* preview mode*/

	vdisp_para_t disp_para;		/* window display parameter */

	int port_src_out;		/* video receive component out port */
	int port_sink_in;		/* video display component input port */
	int port_pano_in;
	int port_pano_out;

	sem_t sem_event;
	sem_t sem_eos;

	OMX_S32 buf_num;		/* buffer number to sensor */
	OMX_BUFFERHEADERTYPE **source_buffer;
	OMX_BUFFERHEADERTYPE **pano_buffer;
	OMX_U32 error_value;
} pano_test_t;

/* Callbacks implementation */
static OMX_ERRORTYPE event_handler(
	OMX_HANDLETYPE hComponent,
	OMX_PTR pAppData,
	OMX_EVENTTYPE eEvent,
	OMX_U32 Data1,
	OMX_U32 Data2,
	OMX_PTR pEventData)
{
	pano_test_t *private = (pano_test_t *)pAppData;
	char *name = NULL;
	OSCL_TRACE("hComponent:%p", hComponent);
	if (hComponent == private->sink)
		name = "sink";
	if (hComponent == private->source)
		name = "source";
	if (hComponent == private->pano)
		name = "pano";
	if (NULL == name) {
		OSCL_LOGE("Unknow component.");
		return OMX_ErrorBadParameter;
	}
	if (eEvent == OMX_EventCmdComplete) {
		if (Data1 == OMX_CommandStateSet) {
			switch ((int)Data2) {
			case OMX_StateInvalid:
				OSCL_LOGI("%s StateSet OMX_StateInvalid", name);
				break;
			case OMX_StateLoaded:
				OSCL_LOGI("%s StateSet OMX_StateLoaded", name);
				break;
			case OMX_StateIdle:
				OSCL_LOGI("%s StateSet OMX_StateIdle", name);
				break;
			case OMX_StateExecuting:
				OSCL_LOGI("%s StateSet OMX_StateExecuting", name);
				break;
			case OMX_StatePause:
				OSCL_LOGI("%s StateSet OMX_StatePause", name);
				break;
			case OMX_StateWaitForResources:
				OSCL_LOGI("%s StateSet WaitForResources", name);
				break;
			default:
				OSCL_LOGI("%s StateSet unkown state", name);
				break;
			}
			sem_post(&private->sem_event);
		} else if (Data1 == OMX_CommandPortEnable) {
			OSCL_LOGI("%s CmdComplete OMX_CommandPortEnable", name);
			sem_post(&private->sem_event);
		} else if (Data1 == OMX_CommandPortDisable) {
			OSCL_LOGI("%s CmdComplete OMX_CommandPortDisable", name);
			sem_post(&private->sem_event);
		}
	} else if (eEvent == OMX_EventBufferFlag) {
		if ((int)Data2 == OMX_BUFFERFLAG_EOS) {
			OSCL_LOGI("%s BufferFlag OMX_BUFFERFLAG_EOS", name);
			if (hComponent == private->sink) {
				OSCL_LOGE("end of tunnel");
				sem_post(&private->sem_eos);
			}
		} else
			OSCL_LOGI("%s OMX_EventBufferFlag %x", name, Data2);
	}  else if (eEvent == OMX_EventError) {
		private->error_value = Data1;
		OSCL_LOGE("Receive error event. value:%x", private->error_value);
		sem_post(&private->sem_event);
	} else {
		OSCL_LOGI("%s parm:%x %x", name, Data1, Data2);
	}

	return OMX_ErrorNone;
}

static OMX_ERRORTYPE get_screen_size(disp_size_t *size)
{
	void *hdl = NULL;

	oscl_param_check((size != NULL), OMX_ErrorBadParameter, NULL);
	hdl = oscl_open_disp_engine();
	if (hdl) {
		oscl_disp_get_screen_size(hdl, size);
		oscl_open_disp_engine(hdl);
	} else {
		return OMX_ErrorResourcesLost;
	}

	return OMX_ErrorNone;
}

static OMX_ERRORTYPE alloc_buffer(pano_test_t *priv,
			OMX_S32 sel_comp, OMX_S32 num, OMX_S32 size)
{
	OMX_S32 i = 0;
	OMX_S32 port_index = 0;
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	OMX_BUFFERHEADERTYPE **buffer;
	OMX_COMPONENTTYPE *comp = NULL;

	buffer = oscl_zalloc(num * sizeof(OMX_BUFFERHEADERTYPE *));
	if (NULL == buffer)
		return OMX_ErrorBadParameter;
	if (0 == sel_comp) {
		comp = priv->source;
		port_index = SOURCE_OUTPUT_PORT_INDEX;
	} else {
		comp = priv->pano;
		port_index = PANO_OUTPUT_PORT_INDEX;
	}
	for (i = 0; i < num; i++) {
		buffer[i] = NULL;
		ret = OMX_AllocateBuffer(comp, &buffer[i],
				port_index, priv, size);
		OSCL_LOGI("AllocateBuffer %p on port %i", buffer[i], port_index);
		if (ret != OMX_ErrorNone) {
			OSCL_LOGE("Error on AllocateBuffer %p on port %i\n",
				&buffer[i], port_index);
			break;
		}
	}
	if (0 == sel_comp)
		priv->source_buffer = buffer;
	else
		priv->pano_buffer = buffer;

	return ret;
}

static void free_buffer(pano_test_t *priv, OMX_S32 sel_comp)
{
	OMX_S32 i = 0;
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	OMX_S32 port_index = 0;
	OMX_BUFFERHEADERTYPE **buffer;

	if (0 == sel_comp) {
		buffer = priv->source_buffer;
		port_index = SOURCE_OUTPUT_PORT_INDEX;
	} else {
		buffer = priv->pano_buffer;
		port_index = PANO_OUTPUT_PORT_INDEX;
	}

	if (buffer) {
		for (i = 0; i < priv->buf_num; i++) {
			if (buffer[i]) {
				ret = OMX_FreeBuffer(priv->source,
						port_index,
						buffer[i]);
				if (ret != OMX_ErrorNone)
					OSCL_LOGE("port %d ,freebuffer:%d failed",
						port_index, i);
			}
			buffer[i] = NULL;
		}
		oscl_free(buffer);
		buffer = NULL;
	}
}

static void untunel_exit(pano_test_t *priv)
{
	if (priv->source) {
		OMX_FreeHandle(priv->source);
		priv->source = NULL;
	}
	if (priv->pano) {
		OMX_FreeHandle(priv->pano);
		priv->source = NULL;
	}
	if (priv->sink) {
		OMX_FreeHandle(priv->sink);
		priv->sink = NULL;
	}
	sem_destroy(&priv->sem_event);
	sem_destroy(&priv->sem_eos);
	OSCL_LOGD("here");
}

static int untunnel_config_source_component(pano_test_t *priv)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	char *name = NULL;
	OMX_PARAM_U32TYPE para;
	OMX_PARAM_PORTDEFINITIONTYPE port_def;

	name = INPUT_DEVICE_NAME;
	priv->cam_size = CAM_OUTPUT_SIZE_720P;

	ret = OMX_SetParameter(priv->source, OMX_IndexParamVideoInit, (OMX_PTR)name);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);
	OMX_GetParameter(priv->source, OMX_IndexParamNumAvailableStreams, (OMX_PTR)&para);
	oscl_param_check_exit((para.nU32 > 0), ret, NULL);
	memset(&priv->source_para, 0, sizeof(priv->source_para));
	priv->source_para.nVersion.nVersion = OMX_VERSION;
	priv->source_para.nPortIndex = SOURCE_OUTPUT_PORT_INDEX;
	ret = OMX_GetParameter(priv->source, OMX_IndexParamPortDefinition,
			&priv->source_para);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);
	priv->source_para.nBufferCountActual = para.nU32 * INPUT_BUF_NUM;
	priv->source_para.format.video.eColorFormat = OMX_COLOR_FormatYUV420SemiPlanar;
	if (CAM_OUTPUT_SIZE_VGA == priv->cam_size) {
		priv->source_para.format.video.nFrameWidth = 640;
		priv->source_para.format.video.nFrameHeight = 480;
		priv->source_para.format.video.nStride = 640;
		priv->source_para.format.video.nSliceHeight = 480;
	} else if (CAM_OUTPUT_SIZE_720P == priv->cam_size) {
		priv->source_para.format.video.nFrameWidth = 1280;
		priv->source_para.format.video.nFrameHeight = 720;
		priv->source_para.format.video.nStride = 1280;
		priv->source_para.format.video.nSliceHeight = 720;
	} else if (CAM_OUTPUT_SIZE_1080P == priv->cam_size) {
		priv->source_para.format.video.nFrameWidth = 1920;
		priv->source_para.format.video.nFrameHeight = 1080;
		priv->source_para.format.video.nStride = 1920;
		priv->source_para.format.video.nSliceHeight = 1080;
	} else {
		OSCL_LOGE("Do not supppano format.");
		return OMX_ErrorBadParameter;
	}
	priv->source_para.nBufferAlignment = 1024;
	priv->source_para.format.video.xFramerate = 25000; /* 25fps */
	OSCL_LOGD("source priv->port_para>> align:%d, w:%d h:%d, fps:%d (%d %d)",
		priv->source_para.nBufferAlignment,
		priv->source_para.format.video.nFrameWidth,
		priv->source_para.format.video.nFrameHeight,
		priv->source_para.format.video.xFramerate,
		priv->source_para.format.video.nStride,
		priv->source_para.format.video.nSliceHeight);
	ret = OMX_SetParameter(priv->source, OMX_IndexParamPortDefinition,
			&priv->source_para);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	port_def.nVersion.nVersion = OMX_VERSION;
	port_def.nPortIndex = SOURCE_OUTPUT_PORT_INDEX;
	ret = OMX_GetParameter(priv->source, OMX_IndexParamPortDefinition, &port_def);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("Error when getting OMX_PORT_PARAM_TYPE,%x\n", ret);
		goto EXIT;
	}
	if ((0 == port_def.nBufferSize) || (0 == port_def.nBufferCountActual)) {
		OSCL_LOGE("Error when getting OMX_PORT_PARAM_TYPE");
		goto EXIT;
	}
	LOG_I("port:%d, bufnum:%d", port_def.nPortIndex, port_def.nBufferCountActual);
	ret = alloc_buffer(priv, 0, port_def.nBufferCountActual, port_def.nBufferSize);
EXIT:
	return ret;
}

void set_calibration_para(pano_cali_para_t *cali_para, OMX_S32 prv_w, OMX_S32 prv_h)
{
	/* checker board */
	cali_para->box_rows = 5;
	cali_para->box_cols = 11;
	cali_para->box_width = 20;
	cali_para->box_height = 20;
	cali_para->dist_2_rear = 80;
	cali_para->car_width = 180;
	cali_para->car_length = 460;
	cali_para->preview_width = prv_w;
	cali_para->preview_height = prv_h;
	cali_para->front_dist = 100;
	cali_para->rear_dist = 500;
	cali_para->align = -1;

	/* calibration imgage */
	cali_para->use_ext_cali_img = 1; /* only use debug calibration */
	cali_para->cali_img.width = 1280;
	cali_para->cali_img.height = 720;
	cali_para->cali_img.format = OMX_COLOR_FormatYUV420SemiPlanar; /* NV12 */
	strncpy((char *)cali_para->cali_img.path, "/mnt/sdcard/cali_yuv.bin",
			sizeof(cali_para->cali_img.path));
}

void set_init_pano_para(pano_init_para_t *init_para, OMX_S32 car_w)
{
	init_para->in_gps = 0;
	init_para->in_obd = 0;
	init_para->car_para_en = 1;
	init_para->car_width = car_w;
	init_para->data_format = NULL;

	/* car image */
	init_para->carb_img.format = OMX_COLOR_FormatYUV420SemiPlanar;
	init_para->carb_img.width = 82;
	init_para->carb_img.height = 209;
	strncpy((char *)init_para->carb_img.path, "/mnt/sdcard/car_yuv.bin",
			sizeof(init_para->carb_img.path));

}
static OMX_ERRORTYPE untunnel_config_pano_component(pano_test_t *priv)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	pano_cali_para_t cali_para;
	pano_init_para_t init_para;

	/* 1. config input port para */
	memcpy(&priv->pano_in_para, &priv->source_para , sizeof(priv->pano_in_para));
	priv->pano_in_para.nVersion.nVersion = OMX_VERSION;
	priv->pano_in_para.nPortIndex = PANO_INPUT_PORT_INDEX;

	OSCL_LOGD("pano src:%d %d %d %d",
		priv->pano_in_para.format.video.nFrameWidth,
		priv->pano_in_para.format.video.nFrameHeight,
		priv->pano_in_para.format.video.nStride,
		priv->pano_in_para.format.video.nSliceHeight);
	ret = OMX_SetParameter(priv->pano, OMX_IndexParamPortDefinition,
					&priv->pano_in_para);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	OSCL_LOGD("tunnel_config_pano_component1");
	/* 3. config output port para, default output format is the same as input
	 * format */
	memset(&priv->pano_out_para, 0, sizeof(priv->pano_out_para));
	priv->pano_out_para.nVersion.nVersion = OMX_VERSION;
	priv->pano_out_para.nPortIndex = PANO_OUTPUT_PORT_INDEX;
	ret = OMX_GetParameter(priv->pano, OMX_IndexParamPortDefinition,
			&priv->pano_out_para);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	priv->pano_out_para.nBufferAlignment = 1024;
	priv->pano_out_para.format.video.nFrameWidth = 240;
	priv->pano_out_para.format.video.nFrameHeight = 480;
	priv->pano_out_para.format.video.nStride =
		priv->pano_out_para.format.video.nFrameWidth;
	priv->pano_out_para.format.video.nSliceHeight =
		priv->pano_out_para.format.video.nFrameHeight;
	ret = OMX_SetParameter(priv->pano, OMX_IndexParamPortDefinition,
					&priv->pano_out_para);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	OSCL_LOGD("tunnel_config_pano_component4");
	/* 6. get param after pano */
	priv->pano_out_para.nVersion.nVersion = OMX_VERSION;
	priv->pano_out_para.nPortIndex = PANO_OUTPUT_PORT_INDEX;
	ret = OMX_GetParameter(priv->pano, OMX_IndexParamPortDefinition,
			&priv->pano_out_para);

	OSCL_LOGI("pano:%d %d %d %d", priv->pano_out_para.format.video.nFrameWidth,
			priv->pano_out_para.format.video.nFrameHeight,
			priv->pano_out_para.format.video.nStride,
			priv->pano_out_para.format.video.nSliceHeight);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

#if 1  /* if already have calibration data, do not need to calibrate */
	/* 7. set calibration para */
	memset(&cali_para, 0, sizeof(cali_para));
	set_calibration_para(&cali_para,
		priv->pano_out_para.format.video.nStride,
		priv->pano_out_para.format.video.nSliceHeight);
	ret = OMX_SetParameter(priv->pano, omx_index_pano_cb_para, &cali_para);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	/* start calibration */
	ret = OMX_SetParameter(priv->pano, omx_index_pano_cb_process, (void *)1);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);
#else
	ret = OMX_SetParameter(priv->pano, omx_index_pano_cali_out_data, &cali_para);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);
#endif
	/* 8. set pano init para */
	memset(&init_para, 0, sizeof(init_para));
	set_init_pano_para(&init_para, cali_para.car_width);
	ret = OMX_SetParameter(priv->pano, omx_index_pano_init_para, &init_para);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	ret = alloc_buffer(priv, 1, priv->pano_out_para.nBufferCountActual,
			priv->pano_out_para.nBufferSize);
EXIT:
	return ret;
}

static OMX_ERRORTYPE untunnel_config_sink_component
							(pano_test_t *priv)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	disp_size_t scn_size;

	/* 1. get screen size */
	memset(&scn_size, 0, sizeof(scn_size));
	ret = get_screen_size(&scn_size);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);
	OSCL_LOGI("scn_size.width:%d, height:%d", scn_size.width, scn_size.height);

	/* 2. config video diplay area */
	memset(&priv->disp_para, 0, sizeof(priv->disp_para));
	if (FRONT_STREAM_ID == priv->id) {
		priv->disp_para.mode = VDISP_WINDOW_FULL_SCREEN_VIDEO_RATIO;
		priv->disp_para.win_rect.width = scn_size.width;
		priv->disp_para.win_rect.height = scn_size.height;
				/* scn_size.height >> 1; */
		priv->disp_para.win_rect.left = 0;
		priv->disp_para.win_rect.top = 0;
			/* (scn_size.height - priv->disp_para.win_rect.height) >> 1; */
	} else if (BACK_STREAM_ID == priv->id) {
#if 1  /* cdr */
		priv->disp_para.mode = VDISP_WINDOW_FULL_SCREEN_VIDEO_RATIO;
		priv->disp_para.win_rect.width = scn_size.width;
		priv->disp_para.win_rect.height = scn_size.height >> 1;
		priv->disp_para.win_rect.left = 0;
		priv->disp_para.win_rect.top =
					scn_size.height - priv->disp_para.win_rect.height;
#else   /* evb */
		priv->disp_para.mode = VDISP_WINDOW_USERDEF;
		priv->disp_para.win_rect.width = scn_size.width;
		priv->disp_para.win_rect.height = scn_size.height;
		priv->disp_para.win_rect.left = 0;
		priv->disp_para.win_rect.top = 0;
#endif
	} else if (ISP_PREV_STREAM_ID == priv->id) {
#if 0  /* evb */
		priv->disp_para.mode = VDISP_WINDOW_FULL_SCREEN_VIDEO_RATIO;
		priv->disp_para.win_rect.width = scn_size.width;
		priv->disp_para.win_rect.height = scn_size.height;
		priv->disp_para.win_rect.left = 0;
		priv->disp_para.win_rect.top = 0;
#else  /* cdr */
		priv->disp_para.mode = VDISP_WINDOW_FULL_SCREEN_VIDEO_RATIO;
		priv->disp_para.win_rect.width = scn_size.width;
		priv->disp_para.win_rect.height = scn_size.height >> 1;
		priv->disp_para.win_rect.left = 0;
		priv->disp_para.win_rect.top = 0;
#endif
	} else if (ISP_CAP_STREAM_ID == priv->id) {
		priv->disp_para.mode = VDISP_WINDOW_FULL_SCREEN_VIDEO_RATIO;
		priv->disp_para.win_rect.width = scn_size.width >> 1;
		priv->disp_para.win_rect.height = scn_size.height >> 2;
		priv->disp_para.win_rect.left = 0;
		priv->disp_para.win_rect.top = 0;
	}
	ret = OMX_SetParameter(priv->sink, omx_index_vrender_mode,
			&priv->disp_para);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	/* 3. config port param */
	memcpy(&priv->sink_para, &priv->pano_out_para, sizeof(priv->sink_para));
	OSCL_LOGI("sink:%d %d %d %d", priv->sink_para.format.video.nFrameWidth,
			priv->sink_para.format.video.nFrameHeight,
			priv->sink_para.format.video.nStride,
			priv->sink_para.format.video.nSliceHeight);
	priv->sink_para.nVersion.nVersion = OMX_VERSION;
	priv->sink_para.nPortIndex = PANO_INPUT_PORT_INDEX;
	ret = OMX_SetParameter(priv->sink, OMX_IndexParamPortDefinition,
			&priv->sink_para);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	/* 4. config rot angle */
	priv->rot_mode.nRotation = 0;
	ret = OMX_SetParameter(priv->sink, OMX_IndexConfigCommonRotate,
			&priv->rot_mode);
EXIT:
	return ret;
}


static OMX_ERRORTYPE untunnel_empty_buffer_done(
	OMX_HANDLETYPE hComponent,
	OMX_PTR pAppData,
	OMX_BUFFERHEADERTYPE *pBuffer)
{
	OMX_ERRORTYPE err;
	OMX_COMPONENTTYPE *base_comp = NULL;
	pano_test_t *priv = pAppData;

	if (pBuffer == NULL || pAppData == NULL) {
		OSCL_LOGE("err: buffer header is null");
		return OMX_ErrorBadParameter;
	}
	OSCL_LOGD("empty buffer done %s>> %p, %d, input:%d output:%d",
		get_component_name(get_lb_component(hComponent)),
		pBuffer->pBuffer, pBuffer->nFlags,
		pBuffer->nInputPortIndex, pBuffer->nOutputPortIndex);
	if (priv->pano == hComponent) {
		base_comp = priv->source;
		pBuffer->nOutputPortIndex = SOURCE_OUTPUT_PORT_INDEX;
	} else if (priv->sink == hComponent) {
		base_comp = priv->pano;
		pBuffer->nOutputPortIndex = PANO_OUTPUT_PORT_INDEX;
	}
	err = OMX_FillThisBuffer(base_comp, pBuffer);
	if (err != OMX_ErrorNone)
		OSCL_LOGE("OMX_FillThisBuffer err: %x", err);

	return err;
}

static OMX_ERRORTYPE untunnel_fill_buffer_done(
	OMX_HANDLETYPE hComponent,
	OMX_PTR pAppData,
	OMX_BUFFERHEADERTYPE *pBuffer)
{
	OMX_ERRORTYPE err;
	OMX_COMPONENTTYPE *base_comp = NULL;
	pano_test_t *priv = pAppData;

	if (pBuffer == NULL || pAppData == NULL) {
		OSCL_LOGE("err: buffer header is null");
		return OMX_ErrorBadParameter;
	}
	OSCL_LOGD("fill buffer done %s>> %p, %d, input:%d output:%d",
		get_component_name(get_lb_component(hComponent)),
		pBuffer->pBuffer, pBuffer->nFlags,
		pBuffer->nInputPortIndex, pBuffer->nOutputPortIndex);
	if (priv->pano == hComponent) {
		base_comp = priv->sink;
		pBuffer->nInputPortIndex = SINK_INPUT_PORT_INDEX;
	} else if (priv->source == hComponent) {
		base_comp = priv->pano;
		pBuffer->nInputPortIndex = PANO_INPUT_PORT_INDEX;
	}
	err = OMX_EmptyThisBuffer(base_comp, pBuffer);
	if (err != OMX_ErrorNone)
		OSCL_LOGE("OMX_EmptyThisBuffer err: %x", err);

	return err;
}

static OMX_CALLBACKTYPE untunnel_callbacks = {
	.EventHandler = event_handler,
	.EmptyBufferDone = untunnel_empty_buffer_done,
	.FillBufferDone = untunnel_fill_buffer_done,
};

static void *untunnel_stream_process_thread(void *param)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	pano_test_t *priv = NULL;
	OMX_S32 i = 0;
	OMX_PARAM_PORTDEFINITIONTYPE port_def;

	priv = oscl_zalloc(sizeof(pano_test_t));
	if (NULL == priv) {
		OSCL_LOGE("malloc fail.");
		return NULL;
	}
	priv->id = (stream_id_e)param;

	/* 1. get component handle */
	OMX_GetHandle((void **)&priv->source,
		"OMX.LB.SOURCE.VREC", priv, &untunnel_callbacks);
	OMX_GetHandle((void **)&priv->pano,
		"OMX.LB.VIDEO.PANO", priv, &untunnel_callbacks);
	OMX_GetHandle((void **)&priv->rot,
		"OMX.LB.VIDEO.ROT", priv, &untunnel_callbacks);
	OMX_GetHandle((void **)&priv->sink,
		"OMX.LB.SINK.VRENDER", priv, &untunnel_callbacks);
	OSCL_LOGD("source:%p pano:%p rot:%p sink:%p",
				priv->source, priv->pano,
				priv->rot, priv->sink);
	if (priv->source == NULL ||
		priv->rot == NULL || priv->sink == NULL) {
		OSCL_LOGE("get handle failed! %x, %x, %x, %x", priv->source,
			priv->pano, priv->rot, priv->sink);
		OSCL_LOGE("here");
		goto EXIT;
	}
	OSCL_LOGD("here");

	sem_init(&priv->sem_event, 0, 0);
	sem_init(&priv->sem_eos, 0, 0);

	/* 2. config video receive component */
	ret = untunnel_config_source_component(priv);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	/* 3. config video pano component */
	ret = untunnel_config_pano_component(priv);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	/* 4. config video receive component */
	ret = untunnel_config_sink_component(priv);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	/* 5. set component stat to idle */
	ret = OMX_SendCommand(priv->source, OMX_CommandStateSet, OMX_StateIdle, NULL);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	sem_wait(&priv->sem_event);
	ret = OMX_SendCommand(priv->pano, OMX_CommandStateSet, OMX_StateIdle, NULL);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	sem_wait(&priv->sem_event);
	ret = OMX_SendCommand(priv->sink, OMX_CommandStateSet, OMX_StateIdle, NULL);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	sem_wait(&priv->sem_event);

	/* 6. send buffer to source component queue */
	port_def.nVersion.nVersion = OMX_VERSION;
	port_def.nPortIndex = SOURCE_OUTPUT_PORT_INDEX;
	ret = OMX_GetParameter(priv->source, OMX_IndexParamPortDefinition, &port_def);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("Error when getting OMX_PORT_PARAM_TYPE,%x\n", ret);
		goto EXIT;
	}
	OSCL_LOGD("port_def.nBufferCountActual:%d", port_def.nBufferCountActual);
	for (i = 0; i < port_def.nBufferCountActual; i++) {
		ret = OMX_FillThisBuffer(priv->source, priv->source_buffer[i]);
		OSCL_LOGI("OMX_FillThisBuffer %p on port %i", priv->source_buffer[i],
			priv->source_buffer[i]->nOutputPortIndex);
		oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	}

	/* 7. send buffer to pano component queue */
	port_def.nVersion.nVersion = OMX_VERSION;
	port_def.nPortIndex = PANO_OUTPUT_PORT_INDEX;
	ret = OMX_GetParameter(priv->pano, OMX_IndexParamPortDefinition, &port_def);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("Error when getting OMX_PORT_PARAM_TYPE,%x\n", ret);
		goto EXIT;
	}
	OSCL_LOGD("port_def.nBufferCountActual:%d", port_def.nBufferCountActual);
	for (i = 0; i < port_def.nBufferCountActual; i++) {
		ret = OMX_FillThisBuffer(priv->pano, priv->pano_buffer[i]);
		OSCL_LOGI("OMX_FillThisBuffer %p on port %i", priv->pano_buffer[i],
			priv->pano_buffer[i]->nOutputPortIndex);
		oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	}

	/* 8. set component stat to executing */
	ret = OMX_SendCommand(priv->source, OMX_CommandStateSet,
			OMX_StateExecuting, NULL);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	sem_wait(&priv->sem_event);
	ret = OMX_SendCommand(priv->pano, OMX_CommandStateSet,
			OMX_StateExecuting, NULL);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	sem_wait(&priv->sem_event);
	ret = OMX_SendCommand(priv->sink, OMX_CommandStateSet, OMX_StateExecuting, NULL);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	sem_wait(&priv->sem_event);
#if 1
	sem_wait(&priv->sem_eos);
#else
	rt_thread_delay(1500); /* display 15s */
	OSCL_LOGD("quit test...");
#endif
	/* get calibrate data */
	{
		pano_cali_out_data_t cali_data;
		memset(&cali_data, 0, sizeof(cali_data));
		OMX_GetParameter(priv->pano, omx_index_pano_cali_out_data, &cali_data);
		OSCL_LOGI("..................calibrate out data.................");
		OSCL_LOGI("\tdata_size:%d", cali_data.data_size);
		OSCL_LOGI("\tdata:%p", cali_data.data);
		OSCL_LOGI(".....................................................");
	}
	/* 6. set component stat to idle */
	ret = OMX_SendCommand(priv->sink, OMX_CommandStateSet, OMX_StateIdle, NULL);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	ret = OMX_SendCommand(priv->pano, OMX_CommandStateSet, OMX_StateIdle, NULL);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	ret = OMX_SendCommand(priv->source, OMX_CommandStateSet, OMX_StateIdle, NULL);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	sem_wait(&priv->sem_event);
	sem_wait(&priv->sem_event);
	sem_wait(&priv->sem_event);

	/* 8. free buffer */
	free_buffer(priv, 0);
	free_buffer(priv, 1);

	/* 9. set component stat to loaded */
	ret = OMX_SendCommand(priv->source, OMX_CommandStateSet, OMX_StateLoaded, NULL);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	sem_wait(&priv->sem_event);
	ret = OMX_SendCommand(priv->pano, OMX_CommandStateSet, OMX_StateLoaded, NULL);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	sem_wait(&priv->sem_event);
	ret = OMX_SendCommand(priv->sink, OMX_CommandStateSet, OMX_StateLoaded, NULL);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	sem_wait(&priv->sem_event);

EXIT:
	untunel_exit(priv);
	oscl_free(priv);
	pthread_exit(NULL);

	return NULL;
}

const pthread_attr_t pano_pthread_default_attr = {
	0,				/* stack base */
	4096,				/* stack size */
	(RT_THREAD_PRIORITY_MAX/2 + RT_THREAD_PRIORITY_MAX/4),	/* priority */
	PTHREAD_CREATE_JOINABLE,	/* detach state */
	SCHED_FIFO,			/* scheduler policy */
	PTHREAD_INHERIT_SCHED		/* Inherit parent prio/policy */
};

int openmax_test_pano_untunnel(void)
{
	int ret = OMX_ErrorNone;
	pthread_t thread_id = NULL;

	OSCL_LOGD("test start...");
	pthread_create(&thread_id,
			&pano_pthread_default_attr,
			untunnel_stream_process_thread,
			(void *)BACK_STREAM_ID);
	OSCL_LOGD("Wait thread exit");
	if (thread_id)
		pthread_join(thread_id, NULL);
	OSCL_LOGD("Test complete....");

	return ret;
}

