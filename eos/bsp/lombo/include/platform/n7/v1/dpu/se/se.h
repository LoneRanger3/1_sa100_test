/* se.h */

#ifndef ___SE___H___
#define ___SE___H___

#define BASE_SE0            0x00000000
#define BASE_SE1            0x00010000

/*******************modify************************/
#define REG_SE_VER		(0x00000000)
#define REG_SE_FTR		(0x00000004)
#define REG_SE_CSTA		(0x00000008)
#define REG_SE_STA		(0x0000000C)
#define REG_SE_CTL		(0x00000010)
#define REG_SE_CFG0		(0x00000014)
#define REG_SE_CFG1		(0x00000018)
#define REG_SE_CFG2		(0x0000001C)
#define REG_SE_INTCTL		(0x00000020)
#define REG_SE_INTSTA		(0x00000024)
#define REG_SE_INTCLR		(0x00000028)
#define REG_SE_LNCNT		(0x0000002C)
#define REG_SE_UPDCTL		(0x00000030)
#define REG_SE_CTABSWT		(0x00000034)
#define REG_SE_STRMCTL		(0x00000040)
#define REG_SE_INADDR0		(0x00000044)
#define REG_SE_INADDR1		(0x00000048)
#define REG_SE_INADDR2		(0x0000004C)
#define REG_SE_INSIZE0		(0x00000050)
#define REG_SE_INSIZE1		(0x00000054)
#define REG_SE_RSMPSIZE		(0x00000058)
#define REG_SE_INLSTR0		(0x00000060)
#define REG_SE_INLSTR1		(0x00000064)
#define REG_SE_SCRX0		(0x00000070)
#define REG_SE_SCRY0		(0x00000074)
#define REG_SE_SCRX1		(0x00000078)
#define REG_SE_SCRY1		(0x0000007C)
#define REG_SE_CH0XOFST		(0x00000080)
#define REG_SE_CH0YOFST		(0x00000084)
#define REG_SE_CH1XOFST		(0x00000088)
#define REG_SE_CH1YOFST		(0x0000008C)
#define REG_SE_CH2XOFST		(0x00000090)
#define REG_SE_CH2YOFST		(0x00000094)
#define REG_SE_SHIFTCTL		(0x00000098)
#define REG_SE_OCROPOFST	(0x000000A0)
#define REG_SE_OCROPSIZE	(0x000000A4)
#define REG_SE_WBSIZE0		(0x000000A8)
#define REG_SE_WBSIZE1		(0x000000AC)
#define REG_SE_WBCTL		(0x000000C0)
#define REG_SE_WBCFG		(0x000000C4)
#define REG_SE_WBTMR		(0x000000C8)
#define REG_SE_WBADDR0		(0x000000D0)
#define REG_SE_WBADDR1		(0x000000D4)
#define REG_SE_WBADDR2		(0x000000D8)
#define REG_SE_WBLSTR0		(0x000000DC)
#define REG_SE_WBLSTR1		(0x000000E0)
#define REG_SE_CSCI_COEF	(0x00000100)
#define REG_SE_CSCO_COEF	(0x00000130)
#define REG_SE_VPPCTL		(0x00000180)
#define REG_SE_VPPLUTSW		(0x00000184)
#define REG_SE_CHECFG0		(0x00000188)
#define REG_SE_CHECFG1		(0x0000018C)
#define REG_SE_PEAKCFG		(0x000001A0)
#define REG_SE_LTIVCFG		(0x000001A4)
#define REG_SE_PEAKCOEF0	(0x000001A8)
#define REG_SE_PEAKCOEF1	(0x000001AC)
#define REG_SE_LTICFG		(0x000001B0)
#define REG_SE_CTICFG		(0x000001B4)
#define REG_SE_LTICOEF0		(0x000001B8)
#define REG_SE_LTICOEF1		(0x000001BC)
#define REG_SE_CTICOEF0		(0x000001C0)
#define REG_SE_CTICOEF1		(0x000001C4)
#define REG_SE_DMA0CFG		(0x00000300)
#define REG_SE_DMA1CFG		(0x00000304)
#define REG_SE_DMA2CFG		(0x00000308)
#define REG_SE_DBGCTL		(0x00000320)
#define REG_SE_CH0_XCOEF0	(0x00000400)
#define REG_SE_CH0_XCOEF1	(0x00000480)
#define REG_SE_CH0_YCOEF	(0x00000500)
#define REG_SE_CH1_XCOEF0	(0x00000580)
#define REG_SE_CH1_XCOEF1	(0x00000600)
#define REG_SE_CH1_YCOEF	(0x00000680)
#define REG_SE_CH2_XCOEF0	(0x00000700)
#define REG_SE_CH2_XCOEF1	(0x00000780)
#define REG_SE_CH2_YCOEF	(0x00000800)
#define REG_SE_CH0LUT		(0x00000900)
#define REG_SE_CH1LUT		(0x00000A00)
#define REG_SE_CH2LUT		(0x00000B00)
#define REG_SE_CHHIST		(0x00000C00)
/*******************modify************************/
#define VA_SE0_VER                   (0x00000000 + BASE_SE0 + VA_SE)
#define VA_SE0_FTR                   (0x00000004 + BASE_SE0 + VA_SE)
#define VA_SE0_CSTA                  (0x00000008 + BASE_SE0 + VA_SE)
#define VA_SE0_STA                   (0x0000000C + BASE_SE0 + VA_SE)
#define VA_SE0_CTL                   (0x00000010 + BASE_SE0 + VA_SE)
#define VA_SE0_CFG0                  (0x00000014 + BASE_SE0 + VA_SE)
#define VA_SE0_CFG1                  (0x00000018 + BASE_SE0 + VA_SE)
#define VA_SE0_CFG2                  (0x0000001C + BASE_SE0 + VA_SE)
#define VA_SE0_INTCTL                (0x00000020 + BASE_SE0 + VA_SE)
#define VA_SE0_INTSTA                (0x00000024 + BASE_SE0 + VA_SE)
#define VA_SE0_INTCLR                (0x00000028 + BASE_SE0 + VA_SE)
#define VA_SE0_LNCNT                 (0x0000002C + BASE_SE0 + VA_SE)
#define VA_SE0_UPDCTL                (0x00000030 + BASE_SE0 + VA_SE)
#define VA_SE0_CTABSWT               (0x00000034 + BASE_SE0 + VA_SE)
#define VA_SE0_STRMCTL               (0x00000040 + BASE_SE0 + VA_SE)
#define VA_SE0_INADDR0               (0x00000044 + BASE_SE0 + VA_SE)
#define VA_SE0_INADDR1               (0x00000048 + BASE_SE0 + VA_SE)
#define VA_SE0_INADDR2               (0x0000004C + BASE_SE0 + VA_SE)
#define VA_SE0_INSIZE0               (0x00000050 + BASE_SE0 + VA_SE)
#define VA_SE0_INSIZE1               (0x00000054 + BASE_SE0 + VA_SE)
#define VA_SE0_RSMPSIZE              (0x00000058 + BASE_SE0 + VA_SE)
#define VA_SE0_INLSTR0               (0x00000060 + BASE_SE0 + VA_SE)
#define VA_SE0_INLSTR1               (0x00000064 + BASE_SE0 + VA_SE)
#define VA_SE0_SCRX0                 (0x00000070 + BASE_SE0 + VA_SE)
#define VA_SE0_SCRY0                 (0x00000074 + BASE_SE0 + VA_SE)
#define VA_SE0_SCRX1                 (0x00000078 + BASE_SE0 + VA_SE)
#define VA_SE0_SCRY1                 (0x0000007C + BASE_SE0 + VA_SE)
#define VA_SE0_CH0XOFST              (0x00000080 + BASE_SE0 + VA_SE)
#define VA_SE0_CH0YOFST              (0x00000084 + BASE_SE0 + VA_SE)
#define VA_SE0_CH1XOFST              (0x00000088 + BASE_SE0 + VA_SE)
#define VA_SE0_CH1YOFST              (0x0000008C + BASE_SE0 + VA_SE)
#define VA_SE0_SHIFTCTL              (0x00000098 + BASE_SE0 + VA_SE)
#define VA_SE0_OUT_CROPOFST          (0x000000A0 + BASE_SE0 + VA_SE)
#define VA_SE0_OUT_CROPSIZE          (0x000000A4 + BASE_SE0 + VA_SE)
#define VA_SE0_WBSIZE0               (0x000000A8 + BASE_SE0 + VA_SE)
#define VA_SE0_WBSIZE1               (0x000000AC + BASE_SE0 + VA_SE)
#define VA_SE0_WBCTL                 (0x000000C0 + BASE_SE0 + VA_SE)
#define VA_SE0_WBCFG                 (0x000000C4 + BASE_SE0 + VA_SE)
#define VA_SE0_WBTMR                 (0x000000C8 + BASE_SE0 + VA_SE)
#define VA_SE0_WBADDR0               (0x000000D0 + BASE_SE0 + VA_SE)
#define VA_SE0_WBADDR1               (0x000000D4 + BASE_SE0 + VA_SE)
#define VA_SE0_WBADDR2               (0x000000D8 + BASE_SE0 + VA_SE)
#define VA_SE0_WBLSTR0               (0x000000DC + BASE_SE0 + VA_SE)
#define VA_SE0_WBLSTR1               (0x000000E0 + BASE_SE0 + VA_SE)
#define VA_SE0_CSCI_COEF             (0x00000100 + BASE_SE0 + VA_SE)
#define VA_SE0_CSCO_COEF             (0x00000130 + BASE_SE0 + VA_SE)
#define VA_SE0_VPPCTL                (0x00000180 + BASE_SE0 + VA_SE)
#define VA_SE0_VPPLUTSW              (0x00000184 + BASE_SE0 + VA_SE)
#define VA_SE0_CHECFG0               (0x00000188 + BASE_SE0 + VA_SE)
#define VA_SE0_CHECFG1               (0x0000018C + BASE_SE0 + VA_SE)
#define VA_SE0_PEAKCFG               (0x000001A0 + BASE_SE0 + VA_SE)
#define VA_SE0_LTIVCFG               (0x000001A4 + BASE_SE0 + VA_SE)
#define VA_SE0_PEAKCOEF0             (0x000001A8 + BASE_SE0 + VA_SE)
#define VA_SE0_PEAKCOEF1             (0x000001AC + BASE_SE0 + VA_SE)
#define VA_SE0_LTICFG                (0x000001B0 + BASE_SE0 + VA_SE)
#define VA_SE0_CTICFG                (0x000001B4 + BASE_SE0 + VA_SE)
#define VA_SE0_LTICOEF0              (0x000001B8 + BASE_SE0 + VA_SE)
#define VA_SE0_LTICOEF1              (0x000001BC + BASE_SE0 + VA_SE)
#define VA_SE0_CTICOEF0              (0x000001C0 + BASE_SE0 + VA_SE)
#define VA_SE0_CTICOEF1              (0x000001C4 + BASE_SE0 + VA_SE)
#define VA_SE0_CH0_XCOEF0            (0x00000400 + BASE_SE0 + VA_SE)
#define VA_SE0_CH0_XCOEF1            (0x00000480 + BASE_SE0 + VA_SE)
#define VA_SE0_CH0_YCOEF             (0x00000500 + BASE_SE0 + VA_SE)
#define VA_SE0_CH1_XCOEF0            (0x00000580 + BASE_SE0 + VA_SE)
#define VA_SE0_CH1_XCOEF1            (0x00000600 + BASE_SE0 + VA_SE)
#define VA_SE0_CH1_YCOEF             (0x00000680 + BASE_SE0 + VA_SE)
#define VA_SE0_CH0LUT                (0x00000900 + BASE_SE0 + VA_SE)
#define VA_SE0_CH1LUT                (0x00000A00 + BASE_SE0 + VA_SE)
#define VA_SE0_CH2LUT                (0x00000B00 + BASE_SE0 + VA_SE)
#define VA_SE0_CHHIST                (0x00000C00 + BASE_SE0 + VA_SE)
#define VA_SE1_VER                   (0x00000000 + BASE_SE1 + VA_SE)
#define VA_SE1_FTR                   (0x00000004 + BASE_SE1 + VA_SE)
#define VA_SE1_CSTA                  (0x00000008 + BASE_SE1 + VA_SE)
#define VA_SE1_STA                   (0x0000000C + BASE_SE1 + VA_SE)
#define VA_SE1_CTL                   (0x00000010 + BASE_SE1 + VA_SE)
#define VA_SE1_CFG0                  (0x00000014 + BASE_SE1 + VA_SE)
#define VA_SE1_CFG1                  (0x00000018 + BASE_SE1 + VA_SE)
#define VA_SE1_CFG2                  (0x0000001C + BASE_SE1 + VA_SE)
#define VA_SE1_INTCTL                (0x00000020 + BASE_SE1 + VA_SE)
#define VA_SE1_INTSTA                (0x00000024 + BASE_SE1 + VA_SE)
#define VA_SE1_INTCLR                (0x00000028 + BASE_SE1 + VA_SE)
#define VA_SE1_LNCNT                 (0x0000002C + BASE_SE1 + VA_SE)
#define VA_SE1_UPDCTL                (0x00000030 + BASE_SE1 + VA_SE)
#define VA_SE1_CTABSWT               (0x00000034 + BASE_SE1 + VA_SE)
#define VA_SE1_STRMCTL               (0x00000040 + BASE_SE1 + VA_SE)
#define VA_SE1_INADDR0               (0x00000044 + BASE_SE1 + VA_SE)
#define VA_SE1_INADDR1               (0x00000048 + BASE_SE1 + VA_SE)
#define VA_SE1_INADDR2               (0x0000004C + BASE_SE1 + VA_SE)
#define VA_SE1_INSIZE0               (0x00000050 + BASE_SE1 + VA_SE)
#define VA_SE1_INSIZE1               (0x00000054 + BASE_SE1 + VA_SE)
#define VA_SE1_RSMPSIZE              (0x00000058 + BASE_SE1 + VA_SE)
#define VA_SE1_INLSTR0               (0x00000060 + BASE_SE1 + VA_SE)
#define VA_SE1_INLSTR1               (0x00000064 + BASE_SE1 + VA_SE)
#define VA_SE1_SCRX0                 (0x00000070 + BASE_SE1 + VA_SE)
#define VA_SE1_SCRY0                 (0x00000074 + BASE_SE1 + VA_SE)
#define VA_SE1_SCRX1                 (0x00000078 + BASE_SE1 + VA_SE)
#define VA_SE1_SCRY1                 (0x0000007C + BASE_SE1 + VA_SE)
#define VA_SE1_CH0XOFST              (0x00000080 + BASE_SE1 + VA_SE)
#define VA_SE1_CH0YOFST              (0x00000084 + BASE_SE1 + VA_SE)
#define VA_SE1_CH1XOFST              (0x00000088 + BASE_SE1 + VA_SE)
#define VA_SE1_CH1YOFST              (0x0000008C + BASE_SE1 + VA_SE)
#define VA_SE1_SHIFTCTL              (0x00000098 + BASE_SE1 + VA_SE)
#define VA_SE1_OUT_CROPOFST          (0x000000A0 + BASE_SE1 + VA_SE)
#define VA_SE1_OUT_CROPSIZE          (0x000000A4 + BASE_SE1 + VA_SE)
#define VA_SE1_WBSIZE0               (0x000000A8 + BASE_SE1 + VA_SE)
#define VA_SE1_WBSIZE1               (0x000000AC + BASE_SE1 + VA_SE)
#define VA_SE1_WBCTL                 (0x000000C0 + BASE_SE1 + VA_SE)
#define VA_SE1_WBCFG                 (0x000000C4 + BASE_SE1 + VA_SE)
#define VA_SE1_WBTMR                 (0x000000C8 + BASE_SE1 + VA_SE)
#define VA_SE1_WBADDR0               (0x000000D0 + BASE_SE1 + VA_SE)
#define VA_SE1_WBADDR1               (0x000000D4 + BASE_SE1 + VA_SE)
#define VA_SE1_WBADDR2               (0x000000D8 + BASE_SE1 + VA_SE)
#define VA_SE1_WBLSTR0               (0x000000DC + BASE_SE1 + VA_SE)
#define VA_SE1_WBLSTR1               (0x000000E0 + BASE_SE1 + VA_SE)
#define VA_SE1_CSCI_COEF             (0x00000100 + BASE_SE1 + VA_SE)
#define VA_SE1_CSCO_COEF             (0x00000130 + BASE_SE1 + VA_SE)
#define VA_SE1_VPPCTL                (0x00000180 + BASE_SE1 + VA_SE)
#define VA_SE1_VPPLUTSW              (0x00000184 + BASE_SE1 + VA_SE)
#define VA_SE1_CHECFG0               (0x00000188 + BASE_SE1 + VA_SE)
#define VA_SE1_CHECFG1               (0x0000018C + BASE_SE1 + VA_SE)
#define VA_SE1_PEAKCFG               (0x000001A0 + BASE_SE1 + VA_SE)
#define VA_SE1_LTIVCFG               (0x000001A4 + BASE_SE1 + VA_SE)
#define VA_SE1_PEAKCOEF0             (0x000001A8 + BASE_SE1 + VA_SE)
#define VA_SE1_PEAKCOEF1             (0x000001AC + BASE_SE1 + VA_SE)
#define VA_SE1_LTICFG                (0x000001B0 + BASE_SE1 + VA_SE)
#define VA_SE1_CTICFG                (0x000001B4 + BASE_SE1 + VA_SE)
#define VA_SE1_LTICOEF0              (0x000001B8 + BASE_SE1 + VA_SE)
#define VA_SE1_LTICOEF1              (0x000001BC + BASE_SE1 + VA_SE)
#define VA_SE1_CTICOEF0              (0x000001C0 + BASE_SE1 + VA_SE)
#define VA_SE1_CTICOEF1              (0x000001C4 + BASE_SE1 + VA_SE)
#define VA_SE1_CH0_XCOEF0            (0x00000400 + BASE_SE1 + VA_SE)
#define VA_SE1_CH0_XCOEF1            (0x00000480 + BASE_SE1 + VA_SE)
#define VA_SE1_CH0_YCOEF             (0x00000500 + BASE_SE1 + VA_SE)
#define VA_SE1_CH1_XCOEF0            (0x00000580 + BASE_SE1 + VA_SE)
#define VA_SE1_CH1_XCOEF1            (0x00000600 + BASE_SE1 + VA_SE)
#define VA_SE1_CH1_YCOEF             (0x00000680 + BASE_SE1 + VA_SE)
#define VA_SE1_CH0LUT                (0x00000900 + BASE_SE1 + VA_SE)
#define VA_SE1_CH1LUT                (0x00000A00 + BASE_SE1 + VA_SE)
#define VA_SE1_CH2LUT                (0x00000B00 + BASE_SE1 + VA_SE)
#define VA_SE1_CHHIST                (0x00000C00 + BASE_SE1 + VA_SE)

