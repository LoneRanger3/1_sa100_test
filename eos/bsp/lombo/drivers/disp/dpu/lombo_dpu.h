/*
 * lombo_dpu.h - Lombo dpu module head file
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

#ifndef _LOMBO_DPU_H_
#define _LOMBO_DPU_H_

#include <list.h>
#include "csp_dpu_se.h"
#include "dpu_clk.h"

/* Statistics func support */
#define TCON_VBI_STATISTICAL_DATA

/* for statistics vbi irq err curline, 2 frames period */
#define TCON_VBI_ERR_COUNT		120

#define WIN_NAME_MAX			(2 * RT_NAME_MAX)

#define MAX_SCREEN_WIDTH		1920
#define MAX_SCREEN_HEIGHT		1920

#define RGB_IS_PREMULTIPLIED		1
#define RGB_IS_NOT_PREMULTIPLIED	0

typedef enum dpu_dc_index {
	DPU_DC_INDEX0			= 0,
	DPU_DC_INDEX1			= 1
} dpu_dc_index_t;

typedef enum dpu_dc_pipe_index {
	DPU_DC_PIPE_INDEX0		= 0,
	DPU_DC_PIPE_INDEX1		= 1
} dpu_dc_pipe_index_t;

typedef enum dpu_dc_win_index {
	DPU_DC_WIN_INDEX0		= 0,
	DPU_DC_WIN_INDEX1		= 1,
	DPU_DC_WIN_INDEX2		= 2,
	DPU_DC_WIN_INDEX3		= 3,
} dpu_dc_win_index_t;

typedef enum dpu_dc_win_bpp {
	ONE_BYTES_PER_PIXEL		= 1,
	TWO_BYTES_PER_PIXEL		= 2,
	THREE_BYTES_PER_PIXEL		= 3,
	FOUR_BYTES_PER_PIXEL		= 4,
} dpu_dc_win_bpp_t;

typedef enum dpu_se_index {
	DPU_SE_INDEX0			= 0,
	DPU_SE_INDEX1			= 1
} dpu_se_index_t;

typedef enum dpu_dc_hwc_size {
	DPU_DC_HWC_SIZE_w32_h32		= 0,
	DPU_DC_HWC_SIZE_w64_h64		= 1
} dpu_dc_hwc_size_t;

typedef enum tag_dc_win_number {
	DC_WIN_0,
	DC_WIN_1,
	DC_WIN_2,
	DC_WIN_3,
	DC_BL_0,
	DC_BL_1,
	DC_BL_2,
	DC_BL_3,
	DC_BL_4,
	DC_BL_5,
	DC_BL_6,
	DC_BL_7,
	DC_BL_8,
	DC_BL_9,
	DC_BL_10,
	DC_BL_11,
	DC_BL_12,
	DC_BL_13,
	DC_BL_14,
	DC_BL_15,
	DC_HWC,
	DC_MAX_NR,
} dc_win_number_t;

typedef enum tag_dc_bl_index {
	DC_BL_IDX_0,
	DC_BL_IDX_1,
	DC_BL_IDX_2,
	DC_BL_IDX_3,
	DC_BL_IDX_4,
	DC_BL_IDX_5,
	DC_BL_IDX_6,
	DC_BL_IDX_7,
	DC_BL_IDX_8,
	DC_BL_IDX_9,
	DC_BL_IDX_10,
	DC_BL_IDX_11,
	DC_BL_IDX_12,
	DC_BL_IDX_13,
	DC_BL_IDX_14,
	DC_BL_IDX_15,
} dc_bl_index_t;

enum dc_version_id {
	DC_VER_0_1,
	DC_VER_0_2,
	DC_VER_0_3,
	DC_VER_0_4,
};

typedef enum tag_win_sta_index {
	WIN0_STA_INDEX,
	WIN1_STA_INDEX,
	WIN2_STA_INDEX,
	WIN3_STA_INDEX,
	BKL_STA_INDEX,
	HWC_STA_INDEX,
	WIN_STA_NUM,
} win_sta_index_t;

typedef enum dup_win_buf_sta {
	WIN_BUF_STA_IDLE		= 0, /* buf is idle */
	WIN_BUF_STA_PREPARE		= 1, /* buf is filled of data  */
	WIN_BUF_STA_REG			= 2, /* buf is updated to register */
	WIN_BUF_STA_DBR			= 3, /* buf is updated to dbr */
} dup_win_buf_sta_t;

typedef enum tag_dpu_dc_bsc {
	DPU_DC_BRIGHTNESS		= 0,
	DPU_DC_SATURATION		= 1,
	DPU_DC_CONTRAST			= 2,
	DPU_DC_BSC_MAX			= 3
} dpu_dc_bsc_t;

