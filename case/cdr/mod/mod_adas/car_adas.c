/*
 * car_adas.c - the Auxiliary Driving System of car
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
#include <finsh.h>
#include <rtthread.h>
#include <dfs_posix.h>

#include <string.h>
#include "lombo_disp.h"
#include "disp_list.h"

#include "car_adas_lib.h"
#include "car_adas.h"
#include "car_adas_set.h"
#include "frame_queue.h"
#include "car_mq.h"

/* #define STATICS_FREE_A	1 */
/* #define STATICS_FREE_B	1 */

#ifdef STATICS_FREE_A
static rt_tick_t a_last_time = 0L;
static rt_tick_t a_curr_time = 0L;
static rt_tick_t a_total_time = 0L;
static int a_total_cnt;
#endif

#ifdef STATICS_FREE_B
static rt_tick_t b_last_time = 0L;
static rt_tick_t b_curr_time = 0L;
static rt_tick_t b_total_time = 0L;
static int b_total_cnt;
#endif


/* static rt_thread_t get_data_proc = RT_NULL; */
static pthread_t get_data_proc;
static int car_adas_status;
static pthread_t set_data_proc;
static al_frame_t *frame;

char frm_name[32];

mm_frame_t mm_frame[QUEUE_NUM];
queue_frame_t *queue_free;
queue_frame_t *queue_full;
void *media_hd;
car_msg_t car_buffer;
adas_screen_info_t adas_screen;

/**
* update_frame - update the data sending to algorithm database
* @mm_frame_t: address of vedio data buffer
*
* This function use to update the data sending to algorithm database
*
* Returns -1 if called when get error ; otherwise, return 0
*/
int update_frame(mm_frame_t *mm_frame)
{
	int ret = -1;

	RT_ASSERT(mm_frame);
	RT_ASSERT(mm_frame->multi_yP);
	frame_set_data.imgYuv[MAJOR_CAM].yP = mm_frame->multi_yP;
	RT_ASSERT(mm_frame->multi_uvP);
	frame_set_data.imgYuv[MAJOR_CAM].uvP = mm_frame->multi_uvP;
	adas_get_gps_info(&frame_set_data);
#ifdef CAPTURE_SOURECE
#ifdef SAVE_FRM
	int i;
	FILE *fp = NULL, *fp_out = NULL;
	for (i = 0; i < 10000; i++) {
		sprintf(frm_name, "%s%d.yuv", "/720p", i);
		fp = fopen(frm_name, "rb");
		if (fp)
			fclose(fp);
		else
			break;
	}
	MOD_LOG_D("frm_name:%s\n", frm_name);
	fp_out = fopen(frm_name, "wb");
	if (fp_out == NULL)
		MOD_LOG_D("fopen fp_out error\n");
	fwrite(frame_set_data.imgYuv[MAJOR_CAM].yP, 1, frame->info.video.size[0], fp_out);
	fwrite(frame_set_data.imgYuv[MAJOR_CAM].uvP, 1, frame->info.video.size[1],
		fp_out);
	if (fp_out != NULL) {
		fclose(fp_out);
		fp_out = NULL;
	}
#endif
#endif
	ret = 0;
	return ret;
}

