/* rot_get.h */

#ifndef ___ROT__GET___H___
#define ___ROT__GET___H___

#ifdef DEF_GET_ROT_VER_VER_L
def_get_mod_reg_bit(rot, ver, ver_l, addr);
#endif

#ifdef DEF_GET_ROT_VER_VER_H
def_get_mod_reg_bit(rot, ver, ver_h, addr);
#endif

#ifdef DEF_GET_ROT_VER_COMP
def_get_mod_reg_bit(rot, ver, comp, addr);
#endif

#ifdef DEF_GET_ROT_CTL_EN
def_get_mod_reg_bit(rot, ctl, en, addr);
#endif

#ifdef DEF_GET_ROT_CTL_BYPASS
def_get_mod_reg_bit(rot, ctl, bypass, addr);
#endif

#ifdef DEF_GET_ROT_CTL_DMASSUSPD
def_get_mod_reg_bit(rot, ctl, dmassuspd, addr);
#endif

#ifdef DEF_GET_ROT_CTL_DMADSUSPD
def_get_mod_reg_bit(rot, ctl, dmadsuspd, addr);
#endif

#ifdef DEF_GET_ROT_CTL_RST
def_get_mod_reg_bit(rot, ctl, rst, addr);
#endif

#ifdef DEF_GET_ROT_CFG_MODE
def_get_mod_reg_bit(rot, cfg, mode, addr);
#endif

#ifdef DEF_GET_ROT_CFG_BPP0
def_get_mod_reg_bit(rot, cfg, bpp0, addr);
#endif

#ifdef DEF_GET_ROT_CFG_BPP1
def_get_mod_reg_bit(rot, cfg, bpp1, addr);
#endif

#ifdef DEF_GET_ROT_CFG_BPP2
def_get_mod_reg_bit(rot, cfg, bpp2, addr);
#endif

#ifdef DEF_GET_ROT_CFG_CHSEL
def_get_mod_reg_bit(rot, cfg, chsel, addr);
#endif

#ifdef DEF_GET_ROT_STRMCTL_START
def_get_mod_reg_bit(rot, strmctl, start, addr);
#endif

#ifdef DEF_GET_ROT_STA_INSTA
def_get_mod_reg_bit(rot, sta, insta, addr);
#endif

#ifdef DEF_GET_ROT_STA_OUTSTA
def_get_mod_reg_bit(rot, sta, outsta, addr);
#endif

#ifdef DEF_GET_ROT_INTEN_SRCBLK
def_get_mod_reg_bit(rot, inten, srcblk, addr);
#endif

#ifdef DEF_GET_ROT_INTEN_SRCTRAN
def_get_mod_reg_bit(rot, inten, srctran, addr);
#endif

#ifdef DEF_GET_ROT_INTEN_DSTBLK
def_get_mod_reg_bit(rot, inten, dstblk, addr);
#endif

#ifdef DEF_GET_ROT_INTEN_DSTTRAN
def_get_mod_reg_bit(rot, inten, dsttran, addr);
#endif

#ifdef DEF_GET_ROT_INTEN_SRCSUSPD
def_get_mod_reg_bit(rot, inten, srcsuspd, addr);
#endif

#ifdef DEF_GET_ROT_INTEN_SRCSUSPDACK
def_get_mod_reg_bit(rot, inten, srcsuspdack, addr);
#endif

#ifdef DEF_GET_ROT_INTEN_DSTSUSPD
def_get_mod_reg_bit(rot, inten, dstsuspd, addr);
#endif

#ifdef DEF_GET_ROT_INTEN_DSTSUSPDACK
def_get_mod_reg_bit(rot, inten, dstsuspdack, addr);
#endif

#ifdef DEF_GET_ROT_INTEN_SRCDECERR
def_get_mod_reg_bit(rot, inten, srcdecerr, addr);
#endif

#ifdef DEF_GET_ROT_INTEN_DSTDECERR
def_get_mod_reg_bit(rot, inten, dstdecerr, addr);
#endif

