/*
 * mmu.h - MMU definations
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

#ifndef __MMU__H__
#define __MMU__H__

#define PAGE_SHIFT		12
#define PAGE_SIZE		(1 << PAGE_SHIFT)

#define SECTION_SHIFT		20
#define SECTION_SIZE		(1 << SECTION_SHIFT)
#define L2_TABLE_SIZE		SZ_1K	/* one section's l2 table size(256_entry * 4) */

#define L1_PT_ALIGN		SZ_16K	/* l1 page table alignment: 16k */
#define L2_PT_ALIGN		SZ_4K	/* l2 page table alignment: 1k (4k also ok) */

/* l1 pte types */
#define L1_PROG_FAULT		0b00
#define L1_PROG_L2TABLE		0b01
#define L1_PROG_SECTION		0b10

/* first level page table, size 16k, [0x40004000, 0x40008000] */
#define page_table_l1		0x40004000
#define page_l1_size		SZ_16K

/*
 * mmu property
 */
enum mmu_prog {
	/*
	 * cache property, must only select one
	 */
	WRITE_BACK	= BIT(0), /* Outer/I write-back, no allocate on write */
	WRITE_THROUGH	= BIT(1), /* Outer/I write-through, no allocate on w */
	UNCACHED	= BIT(2), /* non cacheable (normal space, not device) */
	WRITE_BACK_ALLOC = BIT(3), /* Outer/I write-back, write-allocate */
	STRONGLY_ORDERED = BIT(4), /* strongly_ordered */
	DEVICE_SH	= BIT(5), /* Shareable device */
	DEVICE_NSH	= BIT(6), /* non_Shareable device */

	SHAREABLE	= BIT(7), /* shareable bit, must be enabled in SMP */

	PROG_NG		= BIT(8), /* not global bit */

	/*
	 * access permisson.
	 * the ro/rw must select one, but not both
	 */
	ACCESS_RO	= BIT(9),  /* read only */
	ACCESS_RW	= BIT(10), /* read write */
	EXECUTABEL	= BIT(11), /* execute */
};

#define MMU_ACCESS_RW_MASK	(ACCESS_RO | ACCESS_RW)
#define MMU_PROG_CACHE_MASK	(WRITE_BACK | WRITE_THROUGH | UNCACHED		\
					| WRITE_BACK_ALLOC | STRONGLY_ORDERED	\
					| DEVICE_SH | DEVICE_NSH)

/*
 * mmu space definations (for user)
 */
enum mmu_space {
	/*
	 * space type on intial, before mmu enabled
	 */
	/* normal memory: read write, execute, shareable, cached(write back) */
	/* SPACE_INIT_NORMAL_CA = (WRITE_BACK | SHAREABLE | ACCESS_RW | EXECUTABEL), */
	SPACE_INIT_NORMAL_CA = (WRITE_BACK | SHAREABLE | ACCESS_RW),

	/* uncached memory: read write, execute, shareable, uncached */
	/* SPACE_INIT_NORMAL_UNCA = (UNCACHED | SHAREABLE | ACCESS_RW | EXECUTABEL), */
	SPACE_INIT_NORMAL_UNCA = (UNCACHED | SHAREABLE | ACCESS_RW),

	/* device space: read write, shareable, STRONGLY_ORDERED */
	SPACE_INIT_DEVICE = (STRONGLY_ORDERED | SHAREABLE | ACCESS_RW),

	/*
	 * space type that can be dynamically set (with mmu enabled)
	 */
	/* text space: read, execute, shareable, cached(write back) */
	SPACE_TEXT = (WRITE_BACK | SHAREABLE | ACCESS_RO | EXECUTABEL),

	/* ro data space: read, shareable, cached(write back) */
	SPACE_RODATA = (WRITE_BACK | SHAREABLE | ACCESS_RO),

	/* rw data space: read write, shareable, cached(write back) */
	SPACE_RWDATA = (WRITE_BACK | SHAREABLE | ACCESS_RW),

	/* text space uncached: read, execute, shareable, uncached */
	SPACE_TEXT_UNCA = (UNCACHED | SHAREABLE | ACCESS_RO | EXECUTABEL),

