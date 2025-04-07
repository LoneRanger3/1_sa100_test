/*
 * csp_dpu_dc.h - Dpu dc module head file
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

#ifndef __CSP_DPU_DC_H_
#define __CSP_DPU_DC_H_

#include "../disp_common.h"

#define DC_END_REG       0x9c00

#ifndef ARRARSIZE
#define ARRARSIZE(x) (sizeof(x) / sizeof(*x))
#endif

#define DC_TRUE 1
#define DC_FALSE 0

#define DC_UP_ALIGN(x, a)      (((x) + ((a) - 1)) & (~((a) - 1)))
#define DC_DOWN_ALIGN(x, a)    ((x)               & (~((a) - 1)))
#define DC_COLOR(a, r, g, b)   ((((a) & 0xff) << 24) | (((r) & 0xff) << 16) | \
				(((g) & 0xff) << 8) | ((b) & 0xff))

/* 256 color for 8bpp, and 4bytes(argb) for each color */
#define DC_LUT_MAX_SIZE       ((1 << 8) * sizeof(u32))
#define DC_IFB_MAX_SIZE       0x1c00
#define DC_HWC_INDEX_MAX_SIZE 0x400

#define DC_NUM			1
#define DC_WIN_NUM		4
#define DC_PIPE_NUM		2
#define DC_BKL_NUM		16
#define DC_SE_NUM		2

/* Priority: hwc > Block linker > Window3 > Window2 > Window1 > Window0 */
#define DC_WIN_NR		21

/* max  size supported */
#define DC_MAX_WIDTH		1920
#define DC_MAX_HEIGHT		1920

/* max size for hwc */
#define DC_HWC_MAX_WIDTH	64
#define DC_HWC_MAX_HEIGHT	64

/* Sram base */
#define DC_BLK_PALETTE_BASE       0x8000
#define DC_HWC_INDEX_BASE         0x8400
#define DC_HWC_PALETTE_BASE       0x8800
#define DC_WIN_PALETTE_BASE(w)    (0x8c00 + (w) * 0x400)
#define DC_WIN_GAMMA_BASE(w)      DC_WIN_PALETTE_BASE(w)
#define DC_WIN_IFB_BASE           0x8000
#define DC_CECOEF_NUM             12 /* number of color enhance coef register */
#define DC_ALPHA_MAX_VALUE        255
#define DC_FMT_BPP_MAX            32
#define DC_HWC_OFFSET_BITWIDTH    6

/*
 * Pixel format encodes in the following way:
 *
 * @index: pixel format index, will be used direct by register
 * @colot_bits: effective color (or index) bits per pixel of format
 * @alpha_bits: effective alpha bits per pixel of format
 * @has_lut: color lookup table present
 * @plane_num: plane number consist per pixel of format
 */
#define DC_PIXEL_FMT(index, color_bits, alpha_bits, has_lut, plane_num) \
	 ((((index)      & 0x7f))       |   \
	  (((color_bits) & 0x1f) <<  7) |   \
	  (((alpha_bits) & 0x0f) << 12) |   \
	  (((has_lut)    & 0x01) << 16) |   \
	  (((plane_num)  & 0x07) << 17))

#define DC_PIXEL_FMT_INDEX(fmt)        (((fmt))        & 0x7f) /* index for dc
 * @bpp: in bit unit
 */
#define DC_PIXEL_FMT_BPP(fmt)          ((((fmt) >>  7) & 0x1f) + \
							(((fmt) >> 12) & 0x0f))
#define DC_PIXEL_FMT_HAS_ALPHA(fmt)    ((((fmt) >> 12) & 0x0f) != 0)
#define DC_PIXEL_FMT_HAS_LUT(fmt)      ((((fmt) >> 16) & 0x01) != 0)
#define DC_PIXEL_FMT_PLANES(fmt)       (((fmt)  >> 17) & 0x07)
/* palettte size: in byte unit */
#define DC_PIXEL_FMT_PALETTE_SIZE(fmt) (DC_PIXEL_FMT_HAS_LUT(fmt) ? \
					((1 << (1 << DC_PIXEL_FMT_INDEX(fmt))) \
					* sizeof(u32)) : 0)

