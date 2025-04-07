/* prcm_get.h */

#ifndef ___PRCM__GET___H___
#define ___PRCM__GET___H___

#ifdef DEF_GET_PRCM_VER_VER_L
def_get_mod_reg_bit(prcm, ver, ver_l, addr);
#endif

#ifdef DEF_GET_PRCM_VER_VER_H
def_get_mod_reg_bit(prcm, ver, ver_h, addr);
#endif

#ifdef DEF_GET_PRCM_VER_COMP
def_get_mod_reg_bit(prcm, ver, comp, addr);
#endif

#ifdef DEF_GET_PRCM_AHB_GAT0_DMA
def_get_mod_reg_bit(prcm, ahb_gat0, dma, addr);
#endif

#ifdef DEF_GET_PRCM_AHB_GAT0_PTIMER
def_get_mod_reg_bit(prcm, ahb_gat0, ptimer, addr);
#endif

#ifdef DEF_GET_PRCM_AHB_GAT0_AES
def_get_mod_reg_bit(prcm, ahb_gat0, aes, addr);
#endif

#ifdef DEF_GET_PRCM_AHB_GAT0_SDRAM
def_get_mod_reg_bit(prcm, ahb_gat0, sdram, addr);
#endif

#ifdef DEF_GET_PRCM_AHB_GAT0_SDC0
def_get_mod_reg_bit(prcm, ahb_gat0, sdc0, addr);
#endif

#ifdef DEF_GET_PRCM_AHB_GAT0_SDC1
def_get_mod_reg_bit(prcm, ahb_gat0, sdc1, addr);
#endif

#ifdef DEF_GET_PRCM_AHB_GAT0_SDC2
def_get_mod_reg_bit(prcm, ahb_gat0, sdc2, addr);
#endif

#ifdef DEF_GET_PRCM_AHB_GAT0_SPI0
def_get_mod_reg_bit(prcm, ahb_gat0, spi0, addr);
#endif

#ifdef DEF_GET_PRCM_AHB_GAT0_SPI1
def_get_mod_reg_bit(prcm, ahb_gat0, spi1, addr);
#endif

#ifdef DEF_GET_PRCM_AHB_GAT0_SPI2
def_get_mod_reg_bit(prcm, ahb_gat0, spi2, addr);
#endif

#ifdef DEF_GET_PRCM_AHB_GAT1_VISS
def_get_mod_reg_bit(prcm, ahb_gat1, viss, addr);
#endif

#ifdef DEF_GET_PRCM_AHB_GAT1_DPU
def_get_mod_reg_bit(prcm, ahb_gat1, dpu, addr);
#endif

#ifdef DEF_GET_PRCM_AHB_GAT1_DOSS
def_get_mod_reg_bit(prcm, ahb_gat1, doss, addr);
#endif

#ifdef DEF_GET_PRCM_AHB_GAT1_VC_DEC
def_get_mod_reg_bit(prcm, ahb_gat1, vc_dec, addr);
#endif

#ifdef DEF_GET_PRCM_AHB_GAT1_VC_ENC
def_get_mod_reg_bit(prcm, ahb_gat1, vc_enc, addr);
#endif

#ifdef DEF_GET_PRCM_AHB_GAT1_AX
def_get_mod_reg_bit(prcm, ahb_gat1, ax, addr);
#endif

#ifdef DEF_GET_PRCM_AHB_GAT1_USB
def_get_mod_reg_bit(prcm, ahb_gat1, usb, addr);
#endif

#ifdef DEF_GET_PRCM_AHB_GAT1_GMAC
def_get_mod_reg_bit(prcm, ahb_gat1, gmac, addr);
#endif

#ifdef DEF_GET_PRCM_AHB_RST0_DMA
def_get_mod_reg_bit(prcm, ahb_rst0, dma, addr);
#endif

#ifdef DEF_GET_PRCM_AHB_RST0_PTIMER
def_get_mod_reg_bit(prcm, ahb_rst0, ptimer, addr);
#endif

#ifdef DEF_GET_PRCM_AHB_RST0_AES
def_get_mod_reg_bit(prcm, ahb_rst0, aes, addr);
#endif

#ifdef DEF_GET_PRCM_AHB_RST0_SDRAM
def_get_mod_reg_bit(prcm, ahb_rst0, sdram, addr);
#endif

#ifdef DEF_GET_PRCM_AHB_RST0_SDC0
def_get_mod_reg_bit(prcm, ahb_rst0, sdc0, addr);
#endif

#ifdef DEF_GET_PRCM_AHB_RST0_SDC1
def_get_mod_reg_bit(prcm, ahb_rst0, sdc1, addr);
#endif

#ifdef DEF_GET_PRCM_AHB_RST0_SDC2
def_get_mod_reg_bit(prcm, ahb_rst0, sdc2, addr);
#endif

#ifdef DEF_GET_PRCM_AHB_RST0_SPI0
def_get_mod_reg_bit(prcm, ahb_rst0, spi0, addr);
#endif

#ifdef DEF_GET_PRCM_AHB_RST0_SPI1
def_get_mod_reg_bit(prcm, ahb_rst0, spi1, addr);
#endif

#ifdef DEF_GET_PRCM_AHB_RST0_SPI2
def_get_mod_reg_bit(prcm, ahb_rst0, spi2, addr);
#endif

#ifdef DEF_GET_PRCM_AHB_RST1_VISS
def_get_mod_reg_bit(prcm, ahb_rst1, viss, addr);
#endif

#ifdef DEF_GET_PRCM_AHB_RST1_DPU
def_get_mod_reg_bit(prcm, ahb_rst1, dpu, addr);
#endif

#ifdef DEF_GET_PRCM_AHB_RST1_DOSS
def_get_mod_reg_bit(prcm, ahb_rst1, doss, addr);
#endif

#ifdef DEF_GET_PRCM_AHB_RST1_VC_DEC
def_get_mod_reg_bit(prcm, ahb_rst1, vc_dec, addr);
#endif

#ifdef DEF_GET_PRCM_AHB_RST1_VC_ENC
def_get_mod_reg_bit(prcm, ahb_rst1, vc_enc, addr);
#endif

#ifdef DEF_GET_PRCM_AHB_RST1_AX
def_get_mod_reg_bit(prcm, ahb_rst1, ax, addr);
#endif

#ifdef DEF_GET_PRCM_AHB_RST1_USB
def_get_mod_reg_bit(prcm, ahb_rst1, usb, addr);
#endif

#ifdef DEF_GET_PRCM_AHB_RST1_GMAC
def_get_mod_reg_bit(prcm, ahb_rst1, gmac, addr);
#endif

#ifdef DEF_GET_PRCM_APB_GAT0_I2C0
def_get_mod_reg_bit(prcm, apb_gat0, i2c0, addr);
#endif

#ifdef DEF_GET_PRCM_APB_GAT0_I2C1
def_get_mod_reg_bit(prcm, apb_gat0, i2c1, addr);
#endif

#ifdef DEF_GET_PRCM_APB_GAT0_I2C2
def_get_mod_reg_bit(prcm, apb_gat0, i2c2, addr);
#endif

#ifdef DEF_GET_PRCM_APB_GAT0_I2C3
def_get_mod_reg_bit(prcm, apb_gat0, i2c3, addr);
#endif

#ifdef DEF_GET_PRCM_APB_GAT0_UART0
def_get_mod_reg_bit(prcm, apb_gat0, uart0, addr);
#endif

#ifdef DEF_GET_PRCM_APB_GAT0_UART1
def_get_mod_reg_bit(prcm, apb_gat0, uart1, addr);
#endif

#ifdef DEF_GET_PRCM_APB_GAT0_UART2
def_get_mod_reg_bit(prcm, apb_gat0, uart2, addr);
#endif

#ifdef DEF_GET_PRCM_APB_GAT0_UART3
def_get_mod_reg_bit(prcm, apb_gat0, uart3, addr);
#endif

#ifdef DEF_GET_PRCM_APB_RST0_I2C0
def_get_mod_reg_bit(prcm, apb_rst0, i2c0, addr);
#endif

#ifdef DEF_GET_PRCM_APB_RST0_I2C1
def_get_mod_reg_bit(prcm, apb_rst0, i2c1, addr);
#endif

#ifdef DEF_GET_PRCM_APB_RST0_I2C2
def_get_mod_reg_bit(prcm, apb_rst0, i2c2, addr);
#endif

#ifdef DEF_GET_PRCM_APB_RST0_I2C3
def_get_mod_reg_bit(prcm, apb_rst0, i2c3, addr);
#endif

#ifdef DEF_GET_PRCM_APB_RST0_UART0
def_get_mod_reg_bit(prcm, apb_rst0, uart0, addr);
#endif

#ifdef DEF_GET_PRCM_APB_RST0_UART1
def_get_mod_reg_bit(prcm, apb_rst0, uart1, addr);
#endif

#ifdef DEF_GET_PRCM_APB_RST0_UART2
def_get_mod_reg_bit(prcm, apb_rst0, uart2, addr);
#endif

#ifdef DEF_GET_PRCM_APB_RST0_UART3
def_get_mod_reg_bit(prcm, apb_rst0, uart3, addr);
#endif

#ifdef DEF_GET_PRCM_APB_GAT1_I2S0
def_get_mod_reg_bit(prcm, apb_gat1, i2s0, addr);
#endif

#ifdef DEF_GET_PRCM_APB_GAT1_I2S1
def_get_mod_reg_bit(prcm, apb_gat1, i2s1, addr);
#endif

#ifdef DEF_GET_PRCM_APB_GAT1_GPADC
def_get_mod_reg_bit(prcm, apb_gat1, gpadc, addr);
#endif

#ifdef DEF_GET_PRCM_APB_GAT1_IR
def_get_mod_reg_bit(prcm, apb_gat1, ir, addr);
#endif

#ifdef DEF_GET_PRCM_APB_GAT1_GPIO
def_get_mod_reg_bit(prcm, apb_gat1, gpio, addr);
#endif

#ifdef DEF_GET_PRCM_APB_RST1_I2S0
def_get_mod_reg_bit(prcm, apb_rst1, i2s0, addr);
#endif

#ifdef DEF_GET_PRCM_APB_RST1_I2S1
def_get_mod_reg_bit(prcm, apb_rst1, i2s1, addr);
#endif