int set_buf_free(void *multi_frame)
{
	int ret = 0;

#ifdef STATICS_FREE_A
	if (a_curr_time == 0) {
		a_curr_time = rt_time_get_msec();
		a_last_time = rt_time_get_msec();
	} else {
		a_curr_time = rt_time_get_msec();
		a_total_time = a_curr_time - a_last_time;
	}

	a_total_cnt++;

	if (a_total_cnt == 30) {
		MOD_LOG_D("type a free use:(%dms)/(%d frame)\n",
			a_total_time, a_total_cnt);
		a_curr_time = 0;
		a_last_time = 0;
		a_total_time = 0;
		a_total_cnt = 0;
	}
#endif

#ifdef STATICS_FREE_B
	if (b_curr_time == 0) {
		b_curr_time = rt_time_get_msec();
		b_last_time = rt_time_get_msec();
	} else {
		b_curr_time = rt_time_get_msec();
		b_total_time = b_curr_time - b_last_time;
	}

	b_total_cnt++;

	if (b_total_time >= 1000) {
		MOD_LOG_D("type b free use:(%d frame)/(%dms)\n",
			b_total_cnt, b_total_time);
		b_curr_time = 0;
		b_last_time = 0;
		b_total_time = 0;
		b_total_cnt = 0;
	}
#endif


#ifdef CAPTURE_SOURECE
	RT_ASSERT(multi_frame);
	ret = lb_recorder_ctrl(media_hd, LB_REC_FREE_FRAME, (void *)multi_frame);
#endif
	return ret;
}
static int buf_handle(void *eplayer, al_frame_t *frame)
{
	int ret = -1;

#if 0
	int frmid = 0;

	RT_ASSERT(sem_adas_mutex);
	rt_sem_take(sem_adas_mutex, RT_WAITING_FOREVER);

	/* get a frame from local free queue */
	frmid = queue_delete(queue_free, __FILE__, __LINE__);
	if (frmid != -1) {
		mm_frame[frmid].multi_yP = frame->info.video.addr[0];
		mm_frame[frmid].multi_uvP = frame->info.video.addr[1];
		mm_frame[frmid].multi_frame = frame;
		/* memcpy(mm_frame[frmid].multi_frame,frame,sizeof(al_frame_t)); */
		MOD_LOG_D("frmid:%d multi_frame:%x\n", frmid, frame);
		/* give it to adas to calculate objects */
		ret = update_frame(&mm_frame[frmid]);
		RT_ASSERT(ret == 0);

		/* set a frame to local full queue */
		ret = queue_insert(queue_full, frmid, __FILE__, __LINE__);
		RT_ASSERT(ret == 0);

		RT_ASSERT(sem_adas_mutex);

		RT_ASSERT(sem_adas_data);
		rt_sem_release(sem_adas_data);
		return ret;
	}

	rt_sem_release(sem_adas_mutex);

	/* back the frame to the multimedia */
	ret = set_buf_free((void *)frame);
	RT_ASSERT(ret == 0);
	RT_ASSERT(sem_adas_mutex);

	MOD_LOG_D("free end\n");
#else
	if (!get_data_proc_thread_exit) {
		ret = car_mq_send(LB_CAR_FRONT_BUFFER, &frame, sizeof(al_frame_t *),
			ASYNC_FLAG);
		if (ret < 0) {
			ret = set_buf_free((void *)frame);
			RT_ASSERT(ret == 0);
		}
	} else {
		ret = set_buf_free((void *)frame);
		RT_ASSERT(ret == 0);
	}
#endif

	return 0;
}
int set_media_buf_cb(void *recorder_hd, int flag)
{
	int ret = -1;
	void *para = NULL;
	app_frame_cb_t cb;

	cb.app_data = recorder_hd;
	if (flag == 0)
		cb.buf_handle = buf_handle;
	else
		cb.buf_handle = NULL;
	cb.type = AL_VIDEO_RAW_FRAME;
	para = &cb;
	ret = lb_recorder_ctrl(recorder_hd, LB_REC_SET_CB_FRAME, (void *)para);
	MOD_LOG_D("ret:%d\n", ret);

	return ret;
}

/**
 * get_buf_handle - get the data from queue
 * @multi_yP: address of vedio Y data buffer
 * @multi_uvP: address of vedio uv data buffer
 * @multi_frame: address of vedio data buffer
 *
 * This function use to get the data from queue
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
int get_buf_handle(void *multi_yP, void *multi_uvP, void *multi_frame)
{
	int ret = -1;
	int frmid = 0;

	RT_ASSERT(sem_adas_mutex);
	rt_sem_take(sem_adas_mutex, RT_WAITING_FOREVER);

	/* get a frame from local free queue */
	frmid = queue_delete(queue_free, __FILE__, __LINE__);
	if (frmid != -1) {
		mm_frame[frmid].multi_yP = multi_yP;
		mm_frame[frmid].multi_uvP = multi_uvP;
		mm_frame[frmid].multi_frame = multi_frame;

		/* give it to adas to calculate objects */
		ret = update_frame(&mm_frame[frmid]);
		RT_ASSERT(ret == 0);

		/* set a frame to local full queue */
		ret = queue_insert(queue_full, frmid, __FILE__, __LINE__);
		RT_ASSERT(ret == 0);

		RT_ASSERT(sem_adas_mutex);
		rt_sem_release(sem_adas_mutex);

		RT_ASSERT(sem_adas_data);
		rt_sem_release(sem_adas_data);
		return ret;
	}

	RT_ASSERT(sem_adas_mutex);
	rt_sem_release(sem_adas_mutex);

	/* back the frame to the multimedia */
	ret = set_buf_free(multi_frame);
	RT_ASSERT(ret == 0);

	return ret;
}

