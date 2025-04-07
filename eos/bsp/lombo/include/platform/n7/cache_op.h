/*
 * cache_op.h - Cache operations(extra) head file for n7
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

#ifndef __CACHE_OP_H
#define __CACHE_OP_H

int n7_is_dcache_en(void);
int n7_is_icache_en(void);

void n7_flush_dcache_all(void); /* clean & invalidate all dcache */
void n7_inv_dcache_all(void); /* invalidate all dcache */
void n7_inv_icache_all(void);
void n7_inv_icache_range(void *start, int size);
void n7_inv_tlb(void);

void n7_clean_dcache_range(void *start, void *end);
void n7_inv_dcache_range(void *start, void *end);
void n7_flush_dcache_range(void *start, void *end);

int n7_is_mmu_en(void);

void n7_join_smp(void);
void n7_leave_smp(void);
void n7_inv_branch_pred_cache(void);
void n7_branch_prediction_enable(void);
void n7_set_ttbcr(int val);

void n7_do_idle(void);

#endif /* __CACHE_OP_H */
