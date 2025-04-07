/* pwm_dump.h */

#ifndef ___PWM___DUMP__H___
#define ___PWM___DUMP__H___

#ifndef TMP_STR_LEN
#define TMP_STR_LEN  256
#endif

#ifndef REG_INFO_DEF
#define REG_INFO_DEF

typedef u32 (*pfn_dump)(u32 addr, u32 data, u32 mode, char *buffer);

typedef struct tag_reg_info {
	u32      addr;           /*address         */
	u32      reset;          /*reset value     */
	pfn_dump dump;           /*reg dump func   */
	char     name[28];       /*reg name        */
	u32      res;            /*0               */
} reg_info_t;

#endif /* REG_INFO_DEF */

u32 dump_pwm_ver(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm_ce(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm_pe(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm_int_en(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm_clr_int_pending(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm_int_pending(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm0_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm1_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm2_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm3_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm4_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm5_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm6_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm7_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm8_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm9_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm0_cnt_store(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm1_cnt_store(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm2_cnt_store(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm3_cnt_store(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm4_cnt_store(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm5_cnt_store(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm6_cnt_store(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm7_cnt_store(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm8_cnt_store(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm9_cnt_store(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm0_cnt_cmp(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm1_cnt_cmp(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm2_cnt_cmp(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm3_cnt_cmp(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm4_cnt_cmp(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm5_cnt_cmp(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm6_cnt_cmp(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm7_cnt_cmp(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm8_cnt_cmp(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm9_cnt_cmp(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm0_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm1_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm2_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm3_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm4_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm5_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm6_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm7_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm8_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm9_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm0_cnt_cap(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm1_cnt_cap(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm2_cnt_cap(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm3_cnt_cap(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm4_cnt_cap(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm5_cnt_cap(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm6_cnt_cap(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm7_cnt_cap(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm8_cnt_cap(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm9_cnt_cap(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm0_in_sts(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm1_in_sts(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm2_in_sts(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm3_in_sts(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm4_in_sts(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm5_in_sts(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm6_in_sts(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm7_in_sts(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm8_in_sts(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm9_in_sts(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm0_dly_cnt(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm1_dly_cnt(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm2_dly_cnt(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm3_dly_cnt(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm4_dly_cnt(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm5_dly_cnt(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm6_dly_cnt(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm7_dly_cnt(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm8_dly_cnt(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm9_dly_cnt(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm0_pulse_cnt(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm1_pulse_cnt(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm2_pulse_cnt(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm3_pulse_cnt(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm4_pulse_cnt(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm5_pulse_cnt(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm6_pulse_cnt(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm7_pulse_cnt(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm8_pulse_cnt(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_pwm_pwm9_pulse_cnt(u32 addr, u32 data, u32 mode, char *outbuf);

#endif /* ___PWM___DUMP__H___ */
