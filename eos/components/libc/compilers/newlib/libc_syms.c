/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017/10/15     bernard      the first version
 */
#include <rtthread.h>
#include <rtm.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

RTM_EXPORT(strcpy);
RTM_EXPORT(strncpy);
RTM_EXPORT(strlen);
RTM_EXPORT(strcat);
RTM_EXPORT(strstr);
RTM_EXPORT(strchr);
RTM_EXPORT(strrchr);
RTM_EXPORT(strcmp);
RTM_EXPORT(strtol);
RTM_EXPORT(strtoul);
RTM_EXPORT(strncmp);

RTM_EXPORT(memcpy);
RTM_EXPORT(memcmp);
RTM_EXPORT(memmove);
RTM_EXPORT(memset);
RTM_EXPORT(memchr);

RTM_EXPORT(putchar);
RTM_EXPORT(puts);
RTM_EXPORT(printf);
RTM_EXPORT(sprintf);
RTM_EXPORT(snprintf);
RTM_EXPORT(fprintf);
RTM_EXPORT(fflush);

RTM_EXPORT(fopen);
RTM_EXPORT(fclose);
RTM_EXPORT(fread);
RTM_EXPORT(fseek);
RTM_EXPORT(fsetpos);
RTM_EXPORT(fwrite);
RTM_EXPORT(ftell);
RTM_EXPORT(fgets);
RTM_EXPORT(feof);
RTM_EXPORT(remove);
RTM_EXPORT(strtok);


#include <time.h>
RTM_EXPORT(localtime);
RTM_EXPORT(time);

#include <setjmp.h>
RTM_EXPORT(longjmp);
RTM_EXPORT(setjmp);

RTM_EXPORT(exit);
RTM_EXPORT(atexit);
RTM_EXPORT(abort);

RTM_EXPORT(rand);
#include <assert.h>
RTM_EXPORT(__assert_func);

#ifdef ARCH_LOMBO
#include <debug.h>
#include <dfs_posix.h>
#include <math.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/unistd.h>
#include <sys/time.h>

#define RTM_EXPORT_LIBC(symb)	extern void *symb;	\
				RTM_EXPORT(symb);

/* math functions */
RTM_EXPORT(sqrt);
RTM_EXPORT(logf);
RTM_EXPORT(sqrtf);
RTM_EXPORT(cosf);
RTM_EXPORT(acosf);
RTM_EXPORT(tanf);
RTM_EXPORT(asinf);
RTM_EXPORT(sinf);
RTM_EXPORT(atanf);
RTM_EXPORT(srand);
RTM_EXPORT(powf);
RTM_EXPORT(qsort);
RTM_EXPORT(cos);
RTM_EXPORT(sin);
RTM_EXPORT(atan2);
RTM_EXPORT(tan);
RTM_EXPORT(atan);
RTM_EXPORT(expf);
RTM_EXPORT(acos);
RTM_EXPORT(log);
RTM_EXPORT(atoi);
RTM_EXPORT(floorf);
RTM_EXPORT(ceilf);
RTM_EXPORT(pow);
RTM_EXPORT(atof);
RTM_EXPORT(floor);
RTM_EXPORT(ceil);
RTM_EXPORT(round);
RTM_EXPORT(getenv);
RTM_EXPORT(mktime);
RTM_EXPORT(roundf);
RTM_EXPORT(exp);
RTM_EXPORT(tanhf);
RTM_EXPORT(log10f);
/* stdio/string functions */
RTM_EXPORT(strtod);
RTM_EXPORT(vsnprintf);
RTM_EXPORT(sscanf);
RTM_EXPORT(strtoull);
RTM_EXPORT(fileno);
RTM_EXPORT(strcasecmp);
RTM_EXPORT(fputc);
RTM_EXPORT(fputs);
RTM_EXPORT(_impure_ptr);
RTM_EXPORT(fscanf);
/* heap functions */
RTM_EXPORT(malloc);
RTM_EXPORT(realloc);
RTM_EXPORT(calloc);
RTM_EXPORT(free);
RTM_EXPORT(access);

/*
 * libgcc functions - functions that are used internally by the
 * compiler...  (prototypes are not correct though, but that
 * doesn't really matter since they're not versioned).
 */
