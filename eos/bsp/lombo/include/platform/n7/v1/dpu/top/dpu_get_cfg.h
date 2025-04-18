/* dpu_get_cfg.h */

#ifndef ___DPU__GET_CFG__H___
#define ___DPU__GET_CFG__H___

/*
 * #define DEF_GET_DPU_VER_VER_L
 * #define DEF_GET_DPU_VER_VER_H
 * #define DEF_GET_DPU_VER_COMP
 * #define DEF_GET_DPU_FTR_HAS_DC0
 * #define DEF_GET_DPU_FTR_HAS_DC1
 * #define DEF_GET_DPU_FTR_HAS_SE0
 * #define DEF_GET_DPU_FTR_HAS_SE1
 * #define DEF_GET_DPU_FTR_HAS_ROT
 * #define DEF_GET_DPU_FTR_HAS_DIT
 * #define DEF_GET_DPU_CTL_DC0_RST
 * #define DEF_GET_DPU_CTL_DC1_RST
 * #define DEF_GET_DPU_CTL_SE0_RST
 * #define DEF_GET_DPU_CTL_SE1_RST
 * #define DEF_GET_DPU_CTL_ROT_RST
 * #define DEF_GET_DPU_CTL_DIT_RST
 * #define DEF_GET_DPU_CTL_TOP_RST
 * #define DEF_GET_DPU_CLK_SRC_DC0_CS
 * #define DEF_GET_DPU_CLK_SRC_DC1_CS
 * #define DEF_GET_DPU_CLK_SRC_SE0_CS
 * #define DEF_GET_DPU_CLK_SRC_SE1_CS
 * #define DEF_GET_DPU_CLK_SRC_ROT_CS
 * #define DEF_GET_DPU_CLK_SRC_DIT_CS
 * #define DEF_GET_DPU_CLK_DIV_DC0_DIV
 * #define DEF_GET_DPU_CLK_DIV_DC1_DIV
 * #define DEF_GET_DPU_CLK_DIV_SE0_DIV
 * #define DEF_GET_DPU_CLK_DIV_SE1_DIV
 * #define DEF_GET_DPU_CLK_DIV_ROT_DIV
 * #define DEF_GET_DPU_CLK_DIV_DIT_DIV
 * #define DEF_GET_DPU_CLK_DIV_TCK_DIV
 * #define DEF_GET_DPU_CLK_GATING_DC0_GT
 * #define DEF_GET_DPU_CLK_GATING_DC1_GT
 * #define DEF_GET_DPU_CLK_GATING_SE0_GT
 * #define DEF_GET_DPU_CLK_GATING_SE1_GT
 * #define DEF_GET_DPU_CLK_GATING_ROT_GT
 * #define DEF_GET_DPU_CLK_GATING_DIT_GT
 * #define DEF_GET_DPU_CLK_GATING_TCK_GT
 * #define DEF_GET_DPU_ROUTINE_SE0_OUT_SEL
 * #define DEF_GET_DPU_ROUTINE_SE1_OUT_SEL
 * #define DEF_GET_DPU_ROUTINE_SE_IN_SEL
 * #define DEF_GET_DPU_ROUTINE_DC0SYNC_SEL
 * #define DEF_GET_DPU_ROUTINE_DC1SYNC_SEL
 * #define DEF_GET_DPU_SCLK_SEL_SCLK0_SEL
 * #define DEF_GET_DPU_SCLK_SEL_SCLK1_SEL
 * #define DEF_GET_DPU_ADPLL0_CTL_N
 * #define DEF_GET_DPU_ADPLL0_CTL_EN
 * #define DEF_GET_DPU_ADPLL0_TUNE0_TUNE0
 * #define DEF_GET_DPU_ADPLL0_TUNE0_TUNE1
 * #define DEF_GET_DPU_ADPLL0_TUNE0_AUTO_ADRANGE
 * #define DEF_GET_DPU_ADPLL0_TUNE0_TUNE2
 * #define DEF_GET_DPU_ADPLL0_TUNE0_TUNE3
 * #define DEF_GET_DPU_ADPLL0_TUNE0_TUNE4
 * #define DEF_GET_DPU_ADPLL0_TUNE1_TUNE0
 * #define DEF_GET_DPU_ADPLL0_TUNE1_TUNE1
 * #define DEF_GET_DPU_ADPLL0_TUNE1_TUNE2
 * #define DEF_GET_DPU_ADPLL0_TUNE1_TUNE3
 * #define DEF_GET_DPU_ADPLL0_TUNE1_SLEW_RATE
 * #define DEF_GET_DPU_ADPLL0_TUNE1_TUNE4
 * #define DEF_GET_DPU_ADPLL0_TUNE1_TUNE5
 * #define DEF_GET_DPU_ADPLL0_TUNE1_TUNE6
 * #define DEF_GET_DPU_ADPLL0_TUNE1_TUNE7
 * #define DEF_GET_DPU_ADPLL0_STAT_FERR
 * #define DEF_GET_DPU_ADPLL0_STAT_FLOCK
 * #define DEF_GET_DPU_ADPLL0_STAT_PERR
 * #define DEF_GET_DPU_ADPLL0_STAT_PLOCK
 * #define DEF_GET_DPU_ADPLL1_CTL_N
 * #define DEF_GET_DPU_ADPLL1_CTL_EN
 * #define DEF_GET_DPU_ADPLL1_TUNE0_TUNE0
 * #define DEF_GET_DPU_ADPLL1_TUNE0_TUNE1
 * #define DEF_GET_DPU_ADPLL1_TUNE0_AUTO_ADRANGE
 * #define DEF_GET_DPU_ADPLL1_TUNE0_TUNE2
 * #define DEF_GET_DPU_ADPLL1_TUNE0_TUNE3
 * #define DEF_GET_DPU_ADPLL1_TUNE0_TUNE4
 * #define DEF_GET_DPU_ADPLL1_TUNE1_TUNE0
 * #define DEF_GET_DPU_ADPLL1_TUNE1_TUNE1
 * #define DEF_GET_DPU_ADPLL1_TUNE1_TUNE2
 * #define DEF_GET_DPU_ADPLL1_TUNE1_TUNE3
 * #define DEF_GET_DPU_ADPLL1_TUNE1_SLEW_RATE
 * #define DEF_GET_DPU_ADPLL1_TUNE1_TUNE4
 * #define DEF_GET_DPU_ADPLL1_TUNE1_TUNE5
 * #define DEF_GET_DPU_ADPLL1_TUNE1_TUNE6
 * #define DEF_GET_DPU_ADPLL1_TUNE1_TUNE7
 * #define DEF_GET_DPU_ADPLL1_STAT_FERR
 * #define DEF_GET_DPU_ADPLL1_STAT_FLOCK
 * #define DEF_GET_DPU_ADPLL1_STAT_PERR
 * #define DEF_GET_DPU_ADPLL1_STAT_PLOCK
*/

#endif /* ___DPU__GET_CFG__H___ */
