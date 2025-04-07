/*
 * clock_port.h - Standard functionality for lombo clock port.
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
#ifndef __CLOCK_PORT_H__
#define __CLOCK_PORT_H__

typedef struct clock_port_private {
	OMX_TIME_CONFIG_TIMESTAMPTYPE		timestamp; /* timestamp of the refclk */
	OMX_TIME_MEDIATIMETYPE			mediatime; /* media time of this port */
	OMX_TIME_CONFIG_MEDIATIMEREQUESTTYPE	mediatime_request;
	OMX_OTHER_PARAM_PORTFORMATTYPE		other_param;
} clock_port_private_t;

OMX_ERRORTYPE clock_port_init(lb_omx_component_t *component,
	base_port_t *base_port,
	u32 index,
	u32 dir_type);

OMX_ERRORTYPE clock_port_deinit(base_port_t *port);

OMX_ERRORTYPE clock_port_recive_buffer(base_port_t *port,
	OMX_BUFFERHEADERTYPE *buffer);

#endif /* __CLOCK_PORT_H__ */
