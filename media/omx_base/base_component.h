/*
 * base_component.h - Standard functionality for lombo common component.
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

#ifndef __BASE_COMPOMEMT_H__
#define __BASE_COMPOMEMT_H__
#include "OMX_Core.h"
#include "OMX_Component.h"
#include "oscl.h"
#include <lb_omx_as_string.h>

#define OMX_VERSION 0x00000101
#define BASE_COMPONENT_VERSION 0x00000101
#define OMX_PORT_NUMBER_SUPPORTED 8

#define OMX_DEFAULT_INPUT_PORT 0
#define OMX_DEFAULT_OUTPUT_PORT 1
#define OMX_DEFAULT_BUF_SIZE (32*1024)

#define TUNNEL_USE_BUFFER_RETRY 20
#define TUNNEL_USE_BUFFER_RETRY_USLEEP_TIME 50000

#define BUFFER_ALLOCATED (1 << 0)
#define BUFFER_ASSIGNED (1 << 1)
#define HEADER_ALLOCATED (1 << 2)
#define BUFFER_FREE 0

#define BASE_INPUT_PORT 0
#define BASE_OUTPUT_PORT 1

typedef enum TUNNEL_STATUS_FLAG {
	NO_TUNNEL = 0,
	TUNNEL_ESTABLISHED = 0x0001,
	TUNNEL_IS_SUPPLIER = 0x0002,
} TUNNEL_STATUS_FLAG;

#define BASE_COMPONENT_DEBUG_MASK (1<<31)

/**
 * Port Specific Macro's
 */
#define PORT_IS_ENABLED(port) (port->port_param.bEnabled != 0)
#define PORT_IS_SUPPLIER(port) (port->tunnel_flags & TUNNEL_IS_SUPPLIER)
#define PORT_IS_TUNNELED(port) (port->tunnel_flags & TUNNEL_ESTABLISHED)
typedef struct base_port {
	void *component;
	OMX_PTR port_private;
	OMX_PARAM_PORTDEFINITIONTYPE port_param;
	sem_t populate_sem;
	sem_t buffer_sem;
	OMX_U32 being_onoff;

	OMX_U32 is_flushed;
	OMX_U32 num_assigned;
	OMX_U32 *buf_state;
	OMX_BUFFERHEADERTYPE **header;
	oscl_queue_t buffer_queue;

	OMX_U32 tunnel_flags;
	OMX_HANDLETYPE tunneled_component;
	OMX_U32 tunnel_ports;
	OMX_U32 num_tunnel_buffer;

	OMX_U32 log_level;

	OMX_ERRORTYPE(*deinit)(struct base_port *);
	OMX_ERRORTYPE(*enable)(struct base_port *);
	OMX_ERRORTYPE(*disable)(struct base_port *);
	OMX_ERRORTYPE(*allocate_buffer)(struct base_port *, OMX_BUFFERHEADERTYPE **,
		OMX_PTR, OMX_U32);
	OMX_ERRORTYPE(*use_buffer)(struct base_port *, OMX_BUFFERHEADERTYPE **,
		OMX_PTR, OMX_U32, OMX_U8 *);
	OMX_ERRORTYPE(*free_buffer)(struct base_port *, OMX_BUFFERHEADERTYPE *);
	OMX_ERRORTYPE(*allocate_tunnel_buffer)(struct base_port *);
	OMX_ERRORTYPE(*free_tunnel_buffer)(struct base_port *);
	OMX_ERRORTYPE(*tunnel_request)(struct base_port *, OMX_HANDLETYPE, OMX_U32,
		OMX_TUNNELSETUPTYPE *);
	OMX_ERRORTYPE(*buf_done_callback)(OMX_HANDLETYPE, OMX_PTR,
		OMX_BUFFERHEADERTYPE *);
	OMX_ERRORTYPE(*recive_buffer)(struct base_port *, OMX_BUFFERHEADERTYPE *);
	OMX_ERRORTYPE(*return_buffer)(struct base_port *, OMX_BUFFERHEADERTYPE *);
	OMX_ERRORTYPE(*flush_buffer)(struct base_port *);
} base_port_t;