#define DATA_SE0_VER                     0x00001100
#define DATA_SE0_FTR                     0x00000003
#define DATA_SE0_CSTA                    0x00000000
#define DATA_SE0_STA                     0x00000000
#define DATA_SE0_CTL                     0x80000000
#define DATA_SE0_CFG0                    0x00000000
#define DATA_SE0_CFG1                    0x00000000
#define DATA_SE0_CFG2                    0x00000000
#define DATA_SE0_INTCTL                  0x00000000
#define DATA_SE0_INTSTA                  0x00000000
#define DATA_SE0_INTCLR                  0x00000000
#define DATA_SE0_LNCNT                   0x00001FFF
#define DATA_SE0_UPDCTL                  0x00000000
#define DATA_SE0_CTABSWT                 0x00000000
#define DATA_SE0_STRMCTL                 0x00000000
#define DATA_SE0_INADDR0                 0x00000000
#define DATA_SE0_INADDR1                 0x00000000
#define DATA_SE0_INADDR2                 0x00000000
#define DATA_SE0_INSIZE0                 0x00000000
#define DATA_SE0_INSIZE1                 0x00000000
#define DATA_SE0_RSMPSIZE                0x00000000
#define DATA_SE0_INLSTR0                 0x00000000
#define DATA_SE0_INLSTR1                 0x00000000
#define DATA_SE0_SCRX0                   0x00010000
#define DATA_SE0_SCRY0                   0x00010000
#define DATA_SE0_SCRX1                   0x00010000
#define DATA_SE0_SCRY1                   0x00010000
#define DATA_SE0_CH0XOFST                0x00000000
#define DATA_SE0_CH0YOFST                0x00000000
#define DATA_SE0_CH1XOFST                0x00000000
#define DATA_SE0_CH1YOFST                0x00000000
#define DATA_SE0_SHIFTCTL                0x00000000
#define DATA_SE0_OUT_CROPOFST            0x00000000
#define DATA_SE0_OUT_CROPSIZE            0x00000000
#define DATA_SE0_WBSIZE0                 0x00000000
#define DATA_SE0_WBSIZE1                 0x00000000
#define DATA_SE0_WBCTL                   0x00000000
#define DATA_SE0_WBCFG                   0xFFFF0000
#define DATA_SE0_WBTMR                   0x00000000
#define DATA_SE0_WBADDR0                 0x00000000
#define DATA_SE0_WBADDR1                 0x00000000
#define DATA_SE0_WBADDR2                 0x00000000
#define DATA_SE0_WBLSTR0                 0x00000000
#define DATA_SE0_WBLSTR1                 0x00000000
#define DATA_SE0_CSCI_COEF               0x00000000
#define DATA_SE0_CSCO_COEF               0x00000000
#define DATA_SE0_VPPCTL                  0x00000000
#define DATA_SE0_VPPLUTSW                0x00000000
#define DATA_SE0_CHECFG0                 0x00000000
#define DATA_SE0_CHECFG1                 0x00000000
#define DATA_SE0_PEAKCFG                 0x00140A04
#define DATA_SE0_LTIVCFG                 0x08080804
#define DATA_SE0_PEAKCOEF0               0x00000000
#define DATA_SE0_PEAKCOEF1               0x00000000
#define DATA_SE0_LTICFG                  0x08080804
#define DATA_SE0_CTICFG                  0x08080804
#define DATA_SE0_LTICOEF0                0x00000000
#define DATA_SE0_LTICOEF1                0x00000000
#define DATA_SE0_CTICOEF0                0x00000000
#define DATA_SE0_CTICOEF1                0x00000000
#define DATA_SE0_CH0_XCOEF0              0x00000000
#define DATA_SE0_CH0_XCOEF1              0x00000000
#define DATA_SE0_CH0_YCOEF               0x00000000
#define DATA_SE0_CH1_XCOEF0              0x00000000
#define DATA_SE0_CH1_XCOEF1              0x00000000
#define DATA_SE0_CH1_YCOEF               0x00000000
#define DATA_SE0_CH0LUT                  0x00000000
#define DATA_SE0_CH1LUT                  0x00000000
#define DATA_SE0_CH2LUT                  0x00000000
#define DATA_SE0_CHHIST                  0x00000000
#define DATA_SE1_VER                     0x00001100
#define DATA_SE1_FTR                     0x00000003
#define DATA_SE1_CSTA                    0x00000000
#define DATA_SE1_STA                     0x00000000
#define DATA_SE1_CTL                     0x80000000
#define DATA_SE1_CFG0                    0x00000000
#define DATA_SE1_CFG1                    0x00000000
#define DATA_SE1_CFG2                    0x00000000
#define DATA_SE1_INTCTL                  0x00000000
#define DATA_SE1_INTSTA                  0x00000000
#define DATA_SE1_INTCLR                  0x00000000
#define DATA_SE1_LNCNT                   0x00001FFF
#define DATA_SE1_UPDCTL                  0x00000000
#define DATA_SE1_CTABSWT                 0x00000000
#define DATA_SE1_STRMCTL                 0x00000000
#define DATA_SE1_INADDR0                 0x00000000
#define DATA_SE1_INADDR1                 0x00000000
#define DATA_SE1_INADDR2                 0x00000000
#define DATA_SE1_INSIZE0                 0x00000000
#define DATA_SE1_INSIZE1                 0x00000000
#define DATA_SE1_RSMPSIZE                0x00000000
#define DATA_SE1_INLSTR0                 0x00000000
#define DATA_SE1_INLSTR1                 0x00000000
#define DATA_SE1_SCRX0                   0x00010000
#define DATA_SE1_SCRY0                   0x00010000
#define DATA_SE1_SCRX1                   0x00010000
#define DATA_SE1_SCRY1                   0x00010000
#define DATA_SE1_CH0XOFST                0x00000000
#define DATA_SE1_CH0YOFST                0x00000000
#define DATA_SE1_CH1XOFST                0x00000000
#define DATA_SE1_CH1YOFST                0x00000000
#define DATA_SE1_SHIFTCTL                0x00000000
#define DATA_SE1_OUT_CROPOFST            0x00000000
#define DATA_SE1_OUT_CROPSIZE            0x00000000
#define DATA_SE1_WBSIZE0                 0x00000000
#define DATA_SE1_WBSIZE1                 0x00000000
#define DATA_SE1_WBCTL                   0x00000000
#define DATA_SE1_WBCFG                   0xFFFF0000
#define DATA_SE1_WBTMR                   0x00000000
#define DATA_SE1_WBADDR0                 0x00000000
#define DATA_SE1_WBADDR1                 0x00000000
#define DATA_SE1_WBADDR2                 0x00000000
#define DATA_SE1_WBLSTR0                 0x00000000
#define DATA_SE1_WBLSTR1                 0x00000000
#define DATA_SE1_CSCI_COEF               0x00000000
#define DATA_SE1_CSCO_COEF               0x00000000
#define DATA_SE1_VPPCTL                  0x00000000
#define DATA_SE1_VPPLUTSW                0x00000000
#define DATA_SE1_CHECFG0                 0x00000000
#define DATA_SE1_CHECFG1                 0x00000000
#define DATA_SE1_PEAKCFG                 0x00140A04
#define DATA_SE1_LTIVCFG                 0x08080804
#define DATA_SE1_PEAKCOEF0               0x00000000
#define DATA_SE1_PEAKCOEF1               0x00000000
#define DATA_SE1_LTICFG                  0x08080804
#define DATA_SE1_CTICFG                  0x08080804
#define DATA_SE1_LTICOEF0                0x00000000
#define DATA_SE1_LTICOEF1                0x00000000
#define DATA_SE1_CTICOEF0                0x00000000
#define DATA_SE1_CTICOEF1                0x00000000
#define DATA_SE1_CH0_XCOEF0              0x00000000
#define DATA_SE1_CH0_XCOEF1              0x00000000
#define DATA_SE1_CH0_YCOEF               0x00000000
#define DATA_SE1_CH1_XCOEF0              0x00000000
#define DATA_SE1_CH1_XCOEF1              0x00000000
#define DATA_SE1_CH1_YCOEF               0x00000000
#define DATA_SE1_CH0LUT                  0x00000000
#define DATA_SE1_CH1LUT                  0x00000000
#define DATA_SE1_CH2LUT                  0x00000000
#define DATA_SE1_CHHIST                  0x00000000

