/*
 * csp_dpu_se.h - Dpu se module head file
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

#ifndef __CSP_DPU_SE_H__
#define __CSP_DPU_SE_H__

#include "csp_dpu_dc.h"

#define BASE_SE	0x01560000

#define CSC_SHIFT_BIT	12
#define SE_DEV_COUNT	2

#define SE_UP_ALIGN(x, a)   (((x) + ((a) - 1)) & (~((a) - 1)))
#define SE_DOWN_ALIGN(x, a) ((x)               & (~((a) - 1)))

enum tag_csc_conv {
	CSC_RGB2YUV = 0,
	CSC_YUV2RGB = 1,
	CSC_BYPASS = 2,
};

enum tag_csc_std {
	ITU601L = 0,
	ITU601F = 1,
	YCC_JPEG = 1,
	ITU709L = 2,
	ITU709F = 3,
	/* ITU2020NC, */
};

/* Rectangle specified by a start point and dimension */
/* All in pixel unit */
typedef struct tag_disp_rectangle {
	s32 x;   /* X coordinate of top-left point */
	s32 y;   /* Y coordinate of top-left point */
	u32 w;   /* Width of this rectangle */
	u32 h;   /* Height of this rectangle */
} disp_rectangle_t;

/* Framebuffer information */
typedef struct tag_disp_fb {
	u32 addr[3];                    /* Physical or bus address of fb */
	dc_pixel_format_t format;       /* Pixel format */
	dc_pixel_order_t pixel_order;   /* Pixel order */
	u32 w[3];                       /* Width of fb, in pixel unit */
	u32 h[3];                       /* Height of fb, in pixel unit */
	disp_rectangle_t clip;          /* The visual rectangle of fb */
	/* The horizontal align of fb, in bit unit */
#if 0
	u32 align;
#endif
	u32 lstr[3];
} disp_fb_t;

typedef enum tag_img_fmt {
	ARGB8888	= 0x00,/* 1plane */
	BGRA8888	= 0x01,
	ARGB4444	= 0x02,
	BGRA4444	= 0x03,
	ARGB1555	= 0x04,
	BGRA5551	= 0x05,
	RGB565		= 0x06,
	BGR565		= 0x07,
	UYVY		= 0x08,
	VYUY		= 0x09,
	YUYV		= 0x0a,
	YVYU		= 0x0b,
	YUV420SP	= 0x10,/* 2plane */
	YVU420SP	= 0x11,
	YUV422SP	= 0x12,
	YVU422SP	= 0x13,
	YUV420P		= 0x20,/* 3plane */
	YUV422P		= 0x21,
	YUV444P		= 0x22,
} se_img_fmt_t;

#define IS_YUV_FMT(infmt) \
		(((infmt & 0xFC) == UYVY) || \
		((infmt & 0xFC) == YUV420SP) || \
		((infmt & 0xFC) == YUV420P))

typedef enum tag_wb_fmt {
	WB_YUV420P,
	WB_YUV420SP,
	WB_YUV444P,
	WB_ARGB8888,
	WB_RGB565,
} se_wb_fmt_t;

/* Write back mode */
typedef enum tag_se_wb_mode {
	SE_WB_DISPLAY_MODE,   /* Write back and Display simultaneously */
	SE_WB_ONLY_MODE,      /* Write back only */
} se_wb_mode_t;

/* Data source of window */
typedef enum tag_se_input_sel {
	/* Image data comes from DRAM via inner-DMA */
	SE_INPUT_DRAM	= 0,
	/* Image data comes from external Display Controller */
	SE_INPUT_DC	= 1,
} se_input_sel_t;

/* Interrupt source of se */
typedef enum tag_se_irq_src {
	SE_IRQ_LINECNT		= 1 << 0, /* Line counter interrupt */
	SE_IRQ_WB_FINISH	= 1 << 4,   /* Write-back interrupt */
	SE_IRQ_WB_OVERFLOW	= 1 << 5,      /* Write-back interrupt */
	SE_IRQ_WB_TIMEOUT	= 1 << 6,     /* Write-back interrupt */
} se_irq_src_t;

/* Trig select of se linecounter */
typedef enum tag_se_trig_field {
	SE_TRIG_TOP	= 0x0,     /* trig top field */
	SE_TRIG_BOT	= 0x1,     /* trig bottom field */
	SE_TRIG_EVERY	= 0x2,   /* trig every field */
} se_trig_field_t;

/* Register update mode of se */
typedef enum tag_se_update_mode {
	SE_UPD_AUTO	= 0x0,   /* Auto update with dc_sync */
	SE_UPD_MAN	= 0x1,    /* Manual update mode */
} se_upd_mode_t;

/* Frame buffer used for SE input */
typedef struct tag_se_input_fb {
	s32 inw[3];
	s32 inh[3];
	s32 lstr[3];
	u32 addr[3];
	se_img_fmt_t infmt;
	disp_rectangle_t clip;
} se_input_fb_t;

/* SE common control */
s32 csp_se_init(u32 id);
s32 csp_se_exit(u32 id);
s32 csp_se_set_register_base(u32 id, void *addr, u32 size);
s32 csp_se_get_register_base(u32 id, unsigned long *addr);
s32 csp_se_enable(u32 id);
s32 csp_se_disable(u32 id);
s32 csp_se_stream_start(u32 id);
s32 csp_se_stream_stop(u32 id);
s32 csp_se_set_update_mode(u32 id, se_upd_mode_t update_mode);
s32 csp_se_update_dbr(u32 id);

