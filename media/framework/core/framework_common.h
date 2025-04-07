/*
 * lb_omx_core.h - Standard functionality for lombo omx core.
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

#ifndef LB_OMX_CORE_H
#define LB_OMX_CORE_H
#include "OMX_Core.h"
#include "lb_recorder.h"
#include "base_component.h"

#define OPENMAX_SHARED_PORT 4
#if (OPENMAX_SHARED_PORT > 8)
#error "OPENMAX_SHARED_PORT must <= 8"
#endif

#define UNTUNNEL_BUFFER_MAP_SIZE 8

typedef enum {
	AL_PORT_STATE_INIT	= 0,
	AL_PORT_STATE_TUNNELED,
	AL_PORT_STATE_UNTUN_SETUP,
} al_port_state_t;


typedef struct al_port_info {
	void *comp_info;	/* al component*/
	al_port_state_t state;
	int index;
	OMX_DIRTYPE edir;
	OMX_PORTDOMAINTYPE domain;
	struct al_port_info *tunnel_port;
	void *tunnel_hdl;
	app_frame_cb_t cb;

	int nbuffer;
	int buf_size;
	int is_shared_buffer;
	OMX_BUFFERHEADERTYPE **header;
	int nbuffer_hold;
	int *hold_map;
	void *priv_data;
} al_port_info_t;

typedef struct al_muxer_private {
	void *rec_handle;
	fix_duration_param_t fix_duration_param;
} al_muxer_private_t;
typedef struct al_comp_info {
	OMX_COMPONENTTYPE *cmp_hdl;
	char *name;
	OMX_STATETYPE state;
	sem_t *sem_cmd;
	int num_port;
	al_port_info_t *port_info;
	pthread_mutex_t state_lock;
	void *priv_data;
} al_comp_info_t;

OMX_CALLBACKTYPE al_untunnel_common_callbacks;

#define is_al_component_valid(comp) (comp->cmp_hdl != NULL)
#define al_port_name(port) (((al_comp_info_t *)(port->comp_info))->name)

int al_component_init(al_comp_info_t *comp_info,
	char *name,
	OMX_CALLBACKTYPE *callbacks);
void al_component_deinit(al_comp_info_t *comp_info);
OMX_ERRORTYPE al_component_setstate(al_comp_info_t *al, OMX_STATETYPE s);
OMX_ERRORTYPE al_component_sendcom(al_comp_info_t *al, OMX_COMMANDTYPE cmd,
					   OMX_U32 param, OMX_PTR cmd_data);
int al_get_port_index(al_comp_info_t *comp_info,
			 OMX_DIRTYPE edir,
			 OMX_PORTDOMAINTYPE domain);
int al_untunnel_setup_cb(al_port_info_t *port, app_frame_cb_t *cb);
void al_untunnel_unset_cb(al_port_info_t *out_port);
int al_untunnel_setup_ports(al_port_info_t *out_port, al_port_info_t *in_port);
void al_untunnel_unset_ports(al_port_info_t *out_port, al_port_info_t *in_port);
void al_untunnel_queue_buffers(al_comp_info_t *al);
#endif

