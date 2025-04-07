/* ptimer.h */

#ifndef ___PTIMER___H___
#define ___PTIMER___H___

#define BASE_PTIMER                0x01202000

#define VA_PTIMER_PTMR_VER                       (0x00000000 + BASE_PTIMER + VA_PTIMER)
#define VA_PTIMER_PTMR0_STORE_LOW_VAL            (0x00000010 + BASE_PTIMER + VA_PTIMER)
#define VA_PTIMER_PTMR0_STORE_HIGH_VAL           (0x00000014 + BASE_PTIMER + VA_PTIMER)
#define VA_PTIMER_PTMR0_CUR_LOW_VAL              (0x00000018 + BASE_PTIMER + VA_PTIMER)
#define VA_PTIMER_PTMR0_CUR_HIGH_VAL             (0x0000001C + BASE_PTIMER + VA_PTIMER)
#define VA_PTIMER_PTMR0_CTRL                     (0x00000020 + BASE_PTIMER + VA_PTIMER)
#define VA_PTIMER_PTMR0_INT_EN                   (0x00000024 + BASE_PTIMER + VA_PTIMER)
#define VA_PTIMER_PTMR0_CLR_INT_PENDING          (0x00000028 + BASE_PTIMER + VA_PTIMER)
#define VA_PTIMER_PTMR0_INT_PENDING              (0x0000002C + BASE_PTIMER + VA_PTIMER)
#define VA_PTIMER_PTMR1_STORE_LOW_VAL            (0x00000030 + BASE_PTIMER + VA_PTIMER)
#define VA_PTIMER_PTMR1_STORE_HIGH_VAL           (0x00000034 + BASE_PTIMER + VA_PTIMER)
#define VA_PTIMER_PTMR1_CUR_LOW_VAL              (0x00000038 + BASE_PTIMER + VA_PTIMER)
#define VA_PTIMER_PTMR1_CUR_HIGH_VAL             (0x0000003C + BASE_PTIMER + VA_PTIMER)
#define VA_PTIMER_PTMR1_CTRL                     (0x00000040 + BASE_PTIMER + VA_PTIMER)
#define VA_PTIMER_PTMR1_INT_EN                   (0x00000044 + BASE_PTIMER + VA_PTIMER)
#define VA_PTIMER_PTMR1_CLR_INT_PENDING          (0x00000048 + BASE_PTIMER + VA_PTIMER)
#define VA_PTIMER_PTMR1_INT_PENDING              (0x0000004C + BASE_PTIMER + VA_PTIMER)

#define DATA_PTIMER_PTMR_VER                         0x00001100
#define DATA_PTIMER_PTMR0_STORE_LOW_VAL              0x00000000
#define DATA_PTIMER_PTMR0_STORE_HIGH_VAL             0x00000000
#define DATA_PTIMER_PTMR0_CUR_LOW_VAL                0x00000000
#define DATA_PTIMER_PTMR0_CUR_HIGH_VAL               0x00000000
#define DATA_PTIMER_PTMR0_CTRL                       0x00000000
#define DATA_PTIMER_PTMR0_INT_EN                     0x00000000
#define DATA_PTIMER_PTMR0_CLR_INT_PENDING            0x00000000
#define DATA_PTIMER_PTMR0_INT_PENDING                0x00000000
#define DATA_PTIMER_PTMR1_STORE_LOW_VAL              0x00000000
#define DATA_PTIMER_PTMR1_STORE_HIGH_VAL             0x00000000
#define DATA_PTIMER_PTMR1_CUR_LOW_VAL                0x00000000
#define DATA_PTIMER_PTMR1_CUR_HIGH_VAL               0x00000000
#define DATA_PTIMER_PTMR1_CTRL                       0x00000000
#define DATA_PTIMER_PTMR1_INT_EN                     0x00000000
#define DATA_PTIMER_PTMR1_CLR_INT_PENDING            0x00000000
#define DATA_PTIMER_PTMR1_INT_PENDING                0x00000000

/* PTIMER Version Register */
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
} reg_ptimer_ptmr_ver_t;

/* PTIMER0 Store Low Value Register */
typedef union {
	u32 val;
	struct {
	u32 low:32;/**/
	} bits;
} reg_ptimer_ptmr0_store_low_val_t;

