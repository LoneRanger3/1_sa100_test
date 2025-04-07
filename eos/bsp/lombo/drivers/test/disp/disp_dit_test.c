/*
 * disp_dit_test.c - Disp dit test module driver code for LomboTech
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
#include "disp_test.h"

#define BUF_LEN		256
#define DIT_DIR		"/mnt/sdcard/dit_src/"
#define DIT_DIR_LEN	sizeof(DIT_DIR)

typedef struct tag_dit_txt_parameter {
	char if_name[64]; /* input file name */
	char of_name[64]; /* output file name */
	u8 mode;
	u32 format;
	u32 width;
	u32 height;
} dit_txt_parameter_t;

/* delete all (' ') ('\t') ('\r') ('\n') ('\v') ('\f) */
static char *t_trim(char *sz_output, const char *sz_input)
{
	const char *p_in = sz_input;
	char *p_out = sz_output;

	RT_ASSERT(sz_input != NULL);
	RT_ASSERT(sz_output != NULL);

	/* LOG_I("%s\n", sz_input); */

	for (p_in = sz_input; p_in < sz_input + strlen(sz_input) - 1; p_in++) {
		if (!isspace((unsigned char)(*p_in))) {
			(*p_out) = *p_in;
			p_out++;
		}
	}
	*p_out = '\0';
	/* LOG_I("sz_output:%s\n", sz_output); */
	return sz_output;
}

int dit_para_cfg(char *buf, u32 len, void *para)
{
	char *data;
	int symbol_len, value_len;
	dit_txt_parameter_t *dtp = (dit_txt_parameter_t *)para;

	data = strchr(buf, '=');
	if (data == NULL) { /* no include '=' that mean illegal item */
		LOG_E("not include =");
		return DISP_ERROR;
	}

#if 0
	if (strchr(buf, "file") != NULL)
		return DISP_ERROR;
#endif
	symbol_len = data - buf;
	value_len = len - symbol_len - 1; /* 1 is for '=' */

	if ((symbol_len <= 0) || (value_len <= 0)) {
		LOG_E("len err");
		return DISP_ERROR;
	}

	data = data + 1; /* for '=' */

	if (strncmp(buf, "input_file", sizeof("input_file") - 1) == 0) {
		memcpy(dtp->if_name, data, value_len);
		LOG_D("if_name[%s]", dtp->if_name);
	} else if (strncmp(buf, "output_file", sizeof("output_file") - 1) == 0) {
		memcpy(dtp->of_name, data, value_len);
		LOG_D("of_name[%s]", dtp->of_name);
	} else if (strncmp(buf, "mode", sizeof("mode") - 1) == 0) {
		dtp->mode = DISP_DIT_MODE0 + atoi(data);
		LOG_D("mode[%d]", dtp->mode);
	} else if (strncmp(buf, "format", sizeof("format") - 1) == 0) {
		if (strncmp(data, "YU12", sizeof("YU12") - 1) == 0) {
			dtp->format = DISP_FORMAT_YUV420;
			LOG_D("format[YU12]");
		} else if (strncmp(data, "YV12", sizeof("YV12") - 1) == 0) {
			dtp->format = DISP_FORMAT_YVU420;
			LOG_D("format[YV12]");
		} else if (strncmp(data, "NV12", sizeof("NV12") - 1) == 0) {
			dtp->format = DISP_FORMAT_NV12;
			LOG_D("format[NV12]");
		} else if (strncmp(data, "NV21", sizeof("NV21") - 1) == 0) {
			dtp->format = DISP_FORMAT_NV21;
			LOG_D("format[NV21]");
		} else if (strncmp(data, "YU16", sizeof("YU16") - 1) == 0) {
			dtp->format = DISP_FORMAT_YUV422;
			LOG_D("format[YU16]");
		} else if (strncmp(data, "YV16", sizeof("YV16") - 1) == 0) {
			dtp->format = DISP_FORMAT_YVU422;
			LOG_D("format[YV16]");
		} else if (strncmp(data, "NV16", sizeof("NV16") - 1) == 0) {
			dtp->format = DISP_FORMAT_NV16;
			LOG_D("format[NV16]");
		} else if (strncmp(data, "NV61", sizeof("NV61") - 1) == 0) {
			dtp->format = DISP_FORMAT_NV61;
			LOG_D("format[NV61]");
		}
	} else if (strncmp(buf, "width", sizeof("width") - 1) == 0) {
		dtp->width = atoi(data);
		LOG_D("width[%d]", dtp->width);
	} else if (strncmp(buf, "height", sizeof("height") - 1) == 0) {
		dtp->height = atoi(data);
		LOG_D("height[%d]", dtp->height);
	}
	LOG_D("symbol_len[%d] value_len[%d]", symbol_len, value_len);

	return DISP_OK;
}

