/* tcon.h */

#ifndef _CSP_DOSS_TCON_REG_V1_1_H_
#define _CSP_DOSS_TCON_REG_V1_1_H_

#include <stdbool.h>

#define BASE_TCON                0x01602000

#define VA_TCON_VER                      (0x00000000 + BASE_TCON + VA_TCON)
#define VA_TCON_FTR                      (0x00000004 + BASE_TCON + VA_TCON)
#define VA_TCON_TCON_CTRL                (0x00000010 + BASE_TCON + VA_TCON)
#define VA_TCON_TCON_CFG                 (0x00000014 + BASE_TCON + VA_TCON)
#define VA_TCON_VSYNC                    (0x00000018 + BASE_TCON + VA_TCON)
#define VA_TCON_IO_CFG                   (0x0000001C + BASE_TCON + VA_TCON)
#define VA_TCON_FIFO_CTRL                (0x00000020 + BASE_TCON + VA_TCON)
#define VA_TCON_FIFO_STA                 (0x00000024 + BASE_TCON + VA_TCON)
#define VA_TCON_FIFO_DATA                (0x00000028 + BASE_TCON + VA_TCON)
#define VA_TCON_BG_COLOR                 (0x0000002C + BASE_TCON + VA_TCON)
#define VA_TCON_INT_EN                   (0x00000030 + BASE_TCON + VA_TCON)
#define VA_TCON_INT_PD                   (0x00000034 + BASE_TCON + VA_TCON)
#define VA_TCON_INT_CLR                  (0x00000038 + BASE_TCON + VA_TCON)
#define VA_TCON_LINE_CNT                 (0x0000003C + BASE_TCON + VA_TCON)
#define VA_TCON_GEN_HTIM0                (0x00000040 + BASE_TCON + VA_TCON)
#define VA_TCON_GEN_HTIM1                (0x00000044 + BASE_TCON + VA_TCON)
#define VA_TCON_GEN_VTIM0                (0x00000048 + BASE_TCON + VA_TCON)
#define VA_TCON_GEN_VTIM1                (0x0000004C + BASE_TCON + VA_TCON)
#define VA_TCON_GEN_VTIM2                (0x00000050 + BASE_TCON + VA_TCON)
#define VA_TCON_GEN_VTIM3                (0x00000054 + BASE_TCON + VA_TCON)
#define VA_TCON_GEN_IN_RES0              (0x00000058 + BASE_TCON + VA_TCON)
#define VA_TCON_GEN_IN_RES1              (0x0000005C + BASE_TCON + VA_TCON)
#define VA_TCON_GEN_AS_COORD0            (0x00000060 + BASE_TCON + VA_TCON)
#define VA_TCON_GEN_AS_DAT0              (0x00000064 + BASE_TCON + VA_TCON)
#define VA_TCON_GEN_AS_COORD1            (0x00000068 + BASE_TCON + VA_TCON)
#define VA_TCON_GEN_AS_DAT1              (0x0000006C + BASE_TCON + VA_TCON)
#define VA_TCON_GEN_AS_COORD2            (0x00000070 + BASE_TCON + VA_TCON)
#define VA_TCON_GEN_AS_DAT2              (0x00000074 + BASE_TCON + VA_TCON)
#define VA_TCON_RGB_IF_CFG               (0x00000080 + BASE_TCON + VA_TCON)
#define VA_TCON_CPU_IF_CFG               (0x00000084 + BASE_TCON + VA_TCON)
#define VA_TCON_CPU_VSYNC_CMD            (0x00000088 + BASE_TCON + VA_TCON)
#define VA_TCON_CPU_HSYNC_CMD            (0x0000008C + BASE_TCON + VA_TCON)
#define VA_TCON_CPU_IF_OP                (0x00000090 + BASE_TCON + VA_TCON)
#define VA_TCON_CPU_WR_DAT               (0x00000098 + BASE_TCON + VA_TCON)
#define VA_TCON_CPU_RD_DAT               (0x0000009C + BASE_TCON + VA_TCON)
#define VA_TCON_BT_CFG                   (0x000000A0 + BASE_TCON + VA_TCON)
#define VA_TCON_LVDS_IF_CFG              (0x000000D0 + BASE_TCON + VA_TCON)
#define VA_TCON_LVDS_ANL_CFG0            (0x000000D4 + BASE_TCON + VA_TCON)
#define VA_TCON_CSC_C0                   (0x00000100 + BASE_TCON + VA_TCON)
#define VA_TCON_CSC_C1                   (0x00000104 + BASE_TCON + VA_TCON)
#define VA_TCON_CSC_C2                   (0x00000108 + BASE_TCON + VA_TCON)
#define VA_TCON_CSC_C3                   (0x0000010C + BASE_TCON + VA_TCON)
#define VA_TCON_CSC_C4                   (0x00000110 + BASE_TCON + VA_TCON)
#define VA_TCON_CSC_C5                   (0x00000114 + BASE_TCON + VA_TCON)
#define VA_TCON_CSC_C6                   (0x00000118 + BASE_TCON + VA_TCON)
#define VA_TCON_CSC_C7                   (0x0000011C + BASE_TCON + VA_TCON)
#define VA_TCON_CSC_C8                   (0x00000120 + BASE_TCON + VA_TCON)
#define VA_TCON_CSC_C9                   (0x00000124 + BASE_TCON + VA_TCON)
#define VA_TCON_CSC_C10                  (0x00000128 + BASE_TCON + VA_TCON)
#define VA_TCON_CSC_C11                  (0x0000012C + BASE_TCON + VA_TCON)
#define VA_TCON_CSC_CLIP0                (0x00000130 + BASE_TCON + VA_TCON)
#define VA_TCON_CSC_CLIP1                (0x00000134 + BASE_TCON + VA_TCON)
#define VA_TCON_CSC_CLIP2                (0x00000138 + BASE_TCON + VA_TCON)
#define VA_TCON_FRC_CFG                  (0x00000140 + BASE_TCON + VA_TCON)
#define VA_TCON_FRC_SEED0                (0x00000144 + BASE_TCON + VA_TCON)
#define VA_TCON_FRC_SEED1                (0x00000148 + BASE_TCON + VA_TCON)
#define VA_TCON_FRC_SEED2                (0x0000014C + BASE_TCON + VA_TCON)
#define VA_TCON_FRC_SEED3                (0x00000150 + BASE_TCON + VA_TCON)
#define VA_TCON_FRC_SEED4                (0x00000154 + BASE_TCON + VA_TCON)
#define VA_TCON_FRC_SEED5                (0x00000158 + BASE_TCON + VA_TCON)
#define VA_TCON_FRC_LUT0                 (0x00000160 + BASE_TCON + VA_TCON)
#define VA_TCON_FRC_LUT1                 (0x00000164 + BASE_TCON + VA_TCON)
#define VA_TCON_FRC_LUT2                 (0x00000168 + BASE_TCON + VA_TCON)
#define VA_TCON_FRC_LUT3                 (0x0000016C + BASE_TCON + VA_TCON)

#define DATA_TCON_VER                    0x00001101
#define DATA_TCON_FTR                    0x0000007B
#define DATA_TCON_TCON_CTRL              0x00000000
#define DATA_TCON_TCON_CFG               0x00000000
#define DATA_TCON_VSYNC                  0x00000000
#define DATA_TCON_IO_CFG                 0x00000000
#define DATA_TCON_FIFO_CTRL              0x00000000
#define DATA_TCON_FIFO_STA               0x000000AA
#define DATA_TCON_FIFO_DATA              0x00000000
#define DATA_TCON_BG_COLOR               0x00000000
#define DATA_TCON_INT_EN                 0x00000000
#define DATA_TCON_INT_PD                 0x00000000
#define DATA_TCON_INT_CLR                0x00000000
#define DATA_TCON_LINE_CNT               0x00000000
#define DATA_TCON_GEN_HTIM0              0x00000000
#define DATA_TCON_GEN_HTIM1              0x00000000
#define DATA_TCON_GEN_VTIM0              0x00000000
#define DATA_TCON_GEN_VTIM1              0x00000000
#define DATA_TCON_GEN_VTIM2              0x00000000
#define DATA_TCON_GEN_VTIM3              0x00000000
#define DATA_TCON_GEN_IN_RES0            0x00000000
#define DATA_TCON_GEN_IN_RES1            0x00000000
#define DATA_TCON_GEN_AS_COORD0          0x00000000
#define DATA_TCON_GEN_AS_DAT0            0x00000000
#define DATA_TCON_GEN_AS_COORD1          0x00000000
#define DATA_TCON_GEN_AS_DAT1            0x00000000
#define DATA_TCON_GEN_AS_COORD2          0x00000000
#define DATA_TCON_GEN_AS_DAT2            0x00000000
#define DATA_TCON_RGB_IF_CFG             0x00000000
#define DATA_TCON_CPU_IF_CFG             0x00000000
#define DATA_TCON_CPU_VSYNC_CMD          0x0000002C
#define DATA_TCON_CPU_HSYNC_CMD          0x0000003C
#define DATA_TCON_CPU_IF_OP              0x00000000
#define DATA_TCON_CPU_WR_DAT             0x00000000
#define DATA_TCON_CPU_RD_DAT             0x00000000
#define DATA_TCON_BT_CFG                 0x00000000
#define DATA_TCON_LVDS_IF_CFG            0x00000063
#define DATA_TCON_LVDS_ANL_CFG0          0x00000033
#define DATA_TCON_CSC_C0                 0x00000000
#define DATA_TCON_CSC_C1                 0x00000000
#define DATA_TCON_CSC_C2                 0x00000000
#define DATA_TCON_CSC_C3                 0x00000000
#define DATA_TCON_CSC_C4                 0x00000000
#define DATA_TCON_CSC_C5                 0x00000000
#define DATA_TCON_CSC_C6                 0x00000000
#define DATA_TCON_CSC_C7                 0x00000000
#define DATA_TCON_CSC_C8                 0x00000000
#define DATA_TCON_CSC_C9                 0x00000000
#define DATA_TCON_CSC_C10                0x00000000
#define DATA_TCON_CSC_C11                0x00000000
#define DATA_TCON_CSC_CLIP0              0x00FF0000
#define DATA_TCON_CSC_CLIP1              0x00FF0000
#define DATA_TCON_CSC_CLIP2              0x00FF0000
#define DATA_TCON_FRC_CFG                0x00000000
#define DATA_TCON_FRC_SEED0              0x00000000
#define DATA_TCON_FRC_SEED1              0x00000000
#define DATA_TCON_FRC_SEED2              0x00000000
#define DATA_TCON_FRC_SEED3              0x00000000
#define DATA_TCON_FRC_SEED4              0x00000000
#define DATA_TCON_FRC_SEED5              0x00000000
#define DATA_TCON_FRC_LUT0               0x00000000
#define DATA_TCON_FRC_LUT1               0x00000000
#define DATA_TCON_FRC_LUT2               0x00000000
#define DATA_TCON_FRC_LUT3               0x00000000

/* Version Register */
typedef union {
	u32 val;
	struct {
	u32 ver_l:5;            /* The low 5bits of version register */
	u32 rsvd0:3;            /* - */
	u32 ver_h:3;            /* The high 3bits of version register */
	u32 rsvd1:1;            /* - */
	u32 comp:1;             /* Backward Compatibility
				 * 0: Incompatible to last version of hardware
				 * 1: Compatible to last version of hardware */
	u32 rsvd2:19;           /* - */
	} bits;
} reg_tcon_ver_t;

