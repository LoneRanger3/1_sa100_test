/*
 * sys_test.c - eos system base service test
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

#include <rtthread.h>
#include <semaphore.h>
#include <debug.h>
#include <ipc/completion.h>
#include "board.h"

#define TEST_ACCESS_PERM
/* #define TEST_TIME */
/* #define TEST_COMPLETION */
/* #define TEST_SEM */
/* #define TEST_POSIX_FILE */

typedef rt_err_t  (*test_func)(void);

#ifdef TEST_SEM
#define SEM_TEST_SEC	5 /* sem test time, in seconds */
#define SEM_VALUE_TEST	2 /* sem init count */

struct sem_test {
	sem_t	*psem;		/* sem to test */
	int	need_exit;	/* if the sem test thread need exit */
};
void *sem_test_thread(void *para)
{
	struct sem_test *pst = para;

	LOG_I("start");

	RT_ASSERT(RT_NULL != pst);

	while (!pst->need_exit) { /* check if the thread need exit */
		LOG_I("start sem_post...");

		/* post the sem */
		if (sem_post(pst->psem) != 0)
			LOG_E("sem_post failed");
		else
			LOG_I("sem_post success");

		rt_thread_delay(100);
	}

	LOG_I("end");

	/* exit the sem test thread */
	pthread_exit(0);
	return 0;
}

rt_err_t test_sem_open_close(void)
{
	rt_tick_t start_ms, test_sec = 10;
	char name[] = "test_sem";
	sem_t *data_sem = NULL;
	INIT_DUMP_MEM;

	LOG_I("start");

	start_ms = rt_time_get_msec();
	while (rt_time_get_msec() - start_ms < test_sec * 1000) {
		DUMP_MEMORY();

		data_sem = sem_open(name, O_CREAT, 0644, 0);
		if (NULL == data_sem) {
			LOG_E("sem open %s fail!", name);
			return -RT_ERROR;
		}
		LOG_I("sem open %s success!", name);

		if (sem_unlink(name)) {
			LOG_E("sem_unlink %s fail!", name);
			return -RT_ERROR;
		}
		LOG_I("sem_unlink %s success!", name);

		if (sem_close(data_sem)) {
			LOG_E("sem_close %s fail!", name);
			return -RT_ERROR;
		}
		LOG_I("sem_close %s success!", name);

		DUMP_MEMORY();
		rt_thread_delay(10);
	}

	LOG_I("end");
	return RT_EOK;
}

rt_err_t test_sys_sem(void)
{
	rt_thread_t tid = RT_NULL;
	rt_tick_t start_ms, cur_ms;
	rt_err_t ret = RT_EOK;
	struct sem_test st;
	sem_t sem_temp;
	int i;

	LOG_I("start");

	/* test sem_open/sem_close first */
	if (test_sem_open_close()) {
		LOG_E("test_sem_open_close failed");
		return -RT_ERROR;
	}
	LOG_I("test_sem_open_close success");

	/* init the sem */
	if (sem_init(&sem_temp, 0, SEM_VALUE_TEST)) {
		LOG_E("sem_init failed");
		return -RT_ERROR;
	}
	LOG_I("sem_init success");

	/* create sem test thread */
	st.psem = &sem_temp;
	st.need_exit = 0;
	if (pthread_create(&tid, 0, &sem_test_thread, &st) != 0) {
		LOG_E("create sem test thread failed");
		ret = -RT_ERROR;
		goto end;
	}
	LOG_I("create sem test thread success");

	/* continue test until time end */
	start_ms = rt_time_get_msec();
	cur_ms = start_ms;
	while (cur_ms - start_ms < SEM_TEST_SEC * 1000) {
		/* wait for sem */
		for (i = 0; i < SEM_VALUE_TEST + 3; i++) {
			LOG_I("start wait sem%d...", i);
			if (sem_wait(&sem_temp) != 0)
				LOG_E("wait sem%d failed", i);
			else
				LOG_I("wait sem%d success", i);
		}

		cur_ms = rt_time_get_msec();
	}

	/* wait until the sem test thread exit */
	LOG_I("wait sem test thread exit..");
	st.need_exit = 1;
	pthread_join(tid, RT_NULL);

end:
	/* destory the sem */
	sem_destroy(&sem_temp);

	LOG_I("end");
	return ret;
}
#endif /* TEST_SEM */

#ifdef TEST_POSIX_FILE
#define ORG_FILE	"/testfile"
#define NEW_FILE	"/testfile-new"
#define FILE_BUF_LEN	64

#define FILE_TEST_SEC	20

