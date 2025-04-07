/*
 * rtc.h - head file for RTC module
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
#ifndef __LOMBO_RTC_H__
#define __LOMBO_RTC_H__

#define ENABLE		1
#define DISABLE		0

/* RTC CLK source */
#define CLK_SRC_LFEOSC	1
#define CLK_SRC_RCOSC	0

/* External function interface */
void lombo_rtc_lfeosc_fanout(u32 lfeosc_out_en);
void lombo_rtc_clk_src_set(u32 clk_src);
u32 lombo_rtc_get_clk_src_stat(void);


#endif
