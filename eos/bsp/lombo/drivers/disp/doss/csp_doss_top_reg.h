/* doss.h */

#ifndef ___DOSS___H___
#define ___DOSS___H___

#define BASE_DOSS                0x01600000

#define VA_DOSS_VER                          (0x00000000 + BASE_DOSS + VA_DOSS)
#define VA_DOSS_DO0_FTR                      (0x00000010 + BASE_DOSS + VA_DOSS)
#define VA_DOSS_DO1_FTR                      (0x00000014 + BASE_DOSS + VA_DOSS)
#define VA_DOSS_RESET                        (0x00000020 + BASE_DOSS + VA_DOSS)
#define VA_DOSS_ROUTE                        (0x00000024 + BASE_DOSS + VA_DOSS)
#define VA_DOSS_TCON0_CTRL              (0x00000100 + BASE_DOSS + VA_DOSS)
#define VA_DOSS_TCON1_CTRL              (0x00000120 + BASE_DOSS + VA_DOSS)
#define VA_DOSS_TCON0_CLK               (0x00000104 + BASE_DOSS + VA_DOSS)
#define VA_DOSS_TCON1_CLK               (0x00000124 + BASE_DOSS + VA_DOSS)
#define VA_DOSS_LVDS0_CTRL              (0x00000140 + BASE_DOSS + VA_DOSS)
#define VA_DOSS_LVDS1_CTRL              (0x00000160 + BASE_DOSS + VA_DOSS)
#define VA_DOSS_LVDS0_CLK               (0x00000144 + BASE_DOSS + VA_DOSS)
#define VA_DOSS_LVDS1_CLK               (0x00000164 + BASE_DOSS + VA_DOSS)
#define VA_DOSS_MIPI_DSI0_CTRL          (0x00000180 + BASE_DOSS + VA_DOSS)
#define VA_DOSS_MIPI_DSI1_CTRL          (0x000001A0 + BASE_DOSS + VA_DOSS)
#define VA_DOSS_MIPI_DSI0_REF_CLK       (0x00000184 + BASE_DOSS + VA_DOSS)
#define VA_DOSS_MIPI_DSI1_REF_CLK       (0x000001A4 + BASE_DOSS + VA_DOSS)
#define VA_DOSS_MIPI_DSI0_CFG_CLK       (0x00000188 + BASE_DOSS + VA_DOSS)
#define VA_DOSS_MIPI_DSI1_CFG_CLK       (0x000001A8 + BASE_DOSS + VA_DOSS)
#define VA_DOSS_TVE0_CTRL               (0x00000400 + BASE_DOSS + VA_DOSS)
#define VA_DOSS_TVE1_CTRL               (0x00000420 + BASE_DOSS + VA_DOSS)
#define VA_DOSS_TVE0_CLK                (0x00000404 + BASE_DOSS + VA_DOSS)
#define VA_DOSS_TVE1_CLK                (0x00000424 + BASE_DOSS + VA_DOSS)

#define DATA_DOSS_VER                        0x00001100
#define DATA_DOSS_DO0_FTR                    0x80000107
#define DATA_DOSS_DO1_FTR                    0x00000000
#define DATA_DOSS_RESET                      0x00000000
#define DATA_DOSS_ROUTE                      0x00000000
#define DATA_DOSS_TCON0_CTRL            0x00000000
#define DATA_DOSS_TCON1_CTRL            0x00000000
#define DATA_DOSS_TCON0_CLK             0x00000000
#define DATA_DOSS_TCON1_CLK             0x00000000
#define DATA_DOSS_LVDS0_CTRL            0x00000000
#define DATA_DOSS_LVDS1_CTRL            0x00000000
#define DATA_DOSS_LVDS0_CLK             0x00000000
#define DATA_DOSS_LVDS1_CLK             0x00000000
#define DATA_DOSS_MIPI_DSI0_CTRL        0x00000000
#define DATA_DOSS_MIPI_DSI1_CTRL        0x00000000
#define DATA_DOSS_MIPI_DSI0_REF_CLK     0x00000000
#define DATA_DOSS_MIPI_DSI1_REF_CLK     0x00000000
#define DATA_DOSS_MIPI_DSI0_CFG_CLK     0x00000000
#define DATA_DOSS_MIPI_DSI1_CFG_CLK     0x00000000
#define DATA_DOSS_TVE0_CTRL             0x00000000
#define DATA_DOSS_TVE1_CTRL             0x00000000
#define DATA_DOSS_TVE0_CLK              0x00000000
#define DATA_DOSS_TVE1_CLK              0x00000000

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
} reg_doss_ver_t;

