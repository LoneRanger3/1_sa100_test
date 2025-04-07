/* gtimer.h */

#ifndef ___GTIMER___H___
#define ___GTIMER___H___

#define BASE_GTIMER                0x04009000

#define VA_GTIMER_GTMR_VER                       (0x00000000 + BASE_GTIMER + VA_GTIMER)
#define VA_GTIMER_GTMR0_CLK_CTRL                 (0x00000010 + BASE_GTIMER + VA_GTIMER)
#define VA_GTIMER_GTMR0_STORE_VAL                (0x00000014 + BASE_GTIMER + VA_GTIMER)
#define VA_GTIMER_GTMR0_CUR_VAL                  (0x00000018 + BASE_GTIMER + VA_GTIMER)
#define VA_GTIMER_GTMR0_CTRL                     (0x0000001C + BASE_GTIMER + VA_GTIMER)
#define VA_GTIMER_GTMR0_INT_EN                   (0x00000020 + BASE_GTIMER + VA_GTIMER)
#define VA_GTIMER_GTMR0_CLR_INT_PENDING          (0x00000024 + BASE_GTIMER + VA_GTIMER)
#define VA_GTIMER_GTMR0_INT_PENDING              (0x00000028 + BASE_GTIMER + VA_GTIMER)
#define VA_GTIMER_GTMR1_CLK_CTRL                 (0x00000030 + BASE_GTIMER + VA_GTIMER)
#define VA_GTIMER_GTMR1_STORE_VAL                (0x00000034 + BASE_GTIMER + VA_GTIMER)
#define VA_GTIMER_GTMR1_CUR_VAL                  (0x00000038 + BASE_GTIMER + VA_GTIMER)
#define VA_GTIMER_GTMR1_CTRL                     (0x0000003C + BASE_GTIMER + VA_GTIMER)
#define VA_GTIMER_GTMR1_INT_EN                   (0x00000040 + BASE_GTIMER + VA_GTIMER)
#define VA_GTIMER_GTMR1_CLR_INT_PENDING          (0x00000044 + BASE_GTIMER + VA_GTIMER)
#define VA_GTIMER_GTMR1_INT_PENDING              (0x00000048 + BASE_GTIMER + VA_GTIMER)
#define VA_GTIMER_GTMR2_CLK_CTRL                 (0x00000050 + BASE_GTIMER + VA_GTIMER)
#define VA_GTIMER_GTMR2_STORE_VAL                (0x00000054 + BASE_GTIMER + VA_GTIMER)
#define VA_GTIMER_GTMR2_CUR_VAL                  (0x00000058 + BASE_GTIMER + VA_GTIMER)
#define VA_GTIMER_GTMR2_CTRL                     (0x0000005C + BASE_GTIMER + VA_GTIMER)
#define VA_GTIMER_GTMR2_INT_EN                   (0x00000060 + BASE_GTIMER + VA_GTIMER)
#define VA_GTIMER_GTMR2_CLR_INT_PENDING          (0x00000064 + BASE_GTIMER + VA_GTIMER)
#define VA_GTIMER_GTMR2_INT_PENDING              (0x00000068 + BASE_GTIMER + VA_GTIMER)
#define VA_GTIMER_GTMR3_CLK_CTRL                 (0x00000070 + BASE_GTIMER + VA_GTIMER)
#define VA_GTIMER_GTMR3_STORE_VAL                (0x00000074 + BASE_GTIMER + VA_GTIMER)
#define VA_GTIMER_GTMR3_CUR_VAL                  (0x00000078 + BASE_GTIMER + VA_GTIMER)
#define VA_GTIMER_GTMR3_CTRL                     (0x0000007C + BASE_GTIMER + VA_GTIMER)
#define VA_GTIMER_GTMR3_INT_EN                   (0x00000080 + BASE_GTIMER + VA_GTIMER)
#define VA_GTIMER_GTMR3_CLR_INT_PENDING          (0x00000084 + BASE_GTIMER + VA_GTIMER)
#define VA_GTIMER_GTMR3_INT_PENDING              (0x00000088 + BASE_GTIMER + VA_GTIMER)

