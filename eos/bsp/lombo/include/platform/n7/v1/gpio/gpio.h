/* gpio.h */

#ifndef ___GPIO___H___
#define ___GPIO___H___

#define BASE_GPIO                0x04015000

#define VA_GPIO_GPIOC_VER                    (0x00000000 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOA_FUNC_R0                (0x00000020 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOA_FUNC_R1                (0x00000024 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOA_PUD_R0                 (0x00000030 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOA_PUD_R1                 (0x00000034 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOA_DRV_R0                 (0x00000040 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOA_DRV_R1                 (0x00000044 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOA_DATA                   (0x00000050 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOA_PR_R                   (0x00000054 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOB_FUNC_R0                (0x00000060 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOB_FUNC_R1                (0x00000064 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOB_FUNC_R2                (0x00000068 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOB_FUNC_R3                (0x0000006C + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOB_PUD_R0                 (0x00000070 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOB_PUD_R1                 (0x00000074 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOB_PUD_R2                 (0x00000078 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOB_PUD_R3                 (0x0000007C + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOB_DRV_R0                 (0x00000080 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOB_DRV_R1                 (0x00000084 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOB_DRV_R2                 (0x00000088 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOB_DRV_R3                 (0x0000008C + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOB_DATA                   (0x00000090 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOB_PR_R                   (0x00000094 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOC_FUNC_R0                (0x000000A0 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOC_FUNC_R1                (0x000000A4 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOC_FUNC_R2                (0x000000A8 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOC_PUD_R0                 (0x000000B0 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOC_PUD_R1                 (0x000000B4 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOC_PUD_R2                 (0x000000B8 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOC_DRV_R0                 (0x000000C0 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOC_DRV_R1                 (0x000000C4 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOC_DRV_R2                 (0x000000C8 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOC_DATA                   (0x000000D0 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOC_PR_R                   (0x000000D4 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOD_FUNC_R0                (0x000000E0 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOD_PUD_R0                 (0x000000F0 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOD_DRV_R0                 (0x00000100 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOD_DATA                   (0x00000110 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOD_PR_R                   (0x00000114 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOE_FUNC_R0                (0x00000120 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOE_FUNC_R1                (0x00000124 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOE_PUD_R0                 (0x00000130 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOE_PUD_R1                 (0x00000134 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOE_DRV_R0                 (0x00000140 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOE_DRV_R1                 (0x00000144 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOE_DATA                   (0x00000150 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOE_PR_R                   (0x00000154 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOF_FUNC_R0                (0x00000160 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOF_FUNC_R1                (0x00000164 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOF_PUD_R0                 (0x00000170 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOF_PUD_R1                 (0x00000174 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOF_DRV_R0                 (0x00000180 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOF_DRV_R1                 (0x00000184 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOF_DATA                   (0x00000190 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOF_PR_R                   (0x00000194 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOG_FUNC_R0                (0x000001A0 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOG_FUNC_R1                (0x000001A4 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOG_PUD_R0                 (0x000001B0 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOG_PUD_R1                 (0x000001B4 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOG_DRV_R0                 (0x000001C0 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOG_DRV_R1                 (0x000001C4 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOG_DATA                   (0x000001D0 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOG_PR_R                   (0x000001D4 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOJ_FUNC_R0                (0x00000220 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOJ_PUD_R0                 (0x00000230 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOJ_DRV_R0                 (0x00000240 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOJ_DATA                   (0x00000250 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOJ_PR_R                   (0x00000254 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOB_INT_TRI_R0             (0x00000560 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOB_INT_TRI_R1             (0x00000564 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOB_INT_TRI_R2             (0x00000568 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOB_INT_TRI_R3             (0x0000056C + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOB_INT_MASK               (0x00000570 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOB_INT_PEND               (0x00000574 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOB_INT_SAMP_CTRL          (0x00000578 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOE_INT_TRI_R0             (0x00000620 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOE_INT_TRI_R1             (0x00000624 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOE_INT_MASK               (0x00000630 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOE_INT_PEND               (0x00000634 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOE_INT_SAMP_CTRL          (0x00000638 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOF_INT_TRI_R0             (0x00000660 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOF_INT_TRI_R1             (0x00000664 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOF_INT_MASK               (0x00000670 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOF_INT_PEND               (0x00000674 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOF_INT_SAMP_CTRL          (0x00000678 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOG_INT_TRI_R0             (0x000006A0 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOG_INT_TRI_R1             (0x000006A4 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOG_INT_MASK               (0x000006B0 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOG_INT_PEND               (0x000006B4 + BASE_GPIO + VA_GPIO)
#define VA_GPIO_GPIOG_INT_SAMP_CTRL          (0x000006B8 + BASE_GPIO + VA_GPIO)

#define DATA_GPIO_GPIOC_VER                      0x00001100
#define DATA_GPIO_GPIOA_FUNC_R0                  0x33003333
#define DATA_GPIO_GPIOA_FUNC_R1                  0x00003333
#define DATA_GPIO_GPIOA_PUD_R0                   0x00000000
#define DATA_GPIO_GPIOA_PUD_R1                   0x00000000
#define DATA_GPIO_GPIOA_DRV_R0                   0x33333333
#define DATA_GPIO_GPIOA_DRV_R1                   0x33333333
#define DATA_GPIO_GPIOA_DATA                     0x00000000
#define DATA_GPIO_GPIOA_PR_R                     0x00000000
#define DATA_GPIO_GPIOB_FUNC_R0                  0x00000000
#define DATA_GPIO_GPIOB_FUNC_R1                  0x00000000
#define DATA_GPIO_GPIOB_FUNC_R2                  0x00000000
#define DATA_GPIO_GPIOB_FUNC_R3                  0x00000000
#define DATA_GPIO_GPIOB_PUD_R0                   0x00000000
#define DATA_GPIO_GPIOB_PUD_R1                   0x00000000
#define DATA_GPIO_GPIOB_PUD_R2                   0x00000000
#define DATA_GPIO_GPIOB_PUD_R3                   0x00000000
#define DATA_GPIO_GPIOB_DRV_R0                   0x33333300
#define DATA_GPIO_GPIOB_DRV_R1                   0x33333300
#define DATA_GPIO_GPIOB_DRV_R2                   0x33333300
#define DATA_GPIO_GPIOB_DRV_R3                   0x00003333
#define DATA_GPIO_GPIOB_DATA                     0x00000000
#define DATA_GPIO_GPIOB_PR_R                     0x00000000
#define DATA_GPIO_GPIOC_FUNC_R0                  0x00000000
#define DATA_GPIO_GPIOC_FUNC_R1                  0x00000000
#define DATA_GPIO_GPIOC_FUNC_R2                  0x00000000
#define DATA_GPIO_GPIOC_PUD_R0                   0x00000000
#define DATA_GPIO_GPIOC_PUD_R1                   0x00000000
#define DATA_GPIO_GPIOC_PUD_R2                   0x00000000
#define DATA_GPIO_GPIOC_DRV_R0                   0x33333333
#define DATA_GPIO_GPIOC_DRV_R1                   0x33333333
#define DATA_GPIO_GPIOC_DRV_R2                   0x03333333
#define DATA_GPIO_GPIOC_DATA                     0x00000000
#define DATA_GPIO_GPIOC_PR_R                     0x00000000
#define DATA_GPIO_GPIOD_FUNC_R0                  0x00000000
#define DATA_GPIO_GPIOD_PUD_R0                   0x00000000
#define DATA_GPIO_GPIOD_DRV_R0                   0x00333333
#define DATA_GPIO_GPIOD_DATA                     0x00000000
#define DATA_GPIO_GPIOD_PR_R                     0x00000000
#define DATA_GPIO_GPIOE_FUNC_R0                  0x00000000
#define DATA_GPIO_GPIOE_FUNC_R1                  0x00000000
#define DATA_GPIO_GPIOE_PUD_R0                   0x00000000
#define DATA_GPIO_GPIOE_PUD_R1                   0x00000000
#define DATA_GPIO_GPIOE_DRV_R0                   0x33333333
#define DATA_GPIO_GPIOE_DRV_R1                   0x03333333
#define DATA_GPIO_GPIOE_DATA                     0x00000000
#define DATA_GPIO_GPIOE_PR_R                     0x00000000
#define DATA_GPIO_GPIOF_FUNC_R0                  0x33333333
#define DATA_GPIO_GPIOF_FUNC_R1                  0x00000033
#define DATA_GPIO_GPIOF_PUD_R0                   0x00000000
#define DATA_GPIO_GPIOF_PUD_R1                   0x00000000
#define DATA_GPIO_GPIOF_DRV_R0                   0x33333333
#define DATA_GPIO_GPIOF_DRV_R1                   0x00000033
#define DATA_GPIO_GPIOF_DATA                     0x00000000
#define DATA_GPIO_GPIOF_PR_R                     0x00000000
#define DATA_GPIO_GPIOG_FUNC_R0                  0x33333333
#define DATA_GPIO_GPIOG_FUNC_R1                  0x00000033
#define DATA_GPIO_GPIOG_PUD_R0                   0x00000000
#define DATA_GPIO_GPIOG_PUD_R1                   0x00000000
#define DATA_GPIO_GPIOG_DRV_R0                   0x33333333
#define DATA_GPIO_GPIOG_DRV_R1                   0x00000033
#define DATA_GPIO_GPIOG_DATA                     0x00000000
#define DATA_GPIO_GPIOG_PR_R                     0x00000000
#define DATA_GPIO_GPIOJ_FUNC_R0                  0x00000000
#define DATA_GPIO_GPIOJ_PUD_R0                   0x00000000
#define DATA_GPIO_GPIOJ_DRV_R0                   0x00000003
#define DATA_GPIO_GPIOJ_DATA                     0x00000000
#define DATA_GPIO_GPIOJ_PR_R                     0x00000000
#define DATA_GPIO_GPIOB_INT_TRI_R0               0x00000000
#define DATA_GPIO_GPIOB_INT_TRI_R1               0x00000000
#define DATA_GPIO_GPIOB_INT_TRI_R2               0x00000000
#define DATA_GPIO_GPIOB_INT_TRI_R3               0x00000000
#define DATA_GPIO_GPIOB_INT_MASK                 0x00000000
#define DATA_GPIO_GPIOB_INT_PEND                 0x00000000
#define DATA_GPIO_GPIOB_INT_SAMP_CTRL            0x00000000
#define DATA_GPIO_GPIOE_INT_TRI_R0               0x00000000
#define DATA_GPIO_GPIOE_INT_TRI_R1               0x00000000
#define DATA_GPIO_GPIOE_INT_MASK                 0x00000000
#define DATA_GPIO_GPIOE_INT_PEND                 0x00000000
#define DATA_GPIO_GPIOE_INT_SAMP_CTRL            0x00000000
#define DATA_GPIO_GPIOF_INT_TRI_R0               0x00000000
#define DATA_GPIO_GPIOF_INT_TRI_R1               0x00000000
#define DATA_GPIO_GPIOF_INT_MASK                 0x00000000
#define DATA_GPIO_GPIOF_INT_PEND                 0x00000000
#define DATA_GPIO_GPIOF_INT_SAMP_CTRL            0x00000000
#define DATA_GPIO_GPIOG_INT_TRI_R0               0x00000000
#define DATA_GPIO_GPIOG_INT_TRI_R1               0x00000000
#define DATA_GPIO_GPIOG_INT_MASK                 0x00000000
#define DATA_GPIO_GPIOG_INT_PEND                 0x00000000
#define DATA_GPIO_GPIOG_INT_SAMP_CTRL            0x00000000

/* GPIO Controller Version Register */
typedef union {
	u32 val;
	struct {
	u32 ver_l:5;           /* */
	u32 rsvd0:3;           /* */
	u32 ver_h:3;           /* */
	u32 rsvd1:1;           /* */
	u32 comp:1;            /* */
	u32 rsvd2:19;          /* */
	} bits;
} reg_gpio_gpioc_ver_t;

#define GPIO_GPIOC_VER_COMP_0   0x0
#define GPIO_GPIOC_VER_COMP_1   0x1

/* GPIOA Function Configuration Register0 */
typedef union {
	u32 val;
	struct {
	u32 gpioa0:4;          /* */
	u32 gpioa1:4;          /* */
	u32 gpioa2:4;          /* */
	u32 gpioa3:4;          /* */
	u32 gpioa4:4;          /* */
	u32 gpioa5:4;          /* */
	u32 gpioa6:4;          /* */
	u32 gpioa7:4;          /* */
	} bits;
} reg_gpio_gpioa_func_r0_t;

#define GPIO_GPIOA_FUNC_R0_GPIOA0_0     0x0
#define GPIO_GPIOA_FUNC_R0_GPIOA0_1     0x1
#define GPIO_GPIOA_FUNC_R0_GPIOA0_2     0x2
#define GPIO_GPIOA_FUNC_R0_GPIOA0_3     0x3
#define GPIO_GPIOA_FUNC_R0_GPIOA0_4     0x4
#define GPIO_GPIOA_FUNC_R0_GPIOA1_0     0x0
#define GPIO_GPIOA_FUNC_R0_GPIOA1_1     0x1
#define GPIO_GPIOA_FUNC_R0_GPIOA1_2     0x2
#define GPIO_GPIOA_FUNC_R0_GPIOA1_3     0x3
#define GPIO_GPIOA_FUNC_R0_GPIOA1_4     0x4
#define GPIO_GPIOA_FUNC_R0_GPIOA2_0     0x0
#define GPIO_GPIOA_FUNC_R0_GPIOA2_1     0x1
#define GPIO_GPIOA_FUNC_R0_GPIOA2_2     0x2
#define GPIO_GPIOA_FUNC_R0_GPIOA2_3     0x3
#define GPIO_GPIOA_FUNC_R0_GPIOA2_4     0x4
#define GPIO_GPIOA_FUNC_R0_GPIOA3_0     0x0
#define GPIO_GPIOA_FUNC_R0_GPIOA3_1     0x1
#define GPIO_GPIOA_FUNC_R0_GPIOA3_2     0x2
#define GPIO_GPIOA_FUNC_R0_GPIOA3_3     0x3
#define GPIO_GPIOA_FUNC_R0_GPIOA3_4     0x4
#define GPIO_GPIOA_FUNC_R0_GPIOA4_0     0x0
#define GPIO_GPIOA_FUNC_R0_GPIOA4_1     0x1
#define GPIO_GPIOA_FUNC_R0_GPIOA4_2     0x2
#define GPIO_GPIOA_FUNC_R0_GPIOA4_3     0x3
#define GPIO_GPIOA_FUNC_R0_GPIOA4_4     0x4
#define GPIO_GPIOA_FUNC_R0_GPIOA5_0     0x0
#define GPIO_GPIOA_FUNC_R0_GPIOA5_1     0x1
#define GPIO_GPIOA_FUNC_R0_GPIOA5_2     0x2
#define GPIO_GPIOA_FUNC_R0_GPIOA5_3     0x3
#define GPIO_GPIOA_FUNC_R0_GPIOA5_4     0x4
#define GPIO_GPIOA_FUNC_R0_GPIOA6_0     0x0
#define GPIO_GPIOA_FUNC_R0_GPIOA6_1     0x1
#define GPIO_GPIOA_FUNC_R0_GPIOA6_2     0x2
#define GPIO_GPIOA_FUNC_R0_GPIOA6_3     0x3
#define GPIO_GPIOA_FUNC_R0_GPIOA6_4     0x4
#define GPIO_GPIOA_FUNC_R0_GPIOA6_5     0x5
#define GPIO_GPIOA_FUNC_R0_GPIOA6_6     0x6
#define GPIO_GPIOA_FUNC_R0_GPIOA6_7     0x7
#define GPIO_GPIOA_FUNC_R0_GPIOA7_0     0x0
#define GPIO_GPIOA_FUNC_R0_GPIOA7_1     0x1
#define GPIO_GPIOA_FUNC_R0_GPIOA7_2     0x2
#define GPIO_GPIOA_FUNC_R0_GPIOA7_3     0x3
#define GPIO_GPIOA_FUNC_R0_GPIOA7_4     0x4
#define GPIO_GPIOA_FUNC_R0_GPIOA7_5     0x5

/* GPIOA Function Configuration Register1 */
typedef union {
	u32 val;
	struct {
	u32 gpioa8:4;          /* */
	u32 gpioa9:4;          /* */
	u32 gpioa10:4;         /* */
	u32 gpioa11:4;         /* */
	u32 gpioa12:4;         /* */
	u32 gpioa13:4;         /* */
	u32 gpioa14:4;         /* */
	u32 gpioa15:4;         /* */
	} bits;
} reg_gpio_gpioa_func_r1_t;

#define GPIO_GPIOA_FUNC_R1_GPIOA8_0     0x0
#define GPIO_GPIOA_FUNC_R1_GPIOA8_1     0x1
#define GPIO_GPIOA_FUNC_R1_GPIOA8_2     0x2
#define GPIO_GPIOA_FUNC_R1_GPIOA8_3     0x3
#define GPIO_GPIOA_FUNC_R1_GPIOA8_4     0x4
#define GPIO_GPIOA_FUNC_R1_GPIOA8_5     0x5
#define GPIO_GPIOA_FUNC_R1_GPIOA8_6     0x6
#define GPIO_GPIOA_FUNC_R1_GPIOA8_7     0x7
#define GPIO_GPIOA_FUNC_R1_GPIOA9_0     0x0
#define GPIO_GPIOA_FUNC_R1_GPIOA9_1     0x1
#define GPIO_GPIOA_FUNC_R1_GPIOA9_2     0x2
#define GPIO_GPIOA_FUNC_R1_GPIOA9_3     0x3
#define GPIO_GPIOA_FUNC_R1_GPIOA9_4     0x4
#define GPIO_GPIOA_FUNC_R1_GPIOA9_5     0x5
#define GPIO_GPIOA_FUNC_R1_GPIOA10_0    0x0
#define GPIO_GPIOA_FUNC_R1_GPIOA10_1    0x1
#define GPIO_GPIOA_FUNC_R1_GPIOA10_2    0x2
#define GPIO_GPIOA_FUNC_R1_GPIOA10_3    0x3
#define GPIO_GPIOA_FUNC_R1_GPIOA10_4    0x4
#define GPIO_GPIOA_FUNC_R1_GPIOA10_5    0x5
#define GPIO_GPIOA_FUNC_R1_GPIOA11_0    0x0
#define GPIO_GPIOA_FUNC_R1_GPIOA11_1    0x1
#define GPIO_GPIOA_FUNC_R1_GPIOA11_2    0x2
#define GPIO_GPIOA_FUNC_R1_GPIOA11_3    0x3
#define GPIO_GPIOA_FUNC_R1_GPIOA11_4    0x4
#define GPIO_GPIOA_FUNC_R1_GPIOA11_5    0x5
#define GPIO_GPIOA_FUNC_R1_GPIOA12_0    0x0
#define GPIO_GPIOA_FUNC_R1_GPIOA12_1    0x1
#define GPIO_GPIOA_FUNC_R1_GPIOA12_2    0x2
#define GPIO_GPIOA_FUNC_R1_GPIOA12_3    0x3
#define GPIO_GPIOA_FUNC_R1_GPIOA12_4    0x4
#define GPIO_GPIOA_FUNC_R1_GPIOA12_5    0x5
#define GPIO_GPIOA_FUNC_R1_GPIOA13_0    0x0
#define GPIO_GPIOA_FUNC_R1_GPIOA13_1    0x1
#define GPIO_GPIOA_FUNC_R1_GPIOA13_2    0x2
#define GPIO_GPIOA_FUNC_R1_GPIOA13_3    0x3
#define GPIO_GPIOA_FUNC_R1_GPIOA13_4    0x4
#define GPIO_GPIOA_FUNC_R1_GPIOA13_5    0x5
#define GPIO_GPIOA_FUNC_R1_GPIOA14_0    0x0
#define GPIO_GPIOA_FUNC_R1_GPIOA14_1    0x1
#define GPIO_GPIOA_FUNC_R1_GPIOA14_2    0x2
#define GPIO_GPIOA_FUNC_R1_GPIOA14_3    0x3
#define GPIO_GPIOA_FUNC_R1_GPIOA14_4    0x4
#define GPIO_GPIOA_FUNC_R1_GPIOA14_5    0x5
#define GPIO_GPIOA_FUNC_R1_GPIOA15_0    0x0
#define GPIO_GPIOA_FUNC_R1_GPIOA15_1    0x1
#define GPIO_GPIOA_FUNC_R1_GPIOA15_2    0x2
#define GPIO_GPIOA_FUNC_R1_GPIOA15_3    0x3
#define GPIO_GPIOA_FUNC_R1_GPIOA15_4    0x4
#define GPIO_GPIOA_FUNC_R1_GPIOA15_5    0x5

/* GPIOA Pull Up or Pull Down Configuration Register0 */
typedef union {
	u32 val;
	struct {
	u32 gpioa0:2;          /* */
	u32 rsvd0:2;           /* */
	u32 gpioa1:2;          /* */
	u32 rsvd1:2;           /* */
	u32 gpioa2:2;          /* */
	u32 rsvd2:2;           /* */
	u32 gpioa3:2;          /* */
	u32 rsvd3:2;           /* */
	u32 gpioa4:2;          /* */
	u32 rsvd4:2;           /* */
	u32 gpioa5:2;          /* */
	u32 rsvd5:2;           /* */
	u32 gpioa6:2;          /* */
	u32 rsvd6:2;           /* */
	u32 gpioa7:2;          /* */
	u32 rsvd7:2;           /* */
	} bits;
} reg_gpio_gpioa_pud_r0_t;

#define GPIO_GPIOA_PUD_R0_GPIOA0_0      0x0
#define GPIO_GPIOA_PUD_R0_GPIOA0_1      0x1
#define GPIO_GPIOA_PUD_R0_GPIOA0_2      0x2
#define GPIO_GPIOA_PUD_R0_GPIOA0_3      0x3
#define GPIO_GPIOA_PUD_R0_GPIOA1_0      0x0
#define GPIO_GPIOA_PUD_R0_GPIOA1_1      0x1
#define GPIO_GPIOA_PUD_R0_GPIOA1_2      0x2
#define GPIO_GPIOA_PUD_R0_GPIOA1_3      0x3
#define GPIO_GPIOA_PUD_R0_GPIOA2_0      0x0
#define GPIO_GPIOA_PUD_R0_GPIOA2_1      0x1
#define GPIO_GPIOA_PUD_R0_GPIOA2_2      0x2
#define GPIO_GPIOA_PUD_R0_GPIOA2_3      0x3
#define GPIO_GPIOA_PUD_R0_GPIOA3_0      0x0
#define GPIO_GPIOA_PUD_R0_GPIOA3_1      0x1
#define GPIO_GPIOA_PUD_R0_GPIOA3_2      0x2
#define GPIO_GPIOA_PUD_R0_GPIOA3_3      0x3
#define GPIO_GPIOA_PUD_R0_GPIOA4_0      0x0
#define GPIO_GPIOA_PUD_R0_GPIOA4_1      0x1
#define GPIO_GPIOA_PUD_R0_GPIOA4_2      0x2
#define GPIO_GPIOA_PUD_R0_GPIOA4_3      0x3
#define GPIO_GPIOA_PUD_R0_GPIOA5_0      0x0
#define GPIO_GPIOA_PUD_R0_GPIOA5_1      0x1
#define GPIO_GPIOA_PUD_R0_GPIOA5_2      0x2
#define GPIO_GPIOA_PUD_R0_GPIOA5_3      0x3
#define GPIO_GPIOA_PUD_R0_GPIOA6_0      0x0
#define GPIO_GPIOA_PUD_R0_GPIOA6_1      0x1
#define GPIO_GPIOA_PUD_R0_GPIOA6_2      0x2
#define GPIO_GPIOA_PUD_R0_GPIOA6_3      0x3
#define GPIO_GPIOA_PUD_R0_GPIOA7_0      0x0
#define GPIO_GPIOA_PUD_R0_GPIOA7_1      0x1
#define GPIO_GPIOA_PUD_R0_GPIOA7_2      0x2
#define GPIO_GPIOA_PUD_R0_GPIOA7_3      0x3

/* GPIOA Pull Up or Pull Down Configuration Register1 */
typedef union {
	u32 val;
	struct {
	u32 gpioa8:2;          /* */
	u32 rsvd0:2;           /* */
	u32 gpioa9:2;          /* */
	u32 rsvd1:2;           /* */
	u32 gpioa10:2;         /* */
	u32 rsvd2:2;           /* */
	u32 gpioa11:2;         /* */
	u32 rsvd3:2;           /* */
	u32 gpioa12:2;         /* */
	u32 rsvd4:2;           /* */
	u32 gpioa13:2;         /* */
	u32 rsvd5:2;           /* */
	u32 gpioa14:2;         /* */
	u32 rsvd6:2;           /* */
	u32 gpioa15:2;         /* */
	u32 rsvd7:2;           /* */
	} bits;
} reg_gpio_gpioa_pud_r1_t;

#define GPIO_GPIOA_PUD_R1_GPIOA8_0      0x0
#define GPIO_GPIOA_PUD_R1_GPIOA8_1      0x1
#define GPIO_GPIOA_PUD_R1_GPIOA8_2      0x2
#define GPIO_GPIOA_PUD_R1_GPIOA8_3      0x3
#define GPIO_GPIOA_PUD_R1_GPIOA9_0      0x0
#define GPIO_GPIOA_PUD_R1_GPIOA9_1      0x1
#define GPIO_GPIOA_PUD_R1_GPIOA9_2      0x2
#define GPIO_GPIOA_PUD_R1_GPIOA9_3      0x3
#define GPIO_GPIOA_PUD_R1_GPIOA10_0     0x0
#define GPIO_GPIOA_PUD_R1_GPIOA10_1     0x1
#define GPIO_GPIOA_PUD_R1_GPIOA10_2     0x2
#define GPIO_GPIOA_PUD_R1_GPIOA10_3     0x3
#define GPIO_GPIOA_PUD_R1_GPIOA11_0     0x0
#define GPIO_GPIOA_PUD_R1_GPIOA11_1     0x1
#define GPIO_GPIOA_PUD_R1_GPIOA11_2     0x2
#define GPIO_GPIOA_PUD_R1_GPIOA11_3     0x3
#define GPIO_GPIOA_PUD_R1_GPIOA12_0     0x0
#define GPIO_GPIOA_PUD_R1_GPIOA12_1     0x1
#define GPIO_GPIOA_PUD_R1_GPIOA12_2     0x2
#define GPIO_GPIOA_PUD_R1_GPIOA12_3     0x3
#define GPIO_GPIOA_PUD_R1_GPIOA13_0     0x0
#define GPIO_GPIOA_PUD_R1_GPIOA13_1     0x1
#define GPIO_GPIOA_PUD_R1_GPIOA13_2     0x2
#define GPIO_GPIOA_PUD_R1_GPIOA13_3     0x3
#define GPIO_GPIOA_PUD_R1_GPIOA14_0     0x0
#define GPIO_GPIOA_PUD_R1_GPIOA14_1     0x1
#define GPIO_GPIOA_PUD_R1_GPIOA14_2     0x2
#define GPIO_GPIOA_PUD_R1_GPIOA14_3     0x3
#define GPIO_GPIOA_PUD_R1_GPIOA15_0     0x0
#define GPIO_GPIOA_PUD_R1_GPIOA15_1     0x1
#define GPIO_GPIOA_PUD_R1_GPIOA15_2     0x2
#define GPIO_GPIOA_PUD_R1_GPIOA15_3     0x3

/* GPIOA Driver Configuration Register0 */
typedef union {
	u32 val;
	struct {
	u32 gpioa0:3;          /* */
	u32 rsvd0:1;           /* */
	u32 gpioa1:3;          /* */
	u32 rsvd1:1;           /* */
	u32 gpioa2:3;          /* */
	u32 rsvd2:1;           /* */
	u32 gpioa3:3;          /* */
	u32 rsvd3:1;           /* */
	u32 gpioa4:3;          /* */
	u32 rsvd4:1;           /* */
	u32 gpioa5:3;          /* */
	u32 rsvd5:1;           /* */
	u32 gpioa6:3;          /* */
	u32 rsvd6:1;           /* */
	u32 gpioa7:3;          /* */
	u32 rsvd7:1;           /* */
	} bits;
} reg_gpio_gpioa_drv_r0_t;

#define GPIO_GPIOA_DRV_R0_GPIOA0_0      0x0
#define GPIO_GPIOA_DRV_R0_GPIOA0_1      0x1
#define GPIO_GPIOA_DRV_R0_GPIOA0_2      0x2
#define GPIO_GPIOA_DRV_R0_GPIOA0_3      0x3
#define GPIO_GPIOA_DRV_R0_GPIOA0_4      0x4
#define GPIO_GPIOA_DRV_R0_GPIOA0_5      0x5
#define GPIO_GPIOA_DRV_R0_GPIOA0_6      0x6
#define GPIO_GPIOA_DRV_R0_GPIOA0_7      0x7
#define GPIO_GPIOA_DRV_R0_GPIOA1_0      0x0
#define GPIO_GPIOA_DRV_R0_GPIOA1_1      0x1
#define GPIO_GPIOA_DRV_R0_GPIOA1_2      0x2
#define GPIO_GPIOA_DRV_R0_GPIOA1_3      0x3
#define GPIO_GPIOA_DRV_R0_GPIOA1_4      0x4
#define GPIO_GPIOA_DRV_R0_GPIOA1_5      0x5
#define GPIO_GPIOA_DRV_R0_GPIOA1_6      0x6
#define GPIO_GPIOA_DRV_R0_GPIOA1_7      0x7
#define GPIO_GPIOA_DRV_R0_GPIOA2_0      0x0
#define GPIO_GPIOA_DRV_R0_GPIOA2_1      0x1
#define GPIO_GPIOA_DRV_R0_GPIOA2_2      0x2
#define GPIO_GPIOA_DRV_R0_GPIOA2_3      0x3
#define GPIO_GPIOA_DRV_R0_GPIOA2_4      0x4
#define GPIO_GPIOA_DRV_R0_GPIOA2_5      0x5
#define GPIO_GPIOA_DRV_R0_GPIOA2_6      0x6
#define GPIO_GPIOA_DRV_R0_GPIOA2_7      0x7
#define GPIO_GPIOA_DRV_R0_GPIOA3_0      0x0
#define GPIO_GPIOA_DRV_R0_GPIOA3_1      0x1
#define GPIO_GPIOA_DRV_R0_GPIOA3_2      0x2
#define GPIO_GPIOA_DRV_R0_GPIOA3_3      0x3
#define GPIO_GPIOA_DRV_R0_GPIOA3_4      0x4
#define GPIO_GPIOA_DRV_R0_GPIOA3_5      0x5
#define GPIO_GPIOA_DRV_R0_GPIOA3_6      0x6
#define GPIO_GPIOA_DRV_R0_GPIOA3_7      0x7
#define GPIO_GPIOA_DRV_R0_GPIOA4_0      0x0
#define GPIO_GPIOA_DRV_R0_GPIOA4_1      0x1
#define GPIO_GPIOA_DRV_R0_GPIOA4_2      0x2
#define GPIO_GPIOA_DRV_R0_GPIOA4_3      0x3
#define GPIO_GPIOA_DRV_R0_GPIOA4_4      0x4
#define GPIO_GPIOA_DRV_R0_GPIOA4_5      0x5
#define GPIO_GPIOA_DRV_R0_GPIOA4_6      0x6
#define GPIO_GPIOA_DRV_R0_GPIOA4_7      0x7
#define GPIO_GPIOA_DRV_R0_GPIOA5_0      0x0
#define GPIO_GPIOA_DRV_R0_GPIOA5_1      0x1
#define GPIO_GPIOA_DRV_R0_GPIOA5_2      0x2
#define GPIO_GPIOA_DRV_R0_GPIOA5_3      0x3
#define GPIO_GPIOA_DRV_R0_GPIOA5_4      0x4
#define GPIO_GPIOA_DRV_R0_GPIOA5_5      0x5
#define GPIO_GPIOA_DRV_R0_GPIOA5_6      0x6
#define GPIO_GPIOA_DRV_R0_GPIOA5_7      0x7
#define GPIO_GPIOA_DRV_R0_GPIOA6_0      0x0
#define GPIO_GPIOA_DRV_R0_GPIOA6_1      0x1
#define GPIO_GPIOA_DRV_R0_GPIOA6_2      0x2
#define GPIO_GPIOA_DRV_R0_GPIOA6_3      0x3
#define GPIO_GPIOA_DRV_R0_GPIOA6_4      0x4
#define GPIO_GPIOA_DRV_R0_GPIOA6_5      0x5
#define GPIO_GPIOA_DRV_R0_GPIOA6_6      0x6
#define GPIO_GPIOA_DRV_R0_GPIOA6_7      0x7
#define GPIO_GPIOA_DRV_R0_GPIOA7_0      0x0
#define GPIO_GPIOA_DRV_R0_GPIOA7_1      0x1
#define GPIO_GPIOA_DRV_R0_GPIOA7_2      0x2
#define GPIO_GPIOA_DRV_R0_GPIOA7_3      0x3
#define GPIO_GPIOA_DRV_R0_GPIOA7_4      0x4
#define GPIO_GPIOA_DRV_R0_GPIOA7_5      0x5
#define GPIO_GPIOA_DRV_R0_GPIOA7_6      0x6
#define GPIO_GPIOA_DRV_R0_GPIOA7_7      0x7