typedef struct lb_omx_component {
	OMX_COMPONENTTYPE base_comp;
	OMX_PTR component_private;
	char *name;
	OMX_STATETYPE state;
	OMX_STATETYPE target_state;

	sem_t *msg_sem;
	oscl_queue_t msg_queue;
	OMX_S32 msg_handler_thread_id;
	pthread_t msg_handler_thread;

	sem_t *buf_mgnt_sem;
	OMX_S32 buf_mgnt_thread_id;
	pthread_t buffer_mgmt_thread;
	pthread_attr_t buf_thread_attr;
	OMX_MARKTYPE mark;

	OMX_U32 num_ports;
	struct base_port port[OMX_PORT_NUMBER_SUPPORTED];
	pthread_mutex_t flush_mutex;
	sem_t *flush_sem;
	sem_t *mgmt_flush_sem;

	OMX_U32 eos_flag;
	OMX_CALLBACKTYPE callbacks;
	OMX_PTR callback_data;
	OMX_U32 dbg_flag;
	OMX_U32 dbg_wdog;
	OMX_U32 log_level;

	OMX_PTR(*buf_manager)(OMX_PTR);
	void (*buf_handle)(OMX_HANDLETYPE, OMX_BUFFERHEADERTYPE *,
		OMX_BUFFERHEADERTYPE *);
	OMX_ERRORTYPE(*msg_handle)(OMX_HANDLETYPE, oscl_message_t *);
	OMX_ERRORTYPE(*do_state_set)(OMX_HANDLETYPE, OMX_U32);
} lb_omx_component_t;


/**
 * get_lb_component - get lb_omx_component_t handle
 */
#define get_lb_component(ptr) \
	((lb_omx_component_t *)(((OMX_COMPONENTTYPE *)ptr)->pComponentPrivate))
#define get_component_name(component) \
	component->name ? (component->name) : "unknown component"
#define get_component_state(port) \
	(((struct lb_omx_component *)(port->component))->state)
#define get_port_name(port) (((struct lb_omx_component *)(port->component))->name)

#define base_check_eos(component, port, buffer) \
	do { \
		if (buffer != NULL && ((buffer->nFlags & OMX_BUFFERFLAG_EOS) != 0)) { \
			OSCL_LOGI("component:%s, Detected EOS flags, filled=%d\n", \
				  get_component_name(component), buffer->nFilledLen); \
			(*(component->callbacks.EventHandler))(&component->base_comp, \
							component->callback_data, \
							OMX_EventBufferFlag, \
							port->port_param.nPortIndex, \
							buffer->nFlags, \
							NULL); \
		} \
	} while (0)

#define base_check_mark(component, buffer) \
	do { \
		if (buffer != NULL \
			&& buffer->hMarkTargetComponent == &component->base_comp) { \
			(*(component->callbacks.EventHandler))(&component->base_comp, \
					component->callback_data, OMX_EventMark, 0, 0, \
					buffer->pMarkData); \
		} \
	} while (0)

#define set_port_loglvl(port, lvl) (port->log_level = lvl)
#define set_comp_loglvl(comp, lvl) (comp->log_level = lvl)

OMX_ERRORTYPE base_port_init(lb_omx_component_t *component,
	base_port_t *base_port, OMX_U32 index, OMX_U32 dir_type);
OMX_ERRORTYPE base_port_deinit(base_port_t *port);
OMX_ERRORTYPE base_port_enable(base_port_t *port);
OMX_ERRORTYPE base_port_disable(base_port_t *port);
OMX_ERRORTYPE base_port_allocate_buf(base_port_t *port,
	OMX_BUFFERHEADERTYPE **buf_header, OMX_PTR app_private, OMX_U32 size);
OMX_ERRORTYPE base_port_use_buffer(base_port_t *port,
	OMX_BUFFERHEADERTYPE **buf_header,
	OMX_PTR app_private, OMX_U32 size, OMX_U8 *buffer);
OMX_ERRORTYPE base_port_free_buffer(
	base_port_t *port,
	OMX_BUFFERHEADERTYPE *buf_header);
OMX_ERRORTYPE base_port_tunnel_request(base_port_t *port,
	OMX_HANDLETYPE tunneld_comp, OMX_U32 tunneled_index,
	OMX_TUNNELSETUPTYPE *tunnel);
OMX_ERRORTYPE base_port_allocate_tunnel_buf(base_port_t *port);
OMX_ERRORTYPE base_port_free_tunnel_buffer(base_port_t *port);
OMX_ERRORTYPE base_port_flush_buffer(base_port_t *port);
OMX_ERRORTYPE base_port_recive_buffer(base_port_t *port,
	OMX_BUFFERHEADERTYPE *buffer);
OMX_ERRORTYPE base_port_return_buffer(base_port_t *port,
	OMX_BUFFERHEADERTYPE *buffer);

