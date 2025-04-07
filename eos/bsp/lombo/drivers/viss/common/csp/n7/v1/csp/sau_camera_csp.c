/*
 * Copyright (C) 2016-2018, LomboTech Co.Ltd.
 * Author: lomboswer <lomboswer@lombotech.com>
 *
 * Lombo n7 sau register definitions
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

/******************************************************************************
 * base operations
 *****************************************************************************/

void csp_sau_camera_ldo_enable(void)
{
	reg_sau_camera_ldo_t camera_ldo;
#if 0
	camera_ldo.val = READREG32(VA_SAU_CAMERA_LDO);
	camera_ldo.bits.sel = 4;
	WRITEREG32(VA_SAU_CAMERA_LDO, camera_ldo.val);
#endif
	camera_ldo.val = READREG32(VA_SAU_CAMERA_LDO);
	camera_ldo.bits.en = 1;
	WRITEREG32(VA_SAU_CAMERA_LDO, camera_ldo.val);
}

void csp_sau_camera_ldo_disable(void)
{
	reg_sau_camera_ldo_t camera_ldo;

	camera_ldo.val = READREG32(VA_SAU_CAMERA_LDO);
	camera_ldo.bits.en = 0;
	WRITEREG32(VA_SAU_CAMERA_LDO, camera_ldo.val);
}

void csp_sau_camera_ldo_output_voltage(u32 vol)
{
	reg_sau_camera_ldo_t camera_ldo;

	camera_ldo.val = READREG32(VA_SAU_CAMERA_LDO);
	camera_ldo.bits.sel = vol;
	WRITEREG32(VA_SAU_CAMERA_LDO, camera_ldo.val);
}

