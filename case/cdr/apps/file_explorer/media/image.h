/*
 * image.h -  image code from file explorer
 * image code interface and macro define
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

#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "lb_types.h"
#include "lb_common.h"
#include <pthread.h>
#include "fileexp_cfg.h"

extern fxp_config_t fileexp_config;

lb_int32 image_mod_init(void *param);
lb_int32 image_mod_exit(void *param);
lb_int32 image_reg_init(void);
lb_int32 image_unreg_init(void);
lb_int32 image_reg_resp(void);
lb_int32 image_unreg_resp(void);
lb_int32 image_set(void *desert, lb_int32 index);

#endif /* __IMAGE_H__ */
