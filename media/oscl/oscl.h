/*
 * oscl.h - common lib api used by lombo media player.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef __LB_OSCL_H__
#define __LB_OSCL_H__

#include "oscl_debug.h"
#include "pthread.h"
#include "oscl_queue.h"
#include "oscl_mem.h"
#include "semaphore.h"
#include <rtthread.h>
#include "soc_define.h"
#include "oscl_cache_file.h"
#include "oscl_video_frame.h"
#include "oscl_semaphore.h"



#define LB_FILE_SYSTEM_PRIO 16
#define LB_RECORDER_SINK_PRIO 15
#define LB_RECORDER_MUXER_PRIO 16
#define LB_RECORDER_AUDIO_PRIO 17
#define LB_RECORDER_VSRC_PRIO 8
#define LB_RECORDER_VSPLIT_PRIO 8
#define LB_RECORDER_VIDEO_PRIO 8
#define LB_FILE_CLOSE_PRIO 24



#ifdef __QEMU__
#include "qemu.h"
#endif
#include "board.h"

#define oscl_mdelay(m) rt_thread_mdelay(m)
#define oscl_get_msec()	rt_time_get_msec()
#define oscl_get_usec() rt_time_get_usec()


#endif /* __LB_OSCL_H__ */
