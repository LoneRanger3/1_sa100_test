/* dpu_get.h */

#ifndef ___DPU__GET___H___
#define ___DPU__GET___H___

#ifdef DEF_GET_DPU_VER_VER_L
def_get_mod_reg_bit(dpu, ver, ver_l, addr);
#endif

#ifdef DEF_GET_DPU_VER_VER_H
def_get_mod_reg_bit(dpu, ver, ver_h, addr);
#endif

#ifdef DEF_GET_DPU_VER_COMP
def_get_mod_reg_bit(dpu, ver, comp, addr);
#endif

#ifdef DEF_GET_DPU_FTR_HAS_DC0
def_get_mod_reg_bit(dpu, ftr, has_dc0, addr);
#endif

#ifdef DEF_GET_DPU_FTR_HAS_DC1
def_get_mod_reg_bit(dpu, ftr, has_dc1, addr);
#endif

#ifdef DEF_GET_DPU_FTR_HAS_SE0
def_get_mod_reg_bit(dpu, ftr, has_se0, addr);
#endif

#ifdef DEF_GET_DPU_FTR_HAS_SE1
def_get_mod_reg_bit(dpu, ftr, has_se1, addr);
#endif

#ifdef DEF_GET_DPU_FTR_HAS_ROT
def_get_mod_reg_bit(dpu, ftr, has_rot, addr);
#endif

#ifdef DEF_GET_DPU_FTR_HAS_DIT
def_get_mod_reg_bit(dpu, ftr, has_dit, addr);
#endif

#ifdef DEF_GET_DPU_CTL_DC0_RST
def_get_mod_reg_bit(dpu, ctl, dc0_rst, addr);
#endif

#ifdef DEF_GET_DPU_CTL_DC1_RST
def_get_mod_reg_bit(dpu, ctl, dc1_rst, addr);
#endif

#ifdef DEF_GET_DPU_CTL_SE0_RST
def_get_mod_reg_bit(dpu, ctl, se0_rst, addr);
#endif

#ifdef DEF_GET_DPU_CTL_SE1_RST
def_get_mod_reg_bit(dpu, ctl, se1_rst, addr);
#endif

#ifdef DEF_GET_DPU_CTL_ROT_RST
def_get_mod_reg_bit(dpu, ctl, rot_rst, addr);
#endif

#ifdef DEF_GET_DPU_CTL_DIT_RST
def_get_mod_reg_bit(dpu, ctl, dit_rst, addr);
#endif

#ifdef DEF_GET_DPU_CTL_TOP_RST
def_get_mod_reg_bit(dpu, ctl, top_rst, addr);
#endif

#ifdef DEF_GET_DPU_CLK_SRC_DC0_CS
def_get_mod_reg_bit(dpu, clk_src, dc0_cs, addr);
#endif

#ifdef DEF_GET_DPU_CLK_SRC_DC1_CS
def_get_mod_reg_bit(dpu, clk_src, dc1_cs, addr);
#endif

#ifdef DEF_GET_DPU_CLK_SRC_SE0_CS
def_get_mod_reg_bit(dpu, clk_src, se0_cs, addr);
#endif

#ifdef DEF_GET_DPU_CLK_SRC_SE1_CS
def_get_mod_reg_bit(dpu, clk_src, se1_cs, addr);
#endif

#ifdef DEF_GET_DPU_CLK_SRC_ROT_CS
def_get_mod_reg_bit(dpu, clk_src, rot_cs, addr);
#endif

#ifdef DEF_GET_DPU_CLK_SRC_DIT_CS
def_get_mod_reg_bit(dpu, clk_src, dit_cs, addr);
#endif

#ifdef DEF_GET_DPU_CLK_DIV_DC0_DIV
def_get_mod_reg_bit(dpu, clk_div, dc0_div, addr);
#endif

#ifdef DEF_GET_DPU_CLK_DIV_DC1_DIV
def_get_mod_reg_bit(dpu, clk_div, dc1_div, addr);
#endif