int get_buf_handle1()
{
	int ret = -1;
	int frmid = 0;
	al_frame_t **frame_temp = NULL;

	ret = car_mq_recv(&car_buffer, 200, ASYNC_FLAG);
	if (ret < 0)
		return 0;
	if (car_buffer.type == LB_CAR_FRONT_BUFFER) {
		frame_temp = (al_frame_t **)car_buffer.buf;
		frame = *frame_temp;
	}
	if (frame == NULL)
		return 0;

	RT_ASSERT(sem_adas_mutex);
	rt_sem_take(sem_adas_mutex, RT_WAITING_FOREVER);
	/* get a frame from local free queue */
	frmid = queue_delete(queue_free, __FILE__, __LINE__);
	if (frmid != -1) {
		mm_frame[frmid].multi_yP = frame->info.video.addr[0];
		mm_frame[frmid].multi_uvP = frame->info.video.addr[1];
		mm_frame[frmid].multi_frame = frame;
		/* give it to adas to calculate objects */
		ret = update_frame(&mm_frame[frmid]);
		RT_ASSERT(ret == 0);

		/* set a frame to local full queue */
		ret = queue_insert(queue_full, frmid, __FILE__, __LINE__);
		RT_ASSERT(ret == 0);
		RT_ASSERT(sem_adas_mutex);
		rt_sem_release(sem_adas_mutex);
		RT_ASSERT(sem_adas_data);
		rt_sem_release(sem_adas_data);
		return ret;
	}

	RT_ASSERT(sem_adas_mutex);
	rt_sem_release(sem_adas_mutex);

	/* back the frame to the multimedia */
	ret = set_buf_free(frame);
	RT_ASSERT(ret == 0);

	return ret;
}

/**
 * frame_set_data_init - initial the image parameter
 * @set_data: address of vedio Y data buffer
 *
 * This function use to initial the image parameter
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
int frame_set_data_init(HDFrameSetData *set_data)
{
	int ret = -1;

	memset(set_data, 0x00, sizeof(HDFrameSetData));
	ret = adas_get_time_stamp(set_data);
	RT_ASSERT(ret == 0);
	ret = adas_get_day_or_night(set_data);
	RT_ASSERT(ret == 0);
	ret = adas_set_cpugear(set_data);
	RT_ASSERT(ret == 0);
	ret = adas_get_vedio_image_info(set_data);
	RT_ASSERT(ret == 0);
	ret = adas_get_gps_info(set_data);
	RT_ASSERT(ret == 0);
	ret = adas_get_gsensor_info(set_data);
	RT_ASSERT(ret == 0);
	ret = adas_get_obd_para(set_data);
	RT_ASSERT(ret == 0);
	ret = adas_set_warn_sensity(set_data);
	RT_ASSERT(ret == 0);
	ret = adas_get_run_ped(set_data);
	RT_ASSERT(ret == 0);

	return ret;
}

int check_cali_exist(void)
{
	int ret = -1;

	/* Read the data saved in flash last calibration */
	/* need to fill */

	return ret;
}

int init_set_data_init(HDIniSetData *init_set_data)
{
	int ret = -1;

	memset(init_set_data, 0x00, sizeof(HDIniSetData));
	ret = adas_module_enable(init_set_data);
	RT_ASSERT(ret == 0);
	ret = adas_set_carpara(init_set_data);
	RT_ASSERT(ret == 0);

	if (check_cali_exist() == 0) {
		/* paremeters should be derived from calibration */
		ret = adas_set_roi_para(init_set_data);
		RT_ASSERT(ret == 0);
		/* paremeters should be derived from calibration */
		ret = adas_set_cam_out_para(init_set_data);
		RT_ASSERT(ret == 0);
	} else {
		/* start to calibrate */
		adas_create_calibration();
		/* paremeters should be derived from calibration */
		ret = adas_set_roi_para(init_set_data);
		RT_ASSERT(ret == 0);
		/* paremeters should be derived from calibration */
		ret = adas_set_cam_out_para(init_set_data);
		RT_ASSERT(ret == 0);
		/* finish to calibrate */
		adas_delete_calibration();
	}

	ret = adas_set_cam_in_para(init_set_data);
	RT_ASSERT(ret == 0);
	ret = adas_set_ped_para(init_set_data);
	RT_ASSERT(ret == 0);

	return ret;
}

