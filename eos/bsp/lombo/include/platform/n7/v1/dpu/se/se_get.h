/* se_get.h */

#ifndef ___SE__GET___H___
#define ___SE__GET___H___

#ifdef DEF_GET_SE_VER_VER_L
def_get_mod_reg_bit(se, ver, ver_l, addr);
#endif

#ifdef DEF_GET_SE_VER_VER_H
def_get_mod_reg_bit(se, ver, ver_h, addr);
#endif

#ifdef DEF_GET_SE_VER_COMP
def_get_mod_reg_bit(se, ver, comp, addr);
#endif

#ifdef DEF_GET_SE_FTR_HAS_ENHANCE
def_get_mod_reg_bit(se, ftr, has_enhance, addr);
#endif

#ifdef DEF_GET_SE_FTR_HAS_LUT
def_get_mod_reg_bit(se, ftr, has_lut, addr);
#endif

#ifdef DEF_GET_SE_FTR_HAS_DIT
def_get_mod_reg_bit(se, ftr, has_dit, addr);
#endif

#ifdef DEF_GET_SE_CSTA_LN_NUM
def_get_mod_reg_bit(se, csta, ln_num, addr);
#endif

#ifdef DEF_GET_SE_CSTA_FIELD_STA
def_get_mod_reg_bit(se, csta, field_sta, addr);
#endif

#ifdef DEF_GET_SE_CSTA_FRM_CNT
def_get_mod_reg_bit(se, csta, frm_cnt, addr);
#endif

#ifdef DEF_GET_SE_STA_RD0STA
def_get_mod_reg_bit(se, sta, rd0sta, addr);
#endif

#ifdef DEF_GET_SE_STA_RD1STA
def_get_mod_reg_bit(se, sta, rd1sta, addr);
#endif

#ifdef DEF_GET_SE_STA_RD2STA
def_get_mod_reg_bit(se, sta, rd2sta, addr);
#endif

#ifdef DEF_GET_SE_STA_RDTOTALSTA
def_get_mod_reg_bit(se, sta, rdtotalsta, addr);
#endif

#ifdef DEF_GET_SE_STA_WR0STA
def_get_mod_reg_bit(se, sta, wr0sta, addr);
#endif

#ifdef DEF_GET_SE_STA_WR1STA
def_get_mod_reg_bit(se, sta, wr1sta, addr);
#endif

#ifdef DEF_GET_SE_STA_WR2STA
def_get_mod_reg_bit(se, sta, wr2sta, addr);
#endif

#ifdef DEF_GET_SE_STA_WRTOTALSTA
def_get_mod_reg_bit(se, sta, wrtotalsta, addr);
#endif

#ifdef DEF_GET_SE_CTL_SE_EN
def_get_mod_reg_bit(se, ctl, se_en, addr);
#endif

#ifdef DEF_GET_SE_CTL_SE_BYPASS
def_get_mod_reg_bit(se, ctl, se_bypass, addr);
#endif

#ifdef DEF_GET_SE_CTL_DBG_CTL
def_get_mod_reg_bit(se, ctl, dbg_ctl, addr);
#endif

#ifdef DEF_GET_SE_CTL_RSTN
def_get_mod_reg_bit(se, ctl, rstn, addr);
#endif

#ifdef DEF_GET_SE_CFG0_DCOUT_EN
def_get_mod_reg_bit(se, cfg0, dcout_en, addr);
#endif

#ifdef DEF_GET_SE_CFG0_IN_SEL
def_get_mod_reg_bit(se, cfg0, in_sel, addr);
#endif

#ifdef DEF_GET_SE_CFG0_OUT_MODE
def_get_mod_reg_bit(se, cfg0, out_mode, addr);
#endif

#ifdef DEF_GET_SE_CFG1_INFMT
def_get_mod_reg_bit(se, cfg1, infmt, addr);
#endif

#ifdef DEF_GET_SE_CFG2_CSCI_EN
def_get_mod_reg_bit(se, cfg2, csci_en, addr);
#endif

#ifdef DEF_GET_SE_CFG2_CSCO_EN
def_get_mod_reg_bit(se, cfg2, csco_en, addr);
#endif

