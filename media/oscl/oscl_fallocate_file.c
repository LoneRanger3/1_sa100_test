/*
 * oscl_fallocate_file.c - fallocate file api used by lombo media framework.
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

#define DBG_LEVEL		DBG_WARNING

#include <dfs_posix.h>
#include <oscl.h>
#include "oscl_fallocate_file.h"

/* #define FALLOCATE_TIME_DBG */

#define IO_FALLOCATE	0xfa

#define FALLOCATE_MIN_SIZE		(1*1024*1024)
#define FALLOCATE_RESERVE_SIZE		(8*1024*1024)

#define TEMP_FILE_PATH		"/mnt/sdcard/.tmp"
#define FILE_NAME_MAX_LENTH	512
#define MAX_TMP_FILE_NUM	2

typedef enum file_state {
	STAT_INVALID = 0,
	STAT_ALLOCATED,
	STAT_USING,
} file_state_e;

typedef struct fallocate_file {
	char tmp_filename[64];
	char filename[FILE_NAME_MAX_LENTH];
	int fd;
	file_state_e state;
} fallocate_file_t;

typedef struct oscl_fallocate {
	size_t filesize;
	pthread_t thread;
	sem_t need_alloc_sem;
	fallocate_file_t tmp_file[MAX_TMP_FILE_NUM];
	int thread_exit;
} oscl_fallocate_t;

static pthread_mutex_t g_mutex;

static __attribute__((constructor)) void fallocate_init(void)
{
	OSCL_LOGD("");
	pthread_mutex_init(&g_mutex, NULL);
}

#if 0
static inline void check_temp_file_and_rm(const char *filename)
{
	if (access(filename, F_OK) == 0) {
		OSCL_LOGD("unlink file \"%s\"", filename);
		if (unlink(filename) == -1) {
			OSCL_LOGE("Failed to unlink temp file \"%s\"!",
				filename);
		}
	}
}
#endif

static inline int need_alloc_file(oscl_fallocate_t *fallocate)
{
	int i;
	if (fallocate->filesize == 0)
		return 0;
	for (i = 0; i < MAX_TMP_FILE_NUM; ++i) {
		if (fallocate->tmp_file[i].state == STAT_INVALID)
			return 1;
	}
	return 0;
}

static void *oscl_fallocate_thread(void *param)
{
	oscl_fallocate_t *fallocate = param;
	int fd = -1;
	int i, j;
	struct stat st;
	static int s_running_cnt;
	static u32 s_using_flag;

	OSCL_LOGD("start");

	pthread_mutex_lock(&g_mutex);
	for (i = 0; i <= s_running_cnt; ++i) {
		if (!(s_using_flag & (1 << i)))
			break;
	}
	s_using_flag |= (1 << i);
	++s_running_cnt;
	if (s_running_cnt > 32) {
		OSCL_LOGW("%d fallocate thread is already running!", s_running_cnt);
		fallocate->thread_exit = 1;
	}
	pthread_mutex_unlock(&g_mutex);

	if (stat(TEMP_FILE_PATH, &st) != 0 || !S_ISDIR(st.st_mode))
		mkdir(TEMP_FILE_PATH, 0777);
	for (j = 0; j < MAX_TMP_FILE_NUM; ++j) {
		snprintf(fallocate->tmp_file[j].tmp_filename,
			sizeof(fallocate->tmp_file[j].tmp_filename),
			"%s/fallocate_%d_%d.tmp", TEMP_FILE_PATH, i, j);
		OSCL_LOGD("temp file %s", fallocate->tmp_file[j].tmp_filename);
		if (access(fallocate->tmp_file[j].tmp_filename, F_OK) == 0)
			fallocate->tmp_file[j].state = STAT_ALLOCATED;
		else
			fallocate->tmp_file[j].state = STAT_INVALID;
		fallocate->tmp_file[j].fd = -1;
	}

	while (fallocate->thread_exit == 0) {
		if (!need_alloc_file(fallocate)) {
			sem_wait(&fallocate->need_alloc_sem);
			continue;
		}

		for (j = 0; j < MAX_TMP_FILE_NUM; ++j) {
			if (fallocate->tmp_file[j].state == STAT_INVALID)
				break;
		}
		if (j >= MAX_TMP_FILE_NUM)
			continue;

		fallocate_file_t *al_file = &fallocate->tmp_file[j];
		if (stat(al_file->tmp_filename, &st) == 0) {
			if (st.st_size != fallocate->filesize) {
				OSCL_LOGD("st_size(%d)!=alloc_size(%d), unlink file %s",
					st.st_size,
					fallocate->filesize,
					al_file->tmp_filename);
				unlink(al_file->tmp_filename);
			} else {
				al_file->state = STAT_ALLOCATED;
				continue;
			}
		}

		#ifdef FALLOCATE_TIME_DBG
		int time = oscl_get_msec();
		#endif
		fd = open(al_file->tmp_filename, O_RDWR | O_CREAT);
		if (fd < 0) {
			OSCL_LOGE("Failed to open file %s!",
				al_file->tmp_filename);
			continue;
		}
		if (ioctl(fd, IO_FALLOCATE, &fallocate->filesize) < 0) {
			OSCL_LOGE("Failed IO_FALLOCATE file %s (%d)!",
				al_file->tmp_filename, errno);
		}
		close(fd);
		al_file->state = STAT_ALLOCATED;
		#ifdef FALLOCATE_TIME_DBG
		OSCL_LOGD("fallocate file %s use %d ms",
		al_file->tmp_filename, oscl_get_msec()-time);
		#endif
	}

	/* check_temp_file_and_rm(fallocate->tmp_filename); */
	pthread_mutex_lock(&g_mutex);
	s_using_flag &= ~(1 << i);
	--s_running_cnt;
	pthread_mutex_unlock(&g_mutex);
	OSCL_LOGD("end");
	pthread_exit(NULL);
	return NULL;
}