/* Feature Register */
typedef union {
	u32 val;
	struct {
	u32 has_lcd:1;          /* Indicates that the module has RGB/CPU
				 * LCD Interface function
				 * [1¡¯b0]: Not Exist
				 * [1¡¯b1]: Exist */
	u32 has_tv:1;           /* Indicates that the module has TV interface
				 * function
				 * [1¡¯b0]: Not Exist
				 * [1¡¯b1]: Exist */
	u32 has_crmp:1;         /* Indicates that the module has color
				 * remap function
				 * [1¡¯b0]: Not Exist
				 * [1¡¯b1]: Exist */
	u32 has_tf:1;           /* Indicates that the module has TD FIFO
				 * [1¡¯b0]: Not Exist
				 * [1¡¯b1]: Exist */
	u32 has_gamma:1;        /* Indicates that the module has GAMMA function
				 * [1¡¯b0]: Not Exist
				 * [1¡¯b1]: Exist */
	u32 has_csc:1;          /* Indicates that the module has CSC function
				 * [1¡¯b0]: Not Exist
				 * [1¡¯b1]: Exist */
	u32 has_frc:1;          /* Indicates that the module has FRC function
				 * [1¡¯b0]: Not Exist
				 * [1¡¯b1]: Exist */
	u32 rsvd0:25;           /* - */
	} bits;
} reg_tcon_ftr_t;

#define TCON_FTR_HAS_LCD_0      0x0
#define TCON_FTR_HAS_LCD_1      0x1
#define TCON_FTR_HAS_TV_0       0x0
#define TCON_FTR_HAS_TV_1       0x1
#define TCON_FTR_HAS_CRMP_0     0x0
#define TCON_FTR_HAS_CRMP_1     0x1
#define TCON_FTR_HAS_TF_0       0x0
#define TCON_FTR_HAS_TF_1       0x1
#define TCON_FTR_HAS_GAMMA_0    0x0
#define TCON_FTR_HAS_GAMMA_1    0x1
#define TCON_FTR_HAS_CSC_0      0x0
#define TCON_FTR_HAS_CSC_1      0x1
#define TCON_FTR_HAS_FRC_0      0x0
#define TCON_FTR_HAS_FRC_1      0x1

/* TCON Control Register */
typedef union {
	u32 val;
	struct {
	u32 tcon_en:1;          /* TCON Enable
				 * [1¡¯b0]: Disable
				 * [1¡¯b1]: Enable
				 * When TCON_EN is cleared to ¡®0¡¯, the
				 * hardware should be disabled after the
				 * current frame has finished. */
	u32 tgen_en:1;          /* Timing Generator Enable
				 * [1¡¯b0]: Disable
				 * [1¡¯b1]: Enable */
	u32 rsvd0:2;            /* - */
	u32 gamma_en:1;         /* Gamma Enable
				 * [1¡¯b0]: Disable
				 * [1¡¯b1]: Enable */
	u32 csc_en:1;           /* CSC Enable
				 * [1¡¯b0]: Disable
				 * [1¡¯b1]: Enable */
	u32 frc_en:1;           /* Frame Rate Control Enable
				 * [1¡¯b0]: Disable
				 * [1¡¯b1]: Enable */
	u32 crmp_en:1;          /* Color Remap Enable
				 * [1¡¯b0]: Disable
				 * [1¡¯b1]: Enable */
	u32 fifo_en:1;          /* FIFO Enable
				 * [1¡¯b0]: Disable
				 * [1¡¯b1]: Enable */
	u32 asd_en:1;           /* ASD Mode
				 * [1¡¯b0]: Disable
				 * [1¡¯b1]: Enable
				 * Active Space Data Mode which inserting
				 * space data between fields */
	u32 rsvd1:21;           /* - */
	u32 mipi_en:1;          /* MIPI Interface Enable
				 * [1¡¯b0]: Disable
				 * [1¡¯b1]: Enable
				 * When MIPI_EN is enabled, CPU_RD_DAT
				 * will be read from MIPI DBI bus, otherwise,
				 * from DBI IO Bus. */
	} bits;
} reg_tcon_tcon_ctrl_t;

#define TCON_TCON_CTRL_TCON_EN_0        0x0
#define TCON_TCON_CTRL_TCON_EN_1        0x1
#define TCON_TCON_CTRL_TGEN_EN_0        0x0
#define TCON_TCON_CTRL_TGEN_EN_1        0x1
#define TCON_TCON_CTRL_GAMMA_EN_0       0x0
#define TCON_TCON_CTRL_GAMMA_EN_1       0x1
#define TCON_TCON_CTRL_CSC_EN_0         0x0
#define TCON_TCON_CTRL_CSC_EN_1         0x1
#define TCON_TCON_CTRL_FRC_EN_0         0x0
#define TCON_TCON_CTRL_FRC_EN_1         0x1
#define TCON_TCON_CTRL_CRMP_EN_0        0x0
#define TCON_TCON_CTRL_CRMP_EN_1        0x1
#define TCON_TCON_CTRL_FIFO_EN_0        0x0
#define TCON_TCON_CTRL_FIFO_EN_1        0x1
#define TCON_TCON_CTRL_ASD_EN_0         0x0
#define TCON_TCON_CTRL_ASD_EN_1         0x1
#define TCON_TCON_CTRL_MIPI_EN_0        0x0
#define TCON_TCON_CTRL_MIPI_EN_1        0x1

/* TCON Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 src_sel:3;          /* Source Image Data Selection
				 * [0x0]: Display Controller
				 * [0x1]: DMA
				 * [0x2]: Color Bar
				 * [0x3]: Gray Scale
				 * [0x4]: Black and White
				 * [0x5]: Grid
				 * [0x6]: Checker Board
				 * [0x7]: Back Ground Color
				 * Other: Reserved */
	u32 rsvd0:5;            /* - */
	u32 out_sel:6;          /* Video Output Interface Selection
				 * Sync RGB Interface:
				 * [0x00]: 24bit Parallel RGB
				 * [0x04]: 8bit Serial RGB
				 * [0x05]: 8bit Serial Dummy+RGB
				 * [0x06]: 8bit Serial RGB+ Dummy
				 * CPU i8080 Interface:
				 * [0x10]: 18bit RGB666 1P1T
				 * [0x14]: 16bit RGB888 2P3T or
				 * 16bit RGB666 2P3T
				 * [0x15]: 16bit RGB565 1P1T
				 * [0x18]: 9bit RGB666 1P2T
				 * [0x1C]: 8bit RGB565 1P2T
				 * [0x1D]: 8bit RGB888 1P3T or
				 * 8bit RGB666 1P3T
				 * Digital TV Timing Interface:
				 * [0x21]: 16bit BT.1120(Embedded Sync)
				 * or 16bit BT.601(Separated Sync)
				 * [0x22]: 8bit BT.1120 or 8bit BT.601(Separated
				 * Sync)
				 * [0x23]: 8bit BT.656(Embedded Sync) or
				 * 8bit BT.601(Separated Sync)
				 * [0x24]: NTSC for TV Encoder
				 * [0x25]: PAL for TV Encoder
				 * Others: Reserved */
	u32 rsvd2:2;            /* - */
	u32 out_mode:1;         /* Output Mode
				 * [1¡¯b0]: Single
				 * [1¡¯b1]: Dual (odd pixels in one channel
				 * and even pixels in the other channel) */
	u32 fifo_mode:2;        /* FIFO Mode
				 * [2¡¯b00]: Normal
				 * [2¡¯b01]: Two Halves (Left and Right)
				 * [2¡¯b10]: EC (Error Corrected)
				 * [2¡¯b11]: Reserved */
	u32 rsvd3:1;            /* - */
	u32 rb_swap:1;          /* Swap red and blue after FIFO
				 * [1¡¯b0]: Non-Swap
				 * [1¡¯b1]: Swap */
	u32 clk_mode:1;         /* Clock sample mode
				 * [1¡¯b0]: SDR
				 * [1¡¯b1]: DDR
				 * Only valid in Parallel RGB and BT.601/B
				 * T.656/BT.1120 */
	u32 ds_mode:1;          /* YCbCr444 to YCbCr422 down sampling
				 * [1¡¯b0]: Directly (Horizontal)
				 * [1¡¯b1]: Averaged (Horizontal) */
	u32 rsvd4:1;            /* - */
	u32 sram_acs:1;         /* Access for LUT SRAM of Gamma Table
				 * [1¡¯b0]: TCON
				 * [1¡¯b1]: AHB-BUS */
	u32 rsvd5:3;            /* - */
	u32 trig_src:2;         /* Timing Generator Trigger Source
				 * [2¡¯b00]: Internal Register
				 * [2¡¯b01]: TE Signal
				 * [2¡¯b10]: External Trigger
				 * [2¡¯b11]: Reserved */
	u32 trig_mode:1;        /* Timing Generator Trigger Mode
				 * [1¡¯b0]: Auto
				 * [1¡¯b1]: Single */
	u32 is_itl:1;           /* Progressive or Interlaced output
				 * [1¡¯b0]: Progressive
				 * [1¡¯b1]: Interlaced */
	} bits;
} reg_tcon_tcon_cfg_t;

#define TCON_TCON_CFG_SRC_SEL_0         0x0
#define TCON_TCON_CFG_SRC_SEL_1         0x1
#define TCON_TCON_CFG_SRC_SEL_2         0x2
#define TCON_TCON_CFG_SRC_SEL_3         0x3
#define TCON_TCON_CFG_SRC_SEL_4         0x4
#define TCON_TCON_CFG_SRC_SEL_5         0x5
#define TCON_TCON_CFG_SRC_SEL_6         0x6
#define TCON_TCON_CFG_SRC_SEL_7         0x7

#define TCON_TCON_CFG_OUT_SEL_0         0x0
#define TCON_TCON_CFG_OUT_SEL_4         0x4
#define TCON_TCON_CFG_OUT_SEL_5         0x5
#define TCON_TCON_CFG_OUT_SEL_6         0x6
#define TCON_TCON_CFG_OUT_SEL_10        0x10
#define TCON_TCON_CFG_OUT_SEL_14        0x14
#define TCON_TCON_CFG_OUT_SEL_15        0x15
#define TCON_TCON_CFG_OUT_SEL_18        0x18
#define TCON_TCON_CFG_OUT_SEL_1C        0x1C
#define TCON_TCON_CFG_OUT_SEL_1D        0x1D
#define TCON_TCON_CFG_OUT_SEL_21        0x21
#define TCON_TCON_CFG_OUT_SEL_22        0x22
#define TCON_TCON_CFG_OUT_SEL_23        0x23
#define TCON_TCON_CFG_OUT_SEL_24        0x24
#define TCON_TCON_CFG_OUT_SEL_25        0x25
#define TCON_TCON_CFG_OUT_MODE_0        0x0
#define TCON_TCON_CFG_OUT_MODE_1        0x1
#define TCON_TCON_CFG_FIFO_MODE_0       0x0
#define TCON_TCON_CFG_FIFO_MODE_1       0x1
#define TCON_TCON_CFG_FIFO_MODE_2       0x2
#define TCON_TCON_CFG_FIFO_MODE_3       0x3
#define TCON_TCON_CFG_RB_SWAP_0         0x0
#define TCON_TCON_CFG_RB_SWAP_1         0x1
#define TCON_TCON_CFG_CLK_MODE_0        0x0
#define TCON_TCON_CFG_CLK_MODE_1        0x1
#define TCON_TCON_CFG_DS_MODE_0         0x0
#define TCON_TCON_CFG_DS_MODE_1         0x1
#define TCON_TCON_CFG_SRAM_ACS_0        0x0
#define TCON_TCON_CFG_SRAM_ACS_1        0x1
#define TCON_TCON_CFG_TRIG_SRC_0        0x0
#define TCON_TCON_CFG_TRIG_SRC_1        0x1
#define TCON_TCON_CFG_TRIG_SRC_2        0x2
#define TCON_TCON_CFG_TRIG_SRC_3        0x3
#define TCON_TCON_CFG_TRIG_MODE_0       0x0
#define TCON_TCON_CFG_TRIG_MODE_1       0x1
#define TCON_TCON_CFG_IS_ITL_0          0x0
#define TCON_TCON_CFG_IS_ITL_1          0x1

