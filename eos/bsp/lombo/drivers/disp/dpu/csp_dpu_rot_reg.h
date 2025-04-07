/*
 * csp_dpu_rot_reg.h - Dpu rot ger module head file
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

#ifndef __CSP_DPU_ROT_REF_H__
#define __CSP_DPU_ROT_REF_H__

#define BASE_ROT                0x01580000

#define VA_ROT_VER               (0x00000000 + BASE_ROT + VA_ROT)
#define VA_ROT_FTR               (0x00000004 + BASE_ROT + VA_ROT)
#define VA_ROT_CTL               (0x00000010 + BASE_ROT + VA_ROT)
#define VA_ROT_CFG               (0x00000014 + BASE_ROT + VA_ROT)
#define VA_ROT_STRMCTL           (0x00000018 + BASE_ROT + VA_ROT)
#define VA_ROT_STA               (0x0000001C + BASE_ROT + VA_ROT)
#define VA_ROT_INTEN             (0x00000020 + BASE_ROT + VA_ROT)
#define VA_ROT_INTPD             (0x00000024 + BASE_ROT + VA_ROT)
#define VA_ROT_INTCLR            (0x00000028 + BASE_ROT + VA_ROT)
#define VA_ROT_INCFG             (0x00000030 + BASE_ROT + VA_ROT)
#define VA_ROT_INSIZE0           (0x00000040 + BASE_ROT + VA_ROT)
#define VA_ROT_INADD0            (0x00000044 + BASE_ROT + VA_ROT)
#define VA_ROT_INLNSTD0          (0x00000048 + BASE_ROT + VA_ROT)
#define VA_ROT_INSIZE1           (0x00000050 + BASE_ROT + VA_ROT)
#define VA_ROT_INADD1            (0x00000054 + BASE_ROT + VA_ROT)
#define VA_ROT_INLNSTD1          (0x00000058 + BASE_ROT + VA_ROT)
#define VA_ROT_INSIZE2           (0x00000060 + BASE_ROT + VA_ROT)
#define VA_ROT_INADD2            (0x00000064 + BASE_ROT + VA_ROT)
#define VA_ROT_INLNSTD2          (0x00000068 + BASE_ROT + VA_ROT)
#define VA_ROT_OUTCFG            (0x00000070 + BASE_ROT + VA_ROT)
#define VA_ROT_OUTSIZE0          (0x00000080 + BASE_ROT + VA_ROT)
#define VA_ROT_OUTADD0           (0x00000084 + BASE_ROT + VA_ROT)
#define VA_ROT_OUTLNSTD0         (0x00000088 + BASE_ROT + VA_ROT)
#define VA_ROT_OUTSIZE1          (0x00000090 + BASE_ROT + VA_ROT)
#define VA_ROT_OUTADD1           (0x00000094 + BASE_ROT + VA_ROT)
#define VA_ROT_OUTLNSTD1         (0x00000098 + BASE_ROT + VA_ROT)
#define VA_ROT_OUTSIZE2          (0x000000A0 + BASE_ROT + VA_ROT)
#define VA_ROT_OUTADD2           (0x000000A4 + BASE_ROT + VA_ROT)
#define VA_ROT_OUTLNSTD2         (0x000000A8 + BASE_ROT + VA_ROT)
#define VA_ROT_DMASCTL0          (0x000000B0 + BASE_ROT + VA_ROT)
#define VA_ROT_DMADCTL0          (0x000000B4 + BASE_ROT + VA_ROT)
#define VA_ROT_DMACFG0           (0x000000B8 + BASE_ROT + VA_ROT)

#define DATA_ROT_VER                 0x00001100
#define DATA_ROT_FTR                 0x00000000
#define DATA_ROT_CTL                 0x00000000
#define DATA_ROT_CFG                 0x00000000
#define DATA_ROT_STRMCTL             0x00000000
#define DATA_ROT_STA                 0x00000000
#define DATA_ROT_INTEN               0x00000000
#define DATA_ROT_INTPD               0x00000000
#define DATA_ROT_INTCLR              0x00000000
#define DATA_ROT_INCFG               0x00000000
#define DATA_ROT_INSIZE0             0x00000000
#define DATA_ROT_INADD0              0x00000000
#define DATA_ROT_INLNSTD0            0x00000000
#define DATA_ROT_INSIZE1             0x00000000
#define DATA_ROT_INADD1              0x00000000
#define DATA_ROT_INLNSTD1            0x00000000
#define DATA_ROT_INSIZE2             0x00000000
#define DATA_ROT_INADD2              0x00000000
#define DATA_ROT_INLNSTD2            0x00000000
#define DATA_ROT_OUTCFG              0x00000000
#define DATA_ROT_OUTSIZE0            0x00000000
#define DATA_ROT_OUTADD0             0x00000000
#define DATA_ROT_OUTLNSTD0           0x00000000
#define DATA_ROT_OUTSIZE1            0x00000000
#define DATA_ROT_OUTADD1             0x00000000
#define DATA_ROT_OUTLNSTD1           0x00000000
#define DATA_ROT_OUTSIZE2            0x00000000
#define DATA_ROT_OUTADD2             0x00000000
#define DATA_ROT_OUTLNSTD2           0x00000000
#define DATA_ROT_DMASCTL0            0x01000002
#define DATA_ROT_DMADCTL0            0x01000002
#define DATA_ROT_DMACFG0             0x0F0F0022

/* ROT Version Register */
typedef union {
	u32 val;
	struct {
	u32 ver_l:5;            /* The low 5bits of version register */
	u32 rsvd0:3;            /* - */
	u32 ver_h:3;            /* The high 3bits of version register */
	u32 rsvd1:1;            /* - */
	u32 comp:1;             /* Backward Compatibility
				 * [1' b0]: Incompatible to last version
				 * of hardware
				 * [1' b1]: Compatible to last version
				 * of hardware */
	u32 rsvd2:19;           /* - */
	} bits;
} reg_rot_ver_t;

