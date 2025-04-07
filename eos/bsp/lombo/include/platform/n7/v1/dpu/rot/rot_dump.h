/* rot_dump.h */

#ifndef ___ROT___DUMP__H___
#define ___ROT___DUMP__H___

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

u32 dump_rot_ver(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rot_ftr(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rot_ctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rot_cfg(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rot_strmctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rot_sta(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rot_inten(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rot_intpd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rot_intclr(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rot_incfg(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rot_insize0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rot_inadd0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rot_inlnstd0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rot_insize1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rot_inadd1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rot_inlnstd1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rot_insize2(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rot_inadd2(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rot_inlnstd2(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rot_outcfg(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rot_outsize0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rot_outadd0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rot_outlnstd0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rot_outsize1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rot_outadd1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rot_outlnstd1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rot_outsize2(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rot_outadd2(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rot_outlnstd2(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rot_dmasctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rot_dmadctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rot_dmasrccfg0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rot_dmasrccfg1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rot_dmadstcfg0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rot_dmadstcfg1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_rot_dbg0(u32 addr, u32 data, u32 mode, char *outbuf);

#endif /* ___ROT___DUMP__H___ */
