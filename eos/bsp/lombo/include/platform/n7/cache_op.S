/*
 * cache_op.S - Cache operations(extra) for n7
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

#include <linkage.h>

/* ------------------ the macros merge from kernel ------------------ */
	.macro	disable_irq_notrace
	cpsid	i
	.endm

	.macro	save_and_disable_irqs_notrace, oldcpsr
	mrs	\oldcpsr, cpsr
	disable_irq_notrace
	.endm

	.macro	restore_irqs_notrace, oldcpsr
	msr	cpsr_c, \oldcpsr
	.endm

	/* get the minimum D-cache line size from the CTR register on ARMv7 */
	.macro	dcache_line_size, reg, tmp
	mrc	p15, 0, \tmp, c0, c0, 1		@ read ctr
	lsr	\tmp, \tmp, #16
	and	\tmp, \tmp, #0xf		@ cache line size encoding
	mov	\reg, #4			@ bytes per word
	mov	\reg, \reg, lsl \tmp		@ actual cache line size
	.endm

	/* ALT_SMP, ALT_UP and ALT_UP_B */
#ifdef RT_USING_SMP
#define ALT_SMP(instr...)					\
9998:	instr
#define ALT_UP(instr...)					\
	.pushsection ".alt.smp.init", "a"			;\
	.long	9998b						;\
9997:	instr							;\
	.if . - 9997b != 4					;\
		.error "ALT_UP() content must assemble to exactly 4 bytes";\
	.endif							;\
	.popsection
#define ALT_UP_B(label)						\
	.equ	up_b_offset, label - 9998b			;\
	.pushsection ".alt.smp.init", "a"			;\
	.long	9998b						;\
	W(b)	. + up_b_offset					;\
	.popsection
#else
#define ALT_SMP(instr...)
#define ALT_UP(instr...) instr
#define ALT_UP_B(label) b label
#endif
/* ------------------ the macros merge from kernel ------------------ */

/**
 * n7_inv_icache_all - Invalidate the whole I-cache.
 *
 * void n7_inv_icache_all(void);
 */
.globl n7_inv_icache_all
n7_inv_icache_all:
	stmfd	sp!, {r0, lr}
	mov	r0, #0
	ALT_SMP(mcr	p15, 0, r0, c7, c1, 0)	@ inv I-cache inner shareable
	ALT_UP(mcr	p15, 0, r0, c7, c5, 0)	@ I+BTB cache invalidate
	ldmfd	sp!, {r0, pc}
ENDPROC(n7_inv_icache_all)

/**
 * n7_flush_dcache_all - Flush(clean and invalidate) the whole D-cache.
 *
 * void n7_flush_dcache_all(void);
 *
 * Corrupted registers: r0-r7, r9-r11 (r6 only in Thumb mode)
 */
.globl n7_flush_dcache_all
n7_flush_dcache_all:
	stmfd	sp!, {r0 - r7, r9 - r11, lr}
	dmb				@ ensure ordering with prev mem access
	mrc	p15, 1, r0, c0, c0, 1	@ read clidr
	ands	r3, r0, #0x7000000	@ extract loc from clidr
	mov	r3, r3, lsr #23		@ left align loc bit field
	beq	flush_finished		@ if loc is 0, then no need to clean
	mov	r10, #0			@ start clean at cache level 0
flush_levels:
	add	r2, r10, r10, lsr #1	@ work out 3x current cache level
	mov	r1, r0, lsr r2		@ extract cache type bits from clidr
	and	r1, r1, #7		@ mask of the bits for cur cache only
	cmp	r1, #2			@ see what cache we have at this level
	blt	flush_skip			@ flush_skip if no cache, or just i-cache
/* #ifdef CONFIG_PREEMPT */
#if 1
	save_and_disable_irqs_notrace r9 @ make cssr&csidr read atomic
#endif
	mcr	p15, 2, r10, c0, c0, 0	@ select current cache level in cssr
	isb				@ isb to sych the new cssr&csidr
	mrc	p15, 1, r1, c0, c0, 0	@ read the new csidr
/* #ifdef CONFIG_PREEMPT */
#if 1
	restore_irqs_notrace r9
#endif
	and	r2, r1, #7		@ extract the length of the cache lines
	add	r2, r2, #4		@ add 4 (line length offset)
	ldr	r4, =0x3ff
	ands	r4, r4, r1, lsr #3	@ find maximum number on the way size
	clz	r5, r4			@ find bit pos of way size increment
	ldr	r7, =0x7fff
	ands	r7, r7, r1, lsr #13	@ extract max number of the index size
