/*
 * mars.h -  file man_system code for LomboTech
 * file man_system interface and macro define
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

#ifndef __MARS_H__
#define __MARS_H__

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <rtthread.h>
#include "system/system.h"
#include "lb_types.h"
#include "case_config.h"

#include <mars/drivers/mem.h>
#include <mars/drivers/filesys.h>

/* #define QUICK_RESOTRE */
#define DATABASE_OMIT

#define MAX_FILE_LIST 500
#define MIN_FILE_LIST 100
#define MARS_FD_PATH SDCARD_MOUNT_PATH
#define MARS_DB_PATH SDCARD_MOUNT_PATH"/DATA_DB/"
#define MARS_TB_VIDEO_PATH SDCARD_MOUNT_PATH"/THUMB_VIDEO_DB/"
#define MARS_TB_IMAGE_PATH SDCARD_MOUNT_PATH"/THUMB_IMAGE_DB/"
#define MARS_TB_OTHER_PATH SDCARD_MOUNT_PATH"/THUMB_OTHER_DB/"

typedef enum {
	SORT_ASCENDING = 0x00,
	SORT_DESCENDING = 0x01,
	SORT_EXTENSION = 0x02,
} sort_type_t;

typedef enum {
	MEDIA_ALL = 0x00,
	MEDIA_PIC = 0x01,
	MEDIA_AUDIO = 0x02,
	MEDIA_VIDEO = 0x03,
	MEDIA_EBOOK = 0x04,
	MEDIA_UNKNOWN = 0x05,
} media_type_t;

typedef enum {
	NODE_ALL = 0x00,
	NODE_DIR = 0x01,
	NODE_FILE = 0x02,
	NODE_UNKNOWN = 0x03,
} node_type_t;

typedef enum {
	RECURSIVE = 0x00,
	NON_RECUR = 0x01,
} query_type_t;

typedef enum {
	LOCKED = 0x00,
	UNLOCK = 0x01,
	ALTYPE = 0x02,
} lock_type_t;

typedef struct tag_list_conf {
	media_type_t me_type;
	node_type_t nd_type;
	query_type_t qu_type;
	sort_type_t st_type;
	lock_type_t lk_type;
	char path[256];
} list_conf_t;

typedef struct tag_list_node {
	lb_uint32 index;
	/* To store the type of lock maybe LOCK/UNLOCK */
	lock_type_t lk_type;
	/* To store the type of media maybe JPEG/MP4 */
	media_type_t me_type;
	/* To store the type of node maybe DIR/FILE */
	node_type_t nd_type;
	/* Pointer to the path of node of the link */
	char *path;
	/* Pointer to the thumb of node of the link */
	char *thumb;
	/* Pointer to the name of node of the link */
	char *name;
	/* Pointer to the suffix of node of the link */
	char *suffix;
} list_node_t;

typedef struct tag_file_list {
	void *mars;
	/* An array to store the node of the link */
	list_node_t *list_node;
	/* Total num of array above */
	lb_uint32 node_num;
	/* Node needed for recursive processing */
	list_node_t *temp_node;
	/* An array to store by this sort */
	sort_type_t st_type;
	/* To store the type of media maybe JPEG/MP4 */
	media_type_t me_type;
	/* To store the type of node maybe DIR/FILE */
	node_type_t nd_type;
	/* Queries can be two of types recursive and non */
	query_type_t qu_type;
	/* Queries can be lock and unlock */
	lock_type_t lk_type;

	/* Active num of array above */
	lb_uint32 act_num;
	/* File num of array above */
	lb_uint32 file_num;
	/* Dir num of array above */
	lb_uint32 dir_num;

	char *par_path;
	char *cur_path;
	void *par_node;
	void *cur_node;
} file_list_t;

/**
 * mars_init - Initiallizes the file management module mars
 * @path: file path to manage
 *
 * returns 0 means successed,others mean failed
 *
 */
lb_int32 mars_init(const char *path);

/**
 * mars_exit - Exit the file management module mars
 *
 * returns 0 means successed
 *
 */
lb_int32 mars_exit(void);

/**
 * mars_set_path - Sets the path to the list to be appiled
 * @path: the path to the list
 *
 * returns 0 means successed,-1 means failed
 *
 */
lb_int32 mars_set_path(char *path);

/**
 * mars_set_me_conf - Sets the media type to the list to be appiled
 * @me_type: the media type to the list
 *
 * returns 0 means successed,-1 means failed
 *
 */
lb_int32 mars_set_media(media_type_t me_type);

/**
 * mars_set_node Sets - the node type to the list to be appiled
 * @nd_type: the node type to the list
 *
 * returns 0 means successed,-1 means failed
 *
 */
lb_int32 mars_set_node(node_type_t nd_type);

/**
 * mars_set_query - Sets the query type to the list to be appiled
 * @qu_type: the query type to the list
 *
 * returns 0 means successed,-1 means failed
 *
 */
