/* dc_get.h */

#ifndef ___DC__GET___H___
#define ___DC__GET___H___

#ifdef DEF_GET_DC_GNECTL_DCEN
def_get_mod_reg_bit(dc, gnectl, dcen, addr);
#endif

#ifdef DEF_GET_DC_GNECTL_DCRST
def_get_mod_reg_bit(dc, gnectl, dcrst, addr);
#endif

#ifdef DEF_GET_DC_GNECTL_FIFORST
def_get_mod_reg_bit(dc, gnectl, fiforst, addr);
#endif

#ifdef DEF_GET_DC_GNECTL_DBC
def_get_mod_reg_bit(dc, gnectl, dbc, addr);
#endif

#ifdef DEF_GET_DC_GNECTL_DBAUTOLD
def_get_mod_reg_bit(dc, gnectl, dbautold, addr);
#endif

#ifdef DEF_GET_DC_GNECTL_OUTMODE
def_get_mod_reg_bit(dc, gnectl, outmode, addr);
#endif

#ifdef DEF_GET_DC_GNECTL_FIELDPOL
def_get_mod_reg_bit(dc, gnectl, fieldpol, addr);
#endif

#ifdef DEF_GET_DC_IMGSRCCTL_WIN0EN
def_get_mod_reg_bit(dc, imgsrcctl, win0en, addr);
#endif

#ifdef DEF_GET_DC_IMGSRCCTL_WIN1EN
def_get_mod_reg_bit(dc, imgsrcctl, win1en, addr);
#endif

#ifdef DEF_GET_DC_IMGSRCCTL_WIN2EN
def_get_mod_reg_bit(dc, imgsrcctl, win2en, addr);
#endif

#ifdef DEF_GET_DC_IMGSRCCTL_WIN3EN
def_get_mod_reg_bit(dc, imgsrcctl, win3en, addr);
#endif

#ifdef DEF_GET_DC_IMGSRCCTL_PIPE0BYPASS
def_get_mod_reg_bit(dc, imgsrcctl, pipe0bypass, addr);
#endif

#ifdef DEF_GET_DC_IMGSRCCTL_PIPE1BYPASS
def_get_mod_reg_bit(dc, imgsrcctl, pipe1bypass, addr);
#endif

#ifdef DEF_GET_DC_IMGSRCCTL_HWCEN
def_get_mod_reg_bit(dc, imgsrcctl, hwcen, addr);
#endif

#ifdef DEF_GET_DC_IMGSRCCTL_BLKEN
def_get_mod_reg_bit(dc, imgsrcctl, blken, addr);
#endif

#ifdef DEF_GET_DC_SCRSIZ_W
def_get_mod_reg_bit(dc, scrsiz, w, addr);
#endif

#ifdef DEF_GET_DC_SCRSIZ_H
def_get_mod_reg_bit(dc, scrsiz, h, addr);
#endif

#ifdef DEF_GET_DC_BKCOLOR_B
def_get_mod_reg_bit(dc, bkcolor, b, addr);
#endif

#ifdef DEF_GET_DC_BKCOLOR_G
def_get_mod_reg_bit(dc, bkcolor, g, addr);
#endif

#ifdef DEF_GET_DC_BKCOLOR_R
def_get_mod_reg_bit(dc, bkcolor, r, addr);
#endif

#ifdef DEF_GET_DC_BKCOLOR_A
def_get_mod_reg_bit(dc, bkcolor, a, addr);
#endif

#ifdef DEF_GET_DC_VER_VER_L
def_get_mod_reg_bit(dc, ver, ver_l, addr);
#endif

#ifdef DEF_GET_DC_VER_VER_H
def_get_mod_reg_bit(dc, ver, ver_h, addr);
#endif

#ifdef DEF_GET_DC_VER_COMP
def_get_mod_reg_bit(dc, ver, comp, addr);
#endif

#ifdef DEF_GET_DC_FTR_HAS_WB
def_get_mod_reg_bit(dc, ftr, has_wb, addr);
#endif

#ifdef DEF_GET_DC_FTR_HAS_YUV
def_get_mod_reg_bit(dc, ftr, has_yuv, addr);
#endif

#ifdef DEF_GET_DC_FTR_HAS_HWC
def_get_mod_reg_bit(dc, ftr, has_hwc, addr);
#endif

#ifdef DEF_GET_DC_FTR_HAS_BKL
def_get_mod_reg_bit(dc, ftr, has_bkl, addr);
#endif

#ifdef DEF_GET_DC_FTR_HAS_IFB
def_get_mod_reg_bit(dc, ftr, has_ifb, addr);
#endif

#ifdef DEF_GET_DC_FTR_HAS_PAL
def_get_mod_reg_bit(dc, ftr, has_pal, addr);
#endif

#ifdef DEF_GET_DC_FTR_HAS_GAMMA
def_get_mod_reg_bit(dc, ftr, has_gamma, addr);
#endif

#ifdef DEF_GET_DC_FTR_HAS_BLEND
def_get_mod_reg_bit(dc, ftr, has_blend, addr);
#endif

#ifdef DEF_GET_DC_FTR_HAS_COLORKEY
def_get_mod_reg_bit(dc, ftr, has_colorkey, addr);
#endif

#ifdef DEF_GET_DC_FTR_HAS_CSC
def_get_mod_reg_bit(dc, ftr, has_csc, addr);
#endif

#ifdef DEF_GET_DC_FTR_HAS_OVE
def_get_mod_reg_bit(dc, ftr, has_ove, addr);
#endif

#ifdef DEF_GET_DC_FTR_OVENUM
def_get_mod_reg_bit(dc, ftr, ovenum, addr);
#endif

#ifdef DEF_GET_DC_FTR_WINNUM
def_get_mod_reg_bit(dc, ftr, winnum, addr);
#endif

#ifdef DEF_GET_DC_WIN0CTL_WINWKMOD
def_get_mod_reg_bit(dc, win0ctl, winwkmod, addr);
#endif

#ifdef DEF_GET_DC_WIN0CTL_WINSRC
def_get_mod_reg_bit(dc, win0ctl, winsrc, addr);
#endif

