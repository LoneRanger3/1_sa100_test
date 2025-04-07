/*
 * panel-tianma-tm070jdhg30-00.c - Panel manufacturer module driver code for LomboTech
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
#include <pinctrl.h>
#include <cfg/config_api.h>

#define DBG_SECTION_NAME	"SLM5P5_HX8399C"
#define DBG_LEVEL		DBG_INFO
#include <debug.h>

/*
 ***********************SELF GPIO CONFIGURATION*****************************
 */
#define DISP_PANEL_MODULE	"panel_hx8399c"

#define DISP_BL_ON		IO_LOW
#define DISP_BL_OFF		IO_HIGH

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
#define BITCLK (849000000) /* Unit: Hz, valid when IS_FIXED_BITCLK == TRUE */

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
#define GAMMA_EN	(FALSE) /* (FALSE) */
#define FRC_EN		(FALSE) /* (FALSE) */
#define CSC_EN		(FALSE) /* (FALSE) */
#define HAS_TE		(FALSE)
#define TE_POL		(TCON_IO_POS)

#define WIDTH		(1080)
#define HEIGHT		(1920)

#define HBP		(60)
#define HFP		(16)
#define HSW		(60)
#define HACT		(WIDTH)
#define VBP_F1		(3)
#define VFP_F1		(9)
#define VSW_F1		(4)
#define VACT_F1		(HEIGHT)
#define VBP_F2		(0)
#define VFP_F2		(0)
#define VSW_F2		(0)
#define VACT_F2		(0)
#define VT		(VBP_F1 + VFP_F1 + VSW_F1 + VACT_F1)
#define DCLK		(74250000) /* 297/2 = 148.5MHz */

/* DOSS TOP IO Config */
#define DCS_MDELAY_FLAG (0)
/*
 ***********************PATTERN DEPENDED PARA END*******************************
 */

static u32 gamma_tbl[256] = {
	/* red   power = 2.2 */
	/* green power = 1/2.2 */
	/* blue  power = 1.4 */
	0x000000, 0x001400, 0x001c00, 0x002100, 0x002600, 0x002a01, 0x002e01, 0x003101,
	0x003402, 0x003702, 0x003a02, 0x003d03, 0x003f03, 0x004203, 0x004404, 0x004604,
	0x004805, 0x004a05, 0x004c06, 0x004e06, 0x005007, 0x015207, 0x015308, 0x015508,
	0x015709, 0x015809, 0x015a0a, 0x015c0a, 0x015d0b, 0x025f0c, 0x02600c, 0x02620d,
	0x02630d, 0x02640e, 0x03660f, 0x03670f, 0x036810, 0x036a11, 0x036b11, 0x046c12,
	0x046e13, 0x046f13, 0x047014, 0x057115, 0x057215, 0x057416, 0x057517, 0x067617,
	0x067718, 0x067819, 0x07791a, 0x077a1a, 0x077c1b, 0x087d1c, 0x087e1c, 0x087f1d,
	0x09801e, 0x09811f, 0x098220, 0x0a8320, 0x0a8421, 0x0a8522, 0x0b8623, 0x0b8723,
	0x0c8824, 0x0c8925, 0x0c8a26, 0x0d8b27, 0x0d8c28, 0x0e8d28, 0x0e8d29, 0x0f8e2a,
	0x0f8f2b, 0x10902c, 0x10912d, 0x11922d, 0x11932e, 0x12942f, 0x129530, 0x139631,
	0x139632, 0x149733, 0x149834, 0x159934, 0x169a35, 0x169b36, 0x179b37, 0x179c38,
	0x189d39, 0x199e3a, 0x199f3b, 0x1a9f3c, 0x1aa03d, 0x1ba13e, 0x1ca23e, 0x1ca33f,
	0x1da340, 0x1ea441, 0x1ea542, 0x1fa643, 0x20a644, 0x21a745, 0x21a846, 0x22a947,
	0x23a948, 0x24aa49, 0x24ab4a, 0x25ac4b, 0x26ac4c, 0x27ad4d, 0x27ae4e, 0x28af4f,
	0x29af50, 0x2ab051, 0x2bb152, 0x2cb153, 0x2cb254, 0x2db355, 0x2eb456, 0x2fb457,
	0x30b558, 0x31b659, 0x32b65a, 0x33b75b, 0x33b85c, 0x34b85d, 0x35b95e, 0x36ba5f,
	0x37ba61, 0x38bb62, 0x39bc63, 0x3abc64, 0x3bbd65, 0x3cbe66, 0x3dbe67, 0x3ebf68,
	0x3fc069, 0x40c06a, 0x41c16b, 0x42c16c, 0x43c26d, 0x44c36f, 0x46c370, 0x47c471,
	0x48c572, 0x49c573, 0x4ac674, 0x4bc675, 0x4cc776, 0x4dc877, 0x4ec879, 0x50c97a,
	0x51c97b, 0x52ca7c, 0x53cb7d, 0x54cb7e, 0x56cc7f, 0x57cc81, 0x58cd82, 0x59ce83,
	0x5bce84, 0x5ccf85, 0x5dcf86, 0x5ed088, 0x60d189, 0x61d18a, 0x62d28b, 0x64d28c,
	0x65d38d, 0x66d38f, 0x68d490, 0x69d591, 0x6ad592, 0x6cd693, 0x6dd695, 0x6ed796,
	0x70d797, 0x71d898, 0x73d999, 0x74d99b, 0x75da9c, 0x77da9d, 0x78db9e, 0x7adba0,
	0x7bdca1, 0x7ddca2, 0x7edda3, 0x80dda4, 0x81dea6, 0x83dfa7, 0x84dfa8, 0x86e0a9,
	0x87e0ab, 0x89e1ac, 0x8be1ad, 0x8ce2ae, 0x8ee2b0, 0x8fe3b1, 0x91e3b2, 0x93e4b3,
	0x94e4b5, 0x96e5b6, 0x98e5b7, 0x99e6b9, 0x9be6ba, 0x9de7bb, 0x9ee7bc, 0xa0e8be,
	0xa2e8bf, 0xa3e9c0, 0xa5e9c2, 0xa7eac3, 0xa9eac4, 0xaaebc5, 0xacebc7, 0xaeecc8,
	0xb0ecc9, 0xb1edcb, 0xb3edcc, 0xb5eecd, 0xb7eecf, 0xb9efd0, 0xbbefd1, 0xbcf0d3,
	0xbef0d4, 0xc0f1d5, 0xc2f1d7, 0xc4f2d8, 0xc6f2d9, 0xc8f3db, 0xcaf3dc, 0xccf4dd,
	0xcef4df, 0xd0f5e0, 0xd2f5e1, 0xd4f6e3, 0xd6f6e4, 0xd8f7e5, 0xdaf7e7, 0xdcf8e8,
	0xdef8e9, 0xe0f9eb, 0xe2f9ec, 0xe4faed, 0xe6faef, 0xe8faf0, 0xeafbf2, 0xecfbf3,
	0xeefcf4, 0xf0fcf6, 0xf2fdf7, 0xf5fdf9, 0xf7fefa, 0xf9fefb, 0xfbfffd, 0xfdfffe,
};

