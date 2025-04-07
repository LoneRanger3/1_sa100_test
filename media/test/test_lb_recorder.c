/*
 * lb_recorder.c - Standard functionality for recorder.
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

#define DBG_LEVEL		DBG_INFO

#include <oscl.h>
#include <base_component.h>
#include <omx_test.h>
#include "vrender_component.h"
#include "vrec_component.h"
#include "framework_common.h"
#include "lb_recorder.h"
#include <getopt.h>
#include "recorder_private.h"
#include "dirent.h"
void *disp;

#define MAX_ARGS_LEN (16)
#define TEST_ROOT_PATH "/mnt/sdcard/"
#define VIC_PATH "/mnt/sdcard/VIDEO_R/"
#define ISP_PATH "/mnt/sdcard/VIDEO_F/"
#define PATH0 "/mnt/sdcard/VIDEO_0/"
#define PATH1 "/mnt/sdcard/VIDEO_1/"
#define PATH2 "/mnt/sdcard/VIDEO_2/"
#define PATH3 "/mnt/sdcard/VIDEO_3/"

typedef struct auto_test_para {
	int index;
	int watermark;
	int stop_flag;
	int preview_flag;
	int record_flag;
	int delay;
	int rotate;
	win_para_t disp_para;
	rec_param_t rec_para;
	rec_time_lag_para_t time_lag_para;
	char *source;
	char *path;
	void *recorder;
	recorder_type_e record_type;
	pthread_t auto_thread;
	rt_timer_t timer;
	int line;
	int last_loop;
	int loop;
} auto_test_para_t;

typedef struct test_cmd {
	char str[64];
	char argc;
	char argv[FINSH_ARG_MAX][MAX_ARGS_LEN];
} test_cmd_t;

#define TEST_MAX_REC_NUM 4

static auto_test_para_t g_test_para[TEST_MAX_REC_NUM];
static int test_max_loop_num = 20000;
static int test_all_flag;

static void auto_test_all(void);
static void mk_dir(char *path)
{
	DIR *dir = NULL;

	dir = opendir(path);
	if (dir == NULL)
		mkdir(path, 0);
	else
		closedir(dir);
}

static int init_dvr_para(int total)
{
	int i;
	auto_test_para_t *test_para;

	for (i = 0; i < total; i++) {
		test_para = &g_test_para[i];
		test_para->source = "vic";

		if (i != 0) {
			test_para->preview_flag = 0;
			memcpy(&test_para->rec_para, &g_test_para[0].rec_para,
				sizeof(rec_param_t));
		}
		if (total == 2) {
			test_para->rec_para.source_height = 1440;
			test_para->rec_para.source_width = 1280;
			test_para->rec_para.height =
				test_para->rec_para.source_height / 2;
			test_para->rec_para.width = test_para->rec_para.source_width;
			test_para->rec_para.enc_rect.x = 0;
			test_para->rec_para.enc_rect.y = (i)*test_para->rec_para.height;
			test_para->rec_para.enc_rect.width = test_para->rec_para.width;
			test_para->rec_para.enc_rect.height = test_para->rec_para.height;
		} else if (total == 4) {
			test_para->rec_para.source_height = 1440;
			test_para->rec_para.source_width = 2560;
			test_para->rec_para.height =
				test_para->rec_para.source_height / 2;
			test_para->rec_para.width = test_para->rec_para.source_width;
			test_para->rec_para.enc_rect.x = (i)%2*test_para->rec_para.width;
			test_para->rec_para.enc_rect.y = (i)/2*test_para->rec_para.height;
			test_para->rec_para.enc_rect.width = test_para->rec_para.width;
			test_para->rec_para.enc_rect.height = test_para->rec_para.height;
		}
		test_para->rec_para.bitrate = 6000000;
	}
	return 0;
}

static void wait_test_finished(int index)
{
	if (g_test_para[index].auto_thread) {
		pthread_join(g_test_para[index].auto_thread, NULL);
		g_test_para[index].auto_thread = NULL;
	}
}

void init_test_para(void)
{
	mk_dir(ISP_PATH);
	mk_dir(VIC_PATH);
	mk_dir(PATH0);
	mk_dir(PATH1);
	mk_dir(PATH2);
	mk_dir(PATH3);

	memset((unsigned char *)g_test_para, 0x00, sizeof(g_test_para));
	g_test_para[0].source = "vic";
	g_test_para[0].index = 0;
	g_test_para[0].rotate = 1;
	g_test_para[0].path = PATH0;
	g_test_para[0].rec_para.bitrate = 6000000;
	g_test_para[0].rec_para.source_height = 720;
	g_test_para[0].rec_para.source_width = 1280;
	g_test_para[0].rec_para.height = 720;
	g_test_para[0].rec_para.width = 1280;

	g_test_para[1].source = "isp";
	g_test_para[1].index = 1;
	g_test_para[1].rotate = 1;
	g_test_para[1].path = PATH1;
	g_test_para[1].rec_para.bitrate = 14000000;
	g_test_para[1].rec_para.source_height = 1080;
	g_test_para[1].rec_para.source_width = 1920;
	g_test_para[1].rec_para.height = 1080;
	g_test_para[1].rec_para.width = 1920;

	g_test_para[2].index = 2;
	g_test_para[2].path = PATH2;
	g_test_para[3].index = 3;
	g_test_para[3].path = PATH3;

}

rt_uint32_t check_mem(const char *func, rt_uint32_t pre_mem, rt_int32_t count)
{
	rt_uint32_t total = 0;
	rt_uint32_t used = 0;
	rt_uint32_t max_used = 0;

	rt_memory_info(&total, &used, &max_used);
	if (pre_mem > 0) {
		if (pre_mem != used) {
			OSCL_LOGE(">>>>>>>>>>>>>>>[%s %d %d]test fail.<<<<<<<<<<<<<<<",
					func, count, (used - pre_mem));
		} else {
			OSCL_LOGI(">>>>>>>>>>>>>>>[%s %d %d]test success.<<<<<<<<<<<<<<<",
					func, count, (used - pre_mem));
		}
	}
	OSCL_LOGI("total memory: %d\n", total);
	OSCL_LOGI("used memory : %d\n", used);
	OSCL_LOGI("maximum allocated memory: %d\n", max_used);

	return used;
}
void *open_disp_engine(void)
{
	disp_hdl_t *vdisp = NULL;

	vdisp = oscl_zalloc(sizeof(disp_hdl_t));
	disp = vdisp;
	oscl_param_check((NULL != vdisp), NULL, NULL);
	vdisp->disp_device = rt_device_find("disp");
	if (vdisp->disp_device != NULL) {
		rt_device_open(vdisp->disp_device, 0);
		vdisp->disp_ops =
			(rt_device_disp_ops_t *)(vdisp->disp_device->user_data);
		RT_ASSERT(vdisp->disp_ops != NULL);
	} else {
		oscl_free(vdisp);
		OSCL_LOGE("disp_config err");
		return NULL;
	}
	return vdisp;
}


int recorder_show_components(int argc, char **argv)
{
	omx_show_active();
	return 0;
}
MSH_CMD_EXPORT(recorder_show_components, "recorder_show_components");


static void lbrecorder_help(void)
{
	const char *arg_format = "  %-20s %s%s\n";

	OSCL_PRINT("usage: lbrecorder [argument]\n");
	OSCL_PRINT("arguments:\n");
	OSCL_PRINT(arg_format, "--file filename", "set out filename", "");
	OSCL_PRINT(arg_format, "--preview", "start preview", "");
	OSCL_PRINT(arg_format, "--rec", "start recorder", "");
	OSCL_PRINT(arg_format, "--stop", "stop recorder", "");
	OSCL_PRINT(arg_format, "--help", "show this help info", "");
}

static int buf_handle(void *lbrecorder, al_frame_t *frame)
{
	OSCL_LOGE("fame:%x:%x, time:%d, size:%d,%d, pri:%x, %x", frame,
		  frame->info.video.addr[0], (int)frame->info.video.time_stamp,
		  frame->info.video.width, frame->info.video.height,
		  frame->al_data, frame->header);
	lb_recorder_ctrl(lbrecorder, LB_REC_FREE_FRAME, (void *)frame);
	OSCL_LOGE("free end");
	return 0;
}

static void _get_config_disp_para(win_para_t *disp_para, int index)
{
	disp_size_t size;
	disp_hdl_t *disp_dev;
	if (index > 2)
		return;

	memset(&size, 0, sizeof(disp_size_t));
	disp_dev = oscl_open_disp_engine();
	if (disp_dev)
		oscl_disp_get_screen_size(disp_dev, &size);
	oscl_close_disp_engine(disp_dev);
	disp_para->rect.x = 0;
	disp_para->rect.y = 0;
	if (size.width > size.height) {
		size.width /= 2;
		disp_para->rect.x += index * size.width;
	} else {
		size.height /= 2;
		disp_para->rect.y += index * size.height;
	}
	disp_para->mode = VDISP_WINDOW_FULL_SCREEN_VIDEO_RATIO;
	disp_para->rect.width = size.width;
	disp_para->rect.height = size.height;

}

static cali_out_data_t g_cali_out;
static int cali_flag;

static int pano_test(void *erecorder)
{
	cali_param_t cali_para;
	win_para_t win_para;
	pano_param_t init_para;
	cali_contex_t cali_ctx;
	vsize_t prev_size;
	vsize_t size;
	int ret = 0;

	if (NULL == erecorder)
		return -1;
	memset(&cali_para, 0, sizeof(cali_para));
	memset(&win_para, 0, sizeof(win_para));
	memset(&init_para, 0, sizeof(init_para));
	memset(&size, 0, sizeof(size));

	ret = lb_recorder_ctrl(erecorder, LB_REC_PANO_CREAT, NULL);
	if (0 == cali_flag) {
		/* 1. set calibration para */
		cali_para.box_rows = 5;
		cali_para.box_cols = 11;
		cali_para.box_width = 20;
		cali_para.box_height = 20;
		cali_para.dist_2_rear = 80;
		cali_para.car_width = 180;
		cali_para.car_length = 460;
		cali_para.front_dist = 100;
		cali_para.rear_dist = 500;
		cali_para.align = -1;
		cali_para.use_ext_cali_img = 1; /* use extern calibration image */
		cali_para.ext_cali_img.width = 1280;
		cali_para.ext_cali_img.height = 720;
		strncpy(cali_para.ext_cali_img.format, "nv12",
				sizeof(cali_para.ext_cali_img.format) - 1);
		strncpy(cali_para.ext_cali_img.path, TEST_ROOT_PATH"cali_yuv.bin",
				sizeof(cali_para.ext_cali_img.path) - 1);
		ret = lb_recorder_ctrl(erecorder, LB_REC_PANO_SET_CALI_PARA,
				(void *)&cali_para);
	}
	/* 2. set init pano para */
	init_para.in_gps = 0;
	init_para.in_obd = 0;
	init_para.car_para_en = 1;
	init_para.car_width = cali_para.car_width;
	init_para.data_format = NULL;
	init_para.carboard_img.width = 82;
	init_para.carboard_img.height = 209;

	init_para.use_ext_cutline = 0; /* use extern cutline value */
	init_para.culine = 300;

	strncpy(init_para.carboard_img.format, "nv12",
			sizeof(init_para.carboard_img.format) - 1);
	strncpy(init_para.carboard_img.path, TEST_ROOT_PATH"car_yuv.bin",
			sizeof(init_para.carboard_img.path) - 1);
	ret = lb_recorder_ctrl(erecorder, LB_REC_PANO_SET_INIT_PARA, (void *)&init_para);


	/* 4. set pano display mode */
	win_para.mode = VIDEO_WINDOW_USERDEF;
	win_para.rect.x = 0;
	win_para.rect.y = 100;
	win_para.rect.width = 320;
	win_para.rect.height = 160;
	ret = lb_recorder_ctrl(erecorder, LB_REC_PANO_SET_DISP_MODE, (void *)&win_para);

	prev_size.width = 160;
	prev_size.height = 320;
	ret = lb_recorder_ctrl(erecorder, LB_REC_PANO_SET_PREVIEW_SIZE,
				(void *)&prev_size);

	if (cali_flag) {
		ret = lb_recorder_ctrl(erecorder, LB_REC_PANO_SET_CALI_DATA, &g_cali_out);
		if (g_cali_out.data)
			oscl_free(g_cali_out.data);
		memset(&g_cali_out, 0, sizeof(g_cali_out));
	}
	/* 5. enable pano stream */
	ret = lb_recorder_ctrl(erecorder, LB_REC_PANO_START, NULL);

	/* 6. enable calibration */
	if (0 == cali_flag) {
		memset(&cali_ctx, 0, sizeof(cali_ctx));
		ret = lb_recorder_ctrl(erecorder, LB_REC_PANO_CALI_PROCESS, &cali_ctx);
		if (0 == ret) {
			OSCL_LOGI("cutline_dnthr:%d", cali_ctx.cutline_dnthr);
			OSCL_LOGI("cutline_upthr:%d", cali_ctx.cutline_upthr);
			OSCL_LOGI("car_rect.x:%d", cali_ctx.car_rect.x);
			OSCL_LOGI("car_rect.y:%d", cali_ctx.car_rect.y);
			OSCL_LOGI("car_rect.width:%d", cali_ctx.car_rect.width);
			OSCL_LOGI("car_rect.height:%d", cali_ctx.car_rect.height);
		}
	}
	return ret;
}