#ifdef DEF_GET_PRCM_APB_RST1_GPADC
def_get_mod_reg_bit(prcm, apb_rst1, gpadc, addr);
#endif

#ifdef DEF_GET_PRCM_APB_RST1_IR
def_get_mod_reg_bit(prcm, apb_rst1, ir, addr);
#endif

#ifdef DEF_GET_PRCM_APB_RST1_GPIO
def_get_mod_reg_bit(prcm, apb_rst1, gpio, addr);
#endif

#ifdef DEF_GET_PRCM_AXI_GAT_DMA
def_get_mod_reg_bit(prcm, axi_gat, dma, addr);
#endif

#ifdef DEF_GET_PRCM_AXI_GAT_VC_ENC
def_get_mod_reg_bit(prcm, axi_gat, vc_enc, addr);
#endif

#ifdef DEF_GET_PRCM_AXI_GAT_VC_DEC
def_get_mod_reg_bit(prcm, axi_gat, vc_dec, addr);
#endif

#ifdef DEF_GET_PRCM_AXI_GAT_VISS0
def_get_mod_reg_bit(prcm, axi_gat, viss0, addr);
#endif

#ifdef DEF_GET_PRCM_AXI_GAT_VISS1
def_get_mod_reg_bit(prcm, axi_gat, viss1, addr);
#endif

#ifdef DEF_GET_PRCM_AXI_GAT_DPU
def_get_mod_reg_bit(prcm, axi_gat, dpu, addr);
#endif

#ifdef DEF_GET_PRCM_AXI_GAT_DOSS
def_get_mod_reg_bit(prcm, axi_gat, doss, addr);
#endif

#ifdef DEF_GET_PRCM_AXI_GAT_AX
def_get_mod_reg_bit(prcm, axi_gat, ax, addr);
#endif

#ifdef DEF_GET_PRCM_AXI_GAT_GMAC
def_get_mod_reg_bit(prcm, axi_gat, gmac, addr);
#endif

#ifdef DEF_GET_PRCM_HFEOSC_CTRL_EN
def_get_mod_reg_bit(prcm, hfeosc_ctrl, en, addr);
#endif

#ifdef DEF_GET_PRCM_HFEOSC_CTRL_SRC_SEL
def_get_mod_reg_bit(prcm, hfeosc_ctrl, src_sel, addr);
#endif

#ifdef DEF_GET_PRCM_HFEOSC_CTRL_XTAL_START
def_get_mod_reg_bit(prcm, hfeosc_ctrl, xtal_start, addr);
#endif

#ifdef DEF_GET_PRCM_HFEOSC_CTRL_TUNE
def_get_mod_reg_bit(prcm, hfeosc_ctrl, tune, addr);
#endif

#ifdef DEF_GET_PRCM_HFEOSC_CTRL_SRC_SEL_DBG
def_get_mod_reg_bit(prcm, hfeosc_ctrl, src_sel_dbg, addr);
#endif

#ifdef DEF_GET_PRCM_MBIAS_CTRL_EN
def_get_mod_reg_bit(prcm, mbias_ctrl, en, addr);
#endif

#ifdef DEF_GET_PRCM_MBIAS_CTRL_TST_EN0
def_get_mod_reg_bit(prcm, mbias_ctrl, tst_en0, addr);
#endif

#ifdef DEF_GET_PRCM_MBIAS_CTRL_TST_EN1
def_get_mod_reg_bit(prcm, mbias_ctrl, tst_en1, addr);
#endif

#ifdef DEF_GET_PRCM_MBIAS_CTRL_TST_EN2
def_get_mod_reg_bit(prcm, mbias_ctrl, tst_en2, addr);
#endif

#ifdef DEF_GET_PRCM_MBIAS_CTRL_TST_EN3
def_get_mod_reg_bit(prcm, mbias_ctrl, tst_en3, addr);
#endif

#ifdef DEF_GET_PRCM_MBIAS_CTRL_TUNE2_0
def_get_mod_reg_bit(prcm, mbias_ctrl, tune2_0, addr);
#endif

#ifdef DEF_GET_PRCM_MBIAS_CTRL_TUNE3
def_get_mod_reg_bit(prcm, mbias_ctrl, tune3, addr);
#endif

#ifdef DEF_GET_PRCM_MBIAS_CTRL_TUNE4
def_get_mod_reg_bit(prcm, mbias_ctrl, tune4, addr);
#endif

#ifdef DEF_GET_PRCM_CPU_PLL_EN_ENP
def_get_mod_reg_bit(prcm, cpu_pll_en, enp, addr);
#endif

#ifdef DEF_GET_PRCM_CPU_PLL_EN_ENM
def_get_mod_reg_bit(prcm, cpu_pll_en, enm, addr);
#endif

#ifdef DEF_GET_PRCM_CPU_PLL_EN_OEN
def_get_mod_reg_bit(prcm, cpu_pll_en, oen, addr);
#endif

#ifdef DEF_GET_PRCM_CPU_PLL_EN_REN
def_get_mod_reg_bit(prcm, cpu_pll_en, ren, addr);
#endif

#ifdef DEF_GET_PRCM_CPU_PLL_FAC_PRE_DIV
def_get_mod_reg_bit(prcm, cpu_pll_fac, pre_div, addr);
#endif

#ifdef DEF_GET_PRCM_CPU_PLL_FAC_POST_DIV
def_get_mod_reg_bit(prcm, cpu_pll_fac, post_div, addr);
#endif

#ifdef DEF_GET_PRCM_CPU_PLL_FAC_N
def_get_mod_reg_bit(prcm, cpu_pll_fac, n, addr);
#endif

#ifdef DEF_GET_PRCM_CPU_PLL_TUNE0_TUNE1
def_get_mod_reg_bit(prcm, cpu_pll_tune0, tune1, addr);
#endif

#ifdef DEF_GET_PRCM_CPU_PLL_TUNE0_TUNE2
def_get_mod_reg_bit(prcm, cpu_pll_tune0, tune2, addr);
#endif

#ifdef DEF_GET_PRCM_CPU_PLL_TUNE0_TUNE3
def_get_mod_reg_bit(prcm, cpu_pll_tune0, tune3, addr);
#endif

#ifdef DEF_GET_PRCM_CPU_PLL_TUNE0_TUNE4
def_get_mod_reg_bit(prcm, cpu_pll_tune0, tune4, addr);
#endif

#ifdef DEF_GET_PRCM_CPU_PLL_TUNE0_TUNE5
def_get_mod_reg_bit(prcm, cpu_pll_tune0, tune5, addr);
#endif

#ifdef DEF_GET_PRCM_CPU_PLL_TUNE0_TUNE6
def_get_mod_reg_bit(prcm, cpu_pll_tune0, tune6, addr);
#endif

#ifdef DEF_GET_PRCM_CPU_PLL_TUNE0_TUNE7
def_get_mod_reg_bit(prcm, cpu_pll_tune0, tune7, addr);
#endif

#ifdef DEF_GET_PRCM_CPU_PLL_TUNE0_TUNE8
def_get_mod_reg_bit(prcm, cpu_pll_tune0, tune8, addr);
#endif

#ifdef DEF_GET_PRCM_CPU_PLL_TUNE0_TUNE9
def_get_mod_reg_bit(prcm, cpu_pll_tune0, tune9, addr);
#endif

#ifdef DEF_GET_PRCM_CPU_PLL_TUNE0_TUNE10
def_get_mod_reg_bit(prcm, cpu_pll_tune0, tune10, addr);
#endif

#ifdef DEF_GET_PRCM_CPU_PLL_TUNE0_TUNE11
def_get_mod_reg_bit(prcm, cpu_pll_tune0, tune11, addr);
#endif

#ifdef DEF_GET_PRCM_CPU_PLL_TUNE0_TUNE12
def_get_mod_reg_bit(prcm, cpu_pll_tune0, tune12, addr);
#endif

#ifdef DEF_GET_PRCM_CPU_PLL_TEN_TEN0
def_get_mod_reg_bit(prcm, cpu_pll_ten, ten0, addr);
#endif

#ifdef DEF_GET_PRCM_CPU_PLL_TEN_TEN1
def_get_mod_reg_bit(prcm, cpu_pll_ten, ten1, addr);
#endif

#ifdef DEF_GET_PRCM_CPU_PLL_STAT_DLOCK
def_get_mod_reg_bit(prcm, cpu_pll_stat, dlock, addr);
#endif

#ifdef DEF_GET_PRCM_CPU_PLL_STAT_ASTEADY
def_get_mod_reg_bit(prcm, cpu_pll_stat, asteady, addr);
#endif

#ifdef DEF_GET_PRCM_CPU_PLL_STAT_ALOCK
def_get_mod_reg_bit(prcm, cpu_pll_stat, alock, addr);
#endif

#ifdef DEF_GET_PRCM_CPU_PLL_MOD_MOD
def_get_mod_reg_bit(prcm, cpu_pll_mod, mod, addr);
#endif

#ifdef DEF_GET_PRCM_CPU_PLL_NFRAC_NFRAC
def_get_mod_reg_bit(prcm, cpu_pll_nfrac, nfrac, addr);
#endif

#ifdef DEF_GET_PRCM_CPU_PLL_TUNE2_EN
def_get_mod_reg_bit(prcm, cpu_pll_tune2, en, addr);
#endif

#ifdef DEF_GET_PRCM_CPU_PLL_TUNE2_STEP
def_get_mod_reg_bit(prcm, cpu_pll_tune2, step, addr);
#endif

#ifdef DEF_GET_PRCM_SDRAM_PLL_EN_ENP
def_get_mod_reg_bit(prcm, sdram_pll_en, enp, addr);
#endif

#ifdef DEF_GET_PRCM_SDRAM_PLL_EN_ENM
def_get_mod_reg_bit(prcm, sdram_pll_en, enm, addr);
#endif

#ifdef DEF_GET_PRCM_SDRAM_PLL_EN_OEN
def_get_mod_reg_bit(prcm, sdram_pll_en, oen, addr);
#endif

#ifdef DEF_GET_PRCM_SDRAM_PLL_EN_REN
def_get_mod_reg_bit(prcm, sdram_pll_en, ren, addr);
#endif