/*
 * @width: in pixel
 * @align: in bit unit
 * @return: in bit unit
 */
#define DC_PIXEL_FMT_LINESTRIDE(fmt, width, align)  \
			DC_UP_ALIGN((DC_PIXEL_FMT_BPP(fmt)) * (width), align)

typedef enum tag_dc_retval {
	DC_OK,            /* No errror occured */
	DC_ERROR,         /* A genernal or unkown error occured */
	DC_BUSY,          /* The resource or device is busy */
} dc_retval_t;

/* pixel format */
typedef enum tag_dc_pixel_format {
	/* 32bit, Alpha at the MSB, B at the LSB */
	DC_FMT_ARGB8888 = DC_PIXEL_FMT(0x00, 24, 8, 0, 1),
	/* 32ibt, Alpha at the MSB, R at the LSB */
	DC_FMT_ABGR8888 = DC_PIXEL_FMT(0x01, 24, 8, 0, 1),
	DC_FMT_BGRA8888 = DC_PIXEL_FMT(0x02, 24, 8, 0, 1),
	DC_FMT_RGBA8888 = DC_PIXEL_FMT(0x03, 24, 8, 0, 1),
	DC_FMT_ARGB4444 = DC_PIXEL_FMT(0x04, 12, 4, 0, 1),
	DC_FMT_ABGR4444 = DC_PIXEL_FMT(0x05, 12, 4, 0, 1),
	DC_FMT_BGRA4444 = DC_PIXEL_FMT(0x06, 12, 4, 0, 1),
	DC_FMT_RGBA4444 = DC_PIXEL_FMT(0x07, 12, 4, 0, 1),
	DC_FMT_ARGB1555 = DC_PIXEL_FMT(0x08, 15, 1, 0, 1),
	DC_FMT_ABGR1555 = DC_PIXEL_FMT(0x09, 15, 1, 0, 1),
	DC_FMT_BGRA5551 = DC_PIXEL_FMT(0x0A, 15, 1, 0, 1),
	DC_FMT_RGBA5551 = DC_PIXEL_FMT(0x0B, 15, 1, 0, 1),
	DC_FMT_RGB565   = DC_PIXEL_FMT(0x0C, 16, 0, 0, 1),
	DC_FMT_BGR565   = DC_PIXEL_FMT(0x0D, 16, 0, 0, 1),
	/* The same as BGR565 */
	DC_FMT_RGB565_R = DC_PIXEL_FMT(0x0E, 16, 0, 0, 1),
	/* The same as RGB565 */
	DC_FMT_BGR565_R = DC_PIXEL_FMT(0x0F, 16, 0, 0, 1),
	/* 24bit, R at the MSB, B at the LSB */
	DC_FMT_RGB888   = DC_PIXEL_FMT(0x10, 24, 0, 0, 1),
	DC_FMT_BGR888   = DC_PIXEL_FMT(0x11, 24, 0, 0, 1),
	/* The same as BGR888 */
	DC_FMT_RGB888_R = DC_PIXEL_FMT(0x12, 24, 0, 0, 1),
	/* The same as RGB888 */
	DC_FMT_BGR888_R = DC_PIXEL_FMT(0x13, 24, 0, 0, 1),
	DC_FMT_1BPP     = DC_PIXEL_FMT(0x00,  1, 0, 1, 1),
	DC_FMT_2BPP     = DC_PIXEL_FMT(0x01,  2, 0, 1, 1),
	DC_FMT_4BPP     = DC_PIXEL_FMT(0x02,  4, 0, 1, 1),
	DC_FMT_8BPP     = DC_PIXEL_FMT(0x03,  8, 0, 1, 1),
	DC_FMT_YUV444P  = DC_PIXEL_FMT(0xFF, 24, 0, 0, 3),
} dc_pixel_format_t;

