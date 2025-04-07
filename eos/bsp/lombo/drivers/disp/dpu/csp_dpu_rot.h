/*
 * csp_dpu_rot.h - Dpu rot module head file
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

#ifndef __CSP_DPU_ROT_H__
#define __CSP_DPU_ROT_H__

/* for include dc.h in csp.h*/
/* #include <csp.h> */
#include "../disp_common.h"
#include "csp_dpu_dc.h"

#define ROT_REG_STEP 0x10

#define ROT_IRQ_FINISH 31
#define ROT_IRQ_DSTDECERR 10
#define ROT_IRQ_SRCDECERR 8

typedef enum tag_rot_mode {
	ROT_MODE_0, /* rot 0 degree before none flip */
	ROT_MODE_1, /* rot 90 degree before none flip */
	ROT_MODE_2, /* rot 180 degree before none flip */
	ROT_MODE_3, /* rot 270 degree before none flip */
	ROT_MODE_4, /* hflip before rot 0 degree
		     * vflip before rot 180 degree
		     * rot 0 degree before hflip
		     * rot 180 degree before vflip
		     */
	ROT_MODE_5, /* hflip before rot 90 degree
		     * vflip before rot 270 degree
		     * rot 270 degree before hflip
		     * rot 90 degree before vflip
		     */
	ROT_MODE_6, /* hflip before rot 180 degree
		     * vflip before rot 0 degree
		     * rot 180 degree before hflip
		     * rot 0 degree before vflip
		     */
	ROT_MODE_7, /* hflip before rot 270 degree
		     * vflip before rot 90 degree
		     * rot 90 degree before hflip
		     * rot 270 degree before vflip
		     */
	ROT_MODE_MAX,
} rot_mode_t;

/**
 * rot_cfg_ch_t - rot configs for each channel
 * @en: this channel need to be processed
 * @bytespp: bytes per pixel, regardless of sub-sample,
 *        for example: the bytespp of uv component is 2 in semi-planar format,
 *        no matter whether it's yuv444,yuv422 or yuv 420.
 * @in_w: input buffer width, in pixels,
 *        need to take sub-sample into account,
 *        for example, the width of uv component is
 *        1/2 width of y in yuv422sp format
 * @in_h: input buffer height, in pixels,
 *        need to take sub-sample into account,
 *        for example, the height of uv component is
 *        1/2 height of y in yuv420sp format
 * @in_addr: input buffer address, in bytes
 * @in_lnstd: input buffer line stride, in bytes
 * @out_w: output buffer width, in pixels,
 *        need to take sub-sample into account,
 *        for example, the width of uv component is
 *        1/2 width of y in yuv422sp format
 * @out_h: output buffer height, in pixels,
 *        need to take sub-sample into account,
 *        for example, the height of uv component is
 *        1/2 height of y in yuv420sp format
 * @out_addr: output buffer address, in bytes
 * @out_lnstd: output buffer line stride, in bytes
 */
typedef struct tag_rot_cfg_ch {
	unsigned int en;
	unsigned int bytespp;
	unsigned int in_w;
	unsigned int in_h;
	unsigned int in_addr;
	unsigned int in_lnstd;
	unsigned int out_w;
	unsigned int out_h;
	unsigned int out_addr;
	unsigned int out_lnstd;
} rot_cfg_ch_t;

typedef struct tag_rot_cfg {
	dpu_fb_t infb;
	dpu_fb_t outfb;
} rot_cfg_t;

#define ROT_DMA_TRTYPE_CONTINUE 0
#define ROT_DMA_TRTYPE_SHADOW   2
/**
 * rot_cfg_ch_t - rot configs for each channel
 * @burst_size: Burst size in bytes, the size of each transfer
 * @burst_len: Max burst length, the exact number of transfers in a burst
 * @burst_len_valid: Indicate whether burst_len is valid
 * @osrlmt: Outstanding request number limit
 * @transfer_type: transfer type, use ROT_DMA_TRTYPE\_ defines
 */
typedef struct tag_rot_dma_cfgs {
	unsigned int burst_size;
	unsigned int burst_len;
	unsigned int burst_len_valid;
	unsigned int osrlmt;
	unsigned int transfer_type;
} rot_dma_cfgs_t;

/**
 * tag_rot_status - rot status
 * @status: 0: normal; other: something error
 * @indma_busy: indicate whether input dma is busy transfering
 * @indma_done: indicate whether input dma transfer done
 * @indma_error: indicate whether input dma is error
 * @indma_shadow_valid:
 *    indicate whether input dma' shadow parameter is valid
 * @indma_shadow_last:
 *    indicate whether output dma' shadow parameter is the last one
 * @outdma_busy: indicate whether output dma is busy transfering
 * @outdma_done: indicate whether output dma transfer done
 * @outdma_error: indicate whether output dma is error
 * @outdma_shadow_valid:
 *    indicate whether output dma' shadow parameter is valid
 * @outdma_shadow_last:
 *    indicate whether output dma' shadow parameter is the last one
 */
typedef struct tag_rot_status {
	unsigned int status;
	unsigned int indma_busy;
	unsigned int indma_done;
	unsigned int indma_error;
	unsigned int indma_shadow_valid;
	unsigned int indma_shadow_last;
	unsigned int outdma_busy;
	unsigned int outdma_done;
	unsigned int outdma_error;
	unsigned int outdma_shadow_valid;
	unsigned int outdma_shadow_last;
} rot_status_t;

/* ROT Buffer Size Register */
typedef union {
	unsigned int val;
	struct {
	unsigned int w:13;               /* buffer width in pixels.
				 * Actual buffer width is W + 1 */
	unsigned int rsvd0:3;            /* - */
	unsigned int h:13;               /* buffer height in pixels
				 * Actual buffer height is H + 1 */
	unsigned int rsvd1:3;            /* - */
	} bits;
} reg_rot_size_t;

/* ROT Buffer Address Register */
typedef union {
	unsigned int val;
	struct {
	unsigned int add:32;     /* buffer address in bytes. */
	} bits;
} reg_rot_add_t;

/* ROT Buffer Line Stride Register */
typedef union {
	unsigned int val;
	struct {
	unsigned int std:32;     /* buffer line stride in bytes. */
	} bits;
} reg_rot_inlnstd_t;

int csp_rot_set_register_base(unsigned long addr, unsigned int size);
int csp_rot_get_register_base(unsigned long *addr);
int csp_rot_set_delay_func(void (*udelay)(unsigned long));
int csp_rot_get_ip_ver(unsigned int *major, unsigned int *minor);
int csp_rot_init(void);
int csp_rot_exit(void);
int csp_rot_reset(void);
int csp_rot_start(void);
int csp_rot_stop(void);
int csp_rot_enable_irq(unsigned int irqnr);
int csp_rot_disable_irq(unsigned int irqnr);
bool csp_rot_query_irq(unsigned int irqnr);
int csp_rot_clear_irq(unsigned int irqnr);
bool csp_rot_query_and_clear_irq(unsigned int irqnr);
int csp_rot_set_cfg_ch(unsigned int chn, rot_cfg_ch_t *cfgs);
int csp_rot_set_cfg(rot_cfg_t *cfgs);
int csp_rot_set_mode(unsigned int mode);
int csp_rot_get_status(rot_status_t *status);
int csp_rot_get_time(unsigned int freq, unsigned int *ms);
int csp_rot_set_input_dma(rot_dma_cfgs_t *dma_cfgs);
int csp_rot_set_output_dma(rot_dma_cfgs_t *dma_cfgs);

#endif /* __CSP_DPU_ROT_H__ */