	/* ro data space uncached: read, shareable, uncached */
	SPACE_RODATA_UNCA = (UNCACHED | SHAREABLE | ACCESS_RO),

	/* rw data space uncached: read write, shareable, uncached */
	SPACE_RWDATA_UNCA = (UNCACHED | SHAREABLE | ACCESS_RW),
};

struct mem_area {
	rt_uint32_t va_start;	/* virtual addr start */
	rt_uint32_t pa_start;	/* phys addr start */
	rt_uint32_t size;	/* size in bytes */
	rt_uint32_t prog;	/* attribute for mapping */
};

/*
 * level 1 translation table entry (type: section)
 */
typedef union {
	u32 val;
	struct {
		u32 types	: 2;	/* lower 2 bits stand for l1 types */
		u32 b		: 1;	/* buffer */
		u32 c		: 1;	/* cacheable */
		/*
		 * execute never on strongly-ordered memory
		 * 1 means any attempt to execute an instrurcion result
		 * in a permission fault
		 */
		u32 xn		: 1;
		u32 domain	: 4;	/* domain id0-15 */
		u32 p		: 1;	/* bit[9] implementation define */
		u32 ap01	: 2;	/* ap[1:0]  access permissions */
		u32 tex		: 3;	/* type extension */
		u32 ap2		: 1;	/* apx-ap[2] */
		u32 s		: 1;	/* shareable */
		u32 ng		: 1;	/* not global bit */
		u32 rsvd0	: 1;	/* bit[18] reserved0 */
		u32 ns		: 1;	/* bit[19] */
		u32 sbaddr	: 12;	/* section base address */
	} bits;
} l1_sec_t;

/*
 * level 1 translation table entry (type: page table)
 */
typedef union {
	u32 val;
	struct {
		u32 types	: 2;	/* Lower 2 bits stand for L1 types */
		u32 pxn		: 1;
		u32 ns		: 1;	/* non secure */
		u32 sbz		: 1;	/* Should Be Preserved */
		u32 domain	: 4;
		u32 p		: 1;	/* bit[9] Implementation define */
		u32 l2_base	: 22;	/* L2 descriptor base address */
	} bits;
} l1_pte_t;

/*
 * level 2 translation table entry (for small page)
 */
typedef union {
	u32 val;
	struct {
		u32 xn		: 1;	/* lower0: execute-never bit */
		u32 one		: 1;	/* fixed to 1 */
		u32 b		: 1;	/* bufferable */
		u32 c		: 1;	/* cacheable */
		u32 ap01	: 2;	/* access permission[1:0] */
		u32 tex		: 3;
		u32 ap2		: 1;	/* access permission[2] */
		u32 s		: 1;	/* shareable */
		u32 ng		: 1;	/* not global bit */
		u32 pa		: 20;	/* small page base address PA[31:12] */
	} bits;
} l2_pte_t;

extern rt_uint32_t __text_start, __text_end;
extern rt_uint32_t __rodata_start, __rodata_end;

void rt_hw_mmu_setmtt_l1(rt_uint32_t va_start, rt_uint32_t pa_start,
			rt_uint32_t size, int prog);
void rt_hw_mmu_setmtt_l2(rt_uint32_t va_start, rt_uint32_t pa_start,
			rt_uint32_t size, int prog);
void rt_hw_mmu_change_attr(rt_uint32_t va_start, rt_uint32_t pa_start,
			rt_uint32_t size, int prog);
void rt_hw_dump_page_table(rt_uint32_t va_start, rt_uint32_t size);
void rt_hw_mmu_inv_tlb_all(void);
void rt_hw_mmu_inv_icache_all(void);
unsigned long rt_hw_set_domain_register(unsigned long domain_val);

/*
 * set mmu attribute for a range of virt space.
 * note: the vstart and pstart should be PAGE_SIZE align
 */
#define mmu_set_attr_space(vstart, pstart, vend, attr)			\
	rt_hw_mmu_change_attr((u32)(vstart), (u32)(pstart),		\
		SIZE_ALIGN((u32)(vend) - (u32)(vstart), PAGE_SIZE),	\
		(attr))

#endif /* __MMU__H__ */
