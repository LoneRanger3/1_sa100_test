/*
 * oscl_cache_file.h - cache file api used by lombo media framework.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef __OSCL_CFILE_H__
#define __OSCL_CFILE_H__

int oscl_cfile_open(const char *file, int flags);
int oscl_cfile_close(int fd);
off_t oscl_cfile_lseek(int fd, off_t offset, int whence);
int oscl_cfile_read(int fd, void *buf, size_t len);
int oscl_cfile_write(int fd, const void *buf, size_t len);
void oscl_cache_fs_init(void);
void oscl_cache_fs_deinit(void);
int oscl_cfile_realloc_cache(int fd, int num, int size);
int oscl_cfile_set_prio(int fd, int prio);

#endif /* __OSCL_CFILE_H__ */

