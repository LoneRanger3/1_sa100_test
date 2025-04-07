/*
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

#ifndef __SAU_CAMERA_CSP_H__
#define __SAU_CAMERA_CSP_H__

enum camera_ldo_vol {
	vol_1p65v	= 0,
	vol_1p70v	= 1,
	vol_1p75v	= 2,
	vol_1p80v	= 3,
	vol_1p85v	= 4,
	vol_1p90v	= 5,
	vol_1p95v	= 6,
	vol_2p00v	= 7,
	vol_2p60v	= 8,
	vol_2p70v	= 9,
	vol_2p75v	= 10,
	vol_2p80v	= 11,
	vol_2p85v	= 12,
	vol_2p90v	= 13,
	vol_3p00v	= 14,
	vol_3p10v	= 15,
};

void csp_sau_camera_ldo_enable(void);
void csp_sau_camera_ldo_disable(void);
void csp_sau_camera_ldo_output_voltage(u32 vol);

#endif