/* Vertical Synchronization Register */
typedef union {
	u32 val;
	struct {
	u32 dc_sync_dly:12;     /* The time that DC Sync Signal will last,
				 * counted by lines.
				 * Note: DC Sync Signal will be set at
				 * the second line of VFP, and will be
				 * cleared after DC_SYNC_DLY. */
	u32 rsvd0:4;            /* - */
	u32 vbi_trig:2;         /* Trigger Timing for VBI interrupt
				 * [2¡¯b00]: Start of VFP
				 * [2¡¯b01]: Start of VSW
				 * [2¡¯b10]: Start of VBP
				 * [2¡¯b11]: End of VBP */
	u32 rsvd1:14;           /* - */
	} bits;
} reg_tcon_vsync_t;

#define TCON_VSYNC_VBI_TRIG_0           0x0
#define TCON_VSYNC_VBI_TRIG_1           0x1
#define TCON_VSYNC_VBI_TRIG_2           0x2
#define TCON_VSYNC_VBI_TRIG_3           0x3

/* IO Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 dclk_wr_inv:1;      /* DCLK/WR Polarity Inversion
				 * [1¡¯b0]: Non-Inverted (Rising Edge)
				 * [1¡¯b1]: Inverted (Falling Edge) */
	u32 de_rd_inv:1;        /* DE/RD Polarity Inversion
				 * [1¡¯b0]: Non-Inverted (High Active)
				 * [1¡¯b1]: Inverted (Low Active)
				 * Also valid in LVDS Mode */
	u32 hs_rs_inv:1;        /* HSYNC/RS Polarity Inversion
				 * [1¡¯b0]: Non-Inverted (High Active)
				 * [1¡¯b1]: Inverted (Low Active)
				 * Also valid in LVDS Mode */
	u32 vs_cs_inv:1;        /* VSYNC/CS Polarity Inversion
				 * [1¡¯b0]: Non-Inverted (High Active)
				 * [1¡¯b1]: Inverted (Low Active)
				 * Also valid in LVDS Mode */
	u32 field_inv:1;        /* FIELD Polarity Inversion
					 * [1¡¯b0]: Non-Inverted
					 * Low for Field1,High for Field2
					 * [1¡¯b1]: Inverted
					 * High for Field1,Low for Field2 */
	u32 hb_inv:1;           /* HBLANK Polarity Inversion
				 * [1¡¯b0]: Non-Inverted (High Active)
				 * [1¡¯b1]: Inverted (Low Active) */
	u32 vb_inv:1;           /* VBLANK Polarity Inversion
				 * [1¡¯b0]: Non-Inverted (High Active)
				 * [1¡¯b1]: Inverted (Low Active) */
	u32 te_inv:1;           /* TE Polarity Inversion
				 * [1¡¯b0]: Non-Inverted (High Active)
				 * [1¡¯b1]: Inverted (Low Active) */
	u32 clk_dly:6;          /* Clock Delay Adjustment (After DCLK Polarity
				 * Inversion )
				 * Step 0.2ns */
	u32 rsvd0:3;            /* - */
	u32 de_map:1;           /* DE function mapping
				 * [1¡¯b0]: DE
				 * [1¡¯b1]: HBLANK_BT */
	u32 hs_map:1;           /* HSYNC function mapping
				 * [1¡¯b0]: HSYNC
				 * [1¡¯b1]: HBLANK_BT */
	u32 vs_map:1;           /* VSYNC function mapping
				 * [1¡¯b0]: VSYNC
				 * [1¡¯b1]: VBLANK_BT */
	u32 field_map:1;        /* FIELD function mapping
				 * [1¡¯b0]: FIELD
				 * [1¡¯b1]: FIELD_BT */
	u32 rsvd2:11;           /* - */
	} bits;
} reg_tcon_io_cfg_t;

#define TCON_IO_CFG_DCLK_WR_INV_0       0x0
#define TCON_IO_CFG_DCLK_WR_INV_1       0x1
#define TCON_IO_CFG_DE_RD_INV_0         0x0
#define TCON_IO_CFG_DE_RD_INV_1         0x1
#define TCON_IO_CFG_HS_RS_INV_0         0x0
#define TCON_IO_CFG_HS_RS_INV_1         0x1
#define TCON_IO_CFG_VS_CS_INV_0         0x0
#define TCON_IO_CFG_VS_CS_INV_1         0x1
#define TCON_IO_CFG_HB_INV_0            0x0
#define TCON_IO_CFG_HB_INV_1            0x1
#define TCON_IO_CFG_VB_INV_0            0x0
#define TCON_IO_CFG_VB_INV_1            0x1
#define TCON_IO_CFG_FIELD_INV_0         0x0
#define TCON_IO_CFG_FIELD_INV_1         0x1
#define TCON_IO_CFG_TE_INV_0            0x0
#define TCON_IO_CFG_TE_INV_1            0x1
#define TCON_IO_CFG_DE_MAP_0            0x0
#define TCON_IO_CFG_DE_MAP_1            0x1
#define TCON_IO_CFG_HS_MAP_0            0x0
#define TCON_IO_CFG_HS_MAP_1            0x1
#define TCON_IO_CFG_VS_MAP_0            0x0
#define TCON_IO_CFG_VS_MAP_1            0x1
#define TCON_IO_CFG_FIELD_MAP_0         0x0
#define TCON_IO_CFG_FIELD_MAP_1         0x1

/* FIFO Control Register */
typedef union {
	u32 val;
	struct {
	u32 dma_en:1;           /* DMA Enable
				 * [1¡¯b0]: Disable
				 * [1¡¯b1]: Enable */
	u32 rsvd0:3;            /* - */
	u32 drq_trig:2;         /* FIFO DMA Request Triger Threshold
				 * When FIFO fill level is less or equal
				 * to  the threshold, dma_req is asserted.
				 * [2¡¯b00]: Full - 8
				 * [2¡¯b01]: Full - 16
				 * [2¡¯b10]: 1/2 Full
				 * [2¡¯b11]: Empty */
	u32 rsvd1:21;           /* - */
	u32 lf_clr:1;           /* Lack Flag Clear
				 * Write ¡®1¡¯ will clear the Lack Flag
				 * and reset the Lack Counter, auto cleared
				 * by hardware. */
	u32 rsvd2:3;            /* - */
	u32 rst:1;              /* FIFO Reset
				 * Write ¡®1¡¯ to reset FIFO control logic
				 * and flush the FIFO data.
				 * Auto Cleared By Hardware */
	} bits;
} reg_tcon_fifo_ctrl_t;

#define TCON_FIFO_CTRL_DMA_EN_0         0x0
#define TCON_FIFO_CTRL_DMA_EN_1         0x1
#define TCON_FIFO_CTRL_DRQ_TRIG_0       0x0
#define TCON_FIFO_CTRL_DRQ_TRIG_1       0x1
#define TCON_FIFO_CTRL_DRQ_TRIG_2       0x2
#define TCON_FIFO_CTRL_DRQ_TRIG_3       0x3

/* FIFO Status Register */
typedef union {
	u32 val;
	struct {
	u32 pix_full:1;         /* Full Flag for Pixel FIFO (Async FIFO
				 * at the front-end) */
	u32 pix_empty:1;        /* Empty Flag for Pixel FIFO (Async FIFO
				 * at the front-end) */
	u32 src_full:1;         /* Full Flag for Source FIFO (FIFO after
				 * source mux) */
	u32 src_empty:1;        /* Empty Flag for Source FIFO (FIFO after
				 * source mux) */
	u32 last_full:1;        /* Full Flag for Last FIFO (FIFO Before
				 * I/O interface) */
	u32 last_empty:1;       /* Empty Flag for Last FIFO (FIFO Before
				 * I/O interface) */
	u32 lb_full:1;          /* Full Flag for Line Buffer */
	u32 lb_empty:1;         /* Empty Flag for Line Buffer */
	u32 rsvd0:8;            /* - */
	u32 lack_cnt:12;  /* Counter for lacked pixel data in pixel unit */
	u32 rsvd1:3;            /* - */
	u32 lack_flag:1;        /* Indicate that lack of valid pixel data */
	} bits;
} reg_tcon_fifo_sta_t;

/* FIFO Data Register */
typedef union {
	u32 val;
	struct {
	u32 dat:24;             /* FIFO Data for DMA */
	u32 rsvd0:8;            /* - */
	} bits;
} reg_tcon_fifo_data_t;

/* Back Ground Color Register */
typedef union {
	u32 val;
	struct {
	u32 dat:24;             /* Back Ground Color
				 * {Red[7:0],Green[7:0],Blue[7:0]} */
	u32 rsvd0:8;            /* - */
	} bits;
} reg_tcon_bg_color_t;

/* Interrupt Enable Register */
typedef union {
	u32 val;
	struct {
	u32 line_trig:1;        /* Line Trigger Interrupt Enable
				 * [1¡¯b0]: Disable
				 * [1¡¯b1]: Enable */
	u32 vbi:1;              /* Vertical Blanking Interrupt Enable
				 * [1¡¯b0]: Disable
				 * [1¡¯b1]: Enable */
	u32 rsvd0:6;            /* - */
	u32 trig_finish:1;      /* Trigger Finish Interrupt Enable
				 * [1¡¯b0]: Disable
				 * [1¡¯b1]: Enable */
	u32 te_trig:1;          /* TE Trigger Interrupt Enable
				 * [1¡¯b0]: Disable
				 * [1¡¯b1]: Enable */
	u32 ext_trig:1;         /* External Trigger Interrupt Enable
				 * [1¡¯b0]: Disable
				 * [1¡¯b1]: Enable */
	u32 rsvd1:21;           /* - */
	} bits;
} reg_tcon_int_en_t;

#define TCON_INT_EN_LINE_TRIG_0         0x0
#define TCON_INT_EN_LINE_TRIG_1         0x1
#define TCON_INT_EN_VBI_0               0x0
#define TCON_INT_EN_VBI_1               0x1
#define TCON_INT_EN_TRIG_FINISH_0       0x0
#define TCON_INT_EN_TRIG_FINISH_1       0x1
#define TCON_INT_EN_TE_TRIG_0           0x0
#define TCON_INT_EN_TE_TRIG_1           0x1
#define TCON_INT_EN_EXT_TRIG_0          0x0
#define TCON_INT_EN_EXT_TRIG_1          0x1

/* Interrupt Pending Register */
typedef union {
	u32 val;
	struct {
	u32 line_trig:1;        /* Line Trigger Interrupt Pending */
	u32 vbi:1;              /* Vertical Blanking Pending */
	u32 rsvd0:6;            /* - */
	u32 trig_finish:1;      /* Trigger Finish Pending */
	u32 te_trig:1;          /* TE Trigger Pending */
	u32 ext_trig:1;         /* External Trigger Pending */
	u32 rsvd1:21;           /* - */
	} bits;
} reg_tcon_int_pd_t;