typedef enum tag_disp_power_sta {
	DISP_POWER_ON			= 0,
	DISP_POWER_SUSPEND		= 1,
} disp_power_sta_t;

typedef enum tag_disp_list_index {
	DISP_LIST_WIN,
	DISP_LIST_BKL,
	DISP_LIST_NUM
} disp_list_index_t;

typedef struct dc_win_data {
	dma_addr_t		dma_addr;
	dma_addr_t		chroma_dma_addr;
	dma_addr_t		chroma_x_dma_addr;
	u32			pixel_format;
	dc_pixel_order_t	pixel_order;
	unsigned int		bpp;
	unsigned int		crtc_x;
	unsigned int		crtc_y;
	unsigned int		crtc_width;
	unsigned int		crtc_height;
	unsigned int		fb_x;
	unsigned int		fb_y;
	unsigned int		fb_width;
	unsigned int		fb_height;
	unsigned int		src_width;
	unsigned int		src_height;
	unsigned int		mode_width;
	unsigned int		mode_height;
	unsigned int		line_size;	/* bytes unit*/
	unsigned int		scan_flags;

	char			cursor_update_type;

	bool			enabled;
	bool			resume;
	bool			update_flag;
} dc_win_data_t;

typedef struct disp_se {
	bool			is_requested;
	bool			is_online_mode;
	u8			se_idx;
	void			*wctl; /* disp_ctl_t  just for online mode */
} disp_se_t;

/* Win para information */
typedef struct tag_dc_win_para {
	bool			is_initial;

	bool			show_enable_update; /* use to update show_enable */
	bool			show_enable; /* show when true, hide when false */

	bool			alpha_mode_update; /* use to update alpha_mode */
	dc_alpha_mode_t		alpha_mode; /* alpha mode */

	bool			alpha_value_update; /* use to update alpha_value */
	u8			alpha_value; /* alpha value */

	bool			ck_enable_update; /* use to update ck_enable */
				/* enable ck when true, disable ck  when false */
	bool			ck_enable;
	bool			ck_update;/* use to update ck */
	dc_colorkey_t		ck; /* color key */
} dc_win_para_t;

typedef struct tag_dpu_dc_info {
	bool			is_initial;

	bool			bc_update; /* use to update bc */
	dc_color_t		bc; /* backcolor */

	bool			ce_update; /* use to update ce */
	dc_enhance_info_t	ce; /* color enhance */

	dc_win_para_t		win_para[DC_WIN_NUM]; /* parameters of windows */
	dc_win_para_t		bkl_para; /* parameters of blocklinker */
	dc_win_para_t		hwc_para; /* parameters of hwc */
} dpu_dc_info_t;

typedef struct dc_context {
	bool			dc_enable;
	bool			se_enable;
	bool			bkl_enable;
	/* this make sure bkl dcahce happen early than hw update */
	bool			bkl_dcahce;
	bool			hwc_enable;

	bool			rot_enable;
	bool			dit_enable;

	u32			rot_enable_cnt;
	u32			dit_enable_cnt;

	unsigned int		dc_index;

	unsigned int		max_win_num; /* the request win num */
	unsigned int		max_bkl_num;
	unsigned int		max_bkl_debug_num; /* for debug info */
	unsigned int		palette_table_size;

	unsigned int		se_update_coef_count[DC_SE_NUM];

	bool			interlace;
	bool			powered;
	bool			dc0_pre_enabled;
	bool			dc0_enabled;
	bool			dc1_pre_enabled;
	bool			dc1_enabled;

	bool			se0_pre_enabled;
	bool			se0_enabled;
	bool			se1_pre_enabled;
	bool			se1_enabled;

	char			cursor_update_type;
	unsigned int		int_en;

	struct disp_ctl		*dctl[DC_WIN_NUM];

	/* only value for win0-win3 */
	dc_window_src_t		win_src[DC_WIN_NUM];

	/* win0/win1/win2/win3 */
	/* struct dc_win_data	win_data[DC_WIN_NUM]; */

	/* bkl0~bkl15*/
	struct disp_ctl		*bkl_dctl[DC_BKL_NUM];
	struct disp_ctl		*bkl_debug_dctl[DC_BKL_NUM]; /* for debug info */
	/* struct dc_win_data	bkl_data[DC_BKL_NUM]; */
	struct dc_win_data	hwc_data;

	/* win0/win1/win2/win3/bkl/hwc */
	dup_win_buf_sta_t	win_sta[WIN_STA_NUM];
	dup_win_buf_sta_t	pageflip_sta; /* for page flip */

	disp_se_t		se_dev[DC_SE_NUM];
	disp_power_sta_t	power_sta;
	bool			page_fliping; /* true when is doing page flip */
	dpu_dc_info_t		dpu_dc_info[DC_NUM];
	enum dc_version_id	dc_ver;
} dc_context_t;

