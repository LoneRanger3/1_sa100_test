/* cpu_ctrl.h */

#ifndef ___CPU_CTRL___H___
#define ___CPU_CTRL___H___

#define BASE_CPU_CTRL                0x04020000

#define VA_CPU_CTRL_RST_CTRL             (0x00000000 + BASE_CPU_CTRL + VA_CPU_CTRL)
#define VA_CPU_CTRL_CTRL_R               (0x00000010 + BASE_CPU_CTRL + VA_CPU_CTRL)
#define VA_CPU_CTRL_CFG_R                (0x00000020 + BASE_CPU_CTRL + VA_CPU_CTRL)
#define VA_CPU_CTRL_STATUS_R             (0x00000050 + BASE_CPU_CTRL + VA_CPU_CTRL)
#define VA_CPU_CTRL_GENER_REG            (0x00000060 + BASE_CPU_CTRL + VA_CPU_CTRL)
#define VA_CPU_CTRL_CPU_CLK_SEL          (0x0000007C + BASE_CPU_CTRL + VA_CPU_CTRL)
#define VA_CPU_CTRL_ADPLL_FAC            (0x00000080 + BASE_CPU_CTRL + VA_CPU_CTRL)
#define VA_CPU_CTRL_ADPLL_TUNE0          (0x00000084 + BASE_CPU_CTRL + VA_CPU_CTRL)
#define VA_CPU_CTRL_ADPLL_TUNE1          (0x00000088 + BASE_CPU_CTRL + VA_CPU_CTRL)
#define VA_CPU_CTRL_ADPLL_STAT           (0x0000008C + BASE_CPU_CTRL + VA_CPU_CTRL)
#define VA_CPU_CTRL_CACHE_CFG0           (0x00000090 + BASE_CPU_CTRL + VA_CPU_CTRL)
#define VA_CPU_CTRL_CACHE_CFG1           (0x00000094 + BASE_CPU_CTRL + VA_CPU_CTRL)
#define VA_CPU_CTRL_DBG_REG0             (0x000000A0 + BASE_CPU_CTRL + VA_CPU_CTRL)
#define VA_CPU_CTRL_VER                  (0x000000F0 + BASE_CPU_CTRL + VA_CPU_CTRL)
#define VA_CPU_CTRL_C_RST_CTRL           (0x00000400 + BASE_CPU_CTRL + VA_CPU_CTRL)

#define DATA_CPU_CTRL_RST_CTRL               0x00000FF1
#define DATA_CPU_CTRL_CTRL_R                 0x00020000
#define DATA_CPU_CTRL_CFG_R                  0x00000008
#define DATA_CPU_CTRL_STATUS_R               0x00000000
#define DATA_CPU_CTRL_GENER_REG              0x00000000
#define DATA_CPU_CTRL_CPU_CLK_SEL            0x00000000
#define DATA_CPU_CTRL_ADPLL_FAC              0x00001600
#define DATA_CPU_CTRL_ADPLL_TUNE0            0x139F0000
#define DATA_CPU_CTRL_ADPLL_TUNE1            0x57379130
#define DATA_CPU_CTRL_ADPLL_STAT             0x00000000
#define DATA_CPU_CTRL_CACHE_CFG0             0x00000000
#define DATA_CPU_CTRL_CACHE_CFG1             0x00000000
#define DATA_CPU_CTRL_DBG_REG0               0x00000000
#define DATA_CPU_CTRL_VER                    0x00001100
#define DATA_CPU_CTRL_C_RST_CTRL             0x00011071

/* CPU Reset Control Register */
typedef union {
	u32 val;
	struct {
	u32 ncorereset0:1;      /**/
	u32 ncorereset1:1;      /**/
	u32 rsvd0:2;            /**/
	u32 ncoreporeset0:1;    /**/
	u32 ncoreporeset1:1;    /**/
	u32 rsvd1:2;            /**/
	u32 ndbgreset0:1;       /**/
	u32 ndbgreset1:1;       /**/
	u32 rsvd2:2;            /**/
	u32 rsvd3:4;            /**/
	u32 l1rstdisable0:1;    /**/
	u32 l1rstdisable1:1;    /**/
	u32 rsvd4:2;            /**/
	u32 rsvd5:12;           /**/
	} bits;
} reg_cpu_ctrl_rst_ctrl_t;

