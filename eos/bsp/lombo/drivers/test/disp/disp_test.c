/*
 * disp_test.c - Disp test module driver code for LomboTech
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

/* bmp head struct */
typedef struct tag_bitmap_file_header {
	u16 bf_type;
	u32 bf_size;
	u16 bf_reserved1;
	u16 bf_reserved2;
	u32 bf_offbits;
	u32 bi_size;
	u32 bi_width;
	u32 bi_heigh;
	u16 bi_planes;
	u16 bi_bitcount;
	u32 bi_compression;
	u32 bi_size_image;
	u32 bi_xppm;
	u32 bi_yppm;
	u32 bi_clr_used;
	u32 bi_clr_important;
} bitmap_file_header_t;

rt_device_t disp_device;
disp_ctl_t *wctl[DC_WIN_NUM + 1];
bool disp_test_start_flag;

static char test_win_name[DC_WIN_NUM][WIN_NAME_MAX];

#if !PAGE_FLIP_TEST
#ifndef RT_USING_EGUI
static void thread_disp_hwc_test_entry(void *parameter)
{
	int ret;
	u32 cnt = 0;
	u32 update_cnt = 0;
	dc_win_data_t win_data;
	rt_device_disp_ops_t *disp_ops_p;

	LOG_I("thread_disp_hwc_test_entry start");
	ret = rt_device_control(disp_device, DISP_CMD_ENABLE_DC_HWC, NULL);
	if (ret != DISP_OK) {
		LOG_E("enable hwc err");
		return;
	}

	disp_ops_p = (rt_device_disp_ops_t *)parameter;
	RT_ASSERT(disp_ops_p != NULL);

	rt_memset(&win_data, 0, sizeof(dc_win_data_t));

	win_data.crtc_x = 0;
	win_data.crtc_y = 0;

	win_data.update_flag = true;
	while (disp_test_start_flag) {
		disp_ops_p->disp_hwc_update(win_data.crtc_x, win_data.crtc_y);
		cnt++;
		update_cnt = cnt % (DEFAULT_SCREEN_WIDTH / 10);
		win_data.crtc_x = DEFAULT_SCREEN_WIDTH - update_cnt*10;
		win_data.crtc_y = update_cnt*5;
		disp_delay(10);
	};

	ret = rt_device_control(disp_device, DISP_CMD_DISABLE_DC_HWC, NULL);
	if (ret != DISP_OK) {
		LOG_E("disable hwc err");
		return;
	}

	LOG_I("thread_disp_hwc_test_entry end");
}

static void disp_hwc_test(rt_device_disp_ops_t *disp_ops_p)
{
	rt_thread_t tid_hwc_test;

	tid_hwc_test = rt_thread_create("disp_hwc",
			thread_disp_hwc_test_entry, disp_ops_p,
			DISP_TEST_THREAD_STACK_SIZE, DISP_TEST_THREAD_PRIORITY, 10);
	if (tid_hwc_test != RT_NULL)
		rt_thread_startup(tid_hwc_test);
}
#endif
#endif

