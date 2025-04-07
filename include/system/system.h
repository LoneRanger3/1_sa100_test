/*
 * system.h - case common head file
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

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

/* #define ARCH_LOMBO_N7_CDR_MMC */
#ifdef ARCH_LOMBO_N7_CDR_MMC
#define ROOTFS_MOUNT_PATH "/mnt/sdcard"
#define SDCARD_MOUNT_PATH "/mnt/sdcard"
#else
#define ROOTFS_MOUNT_PATH "/"
#define SDCARD_MOUNT_PATH "/mnt/sdcard"
#endif

#endif /* __SYSTEM_H__ */
