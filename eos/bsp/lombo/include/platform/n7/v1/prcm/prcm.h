/* prcm.h */

#ifndef ___PRCM___H___
#define ___PRCM___H___

#define BASE_PRCM                0x0400A000

#define VA_PRCM_VER                          (0x00000000 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_AHB_GAT0                     (0x00000200 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_AHB_GAT1                     (0x00000204 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_AHB_RST0                     (0x00000220 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_AHB_RST1                     (0x00000224 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_APB_GAT0                     (0x00000280 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_APB_RST0                     (0x000002A0 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_APB_GAT1                     (0x000002C0 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_APB_RST1                     (0x000002E0 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_AXI_GAT                      (0x00000340 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_HFEOSC_CTRL                  (0x00000420 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_MBIAS_CTRL                   (0x00000424 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_CPU_PLL_EN                   (0x00000460 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_CPU_PLL_FAC                  (0x00000468 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_CPU_PLL_TUNE0                (0x0000046C + BASE_PRCM + VA_PRCM)
#define VA_PRCM_CPU_PLL_TEN                  (0x00000474 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_CPU_PLL_STAT                 (0x00000478 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_CPU_PLL_MOD                  (0x0000047C + BASE_PRCM + VA_PRCM)
#define VA_PRCM_CPU_PLL_NFRAC                (0x00000480 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_CPU_PLL_TUNE2                (0x0000048C + BASE_PRCM + VA_PRCM)
#define VA_PRCM_SDRAM_PLL_EN                 (0x000004A0 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_SDRAM_PLL_FAC                (0x000004A4 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_SDRAM_PLL_TUNE0              (0x000004A8 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_SDRAM_PLL_TEN                (0x000004AC + BASE_PRCM + VA_PRCM)
#define VA_PRCM_SDRAM_PLL_STAT               (0x000004B0 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_SDRAM_PLL_MOD                (0x000004B4 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_SDRAM_PLL_NFRAC              (0x000004B8 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_SDRAM_PLL_TUNE1              (0x000004BC + BASE_PRCM + VA_PRCM)
#define VA_PRCM_SDRAM_PLL_TUNE2              (0x000004C0 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_PERH0_PLL_EN                 (0x000004E0 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_PERH0_PLL_FAC                (0x000004E4 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_PERH0_PLL_TUNE0              (0x000004E8 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_PERH0_PLL_TEN                (0x000004EC + BASE_PRCM + VA_PRCM)
#define VA_PRCM_PERH0_PLL_STAT               (0x000004F0 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_PERH0_PLL_MOD                (0x000004F4 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_PERH0_PLL_NFRAC              (0x000004F8 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_AUDIO_PLL_EN                 (0x00000520 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_AUDIO_PLL_FAC                (0x00000524 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_AUDIO_PLL_TUNE0              (0x00000528 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_AUDIO_PLL_TEN                (0x0000052C + BASE_PRCM + VA_PRCM)
#define VA_PRCM_AUDIO_PLL_STAT               (0x00000530 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_AUDIO_PLL_MOD                (0x00000534 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_AUDIO_PLL_NFRAC              (0x00000538 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_VC_PLL_EN                    (0x000005A0 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_VC_PLL_FAC                   (0x000005A4 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_VC_PLL_TUNE0                 (0x000005A8 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_VC_PLL_TEN                   (0x000005AC + BASE_PRCM + VA_PRCM)
#define VA_PRCM_VC_PLL_STAT                  (0x000005B0 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_VC_PLL_MOD                   (0x000005B4 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_VC_PLL_NFRAC                 (0x000005B8 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_PERH1_PLL_EN                 (0x000005E0 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_PERH1_PLL_FAC                (0x000005E4 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_PERH1_PLL_TUNE0              (0x000005E8 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_PERH1_PLL_TEN                (0x000005EC + BASE_PRCM + VA_PRCM)
#define VA_PRCM_PERH1_PLL_STAT               (0x000005F0 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_PERH1_PLL_MOD                (0x000005F4 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_PERH1_PLL_NFRAC              (0x000005F8 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_PERH1_PLL_TUNE1              (0x000005FC + BASE_PRCM + VA_PRCM)
#define VA_PRCM_DISP_PLL_EN                  (0x00000620 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_DISP_PLL_FAC                 (0x00000624 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_DISP_PLL_TUNE0               (0x00000628 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_DISP_PLL_TEN                 (0x0000062C + BASE_PRCM + VA_PRCM)
#define VA_PRCM_DISP_PLL_STAT                (0x00000630 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_DISP_PLL_MOD                 (0x00000634 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_DISP_PLL_NFRAC               (0x00000638 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_DISP_PLL_TUNE1               (0x0000063C + BASE_PRCM + VA_PRCM)
#define VA_PRCM_AX_PLL_EN                    (0x000006A0 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_AX_PLL_FAC                   (0x000006A8 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_AX_PLL_TUNE0                 (0x000006AC + BASE_PRCM + VA_PRCM)
#define VA_PRCM_AX_PLL_TEN                   (0x000006B4 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_AX_PLL_STAT                  (0x000006B8 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_AX_PLL_MOD                   (0x000006BC + BASE_PRCM + VA_PRCM)
#define VA_PRCM_AX_PLL_NFRAC                 (0x000006C0 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_AX_PLL_TUNE2                 (0x000006CC + BASE_PRCM + VA_PRCM)
#define VA_PRCM_PERH2_PLL_EN                 (0x000006E0 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_PERH2_PLL_FAC                (0x000006E4 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_PERH2_PLL_TUNE0              (0x000006E8 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_PERH2_PLL_TEN                (0x000006EC + BASE_PRCM + VA_PRCM)
#define VA_PRCM_PERH2_PLL_STAT               (0x000006F0 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_PERH2_PLL_MOD                (0x000006F4 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_PERH2_PLL_NFRAC              (0x000006F8 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_PERH2_PLL_TUNE1              (0x000006FC + BASE_PRCM + VA_PRCM)
#define VA_PRCM_PLL_TBLK                     (0x00000860 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_CPU_AXI_CLK_CTRL             (0x00000900 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_AHB_APB_CLK_CTRL             (0x00000904 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_MAXI_CLK_CTRL                (0x00000920 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_SDRAM_CLK_CTRL               (0x00000940 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_SDRAM_RST                    (0x00000944 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_SDRAM_BM_CLK_CTRL            (0x00000948 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_SDC0_CLK_CTRL                (0x00000960 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_SDC1_CLK_CTRL                (0x00000964 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_SDC2_CLK_CTRL                (0x00000968 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_SPI0_CLK_CTRL                (0x00000970 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_SPI1_CLK_CTRL                (0x00000974 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_SPI2_CLK_CTRL                (0x00000978 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_VC_ENC_CLK_CTRL              (0x000009C0 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_VC_DEC_CLK_CTRL              (0x000009C4 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_VISS_CLK_CTRL                (0x000009D0 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_DPU_SCLK0_CLK_CTRL           (0x00000AD0 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_DPU_SCLK1_CLK_CTRL           (0x00000AD4 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_DPU_SCLK2_CLK_CTRL           (0x00000AD8 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_DOSS_CLK_CTRL                (0x00000AE0 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_AX_CLK_CTRL                  (0x00000BD0 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_I2S0_CLK_CTRL                (0x00000BE0 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_I2S1_CLK_CTRL                (0x00000BE4 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_I2C0_CLK_CTRL                (0x00000CA0 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_I2C1_CLK_CTRL                (0x00000CA4 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_I2C2_CLK_CTRL                (0x00000CA8 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_I2C3_CLK_CTRL                (0x00000CAC + BASE_PRCM + VA_PRCM)
#define VA_PRCM_UART0_CLK_CTRL               (0x00000CC0 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_UART1_CLK_CTRL               (0x00000CC4 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_UART2_CLK_CTRL               (0x00000CC8 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_UART3_CLK_CTRL               (0x00000CCC + BASE_PRCM + VA_PRCM)
#define VA_PRCM_USB_PHY_RST                  (0x00000CE0 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_GPADC_CLK_CTRL               (0x00000CF0 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_IR_CLK_CTRL                  (0x00000D20 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_GMAC_CLK_CTRL                (0x00000D40 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_MBIST_CLK_CTRL               (0x00000EB0 + BASE_PRCM + VA_PRCM)
#define VA_PRCM_AES_CLK_CTRL                 (0x00000EC0 + BASE_PRCM + VA_PRCM)

#define DATA_PRCM_VER                        0x00001100
#define DATA_PRCM_AHB_GAT0                   0x00000000
#define DATA_PRCM_AHB_GAT1                   0x00000000
#define DATA_PRCM_AHB_RST0                   0x00000000
#define DATA_PRCM_AHB_RST1                   0x00000000
#define DATA_PRCM_APB_GAT0                   0x00000000
#define DATA_PRCM_APB_RST0                   0x00000000
#define DATA_PRCM_APB_GAT1                   0x00000000
#define DATA_PRCM_APB_RST1                   0x00000000
#define DATA_PRCM_AXI_GAT                    0x00000000
#define DATA_PRCM_HFEOSC_CTRL                0x00000031
#define DATA_PRCM_MBIAS_CTRL                 0x00000101
#define DATA_PRCM_CPU_PLL_EN                 0x00000000
#define DATA_PRCM_CPU_PLL_FAC                0x00001400
#define DATA_PRCM_CPU_PLL_TUNE0              0x03058E18
#define DATA_PRCM_CPU_PLL_TEN                0x00000000
#define DATA_PRCM_CPU_PLL_STAT               0x00000000
#define DATA_PRCM_CPU_PLL_MOD                0x00000000
#define DATA_PRCM_CPU_PLL_NFRAC              0x00000000
#define DATA_PRCM_CPU_PLL_TUNE2              0x00000000
#define DATA_PRCM_SDRAM_PLL_EN               0x00000000
#define DATA_PRCM_SDRAM_PLL_FAC              0x00001100
#define DATA_PRCM_SDRAM_PLL_TUNE0            0x03059E18
#define DATA_PRCM_SDRAM_PLL_TEN              0x00000000
#define DATA_PRCM_SDRAM_PLL_STAT             0x00000000
#define DATA_PRCM_SDRAM_PLL_MOD              0x00000000
#define DATA_PRCM_SDRAM_PLL_NFRAC            0x00000000
#define DATA_PRCM_SDRAM_PLL_TUNE1            0x00000000
#define DATA_PRCM_SDRAM_PLL_TUNE2            0x00000000
#define DATA_PRCM_PERH0_PLL_EN               0x00000000
#define DATA_PRCM_PERH0_PLL_FAC              0x00006311
#define DATA_PRCM_PERH0_PLL_TUNE0            0x0020B3D8
#define DATA_PRCM_PERH0_PLL_TEN              0x00000000
#define DATA_PRCM_PERH0_PLL_STAT             0x00000000
#define DATA_PRCM_PERH0_PLL_MOD              0x00000000
#define DATA_PRCM_PERH0_PLL_NFRAC            0x00000000
#define DATA_PRCM_AUDIO_PLL_EN               0x00000000
#define DATA_PRCM_AUDIO_PLL_FAC              0x00002B21
#define DATA_PRCM_AUDIO_PLL_TUNE0            0x0060B1D8
#define DATA_PRCM_AUDIO_PLL_TEN              0x00000000
#define DATA_PRCM_AUDIO_PLL_STAT             0x00000000
#define DATA_PRCM_AUDIO_PLL_MOD              0x00000000
#define DATA_PRCM_AUDIO_PLL_NFRAC            0x00000000
#define DATA_PRCM_VC_PLL_EN                  0x00000000
#define DATA_PRCM_VC_PLL_FAC                 0x00003C11
#define DATA_PRCM_VC_PLL_TUNE0               0x0020B1D8
#define DATA_PRCM_VC_PLL_TEN                 0x00000000
#define DATA_PRCM_VC_PLL_STAT                0x00000000
#define DATA_PRCM_VC_PLL_MOD                 0x00000000
#define DATA_PRCM_VC_PLL_NFRAC               0x00000000
#define DATA_PRCM_PERH1_PLL_EN               0x00000000
#define DATA_PRCM_PERH1_PLL_FAC              0x00006311
#define DATA_PRCM_PERH1_PLL_TUNE0            0x0060B1D8
#define DATA_PRCM_PERH1_PLL_TEN              0x00000000
#define DATA_PRCM_PERH1_PLL_STAT             0x00000000
#define DATA_PRCM_PERH1_PLL_MOD              0x00000000
#define DATA_PRCM_PERH1_PLL_NFRAC            0x00000000
#define DATA_PRCM_PERH1_PLL_TUNE1            0x00000000
#define DATA_PRCM_DISP_PLL_EN                0x00000000
#define DATA_PRCM_DISP_PLL_FAC               0x00006331
#define DATA_PRCM_DISP_PLL_TUNE0             0x0060B1D8
#define DATA_PRCM_DISP_PLL_TEN               0x00000000
#define DATA_PRCM_DISP_PLL_STAT              0x00000000
#define DATA_PRCM_DISP_PLL_MOD               0x00000000
#define DATA_PRCM_DISP_PLL_NFRAC             0x00000000
#define DATA_PRCM_DISP_PLL_TUNE1             0x00000000
#define DATA_PRCM_AX_PLL_EN                  0x00000000
#define DATA_PRCM_AX_PLL_FAC                 0x00001400
#define DATA_PRCM_AX_PLL_TUNE0               0x03058E18
#define DATA_PRCM_AX_PLL_TEN                 0x00000000
#define DATA_PRCM_AX_PLL_STAT                0x00000000
#define DATA_PRCM_AX_PLL_MOD                 0x00000000
#define DATA_PRCM_AX_PLL_NFRAC               0x00000000
#define DATA_PRCM_AX_PLL_TUNE2               0x00000000
#define DATA_PRCM_PERH2_PLL_EN               0x00000000
#define DATA_PRCM_PERH2_PLL_FAC              0x00007D12
#define DATA_PRCM_PERH2_PLL_TUNE0            0x0060B1D8
#define DATA_PRCM_PERH2_PLL_TEN              0x00000000
#define DATA_PRCM_PERH2_PLL_STAT             0x00000000
#define DATA_PRCM_PERH2_PLL_MOD              0x00000000
#define DATA_PRCM_PERH2_PLL_NFRAC            0x00000000
#define DATA_PRCM_PERH2_PLL_TUNE1            0x00000000
#define DATA_PRCM_PLL_TBLK                   0x00040000
#define DATA_PRCM_CPU_AXI_CLK_CTRL           0x00000000
#define DATA_PRCM_AHB_APB_CLK_CTRL           0x00000000
#define DATA_PRCM_MAXI_CLK_CTRL              0x00000000
#define DATA_PRCM_SDRAM_CLK_CTRL             0x00000000
#define DATA_PRCM_SDRAM_RST                  0x00000000
#define DATA_PRCM_SDRAM_BM_CLK_CTRL          0x00000000
#define DATA_PRCM_SDC0_CLK_CTRL              0x00000000
#define DATA_PRCM_SDC1_CLK_CTRL              0x00000000
#define DATA_PRCM_SDC2_CLK_CTRL              0x00000000
#define DATA_PRCM_SPI0_CLK_CTRL              0x00000001
#define DATA_PRCM_SPI1_CLK_CTRL              0x00000000
#define DATA_PRCM_SPI2_CLK_CTRL              0x00000000
#define DATA_PRCM_VC_ENC_CLK_CTRL            0x00000000
#define DATA_PRCM_VC_DEC_CLK_CTRL            0x00000000
#define DATA_PRCM_VISS_CLK_CTRL              0x00000000
#define DATA_PRCM_DPU_SCLK0_CLK_CTRL         0x00000000
#define DATA_PRCM_DPU_SCLK1_CLK_CTRL         0x00000000
#define DATA_PRCM_DPU_SCLK2_CLK_CTRL         0x00000000
#define DATA_PRCM_DOSS_CLK_CTRL              0x00000000
#define DATA_PRCM_AX_CLK_CTRL                0x00000000
#define DATA_PRCM_I2S0_CLK_CTRL              0x00000000
#define DATA_PRCM_I2S1_CLK_CTRL              0x00000000
#define DATA_PRCM_I2C0_CLK_CTRL              0x00000000
#define DATA_PRCM_I2C1_CLK_CTRL              0x00000000
#define DATA_PRCM_I2C2_CLK_CTRL              0x00000000
#define DATA_PRCM_I2C3_CLK_CTRL              0x00000000
#define DATA_PRCM_UART0_CLK_CTRL             0x00000000
#define DATA_PRCM_UART1_CLK_CTRL             0x00000000
#define DATA_PRCM_UART2_CLK_CTRL             0x00000000
#define DATA_PRCM_UART3_CLK_CTRL             0x00000000
#define DATA_PRCM_USB_PHY_RST                0x00000000
#define DATA_PRCM_GPADC_CLK_CTRL             0x00000000
#define DATA_PRCM_IR_CLK_CTRL                0x00000000
#define DATA_PRCM_GMAC_CLK_CTRL              0x00000000
#define DATA_PRCM_MBIST_CLK_CTRL             0x80000001
#define DATA_PRCM_AES_CLK_CTRL               0x00000000

/* CLOCK Version Register */
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
} reg_prcm_ver_t;

/* AHB BUS Clock Gating Register 0 */
typedef union {
	u32 val;
	struct {
	u32 dma:1;      /**/
	u32 ptimer:1;   /**/
	u32 rsvd0:4;    /**/
	u32 aes:1;      /**/
	u32 rsvd1:9;    /**/
	u32 sdram:1;    /**/
	u32 sdc0:1;     /**/
	u32 sdc1:1;     /**/
	u32 sdc2:1;     /**/
	u32 rsvd2:1;    /**/
	u32 spi0:1;     /**/
	u32 spi1:1;     /**/
	u32 spi2:1;     /**/
	u32 rsvd3:8;    /**/
	} bits;
} reg_prcm_ahb_gat0_t;