/* Display Output 0 Feature Register */
typedef union {
	u32 val;
	struct {
	u32 rgb:1;      /**/
	u32 lvds:1;     /**/
	u32 dsi:1;      /**/
	u32 edp:1;      /**/
	u32 rsvd0:4;    /**/
	u32 tve:1;      /**/
	u32 hdmi:1;     /**/
	u32 rsvd1:6;    /**/
	u32 lvds_dual:1;/**/
	u32 dsi_dual:1; /**/
	u32 rsvd2:13;   /**/
	u32 tcon:1;     /**/
	} bits;
} reg_doss_do0_ftr_t;

/* Display Output 1 Feature Register */
typedef union {
	u32 val;
	struct {
	u32 rgb:1;      /**/
	u32 lvds:1;     /**/
	u32 dsi:1;      /**/
	u32 edp:1;      /**/
	u32 rsvd0:4;    /**/
	u32 tve:1;      /**/
	u32 hdmi:1;     /**/
	u32 rsvd1:6;    /**/
	u32 lvds_dual:1;/**/
	u32 dsi_dual:1; /**/
	u32 rsvd2:13;   /**/
	u32 tcon:1;     /**/
	} bits;
} reg_doss_do1_ftr_t;

/* Reset Register */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:31;   /**/
	u32 rst:1;      /**/
	} bits;
} reg_doss_reset_t;

#define DOSS_RESET_RST_0        0x0
#define DOSS_RESET_RST_1        0x1

/* Route Register */
typedef union {
	u32 val;
	struct {
	u32 src_do0:2;  /**/
	u32 rsvd0:2;    /**/
	u32 src_do1:2;  /**/
	u32 rsvd1:26;   /**/
	} bits;
} reg_doss_route_t;

#define DOSS_ROUTE_SRC_DO0_0    0x0
#define DOSS_ROUTE_SRC_DO0_1    0x1
#define DOSS_ROUTE_SRC_DO1_0    0x0
#define DOSS_ROUTE_SRC_DO1_1    0x1

/* TCON[CH] Control Register */
typedef union {
	u32 val;
	struct {
	u32 slave:1;    /**/
	u32 rsvd0:26;   /**/
	u32 gate:1;     /**/
	u32 rsvd1:3;    /**/
	u32 rst:1;      /**/
	} bits;
} reg_doss_tcon0_ctrl_t;

#define DOSS_TCON0_CTRL_RST_0   0x0
#define DOSS_TCON0_CTRL_RST_1   0x1

/* TCON[CH] Control Register */
typedef union {
	u32 val;
	struct {
	u32 slave:1;    /**/
	u32 rsvd0:26;   /**/
	u32 gate:1;     /**/
	u32 rsvd1:3;    /**/
	u32 rst:1;      /**/
	} bits;
} reg_doss_tcon1_ctrl_t;

#define DOSS_TCON1_CTRL_RST_0   0x0
#define DOSS_TCON1_CTRL_RST_1   0x1

/* TCON[CH] Clock Register */
typedef union {
	u32 val;
	struct {
	u32 div0:4;     /**/
	u32 rsvd0:4;    /**/
	u32 div1:1;     /**/
	u32 rsvd1:7;    /**/
	u32 rsvd2:4;    /**/
	u32 rsvd3:11;   /**/
	u32 gate:1;     /**/
	} bits;
} reg_doss_tcon0_clk_t;

