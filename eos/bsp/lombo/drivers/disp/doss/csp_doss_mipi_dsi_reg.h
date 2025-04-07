/* mipi_dsi.h */

#ifndef ___MIPI_DSI___H___
#define ___MIPI_DSI___H___

#define BASE_MIPI_DSI                0x01606000

#define VA_MIPI_DSI_VER              (0x00000000 + BASE_MIPI_DSI + VA_MIPI_DSI)
#define VA_MIPI_DSI_HOST_CTRL        (0x00000004 + BASE_MIPI_DSI + VA_MIPI_DSI)
#define VA_MIPI_DSI_CLK_DIV          (0x00000008 + BASE_MIPI_DSI + VA_MIPI_DSI)
#define VA_MIPI_DSI_DPI_CFG          (0x0000000C + BASE_MIPI_DSI + VA_MIPI_DSI)
#define VA_MIPI_DSI_DBI_CFG          (0x00000010 + BASE_MIPI_DSI + VA_MIPI_DSI)
#define VA_MIPI_DSI_CMD_SIZE         (0x00000014 + BASE_MIPI_DSI + VA_MIPI_DSI)
#define VA_MIPI_DSI_PKT_CFG          (0x00000018 + BASE_MIPI_DSI + VA_MIPI_DSI)
#define VA_MIPI_DSI_VM_CFG           (0x0000001C + BASE_MIPI_DSI + VA_MIPI_DSI)
#define VA_MIPI_DSI_VM_PKT_CFG       (0x00000020 + BASE_MIPI_DSI + VA_MIPI_DSI)
#define VA_MIPI_DSI_CM_CFG           (0x00000024 + BASE_MIPI_DSI + VA_MIPI_DSI)
#define VA_MIPI_DSI_LT_CFG           (0x00000028 + BASE_MIPI_DSI + VA_MIPI_DSI)
#define VA_MIPI_DSI_VT_CFG           (0x0000002C + BASE_MIPI_DSI + VA_MIPI_DSI)
#define VA_MIPI_DSI_PHY_TIM          (0x00000030 + BASE_MIPI_DSI + VA_MIPI_DSI)
#define VA_MIPI_DSI_GEN_PH_CFG       (0x00000034 + BASE_MIPI_DSI + VA_MIPI_DSI)
#define VA_MIPI_DSI_GEN_PL_DATA      (0x00000038 + BASE_MIPI_DSI + VA_MIPI_DSI)
#define VA_MIPI_DSI_CMD_PKT_STA      (0x0000003C + BASE_MIPI_DSI + VA_MIPI_DSI)
#define VA_MIPI_DSI_TO_CNT_CFG       (0x00000040 + BASE_MIPI_DSI + VA_MIPI_DSI)
#define VA_MIPI_DSI_INT_EN0          (0x00000048 + BASE_MIPI_DSI + VA_MIPI_DSI)
#define VA_MIPI_DSI_INT_EN1          (0x0000004C + BASE_MIPI_DSI + VA_MIPI_DSI)
#define VA_MIPI_DSI_INT_PD0          (0x00000050 + BASE_MIPI_DSI + VA_MIPI_DSI)
#define VA_MIPI_DSI_INT_PD1          (0x00000054 + BASE_MIPI_DSI + VA_MIPI_DSI)
#define VA_MIPI_DSI_INT_CLR0         (0x00000058 + BASE_MIPI_DSI + VA_MIPI_DSI)
#define VA_MIPI_DSI_INT_CLR1         (0x0000005C + BASE_MIPI_DSI + VA_MIPI_DSI)
#define VA_MIPI_DSI_PHY_CTRL         (0x00000060 + BASE_MIPI_DSI + VA_MIPI_DSI)
#define VA_MIPI_DSI_PHY_IF_CFG       (0x00000064 + BASE_MIPI_DSI + VA_MIPI_DSI)
#define VA_MIPI_DSI_PHY_IF_CTRL      (0x00000068 + BASE_MIPI_DSI + VA_MIPI_DSI)
#define VA_MIPI_DSI_PHY_STA          (0x0000006C + BASE_MIPI_DSI + VA_MIPI_DSI)
#define VA_MIPI_DSI_PHY_TST_CTRL0    (0x00000070 + BASE_MIPI_DSI + VA_MIPI_DSI)
#define VA_MIPI_DSI_PHY_TST_CTRL1    (0x00000074 + BASE_MIPI_DSI + VA_MIPI_DSI)

#define DATA_MIPI_DSI_VER                    0x00001100
#define DATA_MIPI_DSI_HOST_CTRL              0x00000000
#define DATA_MIPI_DSI_CLK_DIV                0x00000000
#define DATA_MIPI_DSI_DPI_CFG                0x00000000
#define DATA_MIPI_DSI_DBI_CFG                0x00000000
#define DATA_MIPI_DSI_CMD_SIZE               0x00000000
#define DATA_MIPI_DSI_PKT_CFG                0x00000000
#define DATA_MIPI_DSI_VM_CFG                 0x00000000
#define DATA_MIPI_DSI_VM_PKT_CFG             0x00000000
#define DATA_MIPI_DSI_CM_CFG                 0x00000000
#define DATA_MIPI_DSI_LT_CFG                 0x00000000
#define DATA_MIPI_DSI_VT_CFG                 0x00000000
#define DATA_MIPI_DSI_PHY_TIM                0x00000000
#define DATA_MIPI_DSI_GEN_PH_CFG             0x00000000
#define DATA_MIPI_DSI_GEN_PL_DATA            0x00000000
#define DATA_MIPI_DSI_CMD_PKT_STA            0x00001515
#define DATA_MIPI_DSI_TO_CNT_CFG             0x00000000
#define DATA_MIPI_DSI_INT_EN0                0x00000000
#define DATA_MIPI_DSI_INT_EN1                0x00000000
#define DATA_MIPI_DSI_INT_PD0                0x00000000
#define DATA_MIPI_DSI_INT_PD1                0x00000000
#define DATA_MIPI_DSI_INT_CLR0               0x00000000
#define DATA_MIPI_DSI_INT_CLR1               0x00000000
#define DATA_MIPI_DSI_PHY_CTRL               0x00000000
#define DATA_MIPI_DSI_PHY_IF_CFG             0x00000003
#define DATA_MIPI_DSI_PHY_IF_CTRL            0x00000000
#define DATA_MIPI_DSI_PHY_STA                0x00000000
#define DATA_MIPI_DSI_PHY_TST_CTRL0          0x00000001
#define DATA_MIPI_DSI_PHY_TST_CTRL1          0x00000000

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
} reg_mipi_dsi_ver_t;

