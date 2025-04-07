/*
 * clock_component.h - Standard functionality for lombo clock component.
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
#ifndef __CLOCK_COMPONENT_H__
#define __CLOCK_COMPONENT_H__

/* Maximum number of clock ports */
#define MAX_CLOCK_PORTS                          4
#define CLOCK_PORT_AUDIO                         0
#define CLOCK_PORT_VIDEO                         1
#define CLOCK_PORT_DEMUXER                       2
#define CLOCK_PORT_SUBTITLE                      3

/** Clock component private structure.
 * see the define above
 * @param clock_state This structure holds the state of the clock
 * @param sem_starttime the semaphore that coordinates the arrival
 *	of start times from all clients
 * @param sem_clockevent the semaphore that coordinates clock event
 *	received from the client
 * @param sem_clkevent_complete the semaphore that coordinates clock
 *	event sent to the client
 * @param walltime_base the wall time at which the clock was started
 * @param mediatime_base the Media time at which the clock was started
 * @param update_type indicates the type of update received
 *	from the clock src component
 * @param min_starttime keeps the minimum starttime of the clients
 * @param config_scale Representing the current media time scale factor
 */
typedef struct clock_component_private {
	OMX_TIME_CONFIG_CLOCKSTATETYPE      clock_state;
	OMX_TIME_CONFIG_ACTIVEREFCLOCKTYPE  ref_clock;
	sem_t                               sem_starttime;
	sem_t                               sem_clockevent;
	sem_t                               sem_clkevent_complete;
	OMX_TICKS                           walltime_base;
	OMX_TICKS                           mediatime_base;
	OMX_TICKS                           mediatime;
	OMX_TIME_UPDATETYPE                 update_type;
	OMX_TIME_CONFIG_TIMESTAMPTYPE       min_starttime;
	OMX_TIME_CONFIG_SCALETYPE           config_scale;
} clock_component_private_t;

OMX_ERRORTYPE clock_component_deinit(OMX_IN OMX_HANDLETYPE hComponent);

OMX_ERRORTYPE clock_component_init(lb_omx_component_t *cmp_handle);

#endif /* __CLOCK_COMPONENT_H__ */

