/* pwm.h */

#ifndef ___PWM___H___
#define ___PWM___H___

#define BASE_PWM                0x0400D000

#define VA_PWM_VER                           (0x00000000 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM_CE                        (0x00000004 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM_PE                        (0x00000008 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM_INT_EN                    (0x00000010 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM_CLR_INT_PENDING           (0x00000014 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM_INT_PENDING               (0x00000018 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM0_CLK_CTRL            (0x00000040 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM1_CLK_CTRL            (0x00000080 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM2_CLK_CTRL            (0x000000C0 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM3_CLK_CTRL            (0x00000100 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM4_CLK_CTRL            (0x00000140 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM5_CLK_CTRL            (0x00000180 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM6_CLK_CTRL            (0x000001C0 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM7_CLK_CTRL            (0x00000200 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM8_CLK_CTRL            (0x00000240 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM9_CLK_CTRL            (0x00000280 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM0_CNT_STORE           (0x00000044 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM1_CNT_STORE           (0x00000084 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM2_CNT_STORE           (0x000000C4 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM3_CNT_STORE           (0x00000104 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM4_CNT_STORE           (0x00000144 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM5_CNT_STORE           (0x00000184 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM6_CNT_STORE           (0x000001C4 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM7_CNT_STORE           (0x00000204 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM8_CNT_STORE           (0x00000244 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM9_CNT_STORE           (0x00000284 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM0_CNT_CMP             (0x00000048 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM1_CNT_CMP             (0x00000088 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM2_CNT_CMP             (0x000000C8 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM3_CNT_CMP             (0x00000108 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM4_CNT_CMP             (0x00000148 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM5_CNT_CMP             (0x00000188 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM6_CNT_CMP             (0x000001C8 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM7_CNT_CMP             (0x00000208 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM8_CNT_CMP             (0x00000248 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM9_CNT_CMP             (0x00000288 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM0_CTRL                (0x0000004C + BASE_PWM + VA_PWM)
#define VA_PWM_PWM1_CTRL                (0x0000008C + BASE_PWM + VA_PWM)
#define VA_PWM_PWM2_CTRL                (0x000000CC + BASE_PWM + VA_PWM)
#define VA_PWM_PWM3_CTRL                (0x0000010C + BASE_PWM + VA_PWM)
#define VA_PWM_PWM4_CTRL                (0x0000014C + BASE_PWM + VA_PWM)
#define VA_PWM_PWM5_CTRL                (0x0000018C + BASE_PWM + VA_PWM)
#define VA_PWM_PWM6_CTRL                (0x000001CC + BASE_PWM + VA_PWM)
#define VA_PWM_PWM7_CTRL                (0x0000020C + BASE_PWM + VA_PWM)
#define VA_PWM_PWM8_CTRL                (0x0000024C + BASE_PWM + VA_PWM)
#define VA_PWM_PWM9_CTRL                (0x0000028C + BASE_PWM + VA_PWM)
#define VA_PWM_PWM0_CNT_CAP             (0x00000050 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM1_CNT_CAP             (0x00000090 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM2_CNT_CAP             (0x000000D0 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM3_CNT_CAP             (0x00000110 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM4_CNT_CAP             (0x00000150 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM5_CNT_CAP             (0x00000190 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM6_CNT_CAP             (0x000001D0 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM7_CNT_CAP             (0x00000210 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM8_CNT_CAP             (0x00000250 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM9_CNT_CAP             (0x00000290 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM0_IN_STS              (0x00000054 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM1_IN_STS              (0x00000094 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM2_IN_STS              (0x000000D4 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM3_IN_STS              (0x00000114 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM4_IN_STS              (0x00000154 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM5_IN_STS              (0x00000194 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM6_IN_STS              (0x000001D4 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM7_IN_STS              (0x00000214 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM8_IN_STS              (0x00000254 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM9_IN_STS              (0x00000294 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM0_DLY_CNT             (0x00000058 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM1_DLY_CNT             (0x00000098 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM2_DLY_CNT             (0x000000D8 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM3_DLY_CNT             (0x00000118 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM4_DLY_CNT             (0x00000158 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM5_DLY_CNT             (0x00000198 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM6_DLY_CNT             (0x000001D8 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM7_DLY_CNT             (0x00000218 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM8_DLY_CNT             (0x00000258 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM9_DLY_CNT             (0x00000298 + BASE_PWM + VA_PWM)
#define VA_PWM_PWM0_PULSE_CNT           (0x0000005C + BASE_PWM + VA_PWM)
#define VA_PWM_PWM1_PULSE_CNT           (0x0000009C + BASE_PWM + VA_PWM)
#define VA_PWM_PWM2_PULSE_CNT           (0x000000DC + BASE_PWM + VA_PWM)
#define VA_PWM_PWM3_PULSE_CNT           (0x0000011C + BASE_PWM + VA_PWM)
#define VA_PWM_PWM4_PULSE_CNT           (0x0000015C + BASE_PWM + VA_PWM)
#define VA_PWM_PWM5_PULSE_CNT           (0x0000019C + BASE_PWM + VA_PWM)
#define VA_PWM_PWM6_PULSE_CNT           (0x000001DC + BASE_PWM + VA_PWM)
#define VA_PWM_PWM7_PULSE_CNT           (0x0000021C + BASE_PWM + VA_PWM)
#define VA_PWM_PWM8_PULSE_CNT           (0x0000025C + BASE_PWM + VA_PWM)
#define VA_PWM_PWM9_PULSE_CNT           (0x0000029C + BASE_PWM + VA_PWM)

#define DATA_PWM_VER                         0x00001100
#define DATA_PWM_PWM_CE                      0x00000000
#define DATA_PWM_PWM_PE                      0x00000000
#define DATA_PWM_PWM_INT_EN                  0x00000000
#define DATA_PWM_PWM_CLR_INT_PENDING         0x00000000
#define DATA_PWM_PWM_INT_PENDING             0x00000000
#define DATA_PWM_PWM0_CLK_CTRL          0x00000000
#define DATA_PWM_PWM1_CLK_CTRL          0x00000000
#define DATA_PWM_PWM2_CLK_CTRL          0x00000000
#define DATA_PWM_PWM3_CLK_CTRL          0x00000000
#define DATA_PWM_PWM4_CLK_CTRL          0x00000000
#define DATA_PWM_PWM5_CLK_CTRL          0x00000000
#define DATA_PWM_PWM6_CLK_CTRL          0x00000000
#define DATA_PWM_PWM7_CLK_CTRL          0x00000000
#define DATA_PWM_PWM8_CLK_CTRL          0x00000000
#define DATA_PWM_PWM9_CLK_CTRL          0x00000000
#define DATA_PWM_PWM0_CNT_STORE         0x00000000
#define DATA_PWM_PWM1_CNT_STORE         0x00000000
#define DATA_PWM_PWM2_CNT_STORE         0x00000000
#define DATA_PWM_PWM3_CNT_STORE         0x00000000
#define DATA_PWM_PWM4_CNT_STORE         0x00000000
#define DATA_PWM_PWM5_CNT_STORE         0x00000000
#define DATA_PWM_PWM6_CNT_STORE         0x00000000
#define DATA_PWM_PWM7_CNT_STORE         0x00000000
#define DATA_PWM_PWM8_CNT_STORE         0x00000000
#define DATA_PWM_PWM9_CNT_STORE         0x00000000
#define DATA_PWM_PWM0_CNT_CMP           0x00000000
#define DATA_PWM_PWM1_CNT_CMP           0x00000000
#define DATA_PWM_PWM2_CNT_CMP           0x00000000
#define DATA_PWM_PWM3_CNT_CMP           0x00000000
#define DATA_PWM_PWM4_CNT_CMP           0x00000000
#define DATA_PWM_PWM5_CNT_CMP           0x00000000
#define DATA_PWM_PWM6_CNT_CMP           0x00000000
#define DATA_PWM_PWM7_CNT_CMP           0x00000000
#define DATA_PWM_PWM8_CNT_CMP           0x00000000
#define DATA_PWM_PWM9_CNT_CMP           0x00000000
#define DATA_PWM_PWM0_CTRL              0x00000000
#define DATA_PWM_PWM1_CTRL              0x00000000
#define DATA_PWM_PWM2_CTRL              0x00000000
#define DATA_PWM_PWM3_CTRL              0x00000000
#define DATA_PWM_PWM4_CTRL              0x00000000
#define DATA_PWM_PWM5_CTRL              0x00000000
#define DATA_PWM_PWM6_CTRL              0x00000000
#define DATA_PWM_PWM7_CTRL              0x00000000
#define DATA_PWM_PWM8_CTRL              0x00000000
#define DATA_PWM_PWM9_CTRL              0x00000000
#define DATA_PWM_PWM0_CNT_CAP           0x00000000
#define DATA_PWM_PWM1_CNT_CAP           0x00000000
#define DATA_PWM_PWM2_CNT_CAP           0x00000000
#define DATA_PWM_PWM3_CNT_CAP           0x00000000
#define DATA_PWM_PWM4_CNT_CAP           0x00000000
#define DATA_PWM_PWM5_CNT_CAP           0x00000000
#define DATA_PWM_PWM6_CNT_CAP           0x00000000
#define DATA_PWM_PWM7_CNT_CAP           0x00000000
#define DATA_PWM_PWM8_CNT_CAP           0x00000000
#define DATA_PWM_PWM9_CNT_CAP           0x00000000
#define DATA_PWM_PWM0_IN_STS            0x00000000
#define DATA_PWM_PWM1_IN_STS            0x00000000
#define DATA_PWM_PWM2_IN_STS            0x00000000
#define DATA_PWM_PWM3_IN_STS            0x00000000
#define DATA_PWM_PWM4_IN_STS            0x00000000
#define DATA_PWM_PWM5_IN_STS            0x00000000
#define DATA_PWM_PWM6_IN_STS            0x00000000
#define DATA_PWM_PWM7_IN_STS            0x00000000
#define DATA_PWM_PWM8_IN_STS            0x00000000
#define DATA_PWM_PWM9_IN_STS            0x00000000
#define DATA_PWM_PWM0_DLY_CNT           0x00000000
#define DATA_PWM_PWM1_DLY_CNT           0x00000000
#define DATA_PWM_PWM2_DLY_CNT           0x00000000
#define DATA_PWM_PWM3_DLY_CNT           0x00000000
#define DATA_PWM_PWM4_DLY_CNT           0x00000000
#define DATA_PWM_PWM5_DLY_CNT           0x00000000
#define DATA_PWM_PWM6_DLY_CNT           0x00000000
#define DATA_PWM_PWM7_DLY_CNT           0x00000000
#define DATA_PWM_PWM8_DLY_CNT           0x00000000
#define DATA_PWM_PWM9_DLY_CNT           0x00000000
#define DATA_PWM_PWM0_PULSE_CNT         0x00000000
#define DATA_PWM_PWM1_PULSE_CNT         0x00000000
#define DATA_PWM_PWM2_PULSE_CNT         0x00000000
#define DATA_PWM_PWM3_PULSE_CNT         0x00000000
#define DATA_PWM_PWM4_PULSE_CNT         0x00000000
#define DATA_PWM_PWM5_PULSE_CNT         0x00000000
#define DATA_PWM_PWM6_PULSE_CNT         0x00000000
#define DATA_PWM_PWM7_PULSE_CNT         0x00000000
#define DATA_PWM_PWM8_PULSE_CNT         0x00000000
#define DATA_PWM_PWM9_PULSE_CNT         0x00000000