int dit_txt_translate(char *file_name, void *para)
{
	FILE *fp;

	int str_len = 0;
	int line_num = 0;

	char *buf_i;
	char *buf_o;

	buf_i = rt_zalloc(BUF_LEN);
	if (NULL == buf_i)
		return DISP_ERROR;

	buf_o = rt_zalloc(BUF_LEN);
	if (NULL == buf_o)
		return DISP_ERROR;

	fp = fopen(file_name, "r");
	if (fp == NULL) {
		LOG_I("openfile [%s] error [%s]\n", file_name, strerror(errno));
		return DISP_ERROR;
	}

	/* get a line from file */
	while (!feof(fp) && fgets(buf_i, BUF_LEN - 1, fp) != NULL) {
		line_num++;
		str_len = strlen(buf_i);
		if (str_len <= 0) {
			LOG_I("error line %d:str_len error:%d, consider file error\n",
				line_num, str_len);
			break;
		}

		if (str_len == 1) { /* ignore empty line */
			continue;
		}

		/* no include '=' that mean illegal item */
		if (strchr(buf_i, '=') == NULL)
			continue;

		t_trim(buf_o, buf_i); /* delete space */

		str_len = strlen(buf_o);
		RT_ASSERT(str_len >= 0);
		dit_para_cfg(buf_o, str_len, para);

		LOG_I("%d[%d]:%s\n", line_num, str_len, buf_o);
	}

	fclose(fp);

	rt_free(buf_i);
	rt_free(buf_o);
	return DISP_OK;
}

void dit_test_entry(rt_device_disp_ops_t *disp_ops_p, dit_txt_parameter_t *dtp)
{
	bool is_422 = false;
	int fd_i, fd_o;
	u32 img_w, img_h, write_size, read_size, size, uv_off[2];
	u32 i, img_cnt, real_cnt, file_len, file_offset;
	char *buf_data;
	char *img_data[3];
	disp_dit_cfg_t cfgs;
	char *file_name;

	LOG_I("dit_test_entry start");
	RT_ASSERT(disp_ops_p != NULL);

	img_w = dtp->width;
	img_h = dtp->height;

	rt_memset(&cfgs, 0x00, sizeof(disp_dit_cfg_t));

	cfgs.format = dtp->format;
	cfgs.mode = DISP_DIT_MODE0;
	cfgs.width = img_w;
	cfgs.height = img_h;

	file_name = rt_zalloc(BUF_LEN);
	if (NULL == file_name)
		return;

	if (dtp->format == DISP_FORMAT_YUV420 ||
		dtp->format == DISP_FORMAT_YVU420 ||
		dtp->format == DISP_FORMAT_NV12 ||
		dtp->format == DISP_FORMAT_NV21) {
		size = img_w * img_h * 3 / 2;
	} else if (dtp->format == DISP_FORMAT_YUV422 ||
		dtp->format == DISP_FORMAT_YVU422 ||
		dtp->format == DISP_FORMAT_NV16 ||
		dtp->format == DISP_FORMAT_NV61) {
		is_422 = true;
		size = img_w * img_h * 2;
	} else {
		LOG_E("illegal format");
		return;
	}

	memcpy(file_name, DIT_DIR, DIT_DIR_LEN);
	strcat(file_name, (const char *)dtp->if_name);
	fd_i = open((const char *)file_name, O_RDONLY);
	if (fd_i <= 0) {
		LOG_E("open input file[%s] err", file_name);
		return;
	}

	file_len = lseek(fd_i, 0, SEEK_END);
	lseek(fd_i, 0, SEEK_SET);
	file_offset = 0;

	rt_memset(file_name, 0x00, BUF_LEN);
	memcpy(file_name, DIT_DIR, DIT_DIR_LEN);
	strcat(file_name, (const char *)dtp->of_name);
	fd_o = open((const char *)file_name, O_RDWR | O_CREAT);
	if (fd_o <= 0) {
		LOG_E("open output file[%s] err", file_name);
		return;
	}

	RT_ASSERT(size > 0);

	LOG_I("size %d", size);
	buf_data = rt_malloc_align(size, 32);
	RT_ASSERT(buf_data != NULL);
	LOG_D("buf_data addr %p", buf_data);

	img_cnt = 0;

	for (i = 0; i < 3; i++) {
		img_data[i] = rt_malloc_align(size, 32);
		RT_ASSERT(img_data[i] != NULL);
		LOG_D("img_data[%d] addr %p", i, img_data[i]);

		read_size = read(fd_i, img_data[i], size);
		LOG_I("read size %d", read_size);
		if (read_size != size) {
			close(fd_i);
			LOG_E("read image data err");
			return;
		}
		file_offset += size;
	}

	uv_off[0] = img_w * img_h;
	if (is_422)
		uv_off[1] = img_w * img_h * 3 / 2;
	else
		uv_off[1] = img_w * img_h * 5 / 4;

	while (1) {
		LOG_I("dit img_cnt %d", img_cnt);
		for (i = 0; i < 3; i++) {
			real_cnt = (img_cnt + i) % 3;
			cfgs.infb[i].addr[0] = (u32)img_data[real_cnt];
			cfgs.infb[i].addr[1] = (u32)(img_data[real_cnt] + uv_off[0]);
			cfgs.infb[i].addr[2] = (u32)(img_data[real_cnt] + uv_off[1]);
		}

		cfgs.outfb.addr[0] = (u32)buf_data;
		cfgs.outfb.addr[1] = (u32)(buf_data + uv_off[0]);
		cfgs.outfb.addr[2] = (u32)(buf_data + uv_off[1]);

		disp_ops_p->disp_dit_process(&cfgs);

		write_size = write(fd_o, buf_data, size);
		/* LOG_I("write size %d", write_size); */
		if (write_size != size) {
			close(fd_o);
			LOG_E("write image data err");
			return;
		}

		if (file_offset >= file_len)
			break;

		read_size = read(fd_i, img_data[img_cnt % 3], size);
		if (read_size != size) {
			close(fd_i);
			LOG_E("read image data err");
			return;
		}
		file_offset += size;
		img_cnt++;
#if 0
		if (img_cnt > 20)
			break;
#endif
	}

	if (fd_i > 0)
		close(fd_i);

	if (fd_o > 0)
		close(fd_o);

	rt_free_align(buf_data);
	for (i = 0; i < 3; i++)
		rt_free_align(img_data[i]);

	rt_free(file_name);
	LOG_I("dit_test_entry end");
}

