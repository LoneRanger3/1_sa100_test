/* cpu_ctrl_dump.h */

#ifndef ___CPU_CTRL___DUMP__H___
#define ___CPU_CTRL___DUMP__H___

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

u32 dump_cpu_ctrl_rst_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_cpu_ctrl_ctrl_r(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_cpu_ctrl_cfg_r(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_cpu_ctrl_status_r(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_cpu_ctrl_gener_reg(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_cpu_ctrl_cpu_clk_sel(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_cpu_ctrl_adpll_fac(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_cpu_ctrl_adpll_tune0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_cpu_ctrl_adpll_tune1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_cpu_ctrl_adpll_stat(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_cpu_ctrl_cache_cfg0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_cpu_ctrl_cache_cfg1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_cpu_ctrl_dbg_reg0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_cpu_ctrl_ver(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_cpu_ctrl_c_rst_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

#endif /* ___CPU_CTRL___DUMP__H___ */
