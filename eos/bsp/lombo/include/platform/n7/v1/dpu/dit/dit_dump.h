/* dit_dump.h */

#ifndef ___DIT___DUMP__H___
#define ___DIT___DUMP__H___

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

u32 dump_dit_ver(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dit_ftr(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dit_sta(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dit_ctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dit_cfg(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dit_wbctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dit_wbtmr(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dit_inaddr0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dit_inaddr1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dit_inaddr2(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dit_inaddr3(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dit_outaddr(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dit_insize(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dit_outsize(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dit_lstr(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dit_wbintctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dit_wbintsta(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dit_wbintclr(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dit_sthr(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dit_ithr(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dit_spcnt(u32 addr, u32 data, u32 mode, char *outbuf);

#endif /* ___DIT___DUMP__H___ */
