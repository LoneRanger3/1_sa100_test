/* ir_get.h */

#ifndef ___IR__GET___H___
#define ___IR__GET___H___

#ifdef DEF_GET_IR_VER_VER_L
def_get_mod_reg_bit(ir, ver, ver_l, addr);
#endif

#ifdef DEF_GET_IR_VER_VER_H
def_get_mod_reg_bit(ir, ver, ver_h, addr);
#endif

#ifdef DEF_GET_IR_VER_COMP
def_get_mod_reg_bit(ir, ver, comp, addr);
#endif

#ifdef DEF_GET_IR_FTR_NEC_PRO
def_get_mod_reg_bit(ir, ftr, nec_pro, addr);
#endif

#ifdef DEF_GET_IR_FTR_SIRC_PRO
def_get_mod_reg_bit(ir, ftr, sirc_pro, addr);
#endif

#ifdef DEF_GET_IR_FTR_SIRC_EXT_15
def_get_mod_reg_bit(ir, ftr, sirc_ext_15, addr);
#endif

#ifdef DEF_GET_IR_FTR_SIRC_EXT_20
def_get_mod_reg_bit(ir, ftr, sirc_ext_20, addr);
#endif

#ifdef DEF_GET_IR_FTR_RC5_PRO
def_get_mod_reg_bit(ir, ftr, rc5_pro, addr);
#endif

#ifdef DEF_GET_IR_FTR_RC6_PRO
def_get_mod_reg_bit(ir, ftr, rc6_pro, addr);
#endif

#ifdef DEF_GET_IR_FTR_IP_VER_L
def_get_mod_reg_bit(ir, ftr, ip_ver_l, addr);
#endif

#ifdef DEF_GET_IR_FTR_IP_VER_H
def_get_mod_reg_bit(ir, ftr, ip_ver_h, addr);
#endif

#ifdef DEF_GET_IR_CTL_CLK_DIV
def_get_mod_reg_bit(ir, ctl, clk_div, addr);
#endif

#ifdef DEF_GET_IR_CTL_SIRC_EXT
def_get_mod_reg_bit(ir, ctl, sirc_ext, addr);
#endif

#ifdef DEF_GET_IR_CTL_PROTOCOL_SEL
def_get_mod_reg_bit(ir, ctl, protocol_sel, addr);
#endif

#ifdef DEF_GET_IR_CTL_IR_INVERT
def_get_mod_reg_bit(ir, ctl, ir_invert, addr);
#endif

#ifdef DEF_GET_IR_CTL_IR_EN
def_get_mod_reg_bit(ir, ctl, ir_en, addr);
#endif

#ifdef DEF_GET_IR_TIMEOUT_TH_THD
def_get_mod_reg_bit(ir, timeout_th, thd, addr);
#endif

#ifdef DEF_GET_IR_NOISE_TH_THD
def_get_mod_reg_bit(ir, noise_th, thd, addr);
#endif

#ifdef DEF_GET_IR_INT_EN_IR_START
def_get_mod_reg_bit(ir, int_en, ir_start, addr);
#endif

#ifdef DEF_GET_IR_INT_EN_IR_FRAME
def_get_mod_reg_bit(ir, int_en, ir_frame, addr);
#endif

#ifdef DEF_GET_IR_INT_EN_IR_REPEAT
def_get_mod_reg_bit(ir, int_en, ir_repeat, addr);
#endif

#ifdef DEF_GET_IR_INT_EN_IR_F_ERR
def_get_mod_reg_bit(ir, int_en, ir_f_err, addr);
#endif

#ifdef DEF_GET_IR_INT_EN_IR_TO
def_get_mod_reg_bit(ir, int_en, ir_to, addr);
#endif

#ifdef DEF_GET_IR_INT_PD_IR_START
def_get_mod_reg_bit(ir, int_pd, ir_start, addr);
#endif

#ifdef DEF_GET_IR_INT_PD_IR_FRAME
def_get_mod_reg_bit(ir, int_pd, ir_frame, addr);
#endif

#ifdef DEF_GET_IR_INT_PD_IR_REPEAT
def_get_mod_reg_bit(ir, int_pd, ir_repeat, addr);
#endif

#ifdef DEF_GET_IR_INT_PD_IR_F_ERR
def_get_mod_reg_bit(ir, int_pd, ir_f_err, addr);
#endif

#ifdef DEF_GET_IR_INT_PD_IR_TO
def_get_mod_reg_bit(ir, int_pd, ir_to, addr);
#endif

#ifdef DEF_GET_IR_INT_CLR_IR_START
def_get_mod_reg_bit(ir, int_clr, ir_start, addr);
#endif

#ifdef DEF_GET_IR_INT_CLR_IR_FRAME
def_get_mod_reg_bit(ir, int_clr, ir_frame, addr);
#endif

#ifdef DEF_GET_IR_INT_CLR_IR_REPEAT
def_get_mod_reg_bit(ir, int_clr, ir_repeat, addr);
#endif

#ifdef DEF_GET_IR_INT_CLR_IR_F_ERR
def_get_mod_reg_bit(ir, int_clr, ir_f_err, addr);
#endif

#ifdef DEF_GET_IR_INT_CLR_IR_TO
def_get_mod_reg_bit(ir, int_clr, ir_to, addr);
#endif

#ifdef DEF_GET_IR_DATA_DATA
def_get_mod_reg_bit(ir, data, data, addr);
#endif

#endif /* ___IR__GET___H___ */
