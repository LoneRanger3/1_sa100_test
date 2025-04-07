/*
 * vce_recorder.c - main interface of vce recorder testcase.
 *
 * Copyright (C) 2016 - 2019, LomboTech Co.Ltd.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include "lombo_al_lib.h"
#include "lombo_encplugin.h"
#include "oscl.h"
static int frame_encode_num = -1;

#define MAX_STREAM_BUFFER_NUM 5
#define H264_INFO_SIZE 1024
#define MAX_VC_ENCODE_INSTANCE 10
#define MAX_TEST_LOOP 1
#define STEAM_BUF_SIZE (2*1024)

#define BASE_ROOT "/mnt/sdcard/"

enum THREAD_STATUS {
	THREAD_INVALID = 0,
	THREAD_INIT,
	THREAD_WAIT_JOIN_NULL,
	THREAD_EXIT,
};

typedef struct vce_test_param {
	int stop_flag;
	int width_in;
	int l_stride;
	int height_in;
	int width_out;
	int height_out;
	int bitrate;
	int input_mode;
	int index;
	char *input_file_name[100];
	pthread_t auto_thread;
	int thread_status;
} vce_test_param_t;

static vce_test_param_t *test_param[MAX_VC_ENCODE_INSTANCE];


typedef struct vce_encode {
	int width_in;
	int height_in;
	int width_out;
	int height_out;
	int bitrate;
	int offset_y;
	int offset_u;
	int offset_v;
	int slice_mode;
	int input_mode;
	void *stream_port;
	FILE *input_flip;
	FILE *output_flip;
	void *handle;
	long long capture_time;
	venc_parm_t *encode_parm;
	unsigned char *capture_buffer_vir;
	unsigned long capture_buffer_phy;
	void *lib_handle;
	venc_plugin_t *video_encode_info;
} vce_encode_t;


void *vce_test_al_malloc(int size, int flag, unsigned long *phy_addr,
			char *file, int line)
{
	void *address = NULL;

	address = lombo_al_malloc(size, flag, phy_addr, file, line);
	/* lombo_al_printf("address : %x\n", address); */

	return address;
}

void vce_test_al_free(void *ptr, int flag)
{
	lombo_al_free(ptr, flag);
	/* lombo_al_printf("ptr : %x\n", ptr); */
}

venc_plugin_t h264_plugin = {
	.open = h264_enc_open,
	.queue_buf = h264_queue_buf,
	.encode_frame = h264_enc_frame,
	.dequeue_buf = h264_dequeue_buf,
	.close = h264_enc_close,
	.ex_ops = h264_enc_ops,
};
static int __test_load_symb(vce_encode_t *vec_private)
{
	vec_private->video_encode_info = &h264_plugin;
	return 0;
}

static void __test_unload_symb(vce_encode_t *vec_private)
{
	vec_private->video_encode_info = NULL;
}


static int get_capture_data(vce_encode_t *vce_encode)
{
	int read_len = 0;
	unsigned char *vir_addr = vce_encode->capture_buffer_vir;
	unsigned char *y_data = vir_addr + vce_encode->offset_y;
	unsigned char *u_data = NULL;
	unsigned char *v_data = NULL;

	int input_mode = vce_encode->input_mode;
	int in_width = vce_encode->width_in;
	int in_height = vce_encode->height_in;

	int y_len = in_width * in_height;
	int u_len = -1;
	int v_len = -1;

	int offset_y = vce_encode->offset_y;
	int offset_u = vce_encode->offset_u;
	int offset_v = vce_encode->offset_v;

	y_data = vir_addr + offset_y;

	/* lombo_al_printf("y_len : %d\n", y_len); */

	read_len = fread(y_data, 1, y_len, vce_encode->input_flip);
	if (read_len != y_len) {
		if (1) {
			lombo_al_printf("file end 1\n");
			return -1;
		}
	}
	if ((input_mode == ENC_YUV420P) || (input_mode == ENC_YVU420P)) {
		u_len = in_width * in_height / 4;
		u_data = vir_addr + offset_u;

		read_len = fread(u_data, 1, u_len, vce_encode->input_flip);
		if (read_len != u_len) {
			if (1) {
				lombo_al_printf("file end 2\n");
				return -1;
			}
		}
		v_len = in_width * in_height / 4;
		v_data = vir_addr + offset_v;

		read_len = fread(v_data, 1, v_len, vce_encode->input_flip);
		if (read_len != v_len) {
			if (1) {
				lombo_al_printf("file end 3\n");
				return -1;
			}
		}
	} else if ((input_mode == ENC_YUV420SP) ||
		(input_mode == ENC_YVU420SP)) {
		u_len = in_width * in_height / 2;
		u_data = vir_addr + offset_u;
		read_len = fread(u_data, 1, u_len, vce_encode->input_flip);
		if (read_len != u_len) {
			if (1) {
				lombo_al_printf("file end 2\n");
				return -1;
			}
		}
	} else {
		lombo_al_printf("not support inputformat\n");
		return -1;
	}

	vce_encode->capture_time += 10;

	return 0;

}

