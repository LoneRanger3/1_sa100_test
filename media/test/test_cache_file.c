/*
 * test_cache_file.c - test case for cache file.
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

#include <oscl.h>
#include <base_component.h>
#include <lb_omx_core.h>
#include <omx_test.h>
#include <stdlib.h>
#include <oscl_cache_file.h>

#if 1
int g_exit_flag;
#define CACHE_BUFFER_SIZE (64*1024)

char *g_src_buf;
char *g_tmp_buf1;
char *g_tmp_buf2;
int g_buf_len;
typedef struct {
	char func;
	int para1;
	int para2;
} fop_t;
fop_t op_case0[] = {
	{'w', 0, 0},
	{'w', 0, CACHE_BUFFER_SIZE},
	{'w', 0, 2 * CACHE_BUFFER_SIZE},
	{'w', 0, 3 * CACHE_BUFFER_SIZE},
	{'w', 0, 4 * CACHE_BUFFER_SIZE},
	{'w', 0, 1},
	{'w', 0, CACHE_BUFFER_SIZE - 1},
	{'w', 0, 1},
	{'w', 0, 2},
	{'w', 0, 3},
	{'w', 0, 4},
	{'w', 0, 5},
	{'w', 0, 6},
	{'w', 0, 7},
	{'w', 0, 8},
	{'w', 0, CACHE_BUFFER_SIZE - 2},
	{'w', 0, 2 * CACHE_BUFFER_SIZE},
	{'w', 0, 3 * CACHE_BUFFER_SIZE},
	{0, 0, 0},
};

typedef struct {
	char *src_name;
	char *dest_name;
	fop_t *fop;
} cfile_tester;

void *cfile_test_thread(void *param)
{
	cfile_tester *cfile_test_para = param;
	fop_t *fop;
	int len1, len2;
	int fd1, fd2;
	off_t offset1, offset2;
	int ret;
	int len = 0;
	int i = 200;

	OSCL_LOGE("");
	oscl_cache_fs_init();
	OSCL_LOGE("");
	OSCL_LOGE("cfile_test_thread:%s\n", rt_thread_self()->name);

	if (cfile_test_para == NULL || cfile_test_para->src_name == NULL
		|| cfile_test_para->dest_name == NULL || cfile_test_para->fop == NULL) {
		OSCL_LOGE("%x %x %x %x", cfile_test_para, cfile_test_para->src_name,
				  cfile_test_para->dest_name, cfile_test_para->fop);
		return NULL;
	}

	fd1 = open(cfile_test_para->src_name, O_RDONLY | O_WRONLY | O_CREAT);
	if (fd1 < 0) {
		OSCL_LOGE("open file err!");
		return NULL;
	}

	fd2 = oscl_cfile_open(cfile_test_para->dest_name, O_RDONLY | O_WRONLY | O_CREAT);
	if (fd2 < 0) {
		OSCL_LOGE("open file err!");
		close(fd1);
		return NULL;
	}
	fop = &cfile_test_para->fop[0];
	while (i--) {
		len = rand() % g_buf_len;
		if (len <= 0)
			len = 5;
		OSCL_LOGI("write:%d(%x), len:%d",
				fop->para1, g_src_buf, len);
		len2 = oscl_cfile_write(fd2, g_src_buf, len);
		len1 = write(fd1, g_src_buf, len);
		if (len1 != len2)
			OSCL_LOGE("write file faile(%d %d)!", len1, len2);
	}
	while (fop->func != 0) {
		switch (fop->func) {
		case 'w':
			len = rand() % g_buf_len;
			if (len <= 0)
				len = 5;
			OSCL_LOGI("write:%d(%x), len:%d",
					fop->para1, g_src_buf, len);
			len2 = oscl_cfile_write(fd2, g_src_buf + fop->para1, len);
			len1 = write(fd1, g_src_buf + fop->para1, len);
			if (len1 != len2)
				OSCL_LOGE("write file faile(%d %d)!", len1, len2);
			break;
		case 'r':
			OSCL_LOGI("read:%d, len:%d", fop->para1, fop->para2);
			len1 = read(fd1, g_tmp_buf1, fop->para2);
			len2 = oscl_cfile_read(fd2, g_tmp_buf2, fop->para2);
			if ((len1 != len2)
				|| memcmp(g_tmp_buf1, g_tmp_buf2, fop->para2) != 0)
				OSCL_LOGE("read file faile(%d %d)!", len1, len2);
			break;
		case 'l':
			OSCL_LOGI("seek:%d, len:%d", fop->para1, fop->para2);
			offset1 = lseek(fd1, fop->para1, fop->para2);
			offset2 = oscl_cfile_lseek(fd2, fop->para1, fop->para2);
			if (offset1 != offset2)
				OSCL_LOGE("write file faile(%ld %ld)!", offset1, offset2);
			break;
		default:
			OSCL_LOGE("err operateion:%d", fop->func);
			break;
		}
		fop++;
	}
	ret = close(fd1);
	OSCL_LOGE("close fd1:%d,%d", fd1, ret);
	ret = oscl_cfile_close(fd2);
	OSCL_LOGE("close fd2:%d,%d", fd2, ret);

	OSCL_LOGE("oscl_cache_fs_writer_thread:%s\n", rt_thread_self()->name);
	pthread_exit(NULL);

	return NULL;
}


int write_file_test(int argc, char **argv)
{
	cfile_tester cfile_test_para1;
	cfile_tester cfile_test_para2;
	pthread_t file_op_thread1;
	pthread_t file_op_thread2;
	int i;
	int ret;
	int nretry = 5;

	g_buf_len = 1 * 1024 * 1024;
	g_src_buf = oscl_malloc(g_buf_len);
	g_tmp_buf1 = oscl_malloc(g_buf_len);
	g_tmp_buf2 = oscl_malloc(g_buf_len);
	if (g_src_buf == NULL || g_tmp_buf1 == NULL || g_tmp_buf2 == NULL) {
		OSCL_LOGE("malloc buf err!");
		oscl_free(g_src_buf);
		oscl_free(g_tmp_buf1);
		oscl_free(g_tmp_buf2);
		return -1;
	}
	for (i = 0; i < (g_buf_len / 4); i++)
		*(int *)(g_src_buf + i * 4) = i;
	cfile_test_para1.src_name = "src1.bin";
	cfile_test_para1.dest_name = "dest1.bin";
	cfile_test_para1.fop = op_case0;
	cfile_test_para2.src_name = "src2.bin";
	cfile_test_para2.dest_name = "dest2.bin";
	cfile_test_para2.fop = op_case0;
	if (g_exit_flag) {
		compare_in_out("src1.bin", "dest1.bin");
		compare_in_out("src2.bin", "dest2.bin");
	}
	OSCL_LOGE("write_file_test:%s\n", rt_thread_self()->name);
	while (g_exit_flag == 0) {
		pthread_create(&file_op_thread1, NULL,
					   cfile_test_thread, &cfile_test_para1);

		pthread_create(&file_op_thread2, NULL,
					   cfile_test_thread, &cfile_test_para2);
		pthread_join(file_op_thread1, NULL);
		pthread_join(file_op_thread2, NULL);
		ret = 0;
		nretry = 5;
		while (nretry--) {
			ret = compare_in_out(cfile_test_para1.src_name,
				cfile_test_para1.dest_name);
			if (ret == 0)
				break;
		}
		if (ret < 0)
			break;
		ret = 0;
		nretry = 5;
		while (nretry--) {
			ret = compare_in_out(cfile_test_para2.src_name,
				cfile_test_para2.dest_name);
			if (ret == 0)
				break;
		}
		if (ret < 0)
			break;
	}
	return 0;
}
MSH_CMD_EXPORT(write_file_test, "write_file_test");
#endif

