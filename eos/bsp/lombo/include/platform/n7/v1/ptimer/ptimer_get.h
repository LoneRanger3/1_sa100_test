/* ptimer_get.h */

#ifndef ___PTIMER__GET___H___
#define ___PTIMER__GET___H___

#ifdef DEF_GET_PTIMER_PTMR_VER_VER_L
def_get_mod_reg_bit(ptimer, ptmr_ver, ver_l, addr);
#endif

#ifdef DEF_GET_PTIMER_PTMR_VER_VER_H
def_get_mod_reg_bit(ptimer, ptmr_ver, ver_h, addr);
#endif

#ifdef DEF_GET_PTIMER_PTMR_VER_COMP
def_get_mod_reg_bit(ptimer, ptmr_ver, comp, addr);
#endif

#ifdef DEF_GET_PTIMER_PTMR0_STORE_LOW_VAL_LOW
def_get_mod_reg_bit(ptimer, ptmr0_store_low_val, low, addr);
#endif

#ifdef DEF_GET_PTIMER_PTMR0_STORE_HIGH_VAL_HIGH
def_get_mod_reg_bit(ptimer, ptmr0_store_high_val, high, addr);
#endif

#ifdef DEF_GET_PTIMER_PTMR0_CUR_LOW_VAL_LOW
def_get_mod_reg_bit(ptimer, ptmr0_cur_low_val, low, addr);
#endif

#ifdef DEF_GET_PTIMER_PTMR0_CUR_HIGH_VAL_HIGH
def_get_mod_reg_bit(ptimer, ptmr0_cur_high_val, high, addr);
#endif

#ifdef DEF_GET_PTIMER_PTMR0_CTRL_ENABLE
def_get_mod_reg_bit(ptimer, ptmr0_ctrl, enable, addr);
#endif

#ifdef DEF_GET_PTIMER_PTMR0_CTRL_MODE
def_get_mod_reg_bit(ptimer, ptmr0_ctrl, mode, addr);
#endif

#ifdef DEF_GET_PTIMER_PTMR0_CTRL_DIRECTION
def_get_mod_reg_bit(ptimer, ptmr0_ctrl, direction, addr);
#endif

#ifdef DEF_GET_PTIMER_PTMR0_CTRL_DIV
def_get_mod_reg_bit(ptimer, ptmr0_ctrl, div, addr);
#endif

#ifdef DEF_GET_PTIMER_PTMR0_INT_EN_ENABLE
def_get_mod_reg_bit(ptimer, ptmr0_int_en, enable, addr);
#endif

#ifdef DEF_GET_PTIMER_PTMR0_CLR_INT_PENDING_CLR
def_get_mod_reg_bit(ptimer, ptmr0_clr_int_pending, clr, addr);
#endif

#ifdef DEF_GET_PTIMER_PTMR0_INT_PENDING_PENDING
def_get_mod_reg_bit(ptimer, ptmr0_int_pending, pending, addr);
#endif

#ifdef DEF_GET_PTIMER_PTMR1_STORE_LOW_VAL_LOW
def_get_mod_reg_bit(ptimer, ptmr1_store_low_val, low, addr);
#endif

#ifdef DEF_GET_PTIMER_PTMR1_STORE_HIGH_VAL_HIGH
def_get_mod_reg_bit(ptimer, ptmr1_store_high_val, high, addr);
#endif

#ifdef DEF_GET_PTIMER_PTMR1_CUR_LOW_VAL_LOW
def_get_mod_reg_bit(ptimer, ptmr1_cur_low_val, low, addr);
#endif

#ifdef DEF_GET_PTIMER_PTMR1_CUR_HIGH_VAL_HIGH
def_get_mod_reg_bit(ptimer, ptmr1_cur_high_val, high, addr);
#endif

#ifdef DEF_GET_PTIMER_PTMR1_CTRL_ENABLE
def_get_mod_reg_bit(ptimer, ptmr1_ctrl, enable, addr);
#endif

#ifdef DEF_GET_PTIMER_PTMR1_CTRL_MODE
def_get_mod_reg_bit(ptimer, ptmr1_ctrl, mode, addr);
#endif

#ifdef DEF_GET_PTIMER_PTMR1_CTRL_DIRECTION
def_get_mod_reg_bit(ptimer, ptmr1_ctrl, direction, addr);
#endif

#ifdef DEF_GET_PTIMER_PTMR1_CTRL_DIV
def_get_mod_reg_bit(ptimer, ptmr1_ctrl, div, addr);
#endif

#ifdef DEF_GET_PTIMER_PTMR1_INT_EN_ENABLE
def_get_mod_reg_bit(ptimer, ptmr1_int_en, enable, addr);
#endif

#ifdef DEF_GET_PTIMER_PTMR1_CLR_INT_PENDING_CLR
def_get_mod_reg_bit(ptimer, ptmr1_clr_int_pending, clr, addr);
#endif

#ifdef DEF_GET_PTIMER_PTMR1_INT_PENDING_PENDING
def_get_mod_reg_bit(ptimer, ptmr1_int_pending, pending, addr);
#endif

#endif /* ___PTIMER__GET___H___ */