#define PRCM_AHB_GAT0_DMA_0     0x0
#define PRCM_AHB_GAT0_DMA_1     0x1
#define PRCM_AHB_GAT0_PTIMER_0  0x0
#define PRCM_AHB_GAT0_PTIMER_1  0x1
#define PRCM_AHB_GAT0_AES_0     0x0
#define PRCM_AHB_GAT0_AES_1     0x1
#define PRCM_AHB_GAT0_SDRAM_0   0x0
#define PRCM_AHB_GAT0_SDRAM_1   0x1
#define PRCM_AHB_GAT0_SDC0_0    0x0
#define PRCM_AHB_GAT0_SDC0_1    0x1
#define PRCM_AHB_GAT0_SDC1_0    0x0
#define PRCM_AHB_GAT0_SDC1_1    0x1
#define PRCM_AHB_GAT0_SDC2_0    0x0
#define PRCM_AHB_GAT0_SDC2_1    0x1
#define PRCM_AHB_GAT0_SPI0_0    0x0
#define PRCM_AHB_GAT0_SPI0_1    0x1
#define PRCM_AHB_GAT0_SPI1_0    0x0
#define PRCM_AHB_GAT0_SPI1_1    0x1
#define PRCM_AHB_GAT0_SPI2_0    0x0
#define PRCM_AHB_GAT0_SPI2_1    0x1

/* AHB BUS Clock Gating Register 1 */
typedef union {
	u32 val;
	struct {
	u32 viss:1;     /**/
	u32 rsvd0:1;    /**/
	u32 dpu:1;      /**/
	u32 rsvd1:1;    /**/
	u32 doss:1;     /**/
	u32 rsvd2:6;    /**/
	u32 vc_dec:1;   /**/
	u32 vc_enc:1;   /**/
	u32 ax:1;       /**/
	u32 rsvd3:2;    /**/
	u32 usb:1;      /**/
	u32 rsvd4:1;    /**/
	u32 gmac:1;     /**/
	u32 rsvd5:13;   /**/
	} bits;
} reg_prcm_ahb_gat1_t;

#define PRCM_AHB_GAT1_VISS_0    0x0
#define PRCM_AHB_GAT1_VISS_1    0x1
#define PRCM_AHB_GAT1_DPU_0     0x0
#define PRCM_AHB_GAT1_DPU_1     0x1
#define PRCM_AHB_GAT1_DOSS_0    0x0
#define PRCM_AHB_GAT1_DOSS_1    0x1
#define PRCM_AHB_GAT1_VC_DEC_0  0x0
#define PRCM_AHB_GAT1_VC_DEC_1  0x1
#define PRCM_AHB_GAT1_VC_ENC_0  0x0
#define PRCM_AHB_GAT1_VC_ENC_1  0x1
#define PRCM_AHB_GAT1_AX_0      0x0
#define PRCM_AHB_GAT1_AX_1      0x1
#define PRCM_AHB_GAT1_USB_0     0x0
#define PRCM_AHB_GAT1_USB_1     0x1
#define PRCM_AHB_GAT1_GMAC_0    0x0
#define PRCM_AHB_GAT1_GMAC_1    0x1

/* AHB BUS Reset Register 0 */
typedef union {
	u32 val;
	struct {
	u32 dma:1;      /**/
	u32 ptimer:1;   /**/
	u32 rsvd0:4;    /**/
	u32 aes:1;      /**/
	u32 rsvd1:9;    /**/
	u32 sdram:1;    /**/
	u32 sdc0:1;     /**/
	u32 sdc1:1;     /**/
	u32 sdc2:1;     /**/
	u32 rsvd2:1;    /**/
	u32 spi0:1;     /**/
	u32 spi1:1;     /**/
	u32 spi2:1;     /**/
	u32 rsvd3:8;    /**/
	} bits;
} reg_prcm_ahb_rst0_t;

#define PRCM_AHB_RST0_DMA_0     0x0
#define PRCM_AHB_RST0_DMA_1     0x1
#define PRCM_AHB_RST0_PTIMER_0  0x0
#define PRCM_AHB_RST0_PTIMER_1  0x1
#define PRCM_AHB_RST0_AES_0     0x0
#define PRCM_AHB_RST0_AES_1     0x1
#define PRCM_AHB_RST0_SDRAM_0   0x0
#define PRCM_AHB_RST0_SDRAM_1   0x1
#define PRCM_AHB_RST0_SDC0_0    0x0
#define PRCM_AHB_RST0_SDC0_1    0x1
#define PRCM_AHB_RST0_SDC1_0    0x0
#define PRCM_AHB_RST0_SDC1_1    0x1
#define PRCM_AHB_RST0_SDC2_0    0x0
#define PRCM_AHB_RST0_SDC2_1    0x1
#define PRCM_AHB_RST0_SPI0_0    0x0
#define PRCM_AHB_RST0_SPI0_1    0x1
#define PRCM_AHB_RST0_SPI1_0    0x0
#define PRCM_AHB_RST0_SPI1_1    0x1
#define PRCM_AHB_RST0_SPI2_0    0x0
#define PRCM_AHB_RST0_SPI2_1    0x1

/* AHB BUS Reset Register 1 */
typedef union {
	u32 val;
	struct {
	u32 viss:1;     /**/
	u32 rsvd0:1;    /**/
	u32 dpu:1;      /**/
	u32 rsvd1:1;    /**/
	u32 doss:1;     /**/
	u32 rsvd2:6;    /**/
	u32 vc_dec:1;   /**/
	u32 vc_enc:1;   /**/
	u32 ax:1;       /**/
	u32 rsvd3:2;    /**/
	u32 usb:1;      /**/
	u32 rsvd4:1;    /**/
	u32 gmac:1;     /**/
	u32 rsvd5:13;   /**/
	} bits;
} reg_prcm_ahb_rst1_t;

#define PRCM_AHB_RST1_VISS_0    0x0
#define PRCM_AHB_RST1_VISS_1    0x1
#define PRCM_AHB_RST1_DPU_0     0x0
#define PRCM_AHB_RST1_DPU_1     0x1
#define PRCM_AHB_RST1_DOSS_0    0x0
#define PRCM_AHB_RST1_DOSS_1    0x1
#define PRCM_AHB_RST1_VC_DEC_0  0x0
#define PRCM_AHB_RST1_VC_DEC_1  0x1
#define PRCM_AHB_RST1_VC_ENC_0  0x0
#define PRCM_AHB_RST1_VC_ENC_1  0x1
#define PRCM_AHB_RST1_AX_0      0x0
#define PRCM_AHB_RST1_AX_1      0x1
#define PRCM_AHB_RST1_USB_0     0x0
#define PRCM_AHB_RST1_USB_1     0x1
#define PRCM_AHB_RST1_GMAC_0    0x0
#define PRCM_AHB_RST1_GMAC_1    0x1

/* APB BUS Gating Register 0 */
typedef union {
	u32 val;
	struct {
	u32 i2c0:1;     /**/
	u32 i2c1:1;     /**/
	u32 i2c2:1;     /**/
	u32 i2c3:1;     /**/
	u32 rsvd0:4;    /**/
	u32 uart0:1;    /**/
	u32 uart1:1;    /**/
	u32 uart2:1;    /**/
	u32 uart3:1;    /**/
	u32 rsvd1:20;   /**/
	} bits;
} reg_prcm_apb_gat0_t;

#define PRCM_APB_GAT0_I2C0_0    0x0
#define PRCM_APB_GAT0_I2C0_1    0x1
#define PRCM_APB_GAT0_I2C1_0    0x0
#define PRCM_APB_GAT0_I2C1_1    0x1
#define PRCM_APB_GAT0_I2C2_0    0x0
#define PRCM_APB_GAT0_I2C2_1    0x1
#define PRCM_APB_GAT0_I2C3_0    0x0
#define PRCM_APB_GAT0_I2C3_1    0x1
#define PRCM_APB_GAT0_UART0_0   0x0
#define PRCM_APB_GAT0_UART0_1   0x1
#define PRCM_APB_GAT0_UART1_0   0x0
#define PRCM_APB_GAT0_UART1_1   0x1
#define PRCM_APB_GAT0_UART2_0   0x0
#define PRCM_APB_GAT0_UART2_1   0x1
#define PRCM_APB_GAT0_UART3_0   0x0
#define PRCM_APB_GAT0_UART3_1   0x1

/* APB0 BUS Reset Register 0 */
typedef union {
	u32 val;
	struct {
	u32 i2c0:1;     /**/
	u32 i2c1:1;     /**/
	u32 i2c2:1;     /**/
	u32 i2c3:1;     /**/
	u32 rsvd0:4;    /**/
	u32 uart0:1;    /**/
	u32 uart1:1;    /**/
	u32 uart2:1;    /**/
	u32 uart3:1;    /**/
	u32 rsvd1:20;   /**/
	} bits;
} reg_prcm_apb_rst0_t;

#define PRCM_APB_RST0_I2C0_0    0x0
#define PRCM_APB_RST0_I2C0_1    0x1
#define PRCM_APB_RST0_I2C1_0    0x0
#define PRCM_APB_RST0_I2C1_1    0x1
#define PRCM_APB_RST0_I2C2_0    0x0
#define PRCM_APB_RST0_I2C2_1    0x1
#define PRCM_APB_RST0_I2C3_0    0x0
#define PRCM_APB_RST0_I2C3_1    0x1
#define PRCM_APB_RST0_UART0_0   0x0
#define PRCM_APB_RST0_UART0_1   0x1
#define PRCM_APB_RST0_UART1_0   0x0
#define PRCM_APB_RST0_UART1_1   0x1
#define PRCM_APB_RST0_UART2_0   0x0
#define PRCM_APB_RST0_UART2_1   0x1
#define PRCM_APB_RST0_UART3_0   0x0
#define PRCM_APB_RST0_UART3_1   0x1

/* APB1 BUS Gating Register 1 */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:2;    /**/
	u32 i2s0:1;     /**/
	u32 i2s1:1;     /**/
	u32 rsvd1:2;    /**/
	u32 gpadc:1;    /**/
	u32 rsvd2:1;    /**/
	u32 ir:1;       /**/
	u32 gpio:1;     /**/
	u32 rsvd3:22;   /**/
	} bits;
} reg_prcm_apb_gat1_t;

#define PRCM_APB_GAT1_I2S0_0    0x0
#define PRCM_APB_GAT1_I2S0_1    0x1
#define PRCM_APB_GAT1_I2S1_0    0x0
#define PRCM_APB_GAT1_I2S1_1    0x1
#define PRCM_APB_GAT1_GPADC_0   0x0
#define PRCM_APB_GAT1_GPADC_1   0x1
#define PRCM_APB_GAT1_IR_0      0x0
#define PRCM_APB_GAT1_IR_1      0x1
#define PRCM_APB_GAT1_GPIO_0    0x0
#define PRCM_APB_GAT1_GPIO_1    0x1

/* APB1 BUS Reset Register 1 */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:2;    /**/
	u32 i2s0:1;     /**/
	u32 i2s1:1;     /**/
	u32 rsvd1:2;    /**/
	u32 gpadc:1;    /**/
	u32 rsvd2:1;    /**/
	u32 ir:1;       /**/
	u32 gpio:1;     /**/
	u32 rsvd3:22;   /**/
	} bits;
} reg_prcm_apb_rst1_t;

#define PRCM_APB_RST1_I2S0_0    0x0
#define PRCM_APB_RST1_I2S0_1    0x1
#define PRCM_APB_RST1_I2S1_0    0x0
#define PRCM_APB_RST1_I2S1_1    0x1
#define PRCM_APB_RST1_GPADC_0   0x0
#define PRCM_APB_RST1_GPADC_1   0x1
#define PRCM_APB_RST1_IR_0      0x0
#define PRCM_APB_RST1_IR_1      0x1
#define PRCM_APB_RST1_GPIO_0    0x0
#define PRCM_APB_RST1_GPIO_1    0x1

/* AXI BUS Clock Gating Register */
typedef union {
	u32 val;
	struct {
	u32 dma:1;      /**/
	u32 vc_enc:1;   /**/
	u32 vc_dec:1;   /**/
	u32 rsvd0:1;    /**/
	u32 viss0:1;    /**/
	u32 viss1:1;    /**/
	u32 rsvd1:2;    /**/
	u32 dpu:1;      /**/
	u32 rsvd2:1;    /**/
	u32 doss:1;     /**/
	u32 rsvd3:1;    /**/
	u32 ax:1;       /**/
	u32 rsvd4:5;    /**/
	u32 gmac:1;     /**/
	u32 rsvd5:13;   /**/
	} bits;
} reg_prcm_axi_gat_t;

#define PRCM_AXI_GAT_DMA_0      0x0
#define PRCM_AXI_GAT_DMA_1      0x1
#define PRCM_AXI_GAT_VC_ENC_0   0x0
#define PRCM_AXI_GAT_VC_ENC_1   0x1
#define PRCM_AXI_GAT_VC_DEC_0   0x0
#define PRCM_AXI_GAT_VC_DEC_1   0x1
#define PRCM_AXI_GAT_VISS0_0    0x0
#define PRCM_AXI_GAT_VISS0_1    0x1
#define PRCM_AXI_GAT_VISS1_0    0x0
#define PRCM_AXI_GAT_VISS1_1    0x1
#define PRCM_AXI_GAT_DPU_0      0x0
#define PRCM_AXI_GAT_DPU_1      0x1
#define PRCM_AXI_GAT_DOSS_0     0x0
#define PRCM_AXI_GAT_DOSS_1     0x1
#define PRCM_AXI_GAT_AX_0       0x0
#define PRCM_AXI_GAT_AX_1       0x1
#define PRCM_AXI_GAT_GMAC_0     0x0
#define PRCM_AXI_GAT_GMAC_1     0x1

/* HFEOSC XTAL Control Register */
typedef union {
	u32 val;
	struct {
	u32 en:1;       /**/
	u32 src_sel:2;  /**/
	u32 xtal_start:1;/**/
	u32 tune:3;     /**/
	u32 rsvd0:21;   /**/
	u32 src_sel_dbg:2;/**/
	u32 rsvd1:2;    /**/
	} bits;
} reg_prcm_hfeosc_ctrl_t;

#define PRCM_HFEOSC_CTRL_EN_0           0x0
#define PRCM_HFEOSC_CTRL_EN_1           0x1
#define PRCM_HFEOSC_CTRL_SRC_SEL_0      0x0
#define PRCM_HFEOSC_CTRL_SRC_SEL_1      0x1
#define PRCM_HFEOSC_CTRL_SRC_SEL_2      0x2
#define PRCM_HFEOSC_CTRL_XTAL_START_0   0x0
#define PRCM_HFEOSC_CTRL_XTAL_START_1   0x1
#define PRCM_HFEOSC_CTRL_TUNE_0         0x0
#define PRCM_HFEOSC_CTRL_TUNE_1         0x1
#define PRCM_HFEOSC_CTRL_TUNE_2         0x2
#define PRCM_HFEOSC_CTRL_TUNE_3         0x3
#define PRCM_HFEOSC_CTRL_TUNE_4         0x4
#define PRCM_HFEOSC_CTRL_TUNE_5         0x5
#define PRCM_HFEOSC_CTRL_TUNE_6         0x6
#define PRCM_HFEOSC_CTRL_TUNE_7         0x7
#define PRCM_HFEOSC_CTRL_SRC_SEL_DBG_0  0x0
#define PRCM_HFEOSC_CTRL_SRC_SEL_DBG_1  0x1
#define PRCM_HFEOSC_CTRL_SRC_SEL_DBG_2  0x2

/* MBIAS Control Register */
typedef union {
	u32 val;
	struct {
	u32 en:1;       /**/
	u32 rsvd0:3;    /**/
	u32 tst_en0:1;  /**/
	u32 tst_en1:1;  /**/
	u32 tst_en2:1;  /**/
	u32 tst_en3:1;  /**/
	u32 tune2_0:3;  /**/
	u32 tune3:1;    /**/
	u32 tune4:1;    /**/
	u32 rsvd1:19;   /**/
	} bits;
} reg_prcm_mbias_ctrl_t;

#define PRCM_MBIAS_CTRL_EN_0            0x0
#define PRCM_MBIAS_CTRL_EN_1            0x1
#define PRCM_MBIAS_CTRL_TST_EN0_0       0x0
#define PRCM_MBIAS_CTRL_TST_EN0_1       0x1
#define PRCM_MBIAS_CTRL_TST_EN1_0       0x0
#define PRCM_MBIAS_CTRL_TST_EN1_1       0x1
#define PRCM_MBIAS_CTRL_TST_EN2_0       0x0
#define PRCM_MBIAS_CTRL_TST_EN2_1       0x1
#define PRCM_MBIAS_CTRL_TST_EN3_0       0x0
#define PRCM_MBIAS_CTRL_TST_EN3_1       0x1
#define PRCM_MBIAS_CTRL_TUNE2_0_0       0x0
#define PRCM_MBIAS_CTRL_TUNE2_0_1       0x1
#define PRCM_MBIAS_CTRL_TUNE2_0_2       0x2
#define PRCM_MBIAS_CTRL_TUNE2_0_3       0x3
#define PRCM_MBIAS_CTRL_TUNE2_0_4       0x4
#define PRCM_MBIAS_CTRL_TUNE2_0_5       0x5
#define PRCM_MBIAS_CTRL_TUNE2_0_6       0x6
#define PRCM_MBIAS_CTRL_TUNE2_0_7       0x7
#define PRCM_MBIAS_CTRL_TUNE3_0         0x0
#define PRCM_MBIAS_CTRL_TUNE3_1         0x1
#define PRCM_MBIAS_CTRL_TUNE4_0         0x0
#define PRCM_MBIAS_CTRL_TUNE4_1         0x1

/* CPU_PLL Enable Register */
typedef union {
	u32 val;
	struct {
	u32 enp:1;      /**/
	u32 enm:1;      /**/
	u32 oen:1;      /**/
	u32 rsvd0:1;    /**/
	u32 ren:1;      /**/
	u32 rsvd1:27;   /**/
	} bits;
} reg_prcm_cpu_pll_en_t;

#define PRCM_CPU_PLL_EN_ENP_0   0x0
#define PRCM_CPU_PLL_EN_ENP_1   0x1
#define PRCM_CPU_PLL_EN_ENM_0   0x0
#define PRCM_CPU_PLL_EN_ENM_1   0x1
#define PRCM_CPU_PLL_EN_OEN_0   0x0
#define PRCM_CPU_PLL_EN_OEN_1   0x1
#define PRCM_CPU_PLL_EN_REN_0   0x0
#define PRCM_CPU_PLL_EN_REN_1   0x1

