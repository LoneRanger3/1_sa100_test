/*
 * sensor_csp.h - gpadc sensor csp head file
 *
 * Copyright (C) 2016-2019, LomboTech Co.Ltd.
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

#include <rtthread.h>
#include <csp.h>

typedef enum {
	SAMPLE_RATE_8HZ = 0x0,
	SAMPLE_RATE_4HZ = 0x1,
	SAMPLE_RATE_2HZ = 0x2,
	SAMPLE_RATE_1HZ = 0x3,
} SENSOR_SAMPLE_RATE;

/* GPADC sensor csp function */

/* Set sensor enable */
void csp_sensor_set_en(rt_bool_t en);

/* Set sensor sample data average enable */
void csp_sensor_set_sda_en(rt_bool_t en);

/* Set sensor sample rate */
void csp_sensor_set_sample_rate(SENSOR_SAMPLE_RATE sr);

/* Set sensor down threshold interrupt enable */
void csp_sensor_set_dth_en(u32 sensor_no, rt_bool_t en);

/* Set sensor up threshold interrupt enable */
void csp_sensor_set_uth_en(u32 sensor_no, rt_bool_t en);

/* Set sensor up data interrupt enable */
void csp_sensor_set_data_en(u32 sensor_no, rt_bool_t en);

/* Get sensor down threshold interrupt pending status */
u32 csp_sensor_get_dth_pend(u32 sensor_no);

/* Get sensor up threshold interrupt pending status */
u32 csp_sensor_get_uth_pend(u32 sensor_no);

/* Get sensor data interrupt pending status */
u32 csp_sensor_get_data_pend(u32 sensor_no);

/* Clear sensor down threshold interrupt pending status */
void csp_sensor_clr_dth_pend(u32 sensor_no);

/* Clear sensor up threshold interrupt pending status */
void csp_sensor_clr_uth_pend(u32 sensor_no);

/* Clear sensor data interrupt pending status */
void csp_sensor_clr_data_pend(u32 sensor_no);

/* Get sensor calibration data */
u32 csp_sensor_get_cali_data(u32 sensor_no);

/* Get sensor data */
u32 csp_sensor_get_data(u32 sensor_no);

/* Set sensor down threshold data */
void csp_sensor_set_dth_data(u32 sensor_no, u32 val);

/* Set sensor up threshold data */
void csp_sensor_set_uth_data(u32 sensor_no, u32 val);