#ifdef DEF_GET_SE_CFG2_ALPHA_EN
def_get_mod_reg_bit(se, cfg2, alpha_en, addr);
#endif

#ifdef DEF_GET_SE_CFG2_PREMUL_EN
def_get_mod_reg_bit(se, cfg2, premul_en, addr);
#endif

#ifdef DEF_GET_SE_CFG2_RSMP_EN
def_get_mod_reg_bit(se, cfg2, rsmp_en, addr);
#endif

#ifdef DEF_GET_SE_CFG2_LB_MODE
def_get_mod_reg_bit(se, cfg2, lb_mode, addr);
#endif

#ifdef DEF_GET_SE_CFG2_FIELD_POL
def_get_mod_reg_bit(se, cfg2, field_pol, addr);
#endif

#ifdef DEF_GET_SE_CFG2_WB_FIELD_LV
def_get_mod_reg_bit(se, cfg2, wb_field_lv, addr);
#endif

#ifdef DEF_GET_SE_INTCTL_LNTRIG_EN
def_get_mod_reg_bit(se, intctl, lntrig_en, addr);
#endif

#ifdef DEF_GET_SE_INTCTL_WBFIN_EN
def_get_mod_reg_bit(se, intctl, wbfin_en, addr);
#endif

#ifdef DEF_GET_SE_INTCTL_WBOVFL_EN
def_get_mod_reg_bit(se, intctl, wbovfl_en, addr);
#endif

#ifdef DEF_GET_SE_INTCTL_WBTMOUT_EN
def_get_mod_reg_bit(se, intctl, wbtmout_en, addr);
#endif

#ifdef DEF_GET_SE_INTCTL_RDFIN_EN
def_get_mod_reg_bit(se, intctl, rdfin_en, addr);
#endif

#ifdef DEF_GET_SE_INTSTA_LNTRIG
def_get_mod_reg_bit(se, intsta, lntrig, addr);
#endif

#ifdef DEF_GET_SE_INTSTA_WBFIN
def_get_mod_reg_bit(se, intsta, wbfin, addr);
#endif

#ifdef DEF_GET_SE_INTSTA_WBOVFL
def_get_mod_reg_bit(se, intsta, wbovfl, addr);
#endif

#ifdef DEF_GET_SE_INTSTA_WBTMOUT
def_get_mod_reg_bit(se, intsta, wbtmout, addr);
#endif

#ifdef DEF_GET_SE_INTSTA_RDFIN
def_get_mod_reg_bit(se, intsta, rdfin, addr);
#endif

#ifdef DEF_GET_SE_INTCLR_LNTRIGCLR
def_get_mod_reg_bit(se, intclr, lntrigclr, addr);
#endif

#ifdef DEF_GET_SE_INTCLR_WBFINCLR
def_get_mod_reg_bit(se, intclr, wbfinclr, addr);
#endif

#ifdef DEF_GET_SE_INTCLR_WBOVFLCLR
def_get_mod_reg_bit(se, intclr, wbovflclr, addr);
#endif

#ifdef DEF_GET_SE_INTCLR_WBTMOUTCLR
def_get_mod_reg_bit(se, intclr, wbtmoutclr, addr);
#endif

#ifdef DEF_GET_SE_INTCLR_RDFINCLR
def_get_mod_reg_bit(se, intclr, rdfinclr, addr);
#endif

#ifdef DEF_GET_SE_LNCNT_TRIG_NUM
def_get_mod_reg_bit(se, lncnt, trig_num, addr);
#endif

#ifdef DEF_GET_SE_LNCNT_TRIG_FIELD
def_get_mod_reg_bit(se, lncnt, trig_field, addr);
#endif

#ifdef DEF_GET_SE_UPDCTL_UPD_IM
def_get_mod_reg_bit(se, updctl, upd_im, addr);
#endif

#ifdef DEF_GET_SE_UPDCTL_SYNC_MODE
def_get_mod_reg_bit(se, updctl, sync_mode, addr);
#endif

#ifdef DEF_GET_SE_CTABSWT_TAB_SWT
def_get_mod_reg_bit(se, ctabswt, tab_swt, addr);
#endif

