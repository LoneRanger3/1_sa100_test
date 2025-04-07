/* dit_get.h */

#ifndef ___DIT__GET___H___
#define ___DIT__GET___H___

#ifdef DEF_GET_DIT_VER_VER_L
def_get_mod_reg_bit(dit, ver, ver_l, addr);
#endif

#ifdef DEF_GET_DIT_VER_VER_H
def_get_mod_reg_bit(dit, ver, ver_h, addr);
#endif

#ifdef DEF_GET_DIT_VER_COMP
def_get_mod_reg_bit(dit, ver, comp, addr);
#endif

#ifdef DEF_GET_DIT_FTR_HAS_FTR0
def_get_mod_reg_bit(dit, ftr, has_ftr0, addr);
#endif

#ifdef DEF_GET_DIT_FTR_HAS_FTR1
def_get_mod_reg_bit(dit, ftr, has_ftr1, addr);
#endif

#ifdef DEF_GET_DIT_FTR_HAS_FTR2
def_get_mod_reg_bit(dit, ftr, has_ftr2, addr);
#endif

#ifdef DEF_GET_DIT_STA_RDSTA
def_get_mod_reg_bit(dit, sta, rdsta, addr);
#endif

#ifdef DEF_GET_DIT_STA_WRSTA
def_get_mod_reg_bit(dit, sta, wrsta, addr);
#endif

#ifdef DEF_GET_DIT_CTL_RST
def_get_mod_reg_bit(dit, ctl, rst, addr);
#endif

#ifdef DEF_GET_DIT_CTL_BISTEN
def_get_mod_reg_bit(dit, ctl, bisten, addr);
#endif

#ifdef DEF_GET_DIT_CTL_DBG_CTL
def_get_mod_reg_bit(dit, ctl, dbg_ctl, addr);
#endif

#ifdef DEF_GET_DIT_CTL_DIT_EN
def_get_mod_reg_bit(dit, ctl, dit_en, addr);
#endif

#ifdef DEF_GET_DIT_CFG_DITMODE
def_get_mod_reg_bit(dit, cfg, ditmode, addr);
#endif

#ifdef DEF_GET_DIT_CFG_PIXSEQ
def_get_mod_reg_bit(dit, cfg, pixseq, addr);
#endif

#ifdef DEF_GET_DIT_CFG_FIELD_POL
def_get_mod_reg_bit(dit, cfg, field_pol, addr);
#endif

#ifdef DEF_GET_DIT_WBCTL_START
def_get_mod_reg_bit(dit, wbctl, start, addr);
#endif

#ifdef DEF_GET_DIT_WBTMR_WBTHR
def_get_mod_reg_bit(dit, wbtmr, wbthr, addr);
#endif

#ifdef DEF_GET_DIT_WBTMR_WBTMR
def_get_mod_reg_bit(dit, wbtmr, wbtmr, addr);
#endif

#ifdef DEF_GET_DIT_INADDR0_ADDR
def_get_mod_reg_bit(dit, inaddr0, addr, addr);
#endif

#ifdef DEF_GET_DIT_INADDR1_ADDR
def_get_mod_reg_bit(dit, inaddr1, addr, addr);
#endif

#ifdef DEF_GET_DIT_INADDR2_ADDR
def_get_mod_reg_bit(dit, inaddr2, addr, addr);
#endif

#ifdef DEF_GET_DIT_INADDR3_ADDR
def_get_mod_reg_bit(dit, inaddr3, addr, addr);
#endif

#ifdef DEF_GET_DIT_OUTADDR_ADDR
def_get_mod_reg_bit(dit, outaddr, addr, addr);
#endif

#ifdef DEF_GET_DIT_INSIZE_W
def_get_mod_reg_bit(dit, insize, w, addr);
#endif

#ifdef DEF_GET_DIT_INSIZE_H
def_get_mod_reg_bit(dit, insize, h, addr);
#endif

#ifdef DEF_GET_DIT_OUTSIZE_W
def_get_mod_reg_bit(dit, outsize, w, addr);
#endif

#ifdef DEF_GET_DIT_OUTSIZE_H
def_get_mod_reg_bit(dit, outsize, h, addr);
#endif

#ifdef DEF_GET_DIT_LSTR_ILSTR
def_get_mod_reg_bit(dit, lstr, ilstr, addr);
#endif

#ifdef DEF_GET_DIT_LSTR_OLSTR
def_get_mod_reg_bit(dit, lstr, olstr, addr);
#endif

#ifdef DEF_GET_DIT_WBINTCTL_WBFIN_EN
def_get_mod_reg_bit(dit, wbintctl, wbfin_en, addr);
#endif

#ifdef DEF_GET_DIT_WBINTCTL_WBOVFL_EN
def_get_mod_reg_bit(dit, wbintctl, wbovfl_en, addr);
#endif

#ifdef DEF_GET_DIT_WBINTCTL_WBTMOUT_EN
def_get_mod_reg_bit(dit, wbintctl, wbtmout_en, addr);
#endif

#ifdef DEF_GET_DIT_WBINTSTA_WBFIN
def_get_mod_reg_bit(dit, wbintsta, wbfin, addr);
#endif

#ifdef DEF_GET_DIT_WBINTSTA_WBOVFL
def_get_mod_reg_bit(dit, wbintsta, wbovfl, addr);
#endif

#ifdef DEF_GET_DIT_WBINTSTA_WBTMOUT
def_get_mod_reg_bit(dit, wbintsta, wbtmout, addr);
#endif

#ifdef DEF_GET_DIT_WBINTCLR_WBFINCLR
def_get_mod_reg_bit(dit, wbintclr, wbfinclr, addr);
#endif

#ifdef DEF_GET_DIT_WBINTCLR_WBOVFLCLR
def_get_mod_reg_bit(dit, wbintclr, wbovflclr, addr);
#endif

#ifdef DEF_GET_DIT_WBINTCLR_WBTMOUTCLR
def_get_mod_reg_bit(dit, wbintclr, wbtmoutclr, addr);
#endif

#ifdef DEF_GET_DIT_STHR_STHR0
def_get_mod_reg_bit(dit, sthr, sthr0, addr);
#endif

#ifdef DEF_GET_DIT_STHR_STAVG
def_get_mod_reg_bit(dit, sthr, stavg, addr);
#endif

#ifdef DEF_GET_DIT_ITHR_THR0
def_get_mod_reg_bit(dit, ithr, thr0, addr);
#endif

#ifdef DEF_GET_DIT_ITHR_THR1
def_get_mod_reg_bit(dit, ithr, thr1, addr);
#endif

#ifdef DEF_GET_DIT_ITHR_THR2
def_get_mod_reg_bit(dit, ithr, thr2, addr);
#endif

#ifdef DEF_GET_DIT_ITHR_THR3
def_get_mod_reg_bit(dit, ithr, thr3, addr);
#endif

#ifdef DEF_GET_DIT_SPCNT_CNT
def_get_mod_reg_bit(dit, spcnt, cnt, addr);
#endif

#endif /* ___DIT__GET___H___ */