int disp_dit_test(rt_device_disp_ops_t *disp_ops_p)
{
	int ret;
	FILE *fp;
	char *file_name;
	char *dit_txt;
	char *file_total_name;
	dit_txt_parameter_t dit_txt_para;

	LOG_I("disp_dit_test start");

	ret = rt_device_control(disp_device, DISP_CMD_ENABLE_DIT, NULL);
	if (ret != DISP_OK) {
		LOG_E("enable dit err");
		return ret;
	}

	file_name = rt_zalloc(BUF_LEN);
	if (NULL == file_name)
		return DISP_ERROR;

	dit_txt = rt_zalloc(BUF_LEN);
	if (NULL == dit_txt)
		return DISP_ERROR;

	file_total_name = rt_zalloc(BUF_LEN);
	if (NULL == file_total_name)
		return DISP_ERROR;

	memcpy(dit_txt, DIT_DIR, DIT_DIR_LEN);

	strcat(dit_txt, "dit_test.txt");

	LOG_E("dit_txt[%s]\n", dit_txt);

	fp = fopen(dit_txt, "r");
	if (fp == NULL) {
		LOG_I("openfile [%s] error [%s]\n", dit_txt, strerror(errno));
		return DISP_ERROR;
	}

	while (!feof(fp) && fgets(file_name, BUF_LEN - 1, fp) != NULL) {
		rt_memset(file_total_name, 0x00, BUF_LEN);
		memcpy(file_total_name, DIT_DIR, DIT_DIR_LEN);
		strcat(file_total_name, file_name);
		LOG_E("file_name[%s]\n", file_name);
		rt_memset(&dit_txt_para, 0x00, sizeof(dit_txt_parameter_t));
		dit_txt_translate(file_total_name, &dit_txt_para);
		dit_test_entry(disp_ops_p, &dit_txt_para);
	}

	fclose(fp);

	rt_free(file_name);
	rt_free(dit_txt);
	rt_free(file_total_name);

	ret = rt_device_control(disp_device, DISP_CMD_DISABLE_DIT, NULL);
	if (ret != DISP_OK) {
		LOG_E("disable dit err");
		return ret;
	}

	LOG_I("disp_dit_test end");
	return DISP_OK;
}

