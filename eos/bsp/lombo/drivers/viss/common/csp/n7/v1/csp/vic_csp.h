/*
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

#ifndef ___VISS_VIC___H___
#define ___VISS_VIC___H___

/******************************************************************************
 * base operations
 *****************************************************************************/

void csp_vic_set_register_base(void *base);

/**
 * VIC enable
 */
u32 csp_vic_enable(void);

/**
 * VIC disable
 */
u32 csp_vic_disable(void);

/**
 * VIC status
 */
u32 csp_vic_status(void);

/******************************************************************************
 * interface configure
 *****************************************************************************/

/**
 * Select VIC data path
 */
u32 csp_vic_data_path(u32 path);

/**
 * Set PCLK polarity
 */
u32 csp_vic_set_pclk_polarity(u32 pol);

/**
 * Set Filed polarity
 */
u32 csp_vic_set_field_polarity(u32 pol);

/**
 * Set HSYNC polarity
 */
u32 csp_vic_set_hsync_polarity(u32 pol);

/**
 * Set VSYNC polarity
 */
u32 csp_vic_set_vsync_polarity(u32 pol);

/**
 * Set PCLK phase
 */
u32 csp_vic_set_pclk_phase(u32 phase);

/**
 * Select DVP data bus protocol
 */
u32 csp_vic_select_data_protocol(u32 ptcl);

/**
 * Select YUV componet sequence
 */
u32 csp_vic_set_yuv_seq(u32 seq);

/**
 * Select embeded SYNC code position for BT.1120 mode0, 16-bit data bus
 */
u32 csp_vic_select_sync_code_position(u32 pos);

/**
 * Select SYNC protocol
 */
u32 csp_vic_select_sync_protocol(u32 ptcl);

/**
 * Select Field signal detection
 */
u32 csp_vic_select_field_detecton(u32 det);

/**
 * Select input video mode
 */
u32 csp_vic_select_video_mode(u32 mode);

/**
 * DMA Multi-channel enable
 * Only for BT.656 and BT.1120
 */
u32 csp_vic_multi_channel_enable(void);

/**
 * DMA Multi-channel disable
 * Only for BT.656 and BT.1120
 */
u32 csp_vic_multi_channel_disable(void);

/**
 * Set DMA channel numbers
 * Only for BT.656 and BT.1120
 */
u32 csp_vic_set_multi_channel_number(u32 num);

/**
 * Set DMA channel accept relevant ID data, Only for BT.656 and BT.1120
 */
u32 csp_vic_set_multi_channel_id(u32 ch, u32 id);

/**
 * Set HSYNC front blanking offset, Only for BT.601 mode
 */
u32 csp_vic_hsync_fb_offset(u32 offset);

/**
 * Set HSYNC active width, Only for BT.601 mode
 */
u32 csp_vic_hsync_active_width(u32 width);

/**
 * Set Field1 vertical front blanking offset, Only for BT.601 mode
 */
u32 csp_vic_f1_vsync_fb_offset(u32 offset);

/**
 * Set Field1 vertical active line, Only for BT.601 mode
 */
u32 csp_vic_f1_vsync_active_line(u32 line);

/**
 * Set Field1 vertical front blanking offset, Only for BT.601 mode
 */
u32 csp_vic_f2_vsync_fb_offset(u32 offset);

/**
 * Set Field2 vertical active line, Only for BT.601 mode
 */
u32 csp_vic_f2_vsync_active_line(u32 line);

/******************************************************************************
 * dma configure
 *****************************************************************************/

/**
 * Set data offset of crop function for DMA[ch]
 */
u32 csp_vic_dma_offset(u32 ch, u32 x, u32 y);

/**
 * Set data size of crop function for DMA[ch]
 */
u32 csp_vic_dma_size(u32 ch, u32 x, u32 y);

/**
 * Set DMA[ch] output format
 */
u32 csp_vic_dma_output_format(u32 ch, u32 fmt);

/**
 * Set DMA[ch] burst length
 */
