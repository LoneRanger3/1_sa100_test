/*
 * recorder_watermark.c - Standard functionality for take picture.
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
#define DBG_LEVEL         DBG_WARNING
#include <oscl.h>
#include <base_component.h>
#include "vrender_component.h"
#include "vrec_component.h"
#include "framework_common.h"
#include "lb_recorder.h"
#include "recorder_private.h"
#include "omx_vendor_lb.h"
#include "watermark.h"

#define BLENDING_BUF_ALIGN 4096

#if 1

/***
int write_output_picture(numeral_output_t *output)
{
	int color_format;
	int bpp = 0;
	int size = output->numeral_picture.width * output->numeral_picture.height * 4;

	FILE *flip_out = NULL;
	flip_out = fopen("out1.bin","wb");
	if (flip_out == NULL)
	{
		//lombo_printf("open out put file error\n");
	}
	color_format = output->colorspace;

	switch (color_format)
	{
	case VC_ARGB8888:
		fwrite(output->numeral_picture.data, 1, size, flip_out);
		fclose(flip_out);
		break;
	default :
		break;
	}
}
***/

void free_blending_numeral_pictures(numeral_input_t *p_blending_numeral_picture)
{
	int i = 0;
	p_blending_numeral_picture->input_picture_num = 0;
	for (i = 0; i < MAX_BLENDING_PICTURES; i++) {
		if (p_blending_numeral_picture->numeral_picture[i].data != 0) {
			oscl_free(p_blending_numeral_picture->numeral_picture[i].data);
			p_blending_numeral_picture->numeral_picture[i].data = NULL;
		}
	}
}

int check_input_blending_picture_param(numeral_input_t *ipnut)
{
	int i = 0;
	numeral_picture_t *picture = NULL;
	int height = 0;
	int input_picture_num;

	oscl_param_check(ipnut != NULL, -1, NULL);
	input_picture_num = ipnut->input_picture_num;
	picture = &(ipnut->numeral_picture[0]);
	height = picture->height;

	if (input_picture_num > MAX_BLENDING_PICTURES) {
		OSCL_LOGE("input blending picture num is too larger\n");
		return -1;
	}

	for (i = 1; i < input_picture_num; i++) {
		picture = &(ipnut->numeral_picture[i]);
		if (height != picture->height) {
			OSCL_LOGE("invalid height(%d):%d\n", i, picture->height);
			return -1;
		}
	}
	return 0;
}

int save_numeral_input_pictures(numeral_input_t *p_blending,
	numeral_input_t *p_picture_input)
{
	int i = 0;
	p_blending->input_picture_num = p_picture_input->input_picture_num;
	p_blending->colorspace = p_picture_input->colorspace;

	for (i = 0; i < p_blending->input_picture_num; i++) {
		p_blending->numeral_picture[i].width =
			p_picture_input->numeral_picture[i].width;
		p_blending->numeral_picture[i].height =
			p_picture_input->numeral_picture[i].height;
		p_blending->numeral_picture[i].picture_size =
			p_picture_input->numeral_picture[i].picture_size;

		if (p_blending->numeral_picture[i].data != NULL) {
			oscl_free(p_blending->numeral_picture[i].data);
			p_blending->numeral_picture[i].data = NULL;
		}
		if (p_blending->numeral_picture[i].data == NULL)
			p_blending->numeral_picture[i].data = oscl_zalloc(
				p_blending->numeral_picture[i].picture_size);

		if (p_blending->numeral_picture[i].data == NULL) {
			OSCL_LOGE("malloc buf error\n");
			return -1;
		} else
			memcpy(p_blending->numeral_picture[i].data,
				p_picture_input->numeral_picture[i].data,
				p_blending->numeral_picture[i].picture_size);
	}
	return 0;
}

static void free_watermark(numeral_output_t *mark)
{
	int tmp_index;

	if (mark == NULL)
		return;
	if (mark->numeral_picture.data)
		oscl_free_align(mark->numeral_picture.data);
	tmp_index = mark->blending_area_index;
	memset(mark, 0, sizeof(numeral_output_t));
	mark->blending_area_index = tmp_index;
}