static void thread_disp_bkl_test_entry(void *parameter)
{
	disp_ctl_t *bkl[DC_BKL_NUM];
	char bkl_name[DC_BKL_NUM][WIN_NAME_MAX];
	u32 i, j, k, size, color[BKL_COLOR_TEST_NUM];
	char *buf_data[BKL_COLOR_TEST_NUM];
	u32 *test_data;
	u32 cnt = 0, ms_cnt = 0, update_cnt = 0, win_idx = 0;
	u16 crtc_w, crtc_h, crtc_off;
	dc_win_data_t win_data[BKL_COLOR_TEST_NUM];
	rt_device_disp_ops_t *disp_ops_p;

	LOG_I("thread_disp_bkl_test_entry start");
	crtc_w = 250;
	crtc_h = 30;
	crtc_off = 50;
	disp_ops_p = (rt_device_disp_ops_t *)parameter;
	RT_ASSERT(disp_ops_p != NULL);

	rt_memset(win_data, 0, 4 * sizeof(dc_win_data_t));

	size = 4 * crtc_w * crtc_h;

	color[DC_BL_IDX_0] = 0xffff0000; /* red */
	color[DC_BL_IDX_1] = 0xff00ff00; /* green */
	color[DC_BL_IDX_2] = 0xff0000ff; /* blue */

	color[DC_BL_IDX_3] = 0xffffff00; /* yello */
	color[DC_BL_IDX_4] = 0xff00ffff; /* ching */
	color[DC_BL_IDX_5] = 0xffff00ff; /* purplish red */

	for (k = DC_BL_IDX_0; k < BKL_COLOR_TEST_NUM; k++) {
		buf_data[k] = rt_malloc(size);
		RT_ASSERT(buf_data[k] != NULL);
		test_data = (u32 *)buf_data[k];
		for (j = 0; j < crtc_h; j++) {
			for (i = 0; i < crtc_w; i++)
				test_data[i + j * crtc_w] = color[k];
		}
		win_data[k].dma_addr = (u32)buf_data[k];
	}

	for (win_idx = DC_BL_IDX_0; win_idx < BKL_COLOR_TEST_NUM; win_idx++) {
		win_data[win_idx].pixel_format = DISP_FORMAT_ARGB8888;
		win_data[win_idx].pixel_order = DC_PO_NORMAL;
		win_data[win_idx].bpp = 32;

		win_data[win_idx].fb_x = 0;
		win_data[win_idx].fb_y = 0;
		win_data[win_idx].fb_width = crtc_w;
		win_data[win_idx].fb_height = crtc_h;

		win_data[win_idx].src_width = crtc_w;
		win_data[win_idx].src_height = crtc_h;

		win_data[win_idx].crtc_x = 0;
		win_data[win_idx].crtc_y = crtc_off * win_idx;
		#ifdef ARCH_LOMBO_N7V1_CDR
		win_data[win_idx].crtc_y += CDR_START_Y;
		#endif
		win_data[win_idx].crtc_width = crtc_w;
		win_data[win_idx].crtc_height = crtc_h;

		win_data[win_idx].update_flag = true;
	}

	win_data[DC_BL_IDX_4].crtc_x = win_data[DC_BL_IDX_3].crtc_x + 160;
	win_data[DC_BL_IDX_4].crtc_y = win_data[DC_BL_IDX_3].crtc_y + 10;

	win_data[DC_BL_IDX_5].crtc_x = win_data[DC_BL_IDX_3].crtc_x + 80;
	win_data[DC_BL_IDX_5].crtc_y = win_data[DC_BL_IDX_3].crtc_y + 20;

	for (i = 0; i < DC_BKL_NUM; i++) {
		rt_sprintf(bkl_name[i], "bkl_name%d", i);
		bkl[i] = disp_ops_p->disp_bkl_item_request(bkl_name[i]);
		if (NULL == bkl[i])
			LOG_E("fail to request bkl[%]", bkl_name[i]);
	}

	disp_ops_p->disp_bkl_item_config(bkl[4], &win_data[4]);

	for (i = 0; i < BKL_COLOR_TEST_NUM; i++)
		disp_ops_p->disp_bkl_list_add_tail(bkl[i]);

	for (i = 0; i < BKL_COLOR_TEST_NUM; i++)
		if (i != 4)
			disp_ops_p->disp_bkl_item_config(bkl[i], &win_data[i]);

	disp_bkl_printf_list_item_name();

	while (disp_test_start_flag) {
		disp_ops_p->disp_bkl_update();
		cnt++;
		update_cnt = cnt % (DEFAULT_SCREEN_WIDTH / 10);
		for (win_idx = DC_BL_IDX_0; win_idx < BKL_COLOR_TEST_NUM; win_idx++)
			win_data[win_idx].crtc_x = update_cnt * 10;

		for (i = 0; i < 3; i++)
			disp_ops_p->disp_bkl_item_config(bkl[i], &win_data[i]);

		disp_delay(10);
		if (cnt % 100 == 0) {
			ms_cnt++;
			/* rt_kprintf("ms_cnt[%d] ", ms_cnt); */
		}

		if (ms_cnt == 3) {
			disp_ops_p->disp_bkl_list_rm(bkl[DC_BL_IDX_3]);
			ms_cnt++;
		} else if (ms_cnt == 6) {
			disp_ops_p->disp_bkl_list_rm(bkl[DC_BL_IDX_4]);
			ms_cnt++;
		} else if (ms_cnt == 9) {
			disp_ops_p->disp_bkl_list_add_tail(bkl[DC_BL_IDX_4]);
#if 0
			disp_ops_p->disp_bkl_list_insert_after(bkl[DC_BL_IDX_4],
								bkl[DC_BL_IDX_5]);
#endif
			ms_cnt++;
		} else if (ms_cnt == 12) {
			/* disp_bkl_list_add_head(bkl[DC_BL_IDX_3]); */
			disp_ops_p->disp_bkl_list_insert_before(bkl[DC_BL_IDX_3],
								bkl[DC_BL_IDX_5]);
			ms_cnt++;
		}
	};

	for (i = 0; i < DC_BKL_NUM; i++) {
		if (bkl[i] != NULL)
			disp_ops_p->disp_bkl_item_release(&bkl[i]);
	}

	for (k = DC_BL_IDX_0; k < BKL_COLOR_TEST_NUM; k++)
		rt_free(buf_data[k]);

	LOG_I("thread_disp_bkl_test_entry end");
}

static void disp_bkl_test(rt_device_disp_ops_t *disp_ops_p)
{
	rt_thread_t tid_bl_test;

	tid_bl_test = rt_thread_create("disp_bl",
			thread_disp_bkl_test_entry, disp_ops_p,
			DISP_TEST_THREAD_STACK_SIZE, DISP_TEST_THREAD_PRIORITY, 10);
	if (tid_bl_test != RT_NULL)
		rt_thread_startup(tid_bl_test);
}

