/*
 * aux_csp.c - aux csp operations for GPADC
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

#include "aux_csp.h"
#include <debug.h>

#define GPADC_AUX_TOTAL			2

/* Set AUXIN enable */
void csp_aux_set_en(u32 aux_no, rt_bool_t en)
{
	reg_gpadc_gpac_t reg;

	RT_ASSERT(aux_no < GPADC_AUX_TOTAL);

	reg.val = READREG32(VA_GPADC_GPAC);
	if (0 == aux_no)
		reg.bits.auxin0e = en ? 1 : 0;
	else if (1 == aux_no)
		reg.bits.auxin1e = en ? 1 : 0;

	WRITEREG32(VA_GPADC_GPAC, reg.val);
}

/* Set AUXINx threshold interrupt enable  (x = 0, 1) */
void csp_aux_set_th_en(u32 aux_no, rt_bool_t en)
{
	reg_gpadc_gpaie_t reg;

	RT_ASSERT(aux_no < GPADC_AUX_TOTAL);

	reg.val = READREG32(VA_GPADC_GPAIE);
	if (0 == aux_no)
		reg.bits.aux0ie = en ? 1 : 0;
	else if (1 == aux_no)
		reg.bits.aux1ie = en ? 1 : 0;

	WRITEREG32(VA_GPADC_GPAIE, reg.val);
}

/* Set AUXINx data interrupt enable  (x = 0, 1) */
void csp_aux_set_data_en(u32 aux_no, rt_bool_t en)
{
	reg_gpadc_gpaie_t reg;

	RT_ASSERT(aux_no < GPADC_AUX_TOTAL);

	reg.val = READREG32(VA_GPADC_GPAIE);
	if (0 == aux_no)
		reg.bits.ain0datie = en ? 1 : 0;
	else if (1 == aux_no)
		reg.bits.ain1datie = en ? 1 : 0;

	WRITEREG32(VA_GPADC_GPAIE, reg.val);
}

/* Get AUXINx threshold interrupt enable (x = 0, 1) */
u32 csp_aux_get_th_en(u32 aux_no)
{
	reg_gpadc_gpaie_t reg;

	RT_ASSERT(aux_no < GPADC_AUX_TOTAL);

	reg.val = READREG32(VA_GPADC_GPAIE);
	if (0 == aux_no)
		return reg.bits.aux0ie;
	else if (1 == aux_no)
		return reg.bits.aux1ie;
	else {
		LOG_E("aux_no error;%d\n", aux_no);
		return 0;
	}
}

/* Get AUXIN threshold interrupt pending status */
u32 csp_aux_get_th_pend(u32 aux_no)
{
	reg_gpadc_gpais_t reg;
	u32 pend;

	RT_ASSERT(aux_no < GPADC_AUX_TOTAL);

	reg.val = READREG32(VA_GPADC_GPAIS);
	if (0 == aux_no)
		pend = reg.bits.aux0is;
	else if (1 == aux_no)
		pend = reg.bits.aux1is;
	else
		pend = 0;

	return pend;
}

/* Get AUXIN data interrupt pending status */
u32 csp_aux_get_data_pend(u32 aux_no)
{
	reg_gpadc_gpais_t reg;
	u32 pend;

	RT_ASSERT(aux_no < GPADC_AUX_TOTAL);

	reg.val = READREG32(VA_GPADC_GPAIS);
	if (0 == aux_no)
		pend = reg.bits.ain0datis;
	else if (1 == aux_no)
		pend = reg.bits.ain1datis;
	else
		pend = 0;

	return pend;
}


/* Clear AUXIN threshold interrupt pending status */
void csp_aux_clr_th_pend(u32 aux_no)
{
	reg_gpadc_gpaic_t reg;

	RT_ASSERT(aux_no < GPADC_AUX_TOTAL);

	reg.val = READREG32(VA_GPADC_GPAIC);
	if (0 == aux_no)
		reg.bits.auxin0pc = 1;
	else if (1 == aux_no)
		reg.bits.auxin1pc = 1;

	WRITEREG32(VA_GPADC_GPAIC, reg.val);
}

/* Clear AUXIN data interrupt pending status */
void csp_aux_clr_data_pend(u32 aux_no)
{
	reg_gpadc_gpaic_t reg;

	RT_ASSERT(aux_no < GPADC_AUX_TOTAL);

	reg.val = READREG32(VA_GPADC_GPAIC);
	if (0 == aux_no)
		reg.bits.ain0datpc = 1;
	else if (1 == aux_no)
		reg.bits.ain1datpc = 1;

	WRITEREG32(VA_GPADC_GPAIC, reg.val);
}

/* Get AUXINx data (x = 0, 1) */
u32 csp_aux_get_data(u32 aux_no)
{
	u32 val;

	RT_ASSERT(aux_no < GPADC_AUX_TOTAL);

	if (0 == aux_no) {
		reg_gpadc_ain0dat_t reg;

		reg.val = READREG32(VA_GPADC_AIN0DAT);
		val = reg.bits.dat;
	} else if (1 == aux_no) {
		reg_gpadc_ain1dat_t reg;

		reg.val = READREG32(VA_GPADC_AIN1DAT);
		val = reg.bits.dat;
	} else {
		LOG_E("aux_no: %d error", aux_no);
		val = 0;
	}

	return val;
}

/* Get AUXINx sample hold data */
u32 csp_aux_get_sh_data(u32 aux_no)
{
	u32 val;

	RT_ASSERT(aux_no < GPADC_AUX_TOTAL);

	if (0 == aux_no) {
		reg_gpadc_aux0hdat_t reg;

		reg.val = READREG32(VA_GPADC_AUX0HDAT);
		val = reg.bits.auxin0shdat;
	} else if (1 == aux_no) {
		reg_gpadc_aux1hdat_t reg;

		reg.val = READREG32(VA_GPADC_AUX1HDAT);
		val = reg.bits.auxin1hdat;
	} else {
		LOG_E("aux_no: %d error", aux_no);
		val = 0;
	}

	return val;
}

/* Set AUXINx threashold data */
void csp_aux_set_th_data(u32 aux_no, u32 val)
{
	RT_ASSERT(aux_no < GPADC_AUX_TOTAL);

	if (0 == aux_no) {
		reg_gpadc_aux0tdat_t reg;

		reg.val = READREG32(VA_GPADC_AUX0TDAT);
		reg.bits.auxin0tdat = val;
		WRITEREG32(VA_GPADC_AUX0TDAT, reg.val);
	} else if (1 == aux_no) {
		reg_gpadc_aux1tdat_t reg;

		reg.val = READREG32(VA_GPADC_AUX1TDAT);
		reg.bits.auxin1tdat = val;
		WRITEREG32(VA_GPADC_AUX1TDAT, reg.val);
	}
}

