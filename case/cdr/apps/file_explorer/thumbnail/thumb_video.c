/*
 * thumb_image.c - thumb video from file explorer
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
#include "thumb_video.h"
#include "thumb_image.h"
#include <pthread.h>
#include "lb_common.h"
#include "lb_gal_common.h"
#include "lb_ui.h"
#include "mars.h"
#include "view_stack.h"
#include "view_node.h"
#include "player.h"
#include "mod_media.h"
#include "fileexp_common.h"
#include <time.h>

static thumb_vid_t thumb_vid;

static lb_int32 thumb_check_para(void)
{
	lb_int32 ret = 0;

	RT_ASSERT(thumb_vid.desert != NULL);
	RT_ASSERT(thumb_vid.path != NULL);
	RT_ASSERT(thumb_vid.thumb != NULL);
	RT_ASSERT(thumb_vid.data != NULL);
	RT_ASSERT(thumb_vid.video != NULL);
	RT_ASSERT(thumb_vid.db_data != NULL);
	RT_ASSERT(thumb_vid.db_video != NULL);

	return ret;
}

static lb_int32 load_from_dbs(lb_int32 i)
{
	lb_int32 ret = 0;

#ifdef DATABASE_OMIT
	ret = 1;
	return ret;
#else

	lb_int32 len = 0;
	lb_int32 file = -1;
	thumb_vid_dsc_t img_dsc;

	thumb_check_para();

	ret = mars_get_node_thumb(thumb_vid.desert, i, &thumb_vid.thumb[i]);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	file = open(thumb_vid.thumb[i], O_RDONLY);
	if (file == -1) {
		APP_LOG_D("No thumb for loading\n");
		ret = 1;
		goto exit;
	}

	thumb_vid.db_video[i] = mars_mem_alloc(sizeof(thumb_vid_dsc_t));
	if (thumb_vid.db_video[i] == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	len = read(file, &img_dsc, sizeof(thumb_vid_dsc_t));
	if (len != sizeof(thumb_vid_dsc_t)) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	thumb_vid.db_data[i] = mars_mem_alloc(img_dsc.data_size);
	if (thumb_vid.db_data[i] == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	len = read(file, thumb_vid.db_data[i], img_dsc.data_size);
	if (len != img_dsc.data_size) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	img_dsc.data = (const uint8_t *)thumb_vid.db_data[i];
	memcpy(thumb_vid.db_video[i], &img_dsc, sizeof(thumb_vid_dsc_t));

	close(file);
	file = -1;

	return ret;

exit:
	if (thumb_vid.db_data[i]) {
		mars_mem_free(thumb_vid.db_data[i]);
		thumb_vid.db_data[i] = NULL;
	}

	if (thumb_vid.db_video[i]) {
		mars_mem_free(thumb_vid.db_video[i]);
		thumb_vid.db_video[i] = NULL;
	}

	if (file != -1) {
		close(file);
		file = -1;
	}

	return ret;
#endif
}

static lb_int32 load_from_dec(lb_int32 i)
{
	lb_int32 ret = 0;
	lb_int32 file = -1;

	thumb_check_para();

	ret = mars_get_node_thumb(thumb_vid.desert, i, &thumb_vid.thumb[i]);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	file = open(thumb_vid.thumb[i], O_RDONLY);
	if (file != -1) {
		/* APP_LOG_W("\n"); */
		ret = -1;
		goto exit;
	}

	ret = mars_get_node_path(thumb_vid.desert, i, &thumb_vid.path[i]);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	thumb_vid.data[i] = mod_media_video_get_thumb(thumb_vid.path[i], 96, 48);
	if (thumb_vid.data[i] == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	thumb_vid.video[i] = (char *)eimage_create_img_buf_thumb(thumb_vid.data[i]);
	if (thumb_vid.video[i] == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	return ret;

exit:
	if (thumb_vid.video[i]) {
		eimage_destory_img_buf_thumb((void *)thumb_vid.video[i]);
		thumb_vid.video[i] = NULL;
	}

	if (thumb_vid.data[i]) {
		mod_media_video_free_thumb(thumb_vid.data[i]);
		thumb_vid.data[i] = NULL;
	}

	if (file != -1) {
		close(file);
		file = -1;
	}

	return ret;
}

static lb_int32 store_to_dbs(lb_int32 i)
{
	lb_int32 ret = 0;

#ifdef DATABASE_OMIT
	ret = 1;
	return ret;
#else

	lb_int32 len = 0;
	lb_int32 file = -1;
	thumb_vid_dsc_t img_dsc;

	thumb_check_para();

	if (thumb_vid.video[i] == NULL) {
		APP_LOG_D("No thumb for storing\n");
		ret = 1;
		goto exit;
	}

	memcpy(&img_dsc, thumb_vid.video[i], sizeof(thumb_vid_dsc_t));

	if (thumb_vid.thumb[i] == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	file = open(thumb_vid.thumb[i], O_RDONLY);
	if (file != -1) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	file = open(thumb_vid.thumb[i], O_WRONLY | O_CREAT);
	if (file == -1) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	len = write(file, &img_dsc, sizeof(thumb_vid_dsc_t));
	if (len != sizeof(thumb_vid_dsc_t)) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	len = write(file, img_dsc.data, img_dsc.data_size);
	if (len != img_dsc.data_size) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	close(file);
	file = -1;

	return ret;
exit:
	if (file != -1) {
		close(file);
		file = -1;
	}
	return ret;
#endif
}

static lb_int32 thumb_update_one(lb_int32 index)
{
	lb_int32 ret = 0;
	lb_flist_t *pproperty = NULL;
	lb_int16 tab = 0;

	/* Find the list by id because params are buttons not the list we need */
	file_get_list(tab, (void **)&pproperty);
	if (NULL == pproperty) {
		APP_LOG_W("failed\n");
		return LB_ERROR_NO_MEM;
	}

	if (thumb_vid.data[index] && thumb_vid.video[index]) {
		pproperty->file_array[index].thumb_data = thumb_vid.video[index];
		pproperty->file_array[index].thumb_used = 1;
		pproperty->file_array[index].decode_flag = 1;
	} else if (thumb_vid.db_data[index] && thumb_vid.db_video[index]) {
		pproperty->file_array[index].thumb_data = thumb_vid.db_video[index];
		pproperty->file_array[index].thumb_used = 1;
		pproperty->file_array[index].databs_flag = 1;
	}

	lb_gal_update_flist(pproperty, LB_FLIST_UPDATE_ONE_THUMB, (void *)index);

	return ret;
}

static lb_int32 thumb_dbs_alloc(lb_int32 i)
{
	lb_int32 ret = 0;

	ret = load_from_dbs(i);

	return ret;
}

static lb_int32 thumb_dbs_free(lb_int32 i)
{
	lb_int32 ret = 0;

	if (thumb_vid.db_video[i]) {
		mars_mem_free(thumb_vid.db_video[i]);
		thumb_vid.db_video[i] = NULL;
	}

	if (thumb_vid.db_data[i]) {
		mars_mem_free(thumb_vid.db_data[i]);
		thumb_vid.db_data[i] = NULL;
	}

	return ret;
}

static lb_int32 thumb_dec_alloc(lb_int32 i)
{
	lb_int32 ret = 0;

	ret = load_from_dec(i);
	if (ret != 0) {
		ret = -1;
		goto exit;
	}

	return ret;
exit:
	return ret;
}

static lb_int32 thumb_dec_free(lb_int32 i)
{
	lb_int32 ret = 0;

	if (thumb_vid.video[i]) {
		eimage_destory_img_buf_thumb((void *)thumb_vid.video[i]);
		thumb_vid.video[i] = NULL;
	}

	if (thumb_vid.data[i]) {
		mod_media_video_free_thumb(thumb_vid.data[i]);
		thumb_vid.data[i] = NULL;
	}

	return ret;
}

static lb_int32 thumb_buff_init(lb_int32 num)
{
	lb_int32 ret = 0;

	thumb_vid.path = mars_mem_alloc(num * sizeof(char *));
	if (thumb_vid.path == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}
	memset(thumb_vid.path, 0x00, num * sizeof(char *));

	thumb_vid.thumb = mars_mem_alloc(num * sizeof(char *));
	if (thumb_vid.thumb == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}
	memset(thumb_vid.thumb, 0x00, num * sizeof(char *));

	thumb_vid.data = mars_mem_alloc(num * sizeof(char *));
	if (thumb_vid.data == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}
	memset(thumb_vid.data, 0x00, num * sizeof(char *));

	thumb_vid.video = mars_mem_alloc(num * sizeof(char *));
	if (thumb_vid.video == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}
	memset(thumb_vid.video, 0x00, num * sizeof(char *));

	thumb_vid.db_data = mars_mem_alloc(num * sizeof(char *));
	if (thumb_vid.db_data == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}
	memset(thumb_vid.db_data, 0x00, num * sizeof(char *));

	thumb_vid.db_video = mars_mem_alloc(num * sizeof(char *));
	if (thumb_vid.db_video == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}
	memset(thumb_vid.db_video, 0x00, num * sizeof(char *));

	return ret;
exit:
	if (thumb_vid.path) {
		mars_mem_free(thumb_vid.path);
		thumb_vid.path = NULL;
	}

	if (thumb_vid.thumb) {
		mars_mem_free(thumb_vid.thumb);
		thumb_vid.thumb = NULL;
	}

	if (thumb_vid.video) {
		mars_mem_free(thumb_vid.video);
		thumb_vid.video = NULL;
	}

	if (thumb_vid.data) {
		mars_mem_free(thumb_vid.data);
		thumb_vid.data = NULL;
	}

	if (thumb_vid.db_video) {
		mars_mem_free(thumb_vid.db_video);
		thumb_vid.db_video = NULL;
	}

	if (thumb_vid.db_data) {
		mars_mem_free(thumb_vid.db_data);
		thumb_vid.db_data = NULL;
	}

	return ret;
}

static lb_int32 thumb_buff_exit(void)
{
	lb_int32 ret = 0;

	if (thumb_vid.path) {
		mars_mem_free(thumb_vid.path);
		thumb_vid.path = NULL;
	}

	if (thumb_vid.thumb) {
		mars_mem_free(thumb_vid.thumb);
		thumb_vid.thumb = NULL;
	}

	if (thumb_vid.video) {
		mars_mem_free(thumb_vid.video);
		thumb_vid.video = NULL;
	}

	if (thumb_vid.data) {
		mars_mem_free(thumb_vid.data);
		thumb_vid.data = NULL;
	}

	if (thumb_vid.db_video) {
		mars_mem_free(thumb_vid.db_video);
		thumb_vid.db_video = NULL;
	}

	if (thumb_vid.db_data) {
		mars_mem_free(thumb_vid.db_data);
		thumb_vid.db_data = NULL;
	}

	return ret;
}

static lb_int32 thumb_core_init(void)
{
	lb_int32 ret = 0;

	ret = sem_init(&thumb_vid.out_sem, 0, 1);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		return -1;
	}

	return ret;
}

static lb_int32 thumb_core_exit(void)
{
	lb_int32 ret = 0;

	ret = sem_destroy(&thumb_vid.out_sem);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		return -1;
	}
	memset(&thumb_vid.out_sem, 0x00, sizeof(sem_t));

	return ret;
}