static int bmp_open_and_get_head(const char *name, bitmap_file_header_t *bitmap_head_p,
					u32 *data_size)
{
	int fd;
	u32 size, align_32 = 4, try_cnt = 3;
	char buf[64];

	RT_ASSERT(bitmap_head_p != NULL);

	fd = open(name, O_RDWR);
	while ((fd <= 0) && (try_cnt > 0)) {
		/* LOG_I("open %s again" , name); */
		if (try_cnt)
			fd = open(name, O_RDWR);
		try_cnt--;
		disp_delay(500);
	}

	if (fd > 0) {
		size = sizeof(bitmap_file_header_t);
		/* LOG_D("head size %d", size); */
		int read_size = read(fd, (void *)buf, size);
		/* LOG_D("read size %d", read_size); */
		if (read_size != size) {
			close(fd);
			LOG_E("read head err");
			return -RT_ERROR;
		}

		/* must consider 32 align problem*/
		rt_memcpy(&bitmap_head_p->bf_type, buf, sizeof(u16));
		rt_memcpy(&bitmap_head_p->bf_size, buf + sizeof(u16),
				read_size - align_32);

#if 0
		rt_kprintf("bf_type[%d]\n", bitmap_head_p->bf_type);
		rt_kprintf("bf_size[%d]\n", bitmap_head_p->bf_size);
		rt_kprintf("bf_offbits[%d]\n", bitmap_head_p->bf_offbits);
		rt_kprintf("bi_size[%d]\n", bitmap_head_p->bi_size);
		rt_kprintf("bi_width[%d]\n", bitmap_head_p->bi_width);
		rt_kprintf("bi_heigh[%d]\n", bitmap_head_p->bi_heigh);
		rt_kprintf("bi_planes[%d]\n", bitmap_head_p->bi_planes);
		rt_kprintf("bi_bitcount[%d]\n", bitmap_head_p->bi_bitcount);
#endif
		*data_size = bitmap_head_p->bi_width * bitmap_head_p->bi_heigh *
				(bitmap_head_p->bi_bitcount / 8);
		/* rt_kprintf("data_size[%d]\n", *data_size); */
		if (bitmap_head_p->bf_size < bitmap_head_p->bf_offbits + (*data_size)) {
			LOG_E("file length err");
			return -RT_ERROR;
		}
		return fd;
	}

	LOG_E("open %s err" , name);
	return -RT_ERROR;
}

#if !DOUBLE_SE_TEST
static void thread_win3_test_entry(void *parameter)
{
	int fd;
	u32 img_w, img_h, read_size, size;
	u32 cnt = 0, update_cnt = 0;
	bitmap_file_header_t bitmap_head;
	char *buf_addr;
	char *buf_data;
	dc_win_data_t win_data;
	rt_device_disp_ops_t *disp_ops_p;
	char file_name[64];

	LOG_I("thread_win3_test_entry start");
	disp_ops_p = (rt_device_disp_ops_t *)parameter;
	RT_ASSERT(disp_ops_p != NULL);

	rt_sprintf(file_name, "%s%s", SRC_FILE_PATH, SUBTITLE_NAME);
	fd = bmp_open_and_get_head(file_name, &bitmap_head, &size);
	if (fd <= 0) {
		LOG_E("open file[%s] err", file_name);
		return;
	}

	RT_ASSERT(size > 0);

	img_w = bitmap_head.bi_width;
	img_h = bitmap_head.bi_heigh;

	buf_data = rt_malloc(size);
	RT_ASSERT(buf_data != NULL);

	size = size + bitmap_head.bf_offbits;

	buf_addr = rt_malloc(size);
	RT_ASSERT(buf_addr != NULL);

	/* int bf_offbits = lseek(fd, bitmap_head.bf_offbits, SEEK_SET); */
	int bf_offbits = lseek(fd, 0, SEEK_SET);
	if (bf_offbits < 0) {
		LOG_E("lseek err");
		return;
	}

	LOG_D("bf_offbits %d size %d", bitmap_head.bf_offbits, size);
	read_size = read(fd, buf_addr, size);
	LOG_D("read size %d", read_size);
	if (read_size != size) {
		close(fd);
		LOG_E("read image data err");
		return;
	}

	if (fd > 0)
		close(fd);

	/* buf_data = buf_addr + bitmap_head.bf_offbits; */

	rt_memcpy(buf_data, buf_addr + bitmap_head.bf_offbits,
			size - bitmap_head.bf_offbits);

	rt_memset(&win_data, 0, sizeof(dc_win_data_t));

	win_data.dma_addr = (u32)buf_data;
	win_data.pixel_format = DISP_FORMAT_ARGB8888;
	win_data.pixel_order = DC_PO_NORMAL;
	win_data.bpp = 32;

	win_data.crtc_x = 0;
	win_data.crtc_y = 200;
	win_data.crtc_width = img_w;
	win_data.crtc_height = img_h;

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
		if (cnt % 100 == 0)
			rt_kprintf("dc cnt[%d]\n", cnt);
#endif
		update_cnt = cnt % 22;
		win_data.crtc_x = update_cnt * 10 + 50;
	};

	rt_free(buf_addr);
	rt_free(buf_data);

	LOG_I("thread_win3_test_entry end");
}

static void disp_win3_test(rt_device_disp_ops_t *disp_ops_p)
{
	rt_thread_t tid_win3_test;

	tid_win3_test = rt_thread_create("win3_test",
			thread_win3_test_entry, disp_ops_p,
			DISP_TEST_THREAD_STACK_SIZE, DISP_TEST_THREAD_PRIORITY, 10);

	if (tid_win3_test != RT_NULL)
		rt_thread_startup(tid_win3_test);
}
#endif