/* PWM Version Register */
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
} reg_pwm_ver_t;

/* PWM Counter Register */
typedef union {
	u32 val;
	struct {
	u32 ce0:1;      /**/
	u32 ce1:1;      /**/
	u32 ce2:1;      /**/
	u32 ce3:1;      /**/
	u32 ce4:1;      /**/
	u32 ce5:1;      /**/
	u32 ce6:1;      /**/
	u32 ce7:1;      /**/
	u32 ce8:1;      /**/
	u32 ce9:1;      /**/
	u32 rsvd0:22;   /**/
	} bits;
} reg_pwm_pwm_ce_t;

#define PWM_PWM_CE_CE0_0        0x0
#define PWM_PWM_CE_CE0_1        0x1
#define PWM_PWM_CE_CE1_0        0x0
#define PWM_PWM_CE_CE1_1        0x1
#define PWM_PWM_CE_CE2_0        0x0
#define PWM_PWM_CE_CE2_1        0x1
#define PWM_PWM_CE_CE3_0        0x0
#define PWM_PWM_CE_CE3_1        0x1
#define PWM_PWM_CE_CE4_0        0x0
#define PWM_PWM_CE_CE4_1        0x1
#define PWM_PWM_CE_CE5_0        0x0
#define PWM_PWM_CE_CE5_1        0x1
#define PWM_PWM_CE_CE6_0        0x0
#define PWM_PWM_CE_CE6_1        0x1
#define PWM_PWM_CE_CE7_0        0x0
#define PWM_PWM_CE_CE7_1        0x1
#define PWM_PWM_CE_CE8_0        0x0
#define PWM_PWM_CE_CE8_1        0x1
#define PWM_PWM_CE_CE9_0        0x0
#define PWM_PWM_CE_CE9_1        0x1

/* PWM Pulse Enable Register */
typedef union {
	u32 val;
	struct {
	u32 pe0:1;      /**/
	u32 pe1:1;      /**/
	u32 pe2:1;      /**/
	u32 pe3:1;      /**/
	u32 pe4:1;      /**/
	u32 pe5:1;      /**/
	u32 pe6:1;      /**/
	u32 pe7:1;      /**/
	u32 pe8:1;      /**/
	u32 pe9:1;      /**/
	u32 rsvd0:22;   /**/
	} bits;
} reg_pwm_pwm_pe_t;

#define PWM_PWM_PE_PE0_0        0x0
#define PWM_PWM_PE_PE0_1        0x1
#define PWM_PWM_PE_PE1_0        0x0
#define PWM_PWM_PE_PE1_1        0x1
#define PWM_PWM_PE_PE2_0        0x0
#define PWM_PWM_PE_PE2_1        0x1
#define PWM_PWM_PE_PE3_0        0x0
#define PWM_PWM_PE_PE3_1        0x1
#define PWM_PWM_PE_PE4_0        0x0
#define PWM_PWM_PE_PE4_1        0x1
#define PWM_PWM_PE_PE5_0        0x0
#define PWM_PWM_PE_PE5_1        0x1
#define PWM_PWM_PE_PE6_0        0x0
#define PWM_PWM_PE_PE6_1        0x1
#define PWM_PWM_PE_PE7_0        0x0
#define PWM_PWM_PE_PE7_1        0x1
#define PWM_PWM_PE_PE8_0        0x0
#define PWM_PWM_PE_PE8_1        0x1
#define PWM_PWM_PE_PE9_0        0x0
#define PWM_PWM_PE_PE9_1        0x1

/* PWM Interrupt Enable Register */
typedef union {
	u32 val;
	struct {
	u32 en0:1;      /**/
	u32 en1:1;      /**/
	u32 en2:1;      /**/
	u32 en3:1;      /**/
	u32 en4:1;      /**/
	u32 en5:1;      /**/
	u32 en6:1;      /**/
	u32 en7:1;      /**/
	u32 en8:1;      /**/
	u32 en9:1;      /**/
	u32 rsvd0:22;   /**/
	} bits;
} reg_pwm_pwm_int_en_t;

#define PWM_PWM_INT_EN_EN0_0    0x0
#define PWM_PWM_INT_EN_EN0_1    0x1
#define PWM_PWM_INT_EN_EN1_0    0x0
#define PWM_PWM_INT_EN_EN1_1    0x1
#define PWM_PWM_INT_EN_EN2_0    0x0
#define PWM_PWM_INT_EN_EN2_1    0x1
#define PWM_PWM_INT_EN_EN3_0    0x0
#define PWM_PWM_INT_EN_EN3_1    0x1
#define PWM_PWM_INT_EN_EN4_0    0x0
#define PWM_PWM_INT_EN_EN4_1    0x1
#define PWM_PWM_INT_EN_EN5_0    0x0
#define PWM_PWM_INT_EN_EN5_1    0x1
#define PWM_PWM_INT_EN_EN6_0    0x0
#define PWM_PWM_INT_EN_EN6_1    0x1
#define PWM_PWM_INT_EN_EN7_0    0x0
#define PWM_PWM_INT_EN_EN7_1    0x1
#define PWM_PWM_INT_EN_EN8_0    0x0
#define PWM_PWM_INT_EN_EN8_1    0x1
#define PWM_PWM_INT_EN_EN9_0    0x0
#define PWM_PWM_INT_EN_EN9_1    0x1

/* PWM Clear Interrupt Pending Register */
typedef union {
	u32 val;
	struct {
	u32 clr0:1;     /**/
	u32 clr1:1;     /**/
	u32 clr2:1;     /**/
	u32 clr3:1;     /**/
	u32 clr4:1;     /**/
	u32 clr5:1;     /**/
	u32 clr6:1;     /**/
	u32 clr7:1;     /**/
	u32 clr8:1;     /**/
	u32 clr9:1;     /**/
	u32 rsvd0:22;   /**/
	} bits;
} reg_pwm_pwm_clr_int_pending_t;

#define PWM_PWM_CLR_INT_PENDING_CLR0_0  0x0
#define PWM_PWM_CLR_INT_PENDING_CLR0_1  0x1
#define PWM_PWM_CLR_INT_PENDING_CLR1_0  0x0
#define PWM_PWM_CLR_INT_PENDING_CLR1_1  0x1
#define PWM_PWM_CLR_INT_PENDING_CLR2_0  0x0
#define PWM_PWM_CLR_INT_PENDING_CLR2_1  0x1
#define PWM_PWM_CLR_INT_PENDING_CLR3_0  0x0
#define PWM_PWM_CLR_INT_PENDING_CLR3_1  0x1
#define PWM_PWM_CLR_INT_PENDING_CLR4_0  0x0
#define PWM_PWM_CLR_INT_PENDING_CLR4_1  0x1
#define PWM_PWM_CLR_INT_PENDING_CLR5_0  0x0
#define PWM_PWM_CLR_INT_PENDING_CLR5_1  0x1
#define PWM_PWM_CLR_INT_PENDING_CLR6_0  0x0
#define PWM_PWM_CLR_INT_PENDING_CLR6_1  0x1
#define PWM_PWM_CLR_INT_PENDING_CLR7_0  0x0
#define PWM_PWM_CLR_INT_PENDING_CLR7_1  0x1
#define PWM_PWM_CLR_INT_PENDING_CLR8_0  0x0
#define PWM_PWM_CLR_INT_PENDING_CLR8_1  0x1
#define PWM_PWM_CLR_INT_PENDING_CLR9_0  0x0
#define PWM_PWM_CLR_INT_PENDING_CLR9_1  0x1

/* PWM Interrupt Pending Register */
typedef union {
	u32 val;
	struct {
	u32 pending0:1; /**/
	u32 pending1:1; /**/
	u32 pending2:1; /**/
	u32 pending3:1; /**/
	u32 pending4:1; /**/
	u32 pending5:1; /**/
	u32 pending6:1; /**/
	u32 pending7:1; /**/
	u32 pending8:1; /**/
	u32 pending9:1; /**/
	u32 rsvd0:22;   /**/
	} bits;
} reg_pwm_pwm_int_pending_t;