#define DATA_GTIMER_GTMR_VER                         0x00001100
#define DATA_GTIMER_GTMR0_CLK_CTRL                   0x00000000
#define DATA_GTIMER_GTMR0_STORE_VAL                  0x00000000
#define DATA_GTIMER_GTMR0_CUR_VAL                    0x00000000
#define DATA_GTIMER_GTMR0_CTRL                       0x00000000
#define DATA_GTIMER_GTMR0_INT_EN                     0x00000000
#define DATA_GTIMER_GTMR0_CLR_INT_PENDING            0x00000000
#define DATA_GTIMER_GTMR0_INT_PENDING                0x00000000
#define DATA_GTIMER_GTMR1_CLK_CTRL                   0x00000000
#define DATA_GTIMER_GTMR1_STORE_VAL                  0x00000000
#define DATA_GTIMER_GTMR1_CUR_VAL                    0x00000000
#define DATA_GTIMER_GTMR1_CTRL                       0x00000000
#define DATA_GTIMER_GTMR1_INT_EN                     0x00000000
#define DATA_GTIMER_GTMR1_CLR_INT_PENDING            0x00000000
#define DATA_GTIMER_GTMR1_INT_PENDING                0x00000000
#define DATA_GTIMER_GTMR2_CLK_CTRL                   0x00000000
#define DATA_GTIMER_GTMR2_STORE_VAL                  0x00000000
#define DATA_GTIMER_GTMR2_CUR_VAL                    0x00000000
#define DATA_GTIMER_GTMR2_CTRL                       0x00000000
#define DATA_GTIMER_GTMR2_INT_EN                     0x00000000
#define DATA_GTIMER_GTMR2_CLR_INT_PENDING            0x00000000
#define DATA_GTIMER_GTMR2_INT_PENDING                0x00000000
#define DATA_GTIMER_GTMR3_CLK_CTRL                   0x00000000
#define DATA_GTIMER_GTMR3_STORE_VAL                  0x00000000
#define DATA_GTIMER_GTMR3_CUR_VAL                    0x00000000
#define DATA_GTIMER_GTMR3_CTRL                       0x00000000
#define DATA_GTIMER_GTMR3_INT_EN                     0x00000000
#define DATA_GTIMER_GTMR3_CLR_INT_PENDING            0x00000000
#define DATA_GTIMER_GTMR3_INT_PENDING                0x00000000

/* GTIMER Version Register */
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
} reg_gtimer_gtmr_ver_t;

/* GTIMER0 Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 clk_div:3;  /**/
	u32 clk_sel:2;  /**/
	u32 rsvd0:27;   /**/
	} bits;
} reg_gtimer_gtmr0_clk_ctrl_t;

#define GTIMER_GTMR0_CLK_CTRL_CLK_DIV_0 0x0
#define GTIMER_GTMR0_CLK_CTRL_CLK_DIV_1 0x1
#define GTIMER_GTMR0_CLK_CTRL_CLK_DIV_2 0x2
#define GTIMER_GTMR0_CLK_CTRL_CLK_DIV_3 0x3
#define GTIMER_GTMR0_CLK_CTRL_CLK_DIV_4 0x4
#define GTIMER_GTMR0_CLK_CTRL_CLK_DIV_5 0x5
#define GTIMER_GTMR0_CLK_CTRL_CLK_DIV_6 0x6
#define GTIMER_GTMR0_CLK_CTRL_CLK_SEL_0 0x0
#define GTIMER_GTMR0_CLK_CTRL_CLK_SEL_1 0x1

/* GTIMER0 Store Value Register */
typedef union {
	u32 val;
	struct {
	u32 store_val:32;/**/
	} bits;
} reg_gtimer_gtmr0_store_val_t;

/* GTIMER0 Current Value Register */
typedef union {
	u32 val;
	struct {
	u32 cur_val:32; /**/
	} bits;
} reg_gtimer_gtmr0_cur_val_t;

/* GTIMER0 Control Register */
typedef union {
	u32 val;
	struct {
	u32 enable:1;   /**/
	u32 mode:1;     /**/
	u32 direction:1;/**/
	u32 rsvd0:28;   /**/
	u32 nrdy:1;     /**/
	} bits;
} reg_gtimer_gtmr0_ctrl_t;

