/*
 *
 * Copyright (C) 2016-2018, LomboTech Co.Ltd.
 * Authors:
 *	lomboswer <lomboswer@lombotech.com>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#ifndef _CSP_DOSS_TCON_V1_0_H_
#define _CSP_DOSS_TCON_V1_0_H_

#include "../disp_common.h"

#include "csp_doss_tcon_reg_v1_0.h"

#define TCON_MAX_INST		(2)
#define TRUE			1
#define FALSE			0

/* tcon custom define */
#define OFS_TCON_VER			(0x00000000)
#define OFS_TCON_FTR			(0x00000004)
#define OFS_TCON_TCON_CTRL		(0x00000010)
#define OFS_TCON_TCON_CFG		(0x00000014)
#define OFS_TCON_VSYNC			(0x00000018)
#define OFS_TCON_IO_CFG			(0x0000001C)
#define OFS_TCON_FIFO_CTRL		(0x00000020)
#define OFS_TCON_FIFO_STA		(0x00000024)
#define OFS_TCON_FIFO_DATA		(0x00000028)
#define OFS_TCON_BG_COLOR		(0x0000002C)
#define OFS_TCON_INT_EN			(0x00000030)
#define OFS_TCON_INT_PD			(0x00000034)
#define OFS_TCON_INT_CLR		(0x00000038)
#define OFS_TCON_LINE_CNT		(0x0000003C)
#define OFS_TCON_GEN_HTIM0		(0x00000040)
#define OFS_TCON_GEN_HTIM1		(0x00000044)
#define OFS_TCON_GEN_VTIM0		(0x00000048)
#define OFS_TCON_GEN_VTIM1		(0x0000004C)
#define OFS_TCON_GEN_IN_RES		(0x00000058)
#define OFS_TCON_RGB_IF_CFG		(0x00000080)
#define OFS_TCON_CPU_IF_CFG		(0x00000084)
#define OFS_TCON_CPU_VSYNC_CMD		(0x00000088)
#define OFS_TCON_CPU_HSYNC_CMD		(0x0000008C)
#define OFS_TCON_CPU_IF_OP		(0x00000090)
#define OFS_TCON_CPU_WR_DAT		(0x00000098)
#define OFS_TCON_CPU_RD_DAT		(0x0000009C)
#define OFS_TCON_LVDS_IF_CFG		(0x000000D0)
#define OFS_TCON_LVDS_ANL_CFG0		(0x000000D4)
#define OFS_TCON_CSC_C0			(0x00000100)
#define OFS_TCON_CSC_C1			(0x00000104)
#define OFS_TCON_CSC_C2			(0x00000108)
#define OFS_TCON_CSC_C3			(0x0000010C)
#define OFS_TCON_CSC_C4			(0x00000110)
#define OFS_TCON_CSC_C5			(0x00000114)
#define OFS_TCON_CSC_C6			(0x00000118)
#define OFS_TCON_CSC_C7			(0x0000011C)
#define OFS_TCON_CSC_C8			(0x00000120)
#define OFS_TCON_CSC_C9			(0x00000124)
#define OFS_TCON_CSC_C10		(0x00000128)
#define OFS_TCON_CSC_C11		(0x0000012C)
#define OFS_TCON_CSC_CLIP0		(0x00000130)
#define OFS_TCON_CSC_CLIP1		(0x00000134)
#define OFS_TCON_CSC_CLIP2		(0x00000138)
#define OFS_TCON_FRC_CFG		(0x00000140)
#define OFS_TCON_FRC_SEED0		(0x00000144)
#define OFS_TCON_FRC_SEED1		(0x00000148)
#define OFS_TCON_FRC_SEED2		(0x0000014C)
#define OFS_TCON_FRC_SEED3		(0x00000150)
#define OFS_TCON_FRC_SEED4		(0x00000154)
#define OFS_TCON_FRC_SEED5		(0x00000158)
#define OFS_TCON_FRC_LUT0		(0x00000160)
#define OFS_TCON_FRC_LUT1		(0x00000164)
#define OFS_TCON_FRC_LUT2		(0x00000168)
#define OFS_TCON_FRC_LUT3		(0x0000016C)
#define OFS_TCON_GAMMA_TBL		(0x00001000)