#define DOSS_TCON0_CLK_GATE_0   0x0
#define DOSS_TCON0_CLK_GATE_1   0x1

/* TCON[CH] Clock Register */
typedef union {
	u32 val;
	struct {
	u32 div0:4;     /**/
	u32 rsvd0:4;    /**/
	u32 div1:1;     /**/
	u32 rsvd1:7;    /**/
	u32 rsvd2:4;    /**/
	u32 rsvd3:11;   /**/
	u32 gate:1;     /**/
	} bits;
} reg_doss_tcon1_clk_t;

#define DOSS_TCON1_CLK_GATE_0   0x0
#define DOSS_TCON1_CLK_GATE_1   0x1

/* LVDS[CH] Control Register */
typedef union {
	u32 val;
	struct {
	u32 slave:1;    /**/
	u32 rsvd0:26;   /**/
	u32 gate:1;     /**/
	u32 rsvd1:3;    /**/
	u32 rst:1;      /**/
	} bits;
} reg_doss_lvds0_ctrl_t;

#define DOSS_LVDS0_CTRL_SLAVE_0 0x0
#define DOSS_LVDS0_CTRL_SLAVE_1 0x1
#define DOSS_LVDS0_CTRL_GATE_0  0x0
#define DOSS_LVDS0_CTRL_GATE_1  0x1
#define DOSS_LVDS0_CTRL_RST_0   0x0
#define DOSS_LVDS0_CTRL_RST_1   0x1

/* LVDS[CH] Control Register */
typedef union {
	u32 val;
	struct {
	u32 slave:1;    /**/
	u32 rsvd0:26;   /**/
	u32 gate:1;     /**/
	u32 rsvd1:3;    /**/
	u32 rst:1;      /**/
	} bits;
} reg_doss_lvds1_ctrl_t;

#define DOSS_LVDS1_CTRL_SLAVE_0 0x0
#define DOSS_LVDS1_CTRL_SLAVE_1 0x1
#define DOSS_LVDS1_CTRL_GATE_0  0x0
#define DOSS_LVDS1_CTRL_GATE_1  0x1
#define DOSS_LVDS1_CTRL_RST_0   0x0
#define DOSS_LVDS1_CTRL_RST_1   0x1

/* LVDS[CH] Clock Register */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:4;    /**/
	u32 rsvd1:4;    /**/
	u32 rsvd2:8;    /**/
	u32 rsvd3:4;    /**/
	u32 rsvd4:11;   /**/
	u32 gate:1;     /**/
	} bits;
} reg_doss_lvds0_clk_t;

#define DOSS_LVDS0_CLK_GATE_0   0x0
#define DOSS_LVDS0_CLK_GATE_1   0x1

/* LVDS[CH] Clock Register */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:4;    /**/
	u32 rsvd1:4;    /**/
	u32 rsvd2:8;    /**/
	u32 rsvd3:4;    /**/
	u32 rsvd4:11;   /**/
	u32 gate:1;     /**/
	} bits;
} reg_doss_lvds1_clk_t;

#define DOSS_LVDS1_CLK_GATE_0   0x0
#define DOSS_LVDS1_CLK_GATE_1   0x1

/* MIPI DSI[CH] Control Register */
typedef union {
	u32 val;
	struct {
	u32 slave:1;    /**/
	u32 rsvd0:26;   /**/
	u32 gate:1;     /**/
	u32 rsvd1:3;    /**/
	u32 rst:1;      /**/
	} bits;
} reg_doss_mipi_dsi0_ctrl_t;

#define DOSS_MIPI_DSI0_CTRL_SLAVE_0     0x0
#define DOSS_MIPI_DSI0_CTRL_SLAVE_1     0x1
#define DOSS_MIPI_DSI0_CTRL_GATE_0      0x0
#define DOSS_MIPI_DSI0_CTRL_GATE_1      0x1
#define DOSS_MIPI_DSI0_CTRL_RST_0       0x0
#define DOSS_MIPI_DSI0_CTRL_RST_1       0x1

