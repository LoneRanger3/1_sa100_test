/*
 * disp_test.c - Disp test module driver code for LomboTech
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
#include "disp_test.h"

enum {
	BACKLIGHT,
	BACKLIGHT_PWM,

	DC_BACKCOLOR,
	DC_COLOR_ENHANCEMENT,
	DC_BKL_SHOW_HIDE,
	DC_HWC_SHOW_HIDE,
	DC_BKL_ALPHA,

	WIN_COLOR_KEY,
	WIN_ALPHA,
	WIN_SHOW_HIDE,

	DISP_IOCTL_MAX_NUM
};

typedef struct tag_disp_ioctl_test_module {
	char	name[32];	/* name of the module */
	bool	enable;		/* enable test module */
} disp_ioctl_test_module_t;

static disp_ioctl_test_module_t ditm[DISP_IOCTL_MAX_NUM] = {
	{"backlight_test",		false},
	{"backlight_pwm_test",		false},

	{"dc_backcolor_test",		false},
	{"dc_color_enhancement_test",	false},
	{"dc_blocklinker_test",		false},
	{"dc_hwc_test",			false},
	{"dc_blocklinker_alpha_test",	false},

	{"win_color_key_test",		false},
	{"win_alpha_test",		false},
	{"win_show_hide_test",		false},
};

#if BACKLIGHT_TEST
static void backlight_test(void)
{
	static u8 on_flag;

	LOG_I("backlight_test: sta[%d]", on_flag);

	if (on_flag)
		rt_device_control(disp_device, DISP_CMD_SET_BACKLIGHT_ON, NULL);
	else
		rt_device_control(disp_device, DISP_CMD_SET_BACKLIGHT_OFF, NULL);

	on_flag = !on_flag;
}
#endif

#if BACKLIGHT_PWM_TEST
static void backlight_pwm_test(void)
{
	static u32 value;
	disp_io_ctrl_t dic;

	LOG_I("backlight_pwm_test: value[%d]", value);
	rt_memset(&dic, 0x00, sizeof(disp_io_ctrl_t));
	dic.args = &value;
	rt_device_control(disp_device, DISP_CMD_SET_BACKLIGHT_VALUE, &dic);
	value = value + 10;
	if (value >= 100)
		value = 0;
}

void backlight_pwm_set_test(u32 value)
{
	disp_io_ctrl_t dic;

	LOG_I("backlight_pwm_set_test: value[%d]", value);
	if (value >= 100)
		value = 100;
	rt_memset(&dic, 0x00, sizeof(disp_io_ctrl_t));
	dic.args = &value;
	rt_device_control(disp_device, DISP_CMD_SET_BACKLIGHT_VALUE, &dic);
}

#endif

#if DC_BACKCOLOR_TEST
static void dc_backcolor_test(void)
{
	static u8 color;
	u32 bk_color[3];
	disp_io_ctrl_t dic;

	LOG_I("dc_backcolor_test");
	rt_memset(&dic, 0x00, sizeof(disp_io_ctrl_t));
	bk_color[0] = 0xffff0000;
	bk_color[1] = 0xff00ff00;
	bk_color[2] = 0xff0000ff;

	dic.dc_index = 0;
	dic.args = &bk_color[color % 3];
	rt_device_control(disp_device, DISP_CMD_SET_DC_BACKCOLOR, &dic);
	color++;
}

void backcolor_set_test(u32 bk_color)
{
	disp_io_ctrl_t dic;

	LOG_I("dc_backcolor_test");
	rt_memset(&dic, 0x00, sizeof(disp_io_ctrl_t));

	dic.dc_index = 0;
	dic.args = &bk_color;
	rt_device_control(disp_device, DISP_CMD_SET_DC_BACKCOLOR, &dic);
}
#endif