/* SE Version Register */
typedef union {
	u32 val;
	struct {
	u32 ver_l:8;    /**/
	u32 ver_h:3;    /**/
	u32 rsvd0:1;    /**/
	u32 comp:1;     /**/
	u32 rsvd1:19;   /**/
	} bits;
} reg_se_ver_t;

#define SE_VER_COMP_0   0x0
#define SE_VER_COMP_1   0x1

/* SE Feature Register */
typedef union {
	u32 val;
	struct {
	u32 has_enhance:1;/**/
	u32 has_lut:1;  /**/
	u32 has_dit:1;  /**/
	u32 rsvd0:29;   /**/
	} bits;
} reg_se_ftr_t;

#define SE_FTR_HAS_ENHANCE_0    0x0
#define SE_FTR_HAS_ENHANCE_1    0x1
#define SE_FTR_HAS_LUT_0        0x0
#define SE_FTR_HAS_LUT_1        0x1
#define SE_FTR_HAS_DIT_0        0x0
#define SE_FTR_HAS_DIT_1        0x1

/* SE Counter Status Register */
typedef union {
	u32 val;
	struct {
	u32 ln_num:13;  /**/
	u32 rsvd0:2;    /**/
	u32 field_sta:1;/**/
	u32 frm_cnt:16; /**/
	} bits;
} reg_se_csta_t;

