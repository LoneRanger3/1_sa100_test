/*
 * viss_const.h - viss const head file
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

#ifndef __VISS_CONST__H__
#define __VISS_CONST__H__

/*
 * input field selection
 */
enum viss_field_sel {
	VISS_FIELD_ODD,    /* odd field */
	VISS_FIELD_EVEN,   /* even field */
};

/*
 * input reference signal polarity
 */
enum viss_ref_signal {
	VISS_REF_HIGH,   /* active high */
	VISS_REF_LOW,    /* active low */
};

/*
 * input data valid of the input clock edge type
 */
enum viss_clk_active_type {
	VISS_CLK_RISING,     /* active rising */
	VISS_CLK_FALLING,    /* active falling */
};


/*
 * TOP
 */
enum viss_clksrc_type {
	VISS_CLKSRC_PERH0_PLL_DIV2,
	VISS_CLKSRC_PLL_MUX,
};

enum viss_mclksrc_type {
	VISS_MCLKSRC_HOSC_24M,
	VISS_MCLKSRC_PERH0_PLL_DIV2,
};

/* interface type of VISS */
enum viss_if_type {
	VISS_IF_TYPE_DC,
	VISS_IF_TYPE_ITU_601,
	VISS_IF_TYPE_ITU_656,
	VISS_IF_TYPE_ITU_1120,
	VISS_IF_TYPE_MCSI,
	VISS_IF_TYPE_TVD,
};

/*
 * VIC
 */

/* color format from sensor to VIC */
enum viss_color_fmt {
	VISS_FMT_RGB444 = 0x10,
	VISS_FMT_RGB555,
	VISS_FMT_RGB565,
	VISS_FMT_RGB666,
	VISS_FMT_RGB888,
	VISS_FMT_RGB30_LOCAL,
	VISS_FMT_YCBCR420 = 0x20,
	VISS_FMT_YCRCB420,
	VISS_FMT_YCBYCR422,
	VISS_FMT_YCRYCB422,
	VISS_FMT_CBYCRY422,
	VISS_FMT_CRYCBY422,
	VISS_FMT_YCBCR444_LOCAL,
	VISS_FMT_RAW8 = 0x40,
	VISS_FMT_RAW10,
	VISS_FMT_RAW12,
	VISS_FMT_JPEG = 0x80,
	VISS_FMT_YUYV_JPEG = 0x100,

};

#define VIC_IO_PCLK_INV			(1UL << 0)
#define VIC_IO_FIELD_INV		(1UL << 1)
#define VIC_IO_HS_INV			(1UL << 2)
#define VIC_IO_VS_INV			(1UL << 3)

enum vic_data_path {
	VIC_OUTPUT_DMA,
	VIC_OUTPUT_PIXEL_CH,
	VIC_OUTPUT_BOTH,
};

/* dma chanel */
enum vic_dma_chanel {
	VIC_DMA_CH0,
	VIC_DMA_CH1,
	VIC_DMA_CH2,
	VIC_DMA_CH3,
};

/* output format */
enum vic_output_format {
	VIC_OUTFMT_PAST_THROUGH,
	VIC_OUTFMT_YUV422SP,
	VIC_OUTFMT_YUV420SP,
	VIC_OUTFMT_YUV422P,
	VIC_OUTFMT_YUV420P,
};

/* yuv sequence of input source  */
enum vic_yuv_seq {
	VIC_C0C1C2C3	= 0,
	VIC_UVY0Y1	= 0,
	VIC_C2C1C0C3	= 1,
	VIC_VUY0Y1	= 1,
	VIC_C1C0C3C2	= 2,
	VIC_Y0Y1UV	= 2,
	VIC_C3C0C1C2	= 3,
	VIC_Y0Y1VU	= 3,
};

/* field store order */
enum vic_filed_order {
	VIC_FIELD_NONE,
	VIC_FIELD_SEQ_TB,
	VIC_FIELD_SEQ_BT,
	VIC_FIELD_INTERLACED_TB,
	VIC_FIELD_INTERLACED_BT,
};

enum vic_capture_mode {
	VIC_CAP_SINGLE,
	VIC_CAP_CONT,
};

/* dma address update timing */
enum vic_update_timing {
	VIC_UD_TIMING_VBI,
	VIC_UD_TIMING_FCI,
	VIC_UD_TIMING_MANUALLY,
};