#define GTIMER_GTMR0_CTRL_ENABLE_0      0x0
#define GTIMER_GTMR0_CTRL_ENABLE_1      0x1
#define GTIMER_GTMR0_CTRL_MODE_0        0x0
#define GTIMER_GTMR0_CTRL_MODE_1        0x1
#define GTIMER_GTMR0_CTRL_DIRECTION_0   0x0
#define GTIMER_GTMR0_CTRL_DIRECTION_1   0x1
#define GTIMER_GTMR0_CTRL_NRDY_0        0x0
#define GTIMER_GTMR0_CTRL_NRDY_1        0x1

/* GTIMER0 Interrupt Enable Register */
typedef union {
	u32 val;
	struct {
	u32 enable:1;   /**/
	u32 rsvd0:31;   /**/
	} bits;
} reg_gtimer_gtmr0_int_en_t;

#define GTIMER_GTMR0_INT_EN_ENABLE_0    0x0
#define GTIMER_GTMR0_INT_EN_ENABLE_1    0x1

/* GTIMER0 Clear Interrupt Pending Register */
typedef union {
	u32 val;
	struct {
	u32 clr:1;      /**/
	u32 rsvd0:31;   /**/
	} bits;
} reg_gtimer_gtmr0_clr_int_pending_t;

#define GTIMER_GTMR0_CLR_INT_PENDING_CLR_0      0x0
#define GTIMER_GTMR0_CLR_INT_PENDING_CLR_1      0x1

/* GTIMER0 Interrupt Pending Register */
typedef union {
	u32 val;
	struct {
	u32 pending:1;  /**/
	u32 rsvd0:31;   /**/
	} bits;
} reg_gtimer_gtmr0_int_pending_t;

#define GTIMER_GTMR0_INT_PENDING_PENDING_0      0x0
#define GTIMER_GTMR0_INT_PENDING_PENDING_1      0x1

/* GTIMER1 Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 clk_div:3;  /**/
	u32 clk_sel:2;  /**/
	u32 rsvd0:27;   /**/
	} bits;
} reg_gtimer_gtmr1_clk_ctrl_t;

#define GTIMER_GTMR1_CLK_CTRL_CLK_DIV_0 0x0
#define GTIMER_GTMR1_CLK_CTRL_CLK_DIV_1 0x1
#define GTIMER_GTMR1_CLK_CTRL_CLK_DIV_2 0x2
#define GTIMER_GTMR1_CLK_CTRL_CLK_DIV_3 0x3
#define GTIMER_GTMR1_CLK_CTRL_CLK_DIV_4 0x4
#define GTIMER_GTMR1_CLK_CTRL_CLK_DIV_5 0x5
#define GTIMER_GTMR1_CLK_CTRL_CLK_DIV_6 0x6
#define GTIMER_GTMR1_CLK_CTRL_CLK_SEL_0 0x0
#define GTIMER_GTMR1_CLK_CTRL_CLK_SEL_1 0x1

/* GTIMER1 Store Value Register */
typedef union {
	u32 val;
	struct {
	u32 store_val:32;/**/
	} bits;
} reg_gtimer_gtmr1_store_val_t;

/* GTIMER1 Current Value Register */
typedef union {
	u32 val;
	struct {
	u32 cur_val:32; /**/
	} bits;
} reg_gtimer_gtmr1_cur_val_t;

/* GTIMER1 Control Register */
typedef union {
	u32 val;
	struct {
	u32 enable:1;   /**/
	u32 mode:1;     /**/
	u32 direction:1;/**/
	u32 rsvd0:28;   /**/
	u32 nrdy:1;     /**/
	} bits;
} reg_gtimer_gtmr1_ctrl_t;

