/*
 * application.c - the main entry for application
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

#ifndef APPLICATION_C
#define APPLICATION_C

#include <rtthread.h>
#include <debug.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#ifdef ARCH_LOMBO_N7
#include <app_manage.h>
#include <mod_manage.h>
#include "system/system_mq.h"
#include "system/system_thread.h"
#include "system/system.h"
#ifdef CPUFREQ_GOVERNOR_ADAPTIVE
#include "cpufreq.h"
#endif
#ifdef LOMBO_CPUFREQ_COOLING
#include "lombo_thermal.h"
#endif
#ifdef RT_USING_EGUI
#ifdef RT_SEL_CAR_VIDEO_CASE
#include "park_monitor.h"
#include "vcom_pwm.h"
#endif
#include "lb_ui.h"
#include "lb_types.h"

#endif
#endif
#include <dfs_posix.h>
#include <boot_param.h>
#include "../rtc/rtc_csp.h"

#ifdef ARCH_LOMBO_N7
#ifdef RT_USING_EGUI
#ifdef RT_SEL_CAR_VIDEO_CASE
static app_t home;

#endif
#endif
#endif

extern rt_int16_t g_sd_plugout;
#define NOR_DEV_NAME			"nor0"
#define READ_BUF_SIZE			(256 * 1024)
struct tag_eos_firmware {
	int f_firmware;
	rt_device_t nor_dev;
	int sector_num;
	int sector_cnt;
	int sector_start;
	int bytes_per_sector;
	unsigned char buf[READ_BUF_SIZE];
	unsigned char buf_temp[READ_BUF_SIZE];
	char ver_str[96];
	int percent;
	
	char *bin_buffer;
	int bin_cnt;
};

static char *g_bin_buffer;

int get_update_state_ext(void)
{
	int f;
	int err = 0;
	struct rt_device_blk_geometry geometry;
	rt_device_t nor_dev;
	struct stat f_buf = {0};
	char ver_str[96] = {0};
	char ver_str_me[96] = {0};

	f = open("/mnt/sdcard/firmware.bin", O_RDONLY);
	if (f == -1) {
		LOG_E("open firmware fail\n");
		return -1;
	}

	/* find nor device */
	nor_dev = rt_device_find(NOR_DEV_NAME);
	if (!nor_dev) {
		LOG_E("failed to find nor device: %s", NOR_DEV_NAME);
		close(f);
		return -1;
	}
	rt_device_open(nor_dev, RT_DEVICE_OFLAG_RDWR);

	/* get flash geometry */
	err = rt_device_control(nor_dev, RT_DEVICE_CTRL_BLK_GETGEOME, &geometry);
	if (err) {
		LOG_E("failed to get flash geometry");
		rt_device_close(nor_dev);
		close(f);
		return -1;
	}

	fstat(f, &f_buf);
	if (
	f_buf.st_size - geometry.sector_count * geometry.bytes_per_sector < 0 ||
	f_buf.st_size - geometry.sector_count * geometry.bytes_per_sector > 96) {
		LOG_E("firmware file len err,%d, %d", f_buf.st_size,
				geometry.sector_count * geometry.bytes_per_sector);
		rt_device_close(nor_dev);
		close(f);
		return -1;
	}

	if (f_buf.st_size - geometry.sector_count * geometry.bytes_per_sector > 0) {
		lseek(f, geometry.sector_count * geometry.bytes_per_sector, SEEK_SET);
		read(f, ver_str,
		      f_buf.st_size - geometry.sector_count * geometry.bytes_per_sector);
		lseek(f, 0, SEEK_SET);

		lb_get_cfg_version(ver_str_me);
		if (strcmp(ver_str, ver_str_me) == 0 &&
				strcmp("spinor", boot_get_boot_type()) == 0) {
			LOG_E("version same!");
			rt_device_close(nor_dev);
			close(f);
			return -1;
		}
	}




	g_bin_buffer = malloc(f_buf.st_size);
	if (g_bin_buffer == NULL) {
		LOG_E("malloc fail!");
		return -1;
	} else {
		if ( f_buf.st_size != read(f, g_bin_buffer, f_buf.st_size) ) {
			LOG_E("read firmware bin fail!");
			free(g_bin_buffer);
			return -1;
		}
	}





	rt_device_close(nor_dev);
	close(f);

	return 0;
}
RTM_EXPORT(get_update_state_ext);

