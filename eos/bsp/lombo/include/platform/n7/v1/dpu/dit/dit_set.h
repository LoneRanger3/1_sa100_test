/* dit_set.h */

#ifndef ___DIT__SET___H___
#define ___DIT__SET___H___

#ifdef DEF_SET_DIT_VER
void set_dit_ver(u32 reg_addr,
		u32 ver_l,
		u32 ver_h,
		u32 comp,
		u32 m_or_r);
#endif

#ifdef DEF_SET_DIT_VER_VER_L
def_set_mod_reg_bit(dit, ver, ver_l, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_VER_VER_H
def_set_mod_reg_bit(dit, ver, ver_h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_VER_COMP
def_set_mod_reg_bit(dit, ver, comp, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_FTR
void set_dit_ftr(u32 reg_addr,
		u32 has_ftr0,
		u32 has_ftr1,
		u32 has_ftr2,
		u32 m_or_r);
#endif

#ifdef DEF_SET_DIT_FTR_HAS_FTR0
def_set_mod_reg_bit(dit, ftr, has_ftr0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_FTR_HAS_FTR1
def_set_mod_reg_bit(dit, ftr, has_ftr1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_FTR_HAS_FTR2
def_set_mod_reg_bit(dit, ftr, has_ftr2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_STA
void set_dit_sta(u32 reg_addr,
		u32 rdsta,
		u32 wrsta,
		u32 m_or_r);
#endif

#ifdef DEF_SET_DIT_STA_RDSTA
def_set_mod_reg_bit(dit, sta, rdsta, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_STA_WRSTA
def_set_mod_reg_bit(dit, sta, wrsta, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_CTL
void set_dit_ctl(u32 reg_addr,
		u32 rst,
		u32 bisten,
		u32 dbg_ctl,
		u32 dit_en,
		u32 m_or_r);
#endif

#ifdef DEF_SET_DIT_CTL_RST
def_set_mod_reg_bit(dit, ctl, rst, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_CTL_BISTEN
def_set_mod_reg_bit(dit, ctl, bisten, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_CTL_DBG_CTL
def_set_mod_reg_bit(dit, ctl, dbg_ctl, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_CTL_DIT_EN
def_set_mod_reg_bit(dit, ctl, dit_en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_CFG
void set_dit_cfg(u32 reg_addr,
		u32 ditmode,
		u32 pixseq,
		u32 field_pol,
		u32 m_or_r);
#endif

#ifdef DEF_SET_DIT_CFG_DITMODE
def_set_mod_reg_bit(dit, cfg, ditmode, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_CFG_PIXSEQ
def_set_mod_reg_bit(dit, cfg, pixseq, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_CFG_FIELD_POL
def_set_mod_reg_bit(dit, cfg, field_pol, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_WBCTL
void set_dit_wbctl(u32 reg_addr,
		u32 start,
		u32 m_or_r);
#endif

#ifdef DEF_SET_DIT_WBCTL_START
def_set_mod_reg_bit(dit, wbctl, start, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_WBTMR
void set_dit_wbtmr(u32 reg_addr,
		u32 wbthr,
		u32 wbtmr,
		u32 m_or_r);
#endif

#ifdef DEF_SET_DIT_WBTMR_WBTHR
def_set_mod_reg_bit(dit, wbtmr, wbthr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_WBTMR_WBTMR
def_set_mod_reg_bit(dit, wbtmr, wbtmr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_INADDR0
void set_dit_inaddr0(u32 reg_addr,
			u32 addr,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DIT_INADDR0_ADDR
def_set_mod_reg_bit(dit, inaddr0, addr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_INADDR1
void set_dit_inaddr1(u32 reg_addr,
			u32 addr,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DIT_INADDR1_ADDR
def_set_mod_reg_bit(dit, inaddr1, addr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_INADDR2
void set_dit_inaddr2(u32 reg_addr,
			u32 addr,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DIT_INADDR2_ADDR
def_set_mod_reg_bit(dit, inaddr2, addr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_INADDR3
void set_dit_inaddr3(u32 reg_addr,
			u32 addr,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DIT_INADDR3_ADDR
def_set_mod_reg_bit(dit, inaddr3, addr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_OUTADDR
void set_dit_outaddr(u32 reg_addr,
			u32 addr,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DIT_OUTADDR_ADDR
def_set_mod_reg_bit(dit, outaddr, addr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_INSIZE
void set_dit_insize(u32 reg_addr,
			u32 w,
			u32 h,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DIT_INSIZE_W
def_set_mod_reg_bit(dit, insize, w, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_INSIZE_H
def_set_mod_reg_bit(dit, insize, h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_OUTSIZE
void set_dit_outsize(u32 reg_addr,
			u32 w,
			u32 h,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DIT_OUTSIZE_W
def_set_mod_reg_bit(dit, outsize, w, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_OUTSIZE_H
def_set_mod_reg_bit(dit, outsize, h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_LSTR
void set_dit_lstr(u32 reg_addr,
		u32 ilstr,
		u32 olstr,
		u32 m_or_r);
#endif

#ifdef DEF_SET_DIT_LSTR_ILSTR
def_set_mod_reg_bit(dit, lstr, ilstr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_LSTR_OLSTR
def_set_mod_reg_bit(dit, lstr, olstr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_WBINTCTL
void set_dit_wbintctl(u32 reg_addr,
			u32 wbfin_en,
			u32 wbovfl_en,
			u32 wbtmout_en,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DIT_WBINTCTL_WBFIN_EN
def_set_mod_reg_bit(dit, wbintctl, wbfin_en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_WBINTCTL_WBOVFL_EN
def_set_mod_reg_bit(dit, wbintctl, wbovfl_en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_WBINTCTL_WBTMOUT_EN
def_set_mod_reg_bit(dit, wbintctl, wbtmout_en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_WBINTSTA
void set_dit_wbintsta(u32 reg_addr,
			u32 wbfin,
			u32 wbovfl,
			u32 wbtmout,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DIT_WBINTSTA_WBFIN
def_set_mod_reg_bit(dit, wbintsta, wbfin, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_WBINTSTA_WBOVFL
def_set_mod_reg_bit(dit, wbintsta, wbovfl, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_WBINTSTA_WBTMOUT
def_set_mod_reg_bit(dit, wbintsta, wbtmout, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_WBINTCLR
void set_dit_wbintclr(u32 reg_addr,
			u32 wbfinclr,
			u32 wbovflclr,
			u32 wbtmoutclr,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DIT_WBINTCLR_WBFINCLR
def_set_mod_reg_bit(dit, wbintclr, wbfinclr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_WBINTCLR_WBOVFLCLR
def_set_mod_reg_bit(dit, wbintclr, wbovflclr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_WBINTCLR_WBTMOUTCLR
def_set_mod_reg_bit(dit, wbintclr, wbtmoutclr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_STHR
void set_dit_sthr(u32 reg_addr,
		u32 sthr0,
		u32 stavg,
		u32 m_or_r);
#endif

#ifdef DEF_SET_DIT_STHR_STHR0
def_set_mod_reg_bit(dit, sthr, sthr0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_STHR_STAVG
def_set_mod_reg_bit(dit, sthr, stavg, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_ITHR
void set_dit_ithr(u32 reg_addr,
		u32 thr0,
		u32 thr1,
		u32 thr2,
		u32 thr3,
		u32 m_or_r);
#endif

#ifdef DEF_SET_DIT_ITHR_THR0
def_set_mod_reg_bit(dit, ithr, thr0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_ITHR_THR1
def_set_mod_reg_bit(dit, ithr, thr1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_ITHR_THR2
def_set_mod_reg_bit(dit, ithr, thr2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_ITHR_THR3
def_set_mod_reg_bit(dit, ithr, thr3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DIT_SPCNT
void set_dit_spcnt(u32 reg_addr,
		u32 cnt,
		u32 m_or_r);
#endif

#ifdef DEF_SET_DIT_SPCNT_CNT
def_set_mod_reg_bit(dit, spcnt, cnt, addr, val, m_or_r);
#endif

#endif /* ___DIT__SET___H___ */