#define MIPI_DSI_VER_COMP_0     0x0
#define MIPI_DSI_VER_COMP_1     0x1

/* Host Control Register */
typedef union {
	u32 val;
	struct {
	u32 rstn:1;     /**/
	u32 rsvd0:31;   /**/
	} bits;
} reg_mipi_dsi_host_ctrl_t;

#define MIPI_DSI_HOST_CTRL_RSTN_0       0x0
#define MIPI_DSI_HOST_CTRL_RSTN_1       0x1

/* Clock Division Register */
typedef union {
	u32 val;
	struct {
	u32 tx_esc_div:8;/**/
	u32 to_clk_div:8;/**/
	u32 rsvd0:16;   /**/
	} bits;
} reg_mipi_dsi_clk_div_t;

/* DPI Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 vid:2;      /**/
	u32 cc:2;       /**/
	u32 rsvd0:1;    /**/
	u32 de_inv:1;   /**/
	u32 vs_inv:1;   /**/
	u32 hs_inv:1;   /**/
	u32 sd_inv:1;   /**/
	u32 cm_inv:1;   /**/
	u32 rgb666_l_en:1;/**/
	u32 rsvd1:1;    /**/
	u32 sd:1;       /**/
	u32 cm:1;       /**/
	u32 rsvd2:18;   /**/
	} bits;
} reg_mipi_dsi_dpi_cfg_t;

#define MIPI_DSI_DPI_CFG_CC_0           0x0
#define MIPI_DSI_DPI_CFG_CC_1           0x1
#define MIPI_DSI_DPI_CFG_CC_2           0x2
#define MIPI_DSI_DPI_CFG_CC_3           0x3
#define MIPI_DSI_DPI_CFG_DE_INV_0       0x0
#define MIPI_DSI_DPI_CFG_DE_INV_1       0x1
#define MIPI_DSI_DPI_CFG_VS_INV_0       0x0
#define MIPI_DSI_DPI_CFG_VS_INV_1       0x1
#define MIPI_DSI_DPI_CFG_HS_INV_0       0x0
#define MIPI_DSI_DPI_CFG_HS_INV_1       0x1
#define MIPI_DSI_DPI_CFG_SD_INV_0       0x0
#define MIPI_DSI_DPI_CFG_SD_INV_1       0x1
#define MIPI_DSI_DPI_CFG_CM_INV_0       0x0
#define MIPI_DSI_DPI_CFG_CM_INV_1       0x1
#define MIPI_DSI_DPI_CFG_RGB666_L_EN_0  0x0
#define MIPI_DSI_DPI_CFG_RGB666_L_EN_1  0x1
#define MIPI_DSI_DPI_CFG_SD_0           0x0
#define MIPI_DSI_DPI_CFG_SD_1           0x1
#define MIPI_DSI_DPI_CFG_CM_0           0x0
#define MIPI_DSI_DPI_CFG_CM_1           0x1

/* DBI Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 vid:2;      /**/
	u32 in_fmt:4;   /**/
	u32 lut_size:2; /**/
	u32 part_en:1;  /**/
	u32 out_fmt:4;  /**/
	u32 rsvd0:19;   /**/
	} bits;
} reg_mipi_dsi_dbi_cfg_t;

#define MIPI_DSI_DBI_CFG_IN_FMT_0       0x0
#define MIPI_DSI_DBI_CFG_IN_FMT_1       0x1
#define MIPI_DSI_DBI_CFG_IN_FMT_2       0x2
#define MIPI_DSI_DBI_CFG_IN_FMT_3       0x3
#define MIPI_DSI_DBI_CFG_IN_FMT_4       0x4
#define MIPI_DSI_DBI_CFG_IN_FMT_5       0x5
#define MIPI_DSI_DBI_CFG_IN_FMT_6       0x6
#define MIPI_DSI_DBI_CFG_IN_FMT_7       0x7
#define MIPI_DSI_DBI_CFG_IN_FMT_8       0x8
#define MIPI_DSI_DBI_CFG_IN_FMT_9       0x9
#define MIPI_DSI_DBI_CFG_IN_FMT_A       0xA
#define MIPI_DSI_DBI_CFG_IN_FMT_B       0xB
#define MIPI_DSI_DBI_CFG_IN_FMT_C       0xC
#define MIPI_DSI_DBI_CFG_LUT_SIZE_0     0x0
#define MIPI_DSI_DBI_CFG_LUT_SIZE_1     0x1
#define MIPI_DSI_DBI_CFG_LUT_SIZE_2     0x2
#define MIPI_DSI_DBI_CFG_LUT_SIZE_3     0x3
#define MIPI_DSI_DBI_CFG_PART_EN_0      0x0
#define MIPI_DSI_DBI_CFG_PART_EN_1      0x1
#define MIPI_DSI_DBI_CFG_OUT_FMT_0      0x0
#define MIPI_DSI_DBI_CFG_OUT_FMT_1      0x1
#define MIPI_DSI_DBI_CFG_OUT_FMT_2      0x2
#define MIPI_DSI_DBI_CFG_OUT_FMT_3      0x3
#define MIPI_DSI_DBI_CFG_OUT_FMT_4      0x4
#define MIPI_DSI_DBI_CFG_OUT_FMT_5      0x5
#define MIPI_DSI_DBI_CFG_OUT_FMT_6      0x6
#define MIPI_DSI_DBI_CFG_OUT_FMT_7      0x7
#define MIPI_DSI_DBI_CFG_OUT_FMT_8      0x8
#define MIPI_DSI_DBI_CFG_OUT_FMT_9      0x9
#define MIPI_DSI_DBI_CFG_OUT_FMT_A      0xA
#define MIPI_DSI_DBI_CFG_OUT_FMT_B      0xB
#define MIPI_DSI_DBI_CFG_OUT_FMT_C      0xC