void dumpbuf(char *addr, int size)
{
	int i = 0;

	LOG_I("---- dump buf for [0x%08x, 0x%08x] ----",
		(int)addr, (int)(addr + size));

	for (i = (int)addr; i < (int)(addr + size); i += 16) {
		LOG_I("0x%08x: 0x%08x 0x%08x 0x%08x 0x%08x", i,
			READREG32(i + 0), READREG32(i + 4),
			READREG32(i + 8), READREG32(i + 12));
	}
}

int copy_file(char *src_file, char *dst_file)
{
	int fd1 = -1, fd2 = -1, len1, len2, file_end = 0;
	char buf[FILE_BUF_LEN];
	int ret = -1;

	fd1 = open(src_file, O_RDONLY, 0);
	if (fd1 < 0) {
		LOG_E("open file %s failed", src_file);
		goto end;
	}
	LOG_I("open file %s success, ret fd %d", src_file, fd1);

	fd2 = open(dst_file, O_WRONLY | O_CREAT | O_TRUNC, 0);
	if (fd2 < 0) {
		LOG_E("open file %s failed", dst_file);
		goto end;
	}
	LOG_I("open file %s success, ret fd %d", dst_file, fd2);

	/* copy file */
	do {
		len1 = read(fd1, buf, sizeof(buf));
		if (len1 != sizeof(buf)) {
			LOG_W("read file %s ret(len) %d != buf_size(%d), end of file?",
				src_file, len1, sizeof(buf));
			file_end = 1;
		}

		len2 = write(fd2, buf, len1);
		if (len2 != len1) {
			LOG_E("write file %s ret(len) %d != buf_size %d",
				dst_file, len2, len1);
			goto end;
		}
	} while (!file_end);

	ret = 0;
end:
	if (fd1 >= 0)
		close(fd1);
	if (fd2 >= 0)
		close(fd2);
	return ret;
}

int compare_file(char *file1, char *file2)
{
	char buf1[FILE_BUF_LEN], buf2[FILE_BUF_LEN];
	int fd1 = -1, fd2 = -1, len1, len2;
	int file_end1 = 0, file_end2 = 0;
	int ret = -1;

	fd1 = open(file1, O_RDONLY, 0);
	if (fd1 < 0) {
		LOG_E("open file %s failed", file1);
		goto end;
	}
	LOG_I("open file %s success, ret fd %d", file1, fd1);

	fd2 = open(file2, O_RDONLY, 0);
	if (fd2 < 0) {
		LOG_E("open file %s failed", file2);
		goto end;
	}
	LOG_I("open file %s success, ret fd %d", file2, fd2);

	/* compare file contents */
	do {
		len1 = read(fd1, buf1, sizeof(buf1));
		if (len1 != sizeof(buf1)) {
			LOG_W("read file %s ret(len) %d != buf_size(%d), end of file?",
				file1, len1, sizeof(buf1));
			file_end1 = 1;
		}

		len2 = read(fd2, buf2, sizeof(buf2));
		if (len2 != sizeof(buf2)) {
			LOG_W("read file %s ret(len) %d != buf_size(%d), end of file?",
				file2, len2, sizeof(buf2));
			file_end2 = 1;
		}

		if (file_end1 != file_end2) {
			LOG_E("some err occur, file1 reach end while file2 not");
			goto end;
		} else if ((len1 | len2) && memcmp(buf1, buf2, len1)) {
			LOG_E("compare failure, buf1 content != buf2, len1 %d, len2 %d",
				len1, len2);
			dumpbuf(buf1, len1);
			dumpbuf(buf2, len2);
			goto end;
		}
	} while (!file_end1);

	ret = 0;
end:
	if (fd1 >= 0)
		close(fd1);
	if (fd2 >= 0)
		close(fd2);
	return ret;
}

void *read_write_test(void *para)
{
	if (copy_file(ORG_FILE, NEW_FILE)) {
		LOG_E("copy file %s to %s failed", ORG_FILE, NEW_FILE);
		return 0;
	}

	LOG_I("copy file %s to %s success", ORG_FILE, NEW_FILE);

	if (compare_file(ORG_FILE, NEW_FILE)) {
		LOG_E("compare file %s and %s failed", ORG_FILE, NEW_FILE);
		return 0;
	}

	LOG_I("compare file %s and %s success", ORG_FILE, NEW_FILE);

	return 0;
}