void *oscl_fallocate_init(void)
{
	oscl_fallocate_t *fallocate;
	pthread_attr_t thread_attr;
	int i;
	int ret;

	OSCL_LOGD("start");

	fallocate = oscl_zalloc(sizeof(oscl_fallocate_t));
	if (fallocate == NULL) {
		OSCL_LOGE("alloc oscl_fallocate_t failed!");
		return NULL;
	}

	/* fallocate size must divisible by 1MB */
	fallocate->filesize = 0;
	fallocate->thread_exit = 0;

	for (i = 0; i < MAX_TMP_FILE_NUM; ++i) {
		fallocate->tmp_file[i].fd = -1;
		fallocate->tmp_file[i].state = STAT_INVALID;
	}

	ret = sem_init(&fallocate->need_alloc_sem, 0, 0);
	if (ret != 0) {
		OSCL_LOGE("sem_init error!");
		goto ERR_EXIT0;
	}

	pthread_attr_init(&thread_attr);
	pthread_attr_setstacksize(&thread_attr, 0x1000);
	ret = pthread_create(&fallocate->thread, &thread_attr,
		oscl_fallocate_thread, fallocate);
	if (ret != 0) {
		OSCL_LOGE("create oscl_fallocate_thread failed!");
		goto ERR_EXIT1;
	}

	OSCL_LOGD("end");

	return fallocate;

ERR_EXIT1:
	sem_destroy(&fallocate->need_alloc_sem);
ERR_EXIT0:
	oscl_free(fallocate);
	return NULL;
}

int oscl_fallocate_deinit(void *hdl)
{
	oscl_fallocate_t *fallocate = (oscl_fallocate_t *)hdl;
	OSCL_LOGD("start");

	fallocate->thread_exit = 1;
	sem_post(&fallocate->need_alloc_sem);
	pthread_join(fallocate->thread, NULL);
	sem_destroy(&fallocate->need_alloc_sem);

	oscl_free(fallocate);

	OSCL_LOGD("end");

	return 0;
}

int oscl_fallocate_set_filesize(void *hdl, size_t filesize)
{
	oscl_fallocate_t *fallocate = (oscl_fallocate_t *)hdl;

	OSCL_LOGD("%u", filesize);
	if (filesize == 0)
		return 0;
	if (filesize < FALLOCATE_MIN_SIZE)
		filesize = FALLOCATE_MIN_SIZE;
	/* fallocate size must divisible by 1MB */
	filesize = ((filesize + (1<<20) - 1) >> 20) << 20;
	filesize += FALLOCATE_RESERVE_SIZE;
	if (fallocate->filesize == filesize)
		return 0;
	OSCL_LOGD("set size to %u", filesize);
	/* fallocate size must divisible by 1MB */
	fallocate->filesize = filesize;
	sem_post(&fallocate->need_alloc_sem);
	return 0;
}

int oscl_fallocate_close(void *hdl, int fd)
{
	oscl_fallocate_t *fallocate = (oscl_fallocate_t *)hdl;
	fallocate_file_t *al_file = NULL;
	int i;
	int ret;

	close(fd);

	for (i = 0; i < MAX_TMP_FILE_NUM; ++i) {
		if (fallocate->tmp_file[i].state == STAT_USING &&
			fallocate->tmp_file[i].fd == fd)
			al_file = &fallocate->tmp_file[i];
	}
	if (NULL == al_file) {
		OSCL_LOGW("no fallocate file!");
		goto EXIT;
	}
	OSCL_LOGD("rename file %s to %s", al_file->tmp_filename, al_file->filename);
	ret = rename(al_file->tmp_filename, al_file->filename);
	if (ret < 0) {
		OSCL_LOGW("Failed to rename file %s to %s (%d)!",
			al_file->tmp_filename, al_file->filename, errno);
	} else {
		al_file->state = STAT_INVALID;
		al_file->fd = -1;
	}

EXIT:
	sem_post(&fallocate->need_alloc_sem);
	return 0;
}

int oscl_fallocate_open(void *hdl, const char *filename, int flag)
{
	oscl_fallocate_t *fallocate = (oscl_fallocate_t *)hdl;
	fallocate_file_t *al_file = NULL;
	int fd;
	int i;

	#ifdef FALLOCATE_TIME_DBG
	int time = oscl_get_msec();
	#endif
	for (i = 0; i < MAX_TMP_FILE_NUM; ++i) {
		if (fallocate->tmp_file[i].state == STAT_ALLOCATED) {
			al_file = &fallocate->tmp_file[i];
			break;
		}
	}

	if (al_file != NULL) {
		OSCL_LOGD("opening file %s", al_file->tmp_filename);
		fd = open(al_file->tmp_filename, flag);
		if (fd >= 0) {
			al_file->state = STAT_USING;
			al_file->fd = fd;
			strncpy(al_file->filename, filename,
				FILE_NAME_MAX_LENTH - 1);
		} else
			OSCL_LOGE("Failed to open file %s!", al_file->tmp_filename);
	} else {
		OSCL_LOGD("opening file %s", filename);
		fd = open(filename, flag);
		if (fd < 0)
			OSCL_LOGE("Failed to open file %s!", filename);
	}
	#ifdef FALLOCATE_TIME_DBG
	OSCL_LOGD("open file use %d ms", oscl_get_msec()-time);
	#endif
	return fd;
}