/* SE format and mode config */
s32 csp_se_set_linebuffer_mode(u32 id, u32 lb_mode);
s32 csp_se_set_enable_rsmp(u32 id);
s32 csp_se_set_disable_rsmp(u32 id);
s32 csp_se_set_infmt(u32 id, se_img_fmt_t infmt);
s32 csp_se_set_input_sel(u32 id, se_input_sel_t sel);
s32 csp_se_set_online_mode(u32 id);
s32 csp_se_set_offline_mode(u32 id);
s32 csp_se_set_input_src_dram(u32 id);
s32 csp_se_set_input_src_dc(u32 id);
s32 csp_se_set_online_field_pol(u32 id, u32 pol);
s32 csp_se_set_offline_field_lv(u32 id, u32 field_lv);
s32 csp_se_set_output_progressive(u32 id);
s32 csp_se_set_output_interlaced(u32 id);

s32 csp_se_enable_premul(u32 id);
s32 csp_se_disable_premul(u32 id);
s32 csp_se_enable_alpha(u32 id);
s32 csp_se_disable_alpha(u32 id);
s32 csp_se_enable_csci(u32 id);
s32 csp_se_disable_csci(u32 id);
s32 csp_se_enable_csco(u32 id);
s32 csp_se_disable_csco(u32 id);
s32 csp_se_set_csci_coef(u32 id,
			 enum tag_csc_conv yuv2rgb,
			 enum tag_csc_std csc_sel);
s32 csp_se_set_csco_coef(u32 id,
			 enum tag_csc_conv yuv2rgb,
			 enum tag_csc_std csc_sel);

/* SE interrupt related */
s32 csp_se_set_linecounter_trig_num(u32 id, u32 num);
s32 csp_se_set_linecounter_trig_field(u32 id, se_trig_field_t field_sel);

s32 csp_se_enable_irq_src(u32 id, se_irq_src_t irq_trig);
s32 csp_se_disable_irq_src(u32 id, se_irq_src_t irq_trig);
s32 csp_se_get_irq_status(u32 id);
s32 csp_se_clear_irq_status(u32 id, se_irq_src_t irq_trig);

/* SE status */
s32 csp_se_get_field_status(u32 id);
s32 csp_se_get_frame_num(u32 id);
s32 csp_se_get_line_num(u32 id);
s32 csp_se_get_stream_status(u32 id);


/* SE in/output para */
s32 csp_se_set_insize(u32 id, u32 inw[2], u32 inh[2]);
s32 csp_se_set_inlstr(u32 id, u32 lstr[2]);
s32 csp_se_set_inbuf_addr(u32 id, u32 addr[3]);

s32 csp_se_set_rsmp_size(u32 id, u32 rsmpw, u32 rsmph);

s32 csp_se_set_out_crop_size(u32 id, u32 cropw, u32 croph);
s32 csp_se_set_out_crop_offset(u32 id, u32 offsetx, u32 offsety);

s32 csp_se_set_scaling_ratio_x(u32 id, u32 ratio[2]);
s32 csp_se_set_scaling_ratio_y(u32 id, u32 ratio[2]);

s32 csp_se_set_xoffset(u32 id, s32 offset[2]);
s32 csp_se_set_yoffset(u32 id, s32 offset[2]);

s32 csp_se_set_shift_ctl(u32 id, u32 shift_bitx[2], u32 shift_bity[2]);

s32 csp_se_set_scaling_xcoef0(u32 id, u32 chnum, u32 *coef);
s32 csp_se_set_scaling_xcoef1(u32 id, u32 chnum, u32 *coef);
s32 csp_se_set_scaling_ycoef(u32 id, u32 chnum, u32 *coef);

/* Write-back function */
s32 csp_se_set_wb_format(u32 id, se_wb_fmt_t outfmt);
s32 csp_se_start_wb(u32 id);
s32 csp_se_stop_wb(u32 id);
s32 csp_se_set_wb_size(u32 id, u32 wbw[2], u32 wbh[2]);
s32 csp_se_set_wb_linestride(u32 id, u32 lstr[3]);
s32 csp_se_set_wb_addr(u32 id, u32 addr[3]);
s32 csp_se_set_wb_timer(u32 id, s32 timeout_len);
s32 csp_se_get_wb_timer_status(u32 id);
s32 csp_se_check_wb_finish(u32 id);
s32 csp_se_check_wb_timeout(u32 id);
s32 csp_se_check_wb_overflow(u32 id);

/* Video post processing function */
s32 csp_se_gmtab_set_coef(u32 id, s32 tab_sel, u8 coef[256]);
s32 csp_se_gmtab_set_rgb_mode(u32 id);
s32 csp_se_gmtab_set_yuv_mode(u32 id);
s32 csp_se_gmtab_enable(u32 id, u32 tab_sel);
s32 csp_se_gmtab_disable(u32 id, u32 tab_sel);
s32 csp_se_hist_enable(u32 id);
s32 csp_se_hist_disable(u32 id);
s32 csp_se_hist_clear(u32 id);

s32 csp_se_peaking_enable(u32 id, s32 lvl);
s32 csp_se_peaking_disable(u32 id);
s32 csp_se_ltiv_enable(u32 id, s32 lvl);
s32 csp_se_ltiv_disable(u32 id);
s32 csp_se_lti_enable(u32 id, s32 lvl);
s32 csp_se_lti_disable(u32 id);
s32 csp_se_cti_enable(u32 id, s32 lvl);
s32 csp_se_cti_disable(u32 id);

s32 csp_se_peaking_set_coef(u32 id, s32 pktype, s32 ext_coef[9]);
s32 csp_se_lti_set_coef(u32 id, s32 ltitype, s32 ext_coef[9]);
s32 csp_se_cti_set_coef(u32 id, s32 ctitype, s32 ext_coef[9]);

#endif /* __CSP_DPU_SE_H__ */
