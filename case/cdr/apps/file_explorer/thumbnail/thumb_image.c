/*
 * thumb_image.c - thumb image from file explorer
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

static thumb_img_t thumb_img;

static lb_int32 thumb_check_para(void)
{
	lb_int32 ret = 0;

	RT_ASSERT(thumb_img.desert != NULL);
	RT_ASSERT(thumb_img.path != NULL);
	RT_ASSERT(thumb_img.thumb != NULL);
	RT_ASSERT(thumb_img.data != NULL);
	RT_ASSERT(thumb_img.image != NULL);
	RT_ASSERT(thumb_img.db_data != NULL);
	RT_ASSERT(thumb_img.db_image != NULL);

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
	thumb_img_dsc_t img_dsc;


	thumb_check_para();

	ret = mars_get_node_thumb(thumb_img.desert, i, &thumb_img.thumb[i]);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	file = open(thumb_img.thumb[i], O_RDONLY);
	if (file == -1) {
		APP_LOG_D("No thumb for loading\n");
		ret = 1;
		goto exit;
	}

	thumb_img.db_image[i] = mars_mem_alloc(sizeof(thumb_img_dsc_t));
	if (thumb_img.db_image[i] == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	len = read(file, &img_dsc, sizeof(thumb_img_dsc_t));
	if (len != sizeof(thumb_img_dsc_t)) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	thumb_img.db_data[i] = mars_mem_alloc(img_dsc.data_size);
	if (thumb_img.db_data[i] == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	len = read(file, thumb_img.db_data[i], img_dsc.data_size);
	if (len != img_dsc.data_size) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	img_dsc.data = (const uint8_t *)thumb_img.db_data[i];
	memcpy(thumb_img.db_image[i], &img_dsc, sizeof(thumb_img_dsc_t));

	close(file);
	file = -1;

	return ret;

exit:
	if (thumb_img.db_data[i]) {
		mars_mem_free(thumb_img.db_data[i]);
		thumb_img.db_data[i] = NULL;
	}

	if (thumb_img.db_image[i]) {
		mars_mem_free(thumb_img.db_image[i]);
		thumb_img.db_image[i] = NULL;
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

	ret = mars_get_node_thumb(thumb_img.desert, i, &thumb_img.thumb[i]);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	file = open(thumb_img.thumb[i], O_RDONLY);
	if (file != -1) {
		/* APP_LOG_W("\n"); */
		ret = -1;
		goto exit;
	}

	ret = mars_get_node_path(thumb_img.desert, i, &thumb_img.path[i]);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	thumb_img.data[i] = mod_media_photo_get_thumb(thumb_img.path[i], 96, 48);
	if (thumb_img.data[i] == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	thumb_img.image[i] = (char *)eimage_create_img_buf_thumb(thumb_img.data[i]);
	if (thumb_img.image[i] == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	return ret;

exit:
	if (thumb_img.image[i]) {
		eimage_destory_img_buf_thumb((void *)thumb_img.image[i]);
		thumb_img.image[i] = NULL;
	}

	if (thumb_img.data[i]) {
		mod_media_photo_free_thumb(thumb_img.data[i]);
		thumb_img.data[i] = NULL;
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
	thumb_img_dsc_t img_dsc;

	thumb_check_para();

	if (thumb_img.image[i] == NULL) {
		APP_LOG_D("No thumb for storing\n");
		ret = 1;
		goto exit;
	}

	memcpy(&img_dsc, thumb_img.image[i], sizeof(thumb_img_dsc_t));

	if (thumb_img.thumb[i] == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	file = open(thumb_img.thumb[i], O_RDONLY);
	if (file != -1) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	file = open(thumb_img.thumb[i], O_WRONLY | O_CREAT);
	if (file == -1) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	len = write(file, &img_dsc, sizeof(thumb_img_dsc_t));
	if (len != sizeof(thumb_img_dsc_t)) {
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
	lb_int16 tab = 1;

	/* Find the list by id because params are buttons not the list we need */
	file_get_list(tab, (void **)&pproperty);
	if (NULL == pproperty) {
		APP_LOG_W("failed\n");
		return LB_ERROR_NO_MEM;
	}

	if (thumb_img.data[index] && thumb_img.image[index]) {
		pproperty->file_array[index].thumb_data = thumb_img.image[index];
		pproperty->file_array[index].thumb_used = 1;
		pproperty->file_array[index].decode_flag = 1;
	} else if (thumb_img.db_data[index] && thumb_img.db_image[index]) {
		pproperty->file_array[index].thumb_data = thumb_img.db_image[index];
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

	if (thumb_img.db_image[i]) {
		mars_mem_free(thumb_img.db_image[i]);
		thumb_img.db_image[i] = NULL;
	}

	if (thumb_img.db_data[i]) {
		mars_mem_free(thumb_img.db_data[i]);
		thumb_img.db_data[i] = NULL;
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

	if (thumb_img.image[i]) {
		eimage_destory_img_buf_thumb((void *)thumb_img.image[i]);
		thumb_img.image[i] = NULL;
	}

	if (thumb_img.data[i]) {
		mod_media_photo_free_thumb(thumb_img.data[i]);
		thumb_img.data[i] = NULL;
	}

	return ret;
}

static lb_int32 thumb_buff_init(lb_int32 num)
{
	lb_int32 ret = 0;

	thumb_img.path = mars_mem_alloc(num * sizeof(char *));
	if (thumb_img.path == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}
	memset(thumb_img.path, 0x00, num * sizeof(char *));

	thumb_img.thumb = mars_mem_alloc(num * sizeof(char *));
	if (thumb_img.thumb == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}
	memset(thumb_img.thumb, 0x00, num * sizeof(char *));

	thumb_img.data = mars_mem_alloc(num * sizeof(char *));
	if (thumb_img.data == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}
	memset(thumb_img.data, 0x00, num * sizeof(char *));

	thumb_img.image = mars_mem_alloc(num * sizeof(char *));
	if (thumb_img.image == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}
	memset(thumb_img.image, 0x00, num * sizeof(char *));

	thumb_img.db_data = mars_mem_alloc(num * sizeof(char *));
	if (thumb_img.db_data == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}
	memset(thumb_img.db_data, 0x00, num * sizeof(char *));

	thumb_img.db_image = mars_mem_alloc(num * sizeof(char *));
	if (thumb_img.db_image == NULL) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}
	memset(thumb_img.db_image, 0x00, num * sizeof(char *));

	return ret;
exit:
	if (thumb_img.path) {
		mars_mem_free(thumb_img.path);
		thumb_img.path = NULL;
	}

	if (thumb_img.thumb) {
		mars_mem_free(thumb_img.thumb);
		thumb_img.thumb = NULL;
	}

	if (thumb_img.image) {
		mars_mem_free(thumb_img.image);
		thumb_img.image = NULL;
	}

	if (thumb_img.data) {
		mars_mem_free(thumb_img.data);
		thumb_img.data = NULL;
	}

	if (thumb_img.db_image) {
		mars_mem_free(thumb_img.db_image);
		thumb_img.db_image = NULL;
	}

	if (thumb_img.db_data) {
		mars_mem_free(thumb_img.db_data);
		thumb_img.db_data = NULL;
	}

	return ret;
}

static lb_int32 thumb_buff_exit(void)
{
	lb_int32 ret = 0;

	if (thumb_img.path) {
		mars_mem_free(thumb_img.path);
		thumb_img.path = NULL;
	}

	if (thumb_img.thumb) {
		mars_mem_free(thumb_img.thumb);
		thumb_img.thumb = NULL;
	}

	if (thumb_img.image) {
		mars_mem_free(thumb_img.image);
		thumb_img.image = NULL;
	}

	if (thumb_img.data) {
		mars_mem_free(thumb_img.data);
		thumb_img.data = NULL;
	}

	if (thumb_img.db_image) {
		mars_mem_free(thumb_img.db_image);
		thumb_img.db_image = NULL;
	}

	if (thumb_img.db_data) {
		mars_mem_free(thumb_img.db_data);
		thumb_img.db_data = NULL;
	}

	return ret;
}

static lb_int32 thumb_core_init(void)
{
	lb_int32 ret = 0;

	ret = sem_init(&thumb_img.out_sem, 0, 1);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		return -1;
	}

	return ret;
}

static lb_int32 thumb_core_exit(void)
{
	lb_int32 ret = 0;

	ret = sem_destroy(&thumb_img.out_sem);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		return -1;
	}
	memset(&thumb_img.out_sem, 0x00, sizeof(sem_t));

	return ret;
}

static void *image_thread_refr(void *parameter)
{
	lb_int32 i = 0;
	lb_int32 num = 0;
	lb_int32 ret = 0;

	APP_LOG_W("ENTER\n");

	rt_thread_delay(1);

	num = mars_get_node_num(thumb_img.desert);
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
		if (thumb_img.exit == 1) {
			APP_LOG_W("Exit\n");
			goto exit;
		}

		sem_wait(&thumb_img.out_sem);
		ret = thumb_dbs_alloc(i);
		sem_post(&thumb_img.out_sem);

		if (ret == -1) {
			APP_LOG_W("Exit\n");
			goto exit;
		}

		if (ret == 0) {
			APP_LOG_D("Load thumb from db\n");
			rt_thread_delay(1);
		} else if (ret == 1) {
			APP_LOG_D("Load thumb from dec\n");
			sem_wait(&thumb_img.out_sem);
			ret = thumb_dec_alloc(i);
			sem_post(&thumb_img.out_sem);
			if (ret != 0) {
				/* APP_LOG_W("\n"); */
				rt_thread_delay(1);
				continue;
			}
		}
		sem_wait(&thumb_img.out_sem);
		ret = thumb_update_one(i);
		sem_post(&thumb_img.out_sem);
		if (ret != 0) {
			APP_LOG_W("Exit\n");
			goto exit;
		}
		rt_thread_delay(1);
	}

	for (i = 0; i < num; i++) {
		if (thumb_img.exit == 1) {
			APP_LOG_W("Exit\n");
			goto exit;
		}

		sem_wait(&thumb_img.out_sem);
		ret = store_to_dbs(i);
		sem_post(&thumb_img.out_sem);

		if (ret == -1) {
			APP_LOG_W("Exit\n");
			goto exit;
		}

		rt_thread_delay(1);
	}

	while (thumb_img.exit != 1)
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

static lb_int32 image_thread_init(void)
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

	ret = pthread_create(&thumb_img.id, &tmp_attr, &image_thread_refr, NULL);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		return -1;
	}

	pthread_attr_destroy(&tmp_attr);

	thumb_img.exit = 0;

	return ret;
}

static lb_int32 image_thread_exit(void)
{
	lb_int32 ret = 0;

	thumb_img.exit = 1;
	ret = pthread_join(thumb_img.id, NULL);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		return -1;
	}
	thumb_img.id = 0;

	return ret;
}

