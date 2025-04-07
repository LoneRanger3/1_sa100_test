/* wdog_set.h */

#ifndef ___WDOG__SET___H___
#define ___WDOG__SET___H___

#ifdef DEF_SET_WDOG_WDOG_VER
void set_wdog_wdog_ver(u32 reg_addr,
			u32 ver_l,
			u32 ver_h,
			u32 comp,
			u32 m_or_r);
#endif

#ifdef DEF_SET_WDOG_WDOG_VER_VER_L
def_set_mod_reg_bit(wdog, wdog_ver, ver_l, addr, val, m_or_r);
#endif

#ifdef DEF_SET_WDOG_WDOG_VER_VER_H
def_set_mod_reg_bit(wdog, wdog_ver, ver_h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_WDOG_WDOG_VER_COMP
def_set_mod_reg_bit(wdog, wdog_ver, comp, addr, val, m_or_r);
#endif

#ifdef DEF_SET_WDOG_WDOG_CLK_CTRL
void set_wdog_wdog_clk_ctrl(u32 reg_addr,
				u32 sel,
				u32 m_or_r);
#endif

#ifdef DEF_SET_WDOG_WDOG_CLK_CTRL_SEL
def_set_mod_reg_bit(wdog, wdog_clk_ctrl, sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_WDOG_WDOG_CTRL
void set_wdog_wdog_ctrl(u32 reg_addr,
			u32 en,
			u32 rmod,
			u32 rpl,
			u32 su,
			u32 m_or_r);
#endif

#ifdef DEF_SET_WDOG_WDOG_CTRL_EN
def_set_mod_reg_bit(wdog, wdog_ctrl, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_WDOG_WDOG_CTRL_RMOD
def_set_mod_reg_bit(wdog, wdog_ctrl, rmod, addr, val, m_or_r);
#endif

#ifdef DEF_SET_WDOG_WDOG_CTRL_RPL
def_set_mod_reg_bit(wdog, wdog_ctrl, rpl, addr, val, m_or_r);
#endif

#ifdef DEF_SET_WDOG_WDOG_CTRL_SU
def_set_mod_reg_bit(wdog, wdog_ctrl, su, addr, val, m_or_r);
#endif

#ifdef DEF_SET_WDOG_WDOG_TMRPERIOD
void set_wdog_wdog_tmrperiod(u32 reg_addr,
				u32 tmrperiod,
				u32 m_or_r);
#endif

#ifdef DEF_SET_WDOG_WDOG_TMRPERIOD_TMRPERIOD
def_set_mod_reg_bit(wdog, wdog_tmrperiod, tmrperiod, addr, val, m_or_r);
#endif

#ifdef DEF_SET_WDOG_WDOG_CNT_CUR_VAL
void set_wdog_wdog_cnt_cur_val(u32 reg_addr,
				u32 cnt_cur_val,
				u32 m_or_r);
#endif

#ifdef DEF_SET_WDOG_WDOG_CNT_CUR_VAL_CNT_CUR_VAL
def_set_mod_reg_bit(wdog, wdog_cnt_cur_val, cnt_cur_val, addr, val, m_or_r);
#endif

#ifdef DEF_SET_WDOG_WDOG_RESTART
void set_wdog_wdog_restart(u32 reg_addr,
			u32 restart,
			u32 m_or_r);
#endif

#ifdef DEF_SET_WDOG_WDOG_RESTART_RESTART
def_set_mod_reg_bit(wdog, wdog_restart, restart, addr, val, m_or_r);
#endif

#ifdef DEF_SET_WDOG_WDOG_INT_EN
void set_wdog_wdog_int_en(u32 reg_addr,
			u32 enable,
			u32 m_or_r);
#endif

#ifdef DEF_SET_WDOG_WDOG_INT_EN_ENABLE
def_set_mod_reg_bit(wdog, wdog_int_en, enable, addr, val, m_or_r);
#endif

#ifdef DEF_SET_WDOG_WDOG_CLR_INT_PENDING
void set_wdog_wdog_clr_int_pending(u32 reg_addr,
				u32 clr,
				u32 m_or_r);
#endif

#ifdef DEF_SET_WDOG_WDOG_CLR_INT_PENDING_CLR
def_set_mod_reg_bit(wdog, wdog_clr_int_pending, clr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_WDOG_WDOG_INT_PENDING
void set_wdog_wdog_int_pending(u32 reg_addr,
				u32 pending,
				u32 m_or_r);
#endif

#ifdef DEF_SET_WDOG_WDOG_INT_PENDING_PENDING
def_set_mod_reg_bit(wdog, wdog_int_pending, pending, addr, val, m_or_r);
#endif

#endif /* ___WDOG__SET___H___ */
