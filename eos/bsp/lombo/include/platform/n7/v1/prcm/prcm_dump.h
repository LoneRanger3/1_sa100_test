/* prcm_dump.h */

#ifndef ___PRCM___DUMP__H___
#define ___PRCM___DUMP__H___

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

u32 dump_prcm_ver(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_ahb_gat0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_ahb_gat1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_ahb_rst0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_ahb_rst1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_apb_gat0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_apb_rst0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_apb_gat1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_apb_rst1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_axi_gat(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_hfeosc_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_mbias_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_cpu_pll_en(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_cpu_pll_fac(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_cpu_pll_tune0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_cpu_pll_ten(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_cpu_pll_stat(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_cpu_pll_mod(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_cpu_pll_nfrac(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_cpu_pll_tune2(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_sdram_pll_en(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_sdram_pll_fac(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_sdram_pll_tune0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_sdram_pll_ten(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_sdram_pll_stat(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_sdram_pll_mod(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_sdram_pll_nfrac(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_sdram_pll_tune1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_sdram_pll_tune2(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_perh0_pll_en(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_perh0_pll_fac(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_perh0_pll_tune0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_perh0_pll_ten(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_perh0_pll_stat(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_perh0_pll_mod(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_perh0_pll_nfrac(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_audio_pll_en(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_audio_pll_fac(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_audio_pll_tune0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_audio_pll_ten(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_audio_pll_stat(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_audio_pll_mod(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_audio_pll_nfrac(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_vc_pll_en(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_vc_pll_fac(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_vc_pll_tune0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_vc_pll_ten(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_vc_pll_stat(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_vc_pll_mod(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_vc_pll_nfrac(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_perh1_pll_en(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_perh1_pll_fac(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_perh1_pll_tune0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_perh1_pll_ten(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_perh1_pll_stat(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_perh1_pll_mod(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_perh1_pll_nfrac(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_perh1_pll_tune1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_disp_pll_en(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_disp_pll_fac(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_disp_pll_tune0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_disp_pll_ten(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_disp_pll_stat(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_disp_pll_mod(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_disp_pll_nfrac(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_disp_pll_tune1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_ax_pll_en(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_ax_pll_fac(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_ax_pll_tune0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_ax_pll_ten(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_ax_pll_stat(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_ax_pll_mod(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_ax_pll_nfrac(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_ax_pll_tune2(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_perh2_pll_en(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_perh2_pll_fac(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_perh2_pll_tune0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_perh2_pll_ten(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_perh2_pll_stat(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_perh2_pll_mod(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_perh2_pll_nfrac(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_perh2_pll_tune1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_pll_tblk(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_cpu_axi_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_ahb_apb_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_maxi_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_sdram_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_sdram_rst(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_sdram_bm_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_sdc0_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_sdc1_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_sdc2_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_spi0_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_spi1_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_spi2_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_vc_enc_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_vc_dec_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_viss_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_dpu_sclk0_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_dpu_sclk1_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_dpu_sclk2_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_doss_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_ax_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_i2s0_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_i2s1_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_i2c0_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_i2c1_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_i2c2_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_i2c3_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_uart0_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_uart1_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_uart2_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_uart3_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_usb_phy_rst(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_gpadc_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_ir_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_gmac_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_mbist_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_prcm_aes_clk_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

#endif /* ___PRCM___DUMP__H___ */