RTM_EXPORT_LIBC(__ashldi3);
RTM_EXPORT_LIBC(__ashrdi3);
RTM_EXPORT_LIBC(__divsi3);
RTM_EXPORT_LIBC(__lshrdi3);
RTM_EXPORT_LIBC(__modsi3);
RTM_EXPORT_LIBC(__muldi3);
RTM_EXPORT_LIBC(__ucmpdi2);
RTM_EXPORT_LIBC(__udivsi3);
RTM_EXPORT_LIBC(__umodsi3);
RTM_EXPORT_LIBC(__aeabi_idiv);
RTM_EXPORT_LIBC(__aeabi_idivmod);
RTM_EXPORT_LIBC(__aeabi_lasr);
RTM_EXPORT_LIBC(__aeabi_llsl);
RTM_EXPORT_LIBC(__aeabi_llsr);
RTM_EXPORT_LIBC(__aeabi_lmul);
RTM_EXPORT_LIBC(__aeabi_uidiv);
RTM_EXPORT_LIBC(__aeabi_uidivmod);
RTM_EXPORT_LIBC(__aeabi_ulcmp);
RTM_EXPORT_LIBC(__aeabi_ldivmod);
RTM_EXPORT_LIBC(__aeabi_uldivmod);
RTM_EXPORT_LIBC(__aeabi_ui2d);
RTM_EXPORT_LIBC(__aeabi_dcmplt);
RTM_EXPORT_LIBC(__aeabi_ddiv);
RTM_EXPORT_LIBC(__aeabi_dmul);
RTM_EXPORT_LIBC(__aeabi_dcmpgt);
RTM_EXPORT_LIBC(__aeabi_dcmpge);
RTM_EXPORT_LIBC(__aeabi_dsub);
RTM_EXPORT_LIBC(__aeabi_i2d);
RTM_EXPORT_LIBC(__aeabi_dcmpeq);
RTM_EXPORT_LIBC(__aeabi_d2iz);
RTM_EXPORT_LIBC(__aeabi_l2d);
RTM_EXPORT_LIBC(__aeabi_atexit);
RTM_EXPORT_LIBC(__aeabi_ul2d);
RTM_EXPORT_LIBC(tolower);
RTM_EXPORT_LIBC(__locale_ctype_ptr);

/* fs interface realization */
int _fstat(int file, struct stat *st)
{
	return fstat(file, st);
}
RTM_EXPORT(_fstat);

int _isatty(int fd)
{
	return isatty(fd);
}
RTM_EXPORT(_isatty);

caddr_t _sbrk(int incr)
{
	return rt_malloc(incr);
}
RTM_EXPORT(_sbrk);

off_t _lseek(int fd, off_t offset, int whence)
{
	return lseek(fd, offset, whence);
}
RTM_EXPORT(_lseek);

int _write(int fd, char *buf, int nbytes)
{
	return write(fd, buf, nbytes);
}
RTM_EXPORT(_write);

int _read(int fd, char *buf, int nbytes)
{
	return read(fd, buf, nbytes);
}
RTM_EXPORT(_read);

int _close(int file)
{
	return close(file);
}
RTM_EXPORT(_close);

int _open(const char *file, int flags, ...)
{
	return open(file, flags);
}
RTM_EXPORT(_open);

int _unlink(const char *pathname)
{
	return unlink(pathname);
}
RTM_EXPORT(_unlink);

int _stat(const char *file, struct stat *buf)
{
	return stat(file, buf);
}
RTM_EXPORT(_stat);

int _gettimeofday(struct timeval *tp, void *ignore)
{
	return gettimeofday(tp, ignore);
}
RTM_EXPORT(_gettimeofday);

#ifdef RT_USING_CPLUSPLUS
#include <unwind.h>

/* global c++ constructor */
extern int __ctors_start__;
extern int __ctors_end__;
RTM_EXPORT(__ctors_start__);
RTM_EXPORT(__ctors_end__);

/* c++ exception hander */
RTM_EXPORT(_Unwind_Resume_or_Rethrow);
RTM_EXPORT(_Unwind_GetRegionStart);
RTM_EXPORT(_Unwind_VRS_Set);
RTM_EXPORT(_Unwind_Resume);
RTM_EXPORT(_Unwind_DeleteException);
RTM_EXPORT(_Unwind_Complete);
RTM_EXPORT(_Unwind_RaiseException);
RTM_EXPORT(_Unwind_GetTextRelBase);
RTM_EXPORT(_Unwind_GetLanguageSpecificData);
RTM_EXPORT(_Unwind_VRS_Get);
RTM_EXPORT(__gnu_unwind_frame);
RTM_EXPORT(_Unwind_GetDataRelBase);

/* c++ std symbols */
RTM_EXPORT_LIBC(_ZNSt8ios_base4InitC1Ev);
RTM_EXPORT_LIBC(_ZNSt8ios_base4InitD1Ev);
RTM_EXPORT_LIBC(_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC1Ev);
RTM_EXPORT_LIBC(_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev);
RTM_EXPORT_LIBC(_ZNSt9basic_iosIcSt11char_traitsIcEE4initEPSt15basic_streambufIcS1_E);
RTM_EXPORT_LIBC(_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj);
RTM_EXPORT_LIBC(_ZNSt8ios_baseC2Ev);
RTM_EXPORT_LIBC(_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_appendEPKcj);
RTM_EXPORT_LIBC(_ZNSt8__detail15_List_node_base9_M_unhookEv);
RTM_EXPORT_LIBC(_ZNSt6localeC1Ev);
RTM_EXPORT_LIBC(_ZNSt6chrono3_V212system_clock3nowEv);
RTM_EXPORT_LIBC(_ZNSt7__cxx1115basic_stringbufIcSt11char_traitsIcESaIcEE7_M_syncEPcjj);
RTM_EXPORT_LIBC(_ZNSt6localeD1Ev);
RTM_EXPORT_LIBC(_ZNSt8__detail15_List_node_base7_M_hookEPS0_);
RTM_EXPORT_LIBC(_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_replaceEjjPKcj);
RTM_EXPORT_LIBC(_ZNSt8ios_baseD2Ev);