static void pano_exit(void *erecorder)
{
	int ret = 0;
	cali_out_data_t out;

	if (NULL == erecorder)
		return;
#if 1
	ret = lb_recorder_ctrl(erecorder, LB_REC_PANO_GET_CALI_DATA, &out);
	OSCL_LOGE("get cali out data, ret:%d", ret);
	if ((0 == ret) && (out.data)) {
		g_cali_out.data = oscl_zalloc(out.data_size);
		if (NULL == g_cali_out.data) {
			OSCL_LOGE("Malloc fail...");
		} else {
			memcpy(g_cali_out.data, out.data, out.data_size);
			g_cali_out.data_size = out.data_size;
			cali_flag = 1;
		}
		oscl_free(out.data);
	}
#endif
	lb_recorder_ctrl(erecorder, LB_REC_PANO_STOP, NULL);
	lb_recorder_ctrl(erecorder, LB_REC_PANO_RELEASE, NULL);
}

struct option long_options[] = {
	{"file", 1, 0, 'f'},
	{"fixtime", 0, 0, 'x'},
	{"prepare", 0, 0, 'p'},
	{"preview", 0, 0, 'v'},
	{"stop-preview", 0, 0, 't'},
	{"rec", 0, 0, 'r'},
	{"stop", 0, 0, 's'},
	{"quit", 0, 0, 'q'},
	{"help", 0, 0, 'h'},
	{"callback", 0, 0, 'c'},
	{"pano", 0, 0, 'n'},
	{"exit-pano", 0, 0, 'e'},
	{"time", 0, 0, 'm'},
	{"status", 0, 0, 'u'},
	{"auto-all", 0, 0, 'a'},
	{"pic", 1, 0, 'i'},
	{"watermark", 1, 0, 'w'}
};
int cb_get_next_file(void *hdl, char *next_file)
{
	int idx = 0;
	int i;
	lb_recorder_t *recorder = (lb_recorder_t *)hdl;

	for (i = 0; i < TEST_MAX_REC_NUM; i++) {
		if (recorder == g_test_para[i].recorder) {
			g_test_para[i].loop++;
			idx = i;
		}
	}

	list_mem();
	sprintf(next_file, "%s%s_%04d.mp4", g_test_para[idx].path,
		recorder->video_rec->camera->vsrc_info.dev_name,
		g_test_para[idx].loop%70);
	OSCL_LOGE("get new file : %s\n", next_file);
	return 0;
}