#define PWM_PWM_INT_PENDING_PENDING0_0  0x0
#define PWM_PWM_INT_PENDING_PENDING0_1  0x1
#define PWM_PWM_INT_PENDING_PENDING1_0  0x0
#define PWM_PWM_INT_PENDING_PENDING1_1  0x1
#define PWM_PWM_INT_PENDING_PENDING2_0  0x0
#define PWM_PWM_INT_PENDING_PENDING2_1  0x1
#define PWM_PWM_INT_PENDING_PENDING3_0  0x0
#define PWM_PWM_INT_PENDING_PENDING3_1  0x1
#define PWM_PWM_INT_PENDING_PENDING4_0  0x0
#define PWM_PWM_INT_PENDING_PENDING4_1  0x1
#define PWM_PWM_INT_PENDING_PENDING5_0  0x0
#define PWM_PWM_INT_PENDING_PENDING5_1  0x1
#define PWM_PWM_INT_PENDING_PENDING6_0  0x0
#define PWM_PWM_INT_PENDING_PENDING6_1  0x1
#define PWM_PWM_INT_PENDING_PENDING7_0  0x0
#define PWM_PWM_INT_PENDING_PENDING7_1  0x1
#define PWM_PWM_INT_PENDING_PENDING8_0  0x0
#define PWM_PWM_INT_PENDING_PENDING8_1  0x1
#define PWM_PWM_INT_PENDING_PENDING9_0  0x0
#define PWM_PWM_INT_PENDING_PENDING9_1  0x1

/* PWM[CH] Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 div:8;      /**/
	u32 src_sel:1;  /**/
	u32 gat:1;      /**/
	u32 bypass:1;   /**/
	u32 rsvd0:21;   /**/
	} bits;
} reg_pwm_pwm0_clk_ctrl_t;

#define PWM_PWM0_CLK_CTRL_SRC_SEL_0     0x0
#define PWM_PWM0_CLK_CTRL_SRC_SEL_1     0x1
#define PWM_PWM0_CLK_CTRL_GAT_0         0x0
#define PWM_PWM0_CLK_CTRL_GAT_1         0x1
#define PWM_PWM0_CLK_CTRL_BYPASS_0      0x0
#define PWM_PWM0_CLK_CTRL_BYPASS_1      0x1

/* PWM[CH] Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 div:8;      /**/
	u32 src_sel:1;  /**/
	u32 gat:1;      /**/
	u32 bypass:1;   /**/
	u32 rsvd0:21;   /**/
	} bits;
} reg_pwm_pwm1_clk_ctrl_t;

#define PWM_PWM1_CLK_CTRL_SRC_SEL_0     0x0
#define PWM_PWM1_CLK_CTRL_SRC_SEL_1     0x1
#define PWM_PWM1_CLK_CTRL_GAT_0         0x0
#define PWM_PWM1_CLK_CTRL_GAT_1         0x1
#define PWM_PWM1_CLK_CTRL_BYPASS_0      0x0
#define PWM_PWM1_CLK_CTRL_BYPASS_1      0x1

/* PWM[CH] Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 div:8;      /**/
	u32 src_sel:1;  /**/
	u32 gat:1;      /**/
	u32 bypass:1;   /**/
	u32 rsvd0:21;   /**/
	} bits;
} reg_pwm_pwm2_clk_ctrl_t;

#define PWM_PWM2_CLK_CTRL_SRC_SEL_0     0x0
#define PWM_PWM2_CLK_CTRL_SRC_SEL_1     0x1
#define PWM_PWM2_CLK_CTRL_GAT_0         0x0
#define PWM_PWM2_CLK_CTRL_GAT_1         0x1
#define PWM_PWM2_CLK_CTRL_BYPASS_0      0x0
#define PWM_PWM2_CLK_CTRL_BYPASS_1      0x1

/* PWM[CH] Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 div:8;      /**/
	u32 src_sel:1;  /**/
	u32 gat:1;      /**/
	u32 bypass:1;   /**/
	u32 rsvd0:21;   /**/
	} bits;
} reg_pwm_pwm3_clk_ctrl_t;

#define PWM_PWM3_CLK_CTRL_SRC_SEL_0     0x0
#define PWM_PWM3_CLK_CTRL_SRC_SEL_1     0x1
#define PWM_PWM3_CLK_CTRL_GAT_0         0x0
#define PWM_PWM3_CLK_CTRL_GAT_1         0x1
#define PWM_PWM3_CLK_CTRL_BYPASS_0      0x0
#define PWM_PWM3_CLK_CTRL_BYPASS_1      0x1

/* PWM[CH] Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 div:8;      /**/
	u32 src_sel:1;  /**/
	u32 gat:1;      /**/
	u32 bypass:1;   /**/
	u32 rsvd0:21;   /**/
	} bits;
} reg_pwm_pwm4_clk_ctrl_t;

#define PWM_PWM4_CLK_CTRL_SRC_SEL_0     0x0
#define PWM_PWM4_CLK_CTRL_SRC_SEL_1     0x1
#define PWM_PWM4_CLK_CTRL_GAT_0         0x0
#define PWM_PWM4_CLK_CTRL_GAT_1         0x1
#define PWM_PWM4_CLK_CTRL_BYPASS_0      0x0
#define PWM_PWM4_CLK_CTRL_BYPASS_1      0x1

/* PWM[CH] Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 div:8;      /**/
	u32 src_sel:1;  /**/
	u32 gat:1;      /**/
	u32 bypass:1;   /**/
	u32 rsvd0:21;   /**/
	} bits;
} reg_pwm_pwm5_clk_ctrl_t;

#define PWM_PWM5_CLK_CTRL_SRC_SEL_0     0x0
#define PWM_PWM5_CLK_CTRL_SRC_SEL_1     0x1
#define PWM_PWM5_CLK_CTRL_GAT_0         0x0
#define PWM_PWM5_CLK_CTRL_GAT_1         0x1
#define PWM_PWM5_CLK_CTRL_BYPASS_0      0x0
#define PWM_PWM5_CLK_CTRL_BYPASS_1      0x1

/* PWM[CH] Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 div:8;      /**/
	u32 src_sel:1;  /**/
	u32 gat:1;      /**/
	u32 bypass:1;   /**/
	u32 rsvd0:21;   /**/
	} bits;
} reg_pwm_pwm6_clk_ctrl_t;

#define PWM_PWM6_CLK_CTRL_SRC_SEL_0     0x0
#define PWM_PWM6_CLK_CTRL_SRC_SEL_1     0x1
#define PWM_PWM6_CLK_CTRL_GAT_0         0x0
#define PWM_PWM6_CLK_CTRL_GAT_1         0x1
#define PWM_PWM6_CLK_CTRL_BYPASS_0      0x0
#define PWM_PWM6_CLK_CTRL_BYPASS_1      0x1

/* PWM[CH] Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 div:8;      /**/
	u32 src_sel:1;  /**/
	u32 gat:1;      /**/
	u32 bypass:1;   /**/
	u32 rsvd0:21;   /**/
	} bits;
} reg_pwm_pwm7_clk_ctrl_t;

#define PWM_PWM7_CLK_CTRL_SRC_SEL_0     0x0
#define PWM_PWM7_CLK_CTRL_SRC_SEL_1     0x1
#define PWM_PWM7_CLK_CTRL_GAT_0         0x0
#define PWM_PWM7_CLK_CTRL_GAT_1         0x1
#define PWM_PWM7_CLK_CTRL_BYPASS_0      0x0
#define PWM_PWM7_CLK_CTRL_BYPASS_1      0x1

/* PWM[CH] Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 div:8;      /**/
	u32 src_sel:1;  /**/
	u32 gat:1;      /**/
	u32 bypass:1;   /**/
	u32 rsvd0:21;   /**/
	} bits;
} reg_pwm_pwm8_clk_ctrl_t;

#define PWM_PWM8_CLK_CTRL_SRC_SEL_0     0x0
#define PWM_PWM8_CLK_CTRL_SRC_SEL_1     0x1
#define PWM_PWM8_CLK_CTRL_GAT_0         0x0
#define PWM_PWM8_CLK_CTRL_GAT_1         0x1
#define PWM_PWM8_CLK_CTRL_BYPASS_0      0x0
#define PWM_PWM8_CLK_CTRL_BYPASS_1      0x1

/* PWM[CH] Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 div:8;      /**/
	u32 src_sel:1;  /**/
	u32 gat:1;      /**/
	u32 bypass:1;   /**/
	u32 rsvd0:21;   /**/
	} bits;
} reg_pwm_pwm9_clk_ctrl_t;

#define PWM_PWM9_CLK_CTRL_SRC_SEL_0     0x0
#define PWM_PWM9_CLK_CTRL_SRC_SEL_1     0x1
#define PWM_PWM9_CLK_CTRL_GAT_0         0x0
#define PWM_PWM9_CLK_CTRL_GAT_1         0x1
#define PWM_PWM9_CLK_CTRL_BYPASS_0      0x0
#define PWM_PWM9_CLK_CTRL_BYPASS_1      0x1

