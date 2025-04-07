/* dc.h */

#ifndef ___DC___H___
#define ___DC___H___

#define BASE_DC                0x01520000

#define VA_DC_GNECTL                     (0x00000000 + BASE_DC + VA_DC)
#define VA_DC_IMGSRCCTL                  (0x00000004 + BASE_DC + VA_DC)
#define VA_DC_SCRSIZ                     (0x00000008 + BASE_DC + VA_DC)
#define VA_DC_BKCOLOR                    (0x0000000C + BASE_DC + VA_DC)
#define VA_DC_VER                        (0x00000010 + BASE_DC + VA_DC)
#define VA_DC_FTR                        (0x00000014 + BASE_DC + VA_DC)
#define VA_DC_WIN0CTL                    (0x00000020 + BASE_DC + VA_DC)
#define VA_DC_WIN1CTL                    (0x00000024 + BASE_DC + VA_DC)
#define VA_DC_WIN2CTL                    (0x00000028 + BASE_DC + VA_DC)
#define VA_DC_WIN3CTL                    (0x0000002C + BASE_DC + VA_DC)
#define VA_DC_WIN0BUFFMT                 (0x00000030 + BASE_DC + VA_DC)
#define VA_DC_WIN1BUFFMT                 (0x00000034 + BASE_DC + VA_DC)
#define VA_DC_WIN2BUFFMT                 (0x00000038 + BASE_DC + VA_DC)
#define VA_DC_WIN3BUFFMT                 (0x0000003C + BASE_DC + VA_DC)
#define VA_DC_WIN0SIZ                    (0x00000040 + BASE_DC + VA_DC)
#define VA_DC_WIN1SIZ                    (0x00000044 + BASE_DC + VA_DC)
#define VA_DC_WIN2SIZ                    (0x00000048 + BASE_DC + VA_DC)
#define VA_DC_WIN3SIZ                    (0x0000004C + BASE_DC + VA_DC)
#define VA_DC_WIN0COOR                   (0x00000050 + BASE_DC + VA_DC)
#define VA_DC_WIN1COOR                   (0x00000054 + BASE_DC + VA_DC)
#define VA_DC_WIN2COOR                   (0x00000058 + BASE_DC + VA_DC)
#define VA_DC_WIN3COOR                   (0x0000005C + BASE_DC + VA_DC)
#define VA_DC_WIN0BUFLADD                (0x00000060 + BASE_DC + VA_DC)
#define VA_DC_WIN1BUFLADD                (0x00000064 + BASE_DC + VA_DC)
#define VA_DC_WIN2BUFLADD                (0x00000068 + BASE_DC + VA_DC)
#define VA_DC_WIN3BUFLADD                (0x0000006C + BASE_DC + VA_DC)
#define VA_DC_WINBUFHADD                 (0x00000070 + BASE_DC + VA_DC)
#define VA_DC_WIN0BUFLNSTD               (0x00000080 + BASE_DC + VA_DC)
#define VA_DC_WIN1BUFLNSTD               (0x00000084 + BASE_DC + VA_DC)
#define VA_DC_WIN2BUFLNSTD               (0x00000088 + BASE_DC + VA_DC)
#define VA_DC_WIN3BUFLNSTD               (0x0000008C + BASE_DC + VA_DC)
#define VA_DC_BLD0CTL                    (0x00000090 + BASE_DC + VA_DC)
#define VA_DC_BLD1CTL                    (0x00000094 + BASE_DC + VA_DC)
#define VA_DC_BLD2CTL                    (0x00000098 + BASE_DC + VA_DC)
#define VA_DC_BLD3CTL                    (0x0000009C + BASE_DC + VA_DC)
#define VA_DC_BLD4CTL                    (0x000000A0 + BASE_DC + VA_DC)
#define VA_DC_BLD0CKMIN                  (0x000000B0 + BASE_DC + VA_DC)
#define VA_DC_BLD1CKMIN                  (0x000000B4 + BASE_DC + VA_DC)
#define VA_DC_BLD2CKMIN                  (0x000000B8 + BASE_DC + VA_DC)
#define VA_DC_BLD3CKMIN                  (0x000000BC + BASE_DC + VA_DC)
#define VA_DC_BLD4CKMIN                  (0x000000C0 + BASE_DC + VA_DC)
#define VA_DC_BLD0CKMAX                  (0x000000D0 + BASE_DC + VA_DC)
#define VA_DC_BLD1CKMAX                  (0x000000D4 + BASE_DC + VA_DC)
#define VA_DC_BLD2CKMAX                  (0x000000D8 + BASE_DC + VA_DC)
#define VA_DC_BLD3CKMAX                  (0x000000DC + BASE_DC + VA_DC)
#define VA_DC_BLD4CKMAX                  (0x000000E0 + BASE_DC + VA_DC)
#define VA_DC_HWCCTL                     (0x000000F0 + BASE_DC + VA_DC)
#define VA_DC_HWCCOOR                    (0x000000F4 + BASE_DC + VA_DC)
#define VA_DC_CEEN                       (0x00000100 + BASE_DC + VA_DC)
#define VA_DC_CERRCOEF                   (0x00000110 + BASE_DC + VA_DC)
#define VA_DC_CERGCOEF                   (0x00000114 + BASE_DC + VA_DC)
#define VA_DC_CERBCOEF                   (0x00000118 + BASE_DC + VA_DC)
#define VA_DC_CERCONS                    (0x0000011C + BASE_DC + VA_DC)
#define VA_DC_CEGRCOEF                   (0x00000120 + BASE_DC + VA_DC)
#define VA_DC_CEGGCOEF                   (0x00000124 + BASE_DC + VA_DC)
#define VA_DC_CEGBCOEF                   (0x00000128 + BASE_DC + VA_DC)
#define VA_DC_CEGCONS                    (0x0000012C + BASE_DC + VA_DC)
#define VA_DC_CEBRCOEF                   (0x00000130 + BASE_DC + VA_DC)
#define VA_DC_CEBGCOEF                   (0x00000134 + BASE_DC + VA_DC)
#define VA_DC_CEBBCOEF                   (0x00000138 + BASE_DC + VA_DC)
#define VA_DC_CEBCONS                    (0x0000013C + BASE_DC + VA_DC)
#define VA_DC_BLKGNE                     (0x000001E0 + BASE_DC + VA_DC)
#define VA_DC_WBC                        (0x000001F0 + BASE_DC + VA_DC)
#define VA_DC_WBADD                      (0x000001F4 + BASE_DC + VA_DC)
#define VA_DC_WBLNSTD                    (0x000001F8 + BASE_DC + VA_DC)
#define VA_DC_CH0_BLKCTL            (0x00000200 + BASE_DC + VA_DC)
#define VA_DC_CH1_BLKCTL            (0x00000204 + BASE_DC + VA_DC)
#define VA_DC_CH2_BLKCTL            (0x00000208 + BASE_DC + VA_DC)
#define VA_DC_CH3_BLKCTL            (0x0000020C + BASE_DC + VA_DC)
#define VA_DC_CH4_BLKCTL            (0x00000210 + BASE_DC + VA_DC)
#define VA_DC_CH5_BLKCTL            (0x00000214 + BASE_DC + VA_DC)
#define VA_DC_CH6_BLKCTL            (0x00000218 + BASE_DC + VA_DC)
#define VA_DC_CH7_BLKCTL            (0x0000021C + BASE_DC + VA_DC)
#define VA_DC_CH8_BLKCTL            (0x00000220 + BASE_DC + VA_DC)
#define VA_DC_CH9_BLKCTL            (0x00000224 + BASE_DC + VA_DC)
#define VA_DC_CH10_BLKCTL           (0x00000228 + BASE_DC + VA_DC)
#define VA_DC_CH11_BLKCTL           (0x0000022C + BASE_DC + VA_DC)
#define VA_DC_CH12_BLKCTL           (0x00000230 + BASE_DC + VA_DC)
#define VA_DC_CH13_BLKCTL           (0x00000234 + BASE_DC + VA_DC)
#define VA_DC_CH14_BLKCTL           (0x00000238 + BASE_DC + VA_DC)
#define VA_DC_CH15_BLKCTL           (0x0000023C + BASE_DC + VA_DC)
#define VA_DC_CH0_BLKCOOR           (0x00000300 + BASE_DC + VA_DC)
#define VA_DC_CH1_BLKCOOR           (0x00000304 + BASE_DC + VA_DC)
#define VA_DC_CH2_BLKCOOR           (0x00000308 + BASE_DC + VA_DC)
#define VA_DC_CH3_BLKCOOR           (0x0000030C + BASE_DC + VA_DC)
#define VA_DC_CH4_BLKCOOR           (0x00000310 + BASE_DC + VA_DC)
#define VA_DC_CH5_BLKCOOR           (0x00000314 + BASE_DC + VA_DC)
#define VA_DC_CH6_BLKCOOR           (0x00000318 + BASE_DC + VA_DC)
#define VA_DC_CH7_BLKCOOR           (0x0000031C + BASE_DC + VA_DC)
#define VA_DC_CH8_BLKCOOR           (0x00000320 + BASE_DC + VA_DC)
#define VA_DC_CH9_BLKCOOR           (0x00000324 + BASE_DC + VA_DC)
#define VA_DC_CH10_BLKCOOR          (0x00000328 + BASE_DC + VA_DC)
#define VA_DC_CH11_BLKCOOR          (0x0000032C + BASE_DC + VA_DC)
#define VA_DC_CH12_BLKCOOR          (0x00000330 + BASE_DC + VA_DC)
#define VA_DC_CH13_BLKCOOR          (0x00000334 + BASE_DC + VA_DC)
#define VA_DC_CH14_BLKCOOR          (0x00000338 + BASE_DC + VA_DC)
#define VA_DC_CH15_BLKCOOR          (0x0000033C + BASE_DC + VA_DC)
#define VA_DC_CH0_BLKBUFADD         (0x00000400 + BASE_DC + VA_DC)
#define VA_DC_CH1_BLKBUFADD         (0x00000404 + BASE_DC + VA_DC)
#define VA_DC_CH2_BLKBUFADD         (0x00000408 + BASE_DC + VA_DC)
#define VA_DC_CH3_BLKBUFADD         (0x0000040C + BASE_DC + VA_DC)
#define VA_DC_CH4_BLKBUFADD         (0x00000410 + BASE_DC + VA_DC)
#define VA_DC_CH5_BLKBUFADD         (0x00000414 + BASE_DC + VA_DC)
#define VA_DC_CH6_BLKBUFADD         (0x00000418 + BASE_DC + VA_DC)
#define VA_DC_CH7_BLKBUFADD         (0x0000041C + BASE_DC + VA_DC)
#define VA_DC_CH8_BLKBUFADD         (0x00000420 + BASE_DC + VA_DC)
#define VA_DC_CH9_BLKBUFADD         (0x00000424 + BASE_DC + VA_DC)
#define VA_DC_CH10_BLKBUFADD        (0x00000428 + BASE_DC + VA_DC)
#define VA_DC_CH11_BLKBUFADD        (0x0000042C + BASE_DC + VA_DC)
#define VA_DC_CH12_BLKBUFADD        (0x00000430 + BASE_DC + VA_DC)
#define VA_DC_CH13_BLKBUFADD        (0x00000434 + BASE_DC + VA_DC)
#define VA_DC_CH14_BLKBUFADD        (0x00000438 + BASE_DC + VA_DC)
#define VA_DC_CH15_BLKBUFADD        (0x0000043C + BASE_DC + VA_DC)
#define VA_DC_CH0_BLKBUFLNSTD       (0x00000500 + BASE_DC + VA_DC)
#define VA_DC_CH1_BLKBUFLNSTD       (0x00000504 + BASE_DC + VA_DC)
#define VA_DC_CH2_BLKBUFLNSTD       (0x00000508 + BASE_DC + VA_DC)
#define VA_DC_CH3_BLKBUFLNSTD       (0x0000050C + BASE_DC + VA_DC)
#define VA_DC_CH4_BLKBUFLNSTD       (0x00000510 + BASE_DC + VA_DC)
#define VA_DC_CH5_BLKBUFLNSTD       (0x00000514 + BASE_DC + VA_DC)
#define VA_DC_CH6_BLKBUFLNSTD       (0x00000518 + BASE_DC + VA_DC)
#define VA_DC_CH7_BLKBUFLNSTD       (0x0000051C + BASE_DC + VA_DC)
#define VA_DC_CH8_BLKBUFLNSTD       (0x00000520 + BASE_DC + VA_DC)
#define VA_DC_CH9_BLKBUFLNSTD       (0x00000524 + BASE_DC + VA_DC)
#define VA_DC_CH10_BLKBUFLNSTD      (0x00000528 + BASE_DC + VA_DC)
#define VA_DC_CH11_BLKBUFLNSTD      (0x0000052C + BASE_DC + VA_DC)
#define VA_DC_CH12_BLKBUFLNSTD      (0x00000530 + BASE_DC + VA_DC)
#define VA_DC_CH13_BLKBUFLNSTD      (0x00000534 + BASE_DC + VA_DC)
#define VA_DC_CH14_BLKBUFLNSTD      (0x00000538 + BASE_DC + VA_DC)
#define VA_DC_CH15_BLKBUFLNSTD      (0x0000053C + BASE_DC + VA_DC)

