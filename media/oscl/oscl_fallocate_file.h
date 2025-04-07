/*
 * oscl_fallocate_file.h - fallocate file api used by lombo media framework.
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

#ifndef __OSCL_FALLOCATE_H__
#define __OSCL_FALLOCATE_H__

#include <pthread.h>

void *oscl_fallocate_init(void);
int oscl_fallocate_deinit(void *hdl);
int oscl_fallocate_set_filesize(void *hdl, size_t filesize);
int oscl_fallocate_open(void *hdl, const char *filename, int flag);
int oscl_fallocate_close(void *hdl, int fd);

#endif /* __OSCL_FALLOCATE_H__ */