/* Enable/Disable */
#define TCON_DISABLE			(0)
#define TCON_ENABLE			(1)
/* NTSC or PAL */
#define TCON_NTSC			(0)
#define TCON_PAL			(1)
/* IO Invert */
#define TCON_IO_NON_INVERT		(0)
#define TCON_IO_INVERT			(1)
/* Start and finish operation */
#define	TCON_REG_START			(1)
#define TCON_REG_FINISH			(0)

/* FIFO Mode */
#define TCON_REG_FIFO_NORMAL		TCON_TCON_CFG_FIFO_MODE_0
#define TCON_REG_FIFO_TWO_HALVES	TCON_TCON_CFG_FIFO_MODE_1
#define TCON_REG_FIFO_EC		TCON_TCON_CFG_FIFO_MODE_2

/* VBI Trigger Condiction */
#define TCON_REG_VBI_VFP		TCON_VSYNC_VBI_TRIG_0
#define TCON_REG_VBI_VSW		TCON_VSYNC_VBI_TRIG_1
#define TCON_REG_VBI_VBP		TCON_VSYNC_VBI_TRIG_2
#define TCON_REG_VBI_VACT		TCON_VSYNC_VBI_TRIG_3
/* Frmae Trigger Source */
#define TCON_REG_TRIG_INT		TCON_TCON_CFG_TRIG_SRC_0
#define TCON_REG_TRIG_TE		TCON_TCON_CFG_TRIG_SRC_1
#define TCON_REG_TRIG_EXT		TCON_TCON_CFG_TRIG_SRC_2
/* Frame Trigger Mode */
#define TCON_REG_TRIG_AUTO		TCON_TCON_CFG_TRIG_MODE_0
#define TCON_REG_TRIG_SINGLE		TCON_TCON_CFG_TRIG_MODE_1
/* Output Interface Selection */
#define TCON_REG_PRGB			TCON_TCON_CFG_OUT_SEL_0
#define TCON_REG_SRGB_RGB		TCON_TCON_CFG_OUT_SEL_4
#define TCON_REG_SRGB_DRGB		TCON_TCON_CFG_OUT_SEL_5
#define TCON_REG_SRGB_RGBD		TCON_TCON_CFG_OUT_SEL_6
#define TCON_REG_CPU_18BIT_RGB666	TCON_TCON_CFG_OUT_SEL_10
#define TCON_REG_CPU_16BIT_RGB888	TCON_TCON_CFG_OUT_SEL_14
#define TCON_REG_CPU_16BIT_RGB565	TCON_TCON_CFG_OUT_SEL_15
#define TCON_REG_CPU_9BIT_RGB666	TCON_TCON_CFG_OUT_SEL_18
#define TCON_REG_CPU_8BIT_RGB565	TCON_TCON_CFG_OUT_SEL_1C
#define TCON_REG_CPU_8BIT_RGB888	TCON_TCON_CFG_OUT_SEL_1D
#define TCON_REG_BT601_1120_16BIT	TCON_TCON_CFG_OUT_SEL_21
#define TCON_REG_BT1120_8BIT		TCON_TCON_CFG_OUT_SEL_22
#define TCON_REG_BT601_656_8BIT		TCON_TCON_CFG_OUT_SEL_23
/* Serial RGB Sequence */
#define TCON_REG_SRGB_SEQ_RGB		TCON_RGB_IF_CFG_OL_SEQ_0
#define TCON_REG_SRGB_SEQ_GBR		TCON_RGB_IF_CFG_OL_SEQ_1
#define TCON_REG_SRGB_SEQ_BRG		TCON_RGB_IF_CFG_OL_SEQ_2
/* CPU mode */
#define TCON_REG_CPU_MODE_CMD		TCON_CPU_IF_CFG_MODE_0
#define TCON_REG_CPU_MODE_PAYLOAD	TCON_CPU_IF_CFG_MODE_1
/* CPU RS */
#define TCON_REG_CPU_RS_CMD		TCON_CPU_IF_OP_RS_0
#define TCON_REG_CPU_RS_DATA		TCON_CPU_IF_OP_RS_1
/* Data Source */
#define TCON_REG_SRC_DISP_CTL		TCON_TCON_CFG_SRC_SEL_0
#define TCON_REG_SRC_DMA		TCON_TCON_CFG_SRC_SEL_1
#define TCON_REG_SRC_COLOR_BAR		TCON_TCON_CFG_SRC_SEL_2
#define TCON_REG_SRC_GREY_SCALE		TCON_TCON_CFG_SRC_SEL_3
#define TCON_REG_SRC_BLACK_WHITE	TCON_TCON_CFG_SRC_SEL_4
#define TCON_REG_SRC_GRID		TCON_TCON_CFG_SRC_SEL_5
#define TCON_REG_SRC_CHECKER_BOARD	TCON_TCON_CFG_SRC_SEL_6
#define TCON_REG_SRC_BG_COLOR		TCON_TCON_CFG_SRC_SEL_7
/* LVDS Mode */
#define TCON_REG_LVDS_JEIDA		TCON_LVDS_IF_CFG_MODE_0
#define TCON_REG_LVDS_VESA		TCON_LVDS_IF_CFG_MODE_1
/* FRC Mode */
#define TCON_REG_FRC_6BIT		TCON_FRC_CFG_R_MODE_0
#define TCON_REG_FRC_5BIT		TCON_FRC_CFG_R_MODE_1
/* GAMMA SRAM */
#define TCON_REG_GAMMA_TBL_HW		TCON_TCON_CFG_SRAM_ACS_0
#define TCON_REG_GAMMA_TBL_SW		TCON_TCON_CFG_SRAM_ACS_1

