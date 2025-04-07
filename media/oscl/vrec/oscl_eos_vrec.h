/*
 * oscl_eos_vrec.h - eos video receive api used by lombo media framework.
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

#ifndef __OSCL_EOS_VREC_H__
#define __OSCL_EOS_VREC_H__

#include <base_component.h>
#include "oscl_vrec_i.h"
#include "viss.h"

typedef struct vrec_hdl {
	char name[10];		/* video receive device name */
	char count;
	rt_sem_t event_hdl;	/* check video data input */
	rt_device_t vrec_device;/* video receive driver handle */
	vrec_info_t info;	/* sensor information */
	struct viss_frame_queue *frame_q;
} vrec_hdl_t;

#endif

