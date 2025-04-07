/*
 * rotate_test.c - test rotate mod of omx component.
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
#include "rotate_component.h"

#define INPUT_BUF_NUM		8
#define OUTPUT_BUF_NUM		8

#define CAM_SIZE_VGA	(480)  /* 640*480 */
#define CAM_SIZE_720P	(720)  /* 1280*720 */
#define CAM_SIZE_1080P	(1080) /* 1920*1080 */

#define FRONT_INPUT_DEVICE_NAME	"mcsi"
#define BACK_INPUT_DEVICE_NAME	"vic"
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

typedef struct rotate_test {
	stream_id_e id;
	OMX_COMPONENTTYPE *source;	/* video receive component */
	OMX_COMPONENTTYPE *sink;	/* video display component */
	OMX_COMPONENTTYPE *rot;		/* video rotation component */
	OMX_S32 plane_size[3];		/* video size */
	OMX_PARAM_PORTDEFINITIONTYPE source_para;
	OMX_PARAM_PORTDEFINITIONTYPE rot_in_para;
	OMX_PARAM_PORTDEFINITIONTYPE rot_out_para;
	OMX_PARAM_PORTDEFINITIONTYPE sink_para;

	OMX_CONFIG_ROTATIONTYPE rot_mode;
	cam_out_size_e cam_size;	/* sensor out video size */
	preview_mode_e pre_mode;	/* preview mode*/

	vdisp_para_t disp_para;		/* window display parameter */

	int port_src_out;		/* video receive component out port */
	int port_sink_in;		/* video display component input port */
	int port_rot_in;
	int port_rot_out;

	sem_t sem_event;
	sem_t sem_eos;

	OMX_S32 buf_num;		/* buffer number to sensor */
	OMX_BUFFERHEADERTYPE **source_buffer;
	OMX_BUFFERHEADERTYPE **rot_buffer;
	OMX_U32 error_value;
} rotate_test_t;

