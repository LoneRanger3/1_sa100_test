/* dpu.h */

#ifndef ___DPU___H___
#define ___DPU___H___

#define BASE_DPU                0x01500000

#define VA_DPU_VER                   (0x00000000 + BASE_DPU + VA_DPU)
#define VA_DPU_FTR                   (0x00000004 + BASE_DPU + VA_DPU)
#define VA_DPU_CTL                   (0x00000010 + BASE_DPU + VA_DPU)
#define VA_DPU_CLK_SRC               (0x00000014 + BASE_DPU + VA_DPU)
#define VA_DPU_CLK_DIV               (0x00000018 + BASE_DPU + VA_DPU)
#define VA_DPU_CLK_GATING            (0x0000001C + BASE_DPU + VA_DPU)
#define VA_DPU_ROUTINE               (0x00000020 + BASE_DPU + VA_DPU)
#define VA_DPU_SCLK_SEL              (0x00000024 + BASE_DPU + VA_DPU)
#define VA_DPU_AXI_PRI               (0x00000030 + BASE_DPU + VA_DPU)
#define VA_DPU_ADPLL0_CTL            (0x00000040 + BASE_DPU + VA_DPU)
#define VA_DPU_ADPLL0_TUNE0          (0x00000044 + BASE_DPU + VA_DPU)
#define VA_DPU_ADPLL0_TUNE1          (0x00000048 + BASE_DPU + VA_DPU)
#define VA_DPU_ADPLL0_STAT           (0x0000004C + BASE_DPU + VA_DPU)
#define VA_DPU_ADPLL1_CTL            (0x00000050 + BASE_DPU + VA_DPU)
#define VA_DPU_ADPLL1_TUNE0          (0x00000054 + BASE_DPU + VA_DPU)
#define VA_DPU_ADPLL1_TUNE1          (0x00000058 + BASE_DPU + VA_DPU)
#define VA_DPU_ADPLL1_STAT           (0x0000005C + BASE_DPU + VA_DPU)
#define VA_DPU_BIST_CTL              (0x00000090 + BASE_DPU + VA_DPU)
#define VA_DPU_BIST_END_STA          (0x00000094 + BASE_DPU + VA_DPU)
#define VA_DPU_BIST_RES              (0x00000098 + BASE_DPU + VA_DPU)

#define DATA_DPU_VER                     0x00001100
#define DATA_DPU_FTR                     0x0000003D
#define DATA_DPU_CTL                     0x00000000
#define DATA_DPU_CLK_SRC                 0x00000000
#define DATA_DPU_CLK_DIV                 0x00000000
#define DATA_DPU_CLK_GATING              0x00000000
#define DATA_DPU_ROUTINE                 0x00000000
#define DATA_DPU_SCLK_SEL                0x00000000
#define DATA_DPU_AXI_PRI                 0x00000000
#define DATA_DPU_ADPLL0_CTL              0x00000005
#define DATA_DPU_ADPLL0_TUNE0            0x13000000
#define DATA_DPU_ADPLL0_TUNE1            0x37071111
#define DATA_DPU_ADPLL0_STAT             0x00000000
#define DATA_DPU_ADPLL1_CTL              0x00000005
#define DATA_DPU_ADPLL1_TUNE0            0x13000000
#define DATA_DPU_ADPLL1_TUNE1            0x37071111
#define DATA_DPU_ADPLL1_STAT             0x00000000
#define DATA_DPU_BIST_CTL                0x0000003F
#define DATA_DPU_BIST_END_STA            0x00000000
#define DATA_DPU_BIST_RES                0x00000000

/* DPU Version Register */
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
} reg_dpu_ver_t;

/* DPU Feature Register */
typedef union {
	u32 val;
	struct {
	u32 has_dc0:1;  /**/
	u32 has_dc1:1;  /**/
	u32 has_se0:1;  /**/
	u32 has_se1:1;  /**/
	u32 has_rot:1;  /**/
	u32 has_dit:1;  /**/
	u32 rsvd0:26;   /**/
	} bits;
} reg_dpu_ftr_t;

