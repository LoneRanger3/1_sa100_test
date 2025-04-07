/*
 * recorder_camera.c - Standard functionality for video stream recorder.
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

#define CAM_PRINT_VIDEO_INFO(video) \
OSCL_LOGI("eColorFormat:%d, w-h(%d, %d), (%d, %d)", (video).eColorFormat,\
	(video).nFrameWidth, (video).nFrameHeight, (video).nStride, (video).nSliceHeight);


/**
 * _camera_get_config - get default config for camera source.
 *
 * @vsrc_info: video source handle
 *
 */
static int _camera_get_config(vsrc_camera_t *camera)
{
	vsrc_info_t *vsrc_info;
	int ret = 0;

	OSCL_TRACE("==");
	oscl_param_check_exit(camera != NULL, -1, NULL);

	/* default config for rotate component */
	camera->vrot_info.mode.nRotation = VDISP_ROTATE_90;

	/* default config for rotate camera */
	vsrc_info = &camera->vsrc_info;
	vsrc_info->video.eColorFormat = REC_DEF_VIDEO_COLOR_FMT;
	if (REC_VIDEO_SIZE_DEFAULT == REC_VIDEO_SIZE_VGA) {
		vsrc_info->video.nFrameWidth = 640;
		vsrc_info->video.nFrameHeight = 480;
	} else if (REC_VIDEO_SIZE_DEFAULT == REC_VIDEO_SIZE_720P) {
		vsrc_info->video.nFrameWidth = 1280;
		vsrc_info->video.nFrameHeight = 720;
	} else if (REC_VIDEO_SIZE_DEFAULT == REC_VIDEO_SIZE_1080P) {
		vsrc_info->video.nFrameWidth = 1920;
		vsrc_info->video.nFrameHeight = 1080;
	} else {
		OSCL_LOGE("Do not supprot format.");
		vsrc_info->video.nFrameWidth = 1280;
		vsrc_info->video.nFrameHeight = 720;
	}
	vsrc_info->video.nStride = vsrc_info->video.nFrameWidth;
	vsrc_info->video.nSliceHeight = vsrc_info->video.nFrameHeight;
	vsrc_info->video.xFramerate = 0;
	vsrc_info->dev_name = oscl_strdup(REC_DEFAULT_VIDEO_SRC);
	vsrc_info->channel = REC_INPUT_CHANNEL_NUM;
	vsrc_info->vout->nbuffer = CAMERA_DEFAULT_BUF_NUM;

	CAM_PRINT_VIDEO_INFO(vsrc_info->video);
EXIT:
	OSCL_TRACE("==");
	return ret;
}


/**
 * _camera_creat_component - rotate component.
 *
 * @camera: camera handle
 *
 */
static int _camera_creat_component(vsrc_camera_t *camera)
{
	int ret = 0;
	int index;
	oscl_param_check_exit(camera != NULL, -1, NULL);

	OSCL_TRACE("==");
	/* init video source component */
	ret = al_component_init(&camera->vsrc_info.al_comp, "OMX.LB.SOURCE.VREC",
			&al_untunnel_common_callbacks);
	oscl_param_check_exit(ret == 0, ret, NULL);
	index = al_get_port_index(&camera->vsrc_info.al_comp, OMX_DirOutput,
			OMX_PortDomainVideo);
	oscl_param_check_exit(index >= 0, -1, NULL);
	camera->vsrc_info.vout = &camera->vsrc_info.al_comp.port_info[index];

	/* init video rotate component */
	ret = al_component_init(&camera->vrot_info.al_comp, "OMX.LB.VIDEO.ROT",
			&al_untunnel_common_callbacks);
	oscl_param_check_exit(ret == 0, ret, NULL);
	index = al_get_port_index(&camera->vrot_info.al_comp, OMX_DirInput,
			OMX_PortDomainVideo);
	oscl_param_check_exit(index >= 0, -1, NULL);
	camera->vrot_info.vin = &camera->vrot_info.al_comp.port_info[index];
	index = al_get_port_index(&camera->vrot_info.al_comp, OMX_DirOutput,
			OMX_PortDomainVideo);
	oscl_param_check_exit(index >= 0, -1, NULL);
	camera->vrot_info.vout = &camera->vrot_info.al_comp.port_info[index];

	/* init video splitter component */
	ret = al_component_init(&camera->vsplt_info.al_comp, "OMX.LB.SPLITTER.BASE",
			&al_untunnel_common_callbacks);
	oscl_param_check_exit(ret == 0, ret, NULL);
	camera->vsplt_info.in = &camera->vsplt_info.al_comp.port_info[0];
EXIT:
	OSCL_TRACE("==");
	return ret;
}