/* Callbacks implementation */
static OMX_ERRORTYPE event_handler(
	OMX_HANDLETYPE hComponent,
	OMX_PTR pAppData,
	OMX_EVENTTYPE eEvent,
	OMX_U32 Data1,
	OMX_U32 Data2,
	OMX_PTR pEventData)
{
	rotate_test_t *private = (rotate_test_t *)pAppData;
	char *name = NULL;
	OSCL_TRACE("hComponent:%p", hComponent);
	if (hComponent == private->sink)
		name = "sink";
	if (hComponent == private->source)
		name = "source";
	if (hComponent == private->rot)
		name = "rotate";
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

static void tunnel_exit(rotate_test_t *priv)
{
	if (priv->source)
		OMX_FreeHandle(priv->source);
	if (priv->rot)
		OMX_FreeHandle(priv->rot);
	if (priv->sink)
		OMX_FreeHandle(priv->sink);
	priv->source = priv->sink = priv->rot = NULL;
	sem_destroy(&priv->sem_event);
	sem_destroy(&priv->sem_eos);

	OSCL_LOGD("here");
}

static int get_port_index(OMX_COMPONENTTYPE *comp, OMX_DIRTYPE dir,
	OMX_PORTDOMAINTYPE domain, int start)
{
	int i;
	int ret = -1;
	OMX_PARAM_PORTDEFINITIONTYPE port_def;

	port_def.nVersion.nVersion = OMX_VERSION;
	for (i = start; i < OMX_PORT_NUMBER_SUPPORTED; i++) {
		port_def.nPortIndex = i;
		ret = OMX_GetParameter(comp, OMX_IndexParamPortDefinition, &port_def);
		if (ret == OMX_ErrorNone && port_def.eDir == dir
			&& port_def.eDomain == domain) {
			ret = i;
			break;
		}
	}
	OSCL_LOGE("index:%d", i);
	if (i == OMX_PORT_NUMBER_SUPPORTED)
		OSCL_LOGE("can not get port, dir:%d, domain:%d, start:%d",
			dir, domain, start);
	return ret;
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

static int tunnel_config_source_component(rotate_test_t *priv)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	char *name = NULL;
	OMX_PARAM_U32TYPE para;

	if (FRONT_STREAM_ID == priv->id) {
		name = FRONT_INPUT_DEVICE_NAME;
		priv->cam_size = CAM_OUTPUT_SIZE_720P;
	} else if (BACK_STREAM_ID == priv->id) {
		name = BACK_INPUT_DEVICE_NAME;
		priv->cam_size = CAM_OUTPUT_SIZE_720P;
	} else if (ISP_PREV_STREAM_ID == priv->id) {
		name = ISP_PREVIEW_DEV_NAME;
		priv->cam_size = CAM_OUTPUT_SIZE_720P;
	} else if (ISP_CAP_STREAM_ID == priv->id) {
		name = ISP_CAPTURE_DEV_NAME;
		priv->cam_size = CAM_OUTPUT_SIZE_1080P;
	}

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
		OSCL_LOGE("Do not supprot format.");
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

EXIT:
	return ret;
}

static OMX_ERRORTYPE tunnel_config_rot_component(rotate_test_t *priv)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	OMX_CONFIG_RECTTYPE rect;

	/* 1. config input port para */
	memcpy(&priv->rot_in_para, &priv->source_para , sizeof(priv->rot_in_para));
	priv->rot_in_para.nVersion.nVersion = OMX_VERSION;
	priv->rot_in_para.nPortIndex = ROT_INPUT_PORT_INDEX;

	OSCL_LOGD("rot src:%d %d %d %d",
		priv->rot_in_para.format.video.nFrameWidth,
		priv->rot_in_para.format.video.nFrameHeight,
		priv->rot_in_para.format.video.nStride,
		priv->rot_in_para.format.video.nSliceHeight);
	ret = OMX_SetParameter(priv->rot, OMX_IndexParamPortDefinition,
					&priv->rot_in_para);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	OSCL_LOGD("tunnel_config_rot_component1");
	/* 3. config output port para, default output format is the same as input
	 * format */
	memset(&priv->rot_out_para, 0, sizeof(priv->rot_out_para));
	priv->rot_out_para.nVersion.nVersion = OMX_VERSION;
	priv->rot_out_para.nPortIndex = ROT_OUTPUT_PORT_INDEX;
	ret = OMX_GetParameter(priv->rot, OMX_IndexParamPortDefinition,
			&priv->rot_out_para);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	priv->rot_out_para.nBufferAlignment = 1024;
	ret = OMX_SetParameter(priv->rot, OMX_IndexParamPortDefinition,
					&priv->rot_out_para);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	OSCL_LOGD("tunnel_config_rot_component2");
	/* 4. config crop rect */
	memset(&rect, 0, sizeof(rect));
	rect.nTop = 0;
	rect.nLeft = 0;
	rect.nWidth = priv->rot_in_para.format.video.nStride;
	rect.nHeight = priv->rot_in_para.format.video.nSliceHeight;
	ret = OMX_SetParameter(priv->rot, OMX_IndexConfigCommonInputCrop, &rect);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	OSCL_LOGD("tunnel_config_rot_component3");
	/* 5. set rotate angle */
	memset(&priv->rot_mode, 0, sizeof(priv->rot_mode));
	priv->rot_mode.nRotation = VDISP_ROTATE_90;
	ret = OMX_SetParameter(priv->rot, OMX_IndexConfigCommonRotate,
						&priv->rot_mode);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	OSCL_LOGD("tunnel_config_rot_component4");
	/* 6. get param after rotate */
	priv->rot_out_para.nVersion.nVersion = OMX_VERSION;
	priv->rot_out_para.nPortIndex = ROT_OUTPUT_PORT_INDEX;
	ret = OMX_GetParameter(priv->rot, OMX_IndexParamPortDefinition,
			&priv->rot_out_para);

	OSCL_LOGI("rot:%d %d %d %d", priv->rot_out_para.format.video.nFrameWidth,
			priv->rot_out_para.format.video.nFrameHeight,
			priv->rot_out_para.format.video.nStride,
			priv->rot_out_para.format.video.nSliceHeight);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

EXIT:
	return ret;
}

static OMX_ERRORTYPE tunnel_config_sink_component(rotate_test_t *priv)
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
#if 1
		priv->disp_para.mode = VDISP_WINDOW_FULL_SCREEN_VIDEO_RATIO;
		priv->disp_para.win_rect.width = scn_size.width;
		priv->disp_para.win_rect.height = scn_size.height >> 1;
		priv->disp_para.win_rect.left = 0;
		priv->disp_para.win_rect.top =
					scn_size.height - priv->disp_para.win_rect.height;
