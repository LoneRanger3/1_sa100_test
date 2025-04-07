/* se_dump.h */

#ifndef ___SE___DUMP__H___
#define ___SE___DUMP__H___

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

u32 dump_se0_ver(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_ftr(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_csta(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_sta(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_ctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_cfg0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_cfg1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_cfg2(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_intctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_intsta(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_intclr(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_lncnt(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_updctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_ctabswt(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_strmctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_inaddr0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_inaddr1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_inaddr2(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_insize0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_insize1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_rsmpsize(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_inlstr0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_inlstr1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_scrx0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_scry0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_scrx1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_scry1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_ch0xofst(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_ch0yofst(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_ch1xofst(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_ch1yofst(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_shiftctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_out_cropofst(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_out_cropsize(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_wbsize0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_wbsize1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_wbctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_wbcfg(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_wbtmr(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_wbaddr0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_wbaddr1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_wbaddr2(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_wblstr0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_wblstr1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_csci_coef(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_csco_coef(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_vppctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_vpplutsw(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_checfg0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_checfg1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_peakcfg(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_ltivcfg(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_peakcoef0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_peakcoef1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_lticfg(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_cticfg(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_lticoef0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_lticoef1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_cticoef0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_cticoef1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_ch0_xcoef0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_ch0_xcoef1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_ch0_ycoef(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_ch1_xcoef0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_ch1_xcoef1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_ch1_ycoef(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_ch0lut(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_ch1lut(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_ch2lut(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se0_chhist(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_ver(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_ftr(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_csta(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_sta(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_ctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_cfg0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_cfg1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_cfg2(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_intctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_intsta(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_intclr(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_lncnt(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_updctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_ctabswt(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_strmctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_inaddr0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_inaddr1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_inaddr2(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_insize0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_insize1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_rsmpsize(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_inlstr0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_inlstr1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_scrx0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_scry0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_scrx1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_scry1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_ch0xofst(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_ch0yofst(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_ch1xofst(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_ch1yofst(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_shiftctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_out_cropofst(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_out_cropsize(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_wbsize0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_wbsize1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_wbctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_wbcfg(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_wbtmr(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_wbaddr0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_wbaddr1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_wbaddr2(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_wblstr0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_wblstr1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_csci_coef(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_csco_coef(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_vppctl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_vpplutsw(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_checfg0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_checfg1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_peakcfg(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_ltivcfg(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_peakcoef0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_peakcoef1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_lticfg(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_cticfg(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_lticoef0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_lticoef1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_cticoef0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_cticoef1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_ch0_xcoef0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_ch0_xcoef1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_ch0_ycoef(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_ch1_xcoef0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_ch1_xcoef1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_ch1_ycoef(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_ch0lut(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_ch1lut(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_ch2lut(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_se1_chhist(u32 addr, u32 data, u32 mode, char *outbuf);

#endif /* ___SE___DUMP__H___ */
