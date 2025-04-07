/* gicc.h */

#ifndef ___GICC___H___
#define ___GICC___H___

#define BASE_GICC                0x01052000

#define VA_GICC_CTLR             (0x00000000 + BASE_GICC + VA_GICC)
#define VA_GICC_PMR              (0x00000004 + BASE_GICC + VA_GICC)
#define VA_GICC_BPR              (0x00000008 + BASE_GICC + VA_GICC)
#define VA_GICC_IAR              (0x0000000C + BASE_GICC + VA_GICC)
#define VA_GICC_EOIR             (0x00000010 + BASE_GICC + VA_GICC)
#define VA_GICC_RPR              (0x00000014 + BASE_GICC + VA_GICC)
#define VA_GICC_HPPIR            (0x00000018 + BASE_GICC + VA_GICC)
#define VA_GICC_ABPR             (0x0000001C + BASE_GICC + VA_GICC)
#define VA_GICC_AIAR             (0x00000020 + BASE_GICC + VA_GICC)
#define VA_GICC_AEOIR            (0x00000024 + BASE_GICC + VA_GICC)
#define VA_GICC_AHPPIR           (0x00000028 + BASE_GICC + VA_GICC)
#define VA_GICC_DIR              (0x00001000 + BASE_GICC + VA_GICC)

#define DATA_GICC_CTLR           0x00000000
#define DATA_GICC_PMR            0x00000000
#define DATA_GICC_BPR            0x00000000
#define DATA_GICC_IAR            0x000003FF
#define DATA_GICC_EOIR           0x00000000
#define DATA_GICC_RPR            0x000000FF
#define DATA_GICC_HPPIR          0x000003FF
#define DATA_GICC_ABPR           0x00000000
#define DATA_GICC_AIAR           0x000003FF
#define DATA_GICC_AEOIR          0x00000000
#define DATA_GICC_AHPPIR         0x000003FF
#define DATA_GICC_DIR            0x00000000

/* CPU Interface Control RegisterIt enables the signaling of interrupt by the CPU interface to the connected processor. */
typedef union {
	u32 val;
	struct {
	u32 enablegrp0:1;               /* [1¡¯b0]: Disable signaling of
					 * Group0 interrupts
					 * [1¡¯b1]: Enable signaling of
					 * Group0 interrupts */
	u32 enablegrp1:1;               /* [1¡¯b0]: Disable signaling of
					 * Group1 interrupts
					 * [1¡¯b1]: Enable signaling of
					 * Group1 interrupts */
	u32 ackctl:1;                   /* When received a Group1 interrupt,
					 * determines whether a read of
					 * IAR/HPPIR returns a spurious
					 * interrupt ID or not. */
	u32 fiqen:1;                    /* [1¡¯b0]: Signal Group0 interrupts
					 * using the IRQ signal.
					 * [1¡¯b1]: Signal Group0 interrupts
					 * using the FIQ signal.
					 * Note: The GIC always signals
					 * Group1 interrupts using the
					 * IRQ signal. */
	u32 cbpr:1;                     /* [1¡¯b0]: To determine any preemption
					 * use the BPR for Group0 interrupts
					 * and the ABPR for Group1 interrupts.
					 * [1¡¯b1]: To determine any preemption
					 * use the BPR for both Group0
					 * and Group1 interrupts. */
	u32 fiqbypdisgrp0:1;            /* [1¡¯b0]: Bypass FIQ signal is
					 * signaled to the processor.
					 * [1¡¯b1]: Bypass FIQ signal is
					 * not signaled to the processor. */
	u32 irqbypdisgrp0:1;            /* [1¡¯b0]: Bypass IRQ signal is
					 * signaled to the processor.
					 * [1¡¯b1]: Bypass IRQ signal is
					 * not signaled to the processor. */
	u32 fiqbypdisgrp1:1;            /* Alias of FIQBypDisGrp1 from
					 * the non-secure copy of this register. */
	u32 irqbypdisgrp1:1;            /* Alias of IRQBypDisGrp1 from
					 * the non-secure copy of this register. */
	u32 eoimodes:1;                 /* [1¡¯b0]: EOIR has both priority
					 * drop and deactivate interrupt
					 * functionality.
					 * [1¡¯b1]: EOIR has priority drop
					 * functionality only. */
	u32 eoimodens:1;                /* Alias of EOImodeNS from the
					 * Non-secure copy of this register. */
	u32 rsvd0:21;                   /* - */
	} bits;
} reg_gicc_ctlr_t;

