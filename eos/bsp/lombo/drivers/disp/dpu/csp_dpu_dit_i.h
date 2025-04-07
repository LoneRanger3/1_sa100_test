/*
 * csp_dpu_dit_i.h - Dpu dit interrupt module head file
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

/* Interrupt source of dit */
typedef enum tag_dit_irq_src {
	DIT_IRQ_WB_FINISH       = 1 << 0,   /* trig when finished */
	DIT_IRQ_WB_OVERFLOW  = 1 << 1,   /* trig when fifo overflow */
	DIT_IRQ_WB_TIMEOUT    =  1 << 2,   /* trig when wb timeout */
} __dit_irq_src_t;

typedef enum tag_dit_field_pol {
	DIT_FIRST_FIELD = 0,
	DIT_SECOND_FIELD = 1,
} __dit_field_pol_t;

typedef enum tag_dit_field_seq {
	DIT_TFF_SEQ = 0,
	DIT_BFF_SEQ = 1,
} __dit_field_seq_t;
