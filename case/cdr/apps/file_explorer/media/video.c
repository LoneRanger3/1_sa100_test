/*
 * video.c - video code from file explorer
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

#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
#include <app_manage.h>
#include <mod_manage.h>
#include "video.h"
#include <semaphore.h>
#include <pthread.h>
#include "lb_common.h"
#include "lb_ui.h"
#include "lb_gal_common.h"

#include "mars.h"
#include "view_stack.h"
#include "view_node.h"
#include "player.h"
#include "mod_media.h"
#include "fileexp_common.h"
#include "thumb_image.h"
#include "thumb_video.h"
#include "fileexp_cfg.h"
#include <time.h>

static lb_int32 video_imgbtn_prev(void *param);
static lb_int32 video_imgbtn_next(void *param);
static lb_int32 cur_slider_set(lb_int16 *prog_val);
static lb_int32 cur_slider_get(lb_int16 *prog_val);
static lb_int32 cur_slider_update(void);
static lb_int32 cur_time_update(void);
static lb_int32 cur_time_set(char *time);
static lb_int32 cur_time_get(char *time);
static lb_int32 total_time_set(char *time);
static lb_int32 total_time_get(char *time);
static lb_int32 index_set(char *index);
static lb_int32 index_get(char *index);

static lb_int32 video_cur_index;
static lb_int32 video_tot_index;
static void *video_desert;
static pthread_t internal_id;
static pthread_t external_id;
static pthread_t error_id;
static lb_int32 in_need_exit;
static lb_int32 in_need_update;
static lb_int32 ex_exchge_time;
static lb_int32 ex_need_exit;
static lb_int32 error_need_exit;
static lb_int16 prog_val;
static char cur_time[16];
static char tot_time[16];
static char list_index[16];

static lb_int32 thumb_start(void *para)
{
	image_thumb_start(para);
	video_thumb_start(para);

	return 0;
}

static lb_int32 thumb_stop(void *para)
{
	image_thumb_stop(para);
	video_thumb_stop(para);

	return 0;
}

static void *error_refr(void *parameter)
{
	lb_ui_send_msg(LB_MSG_FILEEXP_VIDEO_ERROR, NULL, 0, 0);
	rt_thread_delay(200);
	while (error_need_exit != 1)
		rt_thread_delay(5);

	pthread_exit(0);
	return NULL;
}

static lb_int32 error_init(void)
{
	pthread_attr_t tmp_attr;
	struct sched_param shed_param;
	lb_int32 ret = 0;

	if ((void *)error_id) {
		APP_LOG_W("failed\n");
		ret = 0;
		goto exit;
	}

	error_need_exit = 0;

	ret = pthread_attr_init(&tmp_attr);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = pthread_attr_setscope(&tmp_attr, PTHREAD_SCOPE_SYSTEM);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = pthread_attr_getschedparam(&tmp_attr, &shed_param);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	shed_param.sched_priority = ERROR_PRIO;
	ret = pthread_attr_setschedparam(&tmp_attr, &shed_param);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = pthread_attr_setstacksize(&tmp_attr, (size_t)ERROR_SIZE);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = pthread_create(&error_id, &tmp_attr, &error_refr, NULL);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	pthread_attr_destroy(&tmp_attr);

exit:
	return ret;
}

static lb_int32 error_exit(void)
{
	lb_int32 ret = 0;

	if ((void *)error_id) {
		error_need_exit = 1;
		pthread_join(error_id, NULL);
		error_id = (pthread_t)NULL;
	}

	return ret;
}

static void *internel_refr(void *parameter)
{

	while (in_need_exit != 1) {
		if (in_need_update) {
			cur_time_update();
			cur_slider_update();
		}

		rt_thread_delay(1);
	}

	pthread_exit(0);
	return NULL;
}

static lb_int32 internel_init(void)
{
	pthread_attr_t tmp_attr;
	struct sched_param shed_param;
	lb_int32 ret = 0;

	if ((void *)internal_id) {
		APP_LOG_W("failed\n");
		ret = 0;
		goto exit;
	}

	in_need_exit = 0;
	in_need_update = 1;

	ret = pthread_attr_init(&tmp_attr);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = pthread_attr_setscope(&tmp_attr, PTHREAD_SCOPE_SYSTEM);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = pthread_attr_getschedparam(&tmp_attr, &shed_param);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	shed_param.sched_priority = INTERNEL_PRIO;
	ret = pthread_attr_setschedparam(&tmp_attr, &shed_param);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = pthread_attr_setstacksize(&tmp_attr, (size_t)INTERNEL_SIZE);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = pthread_create(&internal_id, &tmp_attr, &internel_refr, NULL);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	pthread_attr_destroy(&tmp_attr);

exit:
	return ret;
}

static lb_int32 internel_exit(void)
{
	lb_int32 ret = 0;

	if ((void *)internal_id) {
		in_need_exit = 1;
		pthread_join(internal_id, NULL);
		internal_id = (pthread_t)NULL;
	}

	return ret;
}

static lb_int32 externel_get_path(char **path)
{
	lb_int32 ret = 0;

	ret = mars_get_node_path(video_desert, video_cur_index, path);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	APP_LOG_W("path:%s,video_cur_index:%d\n", *path, video_cur_index);

exit:
	return ret;
}

static lb_int32 externel_set_mode(lb_int32 mode)
{
	lb_int32 ret = 0;

	if (mode == 0) {
		if (video_cur_index < (video_tot_index - 1))
			video_cur_index++;
		else
			video_cur_index = 0;
	} else if (mode == 1) {
		if (video_cur_index > 0)
			video_cur_index--;
		else
			video_cur_index = (video_tot_index - 1);
	}

	return ret;
}

static lb_int32 externel_set_pp_picture(lb_int8 index)
{
	lb_int32 ret = 0;
	lb_imgbtn_t *property;
	void *obj;

	ret = lb_view_get_obj_property_by_id(302, (void *)&property);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = lb_view_get_obj_ext_by_id(302, (void *)&obj);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	lb_gal_update_imgbtn(property, obj,
		LB_IMGBTN_UPD_SRC, index, NULL);

exit:
	return ret;
}

static void *externel_refr(void *parameter)
{
	lb_int32 ret = 0;
	lb_int32 state = 0;
	lb_int32 mode = 0;
	char *path = NULL;

	while (ex_need_exit != 1) {
		ret = cur_slider_get(&prog_val);
		if (ret != 0) {
			APP_LOG_W("failed\n");
			goto exit0;
		}
		state = mod_media_video_get_state();

		if (prog_val == 0 &&
			state == MEDIA_VIDEO_STATE_IDLE) {

			externel_set_pp_picture(0);

			ret = index_get(list_index);
			if (ret != 0) {
				APP_LOG_W("failed\n");
				goto exit0;
			}

			ret = index_set(list_index);
			if (ret != 0) {
				APP_LOG_W("failed\n");
				goto exit0;
			}

			ret = externel_get_path(&path);
			if (ret != 0) {
				APP_LOG_W("failed\n");
				goto exit0;
			}

			ret = mod_media_video_set_path(path);
			if (ret != 0) {
				APP_LOG_W("failed\n");
				goto exit0;
			}

			ret = mod_media_video_play();
			if (ret != 0) {
				APP_LOG_W("failed\n");
				goto exit0;
			}

			ret = mod_media_video_set_win_level(1);
			if (ret != 0) {
				APP_LOG_W("failed\n");
				ret = -1;
				goto exit0;
			}

			ret = total_time_get(tot_time);
			if (ret != 0) {
				APP_LOG_W("failed\n");
				goto exit0;
			}

			ret = total_time_set(tot_time);
			if (ret != 0) {
				APP_LOG_W("failed\n");
				goto exit0;
			}

			ret = internel_init();
			if (ret != 0) {
				APP_LOG_W("failed\n");
				goto exit0;
			}
		} else if (prog_val == 100 &&
			state == MEDIA_VIDEO_STATE_COMPLETED) {

			internel_exit();
			mod_media_video_stop();

			prog_val = 0;
			ret = cur_slider_set(&prog_val);
			if (ret != 0) {
				APP_LOG_W("failed\n");
				goto exit0;
			}

			strcpy(cur_time, "00:00:00");
			ret = cur_time_set(cur_time);
			if (ret != 0) {
				APP_LOG_W("failed\n");
				goto exit0;
			}

			strcpy(tot_time, "00:00:00");
			ret = total_time_set(tot_time);
			if (ret != 0) {
				APP_LOG_W("failed\n");
				goto exit0;
			}

			ret = externel_set_mode(mode);
			if (ret != 0) {
				APP_LOG_W("failed\n");
				goto exit0;
			}
		} else if (state == MEDIA_VIDEO_STATE_IDLE) {
			prog_val = 0;
			ret = cur_slider_set(&prog_val);
			if (ret != 0) {
				APP_LOG_W("failed\n");
				goto exit0;
			}

			strcpy(cur_time, "00:00:00");
			ret = cur_time_set(cur_time);
			if (ret != 0) {
				APP_LOG_W("failed\n");
				goto exit0;
			}

			strcpy(tot_time, "00:00:00");
			ret = total_time_set(tot_time);
			if (ret != 0) {
				APP_LOG_W("failed\n");
				goto exit0;
			}
		} else if (state == MEDIA_VIDEO_STATE_COMPLETED) {
			prog_val = 100;
			ret = cur_slider_set(&prog_val);
			if (ret != 0) {
				APP_LOG_W("failed\n");
				goto exit0;
			}

			strcpy(cur_time, tot_time);
			ret = cur_time_set(cur_time);
			if (ret != 0) {
				APP_LOG_W("failed\n");
				goto exit0;
			}
		} else if (state == MEDIA_VIDEO_STATE_ERROR) {
			ret = -1;
			goto exit0;
		}

		rt_thread_delay(50);
	}

exit0:

	prog_val = 0;
	strcpy(cur_time, "00:00:00");
	strcpy(tot_time, "00:00:00");

	internel_exit();
	mod_media_video_stop();

	cur_slider_set(&prog_val);
	cur_time_set(cur_time);
	total_time_set(tot_time);

	if (ret != 0) {
		error_init();
		error_exit();
	}

	pthread_exit(0);
	return NULL;
}

static lb_int32 externel_init(void)
{
	pthread_attr_t tmp_attr;
	struct sched_param shed_param;
	lb_int32 ret = 0;

	if ((void *)external_id) {
		APP_LOG_W("failed\n");
		ret = 0;
		goto exit;
	}
	ex_need_exit = 0;

	ret = pthread_attr_init(&tmp_attr);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = pthread_attr_setscope(&tmp_attr, PTHREAD_SCOPE_SYSTEM);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = pthread_attr_getschedparam(&tmp_attr, &shed_param);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	shed_param.sched_priority = EXTERNEL_PRIO;
	ret = pthread_attr_setschedparam(&tmp_attr, &shed_param);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = pthread_attr_setstacksize(&tmp_attr, (size_t)EXTERNEL_SIZE);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = pthread_create(&external_id, &tmp_attr, &externel_refr, NULL);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	pthread_attr_destroy(&tmp_attr);

exit:
	return ret;
}

static lb_int32 externel_exit(void)
{
	lb_int32 ret = 0;

	if ((void *)external_id) {
		ex_need_exit = 1;
		pthread_join(external_id, NULL);
		external_id = (pthread_t)NULL;
	}

	return ret;
}

static lb_int32 cur_slider_set(lb_int16 *prog_val)
{
	lb_int32 ret = 0;
	void *obj = NULL;

	ret = lb_view_get_obj_ext_by_id(306, &obj);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	lb_gal_update_slider(obj, LB_SLIDER_UPD_VALUE, prog_val);

exit:
	return ret;
}

static lb_int32 cur_slider_get(lb_int16 *prog_val)
{
	lb_int32 ret = 0;
	void *obj = NULL;

	ret = lb_view_get_obj_ext_by_id(306, &obj);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	lb_gal_get_slider_value(obj, prog_val);

exit:
	return ret;
}

static lb_int32 cur_slider_update(void)
{
	lb_int32 ret = 0;
	lb_int32 state = 0;
	lb_int32 cur_pos = 0;
	lb_int32 end_pos = 0;

	state = mod_media_video_get_state();
	if (state == MEDIA_VIDEO_STATE_STARTED) {
		cur_pos  = mod_media_get_cur_pos();
		if (cur_pos < 0) {
			APP_LOG_W("failed\n");
			ret = -1;
			goto exit;
		}

		end_pos  = mod_media_get_end_pos();
		if (end_pos < 0) {
			APP_LOG_W("failed\n");
			ret = -1;
			goto exit;
		}

		if (end_pos)
			prog_val = (100 * cur_pos) / end_pos;
		else
			prog_val = 0;
		if (in_need_update)
			cur_slider_set(&prog_val);
	}

exit:
	return ret;
}

static lb_int32 cur_time_from_sec(lb_int32 pos, char *time)
{
	lb_int32 ret = 0;
	lb_int32 div = 0;
	lb_int32 rem = 0;
	lb_int32 hour;
	lb_int32 minute;
	lb_int32 second;
	char sec[16];
	char min[16];
	char hou[16];

	div = pos / 3600;
	hour = div;
	rem = pos % 3600;

	div = rem / 60;
	minute = div;
	rem = rem % 60;

	second = rem;
	if (second >= 10)
		sprintf(sec, "%d", second);
	else
		sprintf(sec, "0%d", second);

	if (minute >= 10)
		sprintf(min, "%d", minute);
	else
		sprintf(min, "0%d", minute);

	if (hour >= 10)
		sprintf(hou, "%d", hour);
	else
		sprintf(hou, "0%d", hour);

	time[0] = hou[0];
	time[1] = hou[1];
	time[3] = min[0];
	time[4] = min[1];
	time[6] = sec[0];
	time[7] = sec[1];
	time[8] = '\0';

	return ret;
}

static lb_int32 cur_time_get(char *time)
{
	lb_int32 ret = 0;
	lb_int32 cur_pos = 0;
	lb_int32 state = 0;

	state = mod_media_video_get_state();
	if (state == MEDIA_VIDEO_STATE_STARTED) {
		strcpy(time, "00:00:00");
		cur_pos  = mod_media_get_cur_pos();
		if (cur_pos == -1) {
			APP_LOG_W("failed\n");
			ret = -1;
			goto exit;
		}
		cur_time_from_sec(cur_pos, time);
	}

exit:
	return ret;
}

static lb_int32 cur_time_set(char *time)
{
	lb_int32 ret = 0;
	void *cobj = NULL;

	ret = lb_view_get_obj_ext_by_id(304, &cobj);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	lb_gal_update_label(cobj, LB_LABEL_UPD_TXT, time);

exit:
	return ret;
}

static lb_int32 cur_time_update(void)
{
	lb_int32 ret = 0;
	char old_time[16];

	strcpy(old_time, cur_time);
	ret = cur_time_get(cur_time);
	if (ret == 0) {
		if (strcmp(old_time, cur_time) == 0)
			goto exit;

		ret = cur_time_set(cur_time);
		if (ret != 0) {
			APP_LOG_W("failed\n");
			ret = -1;
			goto exit;
		}
	}

exit:
	return ret;
}

/**
 * cur_time_init - current time label widget init function
 * @param: lb_obj_t object pointer.
 *
 * This function init time label widget property
 *
 * Returns 0
 */