#define SE_CSTA_FIELD_STA_0     0x0
#define SE_CSTA_FIELD_STA_1     0x1

/* SE Status Register */
typedef union {
	u32 val;
	struct {
	u32 rd0sta:1;   /**/
	u32 rd1sta:1;   /**/
	u32 rd2sta:1;   /**/
	u32 rdtotalsta:1;/**/
	u32 wr0sta:1;   /**/
	u32 wr1sta:1;   /**/
	u32 wr2sta:1;   /**/
	u32 wrtotalsta:1;/**/
	u32 rsvd0:23;   /**/
	u32 rsvd1:1;    /**/
	} bits;
} reg_se_sta_t;

#define SE_STA_RD0STA_0         0x0
#define SE_STA_RD0STA_1         0x1
#define SE_STA_RD1STA_0         0x0
#define SE_STA_RD1STA_1         0x1
#define SE_STA_RD2STA_0         0x0
#define SE_STA_RD2STA_1         0x1
#define SE_STA_RDTOTALSTA_0     0x0
#define SE_STA_RDTOTALSTA_1     0x1
#define SE_STA_WR0STA_0         0x0
#define SE_STA_WR0STA_1         0x1
#define SE_STA_WR1STA_0         0x0
#define SE_STA_WR1STA_1         0x1
#define SE_STA_WR2STA_0         0x0
#define SE_STA_WR2STA_1         0x1
#define SE_STA_WRTOTALSTA_0     0x0
#define SE_STA_WRTOTALSTA_1     0x1

/* SE Control Register */
typedef union {
	u32 val;
	struct {
	u32 se_en:1;    /**/
	u32 rsvd0:7;    /**/
	u32 rsvd1:20;   /**/
	u32 se_bypass:1;/**/
	u32 rsvd2:1;    /**/
	u32 dbg_ctl:1;  /**/
	u32 rstn:1;     /**/
	} bits;
} reg_se_ctl_t;

