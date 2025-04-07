/* wdog_dump.h */

#ifndef ___WDOG___DUMP__H___
#define ___WDOG___DUMP__H___

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

u32 dump_wdog_wdog_ver(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_wdog_wdog_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_wdog_wdog_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_wdog_wdog_tmrperiod(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_wdog_wdog_cnt_cur_val(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_wdog_wdog_restart(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_wdog_wdog_int_en(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_wdog_wdog_clr_int_pending(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_wdog_wdog_int_pending(u32 addr, u32 data, u32 mode, char *outbuf);

#endif /* ___WDOG___DUMP__H___ */