#ifdef DEF_GET_PRCM_SDRAM_PLL_FAC_PRE_DIV
def_get_mod_reg_bit(prcm, sdram_pll_fac, pre_div, addr);
#endif

#ifdef DEF_GET_PRCM_SDRAM_PLL_FAC_POST_DIV
def_get_mod_reg_bit(prcm, sdram_pll_fac, post_div, addr);
#endif

#ifdef DEF_GET_PRCM_SDRAM_PLL_FAC_N
def_get_mod_reg_bit(prcm, sdram_pll_fac, n, addr);
#endif

#ifdef DEF_GET_PRCM_SDRAM_PLL_TUNE0_TUNE1
def_get_mod_reg_bit(prcm, sdram_pll_tune0, tune1, addr);
#endif

#ifdef DEF_GET_PRCM_SDRAM_PLL_TUNE0_TUNE3
def_get_mod_reg_bit(prcm, sdram_pll_tune0, tune3, addr);
#endif

#ifdef DEF_GET_PRCM_SDRAM_PLL_TUNE0_TUNE4
def_get_mod_reg_bit(prcm, sdram_pll_tune0, tune4, addr);
#endif

#ifdef DEF_GET_PRCM_SDRAM_PLL_TUNE0_TUNE5
def_get_mod_reg_bit(prcm, sdram_pll_tune0, tune5, addr);
#endif

#ifdef DEF_GET_PRCM_SDRAM_PLL_TUNE0_TUNE6
def_get_mod_reg_bit(prcm, sdram_pll_tune0, tune6, addr);
#endif

#ifdef DEF_GET_PRCM_SDRAM_PLL_TUNE0_TUNE7
def_get_mod_reg_bit(prcm, sdram_pll_tune0, tune7, addr);
#endif

#ifdef DEF_GET_PRCM_SDRAM_PLL_TUNE0_TUNE8
def_get_mod_reg_bit(prcm, sdram_pll_tune0, tune8, addr);
#endif

#ifdef DEF_GET_PRCM_SDRAM_PLL_TUNE0_TUNE9
def_get_mod_reg_bit(prcm, sdram_pll_tune0, tune9, addr);
#endif

#ifdef DEF_GET_PRCM_SDRAM_PLL_TUNE0_TUNE10
def_get_mod_reg_bit(prcm, sdram_pll_tune0, tune10, addr);
#endif

#ifdef DEF_GET_PRCM_SDRAM_PLL_TUNE0_TUNE11
def_get_mod_reg_bit(prcm, sdram_pll_tune0, tune11, addr);
#endif

#ifdef DEF_GET_PRCM_SDRAM_PLL_TUNE0_TUNE12
def_get_mod_reg_bit(prcm, sdram_pll_tune0, tune12, addr);
#endif

#ifdef DEF_GET_PRCM_SDRAM_PLL_TUNE0_TUNE13
def_get_mod_reg_bit(prcm, sdram_pll_tune0, tune13, addr);
#endif

#ifdef DEF_GET_PRCM_SDRAM_PLL_TEN_TEN0
def_get_mod_reg_bit(prcm, sdram_pll_ten, ten0, addr);
#endif

#ifdef DEF_GET_PRCM_SDRAM_PLL_TEN_TEN1
def_get_mod_reg_bit(prcm, sdram_pll_ten, ten1, addr);
#endif

#ifdef DEF_GET_PRCM_SDRAM_PLL_TEN_TEN2
def_get_mod_reg_bit(prcm, sdram_pll_ten, ten2, addr);
#endif

#ifdef DEF_GET_PRCM_SDRAM_PLL_TEN_TEN3
def_get_mod_reg_bit(prcm, sdram_pll_ten, ten3, addr);
#endif

#ifdef DEF_GET_PRCM_SDRAM_PLL_STAT_DLOCK
def_get_mod_reg_bit(prcm, sdram_pll_stat, dlock, addr);
#endif

#ifdef DEF_GET_PRCM_SDRAM_PLL_STAT_ASTEADY
def_get_mod_reg_bit(prcm, sdram_pll_stat, asteady, addr);
#endif

#ifdef DEF_GET_PRCM_SDRAM_PLL_STAT_ALOCK
def_get_mod_reg_bit(prcm, sdram_pll_stat, alock, addr);
#endif

#ifdef DEF_GET_PRCM_SDRAM_PLL_MOD_MOD
def_get_mod_reg_bit(prcm, sdram_pll_mod, mod, addr);
#endif

#ifdef DEF_GET_PRCM_SDRAM_PLL_NFRAC_NFRAC
def_get_mod_reg_bit(prcm, sdram_pll_nfrac, nfrac, addr);
#endif

#ifdef DEF_GET_PRCM_SDRAM_PLL_TUNE1_PERIOD
def_get_mod_reg_bit(prcm, sdram_pll_tune1, period, addr);
#endif

#ifdef DEF_GET_PRCM_SDRAM_PLL_TUNE1_AMPLITUDE
def_get_mod_reg_bit(prcm, sdram_pll_tune1, amplitude, addr);
#endif

#ifdef DEF_GET_PRCM_SDRAM_PLL_TUNE2_EN
def_get_mod_reg_bit(prcm, sdram_pll_tune2, en, addr);
#endif

#ifdef DEF_GET_PRCM_SDRAM_PLL_TUNE2_STEP
def_get_mod_reg_bit(prcm, sdram_pll_tune2, step, addr);
#endif

#ifdef DEF_GET_PRCM_PERH0_PLL_EN_ENP
def_get_mod_reg_bit(prcm, perh0_pll_en, enp, addr);
#endif

#ifdef DEF_GET_PRCM_PERH0_PLL_EN_ENM
def_get_mod_reg_bit(prcm, perh0_pll_en, enm, addr);
#endif

#ifdef DEF_GET_PRCM_PERH0_PLL_EN_OEN
def_get_mod_reg_bit(prcm, perh0_pll_en, oen, addr);
#endif

#ifdef DEF_GET_PRCM_PERH0_PLL_EN_REN
def_get_mod_reg_bit(prcm, perh0_pll_en, ren, addr);
#endif

#ifdef DEF_GET_PRCM_PERH0_PLL_FAC_PRE_DIV
def_get_mod_reg_bit(prcm, perh0_pll_fac, pre_div, addr);
#endif

#ifdef DEF_GET_PRCM_PERH0_PLL_FAC_POST_DIV
def_get_mod_reg_bit(prcm, perh0_pll_fac, post_div, addr);
#endif

#ifdef DEF_GET_PRCM_PERH0_PLL_FAC_N
def_get_mod_reg_bit(prcm, perh0_pll_fac, n, addr);
#endif

#ifdef DEF_GET_PRCM_PERH0_PLL_TUNE0_TUNE1
def_get_mod_reg_bit(prcm, perh0_pll_tune0, tune1, addr);
#endif

#ifdef DEF_GET_PRCM_PERH0_PLL_TUNE0_TUNE2
def_get_mod_reg_bit(prcm, perh0_pll_tune0, tune2, addr);
#endif

#ifdef DEF_GET_PRCM_PERH0_PLL_TUNE0_TUNE3
def_get_mod_reg_bit(prcm, perh0_pll_tune0, tune3, addr);
#endif

#ifdef DEF_GET_PRCM_PERH0_PLL_TUNE0_TUNE4
def_get_mod_reg_bit(prcm, perh0_pll_tune0, tune4, addr);
#endif

#ifdef DEF_GET_PRCM_PERH0_PLL_TUNE0_TUNE5
def_get_mod_reg_bit(prcm, perh0_pll_tune0, tune5, addr);
#endif

#ifdef DEF_GET_PRCM_PERH0_PLL_TUNE0_TUNE6
def_get_mod_reg_bit(prcm, perh0_pll_tune0, tune6, addr);
#endif

#ifdef DEF_GET_PRCM_PERH0_PLL_TUNE0_TUNE7
def_get_mod_reg_bit(prcm, perh0_pll_tune0, tune7, addr);
#endif

#ifdef DEF_GET_PRCM_PERH0_PLL_TUNE0_TUNE8
def_get_mod_reg_bit(prcm, perh0_pll_tune0, tune8, addr);
#endif

#ifdef DEF_GET_PRCM_PERH0_PLL_TUNE0_TUNE9
def_get_mod_reg_bit(prcm, perh0_pll_tune0, tune9, addr);
#endif

#ifdef DEF_GET_PRCM_PERH0_PLL_TUNE0_TUNE10
def_get_mod_reg_bit(prcm, perh0_pll_tune0, tune10, addr);
#endif

#ifdef DEF_GET_PRCM_PERH0_PLL_TEN_TEN0
def_get_mod_reg_bit(prcm, perh0_pll_ten, ten0, addr);
#endif

#ifdef DEF_GET_PRCM_PERH0_PLL_TEN_TEN1
def_get_mod_reg_bit(prcm, perh0_pll_ten, ten1, addr);
#endif

#ifdef DEF_GET_PRCM_PERH0_PLL_TEN_TEN3
def_get_mod_reg_bit(prcm, perh0_pll_ten, ten3, addr);
#endif

#ifdef DEF_GET_PRCM_PERH0_PLL_TEN_TEN2
def_get_mod_reg_bit(prcm, perh0_pll_ten, ten2, addr);
#endif

#ifdef DEF_GET_PRCM_PERH0_PLL_STAT_DLOCK
def_get_mod_reg_bit(prcm, perh0_pll_stat, dlock, addr);
#endif

#ifdef DEF_GET_PRCM_PERH0_PLL_STAT_ASTEADY
def_get_mod_reg_bit(prcm, perh0_pll_stat, asteady, addr);
#endif

#ifdef DEF_GET_PRCM_PERH0_PLL_STAT_ALOCK
def_get_mod_reg_bit(prcm, perh0_pll_stat, alock, addr);
#endif

#ifdef DEF_GET_PRCM_PERH0_PLL_MOD_MOD
def_get_mod_reg_bit(prcm, perh0_pll_mod, mod, addr);
#endif

#ifdef DEF_GET_PRCM_PERH0_PLL_NFRAC_NFRAC
def_get_mod_reg_bit(prcm, perh0_pll_nfrac, nfrac, addr);
#endif