#define CPU_CTRL_RST_CTRL_NCORERESET0_0         0x0
#define CPU_CTRL_RST_CTRL_NCORERESET0_1         0x1
#define CPU_CTRL_RST_CTRL_NCORERESET1_0         0x0
#define CPU_CTRL_RST_CTRL_NCORERESET1_1         0x1
#define CPU_CTRL_RST_CTRL_NCOREPORESET0_0       0x0
#define CPU_CTRL_RST_CTRL_NCOREPORESET0_1       0x1
#define CPU_CTRL_RST_CTRL_NCOREPORESET1_0       0x0
#define CPU_CTRL_RST_CTRL_NCOREPORESET1_1       0x1
#define CPU_CTRL_RST_CTRL_NDBGRESET0_0          0x0
#define CPU_CTRL_RST_CTRL_NDBGRESET0_1          0x1
#define CPU_CTRL_RST_CTRL_NDBGRESET1_0          0x0
#define CPU_CTRL_RST_CTRL_NDBGRESET1_1          0x1
#define CPU_CTRL_RST_CTRL_L1RSTDISABLE0_0       0x0
#define CPU_CTRL_RST_CTRL_L1RSTDISABLE0_1       0x1
#define CPU_CTRL_RST_CTRL_L1RSTDISABLE1_0       0x0
#define CPU_CTRL_RST_CTRL_L1RSTDISABLE1_1       0x1

/* CPU Control Register */
typedef union {
	u32 val;
	struct {
	u32 eventi:1;           /**/
	u32 rsvd0:3;            /**/
	u32 cfgsdisable:1;      /**/
	u32 rsvd1:3;            /**/
	u32 cp15sdisable0:1;    /**/
	u32 cp15sdisable1:1;    /**/
	u32 rsvd2:2;            /**/
	u32 rsvd3:4;            /**/
	u32 rsvd4:1;            /**/
	u32 cpu1_gat:1;         /**/
	u32 rsvd5:13;           /**/
	u32 acinactm:1;         /**/
	} bits;
} reg_cpu_ctrl_ctrl_r_t;

#define CPU_CTRL_CTRL_R_CFGSDISABLE_0   0x0
#define CPU_CTRL_CTRL_R_CFGSDISABLE_1   0x1
#define CPU_CTRL_CTRL_R_CP15SDISABLE0_0 0x0
#define CPU_CTRL_CTRL_R_CP15SDISABLE0_1 0x1
#define CPU_CTRL_CTRL_R_CP15SDISABLE1_0 0x0
#define CPU_CTRL_CTRL_R_CP15SDISABLE1_1 0x1
#define CPU_CTRL_CTRL_R_CPU1_GAT_0      0x0
#define CPU_CTRL_CTRL_R_CPU1_GAT_1      0x1

/* CPU Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 broadcastinner:1;   /**/
	u32 broadcastouter:1;   /**/
	u32 broadcastcachemaint:1;/**/
	u32 sysbardisable:1;    /**/
	u32 rsvd0:28;           /**/
	} bits;
} reg_cpu_ctrl_cfg_r_t;

#define CPU_CTRL_CFG_R_BROADCASTINNER_0         0x0
#define CPU_CTRL_CFG_R_BROADCASTINNER_1         0x1
#define CPU_CTRL_CFG_R_BROADCASTOUTER_0         0x0
#define CPU_CTRL_CFG_R_BROADCASTOUTER_1         0x1
#define CPU_CTRL_CFG_R_BROADCASTCACHEMAINT_0    0x0
#define CPU_CTRL_CFG_R_BROADCASTCACHEMAINT_1    0x1
#define CPU_CTRL_CFG_R_SYSBARDISABLE_0          0x0
#define CPU_CTRL_CFG_R_SYSBARDISABLE_1          0x1

/* CPU Status Register */
typedef union {
	u32 val;
	struct {
	u32 smpnamp0:1;         /**/
	u32 smpnamp1:1;         /**/
	u32 rsvd0:2;            /**/
	u32 rsvd1:4;            /**/
	u32 standbywfe0:1;      /**/
	u32 standbywfe1:1;      /**/
	u32 rsvd2:2;            /**/
	u32 rsvd3:4;            /**/
	u32 standbywfi0:1;      /**/
	u32 standbywfi1:1;      /**/
	u32 rsvd4:2;            /**/
	u32 standbywfil2:1;     /**/
	u32 rsvd5:11;           /**/
	} bits;
} reg_cpu_ctrl_status_r_t;