/* Interrupt Clear Register */
typedef union {
	u32 val;
	struct {
	u32 line_trig:1;        /* Line Trigger Interrupt Pending Clear
				 * [1¡¯b0]: No effect
				 * [1¡¯b1]: Write ¡°1¡± to this bit clear
				 * the corresponding pending
				 * Note: Reading from this bit is meaningless */
	u32 vbi:1;              /* Vertical Blanking Pending Clear
				 * [1¡¯b0]: No effect
				 * [1¡¯b1]: Write ¡°1¡± to this bit clear
				 * the corresponding pending
				 * Note: Reading from this bit is meaningless */
	u32 rsvd0:6;            /* - */
	u32 trig_finish:1;      /* Trigger Finish Pending Clear
				 * [1¡¯b0]: No effect
				 * [1¡¯b1]: Write ¡°1¡± to this bit clear
				 * the corresponding pending
				 * Note: Reading from this bit is meaningless */
	u32 te_trig:1;          /* TE Trigger Pending Clear
				 * [1¡¯b0]: No effect
				 * [1¡¯b1]: Write ¡°1¡± to this bit clear
				 * the corresponding pending
				 * Note: Reading from this bit is meaningless */
	u32 ext_trig:1;         /* External Trigger Pending Clear
				 * [1¡¯b0]: No effect
				 * [1¡¯b1]: Write ¡°1¡± to this bit clear
				 * the corresponding pending
				 * Note: Reading from this bit is meaningless */
	u32 rsvd1:21;           /* - */
	} bits;
} reg_tcon_int_clr_t;

#define TCON_INT_CLR_LINE_TRIG_0        0x0
#define TCON_INT_CLR_LINE_TRIG_1        0x1
#define TCON_INT_CLR_VBI_0              0x0
#define TCON_INT_CLR_VBI_1              0x1
#define TCON_INT_CLR_TRIG_FINISH_0      0x0
#define TCON_INT_CLR_TRIG_FINISH_1      0x1
#define TCON_INT_CLR_TE_TRIG_0          0x0
#define TCON_INT_CLR_TE_TRIG_1          0x1
#define TCON_INT_CLR_EXT_TRIG_0         0x0
#define TCON_INT_CLR_EXT_TRIG_1         0x1

/* Line Counter Register */
typedef union {
	u32 val;
	struct {
	u32 trig:13;            /* Line Trigger for interrupt
				 * Interrupt Status LINE will be asserted
				 * when current line counter equals to
				 * line trigger */
	u32 rsvd0:3;            /* - */
	u32 crnt:13;            /* Current line counter
				 * Note:
				 * Counter will be increased by 1 at the
				 * start of HSW
				 * Counter will be reset to ¡®0¡¯ at the
				 * first Line of VSW
				 * every frame in progressive mode
				 * every two fields in interlace mode */
	u32 rsvd1:3;            /* - */
	} bits;
} reg_tcon_line_cnt_t;

/* General Horizontal Timing 0 Register */
typedef union {
	u32 val;
	struct {
	u32 ht:13;              /* Horizontal Total Time
				 * Note: Register Value = Actual Value - 1,
				 * Counted by DCLK. */
	u32 rsvd0:3;            /* - */
	u32 hact:12;            /* Horizontal Active Time
				 * Note: Register Value = Actual Value - 1,
				 * Counted by DCLK. */
	u32 rsvd1:4;            /* - */
	} bits;
} reg_tcon_gen_htim0_t;

/* General Horizontal Timing 1 Register */
typedef union {
	u32 val;
	struct {
	u32 hfp:12;             /* Horizontal Front Porch Time
				 * Note: Register Value = Actual Value ¨C 1,
				 * Counted by DCLK. */
	u32 rsvd0:4;            /* - */
	u32 hsw:10;             /* Horizontal Synchronization Width
				 * Note: Register Value = Actual Value - 1,
				 * Counted by DCLK. */
	u32 rsvd1:6;            /* - */
	} bits;
} reg_tcon_gen_htim1_t;

/* General Vertical Timing 0 Register */
typedef union {
	u32 val;
	struct {
	u32 vt_f1:14;           /* Vertical Total Time
				 * When the output timing is interlace
				 * (TCON_CFG.IS_ITL = 1), it refers to
				 * vertical total lines for Field1.
				 * When the output timing is progressive
				 * (TCON_CFG.IS_ITL = 0) , it refers to
				 * vertical total lines for each frame.
				 * Note: Register Value = Actual Value ¨C 1,
				 * Counted by half lines. */
	u32 rsvd0:2;            /* - */
	u32 vact_f1:13;         /* Vertical Active Time
				 * When the output timing is interlace
				 * (TCON_CFG.IS_ITL = 1), it refers to
				 * vertical active lines for Field1.
				 * When the output timing is progressive
				 * (TCON_CFG.IS_ITL = 0) , it refers to
				 * vertical active lines for each frame.
				 * Note: Register Value = Actual Value ¨C 1,
				 * Counted by half lines. */
	u32 rsvd1:3;            /* - */
	} bits;
} reg_tcon_gen_vtim0_t;

/* General Vertical Timing 1 Register */
typedef union {
	u32 val;
	struct {
	u32 vfp_f1:13;          /* Vertical Front Porch Time
				 * When the output timing is interlace
				 * (TCON_CFG.IS_ITL = 1), it refers to
				 * vertical front porch for Field1.
				 * When the output timing is progressive
				 * (TCON_CFG.IS_ITL = 0) , it refers to
				 * vertical front porch for each frame.
				 * Note: Register Value = Actual Value - 1,
				 * Counted by half lines. */
	u32 rsvd0:3;            /* - */
	u32 vsw_f1:11;          /* Vertical Synchronization Width
				 * When the output timing is interlace
				 * (TCON_CFG.IS_ITL = 1), it refers to
				 * vertical synchronization width for Field1.
				 * When the output timing is progressive
				 * (TCON_CFG.IS_ITL = 0) , it refers to
				 * vertical synchronization width for each
				 * frame.
				 * Note: Register Value = Actual Value - 1,
				 * Counted by half lines. */
	u32 rsvd1:5;            /* - */
	} bits;
} reg_tcon_gen_vtim1_t;

/* General Vertical Timing 2 Register */
typedef union {
	u32 val;
	struct {
	u32 vt_f2:14;           /* Vertical Total Time
				 * When the output timing is interlace
				 * (TCON_CFG.IS_ITL = 1), it refers to
				 * vertical total lines for Field1.
				 * When the output timing is progressive
				 * (TCON_CFG.IS_ITL = 0) , it is meaningless.
				 * Note: Register Value = Actual Value - 1,
				 * Counted by half lines. */
	u32 rsvd0:2;            /* - */
	u32 vact_f2:13;         /* Vertical Active Time
				 * When the output timing is interlace
				 * (TCON_CFG.IS_ITL = 1), it refers to
				 * vertical active lines for Field2.
				 * When the output timing is progressive
				 * (TCON_CFG.IS_ITL = 0) , it is meaningless.
				 * Note: Register Value = Actual Value - 1,
				 * Counted by half lines. */
	u32 rsvd1:3;            /* - */
	} bits;
} reg_tcon_gen_vtim2_t;

/* General Vertical Timing 3 Register */
typedef union {
	u32 val;
	struct {
	u32 vfp_f2:13;          /* Vertical Front Porch Time
				 * When the output timing is interlace
				 * (TCON_CFG.IS_ITL = 1), it refers to
				 * vertical front porch for Field1.
				 * When the output timing is progressive
				 * (TCON_CFG.IS_ITL = 0) , it is meaningless.
				 * Note: Register Value = Actual Value - 1,
				 * Counted by half lines. */
	u32 rsvd0:3;            /* - */
	u32 vsw_f2:11;          /* Vertical Synchronization Width
				 * When the output timing is interlace
				 * (TCON_CFG.IS_ITL = 1), it refers to
				 * vertical synchronization width for Field1.
				 * When the output timing is progressive
				 * (TCON_CFG.IS_ITL = 0) , it is meaningless.
				 * Note: Register Value = Actual Value - 1,
				 * Counted by half lines. */
	u32 rsvd1:5;            /* - */
	} bits;
} reg_tcon_gen_vtim3_t;

/* General Input Resolution 0 Register */
typedef union {
	u32 val;
	struct {
	u32 hsize_f1:12;        /* Input Horizontal Active Size
				 * When the output timing is interlace
				 * (TCON_CFG.IS_ITL = 1), it refers to
				 * Horizontal Active Size for Field1.
				 * When the output timing is progressive
				 * (TCON_CFG.IS_ITL = 0) , it refers to
				 * Horizontal Active Size for each frame.
				 * Note: Register Value = Actual Value - 1,
				 * Counted by pixels. */
	u32 rsvd0:4;            /* - */
	u32 vsize_f1:13;        /* Input Vertical Active Size
				 * When the output timing is interlace
				 * (TCON_CFG.IS_ITL = 1), it refers to
				 * Vertical Active Size for Field1.
				 * When the output timing is progressive
				 * (TCON_CFG.IS_ITL = 0) , it refers to
				 * Vertical Active Size for each frame.
				 * Note: Register Value = Actual Value - 1,
				 * Counted by lines. */
	u32 rsvd1:3;            /* - */
	} bits;
} reg_tcon_gen_in_res0_t;


/* General Input Resolution 1 Register */
typedef union {
	u32 val;
	struct {
	u32 hsize_f2:12;        /* Input Horizontal Active Size
				 * When the output timing is interlace
				 * (TCON_CFG.IS_ITL = 1), it refers to
				 * Horizontal Active Size for Field1.
				 * When the output timing is progressive
				 * (TCON_CFG.IS_ITL = 0) , it is meaningless.
				 * Note: Register Value = Actual Value - 1,
				 * Counted by pixels. */
	u32 rsvd0:4;            /* - */
	u32 vsize_f2:13;        /* Input Vertical Active Size
				 * When the output timing is interlace
				 * (TCON_CFG.IS_ITL = 1), it refers to
				 * Vertical Active Size for Field1.
				 * When the output timing is progressive
				 * (TCON_CFG.IS_ITL = 0) , it is meaningless.
				 * Note: Register Value = Actual Value - 1,
				 * Counted by lines. */
	u32 rsvd1:3;            /* - */
	} bits;
} reg_tcon_gen_in_res1_t;


/* General Active Space Coordinate 0 Register */
typedef union {
	u32 val;
	struct {
	u32 begin:10;           /* Begin of Active Space 0
				 * When the output timing is interlace
				 * (TCON_CFG.IS_ITL = 1), it refers to
				 * the space after Field1.
				 * Only Valid when ASD_EN = 1 and
				 * END ¡Ý BEGIN.
				 * Note: Register Value = Actual Value - 1,
				 * Counted by lines. */
	u32 rsvd0:6;            /* - */
	u32 end:10;             /* End of Active Space 0
				 * When the output timing is interlace
				 * (TCON_CFG.IS_ITL = 1), it refers to
				 * the space after Field1.
				 * Only Valid when ASD_EN = 1 and
				 * END ¡Ý BEGIN.
				 * Note: Register Value = Actual Value - 1,
				 * Counted by lines. */
	u32 rsvd1:6;            /* - */
	} bits;
} reg_tcon_gen_as_coord0_t;

/* General Active Space Data 0 Register */
typedef union {
	u32 val;
	struct {
	u32 asd:24;             /* {R,G,B} or {Cr,Y,Cb} value
				 * Data filled into active space 0 */
	u32 rsvd0:8;            /* - */
	} bits;
} reg_tcon_gen_as_dat0_t;