#ifdef DEF_GET_PRCM_AUDIO_PLL_EN_ENP
def_get_mod_reg_bit(prcm, audio_pll_en, enp, addr);
#endif

#ifdef DEF_GET_PRCM_AUDIO_PLL_EN_ENM
def_get_mod_reg_bit(prcm, audio_pll_en, enm, addr);
#endif

#ifdef DEF_GET_PRCM_AUDIO_PLL_EN_OEN
def_get_mod_reg_bit(prcm, audio_pll_en, oen, addr);
#endif

#ifdef DEF_GET_PRCM_AUDIO_PLL_EN_REN
def_get_mod_reg_bit(prcm, audio_pll_en, ren, addr);
#endif

#ifdef DEF_GET_PRCM_AUDIO_PLL_EN_DIV_ENDIV17
def_get_mod_reg_bit(prcm, audio_pll_en, div_endiv17, addr);
#endif

#ifdef DEF_GET_PRCM_AUDIO_PLL_EN_DIV_ENDIV7
def_get_mod_reg_bit(prcm, audio_pll_en, div_endiv7, addr);
#endif

#ifdef DEF_GET_PRCM_AUDIO_PLL_FAC_PRE_DIV
def_get_mod_reg_bit(prcm, audio_pll_fac, pre_div, addr);
#endif

#ifdef DEF_GET_PRCM_AUDIO_PLL_FAC_POST_DIV
def_get_mod_reg_bit(prcm, audio_pll_fac, post_div, addr);
#endif

#ifdef DEF_GET_PRCM_AUDIO_PLL_FAC_N
def_get_mod_reg_bit(prcm, audio_pll_fac, n, addr);
#endif

#ifdef DEF_GET_PRCM_AUDIO_PLL_TUNE0_TUNE1
def_get_mod_reg_bit(prcm, audio_pll_tune0, tune1, addr);
#endif

#ifdef DEF_GET_PRCM_AUDIO_PLL_TUNE0_TUNE2
def_get_mod_reg_bit(prcm, audio_pll_tune0, tune2, addr);
#endif

#ifdef DEF_GET_PRCM_AUDIO_PLL_TUNE0_TUNE3
def_get_mod_reg_bit(prcm, audio_pll_tune0, tune3, addr);
#endif

#ifdef DEF_GET_PRCM_AUDIO_PLL_TUNE0_TUNE4
def_get_mod_reg_bit(prcm, audio_pll_tune0, tune4, addr);
#endif

#ifdef DEF_GET_PRCM_AUDIO_PLL_TUNE0_TUNE5
def_get_mod_reg_bit(prcm, audio_pll_tune0, tune5, addr);
#endif

#ifdef DEF_GET_PRCM_AUDIO_PLL_TUNE0_TUNE6
def_get_mod_reg_bit(prcm, audio_pll_tune0, tune6, addr);
#endif

#ifdef DEF_GET_PRCM_AUDIO_PLL_TUNE0_TUNE7
def_get_mod_reg_bit(prcm, audio_pll_tune0, tune7, addr);
#endif

#ifdef DEF_GET_PRCM_AUDIO_PLL_TUNE0_TUNE8
def_get_mod_reg_bit(prcm, audio_pll_tune0, tune8, addr);
#endif

#ifdef DEF_GET_PRCM_AUDIO_PLL_TUNE0_TUNE9
def_get_mod_reg_bit(prcm, audio_pll_tune0, tune9, addr);
#endif

#ifdef DEF_GET_PRCM_AUDIO_PLL_TUNE0_TUNE10
def_get_mod_reg_bit(prcm, audio_pll_tune0, tune10, addr);
#endif

#ifdef DEF_GET_PRCM_AUDIO_PLL_TUNE0_TUNE11
def_get_mod_reg_bit(prcm, audio_pll_tune0, tune11, addr);
#endif

#ifdef DEF_GET_PRCM_AUDIO_PLL_TUNE0_TUNE12
def_get_mod_reg_bit(prcm, audio_pll_tune0, tune12, addr);
#endif

#ifdef DEF_GET_PRCM_AUDIO_PLL_TEN_TEN0
def_get_mod_reg_bit(prcm, audio_pll_ten, ten0, addr);
#endif

#ifdef DEF_GET_PRCM_AUDIO_PLL_TEN_TEN1
def_get_mod_reg_bit(prcm, audio_pll_ten, ten1, addr);
#endif

#ifdef DEF_GET_PRCM_AUDIO_PLL_STAT_DLOCK
def_get_mod_reg_bit(prcm, audio_pll_stat, dlock, addr);
#endif

#ifdef DEF_GET_PRCM_AUDIO_PLL_STAT_ASTEADY
def_get_mod_reg_bit(prcm, audio_pll_stat, asteady, addr);
#endif

#ifdef DEF_GET_PRCM_AUDIO_PLL_STAT_ALOCK
def_get_mod_reg_bit(prcm, audio_pll_stat, alock, addr);
#endif

#ifdef DEF_GET_PRCM_AUDIO_PLL_MOD_MOD
def_get_mod_reg_bit(prcm, audio_pll_mod, mod, addr);
#endif

#ifdef DEF_GET_PRCM_AUDIO_PLL_NFRAC_NFRAC
def_get_mod_reg_bit(prcm, audio_pll_nfrac, nfrac, addr);
#endif

#ifdef DEF_GET_PRCM_VC_PLL_EN_ENP
def_get_mod_reg_bit(prcm, vc_pll_en, enp, addr);
#endif

#ifdef DEF_GET_PRCM_VC_PLL_EN_ENM
def_get_mod_reg_bit(prcm, vc_pll_en, enm, addr);
#endif

#ifdef DEF_GET_PRCM_VC_PLL_EN_OEN
def_get_mod_reg_bit(prcm, vc_pll_en, oen, addr);
#endif

#ifdef DEF_GET_PRCM_VC_PLL_EN_REN
def_get_mod_reg_bit(prcm, vc_pll_en, ren, addr);
#endif

#ifdef DEF_GET_PRCM_VC_PLL_FAC_PRE_DIV
def_get_mod_reg_bit(prcm, vc_pll_fac, pre_div, addr);
#endif

#ifdef DEF_GET_PRCM_VC_PLL_FAC_POST_DIV
def_get_mod_reg_bit(prcm, vc_pll_fac, post_div, addr);
#endif

#ifdef DEF_GET_PRCM_VC_PLL_FAC_N
def_get_mod_reg_bit(prcm, vc_pll_fac, n, addr);
#endif

#ifdef DEF_GET_PRCM_VC_PLL_TUNE0_TUNE1
def_get_mod_reg_bit(prcm, vc_pll_tune0, tune1, addr);
#endif

#ifdef DEF_GET_PRCM_VC_PLL_TUNE0_TUNE2
def_get_mod_reg_bit(prcm, vc_pll_tune0, tune2, addr);
#endif

#ifdef DEF_GET_PRCM_VC_PLL_TUNE0_TUNE3
def_get_mod_reg_bit(prcm, vc_pll_tune0, tune3, addr);
#endif

#ifdef DEF_GET_PRCM_VC_PLL_TUNE0_TUNE4
def_get_mod_reg_bit(prcm, vc_pll_tune0, tune4, addr);
#endif

#ifdef DEF_GET_PRCM_VC_PLL_TUNE0_TUNE5
def_get_mod_reg_bit(prcm, vc_pll_tune0, tune5, addr);
#endif

#ifdef DEF_GET_PRCM_VC_PLL_TUNE0_TUNE6
def_get_mod_reg_bit(prcm, vc_pll_tune0, tune6, addr);
#endif

#ifdef DEF_GET_PRCM_VC_PLL_TUNE0_TUNE7
def_get_mod_reg_bit(prcm, vc_pll_tune0, tune7, addr);
#endif

#ifdef DEF_GET_PRCM_VC_PLL_TUNE0_TUNE8
def_get_mod_reg_bit(prcm, vc_pll_tune0, tune8, addr);
#endif

#ifdef DEF_GET_PRCM_VC_PLL_TUNE0_TUNE9
def_get_mod_reg_bit(prcm, vc_pll_tune0, tune9, addr);
#endif

#ifdef DEF_GET_PRCM_VC_PLL_TUNE0_TUNE10
def_get_mod_reg_bit(prcm, vc_pll_tune0, tune10, addr);
#endif

#ifdef DEF_GET_PRCM_VC_PLL_TEN_TEN0
def_get_mod_reg_bit(prcm, vc_pll_ten, ten0, addr);
#endif

#ifdef DEF_GET_PRCM_VC_PLL_TEN_TEN1
def_get_mod_reg_bit(prcm, vc_pll_ten, ten1, addr);
#endif

#ifdef DEF_GET_PRCM_VC_PLL_STAT_DLOCK
def_get_mod_reg_bit(prcm, vc_pll_stat, dlock, addr);
#endif

#ifdef DEF_GET_PRCM_VC_PLL_STAT_ASTEADY
def_get_mod_reg_bit(prcm, vc_pll_stat, asteady, addr);
#endif

#ifdef DEF_GET_PRCM_VC_PLL_STAT_ALOCK
def_get_mod_reg_bit(prcm, vc_pll_stat, alock, addr);
#endif

#ifdef DEF_GET_PRCM_VC_PLL_MOD_MOD
def_get_mod_reg_bit(prcm, vc_pll_mod, mod, addr);
#endif

#ifdef DEF_GET_PRCM_VC_PLL_NFRAC_NFRAC
def_get_mod_reg_bit(prcm, vc_pll_nfrac, nfrac, addr);
#endif

#ifdef DEF_GET_PRCM_PERH1_PLL_EN_ENP
def_get_mod_reg_bit(prcm, perh1_pll_en, enp, addr);
#endif

#ifdef DEF_GET_PRCM_PERH1_PLL_EN_ENM
def_get_mod_reg_bit(prcm, perh1_pll_en, enm, addr);
#endif

#ifdef DEF_GET_PRCM_PERH1_PLL_EN_OEN
def_get_mod_reg_bit(prcm, perh1_pll_en, oen, addr);
#endif

#ifdef DEF_GET_PRCM_PERH1_PLL_EN_REN
def_get_mod_reg_bit(prcm, perh1_pll_en, ren, addr);
#endif