/* PTIMER0 Store High Value Register */
typedef union {
	u32 val;
	struct {
	u32 high:16;    /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_ptimer_ptmr0_store_high_val_t;

/* PTIMER0 Current Low Value Register */
typedef union {
	u32 val;
	struct {
	u32 low:32;/**/
	} bits;
} reg_ptimer_ptmr0_cur_low_val_t;

/* PTIMER0 Current High Value Register */
typedef union {
	u32 val;
	struct {
	u32 high:16;    /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_ptimer_ptmr0_cur_high_val_t;

/* PTIMER0 Control Register */
typedef union {
	u32 val;
	struct {
	u32 enable:1;   /**/
	u32 mode:1;     /**/
	u32 direction:1;/**/
	u32 rsvd0:1;    /**/
	u32 div:3;      /**/
	u32 rsvd1:25;   /**/
	} bits;
} reg_ptimer_ptmr0_ctrl_t;

#define PTIMER_PTMR0_CTRL_ENABLE_0      0x0
#define PTIMER_PTMR0_CTRL_ENABLE_1      0x1
#define PTIMER_PTMR0_CTRL_MODE_0        0x0
#define PTIMER_PTMR0_CTRL_MODE_1        0x1
#define PTIMER_PTMR0_CTRL_DIRECTION_0   0x0
#define PTIMER_PTMR0_CTRL_DIRECTION_1   0x1
#define PTIMER_PTMR0_CTRL_DIV_0         0x0
#define PTIMER_PTMR0_CTRL_DIV_1         0x1
#define PTIMER_PTMR0_CTRL_DIV_2         0x2
#define PTIMER_PTMR0_CTRL_DIV_3         0x3
#define PTIMER_PTMR0_CTRL_DIV_4         0x4
#define PTIMER_PTMR0_CTRL_DIV_5         0x5
#define PTIMER_PTMR0_CTRL_DIV_6         0x6

/* PTIMER0 Interrupt Enable Register */
typedef union {
	u32 val;
	struct {
	u32 enable:1;   /**/
	u32 rsvd0:31;   /**/
	} bits;
} reg_ptimer_ptmr0_int_en_t;

#define PTIMER_PTMR0_INT_EN_ENABLE_0    0x0
#define PTIMER_PTMR0_INT_EN_ENABLE_1    0x1

/* PTIMER0 Clear Interrupt Pending Register */
typedef union {
	u32 val;
	struct {
	u32 clr:1;      /**/
	u32 rsvd0:31;   /**/
	} bits;
} reg_ptimer_ptmr0_clr_int_pending_t;

#define PTIMER_PTMR0_CLR_INT_PENDING_CLR_0      0x0
#define PTIMER_PTMR0_CLR_INT_PENDING_CLR_1      0x1

/* PTIMER0 Interrupt Pending Register */
typedef union {
	u32 val;
	struct {
	u32 pending:1;  /**/
	u32 rsvd0:31;   /**/
	} bits;
} reg_ptimer_ptmr0_int_pending_t;

#define PTIMER_PTMR0_INT_PENDING_PENDING_0      0x0
#define PTIMER_PTMR0_INT_PENDING_PENDING_1      0x1

/* PTIMER1 Store Low Value Register */
typedef union {
	u32 val;
	struct {
	u32 low:32;/**/
	} bits;
} reg_ptimer_ptmr1_store_low_val_t;

/* PTIMER1 Store High Value Register */
typedef union {
	u32 val;
	struct {
	u32 high:16;    /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_ptimer_ptmr1_store_high_val_t;

/* PTIMER1 Current Low Value Register */
typedef union {
	u32 val;
	struct {
	u32 low:32;/**/
	} bits;
} reg_ptimer_ptmr1_cur_low_val_t;

/* PTIMER1 Current High Value Register */
typedef union {
	u32 val;
	struct {
	u32 high:16;    /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_ptimer_ptmr1_cur_high_val_t;

/* PTIMER1 Control Register */
typedef union {
	u32 val;
	struct {
	u32 enable:1;   /**/
	u32 mode:1;     /**/
	u32 direction:1;/**/
	u32 rsvd0:1;    /**/
	u32 div:3;      /**/
	u32 rsvd1:25;   /**/
	} bits;
} reg_ptimer_ptmr1_ctrl_t;

#define PTIMER_PTMR1_CTRL_ENABLE_0      0x0
#define PTIMER_PTMR1_CTRL_ENABLE_1      0x1
#define PTIMER_PTMR1_CTRL_MODE_0        0x0
#define PTIMER_PTMR1_CTRL_MODE_1        0x1
#define PTIMER_PTMR1_CTRL_DIRECTION_0   0x0
#define PTIMER_PTMR1_CTRL_DIRECTION_1   0x1
#define PTIMER_PTMR1_CTRL_DIV_0         0x0
#define PTIMER_PTMR1_CTRL_DIV_1         0x1
#define PTIMER_PTMR1_CTRL_DIV_2         0x2
#define PTIMER_PTMR1_CTRL_DIV_3         0x3
#define PTIMER_PTMR1_CTRL_DIV_4         0x4
#define PTIMER_PTMR1_CTRL_DIV_5         0x5
#define PTIMER_PTMR1_CTRL_DIV_6         0x6

/* PTIMER1 Interrupt Enable Register */
typedef union {
	u32 val;
	struct {
	u32 enable:1;   /**/
	u32 rsvd0:31;   /**/
	} bits;
} reg_ptimer_ptmr1_int_en_t;

#define PTIMER_PTMR1_INT_EN_ENABLE_0    0x0
#define PTIMER_PTMR1_INT_EN_ENABLE_1    0x1

/* PTIMER1 Clear Interrupt Pending Register */
typedef union {
	u32 val;
	struct {
	u32 clr:1;      /**/
	u32 rsvd0:31;   /**/
	} bits;
} reg_ptimer_ptmr1_clr_int_pending_t;

#define PTIMER_PTMR1_CLR_INT_PENDING_CLR_0      0x0
#define PTIMER_PTMR1_CLR_INT_PENDING_CLR_1      0x1

/* PTIMER1 Interrupt Pending Register */
typedef union {
	u32 val;
	struct {
	u32 pending:1;  /**/
	u32 rsvd0:31;   /**/
	} bits;
} reg_ptimer_ptmr1_int_pending_t;

#define PTIMER_PTMR1_INT_PENDING_PENDING_0      0x0
#define PTIMER_PTMR1_INT_PENDING_PENDING_1      0x1

/* reg_ptimer_t bank */
typedef struct tag_ptimer {
	reg_ptimer_ptmr_ver_t                  ptmr_ver;               /* 0000 */
	u32                                    res0[3];
	reg_ptimer_ptmr0_store_low_val_t       ptmr0_store_low_val;    /* 0010 */
	reg_ptimer_ptmr0_store_high_val_t      ptmr0_store_high_val;   /* 0014 */
	reg_ptimer_ptmr0_cur_low_val_t         ptmr0_cur_low_val;      /* 0018 */
	reg_ptimer_ptmr0_cur_high_val_t        ptmr0_cur_high_val;     /* 001C */
	reg_ptimer_ptmr0_ctrl_t                ptmr0_ctrl;             /* 0020 */
	reg_ptimer_ptmr0_int_en_t              ptmr0_int_en;           /* 0024 */
	reg_ptimer_ptmr0_clr_int_pending_t     ptmr0_clr_int_pending;  /* 0028 */
	reg_ptimer_ptmr0_int_pending_t         ptmr0_int_pending;      /* 002C */
	reg_ptimer_ptmr1_store_low_val_t       ptmr1_store_low_val;    /* 0030 */
	reg_ptimer_ptmr1_store_high_val_t      ptmr1_store_high_val;   /* 0034 */
	reg_ptimer_ptmr1_cur_low_val_t         ptmr1_cur_low_val;      /* 0038 */
	reg_ptimer_ptmr1_cur_high_val_t        ptmr1_cur_high_val;     /* 003C */
	reg_ptimer_ptmr1_ctrl_t                ptmr1_ctrl;             /* 0040 */
	reg_ptimer_ptmr1_int_en_t              ptmr1_int_en;           /* 0044 */
	reg_ptimer_ptmr1_clr_int_pending_t     ptmr1_clr_int_pending;  /* 0048 */
	reg_ptimer_ptmr1_int_pending_t         ptmr1_int_pending;      /* 004C */
} reg_ptimer_t;

#endif /* ___PTIMER___H___ */
