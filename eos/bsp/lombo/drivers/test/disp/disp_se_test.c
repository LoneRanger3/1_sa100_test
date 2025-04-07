/*
 * disp_se_test.c - Disp se test module driver code for LomboTech
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
#include "disp_test.h"

typedef enum buf_state {
	BUF_STA_IDLE,
	BUF_STA_BUSYING,
} buf_state_t;

/* for se buf */
typedef struct se_data {
	buf_state_t	buf_sta;
	char		*buf;
	u8		protect_cnt;
} se_data_t;
se_data_t se_dat[SE_BUF_NUM];

/* for offline  se */
typedef struct se_offline_test_tag {
	const char *name;
	u32 out_fmt;
} se_offline_test_t;

se_data_t se_dat[SE_BUF_NUM];

static rt_mutex_t se_buf_mutex;
static rt_mq_t se_buf_mq_p;
static struct rt_messagequeue se_buf_mq;
static char se_buf_msg_pool[1024];
static bool se_test_start_flag;

static void se_buf_mq_init(void)
{
	rt_err_t ret;

	se_buf_mq_p = &se_buf_mq;

	ret = rt_mq_init(se_buf_mq_p, "se_buf_mq",
		&se_buf_msg_pool[0],
		128 - sizeof(void *),
		sizeof(se_buf_msg_pool),
		RT_IPC_FLAG_FIFO);

	RT_ASSERT(ret == RT_EOK);
}

static void se_buf_mq_uninit(void)
{
	rt_err_t ret;

	if (se_buf_mq_p != NULL) {
		ret = rt_mq_detach(se_buf_mq_p);
		RT_ASSERT(ret == RT_EOK);
		se_buf_mq_p = NULL;
	}
}

static void se_buf_lock(void)
{
	rt_err_t result;
	result = rt_mutex_take(se_buf_mutex, RT_WAITING_FOREVER);
	RT_ASSERT(result == RT_EOK);
}

static void se_buf_unlock(void)
{
	rt_mutex_release(se_buf_mutex);
}

static void se_buf_mutex_init(void)
{
	se_buf_mutex = rt_mutex_create("se_buf_mutex", RT_IPC_FLAG_FIFO);
	RT_ASSERT(se_buf_mutex != NULL);
}

static void se_buf_mutex_uninit(void)
{
	rt_err_t ret;
	if (se_buf_mutex != NULL) {
		ret = rt_mutex_delete(se_buf_mutex);
		RT_ASSERT(ret == RT_EOK);
	}
}

static bool deque_buf(u8 *num_p)
{
	u8 i;

	se_buf_lock();
	for (i = 0; i < SE_BUF_NUM; i++) {
		if (se_dat[i].buf_sta == BUF_STA_IDLE) {
			*num_p = i;
			if (se_dat[i].protect_cnt > 0) {
				se_dat[i].protect_cnt--;
			} else {
				se_dat[i].buf_sta = BUF_STA_BUSYING;
				se_buf_unlock();
				return true;
			}
		}
	}
	se_buf_unlock();

	return false;
}

static void enque_buf(u8 *num_p)
{
	u8 num = *num_p;

	if (num >= SE_BUF_NUM)
		LOG_E("enque_buf overflow");

	se_buf_lock();
	se_dat[num].buf_sta = BUF_STA_IDLE;
	se_dat[num].protect_cnt = SE_BUF_NUM - 2;
	se_buf_unlock();
}

