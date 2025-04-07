/* cpu_ctrl_set.h */

#ifndef ___CPU_CTRL__SET___H___
#define ___CPU_CTRL__SET___H___

#ifdef DEF_SET_CPU_CTRL_RST_CTRL
void set_cpu_ctrl_rst_ctrl(u32 reg_addr,
			u32 ncorereset0,
			u32 ncorereset1,
			u32 ncoreporeset0,
			u32 ncoreporeset1,
			u32 ndbgreset0,
			u32 ndbgreset1,
			u32 l1rstdisable0,
			u32 l1rstdisable1,
			u32 m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_RST_CTRL_NCORERESET0
def_set_mod_reg_bit(cpu_ctrl, rst_ctrl, ncorereset0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_RST_CTRL_NCORERESET1
def_set_mod_reg_bit(cpu_ctrl, rst_ctrl, ncorereset1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_RST_CTRL_NCOREPORESET0
def_set_mod_reg_bit(cpu_ctrl, rst_ctrl, ncoreporeset0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_RST_CTRL_NCOREPORESET1
def_set_mod_reg_bit(cpu_ctrl, rst_ctrl, ncoreporeset1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_RST_CTRL_NDBGRESET0
def_set_mod_reg_bit(cpu_ctrl, rst_ctrl, ndbgreset0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_RST_CTRL_NDBGRESET1
def_set_mod_reg_bit(cpu_ctrl, rst_ctrl, ndbgreset1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_RST_CTRL_L1RSTDISABLE0
def_set_mod_reg_bit(cpu_ctrl, rst_ctrl, l1rstdisable0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_RST_CTRL_L1RSTDISABLE1
def_set_mod_reg_bit(cpu_ctrl, rst_ctrl, l1rstdisable1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_CTRL_R
void set_cpu_ctrl_ctrl_r(u32 reg_addr,
			u32 eventi,
			u32 cfgsdisable,
			u32 cp15sdisable0,
			u32 cp15sdisable1,
			u32 cpu1_gat,
			u32 acinactm,
			u32 m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_CTRL_R_EVENTI
def_set_mod_reg_bit(cpu_ctrl, ctrl_r, eventi, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_CTRL_R_CFGSDISABLE
def_set_mod_reg_bit(cpu_ctrl, ctrl_r, cfgsdisable, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_CTRL_R_CP15SDISABLE0
def_set_mod_reg_bit(cpu_ctrl, ctrl_r, cp15sdisable0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_CTRL_R_CP15SDISABLE1
def_set_mod_reg_bit(cpu_ctrl, ctrl_r, cp15sdisable1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_CTRL_R_CPU1_GAT
def_set_mod_reg_bit(cpu_ctrl, ctrl_r, cpu1_gat, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_CTRL_R_ACINACTM
def_set_mod_reg_bit(cpu_ctrl, ctrl_r, acinactm, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_CFG_R
void set_cpu_ctrl_cfg_r(u32 reg_addr,
			u32 broadcastinner,
			u32 broadcastouter,
			u32 broadcastcachemaint,
			u32 sysbardisable,
			u32 m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_CFG_R_BROADCASTINNER
def_set_mod_reg_bit(cpu_ctrl, cfg_r, broadcastinner, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_CFG_R_BROADCASTOUTER
def_set_mod_reg_bit(cpu_ctrl, cfg_r, broadcastouter, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_CFG_R_BROADCASTCACHEMAINT
def_set_mod_reg_bit(cpu_ctrl, cfg_r, broadcastcachemaint, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_CFG_R_SYSBARDISABLE
def_set_mod_reg_bit(cpu_ctrl, cfg_r, sysbardisable, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_STATUS_R
void set_cpu_ctrl_status_r(u32 reg_addr,
			u32 smpnamp0,
			u32 smpnamp1,
			u32 standbywfe0,
			u32 standbywfe1,
			u32 standbywfi0,
			u32 standbywfi1,
			u32 standbywfil2,
			u32 m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_STATUS_R_SMPNAMP0
def_set_mod_reg_bit(cpu_ctrl, status_r, smpnamp0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_STATUS_R_SMPNAMP1
def_set_mod_reg_bit(cpu_ctrl, status_r, smpnamp1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_STATUS_R_STANDBYWFE0
def_set_mod_reg_bit(cpu_ctrl, status_r, standbywfe0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_STATUS_R_STANDBYWFE1
def_set_mod_reg_bit(cpu_ctrl, status_r, standbywfe1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_STATUS_R_STANDBYWFI0
def_set_mod_reg_bit(cpu_ctrl, status_r, standbywfi0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_STATUS_R_STANDBYWFI1
def_set_mod_reg_bit(cpu_ctrl, status_r, standbywfi1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_STATUS_R_STANDBYWFIL2
def_set_mod_reg_bit(cpu_ctrl, status_r, standbywfil2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_GENER_REG
void set_cpu_ctrl_gener_reg(u32 reg_addr,
				u32 gpr,
				u32 m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_GENER_REG_GPR
def_set_mod_reg_bit(cpu_ctrl, gener_reg, gpr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_CPU_CLK_SEL
void set_cpu_ctrl_cpu_clk_sel(u32 reg_addr,
				u32 src,
				u32 src_div,
				u32 dbg_en,
				u32 en,
				u32 m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_CPU_CLK_SEL_SRC
def_set_mod_reg_bit(cpu_ctrl, cpu_clk_sel, src, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_CPU_CLK_SEL_SRC_DIV
def_set_mod_reg_bit(cpu_ctrl, cpu_clk_sel, src_div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_CPU_CLK_SEL_DBG_EN
def_set_mod_reg_bit(cpu_ctrl, cpu_clk_sel, dbg_en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_CPU_CLK_SEL_EN
def_set_mod_reg_bit(cpu_ctrl, cpu_clk_sel, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_ADPLL_FAC
void set_cpu_ctrl_adpll_fac(u32 reg_addr,
				u32 pre_div,
				u32 n,
				u32 m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_ADPLL_FAC_PRE_DIV
def_set_mod_reg_bit(cpu_ctrl, adpll_fac, pre_div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_ADPLL_FAC_N
def_set_mod_reg_bit(cpu_ctrl, adpll_fac, n, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_ADPLL_TUNE0
void set_cpu_ctrl_adpll_tune0(u32 reg_addr,
				u32 tune0,
				u32 tune1,
				u32 auto_adrange,
				u32 tune2,
				u32 tune3,
				u32 tune4,
				u32 m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_ADPLL_TUNE0_TUNE0
def_set_mod_reg_bit(cpu_ctrl, adpll_tune0, tune0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_ADPLL_TUNE0_TUNE1
def_set_mod_reg_bit(cpu_ctrl, adpll_tune0, tune1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_ADPLL_TUNE0_AUTO_ADRANGE
def_set_mod_reg_bit(cpu_ctrl, adpll_tune0, auto_adrange, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_ADPLL_TUNE0_TUNE2
def_set_mod_reg_bit(cpu_ctrl, adpll_tune0, tune2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_ADPLL_TUNE0_TUNE3
def_set_mod_reg_bit(cpu_ctrl, adpll_tune0, tune3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_ADPLL_TUNE0_TUNE4
def_set_mod_reg_bit(cpu_ctrl, adpll_tune0, tune4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_ADPLL_TUNE1
void set_cpu_ctrl_adpll_tune1(u32 reg_addr,
				u32 tune0,
				u32 tune1,
				u32 tune2,
				u32 tune3,
				u32 slewrate,
				u32 tune4,
				u32 tune5,
				u32 tune6,
				u32 tune7,
				u32 m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_ADPLL_TUNE1_TUNE0
def_set_mod_reg_bit(cpu_ctrl, adpll_tune1, tune0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_ADPLL_TUNE1_TUNE1
def_set_mod_reg_bit(cpu_ctrl, adpll_tune1, tune1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_ADPLL_TUNE1_TUNE2
def_set_mod_reg_bit(cpu_ctrl, adpll_tune1, tune2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_ADPLL_TUNE1_TUNE3
def_set_mod_reg_bit(cpu_ctrl, adpll_tune1, tune3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_ADPLL_TUNE1_SLEWRATE
def_set_mod_reg_bit(cpu_ctrl, adpll_tune1, slewrate, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_ADPLL_TUNE1_TUNE4
def_set_mod_reg_bit(cpu_ctrl, adpll_tune1, tune4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_ADPLL_TUNE1_TUNE5
def_set_mod_reg_bit(cpu_ctrl, adpll_tune1, tune5, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_ADPLL_TUNE1_TUNE6
def_set_mod_reg_bit(cpu_ctrl, adpll_tune1, tune6, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_ADPLL_TUNE1_TUNE7
def_set_mod_reg_bit(cpu_ctrl, adpll_tune1, tune7, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_ADPLL_STAT
void set_cpu_ctrl_adpll_stat(u32 reg_addr,
				u32 ferr,
				u32 flock,
				u32 perr,
				u32 plock,
				u32 m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_ADPLL_STAT_FERR
def_set_mod_reg_bit(cpu_ctrl, adpll_stat, ferr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_ADPLL_STAT_FLOCK
def_set_mod_reg_bit(cpu_ctrl, adpll_stat, flock, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_ADPLL_STAT_PERR
def_set_mod_reg_bit(cpu_ctrl, adpll_stat, perr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_ADPLL_STAT_PLOCK
def_set_mod_reg_bit(cpu_ctrl, adpll_stat, plock, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_CACHE_CFG0
void set_cpu_ctrl_cache_cfg0(u32 reg_addr,
				u32 parameter,
				u32 m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_CACHE_CFG0_PARAMETER
def_set_mod_reg_bit(cpu_ctrl, cache_cfg0, parameter, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_CACHE_CFG1
void set_cpu_ctrl_cache_cfg1(u32 reg_addr,
				u32 parameter,
				u32 m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_CACHE_CFG1_PARAMETER
def_set_mod_reg_bit(cpu_ctrl, cache_cfg1, parameter, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_DBG_REG0
void set_cpu_ctrl_dbg_reg0(u32 reg_addr,
			u32 dbgpwrdup0,
			u32 dbgpwrdup1,
			u32 dbgpwrupreq0,
			u32 dbgpwrupreq1,
			u32 dbgnopwrdwn0,
			u32 dbgnopwrdwn1,
			u32 m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_DBG_REG0_DBGPWRDUP0
def_set_mod_reg_bit(cpu_ctrl, dbg_reg0, dbgpwrdup0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_DBG_REG0_DBGPWRDUP1
def_set_mod_reg_bit(cpu_ctrl, dbg_reg0, dbgpwrdup1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_DBG_REG0_DBGPWRUPREQ0
def_set_mod_reg_bit(cpu_ctrl, dbg_reg0, dbgpwrupreq0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_DBG_REG0_DBGPWRUPREQ1
def_set_mod_reg_bit(cpu_ctrl, dbg_reg0, dbgpwrupreq1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_DBG_REG0_DBGNOPWRDWN0
def_set_mod_reg_bit(cpu_ctrl, dbg_reg0, dbgnopwrdwn0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_DBG_REG0_DBGNOPWRDWN1
def_set_mod_reg_bit(cpu_ctrl, dbg_reg0, dbgnopwrdwn1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_VER
void set_cpu_ctrl_ver(u32 reg_addr,
			u32 ver_l,
			u32 ver_h,
			u32 comp,
			u32 m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_VER_VER_L
def_set_mod_reg_bit(cpu_ctrl, ver, ver_l, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_VER_VER_H
def_set_mod_reg_bit(cpu_ctrl, ver, ver_h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_VER_COMP
def_set_mod_reg_bit(cpu_ctrl, ver, comp, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_C_RST_CTRL
void set_cpu_ctrl_c_rst_ctrl(u32 reg_addr,
				u32 cluster_rst,
				u32 soc_dbg_rst,
				u32 dap_rst,
				u32 jtag_rst,
				u32 gic_rst,
				u32 l2_rst,
				u32 l2rstdisable,
				u32 m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_C_RST_CTRL_CLUSTER_RST
def_set_mod_reg_bit(cpu_ctrl, c_rst_ctrl, cluster_rst, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_C_RST_CTRL_SOC_DBG_RST
def_set_mod_reg_bit(cpu_ctrl, c_rst_ctrl, soc_dbg_rst, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_C_RST_CTRL_DAP_RST
def_set_mod_reg_bit(cpu_ctrl, c_rst_ctrl, dap_rst, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_C_RST_CTRL_JTAG_RST
def_set_mod_reg_bit(cpu_ctrl, c_rst_ctrl, jtag_rst, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_C_RST_CTRL_GIC_RST
def_set_mod_reg_bit(cpu_ctrl, c_rst_ctrl, gic_rst, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_C_RST_CTRL_L2_RST
def_set_mod_reg_bit(cpu_ctrl, c_rst_ctrl, l2_rst, addr, val, m_or_r);
#endif

#ifdef DEF_SET_CPU_CTRL_C_RST_CTRL_L2RSTDISABLE
def_set_mod_reg_bit(cpu_ctrl, c_rst_ctrl, l2rstdisable, addr, val, m_or_r);
#endif

#endif /* ___CPU_CTRL__SET___H___ */