/**
 * _camera_creat_component - rotate component.
 *
 * @camera: camera handle
 *
 */
void _camera_release_component(vsrc_camera_t *camera)
{
	OSCL_TRACE("==");
	al_component_deinit(&camera->vsrc_info.al_comp);
	al_component_deinit(&camera->vsplt_info.al_comp);
	al_component_deinit(&camera->vrot_info.al_comp);
	OSCL_TRACE("==");
}

static int _vsrc_prepare(vsrc_info_t *vsrc_info)
{
	OMX_ERRORTYPE ret = 0;
	al_comp_info_t *al_comp;
	int index;
	OMX_PARAM_PORTDEFINITIONTYPE port_para;
	al_port_info_t *port_cfg;
	OMX_COMPONENTTYPE *cmp_hdl;
	OMX_PARAM_U32TYPE para;

	OSCL_TRACE("==");
	/* set config to component */
	al_comp = &vsrc_info->al_comp;
	cmp_hdl = al_comp->cmp_hdl;
	ret = OMX_SetParameter(cmp_hdl, OMX_IndexParamVideoInit,
			(OMX_PTR)vsrc_info->dev_name);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	OMX_GetParameter(cmp_hdl, OMX_IndexParamNumAvailableStreams, (OMX_PTR)&para);
	oscl_param_check_exit((para.nU32 > 0), ret, NULL);

	index = al_get_port_index(al_comp, OMX_DirOutput, OMX_PortDomainVideo);
	oscl_param_check_exit(index >= 0, -1, NULL);
	port_cfg = &al_comp->port_info[index];

	/* set outport info, buffersize = 0 to uncare */
	memset(&port_para, 0, sizeof(port_para));
	port_para.nVersion.nVersion = OMX_VERSION;
	port_para.nPortIndex = index;
	ret = OMX_GetParameter(cmp_hdl, OMX_IndexParamPortDefinition,
			&port_para);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);
	/* if (port_cfg->nbuffer > port_para.nBufferCountActual) */
	port_para.nBufferCountActual = port_cfg->nbuffer;
	memcpy(&port_para.format.video, &vsrc_info->video,
		sizeof(port_para.format.video));
	CAM_PRINT_VIDEO_INFO(port_para.format.video);

	ret = OMX_SetParameter(cmp_hdl, OMX_IndexParamPortDefinition, &port_para);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	ret = OMX_GetParameter(cmp_hdl, OMX_IndexParamPortDefinition, &port_para);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);
	memcpy(&vsrc_info->video, &port_para.format.video,
		sizeof(port_para.format.video));
	port_cfg->nbuffer = port_para.nBufferCountActual;
	port_cfg->buf_size = port_para.nBufferSize;
	CAM_PRINT_VIDEO_INFO(vsrc_info->video);

EXIT:
	OSCL_TRACE("==");
	return ret;
}

