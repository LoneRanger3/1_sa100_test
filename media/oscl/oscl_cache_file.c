/*
 * oscl_cache_file.c - file api used by lombo media framework.
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
#define LOG_TAG		"oscl_cache_file"

#include <string.h>
#include <strings.h>
#include <oscl.h>
#include <dfs_posix.h>

#define OSCL_FILE_INIT 0
#define OSCL_FILE_OPEN 1
#define OSCL_FILE_CLOSE 2
#define OSCL_FILE_EXIT 3

#define DEFAULT_CACHE_BUFFER_SIZE (128*1024)
#define DEFAULT_CACHE_NUM_PER_FILE 2
#define MAX_CACHE_NUM_PER_FILE 32
#define MAX_FILE_NAME_SIZE (128)

#ifdef __linux__
#define MAX_FILE_NUM 32
#else
#define MAX_FILE_NUM 8
#endif

#define OSCL_FILE_CACE_INVALID 0x0
#define OSCL_FILE_CACE_DIRT 0x1
#define OSCL_FILE_CACE_CLEAN 0x2

typedef struct oscl_file_cache {
	void *buf;
	int stat;
	int buf_len;
	int cache_len;
	off_t offset;
	pthread_mutex_t lock;
} oscl_file_cache_t;

typedef struct oscl_file {
	int fd;
	int stat;
	off_t offset;
	off_t file_len;
	oscl_file_cache_t *cur_cache;
	int prio;
	pthread_t writer_thread;
	pthread_mutex_t rw_lock;
	pthread_mutex_t c_lock;
	sem_t sem;
	int cache_size;
	int cache_num;
	oscl_file_cache_t cache[MAX_CACHE_NUM_PER_FILE];

	int write_size;
	int ts_stream;
	char name[MAX_FILE_NAME_SIZE];
} oscl_file_t;

typedef struct oscl_file_manager {
	pthread_mutex_t lock;
	oscl_file_t file[MAX_FILE_NUM];
} oscl_file_manager_t;

#define TS_STREAM_SUFFIX "ts"
#define TS_STREAM_UTIT_SIZE (2 * 1024 * 1024)

static oscl_file_manager_t *file_manager;
static int _resize_oscl_file(int fd, size_t size);

static int _init_cache(oscl_file_cache_t *cache, int size)
{
	oscl_param_check((cache != NULL), -1, NULL);
	memset(cache, 0, sizeof(oscl_file_cache_t));
	cache->buf = oscl_zalloc(size);
	oscl_param_check(cache->buf != NULL, -1, NULL);
	cache->stat = OSCL_FILE_CACE_INVALID;
	cache->buf_len = size;
	pthread_mutex_init(&cache->lock, NULL);
	return 0;
}

static void _deinit_cache(oscl_file_cache_t *cache)
{
	if (cache->stat == OSCL_FILE_CACE_DIRT)
		OSCL_LOGE("cache buffer not write while deinit");

	pthread_mutex_destroy(&cache->lock);
	oscl_free(cache->buf);
	memset(cache, 0, sizeof(oscl_file_cache_t));
}

static void dump_cache(int fd, oscl_file_cache_t *cache)
{
#if 0
	char *tmp_cache_buf = cache->buf;
	OSCL_LOGD("fd:%d cache data:%x %x %x %x", fd,
		tmp_cache_buf[0], tmp_cache_buf[1],
		tmp_cache_buf[2], tmp_cache_buf[3]);
	OSCL_LOGD("fd:%d cache:%p(%p), stat:%d, buflen:%x, clen:%d, offset:%llx",
		fd, cache, cache->buf, cache->stat, cache->buf_len,
		cache->cache_len, cache->offset);
#endif
}

static int _clean_fs_cache(oscl_file_t *file, oscl_file_cache_t *cache)
{
	off_t tmp_offset;
	int ret = 0;
	int len = 0;
	int fd = file->fd;

	OSCL_TRACE("fd:%d", fd);
	if (cache->stat == OSCL_FILE_CACE_CLEAN)
		return 0;
	pthread_mutex_lock(&file->c_lock);
	tmp_offset = lseek(fd, cache->offset, SEEK_SET);
	if (tmp_offset != cache->offset) {
#if 0
		OSCL_LOGE("seek err! fd:%d(%d-%d), buffer(%p-%d:%d), err:%d",
			fd, (int)tmp_offset, (int)cache->offset, cache,
			len, (unsigned int)file->file_len, errno);
#endif
		pthread_mutex_unlock(&file->c_lock);
		ret = -1;
		goto EXIT;
	}

	len = write(fd, cache->buf, cache->cache_len);
	if (len != cache->cache_len) {
#if 0
		OSCL_LOGE("write file err! fd:%d(%lld), buffer(%p-%d:%d), err:%d",
			fd, (long long)tmp_offset, cache->buf, len,
			cache->cache_len, errno);
#endif
		pthread_mutex_unlock(&file->c_lock);
		ret = -1;
		goto EXIT;
	}
	if (file->ts_stream) {
		file->write_size += len;
		if (file->write_size >= TS_STREAM_UTIT_SIZE) {
			off_t file_pos = 0;

			file->write_size = 0;
			file_pos = lseek(fd, 0, SEEK_CUR);
			if (file_pos >= 0)
				_resize_oscl_file(fd, file_pos);
			else
				OSCL_LOGE("Seek fail. %s", file->name);
		}
	}
	OSCL_LOGD("==write to file, fd:%d(%lld), buf:%p, len:%d,%d",
		fd, tmp_offset, cache->buf, cache->cache_len, len);
	if (cache->cache_len != cache->buf_len) {
		OSCL_LOGI("fd:%d clean cache which is not full:%p", fd, cache);
		OSCL_LOGI("==write to file, fd:%d(%lld), buf:%p, len:%d,%d",
			fd, tmp_offset, cache->buf, cache->cache_len, len);
	}
	cache->stat = OSCL_FILE_CACE_CLEAN;
	pthread_mutex_unlock(&file->c_lock);
EXIT:
	OSCL_TRACE("fd:%d", fd);
	return ret;
}

/*
static void dump_all_cache(oscl_file_t *file)
{
	int i = 0;

	OSCL_LOGE("file offset : %d  %d\n", (int)file->offset, (int)file->file_len);
	for (i = 0; i < MAX_CACHE_NUM_PER_FILE; i++) {
		OSCL_LOGE("----------- %d ------------\n", i);
		OSCL_LOGE("offset : %d\n", (int)file->cache[i].offset);
		OSCL_LOGE("cache_len : %d\n", file->cache[i].cache_len);
		OSCL_LOGE("buf_len : %d\n", file->cache[i].buf_len);
		OSCL_LOGE("stat : %d\n", file->cache[i].stat);
	}
}
*/
static int _clean_one_cache(oscl_file_t *file, int cache_len)
{
	int j;
	int ret;
	oscl_file_cache_t *tmp_cache;

	if (file == NULL)
		return -1;
	OSCL_TRACE("fd:%d", file->fd);
	/* find min offset cache and write to file */
	tmp_cache = NULL;
	for (j = 0; j < file->cache_num; j++) {
		if (file->cur_cache == &file->cache[j])
			continue;
		if (file->cache[j].stat == OSCL_FILE_CACE_DIRT
			&& file->cache[j].cache_len >= cache_len) {
			if (tmp_cache == NULL
				|| file->cache[j].offset < tmp_cache->offset)
				tmp_cache = &file->cache[j];
		}
	}
	if (tmp_cache == NULL) {
		/*OSCL_LOGE("no cache datalen > %d", cache_len);*/
		return 0;
	}
	pthread_mutex_lock(&tmp_cache->lock);
	dump_cache(file->fd, tmp_cache);
	ret = _clean_fs_cache(file, tmp_cache);
	pthread_mutex_unlock(&tmp_cache->lock);
#if 0
	if (ret < 0) {
		OSCL_LOGE("fd:%d clean cach fail! check it!", file->fd);
		tmp_cache = NULL;
	}
#endif
	OSCL_TRACE("fd:%d clean one cache", file->fd);
	return ret;
}