u32 csp_vic_dma_burst_length(u32 ch, u32 length);

/**
 * Set DMA[ch] storage mode
 */
u32 csp_vic_dma_storage_mode(u32 ch, u32 mode);

/**
 * Set DMA[ch] FCI generate mode
 */
u32 csp_vic_dma_fci_gen_mode(u32 ch, u32 mode);

/**
 * Set DMA[ch] VBI generate mode
 */
u32 csp_vic_dma_vbi_gen_mode(u32 ch, u32 mode);

/**
 * Set DMA[ch] LCTI generate mode
 */
u32 csp_vic_dma_lcti_gen_mode(u32 ch, u32 mode);

/**
 * Set DMA[ch] update timing of component address registers,
 * which are double-buffered
 */
u32 csp_vic_dma_update_cmpt_address(u32 ch, u32 timing);

/**
 * Set DMA[ch] capture mode
 */
u32 csp_vic_dma_capture_mode(u32 ch, u32 mode);

/**
 * Set DMA[ch] start
 */
u32 csp_vic_dma_start(u32 ch);

/**
 * Set DMA[ch] stop
 */
u32 csp_vic_dma_stop(u32 ch);

/**
 * Set DMA[ch] update component address registers manually
 */
u32 csp_vic_dma_update_manual(u32 ch);

/**
 * Set DMA[ch] component0 output address
 */
u32 csp_vic_dma_cmpt0_addr(u32 ch, u32 addr);

/**
 * Set DMA[ch] component1 output address
 */
u32 csp_vic_dma_cmpt1_addr(u32 ch, u32 addr);

/**
 * Set DMA[ch] component2 output address
 */
u32 csp_vic_dma_cmpt2_addr(u32 ch, u32 addr);

/**
 * Set data line stride of DMA[ch]
 */
u32 csp_vic_dma_linestride(u32 ch, u32 luma, u32 chroma);

/**
 * Read DMA[ch] current byte counter, which would be reset by every VSYNC
 */
u32 csp_vic_dma_cur_byte_cnt(u32 ch);

/**
 * Set DMA[ch] LCTI trigger value
 */
u32 csp_vic_dma_lcti_trig_val(u32 ch, u32 line);

/**
 * Read DMA[ch] current line counter
 * @ch: dma chanel id
 */
u32 csp_vic_dma_cur_line_cnt(u32 ch);

void csp_vic_cfg_fifo(void);

/**
 * Reset VIC circuit
 */
u32 csp_vic_reset(void);

/******************************************************************************
 * status & inturrupts
 *****************************************************************************/

/**
 * csp_vic_clear_set_ints - clear some int enable bit,
 *			    and than set other int enable bit
 */
void csp_vic_clear_set_ints(u32 ch, u32 clear_mask, u32 set_mask);

/**
 * csp_sdc_enable_ints - enable the specified interrputs
 */
void csp_vic_enable_ints(u32 ch, u32 int_mask);

/**
 * csp_vic_disable_ints - enable the specified interrputs
 */
void csp_vic_disable_ints(u32 ch, u32 int_mask);

/**
 * csp_vic_clr_pd - clear int pending status
 */
void csp_vic_clr_pd(u32 ch, u32 pd_mask);

/**
 * csp_vic_get_int_pd - get int pending status
 */
u32 csp_vic_get_int_pd(u32 ch);

/**
 * csp_vic_chk_err - check whether there are error pendings occur
 */
u32 csp_vic_chk_err(u32 ch);

/**
 * csp_vic_chk_pd - check whether all the expected int pending have been raise
 */
u32 csp_vic_chk_pd(u32 ch, u32 pd_mask);

/**
 * csp_vic_chk_pd - check whether all the expected int pending have been raise
 */
void csp_vic_dump_regs(const char *func_name);

/**
 * enable vic output high 8bit data
 */
u32 csp_enable_vic_output_high_8bit_data(void);

/**
 * enable vic output raw msb data
 */
u32 csp_enable_vic_output_raw_msb_data(void);

#endif /* __VIC_CSP_H__ */
