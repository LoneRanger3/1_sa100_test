/* gicc_dump.h */

#ifndef ___GICC___DUMP__H___
#define ___GICC___DUMP__H___

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

u32 dump_gicc_ctlr(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicc_pmr(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicc_bpr(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicc_iar(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicc_eoir(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicc_rpr(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicc_hppir(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicc_abpr(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicc_aiar(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicc_aeoir(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicc_ahppir(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicc_dir(u32 addr, u32 data, u32 mode, char *outbuf);

#endif /* ___GICC___DUMP__H___ */
