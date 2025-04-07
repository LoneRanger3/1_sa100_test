/*
 * car_recorder_file.c - car recorder module file manager
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

#include "eos.h"
#include <dfs_posix.h>
#include "car_recorder.h"
#include "car_recorder_common.h"
#include "lb_cfg_file.h"
#include "lb_types.h"

/* #define RENAME_AFTER_REC */
#define IO_FALLOCATE	0xfa
#define IO_FTRUNCATE	0xfb

#define CDR_FILENAME_LEN 64

typedef struct filename_map {
	char tmp_file[CDR_FILENAME_LEN];
#ifdef MEDIA_TYPE_TS
	char ts_file[CDR_FILENAME_LEN];
#else
	char mp4_file[CDR_FILENAME_LEN];
#endif
} filename_map_t;

/* recorder file manager struct */
typedef struct car_rec_file_manager {
	sem_t sem;
	int time_wait;
	sem_t wait_sem;
	pthread_mutex_t lock;
	pthread_t thread; /* recorder file manager thread id */
	int thread_exit;
	int resize;

	int r_bitrate;
	int r_index; /* rear temp recorder file index */
	char r_tmp_file[CDR_FILENAME_LEN];
	int r_size;
	sem_t r_sem;

	int f_bitrate;
	int f_index;/* front temp recorder file index */
	char f_tmp_file[CDR_FILENAME_LEN]; /* front temp recorder file name */
	int f_size;
	sem_t f_sem;
#ifdef RENAME_AFTER_REC
	filename_map_t r_map[2];
	filename_map_t f_map[2];
#endif
} car_rec_file_manager_t;

#define FALLOC_RESERVE		(15*1024*1024)
#define get_falloc_size(bitrate, time) \
	((bitrate >> 3) * (time) + ((time)/60) * (FALLOC_RESERVE))
#define falloc_align(n) (((n + (1<<20) - 1) >> 20) << 20)
#ifdef MEDIA_TYPE_TS
#define is_size_chg(path, file, dur, res) \
	(file[0] && (_get_file_chg(path, file, dur, res)))
#else
#define is_size_chg(path, file, size) \
	(file[0] && (_get_file_size(path, file) != size))
#endif
#define set_tmp_filename(file, path, index) \
	sprintf(file, "%s.%02d.tmp", path, index);

#define rec_namecmp(f1, f2) strncmp(f1, f2, 14)

#define _get_full_name(path, sname, fname) \
{ \
	int len; \
	fname[CDR_FILENAME_LEN - 1] = 0; \
	strncpy(fname, path, CDR_FILENAME_LEN - 1);\
	len = strlen(fname); \
	strncpy(fname + len, sname, CDR_FILENAME_LEN - len - 1); \
}
#if 1//jiasuofenqu
pthread_mutex_t lock_file_mutex;
#endif
#ifdef MEDIA_TYPE_TS
static int _get_file_chg(char *path, char *file, int dur, int res)
{
	char tmp_file[CDR_FILENAME_LEN];

	_get_full_name(path, file, tmp_file);
	if (strstr(path, "_R") != NULL)
		res = 0;
	if ((strstr(tmp_file, "10_") != NULL) && (dur == 1) && (res == 0))
		return 0;
	else if ((strstr(tmp_file, "11_") != NULL) && (dur == 1) && (res == 1))
		return 0;
	else if ((strstr(tmp_file, "20_") != NULL) && (dur == 2) && (res == 0))
		return 0;
	else if (strstr(tmp_file, "21_") != NULL && (dur == 2) && (res == 1))
		return 0;

	return 1;
}
#else
static int _get_file_size(char *path, char *file)
{
	struct stat st;
	int size = 0;
	char tmp_file[CDR_FILENAME_LEN];

	_get_full_name(path, file, tmp_file);
	if (stat(tmp_file, &st) == 0)
		size = st.st_size;

	return size;
}
#endif
u32 _get_disk_info(char *path, u64 *used_mbsize, u64 *free_mbsize)
{
	struct statfs disk_info;
	int err = 0;

	if (get_sd_status() != SDCARD_PLUGIN)
		return -1;

	err = statfs(path, &disk_info);
	if (err) {
		APP_LOG_E("failed to get disk infomation\n");
		return -1;
	}
	*used_mbsize = (((u64)(disk_info.f_blocks - disk_info.f_bfree)) * ((
					u64)disk_info.f_bsize)) >> 20;
	*free_mbsize = (((u64)disk_info.f_bfree) * ((u64)disk_info.f_bsize)) >> 20;

	return err;
}

/**
 * _check_tmp_file - check tmp file
 * @manager: file manager handle
 *
 * This function check temp file give to recorder, or delete change size temp file
 *
 */
