/* tcon.h */

#ifndef _CSP_DOSS_TCON_REG_V1_0_H_
#define _CSP_DOSS_TCON_REG_V1_0_H_

#define BASE_TCON                0x01602000

#define VA_TCON_VER                      (0x00000000 + BASE_TCON + VA_TCON)
#define VA_TCON_FTR                      (0x00000004 + BASE_TCON + VA_TCON)
#define VA_TCON_TCON_CTRL                (0x00000010 + BASE_TCON + VA_TCON)
#define VA_TCON_TCON_CFG                 (0x00000014 + BASE_TCON + VA_TCON)
#define VA_TCON_VSYNC                    (0x00000018 + BASE_TCON + VA_TCON)
#define VA_TCON_IO_CFG                   (0x0000001C + BASE_TCON + VA_TCON)
#define VA_TCON_FIFO_CTRL                (0x00000020 + BASE_TCON + VA_TCON)
#define VA_TCON_FIFO_STA                 (0x00000024 + BASE_TCON + VA_TCON)
#define VA_TCON_FIFO_DATA                (0x00000028 + BASE_TCON + VA_TCON)
#define VA_TCON_BG_COLOR                 (0x0000002C + BASE_TCON + VA_TCON)
#define VA_TCON_INT_EN                   (0x00000030 + BASE_TCON + VA_TCON)
#define VA_TCON_INT_PD                   (0x00000034 + BASE_TCON + VA_TCON)
#define VA_TCON_INT_CLR                  (0x00000038 + BASE_TCON + VA_TCON)
#define VA_TCON_LINE_CNT                 (0x0000003C + BASE_TCON + VA_TCON)
#define VA_TCON_GEN_HTIM0                (0x00000040 + BASE_TCON + VA_TCON)
#define VA_TCON_GEN_HTIM1                (0x00000044 + BASE_TCON + VA_TCON)
#define VA_TCON_GEN_VTIM0                (0x00000048 + BASE_TCON + VA_TCON)
#define VA_TCON_GEN_VTIM1                (0x0000004C + BASE_TCON + VA_TCON)
#define VA_TCON_GEN_IN_RES               (0x00000058 + BASE_TCON + VA_TCON)
#define VA_TCON_RGB_IF_CFG               (0x00000080 + BASE_TCON + VA_TCON)
#define VA_TCON_CPU_IF_CFG               (0x00000084 + BASE_TCON + VA_TCON)
#define VA_TCON_CPU_VSYNC_CMD            (0x00000088 + BASE_TCON + VA_TCON)
#define VA_TCON_CPU_HSYNC_CMD            (0x0000008C + BASE_TCON + VA_TCON)
#define VA_TCON_CPU_IF_OP                (0x00000090 + BASE_TCON + VA_TCON)
#define VA_TCON_CPU_WR_DAT               (0x00000098 + BASE_TCON + VA_TCON)
#define VA_TCON_CPU_RD_DAT               (0x0000009C + BASE_TCON + VA_TCON)
#define VA_TCON_LVDS_IF_CFG              (0x000000D0 + BASE_TCON + VA_TCON)
#define VA_TCON_LVDS_ANL_CFG0            (0x000000D4 + BASE_TCON + VA_TCON)
#define VA_TCON_CSC_C0                   (0x00000100 + BASE_TCON + VA_TCON)
#define VA_TCON_CSC_C1                   (0x00000104 + BASE_TCON + VA_TCON)
#define VA_TCON_CSC_C2                   (0x00000108 + BASE_TCON + VA_TCON)
#define VA_TCON_CSC_C3                   (0x0000010C + BASE_TCON + VA_TCON)
#define VA_TCON_CSC_C4                   (0x00000110 + BASE_TCON + VA_TCON)
#define VA_TCON_CSC_C5                   (0x00000114 + BASE_TCON + VA_TCON)
#define VA_TCON_CSC_C6                   (0x00000118 + BASE_TCON + VA_TCON)
#define VA_TCON_CSC_C7                   (0x0000011C + BASE_TCON + VA_TCON)
#define VA_TCON_CSC_C8                   (0x00000120 + BASE_TCON + VA_TCON)
#define VA_TCON_CSC_C9                   (0x00000124 + BASE_TCON + VA_TCON)
#define VA_TCON_CSC_C10                  (0x00000128 + BASE_TCON + VA_TCON)
#define VA_TCON_CSC_C11                  (0x0000012C + BASE_TCON + VA_TCON)
#define VA_TCON_CSC_CLIP0                (0x00000130 + BASE_TCON + VA_TCON)
#define VA_TCON_CSC_CLIP1                (0x00000134 + BASE_TCON + VA_TCON)
#define VA_TCON_CSC_CLIP2                (0x00000138 + BASE_TCON + VA_TCON)
#define VA_TCON_FRC_CFG                  (0x00000140 + BASE_TCON + VA_TCON)
#define VA_TCON_FRC_SEED0                (0x00000144 + BASE_TCON + VA_TCON)
#define VA_TCON_FRC_SEED1                (0x00000148 + BASE_TCON + VA_TCON)
#define VA_TCON_FRC_SEED2                (0x0000014C + BASE_TCON + VA_TCON)
#define VA_TCON_FRC_SEED3                (0x00000150 + BASE_TCON + VA_TCON)
#define VA_TCON_FRC_SEED4                (0x00000154 + BASE_TCON + VA_TCON)
#define VA_TCON_FRC_SEED5                (0x00000158 + BASE_TCON + VA_TCON)
#define VA_TCON_FRC_LUT0                 (0x00000160 + BASE_TCON + VA_TCON)
#define VA_TCON_FRC_LUT1                 (0x00000164 + BASE_TCON + VA_TCON)
#define VA_TCON_FRC_LUT2                 (0x00000168 + BASE_TCON + VA_TCON)
#define VA_TCON_FRC_LUT3                 (0x0000016C + BASE_TCON + VA_TCON)