/* offset of dma_ofs and dma_size register between each dma chanel */
#define VIC_DMA_CH_WIN_OFFSET		0x20
/* offset of other register between each dma chanel */
#define VIC_DMA_CH_CFG_OFFSET		0x80

/*
 * DMA[CH] Interrupt Enable/Pending/Clear Register
 */

#define VIC_DMA_INT_FCI			(1UL << 0)
#define VIC_DMA_INT_VBI			(1UL << 1)
#define VIC_DMA_INT_LCTI		(1UL << 2)
#define VIC_DMA_INT_P0_OVF		(1UL << 8)
#define VIC_DMA_INT_P1_OVF		(1UL << 9)
#define VIC_DMA_INT_P2_OVF		(1UL << 10)

#define VIC_DMA_INT_ALL_MASK		(0xFFFFFFFFUL)
#define VIC_DMA_INT_ALL_ERR		\
		(VIC_DMA_INT_P0_OVF | VIC_DMA_INT_P1_OVF | \
		 VIC_DMA_INT_P2_OVF)

/*
 * MIPI_CSI
 */

enum mcsi_dt {
	/* short packet */
	MCSI_FS			= 0x00,
	MCSI_FE			= 0x01,
	MCSI_LS			= 0x02,
	MCSI_LE			= 0x03,
	MCSI_GSP0		= 0x08,
	MCSI_GSP1		= 0x09,
	MCSI_GSP2		= 0x0a,
	MCSI_GSP3		= 0x0b,
	MCSI_GSP4		= 0x0c,
	MCSI_GSP5		= 0x0d,
	MCSI_GSP6		= 0x0e,
	MCSI_GSP7		= 0x0f,
	/* long packet */
	MCSI_NULL		= 0x10,
	MCSI_BLK		= 0x11,
	MCSI_EMBD		= 0x12,
	MCSI_YUV420		= 0x18,
	MCSI_YUV420_10BIT	= 0x19,
	MCSI_YUV420_LGY		= 0x1a,
	MCSI_YUV420_CSPS	= 0x1c,
	MCSI_YUV420_CSPS_10BIT	= 0x1d,
	MCSI_YUV422		= 0x1e,
	MCSI_YUV422_10BIT	= 0x1f,
	MCSI_RGB565		= 0x22,
	MCSI_RGB888		= 0x24,
	MCSI_RAW8		= 0x2a,
	MCSI_RAW10		= 0x2b,
	MCSI_RAW12		= 0x2c,
	MCSI_UD0		= 0x30,
	MCSI_UD1		= 0x31,
	MCSI_UD2		= 0x32,
	MCSI_UD3		= 0x33,
	MCSI_UD4		= 0x34,
	MCSI_UD5		= 0x35,
	MCSI_UD6		= 0x36,
	MCSI_UD7		= 0x37,
};

enum mcsi_vc {
	MCSI_VC0 = 0,
	MCSI_VC1 = 1,
	MCSI_VC2 = 2,
	MCSI_VC3 = 3,
};

enum mcsi_id {
	MCSI_ID0 = 0,
	MCSI_ID1 = 1,
	MCSI_ID2 = 2,
	MCSI_ID3 = 3,
};


enum mcsi_data_path {
	MCSI_OUTPUT_DMA,
	MCSI_OUTPUT_PIXEL_CH,
	MCSI_OUTPUT_BOTH,
};

/* output format */
enum mcsi_output_format {
	MCSI_OUTFMT_PAST_THROUGH,
	MCSI_OUTFMT_YUV422SP,
	MCSI_OUTFMT_YUV420SP,
	MCSI_OUTFMT_YUV422P,
	MCSI_OUTFMT_YUV420P,
};

/* yuv sequence of input source  */
enum mcsi_yuv_seq {
	MCSI_C0C1C2C3	= 0,
	MCSI_C2C1C0C3	= 1,
	MCSI_C1C0C3C2	= 2,
	MCSI_C3C0C1C2	= 3,
};

enum mcsi_capture_mode {
	MCSI_CAP_SINGLE,
	MCSI_CAP_CONT,
};

/* dma address update timing */
enum mcsi_update_timing {
	MCSI_UD_TIMING_VBI,
	MCSI_UD_TIMING_FCI,
	MCSI_UD_TIMING_MANUALLY,
};

