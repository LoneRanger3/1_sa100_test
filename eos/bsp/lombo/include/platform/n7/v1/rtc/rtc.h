/* rtc.h */

#ifndef ___RTC___H___
#define ___RTC___H___

#define BASE_RTC                0x0400B000

#define VA_RTC_VER                           (0x00000000 + BASE_RTC + VA_RTC)
#define VA_RTC_RTC_CLK_CTRL                  (0x00000020 + BASE_RTC + VA_RTC)
#define VA_RTC_RTC_LD_EN                     (0x00000024 + BASE_RTC + VA_RTC)
#define VA_RTC_RTC_LD_INT_EN                 (0x00000028 + BASE_RTC + VA_RTC)
#define VA_RTC_RTC_LD_INT_CLR                (0x0000002C + BASE_RTC + VA_RTC)
#define VA_RTC_RTC_LD_INT_PENDING            (0x00000030 + BASE_RTC + VA_RTC)
#define VA_RTC_RTC_YMD                       (0x00000040 + BASE_RTC + VA_RTC)
#define VA_RTC_RTC_WEEK                      (0x00000044 + BASE_RTC + VA_RTC)
#define VA_RTC_RTC_HMS                       (0x00000048 + BASE_RTC + VA_RTC)
#define VA_RTC_RTC_ALARM_INT_EN              (0x0000004C + BASE_RTC + VA_RTC)
#define VA_RTC_RTC_ALARM_INT_CLR             (0x00000050 + BASE_RTC + VA_RTC)
#define VA_RTC_RTC_ALARM_INT_PENDING         (0x00000054 + BASE_RTC + VA_RTC)
#define VA_RTC_RTC_ALARM_MATCH_EN            (0x00000058 + BASE_RTC + VA_RTC)
#define VA_RTC_RTC_ALARM_DAY_MATCH           (0x0000005C + BASE_RTC + VA_RTC)
#define VA_RTC_RTC_ALARM_WEEK_MATCH          (0x00000060 + BASE_RTC + VA_RTC)
#define VA_RTC_RTC_ALARM_HMS_MATCH           (0x00000064 + BASE_RTC + VA_RTC)
#define VA_RTC_RTC_WAKEUP_EN                 (0x00000068 + BASE_RTC + VA_RTC)
#define VA_RTC_RTC_LFEOSC_FANOUT_CFG         (0x0000006C + BASE_RTC + VA_RTC)
#define VA_RTC_RTC_SU_EN                     (0x00000070 + BASE_RTC + VA_RTC)
#define VA_RTC_RTC_CORE                      (0x00000400 + BASE_RTC + VA_RTC)
#define VA_RTC_RTC_TOP                       (0x00000404 + BASE_RTC + VA_RTC)
#define VA_RTC_RTC_HFEOSC                    (0x00000408 + BASE_RTC + VA_RTC)
#define VA_RTC_RTC_LFEOSC                    (0x0000040C + BASE_RTC + VA_RTC)
#define VA_RTC_RTC_ALIVE_REG0                (0x00000420 + BASE_RTC + VA_RTC)
#define VA_RTC_RTC_ALIVE_REG1                (0x00000424 + BASE_RTC + VA_RTC)
#define VA_RTC_RTC_ALIVE_REG2                (0x00000428 + BASE_RTC + VA_RTC)
#define VA_RTC_RTC_ALIVE_REG3                (0x0000042C + BASE_RTC + VA_RTC)
#define VA_RTC_PM_PE                         (0x00000460 + BASE_RTC + VA_RTC)
#define VA_RTC_PM_CFG                        (0x00000464 + BASE_RTC + VA_RTC)
#define VA_RTC_PM_INT_EN                     (0x00000468 + BASE_RTC + VA_RTC)
#define VA_RTC_PM_PEND_CLR                   (0x0000046C + BASE_RTC + VA_RTC)
#define VA_RTC_PM_PEND                       (0x00000470 + BASE_RTC + VA_RTC)
#define VA_RTC_PM_PE1                        (0x00000474 + BASE_RTC + VA_RTC)
#define VA_RTC_LOCK_CTRL                     (0x00000700 + BASE_RTC + VA_RTC)
#define VA_RTC_DEEP_SLP_FLAG                 (0x00000704 + BASE_RTC + VA_RTC)
#define VA_RTC_DEEP_SLP_EXIT                 (0x00000708 + BASE_RTC + VA_RTC)
#define VA_RTC_HOT_RST_FLAG                  (0x00000710 + BASE_RTC + VA_RTC)
#define VA_RTC_HOT_RST_EXIT                  (0x00000714 + BASE_RTC + VA_RTC)
#define VA_RTC_PAD_HOLD                      (0x00000720 + BASE_RTC + VA_RTC)
#define VA_RTC_SYS_STAT                      (0x00000728 + BASE_RTC + VA_RTC)
#define VA_RTC_AX_CLAMP                      (0x00000760 + BASE_RTC + VA_RTC)
#define VA_RTC_SIO_FUNC_R0                   (0x00000820 + BASE_RTC + VA_RTC)
#define VA_RTC_SIO_PUD_R0                    (0x00000830 + BASE_RTC + VA_RTC)
#define VA_RTC_SIO_DRV_R0                    (0x00000840 + BASE_RTC + VA_RTC)
#define VA_RTC_SIO_DATA_R0                   (0x00000850 + BASE_RTC + VA_RTC)
#define VA_RTC_SIO_PR_R                      (0x00000854 + BASE_RTC + VA_RTC)
#define VA_RTC_SIO_INT_TRI_R0                (0x00000860 + BASE_RTC + VA_RTC)
#define VA_RTC_SIO_INT_MASK0                 (0x00000870 + BASE_RTC + VA_RTC)
#define VA_RTC_SIO_INT_PEND0                 (0x00000874 + BASE_RTC + VA_RTC)
#define VA_RTC_SIO_INT_SAMP_CTRL0            (0x00000878 + BASE_RTC + VA_RTC)

