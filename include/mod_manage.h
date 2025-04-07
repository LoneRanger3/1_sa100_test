/*
 * file mod_manage.h - Common code for LomboTech Socs
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



#ifndef MOD_MANAGE_H
#define MOD_MANAGE_H


#ifdef __cplusplus
extern "C" {
#endif
/*********************
 *      INCLUDES
 *********************/
#include <string.h>
#include <stdio.h>
#include <lb_types.h>

/* MOD DEBUG level */
#define MOD_DBG_ERROR           0
#define MOD_DBG_WARNING         1
#define MOD_DBG_INFO            2
#define MOD_DBG_LOG             3

#ifndef MOD_DBG_LEVEL
#define MOD_DBG_LEVEL MOD_DBG_WARNING
#endif

#if __cplusplus < 201103L
#define mod_dbg_log_line(lvl, color_n, fmt, ...) \
			rt_kprintf("\033["#color_n"m["lvl"][%s:%d]: "fmt"\033[0m",\
			__func__, __LINE__, ##__VA_ARGS__)
#else
#define mod_dbg_log_line(lvl, color_n, fmt, ...) \
			rt_kprintf("\033["#color_n"m[" lvl"][%s:%d]: " fmt"\033[0m",\
			__func__, __LINE__, ##__VA_ARGS__)
#endif

#if (MOD_DBG_LEVEL >= MOD_DBG_LOG)
#define MOD_LOG_D(fmt, ...)      mod_dbg_log_line("mod_dbg", 0, fmt, ##__VA_ARGS__)
#else
#define MOD_LOG_D(...)
#endif

#if (MOD_DBG_LEVEL >= MOD_DBG_INFO)
#define MOD_LOG_I(fmt, ...)      mod_dbg_log_line("mod_inf", 32, fmt, ##__VA_ARGS__)
#else
#define MOD_LOG_I(...)
#endif

#if (MOD_DBG_LEVEL >= MOD_DBG_WARNING)
#define MOD_LOG_W(fmt, ...)      mod_dbg_log_line("mod_wrn", 33, fmt, ##__VA_ARGS__)
#else
#define MOD_LOG_W(...)
#endif

#if (MOD_DBG_LEVEL >= MOD_DBG_ERROR)
#define MOD_LOG_E(fmt, ...)      mod_dbg_log_line("mod_err", 31, fmt, ##__VA_ARGS__)
#else
#define MOD_LOG_E(...)
#endif

/**********************
 *      TYPEDEFS
 **********************/
typedef struct _mod_attr {
	lb_uint8 support_format_num;
	char *support_format_name[32];
} mod_attr_t;

typedef struct _mod {
	lb_int32 mh;
	lb_int32 mod_type;
	char *mod_name;
	char version[16];
	char magic[16];
	mod_attr_t mod_attr;
	void *modx_if;
	void *userdata;
} mod_t;

typedef lb_int32(*mod_init)(mod_t *mp);
typedef lb_int32(*mod_exit)(mod_t *mp);
typedef lb_int32(*mod_read)(mod_t *mp, void *mdata, lb_uint32 size, lb_uint32 n);
typedef lb_int32(*mod_write)(mod_t *mp, const void *mdata, lb_uint32 size, lb_uint32 n);
typedef lb_int32(*mod_ctrl)(mod_t *mp, lb_uint32 cmd, lb_int32 aux0, void *aux1);

typedef struct mod_if {
	mod_init	init;
	mod_exit	exit;
	mod_read	read;
	mod_write	 write;
	mod_ctrl	ctrl;
} mod_if_t;
typedef struct mod_manage {
	mod_t  *modx;
	struct mod_manage *next;
} mod_manage_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/
lb_int32 lb_mod_open(mod_t *mp, const char *mod_path, lb_uint8 mode);
lb_int32 lb_mod_close(mod_t *mp);
lb_int32 lb_mod_read(mod_t *mp, void *mdata, lb_uint32 size, lb_uint32 n);
lb_int32 lb_mod_write(mod_t *mp, const void *mdata, lb_uint32 size, lb_uint32 n);
lb_int32 lb_mod_ctrl(mod_t *mp, lb_uint32 cmd, lb_int32 aux0, void *aux1);
void lb_mod_dump();
mod_if_t *lb_mod_get_if(mod_t *mp);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*MOD_MANAGE_H*/