static void thread_win1_test_entry(void *parameter)
{
	int fd;
	u32 img_w, img_h, read_size, size;
	u32 cnt = 0, update_cnt = 0;
	bitmap_file_header_t bitmap_head;
	char *buf_addr;
	char *buf_data;
	dc_win_data_t win_data;
	rt_device_disp_ops_t *disp_ops_p;
	char file_name[64];

	LOG_I("thread_win1_test_entry start");
	disp_ops_p = (rt_device_disp_ops_t *)parameter;
	RT_ASSERT(disp_ops_p != NULL);

	rt_sprintf(file_name, "%s%s", SRC_FILE_PATH, SYS_BAR_NAME);
	fd = bmp_open_and_get_head(file_name, &bitmap_head, &size);
	if (fd <= 0) {
		LOG_E("open file[%s] err", file_name);
		return;
	}

	RT_ASSERT(size > 0);

	img_w = bitmap_head.bi_width;
	img_h = bitmap_head.bi_heigh;

	buf_data = rt_malloc(size);
	RT_ASSERT(buf_data != NULL);

	size = size + bitmap_head.bf_offbits;

	buf_addr = rt_malloc(size);
	RT_ASSERT(buf_addr != NULL);

	/* int bf_offbits = lseek(fd, bitmap_head.bf_offbits, SEEK_SET); */
	int bf_offbits = lseek(fd, 0, SEEK_SET);
	if (bf_offbits < 0) {
		LOG_E("lseek err");
		return;
	}

	LOG_D("bf_offbits %d size %d", bitmap_head.bf_offbits, size);
	read_size = read(fd, buf_addr, size);
	LOG_D("read size %d", read_size);
	if (read_size != size) {
		close(fd);
		LOG_E("read image data err");
		return;
	}

	if (fd > 0)
		close(fd);

	/* buf_data = buf_addr + bitmap_head.bf_offbits; */

	rt_memcpy(buf_data, buf_addr + bitmap_head.bf_offbits,
			size - bitmap_head.bf_offbits);

	rt_memset(&win_data, 0, sizeof(dc_win_data_t));

	win_data.dma_addr = (u32)buf_data;
	win_data.pixel_format = DISP_FORMAT_ARGB8888;
	win_data.pixel_order = DC_PO_NORMAL;
	win_data.bpp = 32;

	win_data.crtc_x = 0;
	win_data.crtc_y = DEFAULT_SCREEN_HEIGHT - img_h;
	win_data.crtc_width = img_w;
	win_data.crtc_height = img_h;

	win_data.fb_x = 0;
	win_data.fb_y = 0;
	win_data.fb_width = img_w;
	win_data.fb_height = img_h;

	win_data.src_width = img_w;
	win_data.src_height = img_h;

	win_data.update_flag = true;

	while (disp_test_start_flag) {
#if POWER_TEST
		if (cnt < 80)
#endif
			disp_ops_p->disp_win_update(wctl[1], &win_data);

		disp_delay(100);
		cnt++;
#if 0
		if (cnt % 100 == 0)
			rt_kprintf("dc cnt[%d]\n", cnt);
#endif
#if POWER_TEST
		if (cnt < 80) {
			update_cnt = cnt % (DEFAULT_SCREEN_WIDTH / 10);
			win_data.crtc_x = update_cnt * 10;
		}
#else
		update_cnt = cnt % (DEFAULT_SCREEN_WIDTH / 10);
		win_data.crtc_x = update_cnt * 10;
#endif
	};

	rt_free(buf_addr);
	rt_free(buf_data);

	LOG_I("thread_win1_test_entry end");
}

static void disp_win1_test(rt_device_disp_ops_t *disp_ops_p)
{
	rt_thread_t tid_win1_test;

	tid_win1_test = rt_thread_create("win1_test",
			thread_win1_test_entry, disp_ops_p,
			DISP_TEST_THREAD_STACK_SIZE, DISP_TEST_THREAD_PRIORITY, 10);
	if (tid_win1_test != RT_NULL)
		rt_thread_startup(tid_win1_test);
}

#if WIN0_TEST_ENABLE
static void thread_win0_test_entry(void *parameter)
{
	char *buf_data;
	u32 *test_data;
	u32 img_w, img_h, i, j, size;
	u32 cnt = 0, update_cnt = 0;
#if DYNAMIC_SE
	u32 dy_cnt = 0;
#endif
	dc_win_data_t win_data;
	rt_device_disp_ops_t *disp_ops_p;

	LOG_I("thread_win0_test_entry start");
	disp_ops_p = (rt_device_disp_ops_t *)parameter;
	RT_ASSERT(disp_ops_p != NULL);

	img_w = DEFAULT_SCREEN_WIDTH;
	img_h = DEFAULT_SCREEN_HEIGHT;

	size = 4 * img_w * img_h;
	buf_data = rt_malloc_align(size, 16);
	RT_ASSERT(buf_data != NULL);
	test_data = (u32 *)buf_data;
	for (j = 0; j < DEFAULT_SCREEN_HEIGHT / 3; j++) {
		for (i = 0; i < DEFAULT_SCREEN_WIDTH; i++)
			test_data[i + j * DEFAULT_SCREEN_WIDTH] = 0xffff8888; /* r */
	}
	for (j = DEFAULT_SCREEN_HEIGHT / 3; j < DEFAULT_SCREEN_HEIGHT * 2 / 3; j++) {
		for (i = 0; i < DEFAULT_SCREEN_WIDTH; i++)
			test_data[i + j * DEFAULT_SCREEN_WIDTH] = 0xff88ff88; /* g */
	}
	for (j = DEFAULT_SCREEN_HEIGHT * 2 / 3; j < DEFAULT_SCREEN_HEIGHT; j++) {
		for (i = 0; i < DEFAULT_SCREEN_WIDTH; i++)
			test_data[i + j * DEFAULT_SCREEN_WIDTH] = 0xff8888ff; /* b */
	}

	rt_memset(&win_data, 0, sizeof(dc_win_data_t));

	win_data.dma_addr = (u32)buf_data;
	win_data.pixel_format = DISP_FORMAT_ARGB8888;
	win_data.pixel_order = DC_PO_NORMAL;
	win_data.bpp = 32;

	win_data.crtc_x = 0;
	win_data.crtc_y = 0;
	#ifdef ARCH_LOMBO_N7V1_CDR
	win_data.crtc_y += CDR_START_Y;
	#endif
	win_data.crtc_width = img_w;
	win_data.crtc_height = img_h;

	win_data.fb_x = 0;
	win_data.fb_y = 0;
	win_data.fb_width = img_w;
	win_data.fb_height = img_h;

	win_data.src_width = img_w;
	win_data.src_height = img_h;

	win_data.update_flag = true;

	while (disp_test_start_flag) {
		cnt++;
		disp_ops_p->disp_win_update(wctl[0], &win_data);

		update_cnt = cnt % 160;
		win_data.crtc_x = update_cnt * 5;

		disp_delay(10);
#if DYNAMIC_SE
		if (cnt % 300 == 0) {
			dy_cnt++;
			if (dy_cnt % 2 == 0) {
				LOG_I("full");
				win_data.crtc_width = img_w;
				win_data.crtc_height = img_h;
			} else {
				LOG_I("half");
				win_data.crtc_width = img_w / 2;
				win_data.crtc_height = img_h / 2;
			}
		}
#endif
	};
	rt_free_align(buf_data);
	LOG_I("thread_win0_test_entry end");
}