#ifdef DEF_GET_DC_WIN0CTL_ALPHACTL
def_get_mod_reg_bit(dc, win0ctl, alphactl, addr);
#endif

#ifdef DEF_GET_DC_WIN0CTL_PLAALPHA
def_get_mod_reg_bit(dc, win0ctl, plaalpha, addr);
#endif

#ifdef DEF_GET_DC_WIN1CTL_WINWKMOD
def_get_mod_reg_bit(dc, win1ctl, winwkmod, addr);
#endif

#ifdef DEF_GET_DC_WIN1CTL_WINSRC
def_get_mod_reg_bit(dc, win1ctl, winsrc, addr);
#endif

#ifdef DEF_GET_DC_WIN1CTL_ALPHACTL
def_get_mod_reg_bit(dc, win1ctl, alphactl, addr);
#endif

#ifdef DEF_GET_DC_WIN1CTL_PLAALPHA
def_get_mod_reg_bit(dc, win1ctl, plaalpha, addr);
#endif

#ifdef DEF_GET_DC_WIN2CTL_WINWKMOD
def_get_mod_reg_bit(dc, win2ctl, winwkmod, addr);
#endif

#ifdef DEF_GET_DC_WIN2CTL_WINSRC
def_get_mod_reg_bit(dc, win2ctl, winsrc, addr);
#endif

#ifdef DEF_GET_DC_WIN2CTL_ALPHACTL
def_get_mod_reg_bit(dc, win2ctl, alphactl, addr);
#endif

#ifdef DEF_GET_DC_WIN2CTL_PLAALPHA
def_get_mod_reg_bit(dc, win2ctl, plaalpha, addr);
#endif

#ifdef DEF_GET_DC_WIN3CTL_WINWKMOD
def_get_mod_reg_bit(dc, win3ctl, winwkmod, addr);
#endif

#ifdef DEF_GET_DC_WIN3CTL_WINSRC
def_get_mod_reg_bit(dc, win3ctl, winsrc, addr);
#endif

#ifdef DEF_GET_DC_WIN3CTL_ALPHACTL
def_get_mod_reg_bit(dc, win3ctl, alphactl, addr);
#endif

#ifdef DEF_GET_DC_WIN3CTL_PLAALPHA
def_get_mod_reg_bit(dc, win3ctl, plaalpha, addr);
#endif

#ifdef DEF_GET_DC_WIN0BUFFMT_PO
def_get_mod_reg_bit(dc, win0buffmt, po, addr);
#endif

#ifdef DEF_GET_DC_WIN0BUFFMT_FMT
def_get_mod_reg_bit(dc, win0buffmt, fmt, addr);
#endif

#ifdef DEF_GET_DC_WIN1BUFFMT_PO
def_get_mod_reg_bit(dc, win1buffmt, po, addr);
#endif

#ifdef DEF_GET_DC_WIN1BUFFMT_FMT
def_get_mod_reg_bit(dc, win1buffmt, fmt, addr);
#endif

#ifdef DEF_GET_DC_WIN2BUFFMT_PO
def_get_mod_reg_bit(dc, win2buffmt, po, addr);
#endif

#ifdef DEF_GET_DC_WIN2BUFFMT_FMT
def_get_mod_reg_bit(dc, win2buffmt, fmt, addr);
#endif

#ifdef DEF_GET_DC_WIN3BUFFMT_PO
def_get_mod_reg_bit(dc, win3buffmt, po, addr);
#endif

#ifdef DEF_GET_DC_WIN3BUFFMT_FMT
def_get_mod_reg_bit(dc, win3buffmt, fmt, addr);
#endif

#ifdef DEF_GET_DC_WIN0SIZ_W
def_get_mod_reg_bit(dc, win0siz, w, addr);
#endif

#ifdef DEF_GET_DC_WIN0SIZ_H
def_get_mod_reg_bit(dc, win0siz, h, addr);
#endif

#ifdef DEF_GET_DC_WIN1SIZ_W
def_get_mod_reg_bit(dc, win1siz, w, addr);
#endif

#ifdef DEF_GET_DC_WIN1SIZ_H
def_get_mod_reg_bit(dc, win1siz, h, addr);
#endif

#ifdef DEF_GET_DC_WIN2SIZ_W
def_get_mod_reg_bit(dc, win2siz, w, addr);
#endif

#ifdef DEF_GET_DC_WIN2SIZ_H
def_get_mod_reg_bit(dc, win2siz, h, addr);
#endif

#ifdef DEF_GET_DC_WIN3SIZ_W
def_get_mod_reg_bit(dc, win3siz, w, addr);
#endif

#ifdef DEF_GET_DC_WIN3SIZ_H
def_get_mod_reg_bit(dc, win3siz, h, addr);
#endif

#ifdef DEF_GET_DC_WIN0COOR_X
def_get_mod_reg_bit(dc, win0coor, x, addr);
#endif

#ifdef DEF_GET_DC_WIN0COOR_Y
def_get_mod_reg_bit(dc, win0coor, y, addr);
#endif

#ifdef DEF_GET_DC_WIN1COOR_X
def_get_mod_reg_bit(dc, win1coor, x, addr);
#endif

#ifdef DEF_GET_DC_WIN1COOR_Y
def_get_mod_reg_bit(dc, win1coor, y, addr);
#endif

#ifdef DEF_GET_DC_WIN2COOR_X
def_get_mod_reg_bit(dc, win2coor, x, addr);
#endif

#ifdef DEF_GET_DC_WIN2COOR_Y
def_get_mod_reg_bit(dc, win2coor, y, addr);
#endif

#ifdef DEF_GET_DC_WIN3COOR_X
def_get_mod_reg_bit(dc, win3coor, x, addr);
#endif

#ifdef DEF_GET_DC_WIN3COOR_Y
def_get_mod_reg_bit(dc, win3coor, y, addr);
#endif

#ifdef DEF_GET_DC_WIN0BUFLADD_ADD
def_get_mod_reg_bit(dc, win0bufladd, add, addr);
#endif

#ifdef DEF_GET_DC_WIN1BUFLADD_ADD
def_get_mod_reg_bit(dc, win1bufladd, add, addr);
#endif