flush_loop1:
	mov	r9, r7			@ create working copy of max index
flush_loop2:
 /* ARM(	orr	r11, r10, r4, lsl r5	) @ factor way and cache number to r11 */
 /* THUMB(	lsl	r6, r4, r5		) */
 /* THUMB(	orr	r11, r10, r6		) @ factor way and cache number to r11 */
	orr	r11, r10, r4, lsl r5	@ factor way and cache number to r11
 /* ARM(	orr	r11, r11, r9, lsl r2	) @ factor index number into r11 */
 /* THUMB(	lsl	r6, r9, r2		) */
 /* THUMB(	orr	r11, r11, r6		) @ factor index number into r11 */
	orr	r11, r11, r9, lsl r2	@ factor index number into r11
	mcr	p15, 0, r11, c7, c14, 2	@ clean & invalidate by set/way
	subs	r9, r9, #1		@ decrement the index
	bge	flush_loop2
	subs	r4, r4, #1		@ decrement the way
	bge	flush_loop1
flush_skip:
	add	r10, r10, #2		@ increment cache number
	cmp	r3, r10
	bgt	flush_levels
flush_finished:
	mov	r10, #0			@ swith back to cache level 0
	mcr	p15, 2, r10, c0, c0, 0	@ select current cache level in cssr
	dsb
	isb
	ldmfd	sp!, {r0 - r7, r9 - r11, pc}
ENDPROC(n7_flush_dcache_all)

/**
 * n7_inv_dcache_all - Invalidate the whole D-cache.
 *
 * void n7_inv_dcache_all(void);
 *
 * Corrupted registers: r0-r7, r9-r11 (r6 only in Thumb mode)
 */
.globl n7_inv_dcache_all
n7_inv_dcache_all:
	stmfd	sp!, {r0 - r7, r9 - r11, lr}
	dmb				@ ensure ordering with prev mem access
	mrc	p15, 1, r0, c0, c0, 1	@ read clidr
	ands	r3, r0, #0x7000000	@ extract loc from clidr
	mov	r3, r3, lsr #23		@ left align loc bit field
	beq	inv_finished		@ if loc is 0, then no need to clean
	mov	r10, #0			@ start clean at cache level 0
inv_levels:
	add	r2, r10, r10, lsr #1	@ work out 3x current cache level
	mov	r1, r0, lsr r2		@ extract cache type bits from clidr
	and	r1, r1, #7		@ mask of the bits for cur cache only
	cmp	r1, #2			@ see what cache we have at this level
	blt	inv_skip			@ inv_skip if no cache, or just i-cache
/* #ifdef CONFIG_PREEMPT */
#if 1
	save_and_disable_irqs_notrace r9 @ make cssr&csidr read atomic
#endif
	mcr	p15, 2, r10, c0, c0, 0	@ select current cache level in cssr
	isb				@ isb to sych the new cssr&csidr
	mrc	p15, 1, r1, c0, c0, 0	@ read the new csidr
/* #ifdef CONFIG_PREEMPT */
#if 1
	restore_irqs_notrace r9
#endif
	and	r2, r1, #7		@ extract the length of the cache lines
	add	r2, r2, #4		@ add 4 (line length offset)
	ldr	r4, =0x3ff
	ands	r4, r4, r1, lsr #3	@ find maximum number on the way size
	clz	r5, r4			@ find bit pos of way size increment
	ldr	r7, =0x7fff
	ands	r7, r7, r1, lsr #13	@ extract max number of the index size
inv_loop1:
	mov	r9, r7			@ create working copy of max index
inv_loop2:
 /* ARM(	orr	r11, r10, r4, lsl r5	) @ factor way and cache number to r11 */
 /* THUMB(	lsl	r6, r4, r5		) */
 /* THUMB(	orr	r11, r10, r6		) @ factor way and cache number to r11 */
	orr	r11, r10, r4, lsl r5	@ factor way and cache number to r11
 /* ARM(	orr	r11, r11, r9, lsl r2	) @ factor index number into r11 */
 /* THUMB(	lsl	r6, r9, r2		) */
 /* THUMB(	orr	r11, r11, r6		) @ factor index number into r11 */
	orr	r11, r11, r9, lsl r2	@ factor index number into r11
	mcr	p15, 0, r11, c7, c6, 2	@ invalidate D entry
	subs	r9, r9, #1		@ decrement the index
	bge	inv_loop2
	subs	r4, r4, #1		@ decrement the way
	bge	inv_loop1
inv_skip:
	add	r10, r10, #2		@ increment cache number
	cmp	r3, r10
	bgt	inv_levels