#if DC_COLOR_ENHANCE_ENABLE
static void dc_color_enhancement_test(void)
{
	static u32 ce_cnt;
	static u32 brightness;
	static u32 saturation;
	static u32 contrast;
	disp_io_ctrl_t dic;

	LOG_I("dc_color_enhancement_test");
	rt_memset(&dic, 0x00, sizeof(disp_io_ctrl_t));

	rt_kprintf("ce cnt[%d]\n", ce_cnt);

	dic.dc_index = 0;
	if (ce_cnt == 0) {
		rt_device_control(disp_device,
				DISP_CMD_ENABLE_DC_COLOR_ENHANCEMENT, &dic);
	}
	if (ce_cnt <= 10) {
		brightness = ce_cnt * 10;
		dic.args = &brightness;
		rt_device_control(disp_device, DISP_CMD_SET_DC_BRIGHTNESS, &dic);
	} else if (ce_cnt <= 20) {
		saturation = (ce_cnt - 10) * 10;
		brightness = 50; /* set to default value */
		dic.args = &brightness;
		rt_device_control(disp_device, DISP_CMD_SET_DC_BRIGHTNESS, &dic);

		dic.args = &saturation;
		rt_device_control(disp_device, DISP_CMD_SET_DC_SATURATION, &dic);
	} else if (ce_cnt <= 30) {
		contrast = (ce_cnt - 20) * 10;
		saturation = 50; /* set to default value */
		dic.args = &saturation;
		rt_device_control(disp_device, DISP_CMD_SET_DC_SATURATION, &dic);

		dic.args = &contrast;
		rt_device_control(disp_device, DISP_CMD_SET_DC_CONTRAST, &dic);
	}

	ce_cnt++;
	if (ce_cnt >= 35) {
		ce_cnt = 0;
		brightness = 0;
		saturation = 0;
		contrast = 0;
	}
}
#endif

#if DC_BKL_SHOW_HIDE_ENABLE
static void dc_blocklinker_test(void)
{
	static bool show_enable = true;
	disp_io_ctrl_t dic;

	LOG_I("dc_blocklinker_test: sta[%d]", show_enable);
	rt_memset(&dic, 0x00, sizeof(disp_io_ctrl_t));
	dic.dc_index = 0;
	if (show_enable)
		rt_device_control(disp_device, DISP_CMD_ENABLE_DC_BLOCKLINKER, &dic);
	else
		rt_device_control(disp_device, DISP_CMD_DISABLE_DC_BLOCKLINKER, &dic);

	show_enable = !show_enable;
}
#endif

#if DC_HWC_SHOW_HIDE_ENABLE
static void dc_hwc_test(void)
{
	static bool show_enable = true;
	disp_io_ctrl_t dic;

	LOG_I("dc_hwc_test: sta[%d]", show_enable);
	rt_memset(&dic, 0x00, sizeof(disp_io_ctrl_t));

	dic.dc_index = 0;
	if (show_enable)
		rt_device_control(disp_device, DISP_CMD_ENABLE_DC_HWC, &dic);
	else
		rt_device_control(disp_device, DISP_CMD_DISABLE_DC_HWC, &dic);

	show_enable = !show_enable;
}
#endif

#if DC_BKL_ALPHA_TEST
static void dc_blocklinker_alpha_test(void)
{
	static u8 value = 255;
	dc_alpha_mode_t mode = DC_PLANE_ALPHA;
	disp_io_ctrl_t dic;

	LOG_I("dc_blocklinker_alpha_test: mode[%d] value[%d]", mode, value);
	rt_memset(&dic, 0x00, sizeof(disp_io_ctrl_t));
	dic.dc_index = 0;
	dic.args = &mode;
	rt_device_control(disp_device, DISP_CMD_SET_DC_BLOCKLINKER_ALPHA_MODE, &dic);

	dic.args = &value;
	rt_device_control(disp_device, DISP_CMD_SET_DC_BLOCKLINKER_ALPHA_VALUE, &dic);

	if (value == 250)
		value = 255;
	else if (value == 255)
		value = 0;
	else
		value += 25;
}
#endif

#if WIN_COLOR_KEY_TEST
static void win_color_key_test(void)
{
	dc_colorkey_t ck;
	disp_io_ctrl_t dic;

	LOG_I("win_color_key_test");
	rt_memset(&dic, 0x00, sizeof(disp_io_ctrl_t));

	ck.target     = DC_COLORKEY_TARGET_THIS;
	ck.red_rule   = DC_COLORKEY_INSIDE_RANGE_MATCH;
	ck.green_rule = DC_COLORKEY_INSIDE_RANGE_MATCH;
	ck.blue_rule  = DC_COLORKEY_INSIDE_RANGE_MATCH;
	/* sys_bar.bmp  threshold 0x939aca */
	ck.min.r      = 0x93;
	ck.min.g      = 0x93;
	ck.min.b      = 0x93;
	ck.max.r      = 0xff;
	ck.max.g      = 0xff;
	ck.max.b      = 0xff;

	dic.dc_index = 0;
	dic.dctl = wctl[1];
	dic.args = &ck;
	rt_device_control(disp_device, DISP_CMD_ENABLE_DC_WIN_COLOR_KEY, &dic);
	rt_device_control(disp_device, DISP_CMD_SET_DC_WIN_COLOR_KEY, &dic);
}
#endif

