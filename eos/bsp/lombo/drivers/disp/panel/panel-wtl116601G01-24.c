/*
 * panel-wt1096601g01_24_ivo.c - Panel manufacturer module driver code for LomboTech
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
#include "panel.h"

#define DBG_SECTION_NAME	"WT1116601G01_24"
#define DBG_LEVEL		DBG_INFO
#include <debug.h>

/*
 ***********************SELF GPIO CONFIGURATION*****************************
 */
#define DISP_PANEL_MODULE	"panel_wtl116601"

#define DISP_BL_ON		IO_HIGH
#define DISP_BL_OFF		IO_LOW

/*
 ***********************PATTERN DEPENDED PARA START*****************************
 */

/*
 ***********************DEVICE DEPENDENT CONFIGURATION**************************
 */
/*
 * @INTERFACE
 * VO_DEV_PRGB, VO_DEV_SRGB_RGB, VO_DEV_SRGB_DUMMY_RGB, VO_DEV_SRGB_RGB_DUMMY,
 * VO_DEV_CPU_18BIT_RGB666,
 * VO_DEV_CPU_16BIT_RGB888, VO_DEV_CPU_16BIT_RGB666, VO_DEV_CPU_16BIT_RGB565,
 * VO_DEV_CPU_9BIT_RGB666,
 * VO_DEV_CPU_8BIT_RGB666, VO_DEV_CPU_8BIT_RGB565, VO_DEV_CPU_8BIT_RGB888,
 * VO_DEV_LVDS, VO_DEV_MIPI_DSI_VIDEO, VO_DEV_MIPI_DSI_CMD,
 * VO_DEV_BT601_24BIT, VO_DEV_BT601_16BIT, VO_DEV_BT601_8BIT,
 * VO_DEV_BT656_8BIT,
 * VO_DEV_BT1120_16BIT,VO_DEV_BT1120_8BIT
 * VO_DEV_TVE
 */
#define INTERFACE	(VO_DEV_MIPI_DSI_VIDEO)

/*
 * @FIFO_MODE
 * TCON_FIFO_NORMAL, TCON_FIFO_TWO_HALVES, TCON_FIFO_EC
 */
#define FIFO_MODE		(TCON_FIFO_NORMAL)

/*
 * @TCON_SRC
 * TCON_SRC_DISP_CTL, TCON_SRC_DMA, TCON_SRC_COLOR_BAR
 * TCON_SRC_GREY_SCALE,TCON_SRC_BLACK_WHITE,TCON_SRC_GRID
 * TCON_SRC_CHECKER_BOARD,TCON_SRC_BG_COLOR
 */
#define TCON_SRC		(TCON_SRC_DISP_CTL)

/*
 * @TG_TRIG_SRC_MODE
 * TCON_TRIG_INT_HW, TCON_TRIG_INT_SW
 * TCON_TRIG_TE_HW, TCON_TRIG_TE_SW
 * TCON_TRIG_EXT_HW, TCON_TRIG_EXT_SW
 */
#define TG_TRIG_SRC_MODE	(TCON_TRIG_INT_HW)

/*
 * @VBI_TRIG
 * TCON_VBI_VFP, TCON_VBI_VSW, TCON_VBI_VBP, TCON_VBI_VACT
 */
#define VBI_TRIG		(TCON_VBI_VFP)

#define IS_RB_SWAP		(FALSE)

/* mipi dsi host config */
#define IS_MIPI_DUAL	(FALSE)
#define IS_BTA		(FALSE)
#define LANE_NUM	(4)
#define MIPI_TX_FMT	(DSI_RGB888)
#define MIPI_TX_MODE	(DSI_VIDEO_MODE)

/*
 * @TRANS_MODE
 * DSI_NON_BURST_PULSE, DSI_NON_BURST_EVENT, DSI_BURST
 */
#define TRANS_MODE	(DSI_NON_BURST_EVENT)
#define IS_FIXED_BITCLK	(FALSE)	  /* valid when TRANS_MODE == DSIBURST */
#define BITCLK (999000000) /* Unit: Hz, valid when IS_FIXED_BITCLK == TRUE */

/* * @COLOR_DEPTH
* TCON_COLOR_24BPP, TCON_COLOR_18BPP, TCON_COLOR_16BPP
*/
#define COLOR_DEPTH	(TCON_COLOR_24BPP)

/*
 ***********************TCON COMMON CONFIGURATION*******************************
 */
/*
 * @TCON_FMT
 * TCON_FMT_RGB888, TCON_FMT_RGB666, TCON_FMT_RGB565
 * TCON_FMT_YUV444, TCON_FMT_YUV422,
 */