#ifdef DEF_GET_DC_WIN2BUFLADD_ADD
def_get_mod_reg_bit(dc, win2bufladd, add, addr);
#endif

#ifdef DEF_GET_DC_WIN3BUFLADD_ADD
def_get_mod_reg_bit(dc, win3bufladd, add, addr);
#endif

#ifdef DEF_GET_DC_WINBUFHADD_WIN0ADD
def_get_mod_reg_bit(dc, winbufhadd, win0add, addr);
#endif

#ifdef DEF_GET_DC_WINBUFHADD_WIN1ADD
def_get_mod_reg_bit(dc, winbufhadd, win1add, addr);
#endif

#ifdef DEF_GET_DC_WINBUFHADD_WIN2ADD
def_get_mod_reg_bit(dc, winbufhadd, win2add, addr);
#endif

#ifdef DEF_GET_DC_WINBUFHADD_WIN3ADD
def_get_mod_reg_bit(dc, winbufhadd, win3add, addr);
#endif

#ifdef DEF_GET_DC_WIN0BUFLNSTD_STD
def_get_mod_reg_bit(dc, win0buflnstd, std, addr);
#endif

#ifdef DEF_GET_DC_WIN1BUFLNSTD_STD
def_get_mod_reg_bit(dc, win1buflnstd, std, addr);
#endif

#ifdef DEF_GET_DC_WIN2BUFLNSTD_STD
def_get_mod_reg_bit(dc, win2buflnstd, std, addr);
#endif

#ifdef DEF_GET_DC_WIN3BUFLNSTD_STD
def_get_mod_reg_bit(dc, win3buflnstd, std, addr);
#endif

#ifdef DEF_GET_DC_BLD0CTL_CKEN
def_get_mod_reg_bit(dc, bld0ctl, cken, addr);
#endif

#ifdef DEF_GET_DC_BLD0CTL_CKMATCH
def_get_mod_reg_bit(dc, bld0ctl, ckmatch, addr);
#endif

#ifdef DEF_GET_DC_BLD0CTL_CKBCON
def_get_mod_reg_bit(dc, bld0ctl, ckbcon, addr);
#endif

#ifdef DEF_GET_DC_BLD0CTL_CKGCON
def_get_mod_reg_bit(dc, bld0ctl, ckgcon, addr);
#endif

#ifdef DEF_GET_DC_BLD0CTL_CKRCON
def_get_mod_reg_bit(dc, bld0ctl, ckrcon, addr);
#endif

#ifdef DEF_GET_DC_BLD0CTL_C0
def_get_mod_reg_bit(dc, bld0ctl, c0, addr);
#endif

#ifdef DEF_GET_DC_BLD0CTL_C1
def_get_mod_reg_bit(dc, bld0ctl, c1, addr);
#endif

#ifdef DEF_GET_DC_BLD0CTL_C2
def_get_mod_reg_bit(dc, bld0ctl, c2, addr);
#endif

#ifdef DEF_GET_DC_BLD0CTL_C3
def_get_mod_reg_bit(dc, bld0ctl, c3, addr);
#endif

#ifdef DEF_GET_DC_BLD1CTL_CKEN
def_get_mod_reg_bit(dc, bld1ctl, cken, addr);
#endif

#ifdef DEF_GET_DC_BLD1CTL_CKMATCH
def_get_mod_reg_bit(dc, bld1ctl, ckmatch, addr);
#endif

#ifdef DEF_GET_DC_BLD1CTL_CKBCON
def_get_mod_reg_bit(dc, bld1ctl, ckbcon, addr);
#endif

#ifdef DEF_GET_DC_BLD1CTL_CKGCON
def_get_mod_reg_bit(dc, bld1ctl, ckgcon, addr);
#endif

#ifdef DEF_GET_DC_BLD1CTL_CKRCON
def_get_mod_reg_bit(dc, bld1ctl, ckrcon, addr);
#endif

#ifdef DEF_GET_DC_BLD1CTL_C0
def_get_mod_reg_bit(dc, bld1ctl, c0, addr);
#endif

#ifdef DEF_GET_DC_BLD1CTL_C1
def_get_mod_reg_bit(dc, bld1ctl, c1, addr);
#endif

#ifdef DEF_GET_DC_BLD1CTL_C2
def_get_mod_reg_bit(dc, bld1ctl, c2, addr);
#endif

#ifdef DEF_GET_DC_BLD1CTL_C3
def_get_mod_reg_bit(dc, bld1ctl, c3, addr);
#endif

#ifdef DEF_GET_DC_BLD2CTL_CKEN
def_get_mod_reg_bit(dc, bld2ctl, cken, addr);
#endif

#ifdef DEF_GET_DC_BLD2CTL_CKMATCH
def_get_mod_reg_bit(dc, bld2ctl, ckmatch, addr);
#endif

#ifdef DEF_GET_DC_BLD2CTL_CKBCON
def_get_mod_reg_bit(dc, bld2ctl, ckbcon, addr);
#endif

#ifdef DEF_GET_DC_BLD2CTL_CKGCON
def_get_mod_reg_bit(dc, bld2ctl, ckgcon, addr);
#endif

#ifdef DEF_GET_DC_BLD2CTL_CKRCON
def_get_mod_reg_bit(dc, bld2ctl, ckrcon, addr);
#endif

#ifdef DEF_GET_DC_BLD2CTL_C0
def_get_mod_reg_bit(dc, bld2ctl, c0, addr);
#endif

#ifdef DEF_GET_DC_BLD2CTL_C1
def_get_mod_reg_bit(dc, bld2ctl, c1, addr);
#endif

#ifdef DEF_GET_DC_BLD2CTL_C2
def_get_mod_reg_bit(dc, bld2ctl, c2, addr);
#endif

#ifdef DEF_GET_DC_BLD2CTL_C3
def_get_mod_reg_bit(dc, bld2ctl, c3, addr);
#endif

#ifdef DEF_GET_DC_BLD3CTL_CKEN
def_get_mod_reg_bit(dc, bld3ctl, cken, addr);
#endif

#ifdef DEF_GET_DC_BLD3CTL_CKMATCH
def_get_mod_reg_bit(dc, bld3ctl, ckmatch, addr);
#endif

