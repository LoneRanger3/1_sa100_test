/*
 * pm.h - head file for pm module
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

#include <rtthread.h>
#include <debug.h>
#include <rthw.h>
#include <csp.h>
#include <memory.h> /* for STANDBY_DDR_PARA/SIZE, STANDBY_DDR_TIMING/SIZE */
#include <drivers/pm.h>

#include "../drivers/board.h"

extern void *suspend_start;
extern void *suspend_end;

/* standby entry in sram */
typedef void (*sram_standby_fn)(int mode, int ddr_para, int ddr_tm);

/* deepslp->suspend entry in sram */
typedef void (*sram_deepslp_fn)(int mode, int ddr_para, int ddr_tm);

/*
 * deepslp->suspend entry in eos kernel(dram)
 *
 * at the end of which, the deepslp->suspend entry in sram will be called
 */
extern void lombo_cpu_deepslp(int mode, int ddr_para, int ddr_tm, sram_deepslp_fn fn);

/*
 * deepslp->resume entry in eos kernel(dram)
 *
 * after ddr exit sr, the leaver(boot procedure) will jump to this entry
 */
extern void lombo_deepslp_resume(void);

