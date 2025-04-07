/* gtimer_dump.h */

#ifndef ___GTIMER___DUMP__H___
#define ___GTIMER___DUMP__H___

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

u32 dump_gtimer_gtmr_ver(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gtimer_gtmr0_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gtimer_gtmr0_store_val(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gtimer_gtmr0_cur_val(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gtimer_gtmr0_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gtimer_gtmr0_int_en(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gtimer_gtmr0_clr_int_pending(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gtimer_gtmr0_int_pending(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gtimer_gtmr1_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gtimer_gtmr1_store_val(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gtimer_gtmr1_cur_val(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gtimer_gtmr1_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gtimer_gtmr1_int_en(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gtimer_gtmr1_clr_int_pending(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gtimer_gtmr1_int_pending(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gtimer_gtmr2_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gtimer_gtmr2_store_val(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gtimer_gtmr2_cur_val(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gtimer_gtmr2_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gtimer_gtmr2_int_en(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gtimer_gtmr2_clr_int_pending(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gtimer_gtmr2_int_pending(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gtimer_gtmr3_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gtimer_gtmr3_store_val(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gtimer_gtmr3_cur_val(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gtimer_gtmr3_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gtimer_gtmr3_int_en(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gtimer_gtmr3_clr_int_pending(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gtimer_gtmr3_int_pending(u32 addr, u32 data, u32 mode, char *outbuf);

#endif /* ___GTIMER___DUMP__H___ */
