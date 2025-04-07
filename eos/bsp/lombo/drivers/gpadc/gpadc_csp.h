/*
 * gpadc_csp.h - CPADC csp head file
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

#ifndef __GDAPC_CSP_H__
#define __GDAPC_CSP_H__

#include <rtthread.h>
#include <csp.h>
#include "sensor_csp.h"
#include "aux_csp.h"

typedef enum {
	GPADC_SRC_1KHZ = 0x0,
	GPADC_SRC_512HZ = 0x1,
	GPADC_SRC_256HZ = 0x2,
	GPADC_SRC_128HZ = 0x3,
	GPADC_SRC_1MHZ = 0x5,
} GPADC_SRC_TYPE;

/* Set GPADC enable */
void csp_gpadc_set_en(rt_bool_t en);

/* Set GPADC Calibration enable */
void csp_gpadc_set_cali_en(rt_bool_t en);

/* Set GPADC sample rate */
void csp_gpadc_set_sample_rate(GPADC_SRC_TYPE t);

/* Set GPADC sample data average enable */
void csp_gpadc_sda_en(rt_bool_t en);

/* setup gpadc clock */
void csp_gpadc_clk_cfg();

#endif /* __GDAPC_CSP_H__ */