#define DATA_RTC_VER                             0x00001100
#define DATA_RTC_RTC_CLK_CTRL                    0x000000F0
#define DATA_RTC_RTC_LD_EN                       0x00000000
#define DATA_RTC_RTC_LD_INT_EN                   0x00000000
#define DATA_RTC_RTC_LD_INT_CLR                  0x00000000
#define DATA_RTC_RTC_LD_INT_PENDING              0x00000000
#define DATA_RTC_RTC_YMD                         0x076C0101
#define DATA_RTC_RTC_WEEK                        0x00000000
#define DATA_RTC_RTC_HMS                         0x00000000
#define DATA_RTC_RTC_ALARM_INT_EN                0x00000000
#define DATA_RTC_RTC_ALARM_INT_CLR               0x00000000
#define DATA_RTC_RTC_ALARM_INT_PENDING           0x00000000
#define DATA_RTC_RTC_ALARM_MATCH_EN              0x00000000
#define DATA_RTC_RTC_ALARM_DAY_MATCH             0x00000001
#define DATA_RTC_RTC_ALARM_WEEK_MATCH            0x00000000
#define DATA_RTC_RTC_ALARM_HMS_MATCH             0x00000000
#define DATA_RTC_RTC_WAKEUP_EN                   0x00000000
#define DATA_RTC_RTC_LFEOSC_FANOUT_CFG           0x00000000
#define DATA_RTC_RTC_SU_EN                       0x00010000
#define DATA_RTC_RTC_CORE                        0x00300922
#define DATA_RTC_RTC_TOP                         0x00000002
#define DATA_RTC_RTC_HFEOSC                      0x00058800
#define DATA_RTC_RTC_LFEOSC                      0x0001881D
#define DATA_RTC_RTC_ALIVE_REG0                  0x00000000
#define DATA_RTC_RTC_ALIVE_REG1                  0x00000000
#define DATA_RTC_RTC_ALIVE_REG2                  0x00000000
#define DATA_RTC_RTC_ALIVE_REG3                  0x00000000
#define DATA_RTC_PM_PE                           0x00000000
#define DATA_RTC_PM_CFG                          0x00000080
#define DATA_RTC_PM_INT_EN                       0x00000000
#define DATA_RTC_PM_PEND_CLR                     0x00000000
#define DATA_RTC_PM_PEND                         0x00000000
#define DATA_RTC_PM_PE1                          0x00000000
#define DATA_RTC_LOCK_CTRL                       0x00000000
#define DATA_RTC_DEEP_SLP_FLAG                   0x00000000
#define DATA_RTC_DEEP_SLP_EXIT                   0x00000000
#define DATA_RTC_HOT_RST_FLAG                    0x00000000
#define DATA_RTC_HOT_RST_EXIT                    0x00000000
#define DATA_RTC_PAD_HOLD                        0x00000000
#define DATA_RTC_SYS_STAT                        0x00000000
#define DATA_RTC_AX_CLAMP                        0x00000000
#define DATA_RTC_SIO_FUNC_R0                     0x00030333
#define DATA_RTC_SIO_PUD_R0                      0x00000000
#define DATA_RTC_SIO_DRV_R0                      0x33333333
#define DATA_RTC_SIO_DATA_R0                     0x00000000
#define DATA_RTC_SIO_PR_R                        0x00000000
#define DATA_RTC_SIO_INT_TRI_R0                  0x00000000
#define DATA_RTC_SIO_INT_MASK0                   0x00000000
#define DATA_RTC_SIO_INT_PEND0                   0x00000000
#define DATA_RTC_SIO_INT_SAMP_CTRL0              0x00000000

/* RTC Version Register */
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
} reg_rtc_ver_t;

#define RTC_VER_COMP_1  0x1

/* RTC Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 sel:1;             /* */
	u32 stat:1;            /* */
	u32 rsvd0:2;           /* */
	u32 rcosc_div:5;       /* */
	u32 rsvd1:23;          /* */
	} bits;
} reg_rtc_rtc_clk_ctrl_t;

#define RTC_RTC_CLK_CTRL_SEL_0          0x0
#define RTC_RTC_CLK_CTRL_SEL_1          0x1
#define RTC_RTC_CLK_CTRL_STAT_0         0x0
#define RTC_RTC_CLK_CTRL_STAT_1         0x1

/* RTC LFEOSC Detect Enable Register */
typedef union {
	u32 val;
	struct {
	u32 en:1;              /* */
	u32 rsvd0:31;          /* */
	} bits;
} reg_rtc_rtc_ld_en_t;

#define RTC_RTC_LD_EN_EN_0      0x0
#define RTC_RTC_LD_EN_EN_1      0x1

/* RTC LFEOSC Detect Interrupt Enable Register */
typedef union {
	u32 val;
	struct {
	u32 en:1;              /* */
	u32 rsvd0:31;          /* */
	} bits;
} reg_rtc_rtc_ld_int_en_t;

#define RTC_RTC_LD_INT_EN_EN_0          0x0
#define RTC_RTC_LD_INT_EN_EN_1          0x1

/* RTC LFEOSC Detect Clear Interrupt Pending Register */
typedef union {
	u32 val;
	struct {
	u32 clr:1;             /* */
	u32 rsvd0:31;          /* */
	} bits;
} reg_rtc_rtc_ld_int_clr_t;

#define RTC_RTC_LD_INT_CLR_CLR_0        0x0
#define RTC_RTC_LD_INT_CLR_CLR_1        0x1

/* RTC LFEOSC Detect Interrupt Pending Register */
typedef union {
	u32 val;
	struct {
	u32 pending:1;         /* */
	u32 rsvd0:31;          /* */
	} bits;
} reg_rtc_rtc_ld_int_pending_t;

#define RTC_RTC_LD_INT_PENDING_PENDING_0        0x0
#define RTC_RTC_LD_INT_PENDING_PENDING_1        0x1

/* RTC Year-Month-Day Register */
typedef union {
	u32 val;
	struct {
	u32 day:5;             /* */
	u32 rsvd0:3;           /* */
	u32 month:4;           /* */
	u32 rsvd1:4;           /* */
	u32 year:12;           /* */
	u32 rsvd2:4;           /* */
	} bits;
} reg_rtc_rtc_ymd_t;

/* RTC Week Register */
typedef union {
	u32 val;
	struct {
	u32 week:3;            /* */
	u32 rsvd0:29;          /* */
	} bits;
} reg_rtc_rtc_week_t;

/* RTC Hour-Minute-Second Register */
typedef union {
	u32 val;
	struct {
	u32 second:6;          /* */
	u32 rsvd0:2;           /* */
	u32 minute:6;          /* */
	u32 rsvd1:2;           /* */
	u32 hour:5;            /* */
	u32 rsvd2:11;          /* */
	} bits;
} reg_rtc_rtc_hms_t;

/* RTC Alarm Interrupt Enable Register */
typedef union {
	u32 val;
	struct {
	u32 en:1;              /* */
	u32 rsvd0:31;          /* */
	} bits;
} reg_rtc_rtc_alarm_int_en_t;

/* RTC Alarm Clear Interrupt Status Register */
typedef union {
	u32 val;
	struct {
	u32 clr:1;             /* */
	u32 rsvd0:31;          /* */
	} bits;
} reg_rtc_rtc_alarm_int_clr_t;

/* RTC Alarm Interrupt Pending Register */
typedef union {
	u32 val;
	struct {
	u32 pending:1;         /* */
	u32 rsvd0:31;          /* */
	} bits;
} reg_rtc_rtc_alarm_int_pending_t;

/* RTC Alarm Match Enable Register */
typedef union {
	u32 val;
	struct {
	u32 sec_alarm_match_en:1;              /* */
	u32 min_alarm_match_en:1;              /* */
	u32 hour_alarm_match_en:1;             /* */
	u32 week_alarm_match_en:1;             /* */
	u32 day_alarm_match_en:1;              /* */
	u32 rsvd0:27;                          /* */
	} bits;
} reg_rtc_rtc_alarm_match_en_t;

/* RTC Alarm Day Match Register */
typedef union {
	u32 val;
	struct {
	u32 day_match:5;       /* */
	u32 rsvd0:27;          /* */
	} bits;
} reg_rtc_rtc_alarm_day_match_t;