#define DATA_TCON_VER                    0x00001100
#define DATA_TCON_FTR                    0x0000007B
#define DATA_TCON_TCON_CTRL              0x00000000
#define DATA_TCON_TCON_CFG               0x00000000
#define DATA_TCON_VSYNC                  0x00000000
#define DATA_TCON_IO_CFG                 0x00000000
#define DATA_TCON_FIFO_CTRL              0x00000000
#define DATA_TCON_FIFO_STA               0x000000AA
#define DATA_TCON_FIFO_DATA              0x00000000
#define DATA_TCON_BG_COLOR               0x00000000
#define DATA_TCON_INT_EN                 0x00000000
#define DATA_TCON_INT_PD                 0x00000000
#define DATA_TCON_INT_CLR                0x00000000
#define DATA_TCON_LINE_CNT               0x00000000
#define DATA_TCON_GEN_HTIM0              0x00000000
#define DATA_TCON_GEN_HTIM1              0x00000000
#define DATA_TCON_GEN_VTIM0              0x00000000
#define DATA_TCON_GEN_VTIM1              0x00000000
#define DATA_TCON_GEN_IN_RES             0x00000000
#define DATA_TCON_RGB_IF_CFG             0x00000000
#define DATA_TCON_CPU_IF_CFG             0x00000000
#define DATA_TCON_CPU_VSYNC_CMD          0x0000002C
#define DATA_TCON_CPU_HSYNC_CMD          0x0000003C
#define DATA_TCON_CPU_IF_OP              0x00000000
#define DATA_TCON_CPU_WR_DAT             0x00000000
#define DATA_TCON_CPU_RD_DAT             0x00000000
#define DATA_TCON_LVDS_IF_CFG            0x00000063
#define DATA_TCON_LVDS_ANL_CFG0          0x00000033
#define DATA_TCON_CSC_C0                 0x00000000
#define DATA_TCON_CSC_C1                 0x00000000
#define DATA_TCON_CSC_C2                 0x00000000
#define DATA_TCON_CSC_C3                 0x00000000
#define DATA_TCON_CSC_C4                 0x00000000
#define DATA_TCON_CSC_C5                 0x00000000
#define DATA_TCON_CSC_C6                 0x00000000
#define DATA_TCON_CSC_C7                 0x00000000
#define DATA_TCON_CSC_C8                 0x00000000
#define DATA_TCON_CSC_C9                 0x00000000
#define DATA_TCON_CSC_C10                0x00000000
#define DATA_TCON_CSC_C11                0x00000000
#define DATA_TCON_CSC_CLIP0              0x00FF0000
#define DATA_TCON_CSC_CLIP1              0x00FF0000
#define DATA_TCON_CSC_CLIP2              0x00FF0000
#define DATA_TCON_FRC_CFG                0x00000000
#define DATA_TCON_FRC_SEED0              0x00000000
#define DATA_TCON_FRC_SEED1              0x00000000
#define DATA_TCON_FRC_SEED2              0x00000000
#define DATA_TCON_FRC_SEED3              0x00000000
#define DATA_TCON_FRC_SEED4              0x00000000
#define DATA_TCON_FRC_SEED5              0x00000000
#define DATA_TCON_FRC_LUT0               0x00000000
#define DATA_TCON_FRC_LUT1               0x00000000
#define DATA_TCON_FRC_LUT2               0x00000000
#define DATA_TCON_FRC_LUT3               0x00000000

/* Version Register */
typedef union {
	u32 val;
	struct {
	u32 ver_l:5;    /**/
	u32 rsvd0:3;    /**/
	u32 ver_h:3;    /**/
	u32 rsvd1:1;    /**/
	u32 comp:1;     /**/
	u32 rsvd2:19;   /**/
	} bits;
} reg_tcon_ver_t;

/* Feature Register */
typedef union {
	u32 val;
	struct {
	u32 has_lcd:1;  /**/
	u32 has_tv:1;   /**/
	u32 has_crmp:1; /**/
	u32 has_tf:1;   /**/
	u32 has_gamma:1;/**/
	u32 has_csc:1;  /**/
	u32 has_frc:1;  /**/
	u32 rsvd0:25;   /**/
	} bits;
} reg_tcon_ftr_t;

#define TCON_FTR_HAS_LCD_0      0x0
#define TCON_FTR_HAS_LCD_1      0x1
#define TCON_FTR_HAS_TV_0       0x0
#define TCON_FTR_HAS_TV_1       0x1
#define TCON_FTR_HAS_CRMP_0     0x0
#define TCON_FTR_HAS_CRMP_1     0x1
#define TCON_FTR_HAS_TF_0       0x0
#define TCON_FTR_HAS_TF_1       0x1
#define TCON_FTR_HAS_GAMMA_0    0x0
#define TCON_FTR_HAS_GAMMA_1    0x1
#define TCON_FTR_HAS_CSC_0      0x0
#define TCON_FTR_HAS_CSC_1      0x1
#define TCON_FTR_HAS_FRC_0      0x0
#define TCON_FTR_HAS_FRC_1      0x1

/* TCON Control Register */
typedef union {
	u32 val;
	struct {
	u32 tcon_en:1;  /**/
	u32 tgen_en:1;  /**/
	u32 rsvd0:2;    /**/
	u32 gamma_en:1; /**/
	u32 csc_en:1;   /**/
	u32 frc_en:1;   /**/
	u32 crmp_en:1;  /**/
	u32 fifo_en:1;  /**/
	u32 rsvd1:22;   /**/
	u32 mipi_en:1;  /**/
	} bits;
} reg_tcon_tcon_ctrl_t;

