/* dc_set.h */

#ifndef ___DC__SET___H___
#define ___DC__SET___H___

#ifdef DEF_SET_DC_GNECTL
void set_dc_gnectl(u32 reg_addr,
		u32 dcen,
		u32 dcrst,
		u32 fiforst,
		u32 dbc,
		u32 dbautold,
		u32 outmode,
		u32 fieldpol,
		u32 m_or_r);
#endif

#ifdef DEF_SET_DC_GNECTL_DCEN
def_set_mod_reg_bit(dc, gnectl, dcen, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_GNECTL_DCRST
def_set_mod_reg_bit(dc, gnectl, dcrst, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_GNECTL_FIFORST
def_set_mod_reg_bit(dc, gnectl, fiforst, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_GNECTL_DBC
def_set_mod_reg_bit(dc, gnectl, dbc, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_GNECTL_DBAUTOLD
def_set_mod_reg_bit(dc, gnectl, dbautold, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_GNECTL_OUTMODE
def_set_mod_reg_bit(dc, gnectl, outmode, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_GNECTL_FIELDPOL
def_set_mod_reg_bit(dc, gnectl, fieldpol, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_IMGSRCCTL
void set_dc_imgsrcctl(u32 reg_addr,
			u32 win0en,
			u32 win1en,
			u32 win2en,
			u32 win3en,
			u32 pipe0bypass,
			u32 pipe1bypass,
			u32 hwcen,
			u32 blken,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_IMGSRCCTL_WIN0EN
def_set_mod_reg_bit(dc, imgsrcctl, win0en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_IMGSRCCTL_WIN1EN
def_set_mod_reg_bit(dc, imgsrcctl, win1en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_IMGSRCCTL_WIN2EN
def_set_mod_reg_bit(dc, imgsrcctl, win2en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_IMGSRCCTL_WIN3EN
def_set_mod_reg_bit(dc, imgsrcctl, win3en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_IMGSRCCTL_PIPE0BYPASS
def_set_mod_reg_bit(dc, imgsrcctl, pipe0bypass, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_IMGSRCCTL_PIPE1BYPASS
def_set_mod_reg_bit(dc, imgsrcctl, pipe1bypass, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_IMGSRCCTL_HWCEN
def_set_mod_reg_bit(dc, imgsrcctl, hwcen, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_IMGSRCCTL_BLKEN
def_set_mod_reg_bit(dc, imgsrcctl, blken, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_SCRSIZ
void set_dc_scrsiz(u32 reg_addr,
		u32 w,
		u32 h,
		u32 m_or_r);
#endif

#ifdef DEF_SET_DC_SCRSIZ_W
def_set_mod_reg_bit(dc, scrsiz, w, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_SCRSIZ_H
def_set_mod_reg_bit(dc, scrsiz, h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BKCOLOR
void set_dc_bkcolor(u32 reg_addr,
			u32 b,
			u32 g,
			u32 r,
			u32 a,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_BKCOLOR_B
def_set_mod_reg_bit(dc, bkcolor, b, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BKCOLOR_G
def_set_mod_reg_bit(dc, bkcolor, g, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BKCOLOR_R
def_set_mod_reg_bit(dc, bkcolor, r, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BKCOLOR_A
def_set_mod_reg_bit(dc, bkcolor, a, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_VER
void set_dc_ver(u32 reg_addr,
		u32 ver_l,
		u32 ver_h,
		u32 comp,
		u32 m_or_r);
#endif

#ifdef DEF_SET_DC_VER_VER_L
def_set_mod_reg_bit(dc, ver, ver_l, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_VER_VER_H
def_set_mod_reg_bit(dc, ver, ver_h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_VER_COMP
def_set_mod_reg_bit(dc, ver, comp, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_FTR
void set_dc_ftr(u32 reg_addr,
		u32 has_wb,
		u32 has_yuv,
		u32 has_hwc,
		u32 has_bkl,
		u32 has_ifb,
		u32 has_pal,
		u32 has_gamma,
		u32 has_blend,
		u32 has_colorkey,
		u32 has_csc,
		u32 has_ove,
		u32 ovenum,
		u32 winnum,
		u32 m_or_r);
#endif

#ifdef DEF_SET_DC_FTR_HAS_WB
def_set_mod_reg_bit(dc, ftr, has_wb, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_FTR_HAS_YUV
def_set_mod_reg_bit(dc, ftr, has_yuv, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_FTR_HAS_HWC
def_set_mod_reg_bit(dc, ftr, has_hwc, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_FTR_HAS_BKL
def_set_mod_reg_bit(dc, ftr, has_bkl, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_FTR_HAS_IFB
def_set_mod_reg_bit(dc, ftr, has_ifb, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_FTR_HAS_PAL
def_set_mod_reg_bit(dc, ftr, has_pal, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_FTR_HAS_GAMMA
def_set_mod_reg_bit(dc, ftr, has_gamma, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_FTR_HAS_BLEND
def_set_mod_reg_bit(dc, ftr, has_blend, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_FTR_HAS_COLORKEY
def_set_mod_reg_bit(dc, ftr, has_colorkey, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_FTR_HAS_CSC
def_set_mod_reg_bit(dc, ftr, has_csc, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_FTR_HAS_OVE
def_set_mod_reg_bit(dc, ftr, has_ove, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_FTR_OVENUM
def_set_mod_reg_bit(dc, ftr, ovenum, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_FTR_WINNUM
def_set_mod_reg_bit(dc, ftr, winnum, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN0CTL
void set_dc_win0ctl(u32 reg_addr,
			u32 winwkmod,
			u32 winsrc,
			u32 alphactl,
			u32 plaalpha,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_WIN0CTL_WINWKMOD
def_set_mod_reg_bit(dc, win0ctl, winwkmod, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN0CTL_WINSRC
def_set_mod_reg_bit(dc, win0ctl, winsrc, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN0CTL_ALPHACTL
def_set_mod_reg_bit(dc, win0ctl, alphactl, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN0CTL_PLAALPHA
def_set_mod_reg_bit(dc, win0ctl, plaalpha, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN1CTL
void set_dc_win1ctl(u32 reg_addr,
			u32 winwkmod,
			u32 winsrc,
			u32 alphactl,
			u32 plaalpha,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_WIN1CTL_WINWKMOD
def_set_mod_reg_bit(dc, win1ctl, winwkmod, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN1CTL_WINSRC
def_set_mod_reg_bit(dc, win1ctl, winsrc, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN1CTL_ALPHACTL
def_set_mod_reg_bit(dc, win1ctl, alphactl, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN1CTL_PLAALPHA
def_set_mod_reg_bit(dc, win1ctl, plaalpha, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN2CTL
void set_dc_win2ctl(u32 reg_addr,
			u32 winwkmod,
			u32 winsrc,
			u32 alphactl,
			u32 plaalpha,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_WIN2CTL_WINWKMOD
def_set_mod_reg_bit(dc, win2ctl, winwkmod, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN2CTL_WINSRC
def_set_mod_reg_bit(dc, win2ctl, winsrc, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN2CTL_ALPHACTL
def_set_mod_reg_bit(dc, win2ctl, alphactl, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN2CTL_PLAALPHA
def_set_mod_reg_bit(dc, win2ctl, plaalpha, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN3CTL
void set_dc_win3ctl(u32 reg_addr,
			u32 winwkmod,
			u32 winsrc,
			u32 alphactl,
			u32 plaalpha,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_WIN3CTL_WINWKMOD
def_set_mod_reg_bit(dc, win3ctl, winwkmod, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN3CTL_WINSRC
def_set_mod_reg_bit(dc, win3ctl, winsrc, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN3CTL_ALPHACTL
def_set_mod_reg_bit(dc, win3ctl, alphactl, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN3CTL_PLAALPHA
def_set_mod_reg_bit(dc, win3ctl, plaalpha, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN0BUFFMT
void set_dc_win0buffmt(u32 reg_addr,
			u32 po,
			u32 fmt,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_WIN0BUFFMT_PO
def_set_mod_reg_bit(dc, win0buffmt, po, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN0BUFFMT_FMT
def_set_mod_reg_bit(dc, win0buffmt, fmt, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN1BUFFMT
void set_dc_win1buffmt(u32 reg_addr,
			u32 po,
			u32 fmt,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_WIN1BUFFMT_PO
def_set_mod_reg_bit(dc, win1buffmt, po, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN1BUFFMT_FMT
def_set_mod_reg_bit(dc, win1buffmt, fmt, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN2BUFFMT
void set_dc_win2buffmt(u32 reg_addr,
			u32 po,
			u32 fmt,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_WIN2BUFFMT_PO
def_set_mod_reg_bit(dc, win2buffmt, po, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN2BUFFMT_FMT
def_set_mod_reg_bit(dc, win2buffmt, fmt, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN3BUFFMT
void set_dc_win3buffmt(u32 reg_addr,
			u32 po,
			u32 fmt,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_WIN3BUFFMT_PO
def_set_mod_reg_bit(dc, win3buffmt, po, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN3BUFFMT_FMT
def_set_mod_reg_bit(dc, win3buffmt, fmt, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN0SIZ
void set_dc_win0siz(u32 reg_addr,
			u32 w,
			u32 h,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_WIN0SIZ_W
def_set_mod_reg_bit(dc, win0siz, w, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN0SIZ_H
def_set_mod_reg_bit(dc, win0siz, h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN1SIZ
void set_dc_win1siz(u32 reg_addr,
			u32 w,
			u32 h,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_WIN1SIZ_W
def_set_mod_reg_bit(dc, win1siz, w, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN1SIZ_H
def_set_mod_reg_bit(dc, win1siz, h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN2SIZ
void set_dc_win2siz(u32 reg_addr,
			u32 w,
			u32 h,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_WIN2SIZ_W
def_set_mod_reg_bit(dc, win2siz, w, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN2SIZ_H
def_set_mod_reg_bit(dc, win2siz, h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN3SIZ
void set_dc_win3siz(u32 reg_addr,
			u32 w,
			u32 h,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_WIN3SIZ_W
def_set_mod_reg_bit(dc, win3siz, w, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN3SIZ_H
def_set_mod_reg_bit(dc, win3siz, h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN0COOR
void set_dc_win0coor(u32 reg_addr,
			u32 x,
			u32 y,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_WIN0COOR_X
def_set_mod_reg_bit(dc, win0coor, x, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN0COOR_Y
def_set_mod_reg_bit(dc, win0coor, y, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN1COOR
void set_dc_win1coor(u32 reg_addr,
			u32 x,
			u32 y,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_WIN1COOR_X
def_set_mod_reg_bit(dc, win1coor, x, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN1COOR_Y
def_set_mod_reg_bit(dc, win1coor, y, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN2COOR
void set_dc_win2coor(u32 reg_addr,
			u32 x,
			u32 y,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_WIN2COOR_X
def_set_mod_reg_bit(dc, win2coor, x, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN2COOR_Y
def_set_mod_reg_bit(dc, win2coor, y, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN3COOR
void set_dc_win3coor(u32 reg_addr,
			u32 x,
			u32 y,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_WIN3COOR_X
def_set_mod_reg_bit(dc, win3coor, x, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN3COOR_Y
def_set_mod_reg_bit(dc, win3coor, y, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN0BUFLADD
void set_dc_win0bufladd(u32 reg_addr,
			u32 add,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_WIN0BUFLADD_ADD
def_set_mod_reg_bit(dc, win0bufladd, add, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN1BUFLADD
void set_dc_win1bufladd(u32 reg_addr,
			u32 add,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_WIN1BUFLADD_ADD
def_set_mod_reg_bit(dc, win1bufladd, add, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN2BUFLADD
void set_dc_win2bufladd(u32 reg_addr,
			u32 add,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_WIN2BUFLADD_ADD
def_set_mod_reg_bit(dc, win2bufladd, add, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN3BUFLADD
void set_dc_win3bufladd(u32 reg_addr,
			u32 add,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_WIN3BUFLADD_ADD
def_set_mod_reg_bit(dc, win3bufladd, add, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WINBUFHADD
void set_dc_winbufhadd(u32 reg_addr,
			u32 win0add,
			u32 win1add,
			u32 win2add,
			u32 win3add,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_WINBUFHADD_WIN0ADD
def_set_mod_reg_bit(dc, winbufhadd, win0add, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WINBUFHADD_WIN1ADD
def_set_mod_reg_bit(dc, winbufhadd, win1add, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WINBUFHADD_WIN2ADD
def_set_mod_reg_bit(dc, winbufhadd, win2add, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WINBUFHADD_WIN3ADD
def_set_mod_reg_bit(dc, winbufhadd, win3add, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN0BUFLNSTD
void set_dc_win0buflnstd(u32 reg_addr,
			u32 std,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_WIN0BUFLNSTD_STD
def_set_mod_reg_bit(dc, win0buflnstd, std, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN1BUFLNSTD
void set_dc_win1buflnstd(u32 reg_addr,
			u32 std,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_WIN1BUFLNSTD_STD
def_set_mod_reg_bit(dc, win1buflnstd, std, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN2BUFLNSTD
void set_dc_win2buflnstd(u32 reg_addr,
			u32 std,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_WIN2BUFLNSTD_STD
def_set_mod_reg_bit(dc, win2buflnstd, std, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WIN3BUFLNSTD
void set_dc_win3buflnstd(u32 reg_addr,
			u32 std,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_WIN3BUFLNSTD_STD
def_set_mod_reg_bit(dc, win3buflnstd, std, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD0CTL
void set_dc_bld0ctl(u32 reg_addr,
			u32 cken,
			u32 ckmatch,
			u32 ckbcon,
			u32 ckgcon,
			u32 ckrcon,
			u32 c0,
			u32 c1,
			u32 c2,
			u32 c3,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_BLD0CTL_CKEN
def_set_mod_reg_bit(dc, bld0ctl, cken, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD0CTL_CKMATCH
def_set_mod_reg_bit(dc, bld0ctl, ckmatch, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD0CTL_CKBCON
def_set_mod_reg_bit(dc, bld0ctl, ckbcon, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD0CTL_CKGCON
def_set_mod_reg_bit(dc, bld0ctl, ckgcon, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD0CTL_CKRCON
def_set_mod_reg_bit(dc, bld0ctl, ckrcon, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD0CTL_C0
def_set_mod_reg_bit(dc, bld0ctl, c0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD0CTL_C1
def_set_mod_reg_bit(dc, bld0ctl, c1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD0CTL_C2
def_set_mod_reg_bit(dc, bld0ctl, c2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD0CTL_C3
def_set_mod_reg_bit(dc, bld0ctl, c3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD1CTL
void set_dc_bld1ctl(u32 reg_addr,
			u32 cken,
			u32 ckmatch,
			u32 ckbcon,
			u32 ckgcon,
			u32 ckrcon,
			u32 c0,
			u32 c1,
			u32 c2,
			u32 c3,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_BLD1CTL_CKEN
def_set_mod_reg_bit(dc, bld1ctl, cken, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD1CTL_CKMATCH
def_set_mod_reg_bit(dc, bld1ctl, ckmatch, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD1CTL_CKBCON
def_set_mod_reg_bit(dc, bld1ctl, ckbcon, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD1CTL_CKGCON
def_set_mod_reg_bit(dc, bld1ctl, ckgcon, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD1CTL_CKRCON
def_set_mod_reg_bit(dc, bld1ctl, ckrcon, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD1CTL_C0
def_set_mod_reg_bit(dc, bld1ctl, c0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD1CTL_C1
def_set_mod_reg_bit(dc, bld1ctl, c1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD1CTL_C2
def_set_mod_reg_bit(dc, bld1ctl, c2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD1CTL_C3
def_set_mod_reg_bit(dc, bld1ctl, c3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD2CTL
void set_dc_bld2ctl(u32 reg_addr,
			u32 cken,
			u32 ckmatch,
			u32 ckbcon,
			u32 ckgcon,
			u32 ckrcon,
			u32 c0,
			u32 c1,
			u32 c2,
			u32 c3,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_BLD2CTL_CKEN
def_set_mod_reg_bit(dc, bld2ctl, cken, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD2CTL_CKMATCH
def_set_mod_reg_bit(dc, bld2ctl, ckmatch, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD2CTL_CKBCON
def_set_mod_reg_bit(dc, bld2ctl, ckbcon, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD2CTL_CKGCON
def_set_mod_reg_bit(dc, bld2ctl, ckgcon, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD2CTL_CKRCON
def_set_mod_reg_bit(dc, bld2ctl, ckrcon, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD2CTL_C0
def_set_mod_reg_bit(dc, bld2ctl, c0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD2CTL_C1
def_set_mod_reg_bit(dc, bld2ctl, c1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD2CTL_C2
def_set_mod_reg_bit(dc, bld2ctl, c2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD2CTL_C3
def_set_mod_reg_bit(dc, bld2ctl, c3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD3CTL
void set_dc_bld3ctl(u32 reg_addr,
			u32 cken,
			u32 ckmatch,
			u32 ckbcon,
			u32 ckgcon,
			u32 ckrcon,
			u32 c0,
			u32 c1,
			u32 c2,
			u32 c3,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_BLD3CTL_CKEN
def_set_mod_reg_bit(dc, bld3ctl, cken, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD3CTL_CKMATCH
def_set_mod_reg_bit(dc, bld3ctl, ckmatch, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD3CTL_CKBCON
def_set_mod_reg_bit(dc, bld3ctl, ckbcon, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD3CTL_CKGCON
def_set_mod_reg_bit(dc, bld3ctl, ckgcon, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD3CTL_CKRCON
def_set_mod_reg_bit(dc, bld3ctl, ckrcon, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD3CTL_C0
def_set_mod_reg_bit(dc, bld3ctl, c0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD3CTL_C1
def_set_mod_reg_bit(dc, bld3ctl, c1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD3CTL_C2
def_set_mod_reg_bit(dc, bld3ctl, c2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD3CTL_C3
def_set_mod_reg_bit(dc, bld3ctl, c3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD4CTL
void set_dc_bld4ctl(u32 reg_addr,
			u32 cken,
			u32 ckmatch,
			u32 ckbcon,
			u32 ckgcon,
			u32 ckrcon,
			u32 c0,
			u32 c1,
			u32 c2,
			u32 c3,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_BLD4CTL_CKEN
def_set_mod_reg_bit(dc, bld4ctl, cken, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD4CTL_CKMATCH
def_set_mod_reg_bit(dc, bld4ctl, ckmatch, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD4CTL_CKBCON
def_set_mod_reg_bit(dc, bld4ctl, ckbcon, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD4CTL_CKGCON
def_set_mod_reg_bit(dc, bld4ctl, ckgcon, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD4CTL_CKRCON
def_set_mod_reg_bit(dc, bld4ctl, ckrcon, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD4CTL_C0
def_set_mod_reg_bit(dc, bld4ctl, c0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD4CTL_C1
def_set_mod_reg_bit(dc, bld4ctl, c1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD4CTL_C2
def_set_mod_reg_bit(dc, bld4ctl, c2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD4CTL_C3
def_set_mod_reg_bit(dc, bld4ctl, c3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD0CKMIN
void set_dc_bld0ckmin(u32 reg_addr,
			u32 ckbmin,
			u32 ckgmin,
			u32 ckrmin,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_BLD0CKMIN_CKBMIN
def_set_mod_reg_bit(dc, bld0ckmin, ckbmin, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD0CKMIN_CKGMIN
def_set_mod_reg_bit(dc, bld0ckmin, ckgmin, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD0CKMIN_CKRMIN
def_set_mod_reg_bit(dc, bld0ckmin, ckrmin, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD1CKMIN
void set_dc_bld1ckmin(u32 reg_addr,
			u32 ckbmin,
			u32 ckgmin,
			u32 ckrmin,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_BLD1CKMIN_CKBMIN
def_set_mod_reg_bit(dc, bld1ckmin, ckbmin, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD1CKMIN_CKGMIN
def_set_mod_reg_bit(dc, bld1ckmin, ckgmin, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD1CKMIN_CKRMIN
def_set_mod_reg_bit(dc, bld1ckmin, ckrmin, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD2CKMIN
void set_dc_bld2ckmin(u32 reg_addr,
			u32 ckbmin,
			u32 ckgmin,
			u32 ckrmin,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_BLD2CKMIN_CKBMIN
def_set_mod_reg_bit(dc, bld2ckmin, ckbmin, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD2CKMIN_CKGMIN
def_set_mod_reg_bit(dc, bld2ckmin, ckgmin, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD2CKMIN_CKRMIN
def_set_mod_reg_bit(dc, bld2ckmin, ckrmin, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD3CKMIN
void set_dc_bld3ckmin(u32 reg_addr,
			u32 ckbmin,
			u32 ckgmin,
			u32 ckrmin,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_BLD3CKMIN_CKBMIN
def_set_mod_reg_bit(dc, bld3ckmin, ckbmin, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD3CKMIN_CKGMIN
def_set_mod_reg_bit(dc, bld3ckmin, ckgmin, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD3CKMIN_CKRMIN
def_set_mod_reg_bit(dc, bld3ckmin, ckrmin, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD4CKMIN
void set_dc_bld4ckmin(u32 reg_addr,
			u32 ckbmin,
			u32 ckgmin,
			u32 ckrmin,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_BLD4CKMIN_CKBMIN
def_set_mod_reg_bit(dc, bld4ckmin, ckbmin, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD4CKMIN_CKGMIN
def_set_mod_reg_bit(dc, bld4ckmin, ckgmin, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD4CKMIN_CKRMIN
def_set_mod_reg_bit(dc, bld4ckmin, ckrmin, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD0CKMAX
void set_dc_bld0ckmax(u32 reg_addr,
			u32 ckbmax,
			u32 ckgmax,
			u32 ckrmax,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_BLD0CKMAX_CKBMAX
def_set_mod_reg_bit(dc, bld0ckmax, ckbmax, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD0CKMAX_CKGMAX
def_set_mod_reg_bit(dc, bld0ckmax, ckgmax, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD0CKMAX_CKRMAX
def_set_mod_reg_bit(dc, bld0ckmax, ckrmax, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD1CKMAX
void set_dc_bld1ckmax(u32 reg_addr,
			u32 ckbmax,
			u32 ckgmax,
			u32 ckrmax,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_BLD1CKMAX_CKBMAX
def_set_mod_reg_bit(dc, bld1ckmax, ckbmax, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD1CKMAX_CKGMAX
def_set_mod_reg_bit(dc, bld1ckmax, ckgmax, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD1CKMAX_CKRMAX
def_set_mod_reg_bit(dc, bld1ckmax, ckrmax, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD2CKMAX
void set_dc_bld2ckmax(u32 reg_addr,
			u32 ckbmax,
			u32 ckgmax,
			u32 ckrmax,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_BLD2CKMAX_CKBMAX
def_set_mod_reg_bit(dc, bld2ckmax, ckbmax, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD2CKMAX_CKGMAX
def_set_mod_reg_bit(dc, bld2ckmax, ckgmax, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD2CKMAX_CKRMAX
def_set_mod_reg_bit(dc, bld2ckmax, ckrmax, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD3CKMAX
void set_dc_bld3ckmax(u32 reg_addr,
			u32 ckbmax,
			u32 ckgmax,
			u32 ckrmax,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_BLD3CKMAX_CKBMAX
def_set_mod_reg_bit(dc, bld3ckmax, ckbmax, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD3CKMAX_CKGMAX
def_set_mod_reg_bit(dc, bld3ckmax, ckgmax, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD3CKMAX_CKRMAX
def_set_mod_reg_bit(dc, bld3ckmax, ckrmax, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD4CKMAX
void set_dc_bld4ckmax(u32 reg_addr,
			u32 ckbmax,
			u32 ckgmax,
			u32 ckrmax,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_BLD4CKMAX_CKBMAX
def_set_mod_reg_bit(dc, bld4ckmax, ckbmax, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD4CKMAX_CKGMAX
def_set_mod_reg_bit(dc, bld4ckmax, ckgmax, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLD4CKMAX_CKRMAX
def_set_mod_reg_bit(dc, bld4ckmax, ckrmax, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_HWCCTL
void set_dc_hwcctl(u32 reg_addr,
		u32 w,
		u32 h,
		u32 fmt,
		u32 xoff,
		u32 yoff,
		u32 m_or_r);
#endif

#ifdef DEF_SET_DC_HWCCTL_W
def_set_mod_reg_bit(dc, hwcctl, w, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_HWCCTL_H
def_set_mod_reg_bit(dc, hwcctl, h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_HWCCTL_FMT
def_set_mod_reg_bit(dc, hwcctl, fmt, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_HWCCTL_XOFF
def_set_mod_reg_bit(dc, hwcctl, xoff, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_HWCCTL_YOFF
def_set_mod_reg_bit(dc, hwcctl, yoff, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_HWCCOOR
void set_dc_hwccoor(u32 reg_addr,
			u32 x,
			u32 y,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_HWCCOOR_X
def_set_mod_reg_bit(dc, hwccoor, x, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_HWCCOOR_Y
def_set_mod_reg_bit(dc, hwccoor, y, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CEEN
void set_dc_ceen(u32 reg_addr,
		u32 en,
		u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CEEN_EN
def_set_mod_reg_bit(dc, ceen, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CERRCOEF
void set_dc_cerrcoef(u32 reg_addr,
			u32 coef,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CERRCOEF_COEF
def_set_mod_reg_bit(dc, cerrcoef, coef, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CERGCOEF
void set_dc_cergcoef(u32 reg_addr,
			u32 coef,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CERGCOEF_COEF
def_set_mod_reg_bit(dc, cergcoef, coef, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CERBCOEF
void set_dc_cerbcoef(u32 reg_addr,
			u32 coef,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CERBCOEF_COEF
def_set_mod_reg_bit(dc, cerbcoef, coef, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CERCONS
void set_dc_cercons(u32 reg_addr,
			u32 cons,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CERCONS_CONS
def_set_mod_reg_bit(dc, cercons, cons, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CEGRCOEF
void set_dc_cegrcoef(u32 reg_addr,
			u32 coef,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CEGRCOEF_COEF
def_set_mod_reg_bit(dc, cegrcoef, coef, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CEGGCOEF
void set_dc_ceggcoef(u32 reg_addr,
			u32 coef,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CEGGCOEF_COEF
def_set_mod_reg_bit(dc, ceggcoef, coef, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CEGBCOEF
void set_dc_cegbcoef(u32 reg_addr,
			u32 coef,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CEGBCOEF_COEF
def_set_mod_reg_bit(dc, cegbcoef, coef, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CEGCONS
void set_dc_cegcons(u32 reg_addr,
			u32 cons,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CEGCONS_CONS
def_set_mod_reg_bit(dc, cegcons, cons, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CEBRCOEF
void set_dc_cebrcoef(u32 reg_addr,
			u32 coef,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CEBRCOEF_COEF
def_set_mod_reg_bit(dc, cebrcoef, coef, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CEBGCOEF
void set_dc_cebgcoef(u32 reg_addr,
			u32 coef,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CEBGCOEF_COEF
def_set_mod_reg_bit(dc, cebgcoef, coef, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CEBBCOEF
void set_dc_cebbcoef(u32 reg_addr,
			u32 coef,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CEBBCOEF_COEF
def_set_mod_reg_bit(dc, cebbcoef, coef, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CEBCONS
void set_dc_cebcons(u32 reg_addr,
			u32 cons,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CEBCONS_CONS
def_set_mod_reg_bit(dc, cebcons, cons, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLKGNE
void set_dc_blkgne(u32 reg_addr,
		u32 fmt,
		u32 po,
		u32 alphactl,
		u32 plaalpha,
		u32 m_or_r);
#endif

#ifdef DEF_SET_DC_BLKGNE_FMT
def_set_mod_reg_bit(dc, blkgne, fmt, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLKGNE_PO
def_set_mod_reg_bit(dc, blkgne, po, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLKGNE_ALPHACTL
def_set_mod_reg_bit(dc, blkgne, alphactl, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_BLKGNE_PLAALPHA
def_set_mod_reg_bit(dc, blkgne, plaalpha, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WBC
void set_dc_wbc(u32 reg_addr,
		u32 sta,
		u32 wbmod,
		u32 fmt,
		u32 wbstatus,
		u32 wbexc,
		u32 m_or_r);
#endif

#ifdef DEF_SET_DC_WBC_STA
def_set_mod_reg_bit(dc, wbc, sta, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WBC_WBMOD
def_set_mod_reg_bit(dc, wbc, wbmod, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WBC_FMT
def_set_mod_reg_bit(dc, wbc, fmt, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WBC_WBSTATUS
def_set_mod_reg_bit(dc, wbc, wbstatus, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WBC_WBEXC
def_set_mod_reg_bit(dc, wbc, wbexc, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WBADD
void set_dc_wbadd(u32 reg_addr,
		u32 add,
		u32 m_or_r);
#endif

#ifdef DEF_SET_DC_WBADD_ADD
def_set_mod_reg_bit(dc, wbadd, add, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_WBLNSTD
void set_dc_wblnstd(u32 reg_addr,
			u32 std,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_WBLNSTD_STD
def_set_mod_reg_bit(dc, wblnstd, std, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH0_BLKCTL
void set_dc_ch0_blkctl(u32 reg_addr,
			u32 w,
			u32 h,
			u32 np,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH0_BLKCTL_W
def_set_mod_reg_bit(dc, ch0_blkctl, w, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH0_BLKCTL_H
def_set_mod_reg_bit(dc, ch0_blkctl, h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH0_BLKCTL_NP
def_set_mod_reg_bit(dc, ch0_blkctl, np, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH1_BLKCTL
void set_dc_ch1_blkctl(u32 reg_addr,
			u32 w,
			u32 h,
			u32 np,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH1_BLKCTL_W
def_set_mod_reg_bit(dc, ch1_blkctl, w, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH1_BLKCTL_H
def_set_mod_reg_bit(dc, ch1_blkctl, h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH1_BLKCTL_NP
def_set_mod_reg_bit(dc, ch1_blkctl, np, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH2_BLKCTL
void set_dc_ch2_blkctl(u32 reg_addr,
			u32 w,
			u32 h,
			u32 np,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH2_BLKCTL_W
def_set_mod_reg_bit(dc, ch2_blkctl, w, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH2_BLKCTL_H
def_set_mod_reg_bit(dc, ch2_blkctl, h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH2_BLKCTL_NP
def_set_mod_reg_bit(dc, ch2_blkctl, np, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH3_BLKCTL
void set_dc_ch3_blkctl(u32 reg_addr,
			u32 w,
			u32 h,
			u32 np,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH3_BLKCTL_W
def_set_mod_reg_bit(dc, ch3_blkctl, w, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH3_BLKCTL_H
def_set_mod_reg_bit(dc, ch3_blkctl, h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH3_BLKCTL_NP
def_set_mod_reg_bit(dc, ch3_blkctl, np, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH4_BLKCTL
void set_dc_ch4_blkctl(u32 reg_addr,
			u32 w,
			u32 h,
			u32 np,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH4_BLKCTL_W
def_set_mod_reg_bit(dc, ch4_blkctl, w, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH4_BLKCTL_H
def_set_mod_reg_bit(dc, ch4_blkctl, h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH4_BLKCTL_NP
def_set_mod_reg_bit(dc, ch4_blkctl, np, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH5_BLKCTL
void set_dc_ch5_blkctl(u32 reg_addr,
			u32 w,
			u32 h,
			u32 np,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH5_BLKCTL_W
def_set_mod_reg_bit(dc, ch5_blkctl, w, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH5_BLKCTL_H
def_set_mod_reg_bit(dc, ch5_blkctl, h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH5_BLKCTL_NP
def_set_mod_reg_bit(dc, ch5_blkctl, np, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH6_BLKCTL
void set_dc_ch6_blkctl(u32 reg_addr,
			u32 w,
			u32 h,
			u32 np,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH6_BLKCTL_W
def_set_mod_reg_bit(dc, ch6_blkctl, w, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH6_BLKCTL_H
def_set_mod_reg_bit(dc, ch6_blkctl, h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH6_BLKCTL_NP
def_set_mod_reg_bit(dc, ch6_blkctl, np, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH7_BLKCTL
void set_dc_ch7_blkctl(u32 reg_addr,
			u32 w,
			u32 h,
			u32 np,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH7_BLKCTL_W
def_set_mod_reg_bit(dc, ch7_blkctl, w, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH7_BLKCTL_H
def_set_mod_reg_bit(dc, ch7_blkctl, h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH7_BLKCTL_NP
def_set_mod_reg_bit(dc, ch7_blkctl, np, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH8_BLKCTL
void set_dc_ch8_blkctl(u32 reg_addr,
			u32 w,
			u32 h,
			u32 np,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH8_BLKCTL_W
def_set_mod_reg_bit(dc, ch8_blkctl, w, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH8_BLKCTL_H
def_set_mod_reg_bit(dc, ch8_blkctl, h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH8_BLKCTL_NP
def_set_mod_reg_bit(dc, ch8_blkctl, np, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH9_BLKCTL
void set_dc_ch9_blkctl(u32 reg_addr,
			u32 w,
			u32 h,
			u32 np,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH9_BLKCTL_W
def_set_mod_reg_bit(dc, ch9_blkctl, w, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH9_BLKCTL_H
def_set_mod_reg_bit(dc, ch9_blkctl, h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH9_BLKCTL_NP
def_set_mod_reg_bit(dc, ch9_blkctl, np, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH10_BLKCTL
void set_dc_ch10_blkctl(u32 reg_addr,
			u32 w,
			u32 h,
			u32 np,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH10_BLKCTL_W
def_set_mod_reg_bit(dc, ch10_blkctl, w, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH10_BLKCTL_H
def_set_mod_reg_bit(dc, ch10_blkctl, h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH10_BLKCTL_NP
def_set_mod_reg_bit(dc, ch10_blkctl, np, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH11_BLKCTL
void set_dc_ch11_blkctl(u32 reg_addr,
			u32 w,
			u32 h,
			u32 np,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH11_BLKCTL_W
def_set_mod_reg_bit(dc, ch11_blkctl, w, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH11_BLKCTL_H
def_set_mod_reg_bit(dc, ch11_blkctl, h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH11_BLKCTL_NP
def_set_mod_reg_bit(dc, ch11_blkctl, np, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH12_BLKCTL
void set_dc_ch12_blkctl(u32 reg_addr,
			u32 w,
			u32 h,
			u32 np,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH12_BLKCTL_W
def_set_mod_reg_bit(dc, ch12_blkctl, w, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH12_BLKCTL_H
def_set_mod_reg_bit(dc, ch12_blkctl, h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH12_BLKCTL_NP
def_set_mod_reg_bit(dc, ch12_blkctl, np, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH13_BLKCTL
void set_dc_ch13_blkctl(u32 reg_addr,
			u32 w,
			u32 h,
			u32 np,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH13_BLKCTL_W
def_set_mod_reg_bit(dc, ch13_blkctl, w, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH13_BLKCTL_H
def_set_mod_reg_bit(dc, ch13_blkctl, h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH13_BLKCTL_NP
def_set_mod_reg_bit(dc, ch13_blkctl, np, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH14_BLKCTL
void set_dc_ch14_blkctl(u32 reg_addr,
			u32 w,
			u32 h,
			u32 np,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH14_BLKCTL_W
def_set_mod_reg_bit(dc, ch14_blkctl, w, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH14_BLKCTL_H
def_set_mod_reg_bit(dc, ch14_blkctl, h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH14_BLKCTL_NP
def_set_mod_reg_bit(dc, ch14_blkctl, np, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH15_BLKCTL
void set_dc_ch15_blkctl(u32 reg_addr,
			u32 w,
			u32 h,
			u32 np,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH15_BLKCTL_W
def_set_mod_reg_bit(dc, ch15_blkctl, w, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH15_BLKCTL_H
def_set_mod_reg_bit(dc, ch15_blkctl, h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH15_BLKCTL_NP
def_set_mod_reg_bit(dc, ch15_blkctl, np, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH0_BLKCOOR
void set_dc_ch0_blkcoor(u32 reg_addr,
			u32 x,
			u32 y,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH0_BLKCOOR_X
def_set_mod_reg_bit(dc, ch0_blkcoor, x, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH0_BLKCOOR_Y
def_set_mod_reg_bit(dc, ch0_blkcoor, y, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH1_BLKCOOR
void set_dc_ch1_blkcoor(u32 reg_addr,
			u32 x,
			u32 y,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH1_BLKCOOR_X
def_set_mod_reg_bit(dc, ch1_blkcoor, x, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH1_BLKCOOR_Y
def_set_mod_reg_bit(dc, ch1_blkcoor, y, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH2_BLKCOOR
void set_dc_ch2_blkcoor(u32 reg_addr,
			u32 x,
			u32 y,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH2_BLKCOOR_X
def_set_mod_reg_bit(dc, ch2_blkcoor, x, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH2_BLKCOOR_Y
def_set_mod_reg_bit(dc, ch2_blkcoor, y, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH3_BLKCOOR
void set_dc_ch3_blkcoor(u32 reg_addr,
			u32 x,
			u32 y,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH3_BLKCOOR_X
def_set_mod_reg_bit(dc, ch3_blkcoor, x, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH3_BLKCOOR_Y
def_set_mod_reg_bit(dc, ch3_blkcoor, y, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH4_BLKCOOR
void set_dc_ch4_blkcoor(u32 reg_addr,
			u32 x,
			u32 y,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH4_BLKCOOR_X
def_set_mod_reg_bit(dc, ch4_blkcoor, x, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH4_BLKCOOR_Y
def_set_mod_reg_bit(dc, ch4_blkcoor, y, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH5_BLKCOOR
void set_dc_ch5_blkcoor(u32 reg_addr,
			u32 x,
			u32 y,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH5_BLKCOOR_X
def_set_mod_reg_bit(dc, ch5_blkcoor, x, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH5_BLKCOOR_Y
def_set_mod_reg_bit(dc, ch5_blkcoor, y, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH6_BLKCOOR
void set_dc_ch6_blkcoor(u32 reg_addr,
			u32 x,
			u32 y,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH6_BLKCOOR_X
def_set_mod_reg_bit(dc, ch6_blkcoor, x, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH6_BLKCOOR_Y
def_set_mod_reg_bit(dc, ch6_blkcoor, y, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH7_BLKCOOR
void set_dc_ch7_blkcoor(u32 reg_addr,
			u32 x,
			u32 y,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH7_BLKCOOR_X
def_set_mod_reg_bit(dc, ch7_blkcoor, x, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH7_BLKCOOR_Y
def_set_mod_reg_bit(dc, ch7_blkcoor, y, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH8_BLKCOOR
void set_dc_ch8_blkcoor(u32 reg_addr,
			u32 x,
			u32 y,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH8_BLKCOOR_X
def_set_mod_reg_bit(dc, ch8_blkcoor, x, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH8_BLKCOOR_Y
def_set_mod_reg_bit(dc, ch8_blkcoor, y, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH9_BLKCOOR
void set_dc_ch9_blkcoor(u32 reg_addr,
			u32 x,
			u32 y,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH9_BLKCOOR_X
def_set_mod_reg_bit(dc, ch9_blkcoor, x, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH9_BLKCOOR_Y
def_set_mod_reg_bit(dc, ch9_blkcoor, y, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH10_BLKCOOR
void set_dc_ch10_blkcoor(u32 reg_addr,
			u32 x,
			u32 y,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH10_BLKCOOR_X
def_set_mod_reg_bit(dc, ch10_blkcoor, x, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH10_BLKCOOR_Y
def_set_mod_reg_bit(dc, ch10_blkcoor, y, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH11_BLKCOOR
void set_dc_ch11_blkcoor(u32 reg_addr,
			u32 x,
			u32 y,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH11_BLKCOOR_X
def_set_mod_reg_bit(dc, ch11_blkcoor, x, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH11_BLKCOOR_Y
def_set_mod_reg_bit(dc, ch11_blkcoor, y, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH12_BLKCOOR
void set_dc_ch12_blkcoor(u32 reg_addr,
			u32 x,
			u32 y,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH12_BLKCOOR_X
def_set_mod_reg_bit(dc, ch12_blkcoor, x, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH12_BLKCOOR_Y
def_set_mod_reg_bit(dc, ch12_blkcoor, y, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH13_BLKCOOR
void set_dc_ch13_blkcoor(u32 reg_addr,
			u32 x,
			u32 y,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH13_BLKCOOR_X
def_set_mod_reg_bit(dc, ch13_blkcoor, x, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH13_BLKCOOR_Y
def_set_mod_reg_bit(dc, ch13_blkcoor, y, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH14_BLKCOOR
void set_dc_ch14_blkcoor(u32 reg_addr,
			u32 x,
			u32 y,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH14_BLKCOOR_X
def_set_mod_reg_bit(dc, ch14_blkcoor, x, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH14_BLKCOOR_Y
def_set_mod_reg_bit(dc, ch14_blkcoor, y, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH15_BLKCOOR
void set_dc_ch15_blkcoor(u32 reg_addr,
			u32 x,
			u32 y,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH15_BLKCOOR_X
def_set_mod_reg_bit(dc, ch15_blkcoor, x, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH15_BLKCOOR_Y
def_set_mod_reg_bit(dc, ch15_blkcoor, y, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH0_BLKBUFADD
void set_dc_ch0_blkbufadd(u32 reg_addr,
			u32 add,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH0_BLKBUFADD_ADD
def_set_mod_reg_bit(dc, ch0_blkbufadd, add, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH1_BLKBUFADD
void set_dc_ch1_blkbufadd(u32 reg_addr,
			u32 add,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH1_BLKBUFADD_ADD
def_set_mod_reg_bit(dc, ch1_blkbufadd, add, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH2_BLKBUFADD
void set_dc_ch2_blkbufadd(u32 reg_addr,
			u32 add,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH2_BLKBUFADD_ADD
def_set_mod_reg_bit(dc, ch2_blkbufadd, add, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH3_BLKBUFADD
void set_dc_ch3_blkbufadd(u32 reg_addr,
			u32 add,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH3_BLKBUFADD_ADD
def_set_mod_reg_bit(dc, ch3_blkbufadd, add, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH4_BLKBUFADD
void set_dc_ch4_blkbufadd(u32 reg_addr,
			u32 add,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH4_BLKBUFADD_ADD
def_set_mod_reg_bit(dc, ch4_blkbufadd, add, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH5_BLKBUFADD
void set_dc_ch5_blkbufadd(u32 reg_addr,
			u32 add,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH5_BLKBUFADD_ADD
def_set_mod_reg_bit(dc, ch5_blkbufadd, add, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH6_BLKBUFADD
void set_dc_ch6_blkbufadd(u32 reg_addr,
			u32 add,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH6_BLKBUFADD_ADD
def_set_mod_reg_bit(dc, ch6_blkbufadd, add, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH7_BLKBUFADD
void set_dc_ch7_blkbufadd(u32 reg_addr,
			u32 add,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH7_BLKBUFADD_ADD
def_set_mod_reg_bit(dc, ch7_blkbufadd, add, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH8_BLKBUFADD
void set_dc_ch8_blkbufadd(u32 reg_addr,
			u32 add,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH8_BLKBUFADD_ADD
def_set_mod_reg_bit(dc, ch8_blkbufadd, add, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH9_BLKBUFADD
void set_dc_ch9_blkbufadd(u32 reg_addr,
			u32 add,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH9_BLKBUFADD_ADD
def_set_mod_reg_bit(dc, ch9_blkbufadd, add, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH10_BLKBUFADD
void set_dc_ch10_blkbufadd(u32 reg_addr,
			u32 add,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH10_BLKBUFADD_ADD
def_set_mod_reg_bit(dc, ch10_blkbufadd, add, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH11_BLKBUFADD
void set_dc_ch11_blkbufadd(u32 reg_addr,
			u32 add,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH11_BLKBUFADD_ADD
def_set_mod_reg_bit(dc, ch11_blkbufadd, add, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH12_BLKBUFADD
void set_dc_ch12_blkbufadd(u32 reg_addr,
			u32 add,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH12_BLKBUFADD_ADD
def_set_mod_reg_bit(dc, ch12_blkbufadd, add, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH13_BLKBUFADD
void set_dc_ch13_blkbufadd(u32 reg_addr,
			u32 add,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH13_BLKBUFADD_ADD
def_set_mod_reg_bit(dc, ch13_blkbufadd, add, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH14_BLKBUFADD
void set_dc_ch14_blkbufadd(u32 reg_addr,
			u32 add,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH14_BLKBUFADD_ADD
def_set_mod_reg_bit(dc, ch14_blkbufadd, add, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH15_BLKBUFADD
void set_dc_ch15_blkbufadd(u32 reg_addr,
			u32 add,
			u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH15_BLKBUFADD_ADD
def_set_mod_reg_bit(dc, ch15_blkbufadd, add, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH0_BLKBUFLNSTD
void set_dc_ch0_blkbuflnstd(u32 reg_addr,
				u32 std,
				u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH0_BLKBUFLNSTD_STD
def_set_mod_reg_bit(dc, ch0_blkbuflnstd, std, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH1_BLKBUFLNSTD
void set_dc_ch1_blkbuflnstd(u32 reg_addr,
				u32 std,
				u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH1_BLKBUFLNSTD_STD
def_set_mod_reg_bit(dc, ch1_blkbuflnstd, std, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH2_BLKBUFLNSTD
void set_dc_ch2_blkbuflnstd(u32 reg_addr,
				u32 std,
				u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH2_BLKBUFLNSTD_STD
def_set_mod_reg_bit(dc, ch2_blkbuflnstd, std, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH3_BLKBUFLNSTD
void set_dc_ch3_blkbuflnstd(u32 reg_addr,
				u32 std,
				u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH3_BLKBUFLNSTD_STD
def_set_mod_reg_bit(dc, ch3_blkbuflnstd, std, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH4_BLKBUFLNSTD
void set_dc_ch4_blkbuflnstd(u32 reg_addr,
				u32 std,
				u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH4_BLKBUFLNSTD_STD
def_set_mod_reg_bit(dc, ch4_blkbuflnstd, std, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH5_BLKBUFLNSTD
void set_dc_ch5_blkbuflnstd(u32 reg_addr,
				u32 std,
				u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH5_BLKBUFLNSTD_STD
def_set_mod_reg_bit(dc, ch5_blkbuflnstd, std, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH6_BLKBUFLNSTD
void set_dc_ch6_blkbuflnstd(u32 reg_addr,
				u32 std,
				u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH6_BLKBUFLNSTD_STD
def_set_mod_reg_bit(dc, ch6_blkbuflnstd, std, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH7_BLKBUFLNSTD
void set_dc_ch7_blkbuflnstd(u32 reg_addr,
				u32 std,
				u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH7_BLKBUFLNSTD_STD
def_set_mod_reg_bit(dc, ch7_blkbuflnstd, std, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH8_BLKBUFLNSTD
void set_dc_ch8_blkbuflnstd(u32 reg_addr,
				u32 std,
				u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH8_BLKBUFLNSTD_STD
def_set_mod_reg_bit(dc, ch8_blkbuflnstd, std, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH9_BLKBUFLNSTD
void set_dc_ch9_blkbuflnstd(u32 reg_addr,
				u32 std,
				u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH9_BLKBUFLNSTD_STD
def_set_mod_reg_bit(dc, ch9_blkbuflnstd, std, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH10_BLKBUFLNSTD
void set_dc_ch10_blkbuflnstd(u32 reg_addr,
				u32 std,
				u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH10_BLKBUFLNSTD_STD
def_set_mod_reg_bit(dc, ch10_blkbuflnstd, std, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH11_BLKBUFLNSTD
void set_dc_ch11_blkbuflnstd(u32 reg_addr,
				u32 std,
				u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH11_BLKBUFLNSTD_STD
def_set_mod_reg_bit(dc, ch11_blkbuflnstd, std, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH12_BLKBUFLNSTD
void set_dc_ch12_blkbuflnstd(u32 reg_addr,
				u32 std,
				u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH12_BLKBUFLNSTD_STD
def_set_mod_reg_bit(dc, ch12_blkbuflnstd, std, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH13_BLKBUFLNSTD
void set_dc_ch13_blkbuflnstd(u32 reg_addr,
				u32 std,
				u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH13_BLKBUFLNSTD_STD
def_set_mod_reg_bit(dc, ch13_blkbuflnstd, std, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH14_BLKBUFLNSTD
void set_dc_ch14_blkbuflnstd(u32 reg_addr,
				u32 std,
				u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH14_BLKBUFLNSTD_STD
def_set_mod_reg_bit(dc, ch14_blkbuflnstd, std, addr, val, m_or_r);
#endif

#ifdef DEF_SET_DC_CH15_BLKBUFLNSTD
void set_dc_ch15_blkbuflnstd(u32 reg_addr,
				u32 std,
				u32 m_or_r);
#endif

#ifdef DEF_SET_DC_CH15_BLKBUFLNSTD_STD
def_set_mod_reg_bit(dc, ch15_blkbuflnstd, std, addr, val, m_or_r);
#endif

#endif /* ___DC__SET___H___ */
