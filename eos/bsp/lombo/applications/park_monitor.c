/*
 * park_monitor.c - car recorder plugin implement
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
#include <pthread.h>
#include "park_monitor.h"
#include "cJSON.h"
#include "lb_types.h"
#include "lb_common.h"

static pthread_t park_monitor_task_id;

static pm_cr_status_t pm_cr_status; /* car recorder status struct */
static pm_cr_jobj_t pm_cr_jobj;
static int efs_create_flag;

char pm_cdr_rearfilename[64];
char pm_cdr_frontfilename[64];

char pm_first_filename[64];
numeral_input_t pm_wm_source;

pm_img_dsc_t *pm_wm_img[PM_WATERMARK_SOURCE_NUM];

u64 pm_used_mbsize;
u64 pm_free_mbsize;

char pm_watermark_filename[PM_WATERMARK_SOURCE_NUM][64] = {
	"V:/image/num0.png.BGRA8888.ez",
	"V:/image/num1.png.BGRA8888.ez",
	"V:/image/num2.png.BGRA8888.ez",
	"V:/image/num3.png.BGRA8888.ez",
	"V:/image/num4.png.BGRA8888.ez",
	"V:/image/num5.png.BGRA8888.ez",
	"V:/image/num6.png.BGRA8888.ez",
	"V:/image/num7.png.BGRA8888.ez",
	"V:/image/num8.png.BGRA8888.ez",
	"V:/image/num9.png.BGRA8888.ez",
	"V:/image/jianhao.png.BGRA8888.ez",
	"V:/image/maohao.png.BGRA8888.ez",
	"V:/image/temple.png.BGRA8888.ez",
	"V:/image/xiegang.png.BGRA8888.ez",
	"V:/image/logos.png.BGRA8888.ez"
};

static void pm_cr_config_array_init(cJSON *para)
{
	lb_int32 i = 0;

	for (i = 0; i < cJSON_GetArraySize(para); i++) {
		cJSON *temp;

		temp = cJSON_GetArrayItem(para, i);
		if (temp && temp->type == cJSON_Object) {
			if (cJSON_GetObjectItem(temp, "park_monitor_times")) {
				pm_cr_jobj.park_monitor_times = cJSON_GetObjectItem(temp,
						"park_monitor_times");
				printf("pm_cr_jobj.park_monitor_times %d\n",
					pm_cr_jobj.park_monitor_times->valueint);
			} else if (cJSON_GetObjectItem(temp, "park_monitor")) {
				pm_cr_jobj.park_monitor = cJSON_GetObjectItem(temp,
						"park_monitor");
				printf("pm_cr_jobj.park_monitor %d\n",
					pm_cr_jobj.park_monitor->valueint);
			}

		}
	}
}
lb_int32 pm_cr_cfg_save(void)
{
	lb_int32 ret;

	if (pm_cr_jobj.cfg_root) {
#ifdef ARCH_LOMBO_N7_CDR_MMC
		ret = lb_save_cfg_file("/mnt/sdcard/etc/cdr_config1.cfg",
				pm_cr_jobj.cfg_root);
#else
		ret = lb_save_cfg_file("/mnt/data/cdr_config.cfg", pm_cr_jobj.cfg_root);
#endif
	} else {
		printf("err cr_cfg_jobj.cfg_root is NULL");
		return -1;
	}

	return ret;
}

/**
 * cr_cfg_exit - exit config file
 *
 * This function use to exit config file,if you want save_para,
 * you must call home_cfg_init first
 *
 * Returns  1
 */
lb_int32 pm_cr_cfg_exit(void)
{
	lb_int32 ret = -1;

	pm_cr_cfg_save();
	if (pm_cr_jobj.cfg_root)
		ret = lb_exit_cfg_file(pm_cr_jobj.cfg_root);
	else {
		printf("err cr_cfg_jobj.cfg_root is NULL");
		return -1;
	}

	return ret;
}

/**
 * cr_cfg_init - get config parameter from file
 *
 * This function use to init config parameter from config file
 *
 * Returns -1 if called when get error ; otherwise, return 0
 */