/* GPIOA Driver Configuration Register1 */
typedef union {
	u32 val;
	struct {
	u32 gpioa8:3;          /* */
	u32 rsvd0:1;           /* */
	u32 gpioa9:3;          /* */
	u32 rsvd1:1;           /* */
	u32 gpioa10:3;         /* */
	u32 rsvd2:1;           /* */
	u32 gpioa11:3;         /* */
	u32 rsvd3:1;           /* */
	u32 gpioa12:3;         /* */
	u32 rsvd4:1;           /* */
	u32 gpioa13:3;         /* */
	u32 rsvd5:1;           /* */
	u32 gpioa14:3;         /* */
	u32 rsvd6:1;           /* */
	u32 gpioa15:3;         /* */
	u32 rsvd7:1;           /* */
	} bits;
} reg_gpio_gpioa_drv_r1_t;

#define GPIO_GPIOA_DRV_R1_GPIOA8_0      0x0
#define GPIO_GPIOA_DRV_R1_GPIOA8_1      0x1
#define GPIO_GPIOA_DRV_R1_GPIOA8_2      0x2
#define GPIO_GPIOA_DRV_R1_GPIOA8_3      0x3
#define GPIO_GPIOA_DRV_R1_GPIOA8_4      0x4
#define GPIO_GPIOA_DRV_R1_GPIOA8_5      0x5
#define GPIO_GPIOA_DRV_R1_GPIOA8_6      0x6
#define GPIO_GPIOA_DRV_R1_GPIOA8_7      0x7
#define GPIO_GPIOA_DRV_R1_GPIOA9_0      0x0
#define GPIO_GPIOA_DRV_R1_GPIOA9_1      0x1
#define GPIO_GPIOA_DRV_R1_GPIOA9_2      0x2
#define GPIO_GPIOA_DRV_R1_GPIOA9_3      0x3
#define GPIO_GPIOA_DRV_R1_GPIOA9_4      0x4
#define GPIO_GPIOA_DRV_R1_GPIOA9_5      0x5
#define GPIO_GPIOA_DRV_R1_GPIOA9_6      0x6
#define GPIO_GPIOA_DRV_R1_GPIOA9_7      0x7
#define GPIO_GPIOA_DRV_R1_GPIOA10_0     0x0
#define GPIO_GPIOA_DRV_R1_GPIOA10_1     0x1
#define GPIO_GPIOA_DRV_R1_GPIOA10_2     0x2
#define GPIO_GPIOA_DRV_R1_GPIOA10_3     0x3
#define GPIO_GPIOA_DRV_R1_GPIOA10_4     0x4
#define GPIO_GPIOA_DRV_R1_GPIOA10_5     0x5
#define GPIO_GPIOA_DRV_R1_GPIOA10_6     0x6
#define GPIO_GPIOA_DRV_R1_GPIOA10_7     0x7
#define GPIO_GPIOA_DRV_R1_GPIOA11_0     0x0
#define GPIO_GPIOA_DRV_R1_GPIOA11_1     0x1
#define GPIO_GPIOA_DRV_R1_GPIOA11_2     0x2
#define GPIO_GPIOA_DRV_R1_GPIOA11_3     0x3
#define GPIO_GPIOA_DRV_R1_GPIOA11_4     0x4
#define GPIO_GPIOA_DRV_R1_GPIOA11_5     0x5
#define GPIO_GPIOA_DRV_R1_GPIOA11_6     0x6
#define GPIO_GPIOA_DRV_R1_GPIOA11_7     0x7
#define GPIO_GPIOA_DRV_R1_GPIOA12_0     0x0
#define GPIO_GPIOA_DRV_R1_GPIOA12_1     0x1
#define GPIO_GPIOA_DRV_R1_GPIOA12_2     0x2
#define GPIO_GPIOA_DRV_R1_GPIOA12_3     0x3
#define GPIO_GPIOA_DRV_R1_GPIOA12_4     0x4
#define GPIO_GPIOA_DRV_R1_GPIOA12_5     0x5
#define GPIO_GPIOA_DRV_R1_GPIOA12_6     0x6
#define GPIO_GPIOA_DRV_R1_GPIOA12_7     0x7
#define GPIO_GPIOA_DRV_R1_GPIOA13_0     0x0
#define GPIO_GPIOA_DRV_R1_GPIOA13_1     0x1
#define GPIO_GPIOA_DRV_R1_GPIOA13_2     0x2
#define GPIO_GPIOA_DRV_R1_GPIOA13_3     0x3
#define GPIO_GPIOA_DRV_R1_GPIOA13_4     0x4
#define GPIO_GPIOA_DRV_R1_GPIOA13_5     0x5
#define GPIO_GPIOA_DRV_R1_GPIOA13_6     0x6
#define GPIO_GPIOA_DRV_R1_GPIOA13_7     0x7
#define GPIO_GPIOA_DRV_R1_GPIOA14_0     0x0
#define GPIO_GPIOA_DRV_R1_GPIOA14_1     0x1
#define GPIO_GPIOA_DRV_R1_GPIOA14_2     0x2
#define GPIO_GPIOA_DRV_R1_GPIOA14_3     0x3
#define GPIO_GPIOA_DRV_R1_GPIOA14_4     0x4
#define GPIO_GPIOA_DRV_R1_GPIOA14_5     0x5
#define GPIO_GPIOA_DRV_R1_GPIOA14_6     0x6
#define GPIO_GPIOA_DRV_R1_GPIOA14_7     0x7
#define GPIO_GPIOA_DRV_R1_GPIOA15_0     0x0
#define GPIO_GPIOA_DRV_R1_GPIOA15_1     0x1
#define GPIO_GPIOA_DRV_R1_GPIOA15_2     0x2
#define GPIO_GPIOA_DRV_R1_GPIOA15_3     0x3
#define GPIO_GPIOA_DRV_R1_GPIOA15_4     0x4
#define GPIO_GPIOA_DRV_R1_GPIOA15_5     0x5
#define GPIO_GPIOA_DRV_R1_GPIOA15_6     0x6
#define GPIO_GPIOA_DRV_R1_GPIOA15_7     0x7

/* GPIOA Data Register */
typedef union {
	u32 val;
	struct {
	u32 dat:16;            /* */
	u32 rsvd0:16;          /* */
	} bits;
} reg_gpio_gpioa_data_t;

/* GPIOA Pull Up/Down Resistors Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 pull_cfg:16;       /* */
	u32 rsvd0:16;          /* */
	} bits;
} reg_gpio_gpioa_pr_r_t;

#define GPIO_GPIOA_PR_R_PULL_CFG_0      0x0
#define GPIO_GPIOA_PR_R_PULL_CFG_1      0x1

/* GPIOB Function Configuration Register0 */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:8;           /* */
	u32 gpiob2:4;          /* */
	u32 gpiob3:4;          /* */
	u32 gpiob4:4;          /* */
	u32 gpiob5:4;          /* */
	u32 gpiob6:4;          /* */
	u32 gpiob7:4;          /* */
	} bits;
} reg_gpio_gpiob_func_r0_t;

#define GPIO_GPIOB_FUNC_R0_GPIOB2_0     0x0
#define GPIO_GPIOB_FUNC_R0_GPIOB2_1     0x1
#define GPIO_GPIOB_FUNC_R0_GPIOB2_2     0x2
#define GPIO_GPIOB_FUNC_R0_GPIOB2_3     0x3
#define GPIO_GPIOB_FUNC_R0_GPIOB2_4     0x4
#define GPIO_GPIOB_FUNC_R0_GPIOB2_5     0x5
#define GPIO_GPIOB_FUNC_R0_GPIOB2_6     0x6
#define GPIO_GPIOB_FUNC_R0_GPIOB3_0     0x0
#define GPIO_GPIOB_FUNC_R0_GPIOB3_1     0x1
#define GPIO_GPIOB_FUNC_R0_GPIOB3_2     0x2
#define GPIO_GPIOB_FUNC_R0_GPIOB3_3     0x3
#define GPIO_GPIOB_FUNC_R0_GPIOB3_4     0x4
#define GPIO_GPIOB_FUNC_R0_GPIOB3_5     0x5
#define GPIO_GPIOB_FUNC_R0_GPIOB3_6     0x6
#define GPIO_GPIOB_FUNC_R0_GPIOB4_0     0x0
#define GPIO_GPIOB_FUNC_R0_GPIOB4_1     0x1
#define GPIO_GPIOB_FUNC_R0_GPIOB4_2     0x2
#define GPIO_GPIOB_FUNC_R0_GPIOB4_3     0x3
#define GPIO_GPIOB_FUNC_R0_GPIOB4_4     0x4
#define GPIO_GPIOB_FUNC_R0_GPIOB4_5     0x5
#define GPIO_GPIOB_FUNC_R0_GPIOB4_6     0x6
#define GPIO_GPIOB_FUNC_R0_GPIOB4_7     0x7
#define GPIO_GPIOB_FUNC_R0_GPIOB5_0     0x0
#define GPIO_GPIOB_FUNC_R0_GPIOB5_1     0x1
#define GPIO_GPIOB_FUNC_R0_GPIOB5_2     0x2
#define GPIO_GPIOB_FUNC_R0_GPIOB5_3     0x3
#define GPIO_GPIOB_FUNC_R0_GPIOB5_4     0x4
#define GPIO_GPIOB_FUNC_R0_GPIOB5_5     0x5
#define GPIO_GPIOB_FUNC_R0_GPIOB5_6     0x6
#define GPIO_GPIOB_FUNC_R0_GPIOB5_7     0x7
#define GPIO_GPIOB_FUNC_R0_GPIOB6_0     0x0
#define GPIO_GPIOB_FUNC_R0_GPIOB6_1     0x1
#define GPIO_GPIOB_FUNC_R0_GPIOB6_2     0x2
#define GPIO_GPIOB_FUNC_R0_GPIOB6_3     0x3
#define GPIO_GPIOB_FUNC_R0_GPIOB6_4     0x4
#define GPIO_GPIOB_FUNC_R0_GPIOB6_5     0x5
#define GPIO_GPIOB_FUNC_R0_GPIOB6_6     0x6
#define GPIO_GPIOB_FUNC_R0_GPIOB7_0     0x0
#define GPIO_GPIOB_FUNC_R0_GPIOB7_1     0x1
#define GPIO_GPIOB_FUNC_R0_GPIOB7_2     0x2
#define GPIO_GPIOB_FUNC_R0_GPIOB7_3     0x3
#define GPIO_GPIOB_FUNC_R0_GPIOB7_4     0x4
#define GPIO_GPIOB_FUNC_R0_GPIOB7_5     0x5
#define GPIO_GPIOB_FUNC_R0_GPIOB7_6     0x6
#define GPIO_GPIOB_FUNC_R0_GPIOB7_7     0x7

/* GPIOB Function Configuration Register1 */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:8;           /* */
	u32 gpiob10:4;         /* */
	u32 gpiob11:4;         /* */
	u32 gpiob12:4;         /* */
	u32 gpiob13:4;         /* */
	u32 gpiob14:4;         /* */
	u32 gpiob15:4;         /* */
	} bits;
} reg_gpio_gpiob_func_r1_t;

#define GPIO_GPIOB_FUNC_R1_GPIOB10_0    0x0
#define GPIO_GPIOB_FUNC_R1_GPIOB10_1    0x1
#define GPIO_GPIOB_FUNC_R1_GPIOB10_2    0x2
#define GPIO_GPIOB_FUNC_R1_GPIOB10_3    0x3
#define GPIO_GPIOB_FUNC_R1_GPIOB10_4    0x4
#define GPIO_GPIOB_FUNC_R1_GPIOB10_5    0x5
#define GPIO_GPIOB_FUNC_R1_GPIOB10_6    0x6
#define GPIO_GPIOB_FUNC_R1_GPIOB10_7    0x7
#define GPIO_GPIOB_FUNC_R1_GPIOB11_0    0x0
#define GPIO_GPIOB_FUNC_R1_GPIOB11_1    0x1
#define GPIO_GPIOB_FUNC_R1_GPIOB11_2    0x2
#define GPIO_GPIOB_FUNC_R1_GPIOB11_3    0x3
#define GPIO_GPIOB_FUNC_R1_GPIOB11_4    0x4
#define GPIO_GPIOB_FUNC_R1_GPIOB11_5    0x5
#define GPIO_GPIOB_FUNC_R1_GPIOB11_6    0x6
#define GPIO_GPIOB_FUNC_R1_GPIOB11_7    0x7
#define GPIO_GPIOB_FUNC_R1_GPIOB12_0    0x0
#define GPIO_GPIOB_FUNC_R1_GPIOB12_1    0x1
#define GPIO_GPIOB_FUNC_R1_GPIOB12_2    0x2
#define GPIO_GPIOB_FUNC_R1_GPIOB12_3    0x3
#define GPIO_GPIOB_FUNC_R1_GPIOB12_4    0x4
#define GPIO_GPIOB_FUNC_R1_GPIOB12_5    0x5
#define GPIO_GPIOB_FUNC_R1_GPIOB12_6    0x6
#define GPIO_GPIOB_FUNC_R1_GPIOB13_0    0x0
#define GPIO_GPIOB_FUNC_R1_GPIOB13_1    0x1
#define GPIO_GPIOB_FUNC_R1_GPIOB13_2    0x2
#define GPIO_GPIOB_FUNC_R1_GPIOB13_3    0x3
#define GPIO_GPIOB_FUNC_R1_GPIOB13_4    0x4
#define GPIO_GPIOB_FUNC_R1_GPIOB13_5    0x5
#define GPIO_GPIOB_FUNC_R1_GPIOB13_6    0x6
#define GPIO_GPIOB_FUNC_R1_GPIOB14_0    0x0
#define GPIO_GPIOB_FUNC_R1_GPIOB14_1    0x1
#define GPIO_GPIOB_FUNC_R1_GPIOB14_2    0x2
#define GPIO_GPIOB_FUNC_R1_GPIOB14_3    0x3
#define GPIO_GPIOB_FUNC_R1_GPIOB14_4    0x4
#define GPIO_GPIOB_FUNC_R1_GPIOB14_5    0x5
#define GPIO_GPIOB_FUNC_R1_GPIOB15_0    0x0
#define GPIO_GPIOB_FUNC_R1_GPIOB15_1    0x1
#define GPIO_GPIOB_FUNC_R1_GPIOB15_2    0x2
#define GPIO_GPIOB_FUNC_R1_GPIOB15_3    0x3
#define GPIO_GPIOB_FUNC_R1_GPIOB15_4    0x4
#define GPIO_GPIOB_FUNC_R1_GPIOB15_5    0x5

/* GPIOB Function Configuration Register2 */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:8;           /* */
	u32 gpiob18:4;         /* */
	u32 gpiob19:4;         /* */
	u32 gpiob20:4;         /* */
	u32 gpiob21:4;         /* */
	u32 gpiob22:4;         /* */
	u32 gpiob23:4;         /* */
	} bits;
} reg_gpio_gpiob_func_r2_t;

#define GPIO_GPIOB_FUNC_R2_GPIOB18_0    0x0
#define GPIO_GPIOB_FUNC_R2_GPIOB18_1    0x1
#define GPIO_GPIOB_FUNC_R2_GPIOB18_2    0x2
#define GPIO_GPIOB_FUNC_R2_GPIOB18_3    0x3
#define GPIO_GPIOB_FUNC_R2_GPIOB18_4    0x4
#define GPIO_GPIOB_FUNC_R2_GPIOB18_5    0x5
#define GPIO_GPIOB_FUNC_R2_GPIOB19_0    0x0
#define GPIO_GPIOB_FUNC_R2_GPIOB19_1    0x1
#define GPIO_GPIOB_FUNC_R2_GPIOB19_2    0x2
#define GPIO_GPIOB_FUNC_R2_GPIOB19_3    0x3
#define GPIO_GPIOB_FUNC_R2_GPIOB19_4    0x4
#define GPIO_GPIOB_FUNC_R2_GPIOB19_5    0x5
#define GPIO_GPIOB_FUNC_R2_GPIOB20_0    0x0
#define GPIO_GPIOB_FUNC_R2_GPIOB20_1    0x1
#define GPIO_GPIOB_FUNC_R2_GPIOB20_2    0x2
#define GPIO_GPIOB_FUNC_R2_GPIOB20_3    0x3
#define GPIO_GPIOB_FUNC_R2_GPIOB20_4    0x4
#define GPIO_GPIOB_FUNC_R2_GPIOB20_5    0x5
#define GPIO_GPIOB_FUNC_R2_GPIOB21_0    0x0
#define GPIO_GPIOB_FUNC_R2_GPIOB21_1    0x1
#define GPIO_GPIOB_FUNC_R2_GPIOB21_2    0x2
#define GPIO_GPIOB_FUNC_R2_GPIOB21_3    0x3
#define GPIO_GPIOB_FUNC_R2_GPIOB21_4    0x4
#define GPIO_GPIOB_FUNC_R2_GPIOB21_5    0x5
#define GPIO_GPIOB_FUNC_R2_GPIOB22_0    0x0
#define GPIO_GPIOB_FUNC_R2_GPIOB22_1    0x1
#define GPIO_GPIOB_FUNC_R2_GPIOB22_2    0x2
#define GPIO_GPIOB_FUNC_R2_GPIOB22_3    0x3
#define GPIO_GPIOB_FUNC_R2_GPIOB22_4    0x4
#define GPIO_GPIOB_FUNC_R2_GPIOB22_5    0x5
#define GPIO_GPIOB_FUNC_R2_GPIOB23_0    0x0
#define GPIO_GPIOB_FUNC_R2_GPIOB23_1    0x1
#define GPIO_GPIOB_FUNC_R2_GPIOB23_2    0x2
#define GPIO_GPIOB_FUNC_R2_GPIOB23_3    0x3
#define GPIO_GPIOB_FUNC_R2_GPIOB23_4    0x4
#define GPIO_GPIOB_FUNC_R2_GPIOB23_5    0x5

/* GPIOB Function Configuration Register3 */
typedef union {
	u32 val;
	struct {
	u32 gpiob24:4;         /* */
	u32 gpiob25:4;         /* */
	u32 gpiob26:4;         /* */
	u32 gpiob27:4;         /* */
	u32 rsvd0:16;          /* */
	} bits;
} reg_gpio_gpiob_func_r3_t;

#define GPIO_GPIOB_FUNC_R3_GPIOB24_0    0x0
#define GPIO_GPIOB_FUNC_R3_GPIOB24_1    0x1
#define GPIO_GPIOB_FUNC_R3_GPIOB24_2    0x2
#define GPIO_GPIOB_FUNC_R3_GPIOB24_3    0x3
#define GPIO_GPIOB_FUNC_R3_GPIOB24_4    0x4
#define GPIO_GPIOB_FUNC_R3_GPIOB24_5    0x5
#define GPIO_GPIOB_FUNC_R3_GPIOB25_0    0x0
#define GPIO_GPIOB_FUNC_R3_GPIOB25_1    0x1
#define GPIO_GPIOB_FUNC_R3_GPIOB25_2    0x2
#define GPIO_GPIOB_FUNC_R3_GPIOB25_3    0x3
#define GPIO_GPIOB_FUNC_R3_GPIOB25_4    0x4
#define GPIO_GPIOB_FUNC_R3_GPIOB25_5    0x5
#define GPIO_GPIOB_FUNC_R3_GPIOB26_0    0x0
#define GPIO_GPIOB_FUNC_R3_GPIOB26_1    0x1
#define GPIO_GPIOB_FUNC_R3_GPIOB26_2    0x2
#define GPIO_GPIOB_FUNC_R3_GPIOB26_3    0x3
#define GPIO_GPIOB_FUNC_R3_GPIOB26_4    0x4
#define GPIO_GPIOB_FUNC_R3_GPIOB26_5    0x5
#define GPIO_GPIOB_FUNC_R3_GPIOB27_0    0x0
#define GPIO_GPIOB_FUNC_R3_GPIOB27_1    0x1
#define GPIO_GPIOB_FUNC_R3_GPIOB27_2    0x2
#define GPIO_GPIOB_FUNC_R3_GPIOB27_3    0x3
#define GPIO_GPIOB_FUNC_R3_GPIOB27_4    0x4
#define GPIO_GPIOB_FUNC_R3_GPIOB27_5    0x5

/* GPIOB Pull Up or Pull Down Configuration Register0 */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:8;           /* */
	u32 gpiob2:2;          /* */
	u32 rsvd1:2;           /* */
	u32 gpiob3:2;          /* */
	u32 rsvd2:2;           /* */
	u32 gpiob4:2;          /* */
	u32 rsvd3:2;           /* */
	u32 gpiob5:2;          /* */
	u32 rsvd4:2;           /* */
	u32 gpiob6:2;          /* */
	u32 rsvd5:2;           /* */
	u32 gpiob7:2;          /* */
	u32 rsvd6:2;           /* */
	} bits;
} reg_gpio_gpiob_pud_r0_t;

#define GPIO_GPIOB_PUD_R0_GPIOB2_0      0x0
#define GPIO_GPIOB_PUD_R0_GPIOB2_1      0x1
#define GPIO_GPIOB_PUD_R0_GPIOB2_2      0x2
#define GPIO_GPIOB_PUD_R0_GPIOB2_3      0x3
#define GPIO_GPIOB_PUD_R0_GPIOB3_0      0x0
#define GPIO_GPIOB_PUD_R0_GPIOB3_1      0x1
#define GPIO_GPIOB_PUD_R0_GPIOB3_2      0x2
#define GPIO_GPIOB_PUD_R0_GPIOB3_3      0x3
#define GPIO_GPIOB_PUD_R0_GPIOB4_0      0x0
#define GPIO_GPIOB_PUD_R0_GPIOB4_1      0x1
#define GPIO_GPIOB_PUD_R0_GPIOB4_2      0x2
#define GPIO_GPIOB_PUD_R0_GPIOB4_3      0x3
#define GPIO_GPIOB_PUD_R0_GPIOB5_0      0x0
#define GPIO_GPIOB_PUD_R0_GPIOB5_1      0x1
#define GPIO_GPIOB_PUD_R0_GPIOB5_2      0x2
#define GPIO_GPIOB_PUD_R0_GPIOB5_3      0x3
#define GPIO_GPIOB_PUD_R0_GPIOB6_0      0x0
#define GPIO_GPIOB_PUD_R0_GPIOB6_1      0x1
#define GPIO_GPIOB_PUD_R0_GPIOB6_2      0x2
#define GPIO_GPIOB_PUD_R0_GPIOB6_3      0x3
#define GPIO_GPIOB_PUD_R0_GPIOB7_0      0x0
#define GPIO_GPIOB_PUD_R0_GPIOB7_1      0x1
#define GPIO_GPIOB_PUD_R0_GPIOB7_2      0x2
#define GPIO_GPIOB_PUD_R0_GPIOB7_3      0x3

/* GPIOB Pull Up or Pull Down Configuration Register1 */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:8;           /* */
	u32 gpiob10:2;         /* */
	u32 rsvd1:2;           /* */
	u32 gpiob11:2;         /* */
	u32 rsvd2:2;           /* */
	u32 gpiob12:2;         /* */
	u32 rsvd3:2;           /* */
	u32 gpiob13:2;         /* */
	u32 rsvd4:2;           /* */
	u32 gpiob14:2;         /* */
	u32 rsvd5:2;           /* */
	u32 gpiob15:2;         /* */
	u32 rsvd6:2;           /* */
	} bits;
} reg_gpio_gpiob_pud_r1_t;

#define GPIO_GPIOB_PUD_R1_GPIOB10_0     0x0
#define GPIO_GPIOB_PUD_R1_GPIOB10_1     0x1
#define GPIO_GPIOB_PUD_R1_GPIOB10_2     0x2
#define GPIO_GPIOB_PUD_R1_GPIOB10_3     0x3
#define GPIO_GPIOB_PUD_R1_GPIOB11_0     0x0
#define GPIO_GPIOB_PUD_R1_GPIOB11_1     0x1
#define GPIO_GPIOB_PUD_R1_GPIOB11_2     0x2
#define GPIO_GPIOB_PUD_R1_GPIOB11_3     0x3
#define GPIO_GPIOB_PUD_R1_GPIOB12_0     0x0
#define GPIO_GPIOB_PUD_R1_GPIOB12_1     0x1
#define GPIO_GPIOB_PUD_R1_GPIOB12_2     0x2
#define GPIO_GPIOB_PUD_R1_GPIOB12_3     0x3
#define GPIO_GPIOB_PUD_R1_GPIOB13_0     0x0
#define GPIO_GPIOB_PUD_R1_GPIOB13_1     0x1
#define GPIO_GPIOB_PUD_R1_GPIOB13_2     0x2
#define GPIO_GPIOB_PUD_R1_GPIOB13_3     0x3
#define GPIO_GPIOB_PUD_R1_GPIOB14_0     0x0
#define GPIO_GPIOB_PUD_R1_GPIOB14_1     0x1
#define GPIO_GPIOB_PUD_R1_GPIOB14_2     0x2
#define GPIO_GPIOB_PUD_R1_GPIOB14_3     0x3
#define GPIO_GPIOB_PUD_R1_GPIOB15_0     0x0
#define GPIO_GPIOB_PUD_R1_GPIOB15_1     0x1
#define GPIO_GPIOB_PUD_R1_GPIOB15_2     0x2
#define GPIO_GPIOB_PUD_R1_GPIOB15_3     0x3

/* GPIOB Pull Up or Pull Down Configuration Register2 */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:8;           /* */
	u32 gpiob18:2;         /* */
	u32 rsvd1:2;           /* */
	u32 gpiob19:2;         /* */
	u32 rsvd2:2;           /* */
	u32 gpiob20:2;         /* */
	u32 rsvd3:2;           /* */
	u32 gpiob21:2;         /* */
	u32 rsvd4:2;           /* */
	u32 gpiob22:2;         /* */
	u32 rsvd5:2;           /* */
	u32 gpiob23:2;         /* */
	u32 rsvd6:2;           /* */
	} bits;
} reg_gpio_gpiob_pud_r2_t;

#define GPIO_GPIOB_PUD_R2_GPIOB18_0     0x0
#define GPIO_GPIOB_PUD_R2_GPIOB18_1     0x1
#define GPIO_GPIOB_PUD_R2_GPIOB18_2     0x2
#define GPIO_GPIOB_PUD_R2_GPIOB18_3     0x3
#define GPIO_GPIOB_PUD_R2_GPIOB19_0     0x0
#define GPIO_GPIOB_PUD_R2_GPIOB19_1     0x1
#define GPIO_GPIOB_PUD_R2_GPIOB19_2     0x2
#define GPIO_GPIOB_PUD_R2_GPIOB19_3     0x3
#define GPIO_GPIOB_PUD_R2_GPIOB20_0     0x0
#define GPIO_GPIOB_PUD_R2_GPIOB20_1     0x1
#define GPIO_GPIOB_PUD_R2_GPIOB20_2     0x2
#define GPIO_GPIOB_PUD_R2_GPIOB20_3     0x3
#define GPIO_GPIOB_PUD_R2_GPIOB21_0     0x0
#define GPIO_GPIOB_PUD_R2_GPIOB21_1     0x1
#define GPIO_GPIOB_PUD_R2_GPIOB21_2     0x2
#define GPIO_GPIOB_PUD_R2_GPIOB21_3     0x3
#define GPIO_GPIOB_PUD_R2_GPIOB22_0     0x0
#define GPIO_GPIOB_PUD_R2_GPIOB22_1     0x1
#define GPIO_GPIOB_PUD_R2_GPIOB22_2     0x2
#define GPIO_GPIOB_PUD_R2_GPIOB22_3     0x3
#define GPIO_GPIOB_PUD_R2_GPIOB23_0     0x0
#define GPIO_GPIOB_PUD_R2_GPIOB23_1     0x1
#define GPIO_GPIOB_PUD_R2_GPIOB23_2     0x2
#define GPIO_GPIOB_PUD_R2_GPIOB23_3     0x3

/* GPIOB Pull Up or Pull Down Configuration Register3 */
typedef union {
	u32 val;
	struct {
	u32 gpiob24:2;         /* */
	u32 rsvd0:2;           /* */
	u32 gpiob25:2;         /* */
	u32 rsvd1:2;           /* */
	u32 gpiob26:2;         /* */
	u32 rsvd2:2;           /* */
	u32 gpiob27:2;         /* */
	u32 rsvd3:18;          /* */
	} bits;
} reg_gpio_gpiob_pud_r3_t;