static void _clean_all_cache(oscl_file_t *file, int cache_len)
{
	int i;
	int j;
	int ret;
	oscl_file_cache_t *tmp_cache = NULL;

	if (file == NULL)
		return;
	OSCL_TRACE("fd:%d", file->fd);
	for (i = 0; i < file->cache_num; i++) {
		/* find min offset cache and write to file */
		tmp_cache = NULL;
		for (j = 0; j < file->cache_num; j++) {
			pthread_mutex_lock(&file->cache[j].lock);
			if (file->cache[j].stat == OSCL_FILE_CACE_DIRT
				&& file->cache[j].cache_len >= cache_len) {
				if (tmp_cache == NULL
					|| file->cache[j].offset < tmp_cache->offset)
					tmp_cache = &file->cache[j];
			}
			pthread_mutex_unlock(&file->cache[j].lock);
		}
		if (tmp_cache == NULL) {
			OSCL_LOGI("write %d cacach", i);
			break;
		}
		pthread_mutex_lock(&tmp_cache->lock);
		dump_cache(file->fd, tmp_cache);
		ret = _clean_fs_cache(file, tmp_cache);
		if (ret < 0)
			OSCL_LOGE("fd:%d clean cach fail! check it!", file->fd);
		pthread_mutex_unlock(&tmp_cache->lock);
	}
	OSCL_TRACE("fd:%d clean %d cache line", file->fd, i);
}