#define SE_CTL_SE_EN_0          0x0
#define SE_CTL_SE_EN_1          0x1
#define SE_CTL_SE_BYPASS_0      0x0
#define SE_CTL_SE_BYPASS_1      0x1
#define SE_CTL_DBG_CTL_0        0x0
#define SE_CTL_DBG_CTL_1        0x1
#define SE_CTL_RSTN_0           0x0
#define SE_CTL_RSTN_1           0x1

/* SE Configuration 0 Register */
typedef union {
	u32 val;
	struct {
	u32 dcout_en:1; /**/
	u32 rsvd0:3;    /**/
	u32 in_sel:1;   /**/
	u32 rsvd1:3;    /**/
	u32 rsvd2:8;    /**/
	u32 out_mode:1; /**/
	u32 rsvd3:15;   /**/
	} bits;
} reg_se_cfg0_t;

#define SE_CFG0_DCOUT_EN_0      0x0
#define SE_CFG0_DCOUT_EN_1      0x1
#define SE_CFG0_IN_SEL_0        0x0
#define SE_CFG0_IN_SEL_1        0x1
#define SE_CFG0_OUT_MODE_0      0x0
#define SE_CFG0_OUT_MODE_1      0x1

/* SE Configuration 1 Register */
typedef union {
	u32 val;
	struct {
	u32 infmt:6;    /**/
	u32 rsvd0:2;    /**/
	u32 rsvd1:24;   /**/
	} bits;
} reg_se_cfg1_t;

#define SE_CFG1_INFMT_0         0x0
#define SE_CFG1_INFMT_1         0x1
#define SE_CFG1_INFMT_2         0x2
#define SE_CFG1_INFMT_3         0x3
#define SE_CFG1_INFMT_4         0x4
#define SE_CFG1_INFMT_5         0x5
#define SE_CFG1_INFMT_6         0x6
#define SE_CFG1_INFMT_7         0x7
#define SE_CFG1_INFMT_8         0x8
#define SE_CFG1_INFMT_9         0x9
#define SE_CFG1_INFMT_A         0xA
#define SE_CFG1_INFMT_B         0xB
#define SE_CFG1_INFMT_10        0x10
#define SE_CFG1_INFMT_11        0x11
#define SE_CFG1_INFMT_12        0x12
#define SE_CFG1_INFMT_13        0x13
#define SE_CFG1_INFMT_20        0x20
#define SE_CFG1_INFMT_21        0x21
#define SE_CFG1_INFMT_22        0x22

/* SE Configuration 2 Register */
typedef union {
	u32 val;
	struct {
	u32 csci_en:1;  /**/
	u32 csco_en:1;  /**/
	u32 alpha_en:1; /**/
	u32 premul_en:1;/**/
	u32 rsmp_en:1;  /**/
	u32 rsvd0:3;    /**/
	u32 lb_mode:2;  /**/
	u32 rsvd1:6;    /**/
	u32 field_pol:1;/**/
	u32 wb_field_lv:1;/**/
	u32 rsvd2:14;   /**/
	} bits;
} reg_se_cfg2_t;

#define SE_CFG2_CSCI_EN_0       0x0
#define SE_CFG2_CSCI_EN_1       0x1
#define SE_CFG2_CSCO_EN_0       0x0
#define SE_CFG2_CSCO_EN_1       0x1
#define SE_CFG2_ALPHA_EN_0      0x0
#define SE_CFG2_ALPHA_EN_1      0x1
#define SE_CFG2_PREMUL_EN_0     0x0
#define SE_CFG2_PREMUL_EN_1     0x1
#define SE_CFG2_RSMP_EN_0       0x0
#define SE_CFG2_RSMP_EN_1       0x1
#define SE_CFG2_LB_MODE_0       0x0
#define SE_CFG2_LB_MODE_1       0x1
#define SE_CFG2_LB_MODE_2       0x2
#define SE_CFG2_LB_MODE_3       0x3
#define SE_CFG2_FIELD_POL_0     0x0
#define SE_CFG2_FIELD_POL_1     0x1
#define SE_CFG2_WB_FIELD_LV_0   0x0
#define SE_CFG2_WB_FIELD_LV_1   0x1

/* SE Interrupt Control Register */
typedef union {
	u32 val;
	struct {
	u32 lntrig_en:1;/**/
	u32 rsvd0:3;    /**/
	u32 wbfin_en:1; /**/
	u32 wbovfl_en:1;/**/
	u32 wbtmout_en:1;/**/
	u32 rdfin_en:1; /**/
	u32 rsvd1:24;   /**/
	} bits;
} reg_se_intctl_t;

#define SE_INTCTL_LNTRIG_EN_0   0x0
#define SE_INTCTL_LNTRIG_EN_1   0x1
#define SE_INTCTL_WBFIN_EN_0    0x0
#define SE_INTCTL_WBFIN_EN_1    0x1
#define SE_INTCTL_WBOVFL_EN_0   0x0
#define SE_INTCTL_WBOVFL_EN_1   0x1
#define SE_INTCTL_WBTMOUT_EN_0  0x0
#define SE_INTCTL_WBTMOUT_EN_1  0x1
#define SE_INTCTL_RDFIN_EN_0    0x0
#define SE_INTCTL_RDFIN_EN_1    0x1

/* SE Interrupt Status Register */
typedef union {
	u32 val;
	struct {
	u32 lntrig:1;   /**/
	u32 rsvd0:3;    /**/
	u32 wbfin:1;    /**/
	u32 wbovfl:1;   /**/
	u32 wbtmout:1;  /**/
	u32 rsvd1:1;    /**/
	u32 rdfin:1;    /**/
	u32 rsvd2:23;   /**/
	} bits;
} reg_se_intsta_t;

#define SE_INTSTA_WBFIN_0       0x0
#define SE_INTSTA_WBFIN_1       0x1
#define SE_INTSTA_WBOVFL_0      0x0
#define SE_INTSTA_WBOVFL_1      0x1
#define SE_INTSTA_WBTMOUT_0     0x0
#define SE_INTSTA_WBTMOUT_1     0x1
#define SE_INTSTA_RDFIN_0       0x0
#define SE_INTSTA_RDFIN_1       0x1

/* SE Interrupt Status Clear Register */
typedef union {
	u32 val;
	struct {
	u32 lntrigclr:1;/**/
	u32 rsvd0:3;    /**/
	u32 wbfinclr:1; /**/
	u32 wbovflclr:1;/**/
	u32 wbtmoutclr:1;/**/
	u32 rdfinclr:1; /**/
	u32 rsvd1:24;   /**/
	} bits;
} reg_se_intclr_t;

/* SE Line Counter Register */
typedef union {
	u32 val;
	struct {
	u32 trig_num:13;/**/
	u32 trig_field:2;/**/
	u32 rsvd0:17;   /**/
	} bits;
} reg_se_lncnt_t;

#define SE_LNCNT_TRIG_FIELD_0   0x0
#define SE_LNCNT_TRIG_FIELD_1   0x1
#define SE_LNCNT_TRIG_FIELD_2   0x2
#define SE_LNCNT_TRIG_FIELD_3   0x3

/* SE Update Control Register */
typedef union {
	u32 val;
	struct {
	u32 upd_im:1;   /**/
	u32 sync_mode:1;/**/
	u32 rsvd0:30;   /**/
	} bits;
} reg_se_updctl_t;

#define SE_UPDCTL_UPD_IM_0      0x0
#define SE_UPDCTL_UPD_IM_1      0x1
#define SE_UPDCTL_SYNC_MODE_0   0x0
#define SE_UPDCTL_SYNC_MODE_1   0x1