/* CPU_PLL Factor Register */
typedef union {
	u32 val;
	struct {
	u32 pre_div:2;  /**/
	u32 rsvd0:2;    /**/
	u32 post_div:2; /**/
	u32 rsvd1:2;    /**/
	u32 n:8;        /**/
	u32 rsvd2:16;   /**/
	} bits;
} reg_prcm_cpu_pll_fac_t;

#define PRCM_CPU_PLL_FAC_PRE_DIV_0      0x0
#define PRCM_CPU_PLL_FAC_PRE_DIV_1      0x1
#define PRCM_CPU_PLL_FAC_PRE_DIV_2      0x2
#define PRCM_CPU_PLL_FAC_PRE_DIV_3      0x3
#define PRCM_CPU_PLL_FAC_POST_DIV_0     0x0
#define PRCM_CPU_PLL_FAC_POST_DIV_1     0x1
#define PRCM_CPU_PLL_FAC_POST_DIV_2     0x2
#define PRCM_CPU_PLL_FAC_POST_DIV_3     0x3

/* CPU_PLL Tune0 Register */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:3;    /**/
	u32 tune1:3;    /**/
	u32 rsvd1:3;    /**/
	u32 tune2:5;    /**/
	u32 tune3:1;    /**/
	u32 tune4:3;    /**/
	u32 tune5:1;    /**/
	u32 tune6:1;    /**/
	u32 tune7:1;    /**/
	u32 tune8:1;    /**/
	u32 tune9:3;    /**/
	u32 tune10:1;   /**/
	u32 tune11:2;   /**/
	u32 tune12:1;   /**/
	u32 rsvd2:3;    /**/
	} bits;
} reg_prcm_cpu_pll_tune0_t;

#define PRCM_CPU_PLL_TUNE0_TUNE1_0      0x0
#define PRCM_CPU_PLL_TUNE0_TUNE1_1      0x1
#define PRCM_CPU_PLL_TUNE0_TUNE1_2      0x2
#define PRCM_CPU_PLL_TUNE0_TUNE1_3      0x3
#define PRCM_CPU_PLL_TUNE0_TUNE1_4      0x4
#define PRCM_CPU_PLL_TUNE0_TUNE1_5      0x5
#define PRCM_CPU_PLL_TUNE0_TUNE1_6      0x6
#define PRCM_CPU_PLL_TUNE0_TUNE1_7      0x7
#define PRCM_CPU_PLL_TUNE0_TUNE3_0      0x0
#define PRCM_CPU_PLL_TUNE0_TUNE3_1      0x1
#define PRCM_CPU_PLL_TUNE0_TUNE4_0      0x0
#define PRCM_CPU_PLL_TUNE0_TUNE4_1      0x1
#define PRCM_CPU_PLL_TUNE0_TUNE4_2      0x2
#define PRCM_CPU_PLL_TUNE0_TUNE4_3      0x3
#define PRCM_CPU_PLL_TUNE0_TUNE4_4      0x4
#define PRCM_CPU_PLL_TUNE0_TUNE4_5      0x5
#define PRCM_CPU_PLL_TUNE0_TUNE4_6      0x6
#define PRCM_CPU_PLL_TUNE0_TUNE4_7      0x7
#define PRCM_CPU_PLL_TUNE0_TUNE5_0      0x0
#define PRCM_CPU_PLL_TUNE0_TUNE5_1      0x1
#define PRCM_CPU_PLL_TUNE0_TUNE6_0      0x0
#define PRCM_CPU_PLL_TUNE0_TUNE6_1      0x1
#define PRCM_CPU_PLL_TUNE0_TUNE7_0      0x0
#define PRCM_CPU_PLL_TUNE0_TUNE7_1      0x1
#define PRCM_CPU_PLL_TUNE0_TUNE8_0      0x0
#define PRCM_CPU_PLL_TUNE0_TUNE8_1      0x1
#define PRCM_CPU_PLL_TUNE0_TUNE10_0     0x0
#define PRCM_CPU_PLL_TUNE0_TUNE10_1     0x1
#define PRCM_CPU_PLL_TUNE0_TUNE11_0     0x0
#define PRCM_CPU_PLL_TUNE0_TUNE11_1     0x1
#define PRCM_CPU_PLL_TUNE0_TUNE11_2     0x2
#define PRCM_CPU_PLL_TUNE0_TUNE11_3     0x3
#define PRCM_CPU_PLL_TUNE0_TUNE12_0     0x0
#define PRCM_CPU_PLL_TUNE0_TUNE12_1     0x1

/* CPU_PLL Test Enable Register */
typedef union {
	u32 val;
	struct {
	u32 ten0:1;     /**/
	u32 rsvd0:2;    /**/
	u32 ten1:1;     /**/
	u32 rsvd1:28;   /**/
	} bits;
} reg_prcm_cpu_pll_ten_t;

#define PRCM_CPU_PLL_TEN_TEN0_0 0x0
#define PRCM_CPU_PLL_TEN_TEN0_1 0x1
#define PRCM_CPU_PLL_TEN_TEN1_0 0x0
#define PRCM_CPU_PLL_TEN_TEN1_1 0x1

/* CPU_PLL Status Register */
typedef union {
	u32 val;
	struct {
	u32 dlock:1;    /**/
	u32 asteady:1;  /**/
	u32 alock:1;    /**/
	u32 rsvd0:29;   /**/
	} bits;
} reg_prcm_cpu_pll_stat_t;

#define PRCM_CPU_PLL_STAT_DLOCK_0       0x0
#define PRCM_CPU_PLL_STAT_DLOCK_1       0x1
#define PRCM_CPU_PLL_STAT_ASTEADY_0     0x0
#define PRCM_CPU_PLL_STAT_ASTEADY_1     0x1
#define PRCM_CPU_PLL_STAT_ALOCK_0       0x0
#define PRCM_CPU_PLL_STAT_ALOCK_1       0x1

/* CPU_PLL Mode Register */
typedef union {
	u32 val;
	struct {
	u32 mod:2;      /**/
	u32 rsvd0:30;   /**/
	} bits;
} reg_prcm_cpu_pll_mod_t;

#define PRCM_CPU_PLL_MOD_MOD_0  0x0
#define PRCM_CPU_PLL_MOD_MOD_1  0x1
#define PRCM_CPU_PLL_MOD_MOD_2  0x2
#define PRCM_CPU_PLL_MOD_MOD_3  0x3

/* CPU_PLL N Factor Fraction Register */
typedef union {
	u32 val;
	struct {
	u32 nfrac:18;   /**/
	u32 rsvd0:14;   /**/
	} bits;
} reg_prcm_cpu_pll_nfrac_t;

/* CPU_PLL Tune2 Register */
typedef union {
	u32 val;
	struct {
	u32 en:1;       /**/
	u32 rsvd0:15;   /**/
	u32 step:14;    /**/
	u32 rsvd1:2;    /**/
	} bits;
} reg_prcm_cpu_pll_tune2_t;

#define PRCM_CPU_PLL_TUNE2_EN_0         0x0
#define PRCM_CPU_PLL_TUNE2_EN_1         0x1

/* SDRAM_PLL Enable Register */
typedef union {
	u32 val;
	struct {
	u32 enp:1;      /**/
	u32 enm:1;      /**/
	u32 oen:1;      /**/
	u32 rsvd0:1;    /**/
	u32 ren:1;      /**/
	u32 rsvd1:27;   /**/
	} bits;
} reg_prcm_sdram_pll_en_t;

#define PRCM_SDRAM_PLL_EN_ENP_0         0x0
#define PRCM_SDRAM_PLL_EN_ENP_1         0x1
#define PRCM_SDRAM_PLL_EN_ENM_0         0x0
#define PRCM_SDRAM_PLL_EN_ENM_1         0x1
#define PRCM_SDRAM_PLL_EN_OEN_0         0x0
#define PRCM_SDRAM_PLL_EN_OEN_1         0x1
#define PRCM_SDRAM_PLL_EN_REN_0         0x0
#define PRCM_SDRAM_PLL_EN_REN_1         0x1

/* SDRAM_PLL Factor Register */
typedef union {
	u32 val;
	struct {
	u32 pre_div:2;  /**/
	u32 rsvd0:2;    /**/
	u32 post_div:2; /**/
	u32 rsvd1:2;    /**/
	u32 n:8;        /**/
	u32 rsvd2:16;   /**/
	} bits;
} reg_prcm_sdram_pll_fac_t;

#define PRCM_SDRAM_PLL_FAC_PRE_DIV_0    0x0
#define PRCM_SDRAM_PLL_FAC_PRE_DIV_1    0x1
#define PRCM_SDRAM_PLL_FAC_PRE_DIV_2    0x2
#define PRCM_SDRAM_PLL_FAC_PRE_DIV_3    0x3
#define PRCM_SDRAM_PLL_FAC_POST_DIV_0   0x0
#define PRCM_SDRAM_PLL_FAC_POST_DIV_1   0x1
#define PRCM_SDRAM_PLL_FAC_POST_DIV_2   0x2
#define PRCM_SDRAM_PLL_FAC_POST_DIV_3   0x3

/* SDRAM_PLL Tune0 Register */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:3;    /**/
	u32 tune1:3;    /**/
	u32 rsvd1:3;    /**/
	u32 tune3:5;    /**/
	u32 tune4:1;    /**/
	u32 tune5:3;    /**/
	u32 tune6:1;    /**/
	u32 tune7:1;    /**/
	u32 tune8:1;    /**/
	u32 tune9:1;    /**/
	u32 tune10:3;   /**/
	u32 tune11:1;   /**/
	u32 tune12:2;   /**/
	u32 tune13:1;   /**/
	u32 rsvd2:3;    /**/
	} bits;
} reg_prcm_sdram_pll_tune0_t;

#define PRCM_SDRAM_PLL_TUNE0_TUNE1_0    0x0
#define PRCM_SDRAM_PLL_TUNE0_TUNE1_1    0x1
#define PRCM_SDRAM_PLL_TUNE0_TUNE1_2    0x2
#define PRCM_SDRAM_PLL_TUNE0_TUNE1_3    0x3
#define PRCM_SDRAM_PLL_TUNE0_TUNE1_4    0x4
#define PRCM_SDRAM_PLL_TUNE0_TUNE1_5    0x5
#define PRCM_SDRAM_PLL_TUNE0_TUNE1_6    0x6
#define PRCM_SDRAM_PLL_TUNE0_TUNE1_7    0x7
#define PRCM_SDRAM_PLL_TUNE0_TUNE4_0    0x0
#define PRCM_SDRAM_PLL_TUNE0_TUNE4_1    0x1
#define PRCM_SDRAM_PLL_TUNE0_TUNE5_0    0x0
#define PRCM_SDRAM_PLL_TUNE0_TUNE5_1    0x1
#define PRCM_SDRAM_PLL_TUNE0_TUNE5_2    0x2
#define PRCM_SDRAM_PLL_TUNE0_TUNE5_3    0x3
#define PRCM_SDRAM_PLL_TUNE0_TUNE5_4    0x4
#define PRCM_SDRAM_PLL_TUNE0_TUNE5_5    0x5
#define PRCM_SDRAM_PLL_TUNE0_TUNE5_6    0x6
#define PRCM_SDRAM_PLL_TUNE0_TUNE5_7    0x7
#define PRCM_SDRAM_PLL_TUNE0_TUNE6_0    0x0
#define PRCM_SDRAM_PLL_TUNE0_TUNE6_1    0x1
#define PRCM_SDRAM_PLL_TUNE0_TUNE7_0    0x0
#define PRCM_SDRAM_PLL_TUNE0_TUNE7_1    0x1
#define PRCM_SDRAM_PLL_TUNE0_TUNE8_0    0x0
#define PRCM_SDRAM_PLL_TUNE0_TUNE8_1    0x1
#define PRCM_SDRAM_PLL_TUNE0_TUNE9_0    0x0
#define PRCM_SDRAM_PLL_TUNE0_TUNE9_1    0x1
#define PRCM_SDRAM_PLL_TUNE0_TUNE11_0   0x0
#define PRCM_SDRAM_PLL_TUNE0_TUNE11_1   0x1
#define PRCM_SDRAM_PLL_TUNE0_TUNE12_0   0x0
#define PRCM_SDRAM_PLL_TUNE0_TUNE12_1   0x1
#define PRCM_SDRAM_PLL_TUNE0_TUNE12_2   0x2
#define PRCM_SDRAM_PLL_TUNE0_TUNE12_3   0x3
#define PRCM_SDRAM_PLL_TUNE0_TUNE13_0   0x0
#define PRCM_SDRAM_PLL_TUNE0_TUNE13_1   0x1

/* SDRAM_PLL Test Enable Register */
typedef union {
	u32 val;
	struct {
	u32 ten0:1;     /**/
	u32 ten1:1;     /**/
	u32 ten2:1;     /**/
	u32 ten3:1;     /**/
	u32 rsvd0:28;   /**/
	} bits;
} reg_prcm_sdram_pll_ten_t;

#define PRCM_SDRAM_PLL_TEN_TEN0_0       0x0
#define PRCM_SDRAM_PLL_TEN_TEN0_1       0x1
#define PRCM_SDRAM_PLL_TEN_TEN1_0       0x0
#define PRCM_SDRAM_PLL_TEN_TEN1_1       0x1
#define PRCM_SDRAM_PLL_TEN_TEN2_0       0x0
#define PRCM_SDRAM_PLL_TEN_TEN2_1       0x1
#define PRCM_SDRAM_PLL_TEN_TEN3_0       0x0
#define PRCM_SDRAM_PLL_TEN_TEN3_1       0x1

/* SRAM_PLL Status Register */
typedef union {
	u32 val;
	struct {
	u32 dlock:1;    /**/
	u32 asteady:1;  /**/
	u32 alock:1;    /**/
	u32 rsvd0:29;   /**/
	} bits;
} reg_prcm_sdram_pll_stat_t;

#define PRCM_SDRAM_PLL_STAT_DLOCK_0     0x0
#define PRCM_SDRAM_PLL_STAT_DLOCK_1     0x1
#define PRCM_SDRAM_PLL_STAT_ASTEADY_0   0x0
#define PRCM_SDRAM_PLL_STAT_ASTEADY_1   0x1
#define PRCM_SDRAM_PLL_STAT_ALOCK_0     0x0
#define PRCM_SDRAM_PLL_STAT_ALOCK_1     0x1

/* SDRAM_PLL Mode Register */
typedef union {
	u32 val;
	struct {
	u32 mod:2;      /**/
	u32 rsvd0:30;   /**/
	} bits;
} reg_prcm_sdram_pll_mod_t;

#define PRCM_SDRAM_PLL_MOD_MOD_0        0x0
#define PRCM_SDRAM_PLL_MOD_MOD_1        0x1
#define PRCM_SDRAM_PLL_MOD_MOD_2        0x2
#define PRCM_SDRAM_PLL_MOD_MOD_3        0x3

/* SDRAM_PLL N Factor Fraction Register */
typedef union {
	u32 val;
	struct {
	u32 nfrac:18;   /**/
	u32 rsvd0:14;   /**/
	} bits;
} reg_prcm_sdram_pll_nfrac_t;

/* SDRAM_PLL Tune1 Register */
typedef union {
	u32 val;
	struct {
	u32 period:12;  /**/
	u32 rsvd0:4;    /**/
	u32 amplitude:12;/**/
	u32 rsvd1:4;    /**/
	} bits;
} reg_prcm_sdram_pll_tune1_t;

/* SDRAM_PLL Tune2 Register */
typedef union {
	u32 val;
	struct {
	u32 en:1;       /**/
	u32 rsvd0:15;   /**/
	u32 step:14;    /**/
	u32 rsvd1:2;    /**/
	} bits;
} reg_prcm_sdram_pll_tune2_t;

#define PRCM_SDRAM_PLL_TUNE2_EN_0       0x0
#define PRCM_SDRAM_PLL_TUNE2_EN_1       0x1

/* PERH0_PLL Enable Register */
typedef union {
	u32 val;
	struct {
	u32 enp:1;      /**/
	u32 enm:1;      /**/
	u32 oen:1;      /**/
	u32 rsvd0:1;    /**/
	u32 ren:1;      /**/
	u32 rsvd1:27;   /**/
	} bits;
} reg_prcm_perh0_pll_en_t;

#define PRCM_PERH0_PLL_EN_ENP_0         0x0
#define PRCM_PERH0_PLL_EN_ENP_1         0x1
#define PRCM_PERH0_PLL_EN_ENM_0         0x0
#define PRCM_PERH0_PLL_EN_ENM_1         0x1
#define PRCM_PERH0_PLL_EN_OEN_0         0x0
#define PRCM_PERH0_PLL_EN_OEN_1         0x1
#define PRCM_PERH0_PLL_EN_REN_0         0x0
#define PRCM_PERH0_PLL_EN_REN_1         0x1

/* PERH0_PLL Factor Register */
typedef union {
	u32 val;
	struct {
	u32 pre_div:2;  /**/
	u32 rsvd0:2;    /**/
	u32 post_div:2; /**/
	u32 rsvd1:2;    /**/
	u32 n:8;        /**/
	u32 rsvd2:16;   /**/
	} bits;
} reg_prcm_perh0_pll_fac_t;

#define PRCM_PERH0_PLL_FAC_PRE_DIV_0    0x0
#define PRCM_PERH0_PLL_FAC_PRE_DIV_1    0x1
#define PRCM_PERH0_PLL_FAC_PRE_DIV_2    0x2
#define PRCM_PERH0_PLL_FAC_PRE_DIV_3    0x3
#define PRCM_PERH0_PLL_FAC_POST_DIV_0   0x0
#define PRCM_PERH0_PLL_FAC_POST_DIV_1   0x1
#define PRCM_PERH0_PLL_FAC_POST_DIV_2   0x2
#define PRCM_PERH0_PLL_FAC_POST_DIV_3   0x3

/* PERH0_PLL Tune0 Register */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:3;    /**/
	u32 tune1:3;    /**/
	u32 tune2:5;    /**/
	u32 tune3:1;    /**/
	u32 tune4:3;    /**/
	u32 tune5:1;    /**/
	u32 tune6:1;    /**/
	u32 tune7:1;    /**/
	u32 tune8:1;    /**/
	u32 tune9:3;    /**/
	u32 rsvd1:3;    /**/
	u32 tune10:1;   /**/
	u32 rsvd2:6;    /**/
	} bits;
} reg_prcm_perh0_pll_tune0_t;

