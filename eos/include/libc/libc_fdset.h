/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * File      : libc_errno.h
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-10-30     Bernard      The first version
 */

#ifndef LIBC_FDSET_H__
#define LIBC_FDSET_H__

#include <rtconfig.h>

#ifndef ARCH_LOMBO /* use local defination, not toolchain */
/* #if defined(RT_USING_NEWLIB) || defined(_WIN32) */

#include <sys/types.h>
#if defined(HAVE_SYS_SELECT_H)
#include <sys/select.h>
#endif

#else

#ifndef _SYS_SELECT_H /* lombo: fix type conflict with toolchain */
#define _SYS_SELECT_H

#ifdef SAL_USING_POSIX

#ifdef FD_SETSIZE
#undef FD_SETSIZE
#endif

#define FD_SETSIZE      DFS_FD_MAX
#endif

#  ifndef   FD_SETSIZE
#   define  FD_SETSIZE  32
#  endif

#  define   NBBY    8       /* number of bits in a byte */

typedef long    fd_mask;
#  define   NFDBITS (sizeof (fd_mask) * NBBY)   /* bits per mask */
#  ifndef   howmany
#   define  howmany(x,y)    (((x)+((y)-1))/(y))
#  endif

/* We use a macro for fd_set so that including Sockets.h afterwards
   can work.  */
typedef struct _types_fd_set {
    fd_mask fds_bits[howmany(FD_SETSIZE, NFDBITS)];
} _types_fd_set;

#define fd_set _types_fd_set

#  define   FD_SET(n, p)    ((p)->fds_bits[(n)/NFDBITS] |= (1L << ((n) % NFDBITS)))
#  define   FD_CLR(n, p)    ((p)->fds_bits[(n)/NFDBITS] &= ~(1L << ((n) % NFDBITS)))
#  define   FD_ISSET(n, p)  ((p)->fds_bits[(n)/NFDBITS] & (1L << ((n) % NFDBITS)))
#  define   FD_ZERO(p)      memset((void*)(p), 0, sizeof(*(p)))

#endif /* _SYS_SELECT_H */

#endif

#endif