#define DATA_DC_GNECTL                   0x00000004
#define DATA_DC_IMGSRCCTL                0x00000000
#define DATA_DC_SCRSIZ                   0x00000000
#define DATA_DC_BKCOLOR                  0x00000000
#define DATA_DC_VER                      0x00001100
#define DATA_DC_FTR                      0x300003ED
#define DATA_DC_WIN0CTL                  0x00000000
#define DATA_DC_WIN1CTL                  0x00000000
#define DATA_DC_WIN2CTL                  0x00000000
#define DATA_DC_WIN3CTL                  0x00000000
#define DATA_DC_WIN0BUFFMT               0x00000000
#define DATA_DC_WIN1BUFFMT               0x00000000
#define DATA_DC_WIN2BUFFMT               0x00000000
#define DATA_DC_WIN3BUFFMT               0x00000000
#define DATA_DC_WIN0SIZ                  0x00000000
#define DATA_DC_WIN1SIZ                  0x00000000
#define DATA_DC_WIN2SIZ                  0x00000000
#define DATA_DC_WIN3SIZ                  0x00000000
#define DATA_DC_WIN0COOR                 0x00000000
#define DATA_DC_WIN1COOR                 0x00000000
#define DATA_DC_WIN2COOR                 0x00000000
#define DATA_DC_WIN3COOR                 0x00000000
#define DATA_DC_WIN0BUFLADD              0x00000000
#define DATA_DC_WIN1BUFLADD              0x00000000
#define DATA_DC_WIN2BUFLADD              0x00000000
#define DATA_DC_WIN3BUFLADD              0x00000000
#define DATA_DC_WINBUFHADD               0x00000000
#define DATA_DC_WIN0BUFLNSTD             0x00000000
#define DATA_DC_WIN1BUFLNSTD             0x00000000
#define DATA_DC_WIN2BUFLNSTD             0x00000000
#define DATA_DC_WIN3BUFLNSTD             0x00000000
#define DATA_DC_BLD0CTL                  0x00320000
#define DATA_DC_BLD1CTL                  0x00320000
#define DATA_DC_BLD2CTL                  0x00320000
#define DATA_DC_BLD3CTL                  0x00320000
#define DATA_DC_BLD4CTL                  0x00320000
#define DATA_DC_BLD0CKMIN                0x00000000
#define DATA_DC_BLD1CKMIN                0x00000000
#define DATA_DC_BLD2CKMIN                0x00000000
#define DATA_DC_BLD3CKMIN                0x00000000
#define DATA_DC_BLD4CKMIN                0x00000000
#define DATA_DC_BLD0CKMAX                0x00FFFFFF
#define DATA_DC_BLD1CKMAX                0x00FFFFFF
#define DATA_DC_BLD2CKMAX                0x00FFFFFF
#define DATA_DC_BLD3CKMAX                0x00FFFFFF
#define DATA_DC_BLD4CKMAX                0x00FFFFFF
#define DATA_DC_HWCCTL                   0x00000000
#define DATA_DC_HWCCOOR                  0x00000000
#define DATA_DC_CEEN                     0x00000000
#define DATA_DC_CERRCOEF                 0x00000000
#define DATA_DC_CERGCOEF                 0x00000000
#define DATA_DC_CERBCOEF                 0x00000000
#define DATA_DC_CERCONS                  0x00000000
#define DATA_DC_CEGRCOEF                 0x00000000
#define DATA_DC_CEGGCOEF                 0x00000000
#define DATA_DC_CEGBCOEF                 0x00000000
#define DATA_DC_CEGCONS                  0x00000000
#define DATA_DC_CEBRCOEF                 0x00000000
#define DATA_DC_CEBGCOEF                 0x00000000
#define DATA_DC_CEBBCOEF                 0x00000000
#define DATA_DC_CEBCONS                  0x00000000
#define DATA_DC_BLKGNE                   0x00000000
#define DATA_DC_WBC                      0x00000000
#define DATA_DC_WBADD                    0x00000000
#define DATA_DC_WBLNSTD                  0x00000000
#define DATA_DC_CH0_BLKCTL          0x00000000
#define DATA_DC_CH1_BLKCTL          0x00000000
#define DATA_DC_CH2_BLKCTL          0x00000000
#define DATA_DC_CH3_BLKCTL          0x00000000
#define DATA_DC_CH4_BLKCTL          0x00000000
#define DATA_DC_CH5_BLKCTL          0x00000000
#define DATA_DC_CH6_BLKCTL          0x00000000
#define DATA_DC_CH7_BLKCTL          0x00000000
#define DATA_DC_CH8_BLKCTL          0x00000000
#define DATA_DC_CH9_BLKCTL          0x00000000
#define DATA_DC_CH10_BLKCTL         0x00000000
#define DATA_DC_CH11_BLKCTL         0x00000000
#define DATA_DC_CH12_BLKCTL         0x00000000
#define DATA_DC_CH13_BLKCTL         0x00000000
#define DATA_DC_CH14_BLKCTL         0x00000000
#define DATA_DC_CH15_BLKCTL         0x00000000
#define DATA_DC_CH0_BLKCOOR         0x00000000
#define DATA_DC_CH1_BLKCOOR         0x00000000
#define DATA_DC_CH2_BLKCOOR         0x00000000
#define DATA_DC_CH3_BLKCOOR         0x00000000
#define DATA_DC_CH4_BLKCOOR         0x00000000
#define DATA_DC_CH5_BLKCOOR         0x00000000
#define DATA_DC_CH6_BLKCOOR         0x00000000
#define DATA_DC_CH7_BLKCOOR         0x00000000
#define DATA_DC_CH8_BLKCOOR         0x00000000
#define DATA_DC_CH9_BLKCOOR         0x00000000
#define DATA_DC_CH10_BLKCOOR        0x00000000
#define DATA_DC_CH11_BLKCOOR        0x00000000
#define DATA_DC_CH12_BLKCOOR        0x00000000
#define DATA_DC_CH13_BLKCOOR        0x00000000
#define DATA_DC_CH14_BLKCOOR        0x00000000
#define DATA_DC_CH15_BLKCOOR        0x00000000
#define DATA_DC_CH0_BLKBUFADD       0x00000000
#define DATA_DC_CH1_BLKBUFADD       0x00000000
#define DATA_DC_CH2_BLKBUFADD       0x00000000
#define DATA_DC_CH3_BLKBUFADD       0x00000000
#define DATA_DC_CH4_BLKBUFADD       0x00000000
#define DATA_DC_CH5_BLKBUFADD       0x00000000
#define DATA_DC_CH6_BLKBUFADD       0x00000000
#define DATA_DC_CH7_BLKBUFADD       0x00000000
#define DATA_DC_CH8_BLKBUFADD       0x00000000
#define DATA_DC_CH9_BLKBUFADD       0x00000000
#define DATA_DC_CH10_BLKBUFADD      0x00000000
#define DATA_DC_CH11_BLKBUFADD      0x00000000
#define DATA_DC_CH12_BLKBUFADD      0x00000000
#define DATA_DC_CH13_BLKBUFADD      0x00000000
#define DATA_DC_CH14_BLKBUFADD      0x00000000
#define DATA_DC_CH15_BLKBUFADD      0x00000000
#define DATA_DC_CH0_BLKBUFLNSTD     0x00000000
#define DATA_DC_CH1_BLKBUFLNSTD     0x00000000
#define DATA_DC_CH2_BLKBUFLNSTD     0x00000000
#define DATA_DC_CH3_BLKBUFLNSTD     0x00000000
#define DATA_DC_CH4_BLKBUFLNSTD     0x00000000
#define DATA_DC_CH5_BLKBUFLNSTD     0x00000000
#define DATA_DC_CH6_BLKBUFLNSTD     0x00000000
#define DATA_DC_CH7_BLKBUFLNSTD     0x00000000
#define DATA_DC_CH8_BLKBUFLNSTD     0x00000000
#define DATA_DC_CH9_BLKBUFLNSTD     0x00000000
#define DATA_DC_CH10_BLKBUFLNSTD    0x00000000
#define DATA_DC_CH11_BLKBUFLNSTD    0x00000000
#define DATA_DC_CH12_BLKBUFLNSTD    0x00000000
#define DATA_DC_CH13_BLKBUFLNSTD    0x00000000
#define DATA_DC_CH14_BLKBUFLNSTD    0x00000000
#define DATA_DC_CH15_BLKBUFLNSTD    0x00000000

