/* dpu_set.h */

#ifndef ___DPU__SET___H___
#define ___DPU__SET___H___

#ifdef DEF_SET_DPU_VER
void set_dpu_ver(u32 reg_addr,
		u32 ver_l,
		u32 ver_h,
		u32 comp,
		u32 m_or_r);
#endif

#ifdef DEF_SET_DPU_VER_VER_L
def_set_mod_reg_bit(dpu, ver, ver_l, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_VER_VER_H
def_set_mod_reg_bit(dpu, ver, ver_h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_VER_COMP
def_set_mod_reg_bit(dpu, ver, comp, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_FTR
void set_dpu_ftr(u32 reg_addr,
		u32 has_dc0,
		u32 has_dc1,
		u32 has_se0,
		u32 has_se1,
		u32 has_rot,
		u32 has_dit,
		u32 m_or_r);
#endif

#ifdef DEF_SET_DPU_FTR_HAS_DC0
def_set_mod_reg_bit(dpu, ftr, has_dc0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_FTR_HAS_DC1
def_set_mod_reg_bit(dpu, ftr, has_dc1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_FTR_HAS_SE0
def_set_mod_reg_bit(dpu, ftr, has_se0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_FTR_HAS_SE1
def_set_mod_reg_bit(dpu, ftr, has_se1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_FTR_HAS_ROT
def_set_mod_reg_bit(dpu, ftr, has_rot, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_FTR_HAS_DIT
def_set_mod_reg_bit(dpu, ftr, has_dit, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_CTL
void set_dpu_ctl(u32 reg_addr,
		u32 dc0_rst,
		u32 dc1_rst,
		u32 se0_rst,
		u32 se1_rst,
		u32 rot_rst,
		u32 dit_rst,
		u32 top_rst,
		u32 m_or_r);
#endif

#ifdef DEF_SET_DPU_CTL_DC0_RST
def_set_mod_reg_bit(dpu, ctl, dc0_rst, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_CTL_DC1_RST
def_set_mod_reg_bit(dpu, ctl, dc1_rst, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_CTL_SE0_RST
def_set_mod_reg_bit(dpu, ctl, se0_rst, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_CTL_SE1_RST
def_set_mod_reg_bit(dpu, ctl, se1_rst, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_CTL_ROT_RST
def_set_mod_reg_bit(dpu, ctl, rot_rst, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_CTL_DIT_RST
def_set_mod_reg_bit(dpu, ctl, dit_rst, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_CTL_TOP_RST
def_set_mod_reg_bit(dpu, ctl, top_rst, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_CLK_SRC
void set_dpu_clk_src(u32 reg_addr,
			u32 dc0_cs,
			u32 dc1_cs,
			u32 se0_cs,
			u32 se1_cs,
			u32 rot_cs,
			u32 dit_cs,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DPU_CLK_SRC_DC0_CS
def_set_mod_reg_bit(dpu, clk_src, dc0_cs, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_CLK_SRC_DC1_CS
def_set_mod_reg_bit(dpu, clk_src, dc1_cs, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_CLK_SRC_SE0_CS
def_set_mod_reg_bit(dpu, clk_src, se0_cs, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_CLK_SRC_SE1_CS
def_set_mod_reg_bit(dpu, clk_src, se1_cs, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_CLK_SRC_ROT_CS
def_set_mod_reg_bit(dpu, clk_src, rot_cs, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_CLK_SRC_DIT_CS
def_set_mod_reg_bit(dpu, clk_src, dit_cs, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_CLK_DIV
void set_dpu_clk_div(u32 reg_addr,
			u32 dc0_div,
			u32 dc1_div,
			u32 se0_div,
			u32 se1_div,
			u32 rot_div,
			u32 dit_div,
			u32 tck_div,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DPU_CLK_DIV_DC0_DIV
def_set_mod_reg_bit(dpu, clk_div, dc0_div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_CLK_DIV_DC1_DIV
def_set_mod_reg_bit(dpu, clk_div, dc1_div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_CLK_DIV_SE0_DIV
def_set_mod_reg_bit(dpu, clk_div, se0_div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_CLK_DIV_SE1_DIV
def_set_mod_reg_bit(dpu, clk_div, se1_div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_CLK_DIV_ROT_DIV
def_set_mod_reg_bit(dpu, clk_div, rot_div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_CLK_DIV_DIT_DIV
def_set_mod_reg_bit(dpu, clk_div, dit_div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_CLK_DIV_TCK_DIV
def_set_mod_reg_bit(dpu, clk_div, tck_div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_CLK_GATING
void set_dpu_clk_gating(u32 reg_addr,
			u32 dc0_gt,
			u32 dc1_gt,
			u32 se0_gt,
			u32 se1_gt,
			u32 rot_gt,
			u32 dit_gt,
			u32 tck_gt,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DPU_CLK_GATING_DC0_GT
def_set_mod_reg_bit(dpu, clk_gating, dc0_gt, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_CLK_GATING_DC1_GT
def_set_mod_reg_bit(dpu, clk_gating, dc1_gt, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_CLK_GATING_SE0_GT
def_set_mod_reg_bit(dpu, clk_gating, se0_gt, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_CLK_GATING_SE1_GT
def_set_mod_reg_bit(dpu, clk_gating, se1_gt, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_CLK_GATING_ROT_GT
def_set_mod_reg_bit(dpu, clk_gating, rot_gt, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_CLK_GATING_DIT_GT
def_set_mod_reg_bit(dpu, clk_gating, dit_gt, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_CLK_GATING_TCK_GT
def_set_mod_reg_bit(dpu, clk_gating, tck_gt, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ROUTINE
void set_dpu_routine(u32 reg_addr,
			u32 se0_out_sel,
			u32 se1_out_sel,
			u32 se_in_sel,
			u32 dc0sync_sel,
			u32 dc1sync_sel,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DPU_ROUTINE_SE0_OUT_SEL
def_set_mod_reg_bit(dpu, routine, se0_out_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ROUTINE_SE1_OUT_SEL
def_set_mod_reg_bit(dpu, routine, se1_out_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ROUTINE_SE_IN_SEL
def_set_mod_reg_bit(dpu, routine, se_in_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ROUTINE_DC0SYNC_SEL
def_set_mod_reg_bit(dpu, routine, dc0sync_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ROUTINE_DC1SYNC_SEL
def_set_mod_reg_bit(dpu, routine, dc1sync_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_SCLK_SEL
void set_dpu_sclk_sel(u32 reg_addr,
			u32 sclk0_sel,
			u32 sclk1_sel,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DPU_SCLK_SEL_SCLK0_SEL
def_set_mod_reg_bit(dpu, sclk_sel, sclk0_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_SCLK_SEL_SCLK1_SEL
def_set_mod_reg_bit(dpu, sclk_sel, sclk1_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL0_CTL
void set_dpu_adpll0_ctl(u32 reg_addr,
			u32 n,
			u32 en,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL0_CTL_N
def_set_mod_reg_bit(dpu, adpll0_ctl, n, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL0_CTL_EN
def_set_mod_reg_bit(dpu, adpll0_ctl, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL0_TUNE0
void set_dpu_adpll0_tune0(u32 reg_addr,
			u32 tune0,
			u32 tune1,
			u32 auto_adrange,
			u32 tune2,
			u32 tune3,
			u32 tune4,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL0_TUNE0_TUNE0
def_set_mod_reg_bit(dpu, adpll0_tune0, tune0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL0_TUNE0_TUNE1
def_set_mod_reg_bit(dpu, adpll0_tune0, tune1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL0_TUNE0_AUTO_ADRANGE
def_set_mod_reg_bit(dpu, adpll0_tune0, auto_adrange, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL0_TUNE0_TUNE2
def_set_mod_reg_bit(dpu, adpll0_tune0, tune2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL0_TUNE0_TUNE3
def_set_mod_reg_bit(dpu, adpll0_tune0, tune3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL0_TUNE0_TUNE4
def_set_mod_reg_bit(dpu, adpll0_tune0, tune4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL0_TUNE1
void set_dpu_adpll0_tune1(u32 reg_addr,
			u32 tune0,
			u32 tune1,
			u32 tune2,
			u32 tune3,
			u32 slew_rate,
			u32 tune4,
			u32 tune5,
			u32 tune6,
			u32 tune7,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL0_TUNE1_TUNE0
def_set_mod_reg_bit(dpu, adpll0_tune1, tune0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL0_TUNE1_TUNE1
def_set_mod_reg_bit(dpu, adpll0_tune1, tune1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL0_TUNE1_TUNE2
def_set_mod_reg_bit(dpu, adpll0_tune1, tune2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL0_TUNE1_TUNE3
def_set_mod_reg_bit(dpu, adpll0_tune1, tune3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL0_TUNE1_SLEW_RATE
def_set_mod_reg_bit(dpu, adpll0_tune1, slew_rate, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL0_TUNE1_TUNE4
def_set_mod_reg_bit(dpu, adpll0_tune1, tune4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL0_TUNE1_TUNE5
def_set_mod_reg_bit(dpu, adpll0_tune1, tune5, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL0_TUNE1_TUNE6
def_set_mod_reg_bit(dpu, adpll0_tune1, tune6, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL0_TUNE1_TUNE7
def_set_mod_reg_bit(dpu, adpll0_tune1, tune7, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL0_STAT
void set_dpu_adpll0_stat(u32 reg_addr,
			u32 ferr,
			u32 flock,
			u32 perr,
			u32 plock,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL0_STAT_FERR
def_set_mod_reg_bit(dpu, adpll0_stat, ferr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL0_STAT_FLOCK
def_set_mod_reg_bit(dpu, adpll0_stat, flock, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL0_STAT_PERR
def_set_mod_reg_bit(dpu, adpll0_stat, perr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL0_STAT_PLOCK
def_set_mod_reg_bit(dpu, adpll0_stat, plock, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL1_CTL
void set_dpu_adpll1_ctl(u32 reg_addr,
			u32 n,
			u32 en,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL1_CTL_N
def_set_mod_reg_bit(dpu, adpll1_ctl, n, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL1_CTL_EN
def_set_mod_reg_bit(dpu, adpll1_ctl, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL1_TUNE0
void set_dpu_adpll1_tune0(u32 reg_addr,
			u32 tune0,
			u32 tune1,
			u32 auto_adrange,
			u32 tune2,
			u32 tune3,
			u32 tune4,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL1_TUNE0_TUNE0
def_set_mod_reg_bit(dpu, adpll1_tune0, tune0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL1_TUNE0_TUNE1
def_set_mod_reg_bit(dpu, adpll1_tune0, tune1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL1_TUNE0_AUTO_ADRANGE
def_set_mod_reg_bit(dpu, adpll1_tune0, auto_adrange, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL1_TUNE0_TUNE2
def_set_mod_reg_bit(dpu, adpll1_tune0, tune2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL1_TUNE0_TUNE3
def_set_mod_reg_bit(dpu, adpll1_tune0, tune3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL1_TUNE0_TUNE4
def_set_mod_reg_bit(dpu, adpll1_tune0, tune4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL1_TUNE1
void set_dpu_adpll1_tune1(u32 reg_addr,
			u32 tune0,
			u32 tune1,
			u32 tune2,
			u32 tune3,
			u32 slew_rate,
			u32 tune4,
			u32 tune5,
			u32 tune6,
			u32 tune7,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL1_TUNE1_TUNE0
def_set_mod_reg_bit(dpu, adpll1_tune1, tune0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL1_TUNE1_TUNE1
def_set_mod_reg_bit(dpu, adpll1_tune1, tune1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL1_TUNE1_TUNE2
def_set_mod_reg_bit(dpu, adpll1_tune1, tune2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL1_TUNE1_TUNE3
def_set_mod_reg_bit(dpu, adpll1_tune1, tune3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL1_TUNE1_SLEW_RATE
def_set_mod_reg_bit(dpu, adpll1_tune1, slew_rate, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL1_TUNE1_TUNE4
def_set_mod_reg_bit(dpu, adpll1_tune1, tune4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL1_TUNE1_TUNE5
def_set_mod_reg_bit(dpu, adpll1_tune1, tune5, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL1_TUNE1_TUNE6
def_set_mod_reg_bit(dpu, adpll1_tune1, tune6, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL1_TUNE1_TUNE7
def_set_mod_reg_bit(dpu, adpll1_tune1, tune7, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL1_STAT
void set_dpu_adpll1_stat(u32 reg_addr,
			u32 ferr,
			u32 flock,
			u32 perr,
			u32 plock,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL1_STAT_FERR
def_set_mod_reg_bit(dpu, adpll1_stat, ferr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL1_STAT_FLOCK
def_set_mod_reg_bit(dpu, adpll1_stat, flock, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL1_STAT_PERR
def_set_mod_reg_bit(dpu, adpll1_stat, perr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DPU_ADPLL1_STAT_PLOCK
def_set_mod_reg_bit(dpu, adpll1_stat, plock, addr, val, m_or_r);
#endif

#endif /* ___DPU__SET___H___ */
