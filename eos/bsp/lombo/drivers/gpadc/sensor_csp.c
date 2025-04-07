/*
 * sensor_csp.c - sensor csp operations for GPADC
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

#include "sensor_csp.h"
#include <debug.h>

#ifdef ARCH_LOMBO_N7V1
#define GPADC_SENSOR_TOTAL		3
#else
#define GPADC_SENSOR_TOTAL		2
#endif

/* Set sensor enable */
void csp_sensor_set_en(rt_bool_t en)
{
	reg_gpadc_gsc_t reg;

	reg.val = READREG32(VA_GPADC_GSC);
	reg.bits.sen = en ? 1 : 0;

	WRITEREG32(VA_GPADC_GSC, reg.val);
}

/* Set sensor sample data average enable */
void csp_sensor_set_sda_en(rt_bool_t en)
{
	reg_gpadc_gsc_t reg;

	reg.val = READREG32(VA_GPADC_GSC);
	reg.bits.ssdaen = en ? 1 : 0;

	WRITEREG32(VA_GPADC_GSC, reg.val);
}

/* Set sensor sample rate */
void csp_sensor_set_sample_rate(SENSOR_SAMPLE_RATE sr)
{
	reg_gpadc_gsc_t reg;

	reg.val = READREG32(VA_GPADC_GSC);
	reg.bits.ssr = sr;

	WRITEREG32(VA_GPADC_GSC, reg.val);
}

/* Set SENSORx down threshold interrupt enable (x = 0, 1, 2) */
void csp_sensor_set_dth_en(u32 sensor_no, rt_bool_t en)
{
	reg_gpadc_gpaie_t reg;
	u32 e;

	RT_ASSERT(sensor_no < GPADC_SENSOR_TOTAL);

	e = en ? 1 : 0;
	reg.val = READREG32(VA_GPADC_GPAIE);
	switch (sensor_no) {
	case 0:
		reg.bits.s0die = e;
		break;
	case 1:
		reg.bits.s1die = e;
		break;
#ifdef ARCH_LOMBO_N7V1
	case 2:
		reg.bits.s2die = e;
		break;
#endif
	default:
		LOG_E("sensor_no:%d error", sensor_no);
	}

	WRITEREG32(VA_GPADC_GPAIE, reg.val);
}

/* Set SENSORx up threshold interrupt enable (x = 0, 1, 2) */
void csp_sensor_set_uth_en(u32 sensor_no, rt_bool_t en)
{
	reg_gpadc_gpaie_t reg;
	u32 e;

	RT_ASSERT(sensor_no < GPADC_SENSOR_TOTAL);

	e = en ? 1 : 0;
	reg.val = READREG32(VA_GPADC_GPAIE);
	switch (sensor_no) {
	case 0:
		reg.bits.s0uie = e;
		break;
	case 1:
		reg.bits.s1uie = e;
		break;
#ifdef ARCH_LOMBO_N7V1
	case 2:
		reg.bits.s2uie = e;
		break;
#endif
	default:
		LOG_E("sensor_no:%d error", sensor_no);
	}

	WRITEREG32(VA_GPADC_GPAIE, reg.val);
}

/* Set SENSORx up data interrupt enable (x = 0, 1, 2) */
void csp_sensor_set_data_en(u32 sensor_no, rt_bool_t en)
{
	reg_gpadc_gpaie_t reg;
	u32 e;

	RT_ASSERT(sensor_no < GPADC_SENSOR_TOTAL);

	e = en ? 1 : 0;
	reg.val = READREG32(VA_GPADC_GPAIE);
	switch (sensor_no) {
	case 0:
		reg.bits.s0datie = e;
		break;
	case 1:
		reg.bits.s1datie = e;
		break;
#ifdef ARCH_LOMBO_N7V1
	case 2:
		reg.bits.s2datie = e;
		break;
#endif
	default:
		LOG_E("sensor_no:%d error", sensor_no);
	}

	WRITEREG32(VA_GPADC_GPAIE, reg.val);
}