/* General Control Register */
typedef union {
	u32 val;
	struct {
	u32 dcen:1;     /**/
	u32 dcrst:1;    /**/
	u32 fiforst:1;  /**/
	u32 rsvd0:5;    /**/
	u32 dbc:1;      /**/
	u32 dbautold:1; /**/
	u32 rsvd1:6;    /**/
	u32 outmode:1;  /**/
	u32 fieldpol:1; /**/
	u32 rsvd2:14;   /**/
	} bits;
} reg_dc_gnectl_t;

#define DC_GNECTL_DCEN_0        0x0
#define DC_GNECTL_DCEN_1        0x1
#define DC_GNECTL_DCRST_0       0x0
#define DC_GNECTL_DCRST_1       0x1
#define DC_GNECTL_FIFORST_0     0x0
#define DC_GNECTL_FIFORST_1     0x1
#define DC_GNECTL_DBAUTOLD_0    0x0
#define DC_GNECTL_DBAUTOLD_1    0x1
#define DC_GNECTL_OUTMODE_0     0x0
#define DC_GNECTL_OUTMODE_1     0x1
#define DC_GNECTL_FIELDPOL_0    0x0
#define DC_GNECTL_FIELDPOL_1    0x1

/* Image Source Control Register */
typedef union {
	u32 val;
	struct {
	u32 win0en:1;   /**/
	u32 win1en:1;   /**/
	u32 win2en:1;   /**/
	u32 win3en:1;   /**/
	u32 rsvd0:2;    /**/
	u32 pipe0bypass:1;/**/
	u32 pipe1bypass:1;/**/
	u32 hwcen:1;    /**/
	u32 blken:1;    /**/
	u32 rsvd1:22;   /**/
	} bits;
} reg_dc_imgsrcctl_t;

#define DC_IMGSRCCTL_WIN0EN_0           0x0
#define DC_IMGSRCCTL_WIN0EN_1           0x1
#define DC_IMGSRCCTL_WIN1EN_0           0x0
#define DC_IMGSRCCTL_WIN1EN_1           0x1
#define DC_IMGSRCCTL_WIN2EN_0           0x0
#define DC_IMGSRCCTL_WIN2EN_1           0x1
#define DC_IMGSRCCTL_WIN3EN_0           0x0
#define DC_IMGSRCCTL_WIN3EN_1           0x1
#define DC_IMGSRCCTL_PIPE0BYPASS_0      0x0
#define DC_IMGSRCCTL_PIPE0BYPASS_1      0x1
#define DC_IMGSRCCTL_PIPE1BYPASS_0      0x0
#define DC_IMGSRCCTL_PIPE1BYPASS_1      0x1
#define DC_IMGSRCCTL_HWCEN_0            0x0
#define DC_IMGSRCCTL_HWCEN_1            0x1
#define DC_IMGSRCCTL_BLKEN_0            0x0
#define DC_IMGSRCCTL_BLKEN_1            0x1

/* Screen Size Register */
typedef union {
	u32 val;
	struct {
	u32 w:16;/**/
	u32 h:16;/**/
	} bits;
} reg_dc_scrsiz_t;

/* Back Color Register */
typedef union {
	u32 val;
	struct {
	u32 b:8;/**/
	u32 g:8;/**/
	u32 r:8;/**/
	u32 a:8;/**/
	} bits;
} reg_dc_bkcolor_t;

/* Version Register */
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
} reg_dc_ver_t;

#define DC_VER_COMP_0   0x0
#define DC_VER_COMP_1   0x1

/* Feature Register */
typedef union {
	u32 val;
	struct {
	u32 has_wb:1;           /**/
	u32 has_yuv:1;          /**/
	u32 has_hwc:1;          /**/
	u32 has_bkl:1;          /**/
	u32 has_ifb:1;          /**/
	u32 has_pal:1;          /**/
	u32 has_gamma:1;        /**/
	u32 has_blend:1;        /**/
	u32 has_colorkey:1;     /**/
	u32 has_csc:1;          /**/
	u32 has_ove:1;          /**/
	u32 rsvd0:13;           /**/
	u32 ovenum:4;           /**/
	u32 winnum:4;           /**/
	} bits;
} reg_dc_ftr_t;

#define DC_FTR_HAS_WB_0         0x0
#define DC_FTR_HAS_WB_1         0x1
#define DC_FTR_HAS_YUV_0        0x0
#define DC_FTR_HAS_YUV_1        0x1
#define DC_FTR_HAS_HWC_0        0x0
#define DC_FTR_HAS_HWC_1        0x1
#define DC_FTR_HAS_BKL_0        0x0
#define DC_FTR_HAS_BKL_1        0x1
#define DC_FTR_HAS_IFB_0        0x0
#define DC_FTR_HAS_IFB_1        0x1
#define DC_FTR_HAS_PAL_0        0x0
#define DC_FTR_HAS_PAL_1        0x1
#define DC_FTR_HAS_GAMMA_0      0x0
#define DC_FTR_HAS_GAMMA_1      0x1
#define DC_FTR_HAS_BLEND_0      0x0
#define DC_FTR_HAS_BLEND_1      0x1
#define DC_FTR_HAS_COLORKEY_0   0x0
#define DC_FTR_HAS_COLORKEY_1   0x1
#define DC_FTR_HAS_CSC_0        0x0
#define DC_FTR_HAS_CSC_1        0x1
#define DC_FTR_HAS_OVE_0        0x0
#define DC_FTR_HAS_OVE_1        0x1

/* Window0 Control Register */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:4;    /**/
	u32 winwkmod:2; /**/
	u32 rsvd1:2;    /**/
	u32 winsrc:2;   /**/
	u32 rsvd2:6;    /**/
	u32 alphactl:2; /**/
	u32 rsvd3:6;    /**/
	u32 plaalpha:8; /**/
	} bits;
} reg_dc_win0ctl_t;

#define DC_WIN0CTL_WINWKMOD_0   0x0
#define DC_WIN0CTL_WINWKMOD_1   0x1
#define DC_WIN0CTL_WINWKMOD_2   0x2
#define DC_WIN0CTL_WINWKMOD_3   0x3
#define DC_WIN0CTL_WINSRC_0     0x0
#define DC_WIN0CTL_WINSRC_1     0x1
#define DC_WIN0CTL_WINSRC_2     0x2
#define DC_WIN0CTL_WINSRC_3     0x3
#define DC_WIN0CTL_ALPHACTL_0   0x0
#define DC_WIN0CTL_ALPHACTL_1   0x1
#define DC_WIN0CTL_ALPHACTL_2   0x2
#define DC_WIN0CTL_ALPHACTL_3   0x3

/* Window1 Control Register */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:4;    /**/
	u32 winwkmod:2; /**/
	u32 rsvd1:2;    /**/
	u32 winsrc:2;   /**/
	u32 rsvd2:6;    /**/
	u32 alphactl:2; /**/
	u32 rsvd3:6;    /**/
	u32 plaalpha:8; /**/
	} bits;
} reg_dc_win1ctl_t;

#define DC_WIN1CTL_WINWKMOD_0   0x0
#define DC_WIN1CTL_WINWKMOD_1   0x1
#define DC_WIN1CTL_WINWKMOD_2   0x2
#define DC_WIN1CTL_WINWKMOD_3   0x3
#define DC_WIN1CTL_WINSRC_0     0x0
#define DC_WIN1CTL_WINSRC_1     0x1
#define DC_WIN1CTL_WINSRC_2     0x2
#define DC_WIN1CTL_WINSRC_3     0x3
#define DC_WIN1CTL_ALPHACTL_0   0x0
#define DC_WIN1CTL_ALPHACTL_1   0x1
#define DC_WIN1CTL_ALPHACTL_2   0x2
#define DC_WIN1CTL_ALPHACTL_3   0x3

/* Window2 Control Register */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:4;    /**/
	u32 winwkmod:2; /**/
	u32 rsvd1:2;    /**/
	u32 winsrc:2;   /**/
	u32 rsvd2:6;    /**/
	u32 alphactl:2; /**/
	u32 rsvd3:6;    /**/
	u32 plaalpha:8; /**/
	} bits;
} reg_dc_win2ctl_t;

#define DC_WIN2CTL_WINWKMOD_0   0x0
#define DC_WIN2CTL_WINWKMOD_1   0x1
#define DC_WIN2CTL_WINWKMOD_2   0x2
#define DC_WIN2CTL_WINWKMOD_3   0x3
#define DC_WIN2CTL_WINSRC_0     0x0
#define DC_WIN2CTL_WINSRC_1     0x1
#define DC_WIN2CTL_WINSRC_2     0x2
#define DC_WIN2CTL_WINSRC_3     0x3
#define DC_WIN2CTL_ALPHACTL_0   0x0
#define DC_WIN2CTL_ALPHACTL_1   0x1
#define DC_WIN2CTL_ALPHACTL_2   0x2
#define DC_WIN2CTL_ALPHACTL_3   0x3

/* Window3 Control Register */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:4;    /**/
	u32 winwkmod:2; /**/
	u32 rsvd1:2;    /**/
	u32 winsrc:2;   /**/
	u32 rsvd2:6;    /**/
	u32 alphactl:2; /**/
	u32 rsvd3:6;    /**/
	u32 plaalpha:8; /**/
	} bits;
} reg_dc_win3ctl_t;

#define DC_WIN3CTL_WINWKMOD_0   0x0
#define DC_WIN3CTL_WINWKMOD_1   0x1
#define DC_WIN3CTL_WINWKMOD_2   0x2
#define DC_WIN3CTL_WINWKMOD_3   0x3
#define DC_WIN3CTL_WINSRC_0     0x0
#define DC_WIN3CTL_WINSRC_1     0x1
#define DC_WIN3CTL_WINSRC_2     0x2
#define DC_WIN3CTL_WINSRC_3     0x3
#define DC_WIN3CTL_ALPHACTL_0   0x0
#define DC_WIN3CTL_ALPHACTL_1   0x1
#define DC_WIN3CTL_ALPHACTL_2   0x2
#define DC_WIN3CTL_ALPHACTL_3   0x3