static void thread_read_sd_entry(void *parameter)
{
	bool se_test_start_flag_tmp;
	u8 cur_buf = 0;
	u32 i, cnt, try_cnt, file_len, offset;
	/* rt_tick_t t0, t1; */
	int fd, size, read_size;
	char *se_buffer[SE_BUF_NUM];
	char file_name[64];

	LOG_I("thread_read_sd_entry start");
	size = VIDEO_SIZE;
	offset = 0;
	for (i = 0; i < SE_BUF_NUM; i++) {
		se_buffer[i] =  rt_malloc_align(size, 16);
		RT_ASSERT(se_buffer[i] != NULL);
		se_dat[i].buf = se_buffer[i];
	}

	try_cnt = 5;
	disp_delay(500);

	rt_sprintf(file_name, "%s%s", SRC_FILE_PATH, VIDEO_NAME);
	fd = open(file_name, O_RDONLY);
	while (disp_test_start_flag && (fd <= 0) && (try_cnt > 0)) {
		/* LOG_I("open ok.yuv again"); */
		if (try_cnt)
			fd = open(file_name, O_RDONLY);
		try_cnt--;
		disp_delay(200);
	}

	if (fd <= 0) {
		LOG_E("open file[%s] err", file_name);
		return;
	}

	file_len = lseek(fd, 0, SEEK_END);

	lseek(fd, 0, SEEK_SET);

	/* LOG_I("file_len[%d]", file_len); */

	while (disp_test_start_flag) {
		if (fd > 0) {
			if (deque_buf(&cur_buf)) {
				/* t0 = disp_get_tick(); */
				read_size = read(fd, se_buffer[cur_buf], size);
				if (read_size != size) {
					close(fd);
					LOG_E("read image data err");
					return;
				}
				/* t1 = disp_get_tick(); */
				/* rt_kprintf("***sd test[%d]\n", t1 - t0); */

				offset += size;
				if (offset >= file_len) {
					lseek(fd, 0, SEEK_SET);
					LOG_I("begin again");
					cnt = 0;
					offset = 0;
				}

				if (se_buf_mq_p != NULL)
					rt_mq_send(se_buf_mq_p,
							&cur_buf, sizeof(cur_buf));
			} else {
				/* rt_kprintf("que full sleep"); */
				disp_delay(10);
			}
		} else {
			LOG_E("open ok.yuv error");
			break;
		}
		cnt++;
	};

	rt_mq_control(se_buf_mq_p, RT_IPC_CMD_RESET, NULL);

	if (fd > 0)
		close(fd);

	se_buf_lock();
	se_test_start_flag_tmp = se_test_start_flag;
	se_buf_unlock();
	while (se_test_start_flag_tmp) {
		disp_delay(10);
		se_buf_lock();
		se_test_start_flag_tmp = se_test_start_flag;
		se_buf_unlock();
	}

	se_buf_mq_uninit();
	se_buf_mutex_uninit();

	for (i = 0; i < SE_BUF_NUM; i++)
		rt_free_align(se_buffer[i]);

	rt_memset(&se_dat, 0x00, sizeof(se_dat));
	LOG_I("thread_read_sd_entry end");
}