/* General Active Space Coordinate 1 Register */
typedef union {
	u32 val;
	struct {
	u32 begin:10;           /* Begin of Active Space 1
				 * When the output timing is interlace
				 * (TCON_CFG.IS_ITL = 1), it refers to
				 * the space after Field2.
				 * Only Valid when ASD_EN = 1 and
				 * END ¡Ý BEGIN.
				 * Note: Register Value = Actual Value - 1,
				 * Counted by lines. */
	u32 rsvd0:6;            /* - */
	u32 end:10;             /* End of Active Space 1
				 * When the output timing is interlace
				 * (TCON_CFG.IS_ITL = 1), it refers to
				 * the space after Field2.
				 * Only Valid when ASD_EN = 1 and
				 * END ¡Ý BEGIN.
				 * Note: Register Value = Actual Value - 1,
				 * Counted by lines. */
	u32 rsvd1:6;            /* - */
	} bits;
} reg_tcon_gen_as_coord1_t;

/* General Active Space Data 1 Register */
typedef union {
	u32 val;
	struct {
	u32 asd:24;             /* {R,G,B} or {Cr,Y,Cb} value
				 * Data filled into active space 1 */
	u32 rsvd0:8;            /* - */
	} bits;
} reg_tcon_gen_as_dat1_t;

/* General Active Space Coordinate 2 Register */
typedef union {
	u32 val;
	struct {
	u32 begin:10;           /* Begin of Active Space 2
				 * Only Valid when ASD_EN = 1 and
				 * END ¡Ý BEGIN.
				 * Note: Register Value = Actual Value - 1,
				 * Counted by lines. */
	u32 rsvd0:6;            /* - */
	u32 end:10;             /* End of Active Space 2
				 * Only Valid when ASD_EN = 1 and
				 * END ¡Ý BEGIN.
				 * Note: Register Value = Actual Value - 1,
				 * Counted by lines. */
	u32 rsvd1:6;            /* - */
	} bits;
} reg_tcon_gen_as_coord2_t;

/* General Active Space Data 2 Register */
typedef union {
	u32 val;
	struct {
	u32 asd:24;             /* {R,G,B} or {Cr,Y,Cb} value
				 * Data filled into active space 2 */
	u32 rsvd0:8;            /* - */
	} bits;
} reg_tcon_gen_as_dat2_t;

/* RGB Interface Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 ol_seq:2;           /* The Sequence of ODD line for serial RGB
				 * [2¡¯b00]: RGB
				 * [2¡¯b01]: GBR
				 * [2¡¯b10]: BRG
				 * Others: Reserved */
	u32 el_seq:2;           /* The Sequence of EVEN line for serial RGB
				 * [2¡¯b00]: RGB
				 * [2¡¯b01]: GBR
				 * [2¡¯b10]: BRG
				 * Others: Reserved */
	u32 rsvd0:28;           /* - */
	} bits;
} reg_tcon_rgb_if_cfg_t;

#define TCON_RGB_IF_CFG_OL_SEQ_0        0x0
#define TCON_RGB_IF_CFG_OL_SEQ_1        0x1
#define TCON_RGB_IF_CFG_OL_SEQ_2        0x2
#define TCON_RGB_IF_CFG_EL_SEQ_0        0x0
#define TCON_RGB_IF_CFG_EL_SEQ_1        0x1
#define TCON_RGB_IF_CFG_EL_SEQ_2        0x2

/* CPU Interface Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:1;            /* - */
	u32 vsync_cmd:1;        /* Send CPU_SYNC_CMD at every first line
				 * [1¡¯b0]: Do not send
				 * [1¡¯b1]: Send
				 * Note:
				 * Only valid when CPU_IF_CFG.MODE equals to 1
				 */
	u32 hsync_cmd:1;        /* Send CPU_SYNC_CMD at every HSYNC
				 * except for the first line.
				 * [1¡¯b0]: Do not send
				 * [1¡¯b1]: Send
				 * Note:
				 * Only valid when CPU_IF_CFG.MODE equals to 1
				 */
	u32 rsvd1:1;            /* - */
	u32 mode:1;             /* Command Mode or Payload Mode
				 * [1¡¯b0]: Command Mode
				 * [1¡¯b1]: Payload Mode */
	u32 rsvd2:27;           /* - */
	} bits;
} reg_tcon_cpu_if_cfg_t;

#define TCON_CPU_IF_CFG_VSYNC_CMD_0     0x0
#define TCON_CPU_IF_CFG_VSYNC_CMD_1     0x1
#define TCON_CPU_IF_CFG_HSYNC_CMD_0     0x0
#define TCON_CPU_IF_CFG_HSYNC_CMD_1     0x1
#define TCON_CPU_IF_CFG_MODE_0          0x0
#define TCON_CPU_IF_CFG_MODE_1          0x1

/* CPU Vertical Synchronized Command Register */
typedef union {
	u32 val;
	struct {
	u32 dat:18;             /* The value of synchronized command
				 * Send CPU_VSYNC_CMD at every last line of VBP.
				 * Note:
				 * Only valid when CPU_IF_CFG.MODE
				 * equals 1 and CPU_IF_CFG.VSYNC_CMD equals 1.
				 */
	u32 rsvd0:14;           /* - */
	} bits;
} reg_tcon_cpu_vsync_cmd_t;

/* CPU Horizontal Synchronized Command Register */
typedef union {
	u32 val;
	struct {
	u32 dat:18;             /* The value of synchronized command
				 * Send CPU_HSYNC_CMD at every last line
				 * of HSYC except the first line.
				 * Note:
				 * Only valid when CPU_IF_CFG.MODE
				 * equals 1 and CPU_IF_CFG.HSYNC_CMD equals 1.
				 */
	u32 rsvd0:14;           /* - */
	} bits;
} reg_tcon_cpu_hsync_cmd_t;

/* CPU Interface Operation Register */
typedef union {
	u32 val;
	struct {
	u32 wr_op:1;            /* Write Operation
				 * Write ¡®1¡¯ to Start a write operation
				 * on i8080 bus, auto cleared by hardware
				 * when the write operation has been finished.
				 */
	u32 rsvd0:3;            /* - */
	u32 rd_op:1;            /* Read Operation
				 * Write ¡®1¡¯ to Start a read operation
				 * on i8080 bus, auto cleared by hardware
				 * when data has been read into CPU_RD_DAT. */
	u32 rsvd1:3;            /* - */
	u32 rs:1;               /* RS State
				 * [1¡¯b0]: RS low level
				 * [1¡¯b1]: RS high level */
	u32 rsvd2:3;            /* - */
	u32 s_trig:1;           /* Single Trigger
				 * Write ¡®1¡¯ will trigger a frame transfer,
				 * auto cleared by hardware when finished.
				 * Only valid when TRIG_MODE is single mode. */
	u32 rsvd3:19;           /* - */
	} bits;
} reg_tcon_cpu_if_op_t;

#define TCON_CPU_IF_OP_RS_0     0x0
#define TCON_CPU_IF_OP_RS_1     0x1

/* CPU Write Data Register */
typedef union {
	u32 val;
	struct {
	u32 dat:18;             /* Writing to this register will trigger
				 * a write transmission on i8080 bus */
	u32 rsvd0:14;           /* - */
	} bits;
} reg_tcon_cpu_wr_dat_t;

/* CPU Read Data Register */
typedef union {
	u32 val;
	struct {
	u32 dat:18;             /* Reading this register will trigger a
				 * read transmission on i8080 bus. */
	u32 rsvd0:14;           /* - */
	} bits;
} reg_tcon_cpu_rd_dat_t;

/* BT Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 yc_seq:2;           /* YC sequence
				 * [2¡¯b00]:
				 * 8bit: D[7:0]->Cb0,Y0,Cr0,Y1
				 * 16bit: D[15:8]-> Y0,Y1
				 * D[7:0]->Cb0,Cr0
				 * [2¡¯b01]:
				 * 8bit: D[7:0]->Cr0,Y0,Cb0,Y1
				 * 16bit: D[15:8]-> Y0,Y1
				 * D[7:0]->Cr0,Cb0
				 * [2¡¯b10]:
				 * 8bit: D[7:0]-> Y0,Cb0,Y1,Cr0
				 * 16bit: D[15:8]-> Cb0,Cr0
				 * D[7:0]-> Y0,Y1
				 * [2¡¯b11]:
				 * 8bit: D[7:0]-> Y0,Cr0,Y1,Cb0
				 * 16bit: D[15:8]-> Cr0,Cb0
				 * D[7:0]->Y0,Y1 */
	u32 rsvd0:30;           /* - */
	} bits;
} reg_tcon_bt_cfg_t;

#define TCON_BT_CFG_YC_SEQ_0    0x0
#define TCON_BT_CFG_YC_SEQ_1    0x1
#define TCON_BT_CFG_YC_SEQ_2    0x2
#define TCON_BT_CFG_YC_SEQ_3    0x3

/* LVDS Interface Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 clk_pat:7;          /* LVDS Clock lane pattern:
				 * Default ¡®1100011¡¯ */
	u32 rsvd0:1;            /* - */
	u32 mode:1;             /* LVDS Mode
				 * [1¡¯b0]: JEIDA
				 * [1¡¯b1]: VESA or NS */
	u32 rsvd1:3;            /* - */
	u32 polar:1;            /* Polarity for frame Sync for LVDS
				 * [1¡¯b0]: None-inverse
				 * [1¡¯b1]: Inverse */
	u32 rsvd2:3;            /* - */
	u32 clk_adj:1;          /* Enable clock adjustment every frame
				 * [1¡¯b0]: Disable
				 * [1¡¯b1]: Enable */
	u32 rsvd3:7;            /* - */
	u32 lsb_first:1;        /* LSB or MSB first for P2S
				 * [1¡¯b0]: MSB first
				 * [1¡¯b1]: LSB first */
	u32 lane_rvs:1;         /* Lane Reverse
				 * [1¡¯b0]: Non-Reverse
				 * [1¡¯b1]: Reverse */
	u32 dbg_pat:1;          /* Pattern for Debug Mode
				 * [1¡¯b0]: Clock
				 * [1¡¯b1]: PRBS */
	u32 dbg_en:1;           /* Debug Mode Enable
				 * [1¡¯b0]: Disable
				 * [1¡¯b1]: Enable */
	u32 rsvd4:3;            /* - */
	u32 en:1;               /* LVDS Enable
				 * [1¡¯b0]: Disable
				 * [1¡¯b1]: Enable */
	} bits;
} reg_tcon_lvds_if_cfg_t;

#define TCON_LVDS_IF_CFG_MODE_0         0x0
#define TCON_LVDS_IF_CFG_MODE_1         0x1
#define TCON_LVDS_IF_CFG_POLAR_0        0x0
#define TCON_LVDS_IF_CFG_POLAR_1        0x1
#define TCON_LVDS_IF_CFG_CLK_ADJ_0      0x0
#define TCON_LVDS_IF_CFG_CLK_ADJ_1      0x1
#define TCON_LVDS_IF_CFG_LSB_FIRST_0    0x0
#define TCON_LVDS_IF_CFG_LSB_FIRST_1    0x1
#define TCON_LVDS_IF_CFG_LANE_RVS_0     0x0
#define TCON_LVDS_IF_CFG_LANE_RVS_1     0x1
#define TCON_LVDS_IF_CFG_DBG_PAT_0      0x0
#define TCON_LVDS_IF_CFG_DBG_PAT_1      0x1
#define TCON_LVDS_IF_CFG_DBG_EN_0       0x0
#define TCON_LVDS_IF_CFG_DBG_EN_1       0x1
#define TCON_LVDS_IF_CFG_EN_0           0x0
#define TCON_LVDS_IF_CFG_EN_1           0x1