/* SE Coefficient Table Switch Register */
typedef union {
	u32 val;
	struct {
	u32 tab_swt:1;  /**/
	u32 rsvd0:15;   /**/
	u32 clk_swt:1;  /**/
	u32 rsvd1:15;   /**/
	} bits;
} reg_se_ctabswt_t;

#define SE_CTABSWT_TAB_SWT_0    0x0
#define SE_CTABSWT_TAB_SWT_1    0x1

/* SE Streaming Control Register */
typedef union {
	u32 val;
	struct {
	u32 strm_start:1;/**/
	u32 rsvd0:31;   /**/
	} bits;
} reg_se_strmctl_t;

#define SE_STRMCTL_STRM_START_0 0x0
#define SE_STRMCTL_STRM_START_1 0x1

/* SE Input Address 0 Register */
typedef union {
	u32 val;
	struct {
	u32 addr:32;    /**/
	} bits;
} reg_se_inaddr0_t;

/* SE Input Address 1 Register */
typedef union {
	u32 val;
	struct {
	u32 addr:32;    /**/
	} bits;
} reg_se_inaddr1_t;

/* SE Input Address 2 Register */
typedef union {
	u32 val;
	struct {
	u32 addr:32;    /**/
	} bits;
} reg_se_inaddr2_t;

/* SE Input Size 0 Register */
typedef union {
	u32 val;
	struct {
	u32 w:13;       /**/
	u32 rsvd0:3;    /**/
	u32 h:13;       /**/
	u32 rsvd1:3;    /**/
	} bits;
} reg_se_insize0_t;

/* SE Input Size 1 Register */
typedef union {
	u32 val;
	struct {
	u32 w:13;       /**/
	u32 rsvd0:3;    /**/
	u32 h:13;       /**/
	u32 rsvd1:3;    /**/
	} bits;
} reg_se_insize1_t;

/* SE Resampling Size Register */
typedef union {
	u32 val;
	struct {
	u32 w:13;       /**/
	u32 rsvd0:3;    /**/
	u32 h:13;       /**/
	u32 rsvd1:3;    /**/
	} bits;
} reg_se_rsmpsize_t;

/* SE Input Line Stride 0 Register */
typedef union {
	u32 val;
	struct {
	u32 lstr:18;    /**/
	u32 rsvd0:14;   /**/
	} bits;
} reg_se_inlstr0_t;

/* SE input Line Stride 1 Register */
typedef union {
	u32 val;
	struct {
	u32 lstr:18;    /**/
	u32 rsvd0:14;   /**/
	} bits;
} reg_se_inlstr1_t;

/* SE Scale Xratio 0 Register */
typedef union {
	u32 val;
	struct {
	u32 scr:21;     /**/
	u32 rsvd0:11;   /**/
	} bits;
} reg_se_scrx0_t;

/* SE Scale Yratio 0 Register */
typedef union {
	u32 val;
	struct {
	u32 scr:21;     /**/
	u32 rsvd0:11;   /**/
	} bits;
} reg_se_scry0_t;

/* SE Scale Xratio 1 Register */
typedef union {
	u32 val;
	struct {
	u32 scr:21;     /**/
	u32 rsvd0:11;   /**/
	} bits;
} reg_se_scrx1_t;

/* SE Scale Yratio 1 Register */
typedef union {
	u32 val;
	struct {
	u32 scr:21;     /**/
	u32 rsvd0:11;   /**/
	} bits;
} reg_se_scry1_t;

/* SE Ch0 Xoffset Register */
typedef union {
	u32 val;
	struct {
	u32 ofst:20;    /**/
	u32 rsvd0:12;   /**/
	} bits;
} reg_se_ch0xofst_t;

/* SE Ch0 Yoffset Register */
typedef union {
	u32 val;
	struct {
	u32 ofst:20;    /**/
	u32 rsvd0:12;   /**/
	} bits;
} reg_se_ch0yofst_t;

/* SE Ch1 Xoffset Register */
typedef union {
	u32 val;
	struct {
	u32 ofst:20;    /**/
	u32 rsvd0:12;   /**/
	} bits;
} reg_se_ch1xofst_t;

/* SE Ch1 Yoffset Register */
typedef union {
	u32 val;
	struct {
	u32 ofst:20;    /**/
	u32 rsvd0:12;   /**/
	} bits;
} reg_se_ch1yofst_t;

/* SE Shift Control Register */
typedef union {
	u32 val;
	struct {
	u32 ctlx0:2;    /**/
	u32 ctly0:2;    /**/
	u32 ctlx1:2;    /**/
	u32 ctly1:2;    /**/
	u32 rsvd0:24;   /**/
	} bits;
} reg_se_shiftctl_t;

/* SE Output Crop Offset Register */
typedef union {
	u32 val;
	struct {
	u32 x:13;       /**/
	u32 rsvd0:3;    /**/
	u32 y:13;       /**/
	u32 rsvd1:3;    /**/
	} bits;
} reg_se_out_cropofst_t;

/* SE Output Crop Size Register */
typedef union {
	u32 val;
	struct {
	u32 w:13;       /**/
	u32 rsvd0:3;    /**/
	u32 h:13;       /**/
	u32 rsvd1:3;    /**/
	} bits;
} reg_se_out_cropsize_t;

/* SE Write-back Size0 Register */
typedef union {
	u32 val;
	struct {
	u32 w:13;       /**/
	u32 rsvd0:3;    /**/
	u32 h:13;       /**/
	u32 rsvd1:3;    /**/
	} bits;
} reg_se_wbsize0_t;

/* SE Write-back Size1 Register */
typedef union {
	u32 val;
	struct {
	u32 w:13;       /**/
	u32 rsvd0:3;    /**/
	u32 h:13;       /**/
	u32 rsvd1:3;    /**/
	} bits;
} reg_se_wbsize1_t;

/* SE Write-Back Control Register */
typedef union {
	u32 val;
	struct {
	u32 wb_start:1; /**/
	u32 rsvd0:31;   /**/
	} bits;
} reg_se_wbctl_t;

#define SE_WBCTL_WB_START_0     0x0
#define SE_WBCTL_WB_START_1     0x1

/* SE Write-Back Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 wbfmt:3;    /**/
	u32 rsvd0:13;   /**/
	u32 wbthr:16;   /**/
	} bits;
} reg_se_wbcfg_t;

#define SE_WBCFG_WBFMT_0        0x0
#define SE_WBCFG_WBFMT_1        0x1
#define SE_WBCFG_WBFMT_2        0x2
#define SE_WBCFG_WBFMT_3        0x3
#define SE_WBCFG_WBFMT_4        0x4

/* SE Write-Back Timer Register */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:16;   /**/
	u32 wbtmr:16;   /**/
	} bits;
} reg_se_wbtmr_t;

/* SE Write-Back Address 0 Register */
typedef union {
	u32 val;
	struct {
	u32 addr:32;    /**/
	} bits;
} reg_se_wbaddr0_t;

/* SE Write-Back Address 1 Register */
typedef union {
	u32 val;
	struct {
	u32 addr:32;    /**/
	} bits;
} reg_se_wbaddr1_t;

/* SE Write-Back Address 2 Register */
typedef union {
	u32 val;
	struct {
	u32 addr:32;    /**/
	} bits;
} reg_se_wbaddr2_t;

/* SE Write-Back Line Stride 0 Register */
typedef union {
	u32 val;
	struct {
	u32 lstr:15;    /**/
	u32 rsvd0:17;   /**/
	} bits;
} reg_se_wblstr0_t;

/* SE Write-Back Line Stride 1 Register */
typedef union {
	u32 val;
	struct {
	u32 lstr:15;    /**/
	u32 rsvd0:17;   /**/
	} bits;
} reg_se_wblstr1_t;