rt_err_t test_sys_file(void)
{
	int start_ms, cur_ms;
	int need_exit1 = 0;
	rt_thread_t tid1 = NULL;
	rt_err_t ret = RT_EOK;

	if (pthread_create(&tid1, 0, &read_write_test, &need_exit1) != 0) {
		LOG_E("create read_write_test failed!\n");
		goto end;
	}

	/* continue test until time end */
	start_ms = rt_time_get_msec();
	cur_ms = start_ms;
	while (cur_ms - start_ms < FILE_TEST_SEC * 1000) {
		rt_thread_delay(5);
		cur_ms = rt_time_get_msec();
	}

end:
	/* let the test thread exit */
	if (NULL != tid1) {
		need_exit1 = 1;
		pthread_join(tid1, NULL);
		tid1 = NULL;
	}

	LOG_E("end");
	return ret;
}
#endif /* TEST_POSIX_FILE */

#ifdef TEST_COMPLETION
#define COMPLETION_TEST_SEC	5	/* completion test time, in seconds */

struct comp_test {
	struct rt_completion comp;	/* completion to test */
	int need_exit;			/* if the thread need exit */
};

void *comp_test_thread(void *para)
{
	struct comp_test *pct = para;

	LOG_I("start");

	RT_ASSERT(RT_NULL != pct);

	LOG_I("start wait completion...");

	/* wait complition */
	rt_completion_wait(&pct->comp, RT_WAITING_FOREVER);

	LOG_I("wait completion success!");

	LOG_I("end");
	return 0;
}

rt_err_t test_sys_comp(void)
{
	rt_thread_t tid = RT_NULL;
	rt_err_t ret = RT_EOK;
	struct comp_test st;

	LOG_I("start");

	/* init the comp */
	LOG_I("init completion..");
	rt_completion_init(&st.comp);

	/* create comp test thread */
	st.need_exit = 0;
	if (pthread_create(&tid, 0, &comp_test_thread, &st) != 0) {
		LOG_E("create comp_test_thread failed");
		ret = -RT_ERROR;
		goto end;
	}
	LOG_I("create comp_test_thread success");

	/* to let comp_test_thread start wait the complition */
	rt_thread_delay(100);

	/* completion done */
	LOG_I("before rt_completion_done..");
	rt_completion_done(&st.comp);
	LOG_I("after rt_completion_done!");

	/* wait until the comp test thread exit */
	LOG_I("wait comp_test_thread exit..");
	st.need_exit = 1;
	pthread_join(tid, RT_NULL);

end:
	LOG_I("end");
	return ret;
}
#endif /* TEST_COMPLETION */

#ifdef TEST_TIME
int test_sys_time(void)
{
	int i, ret = -RT_EOK;
	struct tm *p_tm;
	time_t now;

	for (i = 0; i < 10; i++) {
		now = time(RT_NULL);
		p_tm = localtime(&now);

		LOG_I("time: %02d:%02d:%02d", p_tm->tm_hour,
			p_tm->tm_min, p_tm->tm_sec);

		rt_thread_delay(100);
	}

	return ret;
}
#endif /* TEST_TIME */

#ifdef TEST_ACCESS_PERM
/* #define CASE_WRITE_RO_RGN */
#define CASE_WRITE_TEXT_RGN

#ifdef CASE_WRITE_RO_RGN
const char test_ap_str[8] = "abcde";
const int test_ap_int = 0x5a5a;
#endif
rt_err_t test_sys_ap(void)
{
	int ret = -RT_EOK;

#ifdef CASE_WRITE_RO_RGN
	volatile char *pch;

	/*
	 * access const(readonly) areas
	 */
	pch = (volatile char *)test_ap_str;

	/* data abort: attemp to write a read-only region */
	/* pch[6] = 'x'; */

	/* compile err: assignment of read-only location */
	/* str[1] = 'h'; */

	/* data abort: attemp to write a read-only region */
	*(volatile int *)&test_ap_int = 0xff;
#endif

#ifdef CASE_WRITE_TEXT_RGN
	/*
	 * access text(readonly, execute) areas
	 */
	/* data abort: attemp to write a read-only region */
	memset(time, 0, sizeof(time));
#endif

	return ret;
}
#endif /* TEST_ACCESS_PERM */

static test_func test_funcs[] = {
#ifdef TEST_SEM
	test_sys_sem,
#endif
#ifdef TEST_POSIX_FILE
	test_sys_file,
#endif
#ifdef TEST_COMPLETION
	test_sys_comp,
#endif
#ifdef TEST_TIME
	test_sys_time,
#endif
#ifdef TEST_ACCESS_PERM
	test_sys_ap,
#endif
};

/**
 * test_sys - test for timer
 *
 * return 0 if success, -RT_ERROR if failed
 */
long test_sys(int argc, char **argv)
{
	long ret = 0;
	int i;

	LOG_I("start");

	for (i = 0; i < ARRAY_SIZE(test_funcs); i++) {
		ret = test_funcs[i]();
		if (ret)
			goto end;
	}

	LOG_I("end");
end:
	if (ret)
		LOG_E("failed!");
	return ret;
}