/* Command Size Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 wr_cmd_size:16;     /**/
	u32 max_cmd_size:16;    /**/
	} bits;
} reg_mipi_dsi_cmd_size_t;

/* Packet Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 eot_tx_en:1;/**/
	u32 eot_rx_en:1;/**/
	u32 bta_en:1;   /**/
	u32 ecc_en:1;   /**/
	u32 crc_en:1;   /**/
	u32 vid_rx:2;   /**/
	u32 rsvd0:25;   /**/
	} bits;
} reg_mipi_dsi_pkt_cfg_t;

#define MIPI_DSI_PKT_CFG_EOT_TX_EN_0    0x0
#define MIPI_DSI_PKT_CFG_EOT_TX_EN_1    0x1
#define MIPI_DSI_PKT_CFG_EOT_RX_EN_0    0x0
#define MIPI_DSI_PKT_CFG_EOT_RX_EN_1    0x1
#define MIPI_DSI_PKT_CFG_BTA_EN_0       0x0
#define MIPI_DSI_PKT_CFG_BTA_EN_1       0x1
#define MIPI_DSI_PKT_CFG_ECC_EN_0       0x0
#define MIPI_DSI_PKT_CFG_ECC_EN_1       0x1
#define MIPI_DSI_PKT_CFG_CRC_EN_0       0x0
#define MIPI_DSI_PKT_CFG_CRC_EN_1       0x1

/* Video Mode Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 vm_en:1;            /**/
	u32 vm_type:2;          /**/
	u32 lp_vsa_en:1;        /**/
	u32 lp_vbp_en:1;        /**/
	u32 lp_vfp_en:1;        /**/
	u32 lp_vact_en:1;       /**/
	u32 lp_hbp_en:1;        /**/
	u32 lp_hfp_en:1;        /**/
	u32 multi_pkt_en:1;     /**/
	u32 null_pkt_en:1;      /**/
	u32 bta_ack_en:1;       /**/
	u32 rsvd0:20;           /**/
	} bits;
} reg_mipi_dsi_vm_cfg_t;

#define MIPI_DSI_VM_CFG_VM_EN_0         0x0
#define MIPI_DSI_VM_CFG_VM_EN_1         0x1
#define MIPI_DSI_VM_CFG_VM_TYPE_0       0x0
#define MIPI_DSI_VM_CFG_VM_TYPE_1       0x1
#define MIPI_DSI_VM_CFG_VM_TYPE_2       0x2
#define MIPI_DSI_VM_CFG_VM_TYPE_3       0x3
#define MIPI_DSI_VM_CFG_LP_VSA_EN_0     0x0
#define MIPI_DSI_VM_CFG_LP_VSA_EN_1     0x1
#define MIPI_DSI_VM_CFG_LP_VBP_EN_0     0x0
#define MIPI_DSI_VM_CFG_LP_VBP_EN_1     0x1
#define MIPI_DSI_VM_CFG_LP_VFP_EN_0     0x0
#define MIPI_DSI_VM_CFG_LP_VFP_EN_1     0x1
#define MIPI_DSI_VM_CFG_LP_VACT_EN_0    0x0
#define MIPI_DSI_VM_CFG_LP_VACT_EN_1    0x1
#define MIPI_DSI_VM_CFG_LP_HBP_EN_0     0x0
#define MIPI_DSI_VM_CFG_LP_HBP_EN_1     0x1
#define MIPI_DSI_VM_CFG_LP_HFP_EN_0     0x0
#define MIPI_DSI_VM_CFG_LP_HFP_EN_1     0x1
#define MIPI_DSI_VM_CFG_MULTI_PKT_EN_0  0x0
#define MIPI_DSI_VM_CFG_MULTI_PKT_EN_1  0x1
#define MIPI_DSI_VM_CFG_NULL_PKT_EN_0   0x0
#define MIPI_DSI_VM_CFG_NULL_PKT_EN_1   0x1
#define MIPI_DSI_VM_CFG_BTA_ACK_EN_0    0x0
#define MIPI_DSI_VM_CFG_BTA_ACK_EN_1    0x1

/* Video Packet Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 vm_pkt_size:11;     /**/
	u32 chunks_num:10;      /**/
	u32 null_pkt_size:10;   /**/
	u32 rsvd0:1;            /**/
	} bits;
} reg_mipi_dsi_vm_pkt_cfg_t;

/* Command Mode Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 cm_en:1;    /**/
	u32 gen_sw_0p:1;/**/
	u32 gen_sw_1p:1;/**/
	u32 gen_sw_2p:1;/**/
	u32 gen_sr_0p:1;/**/
	u32 gen_sr_1p:1;/**/
	u32 gen_sr_2p:1;/**/
	u32 dcs_sw_0p:1;/**/
	u32 dcs_sw_1p:1;/**/
	u32 dcs_sr_0p:1;/**/
	u32 max_rd_pkt:1;/**/
	u32 gen_lw:1;   /**/
	u32 dcs_lw:1;   /**/
	u32 ack_rq_en:1;/**/
	u32 tear_en:1;  /**/
	u32 rsvd0:17;   /**/
	} bits;
} reg_mipi_dsi_cm_cfg_t;