static void disp_win0_test(rt_device_disp_ops_t *disp_ops_p)
{
	rt_thread_t tid_win0_test;

	tid_win0_test = rt_thread_create("win0_test", thread_win0_test_entry,
					disp_ops_p, DISP_TEST_THREAD_STACK_SIZE,
					DISP_TEST_THREAD_PRIORITY, 10);
	if (tid_win0_test != RT_NULL)
		rt_thread_startup(tid_win0_test);
}
#endif

#if SCREENSHOT_TEST
/* screenshot only for debug */
static void screenshot_test(rt_device_disp_ops_t *disp_ops_p)
{
	u32 write_size, size;
	int ret, fd;
	bitmap_file_header_t bitmap_head;
	char *buf_data;
	char file_name[64];
	disp_io_ctrl_t dic;
	struct rt_device_graphic_info info;
	u32 hour = 0, minute = 0, second = 0;
#if SCREEN_ROT_TEST
	u32 img_w, img_h;
	char *rot_data;
	disp_rot_cfg_t cfgs;
#endif

	LOG_I("screenshot_test start");
	RT_ASSERT(disp_ops_p != NULL);

#if SCREEN_ROT_TEST
	ret = rt_device_control(disp_device, DISP_CMD_ENABLE_HW_ROT, NULL);
	if (ret != DISP_OK) {
		LOG_E("enable hw rot err");
		return;
	}
#endif

	rt_memset(&dic, 0x00, sizeof(disp_io_ctrl_t));
	dic.dc_index = 0;
	dic.args = &info;
	rt_device_control(disp_device, DISP_CMD_GET_INFO, &dic);
	LOG_I("width[%d] height[%d]", info.width, info.height);

	bitmap_head.bf_type = 0x4d42; /* BM */
	bitmap_head.bf_reserved1 = 0;
	bitmap_head.bf_reserved2 = 0;
	bitmap_head.bf_offbits = 54;/* 0x00000036 */
	bitmap_head.bi_size = 40; /* 0x00000028 */
	bitmap_head.bi_width = info.width;
	bitmap_head.bi_heigh = info.height;
	bitmap_head.bi_planes = 1; /* 0x0001 */
	bitmap_head.bi_bitcount = 32; /* 0x0020 */
	bitmap_head.bi_compression = 0; /* 0x00000000 */
	bitmap_head.bi_xppm = 0x00000b12;
	bitmap_head.bi_yppm = 0x00000b12;
	bitmap_head.bi_clr_used = 0;
	bitmap_head.bi_clr_important = 0;

	second = rt_time_get_msec() / 1000;
	minute = second / 60;
	hour = minute / 60;
	rt_sprintf(file_name, "%s-%d-%d-%d%s", SCREENSHOT_FILE_NAME,
			hour, minute % 64, second % 60, ".bmp");
	fd = open(file_name, O_RDWR | O_CREAT);
	if (fd <= 0) {
		LOG_E("open file[%s] err", file_name);
		return;
	}

	size = info.width * info.height * 4; /* argb is 4 */
	RT_ASSERT(size > 0);
	bitmap_head.bf_size = size + bitmap_head.bf_offbits;
	bitmap_head.bi_size_image = size;

#if 0
	rt_kprintf("bf_type[%d]\n", bitmap_head.bf_type);
	rt_kprintf("bf_size[%d]\n", bitmap_head.bf_size);
	rt_kprintf("bf_offbits[%d]\n", bitmap_head.bf_offbits);
	rt_kprintf("bi_size[%d]\n", bitmap_head.bi_size);
	rt_kprintf("bi_width[%d]\n", bitmap_head.bi_width);
	rt_kprintf("bi_heigh[%d]\n", bitmap_head.bi_heigh);
	rt_kprintf("bi_planes[%d]\n", bitmap_head.bi_planes);
	rt_kprintf("bi_bitcount[%d]\n", bitmap_head.bi_bitcount);
#endif

	write_size = write(fd, &bitmap_head.bf_type, 2); /* bf_type */
	/* other 52bytes of bitmap_head */
	write_size = write(fd, &bitmap_head.bf_size, 52);

	buf_data = rt_malloc_align(size, 32);
	RT_ASSERT(buf_data != NULL);

#if SCREEN_ROT_TEST
	img_w = bitmap_head.bi_width;
	img_h = bitmap_head.bi_heigh;

	rot_data = rt_malloc_align(size, 32);
	RT_ASSERT(rot_data != NULL);
#endif

	disp_screenshot(buf_data, size);

	int bf_offbits = lseek(fd, bitmap_head.bf_offbits, SEEK_SET);
#if 0
	int bf_offbits = lseek(fd, 0, SEEK_SET);
#endif

	if (bf_offbits < 0) {
		LOG_E("lseek err");
		return;
	}

	write_size = 0;

#if SCREEN_ROT_TEST
	LOG_I("size[%d] img_w[%d] img_h[%d]", size, img_w, img_h);
	LOG_I("name[%s]", file_name);
	rt_memset(&cfgs, 0x00, sizeof(disp_rot_cfg_t));

	cfgs.mode = LOMBO_DRM_TRANSFORM_FLIP_V_ROT_90;
	cfgs.rot_way = SW_ROT;

	cfgs.infb.format = DISP_FORMAT_ARGB8888;
	cfgs.infb.addr[0] = (unsigned int)buf_data;
	cfgs.infb.width[0] = img_w;
	cfgs.infb.height[0] = img_h;

	cfgs.outfb.format = DISP_FORMAT_ARGB8888;
	cfgs.outfb.addr[0] = (unsigned int)rot_data;
	if (cfgs.mode % 2 == 0) {
		cfgs.outfb.width[0] = img_w;
		cfgs.outfb.height[0] = img_h;
	} else {/* w and h is exchange */
		cfgs.outfb.width[0] = img_h;
		cfgs.outfb.height[0] = img_w;
	}

	LOG_I("screen rot_process start");
	disp_ops_p->disp_rot_process(&cfgs);
	LOG_I("screen rot_process end");
	write_size = write(fd, rot_data, size);
#else
	write_size = write(fd, buf_data, size);
#endif

	if (write_size != size)
		LOG_E("write err size[%d] write_size[%d]", size, write_size);

#if SCREEN_ROT_TEST
	if (cfgs.mode % 2 != 0) { /* w and h is exchange */
		lseek(fd, 18, SEEK_SET); /* width */
		write(fd, &img_h, 4);
		write(fd, &img_w, 4);
	}
#endif

	if (fd > 0)
		close(fd);

	rt_free_align(buf_data);

#if SCREEN_ROT_TEST
	rt_free_align(rot_data);
	ret = rt_device_control(disp_device, DISP_CMD_DISABLE_HW_ROT, NULL);
	if (ret != DISP_OK) {
		LOG_E("disable hw rot err");
		return;
	}
#endif

	LOG_I("screenshot_test end");
}
#endif

