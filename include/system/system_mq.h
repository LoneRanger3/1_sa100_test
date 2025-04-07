/*
 * system_mq.h - header file for system message queue porting interface.
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
#ifndef _LB_SYSTEM_MQ_H_
#define _LB_SYSTEM_MQ_H_

#include <rtthread.h>

#define ASYNC_FLAG	0
#define SYNC_FLAG	1

#define     LB_SYSTEM_MSG_SIZE         64
#define     LB_SYSTEM_MSG_MAX_NUM      128

typedef enum _SYSTEM_MSG_TYPE_ {
	LB_SYSMSG_BEGIN = 0xF000,
	LB_SYSMSG_SD_PLUGIN,
	LB_SYSMSG_SD_PLUGOUT,
	LB_SYSMSG_SD_INIT_OK,
	LB_SYSMSG_SD_INIT_FAIL,
	LB_SYSMSG_SD_MOUNT_INV_VOL, /* sdcard not format by device */
	LB_SYSMSG_SD_LOW_PERF,
	LB_SYSMSG_USBD_PLUGIN,
	LB_SYSMSG_USBD_PLUGOUT,
	LB_SYSMSG_USBH_PLUGIN,
	LB_SYSMSG_USBH_PLUGOUT,
	LB_SYSMSG_FS_PART_MOUNT_OK,
	LB_SYSMSG_FS_PART_MOUNT_FAIL,
	LB_SYSMSG_FS_PART_UNMOUNT_PREPARE, /* app finish fs op and notify fs umount */
	LB_SYSMSG_FS_PART_UNMOUNT,
	LB_SYSMSG_STANDBY,
	LB_SYSMSG_RESUME,
	LB_SYSMSG_SCREEN_OPEN,
	LB_SYSMSG_SCREEN_CLOSE,
	LB_SYSMSG_SCREEN_LOCK,
	LB_SYSMSG_SCREEN_UNLOCK,
	LB_SYSMSG_POWER_OFF,
	LB_SYSMSG_POWER_LOW,
	LB_SYSMSG_USB_POWER_CONNECT,
	LB_SYSMSG_USB_POWER_DISCONNECT,
	LB_SYSMSG_KEY,
	LB_SYSMSG_AV_PLUGIN,
	LB_SYSMSG_AV_PLUGOUT,
	LB_SYSMSG_BACK_ON,
	LB_SYSMSG_BACK_OFF,
	LB_SYSMSG_RECORDER_FILE_FULL,
	LB_SYSMSG_LOCKFILE_FULL,
	LB_SYSMSG_GSENSOR,
	LB_SYSMSG_ALARM_COLLIDE,
	LB_SYSMSG_ALARM_PRESSURE,
	LB_SYSMSG_ALARM_LAUNCH,
	LB_SYSMSG_ALARM_DISTANCE,
	LB_SYSMSG_ALARM_BSD_LEFT,
	LB_SYSMSG_ALARM_BSD_RIGHT,
	LB_SYSMSG_ALARM_ACC_CHANGE,
	LB_SYSMSG_DIALOG,
	LB_SYSMSG_USERDEF,
#ifdef ARCH_LOMBO_N7V1_TDR
	LB_SYSMSG_BACK_ON_1,
	LB_SYSMSG_BACK_OFF_1,
	LB_SYSMSG_BACK_ON_2,
	LB_SYSMSG_BACK_OFF_2,
	LB_SYSMSG_BACK_ON_3,
	LB_SYSMSG_BACK_OFF_3,
	LB_SYSMSG_BACK_ON_4,
	LB_SYSMSG_BACK_OFF_4,
#endif
	LB_SYSMSG_DMS_PLUGIN,
	LB_SYSMSG_DMS_PLUGOUT,
	LB_SYSMSG_BACKDOOR,
	LB_SYSMSG_END,
} SYSTEM_MSG_TYPE;

typedef enum _SYSTEM_MSG_USERDEF_ {
	LB_USERDEF_SYSMSG_BEGIN = 0xFE00,
	LB_USERDEF_SYSMSG_ENTER_CDR,
	LB_USERDEF_SYSMSG_RETURN_HOME,
	LB_USERDEF_SYSMSG_POWER,
	LB_USERDEF_SYSMSG_PLAYER,
	LB_USERDEF_SYSMSG_MODE,
	LB_USERDEF_SYSMSG_MENU,
	LB_USERDEF_SYSMSG_ADD,
	LB_USERDEF_SYSMSG_SUB,
	LB_USERDEF_SYSMSG_CDR_SETTING,
	LB_USERDEF_SYSMSG_SYSTEM_SETTING,
	LB_USERDEF_SYSMSG_CDR,
	LB_USERDEF_SMART_DRIVE,
	LB_USERDEF_SYSMSG_STOP_REC,
} SYSTEM_MSG_USERDEF;

typedef struct tag_system_mq {
	rt_mq_t mq;
	rt_mq_t sync_mq;
	rt_sem_t sync_sem;
} lb_system_mq_t;

typedef struct tag_system_msg {
	int         type;
	int         len;
	char        data[LB_SYSTEM_MSG_SIZE-8];
} lb_system_msg_t;

int lb_system_mq_create(void);
int lb_system_mq_destroy(void);
int lb_system_mq_send(int msgtype, void *msg_data, int msg_len, int flag);
int lb_system_mq_recv(lb_system_msg_t *system_msg, int timeout_ms, int flag);
int lb_system_mq_syncsem_take(int time);
int lb_system_mq_syncsem_release(void);

#endif