#ifdef DEF_GET_DPU_CLK_DIV_SE0_DIV
def_get_mod_reg_bit(dpu, clk_div, se0_div, addr);
#endif

#ifdef DEF_GET_DPU_CLK_DIV_SE1_DIV
def_get_mod_reg_bit(dpu, clk_div, se1_div, addr);
#endif

#ifdef DEF_GET_DPU_CLK_DIV_ROT_DIV
def_get_mod_reg_bit(dpu, clk_div, rot_div, addr);
#endif

#ifdef DEF_GET_DPU_CLK_DIV_DIT_DIV
def_get_mod_reg_bit(dpu, clk_div, dit_div, addr);
#endif

#ifdef DEF_GET_DPU_CLK_DIV_TCK_DIV
def_get_mod_reg_bit(dpu, clk_div, tck_div, addr);
#endif

#ifdef DEF_GET_DPU_CLK_GATING_DC0_GT
def_get_mod_reg_bit(dpu, clk_gating, dc0_gt, addr);
#endif

#ifdef DEF_GET_DPU_CLK_GATING_DC1_GT
def_get_mod_reg_bit(dpu, clk_gating, dc1_gt, addr);
#endif

#ifdef DEF_GET_DPU_CLK_GATING_SE0_GT
def_get_mod_reg_bit(dpu, clk_gating, se0_gt, addr);
#endif

#ifdef DEF_GET_DPU_CLK_GATING_SE1_GT
def_get_mod_reg_bit(dpu, clk_gating, se1_gt, addr);
#endif

#ifdef DEF_GET_DPU_CLK_GATING_ROT_GT
def_get_mod_reg_bit(dpu, clk_gating, rot_gt, addr);
#endif

#ifdef DEF_GET_DPU_CLK_GATING_DIT_GT
def_get_mod_reg_bit(dpu, clk_gating, dit_gt, addr);
#endif

#ifdef DEF_GET_DPU_CLK_GATING_TCK_GT
def_get_mod_reg_bit(dpu, clk_gating, tck_gt, addr);
#endif

#ifdef DEF_GET_DPU_ROUTINE_SE0_OUT_SEL
def_get_mod_reg_bit(dpu, routine, se0_out_sel, addr);
#endif

#ifdef DEF_GET_DPU_ROUTINE_SE1_OUT_SEL
def_get_mod_reg_bit(dpu, routine, se1_out_sel, addr);
#endif

#ifdef DEF_GET_DPU_ROUTINE_SE_IN_SEL
def_get_mod_reg_bit(dpu, routine, se_in_sel, addr);
#endif

#ifdef DEF_GET_DPU_ROUTINE_DC0SYNC_SEL
def_get_mod_reg_bit(dpu, routine, dc0sync_sel, addr);
#endif

#ifdef DEF_GET_DPU_ROUTINE_DC1SYNC_SEL
def_get_mod_reg_bit(dpu, routine, dc1sync_sel, addr);
#endif

#ifdef DEF_GET_DPU_SCLK_SEL_SCLK0_SEL
def_get_mod_reg_bit(dpu, sclk_sel, sclk0_sel, addr);
#endif