#define ROT_VER_COMP_0  0x0
#define ROT_VER_COMP_1  0x1

/* ROT Feature Register */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:32;           /* - */
	} bits;
} reg_rot_ftr_t;

/* ROT Control Register */
typedef union {
	u32 val;
	struct {
	u32 en:1;               /* Whole ROT module enable control
				 * [0x0]: disable the module
				 * [0x1]: enable the module */
	u32 rsvd0:3;            /* - */
	u32 bypass:1;           /* Bypass ROT process
				 * [0x0]: Normal work mode
				 * [0x1]: Bypass rot/flip process, only
				 * dma will be at work */
	u32 rsvd1:3;            /* - */
	u32 dmaen0:1;           /* Enable dma channel 0
				 * [0x0]: Disable
				 * [0x1]: Enable */
	u32 rsvd2:3;            /* - */
	u32 dmasspnd:1;         /* Suspend dma channel 0
				 * [0x0]: Normal/Resume
				 * [0x1]: Suspend */
	u32 rsvd3:18;           /* - */
	u32 rst:1;              /* [0x0]: soft reset the module.
				 * [0x1]: release the reset signal.
				 * Write "0" to this bit will reset the
				 * whole module. */
	} bits;
} reg_rot_ctl_t;

#define ROT_CTL_EN_0            0x0
#define ROT_CTL_EN_1            0x1
#define ROT_CTL_BYPASS_0        0x0
#define ROT_CTL_BYPASS_1        0x1
#define ROT_CTL_DMAEN0_0        0x0
#define ROT_CTL_DMAEN0_1        0x1
#define ROT_CTL_DMASSPND_0      0x0
#define ROT_CTL_DMASSPND_1      0x1
#define ROT_CTL_RST_0           0x0
#define ROT_CTL_RST_1           0x1