int cb_get_next_jpg(void *hdl, char *next_file)
{
	int idx = 0;
	int i;
	lb_recorder_t *recorder = (lb_recorder_t *)hdl;

	for (i = 0; i < TEST_MAX_REC_NUM; i++) {
		if (recorder == g_test_para[i].recorder)
			idx = i;
	}

	list_mem();
	sprintf(next_file, "%s%s_%04d.jpg", g_test_para[idx].path,
		recorder->video_rec->camera->vsrc_info.dev_name,
		g_test_para[idx].loop%70);
	return 0;
}

int cb_file_closed(void *hdl, char *file_name)
{
	OSCL_LOGE("close new file hdl:%x, file %s closed", hdl, file_name);
	return 0;
}
static fix_duration_param_t fix_duration_param;

void auto_lb_timeout(void *para)
{
	auto_test_para_t *testpara = (auto_test_para_t *)para;
	rt_enter_critical();
	if (testpara->loop == testpara->last_loop) {
		OSCL_LOGE("======timer(%d:%s): line:%d",
			testpara->index, testpara->source, testpara->line);
		if (testpara->auto_thread) {
			OSCL_LOGE("dump test_thread(%s)", testpara->auto_thread->name);
			dump_stack_thread(testpara->auto_thread);
		}
		OSCL_LOGE("\n===list thread===");
		list_thread();
		OSCL_LOGE("\n===list sem===");
		list_sem();
		OSCL_LOGE("\n===list mutex===");
		list_mutex();
		OSCL_LOGE("\n===dump_stack_all===");
		dump_stack_all();
		OSCL_LOGE("\n===list_timer===");
		list_timer();
		OSCL_LOGE("\n===list_mailbox===");
		list_mailbox();
		OSCL_LOGE("\n===list_msgqueue===");
		list_msgqueue();
		OSCL_LOGE("\n===list_event===");
		list_event();
		OSCL_LOGE("\n===list_mempool===");
		list_mempool();
		OSCL_LOGE("\n===omx_show_active===");
		/*
		omx_show_active();
		OSCL_LOGE("\n");
		*/
	}
	testpara->last_loop = testpara->loop;
	rt_exit_critical();
}

void auto_test_starttimer(auto_test_para_t *testpara, int timeout)
{
#if 0
	if (testpara->timer == NULL) {
		OSCL_LOGE("====creat and start timer %d", testpara->index);
		testpara->timer = rt_timer_create("autolb",
			auto_lb_timeout, testpara,
			timeout * RT_TICK_PER_SECOND,
			RT_TIMER_FLAG_SOFT_TIMER | RT_TIMER_FLAG_PERIODIC);
		rt_timer_start(testpara->timer);
	}
#endif
}

