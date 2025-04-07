/*
 * csp_dpu_dit_reg.h - Dpu dit register module head file
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

 /* -----------------------------------------------------------
 *
 * REG_DIT.h
 *
 * ----------------------------------------------------------- */

#ifndef __REG_DIT___H___
#define __REG_DIT___H___

#include "../disp_common.h"

#define BASE_DIT            (0x01500000+0x00090000)

/* ----------------------------------------------------------- */
#define VA_DIT_VER       (0x00000000 + BASE_DIT)
#define VA_DIT_FTR       (0x00000004 + BASE_DIT)
#define VA_DIT_BURSTCTL  (0x00000008 + BASE_DIT)
#define VA_DIT_STA       (0x0000000C + BASE_DIT)
#define VA_DIT_CTL       (0x00000010 + BASE_DIT)
#define VA_DIT_CFG       (0x00000014 + BASE_DIT)
#define VA_DIT_WBCTL     (0x00000018 + BASE_DIT)
#define VA_DIT_WBTMR     (0x0000001C + BASE_DIT)
#define VA_DIT_INADDR0   (0x00000020 + BASE_DIT)
#define VA_DIT_INADDR1   (0x00000024 + BASE_DIT)
#define VA_DIT_INADDR2   (0x00000028 + BASE_DIT)
#define VA_DIT_INADDR3   (0x0000002C + BASE_DIT)
#define VA_DIT_OUTADDR   (0x00000030 + BASE_DIT)
#define VA_DIT_INSIZE    (0x00000034 + BASE_DIT)
#define VA_DIT_OUTSIZE   (0x00000038 + BASE_DIT)
#define VA_DIT_LSTR      (0x0000003C + BASE_DIT)
#define VA_DIT_WBINTCTL  (0x00000040 + BASE_DIT)
#define VA_DIT_WBINTSTA  (0x00000044 + BASE_DIT)
#define VA_DIT_WBINTCLR  (0x00000048 + BASE_DIT)
#define VA_DIT_STHR      (0x00000050 + BASE_DIT)
#define VA_DIT_ITHR      (0x00000054 + BASE_DIT)

/* ----------------------------------------------------------- */
#define REG_DIT_VER		(0x00000000)
#define REG_DIT_FTR		(0x00000004)
#define REG_DIT_BURSTCTL	(0x00000008)
#define REG_DIT_STA		(0x0000000C)
#define REG_DIT_CTL		(0x00000010)
#define REG_DIT_CFG		(0x00000014)
#define REG_DIT_WBCTL		(0x00000018)
#define REG_DIT_WBTMR		(0x0000001C)
#define REG_DIT_INADDR0	(0x00000020)
#define REG_DIT_INADDR1	(0x00000024)
#define REG_DIT_INADDR2	(0x00000028)
#define REG_DIT_INADDR3	(0x0000002C)
#define REG_DIT_OUTADDR	(0x00000030)
#define REG_DIT_INSIZE		(0x00000034)
#define REG_DIT_OUTSIZE	(0x00000038)
#define REG_DIT_LSTR		(0x0000003C)
#define REG_DIT_WBINTCTL	(0x00000040)
#define REG_DIT_WBINTSTA	(0x00000044)
#define REG_DIT_WBINTCLR	(0x00000048)
#define REG_DIT_STHR		(0x00000050)
#define REG_DIT_ITHR		(0x00000054)
#define REG_DIT_SPCNT		(0x00000058)

#define DATA_DIT_VER                 0x00001100
#define DATA_DIT_FTR                 0x00000001
#define DATA_DIT_BURSTCTL            0x00000000
#define DATA_DIT_STA                 0x00000000
#define DATA_DIT_CTL                 0x00000000
#define DATA_DIT_CFG                 0x00000000
#define DATA_DIT_WBCTL               0x00000000
#define DATA_DIT_WBTMR               0x0000FFFF
#define DATA_DIT_INADDR0             0x00000000
#define DATA_DIT_INADDR1             0x00000000
#define DATA_DIT_INADDR2             0x00000000
#define DATA_DIT_INADDR3             0x00000000
#define DATA_DIT_OUTADDR             0x00000000
#define DATA_DIT_INSIZE              0x00000000
#define DATA_DIT_OUTSIZE             0x00000000
#define DATA_DIT_LSTR                0x00000000
#define DATA_DIT_WBINTCTL            0x00000000
#define DATA_DIT_WBINTSTA            0x00000000
#define DATA_DIT_WBINTCLR            0x00000000
#define DATA_DIT_STHR                0x00000000
#define DATA_DIT_ITHR                0x00000000
#define DATA_DIT_SPCNT               0x00000000