lb_int32 pm_cr_cfg_init(void)
{
	lb_int32 ret = 0;

	printf("\n");
#ifdef ARCH_LOMBO_N7_CDR_MMC
	pm_cr_jobj.cfg_root = lb_open_cfg_file("/mnt/sdcard/etc/cdr_config1.cfg");
#else
	pm_cr_jobj.cfg_root = lb_open_cfg_file("/mnt/data/cdr_config.cfg");
#endif
	if (pm_cr_jobj.cfg_root == NULL) {
		pm_cr_jobj.cfg_root =
			lb_open_cfg_file(ROOTFS_MOUNT_PATH"/etc/cdr_config.cfg");
		if (pm_cr_jobj.cfg_root == NULL) {
			printf("cr_cfg_jobj.cfg_root is NULL\n");
			return -1;
		}
	}
	RT_ASSERT(pm_cr_jobj.cfg_root != NULL)

	pm_cr_jobj.record_root = cJSON_GetObjectItem(pm_cr_jobj.cfg_root, "record");
	RT_ASSERT(pm_cr_jobj.record_root != NULL)

	if (pm_cr_jobj.record_root && pm_cr_jobj.record_root->type == cJSON_Array)
		pm_cr_config_array_init(pm_cr_jobj.record_root);

	return ret;
}
static void pm_monitor_time_add()
{
	pm_cr_cfg_init();
	if (pm_cr_jobj.park_monitor_times) {
		pm_cr_jobj.park_monitor_times->valueint++;
		pm_cr_jobj.park_monitor_times->valuedouble++;
	}
	pm_cr_cfg_exit();
}
int pm_get_park_monitor_enable(void)
{
	if (pm_cr_jobj.park_monitor)
		return pm_cr_jobj.park_monitor->valueint;

	return 0;
}

int pm_get_av_status(void)
{
	return pm_cr_status.av_status;
}

void pm_set_av_status(int value)
{
	pm_cr_status.av_status = value;
}

int pm_get_sd_status()
{
	return pm_cr_status.sd_status;
}

void pm_set_sd_status(int value)
{
	pm_cr_status.sd_status = value;
}

char *pm_covert_path2basename(const char *path)
{
	const lb_byte *first, *end, *ptr;
	lb_byte *name;
	lb_int32 size;

	ptr   = (lb_byte *)path;
	first = ptr;
	end   = path + strlen(path);
	while (*ptr != '\0') {
		if (*ptr == '/')
			first = ptr + 1;
		if (*ptr == '.')
			end = ptr - 1;

		ptr++;
	}

	size = end - first + 1;
	name = malloc(size + 1);
	strncpy(name, first, size);
	name[size] = '\0';

	return name;
}

void *pm_car_recorder_init(char *video_source)
{
	int ret = -1;
	void *recorder_hd = NULL;
	rec_param_t rec_para = {0};
	int record_mod = RECORDER_TYPE_NORMAL;
	rec_time_lag_para_t rec_time_lag_para;

	recorder_hd = lb_recorder_creat();
	if (recorder_hd) {
		if(!strcmp(video_source, "isp") ||
				!strcmp(video_source, "isp_cap.0")) {
			ret = lb_recorder_ctrl(recorder_hd, LB_REC_SET_VIDEO_SOURCE,
					(void *)"vic");
			if (ret < 0) {
				printf("LB_REC_SET_VIDEO_SOURCE failed!\n");
				return NULL;
			}
		} else {
			ret = lb_recorder_ctrl(recorder_hd, LB_REC_SET_VIDEO_SOURCE,
					(void *)video_source);
			if (ret < 0) {
				printf("LB_REC_SET_VIDEO_SOURCE failed!\n");
				return NULL;
			}
		}
		rec_time_lag_para.interval = 0;
		rec_time_lag_para.play_framerate = 30000;
		lb_recorder_ctrl(recorder_hd, LB_REC_SET_MODE, &record_mod);
		lb_recorder_ctrl(recorder_hd, LB_REC_SET_TIME_LAG_PARA,
			&rec_time_lag_para);
		if (!strcmp(video_source, "isp")) {
			ret = lb_recorder_ctrl(recorder_hd, LB_REC_GET_PARA, &rec_para);
			if (ret < 0) {
				printf("LB_REC_GET_PARA failed!");
				return NULL;
			}
			rec_para.source_width = 1920;
			rec_para.source_height = 2160;
			rec_para.width = FRONT_RECORDER_SOURCE_WIDTH;
			rec_para.height = FRONT_RECORDER_SOURCE_HEIGHT;
			rec_para.enc_rect.x = 0;
			rec_para.enc_rect.y = 0;
			rec_para.enc_rect.width = 1920;
			rec_para.enc_rect.height = 1080;

			if (rec_para.height > 720)
				rec_para.bitrate = PM_HIGH_REC_BITRATE;
			else
				rec_para.bitrate = PM_LOW_REC_BITRATE;

			rec_para.audio_sample_rate = 16000;
#ifdef MEDIA_TYPE_TS
			rec_para.file_fmt = REC_OUTPUT_FORMAT_TS;
#else
			rec_para.file_fmt = REC_OUTPUT_FORMAT_MP4;
#endif
			rec_para.frame_rate = FRONT_RECORDER_SOURCE_FPS * 1000;
			ret = lb_recorder_ctrl(recorder_hd, LB_REC_SET_PARA, &rec_para);
			if (ret < 0) {
				printf("LB_REC_SET_PARA failed!");
				return NULL;
			}
		} else if (!strcmp(video_source, "vic")) {
			ret = lb_recorder_ctrl(recorder_hd, LB_REC_GET_PARA, &rec_para);
			if (ret < 0) {
				printf("LB_REC_GET_PARA failed!");
				return NULL;
			}
#ifdef ARCH_LOMBO_N7V1_TDR
			rec_para.source_width = BACK_RECORDER_SOURCE_WIDTH;
			rec_para.source_height = BACK_RECORDER_SOURCE_HEIGHT;
			rec_para.width = BACK_RECORDER_SOURCE_WIDTH/2;
			rec_para.height = BACK_RECORDER_SOURCE_HEIGHT/2;
#else
			rec_para.source_width = 1920;
			rec_para.source_height = 2160;
			rec_para.width = REAR_RECORDER_SOURCE_WIDTH;
			rec_para.height = REAR_RECORDER_SOURCE_HEIGHT;
#endif
			rec_para.enc_rect.x = 0;
			rec_para.enc_rect.y = 1080;
			rec_para.enc_rect.width = 1920;
			rec_para.enc_rect.height = 1080;
			if (rec_para.height > 720)
				rec_para.bitrate = PM_HIGH_REC_BITRATE;
			else
				rec_para.bitrate = PM_LOW_REC_BITRATE;

			rec_para.audio_sample_rate = 16000;
#ifdef MEDIA_TYPE_TS
			rec_para.file_fmt = REC_OUTPUT_FORMAT_TS;
#else
			rec_para.file_fmt = REC_OUTPUT_FORMAT_MP4;
#endif
#ifdef ARCH_LOMBO_N7V1_TDR
			rec_para.frame_rate = BACK_RECORDER_SOURCE_FPS * 1000;
#else
			rec_para.frame_rate = REAR_RECORDER_SOURCE_FPS * 1000;
#endif
			ret = lb_recorder_ctrl(recorder_hd, LB_REC_SET_PARA, &rec_para);
			if (ret < 0) {
				printf("LB_REC_SET_PARA failed!");
				return NULL;
			}
		}

	} else {
		printf("err %s %d\n", __func__, __LINE__);
		return NULL;
	}
	ret = lb_recorder_ctrl(recorder_hd, LB_REC_PREPARE, 0);
	if (ret < 0) {
		printf("LB_REC_PREPARE failed!");
		return NULL;
	}
	return recorder_hd;
}