/* ROT Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 mode:3;             /* Rotation/flip mode select:
				 * [0x0]: Mode 0
				 * [0x1]: Mode 1
				 * [0x2]: Mode 2
				 * [0x3]: Mode 3
				 * [0x4]: Mode 4
				 * [0x5]: Mode 5
				 * [0x6]: Mode 6
				 * [0x7]: Mode 7 */
	u32 rsvd0:3;            /* - */
	u32 rsvd1:10;           /* - */
	u32 bpp0:2;             /* Bytes per pixel stored in channel 0
				 * (Disregard subsample rate).
				 * Actual bytes per pixel is BPP0 + 1. */
	u32 rsvd2:2;            /* - */
	u32 bpp1:2;             /* Bytes per pixel stored in channel 1
				 * (Disregard subsample rate).
				 * definition is the same as BPP0 */
	u32 rsvd3:2;            /* - */
	u32 bpp2:2;             /* Bytes per pixel stored in channel 2
				 * (Disregard subsample rate).
				 * definition is the same as BPP0 */
	u32 rsvd4:2;            /* - */
	u32 chsel:3;            /* Specified channel to be process, 0 of
				 * bit28 means channel 0 is not to be process,
				 * while 1 means is to be process. The
				 * meaning of bit29/30 is the same for
				 * channel 1/2.
				 * You can choose any channel by setting
				 * the corresponding bit. */
	u32 rsvd5:1;            /* - */
	} bits;
} reg_rot_cfg_t;

#define ROT_CFG_MODE_0  0x0
#define ROT_CFG_MODE_1  0x1
#define ROT_CFG_MODE_2  0x2
#define ROT_CFG_MODE_3  0x3
#define ROT_CFG_MODE_4  0x4
#define ROT_CFG_MODE_5  0x5
#define ROT_CFG_MODE_6  0x6
#define ROT_CFG_MODE_7  0x7

/* ROT Stream Contol Register */
typedef union {
	u32 val;
	struct {
	u32 start:1;            /* Write 1 to this bit will start rotation/flip
				 * process. After completion of the transfer,
				 * the hardware will clear this bit to 0. */
	u32 rsvd0:31;           /* - */
	} bits;
} reg_rot_strmctl_t;

/* ROT Status Register */
typedef union {
	u32 val;
	struct {
	u32 insta:1;            /* [1' b0]: input dma is idle
				 * [1' b1]: input dma is transferring */
	u32 rsvd0:3;            /* - */
	u32 outsta:1;           /* [1' b0]: output dma is idle
				 * [1' b1]: output dma is transferring */
	u32 rsvd1:11;           /* - */
	u32 tmr:16;             /* Elapse time of the last process in 2.73ms
				 * unit. The value will be reset to 0 when
				 * module reset or module start, and will
				 * be updated after process finished.
				 * Its clock parent is 24Mhz clock with
				 * pre-divider of 65536, So the period
				 * is 65536/24Mhz=2.73ms. */
	} bits;
} reg_rot_sta_t;

#define ROT_STA_INSTA_0         0x0
#define ROT_STA_INSTA_1         0x1
#define ROT_STA_OUTSTA_0        0x0
#define ROT_STA_OUTSTA_1        0x1

/* ROT Interrupt Enable Register */
typedef union {
	u32 val;
	struct {
	u32 srcblk:1;           /* DMA source block transfer done interrupt
				 * enable
				 * [0x0]: Disabled
				 * [0x1]: Enabled */
	u32 srctran:1;          /* DMA source transfer done interrupt enable
				 * [0x0]: Disabled
				 * [0x1]: Enabled */
	u32 dstblk:1;           /* DMA destination block transfer done
				 * interrupt enable
				 * [0x0]: Disabled
				 * [0x1]: Enabled */
	u32 dsttran:1;          /* DMA destination transfer done interrupt
				 * enable
				 * [0x0]: Disabled
				 * [0x1]: Enabled */
	u32 srcdecerr:1;        /* DMA source decode error interrupt enable
				 * [0x0]: Disabled
				 * [0x1]: Enabled */
	u32 rsvd0:1;            /* - */
	u32 dstdecerr:1;        /* DMA destination decode error interrupt enable
				 * [0x0]: Disabled
				 * [0x1]: Enabled */
	u32 rsvd1:24;           /* - */
	u32 fnsh:1;             /* Processing finish interrupt enable
				 * [0x0]: Disabled
				 * [0x1]: Enabled */
	} bits;
} reg_rot_inten_t;