#ifdef DEF_GET_PRCM_PERH1_PLL_FAC_PRE_DIV
def_get_mod_reg_bit(prcm, perh1_pll_fac, pre_div, addr);
#endif

#ifdef DEF_GET_PRCM_PERH1_PLL_FAC_POST_DIV
def_get_mod_reg_bit(prcm, perh1_pll_fac, post_div, addr);
#endif

#ifdef DEF_GET_PRCM_PERH1_PLL_FAC_N
def_get_mod_reg_bit(prcm, perh1_pll_fac, n, addr);
#endif

#ifdef DEF_GET_PRCM_PERH1_PLL_TUNE0_TUNE1
def_get_mod_reg_bit(prcm, perh1_pll_tune0, tune1, addr);
#endif

#ifdef DEF_GET_PRCM_PERH1_PLL_TUNE0_TUNE2
def_get_mod_reg_bit(prcm, perh1_pll_tune0, tune2, addr);
#endif

#ifdef DEF_GET_PRCM_PERH1_PLL_TUNE0_TUNE3
def_get_mod_reg_bit(prcm, perh1_pll_tune0, tune3, addr);
#endif

#ifdef DEF_GET_PRCM_PERH1_PLL_TUNE0_TUNE4
def_get_mod_reg_bit(prcm, perh1_pll_tune0, tune4, addr);
#endif

#ifdef DEF_GET_PRCM_PERH1_PLL_TUNE0_TUNE5
def_get_mod_reg_bit(prcm, perh1_pll_tune0, tune5, addr);
#endif

#ifdef DEF_GET_PRCM_PERH1_PLL_TUNE0_TUNE6
def_get_mod_reg_bit(prcm, perh1_pll_tune0, tune6, addr);
#endif

#ifdef DEF_GET_PRCM_PERH1_PLL_TUNE0_TUNE7
def_get_mod_reg_bit(prcm, perh1_pll_tune0, tune7, addr);
#endif

#ifdef DEF_GET_PRCM_PERH1_PLL_TUNE0_TUNE8
def_get_mod_reg_bit(prcm, perh1_pll_tune0, tune8, addr);
#endif

#ifdef DEF_GET_PRCM_PERH1_PLL_TUNE0_TUNE9
def_get_mod_reg_bit(prcm, perh1_pll_tune0, tune9, addr);
#endif

#ifdef DEF_GET_PRCM_PERH1_PLL_TUNE0_TUNE10
def_get_mod_reg_bit(prcm, perh1_pll_tune0, tune10, addr);
#endif

#ifdef DEF_GET_PRCM_PERH1_PLL_TUNE0_TUNE11
def_get_mod_reg_bit(prcm, perh1_pll_tune0, tune11, addr);
#endif

#ifdef DEF_GET_PRCM_PERH1_PLL_TUNE0_TUNE12
def_get_mod_reg_bit(prcm, perh1_pll_tune0, tune12, addr);
#endif

#ifdef DEF_GET_PRCM_PERH1_PLL_TEN_TEN0
def_get_mod_reg_bit(prcm, perh1_pll_ten, ten0, addr);
#endif

#ifdef DEF_GET_PRCM_PERH1_PLL_TEN_TEN1
def_get_mod_reg_bit(prcm, perh1_pll_ten, ten1, addr);
#endif

#ifdef DEF_GET_PRCM_PERH1_PLL_STAT_DLOCK
def_get_mod_reg_bit(prcm, perh1_pll_stat, dlock, addr);
#endif

#ifdef DEF_GET_PRCM_PERH1_PLL_STAT_ASTEADY
def_get_mod_reg_bit(prcm, perh1_pll_stat, asteady, addr);
#endif

#ifdef DEF_GET_PRCM_PERH1_PLL_STAT_ALOCK
def_get_mod_reg_bit(prcm, perh1_pll_stat, alock, addr);
#endif

#ifdef DEF_GET_PRCM_PERH1_PLL_MOD_MOD
def_get_mod_reg_bit(prcm, perh1_pll_mod, mod, addr);
#endif

#ifdef DEF_GET_PRCM_PERH1_PLL_NFRAC_NFRAC
def_get_mod_reg_bit(prcm, perh1_pll_nfrac, nfrac, addr);
#endif

#ifdef DEF_GET_PRCM_PERH1_PLL_TUNE1_PERIOD
def_get_mod_reg_bit(prcm, perh1_pll_tune1, period, addr);
#endif

#ifdef DEF_GET_PRCM_PERH1_PLL_TUNE1_AMPLITUDE
def_get_mod_reg_bit(prcm, perh1_pll_tune1, amplitude, addr);
#endif

#ifdef DEF_GET_PRCM_DISP_PLL_EN_ENP
def_get_mod_reg_bit(prcm, disp_pll_en, enp, addr);
#endif

#ifdef DEF_GET_PRCM_DISP_PLL_EN_ENM
def_get_mod_reg_bit(prcm, disp_pll_en, enm, addr);
#endif

#ifdef DEF_GET_PRCM_DISP_PLL_EN_OEN
def_get_mod_reg_bit(prcm, disp_pll_en, oen, addr);
#endif

#ifdef DEF_GET_PRCM_DISP_PLL_EN_REN
def_get_mod_reg_bit(prcm, disp_pll_en, ren, addr);
#endif

#ifdef DEF_GET_PRCM_DISP_PLL_FAC_PRE_DIV
def_get_mod_reg_bit(prcm, disp_pll_fac, pre_div, addr);
#endif

#ifdef DEF_GET_PRCM_DISP_PLL_FAC_POST_DIV
def_get_mod_reg_bit(prcm, disp_pll_fac, post_div, addr);
#endif

#ifdef DEF_GET_PRCM_DISP_PLL_FAC_N
def_get_mod_reg_bit(prcm, disp_pll_fac, n, addr);
#endif

#ifdef DEF_GET_PRCM_DISP_PLL_TUNE0_TUNE1
def_get_mod_reg_bit(prcm, disp_pll_tune0, tune1, addr);
#endif

#ifdef DEF_GET_PRCM_DISP_PLL_TUNE0_TUNE2
def_get_mod_reg_bit(prcm, disp_pll_tune0, tune2, addr);
#endif

#ifdef DEF_GET_PRCM_DISP_PLL_TUNE0_TUNE3
def_get_mod_reg_bit(prcm, disp_pll_tune0, tune3, addr);
#endif

#ifdef DEF_GET_PRCM_DISP_PLL_TUNE0_TUNE4
def_get_mod_reg_bit(prcm, disp_pll_tune0, tune4, addr);
#endif

#ifdef DEF_GET_PRCM_DISP_PLL_TUNE0_TUNE5
def_get_mod_reg_bit(prcm, disp_pll_tune0, tune5, addr);
#endif

#ifdef DEF_GET_PRCM_DISP_PLL_TUNE0_TUNE6
def_get_mod_reg_bit(prcm, disp_pll_tune0, tune6, addr);
#endif

#ifdef DEF_GET_PRCM_DISP_PLL_TUNE0_TUNE7
def_get_mod_reg_bit(prcm, disp_pll_tune0, tune7, addr);
#endif

#ifdef DEF_GET_PRCM_DISP_PLL_TUNE0_TUNE8
def_get_mod_reg_bit(prcm, disp_pll_tune0, tune8, addr);
#endif

#ifdef DEF_GET_PRCM_DISP_PLL_TUNE0_TUNE9
def_get_mod_reg_bit(prcm, disp_pll_tune0, tune9, addr);
#endif

#ifdef DEF_GET_PRCM_DISP_PLL_TUNE0_TUNE10
def_get_mod_reg_bit(prcm, disp_pll_tune0, tune10, addr);
#endif

#ifdef DEF_GET_PRCM_DISP_PLL_TUNE0_TUNE11
def_get_mod_reg_bit(prcm, disp_pll_tune0, tune11, addr);
#endif

#ifdef DEF_GET_PRCM_DISP_PLL_TUNE0_TUNE12
def_get_mod_reg_bit(prcm, disp_pll_tune0, tune12, addr);
#endif

#ifdef DEF_GET_PRCM_DISP_PLL_TEN_TEN0
def_get_mod_reg_bit(prcm, disp_pll_ten, ten0, addr);
#endif

#ifdef DEF_GET_PRCM_DISP_PLL_TEN_TEN1
def_get_mod_reg_bit(prcm, disp_pll_ten, ten1, addr);
#endif

#ifdef DEF_GET_PRCM_DISP_PLL_STAT_DLOCK
def_get_mod_reg_bit(prcm, disp_pll_stat, dlock, addr);
#endif

#ifdef DEF_GET_PRCM_DISP_PLL_STAT_ASTEADY
def_get_mod_reg_bit(prcm, disp_pll_stat, asteady, addr);
#endif

#ifdef DEF_GET_PRCM_DISP_PLL_STAT_ALOCK
def_get_mod_reg_bit(prcm, disp_pll_stat, alock, addr);
#endif

#ifdef DEF_GET_PRCM_DISP_PLL_MOD_MOD
def_get_mod_reg_bit(prcm, disp_pll_mod, mod, addr);
#endif

#ifdef DEF_GET_PRCM_DISP_PLL_NFRAC_NFRAC
def_get_mod_reg_bit(prcm, disp_pll_nfrac, nfrac, addr);
#endif

#ifdef DEF_GET_PRCM_DISP_PLL_TUNE1_PERIOD
def_get_mod_reg_bit(prcm, disp_pll_tune1, period, addr);
#endif

#ifdef DEF_GET_PRCM_DISP_PLL_TUNE1_AMPLITUDE
def_get_mod_reg_bit(prcm, disp_pll_tune1, amplitude, addr);
#endif

#ifdef DEF_GET_PRCM_AX_PLL_EN_ENP
def_get_mod_reg_bit(prcm, ax_pll_en, enp, addr);
#endif

#ifdef DEF_GET_PRCM_AX_PLL_EN_ENM
def_get_mod_reg_bit(prcm, ax_pll_en, enm, addr);
#endif

#ifdef DEF_GET_PRCM_AX_PLL_EN_OEN
def_get_mod_reg_bit(prcm, ax_pll_en, oen, addr);
#endif

#ifdef DEF_GET_PRCM_AX_PLL_EN_REN
def_get_mod_reg_bit(prcm, ax_pll_en, ren, addr);
#endif