/* LVDS Analog Configuration 0 Register */
typedef union {
	u32 val;
	struct {
	u32 diff:3;             /* Diff-Mode Configuration
				 * [3¡¯b000]: 320mV
				 * [3¡¯b001]: 330mV
				 * [3¡¯b010]: 340mV
				 * [3¡¯b011]: 350mV
				 * [3¡¯b100]: 360mV
				 * [3¡¯b101]: 370mV
				 * [3¡¯b110]: 380mV
				 * [3¡¯b111]: 390mV */
	u32 rsvd0:1;            /* - */
	u32 com:3;              /* Com-Mode Configuration
				 * [3¡¯b000]: 1080mV
				 * [3¡¯b001]: 1120mV
				 * [3¡¯b010]: 1160mV
				 * [3¡¯b011]: 1200mV
				 * [3¡¯b100]: 1240mV
				 * [3¡¯b101]: 1280mV
				 * [3¡¯b110]: 1320mV
				 * [3¡¯b111]: 1360mV */
	u32 rsvd1:1;            /* - */
	u32 d0_pol:1;           /* Differential Polarity for Data Lane 0
				 * [1¡¯b0]: Non-Inverse
				 * [1¡¯b1]: Inverse */
	u32 d1_pol:1;           /* Differential Polarity for Data Lane 1
				 * [1¡¯b0]: Non-Inverse
				 * [1¡¯b1]: Inverse */
	u32 d2_pol:1;           /* Differential Polarity for Data Lane 2
				 * [1¡¯b0]: Non-Inverse
				 * [1¡¯b1]: Inverse */
	u32 d3_pol:1;           /* Differential Polarity for Data Lane 3
				 * [1¡¯b0]: Non-Inverse
				 * [1¡¯b1]: Inverse */
	u32 ck_pol:1;           /* Differential Polarity for Clock Lane
				 * [1¡¯b0]: Non-Inverse
				 * [1¡¯b1]: Inverse */
	u32 rsvd2:3;            /* - */
	u32 d0_en:1;            /* Enable for Data Lane 0
				 * [1¡¯b0]: Disable
				 * [1¡¯b1]: Enable */
	u32 d1_en:1;            /* Enable for Data Lane 1
				 * [1¡¯b0]: Disable
				 * [1¡¯b1]: Enable */
	u32 d2_en:1;            /* Enable for Data Lane 2
				 * [1¡¯b0]: Disable
				 * [1¡¯b1]: Enable */
	u32 d3_en:1;            /* Enable for Data Lane 3
				 * [1¡¯b0]: Disable
				 * [1¡¯b1]: Enable */
	u32 ck_en:1;            /* Enable for Clock Lane
				 * [1¡¯b0]: Disable
				 * [1¡¯b1]: Enable */
	u32 test_sel:3;         /* Analog Signal output for test
				 * [3¡¯b000]: Disable
				 * [3¡¯b001]: isrcrx_ratio
				 * [3¡¯b010]: isrcrx_5u
				 * [3¡¯b011]: vref
				 * [3¡¯b100]: vbn
				 * [3¡¯b101]: vbp
				 * [3¡¯b110]: hg
				 * [3¡¯b111]: hv */
	u32 bias_d0:1;          /* Bias for Data Lane 0
				 * [1¡¯b0]: Local cmfb Bias
				 * [1¡¯b1]: Global Replica Bias */
	u32 bias_d1:1;          /* Bias for Data Lane 1
				 * [1¡¯b0]: Local cmfb Bias
				 * [1¡¯b1]: Global Replica Bias */
	u32 bias_d2:1;          /* Bias for Data Lane 2
				 * [1¡¯b0]: Local cmfb Bias
				 * [1¡¯b1]: Global Replica Bias */
	u32 bias_d3:1;          /* Bias for Data Lane 3
				 * [1¡¯b0]: Local cmfb Bias
				 * [1¡¯b1]: Global Replica Bias */
	u32 bias_ck:1;          /* Bias for Clock Lane
				 * [1¡¯b0]: Local cmfb Bias
				 * [1¡¯b1]: Global Replica Bias */
	u32 rsvd3:1;            /* - */
	u32 bias_sel:1;         /* Local Bias for diff-mode and com-mode
				 * configuration
				 * [1¡¯b0]: Use Local Bias
				 * [1¡¯b1]: Use Bias from REF */
	u32 tm_en:1;            /* Test Mode Enable for PLL Clock
				 * [1¡¯b0]: Disable
				 * [1¡¯b1]: Enable
				 * When set to ¡®1¡¯, the LVDS will output
				 * backdoor clock of the PLL. */
	} bits;
} reg_tcon_lvds_anl_cfg0_t;

#define TCON_LVDS_ANL_CFG0_DIFF_0       0x0
#define TCON_LVDS_ANL_CFG0_DIFF_1       0x1
#define TCON_LVDS_ANL_CFG0_DIFF_2       0x2
#define TCON_LVDS_ANL_CFG0_DIFF_3       0x3
#define TCON_LVDS_ANL_CFG0_DIFF_4       0x4
#define TCON_LVDS_ANL_CFG0_DIFF_5       0x5
#define TCON_LVDS_ANL_CFG0_DIFF_6       0x6
#define TCON_LVDS_ANL_CFG0_DIFF_7       0x7
#define TCON_LVDS_ANL_CFG0_COM_0        0x0
#define TCON_LVDS_ANL_CFG0_COM_1        0x1
#define TCON_LVDS_ANL_CFG0_COM_2        0x2
#define TCON_LVDS_ANL_CFG0_COM_3        0x3
#define TCON_LVDS_ANL_CFG0_COM_4        0x4
#define TCON_LVDS_ANL_CFG0_COM_5        0x5
#define TCON_LVDS_ANL_CFG0_COM_6        0x6
#define TCON_LVDS_ANL_CFG0_COM_7        0x7
#define TCON_LVDS_ANL_CFG0_D0_POL_0     0x0
#define TCON_LVDS_ANL_CFG0_D0_POL_1     0x1
#define TCON_LVDS_ANL_CFG0_D1_POL_0     0x0
#define TCON_LVDS_ANL_CFG0_D1_POL_1     0x1
#define TCON_LVDS_ANL_CFG0_D2_POL_0     0x0
#define TCON_LVDS_ANL_CFG0_D2_POL_1     0x1
#define TCON_LVDS_ANL_CFG0_D3_POL_0     0x0
#define TCON_LVDS_ANL_CFG0_D3_POL_1     0x1
#define TCON_LVDS_ANL_CFG0_CK_POL_0     0x0
#define TCON_LVDS_ANL_CFG0_CK_POL_1     0x1
#define TCON_LVDS_ANL_CFG0_D0_EN_0      0x0
#define TCON_LVDS_ANL_CFG0_D0_EN_1      0x1
#define TCON_LVDS_ANL_CFG0_D1_EN_0      0x0
#define TCON_LVDS_ANL_CFG0_D1_EN_1      0x1
#define TCON_LVDS_ANL_CFG0_D2_EN_0      0x0
#define TCON_LVDS_ANL_CFG0_D2_EN_1      0x1
#define TCON_LVDS_ANL_CFG0_D3_EN_0      0x0
#define TCON_LVDS_ANL_CFG0_D3_EN_1      0x1
#define TCON_LVDS_ANL_CFG0_CK_EN_0      0x0
#define TCON_LVDS_ANL_CFG0_CK_EN_1      0x1
#define TCON_LVDS_ANL_CFG0_TEST_SEL_0   0x0
#define TCON_LVDS_ANL_CFG0_TEST_SEL_1   0x1
#define TCON_LVDS_ANL_CFG0_TEST_SEL_2   0x2
#define TCON_LVDS_ANL_CFG0_TEST_SEL_3   0x3
#define TCON_LVDS_ANL_CFG0_TEST_SEL_4   0x4
#define TCON_LVDS_ANL_CFG0_TEST_SEL_5   0x5
#define TCON_LVDS_ANL_CFG0_TEST_SEL_6   0x6
#define TCON_LVDS_ANL_CFG0_TEST_SEL_7   0x7
#define TCON_LVDS_ANL_CFG0_BIAS_D0_0    0x0
#define TCON_LVDS_ANL_CFG0_BIAS_D0_1    0x1
#define TCON_LVDS_ANL_CFG0_BIAS_D1_0    0x0
#define TCON_LVDS_ANL_CFG0_BIAS_D1_1    0x1
#define TCON_LVDS_ANL_CFG0_BIAS_D2_0    0x0
#define TCON_LVDS_ANL_CFG0_BIAS_D2_1    0x1
#define TCON_LVDS_ANL_CFG0_BIAS_D3_0    0x0
#define TCON_LVDS_ANL_CFG0_BIAS_D3_1    0x1
#define TCON_LVDS_ANL_CFG0_BIAS_CK_0    0x0
#define TCON_LVDS_ANL_CFG0_BIAS_CK_1    0x1
#define TCON_LVDS_ANL_CFG0_BIAS_SEL_0   0x0
#define TCON_LVDS_ANL_CFG0_BIAS_SEL_1   0x1
#define TCON_LVDS_ANL_CFG0_TM_EN_0      0x0
#define TCON_LVDS_ANL_CFG0_TM_EN_1      0x1

/* CSC Coefficient 0 Register */
typedef union {
	u32 val;
	struct {
	u32 coef:14;            /* CSC coefficient 0 */
	u32 rsvd0:18;           /* - */
	} bits;
} reg_tcon_csc_c0_t;

/* CSC Coefficient 1 Register */
typedef union {
	u32 val;
	struct {
	u32 coef:14;            /* CSC coefficient 1 */
	u32 rsvd0:18;           /* - */
	} bits;
} reg_tcon_csc_c1_t;

/* CSC Coefficient 2 Register */
typedef union {
	u32 val;
	struct {
	u32 coef:14;            /* CSC coefficient 2 */
	u32 rsvd0:18;           /* - */
	} bits;
} reg_tcon_csc_c2_t;

/* CSC Coefficient 3 Register */
typedef union {
	u32 val;
	struct {
	u32 coef:14;            /* CSC coefficient 3 */
	u32 rsvd0:18;           /* - */
	} bits;
} reg_tcon_csc_c3_t;

/* CSC Coefficient 4 Register */
typedef union {
	u32 val;
	struct {
	u32 coef:14;            /* CSC coefficient 4 */
	u32 rsvd0:18;           /* - */
	} bits;
} reg_tcon_csc_c4_t;

/* CSC Coefficient 5 Register */
typedef union {
	u32 val;
	struct {
	u32 coef:14;            /* CSC coefficient 5 */
	u32 rsvd0:18;           /* - */
	} bits;
} reg_tcon_csc_c5_t;

/* CSC Coefficient 6 Register */
typedef union {
	u32 val;
	struct {
	u32 coef:14;            /* CSC coefficient 6 */
	u32 rsvd0:18;           /* - */
	} bits;
} reg_tcon_csc_c6_t;

/* CSC Coefficient 7 Register */
typedef union {
	u32 val;
	struct {
	u32 coef:14;            /* CSC coefficient 7 */
	u32 rsvd0:18;           /* - */
	} bits;
} reg_tcon_csc_c7_t;