#define PRCM_PERH0_PLL_TUNE0_TUNE1_0    0x0
#define PRCM_PERH0_PLL_TUNE0_TUNE1_1    0x1
#define PRCM_PERH0_PLL_TUNE0_TUNE1_2    0x2
#define PRCM_PERH0_PLL_TUNE0_TUNE1_3    0x3
#define PRCM_PERH0_PLL_TUNE0_TUNE1_4    0x4
#define PRCM_PERH0_PLL_TUNE0_TUNE1_5    0x5
#define PRCM_PERH0_PLL_TUNE0_TUNE1_6    0x6
#define PRCM_PERH0_PLL_TUNE0_TUNE1_7    0x7
#define PRCM_PERH0_PLL_TUNE0_TUNE3_0    0x0
#define PRCM_PERH0_PLL_TUNE0_TUNE3_1    0x1
#define PRCM_PERH0_PLL_TUNE0_TUNE4_0    0x0
#define PRCM_PERH0_PLL_TUNE0_TUNE4_1    0x1
#define PRCM_PERH0_PLL_TUNE0_TUNE4_2    0x2
#define PRCM_PERH0_PLL_TUNE0_TUNE4_3    0x3
#define PRCM_PERH0_PLL_TUNE0_TUNE4_4    0x4
#define PRCM_PERH0_PLL_TUNE0_TUNE4_5    0x5
#define PRCM_PERH0_PLL_TUNE0_TUNE4_6    0x6
#define PRCM_PERH0_PLL_TUNE0_TUNE4_7    0x7
#define PRCM_PERH0_PLL_TUNE0_TUNE5_0    0x0
#define PRCM_PERH0_PLL_TUNE0_TUNE5_1    0x1
#define PRCM_PERH0_PLL_TUNE0_TUNE6_0    0x0
#define PRCM_PERH0_PLL_TUNE0_TUNE6_1    0x1
#define PRCM_PERH0_PLL_TUNE0_TUNE7_0    0x0
#define PRCM_PERH0_PLL_TUNE0_TUNE7_1    0x1
#define PRCM_PERH0_PLL_TUNE0_TUNE8_0    0x0
#define PRCM_PERH0_PLL_TUNE0_TUNE8_1    0x1
#define PRCM_PERH0_PLL_TUNE0_TUNE10_0   0x0
#define PRCM_PERH0_PLL_TUNE0_TUNE10_1   0x1

/* PERH0_PLL Test Enable Register */
typedef union {
	u32 val;
	struct {
	u32 ten0:1;     /**/
	u32 ten1:1;     /**/
	u32 ten3:1;     /**/
	u32 ten2:1;     /**/
	u32 rsvd0:28;   /**/
	} bits;
} reg_prcm_perh0_pll_ten_t;

#define PRCM_PERH0_PLL_TEN_TEN0_0       0x0
#define PRCM_PERH0_PLL_TEN_TEN0_1       0x1
#define PRCM_PERH0_PLL_TEN_TEN1_0       0x0
#define PRCM_PERH0_PLL_TEN_TEN1_1       0x1
#define PRCM_PERH0_PLL_TEN_TEN3_0       0x0
#define PRCM_PERH0_PLL_TEN_TEN3_1       0x1
#define PRCM_PERH0_PLL_TEN_TEN2_0       0x0
#define PRCM_PERH0_PLL_TEN_TEN2_1       0x1

/* PERH0_PLL Status Register */
typedef union {
	u32 val;
	struct {
	u32 dlock:1;    /**/
	u32 asteady:1;  /**/
	u32 alock:1;    /**/
	u32 rsvd0:29;   /**/
	} bits;
} reg_prcm_perh0_pll_stat_t;

#define PRCM_PERH0_PLL_STAT_DLOCK_0     0x0
#define PRCM_PERH0_PLL_STAT_DLOCK_1     0x1
#define PRCM_PERH0_PLL_STAT_ASTEADY_0   0x0
#define PRCM_PERH0_PLL_STAT_ASTEADY_1   0x1
#define PRCM_PERH0_PLL_STAT_ALOCK_0     0x0
#define PRCM_PERH0_PLL_STAT_ALOCK_1     0x1

/* PERH0_PLL Mode Register */
typedef union {
	u32 val;
	struct {
	u32 mod:2;      /**/
	u32 rsvd0:30;   /**/
	} bits;
} reg_prcm_perh0_pll_mod_t;

#define PRCM_PERH0_PLL_MOD_MOD_0        0x0
#define PRCM_PERH0_PLL_MOD_MOD_1        0x1
#define PRCM_PERH0_PLL_MOD_MOD_2        0x2
#define PRCM_PERH0_PLL_MOD_MOD_3        0x3

/* PERH0_PLL N Factor Fraction Register */
typedef union {
	u32 val;
	struct {
	u32 nfrac:18;   /**/
	u32 rsvd0:14;   /**/
	} bits;
} reg_prcm_perh0_pll_nfrac_t;

/* AUDIO_PLL Enable Register */
typedef union {
	u32 val;
	struct {
	u32 enp:1;      /**/
	u32 enm:1;      /**/
	u32 oen:1;      /**/
	u32 rsvd0:1;    /**/
	u32 ren:1;      /**/
	u32 rsvd1:3;    /**/
	u32 div_endiv17:1;/**/
	u32 div_endiv7:1;/**/
	u32 rsvd2:22;   /**/
	} bits;
} reg_prcm_audio_pll_en_t;

#define PRCM_AUDIO_PLL_EN_ENP_0         0x0
#define PRCM_AUDIO_PLL_EN_ENP_1         0x1
#define PRCM_AUDIO_PLL_EN_ENM_0         0x0
#define PRCM_AUDIO_PLL_EN_ENM_1         0x1
#define PRCM_AUDIO_PLL_EN_OEN_0         0x0
#define PRCM_AUDIO_PLL_EN_OEN_1         0x1
#define PRCM_AUDIO_PLL_EN_REN_0         0x0
#define PRCM_AUDIO_PLL_EN_REN_1         0x1
#define PRCM_AUDIO_PLL_EN_DIV_ENDIV17_0 0x0
#define PRCM_AUDIO_PLL_EN_DIV_ENDIV17_1 0x1
#define PRCM_AUDIO_PLL_EN_DIV_ENDIV7_0  0x0
#define PRCM_AUDIO_PLL_EN_DIV_ENDIV7_1  0x1

/* AUDIO_PLL Factor Register */
typedef union {
	u32 val;
	struct {
	u32 pre_div:2;  /**/
	u32 rsvd0:2;    /**/
	u32 post_div:2; /**/
	u32 rsvd1:2;    /**/
	u32 n:8;        /**/
	u32 rsvd2:16;   /**/
	} bits;
} reg_prcm_audio_pll_fac_t;

#define PRCM_AUDIO_PLL_FAC_PRE_DIV_0    0x0
#define PRCM_AUDIO_PLL_FAC_PRE_DIV_1    0x1
#define PRCM_AUDIO_PLL_FAC_PRE_DIV_2    0x2
#define PRCM_AUDIO_PLL_FAC_PRE_DIV_3    0x3
#define PRCM_AUDIO_PLL_FAC_POST_DIV_0   0x0
#define PRCM_AUDIO_PLL_FAC_POST_DIV_1   0x1
#define PRCM_AUDIO_PLL_FAC_POST_DIV_2   0x2
#define PRCM_AUDIO_PLL_FAC_POST_DIV_3   0x3

/* AUDIO_PLL Tune0 Register */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:3;    /**/
	u32 tune1:3;    /**/
	u32 tune2:5;    /**/
	u32 tune3:1;    /**/
	u32 tune4:3;    /**/
	u32 tune5:1;    /**/
	u32 tune6:1;    /**/
	u32 tune7:1;    /**/
	u32 tune8:1;    /**/
	u32 tune9:3;    /**/
	u32 tune10:1;   /**/
	u32 tune11:2;   /**/
	u32 tune12:1;   /**/
	u32 rsvd1:6;    /**/
	} bits;
} reg_prcm_audio_pll_tune0_t;

#define PRCM_AUDIO_PLL_TUNE0_TUNE1_0    0x0
#define PRCM_AUDIO_PLL_TUNE0_TUNE1_1    0x1
#define PRCM_AUDIO_PLL_TUNE0_TUNE1_2    0x2
#define PRCM_AUDIO_PLL_TUNE0_TUNE1_3    0x3
#define PRCM_AUDIO_PLL_TUNE0_TUNE1_4    0x4
#define PRCM_AUDIO_PLL_TUNE0_TUNE1_5    0x5
#define PRCM_AUDIO_PLL_TUNE0_TUNE1_6    0x6
#define PRCM_AUDIO_PLL_TUNE0_TUNE1_7    0x7
#define PRCM_AUDIO_PLL_TUNE0_TUNE3_0    0x0
#define PRCM_AUDIO_PLL_TUNE0_TUNE3_1    0x1
#define PRCM_AUDIO_PLL_TUNE0_TUNE4_0    0x0
#define PRCM_AUDIO_PLL_TUNE0_TUNE4_1    0x1
#define PRCM_AUDIO_PLL_TUNE0_TUNE4_2    0x2
#define PRCM_AUDIO_PLL_TUNE0_TUNE4_3    0x3
#define PRCM_AUDIO_PLL_TUNE0_TUNE4_4    0x4
#define PRCM_AUDIO_PLL_TUNE0_TUNE4_5    0x5
#define PRCM_AUDIO_PLL_TUNE0_TUNE4_6    0x6
#define PRCM_AUDIO_PLL_TUNE0_TUNE4_7    0x7
#define PRCM_AUDIO_PLL_TUNE0_TUNE5_0    0x0
#define PRCM_AUDIO_PLL_TUNE0_TUNE5_1    0x1
#define PRCM_AUDIO_PLL_TUNE0_TUNE6_0    0x0
#define PRCM_AUDIO_PLL_TUNE0_TUNE6_1    0x1
#define PRCM_AUDIO_PLL_TUNE0_TUNE7_0    0x0
#define PRCM_AUDIO_PLL_TUNE0_TUNE7_1    0x1
#define PRCM_AUDIO_PLL_TUNE0_TUNE8_0    0x0
#define PRCM_AUDIO_PLL_TUNE0_TUNE8_1    0x1
#define PRCM_AUDIO_PLL_TUNE0_TUNE10_0   0x0
#define PRCM_AUDIO_PLL_TUNE0_TUNE10_1   0x1
#define PRCM_AUDIO_PLL_TUNE0_TUNE11_0   0x0
#define PRCM_AUDIO_PLL_TUNE0_TUNE11_1   0x1
#define PRCM_AUDIO_PLL_TUNE0_TUNE11_2   0x2
#define PRCM_AUDIO_PLL_TUNE0_TUNE11_3   0x3
#define PRCM_AUDIO_PLL_TUNE0_TUNE12_0   0x0
#define PRCM_AUDIO_PLL_TUNE0_TUNE12_1   0x1

/* AUDIO_PLL Test Enable Register */
typedef union {
	u32 val;
	struct {
	u32 ten0:1;     /**/
	u32 rsvd0:1;    /**/
	u32 ten1:1;     /**/
	u32 rsvd1:29;   /**/
	} bits;
} reg_prcm_audio_pll_ten_t;

#define PRCM_AUDIO_PLL_TEN_TEN0_0       0x0
#define PRCM_AUDIO_PLL_TEN_TEN0_1       0x1
#define PRCM_AUDIO_PLL_TEN_TEN1_0       0x0
#define PRCM_AUDIO_PLL_TEN_TEN1_1       0x1

/* AUDIO_PLL Status Register */
typedef union {
	u32 val;
	struct {
	u32 dlock:1;    /**/
	u32 asteady:1;  /**/
	u32 alock:1;    /**/
	u32 rsvd0:29;   /**/
	} bits;
} reg_prcm_audio_pll_stat_t;

#define PRCM_AUDIO_PLL_STAT_DLOCK_0     0x0
#define PRCM_AUDIO_PLL_STAT_DLOCK_1     0x1
#define PRCM_AUDIO_PLL_STAT_ASTEADY_0   0x0
#define PRCM_AUDIO_PLL_STAT_ASTEADY_1   0x1
#define PRCM_AUDIO_PLL_STAT_ALOCK_0     0x0
#define PRCM_AUDIO_PLL_STAT_ALOCK_1     0x1

/* AUDIO_PLL Mode Register */
typedef union {
	u32 val;
	struct {
	u32 mod:2;      /**/
	u32 rsvd0:30;   /**/
	} bits;
} reg_prcm_audio_pll_mod_t;

#define PRCM_AUDIO_PLL_MOD_MOD_0        0x0
#define PRCM_AUDIO_PLL_MOD_MOD_1        0x1
#define PRCM_AUDIO_PLL_MOD_MOD_2        0x2
#define PRCM_AUDIO_PLL_MOD_MOD_3        0x3

/* AUDIO_PLL N Factor Fraction Register */
typedef union {
	u32 val;
	struct {
	u32 nfrac:18;   /**/
	u32 rsvd0:14;   /**/
	} bits;
} reg_prcm_audio_pll_nfrac_t;

/* VC_PLL Enable Register */
typedef union {
	u32 val;
	struct {
	u32 enp:1;      /**/
	u32 enm:1;      /**/
	u32 oen:1;      /**/
	u32 rsvd0:1;    /**/
	u32 ren:1;      /**/
	u32 rsvd1:27;   /**/
	} bits;
} reg_prcm_vc_pll_en_t;

#define PRCM_VC_PLL_EN_ENP_0    0x0
#define PRCM_VC_PLL_EN_ENP_1    0x1
#define PRCM_VC_PLL_EN_ENM_0    0x0
#define PRCM_VC_PLL_EN_ENM_1    0x1
#define PRCM_VC_PLL_EN_OEN_0    0x0
#define PRCM_VC_PLL_EN_OEN_1    0x1
#define PRCM_VC_PLL_EN_REN_0    0x0
#define PRCM_VC_PLL_EN_REN_1    0x1

/* VC_PLL Factor Register */
typedef union {
	u32 val;
	struct {
	u32 pre_div:2;  /**/
	u32 rsvd0:2;    /**/
	u32 post_div:2; /**/
	u32 rsvd1:2;    /**/
	u32 n:8;        /**/
	u32 rsvd2:16;   /**/
	} bits;
} reg_prcm_vc_pll_fac_t;

#define PRCM_VC_PLL_FAC_PRE_DIV_0       0x0
#define PRCM_VC_PLL_FAC_PRE_DIV_1       0x1
#define PRCM_VC_PLL_FAC_PRE_DIV_2       0x2
#define PRCM_VC_PLL_FAC_PRE_DIV_3       0x3
#define PRCM_VC_PLL_FAC_POST_DIV_0      0x0
#define PRCM_VC_PLL_FAC_POST_DIV_1      0x1
#define PRCM_VC_PLL_FAC_POST_DIV_2      0x2
#define PRCM_VC_PLL_FAC_POST_DIV_3      0x3

/* VC_PLL Tune0 Register */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:3;    /**/
	u32 tune1:3;    /**/
	u32 tune2:5;    /**/
	u32 tune3:1;    /**/
	u32 tune4:3;    /**/
	u32 tune5:1;    /**/
	u32 tune6:1;    /**/
	u32 tune7:1;    /**/
	u32 tune8:1;    /**/
	u32 tune9:3;    /**/
	u32 rsvd1:3;    /**/
	u32 tune10:1;   /**/
	u32 rsvd2:6;    /**/
	} bits;
} reg_prcm_vc_pll_tune0_t;

#define PRCM_VC_PLL_TUNE0_TUNE1_0       0x0
#define PRCM_VC_PLL_TUNE0_TUNE1_1       0x1
#define PRCM_VC_PLL_TUNE0_TUNE1_2       0x2
#define PRCM_VC_PLL_TUNE0_TUNE1_3       0x3
#define PRCM_VC_PLL_TUNE0_TUNE1_4       0x4
#define PRCM_VC_PLL_TUNE0_TUNE1_5       0x5
#define PRCM_VC_PLL_TUNE0_TUNE1_6       0x6
#define PRCM_VC_PLL_TUNE0_TUNE1_7       0x7
#define PRCM_VC_PLL_TUNE0_TUNE3_0       0x0
#define PRCM_VC_PLL_TUNE0_TUNE3_1       0x1
#define PRCM_VC_PLL_TUNE0_TUNE4_0       0x0
#define PRCM_VC_PLL_TUNE0_TUNE4_1       0x1
#define PRCM_VC_PLL_TUNE0_TUNE4_2       0x2
#define PRCM_VC_PLL_TUNE0_TUNE4_3       0x3
#define PRCM_VC_PLL_TUNE0_TUNE4_4       0x4
#define PRCM_VC_PLL_TUNE0_TUNE4_5       0x5
#define PRCM_VC_PLL_TUNE0_TUNE4_6       0x6
#define PRCM_VC_PLL_TUNE0_TUNE4_7       0x7
#define PRCM_VC_PLL_TUNE0_TUNE5_0       0x0
#define PRCM_VC_PLL_TUNE0_TUNE5_1       0x1
#define PRCM_VC_PLL_TUNE0_TUNE6_0       0x0
#define PRCM_VC_PLL_TUNE0_TUNE6_1       0x1
#define PRCM_VC_PLL_TUNE0_TUNE7_0       0x0
#define PRCM_VC_PLL_TUNE0_TUNE7_1       0x1
#define PRCM_VC_PLL_TUNE0_TUNE8_0       0x0
#define PRCM_VC_PLL_TUNE0_TUNE8_1       0x1
#define PRCM_VC_PLL_TUNE0_TUNE10_0      0x0
#define PRCM_VC_PLL_TUNE0_TUNE10_1      0x1

/* VC_PLL Test Enable Register */
typedef union {
	u32 val;
	struct {
	u32 ten0:1;     /**/
	u32 rsvd0:1;    /**/
	u32 ten1:1;     /**/
	u32 rsvd1:29;   /**/
	} bits;
} reg_prcm_vc_pll_ten_t;

#define PRCM_VC_PLL_TEN_TEN0_0  0x0
#define PRCM_VC_PLL_TEN_TEN0_1  0x1
#define PRCM_VC_PLL_TEN_TEN1_0  0x0
#define PRCM_VC_PLL_TEN_TEN1_1  0x1