#define MIPI_DSI_CM_CFG_CM_EN_0         0x0
#define MIPI_DSI_CM_CFG_CM_EN_1         0x1
#define MIPI_DSI_CM_CFG_GEN_SW_0P_0     0x0
#define MIPI_DSI_CM_CFG_GEN_SW_0P_1     0x1
#define MIPI_DSI_CM_CFG_GEN_SW_1P_0     0x0
#define MIPI_DSI_CM_CFG_GEN_SW_1P_1     0x1
#define MIPI_DSI_CM_CFG_GEN_SW_2P_0     0x0
#define MIPI_DSI_CM_CFG_GEN_SW_2P_1     0x1
#define MIPI_DSI_CM_CFG_GEN_SR_0P_0     0x0
#define MIPI_DSI_CM_CFG_GEN_SR_0P_1     0x1
#define MIPI_DSI_CM_CFG_GEN_SR_1P_0     0x0
#define MIPI_DSI_CM_CFG_GEN_SR_1P_1     0x1
#define MIPI_DSI_CM_CFG_GEN_SR_2P_0     0x0
#define MIPI_DSI_CM_CFG_GEN_SR_2P_1     0x1
#define MIPI_DSI_CM_CFG_DCS_SW_0P_0     0x0
#define MIPI_DSI_CM_CFG_DCS_SW_0P_1     0x1
#define MIPI_DSI_CM_CFG_DCS_SW_1P_0     0x0
#define MIPI_DSI_CM_CFG_DCS_SW_1P_1     0x1
#define MIPI_DSI_CM_CFG_DCS_SR_0P_0     0x0
#define MIPI_DSI_CM_CFG_DCS_SR_0P_1     0x1
#define MIPI_DSI_CM_CFG_MAX_RD_PKT_0    0x0
#define MIPI_DSI_CM_CFG_MAX_RD_PKT_1    0x1
#define MIPI_DSI_CM_CFG_GEN_LW_0        0x0
#define MIPI_DSI_CM_CFG_GEN_LW_1        0x1
#define MIPI_DSI_CM_CFG_DCS_LW_0        0x0
#define MIPI_DSI_CM_CFG_DCS_LW_1        0x1
#define MIPI_DSI_CM_CFG_ACK_RQ_EN_0     0x0
#define MIPI_DSI_CM_CFG_ACK_RQ_EN_1     0x1
#define MIPI_DSI_CM_CFG_TEAR_EN_0       0x0
#define MIPI_DSI_CM_CFG_TEAR_EN_1       0x1

/* Line Timer Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 hsa:9;      /**/
	u32 hbp:9;      /**/
	u32 hline:14;   /**/
	} bits;
} reg_mipi_dsi_lt_cfg_t;

/* Vertical Timing Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 vsa:4;      /**/
	u32 vbp:6;      /**/
	u32 vfp:6;      /**/
	u32 vact:11;    /**/
	u32 rsvd0:5;    /**/
	} bits;
} reg_mipi_dsi_vt_cfg_t;

/* PHY Timing Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 bta:12;     /**/
	u32 lp2hs:8;    /**/
	u32 hs2lp:8;    /**/
	u32 rsvd0:4;    /**/
	} bits;
} reg_mipi_dsi_phy_tim_t;

/* Generic Packet Header Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 htype:8;    /**/
	u32 hdata:16;   /**/
	u32 rsvd0:8;    /**/
	} bits;
} reg_mipi_dsi_gen_ph_cfg_t;

/* Generic Payload Data Register */
typedef union {
	u32 val;
	struct {
	u32 data:32;    /**/
	} bits;
} reg_mipi_dsi_gen_pl_data_t;

/* Command Packet Status Register */
typedef union {
	u32 val;
	struct {
	u32 gcmd_empty:1;       /**/
	u32 gcmd_full:1;        /**/
	u32 gpl_wr_empty:1;     /**/
	u32 gpl_wr_full:1;      /**/
	u32 gpl_rd_empty:1;     /**/
	u32 gpl_rd_full:1;      /**/
	u32 grd_cmd_busy:1;     /**/
	u32 rsvd0:1;            /**/
	u32 dcmd_empty:1;       /**/
	u32 dcmd_full:1;        /**/
	u32 dpl_wr_empty:1;     /**/
	u32 dpl_wr_full:1;      /**/
	u32 dpl_rd_empty:1;     /**/
	u32 dpl_rd_full:1;      /**/
	u32 drd_cmd_busy:1;     /**/
	u32 rsvd1:17;           /**/
	} bits;
} reg_mipi_dsi_cmd_pkt_sta_t;

/* Time Out Timer Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 hstx_to_cnt:16;/**/
	u32 lprx_to_cnt:16;/**/
	} bits;
} reg_mipi_dsi_to_cnt_cfg_t;

/* Interrupt Enable 0 Register */
typedef union {
	u32 val;
	struct {
	u32 err_ack_sot:1;      /**/
	u32 err_ack_sots:1;     /**/
	u32 err_ack_eots:1;     /**/
	u32 err_ack_ecmd:1;     /**/
	u32 err_ack_lps:1;      /**/
	u32 err_ack_hsto:1;     /**/
	u32 err_ack_fc:1;       /**/
	u32 err_ack_rsvd00:1;   /**/
	u32 err_ack_ecc1:1;     /**/
	u32 err_ack_eccm:1;     /**/
	u32 err_ack_cks:1;      /**/
	u32 err_ack_dt:1;       /**/
	u32 err_ack_vid:1;      /**/
	u32 err_ack_tl:1;       /**/
	u32 err_ack_rsvd11:1;   /**/
	u32 err_ack_pv:1;       /**/
	u32 err_esc:1;          /**/
	u32 err_esc_sync:1;     /**/
	u32 err_ctl:1;          /**/
	u32 err_lp0:1;          /**/
	u32 err_lp1:1;          /**/
	u32 rsvd2:11;           /**/
	} bits;
} reg_mipi_dsi_int_en0_t;