void *vc_encode_thread(void *parm)
{
	int size = 0;
	vce_test_param_t *param = (vce_test_param_t *)parm;
	int test_loop_cnt = 0;

	int start_time = 0;
	int end_time = 0;
	int total_time = 0;
	int frame_num = 0;

	while (1) {

		char *output_file_name[100];
		if ((param->stop_flag  == 1) || (test_loop_cnt >= MAX_TEST_LOOP)) {
			param->thread_status = THREAD_WAIT_JOIN_NULL;
			break;
		}
		test_loop_cnt++;
		lombo_al_printf("start instance : %d -- loop cnt : %d\n",
			param->index, test_loop_cnt);

		sprintf((char *)output_file_name, BASE_ROOT"%02d_%02d_h24.raw",
			param->index, test_loop_cnt%100);

		vce_encode_t *vce_encode = vce_test_al_malloc(sizeof(vce_test_param_t),
			AL_MEM_VIRT, NULL, __FILE__, __LINE__);
		if (vce_encode == NULL) {
			lombo_al_printf("malloc vce_encode error\n");
			goto vce_encode_fail;
		}
		lombo_al_memset(vce_encode, 0, sizeof(vce_encode_t));

		__test_load_symb(vce_encode);

		vce_encode->encode_parm = vce_test_al_malloc(sizeof(venc_parm_t),
			AL_MEM_VIRT, NULL, __FILE__, __LINE__);
		if (vce_encode->encode_parm == NULL) {
			lombo_al_printf("malloc encode_parm error\n");
			goto vce_encode_fail;
		}

		vce_encode->encode_parm->l_stride = param->l_stride;
		vce_encode->encode_parm->in_width =
			vce_encode->width_in = param->width_in;
		vce_encode->encode_parm->in_height =
			vce_encode->height_in = param->height_in;
		vce_encode->encode_parm->out_width =
			vce_encode->width_out = param->width_out;
		vce_encode->encode_parm->out_height =
			vce_encode->height_out = param->height_out;

		vce_encode->encode_parm->bitrate = vce_encode->bitrate = param->bitrate;
		vce_encode->encode_parm->slice_mode = vce_encode->slice_mode = 0;
		vce_encode->encode_parm->input_mode =
			vce_encode->input_mode = param->input_mode;

		vce_encode->encode_parm->idr_period = 30;
		vce_encode->encode_parm->frame_rate = 30;

		if ((vce_encode->input_mode == ENC_YUV420P)
			|| (vce_encode->input_mode == ENC_YVU420P)) {
			vce_encode->offset_y = 0;
			vce_encode->offset_u = (vce_encode->width_in *
				vce_encode->height_in + 4095) & 0xFFFFF000;
			vce_encode->offset_v = (vce_encode->offset_u +
				vce_encode->width_in * vce_encode->height_in
					/ 4 + 4095) & 0xFFFFF000;
		} else if ((vce_encode->input_mode == ENC_YUV420SP)
			|| (vce_encode->input_mode == ENC_YVU420SP)) {
			vce_encode->offset_y = 0;
			vce_encode->offset_u = (vce_encode->width_in *
				vce_encode->height_in + 4095) & 0xFFFFF000;
			vce_encode->offset_v = -1;
		}

		size = vce_encode->width_in * vce_encode->height_in * 3 / 2 + 4096 + 4096;
		vce_encode->capture_buffer_vir = vce_test_al_malloc(size,
			AL_MEM_UC, &(vce_encode->capture_buffer_phy), __FILE__, __LINE__);
		if (vce_encode->capture_buffer_vir == NULL) {
			lombo_al_printf("malloc capture_buffer_vir error\n");
			goto vce_encode_fail;
		}

		/* lombo_al_printf("param->input_file_name : %s\n",
			param->input_file_name); */
		vce_encode->input_flip = fopen((char *)(param->input_file_name), "rb");
		if (vce_encode->input_flip == NULL)
			lombo_al_printf("vce_encode->input_flip open error\n");


		lombo_al_printf("output_file_name : %s\n", output_file_name);
		vce_encode->output_flip = fopen((char *)(output_file_name), "wb");
		if (vce_encode->output_flip == NULL) {
			lombo_al_printf("vce_encode->output_flip open error\n");
			goto vce_encode_fail;
		}

		if (vce_encode->handle == NULL)
			vce_encode->handle = vce_encode->video_encode_info->open
				(vce_encode->encode_parm, STEAM_BUF_SIZE);
		if (vce_encode->handle == NULL) {
			lombo_al_printf("open error\n");
			goto vce_encode_fail;
		}

		while (1) {
			venc_capbuf_t cap_buf;
			venc_packet_t frame;

			int ret = 0;
			ret = get_capture_data(vce_encode);
			if (ret != 0) {
				lombo_al_printf("file end\n");
				break;
			}

			lombo_al_memset(&cap_buf, 0, sizeof(venc_capbuf_t));
			cap_buf.vir_addr[0] = vce_encode->capture_buffer_vir;
			cap_buf.phy_addr[0] = vce_encode->capture_buffer_phy;
			if ((vce_encode->input_mode == ENC_YUV420P)
				|| (vce_encode->input_mode == ENC_YVU420P)) {
				cap_buf.vir_addr[1] = vce_encode->capture_buffer_vir +
					vce_encode->offset_u;
				cap_buf.phy_addr[1] = vce_encode->capture_buffer_phy +
					vce_encode->offset_u;
				cap_buf.vir_addr[2] = vce_encode->capture_buffer_vir +
					vce_encode->offset_v;
				cap_buf.phy_addr[2] = vce_encode->capture_buffer_phy +
					vce_encode->offset_v;
			} else if ((vce_encode->input_mode == ENC_YUV420SP)
				|| (vce_encode->input_mode == ENC_YVU420SP)) {
				cap_buf.vir_addr[1] = vce_encode->capture_buffer_vir +
					vce_encode->offset_u;
				cap_buf.phy_addr[1] = vce_encode->capture_buffer_phy +
					vce_encode->offset_u;
				cap_buf.vir_addr[2] = 0;
				cap_buf.phy_addr[2] = 0;
			}

			frame_encode_num++;

			if (frame_encode_num % 4 == 0)
				;

			cap_buf.time_stamp = vce_encode->capture_time;

			start_time = oscl_get_msec();

			ret = vce_encode->video_encode_info->encode_frame(
				vce_encode->handle, &cap_buf);
			if (ret != 0) {
				lombo_al_printf("encode frame error\n");
				goto h264_dispose;
			}

			end_time = oscl_get_msec();
			total_time += (end_time - start_time);
			frame_num++;
			lombo_al_printf(" --- encode : %d %d -- %d\n",
				end_time-start_time, total_time/frame_num, frame_num);

			lombo_al_memset(&frame, 0, sizeof(venc_packet_t));
			ret = vce_encode->video_encode_info->dequeue_buf(
				vce_encode->handle, &frame, 50);
			if (ret == 0) {
				fwrite(frame.vir_addr, 1, frame.size,
					vce_encode->output_flip);
			} else {
				lombo_al_printf("dequeue_buf error\n");
				break;
			}

			ret = vce_encode->video_encode_info->queue_buf(
				vce_encode->handle, &frame);
			if (ret == 0) {
				;
			} else {
				lombo_al_printf("queue_buf error\n");
				break;
			}

		}
h264_dispose:
		vce_encode->video_encode_info->close(vce_encode->handle);

vce_encode_fail:
		__test_unload_symb(vce_encode);

		if (vce_encode->input_flip != NULL) {
			fclose(vce_encode->input_flip);
			vce_encode->input_flip = NULL;
		}
		if (vce_encode->output_flip != NULL) {
			fclose(vce_encode->output_flip);
			vce_encode->output_flip = NULL;
		}

		if (vce_encode->capture_buffer_vir != NULL) {
			vce_test_al_free(vce_encode->capture_buffer_vir, AL_MEM_UC);
			vce_encode->capture_buffer_vir = NULL;
		}

		if (vce_encode->encode_parm != NULL) {
			vce_test_al_free(vce_encode->encode_parm, AL_MEM_VIRT);
			vce_encode->encode_parm = NULL;
		}

		if (vce_encode != NULL) {
			vce_test_al_free(vce_encode, AL_MEM_VIRT);
			vce_encode = NULL;
		}
		lombo_al_printf("end instance : %d -- loop cnt : %d\n", param->index,
			test_loop_cnt);
	}

	pthread_exit(NULL);
	return NULL;

}


