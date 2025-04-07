/*
 * media_player_ctrl.c - media player widget reg & unreg head file
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

#ifndef __VIDEO_H__
#define __VIDEO_H__

#include "lb_types.h"
#include "lb_common.h"
#include <pthread.h>
#include "fileexp_cfg.h"

#define EXTERNEL_PRIO 28
#define EXTERNEL_SIZE 4096
#define INTERNEL_PRIO 28
#define INTERNEL_SIZE 4096
#define ERROR_PRIO 28
#define ERROR_SIZE 4096


lb_int32 video_mod_init(void *param);
lb_int32 video_mod_exit(void *param);
lb_int32 video_reg_init(void);
lb_int32 video_unreg_init(void);
lb_int32 video_reg_resp(void);
lb_int32 video_unreg_resp(void);
lb_int32 video_set(void *desert, lb_int32 index);

typedef struct tag_video_time {
	lb_int32 hour;
	lb_int32 minute;
	lb_int32 second;
} video_time_t;

#endif /* __VIDEO_H__ */