#define MIPI_DSI_INT_EN0_ERR_ACK_SOT_0          0x0
#define MIPI_DSI_INT_EN0_ERR_ACK_SOT_1          0x1
#define MIPI_DSI_INT_EN0_ERR_ACK_SOTS_0         0x0
#define MIPI_DSI_INT_EN0_ERR_ACK_SOTS_1         0x1
#define MIPI_DSI_INT_EN0_ERR_ACK_EOTS_0         0x0
#define MIPI_DSI_INT_EN0_ERR_ACK_EOTS_1         0x1
#define MIPI_DSI_INT_EN0_ERR_ACK_ECMD_0         0x0
#define MIPI_DSI_INT_EN0_ERR_ACK_ECMD_1         0x1
#define MIPI_DSI_INT_EN0_ERR_ACK_LPS_0          0x0
#define MIPI_DSI_INT_EN0_ERR_ACK_LPS_1          0x1
#define MIPI_DSI_INT_EN0_ERR_ACK_HSTO_0         0x0
#define MIPI_DSI_INT_EN0_ERR_ACK_HSTO_1         0x1
#define MIPI_DSI_INT_EN0_ERR_ACK_FC_0           0x0
#define MIPI_DSI_INT_EN0_ERR_ACK_FC_1           0x1
#define MIPI_DSI_INT_EN0_ERR_ACK_RSVD0_0        0x0
#define MIPI_DSI_INT_EN0_ERR_ACK_RSVD0_1        0x1
#define MIPI_DSI_INT_EN0_ERR_ACK_ECC1_0         0x0
#define MIPI_DSI_INT_EN0_ERR_ACK_ECC1_1         0x1
#define MIPI_DSI_INT_EN0_ERR_ACK_ECCM_0         0x0
#define MIPI_DSI_INT_EN0_ERR_ACK_ECCM_1         0x1
#define MIPI_DSI_INT_EN0_ERR_ACK_CKS_0          0x0
#define MIPI_DSI_INT_EN0_ERR_ACK_CKS_1          0x1
#define MIPI_DSI_INT_EN0_ERR_ACK_DT_0           0x0
#define MIPI_DSI_INT_EN0_ERR_ACK_DT_1           0x1
#define MIPI_DSI_INT_EN0_ERR_ACK_VID_0          0x0
#define MIPI_DSI_INT_EN0_ERR_ACK_VID_1          0x1
#define MIPI_DSI_INT_EN0_ERR_ACK_TL_0           0x0
#define MIPI_DSI_INT_EN0_ERR_ACK_TL_1           0x1
#define MIPI_DSI_INT_EN0_ERR_ACK_RSVD1_0        0x0
#define MIPI_DSI_INT_EN0_ERR_ACK_RSVD1_1        0x1
#define MIPI_DSI_INT_EN0_ERR_ACK_PV_0           0x0
#define MIPI_DSI_INT_EN0_ERR_ACK_PV_1           0x1
#define MIPI_DSI_INT_EN0_ERR_ESC_0              0x0
#define MIPI_DSI_INT_EN0_ERR_ESC_1              0x1
#define MIPI_DSI_INT_EN0_ERR_ESC_SYNC_0         0x0
#define MIPI_DSI_INT_EN0_ERR_ESC_SYNC_1         0x1
#define MIPI_DSI_INT_EN0_ERR_CTL_0              0x0
#define MIPI_DSI_INT_EN0_ERR_CTL_1              0x1
#define MIPI_DSI_INT_EN0_ERR_LP0_0              0x0
#define MIPI_DSI_INT_EN0_ERR_LP0_1              0x1
#define MIPI_DSI_INT_EN0_ERR_LP1_0              0x0
#define MIPI_DSI_INT_EN0_ERR_LP1_1              0x1

/* Interrupt Enable 1 Register */
typedef union {
	u32 val;
	struct {
	u32 err_lptx_to:1;      /**/
	u32 err_lprx_to:1;      /**/
	u32 err_ecc1:1;         /**/
	u32 err_eccm:1;         /**/
	u32 err_crc:1;          /**/
	u32 err_pkt_size:1;     /**/
	u32 err_eotp:1;         /**/
	u32 err_dpi_wr:1;       /**/
	u32 err_gcmd_wr:1;      /**/
	u32 err_gpl_wr:1;       /**/
	u32 err_gpl_tx:1;       /**/
	u32 err_gpl_rd:1;       /**/
	u32 err_gpl_rx:1;       /**/
	u32 err_cmd_wr:1;       /**/
	u32 err_pl_wr:1;        /**/
	u32 err_pl_rd:1;        /**/
	u32 err_pl_rx:1;        /**/
	u32 err_ill_cmd:1;      /**/
	u32 rsvd0:14;           /**/
	} bits;
} reg_mipi_dsi_int_en1_t;