int _splt_prepare(splt_info_t *splt_info, OMX_PARAM_PORTDEFINITIONTYPE *port)
{
	int i = 0;
	int ret = 0;
	al_port_info_t *al_port;

	OSCL_TRACE("==");
	for (i = 0; i < splt_info->al_comp.num_port; i++) {
		al_port = &splt_info->al_comp.port_info[i];
		al_port->domain = port->eDomain;
		OSCL_LOGI("nbuffer(%d) edir(%d) domain(%d) size(%d)!",
			al_port->nbuffer,
			al_port->edir,
			al_port->domain,
			al_port->buf_size);
		if (al_port->edir == OMX_DirInput) {
			port->eDir = OMX_DirInput;
			port->nPortIndex = i;
			ret = OMX_SetParameter(splt_info->al_comp.cmp_hdl,
					OMX_IndexParamPortDefinition, port);
			OSCL_LOGI("%d buf:%d", i, port->nBufferCountActual);
			CAM_PRINT_VIDEO_INFO(port->format.video);
			break;
		}
	}
	for (i = 0; i < splt_info->al_comp.num_port; i++) {
		al_port = &splt_info->al_comp.port_info[i];
		al_port->domain = port->eDomain;
		port->nPortIndex = i;
		ret = OMX_GetParameter(splt_info->al_comp.cmp_hdl,
				OMX_IndexParamPortDefinition, port);
		al_port->nbuffer = port->nBufferCountActual;
		al_port->buf_size = port->nBufferSize;
		if (al_port->edir == OMX_DirOutput)
			al_port->is_shared_buffer = 1;
		OSCL_LOGI("splt_info->al_comp.cmp_hdl:%x, %d",
			splt_info->al_comp.cmp_hdl,
			port->nPortIndex);
		OSCL_LOGI("%d buf:%d", i, port->nBufferCountActual);
		CAM_PRINT_VIDEO_INFO(port->format.video);
	}
	OSCL_TRACE("==");
	return ret;
}

int _camera_prepare(vsrc_camera_t *camera)
{
	OMX_PARAM_PORTDEFINITIONTYPE omx_port;
	int ret = 0;

	OSCL_TRACE("==");
	if (camera->vsrc_info.al_comp.state != OMX_StateLoaded)
		goto EXIT;

	/* setup para for recorder components*/
	ret = _vsrc_prepare(&camera->vsrc_info);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	omx_port.nPortIndex = camera->vsrc_info.vout->index;
	omx_port.nVersion.nVersion = OMX_VERSION;
	ret = OMX_GetParameter(camera->vsrc_info.al_comp.cmp_hdl,
			OMX_IndexParamPortDefinition, &omx_port);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	/* set port definition to rotate component */
	if (camera->vrot_info.vin) {
		omx_port.eDir = camera->vrot_info.vin->edir;
		omx_port.nPortIndex = camera->vrot_info.vin->index;
		ret = OMX_SetParameter(camera->vrot_info.al_comp.cmp_hdl,
				OMX_IndexParamPortDefinition, &omx_port);
		CAM_PRINT_VIDEO_INFO(omx_port.format.video);
	}

	/* set port definition to splitter component */
	_splt_prepare(&camera->vsplt_info, &omx_port);

	/* connect video source with video splitter */
	ret = al_untunnel_setup_ports(camera->vsrc_info.vout,
			camera->vsplt_info.in);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	al_component_setstate(&camera->vsplt_info.al_comp, OMX_StateIdle);
	al_component_setstate(&camera->vsrc_info.al_comp, OMX_StateIdle);
	al_component_setstate(&camera->vsplt_info.al_comp, OMX_StateExecuting);
	al_component_setstate(&camera->vsrc_info.al_comp, OMX_StateExecuting);

EXIT:
	if (ret != 0)
		OSCL_LOGE("vsrc_camera_prepare error! ret:%x", ret);
	OSCL_TRACE("prepare end! ret:%x", ret);
	return ret;

}

int _camera_reset(vsrc_camera_t *camera)
{
	OSCL_TRACE("==");
	if (camera->vsplt_map != 0 || camera->rotate_map != 0) {
		OSCL_LOGE("ports busy! %x-%x", camera->vsplt_map, camera->rotate_map);
		goto EXIT;
	}
	if (camera->vsplt_info.al_comp.state == OMX_StateExecuting) {
		al_component_setstate(&camera->vsplt_info.al_comp, OMX_StateIdle);
		al_component_setstate(&camera->vsrc_info.al_comp, OMX_StateIdle);
		al_untunnel_unset_ports(camera->vsrc_info.vout,
				camera->vsplt_info.in);
	}
	if (camera->vsplt_info.al_comp.state == OMX_StateIdle) {
		al_component_setstate(&camera->vsplt_info.al_comp, OMX_StateLoaded);
		al_component_setstate(&camera->vsrc_info.al_comp, OMX_StateLoaded);
	}
EXIT:
	OSCL_TRACE("exit");
	return 0;

}

