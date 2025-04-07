/* gtimer_set.h */

#ifndef ___GTIMER__SET___H___
#define ___GTIMER__SET___H___

#ifdef DEF_SET_GTIMER_GTMR_VER
void set_gtimer_gtmr_ver(u32 reg_addr,
			u32 ver_l,
			u32 ver_h,
			u32 comp,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR_VER_VER_L
def_set_mod_reg_bit(gtimer, gtmr_ver, ver_l, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR_VER_VER_H
def_set_mod_reg_bit(gtimer, gtmr_ver, ver_h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR_VER_COMP
def_set_mod_reg_bit(gtimer, gtmr_ver, comp, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR0_CLK_CTRL
void set_gtimer_gtmr0_clk_ctrl(u32 reg_addr,
				u32 clk_div,
				u32 clk_sel,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR0_CLK_CTRL_CLK_DIV
def_set_mod_reg_bit(gtimer, gtmr0_clk_ctrl, clk_div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR0_CLK_CTRL_CLK_SEL
def_set_mod_reg_bit(gtimer, gtmr0_clk_ctrl, clk_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR0_STORE_VAL
void set_gtimer_gtmr0_store_val(u32 reg_addr,
				u32 store_val,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR0_STORE_VAL_STORE_VAL
def_set_mod_reg_bit(gtimer, gtmr0_store_val, store_val, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR0_CUR_VAL
void set_gtimer_gtmr0_cur_val(u32 reg_addr,
				u32 cur_val,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR0_CUR_VAL_CUR_VAL
def_set_mod_reg_bit(gtimer, gtmr0_cur_val, cur_val, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR0_CTRL
void set_gtimer_gtmr0_ctrl(u32 reg_addr,
			u32 enable,
			u32 mode,
			u32 direction,
			u32 nrdy,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR0_CTRL_ENABLE
def_set_mod_reg_bit(gtimer, gtmr0_ctrl, enable, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR0_CTRL_MODE
def_set_mod_reg_bit(gtimer, gtmr0_ctrl, mode, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR0_CTRL_DIRECTION
def_set_mod_reg_bit(gtimer, gtmr0_ctrl, direction, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR0_CTRL_NRDY
def_set_mod_reg_bit(gtimer, gtmr0_ctrl, nrdy, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR0_INT_EN
void set_gtimer_gtmr0_int_en(u32 reg_addr,
				u32 enable,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR0_INT_EN_ENABLE
def_set_mod_reg_bit(gtimer, gtmr0_int_en, enable, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR0_CLR_INT_PENDING
void set_gtimer_gtmr0_clr_int_pending(u32 reg_addr,
					u32 clr,
					u32 m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR0_CLR_INT_PENDING_CLR
def_set_mod_reg_bit(gtimer, gtmr0_clr_int_pending, clr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR0_INT_PENDING
void set_gtimer_gtmr0_int_pending(u32 reg_addr,
				u32 pending,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR0_INT_PENDING_PENDING
def_set_mod_reg_bit(gtimer, gtmr0_int_pending, pending, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR1_CLK_CTRL
void set_gtimer_gtmr1_clk_ctrl(u32 reg_addr,
				u32 clk_div,
				u32 clk_sel,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR1_CLK_CTRL_CLK_DIV
def_set_mod_reg_bit(gtimer, gtmr1_clk_ctrl, clk_div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR1_CLK_CTRL_CLK_SEL
def_set_mod_reg_bit(gtimer, gtmr1_clk_ctrl, clk_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR1_STORE_VAL
void set_gtimer_gtmr1_store_val(u32 reg_addr,
				u32 store_val,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR1_STORE_VAL_STORE_VAL
def_set_mod_reg_bit(gtimer, gtmr1_store_val, store_val, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR1_CUR_VAL
void set_gtimer_gtmr1_cur_val(u32 reg_addr,
				u32 cur_val,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR1_CUR_VAL_CUR_VAL
def_set_mod_reg_bit(gtimer, gtmr1_cur_val, cur_val, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR1_CTRL
void set_gtimer_gtmr1_ctrl(u32 reg_addr,
			u32 enable,
			u32 mode,
			u32 direction,
			u32 nrdy,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR1_CTRL_ENABLE
def_set_mod_reg_bit(gtimer, gtmr1_ctrl, enable, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR1_CTRL_MODE
def_set_mod_reg_bit(gtimer, gtmr1_ctrl, mode, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR1_CTRL_DIRECTION
def_set_mod_reg_bit(gtimer, gtmr1_ctrl, direction, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR1_CTRL_NRDY
def_set_mod_reg_bit(gtimer, gtmr1_ctrl, nrdy, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR1_INT_EN
void set_gtimer_gtmr1_int_en(u32 reg_addr,
				u32 enable,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR1_INT_EN_ENABLE
def_set_mod_reg_bit(gtimer, gtmr1_int_en, enable, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR1_CLR_INT_PENDING
void set_gtimer_gtmr1_clr_int_pending(u32 reg_addr,
					u32 clr,
					u32 m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR1_CLR_INT_PENDING_CLR
def_set_mod_reg_bit(gtimer, gtmr1_clr_int_pending, clr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR1_INT_PENDING
void set_gtimer_gtmr1_int_pending(u32 reg_addr,
				u32 pending,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR1_INT_PENDING_PENDING
def_set_mod_reg_bit(gtimer, gtmr1_int_pending, pending, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR2_CLK_CTRL
void set_gtimer_gtmr2_clk_ctrl(u32 reg_addr,
				u32 clk_div,
				u32 clk_sel,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR2_CLK_CTRL_CLK_DIV
def_set_mod_reg_bit(gtimer, gtmr2_clk_ctrl, clk_div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR2_CLK_CTRL_CLK_SEL
def_set_mod_reg_bit(gtimer, gtmr2_clk_ctrl, clk_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR2_STORE_VAL
void set_gtimer_gtmr2_store_val(u32 reg_addr,
				u32 store_val,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR2_STORE_VAL_STORE_VAL
def_set_mod_reg_bit(gtimer, gtmr2_store_val, store_val, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR2_CUR_VAL
void set_gtimer_gtmr2_cur_val(u32 reg_addr,
				u32 cur_val,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR2_CUR_VAL_CUR_VAL
def_set_mod_reg_bit(gtimer, gtmr2_cur_val, cur_val, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR2_CTRL
void set_gtimer_gtmr2_ctrl(u32 reg_addr,
			u32 enable,
			u32 mode,
			u32 direction,
			u32 nrdy,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR2_CTRL_ENABLE
def_set_mod_reg_bit(gtimer, gtmr2_ctrl, enable, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR2_CTRL_MODE
def_set_mod_reg_bit(gtimer, gtmr2_ctrl, mode, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR2_CTRL_DIRECTION
def_set_mod_reg_bit(gtimer, gtmr2_ctrl, direction, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR2_CTRL_NRDY
def_set_mod_reg_bit(gtimer, gtmr2_ctrl, nrdy, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR2_INT_EN
void set_gtimer_gtmr2_int_en(u32 reg_addr,
				u32 enable,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR2_INT_EN_ENABLE
def_set_mod_reg_bit(gtimer, gtmr2_int_en, enable, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR2_CLR_INT_PENDING
void set_gtimer_gtmr2_clr_int_pending(u32 reg_addr,
					u32 clr,
					u32 m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR2_CLR_INT_PENDING_CLR
def_set_mod_reg_bit(gtimer, gtmr2_clr_int_pending, clr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR2_INT_PENDING
void set_gtimer_gtmr2_int_pending(u32 reg_addr,
				u32 pending,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR2_INT_PENDING_PENDING
def_set_mod_reg_bit(gtimer, gtmr2_int_pending, pending, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR3_CLK_CTRL
void set_gtimer_gtmr3_clk_ctrl(u32 reg_addr,
				u32 clk_div,
				u32 clk_sel,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR3_CLK_CTRL_CLK_DIV
def_set_mod_reg_bit(gtimer, gtmr3_clk_ctrl, clk_div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR3_CLK_CTRL_CLK_SEL
def_set_mod_reg_bit(gtimer, gtmr3_clk_ctrl, clk_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR3_STORE_VAL
void set_gtimer_gtmr3_store_val(u32 reg_addr,
				u32 store_val,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR3_STORE_VAL_STORE_VAL
def_set_mod_reg_bit(gtimer, gtmr3_store_val, store_val, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR3_CUR_VAL
void set_gtimer_gtmr3_cur_val(u32 reg_addr,
				u32 cur_val,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR3_CUR_VAL_CUR_VAL
def_set_mod_reg_bit(gtimer, gtmr3_cur_val, cur_val, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR3_CTRL
void set_gtimer_gtmr3_ctrl(u32 reg_addr,
			u32 enable,
			u32 mode,
			u32 direction,
			u32 nrdy,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR3_CTRL_ENABLE
def_set_mod_reg_bit(gtimer, gtmr3_ctrl, enable, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR3_CTRL_MODE
def_set_mod_reg_bit(gtimer, gtmr3_ctrl, mode, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR3_CTRL_DIRECTION
def_set_mod_reg_bit(gtimer, gtmr3_ctrl, direction, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR3_CTRL_NRDY
def_set_mod_reg_bit(gtimer, gtmr3_ctrl, nrdy, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR3_INT_EN
void set_gtimer_gtmr3_int_en(u32 reg_addr,
				u32 enable,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR3_INT_EN_ENABLE
def_set_mod_reg_bit(gtimer, gtmr3_int_en, enable, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR3_CLR_INT_PENDING
void set_gtimer_gtmr3_clr_int_pending(u32 reg_addr,
					u32 clr,
					u32 m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR3_CLR_INT_PENDING_CLR
def_set_mod_reg_bit(gtimer, gtmr3_clr_int_pending, clr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR3_INT_PENDING
void set_gtimer_gtmr3_int_pending(u32 reg_addr,
				u32 pending,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GTIMER_GTMR3_INT_PENDING_PENDING
def_set_mod_reg_bit(gtimer, gtmr3_int_pending, pending, addr, val, m_or_r);
#endif

#endif /* ___GTIMER__SET___H___ */