#define DPU_FTR_HAS_DC0_0       0x0
#define DPU_FTR_HAS_DC0_1       0x1
#define DPU_FTR_HAS_DC1_0       0x0
#define DPU_FTR_HAS_DC1_1       0x1
#define DPU_FTR_HAS_SE0_0       0x0
#define DPU_FTR_HAS_SE0_1       0x1
#define DPU_FTR_HAS_SE1_0       0x0
#define DPU_FTR_HAS_SE1_1       0x1
#define DPU_FTR_HAS_ROT_0       0x0
#define DPU_FTR_HAS_ROT_1       0x1
#define DPU_FTR_HAS_DIT_0       0x0
#define DPU_FTR_HAS_DIT_1       0x1

/* DPU Control Register */
typedef union {
	u32 val;
	struct {
	u32 dc0_rst:1;  /**/
	u32 dc1_rst:1;  /**/
	u32 se0_rst:1;  /**/
	u32 se1_rst:1;  /**/
	u32 rot_rst:1;  /**/
	u32 dit_rst:1;  /**/
	u32 rsvd0:25;   /**/
	u32 top_rst:1;  /**/
	} bits;
} reg_dpu_ctl_t;

#define DPU_CTL_DC0_RST_0       0x0
#define DPU_CTL_DC0_RST_1       0x1
#define DPU_CTL_DC1_RST_0       0x0
#define DPU_CTL_DC1_RST_1       0x1
#define DPU_CTL_SE0_RST_0       0x0
#define DPU_CTL_SE0_RST_1       0x1
#define DPU_CTL_SE1_RST_0       0x0
#define DPU_CTL_SE1_RST_1       0x1
#define DPU_CTL_ROT_RST_0       0x0
#define DPU_CTL_ROT_RST_1       0x1
#define DPU_CTL_DIT_RST_0       0x0
#define DPU_CTL_DIT_RST_1       0x1
#define DPU_CTL_TOP_RST_0       0x0
#define DPU_CTL_TOP_RST_1       0x1

/* DPU Clock Source Register */
typedef union {
	u32 val;
	struct {
	u32 dc0_cs:2;   /**/
	u32 dc1_cs:2;   /**/
	u32 se0_cs:2;   /**/
	u32 se1_cs:2;   /**/
	u32 rot_cs:2;   /**/
	u32 dit_cs:2;   /**/
	u32 rsvd0:20;   /**/
	} bits;
} reg_dpu_clk_src_t;

#define DPU_CLK_SRC_DC0_CS_0    0x0
#define DPU_CLK_SRC_DC0_CS_1    0x1
#define DPU_CLK_SRC_DC1_CS_0    0x0
#define DPU_CLK_SRC_DC1_CS_1    0x1
#define DPU_CLK_SRC_SE0_CS_0    0x0
#define DPU_CLK_SRC_SE0_CS_1    0x1
#define DPU_CLK_SRC_SE1_CS_0    0x0
#define DPU_CLK_SRC_SE1_CS_1    0x1
#define DPU_CLK_SRC_ROT_CS_0    0x0
#define DPU_CLK_SRC_ROT_CS_1    0x1
#define DPU_CLK_SRC_DIT_CS_0    0x0
#define DPU_CLK_SRC_DIT_CS_1    0x1

/* DPU Clock Divider Register */
typedef union {
	u32 val;
	struct {
	u32 dc0_div:4;  /**/
	u32 dc1_div:4;  /**/
	u32 se0_div:4;  /**/
	u32 se1_div:4;  /**/
	u32 rot_div:4;  /**/
	u32 dit_div:4;  /**/
	u32 rsvd0:4;    /**/
	u32 tck_div:4;  /**/
	} bits;
} reg_dpu_clk_div_t;

/* DPU Clock Gating Register */
typedef union {
	u32 val;
	struct {
	u32 dc0_gt:1;   /**/
	u32 dc1_gt:1;   /**/
	u32 se0_gt:1;   /**/
	u32 se1_gt:1;   /**/
	u32 rot_gt:1;   /**/
	u32 dit_gt:1;   /**/
	u32 rsvd0:25;   /**/
	u32 tck_gt:1;   /**/
	} bits;
} reg_dpu_clk_gating_t;