static void _check_tmp_file(car_rec_file_manager_t *manager)
{
	struct stat st;
	char tmp_file[CDR_FILENAME_LEN];
	int ret = 0;

	pthread_mutex_lock(&cr_mutex);
	set_tmp_filename(tmp_file, REAR_PATH, 0);
	if (stat(tmp_file, &st) == 0) {
		if (manager->r_size == st.st_size) {
			pthread_mutex_lock(&manager->lock);
			strcpy(manager->r_tmp_file, tmp_file);
			sem_post(&manager->r_sem);
			pthread_mutex_unlock(&manager->lock);
		} else {
			ret = unlink(tmp_file);
			if (ret < 0)
				APP_LOG_E("unlink %s failed!", tmp_file);
		}
	}
	set_tmp_filename(tmp_file, REAR_PATH, 1);
	if (stat(tmp_file, &st) == 0) {
		if (manager->r_size == st.st_size) {
			pthread_mutex_lock(&manager->lock);
			strcpy(manager->r_tmp_file, tmp_file);
			sem_post(&manager->r_sem);
			pthread_mutex_unlock(&manager->lock);
		} else {
			ret = unlink(tmp_file);
			if (ret < 0)
				APP_LOG_E("unlink %s failed!", tmp_file);
		}
	}

	set_tmp_filename(tmp_file, FRONT_PATH, 0);
	if (stat(tmp_file, &st) == 0) {
		if (manager->f_size == st.st_size) {
			pthread_mutex_lock(&manager->lock);
			strcpy(manager->f_tmp_file, tmp_file);
			sem_post(&manager->f_sem);
			pthread_mutex_unlock(&manager->lock);
		} else {
			ret = unlink(tmp_file);
			if (ret < 0)
				APP_LOG_E("unlink %s failed!", tmp_file);
		}
	}
	set_tmp_filename(tmp_file, FRONT_PATH, 1);
	if (stat(tmp_file, &st) == 0) {
		if (manager->f_size == st.st_size) {
			pthread_mutex_lock(&manager->lock);
			strcpy(manager->f_tmp_file, tmp_file);
			sem_post(&manager->f_sem);
			pthread_mutex_unlock(&manager->lock);
		} else {
			ret = unlink(tmp_file);
			if (ret < 0)
				APP_LOG_E("unlink %s failed!", tmp_file);
		}
	}
	pthread_mutex_unlock(&cr_mutex);

}

/**
 * _get_first_file - get filename first and second file in file path
 * @path: file path
 * @first_filename: filename first file name
 * @sec_filename: filename second file name
 *
 * This function use to get filename first and second file in file path
 *
 */