/* Window0 Buffer Format Register */
typedef union {
	u32 val;
	struct {
	u32 po:2;       /**/
	u32 rsvd0:6;    /**/
	u32 fmt:5;      /**/
	u32 rsvd1:19;   /**/
	} bits;
} reg_dc_win0buffmt_t;

#define DC_WIN0BUFFMT_FMT_0     0x0
#define DC_WIN0BUFFMT_FMT_1     0x1
#define DC_WIN0BUFFMT_FMT_2     0x2
#define DC_WIN0BUFFMT_FMT_3     0x3
#define DC_WIN0BUFFMT_FMT_4     0x4
#define DC_WIN0BUFFMT_FMT_5     0x5
#define DC_WIN0BUFFMT_FMT_6     0x6
#define DC_WIN0BUFFMT_FMT_7     0x7
#define DC_WIN0BUFFMT_FMT_8     0x8
#define DC_WIN0BUFFMT_FMT_9     0x9
#define DC_WIN0BUFFMT_FMT_A     0xA
#define DC_WIN0BUFFMT_FMT_B     0xB
#define DC_WIN0BUFFMT_FMT_C     0xC
#define DC_WIN0BUFFMT_FMT_D     0xD
#define DC_WIN0BUFFMT_FMT_E     0xE
#define DC_WIN0BUFFMT_FMT_F     0xF
#define DC_WIN0BUFFMT_FMT_10    0x10
#define DC_WIN0BUFFMT_FMT_11    0x11
#define DC_WIN0BUFFMT_FMT_12    0x12
#define DC_WIN0BUFFMT_FMT_13    0x13

/* Window1 Buffer Format Register */
typedef union {
	u32 val;
	struct {
	u32 po:2;       /**/
	u32 rsvd0:6;    /**/
	u32 fmt:5;      /**/
	u32 rsvd1:19;   /**/
	} bits;
} reg_dc_win1buffmt_t;

#define DC_WIN1BUFFMT_FMT_0     0x0
#define DC_WIN1BUFFMT_FMT_1     0x1
#define DC_WIN1BUFFMT_FMT_2     0x2
#define DC_WIN1BUFFMT_FMT_3     0x3
#define DC_WIN1BUFFMT_FMT_4     0x4
#define DC_WIN1BUFFMT_FMT_5     0x5
#define DC_WIN1BUFFMT_FMT_6     0x6
#define DC_WIN1BUFFMT_FMT_7     0x7
#define DC_WIN1BUFFMT_FMT_8     0x8
#define DC_WIN1BUFFMT_FMT_9     0x9
#define DC_WIN1BUFFMT_FMT_A     0xA
#define DC_WIN1BUFFMT_FMT_B     0xB
#define DC_WIN1BUFFMT_FMT_C     0xC
#define DC_WIN1BUFFMT_FMT_D     0xD
#define DC_WIN1BUFFMT_FMT_E     0xE
#define DC_WIN1BUFFMT_FMT_F     0xF
#define DC_WIN1BUFFMT_FMT_10    0x10
#define DC_WIN1BUFFMT_FMT_11    0x11
#define DC_WIN1BUFFMT_FMT_12    0x12
#define DC_WIN1BUFFMT_FMT_13    0x13

/* Window2 Buffer Format Register */
typedef union {
	u32 val;
	struct {
	u32 po:2;       /**/
	u32 rsvd0:6;    /**/
	u32 fmt:5;      /**/
	u32 rsvd1:19;   /**/
	} bits;
} reg_dc_win2buffmt_t;

#define DC_WIN2BUFFMT_FMT_0     0x0
#define DC_WIN2BUFFMT_FMT_1     0x1
#define DC_WIN2BUFFMT_FMT_2     0x2
#define DC_WIN2BUFFMT_FMT_3     0x3
#define DC_WIN2BUFFMT_FMT_4     0x4
#define DC_WIN2BUFFMT_FMT_5     0x5
#define DC_WIN2BUFFMT_FMT_6     0x6
#define DC_WIN2BUFFMT_FMT_7     0x7
#define DC_WIN2BUFFMT_FMT_8     0x8
#define DC_WIN2BUFFMT_FMT_9     0x9
#define DC_WIN2BUFFMT_FMT_A     0xA
#define DC_WIN2BUFFMT_FMT_B     0xB
#define DC_WIN2BUFFMT_FMT_C     0xC
#define DC_WIN2BUFFMT_FMT_D     0xD
#define DC_WIN2BUFFMT_FMT_E     0xE
#define DC_WIN2BUFFMT_FMT_F     0xF
#define DC_WIN2BUFFMT_FMT_10    0x10
#define DC_WIN2BUFFMT_FMT_11    0x11
#define DC_WIN2BUFFMT_FMT_12    0x12
#define DC_WIN2BUFFMT_FMT_13    0x13

/* Window3 Buffer Format Register */
typedef union {
	u32 val;
	struct {
	u32 po:2;       /**/
	u32 rsvd0:6;    /**/
	u32 fmt:5;      /**/
	u32 rsvd1:19;   /**/
	} bits;
} reg_dc_win3buffmt_t;

#define DC_WIN3BUFFMT_FMT_0     0x0
#define DC_WIN3BUFFMT_FMT_1     0x1
#define DC_WIN3BUFFMT_FMT_2     0x2
#define DC_WIN3BUFFMT_FMT_3     0x3
#define DC_WIN3BUFFMT_FMT_4     0x4
#define DC_WIN3BUFFMT_FMT_5     0x5
#define DC_WIN3BUFFMT_FMT_6     0x6
#define DC_WIN3BUFFMT_FMT_7     0x7
#define DC_WIN3BUFFMT_FMT_8     0x8
#define DC_WIN3BUFFMT_FMT_9     0x9
#define DC_WIN3BUFFMT_FMT_A     0xA
#define DC_WIN3BUFFMT_FMT_B     0xB
#define DC_WIN3BUFFMT_FMT_C     0xC
#define DC_WIN3BUFFMT_FMT_D     0xD
#define DC_WIN3BUFFMT_FMT_E     0xE
#define DC_WIN3BUFFMT_FMT_F     0xF
#define DC_WIN3BUFFMT_FMT_10    0x10
#define DC_WIN3BUFFMT_FMT_11    0x11
#define DC_WIN3BUFFMT_FMT_12    0x12
#define DC_WIN3BUFFMT_FMT_13    0x13

/* Window0 Size Register */
typedef union {
	u32 val;
	struct {
	u32 w:13;       /**/
	u32 rsvd0:3;    /**/
	u32 h:13;       /**/
	u32 rsvd1:3;    /**/
	} bits;
} reg_dc_win0siz_t;

/* Window1 Size Register */
typedef union {
	u32 val;
	struct {
	u32 w:13;       /**/
	u32 rsvd0:3;    /**/
	u32 h:13;       /**/
	u32 rsvd1:3;    /**/
	} bits;
} reg_dc_win1siz_t;

/* Window2 Size Register */
typedef union {
	u32 val;
	struct {
	u32 w:13;       /**/
	u32 rsvd0:3;    /**/
	u32 h:13;       /**/
	u32 rsvd1:3;    /**/
	} bits;
} reg_dc_win2siz_t;

/* Window3 Size Register */
typedef union {
	u32 val;
	struct {
	u32 w:13;       /**/
	u32 rsvd0:3;    /**/
	u32 h:13;       /**/
	u32 rsvd1:3;    /**/
	} bits;
} reg_dc_win3siz_t;

/* Window0 Coordinate Register */
typedef union {
	u32 val;
	struct {
	u32 x:16;/**/
	u32 y:16;/**/
	} bits;
} reg_dc_win0coor_t;

/* Window1 Coordinate Register */
typedef union {
	u32 val;
	struct {
	u32 x:16;/**/
	u32 y:16;/**/
	} bits;
} reg_dc_win1coor_t;

/* Window2 Coordinate Register */
typedef union {
	u32 val;
	struct {
	u32 x:16;/**/
	u32 y:16;/**/
	} bits;
} reg_dc_win2coor_t;

/* Window3 Coordinate Register */
typedef union {
	u32 val;
	struct {
	u32 x:16;/**/
	u32 y:16;/**/
	} bits;
} reg_dc_win3coor_t;

/* Window0 Buffer Low 32bit Address Register */
typedef union {
	u32 val;
	struct {
	u32 add:32;/**/
	} bits;
} reg_dc_win0bufladd_t;

/* Window1 Buffer Low 32bit Address Register */
typedef union {
	u32 val;
	struct {
	u32 add:32;/**/
	} bits;
} reg_dc_win1bufladd_t;

/* Window2 Buffer Low 32bit Address Register */
typedef union {
	u32 val;
	struct {
	u32 add:32;/**/
	} bits;
} reg_dc_win2bufladd_t;

/* Window3 Buffer Low 32bit Address Register */
typedef union {
	u32 val;
	struct {
	u32 add:32;/**/
	} bits;
} reg_dc_win3bufladd_t;

/* Window Buffer High 4bit Address Register */
typedef union {
	u32 val;
	struct {
	u32 win0add:4;  /**/
	u32 rsvd0:4;    /**/
	u32 win1add:4;  /**/
	u32 rsvd1:4;    /**/
	u32 win2add:4;  /**/
	u32 rsvd2:4;    /**/
	u32 win3add:4;  /**/
	u32 rsvd3:4;    /**/
	} bits;
} reg_dc_winbufhadd_t;

/* Window0 Buffer Line Stride Register */
typedef union {
	u32 val;
	struct {
	u32 std:32;/**/
	} bits;
} reg_dc_win0buflnstd_t;

/* Window1 Buffer Line Stride Register */
typedef union {
	u32 val;
	struct {
	u32 std:32;/**/
	} bits;
} reg_dc_win1buflnstd_t;

/* Window2 Buffer Line Stride Register */
typedef union {
	u32 val;
	struct {
	u32 std:32;/**/
	} bits;
} reg_dc_win2buflnstd_t;

/* Window3 Buffer Line Stride Register */
typedef union {
	u32 val;
	struct {
	u32 std:32;/**/
	} bits;
} reg_dc_win3buflnstd_t;

