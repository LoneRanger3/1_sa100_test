/* rot.h */

#ifndef ___ROT___H___
#define ___ROT___H___

#define BASE_ROT                0x01580000

#define VA_ROT_VER                   (0x00000000 + BASE_ROT + VA_ROT)
#define VA_ROT_FTR                   (0x00000004 + BASE_ROT + VA_ROT)
#define VA_ROT_CTL                   (0x00000010 + BASE_ROT + VA_ROT)
#define VA_ROT_CFG                   (0x00000014 + BASE_ROT + VA_ROT)
#define VA_ROT_STRMCTL               (0x00000018 + BASE_ROT + VA_ROT)
#define VA_ROT_STA                   (0x0000001C + BASE_ROT + VA_ROT)
#define VA_ROT_INTEN                 (0x00000020 + BASE_ROT + VA_ROT)
#define VA_ROT_INTPD                 (0x00000024 + BASE_ROT + VA_ROT)
#define VA_ROT_INTCLR                (0x00000028 + BASE_ROT + VA_ROT)
#define VA_ROT_INCFG                 (0x00000030 + BASE_ROT + VA_ROT)
#define VA_ROT_INSIZE0               (0x00000040 + BASE_ROT + VA_ROT)
#define VA_ROT_INADD0                (0x00000044 + BASE_ROT + VA_ROT)
#define VA_ROT_INLNSTD0              (0x00000048 + BASE_ROT + VA_ROT)
#define VA_ROT_INSIZE1               (0x00000050 + BASE_ROT + VA_ROT)
#define VA_ROT_INADD1                (0x00000054 + BASE_ROT + VA_ROT)
#define VA_ROT_INLNSTD1              (0x00000058 + BASE_ROT + VA_ROT)
#define VA_ROT_INSIZE2               (0x00000060 + BASE_ROT + VA_ROT)
#define VA_ROT_INADD2                (0x00000064 + BASE_ROT + VA_ROT)
#define VA_ROT_INLNSTD2              (0x00000068 + BASE_ROT + VA_ROT)
#define VA_ROT_OUTCFG                (0x00000070 + BASE_ROT + VA_ROT)
#define VA_ROT_OUTSIZE0              (0x00000080 + BASE_ROT + VA_ROT)
#define VA_ROT_OUTADD0               (0x00000084 + BASE_ROT + VA_ROT)
#define VA_ROT_OUTLNSTD0             (0x00000088 + BASE_ROT + VA_ROT)
#define VA_ROT_OUTSIZE1              (0x00000090 + BASE_ROT + VA_ROT)
#define VA_ROT_OUTADD1               (0x00000094 + BASE_ROT + VA_ROT)
#define VA_ROT_OUTLNSTD1             (0x00000098 + BASE_ROT + VA_ROT)
#define VA_ROT_OUTSIZE2              (0x000000A0 + BASE_ROT + VA_ROT)
#define VA_ROT_OUTADD2               (0x000000A4 + BASE_ROT + VA_ROT)
#define VA_ROT_OUTLNSTD2             (0x000000A8 + BASE_ROT + VA_ROT)
#define VA_ROT_DMASCTL               (0x000000B0 + BASE_ROT + VA_ROT)
#define VA_ROT_DMADCTL               (0x000000B4 + BASE_ROT + VA_ROT)
#define VA_ROT_DMASRCCFG0            (0x000000B8 + BASE_ROT + VA_ROT)
#define VA_ROT_DMASRCCFG1            (0x000000BC + BASE_ROT + VA_ROT)
#define VA_ROT_DMADSTCFG0            (0x000000C0 + BASE_ROT + VA_ROT)
#define VA_ROT_DMADSTCFG1            (0x000000C4 + BASE_ROT + VA_ROT)
#define VA_ROT_DBG0                  (0x000000F0 + BASE_ROT + VA_ROT)

#define DATA_ROT_VER                 0x00001100
#define DATA_ROT_FTR                 0x00000000
#define DATA_ROT_CTL                 0x80000000
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
#define DATA_ROT_DMASCTL             0x01030003
#define DATA_ROT_DMADCTL             0x01030003
#define DATA_ROT_DMASRCCFG0          0x00030F00
#define DATA_ROT_DMASRCCFG1          0x00030003
#define DATA_ROT_DMADSTCFG0          0x00000F00
#define DATA_ROT_DMADSTCFG1          0x00000000
#define DATA_ROT_DBG0                0x0000FFFF