static void _update_offset(oscl_file_t *file)
{
	int i;
	oscl_file_cache_t *tmp_cache;
	oscl_file_cache_t *cache;
	oscl_file_cache_t *free_cache;
	int time = 0;
	int n_partial_cache;

STRAT:
	cache = NULL;
	free_cache = NULL;
	n_partial_cache = 0;
	OSCL_TRACE("trace:%d", file->fd);
	/* get a cache whose offset is suitable */
	for (i = 0; i < file->cache_num; i++) {
		tmp_cache = &file->cache[i];
		OSCL_LOGD("cache[%d-%d:%p]: stat:%d cache len:%d, c_off:%lld, f_off:%lld",
			file->fd, i, tmp_cache, tmp_cache->stat,
			tmp_cache->cache_len, tmp_cache->offset,
			file->offset);
		if ((file->offset >= tmp_cache->offset)
			&& (file->offset < tmp_cache->offset + tmp_cache->buf_len)) {
			cache = tmp_cache;
			break;
		}
		if (tmp_cache->stat == OSCL_FILE_CACE_INVALID)
			free_cache = tmp_cache;
		if (free_cache == NULL && (tmp_cache->stat != OSCL_FILE_CACE_DIRT))
			free_cache = tmp_cache;
		if (tmp_cache->cache_len < tmp_cache->buf_len
			&& (tmp_cache->stat == OSCL_FILE_CACE_DIRT))
			n_partial_cache++;
	}
	if (cache == NULL && free_cache == NULL) {
		if (time == 0) {
			int sval = 0;
			time = oscl_get_msec();
			sem_getvalue(&file->sem, &sval);
			if (sval == 0)
				sem_post(&file->sem);
		}
		if (n_partial_cache == file->cache_num) {
			OSCL_LOGD("all cache is partial cache (dirty cache)");
			_clean_one_cache(file, 1);
		}
#if defined(__EOS__)
		if (file->prio == LB_FILE_SYSTEM_PRIO) {
			file->prio = LB_FILE_SYSTEM_PRIO - 1;
			rt_thread_control(file->writer_thread,
				RT_THREAD_CTRL_CHANGE_PRIORITY, &file->prio);
		}
#endif
		oscl_mdelay(20);
		goto STRAT;
	}
	if (time) {
		time = oscl_get_msec() - time;
		if (time > 500)
			OSCL_LOGE("%d, wait %d ms to get cache.", file->fd, time);
	}
	if (cache == NULL && free_cache != NULL) {
		cache = free_cache;
		pthread_mutex_lock(&cache->lock);
		if (cache->stat != OSCL_FILE_CACE_DIRT) {
			cache->offset = file->offset - file->offset % file->cache_size;
			cache->cache_len = 0;
			cache->stat = OSCL_FILE_CACE_INVALID;
			OSCL_LOGD("=============fd %d set %p stat:%x",
				file->fd, cache, cache->stat);
		} else
			cache = NULL;
		pthread_mutex_unlock(&cache->lock);
	}
	file->cur_cache = cache;
	OSCL_TRACE("trace:%d", file->fd);
}