#define DPU_CLK_GATING_DC0_GT_0 0x0
#define DPU_CLK_GATING_DC0_GT_1 0x1
#define DPU_CLK_GATING_DC1_GT_0 0x0
#define DPU_CLK_GATING_DC1_GT_1 0x1
#define DPU_CLK_GATING_SE0_GT_0 0x0
#define DPU_CLK_GATING_SE0_GT_1 0x1
#define DPU_CLK_GATING_SE1_GT_0 0x0
#define DPU_CLK_GATING_SE1_GT_1 0x1
#define DPU_CLK_GATING_ROT_GT_0 0x0
#define DPU_CLK_GATING_ROT_GT_1 0x1
#define DPU_CLK_GATING_DIT_GT_0 0x0
#define DPU_CLK_GATING_DIT_GT_1 0x1
#define DPU_CLK_GATING_TCK_GT_0 0x0
#define DPU_CLK_GATING_TCK_GT_1 0x1

/* DPU Routine Control Register */
typedef union {
	u32 val;
	struct {
	u32 se0_out_sel:1;/**/
	u32 se1_out_sel:1;/**/
	u32 rsvd0:2;    /**/
	u32 se_in_sel:1;/**/
	u32 rsvd1:3;    /**/
	u32 dc0sync_sel:1;/**/
	u32 dc1sync_sel:1;/**/
	u32 rsvd2:22;   /**/
	} bits;
} reg_dpu_routine_t;

#define DPU_ROUTINE_SE0_OUT_SEL_0       0x0
#define DPU_ROUTINE_SE0_OUT_SEL_1       0x1
#define DPU_ROUTINE_SE1_OUT_SEL_0       0x0
#define DPU_ROUTINE_SE1_OUT_SEL_1       0x1
#define DPU_ROUTINE_SE_IN_SEL_0         0x0
#define DPU_ROUTINE_SE_IN_SEL_1         0x1
#define DPU_ROUTINE_DC0SYNC_SEL_0       0x0
#define DPU_ROUTINE_DC0SYNC_SEL_1       0x1
#define DPU_ROUTINE_DC1SYNC_SEL_0       0x0
#define DPU_ROUTINE_DC1SYNC_SEL_1       0x1

/* DPU SCLK Select Register */
typedef union {
	u32 val;
	struct {
	u32 sclk0_sel:1;/**/
	u32 sclk1_sel:1;/**/
	u32 rsvd0:30;   /**/
	} bits;
} reg_dpu_sclk_sel_t;

#define DPU_SCLK_SEL_SCLK0_SEL_0        0x0
#define DPU_SCLK_SEL_SCLK0_SEL_1        0x1
#define DPU_SCLK_SEL_SCLK1_SEL_0        0x0
#define DPU_SCLK_SEL_SCLK1_SEL_1        0x1

/* ADPLL0 Control Register */
typedef union {
	u32 val;
	struct {
	u32 n:8;        /**/
	u32 rsvd0:23;   /**/
	u32 en:1;       /**/
	} bits;
} reg_dpu_adpll0_ctl_t;

#define DPU_ADPLL0_CTL_EN_0     0x0
#define DPU_ADPLL0_CTL_EN_1     0x1

/* ADPLL0 Tune0 Register */
typedef union {
	u32 val;
	struct {
	u32 tune0:15;   /**/
	u32 rsvd0:1;    /**/
	u32 tune1:5;    /**/
	u32 rsvd1:3;    /**/
	u32 tune2:1;    /**/
	u32 tune3:1;    /**/
	u32 rsvd2:2;    /**/
	u32 tune4:1;    /**/
	u32 rsvd3:3;    /**/
	} bits;
} reg_dpu_adpll0_tune0_t;

#define DPU_ADPLL0_TUNE0_TUNE4_0        0x0
#define DPU_ADPLL0_TUNE0_TUNE4_1        0x1

/* ADPLL0 Tune1 Register */
typedef union {
	u32 val;
	struct {
	u32 tune0:2;    /**/
	u32 rsvd0:2;    /**/
	u32 tune1:2;    /**/
	u32 rsvd1:2;    /**/
	u32 tune2:1;    /**/
	u32 rsvd2:3;    /**/
	u32 tune3:1;    /**/
	u32 rsvd3:3;    /**/
	u32 tune4:3;    /**/
	u32 rsvd4:1;    /**/
	u32 tune5:3;    /**/
	u32 rsvd5:1;    /**/
	u32 tune6:3;    /**/
	u32 rsvd6:1;    /**/
	u32 tune7:3;    /**/
	u32 rsvd7:1;    /**/
	} bits;
} reg_dpu_adpll0_tune1_t;

