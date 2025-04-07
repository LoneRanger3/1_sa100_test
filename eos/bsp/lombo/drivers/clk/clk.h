/*
 * clk.h - Standard functionality for the common clock API.
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

#ifndef __CLK_H___
#define __CLK_H___
#include <rtdef.h>
#include <sizes.h>
#include "list.h"

/* for audio clock */
enum {
	CLK_24571M	= 24571000,
	CLK_24571M_X2	= 49142000,
	CLK_24571M_X3	= 73713000,
	CLK_24571M_X6	= 147426000,
	CLK_225882M	= 22588200,
	CLK_225882M_X2	= 45176400,
	CLK_225882M_X3	= 67764600,
};

/**
 * DOC: clk notifier callback types
 *
 * PRE_RATE_CHANGE - called immediately before the clk rate is changed,
 *     to indicate that the rate change will proceed.  Drivers must
 *     immediately terminate any operations that will be affected by the
 *     rate change.  Callbacks may either return NOTIFY_DONE, NOTIFY_OK,
 *     NOTIFY_STOP or NOTIFY_BAD.
 *
 * ABORT_RATE_CHANGE: called if the rate change failed for some reason
 *     after PRE_RATE_CHANGE.  In this case, all registered notifiers on
 *     the clk will be called with ABORT_RATE_CHANGE. Callbacks must
 *     always return NOTIFY_DONE or NOTIFY_OK.
 *
 * POST_RATE_CHANGE - called after the clk rate change has successfully
 *     completed.  Callbacks must always return NOTIFY_DONE or NOTIFY_OK.
 *
 */
#define PRE_RATE_CHANGE			BIT(0)
#define POST_RATE_CHANGE		BIT(1)
#define ABORT_RATE_CHANGE		BIT(2)

#define NOTIFY_DONE		0x0000		/* Don't care */
#define NOTIFY_OK		0x0001		/* Suits me */
#define NOTIFY_STOP_MASK	0x8000		/* Don't call further */
#define NOTIFY_BAD		(NOTIFY_STOP_MASK|0x0002)
/* Bad/Veto action */
/*
 * Clean way to return from the notifier and stop further calls.
 */
#define NOTIFY_STOP		(NOTIFY_OK|NOTIFY_STOP_MASK)

typedef int clk_handle_t;

/**
 * struct clk_notifier_data - rate data to pass to the notifier callback
 * @clk: clock handle being changed
 * @old_rate: previous rate of this clk
 * @new_rate: new rate of this clk
 *
 * For a pre-notifier, old_rate is the clk's rate before this rate
 * change, and new_rate is what the rate will be in the future.  For a
 * post-notifier, old_rate and new_rate are both set to the clk's
 * current rate (this was done to optimize the implementation).
 */
struct clk_notifier_data {
	clk_handle_t		clk;
	unsigned long		old_rate;
	unsigned long		new_rate;
};

typedef	int (*notifier_fn_t)(u32 action, struct clk_notifier_data *data);

/* fixed clk */
#define CLK_NAME_OSC32K			"osc32k"
#define CLK_NAME_OSC24M			"osc24m"
#define CLK_NAME_NULL_CLK		"null_clk"

/* pll clk */
#define CLK_NAME_CPU_PLL		"cpu_pll"
#define CLK_NAME_SDRAM_PLL		"sdram_pll"
#define CLK_NAME_PERH0_PLL_VCO		"perh0_pll_vco"
#define CLK_NAME_VC_PLL			"vc_pll"
#define CLK_NAME_AUDIO_PLL_DIV0		"audio_pll_div0"
#define CLK_NAME_PERH1_PLL		"perh1_pll"
#define CLK_NAME_VIDEO_PLL		"video_pll"
#define CLK_NAME_DISP_PLL		"disp_pll"
#define CLK_NAME_AX_PLL			"ax_pll"
#define CLK_NAME_PERH2_PLL		"perh2_pll"

#define CLK_NAME_CPU_AXI_CLK		"axi_clk"
#define CLK_NAME_PERH0_PLL_DIVM		"perh0_pll_divm"
#define CLK_NAME_AUDIO_PLL_DIVM		"audio_pll_divm"

/* fixed factor clk */
#define CLK_NAME_APB_CLK		"apb_clk"
#define CLK_NAME_PERH0_PLL_DIV2		"perh0_pll_div2"
#define CLK_NAME_PERH0_PLL_DIV4		"perh0_pll_div4"
#define CLK_NAME_PERH0_PLL_DIV8		"perh0_pll_div8"
#define CLK_NAME_AUDIO_PLL_DIV7		"audio_pll_div7"
#define CLK_NAME_AUDIO_PLL_DIV17	"audio_pll_div17"

