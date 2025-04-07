/* ptimer_set.h */

#ifndef ___PTIMER__SET___H___
#define ___PTIMER__SET___H___

#ifdef DEF_SET_PTIMER_PTMR_VER
void set_ptimer_ptmr_ver(u32 reg_addr,
			u32 ver_l,
			u32 ver_h,
			u32 comp,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PTIMER_PTMR_VER_VER_L
def_set_mod_reg_bit(ptimer, ptmr_ver, ver_l, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PTIMER_PTMR_VER_VER_H
def_set_mod_reg_bit(ptimer, ptmr_ver, ver_h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PTIMER_PTMR_VER_COMP
def_set_mod_reg_bit(ptimer, ptmr_ver, comp, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PTIMER_PTMR0_STORE_LOW_VAL
void set_ptimer_ptmr0_store_low_val(u32 reg_addr,
					u32 low,
					u32 m_or_r);
#endif

#ifdef DEF_SET_PTIMER_PTMR0_STORE_LOW_VAL_LOW
def_set_mod_reg_bit(ptimer, ptmr0_store_low_val, low, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PTIMER_PTMR0_STORE_HIGH_VAL
void set_ptimer_ptmr0_store_high_val(u32 reg_addr,
					u32 high,
					u32 m_or_r);
#endif

#ifdef DEF_SET_PTIMER_PTMR0_STORE_HIGH_VAL_HIGH
def_set_mod_reg_bit(ptimer, ptmr0_store_high_val, high, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PTIMER_PTMR0_CUR_LOW_VAL
void set_ptimer_ptmr0_cur_low_val(u32 reg_addr,
				u32 low,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PTIMER_PTMR0_CUR_LOW_VAL_LOW
def_set_mod_reg_bit(ptimer, ptmr0_cur_low_val, low, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PTIMER_PTMR0_CUR_HIGH_VAL
void set_ptimer_ptmr0_cur_high_val(u32 reg_addr,
				u32 high,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PTIMER_PTMR0_CUR_HIGH_VAL_HIGH
def_set_mod_reg_bit(ptimer, ptmr0_cur_high_val, high, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PTIMER_PTMR0_CTRL
void set_ptimer_ptmr0_ctrl(u32 reg_addr,
			u32 enable,
			u32 mode,
			u32 direction,
			u32 div,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PTIMER_PTMR0_CTRL_ENABLE
def_set_mod_reg_bit(ptimer, ptmr0_ctrl, enable, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PTIMER_PTMR0_CTRL_MODE
def_set_mod_reg_bit(ptimer, ptmr0_ctrl, mode, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PTIMER_PTMR0_CTRL_DIRECTION
def_set_mod_reg_bit(ptimer, ptmr0_ctrl, direction, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PTIMER_PTMR0_CTRL_DIV
def_set_mod_reg_bit(ptimer, ptmr0_ctrl, div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PTIMER_PTMR0_INT_EN
void set_ptimer_ptmr0_int_en(u32 reg_addr,
				u32 enable,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PTIMER_PTMR0_INT_EN_ENABLE
def_set_mod_reg_bit(ptimer, ptmr0_int_en, enable, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PTIMER_PTMR0_CLR_INT_PENDING
void set_ptimer_ptmr0_clr_int_pending(u32 reg_addr,
					u32 clr,
					u32 m_or_r);
#endif

#ifdef DEF_SET_PTIMER_PTMR0_CLR_INT_PENDING_CLR
def_set_mod_reg_bit(ptimer, ptmr0_clr_int_pending, clr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PTIMER_PTMR0_INT_PENDING
void set_ptimer_ptmr0_int_pending(u32 reg_addr,
				u32 pending,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PTIMER_PTMR0_INT_PENDING_PENDING
def_set_mod_reg_bit(ptimer, ptmr0_int_pending, pending, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PTIMER_PTMR1_STORE_LOW_VAL
void set_ptimer_ptmr1_store_low_val(u32 reg_addr,
					u32 low,
					u32 m_or_r);
#endif

#ifdef DEF_SET_PTIMER_PTMR1_STORE_LOW_VAL_LOW
def_set_mod_reg_bit(ptimer, ptmr1_store_low_val, low, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PTIMER_PTMR1_STORE_HIGH_VAL
void set_ptimer_ptmr1_store_high_val(u32 reg_addr,
					u32 high,
					u32 m_or_r);
#endif

#ifdef DEF_SET_PTIMER_PTMR1_STORE_HIGH_VAL_HIGH
def_set_mod_reg_bit(ptimer, ptmr1_store_high_val, high, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PTIMER_PTMR1_CUR_LOW_VAL
void set_ptimer_ptmr1_cur_low_val(u32 reg_addr,
				u32 low,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PTIMER_PTMR1_CUR_LOW_VAL_LOW
def_set_mod_reg_bit(ptimer, ptmr1_cur_low_val, low, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PTIMER_PTMR1_CUR_HIGH_VAL
void set_ptimer_ptmr1_cur_high_val(u32 reg_addr,
				u32 high,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PTIMER_PTMR1_CUR_HIGH_VAL_HIGH
def_set_mod_reg_bit(ptimer, ptmr1_cur_high_val, high, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PTIMER_PTMR1_CTRL
void set_ptimer_ptmr1_ctrl(u32 reg_addr,
			u32 enable,
			u32 mode,
			u32 direction,
			u32 div,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PTIMER_PTMR1_CTRL_ENABLE
def_set_mod_reg_bit(ptimer, ptmr1_ctrl, enable, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PTIMER_PTMR1_CTRL_MODE
def_set_mod_reg_bit(ptimer, ptmr1_ctrl, mode, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PTIMER_PTMR1_CTRL_DIRECTION
def_set_mod_reg_bit(ptimer, ptmr1_ctrl, direction, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PTIMER_PTMR1_CTRL_DIV
def_set_mod_reg_bit(ptimer, ptmr1_ctrl, div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PTIMER_PTMR1_INT_EN
void set_ptimer_ptmr1_int_en(u32 reg_addr,
				u32 enable,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PTIMER_PTMR1_INT_EN_ENABLE
def_set_mod_reg_bit(ptimer, ptmr1_int_en, enable, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PTIMER_PTMR1_CLR_INT_PENDING
void set_ptimer_ptmr1_clr_int_pending(u32 reg_addr,
					u32 clr,
					u32 m_or_r);
#endif

#ifdef DEF_SET_PTIMER_PTMR1_CLR_INT_PENDING_CLR
def_set_mod_reg_bit(ptimer, ptmr1_clr_int_pending, clr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PTIMER_PTMR1_INT_PENDING
void set_ptimer_ptmr1_int_pending(u32 reg_addr,
				u32 pending,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PTIMER_PTMR1_INT_PENDING_PENDING
def_set_mod_reg_bit(ptimer, ptmr1_int_pending, pending, addr, val, m_or_r);
#endif

#endif /* ___PTIMER__SET___H___ */