/* SE CSCI Coefficient Register */
typedef union {
	u32 val;
	struct {
	u32 coef:15;    /**/
	u32 rsvd0:17;   /**/
	} bits;
} reg_se_csci_coef_t;

/* SE CSCO Coefficient Register */
typedef union {
	u32 val;
	struct {
	u32 coef:15;    /**/
	u32 rsvd0:17;   /**/
	} bits;
} reg_se_csco_coef_t;

/* SE Video Post Processor Control Register */
typedef union {
	u32 val;
	struct {
	u32 lut0en:1;   /**/
	u32 lut1en:1;   /**/
	u32 lut2en:1;   /**/
	u32 lut_mod:1;  /**/
	u32 e0en:1;     /**/
	u32 e1en:1;     /**/
	u32 e2en:1;     /**/
	u32 e3en:1;     /**/
	u32 hist_sel:2; /**/
	u32 rsvd0:6;    /**/
	u32 hist_ok:1;  /**/
	u32 hist_clr:1; /**/
	u32 rsvd1:13;   /**/
	u32 hist_en:1;  /**/
	} bits;
} reg_se_vppctl_t;

#define SE_VPPCTL_LUT0EN_0      0x0
#define SE_VPPCTL_LUT0EN_1      0x1
#define SE_VPPCTL_LUT1EN_0      0x0
#define SE_VPPCTL_LUT1EN_1      0x1
#define SE_VPPCTL_LUT2EN_0      0x0
#define SE_VPPCTL_LUT2EN_1      0x1
#define SE_VPPCTL_LUT_MOD_0     0x0
#define SE_VPPCTL_LUT_MOD_1     0x1
#define SE_VPPCTL_HIST_SEL_0    0x0
#define SE_VPPCTL_HIST_SEL_1    0x1
#define SE_VPPCTL_HIST_SEL_2    0x2
#define SE_VPPCTL_HIST_SEL_3    0x3
#define SE_VPPCTL_HIST_CLR_0    0x0
#define SE_VPPCTL_HIST_CLR_1    0x1
#define SE_VPPCTL_HIST_EN_0     0x0
#define SE_VPPCTL_HIST_EN_1     0x1

/* SE Video Post Processor LUT Switch Register */
typedef union {
	u32 val;
	struct {
	u32 lut0sw:1;   /**/
	u32 lut1sw:1;   /**/
	u32 lut2sw:1;   /**/
	u32 histsw:1;   /**/
	u32 rsvd0:12;   /**/
	u32 clk_lut:1;  /**/
	u32 clk_hist:1; /**/
	u32 rsvd1:14;   /**/
	} bits;
} reg_se_vpplutsw_t;

#define SE_VPPLUTSW_LUT0SW_0    0x0
#define SE_VPPLUTSW_LUT0SW_1    0x1
#define SE_VPPLUTSW_LUT1SW_0    0x0
#define SE_VPPLUTSW_LUT1SW_1    0x1
#define SE_VPPLUTSW_LUT2SW_0    0x0
#define SE_VPPLUTSW_LUT2SW_1    0x1
#define SE_VPPLUTSW_HISTSW_0    0x0
#define SE_VPPLUTSW_HISTSW_1    0x1

/* SE Chroma Enhance Configuration 0 Register */
typedef union {
	u32 val;
	struct {
	u32 cthr0:8;    /**/
	u32 cthr1:8;    /**/
	u32 cthr2:8;    /**/
	u32 cthr3:8;    /**/
	} bits;
} reg_se_checfg0_t;

/* SE Chroma Enhance Configuration 1 Register */
typedef union {
	u32 val;
	struct {
	u32 spen:1;     /**/
	u32 rsvd0:7;    /**/
	u32 rsvd1:8;    /**/
	u32 cord0:8;    /**/
	u32 cord1:8;    /**/
	} bits;
} reg_se_checfg1_t;

#define SE_CHECFG1_SPEN_0       0x0
#define SE_CHECFG1_SPEN_1       0x1

/* SE Peak Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 cor:4;      /**/
	u32 rsvd0:4;    /**/
	u32 gain:6;     /**/
	u32 rsvd1:2;    /**/
	u32 limit:6;    /**/
	u32 rsvd2:10;   /**/
	} bits;
} reg_se_peakcfg_t;

/* SE LTIV Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 cor:4;      /**/
	u32 rsvd0:4;    /**/
	u32 gain:6;     /**/
	u32 rsvd1:2;    /**/
	u32 gain2:6;    /**/
	u32 rsvd2:2;    /**/
	u32 oslmt:6;    /**/
	u32 rsvd3:2;    /**/
	} bits;
} reg_se_ltivcfg_t;

/* SE Peak Coefficient 0 Register */
typedef union {
	u32 val;
	struct {
	u32 c0:8;/**/
	u32 c1:8;/**/
	u32 c2:8;/**/
	u32 c3:8;/**/
	} bits;
} reg_se_peakcoef0_t;

/* SE Peak Coefficient 1 Register */
typedef union {
	u32 val;
	struct {
	u32 c4:8;       /**/
	u32 rsvd0:24;   /**/
	} bits;
} reg_se_peakcoef1_t;

/* SE LTI Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 cor:4;      /**/
	u32 rsvd0:4;    /**/
	u32 gain:6;     /**/
	u32 rsvd1:2;    /**/
	u32 gain2:6;    /**/
	u32 rsvd2:2;    /**/
	u32 oslmt:6;    /**/
	u32 rsvd3:2;    /**/
	} bits;
} reg_se_lticfg_t;

/* SE CTI Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 cor:4;      /**/
	u32 rsvd0:4;    /**/
	u32 gain:6;     /**/
	u32 rsvd1:2;    /**/
	u32 gain2:6;    /**/
	u32 rsvd2:2;    /**/
	u32 oslmt:6;    /**/
	u32 rsvd3:2;    /**/
	} bits;
} reg_se_cticfg_t;

/* SE LTI Coefficient 0 Register */
typedef union {
	u32 val;
	struct {
	u32 c0:8;/**/
	u32 c1:8;/**/
	u32 c2:8;/**/
	u32 c3:8;/**/
	} bits;
} reg_se_lticoef0_t;

/* SE LTI Coefficient 1 Register */
typedef union {
	u32 val;
	struct {
	u32 c4:8;       /**/
	u32 rsvd0:24;   /**/
	} bits;
} reg_se_lticoef1_t;

/* SE CTI Coefficient 0 Register */
typedef union {
	u32 val;
	struct {
	u32 c0:8;/**/
	u32 c1:8;/**/
	u32 c2:8;/**/
	u32 c3:8;/**/
	} bits;
} reg_se_cticoef0_t;

/* SE CTI Coefficient 1 Register */
typedef union {
	u32 val;
	struct {
	u32 c4:8;       /**/
	u32 rsvd0:24;   /**/
	} bits;
} reg_se_cticoef1_t;

/* SE Ch0 X-direction Filter Coefficient 0 Register */
typedef union {
	u32 val;
	struct {
	u32 c0:8;/**/
	u32 c1:8;/**/
	u32 c2:8;/**/
	u32 c3:8;/**/
	} bits;
} reg_se_ch0_xcoef0_t;

/* SE Ch0 X-direction Filter Coefficient 1 Register */
typedef union {
	u32 val;
	struct {
	u32 c4:8;/**/
	u32 c5:8;/**/
	u32 c6:8;/**/
	u32 c7:8;/**/
	} bits;
} reg_se_ch0_xcoef1_t;

/* SE Ch0 Y-direction Filter Coefficient Register */
typedef union {
	u32 val;
	struct {
	u32 c0:8;/**/
	u32 c1:8;/**/
	u32 c2:8;/**/
	u32 c3:8;/**/
	} bits;
} reg_se_ch0_ycoef_t;