static void thread_disp_se0_test_entry(void *parameter)
{
	char *se_buf;
	u8 cur_buf = 0;
	/* rt_tick_t t0, t1; */
	u32 cnt = 0, update_cnt = 0;
	u32 frame_size = VIDEO_W * VIDEO_H;
	dc_win_data_t win_data;
	rt_device_disp_ops_t *disp_ops_p;

	LOG_I("thread_disp_se0_test_entry start");
	disp_ops_p = (rt_device_disp_ops_t *)parameter;
	RT_ASSERT(disp_ops_p != NULL);

	rt_memset(&win_data, 0, sizeof(dc_win_data_t));

	win_data.pixel_format = VIDEO_FMT;
	win_data.pixel_order = DC_PO_NORMAL;
	win_data.bpp = 32;

	#ifdef ARCH_LOMBO_N7V1_CDR
	win_data.crtc_x = 0;
	#else
	win_data.crtc_x = 200;
	#endif
	win_data.crtc_y = 100;
	win_data.crtc_width = 320;
	win_data.crtc_height = 240;

	win_data.fb_x = 0;
	win_data.fb_y = 0;
	win_data.fb_width = VIDEO_W;
	win_data.fb_height = VIDEO_H;

	win_data.src_width = VIDEO_W;
	win_data.src_height = VIDEO_H;

	win_data.update_flag = true;

	se_buf_lock();
	se_test_start_flag = true;
	se_buf_unlock();

	while (disp_test_start_flag) {
		if (rt_mq_recv(se_buf_mq_p, &cur_buf, sizeof(cur_buf),
			RT_WAITING_FOREVER) == RT_EOK) {
			/* rt_kprintf("cur_buf[%d]\n", cur_buf); */
			RT_ASSERT(cur_buf < SE_BUF_NUM);
			se_buf = se_dat[cur_buf].buf;

			win_data.dma_addr = (u32)se_buf;
			win_data.chroma_dma_addr = (u32)(se_buf + frame_size);
			win_data.chroma_x_dma_addr = (u32)(se_buf + frame_size * 5 / 4);

			/* t0 = disp_get_tick(); */
			disp_ops_p->disp_win_update(wctl[2], &win_data);
			/* t1 = disp_get_tick(); */
			/* rt_kprintf("***se test[%d]\n", t1 - t0); */
			cnt++;
#if 0
			if (cnt % 10 == 0)
				rt_kprintf("cnt[%d]", cnt);
#endif
#if !POWER_TEST
			update_cnt = cnt % 70;
			#ifdef ARCH_LOMBO_N7V1_CDR
			win_data.crtc_y = update_cnt * 5;
			#else
			win_data.crtc_x = update_cnt * 10;
			win_data.crtc_y = update_cnt * 5;
			#endif
#endif

			enque_buf(&cur_buf);
			disp_delay(20);
		}
	};

	se_buf_lock();
	se_test_start_flag = false;
	se_buf_unlock();
	LOG_I("thread_disp_se0_test_entry end");
}