/* DIT Version Register */
typedef union {
	u32 val;
	struct {
		u32 ver_l:8;	/* The low 8bits of version register */
		u32 ver_h:3;	/* The high 3bits of version register */
		u32 rsvd0:1;	/* - */
		u32 comp:1;	/* Backward Compatibility
			* [0x0]: Incompatible to last version
			* of hardware
			* [0x1]: Compatible to last version of hardware */
		u32 rsvd1:19;	/* - */
	} bits;
} reg_dit_ver_t;

#define DIT_VER_COMP_0  0x0
#define DIT_VER_COMP_1  0x1

/* DIT Feature Register */
typedef union {
	u32 val;
	struct {
		u32 has_ftr0:1;	/* DIT offline mode supported
				 * [0x0]: Not supported
				 * [0x1]: Supported */
		u32 has_ftr1:1;	/* DIT online mode supported
				 * [0x0]: Not supported
				 * [0x1]: Supported */
		u32 has_ftr2:1;	/* Online 1080i supported feature
				 * [0x0]: Not exist
				 * [0x1]: Exist */
		u32 rsvd0:29;	/* - */
	} bits;
} reg_dit_ftr_t;

#define DIT_FTR_HAS_FTR0_0      0x0
#define DIT_FTR_HAS_FTR0_1      0x1
#define DIT_FTR_HAS_FTR1_0      0x0
#define DIT_FTR_HAS_FTR1_1      0x1
#define DIT_FTR_HAS_FTR2_0      0x0
#define DIT_FTR_HAS_FTR2_1      0x1

/* DIT Status Register */
typedef union {
	u32 val;
	struct {
		u32 rsvd0:16;	/* - */
		u32 rdsta:1;	/* [0x0]: Input DMA idle
				 * [0x1]: Input DMA reading DRAM */
		u32 wrsta:1;	/* [0x0]: WB DMA idle
				 * [0x1]: WB DMA writing DRAM */
		u32 rsvd1:14;	/* - */
	} bits;
} reg_dit_sta_t;

#define DIT_STA_RDSTA_0 0x0
#define DIT_STA_RDSTA_1 0x1
#define DIT_STA_WRSTA_0 0x0
#define DIT_STA_WRSTA_1 0x1

/* DIT Control Register */
typedef union {
	u32 val;
	struct {
		u32 rst:1;
		u32 rsvd0:28;	/* - */
		u32 bisten:1;	/* Bist function, mapping internal sram to ahb
				 * [0x0]: internal
				 * [0x1]: mapped to ahb */
		u32 dbg_ctl:1;	/* Debug register access control
			* [0x0]: Debug register can't be read or written
			* [0x1]: Debug register can be read or written */
		u32 dit_en:1;	/* Whole DIT module enable control
				 * [0x0]: Disabled
				 * [0x1]: Enabled
				 * Write ¡°0¡± to this bit will disable
				 * the whole module and gate all clocks
				 * Write ¡°1¡± to this bit will enable
				 * the clock of this module and reset interrupt
				 * / flag registers. */
	} bits;
} reg_dit_ctl_t;

#define DIT_CTL_BISTEN_0        0x0
#define DIT_CTL_BISTEN_1        0x1
#define DIT_CTL_DBG_CTL_0       0x0
#define DIT_CTL_DBG_CTL_1       0x1
#define DIT_CTL_DIT_EN_0        0x0
#define DIT_CTL_DIT_EN_1        0x1

/* DIT Configuration Register */
typedef union {
	u32 val;
	struct {
		u32 ditmode:1; /* DIT mode
				* [0x0]: Luma mode, Y/U/V planar
				* [0x1]: Chroma mode, UV-combine/semi- planar */
		u32 rsvd0:7; /* - */
		u32 pixseq:1; /* Pixel sequence for output in Chroma
				 * Mode, no use in Luma Mode
				 * [0x0]: As read in sequence
				 * [0x1]: Inversed sequence */
		u32 field_pol:1; /* Field polarity of current interpolated
					 * field(F1), inversed value for start
					 * field polarity(F0)
					 * [0x0]: top field
					 * [0x1]: bottom field */
		u32 rsvd1:6;	/* - */
		u32 rsvd2:16;	/* - */
	} bits;
} reg_dit_cfg_t;

#define DIT_CFG_DITMODE_0       0x0
#define DIT_CFG_DITMODE_1       0x1
#define DIT_CFG_PIXSEQ_0        0x0
#define DIT_CFG_PIXSEQ_1        0x1
#define DIT_CFG_FIELD_POL_0     0x0
#define DIT_CFG_FIELD_POL_1     0x1

