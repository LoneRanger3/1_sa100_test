/*
 * omx_core.c - Standard functionality for the openmax core API.
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
#include <lb_omx_core.h>
#include "vrec/oscl_vrec.h"
#include "oscl_cache_file.h"


static pthread_mutex_t omx_lock = {.attr = -1};
omx_component_tbl_t omx_commponents[MAX_TABLE_SIZE] = {
	{
		"OMX.LB.SPLITTER.BASE",
		1,
		splitter_component_init, NULL, NULL,
		{
			"splitter",
			NULL
		}
	},
	{
		"OMX.LB.SOURCE.VREC",
		1,
		vrec_component_init, NULL, NULL,
		{
			"source.vreceive",
			NULL
		}
	},
	{
		"OMX.LB.VIDEO.ENCODECOMPONENT",
		1,
		video_encoder_component_init, NULL, NULL,
		{
			"video_encoder.jpeg",
			"video_encoder.h264",
			NULL
		}
	},
	{
		"OMX.LB.SINK.VRENDER",
		1,
		vrender_component_init, NULL, NULL,
		{
			"sink.vrender",
			NULL
		}
	},
	{
		"OMX.LB.SINK.MUXER",
		1,
		muxer_component_init, NULL, NULL,
		{
			"muxer.mp4",
			NULL
		}
	},
	{
		"OMX.LB.SOURCE.AREC",
		1,
		arec_component_init, NULL, NULL,
		{
			"source.mic",
			NULL
		}
	},
	{
		"OMX.LB.ENCODER.AUDIO",
		1,
		aenc_component_init, NULL, NULL,
		{
			"encoder.aac",
			"encoder.pcm",
			NULL
		}
	},
	{
		"OMX.LB.VIDEO.ROT",
		1,
		rotate_component_init, NULL, NULL,
		{
			"video.rot",
			NULL
		}
	},
	{
		"OMX.LB.VIDEO.PANO",
		1,
		pano_component_init, NULL, NULL,
		{
			"video.pano",
			NULL
		}
	},
	{
		"OMX.LB.SINK.ARENDER",
		1,
		arender_component_init, NULL, NULL,
		{
			"sink.arender",
			NULL
		}
	},
	{
		"OMX.LB.VIDEO.H264DEC",
		1,
		h264dec_component_init, NULL, NULL,
		{
			"video_decoder.h264",
			NULL
		}
	},
	{
		"OMX.LB.SOURCE.DEMUXER",
		1,
		demuxer_component_init, NULL, NULL,
		{
			"source.demuxer",
			NULL
		}
	},
	{
		"OMX.LB.SOURCE.ADEC",
		1,
		adec_component_init, NULL, NULL,
		{
			"source.adec",
			NULL
		}
	},
	{
		"OMX.LB.SOURCE.CLOCK",
		1,
		clock_component_init, NULL, NULL,
		{
			"source.clock",
			NULL
		}
	},
};
#define SIZE_OF_COMPONENTS (sizeof(omx_commponents)/sizeof(omx_component_tbl_t))

static OMX_HANDLETYPE active_handle[OMX_COMP_MAX_INST];
static OMX_S32 omx_init_cnt;
static void *cam_hdl[3];

static OMX_ERRORTYPE __save_handle(OMX_HANDLETYPE handle)
{
	int i;
	OMX_ERRORTYPE ret = OMX_ErrorInsufficientResources;
	for (i = 0; i < OMX_COMP_MAX_INST; i++) {
		if (active_handle[i] == NULL) {
			active_handle[i] = handle;
			ret = OMX_ErrorNone;
			break;
		}
	}
	return ret;
}

static OMX_ERRORTYPE __remove_handle(OMX_HANDLETYPE handle)
{
	int i;
	OMX_ERRORTYPE ret = OMX_ErrorBadParameter;
	for (i = 0; i < OMX_COMP_MAX_INST; i++) {
		if (active_handle[i] == handle) {
			active_handle[i] = NULL;
			ret = OMX_ErrorNone;
			break;
		}
	}
	OSCL_LOGI("index:%d, ret:%x", i, ret);
	return ret;
}


/**
 * _get_index - Obtains the index associated with the name.
 * @name: component name
 */
static int _get_index(char *name)
{
	int i;
	int index = -1;

	for (i = 0; i < (int)SIZE_OF_COMPONENTS; i++) {
		if (omx_commponents[i].reg_flag != 1)
			continue;
		OSCL_LOGD("cmp_name = %s , table[i].name = %s , i = %d",
			name, omx_commponents[i].name, i);
		if (!strcmp(name, omx_commponents[i].name)) {
			index = i;
			break;
		}
	}
	OSCL_TRACE("component index:%d", index);
	return index;
}



