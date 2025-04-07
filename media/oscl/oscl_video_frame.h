/*
 * oscl_queue.h - queue api used by lombo media framework.
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

#ifndef __OSCL_VIDEO_FRAME__H__
#define __OSCL_VIDEO_FRAME__H__
#include "OMX_IVCommon.h"

#define VFRAME_ALIGN(x, a) (((x) + (a) - 1) & ~((a) - 1))

static inline OMX_ERRORTYPE calc_frame_size(OMX_COLOR_FORMATTYPE format,
	OMX_S32 w, OMX_S32 h, OMX_U32 *size, OMX_S32 align)
{
	if (align < 1)
		return OMX_ErrorBadParameter;

	if (OMX_COLOR_FormatYUV420SemiPlanar == format) {
		size[0] = VFRAME_ALIGN((w * h), align);
		size[1] = VFRAME_ALIGN(size[0] >> 1, align);
		size[2] = 0;
	} else {
		return OMX_ErrorBadParameter;
	}
	return OMX_ErrorNone;
}

#endif /* __OSCL_VIDEO_FRAME__H__ */
