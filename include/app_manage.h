/*
 * file app_manage.h - Common code for LomboTech Socs
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



#ifndef APP_MANAGE_H
#define APP_MANAGE_H
#ifdef __cplusplus
extern "C" {
#endif
/*********************
 *      INCLUDES
 *********************/
#include <string.h>
#include <stdio.h>
#include <lb_types.h>

/* APP DEBUG level */
#define APP_DBG_ERROR           0
#define APP_DBG_WARNING         1
#define APP_DBG_INFO            2
#define APP_DBG_LOG             3

#ifndef APP_DBG_LEVEL
#define APP_DBG_LEVEL APP_DBG_WARNING
#endif

#if __cplusplus < 201103L
#define app_dbg_log_line(lvl, color_n, fmt, ...) \
			rt_kprintf("\033["#color_n"m["lvl"][%s:%d]: "fmt"\033[0m",\
			__func__, __LINE__, ##__VA_ARGS__)
#else
#define app_dbg_log_line(lvl, color_n, fmt, ...) \
			rt_kprintf("\033["#color_n"m[" lvl"][%s:%d]: " fmt"\033[0m",\
			__func__, __LINE__, ##__VA_ARGS__)
#endif

#if (APP_DBG_LEVEL >= APP_DBG_LOG)
#define APP_LOG_D(fmt, ...)      app_dbg_log_line("app_dbg", 0, fmt, ##__VA_ARGS__)
#else
#define APP_LOG_D(...)
#endif

#if (APP_DBG_LEVEL >= APP_DBG_INFO)
#define APP_LOG_I(fmt, ...)      app_dbg_log_line("app_inf", 32, fmt, ##__VA_ARGS__)
#else
#define APP_LOG_I(...)
#endif

#if (APP_DBG_LEVEL >= APP_DBG_WARNING)
#define APP_LOG_W(fmt, ...)      app_dbg_log_line("app_wrn", 33, fmt, ##__VA_ARGS__)
#else
#define APP_LOG_W(...)
#endif

#if (APP_DBG_LEVEL >= APP_DBG_ERROR)
#define APP_LOG_E(fmt, ...)      app_dbg_log_line("app_err", 31, fmt, ##__VA_ARGS__)
#else
#define APP_LOG_E(...)
#endif

/**********************
 *      TYPEDEFS
 **********************/
typedef struct _app_attr {
	lb_uint8 stanby_flag;
	lb_uint8 support_type_num;
	char *support_type_name[32];
} app_attr_t;

typedef struct _app {
	lb_int32 ah;
	lb_int32 status;
	lb_int32 app_type;
	char *app_name;
	char version[16];
	char magic[16];
	app_attr_t app_attr;
	void *appx_if;
	void *userdata;
} app_t;

typedef lb_int32(*app_start)(app_t *ap);
typedef lb_int32(*app_stop)(app_t *ap);
typedef lb_int32(*app_suspend)(app_t *ap);
typedef lb_int32(*app_resume)(app_t *ap);
typedef lb_int32(*app_ctrl)(app_t *ap, lb_uint32 cmd, lb_int32 aux0, void *aux1);

typedef struct app_if {
	app_start	start;
	app_stop	stop;
	app_suspend	suspend;
	app_resume	resume;
	app_ctrl	ctrl;
} app_if_t;
typedef struct app_manage {
	app_t  *appx;
	struct app_manage *next;
} app_manage_t;


/**********************
 * GLOBAL PROTOTYPES
 **********************/
lb_int32 lb_app_open(app_t *ap, char *app_path, lb_int32 mode);
lb_int32 lb_app_close(app_t *ap);
lb_int32 lb_app_suspend(app_t *ap);
lb_int32 lb_app_resume(app_t *ap);
lb_int32 lb_app_ctrl(app_t *ap, lb_uint32 cmd, lb_int32 aux0, void *aux1);
void lb_app_dump();
app_t *lb_app_check(char *app_name);
app_if_t *lb_app_get_if(app_t *ap);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*APP_MANAGE_H*/