/* RTC Alarm Week Match Register */
typedef union {
	u32 val;
	struct {
	u32 week_match:3;              /* */
	u32 rsvd0:29;                  /* */
	} bits;
} reg_rtc_rtc_alarm_week_match_t;

/* RTC Alarm Hour-Minute-Second Match Register */
typedef union {
	u32 val;
	struct {
	u32 second_match:6;            /* */
	u32 rsvd0:2;                   /* */
	u32 minute_match:6;            /* */
	u32 rsvd1:2;                   /* */
	u32 hour_match:5;              /* */
	u32 rsvd2:11;                  /* */
	} bits;
} reg_rtc_rtc_alarm_hms_match_t;

/* RTC Wakeup Enable Register */
typedef union {
	u32 val;
	struct {
	u32 wakeup_en:1;       /* */
	u32 rsvd0:31;          /* */
	} bits;
} reg_rtc_rtc_wakeup_en_t;

/* RTC LFEOSC Fanout Config Register */
typedef union {
	u32 val;
	struct {
	u32 func:2;                    /* */
	u32 rsvd0:2;                   /* */
	u32 dat:1;                     /* */
	u32 rsvd1:26;                  /* */
	u32 lfeosc_fanout_en:1;        /* */
	} bits;
} reg_rtc_rtc_lfeosc_fanout_cfg_t;

#define RTC_RTC_LFEOSC_FANOUT_CFG_FUNC_0                0x0
#define RTC_RTC_LFEOSC_FANOUT_CFG_FUNC_1                0x1
#define RTC_RTC_LFEOSC_FANOUT_CFG_FUNC_2                0x2

/* RTC Speed Up Enable Register */
typedef union {
	u32 val;
	struct {
	u32 en:1;              /* */
	u32 rsvd0:15;          /* */
	u32 rate:16;           /* */
	} bits;
} reg_rtc_rtc_su_en_t;

/* RTC Core Control Register */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:1;           /* */
	u32 osc1_en:1;         /* */
	u32 vref_sel:2;        /* */
	u32 ldo_sel:3;         /* */
	u32 rsvd1:1;           /* */
	u32 div_sel:3;         /* */
	u32 div_en:1;          /* */
	u32 tsd:3;             /* */
	u32 rsvd2:1;           /* */
	u32 tsa:2;             /* */
	u32 rsvd3:2;           /* */
	u32 bg1_en:1;          /* */
	u32 bg0_en:1;          /* */
	u32 rsvd4:10;          /* */
	} bits;
} reg_rtc_rtc_core_t;

#define RTC_RTC_CORE_OSC1_EN_0  0x0
#define RTC_RTC_CORE_OSC1_EN_1  0x1
#define RTC_RTC_CORE_VREF_SEL_0 0x0
#define RTC_RTC_CORE_VREF_SEL_1 0x1
#define RTC_RTC_CORE_VREF_SEL_2 0x2
#define RTC_RTC_CORE_VREF_SEL_3 0x3
#define RTC_RTC_CORE_LDO_SEL_0  0x0
#define RTC_RTC_CORE_LDO_SEL_7  0x7
#define RTC_RTC_CORE_DIV_SEL_0  0x0
#define RTC_RTC_CORE_DIV_SEL_7  0x7
#define RTC_RTC_CORE_DIV_EN_0   0x0
#define RTC_RTC_CORE_DIV_EN_1   0x1
#define RTC_RTC_CORE_TSD_0      0x0
#define RTC_RTC_CORE_TSD_1      0x1
#define RTC_RTC_CORE_TSD_2      0x2
#define RTC_RTC_CORE_TSD_3      0x3
#define RTC_RTC_CORE_TSD_4      0x4
#define RTC_RTC_CORE_TSD_5      0x5
#define RTC_RTC_CORE_TSD_6      0x6
#define RTC_RTC_CORE_TSD_7      0x7
#define RTC_RTC_CORE_TSA_0      0x0
#define RTC_RTC_CORE_TSA_1      0x1
#define RTC_RTC_CORE_TSA_2      0x2
#define RTC_RTC_CORE_TSA_3      0x3
#define RTC_RTC_CORE_BG1_EN_1   0x1
#define RTC_RTC_CORE_BG0_EN_1   0x1

/* RTC Top Register */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:1;           /* */
	u32 eno:1;             /* */
	u32 ad:1;              /* */
	u32 rsvd1:1;           /* */
	u32 seld:2;            /* */
	u32 rsvd2:26;          /* */
	} bits;
} reg_rtc_rtc_top_t;

#define RTC_RTC_TOP_ENO_0       0x0
#define RTC_RTC_TOP_ENO_1       0x1
#define RTC_RTC_TOP_AD_0        0x0
#define RTC_RTC_TOP_AD_1        0x1
#define RTC_RTC_TOP_SELD_0      0x0
#define RTC_RTC_TOP_SELD_1      0x1
#define RTC_RTC_TOP_SELD_2      0x2
#define RTC_RTC_TOP_SELD_3      0x3

/* RTC HFEOSC Control Register */
typedef union {
	u32 val;
	struct {
	u32 en:1;              /* */
	u32 capen:1;           /* */
	u32 smtsel:1;          /* */
	u32 smten:1;           /* */
	u32 lpfen:1;           /* */
	u32 rsvd0:3;           /* */
	u32 capsel_i:4;        /* */
	u32 capsel_o:4;        /* */
	u32 gmsel:2;           /* */
	u32 rsel:2;            /* */
	u32 rsvd1:12;          /* */
	} bits;
} reg_rtc_rtc_hfeosc_t;

#define RTC_RTC_HFEOSC_EN_1             0x1
#define RTC_RTC_HFEOSC_CAPEN_1          0x1
#define RTC_RTC_HFEOSC_SMTSEL_1         0x1
#define RTC_RTC_HFEOSC_SMTEN_1          0x1
#define RTC_RTC_HFEOSC_LPFEN_1          0x1
#define RTC_RTC_HFEOSC_CAPSEL_I_0       0x0
#define RTC_RTC_HFEOSC_CAPSEL_I_F       0xF
#define RTC_RTC_HFEOSC_CAPSEL_O_0       0x0
#define RTC_RTC_HFEOSC_CAPSEL_O_F       0xF
#define RTC_RTC_HFEOSC_GMSEL_0          0x0
#define RTC_RTC_HFEOSC_GMSEL_1          0x1
#define RTC_RTC_HFEOSC_GMSEL_2          0x2
#define RTC_RTC_HFEOSC_GMSEL_3          0x3
#define RTC_RTC_HFEOSC_RSEL_0           0x0
#define RTC_RTC_HFEOSC_RSEL_1           0x1
#define RTC_RTC_HFEOSC_RSEL_2           0x2
#define RTC_RTC_HFEOSC_RSEL_3           0x3

/* RTC LFEOSC Control Register */
typedef union {
	u32 val;
	struct {
	u32 en:1;              /* */
	u32 capen:1;           /* */
	u32 smtsel:1;          /* */
	u32 smten:1;           /* */
	u32 lpfen:1;           /* */
	u32 rsvd0:3;           /* */
	u32 capsel_i:4;        /* */
	u32 capsel_o:4;        /* */
	u32 gmsel:2;           /* */
	u32 rsvd1:14;          /* */
	} bits;
} reg_rtc_rtc_lfeosc_t;