inv_finished:
	mov	r10, #0			@ swith back to cache level 0
	mcr	p15, 2, r10, c0, c0, 0	@ select current cache level in cssr
	dsb
	isb
	ldmfd	sp!, {r0 - r7, r9 - r11, pc}
ENDPROC(n7_inv_dcache_all)

/**
 * n7_inv_dcache_range - Invalidate the data cache within the specified region
 * @start: virtual start address of region
 * @end: virtual end address of region
 *
 * void n7_inv_dcache_range(void *start, void *end);
 */
.globl n7_inv_dcache_range
n7_inv_dcache_range:
	stmfd	sp!, {r2 - r3, lr}
	dcache_line_size r2, r3
	sub	r3, r2, #1
	tst	r0, r3
	bic	r0, r0, r3

	mcrne	p15, 0, r0, c7, c14, 1		@ clean & invalidate D / U line

	tst	r1, r3
	bic	r1, r1, r3
	mcrne	p15, 0, r1, c7, c14, 1		@ clean & invalidate D / U line
1:
	mcr	p15, 0, r0, c7, c6, 1		@ invalidate D / U line
	add	r0, r0, r2
	cmp	r0, r1
	blo	1b
	dsb
	ldmfd	sp!, {r2 - r3, pc}
ENDPROC(n7_inv_dcache_range)

/**
 * n7_clean_dcache_range - Write back the dcache within the specified region
 * @start: virtual start address of region
 * @end: virtual end address of region
 *
 * void n7_clean_dcache_range(void *start, void *end);
 */
.globl n7_clean_dcache_range
n7_clean_dcache_range:
	stmfd	sp!, {r2 - r3, lr}
	dcache_line_size r2, r3
	sub	r3, r2, #1
	bic	r0, r0, r3
1:
	mcr	p15, 0, r0, c7, c10, 1		@ clean D / U line
	add	r0, r0, r2
	cmp	r0, r1
	blo	1b
	dsb
	ldmfd	sp!, {r2 - r3, pc}
ENDPROC(n7_clean_dcache_range)

/**
 * n7_flush_dcache_range - Write back and inv the dcache within the region
 * @start: virtual start address of region
 * @end: virtual end address of region
 *
 * void n7_flush_dcache_range(void *start, void *end);
 */
.globl n7_flush_dcache_range
n7_flush_dcache_range:
	stmfd	sp!, {r2 - r3, lr}
	dcache_line_size r2, r3
	sub	r3, r2, #1
	bic	r0, r0, r3
1:
	mcr	p15, 0, r0, c7, c14, 1		@ clean & invalidate D / U line
	add	r0, r0, r2
	cmp	r0, r1
	blo	1b
	dsb
	ldmfd	sp!, {r2 - r3, pc}
ENDPROC(n7_flush_dcache_range)

/**
 * n7_is_dcache_en - Get the dcache status, enabled or disabled
 *
 * int n7_is_dcache_en(void);
 *
 * return 1 if dcache enabled, 0 otherwise
 */
.globl n7_is_dcache_en
n7_is_dcache_en:
	mrc	p15, #0, r0, c1, c0, #0
	tst	r0, #0x4
	moveq	r0, #0				@ dcache is disabled
	movne	r0, #1
	mov	pc, lr
ENDPROC(n7_is_dcache_en)

/**
 * n7_is_icache_en - Get the dcache status, enabled or disabled
 *
 * int n7_is_icache_en(void);
 *
 * return 1 if dcache enabled, 0 otherwise
 */
.globl n7_is_icache_en
n7_is_icache_en:
	mrc     p15, #0, r0, c1, c0, #0
	tst	r0, #0x1000
	moveq	r0, #0				@ icache is disabled
	movne	r0, #1
	mov	pc, lr
ENDPROC(n7_is_icache_en)

/**
 * n7_is_mmu_en - Get the mmu status, enabled or disabled
 *
 * int n7_is_mmu_en(void);
 *
 * return 1 if mmu enabled, 0 otherwise
 */
.globl n7_is_mmu_en
n7_is_mmu_en:
	mrc     p15, #0, r0, c1, c0, #0
	tst	r0, #0x1
	moveq	r0, #0				@ mmu is disabled
	movne	r0, #1
	mov	pc, lr
ENDPROC(n7_is_mmu_en)

/**
 * n7_join_smp - set ACTLR.SMP bit
 *
 * void n7_join_smp(void);
 *
 * You must ensure the ACTLR.SMP bit is set to 1 before the caches and MMU are enabled,
 * or any cache and TLB maintenance operations are performed. The only time this bit is
 * set to 0 is during a processor power-down sequence.
 */