#define GICC_CTLR_ENABLEGRP0_0          0x0
#define GICC_CTLR_ENABLEGRP0_1          0x1
#define GICC_CTLR_ENABLEGRP1_0          0x0
#define GICC_CTLR_ENABLEGRP1_1          0x1
#define GICC_CTLR_FIQEN_0               0x0
#define GICC_CTLR_FIQEN_1               0x1
#define GICC_CTLR_CBPR_0                0x0
#define GICC_CTLR_CBPR_1                0x1
#define GICC_CTLR_FIQBYPDISGRP0_0       0x0
#define GICC_CTLR_FIQBYPDISGRP0_1       0x1
#define GICC_CTLR_IRQBYPDISGRP0_0       0x0
#define GICC_CTLR_IRQBYPDISGRP0_1       0x1
#define GICC_CTLR_EOIMODES_0            0x0
#define GICC_CTLR_EOIMODES_1            0x1

/* Interrupt Priority Mask RegisterIt provides an interrupt priority filter. */
typedef union {
	u32 val;
	struct {
	u32 priority:8;         /* The priority mask level for the CPU
				 * interface. If the priority of an interrupt
				 * is higher than the value indicated by
				 * this field, the interface signals the
				 * interrupt to the processor. */
	u32 rsvd0:24;           /* - */
	} bits;
} reg_gicc_pmr_t;

/* Binary Point RegisterIt defines the point at which the priority value fields split into two parts, the group priority field and the subpriority field. */
typedef union {
	u32 val;
	struct {
	u32 binarypoint:3;      /* The value of this field controls how
				 * the 8-bit interrupt priority field is
				 * split into a group priority field, used
				 * to determine interrupt preemption, and
				 * a subpriority field. */
	u32 rsvd0:29;           /* - */
	} bits;
} reg_gicc_bpr_t;

/* Interrupt Acknowledge RegisterThe processor reads this register to obtain the interrupt ID of the signaled interrupt. */
typedef union {
	u32 val;
	struct {
	u32 interruptid:10;     /* The interrupt ID. A read of the IAR
				 * returns the interrupt ID of the highest
				 * priority pending interrupt for the CPU
				 * interface. */
	u32 cpuid:3;            /* For SGIs in a multiprocessor implementation,
				 * this field identifies the processor
				 * that requested the interrupt. It returns
				 * the number of the CPU interface that
				 * made the request. */
	u32 rsvd0:19;           /* - */
	} bits;
} reg_gicc_iar_t;

/* End of Interrupt RegisterA processor writes to this register to inform the CPU interface that it has completed the processing of the specified interrupt. */
typedef union {
	u32 val;
	struct {
	u32 eoiintid:10;        /* The interrupt ID value from the corresponding
				 * IAR access.. */
	u32 cpuid:3;            /* On a multiprocessor implementation,
				 * if the write refers to an SGI, this
				 * field contains the CPUID value from
				 * the corresponding IAR access. */
	u32 rsvd0:19;           /* - */
	} bits;
} reg_gicc_eoir_t;

/* Running Priority Register */
typedef union {
	u32 val;
	struct {
	u32 priority:8;         /* It indicates the current running priority
				 * on the CPU interface.If there is no
				 * active interrupt on the CPU interface,
				 * the value returned is the Idle priority. */
	u32 rsvd0:24;           /* - */
	} bits;
} reg_gicc_rpr_t;