/**
 * The OMX_Init method is used to initialize the OMX core. It shall be the
 * first call made into OMX and it should only be executed one time without
 * an interviening OMX_Deinit call.
 *
 * The core should return from this call within 20 msec.
 *
 * @return OMX_ERRORTYPE
 *       If the command successfully executes, the return code will be
 *       OMX_ErrorNone.  Otherwise the appropriate OMX error will be returned.
 * @ingroup core
 */
OMX_API OMX_ERRORTYPE OMX_APIENTRY OMX_Init(void)
{
	OSCL_TRACE("OMXCORE API - OMX_Init");
	if (omx_init_cnt == 0) {
		pthread_mutex_init(&omx_lock, NULL);
		cam_hdl[0] = oscl_open_vrec("vic");
		cam_hdl[1] = oscl_open_vrec("isp_cap.0");
		cam_hdl[2] = oscl_open_vrec("isp_prev.0");
		oscl_cache_fs_init();
	}
	omx_init_cnt++;

	return OMX_ErrorNone;
}



/**
 * The OMX_Deinit method is used to deinitialize the OMX core.  It shall be
 * the last call made into OMX. In the event that the core determines that
 * thare are components loaded when this call is made, the core may return
 * with an error rather than try to unload the components.
 *
 * The core should return from this call within 20 msec.
 *
 * @return OMX_ERRORTYPE
 *     If the command successfully executes, the return code will be
 *     OMX_ErrorNone.  Otherwise the appropriate OMX error will be returned.
 * @ingroup core
 */
OMX_API OMX_ERRORTYPE OMX_APIENTRY OMX_Deinit(void)
{
	OMX_S32 i = 0;

	OSCL_TRACE("OMXCORE API - OMX_Deinit");
	omx_init_cnt--;
	if (omx_init_cnt <= 0) {
		omx_init_cnt = 0;
		pthread_mutex_destroy(&omx_lock);
		oscl_cache_fs_deinit();
		for (i = 0; i < (sizeof(cam_hdl) / sizeof(cam_hdl[0])); i++) {
			if (cam_hdl[i])
				oscl_close_vrec(cam_hdl[i]);
			cam_hdl[i] = NULL;
		}
	}
	return OMX_ErrorNone;
}


/**
 * The OMX_ComponentNameEnum method will enumerate through all the names of
 * recognised valid components in the system. This function is provided
 * as a means to detect all the components in the system run-time. There is
 * no strict ordering to the enumeration order of component names, although
 * each name will only be enumerated once.  If the OMX core supports run-time
 * installation of new components, it is only requried to detect newly
 * installed components when the first call to enumerate component names
 * is made (i.e. when nIndex is 0x0).
 *
 * The core should return from this call in 20 msec.
 *
 * @param [out] cComponentName
 *     pointer to a null terminated string with the component name.  The
 *     names of the components are strings less than 127 bytes in length
 *     plus the trailing null for a maximum size of 128 bytes.  An example
 *     of a valid component name is "OMX.TI.AUDIO.DSP.MIXER\0".  Names are
 *     assigned by the vendor, but shall start with "OMX." and then have
 *     the Vendor designation next.
 * @param [in] nNameLength
 *     number of characters in the cComponentName string.  With all
 *     component name strings restricted to less than 128 characters
 *     (including the trailing null) it is recomended that the caller
 *     provide a input string for the cComponentName of 128 characters.
 * @param [in] nIndex
 *     number containing the enumeration index for the component.
 *     Multiple calls to OMX_ComponentNameEnum with increasing values
 *     of nIndex will enumerate through the component names in the
 *     system until OMX_ErrorNoMore is returned.  The value of nIndex
 *     is 0 to (N-1), where N is the number of valid installed components
 *     in the system.
 * @return OMX_ERRORTYPE
 *     If the command successfully executes, the return code will be
 *     OMX_ErrorNone.  When the value of nIndex exceeds the number of
 *     components in the system minus 1, OMX_ErrorNoMore will be
 *     returned. Otherwise the appropriate OMX error will be returned.
 * @ingroup core
 */