#if SWITCH_WIN_TEST
static void switch_win_test(rt_device_disp_ops_t *disp_ops_p)
{
	static u8 cnt;
	u8 update_cnt;

	LOG_I("switch_win_test");
	RT_ASSERT(disp_ops_p != NULL);

	update_cnt = 1 + cnt % 3;
#if 0
	disp_ops_p->disp_win_release(&wctl[update_cnt]);
	disp_win_printf_list_item_name();

	disp_delay(2000);

	wctl[update_cnt] = disp_ops_p->disp_win_request(test_win_name[update_cnt]);

	disp_win_printf_list_item_name();
#endif

	disp_ops_p->disp_set_win_layer(wctl[update_cnt], WIN_LAYER_BOTTOM);
	disp_win_printf_list_item_name();
	cnt++;
}
#endif

#if PAGE_FLIP_TEST
static void disp_hwc_location_test(void *parameter)
{
	static u32 cnt;
	u32 update_cnt = 0;
	dc_win_data_t win_data;

	rt_device_disp_ops_t *disp_ops_p = (rt_device_disp_ops_t *)parameter;
	RT_ASSERT(disp_ops_p != NULL);

	rt_memset(&win_data, 0, sizeof(dc_win_data_t));

	win_data.crtc_x = 0;
	win_data.crtc_y = 0;

	win_data.update_flag = true;
	cnt++;
	update_cnt = cnt % 70;
	win_data.crtc_x = DEFAULT_SCREEN_WIDTH - update_cnt * 10;
	win_data.crtc_y = update_cnt * 5;
	disp_ops_p->disp_hwc_update(win_data.crtc_x, win_data.crtc_y);
}

static void page_flip_test(rt_device_disp_ops_t *disp_ops_p)
{
	static bool show_enable = true;
	disp_io_ctrl_t dic;

	LOG_I("page_flip_test: sta[%d]", show_enable);
	RT_ASSERT(disp_ops_p != NULL);
	rt_memset(&dic, 0x00, sizeof(disp_io_ctrl_t));
	dic.dc_index = 0;

	disp_ops_p->disp_dc_page_flip_start();
	if (show_enable) {
		rt_device_control(disp_device, DISP_CMD_ENABLE_DC_BLOCKLINKER, &dic);

		dic.dctl = wctl[1];
		rt_device_control(disp_device, DISP_CMD_ENABLE_DC_WIN, &dic);
		dic.dctl = wctl[2];
		rt_device_control(disp_device, DISP_CMD_ENABLE_DC_WIN, &dic);
	} else {
		rt_device_control(disp_device, DISP_CMD_DISABLE_DC_BLOCKLINKER, &dic);

		dic.dctl = wctl[1];
		rt_device_control(disp_device, DISP_CMD_DISABLE_DC_WIN, &dic);
		dic.dctl = wctl[2];
		rt_device_control(disp_device, DISP_CMD_DISABLE_DC_WIN, &dic);
	}
	disp_hwc_location_test(disp_ops_p);
	disp_ops_p->disp_dc_page_flip_stop();

	show_enable = !show_enable;
}
#endif