void *_get_data_proc(void *parameter)
{
	int frmid = -1;
	int ret = -1;
	void *multi_frame = NULL;

	while (1) {
		RT_ASSERT(sem_adas_proc);
		ret = rt_sem_take(sem_adas_proc, 100);
		if (-RT_ETIMEOUT == ret) {
			MOD_LOG_W("rt_sem_take timeout\n");
			if (get_data_proc_thread_exit) {
				#ifdef CAPTURE_SOURECE
				ret = set_media_buf_cb(media_hd, 1);
				if (ret != 0)
					MOD_LOG_E("failed ret;%d\n", ret);
				#endif
				MOD_LOG_D("pthread_exit\n");
				break;
			} else {
				continue;
			}
		}
		RT_ASSERT(sem_adas_mutex);
		rt_sem_take(sem_adas_mutex, RT_WAITING_FOREVER);

		/* get a frame from local full queue */
		frmid = queue_delete(queue_full, __FILE__, __LINE__);
		if (frmid != -1) {
			multi_frame = mm_frame[frmid].multi_frame;
			RT_ASSERT(multi_frame);
			/* back the frame to the multimedia */
			ret = set_buf_free(multi_frame);
			RT_ASSERT(ret == 0);
			mm_frame[frmid].multi_frame = NULL;
			mm_frame[frmid].multi_yP = NULL;
			mm_frame[frmid].multi_uvP = NULL;
			/* adas draw objects */
			if (adas_result_cb)
				ret = adas_result_cb->adas_result(&frame_get_data);

			/* set a frame from local free queue */
			ret = queue_insert(queue_free, frmid, __FILE__, __LINE__);
			RT_ASSERT(ret == 0);
		}

		RT_ASSERT(sem_adas_mutex);
		rt_sem_release(sem_adas_mutex);
		if (get_data_proc_thread_exit) {
#ifdef CAPTURE_SOURECE
			ret = set_media_buf_cb(media_hd, 1);
			if (ret != 0)
				MOD_LOG_E("failed ret:%d\n", ret);
#endif
			MOD_LOG_D("pthread_exit\n");
			break;
		}
	}
	return NULL;
}

int get_data_proc_init(void)
{
	int ret = -1;

	pthread_attr_t tmp_attr;
	struct sched_param shed_param;

	ret = pthread_attr_init(&tmp_attr);
	if (ret != 0) {
		MOD_LOG_E("attr_init failed\n");
		return -1;
	}

	ret = pthread_attr_setscope(&tmp_attr, PTHREAD_SCOPE_SYSTEM);
	if (ret != 0) {
		MOD_LOG_E("setscope failed\n");
		goto exit;
	}

	ret = pthread_attr_getschedparam(&tmp_attr, &shed_param);
	if (ret != 0) {
		MOD_LOG_E("getschedparam failed\n");
		goto exit;
	}

	shed_param.sched_priority = 22;
	ret = pthread_attr_setschedparam(&tmp_attr, &shed_param);
	if (ret != 0) {
		MOD_LOG_E("setschedparam failed\n");
		goto exit;
	}

	ret = pthread_attr_setstacksize(&tmp_attr, (size_t)4096);
	if (ret != 0) {
		MOD_LOG_E("setstacksize failed\n");
		goto exit;
	}
	ret = pthread_create(&get_data_proc, &tmp_attr, &_get_data_proc, NULL);
	if (ret != 0) {
		MOD_LOG_E("pthread_create failed\n");
		goto exit;
	}
	rt_thread_delay(5);
exit:
	pthread_attr_destroy(&tmp_attr);

	return ret;
}

int get_data_proc_exit(void)
{
	get_data_proc_thread_exit = 1;

	return 0;
}

void *_set_data_proc(void *parameter)
{
	int ret = -1;

	while (1) {
		/* this is the callback function which from multimedia */
#ifdef CAPTURE_SOURECE
		ret = get_buf_handle1();
		RT_ASSERT(ret == 0);

#else
		ret = get_buf_handle((void *)vedio_y,
				(void *)vedio_uv,
				(void *)0xffdd0000);
		RT_ASSERT(ret == 0);
#endif
		rt_thread_delay(1);

		if (get_data_proc_thread_exit) {
			MOD_LOG_D("pthread_exit\n");
			break;
		}
	}

	return NULL;
}