#ifdef DEF_GET_SE_CTABSWT_CLK_SWT
def_get_mod_reg_bit(se, ctabswt, clk_swt, addr);
#endif

#ifdef DEF_GET_SE_STRMCTL_STRM_START
def_get_mod_reg_bit(se, strmctl, strm_start, addr);
#endif

#ifdef DEF_GET_SE_INADDR0_ADDR
def_get_mod_reg_bit(se, inaddr0, addr, addr);
#endif

#ifdef DEF_GET_SE_INADDR1_ADDR
def_get_mod_reg_bit(se, inaddr1, addr, addr);
#endif

#ifdef DEF_GET_SE_INADDR2_ADDR
def_get_mod_reg_bit(se, inaddr2, addr, addr);
#endif

#ifdef DEF_GET_SE_INSIZE0_W
def_get_mod_reg_bit(se, insize0, w, addr);
#endif

#ifdef DEF_GET_SE_INSIZE0_H
def_get_mod_reg_bit(se, insize0, h, addr);
#endif

#ifdef DEF_GET_SE_INSIZE1_W
def_get_mod_reg_bit(se, insize1, w, addr);
#endif

#ifdef DEF_GET_SE_INSIZE1_H
def_get_mod_reg_bit(se, insize1, h, addr);
#endif

#ifdef DEF_GET_SE_RSMPSIZE_W
def_get_mod_reg_bit(se, rsmpsize, w, addr);
#endif

#ifdef DEF_GET_SE_RSMPSIZE_H
def_get_mod_reg_bit(se, rsmpsize, h, addr);
#endif

#ifdef DEF_GET_SE_INLSTR0_LSTR
def_get_mod_reg_bit(se, inlstr0, lstr, addr);
#endif

#ifdef DEF_GET_SE_INLSTR1_LSTR
def_get_mod_reg_bit(se, inlstr1, lstr, addr);
#endif

#ifdef DEF_GET_SE_SCRX0_SCR
def_get_mod_reg_bit(se, scrx0, scr, addr);
#endif

#ifdef DEF_GET_SE_SCRY0_SCR
def_get_mod_reg_bit(se, scry0, scr, addr);
#endif

#ifdef DEF_GET_SE_SCRX1_SCR
def_get_mod_reg_bit(se, scrx1, scr, addr);
#endif

#ifdef DEF_GET_SE_SCRY1_SCR
def_get_mod_reg_bit(se, scry1, scr, addr);
#endif

#ifdef DEF_GET_SE_CH0XOFST_OFST
def_get_mod_reg_bit(se, ch0xofst, ofst, addr);
#endif

#ifdef DEF_GET_SE_CH0YOFST_OFST
def_get_mod_reg_bit(se, ch0yofst, ofst, addr);
#endif

#ifdef DEF_GET_SE_CH1XOFST_OFST
def_get_mod_reg_bit(se, ch1xofst, ofst, addr);
#endif

#ifdef DEF_GET_SE_CH1YOFST_OFST
def_get_mod_reg_bit(se, ch1yofst, ofst, addr);
#endif

#ifdef DEF_GET_SE_SHIFTCTL_CTLX0
def_get_mod_reg_bit(se, shiftctl, ctlx0, addr);
#endif

#ifdef DEF_GET_SE_SHIFTCTL_CTLY0
def_get_mod_reg_bit(se, shiftctl, ctly0, addr);
#endif

#ifdef DEF_GET_SE_SHIFTCTL_CTLX1
def_get_mod_reg_bit(se, shiftctl, ctlx1, addr);
#endif

#ifdef DEF_GET_SE_SHIFTCTL_CTLY1
def_get_mod_reg_bit(se, shiftctl, ctly1, addr);
#endif

#ifdef DEF_GET_SE_OUT_CROPOFST_X
def_get_mod_reg_bit(se, out_cropofst, x, addr);
#endif

#ifdef DEF_GET_SE_OUT_CROPOFST_Y
def_get_mod_reg_bit(se, out_cropofst, y, addr);
#endif

#ifdef DEF_GET_SE_OUT_CROPSIZE_W
def_get_mod_reg_bit(se, out_cropsize, w, addr);
#endif