#define CPU_CTRL_STATUS_R_SMPNAMP0_0            0x0
#define CPU_CTRL_STATUS_R_SMPNAMP0_1            0x1
#define CPU_CTRL_STATUS_R_SMPNAMP1_0            0x0
#define CPU_CTRL_STATUS_R_SMPNAMP1_1            0x1
#define CPU_CTRL_STATUS_R_STANDBYWFE0_0         0x0
#define CPU_CTRL_STATUS_R_STANDBYWFE0_1         0x1
#define CPU_CTRL_STATUS_R_STANDBYWFE1_0         0x0
#define CPU_CTRL_STATUS_R_STANDBYWFE1_1         0x1
#define CPU_CTRL_STATUS_R_STANDBYWFI0_0         0x0
#define CPU_CTRL_STATUS_R_STANDBYWFI0_1         0x1
#define CPU_CTRL_STATUS_R_STANDBYWFI1_0         0x0
#define CPU_CTRL_STATUS_R_STANDBYWFI1_1         0x1
#define CPU_CTRL_STATUS_R_STANDBYWFIL2_0        0x0
#define CPU_CTRL_STATUS_R_STANDBYWFIL2_1        0x1

/* General Register */
typedef union {
	u32 val;
	struct {
	u32 gpr:32;/**/
	} bits;
} reg_cpu_ctrl_gener_reg_t;

/* CPU Clock Selection Register */
typedef union {
	u32 val;
	struct {
	u32 src:1;      /**/
	u32 rsvd0:3;    /**/
	u32 src_div:2;  /**/
	u32 rsvd1:22;   /**/
	u32 dbg_en:1;   /**/
	u32 rsvd2:2;    /**/
	u32 en:1;       /**/
	} bits;
} reg_cpu_ctrl_cpu_clk_sel_t;

#define CPU_CTRL_CPU_CLK_SEL_SRC_0      0x0
#define CPU_CTRL_CPU_CLK_SEL_SRC_1      0x1
#define CPU_CTRL_CPU_CLK_SEL_SRC_DIV_0  0x0
#define CPU_CTRL_CPU_CLK_SEL_SRC_DIV_1  0x1
#define CPU_CTRL_CPU_CLK_SEL_SRC_DIV_2  0x2
#define CPU_CTRL_CPU_CLK_SEL_SRC_DIV_3  0x3
#define CPU_CTRL_CPU_CLK_SEL_DBG_EN_0   0x0
#define CPU_CTRL_CPU_CLK_SEL_DBG_EN_1   0x1
#define CPU_CTRL_CPU_CLK_SEL_EN_0       0x0
#define CPU_CTRL_CPU_CLK_SEL_EN_1       0x1

/* ADPLL Factor Register */
typedef union {
	u32 val;
	struct {
	u32 pre_div:1;  /**/
	u32 rsvd0:7;    /**/
	u32 n:8;        /**/
	u32 rsvd1:16;   /**/
	} bits;
} reg_cpu_ctrl_adpll_fac_t;

#define CPU_CTRL_ADPLL_FAC_PRE_DIV_0    0x0
#define CPU_CTRL_ADPLL_FAC_PRE_DIV_1    0x1

/* ADPLL Tune0 Register */
typedef union {
	u32 val;
	struct {
	u32 tune0:15;           /**/
	u32 analog_rsvd00:1;    /**/
	u32 tune1:7;            /**/
	u32 auto_adrange:1;     /**/
	u32 tune2:1;            /**/
	u32 tune3:1;            /**/
	u32 analog_rsvd21:2;    /**/
	u32 tune4:1;            /**/
	u32 analog_rsvd32:3;    /**/
	} bits;
} reg_cpu_ctrl_adpll_tune0_t;

#define CPU_CTRL_ADPLL_TUNE0_AUTO_ADRANGE_0     0x0
#define CPU_CTRL_ADPLL_TUNE0_AUTO_ADRANGE_1     0x1
#define CPU_CTRL_ADPLL_TUNE0_TUNE4_0            0x0
#define CPU_CTRL_ADPLL_TUNE0_TUNE4_1            0x1