#define GPIO_GPIOB_PUD_R3_GPIOB24_0     0x0
#define GPIO_GPIOB_PUD_R3_GPIOB24_1     0x1
#define GPIO_GPIOB_PUD_R3_GPIOB24_2     0x2
#define GPIO_GPIOB_PUD_R3_GPIOB24_3     0x3
#define GPIO_GPIOB_PUD_R3_GPIOB25_0     0x0
#define GPIO_GPIOB_PUD_R3_GPIOB25_1     0x1
#define GPIO_GPIOB_PUD_R3_GPIOB25_2     0x2
#define GPIO_GPIOB_PUD_R3_GPIOB25_3     0x3
#define GPIO_GPIOB_PUD_R3_GPIOB26_0     0x0
#define GPIO_GPIOB_PUD_R3_GPIOB26_1     0x1
#define GPIO_GPIOB_PUD_R3_GPIOB26_2     0x2
#define GPIO_GPIOB_PUD_R3_GPIOB26_3     0x3
#define GPIO_GPIOB_PUD_R3_GPIOB27_0     0x0
#define GPIO_GPIOB_PUD_R3_GPIOB27_1     0x1
#define GPIO_GPIOB_PUD_R3_GPIOB27_2     0x2
#define GPIO_GPIOB_PUD_R3_GPIOB27_3     0x3

/* GPIOB Driver Configuration Register0 */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:8;           /* */
	u32 gpiob2:3;          /* */
	u32 rsvd1:1;           /* */
	u32 gpiob3:3;          /* */
	u32 rsvd2:1;           /* */
	u32 gpiob4:3;          /* */
	u32 rsvd3:1;           /* */
	u32 gpiob5:3;          /* */
	u32 rsvd4:1;           /* */
	u32 gpiob6:3;          /* */
	u32 rsvd5:1;           /* */
	u32 gpiob7:3;          /* */
	u32 rsvd6:1;           /* */
	} bits;
} reg_gpio_gpiob_drv_r0_t;

#define GPIO_GPIOB_DRV_R0_GPIOB2_0      0x0
#define GPIO_GPIOB_DRV_R0_GPIOB2_1      0x1
#define GPIO_GPIOB_DRV_R0_GPIOB2_2      0x2
#define GPIO_GPIOB_DRV_R0_GPIOB2_3      0x3
#define GPIO_GPIOB_DRV_R0_GPIOB2_4      0x4
#define GPIO_GPIOB_DRV_R0_GPIOB2_5      0x5
#define GPIO_GPIOB_DRV_R0_GPIOB2_6      0x6
#define GPIO_GPIOB_DRV_R0_GPIOB2_7      0x7
#define GPIO_GPIOB_DRV_R0_GPIOB3_0      0x0
#define GPIO_GPIOB_DRV_R0_GPIOB3_1      0x1
#define GPIO_GPIOB_DRV_R0_GPIOB3_2      0x2
#define GPIO_GPIOB_DRV_R0_GPIOB3_3      0x3
#define GPIO_GPIOB_DRV_R0_GPIOB3_4      0x4
#define GPIO_GPIOB_DRV_R0_GPIOB3_5      0x5
#define GPIO_GPIOB_DRV_R0_GPIOB3_6      0x6
#define GPIO_GPIOB_DRV_R0_GPIOB3_7      0x7
#define GPIO_GPIOB_DRV_R0_GPIOB4_0      0x0
#define GPIO_GPIOB_DRV_R0_GPIOB4_1      0x1
#define GPIO_GPIOB_DRV_R0_GPIOB4_2      0x2
#define GPIO_GPIOB_DRV_R0_GPIOB4_3      0x3
#define GPIO_GPIOB_DRV_R0_GPIOB4_4      0x4
#define GPIO_GPIOB_DRV_R0_GPIOB4_5      0x5
#define GPIO_GPIOB_DRV_R0_GPIOB4_6      0x6
#define GPIO_GPIOB_DRV_R0_GPIOB4_7      0x7
#define GPIO_GPIOB_DRV_R0_GPIOB5_0      0x0
#define GPIO_GPIOB_DRV_R0_GPIOB5_1      0x1
#define GPIO_GPIOB_DRV_R0_GPIOB5_2      0x2
#define GPIO_GPIOB_DRV_R0_GPIOB5_3      0x3
#define GPIO_GPIOB_DRV_R0_GPIOB5_4      0x4
#define GPIO_GPIOB_DRV_R0_GPIOB5_5      0x5
#define GPIO_GPIOB_DRV_R0_GPIOB5_6      0x6
#define GPIO_GPIOB_DRV_R0_GPIOB5_7      0x7
#define GPIO_GPIOB_DRV_R0_GPIOB6_0      0x0
#define GPIO_GPIOB_DRV_R0_GPIOB6_1      0x1
#define GPIO_GPIOB_DRV_R0_GPIOB6_2      0x2
#define GPIO_GPIOB_DRV_R0_GPIOB6_3      0x3
#define GPIO_GPIOB_DRV_R0_GPIOB6_4      0x4
#define GPIO_GPIOB_DRV_R0_GPIOB6_5      0x5
#define GPIO_GPIOB_DRV_R0_GPIOB6_6      0x6
#define GPIO_GPIOB_DRV_R0_GPIOB6_7      0x7
#define GPIO_GPIOB_DRV_R0_GPIOB7_0      0x0
#define GPIO_GPIOB_DRV_R0_GPIOB7_1      0x1
#define GPIO_GPIOB_DRV_R0_GPIOB7_2      0x2
#define GPIO_GPIOB_DRV_R0_GPIOB7_3      0x3
#define GPIO_GPIOB_DRV_R0_GPIOB7_4      0x4
#define GPIO_GPIOB_DRV_R0_GPIOB7_5      0x5
#define GPIO_GPIOB_DRV_R0_GPIOB7_6      0x6
#define GPIO_GPIOB_DRV_R0_GPIOB7_7      0x7

/* GPIOB Driver Configuration Register1 */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:8;           /* */
	u32 gpiob10:3;         /* */
	u32 rsvd1:1;           /* */
	u32 gpiob11:3;         /* */
	u32 rsvd2:1;           /* */
	u32 gpiob12:3;         /* */
	u32 rsvd3:1;           /* */
	u32 gpiob13:3;         /* */
	u32 rsvd4:1;           /* */
	u32 gpiob14:3;         /* */
	u32 rsvd5:1;           /* */
	u32 gpiob15:3;         /* */
	u32 rsvd6:1;           /* */
	} bits;
} reg_gpio_gpiob_drv_r1_t;

#define GPIO_GPIOB_DRV_R1_GPIOB10_0     0x0
#define GPIO_GPIOB_DRV_R1_GPIOB10_1     0x1
#define GPIO_GPIOB_DRV_R1_GPIOB10_2     0x2
#define GPIO_GPIOB_DRV_R1_GPIOB10_3     0x3
#define GPIO_GPIOB_DRV_R1_GPIOB10_4     0x4
#define GPIO_GPIOB_DRV_R1_GPIOB10_5     0x5
#define GPIO_GPIOB_DRV_R1_GPIOB10_6     0x6
#define GPIO_GPIOB_DRV_R1_GPIOB10_7     0x7
#define GPIO_GPIOB_DRV_R1_GPIOB11_0     0x0
#define GPIO_GPIOB_DRV_R1_GPIOB11_1     0x1
#define GPIO_GPIOB_DRV_R1_GPIOB11_2     0x2
#define GPIO_GPIOB_DRV_R1_GPIOB11_3     0x3
#define GPIO_GPIOB_DRV_R1_GPIOB11_4     0x4
#define GPIO_GPIOB_DRV_R1_GPIOB11_5     0x5
#define GPIO_GPIOB_DRV_R1_GPIOB11_6     0x6
#define GPIO_GPIOB_DRV_R1_GPIOB11_7     0x7
#define GPIO_GPIOB_DRV_R1_GPIOB12_0     0x0
#define GPIO_GPIOB_DRV_R1_GPIOB12_1     0x1
#define GPIO_GPIOB_DRV_R1_GPIOB12_2     0x2
#define GPIO_GPIOB_DRV_R1_GPIOB12_3     0x3
#define GPIO_GPIOB_DRV_R1_GPIOB12_4     0x4
#define GPIO_GPIOB_DRV_R1_GPIOB12_5     0x5
#define GPIO_GPIOB_DRV_R1_GPIOB12_6     0x6
#define GPIO_GPIOB_DRV_R1_GPIOB12_7     0x7
#define GPIO_GPIOB_DRV_R1_GPIOB13_0     0x0
#define GPIO_GPIOB_DRV_R1_GPIOB13_1     0x1
#define GPIO_GPIOB_DRV_R1_GPIOB13_2     0x2
#define GPIO_GPIOB_DRV_R1_GPIOB13_3     0x3
#define GPIO_GPIOB_DRV_R1_GPIOB13_4     0x4
#define GPIO_GPIOB_DRV_R1_GPIOB13_5     0x5
#define GPIO_GPIOB_DRV_R1_GPIOB13_6     0x6
#define GPIO_GPIOB_DRV_R1_GPIOB13_7     0x7
#define GPIO_GPIOB_DRV_R1_GPIOB14_0     0x0
#define GPIO_GPIOB_DRV_R1_GPIOB14_1     0x1
#define GPIO_GPIOB_DRV_R1_GPIOB14_2     0x2
#define GPIO_GPIOB_DRV_R1_GPIOB14_3     0x3
#define GPIO_GPIOB_DRV_R1_GPIOB14_4     0x4
#define GPIO_GPIOB_DRV_R1_GPIOB14_5     0x5
#define GPIO_GPIOB_DRV_R1_GPIOB14_6     0x6
#define GPIO_GPIOB_DRV_R1_GPIOB14_7     0x7
#define GPIO_GPIOB_DRV_R1_GPIOB15_0     0x0
#define GPIO_GPIOB_DRV_R1_GPIOB15_1     0x1
#define GPIO_GPIOB_DRV_R1_GPIOB15_2     0x2
#define GPIO_GPIOB_DRV_R1_GPIOB15_3     0x3
#define GPIO_GPIOB_DRV_R1_GPIOB15_4     0x4
#define GPIO_GPIOB_DRV_R1_GPIOB15_5     0x5
#define GPIO_GPIOB_DRV_R1_GPIOB15_6     0x6
#define GPIO_GPIOB_DRV_R1_GPIOB15_7     0x7

/* GPIOB Driver Configuration Register2 */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:8;           /* */
	u32 gpiob18:3;         /* */
	u32 rsvd1:1;           /* */
	u32 gpiob19:3;         /* */
	u32 rsvd2:1;           /* */
	u32 gpiob20:3;         /* */
	u32 rsvd3:1;           /* */
	u32 gpiob21:3;         /* */
	u32 rsvd4:1;           /* */
	u32 gpiob22:3;         /* */
	u32 rsvd5:1;           /* */
	u32 gpiob23:3;         /* */
	u32 rsvd6:1;           /* */
	} bits;
} reg_gpio_gpiob_drv_r2_t;

#define GPIO_GPIOB_DRV_R2_GPIOB18_0     0x0
#define GPIO_GPIOB_DRV_R2_GPIOB18_1     0x1
#define GPIO_GPIOB_DRV_R2_GPIOB18_2     0x2
#define GPIO_GPIOB_DRV_R2_GPIOB18_3     0x3
#define GPIO_GPIOB_DRV_R2_GPIOB18_4     0x4
#define GPIO_GPIOB_DRV_R2_GPIOB18_5     0x5
#define GPIO_GPIOB_DRV_R2_GPIOB18_6     0x6
#define GPIO_GPIOB_DRV_R2_GPIOB18_7     0x7
#define GPIO_GPIOB_DRV_R2_GPIOB19_0     0x0
#define GPIO_GPIOB_DRV_R2_GPIOB19_1     0x1
#define GPIO_GPIOB_DRV_R2_GPIOB19_2     0x2
#define GPIO_GPIOB_DRV_R2_GPIOB19_3     0x3
#define GPIO_GPIOB_DRV_R2_GPIOB19_4     0x4
#define GPIO_GPIOB_DRV_R2_GPIOB19_5     0x5
#define GPIO_GPIOB_DRV_R2_GPIOB19_6     0x6
#define GPIO_GPIOB_DRV_R2_GPIOB19_7     0x7
#define GPIO_GPIOB_DRV_R2_GPIOB20_0     0x0
#define GPIO_GPIOB_DRV_R2_GPIOB20_1     0x1
#define GPIO_GPIOB_DRV_R2_GPIOB20_2     0x2
#define GPIO_GPIOB_DRV_R2_GPIOB20_3     0x3
#define GPIO_GPIOB_DRV_R2_GPIOB20_4     0x4
#define GPIO_GPIOB_DRV_R2_GPIOB20_5     0x5
#define GPIO_GPIOB_DRV_R2_GPIOB20_6     0x6
#define GPIO_GPIOB_DRV_R2_GPIOB20_7     0x7
#define GPIO_GPIOB_DRV_R2_GPIOB21_0     0x0
#define GPIO_GPIOB_DRV_R2_GPIOB21_1     0x1
#define GPIO_GPIOB_DRV_R2_GPIOB21_2     0x2
#define GPIO_GPIOB_DRV_R2_GPIOB21_3     0x3
#define GPIO_GPIOB_DRV_R2_GPIOB21_4     0x4
#define GPIO_GPIOB_DRV_R2_GPIOB21_5     0x5
#define GPIO_GPIOB_DRV_R2_GPIOB21_6     0x6
#define GPIO_GPIOB_DRV_R2_GPIOB21_7     0x7
#define GPIO_GPIOB_DRV_R2_GPIOB22_0     0x0
#define GPIO_GPIOB_DRV_R2_GPIOB22_1     0x1
#define GPIO_GPIOB_DRV_R2_GPIOB22_2     0x2
#define GPIO_GPIOB_DRV_R2_GPIOB22_3     0x3
#define GPIO_GPIOB_DRV_R2_GPIOB22_4     0x4
#define GPIO_GPIOB_DRV_R2_GPIOB22_5     0x5
#define GPIO_GPIOB_DRV_R2_GPIOB22_6     0x6
#define GPIO_GPIOB_DRV_R2_GPIOB22_7     0x7
#define GPIO_GPIOB_DRV_R2_GPIOB23_0     0x0
#define GPIO_GPIOB_DRV_R2_GPIOB23_1     0x1
#define GPIO_GPIOB_DRV_R2_GPIOB23_2     0x2
#define GPIO_GPIOB_DRV_R2_GPIOB23_3     0x3
#define GPIO_GPIOB_DRV_R2_GPIOB23_4     0x4
#define GPIO_GPIOB_DRV_R2_GPIOB23_5     0x5
#define GPIO_GPIOB_DRV_R2_GPIOB23_6     0x6
#define GPIO_GPIOB_DRV_R2_GPIOB23_7     0x7

/* GPIOB Driver Configuration Register3 */
typedef union {
	u32 val;
	struct {
	u32 gpiob24:3;         /* */
	u32 rsvd0:1;           /* */
	u32 gpiob25:3;         /* */
	u32 rsvd1:1;           /* */
	u32 gpiob26:3;         /* */
	u32 rsvd2:1;           /* */
	u32 gpiob27:3;         /* */
	u32 rsvd3:17;          /* */
	} bits;
} reg_gpio_gpiob_drv_r3_t;

#define GPIO_GPIOB_DRV_R3_GPIOB24_0     0x0
#define GPIO_GPIOB_DRV_R3_GPIOB24_1     0x1
#define GPIO_GPIOB_DRV_R3_GPIOB24_2     0x2
#define GPIO_GPIOB_DRV_R3_GPIOB24_3     0x3
#define GPIO_GPIOB_DRV_R3_GPIOB24_4     0x4
#define GPIO_GPIOB_DRV_R3_GPIOB24_5     0x5
#define GPIO_GPIOB_DRV_R3_GPIOB24_6     0x6
#define GPIO_GPIOB_DRV_R3_GPIOB24_7     0x7
#define GPIO_GPIOB_DRV_R3_GPIOB25_0     0x0
#define GPIO_GPIOB_DRV_R3_GPIOB25_1     0x1
#define GPIO_GPIOB_DRV_R3_GPIOB25_2     0x2
#define GPIO_GPIOB_DRV_R3_GPIOB25_3     0x3
#define GPIO_GPIOB_DRV_R3_GPIOB25_4     0x4
#define GPIO_GPIOB_DRV_R3_GPIOB25_5     0x5
#define GPIO_GPIOB_DRV_R3_GPIOB25_6     0x6
#define GPIO_GPIOB_DRV_R3_GPIOB25_7     0x7
#define GPIO_GPIOB_DRV_R3_GPIOB26_0     0x0
#define GPIO_GPIOB_DRV_R3_GPIOB26_1     0x1
#define GPIO_GPIOB_DRV_R3_GPIOB26_2     0x2
#define GPIO_GPIOB_DRV_R3_GPIOB26_3     0x3
#define GPIO_GPIOB_DRV_R3_GPIOB26_4     0x4
#define GPIO_GPIOB_DRV_R3_GPIOB26_5     0x5
#define GPIO_GPIOB_DRV_R3_GPIOB26_6     0x6
#define GPIO_GPIOB_DRV_R3_GPIOB26_7     0x7
#define GPIO_GPIOB_DRV_R3_GPIOB27_0     0x0
#define GPIO_GPIOB_DRV_R3_GPIOB27_1     0x1
#define GPIO_GPIOB_DRV_R3_GPIOB27_2     0x2
#define GPIO_GPIOB_DRV_R3_GPIOB27_3     0x3
#define GPIO_GPIOB_DRV_R3_GPIOB27_4     0x4
#define GPIO_GPIOB_DRV_R3_GPIOB27_5     0x5
#define GPIO_GPIOB_DRV_R3_GPIOB27_6     0x6
#define GPIO_GPIOB_DRV_R3_GPIOB27_7     0x7

/* GPIOB Data Register */
typedef union {
	u32 val;
	struct {
	u32 dat:28;            /* */
	u32 rsvd0:4;           /* */
	} bits;
} reg_gpio_gpiob_data_t;

/* GPIOB Pull Up/Down Resistors Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 pull_cfg:28;       /* */
	u32 rsvd0:4;           /* */
	} bits;
} reg_gpio_gpiob_pr_r_t;

#define GPIO_GPIOB_PR_R_PULL_CFG_0      0x0
#define GPIO_GPIOB_PR_R_PULL_CFG_1      0x1

/* GPIOC Function Configuration Register0 */
typedef union {
	u32 val;
	struct {
	u32 gpioc0:4;          /* */
	u32 gpioc1:4;          /* */
	u32 gpioc2:4;          /* */
	u32 gpioc3:4;          /* */
	u32 gpioc4:4;          /* */
	u32 gpioc5:4;          /* */
	u32 gpioc6:4;          /* */
	u32 gpioc7:4;          /* */
	} bits;
} reg_gpio_gpioc_func_r0_t;

#define GPIO_GPIOC_FUNC_R0_GPIOC0_0     0x0
#define GPIO_GPIOC_FUNC_R0_GPIOC0_1     0x1
#define GPIO_GPIOC_FUNC_R0_GPIOC0_2     0x2
#define GPIO_GPIOC_FUNC_R0_GPIOC0_3     0x3
#define GPIO_GPIOC_FUNC_R0_GPIOC0_4     0x4
#define GPIO_GPIOC_FUNC_R0_GPIOC0_5     0x5
#define GPIO_GPIOC_FUNC_R0_GPIOC0_6     0x6
#define GPIO_GPIOC_FUNC_R0_GPIOC1_0     0x0
#define GPIO_GPIOC_FUNC_R0_GPIOC1_1     0x1
#define GPIO_GPIOC_FUNC_R0_GPIOC1_2     0x2
#define GPIO_GPIOC_FUNC_R0_GPIOC1_3     0x3
#define GPIO_GPIOC_FUNC_R0_GPIOC1_4     0x4
#define GPIO_GPIOC_FUNC_R0_GPIOC1_5     0x5
#define GPIO_GPIOC_FUNC_R0_GPIOC1_6     0x6
#define GPIO_GPIOC_FUNC_R0_GPIOC2_0     0x0
#define GPIO_GPIOC_FUNC_R0_GPIOC2_1     0x1
#define GPIO_GPIOC_FUNC_R0_GPIOC2_2     0x2
#define GPIO_GPIOC_FUNC_R0_GPIOC2_3     0x3
#define GPIO_GPIOC_FUNC_R0_GPIOC2_4     0x4
#define GPIO_GPIOC_FUNC_R0_GPIOC2_5     0x5
#define GPIO_GPIOC_FUNC_R0_GPIOC3_0     0x0
#define GPIO_GPIOC_FUNC_R0_GPIOC3_1     0x1
#define GPIO_GPIOC_FUNC_R0_GPIOC3_2     0x2
#define GPIO_GPIOC_FUNC_R0_GPIOC3_3     0x3
#define GPIO_GPIOC_FUNC_R0_GPIOC3_4     0x4
#define GPIO_GPIOC_FUNC_R0_GPIOC3_5     0x5
#define GPIO_GPIOC_FUNC_R0_GPIOC4_0     0x0
#define GPIO_GPIOC_FUNC_R0_GPIOC4_1     0x1
#define GPIO_GPIOC_FUNC_R0_GPIOC4_2     0x2
#define GPIO_GPIOC_FUNC_R0_GPIOC4_3     0x3
#define GPIO_GPIOC_FUNC_R0_GPIOC4_4     0x4
#define GPIO_GPIOC_FUNC_R0_GPIOC4_5     0x5
#define GPIO_GPIOC_FUNC_R0_GPIOC4_6     0x6
#define GPIO_GPIOC_FUNC_R0_GPIOC5_0     0x0
#define GPIO_GPIOC_FUNC_R0_GPIOC5_1     0x1
#define GPIO_GPIOC_FUNC_R0_GPIOC5_2     0x2
#define GPIO_GPIOC_FUNC_R0_GPIOC5_3     0x3
#define GPIO_GPIOC_FUNC_R0_GPIOC5_4     0x4
#define GPIO_GPIOC_FUNC_R0_GPIOC5_5     0x5
#define GPIO_GPIOC_FUNC_R0_GPIOC5_6     0x6
#define GPIO_GPIOC_FUNC_R0_GPIOC6_0     0x0
#define GPIO_GPIOC_FUNC_R0_GPIOC6_1     0x1
#define GPIO_GPIOC_FUNC_R0_GPIOC6_2     0x2
#define GPIO_GPIOC_FUNC_R0_GPIOC6_3     0x3
#define GPIO_GPIOC_FUNC_R0_GPIOC6_4     0x4
#define GPIO_GPIOC_FUNC_R0_GPIOC6_5     0x5
#define GPIO_GPIOC_FUNC_R0_GPIOC6_6     0x6
#define GPIO_GPIOC_FUNC_R0_GPIOC7_0     0x0
#define GPIO_GPIOC_FUNC_R0_GPIOC7_1     0x1
#define GPIO_GPIOC_FUNC_R0_GPIOC7_2     0x2
#define GPIO_GPIOC_FUNC_R0_GPIOC7_3     0x3
#define GPIO_GPIOC_FUNC_R0_GPIOC7_4     0x4
#define GPIO_GPIOC_FUNC_R0_GPIOC7_5     0x5
#define GPIO_GPIOC_FUNC_R0_GPIOC7_6     0x6

/* GPIOC Function Configuration Register1 */
typedef union {
	u32 val;
	struct {
	u32 gpioc8:4;          /* */
	u32 gpioc9:4;          /* */
	u32 gpioc10:4;         /* */
	u32 gpioc11:4;         /* */
	u32 gpioc12:4;         /* */
	u32 gpioc13:4;         /* */
	u32 gpioc14:4;         /* */
	u32 gpioc15:4;         /* */
	} bits;
} reg_gpio_gpioc_func_r1_t;

#define GPIO_GPIOC_FUNC_R1_GPIOC8_0     0x0
#define GPIO_GPIOC_FUNC_R1_GPIOC8_1     0x1
#define GPIO_GPIOC_FUNC_R1_GPIOC8_2     0x2
#define GPIO_GPIOC_FUNC_R1_GPIOC8_3     0x3
#define GPIO_GPIOC_FUNC_R1_GPIOC8_4     0x4
#define GPIO_GPIOC_FUNC_R1_GPIOC8_5     0x5
#define GPIO_GPIOC_FUNC_R1_GPIOC8_6     0x6
#define GPIO_GPIOC_FUNC_R1_GPIOC9_0     0x0
#define GPIO_GPIOC_FUNC_R1_GPIOC9_1     0x1
#define GPIO_GPIOC_FUNC_R1_GPIOC9_2     0x2
#define GPIO_GPIOC_FUNC_R1_GPIOC9_3     0x3
#define GPIO_GPIOC_FUNC_R1_GPIOC9_4     0x4
#define GPIO_GPIOC_FUNC_R1_GPIOC9_5     0x5
#define GPIO_GPIOC_FUNC_R1_GPIOC9_6     0x6
#define GPIO_GPIOC_FUNC_R1_GPIOC10_0    0x0
#define GPIO_GPIOC_FUNC_R1_GPIOC10_1    0x1
#define GPIO_GPIOC_FUNC_R1_GPIOC10_2    0x2
#define GPIO_GPIOC_FUNC_R1_GPIOC10_3    0x3
#define GPIO_GPIOC_FUNC_R1_GPIOC10_4    0x4
#define GPIO_GPIOC_FUNC_R1_GPIOC10_5    0x5
#define GPIO_GPIOC_FUNC_R1_GPIOC10_6    0x6
#define GPIO_GPIOC_FUNC_R1_GPIOC11_0    0x0
#define GPIO_GPIOC_FUNC_R1_GPIOC11_1    0x1
#define GPIO_GPIOC_FUNC_R1_GPIOC11_2    0x2
#define GPIO_GPIOC_FUNC_R1_GPIOC11_3    0x3
#define GPIO_GPIOC_FUNC_R1_GPIOC11_4    0x4
#define GPIO_GPIOC_FUNC_R1_GPIOC11_5    0x5
#define GPIO_GPIOC_FUNC_R1_GPIOC11_6    0x6
#define GPIO_GPIOC_FUNC_R1_GPIOC12_0    0x0
#define GPIO_GPIOC_FUNC_R1_GPIOC12_1    0x1
#define GPIO_GPIOC_FUNC_R1_GPIOC12_2    0x2
#define GPIO_GPIOC_FUNC_R1_GPIOC12_3    0x3
#define GPIO_GPIOC_FUNC_R1_GPIOC12_4    0x4
#define GPIO_GPIOC_FUNC_R1_GPIOC12_5    0x5
#define GPIO_GPIOC_FUNC_R1_GPIOC12_6    0x6
#define GPIO_GPIOC_FUNC_R1_GPIOC13_0    0x0
#define GPIO_GPIOC_FUNC_R1_GPIOC13_1    0x1
#define GPIO_GPIOC_FUNC_R1_GPIOC13_2    0x2
#define GPIO_GPIOC_FUNC_R1_GPIOC13_3    0x3
#define GPIO_GPIOC_FUNC_R1_GPIOC13_4    0x4
#define GPIO_GPIOC_FUNC_R1_GPIOC13_5    0x5
#define GPIO_GPIOC_FUNC_R1_GPIOC13_6    0x6
#define GPIO_GPIOC_FUNC_R1_GPIOC14_0    0x0
#define GPIO_GPIOC_FUNC_R1_GPIOC14_1    0x1
#define GPIO_GPIOC_FUNC_R1_GPIOC14_2    0x2
#define GPIO_GPIOC_FUNC_R1_GPIOC14_3    0x3
#define GPIO_GPIOC_FUNC_R1_GPIOC14_4    0x4
#define GPIO_GPIOC_FUNC_R1_GPIOC14_5    0x5
#define GPIO_GPIOC_FUNC_R1_GPIOC14_6    0x6
#define GPIO_GPIOC_FUNC_R1_GPIOC15_0    0x0
#define GPIO_GPIOC_FUNC_R1_GPIOC15_1    0x1
#define GPIO_GPIOC_FUNC_R1_GPIOC15_2    0x2
#define GPIO_GPIOC_FUNC_R1_GPIOC15_3    0x3
#define GPIO_GPIOC_FUNC_R1_GPIOC15_4    0x4
#define GPIO_GPIOC_FUNC_R1_GPIOC15_5    0x5
#define GPIO_GPIOC_FUNC_R1_GPIOC15_6    0x6

/* GPIOC Function Configuration Register2 */
typedef union {
	u32 val;
	struct {
	u32 gpioc16:4;         /* */
	u32 gpioc17:4;         /* */
	u32 gpioc18:4;         /* */
	u32 gpioc19:4;         /* */
	u32 gpioc20:4;         /* */
	u32 gpioc21:4;         /* */
	u32 gpioc22:4;         /* */
	u32 rsvd0:4;           /* */
	} bits;
} reg_gpio_gpioc_func_r2_t;

#define GPIO_GPIOC_FUNC_R2_GPIOC16_0    0x0
#define GPIO_GPIOC_FUNC_R2_GPIOC16_1    0x1
#define GPIO_GPIOC_FUNC_R2_GPIOC16_2    0x2
#define GPIO_GPIOC_FUNC_R2_GPIOC16_3    0x3
#define GPIO_GPIOC_FUNC_R2_GPIOC16_4    0x4
#define GPIO_GPIOC_FUNC_R2_GPIOC16_5    0x5
#define GPIO_GPIOC_FUNC_R2_GPIOC16_6    0x6
#define GPIO_GPIOC_FUNC_R2_GPIOC17_0    0x0
#define GPIO_GPIOC_FUNC_R2_GPIOC17_1    0x1
#define GPIO_GPIOC_FUNC_R2_GPIOC17_2    0x2
#define GPIO_GPIOC_FUNC_R2_GPIOC17_3    0x3
#define GPIO_GPIOC_FUNC_R2_GPIOC17_4    0x4
#define GPIO_GPIOC_FUNC_R2_GPIOC17_5    0x5
#define GPIO_GPIOC_FUNC_R2_GPIOC17_6    0x6
#define GPIO_GPIOC_FUNC_R2_GPIOC18_0    0x0
#define GPIO_GPIOC_FUNC_R2_GPIOC18_1    0x1
#define GPIO_GPIOC_FUNC_R2_GPIOC18_2    0x2
#define GPIO_GPIOC_FUNC_R2_GPIOC18_3    0x3
#define GPIO_GPIOC_FUNC_R2_GPIOC18_4    0x4
#define GPIO_GPIOC_FUNC_R2_GPIOC18_5    0x5
#define GPIO_GPIOC_FUNC_R2_GPIOC18_6    0x6
#define GPIO_GPIOC_FUNC_R2_GPIOC19_0    0x0
#define GPIO_GPIOC_FUNC_R2_GPIOC19_1    0x1
#define GPIO_GPIOC_FUNC_R2_GPIOC19_2    0x2
#define GPIO_GPIOC_FUNC_R2_GPIOC19_3    0x3
#define GPIO_GPIOC_FUNC_R2_GPIOC19_4    0x4
#define GPIO_GPIOC_FUNC_R2_GPIOC19_5    0x5
#define GPIO_GPIOC_FUNC_R2_GPIOC19_6    0x6
#define GPIO_GPIOC_FUNC_R2_GPIOC20_0    0x0
#define GPIO_GPIOC_FUNC_R2_GPIOC20_1    0x1
#define GPIO_GPIOC_FUNC_R2_GPIOC20_2    0x2
#define GPIO_GPIOC_FUNC_R2_GPIOC20_3    0x3
#define GPIO_GPIOC_FUNC_R2_GPIOC20_4    0x4
#define GPIO_GPIOC_FUNC_R2_GPIOC20_5    0x5
#define GPIO_GPIOC_FUNC_R2_GPIOC21_0    0x0
#define GPIO_GPIOC_FUNC_R2_GPIOC21_1    0x1
#define GPIO_GPIOC_FUNC_R2_GPIOC21_2    0x2
#define GPIO_GPIOC_FUNC_R2_GPIOC21_3    0x3
#define GPIO_GPIOC_FUNC_R2_GPIOC21_4    0x4
#define GPIO_GPIOC_FUNC_R2_GPIOC21_5    0x5
#define GPIO_GPIOC_FUNC_R2_GPIOC22_0    0x0
#define GPIO_GPIOC_FUNC_R2_GPIOC22_1    0x1
#define GPIO_GPIOC_FUNC_R2_GPIOC22_2    0x2
#define GPIO_GPIOC_FUNC_R2_GPIOC22_3    0x3
#define GPIO_GPIOC_FUNC_R2_GPIOC22_4    0x4
#define GPIO_GPIOC_FUNC_R2_GPIOC22_5    0x5

