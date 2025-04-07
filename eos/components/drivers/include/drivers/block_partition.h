/*
 * block_partition.h - Generic definitions for block device partition
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

#ifndef _LOMBO_BLOCK_PARTITION_H_
#define _LOMBO_BLOCK_PARTITION_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <rtthread.h>

/*
 * functions
 */
rt_err_t rt_block_partition_register(rt_device_t master_bdev);

#ifdef __cplusplus
}
#endif

#endif /* _LOMBO_BLOCK_PARTITION_H_ */