void auto_stop_timer(auto_test_para_t *testpara)
{
#if 0
	OSCL_LOGE("====stop and del timer %d", testpara->index);
	if (testpara->timer) {
		testpara->loop++;
		rt_timer_stop(testpara->timer);
		rt_timer_delete(testpara->timer);
		testpara->timer = NULL;
	}
#endif
}

static long lbrecorder(int argc, char **argv)
{
	int c;
	int index;
	int ret = -1;
	int cmd = -1;
	int time = 0;
	int status = 0;
	void *para = NULL;
	win_para_t disp_para;
	app_frame_cb_t cb;
	static int cb_cnt;
	char *dev = "vic";
	char filename[50];
	int count = 1;
	char **tmp_argv;
	int tmp_argc;
	int g_cur_player = 1;
	void *recorder;

	if (disp == NULL)
		open_disp_engine();

	if (g_test_para[0].source == NULL)
		init_test_para();

	dev = "vic";
	tmp_argc = argc;
	tmp_argv = argv;
	if (argc >= 2) {
		if (argv[1][0] == '0') {
			tmp_argc--;
			tmp_argv = &tmp_argv[1];
			g_cur_player = 0;
		} else if (argv[1][0] == '1') {
			tmp_argc--;
			tmp_argv = &tmp_argv[1];
			g_cur_player = 1;
		}
	}
	dev = g_test_para[g_cur_player].source;
	recorder = g_test_para[g_cur_player].recorder;

	OSCL_LOGI("lbrecorder start............");
	list_mem();
	if (recorder == NULL) {
		recorder = lb_recorder_creat();
		g_test_para[g_cur_player].recorder = recorder;
		if (recorder) {
			vsize_t size;

			memset(&size, 0, sizeof(size));
			lb_recorder_ctrl(recorder, LB_REC_SET_VIDEO_SOURCE, (void *)dev);

			_get_config_disp_para(&disp_para, g_cur_player);
			lb_recorder_ctrl(recorder, LB_REC_SET_DISP_PARA, &disp_para);
			lb_recorder_ctrl(recorder, LB_REC_SET_PARA,
				&g_test_para[g_cur_player].rec_para);
			lb_recorder_ctrl(recorder, LB_REC_SET_ROTATE, (void *)1);
			lb_recorder_ctrl(recorder, LB_REC_PREPARE, (void *)para);
			cb_get_next_file(recorder, filename);
			lb_recorder_ctrl(recorder, LB_REC_SET_OUTPUT_FILE, filename);
		}
	}
	if (recorder == NULL) {
		OSCL_LOGE("lb_recorder_creat failed!");
		optind = 0;
		return -1;
	}
	OSCL_LOGE("recorder:%x", recorder);
	OSCL_LOGE("recorder:%x argc:%d, argv:%s", recorder, argc, *argv);

	while ((c = getopt_long(argc, argv,
				"fxpvtrsqhcnwemuai", long_options, &index)) != -1) {
		LOG_E("option:%c", c);
		LOG_E("option:%x, optarg:%s", c, (optarg == NULL) ? "null" : optarg);
		switch (c) {
		case 'H':
		case 'h':
			lbrecorder_help();
			break;
		case 'f':
			cmd = LB_REC_SET_OUTPUT_FILE;
			para = (void *)optarg;
			break;
		case 'p': {
			cmd = LB_REC_PREPARE;
			break;
		}
		case 'v': {
			cmd = LB_REC_PREVIEW;
			break;
		}
		case 't': {
			cmd = LB_REC_STOP_PREVIEW;
			break;
		}
		case 'r': {
			cmd = LB_REC_START;
			break;
		}
		case 'x': {
			fix_duration_param.file_duration = 30;
			fix_duration_param.cb_get_next_file = cb_get_next_file;
			fix_duration_param.cb_file_closed = cb_file_closed;
			para = &fix_duration_param;
			cmd = LB_REC_SET_FIX_DURATION_PARA;
			break;
		}
		case 's': {
			count++;
			cmd = LB_REC_STOP;
			break;
		}
		case 'q': {
			lb_recorder_release(recorder);
			recorder = NULL;
			g_test_para[g_cur_player].recorder = NULL;
			break;
		}
		case 'c': {
			cmd = LB_REC_SET_CB_FRAME;
			cb.app_data = recorder;
			cb.buf_handle = buf_handle;
			cb.type = AL_VIDEO_RAW_FRAME;
			if (cb_cnt++ % 3 == 0)
				cb.buf_handle = NULL;
			para = &cb;
			break;
		}
		case 'n': {
			cmd = -1;
			pano_test(recorder);
			break;
		}
		case 'e': {
			cmd = -1;
			pano_exit(recorder);
			break;
		}
		case 'm': {
			cmd = LB_REC_GET_TIME;
			para = &time;
			break;
		}
		case 'u': {
			cmd = LB_REC_GET_STATUS;
			para = &status;
			break;
		}
		case 'a': {
			if (g_test_para[0].recorder)
				lb_recorder_release(g_test_para[0].recorder);
			if (g_test_para[1].recorder)
				lb_recorder_release(g_test_para[1].recorder);
			recorder = NULL;
			g_test_para[0].recorder = NULL;
			g_test_para[1].recorder = NULL;
			oscl_mdelay(500);
			list_mem();
			OSCL_LOGI("lb_recorder_release....");
			auto_test_all();
			break;
		}
		case 'i': {
			OSCL_LOGE("==");
			cmd = LB_REC_TAKE_PICTURE;
			para = (void *)optarg;
			OSCL_LOGE("==");
			break;
		}
		case 'w': {
			int mark;
			OSCL_LOGE("==");
			if (g_test_para[g_cur_player].watermark == 0) {
				watermark_test_source(recorder);
				g_test_para[g_cur_player].watermark = 1;
			}
			mark = atoi(optarg);
			watermark_test(recorder, mark);
			break;
		}
		default:
			LOG_E("Unexcepted case, please let me know, option '%c'",
				optopt);
			break;
		}
		OSCL_LOGE("recorder:%x cmd:%s", recorder, media_cmd_as_string(cmd));
		if (cmd != -1)
			ret = lb_recorder_ctrl(recorder, cmd, (void *)para);
		if (cmd == LB_REC_GET_TIME || cmd == LB_REC_GET_STATUS)
			OSCL_LOGE("%s:%d", media_cmd_as_string(cmd), *(int *)para);
		OSCL_LOGE("recorder:%x cmd:%s, ret:%x",
			recorder, media_cmd_as_string(cmd), ret);
	}
	OSCL_LOGE("recorder:%x cmd:%s, ret:%x", recorder, media_cmd_as_string(cmd), ret);
	optind = 0;
	return ret;

}