#define ROT_INTEN_SRCBLK_0      0x0
#define ROT_INTEN_SRCBLK_1      0x1
#define ROT_INTEN_SRCTRAN_0     0x0
#define ROT_INTEN_SRCTRAN_1     0x1
#define ROT_INTEN_DSTBLK_0      0x0
#define ROT_INTEN_DSTBLK_1      0x1
#define ROT_INTEN_DSTTRAN_0     0x0
#define ROT_INTEN_DSTTRAN_1     0x1
#define ROT_INTEN_SRCDECERR_0   0x0
#define ROT_INTEN_SRCDECERR_1   0x1
#define ROT_INTEN_DSTDECERR_0   0x0
#define ROT_INTEN_DSTDECERR_1   0x1
#define ROT_INTEN_FNSH_0        0x0
#define ROT_INTEN_FNSH_1        0x1

/* ROT Interrupt Pending Register */
typedef union {
	u32 val;
	struct {
	u32 srcblk:1;           /* DMA source block transfer done interrupt
				 * pending */
	u32 srctran:1;          /* DMA source transfer done interrupt pending */
	u32 dstblk:1;           /* DMA destination block transfer done
				 * interrupt pending */
	u32 dsttran:1;          /* DMA destination transfer done interrupt
				 * pending */
	u32 srcdecerr:1;        /* DMA source decode error interrupt pending */
	u32 rsvd0:1;            /* - */
	u32 dstdecerr:1;        /* DMA destination decode error interrupt
				 * pending */
	u32 rsvd1:24;           /* - */
	u32 fnsh:1;             /* Processing finish interrupt pending */
	} bits;
} reg_rot_intpd_t;

/* ROT Interrupt Clear Register */
typedef union {
	u32 val;
	struct {
	u32 srcblk:1;           /* DMA source block transfer done interrupt
				 * clear.
				 * Write 1 to clear SRCBLK at INTPD */
	u32 srctran:1;          /* DMA source transfer done interrupt clear.
				 * Write 1 to clear SRCTRAN at INTPD */
	u32 dstblk:1;           /* DMA destination block transfer done
				 * interrupt clear.
				 * Write 1 to clear DSTBLK at INTPD */
	u32 dsttran:1;          /* DMA destination transfer done interrupt
				 * clear.
				 * Write 1 to clear DSTTRAN at INTPD */
	u32 srcdecerr:1;        /* DMA source decode error interrupt clear.
				 * Write 1 to clear SRCDECERR at INTPD */
	u32 rsvd0:1;            /* - */
	u32 dstdecerr:1;        /* DMA destination decode error interrupt clear.
				 * Write 1 to clear DSTDECERR at INTPD */
	u32 rsvd1:24;           /* - */
	u32 fnsh:1;             /* Processing finish interrupt clear.
				 * Write 1 to clear FNSH at INTPD */
	} bits;
} reg_rot_intclr_t;

/* ROT Input Configuration Register  */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:32;           /* - */
	} bits;
} reg_rot_incfg_t;

/* ROT Input Buffer Size0 Register */
typedef union {
	u32 val;
	struct {
	u32 w:13;               /* Input buffer width for ch0 in pixels.
				 * Actual buffer width is W + 1 */
	u32 rsvd0:3;            /* - */
	u32 h:13;               /* Input buffer height for ch0 in pixels
				 * Actual buffer height is H + 1 */
	u32 rsvd1:3;            /* - */
	} bits;
} reg_rot_insize0_t;

/* ROT Input Buffer Address0 Register */
typedef union {
	u32 val;
	struct {
	u32 add:32;     /* Input buffer address for ch0 in bytes. */
	} bits;
} reg_rot_inadd0_t;

/* ROT Input Buffer Line Stride0 Register */
typedef union {
	u32 val;
	struct {
	u32 std:32;     /* Input buffer line stride for ch0 in bytes. */
	} bits;
} reg_rot_inlnstd0_t;