/* PWM[CH] Counter Store Register */
typedef union {
	u32 val;
	struct {
	u32 store:16;   /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_pwm_pwm0_cnt_store_t;

/* PWM[CH] Counter Store Register */
typedef union {
	u32 val;
	struct {
	u32 store:16;   /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_pwm_pwm1_cnt_store_t;

/* PWM[CH] Counter Store Register */
typedef union {
	u32 val;
	struct {
	u32 store:16;   /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_pwm_pwm2_cnt_store_t;

/* PWM[CH] Counter Store Register */
typedef union {
	u32 val;
	struct {
	u32 store:16;   /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_pwm_pwm3_cnt_store_t;

/* PWM[CH] Counter Store Register */
typedef union {
	u32 val;
	struct {
	u32 store:16;   /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_pwm_pwm4_cnt_store_t;

/* PWM[CH] Counter Store Register */
typedef union {
	u32 val;
	struct {
	u32 store:16;   /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_pwm_pwm5_cnt_store_t;

/* PWM[CH] Counter Store Register */
typedef union {
	u32 val;
	struct {
	u32 store:16;   /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_pwm_pwm6_cnt_store_t;

/* PWM[CH] Counter Store Register */
typedef union {
	u32 val;
	struct {
	u32 store:16;   /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_pwm_pwm7_cnt_store_t;

/* PWM[CH] Counter Store Register */
typedef union {
	u32 val;
	struct {
	u32 store:16;   /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_pwm_pwm8_cnt_store_t;

/* PWM[CH] Counter Store Register */
typedef union {
	u32 val;
	struct {
	u32 store:16;   /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_pwm_pwm9_cnt_store_t;

/* PWM[CH] Counter Compare Register */
typedef union {
	u32 val;
	struct {
	u32 cmp:16;     /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_pwm_pwm0_cnt_cmp_t;

/* PWM[CH] Counter Compare Register */
typedef union {
	u32 val;
	struct {
	u32 cmp:16;     /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_pwm_pwm1_cnt_cmp_t;

/* PWM[CH] Counter Compare Register */
typedef union {
	u32 val;
	struct {
	u32 cmp:16;     /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_pwm_pwm2_cnt_cmp_t;

/* PWM[CH] Counter Compare Register */
typedef union {
	u32 val;
	struct {
	u32 cmp:16;     /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_pwm_pwm3_cnt_cmp_t;

/* PWM[CH] Counter Compare Register */
typedef union {
	u32 val;
	struct {
	u32 cmp:16;     /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_pwm_pwm4_cnt_cmp_t;

/* PWM[CH] Counter Compare Register */
typedef union {
	u32 val;
	struct {
	u32 cmp:16;     /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_pwm_pwm5_cnt_cmp_t;

/* PWM[CH] Counter Compare Register */
typedef union {
	u32 val;
	struct {
	u32 cmp:16;     /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_pwm_pwm6_cnt_cmp_t;

/* PWM[CH] Counter Compare Register */
typedef union {
	u32 val;
	struct {
	u32 cmp:16;     /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_pwm_pwm7_cnt_cmp_t;

/* PWM[CH] Counter Compare Register */
typedef union {
	u32 val;
	struct {
	u32 cmp:16;     /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_pwm_pwm8_cnt_cmp_t;

/* PWM[CH] Counter Compare Register */
typedef union {
	u32 val;
	struct {
	u32 cmp:16;     /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_pwm_pwm9_cnt_cmp_t;

/* PWM[CH] Control Register */
typedef union {
	u32 val;
	struct {
	u32 dt:8;       /**/
	u32 dt_en:1;    /**/
	u32 rsvd0:5;    /**/
	u32 mod:2;      /**/
	u32 rsvd1:1;    /**/
	u32 p_oe:1;     /**/
	u32 p_pol:1;    /**/
	u32 n_oe:1;     /**/
	u32 n_pol:1;    /**/
	u32 rsvd2:3;    /**/
	u32 ren:1;      /**/
	u32 fen:1;      /**/
	u32 rsvd3:4;    /**/
	u32 p_rev:1;    /**/
	u32 p_ie:1;     /**/
	} bits;
} reg_pwm_pwm0_ctrl_t;

#define PWM_PWM0_CTRL_DT_EN_0   0x0
#define PWM_PWM0_CTRL_DT_EN_1   0x1
#define PWM_PWM0_CTRL_MOD_0     0x0
#define PWM_PWM0_CTRL_MOD_1     0x1
#define PWM_PWM0_CTRL_MOD_2     0x2
#define PWM_PWM0_CTRL_P_OE_0    0x0
#define PWM_PWM0_CTRL_P_OE_1    0x1
#define PWM_PWM0_CTRL_P_POL_0   0x0
#define PWM_PWM0_CTRL_P_POL_1   0x1
#define PWM_PWM0_CTRL_N_OE_0    0x0
#define PWM_PWM0_CTRL_N_OE_1    0x1
#define PWM_PWM0_CTRL_N_POL_0   0x0
#define PWM_PWM0_CTRL_N_POL_1   0x1
#define PWM_PWM0_CTRL_REN_0     0x0
#define PWM_PWM0_CTRL_REN_1     0x1
#define PWM_PWM0_CTRL_FEN_0     0x0
#define PWM_PWM0_CTRL_FEN_1     0x1
#define PWM_PWM0_CTRL_P_REV_0   0x0
#define PWM_PWM0_CTRL_P_REV_1   0x1
#define PWM_PWM0_CTRL_P_IE_0    0x0
#define PWM_PWM0_CTRL_P_IE_1    0x1

/* PWM[CH] Control Register */
typedef union {
	u32 val;
	struct {
	u32 dt:8;       /**/
	u32 dt_en:1;    /**/
	u32 rsvd0:5;    /**/
	u32 mod:2;      /**/
	u32 rsvd1:1;    /**/
	u32 p_oe:1;     /**/
	u32 p_pol:1;    /**/
	u32 n_oe:1;     /**/
	u32 n_pol:1;    /**/
	u32 rsvd2:3;    /**/
	u32 ren:1;      /**/
	u32 fen:1;      /**/
	u32 rsvd3:4;    /**/
	u32 p_rev:1;    /**/
	u32 p_ie:1;     /**/
	} bits;
} reg_pwm_pwm1_ctrl_t;

#define PWM_PWM1_CTRL_DT_EN_0   0x0
#define PWM_PWM1_CTRL_DT_EN_1   0x1
#define PWM_PWM1_CTRL_MOD_0     0x0
#define PWM_PWM1_CTRL_MOD_1     0x1
#define PWM_PWM1_CTRL_MOD_2     0x2
#define PWM_PWM1_CTRL_P_OE_0    0x0
#define PWM_PWM1_CTRL_P_OE_1    0x1
#define PWM_PWM1_CTRL_P_POL_0   0x0
#define PWM_PWM1_CTRL_P_POL_1   0x1
#define PWM_PWM1_CTRL_N_OE_0    0x0
#define PWM_PWM1_CTRL_N_OE_1    0x1
#define PWM_PWM1_CTRL_N_POL_0   0x0
#define PWM_PWM1_CTRL_N_POL_1   0x1
#define PWM_PWM1_CTRL_REN_0     0x0
#define PWM_PWM1_CTRL_REN_1     0x1
#define PWM_PWM1_CTRL_FEN_0     0x0
#define PWM_PWM1_CTRL_FEN_1     0x1
#define PWM_PWM1_CTRL_P_REV_0   0x0
#define PWM_PWM1_CTRL_P_REV_1   0x1
#define PWM_PWM1_CTRL_P_IE_0    0x0
#define PWM_PWM1_CTRL_P_IE_1    0x1

/* PWM[CH] Control Register */
typedef union {
	u32 val;
	struct {
	u32 dt:8;       /**/
	u32 dt_en:1;    /**/
	u32 rsvd0:5;    /**/
	u32 mod:2;      /**/
	u32 rsvd1:1;    /**/
	u32 p_oe:1;     /**/
	u32 p_pol:1;    /**/
	u32 n_oe:1;     /**/
	u32 n_pol:1;    /**/
	u32 rsvd2:3;    /**/
	u32 ren:1;      /**/
	u32 fen:1;      /**/
	u32 rsvd3:4;    /**/
	u32 p_rev:1;    /**/
	u32 p_ie:1;     /**/
	} bits;
} reg_pwm_pwm2_ctrl_t;

#define PWM_PWM2_CTRL_DT_EN_0   0x0
#define PWM_PWM2_CTRL_DT_EN_1   0x1
#define PWM_PWM2_CTRL_MOD_0     0x0
#define PWM_PWM2_CTRL_MOD_1     0x1
#define PWM_PWM2_CTRL_MOD_2     0x2
#define PWM_PWM2_CTRL_P_OE_0    0x0
#define PWM_PWM2_CTRL_P_OE_1    0x1
#define PWM_PWM2_CTRL_P_POL_0   0x0
#define PWM_PWM2_CTRL_P_POL_1   0x1
#define PWM_PWM2_CTRL_N_OE_0    0x0
#define PWM_PWM2_CTRL_N_OE_1    0x1
#define PWM_PWM2_CTRL_N_POL_0   0x0
#define PWM_PWM2_CTRL_N_POL_1   0x1
#define PWM_PWM2_CTRL_REN_0     0x0
#define PWM_PWM2_CTRL_REN_1     0x1
#define PWM_PWM2_CTRL_FEN_0     0x0
#define PWM_PWM2_CTRL_FEN_1     0x1
#define PWM_PWM2_CTRL_P_REV_0   0x0
#define PWM_PWM2_CTRL_P_REV_1   0x1
#define PWM_PWM2_CTRL_P_IE_0    0x0
#define PWM_PWM2_CTRL_P_IE_1    0x1

/* PWM[CH] Control Register */
typedef union {
	u32 val;
	struct {
	u32 dt:8;       /**/
	u32 dt_en:1;    /**/
	u32 rsvd0:5;    /**/
	u32 mod:2;      /**/
	u32 rsvd1:1;    /**/
	u32 p_oe:1;     /**/
	u32 p_pol:1;    /**/
	u32 n_oe:1;     /**/
	u32 n_pol:1;    /**/
	u32 rsvd2:3;    /**/
	u32 ren:1;      /**/
	u32 fen:1;      /**/
	u32 rsvd3:4;    /**/
	u32 p_rev:1;    /**/
	u32 p_ie:1;     /**/
	} bits;
} reg_pwm_pwm3_ctrl_t;

#define PWM_PWM3_CTRL_DT_EN_0   0x0
#define PWM_PWM3_CTRL_DT_EN_1   0x1
#define PWM_PWM3_CTRL_MOD_0     0x0
#define PWM_PWM3_CTRL_MOD_1     0x1
#define PWM_PWM3_CTRL_MOD_2     0x2
#define PWM_PWM3_CTRL_P_OE_0    0x0
#define PWM_PWM3_CTRL_P_OE_1    0x1
#define PWM_PWM3_CTRL_P_POL_0   0x0
#define PWM_PWM3_CTRL_P_POL_1   0x1
#define PWM_PWM3_CTRL_N_OE_0    0x0
#define PWM_PWM3_CTRL_N_OE_1    0x1
#define PWM_PWM3_CTRL_N_POL_0   0x0
#define PWM_PWM3_CTRL_N_POL_1   0x1
#define PWM_PWM3_CTRL_REN_0     0x0
#define PWM_PWM3_CTRL_REN_1     0x1
#define PWM_PWM3_CTRL_FEN_0     0x0
#define PWM_PWM3_CTRL_FEN_1     0x1
#define PWM_PWM3_CTRL_P_REV_0   0x0
#define PWM_PWM3_CTRL_P_REV_1   0x1
#define PWM_PWM3_CTRL_P_IE_0    0x0
#define PWM_PWM3_CTRL_P_IE_1    0x1

/* PWM[CH] Control Register */
typedef union {
	u32 val;
	struct {
	u32 dt:8;       /**/
	u32 dt_en:1;    /**/
	u32 rsvd0:5;    /**/
	u32 mod:2;      /**/
	u32 rsvd1:1;    /**/
	u32 p_oe:1;     /**/
	u32 p_pol:1;    /**/
	u32 n_oe:1;     /**/
	u32 n_pol:1;    /**/
	u32 rsvd2:3;    /**/
	u32 ren:1;      /**/
	u32 fen:1;      /**/
	u32 rsvd3:4;    /**/
	u32 p_rev:1;    /**/
	u32 p_ie:1;     /**/
	} bits;
} reg_pwm_pwm4_ctrl_t;

#define PWM_PWM4_CTRL_DT_EN_0   0x0
#define PWM_PWM4_CTRL_DT_EN_1   0x1
#define PWM_PWM4_CTRL_MOD_0     0x0
#define PWM_PWM4_CTRL_MOD_1     0x1
#define PWM_PWM4_CTRL_MOD_2     0x2
#define PWM_PWM4_CTRL_P_OE_0    0x0
#define PWM_PWM4_CTRL_P_OE_1    0x1
#define PWM_PWM4_CTRL_P_POL_0   0x0
#define PWM_PWM4_CTRL_P_POL_1   0x1
#define PWM_PWM4_CTRL_N_OE_0    0x0
#define PWM_PWM4_CTRL_N_OE_1    0x1
#define PWM_PWM4_CTRL_N_POL_0   0x0
#define PWM_PWM4_CTRL_N_POL_1   0x1
#define PWM_PWM4_CTRL_REN_0     0x0
#define PWM_PWM4_CTRL_REN_1     0x1
#define PWM_PWM4_CTRL_FEN_0     0x0
#define PWM_PWM4_CTRL_FEN_1     0x1
#define PWM_PWM4_CTRL_P_REV_0   0x0
#define PWM_PWM4_CTRL_P_REV_1   0x1
#define PWM_PWM4_CTRL_P_IE_0    0x0
#define PWM_PWM4_CTRL_P_IE_1    0x1

/* PWM[CH] Control Register */
typedef union {
	u32 val;
	struct {
	u32 dt:8;       /**/
	u32 dt_en:1;    /**/
	u32 rsvd0:5;    /**/
	u32 mod:2;      /**/
	u32 rsvd1:1;    /**/
	u32 p_oe:1;     /**/
	u32 p_pol:1;    /**/
	u32 n_oe:1;     /**/
	u32 n_pol:1;    /**/
	u32 rsvd2:3;    /**/
	u32 ren:1;      /**/
	u32 fen:1;      /**/
	u32 rsvd3:4;    /**/
	u32 p_rev:1;    /**/
	u32 p_ie:1;     /**/
	} bits;
} reg_pwm_pwm5_ctrl_t;

#define PWM_PWM5_CTRL_DT_EN_0   0x0
#define PWM_PWM5_CTRL_DT_EN_1   0x1
#define PWM_PWM5_CTRL_MOD_0     0x0
#define PWM_PWM5_CTRL_MOD_1     0x1
#define PWM_PWM5_CTRL_MOD_2     0x2
#define PWM_PWM5_CTRL_P_OE_0    0x0
#define PWM_PWM5_CTRL_P_OE_1    0x1
#define PWM_PWM5_CTRL_P_POL_0   0x0
#define PWM_PWM5_CTRL_P_POL_1   0x1
#define PWM_PWM5_CTRL_N_OE_0    0x0
#define PWM_PWM5_CTRL_N_OE_1    0x1
#define PWM_PWM5_CTRL_N_POL_0   0x0
#define PWM_PWM5_CTRL_N_POL_1   0x1
#define PWM_PWM5_CTRL_REN_0     0x0
#define PWM_PWM5_CTRL_REN_1     0x1
#define PWM_PWM5_CTRL_FEN_0     0x0
#define PWM_PWM5_CTRL_FEN_1     0x1
#define PWM_PWM5_CTRL_P_REV_0   0x0
#define PWM_PWM5_CTRL_P_REV_1   0x1
#define PWM_PWM5_CTRL_P_IE_0    0x0
#define PWM_PWM5_CTRL_P_IE_1    0x1

/* PWM[CH] Control Register */
typedef union {
	u32 val;
	struct {
	u32 dt:8;       /**/
	u32 dt_en:1;    /**/
	u32 rsvd0:5;    /**/
	u32 mod:2;      /**/
	u32 rsvd1:1;    /**/
	u32 p_oe:1;     /**/
	u32 p_pol:1;    /**/
	u32 n_oe:1;     /**/
	u32 n_pol:1;    /**/
	u32 rsvd2:3;    /**/
	u32 ren:1;      /**/
	u32 fen:1;      /**/
	u32 rsvd3:4;    /**/
	u32 p_rev:1;    /**/
	u32 p_ie:1;     /**/
	} bits;
} reg_pwm_pwm6_ctrl_t;

#define PWM_PWM6_CTRL_DT_EN_0   0x0
#define PWM_PWM6_CTRL_DT_EN_1   0x1
#define PWM_PWM6_CTRL_MOD_0     0x0
#define PWM_PWM6_CTRL_MOD_1     0x1
#define PWM_PWM6_CTRL_MOD_2     0x2
#define PWM_PWM6_CTRL_P_OE_0    0x0
#define PWM_PWM6_CTRL_P_OE_1    0x1
#define PWM_PWM6_CTRL_P_POL_0   0x0
#define PWM_PWM6_CTRL_P_POL_1   0x1
#define PWM_PWM6_CTRL_N_OE_0    0x0
#define PWM_PWM6_CTRL_N_OE_1    0x1
#define PWM_PWM6_CTRL_N_POL_0   0x0
#define PWM_PWM6_CTRL_N_POL_1   0x1
#define PWM_PWM6_CTRL_REN_0     0x0
#define PWM_PWM6_CTRL_REN_1     0x1
#define PWM_PWM6_CTRL_FEN_0     0x0
#define PWM_PWM6_CTRL_FEN_1     0x1
#define PWM_PWM6_CTRL_P_REV_0   0x0
#define PWM_PWM6_CTRL_P_REV_1   0x1
#define PWM_PWM6_CTRL_P_IE_0    0x0
#define PWM_PWM6_CTRL_P_IE_1    0x1

/* PWM[CH] Control Register */
typedef union {
	u32 val;
	struct {
	u32 dt:8;       /**/
	u32 dt_en:1;    /**/
	u32 rsvd0:5;    /**/
	u32 mod:2;      /**/
	u32 rsvd1:1;    /**/
	u32 p_oe:1;     /**/
	u32 p_pol:1;    /**/
	u32 n_oe:1;     /**/
	u32 n_pol:1;    /**/
	u32 rsvd2:3;    /**/
	u32 ren:1;      /**/
	u32 fen:1;      /**/
	u32 rsvd3:4;    /**/
	u32 p_rev:1;    /**/
	u32 p_ie:1;     /**/
	} bits;
} reg_pwm_pwm7_ctrl_t;

#define PWM_PWM7_CTRL_DT_EN_0   0x0
#define PWM_PWM7_CTRL_DT_EN_1   0x1
#define PWM_PWM7_CTRL_MOD_0     0x0
#define PWM_PWM7_CTRL_MOD_1     0x1
#define PWM_PWM7_CTRL_MOD_2     0x2
#define PWM_PWM7_CTRL_P_OE_0    0x0
#define PWM_PWM7_CTRL_P_OE_1    0x1
#define PWM_PWM7_CTRL_P_POL_0   0x0
#define PWM_PWM7_CTRL_P_POL_1   0x1
#define PWM_PWM7_CTRL_N_OE_0    0x0
#define PWM_PWM7_CTRL_N_OE_1    0x1
#define PWM_PWM7_CTRL_N_POL_0   0x0
#define PWM_PWM7_CTRL_N_POL_1   0x1
#define PWM_PWM7_CTRL_REN_0     0x0
#define PWM_PWM7_CTRL_REN_1     0x1
#define PWM_PWM7_CTRL_FEN_0     0x0
#define PWM_PWM7_CTRL_FEN_1     0x1
#define PWM_PWM7_CTRL_P_REV_0   0x0
#define PWM_PWM7_CTRL_P_REV_1   0x1
#define PWM_PWM7_CTRL_P_IE_0    0x0
#define PWM_PWM7_CTRL_P_IE_1    0x1

/* PWM[CH] Control Register */
typedef union {
	u32 val;
	struct {
	u32 dt:8;       /**/
	u32 dt_en:1;    /**/
	u32 rsvd0:5;    /**/
	u32 mod:2;      /**/
	u32 rsvd1:1;    /**/
	u32 p_oe:1;     /**/
	u32 p_pol:1;    /**/
	u32 n_oe:1;     /**/
	u32 n_pol:1;    /**/
	u32 rsvd2:3;    /**/
	u32 ren:1;      /**/
	u32 fen:1;      /**/
	u32 rsvd3:4;    /**/
	u32 p_rev:1;    /**/
	u32 p_ie:1;     /**/
	} bits;
} reg_pwm_pwm8_ctrl_t;

#define PWM_PWM8_CTRL_DT_EN_0   0x0
#define PWM_PWM8_CTRL_DT_EN_1   0x1
#define PWM_PWM8_CTRL_MOD_0     0x0
#define PWM_PWM8_CTRL_MOD_1     0x1
#define PWM_PWM8_CTRL_MOD_2     0x2
#define PWM_PWM8_CTRL_P_OE_0    0x0
#define PWM_PWM8_CTRL_P_OE_1    0x1
#define PWM_PWM8_CTRL_P_POL_0   0x0
#define PWM_PWM8_CTRL_P_POL_1   0x1
#define PWM_PWM8_CTRL_N_OE_0    0x0
#define PWM_PWM8_CTRL_N_OE_1    0x1
#define PWM_PWM8_CTRL_N_POL_0   0x0
#define PWM_PWM8_CTRL_N_POL_1   0x1
#define PWM_PWM8_CTRL_REN_0     0x0
#define PWM_PWM8_CTRL_REN_1     0x1
#define PWM_PWM8_CTRL_FEN_0     0x0
#define PWM_PWM8_CTRL_FEN_1     0x1
#define PWM_PWM8_CTRL_P_REV_0   0x0
#define PWM_PWM8_CTRL_P_REV_1   0x1
#define PWM_PWM8_CTRL_P_IE_0    0x0
#define PWM_PWM8_CTRL_P_IE_1    0x1

/* PWM[CH] Control Register */
typedef union {
	u32 val;
	struct {
	u32 dt:8;       /**/
	u32 dt_en:1;    /**/
	u32 rsvd0:5;    /**/
	u32 mod:2;      /**/
	u32 rsvd1:1;    /**/
	u32 p_oe:1;     /**/
	u32 p_pol:1;    /**/
	u32 n_oe:1;     /**/
	u32 n_pol:1;    /**/
	u32 rsvd2:3;    /**/
	u32 ren:1;      /**/
	u32 fen:1;      /**/
	u32 rsvd3:4;    /**/
	u32 p_rev:1;    /**/
	u32 p_ie:1;     /**/
	} bits;
} reg_pwm_pwm9_ctrl_t;

#define PWM_PWM9_CTRL_DT_EN_0   0x0
#define PWM_PWM9_CTRL_DT_EN_1   0x1
#define PWM_PWM9_CTRL_MOD_0     0x0
#define PWM_PWM9_CTRL_MOD_1     0x1
#define PWM_PWM9_CTRL_MOD_2     0x2
#define PWM_PWM9_CTRL_P_OE_0    0x0
#define PWM_PWM9_CTRL_P_OE_1    0x1
#define PWM_PWM9_CTRL_P_POL_0   0x0
#define PWM_PWM9_CTRL_P_POL_1   0x1
#define PWM_PWM9_CTRL_N_OE_0    0x0
#define PWM_PWM9_CTRL_N_OE_1    0x1
#define PWM_PWM9_CTRL_N_POL_0   0x0
#define PWM_PWM9_CTRL_N_POL_1   0x1
#define PWM_PWM9_CTRL_REN_0     0x0
#define PWM_PWM9_CTRL_REN_1     0x1
#define PWM_PWM9_CTRL_FEN_0     0x0
#define PWM_PWM9_CTRL_FEN_1     0x1
#define PWM_PWM9_CTRL_P_REV_0   0x0
#define PWM_PWM9_CTRL_P_REV_1   0x1
#define PWM_PWM9_CTRL_P_IE_0    0x0
#define PWM_PWM9_CTRL_P_IE_1    0x1

/* PWM[CH] Counter Capture Register */
typedef union {
	u32 val;
	struct {
	u32 r_cap:16;   /**/
	u32 f_cap:16;   /**/
	} bits;
} reg_pwm_pwm0_cnt_cap_t;

/* PWM[CH] Counter Capture Register */
typedef union {
	u32 val;
	struct {
	u32 r_cap:16;   /**/
	u32 f_cap:16;   /**/
	} bits;
} reg_pwm_pwm1_cnt_cap_t;

/* PWM[CH] Counter Capture Register */
typedef union {
	u32 val;
	struct {
	u32 r_cap:16;   /**/
	u32 f_cap:16;   /**/
	} bits;
} reg_pwm_pwm2_cnt_cap_t;

/* PWM[CH] Counter Capture Register */
typedef union {
	u32 val;
	struct {
	u32 r_cap:16;   /**/
	u32 f_cap:16;   /**/
	} bits;
} reg_pwm_pwm3_cnt_cap_t;

/* PWM[CH] Counter Capture Register */
typedef union {
	u32 val;
	struct {
	u32 r_cap:16;   /**/
	u32 f_cap:16;   /**/
	} bits;
} reg_pwm_pwm4_cnt_cap_t;

/* PWM[CH] Counter Capture Register */
typedef union {
	u32 val;
	struct {
	u32 r_cap:16;   /**/
	u32 f_cap:16;   /**/
	} bits;
} reg_pwm_pwm5_cnt_cap_t;

/* PWM[CH] Counter Capture Register */
typedef union {
	u32 val;
	struct {
	u32 r_cap:16;   /**/
	u32 f_cap:16;   /**/
	} bits;
} reg_pwm_pwm6_cnt_cap_t;

/* PWM[CH] Counter Capture Register */
typedef union {
	u32 val;
	struct {
	u32 r_cap:16;   /**/
	u32 f_cap:16;   /**/
	} bits;
} reg_pwm_pwm7_cnt_cap_t;

/* PWM[CH] Counter Capture Register */
typedef union {
	u32 val;
	struct {
	u32 r_cap:16;   /**/
	u32 f_cap:16;   /**/
	} bits;
} reg_pwm_pwm8_cnt_cap_t;

/* PWM[CH] Counter Capture Register */
typedef union {
	u32 val;
	struct {
	u32 r_cap:16;   /**/
	u32 f_cap:16;   /**/
	} bits;
} reg_pwm_pwm9_cnt_cap_t;

/* PWM[CH] Input Status Register */
typedef union {
	u32 val;
	struct {
	u32 rflag:1;    /**/
	u32 orflag:1;   /**/
	u32 fflag:1;    /**/
	u32 offlag:1;   /**/
	u32 rsvd0:28;   /**/
	} bits;
} reg_pwm_pwm0_in_sts_t;

#define PWM_PWM0_IN_STS_RFLAG_0         0x0
#define PWM_PWM0_IN_STS_RFLAG_1         0x1
#define PWM_PWM0_IN_STS_ORFLAG_0        0x0
#define PWM_PWM0_IN_STS_ORFLAG_1        0x1
#define PWM_PWM0_IN_STS_FFLAG_0         0x0
#define PWM_PWM0_IN_STS_FFLAG_1         0x1
#define PWM_PWM0_IN_STS_OFFLAG_0        0x0
#define PWM_PWM0_IN_STS_OFFLAG_1        0x1

/* PWM[CH] Input Status Register */
typedef union {
	u32 val;
	struct {
	u32 rflag:1;    /**/
	u32 orflag:1;   /**/
	u32 fflag:1;    /**/
	u32 offlag:1;   /**/
	u32 rsvd0:28;   /**/
	} bits;
} reg_pwm_pwm1_in_sts_t;

#define PWM_PWM1_IN_STS_RFLAG_0         0x0
#define PWM_PWM1_IN_STS_RFLAG_1         0x1
#define PWM_PWM1_IN_STS_ORFLAG_0        0x0
#define PWM_PWM1_IN_STS_ORFLAG_1        0x1
#define PWM_PWM1_IN_STS_FFLAG_0         0x0
#define PWM_PWM1_IN_STS_FFLAG_1         0x1
#define PWM_PWM1_IN_STS_OFFLAG_0        0x0
#define PWM_PWM1_IN_STS_OFFLAG_1        0x1

/* PWM[CH] Input Status Register */
typedef union {
	u32 val;
	struct {
	u32 rflag:1;    /**/
	u32 orflag:1;   /**/
	u32 fflag:1;    /**/
	u32 offlag:1;   /**/
	u32 rsvd0:28;   /**/
	} bits;
} reg_pwm_pwm2_in_sts_t;

#define PWM_PWM2_IN_STS_RFLAG_0         0x0
#define PWM_PWM2_IN_STS_RFLAG_1         0x1
#define PWM_PWM2_IN_STS_ORFLAG_0        0x0
#define PWM_PWM2_IN_STS_ORFLAG_1        0x1
#define PWM_PWM2_IN_STS_FFLAG_0         0x0
#define PWM_PWM2_IN_STS_FFLAG_1         0x1
#define PWM_PWM2_IN_STS_OFFLAG_0        0x0
#define PWM_PWM2_IN_STS_OFFLAG_1        0x1

/* PWM[CH] Input Status Register */
typedef union {
	u32 val;
	struct {
	u32 rflag:1;    /**/
	u32 orflag:1;   /**/
	u32 fflag:1;    /**/
	u32 offlag:1;   /**/
	u32 rsvd0:28;   /**/
	} bits;
} reg_pwm_pwm3_in_sts_t;

#define PWM_PWM3_IN_STS_RFLAG_0         0x0
#define PWM_PWM3_IN_STS_RFLAG_1         0x1
#define PWM_PWM3_IN_STS_ORFLAG_0        0x0
#define PWM_PWM3_IN_STS_ORFLAG_1        0x1
#define PWM_PWM3_IN_STS_FFLAG_0         0x0
#define PWM_PWM3_IN_STS_FFLAG_1         0x1
#define PWM_PWM3_IN_STS_OFFLAG_0        0x0
#define PWM_PWM3_IN_STS_OFFLAG_1        0x1

/* PWM[CH] Input Status Register */
typedef union {
	u32 val;
	struct {
	u32 rflag:1;    /**/
	u32 orflag:1;   /**/
	u32 fflag:1;    /**/
	u32 offlag:1;   /**/
	u32 rsvd0:28;   /**/
	} bits;
} reg_pwm_pwm4_in_sts_t;

#define PWM_PWM4_IN_STS_RFLAG_0         0x0
#define PWM_PWM4_IN_STS_RFLAG_1         0x1
#define PWM_PWM4_IN_STS_ORFLAG_0        0x0
#define PWM_PWM4_IN_STS_ORFLAG_1        0x1
#define PWM_PWM4_IN_STS_FFLAG_0         0x0
#define PWM_PWM4_IN_STS_FFLAG_1         0x1
#define PWM_PWM4_IN_STS_OFFLAG_0        0x0
#define PWM_PWM4_IN_STS_OFFLAG_1        0x1

/* PWM[CH] Input Status Register */
typedef union {
	u32 val;
	struct {
	u32 rflag:1;    /**/
	u32 orflag:1;   /**/
	u32 fflag:1;    /**/
	u32 offlag:1;   /**/
	u32 rsvd0:28;   /**/
	} bits;
} reg_pwm_pwm5_in_sts_t;

#define PWM_PWM5_IN_STS_RFLAG_0         0x0
#define PWM_PWM5_IN_STS_RFLAG_1         0x1
#define PWM_PWM5_IN_STS_ORFLAG_0        0x0
#define PWM_PWM5_IN_STS_ORFLAG_1        0x1
#define PWM_PWM5_IN_STS_FFLAG_0         0x0
#define PWM_PWM5_IN_STS_FFLAG_1         0x1
#define PWM_PWM5_IN_STS_OFFLAG_0        0x0
#define PWM_PWM5_IN_STS_OFFLAG_1        0x1

/* PWM[CH] Input Status Register */
typedef union {
	u32 val;
	struct {
	u32 rflag:1;    /**/
	u32 orflag:1;   /**/
	u32 fflag:1;    /**/
	u32 offlag:1;   /**/
	u32 rsvd0:28;   /**/
	} bits;
} reg_pwm_pwm6_in_sts_t;

#define PWM_PWM6_IN_STS_RFLAG_0         0x0
#define PWM_PWM6_IN_STS_RFLAG_1         0x1
#define PWM_PWM6_IN_STS_ORFLAG_0        0x0
#define PWM_PWM6_IN_STS_ORFLAG_1        0x1
#define PWM_PWM6_IN_STS_FFLAG_0         0x0
#define PWM_PWM6_IN_STS_FFLAG_1         0x1
#define PWM_PWM6_IN_STS_OFFLAG_0        0x0
#define PWM_PWM6_IN_STS_OFFLAG_1        0x1

/* PWM[CH] Input Status Register */
typedef union {
	u32 val;
	struct {
	u32 rflag:1;    /**/
	u32 orflag:1;   /**/
	u32 fflag:1;    /**/
	u32 offlag:1;   /**/
	u32 rsvd0:28;   /**/
	} bits;
} reg_pwm_pwm7_in_sts_t;

#define PWM_PWM7_IN_STS_RFLAG_0         0x0
#define PWM_PWM7_IN_STS_RFLAG_1         0x1
#define PWM_PWM7_IN_STS_ORFLAG_0        0x0
#define PWM_PWM7_IN_STS_ORFLAG_1        0x1
#define PWM_PWM7_IN_STS_FFLAG_0         0x0
#define PWM_PWM7_IN_STS_FFLAG_1         0x1
#define PWM_PWM7_IN_STS_OFFLAG_0        0x0
#define PWM_PWM7_IN_STS_OFFLAG_1        0x1

/* PWM[CH] Input Status Register */
typedef union {
	u32 val;
	struct {
	u32 rflag:1;    /**/
	u32 orflag:1;   /**/
	u32 fflag:1;    /**/
	u32 offlag:1;   /**/
	u32 rsvd0:28;   /**/
	} bits;
} reg_pwm_pwm8_in_sts_t;

#define PWM_PWM8_IN_STS_RFLAG_0         0x0
#define PWM_PWM8_IN_STS_RFLAG_1         0x1
#define PWM_PWM8_IN_STS_ORFLAG_0        0x0
#define PWM_PWM8_IN_STS_ORFLAG_1        0x1
#define PWM_PWM8_IN_STS_FFLAG_0         0x0
#define PWM_PWM8_IN_STS_FFLAG_1         0x1
#define PWM_PWM8_IN_STS_OFFLAG_0        0x0
#define PWM_PWM8_IN_STS_OFFLAG_1        0x1

/* PWM[CH] Input Status Register */
typedef union {
	u32 val;
	struct {
	u32 rflag:1;    /**/
	u32 orflag:1;   /**/
	u32 fflag:1;    /**/
	u32 offlag:1;   /**/
	u32 rsvd0:28;   /**/
	} bits;
} reg_pwm_pwm9_in_sts_t;

#define PWM_PWM9_IN_STS_RFLAG_0         0x0
#define PWM_PWM9_IN_STS_RFLAG_1         0x1
#define PWM_PWM9_IN_STS_ORFLAG_0        0x0
#define PWM_PWM9_IN_STS_ORFLAG_1        0x1
#define PWM_PWM9_IN_STS_FFLAG_0         0x0
#define PWM_PWM9_IN_STS_FFLAG_1         0x1
#define PWM_PWM9_IN_STS_OFFLAG_0        0x0
#define PWM_PWM9_IN_STS_OFFLAG_1        0x1

/* PWM[CH] Delay Counter Register */
typedef union {
	u32 val;
	struct {
	u32 delay:16;   /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_pwm_pwm0_dly_cnt_t;

/* PWM[CH] Delay Counter Register */
typedef union {
	u32 val;
	struct {
	u32 delay:16;   /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_pwm_pwm1_dly_cnt_t;

/* PWM[CH] Delay Counter Register */
typedef union {
	u32 val;
	struct {
	u32 delay:16;   /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_pwm_pwm2_dly_cnt_t;

/* PWM[CH] Delay Counter Register */
typedef union {
	u32 val;
	struct {
	u32 delay:16;   /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_pwm_pwm3_dly_cnt_t;

/* PWM[CH] Delay Counter Register */
typedef union {
	u32 val;
	struct {
	u32 delay:16;   /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_pwm_pwm4_dly_cnt_t;

/* PWM[CH] Delay Counter Register */
typedef union {
	u32 val;
	struct {
	u32 delay:16;   /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_pwm_pwm5_dly_cnt_t;

/* PWM[CH] Delay Counter Register */
typedef union {
	u32 val;
	struct {
	u32 delay:16;   /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_pwm_pwm6_dly_cnt_t;

/* PWM[CH] Delay Counter Register */
typedef union {
	u32 val;
	struct {
	u32 delay:16;   /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_pwm_pwm7_dly_cnt_t;

/* PWM[CH] Delay Counter Register */
typedef union {
	u32 val;
	struct {
	u32 delay:16;   /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_pwm_pwm8_dly_cnt_t;

/* PWM[CH] Delay Counter Register */
typedef union {
	u32 val;
	struct {
	u32 delay:16;   /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_pwm_pwm9_dly_cnt_t;

/* PWM[CH] Pulse Counter Register */
typedef union {
	u32 val;
	struct {
	u32 pcnt:16;    /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_pwm_pwm0_pulse_cnt_t;

/* PWM[CH] Pulse Counter Register */
typedef union {
	u32 val;
	struct {
	u32 pcnt:16;    /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_pwm_pwm1_pulse_cnt_t;

/* PWM[CH] Pulse Counter Register */
typedef union {
	u32 val;
	struct {
	u32 pcnt:16;    /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_pwm_pwm2_pulse_cnt_t;

/* PWM[CH] Pulse Counter Register */
typedef union {
	u32 val;
	struct {
	u32 pcnt:16;    /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_pwm_pwm3_pulse_cnt_t;

/* PWM[CH] Pulse Counter Register */
typedef union {
	u32 val;
	struct {
	u32 pcnt:16;    /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_pwm_pwm4_pulse_cnt_t;

/* PWM[CH] Pulse Counter Register */
typedef union {
	u32 val;
	struct {
	u32 pcnt:16;    /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_pwm_pwm5_pulse_cnt_t;

/* PWM[CH] Pulse Counter Register */
typedef union {
	u32 val;
	struct {
	u32 pcnt:16;    /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_pwm_pwm6_pulse_cnt_t;

/* PWM[CH] Pulse Counter Register */
typedef union {
	u32 val;
	struct {
	u32 pcnt:16;    /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_pwm_pwm7_pulse_cnt_t;

/* PWM[CH] Pulse Counter Register */
typedef union {
	u32 val;
	struct {
	u32 pcnt:16;    /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_pwm_pwm8_pulse_cnt_t;

/* PWM[CH] Pulse Counter Register */
typedef union {
	u32 val;
	struct {
	u32 pcnt:16;    /**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_pwm_pwm9_pulse_cnt_t;

/* reg_pwm_t bank */
typedef struct tag_pwm {
	reg_pwm_ver_t                  ver;                /* 0000 */
	reg_pwm_pwm_ce_t               pwm_ce;             /* 0004 */
	reg_pwm_pwm_pe_t               pwm_pe;             /* 0008 */
	u32                            res0[1];
	reg_pwm_pwm_int_en_t           pwm_int_en;         /* 0010 */
	reg_pwm_pwm_clr_int_pending_t  pwm_clr_int_pending;/* 0014 */
	reg_pwm_pwm_int_pending_t      pwm_int_pending;    /* 0018 */
	u32                            res1[9];
	reg_pwm_pwm0_clk_ctrl_t        pwm0_clk_ctrl;      /* 0040 */
	reg_pwm_pwm0_cnt_store_t       pwm0_cnt_store;     /* 0044 */
	reg_pwm_pwm0_cnt_cmp_t         pwm0_cnt_cmp;       /* 0048 */
	reg_pwm_pwm0_ctrl_t            pwm0_ctrl;          /* 004C */
	reg_pwm_pwm0_cnt_cap_t         pwm0_cnt_cap;       /* 0050 */
	reg_pwm_pwm0_in_sts_t          pwm0_in_sts;        /* 0054 */
	reg_pwm_pwm0_dly_cnt_t         pwm0_dly_cnt;       /* 0058 */
	reg_pwm_pwm0_pulse_cnt_t       pwm0_pulse_cnt;     /* 005C */
	u32                            res2[8];
	reg_pwm_pwm1_clk_ctrl_t        pwm1_clk_ctrl;      /* 0080 */
	reg_pwm_pwm1_cnt_store_t       pwm1_cnt_store;     /* 0084 */
	reg_pwm_pwm1_cnt_cmp_t         pwm1_cnt_cmp;       /* 0088 */
	reg_pwm_pwm1_ctrl_t            pwm1_ctrl;          /* 008C */
	reg_pwm_pwm1_cnt_cap_t         pwm1_cnt_cap;       /* 0090 */
	reg_pwm_pwm1_in_sts_t          pwm1_in_sts;        /* 0094 */
	reg_pwm_pwm1_dly_cnt_t         pwm1_dly_cnt;       /* 0098 */
	reg_pwm_pwm1_pulse_cnt_t       pwm1_pulse_cnt;     /* 009C */
	u32                            res3[8];
	reg_pwm_pwm2_clk_ctrl_t        pwm2_clk_ctrl;      /* 00C0 */
	reg_pwm_pwm2_cnt_store_t       pwm2_cnt_store;     /* 00C4 */
	reg_pwm_pwm2_cnt_cmp_t         pwm2_cnt_cmp;       /* 00C8 */
	reg_pwm_pwm2_ctrl_t            pwm2_ctrl;          /* 00CC */
	reg_pwm_pwm2_cnt_cap_t         pwm2_cnt_cap;       /* 00D0 */
	reg_pwm_pwm2_in_sts_t          pwm2_in_sts;        /* 00D4 */
	reg_pwm_pwm2_dly_cnt_t         pwm2_dly_cnt;       /* 00D8 */
	reg_pwm_pwm2_pulse_cnt_t       pwm2_pulse_cnt;     /* 00DC */
	u32                            res4[8];
	reg_pwm_pwm3_clk_ctrl_t        pwm3_clk_ctrl;      /* 0100 */
	reg_pwm_pwm3_cnt_store_t       pwm3_cnt_store;     /* 0104 */
	reg_pwm_pwm3_cnt_cmp_t         pwm3_cnt_cmp;       /* 0108 */
	reg_pwm_pwm3_ctrl_t            pwm3_ctrl;          /* 010C */
	reg_pwm_pwm3_cnt_cap_t         pwm3_cnt_cap;       /* 0110 */
	reg_pwm_pwm3_in_sts_t          pwm3_in_sts;        /* 0114 */
	reg_pwm_pwm3_dly_cnt_t         pwm3_dly_cnt;       /* 0118 */
	reg_pwm_pwm3_pulse_cnt_t       pwm3_pulse_cnt;     /* 011C */
	u32                            res5[8];
	reg_pwm_pwm4_clk_ctrl_t        pwm4_clk_ctrl;      /* 0140 */
	reg_pwm_pwm4_cnt_store_t       pwm4_cnt_store;     /* 0144 */
	reg_pwm_pwm4_cnt_cmp_t         pwm4_cnt_cmp;       /* 0148 */
	reg_pwm_pwm4_ctrl_t            pwm4_ctrl;          /* 014C */
	reg_pwm_pwm4_cnt_cap_t         pwm4_cnt_cap;       /* 0150 */
	reg_pwm_pwm4_in_sts_t          pwm4_in_sts;        /* 0154 */
	reg_pwm_pwm4_dly_cnt_t         pwm4_dly_cnt;       /* 0158 */
	reg_pwm_pwm4_pulse_cnt_t       pwm4_pulse_cnt;     /* 015C */
	u32                            res6[8];
	reg_pwm_pwm5_clk_ctrl_t        pwm5_clk_ctrl;      /* 0180 */
	reg_pwm_pwm5_cnt_store_t       pwm5_cnt_store;     /* 0184 */
	reg_pwm_pwm5_cnt_cmp_t         pwm5_cnt_cmp;       /* 0188 */
	reg_pwm_pwm5_ctrl_t            pwm5_ctrl;          /* 018C */
	reg_pwm_pwm5_cnt_cap_t         pwm5_cnt_cap;       /* 0190 */
	reg_pwm_pwm5_in_sts_t          pwm5_in_sts;        /* 0194 */
	reg_pwm_pwm5_dly_cnt_t         pwm5_dly_cnt;       /* 0198 */
	reg_pwm_pwm5_pulse_cnt_t       pwm5_pulse_cnt;     /* 019C */
	u32                            res7[8];
	reg_pwm_pwm6_clk_ctrl_t        pwm6_clk_ctrl;      /* 01C0 */
	reg_pwm_pwm6_cnt_store_t       pwm6_cnt_store;     /* 01C4 */
	reg_pwm_pwm6_cnt_cmp_t         pwm6_cnt_cmp;       /* 01C8 */
	reg_pwm_pwm6_ctrl_t            pwm6_ctrl;          /* 01CC */
	reg_pwm_pwm6_cnt_cap_t         pwm6_cnt_cap;       /* 01D0 */
	reg_pwm_pwm6_in_sts_t          pwm6_in_sts;        /* 01D4 */
	reg_pwm_pwm6_dly_cnt_t         pwm6_dly_cnt;       /* 01D8 */
	reg_pwm_pwm6_pulse_cnt_t       pwm6_pulse_cnt;     /* 01DC */
	u32                            res8[8];
	reg_pwm_pwm7_clk_ctrl_t        pwm7_clk_ctrl;      /* 0200 */
	reg_pwm_pwm7_cnt_store_t       pwm7_cnt_store;     /* 0204 */
	reg_pwm_pwm7_cnt_cmp_t         pwm7_cnt_cmp;       /* 0208 */
	reg_pwm_pwm7_ctrl_t            pwm7_ctrl;          /* 020C */
	reg_pwm_pwm7_cnt_cap_t         pwm7_cnt_cap;       /* 0210 */
	reg_pwm_pwm7_in_sts_t          pwm7_in_sts;        /* 0214 */
	reg_pwm_pwm7_dly_cnt_t         pwm7_dly_cnt;       /* 0218 */
	reg_pwm_pwm7_pulse_cnt_t       pwm7_pulse_cnt;     /* 021C */
	u32                            res9[8];
	reg_pwm_pwm8_clk_ctrl_t        pwm8_clk_ctrl;      /* 0240 */
	reg_pwm_pwm8_cnt_store_t       pwm8_cnt_store;     /* 0244 */
	reg_pwm_pwm8_cnt_cmp_t         pwm8_cnt_cmp;       /* 0248 */
	reg_pwm_pwm8_ctrl_t            pwm8_ctrl;          /* 024C */
	reg_pwm_pwm8_cnt_cap_t         pwm8_cnt_cap;       /* 0250 */
	reg_pwm_pwm8_in_sts_t          pwm8_in_sts;        /* 0254 */
	reg_pwm_pwm8_dly_cnt_t         pwm8_dly_cnt;       /* 0258 */
	reg_pwm_pwm8_pulse_cnt_t       pwm8_pulse_cnt;     /* 025C */
	u32                            res10[8];
	reg_pwm_pwm9_clk_ctrl_t        pwm9_clk_ctrl;      /* 0280 */
	reg_pwm_pwm9_cnt_store_t       pwm9_cnt_store;     /* 0284 */
	reg_pwm_pwm9_cnt_cmp_t         pwm9_cnt_cmp;       /* 0288 */
	reg_pwm_pwm9_ctrl_t            pwm9_ctrl;          /* 028C */
	reg_pwm_pwm9_cnt_cap_t         pwm9_cnt_cap;       /* 0290 */
	reg_pwm_pwm9_in_sts_t          pwm9_in_sts;        /* 0294 */
	reg_pwm_pwm9_dly_cnt_t         pwm9_dly_cnt;       /* 0298 */
	reg_pwm_pwm9_pulse_cnt_t       pwm9_pulse_cnt;     /* 029C */
} reg_pwm_t;

#endif /* ___PWM___H___ */