#define TCON_TCON_CTRL_TCON_EN_0        0x0
#define TCON_TCON_CTRL_TCON_EN_1        0x1
#define TCON_TCON_CTRL_TGEN_EN_0        0x0
#define TCON_TCON_CTRL_TGEN_EN_1        0x1
#define TCON_TCON_CTRL_GAMMA_EN_0       0x0
#define TCON_TCON_CTRL_GAMMA_EN_1       0x1
#define TCON_TCON_CTRL_CSC_EN_0         0x0
#define TCON_TCON_CTRL_CSC_EN_1         0x1
#define TCON_TCON_CTRL_FRC_EN_0         0x0
#define TCON_TCON_CTRL_FRC_EN_1         0x1
#define TCON_TCON_CTRL_CRMP_EN_0        0x0
#define TCON_TCON_CTRL_CRMP_EN_1        0x1
#define TCON_TCON_CTRL_FIFO_EN_0        0x0
#define TCON_TCON_CTRL_FIFO_EN_1        0x1
#define TCON_TCON_CTRL_MIPI_EN_0        0x0
#define TCON_TCON_CTRL_MIPI_EN_1        0x1

/* TCON Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 src_sel:3;  /**/
	u32 rsvd0:1;    /**/
	u32 path_sel:1; /**/
	u32 rsvd1:2;    /**/
	u32 tve_std:1;  /**/
	u32 out_sel:6;  /**/
	u32 rsvd2:2;    /**/
	u32 out_mode:1; /**/
	u32 fifo_mode:2;/**/
	u32 rsvd3:1;    /**/
	u32 rb_swap:1;  /**/
	u32 rsvd4:1;    /**/
	u32 ds_mode:1;  /**/
	u32 rsvd5:1;    /**/
	u32 sram_acs:1; /**/
	u32 rsvd6:3;    /**/
	u32 trig_src:2; /**/
	u32 trig_mode:1;/**/
	u32 force_itl:1;/**/
	} bits;
} reg_tcon_tcon_cfg_t;

#define TCON_TCON_CFG_SRC_SEL_0         0x0
#define TCON_TCON_CFG_SRC_SEL_1         0x1
#define TCON_TCON_CFG_SRC_SEL_2         0x2
#define TCON_TCON_CFG_SRC_SEL_3         0x3
#define TCON_TCON_CFG_SRC_SEL_4         0x4
#define TCON_TCON_CFG_SRC_SEL_5         0x5
#define TCON_TCON_CFG_SRC_SEL_6         0x6
#define TCON_TCON_CFG_SRC_SEL_7         0x7
#define TCON_TCON_CFG_PATH_SEL_0        0x0
#define TCON_TCON_CFG_PATH_SEL_1        0x1
#define TCON_TCON_CFG_TVE_STD_0         0x0
#define TCON_TCON_CFG_TVE_STD_1         0x1
#define TCON_TCON_CFG_OUT_SEL_0         0x0
#define TCON_TCON_CFG_OUT_SEL_4         0x4
#define TCON_TCON_CFG_OUT_SEL_5         0x5
#define TCON_TCON_CFG_OUT_SEL_6         0x6
#define TCON_TCON_CFG_OUT_SEL_10        0x10
#define TCON_TCON_CFG_OUT_SEL_14        0x14
#define TCON_TCON_CFG_OUT_SEL_15        0x15
#define TCON_TCON_CFG_OUT_SEL_18        0x18
#define TCON_TCON_CFG_OUT_SEL_1C        0x1C
#define TCON_TCON_CFG_OUT_SEL_1D        0x1D
#define TCON_TCON_CFG_OUT_SEL_21        0x21
#define TCON_TCON_CFG_OUT_SEL_22        0x22
#define TCON_TCON_CFG_OUT_SEL_23        0x23
#define TCON_TCON_CFG_OUT_MODE_0        0x0
#define TCON_TCON_CFG_OUT_MODE_1        0x1
#define TCON_TCON_CFG_FIFO_MODE_0       0x0
#define TCON_TCON_CFG_FIFO_MODE_1       0x1
#define TCON_TCON_CFG_FIFO_MODE_2       0x2
#define TCON_TCON_CFG_FIFO_MODE_3       0x3
#define TCON_TCON_CFG_RB_SWAP_0         0x0
#define TCON_TCON_CFG_RB_SWAP_1         0x1
#define TCON_TCON_CFG_DS_MODE_0         0x0
#define TCON_TCON_CFG_DS_MODE_1         0x1
#define TCON_TCON_CFG_SRAM_ACS_0        0x0
#define TCON_TCON_CFG_SRAM_ACS_1        0x1
#define TCON_TCON_CFG_TRIG_SRC_0        0x0
#define TCON_TCON_CFG_TRIG_SRC_1        0x1
#define TCON_TCON_CFG_TRIG_SRC_2        0x2
#define TCON_TCON_CFG_TRIG_SRC_3        0x3
#define TCON_TCON_CFG_TRIG_MODE_0       0x0
#define TCON_TCON_CFG_TRIG_MODE_1       0x1
#define TCON_TCON_CFG_FORCE_ITL_0       0x0
#define TCON_TCON_CFG_FORCE_ITL_1       0x1

/* Vertical Synchronization Register */
typedef union {
	u32 val;
	struct {
	u32 dc_sync_dly:12;/**/
	u32 rsvd0:4;    /**/
	u32 vbi_trig:2; /**/
	u32 rsvd1:14;   /**/
	} bits;
} reg_tcon_vsync_t;

#define TCON_VSYNC_VBI_TRIG_0           0x0
#define TCON_VSYNC_VBI_TRIG_1           0x1
#define TCON_VSYNC_VBI_TRIG_2           0x2
#define TCON_VSYNC_VBI_TRIG_3           0x3

/* IO Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 dclk_wr_inv:1;/**/
	u32 de_rd_inv:1;/**/
	u32 hs_rs_inv:1;/**/
	u32 vs_cs_inv:1;/**/
	u32 hb_inv:1;   /**/
	u32 vb_inv:1;   /**/
	u32 field_inv:1;/**/
	u32 te_inv:1;   /**/
	u32 clk_dly:6;  /**/
	u32 rsvd0:18;   /**/
	} bits;
} reg_tcon_io_cfg_t;