#ifdef DEF_GET_SE_OUT_CROPSIZE_H
def_get_mod_reg_bit(se, out_cropsize, h, addr);
#endif

#ifdef DEF_GET_SE_WBSIZE0_W
def_get_mod_reg_bit(se, wbsize0, w, addr);
#endif

#ifdef DEF_GET_SE_WBSIZE0_H
def_get_mod_reg_bit(se, wbsize0, h, addr);
#endif

#ifdef DEF_GET_SE_WBSIZE1_W
def_get_mod_reg_bit(se, wbsize1, w, addr);
#endif

#ifdef DEF_GET_SE_WBSIZE1_H
def_get_mod_reg_bit(se, wbsize1, h, addr);
#endif

#ifdef DEF_GET_SE_WBCTL_WB_START
def_get_mod_reg_bit(se, wbctl, wb_start, addr);
#endif

#ifdef DEF_GET_SE_WBCFG_WBFMT
def_get_mod_reg_bit(se, wbcfg, wbfmt, addr);
#endif

#ifdef DEF_GET_SE_WBCFG_WBTHR
def_get_mod_reg_bit(se, wbcfg, wbthr, addr);
#endif

#ifdef DEF_GET_SE_WBTMR_WBTMR
def_get_mod_reg_bit(se, wbtmr, wbtmr, addr);
#endif

#ifdef DEF_GET_SE_WBADDR0_ADDR
def_get_mod_reg_bit(se, wbaddr0, addr, addr);
#endif

#ifdef DEF_GET_SE_WBADDR1_ADDR
def_get_mod_reg_bit(se, wbaddr1, addr, addr);
#endif

#ifdef DEF_GET_SE_WBADDR2_ADDR
def_get_mod_reg_bit(se, wbaddr2, addr, addr);
#endif

#ifdef DEF_GET_SE_WBLSTR0_LSTR
def_get_mod_reg_bit(se, wblstr0, lstr, addr);
#endif

#ifdef DEF_GET_SE_WBLSTR1_LSTR
def_get_mod_reg_bit(se, wblstr1, lstr, addr);
#endif

#ifdef DEF_GET_SE_CSCI_COEF_COEF
def_get_mod_reg_bit(se, csci_coef, coef, addr);
#endif

#ifdef DEF_GET_SE_CSCO_COEF_COEF
def_get_mod_reg_bit(se, csco_coef, coef, addr);
#endif

#ifdef DEF_GET_SE_VPPCTL_LUT0EN
def_get_mod_reg_bit(se, vppctl, lut0en, addr);
#endif

#ifdef DEF_GET_SE_VPPCTL_LUT1EN
def_get_mod_reg_bit(se, vppctl, lut1en, addr);
#endif

#ifdef DEF_GET_SE_VPPCTL_LUT2EN
def_get_mod_reg_bit(se, vppctl, lut2en, addr);
#endif

#ifdef DEF_GET_SE_VPPCTL_LUT_MOD
def_get_mod_reg_bit(se, vppctl, lut_mod, addr);
#endif

#ifdef DEF_GET_SE_VPPCTL_E0EN
def_get_mod_reg_bit(se, vppctl, e0en, addr);
#endif

#ifdef DEF_GET_SE_VPPCTL_E1EN
def_get_mod_reg_bit(se, vppctl, e1en, addr);
#endif

#ifdef DEF_GET_SE_VPPCTL_E2EN
def_get_mod_reg_bit(se, vppctl, e2en, addr);
#endif

#ifdef DEF_GET_SE_VPPCTL_E3EN
def_get_mod_reg_bit(se, vppctl, e3en, addr);
#endif

#ifdef DEF_GET_SE_VPPCTL_HIST_SEL
def_get_mod_reg_bit(se, vppctl, hist_sel, addr);
#endif

#ifdef DEF_GET_SE_VPPCTL_HIST_OK
def_get_mod_reg_bit(se, vppctl, hist_ok, addr);
#endif

#ifdef DEF_GET_SE_VPPCTL_HIST_CLR
def_get_mod_reg_bit(se, vppctl, hist_clr, addr);
#endif