/* GPIOC Pull Up or Pull Down Configuration Register0 */
typedef union {
	u32 val;
	struct {
	u32 gpioc0:2;          /* */
	u32 rsvd0:2;           /* */
	u32 gpioc1:2;          /* */
	u32 rsvd1:2;           /* */
	u32 gpioc2:2;          /* */
	u32 rsvd2:2;           /* */
	u32 gpioc3:2;          /* */
	u32 rsvd3:2;           /* */
	u32 gpioc4:2;          /* */
	u32 rsvd4:2;           /* */
	u32 gpioc5:2;          /* */
	u32 rsvd5:2;           /* */
	u32 gpioc6:2;          /* */
	u32 rsvd6:2;           /* */
	u32 gpioc7:2;          /* */
	u32 rsvd7:2;           /* */
	} bits;
} reg_gpio_gpioc_pud_r0_t;

#define GPIO_GPIOC_PUD_R0_GPIOC0_0      0x0
#define GPIO_GPIOC_PUD_R0_GPIOC0_1      0x1
#define GPIO_GPIOC_PUD_R0_GPIOC0_2      0x2
#define GPIO_GPIOC_PUD_R0_GPIOC0_3      0x3
#define GPIO_GPIOC_PUD_R0_GPIOC1_0      0x0
#define GPIO_GPIOC_PUD_R0_GPIOC1_1      0x1
#define GPIO_GPIOC_PUD_R0_GPIOC1_2      0x2
#define GPIO_GPIOC_PUD_R0_GPIOC1_3      0x3
#define GPIO_GPIOC_PUD_R0_GPIOC2_0      0x0
#define GPIO_GPIOC_PUD_R0_GPIOC2_1      0x1
#define GPIO_GPIOC_PUD_R0_GPIOC2_2      0x2
#define GPIO_GPIOC_PUD_R0_GPIOC2_3      0x3
#define GPIO_GPIOC_PUD_R0_GPIOC3_0      0x0
#define GPIO_GPIOC_PUD_R0_GPIOC3_1      0x1
#define GPIO_GPIOC_PUD_R0_GPIOC3_2      0x2
#define GPIO_GPIOC_PUD_R0_GPIOC3_3      0x3
#define GPIO_GPIOC_PUD_R0_GPIOC4_0      0x0
#define GPIO_GPIOC_PUD_R0_GPIOC4_1      0x1
#define GPIO_GPIOC_PUD_R0_GPIOC4_2      0x2
#define GPIO_GPIOC_PUD_R0_GPIOC4_3      0x3
#define GPIO_GPIOC_PUD_R0_GPIOC5_0      0x0
#define GPIO_GPIOC_PUD_R0_GPIOC5_1      0x1
#define GPIO_GPIOC_PUD_R0_GPIOC5_2      0x2
#define GPIO_GPIOC_PUD_R0_GPIOC5_3      0x3
#define GPIO_GPIOC_PUD_R0_GPIOC6_0      0x0
#define GPIO_GPIOC_PUD_R0_GPIOC6_1      0x1
#define GPIO_GPIOC_PUD_R0_GPIOC6_2      0x2
#define GPIO_GPIOC_PUD_R0_GPIOC6_3      0x3
#define GPIO_GPIOC_PUD_R0_GPIOC7_0      0x0
#define GPIO_GPIOC_PUD_R0_GPIOC7_1      0x1
#define GPIO_GPIOC_PUD_R0_GPIOC7_2      0x2
#define GPIO_GPIOC_PUD_R0_GPIOC7_3      0x3

/* GPIOC Pull Up or Pull Down Configuration Register1 */
typedef union {
	u32 val;
	struct {
	u32 gpioc8:2;          /* */
	u32 rsvd0:2;           /* */
	u32 gpioc9:2;          /* */
	u32 rsvd1:2;           /* */
	u32 gpioc10:2;         /* */
	u32 rsvd2:2;           /* */
	u32 gpioc11:2;         /* */
	u32 rsvd3:2;           /* */
	u32 gpioc12:2;         /* */
	u32 rsvd4:2;           /* */
	u32 gpioc13:2;         /* */
	u32 rsvd5:2;           /* */
	u32 gpioc14:2;         /* */
	u32 rsvd6:2;           /* */
	u32 gpioc15:2;         /* */
	u32 rsvd7:2;           /* */
	} bits;
} reg_gpio_gpioc_pud_r1_t;

#define GPIO_GPIOC_PUD_R1_GPIOC8_0      0x0
#define GPIO_GPIOC_PUD_R1_GPIOC8_1      0x1
#define GPIO_GPIOC_PUD_R1_GPIOC8_2      0x2
#define GPIO_GPIOC_PUD_R1_GPIOC8_3      0x3
#define GPIO_GPIOC_PUD_R1_GPIOC9_0      0x0
#define GPIO_GPIOC_PUD_R1_GPIOC9_1      0x1
#define GPIO_GPIOC_PUD_R1_GPIOC9_2      0x2
#define GPIO_GPIOC_PUD_R1_GPIOC9_3      0x3
#define GPIO_GPIOC_PUD_R1_GPIOC10_0     0x0
#define GPIO_GPIOC_PUD_R1_GPIOC10_1     0x1
#define GPIO_GPIOC_PUD_R1_GPIOC10_2     0x2
#define GPIO_GPIOC_PUD_R1_GPIOC10_3     0x3
#define GPIO_GPIOC_PUD_R1_GPIOC11_0     0x0
#define GPIO_GPIOC_PUD_R1_GPIOC11_1     0x1
#define GPIO_GPIOC_PUD_R1_GPIOC11_2     0x2
#define GPIO_GPIOC_PUD_R1_GPIOC11_3     0x3
#define GPIO_GPIOC_PUD_R1_GPIOC12_0     0x0
#define GPIO_GPIOC_PUD_R1_GPIOC12_1     0x1
#define GPIO_GPIOC_PUD_R1_GPIOC12_2     0x2
#define GPIO_GPIOC_PUD_R1_GPIOC12_3     0x3
#define GPIO_GPIOC_PUD_R1_GPIOC13_0     0x0
#define GPIO_GPIOC_PUD_R1_GPIOC13_1     0x1
#define GPIO_GPIOC_PUD_R1_GPIOC13_2     0x2
#define GPIO_GPIOC_PUD_R1_GPIOC13_3     0x3
#define GPIO_GPIOC_PUD_R1_GPIOC14_0     0x0
#define GPIO_GPIOC_PUD_R1_GPIOC14_1     0x1
#define GPIO_GPIOC_PUD_R1_GPIOC14_2     0x2
#define GPIO_GPIOC_PUD_R1_GPIOC14_3     0x3
#define GPIO_GPIOC_PUD_R1_GPIOC15_0     0x0
#define GPIO_GPIOC_PUD_R1_GPIOC15_1     0x1
#define GPIO_GPIOC_PUD_R1_GPIOC15_2     0x2
#define GPIO_GPIOC_PUD_R1_GPIOC15_3     0x3

/* GPIOC Pull Up or Pull Down Configuration Register2 */
typedef union {
	u32 val;
	struct {
	u32 gpioc16:2;         /* */
	u32 rsvd0:2;           /* */
	u32 gpioc17:2;         /* */
	u32 rsvd1:2;           /* */
	u32 gpioc18:2;         /* */
	u32 rsvd2:2;           /* */
	u32 gpioc19:2;         /* */
	u32 rsvd3:2;           /* */
	u32 gpioc20:2;         /* */
	u32 rsvd4:2;           /* */
	u32 gpioc21:2;         /* */
	u32 rsvd5:2;           /* */
	u32 gpioc22:2;         /* */
	u32 rsvd6:6;           /* */
	} bits;
} reg_gpio_gpioc_pud_r2_t;

#define GPIO_GPIOC_PUD_R2_GPIOC16_0     0x0
#define GPIO_GPIOC_PUD_R2_GPIOC16_1     0x1
#define GPIO_GPIOC_PUD_R2_GPIOC16_2     0x2
#define GPIO_GPIOC_PUD_R2_GPIOC16_3     0x3
#define GPIO_GPIOC_PUD_R2_GPIOC17_0     0x0
#define GPIO_GPIOC_PUD_R2_GPIOC17_1     0x1
#define GPIO_GPIOC_PUD_R2_GPIOC17_2     0x2
#define GPIO_GPIOC_PUD_R2_GPIOC17_3     0x3
#define GPIO_GPIOC_PUD_R2_GPIOC18_0     0x0
#define GPIO_GPIOC_PUD_R2_GPIOC18_1     0x1
#define GPIO_GPIOC_PUD_R2_GPIOC18_2     0x2
#define GPIO_GPIOC_PUD_R2_GPIOC18_3     0x3
#define GPIO_GPIOC_PUD_R2_GPIOC19_0     0x0
#define GPIO_GPIOC_PUD_R2_GPIOC19_1     0x1
#define GPIO_GPIOC_PUD_R2_GPIOC19_2     0x2
#define GPIO_GPIOC_PUD_R2_GPIOC19_3     0x3
#define GPIO_GPIOC_PUD_R2_GPIOC20_0     0x0
#define GPIO_GPIOC_PUD_R2_GPIOC20_1     0x1
#define GPIO_GPIOC_PUD_R2_GPIOC20_2     0x2
#define GPIO_GPIOC_PUD_R2_GPIOC20_3     0x3
#define GPIO_GPIOC_PUD_R2_GPIOC21_0     0x0
#define GPIO_GPIOC_PUD_R2_GPIOC21_1     0x1
#define GPIO_GPIOC_PUD_R2_GPIOC21_2     0x2
#define GPIO_GPIOC_PUD_R2_GPIOC21_3     0x3
#define GPIO_GPIOC_PUD_R2_GPIOC22_0     0x0
#define GPIO_GPIOC_PUD_R2_GPIOC22_1     0x1
#define GPIO_GPIOC_PUD_R2_GPIOC22_2     0x2
#define GPIO_GPIOC_PUD_R2_GPIOC22_3     0x3

/* GPIOC Driver Configuration Register0 */
typedef union {
	u32 val;
	struct {
	u32 gpioc0:3;          /* */
	u32 rsvd0:1;           /* */
	u32 gpioc1:3;          /* */
	u32 rsvd1:1;           /* */
	u32 gpioc2:3;          /* */
	u32 rsvd2:1;           /* */
	u32 gpioc3:3;          /* */
	u32 rsvd3:1;           /* */
	u32 gpioc4:3;          /* */
	u32 rsvd4:1;           /* */
	u32 gpioc5:3;          /* */
	u32 rsvd5:1;           /* */
	u32 gpioc6:3;          /* */
	u32 rsvd6:1;           /* */
	u32 gpioc7:3;          /* */
	u32 rsvd7:1;           /* */
	} bits;
} reg_gpio_gpioc_drv_r0_t;

#define GPIO_GPIOC_DRV_R0_GPIOC0_0      0x0
#define GPIO_GPIOC_DRV_R0_GPIOC0_1      0x1
#define GPIO_GPIOC_DRV_R0_GPIOC0_2      0x2
#define GPIO_GPIOC_DRV_R0_GPIOC0_3      0x3
#define GPIO_GPIOC_DRV_R0_GPIOC0_4      0x4
#define GPIO_GPIOC_DRV_R0_GPIOC0_5      0x5
#define GPIO_GPIOC_DRV_R0_GPIOC0_6      0x6
#define GPIO_GPIOC_DRV_R0_GPIOC0_7      0x7
#define GPIO_GPIOC_DRV_R0_GPIOC1_0      0x0
#define GPIO_GPIOC_DRV_R0_GPIOC1_1      0x1
#define GPIO_GPIOC_DRV_R0_GPIOC1_2      0x2
#define GPIO_GPIOC_DRV_R0_GPIOC1_3      0x3
#define GPIO_GPIOC_DRV_R0_GPIOC1_4      0x4
#define GPIO_GPIOC_DRV_R0_GPIOC1_5      0x5
#define GPIO_GPIOC_DRV_R0_GPIOC1_6      0x6
#define GPIO_GPIOC_DRV_R0_GPIOC1_7      0x7
#define GPIO_GPIOC_DRV_R0_GPIOC2_0      0x0
#define GPIO_GPIOC_DRV_R0_GPIOC2_1      0x1
#define GPIO_GPIOC_DRV_R0_GPIOC2_2      0x2
#define GPIO_GPIOC_DRV_R0_GPIOC2_3      0x3
#define GPIO_GPIOC_DRV_R0_GPIOC2_4      0x4
#define GPIO_GPIOC_DRV_R0_GPIOC2_5      0x5
#define GPIO_GPIOC_DRV_R0_GPIOC2_6      0x6
#define GPIO_GPIOC_DRV_R0_GPIOC2_7      0x7
#define GPIO_GPIOC_DRV_R0_GPIOC3_0      0x0
#define GPIO_GPIOC_DRV_R0_GPIOC3_1      0x1
#define GPIO_GPIOC_DRV_R0_GPIOC3_2      0x2
#define GPIO_GPIOC_DRV_R0_GPIOC3_3      0x3
#define GPIO_GPIOC_DRV_R0_GPIOC3_4      0x4
#define GPIO_GPIOC_DRV_R0_GPIOC3_5      0x5
#define GPIO_GPIOC_DRV_R0_GPIOC3_6      0x6
#define GPIO_GPIOC_DRV_R0_GPIOC3_7      0x7
#define GPIO_GPIOC_DRV_R0_GPIOC4_0      0x0
#define GPIO_GPIOC_DRV_R0_GPIOC4_1      0x1
#define GPIO_GPIOC_DRV_R0_GPIOC4_2      0x2
#define GPIO_GPIOC_DRV_R0_GPIOC4_3      0x3
#define GPIO_GPIOC_DRV_R0_GPIOC4_4      0x4
#define GPIO_GPIOC_DRV_R0_GPIOC4_5      0x5
#define GPIO_GPIOC_DRV_R0_GPIOC4_6      0x6
#define GPIO_GPIOC_DRV_R0_GPIOC4_7      0x7
#define GPIO_GPIOC_DRV_R0_GPIOC5_0      0x0
#define GPIO_GPIOC_DRV_R0_GPIOC5_1      0x1
#define GPIO_GPIOC_DRV_R0_GPIOC5_2      0x2
#define GPIO_GPIOC_DRV_R0_GPIOC5_3      0x3
#define GPIO_GPIOC_DRV_R0_GPIOC5_4      0x4
#define GPIO_GPIOC_DRV_R0_GPIOC5_5      0x5
#define GPIO_GPIOC_DRV_R0_GPIOC5_6      0x6
#define GPIO_GPIOC_DRV_R0_GPIOC5_7      0x7
#define GPIO_GPIOC_DRV_R0_GPIOC6_0      0x0
#define GPIO_GPIOC_DRV_R0_GPIOC6_1      0x1
#define GPIO_GPIOC_DRV_R0_GPIOC6_2      0x2
#define GPIO_GPIOC_DRV_R0_GPIOC6_3      0x3
#define GPIO_GPIOC_DRV_R0_GPIOC6_4      0x4
#define GPIO_GPIOC_DRV_R0_GPIOC6_5      0x5
#define GPIO_GPIOC_DRV_R0_GPIOC6_6      0x6
#define GPIO_GPIOC_DRV_R0_GPIOC6_7      0x7
#define GPIO_GPIOC_DRV_R0_GPIOC7_0      0x0
#define GPIO_GPIOC_DRV_R0_GPIOC7_1      0x1
#define GPIO_GPIOC_DRV_R0_GPIOC7_2      0x2
#define GPIO_GPIOC_DRV_R0_GPIOC7_3      0x3
#define GPIO_GPIOC_DRV_R0_GPIOC7_4      0x4
#define GPIO_GPIOC_DRV_R0_GPIOC7_5      0x5
#define GPIO_GPIOC_DRV_R0_GPIOC7_6      0x6
#define GPIO_GPIOC_DRV_R0_GPIOC7_7      0x7

/* GPIOC Driver Configuration Register1 */
typedef union {
	u32 val;
	struct {
	u32 gpioc8:3;          /* */
	u32 rsvd0:1;           /* */
	u32 gpioc9:3;          /* */
	u32 rsvd1:1;           /* */
	u32 gpioc10:3;         /* */
	u32 rsvd2:1;           /* */
	u32 gpioc11:3;         /* */
	u32 rsvd3:1;           /* */
	u32 gpioc12:3;         /* */
	u32 rsvd4:1;           /* */
	u32 gpioc13:3;         /* */
	u32 rsvd5:1;           /* */
	u32 gpioc14:3;         /* */
	u32 rsvd6:1;           /* */
	u32 gpioc15:3;         /* */
	u32 rsvd7:1;           /* */
	} bits;
} reg_gpio_gpioc_drv_r1_t;

#define GPIO_GPIOC_DRV_R1_GPIOC8_0      0x0
#define GPIO_GPIOC_DRV_R1_GPIOC8_1      0x1
#define GPIO_GPIOC_DRV_R1_GPIOC8_2      0x2
#define GPIO_GPIOC_DRV_R1_GPIOC8_3      0x3
#define GPIO_GPIOC_DRV_R1_GPIOC8_4      0x4
#define GPIO_GPIOC_DRV_R1_GPIOC8_5      0x5
#define GPIO_GPIOC_DRV_R1_GPIOC8_6      0x6
#define GPIO_GPIOC_DRV_R1_GPIOC8_7      0x7
#define GPIO_GPIOC_DRV_R1_GPIOC9_0      0x0
#define GPIO_GPIOC_DRV_R1_GPIOC9_1      0x1
#define GPIO_GPIOC_DRV_R1_GPIOC9_2      0x2
#define GPIO_GPIOC_DRV_R1_GPIOC9_3      0x3
#define GPIO_GPIOC_DRV_R1_GPIOC9_4      0x4
#define GPIO_GPIOC_DRV_R1_GPIOC9_5      0x5
#define GPIO_GPIOC_DRV_R1_GPIOC9_6      0x6
#define GPIO_GPIOC_DRV_R1_GPIOC9_7      0x7
#define GPIO_GPIOC_DRV_R1_GPIOC10_0     0x0
#define GPIO_GPIOC_DRV_R1_GPIOC10_1     0x1
#define GPIO_GPIOC_DRV_R1_GPIOC10_2     0x2
#define GPIO_GPIOC_DRV_R1_GPIOC10_3     0x3
#define GPIO_GPIOC_DRV_R1_GPIOC10_4     0x4
#define GPIO_GPIOC_DRV_R1_GPIOC10_5     0x5
#define GPIO_GPIOC_DRV_R1_GPIOC10_6     0x6
#define GPIO_GPIOC_DRV_R1_GPIOC10_7     0x7
#define GPIO_GPIOC_DRV_R1_GPIOC11_0     0x0
#define GPIO_GPIOC_DRV_R1_GPIOC11_1     0x1
#define GPIO_GPIOC_DRV_R1_GPIOC11_2     0x2
#define GPIO_GPIOC_DRV_R1_GPIOC11_3     0x3
#define GPIO_GPIOC_DRV_R1_GPIOC11_4     0x4
#define GPIO_GPIOC_DRV_R1_GPIOC11_5     0x5
#define GPIO_GPIOC_DRV_R1_GPIOC11_6     0x6
#define GPIO_GPIOC_DRV_R1_GPIOC11_7     0x7
#define GPIO_GPIOC_DRV_R1_GPIOC12_0     0x0
#define GPIO_GPIOC_DRV_R1_GPIOC12_1     0x1
#define GPIO_GPIOC_DRV_R1_GPIOC12_2     0x2
#define GPIO_GPIOC_DRV_R1_GPIOC12_3     0x3
#define GPIO_GPIOC_DRV_R1_GPIOC12_4     0x4
#define GPIO_GPIOC_DRV_R1_GPIOC12_5     0x5
#define GPIO_GPIOC_DRV_R1_GPIOC12_6     0x6
#define GPIO_GPIOC_DRV_R1_GPIOC12_7     0x7
#define GPIO_GPIOC_DRV_R1_GPIOC13_0     0x0
#define GPIO_GPIOC_DRV_R1_GPIOC13_1     0x1
#define GPIO_GPIOC_DRV_R1_GPIOC13_2     0x2
#define GPIO_GPIOC_DRV_R1_GPIOC13_3     0x3
#define GPIO_GPIOC_DRV_R1_GPIOC13_4     0x4
#define GPIO_GPIOC_DRV_R1_GPIOC13_5     0x5
#define GPIO_GPIOC_DRV_R1_GPIOC13_6     0x6
#define GPIO_GPIOC_DRV_R1_GPIOC13_7     0x7
#define GPIO_GPIOC_DRV_R1_GPIOC14_0     0x0
#define GPIO_GPIOC_DRV_R1_GPIOC14_1     0x1
#define GPIO_GPIOC_DRV_R1_GPIOC14_2     0x2
#define GPIO_GPIOC_DRV_R1_GPIOC14_3     0x3
#define GPIO_GPIOC_DRV_R1_GPIOC14_4     0x4
#define GPIO_GPIOC_DRV_R1_GPIOC14_5     0x5
#define GPIO_GPIOC_DRV_R1_GPIOC14_6     0x6
#define GPIO_GPIOC_DRV_R1_GPIOC14_7     0x7
#define GPIO_GPIOC_DRV_R1_GPIOC15_0     0x0
#define GPIO_GPIOC_DRV_R1_GPIOC15_1     0x1
#define GPIO_GPIOC_DRV_R1_GPIOC15_2     0x2
#define GPIO_GPIOC_DRV_R1_GPIOC15_3     0x3
#define GPIO_GPIOC_DRV_R1_GPIOC15_4     0x4
#define GPIO_GPIOC_DRV_R1_GPIOC15_5     0x5
#define GPIO_GPIOC_DRV_R1_GPIOC15_6     0x6
#define GPIO_GPIOC_DRV_R1_GPIOC15_7     0x7

/* GPIOC Driver Configuration Register2 */
typedef union {
	u32 val;
	struct {
	u32 gpioc16:3;         /* */
	u32 rsvd0:1;           /* */
	u32 gpioc17:3;         /* */
	u32 rsvd1:1;           /* */
	u32 gpioc18:3;         /* */
	u32 rsvd2:1;           /* */
	u32 gpioc19:3;         /* */
	u32 rsvd3:1;           /* */
	u32 gpioc20:3;         /* */
	u32 rsvd4:1;           /* */
	u32 gpioc21:3;         /* */
	u32 rsvd5:1;           /* */
	u32 gpioc22:3;         /* */
	u32 rsvd6:5;           /* */
	} bits;
} reg_gpio_gpioc_drv_r2_t;

#define GPIO_GPIOC_DRV_R2_GPIOC16_0     0x0
#define GPIO_GPIOC_DRV_R2_GPIOC16_1     0x1
#define GPIO_GPIOC_DRV_R2_GPIOC16_2     0x2
#define GPIO_GPIOC_DRV_R2_GPIOC16_3     0x3
#define GPIO_GPIOC_DRV_R2_GPIOC16_4     0x4
#define GPIO_GPIOC_DRV_R2_GPIOC16_5     0x5
#define GPIO_GPIOC_DRV_R2_GPIOC16_6     0x6
#define GPIO_GPIOC_DRV_R2_GPIOC16_7     0x7
#define GPIO_GPIOC_DRV_R2_GPIOC17_0     0x0
#define GPIO_GPIOC_DRV_R2_GPIOC17_1     0x1
#define GPIO_GPIOC_DRV_R2_GPIOC17_2     0x2
#define GPIO_GPIOC_DRV_R2_GPIOC17_3     0x3
#define GPIO_GPIOC_DRV_R2_GPIOC17_4     0x4
#define GPIO_GPIOC_DRV_R2_GPIOC17_5     0x5
#define GPIO_GPIOC_DRV_R2_GPIOC17_6     0x6
#define GPIO_GPIOC_DRV_R2_GPIOC17_7     0x7
#define GPIO_GPIOC_DRV_R2_GPIOC18_0     0x0
#define GPIO_GPIOC_DRV_R2_GPIOC18_1     0x1
#define GPIO_GPIOC_DRV_R2_GPIOC18_2     0x2
#define GPIO_GPIOC_DRV_R2_GPIOC18_3     0x3
#define GPIO_GPIOC_DRV_R2_GPIOC18_4     0x4
#define GPIO_GPIOC_DRV_R2_GPIOC18_5     0x5
#define GPIO_GPIOC_DRV_R2_GPIOC18_6     0x6
#define GPIO_GPIOC_DRV_R2_GPIOC18_7     0x7
#define GPIO_GPIOC_DRV_R2_GPIOC19_0     0x0
#define GPIO_GPIOC_DRV_R2_GPIOC19_1     0x1
#define GPIO_GPIOC_DRV_R2_GPIOC19_2     0x2
#define GPIO_GPIOC_DRV_R2_GPIOC19_3     0x3
#define GPIO_GPIOC_DRV_R2_GPIOC19_4     0x4
#define GPIO_GPIOC_DRV_R2_GPIOC19_5     0x5
#define GPIO_GPIOC_DRV_R2_GPIOC19_6     0x6
#define GPIO_GPIOC_DRV_R2_GPIOC19_7     0x7
#define GPIO_GPIOC_DRV_R2_GPIOC20_0     0x0
#define GPIO_GPIOC_DRV_R2_GPIOC20_1     0x1
#define GPIO_GPIOC_DRV_R2_GPIOC20_2     0x2
#define GPIO_GPIOC_DRV_R2_GPIOC20_3     0x3
#define GPIO_GPIOC_DRV_R2_GPIOC20_4     0x4
#define GPIO_GPIOC_DRV_R2_GPIOC20_5     0x5
#define GPIO_GPIOC_DRV_R2_GPIOC20_6     0x6
#define GPIO_GPIOC_DRV_R2_GPIOC20_7     0x7
#define GPIO_GPIOC_DRV_R2_GPIOC21_0     0x0
#define GPIO_GPIOC_DRV_R2_GPIOC21_1     0x1
#define GPIO_GPIOC_DRV_R2_GPIOC21_2     0x2
#define GPIO_GPIOC_DRV_R2_GPIOC21_3     0x3
#define GPIO_GPIOC_DRV_R2_GPIOC21_4     0x4
#define GPIO_GPIOC_DRV_R2_GPIOC21_5     0x5
#define GPIO_GPIOC_DRV_R2_GPIOC21_6     0x6
#define GPIO_GPIOC_DRV_R2_GPIOC21_7     0x7
#define GPIO_GPIOC_DRV_R2_GPIOC22_0     0x0
#define GPIO_GPIOC_DRV_R2_GPIOC22_1     0x1
#define GPIO_GPIOC_DRV_R2_GPIOC22_2     0x2
#define GPIO_GPIOC_DRV_R2_GPIOC22_3     0x3
#define GPIO_GPIOC_DRV_R2_GPIOC22_4     0x4
#define GPIO_GPIOC_DRV_R2_GPIOC22_5     0x5
#define GPIO_GPIOC_DRV_R2_GPIOC22_6     0x6
#define GPIO_GPIOC_DRV_R2_GPIOC22_7     0x7

/* GPIOC Data Register */
typedef union {
	u32 val;
	struct {
	u32 dat:23;            /* */
	u32 rsvd0:9;           /* */
	} bits;
} reg_gpio_gpioc_data_t;

/* GPIOC Pull Up/Down Resistors Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 pull_cfg:23;       /* */
	u32 rsvd0:9;           /* */
	} bits;
} reg_gpio_gpioc_pr_r_t;

#define GPIO_GPIOC_PR_R_PULL_CFG_0      0x0
#define GPIO_GPIOC_PR_R_PULL_CFG_1      0x1

/* GPIOD Function Configuration Register0 */
typedef union {
	u32 val;
	struct {
	u32 gpiod0:4;          /* */
	u32 gpiod1:4;          /* */
	u32 gpiod2:4;          /* */
	u32 gpiod3:4;          /* */
	u32 gpiod4:4;          /* */
	u32 gpiod5:4;          /* */
	u32 rsvd0:8;           /* */
	} bits;
} reg_gpio_gpiod_func_r0_t;

#define GPIO_GPIOD_FUNC_R0_GPIOD0_0     0x0
#define GPIO_GPIOD_FUNC_R0_GPIOD0_1     0x1
#define GPIO_GPIOD_FUNC_R0_GPIOD0_2     0x2
#define GPIO_GPIOD_FUNC_R0_GPIOD0_3     0x3
#define GPIO_GPIOD_FUNC_R0_GPIOD0_4     0x4
#define GPIO_GPIOD_FUNC_R0_GPIOD1_0     0x0
#define GPIO_GPIOD_FUNC_R0_GPIOD1_1     0x1
#define GPIO_GPIOD_FUNC_R0_GPIOD1_2     0x2
#define GPIO_GPIOD_FUNC_R0_GPIOD1_3     0x3
#define GPIO_GPIOD_FUNC_R0_GPIOD1_4     0x4
#define GPIO_GPIOD_FUNC_R0_GPIOD2_0     0x0
#define GPIO_GPIOD_FUNC_R0_GPIOD2_1     0x1
#define GPIO_GPIOD_FUNC_R0_GPIOD2_2     0x2
#define GPIO_GPIOD_FUNC_R0_GPIOD2_3     0x3
#define GPIO_GPIOD_FUNC_R0_GPIOD2_4     0x4
#define GPIO_GPIOD_FUNC_R0_GPIOD3_0     0x0
#define GPIO_GPIOD_FUNC_R0_GPIOD3_1     0x1
#define GPIO_GPIOD_FUNC_R0_GPIOD3_2     0x2
#define GPIO_GPIOD_FUNC_R0_GPIOD3_3     0x3
#define GPIO_GPIOD_FUNC_R0_GPIOD3_4     0x4
#define GPIO_GPIOD_FUNC_R0_GPIOD4_0     0x0
#define GPIO_GPIOD_FUNC_R0_GPIOD4_1     0x1
#define GPIO_GPIOD_FUNC_R0_GPIOD4_2     0x2
#define GPIO_GPIOD_FUNC_R0_GPIOD4_3     0x3
#define GPIO_GPIOD_FUNC_R0_GPIOD4_4     0x4
#define GPIO_GPIOD_FUNC_R0_GPIOD5_0     0x0
#define GPIO_GPIOD_FUNC_R0_GPIOD5_1     0x1
#define GPIO_GPIOD_FUNC_R0_GPIOD5_2     0x2
#define GPIO_GPIOD_FUNC_R0_GPIOD5_3     0x3
#define GPIO_GPIOD_FUNC_R0_GPIOD5_4     0x4