#define TCON_IO_CFG_DCLK_WR_INV_0       0x0
#define TCON_IO_CFG_DCLK_WR_INV_1       0x1
#define TCON_IO_CFG_DE_RD_INV_0         0x0
#define TCON_IO_CFG_DE_RD_INV_1         0x1
#define TCON_IO_CFG_HS_RS_INV_0         0x0
#define TCON_IO_CFG_HS_RS_INV_1         0x1
#define TCON_IO_CFG_VS_CS_INV_0         0x0
#define TCON_IO_CFG_VS_CS_INV_1         0x1
#define TCON_IO_CFG_HB_INV_0            0x0
#define TCON_IO_CFG_HB_INV_1            0x1
#define TCON_IO_CFG_VB_INV_0            0x0
#define TCON_IO_CFG_VB_INV_1            0x1
#define TCON_IO_CFG_FIELD_INV_0         0x0
#define TCON_IO_CFG_FIELD_INV_1         0x1
#define TCON_IO_CFG_TE_INV_0            0x0
#define TCON_IO_CFG_TE_INV_1            0x1

/* FIFO Control Register */
typedef union {
	u32 val;
	struct {
	u32 dma_en:1;   /**/
	u32 rsvd0:3;    /**/
	u32 drq_trig:2; /**/
	u32 rsvd1:21;   /**/
	u32 lf_clr:1;   /**/
	u32 rsvd2:3;    /**/
	u32 rst:1;      /**/
	} bits;
} reg_tcon_fifo_ctrl_t;

#define TCON_FIFO_CTRL_DMA_EN_0         0x0
#define TCON_FIFO_CTRL_DMA_EN_1         0x1
#define TCON_FIFO_CTRL_DRQ_TRIG_0       0x0
#define TCON_FIFO_CTRL_DRQ_TRIG_1       0x1
#define TCON_FIFO_CTRL_DRQ_TRIG_2       0x2
#define TCON_FIFO_CTRL_DRQ_TRIG_3       0x3

/* FIFO Status Register */
typedef union {
	u32 val;
	struct {
	u32 pix_full:1; /**/
	u32 pix_empty:1;/**/
	u32 src_full:1; /**/
	u32 src_empty:1;/**/
	u32 last_full:1;/**/
	u32 last_empty:1;/**/
	u32 lb_full:1;  /**/
	u32 lb_empty:1; /**/
	u32 rsvd0:8;    /**/
	u32 lack_cnt:12;/**/
	u32 rsvd1:3;    /**/
	u32 lack_flag:1;/**/
	} bits;
} reg_tcon_fifo_sta_t;

/* FIFO Data Register */
typedef union {
	u32 val;
	struct {
	u32 dat:24;     /**/
	u32 rsvd0:8;    /**/
	} bits;
} reg_tcon_fifo_data_t;

/* Back Ground Color Register */
typedef union {
	u32 val;
	struct {
	u32 dat:24;     /**/
	u32 rsvd0:8;    /**/
	} bits;
} reg_tcon_bg_color_t;

/* Interrupt Enable Register */
typedef union {
	u32 val;
	struct {
	u32 line_trig:1;/**/
	u32 vbi:1;      /**/
	u32 rsvd0:6;    /**/
	u32 trig_finish:1;/**/
	u32 te_trig:1;  /**/
	u32 ext_trig:1; /**/
	u32 rsvd1:21;   /**/
	} bits;
} reg_tcon_int_en_t;

#define TCON_INT_EN_LINE_TRIG_0         0x0
#define TCON_INT_EN_LINE_TRIG_1         0x1
#define TCON_INT_EN_VBI_0               0x0
#define TCON_INT_EN_VBI_1               0x1
#define TCON_INT_EN_TRIG_FINISH_0       0x0
#define TCON_INT_EN_TRIG_FINISH_1       0x1
#define TCON_INT_EN_TE_TRIG_0           0x0
#define TCON_INT_EN_TE_TRIG_1           0x1
#define TCON_INT_EN_EXT_TRIG_0          0x0
#define TCON_INT_EN_EXT_TRIG_1          0x1

/* Interrupt Pending Register */
typedef union {
	u32 val;
	struct {
	u32 line_trig:1;/**/
	u32 vbi:1;      /**/
	u32 rsvd0:6;    /**/
	u32 trig_finish:1;/**/
	u32 te_trig:1;  /**/
	u32 ext_trig:1; /**/
	u32 rsvd1:21;   /**/
	} bits;
} reg_tcon_int_pd_t;

/* Interrupt Clear Register */
typedef union {
	u32 val;
	struct {
	u32 line_trig:1;/**/
	u32 vbi:1;      /**/
	u32 rsvd0:6;    /**/
	u32 trig_finish:1;/**/
	u32 te_trig:1;  /**/
	u32 ext_trig:1; /**/
	u32 rsvd1:21;   /**/
	} bits;
} reg_tcon_int_clr_t;

#define TCON_INT_CLR_LINE_TRIG_0        0x0
#define TCON_INT_CLR_LINE_TRIG_1        0x1
#define TCON_INT_CLR_VBI_0              0x0
#define TCON_INT_CLR_VBI_1              0x1
#define TCON_INT_CLR_TRIG_FINISH_0      0x0
#define TCON_INT_CLR_TRIG_FINISH_1      0x1
#define TCON_INT_CLR_TE_TRIG_0          0x0
#define TCON_INT_CLR_TE_TRIG_1          0x1
#define TCON_INT_CLR_EXT_TRIG_0         0x0
#define TCON_INT_CLR_EXT_TRIG_1         0x1

/* Line Counter Register */
typedef union {
	u32 val;
	struct {
	u32 trig:13;    /**/
	u32 rsvd0:3;    /**/
	u32 crnt:13;    /**/
	u32 rsvd1:3;    /**/
	} bits;
} reg_tcon_line_cnt_t;