/* VC_PLL Status Register */
typedef union {
	u32 val;
	struct {
	u32 dlock:1;    /**/
	u32 asteady:1;  /**/
	u32 alock:1;    /**/
	u32 rsvd0:29;   /**/
	} bits;
} reg_prcm_vc_pll_stat_t;

#define PRCM_VC_PLL_STAT_DLOCK_0        0x0
#define PRCM_VC_PLL_STAT_DLOCK_1        0x1
#define PRCM_VC_PLL_STAT_ASTEADY_0      0x0
#define PRCM_VC_PLL_STAT_ASTEADY_1      0x1
#define PRCM_VC_PLL_STAT_ALOCK_0        0x0
#define PRCM_VC_PLL_STAT_ALOCK_1        0x1

/* VC_PLL Mode Register */
typedef union {
	u32 val;
	struct {
	u32 mod:2;      /**/
	u32 rsvd0:30;   /**/
	} bits;
} reg_prcm_vc_pll_mod_t;

#define PRCM_VC_PLL_MOD_MOD_0   0x0
#define PRCM_VC_PLL_MOD_MOD_1   0x1
#define PRCM_VC_PLL_MOD_MOD_2   0x2
#define PRCM_VC_PLL_MOD_MOD_3   0x3

/* VC_PLL N Factor Fraction Register */
typedef union {
	u32 val;
	struct {
	u32 nfrac:18;   /**/
	u32 rsvd0:14;   /**/
	} bits;
} reg_prcm_vc_pll_nfrac_t;

/* PERH1_PLL Enable Register */
typedef union {
	u32 val;
	struct {
	u32 enp:1;      /**/
	u32 enm:1;      /**/
	u32 oen:1;      /**/
	u32 rsvd0:1;    /**/
	u32 ren:1;      /**/
	u32 rsvd1:27;   /**/
	} bits;
} reg_prcm_perh1_pll_en_t;

#define PRCM_PERH1_PLL_EN_ENP_0         0x0
#define PRCM_PERH1_PLL_EN_ENP_1         0x1
#define PRCM_PERH1_PLL_EN_ENM_0         0x0
#define PRCM_PERH1_PLL_EN_ENM_1         0x1
#define PRCM_PERH1_PLL_EN_OEN_0         0x0
#define PRCM_PERH1_PLL_EN_OEN_1         0x1
#define PRCM_PERH1_PLL_EN_REN_0         0x0
#define PRCM_PERH1_PLL_EN_REN_1         0x1

/* PERH1_PLL Factor Register */
typedef union {
	u32 val;
	struct {
	u32 pre_div:2;  /**/
	u32 rsvd0:2;    /**/
	u32 post_div:2; /**/
	u32 rsvd1:2;    /**/
	u32 n:8;        /**/
	u32 rsvd2:16;   /**/
	} bits;
} reg_prcm_perh1_pll_fac_t;

#define PRCM_PERH1_PLL_FAC_PRE_DIV_0    0x0
#define PRCM_PERH1_PLL_FAC_PRE_DIV_1    0x1
#define PRCM_PERH1_PLL_FAC_PRE_DIV_2    0x2
#define PRCM_PERH1_PLL_FAC_PRE_DIV_3    0x3
#define PRCM_PERH1_PLL_FAC_POST_DIV_0   0x0
#define PRCM_PERH1_PLL_FAC_POST_DIV_1   0x1
#define PRCM_PERH1_PLL_FAC_POST_DIV_2   0x2
#define PRCM_PERH1_PLL_FAC_POST_DIV_3   0x3

/* PERH1_PLL Tune0 Register */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:3;    /**/
	u32 tune1:3;    /**/
	u32 tune2:5;    /**/
	u32 tune3:1;    /**/
	u32 tune4:3;    /**/
	u32 tune5:1;    /**/
	u32 tune6:1;    /**/
	u32 tune7:1;    /**/
	u32 tune8:1;    /**/
	u32 tune9:3;    /**/
	u32 tune10:1;   /**/
	u32 tune11:2;   /**/
	u32 tune12:1;   /**/
	u32 rsvd1:6;    /**/
	} bits;
} reg_prcm_perh1_pll_tune0_t;

#define PRCM_PERH1_PLL_TUNE0_TUNE1_0    0x0
#define PRCM_PERH1_PLL_TUNE0_TUNE1_1    0x1
#define PRCM_PERH1_PLL_TUNE0_TUNE1_2    0x2
#define PRCM_PERH1_PLL_TUNE0_TUNE1_3    0x3
#define PRCM_PERH1_PLL_TUNE0_TUNE1_4    0x4
#define PRCM_PERH1_PLL_TUNE0_TUNE1_5    0x5
#define PRCM_PERH1_PLL_TUNE0_TUNE1_6    0x6
#define PRCM_PERH1_PLL_TUNE0_TUNE1_7    0x7
#define PRCM_PERH1_PLL_TUNE0_TUNE3_0    0x0
#define PRCM_PERH1_PLL_TUNE0_TUNE3_1    0x1
#define PRCM_PERH1_PLL_TUNE0_TUNE4_0    0x0
#define PRCM_PERH1_PLL_TUNE0_TUNE4_1    0x1
#define PRCM_PERH1_PLL_TUNE0_TUNE4_2    0x2
#define PRCM_PERH1_PLL_TUNE0_TUNE4_3    0x3
#define PRCM_PERH1_PLL_TUNE0_TUNE4_4    0x4
#define PRCM_PERH1_PLL_TUNE0_TUNE4_5    0x5
#define PRCM_PERH1_PLL_TUNE0_TUNE4_6    0x6
#define PRCM_PERH1_PLL_TUNE0_TUNE4_7    0x7
#define PRCM_PERH1_PLL_TUNE0_TUNE5_0    0x0
#define PRCM_PERH1_PLL_TUNE0_TUNE5_1    0x1
#define PRCM_PERH1_PLL_TUNE0_TUNE6_0    0x0
#define PRCM_PERH1_PLL_TUNE0_TUNE6_1    0x1
#define PRCM_PERH1_PLL_TUNE0_TUNE7_0    0x0
#define PRCM_PERH1_PLL_TUNE0_TUNE7_1    0x1
#define PRCM_PERH1_PLL_TUNE0_TUNE8_0    0x0
#define PRCM_PERH1_PLL_TUNE0_TUNE8_1    0x1
#define PRCM_PERH1_PLL_TUNE0_TUNE10_0   0x0
#define PRCM_PERH1_PLL_TUNE0_TUNE10_1   0x1
#define PRCM_PERH1_PLL_TUNE0_TUNE11_0   0x0
#define PRCM_PERH1_PLL_TUNE0_TUNE11_1   0x1
#define PRCM_PERH1_PLL_TUNE0_TUNE11_2   0x2
#define PRCM_PERH1_PLL_TUNE0_TUNE11_3   0x3
#define PRCM_PERH1_PLL_TUNE0_TUNE12_0   0x0
#define PRCM_PERH1_PLL_TUNE0_TUNE12_1   0x1

/* PERH1_PLL Test Enable Register */
typedef union {
	u32 val;
	struct {
	u32 ten0:1;     /**/
	u32 rsvd0:1;    /**/
	u32 ten1:1;     /**/
	u32 rsvd1:29;   /**/
	} bits;
} reg_prcm_perh1_pll_ten_t;

#define PRCM_PERH1_PLL_TEN_TEN0_0       0x0
#define PRCM_PERH1_PLL_TEN_TEN0_1       0x1
#define PRCM_PERH1_PLL_TEN_TEN1_0       0x0
#define PRCM_PERH1_PLL_TEN_TEN1_1       0x1

/* PERH1_PLL Status Register */
typedef union {
	u32 val;
	struct {
	u32 dlock:1;    /**/
	u32 asteady:1;  /**/
	u32 alock:1;    /**/
	u32 rsvd0:29;   /**/
	} bits;
} reg_prcm_perh1_pll_stat_t;

#define PRCM_PERH1_PLL_STAT_DLOCK_0     0x0
#define PRCM_PERH1_PLL_STAT_DLOCK_1     0x1
#define PRCM_PERH1_PLL_STAT_ASTEADY_0   0x0
#define PRCM_PERH1_PLL_STAT_ASTEADY_1   0x1
#define PRCM_PERH1_PLL_STAT_ALOCK_0     0x0
#define PRCM_PERH1_PLL_STAT_ALOCK_1     0x1

/* PERH1_PLL Mode Register */
typedef union {
	u32 val;
	struct {
	u32 mod:2;      /**/
	u32 rsvd0:30;   /**/
	} bits;
} reg_prcm_perh1_pll_mod_t;

#define PRCM_PERH1_PLL_MOD_MOD_0        0x0
#define PRCM_PERH1_PLL_MOD_MOD_1        0x1
#define PRCM_PERH1_PLL_MOD_MOD_2        0x2
#define PRCM_PERH1_PLL_MOD_MOD_3        0x3

/* PERH1_PLL N Factor Fraction Register */
typedef union {
	u32 val;
	struct {
	u32 nfrac:18;   /**/
	u32 rsvd0:14;   /**/
	} bits;
} reg_prcm_perh1_pll_nfrac_t;

/* PERH1_PLL Tune1 Register */
typedef union {
	u32 val;
	struct {
	u32 period:12;  /**/
	u32 rsvd0:4;    /**/
	u32 amplitude:12;/**/
	u32 rsvd1:4;    /**/
	} bits;
} reg_prcm_perh1_pll_tune1_t;

/* DISP_PLL Enable Register */
typedef union {
	u32 val;
	struct {
	u32 enp:1;      /**/
	u32 enm:1;      /**/
	u32 oen:1;      /**/
	u32 rsvd0:1;    /**/
	u32 ren:1;      /**/
	u32 rsvd1:27;   /**/
	} bits;
} reg_prcm_disp_pll_en_t;

#define PRCM_DISP_PLL_EN_ENP_0  0x0
#define PRCM_DISP_PLL_EN_ENP_1  0x1
#define PRCM_DISP_PLL_EN_ENM_0  0x0
#define PRCM_DISP_PLL_EN_ENM_1  0x1
#define PRCM_DISP_PLL_EN_OEN_0  0x0
#define PRCM_DISP_PLL_EN_OEN_1  0x1
#define PRCM_DISP_PLL_EN_REN_0  0x0
#define PRCM_DISP_PLL_EN_REN_1  0x1

/* DISP_PLL Factor Register */
typedef union {
	u32 val;
	struct {
	u32 pre_div:2;  /**/
	u32 rsvd0:2;    /**/
	u32 post_div:2; /**/
	u32 rsvd1:2;    /**/
	u32 n:8;        /**/
	u32 rsvd2:16;   /**/
	} bits;
} reg_prcm_disp_pll_fac_t;

#define PRCM_DISP_PLL_FAC_PRE_DIV_0     0x0
#define PRCM_DISP_PLL_FAC_PRE_DIV_1     0x1
#define PRCM_DISP_PLL_FAC_PRE_DIV_2     0x2
#define PRCM_DISP_PLL_FAC_PRE_DIV_3     0x3
#define PRCM_DISP_PLL_FAC_POST_DIV_0    0x0
#define PRCM_DISP_PLL_FAC_POST_DIV_1    0x1
#define PRCM_DISP_PLL_FAC_POST_DIV_2    0x2
#define PRCM_DISP_PLL_FAC_POST_DIV_3    0x3

/* DISP_PLL Tune0 Register */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:3;    /**/
	u32 tune1:3;    /**/
	u32 tune2:5;    /**/
	u32 tune3:1;    /**/
	u32 tune4:3;    /**/
	u32 tune5:1;    /**/
	u32 tune6:1;    /**/
	u32 tune7:1;    /**/
	u32 tune8:1;    /**/
	u32 tune9:3;    /**/
	u32 tune10:1;   /**/
	u32 tune11:2;   /**/
	u32 tune12:1;   /**/
	u32 rsvd1:6;    /**/
	} bits;
} reg_prcm_disp_pll_tune0_t;

#define PRCM_DISP_PLL_TUNE0_TUNE1_0     0x0
#define PRCM_DISP_PLL_TUNE0_TUNE1_1     0x1
#define PRCM_DISP_PLL_TUNE0_TUNE1_2     0x2
#define PRCM_DISP_PLL_TUNE0_TUNE1_3     0x3
#define PRCM_DISP_PLL_TUNE0_TUNE1_4     0x4
#define PRCM_DISP_PLL_TUNE0_TUNE1_5     0x5
#define PRCM_DISP_PLL_TUNE0_TUNE1_6     0x6
#define PRCM_DISP_PLL_TUNE0_TUNE1_7     0x7
#define PRCM_DISP_PLL_TUNE0_TUNE3_0     0x0
#define PRCM_DISP_PLL_TUNE0_TUNE3_1     0x1
#define PRCM_DISP_PLL_TUNE0_TUNE4_0     0x0
#define PRCM_DISP_PLL_TUNE0_TUNE4_1     0x1
#define PRCM_DISP_PLL_TUNE0_TUNE4_2     0x2
#define PRCM_DISP_PLL_TUNE0_TUNE4_3     0x3
#define PRCM_DISP_PLL_TUNE0_TUNE4_4     0x4
#define PRCM_DISP_PLL_TUNE0_TUNE4_5     0x5
#define PRCM_DISP_PLL_TUNE0_TUNE4_6     0x6
#define PRCM_DISP_PLL_TUNE0_TUNE4_7     0x7
#define PRCM_DISP_PLL_TUNE0_TUNE5_0     0x0
#define PRCM_DISP_PLL_TUNE0_TUNE5_1     0x1
#define PRCM_DISP_PLL_TUNE0_TUNE6_0     0x0
#define PRCM_DISP_PLL_TUNE0_TUNE6_1     0x1
#define PRCM_DISP_PLL_TUNE0_TUNE7_0     0x0
#define PRCM_DISP_PLL_TUNE0_TUNE7_1     0x1
#define PRCM_DISP_PLL_TUNE0_TUNE8_0     0x0
#define PRCM_DISP_PLL_TUNE0_TUNE8_1     0x1
#define PRCM_DISP_PLL_TUNE0_TUNE10_0    0x0
#define PRCM_DISP_PLL_TUNE0_TUNE10_1    0x1
#define PRCM_DISP_PLL_TUNE0_TUNE11_0    0x0
#define PRCM_DISP_PLL_TUNE0_TUNE11_1    0x1
#define PRCM_DISP_PLL_TUNE0_TUNE11_2    0x2
#define PRCM_DISP_PLL_TUNE0_TUNE11_3    0x3
#define PRCM_DISP_PLL_TUNE0_TUNE12_0    0x0
#define PRCM_DISP_PLL_TUNE0_TUNE12_1    0x1

/* DISP_PLL Test Enable Register */
typedef union {
	u32 val;
	struct {
	u32 ten0:1;     /**/
	u32 rsvd0:1;    /**/
	u32 ten1:1;     /**/
	u32 rsvd1:29;   /**/
	} bits;
} reg_prcm_disp_pll_ten_t;

#define PRCM_DISP_PLL_TEN_TEN0_0        0x0
#define PRCM_DISP_PLL_TEN_TEN0_1        0x1
#define PRCM_DISP_PLL_TEN_TEN1_0        0x0
#define PRCM_DISP_PLL_TEN_TEN1_1        0x1

/* DISP_PLL Status Register */
typedef union {
	u32 val;
	struct {
	u32 dlock:1;    /**/
	u32 asteady:1;  /**/
	u32 alock:1;    /**/
	u32 rsvd0:29;   /**/
	} bits;
} reg_prcm_disp_pll_stat_t;

#define PRCM_DISP_PLL_STAT_DLOCK_0      0x0
#define PRCM_DISP_PLL_STAT_DLOCK_1      0x1
#define PRCM_DISP_PLL_STAT_ASTEADY_0    0x0
#define PRCM_DISP_PLL_STAT_ASTEADY_1    0x1
#define PRCM_DISP_PLL_STAT_ALOCK_0      0x0
#define PRCM_DISP_PLL_STAT_ALOCK_1      0x1

/* DISP_PLL Mode Register */
typedef union {
	u32 val;
	struct {
	u32 mod:2;      /**/
	u32 rsvd0:30;   /**/
	} bits;
} reg_prcm_disp_pll_mod_t;

#define PRCM_DISP_PLL_MOD_MOD_0         0x0
#define PRCM_DISP_PLL_MOD_MOD_1         0x1
#define PRCM_DISP_PLL_MOD_MOD_2         0x2
#define PRCM_DISP_PLL_MOD_MOD_3         0x3

/* DISP_PLL N Factor Fraction Register */
typedef union {
	u32 val;
	struct {
	u32 nfrac:18;   /**/
	u32 rsvd0:14;   /**/
	} bits;
} reg_prcm_disp_pll_nfrac_t;

/* DISP_PLL Tune1 Register */
typedef union {
	u32 val;
	struct {
	u32 period:12;  /**/
	u32 rsvd0:4;    /**/
	u32 amplitude:12;/**/
	u32 rsvd1:4;    /**/
	} bits;
} reg_prcm_disp_pll_tune1_t;

/* AX_PLL Enable Register */
typedef union {
	u32 val;
	struct {
	u32 enp:1;      /**/
	u32 enm:1;      /**/
	u32 oen:1;      /**/
	u32 rsvd0:1;    /**/
	u32 ren:1;      /**/
	u32 rsvd1:27;   /**/
	} bits;
} reg_prcm_ax_pll_en_t;

#define PRCM_AX_PLL_EN_ENP_0    0x0
#define PRCM_AX_PLL_EN_ENP_1    0x1
#define PRCM_AX_PLL_EN_ENM_0    0x0
#define PRCM_AX_PLL_EN_ENM_1    0x1
#define PRCM_AX_PLL_EN_OEN_0    0x0
#define PRCM_AX_PLL_EN_OEN_1    0x1
#define PRCM_AX_PLL_EN_REN_0    0x0
#define PRCM_AX_PLL_EN_REN_1    0x1

/* AX_PLL Factor Register */
typedef union {
	u32 val;
	struct {
	u32 pre_div:2;  /**/
	u32 rsvd0:2;    /**/
	u32 post_div:2; /**/
	u32 rsvd1:2;    /**/
	u32 n:8;        /**/
	u32 rsvd2:16;   /**/
	} bits;
} reg_prcm_ax_pll_fac_t;

