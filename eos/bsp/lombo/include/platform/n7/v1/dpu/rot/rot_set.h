/* rot_set.h */

#ifndef ___ROT__SET___H___
#define ___ROT__SET___H___

#ifdef DEF_SET_ROT_VER
void set_rot_ver(u32 reg_addr,
		u32 ver_l,
		u32 ver_h,
		u32 comp,
		u32 m_or_r);
#endif

#ifdef DEF_SET_ROT_VER_VER_L
def_set_mod_reg_bit(rot, ver, ver_l, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_VER_VER_H
def_set_mod_reg_bit(rot, ver, ver_h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_VER_COMP
def_set_mod_reg_bit(rot, ver, comp, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_FTR
void set_rot_ftr(u32 reg_addr,
		u32 m_or_r);
#endif

#ifdef DEF_SET_ROT_CTL
void set_rot_ctl(u32 reg_addr,
		u32 en,
		u32 bypass,
		u32 dmassuspd,
		u32 dmadsuspd,
		u32 rst,
		u32 m_or_r);
#endif

#ifdef DEF_SET_ROT_CTL_EN
def_set_mod_reg_bit(rot, ctl, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_CTL_BYPASS
def_set_mod_reg_bit(rot, ctl, bypass, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_CTL_DMASSUSPD
def_set_mod_reg_bit(rot, ctl, dmassuspd, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_CTL_DMADSUSPD
def_set_mod_reg_bit(rot, ctl, dmadsuspd, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_CTL_RST
def_set_mod_reg_bit(rot, ctl, rst, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_CFG
void set_rot_cfg(u32 reg_addr,
		u32 mode,
		u32 bpp0,
		u32 bpp1,
		u32 bpp2,
		u32 chsel,
		u32 m_or_r);
#endif

#ifdef DEF_SET_ROT_CFG_MODE
def_set_mod_reg_bit(rot, cfg, mode, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_CFG_BPP0
def_set_mod_reg_bit(rot, cfg, bpp0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_CFG_BPP1
def_set_mod_reg_bit(rot, cfg, bpp1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_CFG_BPP2
def_set_mod_reg_bit(rot, cfg, bpp2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_CFG_CHSEL
def_set_mod_reg_bit(rot, cfg, chsel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_STRMCTL
void set_rot_strmctl(u32 reg_addr,
			u32 start,
			u32 m_or_r);
#endif

#ifdef DEF_SET_ROT_STRMCTL_START
def_set_mod_reg_bit(rot, strmctl, start, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_STA
void set_rot_sta(u32 reg_addr,
		u32 insta,
		u32 outsta,
		u32 m_or_r);
#endif

#ifdef DEF_SET_ROT_STA_INSTA
def_set_mod_reg_bit(rot, sta, insta, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_STA_OUTSTA
def_set_mod_reg_bit(rot, sta, outsta, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INTEN
void set_rot_inten(u32 reg_addr,
		u32 srcblk,
		u32 srctran,
		u32 dstblk,
		u32 dsttran,
		u32 srcsuspd,
		u32 srcsuspdack,
		u32 dstsuspd,
		u32 dstsuspdack,
		u32 srcdecerr,
		u32 dstdecerr,
		u32 tmout,
		u32 fnsh,
		u32 m_or_r);
#endif

#ifdef DEF_SET_ROT_INTEN_SRCBLK
def_set_mod_reg_bit(rot, inten, srcblk, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INTEN_SRCTRAN
def_set_mod_reg_bit(rot, inten, srctran, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INTEN_DSTBLK
def_set_mod_reg_bit(rot, inten, dstblk, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INTEN_DSTTRAN
def_set_mod_reg_bit(rot, inten, dsttran, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INTEN_SRCSUSPD
def_set_mod_reg_bit(rot, inten, srcsuspd, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INTEN_SRCSUSPDACK
def_set_mod_reg_bit(rot, inten, srcsuspdack, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INTEN_DSTSUSPD
def_set_mod_reg_bit(rot, inten, dstsuspd, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INTEN_DSTSUSPDACK
def_set_mod_reg_bit(rot, inten, dstsuspdack, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INTEN_SRCDECERR
def_set_mod_reg_bit(rot, inten, srcdecerr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INTEN_DSTDECERR
def_set_mod_reg_bit(rot, inten, dstdecerr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INTEN_TMOUT
def_set_mod_reg_bit(rot, inten, tmout, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INTEN_FNSH
def_set_mod_reg_bit(rot, inten, fnsh, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INTPD
void set_rot_intpd(u32 reg_addr,
		u32 srcblk,
		u32 srctran,
		u32 dstblk,
		u32 dsttran,
		u32 srcsuspd,
		u32 srcsuspdack,
		u32 dstsuspd,
		u32 dstsuspdack,
		u32 srcdecerr,
		u32 dstdecerr,
		u32 tmout,
		u32 fnsh,
		u32 m_or_r);
#endif

#ifdef DEF_SET_ROT_INTPD_SRCBLK
def_set_mod_reg_bit(rot, intpd, srcblk, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INTPD_SRCTRAN
def_set_mod_reg_bit(rot, intpd, srctran, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INTPD_DSTBLK
def_set_mod_reg_bit(rot, intpd, dstblk, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INTPD_DSTTRAN
def_set_mod_reg_bit(rot, intpd, dsttran, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INTPD_SRCSUSPD
def_set_mod_reg_bit(rot, intpd, srcsuspd, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INTPD_SRCSUSPDACK
def_set_mod_reg_bit(rot, intpd, srcsuspdack, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INTPD_DSTSUSPD
def_set_mod_reg_bit(rot, intpd, dstsuspd, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INTPD_DSTSUSPDACK
def_set_mod_reg_bit(rot, intpd, dstsuspdack, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INTPD_SRCDECERR
def_set_mod_reg_bit(rot, intpd, srcdecerr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INTPD_DSTDECERR
def_set_mod_reg_bit(rot, intpd, dstdecerr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INTPD_TMOUT
def_set_mod_reg_bit(rot, intpd, tmout, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INTPD_FNSH
def_set_mod_reg_bit(rot, intpd, fnsh, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INTCLR
void set_rot_intclr(u32 reg_addr,
			u32 srcblk,
			u32 srctran,
			u32 dstblk,
			u32 dsttran,
			u32 srcsuspd,
			u32 srcsuspdack,
			u32 dstsuspd,
			u32 dstsuspdack,
			u32 srcdecerr,
			u32 dstdecerr,
			u32 tmout,
			u32 fnsh,
			u32 m_or_r);
#endif

#ifdef DEF_SET_ROT_INTCLR_SRCBLK
def_set_mod_reg_bit(rot, intclr, srcblk, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INTCLR_SRCTRAN
def_set_mod_reg_bit(rot, intclr, srctran, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INTCLR_DSTBLK
def_set_mod_reg_bit(rot, intclr, dstblk, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INTCLR_DSTTRAN
def_set_mod_reg_bit(rot, intclr, dsttran, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INTCLR_SRCSUSPD
def_set_mod_reg_bit(rot, intclr, srcsuspd, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INTCLR_SRCSUSPDACK
def_set_mod_reg_bit(rot, intclr, srcsuspdack, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INTCLR_DSTSUSPD
def_set_mod_reg_bit(rot, intclr, dstsuspd, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INTCLR_DSTSUSPDACK
def_set_mod_reg_bit(rot, intclr, dstsuspdack, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INTCLR_SRCDECERR
def_set_mod_reg_bit(rot, intclr, srcdecerr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INTCLR_DSTDECERR
def_set_mod_reg_bit(rot, intclr, dstdecerr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INTCLR_TMOUT
def_set_mod_reg_bit(rot, intclr, tmout, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INTCLR_FNSH
def_set_mod_reg_bit(rot, intclr, fnsh, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INCFG
void set_rot_incfg(u32 reg_addr,
		u32 m_or_r);
#endif

#ifdef DEF_SET_ROT_INSIZE0
void set_rot_insize0(u32 reg_addr,
			u32 w,
			u32 h,
			u32 m_or_r);
#endif

#ifdef DEF_SET_ROT_INSIZE0_W
def_set_mod_reg_bit(rot, insize0, w, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INSIZE0_H
def_set_mod_reg_bit(rot, insize0, h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INADD0
void set_rot_inadd0(u32 reg_addr,
			u32 add,
			u32 m_or_r);
#endif

#ifdef DEF_SET_ROT_INADD0_ADD
def_set_mod_reg_bit(rot, inadd0, add, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INLNSTD0
void set_rot_inlnstd0(u32 reg_addr,
			u32 std,
			u32 m_or_r);
#endif

#ifdef DEF_SET_ROT_INLNSTD0_STD
def_set_mod_reg_bit(rot, inlnstd0, std, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INSIZE1
void set_rot_insize1(u32 reg_addr,
			u32 w,
			u32 h,
			u32 m_or_r);
#endif

#ifdef DEF_SET_ROT_INSIZE1_W
def_set_mod_reg_bit(rot, insize1, w, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INSIZE1_H
def_set_mod_reg_bit(rot, insize1, h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INADD1
void set_rot_inadd1(u32 reg_addr,
			u32 add,
			u32 m_or_r);
#endif

#ifdef DEF_SET_ROT_INADD1_ADD
def_set_mod_reg_bit(rot, inadd1, add, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INLNSTD1
void set_rot_inlnstd1(u32 reg_addr,
			u32 std,
			u32 m_or_r);
#endif

#ifdef DEF_SET_ROT_INLNSTD1_STD
def_set_mod_reg_bit(rot, inlnstd1, std, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INSIZE2
void set_rot_insize2(u32 reg_addr,
			u32 w,
			u32 h,
			u32 m_or_r);
#endif

#ifdef DEF_SET_ROT_INSIZE2_W
def_set_mod_reg_bit(rot, insize2, w, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INSIZE2_H
def_set_mod_reg_bit(rot, insize2, h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INADD2
void set_rot_inadd2(u32 reg_addr,
			u32 add,
			u32 m_or_r);
#endif

#ifdef DEF_SET_ROT_INADD2_ADD
def_set_mod_reg_bit(rot, inadd2, add, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_INLNSTD2
void set_rot_inlnstd2(u32 reg_addr,
			u32 std,
			u32 m_or_r);
#endif

#ifdef DEF_SET_ROT_INLNSTD2_STD
def_set_mod_reg_bit(rot, inlnstd2, std, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_OUTCFG
void set_rot_outcfg(u32 reg_addr,
			u32 m_or_r);
#endif

#ifdef DEF_SET_ROT_OUTSIZE0
void set_rot_outsize0(u32 reg_addr,
			u32 w,
			u32 h,
			u32 m_or_r);
#endif

#ifdef DEF_SET_ROT_OUTSIZE0_W
def_set_mod_reg_bit(rot, outsize0, w, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_OUTSIZE0_H
def_set_mod_reg_bit(rot, outsize0, h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_OUTADD0
void set_rot_outadd0(u32 reg_addr,
			u32 add,
			u32 m_or_r);
#endif

#ifdef DEF_SET_ROT_OUTADD0_ADD
def_set_mod_reg_bit(rot, outadd0, add, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_OUTLNSTD0
void set_rot_outlnstd0(u32 reg_addr,
			u32 std,
			u32 m_or_r);
#endif

#ifdef DEF_SET_ROT_OUTLNSTD0_STD
def_set_mod_reg_bit(rot, outlnstd0, std, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_OUTSIZE1
void set_rot_outsize1(u32 reg_addr,
			u32 w,
			u32 h,
			u32 m_or_r);
#endif

#ifdef DEF_SET_ROT_OUTSIZE1_W
def_set_mod_reg_bit(rot, outsize1, w, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_OUTSIZE1_H
def_set_mod_reg_bit(rot, outsize1, h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_OUTADD1
void set_rot_outadd1(u32 reg_addr,
			u32 add,
			u32 m_or_r);
#endif

#ifdef DEF_SET_ROT_OUTADD1_ADD
def_set_mod_reg_bit(rot, outadd1, add, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_OUTLNSTD1
void set_rot_outlnstd1(u32 reg_addr,
			u32 std,
			u32 m_or_r);
#endif

#ifdef DEF_SET_ROT_OUTLNSTD1_STD
def_set_mod_reg_bit(rot, outlnstd1, std, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_OUTSIZE2
void set_rot_outsize2(u32 reg_addr,
			u32 w,
			u32 h,
			u32 m_or_r);
#endif

#ifdef DEF_SET_ROT_OUTSIZE2_W
def_set_mod_reg_bit(rot, outsize2, w, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_OUTSIZE2_H
def_set_mod_reg_bit(rot, outsize2, h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_OUTADD2
void set_rot_outadd2(u32 reg_addr,
			u32 add,
			u32 m_or_r);
#endif

#ifdef DEF_SET_ROT_OUTADD2_ADD
def_set_mod_reg_bit(rot, outadd2, add, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_OUTLNSTD2
void set_rot_outlnstd2(u32 reg_addr,
			u32 std,
			u32 m_or_r);
#endif

#ifdef DEF_SET_ROT_OUTLNSTD2_STD
def_set_mod_reg_bit(rot, outlnstd2, std, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_DMASCTL
void set_rot_dmasctl(u32 reg_addr,
			u32 bstsize,
			u32 cache,
			u32 prot,
			u32 bstlen,
			u32 fixbstlen,
			u32 m_or_r);
#endif

#ifdef DEF_SET_ROT_DMASCTL_BSTSIZE
def_set_mod_reg_bit(rot, dmasctl, bstsize, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_DMASCTL_CACHE
def_set_mod_reg_bit(rot, dmasctl, cache, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_DMASCTL_PROT
def_set_mod_reg_bit(rot, dmasctl, prot, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_DMASCTL_BSTLEN
def_set_mod_reg_bit(rot, dmasctl, bstlen, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_DMASCTL_FIXBSTLEN
def_set_mod_reg_bit(rot, dmasctl, fixbstlen, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_DMADCTL
void set_rot_dmadctl(u32 reg_addr,
			u32 bstsize,
			u32 cache,
			u32 prot,
			u32 bstlen,
			u32 fixbstlen,
			u32 m_or_r);
#endif

#ifdef DEF_SET_ROT_DMADCTL_BSTSIZE
def_set_mod_reg_bit(rot, dmadctl, bstsize, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_DMADCTL_CACHE
def_set_mod_reg_bit(rot, dmadctl, cache, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_DMADCTL_PROT
def_set_mod_reg_bit(rot, dmadctl, prot, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_DMADCTL_BSTLEN
def_set_mod_reg_bit(rot, dmadctl, bstlen, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_DMADCTL_FIXBSTLEN
def_set_mod_reg_bit(rot, dmadctl, fixbstlen, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_DMASRCCFG0
void set_rot_dmasrccfg0(u32 reg_addr,
			u32 prio,
			u32 rpio2,
			u32 osrlmt,
			u32 osrlmt2,
			u32 conlen0,
			u32 m_or_r);
#endif

#ifdef DEF_SET_ROT_DMASRCCFG0_PRIO
def_set_mod_reg_bit(rot, dmasrccfg0, prio, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_DMASRCCFG0_RPIO2
def_set_mod_reg_bit(rot, dmasrccfg0, rpio2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_DMASRCCFG0_OSRLMT
def_set_mod_reg_bit(rot, dmasrccfg0, osrlmt, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_DMASRCCFG0_OSRLMT2
def_set_mod_reg_bit(rot, dmasrccfg0, osrlmt2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_DMASRCCFG0_CONLEN0
def_set_mod_reg_bit(rot, dmasrccfg0, conlen0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_DMASRCCFG1
void set_rot_dmasrccfg1(u32 reg_addr,
			u32 conlen1,
			u32 conlen2,
			u32 m_or_r);
#endif

#ifdef DEF_SET_ROT_DMASRCCFG1_CONLEN1
def_set_mod_reg_bit(rot, dmasrccfg1, conlen1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_DMASRCCFG1_CONLEN2
def_set_mod_reg_bit(rot, dmasrccfg1, conlen2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_DMADSTCFG0
void set_rot_dmadstcfg0(u32 reg_addr,
			u32 prio,
			u32 rpio2,
			u32 osrlmt,
			u32 osrlmt2,
			u32 conlen0,
			u32 m_or_r);
#endif

#ifdef DEF_SET_ROT_DMADSTCFG0_PRIO
def_set_mod_reg_bit(rot, dmadstcfg0, prio, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_DMADSTCFG0_RPIO2
def_set_mod_reg_bit(rot, dmadstcfg0, rpio2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_DMADSTCFG0_OSRLMT
def_set_mod_reg_bit(rot, dmadstcfg0, osrlmt, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_DMADSTCFG0_OSRLMT2
def_set_mod_reg_bit(rot, dmadstcfg0, osrlmt2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_DMADSTCFG0_CONLEN0
def_set_mod_reg_bit(rot, dmadstcfg0, conlen0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_DMADSTCFG1
void set_rot_dmadstcfg1(u32 reg_addr,
			u32 conlen1,
			u32 conlen2,
			u32 m_or_r);
#endif

#ifdef DEF_SET_ROT_DMADSTCFG1_CONLEN1
def_set_mod_reg_bit(rot, dmadstcfg1, conlen1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_DMADSTCFG1_CONLEN2
def_set_mod_reg_bit(rot, dmadstcfg1, conlen2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_DBG0
void set_rot_dbg0(u32 reg_addr,
		u32 thr,
		u32 tmr,
		u32 m_or_r);
#endif

#ifdef DEF_SET_ROT_DBG0_THR
def_set_mod_reg_bit(rot, dbg0, thr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_ROT_DBG0_TMR
def_set_mod_reg_bit(rot, dbg0, tmr, addr, val, m_or_r);
#endif

#endif /* ___ROT__SET___H___ */