#define TCON_FMT	(TCON_FMT_RGB888) /* (TCON_FMT_RGB888) */
#define IS_INTERLACE	(FALSE)
#define GAMMA_EN	(TRUE) /* (TRUE) */
#define FRC_EN		(FALSE) /* (FALSE) */
#define CSC_EN		(FALSE) /* (FALSE) */
#define HAS_TE		(FALSE)
#define TE_POL		(TCON_IO_POS)

#define WIDTH		(380)
#define HEIGHT		(1920)

#define HBP		(60)
#define HFP		(30)
#define HSW		(30)
#define HACT		(WIDTH)
#define VBP_F1		(20)
#define VFP_F1		(20)
#define VSW_F1		(20)
#define VACT_F1		(HEIGHT)
#define VBP_F2		(0)
#define VFP_F2		(0)
#define VSW_F2		(0)
#define VACT_F2		(0)
#define VT		(VBP_F1 + VFP_F1 + VSW_F1 + VACT_F1)
#define DCLK		(59400000) /* 297/5 = 59.4  */

/* DOSS TOP IO Config */
#define DCS_MDELAY_FLAG (0)
/*
 ***********************PATTERN DEPENDED PARA END*******************************
 */

static u32 gamma_tbl[256] = {
	/* red */
	/* green */
	/* blue */
	0x000000, 0x050505, 0x0A0A0A, 0x0D0D0D, 0x0F0F0F, 0x121212, 0x141414, 0x161616,
	0x191919, 0x1B1B1B, 0x1C1C1C, 0x1E1E1E, 0x202020, 0x222222, 0x242424, 0x252525,
	0x272727, 0x282828, 0x2A2A2A, 0x2C2C2C, 0x2D2D2D, 0x2F2F2F, 0x303030, 0x313131,
	0x333333, 0x343434, 0x363636, 0x373737, 0x383838, 0x393939, 0x3A3A3A, 0x3C3C3C,
	0x3D3D3D, 0x3F3F3F, 0x404040, 0x414141, 0x434343, 0x444444, 0x454545, 0x464646,
	0x484848, 0x494949, 0x4A4A4A, 0x4B4B4B, 0x4C4C4C, 0x4D4D4D, 0x4E4E4E, 0x4F4F4F,
	0x505050, 0x515151, 0x535353, 0x545454, 0x555555, 0x565656, 0x575757, 0x585858,
	0x595959, 0x5A5A5A, 0x5C5C5C, 0x5D5D5D, 0x5E5E5E, 0x5F5F5F, 0x606060, 0x616161,
	0x626262, 0x636363, 0x646464, 0x656565, 0x666666, 0x676767, 0x686868, 0x696969,
	0x6A6A6A, 0x6B6B6B, 0x6C6C6C, 0x6D6D6D, 0x6E6E6E, 0x6F6F6F, 0x707070, 0x717171,
	0x737373, 0x747474, 0x757575, 0x767676, 0x777777, 0x787878, 0x797979, 0x7A7A7A,
	0x7B7B7B, 0x7C7C7C, 0x7D7D7D, 0x7D7D7D, 0x7E7E7E, 0x7F7F7F, 0x808080, 0x818181,
	0x828282, 0x838383, 0x848484, 0x858585, 0x868686, 0x878787, 0x888888, 0x898989,
	0x8A8A8A, 0x8B8B8B, 0x8C8C8C, 0x8D8D8D, 0x8E8E8E, 0x8E8E8E, 0x909090, 0x919191,
	0x929292, 0x939393, 0x949494, 0x959595, 0x959595, 0x969696, 0x979797, 0x989898,
	0x999999, 0x9A9A9A, 0x9A9A9A, 0x9B9B9B, 0x9D9D9D, 0x9E9E9E, 0x9F9F9F, 0x9F9F9F,
	0xA0A0A0, 0xA1A1A1, 0xA2A2A2, 0xA3A3A3, 0xA3A3A3, 0xA4A4A4, 0xA5A5A5, 0xA7A7A7,
	0xA8A8A8, 0xA8A8A8, 0xA9A9A9, 0xAAAAAA, 0xABABAB, 0xACACAC, 0xACACAC, 0xADADAD,
	0xAEAEAE, 0xAFAFAF, 0xAFAFAF, 0xB0B0B0, 0xB2B2B2, 0xB3B3B3, 0xB4B4B4, 0xB4B4B4,
	0xB5B5B5, 0xB6B6B6, 0xB7B7B7, 0xB7B7B7, 0xB9B9B9, 0xBABABA, 0xBBBBBB, 0xBBBBBB,
	0xBCBCBC, 0xBDBDBD, 0xBEBEBE, 0xBEBEBE, 0xBFBFBF, 0xC0C0C0, 0xC0C0C0, 0xC1C1C1,
	0xC2C2C2, 0xC3C3C3, 0xC4C4C4, 0xC5C5C5, 0xC6C6C6, 0xC7C7C7, 0xC7C7C7, 0xC8C8C8,
	0xC9C9C9, 0xC9C9C9, 0xCACACA, 0xCBCBCB, 0xCCCCCC, 0xCDCDCD, 0xCECECE, 0xCFCFCF,
	0xCFCFCF, 0xD0D0D0, 0xD1D1D1, 0xD2D2D2, 0xD2D2D2, 0xD3D3D3, 0xD4D4D4, 0xD4D4D4,
	0xD5D5D5, 0xD6D6D6, 0xD6D6D6, 0xD7D7D7, 0xD8D8D8, 0xD9D9D9, 0xD9D9D9, 0xDADADA,
	0xDBDBDB, 0xDBDBDB, 0xDCDCDC, 0xDDDDDD, 0xDDDDDD, 0xDEDEDE, 0xDFDFDF, 0xE0E0E0,
	0xE1E1E1, 0xE2E2E2, 0xE2E2E2, 0xE3E3E3, 0xE4E4E4, 0xE4E4E4, 0xE5E5E5, 0xE6E6E6,
	0xE6E6E6, 0xE7E7E7, 0xE8E8E8, 0xE8E8E8, 0xE9E9E9, 0xEAEAEA, 0xEAEAEA, 0xEBEBEB,
	0xECECEC, 0xECECEC, 0xEDEDED, 0xEEEEEE, 0xEEEEEE, 0xEFEFEF, 0xF0F0F0, 0xF0F0F0,
	0xF0F0F0, 0xF1F1F1, 0xF1F1F1, 0xF2F2F2, 0xF2F2F2, 0xF3F3F3, 0xF4F4F4, 0xF4F4F4,
	0xF5F5F5, 0xF6F6F6, 0xF6F6F6, 0xF7F7F7, 0xF8F8F8, 0xF8F8F8, 0xF9F9F9, 0xF9F9F9,
	0xFAFAFA, 0xFBFBFB, 0xFBFBFB, 0xFCFCFC, 0xFDFDFD, 0xFDFDFD, 0xFEFEFE, 0xFFFFFF,
};