/* Blender0 Control Register */
typedef union {
	u32 val;
	struct {
	u32 cken:1;     /**/
	u32 ckmatch:1;  /**/
	u32 ckbcon:2;   /**/
	u32 ckgcon:2;   /**/
	u32 ckrcon:2;   /**/
	u32 rsvd0:8;    /**/
	u32 c0:4;       /**/
	u32 c1:4;       /**/
	u32 c2:4;       /**/
	u32 c3:4;       /**/
	} bits;
} reg_dc_bld0ctl_t;

#define DC_BLD0CTL_CKEN_0       0x0
#define DC_BLD0CTL_CKEN_1       0x1
#define DC_BLD0CTL_CKMATCH_0    0x0
#define DC_BLD0CTL_CKMATCH_1    0x1
#define DC_BLD0CTL_CKBCON_0     0x0
#define DC_BLD0CTL_CKBCON_1     0x1
#define DC_BLD0CTL_CKBCON_2     0x2
#define DC_BLD0CTL_CKBCON_3     0x3
#define DC_BLD0CTL_C0_0         0x0
#define DC_BLD0CTL_C0_1         0x1
#define DC_BLD0CTL_C0_2         0x2
#define DC_BLD0CTL_C0_3         0x3
#define DC_BLD0CTL_C0_4         0x4
#define DC_BLD0CTL_C0_5         0x5

/* Blender1 Control Register */
typedef union {
	u32 val;
	struct {
	u32 cken:1;     /**/
	u32 ckmatch:1;  /**/
	u32 ckbcon:2;   /**/
	u32 ckgcon:2;   /**/
	u32 ckrcon:2;   /**/
	u32 rsvd0:8;    /**/
	u32 c0:4;       /**/
	u32 c1:4;       /**/
	u32 c2:4;       /**/
	u32 c3:4;       /**/
	} bits;
} reg_dc_bld1ctl_t;

#define DC_BLD1CTL_CKEN_0       0x0
#define DC_BLD1CTL_CKEN_1       0x1
#define DC_BLD1CTL_CKMATCH_0    0x0
#define DC_BLD1CTL_CKMATCH_1    0x1
#define DC_BLD1CTL_CKBCON_0     0x0
#define DC_BLD1CTL_CKBCON_1     0x1
#define DC_BLD1CTL_CKBCON_2     0x2
#define DC_BLD1CTL_CKBCON_3     0x3
#define DC_BLD1CTL_C0_0         0x0
#define DC_BLD1CTL_C0_1         0x1
#define DC_BLD1CTL_C0_2         0x2
#define DC_BLD1CTL_C0_3         0x3
#define DC_BLD1CTL_C0_4         0x4
#define DC_BLD1CTL_C0_5         0x5

/* Blender2 Control Register */
typedef union {
	u32 val;
	struct {
	u32 cken:1;     /**/
	u32 ckmatch:1;  /**/
	u32 ckbcon:2;   /**/
	u32 ckgcon:2;   /**/
	u32 ckrcon:2;   /**/
	u32 rsvd0:8;    /**/
	u32 c0:4;       /**/
	u32 c1:4;       /**/
	u32 c2:4;       /**/
	u32 c3:4;       /**/
	} bits;
} reg_dc_bld2ctl_t;

#define DC_BLD2CTL_CKEN_0       0x0
#define DC_BLD2CTL_CKEN_1       0x1
#define DC_BLD2CTL_CKMATCH_0    0x0
#define DC_BLD2CTL_CKMATCH_1    0x1
#define DC_BLD2CTL_CKBCON_0     0x0
#define DC_BLD2CTL_CKBCON_1     0x1
#define DC_BLD2CTL_CKBCON_2     0x2
#define DC_BLD2CTL_CKBCON_3     0x3
#define DC_BLD2CTL_C0_0         0x0
#define DC_BLD2CTL_C0_1         0x1
#define DC_BLD2CTL_C0_2         0x2
#define DC_BLD2CTL_C0_3         0x3
#define DC_BLD2CTL_C0_4         0x4
#define DC_BLD2CTL_C0_5         0x5

/* Blender3 Control Register */
typedef union {
	u32 val;
	struct {
	u32 cken:1;     /**/
	u32 ckmatch:1;  /**/
	u32 ckbcon:2;   /**/
	u32 ckgcon:2;   /**/
	u32 ckrcon:2;   /**/
	u32 rsvd0:8;    /**/
	u32 c0:4;       /**/
	u32 c1:4;       /**/
	u32 c2:4;       /**/
	u32 c3:4;       /**/
	} bits;
} reg_dc_bld3ctl_t;

#define DC_BLD3CTL_CKEN_0       0x0
#define DC_BLD3CTL_CKEN_1       0x1
#define DC_BLD3CTL_CKMATCH_0    0x0
#define DC_BLD3CTL_CKMATCH_1    0x1
#define DC_BLD3CTL_CKBCON_0     0x0
#define DC_BLD3CTL_CKBCON_1     0x1
#define DC_BLD3CTL_CKBCON_2     0x2
#define DC_BLD3CTL_CKBCON_3     0x3
#define DC_BLD3CTL_C0_0         0x0
#define DC_BLD3CTL_C0_1         0x1
#define DC_BLD3CTL_C0_2         0x2
#define DC_BLD3CTL_C0_3         0x3
#define DC_BLD3CTL_C0_4         0x4
#define DC_BLD3CTL_C0_5         0x5

/* Blender4 Control Register */
typedef union {
	u32 val;
	struct {
	u32 cken:1;     /**/
	u32 ckmatch:1;  /**/
	u32 ckbcon:2;   /**/
	u32 ckgcon:2;   /**/
	u32 ckrcon:2;   /**/
	u32 rsvd0:8;    /**/
	u32 c0:4;       /**/
	u32 c1:4;       /**/
	u32 c2:4;       /**/
	u32 c3:4;       /**/
	} bits;
} reg_dc_bld4ctl_t;

#define DC_BLD4CTL_CKEN_0       0x0
#define DC_BLD4CTL_CKEN_1       0x1
#define DC_BLD4CTL_CKMATCH_0    0x0
#define DC_BLD4CTL_CKMATCH_1    0x1
#define DC_BLD4CTL_CKBCON_0     0x0
#define DC_BLD4CTL_CKBCON_1     0x1
#define DC_BLD4CTL_CKBCON_2     0x2
#define DC_BLD4CTL_CKBCON_3     0x3
#define DC_BLD4CTL_C0_0         0x0
#define DC_BLD4CTL_C0_1         0x1
#define DC_BLD4CTL_C0_2         0x2
#define DC_BLD4CTL_C0_3         0x3
#define DC_BLD4CTL_C0_4         0x4
#define DC_BLD4CTL_C0_5         0x5

/* Blender0 Color Key MIN Register */
typedef union {
	u32 val;
	struct {
	u32 ckbmin:8;   /**/
	u32 ckgmin:8;   /**/
	u32 ckrmin:8;   /**/
	u32 rsvd0:8;    /**/
	} bits;
} reg_dc_bld0ckmin_t;

/* Blender1 Color Key MIN Register */
typedef union {
	u32 val;
	struct {
	u32 ckbmin:8;   /**/
	u32 ckgmin:8;   /**/
	u32 ckrmin:8;   /**/
	u32 rsvd0:8;    /**/
	} bits;
} reg_dc_bld1ckmin_t;

/* Blender2 Color Key MIN Register */
typedef union {
	u32 val;
	struct {
	u32 ckbmin:8;   /**/
	u32 ckgmin:8;   /**/
	u32 ckrmin:8;   /**/
	u32 rsvd0:8;    /**/
	} bits;
} reg_dc_bld2ckmin_t;

/* Blender3 Color Key MIN Register */
typedef union {
	u32 val;
	struct {
	u32 ckbmin:8;   /**/
	u32 ckgmin:8;   /**/
	u32 ckrmin:8;   /**/
	u32 rsvd0:8;    /**/
	} bits;
} reg_dc_bld3ckmin_t;

/* Blender4 Color Key MIN Register */
typedef union {
	u32 val;
	struct {
	u32 ckbmin:8;   /**/
	u32 ckgmin:8;   /**/
	u32 ckrmin:8;   /**/
	u32 rsvd0:8;    /**/
	} bits;
} reg_dc_bld4ckmin_t;

/* Blender0 Color Key MAX Register */
typedef union {
	u32 val;
	struct {
	u32 ckbmax:8;   /**/
	u32 ckgmax:8;   /**/
	u32 ckrmax:8;   /**/
	u32 rsvd0:8;    /**/
	} bits;
} reg_dc_bld0ckmax_t;

/* Blender1 Color Key MAX Register */
typedef union {
	u32 val;
	struct {
	u32 ckbmax:8;   /**/
	u32 ckgmax:8;   /**/
	u32 ckrmax:8;   /**/
	u32 rsvd0:8;    /**/
	} bits;
} reg_dc_bld1ckmax_t;

/* Blender2 Color Key MAX Register */
typedef union {
	u32 val;
	struct {
	u32 ckbmax:8;   /**/
	u32 ckgmax:8;   /**/
	u32 ckrmax:8;   /**/
	u32 rsvd0:8;    /**/
	} bits;
} reg_dc_bld2ckmax_t;

/* Blender3 Color Key MAX Register */
typedef union {
	u32 val;
	struct {
	u32 ckbmax:8;   /**/
	u32 ckgmax:8;   /**/
	u32 ckrmax:8;   /**/
	u32 rsvd0:8;    /**/
	} bits;
} reg_dc_bld3ckmax_t;

/* Blender4 Color Key MAX Register */
typedef union {
	u32 val;
	struct {
	u32 ckbmax:8;   /**/
	u32 ckgmax:8;   /**/
	u32 ckrmax:8;   /**/
	u32 rsvd0:8;    /**/
	} bits;
} reg_dc_bld4ckmax_t;

/* Hardware Cursor Control Register */
typedef union {
	u32 val;
	struct {
	u32 w:2;        /**/
	u32 h:2;        /**/
	u32 rsvd0:2;    /**/
	u32 fmt:2;      /**/
	u32 rsvd1:8;    /**/
	u32 xoff:6;     /**/
	u32 rsvd2:2;    /**/
	u32 yoff:6;     /**/
	u32 rsvd3:2;    /**/
	} bits;
} reg_dc_hwcctl_t;

#define DC_HWCCTL_W_0           0x0
#define DC_HWCCTL_W_1           0x1
#define DC_HWCCTL_H_0           0x0
#define DC_HWCCTL_H_1           0x1
#define DC_HWCCTL_FMT_0         0x0
#define DC_HWCCTL_FMT_1         0x1
#define DC_HWCCTL_FMT_2         0x2
#define DC_HWCCTL_FMT_3         0x3

