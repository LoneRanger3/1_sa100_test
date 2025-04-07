/*
 * filesys.c - file sub_system code for LomboTech
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

#include <mars/drivers/filesys.h>

char *valid_dir_table[] = {
	// "VIDEO_F",
	// "VIDEO_F_LOCK",
	// "VIDEO_R",
	// "VIDEO_R_LOCK",
	// "PICTURE",
	"F",
	"FO",
	"R",
	"RO",
	"PHOTO",
	"VIDEO_TEST",
	"cuowushipin",
	"huifangkadun",
	"huifangkasi",
	"huifangpaofei",
	"jindubudong",
	"shujubuquan",
	"tuichusiji"
};

valid_dir_t valid_dir = {
	0xff,
	valid_dir_table
};

lb_int32 mars_opendir(void *dir_p, const char *fn)
{
	mars_dir d = opendir(fn);

	if (d == (mars_dir)0)
		return -1;
	else {
		mars_dir *dp = dir_p;
		*dp = d;
	}

	return 0;
}

lb_int32 mars_closedir(void *dir_p)
{
	mars_dir *dp = dir_p;

	return closedir(*dp);
}

lb_int32 mars_mkdir(const char *fn)
{
	mars_dir d = opendir(fn);

	if (d == (mars_dir)0) {
		mkdir(fn, 0);
		return 0;
	} else {
		closedir(d);
		return -1;
	}
}

lb_int32 mars_readdir(void *dir_p, void *dirent_p)
{
	mars_dir *dp = dir_p;
	mars_dirent t = readdir(*dp);

	if (t == (mars_dirent)0)
		return -1;
	else {
		mars_dirent *dtp = dirent_p;
		*dtp = t;
	}

	return 0;
}

lb_int32 mars_isdir(void *dirent_p)
{
	mars_dirent *dtp = dirent_p;
	mars_dirent t = *dtp;

	if (t->d_type == DT_DIR)
		return 1;
	else if (t->d_type == DT_REG)
		return 0;

	return -1;
}

lb_int32 mars_validdir(const char *fn)
{
	char *name = NULL;
	lb_int32 ret = 0;
	lb_uint8 i = 0;

	if (valid_dir.valid != 0xff)
		return 1;

	name = strrchr(fn, '/') + 1;
	if (name == NULL)
		return 0;

	for (i = 0; i < sizeof(valid_dir_table) / sizeof(char *); i++) {
		if (valid_dir.table[i] != NULL) {
			ret = strcmp(name, valid_dir.table[i]);
			if (ret == 0)
				return 1;
		}
	}

	return 0;
}

lb_int32 mars_getmtime(const char *path, time_t *st_mtime)
{
	struct stat s;
	lb_int32 ret = 0;

	ret = stat(path, &s);

	/* printf("s.st_mtime:%lld\n", s.st_mtime); */
	if (ret == 0)
		*st_mtime = s.st_mtime;
	else
		*st_mtime = 0;

	return 0;
}

lb_int32 mars_getsize(const char *path, lb_int32 *st_size)
{
	struct stat s;
	lb_int32 ret = 0;

	ret = stat(path, &s);

	/* printf("s.st_size:%ld\n", s.st_size); */
	if (ret == 0)
		*st_size = s.st_size;
	else
		*st_size = 0;

	return 0;
}

lb_int32 mars_fopen(void *file_p, const char *fn, const char *flags)
{
	mars_file f = fopen(fn, flags);

	if (f == (mars_file)0)
		return -1;
	else {
		fseek(f, 0, SEEK_SET);
		mars_file *fp = file_p;
		*fp = f;
	}

	return 0;
}

lb_int32 mars_fclose(void *file_p)
{
	mars_file *fp = file_p;

	if (*fp == NULL)
		return -1;

	fclose(*fp);
	*fp = NULL;

	return 0;
}

lb_int32 mars_fread(void *file_p, void *buf, lb_uint32 btr, lb_uint32 *br)
{
	mars_file *fp = file_p;

	if (*fp == NULL)
		return -1;

	*br = fread(buf, 1, btr, *fp);

	return 0;
}

lb_int32 mars_fseek(void *file_p, lb_uint32 pos)
{
	mars_file *fp = file_p;

	if (*fp == NULL)
		return -1;

	fseek(*fp, pos, SEEK_SET);

	return 0;
}

lb_int32 mars_ftell(void *file_p, lb_uint32 *pos_p)
{
	mars_file *fp = file_p;

	if (*fp == NULL)
		return -1;

	*pos_p = ftell(*fp);

	return 0;
}

lb_int32 mars_remove(const char *fn)
{
	lb_int32 ret = 0;

	if (fn == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	if (remove(fn) != 0) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

exit:
	return ret;
}

lb_int32 mars_rename(const char *old, const char *new)
{
	lb_int32 ret = 0;

	if (old == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	if (new == NULL) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	if (strcmp(old, new) == 0) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

	if (rename(old, new) != 0) {
		printf("[%s,%d]\n", __func__, __LINE__);
		ret = -1;
		goto exit;
	}

exit:
	return ret;
}