/**
 * video_mod_init - initial the mod of image
 * @param: lb_obj_t object pointer.
 *
 * This function initial the mod of image
 *
 * Returns 0
 */
lb_int32 image_thumb_init(void *para)
{
	lb_int32 ret = 0;

	if (thumb_img.id == 0) {
		ret = thumb_core_init();
		if (ret != 0) {
			APP_LOG_W("failed\n");
			return ret;
		}

		ret = image_thread_init();
		if (ret != 0) {
			APP_LOG_W("failed\n");
			return ret;
		}
	}

	return ret;
}

/**
 * video_mod_exit - exit the mod of image
 * @param: lb_obj_t object pointer.
 *
 * This function exit the mod of image
 *
 * Returns 0
 */
lb_int32 image_thumb_exit(void *para)
{
	lb_int32 ret = 0;

	APP_LOG_W("image_thumb_exit begin\n");

	if (thumb_img.id != 0) {
		ret = image_thread_exit();
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

	APP_LOG_W("image_thumb_exit end\n");

	return ret;
}

lb_int32 image_thumb_start(void *para)
{
	lb_int32 ret = 0;

	if (thumb_img.out_sem.sem) {
		APP_LOG_D("thumb_img.out_sem.sem:%p\n", thumb_img.out_sem.sem);
		sem_post(&thumb_img.out_sem);
	}

	return ret;
}

lb_int32 image_thumb_stop(void *para)
{
	lb_int32 ret = 0;

	if (thumb_img.out_sem.sem) {
		APP_LOG_D("thumb_img.out_sem.sem:%p\n", thumb_img.out_sem.sem);
		sem_wait(&thumb_img.out_sem);
	}

	return ret;
}

/**
 * video_set - set the image list and index
 * @param: lb_obj_t object pointer.
 *
 * This function set the image list and index
 *
 * Returns 0
 */
lb_int32 image_thumb_set(void *desert)
{
	if (desert != NULL)
		thumb_img.desert = desert;

	return 0;
}

/**
 * video_reg_resp - reg response function for widgets
 *
 * This function use to register response function for widgets
 *
 * Returns 0 if called when get success ; otherwise, return other custom values
 */
lb_int32 image_thumb_reg_resp(void)
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
lb_int32 image_thumb_unreg_resp(void)
{
	return 0;
}