#define PRCM_AX_PLL_FAC_PRE_DIV_0       0x0
#define PRCM_AX_PLL_FAC_PRE_DIV_1       0x1
#define PRCM_AX_PLL_FAC_PRE_DIV_2       0x2
#define PRCM_AX_PLL_FAC_PRE_DIV_3       0x3
#define PRCM_AX_PLL_FAC_POST_DIV_0      0x0
#define PRCM_AX_PLL_FAC_POST_DIV_1      0x1
#define PRCM_AX_PLL_FAC_POST_DIV_2      0x2
#define PRCM_AX_PLL_FAC_POST_DIV_3      0x3

/* AX_PLL Tune0 Register */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:3;    /**/
	u32 tune1:3;    /**/
	u32 rsvd1:3;    /**/
	u32 tune2:5;    /**/
	u32 tune3:1;    /**/
	u32 tune4:3;    /**/
	u32 tune5:1;    /**/
	u32 tune6:1;    /**/
	u32 tune7:1;    /**/
	u32 tune8:1;    /**/
	u32 tune9:3;    /**/
	u32 tune10:1;   /**/
	u32 tune11:2;   /**/
	u32 tune12:1;   /**/
	u32 rsvd2:3;    /**/
	} bits;
} reg_prcm_ax_pll_tune0_t;

#define PRCM_AX_PLL_TUNE0_TUNE1_0       0x0
#define PRCM_AX_PLL_TUNE0_TUNE1_1       0x1
#define PRCM_AX_PLL_TUNE0_TUNE1_2       0x2
#define PRCM_AX_PLL_TUNE0_TUNE1_3       0x3
#define PRCM_AX_PLL_TUNE0_TUNE1_4       0x4
#define PRCM_AX_PLL_TUNE0_TUNE1_5       0x5
#define PRCM_AX_PLL_TUNE0_TUNE1_6       0x6
#define PRCM_AX_PLL_TUNE0_TUNE1_7       0x7
#define PRCM_AX_PLL_TUNE0_TUNE3_0       0x0
#define PRCM_AX_PLL_TUNE0_TUNE3_1       0x1
#define PRCM_AX_PLL_TUNE0_TUNE4_0       0x0
#define PRCM_AX_PLL_TUNE0_TUNE4_1       0x1
#define PRCM_AX_PLL_TUNE0_TUNE4_2       0x2
#define PRCM_AX_PLL_TUNE0_TUNE4_3       0x3
#define PRCM_AX_PLL_TUNE0_TUNE4_4       0x4
#define PRCM_AX_PLL_TUNE0_TUNE4_5       0x5
#define PRCM_AX_PLL_TUNE0_TUNE4_6       0x6
#define PRCM_AX_PLL_TUNE0_TUNE4_7       0x7
#define PRCM_AX_PLL_TUNE0_TUNE5_0       0x0
#define PRCM_AX_PLL_TUNE0_TUNE5_1       0x1
#define PRCM_AX_PLL_TUNE0_TUNE6_0       0x0
#define PRCM_AX_PLL_TUNE0_TUNE6_1       0x1
#define PRCM_AX_PLL_TUNE0_TUNE7_0       0x0
#define PRCM_AX_PLL_TUNE0_TUNE7_1       0x1
#define PRCM_AX_PLL_TUNE0_TUNE8_0       0x0
#define PRCM_AX_PLL_TUNE0_TUNE8_1       0x1
#define PRCM_AX_PLL_TUNE0_TUNE10_0      0x0
#define PRCM_AX_PLL_TUNE0_TUNE10_1      0x1
#define PRCM_AX_PLL_TUNE0_TUNE11_0      0x0
#define PRCM_AX_PLL_TUNE0_TUNE11_1      0x1
#define PRCM_AX_PLL_TUNE0_TUNE11_2      0x2
#define PRCM_AX_PLL_TUNE0_TUNE11_3      0x3
#define PRCM_AX_PLL_TUNE0_TUNE12_0      0x0
#define PRCM_AX_PLL_TUNE0_TUNE12_1      0x1

/* AX_PLL Test Enable Register */
typedef union {
	u32 val;
	struct {
	u32 ten0:1;     /**/
	u32 rsvd0:2;    /**/
	u32 ten1:1;     /**/
	u32 rsvd1:28;   /**/
	} bits;
} reg_prcm_ax_pll_ten_t;

#define PRCM_AX_PLL_TEN_TEN0_0  0x0
#define PRCM_AX_PLL_TEN_TEN0_1  0x1
#define PRCM_AX_PLL_TEN_TEN1_0  0x0
#define PRCM_AX_PLL_TEN_TEN1_1  0x1

/* AX_PLL Status Register */
typedef union {
	u32 val;
	struct {
	u32 dlock:1;    /**/
	u32 asteady:1;  /**/
	u32 alock:1;    /**/
	u32 rsvd0:29;   /**/
	} bits;
} reg_prcm_ax_pll_stat_t;

#define PRCM_AX_PLL_STAT_DLOCK_0        0x0
#define PRCM_AX_PLL_STAT_DLOCK_1        0x1
#define PRCM_AX_PLL_STAT_ASTEADY_0      0x0
#define PRCM_AX_PLL_STAT_ASTEADY_1      0x1
#define PRCM_AX_PLL_STAT_ALOCK_0        0x0
#define PRCM_AX_PLL_STAT_ALOCK_1        0x1

/* AX_PLL Mode Register */
typedef union {
	u32 val;
	struct {
	u32 mod:2;      /**/
	u32 rsvd0:30;   /**/
	} bits;
} reg_prcm_ax_pll_mod_t;

#define PRCM_AX_PLL_MOD_MOD_0   0x0
#define PRCM_AX_PLL_MOD_MOD_1   0x1
#define PRCM_AX_PLL_MOD_MOD_2   0x2
#define PRCM_AX_PLL_MOD_MOD_3   0x3

/* AX_PLL N Factor Fraction Register */
typedef union {
	u32 val;
	struct {
	u32 nfrac:18;   /**/
	u32 rsvd0:14;   /**/
	} bits;
} reg_prcm_ax_pll_nfrac_t;

/* AX_PLL Tune2 Register */
typedef union {
	u32 val;
	struct {
	u32 en:1;       /**/
	u32 rsvd0:15;   /**/
	u32 step:14;    /**/
	u32 rsvd1:2;    /**/
	} bits;
} reg_prcm_ax_pll_tune2_t;

#define PRCM_AX_PLL_TUNE2_EN_0          0x0
#define PRCM_AX_PLL_TUNE2_EN_1          0x1

/* PERH2_PLL Enable Register */
typedef union {
	u32 val;
	struct {
	u32 enp:1;      /**/
	u32 enm:1;      /**/
	u32 oen:1;      /**/
	u32 rsvd0:1;    /**/
	u32 ren:1;      /**/
	u32 rsvd1:27;   /**/
	} bits;
} reg_prcm_perh2_pll_en_t;

#define PRCM_PERH2_PLL_EN_ENP_0         0x0
#define PRCM_PERH2_PLL_EN_ENP_1         0x1
#define PRCM_PERH2_PLL_EN_ENM_0         0x0
#define PRCM_PERH2_PLL_EN_ENM_1         0x1
#define PRCM_PERH2_PLL_EN_OEN_0         0x0
#define PRCM_PERH2_PLL_EN_OEN_1         0x1
#define PRCM_PERH2_PLL_EN_REN_0         0x0
#define PRCM_PERH2_PLL_EN_REN_1         0x1

/* PERH2_PLL Factor Register */
typedef union {
	u32 val;
	struct {
	u32 pre_div:2;  /**/
	u32 rsvd0:2;    /**/
	u32 post_div:2; /**/
	u32 rsvd1:2;    /**/
	u32 n:8;        /**/
	u32 rsvd2:16;   /**/
	} bits;
} reg_prcm_perh2_pll_fac_t;

#define PRCM_PERH2_PLL_FAC_PRE_DIV_0    0x0
#define PRCM_PERH2_PLL_FAC_PRE_DIV_1    0x1
#define PRCM_PERH2_PLL_FAC_PRE_DIV_2    0x2
#define PRCM_PERH2_PLL_FAC_PRE_DIV_3    0x3
#define PRCM_PERH2_PLL_FAC_POST_DIV_0   0x0
#define PRCM_PERH2_PLL_FAC_POST_DIV_1   0x1
#define PRCM_PERH2_PLL_FAC_POST_DIV_2   0x2
#define PRCM_PERH2_PLL_FAC_POST_DIV_3   0x3

/* PERH2_PLL Tune0 Register */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:3;    /**/
	u32 tune1:3;    /**/
	u32 tune2:5;    /**/
	u32 tune3:1;    /**/
	u32 tune4:3;    /**/
	u32 tune5:1;    /**/
	u32 tune6:1;    /**/
	u32 tune7:1;    /**/
	u32 tune8:1;    /**/
	u32 tune9:3;    /**/
	u32 tune10:1;   /**/
	u32 tune11:2;   /**/
	u32 tune12:1;   /**/
	u32 rsvd1:6;    /**/
	} bits;
} reg_prcm_perh2_pll_tune0_t;

#define PRCM_PERH2_PLL_TUNE0_TUNE1_0    0x0
#define PRCM_PERH2_PLL_TUNE0_TUNE1_1    0x1
#define PRCM_PERH2_PLL_TUNE0_TUNE1_2    0x2
#define PRCM_PERH2_PLL_TUNE0_TUNE1_3    0x3
#define PRCM_PERH2_PLL_TUNE0_TUNE1_4    0x4
#define PRCM_PERH2_PLL_TUNE0_TUNE1_5    0x5
#define PRCM_PERH2_PLL_TUNE0_TUNE1_6    0x6
#define PRCM_PERH2_PLL_TUNE0_TUNE1_7    0x7
#define PRCM_PERH2_PLL_TUNE0_TUNE3_0    0x0
#define PRCM_PERH2_PLL_TUNE0_TUNE3_1    0x1
#define PRCM_PERH2_PLL_TUNE0_TUNE4_0    0x0
#define PRCM_PERH2_PLL_TUNE0_TUNE4_1    0x1
#define PRCM_PERH2_PLL_TUNE0_TUNE4_2    0x2
#define PRCM_PERH2_PLL_TUNE0_TUNE4_3    0x3
#define PRCM_PERH2_PLL_TUNE0_TUNE4_4    0x4
#define PRCM_PERH2_PLL_TUNE0_TUNE4_5    0x5
#define PRCM_PERH2_PLL_TUNE0_TUNE4_6    0x6
#define PRCM_PERH2_PLL_TUNE0_TUNE4_7    0x7
#define PRCM_PERH2_PLL_TUNE0_TUNE5_0    0x0
#define PRCM_PERH2_PLL_TUNE0_TUNE5_1    0x1
#define PRCM_PERH2_PLL_TUNE0_TUNE6_0    0x0
#define PRCM_PERH2_PLL_TUNE0_TUNE6_1    0x1
#define PRCM_PERH2_PLL_TUNE0_TUNE7_0    0x0
#define PRCM_PERH2_PLL_TUNE0_TUNE7_1    0x1
#define PRCM_PERH2_PLL_TUNE0_TUNE8_0    0x0
#define PRCM_PERH2_PLL_TUNE0_TUNE8_1    0x1
#define PRCM_PERH2_PLL_TUNE0_TUNE10_0   0x0
#define PRCM_PERH2_PLL_TUNE0_TUNE10_1   0x1
#define PRCM_PERH2_PLL_TUNE0_TUNE11_0   0x0
#define PRCM_PERH2_PLL_TUNE0_TUNE11_1   0x1
#define PRCM_PERH2_PLL_TUNE0_TUNE11_2   0x2
#define PRCM_PERH2_PLL_TUNE0_TUNE11_3   0x3
#define PRCM_PERH2_PLL_TUNE0_TUNE12_0   0x0
#define PRCM_PERH2_PLL_TUNE0_TUNE12_1   0x1

/* PERH2_PLL Test Enable Register */
typedef union {
	u32 val;
	struct {
	u32 ten0:1;     /**/
	u32 rsvd0:1;    /**/
	u32 ten1:1;     /**/
	u32 rsvd1:29;   /**/
	} bits;
} reg_prcm_perh2_pll_ten_t;

#define PRCM_PERH2_PLL_TEN_TEN0_0       0x0
#define PRCM_PERH2_PLL_TEN_TEN0_1       0x1
#define PRCM_PERH2_PLL_TEN_TEN1_0       0x0
#define PRCM_PERH2_PLL_TEN_TEN1_1       0x1

/* PERH2_PLL Status Register */
typedef union {
	u32 val;
	struct {
	u32 dlock:1;    /**/
	u32 asteady:1;  /**/
	u32 alock:1;    /**/
	u32 rsvd0:29;   /**/
	} bits;
} reg_prcm_perh2_pll_stat_t;

#define PRCM_PERH2_PLL_STAT_DLOCK_0     0x0
#define PRCM_PERH2_PLL_STAT_DLOCK_1     0x1
#define PRCM_PERH2_PLL_STAT_ASTEADY_0   0x0
#define PRCM_PERH2_PLL_STAT_ASTEADY_1   0x1
#define PRCM_PERH2_PLL_STAT_ALOCK_0     0x0
#define PRCM_PERH2_PLL_STAT_ALOCK_1     0x1

/* PERH2_PLL Mode Register */
typedef union {
	u32 val;
	struct {
	u32 mod:2;      /**/
	u32 rsvd0:30;   /**/
	} bits;
} reg_prcm_perh2_pll_mod_t;

#define PRCM_PERH2_PLL_MOD_MOD_0        0x0
#define PRCM_PERH2_PLL_MOD_MOD_1        0x1
#define PRCM_PERH2_PLL_MOD_MOD_2        0x2
#define PRCM_PERH2_PLL_MOD_MOD_3        0x3

/* PERH2_PLL N Factor Fraction Register */
typedef union {
	u32 val;
	struct {
	u32 nfrac:18;   /**/
	u32 rsvd0:14;   /**/
	} bits;
} reg_prcm_perh2_pll_nfrac_t;

/* PERH2_PLL Tune1 Register */
typedef union {
	u32 val;
	struct {
	u32 period:12;  /**/
	u32 rsvd0:4;    /**/
	u32 amplitude:12;/**/
	u32 rsvd1:4;    /**/
	} bits;
} reg_prcm_perh2_pll_tune1_t;

/* PLL Test Block Control Register */
typedef union {
	u32 val;
	struct {
	u32 en:1;       /**/
	u32 div_endio:1;/**/
	u32 rsvd0:2;    /**/
	u32 sel0:4;     /**/
	u32 sel1:4;     /**/
	u32 rsvd1:4;    /**/
	u32 div0:3;     /**/
	u32 rsvd2:13;   /**/
	} bits;
} reg_prcm_pll_tblk_t;

#define PRCM_PLL_TBLK_EN_0              0x0
#define PRCM_PLL_TBLK_EN_1              0x1
#define PRCM_PLL_TBLK_DIV_ENDIO_0       0x0
#define PRCM_PLL_TBLK_DIV_ENDIO_1       0x1
#define PRCM_PLL_TBLK_SEL0_0            0x0
#define PRCM_PLL_TBLK_SEL0_1            0x1
#define PRCM_PLL_TBLK_SEL0_2            0x2
#define PRCM_PLL_TBLK_SEL0_3            0x3
#define PRCM_PLL_TBLK_SEL0_4            0x4
#define PRCM_PLL_TBLK_SEL0_5            0x5
#define PRCM_PLL_TBLK_SEL0_6            0x6
#define PRCM_PLL_TBLK_SEL0_7            0x7
#define PRCM_PLL_TBLK_SEL0_8            0x8
#define PRCM_PLL_TBLK_SEL0_9            0x9
#define PRCM_PLL_TBLK_SEL1_0            0x0
#define PRCM_PLL_TBLK_SEL1_1            0x1
#define PRCM_PLL_TBLK_SEL1_2            0x2
#define PRCM_PLL_TBLK_SEL1_3            0x3
#define PRCM_PLL_TBLK_SEL1_4            0x4
#define PRCM_PLL_TBLK_SEL1_5            0x5
#define PRCM_PLL_TBLK_SEL1_6            0x6
#define PRCM_PLL_TBLK_SEL1_7            0x7
#define PRCM_PLL_TBLK_SEL1_8            0x8
#define PRCM_PLL_TBLK_SEL1_9            0x9
#define PRCM_PLL_TBLK_DIV0_0            0x0
#define PRCM_PLL_TBLK_DIV0_1            0x1
#define PRCM_PLL_TBLK_DIV0_2            0x2
#define PRCM_PLL_TBLK_DIV0_3            0x3
#define PRCM_PLL_TBLK_DIV0_4            0x4
#define PRCM_PLL_TBLK_DIV0_5            0x5
#define PRCM_PLL_TBLK_DIV0_6            0x6
#define PRCM_PLL_TBLK_DIV0_7            0x7

/* CPU_AXI Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 cpu_src_sel:2;/**/
	u32 rsvd0:2;    /**/
	u32 cpu_src_div:2;/**/
	u32 rsvd1:26;   /**/
	} bits;
} reg_prcm_cpu_axi_clk_ctrl_t;

#define PRCM_CPU_AXI_CLK_CTRL_CPU_SRC_SEL_0     0x0
#define PRCM_CPU_AXI_CLK_CTRL_CPU_SRC_SEL_1     0x1
#define PRCM_CPU_AXI_CLK_CTRL_CPU_SRC_SEL_2     0x2
#define PRCM_CPU_AXI_CLK_CTRL_CPU_SRC_DIV_0     0x0
#define PRCM_CPU_AXI_CLK_CTRL_CPU_SRC_DIV_1     0x1
#define PRCM_CPU_AXI_CLK_CTRL_CPU_SRC_DIV_2     0x2

/* AHB_APB Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 src_sel:2;  /**/
	u32 rsvd0:2;    /**/
	u32 ahb_div:2;  /**/
	u32 rsvd1:26;   /**/
	} bits;
} reg_prcm_ahb_apb_clk_ctrl_t;

#define PRCM_AHB_APB_CLK_CTRL_SRC_SEL_0 0x0
#define PRCM_AHB_APB_CLK_CTRL_SRC_SEL_1 0x1
#define PRCM_AHB_APB_CLK_CTRL_SRC_SEL_2 0x2
#define PRCM_AHB_APB_CLK_CTRL_SRC_SEL_3 0x3

