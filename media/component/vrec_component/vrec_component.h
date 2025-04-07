/*
 * vrec_component.h - video receive component.
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

#ifndef __VREC_COMPONENT_H__
#define __VREC_COMPONENT_H__

#include "vrec/oscl_vrec.h"

#define SOURCE_OUTPUT_PORT_INDEX 0

OMX_ERRORTYPE vrec_component_deinit(OMX_IN OMX_HANDLETYPE hComponent);
OMX_ERRORTYPE vrec_component_init(lb_omx_component_t *cmp_handle);

#endif /* __VREC_COMPONENT_H__ */