void *auto_test_preview(void *param)
{
	void *recorder = NULL;
	int count = 0;
	rec_param_t recorder_para;
	char filename[50] = {0};
	auto_test_para_t *test_para = param;
	unsigned int start_mem = 0, pre_mem = 0;

	auto_test_starttimer(test_para, 30);
	OSCL_LOGI("======start preview test======");
	start_mem = check_mem(__func__, 0, 0);
	memset(&recorder_para, 0, sizeof(rec_param_t));
	recorder = test_para->recorder;
	if (test_para->recorder == NULL) {
		test_para->recorder = lb_recorder_creat();
		recorder = test_para->recorder;
	}
	lb_recorder_ctrl(recorder, LB_REC_SET_VIDEO_SOURCE, test_para->source);
	lb_recorder_ctrl(recorder, LB_REC_SET_PARA, &test_para->rec_para);
	lb_recorder_ctrl(recorder, LB_REC_SET_DISP_PARA, &test_para->disp_para);
	lb_recorder_ctrl(recorder, LB_REC_SET_ROTATE, (void *)test_para->rotate);
	lb_recorder_ctrl(recorder, LB_REC_PREPARE, 0);

	if (test_para->record_flag) {
		cb_get_next_file(recorder, filename);
		lb_recorder_ctrl(recorder, LB_REC_SET_OUTPUT_FILE, filename);
		lb_recorder_ctrl(recorder, LB_REC_START, 0);
		OSCL_LOGI("Start rec....");
	}
	oscl_mdelay(1000);
	while ((!test_para->stop_flag) && (count < test_max_loop_num)) {
		count++;
		test_para->loop++;
		recorder_para.rotate = count%4;
		lb_recorder_ctrl(recorder, LB_REC_PREVIEW, 0);
		if (test_para->delay > 0)
			oscl_mdelay(test_para->delay);
		else
			oscl_mdelay(2000);
		OSCL_LOGI("LB_REC_STOP_PREVIEW");
		lb_recorder_ctrl(recorder, LB_REC_STOP_PREVIEW, 0);
		OSCL_LOGI("======stop count:%d =========", count);
		OSCL_LOGI("==============rotate:%d w,h:%d-%d=============",
			recorder_para.rotate, recorder_para.source_width,
			recorder_para.source_height);
		pre_mem = check_mem(__func__, pre_mem, count);
		oscl_mdelay(500);
	}
	if (test_para->record_flag)
		lb_recorder_ctrl(recorder, LB_REC_STOP, 0);
	lb_recorder_release(recorder);
	test_para->recorder = NULL;
	recorder = NULL;
	oscl_mdelay(500);
	check_mem(__func__, start_mem, 0);
	OSCL_LOGI("======preview test complete======");
	auto_stop_timer(test_para);
	return 0;
}

void *auto_test_recording(void *param)
{
	void *recorder;
	int count = 0;
	rec_param_t recorder_para;
	char filename[50];
	auto_test_para_t *test_para = param;
	unsigned int start_mem = 0, pre_mem = 0;
	int mode =  RECORDER_TYPE_NORMAL;

	auto_test_starttimer(test_para, 30);
	OSCL_LOGI("======start recording test======");
	start_mem = check_mem(__func__, 0, 0);
	if (test_para->recorder == NULL)
		test_para->recorder = lb_recorder_creat();

	recorder = test_para->recorder;
	if (test_para->record_type)
		mode = RECORDER_TYPE_TIME_LAG;
	lb_recorder_ctrl(recorder, LB_REC_SET_MODE, &mode);
	lb_recorder_ctrl(recorder, LB_REC_SET_TIME_LAG_PARA,
		&test_para->time_lag_para);
	memset(&recorder_para, 0, sizeof(rec_param_t));
	lb_recorder_ctrl(recorder, LB_REC_SET_VIDEO_SOURCE, test_para->source);
	lb_recorder_ctrl(recorder, LB_REC_SET_PARA, &test_para->rec_para);
	lb_recorder_ctrl(recorder, LB_REC_SET_DISP_PARA, &test_para->disp_para);
	lb_recorder_ctrl(recorder, LB_REC_SET_ROTATE, (void *)test_para->rotate);
	lb_recorder_ctrl(recorder, LB_REC_PREPARE, 0);
	if (test_para->preview_flag) {
		OSCL_LOGI("Start preview.....");
		lb_recorder_ctrl(recorder, LB_REC_PREVIEW, 0);
	}
	while ((!test_para->stop_flag) && (count < test_max_loop_num)) {
		count++;
		recorder_para.rotate = count%4;
		memset(&recorder_para, 0, sizeof(rec_param_t));
		cb_get_next_file(recorder, filename);
		OSCL_LOGE("======start count===:%d %s=========", count, filename);
/*
		if (count%2) {
			recorder_para.source_height = 640;
			recorder_para.source_width = 480;
		}
*/
		lb_recorder_ctrl(recorder, LB_REC_SET_OUTPUT_FILE, filename);
		lb_recorder_ctrl(recorder, LB_REC_START, 0);
		if (test_para->delay > 0)
			oscl_mdelay(test_para->delay);
		else
			oscl_mdelay(2000);
		lb_recorder_ctrl(recorder, LB_REC_STOP, 0);
		oscl_mdelay(500);
		pre_mem = check_mem(__func__, pre_mem, count);
		OSCL_LOGE("======stop count:%d %s=========", count, filename);
		OSCL_LOGE("==============rotate:%d w,h:%d-%d=============",
			recorder_para.rotate, recorder_para.source_width,
			recorder_para.source_height);
	}
	if (test_para->preview_flag)
		lb_recorder_ctrl(recorder, LB_REC_STOP_PREVIEW, 0);
	lb_recorder_release(recorder);
	recorder = NULL;
	test_para->recorder = NULL;
	oscl_mdelay(500);
	check_mem(__func__, start_mem, 0);
	OSCL_LOGI("======recording test complete======");
	auto_stop_timer(test_para);

	return 0;
}