static u8 init_code[] = {
	/* Sleep Out */
	2,
	0x11, 0x00,
	/* delay 100ms */
	DCS_MDELAY_FLAG, 100,
	/* Display ON */
	2,
	0x29, 0x00,
	/* delay 100ms */
	DCS_MDELAY_FLAG, 100,
};

/* vo dev config */
static void wtl116601G01_24_dev_config(vo_device_t *vo_dev)
{
	tcon_host_t *tcon_host = vo_dev->tcon_host;
	tcon_mipi_dsi_if_t *dsi_if;
	/* device interface, reference to vo_dev_if_t */
	vo_dev->dev_if = INTERFACE;

	/* device specific configuation, reference to vo_if_cfg_t */
	dsi_if = &vo_dev->if_cfg.dsi_if;
	dsi_if->index = 0;
	dsi_if->tcon_host = vo_dev->tcon_host;
	dsi_if->is_dual = IS_MIPI_DUAL;
	dsi_if->is_bta = IS_BTA;
	dsi_if->lane_num = LANE_NUM;
	dsi_if->tx_fmt = MIPI_TX_FMT;
	dsi_if->tx_mode = MIPI_TX_MODE;
	dsi_if->is_bitrate_fixed = IS_FIXED_BITCLK;
	dsi_if->bit_clk = BITCLK;
	dsi_if->tx_mode_cfg.video_mode.trans_mode = TRANS_MODE;

	/* tcon host config */
	tcon_host->fifo_mode = FIFO_MODE;
	tcon_host->tcon_src = TCON_SRC;
	tcon_host->tcon_trig_src_mode = TG_TRIG_SRC_MODE;
	tcon_host->trig_vbi = VBI_TRIG;
	tcon_host->is_rb_swap = IS_RB_SWAP;

	/* timing, reference to tcon_timings_t */
	tcon_host->timing.is_interlace = IS_INTERLACE;
	tcon_host->timing.field1.vt = VT;
	tcon_host->timing.field1.vact = VACT_F1;
	tcon_host->timing.field1.vfp = VFP_F1;
	tcon_host->timing.field1.vsw = VSW_F1;
	tcon_host->timing.field2.vt = (VSW_F2 + VBP_F2 + VACT_F2 + VFP_F2) << 1;
	tcon_host->timing.field2.vact = VACT_F2;
	tcon_host->timing.field2.vfp = VFP_F2;
	tcon_host->timing.field2.vsw = VSW_F2;
	tcon_host->timing.hline.ht = HSW + HBP + HACT + HFP;
	tcon_host->timing.hline.hact = HACT;
	tcon_host->timing.hline.hfp = HFP;
	tcon_host->timing.hline.hsw = HSW;
	tcon_host->timing.dclk_freq = DCLK;
	/* resolution in pixel */
	tcon_host->timing.width = WIDTH;
	tcon_host->timing.height = HEIGHT;

	/* format, reference to tcon_output_fmt_t */
	tcon_host->format = TCON_FMT;
	/* gamma correction */
	tcon_host->gamma_en = GAMMA_EN;
	tcon_host->gamma_tbl = gamma_tbl;
	/* frame rate control (dither) enable */
	tcon_host->frc_en = FRC_EN;
	tcon_host->csc_en = CSC_EN; /* TODO */
	/* TE mode */
	tcon_host->has_te = HAS_TE;
	tcon_host->te_pol = TE_POL;
	/* TCON source */
	/* tcon_host->tcon_src = TCON_REG_SRC_BG_COLOR; */
}