#if !OFFLINE_SE_TEST
static void thread_disp_se1_test_entry(void *parameter)
{
	int fd;
	u32 img_w, img_h, read_size, size;
	u32 cnt = 0, update_cnt = 0;
	char *buf_data;
	dc_win_data_t win_data;
	rt_device_disp_ops_t *disp_ops_p;
	char file_name[64];

	LOG_I("thread_disp_se1_test_entry start");
	disp_ops_p = (rt_device_disp_ops_t *)parameter;
	RT_ASSERT(disp_ops_p != NULL);

	rt_sprintf(file_name, "%s%s", SRC_FILE_PATH, SE1_IMAGE_NAME);
	fd = open(file_name, O_RDONLY);
	if (fd <= 0) {
		LOG_E("open file[%s] err", file_name);
		return;
	}

	img_w = 640;
	img_h = 480;

	size = img_w * img_h * 3 / 2;
	buf_data = rt_malloc_align(size, 16);
	RT_ASSERT(buf_data != NULL);

	read_size = read(fd, buf_data, size);
	LOG_D("read size %d", read_size);
	if (read_size != size) {
		close(fd);
		LOG_E("read image data err");
		return;
	}

	if (fd > 0)
		close(fd);

	win_data.dma_addr = (u32)buf_data;
	win_data.chroma_dma_addr = (u32)(buf_data + img_w * img_h);
	win_data.pixel_format = DISP_FORMAT_NV12;
	win_data.pixel_order = DC_PO_NORMAL;
	win_data.bpp = 32;

	win_data.crtc_x = 0;
	win_data.crtc_y = 0;
	#ifdef ARCH_LOMBO_N7V1_CDR
	win_data.crtc_y += CDR_START_Y;
	#endif
	win_data.crtc_width = img_w / 4;
	win_data.crtc_height = img_h / 4;

	win_data.fb_x = 0;
	win_data.fb_y = 0;
	win_data.fb_width = img_w;
	win_data.fb_height = img_h;

	win_data.src_width = img_w;
	win_data.src_height = img_h;

	win_data.update_flag = true;

	while (disp_test_start_flag) {
		disp_ops_p->disp_win_update(wctl[3], &win_data);
		disp_delay(20);
		cnt++;
#if 0
		if (cnt % 50 == 0)
			rt_kprintf("dc cnt[%d]\n", cnt);
#endif

		update_cnt = cnt % 22;
		win_data.crtc_x = update_cnt * 10 + 50;

		if (cnt == 300) {
			LOG_I("thread_disp_se1_test_entry change se size");
			win_data.crtc_width = img_w / 2;
			win_data.crtc_height = img_h / 2;
		}
	};

	rt_free_align(buf_data);

	LOG_I("thread_disp_se1_test_entry end");
}
#else
static void thread_disp_offline_se_test_entry(void *parameter)
{
	int fd, fd_o, i;
	u32 img_w, img_h, real_size, read_size, write_size, size, size_o;
	char *buf_data;
	char *buf_out;
	u32 out_addr[3] = {0};
	dc_win_data_t win_data;
	rt_device_disp_ops_t *disp_ops_p;
	char in_file_name[64], out_file_name[64];
	disp_se_t *dse = NULL;
	u32 out_format;
	se_offline_test_t se_out_para[4];

	LOG_I("thread_disp_offline_se_test_entry start");

	se_out_para[0].name = "WB_YUV420P";
	se_out_para[1].name = "WB_YUV420SP";
	se_out_para[2].name = "WB_ARGB8888";
	se_out_para[3].name = "WB_RGB565";

	se_out_para[0].out_fmt = DISP_FORMAT_YUV420;
	se_out_para[1].out_fmt = DISP_FORMAT_NV12;
	se_out_para[2].out_fmt = DISP_FORMAT_ARGB8888;
	se_out_para[3].out_fmt = DISP_FORMAT_RGB565;

	disp_ops_p = (rt_device_disp_ops_t *)parameter;
	RT_ASSERT(disp_ops_p != NULL);

	dse = disp_ops_p->disp_se_request();
	if (NULL == dse) {
		LOG_E("disp_se_request err");
		return;
	}

#if OFFLINE_SE_IMAGE_IS_YUV
	rt_sprintf(in_file_name, "%s%s", SRC_FILE_PATH, SE1_IMAGE_NAME);
	img_w = 640;
	img_h = 480;

	size = img_w * img_h * 3 / 2;
	buf_data = rt_malloc_align(size, 32);
#else
	rt_sprintf(in_file_name, "%s%s", SRC_FILE_PATH, SE1_BMP_IMAGE_NAME);
	img_w = 800;
	img_h = 480;

	size = img_w * img_h * 4;
	buf_data = rt_malloc_align(size + 54, 32); /* 54 is bmp head */
#endif
	RT_ASSERT(buf_data != NULL);
	fd = open(in_file_name, O_RDONLY);
	if (fd <= 0) {
		LOG_E("open file[%s] err", in_file_name);
		return;
	}

	read_size = read(fd, buf_data, size);
	LOG_I("read size %d", read_size);
	if (read_size != size) {
		close(fd);
		LOG_E("read image data0 err");
		return;
	}

#if !OFFLINE_SE_IMAGE_IS_YUV
	buf_data = buf_data + 54; /* 54 is bmp head */
#endif
	if (fd > 0)
		close(fd);

	for (i = 0; i < 4; i++) {
		win_data.dma_addr = (u32)buf_data;
		win_data.chroma_dma_addr = (u32)(buf_data + img_w * img_h);
#if OFFLINE_SE_IMAGE_IS_YUV
		win_data.pixel_format = DISP_FORMAT_NV12;
#else
		win_data.pixel_format = DISP_FORMAT_ARGB8888;
#endif
		win_data.pixel_order = DC_PO_NORMAL;
		win_data.bpp = 32;

		win_data.crtc_x = 0;
		win_data.crtc_y = 0;
		win_data.crtc_width = img_w / 2;
		win_data.crtc_height = img_h / 2;

		win_data.fb_x = 0;
		win_data.fb_y = 0;
		win_data.fb_width = img_w;
		win_data.fb_height = img_h;

		win_data.src_width = img_w;
		win_data.src_height = img_h;

		win_data.update_flag = true;

		out_format = se_out_para[i].out_fmt;

		real_size = win_data.crtc_width * win_data.crtc_height;
		if (out_format == DISP_FORMAT_NV12 || out_format == DISP_FORMAT_YUV420)
			size_o =  real_size * 3 / 2;
		else if (out_format == DISP_FORMAT_YVU444)
			size_o =  real_size * 3;
		else if (out_format == DISP_FORMAT_ARGB8888)
			size_o =  real_size * 4;
		else if (out_format == DISP_FORMAT_RGB565)
			size_o =  real_size * 2;

		rt_sprintf(out_file_name, "%s%s-%dx%d", SRC_FILE_PATH,
				se_out_para[i].name, win_data.crtc_width,
				win_data.crtc_height);
		fd_o = open(out_file_name, O_RDWR | O_CREAT);
		if (fd_o <= 0) {
			LOG_E("open file err");
			return;
		}

		buf_out = rt_malloc_align(size_o, 32);
		RT_ASSERT(buf_out != NULL);

		out_addr[0] = (u32)buf_out;
		if (out_format == DISP_FORMAT_NV12)
			out_addr[1] = (u32)(buf_out + real_size);
		else if (out_format == DISP_FORMAT_YUV420) {
			out_addr[1] = (u32)(buf_out + real_size);
			out_addr[2] = (u32)(buf_out + real_size + real_size / 4);
		} else if (out_format == DISP_FORMAT_YVU444) {
			out_addr[1] = (u32)(buf_out + real_size);
			out_addr[2] = (u32)(buf_out + real_size);
		}

		LOG_D("in_addr0[%p] in_addr1[%p] out_addr[%p] out_format[%lx]",
			win_data.dma_addr, win_data.chroma_dma_addr,
			buf_out, out_format);

		disp_ops_p->disp_se_process(dse, &win_data, out_addr, out_format);

		LOG_D("real out_addr[%p]", buf_out);
		write_size = write(fd_o, buf_out, size_o);
		LOG_I("write size %d", write_size);
		if (write_size != size_o) {
			close(fd_o);
			LOG_E("write image data err");
			return;
		}

		if (fd_o > 0)
			close(fd_o);

		rt_free_align(buf_out);
	}
	rt_free_align(buf_data);

	disp_ops_p->disp_se_release(&dse);

	LOG_I("thread_disp_offline_se_test_entry end");
}
#endif