#define RTC_RTC_LFEOSC_EN_1             0x1
#define RTC_RTC_LFEOSC_CAPEN_1          0x1
#define RTC_RTC_LFEOSC_SMTSEL_1         0x1
#define RTC_RTC_LFEOSC_SMTEN_1          0x1
#define RTC_RTC_LFEOSC_LPFEN_0          0x0
#define RTC_RTC_LFEOSC_LPFEN_1          0x1
#define RTC_RTC_LFEOSC_CAPSEL_I_0       0x0
#define RTC_RTC_LFEOSC_CAPSEL_I_F       0xF
#define RTC_RTC_LFEOSC_CAPSEL_O_0       0x0
#define RTC_RTC_LFEOSC_CAPSEL_O_F       0xF
#define RTC_RTC_LFEOSC_GMSEL_0          0x0
#define RTC_RTC_LFEOSC_GMSEL_1          0x1
#define RTC_RTC_LFEOSC_GMSEL_2          0x2
#define RTC_RTC_LFEOSC_GMSEL_3          0x3

/* RTC Alive Register 0 */
typedef union {
	u32 val;
	struct {
	u32 reg:32;            /* */
	} bits;
} reg_rtc_rtc_alive_reg0_t;

/* RTC Alive Register 1 */
typedef union {
	u32 val;
	struct {
	u32 reg:32;            /* */
	} bits;
} reg_rtc_rtc_alive_reg1_t;

/* RTC Alive Register 2 */
typedef union {
	u32 val;
	struct {
	u32 reg:32;            /* */
	} bits;
} reg_rtc_rtc_alive_reg2_t;

/* RTC Alive Register 3 */
typedef union {
	u32 val;
	struct {
	u32 reg:32;            /* */
	} bits;
} reg_rtc_rtc_alive_reg3_t;

/* Power Manage Power Enable Register */
typedef union {
	u32 val;
	struct {
	u32 en:1;              /* */
	u32 ddr_en:1;          /* */
	u32 rsvd0:30;          /* */
	} bits;
} reg_rtc_pm_pe_t;

#define RTC_PM_PE_EN_0          0x0
#define RTC_PM_PE_EN_1          0x1
#define RTC_PM_PE_DDR_EN_0      0x0
#define RTC_PM_PE_DDR_EN_1      0x1

/* Power Manage PIN Config Register */
typedef union {
	u32 val;
	struct {
	u32 g_int_trig:1;                      /* */
	u32 pir_wake_trig:1;                   /* */
	u32 ring_key_trig:1;                   /* */
	u32 rsvd0:1;                           /* */
	u32 gs_wake_detect_en:1;               /* */
	u32 pir_wake_detect_en:1;              /* */
	u32 ring_key_wake_det_en:1;            /* */
	u32 key_slong_en:1;                    /* */
	u32 rsvd1:24;                          /* */
	} bits;
} reg_rtc_pm_cfg_t;

#define RTC_PM_CFG_G_INT_TRIG_0                 0x0
#define RTC_PM_CFG_G_INT_TRIG_1                 0x1
#define RTC_PM_CFG_PIR_WAKE_TRIG_0              0x0
#define RTC_PM_CFG_PIR_WAKE_TRIG_1              0x1
#define RTC_PM_CFG_RING_KEY_TRIG_0              0x0
#define RTC_PM_CFG_RING_KEY_TRIG_1              0x1
#define RTC_PM_CFG_GS_WAKE_DETECT_EN_0          0x0
#define RTC_PM_CFG_GS_WAKE_DETECT_EN_1          0x1
#define RTC_PM_CFG_PIR_WAKE_DETECT_EN_0         0x0
#define RTC_PM_CFG_PIR_WAKE_DETECT_EN_1         0x1
#define RTC_PM_CFG_RING_KEY_WAKE_DET_EN_0       0x0
#define RTC_PM_CFG_RING_KEY_WAKE_DET_EN_1       0x1
#define RTC_PM_CFG_KEY_SLONG_EN_0               0x0
#define RTC_PM_CFG_KEY_SLONG_EN_1               0x1

/* Power Manage Interrupt Enable Register */
typedef union {
	u32 val;
	struct {
	u32 pwr_en:1;                  /* */
	u32 rsvd0:1;                   /* */
	u32 key_press:1;               /* */
	u32 key_short:1;               /* */
	u32 key_long:1;                /* */
	u32 key_release:1;             /* */
	u32 gs_int:1;                  /* */
	u32 pir_wake:1;                /* */
	u32 ring_key:1;                /* */
	u32 rsvd1:23;                  /* */
	} bits;
} reg_rtc_pm_int_en_t;

#define RTC_PM_INT_EN_PWR_EN_0          0x0
#define RTC_PM_INT_EN_PWR_EN_1          0x1
#define RTC_PM_INT_EN_KEY_PRESS_0       0x0
#define RTC_PM_INT_EN_KEY_PRESS_1       0x1
#define RTC_PM_INT_EN_KEY_SHORT_0       0x0
#define RTC_PM_INT_EN_KEY_SHORT_1       0x1
#define RTC_PM_INT_EN_KEY_LONG_0        0x0
#define RTC_PM_INT_EN_KEY_LONG_1        0x1
#define RTC_PM_INT_EN_KEY_RELEASE_0     0x0
#define RTC_PM_INT_EN_KEY_RELEASE_1     0x1
#define RTC_PM_INT_EN_GS_INT_0          0x0
#define RTC_PM_INT_EN_GS_INT_1          0x1
#define RTC_PM_INT_EN_PIR_WAKE_0        0x0
#define RTC_PM_INT_EN_PIR_WAKE_1        0x1
#define RTC_PM_INT_EN_RING_KEY_0        0x0
#define RTC_PM_INT_EN_RING_KEY_1        0x1

/* Power Manage Clear Pending Status Register */
typedef union {
	u32 val;
	struct {
	u32 pw_con:1;                  /* */
	u32 pw_discon:1;               /* */
	u32 key_press:1;               /* */
	u32 key_short:1;               /* */
	u32 key_long:1;                /* */
	u32 key_release:1;             /* */
	u32 gs_int:1;                  /* */
	u32 pir_wake:1;                /* */
	u32 ring_key:1;                /* */
	u32 rsvd0:23;                  /* */
	} bits;
} reg_rtc_pm_pend_clr_t;