/* Get sensor down threshold interrupt pending status */
u32 csp_sensor_get_dth_pend(u32 sensor_no)
{
	reg_gpadc_gpais_t reg;
	u32 pend;

	RT_ASSERT(sensor_no < GPADC_SENSOR_TOTAL);

	reg.val = READREG32(VA_GPADC_GPAIS);
	switch (sensor_no) {
	case 0:
		pend = reg.bits.s0dis;
		break;
	case 1:
		pend = reg.bits.s1dis;
		break;
#ifdef ARCH_LOMBO_N7V1
	case 2:
		pend = reg.bits.s2dis;
		break;
#endif
	default:
		LOG_E("sensor_no:%d error", sensor_no);
		pend = 0;
	}

	return pend;
}

/* Get sensor up threshold interrupt pending status */
u32 csp_sensor_get_uth_pend(u32 sensor_no)
{
	reg_gpadc_gpais_t reg;
	u32 pend;

	RT_ASSERT(sensor_no < GPADC_SENSOR_TOTAL);

	reg.val = READREG32(VA_GPADC_GPAIS);
	switch (sensor_no) {
	case 0:
		pend = reg.bits.s0uis;
		break;
	case 1:
		pend = reg.bits.s1uis;
		break;
#ifdef ARCH_LOMBO_N7V1
	case 2:
		pend = reg.bits.s2uis;
		break;
#endif
	default:
		LOG_E("sensor_no:%d error", sensor_no);
		pend = 0;
	}

	return pend;
}

/* Get sensor data interrupt pending status */
u32 csp_sensor_get_data_pend(u32 sensor_no)
{
	reg_gpadc_gpais_t reg;
	u32 pend;

	RT_ASSERT(sensor_no < GPADC_SENSOR_TOTAL);

	reg.val = READREG32(VA_GPADC_GPAIS);
	switch (sensor_no) {
	case 0:
		pend = reg.bits.s0datis;
		break;
	case 1:
		pend = reg.bits.s1datis;
		break;
#ifdef ARCH_LOMBO_N7V1
	case 2:
		pend = reg.bits.s2datis;
		break;
#endif
	default:
		LOG_E("sensor_no:%d error", sensor_no);
		pend = 0;
	}

	return pend;
}

/* Clear sensor down threshold interrupt pending status */
void csp_sensor_clr_dth_pend(u32 sensor_no)
{
	reg_gpadc_gpaic_t reg;

	RT_ASSERT(sensor_no < GPADC_SENSOR_TOTAL);

	reg.val = READREG32(VA_GPADC_GPAIC);
	switch (sensor_no) {
	case 0:
		reg.bits.s0dpc = 1;
		break;
	case 1:
		reg.bits.s1dpc = 1;
		break;
#ifdef ARCH_LOMBO_N7V1
	case 2:
		reg.bits.s2dpc = 1;
		break;
#endif
	default:
		LOG_E("sensor_no:%d error", sensor_no);
	}

	WRITEREG32(VA_GPADC_GPAIC, reg.val);
}

/* Clear sensor up threshold interrupt pending status */
void csp_sensor_clr_uth_pend(u32 sensor_no)
{
	reg_gpadc_gpaic_t reg;

	RT_ASSERT(sensor_no < GPADC_SENSOR_TOTAL);

	reg.val = READREG32(VA_GPADC_GPAIC);
	switch (sensor_no) {
	case 0:
		reg.bits.s0upc = 1;
		break;
	case 1:
		reg.bits.s1upc = 1;
		break;
#ifdef ARCH_LOMBO_N7V1
	case 2:
		reg.bits.s2upc = 1;
		break;
#endif
	default:
		LOG_E("sensor_no:%d error", sensor_no);
	}

	WRITEREG32(VA_GPADC_GPAIC, reg.val);
}

/* Clear sensor data interrupt pending status */
void csp_sensor_clr_data_pend(u32 sensor_no)
{
	reg_gpadc_gpaic_t reg;

	RT_ASSERT(sensor_no < GPADC_SENSOR_TOTAL);

	reg.val = READREG32(VA_GPADC_GPAIC);
	switch (sensor_no) {
	case 0:
		reg.bits.s0datpc = 1;
		break;
	case 1:
		reg.bits.s1datpc = 1;
		break;
#ifdef ARCH_LOMBO_N7V1
	case 2:
		reg.bits.s2datpc = 1;
		break;
#endif
	default:
		LOG_E("sensor_no:%d error", sensor_no);
	}

	WRITEREG32(VA_GPADC_GPAIC, reg.val);
}

