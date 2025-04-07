/*
 * csp_dpu_dit.h - Dpu dit module head file
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

#include "csp_dpu_dit_i.h"

enum dit_irq_src {
	DIT_IRQ_SRC_WB_FINISH = 1 << 0,   /* trig when finished */
	DIT_IRQ_SRC_WB_OVERFLOW = 1 << 1, /* trig when fifo overflow */
	DIT_IRQ_SRC_WB_TIMEOUT = 1 << 2,  /* trig when wb timeout */
};

enum dit_irq_status {
	DIT_IRQ_STATUS_IRQ_FINISH = 1 << 0,
	DIT_IRQ_STATUS_IRQ_OVERFLOW = 1 << 1,
	DIT_IRQ_STATUS_IRQ_TIMEOUT = 1 << 2,
};

/* ======common========================== */
s32 csp_dit_init(void);
s32 csp_dit_exit(void);
s32 csp_dit_set_register_base(uintptr_t addr, u32 size);
u32 csp_dit_get_ver(u32 *major, u32 *minor);
s32 csp_dit_enable(void);
s32 csp_dit_disable(void);
s32 csp_dit_reset(void);
s32 csp_dit_set_field_pol(u32 pol);
s32 csp_dit_set_pixseq(u32 is_inversed);
s32 csp_dit_set_mode(u32 dit_mode);
s32 csp_dit_enable_still_avg(void);
s32 csp_dit_disable_still_avg(void);
/* ======irq and status========================== */
s32 csp_dit_get_irq_status(void);
s32 csp_dit_clear_irq_status(__dit_irq_src_t irq_trig);
s32 csp_dit_enable_irq_src(__dit_irq_src_t wb_trig);
s32 csp_dit_disable_irq_src(__dit_irq_src_t wb_trig);
/* ======size========================== */
s32 csp_dit_set_input_addr(u32 addr[4]);
s32 csp_dit_set_output_addr(u32 addr);
s32 csp_dit_set_insize(u32 inw, u32 inh);
s32 csp_dit_set_outsize(u32 outw, u32 outh);
s32 csp_dit_set_input_linestride(u32 lstr);
s32 csp_dit_set_output_linestride(u32 lstr);
/* ======wb========================== */
s32 csp_dit_wb_start(void);
s32 csp_dit_wb_stop(void);
s32 csp_dit_set_wb_timer_thr(u32 timeout_len);
s32 csp_dit_get_wb_timer_status(void);
s32 csp_dit_get_wb_timer_thr(void);
s32 csp_dit_check_wb_finish(void);
s32 csp_dit_check_wb_timeout(void);
s32 csp_dit_check_wb_overflow(void);
/* ======paras========================== */
s32 csp_dit_set_still_threshold(u32 still_thr);
s32 csp_dit_set_interpolation_threshold(u32 thr0, u32 thr1, u32 thr2, u32 thr3);
void csp_dit_cal_field_addr_from_frame_addr(u32 frameaddr[3], u32 pfields[4],
	u32 lstr, __dit_field_seq_t fdseq, __dit_field_pol_t F0_pol);

