/*
 * lombo ufi define
 *
 * Copyright(c)2018 Lombo Tech Co.Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef LB_UFI_H
#define LB_UFI_H

/* max trans size */
#define MAX_TRANS_LEN	(64 * 1024)

/* lombo ufi cmd define */
#define UFI_NONE_CMD	0x0
/* get device status */
#define UFI_DEV_CMD	0xF0
/* get device information */
#define UFI_VER_CMD	0xF1
/* get device software information, json struct */
#define UFI_SWI_CMD	0xF2
/* get device hardware information, jsion struct */
#define UFI_HWI_CMD	0xF4
/* jump to img update mode */
#define UFI_UPD_CMD	0xF8
/* register read or write */
#define UFI_REG_CMD	0xF5
/* memory read or write */
#define UFI_MEM_CMD	0xF6
/* run software */
#define UFI_RUN_CMD	0xF7

/* UFI_DEV_CMD response data struct */
struct data_get_status {
	u32 status;		/* status */
	u32 interval;		/* next query time (ms) */
	u8  res[8];		/* res */
} __packed;

/* UFI_DEV_CMD value */
#define LB_STATUS_NULL		0x00
#define LB_STATUS_READY		0x01
#define LB_STATUS_BUSY		0x02
#define LB_STATUS_ERROR		0x03

/* UFI_VER_CMD response data struct */
struct data_get_ver_info {
	u32 ver;		/* version */
	u32 sw_id;		/* software id */
	u32 hw_id;		/* hardware id */
	u8  res[20];		/* res */
} __packed;

/* UFI_SWI_CMD response data struct */
struct data_get_swinfo {
	u8 data[4096];
} __packed;

/* UFI_HWI_CMD response data struct */
struct data_get_hwinfo {
	u8 data[4096];
} __packed;

/* UFI_UPD_CMD img update command struct */
struct cmd_pkg_update {
	u8 cmd;			/* command 0xf8 */
	u8 flag;
	u32 param0;
	u32 param1;
	u8 res[6];
} __packed;

/* UFI_REG_CMD read write register command struct */
struct cmd_reg_xfer {
	u8 cmd;			/* command 0xf5 */
	u8 flag;		/* modules */
	u32 addr;		/* address */
	u32 len;                /* len */
	u8 res[6];
} __packed;

/* UFI_REG_CMD response data struct */
struct data_reg_xfer {
	u32 addr;		/* phy reg address */
	u32 val;		/* reg value */
} __packed;

/* UFI_MEM_CMD read write command struct */
struct cmd_mem_xfer {
	u8 cmd;			/* command 0xf6 */
	u8 flag;		/* modules */
	u32 addr;		/* address */
	u32 len;		/* len */
	u8 res[6];
} __packed;

/* UFI_RUN_CMD run software command struct */
struct cmd_prg_run {
	u8 cmd;			/* command 0xf7 */
	u8 flag;
	u32 param0;
	u32 param1;
	u8 res[6];
} __packed;

extern int isp_usb_write(void *cmd, void *data);
extern int isp_usb_read(void *cmd, void *data);

#endif /* #ifdef LB_UFI_H */