/* MEMORY AXI BUS Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 src_sel:2;  /**/
	u32 rsvd0:2;    /**/
	u32 div:2;      /**/
	u32 rsvd1:25;   /**/
	u32 en:1;       /**/
	} bits;
} reg_prcm_maxi_clk_ctrl_t;

#define PRCM_MAXI_CLK_CTRL_SRC_SEL_0    0x0
#define PRCM_MAXI_CLK_CTRL_SRC_SEL_1    0x1
#define PRCM_MAXI_CLK_CTRL_SRC_SEL_2    0x2
#define PRCM_MAXI_CLK_CTRL_SRC_SEL_3    0x3
#define PRCM_MAXI_CLK_CTRL_DIV_0        0x0
#define PRCM_MAXI_CLK_CTRL_DIV_1        0x1
#define PRCM_MAXI_CLK_CTRL_DIV_2        0x2
#define PRCM_MAXI_CLK_CTRL_DIV_3        0x3
#define PRCM_MAXI_CLK_CTRL_EN_0         0x0
#define PRCM_MAXI_CLK_CTRL_EN_1         0x1

/* SDRAM Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 src_sel:2;  /**/
	u32 rsvd0:2;    /**/
	u32 div:2;      /**/
	u32 rsvd1:25;   /**/
	u32 en:1;       /**/
	} bits;
} reg_prcm_sdram_clk_ctrl_t;

#define PRCM_SDRAM_CLK_CTRL_SRC_SEL_0   0x0
#define PRCM_SDRAM_CLK_CTRL_SRC_SEL_1   0x1
#define PRCM_SDRAM_CLK_CTRL_DIV_0       0x0
#define PRCM_SDRAM_CLK_CTRL_DIV_1       0x1
#define PRCM_SDRAM_CLK_CTRL_DIV_2       0x2
#define PRCM_SDRAM_CLK_CTRL_EN_0        0x0
#define PRCM_SDRAM_CLK_CTRL_EN_1        0x1

/* SDRAM Reset Control Register */
typedef union {
	u32 val;
	struct {
	u32 sdramc:1;   /**/
	u32 pub:1;      /**/
	u32 rsvd0:30;   /**/
	} bits;
} reg_prcm_sdram_rst_t;

#define PRCM_SDRAM_RST_SDRAMC_0 0x0
#define PRCM_SDRAM_RST_SDRAMC_1 0x1
#define PRCM_SDRAM_RST_PUB_0    0x0
#define PRCM_SDRAM_RST_PUB_1    0x1

/* SDRAM Bandwidth Monitor Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 src_sel:2;  /**/
	u32 rsvd0:29;   /**/
	u32 en:1;       /**/
	} bits;
} reg_prcm_sdram_bm_clk_ctrl_t;

#define PRCM_SDRAM_BM_CLK_CTRL_SRC_SEL_0        0x0
#define PRCM_SDRAM_BM_CLK_CTRL_EN_0             0x0
#define PRCM_SDRAM_BM_CLK_CTRL_EN_1             0x1

/* SDC0 Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 src_sel:3;  /**/
	u32 rsvd0:1;    /**/
	u32 div0:4;     /**/
	u32 div1:4;     /**/
	u32 rsvd1:19;   /**/
	u32 en:1;       /**/
	} bits;
} reg_prcm_sdc0_clk_ctrl_t;

#define PRCM_SDC0_CLK_CTRL_SRC_SEL_0    0x0
#define PRCM_SDC0_CLK_CTRL_SRC_SEL_1    0x1
#define PRCM_SDC0_CLK_CTRL_SRC_SEL_2    0x2
#define PRCM_SDC0_CLK_CTRL_SRC_SEL_3    0x3
#define PRCM_SDC0_CLK_CTRL_EN_0         0x0
#define PRCM_SDC0_CLK_CTRL_EN_1         0x1

/* SDC1 Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 src_sel:3;  /**/
	u32 rsvd0:1;    /**/
	u32 div0:4;     /**/
	u32 div1:4;     /**/
	u32 rsvd1:19;   /**/
	u32 en:1;       /**/
	} bits;
} reg_prcm_sdc1_clk_ctrl_t;

#define PRCM_SDC1_CLK_CTRL_SRC_SEL_0    0x0
#define PRCM_SDC1_CLK_CTRL_SRC_SEL_1    0x1
#define PRCM_SDC1_CLK_CTRL_SRC_SEL_2    0x2
#define PRCM_SDC1_CLK_CTRL_SRC_SEL_3    0x3
#define PRCM_SDC1_CLK_CTRL_EN_0         0x0
#define PRCM_SDC1_CLK_CTRL_EN_1         0x1

/* SDC2 Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 src_sel:3;  /**/
	u32 rsvd0:1;    /**/
	u32 div0:4;     /**/
	u32 div1:4;     /**/
	u32 rsvd1:19;   /**/
	u32 en:1;       /**/
	} bits;
} reg_prcm_sdc2_clk_ctrl_t;

#define PRCM_SDC2_CLK_CTRL_SRC_SEL_0    0x0
#define PRCM_SDC2_CLK_CTRL_SRC_SEL_1    0x1
#define PRCM_SDC2_CLK_CTRL_SRC_SEL_2    0x2
#define PRCM_SDC2_CLK_CTRL_SRC_SEL_3    0x3
#define PRCM_SDC2_CLK_CTRL_EN_0         0x0
#define PRCM_SDC2_CLK_CTRL_EN_1         0x1

/* SPI0 Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 src_sel:3;  /**/
	u32 rsvd0:1;    /**/
	u32 div0:3;     /**/
	u32 rsvd1:1;    /**/
	u32 div1:4;     /**/
	u32 rsvd2:19;   /**/
	u32 en:1;       /**/
	} bits;
} reg_prcm_spi0_clk_ctrl_t;

#define PRCM_SPI0_CLK_CTRL_SRC_SEL_0    0x0
#define PRCM_SPI0_CLK_CTRL_SRC_SEL_1    0x1
#define PRCM_SPI0_CLK_CTRL_SRC_SEL_2    0x2
#define PRCM_SPI0_CLK_CTRL_SRC_SEL_3    0x3
#define PRCM_SPI0_CLK_CTRL_EN_0         0x0
#define PRCM_SPI0_CLK_CTRL_EN_1         0x1

/* SPI1 Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 src_sel:3;  /**/
	u32 rsvd0:1;    /**/
	u32 div0:3;     /**/
	u32 rsvd1:1;    /**/
	u32 div1:4;     /**/
	u32 rsvd2:19;   /**/
	u32 en:1;       /**/
	} bits;
} reg_prcm_spi1_clk_ctrl_t;

#define PRCM_SPI1_CLK_CTRL_SRC_SEL_0    0x0
#define PRCM_SPI1_CLK_CTRL_SRC_SEL_1    0x1
#define PRCM_SPI1_CLK_CTRL_SRC_SEL_2    0x2
#define PRCM_SPI1_CLK_CTRL_SRC_SEL_3    0x3
#define PRCM_SPI1_CLK_CTRL_EN_0         0x0
#define PRCM_SPI1_CLK_CTRL_EN_1         0x1

/* SPI2 Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 src_sel:3;  /**/
	u32 rsvd0:1;    /**/
	u32 div0:3;     /**/
	u32 rsvd1:1;    /**/
	u32 div1:4;     /**/
	u32 rsvd2:19;   /**/
	u32 en:1;       /**/
	} bits;
} reg_prcm_spi2_clk_ctrl_t;

#define PRCM_SPI2_CLK_CTRL_SRC_SEL_0    0x0
#define PRCM_SPI2_CLK_CTRL_SRC_SEL_1    0x1
#define PRCM_SPI2_CLK_CTRL_SRC_SEL_2    0x2
#define PRCM_SPI2_CLK_CTRL_SRC_SEL_3    0x3
#define PRCM_SPI2_CLK_CTRL_EN_0         0x0
#define PRCM_SPI2_CLK_CTRL_EN_1         0x1

/* VC_ENC Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 src_sel:3;  /**/
	u32 rsvd0:1;    /**/
	u32 div0:4;     /**/
	u32 rsvd1:23;   /**/
	u32 en:1;       /**/
	} bits;
} reg_prcm_vc_enc_clk_ctrl_t;

#define PRCM_VC_ENC_CLK_CTRL_SRC_SEL_0  0x0
#define PRCM_VC_ENC_CLK_CTRL_SRC_SEL_1  0x1
#define PRCM_VC_ENC_CLK_CTRL_EN_0       0x0
#define PRCM_VC_ENC_CLK_CTRL_EN_1       0x1

/* VC_DEC Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 src_sel:2;  /**/
	u32 rsvd0:2;    /**/
	u32 div0:4;     /**/
	u32 rsvd1:23;   /**/
	u32 en:1;       /**/
	} bits;
} reg_prcm_vc_dec_clk_ctrl_t;

#define PRCM_VC_DEC_CLK_CTRL_SRC_SEL_0  0x0
#define PRCM_VC_DEC_CLK_CTRL_SRC_SEL_1  0x1
#define PRCM_VC_DEC_CLK_CTRL_EN_0       0x0
#define PRCM_VC_DEC_CLK_CTRL_EN_1       0x1

/* VISS Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 sclk0_en:1; /**/
	u32 rsvd0:7;    /**/
	u32 sclk1_en:1; /**/
	u32 rsvd1:7;    /**/
	u32 sclk2_en:1; /**/
	u32 rsvd2:15;   /**/
	} bits;
} reg_prcm_viss_clk_ctrl_t;

#define PRCM_VISS_CLK_CTRL_SCLK0_EN_0   0x0
#define PRCM_VISS_CLK_CTRL_SCLK0_EN_1   0x1
#define PRCM_VISS_CLK_CTRL_SCLK1_EN_0   0x0
#define PRCM_VISS_CLK_CTRL_SCLK1_EN_1   0x1
#define PRCM_VISS_CLK_CTRL_SCLK2_EN_0   0x0
#define PRCM_VISS_CLK_CTRL_SCLK2_EN_1   0x1

/* DPU SCLK0 Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 src_sel:2;  /**/
	u32 rsvd0:2;    /**/
	u32 div:4;      /**/
	u32 rsvd1:23;   /**/
	u32 en:1;       /**/
	} bits;
} reg_prcm_dpu_sclk0_clk_ctrl_t;

#define PRCM_DPU_SCLK0_CLK_CTRL_SRC_SEL_0       0x0
#define PRCM_DPU_SCLK0_CLK_CTRL_SRC_SEL_1       0x1
#define PRCM_DPU_SCLK0_CLK_CTRL_SRC_SEL_2       0x2
#define PRCM_DPU_SCLK0_CLK_CTRL_EN_0            0x0
#define PRCM_DPU_SCLK0_CLK_CTRL_EN_1            0x1

/* DPU SCLK1 Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 src_sel:2;  /**/
	u32 rsvd0:2;    /**/
	u32 div:4;      /**/
	u32 rsvd1:23;   /**/
	u32 en:1;       /**/
	} bits;
} reg_prcm_dpu_sclk1_clk_ctrl_t;

#define PRCM_DPU_SCLK1_CLK_CTRL_SRC_SEL_0       0x0
#define PRCM_DPU_SCLK1_CLK_CTRL_SRC_SEL_1       0x1
#define PRCM_DPU_SCLK1_CLK_CTRL_SRC_SEL_2       0x2
#define PRCM_DPU_SCLK1_CLK_CTRL_EN_0            0x0
#define PRCM_DPU_SCLK1_CLK_CTRL_EN_1            0x1

/* DPU SCLK2 Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:31;   /**/
	u32 en:1;       /**/
	} bits;
} reg_prcm_dpu_sclk2_clk_ctrl_t;

#define PRCM_DPU_SCLK2_CLK_CTRL_EN_0    0x0
#define PRCM_DPU_SCLK2_CLK_CTRL_EN_1    0x1

/* DOSS Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 hfeosc_en:1;/**/
	u32 rsvd0:7;    /**/
	u32 sclk0_en:1; /**/
	u32 rsvd1:23;   /**/
	} bits;
} reg_prcm_doss_clk_ctrl_t;

#define PRCM_DOSS_CLK_CTRL_HFEOSC_EN_0  0x0
#define PRCM_DOSS_CLK_CTRL_HFEOSC_EN_1  0x1
#define PRCM_DOSS_CLK_CTRL_SCLK0_EN_0   0x0
#define PRCM_DOSS_CLK_CTRL_SCLK0_EN_1   0x1

/* AX Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 src_sel:2;  /**/
	u32 rsvd0:2;    /**/
	u32 div:3;      /**/
	u32 rsvd1:24;   /**/
	u32 en:1;       /**/
	} bits;
} reg_prcm_ax_clk_ctrl_t;

#define PRCM_AX_CLK_CTRL_SRC_SEL_0      0x0
#define PRCM_AX_CLK_CTRL_SRC_SEL_1      0x1
#define PRCM_AX_CLK_CTRL_EN_0           0x0
#define PRCM_AX_CLK_CTRL_EN_1           0x1

/* I2S0 Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 src_sel:2;  /**/
	u32 rsvd0:2;    /**/
	u32 div:3;      /**/
	u32 rsvd1:24;   /**/
	u32 en:1;       /**/
	} bits;
} reg_prcm_i2s0_clk_ctrl_t;

#define PRCM_I2S0_CLK_CTRL_SRC_SEL_0    0x0
#define PRCM_I2S0_CLK_CTRL_SRC_SEL_1    0x1
#define PRCM_I2S0_CLK_CTRL_SRC_SEL_2    0x2
#define PRCM_I2S0_CLK_CTRL_SRC_SEL_3    0x3
#define PRCM_I2S0_CLK_CTRL_EN_0         0x0
#define PRCM_I2S0_CLK_CTRL_EN_1         0x1

/* I2S1 Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 src_sel:2;  /**/
	u32 rsvd0:2;    /**/
	u32 div:3;      /**/
	u32 rsvd1:24;   /**/
	u32 en:1;       /**/
	} bits;
} reg_prcm_i2s1_clk_ctrl_t;

#define PRCM_I2S1_CLK_CTRL_SRC_SEL_0    0x0
#define PRCM_I2S1_CLK_CTRL_SRC_SEL_1    0x1
#define PRCM_I2S1_CLK_CTRL_SRC_SEL_2    0x2
#define PRCM_I2S1_CLK_CTRL_SRC_SEL_3    0x3
#define PRCM_I2S1_CLK_CTRL_EN_0         0x0
#define PRCM_I2S1_CLK_CTRL_EN_1         0x1

/* I2C0 Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 src_sel:2;  /**/
	u32 rsvd0:2;    /**/
	u32 div:3;      /**/
	u32 rsvd1:24;   /**/
	u32 en:1;       /**/
	} bits;
} reg_prcm_i2c0_clk_ctrl_t;

#define PRCM_I2C0_CLK_CTRL_SRC_SEL_0    0x0
#define PRCM_I2C0_CLK_CTRL_SRC_SEL_1    0x1
#define PRCM_I2C0_CLK_CTRL_SRC_SEL_2    0x2
#define PRCM_I2C0_CLK_CTRL_EN_0         0x0
#define PRCM_I2C0_CLK_CTRL_EN_1         0x1

/* I2C1 Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 src_sel:2;  /**/
	u32 rsvd0:2;    /**/
	u32 div:3;      /**/
	u32 rsvd1:24;   /**/
	u32 en:1;       /**/
	} bits;
} reg_prcm_i2c1_clk_ctrl_t;

#define PRCM_I2C1_CLK_CTRL_SRC_SEL_0    0x0
#define PRCM_I2C1_CLK_CTRL_SRC_SEL_1    0x1
#define PRCM_I2C1_CLK_CTRL_SRC_SEL_2    0x2
#define PRCM_I2C1_CLK_CTRL_EN_0         0x0
#define PRCM_I2C1_CLK_CTRL_EN_1         0x1

/* I2C2 Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 src_sel:2;  /**/
	u32 rsvd0:2;    /**/
	u32 div:3;      /**/
	u32 rsvd1:24;   /**/
	u32 en:1;       /**/
	} bits;
} reg_prcm_i2c2_clk_ctrl_t;

#define PRCM_I2C2_CLK_CTRL_SRC_SEL_0    0x0
#define PRCM_I2C2_CLK_CTRL_SRC_SEL_1    0x1
#define PRCM_I2C2_CLK_CTRL_SRC_SEL_2    0x2
#define PRCM_I2C2_CLK_CTRL_EN_0         0x0
#define PRCM_I2C2_CLK_CTRL_EN_1         0x1

/* I2C3 Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 src_sel:2;  /**/
	u32 rsvd0:2;    /**/
	u32 div:3;      /**/
	u32 rsvd1:24;   /**/
	u32 en:1;       /**/
	} bits;
} reg_prcm_i2c3_clk_ctrl_t;

#define PRCM_I2C3_CLK_CTRL_SRC_SEL_0    0x0
#define PRCM_I2C3_CLK_CTRL_SRC_SEL_1    0x1
#define PRCM_I2C3_CLK_CTRL_SRC_SEL_2    0x2
#define PRCM_I2C3_CLK_CTRL_EN_0         0x0
#define PRCM_I2C3_CLK_CTRL_EN_1         0x1

/* UART0 Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 src_sel:2;  /**/
	u32 rsvd0:2;    /**/
	u32 div:3;      /**/
	u32 rsvd1:24;   /**/
	u32 en:1;       /**/
	} bits;
} reg_prcm_uart0_clk_ctrl_t;

#define PRCM_UART0_CLK_CTRL_SRC_SEL_0   0x0
#define PRCM_UART0_CLK_CTRL_SRC_SEL_1   0x1
#define PRCM_UART0_CLK_CTRL_SRC_SEL_2   0x2
#define PRCM_UART0_CLK_CTRL_SRC_SEL_3   0x3
#define PRCM_UART0_CLK_CTRL_EN_0        0x0
#define PRCM_UART0_CLK_CTRL_EN_1        0x1

/* UART1 Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 src_sel:2;  /**/
	u32 rsvd0:2;    /**/
	u32 div:3;      /**/
	u32 rsvd1:24;   /**/
	u32 en:1;       /**/
	} bits;
} reg_prcm_uart1_clk_ctrl_t;

#define PRCM_UART1_CLK_CTRL_SRC_SEL_0   0x0
#define PRCM_UART1_CLK_CTRL_SRC_SEL_1   0x1
#define PRCM_UART1_CLK_CTRL_SRC_SEL_2   0x2
#define PRCM_UART1_CLK_CTRL_EN_0        0x0
#define PRCM_UART1_CLK_CTRL_EN_1        0x1

