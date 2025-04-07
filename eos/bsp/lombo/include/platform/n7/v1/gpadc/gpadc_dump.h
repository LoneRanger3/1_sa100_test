/* gpadc_dump.h */

#ifndef ___GPADC___DUMP__H___
#define ___GPADC___DUMP__H___

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

u32 dump_gpadc_ver(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpadc_gpaen(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpadc_gpac(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpadc_gpaie(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpadc_gpais(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpadc_gpaic(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpadc_cdat(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpadc_ain0dat(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpadc_aux0hdat(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpadc_aux0tdat(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpadc_ain1dat(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpadc_aux1hdat(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpadc_aux1tdat(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpadc_gsc(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpadc_s0cdat(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpadc_s1cdat(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpadc_s2cdat(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpadc_s0dat(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpadc_s0utdat(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpadc_s0dtdat(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpadc_s1dat(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpadc_s1utdat(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpadc_s1dtdat(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpadc_s2dat(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpadc_s2utdat(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpadc_s2dtdat(u32 addr, u32 data, u32 mode, char *outbuf);

#endif /* ___GPADC___DUMP__H___ */
