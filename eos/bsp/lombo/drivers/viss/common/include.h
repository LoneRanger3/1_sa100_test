/* include.h */

#include "csp.h"
#if defined(ARCH_LOMBO_N7V0)
#include "csp/n7/v0/include.h"
#elif defined(ARCH_LOMBO_N7V1)
#include "csp/n7/v1/include.h"
#else

#error "please select a valid platform\n"
#endif