#define RTC_PM_PEND_CLR_PW_CON_0        0x0
#define RTC_PM_PEND_CLR_PW_CON_1        0x1
#define RTC_PM_PEND_CLR_PW_DISCON_0     0x0
#define RTC_PM_PEND_CLR_PW_DISCON_1     0x1
#define RTC_PM_PEND_CLR_KEY_PRESS_0     0x0
#define RTC_PM_PEND_CLR_KEY_PRESS_1     0x1
#define RTC_PM_PEND_CLR_KEY_SHORT_0     0x0
#define RTC_PM_PEND_CLR_KEY_SHORT_1     0x1
#define RTC_PM_PEND_CLR_KEY_LONG_0      0x0
#define RTC_PM_PEND_CLR_KEY_LONG_1      0x1
#define RTC_PM_PEND_CLR_KEY_RELEASE_0   0x0
#define RTC_PM_PEND_CLR_KEY_RELEASE_1   0x1
#define RTC_PM_PEND_CLR_GS_INT_0        0x0
#define RTC_PM_PEND_CLR_GS_INT_1        0x1
#define RTC_PM_PEND_CLR_PIR_WAKE_0      0x0
#define RTC_PM_PEND_CLR_PIR_WAKE_1      0x1
#define RTC_PM_PEND_CLR_RING_KEY_0      0x0
#define RTC_PM_PEND_CLR_RING_KEY_1      0x1

/* Power Manage Pending Status Register */
typedef union {
	u32 val;
	struct {
	u32 pw_con:1;                  /* */
	u32 pw_discon:1;               /* */
	u32 key_press:1;               /* */
	u32 key_short:1;               /* */
	u32 key_long:1;                /* */
	u32 key_release:1;             /* */
	u32 gs_int:1;                  /* */
	u32 pir_wake:1;                /* */
	u32 ring_key:1;                /* */
	u32 rsvd0:23;                  /* */
	} bits;
} reg_rtc_pm_pend_t;

#define RTC_PM_PEND_PW_CON_0            0x0
#define RTC_PM_PEND_PW_CON_1            0x1
#define RTC_PM_PEND_PW_DISCON_0         0x0
#define RTC_PM_PEND_PW_DISCON_1         0x1
#define RTC_PM_PEND_KEY_PRESS_0         0x0
#define RTC_PM_PEND_KEY_PRESS_1         0x1
#define RTC_PM_PEND_KEY_SHORT_0         0x0
#define RTC_PM_PEND_KEY_SHORT_1         0x1
#define RTC_PM_PEND_KEY_LONG_0          0x0
#define RTC_PM_PEND_KEY_LONG_1          0x1
#define RTC_PM_PEND_KEY_RELEASE_0       0x0
#define RTC_PM_PEND_KEY_RELEASE_1       0x1
#define RTC_PM_PEND_GS_INT_0            0x0
#define RTC_PM_PEND_GS_INT_1            0x1
#define RTC_PM_PEND_PIR_WAKE_0          0x0
#define RTC_PM_PEND_PIR_WAKE_1          0x1
#define RTC_PM_PEND_RING_KEY_0          0x0
#define RTC_PM_PEND_RING_KEY_1          0x1

/* Power Manage Power Enable 1 Register */
typedef union {
	u32 val;
	struct {
	u32 en:1;              /* */
	u32 pol:1;             /* */
	u32 rsvd0:2;           /* */
	u32 tp:13;             /* */
	u32 rsvd1:2;           /* */
	u32 ap:13;             /* */
	} bits;
} reg_rtc_pm_pe1_t;

#define RTC_PM_PE1_EN_0         0x0
#define RTC_PM_PE1_EN_1         0x1
#define RTC_PM_PE1_POL_0        0x0
#define RTC_PM_PE1_POL_1        0x1

/* LockDown Control register */
typedef union {
	u32 val;
	struct {
	u32 deep_slp_exit:1;           /* */
	u32 deep_slp_flag:1;           /* */
	u32 rsvd0:2;                   /* */
	u32 hot_rst_exit:1;            /* */
	u32 hot_rst_flag:1;            /* */
	u32 rsvd1:2;                   /* */
	u32 pad_hold:1;                /* */
	u32 sys_stat:1;                /* */
	u32 rsvd2:22;                  /* */
	} bits;
} reg_rtc_lock_ctrl_t;

/* Deep Sleep Flag register */
typedef union {
	u32 val;
	struct {
	u32 flag:32;           /* */
	} bits;
} reg_rtc_deep_slp_flag_t;

/* Deep Sleep Exit Address Register */
typedef union {
	u32 val;
	struct {
	u32 exit_addr:32;              /* */
	} bits;
} reg_rtc_deep_slp_exit_t;

/* Hot Reset Flag register */
typedef union {
	u32 val;
	struct {
	u32 flag:32;           /* */
	} bits;
} reg_rtc_hot_rst_flag_t;

/* Hot Reset Exit Address register */
typedef union {
	u32 val;
	struct {
	u32 exit_addr:32;              /* */
	} bits;
} reg_rtc_hot_rst_exit_t;

/* PAD Hold Control register */
typedef union {
	u32 val;
	struct {
	u32 ddr_pad:1;         /* */
	u32 rsvd0:31;          /* */
	} bits;
} reg_rtc_pad_hold_t;

#define RTC_PAD_HOLD_DDR_PAD_0  0x0
#define RTC_PAD_HOLD_DDR_PAD_1  0x1

/* CROM System Status register */
typedef union {
	u32 val;
	struct {
	u32 stat:32;           /* */
	} bits;
} reg_rtc_sys_stat_t;

/* AX Power Clamp Control Register */
typedef union {
	u32 val;
	struct {
	u32 ax:1;              /* */
	u32 rsvd0:31;          /* */
	} bits;
} reg_rtc_ax_clamp_t;

#define RTC_AX_CLAMP_AX_0       0x0
#define RTC_AX_CLAMP_AX_1       0x1

/* SIO Function Configuration Register0 */
typedef union {
	u32 val;
	struct {
	u32 sio0:4;            /* */
	u32 sio1:4;            /* */
	u32 sio2:4;            /* */
	u32 sio3:4;            /* */
	u32 sio4:4;            /* */
	u32 sio5:4;            /* */
	u32 sio6:4;            /* */
	u32 sio7:4;            /* */
	} bits;
} reg_rtc_sio_func_r0_t;

#define RTC_SIO_FUNC_R0_SIO0_0  0x0
#define RTC_SIO_FUNC_R0_SIO0_1  0x1
#define RTC_SIO_FUNC_R0_SIO0_2  0x2
#define RTC_SIO_FUNC_R0_SIO0_3  0x3
#define RTC_SIO_FUNC_R0_SIO1_0  0x0
#define RTC_SIO_FUNC_R0_SIO1_1  0x1
#define RTC_SIO_FUNC_R0_SIO1_2  0x2
#define RTC_SIO_FUNC_R0_SIO1_3  0x3
#define RTC_SIO_FUNC_R0_SIO2_0  0x0
#define RTC_SIO_FUNC_R0_SIO2_1  0x1
#define RTC_SIO_FUNC_R0_SIO2_2  0x2
#define RTC_SIO_FUNC_R0_SIO2_3  0x3
#define RTC_SIO_FUNC_R0_SIO3_0  0x0
#define RTC_SIO_FUNC_R0_SIO3_1  0x1
#define RTC_SIO_FUNC_R0_SIO3_2  0x2
#define RTC_SIO_FUNC_R0_SIO3_3  0x3
#define RTC_SIO_FUNC_R0_SIO4_0  0x0
#define RTC_SIO_FUNC_R0_SIO4_1  0x1
#define RTC_SIO_FUNC_R0_SIO4_2  0x2
#define RTC_SIO_FUNC_R0_SIO4_3  0x3
#define RTC_SIO_FUNC_R0_SIO5_0  0x0
#define RTC_SIO_FUNC_R0_SIO5_1  0x1
#define RTC_SIO_FUNC_R0_SIO5_2  0x2
#define RTC_SIO_FUNC_R0_SIO5_3  0x3
#define RTC_SIO_FUNC_R0_SIO6_0  0x0
#define RTC_SIO_FUNC_R0_SIO6_1  0x1
#define RTC_SIO_FUNC_R0_SIO6_2  0x2
#define RTC_SIO_FUNC_R0_SIO6_3  0x3
#define RTC_SIO_FUNC_R0_SIO7_0  0x0
#define RTC_SIO_FUNC_R0_SIO7_1  0x1
#define RTC_SIO_FUNC_R0_SIO7_2  0x2
#define RTC_SIO_FUNC_R0_SIO7_3  0x3