/* ROT Version Register */
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
} reg_rot_ver_t;

#define ROT_VER_COMP_0  0x0
#define ROT_VER_COMP_1  0x1

/* ROT Feature Register */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:32;   /**/
	} bits;
} reg_rot_ftr_t;

/* ROT Control Register */
typedef union {
	u32 val;
	struct {
	u32 en:1;       /**/
	u32 rsvd0:3;    /**/
	u32 bypass:1;   /**/
	u32 rsvd1:7;    /**/
	u32 dmassuspd:1;/**/
	u32 dmadsuspd:1;/**/
	u32 rsvd2:17;   /**/
	u32 rst:1;      /**/
	} bits;
} reg_rot_ctl_t;

#define ROT_CTL_EN_0            0x0
#define ROT_CTL_EN_1            0x1
#define ROT_CTL_BYPASS_0        0x0
#define ROT_CTL_BYPASS_1        0x1
#define ROT_CTL_DMASSUSPD_0     0x0
#define ROT_CTL_DMASSUSPD_1     0x1
#define ROT_CTL_DMADSUSPD_0     0x0
#define ROT_CTL_DMADSUSPD_1     0x1
#define ROT_CTL_RST_0           0x0
#define ROT_CTL_RST_1           0x1

/* ROT Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 mode:3;     /**/
	u32 rsvd0:3;    /**/
	u32 rsvd1:10;   /**/
	u32 bpp0:2;     /**/
	u32 rsvd2:2;    /**/
	u32 bpp1:2;     /**/
	u32 rsvd3:2;    /**/
	u32 bpp2:2;     /**/
	u32 rsvd4:2;    /**/
	u32 chsel:3;    /**/
	u32 rsvd5:1;    /**/
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
#define ROT_CFG_BPP0_0  0x0
#define ROT_CFG_BPP0_1  0x1
#define ROT_CFG_BPP0_2  0x2
#define ROT_CFG_BPP0_3  0x3

/* ROT Stream Contol Register */
typedef union {
	u32 val;
	struct {
	u32 start:1;    /**/
	u32 rsvd0:31;   /**/
	} bits;
} reg_rot_strmctl_t;

/* ROT Status Register */
typedef union {
	u32 val;
	struct {
	u32 insta:1;    /**/
	u32 rsvd0:3;    /**/
	u32 outsta:1;   /**/
	u32 rsvd1:27;   /**/
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
	u32 srcblk:1;   /**/
	u32 srctran:1;  /**/
	u32 dstblk:1;   /**/
	u32 dsttran:1;  /**/
	u32 srcsuspd:1; /**/
	u32 srcsuspdack:1;/**/
	u32 dstsuspd:1; /**/
	u32 dstsuspdack:1;/**/
	u32 srcdecerr:1;/**/
	u32 rsvd0:1;    /**/
	u32 dstdecerr:1;/**/
	u32 rsvd1:5;    /**/
	u32 tmout:1;    /**/
	u32 rsvd2:14;   /**/
	u32 fnsh:1;     /**/
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
#define ROT_INTEN_SRCSUSPD_0    0x0
#define ROT_INTEN_SRCSUSPD_1    0x1
#define ROT_INTEN_SRCSUSPDACK_0 0x0
#define ROT_INTEN_SRCSUSPDACK_1 0x1
#define ROT_INTEN_DSTSUSPD_0    0x0
#define ROT_INTEN_DSTSUSPD_1    0x1
#define ROT_INTEN_DSTSUSPDACK_0 0x0
#define ROT_INTEN_DSTSUSPDACK_1 0x1
#define ROT_INTEN_SRCDECERR_0   0x0
#define ROT_INTEN_SRCDECERR_1   0x1
#define ROT_INTEN_DSTDECERR_0   0x0
#define ROT_INTEN_DSTDECERR_1   0x1
#define ROT_INTEN_TMOUT_0       0x0
#define ROT_INTEN_TMOUT_1       0x1
#define ROT_INTEN_FNSH_0        0x0
#define ROT_INTEN_FNSH_1        0x1

/* ROT Interrupt Pending Register */
typedef union {
	u32 val;
	struct {
	u32 srcblk:1;   /**/
	u32 srctran:1;  /**/
	u32 dstblk:1;   /**/
	u32 dsttran:1;  /**/
	u32 srcsuspd:1; /**/
	u32 srcsuspdack:1;/**/
	u32 dstsuspd:1; /**/
	u32 dstsuspdack:1;/**/
	u32 srcdecerr:1;/**/
	u32 rsvd0:1;    /**/
	u32 dstdecerr:1;/**/
	u32 rsvd1:5;    /**/
	u32 tmout:1;    /**/
	u32 rsvd2:14;   /**/
	u32 fnsh:1;     /**/
	} bits;
} reg_rot_intpd_t;

/* ROT Interrupt Clear Register */
typedef union {
	u32 val;
	struct {
	u32 srcblk:1;   /**/
	u32 srctran:1;  /**/
	u32 dstblk:1;   /**/
	u32 dsttran:1;  /**/
	u32 srcsuspd:1; /**/
	u32 srcsuspdack:1;/**/
	u32 dstsuspd:1; /**/
	u32 dstsuspdack:1;/**/
	u32 srcdecerr:1;/**/
	u32 rsvd0:1;    /**/
	u32 dstdecerr:1;/**/
	u32 rsvd1:5;    /**/
	u32 tmout:1;    /**/
	u32 rsvd2:14;   /**/
	u32 fnsh:1;     /**/
	} bits;
} reg_rot_intclr_t;

/* ROT Input Configuration Register  */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:32;   /**/
	} bits;
} reg_rot_incfg_t;