#ifdef DEF_GET_SE_VPPCTL_HIST_EN
def_get_mod_reg_bit(se, vppctl, hist_en, addr);
#endif

#ifdef DEF_GET_SE_VPPLUTSW_LUT0SW
def_get_mod_reg_bit(se, vpplutsw, lut0sw, addr);
#endif

#ifdef DEF_GET_SE_VPPLUTSW_LUT1SW
def_get_mod_reg_bit(se, vpplutsw, lut1sw, addr);
#endif

#ifdef DEF_GET_SE_VPPLUTSW_LUT2SW
def_get_mod_reg_bit(se, vpplutsw, lut2sw, addr);
#endif

#ifdef DEF_GET_SE_VPPLUTSW_HISTSW
def_get_mod_reg_bit(se, vpplutsw, histsw, addr);
#endif

#ifdef DEF_GET_SE_VPPLUTSW_CLK_LUT
def_get_mod_reg_bit(se, vpplutsw, clk_lut, addr);
#endif

#ifdef DEF_GET_SE_VPPLUTSW_CLK_HIST
def_get_mod_reg_bit(se, vpplutsw, clk_hist, addr);
#endif

#ifdef DEF_GET_SE_CHECFG0_CTHR0
def_get_mod_reg_bit(se, checfg0, cthr0, addr);
#endif

#ifdef DEF_GET_SE_CHECFG0_CTHR1
def_get_mod_reg_bit(se, checfg0, cthr1, addr);
#endif

#ifdef DEF_GET_SE_CHECFG0_CTHR2
def_get_mod_reg_bit(se, checfg0, cthr2, addr);
#endif

#ifdef DEF_GET_SE_CHECFG0_CTHR3
def_get_mod_reg_bit(se, checfg0, cthr3, addr);
#endif

#ifdef DEF_GET_SE_CHECFG1_SPEN
def_get_mod_reg_bit(se, checfg1, spen, addr);
#endif

#ifdef DEF_GET_SE_CHECFG1_CORD0
def_get_mod_reg_bit(se, checfg1, cord0, addr);
#endif

#ifdef DEF_GET_SE_CHECFG1_CORD1
def_get_mod_reg_bit(se, checfg1, cord1, addr);
#endif

#ifdef DEF_GET_SE_PEAKCFG_COR
def_get_mod_reg_bit(se, peakcfg, cor, addr);
#endif

#ifdef DEF_GET_SE_PEAKCFG_GAIN
def_get_mod_reg_bit(se, peakcfg, gain, addr);
#endif

#ifdef DEF_GET_SE_PEAKCFG_LIMIT
def_get_mod_reg_bit(se, peakcfg, limit, addr);
#endif

#ifdef DEF_GET_SE_LTIVCFG_COR
def_get_mod_reg_bit(se, ltivcfg, cor, addr);
#endif

#ifdef DEF_GET_SE_LTIVCFG_GAIN
def_get_mod_reg_bit(se, ltivcfg, gain, addr);
#endif

#ifdef DEF_GET_SE_LTIVCFG_GAIN2
def_get_mod_reg_bit(se, ltivcfg, gain2, addr);
#endif

#ifdef DEF_GET_SE_LTIVCFG_OSLMT
def_get_mod_reg_bit(se, ltivcfg, oslmt, addr);
#endif

#ifdef DEF_GET_SE_PEAKCOEF0_C0
def_get_mod_reg_bit(se, peakcoef0, c0, addr);
#endif

#ifdef DEF_GET_SE_PEAKCOEF0_C1
def_get_mod_reg_bit(se, peakcoef0, c1, addr);
#endif

#ifdef DEF_GET_SE_PEAKCOEF0_C2
def_get_mod_reg_bit(se, peakcoef0, c2, addr);
#endif

#ifdef DEF_GET_SE_PEAKCOEF0_C3
def_get_mod_reg_bit(se, peakcoef0, c3, addr);
#endif

#ifdef DEF_GET_SE_PEAKCOEF1_C4
def_get_mod_reg_bit(se, peakcoef1, c4, addr);
#endif

#ifdef DEF_GET_SE_LTICFG_COR
def_get_mod_reg_bit(se, lticfg, cor, addr);
#endif

