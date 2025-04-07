/*
 * lb_pano_intf.c - pano interface.
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

#include "lb_pano.h"
#include "sizes.h"
#include <stdio.h>
#include <string.h>

#ifdef RT_USING_CPLUSPLUS
extern "C"
{
#endif

int pano_set_cb_boxrows(HDCheckerBoard *checkerBoard, int boxRows)
{
	int ret = -1;

	if (checkerBoard) {
		checkerBoard->boxRows = boxRows;
		ret = 0;
	}

	return ret;
}


int pano_set_cb_boxcols(HDCheckerBoard *checkerBoard, int boxCols)
{
	int ret = -1;

	if (checkerBoard) {
		checkerBoard->boxCols = boxCols;
		ret = 0;
	}

	return ret;
}

int pano_set_cb_boxwidth(HDCheckerBoard *checkerBoard, int boxWidth)
{
	int ret = -1;

	if (checkerBoard) {
		checkerBoard->boxWidth = boxWidth;
		ret = 0;
	}

	return ret;
}

int pano_set_cb_boxheight(HDCheckerBoard *checkerBoard, int boxheight)
{
	int ret = -1;

	if (checkerBoard) {
		checkerBoard->boxheight = boxheight;
		ret = 0;
	}

	return ret;
}

int pano_set_cb_dist2rear(HDCheckerBoard *checkerBoard, int dist2Rear)
{
	int ret = -1;

	if (checkerBoard) {
		checkerBoard->dist2Rear = dist2Rear;
		ret = 0;
	}

	return ret;
}

int pano_set_cb_carwidth(HDCheckerBoard *checkerBoard, int carWidth)
{
	int ret = -1;

	if (checkerBoard) {
		checkerBoard->carWidth = carWidth;
		ret = 0;
	}

	return ret;
}

int pano_set_cb_carlong(HDCheckerBoard *checkerBoard, int carLong)
{
	int ret = -1;

	if (checkerBoard) {
		checkerBoard->carLong = carLong;
		ret = 0;
	}

	return ret;
}

int pano_set_cb_previeww(HDCheckerBoard *checkerBoard, int preViewWidth)
{
	int ret = -1;

	if (checkerBoard) {
		checkerBoard->preViewWidth = preViewWidth;
		ret = 0;
	}

	return ret;
}

int pano_set_cb_previewh(HDCheckerBoard *checkerBoard, int preViewHeight)
{
	int ret = -1;

	if (checkerBoard) {
		checkerBoard->preViewHeight = preViewHeight;
		ret = 0;
	}

	return ret;
}

int pano_set_cb_frontdist(HDCheckerBoard *checkerBoard, int frontDist)
{
	int ret = -1;

	if (checkerBoard) {
		checkerBoard->frontDist = frontDist;
		ret = 0;
	}

	return ret;
}

int pano_set_cb_reardist(HDCheckerBoard *checkerBoard, int rearDist)
{
	int ret = -1;

	if (checkerBoard) {
		checkerBoard->rearDist = rearDist;
		ret = 0;
	}

	return ret;
}

int pano_set_cb_align(HDCheckerBoard *checkerBoard, int align)
{
	int ret = -1;

	if (checkerBoard) {
		checkerBoard->align = align;
		ret = 0;
	}

	return ret;
}


int pano_set_inis_mod_gps(HDIniSetPano *iniSetPano, int inGps)
{
	int ret = -1;

	if (iniSetPano) {
		iniSetPano->moduleEnable.inGps = inGps;
		ret = 0;
	}

	return ret;
}

int pano_set_inis_mod_obd(HDIniSetPano *iniSetPano, int inObd)
{
	int ret = -1;

	if (iniSetPano) {
		iniSetPano->moduleEnable.inObd = inObd;
		ret = 0;
	}

	return ret;
}

int pano_set_inis_carp_en(HDIniSetPano *iniSetPano, int enable)
{
	int ret = -1;

	if (iniSetPano) {
		iniSetPano->carPara.enable = enable;
		ret = 0;
	}

	return ret;
}

int pano_set_inis_carp_w(HDIniSetPano *iniSetPano, int width)
{
	int ret = -1;

	if (iniSetPano) {
		iniSetPano->carPara.width = width;
		ret = 0;
	}

	return ret;
}

int pano_set_inis_camp_fps(HDIniSetPano *iniSetPano, int fps)
{
	int ret = -1;

	if (iniSetPano) {
		iniSetPano->cameraPara[0].camInPara.fps = fps;
		ret = 0;
	}

	return ret;
}

int pano_set_inis_camp_w(HDIniSetPano *iniSetPano, int width)
{
	int ret = -1;

	if (iniSetPano) {
		iniSetPano->cameraPara[0].camInPara.imgSize.width = width;
		ret = 0;
	}

	return ret;
}

int pano_set_inis_camp_h(HDIniSetPano *iniSetPano, int height)
{
	int ret = -1;

	if (iniSetPano) {
		iniSetPano->cameraPara[0].camInPara.imgSize.height = height;
		ret = 0;
	}

	return ret;
}

int pano_set_inis_calipara(HDIniSetPano *iniSetPano, HDCalibOut *out)
{
	int ret = -1;

	if (iniSetPano) {
		memcpy(&iniSetPano->calibPara, &out->calibPara, sizeof(HDCalibParaPano));
		ret = 0;
	}

	return ret;
}

int pano_set_inis_carbimg(HDIniSetPano *iniSetPano, IMGYUVC *img)
{
	int ret = -1;

	if (iniSetPano) {
		iniSetPano->carBodyImg.size.width = img->size.width;
		iniSetPano->carBodyImg.size.height = img->size.height;
		iniSetPano->carBodyImg.yP = img->yP;
		iniSetPano->carBodyImg.uvP = img->uvP;
		ret = 0;
	}

	return ret;
}

int pano_set_inis_data_f(HDIniSetPano *iniSetPano, char *format)
{
	int ret = -1;

	if (iniSetPano) {
		iniSetPano->dataFormat = format;
		ret = 0;
	}

	return ret;
}

int pano_set_inis_warning_line(HDIniSetPano *iniSetPano, int *line, int size)
{
	int ret = -1;
	int i = 0;

	if (iniSetPano) {
		if (size == ARRAY_SIZE(iniSetPano->warnLines)) {
			for (i = 0; i < size; i++)
				iniSetPano->warnLines[i] = line[i];
			ret = 0;
		} else
			printf("Set pano warning line fail.%d\n", size);
	}

	return ret;
}

void pano_creat(HDIniSetPano *iniSetPano,HDIniGetPano *iniGetPano,void **dev)
{
	CreatPano(iniSetPano, iniGetPano, dev);
}

void pano_delete(void)
{
	DeletePano();
}

int pano_creat_calibrate(HDCalibOut *calibOut,HDCalibIn *calibIn)
{
	return CreatCalibrate(calibOut, calibIn);
}

void pano_delete_calibrate(void)
{
	DelteteCalibrate();
}

#ifdef RT_USING_CPLUSPLUS
}
#endif

