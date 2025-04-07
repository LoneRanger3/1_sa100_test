/* rtc_dump.h */

#ifndef ___RTC___DUMP__H___
#define ___RTC___DUMP__H___

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

u32 dump_rtc_ver(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_rtc_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_rtc_ld_en(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_rtc_ld_int_en(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_rtc_ld_int_clr(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_rtc_ld_int_pending(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_rtc_ymd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_rtc_week(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_rtc_hms(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_rtc_alarm_int_en(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_rtc_alarm_int_clr(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_rtc_alarm_int_pending(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_rtc_alarm_match_en(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_rtc_alarm_day_match(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_rtc_alarm_week_match(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_rtc_alarm_hms_match(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_rtc_wakeup_en(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_rtc_lfeosc_fanout_cfg(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_rtc_su_en(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_rtc_core(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_rtc_top(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_rtc_hfeosc(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_rtc_lfeosc(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_rtc_alive_reg0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_rtc_alive_reg1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_rtc_alive_reg2(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_rtc_alive_reg3(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_pm_pe(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_pm_cfg(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_pm_int_en(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_pm_pend_clr(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_pm_pend(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_pm_pe1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_lock_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_deep_slp_flag(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_deep_slp_exit(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_hot_rst_flag(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_hot_rst_exit(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_pad_hold(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_sys_stat(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_ax_clamp(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_sio_func_r0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_sio_pud_r0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_sio_drv_r0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_sio_data_r0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_sio_pr_r(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_sio_int_tri_r0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_sio_int_mask0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_sio_int_pend0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rtc_sio_int_samp_ctrl0(u32 addr, u32 data, u32 mode, char *outbuf);

#endif /* ___RTC___DUMP__H___ */