/* Get sensor calibration data */
u32 csp_sensor_get_cali_data(u32 sensor_no)
{
	u32 val;

	RT_ASSERT(sensor_no < GPADC_SENSOR_TOTAL);

	switch (sensor_no) {
	case 0:
	{
		reg_gpadc_s0cdat_t reg;

		reg.val = READREG32(VA_GPADC_S0CDAT);
		val = reg.bits.s0cdat;
	}
		break;
	case 1:
	{
		reg_gpadc_s1cdat_t reg;

		reg.val = READREG32(VA_GPADC_S1CDAT);
		val = reg.bits.s1cdat;
	}
		break;
#ifdef ARCH_LOMBO_N7V1
	case 2:
	{
		reg_gpadc_s2cdat_t reg;

		reg.val = READREG32(VA_GPADC_S2CDAT);
		val = reg.bits.s2cdat;
	}
		break;
#endif
	default:
		LOG_E("sensor_no:%d error", sensor_no);
		val = 0;
	}

	return val;
}

/* Get sensor data */
u32 csp_sensor_get_data(u32 sensor_no)
{
	u32 val;

	RT_ASSERT(sensor_no < GPADC_SENSOR_TOTAL);

	switch (sensor_no) {
	case 0:
	{
		reg_gpadc_s0dat_t reg;

		reg.val = READREG32(VA_GPADC_S0DAT);
		val = reg.bits.s0adcdat;
	}
		break;
	case 1:
	{
		reg_gpadc_s1dat_t reg;

		reg.val = READREG32(VA_GPADC_S1DAT);
		val = reg.bits.s1adcdat;
	}
		break;
#ifdef ARCH_LOMBO_N7V1
	case 2:
	{
		reg_gpadc_s2dat_t reg;

		reg.val = READREG32(VA_GPADC_S2DAT);
		val = reg.bits.s2adcdat;
	}
		break;
#endif
	default:
		LOG_E("sensor_no:%d error", sensor_no);
		val = 0;
	}

	return val;
}

/* Set sensor down threshold data */
void csp_sensor_set_dth_data(u32 sensor_no, u32 val)
{
	RT_ASSERT(sensor_no < GPADC_SENSOR_TOTAL);

	switch (sensor_no) {
	case 0:
	{
		reg_gpadc_s0dtdat_t reg;

		reg.val = READREG32(VA_GPADC_S0DTDAT);
		reg.bits.s0datdt = val;
		WRITEREG32(VA_GPADC_S0DTDAT, reg.val);
	}
		break;
	case 1:
	{
		reg_gpadc_s1dtdat_t reg;

		reg.val = READREG32(VA_GPADC_S1DTDAT);
		reg.bits.s1datdt = val;
		WRITEREG32(VA_GPADC_S1DTDAT, reg.val);
	}
		break;
#ifdef ARCH_LOMBO_N7V1
	case 2:
	{
		reg_gpadc_s2dtdat_t reg;

		reg.val = READREG32(VA_GPADC_S2DTDAT);
		reg.bits.s2datdt = val;
		WRITEREG32(VA_GPADC_S2DTDAT, reg.val);
	}
		break;
#endif
	default:
		LOG_E("sensor_no:%d error", sensor_no);
	}
}

/* Set sensor up threshold data */
void csp_sensor_set_uth_data(u32 sensor_no, u32 val)
{
	RT_ASSERT(sensor_no < GPADC_SENSOR_TOTAL);

	switch (sensor_no) {
	case 0:
	{
		reg_gpadc_s0utdat_t reg;

		reg.val = READREG32(VA_GPADC_S0UTDAT);
		reg.bits.s0datut = val;
		WRITEREG32(VA_GPADC_S0UTDAT, reg.val);
	}
		break;
	case 1:
	{
		reg_gpadc_s1utdat_t reg;

		reg.val = READREG32(VA_GPADC_S1UTDAT);
		reg.bits.s1datut = val;
		WRITEREG32(VA_GPADC_S1UTDAT, reg.val);
	}
		break;
#ifdef ARCH_LOMBO_N7V1
	case 2:
	{
		reg_gpadc_s2utdat_t reg;

		reg.val = READREG32(VA_GPADC_S2UTDAT);
		reg.bits.s2datut = val;
		WRITEREG32(VA_GPADC_S2UTDAT, reg.val);
	}
		break;
#endif
	default:
		LOG_E("sensor_no:%d error", sensor_no);
	}
}