#ifdef DEF_GET_PRCM_AX_PLL_FAC_PRE_DIV
def_get_mod_reg_bit(prcm, ax_pll_fac, pre_div, addr);
#endif

#ifdef DEF_GET_PRCM_AX_PLL_FAC_POST_DIV
def_get_mod_reg_bit(prcm, ax_pll_fac, post_div, addr);
#endif

#ifdef DEF_GET_PRCM_AX_PLL_FAC_N
def_get_mod_reg_bit(prcm, ax_pll_fac, n, addr);
#endif

#ifdef DEF_GET_PRCM_AX_PLL_TUNE0_TUNE1
def_get_mod_reg_bit(prcm, ax_pll_tune0, tune1, addr);
#endif

#ifdef DEF_GET_PRCM_AX_PLL_TUNE0_TUNE2
def_get_mod_reg_bit(prcm, ax_pll_tune0, tune2, addr);
#endif

#ifdef DEF_GET_PRCM_AX_PLL_TUNE0_TUNE3
def_get_mod_reg_bit(prcm, ax_pll_tune0, tune3, addr);
#endif

#ifdef DEF_GET_PRCM_AX_PLL_TUNE0_TUNE4
def_get_mod_reg_bit(prcm, ax_pll_tune0, tune4, addr);
#endif

#ifdef DEF_GET_PRCM_AX_PLL_TUNE0_TUNE5
def_get_mod_reg_bit(prcm, ax_pll_tune0, tune5, addr);
#endif

#ifdef DEF_GET_PRCM_AX_PLL_TUNE0_TUNE6
def_get_mod_reg_bit(prcm, ax_pll_tune0, tune6, addr);
#endif

#ifdef DEF_GET_PRCM_AX_PLL_TUNE0_TUNE7
def_get_mod_reg_bit(prcm, ax_pll_tune0, tune7, addr);
#endif

#ifdef DEF_GET_PRCM_AX_PLL_TUNE0_TUNE8
def_get_mod_reg_bit(prcm, ax_pll_tune0, tune8, addr);
#endif

#ifdef DEF_GET_PRCM_AX_PLL_TUNE0_TUNE9
def_get_mod_reg_bit(prcm, ax_pll_tune0, tune9, addr);
#endif

#ifdef DEF_GET_PRCM_AX_PLL_TUNE0_TUNE10
def_get_mod_reg_bit(prcm, ax_pll_tune0, tune10, addr);
#endif

#ifdef DEF_GET_PRCM_AX_PLL_TUNE0_TUNE11
def_get_mod_reg_bit(prcm, ax_pll_tune0, tune11, addr);
#endif

#ifdef DEF_GET_PRCM_AX_PLL_TUNE0_TUNE12
def_get_mod_reg_bit(prcm, ax_pll_tune0, tune12, addr);
#endif

#ifdef DEF_GET_PRCM_AX_PLL_TEN_TEN0
def_get_mod_reg_bit(prcm, ax_pll_ten, ten0, addr);
#endif

#ifdef DEF_GET_PRCM_AX_PLL_TEN_TEN1
def_get_mod_reg_bit(prcm, ax_pll_ten, ten1, addr);
#endif

#ifdef DEF_GET_PRCM_AX_PLL_STAT_DLOCK
def_get_mod_reg_bit(prcm, ax_pll_stat, dlock, addr);
#endif

#ifdef DEF_GET_PRCM_AX_PLL_STAT_ASTEADY
def_get_mod_reg_bit(prcm, ax_pll_stat, asteady, addr);
#endif

#ifdef DEF_GET_PRCM_AX_PLL_STAT_ALOCK
def_get_mod_reg_bit(prcm, ax_pll_stat, alock, addr);
#endif

#ifdef DEF_GET_PRCM_AX_PLL_MOD_MOD
def_get_mod_reg_bit(prcm, ax_pll_mod, mod, addr);
#endif

#ifdef DEF_GET_PRCM_AX_PLL_NFRAC_NFRAC
def_get_mod_reg_bit(prcm, ax_pll_nfrac, nfrac, addr);
#endif

#ifdef DEF_GET_PRCM_AX_PLL_TUNE2_EN
def_get_mod_reg_bit(prcm, ax_pll_tune2, en, addr);
#endif

#ifdef DEF_GET_PRCM_AX_PLL_TUNE2_STEP
def_get_mod_reg_bit(prcm, ax_pll_tune2, step, addr);
#endif

#ifdef DEF_GET_PRCM_PERH2_PLL_EN_ENP
def_get_mod_reg_bit(prcm, perh2_pll_en, enp, addr);
#endif

#ifdef DEF_GET_PRCM_PERH2_PLL_EN_ENM
def_get_mod_reg_bit(prcm, perh2_pll_en, enm, addr);
#endif

#ifdef DEF_GET_PRCM_PERH2_PLL_EN_OEN
def_get_mod_reg_bit(prcm, perh2_pll_en, oen, addr);
#endif

#ifdef DEF_GET_PRCM_PERH2_PLL_EN_REN
def_get_mod_reg_bit(prcm, perh2_pll_en, ren, addr);
#endif

#ifdef DEF_GET_PRCM_PERH2_PLL_FAC_PRE_DIV
def_get_mod_reg_bit(prcm, perh2_pll_fac, pre_div, addr);
#endif

#ifdef DEF_GET_PRCM_PERH2_PLL_FAC_POST_DIV
def_get_mod_reg_bit(prcm, perh2_pll_fac, post_div, addr);
#endif

#ifdef DEF_GET_PRCM_PERH2_PLL_FAC_N
def_get_mod_reg_bit(prcm, perh2_pll_fac, n, addr);
#endif

#ifdef DEF_GET_PRCM_PERH2_PLL_TUNE0_TUNE1
def_get_mod_reg_bit(prcm, perh2_pll_tune0, tune1, addr);
#endif

#ifdef DEF_GET_PRCM_PERH2_PLL_TUNE0_TUNE2
def_get_mod_reg_bit(prcm, perh2_pll_tune0, tune2, addr);
#endif

#ifdef DEF_GET_PRCM_PERH2_PLL_TUNE0_TUNE3
def_get_mod_reg_bit(prcm, perh2_pll_tune0, tune3, addr);
#endif

#ifdef DEF_GET_PRCM_PERH2_PLL_TUNE0_TUNE4
def_get_mod_reg_bit(prcm, perh2_pll_tune0, tune4, addr);
#endif

#ifdef DEF_GET_PRCM_PERH2_PLL_TUNE0_TUNE5
def_get_mod_reg_bit(prcm, perh2_pll_tune0, tune5, addr);
#endif

#ifdef DEF_GET_PRCM_PERH2_PLL_TUNE0_TUNE6
def_get_mod_reg_bit(prcm, perh2_pll_tune0, tune6, addr);
#endif

#ifdef DEF_GET_PRCM_PERH2_PLL_TUNE0_TUNE7
def_get_mod_reg_bit(prcm, perh2_pll_tune0, tune7, addr);
#endif

#ifdef DEF_GET_PRCM_PERH2_PLL_TUNE0_TUNE8
def_get_mod_reg_bit(prcm, perh2_pll_tune0, tune8, addr);
#endif

#ifdef DEF_GET_PRCM_PERH2_PLL_TUNE0_TUNE9
def_get_mod_reg_bit(prcm, perh2_pll_tune0, tune9, addr);
#endif

#ifdef DEF_GET_PRCM_PERH2_PLL_TUNE0_TUNE10
def_get_mod_reg_bit(prcm, perh2_pll_tune0, tune10, addr);
#endif

#ifdef DEF_GET_PRCM_PERH2_PLL_TUNE0_TUNE11
def_get_mod_reg_bit(prcm, perh2_pll_tune0, tune11, addr);
#endif

#ifdef DEF_GET_PRCM_PERH2_PLL_TUNE0_TUNE12
def_get_mod_reg_bit(prcm, perh2_pll_tune0, tune12, addr);
#endif

#ifdef DEF_GET_PRCM_PERH2_PLL_TEN_TEN0
def_get_mod_reg_bit(prcm, perh2_pll_ten, ten0, addr);
#endif

#ifdef DEF_GET_PRCM_PERH2_PLL_TEN_TEN1
def_get_mod_reg_bit(prcm, perh2_pll_ten, ten1, addr);
#endif

#ifdef DEF_GET_PRCM_PERH2_PLL_STAT_DLOCK
def_get_mod_reg_bit(prcm, perh2_pll_stat, dlock, addr);
#endif

#ifdef DEF_GET_PRCM_PERH2_PLL_STAT_ASTEADY
def_get_mod_reg_bit(prcm, perh2_pll_stat, asteady, addr);
#endif

#ifdef DEF_GET_PRCM_PERH2_PLL_STAT_ALOCK
def_get_mod_reg_bit(prcm, perh2_pll_stat, alock, addr);
#endif

#ifdef DEF_GET_PRCM_PERH2_PLL_MOD_MOD
def_get_mod_reg_bit(prcm, perh2_pll_mod, mod, addr);
#endif

#ifdef DEF_GET_PRCM_PERH2_PLL_NFRAC_NFRAC
def_get_mod_reg_bit(prcm, perh2_pll_nfrac, nfrac, addr);
#endif

#ifdef DEF_GET_PRCM_PERH2_PLL_TUNE1_PERIOD
def_get_mod_reg_bit(prcm, perh2_pll_tune1, period, addr);
#endif

#ifdef DEF_GET_PRCM_PERH2_PLL_TUNE1_AMPLITUDE
def_get_mod_reg_bit(prcm, perh2_pll_tune1, amplitude, addr);
#endif

#ifdef DEF_GET_PRCM_PLL_TBLK_EN
def_get_mod_reg_bit(prcm, pll_tblk, en, addr);
#endif

#ifdef DEF_GET_PRCM_PLL_TBLK_DIV_ENDIO
def_get_mod_reg_bit(prcm, pll_tblk, div_endio, addr);
#endif

#ifdef DEF_GET_PRCM_PLL_TBLK_SEL0
def_get_mod_reg_bit(prcm, pll_tblk, sel0, addr);
#endif