/* ADPLL Tune1 Register */
typedef union {
	u32 val;
	struct {
	u32 tune0:2;            /**/
	u32 analog_rsvd00:2;    /**/
	u32 tune1:2;            /**/
	u32 analog_rsvd11:2;    /**/
	u32 tune2:1;            /**/
	u32 analog_rsvd22:3;    /**/
	u32 tune3:1;            /**/
	u32 slewrate:3;         /**/
	u32 tune4:3;            /**/
	u32 analog_rsvd43:1;    /**/
	u32 tune5:3;            /**/
	u32 analog_rsvd54:1;    /**/
	u32 tune6:3;            /**/
	u32 analog_rsvd65:1;    /**/
	u32 tune7:3;            /**/
	u32 analog_rsvd76:1;    /**/
	} bits;
} reg_cpu_ctrl_adpll_tune1_t;

#define CPU_CTRL_ADPLL_TUNE1_TUNE2_0            0x0
#define CPU_CTRL_ADPLL_TUNE1_TUNE2_1            0x1
#define CPU_CTRL_ADPLL_TUNE1_TUNE3_0            0x0
#define CPU_CTRL_ADPLL_TUNE1_TUNE3_1            0x1
#define CPU_CTRL_ADPLL_TUNE1_SLEWRATE_0         0x0
#define CPU_CTRL_ADPLL_TUNE1_SLEWRATE_1         0x1
#define CPU_CTRL_ADPLL_TUNE1_SLEWRATE_2         0x2
#define CPU_CTRL_ADPLL_TUNE1_SLEWRATE_3         0x3
#define CPU_CTRL_ADPLL_TUNE1_SLEWRATE_4         0x4
#define CPU_CTRL_ADPLL_TUNE1_SLEWRATE_5         0x5
#define CPU_CTRL_ADPLL_TUNE1_SLEWRATE_6         0x6
#define CPU_CTRL_ADPLL_TUNE1_SLEWRATE_7         0x7

/* ADPLL Status Register */
typedef union {
	u32 val;
	struct {
	u32 ferr:9;             /**/
	u32 analog_rsvd00:6;    /**/
	u32 flock:1;            /**/
	u32 perr:7;             /**/
	u32 analog_rsvd11:8;    /**/
	u32 plock:1;            /**/
	} bits;
} reg_cpu_ctrl_adpll_stat_t;

#define CPU_CTRL_ADPLL_STAT_FLOCK_0             0x0
#define CPU_CTRL_ADPLL_STAT_FLOCK_1             0x1
#define CPU_CTRL_ADPLL_STAT_PLOCK_0             0x0
#define CPU_CTRL_ADPLL_STAT_PLOCK_1             0x1

/* Cache Parameter Configuration Register0 */
typedef union {
	u32 val;
	struct {
	u32 parameter:32;/**/
	} bits;
} reg_cpu_ctrl_cache_cfg0_t;

/* Cache Parameter Configuration Register1 */
typedef union {
	u32 val;
	struct {
	u32 parameter:32;/**/
	} bits;
} reg_cpu_ctrl_cache_cfg1_t;

/* Debug Register0 */
typedef union {
	u32 val;
	struct {
	u32 dbgpwrdup0:1;       /**/
	u32 dbgpwrdup1:1;       /**/
	u32 rsvd0:2;            /**/
	u32 dbgpwrupreq0:1;     /**/
	u32 dbgpwrupreq1:1;     /**/
	u32 rsvd1:2;            /**/
	u32 rsvd2:4;            /**/
	u32 dbgnopwrdwn0:1;     /**/
	u32 dbgnopwrdwn1:1;     /**/
	u32 rsvd3:2;            /**/
	u32 rsvd4:16;           /**/
	} bits;
} reg_cpu_ctrl_dbg_reg0_t;

#define CPU_CTRL_DBG_REG0_DBGPWRUPREQ0_0        0x0
#define CPU_CTRL_DBG_REG0_DBGPWRUPREQ0_1        0x1
#define CPU_CTRL_DBG_REG0_DBGPWRUPREQ1_0        0x0
#define CPU_CTRL_DBG_REG0_DBGPWRUPREQ1_1        0x1
#define CPU_CTRL_DBG_REG0_DBGNOPWRDWN0_0        0x0
#define CPU_CTRL_DBG_REG0_DBGNOPWRDWN0_1        0x1
#define CPU_CTRL_DBG_REG0_DBGNOPWRDWN1_0        0x0
#define CPU_CTRL_DBG_REG0_DBGNOPWRDWN1_1        0x1