#ifdef DEF_GET_ROT_INTEN_TMOUT
def_get_mod_reg_bit(rot, inten, tmout, addr);
#endif

#ifdef DEF_GET_ROT_INTEN_FNSH
def_get_mod_reg_bit(rot, inten, fnsh, addr);
#endif

#ifdef DEF_GET_ROT_INTPD_SRCBLK
def_get_mod_reg_bit(rot, intpd, srcblk, addr);
#endif

#ifdef DEF_GET_ROT_INTPD_SRCTRAN
def_get_mod_reg_bit(rot, intpd, srctran, addr);
#endif

#ifdef DEF_GET_ROT_INTPD_DSTBLK
def_get_mod_reg_bit(rot, intpd, dstblk, addr);
#endif

#ifdef DEF_GET_ROT_INTPD_DSTTRAN
def_get_mod_reg_bit(rot, intpd, dsttran, addr);
#endif

#ifdef DEF_GET_ROT_INTPD_SRCSUSPD
def_get_mod_reg_bit(rot, intpd, srcsuspd, addr);
#endif

#ifdef DEF_GET_ROT_INTPD_SRCSUSPDACK
def_get_mod_reg_bit(rot, intpd, srcsuspdack, addr);
#endif

#ifdef DEF_GET_ROT_INTPD_DSTSUSPD
def_get_mod_reg_bit(rot, intpd, dstsuspd, addr);
#endif

#ifdef DEF_GET_ROT_INTPD_DSTSUSPDACK
def_get_mod_reg_bit(rot, intpd, dstsuspdack, addr);
#endif

#ifdef DEF_GET_ROT_INTPD_SRCDECERR
def_get_mod_reg_bit(rot, intpd, srcdecerr, addr);
#endif

#ifdef DEF_GET_ROT_INTPD_DSTDECERR
def_get_mod_reg_bit(rot, intpd, dstdecerr, addr);
#endif

#ifdef DEF_GET_ROT_INTPD_TMOUT
def_get_mod_reg_bit(rot, intpd, tmout, addr);
#endif

#ifdef DEF_GET_ROT_INTPD_FNSH
def_get_mod_reg_bit(rot, intpd, fnsh, addr);
#endif

#ifdef DEF_GET_ROT_INTCLR_SRCBLK
def_get_mod_reg_bit(rot, intclr, srcblk, addr);
#endif

#ifdef DEF_GET_ROT_INTCLR_SRCTRAN
def_get_mod_reg_bit(rot, intclr, srctran, addr);
#endif

#ifdef DEF_GET_ROT_INTCLR_DSTBLK
def_get_mod_reg_bit(rot, intclr, dstblk, addr);
#endif

#ifdef DEF_GET_ROT_INTCLR_DSTTRAN
def_get_mod_reg_bit(rot, intclr, dsttran, addr);
#endif

#ifdef DEF_GET_ROT_INTCLR_SRCSUSPD
def_get_mod_reg_bit(rot, intclr, srcsuspd, addr);
#endif

#ifdef DEF_GET_ROT_INTCLR_SRCSUSPDACK
def_get_mod_reg_bit(rot, intclr, srcsuspdack, addr);
#endif

#ifdef DEF_GET_ROT_INTCLR_DSTSUSPD
def_get_mod_reg_bit(rot, intclr, dstsuspd, addr);
#endif

#ifdef DEF_GET_ROT_INTCLR_DSTSUSPDACK
def_get_mod_reg_bit(rot, intclr, dstsuspdack, addr);
#endif

#ifdef DEF_GET_ROT_INTCLR_SRCDECERR
def_get_mod_reg_bit(rot, intclr, srcdecerr, addr);
#endif

#ifdef DEF_GET_ROT_INTCLR_DSTDECERR
def_get_mod_reg_bit(rot, intclr, dstdecerr, addr);
#endif

#ifdef DEF_GET_ROT_INTCLR_TMOUT
def_get_mod_reg_bit(rot, intclr, tmout, addr);
#endif