/* General Horizontal Timing 0 Register */
typedef union {
	u32 val;
	struct {
	u32 ht:13;      /**/
	u32 rsvd0:3;    /**/
	u32 hact:12;    /**/
	u32 rsvd1:4;    /**/
	} bits;
} reg_tcon_gen_htim0_t;

/* General Horizontal Timing 1 Register */
typedef union {
	u32 val;
	struct {
	u32 hfp:12;     /**/
	u32 rsvd0:4;    /**/
	u32 hsw:10;     /**/
	u32 rsvd1:6;    /**/
	} bits;
} reg_tcon_gen_htim1_t;

/* General Vertical Timing 0 Register */
typedef union {
	u32 val;
	struct {
	u32 vt:13;      /**/
	u32 rsvd0:3;    /**/
	u32 vact:12;    /**/
	u32 rsvd1:4;    /**/
	} bits;
} reg_tcon_gen_vtim0_t;

/* General Vertical Timing 1 Register */
typedef union {
	u32 val;
	struct {
	u32 vfp:12;     /**/
	u32 rsvd0:4;    /**/
	u32 vsw:10;     /**/
	u32 rsvd1:6;    /**/
	} bits;
} reg_tcon_gen_vtim1_t;

/* General Input Resolution Register */
typedef union {
	u32 val;
	struct {
	u32 hsize:12;   /**/
	u32 rsvd0:4;    /**/
	u32 vsize:10;   /**/
	u32 rsvd1:6;    /**/
	} bits;
} reg_tcon_gen_in_res_t;

/* RGB Interface Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 ol_seq:2;   /**/
	u32 el_seq:2;   /**/
	u32 rsvd0:28;   /**/
	} bits;
} reg_tcon_rgb_if_cfg_t;

#define TCON_RGB_IF_CFG_OL_SEQ_0        0x0
#define TCON_RGB_IF_CFG_OL_SEQ_1        0x1
#define TCON_RGB_IF_CFG_OL_SEQ_2        0x2
#define TCON_RGB_IF_CFG_EL_SEQ_0        0x0
#define TCON_RGB_IF_CFG_EL_SEQ_1        0x1
#define TCON_RGB_IF_CFG_EL_SEQ_2        0x2

/* CPU Interface Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:1;    /**/
	u32 vsync_cmd:1;/**/
	u32 hsync_cmd:1;/**/
	u32 rsvd1:1;    /**/
	u32 mode:1;     /**/
	u32 rsvd2:27;   /**/
	} bits;
} reg_tcon_cpu_if_cfg_t;

#define TCON_CPU_IF_CFG_VSYNC_CMD_0     0x0
#define TCON_CPU_IF_CFG_VSYNC_CMD_1     0x1
#define TCON_CPU_IF_CFG_HSYNC_CMD_0     0x0
#define TCON_CPU_IF_CFG_HSYNC_CMD_1     0x1
#define TCON_CPU_IF_CFG_MODE_0          0x0
#define TCON_CPU_IF_CFG_MODE_1          0x1

/* CPU Vertical Synchronized Command Register */
typedef union {
	u32 val;
	struct {
	u32 dat:18;     /**/
	u32 rsvd0:14;   /**/
	} bits;
} reg_tcon_cpu_vsync_cmd_t;

/* CPU Horizontal Synchronized Command Register */
typedef union {
	u32 val;
	struct {
	u32 dat:18;     /**/
	u32 rsvd0:14;   /**/
	} bits;
} reg_tcon_cpu_hsync_cmd_t;

/* CPU Interface Operation Register */
typedef union {
	u32 val;
	struct {
	u32 wr_op:1;    /**/
	u32 rsvd0:3;    /**/
	u32 rd_op:1;    /**/
	u32 rsvd1:3;    /**/
	u32 rs:1;       /**/
	u32 rsvd2:3;    /**/
	u32 s_trig:1;   /**/
	u32 rsvd3:3;    /**/
	u32 cm:1;       /**/
	u32 shut:1;     /**/
	u32 rsvd4:14;   /**/
	} bits;
} reg_tcon_cpu_if_op_t;

#define TCON_CPU_IF_OP_RS_0     0x0
#define TCON_CPU_IF_OP_RS_1     0x1

/* CPU Write Data Register */
typedef union {
	u32 val;
	struct {
	u32 dat:18;     /**/
	u32 rsvd0:14;   /**/
	} bits;
} reg_tcon_cpu_wr_dat_t;

/* CPU Read Data Register */
typedef union {
	u32 val;
	struct {
	u32 dat:18;     /**/
	u32 rsvd0:14;   /**/
	} bits;
} reg_tcon_cpu_rd_dat_t;

/* LVDS Interface Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 clk_pat:7;  /**/
	u32 rsvd0:1;    /**/
	u32 mode:1;     /**/
	u32 rsvd1:3;    /**/
	u32 polar:1;    /**/
	u32 rsvd2:3;    /**/
	u32 clk_adj:1;  /**/
	u32 rsvd3:7;    /**/
	u32 lsb_first:1;/**/
	u32 lane_rvs:1; /**/
	u32 dbg_pat:1;  /**/
	u32 dbg_en:1;   /**/
	u32 rsvd4:3;    /**/
	u32 en:1;       /**/
	} bits;
} reg_tcon_lvds_if_cfg_t;

#define TCON_LVDS_IF_CFG_MODE_0         0x0
#define TCON_LVDS_IF_CFG_MODE_1         0x1
#define TCON_LVDS_IF_CFG_POLAR_0        0x0
#define TCON_LVDS_IF_CFG_POLAR_1        0x1
#define TCON_LVDS_IF_CFG_CLK_ADJ_0      0x0
#define TCON_LVDS_IF_CFG_CLK_ADJ_1      0x1
#define TCON_LVDS_IF_CFG_LSB_FIRST_0    0x0
#define TCON_LVDS_IF_CFG_LSB_FIRST_1    0x1
#define TCON_LVDS_IF_CFG_LANE_RVS_0     0x0
#define TCON_LVDS_IF_CFG_LANE_RVS_1     0x1
#define TCON_LVDS_IF_CFG_DBG_PAT_0      0x0
#define TCON_LVDS_IF_CFG_DBG_PAT_1      0x1
#define TCON_LVDS_IF_CFG_DBG_EN_0       0x0
#define TCON_LVDS_IF_CFG_DBG_EN_1       0x1
#define TCON_LVDS_IF_CFG_EN_0           0x0
#define TCON_LVDS_IF_CFG_EN_1           0x1