static int _write_to_buffer(oscl_file_t *file, const void *buf, int len)
{
	off_t tmp_offset;
	int read_len = 0;
	int write_len = 0;
	int buf_dest_offset = 0;
	oscl_file_cache_t *cache = NULL;
	OSCL_TRACE("trace:%d", file->fd);
	if (file->cur_cache == NULL || file->offset < file->cur_cache->offset
		|| file->offset >= file->cur_cache->offset + file->cur_cache->buf_len) {
		file->cur_cache = NULL;
		_update_offset(file);
		oscl_param_check(file->cur_cache != NULL, 0, NULL);
	}

	cache = file->cur_cache;
	pthread_mutex_lock(&cache->lock);
	buf_dest_offset = file->offset - cache->offset;
	if (buf_dest_offset >= cache->buf_len) {
		OSCL_LOGE("ERR! write to invalid cache!");
		write_len = 0;
		goto EXIT;
	}
	if (buf_dest_offset > cache->cache_len) {
		pthread_mutex_lock(&file->c_lock);
		tmp_offset = lseek(file->fd, cache->offset + cache->cache_len, SEEK_SET);
		if (tmp_offset != cache->offset + cache->cache_len)
			OSCL_LOGE("lseek failed, err:%d", errno);

		read_len = cache->buf_len - cache->cache_len;
		read_len = read(file->fd, cache->buf + cache->cache_len, read_len);
		cache->cache_len += read_len;

		cache->stat = OSCL_FILE_CACE_DIRT;
		pthread_mutex_unlock(&file->c_lock);
	}

	write_len = cache->buf_len - buf_dest_offset;
	if (write_len > len)
		write_len = len;
	memcpy(cache->buf + buf_dest_offset, buf, write_len);

	if (cache->cache_len < buf_dest_offset + write_len)
		cache->cache_len = buf_dest_offset + write_len;
	cache->stat = OSCL_FILE_CACE_DIRT;
	OSCL_LOGD("=============fd %d set %p stat:%x", file->fd, cache, cache->stat);
	file->offset += write_len;
	buf_dest_offset += write_len;
	if (buf_dest_offset == cache->buf_len) {
		file->cur_cache = NULL;
		OSCL_LOGD("cache %p full, to be clean", cache);
		sem_post(&file->sem);
	}
EXIT:
#if 0
	if (cache) {
		char *tmp_cache_buf = cache->buf;
		OSCL_LOGD("fd:%d cache:%p(%p), stat:%d, buflen:%x, c_len:%d offset:%llx",
			file->fd, cache, cache->buf, cache->stat,
			cache->buf_len, cache->cache_len, cache->offset);
		OSCL_LOGD("fd:%d cache data:%x %x %x %x",
			file->fd, tmp_cache_buf[0], tmp_cache_buf[1],
			tmp_cache_buf[2], tmp_cache_buf[3]);
	}
#endif
	pthread_mutex_unlock(&cache->lock);
	OSCL_TRACE("trace:%d", file->fd);
	return write_len;
}


