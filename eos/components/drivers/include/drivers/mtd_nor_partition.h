/*
 * mtd_nor_partition.h - Generic definitions for LomboTech MTD NOR partition
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

#ifndef _LOMBO_MTD_NOR_PARTITION_H_
#define _LOMBO_MTD_NOR_PARTITION_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <drivers/mtd_nor.h>

struct rt_mtd_nor_partition {
	union {
		struct rt_mtd_nor_device mtd;
		struct rt_device blk;
	};

	const char *name;
	rt_uint32_t offset;	/* offset within the master MTD space */
	rt_uint32_t size;	/* partition size */
	rt_uint32_t mask_flags;
	void *user_data;	/* hold parent device */
};

/*
 * functions
 */
rt_err_t rt_mtd_nor_register_partition(struct rt_mtd_nor_device *master_mtdnor);

#ifdef __cplusplus
}
#endif

#endif /* _LOMBO_MTD_NOR_PARTITION_H_ */