RTM_EXPORT_LIBC(_ZSt4cout);
RTM_EXPORT_LIBC(_ZSt24__throw_out_of_range_fmtPKcz);
RTM_EXPORT_LIBC(_ZSt20__throw_length_errorPKc);
RTM_EXPORT_LIBC(_ZSt17__throw_bad_allocv);
RTM_EXPORT_LIBC(_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_i);
RTM_EXPORT_LIBC(_ZSt19__throw_logic_errorPKc);
RTM_EXPORT_LIBC(_ZSt16__throw_bad_castv);
RTM_EXPORT_LIBC(_ZSt29_Rb_tree_insert_and_rebalancebPSt18_Rb_tree_node_baseS0_RS_);
RTM_EXPORT_LIBC(_ZSt18_Rb_tree_incrementPSt18_Rb_tree_node_base);
RTM_EXPORT_LIBC(_ZSt20__throw_out_of_rangePKc);
RTM_EXPORT_LIBC(_ZSt18_Rb_tree_decrementPSt18_Rb_tree_node_base);
RTM_EXPORT_LIBC(_ZSt7getlineIcSt11char_traitsIcESaIcEERSt13basic_istreamIT_T0_ES7_RNSt7__cxx1112basic_stringIS4_S5_T1_EES4_);
RTM_EXPORT_LIBC(_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE14_M_replace_auxEjjjc);

RTM_EXPORT_LIBC(_ZTVSt15basic_streambufIcSt11char_traitsIcEE);
RTM_EXPORT_LIBC(_ZTVSt9basic_iosIcSt11char_traitsIcEE);
RTM_EXPORT_LIBC(_ZTVNSt7__cxx1115basic_stringbufIcSt11char_traitsIcESaIcEEE);
RTM_EXPORT_LIBC(_ZTVNSt7__cxx1118basic_stringstreamIcSt11char_traitsIcESaIcEEE);
RTM_EXPORT_LIBC(_ZTTNSt7__cxx1118basic_stringstreamIcSt11char_traitsIcESaIcEEE);
RTM_EXPORT_LIBC(_ZNSo5flushEv);
RTM_EXPORT_LIBC(_ZNSo3putEc);
RTM_EXPORT_LIBC(_ZNKSt5ctypeIcE13_M_widen_initEv);
RTM_EXPORT_LIBC(_ZdaPvj);

RTM_EXPORT_LIBC(_ZSt18_Rb_tree_incrementPKSt18_Rb_tree_node_base);
RTM_EXPORT_LIBC(_ZNSt6chrono3_V212steady_clock3nowEv);
RTM_EXPORT_LIBC(_ZSt28_Rb_tree_rebalance_for_erasePSt18_Rb_tree_node_baseRS_);
RTM_EXPORT_LIBC(_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_assignERKS4_);

RTM_EXPORT_LIBC(_ZNSt7__cxx1118basic_stringstreamIcSt11char_traitsIcESaIcEED1Ev);
RTM_EXPORT_LIBC(_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC1ERKS4_);
RTM_EXPORT_LIBC(_ZNSaIcEC1Ev);
RTM_EXPORT_LIBC(_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE5c_strEv);
RTM_EXPORT_LIBC(_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEaSEPKc);
RTM_EXPORT_LIBC(_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEixEj);
RTM_EXPORT_LIBC(_ZNSaIcED1Ev);
RTM_EXPORT_LIBC(_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE6appendERKS4_);
RTM_EXPORT_LIBC(_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE7compareERKS4_);
RTM_EXPORT_LIBC(_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEpLEPKc);
RTM_EXPORT_LIBC(_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC1EOS4_);
RTM_EXPORT_LIBC(_ZNKSt9basic_iosIcSt11char_traitsIcEEcvbEv);
RTM_EXPORT_LIBC(_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC1EPKcRKS3_);
RTM_EXPORT_LIBC(_ZNSt7__cxx1118basic_stringstreamIcSt11char_traitsIcESaIcEEC1ERKNS_12basic_stringIcS2_S3_EESt13_Ios_Openmode);
RTM_EXPORT_LIBC(_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE6lengthEv);

RTM_EXPORT_LIBC(_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc);
RTM_EXPORT_LIBC(_ZNSt8__detail15_List_node_base11_M_transferEPS0_S1_);
RTM_EXPORT_LIBC(_ZNSt8__detail15_List_node_base4swapERS0_S1_);
RTM_EXPORT_LIBC(_ZStlsIcSt11char_traitsIcESaIcEERSt13basic_ostreamIT_T0_ES7_RKNSt7__cxx1112basic_stringIS4_S5_T1_EE);
RTM_EXPORT_LIBC(_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_);
RTM_EXPORT_LIBC(_ZNSolsEPFRSoS_E);
RTM_EXPORT_LIBC(_ZNSolsEd);

RTM_EXPORT_LIBC(_ZNSo9_M_insertIdEERSoT_);

#endif /* RT_USING_CPLUSPLUS */

#endif /* ARCH_LOMBO */