#define GTIMER_GTMR1_CTRL_ENABLE_0      0x0
#define GTIMER_GTMR1_CTRL_ENABLE_1      0x1
#define GTIMER_GTMR1_CTRL_MODE_0        0x0
#define GTIMER_GTMR1_CTRL_MODE_1        0x1
#define GTIMER_GTMR1_CTRL_DIRECTION_0   0x0
#define GTIMER_GTMR1_CTRL_DIRECTION_1   0x1
#define GTIMER_GTMR1_CTRL_NRDY_0        0x0
#define GTIMER_GTMR1_CTRL_NRDY_1        0x1

/* GTIMER1 Interrupt Enable Register */
typedef union {
	u32 val;
	struct {
	u32 enable:1;   /**/
	u32 rsvd0:31;   /**/
	} bits;
} reg_gtimer_gtmr1_int_en_t;

#define GTIMER_GTMR1_INT_EN_ENABLE_0    0x0
#define GTIMER_GTMR1_INT_EN_ENABLE_1    0x1

/* GTIMER1 Clear Interrupt Pending Register */
typedef union {
	u32 val;
	struct {
	u32 clr:1;      /**/
	u32 rsvd0:31;   /**/
	} bits;
} reg_gtimer_gtmr1_clr_int_pending_t;

#define GTIMER_GTMR1_CLR_INT_PENDING_CLR_0      0x0
#define GTIMER_GTMR1_CLR_INT_PENDING_CLR_1      0x1

/* GTIMER1 Interrupt Pending Register */
typedef union {
	u32 val;
	struct {
	u32 pending:1;  /**/
	u32 rsvd0:31;   /**/
	} bits;
} reg_gtimer_gtmr1_int_pending_t;

#define GTIMER_GTMR1_INT_PENDING_PENDING_0      0x0
#define GTIMER_GTMR1_INT_PENDING_PENDING_1      0x1

/* GTIMER2 Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 clk_div:3;  /**/
	u32 clk_sel:2;  /**/
	u32 rsvd0:27;   /**/
	} bits;
} reg_gtimer_gtmr2_clk_ctrl_t;

#define GTIMER_GTMR2_CLK_CTRL_CLK_DIV_0 0x0
#define GTIMER_GTMR2_CLK_CTRL_CLK_DIV_1 0x1
#define GTIMER_GTMR2_CLK_CTRL_CLK_DIV_2 0x2
#define GTIMER_GTMR2_CLK_CTRL_CLK_DIV_3 0x3
#define GTIMER_GTMR2_CLK_CTRL_CLK_DIV_4 0x4
#define GTIMER_GTMR2_CLK_CTRL_CLK_DIV_5 0x5
#define GTIMER_GTMR2_CLK_CTRL_CLK_DIV_6 0x6
#define GTIMER_GTMR2_CLK_CTRL_CLK_SEL_0 0x0
#define GTIMER_GTMR2_CLK_CTRL_CLK_SEL_1 0x1

/* GTIMER2 Store Value Register */
typedef union {
	u32 val;
	struct {
	u32 store_val:32;/**/
	} bits;
} reg_gtimer_gtmr2_store_val_t;

/* GTIMER2 Current Value Register */
typedef union {
	u32 val;
	struct {
	u32 cur_val:32; /**/
	} bits;
} reg_gtimer_gtmr2_cur_val_t;

/* GTIMER2 Control Register */
typedef union {
	u32 val;
	struct {
	u32 enable:1;   /**/
	u32 mode:1;     /**/
	u32 direction:1;/**/
	u32 rsvd0:28;   /**/
	u32 nrdy:1;     /**/
	} bits;
} reg_gtimer_gtmr2_ctrl_t;

#define GTIMER_GTMR2_CTRL_ENABLE_0      0x0
#define GTIMER_GTMR2_CTRL_ENABLE_1      0x1
#define GTIMER_GTMR2_CTRL_MODE_0        0x0
#define GTIMER_GTMR2_CTRL_MODE_1        0x1
#define GTIMER_GTMR2_CTRL_DIRECTION_0   0x0
#define GTIMER_GTMR2_CTRL_DIRECTION_1   0x1
#define GTIMER_GTMR2_CTRL_NRDY_0        0x0
#define GTIMER_GTMR2_CTRL_NRDY_1        0x1