void disp_se0_test(rt_device_disp_ops_t *disp_ops_p)
{
	rt_thread_t tid_se0_test;
	rt_thread_t tid_read_sd;

	se_buf_mq_init();
	se_buf_mutex_init();

	tid_se0_test = rt_thread_create("disp_se0",
			thread_disp_se0_test_entry, disp_ops_p,
			DISP_TEST_THREAD_STACK_SIZE, DISP_TEST_THREAD_PRIORITY, 10);
	if (tid_se0_test != RT_NULL)
		rt_thread_startup(tid_se0_test);

	tid_read_sd = rt_thread_create("read_sd", thread_read_sd_entry, NULL,
			DISP_TEST_THREAD_STACK_SIZE, DISP_TEST_THREAD_PRIORITY, 10);
	if (tid_read_sd != RT_NULL)
		rt_thread_startup(tid_read_sd);
}

void disp_se1_test(rt_device_disp_ops_t *disp_ops_p)
{
	rt_thread_t tid_se1_test;

#if OFFLINE_SE_TEST
	tid_se1_test = rt_thread_create("disp_se1",
			thread_disp_offline_se_test_entry, disp_ops_p,
			DISP_TEST_THREAD_STACK_SIZE, DISP_TEST_THREAD_PRIORITY, 10);
#else
	tid_se1_test = rt_thread_create("disp_se1",
			thread_disp_se1_test_entry, disp_ops_p,
			DISP_TEST_THREAD_STACK_SIZE, DISP_TEST_THREAD_PRIORITY, 10);
#endif
	if (tid_se1_test != RT_NULL)
		rt_thread_startup(tid_se1_test);
}