/*
 * Pixel order
 * @PIXEL_REVERT_IN_BYTE: Valid in 1/2/4/8bpp format
 *     for example: 8bpp: The same in normal and revert mode
 *                  4bpp: P1 P0(normal, P1 in the MSB),
			  P0 P1(revert, P0 in the MSB)
 *                  2bpp: P3 P2 P1 P0(normal),  P0 P1 P2 P3(revert)
 *                  1bpp: P7 P6 P5 P4 P3 P2 P1 P0(normal),
			  P0 P1 P2 P3 P4 P5 P6 P7(revert)
 *                  and the rule is the same in each byte.
 * @BYTE_REVERT_IN_WORD:  Valid in 1/2/4/8bpp format
 *     for example: 8bpp: P3 P2 P1 P0(normal, P3 in the MSB0),
			  P0 P1 P2 P3(revert, P0 in the MSB)
 *                  4bpp: P7 P6 P5 P4 P3 P2 P1 P0(normal, P7 in the MSB),
			  P1 P0 P3 P2 P5 P4 P7 P6(revert, P1 in the MSB)
 *                  2bpp: P15 P14 P13 P12 P11 P10 P9  P8 P7
			  P6  P5 P4 P3  P2  P1  P0(normal, P15 in the MSB)
 *                        P3  P2  P1  P0  P7  P6  P5  P4
			  P11 P10 P9 P8 P15 P14 P13 P12(revert, P3 in the MSB)
 *                  1bpp: P31 P30 P29 P28 P27 P26 P25 P24 P23 P22 P21 P20
			  P19 P18 P17 P16 P15 P14 P13 P12 P11 P10 P9  P8
			  P7  P6  P5  P4  P3  P2  P1  P0 (normal)
 *                        P7  P6  P5  P4  P3  P2  P1  P0  P15 P14 P13 P12
			  P11 P10 P9  P8  P23 P22 P21 P20 P19 P18 P17 P16
			  P31 P30 P29 P28 P27 P26 P25 P24(revert)
 * @PIXEL_REVERT_IN_WORD: Valid in 16bit pixel format
 *  for example: RGB565: P1 P0(normal, P1 in the MSB),
 *			 P0 P1(revert, P0 in the MSB)
 *
 * PIXEL_REVERT_IN_BYTE and BYTE_REVERT_IN_WORD can be "OR" together,
 *			 like PIXEL_REVERT_IN_BYTE | BYTE_REVERT_IN_WROD.
 */
typedef enum tag_dc_pixel_order {
	DC_PO_NORMAL                 = 0, /* defaault */
	DC_PO_PIXEL_REVERT_IN_BYTE   = 1, /* Valid in 1/2/4/8bpp format */
	DC_PO_BYTE_REVERT_IN_WORD    = 2, /* Valid in 1/2/4/8bpp format */
	DC_PO_PIXEL_REVERT_IN_WORD   = 2, /* Valid in 16bit pixel format */
	DC_PO_MAX                    = 3,
} dc_pixel_order_t;

/* Color data range */
typedef enum tag_dc_data_range {
	/* Full range, 0~255 when in 8bit; 0~1024 when in 10bit */
	DC_DATA_FULL_RANGE,
	/* Limit range, 16~235(240) when in 8bit; 64~940(960) when in 10bit */
	DC_DATA_LIMIT_RANGE,
	DC_DATA_RANGE_MAX,
} dc_data_range_t;

/* Color space */
typedef enum tag_dc_cs {
	DC_CS_BT601, /* ITU BT.601 */
	DC_CS_BT709, /* ITU BT.709 */
	DC_CS_BT2020,/* ITU BT.2020 */
	DC_CS_MAX,
} dc_cs_t;

/* Color key match rule */
typedef enum tag_dc_colorkey_rule {
	/* Always match */
	DC_COLORKEY_ALWAYS_MATCH = 0,
	/* Matched when (min <= color <= max) */
	DC_COLORKEY_INSIDE_RANGE_MATCH = 1,
	/* Matched when (color < min or color > max) */
	DC_COLORKEY_OUTSIDE_RANGE_MATCH = 2,
	DC_COLORKEY_RULE_MAX,
} dc_colorkey_rule_t;

/*
 * blend rule encodes in the following way:
 * equation: cr=c0*cs+c1*cd; ar=c2*as+c3*ad;
 * cs: source color; as: source alpha;
 * cd: dest color;   ad: dest alpha
 * cr: result color; ar: result alpha
 *
 * @c0/1/2/3: blending coefficient
 * @c0p     : blending coefficient 0
 *            when source color carrys premultiplied alpha
 * @s       : 1: support, 0: unsupport when non-premultiplied
 */