/* Power up */
static void wtl116601G01_24_dev_init(vo_device_t *vo_dev)
{
	u32 dsi_index = vo_dev->if_cfg.dsi_if.index;

	/* regulator init */
	panel_rglt_io_init(DISP_PANEL_MODULE, DISP_RGLT_GROUP);
	panel_io_rglt_set(DISP_PANEL_MODULE, IO_HIGH);

	/* reset init */
	panel_rst_io_init(DISP_PANEL_MODULE, DISP_RESET_GROUP);
	panel_io_rst_set(DISP_PANEL_MODULE, IO_HIGH);
	udelay(10 * 1000);
	panel_io_rst_set(DISP_PANEL_MODULE, IO_LOW);
	udelay(100 * 1000);
	panel_io_rst_set(DISP_PANEL_MODULE, IO_HIGH);
	udelay(10 * 1000);

	/* dsi dcs init */
	mipi_dsi_dcs_write_array(dsi_index, 0, init_code, ARRAY_CNT(init_code),
				DCS_MDELAY_FLAG);

	/* back light init */
	panel_bl_io_init(DISP_PANEL_MODULE, DISP_BL_GROUP);
	panel_io_bl_set(DISP_PANEL_MODULE, DISP_BL_ON);

	/* back light pwm init */
	panel_pwm_init(DISP_PANEL_MODULE);
}

/* Power down */
static void wtl116601G01_24_dev_exit(vo_device_t *vo_dev)
{
	/* back light pwm exit */
	panel_pwm_exit(DISP_PANEL_MODULE);

	/* back light exit */
	panel_io_bl_set(DISP_PANEL_MODULE, DISP_BL_OFF);
	panel_bl_io_exit(DISP_PANEL_MODULE, DISP_BL_GROUP);

	mdelay(100);

	panel_io_rst_set(DISP_PANEL_MODULE, IO_LOW);

	/* reset exit */
	panel_rst_io_exit(DISP_PANEL_MODULE, DISP_RESET_GROUP);

	mdelay(100);

	/* regulator exit */
	panel_io_rglt_set(DISP_PANEL_MODULE, IO_LOW);
	panel_rglt_io_exit(DISP_PANEL_MODULE, DISP_RGLT_GROUP);

	mdelay(100);
}

static int wtl116601G01_24_dev_set_backlight_state(bool state)
{
	int ret;

	if (state)
		ret = panel_io_bl_set(DISP_PANEL_MODULE, DISP_BL_ON);
	else
		ret = panel_io_bl_set(DISP_PANEL_MODULE, DISP_BL_OFF);

	return ret;
}

static int wtl116601G01_24_dev_set_backlight_value(u32 value)
{
	return panel_set_backlight_value(DISP_PANEL_MODULE, value);
}

static int rt_wtl116601G01_24_panel_init(void)
{
	/* this memory not need to free  */
	disp_panel_ctl_t *pctl = rt_malloc(sizeof(disp_panel_ctl_t));

	pctl->ops.dev_config = wtl116601G01_24_dev_config;
	pctl->ops.dev_init = wtl116601G01_24_dev_init;
	pctl->ops.dev_exit = wtl116601G01_24_dev_exit;
	pctl->ops.dev_set_backlight_state = wtl116601G01_24_dev_set_backlight_state;
	pctl->ops.dev_set_backlight_value = wtl116601G01_24_dev_set_backlight_value;

	pctl->width = WIDTH;
	pctl->height = HEIGHT;

	panel_status_init(DISP_PANEL_MODULE, pctl);

	if (panel_list_head != NULL)
		list_add_tail(&pctl->node, panel_list_head);
	else
		rt_kprintf("panel list head has not initiated\n");

	return 0;
}

INIT_APP_EXPORT(rt_wtl116601G01_24_panel_init);