#ifdef DEF_GET_PRCM_PLL_TBLK_SEL1
def_get_mod_reg_bit(prcm, pll_tblk, sel1, addr);
#endif

#ifdef DEF_GET_PRCM_PLL_TBLK_DIV0
def_get_mod_reg_bit(prcm, pll_tblk, div0, addr);
#endif

#ifdef DEF_GET_PRCM_CPU_AXI_CLK_CTRL_CPU_SRC_SEL
def_get_mod_reg_bit(prcm, cpu_axi_clk_ctrl, cpu_src_sel, addr);
#endif

#ifdef DEF_GET_PRCM_CPU_AXI_CLK_CTRL_CPU_SRC_DIV
def_get_mod_reg_bit(prcm, cpu_axi_clk_ctrl, cpu_src_div, addr);
#endif

#ifdef DEF_GET_PRCM_AHB_APB_CLK_CTRL_SRC_SEL
def_get_mod_reg_bit(prcm, ahb_apb_clk_ctrl, src_sel, addr);
#endif

#ifdef DEF_GET_PRCM_AHB_APB_CLK_CTRL_AHB_DIV
def_get_mod_reg_bit(prcm, ahb_apb_clk_ctrl, ahb_div, addr);
#endif

#ifdef DEF_GET_PRCM_MAXI_CLK_CTRL_SRC_SEL
def_get_mod_reg_bit(prcm, maxi_clk_ctrl, src_sel, addr);
#endif

#ifdef DEF_GET_PRCM_MAXI_CLK_CTRL_DIV
def_get_mod_reg_bit(prcm, maxi_clk_ctrl, div, addr);
#endif

#ifdef DEF_GET_PRCM_MAXI_CLK_CTRL_EN
def_get_mod_reg_bit(prcm, maxi_clk_ctrl, en, addr);
#endif

#ifdef DEF_GET_PRCM_SDRAM_CLK_CTRL_SRC_SEL
def_get_mod_reg_bit(prcm, sdram_clk_ctrl, src_sel, addr);
#endif

#ifdef DEF_GET_PRCM_SDRAM_CLK_CTRL_DIV
def_get_mod_reg_bit(prcm, sdram_clk_ctrl, div, addr);
#endif

#ifdef DEF_GET_PRCM_SDRAM_CLK_CTRL_EN
def_get_mod_reg_bit(prcm, sdram_clk_ctrl, en, addr);
#endif

#ifdef DEF_GET_PRCM_SDRAM_RST_SDRAMC
def_get_mod_reg_bit(prcm, sdram_rst, sdramc, addr);
#endif

#ifdef DEF_GET_PRCM_SDRAM_RST_PUB
def_get_mod_reg_bit(prcm, sdram_rst, pub, addr);
#endif

#ifdef DEF_GET_PRCM_SDRAM_BM_CLK_CTRL_SRC_SEL
def_get_mod_reg_bit(prcm, sdram_bm_clk_ctrl, src_sel, addr);
#endif

#ifdef DEF_GET_PRCM_SDRAM_BM_CLK_CTRL_EN
def_get_mod_reg_bit(prcm, sdram_bm_clk_ctrl, en, addr);
#endif

#ifdef DEF_GET_PRCM_SDC0_CLK_CTRL_SRC_SEL
def_get_mod_reg_bit(prcm, sdc0_clk_ctrl, src_sel, addr);
#endif

#ifdef DEF_GET_PRCM_SDC0_CLK_CTRL_DIV0
def_get_mod_reg_bit(prcm, sdc0_clk_ctrl, div0, addr);
#endif

#ifdef DEF_GET_PRCM_SDC0_CLK_CTRL_DIV1
def_get_mod_reg_bit(prcm, sdc0_clk_ctrl, div1, addr);
#endif

#ifdef DEF_GET_PRCM_SDC0_CLK_CTRL_EN
def_get_mod_reg_bit(prcm, sdc0_clk_ctrl, en, addr);
#endif

#ifdef DEF_GET_PRCM_SDC1_CLK_CTRL_SRC_SEL
def_get_mod_reg_bit(prcm, sdc1_clk_ctrl, src_sel, addr);
#endif

#ifdef DEF_GET_PRCM_SDC1_CLK_CTRL_DIV0
def_get_mod_reg_bit(prcm, sdc1_clk_ctrl, div0, addr);
#endif

#ifdef DEF_GET_PRCM_SDC1_CLK_CTRL_DIV1
def_get_mod_reg_bit(prcm, sdc1_clk_ctrl, div1, addr);
#endif

#ifdef DEF_GET_PRCM_SDC1_CLK_CTRL_EN
def_get_mod_reg_bit(prcm, sdc1_clk_ctrl, en, addr);
#endif

#ifdef DEF_GET_PRCM_SDC2_CLK_CTRL_SRC_SEL
def_get_mod_reg_bit(prcm, sdc2_clk_ctrl, src_sel, addr);
#endif

#ifdef DEF_GET_PRCM_SDC2_CLK_CTRL_DIV0
def_get_mod_reg_bit(prcm, sdc2_clk_ctrl, div0, addr);
#endif

#ifdef DEF_GET_PRCM_SDC2_CLK_CTRL_DIV1
def_get_mod_reg_bit(prcm, sdc2_clk_ctrl, div1, addr);
#endif

#ifdef DEF_GET_PRCM_SDC2_CLK_CTRL_EN
def_get_mod_reg_bit(prcm, sdc2_clk_ctrl, en, addr);
#endif

#ifdef DEF_GET_PRCM_SPI0_CLK_CTRL_SRC_SEL
def_get_mod_reg_bit(prcm, spi0_clk_ctrl, src_sel, addr);
#endif

#ifdef DEF_GET_PRCM_SPI0_CLK_CTRL_DIV0
def_get_mod_reg_bit(prcm, spi0_clk_ctrl, div0, addr);
#endif

#ifdef DEF_GET_PRCM_SPI0_CLK_CTRL_DIV1
def_get_mod_reg_bit(prcm, spi0_clk_ctrl, div1, addr);
#endif

#ifdef DEF_GET_PRCM_SPI0_CLK_CTRL_EN
def_get_mod_reg_bit(prcm, spi0_clk_ctrl, en, addr);
#endif

#ifdef DEF_GET_PRCM_SPI1_CLK_CTRL_SRC_SEL
def_get_mod_reg_bit(prcm, spi1_clk_ctrl, src_sel, addr);
#endif

#ifdef DEF_GET_PRCM_SPI1_CLK_CTRL_DIV0
def_get_mod_reg_bit(prcm, spi1_clk_ctrl, div0, addr);
#endif

#ifdef DEF_GET_PRCM_SPI1_CLK_CTRL_DIV1
def_get_mod_reg_bit(prcm, spi1_clk_ctrl, div1, addr);
#endif

#ifdef DEF_GET_PRCM_SPI1_CLK_CTRL_EN
def_get_mod_reg_bit(prcm, spi1_clk_ctrl, en, addr);
#endif

#ifdef DEF_GET_PRCM_SPI2_CLK_CTRL_SRC_SEL
def_get_mod_reg_bit(prcm, spi2_clk_ctrl, src_sel, addr);
#endif

#ifdef DEF_GET_PRCM_SPI2_CLK_CTRL_DIV0
def_get_mod_reg_bit(prcm, spi2_clk_ctrl, div0, addr);
#endif

#ifdef DEF_GET_PRCM_SPI2_CLK_CTRL_DIV1
def_get_mod_reg_bit(prcm, spi2_clk_ctrl, div1, addr);
#endif

#ifdef DEF_GET_PRCM_SPI2_CLK_CTRL_EN
def_get_mod_reg_bit(prcm, spi2_clk_ctrl, en, addr);
#endif

#ifdef DEF_GET_PRCM_VC_ENC_CLK_CTRL_SRC_SEL
def_get_mod_reg_bit(prcm, vc_enc_clk_ctrl, src_sel, addr);
#endif

#ifdef DEF_GET_PRCM_VC_ENC_CLK_CTRL_DIV0
def_get_mod_reg_bit(prcm, vc_enc_clk_ctrl, div0, addr);
#endif

#ifdef DEF_GET_PRCM_VC_ENC_CLK_CTRL_EN
def_get_mod_reg_bit(prcm, vc_enc_clk_ctrl, en, addr);
#endif

#ifdef DEF_GET_PRCM_VC_DEC_CLK_CTRL_SRC_SEL
def_get_mod_reg_bit(prcm, vc_dec_clk_ctrl, src_sel, addr);
#endif

#ifdef DEF_GET_PRCM_VC_DEC_CLK_CTRL_DIV0
def_get_mod_reg_bit(prcm, vc_dec_clk_ctrl, div0, addr);
#endif

#ifdef DEF_GET_PRCM_VC_DEC_CLK_CTRL_EN
def_get_mod_reg_bit(prcm, vc_dec_clk_ctrl, en, addr);
#endif

#ifdef DEF_GET_PRCM_VISS_CLK_CTRL_SCLK0_EN
def_get_mod_reg_bit(prcm, viss_clk_ctrl, sclk0_en, addr);
#endif

#ifdef DEF_GET_PRCM_VISS_CLK_CTRL_SCLK1_EN
def_get_mod_reg_bit(prcm, viss_clk_ctrl, sclk1_en, addr);
#endif

#ifdef DEF_GET_PRCM_VISS_CLK_CTRL_SCLK2_EN
def_get_mod_reg_bit(prcm, viss_clk_ctrl, sclk2_en, addr);
#endif

#ifdef DEF_GET_PRCM_DPU_SCLK0_CLK_CTRL_SRC_SEL
def_get_mod_reg_bit(prcm, dpu_sclk0_clk_ctrl, src_sel, addr);
#endif

#ifdef DEF_GET_PRCM_DPU_SCLK0_CLK_CTRL_DIV
def_get_mod_reg_bit(prcm, dpu_sclk0_clk_ctrl, div, addr);
#endif

#ifdef DEF_GET_PRCM_DPU_SCLK0_CLK_CTRL_EN
def_get_mod_reg_bit(prcm, dpu_sclk0_clk_ctrl, en, addr);
#endif

#ifdef DEF_GET_PRCM_DPU_SCLK1_CLK_CTRL_SRC_SEL
def_get_mod_reg_bit(prcm, dpu_sclk1_clk_ctrl, src_sel, addr);
#endif