int set_data_proc_init(void)
{
	int ret = -1;

	pthread_attr_t tmp_attr;
	struct sched_param shed_param;

	ret = pthread_attr_init(&tmp_attr);
	if (ret != 0) {
		MOD_LOG_E("attr_init failed\n");
		return -1;
	}

	ret = pthread_attr_setscope(&tmp_attr, PTHREAD_SCOPE_SYSTEM);
	if (ret != 0) {
		MOD_LOG_E("setscope failed\n");
		goto exit;
	}

	ret = pthread_attr_getschedparam(&tmp_attr, &shed_param);
	if (ret != 0) {
		MOD_LOG_E("getschedparam failed\n");
		goto exit;
	}

	shed_param.sched_priority = 22;
	ret = pthread_attr_setschedparam(&tmp_attr, &shed_param);
	if (ret != 0) {
		MOD_LOG_E("setschedparam failed\n");
		goto exit;
	}

	ret = pthread_attr_setstacksize(&tmp_attr, (size_t)4096);
	if (ret != 0) {
		MOD_LOG_E("setstacksize failed\n");
		goto exit;
	}
	ret = pthread_create(&set_data_proc, &tmp_attr, &_set_data_proc, NULL);
	if (ret != 0) {
		MOD_LOG_E("pthread_create failed\n");
		goto exit;
	}
	rt_thread_delay(5);
exit:
	pthread_attr_destroy(&tmp_attr);

	return ret;
}

int set_data_proc_exit(void)
{
	set_data_proc_thread_exit = 1;

	return 0;
}

int queue_init(void)
{
	int ret = -1;
	int i = 0;

	queue_free = queue_create(QUEUE_NUM + 1, QUEUE_FREE);
	queue_full = queue_create(QUEUE_NUM + 1, QUEUE_FULL);

	for (i = 0; i < QUEUE_NUM; i++) {
		mm_frame[i].multi_yP = NULL;
		mm_frame[i].multi_uvP = NULL;
		mm_frame[i].multi_frame = NULL;
		queue_insert(queue_free, i, __FILE__, __LINE__);
	}

	ret = 0;
	return ret;
}

int queue_exit(void)
{
	int ret = -1;

	if (queue_free) {
		queue_destory(queue_free);
		queue_free = NULL;
	}

	if (queue_full) {
		queue_destory(queue_full);
		queue_full = NULL;
	}
	ret = 0;

	return ret;
}

/**
 * adas_init - initial adas
 *
 * This function use to initial adas
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
int adas_init(void)
{
	int ret = -1;

	MOD_LOG_D("\n");
	/* initial the "set data proc" to get the frame from multimedia */
	ret = set_data_proc_init();
	if (0 != ret) {
		MOD_LOG_E("failed ret:%d\n", ret);
		return ret;
	}
	rt_thread_delay(5);
	MOD_LOG_D("\n");
	ret = frame_set_data_init(&frame_set_data);
	MOD_LOG_D("\n");
	if (0 != ret) {
		MOD_LOG_E("failed ret:%d\n", ret);
		return ret;
	}

	MOD_LOG_D("\n");
	ret = init_set_data_init(&init_set_data);
	if (0 != ret) {
		MOD_LOG_E("failed ret:%d\n", ret);
		return ret;
	}

	MOD_LOG_D("\n");
	/* fill the two callback function first and run the "create adas" */
	ret = adas_lib_init(&init_set_data);
	if (0 != ret) {
		MOD_LOG_E("failed ret:%d\n", ret);
		return ret;
	}

	MOD_LOG_D("\n");
	ret = get_data_proc_init();
	if (0 != ret) {
		MOD_LOG_E("failed ret:%d\n", ret);
		return ret;
	}
	MOD_LOG_D("\n");
	return ret;
}

/**
 * adas_init - exit adas
 *
 * This function use to exit adas
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
int adas_exit(void)
{
	int ret  = -1;

	/* exit the "get data proc" */
	ret = get_data_proc_exit();
	if (0 != ret) {
		MOD_LOG_E("failed ret:%d\n", ret);
		return ret;
	}
	MOD_LOG_D("\n");
	rt_thread_delay(5);
	/* exit the "init set data" */
	ret = adas_lib_exit();
	if (0 != ret) {
		MOD_LOG_E("failed ret:%d\n", ret);
		return ret;
	}
	MOD_LOG_D("\n");
	/* exit the "set data proc" */
	rt_thread_delay(5);
	ret = set_data_proc_exit();
	if (0 != ret) {
		MOD_LOG_E("failed ret:%d\n", ret);
		return ret;
	}
	pthread_join(set_data_proc, NULL);
	MOD_LOG_D("\n");

	ret = 0;
	return ret;
}

