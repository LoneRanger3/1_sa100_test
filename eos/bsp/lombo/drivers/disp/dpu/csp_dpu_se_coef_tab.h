/*
 * csp_dpu_se_coef_tab.h - Dpu se coef_tab for LomboTech head file
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

#ifndef __CSP_DPU_SE_COEF_TAB_H__
#define __CSP_DPU_SE_COEF_TAB_H__

/* scaling coefficients table start */
/* x table */
extern unsigned int sc_tab_8taps_type0[2][32];
extern unsigned int sc_tab_8taps_type1[2][32];
extern unsigned int sc_tab_8taps_type2[2][32];
extern unsigned int sc_tab_8taps_type3[2][32];
extern unsigned int sc_tab_8taps_type4[2][32];

/* y table */
extern unsigned int sc_tab_4taps_type0[32];
extern unsigned int sc_tab_4taps_type1[32];
extern unsigned int sc_tab_4taps_type2[32];
extern unsigned int sc_tab_4taps_type3[32];
extern unsigned int sc_tab_4taps_type4[32];

extern unsigned int sc_tab_2taps_type0[32];
extern unsigned int sc_tab_2taps_type1[32];
extern unsigned int sc_tab_2taps_type2[32];
extern unsigned int sc_tab_2taps_type3[32];
extern unsigned int sc_tab_2taps_type4[32];

extern unsigned int scaling_coef_tab8_arX[5][4];
extern unsigned int scaling_coef_tab4_arY[5][4];
extern unsigned int scaling_coef_tab2_arY[5][4];
/* scaling coefficients table end */

#endif /* __CSP_DPU_SE_COEF_TAB_H__ */