#ifdef DEF_GET_ROT_INTCLR_FNSH
def_get_mod_reg_bit(rot, intclr, fnsh, addr);
#endif

#ifdef DEF_GET_ROT_INSIZE0_W
def_get_mod_reg_bit(rot, insize0, w, addr);
#endif

#ifdef DEF_GET_ROT_INSIZE0_H
def_get_mod_reg_bit(rot, insize0, h, addr);
#endif

#ifdef DEF_GET_ROT_INADD0_ADD
def_get_mod_reg_bit(rot, inadd0, add, addr);
#endif

#ifdef DEF_GET_ROT_INLNSTD0_STD
def_get_mod_reg_bit(rot, inlnstd0, std, addr);
#endif

#ifdef DEF_GET_ROT_INSIZE1_W
def_get_mod_reg_bit(rot, insize1, w, addr);
#endif

#ifdef DEF_GET_ROT_INSIZE1_H
def_get_mod_reg_bit(rot, insize1, h, addr);
#endif

#ifdef DEF_GET_ROT_INADD1_ADD
def_get_mod_reg_bit(rot, inadd1, add, addr);
#endif

#ifdef DEF_GET_ROT_INLNSTD1_STD
def_get_mod_reg_bit(rot, inlnstd1, std, addr);
#endif

#ifdef DEF_GET_ROT_INSIZE2_W
def_get_mod_reg_bit(rot, insize2, w, addr);
#endif

#ifdef DEF_GET_ROT_INSIZE2_H
def_get_mod_reg_bit(rot, insize2, h, addr);
#endif

#ifdef DEF_GET_ROT_INADD2_ADD
def_get_mod_reg_bit(rot, inadd2, add, addr);
#endif

#ifdef DEF_GET_ROT_INLNSTD2_STD
def_get_mod_reg_bit(rot, inlnstd2, std, addr);
#endif

#ifdef DEF_GET_ROT_OUTSIZE0_W
def_get_mod_reg_bit(rot, outsize0, w, addr);
#endif

#ifdef DEF_GET_ROT_OUTSIZE0_H
def_get_mod_reg_bit(rot, outsize0, h, addr);
#endif

#ifdef DEF_GET_ROT_OUTADD0_ADD
def_get_mod_reg_bit(rot, outadd0, add, addr);
#endif

#ifdef DEF_GET_ROT_OUTLNSTD0_STD
def_get_mod_reg_bit(rot, outlnstd0, std, addr);
#endif

#ifdef DEF_GET_ROT_OUTSIZE1_W
def_get_mod_reg_bit(rot, outsize1, w, addr);
#endif

#ifdef DEF_GET_ROT_OUTSIZE1_H
def_get_mod_reg_bit(rot, outsize1, h, addr);
#endif

#ifdef DEF_GET_ROT_OUTADD1_ADD
def_get_mod_reg_bit(rot, outadd1, add, addr);
#endif

#ifdef DEF_GET_ROT_OUTLNSTD1_STD
def_get_mod_reg_bit(rot, outlnstd1, std, addr);
#endif

#ifdef DEF_GET_ROT_OUTSIZE2_W
def_get_mod_reg_bit(rot, outsize2, w, addr);
#endif

#ifdef DEF_GET_ROT_OUTSIZE2_H
def_get_mod_reg_bit(rot, outsize2, h, addr);
#endif

#ifdef DEF_GET_ROT_OUTADD2_ADD
def_get_mod_reg_bit(rot, outadd2, add, addr);
#endif

#ifdef DEF_GET_ROT_OUTLNSTD2_STD
def_get_mod_reg_bit(rot, outlnstd2, std, addr);
#endif

#ifdef DEF_GET_ROT_DMASCTL_BSTSIZE
def_get_mod_reg_bit(rot, dmasctl, bstsize, addr);
#endif

#ifdef DEF_GET_ROT_DMASCTL_CACHE
def_get_mod_reg_bit(rot, dmasctl, cache, addr);
#endif

#ifdef DEF_GET_ROT_DMASCTL_PROT
def_get_mod_reg_bit(rot, dmasctl, prot, addr);
#endif