/* SIO Pull Up or Pull Down Configuration Register0 */
typedef union {
	u32 val;
	struct {
	u32 sio0:2;            /* */
	u32 rsvd0:2;           /* */
	u32 sio1:2;            /* */
	u32 rsvd1:2;           /* */
	u32 sio2:2;            /* */
	u32 rsvd2:2;           /* */
	u32 sio3:2;            /* */
	u32 rsvd3:2;           /* */
	u32 sio4:2;            /* */
	u32 rsvd4:2;           /* */
	u32 sio5:2;            /* */
	u32 rsvd5:2;           /* */
	u32 sio6:2;            /* */
	u32 rsvd6:2;           /* */
	u32 sio7:2;            /* */
	u32 rsvd7:2;           /* */
	} bits;
} reg_rtc_sio_pud_r0_t;

#define RTC_SIO_PUD_R0_SIO0_0   0x0
#define RTC_SIO_PUD_R0_SIO0_1   0x1
#define RTC_SIO_PUD_R0_SIO0_2   0x2
#define RTC_SIO_PUD_R0_SIO0_3   0x3
#define RTC_SIO_PUD_R0_SIO1_0   0x0
#define RTC_SIO_PUD_R0_SIO1_1   0x1
#define RTC_SIO_PUD_R0_SIO1_2   0x2
#define RTC_SIO_PUD_R0_SIO1_3   0x3
#define RTC_SIO_PUD_R0_SIO2_0   0x0
#define RTC_SIO_PUD_R0_SIO2_1   0x1
#define RTC_SIO_PUD_R0_SIO2_2   0x2
#define RTC_SIO_PUD_R0_SIO2_3   0x3
#define RTC_SIO_PUD_R0_SIO3_0   0x0
#define RTC_SIO_PUD_R0_SIO3_1   0x1
#define RTC_SIO_PUD_R0_SIO3_2   0x2
#define RTC_SIO_PUD_R0_SIO3_3   0x3
#define RTC_SIO_PUD_R0_SIO4_0   0x0
#define RTC_SIO_PUD_R0_SIO4_1   0x1
#define RTC_SIO_PUD_R0_SIO4_2   0x2
#define RTC_SIO_PUD_R0_SIO4_3   0x3
#define RTC_SIO_PUD_R0_SIO5_0   0x0
#define RTC_SIO_PUD_R0_SIO5_1   0x1
#define RTC_SIO_PUD_R0_SIO5_2   0x2
#define RTC_SIO_PUD_R0_SIO5_3   0x3
#define RTC_SIO_PUD_R0_SIO6_0   0x0
#define RTC_SIO_PUD_R0_SIO6_1   0x1
#define RTC_SIO_PUD_R0_SIO6_2   0x2
#define RTC_SIO_PUD_R0_SIO6_3   0x3
#define RTC_SIO_PUD_R0_SIO7_0   0x0
#define RTC_SIO_PUD_R0_SIO7_1   0x1
#define RTC_SIO_PUD_R0_SIO7_2   0x2
#define RTC_SIO_PUD_R0_SIO7_3   0x3

/* SIO Driver Configuration Register0 */
typedef union {
	u32 val;
	struct {
	u32 sio0:3;            /* */
	u32 rsvd0:1;           /* */
	u32 sio1:3;            /* */
	u32 rsvd1:1;           /* */
	u32 sio2:3;            /* */
	u32 rsvd2:1;           /* */
	u32 sio3:3;            /* */
	u32 rsvd3:1;           /* */
	u32 sio4:3;            /* */
	u32 rsvd4:1;           /* */
	u32 sio5:3;            /* */
	u32 rsvd5:1;           /* */
	u32 sio6:3;            /* */
	u32 rsvd6:1;           /* */
	u32 sio7:3;            /* */
	u32 rsvd7:1;           /* */
	} bits;
} reg_rtc_sio_drv_r0_t;

#define RTC_SIO_DRV_R0_SIO0_0   0x0
#define RTC_SIO_DRV_R0_SIO0_1   0x1
#define RTC_SIO_DRV_R0_SIO0_2   0x2
#define RTC_SIO_DRV_R0_SIO0_3   0x3
#define RTC_SIO_DRV_R0_SIO0_4   0x4
#define RTC_SIO_DRV_R0_SIO0_5   0x5
#define RTC_SIO_DRV_R0_SIO0_6   0x6
#define RTC_SIO_DRV_R0_SIO0_7   0x7
#define RTC_SIO_DRV_R0_SIO1_0   0x0
#define RTC_SIO_DRV_R0_SIO1_1   0x1
#define RTC_SIO_DRV_R0_SIO1_2   0x2
#define RTC_SIO_DRV_R0_SIO1_3   0x3
#define RTC_SIO_DRV_R0_SIO1_4   0x4
#define RTC_SIO_DRV_R0_SIO1_5   0x5
#define RTC_SIO_DRV_R0_SIO1_6   0x6
#define RTC_SIO_DRV_R0_SIO1_7   0x7
#define RTC_SIO_DRV_R0_SIO2_0   0x0
#define RTC_SIO_DRV_R0_SIO2_1   0x1
#define RTC_SIO_DRV_R0_SIO2_2   0x2
#define RTC_SIO_DRV_R0_SIO2_3   0x3
#define RTC_SIO_DRV_R0_SIO2_4   0x4
#define RTC_SIO_DRV_R0_SIO2_5   0x5
#define RTC_SIO_DRV_R0_SIO2_6   0x6
#define RTC_SIO_DRV_R0_SIO2_7   0x7
#define RTC_SIO_DRV_R0_SIO3_0   0x0
#define RTC_SIO_DRV_R0_SIO3_1   0x1
#define RTC_SIO_DRV_R0_SIO3_2   0x2
#define RTC_SIO_DRV_R0_SIO3_3   0x3
#define RTC_SIO_DRV_R0_SIO3_4   0x4
#define RTC_SIO_DRV_R0_SIO3_5   0x5
#define RTC_SIO_DRV_R0_SIO3_6   0x6
#define RTC_SIO_DRV_R0_SIO3_7   0x7
#define RTC_SIO_DRV_R0_SIO4_0   0x0
#define RTC_SIO_DRV_R0_SIO4_1   0x1
#define RTC_SIO_DRV_R0_SIO4_2   0x2
#define RTC_SIO_DRV_R0_SIO4_3   0x3
#define RTC_SIO_DRV_R0_SIO4_4   0x4
#define RTC_SIO_DRV_R0_SIO4_5   0x5
#define RTC_SIO_DRV_R0_SIO4_6   0x6
#define RTC_SIO_DRV_R0_SIO4_7   0x7
#define RTC_SIO_DRV_R0_SIO5_0   0x0
#define RTC_SIO_DRV_R0_SIO5_1   0x1
#define RTC_SIO_DRV_R0_SIO5_2   0x2
#define RTC_SIO_DRV_R0_SIO5_3   0x3
#define RTC_SIO_DRV_R0_SIO5_4   0x4
#define RTC_SIO_DRV_R0_SIO5_5   0x5
#define RTC_SIO_DRV_R0_SIO5_6   0x6
#define RTC_SIO_DRV_R0_SIO5_7   0x7
#define RTC_SIO_DRV_R0_SIO6_0   0x0
#define RTC_SIO_DRV_R0_SIO6_1   0x1
#define RTC_SIO_DRV_R0_SIO6_2   0x2
#define RTC_SIO_DRV_R0_SIO6_3   0x3
#define RTC_SIO_DRV_R0_SIO6_4   0x4
#define RTC_SIO_DRV_R0_SIO6_5   0x5
#define RTC_SIO_DRV_R0_SIO6_6   0x6
#define RTC_SIO_DRV_R0_SIO6_7   0x7
#define RTC_SIO_DRV_R0_SIO7_0   0x0
#define RTC_SIO_DRV_R0_SIO7_1   0x1
#define RTC_SIO_DRV_R0_SIO7_2   0x2
#define RTC_SIO_DRV_R0_SIO7_3   0x3
#define RTC_SIO_DRV_R0_SIO7_4   0x4
#define RTC_SIO_DRV_R0_SIO7_5   0x5
#define RTC_SIO_DRV_R0_SIO7_6   0x6
#define RTC_SIO_DRV_R0_SIO7_7   0x7

