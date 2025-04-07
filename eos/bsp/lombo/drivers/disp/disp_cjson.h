/*
 * disp_cjson.h - Lombo disp cjson module head file
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
#ifndef __DISP_CJSON_H_
#define __DISP_CJSON_H_
#include <stdbool.h>
#include <soc_define.h>
#include <spinlock.h>
#include "csp_doss_tcon.h"

#define DISP_CJSON_NAME_MAX	64

#define DISP_JSON_SRC_FILE	"/etc/disp_config.json"
#define DISP_JSON_DEBUG_FILE	"/mnt/data/disp_config.json" /* for lcd tool debug */

#define DISP_GAMMA_SRC_FILE	"/etc/disp_gamma.bin"
#define DISP_GAMMA_DEBUG_FILE	"/mnt/data/disp_gamma.bin" /* for lcd tool debug */

typedef enum tag_disp_update_gamma_sta {
	DISP_UPDATING_GAMMA_IDLE		= 0, /* idle */
	DISP_UPDATING_GAMMA_PREPARE		= 1, /* prepare */
	DISP_UPDATING_GAMMA_DISABLED		= 2, /* disable gamma */
	DISP_UPDATING_GAMMA_TABLE		= 3, /* updating gamma table */
	DISP_UPDATING_GAMMA_ENABLED		= 4, /* enable gamma, finish update */
} disp_update_gamma_sta_t;

typedef struct tag_disp_cjson_cfg {
	bool ce_enable;					/* color enhance */
	int brightness;
	int saturation;
	int contrast;
	bool lti_enable;
	int lti_level;					/* lti level 0 to 15 */
	bool peak_enable;
	int peak_level;					/* peak level 0 to 15 */
	bool gamma_enable;
	char *gamma_tbl;
	/* char gamma_filepath[DISP_CJSON_NAME_MAX]; */	/* the gamma bin file path */
	tcon_host_t *tcon_host;
} disp_cjson_cfg_t;

typedef struct tag_disp_pipe_cfg {
	u32 dc_id;
	u32 dc_base;
	u32 tcon_id;
	u32 tcon_base;
	u32 dsi_id;
	u32 dsi_base;
} disp_pipe_cfg_t;

void copy(const char *src, const char *dst);

void disp_cjson_init(void);
void disp_cjson_exit(void);
disp_cjson_cfg_t *disp_cjson_get(void);
void disp_cjson_ce_init(u32 *enhance_enable, u32 *brightness,
	u32 *saturation, u32 *contrast);
void disp_cjson_lti_peak_init(unsigned int se_index);
int disp_cjson_gamma_get(tcon_host_t *tcon_host, u32 table_size);
bool disp_cjson_gamma_enable(void);
int disp_pipeline_get(void);
void disp_cjson_pipeline_dc_get(u32 *dc, u32 *dc_base);
void disp_cjson_pipeline_tcon_get(u32 *tcon, u32 *tcon_base);
void disp_cjson_pipeline_dsi_get(u32 *dsi, u32 *dsi_base);
#endif /* __DISP_CJSON_H_ */
