/*
 * media_dev.h - camera top interface head file
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

#ifndef __MEDIA_DEV_H__
#define __MEDIA_DEV_H__

#include "viss.h"

struct lombo_viss {
	rt_mutex_t lock;
	struct viss_dev *vdev;
	struct viss_dev *isp;
	struct viss_dev *csi_dev;
	struct viss_dev *prev_dev;
	struct viss_dev *cap_dev;
	struct video_dev *sensor;
	rt_uint32_t isp_ch;
};

struct viss_dev *viss_md_create_links(rt_uint32_t type);
#ifdef ARCH_LOMBO_N7V1_CDR
struct vic_det_dev *viss_det_create_links(void);
void viss_det_destory_links(struct vic_det_dev *vdev);
struct vic_det_status viss_det_get_sta(void);
#endif
void viss_md_destory_links(struct viss_dev *vdev);
rt_err_t viss_md_sensor_debug(struct video_dev *sensor,
					struct viss_dbg_register *reg, rt_int32_t rw);
void viss_md_sensor_exit(struct video_dev *sensor);
void viss_md_dev_en(struct viss_dev *vdev, rt_int32_t en);
void viss_md_sensor_get_all_mode(struct video_dev *sensor,
						struct dev_all_mode *all);
struct dev_mode *viss_md_sensor_get_cur_mode(struct video_dev *sensor);
rt_err_t viss_md_sensor_get_info(struct video_dev *sensor,
				struct viss_source_info *info);
rt_err_t viss_md_sensor_prepare(struct video_dev *sensor);
rt_err_t viss_md_sensor_init(struct video_dev *sensor);
rt_err_t viss_md_sensor_ioctl(struct video_dev *sensor,
		rt_int32_t cmd, void *para);
rt_err_t viss_md_sensor_set_mode(struct video_dev *sensor,
					rt_int32_t index);
rt_err_t viss_md_streamoff(struct viss_dev *vdev, rt_int32_t ch);
rt_err_t viss_md_streamon(struct viss_dev *vdev, rt_bool_t isp_out,
				rt_int32_t ch);
rt_err_t viss_md_sensor_standby(struct video_dev *sensor, rt_bool_t enable);
void viss_md_top_clk_exit(void);
void viss_md_top_clk_init(void);
void viss_md_set_mclk(struct video_dev *sensor, rt_uint32_t freq);
struct viss_dev *viss_md_pipeline_get_entity(rt_uint32_t type);
rt_err_t viss_md_pipeline_create(rt_uint32_t type);
rt_err_t viss_md_pipeline_free(rt_uint32_t type);
rt_err_t viss_md_pipeline_init(rt_uint32_t type);
rt_err_t viss_md_pipeline_exit(rt_uint32_t type);
rt_err_t viss_md_pipeline_streamon(rt_uint32_t type, rt_int32_t ch);
rt_err_t viss_isp_pipeline_streamon(rt_int32_t ch);
rt_err_t viss_md_pipeline_streamoff(rt_uint32_t type, rt_int32_t ch);
rt_err_t viss_md_pipeline_stop_hw(rt_uint32_t type, rt_int32_t ch);
rt_err_t viss_md_pipeline_restart_dma();
rt_err_t viss_md_init();
rt_err_t viss_md_deinit();
struct viss_dev *viss_md_get_isp_dev();
struct video_dev *viss_md_get_isp_sensor();
struct viss_dev *viss_md_get_csi_dev();


#endif /* __MEDIA_DEV_H__ */