#ifdef DEF_GET_SE_LTICFG_GAIN
def_get_mod_reg_bit(se, lticfg, gain, addr);
#endif

#ifdef DEF_GET_SE_LTICFG_GAIN2
def_get_mod_reg_bit(se, lticfg, gain2, addr);
#endif

#ifdef DEF_GET_SE_LTICFG_OSLMT
def_get_mod_reg_bit(se, lticfg, oslmt, addr);
#endif

#ifdef DEF_GET_SE_CTICFG_COR
def_get_mod_reg_bit(se, cticfg, cor, addr);
#endif

#ifdef DEF_GET_SE_CTICFG_GAIN
def_get_mod_reg_bit(se, cticfg, gain, addr);
#endif

#ifdef DEF_GET_SE_CTICFG_GAIN2
def_get_mod_reg_bit(se, cticfg, gain2, addr);
#endif

#ifdef DEF_GET_SE_CTICFG_OSLMT
def_get_mod_reg_bit(se, cticfg, oslmt, addr);
#endif

#ifdef DEF_GET_SE_LTICOEF0_C0
def_get_mod_reg_bit(se, lticoef0, c0, addr);
#endif

#ifdef DEF_GET_SE_LTICOEF0_C1
def_get_mod_reg_bit(se, lticoef0, c1, addr);
#endif

#ifdef DEF_GET_SE_LTICOEF0_C2
def_get_mod_reg_bit(se, lticoef0, c2, addr);
#endif

#ifdef DEF_GET_SE_LTICOEF0_C3
def_get_mod_reg_bit(se, lticoef0, c3, addr);
#endif

#ifdef DEF_GET_SE_LTICOEF1_C4
def_get_mod_reg_bit(se, lticoef1, c4, addr);
#endif

#ifdef DEF_GET_SE_CTICOEF0_C0
def_get_mod_reg_bit(se, cticoef0, c0, addr);
#endif

#ifdef DEF_GET_SE_CTICOEF0_C1
def_get_mod_reg_bit(se, cticoef0, c1, addr);
#endif

#ifdef DEF_GET_SE_CTICOEF0_C2
def_get_mod_reg_bit(se, cticoef0, c2, addr);
#endif

#ifdef DEF_GET_SE_CTICOEF0_C3
def_get_mod_reg_bit(se, cticoef0, c3, addr);
#endif

#ifdef DEF_GET_SE_CTICOEF1_C4
def_get_mod_reg_bit(se, cticoef1, c4, addr);
#endif

#ifdef DEF_GET_SE_CH0_XCOEF0_C0
def_get_mod_reg_bit(se, ch0_xcoef0, c0, addr);
#endif

#ifdef DEF_GET_SE_CH0_XCOEF0_C1
def_get_mod_reg_bit(se, ch0_xcoef0, c1, addr);
#endif

#ifdef DEF_GET_SE_CH0_XCOEF0_C2
def_get_mod_reg_bit(se, ch0_xcoef0, c2, addr);
#endif

#ifdef DEF_GET_SE_CH0_XCOEF0_C3
def_get_mod_reg_bit(se, ch0_xcoef0, c3, addr);
#endif

#ifdef DEF_GET_SE_CH0_XCOEF1_C4
def_get_mod_reg_bit(se, ch0_xcoef1, c4, addr);
#endif

#ifdef DEF_GET_SE_CH0_XCOEF1_C5
def_get_mod_reg_bit(se, ch0_xcoef1, c5, addr);
#endif

#ifdef DEF_GET_SE_CH0_XCOEF1_C6
def_get_mod_reg_bit(se, ch0_xcoef1, c6, addr);
#endif

#ifdef DEF_GET_SE_CH0_XCOEF1_C7
def_get_mod_reg_bit(se, ch0_xcoef1, c7, addr);
#endif

#ifdef DEF_GET_SE_CH0_YCOEF_C0
def_get_mod_reg_bit(se, ch0_ycoef, c0, addr);
#endif

#ifdef DEF_GET_SE_CH0_YCOEF_C1
def_get_mod_reg_bit(se, ch0_ycoef, c1, addr);
#endif

