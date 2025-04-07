/*
 * pano_calib.h - pano calibrate code for LomboTech
 * pano calibrate interface and macro define
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

#ifndef __PANO_CALIB_H__
#define __PANO_CALIB_H__

#include "app_manage.h"
#include "case_config.h"
#include <debug.h>

#define PANO_BIRDBIEW_SOURCE_W 320
#define PANO_BIRDBIEW_SOURCE_H 320

#if 1
#define PANO_BIRDBIEW_DEST_X 24
#define PANO_BIRDBIEW_DEST_Y 240
#define PANO_BIRDBIEW_DEST_W 232
#define PANO_BIRDBIEW_DEST_H 162

#define PANO_PREVIEW_WIND_X 24
#define PANO_PREVIEW_WIND_Y 402
#define PANO_PREVIEW_WIND_W 232
#define PANO_PREVIEW_WIND_H 412

#define PANO_CARIMG_W 54
#define PANO_CARIMG_H 142
#else
#define PANO_BIRDBIEW_DEST_X 33
#define PANO_BIRDBIEW_DEST_Y 410
#define PANO_BIRDBIEW_DEST_W 320
#define PANO_BIRDBIEW_DEST_H 223

#define PANO_PREVIEW_WIND_X 33
#define PANO_PREVIEW_WIND_Y 633
#define PANO_PREVIEW_WIND_W 320
#define PANO_PREVIEW_WIND_H 560

#define PANO_CARIMG_W 74
#define PANO_CARIMG_H 196

#endif

#define PANO_OUT_BIN_PATH "mnt/data/cali_out.bin"

lb_int32 pano_calib_init_funcs(void);
lb_int32 pano_calib_uninit_funcs(void);
lb_int32 pano_calib_resp_funcs(void);
lb_int32 pano_cailb_unresp_funcs(void);

#endif /* __PANO_CALIB_H__ */