#ifdef DEF_GET_ROT_DMASCTL_BSTLEN
def_get_mod_reg_bit(rot, dmasctl, bstlen, addr);
#endif

#ifdef DEF_GET_ROT_DMASCTL_FIXBSTLEN
def_get_mod_reg_bit(rot, dmasctl, fixbstlen, addr);
#endif

#ifdef DEF_GET_ROT_DMADCTL_BSTSIZE
def_get_mod_reg_bit(rot, dmadctl, bstsize, addr);
#endif

#ifdef DEF_GET_ROT_DMADCTL_CACHE
def_get_mod_reg_bit(rot, dmadctl, cache, addr);
#endif

#ifdef DEF_GET_ROT_DMADCTL_PROT
def_get_mod_reg_bit(rot, dmadctl, prot, addr);
#endif

#ifdef DEF_GET_ROT_DMADCTL_BSTLEN
def_get_mod_reg_bit(rot, dmadctl, bstlen, addr);
#endif

#ifdef DEF_GET_ROT_DMADCTL_FIXBSTLEN
def_get_mod_reg_bit(rot, dmadctl, fixbstlen, addr);
#endif

#ifdef DEF_GET_ROT_DMASRCCFG0_PRIO
def_get_mod_reg_bit(rot, dmasrccfg0, prio, addr);
#endif

#ifdef DEF_GET_ROT_DMASRCCFG0_RPIO2
def_get_mod_reg_bit(rot, dmasrccfg0, rpio2, addr);
#endif

#ifdef DEF_GET_ROT_DMASRCCFG0_OSRLMT
def_get_mod_reg_bit(rot, dmasrccfg0, osrlmt, addr);
#endif

#ifdef DEF_GET_ROT_DMASRCCFG0_OSRLMT2
def_get_mod_reg_bit(rot, dmasrccfg0, osrlmt2, addr);
#endif

#ifdef DEF_GET_ROT_DMASRCCFG0_CONLEN0
def_get_mod_reg_bit(rot, dmasrccfg0, conlen0, addr);
#endif

#ifdef DEF_GET_ROT_DMASRCCFG1_CONLEN1
def_get_mod_reg_bit(rot, dmasrccfg1, conlen1, addr);
#endif

#ifdef DEF_GET_ROT_DMASRCCFG1_CONLEN2
def_get_mod_reg_bit(rot, dmasrccfg1, conlen2, addr);
#endif

#ifdef DEF_GET_ROT_DMADSTCFG0_PRIO
def_get_mod_reg_bit(rot, dmadstcfg0, prio, addr);
#endif

#ifdef DEF_GET_ROT_DMADSTCFG0_RPIO2
def_get_mod_reg_bit(rot, dmadstcfg0, rpio2, addr);
#endif

#ifdef DEF_GET_ROT_DMADSTCFG0_OSRLMT
def_get_mod_reg_bit(rot, dmadstcfg0, osrlmt, addr);
#endif

#ifdef DEF_GET_ROT_DMADSTCFG0_OSRLMT2
def_get_mod_reg_bit(rot, dmadstcfg0, osrlmt2, addr);
#endif

#ifdef DEF_GET_ROT_DMADSTCFG0_CONLEN0
def_get_mod_reg_bit(rot, dmadstcfg0, conlen0, addr);
#endif

#ifdef DEF_GET_ROT_DMADSTCFG1_CONLEN1
def_get_mod_reg_bit(rot, dmadstcfg1, conlen1, addr);
#endif

#ifdef DEF_GET_ROT_DMADSTCFG1_CONLEN2
def_get_mod_reg_bit(rot, dmadstcfg1, conlen2, addr);
#endif

#ifdef DEF_GET_ROT_DBG0_THR
def_get_mod_reg_bit(rot, dbg0, thr, addr);
#endif

#ifdef DEF_GET_ROT_DBG0_TMR
def_get_mod_reg_bit(rot, dbg0, tmr, addr);
#endif

#endif /* ___ROT__GET___H___ */