static void _camera_set_prio(vsrc_camera_t *camera)
{
	OMX_PRIORITYMGMTTYPE priority;

	OSCL_TRACE("==");
	priority.nVersion.nVersion = OMX_VERSION;
	priority.nGroupPriority = LB_RECORDER_VSRC_PRIO;
	OMX_SetParameter(camera->vsrc_info.al_comp.cmp_hdl,
		OMX_IndexParamPriorityMgmt, &priority);

	priority.nGroupPriority = LB_RECORDER_VSPLIT_PRIO;
	OMX_SetParameter(camera->vsplt_info.al_comp.cmp_hdl,
		OMX_IndexParamPriorityMgmt, &priority);

	priority.nGroupPriority = LB_RECORDER_VSPLIT_PRIO;
	OMX_SetParameter(camera->vrot_info.al_comp.cmp_hdl,
		OMX_IndexParamPriorityMgmt, &priority);
	OSCL_TRACE("==");
}

al_port_info_t *_camera_get_spltport(vsrc_camera_t *camera)
{
	al_port_info_t *port = NULL;
	al_port_info_t *splt_port = NULL;
	int nports;
	int index;
	OSCL_TRACE("==");
	oscl_param_check(camera != NULL, NULL, NULL);
	splt_port = camera->vsplt_info.al_comp.port_info;
	nports = camera->vsplt_info.al_comp.num_port;
	for (index = 0; index < nports; index++) {
		if (splt_port[index].edir == OMX_DirInput)
			continue;
		if ((camera->vsplt_map & (1 << index)) == 0) {
			camera->vsplt_map |= (1 << index);
			port = &splt_port[index];
			break;
		}
	}

	OSCL_LOGI("=nports:%d, index:%d, port:%x=", nports, index, port);
	return port;
}

int _camera_put_spltport(vsrc_camera_t *camera, al_port_info_t *port)
{
	al_port_info_t *splt_port = NULL;

	OSCL_TRACE("==");
	oscl_param_check(camera != NULL, -1, NULL);
	oscl_param_check(port != NULL, -1, NULL);

	splt_port = camera->vsplt_info.al_comp.port_info;
	if (port == &splt_port[port->index]) {
		if ((camera->vsplt_map & (1 << port->index)) == 0)
			OSCL_LOGE("free a port already freed");
		else
			camera->vsplt_map &= ~(1 << port->index);
	}
	OSCL_TRACE("==");
	return 0;
}

int _camera_enable_spltport(vsrc_camera_t *camera, al_port_info_t *port)
{
	int ret;

	OSCL_TRACE("==");
	oscl_param_check(camera != NULL, -1, NULL);
	oscl_param_check(port != NULL, -1, NULL);

	ret = al_component_sendcom(&camera->vsplt_info.al_comp, OMX_CommandPortEnable,
				port->index, NULL);
	OSCL_TRACE("==");
	return ret;
}

int _camera_disable_spltport(vsrc_camera_t *camera, al_port_info_t *port)
{
	int ret;

	OSCL_TRACE("==");
	oscl_param_check(camera != NULL, -1, NULL);
	oscl_param_check(port != NULL, -1, NULL);

	ret = al_component_sendcom(&camera->vsplt_info.al_comp, OMX_CommandPortDisable,
		port->index, NULL);
	OSCL_TRACE("==");
	return ret;
}

int _camera_enable_rotport(vsrc_camera_t *camera, al_port_info_t *port)
{
	int ret = 0;

	OSCL_TRACE("==");
	oscl_param_check(camera != NULL, -1, NULL);
	oscl_param_check(port != NULL, -1, NULL);
	oscl_param_check(camera->splt_rot != NULL, -1, NULL);

	al_component_setstate(&camera->vrot_info.al_comp, OMX_StateExecuting);
	_camera_enable_spltport(camera, camera->splt_rot);

	OSCL_TRACE("==");
	return ret;
}

