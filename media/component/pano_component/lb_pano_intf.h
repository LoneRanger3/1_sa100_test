/*
 * lb_pano_intf.h - pano interface.
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

#ifndef __LB_PANO_INTF_H__
#define __LB_PANO_INTF_H__

#include "lb_pano.h"

int pano_set_cb_boxrows(HDCheckerBoard *checkerBoard, int boxRows);
int pano_set_cb_boxcols(HDCheckerBoard *checkerBoard, int boxCols);
int pano_set_cb_boxwidth(HDCheckerBoard *checkerBoard, int boxWidth);
int pano_set_cb_boxheight(HDCheckerBoard *checkerBoard, int boxheight);
int pano_set_cb_dist2rear(HDCheckerBoard *checkerBoard, int dist2Rear);
int pano_set_cb_carwidth(HDCheckerBoard *checkerBoard, int carWidth);
int pano_set_cb_carlong(HDCheckerBoard *checkerBoard, int carLong);
int pano_set_cb_previeww(HDCheckerBoard *checkerBoard, int preViewWidth);
int pano_set_cb_previewh(HDCheckerBoard *checkerBoard, int preViewHeight);
int pano_set_cb_frontdist(HDCheckerBoard *checkerBoard, int frontDist);
int pano_set_cb_reardist(HDCheckerBoard *checkerBoard, int rearDist);
int pano_set_cb_align(HDCheckerBoard *checkerBoard, int align);
int pano_set_inis_mod_gps(HDIniSetPano *iniSetPano, int inGps);
int pano_set_inis_mod_obd(HDIniSetPano *iniSetPano, int inObd);
int pano_set_inis_carp_en(HDIniSetPano *iniSetPano, int enable);
int pano_set_inis_carp_w(HDIniSetPano *iniSetPano, int width);
int pano_set_inis_camp_fps(HDIniSetPano *iniSetPano, int fps);
int pano_set_inis_camp_w(HDIniSetPano *iniSetPano, int width);
int pano_set_inis_camp_h(HDIniSetPano *iniSetPano, int height);
int pano_set_inis_calipara(HDIniSetPano *iniSetPano, HDCalibOut *out);
int pano_set_inis_carbimg(HDIniSetPano *iniSetPano, IMGYUVC *img);
int pano_set_inis_data_f(HDIniSetPano *iniSetPano, char *format);
void pano_creat(HDIniSetPano *iniSetPano, HDIniGetPano *iniGetPano, void **dev);
void pano_delete(void);
int pano_creat_calibrate(HDCalibOut *calibOut, HDCalibIn *calibIn);
void pano_delete_calibrate(void);
int pano_set_inis_warning_line(HDIniSetPano *iniSetPano, int *line, int size);

#endif