/* SE Ch1 X-direction Filter Coefficient 0 Register */
typedef union {
	u32 val;
	struct {
	u32 c0:8;/**/
	u32 c1:8;/**/
	u32 c2:8;/**/
	u32 c3:8;/**/
	} bits;
} reg_se_ch1_xcoef0_t;

/* SE Ch1 X-direction Filter Coefficient 1 Register */
typedef union {
	u32 val;
	struct {
	u32 c4:8;/**/
	u32 c5:8;/**/
	u32 c6:8;/**/
	u32 c7:8;/**/
	} bits;
} reg_se_ch1_xcoef1_t;

/* SE Ch1 Y-direction Filter Coefficient Register */
typedef union {
	u32 val;
	struct {
	u32 c0:8;/**/
	u32 c1:8;/**/
	u32 c2:8;/**/
	u32 c3:8;/**/
	} bits;
} reg_se_ch1_ycoef_t;

/* SE Ch0 LUT Register */
typedef union {
	u32 val;
	struct {
	u32 c0:8;/**/
	u32 c1:8;/**/
	u32 c2:8;/**/
	u32 c3:8;/**/
	} bits;
} reg_se_ch0lut_t;

/* SE Ch1 LUT Register */
typedef union {
	u32 val;
	struct {
	u32 c0:8;/**/
	u32 c1:8;/**/
	u32 c2:8;/**/
	u32 c3:8;/**/
	} bits;
} reg_se_ch1lut_t;

/* SE Ch2 LUT Register */
typedef union {
	u32 val;
	struct {
	u32 c0:8;/**/
	u32 c1:8;/**/
	u32 c2:8;/**/
	u32 c3:8;/**/
	} bits;
} reg_se_ch2lut_t;

/* SE Channel Histogram Register */
typedef union {
	u32 val;
	struct {
	u32 count:32;   /**/
	} bits;
} reg_se_chhist_t;

/* reg_se_t bank */
typedef struct tag_se {
	reg_se_ver_t           ver;            /* 0000 */
	reg_se_ftr_t           ftr;            /* 0004 */
	reg_se_csta_t          csta;           /* 0008 */
	reg_se_sta_t           sta;            /* 000C */
	reg_se_ctl_t           ctl;            /* 0010 */
	reg_se_cfg0_t          cfg0;           /* 0014 */
	reg_se_cfg1_t          cfg1;           /* 0018 */
	reg_se_cfg2_t          cfg2;           /* 001C */
	reg_se_intctl_t        intctl;         /* 0020 */
	reg_se_intsta_t        intsta;         /* 0024 */
	reg_se_intclr_t        intclr;         /* 0028 */
	reg_se_lncnt_t         lncnt;          /* 002C */
	reg_se_updctl_t        updctl;         /* 0030 */
	reg_se_ctabswt_t       ctabswt;        /* 0034 */
	u32                    res0[2];
	reg_se_strmctl_t       strmctl;        /* 0040 */
	reg_se_inaddr0_t       inaddr0;        /* 0044 */
	reg_se_inaddr1_t       inaddr1;        /* 0048 */
	reg_se_inaddr2_t       inaddr2;        /* 004C */
	reg_se_insize0_t       insize0;        /* 0050 */
	reg_se_insize1_t       insize1;        /* 0054 */
	reg_se_rsmpsize_t      rsmpsize;       /* 0058 */
	u32                    res1[1];
	reg_se_inlstr0_t       inlstr0;        /* 0060 */
	reg_se_inlstr1_t       inlstr1;        /* 0064 */
	u32                    res2[2];
	reg_se_scrx0_t         scrx0;          /* 0070 */
	reg_se_scry0_t         scry0;          /* 0074 */
	reg_se_scrx1_t         scrx1;          /* 0078 */
	reg_se_scry1_t         scry1;          /* 007C */
	reg_se_ch0xofst_t      ch0xofst;       /* 0080 */
	reg_se_ch0yofst_t      ch0yofst;       /* 0084 */
	reg_se_ch1xofst_t      ch1xofst;       /* 0088 */
	reg_se_ch1yofst_t      ch1yofst;       /* 008C */
	u32                    res3[2];
	reg_se_shiftctl_t      shiftctl;       /* 0098 */
	u32                    res4[1];
	reg_se_out_cropofst_t  out_cropofst;   /* 00A0 */
	reg_se_out_cropsize_t  out_cropsize;   /* 00A4 */
	reg_se_wbsize0_t       wbsize0;        /* 00A8 */
	reg_se_wbsize1_t       wbsize1;        /* 00AC */
	u32                    res5[4];
	reg_se_wbctl_t         wbctl;          /* 00C0 */
	reg_se_wbcfg_t         wbcfg;          /* 00C4 */
	reg_se_wbtmr_t         wbtmr;          /* 00C8 */
	u32                    res6[1];
	reg_se_wbaddr0_t       wbaddr0;        /* 00D0 */
	reg_se_wbaddr1_t       wbaddr1;        /* 00D4 */
	reg_se_wbaddr2_t       wbaddr2;        /* 00D8 */
	reg_se_wblstr0_t       wblstr0;        /* 00DC */
	reg_se_wblstr1_t       wblstr1;        /* 00E0 */
	u32                    res7[7];
	reg_se_csci_coef_t     csci_coef;      /* 0100 */
	u32                    res8[11];
	reg_se_csco_coef_t     csco_coef;      /* 0130 */
	u32                    res9[19];
	reg_se_vppctl_t        vppctl;         /* 0180 */
	reg_se_vpplutsw_t      vpplutsw;       /* 0184 */
	reg_se_checfg0_t       checfg0;        /* 0188 */
	reg_se_checfg1_t       checfg1;        /* 018C */
	u32                    res10[4];
	reg_se_peakcfg_t       peakcfg;        /* 01A0 */
	reg_se_ltivcfg_t       ltivcfg;        /* 01A4 */
	reg_se_peakcoef0_t     peakcoef0;      /* 01A8 */
	reg_se_peakcoef1_t     peakcoef1;      /* 01AC */
	reg_se_lticfg_t        lticfg;         /* 01B0 */
	reg_se_cticfg_t        cticfg;         /* 01B4 */
	reg_se_lticoef0_t      lticoef0;       /* 01B8 */
	reg_se_lticoef1_t      lticoef1;       /* 01BC */
	reg_se_cticoef0_t      cticoef0;       /* 01C0 */
	reg_se_cticoef1_t      cticoef1;       /* 01C4 */
	u32                    res11[142];
	reg_se_ch0_xcoef0_t    ch0_xcoef0;     /* 0400 */
	u32                    res12[31];
	reg_se_ch0_xcoef1_t    ch0_xcoef1;     /* 0480 */
	u32                    res13[31];
	reg_se_ch0_ycoef_t     ch0_ycoef;      /* 0500 */
	u32                    res14[31];
	reg_se_ch1_xcoef0_t    ch1_xcoef0;     /* 0580 */
	u32                    res15[31];
	reg_se_ch1_xcoef1_t    ch1_xcoef1;     /* 0600 */
	u32                    res16[31];
	reg_se_ch1_ycoef_t     ch1_ycoef;      /* 0680 */
	u32                    res17[159];
	reg_se_ch0lut_t        ch0lut;         /* 0900 */
	u32                    res18[63];
	reg_se_ch1lut_t        ch1lut;         /* 0A00 */
	u32                    res19[63];
	reg_se_ch2lut_t        ch2lut;         /* 0B00 */
	u32                    res20[63];
	reg_se_chhist_t        chhist;         /* 0C00 */
} reg_se_t;

#endif /* ___SE___H___ */