#ifdef DEF_GET_DC_BLD3CTL_CKBCON
def_get_mod_reg_bit(dc, bld3ctl, ckbcon, addr);
#endif

#ifdef DEF_GET_DC_BLD3CTL_CKGCON
def_get_mod_reg_bit(dc, bld3ctl, ckgcon, addr);
#endif

#ifdef DEF_GET_DC_BLD3CTL_CKRCON
def_get_mod_reg_bit(dc, bld3ctl, ckrcon, addr);
#endif

#ifdef DEF_GET_DC_BLD3CTL_C0
def_get_mod_reg_bit(dc, bld3ctl, c0, addr);
#endif

#ifdef DEF_GET_DC_BLD3CTL_C1
def_get_mod_reg_bit(dc, bld3ctl, c1, addr);
#endif

#ifdef DEF_GET_DC_BLD3CTL_C2
def_get_mod_reg_bit(dc, bld3ctl, c2, addr);
#endif

#ifdef DEF_GET_DC_BLD3CTL_C3
def_get_mod_reg_bit(dc, bld3ctl, c3, addr);
#endif

#ifdef DEF_GET_DC_BLD4CTL_CKEN
def_get_mod_reg_bit(dc, bld4ctl, cken, addr);
#endif

#ifdef DEF_GET_DC_BLD4CTL_CKMATCH
def_get_mod_reg_bit(dc, bld4ctl, ckmatch, addr);
#endif

#ifdef DEF_GET_DC_BLD4CTL_CKBCON
def_get_mod_reg_bit(dc, bld4ctl, ckbcon, addr);
#endif

#ifdef DEF_GET_DC_BLD4CTL_CKGCON
def_get_mod_reg_bit(dc, bld4ctl, ckgcon, addr);
#endif

#ifdef DEF_GET_DC_BLD4CTL_CKRCON
def_get_mod_reg_bit(dc, bld4ctl, ckrcon, addr);
#endif

#ifdef DEF_GET_DC_BLD4CTL_C0
def_get_mod_reg_bit(dc, bld4ctl, c0, addr);
#endif

#ifdef DEF_GET_DC_BLD4CTL_C1
def_get_mod_reg_bit(dc, bld4ctl, c1, addr);
#endif

#ifdef DEF_GET_DC_BLD4CTL_C2
def_get_mod_reg_bit(dc, bld4ctl, c2, addr);
#endif

#ifdef DEF_GET_DC_BLD4CTL_C3
def_get_mod_reg_bit(dc, bld4ctl, c3, addr);
#endif

#ifdef DEF_GET_DC_BLD0CKMIN_CKBMIN
def_get_mod_reg_bit(dc, bld0ckmin, ckbmin, addr);
#endif

#ifdef DEF_GET_DC_BLD0CKMIN_CKGMIN
def_get_mod_reg_bit(dc, bld0ckmin, ckgmin, addr);
#endif

#ifdef DEF_GET_DC_BLD0CKMIN_CKRMIN
def_get_mod_reg_bit(dc, bld0ckmin, ckrmin, addr);
#endif

#ifdef DEF_GET_DC_BLD1CKMIN_CKBMIN
def_get_mod_reg_bit(dc, bld1ckmin, ckbmin, addr);
#endif

#ifdef DEF_GET_DC_BLD1CKMIN_CKGMIN
def_get_mod_reg_bit(dc, bld1ckmin, ckgmin, addr);
#endif

#ifdef DEF_GET_DC_BLD1CKMIN_CKRMIN
def_get_mod_reg_bit(dc, bld1ckmin, ckrmin, addr);
#endif

#ifdef DEF_GET_DC_BLD2CKMIN_CKBMIN
def_get_mod_reg_bit(dc, bld2ckmin, ckbmin, addr);
#endif

#ifdef DEF_GET_DC_BLD2CKMIN_CKGMIN
def_get_mod_reg_bit(dc, bld2ckmin, ckgmin, addr);
#endif

#ifdef DEF_GET_DC_BLD2CKMIN_CKRMIN
def_get_mod_reg_bit(dc, bld2ckmin, ckrmin, addr);
#endif

#ifdef DEF_GET_DC_BLD3CKMIN_CKBMIN
def_get_mod_reg_bit(dc, bld3ckmin, ckbmin, addr);
#endif

#ifdef DEF_GET_DC_BLD3CKMIN_CKGMIN
def_get_mod_reg_bit(dc, bld3ckmin, ckgmin, addr);
#endif

#ifdef DEF_GET_DC_BLD3CKMIN_CKRMIN
def_get_mod_reg_bit(dc, bld3ckmin, ckrmin, addr);
#endif

#ifdef DEF_GET_DC_BLD4CKMIN_CKBMIN
def_get_mod_reg_bit(dc, bld4ckmin, ckbmin, addr);
#endif

#ifdef DEF_GET_DC_BLD4CKMIN_CKGMIN
def_get_mod_reg_bit(dc, bld4ckmin, ckgmin, addr);
#endif

#ifdef DEF_GET_DC_BLD4CKMIN_CKRMIN
def_get_mod_reg_bit(dc, bld4ckmin, ckrmin, addr);
#endif

#ifdef DEF_GET_DC_BLD0CKMAX_CKBMAX
def_get_mod_reg_bit(dc, bld0ckmax, ckbmax, addr);
#endif

#ifdef DEF_GET_DC_BLD0CKMAX_CKGMAX
def_get_mod_reg_bit(dc, bld0ckmax, ckgmax, addr);
#endif

#ifdef DEF_GET_DC_BLD0CKMAX_CKRMAX
def_get_mod_reg_bit(dc, bld0ckmax, ckrmax, addr);
#endif

#ifdef DEF_GET_DC_BLD1CKMAX_CKBMAX
def_get_mod_reg_bit(dc, bld1ckmax, ckbmax, addr);
#endif

#ifdef DEF_GET_DC_BLD1CKMAX_CKGMAX
def_get_mod_reg_bit(dc, bld1ckmax, ckgmax, addr);
#endif

#ifdef DEF_GET_DC_BLD1CKMAX_CKRMAX
def_get_mod_reg_bit(dc, bld1ckmax, ckrmax, addr);
#endif