void *auto_test_recorder(void *param)
{
	void *recorder;
	char filename[50];
	auto_test_para_t *test_para = param;
	unsigned int start_mem = 0, pre_mem = 0;
	int mode = RECORDER_TYPE_NORMAL;

	auto_test_starttimer(test_para, 30);
	OSCL_LOGI("======start recorder test:%x======", test_para);
	start_mem = check_mem(__func__, 0, 0);
	while ((!test_para->stop_flag) && (test_para->loop < test_max_loop_num)) {
		if (test_para->record_flag == 0)
			test_para->loop++;
		if (test_para->recorder == NULL)
			test_para->recorder = lb_recorder_creat();

		recorder = test_para->recorder;
		OSCL_LOGE("======%d start:%d======", test_para->index, test_para->loop);
		lb_recorder_ctrl(recorder, LB_REC_SET_VIDEO_SOURCE, test_para->source);
		if (test_para->record_type)
			mode = RECORDER_TYPE_TIME_LAG;
		lb_recorder_ctrl(recorder, LB_REC_SET_MODE, &mode);
		lb_recorder_ctrl(recorder, LB_REC_SET_TIME_LAG_PARA,
			&test_para->time_lag_para);
		lb_recorder_ctrl(recorder, LB_REC_SET_PARA, &test_para->rec_para);
		lb_recorder_ctrl(recorder, LB_REC_SET_DISP_PARA, &test_para->disp_para);
		lb_recorder_ctrl(recorder, LB_REC_SET_ROTATE, (void *)test_para->rotate);
		lb_recorder_ctrl(recorder, LB_REC_PREPARE, 0);
		OSCL_LOGI("%x-%d record_flag:%d, preview_flag:%d",
			recorder, test_para->index,
			test_para->record_flag, test_para->preview_flag);
		if (test_para->preview_flag)
			lb_recorder_ctrl(recorder, LB_REC_PREVIEW, 0);
		if (test_para->record_flag) {
			cb_get_next_file(recorder, filename);
			lb_recorder_ctrl(recorder, LB_REC_SET_OUTPUT_FILE, filename);
			lb_recorder_ctrl(recorder, LB_REC_START, 0);
			cb_get_next_jpg(recorder, filename);
			lb_recorder_ctrl(recorder, LB_REC_TAKE_PICTURE, filename);
		}

		if (test_para->delay > 0)
			oscl_mdelay(test_para->delay);
		else
			oscl_mdelay(2000);
		if (test_para->record_flag)
			lb_recorder_ctrl(recorder, LB_REC_STOP, 0);
		if (test_para->preview_flag)
			lb_recorder_ctrl(recorder, LB_REC_STOP_PREVIEW, 0);
		lb_recorder_release(recorder);
		pre_mem = check_mem(__func__, pre_mem, test_para->loop);
		OSCL_LOGE("======stop:%d %s=========", test_para->loop, filename);
		recorder = NULL;
		test_para->recorder = NULL;
		oscl_mdelay(500);
	}
	oscl_mdelay(500);
	check_mem(__func__, start_mem, 0);
	OSCL_LOGI("======recorder complete======");
	auto_stop_timer(test_para);

	return 0;
}

void *auto_test_fix_duration(void *param)
{
	void *recorder;
	int count = 0;
	char filename[50];
	auto_test_para_t *test_para = param;
	fix_duration_param_t fix_duration_param;
	unsigned int start_mem = 0;
	int start_time = 0;
	int test_time = 0;
	int mode =  RECORDER_TYPE_NORMAL;

	auto_test_starttimer(test_para, 90);
	OSCL_LOGI("======start fix duration test======");
	oscl_mdelay(500);
	start_mem = check_mem(__func__, 0, 0);
	if (test_para->recorder == NULL)
		test_para->recorder = lb_recorder_creat();
	recorder = test_para->recorder;
	if (test_para->record_type)
		mode = RECORDER_TYPE_TIME_LAG;
	lb_recorder_ctrl(recorder, LB_REC_SET_MODE, &mode);
	lb_recorder_ctrl(recorder, LB_REC_SET_TIME_LAG_PARA,
		&test_para->time_lag_para);
	OSCL_LOGE("======start count===:%d %s=========", count, filename);
	lb_recorder_ctrl(recorder, LB_REC_SET_VIDEO_SOURCE, test_para->source);
	lb_recorder_ctrl(recorder, LB_REC_SET_PARA, &test_para->rec_para);
	lb_recorder_ctrl(recorder, LB_REC_SET_DISP_PARA, &test_para->disp_para);
	lb_recorder_ctrl(recorder, LB_REC_SET_ROTATE, (void *)test_para->rotate);
	lb_recorder_ctrl(recorder, LB_REC_PREPARE, 0);
	lb_recorder_ctrl(recorder, LB_REC_PREVIEW, 0);
	cb_get_next_file(recorder, filename);
	lb_recorder_ctrl(recorder, LB_REC_SET_OUTPUT_FILE, filename);
	lb_recorder_ctrl(recorder, LB_REC_START, 0);
	if (test_para->record_type == RECORDER_TYPE_NORMAL)
		fix_duration_param.file_duration = 60;
	else
		fix_duration_param.file_duration = 5;
	fix_duration_param.cb_get_next_file = cb_get_next_file;
	fix_duration_param.cb_file_closed = cb_file_closed;
	lb_recorder_ctrl(recorder, LB_REC_SET_FIX_DURATION_PARA, &fix_duration_param);

	test_time = fix_duration_param.file_duration * test_max_loop_num * 1000;
	start_time = oscl_get_msec();
	while (!test_para->stop_flag) {
		oscl_mdelay(100);
		if ((oscl_get_msec() - start_time) >= test_time)
			break;
	}

	lb_recorder_ctrl(recorder, LB_REC_STOP, 0);
	lb_recorder_ctrl(recorder, LB_REC_STOP_PREVIEW, 0);
	lb_recorder_release(recorder);
	oscl_mdelay(500);
	check_mem(__func__, start_mem, 0);
	auto_stop_timer(test_para);
	OSCL_LOGI("======fix duration complete======");
	recorder = NULL;
	test_para->recorder = NULL;

	return 0;
}