static int disp_config(rt_device_disp_ops_t **disp_ops_pp)
{
	rt_device_disp_ops_t *disp_ops = NULL;

	LOG_I("disp_config");
	disp_device = rt_device_find(DISP_DEVICE_NAME);
	if (disp_device != NULL) {
		rt_device_open(disp_device, 0);
		disp_ops = (rt_device_disp_ops_t *)(disp_device->user_data);
		RT_ASSERT(disp_ops != NULL);
		*disp_ops_pp = disp_ops;
	} else {
		LOG_E("disp_config err");
		*disp_ops_pp = NULL;
	}

	return 0;
}

static int disp_unconfig(void)
{
	LOG_I("disp_unconfig");

	if (disp_device != NULL)
		rt_device_close(disp_device);
	else
		LOG_E("disp_unconfig err");

	return 0;
}

static void win_alpha_init(disp_ctl_t *win_ctl)
{
	static u8 value = 255;
	dc_alpha_mode_t mode = DC_PLANE_ALPHA;
	disp_io_ctrl_t dic;

	/* LOG_I("win_alpha_init: mode[%d] value[%d]", mode, value); */
	rt_memset(&dic, 0x00, sizeof(disp_io_ctrl_t));

	dic.dc_index = 0;
	dic.dctl = win_ctl;
	dic.args = &mode;
	rt_device_control(disp_device, DISP_CMD_SET_DC_WIN_ALPHA_MODE, &dic);

	dic.args = &value;
	rt_device_control(disp_device, DISP_CMD_SET_DC_WIN_ALPHA_VALUE, &dic);
}

static void thread_disp_test_entry(void *parameter)
{
	u8 i, start_cnt;
	u32 frame_rate = 0, cnt = 0;
#if SWITCH_WIN_TEST
	u8 update_cnt = 0;
#endif
	rt_device_disp_ops_t *disp_ops_p = NULL;
	struct rt_device_graphic_info info;
	disp_io_ctrl_t dic;

	LOG_I("thread_disp_test_entry start");
	if (disp_test_start_flag)
		return;

	disp_test_start_flag = true;

	rt_memset(&test_win_name, 0x00, sizeof(test_win_name));

	disp_config(&disp_ops_p);
	RT_ASSERT(disp_ops_p != NULL);

#if WIN0_TEST_ENABLE
	rt_strncpy(test_win_name[0], "gui", WIN_NAME_MAX);
#endif
	rt_strncpy(test_win_name[1], "sys_bar", WIN_NAME_MAX);
	rt_strncpy(test_win_name[2], "video", WIN_NAME_MAX);
	rt_strncpy(test_win_name[3], "subtitle", WIN_NAME_MAX);

#if WIN0_TEST_ENABLE
	start_cnt = 0;
#else
	start_cnt = 1;
#endif
	for (i = start_cnt; i < 4; i++) {
		wctl[i] = disp_ops_p->disp_win_request(test_win_name[i]);
		if (wctl[i] != NULL)
			win_alpha_init(wctl[i]);
		else
			LOG_E("fail to request win[%s]", test_win_name[i]);
	}
	/* disp_win_printf_list_item_name(); */

	disp_delay(3000);
#if WIN0_TEST_ENABLE
	disp_win0_test(disp_ops_p);
#endif
	disp_se0_test(disp_ops_p);

	disp_win1_test(disp_ops_p);

#if DOUBLE_SE_TEST
#if !DYNAMIC_SE
	disp_se1_test(disp_ops_p);
#endif
#else /* DOUBLE_SE_TEST */
	disp_win3_test(disp_ops_p);
#endif

#if !PAGE_FLIP_TEST
#ifndef RT_USING_EGUI
	disp_hwc_test(disp_ops_p);
#endif
#endif
	disp_bkl_test(disp_ops_p);
	while (1) {
		cnt++;
		disp_ioctl_test(cnt);

		if (cnt == 2) {
			rt_memset(&dic, 0x00, sizeof(disp_io_ctrl_t));
			dic.dc_index = 0;
			dic.args = &info;
			rt_device_control(disp_device, DISP_CMD_GET_INFO, &dic);
			LOG_I("width[%d] height[%d]", info.width, info.height);
			dic.args = &frame_rate;
			rt_device_control(disp_device, DISP_CMD_GET_FRAME_RATE, &dic);
			LOG_I("frame_rate is %d", frame_rate);
		}

#if POWER_TEST
		if (cnt == 3) {
			disp_suspend();
			/* disp_win_printf_list_item_name(); */
		}

		if (cnt == 10) {
			/* disp_win_printf_list_item_name(); */
			disp_resume();
		}
#endif

#if SWITCH_WIN_TEST
		if (cnt % 3 == 0) {
			update_cnt++;
			switch_win_test(disp_ops_p);
		}
#endif

#if 0 /* SCREENSHOT_TEST */
		if (cnt == 5) {
			screenshot_test(disp_ops_p);
		} else
#endif

#if DISP_EXIT_TEST
		if (cnt >= DISP_TEST_TIME) {
			disp_test_start_flag = false;
			disp_delay(1000);
			LOG_I("thread_disp_test_entry time reach %d seconds", cnt);
			break;
		}
#endif

#if PAGE_FLIP_TEST
		if (cnt % 2 == 0)
			page_flip_test(disp_ops_p);
#endif
		disp_delay(1000);
	};

	for (i = start_cnt; i < 4; i++) {
		if (wctl[i] != NULL)
			disp_ops_p->disp_win_release(&wctl[i]);
	}

	disp_unconfig();
	LOG_I("thread_disp_test_entry end");
}

