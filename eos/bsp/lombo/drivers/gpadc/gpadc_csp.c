/*
 * gpadc_csp.c - the chip operations for GPADC
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

#include "gpadc_csp.h"
#include <debug.h>

#define GPADC_AUX_TOTAL			2

/* Set GPADC enable */
void csp_gpadc_set_en(rt_bool_t en)
{
	reg_gpadc_gpaen_t reg;

	reg.val = READREG32(VA_GPADC_GPAEN);
	reg.bits.gen = en ? 1 : 0;

	WRITEREG32(VA_GPADC_GPAEN, reg.val);
}

/* Set GPADC Calibration enable */
void csp_gpadc_set_cali_en(rt_bool_t en)
{
	reg_gpadc_gpaen_t reg;

	reg.val = READREG32(VA_GPADC_GPAEN);
	reg.bits.cen = en ? 1 : 0;

	WRITEREG32(VA_GPADC_GPAEN, reg.val);
}

/* Set GPADC sample rate */
void csp_gpadc_set_sample_rate(GPADC_SRC_TYPE t)
{
	reg_gpadc_gpac_t reg; /* control register setup */

	reg.val = READREG32(VA_GPADC_GPAC);
	reg.bits.src = t;
	reg.bits.auxin0e = 1;

	WRITEREG32(VA_GPADC_GPAC, reg.val);
}

/* Set GPADC sample data average enable */
void csp_gpadc_sda_en(rt_bool_t en)
{
	reg_gpadc_gpac_t reg;

	reg.val = READREG32(VA_GPADC_GPAC);
	reg.bits.sdae = en ? 1 : 0;

	WRITEREG32(VA_GPADC_GPAC, reg.val);
}