/* module clk */
#define CLK_NAME_CPU_CLK		"cpu_clk"
#define CLK_NAME_AHB_CLK		"ahb_clk"
#define CLK_NAME_MEM_AXI_CLK		"mem_axi_clk"
#define CLK_NAME_SDRAM_CLK		"sdram_clk"
#define CLK_NAME_SDRAM_BANDW_CLK	"sdram_bandw_clk"
#define CLK_NAME_SDC0_CLK		"sdc0_clk"
#define CLK_NAME_SDC1_CLK		"sdc1_clk"
#define CLK_NAME_SDC2_CLK		"sdc2_clk"
#define CLK_NAME_SPI0_CLK		"spi0_clk"
#define CLK_NAME_SPI1_CLK		"spi1_clk"
#define CLK_NAME_SPI2_CLK		"spi2_clk"
#define CLK_NAME_VC_CLK			"vc_clk"
#define CLK_NAME_VC_ENC_CLK		"vc_enc_clk"
#define CLK_NAME_VC_DEC_CLK		"vc_dec_clk"
#define CLK_NAME_DPU_SCLK0		"dpu_sclk0"
#define CLK_NAME_DPU_SCLK1		"dpu_sclk1"
#define CLK_NAME_AX_CLK			"ax_clk"
#define CLK_NAME_I2S_CLK		"i2s_clk"
#define CLK_NAME_I2S0_CLK		"i2s0_clk"
#define CLK_NAME_I2S1_CLK		"i2s1_clk"
#define CLK_NAME_I2C0_CLK		"i2c0_clk"
#define CLK_NAME_I2C1_CLK		"i2c1_clk"
#define CLK_NAME_I2C2_CLK		"i2c2_clk"
#define CLK_NAME_I2C3_CLK		"i2c3_clk"
#define CLK_NAME_UART0_CLK		"uart0_clk"
#define CLK_NAME_UART1_CLK		"uart1_clk"
#define CLK_NAME_UART2_CLK		"uart2_clk"
#define CLK_NAME_UART3_CLK		"uart3_clk"
#define CLK_NAME_MBIST_CLK		"mbist_clk"