/* SIO Data Register0 */
typedef union {
	u32 val;
	struct {
	u32 sio_data:8;        /* */
	u32 rsvd0:24;          /* */
	} bits;
} reg_rtc_sio_data_r0_t;

/* SIO Pull Up/Down Resistors Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 pull_cfg:8;        /* */
	u32 rsvd0:24;          /* */
	} bits;
} reg_rtc_sio_pr_r_t;

#define RTC_SIO_PR_R_PULL_CFG_0 0x0
#define RTC_SIO_PR_R_PULL_CFG_1 0x1

/* SIO External Interrupt Trigger Configuration Register0 */
typedef union {
	u32 val;
	struct {
	u32 sio0:3;            /* */
	u32 rsvd0:1;           /* */
	u32 sio1:3;            /* */
	u32 rsvd1:1;           /* */
	u32 sio2:3;            /* */
	u32 rsvd2:1;           /* */
	u32 sio3:3;            /* */
	u32 rsvd3:1;           /* */
	u32 sio4:3;            /* */
	u32 rsvd4:1;           /* */
	u32 sio5:3;            /* */
	u32 rsvd5:1;           /* */
	u32 sio6:3;            /* */
	u32 rsvd6:1;           /* */
	u32 sio7:3;            /* */
	u32 rsvd7:1;           /* */
	} bits;
} reg_rtc_sio_int_tri_r0_t;

#define RTC_SIO_INT_TRI_R0_SIO0_0       0x0
#define RTC_SIO_INT_TRI_R0_SIO0_1       0x1
#define RTC_SIO_INT_TRI_R0_SIO0_2       0x2
#define RTC_SIO_INT_TRI_R0_SIO0_3       0x3
#define RTC_SIO_INT_TRI_R0_SIO0_4       0x4
#define RTC_SIO_INT_TRI_R0_SIO1_0       0x0
#define RTC_SIO_INT_TRI_R0_SIO1_1       0x1
#define RTC_SIO_INT_TRI_R0_SIO1_2       0x2
#define RTC_SIO_INT_TRI_R0_SIO1_3       0x3
#define RTC_SIO_INT_TRI_R0_SIO1_4       0x4
#define RTC_SIO_INT_TRI_R0_SIO2_0       0x0
#define RTC_SIO_INT_TRI_R0_SIO2_1       0x1
#define RTC_SIO_INT_TRI_R0_SIO2_2       0x2
#define RTC_SIO_INT_TRI_R0_SIO2_3       0x3
#define RTC_SIO_INT_TRI_R0_SIO2_4       0x4
#define RTC_SIO_INT_TRI_R0_SIO3_0       0x0
#define RTC_SIO_INT_TRI_R0_SIO3_1       0x1
#define RTC_SIO_INT_TRI_R0_SIO3_2       0x2
#define RTC_SIO_INT_TRI_R0_SIO3_3       0x3
#define RTC_SIO_INT_TRI_R0_SIO3_4       0x4
#define RTC_SIO_INT_TRI_R0_SIO4_0       0x0
#define RTC_SIO_INT_TRI_R0_SIO4_1       0x1
#define RTC_SIO_INT_TRI_R0_SIO4_2       0x2
#define RTC_SIO_INT_TRI_R0_SIO4_3       0x3
#define RTC_SIO_INT_TRI_R0_SIO4_4       0x4
#define RTC_SIO_INT_TRI_R0_SIO5_0       0x0
#define RTC_SIO_INT_TRI_R0_SIO5_1       0x1
#define RTC_SIO_INT_TRI_R0_SIO5_2       0x2
#define RTC_SIO_INT_TRI_R0_SIO5_3       0x3
#define RTC_SIO_INT_TRI_R0_SIO5_4       0x4
#define RTC_SIO_INT_TRI_R0_SIO6_0       0x0
#define RTC_SIO_INT_TRI_R0_SIO6_1       0x1
#define RTC_SIO_INT_TRI_R0_SIO6_2       0x2
#define RTC_SIO_INT_TRI_R0_SIO6_3       0x3
#define RTC_SIO_INT_TRI_R0_SIO6_4       0x4
#define RTC_SIO_INT_TRI_R0_SIO7_0       0x0
#define RTC_SIO_INT_TRI_R0_SIO7_1       0x1
#define RTC_SIO_INT_TRI_R0_SIO7_2       0x2
#define RTC_SIO_INT_TRI_R0_SIO7_3       0x3
#define RTC_SIO_INT_TRI_R0_SIO7_4       0x4

/* SIO External Interrupt Mask Register0 */
typedef union {
	u32 val;
	struct {
	u32 sio_int_mask:8;            /* */
	u32 rsvd0:24;                  /* */
	} bits;
} reg_rtc_sio_int_mask0_t;

#define RTC_SIO_INT_MASK0_SIO_INT_MASK_0        0x0
#define RTC_SIO_INT_MASK0_SIO_INT_MASK_1        0x1

/* SIO External Interrupt Pending Register0 */
typedef union {
	u32 val;
	struct {
	u32 sio_int_pend:8;            /* */
	u32 rsvd0:24;                  /* */
	} bits;
} reg_rtc_sio_int_pend0_t;