int adas_create(void *recorder_hd)
{
	int ret = -1;
	ret = adas_para_init();
	if (ret < 0) {
		MOD_LOG_E("err cr_cfg_get_para fail\n");
		return -1;
	}
	adas_get_screen_info(&adas_screen);
	media_hd = recorder_hd;
	MOD_LOG_D("\n");
	ret = queue_init();
	if (ret != 0) {
		MOD_LOG_E("failed ret:%d\n", ret);
		return ret;
	}

	MOD_LOG_D("\n");
	ret = adas_sem_init();
	if (0 != ret) {
		MOD_LOG_E("failed ret:%d\n", ret);
		return ret;
	}
#ifdef CAPTURE_SOURECE
	ret = car_mq_create(ASYNC_FLAG);
	if (0 != ret) {
		MOD_LOG_E("failed ret:%d\n", ret);
		return ret;
	}
#endif
#ifdef CAPTURE_SOURECE
	ret = set_media_buf_cb(recorder_hd, 0);
	if (ret != 0) {
		MOD_LOG_E("failed ret:%d\n", ret);
		return ret;
	}
	MOD_LOG_D("\n");
#endif
	ret = adas_init();
	if (ret != 0) {
		MOD_LOG_E("failed ret:%d\n", ret);
		return ret;
	}

	return ret;
}

int adas_delete(void *recorder_hd)
{
	int ret  = -1;
	int frmid = -1;
	int i = -1;
	void *multi_frame = NULL;
	al_frame_t **frame_temp = NULL;

	ret = adas_exit();
	if (ret != 0) {
		MOD_LOG_E("failed ret:%d\n", ret);
		return ret;
	}

	MOD_LOG_D("\n");
#ifdef CAPTURE_SOURECE

	while (car_mq_recv(&car_buffer, 10, ASYNC_FLAG) == 0) {
		MOD_LOG_D("\n");
		if (car_buffer.type == LB_CAR_FRONT_BUFFER) {
			frame_temp = (al_frame_t **)car_buffer.buf;
			frame = *frame_temp;
			MOD_LOG_D("frame:%p\n", frame);
			ret = set_buf_free(frame);
		}
	}

	for (i = 0; i < queue_full->size; i++) {
		MOD_LOG_D("\n");
		frmid = queue_delete(queue_full, __FILE__, __LINE__);
		MOD_LOG_D("\n");
		if (frmid != -1) {
			MOD_LOG_D("\n");
			multi_frame = mm_frame[frmid].multi_frame;
			if (multi_frame) {
				MOD_LOG_D("multi_frame:%p\n", multi_frame);
				ret = set_buf_free(multi_frame);
			}
		}
	}

	for (i = 0; i < queue_free->size; i++) {
		MOD_LOG_D("\n");
		frmid = queue_delete(queue_free, __FILE__, __LINE__);
		MOD_LOG_D("\n");
		if (frmid != -1) {
			MOD_LOG_D("\n");
			multi_frame = mm_frame[frmid].multi_frame;
			if (multi_frame) {
				MOD_LOG_D("multi_frame:%p\n", multi_frame);
				ret = set_buf_free(multi_frame);
			}
		}
	}
#endif
	MOD_LOG_D("\n");
	pthread_join(get_data_proc, NULL);
	MOD_LOG_D("\n");

#ifdef CAPTURE_SOURECE
	ret = car_mq_destroy(ASYNC_FLAG);
	if (0 != ret) {
		MOD_LOG_E("failed ret:%d\n", ret);
		return ret;
	}
#endif
	MOD_LOG_D("\n");
	ret = adas_sem_exit();
	if (0 != ret) {
		MOD_LOG_E("failed ret:%d\n", ret);
		return ret;
	}
	MOD_LOG_D("\n");
	ret = queue_exit();
	if (ret != 0) {
		MOD_LOG_E("failed ret:%d\n", ret);
		return ret;
	}

	ret = adas_para_exit();
	if (ret < 0) {
		MOD_LOG_E("failed ret:%d\n", ret);
		return ret;
	}
	ret = 0;

	return ret;
}
int car_adas_set_status(int adas_status)
{
	car_adas_status = adas_status;

	return 0;
}
int car_adas_get_status(void)
{
	return car_adas_status;
}