typedef struct disp_ctl {
	struct list_head node;
	int num;
	bool is_used;
	bool is_config;
	char name[WIN_NAME_MAX+1];
	u32 se_idx;
	dc_window_src_t win_src;
	dup_win_buf_sta_t win_sta;
	dc_win_data_t disp_win;
	dc_win_para_t win_para; /* parameters of windows */
} disp_ctl_t;

#undef ALIGN
#define ALIGN(w, a) (((w) + (a) - 1) / (a) * (a))

void dpu_dcache(int ops, void *addr, int size);
bool win_show_enable(u32 dc_idx, u32 win_idx);
bool is_yuv_format(u32 format);
bool is_422_format(u32 format);
u8 get_bpp_from_format(u32 format, u8 plane_cnt);
int dc_update_dbr(unsigned int dc_index);
bool dc_is_load_dbr_finish(unsigned int dc_index);
int dpu_resources_init(void);
int dpu_resources_uninit(void);
int dc_resources_init(u32 dc_index, dpu_dc_info_t *ddi);
int dc_resources_uninit(u32 dc_index);
int dc_set_backcolor(u32 dc_index, dc_color_t *bk_color);
int dc_enhancement_init(u32 dc_index, dc_enhance_info_t *info);
int dc_set_enhancement(u32 dc_index, dc_enhance_info_t *info);
int dc_windows_show(u32 dc_idx, u32 win_idx);
int dc_windows_hide(u32 dc_idx, u32 win_idx);
int dc_win_set_colorkey(u32 dc_idx, u32 win_idx, dc_colorkey_t *ck);
int dc_win_enable_colorkey(u32 dc_idx, u32 win_idx);
int dc_win_disable_colorkey(u32 dc_idx, u32 win_idx);
int dc_win_set_alpha_mode(u32 dc_idx, u32 win_idx, dc_alpha_mode_t *mode);
int dc_win_set_alpha_value(u32 dc_idx, u32 win_idx, u8 *value);
int dc_init(unsigned int dc_index, u32 screen_w, u32 screen_h);
int dc_uninit(unsigned int dc_index);
int dc_pipe_init(u32 dc_idx, u32 pipe);
int dc_pipe_uninit(u32 dc_idx, u32 pipe);
int dc_windows_init(u32 dc_idx, u32 win_idx);
int dc_windows_uninit(u32 dc_idx, u32 win_idx);
int dc_win_update(struct dc_context *dct, u32 win_index);

int dc_screenshot(u32 dc_idx, char *buf, u32 size);
int dc_get_screenshot_status(u32 dc_idx, u32 *status);

int dc_blocklinker_set_alpha_mode(u32 dc_idx, dc_alpha_mode_t *mode);
int dc_blocklinker_set_alpha_value(u32 dc_idx, u8 *value);
int dc_blocklinker_init(u32 dc_index);
int dc_blocklinker_uninit(u32 dc_index);
int dc_blocklinker_enable(u32 dc_index);
int dc_blocklinker_disable(u32 dc_index);
int dc_blocklinker_dcahce(struct dc_context *blc);
int dc_blocklinker_update(struct dc_context *blc);

int dc_hwc_show(u32 dc_idx);
int dc_hwc_hide(u32 dc_idx);
int dc_hwc_init(u32 dc_index, dpu_dc_hwc_size_t hwc_size_format, u32 hwc_x, u32 hwc_y);
int dc_hwc_uninit(u32 dc_index);
int dc_hwc_update(struct dc_context *dct);

int se_resources_init(u32 se_index);
int se_resources_uninit(u32 se_index);
int se_init(unsigned int se_index);
int se_uninit(unsigned int se_index);
void se_video_buffer(struct dc_context *ctx, int win, unsigned int index,
	bool is_offline, dc_win_data_t *se_data, u32 out_addr[3], u32 out_format);

int se_update_dbr(unsigned int se_index);

int rot_resources_init(void);
int rot_resources_uninit(void);
int rot_init(void);
int rot_uninit(void);
int rot_process(disp_rot_cfg_t *cfgs);

int dit_resources_init(void);
int dit_resources_uninit(void);
int dit_init(void);
int dit_uninit(void);
int dit_process(disp_dit_cfg_t *cfgs);

#endif