#ifdef DEF_GET_DC_BLD2CKMAX_CKBMAX
def_get_mod_reg_bit(dc, bld2ckmax, ckbmax, addr);
#endif

#ifdef DEF_GET_DC_BLD2CKMAX_CKGMAX
def_get_mod_reg_bit(dc, bld2ckmax, ckgmax, addr);
#endif

#ifdef DEF_GET_DC_BLD2CKMAX_CKRMAX
def_get_mod_reg_bit(dc, bld2ckmax, ckrmax, addr);
#endif

#ifdef DEF_GET_DC_BLD3CKMAX_CKBMAX
def_get_mod_reg_bit(dc, bld3ckmax, ckbmax, addr);
#endif

#ifdef DEF_GET_DC_BLD3CKMAX_CKGMAX
def_get_mod_reg_bit(dc, bld3ckmax, ckgmax, addr);
#endif

#ifdef DEF_GET_DC_BLD3CKMAX_CKRMAX
def_get_mod_reg_bit(dc, bld3ckmax, ckrmax, addr);
#endif

#ifdef DEF_GET_DC_BLD4CKMAX_CKBMAX
def_get_mod_reg_bit(dc, bld4ckmax, ckbmax, addr);
#endif

#ifdef DEF_GET_DC_BLD4CKMAX_CKGMAX
def_get_mod_reg_bit(dc, bld4ckmax, ckgmax, addr);
#endif

#ifdef DEF_GET_DC_BLD4CKMAX_CKRMAX
def_get_mod_reg_bit(dc, bld4ckmax, ckrmax, addr);
#endif

#ifdef DEF_GET_DC_HWCCTL_W
def_get_mod_reg_bit(dc, hwcctl, w, addr);
#endif

#ifdef DEF_GET_DC_HWCCTL_H
def_get_mod_reg_bit(dc, hwcctl, h, addr);
#endif

#ifdef DEF_GET_DC_HWCCTL_FMT
def_get_mod_reg_bit(dc, hwcctl, fmt, addr);
#endif

#ifdef DEF_GET_DC_HWCCTL_XOFF
def_get_mod_reg_bit(dc, hwcctl, xoff, addr);
#endif

#ifdef DEF_GET_DC_HWCCTL_YOFF
def_get_mod_reg_bit(dc, hwcctl, yoff, addr);
#endif

#ifdef DEF_GET_DC_HWCCOOR_X
def_get_mod_reg_bit(dc, hwccoor, x, addr);
#endif

#ifdef DEF_GET_DC_HWCCOOR_Y
def_get_mod_reg_bit(dc, hwccoor, y, addr);
#endif

#ifdef DEF_GET_DC_CEEN_EN
def_get_mod_reg_bit(dc, ceen, en, addr);
#endif

#ifdef DEF_GET_DC_CERRCOEF_COEF
def_get_mod_reg_bit(dc, cerrcoef, coef, addr);
#endif

#ifdef DEF_GET_DC_CERGCOEF_COEF
def_get_mod_reg_bit(dc, cergcoef, coef, addr);
#endif

#ifdef DEF_GET_DC_CERBCOEF_COEF
def_get_mod_reg_bit(dc, cerbcoef, coef, addr);
#endif

#ifdef DEF_GET_DC_CERCONS_CONS
def_get_mod_reg_bit(dc, cercons, cons, addr);
#endif

#ifdef DEF_GET_DC_CEGRCOEF_COEF
def_get_mod_reg_bit(dc, cegrcoef, coef, addr);
#endif

#ifdef DEF_GET_DC_CEGGCOEF_COEF
def_get_mod_reg_bit(dc, ceggcoef, coef, addr);
#endif

#ifdef DEF_GET_DC_CEGBCOEF_COEF
def_get_mod_reg_bit(dc, cegbcoef, coef, addr);
#endif

#ifdef DEF_GET_DC_CEGCONS_CONS
def_get_mod_reg_bit(dc, cegcons, cons, addr);
#endif

#ifdef DEF_GET_DC_CEBRCOEF_COEF
def_get_mod_reg_bit(dc, cebrcoef, coef, addr);
#endif

#ifdef DEF_GET_DC_CEBGCOEF_COEF
def_get_mod_reg_bit(dc, cebgcoef, coef, addr);
#endif

#ifdef DEF_GET_DC_CEBBCOEF_COEF
def_get_mod_reg_bit(dc, cebbcoef, coef, addr);
#endif

#ifdef DEF_GET_DC_CEBCONS_CONS
def_get_mod_reg_bit(dc, cebcons, cons, addr);
#endif

#ifdef DEF_GET_DC_BLKGNE_FMT
def_get_mod_reg_bit(dc, blkgne, fmt, addr);
#endif

#ifdef DEF_GET_DC_BLKGNE_PO
def_get_mod_reg_bit(dc, blkgne, po, addr);
#endif

#ifdef DEF_GET_DC_BLKGNE_ALPHACTL
def_get_mod_reg_bit(dc, blkgne, alphactl, addr);
#endif

#ifdef DEF_GET_DC_BLKGNE_PLAALPHA
def_get_mod_reg_bit(dc, blkgne, plaalpha, addr);
#endif

#ifdef DEF_GET_DC_WBC_STA
def_get_mod_reg_bit(dc, wbc, sta, addr);
#endif

#ifdef DEF_GET_DC_WBC_WBMOD
def_get_mod_reg_bit(dc, wbc, wbmod, addr);
#endif

#ifdef DEF_GET_DC_WBC_FMT
def_get_mod_reg_bit(dc, wbc, fmt, addr);
#endif

#ifdef DEF_GET_DC_WBC_WBSTATUS
def_get_mod_reg_bit(dc, wbc, wbstatus, addr);
#endif

#ifdef DEF_GET_DC_WBC_WBEXC
def_get_mod_reg_bit(dc, wbc, wbexc, addr);
#endif

#ifdef DEF_GET_DC_WBADD_ADD
def_get_mod_reg_bit(dc, wbadd, add, addr);
#endif

#ifdef DEF_GET_DC_WBLNSTD_STD
def_get_mod_reg_bit(dc, wblnstd, std, addr);
#endif

