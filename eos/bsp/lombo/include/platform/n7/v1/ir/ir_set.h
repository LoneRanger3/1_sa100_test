/* ir_set.h */

#ifndef ___IR__SET___H___
#define ___IR__SET___H___

#ifdef DEF_SET_IR_VER
void set_ir_ver(u32 reg_addr,
		u32 ver_l,
		u32 ver_h,
		u32 comp,
		u32 m_or_r);
#endif

#ifdef DEF_SET_IR_VER_VER_L
def_set_mod_reg_bit(ir, ver, ver_l, addr, val, m_or_r);
#endif

#ifdef DEF_SET_IR_VER_VER_H
def_set_mod_reg_bit(ir, ver, ver_h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_IR_VER_COMP
def_set_mod_reg_bit(ir, ver, comp, addr, val, m_or_r);
#endif

#ifdef DEF_SET_IR_FTR
void set_ir_ftr(u32 reg_addr,
		u32 nec_pro,
		u32 sirc_pro,
		u32 sirc_ext_15,
		u32 sirc_ext_20,
		u32 rc5_pro,
		u32 rc6_pro,
		u32 ip_ver_l,
		u32 ip_ver_h,
		u32 m_or_r);
#endif

#ifdef DEF_SET_IR_FTR_NEC_PRO
def_set_mod_reg_bit(ir, ftr, nec_pro, addr, val, m_or_r);
#endif

#ifdef DEF_SET_IR_FTR_SIRC_PRO
def_set_mod_reg_bit(ir, ftr, sirc_pro, addr, val, m_or_r);
#endif

#ifdef DEF_SET_IR_FTR_SIRC_EXT_15
def_set_mod_reg_bit(ir, ftr, sirc_ext_15, addr, val, m_or_r);
#endif

#ifdef DEF_SET_IR_FTR_SIRC_EXT_20
def_set_mod_reg_bit(ir, ftr, sirc_ext_20, addr, val, m_or_r);
#endif

#ifdef DEF_SET_IR_FTR_RC5_PRO
def_set_mod_reg_bit(ir, ftr, rc5_pro, addr, val, m_or_r);
#endif

#ifdef DEF_SET_IR_FTR_RC6_PRO
def_set_mod_reg_bit(ir, ftr, rc6_pro, addr, val, m_or_r);
#endif

#ifdef DEF_SET_IR_FTR_IP_VER_L
def_set_mod_reg_bit(ir, ftr, ip_ver_l, addr, val, m_or_r);
#endif

#ifdef DEF_SET_IR_FTR_IP_VER_H
def_set_mod_reg_bit(ir, ftr, ip_ver_h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_IR_CTL
void set_ir_ctl(u32 reg_addr,
		u32 clk_div,
		u32 sirc_ext,
		u32 protocol_sel,
		u32 ir_invert,
		u32 ir_en,
		u32 m_or_r);
#endif

#ifdef DEF_SET_IR_CTL_CLK_DIV
def_set_mod_reg_bit(ir, ctl, clk_div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_IR_CTL_SIRC_EXT
def_set_mod_reg_bit(ir, ctl, sirc_ext, addr, val, m_or_r);
#endif

#ifdef DEF_SET_IR_CTL_PROTOCOL_SEL
def_set_mod_reg_bit(ir, ctl, protocol_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_IR_CTL_IR_INVERT
def_set_mod_reg_bit(ir, ctl, ir_invert, addr, val, m_or_r);
#endif

#ifdef DEF_SET_IR_CTL_IR_EN
def_set_mod_reg_bit(ir, ctl, ir_en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_IR_TIMEOUT_TH
void set_ir_timeout_th(u32 reg_addr,
			u32 thd,
			u32 m_or_r);
#endif

#ifdef DEF_SET_IR_TIMEOUT_TH_THD
def_set_mod_reg_bit(ir, timeout_th, thd, addr, val, m_or_r);
#endif

#ifdef DEF_SET_IR_NOISE_TH
void set_ir_noise_th(u32 reg_addr,
			u32 thd,
			u32 m_or_r);
#endif

#ifdef DEF_SET_IR_NOISE_TH_THD
def_set_mod_reg_bit(ir, noise_th, thd, addr, val, m_or_r);
#endif

#ifdef DEF_SET_IR_INT_EN
void set_ir_int_en(u32 reg_addr,
		u32 ir_start,
		u32 ir_frame,
		u32 ir_repeat,
		u32 ir_f_err,
		u32 ir_to,
		u32 m_or_r);
#endif

#ifdef DEF_SET_IR_INT_EN_IR_START
def_set_mod_reg_bit(ir, int_en, ir_start, addr, val, m_or_r);
#endif

#ifdef DEF_SET_IR_INT_EN_IR_FRAME
def_set_mod_reg_bit(ir, int_en, ir_frame, addr, val, m_or_r);
#endif

#ifdef DEF_SET_IR_INT_EN_IR_REPEAT
def_set_mod_reg_bit(ir, int_en, ir_repeat, addr, val, m_or_r);
#endif

#ifdef DEF_SET_IR_INT_EN_IR_F_ERR
def_set_mod_reg_bit(ir, int_en, ir_f_err, addr, val, m_or_r);
#endif

#ifdef DEF_SET_IR_INT_EN_IR_TO
def_set_mod_reg_bit(ir, int_en, ir_to, addr, val, m_or_r);
#endif

#ifdef DEF_SET_IR_INT_PD
void set_ir_int_pd(u32 reg_addr,
		u32 ir_start,
		u32 ir_frame,
		u32 ir_repeat,
		u32 ir_f_err,
		u32 ir_to,
		u32 m_or_r);
#endif

#ifdef DEF_SET_IR_INT_PD_IR_START
def_set_mod_reg_bit(ir, int_pd, ir_start, addr, val, m_or_r);
#endif

#ifdef DEF_SET_IR_INT_PD_IR_FRAME
def_set_mod_reg_bit(ir, int_pd, ir_frame, addr, val, m_or_r);
#endif

#ifdef DEF_SET_IR_INT_PD_IR_REPEAT
def_set_mod_reg_bit(ir, int_pd, ir_repeat, addr, val, m_or_r);
#endif

#ifdef DEF_SET_IR_INT_PD_IR_F_ERR
def_set_mod_reg_bit(ir, int_pd, ir_f_err, addr, val, m_or_r);
#endif

#ifdef DEF_SET_IR_INT_PD_IR_TO
def_set_mod_reg_bit(ir, int_pd, ir_to, addr, val, m_or_r);
#endif

#ifdef DEF_SET_IR_INT_CLR
void set_ir_int_clr(u32 reg_addr,
			u32 ir_start,
			u32 ir_frame,
			u32 ir_repeat,
			u32 ir_f_err,
			u32 ir_to,
			u32 m_or_r);
#endif

#ifdef DEF_SET_IR_INT_CLR_IR_START
def_set_mod_reg_bit(ir, int_clr, ir_start, addr, val, m_or_r);
#endif

#ifdef DEF_SET_IR_INT_CLR_IR_FRAME
def_set_mod_reg_bit(ir, int_clr, ir_frame, addr, val, m_or_r);
#endif

#ifdef DEF_SET_IR_INT_CLR_IR_REPEAT
def_set_mod_reg_bit(ir, int_clr, ir_repeat, addr, val, m_or_r);
#endif

#ifdef DEF_SET_IR_INT_CLR_IR_F_ERR
def_set_mod_reg_bit(ir, int_clr, ir_f_err, addr, val, m_or_r);
#endif

#ifdef DEF_SET_IR_INT_CLR_IR_TO
def_set_mod_reg_bit(ir, int_clr, ir_to, addr, val, m_or_r);
#endif

#ifdef DEF_SET_IR_DATA
void set_ir_data(u32 reg_addr,
		u32 data,
		u32 m_or_r);
#endif

#ifdef DEF_SET_IR_DATA_DATA
def_set_mod_reg_bit(ir, data, data, addr, val, m_or_r);
#endif

#endif /* ___IR__SET___H___ */
