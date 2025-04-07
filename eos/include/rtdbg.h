/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2016-11-12     Bernard      The first version
 * 2018-05-25     armink       Add simple API, such as LOG_D, LOG_E
 */

/*
 * The macro definitions for debug
 *
 * These macros are defined in static. If you want to use debug macro, you can
 * use as following code:
 *
 * In your C/C++ file, enable/disable DEBUG_ENABLE macro, and then include this
 * header file.
 *
 * #define DBG_TAG           "MOD_TAG"
 * #define DBG_LVL           DBG_INFO
 * #include <rtdbg.h>          // must after of DBG_LVL, DBG_TAG or other options
 *
 * Then in your C/C++ file, you can use LOG_X macro to print out logs:
 * LOG_D("this is a debug log!");
 * LOG_E("this is a error log!");
 */

#ifndef RT_DBG_H__
#define RT_DBG_H__

#include <rtconfig.h>
#include <rthw.h>

/* the debug log will force enable when RT_DEBUG macro is defined */
#if defined(RT_DEBUG) && !defined(DBG_ENABLE)
#define DBG_ENABLE
#endif

/* it will force output color log when RT_DEBUG_COLOR macro is defined */
#if defined(RT_DEBUG_COLOR) && !defined(DBG_COLOR)
#define DBG_COLOR
#endif

#if defined(RT_USING_ULOG)
/* using ulog compatible with rtdbg  */
#include <ulog.h>
#else

/* DEBUG level */
#define DBG_ERROR           0
#define DBG_WARNING         1
#define DBG_INFO            2
#define DBG_LOG             3

#ifdef DBG_TAG
#ifndef DBG_SECTION_NAME
#define DBG_SECTION_NAME    DBG_TAG
#endif
#else
/* compatible with old version */
#ifndef DBG_SECTION_NAME
#define DBG_SECTION_NAME    "DBG"
#endif
#endif /* DBG_TAG */

#ifdef DBG_ENABLE

#ifdef DBG_LVL
#ifndef DBG_LEVEL
#define DBG_LEVEL         DBG_LVL
#endif
#else
/* compatible with old version */
#ifndef DBG_LEVEL
#define DBG_LEVEL         DBG_WARNING
#endif
#endif /* DBG_LVL */

/*
 * The color for terminal (foreground)
 * BLACK    30
 * RED      31
 * GREEN    32
 * YELLOW   33
 * BLUE     34
 * PURPLE   35
 * CYAN     36
 * WHITE    37
 */
#ifdef DBG_COLOR
#define _DBG_COLOR(n)        rt_kprintf("\033["#n"m")
#if defined(ARCH_LOMBO) && defined(RT_USING_SMP)
#define _DBG_LOG_HDR(lvl_name, color_n)                    \
	do {										\
		int msec = rt_time_get_msec();						\
		rt_kprintf("[%5d.%03d]\033["#color_n"m["lvl_name"/"DBG_SECTION_NAME"][cpu:%d][%s:%d] ", \
			msec / 1000, msec % 1000, rt_hw_cpu_id(), __func__, __LINE__);			\
	} while (0)
#else
#define _DBG_LOG_HDR(lvl_name, color_n)                    \
	do {										\
		int msec = rt_time_get_msec();						\
		rt_kprintf("[%5d.%03d]\033["#color_n"m["lvl_name"/"DBG_SECTION_NAME"][%s:%d] ", \
			msec / 1000, msec % 1000, __func__, __LINE__);			\
	} while (0)
#endif /* ARCH_LOMBO && RT_USING_SMP */
#define _DBG_LOG_X_END                                     \
    rt_kprintf("\033[0m\n")
#else
#define _DBG_COLOR(n)
#define _DBG_LOG_HDR(lvl_name, color_n)                    \
	do {										\
		int msec = rt_time_get_msec();						\
		rt_kprintf("[%5d.%03d]["lvl_name"/"DBG_SECTION_NAME"][%s:%d][%d] ",	\
			msec / 1000, msec % 1000, __func__, __LINE__);			\
	} while (0)