/* LVDS Analog Configuration 0 Register */
typedef union {
	u32 val;
	struct {
	u32 diff:3;     /**/
	u32 rsvd0:1;    /**/
	u32 com:3;      /**/
	u32 rsvd1:1;    /**/
	u32 d0_pol:1;   /**/
	u32 d1_pol:1;   /**/
	u32 d2_pol:1;   /**/
	u32 d3_pol:1;   /**/
	u32 ck_pol:1;   /**/
	u32 rsvd2:3;    /**/
	u32 d0_en:1;    /**/
	u32 d1_en:1;    /**/
	u32 d2_en:1;    /**/
	u32 d3_en:1;    /**/
	u32 ck_en:1;    /**/
	u32 test_sel:3; /**/
	u32 bias_d0:1;  /**/
	u32 bias_d1:1;  /**/
	u32 bias_d2:1;  /**/
	u32 bias_d3:1;  /**/
	u32 bias_ck:1;  /**/
	u32 rsvd3:1;    /**/
	u32 bias_sel:1; /**/
	u32 tm_en:1;    /**/
	} bits;
} reg_tcon_lvds_anl_cfg0_t;

#define TCON_LVDS_ANL_CFG0_DIFF_0       0x0
#define TCON_LVDS_ANL_CFG0_DIFF_1       0x1
#define TCON_LVDS_ANL_CFG0_DIFF_2       0x2
#define TCON_LVDS_ANL_CFG0_DIFF_3       0x3
#define TCON_LVDS_ANL_CFG0_DIFF_4       0x4
#define TCON_LVDS_ANL_CFG0_DIFF_5       0x5
#define TCON_LVDS_ANL_CFG0_DIFF_6       0x6
#define TCON_LVDS_ANL_CFG0_DIFF_7       0x7
#define TCON_LVDS_ANL_CFG0_COM_0        0x0
#define TCON_LVDS_ANL_CFG0_COM_1        0x1
#define TCON_LVDS_ANL_CFG0_COM_2        0x2
#define TCON_LVDS_ANL_CFG0_COM_3        0x3
#define TCON_LVDS_ANL_CFG0_COM_4        0x4
#define TCON_LVDS_ANL_CFG0_COM_5        0x5
#define TCON_LVDS_ANL_CFG0_COM_6        0x6
#define TCON_LVDS_ANL_CFG0_COM_7        0x7
#define TCON_LVDS_ANL_CFG0_D0_POL_0     0x0
#define TCON_LVDS_ANL_CFG0_D0_POL_1     0x1
#define TCON_LVDS_ANL_CFG0_D1_POL_0     0x0
#define TCON_LVDS_ANL_CFG0_D1_POL_1     0x1
#define TCON_LVDS_ANL_CFG0_D2_POL_0     0x0
#define TCON_LVDS_ANL_CFG0_D2_POL_1     0x1
#define TCON_LVDS_ANL_CFG0_D3_POL_0     0x0
#define TCON_LVDS_ANL_CFG0_D3_POL_1     0x1
#define TCON_LVDS_ANL_CFG0_CK_POL_0     0x0
#define TCON_LVDS_ANL_CFG0_CK_POL_1     0x1
#define TCON_LVDS_ANL_CFG0_D0_EN_0      0x0
#define TCON_LVDS_ANL_CFG0_D0_EN_1      0x1
#define TCON_LVDS_ANL_CFG0_D1_EN_0      0x0
#define TCON_LVDS_ANL_CFG0_D1_EN_1      0x1
#define TCON_LVDS_ANL_CFG0_D2_EN_0      0x0
#define TCON_LVDS_ANL_CFG0_D2_EN_1      0x1
#define TCON_LVDS_ANL_CFG0_D3_EN_0      0x0
#define TCON_LVDS_ANL_CFG0_D3_EN_1      0x1
#define TCON_LVDS_ANL_CFG0_CK_EN_0      0x0
#define TCON_LVDS_ANL_CFG0_CK_EN_1      0x1
#define TCON_LVDS_ANL_CFG0_TEST_SEL_0   0x0
#define TCON_LVDS_ANL_CFG0_TEST_SEL_1   0x1
#define TCON_LVDS_ANL_CFG0_TEST_SEL_2   0x2
#define TCON_LVDS_ANL_CFG0_TEST_SEL_3   0x3
#define TCON_LVDS_ANL_CFG0_TEST_SEL_4   0x4
#define TCON_LVDS_ANL_CFG0_TEST_SEL_5   0x5
#define TCON_LVDS_ANL_CFG0_TEST_SEL_6   0x6
#define TCON_LVDS_ANL_CFG0_TEST_SEL_7   0x7
#define TCON_LVDS_ANL_CFG0_BIAS_D0_0    0x0
#define TCON_LVDS_ANL_CFG0_BIAS_D0_1    0x1
#define TCON_LVDS_ANL_CFG0_BIAS_D1_0    0x0
#define TCON_LVDS_ANL_CFG0_BIAS_D1_1    0x1
#define TCON_LVDS_ANL_CFG0_BIAS_D2_0    0x0
#define TCON_LVDS_ANL_CFG0_BIAS_D2_1    0x1
#define TCON_LVDS_ANL_CFG0_BIAS_D3_0    0x0
#define TCON_LVDS_ANL_CFG0_BIAS_D3_1    0x1
#define TCON_LVDS_ANL_CFG0_BIAS_CK_0    0x0
#define TCON_LVDS_ANL_CFG0_BIAS_CK_1    0x1
#define TCON_LVDS_ANL_CFG0_BIAS_SEL_0   0x0
#define TCON_LVDS_ANL_CFG0_BIAS_SEL_1   0x1
#define TCON_LVDS_ANL_CFG0_TM_EN_0      0x0
#define TCON_LVDS_ANL_CFG0_TM_EN_1      0x1