/* Interrupt gate */
enum mcsi_int {
	MCSI_INT_DMA,
	MCSI_INT_MIPI,
};

/*
 * DMA Interrupt Enable/Pending/Clear Register
 */

#define MCSI_DMA_INT_FCI		(1UL << 0)
#define MCSI_DMA_INT_VBI		(1UL << 1)
#define MCSI_DMA_INT_LCTI		(1UL << 2)
#define MCSI_DMA_INT_P0_OVF		(1UL << 8)
#define MCSI_DMA_INT_P1_OVF		(1UL << 9)
#define MCSI_DMA_INT_P2_OVF		(1UL << 10)
#define MCSI_DMA_INT_LB_OR_EN           (1UL << 13)

#define MCSI_DMA_INT_ALL_MASK		(0xFFFFFFFFUL)
#define MCSI_DMA_INT_ALL_ERR		\
		(MCSI_DMA_INT_P0_OVF | MCSI_DMA_INT_P1_OVF | \
		 MCSI_DMA_INT_P2_OVF)

/******************************************************************************
 * TVD
 *****************************************************************************/
enum wb_update_timing {
	WB_EVERY_FCI	= 0,
	WB_MANUALLY	= 1,
};

enum wb_storage_mode {
	WB_FIELD_MODE	= 0,
	WB_FRAME_MODE0	= 1,
	WB_FRAME_MODE1	= 2,
};


enum wb_burst_length {
	WB_BURST1	= 0,
	WB_BURST4	= 1,
	WB_BURST8	= 2,
};

enum wb_uv_seq {
	WB_UVUV		= 0,
	WB_VUVU		= 1,
};

enum wb_fci_timing {
	WB_1_VSYNC	= 0,
	WB_2_VSYNC	= 1,
};

enum tvd_wb_output_format {
	TVD_WB_YUV420SP	= 0,
	TVD_WB_YUV422SP	= 1,
	TVD_WB_YUV422	= 3,
};

enum tvd_video_standard {
	TVD_NTSC	= 0,
	TVD_PAL		= 1,
};

enum tvd_scan_lines {
	TVD_525		= 0,
	TVD_625		= 1,
};

enum tvd_display_format {
	TVD_858		= 0,
	TVD_864		= 1,
};

enum tvd_pedestal {
	TVD_NO_PED	= 0,
	TVD_PED		= 1,
};

enum tvd_bg_width {
	TVD_5_CYCLE	= 0,
	TVD_10_CYCLE	= 1,
};

enum tvd_chroma_lpf_width {
	TVD_C_NARROW	= 0,
	TVD_C_WIDE	= 1,
	TVD_C_EXT_WIDE	= 2,
};

enum tvd_luma_nf_width {
	TVD_Y_NONE	= 0,
	TVD_Y_NARROW	= 1,
	TVD_Y_MEDIUM	= 2,
	TVD_Y_WIDE	= 3,
};

enum tvd_afe_clamp_mode {
	TVD_AUTO	= 0,
	TVD_BP		= 1,
	TVD_STIP	= 2,
	TVD_OFF		= 3,
};

enum tvd_gain_update {
	TVD_PER_LINE	= 0,
	TVD_PER_FIELD	= 1,
};

enum tvd_ntsc_ycsep_mode {
	TVD_NTSC_2D	= 0,
	TVD_NTSC_1D	= 1,
	TVD_NTSC_YNOTCH	= 3,
	TVD_NTSC_2TAP	= 4,
	TVD_NTSC_3TAP	= 5,
};

enum tvd_pal_ycsep_mode {
	TVD_PAL_2D		= 0,
	TVD_PAL_YNOTCH_WHBS	= 1,
	TVD_PAL_5TAP_WOHBS	= 2,
	TVD_PAL_YNOTCH_WOHBS	= 3,
	TVD_PAL_YNOTCH_CCOMB	= 4,
	TVD_PAL_3TAP		= 5,
	TVD_PAL_5TAP_WHBS	= 6,
};

enum tvd_3d_cf_mode {
	TVD_3DCF_2D		= 0,
	TVD_3DCF_3D		= 1,
	TVD_3DCF_3TAP		= 2,
	TVD_3DCF_9TAP		= 3,
	TVD_3DCF_3D_WOHBS	= 4,
};

