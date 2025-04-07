/*
 * oscl_eos_disp.h - display api used by lombo media framework.
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

#ifndef __OSCL_EOS_DISP_H__
#define __OSCL_EOS_DISP_H__

#include <base_component.h>
#include <lombo_disp.h>
#include "oscl_disp_i.h"

typedef struct disp_hdl {
	char dgb_flag;
	char layer_name[20];		/* window name */
	disp_ctl_t *layer;		/* window handle */
	rt_device_t disp_device;	/* display driver handle */
	rt_device_disp_ops_t  *disp_ops;/* dispaly driver operating interface */
} disp_hdl_t;

#endif /* __OSCL_EOS_DISP_H__ */