#define DC_BLEND(c0, c0p, c1, c2, c3, s) \
	 ((((c0)  & 0xf))       |   \
	  (((c0p) & 0xf) <<  4) |   \
	  (((c1)  & 0xf) <<  8) |   \
	  (((c2)  & 0xf) << 12) |   \
	  (((c3)  & 0xf) << 16) |   \
	  (((s)   & 0x1) << 20))

#define DC_BLEND_C0(bld, p)      (((bld) >>  (4 * (p)))   & 0xf)
#define DC_BLEND_C1(bld)         (((bld) >>  8)           & 0xf)
#define DC_BLEND_C2(bld)         (((bld) >> 12)           & 0xf)
#define DC_BLEND_C3(bld)         (((bld) >> 16)           & 0xf)
#define DC_BLEND_SUPPORT(bld, p) ((p) ? 1 : ((bld) >> 20) & 0x1)

/* Blending rules(porter-duff rules) */
typedef enum tag_dc_blend_rule {
	/* Default rule(src-over) */
	DC_BLEND_DEFAULT  = 0,
	/* None of the item are used */
	DC_BLEND_CLEAR    = DC_BLEND(0, 0, 0, 0, 0, 1),
	/* Only the terms that contribute source color are used */
	DC_BLEND_SRC      = DC_BLEND(2, 1, 0, 1, 0, 1),
	/* Only the terms that contribute destination color are used */
	DC_BLEND_DST      = DC_BLEND(0, 0, 1, 0, 1, 1),
	/* The source color is placed over the destination color */
	DC_BLEND_SRC_OVER = DC_BLEND(2, 1, 3, 5, 1, 1),
	/* The destination color is placed over the source color */
	DC_BLEND_DST_OVER = DC_BLEND(5, 5, 1, 5, 1, 0),
	/* The source that overlaps the destination, replaces the destination */
	DC_BLEND_SRC_IN   = DC_BLEND(4, 4, 0, 4, 0, 0),
	/* The destination that overlaps the source, replaces the source */
	DC_BLEND_DST_IN   = DC_BLEND(0, 0, 2, 4, 0, 1),
	/* The source that does not overlap
	 * the destination replaces the destination
	 */
	DC_BLEND_SRC_OUT  = DC_BLEND(5, 5, 0, 5, 0, 0),
	/* The destination that does not overlap
	 * the source replaces the source
	 */
	DC_BLEND_DST_OUT  = DC_BLEND(0, 0, 3, 0, 3, 1),
	/* The source that overlaps the destination is
	 * composited with the destination
	 */
	DC_BLEND_SRC_ATOP = DC_BLEND(4, 4, 3, 0, 1, 0),
	/* The destination that overlaps the source is composited
	 * with the source and replaces the destination
	 */
	DC_BLEND_DST_ATOP = DC_BLEND(5, 5, 2, 1, 0, 0),
	/* The non-overlapping regions of source and destination are combined */
	DC_BLEND_XOR      = DC_BLEND(5, 5, 3, 5, 3, 0),
} dc_blend_rule_t;

/* Alpha mode */
typedef enum tag_dc_alpha_mode {
	/* Using the per-pixel alpha */
	DC_PIXEL_ALPHA,
	/* Using the per-plane alpha */
	DC_PLANE_ALPHA,
	/* Using the per-plane and per-pixel alpha together */
	DC_PLANE_PIXEL_ALPHA,
	DC_ALPHA_MODE_MAX,
} dc_alpha_mode_t;

/* Write back mode */
typedef enum tag_dc_wb_mode {
	DC_WB_DISPLAY_MODE,   /* Write back and Dispaly simultaneously */
	DC_WB_ONLY_MODE,      /* Write back only */
	DC_WB_MODE_MAX,
} dc_wb_mode_t;