int vce_test(int argc, char *argv[])
{
	int cnt = 0;
	int i = 0;
	int all_stop_flag = 0;

	if (argc == 1)
		cnt = 1;
	else if (argc == 2)
		cnt = atoi(argv[1]);
	else {
		lombo_al_printf("not support test instance parm : %d\n", argc);
		return -1;
	}

	lombo_al_printf("vc instance : %d\n", cnt);
	if (cnt > MAX_VC_ENCODE_INSTANCE)
		cnt = MAX_VC_ENCODE_INSTANCE;

	for (i = 0; i < cnt; i++) {
		pthread_attr_t thread_attr;
		char *file_input_name1 = BASE_ROOT"bus_cif.yuv";
		char *file_input_name2 = BASE_ROOT"bus_cif.yuv";
		test_param[i] = vce_test_al_malloc(sizeof(vce_test_param_t),
			AL_MEM_VIRT, NULL, __FILE__, __LINE__);
		if (test_param[i] == NULL) {
			lombo_al_printf("alloc test param error\n");
			break;
		}
		memset(test_param[i], 0, sizeof(vce_test_param_t));

		test_param[i]->stop_flag = 0;
		test_param[i]->thread_status = THREAD_INIT;
		test_param[i]->width_in = 352;
		test_param[i]->height_in = 288;
		test_param[i]->l_stride = 352;
		test_param[i]->width_out = 640;
		test_param[i]->height_out = 480;
		test_param[i]->bitrate = 2000000;
		test_param[i]->input_mode = ENC_YUV420P;
		test_param[i]->index = i;
		if (i == 0)
			memcpy(test_param[i]->input_file_name, file_input_name1,
				strlen(file_input_name1) + 1);
		if (i == 1)
			memcpy(test_param[i]->input_file_name, file_input_name2,
				strlen(file_input_name2) + 1);

		lombo_al_printf("start vc thread\n");

		pthread_attr_init(&thread_attr);
		pthread_attr_setstacksize(&thread_attr, 0x2000);

		pthread_create(&test_param[i]->auto_thread, &thread_attr,
			vc_encode_thread, test_param[i]);
	}

	while (1) {
		all_stop_flag = 1;
		for (i = 0; i < cnt; i++) {
			if (test_param[i]->thread_status == THREAD_WAIT_JOIN_NULL) {
				pthread_join(test_param[i]->auto_thread, NULL);
				test_param[i]->auto_thread = (pthread_t)-1;
				test_param[i]->thread_status = THREAD_EXIT;
			}
			if (test_param[i]->thread_status == THREAD_INIT)
				all_stop_flag = 0;
		}
		if (all_stop_flag == 1)
			break;
		else
			lombo_al_task_delay(1000);

	}

	for (i = 0; i < cnt; i++) {
		if (test_param[i] != NULL) {
			vce_test_al_free(test_param[i], AL_MEM_VIRT);
			test_param[i] = NULL;
		}
	}
	return 0;
}
MSH_CMD_EXPORT(vce_test, "vce_test");

