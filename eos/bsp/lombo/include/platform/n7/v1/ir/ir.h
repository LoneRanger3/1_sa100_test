/* ir.h */

#ifndef ___IR___H___
#define ___IR___H___

#define BASE_IR                0x04014800

#define VA_IR_VER                (0x00000000 + BASE_IR + VA_IR)
#define VA_IR_FTR                (0x00000004 + BASE_IR + VA_IR)
#define VA_IR_CTL                (0x00000010 + BASE_IR + VA_IR)
#define VA_IR_TIMEOUT_TH         (0x00000014 + BASE_IR + VA_IR)
#define VA_IR_NOISE_TH           (0x00000018 + BASE_IR + VA_IR)
#define VA_IR_INT_EN             (0x0000001C + BASE_IR + VA_IR)
#define VA_IR_INT_PD             (0x00000020 + BASE_IR + VA_IR)
#define VA_IR_INT_CLR            (0x00000024 + BASE_IR + VA_IR)
#define VA_IR_DATA               (0x00000028 + BASE_IR + VA_IR)
#define VA_IR_DBG                (0x00000080 + BASE_IR + VA_IR)

#define DATA_IR_VER                  0x00001100
#define DATA_IR_FTR                  0x20030701
#define DATA_IR_CTL                  0x00000000
#define DATA_IR_TIMEOUT_TH           0x00000005
#define DATA_IR_NOISE_TH             0x00000000
#define DATA_IR_INT_EN               0x00000000
#define DATA_IR_INT_PD               0x00000000
#define DATA_IR_INT_CLR              0x00000000
#define DATA_IR_DATA                 0x00000000
#define DATA_IR_DBG                  0x00000000

/* IR Version Register */
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
} reg_ir_ver_t;

#define IR_VER_COMP_0   0x0
#define IR_VER_COMP_1   0x1

/* IR Feature Register */
typedef union {
	u32 val;
	struct {
	u32 nec_pro:1;  /**/
	u32 rsvd0:1;    /**/
	u32 rsvd1:6;    /**/
	u32 sirc_pro:1; /**/
	u32 sirc_ext_15:1;/**/
	u32 sirc_ext_20:1;/**/
	u32 rsvd2:5;    /**/
	u32 rc5_pro:1;  /**/
	u32 rc6_pro:1;  /**/
	u32 rsvd3:6;    /**/
	u32 ip_ver_l:5; /**/
	u32 ip_ver_h:3; /**/
	} bits;
} reg_ir_ftr_t;

#define IR_FTR_NEC_PRO_0        0x0
#define IR_FTR_NEC_PRO_1        0x1
#define IR_FTR_SIRC_PRO_0       0x0
#define IR_FTR_SIRC_PRO_1       0x1
#define IR_FTR_SIRC_EXT_15_0    0x0
#define IR_FTR_SIRC_EXT_15_1    0x1
#define IR_FTR_SIRC_EXT_20_0    0x0
#define IR_FTR_SIRC_EXT_20_1    0x1
#define IR_FTR_RC5_PRO_0        0x0
#define IR_FTR_RC5_PRO_1        0x1
#define IR_FTR_RC6_PRO_0        0x0
#define IR_FTR_RC6_PRO_1        0x1

/* IR Control Register */
typedef union {
	u32 val;
	struct {
	u32 clk_div:10;         /**/
	u32 rsvd0:6;            /**/
	u32 sirc_ext:2;         /**/
	u32 rsvd1:2;            /**/
	u32 rsvd2:1;            /**/
	u32 rsvd3:3;            /**/
	u32 rsvd4:1;            /**/
	u32 rsvd5:3;            /**/
	u32 protocol_sel:2;     /**/
	u32 ir_invert:1;        /**/
	u32 ir_en:1;            /**/
	} bits;
} reg_ir_ctl_t;

#define IR_CTL_SIRC_EXT_0       0x0
#define IR_CTL_SIRC_EXT_1       0x1
#define IR_CTL_SIRC_EXT_2       0x2
#define IR_CTL_SIRC_EXT_3       0x3
#define IR_CTL_PROTOCOL_SEL_0   0x0
#define IR_CTL_PROTOCOL_SEL_1   0x1
#define IR_CTL_PROTOCOL_SEL_2   0x2
#define IR_CTL_PROTOCOL_SEL_3   0x3
#define IR_CTL_IR_INVERT_0      0x0
#define IR_CTL_IR_INVERT_1      0x1
#define IR_CTL_IR_EN_0          0x0
#define IR_CTL_IR_EN_1          0x1

/* IR timer threshold Register */
typedef union {
	u32 val;
	struct {
	u32 thd:8;      /**/
	u32 rsvd0:24;   /**/
	} bits;
} reg_ir_timeout_th_t;

/* IR noise threshold Register */
typedef union {
	u32 val;
	struct {
	u32 thd:5;      /**/
	u32 rsvd0:27;   /**/
	} bits;
} reg_ir_noise_th_t;

/* IR interrupt enable Register */
typedef union {
	u32 val;
	struct {
	u32 ir_start:1; /**/
	u32 ir_frame:1; /**/
	u32 ir_repeat:1;/**/
	u32 rsvd0:1;    /**/
	u32 rsvd1:1;    /**/
	u32 rsvd2:1;    /**/
	u32 ir_f_err:1; /**/
	u32 ir_to:1;    /**/
	u32 rsvd3:24;   /**/
	} bits;
} reg_ir_int_en_t;

/* IR interrupt pending Register */
typedef union {
	u32 val;
	struct {
	u32 ir_start:1; /**/
	u32 ir_frame:1; /**/
	u32 ir_repeat:1;/**/
	u32 rsvd0:1;    /**/
	u32 rsvd1:1;    /**/
	u32 rsvd2:1;    /**/
	u32 ir_f_err:1; /**/
	u32 ir_to:1;    /**/
	u32 rsvd3:24;   /**/
	} bits;
} reg_ir_int_pd_t;

/* IR interrupt clear Register */
typedef union {
	u32 val;
	struct {
	u32 ir_start:1; /**/
	u32 ir_frame:1; /**/
	u32 ir_repeat:1;/**/
	u32 rsvd0:1;    /**/
	u32 rsvd1:1;    /**/
	u32 rsvd2:1;    /**/
	u32 ir_f_err:1; /**/
	u32 ir_to:1;    /**/
	u32 rsvd3:24;   /**/
	} bits;
} reg_ir_int_clr_t;

/* IR date Register */
typedef union {
	u32 val;
	struct {
	u32 data:32;    /**/
	} bits;
} reg_ir_data_t;

/* reg_ir_t bank */
typedef struct tag_ir {
	reg_ir_ver_t           ver;        /* 0000 */
	reg_ir_ftr_t           ftr;        /* 0004 */
	u32                    res0[2];
	reg_ir_ctl_t           ctl;        /* 0010 */
	reg_ir_timeout_th_t    timeout_th; /* 0014 */
	reg_ir_noise_th_t      noise_th;   /* 0018 */
	reg_ir_int_en_t        int_en;     /* 001C */
	reg_ir_int_pd_t        int_pd;     /* 0020 */
	reg_ir_int_clr_t       int_clr;    /* 0024 */
	reg_ir_data_t          data;       /* 0028 */
	u32                    res1[21];
	u32                    prv_0080;   /* 0080 */
} reg_ir_t;

#endif /* ___IR___H___ */