static void *oscl_cache_fs_writer_thread(void *param)
{
	oscl_file_t *file;
	int ret = 0;
	int nretry = 0;
	int i;
#if defined(__EOS__)
	int prio;
#endif

	file = param;
	OSCL_LOGD("fd:%d(%p)", file->fd, file);

	if (file == NULL)
		return NULL;
	while (file->stat == OSCL_FILE_OPEN) {
		sem_wait(&file->sem);
		if (file->stat == OSCL_FILE_OPEN) {
			ret = _clean_one_cache(file, file->cache_size);
			if (ret == 0) {
				nretry = 0;
				continue;
			}
			sem_post(&file->sem);
			nretry++;
			if (nretry >= 3) {
				OSCL_LOGE("write cache to file failed!");
				oscl_mdelay(50);
				for (i = 0; i < file->cache_num; i++) {
					pthread_mutex_lock(&file->cache[i].lock);
					file->cache[i].stat = OSCL_FILE_CACE_CLEAN;
					pthread_mutex_unlock(&file->cache[i].lock);
				}
			}
		}
#if defined(__EOS__)
		prio = file->prio;
		if (file->prio == LB_FILE_SYSTEM_PRIO + 1)
			file->prio = LB_FILE_SYSTEM_PRIO;
		if (file->prio == LB_FILE_SYSTEM_PRIO - 1)
			file->prio = LB_FILE_SYSTEM_PRIO + 1;
		if (prio != file->prio) {
			rt_thread_control(file->writer_thread,
				RT_THREAD_CTRL_CHANGE_PRIORITY, &file->prio);
		}
#endif
	}
	/* write cache buffer to cache */
	_clean_all_cache(file, 1);
	OSCL_LOGD("fd:%d(%p)\n", file->fd, file);
	file->stat = OSCL_FILE_EXIT;
	return NULL;
}

static oscl_file_t *_find_oscl_file(int fd)
{
	int i;
	oscl_file_t *tmp_file = NULL;
	if (file_manager == NULL || fd < 0)
		return NULL;
	pthread_mutex_lock(&file_manager->lock);
	for (i = 0; i < MAX_FILE_NUM; i++) {
		if (file_manager->file[i].fd == fd) {
			tmp_file = &file_manager->file[i];
			break;
		}
	}
	pthread_mutex_unlock(&file_manager->lock);
	return tmp_file;
}

static oscl_file_t *_open_oscl_file(int fd, const char *name)
{
	int i;
	oscl_file_t *tmp_file = NULL;
	pthread_attr_t thread_attr;
	struct sched_param shed_param = {0};
	char *suffix = NULL;
	int err;

	if (file_manager == NULL || fd < 0)
		return NULL;
	pthread_mutex_lock(&file_manager->lock);
	for (i = 0; i < MAX_FILE_NUM; i++) {
		if (file_manager->file[i].fd < 0) {
			tmp_file = &file_manager->file[i];
			memset(tmp_file, 0, sizeof(oscl_file_t));
			strcpy(tmp_file->name, name);
			suffix = strrchr(name, '.');
			if (suffix && !strcasecmp(TS_STREAM_SUFFIX, suffix+1))
				tmp_file->ts_stream = 1;
			tmp_file->fd = fd;
			break;
		}
	}
	pthread_mutex_unlock(&file_manager->lock);
	if (tmp_file == NULL) {
		OSCL_LOGE("no free handle for new file");
		goto EXIT;
	}
	tmp_file->file_len = lseek(fd, tmp_file->offset, SEEK_END);
	tmp_file->stat = OSCL_FILE_OPEN;
	sem_init(&tmp_file->sem, 0, 0);
	pthread_mutex_init(&tmp_file->rw_lock, NULL);
	pthread_mutex_init(&tmp_file->c_lock, NULL);
	tmp_file->cache_size = DEFAULT_CACHE_BUFFER_SIZE;
	tmp_file->cache_num = DEFAULT_CACHE_NUM_PER_FILE;
	for (i = 0; i < tmp_file->cache_num; i++)
		_init_cache(&tmp_file->cache[i], DEFAULT_CACHE_BUFFER_SIZE);

	pthread_attr_init(&thread_attr);
#if defined(__EOS__)
	pthread_attr_setstacksize(&thread_attr, 0x1000);
	shed_param.sched_priority = LB_FILE_SYSTEM_PRIO;
	tmp_file->prio = LB_FILE_SYSTEM_PRIO;
#endif
	pthread_attr_setschedparam(&thread_attr, &shed_param);
	err = pthread_create(
			&tmp_file->writer_thread, &thread_attr,
			oscl_cache_fs_writer_thread, tmp_file);
	if (err != 0)
		tmp_file->writer_thread = 0;
EXIT:
	return tmp_file;
}

