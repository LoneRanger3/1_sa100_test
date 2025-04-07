/* prcm_set.h */

#ifndef ___PRCM__SET___H___
#define ___PRCM__SET___H___

#ifdef DEF_SET_PRCM_VER
void set_prcm_ver(u32 reg_addr,
		u32 ver_l,
		u32 ver_h,
		u32 comp,
		u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_VER_VER_L
def_set_mod_reg_bit(prcm, ver, ver_l, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_VER_VER_H
def_set_mod_reg_bit(prcm, ver, ver_h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_VER_COMP
def_set_mod_reg_bit(prcm, ver, comp, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_GAT0
void set_prcm_ahb_gat0(u32 reg_addr,
			u32 dma,
			u32 ptimer,
			u32 aes,
			u32 sdram,
			u32 sdc0,
			u32 sdc1,
			u32 sdc2,
			u32 spi0,
			u32 spi1,
			u32 spi2,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_GAT0_DMA
def_set_mod_reg_bit(prcm, ahb_gat0, dma, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_GAT0_PTIMER
def_set_mod_reg_bit(prcm, ahb_gat0, ptimer, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_GAT0_AES
def_set_mod_reg_bit(prcm, ahb_gat0, aes, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_GAT0_SDRAM
def_set_mod_reg_bit(prcm, ahb_gat0, sdram, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_GAT0_SDC0
def_set_mod_reg_bit(prcm, ahb_gat0, sdc0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_GAT0_SDC1
def_set_mod_reg_bit(prcm, ahb_gat0, sdc1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_GAT0_SDC2
def_set_mod_reg_bit(prcm, ahb_gat0, sdc2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_GAT0_SPI0
def_set_mod_reg_bit(prcm, ahb_gat0, spi0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_GAT0_SPI1
def_set_mod_reg_bit(prcm, ahb_gat0, spi1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_GAT0_SPI2
def_set_mod_reg_bit(prcm, ahb_gat0, spi2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_GAT1
void set_prcm_ahb_gat1(u32 reg_addr,
			u32 viss,
			u32 dpu,
			u32 doss,
			u32 vc_dec,
			u32 vc_enc,
			u32 ax,
			u32 usb,
			u32 gmac,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_GAT1_VISS
def_set_mod_reg_bit(prcm, ahb_gat1, viss, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_GAT1_DPU
def_set_mod_reg_bit(prcm, ahb_gat1, dpu, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_GAT1_DOSS
def_set_mod_reg_bit(prcm, ahb_gat1, doss, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_GAT1_VC_DEC
def_set_mod_reg_bit(prcm, ahb_gat1, vc_dec, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_GAT1_VC_ENC
def_set_mod_reg_bit(prcm, ahb_gat1, vc_enc, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_GAT1_AX
def_set_mod_reg_bit(prcm, ahb_gat1, ax, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_GAT1_USB
def_set_mod_reg_bit(prcm, ahb_gat1, usb, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_GAT1_GMAC
def_set_mod_reg_bit(prcm, ahb_gat1, gmac, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_RST0
void set_prcm_ahb_rst0(u32 reg_addr,
			u32 dma,
			u32 ptimer,
			u32 aes,
			u32 sdram,
			u32 sdc0,
			u32 sdc1,
			u32 sdc2,
			u32 spi0,
			u32 spi1,
			u32 spi2,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_RST0_DMA
def_set_mod_reg_bit(prcm, ahb_rst0, dma, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_RST0_PTIMER
def_set_mod_reg_bit(prcm, ahb_rst0, ptimer, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_RST0_AES
def_set_mod_reg_bit(prcm, ahb_rst0, aes, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_RST0_SDRAM
def_set_mod_reg_bit(prcm, ahb_rst0, sdram, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_RST0_SDC0
def_set_mod_reg_bit(prcm, ahb_rst0, sdc0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_RST0_SDC1
def_set_mod_reg_bit(prcm, ahb_rst0, sdc1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_RST0_SDC2
def_set_mod_reg_bit(prcm, ahb_rst0, sdc2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_RST0_SPI0
def_set_mod_reg_bit(prcm, ahb_rst0, spi0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_RST0_SPI1
def_set_mod_reg_bit(prcm, ahb_rst0, spi1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_RST0_SPI2
def_set_mod_reg_bit(prcm, ahb_rst0, spi2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_RST1
void set_prcm_ahb_rst1(u32 reg_addr,
			u32 viss,
			u32 dpu,
			u32 doss,
			u32 vc_dec,
			u32 vc_enc,
			u32 ax,
			u32 usb,
			u32 gmac,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_RST1_VISS
def_set_mod_reg_bit(prcm, ahb_rst1, viss, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_RST1_DPU
def_set_mod_reg_bit(prcm, ahb_rst1, dpu, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_RST1_DOSS
def_set_mod_reg_bit(prcm, ahb_rst1, doss, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_RST1_VC_DEC
def_set_mod_reg_bit(prcm, ahb_rst1, vc_dec, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_RST1_VC_ENC
def_set_mod_reg_bit(prcm, ahb_rst1, vc_enc, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_RST1_AX
def_set_mod_reg_bit(prcm, ahb_rst1, ax, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_RST1_USB
def_set_mod_reg_bit(prcm, ahb_rst1, usb, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_RST1_GMAC
def_set_mod_reg_bit(prcm, ahb_rst1, gmac, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_APB_GAT0
void set_prcm_apb_gat0(u32 reg_addr,
			u32 i2c0,
			u32 i2c1,
			u32 i2c2,
			u32 i2c3,
			u32 uart0,
			u32 uart1,
			u32 uart2,
			u32 uart3,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_APB_GAT0_I2C0
def_set_mod_reg_bit(prcm, apb_gat0, i2c0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_APB_GAT0_I2C1
def_set_mod_reg_bit(prcm, apb_gat0, i2c1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_APB_GAT0_I2C2
def_set_mod_reg_bit(prcm, apb_gat0, i2c2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_APB_GAT0_I2C3
def_set_mod_reg_bit(prcm, apb_gat0, i2c3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_APB_GAT0_UART0
def_set_mod_reg_bit(prcm, apb_gat0, uart0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_APB_GAT0_UART1
def_set_mod_reg_bit(prcm, apb_gat0, uart1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_APB_GAT0_UART2
def_set_mod_reg_bit(prcm, apb_gat0, uart2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_APB_GAT0_UART3
def_set_mod_reg_bit(prcm, apb_gat0, uart3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_APB_RST0
void set_prcm_apb_rst0(u32 reg_addr,
			u32 i2c0,
			u32 i2c1,
			u32 i2c2,
			u32 i2c3,
			u32 uart0,
			u32 uart1,
			u32 uart2,
			u32 uart3,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_APB_RST0_I2C0
def_set_mod_reg_bit(prcm, apb_rst0, i2c0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_APB_RST0_I2C1
def_set_mod_reg_bit(prcm, apb_rst0, i2c1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_APB_RST0_I2C2
def_set_mod_reg_bit(prcm, apb_rst0, i2c2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_APB_RST0_I2C3
def_set_mod_reg_bit(prcm, apb_rst0, i2c3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_APB_RST0_UART0
def_set_mod_reg_bit(prcm, apb_rst0, uart0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_APB_RST0_UART1
def_set_mod_reg_bit(prcm, apb_rst0, uart1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_APB_RST0_UART2
def_set_mod_reg_bit(prcm, apb_rst0, uart2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_APB_RST0_UART3
def_set_mod_reg_bit(prcm, apb_rst0, uart3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_APB_GAT1
void set_prcm_apb_gat1(u32 reg_addr,
			u32 i2s0,
			u32 i2s1,
			u32 gpadc,
			u32 ir,
			u32 gpio,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_APB_GAT1_I2S0
def_set_mod_reg_bit(prcm, apb_gat1, i2s0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_APB_GAT1_I2S1
def_set_mod_reg_bit(prcm, apb_gat1, i2s1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_APB_GAT1_GPADC
def_set_mod_reg_bit(prcm, apb_gat1, gpadc, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_APB_GAT1_IR
def_set_mod_reg_bit(prcm, apb_gat1, ir, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_APB_GAT1_GPIO
def_set_mod_reg_bit(prcm, apb_gat1, gpio, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_APB_RST1
void set_prcm_apb_rst1(u32 reg_addr,
			u32 i2s0,
			u32 i2s1,
			u32 gpadc,
			u32 ir,
			u32 gpio,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_APB_RST1_I2S0
def_set_mod_reg_bit(prcm, apb_rst1, i2s0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_APB_RST1_I2S1
def_set_mod_reg_bit(prcm, apb_rst1, i2s1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_APB_RST1_GPADC
def_set_mod_reg_bit(prcm, apb_rst1, gpadc, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_APB_RST1_IR
def_set_mod_reg_bit(prcm, apb_rst1, ir, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_APB_RST1_GPIO
def_set_mod_reg_bit(prcm, apb_rst1, gpio, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AXI_GAT
void set_prcm_axi_gat(u32 reg_addr,
			u32 dma,
			u32 vc_enc,
			u32 vc_dec,
			u32 viss0,
			u32 viss1,
			u32 dpu,
			u32 doss,
			u32 ax,
			u32 gmac,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_AXI_GAT_DMA
def_set_mod_reg_bit(prcm, axi_gat, dma, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AXI_GAT_VC_ENC
def_set_mod_reg_bit(prcm, axi_gat, vc_enc, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AXI_GAT_VC_DEC
def_set_mod_reg_bit(prcm, axi_gat, vc_dec, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AXI_GAT_VISS0
def_set_mod_reg_bit(prcm, axi_gat, viss0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AXI_GAT_VISS1
def_set_mod_reg_bit(prcm, axi_gat, viss1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AXI_GAT_DPU
def_set_mod_reg_bit(prcm, axi_gat, dpu, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AXI_GAT_DOSS
def_set_mod_reg_bit(prcm, axi_gat, doss, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AXI_GAT_AX
def_set_mod_reg_bit(prcm, axi_gat, ax, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AXI_GAT_GMAC
def_set_mod_reg_bit(prcm, axi_gat, gmac, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_HFEOSC_CTRL
void set_prcm_hfeosc_ctrl(u32 reg_addr,
			u32 en,
			u32 src_sel,
			u32 xtal_start,
			u32 tune,
			u32 src_sel_dbg,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_HFEOSC_CTRL_EN
def_set_mod_reg_bit(prcm, hfeosc_ctrl, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_HFEOSC_CTRL_SRC_SEL
def_set_mod_reg_bit(prcm, hfeosc_ctrl, src_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_HFEOSC_CTRL_XTAL_START
def_set_mod_reg_bit(prcm, hfeosc_ctrl, xtal_start, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_HFEOSC_CTRL_TUNE
def_set_mod_reg_bit(prcm, hfeosc_ctrl, tune, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_HFEOSC_CTRL_SRC_SEL_DBG
def_set_mod_reg_bit(prcm, hfeosc_ctrl, src_sel_dbg, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_MBIAS_CTRL
void set_prcm_mbias_ctrl(u32 reg_addr,
			u32 en,
			u32 tst_en0,
			u32 tst_en1,
			u32 tst_en2,
			u32 tst_en3,
			u32 tune2_0,
			u32 tune3,
			u32 tune4,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_MBIAS_CTRL_EN
def_set_mod_reg_bit(prcm, mbias_ctrl, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_MBIAS_CTRL_TST_EN0
def_set_mod_reg_bit(prcm, mbias_ctrl, tst_en0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_MBIAS_CTRL_TST_EN1
def_set_mod_reg_bit(prcm, mbias_ctrl, tst_en1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_MBIAS_CTRL_TST_EN2
def_set_mod_reg_bit(prcm, mbias_ctrl, tst_en2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_MBIAS_CTRL_TST_EN3
def_set_mod_reg_bit(prcm, mbias_ctrl, tst_en3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_MBIAS_CTRL_TUNE2_0
def_set_mod_reg_bit(prcm, mbias_ctrl, tune2_0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_MBIAS_CTRL_TUNE3
def_set_mod_reg_bit(prcm, mbias_ctrl, tune3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_MBIAS_CTRL_TUNE4
def_set_mod_reg_bit(prcm, mbias_ctrl, tune4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_CPU_PLL_EN
void set_prcm_cpu_pll_en(u32 reg_addr,
			u32 enp,
			u32 enm,
			u32 oen,
			u32 ren,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_CPU_PLL_EN_ENP
def_set_mod_reg_bit(prcm, cpu_pll_en, enp, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_CPU_PLL_EN_ENM
def_set_mod_reg_bit(prcm, cpu_pll_en, enm, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_CPU_PLL_EN_OEN
def_set_mod_reg_bit(prcm, cpu_pll_en, oen, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_CPU_PLL_EN_REN
def_set_mod_reg_bit(prcm, cpu_pll_en, ren, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_CPU_PLL_FAC
void set_prcm_cpu_pll_fac(u32 reg_addr,
			u32 pre_div,
			u32 post_div,
			u32 n,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_CPU_PLL_FAC_PRE_DIV
def_set_mod_reg_bit(prcm, cpu_pll_fac, pre_div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_CPU_PLL_FAC_POST_DIV
def_set_mod_reg_bit(prcm, cpu_pll_fac, post_div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_CPU_PLL_FAC_N
def_set_mod_reg_bit(prcm, cpu_pll_fac, n, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_CPU_PLL_TUNE0
void set_prcm_cpu_pll_tune0(u32 reg_addr,
				u32 tune1,
				u32 tune2,
				u32 tune3,
				u32 tune4,
				u32 tune5,
				u32 tune6,
				u32 tune7,
				u32 tune8,
				u32 tune9,
				u32 tune10,
				u32 tune11,
				u32 tune12,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_CPU_PLL_TUNE0_TUNE1
def_set_mod_reg_bit(prcm, cpu_pll_tune0, tune1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_CPU_PLL_TUNE0_TUNE2
def_set_mod_reg_bit(prcm, cpu_pll_tune0, tune2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_CPU_PLL_TUNE0_TUNE3
def_set_mod_reg_bit(prcm, cpu_pll_tune0, tune3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_CPU_PLL_TUNE0_TUNE4
def_set_mod_reg_bit(prcm, cpu_pll_tune0, tune4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_CPU_PLL_TUNE0_TUNE5
def_set_mod_reg_bit(prcm, cpu_pll_tune0, tune5, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_CPU_PLL_TUNE0_TUNE6
def_set_mod_reg_bit(prcm, cpu_pll_tune0, tune6, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_CPU_PLL_TUNE0_TUNE7
def_set_mod_reg_bit(prcm, cpu_pll_tune0, tune7, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_CPU_PLL_TUNE0_TUNE8
def_set_mod_reg_bit(prcm, cpu_pll_tune0, tune8, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_CPU_PLL_TUNE0_TUNE9
def_set_mod_reg_bit(prcm, cpu_pll_tune0, tune9, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_CPU_PLL_TUNE0_TUNE10
def_set_mod_reg_bit(prcm, cpu_pll_tune0, tune10, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_CPU_PLL_TUNE0_TUNE11
def_set_mod_reg_bit(prcm, cpu_pll_tune0, tune11, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_CPU_PLL_TUNE0_TUNE12
def_set_mod_reg_bit(prcm, cpu_pll_tune0, tune12, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_CPU_PLL_TEN
void set_prcm_cpu_pll_ten(u32 reg_addr,
			u32 ten0,
			u32 ten1,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_CPU_PLL_TEN_TEN0
def_set_mod_reg_bit(prcm, cpu_pll_ten, ten0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_CPU_PLL_TEN_TEN1
def_set_mod_reg_bit(prcm, cpu_pll_ten, ten1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_CPU_PLL_STAT
void set_prcm_cpu_pll_stat(u32 reg_addr,
			u32 dlock,
			u32 asteady,
			u32 alock,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_CPU_PLL_STAT_DLOCK
def_set_mod_reg_bit(prcm, cpu_pll_stat, dlock, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_CPU_PLL_STAT_ASTEADY
def_set_mod_reg_bit(prcm, cpu_pll_stat, asteady, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_CPU_PLL_STAT_ALOCK
def_set_mod_reg_bit(prcm, cpu_pll_stat, alock, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_CPU_PLL_MOD
void set_prcm_cpu_pll_mod(u32 reg_addr,
			u32 mod,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_CPU_PLL_MOD_MOD
def_set_mod_reg_bit(prcm, cpu_pll_mod, mod, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_CPU_PLL_NFRAC
void set_prcm_cpu_pll_nfrac(u32 reg_addr,
				u32 nfrac,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_CPU_PLL_NFRAC_NFRAC
def_set_mod_reg_bit(prcm, cpu_pll_nfrac, nfrac, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_CPU_PLL_TUNE2
void set_prcm_cpu_pll_tune2(u32 reg_addr,
				u32 en,
				u32 step,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_CPU_PLL_TUNE2_EN
def_set_mod_reg_bit(prcm, cpu_pll_tune2, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_CPU_PLL_TUNE2_STEP
def_set_mod_reg_bit(prcm, cpu_pll_tune2, step, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_PLL_EN
void set_prcm_sdram_pll_en(u32 reg_addr,
			u32 enp,
			u32 enm,
			u32 oen,
			u32 ren,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_PLL_EN_ENP
def_set_mod_reg_bit(prcm, sdram_pll_en, enp, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_PLL_EN_ENM
def_set_mod_reg_bit(prcm, sdram_pll_en, enm, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_PLL_EN_OEN
def_set_mod_reg_bit(prcm, sdram_pll_en, oen, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_PLL_EN_REN
def_set_mod_reg_bit(prcm, sdram_pll_en, ren, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_PLL_FAC
void set_prcm_sdram_pll_fac(u32 reg_addr,
				u32 pre_div,
				u32 post_div,
				u32 n,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_PLL_FAC_PRE_DIV
def_set_mod_reg_bit(prcm, sdram_pll_fac, pre_div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_PLL_FAC_POST_DIV
def_set_mod_reg_bit(prcm, sdram_pll_fac, post_div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_PLL_FAC_N
def_set_mod_reg_bit(prcm, sdram_pll_fac, n, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_PLL_TUNE0
void set_prcm_sdram_pll_tune0(u32 reg_addr,
				u32 tune1,
				u32 tune3,
				u32 tune4,
				u32 tune5,
				u32 tune6,
				u32 tune7,
				u32 tune8,
				u32 tune9,
				u32 tune10,
				u32 tune11,
				u32 tune12,
				u32 tune13,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_PLL_TUNE0_TUNE1
def_set_mod_reg_bit(prcm, sdram_pll_tune0, tune1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_PLL_TUNE0_TUNE3
def_set_mod_reg_bit(prcm, sdram_pll_tune0, tune3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_PLL_TUNE0_TUNE4
def_set_mod_reg_bit(prcm, sdram_pll_tune0, tune4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_PLL_TUNE0_TUNE5
def_set_mod_reg_bit(prcm, sdram_pll_tune0, tune5, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_PLL_TUNE0_TUNE6
def_set_mod_reg_bit(prcm, sdram_pll_tune0, tune6, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_PLL_TUNE0_TUNE7
def_set_mod_reg_bit(prcm, sdram_pll_tune0, tune7, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_PLL_TUNE0_TUNE8
def_set_mod_reg_bit(prcm, sdram_pll_tune0, tune8, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_PLL_TUNE0_TUNE9
def_set_mod_reg_bit(prcm, sdram_pll_tune0, tune9, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_PLL_TUNE0_TUNE10
def_set_mod_reg_bit(prcm, sdram_pll_tune0, tune10, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_PLL_TUNE0_TUNE11
def_set_mod_reg_bit(prcm, sdram_pll_tune0, tune11, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_PLL_TUNE0_TUNE12
def_set_mod_reg_bit(prcm, sdram_pll_tune0, tune12, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_PLL_TUNE0_TUNE13
def_set_mod_reg_bit(prcm, sdram_pll_tune0, tune13, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_PLL_TEN
void set_prcm_sdram_pll_ten(u32 reg_addr,
				u32 ten0,
				u32 ten1,
				u32 ten2,
				u32 ten3,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_PLL_TEN_TEN0
def_set_mod_reg_bit(prcm, sdram_pll_ten, ten0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_PLL_TEN_TEN1
def_set_mod_reg_bit(prcm, sdram_pll_ten, ten1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_PLL_TEN_TEN2
def_set_mod_reg_bit(prcm, sdram_pll_ten, ten2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_PLL_TEN_TEN3
def_set_mod_reg_bit(prcm, sdram_pll_ten, ten3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_PLL_STAT
void set_prcm_sdram_pll_stat(u32 reg_addr,
				u32 dlock,
				u32 asteady,
				u32 alock,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_PLL_STAT_DLOCK
def_set_mod_reg_bit(prcm, sdram_pll_stat, dlock, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_PLL_STAT_ASTEADY
def_set_mod_reg_bit(prcm, sdram_pll_stat, asteady, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_PLL_STAT_ALOCK
def_set_mod_reg_bit(prcm, sdram_pll_stat, alock, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_PLL_MOD
void set_prcm_sdram_pll_mod(u32 reg_addr,
				u32 mod,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_PLL_MOD_MOD
def_set_mod_reg_bit(prcm, sdram_pll_mod, mod, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_PLL_NFRAC
void set_prcm_sdram_pll_nfrac(u32 reg_addr,
				u32 nfrac,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_PLL_NFRAC_NFRAC
def_set_mod_reg_bit(prcm, sdram_pll_nfrac, nfrac, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_PLL_TUNE1
void set_prcm_sdram_pll_tune1(u32 reg_addr,
				u32 period,
				u32 amplitude,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_PLL_TUNE1_PERIOD
def_set_mod_reg_bit(prcm, sdram_pll_tune1, period, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_PLL_TUNE1_AMPLITUDE
def_set_mod_reg_bit(prcm, sdram_pll_tune1, amplitude, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_PLL_TUNE2
void set_prcm_sdram_pll_tune2(u32 reg_addr,
				u32 en,
				u32 step,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_PLL_TUNE2_EN
def_set_mod_reg_bit(prcm, sdram_pll_tune2, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_PLL_TUNE2_STEP
def_set_mod_reg_bit(prcm, sdram_pll_tune2, step, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH0_PLL_EN
void set_prcm_perh0_pll_en(u32 reg_addr,
			u32 enp,
			u32 enm,
			u32 oen,
			u32 ren,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH0_PLL_EN_ENP
def_set_mod_reg_bit(prcm, perh0_pll_en, enp, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH0_PLL_EN_ENM
def_set_mod_reg_bit(prcm, perh0_pll_en, enm, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH0_PLL_EN_OEN
def_set_mod_reg_bit(prcm, perh0_pll_en, oen, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH0_PLL_EN_REN
def_set_mod_reg_bit(prcm, perh0_pll_en, ren, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH0_PLL_FAC
void set_prcm_perh0_pll_fac(u32 reg_addr,
				u32 pre_div,
				u32 post_div,
				u32 n,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH0_PLL_FAC_PRE_DIV
def_set_mod_reg_bit(prcm, perh0_pll_fac, pre_div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH0_PLL_FAC_POST_DIV
def_set_mod_reg_bit(prcm, perh0_pll_fac, post_div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH0_PLL_FAC_N
def_set_mod_reg_bit(prcm, perh0_pll_fac, n, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH0_PLL_TUNE0
void set_prcm_perh0_pll_tune0(u32 reg_addr,
				u32 tune1,
				u32 tune2,
				u32 tune3,
				u32 tune4,
				u32 tune5,
				u32 tune6,
				u32 tune7,
				u32 tune8,
				u32 tune9,
				u32 tune10,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH0_PLL_TUNE0_TUNE1
def_set_mod_reg_bit(prcm, perh0_pll_tune0, tune1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH0_PLL_TUNE0_TUNE2
def_set_mod_reg_bit(prcm, perh0_pll_tune0, tune2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH0_PLL_TUNE0_TUNE3
def_set_mod_reg_bit(prcm, perh0_pll_tune0, tune3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH0_PLL_TUNE0_TUNE4
def_set_mod_reg_bit(prcm, perh0_pll_tune0, tune4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH0_PLL_TUNE0_TUNE5
def_set_mod_reg_bit(prcm, perh0_pll_tune0, tune5, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH0_PLL_TUNE0_TUNE6
def_set_mod_reg_bit(prcm, perh0_pll_tune0, tune6, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH0_PLL_TUNE0_TUNE7
def_set_mod_reg_bit(prcm, perh0_pll_tune0, tune7, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH0_PLL_TUNE0_TUNE8
def_set_mod_reg_bit(prcm, perh0_pll_tune0, tune8, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH0_PLL_TUNE0_TUNE9
def_set_mod_reg_bit(prcm, perh0_pll_tune0, tune9, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH0_PLL_TUNE0_TUNE10
def_set_mod_reg_bit(prcm, perh0_pll_tune0, tune10, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH0_PLL_TEN
void set_prcm_perh0_pll_ten(u32 reg_addr,
				u32 ten0,
				u32 ten1,
				u32 ten3,
				u32 ten2,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH0_PLL_TEN_TEN0
def_set_mod_reg_bit(prcm, perh0_pll_ten, ten0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH0_PLL_TEN_TEN1
def_set_mod_reg_bit(prcm, perh0_pll_ten, ten1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH0_PLL_TEN_TEN3
def_set_mod_reg_bit(prcm, perh0_pll_ten, ten3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH0_PLL_TEN_TEN2
def_set_mod_reg_bit(prcm, perh0_pll_ten, ten2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH0_PLL_STAT
void set_prcm_perh0_pll_stat(u32 reg_addr,
				u32 dlock,
				u32 asteady,
				u32 alock,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH0_PLL_STAT_DLOCK
def_set_mod_reg_bit(prcm, perh0_pll_stat, dlock, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH0_PLL_STAT_ASTEADY
def_set_mod_reg_bit(prcm, perh0_pll_stat, asteady, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH0_PLL_STAT_ALOCK
def_set_mod_reg_bit(prcm, perh0_pll_stat, alock, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH0_PLL_MOD
void set_prcm_perh0_pll_mod(u32 reg_addr,
				u32 mod,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH0_PLL_MOD_MOD
def_set_mod_reg_bit(prcm, perh0_pll_mod, mod, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH0_PLL_NFRAC
void set_prcm_perh0_pll_nfrac(u32 reg_addr,
				u32 nfrac,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH0_PLL_NFRAC_NFRAC
def_set_mod_reg_bit(prcm, perh0_pll_nfrac, nfrac, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AUDIO_PLL_EN
void set_prcm_audio_pll_en(u32 reg_addr,
			u32 enp,
			u32 enm,
			u32 oen,
			u32 ren,
			u32 div_endiv17,
			u32 div_endiv7,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_AUDIO_PLL_EN_ENP
def_set_mod_reg_bit(prcm, audio_pll_en, enp, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AUDIO_PLL_EN_ENM
def_set_mod_reg_bit(prcm, audio_pll_en, enm, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AUDIO_PLL_EN_OEN
def_set_mod_reg_bit(prcm, audio_pll_en, oen, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AUDIO_PLL_EN_REN
def_set_mod_reg_bit(prcm, audio_pll_en, ren, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AUDIO_PLL_EN_DIV_ENDIV17
def_set_mod_reg_bit(prcm, audio_pll_en, div_endiv17, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AUDIO_PLL_EN_DIV_ENDIV7
def_set_mod_reg_bit(prcm, audio_pll_en, div_endiv7, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AUDIO_PLL_FAC
void set_prcm_audio_pll_fac(u32 reg_addr,
				u32 pre_div,
				u32 post_div,
				u32 n,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_AUDIO_PLL_FAC_PRE_DIV
def_set_mod_reg_bit(prcm, audio_pll_fac, pre_div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AUDIO_PLL_FAC_POST_DIV
def_set_mod_reg_bit(prcm, audio_pll_fac, post_div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AUDIO_PLL_FAC_N
def_set_mod_reg_bit(prcm, audio_pll_fac, n, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AUDIO_PLL_TUNE0
void set_prcm_audio_pll_tune0(u32 reg_addr,
				u32 tune1,
				u32 tune2,
				u32 tune3,
				u32 tune4,
				u32 tune5,
				u32 tune6,
				u32 tune7,
				u32 tune8,
				u32 tune9,
				u32 tune10,
				u32 tune11,
				u32 tune12,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_AUDIO_PLL_TUNE0_TUNE1
def_set_mod_reg_bit(prcm, audio_pll_tune0, tune1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AUDIO_PLL_TUNE0_TUNE2
def_set_mod_reg_bit(prcm, audio_pll_tune0, tune2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AUDIO_PLL_TUNE0_TUNE3
def_set_mod_reg_bit(prcm, audio_pll_tune0, tune3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AUDIO_PLL_TUNE0_TUNE4
def_set_mod_reg_bit(prcm, audio_pll_tune0, tune4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AUDIO_PLL_TUNE0_TUNE5
def_set_mod_reg_bit(prcm, audio_pll_tune0, tune5, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AUDIO_PLL_TUNE0_TUNE6
def_set_mod_reg_bit(prcm, audio_pll_tune0, tune6, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AUDIO_PLL_TUNE0_TUNE7
def_set_mod_reg_bit(prcm, audio_pll_tune0, tune7, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AUDIO_PLL_TUNE0_TUNE8
def_set_mod_reg_bit(prcm, audio_pll_tune0, tune8, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AUDIO_PLL_TUNE0_TUNE9
def_set_mod_reg_bit(prcm, audio_pll_tune0, tune9, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AUDIO_PLL_TUNE0_TUNE10
def_set_mod_reg_bit(prcm, audio_pll_tune0, tune10, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AUDIO_PLL_TUNE0_TUNE11
def_set_mod_reg_bit(prcm, audio_pll_tune0, tune11, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AUDIO_PLL_TUNE0_TUNE12
def_set_mod_reg_bit(prcm, audio_pll_tune0, tune12, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AUDIO_PLL_TEN
void set_prcm_audio_pll_ten(u32 reg_addr,
				u32 ten0,
				u32 ten1,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_AUDIO_PLL_TEN_TEN0
def_set_mod_reg_bit(prcm, audio_pll_ten, ten0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AUDIO_PLL_TEN_TEN1
def_set_mod_reg_bit(prcm, audio_pll_ten, ten1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AUDIO_PLL_STAT
void set_prcm_audio_pll_stat(u32 reg_addr,
				u32 dlock,
				u32 asteady,
				u32 alock,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_AUDIO_PLL_STAT_DLOCK
def_set_mod_reg_bit(prcm, audio_pll_stat, dlock, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AUDIO_PLL_STAT_ASTEADY
def_set_mod_reg_bit(prcm, audio_pll_stat, asteady, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AUDIO_PLL_STAT_ALOCK
def_set_mod_reg_bit(prcm, audio_pll_stat, alock, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AUDIO_PLL_MOD
void set_prcm_audio_pll_mod(u32 reg_addr,
				u32 mod,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_AUDIO_PLL_MOD_MOD
def_set_mod_reg_bit(prcm, audio_pll_mod, mod, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AUDIO_PLL_NFRAC
void set_prcm_audio_pll_nfrac(u32 reg_addr,
				u32 nfrac,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_AUDIO_PLL_NFRAC_NFRAC
def_set_mod_reg_bit(prcm, audio_pll_nfrac, nfrac, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_VC_PLL_EN
void set_prcm_vc_pll_en(u32 reg_addr,
			u32 enp,
			u32 enm,
			u32 oen,
			u32 ren,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_VC_PLL_EN_ENP
def_set_mod_reg_bit(prcm, vc_pll_en, enp, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_VC_PLL_EN_ENM
def_set_mod_reg_bit(prcm, vc_pll_en, enm, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_VC_PLL_EN_OEN
def_set_mod_reg_bit(prcm, vc_pll_en, oen, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_VC_PLL_EN_REN
def_set_mod_reg_bit(prcm, vc_pll_en, ren, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_VC_PLL_FAC
void set_prcm_vc_pll_fac(u32 reg_addr,
			u32 pre_div,
			u32 post_div,
			u32 n,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_VC_PLL_FAC_PRE_DIV
def_set_mod_reg_bit(prcm, vc_pll_fac, pre_div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_VC_PLL_FAC_POST_DIV
def_set_mod_reg_bit(prcm, vc_pll_fac, post_div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_VC_PLL_FAC_N
def_set_mod_reg_bit(prcm, vc_pll_fac, n, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_VC_PLL_TUNE0
void set_prcm_vc_pll_tune0(u32 reg_addr,
			u32 tune1,
			u32 tune2,
			u32 tune3,
			u32 tune4,
			u32 tune5,
			u32 tune6,
			u32 tune7,
			u32 tune8,
			u32 tune9,
			u32 tune10,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_VC_PLL_TUNE0_TUNE1
def_set_mod_reg_bit(prcm, vc_pll_tune0, tune1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_VC_PLL_TUNE0_TUNE2
def_set_mod_reg_bit(prcm, vc_pll_tune0, tune2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_VC_PLL_TUNE0_TUNE3
def_set_mod_reg_bit(prcm, vc_pll_tune0, tune3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_VC_PLL_TUNE0_TUNE4
def_set_mod_reg_bit(prcm, vc_pll_tune0, tune4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_VC_PLL_TUNE0_TUNE5
def_set_mod_reg_bit(prcm, vc_pll_tune0, tune5, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_VC_PLL_TUNE0_TUNE6
def_set_mod_reg_bit(prcm, vc_pll_tune0, tune6, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_VC_PLL_TUNE0_TUNE7
def_set_mod_reg_bit(prcm, vc_pll_tune0, tune7, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_VC_PLL_TUNE0_TUNE8
def_set_mod_reg_bit(prcm, vc_pll_tune0, tune8, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_VC_PLL_TUNE0_TUNE9
def_set_mod_reg_bit(prcm, vc_pll_tune0, tune9, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_VC_PLL_TUNE0_TUNE10
def_set_mod_reg_bit(prcm, vc_pll_tune0, tune10, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_VC_PLL_TEN
void set_prcm_vc_pll_ten(u32 reg_addr,
			u32 ten0,
			u32 ten1,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_VC_PLL_TEN_TEN0
def_set_mod_reg_bit(prcm, vc_pll_ten, ten0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_VC_PLL_TEN_TEN1
def_set_mod_reg_bit(prcm, vc_pll_ten, ten1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_VC_PLL_STAT
void set_prcm_vc_pll_stat(u32 reg_addr,
			u32 dlock,
			u32 asteady,
			u32 alock,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_VC_PLL_STAT_DLOCK
def_set_mod_reg_bit(prcm, vc_pll_stat, dlock, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_VC_PLL_STAT_ASTEADY
def_set_mod_reg_bit(prcm, vc_pll_stat, asteady, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_VC_PLL_STAT_ALOCK
def_set_mod_reg_bit(prcm, vc_pll_stat, alock, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_VC_PLL_MOD
void set_prcm_vc_pll_mod(u32 reg_addr,
			u32 mod,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_VC_PLL_MOD_MOD
def_set_mod_reg_bit(prcm, vc_pll_mod, mod, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_VC_PLL_NFRAC
void set_prcm_vc_pll_nfrac(u32 reg_addr,
			u32 nfrac,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_VC_PLL_NFRAC_NFRAC
def_set_mod_reg_bit(prcm, vc_pll_nfrac, nfrac, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH1_PLL_EN
void set_prcm_perh1_pll_en(u32 reg_addr,
			u32 enp,
			u32 enm,
			u32 oen,
			u32 ren,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH1_PLL_EN_ENP
def_set_mod_reg_bit(prcm, perh1_pll_en, enp, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH1_PLL_EN_ENM
def_set_mod_reg_bit(prcm, perh1_pll_en, enm, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH1_PLL_EN_OEN
def_set_mod_reg_bit(prcm, perh1_pll_en, oen, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH1_PLL_EN_REN
def_set_mod_reg_bit(prcm, perh1_pll_en, ren, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH1_PLL_FAC
void set_prcm_perh1_pll_fac(u32 reg_addr,
				u32 pre_div,
				u32 post_div,
				u32 n,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH1_PLL_FAC_PRE_DIV
def_set_mod_reg_bit(prcm, perh1_pll_fac, pre_div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH1_PLL_FAC_POST_DIV
def_set_mod_reg_bit(prcm, perh1_pll_fac, post_div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH1_PLL_FAC_N
def_set_mod_reg_bit(prcm, perh1_pll_fac, n, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH1_PLL_TUNE0
void set_prcm_perh1_pll_tune0(u32 reg_addr,
				u32 tune1,
				u32 tune2,
				u32 tune3,
				u32 tune4,
				u32 tune5,
				u32 tune6,
				u32 tune7,
				u32 tune8,
				u32 tune9,
				u32 tune10,
				u32 tune11,
				u32 tune12,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH1_PLL_TUNE0_TUNE1
def_set_mod_reg_bit(prcm, perh1_pll_tune0, tune1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH1_PLL_TUNE0_TUNE2
def_set_mod_reg_bit(prcm, perh1_pll_tune0, tune2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH1_PLL_TUNE0_TUNE3
def_set_mod_reg_bit(prcm, perh1_pll_tune0, tune3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH1_PLL_TUNE0_TUNE4
def_set_mod_reg_bit(prcm, perh1_pll_tune0, tune4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH1_PLL_TUNE0_TUNE5
def_set_mod_reg_bit(prcm, perh1_pll_tune0, tune5, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH1_PLL_TUNE0_TUNE6
def_set_mod_reg_bit(prcm, perh1_pll_tune0, tune6, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH1_PLL_TUNE0_TUNE7
def_set_mod_reg_bit(prcm, perh1_pll_tune0, tune7, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH1_PLL_TUNE0_TUNE8
def_set_mod_reg_bit(prcm, perh1_pll_tune0, tune8, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH1_PLL_TUNE0_TUNE9
def_set_mod_reg_bit(prcm, perh1_pll_tune0, tune9, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH1_PLL_TUNE0_TUNE10
def_set_mod_reg_bit(prcm, perh1_pll_tune0, tune10, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH1_PLL_TUNE0_TUNE11
def_set_mod_reg_bit(prcm, perh1_pll_tune0, tune11, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH1_PLL_TUNE0_TUNE12
def_set_mod_reg_bit(prcm, perh1_pll_tune0, tune12, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH1_PLL_TEN
void set_prcm_perh1_pll_ten(u32 reg_addr,
				u32 ten0,
				u32 ten1,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH1_PLL_TEN_TEN0
def_set_mod_reg_bit(prcm, perh1_pll_ten, ten0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH1_PLL_TEN_TEN1
def_set_mod_reg_bit(prcm, perh1_pll_ten, ten1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH1_PLL_STAT
void set_prcm_perh1_pll_stat(u32 reg_addr,
				u32 dlock,
				u32 asteady,
				u32 alock,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH1_PLL_STAT_DLOCK
def_set_mod_reg_bit(prcm, perh1_pll_stat, dlock, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH1_PLL_STAT_ASTEADY
def_set_mod_reg_bit(prcm, perh1_pll_stat, asteady, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH1_PLL_STAT_ALOCK
def_set_mod_reg_bit(prcm, perh1_pll_stat, alock, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH1_PLL_MOD
void set_prcm_perh1_pll_mod(u32 reg_addr,
				u32 mod,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH1_PLL_MOD_MOD
def_set_mod_reg_bit(prcm, perh1_pll_mod, mod, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH1_PLL_NFRAC
void set_prcm_perh1_pll_nfrac(u32 reg_addr,
				u32 nfrac,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH1_PLL_NFRAC_NFRAC
def_set_mod_reg_bit(prcm, perh1_pll_nfrac, nfrac, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH1_PLL_TUNE1
void set_prcm_perh1_pll_tune1(u32 reg_addr,
				u32 period,
				u32 amplitude,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH1_PLL_TUNE1_PERIOD
def_set_mod_reg_bit(prcm, perh1_pll_tune1, period, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH1_PLL_TUNE1_AMPLITUDE
def_set_mod_reg_bit(prcm, perh1_pll_tune1, amplitude, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_DISP_PLL_EN
void set_prcm_disp_pll_en(u32 reg_addr,
			u32 enp,
			u32 enm,
			u32 oen,
			u32 ren,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_DISP_PLL_EN_ENP
def_set_mod_reg_bit(prcm, disp_pll_en, enp, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_DISP_PLL_EN_ENM
def_set_mod_reg_bit(prcm, disp_pll_en, enm, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_DISP_PLL_EN_OEN
def_set_mod_reg_bit(prcm, disp_pll_en, oen, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_DISP_PLL_EN_REN
def_set_mod_reg_bit(prcm, disp_pll_en, ren, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_DISP_PLL_FAC
void set_prcm_disp_pll_fac(u32 reg_addr,
			u32 pre_div,
			u32 post_div,
			u32 n,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_DISP_PLL_FAC_PRE_DIV
def_set_mod_reg_bit(prcm, disp_pll_fac, pre_div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_DISP_PLL_FAC_POST_DIV
def_set_mod_reg_bit(prcm, disp_pll_fac, post_div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_DISP_PLL_FAC_N
def_set_mod_reg_bit(prcm, disp_pll_fac, n, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_DISP_PLL_TUNE0
void set_prcm_disp_pll_tune0(u32 reg_addr,
				u32 tune1,
				u32 tune2,
				u32 tune3,
				u32 tune4,
				u32 tune5,
				u32 tune6,
				u32 tune7,
				u32 tune8,
				u32 tune9,
				u32 tune10,
				u32 tune11,
				u32 tune12,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_DISP_PLL_TUNE0_TUNE1
def_set_mod_reg_bit(prcm, disp_pll_tune0, tune1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_DISP_PLL_TUNE0_TUNE2
def_set_mod_reg_bit(prcm, disp_pll_tune0, tune2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_DISP_PLL_TUNE0_TUNE3
def_set_mod_reg_bit(prcm, disp_pll_tune0, tune3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_DISP_PLL_TUNE0_TUNE4
def_set_mod_reg_bit(prcm, disp_pll_tune0, tune4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_DISP_PLL_TUNE0_TUNE5
def_set_mod_reg_bit(prcm, disp_pll_tune0, tune5, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_DISP_PLL_TUNE0_TUNE6
def_set_mod_reg_bit(prcm, disp_pll_tune0, tune6, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_DISP_PLL_TUNE0_TUNE7
def_set_mod_reg_bit(prcm, disp_pll_tune0, tune7, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_DISP_PLL_TUNE0_TUNE8
def_set_mod_reg_bit(prcm, disp_pll_tune0, tune8, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_DISP_PLL_TUNE0_TUNE9
def_set_mod_reg_bit(prcm, disp_pll_tune0, tune9, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_DISP_PLL_TUNE0_TUNE10
def_set_mod_reg_bit(prcm, disp_pll_tune0, tune10, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_DISP_PLL_TUNE0_TUNE11
def_set_mod_reg_bit(prcm, disp_pll_tune0, tune11, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_DISP_PLL_TUNE0_TUNE12
def_set_mod_reg_bit(prcm, disp_pll_tune0, tune12, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_DISP_PLL_TEN
void set_prcm_disp_pll_ten(u32 reg_addr,
			u32 ten0,
			u32 ten1,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_DISP_PLL_TEN_TEN0
def_set_mod_reg_bit(prcm, disp_pll_ten, ten0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_DISP_PLL_TEN_TEN1
def_set_mod_reg_bit(prcm, disp_pll_ten, ten1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_DISP_PLL_STAT
void set_prcm_disp_pll_stat(u32 reg_addr,
				u32 dlock,
				u32 asteady,
				u32 alock,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_DISP_PLL_STAT_DLOCK
def_set_mod_reg_bit(prcm, disp_pll_stat, dlock, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_DISP_PLL_STAT_ASTEADY
def_set_mod_reg_bit(prcm, disp_pll_stat, asteady, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_DISP_PLL_STAT_ALOCK
def_set_mod_reg_bit(prcm, disp_pll_stat, alock, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_DISP_PLL_MOD
void set_prcm_disp_pll_mod(u32 reg_addr,
			u32 mod,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_DISP_PLL_MOD_MOD
def_set_mod_reg_bit(prcm, disp_pll_mod, mod, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_DISP_PLL_NFRAC
void set_prcm_disp_pll_nfrac(u32 reg_addr,
				u32 nfrac,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_DISP_PLL_NFRAC_NFRAC
def_set_mod_reg_bit(prcm, disp_pll_nfrac, nfrac, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_DISP_PLL_TUNE1
void set_prcm_disp_pll_tune1(u32 reg_addr,
				u32 period,
				u32 amplitude,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_DISP_PLL_TUNE1_PERIOD
def_set_mod_reg_bit(prcm, disp_pll_tune1, period, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_DISP_PLL_TUNE1_AMPLITUDE
def_set_mod_reg_bit(prcm, disp_pll_tune1, amplitude, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AX_PLL_EN
void set_prcm_ax_pll_en(u32 reg_addr,
			u32 enp,
			u32 enm,
			u32 oen,
			u32 ren,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_AX_PLL_EN_ENP
def_set_mod_reg_bit(prcm, ax_pll_en, enp, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AX_PLL_EN_ENM
def_set_mod_reg_bit(prcm, ax_pll_en, enm, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AX_PLL_EN_OEN
def_set_mod_reg_bit(prcm, ax_pll_en, oen, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AX_PLL_EN_REN
def_set_mod_reg_bit(prcm, ax_pll_en, ren, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AX_PLL_FAC
void set_prcm_ax_pll_fac(u32 reg_addr,
			u32 pre_div,
			u32 post_div,
			u32 n,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_AX_PLL_FAC_PRE_DIV
def_set_mod_reg_bit(prcm, ax_pll_fac, pre_div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AX_PLL_FAC_POST_DIV
def_set_mod_reg_bit(prcm, ax_pll_fac, post_div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AX_PLL_FAC_N
def_set_mod_reg_bit(prcm, ax_pll_fac, n, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AX_PLL_TUNE0
void set_prcm_ax_pll_tune0(u32 reg_addr,
			u32 tune1,
			u32 tune2,
			u32 tune3,
			u32 tune4,
			u32 tune5,
			u32 tune6,
			u32 tune7,
			u32 tune8,
			u32 tune9,
			u32 tune10,
			u32 tune11,
			u32 tune12,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_AX_PLL_TUNE0_TUNE1
def_set_mod_reg_bit(prcm, ax_pll_tune0, tune1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AX_PLL_TUNE0_TUNE2
def_set_mod_reg_bit(prcm, ax_pll_tune0, tune2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AX_PLL_TUNE0_TUNE3
def_set_mod_reg_bit(prcm, ax_pll_tune0, tune3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AX_PLL_TUNE0_TUNE4
def_set_mod_reg_bit(prcm, ax_pll_tune0, tune4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AX_PLL_TUNE0_TUNE5
def_set_mod_reg_bit(prcm, ax_pll_tune0, tune5, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AX_PLL_TUNE0_TUNE6
def_set_mod_reg_bit(prcm, ax_pll_tune0, tune6, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AX_PLL_TUNE0_TUNE7
def_set_mod_reg_bit(prcm, ax_pll_tune0, tune7, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AX_PLL_TUNE0_TUNE8
def_set_mod_reg_bit(prcm, ax_pll_tune0, tune8, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AX_PLL_TUNE0_TUNE9
def_set_mod_reg_bit(prcm, ax_pll_tune0, tune9, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AX_PLL_TUNE0_TUNE10
def_set_mod_reg_bit(prcm, ax_pll_tune0, tune10, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AX_PLL_TUNE0_TUNE11
def_set_mod_reg_bit(prcm, ax_pll_tune0, tune11, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AX_PLL_TUNE0_TUNE12
def_set_mod_reg_bit(prcm, ax_pll_tune0, tune12, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AX_PLL_TEN
void set_prcm_ax_pll_ten(u32 reg_addr,
			u32 ten0,
			u32 ten1,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_AX_PLL_TEN_TEN0
def_set_mod_reg_bit(prcm, ax_pll_ten, ten0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AX_PLL_TEN_TEN1
def_set_mod_reg_bit(prcm, ax_pll_ten, ten1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AX_PLL_STAT
void set_prcm_ax_pll_stat(u32 reg_addr,
			u32 dlock,
			u32 asteady,
			u32 alock,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_AX_PLL_STAT_DLOCK
def_set_mod_reg_bit(prcm, ax_pll_stat, dlock, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AX_PLL_STAT_ASTEADY
def_set_mod_reg_bit(prcm, ax_pll_stat, asteady, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AX_PLL_STAT_ALOCK
def_set_mod_reg_bit(prcm, ax_pll_stat, alock, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AX_PLL_MOD
void set_prcm_ax_pll_mod(u32 reg_addr,
			u32 mod,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_AX_PLL_MOD_MOD
def_set_mod_reg_bit(prcm, ax_pll_mod, mod, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AX_PLL_NFRAC
void set_prcm_ax_pll_nfrac(u32 reg_addr,
			u32 nfrac,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_AX_PLL_NFRAC_NFRAC
def_set_mod_reg_bit(prcm, ax_pll_nfrac, nfrac, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AX_PLL_TUNE2
void set_prcm_ax_pll_tune2(u32 reg_addr,
			u32 en,
			u32 step,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_AX_PLL_TUNE2_EN
def_set_mod_reg_bit(prcm, ax_pll_tune2, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AX_PLL_TUNE2_STEP
def_set_mod_reg_bit(prcm, ax_pll_tune2, step, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH2_PLL_EN
void set_prcm_perh2_pll_en(u32 reg_addr,
			u32 enp,
			u32 enm,
			u32 oen,
			u32 ren,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH2_PLL_EN_ENP
def_set_mod_reg_bit(prcm, perh2_pll_en, enp, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH2_PLL_EN_ENM
def_set_mod_reg_bit(prcm, perh2_pll_en, enm, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH2_PLL_EN_OEN
def_set_mod_reg_bit(prcm, perh2_pll_en, oen, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH2_PLL_EN_REN
def_set_mod_reg_bit(prcm, perh2_pll_en, ren, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH2_PLL_FAC
void set_prcm_perh2_pll_fac(u32 reg_addr,
				u32 pre_div,
				u32 post_div,
				u32 n,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH2_PLL_FAC_PRE_DIV
def_set_mod_reg_bit(prcm, perh2_pll_fac, pre_div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH2_PLL_FAC_POST_DIV
def_set_mod_reg_bit(prcm, perh2_pll_fac, post_div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH2_PLL_FAC_N
def_set_mod_reg_bit(prcm, perh2_pll_fac, n, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH2_PLL_TUNE0
void set_prcm_perh2_pll_tune0(u32 reg_addr,
				u32 tune1,
				u32 tune2,
				u32 tune3,
				u32 tune4,
				u32 tune5,
				u32 tune6,
				u32 tune7,
				u32 tune8,
				u32 tune9,
				u32 tune10,
				u32 tune11,
				u32 tune12,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH2_PLL_TUNE0_TUNE1
def_set_mod_reg_bit(prcm, perh2_pll_tune0, tune1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH2_PLL_TUNE0_TUNE2
def_set_mod_reg_bit(prcm, perh2_pll_tune0, tune2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH2_PLL_TUNE0_TUNE3
def_set_mod_reg_bit(prcm, perh2_pll_tune0, tune3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH2_PLL_TUNE0_TUNE4
def_set_mod_reg_bit(prcm, perh2_pll_tune0, tune4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH2_PLL_TUNE0_TUNE5
def_set_mod_reg_bit(prcm, perh2_pll_tune0, tune5, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH2_PLL_TUNE0_TUNE6
def_set_mod_reg_bit(prcm, perh2_pll_tune0, tune6, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH2_PLL_TUNE0_TUNE7
def_set_mod_reg_bit(prcm, perh2_pll_tune0, tune7, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH2_PLL_TUNE0_TUNE8
def_set_mod_reg_bit(prcm, perh2_pll_tune0, tune8, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH2_PLL_TUNE0_TUNE9
def_set_mod_reg_bit(prcm, perh2_pll_tune0, tune9, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH2_PLL_TUNE0_TUNE10
def_set_mod_reg_bit(prcm, perh2_pll_tune0, tune10, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH2_PLL_TUNE0_TUNE11
def_set_mod_reg_bit(prcm, perh2_pll_tune0, tune11, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH2_PLL_TUNE0_TUNE12
def_set_mod_reg_bit(prcm, perh2_pll_tune0, tune12, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH2_PLL_TEN
void set_prcm_perh2_pll_ten(u32 reg_addr,
				u32 ten0,
				u32 ten1,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH2_PLL_TEN_TEN0
def_set_mod_reg_bit(prcm, perh2_pll_ten, ten0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH2_PLL_TEN_TEN1
def_set_mod_reg_bit(prcm, perh2_pll_ten, ten1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH2_PLL_STAT
void set_prcm_perh2_pll_stat(u32 reg_addr,
				u32 dlock,
				u32 asteady,
				u32 alock,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH2_PLL_STAT_DLOCK
def_set_mod_reg_bit(prcm, perh2_pll_stat, dlock, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH2_PLL_STAT_ASTEADY
def_set_mod_reg_bit(prcm, perh2_pll_stat, asteady, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH2_PLL_STAT_ALOCK
def_set_mod_reg_bit(prcm, perh2_pll_stat, alock, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH2_PLL_MOD
void set_prcm_perh2_pll_mod(u32 reg_addr,
				u32 mod,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH2_PLL_MOD_MOD
def_set_mod_reg_bit(prcm, perh2_pll_mod, mod, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH2_PLL_NFRAC
void set_prcm_perh2_pll_nfrac(u32 reg_addr,
				u32 nfrac,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH2_PLL_NFRAC_NFRAC
def_set_mod_reg_bit(prcm, perh2_pll_nfrac, nfrac, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH2_PLL_TUNE1
void set_prcm_perh2_pll_tune1(u32 reg_addr,
				u32 period,
				u32 amplitude,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH2_PLL_TUNE1_PERIOD
def_set_mod_reg_bit(prcm, perh2_pll_tune1, period, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PERH2_PLL_TUNE1_AMPLITUDE
def_set_mod_reg_bit(prcm, perh2_pll_tune1, amplitude, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PLL_TBLK
void set_prcm_pll_tblk(u32 reg_addr,
			u32 en,
			u32 div_endio,
			u32 sel0,
			u32 sel1,
			u32 div0,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_PLL_TBLK_EN
def_set_mod_reg_bit(prcm, pll_tblk, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PLL_TBLK_DIV_ENDIO
def_set_mod_reg_bit(prcm, pll_tblk, div_endio, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PLL_TBLK_SEL0
def_set_mod_reg_bit(prcm, pll_tblk, sel0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PLL_TBLK_SEL1
def_set_mod_reg_bit(prcm, pll_tblk, sel1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_PLL_TBLK_DIV0
def_set_mod_reg_bit(prcm, pll_tblk, div0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_CPU_AXI_CLK_CTRL
void set_prcm_cpu_axi_clk_ctrl(u32 reg_addr,
				u32 cpu_src_sel,
				u32 cpu_src_div,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_CPU_AXI_CLK_CTRL_CPU_SRC_SEL
def_set_mod_reg_bit(prcm, cpu_axi_clk_ctrl, cpu_src_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_CPU_AXI_CLK_CTRL_CPU_SRC_DIV
def_set_mod_reg_bit(prcm, cpu_axi_clk_ctrl, cpu_src_div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_APB_CLK_CTRL
void set_prcm_ahb_apb_clk_ctrl(u32 reg_addr,
				u32 src_sel,
				u32 ahb_div,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_APB_CLK_CTRL_SRC_SEL
def_set_mod_reg_bit(prcm, ahb_apb_clk_ctrl, src_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AHB_APB_CLK_CTRL_AHB_DIV
def_set_mod_reg_bit(prcm, ahb_apb_clk_ctrl, ahb_div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_MAXI_CLK_CTRL
void set_prcm_maxi_clk_ctrl(u32 reg_addr,
				u32 src_sel,
				u32 div,
				u32 en,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_MAXI_CLK_CTRL_SRC_SEL
def_set_mod_reg_bit(prcm, maxi_clk_ctrl, src_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_MAXI_CLK_CTRL_DIV
def_set_mod_reg_bit(prcm, maxi_clk_ctrl, div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_MAXI_CLK_CTRL_EN
def_set_mod_reg_bit(prcm, maxi_clk_ctrl, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_CLK_CTRL
void set_prcm_sdram_clk_ctrl(u32 reg_addr,
				u32 src_sel,
				u32 div,
				u32 en,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_CLK_CTRL_SRC_SEL
def_set_mod_reg_bit(prcm, sdram_clk_ctrl, src_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_CLK_CTRL_DIV
def_set_mod_reg_bit(prcm, sdram_clk_ctrl, div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_CLK_CTRL_EN
def_set_mod_reg_bit(prcm, sdram_clk_ctrl, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_RST
void set_prcm_sdram_rst(u32 reg_addr,
			u32 sdramc,
			u32 pub,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_RST_SDRAMC
def_set_mod_reg_bit(prcm, sdram_rst, sdramc, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_RST_PUB
def_set_mod_reg_bit(prcm, sdram_rst, pub, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_BM_CLK_CTRL
void set_prcm_sdram_bm_clk_ctrl(u32 reg_addr,
				u32 src_sel,
				u32 en,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_BM_CLK_CTRL_SRC_SEL
def_set_mod_reg_bit(prcm, sdram_bm_clk_ctrl, src_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDRAM_BM_CLK_CTRL_EN
def_set_mod_reg_bit(prcm, sdram_bm_clk_ctrl, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDC0_CLK_CTRL
void set_prcm_sdc0_clk_ctrl(u32 reg_addr,
				u32 src_sel,
				u32 div0,
				u32 div1,
				u32 en,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDC0_CLK_CTRL_SRC_SEL
def_set_mod_reg_bit(prcm, sdc0_clk_ctrl, src_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDC0_CLK_CTRL_DIV0
def_set_mod_reg_bit(prcm, sdc0_clk_ctrl, div0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDC0_CLK_CTRL_DIV1
def_set_mod_reg_bit(prcm, sdc0_clk_ctrl, div1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDC0_CLK_CTRL_EN
def_set_mod_reg_bit(prcm, sdc0_clk_ctrl, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDC1_CLK_CTRL
void set_prcm_sdc1_clk_ctrl(u32 reg_addr,
				u32 src_sel,
				u32 div0,
				u32 div1,
				u32 en,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDC1_CLK_CTRL_SRC_SEL
def_set_mod_reg_bit(prcm, sdc1_clk_ctrl, src_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDC1_CLK_CTRL_DIV0
def_set_mod_reg_bit(prcm, sdc1_clk_ctrl, div0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDC1_CLK_CTRL_DIV1
def_set_mod_reg_bit(prcm, sdc1_clk_ctrl, div1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDC1_CLK_CTRL_EN
def_set_mod_reg_bit(prcm, sdc1_clk_ctrl, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDC2_CLK_CTRL
void set_prcm_sdc2_clk_ctrl(u32 reg_addr,
				u32 src_sel,
				u32 div0,
				u32 div1,
				u32 en,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDC2_CLK_CTRL_SRC_SEL
def_set_mod_reg_bit(prcm, sdc2_clk_ctrl, src_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDC2_CLK_CTRL_DIV0
def_set_mod_reg_bit(prcm, sdc2_clk_ctrl, div0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDC2_CLK_CTRL_DIV1
def_set_mod_reg_bit(prcm, sdc2_clk_ctrl, div1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SDC2_CLK_CTRL_EN
def_set_mod_reg_bit(prcm, sdc2_clk_ctrl, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SPI0_CLK_CTRL
void set_prcm_spi0_clk_ctrl(u32 reg_addr,
				u32 src_sel,
				u32 div0,
				u32 div1,
				u32 en,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_SPI0_CLK_CTRL_SRC_SEL
def_set_mod_reg_bit(prcm, spi0_clk_ctrl, src_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SPI0_CLK_CTRL_DIV0
def_set_mod_reg_bit(prcm, spi0_clk_ctrl, div0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SPI0_CLK_CTRL_DIV1
def_set_mod_reg_bit(prcm, spi0_clk_ctrl, div1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SPI0_CLK_CTRL_EN
def_set_mod_reg_bit(prcm, spi0_clk_ctrl, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SPI1_CLK_CTRL
void set_prcm_spi1_clk_ctrl(u32 reg_addr,
				u32 src_sel,
				u32 div0,
				u32 div1,
				u32 en,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_SPI1_CLK_CTRL_SRC_SEL
def_set_mod_reg_bit(prcm, spi1_clk_ctrl, src_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SPI1_CLK_CTRL_DIV0
def_set_mod_reg_bit(prcm, spi1_clk_ctrl, div0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SPI1_CLK_CTRL_DIV1
def_set_mod_reg_bit(prcm, spi1_clk_ctrl, div1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SPI1_CLK_CTRL_EN
def_set_mod_reg_bit(prcm, spi1_clk_ctrl, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SPI2_CLK_CTRL
void set_prcm_spi2_clk_ctrl(u32 reg_addr,
				u32 src_sel,
				u32 div0,
				u32 div1,
				u32 en,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_SPI2_CLK_CTRL_SRC_SEL
def_set_mod_reg_bit(prcm, spi2_clk_ctrl, src_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SPI2_CLK_CTRL_DIV0
def_set_mod_reg_bit(prcm, spi2_clk_ctrl, div0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SPI2_CLK_CTRL_DIV1
def_set_mod_reg_bit(prcm, spi2_clk_ctrl, div1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_SPI2_CLK_CTRL_EN
def_set_mod_reg_bit(prcm, spi2_clk_ctrl, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_VC_ENC_CLK_CTRL
void set_prcm_vc_enc_clk_ctrl(u32 reg_addr,
				u32 src_sel,
				u32 div0,
				u32 en,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_VC_ENC_CLK_CTRL_SRC_SEL
def_set_mod_reg_bit(prcm, vc_enc_clk_ctrl, src_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_VC_ENC_CLK_CTRL_DIV0
def_set_mod_reg_bit(prcm, vc_enc_clk_ctrl, div0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_VC_ENC_CLK_CTRL_EN
def_set_mod_reg_bit(prcm, vc_enc_clk_ctrl, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_VC_DEC_CLK_CTRL
void set_prcm_vc_dec_clk_ctrl(u32 reg_addr,
				u32 src_sel,
				u32 div0,
				u32 en,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_VC_DEC_CLK_CTRL_SRC_SEL
def_set_mod_reg_bit(prcm, vc_dec_clk_ctrl, src_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_VC_DEC_CLK_CTRL_DIV0
def_set_mod_reg_bit(prcm, vc_dec_clk_ctrl, div0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_VC_DEC_CLK_CTRL_EN
def_set_mod_reg_bit(prcm, vc_dec_clk_ctrl, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_VISS_CLK_CTRL
void set_prcm_viss_clk_ctrl(u32 reg_addr,
				u32 sclk0_en,
				u32 sclk1_en,
				u32 sclk2_en,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_VISS_CLK_CTRL_SCLK0_EN
def_set_mod_reg_bit(prcm, viss_clk_ctrl, sclk0_en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_VISS_CLK_CTRL_SCLK1_EN
def_set_mod_reg_bit(prcm, viss_clk_ctrl, sclk1_en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_VISS_CLK_CTRL_SCLK2_EN
def_set_mod_reg_bit(prcm, viss_clk_ctrl, sclk2_en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_DPU_SCLK0_CLK_CTRL
void set_prcm_dpu_sclk0_clk_ctrl(u32 reg_addr,
				u32 src_sel,
				u32 div,
				u32 en,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_DPU_SCLK0_CLK_CTRL_SRC_SEL
def_set_mod_reg_bit(prcm, dpu_sclk0_clk_ctrl, src_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_DPU_SCLK0_CLK_CTRL_DIV
def_set_mod_reg_bit(prcm, dpu_sclk0_clk_ctrl, div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_DPU_SCLK0_CLK_CTRL_EN
def_set_mod_reg_bit(prcm, dpu_sclk0_clk_ctrl, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_DPU_SCLK1_CLK_CTRL
void set_prcm_dpu_sclk1_clk_ctrl(u32 reg_addr,
				u32 src_sel,
				u32 div,
				u32 en,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_DPU_SCLK1_CLK_CTRL_SRC_SEL
def_set_mod_reg_bit(prcm, dpu_sclk1_clk_ctrl, src_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_DPU_SCLK1_CLK_CTRL_DIV
def_set_mod_reg_bit(prcm, dpu_sclk1_clk_ctrl, div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_DPU_SCLK1_CLK_CTRL_EN
def_set_mod_reg_bit(prcm, dpu_sclk1_clk_ctrl, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_DPU_SCLK2_CLK_CTRL
void set_prcm_dpu_sclk2_clk_ctrl(u32 reg_addr,
				u32 en,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_DPU_SCLK2_CLK_CTRL_EN
def_set_mod_reg_bit(prcm, dpu_sclk2_clk_ctrl, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_DOSS_CLK_CTRL
void set_prcm_doss_clk_ctrl(u32 reg_addr,
				u32 hfeosc_en,
				u32 sclk0_en,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_DOSS_CLK_CTRL_HFEOSC_EN
def_set_mod_reg_bit(prcm, doss_clk_ctrl, hfeosc_en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_DOSS_CLK_CTRL_SCLK0_EN
def_set_mod_reg_bit(prcm, doss_clk_ctrl, sclk0_en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AX_CLK_CTRL
void set_prcm_ax_clk_ctrl(u32 reg_addr,
			u32 src_sel,
			u32 div,
			u32 en,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_AX_CLK_CTRL_SRC_SEL
def_set_mod_reg_bit(prcm, ax_clk_ctrl, src_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AX_CLK_CTRL_DIV
def_set_mod_reg_bit(prcm, ax_clk_ctrl, div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AX_CLK_CTRL_EN
def_set_mod_reg_bit(prcm, ax_clk_ctrl, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_I2S0_CLK_CTRL
void set_prcm_i2s0_clk_ctrl(u32 reg_addr,
				u32 src_sel,
				u32 div,
				u32 en,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_I2S0_CLK_CTRL_SRC_SEL
def_set_mod_reg_bit(prcm, i2s0_clk_ctrl, src_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_I2S0_CLK_CTRL_DIV
def_set_mod_reg_bit(prcm, i2s0_clk_ctrl, div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_I2S0_CLK_CTRL_EN
def_set_mod_reg_bit(prcm, i2s0_clk_ctrl, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_I2S1_CLK_CTRL
void set_prcm_i2s1_clk_ctrl(u32 reg_addr,
				u32 src_sel,
				u32 div,
				u32 en,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_I2S1_CLK_CTRL_SRC_SEL
def_set_mod_reg_bit(prcm, i2s1_clk_ctrl, src_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_I2S1_CLK_CTRL_DIV
def_set_mod_reg_bit(prcm, i2s1_clk_ctrl, div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_I2S1_CLK_CTRL_EN
def_set_mod_reg_bit(prcm, i2s1_clk_ctrl, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_I2C0_CLK_CTRL
void set_prcm_i2c0_clk_ctrl(u32 reg_addr,
				u32 src_sel,
				u32 div,
				u32 en,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_I2C0_CLK_CTRL_SRC_SEL
def_set_mod_reg_bit(prcm, i2c0_clk_ctrl, src_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_I2C0_CLK_CTRL_DIV
def_set_mod_reg_bit(prcm, i2c0_clk_ctrl, div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_I2C0_CLK_CTRL_EN
def_set_mod_reg_bit(prcm, i2c0_clk_ctrl, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_I2C1_CLK_CTRL
void set_prcm_i2c1_clk_ctrl(u32 reg_addr,
				u32 src_sel,
				u32 div,
				u32 en,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_I2C1_CLK_CTRL_SRC_SEL
def_set_mod_reg_bit(prcm, i2c1_clk_ctrl, src_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_I2C1_CLK_CTRL_DIV
def_set_mod_reg_bit(prcm, i2c1_clk_ctrl, div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_I2C1_CLK_CTRL_EN
def_set_mod_reg_bit(prcm, i2c1_clk_ctrl, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_I2C2_CLK_CTRL
void set_prcm_i2c2_clk_ctrl(u32 reg_addr,
				u32 src_sel,
				u32 div,
				u32 en,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_I2C2_CLK_CTRL_SRC_SEL
def_set_mod_reg_bit(prcm, i2c2_clk_ctrl, src_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_I2C2_CLK_CTRL_DIV
def_set_mod_reg_bit(prcm, i2c2_clk_ctrl, div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_I2C2_CLK_CTRL_EN
def_set_mod_reg_bit(prcm, i2c2_clk_ctrl, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_I2C3_CLK_CTRL
void set_prcm_i2c3_clk_ctrl(u32 reg_addr,
				u32 src_sel,
				u32 div,
				u32 en,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_I2C3_CLK_CTRL_SRC_SEL
def_set_mod_reg_bit(prcm, i2c3_clk_ctrl, src_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_I2C3_CLK_CTRL_DIV
def_set_mod_reg_bit(prcm, i2c3_clk_ctrl, div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_I2C3_CLK_CTRL_EN
def_set_mod_reg_bit(prcm, i2c3_clk_ctrl, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_UART0_CLK_CTRL
void set_prcm_uart0_clk_ctrl(u32 reg_addr,
				u32 src_sel,
				u32 div,
				u32 en,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_UART0_CLK_CTRL_SRC_SEL
def_set_mod_reg_bit(prcm, uart0_clk_ctrl, src_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_UART0_CLK_CTRL_DIV
def_set_mod_reg_bit(prcm, uart0_clk_ctrl, div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_UART0_CLK_CTRL_EN
def_set_mod_reg_bit(prcm, uart0_clk_ctrl, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_UART1_CLK_CTRL
void set_prcm_uart1_clk_ctrl(u32 reg_addr,
				u32 src_sel,
				u32 div,
				u32 en,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_UART1_CLK_CTRL_SRC_SEL
def_set_mod_reg_bit(prcm, uart1_clk_ctrl, src_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_UART1_CLK_CTRL_DIV
def_set_mod_reg_bit(prcm, uart1_clk_ctrl, div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_UART1_CLK_CTRL_EN
def_set_mod_reg_bit(prcm, uart1_clk_ctrl, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_UART2_CLK_CTRL
void set_prcm_uart2_clk_ctrl(u32 reg_addr,
				u32 src_sel,
				u32 div,
				u32 en,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_UART2_CLK_CTRL_SRC_SEL
def_set_mod_reg_bit(prcm, uart2_clk_ctrl, src_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_UART2_CLK_CTRL_DIV
def_set_mod_reg_bit(prcm, uart2_clk_ctrl, div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_UART2_CLK_CTRL_EN
def_set_mod_reg_bit(prcm, uart2_clk_ctrl, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_UART3_CLK_CTRL
void set_prcm_uart3_clk_ctrl(u32 reg_addr,
				u32 src_sel,
				u32 div,
				u32 en,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_UART3_CLK_CTRL_SRC_SEL
def_set_mod_reg_bit(prcm, uart3_clk_ctrl, src_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_UART3_CLK_CTRL_DIV
def_set_mod_reg_bit(prcm, uart3_clk_ctrl, div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_UART3_CLK_CTRL_EN
def_set_mod_reg_bit(prcm, uart3_clk_ctrl, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_USB_PHY_RST
void set_prcm_usb_phy_rst(u32 reg_addr,
			u32 rst,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_USB_PHY_RST_RST
def_set_mod_reg_bit(prcm, usb_phy_rst, rst, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_GPADC_CLK_CTRL
void set_prcm_gpadc_clk_ctrl(u32 reg_addr,
				u32 src_sel,
				u32 div,
				u32 en,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_GPADC_CLK_CTRL_SRC_SEL
def_set_mod_reg_bit(prcm, gpadc_clk_ctrl, src_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_GPADC_CLK_CTRL_DIV
def_set_mod_reg_bit(prcm, gpadc_clk_ctrl, div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_GPADC_CLK_CTRL_EN
def_set_mod_reg_bit(prcm, gpadc_clk_ctrl, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_IR_CLK_CTRL
void set_prcm_ir_clk_ctrl(u32 reg_addr,
			u32 en,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_IR_CLK_CTRL_EN
def_set_mod_reg_bit(prcm, ir_clk_ctrl, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_GMAC_CLK_CTRL
void set_prcm_gmac_clk_ctrl(u32 reg_addr,
				u32 core_src_sel,
				u32 core_div,
				u32 core_en,
				u32 sclk_src_sel,
				u32 sclk1_en,
				u32 sclk0_en,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_GMAC_CLK_CTRL_CORE_SRC_SEL
def_set_mod_reg_bit(prcm, gmac_clk_ctrl, core_src_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_GMAC_CLK_CTRL_CORE_DIV
def_set_mod_reg_bit(prcm, gmac_clk_ctrl, core_div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_GMAC_CLK_CTRL_CORE_EN
def_set_mod_reg_bit(prcm, gmac_clk_ctrl, core_en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_GMAC_CLK_CTRL_SCLK_SRC_SEL
def_set_mod_reg_bit(prcm, gmac_clk_ctrl, sclk_src_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_GMAC_CLK_CTRL_SCLK1_EN
def_set_mod_reg_bit(prcm, gmac_clk_ctrl, sclk1_en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_GMAC_CLK_CTRL_SCLK0_EN
def_set_mod_reg_bit(prcm, gmac_clk_ctrl, sclk0_en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_MBIST_CLK_CTRL
void set_prcm_mbist_clk_ctrl(u32 reg_addr,
				u32 src_sel,
				u32 div,
				u32 en,
				u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_MBIST_CLK_CTRL_SRC_SEL
def_set_mod_reg_bit(prcm, mbist_clk_ctrl, src_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_MBIST_CLK_CTRL_DIV
def_set_mod_reg_bit(prcm, mbist_clk_ctrl, div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_MBIST_CLK_CTRL_EN
def_set_mod_reg_bit(prcm, mbist_clk_ctrl, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_PRCM_AES_CLK_CTRL
void set_prcm_aes_clk_ctrl(u32 reg_addr,
			u32 en,
			u32 m_or_r);
#endif

#ifdef DEF_SET_PRCM_AES_CLK_CTRL_EN
def_set_mod_reg_bit(prcm, aes_clk_ctrl, en, addr, val, m_or_r);
#endif

#endif /* ___PRCM__SET___H___ */