#define DPU_ADPLL0_TUNE1_TUNE2_0        0x0
#define DPU_ADPLL0_TUNE1_TUNE2_1        0x1
#define DPU_ADPLL0_TUNE1_TUNE3_0        0x0
#define DPU_ADPLL0_TUNE1_TUNE3_1        0x1

/* ADPLL0 Status Register */
typedef union {
	u32 val;
	struct {
	u32 ferr:9;     /**/
	u32 rsvd0:6;    /**/
	u32 flock:1;    /**/
	u32 perr:7;     /**/
	u32 rsvd1:8;    /**/
	u32 plock:1;    /**/
	} bits;
} reg_dpu_adpll0_stat_t;

#define DPU_ADPLL0_STAT_FLOCK_0 0x0
#define DPU_ADPLL0_STAT_FLOCK_1 0x1
#define DPU_ADPLL0_STAT_PLOCK_0 0x0
#define DPU_ADPLL0_STAT_PLOCK_1 0x1

/* ADPLL1 Control Register */
typedef union {
	u32 val;
	struct {
	u32 n:8;        /**/
	u32 rsvd0:23;   /**/
	u32 en:1;       /**/
	} bits;
} reg_dpu_adpll1_ctl_t;

#define DPU_ADPLL1_CTL_EN_0     0x0
#define DPU_ADPLL1_CTL_EN_1     0x1

/* ADPLL1 Tune0 Register */
typedef union {
	u32 val;
	struct {
	u32 tune0:15;   /**/
	u32 rsvd0:1;    /**/
	u32 tune1:5;    /**/
	u32 rsvd1:3;    /**/
	u32 tune2:1;    /**/
	u32 tune3:1;    /**/
	u32 rsvd2:2;    /**/
	u32 tune4:1;    /**/
	u32 rsvd3:3;    /**/
	} bits;
} reg_dpu_adpll1_tune0_t;

#define DPU_ADPLL1_TUNE0_TUNE4_0        0x0
#define DPU_ADPLL1_TUNE0_TUNE4_1        0x1

/* ADPLL1 Tune1 Register */
typedef union {
	u32 val;
	struct {
	u32 tune0:2;    /**/
	u32 rsvd0:2;    /**/
	u32 tune1:2;    /**/
	u32 rsvd1:2;    /**/
	u32 tune2:1;    /**/
	u32 rsvd2:3;    /**/
	u32 tune3:1;    /**/
	u32 rsvd3:3;    /**/
	u32 tune4:3;    /**/
	u32 rsvd4:1;    /**/
	u32 tune5:3;    /**/
	u32 rsvd5:1;    /**/
	u32 tune6:3;    /**/
	u32 rsvd6:1;    /**/
	u32 tune7:3;    /**/
	u32 rsvd7:1;    /**/
	} bits;
} reg_dpu_adpll1_tune1_t;

#define DPU_ADPLL1_TUNE1_TUNE2_0        0x0
#define DPU_ADPLL1_TUNE1_TUNE2_1        0x1
#define DPU_ADPLL1_TUNE1_TUNE3_0        0x0
#define DPU_ADPLL1_TUNE1_TUNE3_1        0x1

/* ADPLL1 Status Register */
typedef union {
	u32 val;
	struct {
	u32 ferr:9;     /**/
	u32 rsvd0:6;    /**/
	u32 flock:1;    /**/
	u32 perr:7;     /**/
	u32 rsvd1:8;    /**/
	u32 plock:1;    /**/
	} bits;
} reg_dpu_adpll1_stat_t;

#define DPU_ADPLL1_STAT_FLOCK_0 0x0
#define DPU_ADPLL1_STAT_FLOCK_1 0x1
#define DPU_ADPLL1_STAT_PLOCK_0 0x0
#define DPU_ADPLL1_STAT_PLOCK_1 0x1

#define DPU_BIST_CTL_DC0_BIST_EN_0      0x0
#define DPU_BIST_CTL_DC0_BIST_EN_1      0x1
#define DPU_BIST_CTL_DC1_BIST_EN_0      0x0
#define DPU_BIST_CTL_DC1_BIST_EN_1      0x1
#define DPU_BIST_CTL_SE0_BIST_EN_0      0x0
#define DPU_BIST_CTL_SE0_BIST_EN_1      0x1
#define DPU_BIST_CTL_SE1_BIST_EN_0      0x0
#define DPU_BIST_CTL_SE1_BIST_EN_1      0x1
#define DPU_BIST_CTL_ROT_BIST_EN_0      0x0
#define DPU_BIST_CTL_ROT_BIST_EN_1      0x1
#define DPU_BIST_CTL_DIT_BIST_EN_0      0x0
#define DPU_BIST_CTL_DIT_BIST_EN_1      0x1