/* GTIMER2 Interrupt Enable Register */
typedef union {
	u32 val;
	struct {
	u32 enable:1;   /**/
	u32 rsvd0:31;   /**/
	} bits;
} reg_gtimer_gtmr2_int_en_t;

#define GTIMER_GTMR2_INT_EN_ENABLE_0    0x0
#define GTIMER_GTMR2_INT_EN_ENABLE_1    0x1

/* GTIMER2 Clear Interrupt Pending Register */
typedef union {
	u32 val;
	struct {
	u32 clr:1;      /**/
	u32 rsvd0:31;   /**/
	} bits;
} reg_gtimer_gtmr2_clr_int_pending_t;

#define GTIMER_GTMR2_CLR_INT_PENDING_CLR_0      0x0
#define GTIMER_GTMR2_CLR_INT_PENDING_CLR_1      0x1

/* GTIMER2 Interrupt Pending Register */
typedef union {
	u32 val;
	struct {
	u32 pending:1;  /**/
	u32 rsvd0:31;   /**/
	} bits;
} reg_gtimer_gtmr2_int_pending_t;

#define GTIMER_GTMR2_INT_PENDING_PENDING_0      0x0
#define GTIMER_GTMR2_INT_PENDING_PENDING_1      0x1

/* GTIMER3 Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 clk_div:3;  /**/
	u32 clk_sel:2;  /**/
	u32 rsvd0:27;   /**/
	} bits;
} reg_gtimer_gtmr3_clk_ctrl_t;

#define GTIMER_GTMR3_CLK_CTRL_CLK_DIV_0 0x0
#define GTIMER_GTMR3_CLK_CTRL_CLK_DIV_1 0x1
#define GTIMER_GTMR3_CLK_CTRL_CLK_DIV_2 0x2
#define GTIMER_GTMR3_CLK_CTRL_CLK_DIV_3 0x3
#define GTIMER_GTMR3_CLK_CTRL_CLK_DIV_4 0x4
#define GTIMER_GTMR3_CLK_CTRL_CLK_DIV_5 0x5
#define GTIMER_GTMR3_CLK_CTRL_CLK_DIV_6 0x6
#define GTIMER_GTMR3_CLK_CTRL_CLK_SEL_0 0x0
#define GTIMER_GTMR3_CLK_CTRL_CLK_SEL_1 0x1

/* GTIMER3 Store Value Register */
typedef union {
	u32 val;
	struct {
	u32 store_val:32;/**/
	} bits;
} reg_gtimer_gtmr3_store_val_t;

/* GTIMER3 Current Value Register */
typedef union {
	u32 val;
	struct {
	u32 cur_val:32; /**/
	} bits;
} reg_gtimer_gtmr3_cur_val_t;

/* GTIMER3 Control Register */
typedef union {
	u32 val;
	struct {
	u32 enable:1;   /**/
	u32 mode:1;     /**/
	u32 direction:1;/**/
	u32 rsvd0:28;   /**/
	u32 nrdy:1;     /**/
	} bits;
} reg_gtimer_gtmr3_ctrl_t;

#define GTIMER_GTMR3_CTRL_ENABLE_0      0x0
#define GTIMER_GTMR3_CTRL_ENABLE_1      0x1
#define GTIMER_GTMR3_CTRL_MODE_0        0x0
#define GTIMER_GTMR3_CTRL_MODE_1        0x1
#define GTIMER_GTMR3_CTRL_DIRECTION_0   0x0
#define GTIMER_GTMR3_CTRL_DIRECTION_1   0x1
#define GTIMER_GTMR3_CTRL_NRDY_0        0x0
#define GTIMER_GTMR3_CTRL_NRDY_1        0x1

/* GTIMER3 Interrupt Enable Register */
typedef union {
	u32 val;
	struct {
	u32 enable:1;   /**/
	u32 rsvd0:31;   /**/
	} bits;
} reg_gtimer_gtmr3_int_en_t;

#define GTIMER_GTMR3_INT_EN_ENABLE_0    0x0
#define GTIMER_GTMR3_INT_EN_ENABLE_1    0x1

/* GTIMER3 Clear Interrupt Pending Register */
typedef union {
	u32 val;
	struct {
	u32 clr:1;      /**/
	u32 rsvd0:31;   /**/
	} bits;
} reg_gtimer_gtmr3_clr_int_pending_t;