int _camera_disable_rotport(vsrc_camera_t *camera, al_port_info_t *port)
{
	OSCL_TRACE("==");
	oscl_param_check(camera != NULL, -1, NULL);
	oscl_param_check(port != NULL, -1, NULL);
	oscl_param_check(camera->splt_rot != NULL, -1, NULL);

	_camera_disable_spltport(camera, camera->splt_rot);
	al_component_setstate(&camera->vrot_info.al_comp, OMX_StateIdle);
	OSCL_TRACE("==");
	return 0;
}

void vsrc_camera_set_buffer_num(vsrc_camera_t *camera, int num)
{
	if (camera)
		camera->vsrc_info.vout->nbuffer = num;
}

/**
 * vsrc_camera_putport - get a free port from camera.
 *
 * @camera: camera handle
 *
 * @mode: rotate mode of port to get
 *
 * Returns port handle on success, NULL otherwise..
 */
al_port_info_t *vsrc_camera_getport(vsrc_camera_t *camera, vdisp_rotate_mode_e mode)
{
	al_port_info_t *port = NULL;
	int ret = 0;

	OSCL_TRACE("==");
	oscl_param_check(camera != NULL, NULL, NULL);

	pthread_mutex_lock(camera->lock);
	if (mode == VDISP_ROTATE_NONE) {
		port = _camera_get_spltport(camera);
		goto EXIT;
	}
	if (camera->rotate_map != 0) {
		OSCL_LOGE("current only support one rotate port!");
		port = NULL;
		goto EXIT;
	}
	camera->vrot_info.mode.nRotation = mode;
	camera->splt_rot = _camera_get_spltport(camera);
	oscl_param_check_exit(camera->splt_rot != NULL, -1, NULL);
	ret = al_untunnel_setup_ports(camera->splt_rot, camera->vrot_info.vin);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	/* command OMX_IndexConfigCommonRotate must be set after setup_ports,
	 * because setup_ports will set portdefinition to inport and
	 * OMX_IndexConfigCommonRotate will caculate output para dependson inport para*/
	OMX_SetParameter(camera->vrot_info.al_comp.cmp_hdl,
			OMX_IndexConfigCommonRotate,
			&camera->vrot_info.mode);
	al_component_setstate(&camera->vrot_info.al_comp, OMX_StateIdle);
	port = camera->vrot_info.vout;

EXIT:
	pthread_mutex_unlock(camera->lock);
	if (ret != 0)
		port = NULL;
	OSCL_LOGI("camera:%x get port:%x, index:%d",
		camera, port, port ? port->index : 0);
	return port;
}