OMX_ERRORTYPE base_get_version(OMX_IN OMX_HANDLETYPE cmp_hdl,
	OMX_OUT OMX_STRING name,
	OMX_OUT OMX_VERSIONTYPE * comp_version,
	OMX_OUT OMX_VERSIONTYPE * spec_version,
	OMX_OUT OMX_UUIDTYPE * uuid);
OMX_ERRORTYPE base_send_command(OMX_IN OMX_HANDLETYPE cmp_hdl,
	OMX_IN OMX_COMMANDTYPE cmd,
	OMX_IN OMX_U32 param1,
	OMX_IN OMX_PTR cmd_data);
OMX_ERRORTYPE base_get_parameter(OMX_IN OMX_HANDLETYPE cmp_hdl,
	OMX_IN OMX_INDEXTYPE param_index,
	OMX_INOUT OMX_PTR param_data);
OMX_ERRORTYPE base_set_parameter(OMX_IN OMX_HANDLETYPE cmp_hdl,
	OMX_IN OMX_INDEXTYPE param_index,
	OMX_IN OMX_PTR	param_data);
OMX_ERRORTYPE base_get_config(OMX_IN OMX_HANDLETYPE cmp_hdl,
	OMX_IN OMX_INDEXTYPE cfg_index,
	OMX_INOUT OMX_PTR cfg_data);
OMX_ERRORTYPE base_set_config(OMX_IN OMX_HANDLETYPE cmp_hdl,
	OMX_IN OMX_INDEXTYPE cfg_index,
	OMX_IN OMX_PTR cfg_data);
OMX_ERRORTYPE base_get_extension_index(OMX_IN OMX_HANDLETYPE cmp_hdl,
	OMX_IN OMX_STRING name,
	OMX_OUT OMX_INDEXTYPE * index);
OMX_ERRORTYPE base_get_state(OMX_IN OMX_HANDLETYPE cmp_hdl,
	OMX_OUT OMX_STATETYPE *state);
OMX_ERRORTYPE base_tunnel_request(OMX_IN OMX_HANDLETYPE cmp_hdl,
	OMX_IN OMX_U32 port,
	OMX_IN OMX_HANDLETYPE peer_cmp_hdl,
	OMX_IN OMX_U32 peer_port,
	OMX_INOUT OMX_TUNNELSETUPTYPE *tunnel_setup);
OMX_ERRORTYPE base_use_buffer(OMX_IN OMX_HANDLETYPE cmp_hdl,
	OMX_INOUT OMX_BUFFERHEADERTYPE **header,
	OMX_IN OMX_U32 port_index,
	OMX_IN OMX_PTR app_data,
	OMX_IN OMX_U32 bytes,
	OMX_IN OMX_U8 *buffer);
OMX_ERRORTYPE base_allocate_buffer(OMX_IN OMX_HANDLETYPE cmp_hdl,
	OMX_INOUT OMX_BUFFERHEADERTYPE **header,
	OMX_IN OMX_U32 port_index,
	OMX_IN OMX_PTR app_data,
	OMX_IN OMX_U32 bytes);
OMX_ERRORTYPE base_free_buffer(OMX_IN OMX_HANDLETYPE cmp_hdl,
	OMX_IN OMX_U32 port_index,
	OMX_IN OMX_BUFFERHEADERTYPE *buffer);
OMX_ERRORTYPE base_empty_this_buffer(OMX_IN OMX_HANDLETYPE cmp_hdl,
	OMX_IN OMX_BUFFERHEADERTYPE *header);
OMX_ERRORTYPE base_fill_this_buffer(OMX_IN OMX_HANDLETYPE cmp_hdl,
	OMX_IN OMX_BUFFERHEADERTYPE *header);
OMX_ERRORTYPE base_set_callbacks(OMX_IN OMX_HANDLETYPE cmp_hdl,
	OMX_IN OMX_CALLBACKTYPE *callbacks,
	OMX_IN OMX_PTR	 app_data);
OMX_ERRORTYPE base_deinit(OMX_IN OMX_HANDLETYPE cmp_hdl);
void *base_component_buffer_manager(void *param);
OMX_ERRORTYPE base_component_deinit(OMX_HANDLETYPE cmp_hdl);
OMX_ERRORTYPE base_component_init(lb_omx_component_t *lb_cmp_hdl);
OMX_ERRORTYPE base_component_set_state(OMX_HANDLETYPE cmp_hdl,
	OMX_U32 dest_state);
OMX_ERRORTYPE base_component_message_handle(OMX_HANDLETYPE cmp_hdl,
	oscl_message_t *message);
void base_show_component(OMX_HANDLETYPE cmp_hdl);

#endif