static void _close_oscl_file(int fd)
{
	int i;
	oscl_file_t *tmp_file = NULL;

	tmp_file = _find_oscl_file(fd);
	if (tmp_file == NULL)
		return;

	/* stop writer thread */
	tmp_file->stat = OSCL_FILE_CLOSE;
	sem_post(&tmp_file->sem);
	if (tmp_file->writer_thread) {
		OSCL_LOGD("==fd:%x", fd);
		pthread_join(tmp_file->writer_thread, NULL);
		tmp_file->writer_thread = 0;
	}
	sem_destroy(&tmp_file->sem);

	for (i = 0; i < tmp_file->cache_num; i++)
		_deinit_cache(&tmp_file->cache[i]);

	/* remove fd from manager */
	pthread_mutex_lock(&file_manager->lock);
	pthread_mutex_destroy(&tmp_file->rw_lock);
	pthread_mutex_destroy(&tmp_file->c_lock);
	memset(tmp_file, 0, sizeof(oscl_file_t));
	tmp_file->fd = -1;
	tmp_file->stat = OSCL_FILE_INIT;
	pthread_mutex_unlock(&file_manager->lock);
	return;
}

static int _resize_oscl_file(int fd, size_t size)
{
	int ret = 0;

#if defined(__EOS__)
#define IO_FTRUNCATE    0xfb

	if (fd < 0)
		return -1;
	ret = ioctl(fd, IO_FTRUNCATE, &size);
	fsync(fd);
#endif
	return ret;
}

void oscl_cache_fs_init(void)
{
	int i;
	if (file_manager != NULL)
		return;
	file_manager = oscl_zalloc(sizeof(oscl_file_manager_t));
	if (file_manager == NULL)
		return;
	pthread_mutex_init(&file_manager->lock, NULL);
	pthread_mutex_lock(&file_manager->lock);
	for (i = 0; i < MAX_FILE_NUM; i++)
		file_manager->file[i].fd = -1;
	pthread_mutex_unlock(&file_manager->lock);
}

void oscl_cache_fs_deinit(void)
{
	pthread_mutex_destroy(&file_manager->lock);
	oscl_free(file_manager);
	file_manager = NULL;
}

int oscl_cfile_realloc_cache(int fd, int num, int size)
{
	oscl_file_t *tmp_file = NULL;
	int i = 0;

	if ((num <= 0) || (size <= 0) || (fd < 0))
		return -1;

	tmp_file = _find_oscl_file(fd);
	oscl_param_check((num <= MAX_CACHE_NUM_PER_FILE), -1, NULL);
	oscl_param_check((tmp_file != NULL), -1, NULL);
	oscl_param_check((tmp_file->stat == OSCL_FILE_OPEN), -1, NULL);
	if ((tmp_file->cache_num == num) && (tmp_file->cache_size == size))
		return 0;
	if ((tmp_file->cache_num < num) && (tmp_file->cache_size == size)) {
		for (i = tmp_file->cache_num; i < num; i++)
			_init_cache(&tmp_file->cache[i], tmp_file->cache_size);
		tmp_file->cache_num = num;
		return 0;
	}

	for (i = 0; i < tmp_file->cache_num; i++)
		_deinit_cache(&tmp_file->cache[i]);

	tmp_file->cache_num = num;
	tmp_file->cache_size = size;
	for (i = 0; i < tmp_file->cache_num; i++)
		_init_cache(&tmp_file->cache[i], tmp_file->cache_size);

	return 0;
}

/**
 * this function will open a file and
 * return a file descriptor according specified flags.
 *
 * @param file the path name of file.
 * @param flags the file open flags.
 *
 * @return the non-negative integer on successful open, others for failed.
 */
