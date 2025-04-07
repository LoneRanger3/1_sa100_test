/*
 * oscl_vrec.h - video receive api used by lombo media framework.
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
#ifndef __OSCL_VREC_H__
#define __OSCL_VREC_H__
#ifdef __EOS__
#include "oscl_eos_vrec.h"
#elif defined __KERNEL__
#endif
void oscl_close_vrec(void *hdl);
void *oscl_open_vrec(const char *name);
OMX_S32 oscl_vrec_wait_frame(void *hdl, OMX_S32 timeout);
OMX_ERRORTYPE oscl_vrec_capture_off(void *hdl, OMX_S32 ch);
OMX_ERRORTYPE oscl_vrec_capture_on(void *hdl, OMX_S32 ch);
OMX_ERRORTYPE oscl_vrec_get_cur_format(void *hdl, vrec_format_t *format);
OMX_ERRORTYPE oscl_vrec_get_info(void *hdl, vrec_info_t *info);
OMX_ERRORTYPE oscl_vrec_release_frame(void *hdl, vrec_reqframe_t *vrec_frame);
OMX_ERRORTYPE oscl_vrec_request_frame(void *hdl, vrec_reqframe_t *vrec_frame);
OMX_ERRORTYPE oscl_vrec_set_buffer(void *hdl, vrec_frame_queue_t *frame_q);
OMX_ERRORTYPE oscl_vrec_clean_buffer(void *hdl);
OMX_ERRORTYPE oscl_vrec_set_format(void *hdl, vrec_format_t *format);
OMX_ERRORTYPE oscl_vrec_set_gpts_cb(void *hdl, vrec_pts_cb_t *callback);
OMX_ERRORTYPE oscl_open_isp(void *hdl, const char *name);
void oscl_close_isp(void *hdl);

#endif