int update_firmware_ext(int argc, char **argv)
{
	int f;
	struct rt_device_blk_geometry geometry;
	struct rt_device_blk_write w_info;
	int err = 0;
	static struct tag_eos_firmware *firmware;
	rt_uint32_t erase_addr[2];
	struct stat f_buf = {0};
	char ver_str[96] = {0};

	if (firmware == NULL) {
		f = open("/mnt/sdcard/firmware.bin", O_RDONLY);
		if (f == -1) {
			LOG_E("open firmware fail\n");
			return -1;
		}

		firmware = (struct tag_eos_firmware *)malloc(
							sizeof(struct tag_eos_firmware));
		if (firmware == NULL) {
			LOG_I("malloc fail!");
			return -1;
		}
		memset(firmware, 0, sizeof(struct tag_eos_firmware));
		firmware->f_firmware = f;

		LOG_W("update firmware start...");

		/* find nor device */
		firmware->nor_dev = rt_device_find(NOR_DEV_NAME);
		if (!firmware->nor_dev) {
			LOG_E("failed to find nor device: %s", NOR_DEV_NAME);
			close(firmware->f_firmware);
			free(firmware);
			firmware = NULL;
			return -1;
		}
		rt_device_open(firmware->nor_dev, RT_DEVICE_OFLAG_RDWR);

		/* get flash geometry */
		err = rt_device_control(firmware->nor_dev, RT_DEVICE_CTRL_BLK_GETGEOME,
						&geometry);
		if (err) {
			LOG_E("failed to get flash geometry");
			rt_device_close(firmware->nor_dev);
			close(firmware->f_firmware);
			free(firmware);
			firmware = NULL;
			return -1;
		}

		LOG_W(
		"toatal size: %u(Byte), sector count:%u, sector size:%u, block size:%u",
			geometry.sector_count * geometry.bytes_per_sector,
			geometry.sector_count, geometry.bytes_per_sector,
			geometry.block_size);
		firmware->sector_num = geometry.sector_count;
		firmware->bytes_per_sector = geometry.bytes_per_sector;

		fstat(firmware->f_firmware, &f_buf);
		if (
		f_buf.st_size - firmware->sector_num * firmware->bytes_per_sector < 0 ||
		f_buf.st_size - firmware->sector_num * firmware->bytes_per_sector > 96) {
			LOG_E("firmware file len err,%d, %d", f_buf.st_size,
					firmware->sector_num*firmware->bytes_per_sector);
			rt_device_close(firmware->nor_dev);
			close(firmware->f_firmware);
			free(firmware);
			firmware = NULL;
			return -1;
		}
		if (f_buf.st_size -
				firmware->sector_num * firmware->bytes_per_sector > 0) {
			lseek(firmware->f_firmware,
					firmware->sector_num * firmware->bytes_per_sector,
					SEEK_SET);
			read(firmware->f_firmware, ver_str,
				f_buf.st_size - firmware->sector_num *
							firmware->bytes_per_sector);
			lseek(firmware->f_firmware, 0, SEEK_SET);

			lb_get_cfg_version(firmware->ver_str);
			if (strcmp(ver_str, firmware->ver_str) == 0 &&
					strcmp("spinor", boot_get_boot_type()) == 0) {
				LOG_E("version same!");
				rt_device_close(firmware->nor_dev);
				close(firmware->f_firmware);
				free(firmware);
				firmware = NULL;
				return -1;
			}
		}
		
		firmware->bin_buffer = malloc(f_buf.st_size);
		if (firmware->bin_buffer == NULL) {
			LOG_E("malloc fail!");
			rt_device_close(firmware->nor_dev);
			close(firmware->f_firmware);
			free(firmware);
			firmware = NULL;
			return -1;
		} else {
			if ( f_buf.st_size != read(firmware->f_firmware, firmware->bin_buffer, f_buf.st_size) ) {
				LOG_E("read firmware bin fail!");
				rt_device_close(firmware->nor_dev);
				close(firmware->f_firmware);
				free(firmware->bin_buffer);
				free(firmware);
				firmware = NULL;
				return -1;
			}
			firmware->bin_cnt = 0;
		}

		/* chip erase */
		erase_addr[0] = 0;
		erase_addr[1] = geometry.sector_count;
		err = rt_device_control(firmware->nor_dev, RT_DEVICE_CTRL_BLK_ERASE,
					erase_addr);
		if (err) {
			LOG_E("failed to erase chip");
			rt_device_close(firmware->nor_dev);
			close(firmware->f_firmware);
			free(firmware);
			firmware = NULL;
			return -1;
		}
		LOG_W("version %s to %s", firmware->ver_str, ver_str);
		firmware->percent = 0;
	} else {
		if (firmware->percent == 100) {
			rt_thread_delay(50);
			if (g_sd_plugout) {
				rt_thread_delay(20);
				lb_hw_reboot();
				return 100;
			}
			return 100;
		}
		firmware->sector_cnt = READ_BUF_SIZE / firmware->bytes_per_sector;
		if (firmware->sector_start + firmware->sector_cnt > firmware->sector_num)
			firmware->sector_cnt = firmware->sector_num -
								firmware->sector_start;

		
		//read(firmware->f_firmware, firmware->buf,
		//	firmware->sector_cnt*firmware->bytes_per_sector);
		memcpy(firmware->buf, firmware->bin_buffer + firmware->bin_cnt, firmware->sector_cnt*firmware->bytes_per_sector);
		firmware->bin_cnt += firmware->sector_cnt*firmware->bytes_per_sector;
			
			
		w_info.pos = firmware->sector_start;
		w_info.buffer = firmware->buf;
		w_info.size = firmware->sector_cnt;
		rt_device_control(firmware->nor_dev, RT_DEVICE_CTRL_WRITE_BLK, &w_info);
		/* rt_device_write(firmware->nor_dev, firmware->sector_start,
						firmware->buf, firmware->sector_cnt); */
		rt_device_read(firmware->nor_dev, firmware->sector_start,
						firmware->buf_temp, firmware->sector_cnt);
		if (memcmp(firmware->buf, firmware->buf_temp,
				firmware->sector_cnt*firmware->bytes_per_sector)) {
			LOG_E("write fail: %d, %d", firmware->sector_start,
					firmware->sector_cnt);
			return firmware->percent;
		}

		firmware->sector_start += firmware->sector_cnt;

		firmware->percent = (100*firmware->sector_start)/firmware->sector_num;
	}

	LOG_W("update firmware: %d", firmware->percent);
	return firmware->percent;
}
RTM_EXPORT(update_firmware_ext);