static void *video_thread_refr(void *parameter)
{
	lb_int32 i = 0;
	lb_int32 num = 0;
	lb_int32 ret = 0;

	APP_LOG_W("ENTER\n");

	rt_thread_delay(1);

	num = mars_get_node_num(thumb_vid.desert);
	if (num <= 0) {
		APP_LOG_W("Exit\n");
		goto exit;
	}
	APP_LOG_W("PIC NUM:%d\n", num);

	ret = thumb_buff_init(num);
	if (ret != 0) {
		APP_LOG_W("Exit\n");
		goto exit;
	}

	for (i = 0; i < num; i++) {
		if (thumb_vid.exit == 1) {
			APP_LOG_W("Exit\n");
			goto exit;
		}

		sem_wait(&thumb_vid.out_sem);
		ret = thumb_dbs_alloc(i);
		sem_post(&thumb_vid.out_sem);

		if (ret == -1) {
			APP_LOG_W("Exit\n");
			goto exit;
		}

		if (ret == 0) {
			APP_LOG_D("Load thumb from db\n");
			rt_thread_delay(1);
		} else if (ret == 1) {
			APP_LOG_D("Load thumb from dec\n");
			sem_wait(&thumb_vid.out_sem);
			ret = thumb_dec_alloc(i);
			sem_post(&thumb_vid.out_sem);
			if (ret != 0) {
				/* APP_LOG_W("\n"); */
				rt_thread_delay(1);
				continue;
			}
		}
		sem_wait(&thumb_vid.out_sem);
		ret = thumb_update_one(i);
		sem_post(&thumb_vid.out_sem);
		if (ret != 0) {
			APP_LOG_W("Exit\n");
			goto exit;
		}
		rt_thread_delay(1);
	}

	for (i = 0; i < num; i++) {
		if (thumb_vid.exit == 1) {
			APP_LOG_W("Exit\n");
			goto exit;
		}

		sem_wait(&thumb_vid.out_sem);
		ret = store_to_dbs(i);
		sem_post(&thumb_vid.out_sem);

		if (ret == -1) {
			APP_LOG_W("Exit\n");
			goto exit;
		}

		rt_thread_delay(1);
	}

	while (thumb_vid.exit != 1)
		rt_thread_delay(1);

exit:
	for (i = 0; i < num; i++) {
		thumb_dec_free(i);
		thumb_dbs_free(i);
	}

	thumb_buff_exit();

	APP_LOG_W("QUIT\n");
	pthread_exit(0);
	return NULL;
}

