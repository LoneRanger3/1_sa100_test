/*
 * vframe_list_manager.h - video frame manager head file
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

#ifndef __VISS_CAM_POWER_H__
#define __VISS_CAM_POWER_H__

struct cam_power {
	struct pinctrl *pctrl;
	rt_int32_t cam_pwr_en_valid;
	rt_int32_t cam_pwr_en_gpio;
	rt_uint32_t cam_pwr_en_val[7];
	char        propname[24];
};

#if 0
rt_int32_t cam_power_enable(void);

void cam_power_disable(void);
#endif
void camera_ldo_set(rt_int32_t vol);

void camera_ldo_exit(void);

rt_int32_t cam_power_set(char *propname);

rt_int32_t cam_power_exit(char *propname);



#endif