int oscl_cfile_open(const char *file, int flags)
{
	int fd;
	if (file_manager == NULL)
		oscl_cache_fs_init();
	fd = open(file, flags);
	if (fd == -1) {
		OSCL_LOGE("open file error(%s)", strerror(errno));
		return -1;
	}
	_open_oscl_file(fd, file);
	OSCL_LOGD("==file:%s fd:%d", file, fd);
	return fd;
}

/**
 * this function will close the open
 * file descriptor.
 *
 * @param fd the file descriptor.
 *
 * @return 0 on successful, -1 on failed.
 */
int oscl_cfile_close(int fd)
{
	oscl_file_t *tmp_file;
	off_t file_pos;
	int ts_stream;

	tmp_file = _find_oscl_file(fd);
	if (tmp_file == NULL)
		return -1;
	OSCL_LOGD("==fd:%d", fd);
	ts_stream = tmp_file->ts_stream;
	_close_oscl_file(fd);
	OSCL_LOGD("==fd:%d", fd);
	if (ts_stream) {
		file_pos = lseek(fd, 0, SEEK_CUR);
		if (file_pos >= 0)
			_resize_oscl_file(fd, file_pos);
	}
	return close(fd);
}

/**
 * this function is to seek the offset for
 * an open file descriptor.
 *
 * @param fd the file descriptor.
 * @param offset the offset to be seeked.
 * @param whence the directory of seek.
 *
 * @return the current read/write position in the file, or -1 on failed.
 */
off_t oscl_cfile_lseek(int fd, off_t offset, int whence)
{
	oscl_file_t *tmp_file;
	off_t tmp_offset = 0;
	int i;
	off_t file_len = 0;

	tmp_file = _find_oscl_file(fd);
	/* not find in manager, call lseek */
	if (tmp_file == NULL)
		return lseek(fd, offset, whence);

	/* update filelen */
	pthread_mutex_lock(&tmp_file->rw_lock);
	if (whence == SEEK_CUR)
		tmp_offset = tmp_file->offset + offset;
	if (whence == SEEK_SET)
		tmp_offset = offset;
	if (tmp_offset < 0)
		tmp_offset = 0;
	if (tmp_file->file_len < tmp_file->offset)
		tmp_file->file_len = tmp_file->offset;
	if (tmp_offset <= tmp_file->file_len) {
		tmp_file->offset = tmp_offset;
		goto EXIT;
	}

	pthread_mutex_lock(&tmp_file->c_lock);
	file_len = lseek(fd, 0, SEEK_END);
	pthread_mutex_unlock(&tmp_file->c_lock);
	/* find min offset cache and write to file */
	for (i = 0; i < tmp_file->cache_num; i++) {
		pthread_mutex_lock(&tmp_file->cache[i].lock);
		if (tmp_file->cache[i].stat == OSCL_FILE_CACE_DIRT) {
			tmp_offset = tmp_file->cache[i].offset
				     + tmp_file->cache[i].cache_len;
			if (file_len < tmp_offset)
				file_len = tmp_offset;
		}
		pthread_mutex_unlock(&tmp_file->cache[i].lock);
	}
	tmp_file->file_len = file_len;

	if (whence == SEEK_CUR)
		tmp_offset = tmp_file->offset + offset;
	else if (whence == SEEK_SET)
		tmp_offset = offset;
	else if (whence == SEEK_END)
		tmp_offset = tmp_file->file_len + offset;
	else
		tmp_offset = tmp_file->offset;

	if (tmp_offset > tmp_file->file_len)
		tmp_offset = tmp_file->file_len;
	if (tmp_offset < 0)
		tmp_offset = 0;

	OSCL_LOGD("==file seek(%d) offset:%d, len:%d, seek:(%d, %d), ret:%d",
			fd, (int)tmp_file->offset, (int)file_len,
			whence, (int)offset, (int)tmp_offset);
	tmp_file->offset = tmp_offset;

EXIT:
	pthread_mutex_unlock(&tmp_file->rw_lock);
	return tmp_offset;
}