#ifdef DEF_GET_DC_CH0_BLKCTL_W
def_get_mod_reg_bit(dc, ch0_blkctl, w, addr);
#endif

#ifdef DEF_GET_DC_CH0_BLKCTL_H
def_get_mod_reg_bit(dc, ch0_blkctl, h, addr);
#endif

#ifdef DEF_GET_DC_CH0_BLKCTL_NP
def_get_mod_reg_bit(dc, ch0_blkctl, np, addr);
#endif

#ifdef DEF_GET_DC_CH1_BLKCTL_W
def_get_mod_reg_bit(dc, ch1_blkctl, w, addr);
#endif

#ifdef DEF_GET_DC_CH1_BLKCTL_H
def_get_mod_reg_bit(dc, ch1_blkctl, h, addr);
#endif

#ifdef DEF_GET_DC_CH1_BLKCTL_NP
def_get_mod_reg_bit(dc, ch1_blkctl, np, addr);
#endif

#ifdef DEF_GET_DC_CH2_BLKCTL_W
def_get_mod_reg_bit(dc, ch2_blkctl, w, addr);
#endif

#ifdef DEF_GET_DC_CH2_BLKCTL_H
def_get_mod_reg_bit(dc, ch2_blkctl, h, addr);
#endif

#ifdef DEF_GET_DC_CH2_BLKCTL_NP
def_get_mod_reg_bit(dc, ch2_blkctl, np, addr);
#endif

#ifdef DEF_GET_DC_CH3_BLKCTL_W
def_get_mod_reg_bit(dc, ch3_blkctl, w, addr);
#endif

#ifdef DEF_GET_DC_CH3_BLKCTL_H
def_get_mod_reg_bit(dc, ch3_blkctl, h, addr);
#endif

#ifdef DEF_GET_DC_CH3_BLKCTL_NP
def_get_mod_reg_bit(dc, ch3_blkctl, np, addr);
#endif

#ifdef DEF_GET_DC_CH4_BLKCTL_W
def_get_mod_reg_bit(dc, ch4_blkctl, w, addr);
#endif

#ifdef DEF_GET_DC_CH4_BLKCTL_H
def_get_mod_reg_bit(dc, ch4_blkctl, h, addr);
#endif

#ifdef DEF_GET_DC_CH4_BLKCTL_NP
def_get_mod_reg_bit(dc, ch4_blkctl, np, addr);
#endif

#ifdef DEF_GET_DC_CH5_BLKCTL_W
def_get_mod_reg_bit(dc, ch5_blkctl, w, addr);
#endif

#ifdef DEF_GET_DC_CH5_BLKCTL_H
def_get_mod_reg_bit(dc, ch5_blkctl, h, addr);
#endif

#ifdef DEF_GET_DC_CH5_BLKCTL_NP
def_get_mod_reg_bit(dc, ch5_blkctl, np, addr);
#endif

#ifdef DEF_GET_DC_CH6_BLKCTL_W
def_get_mod_reg_bit(dc, ch6_blkctl, w, addr);
#endif

#ifdef DEF_GET_DC_CH6_BLKCTL_H
def_get_mod_reg_bit(dc, ch6_blkctl, h, addr);
#endif

#ifdef DEF_GET_DC_CH6_BLKCTL_NP
def_get_mod_reg_bit(dc, ch6_blkctl, np, addr);
#endif

#ifdef DEF_GET_DC_CH7_BLKCTL_W
def_get_mod_reg_bit(dc, ch7_blkctl, w, addr);
#endif

#ifdef DEF_GET_DC_CH7_BLKCTL_H
def_get_mod_reg_bit(dc, ch7_blkctl, h, addr);
#endif

#ifdef DEF_GET_DC_CH7_BLKCTL_NP
def_get_mod_reg_bit(dc, ch7_blkctl, np, addr);
#endif

#ifdef DEF_GET_DC_CH8_BLKCTL_W
def_get_mod_reg_bit(dc, ch8_blkctl, w, addr);
#endif

#ifdef DEF_GET_DC_CH8_BLKCTL_H
def_get_mod_reg_bit(dc, ch8_blkctl, h, addr);
#endif

#ifdef DEF_GET_DC_CH8_BLKCTL_NP
def_get_mod_reg_bit(dc, ch8_blkctl, np, addr);
#endif

#ifdef DEF_GET_DC_CH9_BLKCTL_W
def_get_mod_reg_bit(dc, ch9_blkctl, w, addr);
#endif

#ifdef DEF_GET_DC_CH9_BLKCTL_H
def_get_mod_reg_bit(dc, ch9_blkctl, h, addr);
#endif

#ifdef DEF_GET_DC_CH9_BLKCTL_NP
def_get_mod_reg_bit(dc, ch9_blkctl, np, addr);
#endif

#ifdef DEF_GET_DC_CH10_BLKCTL_W
def_get_mod_reg_bit(dc, ch10_blkctl, w, addr);
#endif

#ifdef DEF_GET_DC_CH10_BLKCTL_H
def_get_mod_reg_bit(dc, ch10_blkctl, h, addr);
#endif

#ifdef DEF_GET_DC_CH10_BLKCTL_NP
def_get_mod_reg_bit(dc, ch10_blkctl, np, addr);
#endif

#ifdef DEF_GET_DC_CH11_BLKCTL_W
def_get_mod_reg_bit(dc, ch11_blkctl, w, addr);
#endif

#ifdef DEF_GET_DC_CH11_BLKCTL_H
def_get_mod_reg_bit(dc, ch11_blkctl, h, addr);
#endif

#ifdef DEF_GET_DC_CH11_BLKCTL_NP
def_get_mod_reg_bit(dc, ch11_blkctl, np, addr);
#endif

#ifdef DEF_GET_DC_CH12_BLKCTL_W
def_get_mod_reg_bit(dc, ch12_blkctl, w, addr);
#endif

#ifdef DEF_GET_DC_CH12_BLKCTL_H
def_get_mod_reg_bit(dc, ch12_blkctl, h, addr);
#endif

#ifdef DEF_GET_DC_CH12_BLKCTL_NP
def_get_mod_reg_bit(dc, ch12_blkctl, np, addr);
#endif