/* CSC Coefficient 0 Register */
typedef union {
	u32 val;
	struct {
	u32 coef:14;    /**/
	u32 rsvd0:18;   /**/
	} bits;
} reg_tcon_csc_c0_t;

/* CSC Coefficient 1 Register */
typedef union {
	u32 val;
	struct {
	u32 coef:14;    /**/
	u32 rsvd0:18;   /**/
	} bits;
} reg_tcon_csc_c1_t;

/* CSC Coefficient 2 Register */
typedef union {
	u32 val;
	struct {
	u32 coef:14;    /**/
	u32 rsvd0:18;   /**/
	} bits;
} reg_tcon_csc_c2_t;

/* CSC Coefficient 3 Register */
typedef union {
	u32 val;
	struct {
	u32 coef:14;    /**/
	u32 rsvd0:18;   /**/
	} bits;
} reg_tcon_csc_c3_t;

/* CSC Coefficient 4 Register */
typedef union {
	u32 val;
	struct {
	u32 coef:14;    /**/
	u32 rsvd0:18;   /**/
	} bits;
} reg_tcon_csc_c4_t;

/* CSC Coefficient 5 Register */
typedef union {
	u32 val;
	struct {
	u32 coef:14;    /**/
	u32 rsvd0:18;   /**/
	} bits;
} reg_tcon_csc_c5_t;

/* CSC Coefficient 6 Register */
typedef union {
	u32 val;
	struct {
	u32 coef:14;    /**/
	u32 rsvd0:18;   /**/
	} bits;
} reg_tcon_csc_c6_t;

/* CSC Coefficient 7 Register */
typedef union {
	u32 val;
	struct {
	u32 coef:14;    /**/
	u32 rsvd0:18;   /**/
	} bits;
} reg_tcon_csc_c7_t;

/* CSC Coefficient 8 Register */
typedef union {
	u32 val;
	struct {
	u32 coef:14;    /**/
	u32 rsvd0:18;   /**/
	} bits;
} reg_tcon_csc_c8_t;

/* CSC Coefficient 9 Register */
typedef union {
	u32 val;
	struct {
	u32 coef:14;    /**/
	u32 rsvd0:18;   /**/
	} bits;
} reg_tcon_csc_c9_t;

/* CSC Coefficient 10 Register */
typedef union {
	u32 val;
	struct {
	u32 coef:14;    /**/
	u32 rsvd0:18;   /**/
	} bits;
} reg_tcon_csc_c10_t;

/* CSC Coefficient 11 Register */
typedef union {
	u32 val;
	struct {
	u32 coef:14;    /**/
	u32 rsvd0:18;   /**/
	} bits;
} reg_tcon_csc_c11_t;

/* CSC CLIP 0 Register */
typedef union {
	u32 val;
	struct {
	u32 min:8;      /**/
	u32 rsvd0:8;    /**/
	u32 max:8;      /**/
	u32 rsvd1:8;    /**/
	} bits;
} reg_tcon_csc_clip0_t;

/* CSC CLIP 1 Register */
typedef union {
	u32 val;
	struct {
	u32 min:8;      /**/
	u32 rsvd0:8;    /**/
	u32 max:8;      /**/
	u32 rsvd1:8;    /**/
	} bits;
} reg_tcon_csc_clip1_t;

/* CSC CLIP 2 Register */
typedef union {
	u32 val;
	struct {
	u32 min:8;      /**/
	u32 rsvd0:8;    /**/
	u32 max:8;      /**/
	u32 rsvd1:8;    /**/
	} bits;
} reg_tcon_csc_clip2_t;

/* FRC Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 r_mode:1;   /**/
	u32 g_mode:1;   /**/
	u32 b_mode:1;   /**/
	u32 rsvd0:1;    /**/
	u32 tm:2;       /**/
	u32 rsvd1:26;   /**/
	} bits;
} reg_tcon_frc_cfg_t;

#define TCON_FRC_CFG_R_MODE_0   0x0
#define TCON_FRC_CFG_R_MODE_1   0x1
#define TCON_FRC_CFG_G_MODE_0   0x0
#define TCON_FRC_CFG_G_MODE_1   0x1
#define TCON_FRC_CFG_B_MODE_0   0x0
#define TCON_FRC_CFG_B_MODE_1   0x1
#define TCON_FRC_CFG_TM_0       0x0
#define TCON_FRC_CFG_TM_1       0x1
#define TCON_FRC_CFG_TM_2       0x2
#define TCON_FRC_CFG_TM_3       0x3

/* FRC Seed 0 Register */
typedef union {
	u32 val;
	struct {
	u32 r_pix:25;   /**/
	u32 rsvd0:7;    /**/
	} bits;
} reg_tcon_frc_seed0_t;

/* FRC Seed 1 Register */
typedef union {
	u32 val;
	struct {
	u32 g_pix:25;   /**/
	u32 rsvd0:7;    /**/
	} bits;
} reg_tcon_frc_seed1_t;

/* FRC Seed 2 Register */
typedef union {
	u32 val;
	struct {
	u32 b_pix:25;   /**/
	u32 rsvd0:7;    /**/
	} bits;
} reg_tcon_frc_seed2_t;

/* FRC Seed 3 Register */
typedef union {
	u32 val;
	struct {
	u32 r_line:12;  /**/
	u32 rsvd0:20;   /**/
	} bits;
} reg_tcon_frc_seed3_t;

/* FRC Seed 4 Register */
typedef union {
	u32 val;
	struct {
	u32 g_line:12;  /**/
	u32 rsvd0:20;   /**/
	} bits;
} reg_tcon_frc_seed4_t;

