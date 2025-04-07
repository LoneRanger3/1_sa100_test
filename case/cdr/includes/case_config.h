/*
 * case_config.h - config file for case
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

#ifndef __CASE_CONFIG_H__
#define __CASE_CONFIG_H__

//#define MEDIA_TYPE_TS
/*#define SYS_MEMERY_128M_FOR_DOUBLE_1080P*/

#define FRONT_CAMERA_BUFFER_NUM 4
#define REAR_CAMERA_BUFFER_NUM 5

#define SCREEN_ROT_90
#define BLOCK_LINKER_DRAW
#define FRONT_PREVIEW_WIDTH 1920
#define FRONT_PREVIEW_HEIGHT 1080
#define FRONT_RECORDER_SOURCE_WIDTH 1920
#define FRONT_RECORDER_SOURCE_HEIGHT 1080
#define FRONT_RECORDER_SOURCE_FPS 25

#ifdef SYS_MEMERY_128M_FOR_DOUBLE_1080P
#define REAR_PREVIEW_WIDTH 1920
#define REAR_PREVIEW_HEIGHT 1080
#define REAR_RECORDER_SOURCE_WIDTH 1920
#define REAR_RECORDER_SOURCE_HEIGHT 1080
#define REAR_RECORDER_SOURCE_FPS 25
#else
#define REAR_PREVIEW_WIDTH 1920
#define REAR_PREVIEW_HEIGHT 1080
#define REAR_RECORDER_SOURCE_WIDTH 1920
#define REAR_RECORDER_SOURCE_HEIGHT 1080
#define REAR_RECORDER_SOURCE_FPS 25
#endif


#define LOCK_WAR_SIZE (4 * 1024)
#define REC_RESERVE_SIZE (900)
#define REC_LOWEST_FREE_SIZE (450)
//#define DEFAULT_FRONT_VIEW
#if 1
#define ADAS_CALIB_PREX 0
#define ADAS_CALIB_PREY 473
#define ADAS_CALIB_PREW 320
#define ADAS_CALIB_PREH 568
#else
#define ADAS_CALIB_PREX 0
#define ADAS_CALIB_PREY 410
#define ADAS_CALIB_PREW 440
#define ADAS_CALIB_PREH 781
#endif

#if 1
#define BSD_CALIB_PREX 0
#define BSD_CALIB_PREY 473
#define BSD_CALIB_PREW 320
#define BSD_CALIB_PREH 568
#else
#define BSD_CALIB_PREX 0
#define BSD_CALIB_PREY 410
#define BSD_CALIB_PREW 440
#define BSD_CALIB_PREH 781
#endif

#define REAR_BSD
 #define REAR_BSD_DEBUG 
#ifdef LOMBO_GPS
#define SYNC_TIME_FROM_GPS
#endif
 #define SET_ACC_SHUTDOWN_TIME 
/* #define USB_POWEROFF_DIALOG */
/* #define GPS_DEBUG */
/* #define PANO_DEBUG */
#define UPGRAGE_FIRMWARE

#endif /* __CASE_CONFIG_H__ */