#else
		priv->disp_para.mode = VDISP_WINDOW_USERDEF;
		priv->disp_para.win_rect.width = scn_size.width;
		priv->disp_para.win_rect.height = scn_size.height;
		priv->disp_para.win_rect.left = 0;
		priv->disp_para.win_rect.top = 0;
#endif
	} else if (ISP_PREV_STREAM_ID == priv->id) {
#if 0
		priv->disp_para.mode = VDISP_WINDOW_FULL_SCREEN_VIDEO_RATIO;
		priv->disp_para.win_rect.width = scn_size.width;
		priv->disp_para.win_rect.height = scn_size.height;
		priv->disp_para.win_rect.left = 0;
		priv->disp_para.win_rect.top = 0;
#else
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
	memcpy(&priv->sink_para, &priv->rot_out_para, sizeof(priv->sink_para));
	OSCL_LOGI("sink:%d %d %d %d", priv->sink_para.format.video.nFrameWidth,
			priv->sink_para.format.video.nFrameHeight,
			priv->sink_para.format.video.nStride,
			priv->sink_para.format.video.nSliceHeight);
	priv->sink_para.nVersion.nVersion = OMX_VERSION;
	priv->sink_para.nPortIndex = ROT_INPUT_PORT_INDEX;
	ret = OMX_SetParameter(priv->sink, OMX_IndexParamPortDefinition,
			&priv->sink_para);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	/* 4. config rotate angle */
	ret = OMX_SetParameter(priv->sink, OMX_IndexConfigCommonRotate,
			&priv->rot_mode);
EXIT:
	return ret;
}

static OMX_CALLBACKTYPE tunnel_callbacks = {
	.EventHandler = event_handler,
	.EmptyBufferDone = NULL,
	.FillBufferDone = NULL,
};

static void *tunnel_stream_process_thread(void *param)
{
	int ret = OMX_ErrorNone;
	rotate_test_t *priv = NULL;

	priv = oscl_zalloc(sizeof(rotate_test_t));
	if (NULL == priv) {
		OSCL_LOGE("malloc fail.");
		return NULL;
	}
	priv->id = (stream_id_e)param;

	/* 1. get component handle */
	OMX_GetHandle((void **)&priv->source,
		"OMX.LB.SOURCE.VREC", priv, &tunnel_callbacks);
	OMX_GetHandle((void **)&priv->sink,
		"OMX.LB.SINK.VRENDER", priv, &tunnel_callbacks);
	OMX_GetHandle((void **)&priv->rot,
		"OMX.LB.VIDEO.ROT", priv, &tunnel_callbacks);
	OSCL_LOGD("priv->source:%p priv->sink:%p, priv->rot:%p",
				priv->source, priv->sink, priv->rot);
	if (priv->source == NULL || priv->sink == NULL || priv->rot == NULL) {
		OSCL_LOGE("get handle failed! %x, %x, %x", priv->source,
			priv->sink, priv->rot);
		OSCL_LOGE("here");
		goto EXIT;
	}
	OSCL_LOGD("here");

	sem_init(&priv->sem_event, 0, 0);
	sem_init(&priv->sem_eos, 0, 0);

	/* 2. get component port */
	priv->port_src_out = get_port_index(priv->source, OMX_DirOutput,
			OMX_PortDomainVideo, 0);
	priv->port_sink_in = get_port_index(priv->sink, OMX_DirInput,
			OMX_PortDomainVideo, 0);
	priv->port_rot_in = get_port_index(priv->rot, OMX_DirInput,
			OMX_PortDomainVideo, 0);
	priv->port_rot_out = get_port_index(priv->rot, OMX_DirOutput,
			OMX_PortDomainVideo, 0);
	if (priv->port_src_out < 0 || priv->port_sink_in < 0 ||
		priv->port_rot_out < 0 || priv->port_rot_in < 0)
		goto EXIT;

	/* 3. config source component */
	ret = tunnel_config_source_component(priv);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	/* 4. config rotation component */
	tunnel_config_rot_component(priv);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	/* 5. config sink component */
	ret = tunnel_config_sink_component(priv);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	/* 5. setup tunnel */
	OSCL_LOGD("OMX_SetupTunnel");
	ret = OMX_SetupTunnel(priv->source, priv->port_src_out,
			priv->rot, priv->port_rot_in);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);

	OSCL_LOGD("OMX_SetupTunnel 1");
	ret = OMX_SetupTunnel(priv->rot, priv->port_rot_out,
			priv->sink, priv->port_sink_in);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);

	/* 6. change state to idle */
	OSCL_LOGD("source OMX_SendCommand OMX_StateIdle");
	ret = OMX_SendCommand(priv->source, OMX_CommandStateSet, OMX_StateIdle,
			NULL);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	sem_wait(&priv->sem_event);
	oscl_param_check_exit((priv->error_value == OMX_ErrorNone),
			priv->error_value, NULL);

	OSCL_LOGD("rotation OMX_SendCommand OMX_StateIdle");
	ret = OMX_SendCommand(priv->rot, OMX_CommandStateSet, OMX_StateIdle,
			NULL);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	sem_wait(&priv->sem_event);
	oscl_param_check_exit((priv->error_value == OMX_ErrorNone),
			priv->error_value, NULL);

	OSCL_LOGD("sink OMX_SendCommand OMX_StateIdle");
	ret = OMX_SendCommand(priv->sink, OMX_CommandStateSet, OMX_StateIdle,
			NULL);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	sem_wait(&priv->sem_event);
	oscl_param_check_exit((priv->error_value == OMX_ErrorNone),
			priv->error_value, NULL);

	/* 7. change state to executing */
	ret = OMX_SendCommand(priv->source, OMX_CommandStateSet,
			OMX_StateExecuting, NULL);
	sem_wait(&priv->sem_event);
	ret = OMX_SendCommand(priv->rot, OMX_CommandStateSet,
				OMX_StateExecuting, NULL);
	sem_wait(&priv->sem_event);
	ret = OMX_SendCommand(priv->sink, OMX_CommandStateSet,
			OMX_StateExecuting, NULL);
	sem_wait(&priv->sem_event);