#define GTIMER_GTMR3_CLR_INT_PENDING_CLR_0      0x0
#define GTIMER_GTMR3_CLR_INT_PENDING_CLR_1      0x1

/* GTIMER3 Interrupt Pending Register */
typedef union {
	u32 val;
	struct {
	u32 pending:1;  /**/
	u32 rsvd0:31;   /**/
	} bits;
} reg_gtimer_gtmr3_int_pending_t;

#define GTIMER_GTMR3_INT_PENDING_PENDING_0      0x0
#define GTIMER_GTMR3_INT_PENDING_PENDING_1      0x1

/* reg_gtimer_t bank */
typedef struct tag_gtimer {
	reg_gtimer_gtmr_ver_t                  gtmr_ver;               /* 0000 */
	u32                                    res0[3];
	reg_gtimer_gtmr0_clk_ctrl_t            gtmr0_clk_ctrl;         /* 0010 */
	reg_gtimer_gtmr0_store_val_t           gtmr0_store_val;        /* 0014 */
	reg_gtimer_gtmr0_cur_val_t             gtmr0_cur_val;          /* 0018 */
	reg_gtimer_gtmr0_ctrl_t                gtmr0_ctrl;             /* 001C */
	reg_gtimer_gtmr0_int_en_t              gtmr0_int_en;           /* 0020 */
	reg_gtimer_gtmr0_clr_int_pending_t     gtmr0_clr_int_pending;  /* 0024 */
	reg_gtimer_gtmr0_int_pending_t         gtmr0_int_pending;      /* 0028 */
	u32                                    res1[1];
	reg_gtimer_gtmr1_clk_ctrl_t            gtmr1_clk_ctrl;         /* 0030 */
	reg_gtimer_gtmr1_store_val_t           gtmr1_store_val;        /* 0034 */
	reg_gtimer_gtmr1_cur_val_t             gtmr1_cur_val;          /* 0038 */
	reg_gtimer_gtmr1_ctrl_t                gtmr1_ctrl;             /* 003C */
	reg_gtimer_gtmr1_int_en_t              gtmr1_int_en;           /* 0040 */
	reg_gtimer_gtmr1_clr_int_pending_t     gtmr1_clr_int_pending;  /* 0044 */
	reg_gtimer_gtmr1_int_pending_t         gtmr1_int_pending;      /* 0048 */
	u32                                    res2[1];
	reg_gtimer_gtmr2_clk_ctrl_t            gtmr2_clk_ctrl;         /* 0050 */
	reg_gtimer_gtmr2_store_val_t           gtmr2_store_val;        /* 0054 */
	reg_gtimer_gtmr2_cur_val_t             gtmr2_cur_val;          /* 0058 */
	reg_gtimer_gtmr2_ctrl_t                gtmr2_ctrl;             /* 005C */
	reg_gtimer_gtmr2_int_en_t              gtmr2_int_en;           /* 0060 */
	reg_gtimer_gtmr2_clr_int_pending_t     gtmr2_clr_int_pending;  /* 0064 */
	reg_gtimer_gtmr2_int_pending_t         gtmr2_int_pending;      /* 0068 */
	u32                                    res3[1];
	reg_gtimer_gtmr3_clk_ctrl_t            gtmr3_clk_ctrl;         /* 0070 */
	reg_gtimer_gtmr3_store_val_t           gtmr3_store_val;        /* 0074 */
	reg_gtimer_gtmr3_cur_val_t             gtmr3_cur_val;          /* 0078 */
	reg_gtimer_gtmr3_ctrl_t                gtmr3_ctrl;             /* 007C */
	reg_gtimer_gtmr3_int_en_t              gtmr3_int_en;           /* 0080 */
	reg_gtimer_gtmr3_clr_int_pending_t     gtmr3_clr_int_pending;  /* 0084 */
	reg_gtimer_gtmr3_int_pending_t         gtmr3_int_pending;      /* 0088 */
} reg_gtimer_t;

#endif /* ___GTIMER___H___ */