OMX_API OMX_ERRORTYPE OMX_APIENTRY OMX_ComponentNameEnum(
	OMX_OUT OMX_STRING cComponentName,
	OMX_IN OMX_U32 nNameLength,
	OMX_IN OMX_U32 nIndex)
{
	OMX_ERRORTYPE ret = OMX_ErrorNoMore;
	OSCL_TRACE("OMXCORE API - OMX_ComponentNameEnum %p %d %d\n",
		cComponentName, (unsigned)nNameLength, (unsigned)nIndex);
	oscl_param_check((cComponentName != NULL), OMX_ErrorBadParameter, NULL);
	if (nIndex < SIZE_OF_COMPONENTS && (omx_commponents[nIndex].fn_init != NULL)) {
		strncpy(cComponentName, omx_commponents[nIndex].name, nNameLength);
		cComponentName[nNameLength - 1] = '\0';
		ret = OMX_ErrorNone;
	}
	return ret;
}

/**
 * The OMX_GetHandle method will locate the component specified by the
 * component name given, load that component into memory and then invoke
 * the component's methods to create an instance of the component.
 *
 * The core should return from this call within 20 msec.
 *
 * @param [out] pHandle
 *     pointer to an OMX_HANDLETYPE pointer to be filled in by this method.
 * @param [in] cComponentName
 *     pointer to a null terminated string with the component name.  The
 *     names of the components are strings less than 127 bytes in length
 *     plus the trailing null for a maximum size of 128 bytes.  An example
 *     of a valid component name is "OMX.TI.AUDIO.DSP.MIXER\0".  Names are
 *     assigned by the vendor, but shall start with "OMX." and then have
 *     the Vendor designation next.
 * @param [in] pAppData
 *     pointer to an application defined value that will be returned
 *     during callbacks so that the application can identify the source
 *     of the callback.
 * @param [in] pCallBacks
 *     pointer to a OMX_CALLBACKTYPE structure that will be passed to the
 *     component to initialize it with.
 * @return OMX_ERRORTYPE
 *     If the command successfully executes, the return code will be
 *     OMX_ErrorNone.  Otherwise the appropriate OMX error will be returned.
 * @ingroup core
 */
OMX_API OMX_ERRORTYPE OMX_APIENTRY OMX_GetHandle(
	OMX_OUT OMX_HANDLETYPE *pHandle,
	OMX_IN OMX_STRING cComponentName,
	OMX_IN OMX_PTR pAppData,
	OMX_IN OMX_CALLBACKTYPE *pCallBacks)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	int cmp_index = -1;
	lb_omx_component_t *cmp_handle = NULL;

	OSCL_LOGI("OMXCORE API : GetHandle %p %s %p\n", pHandle,
		cComponentName,
		pAppData);
	oscl_param_check(pHandle != NULL, OMX_ErrorBadParameter, NULL);

	pthread_mutex_lock(&omx_lock);
	cmp_index = _get_index(cComponentName);
	oscl_param_check_exit(
		((cmp_index >= 0) && omx_commponents[cmp_index].fn_init != NULL),
		OMX_ErrorNotImplemented, NULL);

	cmp_handle = oscl_zalloc(sizeof(lb_omx_component_t));
	oscl_param_check_exit(cmp_handle != NULL,
		OMX_ErrorInsufficientResources, NULL);
	memset(cmp_handle, 0, sizeof(lb_omx_component_t));
	ret = (omx_commponents[cmp_index].fn_init)(cmp_handle);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, "Component init failed!\n");

	if (cmp_handle->base_comp.SetCallbacks)
		cmp_handle->base_comp.SetCallbacks(&cmp_handle->base_comp,
			pCallBacks, pAppData);

	*pHandle = &(cmp_handle->base_comp);

	ret = __save_handle(*pHandle);
	oscl_param_check_exit(ret == OMX_ErrorNone,
		OMX_ErrorInsufficientResources, NULL);
	OSCL_LOGI("Component %p created\n", *pHandle);
EXIT:
	pthread_mutex_unlock(&omx_lock);
	if ((ret != OMX_ErrorNone) && (cmp_handle != NULL)) {
		if (cmp_handle->base_comp.ComponentDeInit)
			cmp_handle->base_comp.ComponentDeInit(&cmp_handle->base_comp);
		oscl_free(cmp_handle);
	}
	return ret;
}


/**
 * The OMX_FreeHandle method will free a handle allocated by the OMX_GetHandle
 * method.  If the component reference count goes to zero, the component will
 * be unloaded from memory.
 *
 * The core should return from this call within 20 msec when the component is
 * in the OMX_StateLoaded state.
 *
 * @param [in] hComponent
 *     Handle of the component to be accessed.  This is the component
 *     handle returned by the call to the GetHandle function.
 * @return OMX_ERRORTYPE
 *     If the command successfully executes, the return code will be
 *     OMX_ErrorNone.  Otherwise the appropriate OMX error will be returned.
 * @ingroup core
 */