#ifdef DEF_GET_DC_CH13_BLKCTL_W
def_get_mod_reg_bit(dc, ch13_blkctl, w, addr);
#endif

#ifdef DEF_GET_DC_CH13_BLKCTL_H
def_get_mod_reg_bit(dc, ch13_blkctl, h, addr);
#endif

#ifdef DEF_GET_DC_CH13_BLKCTL_NP
def_get_mod_reg_bit(dc, ch13_blkctl, np, addr);
#endif

#ifdef DEF_GET_DC_CH14_BLKCTL_W
def_get_mod_reg_bit(dc, ch14_blkctl, w, addr);
#endif

#ifdef DEF_GET_DC_CH14_BLKCTL_H
def_get_mod_reg_bit(dc, ch14_blkctl, h, addr);
#endif

#ifdef DEF_GET_DC_CH14_BLKCTL_NP
def_get_mod_reg_bit(dc, ch14_blkctl, np, addr);
#endif

#ifdef DEF_GET_DC_CH15_BLKCTL_W
def_get_mod_reg_bit(dc, ch15_blkctl, w, addr);
#endif

#ifdef DEF_GET_DC_CH15_BLKCTL_H
def_get_mod_reg_bit(dc, ch15_blkctl, h, addr);
#endif

#ifdef DEF_GET_DC_CH15_BLKCTL_NP
def_get_mod_reg_bit(dc, ch15_blkctl, np, addr);
#endif

#ifdef DEF_GET_DC_CH0_BLKCOOR_X
def_get_mod_reg_bit(dc, ch0_blkcoor, x, addr);
#endif

#ifdef DEF_GET_DC_CH0_BLKCOOR_Y
def_get_mod_reg_bit(dc, ch0_blkcoor, y, addr);
#endif

#ifdef DEF_GET_DC_CH1_BLKCOOR_X
def_get_mod_reg_bit(dc, ch1_blkcoor, x, addr);
#endif

#ifdef DEF_GET_DC_CH1_BLKCOOR_Y
def_get_mod_reg_bit(dc, ch1_blkcoor, y, addr);
#endif

#ifdef DEF_GET_DC_CH2_BLKCOOR_X
def_get_mod_reg_bit(dc, ch2_blkcoor, x, addr);
#endif

#ifdef DEF_GET_DC_CH2_BLKCOOR_Y
def_get_mod_reg_bit(dc, ch2_blkcoor, y, addr);
#endif

#ifdef DEF_GET_DC_CH3_BLKCOOR_X
def_get_mod_reg_bit(dc, ch3_blkcoor, x, addr);
#endif

#ifdef DEF_GET_DC_CH3_BLKCOOR_Y
def_get_mod_reg_bit(dc, ch3_blkcoor, y, addr);
#endif

#ifdef DEF_GET_DC_CH4_BLKCOOR_X
def_get_mod_reg_bit(dc, ch4_blkcoor, x, addr);
#endif

#ifdef DEF_GET_DC_CH4_BLKCOOR_Y
def_get_mod_reg_bit(dc, ch4_blkcoor, y, addr);
#endif

#ifdef DEF_GET_DC_CH5_BLKCOOR_X
def_get_mod_reg_bit(dc, ch5_blkcoor, x, addr);
#endif

#ifdef DEF_GET_DC_CH5_BLKCOOR_Y
def_get_mod_reg_bit(dc, ch5_blkcoor, y, addr);
#endif

#ifdef DEF_GET_DC_CH6_BLKCOOR_X
def_get_mod_reg_bit(dc, ch6_blkcoor, x, addr);
#endif

#ifdef DEF_GET_DC_CH6_BLKCOOR_Y
def_get_mod_reg_bit(dc, ch6_blkcoor, y, addr);
#endif

#ifdef DEF_GET_DC_CH7_BLKCOOR_X
def_get_mod_reg_bit(dc, ch7_blkcoor, x, addr);
#endif

#ifdef DEF_GET_DC_CH7_BLKCOOR_Y
def_get_mod_reg_bit(dc, ch7_blkcoor, y, addr);
#endif

#ifdef DEF_GET_DC_CH8_BLKCOOR_X
def_get_mod_reg_bit(dc, ch8_blkcoor, x, addr);
#endif

#ifdef DEF_GET_DC_CH8_BLKCOOR_Y
def_get_mod_reg_bit(dc, ch8_blkcoor, y, addr);
#endif

#ifdef DEF_GET_DC_CH9_BLKCOOR_X
def_get_mod_reg_bit(dc, ch9_blkcoor, x, addr);
#endif

#ifdef DEF_GET_DC_CH9_BLKCOOR_Y
def_get_mod_reg_bit(dc, ch9_blkcoor, y, addr);
#endif

#ifdef DEF_GET_DC_CH10_BLKCOOR_X
def_get_mod_reg_bit(dc, ch10_blkcoor, x, addr);
#endif

#ifdef DEF_GET_DC_CH10_BLKCOOR_Y
def_get_mod_reg_bit(dc, ch10_blkcoor, y, addr);
#endif

#ifdef DEF_GET_DC_CH11_BLKCOOR_X
def_get_mod_reg_bit(dc, ch11_blkcoor, x, addr);
#endif

#ifdef DEF_GET_DC_CH11_BLKCOOR_Y
def_get_mod_reg_bit(dc, ch11_blkcoor, y, addr);
#endif

#ifdef DEF_GET_DC_CH12_BLKCOOR_X
def_get_mod_reg_bit(dc, ch12_blkcoor, x, addr);
#endif

#ifdef DEF_GET_DC_CH12_BLKCOOR_Y
def_get_mod_reg_bit(dc, ch12_blkcoor, y, addr);
#endif

#ifdef DEF_GET_DC_CH13_BLKCOOR_X
def_get_mod_reg_bit(dc, ch13_blkcoor, x, addr);
#endif

#ifdef DEF_GET_DC_CH13_BLKCOOR_Y
def_get_mod_reg_bit(dc, ch13_blkcoor, y, addr);
#endif

#ifdef DEF_GET_DC_CH14_BLKCOOR_X
def_get_mod_reg_bit(dc, ch14_blkcoor, x, addr);
#endif