#define MIPI_DSI_INT_EN1_ERR_LPTX_TO_0  0x0
#define MIPI_DSI_INT_EN1_ERR_LPTX_TO_1  0x1
#define MIPI_DSI_INT_EN1_ERR_LPRX_TO_0  0x0
#define MIPI_DSI_INT_EN1_ERR_LPRX_TO_1  0x1
#define MIPI_DSI_INT_EN1_ERR_ECC1_0     0x0
#define MIPI_DSI_INT_EN1_ERR_ECC1_1     0x1
#define MIPI_DSI_INT_EN1_ERR_ECCM_0     0x0
#define MIPI_DSI_INT_EN1_ERR_ECCM_1     0x1
#define MIPI_DSI_INT_EN1_ERR_CRC_0      0x0
#define MIPI_DSI_INT_EN1_ERR_CRC_1      0x1
#define MIPI_DSI_INT_EN1_ERR_PKT_SIZE_0 0x0
#define MIPI_DSI_INT_EN1_ERR_PKT_SIZE_1 0x1
#define MIPI_DSI_INT_EN1_ERR_EOTP_0     0x0
#define MIPI_DSI_INT_EN1_ERR_EOTP_1     0x1
#define MIPI_DSI_INT_EN1_ERR_DPI_WR_0   0x0
#define MIPI_DSI_INT_EN1_ERR_DPI_WR_1   0x1
#define MIPI_DSI_INT_EN1_ERR_GCMD_WR_0  0x0
#define MIPI_DSI_INT_EN1_ERR_GCMD_WR_1  0x1
#define MIPI_DSI_INT_EN1_ERR_GPL_WR_0   0x0
#define MIPI_DSI_INT_EN1_ERR_GPL_WR_1   0x1
#define MIPI_DSI_INT_EN1_ERR_GPL_TX_0   0x0
#define MIPI_DSI_INT_EN1_ERR_GPL_TX_1   0x1
#define MIPI_DSI_INT_EN1_ERR_GPL_RD_0   0x0
#define MIPI_DSI_INT_EN1_ERR_GPL_RD_1   0x1
#define MIPI_DSI_INT_EN1_ERR_GPL_RX_0   0x0
#define MIPI_DSI_INT_EN1_ERR_GPL_RX_1   0x1
#define MIPI_DSI_INT_EN1_ERR_CMD_WR_0   0x0
#define MIPI_DSI_INT_EN1_ERR_CMD_WR_1   0x1
#define MIPI_DSI_INT_EN1_ERR_PL_WR_0    0x0
#define MIPI_DSI_INT_EN1_ERR_PL_WR_1    0x1
#define MIPI_DSI_INT_EN1_ERR_PL_RD_0    0x0
#define MIPI_DSI_INT_EN1_ERR_PL_RD_1    0x1
#define MIPI_DSI_INT_EN1_ERR_PL_RX_0    0x0
#define MIPI_DSI_INT_EN1_ERR_PL_RX_1    0x1
#define MIPI_DSI_INT_EN1_ERR_ILL_CMD_0  0x0
#define MIPI_DSI_INT_EN1_ERR_ILL_CMD_1  0x1

/* Interrupt Pending 0 Register */
typedef union {
	u32 val;
	struct {
	u32 err_ack_sot:1;      /**/
	u32 err_ack_sots:1;     /**/
	u32 err_ack_eots:1;     /**/
	u32 err_ack_ecmd:1;     /**/
	u32 err_ack_lps:1;      /**/
	u32 err_ack_hsto:1;     /**/
	u32 err_ack_fc:1;       /**/
	u32 err_ack_rsvd00:1;   /**/
	u32 err_ack_ecc1:1;     /**/
	u32 err_ack_eccm:1;     /**/
	u32 err_ack_cks:1;      /**/
	u32 err_ack_dt:1;       /**/
	u32 err_ack_vid:1;      /**/
	u32 err_ack_tl:1;       /**/
	u32 err_ack_rsvd11:1;   /**/
	u32 err_ack_pv:1;       /**/
	u32 err_esc:1;          /**/
	u32 err_esc_sync:1;     /**/
	u32 err_ctl:1;          /**/
	u32 err_lp0:1;          /**/
	u32 err_lp1:1;          /**/
	u32 rsvd2:11;           /**/
	} bits;
} reg_mipi_dsi_int_pd0_t;

/* Interrupt Pending 1 Register */
typedef union {
	u32 val;
	struct {
	u32 err_lptx_to:1;      /**/
	u32 err_lprx_to:1;      /**/
	u32 err_ecc1:1;         /**/
	u32 err_eccm:1;         /**/
	u32 err_crc:1;          /**/
	u32 err_pkt_size:1;     /**/
	u32 err_eotp:1;         /**/
	u32 err_dpi_wr:1;       /**/
	u32 err_gcmd_wr:1;      /**/
	u32 err_gpl_wr:1;       /**/
	u32 err_gpl_tx:1;       /**/
	u32 err_gpl_rd:1;       /**/
	u32 err_gpl_rx:1;       /**/
	u32 err_cmd_wr:1;       /**/
	u32 err_pl_wr:1;        /**/
	u32 err_pl_rd:1;        /**/
	u32 err_pl_rx:1;        /**/
	u32 err_ill_cmd:1;      /**/
	u32 rsvd0:14;           /**/
	} bits;
} reg_mipi_dsi_int_pd1_t;

/* Interrupt Clear 0 Register */
typedef union {
	u32 val;
	struct {
	u32 err_ack_sot:1;      /**/
	u32 err_ack_sots:1;     /**/
	u32 err_ack_eots:1;     /**/
	u32 err_ack_ecmd:1;     /**/
	u32 err_ack_lps:1;      /**/
	u32 err_ack_hsto:1;     /**/
	u32 err_ack_fc:1;       /**/
	u32 err_ack_rsvd00:1;   /**/
	u32 err_ack_ecc1:1;     /**/
	u32 err_ack_eccm:1;     /**/
	u32 err_ack_cks:1;      /**/
	u32 err_ack_dt:1;       /**/
	u32 err_ack_vid:1;      /**/
	u32 err_ack_tl:1;       /**/
	u32 err_ack_rsvd11:1;   /**/
	u32 err_ack_pv:1;       /**/
	u32 err_esc:1;          /**/
	u32 err_esc_sync:1;     /**/
	u32 err_ctl:1;          /**/
	u32 err_lp0:1;          /**/
	u32 err_lp1:1;          /**/
	u32 rsvd2:11;           /**/
	} bits;
} reg_mipi_dsi_int_clr0_t;