/**
 * vsrc_camera_putport - free a port get from camera.
 *
 * @camera: camera handle
 *
 * @port: port to be freed
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int vsrc_camera_putport(vsrc_camera_t *camera, al_port_info_t *port)
{
	int ret = 0;

	OSCL_TRACE("==");
	oscl_param_check(camera != NULL, -1, NULL);
	oscl_param_check(port != NULL, -1, NULL);

	pthread_mutex_lock(camera->lock);
	if (port == camera->vrot_info.vout) {
		ret = _camera_disable_rotport(camera, port);
		al_untunnel_unset_ports(camera->splt_rot, camera->vrot_info.vin);
		al_component_setstate(&camera->vrot_info.al_comp, OMX_StateLoaded);
		camera->rotate_map = 0;
		_camera_put_spltport(camera, camera->splt_rot);
		camera->splt_rot = NULL;
	} else
		ret = _camera_put_spltport(camera, port);
	pthread_mutex_unlock(camera->lock);

	OSCL_LOGI("camera put ports, %s(%d)", al_port_name(port), port->index);
	return ret;
}

/**
 * vsrc_camera_getpara_portinfo - get port info from camera.
 *
 * @camera: camera handle
 * @port: port handle
 * @port: output port info
 *
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int vsrc_camera_getpara_portinfo(vsrc_camera_t *camera,
	al_port_info_t *port, OMX_PARAM_PORTDEFINITIONTYPE *portinfo)
{
	int ret = 0;
	OMX_COMPONENTTYPE *cmp_hdl = NULL;

	OSCL_TRACE("==");
	oscl_param_check(camera != NULL, -1, NULL);
	oscl_param_check(port != NULL, -1, NULL);
	oscl_param_check(portinfo != NULL, -1, NULL);

	memset(portinfo, 0, sizeof(OMX_PARAM_PORTDEFINITIONTYPE));
	pthread_mutex_lock(camera->lock);
	if (port == camera->vrot_info.vout)
		cmp_hdl = camera->vrot_info.al_comp.cmp_hdl;
	else if (port == &camera->vsplt_info.al_comp.port_info[port->index])
		cmp_hdl = camera->vsplt_info.al_comp.cmp_hdl;
	else
		OSCL_LOGE("invalid port!");
	oscl_param_check_exit(cmp_hdl != NULL, -1, NULL);
	portinfo->nPortIndex = port->index;
	portinfo->nVersion.nVersion = OMX_VERSION;
	ret = OMX_GetParameter(cmp_hdl, OMX_IndexParamPortDefinition, portinfo);

EXIT:
	pthread_mutex_unlock(camera->lock);

	OSCL_LOGI("camera put ports, %s(%d)", al_port_name(port), port->index);
	return ret;
}


/**
 * vsrc_camera_enable_port - enable a port get from camera.
 *
 * @camera: camera handle
 *
 * @port: port to be enabled
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int vsrc_camera_enable_port(vsrc_camera_t *camera, al_port_info_t *port)
{
	int ret = -1;

	OSCL_TRACE("==");
	oscl_param_check(camera != NULL, -1, NULL);
	pthread_mutex_lock(camera->lock);
	if (port != camera->vrot_info.vout)
		ret = _camera_enable_spltport(camera, port);
	else
		ret = _camera_enable_rotport(camera, port);
	pthread_mutex_unlock(camera->lock);

	OSCL_TRACE("==");
	return ret;
}

/**
 * vsrc_camera_disable_port - disable a port get from camera.
 *
 * @camera: camera handle
 *
 * @port: port to be disabled
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int vsrc_camera_disable_port(vsrc_camera_t *camera, al_port_info_t *port)
{
	int ret = 0;

	OSCL_TRACE("==");
	oscl_param_check(camera != NULL, -1, NULL);
	oscl_param_check(port != NULL, -1, NULL);

	pthread_mutex_lock(camera->lock);
	if (port != camera->vrot_info.vout)
		ret = _camera_disable_spltport(camera, port);
	else
		ret = _camera_disable_rotport(camera, port);
	pthread_mutex_unlock(camera->lock);

	OSCL_TRACE("==");
	return ret;
}

/**
 * video_rec_set_video_source - set source device of recorder.
 *
 * @video_rec: video recorder handle
 * @src_name: device name
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int vsrc_camera_set_device(vsrc_camera_t *camera, char *src_name)
{
	int ret = 0;

	OSCL_TRACE("==");
	oscl_param_check(camera != NULL, -1, NULL);
	oscl_param_check(src_name != NULL, -1, NULL);

	pthread_mutex_lock(camera->lock);
	if (camera->vsplt_map != 0 || camera->rotate_map != 0) {
		OSCL_LOGE("ports busy! %x-%x", camera->vsplt_map, camera->rotate_map);
		goto EXIT;
	}
	if (strcmp(camera->vsrc_info.dev_name, src_name) == 0) {
		OSCL_LOGI("dev name not change! %s", src_name);
		goto EXIT;
	}

	if (camera->vsrc_info.dev_name) {
		OSCL_LOGI("switch video source:%s to %s",
			camera->vsrc_info.dev_name, src_name);
		oscl_free(camera->vsrc_info.dev_name);
	}
	camera->vsrc_info.dev_name = oscl_strdup(src_name);
	oscl_param_check_exit(camera->vsrc_info.dev_name != NULL,
		OMX_ErrorInsufficientResources, NULL);
	if (camera->vsrc_info.al_comp.state == OMX_StateLoaded)
		goto EXIT;
	ret = _camera_reset(camera);
	oscl_param_check_exit(ret == 0, -1, NULL);
EXIT:
	OSCL_TRACE("==");
	pthread_mutex_unlock(camera->lock);
	return ret;
}

/**
 * vsrc_camera_set_para - set para to camera.
 *
 * @camera: camera handle
 * @rec_para: recorder para
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int vsrc_camera_set_para(vsrc_camera_t *camera, rec_param_t *rec_para)
{
	OSCL_TRACE("start");
	oscl_param_check(camera != NULL, -1, NULL);
	oscl_param_check(rec_para != NULL, -1, NULL);

	pthread_mutex_lock(camera->lock);
	if (rec_para->source_height) {
		camera->vsrc_info.video.nFrameHeight = rec_para->source_height;
		camera->vsrc_info.video.nSliceHeight = rec_para->source_height;
	}
	if (rec_para->source_width) {
		camera->vsrc_info.video.nFrameWidth = rec_para->source_width;
		camera->vsrc_info.video.nStride = rec_para->source_width;
	}
	if (rec_para->frame_rate)
		camera->vsrc_info.video.xFramerate = rec_para->frame_rate;
	pthread_mutex_unlock(camera->lock);

	OSCL_TRACE("end!");
	return 0;
}

/**
 * vsrc_camera_get_time - get current time.
 *
 * @camera: camera handle
 *
 * Returns 0 on success, -EERROR otherwise..
 */