/* ROT Input Buffer Size1 Register */
typedef union {
	u32 val;
	struct {
	u32 w:13;               /* Input buffer width for ch1 in pixels.
				 * Actual buffer width is W + 1 */
	u32 rsvd0:3;            /* - */
	u32 h:13;               /* Input buffer height for ch1 in pixels
				 * Actual buffer height is H + 1 */
	u32 rsvd1:3;            /* - */
	} bits;
} reg_rot_insize1_t;

/* ROT Input Buffer Address1 Register */
typedef union {
	u32 val;
	struct {
	u32 add:32;     /* Input buffer address for ch1 in bytes. */
	} bits;
} reg_rot_inadd1_t;

/* ROT Input Buffer Line Stride1 Register */
typedef union {
	u32 val;
	struct {
	u32 std:32;     /* Input buffer line stride for ch1 in bytes. */
	} bits;
} reg_rot_inlnstd1_t;

/* ROT Input Buffer Size2 Register */
typedef union {
	u32 val;
	struct {
	u32 w:13;               /* Input buffer width for ch2 in pixels.
				 * Actual buffer width is W + 1 */
	u32 rsvd0:3;            /* - */
	u32 h:13;               /* Input buffer height for ch2 in pixels
				 * Actual buffer height is H + 1 */
	u32 rsvd1:3;            /* - */
	} bits;
} reg_rot_insize2_t;

/* ROT Input Buffer Address2 Register */
typedef union {
	u32 val;
	struct {
	u32 add:32;     /* Input buffer address for ch2 in bytes. */
	} bits;
} reg_rot_inadd2_t;

/* ROT Input Buffer Line Stride2 Register */
typedef union {
	u32 val;
	struct {
	u32 std:32;     /* Input buffer line stride for ch2 in bytes. */
	} bits;
} reg_rot_inlnstd2_t;

/* ROT Output Configuration Register  */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:32;           /* - */
	} bits;
} reg_rot_outcfg_t;

/* ROT Output Buffer Size0 Register */
typedef union {
	u32 val;
	struct {
	u32 w:13;               /* Output buffer width for ch0 in pixels.
				 * Actual buffer width is W + 1 */
	u32 rsvd0:3;            /* - */
	u32 h:13;               /* Output buffer height for ch0 in pixels
				 * Actual buffer height is H + 1 */
	u32 rsvd1:3;            /* - */
	} bits;
} reg_rot_outsize0_t;

/* ROT Output Buffer Address0 Register */
typedef union {
	u32 val;
	struct {
	u32 add:32;     /* Output buffer address for ch0 in bytes. */
	} bits;
} reg_rot_outadd0_t;

/* ROT Output Buffer Line Stride0 Register */
typedef union {
	u32 val;
	struct {
	u32 std:32;     /* Output buffer line stride for ch0 in bytes. */
	} bits;
} reg_rot_outlnstd0_t;

/* ROT Output Buffer Size1 Register */
typedef union {
	u32 val;
	struct {
	u32 w:13;               /* Output buffer width for ch1 in pixels.
				 * Actual buffer width is W + 1 */
	u32 rsvd0:3;            /* - */
	u32 h:13;               /* Output buffer height for ch1 in pixels
				 * Actual buffer height is H + 1 */
	u32 rsvd1:3;            /* - */
	} bits;
} reg_rot_outsize1_t;

/* ROT Output Buffer Address1 Register */
typedef union {
	u32 val;
	struct {
	u32 add:32;     /* Output buffer address for ch1 in bytes. */
	} bits;
} reg_rot_outadd1_t;

/* ROT Output Buffer Line Stride1 Register */
typedef union {
	u32 val;
	struct {
	u32 std:32;     /* Output buffer line stride for ch1 in bytes. */
	} bits;
} reg_rot_outlnstd1_t;