#if 1
	sem_wait(&priv->sem_eos);
#else
	rt_thread_delay(1500); /* display 30s */
	OSCL_LOGD("quit test...");
#endif

	/*8. change state to idle */
	ret = OMX_SendCommand(priv->sink, OMX_CommandStateSet, OMX_StateIdle,
			NULL);

	ret = OMX_SendCommand(priv->rot, OMX_CommandStateSet, OMX_StateIdle,
			NULL);

	ret = OMX_SendCommand(priv->source, OMX_CommandStateSet, OMX_StateIdle,
			NULL);
	sem_wait(&priv->sem_event);
	sem_wait(&priv->sem_event);
	sem_wait(&priv->sem_event);
	OSCL_LOGD("here");

EXIT:
	/*8. change state to loaded */
	if (priv->source) {
		ret = OMX_SendCommand(priv->source, OMX_CommandStateSet,
				OMX_StateLoaded, NULL);
		sem_wait(&priv->sem_event);
	}
	if (priv->rot) {
		ret = OMX_SendCommand(priv->rot, OMX_CommandStateSet,
				OMX_StateLoaded, NULL);
		sem_wait(&priv->sem_event);
	}
	if (priv->sink) {
		ret = OMX_SendCommand(priv->sink, OMX_CommandStateSet,
				OMX_StateLoaded, NULL);
		sem_wait(&priv->sem_event);
	}
	tunnel_exit(priv);
	oscl_free(priv);
	pthread_exit(NULL);

	return NULL;
}

int openmax_test_rotate_tunnel(void)
{
	int ret = OMX_ErrorNone;
	pthread_t thread_id = NULL;
	pthread_t thread_id1 = NULL;

	OSCL_LOGD("test start...");
	pthread_create(&thread_id,
			NULL,
			tunnel_stream_process_thread,
			(void *)BACK_STREAM_ID);
	pthread_create(&thread_id1,
			NULL,
			tunnel_stream_process_thread,
			(void *)ISP_PREV_STREAM_ID);
	OSCL_LOGD("Wait thread exit");
	if (thread_id)
		pthread_join(thread_id, NULL);
	if (thread_id1)
		pthread_join(thread_id1, NULL);
	OSCL_LOGD("Test complete....");

	return ret;
}

