/*
 * aux_csp.h - gpadc aux csp head file
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

/* GPADC AUXIN csp function */

/* Set AUXIN enable */
void csp_aux_set_en(u32 aux_no, rt_bool_t en);

/* Set AUXIN threshold interrupt enable */
void csp_aux_set_th_en(u32 aux_no, rt_bool_t en);

/* Set AUXIN data interrupt enable */
void csp_aux_set_data_en(u32 aux_no, rt_bool_t en);

/* Get AUXIN threshold interrupt enable */
u32 csp_aux_get_th_en(u32 aux_no);

/* Get AUXIN threshold interrupt pending status */
u32 csp_aux_get_th_pend(u32 aux_no);

/* Get AUXIN data interrupt pending status */
u32 csp_aux_get_data_pend(u32 aux_no);

/* Clear AUXIN threshold interrupt pending status */
void csp_aux_clr_th_pend(u32 aux_no);

/* Clear AUXIN data interrupt pending status */
void csp_aux_clr_data_pend(u32 aux_no);

/* Get AUXIN data */
u32 csp_aux_get_data(u32 aux_no);

/* Get AUXINx sample hold data */
u32 csp_aux_get_sh_data(u32 aux_no);

/* Set AUXINx threashold data */
void csp_aux_set_th_data(u32 aux_no, u32 val);

