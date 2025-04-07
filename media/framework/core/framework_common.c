/*
 * untunnel_common.c - Standard functionality for utunnel mode.
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
#include <framework_common.h>
#include <clock_time.h>

int al_component_init(al_comp_info_t *comp_info, char *name,
	OMX_CALLBACKTYPE *callbacks)
{
	int ret = OMX_ErrorNone;
	int i;
	OMX_PARAM_PORTDEFINITIONTYPE port_def;

	OSCL_TRACE("%p, %p, %p", comp_info, name, callbacks);
	oscl_param_check_exit((comp_info != NULL) && (name != NULL), -1, NULL);

	pthread_mutex_init(&comp_info->state_lock, NULL);
	comp_info->sem_cmd = oscl_zalloc(sizeof(sem_t));
	oscl_param_check_exit((comp_info->sem_cmd != NULL), -1, NULL);
	sem_init(comp_info->sem_cmd, 0, 0);

	comp_info->name = oscl_strdup(name);
	oscl_param_check_exit((comp_info->name != NULL), -1, NULL);

	ret = OMX_GetHandle((void **)&comp_info->cmp_hdl, name, comp_info, callbacks);
	oscl_param_check_exit((comp_info->cmp_hdl != NULL && ret == OMX_ErrorNone),
		ret, "get handle failed!");
	comp_info->state = OMX_StateLoaded;

	ret = OMX_GetParameter(comp_info->cmp_hdl,
			omx_index_vendor_get_port_number, &comp_info->num_port);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	oscl_param_check_exit((comp_info->num_port <= OMX_PORT_NUMBER_SUPPORTED),
		OMX_ErrorBadParameter, NULL);

	comp_info->port_info = oscl_zalloc(comp_info->num_port * sizeof(al_port_info_t));
	for (i = 0; i < comp_info->num_port; i++) {
		comp_info->port_info[i].index = -1;
		port_def.nVersion.nVersion = OMX_VERSION;
		port_def.nPortIndex = i;
		ret = OMX_GetParameter(comp_info->cmp_hdl,
				OMX_IndexParamPortDefinition, &port_def);
		if (ret != OMX_ErrorNone) {
			OSCL_LOGW("(%s)get port(%d) info err!", comp_info->name, i);
			continue;
		}
		comp_info->port_info[i].nbuffer = port_def.nBufferCountActual;
		comp_info->port_info[i].index = i;
		comp_info->port_info[i].state = AL_PORT_STATE_INIT;
		comp_info->port_info[i].edir = port_def.eDir;
		comp_info->port_info[i].domain = port_def.eDomain;
		comp_info->port_info[i].comp_info = comp_info;
		comp_info->port_info[i].buf_size = port_def.nBufferSize;
		OSCL_LOGI("(%s)get port(%d) info sucess!", comp_info->name, i);
		OSCL_LOGI("nbuffer(%d) edir(%d) domain(%d) size(%d)!",
			comp_info->port_info[i].nbuffer,
			comp_info->port_info[i].edir,
			comp_info->port_info[i].domain,
			comp_info->port_info[i].buf_size);
	}

EXIT:
	if (ret != 0)
		al_component_deinit(comp_info);
	OSCL_TRACE("%x", ret);
	return ret;
}

void al_component_deinit(al_comp_info_t *comp_info)
{
	OSCL_TRACE("comp info:%p", comp_info);

	comp_info->state = OMX_StateInvalid;
	if (comp_info->port_info) {
		int i;
		for (i = 0; i < comp_info->num_port; i++) {
			if (comp_info->port_info[i].hold_map != NULL) {
				oscl_free(comp_info->port_info[i].hold_map);
				comp_info->port_info[i].hold_map = NULL;
			}
		}
		oscl_free(comp_info->port_info);
		comp_info->port_info = NULL;
	}
	if (comp_info->cmp_hdl) {
		OMX_FreeHandle(comp_info->cmp_hdl);
		comp_info->cmp_hdl = NULL;
	}
	if (comp_info->sem_cmd) {
		sem_destroy(comp_info->sem_cmd);
		oscl_free(comp_info->sem_cmd);
	}
	if (comp_info->name)
		oscl_free(comp_info->name);
	if (comp_info->priv_data)
		oscl_free(comp_info->priv_data);
	pthread_mutex_destroy(&comp_info->state_lock);
	memset(comp_info, 0, sizeof(al_comp_info_t));

	OSCL_TRACE("exit");
}

/* note: only return first index fufill the quotions */
int al_get_port_index(al_comp_info_t *comp_info,
	OMX_DIRTYPE edir,
	OMX_PORTDOMAINTYPE domain)
{
	int i;
	int ret = -1;

	for (i = 0; i < comp_info->num_port; i++) {
		if (comp_info->port_info[i].edir == edir
			&& comp_info->port_info[i].domain == domain) {
			ret = i;
			break;
		}
	}
	return ret;
}

OMX_ERRORTYPE al_component_setstate(al_comp_info_t *al, OMX_STATETYPE s)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	OMX_STATETYPE cur_state;
	oscl_param_check(al && al->cmp_hdl, -1, NULL);
	OMX_GetState(al->cmp_hdl, &cur_state);
	if (cur_state != s) {
		pthread_mutex_lock(&al->state_lock);
		if (s == OMX_StateExecuting && cur_state == OMX_StateIdle)
			al_untunnel_queue_buffers(al);
		al->state = s;
		pthread_mutex_unlock(&al->state_lock);
		OSCL_LOGI("%s al_component_setstate %x!", al->name, s);
		ret = OMX_SendCommand(al->cmp_hdl, OMX_CommandStateSet, s, NULL);
		while (oscl_sem_timedwait_ms(al->sem_cmd, 1000))
			OSCL_LOGE("%s wait state %x(%d) timeout", al->name, s, cur_state);
		OSCL_LOGI("%s al_component_setstate %x!", al->name, s);
	}
	return ret;
}

OMX_ERRORTYPE al_component_sendcom(al_comp_info_t *al,
	OMX_COMMANDTYPE cmd,
	OMX_U32 param,
	OMX_PTR cmd_data)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;

	OSCL_LOGI("%s send command %x!", al->name, cmd);
	ret = OMX_SendCommand(al->cmp_hdl, cmd, param, cmd_data);
	if (ret != OMX_ErrorNone)
		OSCL_LOGE("%s send command err:%x!", al->name, ret);

	while (oscl_sem_timedwait_ms(al->sem_cmd, 1000))
		OSCL_LOGE("%s wait cmd %x compolete timeout", al->name, cmd);
	return ret;
}