/* Hardware Cursor Coordinate Register */
typedef union {
	u32 val;
	struct {
	u32 x:16;/**/
	u32 y:16;/**/
	} bits;
} reg_dc_hwccoor_t;

/* Color Enhancement Enable Register */
typedef union {
	u32 val;
	struct {
	u32 en:1;       /**/
	u32 rsvd0:31;   /**/
	} bits;
} reg_dc_ceen_t;

#define DC_CEEN_EN_0    0x0
#define DC_CEEN_EN_1    0x1

/* Color Enhancement R Component Red Coefficient Register */
typedef union {
	u32 val;
	struct {
	u32 coef:14;    /**/
	u32 rsvd0:18;   /**/
	} bits;
} reg_dc_cerrcoef_t;

/* Color Enhancement R Component Green Coefficient Register */
typedef union {
	u32 val;
	struct {
	u32 coef:14;    /**/
	u32 rsvd0:18;   /**/
	} bits;
} reg_dc_cergcoef_t;

/* Color Enhancement R Component Blue Coefficient Register */
typedef union {
	u32 val;
	struct {
	u32 coef:14;    /**/
	u32 rsvd0:18;   /**/
	} bits;
} reg_dc_cerbcoef_t;

/* Color Enhancement R Component Constant Register */
typedef union {
	u32 val;
	struct {
	u32 cons:15;    /**/
	u32 rsvd0:17;   /**/
	} bits;
} reg_dc_cercons_t;

/* Color Enhancement G Component Red Coefficient Register */
typedef union {
	u32 val;
	struct {
	u32 coef:14;    /**/
	u32 rsvd0:18;   /**/
	} bits;
} reg_dc_cegrcoef_t;

/* Color Enhancement G Component Green Coefficient Register */
typedef union {
	u32 val;
	struct {
	u32 coef:14;    /**/
	u32 rsvd0:18;   /**/
	} bits;
} reg_dc_ceggcoef_t;

/* Color Enhancement G Component Blue Coefficient Register */
typedef union {
	u32 val;
	struct {
	u32 coef:14;    /**/
	u32 rsvd0:18;   /**/
	} bits;
} reg_dc_cegbcoef_t;

/* Color Enhancement G Component Constant Register */
typedef union {
	u32 val;
	struct {
	u32 cons:15;    /**/
	u32 rsvd0:17;   /**/
	} bits;
} reg_dc_cegcons_t;

/* Color Enhancement B Component Red Coefficient Register */
typedef union {
	u32 val;
	struct {
	u32 coef:14;    /**/
	u32 rsvd0:18;   /**/
	} bits;
} reg_dc_cebrcoef_t;

/* Color Enhancement B Component Green Coefficient Register */
typedef union {
	u32 val;
	struct {
	u32 coef:14;    /**/
	u32 rsvd0:18;   /**/
	} bits;
} reg_dc_cebgcoef_t;

/* Color Enhancement B Component Blue Coefficient Register */
typedef union {
	u32 val;
	struct {
	u32 coef:14;    /**/
	u32 rsvd0:18;   /**/
	} bits;
} reg_dc_cebbcoef_t;

/* Color Enhancement B Component Constant Register */
typedef union {
	u32 val;
	struct {
	u32 cons:15;    /**/
	u32 rsvd0:17;   /**/
	} bits;
} reg_dc_cebcons_t;

/* Block Linker General Setting Register */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:12;   /**/
	u32 fmt:3;      /**/
	u32 po:1;       /**/
	u32 alphactl:2; /**/
	u32 rsvd1:6;    /**/
	u32 plaalpha:8; /**/
	} bits;
} reg_dc_blkgne_t;

#define DC_BLKGNE_FMT_0         0x0
#define DC_BLKGNE_FMT_1         0x1
#define DC_BLKGNE_FMT_2         0x2
#define DC_BLKGNE_FMT_3         0x3
#define DC_BLKGNE_PO_0          0x0
#define DC_BLKGNE_PO_1          0x1
#define DC_BLKGNE_ALPHACTL_0    0x0
#define DC_BLKGNE_ALPHACTL_1    0x1
#define DC_BLKGNE_ALPHACTL_2    0x2
#define DC_BLKGNE_ALPHACTL_3    0x3

/* Write Back Control Register */
typedef union {
	u32 val;
	struct {
	u32 sta:1;      /**/
	u32 wbmod:1;    /**/
	u32 rsvd0:2;    /**/
	u32 fmt:2;      /**/
	u32 rsvd1:10;   /**/
	u32 wbstatus:1; /**/
	u32 wbexc:1;    /**/
	u32 rsvd2:14;   /**/
	} bits;
} reg_dc_wbc_t;

#define DC_WBC_STA_0            0x0
#define DC_WBC_STA_1            0x1
#define DC_WBC_WBMOD_0          0x0
#define DC_WBC_WBMOD_1          0x1
#define DC_WBC_FMT_0            0x0
#define DC_WBC_FMT_1            0x1
#define DC_WBC_FMT_2            0x2
#define DC_WBC_FMT_3            0x3
#define DC_WBC_WBSTATUS_0       0x0
#define DC_WBC_WBSTATUS_1       0x1
#define DC_WBC_WBEXC_0          0x0
#define DC_WBC_WBEXC_1          0x1

/* Write Back Address Register */
typedef union {
	u32 val;
	struct {
	u32 add:32;/**/
	} bits;
} reg_dc_wbadd_t;

/* Write Back Line Stride Register */
typedef union {
	u32 val;
	struct {
	u32 std:32;/**/
	} bits;
} reg_dc_wblnstd_t;

/* Block Linker Control Registers Array */
typedef union {
	u32 val;
	struct {
	u32 w:12;       /**/
	u32 h:12;       /**/
	u32 np:6;       /**/
	u32 rsvd0:2;    /**/
	} bits;
} reg_dc_ch0_blkctl_t;

/* Block Linker Control Registers Array */
typedef union {
	u32 val;
	struct {
	u32 w:12;       /**/
	u32 h:12;       /**/
	u32 np:6;       /**/
	u32 rsvd0:2;    /**/
	} bits;
} reg_dc_ch1_blkctl_t;

/* Block Linker Control Registers Array */
typedef union {
	u32 val;
	struct {
	u32 w:12;       /**/
	u32 h:12;       /**/
	u32 np:6;       /**/
	u32 rsvd0:2;    /**/
	} bits;
} reg_dc_ch2_blkctl_t;

/* Block Linker Control Registers Array */
typedef union {
	u32 val;
	struct {
	u32 w:12;       /**/
	u32 h:12;       /**/
	u32 np:6;       /**/
	u32 rsvd0:2;    /**/
	} bits;
} reg_dc_ch3_blkctl_t;

/* Block Linker Control Registers Array */
typedef union {
	u32 val;
	struct {
	u32 w:12;       /**/
	u32 h:12;       /**/
	u32 np:6;       /**/
	u32 rsvd0:2;    /**/
	} bits;
} reg_dc_ch4_blkctl_t;

/* Block Linker Control Registers Array */
typedef union {
	u32 val;
	struct {
	u32 w:12;       /**/
	u32 h:12;       /**/
	u32 np:6;       /**/
	u32 rsvd0:2;    /**/
	} bits;
} reg_dc_ch5_blkctl_t;

/* Block Linker Control Registers Array */
typedef union {
	u32 val;
	struct {
	u32 w:12;       /**/
	u32 h:12;       /**/
	u32 np:6;       /**/
	u32 rsvd0:2;    /**/
	} bits;
} reg_dc_ch6_blkctl_t;

/* Block Linker Control Registers Array */
typedef union {
	u32 val;
	struct {
	u32 w:12;       /**/
	u32 h:12;       /**/
	u32 np:6;       /**/
	u32 rsvd0:2;    /**/
	} bits;
} reg_dc_ch7_blkctl_t;

/* Block Linker Control Registers Array */
typedef union {
	u32 val;
	struct {
	u32 w:12;       /**/
	u32 h:12;       /**/
	u32 np:6;       /**/
	u32 rsvd0:2;    /**/
	} bits;
} reg_dc_ch8_blkctl_t;

/* Block Linker Control Registers Array */
typedef union {
	u32 val;
	struct {
	u32 w:12;       /**/
	u32 h:12;       /**/
	u32 np:6;       /**/
	u32 rsvd0:2;    /**/
	} bits;
} reg_dc_ch9_blkctl_t;

/* Block Linker Control Registers Array */
typedef union {
	u32 val;
	struct {
	u32 w:12;       /**/
	u32 h:12;       /**/
	u32 np:6;       /**/
	u32 rsvd0:2;    /**/
	} bits;
} reg_dc_ch10_blkctl_t;

/* Block Linker Control Registers Array */
typedef union {
	u32 val;
	struct {
	u32 w:12;       /**/
	u32 h:12;       /**/
	u32 np:6;       /**/
	u32 rsvd0:2;    /**/
	} bits;
} reg_dc_ch11_blkctl_t;

/* Block Linker Control Registers Array */
typedef union {
	u32 val;
	struct {
	u32 w:12;       /**/
	u32 h:12;       /**/
	u32 np:6;       /**/
	u32 rsvd0:2;    /**/
	} bits;
} reg_dc_ch12_blkctl_t;

/* Block Linker Control Registers Array */
typedef union {
	u32 val;
	struct {
	u32 w:12;       /**/
	u32 h:12;       /**/
	u32 np:6;       /**/
	u32 rsvd0:2;    /**/
	} bits;
} reg_dc_ch13_blkctl_t;

/* Block Linker Control Registers Array */
typedef union {
	u32 val;
	struct {
	u32 w:12;       /**/
	u32 h:12;       /**/
	u32 np:6;       /**/
	u32 rsvd0:2;    /**/
	} bits;
} reg_dc_ch14_blkctl_t;

/* Block Linker Control Registers Array */
typedef union {
	u32 val;
	struct {
	u32 w:12;       /**/
	u32 h:12;       /**/
	u32 np:6;       /**/
	u32 rsvd0:2;    /**/
	} bits;
} reg_dc_ch15_blkctl_t;

/* Block Linker Coordinate Registers Array */
typedef union {
	u32 val;
	struct {
	u32 x:16;/**/
	u32 y:16;/**/
	} bits;
} reg_dc_ch0_blkcoor_t;

/* Block Linker Coordinate Registers Array */
typedef union {
	u32 val;
	struct {
	u32 x:16;/**/
	u32 y:16;/**/
	} bits;
} reg_dc_ch1_blkcoor_t;

