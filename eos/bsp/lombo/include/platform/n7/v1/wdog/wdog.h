/* wdog.h */

#ifndef ___WDOG___H___
#define ___WDOG___H___

#define BASE_WDOG                0x04009800

#define VA_WDOG_WDOG_VER                     (0x00000000 + BASE_WDOG + VA_WDOG)
#define VA_WDOG_WDOG_CLK_CTRL                (0x00000008 + BASE_WDOG + VA_WDOG)
#define VA_WDOG_WDOG_CTRL                    (0x00000010 + BASE_WDOG + VA_WDOG)
#define VA_WDOG_WDOG_TMRPERIOD               (0x00000014 + BASE_WDOG + VA_WDOG)
#define VA_WDOG_WDOG_CNT_CUR_VAL             (0x00000018 + BASE_WDOG + VA_WDOG)
#define VA_WDOG_WDOG_RESTART                 (0x0000001C + BASE_WDOG + VA_WDOG)
#define VA_WDOG_WDOG_INT_EN                  (0x00000020 + BASE_WDOG + VA_WDOG)
#define VA_WDOG_WDOG_CLR_INT_PENDING         (0x00000024 + BASE_WDOG + VA_WDOG)
#define VA_WDOG_WDOG_INT_PENDING             (0x00000028 + BASE_WDOG + VA_WDOG)

#define DATA_WDOG_WDOG_VER                       0x00001100
#define DATA_WDOG_WDOG_CLK_CTRL                  0x00000000
#define DATA_WDOG_WDOG_CTRL                      0x00000030
#define DATA_WDOG_WDOG_TMRPERIOD                 0x00000000
#define DATA_WDOG_WDOG_CNT_CUR_VAL               0x00000000
#define DATA_WDOG_WDOG_RESTART                   0x00000000
#define DATA_WDOG_WDOG_INT_EN                    0x00000000
#define DATA_WDOG_WDOG_CLR_INT_PENDING           0x00000000
#define DATA_WDOG_WDOG_INT_PENDING               0x00000000

/* WDOG Version Register */
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
} reg_wdog_wdog_ver_t;

/* WDOG Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 sel:1;      /**/
	u32 rsvd0:31;   /**/
	} bits;
} reg_wdog_wdog_clk_ctrl_t;

#define WDOG_WDOG_CLK_CTRL_SEL_0        0x0
#define WDOG_WDOG_CLK_CTRL_SEL_1        0x1

/* WDOG Control Register */
typedef union {
	u32 val;
	struct {
	u32 en:1;       /**/
	u32 rmod:1;     /**/
	u32 rsvd0:2;    /**/
	u32 rpl:3;      /**/
	u32 rsvd1:24;   /**/
	u32 su:1;       /**/
	} bits;
} reg_wdog_wdog_ctrl_t;

#define WDOG_WDOG_CTRL_EN_0     0x0
#define WDOG_WDOG_CTRL_EN_1     0x1
#define WDOG_WDOG_CTRL_RMOD_0   0x0
#define WDOG_WDOG_CTRL_RMOD_1   0x1
#define WDOG_WDOG_CTRL_RPL_0    0x0
#define WDOG_WDOG_CTRL_RPL_1    0x1
#define WDOG_WDOG_CTRL_RPL_2    0x2
#define WDOG_WDOG_CTRL_RPL_3    0x3
#define WDOG_WDOG_CTRL_RPL_4    0x4
#define WDOG_WDOG_CTRL_RPL_5    0x5
#define WDOG_WDOG_CTRL_RPL_6    0x6
#define WDOG_WDOG_CTRL_RPL_7    0x7
#define WDOG_WDOG_CTRL_SU_0     0x0
#define WDOG_WDOG_CTRL_SU_1     0x1

/* WDOG Timeout Period Register */
typedef union {
	u32 val;
	struct {
	u32 tmrperiod:4;/**/
	u32 rsvd0:28;   /**/
	} bits;
} reg_wdog_wdog_tmrperiod_t;

