/* wdog_get.h */

#ifndef ___WDOG__GET___H___
#define ___WDOG__GET___H___

#ifdef DEF_GET_WDOG_WDOG_VER_VER_L
def_get_mod_reg_bit(wdog, wdog_ver, ver_l, addr);
#endif

#ifdef DEF_GET_WDOG_WDOG_VER_VER_H
def_get_mod_reg_bit(wdog, wdog_ver, ver_h, addr);
#endif

#ifdef DEF_GET_WDOG_WDOG_VER_COMP
def_get_mod_reg_bit(wdog, wdog_ver, comp, addr);
#endif

#ifdef DEF_GET_WDOG_WDOG_CLK_CTRL_SEL
def_get_mod_reg_bit(wdog, wdog_clk_ctrl, sel, addr);
#endif

#ifdef DEF_GET_WDOG_WDOG_CTRL_EN
def_get_mod_reg_bit(wdog, wdog_ctrl, en, addr);
#endif

#ifdef DEF_GET_WDOG_WDOG_CTRL_RMOD
def_get_mod_reg_bit(wdog, wdog_ctrl, rmod, addr);
#endif

#ifdef DEF_GET_WDOG_WDOG_CTRL_RPL
def_get_mod_reg_bit(wdog, wdog_ctrl, rpl, addr);
#endif

#ifdef DEF_GET_WDOG_WDOG_CTRL_SU
def_get_mod_reg_bit(wdog, wdog_ctrl, su, addr);
#endif

#ifdef DEF_GET_WDOG_WDOG_TMRPERIOD_TMRPERIOD
def_get_mod_reg_bit(wdog, wdog_tmrperiod, tmrperiod, addr);
#endif

#ifdef DEF_GET_WDOG_WDOG_CNT_CUR_VAL_CNT_CUR_VAL
def_get_mod_reg_bit(wdog, wdog_cnt_cur_val, cnt_cur_val, addr);
#endif

#ifdef DEF_GET_WDOG_WDOG_RESTART_RESTART
def_get_mod_reg_bit(wdog, wdog_restart, restart, addr);
#endif

#ifdef DEF_GET_WDOG_WDOG_INT_EN_ENABLE
def_get_mod_reg_bit(wdog, wdog_int_en, enable, addr);
#endif

#ifdef DEF_GET_WDOG_WDOG_CLR_INT_PENDING_CLR
def_get_mod_reg_bit(wdog, wdog_clr_int_pending, clr, addr);
#endif

#ifdef DEF_GET_WDOG_WDOG_INT_PENDING_PENDING
def_get_mod_reg_bit(wdog, wdog_int_pending, pending, addr);
#endif

#endif /* ___WDOG__GET___H___ */