s64 vsrc_camera_get_time(vsrc_camera_t *camera)
{
	OMX_TIME_CONFIG_TIMESTAMPTYPE time;
	OSCL_TRACE("==");
	oscl_param_check(camera != NULL, -1, NULL);

	pthread_mutex_lock(camera->lock);
	time.nTimestamp = -1;
	OMX_GetConfig(camera->vsrc_info.al_comp.cmp_hdl,
			omx_index_lombo_config_cur_time, &time);
	OSCL_LOGI("start time:%d", time.nTimestamp);
	if (camera->vsrc_info.al_comp.state != OMX_StateExecuting)
		OSCL_LOGE("set start time while video source not excuting");
	pthread_mutex_unlock(camera->lock);
	OSCL_TRACE("==");
	return time.nTimestamp;
}

/**
 * vsrc_camera_get_para - get para of camera.
 *
 * @camera: camera handle
 * @rec_para: recorder para
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int vsrc_camera_get_para(vsrc_camera_t *camera, rec_param_t *rec_para)
{
	OSCL_TRACE("start");
	oscl_param_check(camera != NULL, -1, NULL);
	oscl_param_check(rec_para != NULL, -1, NULL);

	pthread_mutex_lock(camera->lock);
	rec_para->source_height = camera->vsrc_info.video.nFrameHeight;
	rec_para->source_width = camera->vsrc_info.video.nFrameWidth;
	rec_para->frame_rate = camera->vsrc_info.video.xFramerate;
	pthread_mutex_unlock(camera->lock);

	OSCL_TRACE("end!");
	return 0;
}


/**
 * vsrc_camera_prepare - set para to components.
 *
 * @camera: camera handle
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int vsrc_camera_prepare(vsrc_camera_t *camera)
{
	int ret = 0;

	OSCL_TRACE("==");
	oscl_param_check_exit(camera != NULL, -1, NULL);

	pthread_mutex_lock(camera->lock);
	ret = _camera_prepare(camera);
	pthread_mutex_unlock(camera->lock);
EXIT:
	if (ret != 0)
		OSCL_LOGE("vsrc_camera_prepare error! ret:%x", ret);

	OSCL_TRACE("prepare end! ret:%x", ret);
	OSCL_TRACE("==");
	return ret;
}

/**
 * vsrc_camera_release - release a camera source.
 *
 * @camera: camera handle
 *
 * Returns 0 on success, -EERROR otherwise..
 */
void vsrc_camera_release(vsrc_camera_t *camera)
{
	OSCL_TRACE("==");
	if (camera == NULL || camera->lock == NULL)
		goto EXIT;
	pthread_mutex_lock(camera->lock);
	_camera_reset(camera);
	_camera_release_component(camera);
	pthread_mutex_unlock(camera->lock);

	if (camera->vsrc_info.dev_name != NULL)
		oscl_free(camera->vsrc_info.dev_name);
EXIT:
	if (camera->lock) {
		oscl_free(camera->lock);
		camera->lock = NULL;
	}
	if (camera)
		oscl_free(camera);
	OSCL_TRACE("==");
	return;
}

