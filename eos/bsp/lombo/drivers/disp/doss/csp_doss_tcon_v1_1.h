/*
------------------------------------------------------------------------------
tcon.h
2018-02-08
raymond

提供tcon常用操作函数头文件
------------------------------------------------------------------------------
*/

#ifndef _CSP_DOSS_TCON_V1_1_H_
#define _CSP_DOSS_TCON_V1_1_H_

#include "../disp_common.h"
#include "csp_doss_tcon_reg_v1_1.h"

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
	u32 vfp_float;
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
	TCON_TVE_NTSC		= TCON_REG_TVE_NTSC,
	TCON_TVE_PAL		= TCON_REG_TVE_PAL,
} tcon_out_if_t;

typedef enum tcon_path_sel {
	TCON_PATH_LCD		= 0x0, /*TCON_TCON_CFG_PATH_SEL_0,*/
	TCON_PATH_TV		= 0x1, /*TCON_TCON_CFG_PATH_SEL_1,*/
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

/*
typedef enum tcon_cpu_cmd_align
{
	TCON_CPU_CMD_ALIGN_L9L = 0x0,
	TCON_CPU_CMD_ALIGN_L9M = 0x1,
	TCON_CPU_CMD_ALIGN_H9L = 0x8,
	TCON_CPU_CMD_ALIGN_H9M = 0x9,
} tcon_cpu_cmd_align_t;
 */

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

typedef enum tcon_timing_io_map {
	TCON_NORMAL_TIMING	= 0x0,
	TCON_BT_TIMING	= 0x1,
} tcon_timing_io_map_t;

typedef enum tcon_field_map {
	TCON_FUNC_MAP_FIELD	= 0x0,
	TCON_FUNC_MAP_FIELD_BT	= 0x1,
} tcon_field_map_t;

typedef enum tcon_vs_map {
	TCON_FUNC_MAP_VSYNC	= 0x0,
	TCON_FUNC_MAP_VSYNC_BT	= 0x1,
} tcon_vs_map_t;

typedef enum tcon_hs_map {
	TCON_FUNC_MAP_HSYNC	= 0x0,
	TCON_FUNC_MAP_HSYNC_BT	= 0x1,
} tcon_hs_map_t;

typedef enum tcon_de_map {
	TCON_FUNC_MAP_DE	= 0x0,
	TCON_FUNC_MAP_DE_BT	= 0x1,
} tcon_de_map_t;

typedef enum tcon_bt_yc_down_sample {
	TCON_BT_YC_DS_DIRECTLY	= TCON_REG_BT_YC_DS_DIRECTLY,
	TCON_BT_YC_DS_AVERAGED	= TCON_REG_BT_YC_DS_AVERAGED,
} tcon_bt_yc_down_sample_t;

typedef enum tcon_bt_yc_sequence {
	TCON_BT_YC_SEQ_CB0Y0CR0Y1	= TCON_REG_BT_YC_SEQ_CB0Y0CR0Y1,
	TCON_BT_YC_SEQ_CR0Y0CB0Y1	= TCON_REG_BT_YC_SEQ_CR0Y0CB0Y1,
	TCON_BT_YC_SEQ_Y0CB0Y1CR0	= TCON_REG_BT_YC_SEQ_Y0CB0Y1CR0,
	TCON_BT_YC_SEQ_Y0CR0Y1CB0	= TCON_REG_BT_YC_SEQ_Y0CR0Y1CB0,
} tcon_bt_yc_sequence_t;

typedef struct tcon_bt_if {
	tcon_io_pol_t		field_pol;
	tcon_io_pol_t		vb_pol;
	tcon_io_pol_t		hb_pol;
	tcon_io_pol_t		de_pol;
	tcon_field_map_t	field_map;
	tcon_vs_map_t		vs_map;
	tcon_hs_map_t		hs_map;
	tcon_de_map_t		de_map;
	tcon_bt_yc_down_sample_t ds_mode;
	tcon_bt_yc_sequence_t yc_senquence;
	bool		is_clk_ddr;
} tcon_bt_if_t;

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
	u32			idx;

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

extern s32 csp_tcon_set_register_base(u32 tidx, uintptr_t addr, u32 size);
extern s32 csp_tcon_get_register_base(u32 tidx, uintptr_t *addr);
extern s32 csp_tcon_init(u32 tidx);
extern s32 csp_tcon_exit(u32 tidx);
extern s32 csp_tcon_enable(u32 tidx);
extern s32 csp_tcon_disable(u32 tidx);
extern s32 csp_tcon_irq_enable(u32 tidx, u32 irqnr);
extern s32 csp_tcon_irq_disable(u32 tidx, u32 irqnr);
extern bool csp_tcon_query_irq(u32 tidx, u32 irqnr);
extern s32 csp_tcon_irq_clear(u32 tidx, u32 irqnr);
extern s32 csp_tcon_set_vbi_trigger(u32 tidx, tcon_vbi_trigger_t trig);
extern s32 csp_tcon_set_line_trigger(u32 tidx, u32 line);
extern s32 csp_tcon_set_fifo_mode(u32 tidx, tcon_fifo_mode_t fifo_mode);
extern s32 csp_tcon_rb_swap_enable(u32 tidx);
extern s32 csp_tcon_rb_swap_disable(u32 tidx);
extern s32 csp_tcon_dma_enable(u32 tidx);
extern s32 csp_tcon_dma_disable(u32 tidx);
extern s32 csp_tcon_is_lack_data(u32 tidx);
extern s32 csp_tcon_start(u32 tidx);
extern s32 csp_tcon_stop(u32 tidx);
extern s32 csp_tcon_set_timing(u32 tidx, tcon_timings_t *ts,
			       tcon_out_if_t out_if);
extern s32 csp_tcon_get_timing(u32 tidx, tcon_timings_t *ts);
extern s32 csp_tcon_set_trigger_src_mode(u32 tidx,
					 tcon_trig_src_mode_t src_mode,
					 tcon_io_pol_t trig_pol);
extern s32 csp_tcon_set_dc_sync_delay(u32 tidx, u32 dly);
extern s32 csp_tcon_get_dc_sync_delay(u32 tidx, u32 *dly);
extern s32 csp_tcon_get_current_line(u32 tidx, u32 *curline);
extern s32 csp_tcon_set_output_interface(u32 tidx, tcon_out_if_t out_if);
extern s32 csp_tcon_set_path(u32 tidx, tcon_path_sel_t path_sel);
extern s32 csp_tcon_set_rgb_cfg(u32 tidx, tcon_rgb_if_t *rgbif);
extern s32 csp_tcon_set_cpu_cfg(u32 tidx, tcon_cpu_if_t *cpuif);
extern s32 csp_tcon_set_bt_cfg(u32 tidx, tcon_bt_if_t *btif);
extern s32 csp_tcon_iomapping_cfg(u32 tidx, tcon_timing_io_map_t timing_io_map);
extern s32 csp_tcon_single_trigger(u32 tidx);
extern bool csp_tcon_single_trigger_is_finish(u32 tidx);
extern s32 csp_tcon_cpu_write_reg_index(u32 tidx, u32 index);
extern s32 csp_tcon_cpu_write_reg_para(u32 tidx, u32 para);
extern s32 csp_tcon_cpu_write_reg(u32 tidx, u32 index, u32 *para, u32 len);
extern s32 csp_tcon_cpu_read_reg_para(u32 tidx, u32 *para);
extern s32 csp_tcon_cpu_read_reg(u32 tidx, u32 index, u32 *para, u32 len);
extern s32 csp_tcon_cpu_write_gram(u32 tidx, u32 *data, u32 len);
extern s32 csp_tcon_cpu_read_gram(u32 tidx, u32 *data, u32 len);
extern s32 csp_tcon_set_lvds_cfg(u32 tidx, tcon_lvds_if_t *lvdsif);
extern s32 csp_tcon_lvds_enable(u32 tidx);
extern s32 csp_tcon_lvds_disable(u32 tidx);
extern s32 csp_tcon_mipi_cm_enable(u32 tidx);
extern s32 csp_tcon_mipi_cm_disable(u32 tidx);
extern s32 csp_tcon_set_csc(u32 tidx, tcon_csc_info_t *csc);
extern s32 csp_tcon_csc_enable(u32 tidx);
extern s32 csp_tcon_csc_disable(u32 tidx);
extern s32 csp_tcon_set_frc(u32 tidx, tcon_frc_mode_t mode);
extern s32 csp_tcon_frc_enable(u32 tidx);
extern s32 csp_tcon_frc_disable(u32 tidx);
extern s32 csp_tcon_set_gamma_table(u32 tidx, u32 *tbl, u32 offset, u32 size);
extern s32 csp_tcon_gamma_enable(u32 tidx);
extern s32 csp_tcon_gamma_disable(u32 tidx);
extern s32 csp_tcon_set_source(u32 tidx, tcon_src_t src);
extern s32 csp_tcon_set_backcolor(u32 tidx, u32 backcolor);
extern s32 csp_tcon_set_clk_dly(u32 tidx, u32 dly);

#endif /* _CSP_DOSS_TCON_V1_1_H_ */