/* Controller Version Register */
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
} reg_cpu_ctrl_ver_t;

#define CPU_CTRL_VER_COMP_0     0x0
#define CPU_CTRL_VER_COMP_1     0x1

/* Cluster0 Reset Control Register */
typedef union {
	u32 val;
	struct {
	u32 cluster_rst:1;      /**/
	u32 rsvd0:3;            /**/
	u32 soc_dbg_rst:1;      /**/
	u32 dap_rst:1;          /**/
	u32 jtag_rst:1;         /**/
	u32 rsvd1:5;            /**/
	u32 gic_rst:1;          /**/
	u32 rsvd2:3;            /**/
	u32 l2_rst:1;           /**/
	u32 rsvd3:3;            /**/
	u32 l2rstdisable:1;     /**/
	u32 rsvd4:11;           /**/
	} bits;
} reg_cpu_ctrl_c_rst_ctrl_t;

#define CPU_CTRL_C_RST_CTRL_CLUSTER_RST_0       0x0
#define CPU_CTRL_C_RST_CTRL_CLUSTER_RST_1       0x1
#define CPU_CTRL_C_RST_CTRL_SOC_DBG_RST_0       0x0
#define CPU_CTRL_C_RST_CTRL_SOC_DBG_RST_1       0x1
#define CPU_CTRL_C_RST_CTRL_DAP_RST_0           0x0
#define CPU_CTRL_C_RST_CTRL_DAP_RST_1           0x1
#define CPU_CTRL_C_RST_CTRL_JTAG_RST_0          0x0
#define CPU_CTRL_C_RST_CTRL_JTAG_RST_1          0x1
#define CPU_CTRL_C_RST_CTRL_GIC_RST_0           0x0
#define CPU_CTRL_C_RST_CTRL_GIC_RST_1           0x1
#define CPU_CTRL_C_RST_CTRL_L2_RST_0            0x0
#define CPU_CTRL_C_RST_CTRL_L2_RST_1            0x1
#define CPU_CTRL_C_RST_CTRL_L2RSTDISABLE_0      0x0
#define CPU_CTRL_C_RST_CTRL_L2RSTDISABLE_1      0x1

/* reg_cpu_ctrl_t bank */
typedef struct tag_cpu_ctrl {
	reg_cpu_ctrl_rst_ctrl_t        rst_ctrl;   /* 0000 */
	u32                            res0[3];
	reg_cpu_ctrl_ctrl_r_t          ctrl_r;     /* 0010 */
	u32                            res1[3];
	reg_cpu_ctrl_cfg_r_t           cfg_r;      /* 0020 */
	u32                            res2[11];
	reg_cpu_ctrl_status_r_t        status_r;   /* 0050 */
	u32                            res3[3];
	reg_cpu_ctrl_gener_reg_t       gener_reg;  /* 0060 */
	u32                            res4[6];
	reg_cpu_ctrl_cpu_clk_sel_t     cpu_clk_sel;/* 007C */
	reg_cpu_ctrl_adpll_fac_t       adpll_fac;  /* 0080 */
	reg_cpu_ctrl_adpll_tune0_t     adpll_tune0;/* 0084 */
	reg_cpu_ctrl_adpll_tune1_t     adpll_tune1;/* 0088 */
	reg_cpu_ctrl_adpll_stat_t      adpll_stat; /* 008C */
	reg_cpu_ctrl_cache_cfg0_t      cache_cfg0; /* 0090 */
	reg_cpu_ctrl_cache_cfg1_t      cache_cfg1; /* 0094 */
	u32                            res5[2];
	reg_cpu_ctrl_dbg_reg0_t        dbg_reg0;   /* 00A0 */
	u32                            res6[19];
	reg_cpu_ctrl_ver_t             ver;        /* 00F0 */
	u32                            res7[195];
	reg_cpu_ctrl_c_rst_ctrl_t      c_rst_ctrl; /* 0400 */
} reg_cpu_ctrl_t;

#endif /* ___CPU_CTRL___H___ */
