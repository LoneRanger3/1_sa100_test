/*
 * boot_param.h - boot param definations
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

#ifndef __BOOT_PARAM_H__
#define __BOOT_PARAM_H__

#ifdef LOMBO_SPI_NOR
#include "spinor_drv.h"
#endif

#define BP_MAGIC         (0x20504220) /* " BP " */

struct boot_param_head {
	u32 magic;                /* MAGIC " BP " */
	u32 size;                 /* sizeof(struct boot_param_head) */
	u32 boot_param_len;       /* boot_param total size */
	u32 boot_info_offset;     /* boot_info offset */
	u16 dfs_mount_tbl_cnt;    /* boot_dfs_mount_tbl count */
	u16 dfs_mount_tbl_offset; /* boot_dfs_mount_tbl offset */
	u16 sys_partition_cnt;    /* boot_sys_partition count */
	u16 sys_partition_offset; /* boot_sys_partition offset */
	u32 rapid_boot;           /* rapid boot flag: 1 - fast boot, 0 - normal boot */
#ifdef ARCH_LOMBO_N7V1
	u32 dma_status;           /* dma status: 1 - dma fail, 0 - dma okay */
	char ver0[8];             /* booster version0 */
	char ver1[4];             /* booster version1 */
	u32 res[5];               /* reserved */
#else
	char ver0[8];             /* booster version0 */
	char ver1[4];             /* booster version1 */
	u32 res[6];               /* reserved */
#endif
};

struct boot_info {
	u32 dram_size;            /* dram size */
	char boot_type[8];        /* boot type, maybe spinor/spinand/mmc/sdcard/nand */
};

struct boot_dfs_mount_tbl {
	char dev_name[16];        /* device which to mount */
	char path[4];             /* path which to mount */
	char fs_type[16];         /* file system which to mount */
	u32 rwflag;               /* read or write flag when mount */
	char data[4];             /* param when mount */
	u32 type;                 /* device type, 0: interal, !0: external */
};

struct boot_sys_partition {
	char name[16];            /* partition name */
	u32 offset;               /* partition offset */
	u32 size;                 /* partition size */
	u32 mask_flags;           /* mask flags */
};

u32 boot_get_dram_size(void);
char *boot_get_boot_type(void);
void boot_param_relocate(void);
void boot_para_dump_all(void);
u32 boot_get_boot_flag(void);
#ifdef ARCH_LOMBO_N7V1
u32 boot_get_dma_status(void);
#endif
#ifdef LOMBO_SPI_NOR
void boot_update_boot_para(struct lombo_nor *flash);
#endif
void boot_print_booster_version(void);

struct boot_dfs_mount_tbl *_boot_para_get_rootfs(void);
struct boot_sys_partition *_boot_para_get_syspartition(void);
int _boot_para_get_partition_cnt(void);


#endif /* __BOOT_PARAM_H__ */