/* GPIOD Pull Up or Pull Down Configuration Register0 */
typedef union {
	u32 val;
	struct {
	u32 gpiod0:2;          /* */
	u32 rsvd0:2;           /* */
	u32 gpiod1:2;          /* */
	u32 rsvd1:2;           /* */
	u32 gpiod2:2;          /* */
	u32 rsvd2:2;           /* */
	u32 gpiod3:2;          /* */
	u32 rsvd3:2;           /* */
	u32 gpiod4:2;          /* */
	u32 rsvd4:2;           /* */
	u32 gpiod5:2;          /* */
	u32 rsvd5:10;          /* */
	} bits;
} reg_gpio_gpiod_pud_r0_t;

#define GPIO_GPIOD_PUD_R0_GPIOD0_0      0x0
#define GPIO_GPIOD_PUD_R0_GPIOD0_1      0x1
#define GPIO_GPIOD_PUD_R0_GPIOD0_2      0x2
#define GPIO_GPIOD_PUD_R0_GPIOD0_3      0x3
#define GPIO_GPIOD_PUD_R0_GPIOD1_0      0x0
#define GPIO_GPIOD_PUD_R0_GPIOD1_1      0x1
#define GPIO_GPIOD_PUD_R0_GPIOD1_2      0x2
#define GPIO_GPIOD_PUD_R0_GPIOD1_3      0x3
#define GPIO_GPIOD_PUD_R0_GPIOD2_0      0x0
#define GPIO_GPIOD_PUD_R0_GPIOD2_1      0x1
#define GPIO_GPIOD_PUD_R0_GPIOD2_2      0x2
#define GPIO_GPIOD_PUD_R0_GPIOD2_3      0x3
#define GPIO_GPIOD_PUD_R0_GPIOD3_0      0x0
#define GPIO_GPIOD_PUD_R0_GPIOD3_1      0x1
#define GPIO_GPIOD_PUD_R0_GPIOD3_2      0x2
#define GPIO_GPIOD_PUD_R0_GPIOD3_3      0x3
#define GPIO_GPIOD_PUD_R0_GPIOD4_0      0x0
#define GPIO_GPIOD_PUD_R0_GPIOD4_1      0x1
#define GPIO_GPIOD_PUD_R0_GPIOD4_2      0x2
#define GPIO_GPIOD_PUD_R0_GPIOD4_3      0x3
#define GPIO_GPIOD_PUD_R0_GPIOD5_0      0x0
#define GPIO_GPIOD_PUD_R0_GPIOD5_1      0x1
#define GPIO_GPIOD_PUD_R0_GPIOD5_2      0x2
#define GPIO_GPIOD_PUD_R0_GPIOD5_3      0x3

/* GPIOD Driver Configuration Register0 */
typedef union {
	u32 val;
	struct {
	u32 gpiod0:3;          /* */
	u32 rsvd0:1;           /* */
	u32 gpiod1:3;          /* */
	u32 rsvd1:1;           /* */
	u32 gpiod2:3;          /* */
	u32 rsvd2:1;           /* */
	u32 gpiod3:3;          /* */
	u32 rsvd3:1;           /* */
	u32 gpiod4:3;          /* */
	u32 rsvd4:1;           /* */
	u32 gpiod5:3;          /* */
	u32 rsvd5:9;           /* */
	} bits;
} reg_gpio_gpiod_drv_r0_t;

#define GPIO_GPIOD_DRV_R0_GPIOD0_0      0x0
#define GPIO_GPIOD_DRV_R0_GPIOD0_1      0x1
#define GPIO_GPIOD_DRV_R0_GPIOD0_2      0x2
#define GPIO_GPIOD_DRV_R0_GPIOD0_3      0x3
#define GPIO_GPIOD_DRV_R0_GPIOD0_4      0x4
#define GPIO_GPIOD_DRV_R0_GPIOD0_5      0x5
#define GPIO_GPIOD_DRV_R0_GPIOD0_6      0x6
#define GPIO_GPIOD_DRV_R0_GPIOD0_7      0x7
#define GPIO_GPIOD_DRV_R0_GPIOD1_0      0x0
#define GPIO_GPIOD_DRV_R0_GPIOD1_1      0x1
#define GPIO_GPIOD_DRV_R0_GPIOD1_2      0x2
#define GPIO_GPIOD_DRV_R0_GPIOD1_3      0x3
#define GPIO_GPIOD_DRV_R0_GPIOD1_4      0x4
#define GPIO_GPIOD_DRV_R0_GPIOD1_5      0x5
#define GPIO_GPIOD_DRV_R0_GPIOD1_6      0x6
#define GPIO_GPIOD_DRV_R0_GPIOD1_7      0x7
#define GPIO_GPIOD_DRV_R0_GPIOD2_0      0x0
#define GPIO_GPIOD_DRV_R0_GPIOD2_1      0x1
#define GPIO_GPIOD_DRV_R0_GPIOD2_2      0x2
#define GPIO_GPIOD_DRV_R0_GPIOD2_3      0x3
#define GPIO_GPIOD_DRV_R0_GPIOD2_4      0x4
#define GPIO_GPIOD_DRV_R0_GPIOD2_5      0x5
#define GPIO_GPIOD_DRV_R0_GPIOD2_6      0x6
#define GPIO_GPIOD_DRV_R0_GPIOD2_7      0x7
#define GPIO_GPIOD_DRV_R0_GPIOD3_0      0x0
#define GPIO_GPIOD_DRV_R0_GPIOD3_1      0x1
#define GPIO_GPIOD_DRV_R0_GPIOD3_2      0x2
#define GPIO_GPIOD_DRV_R0_GPIOD3_3      0x3
#define GPIO_GPIOD_DRV_R0_GPIOD3_4      0x4
#define GPIO_GPIOD_DRV_R0_GPIOD3_5      0x5
#define GPIO_GPIOD_DRV_R0_GPIOD3_6      0x6
#define GPIO_GPIOD_DRV_R0_GPIOD3_7      0x7
#define GPIO_GPIOD_DRV_R0_GPIOD4_0      0x0
#define GPIO_GPIOD_DRV_R0_GPIOD4_1      0x1
#define GPIO_GPIOD_DRV_R0_GPIOD4_2      0x2
#define GPIO_GPIOD_DRV_R0_GPIOD4_3      0x3
#define GPIO_GPIOD_DRV_R0_GPIOD4_4      0x4
#define GPIO_GPIOD_DRV_R0_GPIOD4_5      0x5
#define GPIO_GPIOD_DRV_R0_GPIOD4_6      0x6
#define GPIO_GPIOD_DRV_R0_GPIOD4_7      0x7
#define GPIO_GPIOD_DRV_R0_GPIOD5_0      0x0
#define GPIO_GPIOD_DRV_R0_GPIOD5_1      0x1
#define GPIO_GPIOD_DRV_R0_GPIOD5_2      0x2
#define GPIO_GPIOD_DRV_R0_GPIOD5_3      0x3
#define GPIO_GPIOD_DRV_R0_GPIOD5_4      0x4
#define GPIO_GPIOD_DRV_R0_GPIOD5_5      0x5
#define GPIO_GPIOD_DRV_R0_GPIOD5_6      0x6
#define GPIO_GPIOD_DRV_R0_GPIOD5_7      0x7

/* GPIOD Data Register */
typedef union {
	u32 val;
	struct {
	u32 dat:6;             /* */
	u32 rsvd0:26;          /* */
	} bits;
} reg_gpio_gpiod_data_t;

/* GPIOD Pull Up/Down Resistors Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 pull_cfg:6;        /* */
	u32 rsvd0:26;          /* */
	} bits;
} reg_gpio_gpiod_pr_r_t;

#define GPIO_GPIOD_PR_R_PULL_CFG_0      0x0
#define GPIO_GPIOD_PR_R_PULL_CFG_1      0x1

/* GPIOE Function Configuration Register0 */
typedef union {
	u32 val;
	struct {
	u32 gpioe0:4;          /* */
	u32 gpioe1:4;          /* */
	u32 gpioe2:4;          /* */
	u32 gpioe3:4;          /* */
	u32 gpioe4:4;          /* */
	u32 gpioe5:4;          /* */
	u32 gpioe6:4;          /* */
	u32 gpioe7:4;          /* */
	} bits;
} reg_gpio_gpioe_func_r0_t;

#define GPIO_GPIOE_FUNC_R0_GPIOE0_0     0x0
#define GPIO_GPIOE_FUNC_R0_GPIOE0_1     0x1
#define GPIO_GPIOE_FUNC_R0_GPIOE0_2     0x2
#define GPIO_GPIOE_FUNC_R0_GPIOE0_3     0x3
#define GPIO_GPIOE_FUNC_R0_GPIOE0_4     0x4
#define GPIO_GPIOE_FUNC_R0_GPIOE0_5     0x5
#define GPIO_GPIOE_FUNC_R0_GPIOE0_6     0x6
#define GPIO_GPIOE_FUNC_R0_GPIOE1_0     0x0
#define GPIO_GPIOE_FUNC_R0_GPIOE1_1     0x1
#define GPIO_GPIOE_FUNC_R0_GPIOE1_2     0x2
#define GPIO_GPIOE_FUNC_R0_GPIOE1_3     0x3
#define GPIO_GPIOE_FUNC_R0_GPIOE1_4     0x4
#define GPIO_GPIOE_FUNC_R0_GPIOE1_5     0x5
#define GPIO_GPIOE_FUNC_R0_GPIOE1_6     0x6
#define GPIO_GPIOE_FUNC_R0_GPIOE2_0     0x0
#define GPIO_GPIOE_FUNC_R0_GPIOE2_1     0x1
#define GPIO_GPIOE_FUNC_R0_GPIOE2_2     0x2
#define GPIO_GPIOE_FUNC_R0_GPIOE2_3     0x3
#define GPIO_GPIOE_FUNC_R0_GPIOE2_4     0x4
#define GPIO_GPIOE_FUNC_R0_GPIOE2_5     0x5
#define GPIO_GPIOE_FUNC_R0_GPIOE2_6     0x6
#define GPIO_GPIOE_FUNC_R0_GPIOE3_0     0x0
#define GPIO_GPIOE_FUNC_R0_GPIOE3_1     0x1
#define GPIO_GPIOE_FUNC_R0_GPIOE3_2     0x2
#define GPIO_GPIOE_FUNC_R0_GPIOE3_3     0x3
#define GPIO_GPIOE_FUNC_R0_GPIOE3_4     0x4
#define GPIO_GPIOE_FUNC_R0_GPIOE3_5     0x5
#define GPIO_GPIOE_FUNC_R0_GPIOE4_0     0x0
#define GPIO_GPIOE_FUNC_R0_GPIOE4_1     0x1
#define GPIO_GPIOE_FUNC_R0_GPIOE4_2     0x2
#define GPIO_GPIOE_FUNC_R0_GPIOE4_3     0x3
#define GPIO_GPIOE_FUNC_R0_GPIOE4_4     0x4
#define GPIO_GPIOE_FUNC_R0_GPIOE4_5     0x5
#define GPIO_GPIOE_FUNC_R0_GPIOE4_6     0x6
#define GPIO_GPIOE_FUNC_R0_GPIOE4_7     0x7
#define GPIO_GPIOE_FUNC_R0_GPIOE5_0     0x0
#define GPIO_GPIOE_FUNC_R0_GPIOE5_1     0x1
#define GPIO_GPIOE_FUNC_R0_GPIOE5_2     0x2
#define GPIO_GPIOE_FUNC_R0_GPIOE5_3     0x3
#define GPIO_GPIOE_FUNC_R0_GPIOE5_4     0x4
#define GPIO_GPIOE_FUNC_R0_GPIOE5_5     0x5
#define GPIO_GPIOE_FUNC_R0_GPIOE5_6     0x6
#define GPIO_GPIOE_FUNC_R0_GPIOE5_7     0x7
#define GPIO_GPIOE_FUNC_R0_GPIOE6_0     0x0
#define GPIO_GPIOE_FUNC_R0_GPIOE6_1     0x1
#define GPIO_GPIOE_FUNC_R0_GPIOE6_2     0x2
#define GPIO_GPIOE_FUNC_R0_GPIOE6_3     0x3
#define GPIO_GPIOE_FUNC_R0_GPIOE6_4     0x4
#define GPIO_GPIOE_FUNC_R0_GPIOE6_5     0x5
#define GPIO_GPIOE_FUNC_R0_GPIOE6_6     0x6
#define GPIO_GPIOE_FUNC_R0_GPIOE6_7     0x7
#define GPIO_GPIOE_FUNC_R0_GPIOE7_0     0x0
#define GPIO_GPIOE_FUNC_R0_GPIOE7_1     0x1
#define GPIO_GPIOE_FUNC_R0_GPIOE7_2     0x2
#define GPIO_GPIOE_FUNC_R0_GPIOE7_3     0x3
#define GPIO_GPIOE_FUNC_R0_GPIOE7_4     0x4
#define GPIO_GPIOE_FUNC_R0_GPIOE7_5     0x5
#define GPIO_GPIOE_FUNC_R0_GPIOE7_6     0x6
#define GPIO_GPIOE_FUNC_R0_GPIOE7_7     0x7

/* GPIOE Function Configuration Register1 */
typedef union {
	u32 val;
	struct {
	u32 gpioe8:4;          /* */
	u32 gpioe9:4;          /* */
	u32 gpioe10:4;         /* */
	u32 gpioe11:4;         /* */
	u32 gpioe12:4;         /* */
	u32 gpioe13:4;         /* */
	u32 gpioe14:4;         /* */
	u32 rsvd0:4;           /* */
	} bits;
} reg_gpio_gpioe_func_r1_t;

#define GPIO_GPIOE_FUNC_R1_GPIOE8_0     0x0
#define GPIO_GPIOE_FUNC_R1_GPIOE8_1     0x1
#define GPIO_GPIOE_FUNC_R1_GPIOE8_2     0x2
#define GPIO_GPIOE_FUNC_R1_GPIOE8_3     0x3
#define GPIO_GPIOE_FUNC_R1_GPIOE8_4     0x4
#define GPIO_GPIOE_FUNC_R1_GPIOE8_5     0x5
#define GPIO_GPIOE_FUNC_R1_GPIOE8_6     0x6
#define GPIO_GPIOE_FUNC_R1_GPIOE9_0     0x0
#define GPIO_GPIOE_FUNC_R1_GPIOE9_1     0x1
#define GPIO_GPIOE_FUNC_R1_GPIOE9_2     0x2
#define GPIO_GPIOE_FUNC_R1_GPIOE9_3     0x3
#define GPIO_GPIOE_FUNC_R1_GPIOE9_4     0x4
#define GPIO_GPIOE_FUNC_R1_GPIOE9_5     0x5
#define GPIO_GPIOE_FUNC_R1_GPIOE9_6     0x6
#define GPIO_GPIOE_FUNC_R1_GPIOE10_0    0x0
#define GPIO_GPIOE_FUNC_R1_GPIOE10_1    0x1
#define GPIO_GPIOE_FUNC_R1_GPIOE10_2    0x2
#define GPIO_GPIOE_FUNC_R1_GPIOE10_3    0x3
#define GPIO_GPIOE_FUNC_R1_GPIOE10_4    0x4
#define GPIO_GPIOE_FUNC_R1_GPIOE10_5    0x5
#define GPIO_GPIOE_FUNC_R1_GPIOE10_6    0x6
#define GPIO_GPIOE_FUNC_R1_GPIOE11_0    0x0
#define GPIO_GPIOE_FUNC_R1_GPIOE11_1    0x1
#define GPIO_GPIOE_FUNC_R1_GPIOE11_2    0x2
#define GPIO_GPIOE_FUNC_R1_GPIOE11_3    0x3
#define GPIO_GPIOE_FUNC_R1_GPIOE11_4    0x4
#define GPIO_GPIOE_FUNC_R1_GPIOE11_5    0x5
#define GPIO_GPIOE_FUNC_R1_GPIOE11_6    0x6
#define GPIO_GPIOE_FUNC_R1_GPIOE12_0    0x0
#define GPIO_GPIOE_FUNC_R1_GPIOE12_1    0x1
#define GPIO_GPIOE_FUNC_R1_GPIOE12_2    0x2
#define GPIO_GPIOE_FUNC_R1_GPIOE12_3    0x3
#define GPIO_GPIOE_FUNC_R1_GPIOE12_4    0x4
#define GPIO_GPIOE_FUNC_R1_GPIOE12_5    0x5
#define GPIO_GPIOE_FUNC_R1_GPIOE12_6    0x6
#define GPIO_GPIOE_FUNC_R1_GPIOE13_0    0x0
#define GPIO_GPIOE_FUNC_R1_GPIOE13_1    0x1
#define GPIO_GPIOE_FUNC_R1_GPIOE13_2    0x2
#define GPIO_GPIOE_FUNC_R1_GPIOE13_3    0x3
#define GPIO_GPIOE_FUNC_R1_GPIOE13_4    0x4
#define GPIO_GPIOE_FUNC_R1_GPIOE13_5    0x5
#define GPIO_GPIOE_FUNC_R1_GPIOE13_6    0x6
#define GPIO_GPIOE_FUNC_R1_GPIOE14_0    0x0
#define GPIO_GPIOE_FUNC_R1_GPIOE14_1    0x1
#define GPIO_GPIOE_FUNC_R1_GPIOE14_2    0x2
#define GPIO_GPIOE_FUNC_R1_GPIOE14_3    0x3
#define GPIO_GPIOE_FUNC_R1_GPIOE14_4    0x4
#define GPIO_GPIOE_FUNC_R1_GPIOE14_5    0x5
#define GPIO_GPIOE_FUNC_R1_GPIOE14_6    0x6

/* GPIOE Pull Up or Pull Down Configuration Register0 */
typedef union {
	u32 val;
	struct {
	u32 gpioe0:2;          /* */
	u32 rsvd0:2;           /* */
	u32 gpioe1:2;          /* */
	u32 rsvd1:2;           /* */
	u32 gpioe2:2;          /* */
	u32 rsvd2:2;           /* */
	u32 gpioe3:2;          /* */
	u32 rsvd3:2;           /* */
	u32 gpioe4:2;          /* */
	u32 rsvd4:2;           /* */
	u32 gpioe5:2;          /* */
	u32 rsvd5:2;           /* */
	u32 gpioe6:2;          /* */
	u32 rsvd6:2;           /* */
	u32 gpioe7:2;          /* */
	u32 rsvd7:2;           /* */
	} bits;
} reg_gpio_gpioe_pud_r0_t;

#define GPIO_GPIOE_PUD_R0_GPIOE0_0      0x0
#define GPIO_GPIOE_PUD_R0_GPIOE0_1      0x1
#define GPIO_GPIOE_PUD_R0_GPIOE0_2      0x2
#define GPIO_GPIOE_PUD_R0_GPIOE0_3      0x3
#define GPIO_GPIOE_PUD_R0_GPIOE1_0      0x0
#define GPIO_GPIOE_PUD_R0_GPIOE1_1      0x1
#define GPIO_GPIOE_PUD_R0_GPIOE1_2      0x2
#define GPIO_GPIOE_PUD_R0_GPIOE1_3      0x3
#define GPIO_GPIOE_PUD_R0_GPIOE2_0      0x0
#define GPIO_GPIOE_PUD_R0_GPIOE2_1      0x1
#define GPIO_GPIOE_PUD_R0_GPIOE2_2      0x2
#define GPIO_GPIOE_PUD_R0_GPIOE2_3      0x3
#define GPIO_GPIOE_PUD_R0_GPIOE3_0      0x0
#define GPIO_GPIOE_PUD_R0_GPIOE3_1      0x1
#define GPIO_GPIOE_PUD_R0_GPIOE3_2      0x2
#define GPIO_GPIOE_PUD_R0_GPIOE3_3      0x3
#define GPIO_GPIOE_PUD_R0_GPIOE4_0      0x0
#define GPIO_GPIOE_PUD_R0_GPIOE4_1      0x1
#define GPIO_GPIOE_PUD_R0_GPIOE4_2      0x2
#define GPIO_GPIOE_PUD_R0_GPIOE4_3      0x3
#define GPIO_GPIOE_PUD_R0_GPIOE5_0      0x0
#define GPIO_GPIOE_PUD_R0_GPIOE5_1      0x1
#define GPIO_GPIOE_PUD_R0_GPIOE5_2      0x2
#define GPIO_GPIOE_PUD_R0_GPIOE5_3      0x3
#define GPIO_GPIOE_PUD_R0_GPIOE6_0      0x0
#define GPIO_GPIOE_PUD_R0_GPIOE6_1      0x1
#define GPIO_GPIOE_PUD_R0_GPIOE6_2      0x2
#define GPIO_GPIOE_PUD_R0_GPIOE6_3      0x3
#define GPIO_GPIOE_PUD_R0_GPIOE7_0      0x0
#define GPIO_GPIOE_PUD_R0_GPIOE7_1      0x1
#define GPIO_GPIOE_PUD_R0_GPIOE7_2      0x2
#define GPIO_GPIOE_PUD_R0_GPIOE7_3      0x3

/* GPIOE Pull Up or Pull Down Configuration Register1 */
typedef union {
	u32 val;
	struct {
	u32 gpioe8:2;          /* */
	u32 rsvd0:2;           /* */
	u32 gpioe9:2;          /* */
	u32 rsvd1:2;           /* */
	u32 gpioe10:2;         /* */
	u32 rsvd2:2;           /* */
	u32 gpioe11:2;         /* */
	u32 rsvd3:2;           /* */
	u32 gpioe12:2;         /* */
	u32 rsvd4:2;           /* */
	u32 gpioe13:2;         /* */
	u32 rsvd5:2;           /* */
	u32 gpioe14:2;         /* */
	u32 rsvd6:6;           /* */
	} bits;
} reg_gpio_gpioe_pud_r1_t;

#define GPIO_GPIOE_PUD_R1_GPIOE8_0      0x0
#define GPIO_GPIOE_PUD_R1_GPIOE8_1      0x1
#define GPIO_GPIOE_PUD_R1_GPIOE8_2      0x2
#define GPIO_GPIOE_PUD_R1_GPIOE8_3      0x3
#define GPIO_GPIOE_PUD_R1_GPIOE9_0      0x0
#define GPIO_GPIOE_PUD_R1_GPIOE9_1      0x1
#define GPIO_GPIOE_PUD_R1_GPIOE9_2      0x2
#define GPIO_GPIOE_PUD_R1_GPIOE9_3      0x3
#define GPIO_GPIOE_PUD_R1_GPIOE10_0     0x0
#define GPIO_GPIOE_PUD_R1_GPIOE10_1     0x1
#define GPIO_GPIOE_PUD_R1_GPIOE10_2     0x2
#define GPIO_GPIOE_PUD_R1_GPIOE10_3     0x3
#define GPIO_GPIOE_PUD_R1_GPIOE11_0     0x0
#define GPIO_GPIOE_PUD_R1_GPIOE11_1     0x1
#define GPIO_GPIOE_PUD_R1_GPIOE11_2     0x2
#define GPIO_GPIOE_PUD_R1_GPIOE11_3     0x3
#define GPIO_GPIOE_PUD_R1_GPIOE12_0     0x0
#define GPIO_GPIOE_PUD_R1_GPIOE12_1     0x1
#define GPIO_GPIOE_PUD_R1_GPIOE12_2     0x2
#define GPIO_GPIOE_PUD_R1_GPIOE12_3     0x3
#define GPIO_GPIOE_PUD_R1_GPIOE13_0     0x0
#define GPIO_GPIOE_PUD_R1_GPIOE13_1     0x1
#define GPIO_GPIOE_PUD_R1_GPIOE13_2     0x2
#define GPIO_GPIOE_PUD_R1_GPIOE13_3     0x3
#define GPIO_GPIOE_PUD_R1_GPIOE14_0     0x0
#define GPIO_GPIOE_PUD_R1_GPIOE14_1     0x1
#define GPIO_GPIOE_PUD_R1_GPIOE14_2     0x2
#define GPIO_GPIOE_PUD_R1_GPIOE14_3     0x3

/* GPIOE Driver Configuration Register0 */
typedef union {
	u32 val;
	struct {
	u32 gpioe0:3;          /* */
	u32 rsvd0:1;           /* */
	u32 gpioe1:3;          /* */
	u32 rsvd1:1;           /* */
	u32 gpioe2:3;          /* */
	u32 rsvd2:1;           /* */
	u32 gpioe3:3;          /* */
	u32 rsvd3:1;           /* */
	u32 gpioe4:3;          /* */
	u32 rsvd4:1;           /* */
	u32 gpioe5:3;          /* */
	u32 rsvd5:1;           /* */
	u32 gpioe6:3;          /* */
	u32 rsvd6:1;           /* */
	u32 gpioe7:3;          /* */
	u32 rsvd7:1;           /* */
	} bits;
} reg_gpio_gpioe_drv_r0_t;

#define GPIO_GPIOE_DRV_R0_GPIOE0_0      0x0
#define GPIO_GPIOE_DRV_R0_GPIOE0_1      0x1
#define GPIO_GPIOE_DRV_R0_GPIOE0_2      0x2
#define GPIO_GPIOE_DRV_R0_GPIOE0_3      0x3
#define GPIO_GPIOE_DRV_R0_GPIOE0_4      0x4
#define GPIO_GPIOE_DRV_R0_GPIOE0_5      0x5
#define GPIO_GPIOE_DRV_R0_GPIOE0_6      0x6
#define GPIO_GPIOE_DRV_R0_GPIOE0_7      0x7
#define GPIO_GPIOE_DRV_R0_GPIOE1_0      0x0
#define GPIO_GPIOE_DRV_R0_GPIOE1_1      0x1
#define GPIO_GPIOE_DRV_R0_GPIOE1_2      0x2
#define GPIO_GPIOE_DRV_R0_GPIOE1_3      0x3
#define GPIO_GPIOE_DRV_R0_GPIOE1_4      0x4
#define GPIO_GPIOE_DRV_R0_GPIOE1_5      0x5
#define GPIO_GPIOE_DRV_R0_GPIOE1_6      0x6
#define GPIO_GPIOE_DRV_R0_GPIOE1_7      0x7
#define GPIO_GPIOE_DRV_R0_GPIOE2_0      0x0
#define GPIO_GPIOE_DRV_R0_GPIOE2_1      0x1
#define GPIO_GPIOE_DRV_R0_GPIOE2_2      0x2
#define GPIO_GPIOE_DRV_R0_GPIOE2_3      0x3
#define GPIO_GPIOE_DRV_R0_GPIOE2_4      0x4
#define GPIO_GPIOE_DRV_R0_GPIOE2_5      0x5
#define GPIO_GPIOE_DRV_R0_GPIOE2_6      0x6
#define GPIO_GPIOE_DRV_R0_GPIOE2_7      0x7
#define GPIO_GPIOE_DRV_R0_GPIOE3_0      0x0
#define GPIO_GPIOE_DRV_R0_GPIOE3_1      0x1
#define GPIO_GPIOE_DRV_R0_GPIOE3_2      0x2
#define GPIO_GPIOE_DRV_R0_GPIOE3_3      0x3
#define GPIO_GPIOE_DRV_R0_GPIOE3_4      0x4
#define GPIO_GPIOE_DRV_R0_GPIOE3_5      0x5
#define GPIO_GPIOE_DRV_R0_GPIOE3_6      0x6
#define GPIO_GPIOE_DRV_R0_GPIOE3_7      0x7
#define GPIO_GPIOE_DRV_R0_GPIOE4_0      0x0
#define GPIO_GPIOE_DRV_R0_GPIOE4_1      0x1
#define GPIO_GPIOE_DRV_R0_GPIOE4_2      0x2
#define GPIO_GPIOE_DRV_R0_GPIOE4_3      0x3
#define GPIO_GPIOE_DRV_R0_GPIOE4_4      0x4
#define GPIO_GPIOE_DRV_R0_GPIOE4_5      0x5
#define GPIO_GPIOE_DRV_R0_GPIOE4_6      0x6
#define GPIO_GPIOE_DRV_R0_GPIOE4_7      0x7
#define GPIO_GPIOE_DRV_R0_GPIOE5_0      0x0
#define GPIO_GPIOE_DRV_R0_GPIOE5_1      0x1
#define GPIO_GPIOE_DRV_R0_GPIOE5_2      0x2
#define GPIO_GPIOE_DRV_R0_GPIOE5_3      0x3
#define GPIO_GPIOE_DRV_R0_GPIOE5_4      0x4
#define GPIO_GPIOE_DRV_R0_GPIOE5_5      0x5
#define GPIO_GPIOE_DRV_R0_GPIOE5_6      0x6
#define GPIO_GPIOE_DRV_R0_GPIOE5_7      0x7
#define GPIO_GPIOE_DRV_R0_GPIOE6_0      0x0
#define GPIO_GPIOE_DRV_R0_GPIOE6_1      0x1
#define GPIO_GPIOE_DRV_R0_GPIOE6_2      0x2
#define GPIO_GPIOE_DRV_R0_GPIOE6_3      0x3
#define GPIO_GPIOE_DRV_R0_GPIOE6_4      0x4
#define GPIO_GPIOE_DRV_R0_GPIOE6_5      0x5
#define GPIO_GPIOE_DRV_R0_GPIOE6_6      0x6
#define GPIO_GPIOE_DRV_R0_GPIOE6_7      0x7
#define GPIO_GPIOE_DRV_R0_GPIOE7_0      0x0
#define GPIO_GPIOE_DRV_R0_GPIOE7_1      0x1
#define GPIO_GPIOE_DRV_R0_GPIOE7_2      0x2
#define GPIO_GPIOE_DRV_R0_GPIOE7_3      0x3
#define GPIO_GPIOE_DRV_R0_GPIOE7_4      0x4
#define GPIO_GPIOE_DRV_R0_GPIOE7_5      0x5
#define GPIO_GPIOE_DRV_R0_GPIOE7_6      0x6
#define GPIO_GPIOE_DRV_R0_GPIOE7_7      0x7

