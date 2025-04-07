/*
 * Copyright (C) 2016-2018, LomboTech Co.Ltd.
 * Author: lomboswer <lomboswer@lombotech.com>
 *
 * Lombo n7 VISS-MIPI-CSI controller register definitions
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

/******************************************************************************
 * Controller clock and reset configuration
 *****************************************************************************/


#include "mipi_csi_csp.h"



reg_mipi_csi_t *g_mipi_csi = (reg_mipi_csi_t *)BASE_MIPI_CSI;

void csp_mcsi_set_register_base(void *base)
{
	g_mipi_csi = (reg_mipi_csi_t *)base;
}

/**
 * MIPI_CSI enable
 */
u32 csp_mcsi_enable(void)
{
	reg_mipi_csi_mcsi_ctrl_t tmpreg;

	PRT_TRACE_BEGIN("\n");
	tmpreg.val = 0;
	tmpreg.bits.mcsi_en = 1;
	WRITEREG32(&(g_mipi_csi->mcsi_ctrl), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * MIPI_CSI disable
 */
u32 csp_mcsi_disable(void)
{
	reg_mipi_csi_mcsi_ctrl_t tmpreg;

	PRT_TRACE_BEGIN("\n");
	tmpreg.val = 0;
	tmpreg.bits.mcsi_en = 0;
	WRITEREG32(&(g_mipi_csi->mcsi_ctrl), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * MIPI_CSI status
 * @busy:
 *	return 0: free
 *	return 1: busy
 * Before disable DMA or MIPI_CSI, the busy status should be 0
 */
u32 csp_mcsi_status(void)
{
	reg_mipi_csi_mcsi_cfg_t tmpreg;

	PRT_TRACE_BEGIN("\n");
	tmpreg.val = READREG32(&(g_mipi_csi->mcsi_cfg));
	PRT_TRACE_END("tmpreg.bits.busy=%d\n", tmpreg.bits.busy);

	return tmpreg.bits.busy;
}

/******************************************************************************
 * interface configure
 *****************************************************************************/

/**
 * enable mcsi output high 8bit data
 */
/* only for n7v1 */
u32 csp_enable_mcsi_output_high_8bit_data(void)
{
	reg_mipi_csi_mcsi_cfg_t tmpreg;

	tmpreg.val = READREG32(&(g_mipi_csi->mcsi_cfg));
	tmpreg.bits.data_8bit = 1;
	WRITEREG32(&(g_mipi_csi->mcsi_cfg), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * enable mcsi output raw msb data
 */
/* only for n7v1 */
u32 csp_enable_mcsi_output_raw_msb_data(void)
{
	reg_mipi_csi_mcsi_cfg_t tmpreg;

	tmpreg.val = READREG32(&(g_mipi_csi->mcsi_cfg));
	tmpreg.bits.raw_msb = 1;
	WRITEREG32(&(g_mipi_csi->mcsi_cfg), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}


/**
 * Select MIPI_CSI data path
 * @path:
 *	0: DMA channel, by default
 *	1: Pixel channel to ISP
 *	2: DMA channel and pixel channel
 */
u32 csp_mcsi_data_path(u32 path)
{
	reg_mipi_csi_mcsi_cfg_t tmpreg;

	PRT_TRACE_END("path=%d\n", path);
	tmpreg.val = READREG32(&(g_mipi_csi->mcsi_cfg));
	tmpreg.bits.mcsi_path = path;
	if (path == MCSI_OUTPUT_PIXEL_CH)
		tmpreg.bits.clk_gate = 1;
	WRITEREG32(&(g_mipi_csi->mcsi_cfg), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Select YUV componet sequence
 * @seq:
 * For YUV422 8-bit interleaved
 *	0: C0/C1/C2/C3
 *	1: C2/C1/C0/C3
 *	2: C1/C0/C3/C2
 *	3: C1/C2/C3/C0
 * In order to ensure YUV SP or Planar mode output correct data,
 * the YUV output component sequence should be adjusted to UYVY
 * If select no parse long packet, component sequence setup is useless
 */
u32 csp_mcsi_component_sequence(u32 seq)
{
	reg_mipi_csi_mcsi_pkt_parse_t tmpreg;

	PRT_TRACE_BEGIN("seq=%d\n", seq);
	tmpreg.val = READREG32(&(g_mipi_csi->mcsi_pkt_parse));
	tmpreg.bits.c_seq = seq;
	WRITEREG32(&(g_mipi_csi->mcsi_pkt_parse), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Select long packet parse
 */
u32 csp_mcsi_parse_long_pkg(void)
{
	reg_mipi_csi_mcsi_pkt_parse_t tmpreg;

	PRT_TRACE_BEGIN("\n");
	tmpreg.val = READREG32(&(g_mipi_csi->mcsi_pkt_parse));
	tmpreg.bits.lp_parse = 1;
	WRITEREG32(&(g_mipi_csi->mcsi_pkt_parse), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Not select long packet parse
 */
u32 csp_mcsi_no_parse_long_pkg(void)
{
	reg_mipi_csi_mcsi_pkt_parse_t tmpreg;

	PRT_TRACE_BEGIN("\n");
	tmpreg.val = READREG32(&(g_mipi_csi->mcsi_pkt_parse));
	tmpreg.bits.lp_parse = 0;
	WRITEREG32(&(g_mipi_csi->mcsi_pkt_parse), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Pixel channel head filter config
 * @dt: [5:0], Data type
 * @vc: [7:6], Virtual channel
 */
u32 csp_mcsi_pixel_channel_head_filter(u32 dt, u32 vc)
{
	reg_mipi_csi_pix_hf_t tmpreg;

	PRT_TRACE_BEGIN("dt=%d,vc=%d\n", dt, vc);
	tmpreg.val = READREG32(&(g_mipi_csi->pix_hf));
	tmpreg.bits.ch_dt = dt;
	tmpreg.bits.ch_vc = vc;
	WRITEREG32(&(g_mipi_csi->pix_hf), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * DMA channel head filter config
 * @dt: [5:0], Data type
 * @vc: [7:6], Virtual channel
 */
u32 csp_mcsi_dma_channel_head_filter(u32 dt, u32 vc)
{
	reg_mipi_csi_dma_hf_t tmpreg;

	PRT_TRACE_BEGIN("dt=%d,vc=%d\n", dt, vc);
	tmpreg.val = READREG32(&(g_mipi_csi->dma_hf));
	tmpreg.bits.ch_dt = dt;
	tmpreg.bits.ch_vc = vc;
	WRITEREG32(&(g_mipi_csi->dma_hf), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * DMA channel head mapping config
 * @dt: [5:0], Data type
 * @vc: [7:6], Virtual channel
 */
u32 csp_mcsi_dma_channel_head_mapping(u32 dt, u32 vc)
{
	reg_mipi_csi_mipi_hm_t tmpreg;

	PRT_TRACE_BEGIN("dt=%d,vc=%d\n", dt, vc);
	tmpreg.val = READREG32(&(g_mipi_csi->mipi_hm));
	tmpreg.bits.ch_dt = dt;
	tmpreg.bits.ch_vc = vc;
	WRITEREG32(&(g_mipi_csi->mipi_hm), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Enable MIPI_CSI interrupt
 * @interrupt:
 *	0, enable dma interrupt (MCSI_INT_DMA)
 *	1, enable mipi interrupt (MCSI_INT_MIPI)
 */
u32 csp_mcsi_int_enable(u32 interrput)
{
	reg_mipi_csi_mcsi_int_en_t tmpreg;

	PRT_TRACE_BEGIN("interrput=%d\n", interrput);
	tmpreg.val = READREG32(&(g_mipi_csi->mcsi_int_en));
	switch (interrput) {
	case 0:
		tmpreg.bits.dma_en = 1;
		break;
	case 1:
		tmpreg.bits.dphy_en = 1;
		break;
	default:
		break;
	}
	WRITEREG32(&(g_mipi_csi->mcsi_int_en), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Disable MIPI_CSI interrupt
 * @interrupt:
 *	0, diasble dma interrupt
 *	1, diasble mipi interrupt
 */
u32 csp_mcsi_int_disable(u32 interrput)
{
	reg_mipi_csi_mcsi_int_en_t tmpreg;

	PRT_TRACE_BEGIN("interrput=%d\n", interrput);
	tmpreg.val = READREG32(&(g_mipi_csi->mcsi_int_en));
	switch (interrput) {
	case 0:
		tmpreg.bits.dma_en = 0;
		break;
	case 1:
		tmpreg.bits.dphy_en = 0;
		break;
	default:
		break;
	}
	WRITEREG32(&(g_mipi_csi->mcsi_int_en), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Read MIPI_CSI interrupts' pending
 * @interrupt:
 *	0, return dma pending
 *	1, return mipi pending
 */
u32 csp_mcsi_int_pending(u32 interrput)
{
	u32 pend = 0;
	reg_mipi_csi_mcsi_int_pend_t tmpreg;

	PRT_TRACE_BEGIN("interrput=%d\n", interrput);
	tmpreg.val = READREG32(&(g_mipi_csi->mcsi_int_pend));
	switch (interrput) {
	case 0:
		pend = tmpreg.bits.dma_pend;
		break;
	case 1:
		pend = tmpreg.bits.dphy_pend;
		break;
	default:
		break;
	}
	PRT_TRACE_END("\n");

	return pend;
}

/**
 * Clear MIPI_CSI interrupt
 * @interrupt:
 *	0, clear dma pending
 *	1, clear mipi pending
 */
u32 csp_mcsi_int_clear(u32 interrput)
{
	reg_mipi_csi_mcsi_int_clr_t tmpreg;

	PRT_TRACE_BEGIN("interrput=%d\n", interrput);
	tmpreg.val = READREG32(&(g_mipi_csi->mcsi_int_clr));
	switch (interrput) {
	case 0:
		tmpreg.bits.dma_clr = 1;
		break;
	case 1:
		tmpreg.bits.dphy_clr = 1;
		break;
	default:
		break;
	}
	WRITEREG32(&(g_mipi_csi->mcsi_int_clr), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set data offset of crop function for DMA
 * @x: XS, horizontal offset
 *	a) the unit is byte; b) Align by 4-byte
 * @y: YS, vertical offset
 *	a) the unit is byte; b) Align by 4-byte
 */
u32 csp_mcsi_dma_offset(u32 x, u32 y)
{
	reg_mipi_csi_dma_ofs_t tmpreg;

	PRT_TRACE_BEGIN("x=%d,y=%d\n", x, y);
	tmpreg.val = 0;
	tmpreg.bits.x_off = x;
	tmpreg.bits.y_off = y;
	WRITEREG32(&(g_mipi_csi->dma_ofs), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set data size of crop function for DMA
 * @x: X, width
 *	a) the unit is byte; b) Align by 4-byte
 * @y: Y, height
 *	a) the unit is byte; b) Align by 4-byte
 */
u32 csp_mcsi_dma_size(u32 x, u32 y)
{
	reg_mipi_csi_dma_size_t tmpreg;

	PRT_TRACE_BEGIN("x=%d,y=%d\n", x, y);
	tmpreg.val = 0;
	tmpreg.bits.x = x;
	tmpreg.bits.y = y;
	WRITEREG32(&(g_mipi_csi->dma_size), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set DMA output format
 * @fmt: OUT_FMT
 *	0: Pass-through
 *	1: YUV422SP
 *	2: YUV420SP
 *	3: YUV422P
 *	4: YUV420P
 */
u32 csp_mcsi_dma_output_format(u32 fmt)
{
	reg_mipi_csi_dma_mode_t tmpreg;

	PRT_TRACE_BEGIN("fmt=%d\n", fmt);
	tmpreg.val = READREG32(&(g_mipi_csi->dma_mode));
	tmpreg.bits.out_fmt = fmt;
	WRITEREG32(&(g_mipi_csi->dma_mode), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set DMA burst length
 * @length: BURST
 *	0: Burst 1
 *	1: Burst 4
 *	2: Burst 8
 *	3: Burst 16
 */
u32 csp_mcsi_dma_burst_length(u32 length)
{
	reg_mipi_csi_dma_mode_t tmpreg;

	PRT_TRACE_BEGIN("length=%d\n", length);
	tmpreg.val = READREG32(&(g_mipi_csi->dma_mode));
	tmpreg.bits.burst = length;
	WRITEREG32(&(g_mipi_csi->dma_mode), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set DMA update timing of component address registers,
 * which are double-buffered
 * @timing, UD
 *	0: VBI
 *	1: FCI, the most common way
 *	2: Manually
 */
u32 csp_mcsi_dma_update_cmpt_address(u32 timing)
{
	reg_mipi_csi_dma_mode_t tmpreg;

	PRT_TRACE_BEGIN("timing=%d\n", timing);
	tmpreg.val = READREG32(&(g_mipi_csi->dma_mode));
	tmpreg.bits.ud = timing;
	WRITEREG32(&(g_mipi_csi->dma_mode), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set DMA capture mode
 * @mode, CAP
 *	0: Single mode
 *	1: Continuous mode
 */
u32 csp_mcsi_dma_capture_mode(u32 mode)
{
	reg_mipi_csi_dma_mode_t tmpreg;

	PRT_TRACE_BEGIN("mode=%d\n", mode);
	tmpreg.val = READREG32(&(g_mipi_csi->dma_mode));
	tmpreg.bits.cap = mode;
	WRITEREG32(&(g_mipi_csi->dma_mode), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set DMA start
 */
u32 csp_mcsi_dma_start(void)
{
	reg_mipi_csi_dma_ctrl_t tmpreg;

	PRT_TRACE_BEGIN("\n");
	tmpreg.val = 0;
	tmpreg.bits.start = 1;
	WRITEREG32(&(g_mipi_csi->dma_ctrl), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set DMA stop
 */
u32 csp_mcsi_dma_stop(void)
{
	reg_mipi_csi_dma_ctrl_t tmpreg;

	PRT_TRACE_BEGIN("\n");
	tmpreg.val = 0;
	tmpreg.bits.start = 0;
	WRITEREG32(&(g_mipi_csi->dma_ctrl), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set DMA update component address registers manually
 */
u32 csp_mcsi_dma_update_manual(void)
{
	reg_mipi_csi_dma_update_t tmpreg;

	PRT_TRACE_BEGIN("\n");
	tmpreg.val = 0;
	tmpreg.bits.update_addr = 1;
	WRITEREG32(&(g_mipi_csi->dma_update), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set DMA component0 output address
 * @addr: output address
 */
u32 csp_mcsi_dma_cmpt0_addr(u32 addr)
{
	reg_mipi_csi_dma_addr0_t tmpreg;

	PRT_TRACE_BEGIN("addr=0x%08x\n", addr);
	tmpreg.val = 0;
	tmpreg.bits.addr = addr;
	WRITEREG32(&(g_mipi_csi->dma_addr0), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set DMA component1 output address
 * @addr: output address
 */
u32 csp_mcsi_dma_cmpt1_addr(u32 addr)
{
	reg_mipi_csi_dma_addr1_t tmpreg;

	PRT_TRACE_BEGIN("addr=0x%08x\n", addr);
	tmpreg.val = 0;
	tmpreg.bits.addr = addr;
	WRITEREG32(&(g_mipi_csi->dma_addr1), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set DMA component2 output address
 * @addr: output address
 */
u32 csp_mcsi_dma_cmpt2_addr(u32 addr)
{
	reg_mipi_csi_dma_addr2_t tmpreg;

	PRT_TRACE_BEGIN("addr=0x%08x\n", addr);
	tmpreg.val = 0;
	tmpreg.bits.addr = addr;
	WRITEREG32(&(g_mipi_csi->dma_addr2), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set data line stride of DMA
 * @luma: luma line stride
 * @chroma: chroma line stride
 */
u32 csp_mcsi_dma_linestride(u32 luma, u32 chroma)
{
	reg_mipi_csi_dma_ls_t tmpreg;

	PRT_TRACE_BEGIN("luma=%d,chroma=%d\n", luma, chroma);
	tmpreg.val = 0;
	tmpreg.bits.y = luma;
	tmpreg.bits.c = chroma;
	WRITEREG32(&(g_mipi_csi->dma_ls), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Read DMA current byte counter, which would be reset by every VSYNC
 */
u32 csp_mcsi_dma_cur_byte_cnt(void)
{
	reg_mipi_csi_dma_bc_t tmpreg;

	PRT_TRACE_BEGIN("\n");
	tmpreg.val = READREG32(&(g_mipi_csi->dma_bc));
	PRT_TRACE_END("tmpreg.bits.bc=%d\n", tmpreg.bits.bc);

	return tmpreg.bits.bc;
}

/**
 * Enable DMA interrupts
 * @interrupt: interrput bit to enable
 */
u32 csp_mcsi_dma_int_enable(u32 interrupt)
{
	reg_mipi_csi_dma_int_en_t tmpreg;

	PRT_TRACE_BEGIN("interrupt=%d\n", interrupt);
	tmpreg.val = READREG32(&(g_mipi_csi->dma_int_en));
	switch (interrupt) {
	case 0:
		tmpreg.bits.fci_en = 1;
		break;
	case 1:
		tmpreg.bits.vbi_en = 1;
		break;
	case 2:
		tmpreg.bits.lcti_en = 1;
		break;
	case 8:
		tmpreg.bits.p0_ovf_en = 1;
		break;
	case 9:
		tmpreg.bits.p1_ovf_en = 1;
		break;
	case 10:
		tmpreg.bits.p2_ovf_en = 1;
		break;
	default:
		break;
	}
	WRITEREG32(&(g_mipi_csi->dma_int_en), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set DMA LCTI trigger value
 * @line: line count to trigger LCTI
 */
u32 csp_mcsi_dma_lcti_trig_val(u32 line)
{
	reg_mipi_csi_dma_line_cnt_t tmpreg;

	PRT_TRACE_BEGIN("line=%d\n", line);
	tmpreg.val = READREG32(&(g_mipi_csi->dma_line_cnt));
	tmpreg.bits.ls_trig = line;
	WRITEREG32(&(g_mipi_csi->dma_line_cnt), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Read DMA current line counter
 */
u32 csp_mcsi_dma_cur_line_cnt(void)
{
	reg_mipi_csi_dma_line_cnt_t tmpreg;

	PRT_TRACE_BEGIN("\n");
	tmpreg.val = READREG32(&(g_mipi_csi->dma_line_cnt));
	PRT_TRACE_END("tmpreg.bits.ls_crnt=%d\n", tmpreg.bits.ls_crnt);

	return tmpreg.bits.ls_crnt;
}

void csp_mcsi_cfg_fifo(void)
{
	return;
}

#define MIPI_CSI_LBOR_BIT	13
#define MIPI_CSI_RST_BIT	3

u32 csp_mcsi_chk_lbor(void)
{
	u32 tmpval;
	u32 ret;

	PRT_TRACE_BEGIN("\n");
	tmpval = READREG32(&(g_mipi_csi->prv_0600));
	ret = (tmpval >> MIPI_CSI_LBOR_BIT) & 1;
	PRT_TRACE_END("lb_or=%d\n", ret);

	return ret;
}

/**
 * Reset DMA circuit
 */
u32 csp_mcsi_dma_reset(void)
{
	u32 tmpval;

	PRT_TRACE_BEGIN("\n");
	tmpval = READREG32(&(g_mipi_csi->prv_0600));
	tmpval |= (1 << MIPI_CSI_RST_BIT);
	WRITEREG32(&(g_mipi_csi->prv_0600), tmpval);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Reset MIPI_CSI circuit
 */
u32 csp_mcsi_reset(void)
{
	u32 tmpval;

	PRT_TRACE_BEGIN("\n");
	tmpval = READREG32(&(g_mipi_csi->prv_0600));
	tmpval = tmpval | (1 << MIPI_CSI_RST_BIT);
	WRITEREG32(&(g_mipi_csi->prv_0600), tmpval);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Config MIPI DPHY active lanes
 */
u32 csp_mipi_dphy_active_lane(u32 lane)
{
	reg_mipi_csi_n_lanes_t tmpreg;

	PRT_TRACE_BEGIN("lane=%d\n", lane);
	tmpreg.val = 0;
	tmpreg.bits.nal = lane - 1;
	WRITEREG32(&(g_mipi_csi->n_lanes), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * MIPI DPHY shutdown
 */
u32 csp_mipi_dphy_shutdown(void)
{
	reg_mipi_csi_phy_shutdownz_t tmpreg;

	PRT_TRACE_BEGIN("\n");
	tmpreg.val = 0;
	tmpreg.bits.shutdown = 0;
	WRITEREG32(&(g_mipi_csi->phy_shutdownz), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * MIPI DPHY power up
 */
u32 csp_mipi_dphy_power_up(void)
{
	reg_mipi_csi_phy_shutdownz_t tmpreg;

	PRT_TRACE_BEGIN("\n");
	tmpreg.val = 0;
	tmpreg.bits.shutdown = 1;
	WRITEREG32(&(g_mipi_csi->phy_shutdownz), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * MIPI DPHY reset active
 */
u32 csp_mipi_dphy_reset_active(void)
{
	reg_mipi_csi_dphy_rstz_t tmpreg;

	PRT_TRACE_BEGIN("\n");
	tmpreg.val = 0;
	tmpreg.bits.dphy_rst = 0;
	WRITEREG32(&(g_mipi_csi->dphy_rstz), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * MIPI DPHY reset in-active
 */
u32 csp_mipi_dphy_reset_inactive(void)
{
	reg_mipi_csi_dphy_rstz_t tmpreg;

	PRT_TRACE_BEGIN("\n");
	tmpreg.val = 0;
	tmpreg.bits.dphy_rst = 1;
	WRITEREG32(&(g_mipi_csi->dphy_rstz), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * MIPI DPHY controller reset active
 */
u32 csp_mipi_dphy_controller_reset_active(void)
{
	reg_mipi_csi_csi2_resetn_t tmpreg;

	PRT_TRACE_BEGIN("\n");
	tmpreg.val = 0;
	tmpreg.bits.ctl_rst = 0;
	WRITEREG32(&(g_mipi_csi->csi2_resetn), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * MIPI DPHY controller reset in-active
 */
u32 csp_mipi_dphy_controller_reset_inactive(void)
{
	reg_mipi_csi_csi2_resetn_t tmpreg;

	PRT_TRACE_BEGIN("\n");
	tmpreg.val = 0;
	tmpreg.bits.ctl_rst = 1;
	WRITEREG32(&(g_mipi_csi->csi2_resetn), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * MIPI DPHY status
 */
u32 csp_mipi_dphy_state(void)
{
	reg_mipi_csi_phy_state_t tmpreg;

	PRT_TRACE_BEGIN("\n");
	tmpreg.val = READREG32(&(g_mipi_csi->phy_state));
	PRT_TRACE_END("tmpreg.val=%d\n", tmpreg.val);

	return tmpreg.val;
}

/**
 * Config MIPI DPHY data ID
 * @id: data id
 * @dt: [5:0], Data type
 * @vc: [7:6], Virtual channel
 */
u32 csp_mipi_dphy_data_id(u32 id, u32 dt, u32 vc)
{
	reg_mipi_csi_data_ids_t tmpreg;

	PRT_TRACE_BEGIN("id=%d,dt=%d,vc=%d\n", id, dt, vc);
	tmpreg.val = READREG32(&(g_mipi_csi->data_ids));
	switch (id) {
	case 0:
		tmpreg.bits.di0_dt = dt;
		tmpreg.bits.di0_vc = vc;
		break;
	case 1:
		tmpreg.bits.di1_dt = dt;
		tmpreg.bits.di1_vc = vc;
		break;
	case 2:
		tmpreg.bits.di2_dt = dt;
		tmpreg.bits.di2_vc = vc;
		break;
	case 3:
		tmpreg.bits.di3_dt = dt;
		tmpreg.bits.di3_vc = vc;
		break;
	default:
		break;
	}
	WRITEREG32(&(g_mipi_csi->data_ids), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Read MIPI DPHY register
 * @addr: register address to read
 *
 * return the read value
 */
u32 csp_mipi_dphy_read(u8 addr)
{
	u32 rval;

	PRT_TRACE_BEGIN("addr=%d\n", addr);
	WRITEREG32(&g_mipi_csi->phy_tst_ctrl0, 0x2);
	WRITEREG32(&g_mipi_csi->phy_tst_ctrl1, (1 << 16) | addr);
	WRITEREG32(&g_mipi_csi->phy_tst_ctrl0, 0x0);

	rval = (READREG32(&g_mipi_csi->phy_tst_ctrl1) >> 8) & 0xFF;
	PRT_TRACE_END("rval=%d\n", rval);

	return rval;
}

/**
 * Write MIPI DPHY register
 * @addr: register address to read
 * @wval: write value
 */
u32 csp_mipi_dphy_write(u8 addr, u8 wval)
{
	PRT_TRACE_BEGIN("addr=%d,wval=%d\n", addr, wval);
	WRITEREG32(&g_mipi_csi->phy_tst_ctrl0, 0x2);
	WRITEREG32(&g_mipi_csi->phy_tst_ctrl1, (1 << 16) | addr);
	WRITEREG32(&g_mipi_csi->phy_tst_ctrl0, 0x0);
	WRITEREG32(&g_mipi_csi->phy_tst_ctrl1, wval);
	WRITEREG32(&g_mipi_csi->phy_tst_ctrl0, 0x2);
	PRT_TRACE_END("\n");

	return 0;
}
#if 0
/**
  * MIPI DPHY Frequency Table
*/
typedef struct dphy_freq_map {
	u32 freq;
	u32 freq_code;
} dphy_freq_map_t;

static dphy_freq_map_t dphy_freq_table[] = {
	{ 90000000,   0x00 },
	{ 99000000,   0x10 },
	{ 108000000,  0x20 },
	{ 123000000,  0x01 },
	{ 135000000,  0x11 },
	{ 150000000,  0x21 },
	{ 159000000,  0x02 },
	{ 180000000,  0x12 },
	{ 198000000,  0x22 },
	{ 210000000,  0x03 },
	{ 240000000,  0x13 },
	{ 249000000,  0x23 },
	{ 270000000,  0x04 },
	{ 300000000,  0x14 },
	{ 330000000,  0x05 },
	{ 360000000,  0x15 },
	{ 399000000,  0x25 },
	{ 450000000,  0x06 },
	{ 486000000,  0x16 },
	{ 549000000,  0x07 },
	{ 600750000,  0x17 },
	{ 648000000,  0x08 },
	{ 702000000,  0x18 },
	{ 756000000,  0x09 },
	{ 783000000,  0x19 },
	{ 850500000,  0x0A },
	{ 904500000,  0x1A },
	{ 972000000,  0x2A },
	{ 999000000,  0x3A },
	{ 1053000000, 0x0B },
	{ 1107000000, 0x1B },
	{ 1147500000, 0x2B },
	{ 1188000000, 0x3B },
};
#endif
/*only for n7v1 */
u32 mipi_dphy_read(u8 addr, u8 set)
{
	u32 rval;

	WRITEREG32(&g_mipi_csi->phy_tst_ctrl1, (1<<16)|addr);
	WRITEREG32(&g_mipi_csi->phy_tst_ctrl0, 0x2);
	WRITEREG32(&g_mipi_csi->phy_tst_ctrl0, 0x0);
	WRITEREG32(&g_mipi_csi->phy_tst_ctrl1, (set<<6));

	rval = (READREG32(&g_mipi_csi->phy_tst_ctrl1)>>8)&0xFF;

	return rval;
}

/*only for n7v1 */
void mipi_dphy_write(u8 addr, u8 wval)
{
	WRITEREG32(&g_mipi_csi->phy_tst_ctrl1, (1<<16)|addr);
	WRITEREG32(&g_mipi_csi->phy_tst_ctrl0, 0x2);
	WRITEREG32(&g_mipi_csi->phy_tst_ctrl0, 0x0);
	WRITEREG32(&g_mipi_csi->phy_tst_ctrl1, wval);
	WRITEREG32(&g_mipi_csi->phy_tst_ctrl0, 0x2);

	WRITEREG32(&g_mipi_csi->phy_tst_ctrl0, 0x0);

}

void mipi_phy_dlane_settle(u32 dlane_sett)
{	u32 val, phyreg;

	/* hsrxsettle bypass data lane [7] */
	/* hsrxsettle data lane int [6:1] */
	/* &0x0 is because hsrxsettle data lane int [6:1] have default value */
	val = (mipi_dphy_read(0x45, 0x00) & (0x00)) | (0x80);
	mipi_dphy_write(0x45, val);
	phyreg = mipi_dphy_read(0x45, 0x00);
	/* printk("\nPHY 0x45 register value is %x\n\n", phyreg); */

	val = mipi_dphy_read(0x45, 0x00) | (dlane_sett<<1);
	mipi_dphy_write(0x45, val);
	phyreg = mipi_dphy_read(0x45, 0x00);
	/* printk("\nPHY 0x45 register value is %x\n\n", phyreg); */

}

/**
  * MIPI DPHY Init
  * @lane: data lane count
  * @id: data id
  * @dt: Data type
  * @vc: Virtual channel
  * @freq: frequency for MIPI DPHY
  */
/*only for n7v1 */
u32 csp_mipi_dphy_init_1(u32 lane, u32 id, u32 dt, u32 vc, u32 freq)
{
	u32 code, sett;
#if 0
	void __iomem *sau_addr;
	reg_sau_t *sau;
	PRT_TRACE_BEGIN("lane=%d,id=%d,dt=%d,vc=%d,freq=%d\n",
				lane, id, dt, vc, freq);
	sau_addr = ioremap(0x01204000, 0x1000);
	sau = (reg_sau_t *)sau_addr;
	/* switch DSI to CSI */
	WRITEREG32(&sau->mipidphy_cfg, 1);
#endif
	csp_mipi_dphy_active_lane(lane);

	WRITEREG32(&g_mipi_csi->phy_tst_ctrl0, 0x1);
	WRITEREG32(&g_mipi_csi->phy_tst_ctrl1, 0x0);
	WRITEREG32(&g_mipi_csi->phy_tst_ctrl0, 0x0);

	csp_mipi_dphy_data_id(id, dt, vc);
#if 0
	/* find the matched frequency */
	for (i = 0;
	i < sizeof(dphy_freq_table) / sizeof(dphy_freq_table[0]); i++) {
		if (dphy_freq_table[i].freq >= freq)
			break;
	}
	if (i < sizeof(dphy_freq_table) / sizeof(dphy_freq_table[0]))
		code = dphy_freq_table[i].freq_code;
	else
		code = 0;
#endif

	if (freq >= 1150000000) {
		code = 0x3B;/* 1188000000 */
		sett = 17;
		}
	else if (freq >= 1100000000) {
		code = 0x2B;/* 1147500000 */
		sett = 17;
		}
	else if (freq >= 1050000000) {
		code = 0x1B;/* 1107000000 */
		sett = 16;
		}
	else if (freq >= 1000000000) {
		code = 0x0B;/* 1053000000 */
		sett = 15;
		}
	else if (freq >= 950000000) {
		code = 0x3A;/* 999000000 */
		sett = 14;
		}
	else if (freq >= 900000000) {
		code = 0x2A;/* 972000000 */
		sett = 14;
		}
	else if (freq >= 850000000) {
		code = 0x1A;/* 904500000 */
		sett = 13;
		}
	else if (freq >= 800000000) {
		code = 0x0A;/* 850500000 */
		sett = 12;
		}
	else if (freq >= 750000000) {
		code = 0x19;/* 783000000 */
		sett = 11;
		}
	else if (freq >= 700000000) {
		code = 0x09;/* 756000000 */
		sett = 11;
		}
	else if (freq >= 650000000) {
		code = 0x18;/* 702000000 */
		sett = 10;
		}
	else if (freq >= 600000000) {
		code = 0x08;/* 648000000 */
		sett = 9;
		}
	else if (freq >= 550000000) {
		code = 0x17;/* 600750000 */
		sett = 9;
		}
	else if (freq >= 500000000) {
		code = 0x07;/* 549000000 */
		sett = 8;
		}
	else if (freq >= 450000000) {
		code = 0x16;/* 486000000 */
		sett = 7;
		}
	else if (freq >= 400000000) {
		code = 0x06;/* 450000000 */
		sett = 7;
		}
	else if (freq >= 360000000) {
		code = 0x25;/* 399000000 */
		sett = 6;
		}
	else if (freq >= 330000000) {
		code = 0x15;/* 360000000 */
		sett = 5;
		}
	else if (freq >= 300000000) {
		code = 0x05;/* 330000000 */
		sett = 5;
		}
	else if (freq >= 270000000) {
		code = 0x14;/* 300000000 */
		sett = 5;
		}
	else if (freq >= 250000000) {
		code = 0x04;/* 270000000 */
		sett = 4;
		}
	else if (freq >= 240000000) {
		code = 0x23;/* 249000000 */
		sett = 4;
		}
	else if (freq >= 210000000) {
		code = 0x13;/* 240000000 */
		sett = 4;
		}
	else if (freq >= 200000000) {
		code = 0x03;/* 210000000 */
		sett = 3;
		}
	else if (freq >= 180000000) {
		code = 0x22;/* 198000000 */
		sett = 3;
		}
	else if (freq >= 160000000) {
		code = 0x12;/* 180000000 */
		sett = 3;
		}
	else if (freq >= 150000000) {
		code = 0x02;/* 159000000 */
		sett = 2;
		}
	else if (freq >= 140000000) {
		code = 0x21;/* 150000000 */
		sett = 2;
		}
	else if (freq >= 125000000) {
		code = 0x11;/* 135000000 */
		sett = 2;
		}
	else if (freq >= 110000000) {
		code = 0x01;/* 123000000 */
		sett = 2;
		}
	else if (freq >= 100000000) {
		code = 0x20;/* 108000000 */
		sett = 2;
		}
	else if (freq >= 90000000) {
		code = 0x10;/* 99M */
		sett = 2;
		}
	else if (freq > 80000000) {
		code = 0x00;/* ORG 90M */
		sett = 1;
		}
	else {
		code = 0x00;
		sett = 1;
	}
	/* only for BG0806 */
	#if 0
	mipi_dphy_write(0x35, 0x02);/* FORCE CLOCK MISS DISABLE */
	rval = mipi_dphy_read(0x35, 0x00);
	#endif

	PRT_TRACE_BEGIN("==============code %u=============\n", code);
	mipi_phy_dlane_settle(sett);
	mipi_dphy_write(0x0F, code);

	csp_mipi_dphy_controller_reset_inactive();
	csp_mipi_dphy_power_up();
	csp_mipi_dphy_reset_inactive();
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * MIPI DPHY Init
 * @lane: data lane count
 * @id: data id
 * @dt: Data type
 * @vc: Virtual channel
 * @freq: frequency for MIPI DPHY
 */
u32 csp_mipi_dphy_init_0(u32 lane, u32 id, u32 dt, u32 vc, u32 freq)
{
	PRT_TRACE_BEGIN("lane=%d,id=%d,dt=%d,vc=%d,freq=%d\n",
			lane, id, dt, vc, freq);
	csp_mipi_dphy_active_lane(lane);

	WRITEREG32(&g_mipi_csi->phy_tst_ctrl0, 0x1);
	WRITEREG32(&g_mipi_csi->phy_tst_ctrl1, 0x0);
	WRITEREG32(&g_mipi_csi->phy_tst_ctrl0, 0x0);

	csp_mipi_dphy_data_id(id, dt, vc);

	if (freq > 950000000)
		csp_mipi_dphy_write(0x44, 0x74);
	else if (freq > 900000000)
		csp_mipi_dphy_write(0x44, 0x54);
	else if (freq > 850000000)
		csp_mipi_dphy_write(0x44, 0x34);
	else if (freq > 800000000)
		csp_mipi_dphy_write(0x44, 0x14);
	else if (freq > 750000000)
		csp_mipi_dphy_write(0x44, 0x32);
	else if (freq > 700000000)
		csp_mipi_dphy_write(0x44, 0x12);
	else if (freq > 650000000)
		csp_mipi_dphy_write(0x44, 0x30);
	else if (freq > 600000000)
		csp_mipi_dphy_write(0x44, 0x10);
	else if (freq > 550000000)
		csp_mipi_dphy_write(0x44, 0x2E);
	else if (freq > 500000000)
		csp_mipi_dphy_write(0x44, 0x0E);
	else if (freq > 450000000)
		csp_mipi_dphy_write(0x44, 0x2C);
	else if (freq > 400000000)
		csp_mipi_dphy_write(0x44, 0x0C);
	else if (freq > 360000000)
		csp_mipi_dphy_write(0x44, 0x4A);
	else if (freq > 330000000)
		csp_mipi_dphy_write(0x44, 0x2A);
	else if (freq > 300000000)
		csp_mipi_dphy_write(0x44, 0x08);
	else if (freq > 270000000)
		csp_mipi_dphy_write(0x44, 0x28);
	else if (freq > 250000000)
		csp_mipi_dphy_write(0x44, 0x08);
	else if (freq > 240000000)
		csp_mipi_dphy_write(0x44, 0x46);
	else if (freq > 210000000)
		csp_mipi_dphy_write(0x44, 0x26);
	else if (freq > 200000000)
		csp_mipi_dphy_write(0x44, 0x06);
	else if (freq > 180000000)
		csp_mipi_dphy_write(0x44, 0x44);
	else if (freq > 160000000)
		csp_mipi_dphy_write(0x44, 0x24);
	else if (freq > 150000000)
		csp_mipi_dphy_write(0x44, 0x04);
	else if (freq > 140000000)
		csp_mipi_dphy_write(0x44, 0x42);
	else if (freq > 125000000)
		csp_mipi_dphy_write(0x44, 0x22);
	else if (freq > 110000000)
		csp_mipi_dphy_write(0x44, 0x02);
	else if (freq > 100000000)
		csp_mipi_dphy_write(0x44, 0x40);
	else if (freq > 90000000)
		csp_mipi_dphy_write(0x44, 0x20);
	else if (freq > 80000000)
		csp_mipi_dphy_write(0x44, 0x00);

	csp_mipi_dphy_controller_reset_inactive();
	csp_mipi_dphy_power_up();
	csp_mipi_dphy_reset_inactive();
	PRT_TRACE_END("\n");

	return 0;
}


/******************************************************************************
 * status & inturrupts
 *****************************************************************************/

/**
 * csp_mcsi_clear_set_ints - clear some int enable bit,
 *			    and than set other int enable bit
 * @clear_mask:	int enable bits to be clear
 * @set_mask:	int enable bits to be set
 */
void csp_mcsi_clear_set_ints(u32 clear_mask, u32 set_mask)
{
	u32 int_en;

	PRT_TRACE_BEGIN("clear_mask=0x%08x,set_mask=0x%08x\n",
			clear_mask, set_mask);

	int_en = READREG32(&g_mipi_csi->dma_int_en);
	int_en &= ~clear_mask;
	int_en |= set_mask;
	WRITEREG32(&g_mipi_csi->dma_int_en, int_en);
	PRT_TRACE_END("\n");
}

/**
 * csp_sdc_enable_ints - enable the specified interrputs
 * @int_mask:	int enable bits to be set
 */
void csp_mcsi_enable_ints(u32 int_mask)
{
	u32 int_en;

	PRT_TRACE_BEGIN("int_mask=0x%08x\n", int_mask);
	int_en = READREG32(&g_mipi_csi->dma_int_en);
	int_en |= int_mask;
	WRITEREG32(&g_mipi_csi->dma_int_en, int_en);
	PRT_TRACE_END("\n");
}

/**
 * csp_mcsi_disable_ints - enable the specified interrputs
 * @int_mask:	int enable bits to be set
 */
void csp_mcsi_disable_ints(u32 int_mask)
{
	u32 int_en;

	PRT_TRACE_BEGIN("int_mask=0x%08x\n", int_mask);
	int_en = READREG32(&g_mipi_csi->dma_int_en);
	int_en &= ~int_mask;
	WRITEREG32(&g_mipi_csi->dma_int_en, int_en);
	PRT_TRACE_END("\n");
}

/**
 * csp_mcsi_clr_pd - clear int pending status
 * @ch:		dma chanel index
 * @pd_mask:	int pending bits status to be clear
 */
void csp_mcsi_clr_pd(u32 pd_mask)
{
	PRT_TRACE_BEGIN("pd_mask=0x%08x\n", pd_mask);
	WRITEREG32(&g_mipi_csi->dma_int_clr, pd_mask);
	PRT_TRACE_END("\n");
}

/**
 * csp_mcsi_get_int_pd - get int pending status
 */
u32 csp_mcsi_get_int_pd(void)
{
	PRT_TRACE_BEGIN("\n");
	PRT_TRACE_END("pd=0x%08x\n", READREG32(&g_mipi_csi->dma_int_pend));

	return READREG32(&g_mipi_csi->dma_int_pend);
}

/**
 * csp_mcsi_chk_err - check whether there are error pendings occur
 *
 * return 0 when not error, otherwise 1
 */
u32 csp_mcsi_chk_err(void)
{
	u32 int_pd_tmp = 0;
	u32 ret = 0;

	PRT_TRACE_BEGIN("\n");
	int_pd_tmp = READREG32(&g_mipi_csi->dma_int_pend);
	/* report error when any error pending bit occur */
	if ((int_pd_tmp & MCSI_DMA_INT_ALL_ERR) != 0) {
		LOG_E("int_pd=%x\n", int_pd_tmp);
		ret = 1;
	}
	PRT_TRACE_END("\n");

	return ret;
}

/**
 * csp_mcsi_chk_pd - check whether all the expected int pending have been raise
 * @pd_mask:	int pending bits to be check
 *
 * return 1 when all the expected pending bits are set, otherwise 0
 */
u32 csp_mcsi_chk_pd(u32 pd_mask)
{
	u32 int_pd_tmp;
	u32 ret = 0;

	PRT_TRACE_BEGIN("pd_mask=0x%08x\n", pd_mask);
	int_pd_tmp = READREG32(&g_mipi_csi->dma_int_pend);

	/* return 1 only when all bits in pd_mask are set */
	if ((int_pd_tmp & pd_mask) == pd_mask)
		ret = 1;
	PRT_TRACE_END("ret=%d\n", ret);

	return ret;
}

/**
 * csp_mcsi_dump_regs - dump all the register
 * @func_name:	the name of the function that want dump registers
 */
void csp_mcsi_dump_regs(const char *func_name)
{
#if 0
	u32 offset = 0;

	PRT_DBG(" =========== (%s) ===========\n",
		func_name);
#if 0
	for (offset = 0; offset < 0x1CC; offset += 8) {
		PRT_DBG(" 0x%x: 0x%08x | 0x%x: 0x%08x\n",
			offset, READREG32((u32)g_mipi_csi + offset),
			offset + 4, READREG32((u32)g_mipi_csi + offset + 4));
	}
#endif

	for (offset = 0x600; offset < 0x610; offset += 8) {
		PRT_DBG(" 0x%x: 0x%08x | 0x%x: 0x%08x\n",
			offset, READREG32((u32)g_mipi_csi + offset),
			offset + 4, READREG32((u32)g_mipi_csi + offset + 4));
	}

	for (offset = 0x1004; offset < 0x1064; offset += 8) {
		PRT_DBG(" 0x%x: 0x%08x | 0x%x: 0x%08x\n",
			offset, READREG32((u32)g_mipi_csi + offset),
			offset + 4, READREG32((u32)g_mipi_csi + offset + 4));
	}
	PRT_DBG(" ===========================================\n");
#endif
}
