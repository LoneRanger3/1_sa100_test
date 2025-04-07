/*
 * fallocate_test.c - test fallocate function.
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

#define DBG_LEVEL		DBG_LOG

#include <dfs_posix.h>
#include <oscl.h>
#include "oscl_fallocate_file.h"

#define WRITE_BUFLEN		(128*1024)
#define WRITE_FILESIZE_MB	(40)
#define WRITE_FILESIZE		(WRITE_FILESIZE_MB*1024*1024)
#define CARD_SIZE_MB		(29*1024)
#define MAX_CARD_FILE		((CARD_SIZE_MB / (WRITE_FILESIZE_MB+8)) - 2)

typedef struct fallocate_test_para {
	pthread_t thread;
	int thread_running;
	char *name;
} fallocate_test_para_t;

static fallocate_test_para_t g_fallocate_test[] = {
	{
		.thread_running = 0,
		.name = "stream0",
	},
	{
		.thread_running = 0,
		.name = "stream1",
	}
};
static int g_filenum = 10;
static int s_total_cnt = 1;

static void *fallocate_test_write_thd(void *param)
{
	fallocate_test_para_t *test_param = param;
	void *fallocate;
	char filename[128];
	int cnt = 0, del_cnt = 0;
	char *wbuf;

	OSCL_LOGI("start");
	wbuf = (char *)oscl_malloc(WRITE_BUFLEN);
	memset(wbuf, 'a', WRITE_BUFLEN);
	fallocate = oscl_fallocate_init();
	if (fallocate == NULL) {
		OSCL_LOGE("oscl_fallocate_init error!");
		goto EXIT;
	}
	oscl_fallocate_set_filesize(fallocate, WRITE_FILESIZE);
	while (test_param->thread_running) {
		int fd;
		size_t total_wlen = 0;

		if (cnt >= g_filenum) {
			test_param->thread_running = 0;
			break;
		}

		if (s_total_cnt >= MAX_CARD_FILE) {
			snprintf(filename, 128, "/mnt/sdcard/fallocate_test_%s_%d.mp5",
				test_param->name, del_cnt);
			OSCL_LOGI("unlinking file %s", filename);
			if (unlink(filename) == -1) {
				OSCL_LOGE("Failed to unlink temp file \"%s\"!",
					filename);
			}
			del_cnt++;
		}

		snprintf(filename, 128, "/mnt/sdcard/fallocate_test_%s_%d.mp5",
			test_param->name, cnt);
		OSCL_LOGI("opening file %s", filename);
		fd = oscl_fallocate_open(fallocate, filename, O_RDWR | O_CREAT);
		if (fd < 0) {
			OSCL_LOGE("oscl_fallocate_open file \"%s\" error!", filename);
			break;
		}
		while (test_param->thread_running &&
			(total_wlen+WRITE_BUFLEN < WRITE_FILESIZE)) {
			ssize_t len = write(fd, wbuf, WRITE_BUFLEN);
			if (len < 0) {
				OSCL_LOGE("write file %s error!", filename);
				break;
			}
			total_wlen += len;
		}
		oscl_fallocate_close(fallocate, fd);
		++cnt;
		++s_total_cnt;
	}

EXIT:
	if (fallocate != NULL)
		oscl_fallocate_deinit(fallocate);
	if (wbuf != NULL)
		oscl_free(wbuf);
	OSCL_LOGI("end");
	pthread_exit(NULL);
	return NULL;
}

int fallocate_test(int argc, char **argv)
{
	pthread_attr_t thread_attr;
	int is_active[2];
	int i;

	if (argc < 4) {
		OSCL_LOGW("Usage: %s 0/1/a start/stop filenum");
		return 0;
	}

	g_filenum = atoi(argv[3]);

	s_total_cnt = 0;

	OSCL_LOGI("start, test num %d", g_filenum);

	is_active[0] = 0;

	is_active[1] = 0;

	if (argv[1][0] == '0') {
		is_active[0] = 1;
	} else if (argv[1][0] == '1') {
		is_active[1] = 1;
	} else {
		is_active[0] = 1;
		is_active[1] = 1;
	}
	if (!strcmp(argv[2], "start")) {
		for (i = 0; i < 2; ++i) {
			if (is_active[i] && !g_fallocate_test[i].thread_running) {
				g_fallocate_test[i].thread_running = 1;
				pthread_attr_init(&thread_attr);
				pthread_attr_setstacksize(&thread_attr, 0x1000);
				pthread_create(&g_fallocate_test[i].thread, &thread_attr,
					fallocate_test_write_thd, &g_fallocate_test[i]);
			}
		}
	} else if (!strcmp(argv[2], "stop")) {
		for (i = 0; i < 2; ++i) {
			if (is_active[i] && g_fallocate_test[i].thread_running) {
				g_fallocate_test[i].thread_running = 0;
				pthread_join(g_fallocate_test[i].thread, NULL);
			}
		}
	} else {
		OSCL_LOGW("Invalid argument \"%s\"", argv[2]);
	}

	OSCL_LOGI("end");
	return 0;
}

MSH_CMD_EXPORT(fallocate_test, "fallocate test");