int pm_car_recorder_exit(void *recorder_hd)
{
	if (recorder_hd)
		lb_recorder_release(recorder_hd);
	else
		printf("err %s %d\n", __func__, __LINE__);
	return 0;
}

int pm_get_recorder_time(void *recorder_hd)
{
	int time = 0;
	if (recorder_hd)
		lb_recorder_ctrl(recorder_hd, LB_REC_GET_TIME, (void *)&time);
	else
		printf("err %s %d\n", __func__, __LINE__);

	return time;
}

int pm_get_recorder_status(void *recorder_hd)
{
	int status = 0;

	if (recorder_hd)
		lb_recorder_ctrl(recorder_hd, LB_REC_GET_STATUS, (void *)&status);
	else
		printf("err %s %d\n", __func__, __LINE__);

	return status;
}

void pm_cr_dir_init()
{
	DIR *fd = NULL;

	fd = opendir(PM_FRONT_LOCK_PATH);
	if (fd == NULL)
		mkdir(PM_FRONT_LOCK_PATH, 0);
	else
		closedir(fd);
	fd = opendir(PM_REAR_LOCK_PATH);
	if (fd == NULL)
		mkdir(PM_REAR_LOCK_PATH, 0);
	else
		closedir(fd);
}

u32 pm_get_disk_info(char *path, u64 *used_mbsize, u64 *free_mbsize)
{
	struct statfs disk_info;
	int err = 0;

	err = statfs(path, &disk_info);
	if (err) {
		printf("failed to get disk infomation");
		return -1;
	}
	*used_mbsize = (((u64)(disk_info.f_blocks - disk_info.f_bfree)) * ((
					u64)disk_info.f_bsize)) >> 20;
	*free_mbsize = (((u64)disk_info.f_bfree) * ((u64)disk_info.f_bsize)) >> 20;

	return err;
}
char *pm_get_first_file(char *path)
{
	DIR *dir = NULL;
	struct dirent *ptr = NULL;
	char filename[512];

	dir = opendir(path);
	if (dir == NULL) {
		printf("Open dir error...\n");
		return NULL;
	}
	memset(pm_first_filename, 0x00, 64);
	while ((ptr = readdir(dir)) != NULL) {
		if (ptr->d_type == 1) {
#ifdef MEDIA_TYPE_TS
			if (strstr(ptr->d_name, ".ts") != NULL) {
#else
			if (strstr(ptr->d_name, ".mp4") != NULL) {
#endif
				sprintf(filename, "%s/%s", path, ptr->d_name);
				if (!strlen(pm_first_filename) ||
					strcmp(pm_first_filename, filename) > 0)
					strcpy(pm_first_filename, filename);
			}
		}
	}
	closedir(dir);

	return pm_first_filename;

}
static int __fallocate_file(char *filename, int size)
{
	int fd;
	int fsize = size;
	int ret = -1;

	fd = open(filename, O_RDWR | O_CREAT);
	if (fd < 0) {
		printf("Failed to open file %s!\n", filename);
	} else {
		ret = ioctl(fd, IO_FALLOCATE, &fsize);
		if (ret < 0)
			printf("Failed FALLOCATE %s(%d) s:%d!\n", filename, fd, fsize);
		close(fd);
		printf("filename:%s\n", filename);
	}

	return ret;
}

int pm_recorder_start_front(void *recorder_hd)
{
	int ret = -1;
	struct tm *p_tm; /* time variable */
	time_t now;
	int falloc_size;
	if (pm_get_sd_status() != 1) {
		printf("sd not is plugin %s %d\n", __func__, __LINE__);
		return -1;
	}
	printf("%s:%d\n", __func__, __LINE__);
	pm_cr_dir_init();
	printf("%s:%d\n", __func__, __LINE__);
	while (1) {
		ret = pm_get_disk_info(PM_SDCARD_PATH, &pm_used_mbsize, &pm_free_mbsize);
		if (ret < 0)
			return ret;

		printf("%s %d used_mbsize:%lld free_mbsize:%lld\n", __func__, __LINE__,
			pm_used_mbsize, pm_free_mbsize);
		if (pm_free_mbsize <= REC_RESERVE_SIZE) {
			pm_get_first_file(PM_FRONT_PATH);
			if (strlen(pm_first_filename)) {
				printf("---%s %d remove filename:%s---\n",
					__func__, __LINE__, pm_first_filename);
				remove(pm_first_filename);
			} else {
				printf("---%s %d LB_SYSMSG_RECORDER_FILE_FULL\n",
					__func__, __LINE__);
				ret = -1;
				return ret;
			}
		} else
			break;
	}
	printf("%s:%d\n", __func__, __LINE__);
	now = time(RT_NULL);
	p_tm = localtime(&now);

	memset(pm_cdr_frontfilename, 0x00, 64);
#ifdef MEDIA_TYPE_TS
	sprintf(pm_cdr_frontfilename,
		PM_FRONT_LOCK_PATH"%02d%02d%02d%02d%02d%02d_f_lock.ts",
		p_tm->tm_year + 1900, p_tm->tm_mon + 1, p_tm->tm_mday, p_tm->tm_hour,
		p_tm->tm_min, p_tm->tm_sec);
#else
	sprintf(pm_cdr_frontfilename,
		PM_FRONT_LOCK_PATH"%02d%02d%02d%02d%02d%02d_f_lock.mp4",
		p_tm->tm_year + 1900, p_tm->tm_mon + 1, p_tm->tm_mday, p_tm->tm_hour,
		p_tm->tm_min, p_tm->tm_sec);
#endif
	printf("recorder_start_front:%s\n", pm_cdr_frontfilename);
	falloc_size = get_falloc_size(PM_HIGH_REC_BITRATE, PM_DURATION);

	__fallocate_file(pm_cdr_frontfilename, falloc_align(falloc_size));
	if (recorder_hd) {
		ret = lb_recorder_ctrl(recorder_hd, LB_REC_SET_OUTPUT_FILE,
				(void *)pm_cdr_frontfilename);
		if (ret < 0) {
			printf("LB_REC_SET_OUTPUT_FILE failed!");
			return -1;
		}
		printf("%s %d\n", __func__, __LINE__);
		pm_watermark_set_source(recorder_hd, PM_WATERMARK_SOURCE_NUM,
			pm_watermark_filename);
		pm_watermark_time(recorder_hd, p_tm);
		pm_watermark_logo(recorder_hd, 14);
		printf("%s %d\n", __func__, __LINE__);
		ret = lb_recorder_ctrl(recorder_hd, LB_REC_SET_AUDIO_MUTE, (void *)0);
		if (ret < 0)
			printf("LB_REC_SET_AUDIO_MUTE failed!");
			rt_thread_mdelay(500);
		ret = lb_recorder_ctrl(recorder_hd, LB_REC_START, 0);
		if (ret < 0) {
			printf("LB_REC_START failed!");
			return -1;
		}
		printf("%s:%d\n", __func__, __LINE__);
	}
	printf("%s %d\n", __func__, __LINE__);

	return ret;
}

int pm_recorder_start_rear(void *recorder_hd)
{
	int ret = -1;
	struct tm *p_tm; /* time variable */
	time_t now;
	int falloc_size;

	if (pm_get_sd_status() != 1) {
		printf("sd not is plugin %s %d\n", __func__, __LINE__);
		return -1;
	}
	printf("%s:%d\n", __func__, __LINE__);
	pm_cr_dir_init();

	while (1) {
		pm_get_disk_info(PM_SDCARD_PATH, &pm_used_mbsize, &pm_free_mbsize);
		printf("%s %d used_mbsize:%lld free_mbsize:%lld\n", __func__, __LINE__,
			pm_used_mbsize, pm_free_mbsize);
		if (pm_free_mbsize <= REC_RESERVE_SIZE) {
			pm_get_first_file(PM_FRONT_PATH);
			if (strlen(pm_first_filename)) {
				printf("---%s %d remove filename:%s---\n",
					__func__, __LINE__, pm_first_filename);
				remove(pm_first_filename);
			} else {
				printf("---%s %d LB_SYSMSG_RECORDER_FILE_FULL\n",
					__func__, __LINE__);
				ret = -1;
				return ret;
			}
		} else
			break;
	}

	now = time(RT_NULL);
	p_tm = localtime(&now);
	memset(pm_cdr_rearfilename, 0x00, 64);
#ifdef MEDIA_TYPE_TS
	sprintf(pm_cdr_rearfilename,
		PM_REAR_LOCK_PATH"%02d%02d%02d%02d%02d%02d_r_lock.ts",
		p_tm->tm_year + 1900, p_tm->tm_mon + 1, p_tm->tm_mday, p_tm->tm_hour,
		p_tm->tm_min, p_tm->tm_sec);
#else
	sprintf(pm_cdr_rearfilename,
		PM_REAR_LOCK_PATH"%02d%02d%02d%02d%02d%02d_r_lock.mp4",
		p_tm->tm_year + 1900, p_tm->tm_mon + 1, p_tm->tm_mday, p_tm->tm_hour,
		p_tm->tm_min, p_tm->tm_sec);
#endif
#ifdef ARCH_LOMBO_N7V1_TDR
	if (BACK_RECORDER_SOURCE_HEIGHT/2 > 720)
		falloc_size = get_falloc_size(PM_HIGH_REC_BITRATE, PM_DURATION);
	else
		falloc_size = get_falloc_size(PM_LOW_REC_BITRATE, PM_DURATION);
#else
	if (REAR_RECORDER_SOURCE_HEIGHT > 720)
		falloc_size = get_falloc_size(PM_HIGH_REC_BITRATE, PM_DURATION);
	else
		falloc_size = get_falloc_size(PM_LOW_REC_BITRATE, PM_DURATION);
#endif
	__fallocate_file(pm_cdr_rearfilename, falloc_align(falloc_size));
	if (recorder_hd) {
		ret = lb_recorder_ctrl(recorder_hd, LB_REC_SET_OUTPUT_FILE,
				(void *)pm_cdr_rearfilename);
		if (ret < 0) {
			printf("LB_REC_SET_OUTPUT_FILE failed!");
			return -1;
		}

		printf("%s %d\n", __func__, __LINE__);
		pm_watermark_set_source(recorder_hd, PM_WATERMARK_SOURCE_NUM,
			pm_watermark_filename);
		pm_watermark_time(recorder_hd, p_tm);

		pm_watermark_logo(recorder_hd, 14);
		printf("%s %d\n", __func__, __LINE__);

		ret = lb_recorder_ctrl(recorder_hd, LB_REC_SET_AUDIO_MUTE,
				(void *)0);
		if (ret < 0)
			printf("LB_REC_SET_AUDIO_MUTE failed!");
			rt_thread_mdelay(500);
		ret = lb_recorder_ctrl(recorder_hd, LB_REC_START, 0);
		if (ret < 0) {
			printf("LB_REC_START failed!");
			return -1;
		}
	} else
		printf("err %s %d\n", __func__, __LINE__);

	return ret;
}


/**
 * recoder_stop - stop record
 * @recorder_hd: recorder handle,init by recoder_start
 *
 * This function use to stop record video & release recorder handle
 */
int pm_recoder_stop(void *recorder_hd)
{
	int ret;

	if (recorder_hd) {
		ret = lb_recorder_ctrl(recorder_hd, LB_REC_STOP, 0);
		if (ret < 0) {
			printf("LB_REC_STOP failed!");
			return -1;
		}
	}

	return 0;
}

int pm_watermark_set_source(void *rec, unsigned int source_num,
	char source_name[][64])
{
	int i;
	int ret = -1;
	if (pm_wm_source.input_picture_num == 0) {
		pm_wm_source.colorspace = OMX_COLOR_Format32bitBGRA8888;
		pm_wm_source.input_picture_num = source_num;
		for (i = 0; i < pm_wm_source.input_picture_num; i++) {
#ifdef RT_USING_EGUI
			pm_wm_img[i] = (pm_img_dsc_t *)eimage_create_img_buf(
					source_name[i]);
#endif
			if (pm_wm_img[i]) {
				pm_wm_source.numeral_picture[i].width =
					pm_wm_img[i]->header.w;
				pm_wm_source.numeral_picture[i].height =
					pm_wm_img[i]->header.h;
				pm_wm_source.numeral_picture[i].picture_size =
					pm_wm_img[i]->header.w *
					pm_wm_img[i]->header.h * 4;
				pm_wm_source.numeral_picture[i].stride =
					pm_wm_img[i]->header.w;
				pm_wm_source.numeral_picture[i].data = pm_wm_img[i]->data;
			} else {
				printf("open file error\n");
				return ret;
			}
		}
	}
	if (rec)
		ret = lb_recorder_ctrl(rec, LB_REC_SET_WATERMARK_SOURCE, &pm_wm_source);
	return ret;
}
int pm_watermark_exit_set_source(unsigned int source_num)
{
	int i;
	int ret = -1;

	for (i = 0; i < source_num; i++) {
		if (pm_wm_img[i]) {
#ifdef RT_USING_EGUI
			eimage_destory_img_buf((void *)pm_wm_img[i]);
#endif
			pm_wm_img[i] = NULL;
		} else {
			return ret;
		}
	}
	ret = 0;

	return ret;
}

int pm_watermark_time(void *rec, struct tm *p_tm)
{
	numeral_picture_index_t watermark;
	numeral_picture_index_t *wm;
	int ret = -1;

	watermark.total_index_num = 19;
	watermark.start_x_pos = 64;
	watermark.start_y_pos = 32;
	watermark.blending_area_index = 0;

	watermark.index_array[0] = ((p_tm->tm_year + 1900) / 1000) % 10;
	watermark.index_array[1] = ((p_tm->tm_year + 1900) / 100) % 10;
	watermark.index_array[2] = ((p_tm->tm_year + 1900) / 10) % 10;
	watermark.index_array[3] = (p_tm->tm_year + 1900) % 10;
	watermark.index_array[4] = PM_XIEGANG;
	watermark.index_array[5] = ((p_tm->tm_mon + 1) / 10) % 10;
	watermark.index_array[6] = (p_tm->tm_mon + 1) % 10;
	watermark.index_array[7] = PM_XIEGANG;
	watermark.index_array[8] = (p_tm->tm_mday / 10) % 10;
	watermark.index_array[9] = p_tm->tm_mday % 10;
	watermark.index_array[10] = PM_TEMPLE;
	watermark.index_array[11] = (p_tm->tm_hour / 10) % 10;
	watermark.index_array[12] = p_tm->tm_hour % 10;
	watermark.index_array[13] = PM_MAOHAO;
	watermark.index_array[14] = (p_tm->tm_min / 10) % 10;
	watermark.index_array[15] = p_tm->tm_min % 10;
	watermark.index_array[16] = PM_MAOHAO;
	watermark.index_array[17] = (p_tm->tm_sec / 10) % 10;
	watermark.index_array[18] = p_tm->tm_sec % 10;

	wm = &watermark;
	if (rec)
		ret = lb_recorder_ctrl(rec, LB_REC_SET_WATERMARK_INDEX, wm);
	return ret;
}

int pm_watermark_logo(void *rec, int index)
{
	numeral_picture_index_t watermark;
	numeral_picture_index_t *wm;
	int ret = -1;

	watermark.total_index_num = 1;
	watermark.start_x_pos = 1920-320-64;
	watermark.start_y_pos = 32;
	watermark.blending_area_index = 1;

	watermark.index_array[0] = index;

	wm = &watermark;
	if (index >= PM_WATERMARK_SOURCE_NUM)
		wm = NULL;
	if (rec)
		ret = lb_recorder_ctrl(rec, LB_REC_SET_WATERMARK_INDEX, wm);

	return ret;
}


/**
 * park_monitor_stop - car recorder app exit
 * @ap: APP struct pointer.
 *
 * This function use to exit a app,when lb_app_close is called,this function is called.
 *
 * Returns 0
 */



static void *pm_cdr_status_refresh(void *parameter);

/* static int last_back_status; */
/* static int last_av_status; */

/**
 * cdr_statusbar_init - create left & right bar refreash thread
 * @thread_id: thread id
 *
 * This function init statusbar refreash thread attribute,create bar refreash thread
 *
 * Returns 0
 */
lb_int32 pm_cdr_status_thread_init(pthread_t *thread_id)
{
	pthread_attr_t                  tmp_attr;
	struct sched_param              shed_param;
	lb_int32                        ret;

	ret = pthread_attr_init(&tmp_attr);
	if (ret != 0) {
		printf("\n [%s] init thread attr error: %d!\n", __func__, ret);
		return 0;
	}
	ret = pthread_attr_getschedparam(&tmp_attr, &shed_param);
	if (ret != 0) {
		printf("\n [%s] get thread priority error: %d!\n", __func__, ret);
		return 0;
	}
	ret = pthread_attr_setscope(&tmp_attr, PTHREAD_SCOPE_SYSTEM);
	if (ret != 0) {
		printf("\n [%s] set thread scope error: %d!\n", __func__, ret);
		return 0;
	}

	shed_param.sched_priority = PM_CDR_STATUS_THREAD_PRIORITY;
	ret = pthread_attr_setschedparam(&tmp_attr, &shed_param);
	if (ret != 0) {
		printf("\n [%s] set thread priority error: %d!\n", __func__, ret);
		return 0;
	}
	ret = pthread_attr_setstacksize(&tmp_attr, (size_t)PM_CDR_STATUS_STACK_SIZE);
	if (ret != 0) {
		printf("\n [%s] set thread stack size error: %d!!\n", __func__, ret);
		return 0;
	}

	if (pthread_create(thread_id, &tmp_attr, &pm_cdr_status_refresh, NULL) != 0)
		printf("create thread_func1 failed!\n");


	pthread_attr_destroy(&tmp_attr);

	return 0;
}

/**
 * statusbar_refresh - the thread refresh statusbar view,update time & tfcard & battery
 * view,if status change
 * @parameter: reserved for user custom,no use
 *
 * This function use to refresh statusbar view in thread,update time & tfcard & battery
 * widget view,if status change,when car_recorder app is opened,  update recording status.
 */
static void *pm_cdr_status_refresh(void *parameter)
{
	static int r_time;
	static int last_r_time;
	struct tm *p_tm; /* time variable */
	time_t now;
	lb_system_msg_t system_msg_data;
	int err;

	while (1) {
		err = lb_system_mq_recv((void *)&system_msg_data, 20, 0);
		if (err == 0) {
			printf("%s:%d %x\n", __func__, __LINE__, system_msg_data.type);
			if (system_msg_data.type == LB_SYSMSG_FS_PART_MOUNT_OK) {
				printf("%s:%d\n", __func__, __LINE__);
				pm_set_sd_status(1);
				if (!efs_create_flag) {
					efs_create(ROOTFS_MOUNT_PATH"/res/res.iso");
					efs_create_flag = 1;
				}
				if (pm_get_recorder_status(pm_recorder_fd) !=
					PM_RECORDER_STATUS_RECORD) {
#ifdef PM_OPEN_FRONT_RECORDER
					printf("%s:%d\n", __func__, __LINE__);
					err = pm_recorder_start_front(pm_recorder_fd);
					if (err < 0)
						pthread_exit(NULL);
					printf("%s:%d\n", __func__, __LINE__);
#endif
#ifdef PM_OPEN_REAR_RECORDER
					if (pm_get_av_status())
						err = pm_recorder_start_rear(
								pm_recorder_rd);
#endif
				}
			} else if (system_msg_data.type == LB_SYSMSG_SD_PLUGOUT) {
				printf("%s:%d\n", __func__, __LINE__);
				pm_set_sd_status(0);
				if (pm_get_recorder_status(pm_recorder_fd) ==
					PM_RECORDER_STATUS_RECORD) {
					printf("%s:%d\n", __func__, __LINE__);
#ifdef PM_OPEN_REAR_RECORDER
					if (pm_get_av_status())
						pm_recoder_stop(pm_recorder_rd);
#endif
#ifdef PM_OPEN_FRONT_RECORDER
					printf("%s:%d\n", __func__, __LINE__);
					pm_recoder_stop(pm_recorder_fd);
#endif
					pm_monitor_time_add();
				}
				break;
			} else if (system_msg_data.type == LB_SYSMSG_AV_PLUGIN) {
				pm_set_av_status(1);
#ifdef PM_OPEN_REAR_RECORDER
				if (pm_get_recorder_status(pm_recorder_rd) !=
					PM_RECORDER_STATUS_RECORD) {
					err = pm_recorder_start_rear(pm_recorder_rd);
					if (err < 0)
						printf("recorder_start_rear failed!\n");
				}
#endif
			} else if (system_msg_data.type == LB_SYSMSG_AV_PLUGOUT) {
				pm_set_av_status(0);
				if (pm_get_recorder_status(pm_recorder_rd) ==
					PM_RECORDER_STATUS_RECORD) {
					err = pm_recoder_stop(pm_recorder_rd);
					if (err < 0)
						printf("recorder_start_rear failed!\n");
					pm_monitor_time_add();
					break;
				}

			} else if (system_msg_data.type ==
			LB_SYSMSG_SD_MOUNT_INV_VOL) {
				printf("not format\n");
				break;
			}

		}
		if (get_bat_level() == 1) {
			if (pm_get_recorder_status(pm_recorder_fd) ==
					PM_RECORDER_STATUS_RECORD) {
					printf("%s:%d\n", __func__, __LINE__);
#ifdef PM_OPEN_REAR_RECORDER
					if (pm_get_av_status())
						pm_recoder_stop(pm_recorder_rd);
#endif
#ifdef PM_OPEN_FRONT_RECORDER
					printf("%s:%d\n", __func__, __LINE__);
					pm_recoder_stop(pm_recorder_fd);
					if (r_time > 2)
						pm_monitor_time_add();
					else {
						remove(pm_cdr_rearfilename);
						remove(pm_cdr_frontfilename);
					}
#endif
			}
			break;
		} else if (!get_bat_level() && get_acc_sio_val()) {
			if (pm_get_recorder_status(pm_recorder_fd) ==
					PM_RECORDER_STATUS_RECORD) {
				printf("%s:%d\n", __func__, __LINE__);
#ifdef PM_OPEN_REAR_RECORDER
				if (pm_get_av_status()) {
					pm_recoder_stop(pm_recorder_rd);
					//remove(pm_cdr_rearfilename);
				}
#endif
#ifdef PM_OPEN_FRONT_RECORDER
				printf("%s:%d\n", __func__, __LINE__);
				pm_recoder_stop(pm_recorder_fd);
				//remove(pm_cdr_frontfilename);
#endif
               pm_monitor_time_add();
			}
			lb_hw_reboot();
			break;
		}
		r_time = pm_get_recorder_time(pm_recorder_fd);
		printf("status:%x: %d %s:%d\n", pm_get_recorder_status(pm_recorder_fd),
			get_bat_level(), __FILE__, __LINE__);
		if (pm_get_recorder_status(pm_recorder_fd) == PM_RECORDER_STATUS_RECORD) {
			if (r_time != last_r_time) {
				now = time(RT_NULL);
				p_tm = localtime(&now);
				pm_watermark_time(pm_recorder_fd, p_tm);
				pm_watermark_logo(pm_recorder_fd, 14);
				if (pm_get_av_status()) {
					pm_watermark_time(pm_recorder_rd, p_tm);
					pm_watermark_logo(pm_recorder_rd, 14);
				}
				last_r_time = r_time;
			}
			if (r_time >= 60 ) {
				printf("%s:%d\n", __FILE__, __LINE__);
				if (pm_get_recorder_status(pm_recorder_fd) ==
					PM_RECORDER_STATUS_RECORD) {

#ifdef PM_OPEN_FRONT_RECORDER
					pm_recoder_stop(pm_recorder_fd);
#endif
#ifdef PM_OPEN_REAR_RECORDER
					if (pm_get_av_status()) {
						rt_thread_delay(50);
						pm_recoder_stop(pm_recorder_rd);
					}
#endif
					pm_monitor_time_add();
					pthread_exit(NULL);
				}
			}
			rt_thread_delay(30);
		} else {
			r_time = 0;
			last_r_time++;
			if (last_r_time >= 60)
				break;

		}

	}
	return NULL;
}


/**
 * statusbar_exit - exit statusbar refreash thread
 * @thread_id: thread id
 *
 * This function exit statusbar refreash thread
 *
 * Returns 0
 */
lb_int32 park_monitor_stop(void)
{
	lb_int32 ret = 0;

	printf("in %s %d\n", __func__, __LINE__);
#ifdef PM_OPEN_REAR_RECORDER
	ret = pm_recoder_stop(pm_recorder_rd);
	if (ret < 0)
		printf("recoder_stop err\n");
#endif
	ret = pm_car_recorder_exit(pm_recorder_rd);
	if (ret < 0)
		printf("park_monitor_exit err\n");

#ifdef PM_OPEN_FRONT_RECORDER
	ret = pm_recoder_stop(pm_recorder_fd);
	if (ret < 0)
		printf("recoder_stop err\n");
#endif
	ret = pm_car_recorder_exit(pm_recorder_fd);
	if (ret < 0)
		printf("park_monitor_exit err\n");
	pm_watermark_exit_set_source(PM_WATERMARK_SOURCE_NUM);

	return 0;
}

/**
 * park_monitor_create - car recorder app create
 *
 * This function use to create app view and app module init
 *
 * Returns 0 if called when get success ; otherwise, return other values defined bu user
 */
void park_monitor_create(void)
{
	lb_int32 ret = 0;
#ifdef LOMBO_GSENSOR
	gsensor_set_park_monitor_cfg(RT_TRUE);
#endif
	pm_recorder_fd = pm_car_recorder_init("isp");
	RT_ASSERT(pm_recorder_fd);
	printf("in %s %d\n", __func__, __LINE__);
	pm_recorder_rd = pm_car_recorder_init("vic");
	RT_ASSERT(pm_recorder_rd);
	printf("av_status %d %s %d\n", pm_get_av_status(), __func__, __LINE__);
	pm_cdr_status_thread_init(&park_monitor_task_id);

	printf("ret:%d %s %d\n", ret, __FILE__, __LINE__);
	pthread_join(park_monitor_task_id, NULL);
	printf("ret:%d %s %d\n", ret, __FILE__, __LINE__);

	return;
}