typedef enum tcon_fifo_mode {
	TCON_FIFO_NORMAL	= TCON_REG_FIFO_NORMAL,
	TCON_FIFO_TWO_HALVES	= TCON_REG_FIFO_TWO_HALVES,
	TCON_FIFO_EC		= TCON_REG_FIFO_EC,
} tcon_fifo_mode_t;

typedef enum tcon_irq_num {
	TCON_LINE_TRIG		= (1 << 0),
	TCON_VBI		= (1 << 1),
	TCON_TRIG_FINISH	= (1 << 8),
	TCON_TE_TRIG		= (1 << 9),
	TCON_EXT_TRIG		= (1 << 10),
	TCON_IRQ_ALL		= (0x00000703),
} tcon_irq_num_t;

typedef enum tcon_vbi_trigger {
	TCON_VBI_VFP  = TCON_REG_VBI_VFP,
	TCON_VBI_VSW  = TCON_REG_VBI_VSW,
	TCON_VBI_VBP  = TCON_REG_VBI_VBP,
	TCON_VBI_VACT = TCON_REG_VBI_VACT,
} tcon_vbi_trigger_t;

typedef struct tcon_vtim {
	u32 vt;
	u32 vact;
	u32 vfp;
	u32 vsw;
} tcon_vtim_t;

typedef struct tcon_htim {
	u32 ht;
	u32 hact;
	u32 hfp;
	u32 hsw;
} tcon_htim_t;

typedef struct tcon_timings {
	bool		is_interlace;
	bool		is_pal;
	u32		width;
	u32		height;
	tcon_vtim_t	field1;
	tcon_vtim_t	field2;
	tcon_htim_t	hline;
	u32		dclk_freq;
} tcon_timings_t;

typedef enum tcon_trig_mode {
	TCON_TRIG_AUTO   = TCON_REG_TRIG_AUTO,
	TCON_TRIG_SINGLE = TCON_REG_TRIG_SINGLE,
} tcon_trig_mode_t;

typedef enum tcon_trig_src {
	TCON_TRIG_INT = TCON_REG_TRIG_INT,
	TCON_TRIG_TE  = TCON_REG_TRIG_TE,
	TCON_TRIG_EXT = TCON_REG_TRIG_EXT,
} tcon_trig_src_t;