/* CSC Coefficient 8 Register */
typedef union {
	u32 val;
	struct {
	u32 coef:14;            /* CSC coefficient 8 */
	u32 rsvd0:18;           /* - */
	} bits;
} reg_tcon_csc_c8_t;

/* CSC Coefficient 9 Register */
typedef union {
	u32 val;
	struct {
	u32 coef:14;            /* CSC coefficient 9 */
	u32 rsvd0:18;           /* - */
	} bits;
} reg_tcon_csc_c9_t;

/* CSC Coefficient 10 Register */
typedef union {
	u32 val;
	struct {
	u32 coef:14;            /* CSC coefficient 10 */
	u32 rsvd0:18;           /* - */
	} bits;
} reg_tcon_csc_c10_t;

/* CSC Coefficient 11 Register */
typedef union {
	u32 val;
	struct {
	u32 coef:14;            /* CSC coefficient 11 */
	u32 rsvd0:18;           /* - */
	} bits;
} reg_tcon_csc_c11_t;

/* CSC CLIP 0 Register */
typedef union {
	u32 val;
	struct {
	u32 min:8;              /* Min Value for R channel */
	u32 rsvd0:8;            /* - */
	u32 max:8;              /* Max Value for R channel */
	u32 rsvd1:8;            /* - */
	} bits;
} reg_tcon_csc_clip0_t;

/* CSC CLIP 1 Register */
typedef union {
	u32 val;
	struct {
	u32 min:8;              /* Min Value for G channel */
	u32 rsvd0:8;            /* - */
	u32 max:8;              /* Max Value for G channel */
	u32 rsvd1:8;            /* - */
	} bits;
} reg_tcon_csc_clip1_t;

/* CSC CLIP 2 Register */
typedef union {
	u32 val;
	struct {
	u32 min:8;              /* Min Value for B channel */
	u32 rsvd0:8;            /* - */
	u32 max:8;              /* Max Value for B channel */
	u32 rsvd1:8;            /* - */
	} bits;
} reg_tcon_csc_clip2_t;

/* FRC Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 r_mode:1;           /* Dither Mode for Red
				 * [1¡¯b0]: 8bit to 6bit dithering
				 * [1¡¯b1]: 8bit to 5bit dithering */
	u32 g_mode:1;           /* Dither Mode for Green
				 * [1¡¯b0]: 8bit to 6bit dithering
				 * [1¡¯b1]: 8bit to 5bit dithering */
	u32 b_mode:1;           /* Dither Mode for Blue
				 * [1¡¯b0]: 8bit to 6bit dithering
				 * [1¡¯b1]: 8bit to 5bit dithering */
	u32 rsvd0:1;            /* - */
	u32 tm:2;               /* Test Mode for Dither
				 * [2¡¯b00]: Normal Mode
				 * [2¡¯b01]: Left(Non-Dither) and Right(Dither)
				 * [2¡¯b10]: Up(Non-Dither) and Down(Dither)
				 * [2¡¯b11]: Reserved */
	u32 rsvd1:26;           /* - */
	} bits;
} reg_tcon_frc_cfg_t;

#define TCON_FRC_CFG_R_MODE_0   0x0
#define TCON_FRC_CFG_R_MODE_1   0x1
#define TCON_FRC_CFG_G_MODE_0   0x0
#define TCON_FRC_CFG_G_MODE_1   0x1
#define TCON_FRC_CFG_B_MODE_0   0x0
#define TCON_FRC_CFG_B_MODE_1   0x1
#define TCON_FRC_CFG_TM_0       0x0
#define TCON_FRC_CFG_TM_1       0x1
#define TCON_FRC_CFG_TM_2       0x2
#define TCON_FRC_CFG_TM_3       0x3

/* FRC Seed 0 Register */
typedef union {
	u32 val;
	struct {
	u32 r_pix:25;           /* Seed for Red in pixel clock */
	u32 rsvd0:7;            /* 0 */
	} bits;
} reg_tcon_frc_seed0_t;

/* FRC Seed 1 Register */
typedef union {
	u32 val;
	struct {
	u32 g_pix:25;           /* Seed for Green in pixel clock */
	u32 rsvd0:7;            /* 0 */
	} bits;
} reg_tcon_frc_seed1_t;

/* FRC Seed 2 Register */
typedef union {
	u32 val;
	struct {
	u32 b_pix:25;           /* Seed for Blue in pixel clock */
	u32 rsvd0:7;            /* 0 */
	} bits;
} reg_tcon_frc_seed2_t;

/* FRC Seed 3 Register */
typedef union {
	u32 val;
	struct {
	u32 r_line:12;          /* Seed for Red in line */
	u32 rsvd0:20;           /* 0 */
	} bits;
} reg_tcon_frc_seed3_t;

/* FRC Seed 4 Register */
typedef union {
	u32 val;
	struct {
	u32 g_line:12;          /* Seed for Green in line */
	u32 rsvd0:20;           /* 0 */
	} bits;
} reg_tcon_frc_seed4_t;

/* FRC Seed 5 Register */
typedef union {
	u32 val;
	struct {
	u32 b_line:12;          /* Seed for Blue in line */
	u32 rsvd0:20;           /* 0 */
	} bits;
} reg_tcon_frc_seed5_t;

/* FRC LUT 0 Register */
typedef union {
	u32 val;
	struct {
	u32 lsb0:16;            /* Dither Sequence when LSB equals to 0 */
	u32 lsb1:16;            /* Dither Sequence when LSB equals to 1 */
	} bits;
} reg_tcon_frc_lut0_t;

/* FRC LUT 1 Register */
typedef union {
	u32 val;
	struct {
	u32 lsb2:16;            /* Dither Sequence when LSB equals to 2 */
	u32 lsb3:16;            /* Dither Sequence when LSB equals to 3 */
	} bits;
} reg_tcon_frc_lut1_t;

/* FRC LUT 2 Register */
typedef union {
	u32 val;
	struct {
	u32 lsb4:16;            /* Dither Sequence when LSB equals to 4 */
	u32 lsb5:16;            /* Dither Sequence when LSB equals to 5 */
	} bits;
} reg_tcon_frc_lut2_t;

/* FRC LUT 3 Register */
typedef union {
	u32 val;
	struct {
	u32 lsb6:16;            /* Dither Sequence when LSB equals to 6 */
	u32 lsb7:16;            /* Dither Sequence when LSB equals to 7 */
	} bits;
} reg_tcon_frc_lut3_t;

/* reg_tcon_t bank */
typedef struct tag_tcon {
	reg_tcon_ver_t             ver;            /* 0000 */
	reg_tcon_ftr_t             ftr;            /* 0004 */
	u32                        res0[2];
	reg_tcon_tcon_ctrl_t       tcon_ctrl;      /* 0010 */
	reg_tcon_tcon_cfg_t        tcon_cfg;       /* 0014 */
	reg_tcon_vsync_t           vsync;          /* 0018 */
	reg_tcon_io_cfg_t          io_cfg;         /* 001C */
	reg_tcon_fifo_ctrl_t       fifo_ctrl;      /* 0020 */
	reg_tcon_fifo_sta_t        fifo_sta;       /* 0024 */
	reg_tcon_fifo_data_t       fifo_data;      /* 0028 */
	reg_tcon_bg_color_t        bg_color;       /* 002C */
	reg_tcon_int_en_t          int_en;         /* 0030 */
	reg_tcon_int_pd_t          int_pd;         /* 0034 */
	reg_tcon_int_clr_t         int_clr;        /* 0038 */
	reg_tcon_line_cnt_t        line_cnt;       /* 003C */
	reg_tcon_gen_htim0_t       gen_htim0;      /* 0040 */
	reg_tcon_gen_htim1_t       gen_htim1;      /* 0044 */
	reg_tcon_gen_vtim0_t       gen_vtim0;      /* 0048 */
	reg_tcon_gen_vtim1_t       gen_vtim1;      /* 004C */
	reg_tcon_gen_vtim2_t       gen_vtim2;      /* 0050 */
	reg_tcon_gen_vtim3_t       gen_vtim3;      /* 0054 */
	reg_tcon_gen_in_res0_t     gen_in_res0;    /* 0058 */
	reg_tcon_gen_in_res1_t     gen_in_res1;    /* 005C */
	reg_tcon_gen_as_coord0_t   gen_as_coord0;  /* 0060 */
	reg_tcon_gen_as_dat0_t     gen_as_dat0;    /* 0064 */
	reg_tcon_gen_as_coord1_t   gen_as_coord1;  /* 0068 */
	reg_tcon_gen_as_dat1_t     gen_as_dat1;    /* 006C */
	reg_tcon_gen_as_coord2_t   gen_as_coord2;  /* 0070 */
	reg_tcon_gen_as_dat2_t     gen_as_dat2;    /* 0074 */
	u32                        res1[2];
	reg_tcon_rgb_if_cfg_t      rgb_if_cfg;     /* 0080 */
	reg_tcon_cpu_if_cfg_t      cpu_if_cfg;     /* 0084 */
	reg_tcon_cpu_vsync_cmd_t   cpu_vsync_cmd;  /* 0088 */
	reg_tcon_cpu_hsync_cmd_t   cpu_hsync_cmd;  /* 008C */
	reg_tcon_cpu_if_op_t       cpu_if_op;      /* 0090 */
	u32                        res2[1];
	reg_tcon_cpu_wr_dat_t      cpu_wr_dat;     /* 0098 */
	reg_tcon_cpu_rd_dat_t      cpu_rd_dat;     /* 009C */
	reg_tcon_bt_cfg_t          bt_cfg;         /* 00A0 */
	u32                        res3[11];
	reg_tcon_lvds_if_cfg_t     lvds_if_cfg;    /* 00D0 */
	reg_tcon_lvds_anl_cfg0_t   lvds_anl_cfg0;  /* 00D4 */
	u32                        res4[10];
	reg_tcon_csc_c0_t          csc_c0;         /* 0100 */
	reg_tcon_csc_c1_t          csc_c1;         /* 0104 */
	reg_tcon_csc_c2_t          csc_c2;         /* 0108 */
	reg_tcon_csc_c3_t          csc_c3;         /* 010C */
	reg_tcon_csc_c4_t          csc_c4;         /* 0110 */
	reg_tcon_csc_c5_t          csc_c5;         /* 0114 */
	reg_tcon_csc_c6_t          csc_c6;         /* 0118 */
	reg_tcon_csc_c7_t          csc_c7;         /* 011C */
	reg_tcon_csc_c8_t          csc_c8;         /* 0120 */
	reg_tcon_csc_c9_t          csc_c9;         /* 0124 */
	reg_tcon_csc_c10_t         csc_c10;        /* 0128 */
	reg_tcon_csc_c11_t         csc_c11;        /* 012C */
	reg_tcon_csc_clip0_t       csc_clip0;      /* 0130 */
	reg_tcon_csc_clip1_t       csc_clip1;      /* 0134 */
	reg_tcon_csc_clip2_t       csc_clip2;      /* 0138 */
	u32                        res5[1];
	reg_tcon_frc_cfg_t         frc_cfg;        /* 0140 */
	reg_tcon_frc_seed0_t       frc_seed0;      /* 0144 */
	reg_tcon_frc_seed1_t       frc_seed1;      /* 0148 */
	reg_tcon_frc_seed2_t       frc_seed2;      /* 014C */
	reg_tcon_frc_seed3_t       frc_seed3;      /* 0150 */
	reg_tcon_frc_seed4_t       frc_seed4;      /* 0154 */
	reg_tcon_frc_seed5_t       frc_seed5;      /* 0158 */
	u32                        res6[1];
	reg_tcon_frc_lut0_t        frc_lut0;       /* 0160 */
	reg_tcon_frc_lut1_t        frc_lut1;       /* 0164 */
	reg_tcon_frc_lut2_t        frc_lut2;       /* 0168 */
	reg_tcon_frc_lut3_t        frc_lut3;       /* 016C */
} reg_tcon_t;