/* ROT Input Buffer Size0 Register */
typedef union {
	u32 val;
	struct {
	u32 w:13;       /**/
	u32 rsvd0:3;    /**/
	u32 h:13;       /**/
	u32 rsvd1:3;    /**/
	} bits;
} reg_rot_insize0_t;

/* ROT Input Buffer Address0 Register */
typedef union {
	u32 val;
	struct {
	u32 add:32;/**/
	} bits;
} reg_rot_inadd0_t;

/* ROT Input Buffer Line Stride0 Register */
typedef union {
	u32 val;
	struct {
	u32 std:16;     /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_rot_inlnstd0_t;

/* ROT Input Buffer Size1 Register */
typedef union {
	u32 val;
	struct {
	u32 w:13;       /**/
	u32 rsvd0:3;    /**/
	u32 h:13;       /**/
	u32 rsvd1:3;    /**/
	} bits;
} reg_rot_insize1_t;

/* ROT Input Buffer Address1 Register */
typedef union {
	u32 val;
	struct {
	u32 add:32;/**/
	} bits;
} reg_rot_inadd1_t;

/* ROT Input Buffer Line Stride1 Register */
typedef union {
	u32 val;
	struct {
	u32 std:16;     /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_rot_inlnstd1_t;

/* ROT Input Buffer Size2 Register */
typedef union {
	u32 val;
	struct {
	u32 w:13;       /**/
	u32 rsvd0:3;    /**/
	u32 h:13;       /**/
	u32 rsvd1:3;    /**/
	} bits;
} reg_rot_insize2_t;

/* ROT Input Buffer Address2 Register */
typedef union {
	u32 val;
	struct {
	u32 add:32;/**/
	} bits;
} reg_rot_inadd2_t;

/* ROT Input Buffer Line Stride2 Register */
typedef union {
	u32 val;
	struct {
	u32 std:16;     /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_rot_inlnstd2_t;

/* ROT Output Configuration Register  */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:32;   /**/
	} bits;
} reg_rot_outcfg_t;

/* ROT Output Buffer Size0 Register */
typedef union {
	u32 val;
	struct {
	u32 w:13;       /**/
	u32 rsvd0:3;    /**/
	u32 h:13;       /**/
	u32 rsvd1:3;    /**/
	} bits;
} reg_rot_outsize0_t;

/* ROT Output Buffer Address0 Register */
typedef union {
	u32 val;
	struct {
	u32 add:32;/**/
	} bits;
} reg_rot_outadd0_t;

/* ROT Output Buffer Line Stride0 Register */
typedef union {
	u32 val;
	struct {
	u32 std:16;     /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_rot_outlnstd0_t;

/* ROT Output Buffer Size1 Register */
typedef union {
	u32 val;
	struct {
	u32 w:13;       /**/
	u32 rsvd0:3;    /**/
	u32 h:13;       /**/
	u32 rsvd1:3;    /**/
	} bits;
} reg_rot_outsize1_t;

/* ROT Output Buffer Address1 Register */
typedef union {
	u32 val;
	struct {
	u32 add:32;/**/
	} bits;
} reg_rot_outadd1_t;

/* ROT Output Buffer Line Stride1 Register */
typedef union {
	u32 val;
	struct {
	u32 std:16;     /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_rot_outlnstd1_t;

/* ROT Output Buffer Size2 Register */
typedef union {
	u32 val;
	struct {
	u32 w:13;       /**/
	u32 rsvd0:3;    /**/
	u32 h:13;       /**/
	u32 rsvd1:3;    /**/
	} bits;
} reg_rot_outsize2_t;

/* ROT Output Buffer Address2 Register */
typedef union {
	u32 val;
	struct {
	u32 add:32;/**/
	} bits;
} reg_rot_outadd2_t;

/* ROT Output Buffer Line Stride2 Register */
typedef union {
	u32 val;
	struct {
	u32 std:16;     /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_rot_outlnstd2_t;

/* ROT DMA Source Control Register */
typedef union {
	u32 val;
	struct {
	u32 bstsize:3;  /**/
	u32 rsvd0:5;    /**/
	u32 cache:4;    /**/
	u32 prot:3;     /**/
	u32 rsvd1:1;    /**/
	u32 bstlen:4;   /**/
	u32 rsvd2:4;    /**/
	u32 fixbstlen:1;/**/
	u32 rsvd3:7;    /**/
	} bits;
} reg_rot_dmasctl_t;

#define ROT_DMASCTL_FIXBSTLEN_0 0x0
#define ROT_DMASCTL_FIXBSTLEN_1 0x1

/* ROT DMA Destination Control Register */
typedef union {
	u32 val;
	struct {
	u32 bstsize:3;  /**/
	u32 rsvd0:5;    /**/
	u32 cache:4;    /**/
	u32 prot:3;     /**/
	u32 rsvd1:1;    /**/
	u32 bstlen:4;   /**/
	u32 rsvd2:4;    /**/
	u32 fixbstlen:1;/**/
	u32 rsvd3:7;    /**/
	} bits;
} reg_rot_dmadctl_t;

#define ROT_DMADCTL_FIXBSTLEN_0 0x0
#define ROT_DMADCTL_FIXBSTLEN_1 0x1

/* ROT DMA Source Configuration0 Register */
typedef union {
	u32 val;
	struct {
	u32 prio:1;     /**/
	u32 rpio2:3;    /**/
	u32 rsvd0:4;    /**/
	u32 osrlmt:4;   /**/
	u32 osrlmt2:4;  /**/
	u32 conlen0:16; /**/
	} bits;
} reg_rot_dmasrccfg0_t;

/* ROT DMA Source Configuration1 Register */
typedef union {
	u32 val;
	struct {
	u32 conlen1:16; /**/
	u32 conlen2:16; /**/
	} bits;
} reg_rot_dmasrccfg1_t;

/* ROT DMA Destination Configuration0 Register */
typedef union {
	u32 val;
	struct {
	u32 prio:1;     /**/
	u32 rpio2:3;    /**/
	u32 rsvd0:4;    /**/
	u32 osrlmt:4;   /**/
	u32 osrlmt2:4;  /**/
	u32 conlen0:16; /**/
	} bits;
} reg_rot_dmadstcfg0_t;

/* ROT DMA Destination Configuration1 Register */
typedef union {
	u32 val;
	struct {
	u32 conlen1:16; /**/
	u32 conlen2:16; /**/
	} bits;
} reg_rot_dmadstcfg1_t;

/* ROT Debug0 Register */
typedef union {
	u32 val;
	struct {
	u32 thr:16;/**/
	u32 tmr:16;/**/
	} bits;
} reg_rot_dbg0_t;

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
	reg_rot_dmasctl_t      dmasctl;    /* 00B0 */
	reg_rot_dmadctl_t      dmadctl;    /* 00B4 */
	reg_rot_dmasrccfg0_t   dmasrccfg0; /* 00B8 */
	reg_rot_dmasrccfg1_t   dmasrccfg1; /* 00BC */
	reg_rot_dmadstcfg0_t   dmadstcfg0; /* 00C0 */
	reg_rot_dmadstcfg1_t   dmadstcfg1; /* 00C4 */
	u32                    res10[10];
	reg_rot_dbg0_t         dbg0;       /* 00F0 */
} reg_rot_t;

#endif /* ___ROT___H___ */