#ifdef DEF_GET_SE_CH0_YCOEF_C2
def_get_mod_reg_bit(se, ch0_ycoef, c2, addr);
#endif

#ifdef DEF_GET_SE_CH0_YCOEF_C3
def_get_mod_reg_bit(se, ch0_ycoef, c3, addr);
#endif

#ifdef DEF_GET_SE_CH1_XCOEF0_C0
def_get_mod_reg_bit(se, ch1_xcoef0, c0, addr);
#endif

#ifdef DEF_GET_SE_CH1_XCOEF0_C1
def_get_mod_reg_bit(se, ch1_xcoef0, c1, addr);
#endif

#ifdef DEF_GET_SE_CH1_XCOEF0_C2
def_get_mod_reg_bit(se, ch1_xcoef0, c2, addr);
#endif

#ifdef DEF_GET_SE_CH1_XCOEF0_C3
def_get_mod_reg_bit(se, ch1_xcoef0, c3, addr);
#endif

#ifdef DEF_GET_SE_CH1_XCOEF1_C4
def_get_mod_reg_bit(se, ch1_xcoef1, c4, addr);
#endif

#ifdef DEF_GET_SE_CH1_XCOEF1_C5
def_get_mod_reg_bit(se, ch1_xcoef1, c5, addr);
#endif

#ifdef DEF_GET_SE_CH1_XCOEF1_C6
def_get_mod_reg_bit(se, ch1_xcoef1, c6, addr);
#endif

#ifdef DEF_GET_SE_CH1_XCOEF1_C7
def_get_mod_reg_bit(se, ch1_xcoef1, c7, addr);
#endif

#ifdef DEF_GET_SE_CH1_YCOEF_C0
def_get_mod_reg_bit(se, ch1_ycoef, c0, addr);
#endif

#ifdef DEF_GET_SE_CH1_YCOEF_C1
def_get_mod_reg_bit(se, ch1_ycoef, c1, addr);
#endif

#ifdef DEF_GET_SE_CH1_YCOEF_C2
def_get_mod_reg_bit(se, ch1_ycoef, c2, addr);
#endif

#ifdef DEF_GET_SE_CH1_YCOEF_C3
def_get_mod_reg_bit(se, ch1_ycoef, c3, addr);
#endif

#ifdef DEF_GET_SE_CH0LUT_C0
def_get_mod_reg_bit(se, ch0lut, c0, addr);
#endif

#ifdef DEF_GET_SE_CH0LUT_C1
def_get_mod_reg_bit(se, ch0lut, c1, addr);
#endif

#ifdef DEF_GET_SE_CH0LUT_C2
def_get_mod_reg_bit(se, ch0lut, c2, addr);
#endif

#ifdef DEF_GET_SE_CH0LUT_C3
def_get_mod_reg_bit(se, ch0lut, c3, addr);
#endif

#ifdef DEF_GET_SE_CH1LUT_C0
def_get_mod_reg_bit(se, ch1lut, c0, addr);
#endif

#ifdef DEF_GET_SE_CH1LUT_C1
def_get_mod_reg_bit(se, ch1lut, c1, addr);
#endif

#ifdef DEF_GET_SE_CH1LUT_C2
def_get_mod_reg_bit(se, ch1lut, c2, addr);
#endif

#ifdef DEF_GET_SE_CH1LUT_C3
def_get_mod_reg_bit(se, ch1lut, c3, addr);
#endif

#ifdef DEF_GET_SE_CH2LUT_C0
def_get_mod_reg_bit(se, ch2lut, c0, addr);
#endif

#ifdef DEF_GET_SE_CH2LUT_C1
def_get_mod_reg_bit(se, ch2lut, c1, addr);
#endif

#ifdef DEF_GET_SE_CH2LUT_C2
def_get_mod_reg_bit(se, ch2lut, c2, addr);
#endif

#ifdef DEF_GET_SE_CH2LUT_C3
def_get_mod_reg_bit(se, ch2lut, c3, addr);
#endif

#ifdef DEF_GET_SE_CHHIST_COUNT
def_get_mod_reg_bit(se, chhist, count, addr);
#endif

#endif /* ___SE__GET___H___ */
