/*
 * Copyright (C) 2016-2018, LomboTech Co.Ltd.
 * Author: lomboswer <lomboswer@lombotech.com>
 *
 * Lombo n7 VISS-MIPI-CSI controller register definitions header
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

#ifndef ___VISS_MIPI__CSI___H___
#define ___VISS_MIPI__CSI___H___

void csp_mcsi_set_register_base(void *base);

/**
 * MIPI_CSI enable
 */
u32 csp_mcsi_enable(void);

/**
 * MIPI_CSI disable
 */
u32 csp_mcsi_disable(void);

/**
 * MIPI_CSI status
 */
u32 csp_mcsi_status(void);

/******************************************************************************
 * interface configure
 *****************************************************************************/

/**
 * Select MIPI_CSI data path
 */
u32 csp_mcsi_data_path(u32 path);

/**
 * Select YUV componet sequence
 */
u32 csp_mcsi_component_sequence(u32 seq);

/**
 * Select long packet parse
 */
u32 csp_mcsi_parse_long_pkg(void);

/**
 * Not select long packet parse
 */
u32 csp_mcsi_no_parse_long_pkg(void);

/**
 * Pixel channel head filter config
 */
u32 csp_mcsi_pixel_channel_head_filter(u32 dt, u32 vc);

/**
 * DMA channel head filter config
 */
u32 csp_mcsi_dma_channel_head_filter(u32 dt, u32 vc);

/**
 * DMA channel head mapping config
 */
u32 csp_mcsi_dma_channel_head_mapping(u32 dt, u32 vc);

/**
 * Enable MIPI_CSI interrupt
 */
u32 csp_mcsi_int_enable(u32 interrput);

/**
 * Disable MIPI_CSI interrupt
 */
u32 csp_mcsi_int_disable(u32 interrput);

/**
 * Read MIPI_CSI interrupts' pending
 */
u32 csp_mcsi_int_pending(u32 interrput);

/**
 * Clear MIPI_CSI interrupt
 */
u32 csp_mcsi_int_clear(u32 interrput);

/**
 * Set data offset of crop function for DMA
 */
u32 csp_mcsi_dma_offset(u32 x, u32 y);

/**
 * Set data size of crop function for DMA
 */
u32 csp_mcsi_dma_size(u32 x, u32 y);

/**
 * Set DMA output format
 */
u32 csp_mcsi_dma_output_format(u32 fmt);

/**
 * Set DMA burst length
 */
u32 csp_mcsi_dma_burst_length(u32 length);

/**
 * Set DMA update timing of component address registers,
 * which are double-buffered
 */
u32 csp_mcsi_dma_update_cmpt_address(u32 timing);

/**
 * Set DMA capture mode
 */
u32 csp_mcsi_dma_capture_mode(u32 mode);

/**
 * Set DMA start
 */
u32 csp_mcsi_dma_start(void);

/**
 * Set DMA stop
 */
u32 csp_mcsi_dma_stop(void);

/**
 * Set DMA update component address registers manually
 */
u32 csp_mcsi_dma_update_manual(void);

/**
 * Set DMA component0 output address
 */
u32 csp_mcsi_dma_cmpt0_addr(u32 addr);

/**
 * Set DMA component1 output address
 */
u32 csp_mcsi_dma_cmpt1_addr(u32 addr);

/**
 * Set DMA component2 output address
 */
u32 csp_mcsi_dma_cmpt2_addr(u32 addr);

/**
 * Set data line stride of DMA
 */
u32 csp_mcsi_dma_linestride(u32 luma, u32 chroma);

/**
 * Read DMA current byte counter, which would be reset by every VSYNC
 */
u32 csp_mcsi_dma_cur_byte_cnt(void);

/**
 * Enable DMA interrupts
 */
u32 csp_mcsi_dma_int_enable(u32 interrupt);

/**
 * Set DMA LCTI trigger value
 */
u32 csp_mcsi_dma_lcti_trig_val(u32 line);

/**
 * Read DMA current line counter
 */
u32 csp_mcsi_dma_cur_line_cnt(void);

void csp_mcsi_cfg_fifo(void);

u32 csp_mcsi_chk_lbor(void);

/**
 * Reset DMA circuit
 */
u32 csp_mcsi_dma_reset(void);

/**
 * Reset MIPI_CSI circuit
 */
u32 csp_mcsi_reset(void);

/**
 * Config MIPI DPHY active lanes
 */
u32 csp_mipi_dphy_active_lane(u32 lane);

/**
 * MIPI DPHY shutdown
 */
u32 csp_mipi_dphy_shutdown(void);

/**
 * MIPI DPHY power up
 */
u32 csp_mipi_dphy_power_up(void);

/**
 * MIPI DPHY reset active
 */
u32 csp_mipi_dphy_reset_active(void);

/**
 * MIPI DPHY reset in-active
 */
u32 csp_mipi_dphy_reset_inactive(void);

/**
 * MIPI DPHY controller reset active
 */
u32 csp_mipi_dphy_controller_reset_active(void);

/**
 * MIPI DPHY controller reset in-active
 */
u32 csp_mipi_dphy_controller_reset_inactive(void);

/**
 * MIPI DPHY status
 */
u32 csp_mipi_dphy_state(void);

/**
 * Config MIPI DPHY data ID
 */
u32 csp_mipi_dphy_data_id(u32 id, u32 dt, u32 vc);

/**
 * Read MIPI DPHY register
 */
u32 csp_mipi_dphy_read(u8 addr);

/**
 * Write MIPI DPHY register
 */
u32 csp_mipi_dphy_write(u8 addr, u8 wval);

/**
 * MIPI DPHY Init
 */
u32 csp_mipi_dphy_init_0(u32 lane, u32 id, u32 dt, u32 vc, u32 freq);

/**
 * MIPI DPHY Init
 */
u32 csp_mipi_dphy_init_1(u32 lane, u32 id, u32 dt, u32 vc, u32 freq);


/******************************************************************************
 * status & inturrupts
 *****************************************************************************/

/**
 * csp_mcsi_clear_set_ints - clear some int enable bit,
 *			    and than set other int enable bit
 */
void csp_mcsi_clear_set_ints(u32 clear_mask, u32 set_mask);

/**
 * csp_sdc_enable_ints - enable the specified interrputs
 */
void csp_mcsi_enable_ints(u32 int_mask);

/**
 * csp_mcsi_disable_ints - enable the specified interrputs
 */
void csp_mcsi_disable_ints(u32 int_mask);

/**
 * csp_mcsi_clr_pd - clear int pending status
 */
void csp_mcsi_clr_pd(u32 pd_mask);

/**
 * csp_mcsi_get_int_pd - get int pending status
 */
u32 csp_mcsi_get_int_pd(void);

/**
 * csp_mcsi_chk_err - check whether there are error pendings occur
 */
u32 csp_mcsi_chk_err(void);

/**
 * csp_mcsi_chk_pd - check whether all the expected int pending have been raise
 */
u32 csp_mcsi_chk_pd(u32 pd_mask);

/**
 * csp_mcsi_dump_regs - dump all the register
 */
void csp_mcsi_dump_regs(const char *func_name);

/**
 * enable mcsi output raw msb data
 */
u32 csp_enable_mcsi_output_raw_msb_data(void);

/**
 * enable mcsi output high 8bit data
 */
u32 csp_enable_mcsi_output_high_8bit_data(void);

#endif /* __MIPI_CSI_CSP_H__ */
