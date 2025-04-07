/* dc_dump.h */

#ifndef ___DC___DUMP__H___
#define ___DC___DUMP__H___

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

u32 dump_dc_gnectl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_imgsrcctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_scrsiz(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_bkcolor(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ver(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ftr(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_win0ctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_win1ctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_win2ctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_win3ctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_win0buffmt(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_win1buffmt(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_win2buffmt(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_win3buffmt(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_win0siz(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_win1siz(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_win2siz(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_win3siz(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_win0coor(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_win1coor(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_win2coor(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_win3coor(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_win0bufladd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_win1bufladd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_win2bufladd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_win3bufladd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_winbufhadd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_win0buflnstd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_win1buflnstd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_win2buflnstd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_win3buflnstd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_bld0ctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_bld1ctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_bld2ctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_bld3ctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_bld4ctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_bld0ckmin(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_bld1ckmin(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_bld2ckmin(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_bld3ckmin(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_bld4ckmin(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_bld0ckmax(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_bld1ckmax(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_bld2ckmax(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_bld3ckmax(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_bld4ckmax(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_hwcctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_hwccoor(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ceen(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_cerrcoef(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_cergcoef(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_cerbcoef(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_cercons(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_cegrcoef(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ceggcoef(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_cegbcoef(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_cegcons(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_cebrcoef(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_cebgcoef(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_cebbcoef(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_cebcons(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_blkgne(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_wbc(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_wbadd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_wblnstd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch0_blkctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch1_blkctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch2_blkctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch3_blkctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch4_blkctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch5_blkctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch6_blkctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch7_blkctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch8_blkctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch9_blkctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch10_blkctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch11_blkctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch12_blkctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch13_blkctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch14_blkctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch15_blkctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch0_blkcoor(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch1_blkcoor(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch2_blkcoor(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch3_blkcoor(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch4_blkcoor(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch5_blkcoor(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch6_blkcoor(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch7_blkcoor(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch8_blkcoor(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch9_blkcoor(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch10_blkcoor(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch11_blkcoor(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch12_blkcoor(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch13_blkcoor(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch14_blkcoor(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch15_blkcoor(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch0_blkbufadd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch1_blkbufadd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch2_blkbufadd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch3_blkbufadd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch4_blkbufadd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch5_blkbufadd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch6_blkbufadd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch7_blkbufadd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch8_blkbufadd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch9_blkbufadd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch10_blkbufadd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch11_blkbufadd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch12_blkbufadd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch13_blkbufadd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch14_blkbufadd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch15_blkbufadd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch0_blkbuflnstd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch1_blkbuflnstd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch2_blkbuflnstd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch3_blkbuflnstd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch4_blkbuflnstd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch5_blkbuflnstd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch6_blkbuflnstd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch7_blkbuflnstd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch8_blkbuflnstd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch9_blkbuflnstd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch10_blkbuflnstd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch11_blkbuflnstd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch12_blkbuflnstd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch13_blkbuflnstd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch14_blkbuflnstd(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_dc_ch15_blkbuflnstd(u32 addr, u32 data, u32 mode, char *outbuf);

#endif /* ___DC___DUMP__H___ */