void disp_demo_test(void)
{
	rt_thread_t tid_disp_test;

	tid_disp_test = rt_thread_create("disp_demo_test",
				thread_disp_test_entry, RT_NULL, 5120,
				DISP_TEST_THREAD_PRIORITY, 10);
	if (tid_disp_test != RT_NULL)
		rt_thread_startup(tid_disp_test);
}

void thread_disp_open_test(void *parameter)
{
	u32 test_cnt = 0;
	LOG_I("thread_disp_open_test");

	disp_device = rt_device_find(DISP_DEVICE_NAME);

	if (disp_device != NULL) {
		while (1) {
			test_cnt++;
			LOG_I("disp_open_test[%d]", test_cnt);
			rt_device_open(disp_device, 0);
			disp_delay(200);
		}
	}
}

void thread_disp_close_test(void *parameter)
{
	u32 test_cnt = 0;
	LOG_I("thread_disp_close_test");

	disp_device = rt_device_find(DISP_DEVICE_NAME);

	if (disp_device != NULL) {
		while (1) {
			test_cnt++;
			LOG_I("disp_close_test[%d]", test_cnt);
			rt_device_close(disp_device);
			disp_delay(100);
		}
	}
}

void disp_open_close_test(void)
{

	rt_thread_t tid_open_test, tid_close_test;

	tid_open_test = rt_thread_create("tid_open_test",
			thread_disp_open_test, NULL,
			DISP_TEST_THREAD_STACK_SIZE, DISP_TEST_THREAD_PRIORITY - 1, 10);

	if (tid_open_test != RT_NULL)
		rt_thread_startup(tid_open_test);

	tid_close_test = rt_thread_create("tid_close_test",
			thread_disp_close_test, NULL,
			DISP_TEST_THREAD_STACK_SIZE, DISP_TEST_THREAD_PRIORITY, 10);

	if (tid_close_test != RT_NULL)
		rt_thread_startup(tid_close_test);

}

long test_disp(int argc, char **argv)
{
	char *fun_name = NULL;
	char *ioctl_cmd = NULL;
	rt_device_disp_ops_t *disp_ops_p = NULL;
	u32 bk_color = 0xff000000;
	u32 bl_value;

	LOG_I("test_disp build time: %s %s\n", __DATE__, __TIME__);
#if 0
	LOG_I("argc[%d]", argc);
	for (u8 i = 0; i < argc; i++)
		LOG_I("argc[%d][%s]", i, argv[i]);
#endif

	if (argc >= 3)
		fun_name = argv[2];

	if (fun_name != NULL) {
		if (strcmp(fun_name, "oc") == DISP_OK) {
			disp_open_close_test();
			return 0;
		} else if (strcmp(fun_name, "screenshot") == DISP_OK) {
			#if SCREENSHOT_TEST
			disp_config(&disp_ops_p);
			screenshot_test(disp_ops_p);
			disp_unconfig();
			#endif
			return 0;
		} else if (strcmp(fun_name, "dit") == DISP_OK) {
			disp_config(&disp_ops_p);
			disp_dit_test(disp_ops_p);
			disp_unconfig();
			return 0;
		} else if (strcmp(fun_name, "rot") == DISP_OK) {
			disp_config(&disp_ops_p);
			disp_rot_test(disp_ops_p);
			disp_unconfig();
			return 0;
		} else if (strcmp(fun_name, "ioctl") == DISP_OK) {
			if (argc >= 4)
				ioctl_cmd = argv[3];
			disp_ioctl_cfg(ioctl_cmd);
			return 0;
		} else if (strcmp(fun_name, "backcolor") == DISP_OK) {
			disp_config(&disp_ops_p);
			if (argc < 6) {
				LOG_E("please input r g b value such as '255' '0' '0'");
				disp_unconfig();
				return 0;
			}

			bk_color = bk_color + (atoi(argv[3]) << 16) +
					(atoi(argv[4]) << 8) + atoi(argv[5]);
			LOG_I("backcolor value:0x%lx", bk_color);
			backcolor_set_test(bk_color);
			return 0;
		} else if (strcmp(fun_name, "backlight") == DISP_OK) {
			disp_config(&disp_ops_p);
			if (argc < 4) {
				LOG_E("please input backlight value");
				disp_unconfig();
				return 0;
			}
			bl_value = atoi(argv[3]);
			LOG_I("backlight value:%d", bl_value);
			backlight_pwm_set_test(bl_value);
			return 0;
		} else if (strcmp(fun_name, "info") == DISP_OK) {
			disp_debug_info();
			return 0;
		} else if (strcmp(fun_name, "demo") == DISP_OK) {
			disp_demo_test();
			return 0;
		} else {
			LOG_E("unspported cmd");
		}
	}

	LOG_I("disp cmd0: %s", "demo");
	LOG_I("disp cmd1: %s", "info");
	LOG_I("disp cmd2: %s", "ioctl");
	LOG_I("disp cmd3: %s", "rot");
	LOG_I("disp cmd4: %s", "dit");
	LOG_I("disp cmd5: %s", "screenshot");
	LOG_I("disp cmd6: %s", "backcolor");
	LOG_I("disp cmd7: %s", "backlight");

	return 0;
}

