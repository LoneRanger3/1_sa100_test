/*
 * pano_component.h - pano component.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef __PANO_COMPONENT_H__
#define __PANO_COMPONENT_H__
#include "display/oscl_display.h"

#define PANO_INPUT_PORT_INDEX 0
#define PANO_OUTPUT_PORT_INDEX 1

typedef struct pano_img {
	OMX_S8 path[64];
	OMX_COLOR_FORMATTYPE format; /* img format, support nv12,nv21 */
	OMX_S32 width;
	OMX_S32 height;
} pano_img_t;

typedef struct pano_cali_para {
	OMX_S32  box_rows;	/* the number of rows in the back and white grid */
	OMX_S32  box_cols;	/* the number of columns in the back and white grid */
	OMX_S32  box_width;	/* back and white grid width (unit: cm) */
	OMX_S32  box_height;	/* back and white grid height (unit: cm) */
	OMX_S32  dist_2_rear;	/* the bottom row of the grid to the rear of the car */
	OMX_S32  car_width;	/* car width */
	OMX_S32  car_length;	/* car length */
	OMX_S32  preview_width;
	OMX_S32  preview_height;

	OMX_S32  front_dist;    /* car front distance (unit: cm) */
	OMX_S32  rear_dist;	/* car rear distance (unit: cm)  */
	OMX_S32  align;	    /* rear direction. -1 or 1*/

	OMX_S32 use_ext_cali_img;  /* use extern calibration image, only test */
	pano_img_t cali_img; /* calibration imgae */
} pano_cali_para_t;

typedef struct pano_cali_contex {
	OMX_S32 cutline_dnthr;
	OMX_S32 cutline_upthr;
	OMX_S32 cutline;
	disp_rect_t car_rect;
} pano_cali_contex_t;

typedef struct pano_cali_out_data {
	OMX_S32 data_size;
	OMX_PTR data;  /* HDCalibParaPano */
} pano_cali_out_data_t;

typedef struct pano_init_para {
	OMX_S32 in_gps;	  /* use gps speed */
	OMX_S32 in_obd;	  /* use obd para */

	/* car para */
	OMX_S32 car_para_en;	/* whether to enabel use paras */
	OMX_S32 car_width;      /* car width (unit: cm) */

	/* warning line */
	OMX_S32 warning_line[3]; /* The warning lines at the rear, unit: cm */

	OMX_S32 use_ext_cutline;
	OMX_S32 culine;

	pano_img_t carb_img;
	char *data_format;
} pano_init_para_t;

#endif

