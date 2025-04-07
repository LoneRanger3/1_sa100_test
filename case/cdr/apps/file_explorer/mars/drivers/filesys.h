/*
 * filesys.h - file sub-system code for LomboTech
 * file sub-system interface and macro define
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

#ifndef __FILESYS_H__
#define __FILESYS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <mars/mars.h>

typedef struct tag_valid_dir {
	char valid;
	char **table;
} valid_dir_t;

lb_int32 mars_opendir(void *dir_p, const char *fn);
lb_int32 mars_closedir(void *dir_p);
lb_int32 mars_mkdir(const char *fn);
lb_int32 mars_readdir(void *dir_p, void *dirent_p);
lb_int32 mars_isdir(void *dirent_p);
lb_int32 mars_validdir(const char *fn);
lb_int32 mars_fopen(void *file_p, const char *fn, const char *flags);
lb_int32 mars_fclose(void *file_p);
lb_int32 mars_fread(void *file_p, void *buf, lb_uint32 btr, lb_uint32 *br);
lb_int32 mars_fseek(void *file_p, lb_uint32 pos);
lb_int32 mars_ftell(void *file_p, lb_uint32 *pos_p);
lb_int32 mars_remove(const char *fn);
lb_int32 mars_rename(const char *old, const char *new);
lb_int32 mars_getmtime(const char *path, time_t *st_mtime);
lb_int32 mars_getsize(const char *path, lb_int32 *st_size);

typedef FILE * mars_file;
typedef DIR * mars_dir;
typedef struct dirent *mars_dirent;

#ifdef __cplusplus
}
#endif

#endif /* __FILESYS_H__ */