/* Block Linker Coordinate Registers Array */
typedef union {
	u32 val;
	struct {
	u32 x:16;/**/
	u32 y:16;/**/
	} bits;
} reg_dc_ch2_blkcoor_t;

/* Block Linker Coordinate Registers Array */
typedef union {
	u32 val;
	struct {
	u32 x:16;/**/
	u32 y:16;/**/
	} bits;
} reg_dc_ch3_blkcoor_t;

/* Block Linker Coordinate Registers Array */
typedef union {
	u32 val;
	struct {
	u32 x:16;/**/
	u32 y:16;/**/
	} bits;
} reg_dc_ch4_blkcoor_t;

/* Block Linker Coordinate Registers Array */
typedef union {
	u32 val;
	struct {
	u32 x:16;/**/
	u32 y:16;/**/
	} bits;
} reg_dc_ch5_blkcoor_t;

/* Block Linker Coordinate Registers Array */
typedef union {
	u32 val;
	struct {
	u32 x:16;/**/
	u32 y:16;/**/
	} bits;
} reg_dc_ch6_blkcoor_t;

/* Block Linker Coordinate Registers Array */
typedef union {
	u32 val;
	struct {
	u32 x:16;/**/
	u32 y:16;/**/
	} bits;
} reg_dc_ch7_blkcoor_t;

/* Block Linker Coordinate Registers Array */
typedef union {
	u32 val;
	struct {
	u32 x:16;/**/
	u32 y:16;/**/
	} bits;
} reg_dc_ch8_blkcoor_t;

/* Block Linker Coordinate Registers Array */
typedef union {
	u32 val;
	struct {
	u32 x:16;/**/
	u32 y:16;/**/
	} bits;
} reg_dc_ch9_blkcoor_t;

/* Block Linker Coordinate Registers Array */
typedef union {
	u32 val;
	struct {
	u32 x:16;/**/
	u32 y:16;/**/
	} bits;
} reg_dc_ch10_blkcoor_t;

/* Block Linker Coordinate Registers Array */
typedef union {
	u32 val;
	struct {
	u32 x:16;/**/
	u32 y:16;/**/
	} bits;
} reg_dc_ch11_blkcoor_t;

/* Block Linker Coordinate Registers Array */
typedef union {
	u32 val;
	struct {
	u32 x:16;/**/
	u32 y:16;/**/
	} bits;
} reg_dc_ch12_blkcoor_t;

/* Block Linker Coordinate Registers Array */
typedef union {
	u32 val;
	struct {
	u32 x:16;/**/
	u32 y:16;/**/
	} bits;
} reg_dc_ch13_blkcoor_t;

/* Block Linker Coordinate Registers Array */
typedef union {
	u32 val;
	struct {
	u32 x:16;/**/
	u32 y:16;/**/
	} bits;
} reg_dc_ch14_blkcoor_t;

/* Block Linker Coordinate Registers Array */
typedef union {
	u32 val;
	struct {
	u32 x:16;/**/
	u32 y:16;/**/
	} bits;
} reg_dc_ch15_blkcoor_t;

/* Block Linker Buffer Address Registers Array */
typedef union {
	u32 val;
	struct {
	u32 add:32;/**/
	} bits;
} reg_dc_ch0_blkbufadd_t;

/* Block Linker Buffer Address Registers Array */
typedef union {
	u32 val;
	struct {
	u32 add:32;/**/
	} bits;
} reg_dc_ch1_blkbufadd_t;

/* Block Linker Buffer Address Registers Array */
typedef union {
	u32 val;
	struct {
	u32 add:32;/**/
	} bits;
} reg_dc_ch2_blkbufadd_t;

/* Block Linker Buffer Address Registers Array */
typedef union {
	u32 val;
	struct {
	u32 add:32;/**/
	} bits;
} reg_dc_ch3_blkbufadd_t;

/* Block Linker Buffer Address Registers Array */
typedef union {
	u32 val;
	struct {
	u32 add:32;/**/
	} bits;
} reg_dc_ch4_blkbufadd_t;

/* Block Linker Buffer Address Registers Array */
typedef union {
	u32 val;
	struct {
	u32 add:32;/**/
	} bits;
} reg_dc_ch5_blkbufadd_t;

/* Block Linker Buffer Address Registers Array */
typedef union {
	u32 val;
	struct {
	u32 add:32;/**/
	} bits;
} reg_dc_ch6_blkbufadd_t;

/* Block Linker Buffer Address Registers Array */
typedef union {
	u32 val;
	struct {
	u32 add:32;/**/
	} bits;
} reg_dc_ch7_blkbufadd_t;

/* Block Linker Buffer Address Registers Array */
typedef union {
	u32 val;
	struct {
	u32 add:32;/**/
	} bits;
} reg_dc_ch8_blkbufadd_t;

/* Block Linker Buffer Address Registers Array */
typedef union {
	u32 val;
	struct {
	u32 add:32;/**/
	} bits;
} reg_dc_ch9_blkbufadd_t;

/* Block Linker Buffer Address Registers Array */
typedef union {
	u32 val;
	struct {
	u32 add:32;/**/
	} bits;
} reg_dc_ch10_blkbufadd_t;

/* Block Linker Buffer Address Registers Array */
typedef union {
	u32 val;
	struct {
	u32 add:32;/**/
	} bits;
} reg_dc_ch11_blkbufadd_t;

/* Block Linker Buffer Address Registers Array */
typedef union {
	u32 val;
	struct {
	u32 add:32;/**/
	} bits;
} reg_dc_ch12_blkbufadd_t;

/* Block Linker Buffer Address Registers Array */
typedef union {
	u32 val;
	struct {
	u32 add:32;/**/
	} bits;
} reg_dc_ch13_blkbufadd_t;

/* Block Linker Buffer Address Registers Array */
typedef union {
	u32 val;
	struct {
	u32 add:32;/**/
	} bits;
} reg_dc_ch14_blkbufadd_t;

/* Block Linker Buffer Address Registers Array */
typedef union {
	u32 val;
	struct {
	u32 add:32;/**/
	} bits;
} reg_dc_ch15_blkbufadd_t;

/* Block Linker Buffer Line Stride Registers Array */
typedef union {
	u32 val;
	struct {
	u32 std:32;/**/
	} bits;
} reg_dc_ch0_blkbuflnstd_t;

/* Block Linker Buffer Line Stride Registers Array */
typedef union {
	u32 val;
	struct {
	u32 std:32;/**/
	} bits;
} reg_dc_ch1_blkbuflnstd_t;

/* Block Linker Buffer Line Stride Registers Array */
typedef union {
	u32 val;
	struct {
	u32 std:32;/**/
	} bits;
} reg_dc_ch2_blkbuflnstd_t;

/* Block Linker Buffer Line Stride Registers Array */
typedef union {
	u32 val;
	struct {
	u32 std:32;/**/
	} bits;
} reg_dc_ch3_blkbuflnstd_t;

/* Block Linker Buffer Line Stride Registers Array */
typedef union {
	u32 val;
	struct {
	u32 std:32;/**/
	} bits;
} reg_dc_ch4_blkbuflnstd_t;

/* Block Linker Buffer Line Stride Registers Array */
typedef union {
	u32 val;
	struct {
	u32 std:32;/**/
	} bits;
} reg_dc_ch5_blkbuflnstd_t;

/* Block Linker Buffer Line Stride Registers Array */
typedef union {
	u32 val;
	struct {
	u32 std:32;/**/
	} bits;
} reg_dc_ch6_blkbuflnstd_t;

/* Block Linker Buffer Line Stride Registers Array */
typedef union {
	u32 val;
	struct {
	u32 std:32;/**/
	} bits;
} reg_dc_ch7_blkbuflnstd_t;

/* Block Linker Buffer Line Stride Registers Array */
typedef union {
	u32 val;
	struct {
	u32 std:32;/**/
	} bits;
} reg_dc_ch8_blkbuflnstd_t;

/* Block Linker Buffer Line Stride Registers Array */
typedef union {
	u32 val;
	struct {
	u32 std:32;/**/
	} bits;
} reg_dc_ch9_blkbuflnstd_t;

/* Block Linker Buffer Line Stride Registers Array */
typedef union {
	u32 val;
	struct {
	u32 std:32;/**/
	} bits;
} reg_dc_ch10_blkbuflnstd_t;

/* Block Linker Buffer Line Stride Registers Array */
typedef union {
	u32 val;
	struct {
	u32 std:32;/**/
	} bits;
} reg_dc_ch11_blkbuflnstd_t;

/* Block Linker Buffer Line Stride Registers Array */
typedef union {
	u32 val;
	struct {
	u32 std:32;/**/
	} bits;
} reg_dc_ch12_blkbuflnstd_t;

/* Block Linker Buffer Line Stride Registers Array */
typedef union {
	u32 val;
	struct {
	u32 std:32;/**/
	} bits;
} reg_dc_ch13_blkbuflnstd_t;

/* Block Linker Buffer Line Stride Registers Array */
typedef union {
	u32 val;
	struct {
	u32 std:32;/**/
	} bits;
} reg_dc_ch14_blkbuflnstd_t;

/* Block Linker Buffer Line Stride Registers Array */
typedef union {
	u32 val;
	struct {
	u32 std:32;/**/
	} bits;
} reg_dc_ch15_blkbuflnstd_t;