/* UART2 Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 src_sel:2;  /**/
	u32 rsvd0:2;    /**/
	u32 div:3;      /**/
	u32 rsvd1:24;   /**/
	u32 en:1;       /**/
	} bits;
} reg_prcm_uart2_clk_ctrl_t;

#define PRCM_UART2_CLK_CTRL_SRC_SEL_0   0x0
#define PRCM_UART2_CLK_CTRL_SRC_SEL_1   0x1
#define PRCM_UART2_CLK_CTRL_SRC_SEL_2   0x2
#define PRCM_UART2_CLK_CTRL_EN_0        0x0
#define PRCM_UART2_CLK_CTRL_EN_1        0x1

/* UART3 Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 src_sel:2;  /**/
	u32 rsvd0:2;    /**/
	u32 div:3;      /**/
	u32 rsvd1:24;   /**/
	u32 en:1;       /**/
	} bits;
} reg_prcm_uart3_clk_ctrl_t;

#define PRCM_UART3_CLK_CTRL_SRC_SEL_0   0x0
#define PRCM_UART3_CLK_CTRL_SRC_SEL_1   0x1
#define PRCM_UART3_CLK_CTRL_SRC_SEL_2   0x2
#define PRCM_UART3_CLK_CTRL_EN_0        0x0
#define PRCM_UART3_CLK_CTRL_EN_1        0x1

/* USB PHY Reset Control Register */
typedef union {
	u32 val;
	struct {
	u32 rst:1;      /**/
	u32 rsvd0:31;   /**/
	} bits;
} reg_prcm_usb_phy_rst_t;

#define PRCM_USB_PHY_RST_RST_0  0x0
#define PRCM_USB_PHY_RST_RST_1  0x1

/* GPADC Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 src_sel:2;  /**/
	u32 rsvd0:2;    /**/
	u32 div:1;      /**/
	u32 rsvd1:26;   /**/
	u32 en:1;       /**/
	} bits;
} reg_prcm_gpadc_clk_ctrl_t;

#define PRCM_GPADC_CLK_CTRL_SRC_SEL_0   0x0
#define PRCM_GPADC_CLK_CTRL_SRC_SEL_1   0x1
#define PRCM_GPADC_CLK_CTRL_SRC_SEL_2   0x2
#define PRCM_GPADC_CLK_CTRL_DIV_0       0x0
#define PRCM_GPADC_CLK_CTRL_DIV_1       0x1
#define PRCM_GPADC_CLK_CTRL_EN_0        0x0
#define PRCM_GPADC_CLK_CTRL_EN_1        0x1

/* IR Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:31;   /**/
	u32 en:1;       /**/
	} bits;
} reg_prcm_ir_clk_ctrl_t;

#define PRCM_IR_CLK_CTRL_EN_0   0x0
#define PRCM_IR_CLK_CTRL_EN_1   0x1

/* GMAC Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 core_src_sel:2;     /**/
	u32 rsvd0:2;            /**/
	u32 core_div:3;         /**/
	u32 core_en:1;          /**/
	u32 sclk_src_sel:2;     /**/
	u32 sclk1_en:1;         /**/
	u32 sclk0_en:1;         /**/
	u32 rsvd1:20;           /**/
	} bits;
} reg_prcm_gmac_clk_ctrl_t;

#define PRCM_GMAC_CLK_CTRL_CORE_SRC_SEL_0       0x0
#define PRCM_GMAC_CLK_CTRL_CORE_SRC_SEL_1       0x1
#define PRCM_GMAC_CLK_CTRL_CORE_EN_0            0x0
#define PRCM_GMAC_CLK_CTRL_CORE_EN_1            0x1
#define PRCM_GMAC_CLK_CTRL_SCLK_SRC_SEL_0       0x0
#define PRCM_GMAC_CLK_CTRL_SCLK_SRC_SEL_1       0x1
#define PRCM_GMAC_CLK_CTRL_SCLK1_EN_0           0x0
#define PRCM_GMAC_CLK_CTRL_SCLK1_EN_1           0x1
#define PRCM_GMAC_CLK_CTRL_SCLK0_EN_0           0x0
#define PRCM_GMAC_CLK_CTRL_SCLK0_EN_1           0x1

/* Memory Bist Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 src_sel:2;  /**/
	u32 rsvd0:2;    /**/
	u32 div:4;      /**/
	u32 rsvd1:23;   /**/
	u32 en:1;       /**/
	} bits;
} reg_prcm_mbist_clk_ctrl_t;

#define PRCM_MBIST_CLK_CTRL_SRC_SEL_0   0x0
#define PRCM_MBIST_CLK_CTRL_SRC_SEL_1   0x1
#define PRCM_MBIST_CLK_CTRL_SRC_SEL_2   0x2
#define PRCM_MBIST_CLK_CTRL_EN_0        0x0
#define PRCM_MBIST_CLK_CTRL_EN_1        0x1

/* AES Clock Control Register */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:31;   /**/
	u32 en:1;       /**/
	} bits;
} reg_prcm_aes_clk_ctrl_t;

#define PRCM_AES_CLK_CTRL_EN_0          0x0
#define PRCM_AES_CLK_CTRL_EN_1          0x1

/* reg_prcm_t bank */
typedef struct tag_prcm {
	reg_prcm_ver_t                 ver;                /* 0000 */
	u32                            res0[127];
	reg_prcm_ahb_gat0_t            ahb_gat0;           /* 0200 */
	reg_prcm_ahb_gat1_t            ahb_gat1;           /* 0204 */
	u32                            res1[6];
	reg_prcm_ahb_rst0_t            ahb_rst0;           /* 0220 */
	reg_prcm_ahb_rst1_t            ahb_rst1;           /* 0224 */
	u32                            res2[22];
	reg_prcm_apb_gat0_t            apb_gat0;           /* 0280 */
	u32                            res3[7];
	reg_prcm_apb_rst0_t            apb_rst0;           /* 02A0 */
	u32                            res4[7];
	reg_prcm_apb_gat1_t            apb_gat1;           /* 02C0 */
	u32                            res5[7];
	reg_prcm_apb_rst1_t            apb_rst1;           /* 02E0 */
	u32                            res6[23];
	reg_prcm_axi_gat_t             axi_gat;            /* 0340 */
	u32                            res7[55];
	reg_prcm_hfeosc_ctrl_t         hfeosc_ctrl;        /* 0420 */
	reg_prcm_mbias_ctrl_t          mbias_ctrl;         /* 0424 */
	u32                            res8[14];
	reg_prcm_cpu_pll_en_t          cpu_pll_en;         /* 0460 */
	u32                            res9[1];
	reg_prcm_cpu_pll_fac_t         cpu_pll_fac;        /* 0468 */
	reg_prcm_cpu_pll_tune0_t       cpu_pll_tune0;      /* 046C */
	u32                            res10[1];
	reg_prcm_cpu_pll_ten_t         cpu_pll_ten;        /* 0474 */
	reg_prcm_cpu_pll_stat_t        cpu_pll_stat;       /* 0478 */
	reg_prcm_cpu_pll_mod_t         cpu_pll_mod;        /* 047C */
	reg_prcm_cpu_pll_nfrac_t       cpu_pll_nfrac;      /* 0480 */
	u32                            res11[2];
	reg_prcm_cpu_pll_tune2_t       cpu_pll_tune2;      /* 048C */
	u32                            res12[4];
	reg_prcm_sdram_pll_en_t        sdram_pll_en;       /* 04A0 */
	reg_prcm_sdram_pll_fac_t       sdram_pll_fac;      /* 04A4 */
	reg_prcm_sdram_pll_tune0_t     sdram_pll_tune0;    /* 04A8 */
	reg_prcm_sdram_pll_ten_t       sdram_pll_ten;      /* 04AC */
	reg_prcm_sdram_pll_stat_t      sdram_pll_stat;     /* 04B0 */
	reg_prcm_sdram_pll_mod_t       sdram_pll_mod;      /* 04B4 */
	reg_prcm_sdram_pll_nfrac_t     sdram_pll_nfrac;    /* 04B8 */
	reg_prcm_sdram_pll_tune1_t     sdram_pll_tune1;    /* 04BC */
	reg_prcm_sdram_pll_tune2_t     sdram_pll_tune2;    /* 04C0 */
	u32                            res13[7];
	reg_prcm_perh0_pll_en_t        perh0_pll_en;       /* 04E0 */
	reg_prcm_perh0_pll_fac_t       perh0_pll_fac;      /* 04E4 */
	reg_prcm_perh0_pll_tune0_t     perh0_pll_tune0;    /* 04E8 */
	reg_prcm_perh0_pll_ten_t       perh0_pll_ten;      /* 04EC */
	reg_prcm_perh0_pll_stat_t      perh0_pll_stat;     /* 04F0 */
	reg_prcm_perh0_pll_mod_t       perh0_pll_mod;      /* 04F4 */
	reg_prcm_perh0_pll_nfrac_t     perh0_pll_nfrac;    /* 04F8 */
	u32                            res14[9];
	reg_prcm_audio_pll_en_t        audio_pll_en;       /* 0520 */
	reg_prcm_audio_pll_fac_t       audio_pll_fac;      /* 0524 */
	reg_prcm_audio_pll_tune0_t     audio_pll_tune0;    /* 0528 */
	reg_prcm_audio_pll_ten_t       audio_pll_ten;      /* 052C */
	reg_prcm_audio_pll_stat_t      audio_pll_stat;     /* 0530 */
	reg_prcm_audio_pll_mod_t       audio_pll_mod;      /* 0534 */
	reg_prcm_audio_pll_nfrac_t     audio_pll_nfrac;    /* 0538 */
	u32                            res15[25];
	reg_prcm_vc_pll_en_t           vc_pll_en;          /* 05A0 */
	reg_prcm_vc_pll_fac_t          vc_pll_fac;         /* 05A4 */
	reg_prcm_vc_pll_tune0_t        vc_pll_tune0;       /* 05A8 */
	reg_prcm_vc_pll_ten_t          vc_pll_ten;         /* 05AC */
	reg_prcm_vc_pll_stat_t         vc_pll_stat;        /* 05B0 */
	reg_prcm_vc_pll_mod_t          vc_pll_mod;         /* 05B4 */
	reg_prcm_vc_pll_nfrac_t        vc_pll_nfrac;       /* 05B8 */
	u32                            res16[9];
	reg_prcm_perh1_pll_en_t        perh1_pll_en;       /* 05E0 */
	reg_prcm_perh1_pll_fac_t       perh1_pll_fac;      /* 05E4 */
	reg_prcm_perh1_pll_tune0_t     perh1_pll_tune0;    /* 05E8 */
	reg_prcm_perh1_pll_ten_t       perh1_pll_ten;      /* 05EC */
	reg_prcm_perh1_pll_stat_t      perh1_pll_stat;     /* 05F0 */
	reg_prcm_perh1_pll_mod_t       perh1_pll_mod;      /* 05F4 */
	reg_prcm_perh1_pll_nfrac_t     perh1_pll_nfrac;    /* 05F8 */
	reg_prcm_perh1_pll_tune1_t     perh1_pll_tune1;    /* 05FC */
	u32                            res17[8];
	reg_prcm_disp_pll_en_t         disp_pll_en;        /* 0620 */
	reg_prcm_disp_pll_fac_t        disp_pll_fac;       /* 0624 */
	reg_prcm_disp_pll_tune0_t      disp_pll_tune0;     /* 0628 */
	reg_prcm_disp_pll_ten_t        disp_pll_ten;       /* 062C */
	reg_prcm_disp_pll_stat_t       disp_pll_stat;      /* 0630 */
	reg_prcm_disp_pll_mod_t        disp_pll_mod;       /* 0634 */
	reg_prcm_disp_pll_nfrac_t      disp_pll_nfrac;     /* 0638 */
	reg_prcm_disp_pll_tune1_t      disp_pll_tune1;     /* 063C */
	u32                            res18[24];
	reg_prcm_ax_pll_en_t           ax_pll_en;          /* 06A0 */
	u32                            res19[1];
	reg_prcm_ax_pll_fac_t          ax_pll_fac;         /* 06A8 */
	reg_prcm_ax_pll_tune0_t        ax_pll_tune0;       /* 06AC */
	u32                            res20[1];
	reg_prcm_ax_pll_ten_t          ax_pll_ten;         /* 06B4 */
	reg_prcm_ax_pll_stat_t         ax_pll_stat;        /* 06B8 */
	reg_prcm_ax_pll_mod_t          ax_pll_mod;         /* 06BC */
	reg_prcm_ax_pll_nfrac_t        ax_pll_nfrac;       /* 06C0 */
	u32                            res21[2];
	reg_prcm_ax_pll_tune2_t        ax_pll_tune2;       /* 06CC */
	u32                            res22[4];
	reg_prcm_perh2_pll_en_t        perh2_pll_en;       /* 06E0 */
	reg_prcm_perh2_pll_fac_t       perh2_pll_fac;      /* 06E4 */
	reg_prcm_perh2_pll_tune0_t     perh2_pll_tune0;    /* 06E8 */
	reg_prcm_perh2_pll_ten_t       perh2_pll_ten;      /* 06EC */
	reg_prcm_perh2_pll_stat_t      perh2_pll_stat;     /* 06F0 */
	reg_prcm_perh2_pll_mod_t       perh2_pll_mod;      /* 06F4 */
	reg_prcm_perh2_pll_nfrac_t     perh2_pll_nfrac;    /* 06F8 */
	reg_prcm_perh2_pll_tune1_t     perh2_pll_tune1;    /* 06FC */
	u32                            res23[88];
	reg_prcm_pll_tblk_t            pll_tblk;           /* 0860 */
	u32                            res24[39];
	reg_prcm_cpu_axi_clk_ctrl_t    cpu_axi_clk_ctrl;   /* 0900 */
	reg_prcm_ahb_apb_clk_ctrl_t    ahb_apb_clk_ctrl;   /* 0904 */
	u32                            res25[6];
	reg_prcm_maxi_clk_ctrl_t       maxi_clk_ctrl;      /* 0920 */
	u32                            res26[7];
	reg_prcm_sdram_clk_ctrl_t      sdram_clk_ctrl;     /* 0940 */
	reg_prcm_sdram_rst_t           sdram_rst;          /* 0944 */
	reg_prcm_sdram_bm_clk_ctrl_t   sdram_bm_clk_ctrl;  /* 0948 */
	u32                            res27[5];
	reg_prcm_sdc0_clk_ctrl_t       sdc0_clk_ctrl;      /* 0960 */
	reg_prcm_sdc1_clk_ctrl_t       sdc1_clk_ctrl;      /* 0964 */
	reg_prcm_sdc2_clk_ctrl_t       sdc2_clk_ctrl;      /* 0968 */
	u32                            res28[1];
	reg_prcm_spi0_clk_ctrl_t       spi0_clk_ctrl;      /* 0970 */
	reg_prcm_spi1_clk_ctrl_t       spi1_clk_ctrl;      /* 0974 */
	reg_prcm_spi2_clk_ctrl_t       spi2_clk_ctrl;      /* 0978 */
	u32                            res29[17];
	reg_prcm_vc_enc_clk_ctrl_t     vc_enc_clk_ctrl;    /* 09C0 */
	reg_prcm_vc_dec_clk_ctrl_t     vc_dec_clk_ctrl;    /* 09C4 */
	u32                            res30[2];
	reg_prcm_viss_clk_ctrl_t       viss_clk_ctrl;      /* 09D0 */
	u32                            res31[63];
	reg_prcm_dpu_sclk0_clk_ctrl_t  dpu_sclk0_clk_ctrl; /* 0AD0 */
	reg_prcm_dpu_sclk1_clk_ctrl_t  dpu_sclk1_clk_ctrl; /* 0AD4 */
	reg_prcm_dpu_sclk2_clk_ctrl_t  dpu_sclk2_clk_ctrl; /* 0AD8 */
	u32                            res32[1];
	reg_prcm_doss_clk_ctrl_t       doss_clk_ctrl;      /* 0AE0 */
	u32                            res33[59];
	reg_prcm_ax_clk_ctrl_t         ax_clk_ctrl;        /* 0BD0 */
	u32                            res34[3];
	reg_prcm_i2s0_clk_ctrl_t       i2s0_clk_ctrl;      /* 0BE0 */
	reg_prcm_i2s1_clk_ctrl_t       i2s1_clk_ctrl;      /* 0BE4 */
	u32                            res35[46];
	reg_prcm_i2c0_clk_ctrl_t       i2c0_clk_ctrl;      /* 0CA0 */
	reg_prcm_i2c1_clk_ctrl_t       i2c1_clk_ctrl;      /* 0CA4 */
	reg_prcm_i2c2_clk_ctrl_t       i2c2_clk_ctrl;      /* 0CA8 */
	reg_prcm_i2c3_clk_ctrl_t       i2c3_clk_ctrl;      /* 0CAC */
	u32                            res36[4];
	reg_prcm_uart0_clk_ctrl_t      uart0_clk_ctrl;     /* 0CC0 */
	reg_prcm_uart1_clk_ctrl_t      uart1_clk_ctrl;     /* 0CC4 */
	reg_prcm_uart2_clk_ctrl_t      uart2_clk_ctrl;     /* 0CC8 */
	reg_prcm_uart3_clk_ctrl_t      uart3_clk_ctrl;     /* 0CCC */
	u32                            res37[4];
	reg_prcm_usb_phy_rst_t         usb_phy_rst;        /* 0CE0 */
	u32                            res38[3];
	reg_prcm_gpadc_clk_ctrl_t      gpadc_clk_ctrl;     /* 0CF0 */
	u32                            res39[11];
	reg_prcm_ir_clk_ctrl_t         ir_clk_ctrl;        /* 0D20 */
	u32                            res40[7];
	reg_prcm_gmac_clk_ctrl_t       gmac_clk_ctrl;      /* 0D40 */
	u32                            res41[91];
	reg_prcm_mbist_clk_ctrl_t      mbist_clk_ctrl;     /* 0EB0 */
	u32                            res42[3];
	reg_prcm_aes_clk_ctrl_t        aes_clk_ctrl;       /* 0EC0 */
} reg_prcm_t;

#endif /* ___PRCM___H___ */