.globl n7_join_smp
n7_join_smp:
	stmfd	sp!, {r0 - r1, lr}
	mrc	p15, 0, r0, c1, c0, 1		@ read ACTLR
	mov	r1, r0
	orr	r0, r0, #0x040			@ set bit 6
	cmp	r0, r1
	mcrne	p15, 0, r0, c1, c0, 1		@ write ACTLR
	ldmfd	sp!, {r0 - r1, pc}
ENDPROC(n7_join_smp)

/**
 * n7_leave_smp - clear ACTLR.SMP bit
 *
 * void n7_leave_smp(void);
 *
 * You must ensure the ACTLR.SMP bit is set to 1 before the caches and MMU are enabled,
 * or any cache and TLB maintenance operations are performed. The only time this bit is
 * set to 0 is during a processor power-down sequence.
 */
.globl n7_leave_smp
n7_leave_smp:
	stmfd	sp!, {r0, lr}
	mrc     p15, 0, r0, c1, c0, 1		@ read ACTLR
	bic     r0, r0, #0x040			@ clear bit 6
	mcr     p15, 0, r0, c1, c0, 1		@ write ACTLR
	ldmfd	sp!, {r0, pc}
ENDPROC(n7_leave_smp)

/**
 * n7_inv_branch_pred_cache - Invalidate entire branch predictor array
 *
 * void n7_inv_branch_pred_cache(void);
 *
 * The branch predictor is used to improve the efficiency of jump instructions.
 * If this function(operation) is absent, the program will not run wrongly, because
 * the cpu will automatic clear instruction pipeline if the content of branch
 * predictor array is wrong.
 */
.globl n7_inv_branch_pred_cache
n7_inv_branch_pred_cache:
	stmfd	sp!, {r0, lr}
	mov	r0, #0
	mcr	p15, 0, r0, c7, c5, 6		@ invalid entire branch predictor array
	ldmfd	sp!, {r0, pc}
ENDPROC(n7_inv_branch_pred_cache)

/**
 * n7_branch_prediction_enable - Enable the branch predictor
 *
 * void n7_branch_prediction_enable(void);
 *
 * The branch predictor is used to improve the efficiency of jump instructions.
 */
.globl n7_branch_prediction_enable
n7_branch_prediction_enable:
	stmfd	sp!, {r0, lr}
	mrc	p15, 0, r0, c1, c0, 0		@ read SCTLR
	orr	r0, r0, #(1 << 11)		@ set the Z bit (bit 11)
	mcr	p15, 0,r0, c1, c0, 0		@ write SCTLR
	ldmfd	sp!, {r0, pc}
ENDPROC(n7_branch_prediction_enable)

/**
 * n7_set_ttbcr - Set the TTBCR register
 *
 * void n7_set_ttbcr(int val);
 *
 * TTBCR determines which of the Translation Table Base Registers, TTBR0 or TTBR1,
 *  defines the base address for a translation table walk required for the stage 1
 *  translation of amemory access from any mode other than Hyp mode.
 *
 * see DDI0406C p1703, p1321 for details
 */
.globl n7_set_ttbcr
n7_set_ttbcr:
	mcr     p15, 0, r0, c2, c0, 2		@ set TTBCR
	bx	lr
ENDPROC(n7_set_ttbcr)

/**
 * n7_do_idle - cpu enter wfi
 *
 * void n7_do_idle(void);
 *
 * called with irq disabled, and re-enable irq after it return
 */
.globl n7_do_idle
n7_do_idle:
	dsb					@ WFI may enter a low-power mode
	wfi
	mov	pc, lr
ENDPROC(n7_do_idle)

/**
 * n7_inv_tlb - Invalidate the whole tlb.
 *
 * void n7_inv_tlb(void);
 */
.globl n7_inv_tlb
n7_inv_tlb:
	mov	r0, #1

	dsb

	@
	@ Invalidate second level page table tlb
	@
	@ the inner shareable and out shareable is relevant with system bus,
	@ it is the same for our platform. so the below operations are same
	@
	mcr	p15, 0, r0, c8, c7, 0	@ Invalidate unified TLB

	@ Invalidate first level page table tlb
	mcr	p15, 0, r0, c8, c6, 0	@ Invalidate data TLB
	mcr	p15, 0, r0, c8, c5, 0	@ Invalidate instruction TLB

	mcr	p15, 0, r0, c8, c3, 0	@ Invalidate entire TLB Inner Shareable

	dsb
	isb
	bx	lr
ENDPROC(n7_inv_tlb)