/* ROT Output Buffer Size2 Register */
typedef union {
	u32 val;
	struct {
	u32 w:13;               /* Output buffer width for ch2 in pixels.
				 * Actual buffer width is W + 1 */
	u32 rsvd0:3;            /* - */
	u32 h:13;               /* Output buffer height for ch2 in pixels
				 * Actual buffer height is H + 1 */
	u32 rsvd1:3;            /* - */
	} bits;
} reg_rot_outsize2_t;

/* ROT Output Buffer Address2 Register */
typedef union {
	u32 val;
	struct {
	u32 add:32;     /* Output buffer address for ch2 in bytes. */
	} bits;
} reg_rot_outadd2_t;

/* ROT Output Buffer Line Stride2 Register */
typedef union {
	u32 val;
	struct {
	u32 std:32;     /* Output buffer line stride for ch2 in bytes. */
	} bits;
} reg_rot_outlnstd2_t;

/* ROT DMA Source Control0 Register */
typedef union {
	u32 val;
	struct {
	u32 bstsize:3;          /* Burst size in bytes, the size of each
				 * transfer in the burst.
				 * the actual bust size = 2^BSTSIZE */
	u32 rsvd0:5;            /* - */
	u32 cache:4;            /* Memory cache type */
	u32 prot:3;             /* Memory acess permission type */
	u32 rsvd1:1;            /* - */
	u32 bstlen:8;           /* Max burst length, the exact number of
				 * transfers in a burst, valid when FIXBSTLEN
				 * is 0. */
	u32 fixbstlen:1;        /* Fix Burst length
				 * [0x0]: Alterable burst length, the max
				 * burst length will be specified by BSTLEN.
				 * [0x1]: Fix burst length, fixed by hardware */
	u32 rsvd2:5;            /* - */
	u32 shadowlast:1;       /* 1' b1 indicate the dma parameter is
				 * the one of the last transfer. */
	u32 shadowvalid:1;      /* 1' b1 indicate the dma parameter register
				 * is valid, used when shadow type */
	} bits;
} reg_rot_dmasctl0_t;

#define ROT_DMASCTL0_FIXBSTLEN_0        0x0
#define ROT_DMASCTL0_FIXBSTLEN_1        0x1

/* ROT DMA Destination Control0 Register */
typedef union {
	u32 val;
	struct {
	u32 bstsize:3;          /* Burst size in bytes, the size of each
				 * transfer in the burst.
				 * the actual bust size = 2^BSTSIZE */
	u32 rsvd0:5;            /* - */
	u32 cache:4;            /* Memory cache type */
	u32 prot:3;             /* Memory acess permission type */
	u32 rsvd1:1;            /* - */
	u32 bstlen:8;           /* Max burst length, the exact number of
				 * transfers in a burst, valid when FIXBSTLEN
				 * is 0. */
	u32 fixbstlen:1;        /* Fix Burst length
				 * [0x0]: Alterable burst length, the max
				 * burst length will be specified by BSTLEN.
				 * [0x1]: Fix burst length, fixed by hardware */
	u32 rsvd2:5;            /* - */
	u32 shadowlast:1;       /* 1' b1 indicate the dma parameter is
				 * the one of the last transfer. */
	u32 shadowvalid:1;      /* 1' b1 indicate the dma parameter register
				 * is valid, used when shadow type */
	} bits;
} reg_rot_dmadctl0_t;

#define ROT_DMADCTL0_FIXBSTLEN_0        0x0
#define ROT_DMADCTL0_FIXBSTLEN_1        0x1