static lb_int32 cur_time_init(void *param)
{
	lb_int32 ret = 0;
	lb_obj_t *lb_obj = NULL;
	lb_label_t *lb_label = NULL;
	char time[16];

	if (param == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}
	lb_obj = (lb_obj_t *)param;

	lb_label = (lb_label_t *)lb_obj->property;
	if (lb_label == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	strcpy(time, "00:00:00");
	lb_label->txt = mars_mem_alloc(16);
	if (lb_label->txt) {
		memset(lb_label->txt, 0x00, 16);
		strcpy(lb_label->txt, time);
	}

exit:
	return ret;
}

/**
 * cur_time_exit - current time label widget init function
 * @param: lb_obj_t object pointer.
 *
 * This function exit time label widget property
 *
 * Returns 0
 */
static lb_int32 cur_time_exit(void *param)
{
	lb_int32 ret = 0;
	lb_obj_t *lb_obj = NULL;
	lb_label_t *lb_label = NULL;

	if (param == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}
	lb_obj = (lb_obj_t *)param;

	lb_label = (lb_label_t *)lb_obj->property;
	if (lb_label == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	if (lb_label->txt) {
		mars_mem_free(lb_label->txt);
		lb_label->txt = NULL;
	}

exit:
	return ret;
}

lb_int32 total_time_get(char *time)
{
	lb_int32 ret = 0;
	lb_int32 end_pos = 0;

	strcpy(time, "00:00:00");

	end_pos  = mod_media_get_end_pos();
	if (end_pos < 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	cur_time_from_sec(end_pos, time);

exit:
	return ret;
}

lb_int32 total_time_set(char *time)
{
	lb_int32 ret = 0;
	void *tobj = NULL;

	ret = lb_view_get_obj_ext_by_id(305, &tobj);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	lb_gal_update_label(tobj, LB_LABEL_UPD_TXT, time);

exit:
	return ret;
}

/**
 * total_time_init - total time label widget init function
 * @param: lb_obj_t object pointer.
 *
 * This function use to init time label widget property
 *
 * Returns 0
 */
static lb_int32 total_time_init(void *param)
{
	lb_int32 ret = 0;
	lb_obj_t *lb_obj = NULL;
	lb_label_t *lb_label = NULL;
	char tot_time[16];

	if (param == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}
	lb_obj = (lb_obj_t *)param;

	lb_label = (lb_label_t *)lb_obj->property;
	if (lb_label == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	strcpy(tot_time, "00:00:00");

	lb_label->txt = mars_mem_alloc(16);
	if (lb_label->txt) {
		memset(lb_label->txt, 0x00, 16);
		strcpy(lb_label->txt, tot_time);
	}

exit:
	return ret;
}

/**
 * total_time_exit - total time label widget init function
 * @param: lb_obj_t object pointer.
 *
 * This function use to exit time label widget property
 *
 * Returns 0
 */
static lb_int32 total_time_exit(void *param)
{
	lb_int32 ret = 0;
	lb_obj_t *lb_obj = NULL;
	lb_label_t *lb_label = NULL;

	if (param == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}
	lb_obj = (lb_obj_t *)param;

	lb_label = (lb_label_t *)lb_obj->property;
	if (lb_label == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	if (lb_label->txt) {
		mars_mem_free(lb_label->txt);
		lb_label->txt = NULL;
	}

exit:
	return ret;
}

/**
 * video_init - call the video_mod_init
 * @param: lb_obj_t object pointer.
 *
 * This function call the image_mod_init
 *
 * Returns 0
 */
lb_int32 video_init(void *param)
{
	lb_int32 ret = 0;
	static v_node_t node0;
	static v_node_t node1;

	node0.init_op = thumb_stop;
	node0.exit_op = video_mod_exit;
	node0.next = &node1;

	node1.init_op = video_mod_init;
	node1.exit_op = thumb_start;
	node1.next = NULL;

	ret = view_stack_push(&node0);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	if (node0.init_op)
		node0.init_op((void *)0);

	if (node1.init_op)
		node1.init_op((void *)0);

exit:
	return ret;
}

/**
 * video_exit - call the nothing
 * @param: lb_obj_t object pointer.
 *
 * This function  call the nothing
 *
 * Returns 0
 */
lb_int32 video_exit(void *param)
{
	lb_int32 ret = 0;

	return ret;
}

static lb_int32 index_get(char *index)
{
	lb_int32 ret = 0;
	char cur_index[16];
	char tot_index[16];

	memset(cur_index, 0x00, sizeof(cur_index));
	memset(tot_index, 0x00, sizeof(tot_index));

	if (index == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	sprintf(cur_index, "%d", video_cur_index + 1);
	sprintf(tot_index, "%d", video_tot_index);

	strcpy(index, cur_index);
	strcat(index, "/");
	strcat(index, tot_index);

exit:
	return ret;
}

static lb_int32 index_set(char *index)
{
	lb_int32 ret = 0;
	void *tobj = NULL;

	if (index == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = lb_view_get_obj_ext_by_id(307, &tobj);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	lb_gal_update_label(tobj, LB_LABEL_UPD_TXT, index);

exit:
	return ret;
}

/**
 * index_init - show the index of list
 * @param: lb_obj_t object pointer.
 *
 * This function show the index of list
 *
 * Returns 0
 */
lb_int32 index_init(void *param)
{
	lb_int32 ret = 0;
	lb_obj_t *lb_obj = NULL;
	lb_label_t *lb_label = NULL;
	char index[16];

	if (param == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}
	lb_obj = (lb_obj_t *)param;

	lb_label = (lb_label_t *)lb_obj->property;
	if (lb_label == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	index_get(index);
	lb_label->txt = mars_mem_alloc(16);
	if (lb_label->txt) {
		memset(lb_label->txt, 0x00, 16);
		strcpy(lb_label->txt, index);
	}

exit:
	return ret;
}

/**
 * index_exit - hide the index of list
 * @param: lb_obj_t object pointer.
 *
 * This function hide the index of list
 *
 * Returns 0
 */
lb_int32 index_exit(void *param)
{
	lb_int32 ret = 0;
	lb_obj_t *lb_obj = NULL;
	lb_label_t *lb_label = NULL;

	if (param == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}
	lb_obj = (lb_obj_t *)param;

	lb_label = (lb_label_t *)lb_obj->property;
	if (lb_label == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	if (lb_label->txt) {
		mars_mem_free(lb_label->txt);
		lb_label->txt = NULL;
	}

exit:
	return ret;

}

/**
 * video_mod_init - initial the mod of video
 * @param: lb_obj_t object pointer.
 *
 * This function initial the mod of video
 *
 * Returns 0
 */
lb_int32 video_mod_init(void *param)
{
	lb_int32 ret = 0;

	close_keytone();
	close_standby();
	ret = mod_media_video_start();
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = externel_init();
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	return ret;

exit:
	externel_exit();
	mod_media_video_end();
	open_standby();
	open_keytone();

	return ret;
}

/**
 * video_mod_exit - exit the mod of video
 * @param: lb_obj_t object pointer.
 *
 * This function exit the mod of video
 *
 * Returns 0
 */
lb_int32 video_mod_exit(void *param)
{
	lb_int32 ret = 0;

	externel_exit();
	mod_media_video_end();
	open_standby();
	open_keytone();

	return ret;
}

/**
 * video_set - set the video list and index
 * @param: lb_obj_t object pointer.
 *
 * This function set the video list and index
 *
 * Returns 0
 */
lb_int32 video_set(void *desert, lb_int32 index)
{
	if (desert != NULL) {
		video_desert = desert;
		video_tot_index = mars_get_node_num(video_desert);
		video_cur_index = index;
	}

	return 0;
}


/**
 * video_imgbtn_prev - response function for previous imgbtn
 * @param: imgbtn object pointer.
 *
 * This function use to response to previous imgbtn, jump to last media file
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
static lb_int32 video_imgbtn_prev(void *param)
{
	lb_int32 ret = 0;
	lb_int32 cur_tick = 0;

	cur_tick = rt_tick_get();

	if (cur_tick - ex_exchge_time <= 200) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}
	ex_exchge_time = cur_tick;


	externel_exit();

	ret = externel_set_mode(1);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = externel_init();
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

exit:
	return ret;
}

/**
 * video_imgbtn_pp - response function for play/puase imgbtn
 * @param:imgbtn object pointer.
 *
 * This function use to response to play/puase imgbtn, switch status between play/puase
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 video_imgbtn_pp(void *param)
{
	lb_int32 state = 0;
	lb_int32 ret = 0;

	state = mod_media_video_get_state();

	if (state == MEDIA_VIDEO_STATE_STARTED) {
		ret = mod_media_video_pause();
		if (ret != 0) {
			APP_LOG_W("failed\n");
			ret = -1;
			goto exit;
		}
		ret = externel_set_pp_picture(1);
	} else if (state == MEDIA_VIDEO_STATE_PAUSED) {
		ret = mod_media_video_play();
		if (ret != 0) {
			APP_LOG_W("failed\n");
			ret = -1;
			goto exit;
		}
		ret = mod_media_video_set_win_level(1);
		if (ret != 0) {
			APP_LOG_W("failed\n");
			ret = -1;
			goto exit;
		}
		ret = externel_set_pp_picture(0);
	}

exit:
	return ret;
}

/**
 * video_imgbtn_next - response function for next imgbtn
 * @param: imgbtn object pointer.
 *
 * This function use to response to back imgbtn, jump to next media file
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
static lb_int32 video_imgbtn_next(void *param)
{
	lb_int32 ret = 0;
	lb_int32 cur_tick = 0;

	cur_tick = rt_tick_get();

	if (cur_tick - ex_exchge_time <= 200) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}
	ex_exchge_time = cur_tick;

	externel_exit();

	ret = externel_set_mode(0);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = externel_init();
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

exit:
	return ret;
}

/**
 * video_slider_move - response function for slider widget
 * @param: imgbtn object pointer.
 *
 * This function use to response to slider widget, when slider prog_val changed,
 * this function will be called
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
static lb_int32 video_slider_move(void *param)
{
	lb_int32 state = 0;
	lb_int32 cur_pos = 0;
	lb_int32 end_pos = 0;
	lb_int16 prog_val = 0;
	lb_int32 ret = 0;
	lb_slider_status_msg_t *status_msgt = NULL;
	void *pobj = NULL;

	status_msgt = (lb_slider_status_msg_t *)param;
	if (status_msgt == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	pobj = status_msgt->pobj;
	if (pobj == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = lb_gal_get_slider_value(pobj, &prog_val);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	state = mod_media_video_get_state();
	if (state != MEDIA_VIDEO_STATE_STARTED &&
		state != MEDIA_VIDEO_STATE_PAUSED) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	if (status_msgt->press_status == 1 && status_msgt->release_status == 0) {
		in_need_update = 0;
		ret = 0;
	} else if (status_msgt->press_status == 0 && status_msgt->release_status == 1) {
		state = mod_media_video_get_state();
		if (state == MEDIA_VIDEO_STATE_STARTED ||
			state == MEDIA_VIDEO_STATE_PAUSED) {
			end_pos  = mod_media_get_end_pos();
			if (end_pos < 0) {
				APP_LOG_W("failed\n");
				ret = -1;
				goto exit;
			}

			cur_pos = (prog_val * end_pos + 99) / 100;
			APP_LOG_W("cur_pos:%d\n", cur_pos);
			if (cur_pos >= 0)
				mod_media_set_cur_pos(cur_pos);
		}

		in_need_update = 1;
		ret = 0;
	}

exit:
	return ret;
}

static lb_int32 video_play_error(void *param)
{
	lb_int32 ret = 0;
	lb_int32 data = 3;

	lb_ui_send_msg(LB_MSG_ENTER_ISOLATE,
		(void *)&data, sizeof(void *), 0);

	return ret;
}

/**
 * video_reg_init -reg init function
 *
 * This function use to register init function
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 video_reg_init(void)
{
	lb_int32	err = 0;

	err |= lb_fmngr_reg_init_func("video_init", video_init);
	err |= lb_fmngr_reg_exit_func("video_exit", video_exit);

	err |= lb_fmngr_reg_init_func("cur_time_init", cur_time_init);
	err |= lb_fmngr_reg_exit_func("cur_time_exit", cur_time_exit);

	err |= lb_fmngr_reg_init_func("total_time_init", total_time_init);
	err |= lb_fmngr_reg_exit_func("total_time_exit", total_time_exit);

	err |= lb_fmngr_reg_init_func("index_init", index_init);
	err |= lb_fmngr_reg_exit_func("index_exit", index_exit);

	return err;
}

/**
 * video_unreg_init - unreg init function for widgets
 *
 * This function use to unregister init function for widgets
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 video_unreg_init(void)
{
	lb_int32	err = 0;

	err |= lb_fmngr_unreg_init_func(cur_time_init);
	err |= lb_fmngr_unreg_exit_func(cur_time_exit);

	err |= lb_fmngr_unreg_init_func(total_time_init);
	err |= lb_fmngr_unreg_exit_func(total_time_exit);

	err |= lb_fmngr_unreg_init_func(video_init);
	err |= lb_fmngr_unreg_exit_func(video_exit);

	err |= lb_fmngr_unreg_init_func(index_init);
	err |= lb_fmngr_unreg_exit_func(index_exit);

	return err;
}

/**
 * video_reg_resp - reg response function for widgets
 *
 * This function use to register response function for widgets
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 video_reg_resp(void)
{
	lb_int32	err = 0;

	err |= lb_reg_resp_msg_func(LB_MSG_FILEEXP_VIDEO_NEXT, video_imgbtn_next);
	err |= lb_reg_resp_msg_func(LB_MSG_FILEEXP_VIDEO_PREV, video_imgbtn_prev);
	err |= lb_reg_resp_msg_func(LB_MSG_FILEEXP_VIDEO_PP, video_imgbtn_pp);
	err |= lb_reg_resp_msg_func(LB_MSG_FILEEXP_VIDEO_SLIDER, video_slider_move);
	err |= lb_reg_resp_msg_func(LB_MSG_FILEEXP_VIDEO_ERROR, video_play_error);

	return err;
}

/**
 * video_unreg_resp - unreg response function for widgets
 *
 * This function use to unregister  response function for widgets
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 video_unreg_resp(void)
{
	lb_int32	err = 0;

	err |= lb_unreg_resp_msg_func(video_imgbtn_prev);
	err |= lb_unreg_resp_msg_func(video_imgbtn_next);
	err |= lb_unreg_resp_msg_func(video_imgbtn_pp);
	err |= lb_unreg_resp_msg_func(video_slider_move);
	err |= lb_unreg_resp_msg_func(video_play_error);

	return err;
}
