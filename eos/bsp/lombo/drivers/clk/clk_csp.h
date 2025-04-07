/*
 * clk_csp.h
 *
 * Copyright (C) 2016-2018, LomboTech Co.Ltd.
 * Author: lomboswer <lomboswer@lombotech.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef __CLK_CSP_H___
#define __CLK_CSP_H___

#define BIT(nr)			(1UL << (nr))

/* clock frequence of osc*/
#define CLK_1M			(1000000)
#define CLK_32K			(32768)
#define CLK_24M			(24000000)

/*
 * flags used across common clk.  these flags should only affect the
 * top-level framework.  custom flags for dealing with hardware specifics
 * belong in struct clk_foo
 */
#define CLK_SET_RATE_GATE	BIT(0) /* must be gated across rate change */
#define CLK_SET_PARENT_GATE	BIT(1) /* must be gated across re-parent */
#define CLK_SET_RATE_PARENT	BIT(2) /* propagate rate change up one level */
#define CLK_IGNORE_UNUSED	BIT(3) /* do not gate even if unused */
#define CLK_IS_ROOT		BIT(4) /* root clk, has no parent */
#define CLK_IS_BASIC		BIT(5) /* Basic clk, can't do a to_clk_foo() */
#define CLK_GET_RATE_NOCACHE	BIT(6) /* do not use the cached clk rate */


#define CLK_TYPE_FIXED_CLOCK		BIT(8)
#define CLK_TYPE_PLL_CLOCK		BIT(9)
#define CLK_TYPE_FACT_CLOCK		BIT(10)
#define CLK_TYPE_GATE_CLOCK		BIT(11)
#define CLK_TYPE_DEVIDER_CLOCK		BIT(12)
#define CLK_TYPE_MODULE_CLOCK		BIT(13)

#define CLK_DIVIDER_ONE_BASED		BIT(16)
#define CLK_DIVIDER_POWER_OF_TWO	BIT(17)
#define CLK_DIVIDER_ALLOW_ZERO		BIT(18)
#define CLK_DIVIDER_ROUND_CLOSEST	BIT(19)
#define CLK_DIVIDER_NORMAL_TWO		BIT(20)

#define CLK_GATE_SET_TO_DISABLE		BIT(24)

enum {
	CLK_ID_INVALID = 0,
	CLK_ID_START,
	CLK_ID_OSC32K = CLK_ID_START,
	CLK_ID_OSC24M,
	CLK_ID_NULL_CLK,

	CLK_ID_CPU_PLL,
	CLK_ID_SDRAM_PLL,
	CLK_ID_PERH0_PLL_VCO,

	CLK_ID_VC_PLL,
	CLK_ID_AUDIO_PLL_DIV0,
	CLK_ID_PERH1_PLL,

#ifdef ARCH_LOMBO_N7V0
	CLK_ID_VIDEO_PLL,
#endif /* ARCH_LOMBO_N7V0 */
#ifdef ARCH_LOMBO_N7V1
	CLK_ID_DISP_PLL,
	CLK_ID_AX_PLL,
	CLK_ID_PERH2_PLL,
#endif /* ARCH_LOMBO_N7V1 */

#ifdef ARCH_LOMBO_N7V0
	CLK_ID_CPU_AXI_CLK,
#endif /* ARCH_LOMBO_N7V0 */

	CLK_ID_PERH0_PLL_DIVM,
	CLK_ID_AUDIO_PLL_DIVM,
	CLK_ID_APB_CLK,
	CLK_ID_PERH0_PLL_DIV2,
	CLK_ID_PERH0_PLL_DIV4,
	CLK_ID_PERH0_PLL_DIV8,
	CLK_ID_AUDIO_PLL_DIV7,
	CLK_ID_AUDIO_PLL_DIV17,