OMX_API OMX_ERRORTYPE OMX_APIENTRY OMX_FreeHandle(
	OMX_IN OMX_HANDLETYPE hComponent)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *cmp_handle = NULL;
	OMX_COMPONENTTYPE *base_cmp = hComponent;

	/* param check*/
	oscl_param_check(base_cmp != NULL, OMX_ErrorBadParameter, NULL);
	cmp_handle = (lb_omx_component_t *)(base_cmp->pComponentPrivate);
	oscl_param_check(cmp_handle != NULL, OMX_ErrorBadParameter, NULL);

	if (base_cmp->ComponentDeInit)
		ret = base_cmp->ComponentDeInit(base_cmp);
	pthread_mutex_lock(&omx_lock);
	/*  check handle is valid and remove it from table */
	ret = __remove_handle(base_cmp);
	pthread_mutex_unlock(&omx_lock);
	oscl_free(cmp_handle);
	return ret;
}



/**
 * The OMX_SetupTunnel method will handle the necessary calls to the components
 * to setup the specified tunnel the two components.  NOTE: This is
 * an actual method (not a #define macro).  This method will make calls into
 * the component ComponentTunnelRequest method to do the actual tunnel
 * connection.
 *
 * The ComponentTunnelRequest method on both components will be called.
 * This method shall not be called unless the component is in the
 * OMX_StateLoaded state except when the ports used for the tunnel are
 * disabled. In this case, the component may be in the OMX_StateExecuting,
 * OMX_StatePause, or OMX_StateIdle states.
 *
 * The core should return from this call within 20 msec.
 *
 * @param [in] hOutput
 *     Handle of the component to be accessed.  Also this is the handle
 *     of the component whose port, specified in the nPortOutput parameter
 *     will be used the source for the tunnel. This is the component handle
 *     returned by the call to the OMX_GetHandle function.  There is a
 *     requirement that hOutput be the source for the data when
 *     tunelling (i.e. nPortOutput is an output port).  If 0x0, the component
 *     specified in hInput will have it's port specified in nPortInput
 *     setup for communication with the application / IL client.
 * @param [in] nPortOutput
 *     nPortOutput is used to select the source port on component to be
 *     used in the tunnel.
 * @param [in] hInput
 *     This is the component to setup the tunnel with. This is the handle
 *     of the component whose port, specified in the nPortInput parameter
 *     will be used the destination for the tunnel. This is the component handle
 *     returned by the call to the OMX_GetHandle function.  There is a
 *     requirement that hInput be the destination for the data when
 *     tunelling (i.e. nPortInut is an input port).   If 0x0, the component
 *     specified in hOutput will have it's port specified in nPortPOutput
 *     setup for communication with the application / IL client.
 * @param [in] nPortInput
 *     nPortInput is used to select the destination port on component to be
 *     used in the tunnel.
 * @return OMX_ERRORTYPE
 *     If the command successfully executes, the return code will be
 *     OMX_ErrorNone.  Otherwise the appropriate OMX error will be returned.
 *     When OMX_ErrorNotImplemented is returned, one or both components is
 *     a non-interop component and does not support tunneling.
 *
 *     On failure, the ports of both components are setup for communication
 *     with the application / IL Client.
 * @ingroup core tun
 */