/* DIT WB Control Register */
typedef union {
	u32 val;
	struct {
		u32 start:1;	/* [0x0]: Stop
				 * [0x1]: Start
				 * Write ¡°1¡± to this bit will start DIT.
				 * Write ¡°0¡± to this bit will stop processing
				 * frame.
				 * All DIT register is updated by setting
				 * this bit from ¡°0¡± to ¡°1¡±. */
		u32 rsvd0:31;	/* - */
	} bits;
} reg_dit_wbctl_t;

#define DIT_WBCTL_START_0       0x0
#define DIT_WBCTL_START_1       0x1

/* DIT WB Timer Register */
typedef union {
	u32 val;
	struct {
		u32 wbthr:16;	/* When WB timer excess this threshold,
				 * timeout flag will be pulled. */
		u32 wbtmr:16;	/* The counter will be reset to 0 when
				 * write-back reset or start and be updated
				 * after write-back finished. The counter
				 * will be held after excess 0xFFFF.
				 * Timer clock is decided by TCK_DIV setting
				 * in DPU. */
	} bits;
} reg_dit_wbtmr_t;

/* DIT Input Address 0 Register */
typedef union {
	u32 val;
	struct {
		u32 addr:32;	/* DIT input address0 */
	} bits;
} reg_dit_inaddr0_t;

/* DIT Input Address 1 Register */
typedef union {
	u32 val;
	struct {
		u32 addr:32;	/* DIT input address1 */
	} bits;
} reg_dit_inaddr1_t;

/* DIT Input Address 2 Register */
typedef union {
	u32 val;
	struct {
		u32 addr:32;	/* DIT input address2 */
	} bits;
} reg_dit_inaddr2_t;

/* DIT Input Address 3 Register */
typedef union {
	u32 val;
	struct {
		u32 addr:32;	/* DIT input address3 */
	} bits;
} reg_dit_inaddr3_t;

/* DIT Output Address Register */
typedef union {
	u32 val;
	struct {
		u32 addr:32;	/* Frame output address */
	} bits;
} reg_dit_outaddr_t;

/* DIT Input Size Register */
typedef union {
	u32 val;
	struct {
		u32 w:12;	/* Input image width
				 * Real width=W+1 */
		u32 rsvd0:4;	/* - */
		u32 h:12;	/* Input image height
				 * Real height=H+1 */
		u32 rsvd1:4;	/* - */
	} bits;
} reg_dit_insize_t;

/* DIT Output Size Register */
typedef union {
	u32 val;
	struct {
		u32 w:12;	/* Output image width
				 * Real width=W+1 */
		u32 rsvd0:4;	/* - */
		u32 h:12;	/* Output image height
				 * Real height=H+1 */
		u32 rsvd1:4;	/* - */
	} bits;
} reg_dit_outsize_t;

/* DIT Line Stride Register */
typedef union {
	u32 val;
	struct {
		u32 ilstr:12;	/* Input image line stride */
		u32 rsvd0:4;	/* - */
		u32 olstr:12;	/* Output image line stride */
		u32 rsvd1:4;	/* - */
	} bits;
} reg_dit_lstr_t;

/* DIT WB Interrupt Control Register */
typedef union {
	u32 val;
	struct {
		u32 wbfin_en:1; /* Enable finish signal to trig interrupt
				 * [0x0]: Disable
				 * [0x1]: Enable */
		u32 wbovfl_en:1; /* Enable overflow signal to trig interrupt
					 * [0x0]: Disable
					 * [0x1]: Enable */
		u32 wbtmout_en:1; /* Enable timeout signal to trig interrupt
					 * [0x0]: Disable
					 * [0x1]: Enable */
		u32 rsvd0:29;	/* - */
	} bits;
} reg_dit_wbintctl_t;

#define DIT_WBINTCTL_WBFIN_EN_0         0x0
#define DIT_WBINTCTL_WBFIN_EN_1         0x1
#define DIT_WBINTCTL_WBOVFL_EN_0        0x0
#define DIT_WBINTCTL_WBOVFL_EN_1        0x1
#define DIT_WBINTCTL_WBTMOUT_EN_0       0x0
#define DIT_WBINTCTL_WBTMOUT_EN_1       0x1