#ifdef DEF_GET_DPU_SCLK_SEL_SCLK1_SEL
def_get_mod_reg_bit(dpu, sclk_sel, sclk1_sel, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL0_CTL_N
def_get_mod_reg_bit(dpu, adpll0_ctl, n, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL0_CTL_EN
def_get_mod_reg_bit(dpu, adpll0_ctl, en, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL0_TUNE0_TUNE0
def_get_mod_reg_bit(dpu, adpll0_tune0, tune0, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL0_TUNE0_TUNE1
def_get_mod_reg_bit(dpu, adpll0_tune0, tune1, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL0_TUNE0_AUTO_ADRANGE
def_get_mod_reg_bit(dpu, adpll0_tune0, auto_adrange, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL0_TUNE0_TUNE2
def_get_mod_reg_bit(dpu, adpll0_tune0, tune2, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL0_TUNE0_TUNE3
def_get_mod_reg_bit(dpu, adpll0_tune0, tune3, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL0_TUNE0_TUNE4
def_get_mod_reg_bit(dpu, adpll0_tune0, tune4, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL0_TUNE1_TUNE0
def_get_mod_reg_bit(dpu, adpll0_tune1, tune0, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL0_TUNE1_TUNE1
def_get_mod_reg_bit(dpu, adpll0_tune1, tune1, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL0_TUNE1_TUNE2
def_get_mod_reg_bit(dpu, adpll0_tune1, tune2, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL0_TUNE1_TUNE3
def_get_mod_reg_bit(dpu, adpll0_tune1, tune3, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL0_TUNE1_SLEW_RATE
def_get_mod_reg_bit(dpu, adpll0_tune1, slew_rate, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL0_TUNE1_TUNE4
def_get_mod_reg_bit(dpu, adpll0_tune1, tune4, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL0_TUNE1_TUNE5
def_get_mod_reg_bit(dpu, adpll0_tune1, tune5, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL0_TUNE1_TUNE6
def_get_mod_reg_bit(dpu, adpll0_tune1, tune6, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL0_TUNE1_TUNE7
def_get_mod_reg_bit(dpu, adpll0_tune1, tune7, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL0_STAT_FERR
def_get_mod_reg_bit(dpu, adpll0_stat, ferr, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL0_STAT_FLOCK
def_get_mod_reg_bit(dpu, adpll0_stat, flock, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL0_STAT_PERR
def_get_mod_reg_bit(dpu, adpll0_stat, perr, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL0_STAT_PLOCK
def_get_mod_reg_bit(dpu, adpll0_stat, plock, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL1_CTL_N
def_get_mod_reg_bit(dpu, adpll1_ctl, n, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL1_CTL_EN
def_get_mod_reg_bit(dpu, adpll1_ctl, en, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL1_TUNE0_TUNE0
def_get_mod_reg_bit(dpu, adpll1_tune0, tune0, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL1_TUNE0_TUNE1
def_get_mod_reg_bit(dpu, adpll1_tune0, tune1, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL1_TUNE0_AUTO_ADRANGE
def_get_mod_reg_bit(dpu, adpll1_tune0, auto_adrange, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL1_TUNE0_TUNE2
def_get_mod_reg_bit(dpu, adpll1_tune0, tune2, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL1_TUNE0_TUNE3
def_get_mod_reg_bit(dpu, adpll1_tune0, tune3, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL1_TUNE0_TUNE4
def_get_mod_reg_bit(dpu, adpll1_tune0, tune4, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL1_TUNE1_TUNE0
def_get_mod_reg_bit(dpu, adpll1_tune1, tune0, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL1_TUNE1_TUNE1
def_get_mod_reg_bit(dpu, adpll1_tune1, tune1, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL1_TUNE1_TUNE2
def_get_mod_reg_bit(dpu, adpll1_tune1, tune2, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL1_TUNE1_TUNE3
def_get_mod_reg_bit(dpu, adpll1_tune1, tune3, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL1_TUNE1_SLEW_RATE
def_get_mod_reg_bit(dpu, adpll1_tune1, slew_rate, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL1_TUNE1_TUNE4
def_get_mod_reg_bit(dpu, adpll1_tune1, tune4, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL1_TUNE1_TUNE5
def_get_mod_reg_bit(dpu, adpll1_tune1, tune5, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL1_TUNE1_TUNE6
def_get_mod_reg_bit(dpu, adpll1_tune1, tune6, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL1_TUNE1_TUNE7
def_get_mod_reg_bit(dpu, adpll1_tune1, tune7, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL1_STAT_FERR
def_get_mod_reg_bit(dpu, adpll1_stat, ferr, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL1_STAT_FLOCK
def_get_mod_reg_bit(dpu, adpll1_stat, flock, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL1_STAT_PERR
def_get_mod_reg_bit(dpu, adpll1_stat, perr, addr);
#endif

#ifdef DEF_GET_DPU_ADPLL1_STAT_PLOCK
def_get_mod_reg_bit(dpu, adpll1_stat, plock, addr);
#endif

#endif /* ___DPU__GET___H___ */