#define MIPI_DSI_INT_CLR0_ERR_ACK_SOT_0         0x0
#define MIPI_DSI_INT_CLR0_ERR_ACK_SOT_1         0x1
#define MIPI_DSI_INT_CLR0_ERR_ACK_SOTS_0        0x0
#define MIPI_DSI_INT_CLR0_ERR_ACK_SOTS_1        0x1
#define MIPI_DSI_INT_CLR0_ERR_ACK_EOTS_0        0x0
#define MIPI_DSI_INT_CLR0_ERR_ACK_EOTS_1        0x1
#define MIPI_DSI_INT_CLR0_ERR_ACK_ECMD_0        0x0
#define MIPI_DSI_INT_CLR0_ERR_ACK_ECMD_1        0x1
#define MIPI_DSI_INT_CLR0_ERR_ACK_LPS_0         0x0
#define MIPI_DSI_INT_CLR0_ERR_ACK_LPS_1         0x1
#define MIPI_DSI_INT_CLR0_ERR_ACK_HSTO_0        0x0
#define MIPI_DSI_INT_CLR0_ERR_ACK_HSTO_1        0x1
#define MIPI_DSI_INT_CLR0_ERR_ACK_FC_0          0x0
#define MIPI_DSI_INT_CLR0_ERR_ACK_FC_1          0x1
#define MIPI_DSI_INT_CLR0_ERR_ACK_RSVD0_0       0x0
#define MIPI_DSI_INT_CLR0_ERR_ACK_RSVD0_1       0x1
#define MIPI_DSI_INT_CLR0_ERR_ACK_ECC1_0        0x0
#define MIPI_DSI_INT_CLR0_ERR_ACK_ECC1_1        0x1
#define MIPI_DSI_INT_CLR0_ERR_ACK_ECCM_0        0x0
#define MIPI_DSI_INT_CLR0_ERR_ACK_ECCM_1        0x1
#define MIPI_DSI_INT_CLR0_ERR_ACK_CKS_0         0x0
#define MIPI_DSI_INT_CLR0_ERR_ACK_CKS_1         0x1
#define MIPI_DSI_INT_CLR0_ERR_ACK_DT_0          0x0
#define MIPI_DSI_INT_CLR0_ERR_ACK_DT_1          0x1
#define MIPI_DSI_INT_CLR0_ERR_ACK_VID_0         0x0
#define MIPI_DSI_INT_CLR0_ERR_ACK_VID_1         0x1
#define MIPI_DSI_INT_CLR0_ERR_ACK_TL_0          0x0
#define MIPI_DSI_INT_CLR0_ERR_ACK_TL_1          0x1
#define MIPI_DSI_INT_CLR0_ERR_ACK_RSVD1_0       0x0
#define MIPI_DSI_INT_CLR0_ERR_ACK_RSVD1_1       0x1
#define MIPI_DSI_INT_CLR0_ERR_ACK_PV_0          0x0
#define MIPI_DSI_INT_CLR0_ERR_ACK_PV_1          0x1
#define MIPI_DSI_INT_CLR0_ERR_ESC_0             0x0
#define MIPI_DSI_INT_CLR0_ERR_ESC_1             0x1
#define MIPI_DSI_INT_CLR0_ERR_ESC_SYNC_0        0x0
#define MIPI_DSI_INT_CLR0_ERR_ESC_SYNC_1        0x1
#define MIPI_DSI_INT_CLR0_ERR_CTL_0             0x0
#define MIPI_DSI_INT_CLR0_ERR_CTL_1             0x1
#define MIPI_DSI_INT_CLR0_ERR_LP0_0             0x0
#define MIPI_DSI_INT_CLR0_ERR_LP0_1             0x1
#define MIPI_DSI_INT_CLR0_ERR_LP1_0             0x0
#define MIPI_DSI_INT_CLR0_ERR_LP1_1             0x1

/* Interrupt Clear 1 Register */
typedef union {
	u32 val;
	struct {
	u32 err_lptx_to:1;      /**/
	u32 err_lprx_to:1;      /**/
	u32 err_ecc1:1;         /**/
	u32 err_eccm:1;         /**/
	u32 err_crc:1;          /**/
	u32 err_pkt_size:1;     /**/
	u32 err_eotp:1;         /**/
	u32 err_dpi_wr:1;       /**/
	u32 err_gcmd_wr:1;      /**/
	u32 err_gpl_wr:1;       /**/
	u32 err_gpl_tx:1;       /**/
	u32 err_gpl_rd:1;       /**/
	u32 err_gpl_rx:1;       /**/
	u32 err_cmd_wr:1;       /**/
	u32 err_pl_wr:1;        /**/
	u32 err_pl_rd:1;        /**/
	u32 err_pl_rx:1;        /**/
	u32 err_ill_cmd:1;      /**/
	u32 rsvd0:14;           /**/
	} bits;
} reg_mipi_dsi_int_clr1_t;

#define MIPI_DSI_INT_CLR1_ERR_LPTX_TO_0         0x0
#define MIPI_DSI_INT_CLR1_ERR_LPTX_TO_1         0x1
#define MIPI_DSI_INT_CLR1_ERR_LPRX_TO_0         0x0
#define MIPI_DSI_INT_CLR1_ERR_LPRX_TO_1         0x1
#define MIPI_DSI_INT_CLR1_ERR_ECC1_0            0x0
#define MIPI_DSI_INT_CLR1_ERR_ECC1_1            0x1
#define MIPI_DSI_INT_CLR1_ERR_ECCM_0            0x0
#define MIPI_DSI_INT_CLR1_ERR_ECCM_1            0x1
#define MIPI_DSI_INT_CLR1_ERR_CRC_0             0x0
#define MIPI_DSI_INT_CLR1_ERR_CRC_1             0x1
#define MIPI_DSI_INT_CLR1_ERR_PKT_SIZE_0        0x0
#define MIPI_DSI_INT_CLR1_ERR_PKT_SIZE_1        0x1
#define MIPI_DSI_INT_CLR1_ERR_EOTP_0            0x0
#define MIPI_DSI_INT_CLR1_ERR_EOTP_1            0x1
#define MIPI_DSI_INT_CLR1_ERR_DPI_WR_0          0x0
#define MIPI_DSI_INT_CLR1_ERR_DPI_WR_1          0x1
#define MIPI_DSI_INT_CLR1_ERR_GCMD_WR_0         0x0
#define MIPI_DSI_INT_CLR1_ERR_GCMD_WR_1         0x1
#define MIPI_DSI_INT_CLR1_ERR_GPL_WR_0          0x0
#define MIPI_DSI_INT_CLR1_ERR_GPL_WR_1          0x1
#define MIPI_DSI_INT_CLR1_ERR_GPL_TX_0          0x0
#define MIPI_DSI_INT_CLR1_ERR_GPL_TX_1          0x1
#define MIPI_DSI_INT_CLR1_ERR_GPL_RD_0          0x0
#define MIPI_DSI_INT_CLR1_ERR_GPL_RD_1          0x1
#define MIPI_DSI_INT_CLR1_ERR_GPL_RX_0          0x0
#define MIPI_DSI_INT_CLR1_ERR_GPL_RX_1          0x1
#define MIPI_DSI_INT_CLR1_ERR_CMD_WR_0          0x0
#define MIPI_DSI_INT_CLR1_ERR_CMD_WR_1          0x1
#define MIPI_DSI_INT_CLR1_ERR_PL_WR_0           0x0
#define MIPI_DSI_INT_CLR1_ERR_PL_WR_1           0x1
#define MIPI_DSI_INT_CLR1_ERR_PL_RD_0           0x0
#define MIPI_DSI_INT_CLR1_ERR_PL_RD_1           0x1
#define MIPI_DSI_INT_CLR1_ERR_PL_RX_0           0x0
#define MIPI_DSI_INT_CLR1_ERR_PL_RX_1           0x1
#define MIPI_DSI_INT_CLR1_ERR_ILL_CMD_0         0x0
#define MIPI_DSI_INT_CLR1_ERR_ILL_CMD_1         0x1