static lb_int32 video_thread_init(void)
{
	pthread_attr_t tmp_attr;
	struct sched_param shed_param;
	lb_int32 ret = 0;

	ret = pthread_attr_init(&tmp_attr);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		return -1;
	}

	ret = pthread_attr_setscope(&tmp_attr, PTHREAD_SCOPE_SYSTEM);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		return -1;
	}

	ret = pthread_attr_getschedparam(&tmp_attr, &shed_param);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		return -1;
	}

	shed_param.sched_priority = THUMBNAIL_PRIO;
	ret = pthread_attr_setschedparam(&tmp_attr, &shed_param);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		return -1;
	}

	ret = pthread_attr_setstacksize(&tmp_attr, (size_t)THUMBNAIL_SIZE);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		return -1;
	}

	ret = pthread_create(&thumb_vid.id, &tmp_attr, &video_thread_refr, NULL);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		return -1;
	}

	pthread_attr_destroy(&tmp_attr);

	thumb_vid.exit = 0;

	return ret;
}

static lb_int32 video_thread_exit(void)
{
	lb_int32 ret = 0;

	thumb_vid.exit = 1;
	ret = pthread_join(thumb_vid.id, NULL);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		return -1;
	}
	thumb_vid.id = 0;

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
lb_int32 video_thumb_init(void *para)
{
	lb_int32 ret = 0;

	if (thumb_vid.id == 0) {
		ret = thumb_core_init();
		if (ret != 0) {
			APP_LOG_W("failed\n");
			return ret;
		}

		ret = video_thread_init();
		if (ret != 0) {
			APP_LOG_W("failed\n");
			return ret;
		}
	}

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
lb_int32 video_thumb_exit(void *para)
{
	lb_int32 ret = 0;

	APP_LOG_W("video_thumb_exit begin\n");

	if (thumb_vid.id != 0) {
		ret = video_thread_exit();
		if (ret != 0) {
			APP_LOG_W("failed\n");
			return ret;
		}

		ret = thumb_core_exit();
		if (ret != 0) {
			APP_LOG_W("failed\n");
			return ret;
		}
	}

	APP_LOG_W("video_thumb_exit end\n");

	return ret;
}

lb_int32 video_thumb_start(void *para)
{
	lb_int32 ret = 0;

	if (thumb_vid.out_sem.sem) {
		APP_LOG_D("thumb_vid.out_sem.sem:%p\n", thumb_vid.out_sem.sem);
		sem_post(&thumb_vid.out_sem);
	}

	return ret;
}

lb_int32 video_thumb_stop(void *para)
{
	lb_int32 ret = 0;

	if (thumb_vid.out_sem.sem) {
		APP_LOG_D("thumb_vid.out_sem.sem:%p\n", thumb_vid.out_sem.sem);
		sem_wait(&thumb_vid.out_sem);
	}

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
lb_int32 video_thumb_set(void *desert)
{
	if (desert != NULL)
		thumb_vid.desert = desert;

	return 0;
}

/**
 * video_reg_resp - reg response function for widgets
 *
 * This function use to register response function for widgets
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 video_thumb_reg_resp(void)
{
	return 0;
}

/**
 * video_unreg_resp - unreg response function for widgets
 *
 * This function use to unregister  response function for widgets
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 video_thumb_unreg_resp(void)
{
	return 0;
}