static OMX_ERRORTYPE alloc_buffer(rotate_test_t *priv,
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
		comp = priv->rot;
		port_index = ROT_OUTPUT_PORT_INDEX;
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
		priv->rot_buffer = buffer;

	return ret;
}

static void free_buffer(rotate_test_t *priv, OMX_S32 sel_comp)
{
	OMX_S32 i = 0;
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	OMX_S32 port_index = 0;
	OMX_BUFFERHEADERTYPE **buffer;

	if (0 == sel_comp) {
		buffer = priv->source_buffer;
		port_index = SOURCE_OUTPUT_PORT_INDEX;
	} else {
		buffer = priv->rot_buffer;
		port_index = ROT_OUTPUT_PORT_INDEX;
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

static void untunel_exit(rotate_test_t *priv)
{
	if (priv->source) {
		OMX_FreeHandle(priv->source);
		priv->source = NULL;
	}
	if (priv->rot) {
		OMX_FreeHandle(priv->rot);
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

static int untunnel_config_source_component(rotate_test_t *priv)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	char *name = NULL;
	OMX_PARAM_U32TYPE para;
	OMX_PARAM_PORTDEFINITIONTYPE port_def;

	if (FRONT_STREAM_ID == priv->id) {
		name = FRONT_INPUT_DEVICE_NAME;
		priv->cam_size = CAM_OUTPUT_SIZE_720P;
	} else if (BACK_STREAM_ID == priv->id) {
		name = BACK_INPUT_DEVICE_NAME;
		priv->cam_size = CAM_OUTPUT_SIZE_720P;
	} else if (ISP_PREV_STREAM_ID == priv->id) {
		name = ISP_PREVIEW_DEV_NAME;
		priv->cam_size = CAM_OUTPUT_SIZE_720P;
	} else if (ISP_CAP_STREAM_ID == priv->id) {
		name = ISP_CAPTURE_DEV_NAME;
		priv->cam_size = CAM_OUTPUT_SIZE_1080P;
	}

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
		OSCL_LOGE("Do not supprot format.");
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

static OMX_ERRORTYPE untunnel_config_rot_component(rotate_test_t *priv)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	OMX_CONFIG_RECTTYPE rect;

	/* 1. config input port para */
	memcpy(&priv->rot_in_para, &priv->source_para , sizeof(priv->rot_in_para));
	priv->rot_in_para.nVersion.nVersion = OMX_VERSION;
	priv->rot_in_para.nPortIndex = ROT_INPUT_PORT_INDEX;

	OSCL_LOGD("rot src:%d %d %d %d",
		priv->rot_in_para.format.video.nFrameWidth,
		priv->rot_in_para.format.video.nFrameHeight,
		priv->rot_in_para.format.video.nStride,
		priv->rot_in_para.format.video.nSliceHeight);
	ret = OMX_SetParameter(priv->rot, OMX_IndexParamPortDefinition,
					&priv->rot_in_para);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	OSCL_LOGD("tunnel_config_rot_component1");
	/* 3. config output port para, default output format is the same as input
	 * format */
	memset(&priv->rot_out_para, 0, sizeof(priv->rot_out_para));
	priv->rot_out_para.nVersion.nVersion = OMX_VERSION;
	priv->rot_out_para.nPortIndex = ROT_OUTPUT_PORT_INDEX;
	ret = OMX_GetParameter(priv->rot, OMX_IndexParamPortDefinition,
			&priv->rot_out_para);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	priv->rot_out_para.nBufferAlignment = 1024;
	ret = OMX_SetParameter(priv->rot, OMX_IndexParamPortDefinition,
					&priv->rot_out_para);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	OSCL_LOGD("tunnel_config_rot_component2");
	/* 4. config crop rect */
	memset(&rect, 0, sizeof(rect));
	rect.nTop = 0;
	rect.nLeft = 0;
	rect.nWidth = priv->rot_in_para.format.video.nStride;
	rect.nHeight = priv->rot_in_para.format.video.nSliceHeight;
	ret = OMX_SetParameter(priv->rot, OMX_IndexConfigCommonInputCrop, &rect);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	OSCL_LOGD("tunnel_config_rot_component3");
	/* 5. set rotate angle */
	memset(&priv->rot_mode, 0, sizeof(priv->rot_mode));
	priv->rot_mode.nRotation = VDISP_ROTATE_90;
	ret = OMX_SetParameter(priv->rot, OMX_IndexConfigCommonRotate,
						&priv->rot_mode);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	OSCL_LOGD("tunnel_config_rot_component4");
	/* 6. get param after rotate */
	priv->rot_out_para.nVersion.nVersion = OMX_VERSION;
	priv->rot_out_para.nPortIndex = ROT_OUTPUT_PORT_INDEX;
	ret = OMX_GetParameter(priv->rot, OMX_IndexParamPortDefinition,
			&priv->rot_out_para);

	OSCL_LOGI("rot:%d %d %d %d", priv->rot_out_para.format.video.nFrameWidth,
			priv->rot_out_para.format.video.nFrameHeight,
			priv->rot_out_para.format.video.nStride,
			priv->rot_out_para.format.video.nSliceHeight);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);
	ret = alloc_buffer(priv, 1, priv->rot_out_para.nBufferCountActual,
			priv->rot_out_para.nBufferSize);
EXIT:
	return ret;
}

static OMX_ERRORTYPE untunnel_config_sink_component
							(rotate_test_t *priv)
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
	memcpy(&priv->sink_para, &priv->rot_out_para, sizeof(priv->sink_para));
	OSCL_LOGI("sink:%d %d %d %d", priv->sink_para.format.video.nFrameWidth,
			priv->sink_para.format.video.nFrameHeight,
			priv->sink_para.format.video.nStride,
			priv->sink_para.format.video.nSliceHeight);
	priv->sink_para.nVersion.nVersion = OMX_VERSION;
	priv->sink_para.nPortIndex = ROT_INPUT_PORT_INDEX;
	ret = OMX_SetParameter(priv->sink, OMX_IndexParamPortDefinition,
			&priv->sink_para);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	/* 4. config rotate angle */
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
	rotate_test_t *priv = pAppData;

	if (pBuffer == NULL || pAppData == NULL) {
		OSCL_LOGE("err: buffer header is null");
		return OMX_ErrorBadParameter;
	}
	OSCL_LOGD("empty buffer done %s>> %p, %d, input:%d output:%d",
		get_component_name(get_lb_component(hComponent)),
		pBuffer->pBuffer, pBuffer->nFlags,
		pBuffer->nInputPortIndex, pBuffer->nOutputPortIndex);
	if (priv->rot == hComponent) {
		base_comp = priv->source;
		pBuffer->nOutputPortIndex = SOURCE_OUTPUT_PORT_INDEX;
	} else if (priv->sink == hComponent) {
		base_comp = priv->rot;
		pBuffer->nOutputPortIndex = ROT_OUTPUT_PORT_INDEX;
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
	rotate_test_t *priv = pAppData;

	if (pBuffer == NULL || pAppData == NULL) {
		OSCL_LOGE("err: buffer header is null");
		return OMX_ErrorBadParameter;
	}
	OSCL_LOGD("fill buffer done %s>> %p, %d, input:%d output:%d",
		get_component_name(get_lb_component(hComponent)),
		pBuffer->pBuffer, pBuffer->nFlags,
		pBuffer->nInputPortIndex, pBuffer->nOutputPortIndex);
	if (priv->rot == hComponent) {
		base_comp = priv->sink;
		pBuffer->nInputPortIndex = SINK_INPUT_PORT_INDEX;
	} else if (priv->source == hComponent) {
		base_comp = priv->rot;
		pBuffer->nInputPortIndex = ROT_INPUT_PORT_INDEX;
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
	rotate_test_t *priv = NULL;
	OMX_S32 i = 0;
	OMX_PARAM_PORTDEFINITIONTYPE port_def;

	priv = oscl_zalloc(sizeof(rotate_test_t));
	if (NULL == priv) {
		OSCL_LOGE("malloc fail.");
		return NULL;
	}
	priv->id = (stream_id_e)param;

	/* 1. get component handle */
	OMX_GetHandle((void **)&priv->source,
		"OMX.LB.SOURCE.VREC", priv, &untunnel_callbacks);
	OMX_GetHandle((void **)&priv->sink,
		"OMX.LB.SINK.VRENDER", priv, &untunnel_callbacks);
	OMX_GetHandle((void **)&priv->rot,
		"OMX.LB.VIDEO.ROT", priv, &untunnel_callbacks);
	OSCL_LOGD("priv->source:%p priv->sink:%p, priv->rot:%p",
				priv->source, priv->sink, priv->rot);
	if (priv->source == NULL || priv->sink == NULL || priv->rot == NULL) {
		OSCL_LOGE("get handle failed! %x, %x, %x", priv->source,
			priv->sink, priv->rot);
		OSCL_LOGE("here");
		goto EXIT;
	}
	OSCL_LOGD("here");

	sem_init(&priv->sem_event, 0, 0);
	sem_init(&priv->sem_eos, 0, 0);

	/* 2. config video receive component */
	ret = untunnel_config_source_component(priv);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	/* 3. config video rotate component */
	ret = untunnel_config_rot_component(priv);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	/* 4. config video receive component */
	ret = untunnel_config_sink_component(priv);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	/* 5. set component stat to idle */
	ret = OMX_SendCommand(priv->source, OMX_CommandStateSet, OMX_StateIdle, NULL);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	sem_wait(&priv->sem_event);
	ret = OMX_SendCommand(priv->rot, OMX_CommandStateSet, OMX_StateIdle, NULL);
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

	/* 7. send buffer to rotate component queue */
	port_def.nVersion.nVersion = OMX_VERSION;
	port_def.nPortIndex = ROT_OUTPUT_PORT_INDEX;
	ret = OMX_GetParameter(priv->rot, OMX_IndexParamPortDefinition, &port_def);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("Error when getting OMX_PORT_PARAM_TYPE,%x\n", ret);
		goto EXIT;
	}
	OSCL_LOGD("port_def.nBufferCountActual:%d", port_def.nBufferCountActual);
	for (i = 0; i < port_def.nBufferCountActual; i++) {
		ret = OMX_FillThisBuffer(priv->rot, priv->rot_buffer[i]);
		OSCL_LOGI("OMX_FillThisBuffer %p on port %i", priv->rot_buffer[i],
			priv->rot_buffer[i]->nOutputPortIndex);
		oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	}

	/* 8. set component stat to executing */
	ret = OMX_SendCommand(priv->source, OMX_CommandStateSet,
			OMX_StateExecuting, NULL);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	sem_wait(&priv->sem_event);
	ret = OMX_SendCommand(priv->rot, OMX_CommandStateSet,
			OMX_StateExecuting, NULL);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	sem_wait(&priv->sem_event);
	ret = OMX_SendCommand(priv->sink, OMX_CommandStateSet, OMX_StateExecuting, NULL);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	sem_wait(&priv->sem_event);
#if 0
	sem_wait(&priv->sem_eos);
#else
	rt_thread_delay(1500); /* display 15s */
	OSCL_LOGD("quit test...");
#endif

	/* 6. set component stat to idle */
	ret = OMX_SendCommand(priv->sink, OMX_CommandStateSet, OMX_StateIdle, NULL);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	ret = OMX_SendCommand(priv->rot, OMX_CommandStateSet, OMX_StateIdle, NULL);
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
	ret = OMX_SendCommand(priv->rot, OMX_CommandStateSet, OMX_StateLoaded, NULL);
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

const pthread_attr_t rot_pthread_default_attr = {
	0,				/* stack base */
	4096,				/* stack size */
	(RT_THREAD_PRIORITY_MAX/2 + RT_THREAD_PRIORITY_MAX/4),	/* priority */
	PTHREAD_CREATE_JOINABLE,	/* detach state */
	SCHED_FIFO,			/* scheduler policy */
	PTHREAD_INHERIT_SCHED		/* Inherit parent prio/policy */
};

int openmax_test_rot_untunnel(void)
{
	int ret = OMX_ErrorNone;
	pthread_t thread_id = NULL;
	pthread_t thread_id1 = NULL;

	OSCL_LOGD("test start...");
	pthread_create(&thread_id,
			&rot_pthread_default_attr,
			untunnel_stream_process_thread,
			(void *)BACK_STREAM_ID);
	pthread_create(&thread_id1,
			&rot_pthread_default_attr,
			untunnel_stream_process_thread,
			(void *)ISP_PREV_STREAM_ID);
	OSCL_LOGD("Wait thread exit");
	if (thread_id)
		pthread_join(thread_id, NULL);
	if (thread_id1)
		pthread_join(thread_id1, NULL);
	OSCL_LOGD("Test complete....");

	return ret;
}