/* PHY Control Register */
typedef union {
	u32 val;
	struct {
	u32 phy_shut:1; /**/
	u32 phy_rstn:1; /**/
	u32 phy_clk_en:1;/**/
	u32 rsvd0:29;   /**/
	} bits;
} reg_mipi_dsi_phy_ctrl_t;

#define MIPI_DSI_PHY_CTRL_PHY_SHUT_0    0x0
#define MIPI_DSI_PHY_CTRL_PHY_SHUT_1    0x1
#define MIPI_DSI_PHY_CTRL_PHY_RSTN_0    0x0
#define MIPI_DSI_PHY_CTRL_PHY_RSTN_1    0x1
#define MIPI_DSI_PHY_CTRL_PHY_CLK_EN_0  0x0
#define MIPI_DSI_PHY_CTRL_PHY_CLK_EN_1  0x1

/* PHY Interface Configuration Register */
typedef union {
	u32 val;
	struct {
	u32 lane_num:2; /**/
	u32 stop_wt:8;  /**/
	u32 rsvd0:22;   /**/
	} bits;
} reg_mipi_dsi_phy_if_cfg_t;

#define MIPI_DSI_PHY_IF_CFG_LANE_NUM_0  0x0
#define MIPI_DSI_PHY_IF_CFG_LANE_NUM_1  0x1
#define MIPI_DSI_PHY_IF_CFG_LANE_NUM_2  0x2
#define MIPI_DSI_PHY_IF_CFG_LANE_NUM_3  0x3

#define MIPI_DSI_PHY_TST_CTRL1_TEST_EN_0        0x0
#define MIPI_DSI_PHY_TST_CTRL1_TEST_EN_1        0x1

/* reg_mipi_dsi_t bank */
typedef struct tag_mipi_dsi {
	reg_mipi_dsi_ver_t             ver;        /* 0000 */
	reg_mipi_dsi_host_ctrl_t       host_ctrl;  /* 0004 */
	reg_mipi_dsi_clk_div_t         clk_div;    /* 0008 */
	reg_mipi_dsi_dpi_cfg_t         dpi_cfg;    /* 000C */
	reg_mipi_dsi_dbi_cfg_t         dbi_cfg;    /* 0010 */
	reg_mipi_dsi_cmd_size_t        cmd_size;   /* 0014 */
	reg_mipi_dsi_pkt_cfg_t         pkt_cfg;    /* 0018 */
	reg_mipi_dsi_vm_cfg_t          vm_cfg;     /* 001C */
	reg_mipi_dsi_vm_pkt_cfg_t      vm_pkt_cfg; /* 0020 */
	reg_mipi_dsi_cm_cfg_t          cm_cfg;     /* 0024 */
	reg_mipi_dsi_lt_cfg_t          lt_cfg;     /* 0028 */
	reg_mipi_dsi_vt_cfg_t          vt_cfg;     /* 002C */
	reg_mipi_dsi_phy_tim_t         phy_tim;    /* 0030 */
	reg_mipi_dsi_gen_ph_cfg_t      gen_ph_cfg; /* 0034 */
	reg_mipi_dsi_gen_pl_data_t     gen_pl_data;/* 0038 */
	reg_mipi_dsi_cmd_pkt_sta_t     cmd_pkt_sta;/* 003C */
	reg_mipi_dsi_to_cnt_cfg_t      to_cnt_cfg; /* 0040 */
	u32                            res0[1];
	reg_mipi_dsi_int_en0_t         int_en0;    /* 0048 */
	reg_mipi_dsi_int_en1_t         int_en1;    /* 004C */
	reg_mipi_dsi_int_pd0_t         int_pd0;    /* 0050 */
	reg_mipi_dsi_int_pd1_t         int_pd1;    /* 0054 */
	reg_mipi_dsi_int_clr0_t        int_clr0;   /* 0058 */
	reg_mipi_dsi_int_clr1_t        int_clr1;   /* 005C */
	reg_mipi_dsi_phy_ctrl_t        phy_ctrl;   /* 0060 */
	reg_mipi_dsi_phy_if_cfg_t      phy_if_cfg; /* 0064 */
	u32                            prv_0068;   /* 0068 */
	u32                            prv_006C;   /* 006C */
	u32                            prv_0070;   /* 0070 */
	u32                            prv_0074;   /* 0074 */
} reg_mipi_dsi_t;

#endif /* ___MIPI_DSI___H___ */
