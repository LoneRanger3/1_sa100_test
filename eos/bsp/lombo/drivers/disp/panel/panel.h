/*
 * panel.h - Panel module head file
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

#ifndef __PANEL_H__
#define __PANEL_H__
#include <pinctrl.h>
#include <list.h>
#include <cfg/config_api.h>
#include "lombo_doss.h"

/* lvds or prgb 666 or hight 6 bits for prgb 888 */
#define DISP_GPIO_PIN_GROUP	"pin-grp"
/* extra low 2 bits for prgb 888 */
#define DISP_GPIO_PIN_GROUP_E	"pin-grp-e"

#define DISP_RESET_GROUP	"rst-grp"
#define DISP_BL_GROUP		"bl-grp"
#define DISP_RGLT_GROUP		"rglt-grp"
#define DISP_VREF_GROUP		"vref-grp"

#define DISP_GPIO_BL_PWM_GROUP	"pwm-bl-gpio"
#define DISP_BL_PWM_GROUP	"pwm-bl-config"
#define DISP_BL_PWM_RANGE	"pwm-bl-range"

#define DISP_I2C_BUS		"i2c-bus"
#define DISP_I2C_REG		"i2c-reg"

#define PANEL_STATUS		"status" /*okay or disable*/

#define IO_HIGH			1
#define IO_LOW			0

struct list_head *panel_list_head;

typedef struct disp_panle_ops {
	void (*dev_config)(vo_device_t *vo_dev);
	void (*dev_init)(vo_device_t *vo_dev);
	void (*dev_exit)(vo_device_t *vo_dev);
	int (*dev_set_backlight_state)(bool state);
	int (*dev_set_backlight_value)(u32 value);
} disp_panle_ops_t;

typedef struct disp_panel_ctl {
	struct list_head node;
	disp_panle_ops_t ops;
	u32 width;
	u32 height;
	bool is_okay;
	const char *name;
} disp_panel_ctl_t;

int panel_rst_io_init(const char *module_name, const char *group_name);
int panel_rst_io_exit(const char *module_name, const char *group_name);
int panel_bl_io_init(const char *module_name, const char *group_name);
int panel_bl_io_exit(const char *module_name, const char *group_name);
int panel_rglt_io_init(const char *module_name, const char *group_name);
int panel_rglt_io_exit(const char *module_name, const char *group_name);
int panel_vref_io_init(const char *module_name, const char *group_name);
int panel_vref_io_exit(const char *module_name, const char *group_name);
int panel_io_rst_set(const char *module_name, u32 value);
int panel_io_bl_set(const char *module_name, u32 value);
int panel_io_rglt_set(const char *module_name, u32 value);
int panel_io_vref_set(const char *module_name, u32 value);

void panel_pwm_init(const char *module_name);
void panel_pwm_exit(const char *module_name);
int panel_set_backlight_value(const char *module_name, u32 value);
int panel_set_backlight_state(bool state);

s32 mipi_dsi_dcs_write_array(u32 index, u8 vc, u8 *reg, u32 len, u8 delay_flag);

int panel_find_valid(disp_panel_ctl_t **pctl_p);
int panel_dev_probe(vo_device_t **vo_dev);
void panel_dev_disprobe(void);
void panel_get_dev(vo_device_t **vo_dev);
int panel_get_size(u32 *width, u32 *height);
const char *panel_get_module_name(void);

void panel_status_init(const char *module_name, disp_panel_ctl_t *pctl);
#endif