void *auto_test_pano(void *param)
{
	void *recorder[2] = {NULL, NULL};
	int count = 0;
	char filename[50] = {0};
	auto_test_para_t *test_para = param;
	unsigned int start_mem = 0, pre_mem = 0;

	auto_test_starttimer(test_para, 30);
	OSCL_LOGI("======start pano test======");
	oscl_mdelay(500);
	start_mem = check_mem(__func__, 0, 0);
	/* 1. create recorder */
	recorder[0] = lb_recorder_creat();

	/* 2. set video input source */
	lb_recorder_ctrl(recorder[0], LB_REC_SET_VIDEO_SOURCE, "vic");

	/* 3. set rotate angle */
	lb_recorder_ctrl(recorder[0], LB_REC_SET_ROTATE, (void *)1);

	/* 4. recorder prepare */
	lb_recorder_ctrl(recorder[0], LB_REC_PREPARE, NULL);
	memset(&g_cali_out, 0, sizeof(g_cali_out));
	if (1 == test_para->record_flag) {
		snprintf(filename, sizeof(filename), VIC_PATH"test_pano_0.mp4");
		lb_recorder_ctrl(recorder[0], LB_REC_SET_OUTPUT_FILE, filename);
		lb_recorder_ctrl(recorder[0], LB_REC_START, 0);
		OSCL_LOGI("Start rec1....");
	} else if (2 == test_para->record_flag) {
		snprintf(filename, sizeof(filename), VIC_PATH"test_pano_0.mp4");
		lb_recorder_ctrl(recorder[0], LB_REC_SET_OUTPUT_FILE, filename);
		lb_recorder_ctrl(recorder[0], LB_REC_START, 0);
		OSCL_LOGI("Start rec1....");

		recorder[1] = lb_recorder_creat();
		lb_recorder_ctrl(recorder[1], LB_REC_SET_VIDEO_SOURCE, "isp");
		lb_recorder_ctrl(recorder[1], LB_REC_SET_ROTATE, (void *)1);
		lb_recorder_ctrl(recorder[1], LB_REC_PREPARE, NULL);
		snprintf(filename, sizeof(filename), ISP_PATH"test_pano_1.mp4");
		lb_recorder_ctrl(recorder[1], LB_REC_SET_OUTPUT_FILE, filename);
		lb_recorder_ctrl(recorder[1], LB_REC_START, 0);
		OSCL_LOGI("Start rec2....");
	}
	oscl_mdelay(1000);
	while ((!test_para->stop_flag) && (count < test_max_loop_num)) {
		count++;
		test_para->loop++;

		/* 5. create pano test */
		pano_test(recorder[0]);
		if (test_para->delay > 0)
			oscl_mdelay(test_para->delay);
		else
			oscl_mdelay(2000);

		/* 6. release pano test */
		pano_exit(recorder[0]);
		oscl_mdelay(500);
		pre_mem = check_mem(__func__, pre_mem, count);
		OSCL_LOGI("\n======stop count:%d=========\n", count);
	}
	if (test_para->record_flag) {
		lb_recorder_ctrl(recorder[0], LB_REC_STOP, 0);
		if (recorder[1])
			lb_recorder_ctrl(recorder[1], LB_REC_STOP, 0);
	}
	if (recorder[0])
		lb_recorder_release(recorder[0]);
	if (recorder[1])
		lb_recorder_release(recorder[1]);
	recorder[0] = NULL;
	recorder[1] = NULL;
	if (g_cali_out.data)
		oscl_free(g_cali_out.data);
	memset(&g_cali_out, 0, sizeof(g_cali_out));
	cali_flag = 0;
	oscl_mdelay(500);
	check_mem(__func__, start_mem, 0);
	auto_stop_timer(test_para);
	OSCL_LOGI("======pano test complete======");

	return 0;
}

