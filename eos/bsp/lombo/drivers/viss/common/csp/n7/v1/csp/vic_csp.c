/*
 * Copyright (C) 2016-2018, LomboTech Co.Ltd.
 * Author: lomboswer <lomboswer@lombotech.com>
 *
 * Lombo n7 VISS-VIC controller register definitions
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
 * base operations
 *****************************************************************************/


#include "vic_csp.h"

reg_vic_t *g_vic = (reg_vic_t *)BASE_VIC;

void csp_vic_set_register_base(void *base)
{
	g_vic = (reg_vic_t *)base;
}

/**
 * VIC enable
 */
u32 csp_vic_enable(void)
{
	reg_vic_vic_ctrl_t tmpreg;
	PRT_TRACE_BEGIN("\n");
	tmpreg.val = 0;
	tmpreg.bits.vic_en = 1;
	WRITEREG32(&(g_vic->vic_ctrl), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * VIC disable
 */
u32 csp_vic_disable(void)
{
	reg_vic_vic_ctrl_t tmpreg;
	PRT_TRACE_BEGIN("\n");
	tmpreg.val = 0;
	tmpreg.bits.vic_en = 0;
	WRITEREG32(&(g_vic->vic_ctrl), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * VIC status
 *	return 0: free
 *	return 1: busy
 * Before disable DMA or VIC, the busy status should be 0
 */
u32 csp_vic_status(void)
{
	reg_vic_vic_cfg_t tmpreg;
	PRT_TRACE_BEGIN("\n");
	tmpreg.val = READREG32(&(g_vic->vic_cfg));
	PRT_TRACE_END("tmpreg.bits.busy=%d\n", tmpreg.bits.busy);

	return tmpreg.bits.busy;
}

/******************************************************************************
 * interface configure
 *****************************************************************************/

/**
 * enable vic output high 8bit data
 */
/* only for n7v1 */
u32 csp_enable_vic_output_high_8bit_data(void)
{
	reg_vic_vic_cfg_t tmpreg;

	tmpreg.val = READREG32(&(g_vic->vic_cfg));
	tmpreg.bits.data_8bit = 1;
	WRITEREG32(&(g_vic->vic_cfg), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * enable vic output raw msb data
 */
/* only for n7v1 */
u32 csp_enable_vic_output_raw_msb_data(void)
{
	reg_vic_vic_cfg_t tmpreg;

	tmpreg.val = READREG32(&(g_vic->vic_cfg));
	tmpreg.bits.raw_msb = 1;
	WRITEREG32(&(g_vic->vic_cfg), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Select VIC data path
 * @path:
 *	0: DMA channel, by default
 *	1: Pixel channel to ISP
 *	2: DMA channel and pixel channel
 */
u32 csp_vic_data_path(u32 path)
{
	reg_vic_vic_cfg_t tmpreg;
	PRT_TRACE_END("path=%d\n", path);
	tmpreg.val = READREG32(&(g_vic->vic_cfg));
	tmpreg.bits.dvp_path = path;
	WRITEREG32(&(g_vic->vic_cfg), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set PCLK polarity
 * @pol: PCLK_INV
 *	0: Polarity not inverted
 *	1: Polarity inverted
 */
u32 csp_vic_set_pclk_polarity(u32 pol)
{
	reg_vic_io_cfg_t tmpreg;
	PRT_TRACE_BEGIN("pol=%d\n", pol);
	tmpreg.val = READREG32(&(g_vic->io_cfg));
	tmpreg.bits.pclk_inv = pol;
	WRITEREG32(&(g_vic->io_cfg), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set Filed polarity
 * @pol: FIELD_INV
 *	0: Polarity not inverted
 *	1: Polarity inverted
 */
u32 csp_vic_set_field_polarity(u32 pol)
{
	reg_vic_io_cfg_t tmpreg;

	PRT_TRACE_BEGIN("pol=%d\n", pol);
	tmpreg.val = READREG32(&(g_vic->io_cfg));
	tmpreg.bits.field_inv = pol;
	WRITEREG32(&(g_vic->io_cfg), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set HSYNC polarity
 * @pol: HS_INV
 *	0: Polarity not inverted
 *	1: Polarity inverted
 */
u32 csp_vic_set_hsync_polarity(u32 pol)
{
	reg_vic_io_cfg_t tmpreg;

	PRT_TRACE_BEGIN("pol=%d\n", pol);
	tmpreg.val = READREG32(&(g_vic->io_cfg));
	tmpreg.bits.hs_inv = pol;
	WRITEREG32(&(g_vic->io_cfg), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set VSYNC polarity
 * @pol: VS_INV
 *	0: Polarity not inverted
 *	1: Polarity inverted
 */
u32 csp_vic_set_vsync_polarity(u32 pol)
{
	reg_vic_io_cfg_t tmpreg;

	PRT_TRACE_BEGIN("pol=%d\n", pol);
	tmpreg.val = READREG32(&(g_vic->io_cfg));
	tmpreg.bits.vs_inv = pol;
	WRITEREG32(&(g_vic->io_cfg), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set PCLK phase
 * @phase:
 *	Delay time = phase_value * 0.2ns
 */
u32 csp_vic_set_pclk_phase(u32 phase)
{
	reg_vic_io_cfg_t tmpreg;

	PRT_TRACE_BEGIN("phase=%d\n", phase);
	tmpreg.val = READREG32(&(g_vic->io_cfg));
	tmpreg.bits.pclk_phase = phase;
	WRITEREG32(&(g_vic->io_cfg), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Select DVP data bus protocol
 * @ptcl: DATA_PTCL
 *	0: RAW8
 *	1: RAW10
 *	2: RAW12
 *	3: YUV422 8-bit interleaved
 *	5: YUV422 8-bit separated, which means data bus is 16-bit
 */
u32 csp_vic_select_data_protocol(u32 ptcl)
{
	reg_vic_if_cfg_t tmpreg;

	PRT_TRACE_BEGIN("ptcl=%d\n", ptcl);
	tmpreg.val = READREG32(&(g_vic->if_cfg));
	tmpreg.bits.data_ptcl = ptcl;
	WRITEREG32(&(g_vic->if_cfg), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Select YUV componet sequence
 * @seq: YC_SEQ, For YUV422 8-bit interleaved
 *	0: C0/C1/C2/C3
 *	1: C2/C1/C0/C3
 *	2: C1/C0/C3/C2
 *	3: C1/C2/C3/C0
 * For YUV422 16-bit separated
 *	0: D[15:8]->Y0Y1, D[7:0]->UV
 *	1: D[15:8]->Y0Y1, D[7:0]->VU
 *	2: D[15:8]->UV, D[7:0]->Y0Y1
 *	3: D[15:8]->VU, D[7:0]->Y0Y1
 * In order to ensure YUV SP or Planar mode output correct data,
 * the YUV output component sequence should be adjusted to UYVY
 */
u32 csp_vic_set_yuv_seq(u32 seq)
{
	reg_vic_if_cfg_t tmpreg;

	PRT_TRACE_BEGIN("seq=%d\n", seq);
	tmpreg.val = READREG32(&(g_vic->if_cfg));
	tmpreg.bits.yc_seq = seq;
	WRITEREG32(&(g_vic->if_cfg), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Select embeded SYNC code position for BT.1120 mode0, 16-bit data bus
 * @pos: YC_SYNC
 *	0: Select SYNC code on D[7:0]
 *	1: Select SYNC code on D[15:8]
 */
u32 csp_vic_select_sync_code_position(u32 pos)
{
	reg_vic_if_cfg_t tmpreg;

	PRT_TRACE_BEGIN("pos=%d\n", pos);
	tmpreg.val = READREG32(&(g_vic->if_cfg));
	tmpreg.bits.yc_sync = pos;
	WRITEREG32(&(g_vic->if_cfg), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Select SYNC protocol
 * @ptcl: SYNC_PTCL
 *	0: DC mode, external reference with fixed HREF
 *	1: JEPG mode, external reference with variable HREF
 *	2: BT.601 mode0/2/3, external HSYNC and VSYNC
 *	3: BT.656 and BT.1120 mode0, embedded SYNC with FF0000XY
 *	4: BT.1120 mode1, embedded SYNC with FFFF00000000XYXY
 *	5: BT.601 mode1, external HSYNC without VSYNC
 */
u32 csp_vic_select_sync_protocol(u32 ptcl)
{
	reg_vic_if_cfg_t tmpreg;

	PRT_TRACE_BEGIN("ptcl=%d\n", ptcl);
	tmpreg.val = READREG32(&(g_vic->if_cfg));
	tmpreg.bits.sync_ptcl = ptcl;
	WRITEREG32(&(g_vic->if_cfg), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Select Field signal detection
 * @det: FIELD_DET
 *	0: Embedded SYNC code, such as BT.656 and BT.1120 mode0/1
 *	1: Field signal, such as BT.601 mode1/2
 *	2: Phase between VSYNC and HSYNC, such as BT.601 mode0/3
 */
u32 csp_vic_select_field_detecton(u32 det)
{
	reg_vic_if_cfg_t tmpreg;

	PRT_TRACE_BEGIN("det=%d\n", det);
	tmpreg.val = READREG32(&(g_vic->if_cfg));
	tmpreg.bits.field_det = det;
	WRITEREG32(&(g_vic->if_cfg), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Select input video mode
 * @mode: V_MODE
 *	0: progressive, such as DC mode, 720P, 1080P
 *	1: Interlaced, such as NTSC, PAL, 1080I
 */
u32 csp_vic_select_video_mode(u32 mode)
{
	reg_vic_if_cfg_t tmpreg;

	PRT_TRACE_BEGIN("mode=%d\n", mode);
	tmpreg.val = READREG32(&(g_vic->if_cfg));
	tmpreg.bits.v_mode = mode;
	WRITEREG32(&(g_vic->if_cfg), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * DMA Multi-channel enable
 * Only for BT.656 and BT.1120
 */
u32 csp_vic_multi_channel_enable(void)
{
	reg_vic_dma_multi_ch_t tmpreg;

	PRT_TRACE_BEGIN("\n");
	tmpreg.val = READREG32(&(g_vic->dma_multi_ch));
	tmpreg.bits.en = 1;
	WRITEREG32(&(g_vic->dma_multi_ch), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * DMA Multi-channel disable
 * Only for BT.656 and BT.1120
 */
u32 csp_vic_multi_channel_disable(void)
{
	reg_vic_dma_multi_ch_t tmpreg;

	PRT_TRACE_BEGIN("\n");
	tmpreg.val = READREG32(&(g_vic->dma_multi_ch));
	tmpreg.bits.en = 0;
	WRITEREG32(&(g_vic->dma_multi_ch), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set DMA channel numbers
 * Only for BT.656 and BT.1120
 */
u32 csp_vic_set_multi_channel_number(u32 num)
{
	reg_vic_dma_multi_ch_t tmpreg;

	PRT_TRACE_BEGIN("num=%d\n", num);
	tmpreg.val = READREG32(&(g_vic->dma_multi_ch));
	tmpreg.bits.num = num;
	WRITEREG32(&(g_vic->dma_multi_ch), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set DMA channel accept relevant ID data, Only for BT.656 and BT.1120
 * Rules:
 *	a) According to DMA channel numbers,
 *	   the lower ID channel should be activated,
 *	   if select 2 DMA channels,
 *	   the activated DMA channel should be channel 0/1;
 *	   if select 3 DMA channels,
 *	   the activated DMA channel should be channel 0/1/2;
 *	   if select 4 DMA channels,
 *	   the activated DMA channel should be channel 0/1/2/3;
 *	b) Set DMA channel with relevant ID data before enable
 *	   Multi-channel function
 *	c) Every DMA channel should has unique data ID
 */
u32 csp_vic_set_multi_channel_id(u32 ch, u32 id)
{
	reg_vic_dma_multi_id_t tmpreg;

	PRT_TRACE_BEGIN("ch=%d,id=%d\n", ch, id);
	tmpreg.val = READREG32(&(g_vic->dma_multi_id));
	switch (ch) {
	case VIC_DMA_CH0:
		tmpreg.bits.ch0_id = id;
		break;
	case VIC_DMA_CH1:
		tmpreg.bits.ch1_id = id;
		break;
	case VIC_DMA_CH2:
		tmpreg.bits.ch2_id = id;
		break;
	case VIC_DMA_CH3:
		tmpreg.bits.ch3_id = id;
		break;
	default:
		break;
	}
	WRITEREG32(&(g_vic->dma_multi_id), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set HSYNC front blanking offset, Only for BT.601 mode
 * Front blanking offset = HSW + HBP
 */
u32 csp_vic_hsync_fb_offset(u32 offset)
{
	reg_vic_hfb_ofs_t tmpreg;
	PRT_TRACE_BEGIN("offset=%d\n", offset);
	tmpreg.val = 0;
	tmpreg.bits.hoffset = offset;
	WRITEREG32(&(g_vic->hfb_ofs), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set HSYNC active width, Only for BT.601 mode
 */
u32 csp_vic_hsync_active_width(u32 width)
{
	reg_vic_hact_width_t tmpreg;
	PRT_TRACE_BEGIN("width=%d\n", width);
	tmpreg.val = 0;
	tmpreg.bits.haw = width;
	WRITEREG32(&(g_vic->hact_width), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set Field1 vertical front blanking offset, Only for BT.601 mode
 */
u32 csp_vic_f1_vsync_fb_offset(u32 offset)
{
	reg_vic_f1_vfb_ofs_t tmpreg;
	PRT_TRACE_BEGIN("offset=%d\n", offset);
	tmpreg.val = 0;
	tmpreg.bits.f1_voffset = offset;
	WRITEREG32(&(g_vic->f1_vfb_ofs), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set Field1 vertical active line, Only for BT.601 mode
 */
u32 csp_vic_f1_vsync_active_line(u32 line)
{
	reg_vic_f1_vact_line_t tmpreg;
	PRT_TRACE_BEGIN("line=%d\n", line);
	tmpreg.val = 0;
	tmpreg.bits.f1_vact_line = line;
	WRITEREG32(&(g_vic->f1_vact_line), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set Field1 vertical front blanking offset, Only for BT.601 mode
 */
u32 csp_vic_f2_vsync_fb_offset(u32 offset)
{
	reg_vic_f2_vfb_ofs_t tmpreg;
	PRT_TRACE_BEGIN("offset=%d\n", offset);
	tmpreg.val = 0;
	tmpreg.bits.f2_voffset = offset;
	WRITEREG32(&(g_vic->f2_vfb_ofs), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set Field2 vertical active line, Only for BT.601 mode
 */
u32 csp_vic_f2_vsync_active_line(u32 line)
{
	reg_vic_f2_vact_line_t tmpreg;
	PRT_TRACE_BEGIN("line=%d\n", line);
	tmpreg.val = 0;
	tmpreg.bits.f2_vact_line = line;
	WRITEREG32(&(g_vic->f2_vact_line), tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/******************************************************************************
 * dma configure
 *****************************************************************************/

/**
 * Set data offset of crop function for DMA[ch]
 * @x: XS, horizontal offset
 *	a) the unit is byte; b) Align by 4-byte
 * @y: YS, vertical offset
 *	a) the unit is byte; b) Align by 4-byte
 */
u32 csp_vic_dma_offset(u32 ch, u32 x, u32 y)
{
	reg_vic_dma0_ofs_t tmpreg;
	u32 reg_addr;

	PRT_TRACE_BEGIN("ch=%d,x=%d,y=%d\n", ch, x, y);
	reg_addr = (u32)&g_vic->dma0_ofs + ch * VIC_DMA_CH_WIN_OFFSET;
	tmpreg.val = 0;
	tmpreg.bits.xs = x;
	tmpreg.bits.ys = y;
	WRITEREG32(reg_addr, tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set data size of crop function for DMA[ch]
 * @x: X, width
 *	a) the unit is byte; b) Align by 4-byte
 * @y: Y, height
 *	a) the unit is byte; b) Align by 4-byte
 */
u32 csp_vic_dma_size(u32 ch, u32 x, u32 y)
{
	reg_vic_dma0_size_t tmpreg;
	u32 reg_addr;

	PRT_TRACE_BEGIN("ch=%d,x=%d,y=%d\n", ch, x, y);
	reg_addr = (u32)&g_vic->dma0_size + ch * VIC_DMA_CH_WIN_OFFSET;
	tmpreg.val = 0;
	tmpreg.bits.x = x;
	tmpreg.bits.y = y;
	WRITEREG32(reg_addr, tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set DMA[ch] output format
 * @ch: dma chanel id
 * @fmt: OUT_FMT
 *	0: Pass-through
 *	1: YUV422SP
 *	2: YUV420SP
 *	3: YUV422P
 *	4: YUV420P
 */
u32 csp_vic_dma_output_format(u32 ch, u32 fmt)
{
	reg_vic_dma0_mode_t tmpreg;
	u32 reg_addr;

	PRT_TRACE_BEGIN("ch=%d,fmt=%d\n", ch, fmt);
	reg_addr = (u32)&g_vic->dma0_mode + ch * VIC_DMA_CH_CFG_OFFSET;
	tmpreg.val = READREG32(reg_addr);
	tmpreg.bits.out_fmt = fmt;
	WRITEREG32(reg_addr, tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set DMA[ch] burst length
 * @ch: dma chanel id
 * @length: BURST
 *	0: Burst 1
 *	1: Burst 4
 *	2: Burst 8
 *	3: Burst 16
 */
u32 csp_vic_dma_burst_length(u32 ch, u32 length)
{
	reg_vic_dma0_mode_t tmpreg;
	u32 reg_addr;

	PRT_TRACE_BEGIN("ch=%d,length=%d\n", ch, length);
	reg_addr = (u32)&g_vic->dma0_mode + ch * VIC_DMA_CH_CFG_OFFSET;
	tmpreg.val = READREG32(reg_addr);
	tmpreg.bits.burst = length;
	WRITEREG32(reg_addr, tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set DMA[ch] storage mode
 * @ch: dma chanel id
 * @mode: FIELD_STRG
 *	0: Field mode, every field has its own frame buffer
 *	1: Frame mode0, two fields combine to one frame buffer,
 *	   field 1 is top field, field 2 is bottom field
 *	2: Frame mode1, two fields combine to one frame buffer,
 *	   field 1 is bottom field, field 2 is top field
 */
u32 csp_vic_dma_storage_mode(u32 ch, u32 mode)
{
	reg_vic_dma0_mode_t tmpreg;
	u32 reg_addr;

	PRT_TRACE_BEGIN("ch=%d,mode=%d\n", ch, mode);
	reg_addr = (u32)&g_vic->dma0_mode + ch * VIC_DMA_CH_CFG_OFFSET;
	tmpreg.val = READREG32(reg_addr);
	tmpreg.bits.filed_strg = mode;
	WRITEREG32(reg_addr, tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set DMA[ch] FCI generate mode
 * @ch: dma chanel id
 * @mode: FCI
 *	0: FCI generated in every one VSYNC,
 *	   which is usually used for field mode
 *	1: FCI generated in every two VSYNCs,
 *	   which is usually used for frame mode0/1
 */
u32 csp_vic_dma_fci_gen_mode(u32 ch, u32 mode)
{
	reg_vic_dma0_mode_t tmpreg;
	u32 reg_addr;

	PRT_TRACE_BEGIN("ch=%d,mode=%d\n", ch, mode);
	reg_addr = (u32)&g_vic->dma0_mode + ch * VIC_DMA_CH_CFG_OFFSET;
	tmpreg.val = READREG32(reg_addr);
	tmpreg.bits.fci = mode;
	WRITEREG32(reg_addr, tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set DMA[ch] VBI generate mode
 * @ch: dma chanel id
 * @mode: VBI
 *	0: VBI generated in every one VSYNC,
 *	   which is usually used for field mode
 *	1: VBI generated in every two VSYNCs,
 *	   which is usually used for frame mode0/1
 */
u32 csp_vic_dma_vbi_gen_mode(u32 ch, u32 mode)
{
	reg_vic_dma0_mode_t tmpreg;
	u32 reg_addr;

	PRT_TRACE_BEGIN("ch=%d,mode=%d\n", ch, mode);
	reg_addr = (u32)&g_vic->dma0_mode + ch * VIC_DMA_CH_CFG_OFFSET;
	tmpreg.val = READREG32(reg_addr);
	tmpreg.bits.vbi = mode;
	WRITEREG32(reg_addr, tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set DMA[ch] LCTI generate mode
 * @ch: dma chanel id
 * @mode: LCTI
 *	0: LCTI generated in every one VSYNC,
 *	   which is usually used for field mode
 *	1: LCTI generated in every two VSYNCs,
 *	   which is usually used for frame mode0/1
 */
u32 csp_vic_dma_lcti_gen_mode(u32 ch, u32 mode)
{
	reg_vic_dma0_mode_t tmpreg;
	u32 reg_addr;

	PRT_TRACE_BEGIN("ch=%d,mode=%d\n", ch, mode);
	reg_addr = (u32)&g_vic->dma0_mode + ch * VIC_DMA_CH_CFG_OFFSET;
	tmpreg.val = READREG32(reg_addr);
	tmpreg.bits.lcti = mode;
	WRITEREG32(reg_addr, tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set DMA[ch] update timing of component address registers,
 * which are double-buffered
 * @ch: dma chanel id
 * @timing, UD
 *	0: VBI
 *	1: FCI, the most common way
 *	2: Mannually
 */
u32 csp_vic_dma_update_cmpt_address(u32 ch, u32 timing)
{
	reg_vic_dma0_mode_t tmpreg;
	u32 reg_addr;

	PRT_TRACE_BEGIN("ch=%d,timing=%d\n", ch, timing);
	reg_addr = (u32)&g_vic->dma0_mode + ch * VIC_DMA_CH_CFG_OFFSET;
	tmpreg.val = READREG32(reg_addr);
	tmpreg.bits.ud = timing;
	WRITEREG32(reg_addr, tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set DMA[ch] capture mode
 * @ch: dma chanel id
 * @mode: CAP, capture mode
 *	0: Single mode
 *	1: Continuous mode
 */
u32 csp_vic_dma_capture_mode(u32 ch, u32 mode)
{
	reg_vic_dma0_mode_t tmpreg;
	u32 reg_addr;

	PRT_TRACE_BEGIN("ch=%d,mode=%d\n", ch, mode);
	reg_addr = (u32)&g_vic->dma0_mode + ch * VIC_DMA_CH_CFG_OFFSET;
	tmpreg.val = READREG32(reg_addr);
	tmpreg.bits.cap = mode;
	WRITEREG32(reg_addr, tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set DMA[ch] start
 * @ch: dma chanel id
 */
u32 csp_vic_dma_start(u32 ch)
{
	reg_vic_dma0_ctrl_t tmpreg;
	u32 reg_addr;

	PRT_TRACE_BEGIN("ch=%d\n", ch);
	reg_addr = (u32)&g_vic->dma0_ctrl + ch * VIC_DMA_CH_CFG_OFFSET;
	tmpreg.val = 0;
	tmpreg.bits.start = 1;
	WRITEREG32(reg_addr, tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set DMA[ch] stop
 * @ch: dma chanel id
 */
u32 csp_vic_dma_stop(u32 ch)
{
	reg_vic_dma0_ctrl_t tmpreg;
	u32 reg_addr;

	PRT_TRACE_BEGIN("ch=%d\n", ch);
	reg_addr = (u32)&g_vic->dma0_ctrl + ch * VIC_DMA_CH_CFG_OFFSET;
	tmpreg.val = 0;
	tmpreg.bits.start = 0;
	WRITEREG32(reg_addr, tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set DMA[ch] update component address registers manually
 * @ch: dma chanel id
 */
u32 csp_vic_dma_update_manual(u32 ch)
{
	reg_vic_dma0_update_t tmpreg;
	u32 reg_addr;

	PRT_TRACE_BEGIN("ch=%d\n", ch);
	reg_addr = (u32)&g_vic->dma0_update + ch * VIC_DMA_CH_CFG_OFFSET;
	tmpreg.val = 0;
	tmpreg.bits.ud_man = 1;
	WRITEREG32(reg_addr, tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set DMA[ch] component0 output address
 * @ch: dma chanel id
 * @addr: output address
 */
u32 csp_vic_dma_cmpt0_addr(u32 ch, u32 addr)
{
	reg_vic_dma0_addr0_t tmpreg;
	u32 reg_addr;

	PRT_TRACE_BEGIN("ch=%d,addr=0x%08x\n", ch, addr);
	reg_addr = (u32)&g_vic->dma0_addr0 + ch * VIC_DMA_CH_CFG_OFFSET;
	tmpreg.val = 0;
	tmpreg.bits.addr = addr;
	WRITEREG32(reg_addr, tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set DMA[ch] component1 output address
 * @ch: dma chanel id
 * @addr: output address
 */
u32 csp_vic_dma_cmpt1_addr(u32 ch, u32 addr)
{
	reg_vic_dma0_addr1_t tmpreg;
	u32 reg_addr;

	PRT_TRACE_BEGIN("ch=%d,addr=0x%08x\n", ch, addr);
	reg_addr = (u32)&g_vic->dma0_addr1 + ch * VIC_DMA_CH_CFG_OFFSET;
	tmpreg.val = 0;
	tmpreg.bits.addr = addr;
	WRITEREG32(reg_addr, tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set DMA[ch] component2 output address
 * @ch: dma chanel id
 * @addr: output address
 */
u32 csp_vic_dma_cmpt2_addr(u32 ch, u32 addr)
{
	reg_vic_dma0_addr2_t tmpreg;
	u32 reg_addr;

	PRT_TRACE_BEGIN("ch=%d,addr=0x%08x\n", ch, addr);
	reg_addr = (u32)&g_vic->dma0_addr2 + ch * VIC_DMA_CH_CFG_OFFSET;
	tmpreg.val = 0;
	tmpreg.bits.addr = addr;
	WRITEREG32(reg_addr, tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Set data line stride of DMA[ch]
 * @ch: dma chanel id
 * @luma: luma line stride
 * @chroma: chroma line stride
 */
u32 csp_vic_dma_linestride(u32 ch, u32 luma, u32 chroma)
{
	reg_vic_dma0_ls_t tmpreg;
	u32 reg_addr;

	PRT_TRACE_BEGIN("ch=%d,luma=%d,chroma=%d\n", ch, luma, chroma);
	reg_addr = (u32)&g_vic->dma0_ls + ch * VIC_DMA_CH_CFG_OFFSET;
	tmpreg.val = 0;
	tmpreg.bits.y = luma;
	tmpreg.bits.c = chroma;
	WRITEREG32(reg_addr, tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Read DMA[ch] current byte counter, which would be reset by every VSYNC
 * @ch: dma chanel id
 */
u32 csp_vic_dma_cur_byte_cnt(u32 ch)
{
	reg_vic_dma0_bc_t tmpreg;
	u32 reg_addr;

	PRT_TRACE_BEGIN("ch=%d\n", ch);
	reg_addr = (u32)&g_vic->dma0_bc + ch * VIC_DMA_CH_CFG_OFFSET;
	tmpreg.val = READREG32(reg_addr);
	PRT_TRACE_END("tmpreg.bits.bc=%d\n", tmpreg.bits.bc);

	return tmpreg.bits.bc;
}

/**
 * Set DMA[ch] LCTI trigger value
 * @ch: dma chanel id
 * @line: line count to trigger LCTI
 */
u32 csp_vic_dma_lcti_trig_val(u32 ch, u32 line)
{
	reg_vic_dma0_line_cnt_t tmpreg;
	u32 reg_addr;

	PRT_TRACE_BEGIN("ch=%d,line=%d\n", ch, line);
	reg_addr = (u32)&g_vic->dma0_line_cnt + ch * VIC_DMA_CH_CFG_OFFSET;
	tmpreg.val = READREG32(reg_addr);
	tmpreg.bits.ls_trig = line;
	WRITEREG32(reg_addr, tmpreg.val);
	PRT_TRACE_END("\n");

	return 0;
}

/**
 * Read DMA[ch] current line counter
 * @ch: dma chanel id
 */
u32 csp_vic_dma_cur_line_cnt(u32 ch)
{
	reg_vic_dma0_line_cnt_t tmpreg;
	u32 reg_addr;

	PRT_TRACE_BEGIN("ch=%d\n", ch);
	reg_addr = (u32)&g_vic->dma0_line_cnt + ch * VIC_DMA_CH_CFG_OFFSET;
	tmpreg.val = READREG32(reg_addr);
	PRT_TRACE_END("\n");

	return tmpreg.bits.ls_crnt;
}

void csp_vic_cfg_fifo(void)
{
	return;
}

/**
 * Reset VIC circuit
 */
u32 csp_vic_reset(void)
{
#if 1
	u32 reg_addr;
	u32 tmpval;

	PRT_TRACE_BEGIN("\n");
	reg_addr = (u32)g_vic + 0x600;
	tmpval = READREG32(reg_addr);
	tmpval |= (1 << 3);
	WRITEREG32(reg_addr, tmpval);
	PRT_TRACE_END("\n");
#endif
	return 0;
}

/******************************************************************************
 * status & inturrupts
 *****************************************************************************/

/**
 * csp_vic_clear_set_ints - clear some int enable bit,
 *			    and than set other int enable bit
 * @ch:		dma chanel index
 * @clear_mask:	int enable bits to be clear
 * @set_mask:	int enable bits to be set
 */
void csp_vic_clear_set_ints(u32 ch, u32 clear_mask, u32 set_mask)
{
	u32 int_en;
	u32 reg_addr = 0;

	PRT_TRACE_BEGIN("ch=%d,clear_mask=0x%08x,set_mask=0x%08x\n",
			ch, clear_mask, set_mask);
	reg_addr = (u32)&g_vic->dma0_int_en + ch * VIC_DMA_CH_CFG_OFFSET;

	int_en = READREG32(reg_addr);
	int_en &= ~clear_mask;
	int_en |= set_mask;
	WRITEREG32(reg_addr, int_en);
	PRT_TRACE_END("\n");
}

/**
 * csp_sdc_enable_ints - enable the specified interrputs
 * @ch:		dma chanel index
 * @int_mask:	int enable bits to be set
 */
void csp_vic_enable_ints(u32 ch, u32 int_mask)
{
	u32 int_en;
	u32 reg_addr = 0;

	PRT_TRACE_BEGIN("ch=%d,int_mask=0x%08x\n", ch, int_mask);
	reg_addr = (u32)&g_vic->dma0_int_en + ch * VIC_DMA_CH_CFG_OFFSET;

	int_en = READREG32(reg_addr);
	int_en |= int_mask;
	WRITEREG32(reg_addr, int_en);
	PRT_TRACE_END("\n");
}

/**
 * csp_vic_disable_ints - enable the specified interrputs
 * @ch:		dma chanel index
 * @int_mask:	int enable bits to be set
 */
void csp_vic_disable_ints(u32 ch, u32 int_mask)
{
	u32 int_en;
	u32 reg_addr = 0;

	PRT_TRACE_BEGIN("ch=%d,int_mask=0x%08x\n", ch, int_mask);
	reg_addr = (u32)&g_vic->dma0_int_en + ch * VIC_DMA_CH_CFG_OFFSET;

	int_en = READREG32(reg_addr);
	int_en &= ~int_mask;
	WRITEREG32(reg_addr, int_en);
	PRT_TRACE_END("\n");
}

/**
 * csp_vic_clr_pd - clear int pending status
 * @ch:		dma chanel index
 * @pd_mask:	int pending bits status to be clear
 */
void csp_vic_clr_pd(u32 ch, u32 pd_mask)
{
	u32 reg_addr = 0;

	PRT_TRACE_BEGIN("ch=%d,pd_mask=0x%08x\n", ch, pd_mask);
	reg_addr = (u32)&g_vic->dma0_int_clr + ch * VIC_DMA_CH_CFG_OFFSET;

	WRITEREG32(reg_addr, pd_mask);
	PRT_TRACE_END("\n");
}

/**
 * csp_vic_get_int_pd - get int pending status
 * @ch:		dma chanel index
 */
u32 csp_vic_get_int_pd(u32 ch)
{
	u32 reg_addr = 0;

	PRT_TRACE_BEGIN("ch=%d\n", ch);
	reg_addr = (u32)&g_vic->dma0_int_pend + ch * VIC_DMA_CH_CFG_OFFSET;
	PRT_TRACE_END("pd=0x%08x\n", READREG32(reg_addr));

	return READREG32(reg_addr);
}

/**
 * csp_vic_chk_err - check whether there are error pendings occur
 * @ch:		dma chanel index
 *
 * return 0 when not error, otherwise 1
 */
u32 csp_vic_chk_err(u32 ch)
{
	u32 reg_addr = 0;
	u32 int_pd_tmp = 0;
	u32 ret = 0;

	PRT_TRACE_BEGIN("ch=%d\n", ch);
	reg_addr = (u32)&g_vic->dma0_int_pend + ch * VIC_DMA_CH_CFG_OFFSET;

	int_pd_tmp = READREG32(reg_addr);
	/* report error when any error pending bit occur */
	if ((int_pd_tmp & VIC_DMA_INT_ALL_ERR) != 0) {
		LOG_E("int_pd=%x\n", int_pd_tmp);
		ret = 1;
	}
	PRT_TRACE_END("ret=%d\n", ret);

	return ret;
}

/**
 * csp_vic_chk_pd - check whether all the expected int pending have been raise
 * @ch:		dma chanel index
 * @pd_mask:	int pending bits to be check
 *
 * return 1 when all the expected pending bits are set, otherwise 0
 */
u32 csp_vic_chk_pd(u32 ch, u32 pd_mask)
{
	u32 reg_addr = 0;
	u32 int_pd_tmp;
	u32 ret = 0;

	PRT_TRACE_BEGIN("ch=%d,pd_mask=0x%08x\n", ch, pd_mask);
	reg_addr = (u32)&g_vic->dma0_int_pend + ch * VIC_DMA_CH_CFG_OFFSET;

	int_pd_tmp = READREG32(reg_addr);

	/* return 1 only when all bits in pd_mask are set */
	if ((int_pd_tmp & pd_mask) == pd_mask)
		ret = 1;
	PRT_TRACE_END("ret=%d\n", ret);

	return ret;
}

/**
 * csp_vic_chk_pd - check whether all the expected int pending have been raise
 * @func_name:	the name of the function that want dump registers
 */
void csp_vic_dump_regs(const char *func_name)
{
#if 0
	u32 offset = 0;

	PRT_DBG(" =========== (%s) ===========\n",
		func_name);
#if 0
	for (offset = 0; offset < 0x3CC; offset += 8) {
		PRT_DBG(" 0x%x: 0x%08x | 0x%x: 0x%08x\n",
			offset, READREG32((u32)g_vic + offset),
			offset + 4, READREG32((u32)g_vic + offset + 4));
	}
#endif

	for (offset = 0x600; offset < 0x61C; offset += 8) {
		PRT_DBG(" 0x%x: 0x%08x | 0x%x: 0x%08x\n",
			offset, READREG32((u32)g_vic + offset),
			offset + 4, READREG32((u32)g_vic + offset + 4));
	}
	PRT_DBG(" ===========================================\n");
#endif
}