	CLK_ID_CPU_CLK,
	CLK_ID_AHB_CLK,
	CLK_ID_MEM_AXI_CLK,
	CLK_ID_SDRAM_CLK,
	CLK_ID_SDRAM_BANDW_CLK,
	CLK_ID_SDC0_CLK,
	CLK_ID_SDC1_CLK,
#ifdef ARCH_LOMBO_N7V1
	CLK_ID_SDC2_CLK,
#endif /* ARCH_LOMBO_N7V1 */
	CLK_ID_SPI0_CLK,
	CLK_ID_SPI1_CLK,
	CLK_ID_SPI2_CLK,

#ifdef ARCH_LOMBO_N7V0
	CLK_ID_VC_CLK,
#endif /* ARCH_LOMBO_N7V0 */
#ifdef ARCH_LOMBO_N7V1
	CLK_ID_VC_ENC_CLK,
	CLK_ID_VC_DEC_CLK,
#endif /* ARCH_LOMBO_N7V1 */

	CLK_ID_DPU_SCLK0,
	CLK_ID_DPU_SCLK1,
	CLK_ID_AX_CLK,

#ifdef ARCH_LOMBO_N7V0
	CLK_ID_I2S_CLK,
#endif /* ARCH_LOMBO_N7V0 */
#ifdef ARCH_LOMBO_N7V1
	CLK_ID_I2S0_CLK,
	CLK_ID_I2S1_CLK,
#endif /* ARCH_LOMBO_N7V1 */

	CLK_ID_I2C0_CLK,
	CLK_ID_I2C1_CLK,
	CLK_ID_I2C2_CLK,
	CLK_ID_I2C3_CLK,
	CLK_ID_UART0_CLK,
	CLK_ID_UART1_CLK,
	CLK_ID_UART2_CLK,
	CLK_ID_UART3_CLK,
#ifdef ARCH_LOMBO_N7V1
	CLK_ID_GPADC_CLK,
	CLK_ID_IR_CLK,
	CLK_ID_GMAC_CLK,
#endif /* ARCH_LOMBO_N7V1 */
	CLK_ID_MBIST_CLK,

	/* AHB BUS Clock Gating Register 0 */
	CLK_ID_AHB_DMA_GATE,
	CLK_ID_AHB_PTIMER_GATE,
	CLK_ID_AHB_AES_GATE,
	CLK_ID_AHB_SDRAM_GATE,
	CLK_ID_AHB_SDC0_GATE,
	CLK_ID_AHB_SDC1_GATE,
#ifdef ARCH_LOMBO_N7V1
	CLK_ID_AHB_SDC2_GATE,
#endif /* ARCH_LOMBO_N7V1 */
	CLK_ID_AHB_SPI0_GATE,
	CLK_ID_AHB_SPI1_GATE,
	CLK_ID_AHB_SPI2_GATE,

	/* AHB BUS Reset Register 0 */
	CLK_ID_AHB_DMA_RESET,
	CLK_ID_AHB_PTIMER_RESET,
	CLK_ID_AHB_AES_RESET,
	CLK_ID_AHB_SDRAM_RESET,
	CLK_ID_AHB_SDC0_RESET,
	CLK_ID_AHB_SDC1_RESET,
#ifdef ARCH_LOMBO_N7V1
	CLK_ID_AHB_SDC2_RESET,
#endif /* ARCH_LOMBO_N7V1 */
	CLK_ID_AHB_SPI0_RESET,
	CLK_ID_AHB_SPI1_RESET,
	CLK_ID_AHB_SPI2_RESET,

	/* AHB BUS Clock Gating Register 1 */
	CLK_ID_AHB_VISS_GATE,
	CLK_ID_AHB_DPU_GATE,
	CLK_ID_AHB_DOSS_GATE,
#ifdef ARCH_LOMBO_N7V0
	CLK_ID_AHB_VC_GATE,
#endif /* ARCH_LOMBO_N7V0 */
#ifdef ARCH_LOMBO_N7V1
	CLK_ID_AHB_VC_DEC_GATE,
	CLK_ID_AHB_VC_ENC_GATE,
#endif /* ARCH_LOMBO_N7V1 */
	CLK_ID_AHB_AX_GATE,
	CLK_ID_AHB_USB_GATE,
#ifdef ARCH_LOMBO_N7V1
	CLK_ID_AHB_GMAC_GATE,
#endif /* ARCH_LOMBO_N7V1 */