static long auto_lb(int argc, char **argv)
{
	int index = 0;
	int total = 1;
	int i;
	auto_test_para_t tmp_para;
	pthread_attr_t thread_attr;
	void *(*test_func)(void *);
	int _argv_int[20];

	init_test_para();
	if (disp == NULL)
		open_disp_engine();

	if (argc >= 2) {
		if (argv[1][0] == '1')
			index = 1;
		if (argv[1][0] == 'a') {
			index = 0;
			total = 2;
		}
	}

	memset(&tmp_para, 0, sizeof(tmp_para));
	memset(_argv_int, 0, sizeof(_argv_int));
	for (i = 3; i < argc; i++)
		_argv_int[i] = atoi(argv[i]);
	test_func = NULL;

	if (argc >= 3 && !strncmp(argv[2], "stop", 4)) {
		g_test_para[index].stop_flag = 1;
		wait_test_finished(index);
		if (total != 1) {
			for (i = 0; i < 4; i++) {
				g_test_para[i].stop_flag = 1;
				wait_test_finished(i);
			}
		}
#if 0
		osal_dump();
		list_mem();
#endif
		return 0;
	} else if (argc >= 4 && !strncmp(argv[2], "preview", 4)) {
		test_func = auto_test_preview;
		tmp_para.record_flag = _argv_int[3];
		tmp_para.delay = _argv_int[4];
	} else if (argc >= 4 && !strncmp(argv[2], "rec", 4)) {
		test_func = auto_test_recording;
		tmp_para.preview_flag = _argv_int[3];
		tmp_para.delay = _argv_int[4];
		tmp_para.time_lag_para.interval = _argv_int[5];
		tmp_para.time_lag_para.play_framerate = _argv_int[6];
		tmp_para.record_type = _argv_int[7];
	} else if (argc >= 5 && !strncmp(argv[2], "recorder", 4)) {
		test_func = auto_test_recorder;
		tmp_para.preview_flag = _argv_int[3];
		tmp_para.record_flag = _argv_int[4];
		tmp_para.delay = _argv_int[5];
		tmp_para.time_lag_para.interval = _argv_int[6];
		tmp_para.time_lag_para.play_framerate = _argv_int[7];
		tmp_para.record_type = _argv_int[8];
	} else if (argc >= 3 && !strncmp(argv[2], "dvr", 3)) {
		index = 0;
		total = atoi(argv[1]);
		if ((total < 2) || (total > TEST_MAX_REC_NUM)) {
			OSCL_LOGE("Dvr chanel num error. total:%d", total);
			return -1;
		}
		test_func = auto_test_recorder;
		tmp_para.preview_flag = _argv_int[3];
		tmp_para.record_flag = _argv_int[4];
		tmp_para.delay = _argv_int[5];
		tmp_para.time_lag_para.interval = _argv_int[6];
		tmp_para.time_lag_para.play_framerate = _argv_int[7];
		tmp_para.record_type = _argv_int[8];
	} else if (argc >= 4 && !strncmp(argv[2], "pano", 4)) {
		test_func = auto_test_pano;
		tmp_para.record_flag = _argv_int[3];
		tmp_para.delay = _argv_int[4];
	} else if (argc >= 3 && !strncmp(argv[2], "fixtime", 4)) {
		test_func = auto_test_fix_duration;
		tmp_para.time_lag_para.interval = _argv_int[3];
		tmp_para.time_lag_para.play_framerate = _argv_int[4];
		tmp_para.record_type = _argv_int[5];
	} else {
		OSCL_LOGE("Input para error.");
		return -1;
	}

	init_dvr_para(total);
	for (i = index; i < index + total; i++) {
		g_test_para[i].preview_flag = tmp_para.preview_flag;
		if (i != 0)
			g_test_para[i].preview_flag = 0;
		g_test_para[i].record_flag = tmp_para.record_flag;
		g_test_para[i].delay = tmp_para.delay;
		g_test_para[i].record_type = tmp_para.record_type;
		memcpy(&g_test_para[i].time_lag_para, &tmp_para.time_lag_para,
			sizeof(rec_time_lag_para_t));

		_get_config_disp_para(&g_test_para[i].disp_para, i);
		g_test_para[i].stop_flag = 0;
		pthread_attr_init(&thread_attr);
		pthread_attr_setstacksize(&thread_attr, 0x2000);
		pthread_create(&g_test_para[i].auto_thread, &thread_attr,
			       test_func, &g_test_para[i]);
	}

	if (test_all_flag) {
		for (i = 0; i < 4; i++)
			wait_test_finished(i);
	}
	return 0;
}

test_cmd_t rec_cmd[] = {
	{
		"auto_lb a dvr 1 1",
		5,
		{"auto_lb", "a", "dvr", "1", "1"},
	},
	{
		"auto_lb 0 preview 0",
		4,
		{"auto_lb", "0", "preview", "0"},
	},
	{
		"auto_lb 0 preview 1",
		4,
		{"auto_lb", "0", "preview", "1"},
	},
	{
		"auto_lb 0 rec 0",
		4,
		{"auto_lb", "0", "rec", "0"},
	},
	{
		"auto_lb 0 rec 1",
		4,
		{"auto_lb", "0", "rec", "1"},
	},
	{
		"auto_lb 0 recorder 0 0",
		5,
		{"auto_lb", "0", "recorder", "0", "0"},
	},
	{
		"auto_lb 0 recorder 0 1",
		5,
		{"auto_lb", "0", "recorder", "0", "1"},
	},
	{
		"auto_lb 0 recorder 1 1",
		5,
		{"auto_lb", "0", "recorder", "1", "1"},
	},
	{
		"auto_lb 0 fixtime",
		3,
		{"auto_lb", "0", "fixtime"},
	},
	{
		"auto_lb 0 pano 1",
		4,
		{"auto_lb", "0", "pano", "1"},
	},
	{
		"auto_lb a preview 0",
		4,
		{"auto_lb", "a", "preview", "0"},
	},
	{
		"auto_lb a preview 1",
		4,
		{"auto_lb", "a", "preview", "1"},
	},
	{
		"auto_lb a rec 0",
		4,
		{"auto_lb", "a", "rec", "0"},
	},
	{
		"auto_lb a rec 1",
		4,
		{"auto_lb", "a", "rec", "1"},
	},
	{
		"auto_lb a recorder 0 0",
		5,
		{"auto_lb", "a", "recorder", "0", "0"},
	},
	{
		"auto_lb a recorder 0 1",
		5,
		{"auto_lb", "a", "recorder", "0", "1"},
	},
	{
		"auto_lb a recorder 1 1",
		5,
		{"auto_lb", "a", "recorder", "1", "1"},
	},
	{
		"auto_lb a fixtime",
		3,
		{"auto_lb", "a", "fixtime"},
	},
	{
		"auto_lb 0 pano 2",
		4,
		{"auto_lb", "0", "pano", "2"},
	}
};

void auto_test_all(void)
{
	int argc;
	int i = 0, j = 0;
	int cmd_num = ARRAY_SIZE(rec_cmd);
	char *argv[FINSH_ARG_MAX];
	unsigned int start_mem = 0;

	start_mem = check_mem(__func__, 0, 0);
	memset(argv, 0, sizeof(argv));
	for (i = 0; i < FINSH_ARG_MAX; i++) {
		argv[i] = oscl_zalloc(MAX_ARGS_LEN);
		OSCL_LOGI("argv[%d]:%p", i, argv[i]);
		if (NULL == argv[i]) {
			OSCL_LOGE("Malloc fail.");
			return;
		}
	}
	test_all_flag = 1;
	test_max_loop_num = 3;
	for (i = 0; i < cmd_num; i++) {
		printf("\n*****************************************************\n");
		printf("Test command(%d) [%s]\n", i, rec_cmd[i].str);
		list_mem();
		printf("*****************************************************\n");
		argc = rec_cmd[i].argc;
		for (j = 0; j < argc; j++)
			strncpy(argv[j], rec_cmd[i].argv[j], MAX_ARGS_LEN);
		auto_lb(argc, argv);
	}
	for (i = 0; i < FINSH_ARG_MAX; i++) {
		if (argv[i])
			oscl_free(argv[i]);
	}
	test_all_flag = 0;
	check_mem(__func__, start_mem, 0);
}

MSH_CMD_EXPORT(lbrecorder, "lombo recorder");
MSH_CMD_EXPORT(auto_lb, "auto_lombo recorder");

