/* dit.h */

#ifndef ___DIT___H___
#define ___DIT___H___

#define BASE_DIT                0x00000000

#define VA_DIT_VER               (0x00000000 + BASE_DIT + VA_DIT)
#define VA_DIT_FTR               (0x00000004 + BASE_DIT + VA_DIT)
#define VA_DIT_STA               (0x0000000C + BASE_DIT + VA_DIT)
#define VA_DIT_CTL               (0x00000010 + BASE_DIT + VA_DIT)
#define VA_DIT_CFG               (0x00000014 + BASE_DIT + VA_DIT)
#define VA_DIT_WBCTL             (0x00000018 + BASE_DIT + VA_DIT)
#define VA_DIT_WBTMR             (0x0000001C + BASE_DIT + VA_DIT)
#define VA_DIT_INADDR0           (0x00000020 + BASE_DIT + VA_DIT)
#define VA_DIT_INADDR1           (0x00000024 + BASE_DIT + VA_DIT)
#define VA_DIT_INADDR2           (0x00000028 + BASE_DIT + VA_DIT)
#define VA_DIT_INADDR3           (0x0000002C + BASE_DIT + VA_DIT)
#define VA_DIT_OUTADDR           (0x00000030 + BASE_DIT + VA_DIT)
#define VA_DIT_INSIZE            (0x00000034 + BASE_DIT + VA_DIT)
#define VA_DIT_OUTSIZE           (0x00000038 + BASE_DIT + VA_DIT)
#define VA_DIT_LSTR              (0x0000003C + BASE_DIT + VA_DIT)
#define VA_DIT_WBINTCTL          (0x00000040 + BASE_DIT + VA_DIT)
#define VA_DIT_WBINTSTA          (0x00000044 + BASE_DIT + VA_DIT)
#define VA_DIT_WBINTCLR          (0x00000048 + BASE_DIT + VA_DIT)
#define VA_DIT_STHR              (0x00000050 + BASE_DIT + VA_DIT)
#define VA_DIT_ITHR              (0x00000054 + BASE_DIT + VA_DIT)
#define VA_DIT_SPCNT             (0x00000058 + BASE_DIT + VA_DIT)

#define DATA_DIT_VER                 0x00001101
#define DATA_DIT_FTR                 0x00000001
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
	u32 ver_l:8;    /**/
	u32 ver_h:3;    /**/
	u32 rsvd0:1;    /**/
	u32 comp:1;     /**/
	u32 rsvd1:19;   /**/
	} bits;
} reg_dit_ver_t;

#define DIT_VER_COMP_0  0x0
#define DIT_VER_COMP_1  0x1

/* DIT Feature Register */
typedef union {
	u32 val;
	struct {
	u32 has_ftr0:1; /**/
	u32 has_ftr1:1; /**/
	u32 has_ftr2:1; /**/
	u32 rsvd0:29;   /**/
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
	u32 rsvd0:16;   /**/
	u32 rdsta:1;    /**/
	u32 wrsta:1;    /**/
	u32 rsvd1:14;   /**/
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
	u32 rst:1;      /**/
	u32 rsvd0:28;   /**/
	u32 bisten:1;   /**/
	u32 dbg_ctl:1;  /**/
	u32 dit_en:1;   /**/
	} bits;
} reg_dit_ctl_t;

#define DIT_CTL_RST_0           0x0
#define DIT_CTL_RST_1           0x1
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
	u32 ditmode:1;  /**/
	u32 rsvd0:7;    /**/
	u32 pixseq:1;   /**/
	u32 field_pol:1;/**/
	u32 rsvd1:6;    /**/
	u32 rsvd2:16;   /**/
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
	u32 start:1;    /**/
	u32 rsvd0:31;   /**/
	} bits;
} reg_dit_wbctl_t;

#define DIT_WBCTL_START_0       0x0
#define DIT_WBCTL_START_1       0x1

/* DIT WB Timer Register */
typedef union {
	u32 val;
	struct {
	u32 wbthr:16;   /**/
	u32 wbtmr:16;   /**/
	} bits;
} reg_dit_wbtmr_t;

/* DIT Input Address 0 Register */
typedef union {
	u32 val;
	struct {
	u32 addr:32;    /**/
	} bits;
} reg_dit_inaddr0_t;

/* DIT Input Address 1 Register */
typedef union {
	u32 val;
	struct {
	u32 addr:32;    /**/
	} bits;
} reg_dit_inaddr1_t;