/* Data source of window */
typedef enum tag_dc_window_src {
	/* Image data comes from DRAM via inner-DMA */
	DC_WINDOW_SRC_IDMA,
	/* Image data comes from external SCALING ENGINE0 */
	DC_WINDOW_SRC_SCALING0,
	/* Image data comes from external SCALING ENGINE1 */
	DC_WINDOW_SRC_SCALING1,
	DC_WINDOW_SRC_MAX,
} dc_window_src_t;

/* Work mode of window */
typedef enum tag_dc_window_work_mode {
	/* Normal mode, can support normal rgb format input */
	DC_WINDOW_NORMAL_MODE,
	/* Paltte mode, can support 1/2/4/8bpp width
	 * max-256-color palette table
	 */
	DC_WINDOW_PALETTE_MODE,
	/* Gammma mode, enable gamma correction
	 * according to specified gamma table
	 */
	DC_WINDOW_GAMMA_MODE,
	/* Internal frame buffer mode, image index and lut will be
	 * stored in the internal SRAM, can save power consumption
	 */
	DC_WINDOW_INTERNAL_FB_MODE,
	DC_WINDOW_WORK_MODE_MAX,
} dc_window_work_mode_t;

/* Color key target rule */
typedef enum tag_dc_colorkey_target {
	/* Color key matching rule will target this window */
	DC_COLORKEY_TARGET_THIS,
	/* Color key matching rule will target the background window */
	DC_COLORKEY_TARGET_OTHER,
	DC_COLORKEY_TARGET_MAX,
} dc_colorkey_target_t;

/* Pixel color */
typedef struct tag_dc_color {
	u8 b;  /* blue component */
	u8 g;  /* green component */
	u8 r;  /* red component */
	u8 a;  /* alpha component */
} dc_color_t;

/* Enhance information */
typedef struct tag_dc_enhance_info {
	dc_pixel_format_t fmt; /* Pixel format */
	dc_cs_t cs;            /* Color space */
	dc_data_range_t range; /* Data range */
	u32 enhance_enable;    /* Indicate if the enhance function enable */
	u32 brightness; /* B(rightness)/s/c/h Valid when enhance_enable is 1 */
	u32 saturation;
	u32 contrast;
	u32 hue;
} dc_enhance_info_t;

/*
 * Rectangle specified by a start point and dimension
 * All in pixel unit
 */
typedef struct tag_dc_rectangle {
	s32 x;   /* X coordinate of top-left point */
	s32 y;   /* Y coordinate of top-left point */
	u32 w;   /* Width of this rectangle */
	u32 h;   /* Height of this rectangle */
} dc_rectangle_t;

/* Framebuffer information */
typedef struct tag_dc_fb {
	unsigned long long addr;        /* Physical or bus address of fb */
	dc_pixel_format_t format;       /* Pixel format */
	dc_pixel_order_t pixel_order;   /* Pixel order */
	dc_rectangle_t clip;            /* The visual rectangle of fb */
	u32 linestride_bit;             /* line stride of fb, in bit unit */
} dc_fb_t;

/* Color key information */
typedef struct tag_dc_colorkey {
	dc_colorkey_target_t target;
	/* The minimal color value for color key */
	dc_color_t min;
	/* The maximum color value for color key */
	dc_color_t max;
	/* Color key matching rule for r/g/b component */
	dc_colorkey_rule_t red_rule;
	dc_colorkey_rule_t green_rule;
	dc_colorkey_rule_t blue_rule;
} dc_colorkey_t;

/* 4x4 Matrix */
typedef struct tag_dc_matrix {
	s32 m00, m01, m02, m03;
	s32 m10, m11, m12, m13;
	s32 m20, m21, m22, m23;
	s32 m30, m31, m32, m33;
} dc_matrix;