typedef enum tcon_trig_src_mode {
	TCON_TRIG_INT_HW	= 0x0,
	TCON_TRIG_INT_SW	= 0x1,
	TCON_TRIG_TE_HW		= 0x2,
	TCON_TRIG_TE_SW		= 0x3,
	TCON_TRIG_EXT_HW	= 0x4,
	TCON_TRIG_EXT_SW	= 0x5,
} tcon_trig_src_mode_t;

typedef enum tcon_out_if {
	TCON_PRGB		= TCON_REG_PRGB,
	TCON_SRGB_RGB		= TCON_REG_SRGB_RGB,
	TCON_SRGB_DUMMY_RGB	= TCON_REG_SRGB_DRGB,
	TCON_SRGB_RGB_DUMMY	= TCON_REG_SRGB_RGBD,
	TCON_CPU_18BIT_RGB666	= TCON_REG_CPU_18BIT_RGB666,
	TCON_CPU_16BIT_RGB888	= TCON_REG_CPU_16BIT_RGB888,
	TCON_CPU_16BIT_RGB565	= TCON_REG_CPU_16BIT_RGB565,
	TCON_CPU_9BIT_RGB666	= TCON_REG_CPU_9BIT_RGB666,
	TCON_CPU_8BIT_RGB565	= TCON_REG_CPU_8BIT_RGB565,
	TCON_CPU_8BIT_RGB888	= TCON_REG_CPU_8BIT_RGB888,
	TCON_BT601_1120_16BIT	= TCON_REG_BT601_1120_16BIT,
	TCON_BT1120_8BIT	= TCON_REG_BT1120_8BIT,
	TCON_BT601_656_8BIT	= TCON_REG_BT601_656_8BIT,
} tcon_out_if_t;

typedef enum tcon_path_sel {
	TCON_PATH_LCD		= TCON_TCON_CFG_PATH_SEL_0,
	TCON_PATH_TV		= TCON_TCON_CFG_PATH_SEL_1,
} tcon_path_sel_t;

typedef enum tcon_srgb_seq {
	TCON_SRGB_SEQ_RGB = TCON_REG_SRGB_SEQ_RGB,
	TCON_SRGB_SEQ_GBR = TCON_REG_SRGB_SEQ_GBR,
	TCON_SRGB_SEQ_BRG = TCON_REG_SRGB_SEQ_BRG,
} tcon_srgb_seq_t;

typedef struct tcon_srgb_if {
	tcon_srgb_seq_t	first_line;
	tcon_srgb_seq_t	second_line;
} tcon_srgb_if_t;

typedef enum tcon_io_pol {
	TCON_IO_NEG	= 0x0,
	TCON_IO_POS	= 0x1,
	TCON_IO_FALLING	= 0x0,
	TCON_IO_RISING	= 0x1,
} tcon_io_pol_t;

typedef struct tcon_rgb_if {
	tcon_srgb_if_t	srgb_if;
	tcon_io_pol_t	field_pol;
	tcon_io_pol_t	vblank_pol;
	tcon_io_pol_t	hblank_pol;
	tcon_io_pol_t	vsync_pol;
	tcon_io_pol_t	hsync_pol;
	tcon_io_pol_t	de_pol;
	tcon_io_pol_t	clk_pol;
	bool		is_clk_ddr;
} tcon_rgb_if_t;

#if 0
typedef enum tcon_cpu_cmd_align {
	TCON_CPU_CMD_ALIGN_L9L = 0x0,
	TCON_CPU_CMD_ALIGN_L9M = 0x1,
	TCON_CPU_CMD_ALIGN_H9L = 0x8,
	TCON_CPU_CMD_ALIGN_H9M = 0x9,
} tcon_cpu_cmd_align_t;
#endif

typedef struct tcon_cpu_if {
	bool			has_vsync_cmd;
	bool			has_hsync_cmd;
	u32			cmd_align;
	tcon_io_pol_t		te_pol;
} tcon_cpu_if_t;