static u8 init_code[] = {
	/* source scan sequence */
	0x02,
	0x36, 0x02,
	/* Set EXTC */
	0x04,
	0xB9, 0xFF, 0x83, 0x99,
	/* Set Power */
	0x10,
	0xB1, 0x02, 0x04, 0x6D, 0x8D, 0x01, 0x32, 0x33, 0x11, 0x11, 0x5A, 0x5F,
	    0x56, 0x73, 0x02, 0x02,
	/* Set Display */
	0x0C,
	0xB2, 0x00, 0x80, 0x80, 0xAE, 0x05, 0x07, 0x5A, 0x11, 0x10, 0x10, 0x00,
	/* Set CYC */
	0x2D,
	0xB4, 0x00, 0xFF, 0x10, 0x18, 0x04, 0x9A, 0x00, 0x00, 0x06, 0x00, 0x02,
	    0x04, 0x00, 0x24, 0x02,
	0x04, 0x0A, 0x21, 0x03, 0x00, 0x00, 0x02, 0x9F, 0x88, 0x10, 0x18, 0x04,
	    0x9A, 0x00, 0x00, 0x08,
	0x00, 0x02, 0x04, 0x00, 0x24, 0x02, 0x04, 0x0A, 0x00, 0x00, 0x02, 0x9F,
	    0x12,
	/* Set GIP_0 */
	0x22,
	0xD3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x05,
	    0x00, 0x05, 0x00, 0x07,
	0x88, 0x07, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00,
	    0x01, 0x00, 0x00, 0x00,
	0x05, 0x40,
	/* Set GIP_1 */
	0x21,
	0xD5, 0x20, 0x20, 0x19, 0x19, 0x18, 0x18, 0x01, 0x01, 0x00, 0x00, 0x25,
	    0x25, 0x18, 0x18, 0x18,
	0x18, 0x24, 0x24, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x2F,
	    0x2F, 0x30, 0x30, 0x31,
	0x31,
	/* Set GIP_2 */
	0x21,
	0xD6, 0x24, 0x24, 0x18, 0x18, 0x19, 0x19, 0x00, 0x00, 0x01, 0x01, 0x25,
	    0x25, 0x18, 0x18, 0x18,
	0x18, 0x20, 0x20, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x2F,
	    0x2F, 0x30, 0x30, 0x31,
	0x31,
	/* Set GIP_3_0 */
	0x11,
	0xD8, 0xAA, 0x8A, 0xAA, 0xAA, 0xAA, 0x8A, 0xAA, 0xAA, 0xAA, 0x8A, 0xAA,
	    0xAA, 0xAA, 0x8A, 0xAA,
	0xAA,
	/* Set Bank */
	0x02,
	0xBD, 0x01,
	/* Set GIP_3_1 */
	0x11,
	0xD8, 0xFF, 0xFC, 0xC0, 0x3F, 0xFF, 0xFC, 0xC0, 0x3F, 0xFF, 0xFC, 0xC0,
	    0x3F, 0xFF, 0xFC, 0xC0,
	0x3F,
	/* Set Bank */
	0x02,
	0xBD, 0x02,
	/* Set GIP_3_2 */
	0x09,
	0xD8, 0xFF, 0xFC, 0xC0, 0x3F, 0xFF, 0xFC, 0xC0, 0x3F,
	/* Set Bank */
	0x02,
	0xBD, 0x00,
	/* Set VCOM */
	0x03,
	0xB6, 0x86, 0x86,
	/* Set Panel */
	0x02,
	0xCC, 0x00,
	/* Set GAMMA */
	0x37,
	0xE0, 0x02, 0x2C, 0x3A, 0x35, 0x6E, 0x76, 0x81, 0x7C, 0x81, 0x89, 0x91,
	    0x94, 0x99, 0x9C, 0xA4,
	0xA6, 0xA9, 0xAF, 0xB1, 0xB8, 0xA9, 0xB6, 0xB9, 0x5F, 0x5A, 0x62, 0x7C,
	    0x02, 0x2C, 0x3A, 0x35,
	0x6E, 0x76, 0x81, 0x7C, 0x81, 0x89, 0x91, 0x94, 0x99, 0x9C, 0xA3, 0xA5,
	    0xA7, 0xAF, 0xB1, 0xB6,
	0xAA, 0xB6, 0xB7, 0x5E, 0x59, 0x62, 0x7C,
	/* Checksum Enable */
	0x02,
	0xD0, 0x39,
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
static void slm5p5_hx8399c_dev_config(vo_device_t *vo_dev)
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
static void slm5p5_hx8399c_dev_init(vo_device_t *vo_dev)
{
	u32 dsi_index = vo_dev->if_cfg.dsi_if.index;

	/* reset init */
	panel_rst_io_init(DISP_PANEL_MODULE, DISP_RESET_GROUP);
	panel_io_rst_set(DISP_PANEL_MODULE, IO_HIGH);
	disp_delay(100);
	panel_io_rst_set(DISP_PANEL_MODULE, IO_LOW);
	disp_delay(100);
	panel_io_rst_set(DISP_PANEL_MODULE, IO_HIGH);
	disp_delay(100);

	/* back light init */
	panel_bl_io_init(DISP_PANEL_MODULE, DISP_BL_GROUP);
	panel_io_bl_set(DISP_PANEL_MODULE, DISP_BL_ON);

	/* dsi dcs init */
	mipi_dsi_dcs_write_array(dsi_index, 0, init_code, ARRAY_CNT(init_code),
				DCS_MDELAY_FLAG);
}

/* Power down */
static void slm5p5_hx8399c_dev_exit(vo_device_t *vo_dev)
{
	/* back light exit */
	panel_io_bl_set(DISP_PANEL_MODULE, DISP_BL_OFF);
	panel_bl_io_exit(DISP_PANEL_MODULE, DISP_BL_GROUP);

	/* reset exit */
	panel_rst_io_exit(DISP_PANEL_MODULE, DISP_RESET_GROUP);
}

static int slm5p5_hx8399c_dev_set_backlight_state(bool state)
{
	int ret;

	if (state)
		ret = panel_io_bl_set(DISP_PANEL_MODULE, DISP_BL_ON);
	else
		ret = panel_io_bl_set(DISP_PANEL_MODULE, DISP_BL_OFF);

	return ret;
}

static int rt_slm5p5_hx8399c_panel_init(void)
{
	/* this memory not need to free  */
	disp_panel_ctl_t *pctl = rt_malloc(sizeof(disp_panel_ctl_t));

	pctl->ops.dev_config = slm5p5_hx8399c_dev_config;
	pctl->ops.dev_init = slm5p5_hx8399c_dev_init;
	pctl->ops.dev_exit = slm5p5_hx8399c_dev_exit;
	pctl->ops.dev_set_backlight_state = slm5p5_hx8399c_dev_set_backlight_state;
	pctl->ops.dev_set_backlight_value = NULL;

	pctl->width = WIDTH;
	pctl->height = HEIGHT;

	panel_status_init(DISP_PANEL_MODULE, pctl);

	if (panel_list_head != NULL)
		list_add_tail(&pctl->node, panel_list_head);
	else
		rt_kprintf("panel list head has not initiated\n");

	return 0;
}

INIT_APP_EXPORT(rt_slm5p5_hx8399c_panel_init);