/**
 * this function is to read data from
 * an open file descriptor.
 *
 * @param fd the file descriptor.
 * @param buf output buffer.
 * @param len date len tobe read.
 *
 * @return the data len actually read.
 */
int oscl_cfile_read(int fd, void *buf, size_t len)
{
	oscl_file_t *tmp_file;
	int read_len;
	tmp_file = _find_oscl_file(fd);

	/* not find in manager, read from file */
	if (tmp_file == NULL)
		return read(fd, buf, len);

	/* write all cache buffer to file, and read from file */
	pthread_mutex_lock(&tmp_file->rw_lock);
	OSCL_LOGI("%d", (int)len);
	_clean_all_cache(tmp_file, 1);
	pthread_mutex_lock(&tmp_file->c_lock);
	/* must seek to offset saved, current offset may changed while write cache */
	lseek(fd, tmp_file->offset, SEEK_SET);
	read_len = read(fd, buf, len);
	/* update offset */
	tmp_file->offset = lseek(fd, 0, SEEK_CUR);
	tmp_file->cur_cache = NULL;
	pthread_mutex_unlock(&tmp_file->c_lock);
	pthread_mutex_unlock(&tmp_file->rw_lock);

	return read_len;
}

int oscl_cfile_write(int fd, const void *buf, size_t len)
{
	oscl_file_t *tmp_file;
	int write_len = 0;
	int tmp_len = 0;

	OSCL_TRACE("fd:%d, buf:%p, len:%x", fd, buf, (int)len);
	if (fd < 0 || buf == NULL || len == 0) {
		OSCL_LOGE("invalid para, fd=%d, buf=%p, len=%ld", fd, buf, (long)len);
		return 0;
	}

	tmp_file = _find_oscl_file(fd);
	/* not find in manager, read from file */
	if (tmp_file == NULL) {
		OSCL_LOGE("can not find :%d, write it to file use write", fd);
		return write(fd, buf, len);
	}
	pthread_mutex_lock(&tmp_file->rw_lock);
	while (len) {
		tmp_len = _write_to_buffer(tmp_file, buf, len);
		if (tmp_len == 0 || tmp_len > len) {
			OSCL_LOGE("write to buf err!check it");
			break;
		}
		write_len += tmp_len;
		buf += tmp_len;
		len -= tmp_len;
	}
	pthread_mutex_unlock(&tmp_file->rw_lock);
	return write_len;
}

int oscl_cfile_set_prio(int fd, int prio)
{
#if defined(__EOS__)
	oscl_file_t *tmp_file;
	int prio_set;
	int ret = -1;
	OSCL_TRACE("fd:%d, buf:%p, len:%x", fd, buf, len);
	if (fd < 0 || prio < 0)
		return -1;

	tmp_file = _find_oscl_file(fd);
	/* not find in manager, read from file */
	if (tmp_file == NULL) {
		OSCL_LOGE("can not find :%d, write it to file use write", fd);
		return -1;
	}
	prio_set = prio;
	if (tmp_file->writer_thread) {
		ret = rt_thread_control(tmp_file->writer_thread,
			RT_THREAD_CTRL_CHANGE_PRIORITY, &prio_set);
		if (ret == 0)
			tmp_file->prio = prio;
		else
			OSCL_LOGE("change prio failed!");
	}
	return ret;
#else
	return 0;
#endif
}

int oscl_cfile_get_prio(int fd)
{
#if defined(__EOS__)
	oscl_file_t *tmp_file;

	OSCL_TRACE("fd:%d, buf:%p, len:%x", fd, buf, len);
	if (fd < 0)
		return -1;

	tmp_file = _find_oscl_file(fd);
	/* not find in manager, read from file */
	if (tmp_file == NULL) {
		OSCL_LOGE("can not find :%d, write it to file use write", fd);
		return -1;
	}
	return tmp_file->prio;
#else
	return 0;
#endif
}