/* DIT WB Interrupt Status Register */
typedef union {
	u32 val;
	struct {
		u32 wbfin:1;	/* Write-back finish normally
				 * [0x0]: idle
				 * [0x1]: finish normally */
		u32 wbovfl:1;	/* Write-back FIFO overflow
				 * [0x0]: no overflow
				 * [0x1]: overflow occur */
		u32 wbtmout:1;	/* Write-back timeout
				 * [0x0]: no timeout
				 * [0x1]: timeout occur
				 * When DIT start processing, a counter
				 * in will be check with WBTHR, if greater
				 * than WBTHR, timeout flag is set and
				 * trigger the WB pending bit. */
		u32 rsvd0:29;	/* - */
	} bits;
} reg_dit_wbintsta_t;

#define DIT_WBINTSTA_WBFIN_0    0x0
#define DIT_WBINTSTA_WBFIN_1    0x1
#define DIT_WBINTSTA_WBOVFL_0   0x0
#define DIT_WBINTSTA_WBOVFL_1   0x1
#define DIT_WBINTSTA_WBTMOUT_0  0x0
#define DIT_WBINTSTA_WBTMOUT_1  0x1

/* DIT WB Interrupt Clear Register */
typedef union {
	u32 val;
	struct {
		u32 wbfinclr:1;	/* Write '1' to clear this bit.
				* Reading this bit always return '0' . */
		u32 wbovflclr:1;	/* Write '1' to clear this bit.
				* Reading this bit always return '0' . */
		u32 wbtmoutclr:1;	/* Write '1' to clear this bit.
				* Reading this bit always return '0' . */
		u32 rsvd0:29;	/* - */
	} bits;
} reg_dit_wbintclr_t;

/* DIT Still Threshold Register */
typedef union {
	u32 val;
	struct {
		u32 sthr0:8;	/* Still threshold0, unsigned 8bit */
		u32 rsvd0:23;	/* - */
		u32 stavg:1;	/* Still average enable
				 * [0x0]: Disable
				 * [0x1]: Enable */
	} bits;
} reg_dit_sthr_t;

#define DIT_STHR_STAVG_0        0x0
#define DIT_STHR_STAVG_1        0x1

/* DIT Interpolation Threshold Register */
typedef union {
	u32 val;
	struct {
		u32 thr0:8;	/* Interpolation threshold0, unsigned 8bit */
		u32 thr1:8;	/* Interpolation threshold1, unsigned 8bit */
		u32 thr2:8;	/* Interpolation threshold2, unsigned 8bit */
		u32 thr3:8;	/* Interpolation threshold3, unsigned 8bit */
	} bits;
} reg_dit_ithr_t;

/* DIT Still Pixels Counter Register */
typedef union {
	u32 val;
	struct {
		u32 cnt:20;	/* Counter for still pixels every frame
				 * detected.
				 * Auto clear to 0 when WB_CTL.START bit
				 * is changed from ¡®0¡¯ to ¡®1¡¯ */
		u32 rsvd0:12;	/* - */
	} bits;
} reg_dit_spcnt_t;

/* reg_dit_t bank */
typedef struct tag_dit {
	reg_dit_ver_t ver;	/* 0000 */
	reg_dit_ftr_t ftr;	/* 0004 */
	u32 prv_0008;		/* 0008 */
	reg_dit_sta_t sta;	/* 000C */
	reg_dit_ctl_t ctl;	/* 0010 */
	reg_dit_cfg_t cfg;	/* 0014 */
	reg_dit_wbctl_t wbctl;	/* 0018 */
	reg_dit_wbtmr_t wbtmr;	/* 001C */
	reg_dit_inaddr0_t inaddr0;	/* 0020 */
	reg_dit_inaddr1_t inaddr1;	/* 0024 */
	reg_dit_inaddr2_t inaddr2;	/* 0028 */
	reg_dit_inaddr3_t inaddr3;	/* 002C */
	reg_dit_outaddr_t outaddr;	/* 0030 */
	reg_dit_insize_t insize;	/* 0034 */
	reg_dit_outsize_t outsize;	/* 0038 */
	reg_dit_lstr_t lstr;	/* 003C */
	reg_dit_wbintctl_t wbintctl;	/* 0040 */
	reg_dit_wbintsta_t wbintsta;	/* 0044 */
	reg_dit_wbintclr_t wbintclr;	/* 0048 */
	u32 res0[1];
	reg_dit_sthr_t sthr;	/* 0050 */
	reg_dit_ithr_t ithr;	/* 0054 */
	reg_dit_spcnt_t spcnt;	/* 0058 */
} __regs_dit_t;

/* -------------------------------------------------- */
#endif /* __REG_DIT___H___ */