#define TVD_3D_DMA_BUF_SZ		(0x160000)

#define TVD_STA_NO_SIGNAL_DET		(1UL << 0)
#define TVD_STA_HLOCK			(1UL << 1)
#define TVD_STA_VLOCK			(1UL << 2)
#define TVD_STA_CHROMEA_LOCK		(1UL << 3)
#define TVD_STA_MV_VBI_DET		(1UL << 4)

#define TVD_STA_PSCAN_DET		(1UL << 8)
#define TVD_STA_HFNSIS_DET		(1UL << 9)
#define TVD_STA_VFNSIS_DET		(1UL << 10)
#define TVD_STA_PAL_DET			(1UL << 16)
#define TVD_STA_SECAM_DET		(1UL << 17)
#define TVD_STA_LINE625_DET		(1UL << 18)
#define TVD_STA_NOISY_DET		(1UL << 19)
#define TVD_STA_VCR_DET			(1UL << 20)
#define TVD_STA_VCR_TRICK_DET		(1UL << 21)
#define TVD_STA_VCR_FF_DET		(1UL << 22)
#define TVD_STA_VCR_REWIND_DET		(1UL << 23)

#define TVD_STA_MV_CS_DET_MASK		(((1UL << 3) - 1) << 5)
#define TVD_STA_MV_CS_DET_SHIFT		(5)

#define TVD_M_DETECTION_MASK	(TVD_STA_NO_SIGNAL_DET |		\
			TVD_STA_HLOCK | TVD_STA_VLOCK |			\
			TVD_STA_CHROMEA_LOCK | TVD_STA_HFNSIS_DET |	\
			TVD_STA_VFNSIS_DET | TVD_STA_PAL_DET |		\
			TVD_STA_LINE625_DET)

#define TVD_NTSC_M_DETECTION_STA	(TVD_STA_HLOCK |		\
			TVD_STA_VLOCK | TVD_STA_CHROMEA_LOCK)

#define TVD_PAL_M_DETECTION_STA		(TVD_STA_HLOCK |		\
			TVD_STA_VLOCK | TVD_STA_CHROMEA_LOCK |		\
			TVD_STA_PAL_DET | TVD_STA_LINE625_DET)

/*
 * TVD Interrupt Enable/Pending/Clear Register
 */
#define TVD_INT_SIG_LOCK		(1UL << 0)
#define TVD_INT_SIG_UNLOCK		(1UL << 1)
#define TVD_INT_FRAME_END		(1UL << 2)
#define TVD_INT_LWB_DMA_COMP		(1UL << 6)
#define TVD_INT_CWB_DMA_COMP		(1UL << 7)
#define TVD_INT_CF3D_DMA0_FOR		(1UL << 8)
#define TVD_INT_CF3D_DMA0_FUR		(1UL << 9)
#define TVD_INT_CF3D_DMA1_FOR		(1UL << 10)
#define TVD_INT_CF3D_DMA1_FUR		(1UL << 11)
#define TVD_INT_LWB_DMA_FOR		(1UL << 12)
#define TVD_INT_LWB_DMA_FUR		(1UL << 13)
#define TVD_INT_CWB_DMA_FOR		(1UL << 14)
#define TVD_INT_CWB_DMA_FUR		(1UL << 15)
#define TVD_INT_LB0_OR			(1UL << 16)
#define TVD_INT_LB1_OR			(1UL << 17)
#define TVD_INT_PLUG			(1UL << 20)

#define TVD_INT_ALL_MASK		(0xFFFFFFFFUL)
#define TVD_INT_ALL_ERR		\
		(TVD_INT_CF3D_DMA0_FOR | TVD_INT_CF3D_DMA0_FUR |	\
		 TVD_INT_CF3D_DMA1_FOR | TVD_INT_CF3D_DMA1_FUR |	\
		 TVD_INT_LWB_DMA_FOR | TVD_INT_LWB_DMA_FUR |		\
		 TVD_INT_CWB_DMA_FOR | TVD_INT_CWB_DMA_FUR |		\
		 TVD_INT_LB0_OR | TVD_INT_LB1_OR)

/******************************************************************************
 * ISP
 */

/*
 * ISP-LITE
 */

#endif /* __VISS_CONST__H__ */