/* global control interface */
s32 csp_dc_set_register_base(u32 dc_index, void *addr, u32 size);
s32 csp_dc_get_register_base(u32 dc_index, unsigned long *addr);
s32 csp_dc_init(u32 dc_index);
s32 csp_dc_exit(u32 dc_index);
s32 csp_dc_enable(u32 dc_index);
s32 csp_dc_disable(u32 dc_index);
s32 csp_dc_set_output_mode(u32 dc_index, u32 outmode, u32 field_pol);
s32 csp_dc_set_backcolor(u32 dc_index, const dc_color_t *color);
s32 csp_dc_get_backcolor(u32 dc_index, dc_color_t *color);
s32 csp_dc_enable_pipe(u32 dc_index, u32 pipe);
s32 csp_dc_disable_pipe(u32 dc_index, u32 pipe);
s32 csp_dc_set_screen_size(u32 dc_index, u32 width, u32 height);
s32 csp_dc_get_screen_size(u32 dc_index, u32 *width, u32 *height);
s32 csp_dc_load_dbr(u32 dc_index);
bool csp_dc_is_load_dbr_finish(u32 dc_index);
s32 csp_dc_autoload_dbr(u32 dc_index);
s32 csp_dc_unload_dbr(u32 dc_index);
s32 csp_dc_set_enhancement(u32 dc_index, const dc_enhance_info_t *info);

/* window control interface */
s32 csp_dc_window_show(u32 dc_index, u32 win_index);
s32 csp_dc_window_hide(u32 dc_index, u32 win_index);
s32 csp_dc_window_set_alpha_mode(u32 dc_index,
				 u32 win_index,
				 dc_alpha_mode_t mode);
s32 csp_dc_window_get_alpha_mode(u32 dc_index,
				 u32 win_index,
				 dc_alpha_mode_t *mode);
s32 csp_dc_window_set_alpha_value(u32 dc_index, u32 win_index, u32 value);
s32 csp_dc_window_get_alpha_value(u32 dc_index, u32 win_index, u32 *value);
s32 csp_dc_window_set_dest_rectangle(u32 dc_index,
				     u32 win_index,
				     const dc_rectangle_t *rect);
s32 csp_dc_window_get_dest_rectangle(u32 dc_index,
				     u32 win_index,
				     dc_rectangle_t *rect);
s32 csp_dc_window_set_buffer(u32 dc_index, u32 win_index, const dc_fb_t *fb);
s32 csp_dc_window_get_buffer(u32 dc_index, u32 win_index, dc_fb_t *fb);
s32 csp_dc_window_set_data_source(u32 dc_index,
				  u32 win_index,
				  dc_window_src_t src);
s32 csp_dc_window_get_data_source(u32 dc_index, u32 win_index, u32 *src);
s32 csp_dc_window_set_work_mode(u32 dc_index,
				u32 win_index,
				dc_window_work_mode_t mode);
s32 csp_dc_window_get_work_mode(u32 dc_index,
				u32 win_index,
				dc_window_work_mode_t *mode);
s32 csp_dc_window_enable_premultiply(u32 dc_index, u32 win_index);
s32 csp_dc_window_disable_premultiply(u32 dc_index, u32 window_index);
s32 csp_dc_window_set_colorkey(u32 dc_index,
			       u32 win_index,
			       const dc_colorkey_t *ck);
s32 csp_dc_window_get_colorkey(u32 dc_index, u32 win_index, dc_colorkey_t *ck);
s32 csp_dc_window_enable_colorkey(u32 dc_index, u32 win_index);
s32 csp_dc_window_disable_colorkey(u32 dc_index, u32 win_index);
s32 csp_dc_window_set_blend_rule(u32 dc_index,
				 u32 win_index,
				 u32 premultiplied,
				 dc_blend_rule_t rule);
s32 csp_dc_window_update_palette_table(u32 dc_index,
				       u32 win_index,
				       const u8 *palette,
				       u32 bytes,
				       u32 offset);
s32 csp_dc_window_get_palette_table(u32 dc_index,
				    u32 win_index,
				    u8 *palette,
				    u32 bytes,
				    u32 offset);
s32 csp_dc_window_update_gamma_table(u32 dc_index,
				     u32 win_index,
				     const u8 *gamma,
				     u32 bytes,
				     u32 offset);
s32 csp_dc_window_get_gamma_table(u32 dc_index,
				  u32 win_index,
				  u8 *gamma,
				  u32 bytes,
				  u32 offset);
s32 csp_dc_window_update_internal_fb(u32 dc_index,
				     u32 win_index,
				     const u8 *buf,
				     u32 bytes,
				     u32 offset);
s32 csp_dc_window_get_internal_fb(u32 dc_index,
				  u32 win_index,
				  u8 *buf,
				  u32 bytes,
				  u32 offset);

