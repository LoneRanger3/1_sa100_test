/* cpu_ctrl_get.h */

#ifndef ___CPU_CTRL__GET___H___
#define ___CPU_CTRL__GET___H___

#ifdef DEF_GET_CPU_CTRL_RST_CTRL_NCORERESET0
def_get_mod_reg_bit(cpu_ctrl, rst_ctrl, ncorereset0, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_RST_CTRL_NCORERESET1
def_get_mod_reg_bit(cpu_ctrl, rst_ctrl, ncorereset1, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_RST_CTRL_NCOREPORESET0
def_get_mod_reg_bit(cpu_ctrl, rst_ctrl, ncoreporeset0, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_RST_CTRL_NCOREPORESET1
def_get_mod_reg_bit(cpu_ctrl, rst_ctrl, ncoreporeset1, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_RST_CTRL_NDBGRESET0
def_get_mod_reg_bit(cpu_ctrl, rst_ctrl, ndbgreset0, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_RST_CTRL_NDBGRESET1
def_get_mod_reg_bit(cpu_ctrl, rst_ctrl, ndbgreset1, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_RST_CTRL_L1RSTDISABLE0
def_get_mod_reg_bit(cpu_ctrl, rst_ctrl, l1rstdisable0, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_RST_CTRL_L1RSTDISABLE1
def_get_mod_reg_bit(cpu_ctrl, rst_ctrl, l1rstdisable1, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_CTRL_R_EVENTI
def_get_mod_reg_bit(cpu_ctrl, ctrl_r, eventi, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_CTRL_R_CFGSDISABLE
def_get_mod_reg_bit(cpu_ctrl, ctrl_r, cfgsdisable, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_CTRL_R_CP15SDISABLE0
def_get_mod_reg_bit(cpu_ctrl, ctrl_r, cp15sdisable0, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_CTRL_R_CP15SDISABLE1
def_get_mod_reg_bit(cpu_ctrl, ctrl_r, cp15sdisable1, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_CTRL_R_CPU1_GAT
def_get_mod_reg_bit(cpu_ctrl, ctrl_r, cpu1_gat, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_CTRL_R_ACINACTM
def_get_mod_reg_bit(cpu_ctrl, ctrl_r, acinactm, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_CFG_R_BROADCASTINNER
def_get_mod_reg_bit(cpu_ctrl, cfg_r, broadcastinner, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_CFG_R_BROADCASTOUTER
def_get_mod_reg_bit(cpu_ctrl, cfg_r, broadcastouter, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_CFG_R_BROADCASTCACHEMAINT
def_get_mod_reg_bit(cpu_ctrl, cfg_r, broadcastcachemaint, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_CFG_R_SYSBARDISABLE
def_get_mod_reg_bit(cpu_ctrl, cfg_r, sysbardisable, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_STATUS_R_SMPNAMP0
def_get_mod_reg_bit(cpu_ctrl, status_r, smpnamp0, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_STATUS_R_SMPNAMP1
def_get_mod_reg_bit(cpu_ctrl, status_r, smpnamp1, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_STATUS_R_STANDBYWFE0
def_get_mod_reg_bit(cpu_ctrl, status_r, standbywfe0, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_STATUS_R_STANDBYWFE1
def_get_mod_reg_bit(cpu_ctrl, status_r, standbywfe1, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_STATUS_R_STANDBYWFI0
def_get_mod_reg_bit(cpu_ctrl, status_r, standbywfi0, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_STATUS_R_STANDBYWFI1
def_get_mod_reg_bit(cpu_ctrl, status_r, standbywfi1, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_STATUS_R_STANDBYWFIL2
def_get_mod_reg_bit(cpu_ctrl, status_r, standbywfil2, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_GENER_REG_GPR
def_get_mod_reg_bit(cpu_ctrl, gener_reg, gpr, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_CPU_CLK_SEL_SRC
def_get_mod_reg_bit(cpu_ctrl, cpu_clk_sel, src, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_CPU_CLK_SEL_SRC_DIV
def_get_mod_reg_bit(cpu_ctrl, cpu_clk_sel, src_div, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_CPU_CLK_SEL_DBG_EN
def_get_mod_reg_bit(cpu_ctrl, cpu_clk_sel, dbg_en, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_CPU_CLK_SEL_EN
def_get_mod_reg_bit(cpu_ctrl, cpu_clk_sel, en, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_ADPLL_FAC_PRE_DIV
def_get_mod_reg_bit(cpu_ctrl, adpll_fac, pre_div, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_ADPLL_FAC_N
def_get_mod_reg_bit(cpu_ctrl, adpll_fac, n, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_ADPLL_TUNE0_TUNE0
def_get_mod_reg_bit(cpu_ctrl, adpll_tune0, tune0, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_ADPLL_TUNE0_TUNE1
def_get_mod_reg_bit(cpu_ctrl, adpll_tune0, tune1, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_ADPLL_TUNE0_AUTO_ADRANGE
def_get_mod_reg_bit(cpu_ctrl, adpll_tune0, auto_adrange, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_ADPLL_TUNE0_TUNE2
def_get_mod_reg_bit(cpu_ctrl, adpll_tune0, tune2, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_ADPLL_TUNE0_TUNE3
def_get_mod_reg_bit(cpu_ctrl, adpll_tune0, tune3, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_ADPLL_TUNE0_TUNE4
def_get_mod_reg_bit(cpu_ctrl, adpll_tune0, tune4, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_ADPLL_TUNE1_TUNE0
def_get_mod_reg_bit(cpu_ctrl, adpll_tune1, tune0, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_ADPLL_TUNE1_TUNE1
def_get_mod_reg_bit(cpu_ctrl, adpll_tune1, tune1, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_ADPLL_TUNE1_TUNE2
def_get_mod_reg_bit(cpu_ctrl, adpll_tune1, tune2, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_ADPLL_TUNE1_TUNE3
def_get_mod_reg_bit(cpu_ctrl, adpll_tune1, tune3, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_ADPLL_TUNE1_SLEWRATE
def_get_mod_reg_bit(cpu_ctrl, adpll_tune1, slewrate, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_ADPLL_TUNE1_TUNE4
def_get_mod_reg_bit(cpu_ctrl, adpll_tune1, tune4, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_ADPLL_TUNE1_TUNE5
def_get_mod_reg_bit(cpu_ctrl, adpll_tune1, tune5, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_ADPLL_TUNE1_TUNE6
def_get_mod_reg_bit(cpu_ctrl, adpll_tune1, tune6, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_ADPLL_TUNE1_TUNE7
def_get_mod_reg_bit(cpu_ctrl, adpll_tune1, tune7, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_ADPLL_STAT_FERR
def_get_mod_reg_bit(cpu_ctrl, adpll_stat, ferr, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_ADPLL_STAT_FLOCK
def_get_mod_reg_bit(cpu_ctrl, adpll_stat, flock, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_ADPLL_STAT_PERR
def_get_mod_reg_bit(cpu_ctrl, adpll_stat, perr, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_ADPLL_STAT_PLOCK
def_get_mod_reg_bit(cpu_ctrl, adpll_stat, plock, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_CACHE_CFG0_PARAMETER
def_get_mod_reg_bit(cpu_ctrl, cache_cfg0, parameter, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_CACHE_CFG1_PARAMETER
def_get_mod_reg_bit(cpu_ctrl, cache_cfg1, parameter, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_DBG_REG0_DBGPWRDUP0
def_get_mod_reg_bit(cpu_ctrl, dbg_reg0, dbgpwrdup0, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_DBG_REG0_DBGPWRDUP1
def_get_mod_reg_bit(cpu_ctrl, dbg_reg0, dbgpwrdup1, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_DBG_REG0_DBGPWRUPREQ0
def_get_mod_reg_bit(cpu_ctrl, dbg_reg0, dbgpwrupreq0, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_DBG_REG0_DBGPWRUPREQ1
def_get_mod_reg_bit(cpu_ctrl, dbg_reg0, dbgpwrupreq1, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_DBG_REG0_DBGNOPWRDWN0
def_get_mod_reg_bit(cpu_ctrl, dbg_reg0, dbgnopwrdwn0, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_DBG_REG0_DBGNOPWRDWN1
def_get_mod_reg_bit(cpu_ctrl, dbg_reg0, dbgnopwrdwn1, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_VER_VER_L
def_get_mod_reg_bit(cpu_ctrl, ver, ver_l, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_VER_VER_H
def_get_mod_reg_bit(cpu_ctrl, ver, ver_h, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_VER_COMP
def_get_mod_reg_bit(cpu_ctrl, ver, comp, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_C_RST_CTRL_CLUSTER_RST
def_get_mod_reg_bit(cpu_ctrl, c_rst_ctrl, cluster_rst, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_C_RST_CTRL_SOC_DBG_RST
def_get_mod_reg_bit(cpu_ctrl, c_rst_ctrl, soc_dbg_rst, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_C_RST_CTRL_DAP_RST
def_get_mod_reg_bit(cpu_ctrl, c_rst_ctrl, dap_rst, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_C_RST_CTRL_JTAG_RST
def_get_mod_reg_bit(cpu_ctrl, c_rst_ctrl, jtag_rst, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_C_RST_CTRL_GIC_RST
def_get_mod_reg_bit(cpu_ctrl, c_rst_ctrl, gic_rst, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_C_RST_CTRL_L2_RST
def_get_mod_reg_bit(cpu_ctrl, c_rst_ctrl, l2_rst, addr);
#endif

#ifdef DEF_GET_CPU_CTRL_C_RST_CTRL_L2RSTDISABLE
def_get_mod_reg_bit(cpu_ctrl, c_rst_ctrl, l2rstdisable, addr);
#endif

#endif /* ___CPU_CTRL__GET___H___ */