typedef enum tcon_src {
	TCON_SRC_DISP_CTL	= TCON_REG_SRC_DISP_CTL,
	TCON_SRC_DMA		= TCON_REG_SRC_DMA,
	TCON_SRC_COLOR_BAR	= TCON_REG_SRC_COLOR_BAR,
	TCON_SRC_GREY_SCALE	= TCON_REG_SRC_GREY_SCALE,
	TCON_SRC_BLACK_WHITE	= TCON_REG_SRC_BLACK_WHITE,
	TCON_SRC_GRID		= TCON_REG_SRC_GRID,
	TCON_SRC_CHECKER_BOARD	= TCON_REG_SRC_CHECKER_BOARD,
	TCON_SRC_BG_COLOR	= TCON_REG_SRC_BG_COLOR,
} tcon_src_t;

typedef enum tcon_output_fmt {
	TCON_FMT_RGB888		= 0x0,
	TCON_FMT_RGB666		= 0x1,
	TCON_FMT_RGB565		= 0x2,
	TCON_FMT_RGB555		= 0x3,
	TCON_FMT_YUV444		= 0x4,
	TCON_FMT_YUV422		= 0x5,
} tcon_output_fmt_t;

typedef enum tcon_color_depth {
	TCON_COLOR_24BPP	= 0x0,
	TCON_COLOR_18BPP	= 0x1,
	TCON_COLOR_16BPP	= 0x2,
} tcon_color_depth_t;

typedef enum tcon_lvds_mode {
	TCON_LVDS_JEIDA		= TCON_REG_LVDS_JEIDA,
	TCON_LVDS_VESA		= TCON_REG_LVDS_VESA,
} tcon_lvds_mode_t;

typedef struct tcon_lvds_if {
	bool			is_dual_link;
	tcon_lvds_mode_t	mode;
	tcon_color_depth_t	bpp;
	bool			is_ctrl_invert; /* TODO */
} tcon_lvds_if_t;

typedef struct tcon_csc_info {
	u32			coef[12];
	u32			min[3];
	u32			max[3];
} tcon_csc_info_t;

typedef enum tcon_frc_mode {
	TCON_FRC_666		= 0x1,
	TCON_FRC_565		= 0x2,
	TCON_FRC_555		= 0x3,
} tcon_frc_mode_t;

typedef struct tcon_host {
	u32			tcon_index;
	u32			doss_index;
	/* fixed when application */
	tcon_fifo_mode_t	fifo_mode;
	tcon_src_t		tcon_src;
	tcon_trig_src_mode_t	tcon_trig_src_mode;
	tcon_vbi_trigger_t	trig_vbi;
	bool			is_rb_swap;

	tcon_timings_t		timing;
	tcon_output_fmt_t	format;
	tcon_out_if_t		out_if;
	bool			csc_en;
	tcon_csc_info_t		*csc;
	bool			frc_en;
	tcon_frc_mode_t		frc_mode;
	bool			gamma_en;
	u32			*gamma_tbl;
	bool			has_te;
	tcon_io_pol_t		te_pol;
	u32			tcon_dclk_div;
	u32			tcon_dclk_freq;
} tcon_host_t;

s32 csp_tcon_set_register_base(u32 tcon_index, uintptr_t addr, u32 size);
s32 csp_tcon_get_register_base(u32 tcon_index, unsigned long *addr);
s32 csp_tcon_init(u32 tcon_index);
s32 csp_tcon_exit(u32 tcon_index);
s32 csp_tcon_enable(u32 tcon_index);
s32 csp_tcon_disable(u32 tcon_index);
s32 csp_tcon_irq_enable(u32 tcon_index, u32 irqnr);
s32 csp_tcon_irq_disable(u32 tcon_index, u32 irqnr);
bool csp_tcon_query_irq(u32 tcon_index, u32 irqnr);
s32 csp_tcon_irq_clear(u32 tcon_index, u32 irqnr);
s32 csp_tcon_set_vbi_trigger(u32 tcon_index, tcon_vbi_trigger_t trig);
s32 csp_tcon_set_line_trigger(u32 tcon_index, u32 line);
s32 csp_tcon_set_fifo_mode(u32 tcon_index, tcon_fifo_mode_t fifo_mode);
s32 csp_tcon_rb_swap_enable(u32 tcon_index);
s32 csp_tcon_rb_swap_disable(u32 tcon_index);
s32 csp_tcon_dma_enable(u32 tcon_index);
s32 csp_tcon_dma_disable(u32 tcon_index);
s32 csp_tcon_is_lack_data(u32 tidx);
s32 csp_tcon_start(u32 tcon_index);
s32 csp_tcon_stop(u32 tcon_index);
s32 csp_tcon_set_timing(u32 tcon_index, tcon_timings_t *ts,
	tcon_out_if_t out_if);
