/*
 * lombo_doss.h - Lombo doss module head file
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

#ifndef __LOMBO_DOSS_H__
#define __LOMBO_DOSS_H__
#include <spinlock.h>
#include "csp_doss_top.h"
#include "csp_doss_tcon.h"
#include "csp_doss_mipi_dsi.h"
#include "disp_cjson.h"
/* #include "csp_doss_tve.h" */

typedef enum vo_dev_if {
	VO_DEV_PRGB			= 0,
	VO_DEV_SRGB_RGB			= 1,
	VO_DEV_SRGB_DUMMY_RGB		= 2,
	VO_DEV_SRGB_RGB_DUMMY		= 3,
	VO_DEV_CPU_18BIT_RGB666		= 4,
	VO_DEV_CPU_16BIT_RGB888		= 5,
	VO_DEV_CPU_16BIT_RGB666		= 6,
	VO_DEV_CPU_16BIT_RGB565		= 7,
	VO_DEV_CPU_9BIT_RGB666		= 8,
	VO_DEV_CPU_8BIT_RGB666		= 9,
	VO_DEV_CPU_8BIT_RGB565		= 10,
	VO_DEV_CPU_8BIT_RGB888		= 11,
	VO_DEV_LVDS			= 12,
	VO_DEV_MIPI_DSI_VIDEO		= 13,
	VO_DEV_MIPI_DSI_CMD		= 14,
	VO_DEV_BT601_24BIT		= 15,
	VO_DEV_BT601_16BIT		= 16,
	VO_DEV_BT601_8BIT		= 17,
	VO_DEV_BT656_8BIT		= 18,
	VO_DEV_BT1120_16BIT		= 19,
	VO_DEV_BT1120_8BIT		= 20,
	VO_DEV_TVE			= 21,
} vo_dev_if_t;

typedef struct tcon_if_remap {
	vo_dev_if_t		dev_if;
	tcon_out_if_t		tcon_out_if;
} tcon_if_remap_t;

typedef struct vo_if_cfg {
	tcon_rgb_if_t		rgb_if;
	tcon_cpu_if_t		cpu_if;
	tcon_lvds_if_t		lvds_if;
	tcon_mipi_dsi_if_t	dsi_if;
#ifdef SUPPORT_TVE_MODULE
	tcon_tve_if_t		tve_if;
#endif
} vo_if_cfg_t;

typedef struct vo_device vo_device_t;

typedef struct vo_dev_ops {
	void (*dev_init) (vo_device_t *vo_dev);
	void (*dev_exit) (vo_device_t *vo_dev);
	int (*dev_set_backlight_state)(bool state);
	int (*dev_set_backlight_value)(u32 value);
} vo_dev_ops_t;

typedef struct dpu_cfg {
	u32			dc_index;
	bool			dc_enable;
	bool			se0_enable;
	bool			se1_enable;
} dpu_cfg_t;

struct vo_device {
	tcon_host_t		*tcon_host;
	vo_dev_if_t		dev_if;
	vo_if_cfg_t		if_cfg;
	vo_dev_ops_t		dev_ops;
};

typedef struct tag_tcon_info {
	u32 hact;
	u32 vact;
	u32 dclk; /* unit hz */
	u32 period; /* time of update a frame, unit ns */
	u32 sw_safe_line; /* safe line when sw start update dbr */
	u32 hw_sync_line; /* hardware actually sync line */
	u32 vb_line_num; /* vblank interupt line */
	u32 vi_line_num; /* vline interupt line */
	u32 ns_per_ht;
	u32 dbld_skip;
	u32 dbld_over_syncdly;
	u32 lack_data_cnt;
	u32 irq_cnt;
} tcon_info_t;

typedef struct tag_doss_tcon {
	u32			tcon_index;
	vo_device_t		*vo_dev;
	tcon_info_t		tcon_info;
	dpu_cfg_t		dpu_config;
} doss_tcon_t;

enum tcon_version {
	TCON_VER_0_1,
	TCON_VER_0_2,
};

typedef enum doss_tcon_index {
	DOSS_TCON_INDEX0		= 0,
	DOSS_TCON_INDEX1		= 1
} doss_tcon_index_t;

/**
 * @power_gpio: GPIO for tcon lcd screen power pin
 * @tcon_index: Index for tcon controller
 * @doss_index: Index for doss top controller
 *
 * NULL pointer for *_info fields indicates that
 * the corresponding chip is not present
 */
struct lombo_tcon_platform_data {
	s32 power_gpio;
	u32 tcon_index;
	u32 doss_index;
};

extern spinlock_t disp_spin_lock;
extern spinlock_t disp_cjson_spin_lock;
extern disp_update_gamma_sta_t disp_updating_gamma_sta;

doss_tcon_t *get_doss_tcon(void);
int tcon_host_resource_init(vo_device_t *vo_dev);
int tcon_host_resource_exit(vo_device_t *vo_dev);
int tcon_host_init(vo_device_t *vo_dev);
void tcon_host_exit(vo_device_t *vo_dev);
int tcon_dev_init(vo_device_t *vo_dev);
int tcon_dev_exit(vo_device_t *vo_dev);
void host_start(vo_device_t *vo_dev);
void host_stop(vo_device_t *vo_dev);

int tcon_dev_probe(vo_device_t **vo_dev);
void tcon_dev_disprobe(void);
void tcon_get_dev(vo_device_t **vo_dev);
int tcon_set_dev_backlight_state(bool state);
int tcon_set_dev_backlight_value(u32 value);
int tcon_get_dev_size(u32 *width, u32 *height);
const char *tcon_get_module_name(void);

extern int se_update_dbr(unsigned int se_index);
extern int dc_update_dbr(unsigned int dc_index);
#endif /* __LOMBO_DOSS_H__ */