/* reg_dc_t bank */
typedef struct tag_dc {
	reg_dc_gnectl_t            gnectl;             /* 0000 */
	reg_dc_imgsrcctl_t         imgsrcctl;          /* 0004 */
	reg_dc_scrsiz_t            scrsiz;             /* 0008 */
	reg_dc_bkcolor_t           bkcolor;            /* 000C */
	reg_dc_ver_t               ver;                /* 0010 */
	reg_dc_ftr_t               ftr;                /* 0014 */
	u32                        res0[2];
	reg_dc_win0ctl_t           win0ctl;            /* 0020 */
	reg_dc_win1ctl_t           win1ctl;            /* 0024 */
	reg_dc_win2ctl_t           win2ctl;            /* 0028 */
	reg_dc_win3ctl_t           win3ctl;            /* 002C */
	reg_dc_win0buffmt_t        win0buffmt;         /* 0030 */
	reg_dc_win1buffmt_t        win1buffmt;         /* 0034 */
	reg_dc_win2buffmt_t        win2buffmt;         /* 0038 */
	reg_dc_win3buffmt_t        win3buffmt;         /* 003C */
	reg_dc_win0siz_t           win0siz;            /* 0040 */
	reg_dc_win1siz_t           win1siz;            /* 0044 */
	reg_dc_win2siz_t           win2siz;            /* 0048 */
	reg_dc_win3siz_t           win3siz;            /* 004C */
	reg_dc_win0coor_t          win0coor;           /* 0050 */
	reg_dc_win1coor_t          win1coor;           /* 0054 */
	reg_dc_win2coor_t          win2coor;           /* 0058 */
	reg_dc_win3coor_t          win3coor;           /* 005C */
	reg_dc_win0bufladd_t       win0bufladd;        /* 0060 */
	reg_dc_win1bufladd_t       win1bufladd;        /* 0064 */
	reg_dc_win2bufladd_t       win2bufladd;        /* 0068 */
	reg_dc_win3bufladd_t       win3bufladd;        /* 006C */
	reg_dc_winbufhadd_t        winbufhadd;         /* 0070 */
	u32                        res1[3];
	reg_dc_win0buflnstd_t      win0buflnstd;       /* 0080 */
	reg_dc_win1buflnstd_t      win1buflnstd;       /* 0084 */
	reg_dc_win2buflnstd_t      win2buflnstd;       /* 0088 */
	reg_dc_win3buflnstd_t      win3buflnstd;       /* 008C */
	reg_dc_bld0ctl_t           bld0ctl;            /* 0090 */
	reg_dc_bld1ctl_t           bld1ctl;            /* 0094 */
	reg_dc_bld2ctl_t           bld2ctl;            /* 0098 */
	reg_dc_bld3ctl_t           bld3ctl;            /* 009C */
	reg_dc_bld4ctl_t           bld4ctl;            /* 00A0 */
	u32                        res2[3];
	reg_dc_bld0ckmin_t         bld0ckmin;          /* 00B0 */
	reg_dc_bld1ckmin_t         bld1ckmin;          /* 00B4 */
	reg_dc_bld2ckmin_t         bld2ckmin;          /* 00B8 */
	reg_dc_bld3ckmin_t         bld3ckmin;          /* 00BC */
	reg_dc_bld4ckmin_t         bld4ckmin;          /* 00C0 */
	u32                        res3[3];
	reg_dc_bld0ckmax_t         bld0ckmax;          /* 00D0 */
	reg_dc_bld1ckmax_t         bld1ckmax;          /* 00D4 */
	reg_dc_bld2ckmax_t         bld2ckmax;          /* 00D8 */
	reg_dc_bld3ckmax_t         bld3ckmax;          /* 00DC */
	reg_dc_bld4ckmax_t         bld4ckmax;          /* 00E0 */
	u32                        res4[3];
	reg_dc_hwcctl_t            hwcctl;             /* 00F0 */
	reg_dc_hwccoor_t           hwccoor;            /* 00F4 */
	u32                        res5[2];
	reg_dc_ceen_t              ceen;               /* 0100 */
	u32                        res6[3];
	reg_dc_cerrcoef_t          cerrcoef;           /* 0110 */
	reg_dc_cergcoef_t          cergcoef;           /* 0114 */
	reg_dc_cerbcoef_t          cerbcoef;           /* 0118 */
	reg_dc_cercons_t           cercons;            /* 011C */
	reg_dc_cegrcoef_t          cegrcoef;           /* 0120 */
	reg_dc_ceggcoef_t          ceggcoef;           /* 0124 */
	reg_dc_cegbcoef_t          cegbcoef;           /* 0128 */
	reg_dc_cegcons_t           cegcons;            /* 012C */
	reg_dc_cebrcoef_t          cebrcoef;           /* 0130 */
	reg_dc_cebgcoef_t          cebgcoef;           /* 0134 */
	reg_dc_cebbcoef_t          cebbcoef;           /* 0138 */
	reg_dc_cebcons_t           cebcons;            /* 013C */
	u32                        res7[40];
	reg_dc_blkgne_t            blkgne;             /* 01E0 */
	u32                        res8[3];
	reg_dc_wbc_t               wbc;                /* 01F0 */
	reg_dc_wbadd_t             wbadd;              /* 01F4 */
	reg_dc_wblnstd_t           wblnstd;            /* 01F8 */
	u32                        res9[1];
	reg_dc_ch0_blkctl_t        ch0_blkctl;         /* 0200 */
	reg_dc_ch1_blkctl_t        ch1_blkctl;         /* 0204 */
	reg_dc_ch2_blkctl_t        ch2_blkctl;         /* 0208 */
	reg_dc_ch3_blkctl_t        ch3_blkctl;         /* 020C */
	reg_dc_ch4_blkctl_t        ch4_blkctl;         /* 0210 */
	reg_dc_ch5_blkctl_t        ch5_blkctl;         /* 0214 */
	reg_dc_ch6_blkctl_t        ch6_blkctl;         /* 0218 */
	reg_dc_ch7_blkctl_t        ch7_blkctl;         /* 021C */
	reg_dc_ch8_blkctl_t        ch8_blkctl;         /* 0220 */
	reg_dc_ch9_blkctl_t        ch9_blkctl;         /* 0224 */
	reg_dc_ch10_blkctl_t       ch10_blkctl;        /* 0228 */
	reg_dc_ch11_blkctl_t       ch11_blkctl;        /* 022C */
	reg_dc_ch12_blkctl_t       ch12_blkctl;        /* 0230 */
	reg_dc_ch13_blkctl_t       ch13_blkctl;        /* 0234 */
	reg_dc_ch14_blkctl_t       ch14_blkctl;        /* 0238 */
	reg_dc_ch15_blkctl_t       ch15_blkctl;        /* 023C */
	u32                        res10[48];
	reg_dc_ch0_blkcoor_t       ch0_blkcoor;        /* 0300 */
	reg_dc_ch1_blkcoor_t       ch1_blkcoor;        /* 0304 */
	reg_dc_ch2_blkcoor_t       ch2_blkcoor;        /* 0308 */
	reg_dc_ch3_blkcoor_t       ch3_blkcoor;        /* 030C */
	reg_dc_ch4_blkcoor_t       ch4_blkcoor;        /* 0310 */
	reg_dc_ch5_blkcoor_t       ch5_blkcoor;        /* 0314 */
	reg_dc_ch6_blkcoor_t       ch6_blkcoor;        /* 0318 */
	reg_dc_ch7_blkcoor_t       ch7_blkcoor;        /* 031C */
	reg_dc_ch8_blkcoor_t       ch8_blkcoor;        /* 0320 */
	reg_dc_ch9_blkcoor_t       ch9_blkcoor;        /* 0324 */
	reg_dc_ch10_blkcoor_t      ch10_blkcoor;       /* 0328 */
	reg_dc_ch11_blkcoor_t      ch11_blkcoor;       /* 032C */
	reg_dc_ch12_blkcoor_t      ch12_blkcoor;       /* 0330 */
	reg_dc_ch13_blkcoor_t      ch13_blkcoor;       /* 0334 */
	reg_dc_ch14_blkcoor_t      ch14_blkcoor;       /* 0338 */
	reg_dc_ch15_blkcoor_t      ch15_blkcoor;       /* 033C */
	u32                        res11[48];
	reg_dc_ch0_blkbufadd_t     ch0_blkbufadd;      /* 0400 */
	reg_dc_ch1_blkbufadd_t     ch1_blkbufadd;      /* 0404 */
	reg_dc_ch2_blkbufadd_t     ch2_blkbufadd;      /* 0408 */
	reg_dc_ch3_blkbufadd_t     ch3_blkbufadd;      /* 040C */
	reg_dc_ch4_blkbufadd_t     ch4_blkbufadd;      /* 0410 */
	reg_dc_ch5_blkbufadd_t     ch5_blkbufadd;      /* 0414 */
	reg_dc_ch6_blkbufadd_t     ch6_blkbufadd;      /* 0418 */
	reg_dc_ch7_blkbufadd_t     ch7_blkbufadd;      /* 041C */
	reg_dc_ch8_blkbufadd_t     ch8_blkbufadd;      /* 0420 */
	reg_dc_ch9_blkbufadd_t     ch9_blkbufadd;      /* 0424 */
	reg_dc_ch10_blkbufadd_t    ch10_blkbufadd;     /* 0428 */
	reg_dc_ch11_blkbufadd_t    ch11_blkbufadd;     /* 042C */
	reg_dc_ch12_blkbufadd_t    ch12_blkbufadd;     /* 0430 */
	reg_dc_ch13_blkbufadd_t    ch13_blkbufadd;     /* 0434 */
	reg_dc_ch14_blkbufadd_t    ch14_blkbufadd;     /* 0438 */
	reg_dc_ch15_blkbufadd_t    ch15_blkbufadd;     /* 043C */
	u32                        res12[48];
	reg_dc_ch0_blkbuflnstd_t   ch0_blkbuflnstd;    /* 0500 */
	reg_dc_ch1_blkbuflnstd_t   ch1_blkbuflnstd;    /* 0504 */
	reg_dc_ch2_blkbuflnstd_t   ch2_blkbuflnstd;    /* 0508 */
	reg_dc_ch3_blkbuflnstd_t   ch3_blkbuflnstd;    /* 050C */
	reg_dc_ch4_blkbuflnstd_t   ch4_blkbuflnstd;    /* 0510 */
	reg_dc_ch5_blkbuflnstd_t   ch5_blkbuflnstd;    /* 0514 */
	reg_dc_ch6_blkbuflnstd_t   ch6_blkbuflnstd;    /* 0518 */
	reg_dc_ch7_blkbuflnstd_t   ch7_blkbuflnstd;    /* 051C */
	reg_dc_ch8_blkbuflnstd_t   ch8_blkbuflnstd;    /* 0520 */
	reg_dc_ch9_blkbuflnstd_t   ch9_blkbuflnstd;    /* 0524 */
	reg_dc_ch10_blkbuflnstd_t  ch10_blkbuflnstd;   /* 0528 */
	reg_dc_ch11_blkbuflnstd_t  ch11_blkbuflnstd;   /* 052C */
	reg_dc_ch12_blkbuflnstd_t  ch12_blkbuflnstd;   /* 0530 */
	reg_dc_ch13_blkbuflnstd_t  ch13_blkbuflnstd;   /* 0534 */
	reg_dc_ch14_blkbuflnstd_t  ch14_blkbuflnstd;   /* 0538 */
	reg_dc_ch15_blkbuflnstd_t  ch15_blkbuflnstd;   /* 053C */
} reg_dc_t;

#endif /* ___DC___H___ */