/* DIT Input Address 2 Register */
typedef union {
	u32 val;
	struct {
	u32 addr:32;    /**/
	} bits;
} reg_dit_inaddr2_t;

/* DIT Input Address 3 Register */
typedef union {
	u32 val;
	struct {
	u32 addr:32;    /**/
	} bits;
} reg_dit_inaddr3_t;

/* DIT Output Address Register */
typedef union {
	u32 val;
	struct {
	u32 addr:32;    /**/
	} bits;
} reg_dit_outaddr_t;

/* DIT Input Size Register */
typedef union {
	u32 val;
	struct {
	u32 w:12;       /**/
	u32 rsvd0:4;    /**/
	u32 h:12;       /**/
	u32 rsvd1:4;    /**/
	} bits;
} reg_dit_insize_t;

/* DIT Output Size Register */
typedef union {
	u32 val;
	struct {
	u32 w:12;       /**/
	u32 rsvd0:4;    /**/
	u32 h:12;       /**/
	u32 rsvd1:4;    /**/
	} bits;
} reg_dit_outsize_t;

/* DIT Line Stride Register */
typedef union {
	u32 val;
	struct {
	u32 ilstr:12;   /**/
	u32 rsvd0:4;    /**/
	u32 olstr:12;   /**/
	u32 rsvd1:4;    /**/
	} bits;
} reg_dit_lstr_t;

/* DIT WB Interrupt Control Register */
typedef union {
	u32 val;
	struct {
	u32 wbfin_en:1; /**/
	u32 wbovfl_en:1;/**/
	u32 wbtmout_en:1;/**/
	u32 rsvd0:29;   /**/
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
	u32 wbfin:1;    /**/
	u32 wbovfl:1;   /**/
	u32 wbtmout:1;  /**/
	u32 rsvd0:29;   /**/
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
	u32 wbfinclr:1; /**/
	u32 wbovflclr:1;/**/
	u32 wbtmoutclr:1;/**/
	u32 rsvd0:29;   /**/
	} bits;
} reg_dit_wbintclr_t;

/* DIT Still Threshold Register */
typedef union {
	u32 val;
	struct {
	u32 sthr0:8;    /**/
	u32 rsvd0:23;   /**/
	u32 stavg:1;    /**/
	} bits;
} reg_dit_sthr_t;

#define DIT_STHR_STAVG_0        0x0
#define DIT_STHR_STAVG_1        0x1

/* DIT Interpolation Threshold Register */
typedef union {
	u32 val;
	struct {
	u32 thr0:8;     /**/
	u32 thr1:8;     /**/
	u32 thr2:8;     /**/
	u32 thr3:8;     /**/
	} bits;
} reg_dit_ithr_t;

/* DIT Still Pixels Counter Register */
typedef union {
	u32 val;
	struct {
	u32 cnt:20;     /**/
	u32 rsvd0:12;   /**/
	} bits;
} reg_dit_spcnt_t;

/* reg_dit_t bank */
typedef struct tag_dit {
	reg_dit_ver_t          ver;        /* 0000 */
	reg_dit_ftr_t          ftr;        /* 0004 */
	u32                    res0[1];
	reg_dit_sta_t          sta;        /* 000C */
	reg_dit_ctl_t          ctl;        /* 0010 */
	reg_dit_cfg_t          cfg;        /* 0014 */
	reg_dit_wbctl_t        wbctl;      /* 0018 */
	reg_dit_wbtmr_t        wbtmr;      /* 001C */
	reg_dit_inaddr0_t      inaddr0;    /* 0020 */
	reg_dit_inaddr1_t      inaddr1;    /* 0024 */
	reg_dit_inaddr2_t      inaddr2;    /* 0028 */
	reg_dit_inaddr3_t      inaddr3;    /* 002C */
	reg_dit_outaddr_t      outaddr;    /* 0030 */
	reg_dit_insize_t       insize;     /* 0034 */
	reg_dit_outsize_t      outsize;    /* 0038 */
	reg_dit_lstr_t         lstr;       /* 003C */
	reg_dit_wbintctl_t     wbintctl;   /* 0040 */
	reg_dit_wbintsta_t     wbintsta;   /* 0044 */
	reg_dit_wbintclr_t     wbintclr;   /* 0048 */
	u32                    res1[1];
	reg_dit_sthr_t         sthr;       /* 0050 */
	reg_dit_ithr_t         ithr;       /* 0054 */
	reg_dit_spcnt_t        spcnt;      /* 0058 */
} reg_dit_t;

#endif /* ___DIT___H___ */