/* GPIOE Driver Configuration Register1 */
typedef union {
	u32 val;
	struct {
	u32 gpioe8:3;          /* */
	u32 rsvd0:1;           /* */
	u32 gpioe9:3;          /* */
	u32 rsvd1:1;           /* */
	u32 gpioe10:3;         /* */
	u32 rsvd2:1;           /* */
	u32 gpioe11:3;         /* */
	u32 rsvd3:1;           /* */
	u32 gpioe12:3;         /* */
	u32 rsvd4:1;           /* */
	u32 gpioe13:3;         /* */
	u32 rsvd5:1;           /* */
	u32 gpioe14:3;         /* */
	u32 rsvd6:5;           /* */
	} bits;
} reg_gpio_gpioe_drv_r1_t;

#define GPIO_GPIOE_DRV_R1_GPIOE8_0      0x0
#define GPIO_GPIOE_DRV_R1_GPIOE8_1      0x1
#define GPIO_GPIOE_DRV_R1_GPIOE8_2      0x2
#define GPIO_GPIOE_DRV_R1_GPIOE8_3      0x3
#define GPIO_GPIOE_DRV_R1_GPIOE8_4      0x4
#define GPIO_GPIOE_DRV_R1_GPIOE8_5      0x5
#define GPIO_GPIOE_DRV_R1_GPIOE8_6      0x6
#define GPIO_GPIOE_DRV_R1_GPIOE8_7      0x7
#define GPIO_GPIOE_DRV_R1_GPIOE9_0      0x0
#define GPIO_GPIOE_DRV_R1_GPIOE9_1      0x1
#define GPIO_GPIOE_DRV_R1_GPIOE9_2      0x2
#define GPIO_GPIOE_DRV_R1_GPIOE9_3      0x3
#define GPIO_GPIOE_DRV_R1_GPIOE9_4      0x4
#define GPIO_GPIOE_DRV_R1_GPIOE9_5      0x5
#define GPIO_GPIOE_DRV_R1_GPIOE9_6      0x6
#define GPIO_GPIOE_DRV_R1_GPIOE9_7      0x7
#define GPIO_GPIOE_DRV_R1_GPIOE10_0     0x0
#define GPIO_GPIOE_DRV_R1_GPIOE10_1     0x1
#define GPIO_GPIOE_DRV_R1_GPIOE10_2     0x2
#define GPIO_GPIOE_DRV_R1_GPIOE10_3     0x3
#define GPIO_GPIOE_DRV_R1_GPIOE10_4     0x4
#define GPIO_GPIOE_DRV_R1_GPIOE10_5     0x5
#define GPIO_GPIOE_DRV_R1_GPIOE10_6     0x6
#define GPIO_GPIOE_DRV_R1_GPIOE10_7     0x7
#define GPIO_GPIOE_DRV_R1_GPIOE11_0     0x0
#define GPIO_GPIOE_DRV_R1_GPIOE11_1     0x1
#define GPIO_GPIOE_DRV_R1_GPIOE11_2     0x2
#define GPIO_GPIOE_DRV_R1_GPIOE11_3     0x3
#define GPIO_GPIOE_DRV_R1_GPIOE11_4     0x4
#define GPIO_GPIOE_DRV_R1_GPIOE11_5     0x5
#define GPIO_GPIOE_DRV_R1_GPIOE11_6     0x6
#define GPIO_GPIOE_DRV_R1_GPIOE11_7     0x7
#define GPIO_GPIOE_DRV_R1_GPIOE12_0     0x0
#define GPIO_GPIOE_DRV_R1_GPIOE12_1     0x1
#define GPIO_GPIOE_DRV_R1_GPIOE12_2     0x2
#define GPIO_GPIOE_DRV_R1_GPIOE12_3     0x3
#define GPIO_GPIOE_DRV_R1_GPIOE12_4     0x4
#define GPIO_GPIOE_DRV_R1_GPIOE12_5     0x5
#define GPIO_GPIOE_DRV_R1_GPIOE12_6     0x6
#define GPIO_GPIOE_DRV_R1_GPIOE12_7     0x7
#define GPIO_GPIOE_DRV_R1_GPIOE13_0     0x0
#define GPIO_GPIOE_DRV_R1_GPIOE13_1     0x1
#define GPIO_GPIOE_DRV_R1_GPIOE13_2     0x2
#define GPIO_GPIOE_DRV_R1_GPIOE13_3     0x3
#define GPIO_GPIOE_DRV_R1_GPIOE13_4     0x4
#define GPIO_GPIOE_DRV_R1_GPIOE13_5     0x5
#define GPIO_GPIOE_DRV_R1_GPIOE13_6     0x6
#define GPIO_GPIOE_DRV_R1_GPIOE13_7     0x7
#define GPIO_GPIOE_DRV_R1_GPIOE14_0     0x0
#define GPIO_GPIOE_DRV_R1_GPIOE14_1     0x1
#define GPIO_GPIOE_DRV_R1_GPIOE14_2     0x2
#define GPIO_GPIOE_DRV_R1_GPIOE14_3     0x3
#define GPIO_GPIOE_DRV_R1_GPIOE14_4     0x4
#define GPIO_GPIOE_DRV_R1_GPIOE14_5     0x5
#define GPIO_GPIOE_DRV_R1_GPIOE14_6     0x6
#define GPIO_GPIOE_DRV_R1_GPIOE14_7     0x7

/* GPIOE Data Register */
typedef union {
	u32 val;
	struct {
	u32 dat:15;            /* */
	u32 rsvd0:17;          /* */
	} bits;
} reg_gpio_gpioe_data_t;

/* GPIOE Pull Up/Down Resistors Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 pull_cfg:15;       /* */
	u32 rsvd0:17;          /* */
	} bits;
} reg_gpio_gpioe_pr_r_t;

#define GPIO_GPIOE_PR_R_PULL_CFG_0      0x0
#define GPIO_GPIOE_PR_R_PULL_CFG_1      0x1

/* GPIOF Function Configuration Register0 */
typedef union {
	u32 val;
	struct {
	u32 gpiof0:4;          /* */
	u32 gpiof1:4;          /* */
	u32 gpiof2:4;          /* */
	u32 gpiof3:4;          /* */
	u32 gpiof4:4;          /* */
	u32 gpiof5:4;          /* */
	u32 gpiof6:4;          /* */
	u32 gpiof7:4;          /* */
	} bits;
} reg_gpio_gpiof_func_r0_t;

#define GPIO_GPIOF_FUNC_R0_GPIOF0_0     0x0
#define GPIO_GPIOF_FUNC_R0_GPIOF0_1     0x1
#define GPIO_GPIOF_FUNC_R0_GPIOF0_2     0x2
#define GPIO_GPIOF_FUNC_R0_GPIOF1_0     0x0
#define GPIO_GPIOF_FUNC_R0_GPIOF1_1     0x1
#define GPIO_GPIOF_FUNC_R0_GPIOF1_2     0x2
#define GPIO_GPIOF_FUNC_R0_GPIOF1_3     0x3
#define GPIO_GPIOF_FUNC_R0_GPIOF2_0     0x0
#define GPIO_GPIOF_FUNC_R0_GPIOF2_1     0x1
#define GPIO_GPIOF_FUNC_R0_GPIOF2_2     0x2
#define GPIO_GPIOF_FUNC_R0_GPIOF2_3     0x3
#define GPIO_GPIOF_FUNC_R0_GPIOF3_0     0x0
#define GPIO_GPIOF_FUNC_R0_GPIOF3_1     0x1
#define GPIO_GPIOF_FUNC_R0_GPIOF3_2     0x2
#define GPIO_GPIOF_FUNC_R0_GPIOF3_3     0x3
#define GPIO_GPIOF_FUNC_R0_GPIOF4_0     0x0
#define GPIO_GPIOF_FUNC_R0_GPIOF4_1     0x1
#define GPIO_GPIOF_FUNC_R0_GPIOF4_2     0x2
#define GPIO_GPIOF_FUNC_R0_GPIOF4_3     0x3
#define GPIO_GPIOF_FUNC_R0_GPIOF5_0     0x0
#define GPIO_GPIOF_FUNC_R0_GPIOF5_1     0x1
#define GPIO_GPIOF_FUNC_R0_GPIOF5_2     0x2
#define GPIO_GPIOF_FUNC_R0_GPIOF5_3     0x3
#define GPIO_GPIOF_FUNC_R0_GPIOF6_0     0x0
#define GPIO_GPIOF_FUNC_R0_GPIOF6_1     0x1
#define GPIO_GPIOF_FUNC_R0_GPIOF6_2     0x2
#define GPIO_GPIOF_FUNC_R0_GPIOF6_3     0x3
#define GPIO_GPIOF_FUNC_R0_GPIOF7_0     0x0
#define GPIO_GPIOF_FUNC_R0_GPIOF7_1     0x1
#define GPIO_GPIOF_FUNC_R0_GPIOF7_2     0x2
#define GPIO_GPIOF_FUNC_R0_GPIOF7_3     0x3

/* GPIOF Function Configuration Register1 */
typedef union {
	u32 val;
	struct {
	u32 gpiof8:4;          /* */
	u32 gpiof9:4;          /* */
	u32 rsvd0:24;          /* */
	} bits;
} reg_gpio_gpiof_func_r1_t;

#define GPIO_GPIOF_FUNC_R1_GPIOF8_0     0x0
#define GPIO_GPIOF_FUNC_R1_GPIOF8_1     0x1
#define GPIO_GPIOF_FUNC_R1_GPIOF8_2     0x2
#define GPIO_GPIOF_FUNC_R1_GPIOF8_3     0x3
#define GPIO_GPIOF_FUNC_R1_GPIOF9_0     0x0
#define GPIO_GPIOF_FUNC_R1_GPIOF9_1     0x1
#define GPIO_GPIOF_FUNC_R1_GPIOF9_2     0x2
#define GPIO_GPIOF_FUNC_R1_GPIOF9_3     0x3

/* GPIOF Pull Up or Pull Down Configuration Register0 */
typedef union {
	u32 val;
	struct {
	u32 gpiof0:2;          /* */
	u32 rsvd0:2;           /* */
	u32 gpiof1:2;          /* */
	u32 rsvd1:2;           /* */
	u32 gpiof2:2;          /* */
	u32 rsvd2:2;           /* */
	u32 gpiof3:2;          /* */
	u32 rsvd3:2;           /* */
	u32 gpiof4:2;          /* */
	u32 rsvd4:2;           /* */
	u32 gpiof5:2;          /* */
	u32 rsvd5:2;           /* */
	u32 gpiof6:2;          /* */
	u32 rsvd6:2;           /* */
	u32 gpiof7:2;          /* */
	u32 rsvd7:2;           /* */
	} bits;
} reg_gpio_gpiof_pud_r0_t;

#define GPIO_GPIOF_PUD_R0_GPIOF0_0      0x0
#define GPIO_GPIOF_PUD_R0_GPIOF0_1      0x1
#define GPIO_GPIOF_PUD_R0_GPIOF0_2      0x2
#define GPIO_GPIOF_PUD_R0_GPIOF0_3      0x3
#define GPIO_GPIOF_PUD_R0_GPIOF1_0      0x0
#define GPIO_GPIOF_PUD_R0_GPIOF1_1      0x1
#define GPIO_GPIOF_PUD_R0_GPIOF1_2      0x2
#define GPIO_GPIOF_PUD_R0_GPIOF1_3      0x3
#define GPIO_GPIOF_PUD_R0_GPIOF2_0      0x0
#define GPIO_GPIOF_PUD_R0_GPIOF2_1      0x1
#define GPIO_GPIOF_PUD_R0_GPIOF2_2      0x2
#define GPIO_GPIOF_PUD_R0_GPIOF2_3      0x3
#define GPIO_GPIOF_PUD_R0_GPIOF3_0      0x0
#define GPIO_GPIOF_PUD_R0_GPIOF3_1      0x1
#define GPIO_GPIOF_PUD_R0_GPIOF3_2      0x2
#define GPIO_GPIOF_PUD_R0_GPIOF3_3      0x3
#define GPIO_GPIOF_PUD_R0_GPIOF4_0      0x0
#define GPIO_GPIOF_PUD_R0_GPIOF4_1      0x1
#define GPIO_GPIOF_PUD_R0_GPIOF4_2      0x2
#define GPIO_GPIOF_PUD_R0_GPIOF4_3      0x3
#define GPIO_GPIOF_PUD_R0_GPIOF5_0      0x0
#define GPIO_GPIOF_PUD_R0_GPIOF5_1      0x1
#define GPIO_GPIOF_PUD_R0_GPIOF5_2      0x2
#define GPIO_GPIOF_PUD_R0_GPIOF5_3      0x3
#define GPIO_GPIOF_PUD_R0_GPIOF6_0      0x0
#define GPIO_GPIOF_PUD_R0_GPIOF6_1      0x1
#define GPIO_GPIOF_PUD_R0_GPIOF6_2      0x2
#define GPIO_GPIOF_PUD_R0_GPIOF6_3      0x3
#define GPIO_GPIOF_PUD_R0_GPIOF7_0      0x0
#define GPIO_GPIOF_PUD_R0_GPIOF7_1      0x1
#define GPIO_GPIOF_PUD_R0_GPIOF7_2      0x2
#define GPIO_GPIOF_PUD_R0_GPIOF7_3      0x3

/* GPIOF Pull Up or Pull Down Configuration Register1 */
typedef union {
	u32 val;
	struct {
	u32 gpiof8:2;          /* */
	u32 rsvd0:2;           /* */
	u32 gpiof9:2;          /* */
	u32 rsvd1:26;          /* */
	} bits;
} reg_gpio_gpiof_pud_r1_t;

#define GPIO_GPIOF_PUD_R1_GPIOF8_0      0x0
#define GPIO_GPIOF_PUD_R1_GPIOF8_1      0x1
#define GPIO_GPIOF_PUD_R1_GPIOF8_2      0x2
#define GPIO_GPIOF_PUD_R1_GPIOF8_3      0x3
#define GPIO_GPIOF_PUD_R1_GPIOF9_0      0x0
#define GPIO_GPIOF_PUD_R1_GPIOF9_1      0x1
#define GPIO_GPIOF_PUD_R1_GPIOF9_2      0x2
#define GPIO_GPIOF_PUD_R1_GPIOF9_3      0x3

/* GPIOF Driver Configuration Register0 */
typedef union {
	u32 val;
	struct {
	u32 gpiof0:3;          /* */
	u32 rsvd0:1;           /* */
	u32 gpiof1:3;          /* */
	u32 rsvd1:1;           /* */
	u32 gpiof2:3;          /* */
	u32 rsvd2:1;           /* */
	u32 gpiof3:3;          /* */
	u32 rsvd3:1;           /* */
	u32 gpiof4:3;          /* */
	u32 rsvd4:1;           /* */
	u32 gpiof5:3;          /* */
	u32 rsvd5:1;           /* */
	u32 gpiof6:3;          /* */
	u32 rsvd6:1;           /* */
	u32 gpiof7:3;          /* */
	u32 rsvd7:1;           /* */
	} bits;
} reg_gpio_gpiof_drv_r0_t;

#define GPIO_GPIOF_DRV_R0_GPIOF0_0      0x0
#define GPIO_GPIOF_DRV_R0_GPIOF0_1      0x1
#define GPIO_GPIOF_DRV_R0_GPIOF0_2      0x2
#define GPIO_GPIOF_DRV_R0_GPIOF0_3      0x3
#define GPIO_GPIOF_DRV_R0_GPIOF0_4      0x4
#define GPIO_GPIOF_DRV_R0_GPIOF0_5      0x5
#define GPIO_GPIOF_DRV_R0_GPIOF0_6      0x6
#define GPIO_GPIOF_DRV_R0_GPIOF0_7      0x7
#define GPIO_GPIOF_DRV_R0_GPIOF1_0      0x0
#define GPIO_GPIOF_DRV_R0_GPIOF1_1      0x1
#define GPIO_GPIOF_DRV_R0_GPIOF1_2      0x2
#define GPIO_GPIOF_DRV_R0_GPIOF1_3      0x3
#define GPIO_GPIOF_DRV_R0_GPIOF1_4      0x4
#define GPIO_GPIOF_DRV_R0_GPIOF1_5      0x5
#define GPIO_GPIOF_DRV_R0_GPIOF1_6      0x6
#define GPIO_GPIOF_DRV_R0_GPIOF1_7      0x7
#define GPIO_GPIOF_DRV_R0_GPIOF2_0      0x0
#define GPIO_GPIOF_DRV_R0_GPIOF2_1      0x1
#define GPIO_GPIOF_DRV_R0_GPIOF2_2      0x2
#define GPIO_GPIOF_DRV_R0_GPIOF2_3      0x3
#define GPIO_GPIOF_DRV_R0_GPIOF2_4      0x4
#define GPIO_GPIOF_DRV_R0_GPIOF2_5      0x5
#define GPIO_GPIOF_DRV_R0_GPIOF2_6      0x6
#define GPIO_GPIOF_DRV_R0_GPIOF2_7      0x7
#define GPIO_GPIOF_DRV_R0_GPIOF3_0      0x0
#define GPIO_GPIOF_DRV_R0_GPIOF3_1      0x1
#define GPIO_GPIOF_DRV_R0_GPIOF3_2      0x2
#define GPIO_GPIOF_DRV_R0_GPIOF3_3      0x3
#define GPIO_GPIOF_DRV_R0_GPIOF3_4      0x4
#define GPIO_GPIOF_DRV_R0_GPIOF3_5      0x5
#define GPIO_GPIOF_DRV_R0_GPIOF3_6      0x6
#define GPIO_GPIOF_DRV_R0_GPIOF3_7      0x7
#define GPIO_GPIOF_DRV_R0_GPIOF4_0      0x0
#define GPIO_GPIOF_DRV_R0_GPIOF4_1      0x1
#define GPIO_GPIOF_DRV_R0_GPIOF4_2      0x2
#define GPIO_GPIOF_DRV_R0_GPIOF4_3      0x3
#define GPIO_GPIOF_DRV_R0_GPIOF4_4      0x4
#define GPIO_GPIOF_DRV_R0_GPIOF4_5      0x5
#define GPIO_GPIOF_DRV_R0_GPIOF4_6      0x6
#define GPIO_GPIOF_DRV_R0_GPIOF4_7      0x7
#define GPIO_GPIOF_DRV_R0_GPIOF5_0      0x0
#define GPIO_GPIOF_DRV_R0_GPIOF5_1      0x1
#define GPIO_GPIOF_DRV_R0_GPIOF5_2      0x2
#define GPIO_GPIOF_DRV_R0_GPIOF5_3      0x3
#define GPIO_GPIOF_DRV_R0_GPIOF5_4      0x4
#define GPIO_GPIOF_DRV_R0_GPIOF5_5      0x5
#define GPIO_GPIOF_DRV_R0_GPIOF5_6      0x6
#define GPIO_GPIOF_DRV_R0_GPIOF5_7      0x7
#define GPIO_GPIOF_DRV_R0_GPIOF6_0      0x0
#define GPIO_GPIOF_DRV_R0_GPIOF6_1      0x1
#define GPIO_GPIOF_DRV_R0_GPIOF6_2      0x2
#define GPIO_GPIOF_DRV_R0_GPIOF6_3      0x3
#define GPIO_GPIOF_DRV_R0_GPIOF6_4      0x4
#define GPIO_GPIOF_DRV_R0_GPIOF6_5      0x5
#define GPIO_GPIOF_DRV_R0_GPIOF6_6      0x6
#define GPIO_GPIOF_DRV_R0_GPIOF6_7      0x7
#define GPIO_GPIOF_DRV_R0_GPIOF7_0      0x0
#define GPIO_GPIOF_DRV_R0_GPIOF7_1      0x1
#define GPIO_GPIOF_DRV_R0_GPIOF7_2      0x2
#define GPIO_GPIOF_DRV_R0_GPIOF7_3      0x3
#define GPIO_GPIOF_DRV_R0_GPIOF7_4      0x4
#define GPIO_GPIOF_DRV_R0_GPIOF7_5      0x5
#define GPIO_GPIOF_DRV_R0_GPIOF7_6      0x6
#define GPIO_GPIOF_DRV_R0_GPIOF7_7      0x7

/* GPIOF Driver Configuration Register1 */
typedef union {
	u32 val;
	struct {
	u32 gpiof8:3;          /* */
	u32 rsvd0:1;           /* */
	u32 gpiof9:3;          /* */
	u32 rsvd1:25;          /* */
	} bits;
} reg_gpio_gpiof_drv_r1_t;

#define GPIO_GPIOF_DRV_R1_GPIOF8_0      0x0
#define GPIO_GPIOF_DRV_R1_GPIOF8_1      0x1
#define GPIO_GPIOF_DRV_R1_GPIOF8_2      0x2
#define GPIO_GPIOF_DRV_R1_GPIOF8_3      0x3
#define GPIO_GPIOF_DRV_R1_GPIOF8_4      0x4
#define GPIO_GPIOF_DRV_R1_GPIOF8_5      0x5
#define GPIO_GPIOF_DRV_R1_GPIOF8_6      0x6
#define GPIO_GPIOF_DRV_R1_GPIOF8_7      0x7
#define GPIO_GPIOF_DRV_R1_GPIOF9_0      0x0
#define GPIO_GPIOF_DRV_R1_GPIOF9_1      0x1
#define GPIO_GPIOF_DRV_R1_GPIOF9_2      0x2
#define GPIO_GPIOF_DRV_R1_GPIOF9_3      0x3
#define GPIO_GPIOF_DRV_R1_GPIOF9_4      0x4
#define GPIO_GPIOF_DRV_R1_GPIOF9_5      0x5
#define GPIO_GPIOF_DRV_R1_GPIOF9_6      0x6
#define GPIO_GPIOF_DRV_R1_GPIOF9_7      0x7

/* GPIOF Data Register */
typedef union {
	u32 val;
	struct {
	u32 dat:10;            /* */
	u32 rsvd0:22;          /* */
	} bits;
} reg_gpio_gpiof_data_t;

/* GPIOF Pull Up/Down Resistors Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 pull_cfg:10;       /* */
	u32 rsvd0:22;          /* */
	} bits;
} reg_gpio_gpiof_pr_r_t;

#define GPIO_GPIOF_PR_R_PULL_CFG_0      0x0
#define GPIO_GPIOF_PR_R_PULL_CFG_1      0x1

/* GPIOG Function Configuration Register0 */
typedef union {
	u32 val;
	struct {
	u32 gpiog0:4;          /* */
	u32 gpiog1:4;          /* */
	u32 gpiog2:4;          /* */
	u32 gpiog3:4;          /* */
	u32 gpiog4:4;          /* */
	u32 gpiog5:4;          /* */
	u32 gpiog6:4;          /* */
	u32 gpiog7:4;          /* */
	} bits;
} reg_gpio_gpiog_func_r0_t;

#define GPIO_GPIOG_FUNC_R0_GPIOG0_0     0x0
#define GPIO_GPIOG_FUNC_R0_GPIOG0_1     0x1
#define GPIO_GPIOG_FUNC_R0_GPIOG0_2     0x2
#define GPIO_GPIOG_FUNC_R0_GPIOG0_3     0x3
#define GPIO_GPIOG_FUNC_R0_GPIOG1_0     0x0
#define GPIO_GPIOG_FUNC_R0_GPIOG1_1     0x1
#define GPIO_GPIOG_FUNC_R0_GPIOG1_2     0x2
#define GPIO_GPIOG_FUNC_R0_GPIOG1_3     0x3
#define GPIO_GPIOG_FUNC_R0_GPIOG2_0     0x0
#define GPIO_GPIOG_FUNC_R0_GPIOG2_1     0x1
#define GPIO_GPIOG_FUNC_R0_GPIOG2_2     0x2
#define GPIO_GPIOG_FUNC_R0_GPIOG2_3     0x3
#define GPIO_GPIOG_FUNC_R0_GPIOG3_0     0x0
#define GPIO_GPIOG_FUNC_R0_GPIOG3_1     0x1
#define GPIO_GPIOG_FUNC_R0_GPIOG3_2     0x2
#define GPIO_GPIOG_FUNC_R0_GPIOG3_3     0x3
#define GPIO_GPIOG_FUNC_R0_GPIOG4_0     0x0
#define GPIO_GPIOG_FUNC_R0_GPIOG4_1     0x1
#define GPIO_GPIOG_FUNC_R0_GPIOG4_2     0x2
#define GPIO_GPIOG_FUNC_R0_GPIOG4_3     0x3
#define GPIO_GPIOG_FUNC_R0_GPIOG5_0     0x0
#define GPIO_GPIOG_FUNC_R0_GPIOG5_1     0x1
#define GPIO_GPIOG_FUNC_R0_GPIOG5_2     0x2
#define GPIO_GPIOG_FUNC_R0_GPIOG5_3     0x3
#define GPIO_GPIOG_FUNC_R0_GPIOG6_0     0x0
#define GPIO_GPIOG_FUNC_R0_GPIOG6_1     0x1
#define GPIO_GPIOG_FUNC_R0_GPIOG6_2     0x2
#define GPIO_GPIOG_FUNC_R0_GPIOG6_3     0x3
#define GPIO_GPIOG_FUNC_R0_GPIOG6_4     0x4
#define GPIO_GPIOG_FUNC_R0_GPIOG7_0     0x0
#define GPIO_GPIOG_FUNC_R0_GPIOG7_1     0x1
#define GPIO_GPIOG_FUNC_R0_GPIOG7_2     0x2
#define GPIO_GPIOG_FUNC_R0_GPIOG7_3     0x3
#define GPIO_GPIOG_FUNC_R0_GPIOG7_4     0x4

/* GPIOG Function Configuration Register1 */
typedef union {
	u32 val;
	struct {
	u32 gpiog8:4;          /* */
	u32 gpiog9:4;          /* */
	u32 rsvd0:24;          /* */
	} bits;
} reg_gpio_gpiog_func_r1_t;

#define GPIO_GPIOG_FUNC_R1_GPIOG8_0     0x0
#define GPIO_GPIOG_FUNC_R1_GPIOG8_1     0x1
#define GPIO_GPIOG_FUNC_R1_GPIOG8_2     0x2
#define GPIO_GPIOG_FUNC_R1_GPIOG8_3     0x3
#define GPIO_GPIOG_FUNC_R1_GPIOG8_4     0x4
#define GPIO_GPIOG_FUNC_R1_GPIOG9_0     0x0
#define GPIO_GPIOG_FUNC_R1_GPIOG9_1     0x1
#define GPIO_GPIOG_FUNC_R1_GPIOG9_2     0x2
#define GPIO_GPIOG_FUNC_R1_GPIOG9_3     0x3
#define GPIO_GPIOG_FUNC_R1_GPIOG9_4     0x4

/* GPIOG Pull Up or Pull Down Configuration Register0 */
typedef union {
	u32 val;
	struct {
	u32 gpiog0:2;          /* */
	u32 rsvd0:2;           /* */
	u32 gpiog1:2;          /* */
	u32 rsvd1:2;           /* */
	u32 gpiog2:2;          /* */
	u32 rsvd2:2;           /* */
	u32 gpiog3:2;          /* */
	u32 rsvd3:2;           /* */
	u32 gpiog4:2;          /* */
	u32 rsvd4:2;           /* */
	u32 gpiog5:2;          /* */
	u32 rsvd5:2;           /* */
	u32 gpiog6:2;          /* */
	u32 rsvd6:2;           /* */
	u32 gpiog7:2;          /* */
	u32 rsvd7:2;           /* */
	} bits;
} reg_gpio_gpiog_pud_r0_t;

#define GPIO_GPIOG_PUD_R0_GPIOG0_0      0x0
#define GPIO_GPIOG_PUD_R0_GPIOG0_1      0x1
#define GPIO_GPIOG_PUD_R0_GPIOG0_2      0x2
#define GPIO_GPIOG_PUD_R0_GPIOG0_3      0x3
#define GPIO_GPIOG_PUD_R0_GPIOG1_0      0x0
#define GPIO_GPIOG_PUD_R0_GPIOG1_1      0x1
#define GPIO_GPIOG_PUD_R0_GPIOG1_2      0x2
#define GPIO_GPIOG_PUD_R0_GPIOG1_3      0x3
#define GPIO_GPIOG_PUD_R0_GPIOG2_0      0x0
#define GPIO_GPIOG_PUD_R0_GPIOG2_1      0x1
#define GPIO_GPIOG_PUD_R0_GPIOG2_2      0x2
#define GPIO_GPIOG_PUD_R0_GPIOG2_3      0x3
#define GPIO_GPIOG_PUD_R0_GPIOG3_0      0x0
#define GPIO_GPIOG_PUD_R0_GPIOG3_1      0x1
#define GPIO_GPIOG_PUD_R0_GPIOG3_2      0x2
#define GPIO_GPIOG_PUD_R0_GPIOG3_3      0x3
#define GPIO_GPIOG_PUD_R0_GPIOG4_0      0x0
#define GPIO_GPIOG_PUD_R0_GPIOG4_1      0x1
#define GPIO_GPIOG_PUD_R0_GPIOG4_2      0x2
#define GPIO_GPIOG_PUD_R0_GPIOG4_3      0x3
#define GPIO_GPIOG_PUD_R0_GPIOG5_0      0x0
#define GPIO_GPIOG_PUD_R0_GPIOG5_1      0x1
#define GPIO_GPIOG_PUD_R0_GPIOG5_2      0x2
#define GPIO_GPIOG_PUD_R0_GPIOG5_3      0x3
#define GPIO_GPIOG_PUD_R0_GPIOG6_0      0x0
#define GPIO_GPIOG_PUD_R0_GPIOG6_1      0x1
#define GPIO_GPIOG_PUD_R0_GPIOG6_2      0x2
#define GPIO_GPIOG_PUD_R0_GPIOG6_3      0x3
#define GPIO_GPIOG_PUD_R0_GPIOG7_0      0x0
#define GPIO_GPIOG_PUD_R0_GPIOG7_1      0x1
#define GPIO_GPIOG_PUD_R0_GPIOG7_2      0x2
#define GPIO_GPIOG_PUD_R0_GPIOG7_3      0x3

/* GPIOG Pull Up or Pull Down Configuration Register1 */
typedef union {
	u32 val;
	struct {
	u32 gpiog8:2;          /* */
	u32 rsvd0:2;           /* */
	u32 gpiog9:2;          /* */
	u32 rsvd1:26;          /* */
	} bits;
} reg_gpio_gpiog_pud_r1_t;

#define GPIO_GPIOG_PUD_R1_GPIOG8_0      0x0
#define GPIO_GPIOG_PUD_R1_GPIOG8_1      0x1
#define GPIO_GPIOG_PUD_R1_GPIOG8_2      0x2
#define GPIO_GPIOG_PUD_R1_GPIOG8_3      0x3
#define GPIO_GPIOG_PUD_R1_GPIOG9_0      0x0
#define GPIO_GPIOG_PUD_R1_GPIOG9_1      0x1
#define GPIO_GPIOG_PUD_R1_GPIOG9_2      0x2
#define GPIO_GPIOG_PUD_R1_GPIOG9_3      0x3

