/* dpu_dump.h */

#ifndef ___DPU___DUMP__H___
#define ___DPU___DUMP__H___

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

u32 dump_dpu_ver(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dpu_ftr(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dpu_ctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dpu_clk_src(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dpu_clk_div(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dpu_clk_gating(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dpu_routine(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dpu_sclk_sel(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dpu_adpll0_ctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dpu_adpll0_tune0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dpu_adpll0_tune1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dpu_adpll0_stat(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dpu_adpll1_ctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dpu_adpll1_tune0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dpu_adpll1_tune1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dpu_adpll1_stat(u32 addr, u32 data, u32 mode, char *outbuf);

#endif /* ___DPU___DUMP__H___ */