s32 csp_tcon_get_timing(u32 tcon_index, tcon_timings_t *ts);
s32 csp_tcon_set_trigger_src_mode(u32 tcon_index,
				  tcon_trig_src_mode_t src_mode,
				  tcon_io_pol_t trig_pol);
s32 csp_tcon_set_dc_sync_delay(u32 tcon_index, u32 dly);
s32 csp_tcon_get_dc_sync_delay(u32 tcon_index, u32 *dly);
s32 csp_tcon_get_current_line(u32 tcon_index, u32 *curline);
s32 csp_tcon_set_output_interface(u32 tcon_index, tcon_out_if_t out_if);
s32 csp_tcon_set_path(u32 tcon_index, tcon_path_sel_t path_sel);
s32 csp_tcon_set_rgb_cfg(u32 tcon_index, tcon_rgb_if_t *rgbif);
s32 csp_tcon_set_cpu_cfg(u32 tcon_index, tcon_cpu_if_t *cpuif);
s32 csp_tcon_single_trigger(u32 tcon_index);
bool csp_tcon_single_trigger_is_finish(u32 tcon_index);
s32 csp_tcon_cpu_write_reg_index(u32 tcon_index, u32 index);
s32 csp_tcon_cpu_write_reg_para(u32 tcon_index, u32 para);
s32 csp_tcon_cpu_write_reg(u32 tcon_index, u32 index, u32 *para, u32 len);
s32 csp_tcon_cpu_read_reg_para(u32 tcon_index, u32 *para);
s32 csp_tcon_cpu_read_reg(u32 tcon_index, u32 index, u32 *para, u32 len);
s32 csp_tcon_cpu_write_gram(u32 tcon_index, u32 *data, u32 len);
s32 csp_tcon_cpu_read_gram(u32 tcon_index, u32 *data, u32 len);
s32 csp_tcon_set_lvds_cfg(u32 tcon_index, tcon_lvds_if_t *lvdsif);
s32 csp_tcon_lvds_enable(u32 tcon_index);
s32 csp_tcon_lvds_disable(u32 tcon_index);
s32 csp_tcon_mipi_cm_enable(u32 tcon_index);
s32 csp_tcon_mipi_cm_disable(u32 tcon_index);
s32 csp_tcon_set_csc(u32 tcon_index, tcon_csc_info_t *csc);
s32 csp_tcon_csc_enable(u32 tcon_index);
s32 csp_tcon_csc_disable(u32 tcon_index);
s32 csp_tcon_set_frc(u32 tcon_index, tcon_frc_mode_t mode);
s32 csp_tcon_frc_enable(u32 tcon_index);
s32 csp_tcon_frc_disable(u32 tcon_index);
s32 csp_tcon_set_gamma_table(u32 tcon_index, u32 *tbl, u32 offset, u32 size);
s32 csp_tcon_gamma_enable(u32 tcon_index);
s32 csp_tcon_gamma_disable(u32 tcon_index);
s32 csp_tcon_set_source(u32 tcon_index, tcon_src_t src);
s32 csp_tcon_set_backcolor(u32 tcon_index, u32 backcolor);
s32 csp_tcon_set_clk_dly(u32 tcon_index, u32 dly);

#endif /* _CSP_DOSS_TCON_V1_0_H_ */