/* ROT DMA Configuration0 Register */
typedef union {
	u32 val;
	struct {
	u32 srctrtype:2;        /* Source transfer type
				 * [0x0]: contintue type, the dma source
				 * parameters will need to be configured
				 * explicitly
				 * [0x1]: reserve
				 * [0x2]: shadow type, the dma source parameters
				 * will be filled based on rot/filp mode
				 * automatically
				 * [0x3]: reserve */
	u32 rsvd0:2;            /* - */
	u32 dsttrtype:2;        /* Destination transfer type
				 * [0x0]: contintue type, the dma destination
				 * parameters will need to be configured
				 * explicitly
				 * [0x1]: reserve
				 * [0x2]: shadow type, the dma destination
				 * parameters will be filled based on rot/filp
				 * mode automatically.
				 * [0x3]: reserve */
	u32 rsvd1:10;           /* - */
	u32 srcosrlmt:8;        /* Source outstanding request number limit.
				 * When the number of source outstanding
				 * request is out of this limit, dma will
				 * be suspended until any of these requests
				 * have been responded.
				 * The actual limit = SRCOSRLMT + 1 */
	u32 dstosrlmt:8;        /* Destination outstanding request number limit.
				 * When the number of destination outstanding
				 * request is out of this limit, dma will
				 * be suspended until any of these requests
				 * have been responded.
				 * The actual limit = DSTOSRLMT + 1 */
	} bits;
} reg_rot_dmacfg0_t;

#define ROT_DMACFG0_SRCTRTYPE_0 0x0
#define ROT_DMACFG0_SRCTRTYPE_1 0x1
#define ROT_DMACFG0_SRCTRTYPE_2 0x2
#define ROT_DMACFG0_SRCTRTYPE_3 0x3
#define ROT_DMACFG0_DSTTRTYPE_0 0x0
#define ROT_DMACFG0_DSTTRTYPE_1 0x1
#define ROT_DMACFG0_DSTTRTYPE_2 0x2
#define ROT_DMACFG0_DSTTRTYPE_3 0x3

/* reg_rot_t bank */
typedef struct tag_rot {
	reg_rot_ver_t          ver;        /* 0000 */
	reg_rot_ftr_t          ftr;        /* 0004 */
	u32                    res0[2];
	reg_rot_ctl_t          ctl;        /* 0010 */
	reg_rot_cfg_t          cfg;        /* 0014 */
	reg_rot_strmctl_t      strmctl;    /* 0018 */
	reg_rot_sta_t          sta;        /* 001C */
	reg_rot_inten_t        inten;      /* 0020 */
	reg_rot_intpd_t        intpd;      /* 0024 */
	reg_rot_intclr_t       intclr;     /* 0028 */
	u32                    res1[1];
	reg_rot_incfg_t        incfg;      /* 0030 */
	u32                    res2[3];
	reg_rot_insize0_t      insize0;    /* 0040 */
	reg_rot_inadd0_t       inadd0;     /* 0044 */
	reg_rot_inlnstd0_t     inlnstd0;   /* 0048 */
	u32                    res3[1];
	reg_rot_insize1_t      insize1;    /* 0050 */
	reg_rot_inadd1_t       inadd1;     /* 0054 */
	reg_rot_inlnstd1_t     inlnstd1;   /* 0058 */
	u32                    res4[1];
	reg_rot_insize2_t      insize2;    /* 0060 */
	reg_rot_inadd2_t       inadd2;     /* 0064 */
	reg_rot_inlnstd2_t     inlnstd2;   /* 0068 */
	u32                    res5[1];
	reg_rot_outcfg_t       outcfg;     /* 0070 */
	u32                    res6[3];
	reg_rot_outsize0_t     outsize0;   /* 0080 */
	reg_rot_outadd0_t      outadd0;    /* 0084 */
	reg_rot_outlnstd0_t    outlnstd0;  /* 0088 */
	u32                    res7[1];
	reg_rot_outsize1_t     outsize1;   /* 0090 */
	reg_rot_outadd1_t      outadd1;    /* 0094 */
	reg_rot_outlnstd1_t    outlnstd1;  /* 0098 */
	u32                    res8[1];
	reg_rot_outsize2_t     outsize2;   /* 00A0 */
	reg_rot_outadd2_t      outadd2;    /* 00A4 */
	reg_rot_outlnstd2_t    outlnstd2;  /* 00A8 */
	u32                    res9[1];
	reg_rot_dmasctl0_t     dmasctl0;   /* 00B0 */
	reg_rot_dmadctl0_t     dmadctl0;   /* 00B4 */
	reg_rot_dmacfg0_t      dmacfg0;    /* 00B8 */
} reg_rot_t;

#endif /* __CSP_DPU_ROT_REF_H__ */