void data_regionalization(void)
{
 dfs_unmount("/mnt/data"); 
 dfs_mkfs("elm", "data"); 
 dfs_mount("data", "/mnt/data", "elm", 0, NULL); 
}
RTM_EXPORT(data_regionalization);

int main(void)
{
#ifdef ARCH_LOMBO_N7
#ifdef RT_USING_EGUI
#ifdef RT_SEL_CAR_VIDEO_CASE
	LOG_W("%s :%d start time: %dms boot from %d boot type:%s\n", __FILE__, __LINE__,
	rt_time_get_msec(), is_gs_wakeup_boot(), boot_get_boot_type());
	lb_int32 ret;

	vcom_adjust_start();
	
 #if 1//flash_jiami
	extern int nor_data_sample(void);
	if(nor_data_sample())
	 {
		return 0;
	 }
  #endif

	ret = lb_get_cfg_accpower();
	if (ret != -1)
		set_acc_line(ret);

	if (get_acc_sio_val() == 0) {
		int file;
		file = open("/mnt/data/acc.bin", O_RDONLY);
		if (file != -1) {
			close(file);
			remove("/mnt/data/acc.bin");
			csp_rtc_pm_int_enable(PM_TYPE_RING_KEY, 1);
			rt_pm_shutdown();
		}
	}

	if (get_bat_level() != 0) {
#ifdef LOMBO_GSENSOR
		if (is_gs_wakeup_boot() == 1) {
			if (get_bat_level() == 1) {
				printf("low power %s :%d\n", __FILE__, __LINE__);
				rt_pm_shutdown();

				rt_object_detach((rt_object_t)rt_thread_self());
				return 0;
			}

			lb_ui_customize_init();
			park_monitor_create();
			park_monitor_stop();
			LOG_I("%s :%d\n", __FILE__, __LINE__);
			rt_pm_shutdown();

			rt_object_detach((rt_object_t)rt_thread_self());
			return 0;
		}
#endif
		rt_pm_shutdown();

		rt_object_detach((rt_object_t)rt_thread_self());
		return 0;
	} else {
#ifdef LOMBO_GSENSOR
		if (is_gs_wakeup_boot() == 1 && !get_acc_sio_val()) {
			
			lb_ui_customize_init();
			park_monitor_create();
			park_monitor_stop();
			LOG_I("%s :%d\n", __FILE__, __LINE__);
			rt_pm_shutdown();

			rt_object_detach((rt_object_t)rt_thread_self());
			return 0;
		}
#endif
#ifdef ARCH_LOMBO_N7V1_TDR
		lb_ui_rotate(0);
#else
		lb_ui_rotate(3);
#endif
		lb_ui_init();
		ret = lb_app_open(&home, ROOTFS_MOUNT_PATH"/apps/home.app", 0);
		if (ret < 0)
			LOG_E("ret:%d\n", ret);
#ifdef CPUFREQ_GOVERNOR_ADAPTIVE
		cpufreq_adaptive_init();
#endif
#ifdef LOMBO_CPUFREQ_COOLING
		lombo_thermal_init();
#endif
		lb_ui_start();

		/* show logo and hidden logo */
		ret = lb_app_ctrl(&home, 0x300, 0, 0);
		lb_system_thread_init();
#ifdef ARCH_LOMBO_N7V1_TDR
		lb_system_mq_send(LB_SYSMSG_AV_PLUGIN, 0, 0, 0);
#endif
		}
#else
#ifdef RT_USING_EGUI
	/* demo */
	/* application_main(0, RT_NULL); */
#endif
#endif
#endif
#endif

	/*
	 * main thread is created by rt_thread_create, its stack and thread object
	 * need to be freed after thread exit. so detach thread obj here, to let
	 * the rt_thread_exit add it to defunct thread list, and then daemon thread
	 * free its stack and thread object.
	 */
	rt_object_detach((rt_object_t)rt_thread_self());
	return 0;
}

#endif /* APPLICATION_C */