int combine_digtal_picture(numeral_output_t *output,
	numeral_input_t *bnp, numeral_picture_index_t *wm_config)
{
	int width = 0;
	int height = 0;
	int align_width = 0;
	int align_height = 0;
	int total_index_num = 0;
	int i = 0;
	int j = 0;
	int index = 0;
	unsigned char *blending_buf;
	int buf_size;
	int bpp = 0;
	int ret = 0;
	numeral_picture_t *picture = NULL;

	switch (bnp->colorspace) {
	case OMX_COLOR_Format32bitARGB8888:
	case OMX_COLOR_Format32bitBGRA8888:
	case OMX_COLOR_Format32BitRGBA8888:
		bpp = 4;
		break;
	default:
		OSCL_LOGE("not support other format!\n");
		break;
	}
	oscl_param_check_exit(bpp != 0, -1, NULL);

	total_index_num = wm_config->total_index_num;
	for (i = 0; i < total_index_num; i++) {
		index = wm_config->index_array[i];
		if (index > bnp->input_picture_num) {
			OSCL_LOGE("index error %s - %d\n", __FILE__, __LINE__);
			return -1;
		}
		width += bnp->numeral_picture[index].width;
		if (height < bnp->numeral_picture[index].height)
			height = bnp->numeral_picture[index].height;
	}
	/* The width and height of the out put picture must be divid by 16*/
	align_width = (width + 15) & 0xFFFFFFF0;
	align_height = (height + 15) & 0xFFFFFFF0;

	blending_buf = output->numeral_picture.data;
	if (blending_buf != NULL) {
		if (output->numeral_picture.height < align_height
			|| output->numeral_picture.width < align_width) {
			free_watermark(output);
			blending_buf = NULL;
		}
	}
	buf_size = align_width * align_height * bpp;
	if (blending_buf == NULL)
		blending_buf = oscl_malloc_align(buf_size, BLENDING_BUF_ALIGN);
	oscl_param_check_exit(blending_buf != NULL, -1, NULL);

	output->numeral_picture.height = align_height;
	output->numeral_picture.width = align_width;
	output->numeral_picture.stride = align_width * bpp;
	output->numeral_picture.picture_size = align_height * align_width * bpp;
	output->colorspace = bnp->colorspace;
	/* The start pos of the out put picture must be divid by 16*/
	output->start_x_pos = wm_config->start_x_pos / 16 * 16;
	output->start_y_pos = wm_config->start_y_pos / 16 * 16;
	output->end_x_pos = output->start_x_pos + align_width;
	output->end_y_pos = output->start_y_pos  + align_height;
	output->numeral_picture.data = blending_buf;
	output->numeral_picture.phy_addr = oscl_virt_to_phys(blending_buf);
	output->blending_area_index = wm_config->blending_area_index;

	/* combine the picture */
	memset(blending_buf, 0x0, buf_size);
	for (j = 0; j < height; j++) {
		for (i = 0; i < total_index_num; i++) {
			unsigned char *src = NULL;
			index = wm_config->index_array[i];
			picture = &(bnp->numeral_picture[index]);
			src = picture->data + picture->width * bpp * j;
			memcpy(blending_buf, src, picture->width * bpp);
			blending_buf += picture->width * bpp;
		}
		if (align_width > width)
			blending_buf += (align_width - width) * bpp;
	}

EXIT:
	return ret;

}

watermark_priv_t *watermark_init(void)
{
	watermark_priv_t *handle;
	int i;
	handle = oscl_zalloc(sizeof(watermark_priv_t));
	if (handle)
		for (i = 0; i < MAX_WATERMARK_NUM; i++)
			handle->watermark[i].blending_area_index = i;
	pthread_mutex_init(&handle->lock, NULL);
	return handle;
}

void watermark_deinit(void *handle)
{
	int i;
	watermark_priv_t *priv = handle;
	if (priv == NULL)
		return;
	pthread_mutex_destroy(&priv->lock);
	free_blending_numeral_pictures(&priv->bnp);
	for (i = 0; i < MAX_WATERMARK_NUM; i++)
		if (priv->watermark[i].numeral_picture.data != NULL)
			oscl_free_align(priv->watermark[i].numeral_picture.data);

	oscl_free(priv);
}

int watermark_set_bnp(watermark_priv_t *priv, numeral_input_t *bnp)
{
	int ret = 0;

	oscl_param_check(priv != NULL, -1, NULL);

	pthread_mutex_lock(&priv->lock);
	ret = check_input_blending_picture_param(bnp);
	oscl_param_check_exit(ret == 0, -1, NULL);

	OSCL_LOGD("==");
	ret = save_numeral_input_pictures(&priv->bnp, bnp);
	OSCL_LOGD("==");
EXIT:
	if (ret != 0 && priv)
		free_blending_numeral_pictures(&priv->bnp);
	pthread_mutex_unlock(&priv->lock);
	return ret;
}

numeral_output_t *watermark_set_mark(watermark_priv_t *priv,
	numeral_picture_index_t *wm_config)
{
	int ret = 0;
	numeral_output_t *out = NULL;
	int index;

	oscl_param_check(priv != NULL, NULL, NULL);

	pthread_mutex_lock(&priv->lock);
	if (wm_config == NULL) {
		for (index = 0; index < MAX_WATERMARK_NUM; index++)
			free_watermark(&priv->watermark[index]);
		goto EXIT;
	}
	index = wm_config->blending_area_index;
	if (priv->bnp.input_picture_num == 0) {
		free_watermark(&priv->watermark[index]);
	} else {
		ret = combine_digtal_picture(&priv->watermark[index],
				&priv->bnp, wm_config);
	}
	if (ret == 0)
		out = &priv->watermark[index];

EXIT:
	pthread_mutex_unlock(&priv->lock);
	return out;
}

int watermark_get_markarray(watermark_priv_t *priv, numeral_output_t **mark_array)
{
	int ret = 0;

	oscl_param_check(priv != NULL, 0, NULL);
	oscl_param_check(mark_array != NULL, 0, NULL);

	pthread_mutex_lock(&priv->lock);
	ret = MAX_WATERMARK_NUM;
	*mark_array = priv->watermark;

	if (ret == 0) {
		*mark_array = NULL;
		pthread_mutex_unlock(&priv->lock);
	}
	return ret;
}

int watermark_put_markarray(watermark_priv_t *priv, numeral_output_t *mark_array)
{
	int ret = 0;

	oscl_param_check(priv != NULL, 0, NULL);
	oscl_param_check(mark_array != NULL, 0, NULL);

	if (mark_array == priv->watermark)
		pthread_mutex_unlock(&priv->lock);
	else {
		OSCL_LOGE("inavlid mark array:%x", mark_array);
		ret = -1;
	}
	return ret;
}

#endif