/**
 * vsrc_camera_creat - create a camera source.
 *
 * @camera: camera handle
 *
 * Returns 0 on success, -EERROR otherwise..
 */
vsrc_camera_t *vsrc_camera_creat(void)
{
	int ret = -1;
	vsrc_camera_t *camera;
	OSCL_TRACE("==");

	camera = oscl_zalloc(sizeof(vsrc_camera_t));
	oscl_param_check_exit(camera != NULL, -1, NULL);

	camera->lock = oscl_zalloc(sizeof(pthread_mutex_t));
	oscl_param_check_exit(camera->lock != NULL, -1, NULL);
	pthread_mutex_init(camera->lock, NULL);

	pthread_mutex_lock(camera->lock);
	ret = _camera_creat_component(camera);
	oscl_param_check_exit(ret == 0, ret, NULL);
	_camera_get_config(camera);
	_camera_set_prio(camera);
EXIT:
	if (camera && camera->lock)
		pthread_mutex_unlock(camera->lock);
	if (ret != 0) {
		vsrc_camera_release(camera);
		camera = NULL;
	}
	OSCL_TRACE("==");
	return camera;
}

cam_manager_t cam_manager;

int cam_manager_init(void)
{
	pthread_mutex_init(&cam_manager.lock, NULL);
	return 0;
}


void *cam_manager_get_device(char *name)
{
	int i;
	vsrc_camera_t *hdl = NULL;
	char *dev;
	oscl_param_check(name != NULL, NULL, NULL);

	pthread_mutex_lock(&cam_manager.lock);
	for (i = 0; (i < MAX_CAM_OPENED) && (hdl == NULL); i++) {
		if (cam_manager.cam_hdl[i] != NULL) {
			pthread_mutex_lock(cam_manager.cam_hdl[i]->lock);
			dev = cam_manager.cam_hdl[i]->vsrc_info.dev_name;
			if (strcmp(dev, name) == 0) {
				hdl = cam_manager.cam_hdl[i];
				cam_manager.refcnt[i]++;
			}
			pthread_mutex_unlock(cam_manager.cam_hdl[i]->lock);
		}
	}
	if (hdl == NULL) {
		OSCL_LOGI("device %s is not in camera manager!", name);
		hdl = vsrc_camera_creat();
		vsrc_camera_set_device(hdl, name);
		for (i = 0; i < MAX_CAM_OPENED; i++) {
			if (cam_manager.cam_hdl[i] == NULL) {
				cam_manager.cam_hdl[i] = hdl;
				cam_manager.refcnt[i] = 1;
				break;
			}
		}
		if (i == MAX_CAM_OPENED) {
			OSCL_LOGE("opened camera > MAX_CAM_OPENED, check it!");
			vsrc_camera_release(hdl);
			hdl = NULL;
		}
	}
	OSCL_LOGI("device %s hdl:%p!", name, hdl);
	pthread_mutex_unlock(&cam_manager.lock);
	return hdl;
}

void cam_manager_put_device(void *hdl)
{
	int i;
	int refcnt = 0;

	if (hdl == NULL)
		return;
	pthread_mutex_lock(&cam_manager.lock);
	for (i = 0; i < MAX_CAM_OPENED; i++) {
		if (cam_manager.cam_hdl[i] == hdl) {
			cam_manager.refcnt[i]--;
			refcnt = cam_manager.refcnt[i];
			if (cam_manager.refcnt[i] == 0)
				cam_manager.cam_hdl[i] = NULL;
			break;
		}
	}
	if (i == MAX_CAM_OPENED)
		OSCL_LOGE("removed hdl cannot find in camera manager!");
	if (refcnt == 0)
		vsrc_camera_release(hdl);
	pthread_mutex_unlock(&cam_manager.lock);
	return;
}


INIT_ENV_EXPORT(cam_manager_init);

