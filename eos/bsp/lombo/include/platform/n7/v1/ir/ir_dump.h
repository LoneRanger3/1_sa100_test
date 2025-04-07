/* ir_dump.h */

#ifndef ___IR___DUMP__H___
#define ___IR___DUMP__H___

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

u32 dump_ir_ver(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_ir_ftr(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_ir_ctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_ir_timeout_th(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_ir_noise_th(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_ir_int_en(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_ir_int_pd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_ir_int_clr(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_ir_data(u32 addr, u32 data, u32 mode, char *outbuf);

#endif /* ___IR___DUMP__H___ */