#define _DBG_LOG_X_END                                     \
    rt_kprintf("\n")
#endif /* DBG_COLOR */

/*
 * static debug routine
 * NOTE: This is a NOT RECOMMENDED API. Please using LOG_X API.
 *       It will be DISCARDED later. Because it will take up more resources.
 */
#define dbg_log(level, fmt, ...)                            \
    if ((level) <= DBG_LEVEL)                               \
    {                                                       \
        switch(level)                                       \
        {                                                   \
            case DBG_ERROR:   _DBG_LOG_HDR("E", 31); break; \
            case DBG_WARNING: _DBG_LOG_HDR("W", 33); break; \
            case DBG_INFO:    _DBG_LOG_HDR("I", 32); break; \
            case DBG_LOG:     _DBG_LOG_HDR("D", 0); break;  \
            default: break;                                 \
        }                                                   \
        rt_kprintf(fmt, ##__VA_ARGS__);                     \
        _DBG_COLOR(0);                                      \
    }

#define dbg_here                                            \
    if ((DBG_LEVEL) <= DBG_LOG){                            \
        rt_kprintf(DBG_SECTION_NAME " Here %s:%d\n",        \
            __FUNCTION__, __LINE__);                        \
    }

#if defined(RT_USING_SMP) && defined(ARCH_LOMBO)
#define dbg_log_line(lvl, color_n, fmt, ...)			\
	do {							\
		int print_msec = rt_time_get_msec();		\
		rt_kprintf("[%5d.%03d]\033["#color_n"m["lvl"/"DBG_SECTION_NAME"][cpu:%d][%s:%d] "fmt"\033[0m\n", \
			print_msec / 1000, print_msec % 1000, rt_hw_cpu_id(), \
			__func__, __LINE__, ##__VA_ARGS__);	\
	} while (0)
#else
#define dbg_log_line(lvl, color_n, fmt, ...)			\
	do {							\
		int print_msec = rt_time_get_msec();		\
		rt_kprintf("[%5d.%03d]\033["#color_n"m["lvl"/"DBG_SECTION_NAME"][%s:%d] "fmt"\033[0m\n", \
			print_msec / 1000, print_msec % 1000,	\
			__func__, __LINE__, ##__VA_ARGS__);	\
	} while (0)
#endif /* RT_USING_SMP && ARCH_LOMBO */

#define dbg_raw(...)         rt_kprintf(__VA_ARGS__);

#else
#define dbg_log(level, fmt, ...)
#define dbg_here
#define dbg_enter
#define dbg_exit
#define dbg_log_line(lvl, color_n, fmt, ...)
#define dbg_raw(...)
#endif /* DBG_ENABLE */

#if (DBG_LEVEL >= DBG_LOG)
#define LOG_D(fmt, ...)      dbg_log_line("D", 0, fmt, ##__VA_ARGS__)
#else
#define LOG_D(...)		do {} while (0)
#endif

#if (DBG_LEVEL >= DBG_INFO)
#define LOG_I(fmt, ...)      dbg_log_line("I", 32, fmt, ##__VA_ARGS__)
#else
#define LOG_I(...)		do {} while (0)
#endif

#if (DBG_LEVEL >= DBG_WARNING)
#define LOG_W(fmt, ...)      dbg_log_line("W", 33, fmt, ##__VA_ARGS__)
#else
#define LOG_W(...)		do {} while (0)
#endif

#if (DBG_LEVEL >= DBG_ERROR)
#define LOG_E(fmt, ...)      dbg_log_line("E", 31, fmt, ##__VA_ARGS__)
#else
#define LOG_E(...)		do {} while (0)
#endif

#define LOG_RAW(...)         do { dbg_raw(__VA_ARGS__) } while (0)

#endif /* defined(RT_USING_ULOG) && define(DBG_ENABLE) */

#endif /* RT_DBG_H__ */