#define RTC_SIO_INT_PEND0_SIO_INT_PEND_0        0x0
#define RTC_SIO_INT_PEND0_SIO_INT_PEND_1        0x1

/* SIO Interrupt Sample Time Control Register0 */
typedef union {
	u32 val;
	struct {
	u32 src_sel:2;         /* */
	u32 rsvd0:2;           /* */
	u32 div:4;             /* */
	u32 rsvd1:23;          /* */
	u32 en:1;              /* */
	} bits;
} reg_rtc_sio_int_samp_ctrl0_t;

#define RTC_SIO_INT_SAMP_CTRL0_SRC_SEL_0        0x0
#define RTC_SIO_INT_SAMP_CTRL0_SRC_SEL_1        0x1
#define RTC_SIO_INT_SAMP_CTRL0_DIV_0            0x0
#define RTC_SIO_INT_SAMP_CTRL0_DIV_1            0x1
#define RTC_SIO_INT_SAMP_CTRL0_DIV_2            0x2
#define RTC_SIO_INT_SAMP_CTRL0_DIV_3            0x3
#define RTC_SIO_INT_SAMP_CTRL0_DIV_4            0x4
#define RTC_SIO_INT_SAMP_CTRL0_DIV_5            0x5
#define RTC_SIO_INT_SAMP_CTRL0_DIV_6            0x6
#define RTC_SIO_INT_SAMP_CTRL0_DIV_7            0x7
#define RTC_SIO_INT_SAMP_CTRL0_DIV_8            0x8
#define RTC_SIO_INT_SAMP_CTRL0_DIV_9            0x9
#define RTC_SIO_INT_SAMP_CTRL0_DIV_A            0xA
#define RTC_SIO_INT_SAMP_CTRL0_EN_0             0x0
#define RTC_SIO_INT_SAMP_CTRL0_EN_1             0x1

/* reg_rtc_t bank */
typedef struct tag_rtc {
	reg_rtc_ver_t                      ver;                    /* 0000 */
	u32                                res0[7];
	reg_rtc_rtc_clk_ctrl_t             rtc_clk_ctrl;           /* 0020 */
	reg_rtc_rtc_ld_en_t                rtc_ld_en;              /* 0024 */
	reg_rtc_rtc_ld_int_en_t            rtc_ld_int_en;          /* 0028 */
	reg_rtc_rtc_ld_int_clr_t           rtc_ld_int_clr;         /* 002C */
	reg_rtc_rtc_ld_int_pending_t       rtc_ld_int_pending;     /* 0030 */
	u32                                res1[3];
	reg_rtc_rtc_ymd_t                  rtc_ymd;                /* 0040 */
	reg_rtc_rtc_week_t                 rtc_week;               /* 0044 */
	reg_rtc_rtc_hms_t                  rtc_hms;                /* 0048 */
	reg_rtc_rtc_alarm_int_en_t         rtc_alarm_int_en;       /* 004C */
	reg_rtc_rtc_alarm_int_clr_t        rtc_alarm_int_clr;      /* 0050 */
	reg_rtc_rtc_alarm_int_pending_t    rtc_alarm_int_pending;  /* 0054 */
	reg_rtc_rtc_alarm_match_en_t       rtc_alarm_match_en;     /* 0058 */
	reg_rtc_rtc_alarm_day_match_t      rtc_alarm_day_match;    /* 005C */
	reg_rtc_rtc_alarm_week_match_t     rtc_alarm_week_match;   /* 0060 */
	reg_rtc_rtc_alarm_hms_match_t      rtc_alarm_hms_match;    /* 0064 */
	reg_rtc_rtc_wakeup_en_t            rtc_wakeup_en;          /* 0068 */
	reg_rtc_rtc_lfeosc_fanout_cfg_t    rtc_lfeosc_fanout_cfg;  /* 006C */
	reg_rtc_rtc_su_en_t                rtc_su_en;              /* 0070 */
	u32                                res2[227];
	reg_rtc_rtc_core_t                 rtc_core;               /* 0400 */
	reg_rtc_rtc_top_t                  rtc_top;                /* 0404 */
	reg_rtc_rtc_hfeosc_t               rtc_hfeosc;             /* 0408 */
	reg_rtc_rtc_lfeosc_t               rtc_lfeosc;             /* 040C */
	u32                                res3[4];
	reg_rtc_rtc_alive_reg0_t           rtc_alive_reg0;         /* 0420 */
	reg_rtc_rtc_alive_reg1_t           rtc_alive_reg1;         /* 0424 */
	reg_rtc_rtc_alive_reg2_t           rtc_alive_reg2;         /* 0428 */
	reg_rtc_rtc_alive_reg3_t           rtc_alive_reg3;         /* 042C */
	u32                                res4[12];
	reg_rtc_pm_pe_t                    pm_pe;                  /* 0460 */
	reg_rtc_pm_cfg_t                   pm_cfg;                 /* 0464 */
	reg_rtc_pm_int_en_t                pm_int_en;              /* 0468 */
	reg_rtc_pm_pend_clr_t              pm_pend_clr;            /* 046C */
	reg_rtc_pm_pend_t                  pm_pend;                /* 0470 */
	reg_rtc_pm_pe1_t                   pm_pe1;                 /* 0474 */
	u32                                res5[162];
	reg_rtc_lock_ctrl_t                lock_ctrl;              /* 0700 */
	reg_rtc_deep_slp_flag_t            deep_slp_flag;          /* 0704 */
	reg_rtc_deep_slp_exit_t            deep_slp_exit;          /* 0708 */
	u32                                res6[1];
	reg_rtc_hot_rst_flag_t             hot_rst_flag;           /* 0710 */
	reg_rtc_hot_rst_exit_t             hot_rst_exit;           /* 0714 */
	u32                                res7[2];
	reg_rtc_pad_hold_t                 pad_hold;               /* 0720 */
	u32                                res8[1];
	reg_rtc_sys_stat_t                 sys_stat;               /* 0728 */
	u32                                res9[13];
	reg_rtc_ax_clamp_t                 ax_clamp;               /* 0760 */
	u32                                res10[47];
	reg_rtc_sio_func_r0_t              sio_func_r0;            /* 0820 */
	u32                                res11[3];
	reg_rtc_sio_pud_r0_t               sio_pud_r0;             /* 0830 */
	u32                                res12[3];
	reg_rtc_sio_drv_r0_t               sio_drv_r0;             /* 0840 */
	u32                                res13[3];
	reg_rtc_sio_data_r0_t              sio_data_r0;            /* 0850 */
	reg_rtc_sio_pr_r_t                 sio_pr_r;               /* 0854 */
	u32                                res14[2];
	reg_rtc_sio_int_tri_r0_t           sio_int_tri_r0;         /* 0860 */
	u32                                res15[3];
	reg_rtc_sio_int_mask0_t            sio_int_mask0;          /* 0870 */
	reg_rtc_sio_int_pend0_t            sio_int_pend0;          /* 0874 */
	reg_rtc_sio_int_samp_ctrl0_t       sio_int_samp_ctrl0;     /* 0878 */
} reg_rtc_t;

#endif /* ___RTC___H___ */
