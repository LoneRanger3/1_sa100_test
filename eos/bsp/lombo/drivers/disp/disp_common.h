/*
 * lombo_common.h - Lombo disp  module common head file
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

#ifndef __DISP_COMMON_H_
#define __DISP_COMMON_H_
#include <stdbool.h>
#include <soc_define.h>
#include <div.h>
#include "disp_fourcc.h"
#include "test_image.h"

/* #define s32	signed int */

/* typedef unsigned char uint8_t; */
/* typedef unsigned long long  u64; */
/* typedef unsigned int u32; */
/* typedef unsigned short u16; */
/* typedef unsigned char u8; */
/* typedef unsigned short s16; */
/* typedef signed int s32; */
/* typedef signed int long long s64; */
typedef u32 dma_addr_t;
typedef unsigned int uintptr_t;

#define MS_PER_TICK			(1000 / RT_TICK_PER_SECOND)
#define ms_to_tick(ms)			(ms / MS_PER_TICK)
#define disp_delay(ms)			rt_thread_delay(ms / MS_PER_TICK)

#define DISP_EVENT_TIMEOUT		100		/* ms */
#define DISP_EVENT_MAX_WAIT_FRAME	3		/* how many frames we will wait */

#define LCD_EVENT_UPDATE_REG		(1u << 0)
/* win0~win3, bkl, hwc, total is 6 */
#define LCD_EVENT_UPDATE_ALL		0x3f
#define LCD_EVENT_PAGE_FLIP		(1u << 31)

#define VBANK_EVENT_UPDATE_DBR		(1u << 0)
/* win0~win3, bkl, hwc, total is 6 */
#define VBANK_EVENT_UPDATE_ALL		0x3f
#define VBANK_EVENT_PAGE_FLIP		(1u << 31)

#define DISP_F				1.0
typedef enum tag_rot_mode_type {
	LOMBO_DRM_TRANSFORM_COPY,
	LOMBO_DRM_TRANSFORM_ROT_90,
	LOMBO_DRM_TRANSFORM_ROT_180,
	LOMBO_DRM_TRANSFORM_ROT_270,
	LOMBO_DRM_TRANSFORM_FLIP_H,
	LOMBO_DRM_TRANSFORM_FLIP_H_ROT_90,
	LOMBO_DRM_TRANSFORM_FLIP_V,
	LOMBO_DRM_TRANSFORM_FLIP_V_ROT_90,
	LOMBO_DRM_TRANSFORM_MAX,
} rot_mode_type_t;

typedef enum tag_rot_way {
	HW_ROT,
	SW_ROT,
} rot_way_t;

typedef enum tag_dit_mode_type {
	DISP_DIT_MODE0, /* DIT_FIRST_FIELD DIT_TFF_SEQ */
	DISP_DIT_MODE1, /* DIT_FIRST_FIELD DIT_BFF_SEQ */
	DISP_DIT_MODE2, /* DIT_SECOND_FIELD DIT_TFF_SEQ */
	DISP_DIT_MODE3, /* DIT_SECOND_FIELD DIT_BFF_SEQ */
	DISP_DIT_MODE_MAX,
} dit_mode_type_t;

enum {
	DISP_OK,
	DISP_ERROR,
	DISP_BUSY,
};

typedef enum tag_disp_update_sta {
	DISP_UPDATING_IDLE		= 0, /* disp update is idle */
	DISP_UPDATING_REG		= 1, /* disp updating register */
	DISP_UPDATING_DBR		= 2, /* disp updatingdbr */
} disp_update_sta_t;

/*
 * Framebuffer information
 */
typedef struct tag_dpu_fb {
	u32 format;			/* Pixel format */
	unsigned int planes;		/* Planes of buffer */
	u8 bpp[3];			/* Byte per pixel */
	unsigned int addr[3];		/* Buffer addr */
	unsigned int width[3];		/* Image width */
	unsigned int height[3];		/* Image height */
	unsigned int linestride_bit[3];	/* Line stride of buffer */
} dpu_fb_t;

/*
 * Framebuffer information
 */
typedef struct tag_disp_rot_fb {
	u32 format;			/* Pixel format */
	unsigned int addr[3];		/* Buffer addr */
	unsigned int width[3];		/* Image width */
	unsigned int height[3];		/* Image height */
} disp_rot_fb_t;

typedef struct tag_disp_rot_cfg {
	rot_mode_type_t mode;
	disp_rot_fb_t infb;
	disp_rot_fb_t outfb;
	rot_way_t rot_way; /* use sw rot when true; otherwise use hw rot */
	/*
	* how many ms for hw rot time out, only valid when  rot_way is HW_ROT
	* default value is 200 ms
	* when time_out is less than 200 ms use 200ms
	* when time_out is more than 2000ms use 2000ms
	*/
	u32 time_out;
} disp_rot_cfg_t;

typedef struct tag_disp_dit_fb {
	unsigned int addr[3];		/* Buffer addr */
} disp_dit_fb_t;

typedef struct tag_disp_dit_cfg {
	u32 format;			/* Pixel format */
	dit_mode_type_t mode;		/* Dit mode */
	unsigned int width;		/* Image width */
	unsigned int height;		/* Image height */
	/* Input frame, wo will use 2 or 3 frames depend on mode */
	disp_dit_fb_t infb[3];
	disp_dit_fb_t outfb;		/* One output frame */
} disp_dit_cfg_t;

struct lombo_disp_clk {
	const char *axi_gate;
	const char *gate;
	const char *reset;
	const char *self;
	const char *parent;
	const char *osc;
};

extern void udelay(u32 us);

extern rt_mq_t disp_mq_p;
extern disp_update_sta_t disp_updating_sta;
extern struct rt_event vbank_event;
extern void disp_handle_vblank(void);

#endif /* _DISP_COMMON_H_ */