/* GPIOG Driver Configuration Register0 */
typedef union {
	u32 val;
	struct {
	u32 gpiog0:3;          /* */
	u32 rsvd0:1;           /* */
	u32 gpiog1:3;          /* */
	u32 rsvd1:1;           /* */
	u32 gpiog2:3;          /* */
	u32 rsvd2:1;           /* */
	u32 gpiog3:3;          /* */
	u32 rsvd3:1;           /* */
	u32 gpiog4:3;          /* */
	u32 rsvd4:1;           /* */
	u32 gpiog5:3;          /* */
	u32 rsvd5:1;           /* */
	u32 gpiog6:3;          /* */
	u32 rsvd6:1;           /* */
	u32 gpiog7:3;          /* */
	u32 rsvd7:1;           /* */
	} bits;
} reg_gpio_gpiog_drv_r0_t;

#define GPIO_GPIOG_DRV_R0_GPIOG0_0      0x0
#define GPIO_GPIOG_DRV_R0_GPIOG0_1      0x1
#define GPIO_GPIOG_DRV_R0_GPIOG0_2      0x2
#define GPIO_GPIOG_DRV_R0_GPIOG0_3      0x3
#define GPIO_GPIOG_DRV_R0_GPIOG0_4      0x4
#define GPIO_GPIOG_DRV_R0_GPIOG0_5      0x5
#define GPIO_GPIOG_DRV_R0_GPIOG0_6      0x6
#define GPIO_GPIOG_DRV_R0_GPIOG0_7      0x7
#define GPIO_GPIOG_DRV_R0_GPIOG1_0      0x0
#define GPIO_GPIOG_DRV_R0_GPIOG1_1      0x1
#define GPIO_GPIOG_DRV_R0_GPIOG1_2      0x2
#define GPIO_GPIOG_DRV_R0_GPIOG1_3      0x3
#define GPIO_GPIOG_DRV_R0_GPIOG1_4      0x4
#define GPIO_GPIOG_DRV_R0_GPIOG1_5      0x5
#define GPIO_GPIOG_DRV_R0_GPIOG1_6      0x6
#define GPIO_GPIOG_DRV_R0_GPIOG1_7      0x7
#define GPIO_GPIOG_DRV_R0_GPIOG2_0      0x0
#define GPIO_GPIOG_DRV_R0_GPIOG2_1      0x1
#define GPIO_GPIOG_DRV_R0_GPIOG2_2      0x2
#define GPIO_GPIOG_DRV_R0_GPIOG2_3      0x3
#define GPIO_GPIOG_DRV_R0_GPIOG2_4      0x4
#define GPIO_GPIOG_DRV_R0_GPIOG2_5      0x5
#define GPIO_GPIOG_DRV_R0_GPIOG2_6      0x6
#define GPIO_GPIOG_DRV_R0_GPIOG2_7      0x7
#define GPIO_GPIOG_DRV_R0_GPIOG3_0      0x0
#define GPIO_GPIOG_DRV_R0_GPIOG3_1      0x1
#define GPIO_GPIOG_DRV_R0_GPIOG3_2      0x2
#define GPIO_GPIOG_DRV_R0_GPIOG3_3      0x3
#define GPIO_GPIOG_DRV_R0_GPIOG3_4      0x4
#define GPIO_GPIOG_DRV_R0_GPIOG3_5      0x5
#define GPIO_GPIOG_DRV_R0_GPIOG3_6      0x6
#define GPIO_GPIOG_DRV_R0_GPIOG3_7      0x7
#define GPIO_GPIOG_DRV_R0_GPIOG4_0      0x0
#define GPIO_GPIOG_DRV_R0_GPIOG4_1      0x1
#define GPIO_GPIOG_DRV_R0_GPIOG4_2      0x2
#define GPIO_GPIOG_DRV_R0_GPIOG4_3      0x3
#define GPIO_GPIOG_DRV_R0_GPIOG4_4      0x4
#define GPIO_GPIOG_DRV_R0_GPIOG4_5      0x5
#define GPIO_GPIOG_DRV_R0_GPIOG4_6      0x6
#define GPIO_GPIOG_DRV_R0_GPIOG4_7      0x7
#define GPIO_GPIOG_DRV_R0_GPIOG5_0      0x0
#define GPIO_GPIOG_DRV_R0_GPIOG5_1      0x1
#define GPIO_GPIOG_DRV_R0_GPIOG5_2      0x2
#define GPIO_GPIOG_DRV_R0_GPIOG5_3      0x3
#define GPIO_GPIOG_DRV_R0_GPIOG5_4      0x4
#define GPIO_GPIOG_DRV_R0_GPIOG5_5      0x5
#define GPIO_GPIOG_DRV_R0_GPIOG5_6      0x6
#define GPIO_GPIOG_DRV_R0_GPIOG5_7      0x7
#define GPIO_GPIOG_DRV_R0_GPIOG6_0      0x0
#define GPIO_GPIOG_DRV_R0_GPIOG6_1      0x1
#define GPIO_GPIOG_DRV_R0_GPIOG6_2      0x2
#define GPIO_GPIOG_DRV_R0_GPIOG6_3      0x3
#define GPIO_GPIOG_DRV_R0_GPIOG6_4      0x4
#define GPIO_GPIOG_DRV_R0_GPIOG6_5      0x5
#define GPIO_GPIOG_DRV_R0_GPIOG6_6      0x6
#define GPIO_GPIOG_DRV_R0_GPIOG6_7      0x7
#define GPIO_GPIOG_DRV_R0_GPIOG7_0      0x0
#define GPIO_GPIOG_DRV_R0_GPIOG7_1      0x1
#define GPIO_GPIOG_DRV_R0_GPIOG7_2      0x2
#define GPIO_GPIOG_DRV_R0_GPIOG7_3      0x3
#define GPIO_GPIOG_DRV_R0_GPIOG7_4      0x4
#define GPIO_GPIOG_DRV_R0_GPIOG7_5      0x5
#define GPIO_GPIOG_DRV_R0_GPIOG7_6      0x6
#define GPIO_GPIOG_DRV_R0_GPIOG7_7      0x7

/* GPIOG Driver Configuration Register1 */
typedef union {
	u32 val;
	struct {
	u32 gpiog8:3;          /* */
	u32 rsvd0:1;           /* */
	u32 gpiog9:3;          /* */
	u32 rsvd1:25;          /* */
	} bits;
} reg_gpio_gpiog_drv_r1_t;

#define GPIO_GPIOG_DRV_R1_GPIOG8_0      0x0
#define GPIO_GPIOG_DRV_R1_GPIOG8_1      0x1
#define GPIO_GPIOG_DRV_R1_GPIOG8_2      0x2
#define GPIO_GPIOG_DRV_R1_GPIOG8_3      0x3
#define GPIO_GPIOG_DRV_R1_GPIOG8_4      0x4
#define GPIO_GPIOG_DRV_R1_GPIOG8_5      0x5
#define GPIO_GPIOG_DRV_R1_GPIOG8_6      0x6
#define GPIO_GPIOG_DRV_R1_GPIOG8_7      0x7
#define GPIO_GPIOG_DRV_R1_GPIOG9_0      0x0
#define GPIO_GPIOG_DRV_R1_GPIOG9_1      0x1
#define GPIO_GPIOG_DRV_R1_GPIOG9_2      0x2
#define GPIO_GPIOG_DRV_R1_GPIOG9_3      0x3
#define GPIO_GPIOG_DRV_R1_GPIOG9_4      0x4
#define GPIO_GPIOG_DRV_R1_GPIOG9_5      0x5
#define GPIO_GPIOG_DRV_R1_GPIOG9_6      0x6
#define GPIO_GPIOG_DRV_R1_GPIOG9_7      0x7

/* GPIOG Data Register */
typedef union {
	u32 val;
	struct {
	u32 dat:10;            /* */
	u32 rsvd0:22;          /* */
	} bits;
} reg_gpio_gpiog_data_t;

/* GPIOG Pull Up/Down Resistors Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 pull_cfg:10;       /* */
	u32 rsvd0:22;          /* */
	} bits;
} reg_gpio_gpiog_pr_r_t;

#define GPIO_GPIOG_PR_R_PULL_CFG_0      0x0
#define GPIO_GPIOG_PR_R_PULL_CFG_1      0x1

/* GPIOJ Function Configuration Register0 */
typedef union {
	u32 val;
	struct {
	u32 gpioj0:4;          /* */
	u32 rsvd0:28;          /* */
	} bits;
} reg_gpio_gpioj_func_r0_t;

#define GPIO_GPIOJ_FUNC_R0_GPIOJ0_0     0x0
#define GPIO_GPIOJ_FUNC_R0_GPIOJ0_1     0x1
#define GPIO_GPIOJ_FUNC_R0_GPIOJ0_2     0x2

/* GPIOJ Pull Up or Pull Down Configuration Register0 */
typedef union {
	u32 val;
	struct {
	u32 gpioj0:2;          /* */
	u32 rsvd0:30;          /* */
	} bits;
} reg_gpio_gpioj_pud_r0_t;

#define GPIO_GPIOJ_PUD_R0_GPIOJ0_0      0x0
#define GPIO_GPIOJ_PUD_R0_GPIOJ0_1      0x1
#define GPIO_GPIOJ_PUD_R0_GPIOJ0_2      0x2
#define GPIO_GPIOJ_PUD_R0_GPIOJ0_3      0x3

/* GPIOJ Driver Configuration Register0 */
typedef union {
	u32 val;
	struct {
	u32 gpioj0:3;          /* */
	u32 rsvd0:29;          /* */
	} bits;
} reg_gpio_gpioj_drv_r0_t;

#define GPIO_GPIOJ_DRV_R0_GPIOJ0_0      0x0
#define GPIO_GPIOJ_DRV_R0_GPIOJ0_1      0x1
#define GPIO_GPIOJ_DRV_R0_GPIOJ0_2      0x2
#define GPIO_GPIOJ_DRV_R0_GPIOJ0_3      0x3
#define GPIO_GPIOJ_DRV_R0_GPIOJ0_4      0x4
#define GPIO_GPIOJ_DRV_R0_GPIOJ0_5      0x5
#define GPIO_GPIOJ_DRV_R0_GPIOJ0_6      0x6
#define GPIO_GPIOJ_DRV_R0_GPIOJ0_7      0x7

/* GPIOJ Data Register */
typedef union {
	u32 val;
	struct {
	u32 dat:1;             /* */
	u32 rsvd0:31;          /* */
	} bits;
} reg_gpio_gpioj_data_t;

/* GPIOJ Pull Up/Down Resistors Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 pull_cfg:1;        /* */
	u32 rsvd0:31;          /* */
	} bits;
} reg_gpio_gpioj_pr_r_t;

#define GPIO_GPIOJ_PR_R_PULL_CFG_0      0x0
#define GPIO_GPIOJ_PR_R_PULL_CFG_1      0x1

/* GPIOB External Interrupt Trigger Configuration Register0 */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:8;           /* */
	u32 gpiob2:3;          /* */
	u32 rsvd1:1;           /* */
	u32 gpiob3:3;          /* */
	u32 rsvd2:1;           /* */
	u32 gpiob4:3;          /* */
	u32 rsvd3:1;           /* */
	u32 gpiob5:3;          /* */
	u32 rsvd4:1;           /* */
	u32 gpiob6:3;          /* */
	u32 rsvd5:1;           /* */
	u32 gpiob7:3;          /* */
	u32 rsvd6:1;           /* */
	} bits;
} reg_gpio_gpiob_int_tri_r0_t;

#define GPIO_GPIOB_INT_TRI_R0_GPIOB2_0  0x0
#define GPIO_GPIOB_INT_TRI_R0_GPIOB2_1  0x1
#define GPIO_GPIOB_INT_TRI_R0_GPIOB2_2  0x2
#define GPIO_GPIOB_INT_TRI_R0_GPIOB2_3  0x3
#define GPIO_GPIOB_INT_TRI_R0_GPIOB2_4  0x4
#define GPIO_GPIOB_INT_TRI_R0_GPIOB3_0  0x0
#define GPIO_GPIOB_INT_TRI_R0_GPIOB3_1  0x1
#define GPIO_GPIOB_INT_TRI_R0_GPIOB3_2  0x2
#define GPIO_GPIOB_INT_TRI_R0_GPIOB3_3  0x3
#define GPIO_GPIOB_INT_TRI_R0_GPIOB3_4  0x4
#define GPIO_GPIOB_INT_TRI_R0_GPIOB4_0  0x0
#define GPIO_GPIOB_INT_TRI_R0_GPIOB4_1  0x1
#define GPIO_GPIOB_INT_TRI_R0_GPIOB4_2  0x2
#define GPIO_GPIOB_INT_TRI_R0_GPIOB4_3  0x3
#define GPIO_GPIOB_INT_TRI_R0_GPIOB4_4  0x4
#define GPIO_GPIOB_INT_TRI_R0_GPIOB5_0  0x0
#define GPIO_GPIOB_INT_TRI_R0_GPIOB5_1  0x1
#define GPIO_GPIOB_INT_TRI_R0_GPIOB5_2  0x2
#define GPIO_GPIOB_INT_TRI_R0_GPIOB5_3  0x3
#define GPIO_GPIOB_INT_TRI_R0_GPIOB5_4  0x4
#define GPIO_GPIOB_INT_TRI_R0_GPIOB6_0  0x0
#define GPIO_GPIOB_INT_TRI_R0_GPIOB6_1  0x1
#define GPIO_GPIOB_INT_TRI_R0_GPIOB6_2  0x2
#define GPIO_GPIOB_INT_TRI_R0_GPIOB6_3  0x3
#define GPIO_GPIOB_INT_TRI_R0_GPIOB6_4  0x4
#define GPIO_GPIOB_INT_TRI_R0_GPIOB7_0  0x0
#define GPIO_GPIOB_INT_TRI_R0_GPIOB7_1  0x1
#define GPIO_GPIOB_INT_TRI_R0_GPIOB7_2  0x2
#define GPIO_GPIOB_INT_TRI_R0_GPIOB7_3  0x3
#define GPIO_GPIOB_INT_TRI_R0_GPIOB7_4  0x4

/* GPIOB External Interrupt Trigger Configuration Register1 */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:8;           /* */
	u32 gpiob10:3;         /* */
	u32 rsvd1:1;           /* */
	u32 gpiob11:3;         /* */
	u32 rsvd2:1;           /* */
	u32 gpiob12:3;         /* */
	u32 rsvd3:1;           /* */
	u32 gpiob13:3;         /* */
	u32 rsvd4:1;           /* */
	u32 gpiob14:3;         /* */
	u32 rsvd5:1;           /* */
	u32 gpiob15:3;         /* */
	u32 rsvd6:1;           /* */
	} bits;
} reg_gpio_gpiob_int_tri_r1_t;

#define GPIO_GPIOB_INT_TRI_R1_GPIOB10_0 0x0
#define GPIO_GPIOB_INT_TRI_R1_GPIOB10_1 0x1
#define GPIO_GPIOB_INT_TRI_R1_GPIOB10_2 0x2
#define GPIO_GPIOB_INT_TRI_R1_GPIOB10_3 0x3
#define GPIO_GPIOB_INT_TRI_R1_GPIOB10_4 0x4
#define GPIO_GPIOB_INT_TRI_R1_GPIOB11_0 0x0
#define GPIO_GPIOB_INT_TRI_R1_GPIOB11_1 0x1
#define GPIO_GPIOB_INT_TRI_R1_GPIOB11_2 0x2
#define GPIO_GPIOB_INT_TRI_R1_GPIOB11_3 0x3
#define GPIO_GPIOB_INT_TRI_R1_GPIOB11_4 0x4
#define GPIO_GPIOB_INT_TRI_R1_GPIOB12_0 0x0
#define GPIO_GPIOB_INT_TRI_R1_GPIOB12_1 0x1
#define GPIO_GPIOB_INT_TRI_R1_GPIOB12_2 0x2
#define GPIO_GPIOB_INT_TRI_R1_GPIOB12_3 0x3
#define GPIO_GPIOB_INT_TRI_R1_GPIOB12_4 0x4
#define GPIO_GPIOB_INT_TRI_R1_GPIOB13_0 0x0
#define GPIO_GPIOB_INT_TRI_R1_GPIOB13_1 0x1
#define GPIO_GPIOB_INT_TRI_R1_GPIOB13_2 0x2
#define GPIO_GPIOB_INT_TRI_R1_GPIOB13_3 0x3
#define GPIO_GPIOB_INT_TRI_R1_GPIOB13_4 0x4
#define GPIO_GPIOB_INT_TRI_R1_GPIOB14_0 0x0
#define GPIO_GPIOB_INT_TRI_R1_GPIOB14_1 0x1
#define GPIO_GPIOB_INT_TRI_R1_GPIOB14_2 0x2
#define GPIO_GPIOB_INT_TRI_R1_GPIOB14_3 0x3
#define GPIO_GPIOB_INT_TRI_R1_GPIOB14_4 0x4
#define GPIO_GPIOB_INT_TRI_R1_GPIOB15_0 0x0
#define GPIO_GPIOB_INT_TRI_R1_GPIOB15_1 0x1
#define GPIO_GPIOB_INT_TRI_R1_GPIOB15_2 0x2
#define GPIO_GPIOB_INT_TRI_R1_GPIOB15_3 0x3
#define GPIO_GPIOB_INT_TRI_R1_GPIOB15_4 0x4

/* GPIOB External Interrupt Trigger Configuration Register2 */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:8;           /* */
	u32 gpiob18:3;         /* */
	u32 rsvd1:1;           /* */
	u32 gpiob19:3;         /* */
	u32 rsvd2:1;           /* */
	u32 gpiob20:3;         /* */
	u32 rsvd3:1;           /* */
	u32 gpiob21:3;         /* */
	u32 rsvd4:1;           /* */
	u32 gpiob22:3;         /* */
	u32 rsvd5:1;           /* */
	u32 gpiob23:3;         /* */
	u32 rsvd6:1;           /* */
	} bits;
} reg_gpio_gpiob_int_tri_r2_t;

#define GPIO_GPIOB_INT_TRI_R2_GPIOB18_0 0x0
#define GPIO_GPIOB_INT_TRI_R2_GPIOB18_1 0x1
#define GPIO_GPIOB_INT_TRI_R2_GPIOB18_2 0x2
#define GPIO_GPIOB_INT_TRI_R2_GPIOB18_3 0x3
#define GPIO_GPIOB_INT_TRI_R2_GPIOB18_4 0x4
#define GPIO_GPIOB_INT_TRI_R2_GPIOB19_0 0x0
#define GPIO_GPIOB_INT_TRI_R2_GPIOB19_1 0x1
#define GPIO_GPIOB_INT_TRI_R2_GPIOB19_2 0x2
#define GPIO_GPIOB_INT_TRI_R2_GPIOB19_3 0x3
#define GPIO_GPIOB_INT_TRI_R2_GPIOB19_4 0x4
#define GPIO_GPIOB_INT_TRI_R2_GPIOB20_0 0x0
#define GPIO_GPIOB_INT_TRI_R2_GPIOB20_1 0x1
#define GPIO_GPIOB_INT_TRI_R2_GPIOB20_2 0x2
#define GPIO_GPIOB_INT_TRI_R2_GPIOB20_3 0x3
#define GPIO_GPIOB_INT_TRI_R2_GPIOB20_4 0x4
#define GPIO_GPIOB_INT_TRI_R2_GPIOB21_0 0x0
#define GPIO_GPIOB_INT_TRI_R2_GPIOB21_1 0x1
#define GPIO_GPIOB_INT_TRI_R2_GPIOB21_2 0x2
#define GPIO_GPIOB_INT_TRI_R2_GPIOB21_3 0x3
#define GPIO_GPIOB_INT_TRI_R2_GPIOB21_4 0x4
#define GPIO_GPIOB_INT_TRI_R2_GPIOB22_0 0x0
#define GPIO_GPIOB_INT_TRI_R2_GPIOB22_1 0x1
#define GPIO_GPIOB_INT_TRI_R2_GPIOB22_2 0x2
#define GPIO_GPIOB_INT_TRI_R2_GPIOB22_3 0x3
#define GPIO_GPIOB_INT_TRI_R2_GPIOB22_4 0x4
#define GPIO_GPIOB_INT_TRI_R2_GPIOB23_0 0x0
#define GPIO_GPIOB_INT_TRI_R2_GPIOB23_1 0x1
#define GPIO_GPIOB_INT_TRI_R2_GPIOB23_2 0x2
#define GPIO_GPIOB_INT_TRI_R2_GPIOB23_3 0x3
#define GPIO_GPIOB_INT_TRI_R2_GPIOB23_4 0x4

/* GPIOB External Interrupt Trigger Configuration Register3 */
typedef union {
	u32 val;
	struct {
	u32 gpiob24:3;         /* */
	u32 rsvd0:1;           /* */
	u32 gpiob25:3;         /* */
	u32 rsvd1:1;           /* */
	u32 gpiob26:3;         /* */
	u32 rsvd2:1;           /* */
	u32 gpiob27:3;         /* */
	u32 rsvd3:17;          /* */
	} bits;
} reg_gpio_gpiob_int_tri_r3_t;

#define GPIO_GPIOB_INT_TRI_R3_GPIOB24_0 0x0
#define GPIO_GPIOB_INT_TRI_R3_GPIOB24_1 0x1
#define GPIO_GPIOB_INT_TRI_R3_GPIOB24_2 0x2
#define GPIO_GPIOB_INT_TRI_R3_GPIOB24_3 0x3
#define GPIO_GPIOB_INT_TRI_R3_GPIOB24_4 0x4
#define GPIO_GPIOB_INT_TRI_R3_GPIOB25_0 0x0
#define GPIO_GPIOB_INT_TRI_R3_GPIOB25_1 0x1
#define GPIO_GPIOB_INT_TRI_R3_GPIOB25_2 0x2
#define GPIO_GPIOB_INT_TRI_R3_GPIOB25_3 0x3
#define GPIO_GPIOB_INT_TRI_R3_GPIOB25_4 0x4
#define GPIO_GPIOB_INT_TRI_R3_GPIOB26_0 0x0
#define GPIO_GPIOB_INT_TRI_R3_GPIOB26_1 0x1
#define GPIO_GPIOB_INT_TRI_R3_GPIOB26_2 0x2
#define GPIO_GPIOB_INT_TRI_R3_GPIOB26_3 0x3
#define GPIO_GPIOB_INT_TRI_R3_GPIOB26_4 0x4
#define GPIO_GPIOB_INT_TRI_R3_GPIOB27_0 0x0
#define GPIO_GPIOB_INT_TRI_R3_GPIOB27_1 0x1
#define GPIO_GPIOB_INT_TRI_R3_GPIOB27_2 0x2
#define GPIO_GPIOB_INT_TRI_R3_GPIOB27_3 0x3
#define GPIO_GPIOB_INT_TRI_R3_GPIOB27_4 0x4

/* GPIOB External Interrupt Mask Register */
typedef union {
	u32 val;
	struct {
	u32 mask_en:28;        /* */
	u32 rsvd0:4;           /* */
	} bits;
} reg_gpio_gpiob_int_mask_t;

#define GPIO_GPIOB_INT_MASK_MASK_EN_0   0x0
#define GPIO_GPIOB_INT_MASK_MASK_EN_1   0x1

/* GPIOB External Interrupt Pending Register */
typedef union {
	u32 val;
	struct {
	u32 pending:28;        /* */
	u32 rsvd0:4;           /* */
	} bits;
} reg_gpio_gpiob_int_pend_t;

#define GPIO_GPIOB_INT_PEND_PENDING_0   0x0
#define GPIO_GPIOB_INT_PEND_PENDING_1   0x1

/* GPIOB External Interrupt Sample Time Register */
typedef union {
	u32 val;
	struct {
	u32 src_sel:2;         /* */
	u32 rsvd0:2;           /* */
	u32 div:4;             /* */
	u32 rsvd1:23;          /* */
	u32 en:1;              /* */
	} bits;
} reg_gpio_gpiob_int_samp_ctrl_t;

#define GPIO_GPIOB_INT_SAMP_CTRL_SRC_SEL_0      0x0
#define GPIO_GPIOB_INT_SAMP_CTRL_SRC_SEL_1      0x1
#define GPIO_GPIOB_INT_SAMP_CTRL_DIV_0          0x0
#define GPIO_GPIOB_INT_SAMP_CTRL_DIV_1          0x1
#define GPIO_GPIOB_INT_SAMP_CTRL_DIV_2          0x2
#define GPIO_GPIOB_INT_SAMP_CTRL_DIV_3          0x3
#define GPIO_GPIOB_INT_SAMP_CTRL_DIV_4          0x4
#define GPIO_GPIOB_INT_SAMP_CTRL_DIV_5          0x5
#define GPIO_GPIOB_INT_SAMP_CTRL_DIV_6          0x6
#define GPIO_GPIOB_INT_SAMP_CTRL_DIV_7          0x7
#define GPIO_GPIOB_INT_SAMP_CTRL_DIV_8          0x8
#define GPIO_GPIOB_INT_SAMP_CTRL_DIV_9          0x9
#define GPIO_GPIOB_INT_SAMP_CTRL_DIV_A          0xA
#define GPIO_GPIOB_INT_SAMP_CTRL_EN_0           0x0
#define GPIO_GPIOB_INT_SAMP_CTRL_EN_1           0x1

/* GPIE External Interrupt Trigger Configuration Register0 */
typedef union {
	u32 val;
	struct {
	u32 gpioe0:3;          /* */
	u32 rsvd0:1;           /* */
	u32 gpioe1:3;          /* */
	u32 rsvd1:1;           /* */
	u32 gpioe2:3;          /* */
	u32 rsvd2:1;           /* */
	u32 gpioe3:3;          /* */
	u32 rsvd3:1;           /* */
	u32 gpioe4:3;          /* */
	u32 rsvd4:1;           /* */
	u32 gpioe5:3;          /* */
	u32 rsvd5:1;           /* */
	u32 gpioe6:3;          /* */
	u32 rsvd6:1;           /* */
	u32 gpioe7:3;          /* */
	u32 rsvd7:1;           /* */
	} bits;
} reg_gpio_gpioe_int_tri_r0_t;

#define GPIO_GPIOE_INT_TRI_R0_GPIOE0_0  0x0
#define GPIO_GPIOE_INT_TRI_R0_GPIOE0_1  0x1
#define GPIO_GPIOE_INT_TRI_R0_GPIOE0_2  0x2
#define GPIO_GPIOE_INT_TRI_R0_GPIOE0_3  0x3
#define GPIO_GPIOE_INT_TRI_R0_GPIOE0_4  0x4
#define GPIO_GPIOE_INT_TRI_R0_GPIOE1_0  0x0
#define GPIO_GPIOE_INT_TRI_R0_GPIOE1_1  0x1
#define GPIO_GPIOE_INT_TRI_R0_GPIOE1_2  0x2
#define GPIO_GPIOE_INT_TRI_R0_GPIOE1_3  0x3
#define GPIO_GPIOE_INT_TRI_R0_GPIOE1_4  0x4
#define GPIO_GPIOE_INT_TRI_R0_GPIOE2_0  0x0
#define GPIO_GPIOE_INT_TRI_R0_GPIOE2_1  0x1
#define GPIO_GPIOE_INT_TRI_R0_GPIOE2_2  0x2
#define GPIO_GPIOE_INT_TRI_R0_GPIOE2_3  0x3
#define GPIO_GPIOE_INT_TRI_R0_GPIOE2_4  0x4
#define GPIO_GPIOE_INT_TRI_R0_GPIOE3_0  0x0
#define GPIO_GPIOE_INT_TRI_R0_GPIOE3_1  0x1
#define GPIO_GPIOE_INT_TRI_R0_GPIOE3_2  0x2
#define GPIO_GPIOE_INT_TRI_R0_GPIOE3_3  0x3
#define GPIO_GPIOE_INT_TRI_R0_GPIOE3_4  0x4
#define GPIO_GPIOE_INT_TRI_R0_GPIOE4_0  0x0
#define GPIO_GPIOE_INT_TRI_R0_GPIOE4_1  0x1
#define GPIO_GPIOE_INT_TRI_R0_GPIOE4_2  0x2
#define GPIO_GPIOE_INT_TRI_R0_GPIOE4_3  0x3
#define GPIO_GPIOE_INT_TRI_R0_GPIOE4_4  0x4
#define GPIO_GPIOE_INT_TRI_R0_GPIOE5_0  0x0
#define GPIO_GPIOE_INT_TRI_R0_GPIOE5_1  0x1
#define GPIO_GPIOE_INT_TRI_R0_GPIOE5_2  0x2
#define GPIO_GPIOE_INT_TRI_R0_GPIOE5_3  0x3
#define GPIO_GPIOE_INT_TRI_R0_GPIOE5_4  0x4
#define GPIO_GPIOE_INT_TRI_R0_GPIOE6_0  0x0
#define GPIO_GPIOE_INT_TRI_R0_GPIOE6_1  0x1
#define GPIO_GPIOE_INT_TRI_R0_GPIOE6_2  0x2
#define GPIO_GPIOE_INT_TRI_R0_GPIOE6_3  0x3
#define GPIO_GPIOE_INT_TRI_R0_GPIOE6_4  0x4
#define GPIO_GPIOE_INT_TRI_R0_GPIOE7_0  0x0
#define GPIO_GPIOE_INT_TRI_R0_GPIOE7_1  0x1
#define GPIO_GPIOE_INT_TRI_R0_GPIOE7_2  0x2
#define GPIO_GPIOE_INT_TRI_R0_GPIOE7_3  0x3
#define GPIO_GPIOE_INT_TRI_R0_GPIOE7_4  0x4

/* GPIOE External Interrupt Trigger Configuration Register1 */
typedef union {
	u32 val;
	struct {
	u32 gpioe8:3;          /* */
	u32 rsvd0:1;           /* */
	u32 gpioe9:3;          /* */
	u32 rsvd1:1;           /* */
	u32 gpioe10:3;         /* */
	u32 rsvd2:1;           /* */
	u32 gpioe11:3;         /* */
	u32 rsvd3:1;           /* */
	u32 gpioe12:3;         /* */
	u32 rsvd4:1;           /* */
	u32 gpioe13:3;         /* */
	u32 rsvd5:1;           /* */
	u32 gpioe14:3;         /* */
	u32 rsvd6:5;           /* */
	} bits;
} reg_gpio_gpioe_int_tri_r1_t;