static int _get_first_file(char *path, char *first_filename, char *sec_filename)
{
	DIR *dir = NULL;
	struct dirent *ptr = NULL;

	pthread_mutex_lock(&cr_mutex);
	dir = opendir(path);
	if (dir == NULL) {
		APP_LOG_E("Open dir error...\n");
		pthread_mutex_unlock(&cr_mutex);
		return -1;
	}
	first_filename[0] = 0;
	sec_filename[0] = 0;
#ifdef MEDIA_TYPE_TS
	while ((ptr = readdir(dir)) != NULL) {
		if (ptr->d_type == 1 && strstr(ptr->d_name, ".ts") != NULL) {
			if (!strlen(first_filename)
				|| rec_namecmp(first_filename, ptr->d_name) >= 0) {
				strcpy(sec_filename, first_filename);
				strcpy(first_filename, ptr->d_name);
				continue;
			}
			if (!strlen(sec_filename)
				|| rec_namecmp(sec_filename, ptr->d_name) >= 0)
				strcpy(sec_filename, ptr->d_name);
		}
	}
#else
	while ((ptr = readdir(dir)) != NULL) {
		if (ptr->d_type == 1 && strstr(ptr->d_name, ".mp4") != NULL) {
			if (!strlen(first_filename)
				|| rec_namecmp(first_filename, ptr->d_name) >= 0) {
				strcpy(sec_filename, first_filename);
				strcpy(first_filename, ptr->d_name);
				continue;
			}
			if (!strlen(sec_filename)
				|| rec_namecmp(sec_filename, ptr->d_name) >= 0)
				strcpy(sec_filename, ptr->d_name);
		}
	}
#endif
	closedir(dir);
	pthread_mutex_unlock(&cr_mutex);
	if (!strlen(first_filename))
		return -2;

	return 0;

}
int _get_file_exist_indir_(char *path)
{
	DIR *dir = NULL;
	struct dirent *ptr = NULL;
	int file_count = 0;

	pthread_mutex_lock(&cr_mutex);
	dir = opendir(path);
	if (dir == NULL) {
		APP_LOG_E("Open dir error...\n");
		pthread_mutex_unlock(&cr_mutex);
		rt_thread_mdelay(10);
		return -1;
	}
	while ((ptr = readdir(dir)) != NULL) {
#ifdef MEDIA_TYPE_TS
		if (ptr->d_type == 1 && strstr(ptr->d_name, ".ts") != NULL) {
#else
		if (ptr->d_type == 1 && strstr(ptr->d_name, ".mp4") != NULL) {
#endif
			file_count++;
			if (file_count > 2) {
				closedir(dir);
				pthread_mutex_unlock(&cr_mutex);
				return 1;
			}
		}
	}
	closedir(dir);
	pthread_mutex_unlock(&cr_mutex);

	return 0;

}

static int __remove_file(char *file, char *path)
{
	char tmp_file[CDR_FILENAME_LEN];
	int len;
	int ret;

	if (file[0] == 0)
		return -1;

	strncpy(tmp_file, path, CDR_FILENAME_LEN);
	tmp_file[CDR_FILENAME_LEN - 1] = 0;
	len = strlen(tmp_file);
	strncpy(tmp_file + len, file, CDR_FILENAME_LEN - len);
	tmp_file[CDR_FILENAME_LEN - 1] = 0;
	pthread_mutex_lock(&cr_mutex);
	ret = unlink(tmp_file);
	pthread_mutex_unlock(&cr_mutex);
	if (ret < 0)
		APP_LOG_E("unlink %s failed!", tmp_file);
	APP_LOG_D("remove file %s\n", tmp_file);
	file[0] = 0;

	return 0;
}

int rename_rec(void *hdl, char *path, char *next_file)
{
	int ret = -1;
	char first_file[64];
	char sec_file[64];
	char tmp_file[CDR_FILENAME_LEN];

	car_rec_file_manager_t *mgr;

	mgr = (car_rec_file_manager_t *)hdl;
	ret = _get_first_file(path, first_file, sec_file);
	if (ret < 0)
		return -1;
	APP_LOG_W("rename_rec file :%s %s %s\n", first_file, sec_file, next_file);
#ifdef MEDIA_TYPE_TS
	if (!is_size_chg(path, first_file, get_recorder_duration()/60,
		get_recorder_resolution())) {
#else
	if ((strstr(path, "_R") != NULL && !is_size_chg(path,
		first_file, mgr->r_size)) || (strstr(path, "_F") != NULL &&
		!is_size_chg(path, first_file, mgr->f_size))) {
#endif
		_get_full_name(path, first_file, tmp_file);
		ret = rename(tmp_file, next_file);
		if (ret < 0) {
#ifdef MEDIA_TYPE_TS
			if (!is_size_chg(path, sec_file, get_recorder_duration()/60,
				get_recorder_resolution())) {
#else
			if ((strstr(path, "_R") != NULL && !is_size_chg(path, sec_file,
				mgr->r_size)) || (strstr(path, "_F") != NULL &&
				!is_size_chg(path, sec_file, mgr->f_size))) {
#endif
				_get_full_name(path, sec_file, tmp_file);
				ret = rename(tmp_file, next_file);
			} else
				return -1;
		}
	} else {
		#ifdef MEDIA_TYPE_TS
			if (!is_size_chg(path, sec_file, get_recorder_duration()/60,
				get_recorder_resolution())) {
		#else
			if ((strstr(path, "_R") != NULL && !is_size_chg(path, sec_file,
				mgr->r_size)) || (strstr(path, "_F") != NULL &&
				!is_size_chg(path, sec_file, mgr->f_size))) {
		#endif
				_get_full_name(path, sec_file, tmp_file);
				ret = rename(tmp_file, next_file);
			} else
				return -1;
	}

	return ret;
}

/**
 * __fallocate_file - fallocate a file for recorder
 * @filename: fallocate file name
 * @size: fallocate file size
 *
 * This function fallocate a file for recorder
 *
 */

static int __fallocate_file(char *filename, int size)
{
	int fd;
	int fsize = size;
	int ret = -1;
	u64 used_mbsize = 0;
	u64 free_mbsize = 0;
	char r_first_file[CDR_FILENAME_LEN];
	char r_sec_file[CDR_FILENAME_LEN];
	char f_first_file[CDR_FILENAME_LEN];
	char f_sec_file[CDR_FILENAME_LEN];

	pthread_mutex_lock(&cr_mutex);
	fd = open(filename, O_RDONLY);
	if (fd > 0) {
		APP_LOG_W("exsit file %s!\n", filename);
		close(fd);
		pthread_mutex_unlock(&cr_mutex);
		return 0;
	}

	fd = open(filename, O_RDWR | O_CREAT);
	if (fd < 0) {
		APP_LOG_E("Failed to open file %s!\n", filename);
	} else {
		ret = ioctl(fd, IO_FALLOCATE, &fsize);
		if (ret < 0) {
			APP_LOG_E("Failed FALLOCATE %s(%d) s:%d!\n", filename, fd, fsize);
			_get_disk_info(SDCARD_PATH, &used_mbsize, &free_mbsize);
			APP_LOG_E("used_mbsize:%lld free_mbsize:%lld\n",
				used_mbsize, free_mbsize);
			if (free_mbsize <= ((size/1024/1024) + REC_RESERVE_SIZE)) {
				f_first_file[0] = 0;
				f_sec_file[0] = 0;
				r_first_file[0] = 0;
				r_sec_file[0] = 0;
				_get_first_file(FRONT_PATH, f_first_file, f_sec_file);
				if (f_sec_file[0]) {
					__remove_file(f_first_file, FRONT_PATH);
					__remove_file(f_sec_file, FRONT_PATH);
				}
				_get_first_file(REAR_PATH, r_first_file, r_sec_file);
				if (r_sec_file[0]) {
					__remove_file(r_first_file, REAR_PATH);
					__remove_file(r_sec_file, REAR_PATH);
				}
				ret = ioctl(fd, IO_FALLOCATE, &fsize);
				APP_LOG_E("ret:%d!\n", ret);
			}
		}
		close(fd);
		APP_LOG_D("filename:%s\n", filename);
	}
	pthread_mutex_unlock(&cr_mutex);

	return ret;
}

static int _fallocate_tmp_file(car_rec_file_manager_t *manager)
{
	char tmp_file[CDR_FILENAME_LEN];
	int ret = -1;

	if (manager->r_tmp_file[0] == 0) {
		set_tmp_filename(tmp_file, REAR_PATH, manager->r_index++ % 2);
		ret = __fallocate_file(tmp_file, manager->r_size);
		if (ret < 0) {
			APP_LOG_E("__fallocate_file faile: ret:%d\n", ret);
			return ret;
		}
		pthread_mutex_lock(&manager->lock);
		strcpy(manager->r_tmp_file, tmp_file);
		sem_post(&manager->r_sem);
		pthread_mutex_unlock(&manager->lock);
	}
	if (manager->f_tmp_file[0] == 0) {
		set_tmp_filename(tmp_file, FRONT_PATH, manager->f_index++ % 2);
		ret = __fallocate_file(tmp_file, manager->f_size);
		if (ret < 0) {
			APP_LOG_E("__fallocate_file faile: ret:%d\n", ret);
			return ret;
		}
		pthread_mutex_lock(&manager->lock);
		strcpy(manager->f_tmp_file, tmp_file);
		sem_post(&manager->f_sem);
		pthread_mutex_unlock(&manager->lock);
	}
	APP_LOG_D("tmp_file: r %s, f %s\n",
		manager->r_tmp_file, manager->f_tmp_file);

	return 0;
}

/**
 * _file_mgr_task - file manage task
 * @param: file manage handle
 *
 * This function use to manage recorder file,fallocate/remove/rename recorder filename.
 *
 */
static void *_file_mgr_task(void *param)
{
	int ret;
	car_rec_file_manager_t *manager = param;
	struct timespec time;
	u64 used_mbsize;
	u64 free_mbsize;

	char r_first_file[CDR_FILENAME_LEN];
	char r_sec_file[CDR_FILENAME_LEN];

	char f_first_file[CDR_FILENAME_LEN];
	char f_sec_file[CDR_FILENAME_LEN];

	if (get_sd_status() == SDCARD_PLUGIN)
		_check_tmp_file(manager);

	f_first_file[0] = 0;
	f_sec_file[0] = 0;

	r_first_file[0] = 0;
	r_sec_file[0] = 0;

	while (manager->thread_exit == 0) {
		sem_wait(&manager->sem);
		if (manager->time_wait != 0) {
			clock_gettime(CLOCK_REALTIME, &time);
			time.tv_sec += manager->time_wait;
			sem_timedwait(&manager->wait_sem, &time);
			sem_trywait(&manager->sem);
			manager->time_wait = 0;
		}
		if (manager->thread_exit)
			break;

		while (manager->f_tmp_file[0] == 0 || manager->r_tmp_file[0] == 0) {
			if (manager->thread_exit != 0)
				break;
			pthread_mutex_lock(&cr_mutex);
			ret = _get_disk_info(SDCARD_PATH, &used_mbsize, &free_mbsize);
			pthread_mutex_unlock(&cr_mutex);
			if (ret < 0) {
				rt_thread_mdelay(10);
				continue;
			}
			if (free_mbsize > REC_RESERVE_SIZE) {
				ret = _fallocate_tmp_file(manager);
				if (ret == 0)
					continue;
				else
					rt_thread_mdelay(10);
			}

			if (r_first_file[0] == 0 && r_sec_file[0] == 0)
				_get_first_file(REAR_PATH, r_first_file, r_sec_file);
			if (f_first_file[0] == 0 && f_sec_file[0] == 0)
				_get_first_file(FRONT_PATH, f_first_file, f_sec_file);

			if (f_sec_file[0] && rec_namecmp(f_sec_file, r_first_file) < 0) {
				APP_LOG_W("remove file\n");
				__remove_file(f_first_file, FRONT_PATH);
				__remove_file(f_sec_file, FRONT_PATH);
				continue;
			}
			if (r_sec_file[0] && rec_namecmp(r_sec_file, f_first_file) < 0) {
				APP_LOG_W("remove file\n");
				__remove_file(r_first_file, REAR_PATH);
				__remove_file(r_sec_file, REAR_PATH);
				continue;
			}

			if (!_get_file_exist_indir_(FRONT_PATH) &&
				!_get_file_exist_indir_(REAR_PATH) &&
				free_mbsize <= REC_RESERVE_SIZE) {
				dialog_flag = DIALOG_RECORDER_FILE_FULL;
				lb_system_mq_send(LB_SYSMSG_RECORDER_FILE_FULL,
					&dialog_flag, sizeof(int), 0);
				APP_LOG_W("LB_SYSMSG_RECORDER_FILE_FULL\n");
				break;
			}
#ifdef MEDIA_TYPE_TS
			if (free_mbsize < REC_LOWEST_FREE_SIZE ||
				is_size_chg(REAR_PATH, r_first_file,
				get_recorder_duration()/60, get_recorder_resolution()) ||
				is_size_chg(FRONT_PATH, f_first_file,
				get_recorder_duration()/60, get_recorder_resolution()) ||
				is_size_chg(REAR_PATH, r_sec_file,
				get_recorder_duration()/60, get_recorder_resolution()) ||
				is_size_chg(FRONT_PATH, f_sec_file,
				get_recorder_duration()/60, get_recorder_resolution())) {
#else
			if (free_mbsize < REC_LOWEST_FREE_SIZE ||
				is_size_chg(REAR_PATH, r_first_file, manager->r_size) ||
				is_size_chg(FRONT_PATH, f_first_file, manager->f_size) ||
				is_size_chg(REAR_PATH, r_sec_file, manager->r_size) ||
				is_size_chg(FRONT_PATH, f_sec_file, manager->f_size)) {
#endif

				if (r_sec_file[0]) {
					APP_LOG_W("remove file %lld\n", free_mbsize);
					__remove_file(r_first_file, REAR_PATH);
					__remove_file(r_sec_file, REAR_PATH);
				}
				if (f_sec_file[0]) {
					APP_LOG_W("remove file\n");
					__remove_file(f_first_file, FRONT_PATH);
					__remove_file(f_sec_file, FRONT_PATH);
				}
				continue;
			}
			APP_LOG_D("used_mbsize:%lld free_mbsize:%lld\n",
				used_mbsize, free_mbsize);

			pthread_mutex_lock(&manager->lock);
			if (f_first_file[0] && manager->f_tmp_file[0] == 0) {
				_get_full_name(FRONT_PATH, f_first_file,
					manager->f_tmp_file);
				f_first_file[0] = 0;
				sem_post(&manager->f_sem);
			}
			if (f_sec_file[0] && manager->f_tmp_file[0] == 0) {
				_get_full_name(FRONT_PATH, f_sec_file,
					manager->f_tmp_file);
				f_sec_file[0] = 0;
				sem_post(&manager->f_sem);
			}
			if (r_first_file[0] && manager->r_tmp_file[0] == 0) {
				_get_full_name(REAR_PATH, r_first_file,
					manager->r_tmp_file);
				r_first_file[0] = 0;
				sem_post(&manager->r_sem);
			}
			if (r_sec_file[0] && manager->r_tmp_file[0] == 0) {
				_get_full_name(REAR_PATH, r_sec_file,
					manager->r_tmp_file);
				r_sec_file[0] = 0;
				sem_post(&manager->r_sem);
			}
			pthread_mutex_unlock(&manager->lock);
			APP_LOG_D("tmp_file: r %s, f %s\n",
				manager->r_tmp_file, manager->f_tmp_file);
		}

	}

	return NULL;
}

int fallocate_front_file(void *hdl, char *file)
{
	int ret = -1;
	car_rec_file_manager_t *mgr;

	mgr = (car_rec_file_manager_t *)hdl;
	ret = __fallocate_file(file, mgr->f_size);
	APP_LOG_D("size:%d\n", mgr->f_size);

	return ret;
}

int fallocate_rear_file(void *hdl, char *file)
{
	int ret = -1;
	car_rec_file_manager_t *mgr;

	mgr = (car_rec_file_manager_t *)hdl;
	ret = __fallocate_file(file, mgr->r_size);
	APP_LOG_D("size:%d\n", mgr->r_size);

	return ret;
}
/**
 * truncate_file - truncate file to fallocate size
 * @hdl: car_rec_file_manager_t struct handle
 * @file: file name need to truncate
 * @recoder_flag: front or rear camera flag
 *
 * This function to be called when needs to covered writing file.
 * not use to fallocate a new file.
 */

int truncate_file(void *hdl, char *file, int recoder_flag)
{
	int ret = -1;
	car_rec_file_manager_t *mgr;
	int fd;

	mgr = (car_rec_file_manager_t *)hdl;
	pthread_mutex_lock(&cr_mutex);
	fd = open(file, O_RDWR | O_TRUNC);
	if (fd > 0) {
		APP_LOG_W("truncate_file %s\n", file);
		if (recoder_flag == FRONT_RECORDER)
			ioctl(fd, IO_FTRUNCATE, &mgr->f_size);
		else if (recoder_flag == REAR_RECORDER)
			ioctl(fd, IO_FTRUNCATE, &mgr->r_size);
		fsync(fd);
		close(fd);
		pthread_mutex_unlock(&cr_mutex);
		return 0;
	} else
		APP_LOG_W("not exsit file %s\n", file);
	pthread_mutex_unlock(&cr_mutex);

	return ret;
}
#if 1//jiasuofenqu
int __get_lock_file_total_size(u64 *total)
{
	u64 flock_file_size = 0, rlock_file_size = 0;
	struct dirent *ptr = NULL;
	DIR *dir = NULL;

	pthread_mutex_lock(&cr_mutex);
	/* get total file size of front */
	dir = opendir(FRONT_LOCK_PATH);
	if (dir == NULL) {
		APP_LOG_E("Open dir error...\n");
		pthread_mutex_unlock(&cr_mutex);
		return -1;
	}
#ifdef MEDIA_TYPE_TS
	while ((ptr = readdir(dir)) != NULL) {
		if (ptr->d_type == 1 && strstr(ptr->d_name, ".ts") != NULL)
			flock_file_size += ptr->d_reclen;
	}
#else
	while ((ptr = readdir(dir)) != NULL) {
		if (ptr->d_type == 1 && strstr(ptr->d_name, ".mp4") != NULL)
			flock_file_size += ptr->d_reclen;
	}
#endif
	closedir(dir);

	/* get total file size of front */
	dir = opendir(REAR_LOCK_PATH);
	if (dir == NULL) {
		APP_LOG_E("Open dir error...\n");
		pthread_mutex_unlock(&cr_mutex);
		return -1;
	}
#ifdef MEDIA_TYPE_TS
	while ((ptr = readdir(dir)) != NULL) {
		if (ptr->d_type == 1 && strstr(ptr->d_name, ".ts") != NULL)
			rlock_file_size += ptr->d_reclen;
	}
#else
	while ((ptr = readdir(dir)) != NULL) {
		if (ptr->d_type == 1 && strstr(ptr->d_name, ".mp4") != NULL)
			rlock_file_size += ptr->d_reclen;
	}
#endif
	closedir(dir);

	pthread_mutex_unlock(&cr_mutex);

	*total = flock_file_size + rlock_file_size;
	APP_LOG_E("lock file size: %lld, %lld, %lld\n", flock_file_size, rlock_file_size,
		*total);

	return 0;
}
int __rm_the_oldest_lock_file(void)
{
	char r_first_file[CDR_FILENAME_LEN] = {0};
	char r_sec_file[CDR_FILENAME_LEN] = {0};

	char f_first_file[CDR_FILENAME_LEN] = {0};
	char f_sec_file[CDR_FILENAME_LEN] = {0};

	int ret1 = -1;
	int ret2 = -1;

	if (r_first_file[0] == 0 && r_sec_file[0] == 0)
		ret1 = _get_first_file(REAR_LOCK_PATH, r_first_file, r_sec_file);
	if (f_first_file[0] == 0 && f_sec_file[0] == 0)
		ret2 = _get_first_file(FRONT_LOCK_PATH, f_first_file, f_sec_file);

	if (ret1 == -1 && ret2 == -1)
		return -1;

	if (ret1 == 0 && ret2) {
		__remove_file(r_first_file, REAR_LOCK_PATH);
		__remove_file(r_sec_file, REAR_LOCK_PATH);
		return 0;
	}

	if (ret2 == 0 && ret1) {
		__remove_file(f_first_file, FRONT_LOCK_PATH);
		__remove_file(f_sec_file, FRONT_LOCK_PATH);
		return 0;
	}
	
	if (f_sec_file[0] && rec_namecmp(f_sec_file, r_first_file) < 0) {
		APP_LOG_W("remove file: %s, %s\n", f_first_file, f_sec_file);
		__remove_file(f_first_file, FRONT_LOCK_PATH);
		__remove_file(f_sec_file, FRONT_LOCK_PATH);
		return 0;
	}
	if (r_sec_file[0] && rec_namecmp(r_sec_file, f_first_file) < 0) {
		APP_LOG_W("remove file: %s, %s\n", r_first_file, r_sec_file);
		__remove_file(r_first_file, REAR_LOCK_PATH);
		__remove_file(r_sec_file, REAR_LOCK_PATH);
		return 0;
	}
	APP_LOG_W("remove file: %s, %s\n", f_first_file, r_first_file);
	__remove_file(f_first_file, FRONT_LOCK_PATH);
	__remove_file(r_first_file, REAR_LOCK_PATH);
	
	return 0;
}
int file_mgr_judge_lock_memery(void)
{
	u64 used_size, free_size, total_size;
	u64 total_lock_size, malloc_size;

	int ret = -1;
	pthread_mutex_lock(&lock_file_mutex);
	pthread_mutex_lock(&cr_mutex);
	ret = _get_disk_info(SDCARD_PATH, &used_size, &free_size);
	total_size = used_size + free_size;
	pthread_mutex_unlock(&cr_mutex);
	malloc_size = total_size *
		LOCK_FILE_RATIO / (LOCK_FILE_RATIO + NORMAL_FILE_RATIO);
	APP_LOG_W("prealloc mem size: %lld, %lld, %lld, %lld\n",
		used_size, free_size, total_size, malloc_size);
	while(1) {
		ret = __get_lock_file_total_size(&total_lock_size);
		if (ret) {
			APP_LOG_E("get lock file total size failed\n");
			return -1;
		}

		if (total_lock_size >= malloc_size) {
			ret = __rm_the_oldest_lock_file();
			if (ret) {
				APP_LOG_E("rm the oldest file failed\n");
				pthread_mutex_unlock(&lock_file_mutex);
				return -1;
			}
			continue;
		}
		break;
	}
	pthread_mutex_unlock(&lock_file_mutex);
	return 0;
}
#endif

/**
 * file_mgr_wakeup - wakeup file manager task after timeout(s)
 * @mgr: handle
 * @time: time delay to wakeup task
 *
 * This function to be called when needs to get a new tmp file.
 *
 */
void file_mgr_wakeup(void *hdl, int time)
{
	car_rec_file_manager_t *mgr;

	mgr = (car_rec_file_manager_t *)hdl;
	if (mgr && mgr->thread) {
		mgr->time_wait = time;
		sem_post(&mgr->sem);
	}
}

int file_mgr_front_wait(void *hdl, int timeout_ms)
{
	int ret = -1;
	car_rec_file_manager_t *mgr;
	struct timespec time;

	mgr = (car_rec_file_manager_t *)hdl;
	clock_gettime(CLOCK_REALTIME, &time);
	time.tv_nsec += (timeout_ms % 1000) * 1000 * 1000;
	time.tv_sec  += timeout_ms / 1000;
	ret = sem_timedwait(&mgr->f_sem, &time);
	if (ret == 0)
		sem_post(&mgr->f_sem);

	return ret;
}

int file_mgr_rear_wait(void *hdl, int timeout_ms)
{
	int ret = -1;
	car_rec_file_manager_t *mgr;
	struct timespec time;

	mgr = (car_rec_file_manager_t *)hdl;
	clock_gettime(CLOCK_REALTIME, &time);
	time.tv_nsec += (timeout_ms % 1000) * 1000 * 1000;
	time.tv_sec  += timeout_ms / 1000;
	APP_LOG_D("timeout:%d, %d\n", timeout_ms, mgr->r_sem.sem->value);
	ret = sem_timedwait(&mgr->r_sem, &time);
	APP_LOG_D("timeout:%d, %d, ret:%d\n", timeout_ms, mgr->r_sem.sem->value, ret);
	if (ret == 0)
		sem_post(&mgr->r_sem);

	return ret;
}

int file_mgr_get_front_file(void *hdl, char *file)
{
	int ret = -1;
	car_rec_file_manager_t *mgr;

	mgr = (car_rec_file_manager_t *)hdl;
	ret = sem_trywait(&mgr->f_sem);
	if (ret == 0) {
		pthread_mutex_lock(&mgr->lock);
		strcpy(file, mgr->f_tmp_file);
		mgr->f_tmp_file[0] = 0;
		pthread_mutex_unlock(&mgr->lock);
	} else
		APP_LOG_W("file_mgr_get_front_file failed!\n");

	return ret;
}

int file_mgr_get_rear_file(void *hdl, char *file)
{
	int ret = -1;
	car_rec_file_manager_t *mgr;

	mgr = (car_rec_file_manager_t *)hdl;
	ret = sem_trywait(&mgr->r_sem);
	if (ret == 0) {
		pthread_mutex_lock(&mgr->lock);
		strcpy(file, mgr->r_tmp_file);
		mgr->r_tmp_file[0] = 0;
		pthread_mutex_unlock(&mgr->lock);
	} else
		APP_LOG_W("file_mgr_get_rear_file failed!\n");

	return ret;
}

int file_mgr_resize(void *hdl, int f_bitrate, int r_bitrate, int time)
{
	car_rec_file_manager_t *mgr;
	char tmp_file[64];
	int ret;

	mgr = (car_rec_file_manager_t *)hdl;
	if (mgr->r_bitrate == r_bitrate && mgr->f_bitrate == f_bitrate
		&& mgr->time_wait == time)
		return 0;
	mgr->r_bitrate = r_bitrate;
	mgr->f_bitrate = f_bitrate;
	mgr->time_wait = time;

	mgr->r_size = get_falloc_size(mgr->r_bitrate, mgr->time_wait);
	mgr->r_size = falloc_align(mgr->r_size);

	mgr->f_size = get_falloc_size(mgr->f_bitrate, mgr->time_wait);
	mgr->f_size = falloc_align(mgr->f_size);

	ret = file_mgr_get_front_file(hdl, tmp_file);
	if (ret == 0) {
		pthread_mutex_lock(&cr_mutex);
		ret = unlink(tmp_file);
		pthread_mutex_unlock(&cr_mutex);
		if (ret < 0)
			APP_LOG_E("unlink %s failed!", tmp_file);
	}
	ret = file_mgr_get_rear_file(hdl, tmp_file);
	if (ret == 0) {
		pthread_mutex_lock(&cr_mutex);
		ret = unlink(tmp_file);
		pthread_mutex_unlock(&cr_mutex);
		if (ret < 0)
			APP_LOG_E("unlink %s failed!", tmp_file);
	}
	if (mgr->r_tmp_file[0] == 0 || mgr->f_tmp_file[0] == 0)
		file_mgr_wakeup(hdl, 0);

	return 0;
}

/**
 * file_mgr_create - file_mgr_create
 *
 * This function will create a manager.
 *
 * Returns manager handle
 */
void *file_mgr_create(int f_bitrate, int r_bitrate, int time)
{
	pthread_attr_t thread_attr;
	car_rec_file_manager_t *mgr;

	mgr = malloc(sizeof(car_rec_file_manager_t));
	if (mgr == NULL)
		return NULL;
	memset(mgr, 0, sizeof(car_rec_file_manager_t));
	pthread_attr_init(&thread_attr);
	pthread_attr_setstacksize(&thread_attr, 0x1000);
	mgr->time_wait = 0;

	mgr->r_bitrate = r_bitrate;
	mgr->r_size = get_falloc_size(mgr->r_bitrate, time);
	mgr->r_size = falloc_align(mgr->r_size);

	mgr->f_bitrate = f_bitrate;
	mgr->f_size = get_falloc_size(mgr->f_bitrate, time);
	mgr->f_size = falloc_align(mgr->f_size);
	APP_LOG_D("size:%d %d\n", mgr->f_size, mgr->r_size);
	sem_init(&mgr->sem, 0, 0);
	sem_init(&mgr->wait_sem, 0, 0);
	sem_init(&mgr->r_sem, 0, 0);
	sem_init(&mgr->f_sem, 0, 0);
	pthread_mutex_init(&mgr->lock, NULL);
	#if 1//jiasuofenqu
	pthread_mutex_init(&lock_file_mutex, NULL);
	#endif
	pthread_create(&mgr->thread, &thread_attr,
		_file_mgr_task, mgr);

	return mgr;
}

/**
 * file_mgr_destory - destory file manage
 * mgr: file manage handle
 *
 * This function useto destory file manage.
 *
 */
void file_mgr_destory(void *hdl)
{
	car_rec_file_manager_t *mgr;

	mgr = (car_rec_file_manager_t *)hdl;
	if (mgr && mgr->thread) {
		mgr->thread_exit = 1;
		mgr->time_wait = 0;
		sem_post(&mgr->sem);
		sem_post(&mgr->wait_sem);
		pthread_join(mgr->thread, NULL);
		mgr->thread = NULL;
		sem_destroy(&mgr->sem);
		sem_destroy(&mgr->wait_sem);
		sem_destroy(&mgr->r_sem);
		sem_destroy(&mgr->f_sem);
		pthread_mutex_destroy(&mgr->lock);
		#if 1//jiasuofenqu
		pthread_mutex_destroy(&lock_file_mutex);
		#endif
	}
	free(mgr);
}