OMX_API OMX_ERRORTYPE OMX_APIENTRY OMX_SetupTunnel(
	OMX_IN OMX_HANDLETYPE hOutput,
	OMX_IN OMX_U32 nPortOutput,
	OMX_IN OMX_HANDLETYPE hInput,
	OMX_IN OMX_U32 nPortInput)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	OMX_COMPONENTTYPE *component;
	OMX_TUNNELSETUPTYPE tunnel;

	OSCL_TRACE("output:%p/%i, input:%p/%i\n",
		hOutput, (int)nPortOutput, hInput, (int)nPortInput);
	oscl_param_check(!(hOutput == NULL && hInput == NULL),
		OMX_ErrorBadParameter, NULL);

	component = (OMX_COMPONENTTYPE *)hOutput;
	tunnel.nTunnelFlags = 0;
	tunnel.eSupplier = OMX_BufferSupplyUnspecified;
	if (component) {
		oscl_param_check((component->ComponentTunnelRequest != NULL),
			OMX_ErrorNotImplemented, NULL);
		ret = (component->ComponentTunnelRequest)(hOutput, nPortOutput,
				hInput, nPortInput, &tunnel);
		oscl_param_check((ret == OMX_ErrorNone),
			ret, "setup tunnel failed");
	}
	OSCL_LOGI("outport sucess, eSupplier:%x, flag:%x",
		tunnel.eSupplier, tunnel.nTunnelFlags);

	component = (OMX_COMPONENTTYPE *)hInput;
	if (component) {
		oscl_param_check_exit((component->ComponentTunnelRequest != NULL),
			OMX_ErrorNotImplemented, NULL);
		ret = (component->ComponentTunnelRequest)(hInput, nPortInput,
				hOutput, nPortOutput, &tunnel);
		oscl_param_check((ret == OMX_ErrorNone),
			ret, "setup tunnel failed");
	}
	OSCL_LOGI("inport sucess, eSupplier:%x, flag:%x, ret:%d",
		tunnel.eSupplier, tunnel.nTunnelFlags, ret);

EXIT:
	if (ret != OMX_ErrorNone && hOutput) {
		component = (OMX_COMPONENTTYPE *)hOutput;
		if (component->ComponentTunnelRequest)
			(component->ComponentTunnelRequest)(hOutput,
				nPortOutput, NULL, 0, &tunnel);
	}
	return ret;
}

/** @ingroup cp */
OMX_API OMX_ERRORTYPE OMX_GetContentPipe(
	OMX_OUT OMX_HANDLETYPE *hPipe,
	OMX_IN OMX_STRING szURI)
{
	return OMX_ErrorNotImplemented;
}

/**
 * The OMX_GetComponentsOfRole method will return the number of components that
 * support the given role and (if the compNames field is non-NULL) the names of
 * those components. The call will fail if an insufficiently sized array of names
 * is supplied. To ensure the array is sufficiently sized the
 * client should:
 *    * first call this function with the compNames field NULL to determine the
 *      number of component names
 *    * second call this function with the compNames field pointing to an array
 *      of names allocated according to the number returned by the first call.
 *
 * The core should return from this call within 5 msec.
 *
 * @param [in] role
 *     This is generic standard component name consisting only of component class
 *     name and the type within that class (e.g. 'audio_decoder.aac').
 * @param [inout] pNumComps
 *     This is used both as input and output.
 *
 *     If compNames is NULL, the input is ignored and the output specifies how many
 *     components support the given role.
 *
 *     If compNames is not NULL, on input it bounds the size of the input structure
 *      and on output, it specifies the number of components string names listed
 *      within the compNames parameter.
 * @param [inout] compNames
 *     If NULL this field is ignored. If non-NULL this points to an array of
 *     128-byte strings which accepts a list of the names of all physical components
 *     that implement the specified standard component name.
 *     Each name is NULL terminated. numComps indicates the number of names.
 * @ingroup core
 */
OMX_API OMX_ERRORTYPE OMX_GetComponentsOfRole(
	OMX_IN OMX_STRING role,
	OMX_INOUT OMX_U32 *pNumComps,
	OMX_INOUT OMX_U8 **compNames)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	unsigned i, j;
	int array_size = 0;
	int array_index = 0;
	OSCL_TRACE(" %p %p %p\n", role, pNumComps, compNames);

	oscl_param_check(role != NULL, OMX_ErrorBadParameter, NULL);
	oscl_param_check((pNumComps != NULL), OMX_ErrorBadParameter, NULL);
	oscl_param_check(!((compNames != NULL) && (*pNumComps == 0)),
		OMX_ErrorBadParameter, NULL);
	OSCL_LOGD(" %s\n", role);

	pthread_mutex_lock(&omx_lock);
	/* if compNames is not null,  *pNumComps is the size of the array*/
	if (compNames)
		array_size = *pNumComps;
	*pNumComps = 0;
	for (i = 0; (i < SIZE_OF_COMPONENTS) && omx_commponents[i].name; i++) {
		for (j = 0; (j < OMX_MAX_ROLES) && omx_commponents[i].roles[j]; j++) {
			OSCL_LOGD(" %s", omx_commponents[i].roles[j]);
			if (!strcmp(role, omx_commponents[i].roles[j])) {
				if ((array_index < array_size)
					&& compNames[array_index]) {
					strncpy((char *)compNames[array_index],
						omx_commponents[i].name,
						OMX_MAX_STRINGNAME_SIZE);
					array_index++;
				}
				*pNumComps += 1;
				break;
			}
		}
	}
	if (array_index != 0)
		*pNumComps = array_index;
	pthread_mutex_unlock(&omx_lock);

	return ret;
}