#define GPIO_GPIOE_INT_TRI_R1_GPIOE8_0  0x0
#define GPIO_GPIOE_INT_TRI_R1_GPIOE8_1  0x1
#define GPIO_GPIOE_INT_TRI_R1_GPIOE8_2  0x2
#define GPIO_GPIOE_INT_TRI_R1_GPIOE8_3  0x3
#define GPIO_GPIOE_INT_TRI_R1_GPIOE8_4  0x4
#define GPIO_GPIOE_INT_TRI_R1_GPIOE9_0  0x0
#define GPIO_GPIOE_INT_TRI_R1_GPIOE9_1  0x1
#define GPIO_GPIOE_INT_TRI_R1_GPIOE9_2  0x2
#define GPIO_GPIOE_INT_TRI_R1_GPIOE9_3  0x3
#define GPIO_GPIOE_INT_TRI_R1_GPIOE9_4  0x4
#define GPIO_GPIOE_INT_TRI_R1_GPIOE10_0 0x0
#define GPIO_GPIOE_INT_TRI_R1_GPIOE10_1 0x1
#define GPIO_GPIOE_INT_TRI_R1_GPIOE10_2 0x2
#define GPIO_GPIOE_INT_TRI_R1_GPIOE10_3 0x3
#define GPIO_GPIOE_INT_TRI_R1_GPIOE10_4 0x4
#define GPIO_GPIOE_INT_TRI_R1_GPIOE11_0 0x0
#define GPIO_GPIOE_INT_TRI_R1_GPIOE11_1 0x1
#define GPIO_GPIOE_INT_TRI_R1_GPIOE11_2 0x2
#define GPIO_GPIOE_INT_TRI_R1_GPIOE11_3 0x3
#define GPIO_GPIOE_INT_TRI_R1_GPIOE11_4 0x4
#define GPIO_GPIOE_INT_TRI_R1_GPIOE12_0 0x0
#define GPIO_GPIOE_INT_TRI_R1_GPIOE12_1 0x1
#define GPIO_GPIOE_INT_TRI_R1_GPIOE12_2 0x2
#define GPIO_GPIOE_INT_TRI_R1_GPIOE12_3 0x3
#define GPIO_GPIOE_INT_TRI_R1_GPIOE12_4 0x4
#define GPIO_GPIOE_INT_TRI_R1_GPIOE13_0 0x0
#define GPIO_GPIOE_INT_TRI_R1_GPIOE13_1 0x1
#define GPIO_GPIOE_INT_TRI_R1_GPIOE13_2 0x2
#define GPIO_GPIOE_INT_TRI_R1_GPIOE13_3 0x3
#define GPIO_GPIOE_INT_TRI_R1_GPIOE13_4 0x4
#define GPIO_GPIOE_INT_TRI_R1_GPIOE14_0 0x0
#define GPIO_GPIOE_INT_TRI_R1_GPIOE14_1 0x1
#define GPIO_GPIOE_INT_TRI_R1_GPIOE14_2 0x2
#define GPIO_GPIOE_INT_TRI_R1_GPIOE14_3 0x3
#define GPIO_GPIOE_INT_TRI_R1_GPIOE14_4 0x4

/* GPIOE External Interrupt Mask Register */
typedef union {
	u32 val;
	struct {
	u32 mask_en:15;        /* */
	u32 rsvd0:17;          /* */
	} bits;
} reg_gpio_gpioe_int_mask_t;

#define GPIO_GPIOE_INT_MASK_MASK_EN_0   0x0
#define GPIO_GPIOE_INT_MASK_MASK_EN_1   0x1

/* GPIOE External Interrupt Pending Register */
typedef union {
	u32 val;
	struct {
	u32 pending:15;        /* */
	u32 rsvd0:17;          /* */
	} bits;
} reg_gpio_gpioe_int_pend_t;

#define GPIO_GPIOE_INT_PEND_PENDING_0   0x0
#define GPIO_GPIOE_INT_PEND_PENDING_1   0x1

/* GPIOE External Interrupt Sample Time Register */
typedef union {
	u32 val;
	struct {
	u32 src_sel:2;         /* */
	u32 rsvd0:2;           /* */
	u32 div:4;             /* */
	u32 rsvd1:23;          /* */
	u32 en:1;              /* */
	} bits;
} reg_gpio_gpioe_int_samp_ctrl_t;

#define GPIO_GPIOE_INT_SAMP_CTRL_SRC_SEL_0      0x0
#define GPIO_GPIOE_INT_SAMP_CTRL_SRC_SEL_1      0x1
#define GPIO_GPIOE_INT_SAMP_CTRL_DIV_0          0x0
#define GPIO_GPIOE_INT_SAMP_CTRL_DIV_1          0x1
#define GPIO_GPIOE_INT_SAMP_CTRL_DIV_2          0x2
#define GPIO_GPIOE_INT_SAMP_CTRL_DIV_3          0x3
#define GPIO_GPIOE_INT_SAMP_CTRL_DIV_4          0x4
#define GPIO_GPIOE_INT_SAMP_CTRL_DIV_5          0x5
#define GPIO_GPIOE_INT_SAMP_CTRL_DIV_6          0x6
#define GPIO_GPIOE_INT_SAMP_CTRL_DIV_7          0x7
#define GPIO_GPIOE_INT_SAMP_CTRL_DIV_8          0x8
#define GPIO_GPIOE_INT_SAMP_CTRL_DIV_9          0x9
#define GPIO_GPIOE_INT_SAMP_CTRL_DIV_A          0xA
#define GPIO_GPIOE_INT_SAMP_CTRL_EN_0           0x0
#define GPIO_GPIOE_INT_SAMP_CTRL_EN_1           0x1

/* GPIOF External Interrupt Trigger Configuration Register0 */
typedef union {
	u32 val;
	struct {
	u32 gpiof0:3;          /* */
	u32 rsvd0:1;           /* */
	u32 gpiof1:3;          /* */
	u32 rsvd1:1;           /* */
	u32 gpiof2:3;          /* */
	u32 rsvd2:1;           /* */
	u32 gpiof3:3;          /* */
	u32 rsvd3:1;           /* */
	u32 gpiof4:3;          /* */
	u32 rsvd4:1;           /* */
	u32 gpiof5:3;          /* */
	u32 rsvd5:1;           /* */
	u32 gpiof6:3;          /* */
	u32 rsvd6:1;           /* */
	u32 gpiof7:3;          /* */
	u32 rsvd7:1;           /* */
	} bits;
} reg_gpio_gpiof_int_tri_r0_t;

#define GPIO_GPIOF_INT_TRI_R0_GPIOF0_0  0x0
#define GPIO_GPIOF_INT_TRI_R0_GPIOF0_1  0x1
#define GPIO_GPIOF_INT_TRI_R0_GPIOF0_2  0x2
#define GPIO_GPIOF_INT_TRI_R0_GPIOF0_3  0x3
#define GPIO_GPIOF_INT_TRI_R0_GPIOF0_4  0x4
#define GPIO_GPIOF_INT_TRI_R0_GPIOF1_0  0x0
#define GPIO_GPIOF_INT_TRI_R0_GPIOF1_1  0x1
#define GPIO_GPIOF_INT_TRI_R0_GPIOF1_2  0x2
#define GPIO_GPIOF_INT_TRI_R0_GPIOF1_3  0x3
#define GPIO_GPIOF_INT_TRI_R0_GPIOF1_4  0x4
#define GPIO_GPIOF_INT_TRI_R0_GPIOF2_0  0x0
#define GPIO_GPIOF_INT_TRI_R0_GPIOF2_1  0x1
#define GPIO_GPIOF_INT_TRI_R0_GPIOF2_2  0x2
#define GPIO_GPIOF_INT_TRI_R0_GPIOF2_3  0x3
#define GPIO_GPIOF_INT_TRI_R0_GPIOF2_4  0x4
#define GPIO_GPIOF_INT_TRI_R0_GPIOF3_0  0x0
#define GPIO_GPIOF_INT_TRI_R0_GPIOF3_1  0x1
#define GPIO_GPIOF_INT_TRI_R0_GPIOF3_2  0x2
#define GPIO_GPIOF_INT_TRI_R0_GPIOF3_3  0x3
#define GPIO_GPIOF_INT_TRI_R0_GPIOF3_4  0x4
#define GPIO_GPIOF_INT_TRI_R0_GPIOF4_0  0x0
#define GPIO_GPIOF_INT_TRI_R0_GPIOF4_1  0x1
#define GPIO_GPIOF_INT_TRI_R0_GPIOF4_2  0x2
#define GPIO_GPIOF_INT_TRI_R0_GPIOF4_3  0x3
#define GPIO_GPIOF_INT_TRI_R0_GPIOF4_4  0x4
#define GPIO_GPIOF_INT_TRI_R0_GPIOF5_0  0x0
#define GPIO_GPIOF_INT_TRI_R0_GPIOF5_1  0x1
#define GPIO_GPIOF_INT_TRI_R0_GPIOF5_2  0x2
#define GPIO_GPIOF_INT_TRI_R0_GPIOF5_3  0x3
#define GPIO_GPIOF_INT_TRI_R0_GPIOF5_4  0x4
#define GPIO_GPIOF_INT_TRI_R0_GPIOF6_0  0x0
#define GPIO_GPIOF_INT_TRI_R0_GPIOF6_1  0x1
#define GPIO_GPIOF_INT_TRI_R0_GPIOF6_2  0x2
#define GPIO_GPIOF_INT_TRI_R0_GPIOF6_3  0x3
#define GPIO_GPIOF_INT_TRI_R0_GPIOF6_4  0x4
#define GPIO_GPIOF_INT_TRI_R0_GPIOF7_0  0x0
#define GPIO_GPIOF_INT_TRI_R0_GPIOF7_1  0x1
#define GPIO_GPIOF_INT_TRI_R0_GPIOF7_2  0x2
#define GPIO_GPIOF_INT_TRI_R0_GPIOF7_3  0x3
#define GPIO_GPIOF_INT_TRI_R0_GPIOF7_4  0x4

/* GPIOF External Interrupt Trigger Configuration Register1 */
typedef union {
	u32 val;
	struct {
	u32 gpiof8:3;          /* */
	u32 rsvd0:1;           /* */
	u32 gpiof9:3;          /* */
	u32 rsvd1:25;          /* */
	} bits;
} reg_gpio_gpiof_int_tri_r1_t;

#define GPIO_GPIOF_INT_TRI_R1_GPIOF8_0  0x0
#define GPIO_GPIOF_INT_TRI_R1_GPIOF8_1  0x1
#define GPIO_GPIOF_INT_TRI_R1_GPIOF8_2  0x2
#define GPIO_GPIOF_INT_TRI_R1_GPIOF8_3  0x3
#define GPIO_GPIOF_INT_TRI_R1_GPIOF8_4  0x4
#define GPIO_GPIOF_INT_TRI_R1_GPIOF9_0  0x0
#define GPIO_GPIOF_INT_TRI_R1_GPIOF9_1  0x1
#define GPIO_GPIOF_INT_TRI_R1_GPIOF9_2  0x2
#define GPIO_GPIOF_INT_TRI_R1_GPIOF9_3  0x3
#define GPIO_GPIOF_INT_TRI_R1_GPIOF9_4  0x4

/* GPIOF External Interrupt Mask Register */
typedef union {
	u32 val;
	struct {
	u32 mask_en:10;        /* */
	u32 rsvd0:22;          /* */
	} bits;
} reg_gpio_gpiof_int_mask_t;

#define GPIO_GPIOF_INT_MASK_MASK_EN_0   0x0
#define GPIO_GPIOF_INT_MASK_MASK_EN_1   0x1

/* GPIOF External Interrupt Pending Register */
typedef union {
	u32 val;
	struct {
	u32 pending:10;        /* */
	u32 rsvd0:22;          /* */
	} bits;
} reg_gpio_gpiof_int_pend_t;

#define GPIO_GPIOF_INT_PEND_PENDING_0   0x0
#define GPIO_GPIOF_INT_PEND_PENDING_1   0x1

/* GPIOF External Interrupt Sample Time Register */
typedef union {
	u32 val;
	struct {
	u32 src_sel:2;         /* */
	u32 rsvd0:2;           /* */
	u32 div:4;             /* */
	u32 rsvd1:23;          /* */
	u32 en:1;              /* */
	} bits;
} reg_gpio_gpiof_int_samp_ctrl_t;

#define GPIO_GPIOF_INT_SAMP_CTRL_SRC_SEL_0      0x0
#define GPIO_GPIOF_INT_SAMP_CTRL_SRC_SEL_1      0x1
#define GPIO_GPIOF_INT_SAMP_CTRL_DIV_0          0x0
#define GPIO_GPIOF_INT_SAMP_CTRL_DIV_1          0x1
#define GPIO_GPIOF_INT_SAMP_CTRL_DIV_2          0x2
#define GPIO_GPIOF_INT_SAMP_CTRL_DIV_3          0x3
#define GPIO_GPIOF_INT_SAMP_CTRL_DIV_4          0x4
#define GPIO_GPIOF_INT_SAMP_CTRL_DIV_5          0x5
#define GPIO_GPIOF_INT_SAMP_CTRL_DIV_6          0x6
#define GPIO_GPIOF_INT_SAMP_CTRL_DIV_7          0x7
#define GPIO_GPIOF_INT_SAMP_CTRL_DIV_8          0x8
#define GPIO_GPIOF_INT_SAMP_CTRL_DIV_9          0x9
#define GPIO_GPIOF_INT_SAMP_CTRL_DIV_A          0xA
#define GPIO_GPIOF_INT_SAMP_CTRL_EN_0           0x0
#define GPIO_GPIOF_INT_SAMP_CTRL_EN_1           0x1

/* GPIOG External Interrupt Trigger Configuration Register0 */
typedef union {
	u32 val;
	struct {
	u32 gpiog0:3;          /* */
	u32 rsvd0:1;           /* */
	u32 gpiog1:3;          /* */
	u32 rsvd1:1;           /* */
	u32 gpiog2:3;          /* */
	u32 rsvd2:1;           /* */
	u32 gpiog3:3;          /* */
	u32 rsvd3:1;           /* */
	u32 gpiog4:3;          /* */
	u32 rsvd4:1;           /* */
	u32 gpiog5:3;          /* */
	u32 rsvd5:1;           /* */
	u32 gpiog6:3;          /* */
	u32 rsvd6:1;           /* */
	u32 gpiog7:3;          /* */
	u32 rsvd7:1;           /* */
	} bits;
} reg_gpio_gpiog_int_tri_r0_t;

#define GPIO_GPIOG_INT_TRI_R0_GPIOG0_0  0x0
#define GPIO_GPIOG_INT_TRI_R0_GPIOG0_1  0x1
#define GPIO_GPIOG_INT_TRI_R0_GPIOG0_2  0x2
#define GPIO_GPIOG_INT_TRI_R0_GPIOG0_3  0x3
#define GPIO_GPIOG_INT_TRI_R0_GPIOG0_4  0x4
#define GPIO_GPIOG_INT_TRI_R0_GPIOG1_0  0x0
#define GPIO_GPIOG_INT_TRI_R0_GPIOG1_1  0x1
#define GPIO_GPIOG_INT_TRI_R0_GPIOG1_2  0x2
#define GPIO_GPIOG_INT_TRI_R0_GPIOG1_3  0x3
#define GPIO_GPIOG_INT_TRI_R0_GPIOG1_4  0x4
#define GPIO_GPIOG_INT_TRI_R0_GPIOG2_0  0x0
#define GPIO_GPIOG_INT_TRI_R0_GPIOG2_1  0x1
#define GPIO_GPIOG_INT_TRI_R0_GPIOG2_2  0x2
#define GPIO_GPIOG_INT_TRI_R0_GPIOG2_3  0x3
#define GPIO_GPIOG_INT_TRI_R0_GPIOG2_4  0x4
#define GPIO_GPIOG_INT_TRI_R0_GPIOG3_0  0x0
#define GPIO_GPIOG_INT_TRI_R0_GPIOG3_1  0x1
#define GPIO_GPIOG_INT_TRI_R0_GPIOG3_2  0x2
#define GPIO_GPIOG_INT_TRI_R0_GPIOG3_3  0x3
#define GPIO_GPIOG_INT_TRI_R0_GPIOG3_4  0x4
#define GPIO_GPIOG_INT_TRI_R0_GPIOG4_0  0x0
#define GPIO_GPIOG_INT_TRI_R0_GPIOG4_1  0x1
#define GPIO_GPIOG_INT_TRI_R0_GPIOG4_2  0x2
#define GPIO_GPIOG_INT_TRI_R0_GPIOG4_3  0x3
#define GPIO_GPIOG_INT_TRI_R0_GPIOG4_4  0x4
#define GPIO_GPIOG_INT_TRI_R0_GPIOG5_0  0x0
#define GPIO_GPIOG_INT_TRI_R0_GPIOG5_1  0x1
#define GPIO_GPIOG_INT_TRI_R0_GPIOG5_2  0x2
#define GPIO_GPIOG_INT_TRI_R0_GPIOG5_3  0x3
#define GPIO_GPIOG_INT_TRI_R0_GPIOG5_4  0x4
#define GPIO_GPIOG_INT_TRI_R0_GPIOG6_0  0x0
#define GPIO_GPIOG_INT_TRI_R0_GPIOG6_1  0x1
#define GPIO_GPIOG_INT_TRI_R0_GPIOG6_2  0x2
#define GPIO_GPIOG_INT_TRI_R0_GPIOG6_3  0x3
#define GPIO_GPIOG_INT_TRI_R0_GPIOG6_4  0x4
#define GPIO_GPIOG_INT_TRI_R0_GPIOG7_0  0x0
#define GPIO_GPIOG_INT_TRI_R0_GPIOG7_1  0x1
#define GPIO_GPIOG_INT_TRI_R0_GPIOG7_2  0x2
#define GPIO_GPIOG_INT_TRI_R0_GPIOG7_3  0x3
#define GPIO_GPIOG_INT_TRI_R0_GPIOG7_4  0x4

/* GPIOG External Interrupt Trigger Configuration Register1 */
typedef union {
	u32 val;
	struct {
	u32 gpiog8:3;          /* */
	u32 rsvd0:1;           /* */
	u32 gpiog9:3;          /* */
	u32 rsvd1:25;          /* */
	} bits;
} reg_gpio_gpiog_int_tri_r1_t;

#define GPIO_GPIOG_INT_TRI_R1_GPIOG8_0  0x0
#define GPIO_GPIOG_INT_TRI_R1_GPIOG8_1  0x1
#define GPIO_GPIOG_INT_TRI_R1_GPIOG8_2  0x2
#define GPIO_GPIOG_INT_TRI_R1_GPIOG8_3  0x3
#define GPIO_GPIOG_INT_TRI_R1_GPIOG8_4  0x4
#define GPIO_GPIOG_INT_TRI_R1_GPIOG9_0  0x0
#define GPIO_GPIOG_INT_TRI_R1_GPIOG9_1  0x1
#define GPIO_GPIOG_INT_TRI_R1_GPIOG9_2  0x2
#define GPIO_GPIOG_INT_TRI_R1_GPIOG9_3  0x3
#define GPIO_GPIOG_INT_TRI_R1_GPIOG9_4  0x4

/* GPIOG External Interrupt Mask Register */
typedef union {
	u32 val;
	struct {
	u32 mask_en:10;        /* */
	u32 rsvd0:22;          /* */
	} bits;
} reg_gpio_gpiog_int_mask_t;

#define GPIO_GPIOG_INT_MASK_MASK_EN_0   0x0
#define GPIO_GPIOG_INT_MASK_MASK_EN_1   0x1

/* GPIOG External Interrupt Pending Register */
typedef union {
	u32 val;
	struct {
	u32 pending:10;        /* */
	u32 rsvd0:22;          /* */
	} bits;
} reg_gpio_gpiog_int_pend_t;

#define GPIO_GPIOG_INT_PEND_PENDING_0   0x0
#define GPIO_GPIOG_INT_PEND_PENDING_1   0x1

/* GPIOG External Interrupt Sample Time Register */
typedef union {
	u32 val;
	struct {
	u32 src_sel:2;         /* */
	u32 rsvd0:2;           /* */
	u32 div:4;             /* */
	u32 rsvd1:23;          /* */
	u32 en:1;              /* */
	} bits;
} reg_gpio_gpiog_int_samp_ctrl_t;

#define GPIO_GPIOG_INT_SAMP_CTRL_SRC_SEL_0      0x0
#define GPIO_GPIOG_INT_SAMP_CTRL_SRC_SEL_1      0x1
#define GPIO_GPIOG_INT_SAMP_CTRL_DIV_0          0x0
#define GPIO_GPIOG_INT_SAMP_CTRL_DIV_1          0x1
#define GPIO_GPIOG_INT_SAMP_CTRL_DIV_2          0x2
#define GPIO_GPIOG_INT_SAMP_CTRL_DIV_3          0x3
#define GPIO_GPIOG_INT_SAMP_CTRL_DIV_4          0x4
#define GPIO_GPIOG_INT_SAMP_CTRL_DIV_5          0x5
#define GPIO_GPIOG_INT_SAMP_CTRL_DIV_6          0x6
#define GPIO_GPIOG_INT_SAMP_CTRL_DIV_7          0x7
#define GPIO_GPIOG_INT_SAMP_CTRL_DIV_8          0x8
#define GPIO_GPIOG_INT_SAMP_CTRL_DIV_9          0x9
#define GPIO_GPIOG_INT_SAMP_CTRL_DIV_A          0xA
#define GPIO_GPIOG_INT_SAMP_CTRL_EN_0           0x0
#define GPIO_GPIOG_INT_SAMP_CTRL_EN_1           0x1

/* reg_gpio_t bank */
typedef struct tag_gpio {
	reg_gpio_gpioc_ver_t               gpioc_ver;          /* 0000 */
	u32                                res0[7];
	reg_gpio_gpioa_func_r0_t           gpioa_func_r0;      /* 0020 */
	reg_gpio_gpioa_func_r1_t           gpioa_func_r1;      /* 0024 */
	u32                                res1[2];
	reg_gpio_gpioa_pud_r0_t            gpioa_pud_r0;       /* 0030 */
	reg_gpio_gpioa_pud_r1_t            gpioa_pud_r1;       /* 0034 */
	u32                                res2[2];
	reg_gpio_gpioa_drv_r0_t            gpioa_drv_r0;       /* 0040 */
	reg_gpio_gpioa_drv_r1_t            gpioa_drv_r1;       /* 0044 */
	u32                                res3[2];
	reg_gpio_gpioa_data_t              gpioa_data;         /* 0050 */
	reg_gpio_gpioa_pr_r_t              gpioa_pr_r;         /* 0054 */
	u32                                res4[2];
	reg_gpio_gpiob_func_r0_t           gpiob_func_r0;      /* 0060 */
	reg_gpio_gpiob_func_r1_t           gpiob_func_r1;      /* 0064 */
	reg_gpio_gpiob_func_r2_t           gpiob_func_r2;      /* 0068 */
	reg_gpio_gpiob_func_r3_t           gpiob_func_r3;      /* 006C */
	reg_gpio_gpiob_pud_r0_t            gpiob_pud_r0;       /* 0070 */
	reg_gpio_gpiob_pud_r1_t            gpiob_pud_r1;       /* 0074 */
	reg_gpio_gpiob_pud_r2_t            gpiob_pud_r2;       /* 0078 */
	reg_gpio_gpiob_pud_r3_t            gpiob_pud_r3;       /* 007C */
	reg_gpio_gpiob_drv_r0_t            gpiob_drv_r0;       /* 0080 */
	reg_gpio_gpiob_drv_r1_t            gpiob_drv_r1;       /* 0084 */
	reg_gpio_gpiob_drv_r2_t            gpiob_drv_r2;       /* 0088 */
	reg_gpio_gpiob_drv_r3_t            gpiob_drv_r3;       /* 008C */
	reg_gpio_gpiob_data_t              gpiob_data;         /* 0090 */
	reg_gpio_gpiob_pr_r_t              gpiob_pr_r;         /* 0094 */
	u32                                res5[2];
	reg_gpio_gpioc_func_r0_t           gpioc_func_r0;      /* 00A0 */
	reg_gpio_gpioc_func_r1_t           gpioc_func_r1;      /* 00A4 */
	reg_gpio_gpioc_func_r2_t           gpioc_func_r2;      /* 00A8 */
	u32                                res6[1];
	reg_gpio_gpioc_pud_r0_t            gpioc_pud_r0;       /* 00B0 */
	reg_gpio_gpioc_pud_r1_t            gpioc_pud_r1;       /* 00B4 */
	reg_gpio_gpioc_pud_r2_t            gpioc_pud_r2;       /* 00B8 */
	u32                                res7[1];
	reg_gpio_gpioc_drv_r0_t            gpioc_drv_r0;       /* 00C0 */
	reg_gpio_gpioc_drv_r1_t            gpioc_drv_r1;       /* 00C4 */
	reg_gpio_gpioc_drv_r2_t            gpioc_drv_r2;       /* 00C8 */
	u32                                res8[1];
	reg_gpio_gpioc_data_t              gpioc_data;         /* 00D0 */
	reg_gpio_gpioc_pr_r_t              gpioc_pr_r;         /* 00D4 */
	u32                                res9[2];
	reg_gpio_gpiod_func_r0_t           gpiod_func_r0;      /* 00E0 */
	u32                                res10[3];
	reg_gpio_gpiod_pud_r0_t            gpiod_pud_r0;       /* 00F0 */
	u32                                res11[3];
	reg_gpio_gpiod_drv_r0_t            gpiod_drv_r0;       /* 0100 */
	u32                                res12[3];
	reg_gpio_gpiod_data_t              gpiod_data;         /* 0110 */
	reg_gpio_gpiod_pr_r_t              gpiod_pr_r;         /* 0114 */
	u32                                res13[2];
	reg_gpio_gpioe_func_r0_t           gpioe_func_r0;      /* 0120 */
	reg_gpio_gpioe_func_r1_t           gpioe_func_r1;      /* 0124 */
	u32                                res14[2];
	reg_gpio_gpioe_pud_r0_t            gpioe_pud_r0;       /* 0130 */
	reg_gpio_gpioe_pud_r1_t            gpioe_pud_r1;       /* 0134 */
	u32                                res15[2];
	reg_gpio_gpioe_drv_r0_t            gpioe_drv_r0;       /* 0140 */
	reg_gpio_gpioe_drv_r1_t            gpioe_drv_r1;       /* 0144 */
	u32                                res16[2];
	reg_gpio_gpioe_data_t              gpioe_data;         /* 0150 */
	reg_gpio_gpioe_pr_r_t              gpioe_pr_r;         /* 0154 */
	u32                                res17[2];
	reg_gpio_gpiof_func_r0_t           gpiof_func_r0;      /* 0160 */
	reg_gpio_gpiof_func_r1_t           gpiof_func_r1;      /* 0164 */
	u32                                res18[2];
	reg_gpio_gpiof_pud_r0_t            gpiof_pud_r0;       /* 0170 */
	reg_gpio_gpiof_pud_r1_t            gpiof_pud_r1;       /* 0174 */
	u32                                res19[2];
	reg_gpio_gpiof_drv_r0_t            gpiof_drv_r0;       /* 0180 */
	reg_gpio_gpiof_drv_r1_t            gpiof_drv_r1;       /* 0184 */
	u32                                res20[2];
	reg_gpio_gpiof_data_t              gpiof_data;         /* 0190 */
	reg_gpio_gpiof_pr_r_t              gpiof_pr_r;         /* 0194 */
	u32                                res21[2];
	reg_gpio_gpiog_func_r0_t           gpiog_func_r0;      /* 01A0 */
	reg_gpio_gpiog_func_r1_t           gpiog_func_r1;      /* 01A4 */
	u32                                res22[2];
	reg_gpio_gpiog_pud_r0_t            gpiog_pud_r0;       /* 01B0 */
	reg_gpio_gpiog_pud_r1_t            gpiog_pud_r1;       /* 01B4 */
	u32                                res23[2];
	reg_gpio_gpiog_drv_r0_t            gpiog_drv_r0;       /* 01C0 */
	reg_gpio_gpiog_drv_r1_t            gpiog_drv_r1;       /* 01C4 */
	u32                                res24[2];
	reg_gpio_gpiog_data_t              gpiog_data;         /* 01D0 */
	reg_gpio_gpiog_pr_r_t              gpiog_pr_r;         /* 01D4 */
	u32                                res25[18];
	reg_gpio_gpioj_func_r0_t           gpioj_func_r0;      /* 0220 */
	u32                                res26[3];
	reg_gpio_gpioj_pud_r0_t            gpioj_pud_r0;       /* 0230 */
	u32                                res27[3];
	reg_gpio_gpioj_drv_r0_t            gpioj_drv_r0;       /* 0240 */
	u32                                res28[3];
	reg_gpio_gpioj_data_t              gpioj_data;         /* 0250 */
	reg_gpio_gpioj_pr_r_t              gpioj_pr_r;         /* 0254 */
	u32                                res29[194];
	reg_gpio_gpiob_int_tri_r0_t        gpiob_int_tri_r0;   /* 0560 */
	reg_gpio_gpiob_int_tri_r1_t        gpiob_int_tri_r1;   /* 0564 */
	reg_gpio_gpiob_int_tri_r2_t        gpiob_int_tri_r2;   /* 0568 */
	reg_gpio_gpiob_int_tri_r3_t        gpiob_int_tri_r3;   /* 056C */
	reg_gpio_gpiob_int_mask_t          gpiob_int_mask;     /* 0570 */
	reg_gpio_gpiob_int_pend_t          gpiob_int_pend;     /* 0574 */
	reg_gpio_gpiob_int_samp_ctrl_t     gpiob_int_samp_ctrl;/* 0578 */
	u32                                res30[41];
	reg_gpio_gpioe_int_tri_r0_t        gpioe_int_tri_r0;   /* 0620 */
	reg_gpio_gpioe_int_tri_r1_t        gpioe_int_tri_r1;   /* 0624 */
	u32                                res31[2];
	reg_gpio_gpioe_int_mask_t          gpioe_int_mask;     /* 0630 */
	reg_gpio_gpioe_int_pend_t          gpioe_int_pend;     /* 0634 */
	reg_gpio_gpioe_int_samp_ctrl_t     gpioe_int_samp_ctrl;/* 0638 */
	u32                                res32[9];
	reg_gpio_gpiof_int_tri_r0_t        gpiof_int_tri_r0;   /* 0660 */
	reg_gpio_gpiof_int_tri_r1_t        gpiof_int_tri_r1;   /* 0664 */
	u32                                res33[2];
	reg_gpio_gpiof_int_mask_t          gpiof_int_mask;     /* 0670 */
	reg_gpio_gpiof_int_pend_t          gpiof_int_pend;     /* 0674 */
	reg_gpio_gpiof_int_samp_ctrl_t     gpiof_int_samp_ctrl;/* 0678 */
	u32                                res34[9];
	reg_gpio_gpiog_int_tri_r0_t        gpiog_int_tri_r0;   /* 06A0 */
	reg_gpio_gpiog_int_tri_r1_t        gpiog_int_tri_r1;   /* 06A4 */
	u32                                res35[2];
	reg_gpio_gpiog_int_mask_t          gpiog_int_mask;     /* 06B0 */
	reg_gpio_gpiog_int_pend_t          gpiog_int_pend;     /* 06B4 */
	reg_gpio_gpiog_int_samp_ctrl_t     gpiog_int_samp_ctrl;/* 06B8 */
} reg_gpio_t;

#endif /* ___GPIO___H___ */