/* MIPI DSI[CH] Control Register */
typedef union {
	u32 val;
	struct {
	u32 slave:1;    /**/
	u32 rsvd0:26;   /**/
	u32 gate:1;     /**/
	u32 rsvd1:3;    /**/
	u32 rst:1;      /**/
	} bits;
} reg_doss_mipi_dsi1_ctrl_t;

#define DOSS_MIPI_DSI1_CTRL_SLAVE_0     0x0
#define DOSS_MIPI_DSI1_CTRL_SLAVE_1     0x1
#define DOSS_MIPI_DSI1_CTRL_GATE_0      0x0
#define DOSS_MIPI_DSI1_CTRL_GATE_1      0x1
#define DOSS_MIPI_DSI1_CTRL_RST_0       0x0
#define DOSS_MIPI_DSI1_CTRL_RST_1       0x1

/* MIPI DSI[CH] Reference Clock Register */
typedef union {
	u32 val;
	struct {
	u32 div0:4;     /**/
	u32 rsvd0:4;    /**/
	u32 rsvd1:8;    /**/
	u32 rsvd2:4;    /**/
	u32 rsvd3:11;   /**/
	u32 gate:1;     /**/
	} bits;
} reg_doss_mipi_dsi0_ref_clk_t;

#define DOSS_MIPI_DSI0_REF_CLK_GATE_0   0x0
#define DOSS_MIPI_DSI0_REF_CLK_GATE_1   0x1

/* MIPI DSI[CH] Reference Clock Register */
typedef union {
	u32 val;
	struct {
	u32 div0:4;     /**/
	u32 rsvd0:4;    /**/
	u32 rsvd1:8;    /**/
	u32 rsvd2:4;    /**/
	u32 rsvd3:11;   /**/
	u32 gate:1;     /**/
	} bits;
} reg_doss_mipi_dsi1_ref_clk_t;

#define DOSS_MIPI_DSI1_REF_CLK_GATE_0   0x0
#define DOSS_MIPI_DSI1_REF_CLK_GATE_1   0x1

/* MIPI DSI[CH] Configuration Clock Register */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:4;    /**/
	u32 rsvd1:4;    /**/
	u32 rsvd2:8;    /**/
	u32 rsvd3:4;    /**/
	u32 rsvd4:11;   /**/
	u32 gate:1;     /**/
	} bits;
} reg_doss_mipi_dsi0_cfg_clk_t;

#define DOSS_MIPI_DSI0_CFG_CLK_GATE_0   0x0
#define DOSS_MIPI_DSI0_CFG_CLK_GATE_1   0x1

/* MIPI DSI[CH] Configuration Clock Register */
typedef union {
	u32 val;
	struct {
	u32 rsvd0:4;    /**/
	u32 rsvd1:4;    /**/
	u32 rsvd2:8;    /**/
	u32 rsvd3:4;    /**/
	u32 rsvd4:11;   /**/
	u32 gate:1;     /**/
	} bits;
} reg_doss_mipi_dsi1_cfg_clk_t;

#define DOSS_MIPI_DSI1_CFG_CLK_GATE_0   0x0
#define DOSS_MIPI_DSI1_CFG_CLK_GATE_1   0x1

/* TVE[CH] Control Register */
typedef union {
	u32 val;
	struct {
	u32 slave:1;    /**/
	u32 rsvd0:26;   /**/
	u32 gate:1;     /**/
	u32 rsvd1:3;    /**/
	u32 rst:1;      /**/
	} bits;
} reg_doss_tve0_ctrl_t;

#define DOSS_TVE0_CTRL_GATE_0   0x0
#define DOSS_TVE0_CTRL_GATE_1   0x1
#define DOSS_TVE0_CTRL_RST_0    0x0
#define DOSS_TVE0_CTRL_RST_1    0x1

/* TVE[CH] Control Register */
typedef union {
	u32 val;
	struct {
	u32 slave:1;    /**/
	u32 rsvd0:26;   /**/
	u32 gate:1;     /**/
	u32 rsvd1:3;    /**/
	u32 rst:1;      /**/
	} bits;
} reg_doss_tve1_ctrl_t;