lb_int32 mars_set_query(query_type_t qu_type);

/**
 * mars_set_sort - Sets the sort type to the list to be appiled
 * @st_type: the sort type to the list
 *
 * returns 0 means successed,-1 means failed
 *
 */
lb_int32 mars_set_sort(sort_type_t st_type);

/**
 * mars_set_lock - Sets the lock type to the list to be appiled
 * @st_type: the lock type to the list
 *
 * returns 0 means successed,-1 means failed
 *
 */
lb_int32 mars_set_lock(lock_type_t lk_type);

/**
 * mars_alloc_list - Alloc a list of files
 *
 * returns non-null means successed,null means failed
 *
 */
lb_int32 mars_alloc_list(void **desert);

/**
 * mars_free_list - Free a list of files
 * @desert: the list pointer to be freed
 *
 * returns 0 means successed,-1 means failed
 *
 */
lb_int32 mars_free_list(void *desert);

/**
 * mars_get_node_num - Get the total numbers of list items
 * @desert: the list pointer to be get
 *
 * returns -1 means failed,others mean successed
 *
 */
lb_int32 mars_get_node_num(void *desert);

/**
 * mars_get_node_lock - Get the lock type of the item
 * @desert: the list pointer to be get
 * @index:  the index of item
 *
 * returns UNKNOWN means failed,others mean successed
 *
 */
lock_type_t mars_get_node_lock(void *desert, lb_uint32 index);


/**
 * mars_get_node_media - Get the media type of the item
 * @desert: the list pointer to be get
 * @index:  the index of item
 *
 * returns MEDIA_UNKNOWN means failed,others mean successed
 *
 */
media_type_t mars_get_node_media(void *desert, lb_uint32 index);

/**
 * mars_get_node_type - Get the node type of the item
 * @desert: the list pointer to be get
 * @index:  the index of item
 *
 * returns NODE_UNKNOWN means failed,others mean successed
 *
 */
node_type_t mars_get_node_type(void *desert, lb_uint32 index);

/**
 * mars_get_node_path - Get the node path of the item
 * @desert: the list pointer to be get
 * @index:  the index of item
 * @path:   the secondary pointer of the path
 *
 * returns 0 means successed,-1 means failed
 *
 */
lb_int32 mars_get_node_path(void *desert, lb_uint32 index, char **path);

/**
 * mars_get_node_thumb - Get the node thumb of the item
 * @desert: the list pointer to be get
 * @index:  the index of item
 * @thumb:   the secondary pointer of the thumb
 *
 * returns 0 means successed,-1 means failed
 *
 */
lb_int32 mars_get_node_thumb(void *desert, lb_uint32 index, char **thumb);

/**
 * mars_get_node_name  - Get the node name of the item
 * @desert: the list pointer to be get
 * @index:  the index of item
 * @name:   the secondary pointer of the name
 *
 * returns 0 means successed,-1 means failed
 *
 */
lb_int32 mars_get_node_name(void *desert, lb_uint32 index, char **name);

/**
 * mars_get_current_path - Get the current path of the list
 * @desert: the list pointer to be get
 * @path:   the secondary pointer of the path
 *
 * returns 0 means successed,-1 means failed
 *
 */
lb_int32 mars_get_current_path(void *desert, char **path);

/**
 * mars_get_parent_path - Get the parent path of the list
 * @desert: the list pointer to be get
 * @path:   the secondary pointer of the path
 *
 * returns 0 means successed,-1 means failed
 *
 */
lb_int32 mars_get_parent_path(void *desert, char **path);

/**
 * mars_set_node_lock - set the lock type of the item
 * @desert: the list pointer to be set
 * @index:  the index of the item
 * @lock:   the lock type of the item
 *
 * returns 0 means successed,-1 means failed
 *
 */
lb_int32 mars_set_node_lock(void *desert, lb_uint32 index, lock_type_t lock);

/**
 * mars_create_file - Create the file through mars
 * @desert: the list pointer in which create the file
 * @name:   the file name to be created
 * @flags:  the open mode of file
 *
 * returns 0 means successed,-1 means failed
 *
 */
lb_int32 mars_create_file(void *desert, const char *name, const char *flags);

/**
 * mars_delete_file - Delete the file through mars
 * @desert: the list pointer in which delete the file
 * @name:   the file name to be deleted
 *
 * returns 0 means successed,-1 means failed
 *
 */
lb_int32 mars_delete_file(void *desert, const char *name);

/**
 * mars_rename_file - Rename the file through mars
 * @desert: the list pointer in which Rename the file
 * @name:   the file name to be Renamed
 *
 * returns 0 means successed,-1 means failed
 *
 */
lb_int32 mars_rename_file(void *desert, const char *oldf, const char *newf);
#endif /* __MARS_H__ */