#define TCON_MAX_INST (2)

#define OFS_TCON_VER			(0x00000000)
#define OFS_TCON_FTR			(0x00000004)
#define OFS_TCON_TCON_CTRL		(0x00000010)
#define OFS_TCON_TCON_CFG		(0x00000014)
#define OFS_TCON_VSYNC			(0x00000018)
#define OFS_TCON_IO_CFG			(0x0000001C)
#define OFS_TCON_FIFO_CTRL		(0x00000020)
#define OFS_TCON_FIFO_STA		(0x00000024)
#define OFS_TCON_FIFO_DATA		(0x00000028)
#define OFS_TCON_BG_COLOR		(0x0000002C)
#define OFS_TCON_INT_EN			(0x00000030)
#define OFS_TCON_INT_PD			(0x00000034)
#define OFS_TCON_INT_CLR		(0x00000038)
#define OFS_TCON_LINE_CNT		(0x0000003C)
#define OFS_TCON_GEN_HTIM0		(0x00000040)
#define OFS_TCON_GEN_HTIM1		(0x00000044)
#define OFS_TCON_GEN_VTIM0		(0x00000048)
#define OFS_TCON_GEN_VTIM1		(0x0000004C)
#define OFS_TCON_GEN_VTIM2		(0x00000050)
#define OFS_TCON_GEN_VTIM3		(0x00000054)
#define OFS_TCON_GEN_IN_RES0	(0x00000058)
#define OFS_TCON_GEN_IN_RES1	(0x0000005C)
#define OFS_TCON_GEN_AS_COORD0	(0x00000060)
#define OFS_TCON_GEN_AS_DAT0	(0x00000064)
#define OFS_TCON_GEN_AS_COORD1	(0x00000068)
#define OFS_TCON_GEN_AS_DAT1	(0x0000006C)
#define OFS_TCON_GEN_AS_COORD2	(0x00000070)
#define OFS_TCON_GEN_AS_DAT2	(0x00000074)
#define OFS_TCON_RGB_IF_CFG	(0x00000080)
#define OFS_TCON_CPU_IF_CFG		(0x00000084)
#define OFS_TCON_CPU_VSYNC_CMD	(0x00000088)
#define OFS_TCON_CPU_HSYNC_CMD	(0x0000008C)
#define OFS_TCON_CPU_IF_OP		(0x00000090)
#define OFS_TCON_CPU_WR_DAT		(0x00000098)
#define OFS_TCON_CPU_RD_DAT		(0x0000009C)
#define OFS_TCON_BT_CFG			(0x000000A0)
#define OFS_TCON_LVDS_IF_CFG	(0x000000D0)
#define OFS_TCON_LVDS_ANL_CFG0	(0x000000D4)
#define OFS_TCON_CSC_C0			(0x00000100)
#define OFS_TCON_CSC_C1			(0x00000104)
#define OFS_TCON_CSC_C2			(0x00000108)
#define OFS_TCON_CSC_C3			(0x0000010C)
#define OFS_TCON_CSC_C4			(0x00000110)
#define OFS_TCON_CSC_C5			(0x00000114)
#define OFS_TCON_CSC_C6			(0x00000118)
#define OFS_TCON_CSC_C7			(0x0000011C)
#define OFS_TCON_CSC_C8			(0x00000120)
#define OFS_TCON_CSC_C9			(0x00000124)
#define OFS_TCON_CSC_C10		(0x00000128)
#define OFS_TCON_CSC_C11		(0x0000012C)
#define OFS_TCON_CSC_CLIP0		(0x00000130)
#define OFS_TCON_CSC_CLIP1		(0x00000134)
#define OFS_TCON_CSC_CLIP2		(0x00000138)
#define OFS_TCON_FRC_CFG		(0x00000140)
#define OFS_TCON_FRC_SEED0		(0x00000144)
#define OFS_TCON_FRC_SEED1		(0x00000148)
#define OFS_TCON_FRC_SEED2		(0x0000014C)
#define OFS_TCON_FRC_SEED3		(0x00000150)
#define OFS_TCON_FRC_SEED4		(0x00000154)
#define OFS_TCON_FRC_SEED5		(0x00000158)
#define OFS_TCON_FRC_LUT0		(0x00000160)
#define OFS_TCON_FRC_LUT1		(0x00000164)
#define OFS_TCON_FRC_LUT2		(0x00000168)
#define OFS_TCON_FRC_LUT3		(0x0000016C)
#define OFS_TCON_GAMMA_TBL		(0x00001000)

/* Enable/Disable */
#define TCON_DISABLE			(0)
#define TCON_ENABLE			(1)
/* NTSC or PAL */
#define TCON_NTSC			(0)
#define TCON_PAL			(1)
/* IO Invert */
#define TCON_IO_NON_INVERT		(0)
#define TCON_IO_INVERT			(1)
/* Start and finish operation */
#define	TCON_REG_START			(1)
#define TCON_REG_FINISH			(0)

/* FIFO Mode */
#define TCON_REG_FIFO_NORMAL		TCON_TCON_CFG_FIFO_MODE_0
#define TCON_REG_FIFO_TWO_HALVES	TCON_TCON_CFG_FIFO_MODE_1
#define TCON_REG_FIFO_EC		TCON_TCON_CFG_FIFO_MODE_2

/* VBI Trigger Condiction */
#define TCON_REG_VBI_VFP		TCON_VSYNC_VBI_TRIG_0
#define TCON_REG_VBI_VSW		TCON_VSYNC_VBI_TRIG_1
#define TCON_REG_VBI_VBP		TCON_VSYNC_VBI_TRIG_2
#define TCON_REG_VBI_VACT		TCON_VSYNC_VBI_TRIG_3
/* Frmae Trigger Source */
#define TCON_REG_TRIG_INT		TCON_TCON_CFG_TRIG_SRC_0
#define TCON_REG_TRIG_TE		TCON_TCON_CFG_TRIG_SRC_1
#define TCON_REG_TRIG_EXT		TCON_TCON_CFG_TRIG_SRC_2
/* Frame Trigger Mode */
#define TCON_REG_TRIG_AUTO		TCON_TCON_CFG_TRIG_MODE_0
#define TCON_REG_TRIG_SINGLE		TCON_TCON_CFG_TRIG_MODE_1
/* Output Interface Selection */
#define TCON_REG_PRGB			TCON_TCON_CFG_OUT_SEL_0
#define TCON_REG_SRGB_RGB		TCON_TCON_CFG_OUT_SEL_4
#define TCON_REG_SRGB_DRGB		TCON_TCON_CFG_OUT_SEL_5
#define TCON_REG_SRGB_RGBD		TCON_TCON_CFG_OUT_SEL_6
#define TCON_REG_CPU_18BIT_RGB666	TCON_TCON_CFG_OUT_SEL_10
#define TCON_REG_CPU_16BIT_RGB888	TCON_TCON_CFG_OUT_SEL_14
#define TCON_REG_CPU_16BIT_RGB565	TCON_TCON_CFG_OUT_SEL_15
#define TCON_REG_CPU_9BIT_RGB666	TCON_TCON_CFG_OUT_SEL_18
#define TCON_REG_CPU_8BIT_RGB565	TCON_TCON_CFG_OUT_SEL_1C
#define TCON_REG_CPU_8BIT_RGB888	TCON_TCON_CFG_OUT_SEL_1D
#define TCON_REG_BT601_1120_16BIT	TCON_TCON_CFG_OUT_SEL_21
#define TCON_REG_BT1120_8BIT		TCON_TCON_CFG_OUT_SEL_22
#define TCON_REG_BT601_656_8BIT		TCON_TCON_CFG_OUT_SEL_23
#define TCON_REG_TVE_NTSC		TCON_TCON_CFG_OUT_SEL_24
#define TCON_REG_TVE_PAL		TCON_TCON_CFG_OUT_SEL_25
/* Serial RGB Sequence */
#define TCON_REG_SRGB_SEQ_RGB		TCON_RGB_IF_CFG_OL_SEQ_0
#define TCON_REG_SRGB_SEQ_GBR		TCON_RGB_IF_CFG_OL_SEQ_1
#define TCON_REG_SRGB_SEQ_BRG		TCON_RGB_IF_CFG_OL_SEQ_2
/* CPU mode */
#define TCON_REG_CPU_MODE_CMD		TCON_CPU_IF_CFG_MODE_0
#define TCON_REG_CPU_MODE_PAYLOAD	TCON_CPU_IF_CFG_MODE_1
/* CPU RS */
#define TCON_REG_CPU_RS_CMD		TCON_CPU_IF_OP_RS_0
#define TCON_REG_CPU_RS_DATA		TCON_CPU_IF_OP_RS_1
/* Data Source */
#define TCON_REG_SRC_DISP_CTL		TCON_TCON_CFG_SRC_SEL_0
#define TCON_REG_SRC_DMA		TCON_TCON_CFG_SRC_SEL_1
#define TCON_REG_SRC_COLOR_BAR		TCON_TCON_CFG_SRC_SEL_2
#define TCON_REG_SRC_GREY_SCALE		TCON_TCON_CFG_SRC_SEL_3
#define TCON_REG_SRC_BLACK_WHITE	TCON_TCON_CFG_SRC_SEL_4
#define TCON_REG_SRC_GRID		TCON_TCON_CFG_SRC_SEL_5
#define TCON_REG_SRC_CHECKER_BOARD	TCON_TCON_CFG_SRC_SEL_6
#define TCON_REG_SRC_BG_COLOR		TCON_TCON_CFG_SRC_SEL_7
/* LVDS Mode */
#define TCON_REG_LVDS_JEIDA		TCON_LVDS_IF_CFG_MODE_0
#define TCON_REG_LVDS_VESA		TCON_LVDS_IF_CFG_MODE_1
/* TCON BT YC DOWM SAMPLE */
#define TCON_REG_BT_YC_DS_DIRECTLY		TCON_TCON_CFG_DS_MODE_0
#define TCON_REG_BT_YC_DS_AVERAGED		TCON_TCON_CFG_DS_MODE_1
/* TCON BT YC SEQUENCE */
#define TCON_REG_BT_YC_SEQ_CB0Y0CR0Y1		TCON_BT_CFG_YC_SEQ_0
#define TCON_REG_BT_YC_SEQ_CR0Y0CB0Y1		TCON_BT_CFG_YC_SEQ_1
#define TCON_REG_BT_YC_SEQ_Y0CB0Y1CR0		TCON_BT_CFG_YC_SEQ_2
#define TCON_REG_BT_YC_SEQ_Y0CR0Y1CB0		TCON_BT_CFG_YC_SEQ_3
/* FRC Mode */
#define TCON_REG_FRC_6BIT		TCON_FRC_CFG_R_MODE_0
#define TCON_REG_FRC_5BIT		TCON_FRC_CFG_R_MODE_1
/* GAMMA SRAM */
#define TCON_REG_GAMMA_TBL_HW		TCON_TCON_CFG_SRAM_ACS_0
#define TCON_REG_GAMMA_TBL_SW		TCON_TCON_CFG_SRAM_ACS_1

#endif /* _CSP_DOSS_TCON_REG_V1_1_H_ */