/* Highest Priority Pending Interrupt Register.It indicates the Interrupt ID of the highest priority pending interrupt on the CPU interface */
typedef union {
	u32 val;
	struct {
	u32 pendintid:8;        /* The interrupt ID of the highest priority
				 * pending interrupt. */
	u32 cpuid:5;            /* If the PENDINTID field returns the ID
				 * of an SGI, this field contains the CPUID
				 * value(the processor that generated the
				 * interrupt). In all other cases this
				 * field is RAZ. */
	u32 rsvd0:19;           /* - */
	} bits;
} reg_gicc_hppir_t;

/* Aliased Binary Point Register */
typedef union {
	u32 val;
	struct {
	u32 binarypoint:3;      /* The value of this field controls how
				 * the 8-bit interrupt priority field is
				 * split into a group priority field, used
				 * to determine interrupt preemption, and
				 * a subpriority field. */
	u32 rsvd0:29;           /* - */
	} bits;
} reg_gicc_abpr_t;

/* Aliased Interrupt Acknowledge Register */
typedef union {
	u32 val;
	struct {
	u32 interruptid:10;     /* The interrupt ID. A read of the IAR
				 * returns the interrupt ID of the highest
				 * priority pending interrupt for the CPU
				 * interface. */
	u32 cpuid:3;            /* For SGIs in a multiprocessor implementation,
				 * this field identifies the processor
				 * that requested the interrupt. It returns
				 * the number of the CPU interface that
				 * made the request. */
	u32 rsvd0:19;           /* - */
	} bits;
} reg_gicc_aiar_t;

/* Aliased End of Interrupt Register */
typedef union {
	u32 val;
	struct {
	u32 eoiintid:10;        /* The interrupt ID value from the corresponding
				 * IAR access.. */
	u32 cpuid:3;            /* On a multiprocessor implementation,
				 * if the write refers to an SGI, this
				 * field contains the CPUID value from
				 * the corresponding IAR access. */
	u32 rsvd0:19;           /* - */
	} bits;
} reg_gicc_aeoir_t;

/* Aliased Highest Priority Pending Interrupt Register. */
typedef union {
	u32 val;
	struct {
	u32 pendintid:8;        /* The interrupt ID of the highest priority
				 * pending interrupt. */
	u32 cpuid:5;            /* If the PENDINTID field returns the ID
				 * of an SGI, this field contains the CPUID
				 * value(the processor that generated the
				 * interrupt). In all other cases this
				 * field is RAZ. */
	u32 rsvd0:19;           /* - */
	} bits;
} reg_gicc_ahppir_t;

/* Deactivate Interrupt Register. */
typedef union {
	u32 val;
	struct {
	u32 pendintid:8;        /* The interrupt ID */
	u32 cpuid:5;            /* For an SGI in a multiprocessor implementation,
				 * this field identifies the processor
				 * that requested the interrupt. For other
				 * interrupts this field is RAZ. */
	u32 rsvd0:19;           /* - */
	} bits;
} reg_gicc_dir_t;

/* reg_gicc_t bank */
typedef struct tag_gicc {
	reg_gicc_ctlr_t    ctlr;   /* 0000 */
	reg_gicc_pmr_t     pmr;    /* 0004 */
	reg_gicc_bpr_t     bpr;    /* 0008 */
	reg_gicc_iar_t     iar;    /* 000C */
	reg_gicc_eoir_t    eoir;   /* 0010 */
	reg_gicc_rpr_t     rpr;    /* 0014 */
	reg_gicc_hppir_t   hppir;  /* 0018 */
	reg_gicc_abpr_t    abpr;   /* 001C */
	reg_gicc_aiar_t    aiar;   /* 0020 */
	reg_gicc_aeoir_t   aeoir;  /* 0024 */
	reg_gicc_ahppir_t  ahppir; /* 0028 */
	u32                res0[1013];
	reg_gicc_dir_t     dir;    /* 1000 */
} reg_gicc_t;

#endif /* ___GICC___H___ */
