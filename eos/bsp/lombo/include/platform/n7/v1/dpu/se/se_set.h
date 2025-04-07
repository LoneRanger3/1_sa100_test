/* se_set.h */

#ifndef ___SE__SET___H___
#define ___SE__SET___H___

#ifdef DEF_SET_SE_VER
void set_se_ver(u32 reg_addr,
		u32 ver_l,
		u32 ver_h,
		u32 comp,
		u32 m_or_r);
#endif

#ifdef DEF_SET_SE_VER_VER_L
def_set_mod_reg_bit(se, ver, ver_l, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_VER_VER_H
def_set_mod_reg_bit(se, ver, ver_h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_VER_COMP
def_set_mod_reg_bit(se, ver, comp, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_FTR
void set_se_ftr(u32 reg_addr,
		u32 has_enhance,
		u32 has_lut,
		u32 has_dit,
		u32 m_or_r);
#endif

#ifdef DEF_SET_SE_FTR_HAS_ENHANCE
def_set_mod_reg_bit(se, ftr, has_enhance, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_FTR_HAS_LUT
def_set_mod_reg_bit(se, ftr, has_lut, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_FTR_HAS_DIT
def_set_mod_reg_bit(se, ftr, has_dit, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CSTA
void set_se_csta(u32 reg_addr,
		u32 ln_num,
		u32 field_sta,
		u32 frm_cnt,
		u32 m_or_r);
#endif

#ifdef DEF_SET_SE_CSTA_LN_NUM
def_set_mod_reg_bit(se, csta, ln_num, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CSTA_FIELD_STA
def_set_mod_reg_bit(se, csta, field_sta, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CSTA_FRM_CNT
def_set_mod_reg_bit(se, csta, frm_cnt, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_STA
void set_se_sta(u32 reg_addr,
		u32 rd0sta,
		u32 rd1sta,
		u32 rd2sta,
		u32 rdtotalsta,
		u32 wr0sta,
		u32 wr1sta,
		u32 wr2sta,
		u32 wrtotalsta,
		u32 m_or_r);
#endif

#ifdef DEF_SET_SE_STA_RD0STA
def_set_mod_reg_bit(se, sta, rd0sta, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_STA_RD1STA
def_set_mod_reg_bit(se, sta, rd1sta, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_STA_RD2STA
def_set_mod_reg_bit(se, sta, rd2sta, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_STA_RDTOTALSTA
def_set_mod_reg_bit(se, sta, rdtotalsta, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_STA_WR0STA
def_set_mod_reg_bit(se, sta, wr0sta, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_STA_WR1STA
def_set_mod_reg_bit(se, sta, wr1sta, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_STA_WR2STA
def_set_mod_reg_bit(se, sta, wr2sta, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_STA_WRTOTALSTA
def_set_mod_reg_bit(se, sta, wrtotalsta, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CTL
void set_se_ctl(u32 reg_addr,
		u32 se_en,
		u32 se_bypass,
		u32 dbg_ctl,
		u32 rstn,
		u32 m_or_r);
#endif

#ifdef DEF_SET_SE_CTL_SE_EN
def_set_mod_reg_bit(se, ctl, se_en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CTL_SE_BYPASS
def_set_mod_reg_bit(se, ctl, se_bypass, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CTL_DBG_CTL
def_set_mod_reg_bit(se, ctl, dbg_ctl, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CTL_RSTN
def_set_mod_reg_bit(se, ctl, rstn, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CFG0
void set_se_cfg0(u32 reg_addr,
		u32 dcout_en,
		u32 in_sel,
		u32 out_mode,
		u32 m_or_r);
#endif

#ifdef DEF_SET_SE_CFG0_DCOUT_EN
def_set_mod_reg_bit(se, cfg0, dcout_en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CFG0_IN_SEL
def_set_mod_reg_bit(se, cfg0, in_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CFG0_OUT_MODE
def_set_mod_reg_bit(se, cfg0, out_mode, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CFG1
void set_se_cfg1(u32 reg_addr,
		u32 infmt,
		u32 m_or_r);
#endif

#ifdef DEF_SET_SE_CFG1_INFMT
def_set_mod_reg_bit(se, cfg1, infmt, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CFG2
void set_se_cfg2(u32 reg_addr,
		u32 csci_en,
		u32 csco_en,
		u32 alpha_en,
		u32 premul_en,
		u32 rsmp_en,
		u32 lb_mode,
		u32 field_pol,
		u32 wb_field_lv,
		u32 m_or_r);
#endif

#ifdef DEF_SET_SE_CFG2_CSCI_EN
def_set_mod_reg_bit(se, cfg2, csci_en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CFG2_CSCO_EN
def_set_mod_reg_bit(se, cfg2, csco_en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CFG2_ALPHA_EN
def_set_mod_reg_bit(se, cfg2, alpha_en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CFG2_PREMUL_EN
def_set_mod_reg_bit(se, cfg2, premul_en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CFG2_RSMP_EN
def_set_mod_reg_bit(se, cfg2, rsmp_en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CFG2_LB_MODE
def_set_mod_reg_bit(se, cfg2, lb_mode, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CFG2_FIELD_POL
def_set_mod_reg_bit(se, cfg2, field_pol, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CFG2_WB_FIELD_LV
def_set_mod_reg_bit(se, cfg2, wb_field_lv, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_INTCTL
void set_se_intctl(u32 reg_addr,
		u32 lntrig_en,
		u32 wbfin_en,
		u32 wbovfl_en,
		u32 wbtmout_en,
		u32 rdfin_en,
		u32 m_or_r);
#endif

#ifdef DEF_SET_SE_INTCTL_LNTRIG_EN
def_set_mod_reg_bit(se, intctl, lntrig_en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_INTCTL_WBFIN_EN
def_set_mod_reg_bit(se, intctl, wbfin_en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_INTCTL_WBOVFL_EN
def_set_mod_reg_bit(se, intctl, wbovfl_en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_INTCTL_WBTMOUT_EN
def_set_mod_reg_bit(se, intctl, wbtmout_en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_INTCTL_RDFIN_EN
def_set_mod_reg_bit(se, intctl, rdfin_en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_INTSTA
void set_se_intsta(u32 reg_addr,
		u32 lntrig,
		u32 wbfin,
		u32 wbovfl,
		u32 wbtmout,
		u32 rdfin,
		u32 m_or_r);
#endif

#ifdef DEF_SET_SE_INTSTA_LNTRIG
def_set_mod_reg_bit(se, intsta, lntrig, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_INTSTA_WBFIN
def_set_mod_reg_bit(se, intsta, wbfin, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_INTSTA_WBOVFL
def_set_mod_reg_bit(se, intsta, wbovfl, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_INTSTA_WBTMOUT
def_set_mod_reg_bit(se, intsta, wbtmout, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_INTSTA_RDFIN
def_set_mod_reg_bit(se, intsta, rdfin, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_INTCLR
void set_se_intclr(u32 reg_addr,
		u32 lntrigclr,
		u32 wbfinclr,
		u32 wbovflclr,
		u32 wbtmoutclr,
		u32 rdfinclr,
		u32 m_or_r);
#endif

#ifdef DEF_SET_SE_INTCLR_LNTRIGCLR
def_set_mod_reg_bit(se, intclr, lntrigclr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_INTCLR_WBFINCLR
def_set_mod_reg_bit(se, intclr, wbfinclr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_INTCLR_WBOVFLCLR
def_set_mod_reg_bit(se, intclr, wbovflclr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_INTCLR_WBTMOUTCLR
def_set_mod_reg_bit(se, intclr, wbtmoutclr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_INTCLR_RDFINCLR
def_set_mod_reg_bit(se, intclr, rdfinclr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_LNCNT
void set_se_lncnt(u32 reg_addr,
		u32 trig_num,
		u32 trig_field,
		u32 m_or_r);
#endif

#ifdef DEF_SET_SE_LNCNT_TRIG_NUM
def_set_mod_reg_bit(se, lncnt, trig_num, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_LNCNT_TRIG_FIELD
def_set_mod_reg_bit(se, lncnt, trig_field, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_UPDCTL
void set_se_updctl(u32 reg_addr,
		u32 upd_im,
		u32 sync_mode,
		u32 m_or_r);
#endif

#ifdef DEF_SET_SE_UPDCTL_UPD_IM
def_set_mod_reg_bit(se, updctl, upd_im, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_UPDCTL_SYNC_MODE
def_set_mod_reg_bit(se, updctl, sync_mode, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CTABSWT
void set_se_ctabswt(u32 reg_addr,
			u32 tab_swt,
			u32 clk_swt,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_CTABSWT_TAB_SWT
def_set_mod_reg_bit(se, ctabswt, tab_swt, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CTABSWT_CLK_SWT
def_set_mod_reg_bit(se, ctabswt, clk_swt, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_STRMCTL
void set_se_strmctl(u32 reg_addr,
			u32 strm_start,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_STRMCTL_STRM_START
def_set_mod_reg_bit(se, strmctl, strm_start, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_INADDR0
void set_se_inaddr0(u32 reg_addr,
			u32 addr,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_INADDR0_ADDR
def_set_mod_reg_bit(se, inaddr0, addr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_INADDR1
void set_se_inaddr1(u32 reg_addr,
			u32 addr,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_INADDR1_ADDR
def_set_mod_reg_bit(se, inaddr1, addr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_INADDR2
void set_se_inaddr2(u32 reg_addr,
			u32 addr,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_INADDR2_ADDR
def_set_mod_reg_bit(se, inaddr2, addr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_INSIZE0
void set_se_insize0(u32 reg_addr,
			u32 w,
			u32 h,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_INSIZE0_W
def_set_mod_reg_bit(se, insize0, w, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_INSIZE0_H
def_set_mod_reg_bit(se, insize0, h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_INSIZE1
void set_se_insize1(u32 reg_addr,
			u32 w,
			u32 h,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_INSIZE1_W
def_set_mod_reg_bit(se, insize1, w, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_INSIZE1_H
def_set_mod_reg_bit(se, insize1, h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_RSMPSIZE
void set_se_rsmpsize(u32 reg_addr,
			u32 w,
			u32 h,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_RSMPSIZE_W
def_set_mod_reg_bit(se, rsmpsize, w, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_RSMPSIZE_H
def_set_mod_reg_bit(se, rsmpsize, h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_INLSTR0
void set_se_inlstr0(u32 reg_addr,
			u32 lstr,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_INLSTR0_LSTR
def_set_mod_reg_bit(se, inlstr0, lstr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_INLSTR1
void set_se_inlstr1(u32 reg_addr,
			u32 lstr,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_INLSTR1_LSTR
def_set_mod_reg_bit(se, inlstr1, lstr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_SCRX0
void set_se_scrx0(u32 reg_addr,
		u32 scr,
		u32 m_or_r);
#endif

#ifdef DEF_SET_SE_SCRX0_SCR
def_set_mod_reg_bit(se, scrx0, scr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_SCRY0
void set_se_scry0(u32 reg_addr,
		u32 scr,
		u32 m_or_r);
#endif

#ifdef DEF_SET_SE_SCRY0_SCR
def_set_mod_reg_bit(se, scry0, scr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_SCRX1
void set_se_scrx1(u32 reg_addr,
		u32 scr,
		u32 m_or_r);
#endif

#ifdef DEF_SET_SE_SCRX1_SCR
def_set_mod_reg_bit(se, scrx1, scr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_SCRY1
void set_se_scry1(u32 reg_addr,
		u32 scr,
		u32 m_or_r);
#endif

#ifdef DEF_SET_SE_SCRY1_SCR
def_set_mod_reg_bit(se, scry1, scr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CH0XOFST
void set_se_ch0xofst(u32 reg_addr,
			u32 ofst,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_CH0XOFST_OFST
def_set_mod_reg_bit(se, ch0xofst, ofst, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CH0YOFST
void set_se_ch0yofst(u32 reg_addr,
			u32 ofst,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_CH0YOFST_OFST
def_set_mod_reg_bit(se, ch0yofst, ofst, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CH1XOFST
void set_se_ch1xofst(u32 reg_addr,
			u32 ofst,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_CH1XOFST_OFST
def_set_mod_reg_bit(se, ch1xofst, ofst, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CH1YOFST
void set_se_ch1yofst(u32 reg_addr,
			u32 ofst,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_CH1YOFST_OFST
def_set_mod_reg_bit(se, ch1yofst, ofst, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_SHIFTCTL
void set_se_shiftctl(u32 reg_addr,
			u32 ctlx0,
			u32 ctly0,
			u32 ctlx1,
			u32 ctly1,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_SHIFTCTL_CTLX0
def_set_mod_reg_bit(se, shiftctl, ctlx0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_SHIFTCTL_CTLY0
def_set_mod_reg_bit(se, shiftctl, ctly0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_SHIFTCTL_CTLX1
def_set_mod_reg_bit(se, shiftctl, ctlx1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_SHIFTCTL_CTLY1
def_set_mod_reg_bit(se, shiftctl, ctly1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_OUT_CROPOFST
void set_se_out_cropofst(u32 reg_addr,
			u32 x,
			u32 y,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_OUT_CROPOFST_X
def_set_mod_reg_bit(se, out_cropofst, x, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_OUT_CROPOFST_Y
def_set_mod_reg_bit(se, out_cropofst, y, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_OUT_CROPSIZE
void set_se_out_cropsize(u32 reg_addr,
			u32 w,
			u32 h,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_OUT_CROPSIZE_W
def_set_mod_reg_bit(se, out_cropsize, w, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_OUT_CROPSIZE_H
def_set_mod_reg_bit(se, out_cropsize, h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_WBSIZE0
void set_se_wbsize0(u32 reg_addr,
			u32 w,
			u32 h,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_WBSIZE0_W
def_set_mod_reg_bit(se, wbsize0, w, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_WBSIZE0_H
def_set_mod_reg_bit(se, wbsize0, h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_WBSIZE1
void set_se_wbsize1(u32 reg_addr,
			u32 w,
			u32 h,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_WBSIZE1_W
def_set_mod_reg_bit(se, wbsize1, w, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_WBSIZE1_H
def_set_mod_reg_bit(se, wbsize1, h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_WBCTL
void set_se_wbctl(u32 reg_addr,
		u32 wb_start,
		u32 m_or_r);
#endif

#ifdef DEF_SET_SE_WBCTL_WB_START
def_set_mod_reg_bit(se, wbctl, wb_start, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_WBCFG
void set_se_wbcfg(u32 reg_addr,
		u32 wbfmt,
		u32 wbthr,
		u32 m_or_r);
#endif

#ifdef DEF_SET_SE_WBCFG_WBFMT
def_set_mod_reg_bit(se, wbcfg, wbfmt, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_WBCFG_WBTHR
def_set_mod_reg_bit(se, wbcfg, wbthr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_WBTMR
void set_se_wbtmr(u32 reg_addr,
		u32 wbtmr,
		u32 m_or_r);
#endif

#ifdef DEF_SET_SE_WBTMR_WBTMR
def_set_mod_reg_bit(se, wbtmr, wbtmr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_WBADDR0
void set_se_wbaddr0(u32 reg_addr,
			u32 addr,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_WBADDR0_ADDR
def_set_mod_reg_bit(se, wbaddr0, addr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_WBADDR1
void set_se_wbaddr1(u32 reg_addr,
			u32 addr,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_WBADDR1_ADDR
def_set_mod_reg_bit(se, wbaddr1, addr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_WBADDR2
void set_se_wbaddr2(u32 reg_addr,
			u32 addr,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_WBADDR2_ADDR
def_set_mod_reg_bit(se, wbaddr2, addr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_WBLSTR0
void set_se_wblstr0(u32 reg_addr,
			u32 lstr,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_WBLSTR0_LSTR
def_set_mod_reg_bit(se, wblstr0, lstr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_WBLSTR1
void set_se_wblstr1(u32 reg_addr,
			u32 lstr,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_WBLSTR1_LSTR
def_set_mod_reg_bit(se, wblstr1, lstr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CSCI_COEF
void set_se_csci_coef(u32 reg_addr,
			u32 coef,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_CSCI_COEF_COEF
def_set_mod_reg_bit(se, csci_coef, coef, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CSCO_COEF
void set_se_csco_coef(u32 reg_addr,
			u32 coef,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_CSCO_COEF_COEF
def_set_mod_reg_bit(se, csco_coef, coef, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_VPPCTL
void set_se_vppctl(u32 reg_addr,
		u32 lut0en,
		u32 lut1en,
		u32 lut2en,
		u32 lut_mod,
		u32 e0en,
		u32 e1en,
		u32 e2en,
		u32 e3en,
		u32 hist_sel,
		u32 hist_ok,
		u32 hist_clr,
		u32 hist_en,
		u32 m_or_r);
#endif

#ifdef DEF_SET_SE_VPPCTL_LUT0EN
def_set_mod_reg_bit(se, vppctl, lut0en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_VPPCTL_LUT1EN
def_set_mod_reg_bit(se, vppctl, lut1en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_VPPCTL_LUT2EN
def_set_mod_reg_bit(se, vppctl, lut2en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_VPPCTL_LUT_MOD
def_set_mod_reg_bit(se, vppctl, lut_mod, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_VPPCTL_E0EN
def_set_mod_reg_bit(se, vppctl, e0en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_VPPCTL_E1EN
def_set_mod_reg_bit(se, vppctl, e1en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_VPPCTL_E2EN
def_set_mod_reg_bit(se, vppctl, e2en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_VPPCTL_E3EN
def_set_mod_reg_bit(se, vppctl, e3en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_VPPCTL_HIST_SEL
def_set_mod_reg_bit(se, vppctl, hist_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_VPPCTL_HIST_OK
def_set_mod_reg_bit(se, vppctl, hist_ok, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_VPPCTL_HIST_CLR
def_set_mod_reg_bit(se, vppctl, hist_clr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_VPPCTL_HIST_EN
def_set_mod_reg_bit(se, vppctl, hist_en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_VPPLUTSW
void set_se_vpplutsw(u32 reg_addr,
			u32 lut0sw,
			u32 lut1sw,
			u32 lut2sw,
			u32 histsw,
			u32 clk_lut,
			u32 clk_hist,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_VPPLUTSW_LUT0SW
def_set_mod_reg_bit(se, vpplutsw, lut0sw, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_VPPLUTSW_LUT1SW
def_set_mod_reg_bit(se, vpplutsw, lut1sw, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_VPPLUTSW_LUT2SW
def_set_mod_reg_bit(se, vpplutsw, lut2sw, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_VPPLUTSW_HISTSW
def_set_mod_reg_bit(se, vpplutsw, histsw, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_VPPLUTSW_CLK_LUT
def_set_mod_reg_bit(se, vpplutsw, clk_lut, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_VPPLUTSW_CLK_HIST
def_set_mod_reg_bit(se, vpplutsw, clk_hist, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CHECFG0
void set_se_checfg0(u32 reg_addr,
			u32 cthr0,
			u32 cthr1,
			u32 cthr2,
			u32 cthr3,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_CHECFG0_CTHR0
def_set_mod_reg_bit(se, checfg0, cthr0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CHECFG0_CTHR1
def_set_mod_reg_bit(se, checfg0, cthr1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CHECFG0_CTHR2
def_set_mod_reg_bit(se, checfg0, cthr2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CHECFG0_CTHR3
def_set_mod_reg_bit(se, checfg0, cthr3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CHECFG1
void set_se_checfg1(u32 reg_addr,
			u32 spen,
			u32 cord0,
			u32 cord1,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_CHECFG1_SPEN
def_set_mod_reg_bit(se, checfg1, spen, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CHECFG1_CORD0
def_set_mod_reg_bit(se, checfg1, cord0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CHECFG1_CORD1
def_set_mod_reg_bit(se, checfg1, cord1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_PEAKCFG
void set_se_peakcfg(u32 reg_addr,
			u32 cor,
			u32 gain,
			u32 limit,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_PEAKCFG_COR
def_set_mod_reg_bit(se, peakcfg, cor, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_PEAKCFG_GAIN
def_set_mod_reg_bit(se, peakcfg, gain, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_PEAKCFG_LIMIT
def_set_mod_reg_bit(se, peakcfg, limit, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_LTIVCFG
void set_se_ltivcfg(u32 reg_addr,
			u32 cor,
			u32 gain,
			u32 gain2,
			u32 oslmt,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_LTIVCFG_COR
def_set_mod_reg_bit(se, ltivcfg, cor, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_LTIVCFG_GAIN
def_set_mod_reg_bit(se, ltivcfg, gain, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_LTIVCFG_GAIN2
def_set_mod_reg_bit(se, ltivcfg, gain2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_LTIVCFG_OSLMT
def_set_mod_reg_bit(se, ltivcfg, oslmt, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_PEAKCOEF0
void set_se_peakcoef0(u32 reg_addr,
			u32 c0,
			u32 c1,
			u32 c2,
			u32 c3,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_PEAKCOEF0_C0
def_set_mod_reg_bit(se, peakcoef0, c0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_PEAKCOEF0_C1
def_set_mod_reg_bit(se, peakcoef0, c1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_PEAKCOEF0_C2
def_set_mod_reg_bit(se, peakcoef0, c2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_PEAKCOEF0_C3
def_set_mod_reg_bit(se, peakcoef0, c3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_PEAKCOEF1
void set_se_peakcoef1(u32 reg_addr,
			u32 c4,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_PEAKCOEF1_C4
def_set_mod_reg_bit(se, peakcoef1, c4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_LTICFG
void set_se_lticfg(u32 reg_addr,
		u32 cor,
		u32 gain,
		u32 gain2,
		u32 oslmt,
		u32 m_or_r);
#endif

#ifdef DEF_SET_SE_LTICFG_COR
def_set_mod_reg_bit(se, lticfg, cor, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_LTICFG_GAIN
def_set_mod_reg_bit(se, lticfg, gain, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_LTICFG_GAIN2
def_set_mod_reg_bit(se, lticfg, gain2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_LTICFG_OSLMT
def_set_mod_reg_bit(se, lticfg, oslmt, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CTICFG
void set_se_cticfg(u32 reg_addr,
		u32 cor,
		u32 gain,
		u32 gain2,
		u32 oslmt,
		u32 m_or_r);
#endif

#ifdef DEF_SET_SE_CTICFG_COR
def_set_mod_reg_bit(se, cticfg, cor, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CTICFG_GAIN
def_set_mod_reg_bit(se, cticfg, gain, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CTICFG_GAIN2
def_set_mod_reg_bit(se, cticfg, gain2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CTICFG_OSLMT
def_set_mod_reg_bit(se, cticfg, oslmt, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_LTICOEF0
void set_se_lticoef0(u32 reg_addr,
			u32 c0,
			u32 c1,
			u32 c2,
			u32 c3,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_LTICOEF0_C0
def_set_mod_reg_bit(se, lticoef0, c0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_LTICOEF0_C1
def_set_mod_reg_bit(se, lticoef0, c1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_LTICOEF0_C2
def_set_mod_reg_bit(se, lticoef0, c2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_LTICOEF0_C3
def_set_mod_reg_bit(se, lticoef0, c3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_LTICOEF1
void set_se_lticoef1(u32 reg_addr,
			u32 c4,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_LTICOEF1_C4
def_set_mod_reg_bit(se, lticoef1, c4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CTICOEF0
void set_se_cticoef0(u32 reg_addr,
			u32 c0,
			u32 c1,
			u32 c2,
			u32 c3,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_CTICOEF0_C0
def_set_mod_reg_bit(se, cticoef0, c0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CTICOEF0_C1
def_set_mod_reg_bit(se, cticoef0, c1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CTICOEF0_C2
def_set_mod_reg_bit(se, cticoef0, c2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CTICOEF0_C3
def_set_mod_reg_bit(se, cticoef0, c3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CTICOEF1
void set_se_cticoef1(u32 reg_addr,
			u32 c4,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_CTICOEF1_C4
def_set_mod_reg_bit(se, cticoef1, c4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CH0_XCOEF0
void set_se_ch0_xcoef0(u32 reg_addr,
			u32 c0,
			u32 c1,
			u32 c2,
			u32 c3,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_CH0_XCOEF0_C0
def_set_mod_reg_bit(se, ch0_xcoef0, c0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CH0_XCOEF0_C1
def_set_mod_reg_bit(se, ch0_xcoef0, c1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CH0_XCOEF0_C2
def_set_mod_reg_bit(se, ch0_xcoef0, c2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CH0_XCOEF0_C3
def_set_mod_reg_bit(se, ch0_xcoef0, c3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CH0_XCOEF1
void set_se_ch0_xcoef1(u32 reg_addr,
			u32 c4,
			u32 c5,
			u32 c6,
			u32 c7,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_CH0_XCOEF1_C4
def_set_mod_reg_bit(se, ch0_xcoef1, c4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CH0_XCOEF1_C5
def_set_mod_reg_bit(se, ch0_xcoef1, c5, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CH0_XCOEF1_C6
def_set_mod_reg_bit(se, ch0_xcoef1, c6, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CH0_XCOEF1_C7
def_set_mod_reg_bit(se, ch0_xcoef1, c7, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CH0_YCOEF
void set_se_ch0_ycoef(u32 reg_addr,
			u32 c0,
			u32 c1,
			u32 c2,
			u32 c3,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_CH0_YCOEF_C0
def_set_mod_reg_bit(se, ch0_ycoef, c0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CH0_YCOEF_C1
def_set_mod_reg_bit(se, ch0_ycoef, c1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CH0_YCOEF_C2
def_set_mod_reg_bit(se, ch0_ycoef, c2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CH0_YCOEF_C3
def_set_mod_reg_bit(se, ch0_ycoef, c3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CH1_XCOEF0
void set_se_ch1_xcoef0(u32 reg_addr,
			u32 c0,
			u32 c1,
			u32 c2,
			u32 c3,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_CH1_XCOEF0_C0
def_set_mod_reg_bit(se, ch1_xcoef0, c0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CH1_XCOEF0_C1
def_set_mod_reg_bit(se, ch1_xcoef0, c1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CH1_XCOEF0_C2
def_set_mod_reg_bit(se, ch1_xcoef0, c2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CH1_XCOEF0_C3
def_set_mod_reg_bit(se, ch1_xcoef0, c3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CH1_XCOEF1
void set_se_ch1_xcoef1(u32 reg_addr,
			u32 c4,
			u32 c5,
			u32 c6,
			u32 c7,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_CH1_XCOEF1_C4
def_set_mod_reg_bit(se, ch1_xcoef1, c4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CH1_XCOEF1_C5
def_set_mod_reg_bit(se, ch1_xcoef1, c5, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CH1_XCOEF1_C6
def_set_mod_reg_bit(se, ch1_xcoef1, c6, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CH1_XCOEF1_C7
def_set_mod_reg_bit(se, ch1_xcoef1, c7, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CH1_YCOEF
void set_se_ch1_ycoef(u32 reg_addr,
			u32 c0,
			u32 c1,
			u32 c2,
			u32 c3,
			u32 m_or_r);
#endif

#ifdef DEF_SET_SE_CH1_YCOEF_C0
def_set_mod_reg_bit(se, ch1_ycoef, c0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CH1_YCOEF_C1
def_set_mod_reg_bit(se, ch1_ycoef, c1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CH1_YCOEF_C2
def_set_mod_reg_bit(se, ch1_ycoef, c2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CH1_YCOEF_C3
def_set_mod_reg_bit(se, ch1_ycoef, c3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CH0LUT
void set_se_ch0lut(u32 reg_addr,
		u32 c0,
		u32 c1,
		u32 c2,
		u32 c3,
		u32 m_or_r);
#endif

#ifdef DEF_SET_SE_CH0LUT_C0
def_set_mod_reg_bit(se, ch0lut, c0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CH0LUT_C1
def_set_mod_reg_bit(se, ch0lut, c1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CH0LUT_C2
def_set_mod_reg_bit(se, ch0lut, c2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CH0LUT_C3
def_set_mod_reg_bit(se, ch0lut, c3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CH1LUT
void set_se_ch1lut(u32 reg_addr,
		u32 c0,
		u32 c1,
		u32 c2,
		u32 c3,
		u32 m_or_r);
#endif

#ifdef DEF_SET_SE_CH1LUT_C0
def_set_mod_reg_bit(se, ch1lut, c0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CH1LUT_C1
def_set_mod_reg_bit(se, ch1lut, c1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CH1LUT_C2
def_set_mod_reg_bit(se, ch1lut, c2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CH1LUT_C3
def_set_mod_reg_bit(se, ch1lut, c3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CH2LUT
void set_se_ch2lut(u32 reg_addr,
		u32 c0,
		u32 c1,
		u32 c2,
		u32 c3,
		u32 m_or_r);
#endif

#ifdef DEF_SET_SE_CH2LUT_C0
def_set_mod_reg_bit(se, ch2lut, c0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CH2LUT_C1
def_set_mod_reg_bit(se, ch2lut, c1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CH2LUT_C2
def_set_mod_reg_bit(se, ch2lut, c2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CH2LUT_C3
def_set_mod_reg_bit(se, ch2lut, c3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_SE_CHHIST
void set_se_chhist(u32 reg_addr,
		u32 count,
		u32 m_or_r);
#endif

#ifdef DEF_SET_SE_CHHIST_COUNT
def_set_mod_reg_bit(se, chhist, count, addr, val, m_or_r);
#endif

#endif /* ___SE__SET___H___ */