/**
 * The OMX_GetRolesOfComponent method will return the number of roles supported by
 * the given component and (if the roles field is non-NULL) the names of those roles.
 * The call will fail if an insufficiently sized array of names is supplied. To ensure
 * the array is sufficiently sized the client should:
 *     * first call this function with the roles field NULL to determine the number
 *       of role names
 *     * second call this function with the roles field pointing to an array
 *       of names allocated according to the number returned by the first call.
 *
 * The core should return from this call within 5 msec.
 *
 * @param [in] compName
 *     This is the name of the component being queried about.
 * @param [inout] pNumRoles
 *     This is used both as input and output.
 *
 *     If roles is NULL, the input is ignored and the output specifies how many roles
 * the component supports.
 *     If compNames is not NULL, on input it bounds the size of the input structure and
 *     on output, it specifies the number of roles string names listed within the
 *     roles parameter.
 * @param [out] roles
 *     If NULL this field is ignored. If non-NULL this points to an array of
 *     128-byte strings which accepts a list of the names of all standard components
 *     roles implemented on the specified component name. numComps indicates
 *     the number of names.
 * @ingroup core
 */
OMX_API OMX_ERRORTYPE OMX_GetRolesOfComponent(
	OMX_IN OMX_STRING compName,
	OMX_INOUT OMX_U32 *pNumRoles,
	OMX_OUT OMX_U8 **roles)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	unsigned i, j;
	int array_size = 0;
	int array_index = 0;
	OSCL_TRACE(" %p %p %p\n", compName, pNumRoles, roles);

	oscl_param_check(compName != NULL, OMX_ErrorBadParameter, NULL);
	oscl_param_check((pNumRoles != NULL), OMX_ErrorBadParameter, NULL);
	oscl_param_check(!((roles != NULL) && (*pNumRoles == 0)),
		OMX_ErrorBadParameter, NULL);
	OSCL_LOGD(" %s\n", compName);

	pthread_mutex_lock(&omx_lock);
	/* if roles is not null,  *pNumRoles is the size of the array*/
	if (roles)
		array_size = *pNumRoles;
	*pNumRoles = 0;
	for (i = 0; (i < SIZE_OF_COMPONENTS) && omx_commponents[i].name; i++) {
		if (!strcmp(compName, omx_commponents[i].name)) {
			for (j = 0; (j < OMX_MAX_ROLES) && omx_commponents[i].roles[j];
				j++) {
				OSCL_LOGD(" %s", omx_commponents[i].roles[j]);
				if ((array_index < array_size) && roles[array_index]) {
					strncpy((char *)roles[array_index],
						omx_commponents[i].roles[j],
						OMX_MAX_STRINGNAME_SIZE);
					array_index++;
				}
				*pNumRoles += 1;
			}
		}
	}
	if (array_index != 0)
		*pNumRoles = array_index;
	pthread_mutex_unlock(&omx_lock);

	return ret;
}


void omx_component_register(omx_component_tbl_t *omx_component_tbl)
{
	int i;
	int index = -1;

	if ((omx_component_tbl == NULL) || (omx_component_tbl->name == NULL))
		return;
	for (i = 0; i < (int)SIZE_OF_COMPONENTS; i++) {
		if (omx_commponents[i].reg_flag != 1
			|| (omx_commponents[i].name == NULL)) {
			if (index == -1)
				index = i;
			continue;
		}
		if (strcmp(omx_component_tbl->name, omx_commponents[i].name) == 0) {
			OSCL_LOGE("component already exist");
			return;
		}
		if (omx_component_tbl->fn_init == omx_commponents[i].fn_init) {
			OSCL_LOGE("component already exist");
			return;
		}

	}
	if (index == -1) {
		OSCL_LOGE("reach the max num of components");
		return;
	}
	memcpy(&omx_commponents[index], omx_component_tbl, sizeof(omx_component_tbl_t));
	omx_commponents[index].reg_flag = 1;
	OSCL_TRACE("component index:%d\n", index);
	return;
}


OMX_HANDLETYPE *omx_active_handle_get(void)
{
	pthread_mutex_lock(&omx_lock);
	return active_handle;
}
void omx_active_handle_put(void)
{
	pthread_mutex_unlock(&omx_lock);
}

INIT_ENV_EXPORT(OMX_Init);