/* hardware cursor control interface */
s32 csp_dc_hwc_set_format(u32 dc_index, dc_pixel_format_t format);
s32 csp_dc_hwc_get_format(u32 dc_index, dc_pixel_format_t *format);
s32 csp_dc_hwc_update_palette_table(u32 dc_index,
				     const u8 *palette,
				     u32 bytes,
				     u32 offset);
s32 csp_dc_hwc_get_palette_table(u32 dc_index,
				 u8 *palette,
				 u32 bytes,
				 u32 offset);
s32 csp_dc_hwc_update_fb(u32 dc_index, const u8 *buf, u32 bytes, u32 offset);
s32 csp_dc_hwc_get_fb(u32 dc_index, u8 *buf, u32 bytes, u32 offset);
s32 csp_dc_hwc_show(u32 dc_index);
s32 csp_dc_hwc_hide(u32 dc_index);
s32 csp_dc_hwc_set_dest_coordinate(u32 dc_index, s32 x, s32 y);
s32 csp_dc_hwc_get_dest_coordinate(u32 dc_index, s32 *x, s32 *y);
s32 csp_dc_hwc_set_size(u32 dc_index, u32 width, u32 height);
s32 csp_dc_hwc_get_size(u32 dc_index, u32 *width, u32 *height);
s32 csp_dc_hwc_set_offset(u32 dc_index, u32 x, u32 y);
s32 csp_dc_hwc_get_offset(u32 dc_index, u32 *x, u32 *y);

/* block linker control interface */
s32 csp_dc_blocklinker_set_format(u32 dc_index, dc_pixel_format_t format);
s32 csp_dc_blocklinker_get_format(u32 dc_index, dc_pixel_format_t *format);
s32 csp_dc_blocklinker_update_palette_table(u32 dc_index,
					    const u8 *palette,
					    u32 bytes,
					    u32 offset);
s32 csp_dc_blocklinker_get_palette_table(u32 dc_index,
					 u8 *palette,
					 u32 bytes,
					 u32 offset);
s32 csp_dc_blocklinker_set_alpha_mode(u32 dc_index, dc_alpha_mode_t mode);
s32 csp_dc_blocklinker_get_alpha_mode(u32 dc_index, dc_alpha_mode_t *mode);
s32 csp_dc_blocklinker_set_alpha_value(u32 dc_index, u32 value);
s32 csp_dc_blocklinker_get_alpha_value(u32 dc_index, u32 *value);
s32 csp_dc_blocklinker_set_blend_rule(u32 dc_index,
				      u32 premultiplied,
				      dc_blend_rule_t rule);
s32 csp_dc_blocklinker_enable(u32 dc_index);
s32 csp_dc_blocklinker_disable(u32 dc_index);
s32 csp_dc_blocklinker_set_dest_rectangle(u32 dc_index,
					  u32 bl_index,
					  const dc_rectangle_t *rect);
s32 csp_dc_blocklinker_get_dest_rectangle(u32 dc_index,
					  u32 bl_index,
					  dc_rectangle_t *rect);
s32 csp_dc_blocklinker_set_buffer(u32 dc_index,
				  u32 bl_index,
				  const dc_fb_t *fb);
s32 csp_dc_blocklinker_get_buffer(u32 dc_index, u32 bl_index, dc_fb_t *fb);
s32 csp_dc_blocklinker_set_next(u32 dc_index, u32 bl_index, u32 bl_next);
s32 csp_dc_blocklinker_get_next(u32 dc_index, u32 bl_index, u32 *bl_next);

/* write back control interface */
s32 csp_dc_wb_set_mode(u32 dc_index, dc_wb_mode_t mode);
s32 csp_dc_wb_get_mode(u32 dc_index, dc_wb_mode_t *mode);
s32 csp_dc_wb_set_buffer(u32 dc_index, const dc_fb_t *fb);
s32 csp_dc_wb_get_buffer(u32 dc_index, dc_fb_t *fb);
s32 csp_dc_wb_start(u32 dc_index);
s32 csp_dc_wb_get_status(u32 dc_index, u32 *status);

#endif /* __CSP_DPU_DC_H__ */
