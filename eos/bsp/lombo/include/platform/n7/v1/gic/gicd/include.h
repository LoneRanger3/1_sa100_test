/* include.h */

/* #define DUMP_GICD */

/* open the definations in xxx_cfg.h if you need the function */
#include "gicd_get_cfg.h"
#include "gicd_set_cfg.h"

#include "gicd.h"
#include "gicd_get.h"
#include "gicd_set.h"

#ifdef DUMP_GICD
#include "gicd_dump.h"
#endif