	/* AHB BUS Reset Register 1 */
	CLK_ID_AHB_VISS_RESET,
	CLK_ID_AHB_DPU_RESET,
	CLK_ID_AHB_DOSS_RESET,
#ifdef ARCH_LOMBO_N7V0
	CLK_ID_AHB_VC_RESET,
#endif /* ARCH_LOMBO_N7V0 */
#ifdef ARCH_LOMBO_N7V1
	CLK_ID_AHB_VC_DEC_RESET,
	CLK_ID_AHB_VC_ENC_RESET,
#endif /* ARCH_LOMBO_N7V1 */
	CLK_ID_AHB_AX_RESET,
	CLK_ID_AHB_USB_RESET,
#ifdef ARCH_LOMBO_N7V1
	CLK_ID_AHB_GMAC_RESET,
#endif /* ARCH_LOMBO_N7V1 */

	/* APB BUS Clock Gating Register 0 */
	CLK_ID_APB_I2C0_GATE,
	CLK_ID_APB_I2C1_GATE,
	CLK_ID_APB_I2C2_GATE,
	CLK_ID_APB_I2C3_GATE,
	CLK_ID_APB_UART0_GATE,
	CLK_ID_APB_UART1_GATE,
	CLK_ID_APB_UART2_GATE,
	CLK_ID_APB_UART3_GATE,

	/* APB BUS Reset Register 0 */
	CLK_ID_APB_I2C0_RESET,
	CLK_ID_APB_I2C1_RESET,
	CLK_ID_APB_I2C2_RESET,
	CLK_ID_APB_I2C3_RESET,
	CLK_ID_APB_UART0_RESET,
	CLK_ID_APB_UART1_RESET,
	CLK_ID_APB_UART2_RESET,
	CLK_ID_APB_UART3_RESET,

	/* APB BUS Clock Gating Register 1 */
#ifdef ARCH_LOMBO_N7V0
	CLK_ID_APB_I2S_GATE,
#endif /* ARCH_LOMBO_N7V0 */
#ifdef ARCH_LOMBO_N7V1
	CLK_ID_APB_I2S0_GATE,
	CLK_ID_APB_I2S1_GATE,
#endif /* ARCH_LOMBO_N7V1 */
	CLK_ID_APB_GPADC_GATE,
#ifdef ARCH_LOMBO_N7V1
	CLK_ID_APB_IR_GATE,
#endif /* ARCH_LOMBO_N7V1 */
	CLK_ID_APB_GPIO_GATE,

	/* APB BUS Reset Register 1 */
#ifdef ARCH_LOMBO_N7V0
	CLK_ID_APB_I2S_RESET,
#endif /* ARCH_LOMBO_N7V0 */
#ifdef ARCH_LOMBO_N7V1
	CLK_ID_APB_I2S0_RESET,
	CLK_ID_APB_I2S1_RESET,
#endif /* ARCH_LOMBO_N7V1 */
	CLK_ID_APB_GPADC_RESET,
#ifdef ARCH_LOMBO_N7V1
	CLK_ID_APB_IR_RESET,
#endif /* ARCH_LOMBO_N7V1 */
	CLK_ID_APB_GPIO_RESET,

	/* AXI BUS Clock Gating Register */
	CLK_ID_MAXI_DMA_GATE,
#ifdef ARCH_LOMBO_N7V0
	CLK_ID_MAXI_VC_GATE,
#endif /* ARCH_LOMBO_N7V0 */
#ifdef ARCH_LOMBO_N7V1
	CLK_ID_MAXI_VC_ENC_GATE,
	CLK_ID_MAXI_VC_DEC_GATE,
#endif /* ARCH_LOMBO_N7V1 */
	CLK_ID_MAXI_VISS0_GATE,
	CLK_ID_MAXI_VISS1_GATE,
	CLK_ID_MAXI_DPU_GATE,
	CLK_ID_MAXI_DOSS_GATE,
	CLK_ID_MAXI_AX_GATE,
#ifdef ARCH_LOMBO_N7V1
	CLK_ID_MAXI_GMAC_GATE,
#endif /* ARCH_LOMBO_N7V1 */

