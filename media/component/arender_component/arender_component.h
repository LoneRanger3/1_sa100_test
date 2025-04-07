/*
 * arender_component.h - audio render component.
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

#ifndef __ARENDER_COMPONENT_H__
#define __ARENDER_COMPONENT_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define ARENDER_CLOCK_PORT 1

extern OMX_ERRORTYPE arender_component_deinit(OMX_IN OMX_HANDLETYPE hComponent);
extern OMX_ERRORTYPE arender_component_init(lb_omx_component_t *cmp_handle);

#ifdef __cplusplus
}
#endif

#endif /* __ARENDER_COMPONENT_H__ */