#if WIN_ALPHA_TEST
static void win_alpha_test()
{
	static u8 value = 255;
	dc_alpha_mode_t mode = DC_PLANE_ALPHA;
	disp_io_ctrl_t dic;

	LOG_I("win_alpha_test: mode[%d] value[%d]", mode, value);
	rt_memset(&dic, 0x00, sizeof(disp_io_ctrl_t));

	dic.dc_index = 0;
	dic.dctl = wctl[1];
	dic.args = &mode;
	rt_device_control(disp_device, DISP_CMD_SET_DC_WIN_ALPHA_MODE, &dic);

	dic.args = &value;
	rt_device_control(disp_device, DISP_CMD_SET_DC_WIN_ALPHA_VALUE, &dic);

	if (value == 250)
		value = 255;
	else if (value == 255)
		value = 0;
	else
		value += 25;
}
#endif

#if WIN_SHOW_HIDE_TEST
static void win_show_hide_test(void)
{
	static bool show_enable = true;
	disp_io_ctrl_t dic;

	LOG_I("win_show_hide_test: sta[%d]", show_enable);
	rt_memset(&dic, 0x00, sizeof(disp_io_ctrl_t));
	dic.dc_index = 0;
	dic.dctl = wctl[2];

	if (show_enable)
		rt_device_control(disp_device, DISP_CMD_ENABLE_DC_WIN, &dic);
	else
		rt_device_control(disp_device, DISP_CMD_DISABLE_DC_WIN, &dic);

	show_enable = !show_enable;
}
#endif

int disp_ioctl_test(u32 cnt)
{
#if BACKLIGHT_TEST
	if (ditm[BACKLIGHT].enable) {
		if (cnt % 3 == 0)
			backlight_test();
	}
#endif
#if BACKLIGHT_PWM_TEST
	if (ditm[BACKLIGHT_PWM].enable) {
		if (cnt % 3 == 0)
			backlight_pwm_test();
	}
#endif

#if DC_BACKCOLOR_TEST
	if (ditm[DC_BACKCOLOR].enable)
		dc_backcolor_test();
#endif
#if DC_COLOR_ENHANCE_ENABLE
	if (ditm[DC_COLOR_ENHANCEMENT].enable)
		dc_color_enhancement_test();
#endif
#if DC_BKL_SHOW_HIDE_ENABLE
	if (ditm[DC_BKL_SHOW_HIDE].enable)
		dc_blocklinker_test();
#endif
#if DC_HWC_SHOW_HIDE_ENABLE
	if (ditm[DC_HWC_SHOW_HIDE].enable)
		dc_hwc_test();
#endif
#if DC_BKL_ALPHA_TEST
	if (ditm[DC_BKL_ALPHA].enable)
		dc_blocklinker_alpha_test();
#endif

#if WIN_COLOR_KEY_TEST
	if (ditm[WIN_COLOR_KEY].enable) {
		if (cnt == 6)
			win_color_key_test();
	}
#endif
#if WIN_ALPHA_TEST
	if (ditm[WIN_ALPHA].enable)
		win_alpha_test();
#endif
#if WIN_SHOW_HIDE_TEST
	if (ditm[WIN_SHOW_HIDE].enable)
		win_show_hide_test();
#endif

	return DISP_OK;
}

int disp_ioctl_cfg(char *cmd)
{
	int i;

	if (cmd != NULL) {
		for (i = 0; i < DISP_IOCTL_MAX_NUM; i++) {
			if (strcmp(cmd, ditm[i].name) == DISP_OK) {
				ditm[i].enable = !ditm[i].enable;
				LOG_I("disp ioctl cmd[%s] state[%s]", ditm[i].name,
					ditm[i].enable == true ? "enable" : "disable");
				return DISP_OK;
			}
		}
	}

	LOG_E("argv should be follow:");
	for (i = 0; i < DISP_IOCTL_MAX_NUM; i++)
		LOG_I("ioctl cmd%d:%s", i, ditm[i].name);

	return DISP_ERROR;
}