#define DPU_BIST_END_STA_DC0_END_0      0x0
#define DPU_BIST_END_STA_DC0_END_1      0x1
#define DPU_BIST_END_STA_DC1_END_0      0x0
#define DPU_BIST_END_STA_DC1_END_1      0x1
#define DPU_BIST_END_STA_SE0_END_0      0x0
#define DPU_BIST_END_STA_SE0_END_1      0x1
#define DPU_BIST_END_STA_SE1_END_0      0x0
#define DPU_BIST_END_STA_SE1_END_1      0x1
#define DPU_BIST_END_STA_ROT_END_0      0x0
#define DPU_BIST_END_STA_ROT_END_1      0x1
#define DPU_BIST_END_STA_DIT_END_0      0x0
#define DPU_BIST_END_STA_DIT_END_1      0x1

#define DPU_BIST_RES_DC0_RES_0  0x0
#define DPU_BIST_RES_DC0_RES_1  0x1
#define DPU_BIST_RES_DC1_RES_0  0x0
#define DPU_BIST_RES_DC1_RES_1  0x1
#define DPU_BIST_RES_SE0_RES_0  0x0
#define DPU_BIST_RES_SE0_RES_1  0x1
#define DPU_BIST_RES_SE1_RES_0  0x0
#define DPU_BIST_RES_SE1_RES_1  0x1
#define DPU_BIST_RES_ROT_RES_0  0x0
#define DPU_BIST_RES_ROT_RES_1  0x1
#define DPU_BIST_RES_DIT_RES_0  0x0
#define DPU_BIST_RES_DIT_RES_1  0x1

#define DPU_REG_ADPLL_CTL(base, x) ((char *)(base) + 0x40 + (x) * 0x10)
#define DPU_REG_ADPLL_TUNE0(base, x) ((char *)(base) + 0x44 + (x) * 0x10)
#define DPU_REG_ADPLL_TUNE1(base, x) ((char *)(base) + 0x48 + (x) * 0x10)
#define DPU_REG_ADPLL_STATUS(base, x) ((char *)(base) + 0x4C + (x) * 0x10)

/* reg_dpu_t bank */
typedef struct tag_dpu {
	reg_dpu_ver_t              ver;            /* 0000 */
	reg_dpu_ftr_t              ftr;            /* 0004 */
	u32                        res0[2];
	reg_dpu_ctl_t              ctl;            /* 0010 */
	reg_dpu_clk_src_t          clk_src;        /* 0014 */
	reg_dpu_clk_div_t          clk_div;        /* 0018 */
	reg_dpu_clk_gating_t       clk_gating;     /* 001C */
	reg_dpu_routine_t          routine;        /* 0020 */
	reg_dpu_sclk_sel_t         sclk_sel;       /* 0024 */
	u32                        res1[2];
	u32                        prv_0030;       /* 0030 */
	u32                        res2[3];
	reg_dpu_adpll0_ctl_t       adpll0_ctl;     /* 0040 */
	reg_dpu_adpll0_tune0_t     adpll0_tune0;   /* 0044 */
	reg_dpu_adpll0_tune1_t     adpll0_tune1;   /* 0048 */
	reg_dpu_adpll0_stat_t      adpll0_stat;    /* 004C */
	reg_dpu_adpll1_ctl_t       adpll1_ctl;     /* 0050 */
	reg_dpu_adpll1_tune0_t     adpll1_tune0;   /* 0054 */
	reg_dpu_adpll1_tune1_t     adpll1_tune1;   /* 0058 */
	reg_dpu_adpll1_stat_t      adpll1_stat;    /* 005C */
	u32                        res3[12];
	u32                        prv_0090;       /* 0090 */
	u32                        prv_0094;       /* 0094 */
	u32                        prv_0098;       /* 0098 */
} reg_dpu_t;

#endif /* ___DPU___H___ */