#ifdef DEF_GET_DC_CH14_BLKCOOR_Y
def_get_mod_reg_bit(dc, ch14_blkcoor, y, addr);
#endif

#ifdef DEF_GET_DC_CH15_BLKCOOR_X
def_get_mod_reg_bit(dc, ch15_blkcoor, x, addr);
#endif

#ifdef DEF_GET_DC_CH15_BLKCOOR_Y
def_get_mod_reg_bit(dc, ch15_blkcoor, y, addr);
#endif

#ifdef DEF_GET_DC_CH0_BLKBUFADD_ADD
def_get_mod_reg_bit(dc, ch0_blkbufadd, add, addr);
#endif

#ifdef DEF_GET_DC_CH1_BLKBUFADD_ADD
def_get_mod_reg_bit(dc, ch1_blkbufadd, add, addr);
#endif

#ifdef DEF_GET_DC_CH2_BLKBUFADD_ADD
def_get_mod_reg_bit(dc, ch2_blkbufadd, add, addr);
#endif

#ifdef DEF_GET_DC_CH3_BLKBUFADD_ADD
def_get_mod_reg_bit(dc, ch3_blkbufadd, add, addr);
#endif

#ifdef DEF_GET_DC_CH4_BLKBUFADD_ADD
def_get_mod_reg_bit(dc, ch4_blkbufadd, add, addr);
#endif

#ifdef DEF_GET_DC_CH5_BLKBUFADD_ADD
def_get_mod_reg_bit(dc, ch5_blkbufadd, add, addr);
#endif

#ifdef DEF_GET_DC_CH6_BLKBUFADD_ADD
def_get_mod_reg_bit(dc, ch6_blkbufadd, add, addr);
#endif

#ifdef DEF_GET_DC_CH7_BLKBUFADD_ADD
def_get_mod_reg_bit(dc, ch7_blkbufadd, add, addr);
#endif

#ifdef DEF_GET_DC_CH8_BLKBUFADD_ADD
def_get_mod_reg_bit(dc, ch8_blkbufadd, add, addr);
#endif

#ifdef DEF_GET_DC_CH9_BLKBUFADD_ADD
def_get_mod_reg_bit(dc, ch9_blkbufadd, add, addr);
#endif

#ifdef DEF_GET_DC_CH10_BLKBUFADD_ADD
def_get_mod_reg_bit(dc, ch10_blkbufadd, add, addr);
#endif

#ifdef DEF_GET_DC_CH11_BLKBUFADD_ADD
def_get_mod_reg_bit(dc, ch11_blkbufadd, add, addr);
#endif

#ifdef DEF_GET_DC_CH12_BLKBUFADD_ADD
def_get_mod_reg_bit(dc, ch12_blkbufadd, add, addr);
#endif

#ifdef DEF_GET_DC_CH13_BLKBUFADD_ADD
def_get_mod_reg_bit(dc, ch13_blkbufadd, add, addr);
#endif

#ifdef DEF_GET_DC_CH14_BLKBUFADD_ADD
def_get_mod_reg_bit(dc, ch14_blkbufadd, add, addr);
#endif

#ifdef DEF_GET_DC_CH15_BLKBUFADD_ADD
def_get_mod_reg_bit(dc, ch15_blkbufadd, add, addr);
#endif

#ifdef DEF_GET_DC_CH0_BLKBUFLNSTD_STD
def_get_mod_reg_bit(dc, ch0_blkbuflnstd, std, addr);
#endif

#ifdef DEF_GET_DC_CH1_BLKBUFLNSTD_STD
def_get_mod_reg_bit(dc, ch1_blkbuflnstd, std, addr);
#endif

#ifdef DEF_GET_DC_CH2_BLKBUFLNSTD_STD
def_get_mod_reg_bit(dc, ch2_blkbuflnstd, std, addr);
#endif

#ifdef DEF_GET_DC_CH3_BLKBUFLNSTD_STD
def_get_mod_reg_bit(dc, ch3_blkbuflnstd, std, addr);
#endif

#ifdef DEF_GET_DC_CH4_BLKBUFLNSTD_STD
def_get_mod_reg_bit(dc, ch4_blkbuflnstd, std, addr);
#endif

#ifdef DEF_GET_DC_CH5_BLKBUFLNSTD_STD
def_get_mod_reg_bit(dc, ch5_blkbuflnstd, std, addr);
#endif

#ifdef DEF_GET_DC_CH6_BLKBUFLNSTD_STD
def_get_mod_reg_bit(dc, ch6_blkbuflnstd, std, addr);
#endif

#ifdef DEF_GET_DC_CH7_BLKBUFLNSTD_STD
def_get_mod_reg_bit(dc, ch7_blkbuflnstd, std, addr);
#endif

#ifdef DEF_GET_DC_CH8_BLKBUFLNSTD_STD
def_get_mod_reg_bit(dc, ch8_blkbuflnstd, std, addr);
#endif

#ifdef DEF_GET_DC_CH9_BLKBUFLNSTD_STD
def_get_mod_reg_bit(dc, ch9_blkbuflnstd, std, addr);
#endif

#ifdef DEF_GET_DC_CH10_BLKBUFLNSTD_STD
def_get_mod_reg_bit(dc, ch10_blkbuflnstd, std, addr);
#endif

#ifdef DEF_GET_DC_CH11_BLKBUFLNSTD_STD
def_get_mod_reg_bit(dc, ch11_blkbuflnstd, std, addr);
#endif

#ifdef DEF_GET_DC_CH12_BLKBUFLNSTD_STD
def_get_mod_reg_bit(dc, ch12_blkbuflnstd, std, addr);
#endif

#ifdef DEF_GET_DC_CH13_BLKBUFLNSTD_STD
def_get_mod_reg_bit(dc, ch13_blkbuflnstd, std, addr);
#endif

#ifdef DEF_GET_DC_CH14_BLKBUFLNSTD_STD
def_get_mod_reg_bit(dc, ch14_blkbuflnstd, std, addr);
#endif

#ifdef DEF_GET_DC_CH15_BLKBUFLNSTD_STD
def_get_mod_reg_bit(dc, ch15_blkbuflnstd, std, addr);
#endif

#endif /* ___DC__GET___H___ */
