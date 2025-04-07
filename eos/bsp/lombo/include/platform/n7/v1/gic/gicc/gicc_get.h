/* gicc_get.h */

#ifndef ___GICC__GET___H___
#define ___GICC__GET___H___

#ifdef DEF_GET_GICC_CTLR_ENABLEGRP0
def_get_mod_reg_bit(gicc, ctlr, enablegrp0, addr);
#endif

#ifdef DEF_GET_GICC_CTLR_ENABLEGRP1
def_get_mod_reg_bit(gicc, ctlr, enablegrp1, addr);
#endif

#ifdef DEF_GET_GICC_CTLR_ACKCTL
def_get_mod_reg_bit(gicc, ctlr, ackctl, addr);
#endif

#ifdef DEF_GET_GICC_CTLR_FIQEN
def_get_mod_reg_bit(gicc, ctlr, fiqen, addr);
#endif

#ifdef DEF_GET_GICC_CTLR_CBPR
def_get_mod_reg_bit(gicc, ctlr, cbpr, addr);
#endif

#ifdef DEF_GET_GICC_CTLR_FIQBYPDISGRP0
def_get_mod_reg_bit(gicc, ctlr, fiqbypdisgrp0, addr);
#endif

#ifdef DEF_GET_GICC_CTLR_IRQBYPDISGRP0
def_get_mod_reg_bit(gicc, ctlr, irqbypdisgrp0, addr);
#endif

#ifdef DEF_GET_GICC_CTLR_FIQBYPDISGRP1
def_get_mod_reg_bit(gicc, ctlr, fiqbypdisgrp1, addr);
#endif

#ifdef DEF_GET_GICC_CTLR_IRQBYPDISGRP1
def_get_mod_reg_bit(gicc, ctlr, irqbypdisgrp1, addr);
#endif

#ifdef DEF_GET_GICC_CTLR_EOIMODES
def_get_mod_reg_bit(gicc, ctlr, eoimodes, addr);
#endif

#ifdef DEF_GET_GICC_CTLR_EOIMODENS
def_get_mod_reg_bit(gicc, ctlr, eoimodens, addr);
#endif

#ifdef DEF_GET_GICC_PMR_PRIORITY
def_get_mod_reg_bit(gicc, pmr, priority, addr);
#endif

#ifdef DEF_GET_GICC_BPR_BINARYPOINT
def_get_mod_reg_bit(gicc, bpr, binarypoint, addr);
#endif

#ifdef DEF_GET_GICC_IAR_INTERRUPTID
def_get_mod_reg_bit(gicc, iar, interruptid, addr);
#endif

#ifdef DEF_GET_GICC_IAR_CPUID
def_get_mod_reg_bit(gicc, iar, cpuid, addr);
#endif

#ifdef DEF_GET_GICC_EOIR_EOIINTID
def_get_mod_reg_bit(gicc, eoir, eoiintid, addr);
#endif

#ifdef DEF_GET_GICC_EOIR_CPUID
def_get_mod_reg_bit(gicc, eoir, cpuid, addr);
#endif

#ifdef DEF_GET_GICC_RPR_PRIORITY
def_get_mod_reg_bit(gicc, rpr, priority, addr);
#endif

#ifdef DEF_GET_GICC_HPPIR_PENDINTID
def_get_mod_reg_bit(gicc, hppir, pendintid, addr);
#endif

#ifdef DEF_GET_GICC_HPPIR_CPUID
def_get_mod_reg_bit(gicc, hppir, cpuid, addr);
#endif

#ifdef DEF_GET_GICC_ABPR_BINARYPOINT
def_get_mod_reg_bit(gicc, abpr, binarypoint, addr);
#endif

#ifdef DEF_GET_GICC_AIAR_INTERRUPTID
def_get_mod_reg_bit(gicc, aiar, interruptid, addr);
#endif

#ifdef DEF_GET_GICC_AIAR_CPUID
def_get_mod_reg_bit(gicc, aiar, cpuid, addr);
#endif

#ifdef DEF_GET_GICC_AEOIR_EOIINTID
def_get_mod_reg_bit(gicc, aeoir, eoiintid, addr);
#endif

#ifdef DEF_GET_GICC_AEOIR_CPUID
def_get_mod_reg_bit(gicc, aeoir, cpuid, addr);
#endif

#ifdef DEF_GET_GICC_AHPPIR_PENDINTID
def_get_mod_reg_bit(gicc, ahppir, pendintid, addr);
#endif

#ifdef DEF_GET_GICC_AHPPIR_CPUID
def_get_mod_reg_bit(gicc, ahppir, cpuid, addr);
#endif

#ifdef DEF_GET_GICC_DIR_PENDINTID
def_get_mod_reg_bit(gicc, dir, pendintid, addr);
#endif

#ifdef DEF_GET_GICC_DIR_CPUID
def_get_mod_reg_bit(gicc, dir, cpuid, addr);
#endif

#endif /* ___GICC__GET___H___ */