	CLK_ID_VISS_SCLK0,
	CLK_ID_VISS_SCLK1,
	CLK_ID_VISS_SCLK2,
	CLK_ID_DPU_SCLK2,
	CLK_ID_DOSS_SCLK0,
	CLK_ID_DOSS_OSC24M,
	CLK_ID_AES_CLK,

	CLK_ID_USB_PHY_RESET,
	CLK_ID_MAX,
	CLK_NUM = CLK_ID_MAX,
};

#define INIT_FIXED_CLK(P, frequence) \
{ \
	clk_hw_t *c = &clk_hw_all[CLK_ID_##P]; \
	c->name = CLK_NAME_##P; \
	c->id = CLK_ID_##P; \
	c->flags = CLK_TYPE_FIXED_CLOCK; \
	c->fixed_rate = frequence; \
}
#define INIT_PLL_CLK(P, reg_start)	\
{ \
	clk_hw_t *c = &clk_hw_all[CLK_ID_##P]; \
	c->name = CLK_NAME_##P; \
	c->id = CLK_ID_##P; \
	c->flags = CLK_TYPE_PLL_CLOCK; \
	c->clk_src[0] = CLK_ID_OSC24M; \
	c->reg = reg_start; \
}

#define INIT_FIXED_FACT_CLK(P, factor, div, src)	\
{ \
	clk_hw_t *c = &clk_hw_all[CLK_ID_##P]; \
	c->name = CLK_NAME_##P; \
	c->id = CLK_ID_##P; \
	c->flags = CLK_TYPE_FACT_CLOCK; \
	c->mult = factor; \
	c->fix_div = div; \
	c->clk_src[0] = src; \
}

#define INIT_GATE_CLK(P, register, bit, src)	\
{ \
	clk_hw_t *c = &clk_hw_all[CLK_ID_##P]; \
	c->name = CLK_NAME_##P; \
	c->id = CLK_ID_##P; \
	c->flags = CLK_TYPE_GATE_CLOCK; \
	c->reg = register; \
	c->enable_bit = bit; \
	c->clk_src[0] = src; \
}

#define INIT_DEVIDER_CLK(P, register, div_shift, div_width, src)	\
{ \
	clk_hw_t *c = &clk_hw_all[CLK_ID_##P]; \
	c->name = CLK_NAME_##P; \
	c->id = CLK_ID_##P; \
	c->flags = CLK_TYPE_DEVIDER_CLOCK; \
	c->reg = register; \
	c->div_shift0 = div_shift; \
	c->div_width0 = div_width; \
	c->clk_src[0] = src; \
}

#define MODULE_DEFAULT_ENABLE_SHIFT (31)
#define MODULE_INVALID_ENABLE_SHIFT (0xF0)
#define MODULE_DEFAULT_SRC_SHIFT (0)

#define INIT_M_CLK(P, r, en, d_typ, d_s0, d_w0, d_s1, d_w1, s0, s1, s2, s3) \
{ \
	clk_hw_t *c = &clk_hw_all[CLK_ID_##P]; \
	c->name = CLK_NAME_##P; \
	c->id = CLK_ID_##P; \
	c->enable_bit = en; \
	c->flags = CLK_TYPE_MODULE_CLOCK | d_typ; \
	c->reg = r; \
	c->div_shift0 = d_s0; \
	c->div_width0 = d_w0; \
	c->div_shift1 = d_s1; \
	c->div_width1 = d_w1; \
	c->src_shift = MODULE_DEFAULT_SRC_SHIFT; \
	c->clk_src[0] = s0; \
	c->clk_src[1] = s1; \
	c->clk_src[2] = s2; \
	c->clk_src[3] = s3; \
}

#ifndef readl
#define readl(reg)		test_readl((void *)(reg))
#define writel(val, reg)	test_writel(val, (void *)(reg))
extern int test_readl(void *reg);
extern int test_writel(int val, void *reg);
#endif
int atoi(const char *nptr);

void init_clk_hw(void);

#endif