/* FRC Seed 5 Register */
typedef union {
	u32 val;
	struct {
	u32 b_line:12;  /**/
	u32 rsvd0:20;   /**/
	} bits;
} reg_tcon_frc_seed5_t;

/* FRC LUT 0 Register */
typedef union {
	u32 val;
	struct {
	u32 lsb0:16;    /**/
	u32 lsb1:16;    /**/
	} bits;
} reg_tcon_frc_lut0_t;

/* FRC LUT 1 Register */
typedef union {
	u32 val;
	struct {
	u32 lsb2:16;    /**/
	u32 lsb3:16;    /**/
	} bits;
} reg_tcon_frc_lut1_t;

/* FRC LUT 2 Register */
typedef union {
	u32 val;
	struct {
	u32 lsb4:16;    /**/
	u32 lsb5:16;    /**/
	} bits;
} reg_tcon_frc_lut2_t;

/* FRC LUT 3 Register */
typedef union {
	u32 val;
	struct {
	u32 lsb6:16;    /**/
	u32 lsb7:16;    /**/
	} bits;
} reg_tcon_frc_lut3_t;

/* reg_tcon_t bank */
typedef struct tag_tcon {
	reg_tcon_ver_t             ver;            /* 0000 */
	reg_tcon_ftr_t             ftr;            /* 0004 */
	u32                        res0[2];
	reg_tcon_tcon_ctrl_t       tcon_ctrl;      /* 0010 */
	reg_tcon_tcon_cfg_t        tcon_cfg;       /* 0014 */
	reg_tcon_vsync_t           vsync;          /* 0018 */
	reg_tcon_io_cfg_t          io_cfg;         /* 001C */
	reg_tcon_fifo_ctrl_t       fifo_ctrl;      /* 0020 */
	reg_tcon_fifo_sta_t        fifo_sta;       /* 0024 */
	reg_tcon_fifo_data_t       fifo_data;      /* 0028 */
	reg_tcon_bg_color_t        bg_color;       /* 002C */
	reg_tcon_int_en_t          int_en;         /* 0030 */
	reg_tcon_int_pd_t          int_pd;         /* 0034 */
	reg_tcon_int_clr_t         int_clr;        /* 0038 */
	reg_tcon_line_cnt_t        line_cnt;       /* 003C */
	reg_tcon_gen_htim0_t       gen_htim0;      /* 0040 */
	reg_tcon_gen_htim1_t       gen_htim1;      /* 0044 */
	reg_tcon_gen_vtim0_t       gen_vtim0;      /* 0048 */
	reg_tcon_gen_vtim1_t       gen_vtim1;      /* 004C */
	u32                        res1[2];
	reg_tcon_gen_in_res_t      gen_in_res;     /* 0058 */
	u32                        res2[9];
	reg_tcon_rgb_if_cfg_t      rgb_if_cfg;     /* 0080 */
	reg_tcon_cpu_if_cfg_t      cpu_if_cfg;     /* 0084 */
	reg_tcon_cpu_vsync_cmd_t   cpu_vsync_cmd;  /* 0088 */
	reg_tcon_cpu_hsync_cmd_t   cpu_hsync_cmd;  /* 008C */
	reg_tcon_cpu_if_op_t       cpu_if_op;      /* 0090 */
	u32                        res3[1];
	reg_tcon_cpu_wr_dat_t      cpu_wr_dat;     /* 0098 */
	reg_tcon_cpu_rd_dat_t      cpu_rd_dat;     /* 009C */
	u32                        res4[12];
	reg_tcon_lvds_if_cfg_t     lvds_if_cfg;    /* 00D0 */
	reg_tcon_lvds_anl_cfg0_t   lvds_anl_cfg0;  /* 00D4 */
	u32                        res5[10];
	reg_tcon_csc_c0_t          csc_c0;         /* 0100 */
	reg_tcon_csc_c1_t          csc_c1;         /* 0104 */
	reg_tcon_csc_c2_t          csc_c2;         /* 0108 */
	reg_tcon_csc_c3_t          csc_c3;         /* 010C */
	reg_tcon_csc_c4_t          csc_c4;         /* 0110 */
	reg_tcon_csc_c5_t          csc_c5;         /* 0114 */
	reg_tcon_csc_c6_t          csc_c6;         /* 0118 */
	reg_tcon_csc_c7_t          csc_c7;         /* 011C */
	reg_tcon_csc_c8_t          csc_c8;         /* 0120 */
	reg_tcon_csc_c9_t          csc_c9;         /* 0124 */
	reg_tcon_csc_c10_t         csc_c10;        /* 0128 */
	reg_tcon_csc_c11_t         csc_c11;        /* 012C */
	reg_tcon_csc_clip0_t       csc_clip0;      /* 0130 */
	reg_tcon_csc_clip1_t       csc_clip1;      /* 0134 */
	reg_tcon_csc_clip2_t       csc_clip2;      /* 0138 */
	u32                        res6[1];
	reg_tcon_frc_cfg_t         frc_cfg;        /* 0140 */
	reg_tcon_frc_seed0_t       frc_seed0;      /* 0144 */
	reg_tcon_frc_seed1_t       frc_seed1;      /* 0148 */
	reg_tcon_frc_seed2_t       frc_seed2;      /* 014C */
	reg_tcon_frc_seed3_t       frc_seed3;      /* 0150 */
	reg_tcon_frc_seed4_t       frc_seed4;      /* 0154 */
	reg_tcon_frc_seed5_t       frc_seed5;      /* 0158 */
	u32                        res7[1];
	reg_tcon_frc_lut0_t        frc_lut0;       /* 0160 */
	reg_tcon_frc_lut1_t        frc_lut1;       /* 0164 */
	reg_tcon_frc_lut2_t        frc_lut2;       /* 0168 */
	reg_tcon_frc_lut3_t        frc_lut3;       /* 016C */
} reg_tcon_t;

#endif /* _CSP_DOSS_TCON_REG_V1_0_H_ */