/* gate/reset clk */
#define CLK_NAME_AHB_DMA_GATE		"ahb_dma_gate"
#define CLK_NAME_AHB_PTIMER_GATE	"ahb_ptimer_gate"
#define CLK_NAME_AHB_AES_GATE		"ahb_aes_gate"
#define CLK_NAME_AHB_SDRAM_GATE		"ahb_sdram_gate"
#define CLK_NAME_AHB_SDC0_GATE		"ahb_sdc0_gate"
#define CLK_NAME_AHB_SDC1_GATE		"ahb_sdc1_gate"
#define CLK_NAME_AHB_SDC2_GATE		"ahb_sdc2_gate"
#define CLK_NAME_AHB_SPI0_GATE		"ahb_spi0_gate"
#define CLK_NAME_AHB_SPI1_GATE		"ahb_spi1_gate"
#define CLK_NAME_AHB_SPI2_GATE		"ahb_spi2_gate"
#define CLK_NAME_AHB_DMA_RESET		"ahb_dma_reset"
#define CLK_NAME_AHB_PTIMER_RESET	"ahb_ptimer_reset"
#define CLK_NAME_AHB_AES_RESET		"ahb_aes_reset"
#define CLK_NAME_AHB_SDRAM_RESET	"ahb_sdram_reset"
#define CLK_NAME_AHB_SDC0_RESET		"ahb_sdc0_reset"
#define CLK_NAME_AHB_SDC1_RESET		"ahb_sdc1_reset"
#define CLK_NAME_AHB_SDC2_RESET		"ahb_sdc2_reset"
#define CLK_NAME_AHB_SPI0_RESET		"ahb_spi0_reset"
#define CLK_NAME_AHB_SPI1_RESET		"ahb_spi1_reset"
#define CLK_NAME_AHB_SPI2_RESET		"ahb_spi2_reset"
#define CLK_NAME_AHB_VISS_GATE		"ahb_viss_gate"
#define CLK_NAME_AHB_DPU_GATE		"ahb_dpu_gate"
#define CLK_NAME_AHB_DOSS_GATE		"ahb_doss_gate"
#define CLK_NAME_AHB_VC_GATE		"ahb_vc_gate"
#define CLK_NAME_AHB_VC_DEC_GATE	"ahb_vc_dec_gate"
#define CLK_NAME_AHB_VC_ENC_GATE	"ahb_vc_enc_gate"
#define CLK_NAME_AHB_AX_GATE		"ahb_ax_gate"
#define CLK_NAME_AHB_USB_GATE		"ahb_usb_gate"
#define CLK_NAME_AHB_GMAC_GATE		"ahb_gmac_gate"
#define CLK_NAME_AHB_VISS_RESET		"ahb_viss_reset"
#define CLK_NAME_AHB_DPU_RESET		"ahb_dpu_reset"
#define CLK_NAME_AHB_DOSS_RESET		"ahb_doss_reset"
#define CLK_NAME_AHB_VC_RESET		"ahb_vc_reset"
#define CLK_NAME_AHB_VC_DEC_RESET	"ahb_vc_dec_reset"
#define CLK_NAME_AHB_VC_ENC_RESET	"ahb_vc_enc_reset"
#define CLK_NAME_AHB_AX_RESET		"ahb_ax_reset"
#define CLK_NAME_AHB_USB_RESET		"ahb_usb_reset"
#define CLK_NAME_AHB_GMAC_RESET		"ahb_gmac_reset"
#define CLK_NAME_APB_I2C0_GATE		"apb_i2c0_gate"
#define CLK_NAME_APB_I2C1_GATE		"apb_i2c1_gate"
#define CLK_NAME_APB_I2C2_GATE		"apb_i2c2_gate"
#define CLK_NAME_APB_I2C3_GATE		"apb_i2c3_gate"
#define CLK_NAME_APB_UART0_GATE		"apb_uart0_gate"
#define CLK_NAME_APB_UART1_GATE		"apb_uart1_gate"
#define CLK_NAME_APB_UART2_GATE		"apb_uart2_gate"
#define CLK_NAME_APB_UART3_GATE		"apb_uart3_gate"
#define CLK_NAME_APB_I2C0_RESET		"apb_i2c0_reset"
#define CLK_NAME_APB_I2C1_RESET		"apb_i2c1_reset"
#define CLK_NAME_APB_I2C2_RESET		"apb_i2c2_reset"
#define CLK_NAME_APB_I2C3_RESET		"apb_i2c3_reset"
#define CLK_NAME_APB_UART0_RESET	"apb_uart0_reset"
#define CLK_NAME_APB_UART1_RESET	"apb_uart1_reset"
#define CLK_NAME_APB_UART2_RESET	"apb_uart2_reset"
#define CLK_NAME_APB_UART3_RESET	"apb_uart3_reset"
#define CLK_NAME_APB_I2S_GATE		"apb_i2s_gate"
#define CLK_NAME_APB_I2S0_GATE		"apb_i2s0_gate"
#define CLK_NAME_APB_I2S1_GATE		"apb_i2s1_gate"
#define CLK_NAME_APB_GPADC_GATE		"apb_gpadc_gate"
#define CLK_NAME_APB_IR_GATE		"apb_ir_gate"
#define CLK_NAME_APB_GPIO_GATE		"apb_gpio_gate"
#define CLK_NAME_APB_I2S_RESET		"apb_i2s_reset"
#define CLK_NAME_APB_I2S0_RESET		"apb_i2s0_reset"
#define CLK_NAME_APB_I2S1_RESET		"apb_i2s1_reset"
#define CLK_NAME_APB_GPADC_RESET	"apb_gpadc_reset"
#define CLK_NAME_APB_IR_RESET		"apb_ir_reset"
#define CLK_NAME_APB_GPIO_RESET		"apb_gpio_reset"
#define CLK_NAME_MAXI_DMA_GATE		"maxi_dma_gate"
#define CLK_NAME_MAXI_VC_GATE		"maxi_vc_gate"
#define CLK_NAME_MAXI_VC_ENC_GATE	"maxi_vc_enc_gate"
#define CLK_NAME_MAXI_VC_DEC_GATE	"maxi_vc_dec_gate"
#define CLK_NAME_MAXI_VISS0_GATE	"maxi_viss0_gate"
#define CLK_NAME_MAXI_VISS1_GATE	"maxi_viss1_gate"
#define CLK_NAME_MAXI_DPU_GATE		"maxi_dpu_gate"
#define CLK_NAME_MAXI_DOSS_GATE		"maxi_doss_gate"
#define CLK_NAME_MAXI_AX_GATE		"maxi_ax_gate"
#define CLK_NAME_MAXI_GMAC_GATE		"maxi_gmac_gate"

#define	CLK_NAME_VISS_SCLK0		"viss_sclk0"
#define	CLK_NAME_VISS_SCLK1		"viss_sclk1"
#define	CLK_NAME_VISS_SCLK2		"viss_sclk2"
#define	CLK_NAME_DPU_SCLK2		"dpu_sclk2"
#define	CLK_NAME_DOSS_SCLK0		"doss_sclk0"
#define	CLK_NAME_DOSS_OSC24M		"doss_osc24m"
#define	CLK_NAME_GPADC_CLK		"gpadc_clk"
#define	CLK_NAME_IR_CLK			"ir_clk"
#define	CLK_NAME_GMAC_CLK		"gmac_clk"
#define	CLK_NAME_AES_CLK		"aes_clk"

#define CLK_NAME_USB_PHY_RESET		"usb_phy_reset"

int clk_init(void);
clk_handle_t clk_get(const char *id);
void clk_put(clk_handle_t clk);
int clk_enable(clk_handle_t clk);
void clk_disable(clk_handle_t clk);
u32 clk_get_rate(clk_handle_t clk);
int clk_set_rate(clk_handle_t clk, u32 rate);
clk_handle_t clk_get_parent(clk_handle_t clk);
int clk_set_parent(clk_handle_t clk, clk_handle_t parent);
int clk_notifier_register(clk_handle_t clk, notifier_fn_t notifier_call);
int clk_notifier_unregister(clk_handle_t clk, notifier_fn_t notifier_call);

#endif