#define DOSS_TVE1_CTRL_GATE_0   0x0
#define DOSS_TVE1_CTRL_GATE_1   0x1
#define DOSS_TVE1_CTRL_RST_0    0x0
#define DOSS_TVE1_CTRL_RST_1    0x1

/* TVE[CH] Clock Register */
typedef union {
	u32 val;
	struct {
	u32 div0:4;     /**/
	u32 rsvd0:4;    /**/
	u32 rsvd1:8;    /**/
	u32 rsvd2:4;    /**/
	u32 rsvd3:11;   /**/
	u32 gate:1;     /**/
	} bits;
} reg_doss_tve0_clk_t;

#define DOSS_TVE0_CLK_GATE_0    0x0
#define DOSS_TVE0_CLK_GATE_1    0x1

/* TVE[CH] Clock Register */
typedef union {
	u32 val;
	struct {
	u32 div0:4;     /**/
	u32 rsvd0:4;    /**/
	u32 rsvd1:8;    /**/
	u32 rsvd2:4;    /**/
	u32 rsvd3:11;   /**/
	u32 gate:1;     /**/
	} bits;
} reg_doss_tve1_clk_t;

#define DOSS_TVE1_CLK_GATE_0    0x0
#define DOSS_TVE1_CLK_GATE_1    0x1

/* reg_doss_t bank */
typedef struct tag_doss {
	reg_doss_ver_t                 ver;                /* 0000 */
	u32                            res0[3];
	reg_doss_do0_ftr_t             do0_ftr;            /* 0010 */
	reg_doss_do1_ftr_t             do1_ftr;            /* 0014 */
	u32                            res1[2];
	reg_doss_reset_t               reset;              /* 0020 */
	reg_doss_route_t               route;              /* 0024 */
	u32                            res2[54];
	reg_doss_tcon0_ctrl_t          tcon0_ctrl;         /* 0100 */
	reg_doss_tcon0_clk_t           tcon0_clk;          /* 0104 */
	u32                            res3[6];
	reg_doss_tcon1_ctrl_t          tcon1_ctrl;         /* 0120 */
	reg_doss_tcon1_clk_t           tcon1_clk;          /* 0124 */
	u32                            res4[6];
	reg_doss_lvds0_ctrl_t          lvds0_ctrl;         /* 0140 */
	reg_doss_lvds0_clk_t           lvds0_clk;          /* 0144 */
	u32                            res5[6];
	reg_doss_lvds1_ctrl_t          lvds1_ctrl;         /* 0160 */
	reg_doss_lvds1_clk_t           lvds1_clk;          /* 0164 */
	u32                            res6[6];
	reg_doss_mipi_dsi0_ctrl_t      mipi_dsi0_ctrl;     /* 0180 */
	reg_doss_mipi_dsi0_ref_clk_t   mipi_dsi0_ref_clk;  /* 0184 */
	reg_doss_mipi_dsi0_cfg_clk_t   mipi_dsi0_cfg_clk;  /* 0188 */
	u32                            res7[5];
	reg_doss_mipi_dsi1_ctrl_t      mipi_dsi1_ctrl;     /* 01A0 */
	reg_doss_mipi_dsi1_ref_clk_t   mipi_dsi1_ref_clk;  /* 01A4 */
	reg_doss_mipi_dsi1_cfg_clk_t   mipi_dsi1_cfg_clk;  /* 01A8 */
	u32                            res8[149];
	reg_doss_tve0_ctrl_t           tve0_ctrl;          /* 0400 */
	reg_doss_tve0_clk_t            tve0_clk;           /* 0404 */
	u32                            res9[6];
	reg_doss_tve1_ctrl_t           tve1_ctrl;          /* 0420 */
	reg_doss_tve1_clk_t            tve1_clk;           /* 0424 */
} reg_doss_t;

#endif /* ___DOSS___H___ */