#define WDOG_WDOG_TMRPERIOD_TMRPERIOD_0         0x0
#define WDOG_WDOG_TMRPERIOD_TMRPERIOD_1         0x1
#define WDOG_WDOG_TMRPERIOD_TMRPERIOD_2         0x2
#define WDOG_WDOG_TMRPERIOD_TMRPERIOD_3         0x3
#define WDOG_WDOG_TMRPERIOD_TMRPERIOD_4         0x4
#define WDOG_WDOG_TMRPERIOD_TMRPERIOD_5         0x5
#define WDOG_WDOG_TMRPERIOD_TMRPERIOD_6         0x6
#define WDOG_WDOG_TMRPERIOD_TMRPERIOD_7         0x7
#define WDOG_WDOG_TMRPERIOD_TMRPERIOD_8         0x8
#define WDOG_WDOG_TMRPERIOD_TMRPERIOD_9         0x9
#define WDOG_WDOG_TMRPERIOD_TMRPERIOD_A         0xA
#define WDOG_WDOG_TMRPERIOD_TMRPERIOD_B         0xB
#define WDOG_WDOG_TMRPERIOD_TMRPERIOD_C         0xC
#define WDOG_WDOG_TMRPERIOD_TMRPERIOD_D         0xD
#define WDOG_WDOG_TMRPERIOD_TMRPERIOD_E         0xE
#define WDOG_WDOG_TMRPERIOD_TMRPERIOD_F         0xF

/* WDOG Counter Current Value Register */
typedef union {
	u32 val;
	struct {
	u32 cnt_cur_val:32;/**/
	} bits;
} reg_wdog_wdog_cnt_cur_val_t;

/* WDOG Restart Register */
typedef union {
	u32 val;
	struct {
	u32 restart:16; /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_wdog_wdog_restart_t;

/* WDOG Interrupt Enable Register */
typedef union {
	u32 val;
	struct {
	u32 enable:1;   /**/
	u32 rsvd0:31;   /**/
	} bits;
} reg_wdog_wdog_int_en_t;

#define WDOG_WDOG_INT_EN_ENABLE_0       0x0
#define WDOG_WDOG_INT_EN_ENABLE_1       0x1

/* WDOG Clear Interrupt Pending Register */
typedef union {
	u32 val;
	struct {
	u32 clr:1;      /**/
	u32 rsvd0:31;   /**/
	} bits;
} reg_wdog_wdog_clr_int_pending_t;

#define WDOG_WDOG_CLR_INT_PENDING_CLR_0         0x0
#define WDOG_WDOG_CLR_INT_PENDING_CLR_1         0x1

/* WDOG Interrupt Pending Register */
typedef union {
	u32 val;
	struct {
	u32 pending:1;  /**/
	u32 rsvd0:31;   /**/
	} bits;
} reg_wdog_wdog_int_pending_t;

#define WDOG_WDOG_INT_PENDING_PENDING_0 0x0
#define WDOG_WDOG_INT_PENDING_PENDING_1 0x1

/* reg_wdog_t bank */
typedef struct tag_wdog {
	reg_wdog_wdog_ver_t                wdog_ver;               /* 0000 */
	u32                                res0[1];
	reg_wdog_wdog_clk_ctrl_t           wdog_clk_ctrl;          /* 0008 */
	u32                                res1[1];
	reg_wdog_wdog_ctrl_t               wdog_ctrl;              /* 0010 */
	reg_wdog_wdog_tmrperiod_t          wdog_tmrperiod;         /* 0014 */
	reg_wdog_wdog_cnt_cur_val_t        wdog_cnt_cur_val;       /* 0018 */
	reg_wdog_wdog_restart_t            wdog_restart;           /* 001C */
	reg_wdog_wdog_int_en_t             wdog_int_en;            /* 0020 */
	reg_wdog_wdog_clr_int_pending_t    wdog_clr_int_pending;   /* 0024 */
	reg_wdog_wdog_int_pending_t        wdog_int_pending;       /* 0028 */
} reg_wdog_t;

#endif /* ___WDOG___H___ */