#ifdef DEF_GET_PRCM_DPU_SCLK1_CLK_CTRL_DIV
def_get_mod_reg_bit(prcm, dpu_sclk1_clk_ctrl, div, addr);
#endif

#ifdef DEF_GET_PRCM_DPU_SCLK1_CLK_CTRL_EN
def_get_mod_reg_bit(prcm, dpu_sclk1_clk_ctrl, en, addr);
#endif

#ifdef DEF_GET_PRCM_DPU_SCLK2_CLK_CTRL_EN
def_get_mod_reg_bit(prcm, dpu_sclk2_clk_ctrl, en, addr);
#endif

#ifdef DEF_GET_PRCM_DOSS_CLK_CTRL_HFEOSC_EN
def_get_mod_reg_bit(prcm, doss_clk_ctrl, hfeosc_en, addr);
#endif

#ifdef DEF_GET_PRCM_DOSS_CLK_CTRL_SCLK0_EN
def_get_mod_reg_bit(prcm, doss_clk_ctrl, sclk0_en, addr);
#endif

#ifdef DEF_GET_PRCM_AX_CLK_CTRL_SRC_SEL
def_get_mod_reg_bit(prcm, ax_clk_ctrl, src_sel, addr);
#endif

#ifdef DEF_GET_PRCM_AX_CLK_CTRL_DIV
def_get_mod_reg_bit(prcm, ax_clk_ctrl, div, addr);
#endif

#ifdef DEF_GET_PRCM_AX_CLK_CTRL_EN
def_get_mod_reg_bit(prcm, ax_clk_ctrl, en, addr);
#endif

#ifdef DEF_GET_PRCM_I2S0_CLK_CTRL_SRC_SEL
def_get_mod_reg_bit(prcm, i2s0_clk_ctrl, src_sel, addr);
#endif

#ifdef DEF_GET_PRCM_I2S0_CLK_CTRL_DIV
def_get_mod_reg_bit(prcm, i2s0_clk_ctrl, div, addr);
#endif

#ifdef DEF_GET_PRCM_I2S0_CLK_CTRL_EN
def_get_mod_reg_bit(prcm, i2s0_clk_ctrl, en, addr);
#endif

#ifdef DEF_GET_PRCM_I2S1_CLK_CTRL_SRC_SEL
def_get_mod_reg_bit(prcm, i2s1_clk_ctrl, src_sel, addr);
#endif

#ifdef DEF_GET_PRCM_I2S1_CLK_CTRL_DIV
def_get_mod_reg_bit(prcm, i2s1_clk_ctrl, div, addr);
#endif

#ifdef DEF_GET_PRCM_I2S1_CLK_CTRL_EN
def_get_mod_reg_bit(prcm, i2s1_clk_ctrl, en, addr);
#endif

#ifdef DEF_GET_PRCM_I2C0_CLK_CTRL_SRC_SEL
def_get_mod_reg_bit(prcm, i2c0_clk_ctrl, src_sel, addr);
#endif

#ifdef DEF_GET_PRCM_I2C0_CLK_CTRL_DIV
def_get_mod_reg_bit(prcm, i2c0_clk_ctrl, div, addr);
#endif

#ifdef DEF_GET_PRCM_I2C0_CLK_CTRL_EN
def_get_mod_reg_bit(prcm, i2c0_clk_ctrl, en, addr);
#endif

#ifdef DEF_GET_PRCM_I2C1_CLK_CTRL_SRC_SEL
def_get_mod_reg_bit(prcm, i2c1_clk_ctrl, src_sel, addr);
#endif

#ifdef DEF_GET_PRCM_I2C1_CLK_CTRL_DIV
def_get_mod_reg_bit(prcm, i2c1_clk_ctrl, div, addr);
#endif

#ifdef DEF_GET_PRCM_I2C1_CLK_CTRL_EN
def_get_mod_reg_bit(prcm, i2c1_clk_ctrl, en, addr);
#endif

#ifdef DEF_GET_PRCM_I2C2_CLK_CTRL_SRC_SEL
def_get_mod_reg_bit(prcm, i2c2_clk_ctrl, src_sel, addr);
#endif

#ifdef DEF_GET_PRCM_I2C2_CLK_CTRL_DIV
def_get_mod_reg_bit(prcm, i2c2_clk_ctrl, div, addr);
#endif

#ifdef DEF_GET_PRCM_I2C2_CLK_CTRL_EN
def_get_mod_reg_bit(prcm, i2c2_clk_ctrl, en, addr);
#endif

#ifdef DEF_GET_PRCM_I2C3_CLK_CTRL_SRC_SEL
def_get_mod_reg_bit(prcm, i2c3_clk_ctrl, src_sel, addr);
#endif

#ifdef DEF_GET_PRCM_I2C3_CLK_CTRL_DIV
def_get_mod_reg_bit(prcm, i2c3_clk_ctrl, div, addr);
#endif

#ifdef DEF_GET_PRCM_I2C3_CLK_CTRL_EN
def_get_mod_reg_bit(prcm, i2c3_clk_ctrl, en, addr);
#endif

#ifdef DEF_GET_PRCM_UART0_CLK_CTRL_SRC_SEL
def_get_mod_reg_bit(prcm, uart0_clk_ctrl, src_sel, addr);
#endif

#ifdef DEF_GET_PRCM_UART0_CLK_CTRL_DIV
def_get_mod_reg_bit(prcm, uart0_clk_ctrl, div, addr);
#endif

#ifdef DEF_GET_PRCM_UART0_CLK_CTRL_EN
def_get_mod_reg_bit(prcm, uart0_clk_ctrl, en, addr);
#endif

#ifdef DEF_GET_PRCM_UART1_CLK_CTRL_SRC_SEL
def_get_mod_reg_bit(prcm, uart1_clk_ctrl, src_sel, addr);
#endif

#ifdef DEF_GET_PRCM_UART1_CLK_CTRL_DIV
def_get_mod_reg_bit(prcm, uart1_clk_ctrl, div, addr);
#endif

#ifdef DEF_GET_PRCM_UART1_CLK_CTRL_EN
def_get_mod_reg_bit(prcm, uart1_clk_ctrl, en, addr);
#endif

#ifdef DEF_GET_PRCM_UART2_CLK_CTRL_SRC_SEL
def_get_mod_reg_bit(prcm, uart2_clk_ctrl, src_sel, addr);
#endif

#ifdef DEF_GET_PRCM_UART2_CLK_CTRL_DIV
def_get_mod_reg_bit(prcm, uart2_clk_ctrl, div, addr);
#endif

#ifdef DEF_GET_PRCM_UART2_CLK_CTRL_EN
def_get_mod_reg_bit(prcm, uart2_clk_ctrl, en, addr);
#endif

#ifdef DEF_GET_PRCM_UART3_CLK_CTRL_SRC_SEL
def_get_mod_reg_bit(prcm, uart3_clk_ctrl, src_sel, addr);
#endif

#ifdef DEF_GET_PRCM_UART3_CLK_CTRL_DIV
def_get_mod_reg_bit(prcm, uart3_clk_ctrl, div, addr);
#endif

#ifdef DEF_GET_PRCM_UART3_CLK_CTRL_EN
def_get_mod_reg_bit(prcm, uart3_clk_ctrl, en, addr);
#endif

#ifdef DEF_GET_PRCM_USB_PHY_RST_RST
def_get_mod_reg_bit(prcm, usb_phy_rst, rst, addr);
#endif

#ifdef DEF_GET_PRCM_GPADC_CLK_CTRL_SRC_SEL
def_get_mod_reg_bit(prcm, gpadc_clk_ctrl, src_sel, addr);
#endif

#ifdef DEF_GET_PRCM_GPADC_CLK_CTRL_DIV
def_get_mod_reg_bit(prcm, gpadc_clk_ctrl, div, addr);
#endif

#ifdef DEF_GET_PRCM_GPADC_CLK_CTRL_EN
def_get_mod_reg_bit(prcm, gpadc_clk_ctrl, en, addr);
#endif

#ifdef DEF_GET_PRCM_IR_CLK_CTRL_EN
def_get_mod_reg_bit(prcm, ir_clk_ctrl, en, addr);
#endif

#ifdef DEF_GET_PRCM_GMAC_CLK_CTRL_CORE_SRC_SEL
def_get_mod_reg_bit(prcm, gmac_clk_ctrl, core_src_sel, addr);
#endif

#ifdef DEF_GET_PRCM_GMAC_CLK_CTRL_CORE_DIV
def_get_mod_reg_bit(prcm, gmac_clk_ctrl, core_div, addr);
#endif

#ifdef DEF_GET_PRCM_GMAC_CLK_CTRL_CORE_EN
def_get_mod_reg_bit(prcm, gmac_clk_ctrl, core_en, addr);
#endif

#ifdef DEF_GET_PRCM_GMAC_CLK_CTRL_SCLK_SRC_SEL
def_get_mod_reg_bit(prcm, gmac_clk_ctrl, sclk_src_sel, addr);
#endif

#ifdef DEF_GET_PRCM_GMAC_CLK_CTRL_SCLK1_EN
def_get_mod_reg_bit(prcm, gmac_clk_ctrl, sclk1_en, addr);
#endif

#ifdef DEF_GET_PRCM_GMAC_CLK_CTRL_SCLK0_EN
def_get_mod_reg_bit(prcm, gmac_clk_ctrl, sclk0_en, addr);
#endif

#ifdef DEF_GET_PRCM_MBIST_CLK_CTRL_SRC_SEL
def_get_mod_reg_bit(prcm, mbist_clk_ctrl, src_sel, addr);
#endif

#ifdef DEF_GET_PRCM_MBIST_CLK_CTRL_DIV
def_get_mod_reg_bit(prcm, mbist_clk_ctrl, div, addr);
#endif

#ifdef DEF_GET_PRCM_MBIST_CLK_CTRL_EN
def_get_mod_reg_bit(prcm, mbist_clk_ctrl, en, addr);
#endif

#ifdef DEF_GET_PRCM_AES_CLK_CTRL_EN
def_get_mod_reg_bit(prcm, aes_clk_ctrl, en, addr);
#endif

#endif /* ___PRCM__GET___H___ */
